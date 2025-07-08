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

    {
        __HAL_RCC_TIMx_CLK_ENABLE(Self->TIMx);

        if (Self->Interrupt) {
            HAL_NVIC_SetPriority(TIMx_IRQN(Self->TIMx), Self->Priority, 0);
            HAL_NVIC_EnableIRQ(TIMx_IRQN(Self->TIMx));
        }
    }

    TIM_Init(&TIM);

    if (Self->Trigger) {
        TIM_MasterConfigTypeDef sMasterConfig = {
            .MasterOutputTrigger = Self->Trigger,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
        };
        HAL_TIMEx_MasterConfigSynchronization(&Self->Handler, &sMasterConfig);
    }

    if (Self->Interrupt) {
        __HAL_TIM_CLEAR_IT(TIM.Handler, TIM_FLAG_UPDATE);

        HAL_TIM_Base_Start_IT(TIM.Handler);
    } else {
        HAL_TIM_Base_Start(TIM.Handler);
    }
};