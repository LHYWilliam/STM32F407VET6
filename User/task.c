#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "Serial.h"
#include "Signal.h"
#include "Key.h"

extern LED_Handler LED0;
extern Key_Handler key0;
extern Serial_Handler serial;

extern SignalSampler_Handler sampler;

void vLEDTimerCallback(TimerHandle_t xTimer) {
    LED_Toggle(&LED0);

    UNUSED(xTimer);
}

void vKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&key0) == KeyDown) {
            for (uint32_t i = 0; i < sampler.Length; i ++) {
                Serial_Printf(&serial, "%f\r\n", sampler.Data[i] / 4095.);
            }
        }
    }
}

void vApplicationTickHook() {}