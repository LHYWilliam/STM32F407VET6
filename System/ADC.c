#include <string.h>

#include "ADC.h"

void ADC_Init(ADC_Handler *self) {
    uint8_t NbrOfConversion = 0;
    char *temp = self->Channel;
    do {
        NbrOfConversion++;
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));

    self->Handler = (ADC_HandleTypeDef){
        .Instance = self->ADCx,
        .Init =
            {
                .ScanConvMode = NbrOfConversion > 1 ? ENABLE : DISABLE,
                .ContinuousConvMode = self->Continuous,
                .DMAContinuousRequests = ENABLE,
                .EOCSelection = ADC_EOC_SINGLE_CONV,

                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ExternalTrigConv = self->Trigger,
                .ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING,

                .NbrOfConversion = NbrOfConversion,
            },
    };
    HAL_ADC_Init(&self->Handler);

    uint8_t rank = 1;
    temp = self->Channel;
    do {
        ADC_ChannelConfTypeDef channel = {
            .Channel = ADC_CHANNEL_x(temp),
            .Rank = rank,
            .SamplingTime = ADC_SAMPLETIME_3CYCLES,
        };
        HAL_ADC_ConfigChannel(&self->Handler, &channel);
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (rank = rank + 1));
}

void ADC_Start(ADC_Handler *self) { HAL_ADC_Start(&self->Handler); };

void ADC_DMAStart(ADC_Handler *self, uint32_t *data, uint32_t length) {
    HAL_ADC_Start_DMA(&self->Handler, data, length);
};

uint16_t ADC_Get(ADC_Handler *self) { return HAL_ADC_GetValue(&self->Handler); }