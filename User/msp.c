#include "RTE_Components.h"
#include CMSIS_device_header

#include "Encoder.h"
#include "ICM42688.h"
#include "PWM.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"
#include "TIM.h"
#include "Timer.h"

extern Serial_t Serial1;
extern Serial_t Serial2;
extern Serial_t Serial3;

extern Encoder_t Encoder1;
extern Encoder_t Encoder2;

// extern Timer_t Timer;
extern PWM_t ServoPWM;

extern Sampler_t Sampler;
extern ICM42688_t ICM42688;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial1.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial1.USART);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AF7_USARTx(Serial1.USART),
        };
        if (*Serial1.TX) {
            GPIO_InitPin(&GPIO, Serial1.TX);
        }
        if (*Serial1.RX) {
            GPIO_InitPin(&GPIO, Serial1.RX);
        }

        if (Serial1.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial1.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial1.USART), Serial1.Priority,
                                 0);
        }
    } else if (huart->Instance == Serial2.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial2.USART);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AF7_USARTx(Serial2.USART),
        };
        if (*Serial2.TX) {
            GPIO_InitPin(&GPIO, Serial2.TX);
        }
        if (*Serial2.RX) {
            GPIO_InitPin(&GPIO, Serial2.RX);
        }

        if (Serial2.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial2.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial2.USART), Serial2.Priority,
                                 0);
        }
    } else if (huart->Instance == Serial3.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial3.USART);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AF7_USARTx(Serial3.USART),
        };
        if (*Serial3.TX) {
            GPIO_InitPin(&GPIO, Serial3.TX);
        }
        if (*Serial3.RX) {
            GPIO_InitPin(&GPIO, Serial3.RX);
        }

        if (Serial3.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial3.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial3.USART), Serial3.Priority,
                                 0);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    // if (htim->Instance == Timer.TIMx) {
    //     __HAL_RCC_TIMx_CLK_ENABLE(Timer.TIMx);

    //     if (Timer.Interrupt) {
    //         HAL_NVIC_SetPriority(TIMx_IRQN(Timer.TIMx), Timer.Priority, 0);
    //         HAL_NVIC_EnableIRQ(TIMx_IRQN(Timer.TIMx));
    //     }
    // } else
    if (htim->Instance == Sampler.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Sampler.Timer.TIMx);
    } else if (htim->Instance == ServoPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(ServoPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(ServoPWM.TIMx),
        };

        for (uint8_t i = 0; ServoPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, ServoPWM.GPIOxPiny[i]);
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
    if (htim->Instance == ServoPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(ServoPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(ServoPWM.TIMx),
        };

        for (uint8_t i = 0; ServoPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, ServoPWM.GPIOxPiny[i]);
        }
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Encoder1.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(Encoder1.TIM),
        };
        GPIO_InitPin(&GPIO, Encoder1.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, Encoder1.GPIOxPiny[1]);

    } else if (htim->Instance == Encoder2.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(Encoder2.TIM),
        };
        GPIO_InitPin(&GPIO, Encoder2.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, Encoder2.GPIOxPiny[1]);
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

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == ICM42688.SPIx) {
        __HAL_RCC_SPIx_CLK_ENABLE(ICM42688.SPIx);

        GPIO_t GPIO;

        GPIO.Mode = GPIO_MODE_AF_PP;
        GPIO.Alternate = GPIO_AF5_SPI2;
        GPIO.Pull = GPIO_PULLUP;
        ICM42688.SCLK_ODR = GPIO_InitPin(&GPIO, ICM42688.SCLK);
        GPIO_InitPin(&GPIO, ICM42688.MISO);
        ICM42688.MOSI_ODR = GPIO_InitPin(&GPIO, ICM42688.MOSI);

        GPIO.Mode = GPIO_MODE_OUTPUT_PP;
        ICM42688.CS_ODR = GPIO_InitPin(&GPIO, ICM42688.CS);

        GPIO_Write(ICM42688.SCLK_ODR, 1);
        GPIO_Write(ICM42688.MOSI_ODR, 1);
        GPIO_Write(ICM42688.CS_ODR, 1);
    }
}