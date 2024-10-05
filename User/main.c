#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"
#include "Serial.h"

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

TimerHandle_t LEDTimer;
void vLEDTimerCallback(TimerHandle_t xTimer);

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_Init(&LED0);
    LED_Init(&LED1);
    Key_Init(&key0);
    Key_Init(&key1);
    Serial_Init(&serial);

    LEDTimer = xTimerCreate("LEDTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                            vLEDTimerCallback);

    xTimerStart(LEDTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}