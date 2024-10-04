#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "Serial.h"
#include "Touch.h"

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

LCD mLCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch mTouch = {
    .Direction = LCD_Vertical,
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
    LCD_Init(&mLCD);
    Touch_Init(&mTouch, &mLCD);

    LEDTimer = xTimerCreate("LEDTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                            vLEDTimerCallback);

    xTimerStart(LEDTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}