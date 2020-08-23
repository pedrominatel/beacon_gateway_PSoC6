#include "eddystone_uid.h"

void eddy_printRaw(uint8_t *pkt){

	uint8_t index=0;
    int fieldLength=pkt[index];

    do {
		for(int i=0;i<=fieldLength;i++)
		printf("%02X ",pkt[index+i]);
		index = index + fieldLength + 1;
    fieldLength = pkt[index];

    } while(fieldLength);
    printf("\n");

}

/* Decode the beacon frame to detect */
void eddy_decodeUID(uint8_t *pkt){

	//Filter the Eddystone frame
	if((pkt[2]==EDDY_BYTE_16B_UID)&&pkt[6]==EDDY_BYTE_16B_UID){
		//Print the raw package
		eddy_printRaw(pkt);

		//Check the shared KEY (name space id)
		//From byte 10 to 19 (10 bytes total)
		cyhal_gpio_write(LED_B, CYBSP_LED_STATE_OFF);
		cyhal_gpio_write(LED_G, CYBSP_LED_STATE_ON);

		if(pkt[EDYY_PROTO_LOADCONTROL_EN]==true){
			//Filter the package protocol to control load A or B
			if(pkt[EDYY_PROTO_LOADCONTROL_CH]==0x01){
				app_controlLoadChannel(LOAD_A, CYBSP_LED_STATE_OFF);
				app_timerLoadAReset();
			} else if(pkt[EDYY_PROTO_LOADCONTROL_CH]==0x02){
				app_controlLoadChannel(LOAD_B, CYBSP_LED_STATE_OFF);
				app_timerLoadBReset();
			}
		}

		//Check if the beacon is to publish over MQTT
		if(pkt[EDYY_PROTO_LOADCONTROL_MQTT]==true){
			//Publish to MQTT
		}
	}
}
