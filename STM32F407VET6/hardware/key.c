#include <string.h>

#include "gpio.h"
#include "key.h"

void Key_init(Key *key) {
    GPIO key_gpio = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = key->Mode ? GPIO_PULLDOWN : GPIO_PULLUP,
    };
    strcpy(key_gpio.GPIOxPiny, key->GPIOxPiny);
    GPIO_init(&key_gpio);

    key->GPIOx = key_gpio.GPIOx[0];
    key->GPIO_Pin = key_gpio.GPIO_Pin[0];
}

uint8_t Key_read(Key *key) {
    if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == key->Mode) {
        HAL_Delay(10);
        if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == key->Mode) {

            return KEYDOWN;
        }
    }

    return NOKEYDOWN;
}
