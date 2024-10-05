#include "FreeRTOS.h"
#include "timers.h"

#include "ADC.h"
#include "LED.h"
#include "Serial.h"

extern LED_Handler LED0;
extern Serial_Handler serial;

extern uint32_t ADCValue[];
extern ADC_Handler adc;

void vLEDTimerCallback(TimerHandle_t xTimer) {
    LED_Toggle(&LED0);

    UNUSED(xTimer);
}

void vSerialTimerCallback(TimerHandle_t xTimer) {
    static uint32_t i = 0;
    Serial_Printf(&serial, "%d\r\n", ADCValue[i]);
    i = (i + 1) % adc.Length;

    UNUSED(xTimer);
}
