#include "FreeRTOS.h"
#include "timers.h"

#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "Serial.h"
#include "Signal.h"
#include <stdint.h>

extern LED_t LED0;
extern Key_t Key0;
extern Key_t Key1;
extern Serial_t Serial;
extern LCD_t LCD;

extern Sampler_t Sampler;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED0); }

void vKeyTimerCallback(TimerHandle_t pxTimer) {
    Key_Update(&Key0, 10);
    Key_Update(&Key1, 10);
}

void vKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&Key0) == KeyEvent_Click) {
            for (uint32_t i = 0; i < Sampler.Length; i++) {
                Serial_Printf(&Serial, "%d\r\n", Sampler.Data[i]);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vApplicationTickHook() { HAL_IncTick(); }