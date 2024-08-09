#include "stm32f4xx_hal.h"

#include <string.h>

#include "OLED.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "encoder.h"
#include "gpio.h"
#include "pwm.h"
#include "serial.h"
#include "tim.h"
#include "timer.h"

OLED oled = {
    .SCL = "C2",
    .SDA = "C3",
};

PWM pwm = {
    .TIM = TIM5,
    .channel = "2",
    .Prescaler = 8400 - 1,
    .Period = 100 - 1,
    .GPIOxPiny = "A1",
};

Encoder encoder = {
    .TIM = TIM8,
    .channel = "1 | 2",
    .GPIOxPiny = "C6 | C7",
};

extern Serial serial;
extern mADC adc;
extern DMA ADC_DMA;
extern mDAC dac;
extern DMA DAC_DMA;
extern Timer sampler;
extern Timer timer;

extern int16_t count;

void HAL_MspInit(void) {

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.usart) {
        __HAL_RCC_USARTx_CLK_ENABLE(serial.usart);

        GPIO RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.usart),
        };
        strcpy(RX.GPIOxPiny, serial.RX);
        GPIO_init(&RX);

        GPIO TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.usart),
        };
        strcpy(TX.GPIOxPiny, serial.TX);
        GPIO_init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(serial.usart));
        HAL_NVIC_SetPriority(USARTx_IRQn(serial.usart), 3, 3);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == sampler.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(sampler.TIM);

    } else if (htim->Instance == timer.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(timer.TIM);

        HAL_NVIC_SetPriority(TIMx_IRQN(timer.TIM), 1, 3);
        HAL_NVIC_EnableIRQ(TIMx_IRQN(timer.TIM));
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == pwm.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(pwm.TIM);

        GPIO gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(pwm.TIM),
        };
        strcpy(gpio.GPIOxPiny, pwm.GPIOxPiny);
        GPIO_init(&gpio);
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == encoder.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(encoder.TIM),
        };
        strcpy(gpio.GPIOxPiny, encoder.GPIOxPiny);
        GPIO_init(&gpio);
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == adc.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(adc.ADCx);

        GPIO gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, adc.GPIOxPiny);
        GPIO_init(&gpio);

        __HAL_RCC_DMAx_CLK_ENABLE(ADC_DMA.DMAx);

        __HAL_LINKDMA(&adc.Handler, DMA_Handle, ADC_DMA.Handler);
    }
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == dac.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, dac.GPIOxPiny);
        GPIO_init(&gpio);

        __HAL_RCC_DMAx_CLK_ENABLE(DAC_DMA.DMAx);

        __HAL_LINKDMA(&dac.Handler, DMA_Handle1, DAC_DMA.Handler);
    }
}