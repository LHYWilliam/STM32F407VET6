#ifndef SERIAL_H
#define SERIAL_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "DMA.h"
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

#define GPIO_AFx_USARTy(x)                                                     \
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

#define BUFFER_SIZE 64

typedef enum {
    SerialPack_Int8,
    SerialPack_Int16,
    SerialPack_Int32,
    SerialPack_Uint8,
    SerialPack_Uint16,
    SerialPack_Uint32,
    SerialPack_Float32,
    SerialPack_String,
} Serial_PackType;

typedef struct {
    USART_TypeDef *USARTx;
    GPIOxPiny_t RX;
    GPIOxPiny_t TX;
    uint32_t Baudrate;

    FunctionalState Interrupt;
    FunctionalState IdleDMA;
    uint8_t RxITSize;
    uint8_t Priority;

    DMA_t DMA;

    Serial_PackType PackType;

    FunctionalState Default;

    uint8_t ParsedCount;
    FlagStatus PackRecieved;

    uint8_t TxBuffer[BUFFER_SIZE];
    uint8_t RxBuffer[BUFFER_SIZE];
    uint8_t Pack[BUFFER_SIZE];

    UART_HandleTypeDef Handler;
} Serial_t;

void Serial_Init(Serial_t *Self);
void Serial_RXITStart(Serial_t *Self);
void Serial_IdleDMAStart(Serial_t *Self);
void Serial_SendBytes(Serial_t *Self, uint8_t *Bytes, uint8_t Length);
void Serial_Printf(Serial_t *Self, char *Format, ...);
void Serial_Parse(Serial_t *Self, uint8_t RxData);
void Serial_Clear(Serial_t *Self);

#endif