#ifndef DAC_H
#define DAC_H

#include "stm32f4xx_hal.h"

#include "DMA.h"
#include "Timer.h"

#define DAC_TRIGGER_Tx_TRGO(x)                                                 \
    ((x) == TIM2   ? DAC_TRIGGER_T2_TRGO                                       \
     : (x) == TIM4 ? DAC_TRIGGER_T4_TRGO                                       \
     : (x) == TIM5 ? DAC_TRIGGER_T5_TRGO                                       \
     : (x) == TIM6 ? DAC_TRIGGER_T6_TRGO                                       \
     : (x) == TIM7 ? DAC_TRIGGER_T7_TRGO                                       \
     : (x) == TIM8 ? DAC_TRIGGER_T8_TRGO                                       \
                   : NULL)

#define DAC_CHANNEL_x(x)                                                       \
    ((x[0]) == '1' ? DAC_CHANNEL_1 : (x[0]) == '2' ? DAC_CHANNEL_2 : NULL)

static uint8_t DAC_CHANNEL[] = {NULL, DAC_CHANNEL_1, DAC_CHANNEL_2};

#define DMA_Handlex(x)                                                         \
    ((x[0]) == '1' ? DMA_Handle1 : (x[0]) == '2' ? DMA_Handle2 : NULL)

typedef struct {
    char Channel[32];
    uint32_t Trigger;

    char GPIOxPiny[32];
	
	uint32_t Length;
    DMA DMA;
    Timer Timer;

    DAC_HandleTypeDef Handler;
} mDAC;

void DAC_Init(mDAC *dac);
void DAC_Start(mDAC *dac);
void DAC_DMAStart(mDAC *dac, uint32_t *data, uint16_t length);

void DAC_Set(mDAC *dac, uint8_t channel, uint16_t value);

#endif