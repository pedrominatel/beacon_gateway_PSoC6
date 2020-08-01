#include "common.h"

#define EDDY_BYTE_16B_UID	0xaa
#define EDDY_BYTE_UID_0FE	0xfe
#define EDDY_BYTE_16B_CPT	0x03

void eddy_decodeUID(uint8_t *pkt);
void eddy_printRaw(uint8_t *pkt);
