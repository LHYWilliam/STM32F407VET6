#ifndef ICM42688_SPI_H
#define ICM42688_SPI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "ICM42688.h"

void ICM42688_HWSPI_Init(ICM42688_t *Self);
uint8_t ICM42688_HWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxData);
void ICM42688_HWSPI_ReadWriteBytes(ICM42688_t *Self, uint8_t *Bytes,
                                   uint8_t Length);
uint8_t ICM42688_HWSPI_ReadReg(ICM42688_t *Self, uint8_t Reg);
void ICM42688_HWSPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Bytes,
                             uint16_t Length);
uint8_t ICM42688_HWSPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Byte);

#endif
