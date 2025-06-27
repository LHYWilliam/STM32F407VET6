#ifndef ICM42688_SPI_H
#define ICM42688_SPI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "ICM42688.h"

void ICM42688_HWSPI_Init(ICM42688_t *Self);
void ICM42688_SWSPI_Init(ICM42688_t *Self);
uint8_t ICM42688_SPI_ReadReg(ICM42688_t *Self, uint8_t Reg);
void ICM42688_SPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Bytes,
                           uint16_t Length);
uint8_t ICM42688_SPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Byte);

#endif
