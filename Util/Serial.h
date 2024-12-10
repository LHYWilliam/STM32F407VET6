#ifndef SERIAL_H
#define SERIAL_H

#include CMSIS_device_header

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
    USART_TypeDef *USART;

    char RX[4];
    char TX[4];

    uint32_t Baudrate;

    uint8_t RxIT;
    uint8_t RxITSize;

    uint8_t TXBuffer[TXBUFFER_SIZE];
    uint8_t RXBuffer[RXBUFFER_SIZE];

    UART_HandleTypeDef Handler;
} Serial_t;

void Serial_Init(Serial_t *self);
void Serial_RXITStart(Serial_t *self, uint8_t size);

void Serial_SendBytes(Serial_t *self, uint8_t *bytes, uint8_t length);
void Serial_Printf(Serial_t *self, char *format, ...);

#endif