#ifndef LED_H
#define LED_H

#include "stm32f4xx_hal.h"

typedef struct {
    char GPIOxPiny[4];

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} LED_t;

void LED_Init(LED_t *self);

void LED_On(LED_t *self);
void LED_Off(LED_t *self);
void LED_Toggle(LED_t *self);

#endif