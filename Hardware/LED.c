#include "LED.h"

void LED_Init(LED_t *Self) {
    GPIO_t GPIO = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
    };

    Self->ODR = GPIO_InitPin(&GPIO, Self->GPIOxPiny);

    LED_Off(Self);
}

void LED_On(LED_t *Self) { GPIO_Write(Self->ODR, !Self->Mode); }

void LED_Off(LED_t *Self) { GPIO_Write(Self->ODR, Self->Mode); }

void LED_Toggle(LED_t *Self) { GPIO_Toggle(Self->ODR); }