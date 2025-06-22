#ifndef ADC_H
#define ADC_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

#undef ADC

#define ADC_EXTERNALTRIGCONV_Tx_TRGO(x)                                        \
    ((x) == TIM2   ? ADC_EXTERNALTRIGCONV_T2_TRGO                              \
     : (x) == TIM3 ? ADC_EXTERNALTRIGCONV_T3_TRGO                              \
     : (x) == TIM5 ? ADC_EXTERNALTRIGCONV_T8_TRGO                              \
                   : NULL)

#define ADC_CHANNEL_x(x)                                                       \
    ((x) == 0    ? ADC_CHANNEL_0                                               \
     : (x) == 1  ? ADC_CHANNEL_1                                               \
     : (x) == 2  ? ADC_CHANNEL_2                                               \
     : (x) == 3  ? ADC_CHANNEL_3                                               \
     : (x) == 4  ? ADC_CHANNEL_4                                               \
     : (x) == 5  ? ADC_CHANNEL_5                                               \
     : (x) == 6  ? ADC_CHANNEL_6                                               \
     : (x) == 7  ? ADC_CHANNEL_7                                               \
     : (x) == 8  ? ADC_CHANNEL_8                                               \
     : (x) == 9  ? ADC_CHANNEL_9                                               \
     : (x) == 10 ? ADC_CHANNEL_10                                              \
     : (x) == 11 ? ADC_CHANNEL_11                                              \
     : (x) == 12 ? ADC_CHANNEL_12                                              \
     : (x) == 13 ? ADC_CHANNEL_13                                              \
     : (x) == 14 ? ADC_CHANNEL_14                                              \
     : (x) == 15 ? ADC_CHANNEL_15                                              \
                 : NULL)

#define __HAL_RCC_ADCx_CLK_ENABLE(x)                                           \
    do {                                                                       \
        if ((x) == ADC1) {                                                     \
            __HAL_RCC_ADC1_CLK_ENABLE();                                       \
        } else if ((x) == ADC2) {                                              \
            __HAL_RCC_ADC2_CLK_ENABLE();                                       \
        } else if ((x) == ADC3) {                                              \
            __HAL_RCC_ADC3_CLK_ENABLE();                                       \
        }                                                                      \
    } while (0)

typedef struct {
    ADC_TypeDef *ADCx;
    uint8_t Channel[8];
    GPIOxPiny_t GPIOxPiny[8];

    uint32_t Trigger;
    uint8_t Continuous;

    ADC_HandleTypeDef Handler;
} ADC_t;

void ADC_Init(ADC_t *Self);
void ADC_Start(ADC_t *Self);
void ADC_DMAStart(ADC_t *Self, uint32_t *Data, uint32_t Length);
uint32_t ADC_GetValue(ADC_t *Self);

#endif