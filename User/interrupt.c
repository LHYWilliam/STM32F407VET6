#include "main.h"

void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&SerialBoard.Handler);

    if (SerialBoard.RxITSize) {
        Serial_RXITStart(&SerialBoard);

    } else if (SerialBoard.IdleDMA) {
        Serial_IdleDMAStart(&SerialBoard);
    }
}

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&SerialBluetooth.Handler);

    if (SerialBluetooth.RxITSize) {
        Serial_RXITStart(&SerialBluetooth);

    } else if (SerialBluetooth.IdleDMA) {
        Serial_IdleDMAStart(&SerialBluetooth);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBoard.USARTx) {
        if (SerialBoard.PackRecieved == SET) {
            return;
        }

        Serial_Parse(&SerialBoard, SerialBoard.RxBuffer[0]);

    } else if (huart->Instance == SerialBluetooth.USARTx) {
        if (SerialBluetooth.PackRecieved == SET) {
            return;
        }

        Serial_Parse(&SerialBluetooth, SerialBluetooth.RxBuffer[0]);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == SerialBoard.USARTx) {
        if (SerialBoard.PackRecieved == SET) {
            return;
        }
        for (uint8_t i = 0; i < Size; i++) {
            Serial_Parse(&SerialBoard, SerialBoard.RxBuffer[i]);
            if (SerialBoard.PackRecieved == SET) {
                return;
            }
        }

    } else if (huart->Instance == SerialBluetooth.USARTx) {
        if (SerialBluetooth.PackRecieved == SET) {
            return;
        }
        for (uint8_t i = 0; i < Size; i++) {
            Serial_Parse(&SerialBluetooth, SerialBluetooth.RxBuffer[i]);
            if (SerialBluetooth.PackRecieved == SET) {
                return;
            }
        }
    }
}

// void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim == &Timer.Handler) {
    //     // LED_Toggle(&LED1);
    // }
}
