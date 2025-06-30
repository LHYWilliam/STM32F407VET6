#include "FreeRTOS.h"
#include "timers.h"

#include <stdint.h>
#include <stdio.h>

#include "Encoder.h"
#include "GrayScaleSensor.h"
#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "Serial.h"
#include "Servo.h"

extern LED_t LED1;
extern LED_t LED2;
extern LED_t LED3;

extern Key_t Key1;
extern Key_t Key2;
extern Key_t Key3;
extern Key_t Key4;

extern Serial_t Serial1;
extern Serial_t Serial2;
extern Serial_t Serial3;

extern Servo_t Servo1;
extern Servo_t Servo2;

extern Encoder_t Encoder1;
extern Encoder_t Encoder2;

extern Motor_t Motor1;
extern Motor_t Motor2;

// extern ICM42688_t ICM42688;
// extern GrayScaleSensor_t GrayScaleSensor;

extern uint32_t ADC_Data[];

void vMainTaskCode(void *pvParameters) {
    //////////////////// K230 Test ///////////////////
    // Servo_SetAngle180(&Servo, 1, 110.);
    // Servo_SetAngle270(&Servo, 2, 135.);
    // vTaskDelay(pdMS_TO_TICKS(1000));

    // for (;;) {
    //     if (Serial2.RecieveFlag == SET) {
    //         int16_t dH =
    //             (int16_t)((Serial2.HexData[0] << 8) | Serial2.HexData[1]);
    //         int16_t dV =
    //             (int16_t)((Serial2.HexData[2] << 8) | Serial2.HexData[3]);

    //         Servo_UpdateCompare180(&Servo, 1, dV);
    //         Servo_UpdateCompare270(&Servo, 2, dH);

    //         Serial_Clear(&Serial2);

    //         Serial_Printf(&Serial1, "dx, dy: %d, %d\n", dH, dV);
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(1));
    // }

    /////////////// ICM42688 Test ////////////////////////
    // float YawPitchRoll[3];
    // for (;;) {
    //     ICM42688_AHRS_Update(&ICM42688);

    //     if (ICM42688.CalibrationFinished == RESET) {
    //         vTaskDelay(pdMS_TO_TICKS(10));
    //         continue;
    //     }

    //     ICM42688_AHRS_GetYawPitchRoll(&ICM42688, YawPitchRoll);

    //     printf(" Yaw: %6.2f, Pitch: %6.2f, Roll: %6.2f\r\n", YawPitchRoll[0],
    //            YawPitchRoll[1], YawPitchRoll[2]);

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    ////////////// GrayScacleSensor Test ///////////////////////
    // uint8_t GrayScaleData[8];
    // for (;;) {
    //     GrayScaleSensor_ReadAnalog(&GrayScaleSensor, GrayScaleData);

    //     printf("%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d\n", GrayScaleData[0],
    //            GrayScaleData[1], GrayScaleData[2], GrayScaleData[3],
    //            GrayScaleData[4], GrayScaleData[5], GrayScaleData[6],
    //            GrayScaleData[7]);

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    /////////////////////////  Servo Test ///////////////////////////////
    // for (;;) {
    //     for (uint8_t i = 0; i < 60; i++) {
    //         Servo_SetAngle180(&Servo1, 1, 45. + 90. / 60 * i);
    //         Servo_SetAngle180(&Servo1, 2, 45. + 90. / 60 * i);
    //         Servo_SetAngle180(&Servo2, 3, 45. + 90. / 60 * i);
    //         Servo_SetAngle180(&Servo2, 4, 45. + 90. / 60 * i);
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    //     for (uint8_t i = 60; i > 0; i--) {
    //         Servo_SetAngle180(&Servo1, 1, 135. - 90. / 60 * (60 - i));
    //         Servo_SetAngle180(&Servo1, 2, 135. - 90. / 60 * (60 - i));
    //         Servo_SetAngle180(&Servo2, 3, 135. - 90. / 60 * (60 - i));
    //         Servo_SetAngle180(&Servo2, 4, 135. - 90. / 60 * (60 - i));
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    // }

    /////////////////////// Encoder Test//////////////////////////
    // for (;;) {
    //     int16_t Count1 = Encoder_GetCounter(&Encoder1);
    //     int16_t Count2 = Encoder_GetCounter(&Encoder2);

    //     printf("Count1: %d, Count2: %d\n", Count1, Count2);

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    ///////////////// Motor Test /////////////////
    // for (;;) {
    //     for (int16_t i = -Motor1.Range; i < Motor1.Range; i++) {
    //         Motor_SetSpeed(&Motor1, i);
    //         Motor_SetSpeed(&Motor2, i);

    //         vTaskDelay(pdMS_TO_TICKS(1));
    //     }
    //     for (int16_t i = Motor1.Range; i > -Motor1.Range; i--) {
    //         Motor_SetSpeed(&Motor1, i);
    //         Motor_SetSpeed(&Motor2, i);

    //         vTaskDelay(pdMS_TO_TICKS(1));
    //     }
    // }

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // LED_Toggle(&LED1);
    Serial_Printf(&Serial1, "Hello from Serial1\n");
    Serial_Printf(&Serial2, "Hello from Serial2\n");
    Serial_Printf(&Serial3, "Hello from Serial3\n");
}

void vApplicationTickHook() { HAL_IncTick(); }