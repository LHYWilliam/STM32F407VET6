#include <string.h>

#include "dac.h"

void DAC_init(mDAC *dac) {
    dac->Handler = (DAC_HandleTypeDef){
        .Instance = DAC,
    };
    HAL_DAC_Init(&dac->Handler);

    char *temp = dac->channel;
    do {
        DAC_ChannelConfTypeDef channel = {
            .DAC_Trigger = dac->Trigger ? dac->Trigger : DAC_TRIGGER_NONE,
            .DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE,
        };
        HAL_DAC_ConfigChannel(&dac->Handler, &channel, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_start(mDAC *dac) {
    char *temp = dac->channel;
    do {
        HAL_DAC_Start(&dac->Handler, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_DMAStart(mDAC *dac, uint32_t *data, uint16_t length) {
    HAL_DAC_Start_DMA(&dac->Handler, DAC_CHANNEL_x(dac->channel), data, length,
                      DAC_ALIGN_12B_R);
}

void DAC_set(mDAC *dac, uint8_t channel, uint16_t value) {
    HAL_DAC_SetValue(&dac->Handler, DAC_CHANNEL[channel], DAC_ALIGN_12B_R,
                     value);
};