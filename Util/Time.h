#ifndef TIME_H
#define TIME_H

#include "RTE_Components.h"
#include CMSIS_device_header

void Time_Delayus(uint32_t us);
void Time_Delayms(uint32_t ms);
uint32_t Time_Getus();
uint32_t Time_Getms();

#endif