#include "ADC.h"

void ADC_Init(ADC_t *Self) {
    uint8_t NbrOfConversion = 0;
    for (NbrOfConversion = 0; Self->Channel[NbrOfConversion]; NbrOfConversion++)
        ;

    Self->Handler = (ADC_HandleTypeDef){
        .Instance = Self->ADCx,
        .Init =
            {
                .ScanConvMode = NbrOfConversion > 1 ? ENABLE : DISABLE,
                .ContinuousConvMode = Self->Continuous,
                .DMAContinuousRequests = Self->Trigger ? ENABLE : DISABLE,
                .EOCSelection = Self->Trigger ? ADC_EOC_SINGLE_CONV : DISABLE,

                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ExternalTrigConv =
                    Self->Trigger ? Self->Trigger : ADC_SOFTWARE_START,
                .ExternalTrigConvEdge = Self->Trigger
                                            ? ADC_EXTERNALTRIGCONVEDGE_RISING
                                            : ADC_EXTERNALTRIGCONVEDGE_NONE,

                .NbrOfConversion = NbrOfConversion,
            },
    };
    HAL_ADC_Init(&Self->Handler);

    for (uint8_t i = 0; i < NbrOfConversion; i++) {
        ADC_ChannelConfTypeDef Channel = {
            .Channel = ADC_CHANNEL_x(Self->Channel[i]),
            .Rank = i + 1,
            .SamplingTime = ADC_SAMPLETIME_3CYCLES,
        };
        HAL_ADC_ConfigChannel(&Self->Handler, &Channel);
    }
}

void ADC_Start(ADC_t *Self) { HAL_ADC_Start(&Self->Handler); };

void ADC_DMAStart(ADC_t *Self, uint32_t *Data, uint32_t Length) {
    HAL_ADC_Start_DMA(&Self->Handler, Data, Length);
};

uint32_t ADC_GetValue(ADC_t *Self) { return HAL_ADC_GetValue(&Self->Handler); }