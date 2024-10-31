#ifndef ADC_H
#define ADC_H

#include CMSIS_device_header

#undef ADC

#define ADC_EXTERNALTRIGCONV_Tx_TRGO(x)                                        \
    ((x) == TIM2   ? ADC_EXTERNALTRIGCONV_T2_TRGO                              \
     : (x) == TIM3 ? ADC_EXTERNALTRIGCONV_T3_TRGO                              \
     : (x) == TIM5 ? ADC_EXTERNALTRIGCONV_T8_TRGO                              \
                   : NULL)

#define ADC_CHANNEL_x(x)                                                       \
    ((x[0]) == '0'   ? ADC_CHANNEL_0                                           \
     : (x[1]) == '8' ? ADC_CHANNEL_18                                          \
     : (x[1]) == '7' ? ADC_CHANNEL_17                                          \
     : (x[1]) == '6' ? ADC_CHANNEL_16                                          \
     : (x[1]) == '5' ? ADC_CHANNEL_15                                          \
     : (x[1]) == '4' ? ADC_CHANNEL_14                                          \
     : (x[1]) == '3' ? ADC_CHANNEL_13                                          \
     : (x[1]) == '2' ? ADC_CHANNEL_12                                          \
     : (x[1]) == '1' ? ADC_CHANNEL_11                                          \
     : (x[1]) == '0' ? ADC_CHANNEL_10                                          \
     : (x[0]) == '9' ? ADC_CHANNEL_9                                           \
     : (x[0]) == '8' ? ADC_CHANNEL_8                                           \
     : (x[0]) == '7' ? ADC_CHANNEL_7                                           \
     : (x[0]) == '6' ? ADC_CHANNEL_6                                           \
     : (x[0]) == '5' ? ADC_CHANNEL_5                                           \
     : (x[0]) == '4' ? ADC_CHANNEL_4                                           \
     : (x[0]) == '3' ? ADC_CHANNEL_3                                           \
     : (x[0]) == '2' ? ADC_CHANNEL_2                                           \
     : (x[0]) == '1' ? ADC_CHANNEL_1                                           \
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
    char Channel[32];

    char GPIOxPiny[32];

    uint32_t Trigger;
    uint8_t Continuous;

    ADC_HandleTypeDef Handler;
} ADC_t;

void ADC_Init(ADC_t *self);
void ADC_Start(ADC_t *self);
void ADC_DMAStart(ADC_t *self, uint32_t *data, uint32_t length);

uint32_t ADC_Get(ADC_t *self);

#endif