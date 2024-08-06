#include "dma.h"

void DMA_init(DMA *dma) {
    dma->Handler = (DMA_HandleTypeDef){
        .Instance = DMAx_Streamy(dma->DMAx, dma->stream),
        .Init =
            {
                .Channel = DMA_Channelx(dma->channel),

                .Mode = DMA_CIRCULAR,
                .Priority = DMA_PRIORITY_HIGH,
                .Direction =
                    dma->invert ? DMA_MEMORY_TO_PERIPH : DMA_PERIPH_TO_MEMORY,

                .PeriphInc =
                    dma->sourceInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE,
                .PeriphDataAlignment = DMA_PDATAALIGN_SIZE(dma->sourceSize),

                .MemInc = dma->targetInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE,
                .MemDataAlignment = DMA_MDATAALIGN_SIZE(dma->targetSize),
            },
    };
    HAL_DMA_Init(&dma->Handler);
}