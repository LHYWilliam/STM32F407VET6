#include "FreeRTOS.h"
#include "timers.h"

#include "lvgl.h"

#include "LED.h"

extern LED_Handler LED0;

void vLEDTimerCallback(TimerHandle_t xTimer) {
    LED_Toggle(&LED0);

    UNUSED(xTimer);
}

void vLVGLTimerCallback(TimerHandle_t xTimer) {
    lv_timer_handler();
    
    UNUSED(xTimer);

}

void vApplicationTickHook() { lv_tick_inc(1); }