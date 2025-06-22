#ifndef SIGNAL_H
#define SIGNAL_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "ADC.h"
#include "DMA.h"
#include "Timer.h"

typedef struct {
    uint32_t *Data;
    uint32_t Index;
    uint32_t Length;

    ADC_t ADC;
    DMA_t DMA;
    Timer_t Timer;
} Sampler_t;

void Sampler_Init(Sampler_t *Self);
void Sampler_GetValue(Sampler_t *Self);
void Sampler_UpdateIndex(Sampler_t *Self);

#endif