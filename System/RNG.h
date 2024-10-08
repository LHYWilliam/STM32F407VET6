#ifndef RNG_H
#define RNG_H

#include "stm32f4xx_hal.h"

typedef struct {
    RNG_HandleTypeDef Handler;
} RNG_Handler;

void RNG_Init(RNG_Handler *self);

uint32_t RNG_RandomInt(RNG_Handler *self, int min, int max);

#endif