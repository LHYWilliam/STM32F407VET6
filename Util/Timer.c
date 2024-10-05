#include "Timer.h"

void Timer_Init(Timer *timer) {
    TIM tim = {
        .TIM = timer->TIMx,
        .Prescaler = timer->ms ? (8400 - 1) : (timer->Hz ? 1 - 1 : NULL),
        .Period = timer->ms ? (timer->ms * 10 - 1)
                            : (timer->Hz ? (uint32_t)(84000000. / timer->Hz - 1)
                                         : NULL),
        .Handler = &timer->Handler,
        .HAL_TIM_Init = HAL_TIM_Base_Init,
    };
    TIM_Init(&tim);

    if (timer->Trigger) {
        TIM_MasterConfigTypeDef trigger = {
            .MasterOutputTrigger = timer->Trigger,
        };
        HAL_TIMEx_MasterConfigSynchronization(&timer->Handler, &trigger);
    }

    if (timer->Interrupt) {
        __HAL_TIM_CLEAR_IT(tim.Handler, TIM_FLAG_UPDATE);

        HAL_TIM_Base_Start_IT(tim.Handler);

    } else {
        HAL_TIM_Base_Start(tim.Handler);
    }
};