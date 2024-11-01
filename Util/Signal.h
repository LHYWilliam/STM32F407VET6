#ifndef SIGNAL_H
#define SIGNAL_H

#include CMSIS_device_header

#ifdef __cplusplus
extern "C" {
#endif

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
} Generator_t;

typedef struct {
    uint32_t *Data;
    uint32_t Index;
    uint32_t Length;

    ADC_t ADC;
    DMA_t DMA;
    Timer_t Timer;
} Sampler_t;

void Generator_Init(Generator_t *self);

void Sampler_Init(Sampler_t *self);
void Sampler_Get(Sampler_t *self);

void Sin_Generate(uint32_t *data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif