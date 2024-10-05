#ifndef LED_H
#define LED_H

#include "stm32f4xx_hal.h"

typedef struct {
    char GPIOxPiny[4];

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} LED_Handler;

void LED_Init(LED_Handler *led);

void LED_On(LED_Handler *led);
void LED_Off(LED_Handler *led);
void LED_Toggle(LED_Handler *led);

#endif