#include "Serial.h"

extern Serial_Handler serial;

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
    }

    Serial_RXITStart(&serial, 1);
}