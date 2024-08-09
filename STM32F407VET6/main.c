#include "arm_math.h"

#include "adc.h"
#include "dac.h"
#include "key.h"
#include "led.h"
#include "serial.h"
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

Serial serial = {
    .usart = USART1,
    .TX = "A9",
    .RX = "A10",
    .baudrate = 115200,
};

mDAC dac = {
    .channel = "1",
    .GPIOxPiny = "A4",
    .Trigger = DAC_TRIGGER_T4_TRGO,
    .DMA =
        {
            .DMAx = DMA1,
            .channel = 7,
            .stream = 5,
        },
};

mADC adc = {
    .ADCx = ADC1,
    .channel = "6",
    .GPIOxPiny = "A6",
    .continuous = DISABLE,
    .trigger = ADC_EXTERNALTRIGCONV_T2_TRGO,
    .DMA =
        {
            .DMAx = DMA2,
            .channel = 0,
            .stream = 0,
        },
};

Timer signalGeneratorTimer = {
    .TIM = TIM4,
    .Hz = 10000,
    .Trigger = TIM_TRGO_UPDATE,
};

Timer signalSamplerTimer = {
    .TIM = TIM2,
    .Hz = 10000,
    .Trigger = TIM_TRGO_UPDATE,
};

Timer taskTimer = {
    .TIM = TIM3,
    .ms = 1,
    .interrupt = ENABLE,
};

#define DATA_LENGTH 4096

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
    ADC_DMAStart(&adc, adcValue, DATA_LENGTH);

    uint32_t data[DATA_LENGTH];
    for (uint16_t i = 0; i < DATA_LENGTH; i++) {
        data[i] =
            (uint32_t)(2048 +
                       (2048 * arm_sin_f32(1 * 2 * PI * i / DATA_LENGTH)));
        data[i] = data[i] > 4095 ? 4095 : data[i];
    }

    DAC_init(&dac);
    DAC_DMAStart(&dac, (uint32_t *)data, DATA_LENGTH);

    Timer_init(&signalGeneratorTimer);
    Timer_init(&signalSamplerTimer);
    Timer_init(&taskTimer);

    arm_cfft_instance_f32 scfft;
    arm_cfft_init_f32(&scfft, DATA_LENGTH);

    for (;;) {
        if (Key_read(&key) == KEYDOWN) {
            serialFlag = serialFlag ? DISABLE : ENABLE;
            i = 0;
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

void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&taskTimer.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.usart) {
    }

    Serial_RXITStart(&serial, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &taskTimer.Handler) {
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