#include "FreeRTOS.h"
#include "timers.h"

#include "Key.h"
#include "LED.h"
#include "PWM.h"
#include "Serial.h"
#include "Servo.h"

extern LED_t LED0;
extern LED_t LED1;
extern Key_t Key0;
extern Serial_t Serial;
extern PWM_t PWM;
extern Servo_t Servo;

void vMainTaskCode(void *pvParameters) {
    Servo_SetAngle(&Servo, 2, 45.);
    Servo_SetAngle(&Servo, 3, 135.);
    vTaskDelay(pdMS_TO_TICKS(1000));

    const uint16_t Period = 60000;

    for (;;) {
        // if (Key_IsPressing(&Key0)) {
        //     LED_On(&LED1);
        // } else {
        //     LED_Off(&LED1);
        // }

        // vTaskDelay(pdMS_TO_TICKS(100));

        for (uint16_t i = 0; i < Period; i++) {
            Servo_SetAngle(&Servo, 2, 45. + 90. / Period * i);
            Servo_SetAngle(&Servo, 3, 135. - 90. / Period * (Period - i));
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        for (uint16_t i = Period; i > 0; i--) {
            Servo_SetAngle(&Servo, 2, 135. - 90. / Period * (Period - i));
            Servo_SetAngle(&Servo, 3, 45. + 90. / Period * i);
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // Serial_Printf(&Serial, "LED toggle at %d ms\r\n",
    //               xTaskGetTickCount() * portTICK_PERIOD_MS);
    // LED_Toggle(&LED0);
}

void vApplicationTickHook() { HAL_IncTick(); }