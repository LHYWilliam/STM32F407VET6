#ifndef LED_H
#define LED_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

typedef enum {
    LEDMode_PullDown,
    LEDMode_PullUp,
} LEDMode_t;

typedef struct {
    GPIOxPiny_t GPIOxPiny;
    LEDMode_t Mode;

    uint32_t ODR;
} LED_t;

void LED_Init(LED_t *self);
void LED_On(LED_t *self);
void LED_Off(LED_t *self);
void LED_Toggle(LED_t *self);

#endif