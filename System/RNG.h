#ifndef RNG_H
#define RNG_H

#include CMSIS_device_header

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    RNG_HandleTypeDef Handler;
} RNG_t;

void RNG_Init(RNG_t *self);

uint32_t RNG_RandomInt(RNG_t *self, int min, int max);

#ifdef __cplusplus
}
#endif

#endif