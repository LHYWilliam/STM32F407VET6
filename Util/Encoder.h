#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_TypeDef *TIM;
    char Channel[32];
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} Encoder_Handler;

void Encoder_Init(Encoder_Handler *encoder);

int16_t Encoder_Get(Encoder_Handler *encoder);

#endif