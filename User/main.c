#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "ADC.h"
#include "DAC.h"
#include "Key.h"
#include "LED.h"
#include "Serial.h"

LED LED0 = {
    .GPIOxPiny = "A1",
};

LED LED1 = {
    .GPIOxPiny = "A2",
};

Key key0 = {
    .GPIOxPiny = "C0",
};

Key key1 = {
    .GPIOxPiny = "A0",
};

Serial serial = {
    .USART = USART1,
    .TX = "A9",
    .RX = "A10",
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
};

#define DAC_DataLength 16
#define ADC_DataLength 16
#define DAC_Frequency  2000
#define ADC_Frequency  DAC_Frequency * 16

mDAC dac = {
    .Channel = "1",
    .GPIOxPiny = "A4",
    .Length = DAC_DataLength,
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

mADC adc = {
    .ADCx = ADC1,
    .Channel = "5",
    .GPIOxPiny = "A5",
    .Length = ADC_DataLength,
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

uint32_t DACData[] = {
    2047, 2830, 3494, 3938, 4094, 3938, 3494, 2830,
    2047, 1263, 599,  155,  0,    155,  599,  1263,
};
uint32_t ADCValue[ADC_DataLength];

TimerHandle_t LEDTimer;
void vLEDTimerCallback(TimerHandle_t xTimer);

TimerHandle_t SerialTimer;
void vSerialTimerCallback(TimerHandle_t xTimer);

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_Init(&LED0);
    LED_Init(&LED1);
    Key_Init(&key0);
    Key_Init(&key1);
    Serial_Init(&serial);

    DAC_Init(&dac);
    DAC_DMAStart(&dac, DACData, dac.Length);

    ADC_Init(&adc);
    ADC_DMAStart(&adc, ADCValue, adc.Length);

    LEDTimer = xTimerCreate("LEDTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                            vLEDTimerCallback);
    SerialTimer = xTimerCreate("SerialTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                               vSerialTimerCallback);

    xTimerStart(LEDTimer, 0);
    xTimerStart(SerialTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}