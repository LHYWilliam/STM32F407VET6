#ifndef SERIAL_H
#define SERIAL_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

#define __HAL_RCC_USARTx_CLK_ENABLE(x)                                         \
    do {                                                                       \
        if ((x) == USART1) {                                                   \
            __HAL_RCC_USART1_CLK_ENABLE();                                     \
        } else if ((x) == USART2) {                                            \
            __HAL_RCC_USART2_CLK_ENABLE();                                     \
        } else if ((x) == USART3) {                                            \
            __HAL_RCC_USART3_CLK_ENABLE();                                     \
        } else if ((x) == UART4) {                                             \
            __HAL_RCC_UART4_CLK_ENABLE();                                      \
        }                                                                      \
    } while (0)

#define GPIO_AF7_USARTx(x)                                                     \
    ((x) == USART1   ? GPIO_AF7_USART1                                         \
     : (x) == USART2 ? GPIO_AF7_USART2                                         \
     : (x) == USART3 ? GPIO_AF7_USART3                                         \
     : (x) == UART4  ? GPIO_AF8_UART4                                          \
                     : NULL)

#define USARTx_IRQn(x)                                                         \
    ((x) == USART1   ? USART1_IRQn                                             \
     : (x) == USART2 ? USART2_IRQn                                             \
     : (x) == USART3 ? USART3_IRQn                                             \
     : (x) == UART4  ? UART4_IRQn                                              \
                     : NULL)

#define TXBUFFER_SIZE 128
#define RXBUFFER_SIZE 128

typedef enum {
    Serial_None,
    Serial_Byte,
    Serial_HexPack,
    Serial_StringPack,
} Serial_PackType;

typedef struct {
    USART_TypeDef *USART;
    GPIOxPiny_t RX;
    GPIOxPiny_t TX;

    uint32_t Baudrate;

    uint8_t RxIT;
    uint8_t RxITSize;
    uint8_t Priority;

    uint8_t TXBuffer[TXBUFFER_SIZE];
    uint8_t RXBuffer[RXBUFFER_SIZE];

    uint8_t RecieveByteCount;
    uint8_t RecieveFlag;
    Serial_PackType PackType;
    uint8_t ByteData;
    uint8_t HexData[32];

    uint8_t Default;

    UART_HandleTypeDef Handler;
} Serial_t;

void Serial_Init(Serial_t *Self);
void Serial_RXITStart(Serial_t *Self, uint8_t Size);
void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint8_t Length);
void Serial_Printf(Serial_t *Self, char *Format, ...);
void Serial_Clear(Serial_t *Self);

#endif