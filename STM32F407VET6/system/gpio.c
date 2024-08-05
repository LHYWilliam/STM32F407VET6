#include <string.h>

#include "gpio.h"

void GPIO_init(GPIO *gpio) {
    char *temp = gpio->GPIOxPiny;

    uint8_t count = 0;
    do {
        gpio->GPIOx[count] = GPIOx(temp);
        gpio->GPIO_Pin[count] = GPIO_Pinx(temp);

        HAL_RCC_GPIOx_CLK_ENABLE(gpio->GPIOx[count]);

        GPIO_InitTypeDef GPIO_Init = {
            .Pin = gpio->GPIO_Pin[count],
            .Mode = gpio->Mode[count],
            .Pull = gpio->Pull[count],
            .Alternate = gpio->Alternate[count],
            .Speed = GPIO_SPEED_FREQ_HIGH,
        };
        HAL_GPIO_Init(gpio->GPIOx[count], &GPIO_Init);

    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (count = count + 1));
}