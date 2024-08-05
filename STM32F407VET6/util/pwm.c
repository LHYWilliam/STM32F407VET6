#include <string.h>

#include "pwm.h"
#include "tim.h"

void PWM_init(PWM *pwm) {
    TIM tim = {
        .TIM = pwm->TIM,
        .Prescaler = pwm->Prescaler,
        .Period = pwm->Period,
        .Handler = &pwm->Handler,
        .HAL_TIM_Init = HAL_TIM_PWM_Init,
    };
    TIM_init(&tim);

    TIM_OC_InitTypeDef OC = {
        .OCMode = TIM_OCMODE_PWM1,
        .Pulse = 0,
        .OCPolarity = TIM_OCPOLARITY_HIGH,
    };

    char *temp = pwm->channel;
    do {
        uint8_t channel = TIM_CHANNEL_x(temp);
        HAL_TIM_PWM_ConfigChannel(tim.Handler, &OC, channel);
        HAL_TIM_PWM_Start(tim.Handler, channel);
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void PWM_set(PWM *pwm, uint8_t channel, uint32_t value) {
    __HAL_TIM_SetCompare(&pwm->Handler, TIM_Channel[channel], value);
}