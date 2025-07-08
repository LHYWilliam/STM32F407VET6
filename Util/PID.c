#include "Time.h"

#include "PID.h"

float RC = 1 / (2 * 3.14 * 20);

int32_t PID_Caculate(PID_t *Self, float Error) {
    float Output = 0;
    uint32_t Now = Time_Getms();
    float dt = (float)(Now - Self->LastTime) / 1000;

    if (Self->LastTime == 0 || dt > 1) {
        Self->Integrator = dt = 0;
        Self->NaN = SET;
    }
    Self->LastTime = Now;

    Output += Error * Self->Kp;

    if (Self->Kd && dt) {
        float derivative;
        if (Self->NaN == SET) {
            derivative = 0;
            Self->LastDerivative = 0;
            Self->NaN = RESET;
        } else {
            derivative = (Error - Self->LastError) / dt;
        }

        derivative = Self->LastDerivative +
                     (dt / (RC + dt)) * (derivative - Self->LastDerivative);
        Self->LastError = Error;
        Self->LastDerivative = derivative;

        Output += Self->Kd * derivative;
    }

    if (Self->Ki && dt) {
        Self->Integrator += Error * Self->Ki * dt;
        LIMIT(Self->Integrator, -Self->IMax, Self->IMax);

        Output += Self->Integrator;
    }

    return (int32_t)Output;
}