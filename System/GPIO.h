#ifndef GPIO_H
#define GPIO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define BITBAND(addr, bitnum)                                                  \
    ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)         *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR              (GPIOA_BASE + 20)
#define GPIOB_ODR              (GPIOB_BASE + 20)
#define GPIOC_ODR              (GPIOC_BASE + 20)
#define GPIOD_ODR              (GPIOD_BASE + 20)
#define GPIOE_ODR              (GPIOE_BASE + 20)

#define GPIOA_IDR              (GPIOA_BASE + 16)
#define GPIOB_IDR              (GPIOB_BASE + 16)
#define GPIOC_IDR              (GPIOC_BASE + 16)
#define GPIOD_IDR              (GPIOD_BASE + 16)
#define GPIOE_IDR              (GPIOE_BASE + 16)

#define PAout(n)               BIT_ADDR(GPIOA_ODR, n)
#define PBout(n)               BIT_ADDR(GPIOB_ODR, n)
#define PCout(n)               BIT_ADDR(GPIOC_ODR, n)
#define PDout(n)               BIT_ADDR(GPIOD_ODR, n)
#define PEout(n)               BIT_ADDR(GPIOE_ODR, n)

#define PAin(n)                BIT_ADDR(GPIOA_IDR, n)
#define PBin(n)                BIT_ADDR(GPIOB_IDR, n)
#define PCin(n)                BIT_ADDR(GPIOC_IDR, n)
#define PDin(n)                BIT_ADDR(GPIOD_IDR, n)
#define PEin(n)                BIT_ADDR(GPIOE_IDR, n)

#define A0                     "A0"
#define A1                     "A1"
#define A2                     "A2"
#define A3                     "A3"
#define A4                     "A4"
#define A5                     "A5"
#define A6                     "A6"
#define A7                     "A7"
#define A8                     "A8"
#define A9                     "A9"
#define A10                    "A10"
#define A11                    "A11"
#define A12                    "A12"
#define A13                    "A13"
#define A14                    "A14"
#define A15                    "A15"

#define B0                     "B0"
#define B1                     "B1"
#define B2                     "B2"
#define B3                     "B3"
#define B4                     "B4"
#define B5                     "B5"
#define B6                     "B6"
#define B7                     "B7"
#define B8                     "B8"
#define B9                     "B9"
#define B10                    "B10"
#define B11                    "B11"
#define B12                    "B12"
#define B13                    "B13"
#define B14                    "B14"
#define B15                    "B15"

#define C0                     "C0"
#define C1                     "C1"
#define C2                     "C2"
#define C3                     "C3"
#define C4                     "C4"
#define C5                     "C5"
#define C6                     "C6"
#define C7                     "C7"
#define C8                     "C8"
#define C9                     "C9"
#define C10                    "C10"
#define C11                    "C11"
#define C12                    "C12"
#define C13                    "C13"
#define C14                    "C14"
#define C15                    "C15"

#define D0                     "D0"
#define D1                     "D1"
#define D2                     "D2"
#define D3                     "D3"
#define D4                     "D4"
#define D5                     "D5"
#define D6                     "D6"
#define D7                     "D7"
#define D8                     "D8"
#define D9                     "D9"
#define D10                    "D10"
#define D11                    "D11"
#define D12                    "D12"
#define D13                    "D13"
#define D14                    "D14"
#define D15                    "D15"

#define E0                     "E0"
#define E1                     "E1"
#define E2                     "E2"
#define E3                     "E3"
#define E4                     "E4"
#define E5                     "E5"
#define E6                     "E6"
#define E7                     "E7"
#define E8                     "E8"
#define E9                     "E9"
#define E10                    "E10"
#define E11                    "E11"
#define E12                    "E12"
#define E13                    "E13"
#define E14                    "E14"
#define E15                    "E15"

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
        }                                                                      \
    } while (0)

#define GPIOx(x)                                                               \
    ((x[0]) == 'A'   ? GPIOA                                                   \
     : (x[0]) == 'B' ? GPIOB                                                   \
     : (x[0]) == 'C' ? GPIOC                                                   \
     : (x[0]) == 'D' ? GPIOD                                                   \
     : (x[0]) == 'E' ? GPIOE                                                   \
                     : NULL)

#define GPIO_Pin(x)        (x[2] ? 10 + x[2] - '0' : x[1] - '0')
#define GPIO_Pinx(x)       GPIO_PIN_0 << GPIO_Pin(x)

#define GPIO_Write(x, val) MEM_ADDR((x)) = ((val) ? 1 : 0)
#define GPIO_Toggle(x)     MEM_ADDR((x)) = !MEM_ADDR((x))
#define GPIO_ReadInput(x)  MEM_ADDR((x))
#define GPIO_ReadOutput(x) MEM_ADDR((x))

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
     : (x) == TIM12 ? GPIO_AF9_TIM12                                           \
                    : NULL)

typedef char GPIOxPiny_t[4];

typedef struct {
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Alternate;
} GPIO_t;

uint32_t GPIO_InitPin(GPIO_t *Self, const GPIOxPiny_t Pin);

#endif