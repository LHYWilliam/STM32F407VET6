#include "FreeRTOS.h"
#include "timers.h"

#include "sys.h"
#include "Key.h"
#include "LED.h"

extern LED_t LED;
extern Key_t Key;

void vMainTaskCode(void *pvParameters) {
    for (;;) {
        PAout(1) = !PAout(1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vApplicationTickHook() { HAL_IncTick(); }