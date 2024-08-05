#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_TypeDef *TIM;
    char channel[32];
    uint32_t Prescaler;
    uint32_t Period;
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;

    uint32_t _channel[5];
} PWM;

void PWM_init(PWM *pwm);

void PWM_set(PWM *pwm, uint8_t channel, uint32_t value);

#endif