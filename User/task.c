#include "FreeRTOS.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"

extern LED_t LED0;
extern LED_t LED1;
extern Key_t Key0;

void vMainTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_IsPressing(&Key0)) {
            LED_On(&LED1);
        } else {
            LED_Off(&LED1);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED0); }

void vApplicationTickHook() { HAL_IncTick(); }