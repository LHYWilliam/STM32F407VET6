#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Serial.h"

void Serial_Init(Serial_t *Self) {
    Self->Handler = (UART_HandleTypeDef){
        .Instance = Self->USART,
        .Init =
            {
                .BaudRate = Self->Baudrate,
                .WordLength = UART_WORDLENGTH_8B,
                .Parity = UART_PARITY_NONE,
                .StopBits = UART_STOPBITS_1,
                .Mode = *Self->RX && *Self->TX ? UART_MODE_TX_RX
                                               : (*Self->RX   ? UART_MODE_RX
                                                  : *Self->TX ? UART_MODE_TX
                                                              : 0),
            },
    };
    HAL_UART_Init(&Self->Handler);

    if (Self->RxIT) {
        Serial_RXITStart(Self, Self->RxITSize);
    }
}

void Serial_RXITStart(Serial_t *Self, uint8_t Size) {
    HAL_UART_Receive_IT(&Self->Handler, Self->RXBuffer, Size);
}

void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint8_t Length) {
    HAL_UART_Transmit(&Self->Handler, Bytes, Length, HAL_MAX_DELAY);
}

void Serial_Printf(Serial_t *Self, char *Format, ...) {
    va_list arg;
    va_start(arg, Format);
    vsprintf((char *)Self->TXBuffer, Format, arg);
    va_end(arg);

    HAL_UART_Transmit(&Self->Handler, Self->TXBuffer,
                      strlen((char *)Self->TXBuffer), HAL_MAX_DELAY);
}