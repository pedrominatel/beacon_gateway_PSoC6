/******************************************************************************
* File Name: main.c
*
* Description:This is the source code for the AnyCloud: Multi Beacon Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************/
/*******************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
*        Header Files
*******************************************************************************/
#include "common.h"
#include "app_control.h"

/*******************************************************************************
*        Macro Definitions
*******************************************************************************/
/* Minimum and maximum ADV interval */
#define ADVERT_INTERVAL_MIN (20)
#define ADVERT_INTERVAL_MAX (100)

/*******************************************************************************
*        Variable Definitions
*******************************************************************************/

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
static void                   ble_address_print              (wiced_bt_device_address_t bdadr);

/* Callback function for Bluetooth stack management type events */
static wiced_bt_dev_status_t  app_bt_management_callback (wiced_bt_management_evt_t event,
                                            wiced_bt_management_evt_data_t *p_event_data);

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
int main()
{
    cy_rslt_t result ;

    /* Initialize the board support package */
    if(CY_RSLT_SUCCESS != cybsp_init())
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,\
                        CY_RETARGET_IO_BAUDRATE);

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    cybt_platform_config_init(&bt_platform_cfg_settings);

    printf("***********  Beacon Gateway  ***********\n");
    printf("****Beacon Gateway Application Start****\n");
    printf("****************************************\n\n");

    app_controlInit();

    /* Register call back and configuration with stack */
    result = wiced_bt_stack_init(app_bt_management_callback, &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if( WICED_BT_SUCCESS == result)
    {
        printf("Bluetooth Stack Initialization Successful \n");
        cyhal_gpio_write(LED_G, CYBSP_LED_STATE_ON);
    }
    else
    {
        printf("Bluetooth Stack Initialization failed!! \n");
        CY_ASSERT(0);
    }

    //Start the MQTT client task
    //Facing some issues...
    //xTaskCreate(mqtt_client_task, "MQTT Client task", MQTT_CLIENT_TASK_STACK_SIZE,
    //            NULL, MQTT_CLIENT_TASK_PRIORITY, NULL);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    /* Should never get here */
    CY_ASSERT(0) ;
}

static void scan_result_callback(wiced_bt_ble_scan_results_t *scan_res, uint8_t *p_adv_data)
{
    //Filter the RSSI to detect only close to the gateway
	if(scan_res->rssi >-80){
        //Light LED on/off
        cyhal_gpio_write(LED_G, CYBSP_LED_STATE_ON);
        cyhal_gpio_write(LED_B, CYBSP_LED_STATE_OFF);
        //printf("RSSI: %d\n", scan_res->rssi);
		eddy_decodeUID(p_adv_data);
        
        //Check is the vendor is on the list
        eddy_macFilter(scan_res->remote_bd_addr);
	}
}


/**************************************************************************************************
* Function Name: app_bt_management_callback()
***************************************************************************************************
* Summary:
*   This is a Bluetooth stack event handler function to receive management events from
*   the BLE stack and process as per the application.
*
* Parameters:
*   wiced_bt_management_evt_t event             : BLE event code of one byte length
*   wiced_bt_management_evt_data_t *p_event_data: Pointer to BLE management event structures
*
* Return:
*  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
*
*************************************************************************************************/
wiced_result_t app_bt_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t status = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda = { 0 };
    wiced_bt_ble_advert_mode_t *p_adv_mode = NULL;

    printf("BT Event %d\r\n", event);

    switch (event)
    {
    case BTM_ENABLED_EVT:
        if( WICED_BT_SUCCESS == p_event_data->enabled.status )
        {
            printf("Bluetooth Enabled\r\n");
            wiced_bt_dev_read_local_addr(bda);
            printf("Local Bluetooth Address: ");
            //Start scan passive
            wiced_bt_ble_observe(WICED_TRUE, 0, scan_result_callback);
        }
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:

        /* Advertisement State Changed */
        printf("Advertisement...\n");
        p_adv_mode = &p_event_data->ble_advert_state_changed;
        if (BTM_BLE_ADVERT_OFF == *p_adv_mode)
        {
            printf("Advertisement turned off\n");
        }
        else
        {
            /* Advertisement Started */
            printf("Advertisement turned on\n");
        }
        break;

    default:
        break;
    }

    return status;
}
