#include "app_control.h"


TimerHandle_t xTimer_LoadA_tmr;
TimerHandle_t xTimer_LoadB_tmr;
#define IDLE_INTERVAL   (portMAX_DELAY)
#define LOAD_TIMER_OFF  (pdMS_TO_TICKS(10000u)) //Timer off 1 minute
#define LOAD_TIMER_STP  (pdMS_TO_TICKS(1000u)) //Timer off 1 minute

void app_controlLoadChannel(uint8_t loadCh, uint8_t state){

	cyhal_gpio_write(loadCh, state);

}

void app_controlLoadClear(void){

	app_controlLoadChannel(LOAD_A, CYBSP_LED_STATE_OFF);
	app_controlLoadChannel(LOAD_B, CYBSP_LED_STATE_OFF);
	app_controlLoadChannel(LOAD_AA, CYBSP_LED_STATE_OFF);
	app_controlLoadChannel(LOAD_BB, CYBSP_LED_STATE_OFF);

}


//cyhal_gpio_write(LED_G, CYBSP_LED_STATE_ON);
//cyhal_gpio_write(LED_B, CYBSP_LED_STATE_OFF);
//cyhal_gpio_write(LED_G, CYBSP_LED_STATE_OFF);
//cyhal_gpio_write(LED_B, CYBSP_LED_STATE_ON);

void app_controlInit(void){

	//GPIO initialization for the board RGB LED
    cyhal_gpio_init(LED_R, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    cyhal_gpio_init(LED_G, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    cyhal_gpio_init(LED_B, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    //GPIO initialization for the load control board
    cyhal_gpio_init(LOAD_A, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    cyhal_gpio_init(LOAD_AA, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    cyhal_gpio_init(LOAD_B, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    cyhal_gpio_init(LOAD_BB, CYHAL_GPIO_DIR_OUTPUT,
                             CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);

    printf("Starting Timer\n");
    app_startTimerLoadA();
    app_startTimerLoadB();

}

void app_timerReset(TimerHandle_t xTimer){
    /* or more simply and equivalently
    "if( xTimerIsTimerActive( xTimer ) )" */
    if( xTimerIsTimerActive(xTimer) != pdFALSE )
    {
    	xTimerStop(xTimer, 0);
    }
    else
    {
        /* xTimer is not active, change its period to 500ms.  This will also
        cause the timer to start.  Block for a maximum of 100 ticks if the
        change period command cannot immediately be sent to the timer
        command queue. */
        if(xTimerChangePeriod(xTimer, LOAD_TIMER_OFF, 100 )
                                                            == pdPASS )
        {
            /* The command was successfully sent. */
        }
        else
        {
            /* The command could not be sent, even after waiting for 100 ticks
            to pass.  Take appropriate action here. */
        }
    }
}

void app_timerLoadAReset(void){
	app_timerReset(xTimer_LoadA_tmr);
	xTimerStart(xTimer_LoadA_tmr, 0u);

}

void app_timerLoadBReset(void){
	app_timerReset(xTimer_LoadB_tmr);
	xTimerStart(xTimer_LoadB_tmr, 0u);
}

static void LoadATimerCallback(TimerHandle_t xTimer)
{
	printf("Timer load A\n");
	app_controlLoadChannel(LOAD_A, CYBSP_LED_STATE_ON);
	xTimerStop(xTimer_LoadA_tmr, 0);

}

static void LoadBTimerCallback(TimerHandle_t xTimer)
{
	printf("Timer load B\n");
	app_controlLoadChannel(LOAD_B, CYBSP_LED_STATE_ON);
	xTimerStop(xTimer_LoadB_tmr, 0);

}

void app_startTimerLoadA(void){
    /* Variable used to store the return values of RTOS APIs */
    BaseType_t rtosApiResult;

    /* Create an RTOS timer */
    xTimer_LoadA_tmr    =  xTimerCreate("loadA_tmr",
    										LOAD_TIMER_OFF, pdTRUE,
                                            NULL, LoadATimerCallback);

    /* Make sure that timer handle is valid */
    if (xTimer_LoadA_tmr != NULL)
    {
        /* Start the timer */
        rtosApiResult = xTimerStart(xTimer_LoadA_tmr, 0u);
        /* Check if the operation has been successful */
        if(rtosApiResult != pdPASS)
        {
        	printf("Timer OK\n");
        }
    }
    else
    {
    	printf("Timer Error\n");
    }
}

void app_startTimerLoadB(void){
    /* Variable used to store the return values of RTOS APIs */
    BaseType_t rtosApiResult;

    /* Create an RTOS timer */
    xTimer_LoadB_tmr    =  xTimerCreate("loadB_tmr",
    										LOAD_TIMER_OFF, pdTRUE,
                                            NULL, LoadBTimerCallback);

    /* Make sure that timer handle is valid */
    if (xTimer_LoadB_tmr != NULL)
    {
        /* Start the timer */
        rtosApiResult = xTimerStart(xTimer_LoadB_tmr, 0u);
        /* Check if the operation has been successful */
        if(rtosApiResult != pdPASS)
        {
        	printf("Timer OK\n");
        }
    }
    else
    {
    	printf("Timer Error\n");
    }
}

