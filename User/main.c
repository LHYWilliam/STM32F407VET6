#include "main.h"

extern int32_t GWGrayPositionError;
extern int32_t EncoderLeftCounter, EncoderRightCounter;

int main() {
    HAL_Init();
    // SystemClock_Config(25, 336, 2, 4); // 25MHz HSE
    SystemClock_Config(4, 168, 2, 4); // 8MHz HSE

    LED_Init(&LEDRed);
    LED_Init(&LEDGreen);
    LED_Init(&LEDBlue);

    Key_Init(&Key1);
    Key_Init(&Key2);
    Key_Init(&Key3);
    Key_Init(&Key4);

    Serial_Init(&SerialBoard);
    Serial_Init(&SerialBluetooth);
    Serial_Init(&SerialK230);
    Serial_Init(&SerialJY61P);

    Motor_Init(&MotorLeft);
    Motor_Init(&MotorRight);
    Motor_SetSpeed(&MotorLeft, 0);
    Motor_SetSpeed(&MotorRight, 0);

    Encoder_Init(&EncoderLeft);
    Encoder_Init(&EncoderRight);

    Servo_Init(&Servo1);
    Servo_Init(&Servo2);

    Sampler_Init(&Sampler);

    OLED_Init(&OLED);
    OLED_SetFont(&OLED, OLEDFont_6X8);

    GWGray_Init(&GWGray);
    ICM42688_Init(&ICM42688);

    TextPage_Init(&ParameterPage, &OLED);
    TextMenu.Page = &ParameterPage;
    SelectioneBar_BindTextPage(&Bar, &ParameterPage.LowerPages[0]);

    xTaskCreate(vMainTaskCode, "vMainTask", 128, NULL, 3, &xMainTaskHandle);
    xTaskCreate(vInteractionTaskCode, "vInteractionTask", 128, NULL, 1,
                &xInteractionTaskHandle);
    xTaskCreate(vOLEDTaskCode, "vOLEDTask", 128, NULL, 2, &xOLEDTaskHandle);

    xLEDTimer = xTimerCreate("xLEDTimer", pdMS_TO_TICKS(200), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    xTimerStart(xLEDTimer, 0);

    vTaskStartScheduler();
}

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}