#include "RTE_Components.h"
#include CMSIS_device_header

#include "LED.h"
#include "Serial.h"
#include "Timer.h"

extern Serial_t Serial1;
extern Serial_t Serial2;
// extern Timer_t Timer;
extern LED_t LED1;

void USART2_IRQHandler(void) { HAL_UART_IRQHandler(&Serial2.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial2.USART) {
        Serial2.ByteData = Serial2.RXBuffer[0];

        if (Serial2.RecieveFlag == SET) {
            Serial_RXITStart(&Serial2, 1);
            return;
        }

        switch (Serial2.PackType) {
        case Serial_None:
            if (Serial2.ByteData == 0xFF) {
                Serial2.PackType = Serial_HexPack;
            } else {
                Serial_Clear(&Serial2);
            }
            break;

        case Serial_HexPack:
            if (Serial2.ByteData == 0xFE && Serial2.RecieveByteCount == 4) {
                Serial2.RecieveFlag = SET;
            } else {
                Serial2.HexData[Serial2.RecieveByteCount++] = Serial2.ByteData;
            }

            if (Serial2.RecieveByteCount > 4) {
                Serial_Clear(&Serial2);
            }
            break;

        default:
            Serial_Clear(&Serial2);
            break;
        }

        Serial_RXITStart(&Serial2, 1);
    }
}

// void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim == &Timer.Handler) {
    //     // LED_Toggle(&LED1);
    // }
}
