#include <stdint.h>

#include "Signal.h"

void Generator_Init(Generator_t *self) {

    if (self->Timer.TIMx) {
        self->DAC.Trigger = DAC_TRIGGER_Tx_TRGO(self->Timer.TIMx);
    }
    DAC_Init(&self->DAC);

    if (self->DMA.DMAx) {
        self->DMA.PeriphInc = DISABLE;
        self->DMA.PeriphSize = 32;
        self->DMA.MemInc = ENABLE;
        self->DMA.MemSize = 32;
        self->DMA.Mode = DMA_CIRCULAR;
        self->DMA.Direction = DMA_MEMORY_TO_PERIPH;
        DMA_Init(&self->DMA);
    }

    if (self->Timer.TIMx) {
        self->Timer.Hz *= self->Length;
        self->Timer.Trigger = TIM_TRGO_UPDATE;
        Timer_Init(&self->Timer);
    }
    if (self->DMA.DMAx) {
        DAC_DMAStart(&self->DAC, self->Data, self->Length);
    } else {
        DAC_Start(&self->DAC);
    }
}

void Sampler_Init(Sampler_t *self) {
    if (self->Timer.TIMx) {
        self->ADC.Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(self->Timer.TIMx);
    }
    ADC_Init(&self->ADC);

    if (self->DMA.DMAx) {
        self->DMA.PeriphInc = DISABLE;
        self->DMA.PeriphSize = 32;
        self->DMA.MemInc = ENABLE;
        self->DMA.MemSize = 32;
        self->DMA.Mode = DMA_CIRCULAR;
        self->DMA.Direction = DMA_PERIPH_TO_MEMORY;

        DMA_Init(&self->DMA);
    }

    if (self->Timer.TIMx) {
        self->Timer.Trigger = TIM_TRGO_UPDATE;

        Timer_Init(&self->Timer);
    }

    self->Index = self->Length - 1;

    if (self->DMA.DMAx) {
        ADC_DMAStart(&self->ADC, self->Data, self->Length);
    } else {
        ADC_Start(&self->ADC);
    }
}

void Sampler_Get(Sampler_t *self) {
    self->Data[self->Index] = ADC_Get(&self->ADC);
}

void Sampler_UpdateIndex(Sampler_t *self) {
    self->Index =
        self->Length - DMAx_Streamy(self->DMA.DMAx, self->DMA.Stream)->NDTR;
    self->Index = self->Index > 0 ? self->Index - 1 : self->Length - 1;
}

void Sin_Generate(uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        data[i] = (uint32_t)((arm_sin_f32(2.f * PI * i / length) + 1.f) *
                             (4095. / 2.f));
    }
}