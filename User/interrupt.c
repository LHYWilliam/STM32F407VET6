#include "main.h"

void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&SerialBoard.Handler);
    Serial_RXITStart(&SerialBoard);
}

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&SerialBluetooth.Handler);
    Serial_RXITStart(&SerialBluetooth);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBoard.USART) {
        // Serial_SendBytes(&SerialBoard, SerialBoard.RXBuffer, 1);
        // Serial_RXITStart(&SerialBoard, 1);
        // return;

        if (SerialBoard.PackRecieved == SET) {
            return;
        }

        Serial_Parse(&SerialBoard, SerialBoard.RXBuffer[0]);

    } else if (huart->Instance == SerialBluetooth.USART) {
        // Serial_SendBytes(&SerialBluetooth, SerialBluetooth.RXBuffer, 1);
        // Serial_RXITStart(&SerialBluetooth, 1);
        // return;

        if (SerialBluetooth.PackRecieved == SET) {
            return;
        }

        Serial_Parse(&SerialBluetooth, SerialBluetooth.RXBuffer[0]);
    }
}

// void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim == &Timer.Handler) {
    //     // LED_Toggle(&LED1);
    // }
}
