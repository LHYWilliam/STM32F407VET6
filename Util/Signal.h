#ifndef SIGNAL_H
#define SIGNAL_H

#include CMSIS_device_header

#include "ADC.h"
#include "DAC.h"
#include "DMA.h"
#include "Timer.h"

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

void SignalSampler_Init(SignalSampler_t *self);
uint32_t SignalSampler_Get(SignalSampler_t *self);

void Sin_Generate(uint32_t *data, uint32_t length);

#endif