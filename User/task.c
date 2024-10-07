#include "FreeRTOS.h"
#include "timers.h"

#include "lvgl.h"

#include "LED.h"

extern LED_Handler LED0;

void vLEDTimerCallback(TimerHandle_t xTimer) { LED_Toggle(&LED0); }

void vLVGLTaskCode(void *pvParameters) {

    for (;;) {
        lv_timer_handler();

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vApplicationTickHook() {
    // lv_tick_inc(1);
}