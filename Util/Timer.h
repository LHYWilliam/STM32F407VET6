#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx_hal.h"

#include "TIM.h"

typedef struct {
    TIM_TypeDef *TIMx;
    uint32_t ms;
    uint32_t Hz;

    uint8_t Interrupt;
    uint32_t Trigger;

    TIM_HandleTypeDef Handler;
} Timer;

void Timer_Init(Timer *timer);

#endif