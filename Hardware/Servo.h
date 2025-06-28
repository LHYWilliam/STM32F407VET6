#ifndef SERVO_H
#define SERVO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "PWM.h"

typedef struct {
    TIM_TypeDef *TIMx;
    uint8_t Channel[4];
    GPIOxPiny_t GPIOxPiny[4];

    PWM_t *PWM;
    FunctionalState PWM_Init;

    float Angle[4];
    uint32_t Compare[4];
} Servo_t;

void Servo_Init(Servo_t *Self);
void Servo_SetCompare180(Servo_t *Self, uint8_t Channel, int32_t Compare);
void Servo_SetCompare270(Servo_t *Self, uint8_t Channel, int32_t Compare);
void Servo_SetAngle180(Servo_t *Self, uint8_t Channel, float Angle);
void Servo_SetAngle270(Servo_t *Self, uint8_t Channel, float Angle);
void Servo_UpdateCompare180(Servo_t *Self, uint8_t Channel, int32_t Delta);
void Servo_UpdateCompare270(Servo_t *Self, uint8_t Channel, int32_t Delta);
void Servo_UpdateAngle180(Servo_t *Self, uint8_t Channel, float Delta);
void Servo_UpdateAngle270(Servo_t *Self, uint8_t Channel, float Delta);

#endif