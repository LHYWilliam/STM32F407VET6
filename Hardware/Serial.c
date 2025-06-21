#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Serial.h"

void Serial_Init(Serial_t *self) {
    self->Handler = (UART_HandleTypeDef){
        .Instance = self->USART,
        .Init =
            {
                .BaudRate = self->Baudrate,
                .WordLength = UART_WORDLENGTH_8B,
                .Parity = UART_PARITY_NONE,
                .StopBits = UART_STOPBITS_1,
                .Mode = UART_MODE_TX_RX,
            },
    };
    HAL_UART_Init(&self->Handler);

    if (self->RxIT) {
        Serial_RXITStart(self, 1);
    }
}

void Serial_RXITStart(Serial_t *self, uint8_t size) {
    HAL_UART_Receive_IT(&self->Handler, self->RXBuffer, size);
}

void Serial_SendBytes(Serial_t *self, uint8_t *bytes, uint8_t length) {
    HAL_UART_Transmit(&self->Handler, bytes, length, HAL_MAX_DELAY);
}

void Serial_Printf(Serial_t *self, char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->TXBuffer, format, arg);
    va_end(arg);

    HAL_UART_Transmit(&self->Handler, (uint8_t *)self->TXBuffer,
                      strlen((char *)self->TXBuffer), HAL_MAX_DELAY);
}