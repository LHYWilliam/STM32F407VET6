#include "RNG.h"

void RNG_Init(RNG_Handler *rng) {
    rng->Handler = (RNG_HandleTypeDef){
        .Instance = RNG,
    };
    HAL_RNG_Init(&rng->Handler);

    while (__HAL_RNG_GET_FLAG(&rng->Handler, RNG_FLAG_DRDY) == RESET)
        ;
}

uint32_t RNG_RandomInt(RNG_Handler *rng, int min, int max) {
    return HAL_RNG_GetRandomNumber(&rng->Handler) % (max - min + 1) + min;
}