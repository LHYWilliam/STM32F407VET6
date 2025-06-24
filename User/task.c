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
    Servo_SetAngle180(&Servo, 1, 110.);
    Servo_SetAngle270(&Servo, 2, 135.);
    vTaskDelay(pdMS_TO_TICKS(1000));

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
            // int16_t temp1 = (Serial2.HexData[0] << 8 | Serial2.HexData[1]);
            // int16_t temp2 = (Serial2.HexData[2] << 8 | Serial2.HexData[3]);
            // int16_t temp3 = (Serial2.HexData[4] << 8 | Serial2.HexData[5]);
            // int16_t temp4 = (Serial2.HexData[6] << 8 | Serial2.HexData[7]);

            // float dH = temp1 * 1.f + temp2 / 1000.0f;
            // float dV = temp3 * 1.f + temp4 / 1000.0f;

            // Serial_Printf(&Serial1, "dH: %d.%d, dV: %d.%d\n", (int16_t)dH,
            //               (int16_t)(dH * 1000) % 1000, (int16_t)dV,
            //               (int16_t)(dV * 1000) % 1000);

            int16_t dH =
                (int16_t)((Serial2.HexData[0] << 8) | Serial2.HexData[1]);
            int16_t dV =
                (int16_t)((Serial2.HexData[2] << 8) | Serial2.HexData[3]);

            Servo_UpdateCompare180(&Servo, 1, dV);
            Servo_UpdateCompare270(&Servo, 2, dH);

            Serial_Clear(&Serial2);

            Serial_Printf(&Serial1, "dx, dy: %d, %d\n", dH, dV);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // Serial_Printf(&Serial, "LED toggle at %d ms\r\n",
    //               xTaskGetTickCount() * portTICK_PERIOD_MS);
    LED_Toggle(&LED0);
}

void vApplicationTickHook() { HAL_IncTick(); }