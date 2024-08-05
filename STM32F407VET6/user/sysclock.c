#include "stm32f4xx_hal.h"

void SystemClock_Config(void) {
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure;
    RCC_ClkInitTypeDef RCC_ClkInitStructure;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLM = 25;
    RCC_OscInitStructure.PLL.PLLN = 336;
    RCC_OscInitStructure.PLL.PLLP = 2;
    RCC_OscInitStructure.PLL.PLLQ = 7;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);

    if (ret != HAL_OK)
        while (1)
            ;

    RCC_ClkInitStructure.ClockType =
        (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
         RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_5);

    if (ret != HAL_OK)
        while (1)
            ;

    if (HAL_GetREVID() == 0x1001) {
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}