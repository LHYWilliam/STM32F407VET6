#include "timer.h"
#include "tim.h"

void Timer_init(Timer *timer) {
    TIM tim = {
        .TIM = timer->TIM,
        .Prescaler = 8400 - 1,
        .Period = timer->ms * 10 - 1,
        .Handler = &timer->Handler,
        .HAL_TIM_Init = HAL_TIM_Base_Init,
    };
    TIM_init(&tim);

    HAL_TIM_Base_Start_IT(tim.Handler);
};