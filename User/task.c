#include "main.h"

const float EncoderLeftToPWM = 10000. / 178.;
const float EncoderRightToPWM = 10000. / 187.;
const float EncoderToCM = (M_PI * 4.8) / (4 * 13 * 20.409);
const float CMToEncoder = (4 * 13 * 20.409) / (M_PI * 4.8);

PID_t MotorLeftSpeedPID = {
    .Kp = 5,
    .Ki = 20,
    .IMax = 10000,
};

PID_t MotorRightSpeedPID = {
    .Kp = 5,
    .Ki = 20,
    .IMax = 10000,

};

PID_t GrayPositionPID = {
    .Kp = -2.5,
    .Kd = -0.1,
};

PID_t AnglePID = {
    .Kp = 60,
    .Kd = 6,
};

typedef enum {
    TurnDirection_None,
    TurnDirection_Left,
    TurnDirection_Right,
} TurnDirection_t;
TurnDirection_t TurnDirection;
float TurnBeginAngle, TurnEndAngle;

typedef enum {
    CarStatus_None,
    CarStatus_Stop,
    CarStatus_Advance,
    CarStatus_Cross,
    CarStatus_Trace,
    CarStatus_Angle,
} CarStatus_t;
int32_t CarStatus;

typedef enum {
    TaskOption_None,
    TaskOption_Task1,
} TaskOption_t;
int32_t TaskOption;

FunctionalState OLEDFlushStatus = ENABLE;

int32_t EncoderLeftCounter, EncoderRightCounter;

int32_t BaseSpeed = 1000;
int32_t RoundSpeed = 500;
float TargetAngle;

void CLI_Bind(void);
void TextPage_Bind(void);
void Test_Hnadler(void);

void Stop_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed);
FlagStatus Advance_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                           float NowAngle, float TargetAngle, int32_t NowCount,
                           int32_t TargetCM);
FlagStatus Cross_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                         float NowAngle, float TurnBeginAngle,
                         TurnDirection_t TurnDirection);
ErrorStatus Trace_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                          RoadStatus_t RoadStatus);
FlagStatus Angle_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                         float NowAngle, float _TargetAngle);

void Task1_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed);

void vMainTaskCode(void *pvParameters) {
    CLI_Bind();
    TextPage_Bind();
    Test_Hnadler();

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(10);

    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        ICM42688_AHRS_Update(&ICM42688);
        GWGray_Update(&GWGray);
        CLI_Handler(&CLI);
        EncoderLeftCounter = Encoder_GetCounter(&EncoderLeft);
        EncoderRightCounter = Encoder_GetCounter(&EncoderRight);

        if (ICM42688.CalibrationFinished == RESET) {
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
            continue;
        }

        int32_t AdvanceSpeed = 0, DiffSpeed = 0;
        switch (CarStatus) {
        case CarStatus_None:
        case CarStatus_Stop:
            Stop_Handler(&AdvanceSpeed, &DiffSpeed);
            break;

        case CarStatus_Advance:
            FlagStatus AdvanceStatus = Advance_Handler(
                &AdvanceSpeed, &DiffSpeed, ICM42688.Angles[0], 0,
                (EncoderLeftCounter + EncoderRightCounter) / 2, 60);

            if (AdvanceStatus == SET) {
                CarStatus = CarStatus_Stop;
                Stop_Handler(&AdvanceSpeed, &DiffSpeed);
            }
            break;

        case CarStatus_Cross:
            FlagStatus TurnStatus = Cross_Handler(
                &AdvanceSpeed, &DiffSpeed, ICM42688.Angles[0], NULL, NULL);

            if (TurnStatus == SET) {
                CarStatus = CarStatus_Trace;
                Trace_Handler(&AdvanceSpeed, &DiffSpeed, GWGray.RoadStatus);
            }
            break;

        case CarStatus_Trace:
            ErrorStatus TarceStatus =
                Trace_Handler(&AdvanceSpeed, &DiffSpeed, GWGray.RoadStatus);

            if (TarceStatus == ERROR) {
                if (GWGray.RoadStatus == RoadStatus_DeadEnd) {
                    CarStatus = CarStatus_Stop;
                    Stop_Handler(&AdvanceSpeed, &DiffSpeed);

                } else if (GWGray.RoadStatus == RoadStatus_Cross) {
                    CarStatus = CarStatus_Cross;
                    Cross_Handler(&AdvanceSpeed, &DiffSpeed, ICM42688.Angles[0],
                                  ICM42688.Angles[0], TurnDirection_Right);
                }
            }
            break;

        case CarStatus_Angle:
            FlagStatus AngleStatus = Angle_Handler(
                &AdvanceSpeed, &DiffSpeed, ICM42688.Angles[0], TargetAngle);

            Serial_Printf(&SerialBluetooth, "{Angle}%.2f,%.2f\r\n",
                          ICM42688.Angles[0], TargetAngle);

            if (AngleStatus == SET) {
                CarStatus = CarStatus_Stop;
                Stop_Handler(&AdvanceSpeed, &DiffSpeed);
            }

            break;
        }

        switch (TaskOption) {
        case TaskOption_Task1:
            Task1_Handler(&AdvanceSpeed, &DiffSpeed);
            break;
        }

        int16_t LeftOut = PID_Caculate(
            &MotorLeftSpeedPID,
            AdvanceSpeed + DiffSpeed - EncoderLeftCounter * EncoderLeftToPWM);
        int16_t RightOut = PID_Caculate(
            &MotorRightSpeedPID,
            AdvanceSpeed - DiffSpeed - EncoderRightCounter * EncoderRightToPWM);

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

void Task1_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed) {
    static uint8_t TaskStep = 1;
    static FlagStatus AdvanceStatus = RESET;
    static FlagStatus AngleStatus = RESET;

    int32_t AdvancetCM = 60;

    switch (TaskStep) {
    case 1:
        AngleStatus =
            Angle_Handler(AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], 45);

        if (AngleStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 2:
        AdvanceStatus = Advance_Handler(
            AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], 45,
            (EncoderLeftCounter + EncoderRightCounter) / 2, AdvancetCM);

        if (AdvanceStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 3:
        AngleStatus =
            Angle_Handler(AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], -90);

        if (AngleStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 4:
        AdvanceStatus = Advance_Handler(
            AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], -90,
            (EncoderLeftCounter + EncoderRightCounter) / 2, AdvancetCM);

        if (AdvanceStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 5:
        AngleStatus =
            Angle_Handler(AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], 135);

        if (AngleStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 6:
        AdvanceStatus = Advance_Handler(
            AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], 135,
            (EncoderLeftCounter + EncoderRightCounter) / 2, AdvancetCM);

        if (AdvanceStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 7:
        AngleStatus =
            Angle_Handler(AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], -90);

        if (AngleStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 8:
        AdvanceStatus = Advance_Handler(
            AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], -90,
            (EncoderLeftCounter + EncoderRightCounter) / 2, AdvancetCM);

        if (AdvanceStatus == SET) {
            TaskStep += 1;
        }
        break;

    case 9:
        AngleStatus =
            Angle_Handler(AdvanceSpeed, DiffSpeed, ICM42688.Angles[0], 0);

        if (AngleStatus == SET) {
            TaskStep = 1;
        }
        break;
    }
}

void Stop_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed) {
    *AdvanceSpeed = 0;
    *DiffSpeed = 0;
}

FlagStatus Advance_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                           float NowAngle, float TargetAngle, int32_t NowCount,
                           int32_t TargetCM) {
    static int32_t TotalCount = 0;
    static int32_t TargetCount = 0;
    static FlagStatus FirstHandle = SET;

    if (FirstHandle == SET) {
        TotalCount = 0;
        TargetCount = TargetCM * CMToEncoder;

        FirstHandle = RESET;
    } else {
        TotalCount += NowCount;
    }

    if (TotalCount < TargetCount) {
        float AngelError = TargetAngle - NowAngle;
        if (AngelError > 180.0f) {
            AngelError -= 360.0f;
        } else if (AngelError < -180.0f) {
            AngelError += 360.0f;
        }

        if (TargetCount > 0) {
            *AdvanceSpeed = BaseSpeed;
        } else {
            *AdvanceSpeed = -BaseSpeed;
        }
        *DiffSpeed = PID_Caculate(&AnglePID, AngelError);

        return RESET;

    } else {
        FirstHandle = SET;

        *AdvanceSpeed = 0;
        *DiffSpeed = 0;

        return SET;
    }
}

FlagStatus Cross_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                         float NowAngle, float TurnBeginAngle,
                         TurnDirection_t _TurnDirection) {
    static float TurnEndAngle;
    static TurnDirection_t TurnDirection;
    static FlagStatus FirstHandle = SET;

    if (FirstHandle == SET) {
        TurnDirection = _TurnDirection;

        switch (TurnDirection) {
        case TurnDirection_None:
            TurnEndAngle = TurnBeginAngle;
            break;

        case TurnDirection_Left:
            TurnEndAngle = TurnBeginAngle - 90.f;
            break;

        case TurnDirection_Right:
            TurnEndAngle = TurnBeginAngle + 90.f;
            break;
        }

        TurnEndAngle = TurnEndAngle - 360.0f * floor(TurnEndAngle / 360.0f);
        if (TurnEndAngle > 180.0f) {
            TurnEndAngle -= 360.0f;
        }

        FirstHandle = RESET;

        *AdvanceSpeed = 0;
        *DiffSpeed = 0;

        return RESET;
    }

    if (fabs(NowAngle - TurnEndAngle) > 4.0f) {
        switch (TurnDirection) {
        case TurnDirection_None:
            *AdvanceSpeed = 0;
            *DiffSpeed = 0;
            break;

        case TurnDirection_Left:
            *AdvanceSpeed = BaseSpeed;
            *DiffSpeed = -RoundSpeed;
            break;

        case TurnDirection_Right:
            *AdvanceSpeed = BaseSpeed;
            *DiffSpeed = RoundSpeed;
            break;
        }

        return RESET;

    } else {
        FirstHandle = SET;

        *AdvanceSpeed = 0;
        *DiffSpeed = 0;

        return SET;
    }
}

ErrorStatus Trace_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                          RoadStatus_t RoadStatus) {
    if (RoadStatus == RoadStatus_Straight || RoadStatus == RoadStatus_OnRoad) {
        *AdvanceSpeed = BaseSpeed;
        *DiffSpeed = PID_Caculate(&GrayPositionPID, GWGray.GrayError);

        return SUCCESS;

    } else {
        *AdvanceSpeed = 0;
        *DiffSpeed = 0;

        return ERROR;
    }
}

FlagStatus Angle_Handler(int32_t *AdvanceSpeed, int32_t *DiffSpeed,
                         float NowAngle, float _TargetAngle) {
    static float TargetAngle;
    static TickType_t OnAngleTime = 0;
    static FlagStatus FirstHandle = SET;

    if (FirstHandle == SET) {
        TargetAngle = _TargetAngle;

        FirstHandle = RESET;

        *AdvanceSpeed = 0;
        *DiffSpeed = 0;

        return RESET;
    }

    float AngelError = TargetAngle - NowAngle;
    if (AngelError > 180.0f) {
        AngelError -= 360.0f;
    } else if (AngelError < -180.0f) {
        AngelError += 360.0f;
    }

    if (fabs(AngelError) < 0.5) {
        if (OnAngleTime == 0) {
            OnAngleTime = xTaskGetTickCount();
        }

        if (xTaskGetTickCount() - OnAngleTime > pdMS_TO_TICKS(1000)) {
            FirstHandle = SET;

            *AdvanceSpeed = 0;
            *DiffSpeed = 0;

            return SET;

        } else {
            *AdvanceSpeed = 0;
            *DiffSpeed = PID_Caculate(&AnglePID, AngelError);
        }

    } else {
        OnAngleTime = 0;

        *AdvanceSpeed = 0;
        *DiffSpeed = PID_Caculate(&AnglePID, AngelError);

        return RESET;
    }

    return RESET;
}

void CLI_Bind() {
    CLI.Datas[0].IntDataPtr = &TaskOption;
    CLI.Datas[1].IntDataPtr = &CarStatus;
    CLI.Datas[2].IntDataPtr = &BaseSpeed;
    CLI.Datas[3].IntDataPtr = &RoundSpeed;
    CLI.Datas[4].FloatDataPtr = &TargetAngle;
    CLI.Datas[5].FloatDataPtr = &AnglePID.Kp;
    CLI.Datas[6].FloatDataPtr = &AnglePID.Ki;
    CLI.Datas[7].FloatDataPtr = &AnglePID.Kd;
}

void TextPage_Bind() {
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
    TextPage_t *AnglePIDAdjustPage = &ParameterPage.LowerPages[2].LowerPages[4];
    TextPage_t *OptionPage = &ParameterPage.LowerPages[4];

    ICM42688MonitorPage->LowerPages[1].FloatParameterPtr = &ICM42688.Angles[0];
    ICM42688MonitorPage->LowerPages[2].FloatParameterPtr = &ICM42688.Angles[1];
    ICM42688MonitorPage->LowerPages[3].FloatParameterPtr = &ICM42688.Angles[2];

    GWGrayMonitorPage->LowerPages[1].IntParameterPtr = &GWGray.GrayError;

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

    AnglePIDAdjustPage->LowerPages[1].FloatParameterPtr = &AnglePID.Kp;
    AnglePIDAdjustPage->LowerPages[2].FloatParameterPtr = &AnglePID.Ki;
    AnglePIDAdjustPage->LowerPages[3].FloatParameterPtr = &AnglePID.Kd;

    OptionPage->IntParameterPtr = &CarStatus;
}

void Test_Hnadler() {
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

    // ---------------- Serial Pack Test -------------------- //
    // for (;;) {
    //     if (SerialBoard.PackRecieved == SET) {
    //         switch (SerialBoard.PackType) {
    //         case SerialPack_Int8:
    //         case SerialPack_Int16:
    //         case SerialPack_Int32:
    //         case SerialPack_Uint8:
    //         case SerialPack_Uint16:
    //         case SerialPack_Uint32:
    //         case SerialPack_Float32:
    //             Serial_Printf(&SerialBoard, "Receive HexPack [");
    //             for (uint8_t i = 0; i < SerialBoard.ParsedCount; i++) {
    //                 Serial_Printf(&SerialBoard, i == 0 ? "%#X" : " %#X",
    //                               SerialBoard.Pack[i]);
    //             }
    //             Serial_Printf(&SerialBoard, "]\n");
    //             break;
    //         case SerialPack_String:
    //             Serial_Printf(&SerialBoard, "Receive StringPack [%s]\n",
    //                           SerialBoard.Pack);
    //             break;
    //         }
    //         Serial_Clear(&SerialBoard);
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(1));
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
    //     // printf("{ICM42688}%6.2f,%6.2f,%6.2f\n", YawPitchRoll[0],
    //     //        YawPitchRoll[1], YawPitchRoll[2]);
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
    // Motor_SetSpeed(&MotorLeft, BaseSpeed);
    // for (;;) {
    //     CLI_Handler(&CLI);
    //     EncoderLeftCounter = Encoder_GetCounter(&EncoderLeft);
    //     float Error = BaseSpeed - EncoderLeftCounter * EncoderLeftToPWM;
    //     int32_t Out = (int32_t)PID_Caculate(&MotorLeftSpeedPID, Error);
    //     Motor_SetSpeed(&MotorLeft, Out);
    //     Serial_Printf(&SerialBoard, "{MotorSpeedPID}%d, %f\r\n", BaseSpeed,
    //                   EncoderLeftCounter * EncoderLeftToPWM);
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
}