#include "Sampler.h"

void Sampler_Init(Sampler_t *Self) {
    uint8_t i;
    for (i = 0; Self->ADC.Channel[i]; i++)
        ;
    Self->ADC.NbrOfConversion = i;

    if (Self->DMA.DMAx) {
        Self->DMA.PeriphInc = DISABLE;
        Self->DMA.PeriphSize = 32;
        Self->DMA.MemInc = ENABLE;
        Self->DMA.MemSize = 32;
        Self->DMA.Mode = DMA_CIRCULAR;
        Self->DMA.Direction = DMA_PERIPH_TO_MEMORY;
    }

    if (Self->Timer.TIMx) {
        Self->Timer.Trigger = TIM_TRGO_UPDATE;
        Self->ADC.Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(Self->Timer.TIMx);
    }

    ADC_Init(&Self->ADC);

    if (Self->Timer.TIMx) {
        Timer_Init(&Self->Timer);
    }

    Self->Index = Self->Length - 1;

    if (Self->DMA.DMAx) {
        ADC_DMAStart(&Self->ADC, Self->Data, Self->Length);

    } else if (Self->ADC.Continuous) {
        ADC_Start(&Self->ADC);
    }
}

uint32_t Sampler_GetValue(Sampler_t *Self, uint8_t Channel) {
    if (Self->ADC.Continuous == DISABLE) {
        ADC_ConfigChannel(&Self->ADC, Channel, 1);
        ADC_Start(&Self->ADC);
    }

    if (Self->Data) {
        Self->Index = (Self->Index + 1) % Self->Length;
        Self->Data[Self->Index] = ADC_GetValue(&Self->ADC);

        return Self->Data[Self->Index];

    } else {
        return ADC_GetValue(&Self->ADC);
    }
}

void Sampler_UpdateIndex(Sampler_t *Self) {
    Self->Index =
        Self->Length - DMAx_Streamy(Self->DMA.DMAx, Self->DMA.Stream)->NDTR;
    Self->Index = Self->Index > 0 ? Self->Index - 1 : Self->Length - 1;
}