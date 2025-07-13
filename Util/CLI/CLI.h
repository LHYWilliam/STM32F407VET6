#ifndef CLI_H
#define CLI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "Serial.h"

typedef enum {
    CLIData_Int,
    CLIData_Float,
} CLI_DataType;

typedef struct {
    char Name[32];
    CLI_DataType DataType;
    union {
        int32_t *IntDataPtr;
        float *FloatDataPtr;
    };
} CLIData_t;

typedef struct {
    Serial_t *Serial;
    uint8_t DataCount;
    CLIData_t *Datas;
} CLI_t;

typedef struct {
    char Name[32];
    void (*Handler)(CLI_t *, char *);
} CLIFunction_t;

void CLI_Init(CLI_t *CLI);
void CLI_Handler(CLI_t *CLI);

#endif