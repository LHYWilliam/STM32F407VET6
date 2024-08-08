#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f4xx.h"

#define __HAL_RCC_USARTx_CLK_ENABLE(x)                                         \
    do {                                                                       \
        if ((x) == USART1) {                                                   \
            __HAL_RCC_USART1_CLK_ENABLE();                                     \
        } else if ((x) == USART2) {                                            \
            __HAL_RCC_USART2_CLK_ENABLE();                                     \
        } else if ((x) == USART3) {                                            \
            __HAL_RCC_USART3_CLK_ENABLE();                                     \
        }                                                                      \
    } while (0)

#define GPIO_AF7_USARTx(x)                                                     \
    ((x) == USART1   ? GPIO_AF7_USART1                                         \
     : (x) == USART2 ? GPIO_AF7_USART2                                         \
     : (x) == USART3 ? GPIO_AF7_USART3                                         \
                     : NULL)

#define USARTx_IRQn(x)                                                         \
    ((x) == USART1   ? USART1_IRQn                                             \
     : (x) == USART2 ? USART2_IRQn                                             \
     : (x) == USART3 ? USART3_IRQn                                             \
                     : NULL)

#define TXBUFFER_SIZE 128
#define RXBUFFER_SIZE 128

typedef struct {
    USART_TypeDef *usart;

    char RX[4];
    char TX[4];

    uint32_t baudrate;

    uint8_t TXBuffer[TXBUFFER_SIZE];
    uint8_t RXBuffer[RXBUFFER_SIZE];

    UART_HandleTypeDef Handler;
} Serial;

void Serial_init(Serial *serial);
void Serial_RXITStart(Serial *serial, uint8_t size);

void Serial_sendBytes(Serial *serial, uint8_t *bytes, uint8_t length);
void Serial_printf(Serial *serial, char *format, ...);

#endif