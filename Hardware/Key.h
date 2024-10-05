#ifndef KEY_H
#define KEY_H

#include "stm32f4xx_hal.h"

typedef enum { NoKeyDown, KeyDown } KeyState;

typedef struct {
    char GPIOxPiny[4];

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} Key_Handler;

void Key_Init(Key_Handler *key);

KeyState Key_Read(Key_Handler *key);

#endif