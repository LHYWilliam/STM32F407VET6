#ifndef RNG_H
#define RNG_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    RNG_HandleTypeDef Handler;
} RNG_t;

void RNG_Init(RNG_t *self);

uint32_t RNG_RandomInt(RNG_t *self, int min, int max);

#endif