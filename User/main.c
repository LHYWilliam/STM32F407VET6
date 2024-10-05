#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "lv_port_disp.h"
#include "lvgl.h"

#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "Serial.h"
#include "Timer.h"
#include "Touch.h"

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

LCD_Handler mLCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch_Handler mTouch = {
    .Direction = LCD_Vertical,
};

Timer_Handler LVGLTimer = {
    .TIMx = TIM2,
    .ms = 10,
    .Interrupt = ENABLE,
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
    Touch_Init(&mTouch);

    lv_init();
    lv_port_disp_init();
    
    Timer_Init(&LVGLTimer);

    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    lv_label_set_recolor(label1,
                         true); /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# "
                              "label, align the lines to the center "
                              "and wrap long text automatically.");
    lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t *label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2,
                           LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);

    LEDTimer = xTimerCreate("LEDTimer", pdMS_TO_TICKS(100), pdTRUE, 0,
                            vLEDTimerCallback);

    xTimerStart(LEDTimer, 0);
    vTaskStartScheduler();

    for (;;) {
    }
}