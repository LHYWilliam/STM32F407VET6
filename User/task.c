#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>

#include "Encoder.h"
#include "GrayScaleSensor.h"
#include "ICM42688.h"
#include "ICM42688_AHRS.h"
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
extern ICM42688_t ICM42688;
extern GrayScaleSensor_t GrayScaleSensor;

extern uint32_t ADC_Data[];

void vMainTaskCode(void *pvParameters) {
    // K230 to Servo
    /*
    Servo_SetAngle180(&Servo, 1, 110.);
    Servo_SetAngle270(&Servo, 2, 135.);
    vTaskDelay(pdMS_TO_TICKS(1000));

    for (;;) {
        if (Serial2.RecieveFlag == SET) {
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
    */

    // ICM42688
    /*
    float YawPitchRoll[3];
    for (;;) {
        ICM42688_AHRS_Update(&ICM42688);

        if (ICM42688.CalibrationFinished == RESET) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        ICM42688_AHRS_GetYawPitchRoll(&ICM42688, YawPitchRoll);

        printf(" Yaw: %6.2f, Pitch: %6.2f, Roll: %6.2f\r\n", YawPitchRoll[0],
               YawPitchRoll[1], YawPitchRoll[2]);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    */

    // GrayScacleSensor
    uint8_t GrayScaleData[8];
    for (;;) {
        GrayScaleSensor_ReadAnalog(&GrayScaleSensor, GrayScaleData);

        printf("%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d\n", GrayScaleData[0],
               GrayScaleData[1], GrayScaleData[2], GrayScaleData[3],
               GrayScaleData[4], GrayScaleData[5], GrayScaleData[6],
               GrayScaleData[7]);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LED0); }

void vApplicationTickHook() { HAL_IncTick(); }