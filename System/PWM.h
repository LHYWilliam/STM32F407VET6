#ifndef PWM_H
#define PWM_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    TIM_TypeDef *TIMx;
    uint8_t Channel[4];
    GPIOxPiny_t GPIOxPiny[4];

    uint32_t Prescaler;
    uint32_t Period;

    TIM_HandleTypeDef Handler;
} PWM_t;

void PWM_Init(PWM_t *Self);
void PWM_SetSetCompare(PWM_t *Self, uint8_t Channel, uint32_t Value);

#endif