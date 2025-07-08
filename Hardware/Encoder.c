#include "TIM.h"

#include "Encoder.h"

void Encoder_Init(Encoder_t *Self) {
    Self->Handler = (TIM_HandleTypeDef){
        .Instance = Self->TIMx,
        .Init =
            {
                .Prescaler = 1 - 1,
                .Period = 65536 - 1,
            },
    };
    TIM_Encoder_InitTypeDef sConfig = {
        .EncoderMode = TIM_ENCODERMODE_TI12,

        .IC1Filter = 0xF,
        .IC1Selection = TIM_ICSELECTION_DIRECTTI,

        .IC2Filter = 0xF,
        .IC2Selection = TIM_ICSELECTION_DIRECTTI,
    };

    {
        __HAL_RCC_TIMx_CLK_ENABLE(Self->TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(Self->TIMx),
        };
        GPIO_InitPin(&GPIO, Self->GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, Self->GPIOxPiny[1]);
    }

    HAL_TIM_Encoder_Init(&Self->Handler, &sConfig);

    HAL_TIM_Encoder_Start(&Self->Handler, TIM_CHANNEL_x(Self->Channel[0]));
    HAL_TIM_Encoder_Start(&Self->Handler, TIM_CHANNEL_x(Self->Channel[1]));
}

int16_t Encoder_GetCounter(Encoder_t *Self) {
    int16_t Count = __HAL_TIM_GetCounter(&Self->Handler);
    __HAL_TIM_SetCounter(&Self->Handler, 0);

    return Count;
}