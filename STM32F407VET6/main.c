#include "stm32f4xx_hal.h"

int main() {
    for (;;) {
    }
}

void SysTick_Handler(void) {
  HAL_IncTick();
}
