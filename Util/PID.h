#ifndef PID_H
#define PID_H

#include "RTE_Components.h"
#include CMSIS_device_header

#define LIMIT(data, low, high)                                                 \
    do {                                                                       \
        data = data < low ? low : data;                                        \
        data = data > high ? high : data;                                      \
    } while (0)

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    uint16_t IMax;

    float Integrator;
    float LastError, LastDerivative;

    uint32_t LastTime;

    FlagStatus NaN;
} PID_t;

float PID_Caculate(PID_t *Self, float Error);

#endif