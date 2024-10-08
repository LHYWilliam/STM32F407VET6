#ifndef SIGNAL_H
#define SIGNAL_H

#include "stm32f4xx_hal.h"

#include "ADC.h"
#include "DAC.h"

typedef struct {
    uint32_t *Data;
    uint32_t Length;

    DAC_Handler DAC;
    DMA_Handler DMA;
    Timer_Handler Timer;
} SignalGenerator_Handler;

typedef struct {
    uint32_t *Data;
    uint32_t Length;

    ADC_Handler ADC;
    DMA_Handler DMA;
    Timer_Handler Timer;
} SignalSampler_Handler;

void SignalGenerator_Init(SignalGenerator_Handler *self);
void SignalGenerator_Start(SignalGenerator_Handler *self, uint32_t *data,
                           uint32_t length);

void SignalSampler_Init(SignalSampler_Handler *self);
void SignalSampler_Start(SignalSampler_Handler *self, uint32_t *data,
                         uint32_t length);

void Sin_Generate(uint32_t *data, uint32_t length);

#endif