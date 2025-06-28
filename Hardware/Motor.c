#include "Motor.h"
#include "PWM.h"

void Motor_Init(Motor_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_MODE_AF_PP;
    GPIO_InitPin(&GPIO, Self->PWM);

    GPIO.Mode = GPIO_MODE_OUTPUT_PP;
    Self->IN1_ODR = GPIO_InitPin(&GPIO, Self->IN1);
    Self->IN2_ODR = GPIO_InitPin(&GPIO, Self->IN2);

    PWM_t PWM = {
        .TIMx = Self->TIMx,
        .Prescaler = 100 - 1,
        .Period = Self->Range - 1,
        .Channel = {Self->Channel},
    };
    PWM_Init(&PWM);
}
void Motor_Set(Motor_t *Self, int16_t Speed) {
    GPIO_Write(Self->IN1_ODR, Speed >= 0 ? Self->Invert : !Self->Invert);
    GPIO_Write(Self->IN2_ODR, Speed >= 0 ? !Self->Invert : Self->Invert);
}