#ifndef MOTOR_H
#define MOTOR_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef struct {
    TIM_TypeDef *TIMx;
    uint8_t Channel;

    GPIOxPiny_t PWM;
    GPIOxPiny_t IN1;
    GPIOxPiny_t IN2;
    uint32_t IN1_ODR;
    uint32_t IN2_ODR;

    uint16_t Range;
    FunctionalState Invert;
} Motor_t;

void Motor_Init(Motor_t *Self);
void Motor_Set(Motor_t *Self, int16_t Speed);

#endif