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
                .ScanConvMode = NbrOfConversion > 1 ? ENABLE : DISABLE,
                .ContinuousConvMode = adc->continuous,
                .DMAContinuousRequests = ENABLE,
                .EOCSelection = ADC_EOC_SINGLE_CONV,

                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ExternalTrigConv = adc->trigger,
                .ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING,

                .NbrOfConversion = NbrOfConversion,
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

    if (adc->DMA.DMAx) {
        adc->DMA.sourceInc = DISABLE;
        adc->DMA.sourceSize = 32;
        adc->DMA.targetInc = ENABLE;
        adc->DMA.targetSize = 32;

        DMA_init(&adc->DMA);
    }
}

void ADC_Start(mADC *adc) { HAL_ADC_Start(&adc->Handler); };

void ADC_DMAStart(mADC *adc, uint32_t *data, uint16_t length) {
    HAL_ADC_Start_DMA(&adc->Handler, data, length);
};

uint16_t ADC_get(mADC *adc) { return HAL_ADC_GetValue(&adc->Handler); }