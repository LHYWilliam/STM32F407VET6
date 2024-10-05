#include <string.h>

#include "DAC.h"

void DAC_Init(DAC_Handler *dac) {
    dac->Handler = (DAC_HandleTypeDef){
        .Instance = DAC,
    };
    HAL_DAC_Init(&dac->Handler);

    if (dac->DMA.DMAx && dac->Timer.TIMx) {
        dac->Trigger = DAC_TRIGGER_Tx_TRGO(dac->Timer.TIMx);
    }

    char *temp = dac->Channel;
    do {
        DAC_ChannelConfTypeDef channel = {
            .DAC_Trigger = dac->Trigger,
            .DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE,
        };
        HAL_DAC_ConfigChannel(&dac->Handler, &channel, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));

    if (dac->DMA.DMAx) {
        dac->DMA.PeriphInc = DISABLE;
        dac->DMA.PeriphSize = 32;
        dac->DMA.MemInc = ENABLE;
        dac->DMA.MemSize = 32;
        dac->DMA.Mode = DMA_CIRCULAR;
        dac->DMA.Direction = DMA_MEMORY_TO_PERIPH;

        DMA_Init(&dac->DMA);
    }

    if (dac->Timer.TIMx) {
        dac->Timer.Hz *= dac->Length;
        dac->Timer.Trigger = TIM_TRGO_UPDATE;
        Timer_Init(&dac->Timer);
    }
}

void DAC_Start(DAC_Handler *dac) {
    char *temp = dac->Channel;
    do {
        HAL_DAC_Start(&dac->Handler, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_DMAStart(DAC_Handler *dac, uint32_t *data, uint16_t length) {
    HAL_DAC_Start_DMA(&dac->Handler, DAC_CHANNEL_x(dac->Channel), data, length,
                      DAC_ALIGN_12B_R);
}

void DAC_Set(DAC_Handler *dac, uint8_t channel, uint16_t value) {
    HAL_DAC_SetValue(&dac->Handler, DAC_CHANNEL[channel], DAC_ALIGN_12B_R,
                     value);
};