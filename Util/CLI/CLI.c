#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "CLI.h"

void CLI_ShowHandler(CLI_t *CLI, char *pvParameters);
void CLI_SetHandler(CLI_t *CLI, char *pvParameters);

uint8_t CLIFunctionCount = 2;
CLIFunction_t *CLIFunctionRegistry = (CLIFunction_t[]){
    (CLIFunction_t){
        .Name = "show",
        .Handler = CLI_ShowHandler,
    },
    (CLIFunction_t){
        .Name = "set",
        .Handler = CLI_SetHandler,
    },
};

void CLI_Init(CLI_t *Self) { Serial_Printf(Self->Serial, "\r\n{CLI}# "); }

void CLI_Handler(CLI_t *Self) {
    if (Self->Serial->PackRecieved) {
        Serial_Printf(Self->Serial, "\r{CLI}# %s\r\n", Self->Serial->Pack);

        uint8_t i;
        for (i = 0; i < CLIFunctionCount; i++) {
            if (strncmp(CLIFunctionRegistry[i].Name, (char *)Self->Serial->Pack,
                        strlen(CLIFunctionRegistry[i].Name)) == 0) {
                CLIFunctionRegistry[i].Handler(
                    Self, (char *)Self->Serial->Pack +
                              strlen(CLIFunctionRegistry[i].Name) + 1);

                break;
            }
        }

        if (i == CLIFunctionCount) {
            Serial_Printf(Self->Serial, "{CLI}%s Not Found\r\n",
                          Self->Serial->Pack);
        }

        Serial_Printf(Self->Serial, "{CLI}# ");
        Serial_Clear(Self->Serial);
    } else {
        static uint8_t LastParsedCount = 0;

        if (LastParsedCount != Self->Serial->ParsedCount) {
            Serial_Printf(Self->Serial, "\r{CLI}# %s", Self->Serial->Pack);
            LastParsedCount = Self->Serial->ParsedCount;
        }
    }
}

void CLI_ShowHandler(CLI_t *Self, char *pvParameters) {
    uint8_t i;
    for (i = 0; i < Self->DataCount; i++) {
        if (strncmp(Self->Datas[i].Name, pvParameters,
                    strlen(Self->Datas[i].Name)) == 0) {
            switch (Self->Datas[i].DataType) {
            case CLIData_Int:
                Serial_Printf(Self->Serial, "{CLI}%s: %d\r\n",
                              Self->Datas[i].Name, *Self->Datas[i].IntDataPtr);
                break;

            case CLIData_Float:
                Serial_Printf(Self->Serial, "{CLI}%s: %f\r\n",
                              Self->Datas[i].Name,
                              *Self->Datas[i].FloatDataPtr);
                break;
            }

            break;
        }
    }

    if (i == Self->DataCount) {
        Serial_Printf(Self->Serial, "{CLI}%s Not Found\r\n", pvParameters);
    }
}

void CLI_SetHandler(CLI_t *Self, char *pvParameters) {
    uint8_t i;
    for (i = 0; i < Self->DataCount; i++) {
        if (strncmp(Self->Datas[i].Name, pvParameters,
                    strlen(Self->Datas[i].Name)) == 0) {
            pvParameters += (strlen(Self->Datas[i].Name) + 1);

            switch (Self->Datas[i].DataType) {
            case CLIData_Int:
                sscanf(pvParameters, "%d", Self->Datas[i].IntDataPtr);
                break;

            case CLIData_Float:
                sscanf(pvParameters, "%f", Self->Datas[i].FloatDataPtr);
                break;
            }

            break;
        }
    }

    if (i == Self->DataCount) {
        Serial_Printf(Self->Serial, "{CLI}%s Not Found\r\n", pvParameters);
    }
}