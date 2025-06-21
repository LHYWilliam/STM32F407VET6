#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

// #include "Key.h"
// #include "LED.h"
// #include "sys.h"

#include "GPIO.h"
#include "LED.h"

LED_t LED0 = {
    .GPIOxPiny = A1,
    .Mode = LEDMode_PullUp,
};

LED_t LED1 = {
    .GPIOxPiny = A2,
    .Mode = LEDMode_PullUp,
};

GPIO_t Key = {
    .Mode = GPIO_MODE_INPUT,
    .Pull = GPIO_PULLDOWN,
};

uint32_t Key_IDR;

// Key_t Key = {
//     .GPIOxPiny = "C0",
// };

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    LED_Init(&LED0);
    LED_Init(&LED1);
    Key_IDR = GPIO_InitPin(&Key, C0);

    // LED_Init(&LED);
    // Key_Init(&Key);

    xLEDTimer = xTimerCreate("xLEDTimer", pdMS_TO_TICKS(200), pdTRUE, (void *)0,
                             vLEDTimerCallback);
    xTaskCreate(vMainTaskCode, "vMainTask", 128, NULL, 1, &xMainTaskHandle);

    xTimerStart(xLEDTimer, 0);

    vTaskStartScheduler();
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    while (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        ;

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    while (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
        ;
    /** Enables the Clock Security System
     */
    HAL_RCC_EnableCSS();
}