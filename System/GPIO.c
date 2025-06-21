#include <string.h>

#include "GPIO.h"

uint32_t GPIO_IDR(const GPIOxPiny_t Pin);
uint32_t GPIO_ODR(const GPIOxPiny_t Pin);

uint32_t GPIO_InitPin(GPIO_t *Self, const GPIOxPiny_t Pin) {
    GPIO_InitTypeDef GPIO = {
        .Pin = GPIO_Pinx(Pin),
        .Mode = Self->Mode,
        .Pull = Self->Pull,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = Self->Alternate,
    };

    HAL_RCC_GPIOx_CLK_ENABLE(GPIOx(Pin));

    HAL_GPIO_Init(GPIOx(Pin), &GPIO);

    if (Self->Mode == GPIO_MODE_ANALOG || Self->Mode == GPIO_MODE_INPUT ||
        Self->Mode == GPIO_MODE_IT_RISING ||
        Self->Mode == GPIO_MODE_IT_FALLING ||
        Self->Mode == GPIO_MODE_IT_RISING_FALLING) {
        return GPIO_IDR(Pin);
    } else {
        return GPIO_ODR(Pin);
    }
}

uint32_t GPIO_IDR(const GPIOxPiny_t Pin) {
    return (
        Pin[0] == 'A'   ? (Pin[2] ? BITBAND(GPIOA_IDR, (10 + (Pin[2] - '0')))
                                  : BITBAND(GPIOA_IDR, (Pin[1] - '0')))
        : Pin[0] == 'B' ? (Pin[2] ? BITBAND(GPIOB_IDR, (10 + (Pin[2] - '0')))
                                  : BITBAND(GPIOB_IDR, (Pin[1] - '0')))
        : Pin[0] == 'C' ? (Pin[2] ? BITBAND(GPIOC_IDR, (10 + (Pin[2] - '0')))
                                  : BITBAND(GPIOC_IDR, (Pin[1] - '0')))
        : Pin[0] == 'D' ? (Pin[2] ? BITBAND(GPIOD_IDR, (10 + (Pin[2] - '0')))
                                  : BITBAND(GPIOD_IDR, (Pin[1] - '0')))
        : Pin[0] == 'E' ? (Pin[2] ? BITBAND(GPIOE_IDR, (10 + (Pin[2] - '0')))
                                  : BITBAND(GPIOE_IDR, (Pin[1] - '0')))
                        : NULL);
}

uint32_t GPIO_ODR(const GPIOxPiny_t Pin) {
    return (Pin[0] == 'A'   ? (Pin[2] ? BITBAND(GPIOA_ODR, (10 + Pin[2] - '0'))
                                      : BITBAND(GPIOA_ODR, (Pin[1] - '0')))
            : Pin[0] == 'B' ? (Pin[2] ? BITBAND(GPIOB_ODR, (10 + Pin[2] - '0'))
                                      : BITBAND(GPIOB_ODR, (Pin[1] - '0')))
            : Pin[0] == 'C' ? (Pin[2] ? BITBAND(GPIOC_ODR, (10 + Pin[2] - '0'))
                                      : BITBAND(GPIOC_ODR, (Pin[1] - '0')))
            : Pin[0] == 'D' ? (Pin[2] ? BITBAND(GPIOD_ODR, (10 + Pin[2] - '0'))
                                      : BITBAND(GPIOD_ODR, (Pin[1] - '0')))
            : Pin[0] == 'E' ? (Pin[2] ? BITBAND(GPIOE_ODR, (10 + Pin[2] - '0'))
                                      : BITBAND(GPIOE_ODR, (Pin[1] - '0')))
                            : NULL);
}