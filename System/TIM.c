#include "TIM.h"

void TIM_Init(TIM_t *Self) {
    *(Self->Handler) = (TIM_HandleTypeDef){
        .Instance = Self->TIMx,
        .Init =
            {
                .Prescaler = Self->Prescaler,
                .Period = Self->Period,
                .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
            },
    };
    Self->HAL_TIM_Init(Self->Handler);

    TIM_ClockConfigTypeDef sClockSourceConfig = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(Self->Handler, &sClockSourceConfig);
}