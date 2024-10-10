#ifndef SIGNAL_H
#define SIGNAL_H

#include "stm32f4xx_hal.h"

#include "ADC.h"
#include "DAC.h"

typedef struct {
    uint32_t *Data;
    uint32_t Length;

    DAC_t DAC;
    DMA_t DMA;
    Timer_t Timer;
} SignalGenerator_t;

typedef struct {
    uint32_t *Data;
    uint32_t Length;

    ADC_t ADC;
    DMA_t DMA;
    Timer_t Timer;
} SignalSampler_t;

void SignalGenerator_Init(SignalGenerator_t *self);
void SignalGenerator_Start(SignalGenerator_t *self, uint32_t *data,
                           uint32_t length);

void SignalSampler_Init(SignalSampler_t *self);
void SignalSampler_Start(SignalSampler_t *self, uint32_t *data,
                         uint32_t length);

void Sin_Generate(uint32_t *data, uint32_t length);

#endif