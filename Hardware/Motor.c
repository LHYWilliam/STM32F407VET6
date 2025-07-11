#include <string.h>

#include "Motor.h"
#include "PWM.h"

void Motor_Init(Motor_t *Self) {
    Self->_PWM->TIMx = Self->TIMx;

    uint8_t i = 0;
    while (Self->_PWM->Channel[i] != 0) {
        i++;
    }

    Self->_PWM->Channel[i] = Self->Channel;
    strcpy(Self->_PWM->GPIOxPiny[i], Self->PWM);

    Self->_PWM->Prescaler = 1 - 1;
    Self->_PWM->Period = Self->Range - 1;

    GPIO_t GPIO;

    GPIO.Mode = GPIO_MODE_OUTPUT_PP;
    Self->IN1_ODR = GPIO_InitPin(&GPIO, Self->IN1);
    Self->IN2_ODR = GPIO_InitPin(&GPIO, Self->IN2);

    if (Self->PWM_Init) {
        PWM_Init(Self->_PWM);
    }
}
void Motor_SetSpeed(Motor_t *Self, int32_t Speed) {
    PWM_SetSetCompare(Self->_PWM, Self->Channel, Speed >= 0 ? Speed : -Speed);
    GPIO_Write(Self->IN1_ODR, Speed >= 0 ? Self->Invert : !Self->Invert);
    GPIO_Write(Self->IN2_ODR, Speed >= 0 ? !Self->Invert : Self->Invert);
}