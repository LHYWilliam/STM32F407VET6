#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"
#include "Serial.h"
#include "Signal.h"

LED_Handler LED0 = {
    .GPIOxPiny = "A1",
};

LED_Handler LED1 = {
    .GPIOxPiny = "A2",
};

Key_Handler key0 = {
    .GPIOxPiny = "C0",
};

Key_Handler key1 = {
    .GPIOxPiny = "A0",
};

Serial_Handler serial = {
    .USART = USART1,
    .TX = "A9",
    .RX = "A10",
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
};

#define DAC_DataLength 32
#define DAC_Frequency  1000
#define ADC_DataLength 32
#define ADC_Frequency  DAC_Frequency * 32

uint32_t DAC_Data[DAC_DataLength];
uint32_t ADC_Data[DAC_DataLength];

SignalGenerator_Handler generator = {
    .Data = DAC_Data,
    .Length = DAC_DataLength,
    ._DAC =
        {
            .Channel = "1",
            .GPIOxPiny = "A4",
        },
    .DMA =
        {
            .DMAx = DMA1,
            .Channel = 7,
            .Stream = 5,
        },
    .Timer =
        {
            .TIMx = TIM2,
            .Hz = DAC_Frequency,
        },
};

SignalSampler_Handler sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    ._ADC =
        {
            .ADCx = ADC1,
            .Channel = "5",
            .GPIOxPiny = "A5",
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
            .Hz = ADC_Frequency,
        },
};

TimerHandle_t LEDTimer;
void vLEDTimerCallback(TimerHandle_t xTimer);

TaskHandle_t xKeyTaskHandle;
void vKeyTaskCode(void *pvParameters);

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_Init(&LED0);
    LED_Init(&LED1);
    Key_Init(&key0);
    Key_Init(&key1);
    Serial_Init(&serial);

    Sin_Generate(generator.Data, generator.Length);
    SignalGenerator_Init(&generator);
    SignalSampler_Init(&sampler);

    LEDTimer = xTimerCreate("LEDTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                            vLEDTimerCallback);
    xTaskCreate(vKeyTaskCode, "vKeyTask", 128, NULL, 1,
                &xKeyTaskHandle);

    xTimerStart(LEDTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}