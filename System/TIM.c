#include "TIM.h"

void TIM_Init(TIM_t *self) {
    *(self->Handler) = (TIM_HandleTypeDef){
        .Instance = self->TIM,
        .Init =
            {
                .Prescaler = self->Prescaler,
                .Period = self->Period,
                .CounterMode = TIM_COUNTERMODE_UP,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
            },
    };
    self->HAL_TIM_Init(self->Handler);

    TIM_ClockConfigTypeDef clock = {
        .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(self->Handler, &clock);
}