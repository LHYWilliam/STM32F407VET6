#include "ADC.h"

void ADC_Init(ADC_t *Self) {
    Self->Handler = (ADC_HandleTypeDef){
        .Instance = Self->ADCx,
        .Init =
            {
                .NbrOfConversion = Self->NbrOfConversion,
                .ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,

                .ContinuousConvMode = Self->Continuous,
                .ScanConvMode = (Self->NbrOfConversion > 1 && Self->Trigger)
                                    ? ENABLE
                                    : DISABLE,

                .DMAContinuousRequests = Self->Trigger ? ENABLE : DISABLE,
                .EOCSelection = Self->Trigger ? ADC_EOC_SINGLE_CONV : DISABLE,

                .ExternalTrigConv =
                    Self->Trigger ? Self->Trigger : ADC_SOFTWARE_START,
                .ExternalTrigConvEdge = Self->Trigger
                                            ? ADC_EXTERNALTRIGCONVEDGE_RISING
                                            : ADC_EXTERNALTRIGCONVEDGE_NONE,
            },
    };

    {
        __HAL_RCC_ADCx_CLK_ENABLE(Self->ADCx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_ANALOG,
        };
        for (uint8_t i = 0; Self->Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Self->GPIOxPiny[i]);
        }
    }

    HAL_ADC_Init(&Self->Handler);

    if (Self->Trigger) {
        ADC_ChannelConfTypeDef SConfig = {
            .SamplingTime = ADC_SAMPLETIME_3CYCLES,
        };

        for (uint8_t i = 0; Self->Channel[i]; i++) {
            SConfig.Channel = ADC_CHANNEL_x(Self->Channel[i]);
            SConfig.Rank = i + 1;

            HAL_ADC_ConfigChannel(&Self->Handler, &SConfig);
        }
    }
}

void ADC_Start(ADC_t *Self) {
    if (Self->Continuous) {
        for (uint8_t i = 0; Self->Channel[i]; i++) {
            ADC_ConfigChannel(Self, Self->Channel[i], i + 1);
        };
    }

    HAL_ADC_Start(&Self->Handler);
};

void ADC_DMAStart(ADC_t *Self, uint32_t *Data, uint32_t Length) {

    HAL_ADC_Start_DMA(&Self->Handler, Data, Length);
};

uint32_t ADC_GetValue(ADC_t *Self) {
    HAL_ADC_PollForConversion(&Self->Handler, HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&Self->Handler);
}

void ADC_ConfigChannel(ADC_t *Self, uint8_t Channel, uint8_t Rank) {
    static ADC_ChannelConfTypeDef SConfig = {
        .SamplingTime = ADC_SAMPLETIME_56CYCLES,
    };

    SConfig.Rank = Rank;
    SConfig.Channel = ADC_CHANNEL_x(Channel);

    HAL_ADC_ConfigChannel(&Self->Handler, &SConfig);
}