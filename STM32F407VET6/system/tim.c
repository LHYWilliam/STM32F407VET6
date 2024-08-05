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
    tim->HAL_TIM_Init(tim->Handler);
}