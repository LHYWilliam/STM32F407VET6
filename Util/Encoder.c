#include <string.h>

#include "Encoder.h"
#include "TIM.h"

void Encoder_Init(Encoder_Handler *self) {
    self->Handler = (TIM_HandleTypeDef){
        .Instance = self->TIM,
        .Init =
            {
                .Prescaler = 1 - 1,
                .Period = 65536 - 1,
                .CounterMode = TIM_COUNTERMODE_UP,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
            },
    };
    TIM_Encoder_InitTypeDef IC = {
        .EncoderMode = TIM_ENCODERMODE_TI12,

        .IC1Filter = 0xF,
        .IC1Prescaler = TIM_ICPSC_DIV1,
        .IC1Selection = TIM_ICSELECTION_DIRECTTI,
        .IC1Polarity = TIM_ENCODERINPUTPOLARITY_RISING,

        .IC2Filter = 0xF,
        .IC2Prescaler = TIM_ICPSC_DIV1,
        .IC2Selection = TIM_ICSELECTION_DIRECTTI,
        .IC2Polarity = TIM_ENCODERINPUTPOLARITY_RISING,
    };
    HAL_TIM_Encoder_Init(&self->Handler, &IC);

    char *temp = self->Channel;
    do {
        HAL_TIM_Encoder_Start(&self->Handler, TIM_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

int16_t Encoder_Get(Encoder_Handler *self) {
    int16_t count = (int16_t)__HAL_TIM_GetCounter(&self->Handler);
    __HAL_TIM_SetCounter(&self->Handler, 0);
    return count;
}