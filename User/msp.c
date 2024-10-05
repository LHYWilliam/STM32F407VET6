#include <string.h>

#include "ADC.h"
#include "DAC.h"
#include "GPIO.h"
#include "Serial.h"

extern Serial_Handler serial;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(serial.USART);

        GPIO_Handler RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(RX.GPIOxPiny, serial.RX);
        GPIO_Init(&RX);

        GPIO_Handler TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(TX.GPIOxPiny, serial.TX);
        GPIO_Init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(serial.USART));
        HAL_NVIC_SetPriority(USARTx_IRQn(serial.USART), 4, 0);
    }
}
