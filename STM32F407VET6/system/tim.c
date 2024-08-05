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