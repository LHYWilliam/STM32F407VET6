#include <string.h>

#include "GPIO.h"
#include "LED.h"

void LED_Init(LED *led) {
    GPIO led_gpio = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
    };
    strcpy(led_gpio.GPIOxPiny, led->GPIOxPiny);
    GPIO_Init(&led_gpio);

    led->GPIOx = led_gpio.GPIOx;
    led->GPIO_Pin = led_gpio.GPIO_Pin;

    LED_Off(led);
}

void LED_On(LED *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(LED *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_SET);
}

void LED_Toggle(LED *led) { HAL_GPIO_TogglePin(led->GPIOx, led->GPIO_Pin); }