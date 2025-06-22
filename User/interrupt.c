#include "RTE_Components.h"
#include CMSIS_device_header

#include "LED.h"
#include "Serial.h"
#include "Timer.h"


extern Serial_t Serial;
extern Timer_t Timer;
extern LED_t LED1;

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&Serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        Serial_SendBytes(&Serial, Serial.RXBuffer, Serial.RxITSize);

        Serial_RXITStart(&Serial, 1);
    }
}

void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &Timer.Handler) {
        // LED_Toggle(&LED1);
    }
}
