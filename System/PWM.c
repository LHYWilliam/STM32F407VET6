#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *Self) {
    TIM_t TIM = {
        .TIM = Self->TIM,
        .Prescaler = Self->Prescaler,
        .Period = Self->Period,
        .Handler = &Self->Handler,
        .HAL_TIM_Init = HAL_TIM_PWM_Init,
    };
    TIM_Init(&TIM);

    TIM_OC_InitTypeDef OC = {
        .OCMode = TIM_OCMODE_PWM1,
        .OCPolarity = TIM_OCPOLARITY_HIGH,
    };

    for (uint8_t i = 0; Self->Channel[i]; i++) {
        uint8_t Channel = TIM_CHANNEL_x(Self->Channel[i]);
        HAL_TIM_PWM_ConfigChannel(TIM.Handler, &OC, Channel);
        HAL_TIM_PWM_Start(TIM.Handler, Channel);
    }
}

void PWM_Set(PWM_t *Self, uint8_t Channel, uint32_t Value) {
    __HAL_TIM_SetCompare(&Self->Handler, TIM_CHANNEL_x(Channel), Value);
}