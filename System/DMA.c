#include "DMA.h"

void DMA_Init(DMA_Handler *self) {
    if (self->Direction == DMA_MEMORY_TO_MEMORY) {
        __HAL_RCC_DMAx_CLK_ENABLE(self->DMAx);
    }

    self->Handler = (DMA_HandleTypeDef){
        .Instance = DMAx_Streamy(self->DMAx, self->Stream),
        .Init =
            {
                .Channel = DMA_Channelx(self->Channel),

                .PeriphInc =
                    self->PeriphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE,
                .PeriphDataAlignment = DMA_PDATAALIGN_SIZE(self->PeriphSize),

                .MemInc = self->MemInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE,
                .MemDataAlignment = DMA_MDATAALIGN_SIZE(self->MemSize),

                .Mode = self->Mode,
                .Direction = self->Direction,
                .Priority = DMA_PRIORITY_HIGH,
            },
    };
    HAL_DMA_Init(&self->Handler);

    if (self->Interrupt) {
        HAL_NVIC_SetPriority(DMAx_Streamy_IRQn(self->DMAx, self->Stream), 8, 0);
        HAL_NVIC_EnableIRQ(DMAx_Streamy_IRQn(self->DMAx, self->Stream));
    }
}