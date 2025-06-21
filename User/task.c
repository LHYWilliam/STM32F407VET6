#include "FreeRTOS.h"
#include "GPIO.H"
#include "timers.h"

#include "GPIO.h"
#include "LED.h"

extern LED_t LED0;
extern LED_t LED1;
extern uint32_t Key_IDR;

void vMainTaskCode(void *pvParameters) {
    for (;;) {
        if (GPIO_ReadInput(Key_IDR)) {
            LED_On(&LED1);
        } else {
            LED_Off(&LED1);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED0); }

void vApplicationTickHook() { HAL_IncTick(); }