#include "app_bt_cfg.h"
#include "wiced_bt_stack.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h>
#include "cybt_platform_trace.h"
#include "wiced_memory.h"
#include "stdio.h"
#include "beacon_utils.h"
#include "timers.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "mqtt_task.h"

#define LED_B				(P11_1)
#define LED_R				(P0_3)
#define LED_G				(P1_1)
#define LOAD_A				(P13_2)
#define LOAD_B				(P13_3)
#define LOAD_AA				(P13_4)
#define LOAD_BB				(P13_5)
