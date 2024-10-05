#include <string.h>

#include "GPIO.h"
#include "LED.h"

void LED_Init(LED_Handler *led) {
    GPIO_Handler gpio = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
    };
    strcpy(gpio.GPIOxPiny, led->GPIOxPiny);
    GPIO_Init(&gpio);

    led->GPIOx = gpio.GPIOx;
    led->GPIO_Pin = gpio.GPIO_Pin;

    LED_Off(led);
}

void LED_On(LED_Handler *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(LED_Handler *led) {
    HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_SET);
}

void LED_Toggle(LED_Handler *led) {
    HAL_GPIO_TogglePin(led->GPIOx, led->GPIO_Pin);
}