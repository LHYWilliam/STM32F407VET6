#ifndef TIM_H
#define TIM_H

#include "stm32f4xx_hal.h"

#define __HAL_RCC_TIMx_CLK_ENABLE(x)                                           \
    do {                                                                       \
        if ((x) == TIM1) {                                                     \
            __HAL_RCC_TIM1_CLK_ENABLE();                                       \
        } else if ((x) == TIM2) {                                              \
            __HAL_RCC_TIM2_CLK_ENABLE();                                       \
        } else if ((x) == TIM3) {                                              \
            __HAL_RCC_TIM3_CLK_ENABLE();                                       \
        } else if ((x) == TIM4) {                                              \
            __HAL_RCC_TIM4_CLK_ENABLE();                                       \
        } else if ((x) == TIM5) {                                              \
            __HAL_RCC_TIM5_CLK_ENABLE();                                       \
        } else if ((x) == TIM6) {                                              \
            __HAL_RCC_TIM6_CLK_ENABLE();                                       \
        } else if ((x) == TIM7) {                                              \
            __HAL_RCC_TIM7_CLK_ENABLE();                                       \
        } else if ((x) == TIM8) {                                              \
            __HAL_RCC_TIM8_CLK_ENABLE();                                       \
        } else if ((x) == TIM9) {                                              \
            __HAL_RCC_TIM9_CLK_ENABLE();                                       \
        } else if ((x) == TIM10) {                                             \
            __HAL_RCC_TIM10_CLK_ENABLE();                                      \
        } else if ((x) == TIM11) {                                             \
            __HAL_RCC_TIM11_CLK_ENABLE();                                      \
        } else if ((x) == TIM12) {                                             \
            __HAL_RCC_TIM12_CLK_ENABLE();                                      \
        } else if ((x) == TIM13) {                                             \
            __HAL_RCC_TIM13_CLK_ENABLE();                                      \
        } else if ((x) == TIM14) {                                             \
            __HAL_RCC_TIM14_CLK_ENABLE();                                      \
        }                                                                      \
    } while (0)

#define TIMx_IRQN(x)                                                           \
    ((x) == TIM3   ? TIM3_IRQn                                                 \
     : (x) == TIM4 ? TIM4_IRQn                                                 \
     : (x) == TIM5 ? TIM5_IRQn                                                 \
     : (x) == TIM7 ? TIM7_IRQn                                                 \
                   : NULL)

typedef struct {
    TIM_TypeDef *TIM;
    uint32_t Prescaler;
    uint32_t Period;

    TIM_HandleTypeDef *Handler;
} TIM;

typedef struct {
    TIM_TypeDef *TIM;
    uint32_t ms;

    TIM_HandleTypeDef Handler;
} Timer;

void TIM_init(TIM *tim);
void Timer_init(Timer *timer);

#endif