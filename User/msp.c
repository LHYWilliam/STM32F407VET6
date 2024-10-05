#include <string.h>

#include "ADC.h"
#include "DAC.h"
#include "GPIO.h"
#include "Serial.h"
#include "Timer.h"

extern Serial_Handler serial;

extern DAC_Handler dac;
extern ADC_Handler adc;

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
    if (hdac->Instance == dac.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, dac.GPIOxPiny);
        GPIO_Init(&gpio);

        if (dac.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(dac.DMA.DMAx);

            __HAL_LINKDMA(&dac.Handler, DMA_Handle1, dac.DMA.Handler);
        }
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == adc.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(adc.ADCx);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, adc.GPIOxPiny);
        GPIO_Init(&gpio);

        if (adc.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(adc.DMA.DMAx);

            __HAL_LINKDMA(&adc.Handler, DMA_Handle, adc.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == dac.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(dac.Timer.TIMx);

    } else if (htim->Instance == adc.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(adc.Timer.TIMx);
    }
}