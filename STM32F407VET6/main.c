#include "key.h"
#include "led.h"

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

int main() {
    HAL_Init();

    LED_init(&LED0);
    LED_init(&LED1);
    Key_init(&key);

    for (;;) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }
