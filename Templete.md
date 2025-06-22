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
Serial_t Serial = {
    .USART = USART1,
    .RX = A10,
    .TX = A9,
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
    .Priority = 8,
};

Serial_Init(&Serial);
```

* msp.c
```
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial.USART);

        GPIO_t RTX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        if (*Serial.RX) {
            GPIO_InitPin(&RTX, Serial.RX);
        }
        if (*Serial.TX) {
            GPIO_InitPin(&RTX, Serial.TX);
        }

        if (Serial.RxIT) {
            HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial.USART));
            HAL_NVIC_SetPriority(USARTx_IRQn(Serial.USART), Serial.Priority, 0);
        }
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
Servo_t Servo = {
    .PWM =
        {
            .TIM = TIM2,
            .Channel = {2, 3},
            .GPIOxPiny = {A1, A2},
        },
};

Servo_Init(&Servo);
```

* msp.c
```
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Servo.PWM.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(Servo.PWM.TIM);

        GPIO_t GPIO = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(Servo.PWM.TIM),
        };

        for (uint8_t i = 0; Servo.PWM.Channel[i]; i++) {
            GPIO_InitPin(&GPIO, Servo.PWM.GPIOxPiny[i]);
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
    .GPIOxPiny = {A6, A7},
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
            .Pull = GPIO_NOPULL,
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
```
Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel = "5",
            .GPIOxPiny = "A5",
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
            .Hz = ADC_Frequency,
        },
};

Sampler_Init(&Sampler);
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