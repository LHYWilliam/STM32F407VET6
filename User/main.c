#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "lvgl.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "Serial.h"
#include "Signal.h"
#include "Touch.h"

LED_t LED0 = {
    .GPIOxPiny = "A1",
};

LED_t LED1 = {
    .GPIOxPiny = "A2",
};

Key_t Key0 = {
    .GPIOxPiny = "C0",
};

Key_t Key1 = {
    .GPIOxPiny = "A0",
};

Serial_t Serial = {
    .USART = USART1,
    .TX = "A9",
    .RX = "A10",
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
};

LCD_t LCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch_t Touch = {
    .Direction = LCD_Vertical,
};

#define DAC_DataLength 32
#define DAC_Frequency  1000
#define ADC_DataLength 32
#define ADC_Frequency  DAC_Frequency * 32

uint32_t DAC_Data[DAC_DataLength];
uint32_t ADC_Data[ADC_DataLength];

SignalGenerator_t Generator = {
    .Data = DAC_Data,
    .Length = DAC_DataLength,
    .DAC =
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

SignalSampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
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

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TimerHandle_t xKeyTimer;
void vKeyTimerCallback(TimerHandle_t pxTimerr);

TaskHandle_t xKeyTaskHandle;
void vKeyTaskCode(void *pvParameters);

TaskHandle_t xLVGLTaskHandle;
void vLVGLTaskCode(void *pvParameters);

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_Init(&LED0);
    LED_Init(&LED1);
    Key_Init(&Key0);
    Key_Init(&Key1);
    Serial_Init(&Serial);

    // LCD_Init(&LCD);
    // Touch_Init(&Touch);

    // Sin_Generate(Generator.Data, Generator.Length);
    // SignalGenerator_Init(&Generator);
    // SignalSampler_Init(&Sampler);

    // lv_init();
    // lv_port_disp_init();
    // lv_port_indev_init();

    xLEDTimer = xTimerCreate("xLEDTimer", pdMS_TO_TICKS(100), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    xKeyTimer = xTimerCreate("xKeyTimer", pdMS_TO_TICKS(10), pdTRUE, (void *)1,
                             vKeyTimerCallback);
    xTaskCreate(vKeyTaskCode, "vKeyTask", 128, NULL, 1, &xKeyTaskHandle);
    // xTaskCreate(vLVGLTaskCode, "vLVGLTask", 1024, NULL, 1, &xLVGLTaskHandle);

    xTimerStart(xLEDTimer, 0);
    xTimerStart(xKeyTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}