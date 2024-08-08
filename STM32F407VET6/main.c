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
    .SCL = "C0",
    .SDA = "C1",
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
    .targetInc = DISABLE,
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

uint8_t i = 0, state = 1;
uint32_t adcValue;

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    // LED_init(&LED0);
    LED_init(&LED1);
    Key_init(&key);

    OLED_Init(&oled);
    Serial_init(&serial);
    Serial_RXITStrart(&serial, 1);

    PWM_init(&pwm);

    ADC_init(&adc);
    DMA_init(&ADC_DMA);
    ADC_DMAStart(&adc, &adcValue, 1);
    ADC_Start(&adc);

    DAC_init(&dac);
    DMA_init(&DAC_DMA);
    DAC_DMAStart(&dac, (uint32_t *)sing, 100);
    DAC_start(&dac);
    DAC_set(&dac, 1, 4095);

    Timer_init(&timer);

    for (;;) {
        OLED_ShowNum(&oled, 1, 1, adcValue, 4);
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&serial.Handler); }

void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&timer.Handler); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == serial.usart) {
    }

    Serial_RXITStrart(&serial, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &timer.Handler) {
        PWM_set(&pwm, 2, i);
        state ? i++ : i--;
        state = (i == 0 || i == 100) ? !state : state;

        Serial_printf(&serial, "%d\n", adcValue);
    }
}