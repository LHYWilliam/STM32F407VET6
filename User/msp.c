#include <string.h>

#include "GPIO.h"
#include "Serial.h"
#include "Signal.h"

extern Serial_Handler serial;

extern SignalGenerator_Handler generator;
extern SignalSampler_Handler sampler;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(serial.USART);

        GPIO_Handler RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(RX.GPIOxPiny, serial.RX);
        GPIO_Init(&RX);

        GPIO_Handler TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(TX.GPIOxPiny, serial.TX);
        GPIO_Init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(serial.USART));
        HAL_NVIC_SetPriority(USARTx_IRQn(serial.USART), 4, 0);
    }
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == generator.DAC.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, generator.DAC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (generator.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(generator.DMA.DMAx);

            __HAL_LINKDMA(&generator.DAC.Handler, DMA_Handle1,
                          generator.DMA.Handler);
        }
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(sampler.ADC.ADCx);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, sampler.ADC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (sampler.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(sampler.DMA.DMAx);

            __HAL_LINKDMA(&sampler.ADC.Handler, DMA_Handle,
                          sampler.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == generator.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(generator.Timer.TIMx);

    } else if (htim->Instance == sampler.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(sampler.Timer.TIMx);
    }
}
