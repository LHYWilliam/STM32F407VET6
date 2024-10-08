#include "RNG.h"

void RNG_Init(RNG_Handler *self) {
    self->Handler = (RNG_HandleTypeDef){
        .Instance = RNG,
    };
    HAL_RNG_Init(&self->Handler);

    while (__HAL_RNG_GET_FLAG(&self->Handler, RNG_FLAG_DRDY) == RESET)
        ;
}

uint32_t RNG_RandomInt(RNG_Handler *self, int min, int max) {
    return HAL_RNG_GetRandomNumber(&self->Handler) % (max - min + 1) + min;
}