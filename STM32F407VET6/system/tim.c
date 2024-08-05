#include "tim.h"

void TIM_init(TIM *tim) {
    *(tim->Handler) = (TIM_HandleTypeDef){
        .Instance = tim->TIM,
        .Init =
            {
                .Prescaler = tim->Prescaler,
                .Period = tim->Period,
                .CounterMode = TIM_COUNTERMODE_UP,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
            },
    };
    HAL_TIM_Base_Init(tim->Handler);
}

void Timer_init(Timer *timer) {
    TIM tim = {
        .TIM = timer->TIM,
        .Prescaler = 8400 - 1,
        .Period = timer->ms * 10 - 1,
        .Handler = &timer->Handler,
    };

    TIM_init(&tim);
    HAL_TIM_Base_Start_IT(&timer->Handler);
};