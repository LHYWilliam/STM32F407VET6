# LED0
* main.c
```
LED_t LED0 = {
    .GPIOxPiny = A1,
    .Mode = LEDMode_PullUp,
};

LED_Init(&LED0);
```


# LED1
* main.c
```
LED_t LED1 = {
    .GPIOxPiny = A2,
    .Mode = LEDMode_PullUp,
};

LED_Init(&LED1);
```


# Key0
* main.c
```
Key_t Key0 = {
    .GPIOxPiny = C0,
    .Mode = KeyMode_PullDown,
};

Key_Init(&Key0);
```


# Key1
* main.c
```
Key_t Key1 = {
    .GPIOxPiny = A0,
    .Mode = KeyMode_PullDown,
};

Key_Init(&Key1);
```


# Serial
* main.c
```
Serial_t SerialBoard = {
    .USART = USART1,
    .TX = PA9,
    .RX = PA10,
    .Baudrate = 115200,
    .Default = ENABLE,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
};

Serial_Init(&SerialBoard);
```

* msp.c
```
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == SerialBoard.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(SerialBoard.USART);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_USARTy(SerialBoard.USART),
        };
        if (*SerialBoard.TX) {
            GPIO_InitPin(&GPIO, SerialBoard.TX);
        }
        if (*SerialBoard.RX) {
            GPIO_InitPin(&GPIO, SerialBoard.RX);
        }

        if (SerialBoard.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(SerialBoard.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(SerialBoard.USART),
                                SerialBoard.Priority, 0);
    }
}
```

* interrupt.c
```
void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&Serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        Serial_SendBytes(&Serial, Serial.RXBuffer, Serial.RxITSize);

        Serial_RXITStart(&Serial, 1);
    }
}
```


# OLED
* main.c
```
OLED_t OLED = {
    .SCL = "xx",
    .SDA = "xx",
};

OLED_Init(&OLED);
```


# Timer
* main.c
```
Timer_t Timer = {
    .TIMx = TIM3,
    .ms = 500,
    .Interrupt = ENABLE,
    .Priority = 8,
};

Timer_Init(&Timer);
```

* msp.c
```
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Timer.TIMx);

        if (Timer.Interrupt) {
            HAL_NVIC_SetPriority(TIMx_IRQN(Timer.TIMx), Timer.Priority, 0);
            HAL_NVIC_EnableIRQ(TIMx_IRQN(Timer.TIMx));
        }
    }
}
```

* interrupt.c
```
void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &Timer.Handler) {
    }
}
```


# Servo
* main.c
```
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

* msp.c
```
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == ServoPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(ServoPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(ServoPWM.TIMx),
        };

        for (uint8_t i = 0; ServoPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, ServoPWM.GPIOxPiny[i]);
        }
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == ServoPWM.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(ServoPWM.TIMx);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(ServoPWM.TIMx),
        };

        for (uint8_t i = 0; ServoPWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, ServoPWM.GPIOxPiny[i]);
        }
    }
}
```


# Encoder
* main.c
```
Encoder_t Encoder = {
    .TIM = TIM3,
    .Channel = {1, 2},
    .GPIOxPiny = {B4, B5},
};

Encoder_Init(&Encoder);
```

* msp.c
```
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Encoder.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Alternate = GPIO_AFx_TIMy(Encoder.TIM),
        };
        GPIO_InitPin(&GPIO, Encoder.GPIOxPiny[0]);
        GPIO_InitPin(&GPIO, Encoder.GPIOxPiny[1]);
    }
}
```


# DAC
* main.c
```
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

* msp.c
```
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == Generator.DAC.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_t gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, Generator.DAC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (Generator.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Generator.DMA.DMAx);

            __HAL_LINKDMA(&Generator.DAC.Handler, DMA_Handle1,
                          Generator.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Generator.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Generator.Timer.TIMx);

    }
}
```


# ADC
* main.c

### Soft + Single Channel
```
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
```
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
```
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
```
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

* msp.c
```
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == Sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(Sampler.ADC.ADCx);

        GPIO_t gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, Sampler.ADC.GPIOxPiny);
        GPIO_Init(&gpio);

        if (Sampler.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(Sampler.DMA.DMAx);

            __HAL_LINKDMA(&Sampler.ADC.Handler, DMA_Handle,
                          Sampler.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Sampler.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(Sampler.Timer.TIMx);
    }
}
```


# LCD
* main.c
```
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
```
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


# LVGL
* main.c
```
TaskHandle_t xLVGLTaskHandle;
void vLVGLTaskCode(void *pvParameters);

lv_init();
lv_port_disp_init();
lv_port_indev_init();

xTaskCreate(vLVGLTaskCode, "vLVGLTask", 1024, NULL, 1, &xLVGLTaskHandle);
```

* task.c
```
void vLVGLTaskCode(void *pvParameters) {
    for (;;) {
        lv_timer_t();
        
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vApplicationTickHook() { lv_tick_inc(1); }
```