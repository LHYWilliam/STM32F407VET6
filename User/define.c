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
#include "OLED.h"
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

OLED_t OLED = {
    .SCL = PD14,
    .SDA = PD15,
    .Width = 128,
    .Height = 64,
    .I2C = ENABLE,
};

GWGray_t GWGray = {
    .SCL = PB8,
    .SDA = PB9,
    .DevAddr = 0x4C,
};

ICM42688_t ICM42688 = {
    .SPIx = SPI2,
    .SCLK = PB10,
    .MISO = PC2,
    .MOSI = PC3,
    .CS = PC1,
};

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ);