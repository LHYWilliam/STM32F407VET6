#include "RTE_Components.h"
#include CMSIS_device_header

#include "Encoder.h"
#include "PWM.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"
#include "TIM.h"
#include "Timer.h"

extern Serial_t Serial1;
extern Serial_t Serial2;
extern Timer_t Timer;
extern PWM_t PWM;
extern Servo_t Servo;
extern Encoder_t Encoder;
extern Sampler_t Sampler;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial1.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial1.USART);

        GPIO_t RTX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial1.USART),
        };
        if (*Serial1.RX) {
            GPIO_InitPin(&RTX, Serial1.RX);
        }
        if (*Serial1.TX) {
            GPIO_InitPin(&RTX, Serial1.TX);
        }

        if (Serial1.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial1.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial1.USART), Serial1.Priority, 0);
        }
    } else if (huart->Instance == Serial2.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial2.USART);

        GPIO_t RTX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial2.USART),
        };
        if (*Serial2.RX) {
            GPIO_InitPin(&RTX, Serial2.RX);
        }
        if (*Serial2.TX) {
            GPIO_InitPin(&RTX, Serial2.TX);
        }

        if (Serial2.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial2.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial2.USART), Serial2.Priority, 0);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Timer.TIMx);

        // if (Timer.Interrupt) {
        //     HAL_NVIC_SetPriority(TIMx_IRQN(Timer.TIMx), Timer.Priority, 0);
        //     HAL_NVIC_EnableIRQ(TIMx_IRQN(Timer.TIMx));
        // } else
        if (htim->Instance == Sampler.Timer.TIMx) {
            __HAL_RCC_TIMx_CLK_ENABLE(Sampler.Timer.TIMx);
        }
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    // if (htim->Instance == PWM.TIM) {
    //     __HAL_RCC_TIMx_CLK_ENABLE(PWM.TIM);

    //     GPIO_t GPIO = {
    //         .Mode = GPIO_MODE_AF_PP,
    //         .Pull = GPIO_NOPULL,
    //         .Alternate = GPIO_AFx_TIMy(PWM.TIM),
    //     };

    //     for (uint8_t i = 0; PWM.Channel[i]; i++) {
    //         GPIO_InitPin(&GPIO, PWM.GPIOxPiny[i]);
    //     }
    // } else
    if (htim->Instance == Servo.PWM.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(Servo.PWM.TIM);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(Servo.PWM.TIM),
        };

        for (uint8_t i = 0; Servo.PWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Servo.PWM.GPIOxPiny[i]);
        }
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Encoder.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(Encoder.TIM),
        };
        GPIO_InitPin(&GPIO, Encoder.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, Encoder.GPIOxPiny[1]);
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == Sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(Sampler.ADC.ADCx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        for (uint8_t i = 0; Sampler.ADC.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Sampler.ADC.GPIOxPiny[i]);
        }

        if (Sampler.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Sampler.DMA.DMAx);

            __HAL_LINKDMA(&Sampler.ADC.Handler, DMA_Handle,
                          Sampler.DMA.Handler);
        }
    }
}