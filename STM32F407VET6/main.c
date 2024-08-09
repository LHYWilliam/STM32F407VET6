#include "OLED.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "encoder.h"
#include "key.h"
#include "led.h"
#include "pwm.h"
#include "serial.h"
#include "signal.h"
#include "timer.h"

LED LED0 = {
    .GPIOxPiny = "A1",
    .Mode = LOW,
};

LED LED1 = {
    .GPIOxPiny = "A2",
    .Mode = LOW,
};

Key key = {
    .GPIOxPiny = "C0",
    .Mode = HIGH,
};

OLED oled = {
    .SCL = "C2",
    .SDA = "C3",
};

Serial serial = {
    .usart = USART1,
    .baudrate = 115200,
    .TX = "A9",
    .RX = "A10",
};

PWM pwm = {
    .TIM = TIM5,
    .channel = "2",
    .Prescaler = 8400 - 1,
    .Period = 100 - 1,
    .GPIOxPiny = "A1",
};

Encoder encoder = {
    .TIM = TIM8,
    .channel = "1 | 2",
    .GPIOxPiny = "C6 | C7",
};

mADC adc = {
    .ADCx = ADC1,
    .channel = "6",
    .GPIOxPiny = "A6",
};

DMA ADC_DMA = {
    .DMAx = DMA2,
    .channel = 0,
    .stream = 0,
    .sourceInc = DISABLE,
    .sourceSize = 32,
    .targetInc = ENABLE,
    .targetSize = 32,
};

mDAC dac = {
    .channel = "1",
    .GPIOxPiny = "A4",
    .Trigger = DAC_TRIGGER_T2_TRGO,
};

DMA DAC_DMA = {
    .DMAx = DMA1,
    .channel = 7,
    .stream = 5,
    .sourceInc = ENABLE,
    .sourceSize = 32,
    .targetInc = DISABLE,
    .targetSize = 32,
    .invert = ENABLE,
};

Timer timer = {
    .TIM = TIM2,
    .ms = 1,
    .Trigger = TIM_TRGO_UPDATE,
};

#define DATA_LENGTH 256

uint16_t i;
uint8_t serialFlag;

uint32_t adcValue[DATA_LENGTH];
float32_t FFT_Input[DATA_LENGTH * 2];
float32_t FFT_Output[DATA_LENGTH];

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_init(&LED0);
    LED_init(&LED1);
    Key_init(&key);

    Serial_init(&serial);
    Serial_RXITStart(&serial, 1);

    ADC_init(&adc);
    DMA_init(&ADC_DMA);
    ADC_DMAStart(&adc, adcValue, DATA_LENGTH);
    ADC_Start(&adc);

    DAC_init(&dac);
    DMA_init(&DAC_DMA);
    DAC_DMAStart(&dac, (uint32_t *)sinUint, DATA_LENGTH);
    DAC_start(&dac);

    Timer_init(&timer);

    arm_cfft_instance_f32 scfft;
    arm_cfft_init_f32(&scfft, DATA_LENGTH);

    for (;;) {
        if (Key_read(&key) == KEYDOWN) {
            serialFlag = !serialFlag;
        };

        if (serialFlag == DISABLE) {
            for (uint16_t i = 0; i < DATA_LENGTH; i++) {
                FFT_Input[i * 2] = adcValue[i] / 4095.;
                FFT_Input[i * 2 + 1] = 0;
            }
            arm_cfft_f32(&scfft, FFT_Input, DISABLE, ENABLE);
            arm_cmplx_mag_f32(FFT_Input, FFT_Output, DATA_LENGTH);
        }
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&serial.Handler); }

void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&timer.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.usart) {
    }

    Serial_RXITStart(&serial, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &timer.Handler) {
        if (serialFlag == ENABLE) {
            Serial_printf(&serial, "%f,%f\n", FFT_Output[i],
                          adcValue[i] / 4095.);
            i++;

            if (i == DATA_LENGTH) {
                i = 0;
                serialFlag = DISABLE;
            }
        }
    }
}