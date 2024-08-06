#include <string.h>

#include "adc.h"

void ADC_init(mADC *adc) {
    uint8_t NbrOfConversion = 0;
    char *temp = adc->channel;
    do {
        NbrOfConversion++;
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));

    adc->Handler = (ADC_HandleTypeDef){
        .Instance = adc->ADCx,
        .Init =
            {
                .DataAlign = ADC_DATAALIGN_RIGHT,
                .Resolution = ADC_RESOLUTION_12B,
                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ScanConvMode = ENABLE,
                .ContinuousConvMode = ENABLE,
                .DMAContinuousRequests = ENABLE,

                .NbrOfConversion = NbrOfConversion,

                .ExternalTrigConv = ADC_SOFTWARE_START,
            },
    };
    HAL_ADC_Init(&adc->Handler);

    uint8_t rank = 1;
    temp = adc->channel;
    do {
        ADC_ChannelConfTypeDef channel = {
            .Channel = ADC_CHANNEL_x(temp),
            .Rank = rank,
            .SamplingTime = ADC_SAMPLETIME_56CYCLES,
        };
        HAL_ADC_ConfigChannel(&adc->Handler, &channel);
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (rank = rank + 1));
}

uint16_t ADC_get(mADC *adc) { return HAL_ADC_GetValue(&adc->Handler); }

void ADC_Start(mADC *adc) { HAL_ADC_Start(&adc->Handler); };

void ADC_DMAStart(mADC *adc, uint32_t *data, uint8_t length) {
    HAL_ADC_Start_DMA(&adc->Handler, data, length);
};