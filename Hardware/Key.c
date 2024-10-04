#include <string.h>

#include "Delay.h"
#include "GPIO.h"
#include "Key.h"

void Key_Init(Key *key) {
    GPIO key_gpio = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLDOWN,
    };
    strcpy(key_gpio.GPIOxPiny, key->GPIOxPiny);
    GPIO_Init(&key_gpio);

    key->GPIOx = key_gpio.GPIOx;
    key->GPIO_Pin = key_gpio.GPIO_Pin;
}

KeyState Key_Read(Key *key) {
    if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin)) {
        Delay_ms(10);
        while (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin)) {
        }

        Delay_ms(10);
        return KeyDown;
    }

    return NoKeyDown;
}
