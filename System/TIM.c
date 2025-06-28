#include "TIM.h"

void TIM_Init(TIM_t *Self) {
    *(Self->Handler) = (TIM_HandleTypeDef){
        .Instance = Self->TIMx,
        .Init =
            {
                .Prescaler = Self->Prescaler,
                .Period = Self->Period,
            },
    };
    Self->HAL_TIM_Init(Self->Handler);

    TIM_ClockConfigTypeDef Clock = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(Self->Handler, &Clock);
}