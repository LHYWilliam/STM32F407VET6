#include <string.h>

#include "PWM.h"
#include "TIM.h"

void PWM_Init(PWM_t *self) {
    TIM_t tim = {
        .TIM = self->TIM,
        .Prescaler = self->Prescaler,
        .Period = self->Period,
        .Handler = &self->Handler,
        .HAL_TIM_Init = HAL_TIM_PWM_Init,
    };
    TIM_Init(&tim);

    TIM_OC_InitTypeDef OC = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = 0,
        .OCPolarity = TIM_OCPOLARITY_HIGH,
    };

    char *temp = self->Channel;
    do {
        uint8_t channel = TIM_CHANNEL_x(temp);
        HAL_TIM_PWM_ConfigChannel(tim.Handler, &OC, channel);
        HAL_TIM_PWM_Start(tim.Handler, channel);
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void PWM_Set(PWM_t *self, uint8_t channel, uint32_t value) {
    __HAL_TIM_SetCompare(&self->Handler, TIM_Channel[channel], value);
}