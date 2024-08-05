#include <string.h>

#include "encoder.h"
#include "tim.h"

void Encoder_init(Encoder *encoder) {
    encoder->Handler = (TIM_HandleTypeDef){
        .Instance = encoder->TIM,
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
    HAL_TIM_Encoder_Init(&encoder->Handler, &IC);

    char *temp = encoder->channel;
    do {
        HAL_TIM_Encoder_Start(&encoder->Handler, TIM_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

int32_t Encoder_get(Encoder *encoder) {
    int32_t count = (int32_t)__HAL_TIM_GetCounter(&encoder->Handler);
    __HAL_TIM_SetCounter(&encoder->Handler, 0);
    return count;
}