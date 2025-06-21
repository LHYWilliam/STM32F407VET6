#include "FreeRTOS.h"
#include "GPIO.H"
#include "timers.h"

#include "GPIO.h"

extern uint32_t LED0_ODR;
extern uint32_t LED1_ODR;
extern uint32_t Key_IDR;

void vMainTaskCode(void *pvParameters) {
    for (;;) {
        if (GPIO_ReadInput(Key_IDR)) {
            GPIO_Write(LED1_ODR, 0);
        } else {
            GPIO_Write(LED1_ODR, 1);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    GPIO_Toggle(LED0_ODR);
}

void vApplicationTickHook() { HAL_IncTick(); }