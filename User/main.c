#include "RTE_Components.h"
#include CMSIS_device_header

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include <stdio.h>

#include "Encoder.h"
#include "GrayScaleSensor.h"
#include "ICM42688.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "PWM.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"
#include "Timer.h"

LED_t LED1 = {
    .GPIOxPiny = D13,
    .Mode = LEDMode_PullUp,
};

LED_t LED2 = {
    .GPIOxPiny = D12,
    .Mode = LEDMode_PullUp,
};

LED_t LED3 = {
    .GPIOxPiny = D11,
    .Mode = LEDMode_PullUp,
};

Key_t Key1 = {
    .GPIOxPiny = B12,
    .Mode = KeyMode_PullUp,
};

Key_t Key2 = {
    .GPIOxPiny = B13,
    .Mode = KeyMode_PullUp,
};

Key_t Key3 = {
    .GPIOxPiny = B14,
    .Mode = KeyMode_PullUp,
};

Key_t Key4 = {
    .GPIOxPiny = B15,
    .Mode = KeyMode_PullUp,
};

Serial_t Serial1 = {
    .USART = USART1,
    .TX = A9,
    .RX = A10,
    .Baudrate = 115200,
    .Default = ENABLE,
};

Serial_t Serial2 = {
    .USART = USART2,
    .TX = D5,
    .RX = D6,
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
};

Serial_t Serial3 = {
    .USART = USART3,
    .TX = D8,
    .RX = D9,
    .Baudrate = 115200,
};

/* FLASH
CS      --->    A4
CLK     --->    A5
MISO    --->    A6
MOSI    --->    A7
*/

PWM_t ServoPWM;

Servo_t Servo1 = {
    .TIMx = TIM8,
    .Channel = {1, 2},
    .GPIOxPiny = {C6, C7},
    .PWM = &ServoPWM,
    .PWM_Init = DISABLE,
};

Servo_t Servo2 = {
    .TIMx = TIM8,
    .Channel = {3, 4},
    .GPIOxPiny = {C8, C9},
    .PWM = &ServoPWM,
    .PWM_Init = ENABLE,
};

Encoder_t Encoder1 = {
    .TIM = TIM12,
    .Channel = {1, 2},
    .GPIOxPiny = {B14, B15},
};

Encoder_t Encoder2 = {
    .TIM = TIM4,
    .Channel = {1, 2},
    .GPIOxPiny = {B6, B7},
};

PWM_t MotorPWM;

Motor_t Motor1 = {
    .TIMx = TIM1,
    .Channel = 1,
    .PWM = E9,
    .IN1 = E8,
    .IN2 = E7,
};

Motor_t Motor2 = {
    .TIMx = TIM1,
    .Channel = 2,
    .PWM = E11,
    .IN1 = E10,
    .IN2 = E12,
};

#define ADC_DataLength 1
uint32_t ADC_Data[1];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel = {11, 14, 15},
            .GPIOxPiny = {C0, C4, C5},
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
    .SPIx = SPI2,
    .SCLK = B10,
    .MISO = C2,
    .MOSI = C3,
    .CS = C1,
    .Kp = 0.5f,
    .Ki = 0.001f,
};

GrayScaleSensor_t GrayScaleSensor = {
    .SCL = B8,
    .SDA = B9,
    .Addr = 0x4C,
    .Mode = GrayScaleSensorMode_Analog,
};

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ);

int main() {
    HAL_Init();
    SystemClock_Config(25, 336, 2, 7);

    LED_Init(&LED1);
    LED_Init(&LED2);
    LED_Init(&LED3);

    Key_Init(&Key1);
    Key_Init(&Key2);
    // Key_Init(&Key3);
    // Key_Init(&Key4);

    Serial_Init(&Serial1);
    Serial_Init(&Serial2);
    Serial_Init(&Serial3);

    Servo_Init(&Servo1);
    Servo_Init(&Servo2);

    // Encoder_Init(&Encoder1);
    // Encoder_Init(&Encoder2);

    // Timer_Init(&Timer);
    // PWM_Init(&PWM);
    // Servo_Init(&Servo);
    // Sampler_Init(&Sampler);

    // ICM42688_Init(&ICM42688);
    // GrayScaleSensor_Init(&GrayScaleSensor);

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

    // HAL_StatusTypeDef ret = HAL_OK;
    // RCC_OscInitTypeDef RCC_OscInitStructure;
    // RCC_ClkInitTypeDef RCC_ClkInitStructure;

    // __HAL_RCC_PWR_CLK_ENABLE();

    // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    // RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    // RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    // RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    // RCC_OscInitStructure.PLL.PLLM = PLLM;
    // RCC_OscInitStructure.PLL.PLLN = PLLN;
    // RCC_OscInitStructure.PLL.PLLP = PLLP;
    // RCC_OscInitStructure.PLL.PLLQ = PLLQ;
    // ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);

    // if (ret != HAL_OK)
    //     while (1)
    //         ;

    // RCC_ClkInitStructure.ClockType =
    //     (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
    //      RCC_CLOCKTYPE_PCLK2);
    // RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    // RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    // RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV4;
    // RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV2;
    // ret = HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_5);

    // if (ret != HAL_OK)
    //     while (1)
    //         ;

    // if (HAL_GetREVID() == 0x1001) {
    //     __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    // }
}