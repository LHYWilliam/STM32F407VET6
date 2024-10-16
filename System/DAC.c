#include <string.h>

#include "DAC.h"

void DAC_Init(DAC_t *self) {
    self->Handler = (DAC_HandleTypeDef){
        .Instance = DAC1,
    };
    HAL_DAC_Init(&self->Handler);

    char *temp = self->Channel;
    do {
        DAC_ChannelConfTypeDef channel = {
            .DAC_Trigger = self->Trigger,
            .DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE,
        };
        HAL_DAC_ConfigChannel(&self->Handler, &channel, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_Start(DAC_t *self) {
    char *temp = self->Channel;
    do {
        HAL_DAC_Start(&self->Handler, DAC_CHANNEL_x(temp));
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));
}

void DAC_DMAStart(DAC_t *self, uint32_t *data, uint32_t length) {
    HAL_DAC_Start_DMA(&self->Handler, DAC_CHANNEL_x(self->Channel), data,
                      length, DAC_ALIGN_12B_R);
}

void DAC_Set(DAC_t *self, uint8_t channel, uint16_t value) {
    HAL_DAC_SetValue(&self->Handler, DAC_CHANNEL[channel], DAC_ALIGN_12B_R,
                     value);
};