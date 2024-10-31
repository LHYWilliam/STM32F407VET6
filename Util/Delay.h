#ifndef DELAY_H
#define DELAY_H

#include CMSIS_device_header

#ifdef __cplusplus
extern "C" {
#endif

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif