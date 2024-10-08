#include <string.h>

#include "Delay.h"
#include "GPIO.h"
#include "Key.h"

void Key_Init(Key_Handler *self) {
    GPIO_Handler gpio = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLDOWN,
    };
    strcpy(gpio.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init(&gpio);

    self->GPIOx = gpio.GPIOx;
    self->GPIO_Pin = gpio.GPIO_Pin;
}

KeyState Key_Read(Key_Handler *self) {
    if (HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin)) {
        Delay_ms(10);
        while (HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin)) {
        }

        Delay_ms(10);
        return KeyDown;
    }

    return NoKeyDown;
}
