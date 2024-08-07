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

                .PeriphInc = (dma->invert ? dma->targetInc : dma->sourceInc)
                                 ? DMA_PINC_ENABLE
                                 : DMA_PINC_DISABLE,
                .PeriphDataAlignment = DMA_PDATAALIGN_SIZE(
                    dma->invert ? dma->targetSize : dma->sourceSize),

                .MemInc = (dma->invert ? dma->sourceInc : dma->targetInc)
                              ? DMA_MINC_ENABLE
                              : DMA_MINC_DISABLE,
                .MemDataAlignment = DMA_MDATAALIGN_SIZE(
                    dma->invert ? dma->sourceSize : dma->targetSize),
            },
    };
    HAL_DMA_Init(&dma->Handler);
}