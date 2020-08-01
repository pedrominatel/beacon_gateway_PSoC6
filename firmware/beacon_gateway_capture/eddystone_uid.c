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

void eddy_decodeUID(uint8_t *pkt){


	if((pkt[2]==EDDY_BYTE_16B_UID)&&pkt[6]==EDDY_BYTE_16B_UID){

		eddy_printRaw(pkt);

		if(pkt[20]==0xca){
			app_controlLoadChannel(LOAD_A, CYBSP_LED_STATE_OFF);
			app_timerLoadAReset();
			//Start timer
		} else if(pkt[20]==0xfa){
			app_controlLoadChannel(LOAD_B, CYBSP_LED_STATE_OFF);
			app_timerLoadBReset();
			//Start timer
		}

	}



}
