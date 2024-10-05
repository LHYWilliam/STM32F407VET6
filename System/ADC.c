#include <string.h>

#include "ADC.h"

void ADC_Init(mADC *adc) {
    uint8_t NbrOfConversion = 0;
    char *temp = adc->Channel;
    do {
        NbrOfConversion++;
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2));

    if (adc->Timer.TIMx) {
        adc->Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(adc->Timer.TIMx);
    }

    adc->Handler = (ADC_HandleTypeDef){
        .Instance = adc->ADCx,
        .Init =
            {
                .ScanConvMode = NbrOfConversion > 1 ? ENABLE : DISABLE,
                .ContinuousConvMode = adc->Continuous,
                .DMAContinuousRequests = ENABLE,
                .EOCSelection = ADC_EOC_SINGLE_CONV,

                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ExternalTrigConv = adc->Trigger,
                .ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING,

                .NbrOfConversion = NbrOfConversion,
            },
    };
    HAL_ADC_Init(&adc->Handler);

    uint8_t rank = 1;
    temp = adc->Channel;
    do {
        ADC_ChannelConfTypeDef channel = {
            .Channel = ADC_CHANNEL_x(temp),
            .Rank = rank,
            .SamplingTime = ADC_SAMPLETIME_3CYCLES,
        };
        HAL_ADC_ConfigChannel(&adc->Handler, &channel);
    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (rank = rank + 1));

    if (adc->DMA.DMAx) {
        adc->DMA.PeriphInc = DISABLE;
        adc->DMA.PeriphSize = 32;
        adc->DMA.MemInc = ENABLE;
        adc->DMA.MemSize = 32;
        adc->DMA.Mode = DMA_CIRCULAR;
        adc->DMA.Direction = DMA_PERIPH_TO_MEMORY;

        DMA_Init(&adc->DMA);
    }

    if (adc->Timer.TIMx) {
        adc->Timer.Trigger = TIM_TRGO_UPDATE;
        Timer_Init(&adc->Timer);
    }
}

void ADC_Start(mADC *adc) { HAL_ADC_Start(&adc->Handler); };

void ADC_DMAStart(mADC *adc, uint32_t *data, uint16_t length) {
    HAL_ADC_Start_DMA(&adc->Handler, data, length);
};

uint16_t ADC_Get(mADC *adc) { return HAL_ADC_GetValue(&adc->Handler); }