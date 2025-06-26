#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "Encoder.h"
#include "ICM42688.h"
#include "Key.h"
#include "LED.h"
#include "PWM.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"
#include "Timer.h"
#include <stdio.h>

LED_t LED0 = {
    .GPIOxPiny = A1,
    .Mode = LEDMode_PullUp,
};

LED_t LED1 = {
    .GPIOxPiny = A2,
    .Mode = LEDMode_PullUp,
};

Key_t Key0 = {
    .GPIOxPiny = C0,
    .Mode = KeyMode_PullDown,
};

Serial_t Serial1 = {
    .USART = USART1,
    .RX = A10,
    .TX = A9,
    .Baudrate = 115200,
    .Default = ENABLE,
};

Serial_t Serial2 = {
    .USART = USART2,
    .RX = A3,
    .TX = A2,
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
};

Timer_t Timer = {
    .TIMx = TIM3,
    .ms = 500,
    .Interrupt = ENABLE,
    .Priority = 8,
};

PWM_t PWM = {
    .TIM = TIM2,
    .Channel = {2, 3},
    .GPIOxPiny = {A1, A2},
    .Prescaler = 84 - 1,
    .Period = 1000 - 1,
};

Servo_t Servo = {
    .PWM =
        {
            .TIM = TIM3,
            .Channel = {1, 2},
            .GPIOxPiny = {A6, A7},
        },
};

Encoder_t Encoder = {
    .TIM = TIM3,
    .Channel = {1, 2},
    .GPIOxPiny = {A6, A7},
};

#define ADC_DataLength 1
uint32_t ADC_Data[1];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel = {5},
            .GPIOxPiny = {A5},
        },
    .DMA =
        {
            .DMAx = DMA2,
            .Channel = 0,
            .Stream = 0,
        },
    .Timer =
        {
            .TIMx = TIM3,
            .Hz = 100,
        },
};

ICM42688_t ICM42688 = {
    .SCLK = A5,
    .MISO = A6,
    .MOSI = A7,
    .CS = A4,
    .SPIx = SPI1,
    .Kp = 0.5f,
    .Ki = 0.001f,
};

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ);

int main() {
    HAL_Init();
    SystemClock_Config(25, 336, 2, 4);

    LED_Init(&LED0);
    // LED_Init(&LED1);
    // Key_Init(&Key0);

    Serial_Init(&Serial1);
    // Serial_Init(&Serial2);
    // Timer_Init(&Timer);
    // PWM_Init(&PWM);
    // Servo_Init(&Servo);
    // Encoder_Init(&Encoder);
    // Sampler_Init(&Sampler);

    ICM42688_Init(&ICM42688);

    xTaskCreate(vMainTaskCode, "vMainTask", 128, NULL, 1, &xMainTaskHandle);

    xLEDTimer = xTimerCreate("xLEDTimer", pdMS_TO_TICKS(200), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    xTimerStart(xLEDTimer, 0);

    vTaskStartScheduler();
}

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ) {
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure;
    RCC_ClkInitTypeDef RCC_ClkInitStructure;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLM = PLLM;
    RCC_OscInitStructure.PLL.PLLN = PLLN;
    RCC_OscInitStructure.PLL.PLLP = PLLP;
    RCC_OscInitStructure.PLL.PLLQ = PLLQ;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);

    if (ret != HAL_OK) {
        while (1)
            ;
    }

    RCC_ClkInitStructure.ClockType =
        (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
         RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_5);

    if (ret != HAL_OK) {
        while (1)
            ;
    }

    if (HAL_GetREVID() == 0x1001) {
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}