#ifndef DAC_H
#define DAC_H

#include "RTE_Components.h"
#include CMSIS_device_header

#undef DAC

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

    DAC_HandleTypeDef Handler;
} DAC_t;

void DAC_Init(DAC_t *self);
void DAC_Start(DAC_t *self);
void DAC_DMAStart(DAC_t *self, uint32_t *data, uint32_t length);

void DAC_Set(DAC_t *self, uint8_t channel, uint16_t value);

#endif