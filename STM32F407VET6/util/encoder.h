#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_TypeDef *TIM;
    char channel[32];
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} Encoder;

void Encoder_init(Encoder *encoder);

int16_t Encoder_get(Encoder *encoder);

#endif