#include <string.h>

#include "GPIO.h"

void GPIO_Init(GPIO_t *self) {
    GPIO_InitTypeDef GPIO_Init = {
        .Mode = self->Mode,
        .Pull = self->Pull,
        .Alternate = self->Alternate,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    };

    uint8_t count = 0;
    char *temp = self->GPIOxPiny;
    do {
        self->GPIOx = GPIOx(temp);
        self->GPIO_Pin = GPIO_Pinx(temp);

        HAL_RCC_GPIOx_CLK_ENABLE(self->GPIOx);

        GPIO_Init.Pin = self->GPIO_Pin;
        HAL_GPIO_Init(self->GPIOx, &GPIO_Init);

    } while ((temp = strchr(temp, '|')) && (temp = temp + 2) &&
             (count = count + 1));
}