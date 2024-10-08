#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_TypeDef *TIM;
    char Channel[32];
    uint32_t Prescaler;
    uint32_t Period;
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} PWM_Handler;

void PWM_Init(PWM_Handler *self);

void PWM_Set(PWM_Handler *self, uint8_t channel, uint32_t value);

#endif