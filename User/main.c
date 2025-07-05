#include "RTE_Components.h"
#include CMSIS_device_header

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include <stdio.h>

#include "Encoder.h"
#include "GWGray.h"
#include "ICM42688.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "PWM.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"

LED_t LEDRed = {
    .GPIOxPiny = PD11,
    .Mode = LEDMode_PullUp,
};

LED_t LEDGreen = {
    .GPIOxPiny = PD12,
    .Mode = LEDMode_PullUp,
};

LED_t LEDBlue = {
    .GPIOxPiny = PD13,
    .Mode = LEDMode_PullUp,
};

Key_t Key1 = {
    .GPIOxPiny = PE14,
    .Mode = KeyMode_PullUp,
};

Key_t Key2 = {
    .GPIOxPiny = PE15,
    .Mode = KeyMode_PullUp,
};

Key_t Key3 = {
    .GPIOxPiny = PB12,
    .Mode = KeyMode_PullUp,
};

Key_t Key4 = {
    .GPIOxPiny = PB13,
    .Mode = KeyMode_PullUp,
};

Serial_t SerialBoard = {
    .USART = USART1,
    .TX = PA9,
    .RX = PA10,
    .Baudrate = 115200,
    .Default = ENABLE,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
};

Serial_t SerialBluetooth = {
    .USART = USART2,
    .TX = PD5,
    .RX = PD6,
    .Baudrate = 9600,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
};

Serial_t SerialK230 = {
    .USART = USART3,
    .TX = PD8,
    .RX = PD9,
    .Baudrate = 115200,
};

Serial_t SerialJY61P = {
    .USART = UART4,
    .TX = PC11,
    .RX = PC10,
    .Baudrate = 115200,
};

PWM_t ServoPWM;

Servo_t Servo1 = {
    .TIMx = TIM8,
    .Channel = {1, 2},
    .GPIOxPiny = {PC6, PC7},
    .PWM = &ServoPWM,
    .PWM_Init = DISABLE,
};

Servo_t Servo2 = {
    .TIMx = TIM8,
    .Channel = {3, 4},
    .GPIOxPiny = {PC8, PC9},
    .PWM = &ServoPWM,
    .PWM_Init = ENABLE,
};

Encoder_t EncoderLeft = {
    .TIM = TIM4,
    .Channel = {1, 2},
    .GPIOxPiny = {PB6, PB7},
};

Encoder_t EncoderRight = {
    .TIM = TIM3,
    .Channel = {1, 2},
    .GPIOxPiny = {PB4, PB5},
};

PWM_t MotorPWM;

Motor_t MotorLeft = {
    .TIMx = TIM1,
    .Channel = 2,
    .PWM = PE11,
    .IN1 = PE10,
    .IN2 = PE12,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .Invert = ENABLE,
    .PWM_Init = DISABLE,
};

Motor_t MotorRight = {
    .TIMx = TIM1,
    .Channel = 1,
    .PWM = PE9,
    .IN1 = PE8,
    .IN2 = PE7,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .PWM_Init = ENABLE,
};

#define ADC_DataLength 3
uint32_t ADC_Data[ADC_DataLength];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel =
                {
                    3,
                    4,
                    5,
                },
            .GPIOxPiny = {PA3, PA4, PA5},
        },
    .DMA =
        {
            .DMAx = DMA2,
            .Channel = 0,
            .Stream = 0,
        },
    .Timer =
        {
            .TIMx = TIM2,
            .Hz = 100,
        },
};

ICM42688_t ICM42688 = {
    .SPIx = SPI2,
    .SCLK = PB10,
    .MISO = PC2,
    .MOSI = PC3,
    .CS = PC1,
};

GWGray_t GWGray = {
    .SCL = PB8,
    .SDA = PB9,
    .DevAddr = 0x4C,
};

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ);

int main() {
    HAL_Init();
    // SystemClock_Config(25, 336, 2, 4);
    SystemClock_Config(4, 168, 2, 4);

    LED_Init(&LEDRed);
    LED_Init(&LEDGreen);
    LED_Init(&LEDBlue);

    Key_Init(&Key1);
    Key_Init(&Key2);
    Key_Init(&Key3);
    Key_Init(&Key4);

    Serial_Init(&SerialBoard);
    Serial_Init(&SerialBluetooth);
    Serial_Init(&SerialK230);
    Serial_Init(&SerialJY61P);

    Servo_Init(&Servo1);
    Servo_Init(&Servo2);

    Encoder_Init(&EncoderLeft);
    Encoder_Init(&EncoderRight);

    Motor_Init(&MotorLeft);
    Motor_Init(&MotorRight);
    Motor_SetSpeed(&MotorLeft, 0);
    Motor_SetSpeed(&MotorRight, 0);

    Sampler_Init(&Sampler);

    ICM42688_Init(&ICM42688);
    GWGray_Init(&GWGray);

    xTaskCreate(vMainTaskCode, "vMainTask", 128, NULL, 1, &xMainTaskHandle);

    xLEDTimer = xTimerCreate("xLEDTimer", pdMS_TO_TICKS(200), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    xTimerStart(xLEDTimer, 0);

    vTaskStartScheduler();
}

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}