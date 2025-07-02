#ifndef __GRAYSCALESENSOR_I2C_H
#define __GRAYSCALESENSOR_I2C_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GWGray.h"

#define ACK       0x0
#define NACK      0x1

#define LOW       0x0
#define HIGH      0x1

#define I2C_READ  0x1
#define I2C_WRITE 0x0

void GWGray_SWI2C_Init(GWGray_t *Self);
int8_t GWGray_SWI2C_NowAddrReadBytes(GWGray_t *Self, uint8_t Addr,
                                     uint8_t *Bytes, uint8_t Length);
int8_t GWGray_SWI2C_SingedAddrReadBytes(GWGray_t *Self, uint8_t DevAddr,
                                        uint8_t MemAddr, uint8_t *Bytes,
                                        uint8_t Length);

#endif
