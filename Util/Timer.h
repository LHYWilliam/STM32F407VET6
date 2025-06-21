#ifndef TIMER_H
#define TIMER_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "TIM.h"

typedef struct {
    TIM_TypeDef *TIMx;
    uint32_t ms;
    uint32_t Hz;

    uint8_t Interrupt;
    uint32_t Trigger;

    TIM_HandleTypeDef Handler;
} Timer_t;

void Timer_Init(Timer_t *self);

#endif