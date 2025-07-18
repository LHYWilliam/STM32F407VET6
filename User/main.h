#ifndef MIAN_H
#define MIAN_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include <math.h>
#include <stdio.h>

#include "CLI.h"
#include "Encoder.h"
#include "GWGray.h"
#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "Key.h"
#include "LED.h"
#include "Menu.h"
#include "Motor.h"
#include "OLED.h"
#include "PID.h"
#include "PWM.h"
#include "SEGGER_RTT.h"
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

extern PWM_t MotorPWM;
extern Motor_t MotorLeft;
extern Motor_t MotorRight;

extern Encoder_t EncoderLeft;
extern Encoder_t EncoderRight;

extern PWM_t ServoPWM;
extern Servo_t Servo1;
extern Servo_t Servo2;

extern uint32_t ADC_Data[];
extern Sampler_t Sampler;

extern OLED_t OLED;
extern GWGray_t GWGray;
extern ICM42688_t ICM42688;

extern CLI_t CLI;

extern TaskHandle_t xMainTaskHandle;
extern void vMainTaskCode(void *pvParameters);

extern TaskHandle_t xInteractionTaskHandle;
extern void vInteractionTaskCode(void *pvParameters);

extern TaskHandle_t xOLEDTaskHandle;
extern void vOLEDTaskCode(void *pvParameters);

extern TimerHandle_t xLEDTimer;
extern void vLEDTimerCallback(TimerHandle_t pxTimer);

extern void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                               uint16_t PLLQ);

extern TextMenu_t TextMenu;
extern TextPage_t ParameterPage;
extern SelectioneBar_t Bar;

extern void TextPage_BackCallback(TextPage_t **TextPage);
extern void TextPage_EnterCallback(TextPage_t **TextPage);
extern void TextPage_ParameterDoAdjustCallback(TextPage_t **TextPage);
extern void TextPage_ParameterMultiplyCallback(TextPage_t **TextPage);
extern void TextPage_ParameterDivideCallback(TextPage_t **TextPage);
extern void TextPage_ParameterPlusMinusCallback(TextPage_t **TextPage);
extern void TextPage_ChooseOptionCallback(TextPage_t **TextPage);

extern void TextPage_CursorCallback(TextPage_t *TextPage,
                                    TextPageRotation Direction);

extern void TextPage_ShowCallback(TextPage_t *TextPage);
extern void TextPage_ShowParameterCallback(TextPage_t *TextPage);
extern void TextPage_ShowDialogCallback(TextPage_t *TextPage);
extern void TextPage_ShowOptionCallback(TextPage_t *TextPage);

extern void TextPage_UpdateCallback(TextPage_t *TextPage);
extern void TextPage_UpdateDialogCallback(TextPage_t *TextPage);

#endif