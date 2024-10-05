#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"

extern LED_Handler LED0;

void vLEDTimerCallback(TimerHandle_t xTimer) {
    LED_Toggle(&LED0);

    UNUSED(xTimer);
}
