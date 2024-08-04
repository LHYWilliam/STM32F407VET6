#ifndef KEY_H
#define KEY_H

#include "stm32f4xx_hal.h"

#define LOW 0
#define HIGH 1

#define NOKEYDOWN 0
#define KEYDOWN 1

typedef struct {
    char GPIOxPiny[4];
    uint8_t Mode;

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} Key;

void Key_init(Key *key);

uint8_t Key_read(Key *key);

#endif