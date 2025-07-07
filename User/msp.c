#include "main.h"

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
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

    } else if (htim->Instance == MotorPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(MotorPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(MotorPWM.TIMx),
        };

        for (uint8_t i = 0; MotorPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, MotorPWM.GPIOxPiny[i]);
        }
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == ServoPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(ServoPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(ServoPWM.TIMx),
        };

        for (uint8_t i = 0; ServoPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, ServoPWM.GPIOxPiny[i]);
        }
    } else if (htim->Instance == MotorPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(MotorPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(MotorPWM.TIMx),
        };

        for (uint8_t i = 0; MotorPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, MotorPWM.GPIOxPiny[i]);
        }
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == EncoderLeft.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(EncoderLeft.TIM),
        };
        GPIO_InitPin(&GPIO, EncoderLeft.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, EncoderLeft.GPIOxPiny[1]);

    } else if (htim->Instance == EncoderRight.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(EncoderRight.TIM),
        };
        GPIO_InitPin(&GPIO, EncoderRight.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, EncoderRight.GPIOxPiny[1]);
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == Sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(Sampler.ADC.ADCx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_ANALOG,
        };
        for (uint8_t i = 0; Sampler.ADC.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Sampler.ADC.GPIOxPiny[i]);
        }

        if (Sampler.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Sampler.DMA.DMAx);

            DMA_Init(&Sampler.DMA);

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