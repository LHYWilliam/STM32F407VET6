#include "Timer.h"

void Timer_Init(Timer_t *self) {
    TIM_t tim = {
        .TIM = self->TIMx,
        .Prescaler = self->ms ? (8400 - 1) : (self->Hz ? 1 - 1 : NULL),
        .Period =
            self->ms ? (self->ms * 10 - 1)
                     : (self->Hz ? (uint32_t)(84000000. / self->Hz - 1) : NULL),
        .Handler = &self->Handler,
        .HAL_TIM_Init = HAL_TIM_Base_Init,
    };
    TIM_Init(&tim);

    if (self->Trigger) {
        TIM_MasterConfigTypeDef trigger = {
            .MasterOutputTrigger = self->Trigger,
        };
        HAL_TIMEx_MasterConfigSynchronization(&self->Handler, &trigger);
    }

    if (self->Interrupt) {
        __HAL_TIM_CLEAR_IT(tim.Handler, TIM_FLAG_UPDATE);

        HAL_TIM_Base_Start_IT(tim.Handler);

    } else {
        HAL_TIM_Base_Start(tim.Handler);
    }
};