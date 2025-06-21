#include "RTE_Components.h"
#include CMSIS_device_header

#include "Serial.h"

extern Serial_t Serial;

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&Serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        Serial_SendBytes(&Serial, Serial.RXBuffer, Serial.RxITSize);

        Serial_RXITStart(&Serial, 1);
    }
}
