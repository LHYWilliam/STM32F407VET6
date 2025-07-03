#include "RTE_Components.h"
#include CMSIS_device_header

#include "LED.h"
#include "Serial.h"
#include "Timer.h"

extern Serial_t SerialBoard;
extern Serial_t SerialBluetooth;
// extern Timer_t Timer;
extern LED_t LED1;

void USART2_IRQHandler(void) { HAL_UART_IRQHandler(&SerialBluetooth.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBluetooth.USART) {
        SerialBluetooth.ByteData = SerialBluetooth.RXBuffer[0];

        if (SerialBluetooth.RecieveFlag == SET) {
            Serial_RXITStart(&SerialBluetooth, 1);
            return;
        }

        switch (SerialBluetooth.PackType) {
        case Serial_None:
            if (SerialBluetooth.ByteData == 0xFF) {
                SerialBluetooth.PackType = Serial_HexPack;
            } else {
                Serial_Clear(&SerialBluetooth);
            }
            break;

        case Serial_HexPack:
            if (SerialBluetooth.ByteData == 0xFE &&
                SerialBluetooth.RecieveByteCount == 4) {
                SerialBluetooth.RecieveFlag = SET;
            } else {
                SerialBluetooth.HexData[SerialBluetooth.RecieveByteCount++] =
                    SerialBluetooth.ByteData;
            }

            if (SerialBluetooth.RecieveByteCount > 4) {
                Serial_Clear(&SerialBluetooth);
            }
            break;

        default:
            Serial_Clear(&SerialBluetooth);
            break;
        }

        Serial_RXITStart(&SerialBluetooth, 1);
    }
}

// void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim == &Timer.Handler) {
    //     // LED_Toggle(&LED1);
    // }
}
