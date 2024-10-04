#ifndef SIGNAL_H
#define SIGNAL_H

// #include "arm_math.h"

#include "Timer.h"

#define SignalGenerator_init Timer_init

#define SignalSampler_init   Timer_init

typedef Timer SignalGenerator;

typedef Timer SignalSampler;

void Sing_Generator(uint32_t *data, uint32_t length);

#endif