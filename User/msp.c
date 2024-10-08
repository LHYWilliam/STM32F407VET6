#include <string.h>

#include "GPIO.h"
#include "Serial.h"
#include "Signal.h"

extern Serial_t Serial;

extern SignalGenerator_Handler Generator;
extern SignalSampler_t Sampler;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial.USART);

        GPIO_t RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        strcpy(RX.GPIOxPiny, Serial.RX);
        GPIO_Init(&RX);

        GPIO_t TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        strcpy(TX.GPIOxPiny, Serial.TX);
        GPIO_Init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial.USART));
        HAL_NVIC_SetPriority(USARTx_IRQn(Serial.USART), 4, 0);
    }
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    __HAL_RCC_FSMC_CLK_ENABLE();

    GPIO_t FSMC_D = {
        .GPIOxPiny = "D14 | D15 | D0 | D1 | E7 | E8 | E9 | E10 | E11 | E12 | "
                     "E13 | E14 | E15 | D8 | D9 | D10",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_D);

    GPIO_t FSMC_NOE = {
        .GPIOxPiny = "D4 ",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NOE);

    GPIO_t FSMC_NWE = {
        .GPIOxPiny = "D5",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NWE);

    GPIO_t FSMC_NE1 = {
        .GPIOxPiny = "D7",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NE1);

    GPIO_t FSMC_A18 = {
        .GPIOxPiny = "D13",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_A18);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == Generator.DAC.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_t gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, Generator.DAC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (Generator.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Generator.DMA.DMAx);

            __HAL_LINKDMA(&Generator.DAC.Handler, DMA_Handle1,
                          Generator.DMA.Handler);
        }
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == Sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(Sampler.ADC.ADCx);

        GPIO_t gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, Sampler.ADC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (Sampler.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Sampler.DMA.DMAx);

            __HAL_LINKDMA(&Sampler.ADC.Handler, DMA_Handle,
                          Sampler.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Generator.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Generator.Timer.TIMx);

    } else if (htim->Instance == Sampler.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Sampler.Timer.TIMx);
    }
}
