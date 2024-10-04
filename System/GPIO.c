#include <string.h>

#include "GPIO.h"

void GPIO_Init(GPIO *gpio) {
    char *temp = gpio->GPIOxPiny;

    GPIO_InitTypeDef GPIO_Init = {
        .Mode = gpio->Mode,
        .Pull = gpio->Pull,
        .Alternate = gpio->Alternate,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };

    uint8_t count = 0;
    do {
        gpio->GPIOx = GPIOx(temp);
        gpio->GPIO_Pin = GPIO_Pinx(temp);

        HAL_RCC_GPIOx_CLK_ENABLE(gpio->GPIOx);

        GPIO_Init.Pin = gpio->GPIO_Pin;
        HAL_GPIO_Init(gpio->GPIOx, &GPIO_Init);

    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (count = count + 1));
}