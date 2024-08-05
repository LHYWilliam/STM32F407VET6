#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx_hal.h"

#include "tim.h"

typedef struct {
    TIM_TypeDef *TIM;
    uint32_t ms;

    TIM_HandleTypeDef Handler;
} Timer;

void Timer_init(Timer *timer);

#endif