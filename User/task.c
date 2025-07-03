#include "FreeRTOS.h"
#include "pid.h"
#include "timers.h"

#include <stdint.h>
#include <stdio.h>

#include "Encoder.h"
#include "GWGray.h"
#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "PID.h"
#include "Sampler.h"
#include "Serial.h"
#include "Servo.h"

extern LED_t LEDRed;
extern LED_t LEDGreen;
extern LED_t LEDBlue;

extern Key_t Key1;
extern Key_t Key2;
extern Key_t Key3;
extern Key_t Key4;

extern Serial_t SerialBoard;
extern Serial_t SerialBluetooth;
extern Serial_t SerialK230;
extern Serial_t SerialJY61P;

extern Servo_t Servo1;
extern Servo_t Servo2;

extern Encoder_t EncoderLeft;
extern Encoder_t EncoderRight;

extern Motor_t MotorLeft;
extern Motor_t MotorRight;

extern Sampler_t Sampler;

extern ICM42688_t ICM42688;
extern GWGray_t GWGray;

extern uint32_t ADC_Data[];

const float EncoderToPWM = 10000. / 183.;

PID_t MotorLeftSpeedPID = {
    .Kp = 4,
    .Ki = 20,
    .IMax = 10000,

};

// .Kd = 0.001,

PID_t MotorRightSpeedPID = {
    .Kp = 4,
    .Ki = 20,
    .IMax = 10000,

};

void vMainTaskCode(void *pvParameters) {
    // ----------------- Serial Test ----------------- //
    // for (;;) {
    //     Serial_Printf(&SerialBoard, "Hello from SerialBoard\n");
    //     Serial_Printf(&SerialBluetooth, "Hello from SerialBluetooth\n");
    //     Serial_Printf(&SerialK230, "Hello from SerialK230\n");
    //     Serial_Printf(&SerialJY61P, "Hello from SerialJY61P\n");

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ---------------- Key Test ---------------- //
    // for (;;) {
    //     if (Key_IsPressing(&Key1)) {
    //         printf("Key1 is pressing\n");
    //     }
    //     if (Key_IsPressing(&Key2)) {
    //         printf("Key2 is pressing\n");
    //     }
    //     if (Key_IsPressing(&Key3)) {
    //         printf("Key3 is pressing\n");
    //     }
    //     if (Key_IsPressing(&Key4)) {
    //         printf("Key4 is pressing\n");
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ---------------- LED Test ---------------- //
    // for (;;) {
    //     if (Key_IsPressing(&Key1)) {
    //         LED_On(&LEDRed);
    //     } else {
    //         LED_Off(&LEDRed);
    //     }
    //     if (Key_IsPressing(&Key2)) {
    //         LED_On(&LEDGreen);
    //     } else {
    //         LED_Off(&LEDGreen);
    //     }
    //     if (Key_IsPressing(&Key3)) {
    //         LED_On(&LEDBlue);
    //     } else {
    //         LED_Off(&LEDBlue);
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ---------------- Motor Test ---------------- //
    // Motor_SetSpeed(&MotorLeft, 1000);
    // Motor_SetSpeed(&MotorRight, -1000);

    // ---------------- Encoder Test ---------------- //
    // Motor_SetSpeed(&MotorLeft, 0);
    // Motor_SetSpeed(&MotorRight, 0);
    // for (;;) {
    //     int16_t CountLeft = Encoder_GetCounter(&EncoderLeft);
    //     int16_t CountRight = Encoder_GetCounter(&EncoderRight);

    //     printf("CountLeft, CountRight: %d, %d\n", CountLeft, CountRight);

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ----------------  Servo Test ---------------- //
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

    // ---------------- ICM42688 Test ---------------- //
    // float YawPitchRoll[3];
    // for (;;) {
    //     ICM42688_AHRS_Update(&ICM42688);

    //     if (ICM42688.CalibrationFinished == RESET) {
    //         vTaskDelay(pdMS_TO_TICKS(10));
    //         continue;
    //     }

    //     ICM42688_AHRS_GetYawPitchRoll(&ICM42688, YawPitchRoll);

    //     printf(" Yaw, Pitch, Roll: %6.2f, %6.2f, %6.2f\n", YawPitchRoll[0],
    //            YawPitchRoll[1], YawPitchRoll[2]);

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ---------------- Gray PID Test ---------------- //
    // for (;;) {
    //     int16_t Error = GWGray_CaculateAnalogError(&GWGray);

    //     printf("%d\n", Error);

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ----------------- MotorSpeedPID Test ----------------- //
    // int16_t TargetSpeed = 0;
    // Motor_SetSpeed(&MotorRight, TargetSpeed);

    // for (;;) {
    //     int16_t Count = Encoder_GetCounter(&EncoderRight);
    //     float Error = TargetSpeed - Count * EncoderToPWM;
    //     int16_t Out = PID_Caculate(&MotorSpeedPID, Error);

    //     Motor_SetSpeed(&MotorRight, Out);

    //     printf("Target, Real, Error, Out: %d, %d, %d, %d\n", TargetSpeed,
    //            (int16_t)(Count * EncoderToPWM), (int16_t)Error, Out);

    //     if (SerialBoard.RecieveFlag == SET) {
    //         TargetSpeed = SerialBoard.HexData[0] << 8 |
    //         SerialBoard.HexData[1];

    //         Serial_Clear(&SerialBoard);
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

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

    ////////////// Sampler Test /////////////////////
    // for (;;) {
    //     printf("%4d, %4d,  %4d\n", Sampler.Data[0], Sampler.Data[1],
    //            Sampler.Data[2]);

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) {
    // LED_Toggle(&LED1);
}

void vApplicationTickHook() { HAL_IncTick(); }