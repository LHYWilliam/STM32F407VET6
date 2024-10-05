#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "Serial.h"

extern LED LED0;
extern Serial serial;

extern uint32_t ADCValue[];
extern const uint32_t ADC_DataLength;

void vLEDTimerCallback(TimerHandle_t xTimer) { LED_Toggle(&LED0); }

void vSerialTimerCallback(TimerHandle_t xTimer) {
    static uint32_t i = 0;
    Serial_Printf(&serial, "%d\r\n", ADCValue[i]);
    i = (i + 1) % ADC_DataLength;
}
