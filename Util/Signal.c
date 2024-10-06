#include "arm_math.h"

#include "Signal.h"

void SignalGenerator_Init(SignalGenerator_Handler *generator) {

    generator->_DAC.Trigger = DAC_TRIGGER_Tx_TRGO(generator->Timer.TIMx);
    DAC_Init(&generator->_DAC);

    generator->DMA.PeriphInc = DISABLE;
    generator->DMA.PeriphSize = 32;
    generator->DMA.MemInc = ENABLE;
    generator->DMA.MemSize = 32;
    generator->DMA.Mode = DMA_CIRCULAR;
    generator->DMA.Direction = DMA_MEMORY_TO_PERIPH;
    DMA_Init(&generator->DMA);

    generator->Timer.Hz *= generator->Length;
    generator->Timer.Trigger = TIM_TRGO_UPDATE;
    Timer_Init(&generator->Timer);
    
    SignalGenerator_Start(generator, generator->Data, generator->Length);
}

void SignalGenerator_Start(SignalGenerator_Handler *generator, uint32_t *data,
                           uint32_t length) {
    DAC_DMAStart(&generator->_DAC, data, length);
}

void SignalSampler_Init(SignalSampler_Handler *sampler) {
    sampler->_ADC.Trigger = ADC_EXTERNALTRIGCONV_Tx_TRGO(sampler->Timer.TIMx);
    ADC_Init(&sampler->_ADC);

    sampler->DMA.PeriphInc = DISABLE;
    sampler->DMA.PeriphSize = 32;
    sampler->DMA.MemInc = ENABLE;
    sampler->DMA.MemSize = 32;
    sampler->DMA.Mode = DMA_CIRCULAR;
    sampler->DMA.Direction = DMA_PERIPH_TO_MEMORY;

    DMA_Init(&sampler->DMA);

    sampler->Timer.Trigger = TIM_TRGO_UPDATE;
    Timer_Init(&sampler->Timer);
    
    SignalSampler_Start(sampler, sampler->Data, sampler->Length);
}

void SignalSampler_Start(SignalSampler_Handler *sampler, uint32_t *data,
                         uint32_t length) {
    ADC_DMAStart(&sampler->_ADC, data, length);
}

void Sin_Generate(uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        data[i] = (uint32_t)((arm_sin_f32(2.f * PI * i / length) + 1.f) *
                             (4095. / 2.f));
    }
}