#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_TypeDef *TIM;
    char Channel[32];
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} Encoder_t;

void Encoder_Init(Encoder_t *self);

int16_t Encoder_Get(Encoder_t *self);

#endif