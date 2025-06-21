#ifndef PWM_H
#define PWM_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef struct {
    TIM_TypeDef *TIM;
    char Channel[32];
    uint32_t Prescaler;
    uint32_t Period;
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} PWM_t;

void PWM_Init(PWM_t *self);

void PWM_Set(PWM_t *self, uint8_t channel, uint32_t value);

#endif