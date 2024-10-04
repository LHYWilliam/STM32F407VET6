#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"

extern LED LED0;

void vLEDTimerCallback(TimerHandle_t xTimer) { LED_Toggle(&LED0); }
