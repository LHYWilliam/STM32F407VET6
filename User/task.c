#include "main.h"

const float EncoderLeftToPWM = 10000. / 183.;
const float EncoderRightToPWM = 10000. / 194.;

PID_t MotorLeftSpeedPID = {
    .Kp = 2,
    .Ki = 12,
    .IMax = 10000,
};

// .Kd = 0.001,

PID_t MotorRightSpeedPID = {
    .Kp = 2,
    .Ki = 12,
    .IMax = 10000,

};

PID_t GrayPositionPID = {
    .Kp = -6,
    .Ki = -1,
    .IMax = 512,
};

int32_t DiffSpeed;
int32_t EncoderLeftCounter, EncoderRightCounter;

typedef enum {
    None,
    Stop,
    Advance,
    Round,
    Trace,
} CarStatus_t;

int32_t CarStatus;
FunctionalState OLEDFlushStatus = ENABLE;

int32_t AdvanceSpeed = 1024;
int32_t RoundSpeed = 512;

void vMainTaskCode(void *pvParameters) {
    TextPage_t *ICM42688MonitorPage =
        &ParameterPage.LowerPages[1].LowerPages[1];
    TextPage_t *GWGrayMonitorPage = &ParameterPage.LowerPages[1].LowerPages[2];
    TextPage_t *EncoderMonitorPage = &ParameterPage.LowerPages[1].LowerPages[3];
    TextPage_t *MotorLeftSpeedPIDAdjustPage =
        &ParameterPage.LowerPages[2].LowerPages[1];
    TextPage_t *MotorRightSpeedPIDAdjustPage =
        &ParameterPage.LowerPages[2].LowerPages[2];
    TextPage_t *GWGrayPositionPIDAdjustPage =
        &ParameterPage.LowerPages[2].LowerPages[3];
    TextPage_t *OptionPage = &ParameterPage.LowerPages[4];

    ICM42688MonitorPage->LowerPages[1].FloatParameterPtr = &ICM42688.Angles[0];
    ICM42688MonitorPage->LowerPages[2].FloatParameterPtr = &ICM42688.Angles[1];
    ICM42688MonitorPage->LowerPages[3].FloatParameterPtr = &ICM42688.Angles[2];

    GWGrayMonitorPage->LowerPages[1].IntParameterPtr = &DiffSpeed;

    EncoderMonitorPage->LowerPages[1].IntParameterPtr = &EncoderLeftCounter;
    EncoderMonitorPage->LowerPages[2].IntParameterPtr = &EncoderRightCounter;

    MotorLeftSpeedPIDAdjustPage->LowerPages[1].FloatParameterPtr =
        &MotorLeftSpeedPID.Kp;
    MotorLeftSpeedPIDAdjustPage->LowerPages[2].FloatParameterPtr =
        &MotorLeftSpeedPID.Ki;
    MotorLeftSpeedPIDAdjustPage->LowerPages[3].FloatParameterPtr =
        &MotorLeftSpeedPID.Kd;

    MotorRightSpeedPIDAdjustPage->LowerPages[1].FloatParameterPtr =
        &MotorRightSpeedPID.Kp;
    MotorRightSpeedPIDAdjustPage->LowerPages[2].FloatParameterPtr =
        &MotorRightSpeedPID.Ki;
    MotorRightSpeedPIDAdjustPage->LowerPages[3].FloatParameterPtr =
        &MotorRightSpeedPID.Kd;

    GWGrayPositionPIDAdjustPage->LowerPages[1].FloatParameterPtr =
        &GrayPositionPID.Kp;
    GWGrayPositionPIDAdjustPage->LowerPages[2].FloatParameterPtr =
        &GrayPositionPID.Ki;
    GWGrayPositionPIDAdjustPage->LowerPages[3].FloatParameterPtr =
        &GrayPositionPID.Kd;

    OptionPage->IntParameterPtr = &CarStatus;

    // ---------------- Trace Line Test ---------------- //
    // int16_t BaseSpeed = 500;
    // while (!Key_IsPressing(&Key1))
    //     ;
    // for (;;) {
    //     int16_t CounterLeft = Encoder_GetCounter(&EncoderLeft);
    //     int16_t CounterRight = Encoder_GetCounter(&EncoderRight);
    //     int16_t GrayError = GWGray_CaculateAnalogError(&GWGray);
    //     int16_t GrayDiffSpeed = PID_Caculate(&GrayPID, GrayError);
    //     int16_t LeftOut = PID_Caculate(&MotorLeftSpeedPID,
    //                                    BaseSpeed + GrayDiffSpeed -
    //                                        CounterLeft * EncoderLeftToPWM);
    //     int16_t RightOut = PID_Caculate(&MotorRightSpeedPID,
    //                                     BaseSpeed - GrayDiffSpeed -
    //                                         CounterRight *
    //                                         EncoderRightToPWM);
    //     Motor_SetSpeed(&MotorLeft, LeftOut);
    //     Motor_SetSpeed(&MotorRight, RightOut);
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ---------------- Serial Test -------------------- //
    // for (;;) {
    //     Serial_Printf(&SerialBoard, "Hello from SerialBoard\n");
    //     Serial_Printf(&SerialBluetooth, "Hello from SerialBluetooth\n");
    //     Serial_Printf(&SerialK230, "Hello from SerialK230\n");
    //     Serial_Printf(&SerialJY61P, "Hello from SerialJY61P\n");
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ---------------- Key Test ----------------------- //
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

    // ---------------- LED Test ----------------------- //
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

    // ---------------- Motor Test --------------------- //
    // while (!Key_IsPressing(&Key1))
    //     ;
    // Motor_SetSpeed(&MotorLeft, 1000);
    // Motor_SetSpeed(&MotorRight, -1000);

    // ---------------- Encoder Test ------------------- //
    // while (!Key_IsPressing(&Key1))
    //     ;
    // Motor_SetSpeed(&MotorLeft, 10000);
    // Motor_SetSpeed(&MotorRight, 10000);
    // for (;;) {
    //     int16_t CountLeft = Encoder_GetCounter(&EncoderLeft);
    //     int16_t CountRight = Encoder_GetCounter(&EncoderRight);
    //     printf("CountLeft, CountRight: %d, %d\n", CountLeft, CountRight);
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ---------------- Servo Test --------------------- //
    // while (!Key_IsPressing(&Key1))
    //     ;
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

    // ---------------- ICM42688 Test ------------------ //
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

    // ---------------- Gray PID Test ------------------ //
    // for (;;) {
    //     int16_t Error = GWGray_CaculateAnalogError(&GWGray);
    //     printf("%d\n", Error);
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ---------------- MotorSpeedPID Test ------------- //
    // while (!Key_IsPressing(&Key1))
    //     ;
    // int16_t TargetSpeed = 0;
    // Motor_SetSpeed(&MotorLeft, TargetSpeed);
    // for (;;) {
    //     int16_t Count = Encoder_GetCounter(&EncoderLeft);
    //     float Error = TargetSpeed - Count * EncoderLeftToPWM;
    //     int16_t Out = PID_Caculate(&MotorLeftSpeedPID, Error);
    //     Motor_SetSpeed(&MotorLeft, Out);
    //     printf("Target, Real, Error, Out: %d, %d, %d, %d\n", TargetSpeed,
    //            (int16_t)(Count * EncoderLeftToPWM), (int16_t)Error, Out);
    //     if (SerialBoard.RecieveFlag == SET) {
    //         TargetSpeed = SerialBoard.HexData[0] << 8 |
    //         SerialBoard.HexData[1];
    //         Serial_Clear(&SerialBoard);
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // ---------------- K230 Test ---------------------- //
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

    // ---------------- Sampler Test ------------------- //
    // for (;;) {
    //     printf("%4d, %4d,  %4d\n", Sampler.Data[0], Sampler.Data[1],
    //            Sampler.Data[2]);
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    // ---------------- OLED Test ---------------------- //
    // for (;;) {
    //     OLED_Reverse(&OLED);
    //     OLED_SendBuffer(&OLED);
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(10);

    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        ICM42688_AHRS_Update(&ICM42688);

        if (ICM42688.CalibrationFinished == RESET) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        EncoderLeftCounter = Encoder_GetCounter(&EncoderLeft);
        EncoderRightCounter = Encoder_GetCounter(&EncoderRight);

        // if (SerialBoard.PackRecieved == SET) {
        //     AdvanceSpeed = SerialBoard.HexPack[0] << 8 |
        //     SerialBoard.HexPack[1]; Serial_Clear(&SerialBoard);
        // }

        // if (SerialBluetooth.PackRecieved == SET) {
        //     AdvanceSpeed =
        //         SerialBluetooth.HexPack[0] << 8 | SerialBluetooth.HexPack[1];
        //     Serial_Clear(&SerialBluetooth);
        // }

        int16_t BaseSpeed = 0;
        switch (CarStatus) {
        case Stop:
            BaseSpeed = 0;
            DiffSpeed = 0;
            break;

        case Advance:
            BaseSpeed = AdvanceSpeed;
            DiffSpeed = 0;
            break;

        case Round:
            BaseSpeed = 0;
            DiffSpeed = RoundSpeed;
            break;

        case Trace:
            DiffSpeed = GWGray_CaculateAnalogError(&GWGray);

            if (DiffSpeed == 0xFFFF) {
                BaseSpeed = 0;
                DiffSpeed = 0;

                CarStatus = Stop;
            } else {
                BaseSpeed = AdvanceSpeed;
                DiffSpeed = PID_Caculate(&GrayPositionPID, DiffSpeed);
            }
            break;
        }

        int16_t LeftOut = PID_Caculate(
            &MotorLeftSpeedPID,
            BaseSpeed + DiffSpeed - EncoderLeftCounter * EncoderLeftToPWM);
        int16_t RightOut = PID_Caculate(
            &MotorRightSpeedPID,
            BaseSpeed - DiffSpeed - EncoderRightCounter * EncoderRightToPWM);
        Motor_SetSpeed(&MotorLeft, LeftOut);
        Motor_SetSpeed(&MotorRight, RightOut);

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vInteractionTaskCode(void *pvParameters) {
    for (;;) {
        LED_Toggle(&LEDBlue);

        if (Key_IsPressed(&Key4)) {
            OLEDFlushStatus = !OLEDFlushStatus;
        }

        if (OLEDFlushStatus == ENABLE) {
            if (Key_IsPressed(&Key1)) {
                TextMenu.Page->LowerPages[TextMenu.Page->Cursor].ClickCallback(
                    &TextMenu.Page);
            }
            if (Key_IsPressed(&Key2)) {
                TextMenu.Page->LowerPages[TextMenu.Page->Cursor]
                    .RotationCallback(TextMenu.Page, RotationUp);

            } else if (Key_IsPressed(&Key3)) {
                TextMenu.Page->LowerPages[TextMenu.Page->Cursor]
                    .RotationCallback(TextMenu.Page, RotationDown);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vOLEDTaskCode(void *pvParameters) {
    for (;;) {
        if (OLEDFlushStatus == DISABLE) {
            OLED_ClearBuffer(&OLED);
            OLED_Printf(&OLED, OLED.Width / 2 - OLED.FontWidth * 6,
                        OLED.Height / 2 - OLED.FontHeight / 2, "Flush Disable");
            OLED_SendBuffer(&OLED);

            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        LED_Toggle(&LEDGreen);

        OLED_ClearBuffer(&OLED);

        TextMenu.Page->UpdateCallback(TextMenu.Page);
        SelectioneBar_Update(&Bar);

        TextMenu.Page->ShowCallback(TextMenu.Page);
        OLED_ShowSelectioneBar(&OLED, &Bar);

        OLED_SendBuffer(&OLED);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vLEDTimerCallback(TimerHandle_t pxTimer) { LED_Toggle(&LEDRed); }

void vApplicationTickHook() { HAL_IncTick(); }