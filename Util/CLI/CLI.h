#ifndef CLI_H
#define CLI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "Serial.h"

typedef struct {
    Serial_t *Serial;
} CLI_t;

void CLI_Init(CLI_t *CLI);
void CLI_Handler(CLI_t *CLI);

#endif