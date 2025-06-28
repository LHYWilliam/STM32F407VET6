#include "Encoder.h"
#include "TIM.h"

void Encoder_Init(Encoder_t *Self) {
    Self->Handler = (TIM_HandleTypeDef){
        .Instance = Self->TIM,
        .Init =
            {
                .Prescaler = 1 - 1,
                .Period = 65536 - 1,
            },
    };
    TIM_Encoder_InitTypeDef IC = {
        .EncoderMode = TIM_ENCODERMODE_TI12,

        .IC1Filter = 0xF,
        .IC1Selection = TIM_ICSELECTION_DIRECTTI,

        .IC2Filter = 0xF,
        .IC2Selection = TIM_ICSELECTION_DIRECTTI,
    };
    HAL_TIM_Encoder_Init(&Self->Handler, &IC);

    HAL_TIM_Encoder_Start(&Self->Handler, TIM_CHANNEL_x(Self->Channel[0]));
    HAL_TIM_Encoder_Start(&Self->Handler, TIM_CHANNEL_x(Self->Channel[1]));
}

int16_t Encoder_GetCounter(Encoder_t *Self) {
    int16_t Count = __HAL_TIM_GetCounter(&Self->Handler);
    __HAL_TIM_SetCounter(&Self->Handler, 0);

    return Count;
}