#ifndef RNG_H
#define RNG_H

#include "stm32f4xx_hal.h"

typedef struct {
    RNG_HandleTypeDef Handler;
} RNG_t;

void RNG_Init(RNG_t *self);

uint32_t RNG_RandomInt(RNG_t *self, int min, int max);

#endif