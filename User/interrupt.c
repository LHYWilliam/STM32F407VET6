#include "main.h"

void USART1_IRQHandler() { HAL_UART_IRQHandler(&SerialBoard.Handler); }

void USART2_IRQHandler() { HAL_UART_IRQHandler(&SerialBluetooth.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBoard.USART) {
        // Serial_SendBytes(&SerialBoard, SerialBoard.RXBuffer, 1);
        // Serial_RXITStart(&SerialBoard, 1);
        // return;

        if (SerialBoard.PackRecieved == SET) {
            Serial_RXITStart(&SerialBoard, 1);
            return;
        }

        Serial_Parse(&SerialBoard, SerialBoard.RXBuffer[0]);

        Serial_RXITStart(&SerialBoard, 1);

    } else if (huart->Instance == SerialBluetooth.USART) {
        // Serial_SendBytes(&SerialBluetooth, SerialBluetooth.RXBuffer, 1);
        // Serial_RXITStart(&SerialBluetooth, 1);
        // return;

        if (SerialBluetooth.PackRecieved == SET) {
            Serial_RXITStart(&SerialBluetooth, 1);
            return;
        }

        Serial_Parse(&SerialBluetooth, SerialBluetooth.RXBuffer[0]);

        Serial_RXITStart(&SerialBluetooth, 1);
    }
}

// void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim == &Timer.Handler) {
    //     // LED_Toggle(&LED1);
    // }
}
