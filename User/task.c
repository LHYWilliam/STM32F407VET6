#include "FreeRTOS.h"
#include "timers.h"

#include "LED.h"
#include "Serial.h"
#include "ADC.h"

extern LED LED0;
extern Serial serial;

extern uint32_t ADCValue[];
extern mADC adc;

void vLEDTimerCallback(TimerHandle_t xTimer) { LED_Toggle(&LED0); }

void vSerialTimerCallback(TimerHandle_t xTimer) {
    static uint32_t i = 0;
    Serial_Printf(&serial, "%d\r\n", ADCValue[i]);
    i = (i + 1) % adc.Length;
}
