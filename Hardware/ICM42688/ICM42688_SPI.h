#ifndef ICM42688_SPI_H
#define ICM42688_SPI_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "ICM42688.h"

#define __HAL_RCC_SPIx_CLK_ENABLE(x)                                           \
    do {                                                                       \
        if ((x) == SPI1) {                                                     \
            __HAL_RCC_SPI1_CLK_ENABLE();                                       \
        } else if ((x) == SPI2) {                                              \
            __HAL_RCC_SPI2_CLK_ENABLE();                                       \
        } else if ((x) == SPI3) {                                              \
            __HAL_RCC_SPI3_CLK_ENABLE();                                       \
        }                                                                      \
    } while (0)

#define GPIO_AFx_SPIy(x)                                                       \
    ((x) == SPI1   ? GPIO_AF5_SPI1                                             \
     : (x) == SPI2 ? GPIO_AF5_SPI2                                             \
     : (x) == SPI3 ? GPIO_AF6_SPI3                                             \
                   : NULL)

void ICM42688_HWSPI_Init(ICM42688_t *Self);
void ICM42688_SWSPI_Init(ICM42688_t *Self);
uint8_t ICM42688_SPI_ReadReg(ICM42688_t *Self, uint8_t Reg);
void ICM42688_SPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Bytes,
                           uint16_t Length);
uint8_t ICM42688_SPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Byte);

#endif
