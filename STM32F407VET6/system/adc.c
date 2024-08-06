#include <stdint.h>
#include <string.h>

#include "adc.h"

void ADC_init(mADC *adc) {
    adc->Handler = (ADC_HandleTypeDef){
        .Instance = adc->ADCx,
        .Init =
            {
                .DataAlign = ADC_DATAALIGN_RIGHT,
                .Resolution = ADC_RESOLUTION_12B,
                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ScanConvMode = DISABLE,
                .ContinuousConvMode = ENABLE,

                .NbrOfConversion = 1,

                .ExternalTrigConv = ADC_SOFTWARE_START,
                .ExternalTrigConvEdge = DISABLE,

                .DMAContinuousRequests = DISABLE,
                .EOCSelection = DISABLE,

                .NbrOfDiscConversion = 0,
                .DiscontinuousConvMode = DISABLE,
            },
    };
    HAL_ADC_Init(&adc->Handler);

    uint8_t rank = 1;
    char *temp = adc->channel;
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