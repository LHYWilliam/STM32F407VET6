#include "TIM.h"

void TIM_Init(TIM_t *Self) {
    *(Self->Handler) = (TIM_HandleTypeDef){
        .Instance = Self->TIM,
        .Init =
            {
                .Prescaler = Self->Prescaler,
                .Period = Self->Period,
                .CounterMode = TIM_COUNTERMODE_UP,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
            },
    };
    Self->HAL_TIM_Init(Self->Handler);

    TIM_ClockConfigTypeDef Clock = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(Self->Handler, &Clock);
}