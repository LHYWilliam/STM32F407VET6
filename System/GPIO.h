#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"

#define HAL_RCC_GPIOx_CLK_ENABLE(x)                                            \
    do {                                                                       \
        if ((x) == GPIOA) {                                                    \
            __HAL_RCC_GPIOA_CLK_ENABLE();                                      \
        } else if ((x) == GPIOB) {                                             \
            __HAL_RCC_GPIOB_CLK_ENABLE();                                      \
        } else if ((x) == GPIOC) {                                             \
            __HAL_RCC_GPIOC_CLK_ENABLE();                                      \
        } else if ((x) == GPIOD) {                                             \
            __HAL_RCC_GPIOD_CLK_ENABLE();                                      \
        } else if ((x) == GPIOE) {                                             \
            __HAL_RCC_GPIOE_CLK_ENABLE();                                      \
        } else if ((x) == GPIOF) {                                             \
            __HAL_RCC_GPIOF_CLK_ENABLE();                                      \
        } else if ((x) == GPIOG) {                                             \
            __HAL_RCC_GPIOG_CLK_ENABLE();                                      \
        } else if ((x) == GPIOH) {                                             \
            __HAL_RCC_GPIOH_CLK_ENABLE();                                      \
        } else if ((x) == GPIOI) {                                             \
            __HAL_RCC_GPIOI_CLK_ENABLE();                                      \
        }                                                                      \
    } while (0)

#define GPIOx(x)                                                               \
    ((x[0]) == 'A'   ? GPIOA                                                   \
     : (x[0]) == 'B' ? GPIOB                                                   \
     : (x[0]) == 'C' ? GPIOC                                                   \
     : (x[0]) == 'D' ? GPIOD                                                   \
     : (x[0]) == 'E' ? GPIOE                                                   \
     : (x[0]) == 'F' ? GPIOF                                                   \
     : (x[0]) == 'G' ? GPIOG                                                   \
     : (x[0]) == 'H' ? GPIOH                                                   \
     : (x[0]) == 'I' ? GPIOI                                                   \
                     : NULL)

#define GPIO_Pinx(x)                                                           \
    ((x[1]) == '0'   ? GPIO_PIN_0                                              \
     : (x[2]) == '5' ? GPIO_PIN_15                                             \
     : (x[2]) == '4' ? GPIO_PIN_14                                             \
     : (x[2]) == '3' ? GPIO_PIN_13                                             \
     : (x[2]) == '2' ? GPIO_PIN_12                                             \
     : (x[2]) == '1' ? GPIO_PIN_11                                             \
     : (x[2]) == '0' ? GPIO_PIN_10                                             \
     : (x[1]) == '9' ? GPIO_PIN_9                                              \
     : (x[1]) == '8' ? GPIO_PIN_8                                              \
     : (x[1]) == '7' ? GPIO_PIN_7                                              \
     : (x[1]) == '6' ? GPIO_PIN_6                                              \
     : (x[1]) == '5' ? GPIO_PIN_5                                              \
     : (x[1]) == '4' ? GPIO_PIN_4                                              \
     : (x[1]) == '3' ? GPIO_PIN_3                                              \
     : (x[1]) == '2' ? GPIO_PIN_2                                              \
     : (x[1]) == '1' ? GPIO_PIN_1                                              \
                     : NULL)

#define GPIO_AFx_TIMy(x)                                                       \
    ((x) == TIM1    ? GPIO_AF1_TIM1                                            \
     : (x) == TIM2  ? GPIO_AF1_TIM2                                            \
     : (x) == TIM3  ? GPIO_AF2_TIM3                                            \
     : (x) == TIM4  ? GPIO_AF2_TIM4                                            \
     : (x) == TIM5  ? GPIO_AF2_TIM5                                            \
     : (x) == TIM8  ? GPIO_AF3_TIM8                                            \
     : (x) == TIM9  ? GPIO_AF3_TIM9                                            \
     : (x) == TIM10 ? GPIO_AF3_TIM10                                           \
     : (x) == TIM11 ? GPIO_AF3_TIM11                                           \
                    : NULL)

typedef struct {
    char GPIOxPiny[129];

    uint32_t Mode;
    uint32_t Pull;
    uint32_t Alternate;

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} GPIO_Handler;

void GPIO_Init(GPIO_Handler *gpio);

#endif