# STM32 HAL

## LED

* main.c

``` C
LED_t LED = {
    .GPIOxPiny = A1,
    .Mode = LEDMode_PullUp,
};

LED_Init(&LED);
```

## Key

* main.c

``` C
Key_t Key = {
    .GPIOxPiny = C0,
    .Mode = KeyMode_PullDown,
};

Key_Init(&Key);
```

## Serial

### Normal

* main.c

``` C
Serial_t Serial = {
    .USARTx = USART3,
    .TX = PD8,
    .RX = PD9,
    .Baudrate = 115200,
};

Serial_Init(&Serial);
```

### Rx Interrupt

* main.c

``` C
Serial_t Serial = {
    .USARTx = USART1,
    .TX = PA9,
    .RX = PA10,
    .Baudrate = 115200,
    .Default = ENABLE,
    .Interrupt = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
    .PackLength = 2,
};

Serial_Init(&Serial);
```

* interrupt.c

``` C
void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&Serial.Handler);

    if (Serial.RxITSize) {
        Serial_RXITStart(&Serial);

    } else if (Serial.IdleDMA) {
        Serial_IdleDMAStart(&Serial);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBoard.USARTx) {
        if (SerialBoard.PackRecieved == SET) {
            return;
        }

        Serial_Parse(&SerialBoard, SerialBoard.RxBuffer[0]);

    }
}
```

### Idle Interrupt + DMA

* main.c

``` C
Serial_t SerialBoard = {
    .USARTx = USART1,
    .TX = PA9,
    .RX = PA10,
    .Baudrate = 115200,
    .Interrupt = ENABLE,
    .IdleDMA = ENABLE,
    .Priority = 1,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 2,
            .Channel = 4,
        },
    .PackLength = 2,
    .Default = ENABLE,
};

Serial_Init(&Serial);
```

* interrupt.c

``` C
void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&Serial.Handler);

    if (Serial.RxITSize) {
        Serial_RXITStart(&Serial);

    } else if (Serial.IdleDMA) {
        Serial_IdleDMAStart(&Serial);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == SerialBoard.USARTx) {
        for (uint8_t i = 0; i < Size; i++) {
            Serial_Parse(&SerialBoard, SerialBoard.RxBuffer[i]);
        }

    }
}
```

## OLED

### Software I2C

* main.c

``` C
OLED_t OLED = {
    .SCL = PD14,
    .SDA = PD15,
    .I2C = ENABLE,
};

OLED_Init(&OLED);
```

### Hardware I2C

* main.c

``` C
OLED_t OLED = {
    .SCL = PD14,
    .SDA = PD15,
    .I2Cx = I2C1,
};

OLED_Init(&OLED);
```

## Timer

* main.c

``` C
Timer_t Timer = {
    .TIMx = TIM3,
    .ms = 500,
    .Interrupt = ENABLE,
    .Priority = 8,
};

Timer_Init(&Timer);
```

* interrupt.c

``` C
void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &Timer.Handler) {
    }
}
```

## Servo

* main.c

``` C
PWM_t ServoPWM;

Servo_t Servo1 = {
    .TIMx = TIM8,
    .Channel = {1, 2},
    .GPIOxPiny = {C6, C7},
    .PWM = &ServoPWM,
    .PWM_Init = DISABLE,
};

Servo_t Servo2 = {
    .TIMx = TIM8,
    .Channel = {3, 4},
    .GPIOxPiny = {C8, C9},
    .PWM = &ServoPWM,
    .PWM_Init = ENABLE,
};

Servo_Init(&Servo1);
Servo_Init(&Servo2S);
```

## Encoder

* main.c

``` C
Encoder_t Encoder = {
    .TIM = TIM3,
    .Channel = {1, 2},
    .GPIOxPiny = {B4, B5},
};

Encoder_Init(&Encoder);
```

## Motor

* main.c

``` C
PWM_t MotorPWM;

Motor_t MotorLeft = {
    .TIMx = TIM1,
    .Channel = 2,
    .PWM = PE11,
    .IN1 = PE10,
    .IN2 = PE12,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .Invert = ENABLE,
    .PWM_Init = DISABLE,
};

Motor_t MotorRight = {
    .TIMx = TIM1,
    .Channel = 1,
    .PWM = PE9,
    .IN1 = PE8,
    .IN2 = PE7,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .PWM_Init = ENABLE,
};

Motor_Init(&MotorLeft);
Motor_Init(&MotorRight);
```

## DAC

* main.c

``` C
Generator_t Generator = {
    .Data = DAC_Data,
    .Length = DAC_DataLength,
    .DAC =
        {
            .Channel = "1",
            .GPIOxPiny = "A4",
        },
    .DMA =
        {
            .DMAx = DMA1,
            .Channel = 7,
            .Stream = 5,
        },
    .Timer =
        {
            .TIMx = TIM2,
            .Hz = DAC_Frequency,
        },
};

Generator_Init(&Generator);
```

## ADC

* main.c

### Soft + Single Channel

``` C
Sampler_t Sampler = {
    .ADC =
        {
            .ADCx = ADC1,
            .Channel = {3},
            .GPIOxPiny = {A3},
            .Continuous = ENABLE,
        },
};

Sampler_Init(&Sampler);

Sampler_GetValue(&Sampler, 3);
```

### Soft + Multi Channel

``` C
Sampler_t Sampler = {
    .ADC =
        {
            .ADCx = ADC1,
            .Channel = {3, 4, 5},
            .GPIOxPiny = {A3, A4, A5},
        },
};

Sampler_Init(&Sampler);

Sampler_GetValue(&Sampler, 3);
Sampler_GetValue(&Sampler, 4);
Sampler_GetValue(&Sampler, 5);
```

### DMA + TIM + Single Channel

``` C
#define ADC_DataLength 1
uint32_t ADC_Data[ADC_DataLength];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel =
                {
                    3,
                },
            .GPIOxPiny = {A3},
        },
    .DMA =
        {
            .DMAx = DMA2,
            .Channel = 0,
            .Stream = 0,
        },
    .Timer =
        {
            .TIMx = TIM3,
            .Hz = 100,
        },
};

Sampler_Init(&Sampler);

Sampler.Data[0];
```

### DMA + TIM + Multi Channel

``` C
#define ADC_DataLength 3
uint32_t ADC_Data[ADC_DataLength];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel =
                {
                    3,
                    4,
                    5,
                },
            .GPIOxPiny = {A3, A4, A5},
        },
    .DMA =
        {
            .DMAx = DMA2,
            .Channel = 0,
            .Stream = 0,
        },
    .Timer =
        {
            .TIMx = TIM2,
            .Hz = 100,
        },
};

Sampler_Init(&Sampler);

Sampler.Data[0];
Sampler.Data[2];
Sampler.Data[2];
```

## LCD

* main.c

``` C
LCD_t LCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch_t Touch = {
    .Direction = LCD_Vertical,
};

LCD_Init(&LCD);
Touch_Init(&Touch);
```

* msp.c

``` C
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    __HAL_RCC_FSMC_CLK_ENABLE();

    GPIO_t FSMC_D = {
        .GPIOxPiny = "D14 | D15 | D0 | D1 | E7 | E8 | E9 | E10 | E11 | E12 | "
                     "E13 | E14 | E15 | D8 | D9 | D10",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_D);

    GPIO_t FSMC_NOE = {
        .GPIOxPiny = "D4 ",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NOE);

    GPIO_t FSMC_NWE = {
        .GPIOxPiny = "D5",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NWE);

    GPIO_t FSMC_NE1 = {
        .GPIOxPiny = "D7",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NE1);

    GPIO_t FSMC_A18 = {
        .GPIOxPiny = "D13",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_A18);
}
```

## LVGL

* main.c

``` C
TaskHandle_t xLVGLTaskHandle;
void vLVGLTaskCode(void *pvParameters);

lv_init();
lv_port_disp_init();
lv_port_indev_init();

xTaskCreate(vLVGLTaskCode, "vLVGLTask", 1024, NULL, 1, &xLVGLTaskHandle);
```

* task.c

``` C
void vLVGLTaskCode(void *pvParameters) {
    for (;;) {
        lv_timer_t();
        
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vApplicationTickHook() { lv_tick_inc(1); }
```
