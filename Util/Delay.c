#include "Delay.h"

uint32_t now, last, reload, total, goal;

void Delay_us(uint32_t us) {
    total = 0;
    goal = us * 168;
    last = SysTick->VAL;
    reload = SysTick->LOAD;

    while (1) {
        now = SysTick->VAL;
        if (now != last) {
            total += now < last ? last - now : reload - now + last;

            if (total >= goal) {
                break;
            }

            last = now;
        }
    };
}

void Delay_ms(uint32_t ms) {
    while (ms--) {
        Delay_us(1000);
    }
}