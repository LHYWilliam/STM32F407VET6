#include <string.h>

#include "DAC.h"

void DAC_Init(DAC_Handler *dac) {
    dac->Handler = (DAC_HandleTypeDef){
        .Instance = DAC1,
    };
    HAL_DAC_Init(&dac->Handler);

    char *temp = dac->Channel;
    do {
        DAC_ChannelConfTypeDef channel = {
            .DAC_Trigger = dac->Trigger,
            .DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE,
        };
        HAL_DAC_ConfigChannel(&dac->Handler, &channel, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_Start(DAC_Handler *dac) {
    char *temp = dac->Channel;
    do {
        HAL_DAC_Start(&dac->Handler, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_DMAStart(DAC_Handler *dac, uint32_t *data, uint32_t length) {
    HAL_DAC_Start_DMA(&dac->Handler, DAC_CHANNEL_x(dac->Channel), data, length,
                      DAC_ALIGN_12B_R);
}

void DAC_Set(DAC_Handler *dac, uint8_t channel, uint16_t value) {
    HAL_DAC_SetValue(&dac->Handler, DAC_CHANNEL[channel], DAC_ALIGN_12B_R,
                     value);
};