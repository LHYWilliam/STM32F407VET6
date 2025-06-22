#include "FreeRTOS.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"
#include "PWM.h"
#include "Serial.h"

extern LED_t LED0;
extern LED_t LED1;
extern Key_t Key0;
extern Serial_t Serial;
extern PWM_t PWM;

void vMainTaskCode(void *pvParameters) {
    for (;;) {
        // if (Key_IsPressing(&Key0)) {
        //     LED_On(&LED1);
        // } else {
        //     LED_Off(&LED1);
        // }

        // vTaskDelay(pdMS_TO_TICKS(100));

        for (uint16_t i = 0; i < 1000; i++) {
            PWM_SetSetCompare(&PWM, 2, i);
            PWM_SetSetCompare(&PWM, 3, 1000 - i);
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        for (uint16_t i = 1000; i > 0; i--) {
            PWM_SetSetCompare(&PWM, 2, i);
            PWM_SetSetCompare(&PWM, 3, 1000 - i);
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // Serial_Printf(&Serial, "LED toggle at %d ms\r\n",
    //               xTaskGetTickCount() * portTICK_PERIOD_MS);
    // LED_Toggle(&LED0);
}

void vApplicationTickHook() { HAL_IncTick(); }