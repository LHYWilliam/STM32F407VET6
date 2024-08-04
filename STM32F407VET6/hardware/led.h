#ifndef LED_H
#define LED_H

#include "stm32f4xx_hal.h"

#define LOW 0
#define HIGH 1

typedef struct {
    char GPIOxPiny[4];
    uint8_t Mode;

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} LED;

void LED_init(LED *led);

void LED_on(LED *led);
void LED_off(LED *led);
void LED_toggle(LED *led);

#endif