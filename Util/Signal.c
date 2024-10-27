#include <stdint.h>

#include "arm_math.h"

#include "Signal.h"

void SignalGenerator_Init(SignalGenerator_t *self) {

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

void SignalSampler_Init(SignalSampler_t *self) {
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
    if (self->DMA.DMAx) {
        ADC_DMAStart(&self->ADC, self->Data, self->Length);
    } else {
        ADC_Start(&self->ADC);
    }
}

uint32_t SignalSampler_Get(SignalSampler_t *self) {
    return ADC_Get(&self->ADC);
}

void Sin_Generate(uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        data[i] = (uint32_t)((arm_sin_f32(2.f * PI * i / length) + 1.f) *
                             (4095. / 2.f));
    }
}