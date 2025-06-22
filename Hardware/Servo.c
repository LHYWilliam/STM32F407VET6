#include "Servo.h"

void Servo_Init(Servo_t *Self) {
    Self->PWM.Prescaler = 84 - 1;
    Self->PWM.Period = 20000 - 1;
    PWM_Init(&Self->PWM);
}

void Servo_SetAngle(Servo_t *Self, uint8_t Channel, float Angle) {
    PWM_SetSetCompare(&Self->PWM, Channel,
                      (uint32_t)((Angle / 180.0f) * 2000 + 500));
}