#include "RTE_Components.h"
#include CMSIS_device_header

#include "LED.h"
#include "Serial.h"
#include "Timer.h"


extern Serial_t Serial1;
extern Serial_t Serial2;
extern Timer_t Timer;
extern LED_t LED1;

void USART2_IRQHandler(void) { HAL_UART_IRQHandler(&Serial2.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial2.USART) {
        Serial_SendBytes(&Serial1, Serial2.RXBuffer, Serial2.RxITSize);

        Serial_RXITStart(&Serial2, 1);
    }
}

void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &Timer.Handler) {
        // LED_Toggle(&LED1);
    }
}
