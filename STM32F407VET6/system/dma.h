#ifndef DMA_H
#define DMA_H

#include "stm32f4xx_hal.h"

#define DMA_Channelx(x)                                                        \
    ((x) == 0   ? DMA_CHANNEL_0                                                \
     : (x) == 1 ? DMA_CHANNEL_1                                                \
     : (x) == 2 ? DMA_CHANNEL_2                                                \
     : (x) == 3 ? DMA_CHANNEL_3                                                \
     : (x) == 4 ? DMA_CHANNEL_4                                                \
     : (x) == 5 ? DMA_CHANNEL_5                                                \
     : (x) == 6 ? DMA_CHANNEL_6                                                \
     : (x) == 7 ? DMA_CHANNEL_7                                                \
                : NULL)

#define DMAx_Streamy(x, y)                                                     \
    ((x) == DMA1 && y == 0   ? DMA1_Stream0                                    \
     : (x) == DMA1 && y == 1 ? DMA1_Stream1                                    \
     : (x) == DMA1 && y == 2 ? DMA1_Stream2                                    \
     : (x) == DMA1 && y == 3 ? DMA1_Stream3                                    \
     : (x) == DMA1 && y == 4 ? DMA1_Stream4                                    \
     : (x) == DMA1 && y == 5 ? DMA1_Stream5                                    \
     : (x) == DMA1 && y == 6 ? DMA1_Stream6                                    \
     : (x) == DMA1 && y == 7 ? DMA1_Stream7                                    \
     : (x) == DMA2 && y == 0 ? DMA2_Stream0                                    \
     : (x) == DMA2 && y == 1 ? DMA2_Stream1                                    \
     : (x) == DMA2 && y == 2 ? DMA2_Stream2                                    \
     : (x) == DMA2 && y == 3 ? DMA2_Stream3                                    \
     : (x) == DMA2 && y == 4 ? DMA2_Stream4                                    \
     : (x) == DMA2 && y == 5 ? DMA2_Stream5                                    \
     : (x) == DMA2 && y == 6 ? DMA2_Stream6                                    \
     : (x) == DMA2 && y == 7 ? DMA2_Stream7                                    \
                             : NULL)

#define DMA_PDATAALIGN_SIZE(x)                                                 \
    ((x) == 8    ? DMA_PDATAALIGN_BYTE                                         \
     : (x) == 16 ? DMA_PDATAALIGN_HALFWORD                                     \
     : (x) == 32 ? DMA_PDATAALIGN_WORD                                         \
                 : NULL)

#define DMA_MDATAALIGN_SIZE(x)                                                 \
    ((x) == 8    ? DMA_MDATAALIGN_BYTE                                         \
     : (x) == 16 ? DMA_MDATAALIGN_HALFWORD                                     \
     : (x) == 32 ? DMA_MDATAALIGN_WORD                                         \
                 : NULL)

#define __HAL_RCC_DMAx_CLK_ENABLE(x)                                           \
    do {                                                                       \
        if ((x) == DMA1) {                                                     \
            __HAL_RCC_DMA1_CLK_ENABLE();                                       \
        } else if ((x) == DMA2) {                                              \
            __HAL_RCC_DMA2_CLK_ENABLE();                                       \
        }                                                                      \
    } while (0)

typedef struct {
    DMA_TypeDef *DMAx;
    uint8_t channel;
    uint8_t stream;

    uint8_t sourceInc;
    uint8_t sourceSize;

    uint8_t targetInc;
    uint8_t targetSize;

    uint8_t invert;

    DMA_HandleTypeDef Handler;
} DMA;

void DMA_init(DMA *dma);

#endif