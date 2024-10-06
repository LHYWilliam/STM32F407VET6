#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "Serial.h"
#include "Signal.h"
#include "Key.h"

extern LED_Handler LED0;
extern Key_Handler Key0;
extern Serial_Handler Serial;

extern SignalSampler_Handler Sampler;

void vLEDTimerCallback(TimerHandle_t xTimer) {
    LED_Toggle(&LED0);

    UNUSED(xTimer);
}

void vKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&Key0) == KeyDown) {
            for (uint32_t i = 0; i < Sampler.Length; i ++) {
                Serial_Printf(&Serial, "%f\r\n", Sampler.Data[i] / 4095.);
            }
        }
    }
}

void vApplicationTickHook() {}