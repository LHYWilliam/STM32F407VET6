#include "TIM.h"

void TIM_Init(TIM *tim) {
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

    TIM_ClockConfigTypeDef clock = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(tim->Handler, &clock);
}