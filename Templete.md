# LED0
* main.c
```
LED LED0 = {
    .GPIOxPiny = "A1",
};

LED_Init(&LED0);
```


# LED1
* main.c
```
LED LED1 = {
    .GPIOxPiny = "A2",
};

LED_Init(&LED1);
```


# Key0
* main.c
```
Key key0 = {
    .GPIOxPiny = "C0",
};

Key_Init(&key);
```


# Key1
* main.c
```
Key key1 = {
    .GPIOxPiny = "A0",
};

Key_Init(&key);
```


# Serial
* main.c
```
Serial serial = {
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

        GPIO RX = {
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Alternate = GPIO_AF7_USARTx(serial.USART),
        };
        strcpy(RX.GPIOxPiny, serial.RX);
        GPIO_Init(&RX);

        GPIO TX = {
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
OLED oled = {
    .SCL = "xx",
    .SDA = "xx",
};

OLED_Init(&oled);
```


# Timer
* main.c
```
Timer timer = {
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
PWM pwm = {
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

        GPIO gpio = {
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
Encoder encoder = {
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

        GPIO gpio = {
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
mDAC dac = {
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

        GPIO gpio = {
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
mADC adc = {
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

        GPIO gpio = {
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
LCD mLCD = {
    .Direction = LCD_Vertical,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 0,
            .Channel = 0,
        },
};

Touch mTouch = {
    .Direction = LCD_Vertical,
};

    LCD_Init(&mLCD);
    Touch_Init(&mTouch, &mLCD);
```

* msp.c
```
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram) {
    __HAL_RCC_FSMC_CLK_ENABLE();

    GPIO FSMC_D = {
        .GPIOxPiny = "D14 | D15 | D0 | D1 | E7 | E8 | E9 | E10 | E11 | E12 | "
                     "E13 | E14 | E15 | D8 | D9 | D10",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_D);

    GPIO FSMC_NOE = {
        .GPIOxPiny = "D4 ",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NOE);

    GPIO FSMC_NWE = {
        .GPIOxPiny = "D5",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NWE);

    GPIO FSMC_NE1 = {
        .GPIOxPiny = "D7",
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Alternate = GPIO_AF12_FSMC,
    };
    GPIO_Init(&FSMC_NE1);

    GPIO FSMC_A18 = {
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
Timer LVGLTimer = {
    .TIM = TIMx,
    .ms = 10,
    .Interrupt = ENABLE,
};

Timer_Init(&LVGLTimer);
```

* lvgl.c
```
#define BUFFER_SIZE 480 * 320 / 6

uint16_t dispBuffer[BUFFER_SIZE];

extern LCD mLCD;
extern Touch mTouch;

void LVGL_FlushCallback(lv_display_t *display, const lv_area_t *area,
                        uint8_t *px_map) {
    LCD_ShowImage(&mLCD, area->x1, area->y1, area->x2 - area->x1 + 1,
                  area->y2 - area->y1 + 1, px_map);

    lv_display_flush_ready(display);
}

void LVGL_TouchCallback(lv_indev_t *indev, lv_indev_data_t *data) {
    if (Touch_Scan(&mTouch, &mLCD)) {
        data->point.x = mTouch.X[0];
        data->point.y = mTouch.Y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void LVGL_Init() {
    lv_init();

    lv_disp_t *disp = lv_display_create(320, 480);
    lv_display_set_flush_cb(disp, LVGL_FlushCallback);
    lv_display_set_buffers(disp, dispBuffer, NULL, BUFFER_SIZE,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, LVGL_TouchCallback);
}
```

* interrupt.c
```
void TIMx_IRQHandler(void) { HAL_TIM_IRQHandler(&LEDTimer.Handler); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &LVGLTimer.Handler) {
        lv_timer_handler();
    }
}

void SysTick_Handler(void) {
    lv_tick_inc(1);
}
```