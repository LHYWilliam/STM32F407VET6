#include <string.h>

#include "Servo.h"

void Servo_Init(Servo_t *Self) {
    Self->PWM->TIMx = Self->TIMx;

    for (uint8_t i = 0; Self->Channel[i]; i++) {
        uint8_t j = 0;
        while (Self->PWM->Channel[j] != 0) {
            j++;
        }

        Self->PWM->Channel[j] = Self->Channel[i];
        strcpy(Self->PWM->GPIOxPiny[j], Self->GPIOxPiny[i]);
    }

    if (Self->TIMx == TIM1 || Self->TIMx == TIM8) {
        Self->PWM->Prescaler = 168 - 1;

    } else {
        Self->PWM->Prescaler = 84 - 1;
    }

    Self->PWM->Period = 20000 - 1;

    if (Self->PWM_Init) {
        PWM_Init(Self->PWM);
    }
}

void Servo_SetCompare180(Servo_t *Self, uint8_t Channel, int32_t Compare) {
    if (Compare < 500) {
        Compare = 500;
    } else if (Compare > 2500) {
        Compare = 2500;
    }

    Self->Compare[Channel - 1] = Compare;
    Self->Angle[Channel - 1] = (float)(Compare - 500) / 2000 * 180.0f;

    PWM_SetSetCompare(Self->PWM, Channel, Self->Compare[Channel - 1]);
}

void Servo_SetCompare270(Servo_t *Self, uint8_t Channel, int32_t Compare) {
    if (Compare < 500) {
        Compare = 500;
    } else if (Compare > 2500) {
        Compare = 2500;
    }

    Self->Compare[Channel - 1] = Compare;
    Self->Angle[Channel - 1] = (float)(Compare - 500) / 2000 * 270.0f;

    PWM_SetSetCompare(Self->PWM, Channel, Self->Compare[Channel - 1]);
}

void Servo_SetAngle180(Servo_t *Self, uint8_t Channel, float Angle) {
    if (Angle < 0.0f) {
        Angle = 0.0f;
    } else if (Angle > 180.0f) {
        Angle = 180.0f;
    }

    Self->Angle[Channel - 1] = Angle;
    Self->Compare[Channel - 1] = (uint32_t)((Angle / 180.0f) * 2000 + 500);

    PWM_SetSetCompare(Self->PWM, Channel, Self->Compare[Channel - 1]);
}

void Servo_SetAngle270(Servo_t *Self, uint8_t Channel, float Angle) {
    if (Angle < 0.0f) {
        Angle = 0.0f;
    } else if (Angle > 270.0f) {
        Angle = 270.0f;
    }

    Self->Angle[Channel - 1] = Angle;
    Self->Compare[Channel - 1] = (uint32_t)((Angle / 270.0f) * 2000 + 500);

    PWM_SetSetCompare(Self->PWM, Channel, Self->Compare[Channel - 1]);
}

void Servo_UpdateCompare180(Servo_t *Self, uint8_t Channel, int32_t Delta) {
    Servo_SetCompare180(Self, Channel, Self->Compare[Channel - 1] + Delta);
}

void Servo_UpdateCompare270(Servo_t *Self, uint8_t Channel, int32_t Delta) {
    Servo_SetCompare270(Self, Channel, Self->Compare[Channel - 1] + Delta);
}

void Servo_UpdateAngle180(Servo_t *Self, uint8_t Channel, float Delta) {
    Servo_SetAngle180(Self, Channel, Self->Angle[Channel - 1] + Delta);
}

void Servo_UpdateAngle270(Servo_t *Self, uint8_t Channel, float Delta) {
    Servo_SetAngle270(Self, Channel, Self->Angle[Channel - 1] + Delta);
}