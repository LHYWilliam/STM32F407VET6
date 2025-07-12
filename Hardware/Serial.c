#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Serial.h"

Serial_t *DefaultSerial;

void Serial_Init(Serial_t *Self) {
    GPIO_t GPIO = {
        .Mode = GPIO_MODE_AF_PP,
        .Alternate = GPIO_AFx_USARTy(Self->USARTx),
    };
    if (*Self->TX) {
        GPIO_InitPin(&GPIO, Self->TX);
    }
    if (*Self->RX) {
        GPIO_InitPin(&GPIO, Self->RX);
    }

    Self->Handler = (UART_HandleTypeDef){
        .Instance = Self->USARTx,
        .Init =
            {
                .BaudRate = Self->Baudrate,
                .Mode = *Self->RX && *Self->TX ? UART_MODE_TX_RX
                                               : (*Self->RX   ? UART_MODE_RX
                                                  : *Self->TX ? UART_MODE_TX
                                                              : 0),
            },
    };

    if (Self->DMA.DMAx) {
        Self->DMA.PeriphSize = 8;
        Self->DMA.PeriphInc = DISABLE;
        Self->DMA.MemSize = 8;
        Self->DMA.MemInc = ENABLE;
        Self->DMA.Mode = DMA_NORMAL;
        Self->DMA.Direction = DMA_PERIPH_TO_MEMORY;

        DMA_Init(&Self->DMA);

        __HAL_LINKDMA(&Self->Handler, hdmarx, Self->DMA.Handler);
    }

    if (Self->Interrupt) {
        HAL_NVIC_SetPriority(USARTx_IRQn(Self->USARTx), Self->Priority, 0);
        HAL_NVIC_EnableIRQ(USARTx_IRQn(Self->USARTx));
    }

    __HAL_RCC_USARTx_CLK_ENABLE(Self->USARTx);
    HAL_UART_Init(&Self->Handler);

    if (Self->RxITSize) {
        Serial_RXITStart(Self);

    } else if (Self->IdleDMA) {
        Serial_IdleDMAStart(Self);
    }

    if (Self->Default) {
        DefaultSerial = Self;
    }
}

void Serial_RXITStart(Serial_t *Self) {
    HAL_UART_Receive_IT(&Self->Handler, Self->RxBuffer, Self->RxITSize);
}

void Serial_IdleDMAStart(Serial_t *Self) {
    HAL_UARTEx_ReceiveToIdle_DMA(&Self->Handler, Self->RxBuffer, BUFFER_SIZE);
}

void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint8_t Length) {
    HAL_UART_Transmit(&Self->Handler, Bytes, Length, HAL_MAX_DELAY);
}

void Serial_Printf(Serial_t *Self, char *Format, ...) {
    va_list arg;
    va_start(arg, Format);
    vsprintf((char *)Self->TxBuffer, Format, arg);
    va_end(arg);

    HAL_UART_Transmit(&Self->Handler, Self->TxBuffer,
                      strlen((char *)Self->TxBuffer), HAL_MAX_DELAY);
}

void Serial_Parse(Serial_t *Self, uint8_t RxData) {
    if (Self->FoundPackHead == RESET) {
        if (RxData == 0xFE) {
            Self->FoundPackHead = SET;
            Self->PackType = Serial_HexPack;

        } else if (RxData == '>') {
            Self->FoundPackHead = SET;
            Self->PackType = Serial_StringPack;

        } else {
            Serial_Clear(Self);
        }

        return;
    }

    switch (Self->PackType) {
    case Serial_HexPack:
        if (RxData == 0xEF && Self->ParsedCount == Self->PackLength) {
            Self->PackRecieved = SET;

        } else {
            Self->HexPack[Self->ParsedCount++] = RxData;
        }

        if (Self->ParsedCount > Self->PackLength) {
            Serial_Clear(Self);
        }
        break;

    case Serial_StringPack:
        if (RxData == '\n' && Self->StringPack[Self->ParsedCount - 1] == '\r') {
            Self->PackRecieved = SET;
            Self->StringPack[Self->ParsedCount - 1] = '\0';

        } else {
            Self->StringPack[Self->ParsedCount++] = RxData;
        }

        if (Self->ParsedCount > BUFFER_SIZE) {
            Serial_Clear(Self);
        }
        break;
    }
}

void Serial_Clear(Serial_t *Self) {
    Self->ParsedCount = 0;
    Self->PackRecieved = RESET;
    Self->FoundPackHead = RESET;
}

int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&DefaultSerial->Handler, (uint8_t *)&ch, 1,
                      HAL_MAX_DELAY);

    return ch;
}
