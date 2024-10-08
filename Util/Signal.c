#include "arm_math.h"

#include "Signal.h"

void SignalGenerator_Init(SignalGenerator_Handler *self) {

    self->DAC.Trigger = DAC_TRIGGER_Tx_TRGO(self->Timer.TIMx);
    DAC_Init(&self->DAC);

    self->DMA.PeriphInc = DISABLE;
    self->DMA.PeriphSize = 32;
    self->DMA.MemInc = ENABLE;
    self->DMA.MemSize = 32;
    self->DMA.Mode = DMA_CIRCULAR;
    self->DMA.Direction = DMA_MEMORY_TO_PERIPH;
    DMA_Init(&self->DMA);

    self->Timer.Hz *= self->Length;
    self->Timer.Trigger = TIM_TRGO_UPDATE;
    Timer_Init(&self->Timer);

    SignalGenerator_Start(self, self->Data, self->Length);
}

void SignalGenerator_Start(SignalGenerator_Handler *self, uint32_t *data,
                           uint32_t length) {
    DAC_DMAStart(&self->DAC, data, length);
}

void SignalSampler_Init(SignalSampler_t *self) {
    self->ADC.Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(self->Timer.TIMx);
    ADC_Init(&self->ADC);

    self->DMA.PeriphInc = DISABLE;
    self->DMA.PeriphSize = 32;
    self->DMA.MemInc = ENABLE;
    self->DMA.MemSize = 32;
    self->DMA.Mode = DMA_CIRCULAR;
    self->DMA.Direction = DMA_PERIPH_TO_MEMORY;

    DMA_Init(&self->DMA);

    self->Timer.Trigger = TIM_TRGO_UPDATE;
    Timer_Init(&self->Timer);

    SignalSampler_Start(self, self->Data, self->Length);
}

void SignalSampler_Start(SignalSampler_t *self, uint32_t *data,
                         uint32_t length) {
    ADC_DMAStart(&self->ADC, data, length);
}

void Sin_Generate(uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        data[i] = (uint32_t)((arm_sin_f32(2.f * PI * i / length) + 1.f) *
                             (4095. / 2.f));
    }
}