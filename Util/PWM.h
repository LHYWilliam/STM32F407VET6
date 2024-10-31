#ifndef PWM_H
#define PWM_H

#include CMSIS_device_header

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    TIM_TypeDef *TIM;
    char Channel[32];
    uint32_t Prescaler;
    uint32_t Period;
    char GPIOxPiny[32];

    TIM_HandleTypeDef Handler;
} PWM_t;

void PWM_Init(PWM_t *self);

void PWM_Set(PWM_t *self, uint8_t channel, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif