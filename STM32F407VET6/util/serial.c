#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"

void Serial_init(Serial *serial) {
    serial->Handler = (UART_HandleTypeDef){
        .Instance = serial->usart,
        .Init =
            {
                .BaudRate = serial->baudrate,
                .WordLength = UART_WORDLENGTH_8B,
                .Parity = UART_PARITY_NONE,
                .StopBits = UART_STOPBITS_1,
                .Mode = UART_MODE_TX_RX,
            },
    };
    HAL_UART_Init(&serial->Handler);
}

void Serial_RXITStart(Serial *serial, uint8_t size) {
    HAL_UART_Receive_IT(&serial->Handler, serial->RXBuffer, size);
}

void Serial_sendBytes(Serial *serial, uint8_t *bytes, uint8_t length) {
    HAL_UART_Transmit(&serial->Handler, bytes, length, 0xFFFF);
}

void Serial_printf(Serial *serial, char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)serial->TXBuffer, format, arg);
    va_end(arg);

    HAL_UART_Transmit(&serial->Handler, (uint8_t *)serial->TXBuffer,
                      strlen((char *)serial->TXBuffer), 0xFFFF);
}