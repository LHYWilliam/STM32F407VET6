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
Key_Handler key0 = {
    .GPIOxPiny = "C0",
};

Key_Init(&key);
```


# Key1
* main.c
```
Key_Handler key1 = {
    .GPIOxPiny = "A0",
};

Key_Init(&key);
```


# Serial
* main.c
```
Serial_Handler serial = {
    .USART = USART1,
    .TX = "A9",
    .RX = "A10",
    .Baudrate = 115200,
    .RxIT = ENABLE,
    .RxITSize = 1,
};

Serial_Init(&serial);
```

* msp.c
```
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
        __HAL_RCC_USARTx_CLK_ENABLE(serial.USART);

        GPIO_Handler RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(RX.GPIOxPiny, serial.RX);
        GPIO_Init(&RX);

        GPIO_Handler TX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(TX.GPIOxPiny, serial.TX);
        GPIO_Init(&TX);

        HAL_NVIC_EnableIRQ(USARTx_IRQn(serial.USART));
        HAL_NVIC_SetPriority(USARTx_IRQn(serial.USART), 4, 0);
    }
}
```

* interrupt.c
```
void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&serial.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.USART) {
    }

    Serial_RXITStart(&serial, 1);
}
```


# OLED
* main.c
```
OLED_Handler oled = {
    .SCL = "xx",
    .SDA = "xx",
};

OLED_Init(&oled);
```


# Timer
* main.c
```
Timer_Handler timer = {
    .TIM = TIMx,
    .ms = x,
    .interrupt = ENABLE,
};

Timer_Init(&timer);
```

* msp.c
```
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == timer.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(timer.TIM);

        HAL_NVIC_SetPriority(TIMx_IRQN(timer.TIM), x, 0);
        HAL_NVIC_EnableIRQ(TIMx_IRQN(timer.TIM));
    }
}
```

* interrupt.c
```
void TIMx_IRQHandler(void) { HAL_TIM_IRQHandler(&timer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &timer.Handler) {
    }
}
```


# PWM
* main.c
```
PWM_Handler pwm = {
    .TIM = TIMx,
    .Channel = "x",
    .Prescaler = x - 1,
    .Period = x - 1,
    .GPIOxPiny = "xx",
};

PWM_Init(&pwm);
```

* msp.c
```
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == pwm.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(pwm.TIM);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(pwm.TIM),
        };
        strcpy(gpio.GPIOxPiny, pwm.GPIOxPiny);
        GPIO_Init(&gpio);
    }
}
```

* interrupt.c
```
```


# Encoder
* main.c
```
Encoder_Handler encoder = {
    .TIM = TIMx,
    .Channel = "x | x",
    .GPIOxPiny = "xx | xx",
};

Encoder_Init(&encoder);
```

* msp.c
```
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == encoder.TIM) {
        __HAL_RCC_TIMx_CLK_ENABLE(htim->Instance);

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Alternate = GPIO_AFx_TIMy(encoder.TIM),
        };
        strcpy(gpio.GPIOxPiny, encoder.GPIOxPiny);
        GPIO_Init(&gpio);
    }
}
```

* interrupt.c
```
```


# DAC
* main.c
```
DAC_Handler dac = {
    .Channel = "1",
    .GPIOxPiny = "A4",
    .Length = DAC_DataLength,
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

DAC_Init(&dac);
DAC_DMAStart(&dac, DACData, dac.Length);
```

* msp.c
```
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == dac.Handler.Instance) {
        __HAL_RCC_DAC_CLK_ENABLE();

        GPIO_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, dac.GPIOxPiny);
        GPIO_Init(&gpio);

        if (dac.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(dac.DMA.DMAx);

            __HAL_LINKDMA(&dac.Handler, DMA_Handle1, dac.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == dac.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(dac.Timer.TIMx);

    }
}
```


# ADC
* main.c
```
ADC_Handler adc = {
    .ADCx = ADC1,
    .Channel = "5",
    .GPIOxPiny = "A5",
    .Length = ADC_DataLength,
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

ADC_Init(&adc);
ADC_DMAStart(&adc, ADCValue, adc.Length);
```

* msp.c
```
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == adc.ADCx) {
        __HAL_RCC_ADCx_CLK_ENABLE(adc.ADCx);

        GPI_Handler gpio = {
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
        };
        strcpy(gpio.GPIOxPiny, adc.GPIOxPiny);
        GPIO_Init(&gpio);

        if (adc.DMA.DMAx) {
            __HAL_RCC_DMAx_CLK_ENABLE(adc.DMA.DMAx);

            __HAL_LINKDMA(&adc.Handler, DMA_Handle, adc.DMA.Handler);
        }
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == adc.Timer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(adc.Timer.TIMx);
    }
}
```


# LCD
* main.c
```
LCD_Handler mLCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch_Handler mTouch = {
    .Direction = LCD_Vertical,
};

LCD_Init(&mLCD);
Touch_Init(&mTouch);
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

    UNUSED(hsram);
}
```


# LVGL
* main.c
```
Timer_Handler LVGLTimer = {
    .TIMx = TIM2,
    .ms = 6,
    .Interrupt = ENABLE,
};

lv_init();
lv_port_disp_init();
lv_port_indev_init();

Timer_Init(&LVGLTimer);
```

* msp.c
```
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == LVGLTimer.TIMx) {
        __HAL_RCC_TIMx_CLK_ENABLE(LVGLTimer.TIMx);

        HAL_NVIC_SetPriority(TIMx_IRQN(LVGLTimer.TIMx), 15, 0);
        HAL_NVIC_EnableIRQ(TIMx_IRQN(LVGLTimer.TIMx));
    }
}
```

* task.c
```
void vApplicationTickHook() { lv_tick_inc(1); }
```

* interrupt.c
```
void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&LVGLTimer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &LVGLTimer.Handler) {
        lv_timer_handler();
    }
}
```