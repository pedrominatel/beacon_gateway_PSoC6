#include "eddystone_uid.h"

//Namespace shared key for testing
// 6ea80fc93b138e6d9178
const uint8_t ns_key[10] =
{
    0x6e, 0xa8, 0x0f, 0xc9, 0x3b, 0x13, 0x8e, 0x6d, 0x91, 0x78
};

//Function to print the raw package from scan
void eddy_printRaw(uint8_t *pkt)
{

	uint8_t index=0;
    int fieldLength=pkt[index];

    do
	{
		for(int i=0;i<=fieldLength;i++)
		printf("%02X ",pkt[index+i]);
		index = index + fieldLength + 1;
    fieldLength = pkt[index];

    }
	while(fieldLength);
    printf("\n");

}

/* Filter by MAC (not secure) */
uint8_t eddy_macFilter(wiced_bt_device_address_t mac_addr)
{

	uint32_t spot_vendor = 0 | (mac_addr[0] << 16) | (mac_addr[1] << 8) | mac_addr[2];
    uint32_t spot_uid = 0 | (mac_addr[3] << 16) | (mac_addr[4] << 8) | mac_addr[5];

	for (size_t i = 0; i < MAC_SAFE_LIST_NUM; i++)
	{
		if(spot_vendor == mac_safeVendor[i]){
			printf("MAC on the list!\n");
			//printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
		}
	}
}

/* Decode the beacon frame to detect */
void eddy_decodeUID(uint8_t *pkt)
{

	//Filter the Eddystone frame
	if((pkt[2]==EDDY_BYTE_16B_UID)&&pkt[6]==EDDY_BYTE_16B_UID)
	{
		//Print the raw package
		eddy_printRaw(pkt);

		//Check the shared KEY (name space id)
		//From byte 10 to 19 (10 bytes total)
		if(memcmp(ns_key, pkt + 10, 10)==0)
		{
			//printf("Key match!\n");
			cyhal_gpio_write(LED_G, CYBSP_LED_STATE_OFF);
			cyhal_gpio_write(LED_B, CYBSP_LED_STATE_ON);

			if(pkt[EDYY_PROTO_LOADCONTROL_EN]==true)
			{
				//Filter the package protocol to control load A or B
				if(pkt[EDYY_PROTO_LOADCONTROL_CH]==0x01){
					app_controlLoadChannel(LOAD_A, CYBSP_LED_STATE_OFF);
					app_timerLoadAReset();
				} else if(pkt[EDYY_PROTO_LOADCONTROL_CH]==0x02){
					app_controlLoadChannel(LOAD_B, CYBSP_LED_STATE_OFF);
					app_timerLoadBReset();
				}
			}
		}

		//Check if the beacon is to publish over MQTT
		if(pkt[EDYY_PROTO_LOADCONTROL_MQTT]==true)
		{
			//Publish to MQTT
		}
	}
}
