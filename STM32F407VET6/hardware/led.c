#include "stm32f4xx_hal.h"

#include "string.h"

#include "gpio.h"
#include "led.h"

void LED_init(LED *led) {
    GPIO led_gpio = {
        .Mode = {GPIO_MODE_OUTPUT_PP},
        .Pull = {led->Mode ? GPIO_PULLDOWN : GPIO_PULLUP},
    };
    strcpy(led_gpio.GPIOxPiny, led->GPIOxPiny);
    GPIO_init(&led_gpio);

    led->GPIOx = led_gpio.GPIOx[0];
    led->GPIO_Pin = led_gpio.GPIO_Pin[0];
}

void LED_on(LED *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin,
                      led->Mode ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_off(LED *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin,
                      led->Mode ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void LED_toggle(LED *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin,
                      ~HAL_GPIO_ReadPin(led->GPIOx, led->GPIO_Pin));
}