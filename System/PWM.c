#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *Self) {
    TIM_t TIM = {
        .TIMx = Self->TIMx,
        .Prescaler = Self->Prescaler,
        .Period = Self->Period,
        .Handler = &Self->Handler,
        .HAL_TIM_Init = (Self->TIMx == TIM1 || Self->TIMx == TIM8)
                            ? HAL_TIM_Base_Init
                            : HAL_TIM_PWM_Init,
    };

    {
        __HAL_RCC_TIMx_CLK_ENABLE(Self->TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(Self->TIMx),
        };

        for (uint8_t i = 0; Self->Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Self->GPIOxPiny[i]);
        }
    }

    TIM_Init(&TIM);

    if (Self->TIMx == TIM1 || Self->TIMx == TIM8) {
        HAL_TIM_PWM_Init(&Self->Handler);
    }

    TIM_MasterConfigTypeDef sMasterConfig = {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET,
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
    };
    HAL_TIMEx_MasterConfigSynchronization(&Self->Handler, &sMasterConfig);

    TIM_OC_InitTypeDef OC = {
        .OCMode = TIM_OCMODE_PWM1,
    };
    for (uint8_t i = 0; Self->Channel[i]; i++) {
        uint8_t Channel = TIM_CHANNEL_x(Self->Channel[i]);
        HAL_TIM_PWM_ConfigChannel(&Self->Handler, &OC, Channel);
    }

    if (Self->TIMx == TIM1 || Self->TIMx == TIM8) {
        TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {
            .BreakPolarity = TIM_BREAKPOLARITY_HIGH,
        };
        HAL_TIMEx_ConfigBreakDeadTime(&Self->Handler, &sBreakDeadTimeConfig);
    }

    for (uint8_t i = 0; Self->Channel[i]; i++) {
        uint8_t Channel = TIM_CHANNEL_x(Self->Channel[i]);
        HAL_TIM_PWM_Start(&Self->Handler, Channel);
    }
}

void PWM_SetSetCompare(PWM_t *Self, uint8_t Channel, uint32_t Value) {
    __HAL_TIM_SetCompare(&Self->Handler, TIM_CHANNEL_x(Channel), Value);
}