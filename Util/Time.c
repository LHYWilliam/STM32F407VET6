#include "Time.h"

void Time_Delayus(uint32_t us) {
    uint32_t now, last, reload, total, goal;
    total = 0;
    goal = us * 168;
    last = SysTick->VAL;
    reload = SysTick->LOAD;

    for (;;) {
        now = SysTick->VAL;
        if (now != last) {
            total += now < last ? last - now : reload - now + last;

            if (total >= goal) {
                break;
            }

            last = now;
        }
    }
}

void Time_Delayms(uint32_t ms) {
    while (ms--) {
        Time_Delayus(1000);
    }
}

uint32_t Time_Getus() {
    return Time_Getms() * 1000 +
           (SysTick->LOAD + 1 - SysTick->VAL) * 1000 / (SysTick->LOAD + 1);
}

uint32_t Time_Getms() { return HAL_GetTick(); }