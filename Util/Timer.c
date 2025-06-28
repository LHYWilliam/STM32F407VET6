#include "Timer.h"

void Timer_Init(Timer_t *Self) {
    TIM_t TIM = {
        .TIMx = Self->TIMx,
        .Prescaler = Self->ms ? (8400 - 1) : (Self->Hz ? 84 - 1 : NULL),
        .Period = Self->ms
                      ? (Self->ms * 10 - 1)
                      : (Self->Hz ? (uint32_t)(1000000. / Self->Hz - 1) : NULL),
        .Handler = &Self->Handler,
        .HAL_TIM_Init = HAL_TIM_Base_Init,
    };
    TIM_Init(&TIM);

    if (Self->Trigger) {
        TIM_MasterConfigTypeDef Trigger = {
            .MasterOutputTrigger = Self->Trigger,
        };
        HAL_TIMEx_MasterConfigSynchronization(&Self->Handler, &Trigger);
    }

    if (Self->Interrupt) {
        __HAL_TIM_CLEAR_IT(TIM.Handler, TIM_FLAG_UPDATE);

        HAL_TIM_Base_Start_IT(TIM.Handler);
    } else {
        HAL_TIM_Base_Start(TIM.Handler);
    }
};