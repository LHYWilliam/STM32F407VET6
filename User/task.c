#include "FreeRTOS.h"
#include "timers.h"

#include "Encoder.h"
#include "Key.h"
#include "LED.h"
#include "PWM.h"
#include "Serial.h"
#include "Servo.h"

extern LED_t LED0;
extern LED_t LED1;
extern Key_t Key0;
extern Serial_t Serial1;
extern Serial_t Serial2;
extern PWM_t PWM;
extern Servo_t Servo;
extern Encoder_t Encoder;

extern uint32_t ADC_Data[];

void vMainTaskCode(void *pvParameters) {
    Servo_SetAngle(&Servo, 1, 110.);
    Servo_SetAngle(&Servo, 2, 135.);
    vTaskDelay(pdMS_TO_TICKS(1000));

    const uint16_t Period = 60000;

    for (;;) {
        // if (Key_IsPressing(&Key0)) {
        //     LED_On(&LED1);
        // } else {
        //     LED_Off(&LED1);
        // }

        // vTaskDelay(pdMS_TO_TICKS(100));

        // for (uint16_t i = 0; i < Period; i++) {
        //     Servo_SetAngle(&Servo, 2, 45. + 90. / Period * i);
        //     Servo_SetAngle(&Servo, 3, 135. - 90. / Period * (Period - i));
        //     vTaskDelay(pdMS_TO_TICKS(1));
        // }
        // for (uint16_t i = Period; i > 0; i--) {
        //     Servo_SetAngle(&Servo, 2, 135. - 90. / Period * (Period - i));
        //     Servo_SetAngle(&Servo, 3, 45. + 90. / Period * i);
        //     vTaskDelay(pdMS_TO_TICKS(1));
        // }

        // int16_t Count = __HAL_TIM_GetCounter(&Encoder.Handler);
        // Serial_Printf(&Serial, "Encoder Count: %d\r\n", Count);

        // Serial_Printf(&Serial, "ADC Data: %d\n", ADC_Data[0]);

        if (Serial2.RecieveFlag == SET) {
            int16_t dx =
                (int16_t)((Serial2.HexData[0] << 8) | Serial2.HexData[1]);
            int16_t dy =
                (int16_t)((Serial2.HexData[2] << 8) | Serial2.HexData[3]);
            Serial_Printf(&Serial1, "dx: %d, dy: %d\n", dx, dy);

            Servo_UpdateCompare(&Servo, 1, dx);
            Servo_UpdateCompare(&Servo, 2, dy);

            Serial_Clear(&Serial2);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // Serial_Printf(&Serial, "LED toggle at %d ms\r\n",
    //               xTaskGetTickCount() * portTICK_PERIOD_MS);
    LED_Toggle(&LED0);
}

void vApplicationTickHook() { HAL_IncTick(); }