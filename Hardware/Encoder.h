#ifndef ENCODER_H
#define ENCODER_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    TIM_TypeDef *TIM;
    uint8_t Channel[2];
    GPIOxPiny_t GPIOxPiny[2];

    TIM_HandleTypeDef Handler;
} Encoder_t;

void Encoder_Init(Encoder_t *Self);
int16_t Encoder_GetCounter(Encoder_t *Self);

#endif