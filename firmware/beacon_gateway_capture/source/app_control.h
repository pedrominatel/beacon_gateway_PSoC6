#include "common.h"

#define TURN_ON_TIMER_PERIOD            (9999)
#define LED_BLINK_TIMER_CLOCK_HZ        (10000)

void app_controlLoadChannel(uint8_t loadCh, uint8_t state);
void app_controlInit(void);

void app_timerLoadAReset(void);
void app_timerLoadBReset(void);
