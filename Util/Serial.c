#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Serial.h"

void Serial_Init(Serial *serial) {
    serial->Handler = (UART_HandleTypeDef){
        .Instance = serial->USART,
        .Init =
            {
                .BaudRate = serial->Baudrate,
                .WordLength = UART_WORDLENGTH_8B,
                .Parity = UART_PARITY_NONE,
                .StopBits = UART_STOPBITS_1,
                .Mode = UART_MODE_TX_RX,
            },
    };
    HAL_UART_Init(&serial->Handler);

    if (serial->RxIT) {
        Serial_RXITStart(serial, 1);
    }
}

void Serial_RXITStart(Serial *serial, uint8_t size) {
    HAL_UART_Receive_IT(&serial->Handler, serial->RXBuffer, size);
}

void Serial_SendBytes(Serial *serial, uint8_t *bytes, uint8_t length) {
    HAL_UART_Transmit(&serial->Handler, bytes, length, HAL_MAX_DELAY);
}

void Serial_Printf(Serial *serial, char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)serial->TXBuffer, format, arg);
    va_end(arg);

    HAL_UART_Transmit(&serial->Handler, (uint8_t *)serial->TXBuffer,
                      strlen((char *)serial->TXBuffer), HAL_MAX_DELAY);
}