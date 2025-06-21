#include "RTE_Components.h"
#include CMSIS_device_header

#include "Serial.h"

extern Serial_t Serial;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial.USART);

        GPIO_t RTX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        if (*Serial.RX) {
            GPIO_InitPin(&RTX, Serial.RX);
        }
        if (*Serial.TX) {
            GPIO_InitPin(&RTX, Serial.TX);
        }

        if (Serial.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial.USART), Serial.Priority, 0);
        }
    }
}