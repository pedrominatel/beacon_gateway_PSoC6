#include "common.h"

#define EDDY_BYTE_16B_UID	            0xaa
#define EDDY_BYTE_UID_0FE	            0xfe
#define EDDY_BYTE_16B_CPT	            0x03

#define EDYY_PROTO_LOADCONTROL_EN       20
#define EDYY_PROTO_LOADCONTROL_CH       21
#define EDYY_PROTO_LOADCONTROL_MQTT     22

#define MAC_SAFE_LIST_NUM               4

extern const uint32_t mac_safeVendor[MAC_SAFE_LIST_NUM] = {
    0x3868A4, 0xE509F0, 0xEEE0CF, 0xAC233F,
};

void eddy_decodeUID(uint8_t *pkt);
void eddy_printRaw(uint8_t *pkt);
uint8_t eddy_macFilter(wiced_bt_device_address_t mac_addr);
