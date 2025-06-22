#include "Sampler.h"

void Sampler_Init(Sampler_t *Self) {
    if (Self->Timer.TIMx) {
        Self->ADC.Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(Self->Timer.TIMx);
    }
    ADC_Init(&Self->ADC);

    if (Self->DMA.DMAx) {
        Self->DMA.PeriphInc = DISABLE;
        Self->DMA.PeriphSize = 32;
        Self->DMA.MemInc = ENABLE;
        Self->DMA.MemSize = 32;
        Self->DMA.Mode = DMA_CIRCULAR;
        Self->DMA.Direction = DMA_PERIPH_TO_MEMORY;

        DMA_Init(&Self->DMA);
    }

    if (Self->Timer.TIMx) {
        Self->Timer.Trigger = TIM_TRGO_UPDATE;

        Timer_Init(&Self->Timer);
    }

    Self->Index = Self->Length - 1;

    if (Self->DMA.DMAx) {
        ADC_DMAStart(&Self->ADC, Self->Data, Self->Length);
    } else {
        ADC_Start(&Self->ADC);
    }
}

void Sampler_GetValue(Sampler_t *Self) {
    Self->Data[Self->Index] = ADC_GetValue(&Self->ADC);
}

void Sampler_UpdateIndex(Sampler_t *Self) {
    Self->Index =
        Self->Length - DMAx_Streamy(Self->DMA.DMAx, Self->DMA.Stream)->NDTR;
    Self->Index = Self->Index > 0 ? Self->Index - 1 : Self->Length - 1;
}