#ifndef RNG_H
#define RNG_H

#include "stm32f4xx_hal.h"

typedef struct {
    RNG_HandleTypeDef Handler;
} mRNG;

void RNG_Init(mRNG *rng);

uint32_t RNG_RandomInt(mRNG *rng, int min, int max);

#endif