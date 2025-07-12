#include "DMA.h"

void DMA_Init(DMA_t *Self) {
    Self->Handler = (DMA_HandleTypeDef){
        .Instance = DMAx_Streamy(Self->DMAx, Self->Stream),
        .Init =
            {
                .Channel = DMA_Channelx(Self->Channel),

                .PeriphInc =
                    Self->PeriphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE,
                .PeriphDataAlignment = DMA_PDATAALIGN_SIZE(Self->PeriphSize),

                .MemInc = Self->MemInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE,
                .MemDataAlignment = DMA_MDATAALIGN_SIZE(Self->MemSize),

                .Mode = Self->Mode,
                .Direction = Self->Direction,
                .Priority = DMA_PRIORITY_HIGH,
            },
    };

    __HAL_RCC_DMAx_CLK_ENABLE(Self->DMAx);
    HAL_DMA_Init(&Self->Handler);

    if (Self->Interrupt) {
        HAL_NVIC_SetPriority(DMAx_Streamy_IRQn(Self->DMAx, Self->Stream), 8, 0);
        HAL_NVIC_EnableIRQ(DMAx_Streamy_IRQn(Self->DMAx, Self->Stream));
    }
}