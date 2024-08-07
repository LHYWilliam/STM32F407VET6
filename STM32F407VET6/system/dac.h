#ifndef DAC_H
#define DAC_H

#include "stm32f4xx.h"

#define DAC_CHANNEL_x(x)                                                       \
    ((x[0]) == '1' ? DAC_CHANNEL_1 : (x[0]) == '2' ? DAC_CHANNEL_2 : NULL)

static uint8_t DAC_CHANNEL[] = {NULL, DAC_CHANNEL_1, DAC_CHANNEL_2};

typedef struct {
    char channel[32];

    char GPIOxPiny[32];

    DAC_HandleTypeDef Handler;
} mDAC;

void DAC_init(mDAC *dac);
void DAC_start(mDAC *dac);

void DAC_set(mDAC *dac, uint8_t channel, float value);

#endif