#include "DMA.h"

void DMA_Init(DMA_Handler *dma) {
    if (dma->Direction == DMA_MEMORY_TO_MEMORY) {
        __HAL_RCC_DMAx_CLK_ENABLE(dma->DMAx);
    }

    dma->Handler = (DMA_HandleTypeDef){
        .Instance = DMAx_Streamy(dma->DMAx, dma->Stream),
        .Init =
            {
                .Channel = DMA_Channelx(dma->Channel),

                .PeriphInc =
                    dma->PeriphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE,
                .PeriphDataAlignment = DMA_PDATAALIGN_SIZE(dma->PeriphSize),

                .MemInc = dma->MemInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE,
                .MemDataAlignment = DMA_MDATAALIGN_SIZE(dma->MemSize),

                .Mode = dma->Mode,
                .Direction = dma->Direction,
                .Priority = DMA_PRIORITY_HIGH,
            },
    };
    HAL_DMA_Init(&dma->Handler);

    if (dma->Interrupt) {
        HAL_NVIC_SetPriority(DMAx_Streamy_IRQn(dma->DMAx, dma->Stream), 8, 0);
        HAL_NVIC_EnableIRQ(DMAx_Streamy_IRQn(dma->DMAx, dma->Stream));
    }
}