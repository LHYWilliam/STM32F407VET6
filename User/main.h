#ifndef MIAN_H
#define MIAN_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "Encoder.h"
#include "GWGray.h"
#include "ICM42688.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h"
#include "PID.h"
#include "PWM.h"
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

extern TaskHandle_t xMainTaskHandle;
extern void vMainTaskCode(void *pvParameters);

extern TimerHandle_t xLEDTimer;
extern void vLEDTimerCallback(TimerHandle_t pxTimer);

extern void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                               uint16_t PLLQ);

#endif