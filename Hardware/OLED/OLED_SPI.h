#ifndef OLED_SPI_H
#define OLED_SPI_H

#include "OLED.h"

typedef enum {
    SPI_Command,
    SPI_Data,
} OLED_SPI_ByteType;

void OLED_SWSPI_Init(OLED_t *Self);
void OLED_HWSPI_Init(OLED_t *Self);

#endif