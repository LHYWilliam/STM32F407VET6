#include "RTE_Components.h"
#include CMSIS_device_header

#include "PWM.h"
#include "Serial.h"
#include "TIM.h"
#include "Timer.h"

extern Serial_t Serial;
extern Timer_t Timer;
extern PWM_t PWM;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial.USART);

        GPIO_t RTX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        if (*Serial.RX) {
            GPIO_InitPin(&RTX, Serial.RX);
        }
        if (*Serial.TX) {
            GPIO_InitPin(&RTX, Serial.TX);
        }

        if (Serial.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial.USART), Serial.Priority, 0);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Timer.TIMx);

        if (Timer.Interrupt) {
            HAL_NVIC_SetPriority(TIMx_IRQN(Timer.TIMx), Timer.Priority, 0);
            HAL_NVIC_EnableIRQ(TIMx_IRQN(Timer.TIMx));
        }
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == PWM.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(PWM.TIM);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(PWM.TIM),
        };

        for (uint8_t i = 0; PWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, PWM.GPIOxPiny[i]);
        }
    }
}