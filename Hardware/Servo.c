#include "Servo.h"
#include "TIM.h"

void Servo_Init(Servo_t *Self) {
    Self->PWM.Prescaler = 84 - 1;
    Self->PWM.Period = 20000 - 1;
    PWM_Init(&Self->PWM);
}

void Servo_SetAngle(Servo_t *Self, uint8_t Channel, float Angle) {
    PWM_SetSetCompare(&Self->PWM, Channel,
                      (uint32_t)((Angle / 180.0f) * 2000 + 500));
}

void Servo_UpdateCompare(Servo_t *Self, uint8_t Channel, int32_t Delta) {
    int32_t Value =
        __HAL_TIM_GetCompare(&Self->PWM.Handler, TIM_CHANNEL_x(Channel));
    Value += Delta;

    if (Value + Delta < 500) {
        Value = 500;
    } else if (Value > 2500) {
        Value = 2500;
    }
    
    __HAL_TIM_SetCompare(&Self->PWM.Handler, TIM_CHANNEL_x(Channel), Value);
}