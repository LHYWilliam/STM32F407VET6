#include <string.h>

#include "GPIO.h"
#include "LED.h"

void LED_Init(LED_Handler *self) {
    GPIO_Handler gpio = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
    };
    strcpy(gpio.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init(&gpio);

    self->GPIOx = gpio.GPIOx;
    self->GPIO_Pin = gpio.GPIO_Pin;

    LED_Off(self);
}

void LED_On(LED_Handler *self) {
    HAL_GPIO_WritePin(self->GPIOx, self->GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(LED_Handler *self) {
    HAL_GPIO_WritePin(self->GPIOx, self->GPIO_Pin, GPIO_PIN_SET);
}

void LED_Toggle(LED_Handler *self) {
    HAL_GPIO_TogglePin(self->GPIOx, self->GPIO_Pin);
}