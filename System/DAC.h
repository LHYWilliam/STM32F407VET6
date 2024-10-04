#ifndef DAC_H
#define DAC_H

#include "stm32f4xx.h"

#include "DMA.h"

#define DAC_CHANNEL_x(x)                                                       \
    ((x[0]) == '1' ? DAC_CHANNEL_1 : (x[0]) == '2' ? DAC_CHANNEL_2 : NULL)

static uint8_t DAC_CHANNEL[] = {NULL, DAC_CHANNEL_1, DAC_CHANNEL_2};

#define DMA_Handlex(x)                                                         \
    ((x[0]) == '1' ? DMA_Handle1 : (x[0]) == '2' ? DMA_Handle2 : NULL)

typedef struct {
    char Channel[32];
    uint32_t Trigger;

    char GPIOxPiny[32];

    DMA DMA;

    DAC_HandleTypeDef Handler;
} mDAC;

void DAC_Init(mDAC *dac);
void DAC_Start(mDAC *dac);
void DAC_DMAStart(mDAC *dac, uint32_t *data, uint16_t length);

void DAC_Set(mDAC *dac, uint8_t channel, uint16_t value);

#endif