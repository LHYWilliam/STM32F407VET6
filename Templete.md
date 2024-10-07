# LED0
* main.c
```
LED_Handler LED0 = {
    .GPIOxPiny = "A1",
};

LED_Init(&LED0);
```


# LED1
* main.c
```
LED_Handler LED1 = {
    .GPIOxPiny = "A2",
};

LED_Init(&LED1);
```


# Key0
* main.c
```
Key_Handler Key0 = {
    .GPIOxPiny = "C0",
};

Key_Init(&Key0);
```


# Key1
* main.c
```
Key_Handler Key1 = {
    .GPIOxPiny = "A0",
};

Key_Init(&Key1);
```


# Serial
* main.c
```
Serial_Handler Serial = {
    .USART = USART1,
    .TX = "A9",
    .RX = "A10",
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
};

Serial_Init(&Serial);
```

* msp.c
```
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(Serial.USART);

        GPIO_Handler RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        strcpy(RX.GPIOxPiny, Serial.RX);
        GPIO_Init(&RX);

        GPIO_Handler TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(Serial.USART),
        };
        strcpy(TX.GPIOxPiny, Serial.TX);
        GPIO_Init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(Serial.USART));
        HAL_NVIC_SetPriority(USARTx_IRQn(Serial.USART), 4, 0);
    }
}
```

* interrupt.c
```
void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&Serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == Serial.USART) {
    }

    Serial_RXITStart(&Serial, 1);
}
```


# OLED
* main.c
```
OLED_Handler OLED = {
    .SCL = "xx",
    .SDA = "xx",
};

OLED_Init(&OLED);
```


# Timer
* main.c
```
Timer_Handler Timer = {
    .TIM = TIMx,
    .ms = x,
    .interrupt = ENABLE,
};

Timer_Init(&Timer);
```

* msp.c
```
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Timer.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(Timer.TIM);

        HAL_NVIC_SetPriority(TIMx_IRQN(Timer.TIM), x, 0);
        HAL_NVIC_EnableIRQ(TIMx_IRQN(Timer.TIM));
    }
}
```

* interrupt.c
```
void TIMx_IRQHandler(void) { HAL_TIM_IRQHandler(&Timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &Timer.Handler) {
    }
}
```


# PWM
* main.c
```
PWM_Handler PWM = {
    .TIM = TIMx,
    .Channel = "x",
    .Prescaler = x - 1,
    .Period = x - 1,
    .GPIOxPiny = "xx",
};

PWM_Init(&PWM);
```

* msp.c
```
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == PWM.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(PWM.TIM);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(PWM.TIM),
        };
        strcpy(gpio.GPIOxPiny, PWM.GPIOxPiny);
        GPIO_Init(&gpio);
    }
}
```


# Encoder
* main.c
```
Encoder_Handler Encoder = {
    .TIM = TIMx,
    .Channel = "x | x",
    .GPIOxPiny = "xx | xx",
};

Encoder_Init(&Encoder);
```

* msp.c
```
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == Encoder.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(Encoder.TIM),
        };
        strcpy(gpio.GPIOxPiny, Encoder.GPIOxPiny);
        GPIO_Init(&gpio);
    }
}
```


# DAC
* main.c
```
SignalGenerator_Handler Generator = {
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

SignalGenerator_Init(&Generator);
```

* msp.c
```
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == Generator.DAC.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_Handler gpio = {
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
SignalSampler_Handler Sampler = {
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

SignalSampler_Init(&Sampler);
```

* msp.c
```
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == Sampler.ADC.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(Sampler.ADC.ADCx);

        GPIO_Handler gpio = {
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
LCD_Handler LCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch_Handler Touch = {
    .Direction = LCD_Vertical,
};

LCD_Init(&LCD);
Touch_Init(&Touch);
```

* msp.c
```
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    __HAL_RCC_FSMC_CLK_ENABLE();

    GPIO_Handler FSMC_D = {
        .GPIOxPiny = "D14 | D15 | D0 | D1 | E7 | E8 | E9 | E10 | E11 | E12 | "
                     "E13 | E14 | E15 | D8 | D9 | D10",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_D);

    GPIO_Handler FSMC_NOE = {
        .GPIOxPiny = "D4 ",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NOE);

    GPIO_Handler FSMC_NWE = {
        .GPIOxPiny = "D5",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NWE);

    GPIO_Handler FSMC_NE1 = {
        .GPIOxPiny = "D7",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NE1);

    GPIO_Handler FSMC_A18 = {
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
        lv_timer_handler();
        
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vApplicationTickHook() { lv_tick_inc(1); }
```