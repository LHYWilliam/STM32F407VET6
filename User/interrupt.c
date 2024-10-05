#include "Serial.h"
#include "Timer.h"

#include "lvgl.h"

extern Serial_Handler serial;

extern Timer_Handler LVGLTimer;

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
    }

    Serial_RXITStart(&serial, 1);
}

void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&LVGLTimer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &LVGLTimer.Handler) {
        lv_timer_handler();
    }
}