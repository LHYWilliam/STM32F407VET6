#include "FreeRTOS.h"
#include "timers.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "events_init.h"
#include "gui_guider.h"

#include "Key.h"
#include "LCD.h"
#include "LED.h"
#include "Serial.h"
#include "Signal.h"
#include <stdint.h>

extern LED_t LED0;
extern Key_t Key0;
extern Key_t Key1;
extern Serial_t Serial;
extern LCD_t LCD;

extern Sampler_t Sampler;

lv_ui guider_ui;

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED0); }

void vKeyTimerCallback(TimerHandle_t pxTimer) {
    Key_Update(&Key0, 10);
    Key_Update(&Key1, 10);
}

void vKeyTaskCode(void *pvParameters) {
    for (;;) {
        if (Key_Read(&Key0) == KeyEvent_Click) {
            for (uint32_t i = 0; i < Sampler.Length; i++) {
                Serial_Printf(&Serial, "%d\r\n", Sampler.Data[i]);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLVGLTaskCode(void *pvParameters) {
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    setup_ui(&guider_ui);
    events_init(&guider_ui);

    static lv_point_t line_points[128] = {};

    for (;;) {
        if (lv_scr_act() == guider_ui.SignalScreen) {
            Sampler_UpdateIndex(&Sampler);

            uint32_t Index = Sampler.Index;
            for (uint32_t i = 0; i < Sampler.Length; i++) {
                Index = (Index + 1) % Sampler.Length;

                line_points[i].x =
                    (float)i * (LCD.Width - 1) / (Sampler.Length - 1);
                line_points[i].y =
                    (LCD.Height - 1) - ((float)Sampler.Data[Index] *
                                            (LCD.Height - 1) / 2. / 4095. +
                                        (LCD.Height - 1) / 4.);
            }

            lv_line_set_points(guider_ui.SignalScreen_line, line_points,
                               Sampler.Length);

            float temp = Sampler.Data[Sampler.Index] * 3.3 / 4095.;
            lv_label_set_text_fmt(guider_ui.SignalScreen_ADC_label,
                                  "ADC: %d.%d V ", (uint16_t)temp,
                                  (uint16_t)(temp * 1000) % 1000);
        }

        lv_timer_handler();
    }
}

void vApplicationTickHook() {
    HAL_IncTick();
    lv_tick_inc(1);
}