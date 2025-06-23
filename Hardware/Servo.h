#ifndef SERVO_H
#define SERVO_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "PWM.h"

typedef struct {
    PWM_t PWM;
} Servo_t;

void Servo_Init(Servo_t *Self);
void Servo_SetAngle(Servo_t *Self, uint8_t Channel, float Angle);
void Servo_UpdateCompare(Servo_t *Self, uint8_t Channel, int32_t Delta);

#endif