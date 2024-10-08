#include "FreeRTOS.h"
#include "timers.h"

#include "lvgl.h"

#include "ADC.h"
#include "Key.h"
#include "LED.h"
#include "Serial.h"
#include "Signal.h"

extern LED_t LED0;
extern Key_t Key0;
extern Serial_t Serial;

extern SignalSampler_t Sampler;

void vLEDTimerCallback(TimerHandle_t xTimer) { LED_Toggle(&LED0); }

void vKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&Key0) == KeyDown) {
            for (uint32_t i = 0; i < Sampler.Length; i++) {
                Serial_Printf(&Serial, "%d\r\n", Sampler.Data[i]);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLVGLTaskCode(void *pvParameters) {

    for (;;) {
        lv_timer_handler();

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vApplicationTickHook() {
    HAL_IncTick();
    // lv_tick_inc(1);
}