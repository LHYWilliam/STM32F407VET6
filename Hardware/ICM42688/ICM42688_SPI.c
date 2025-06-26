#include "ICM42688_SPI.h"
#include "GPIO.h"

void ICM42688_HWSPI_Init(ICM42688_t *Self) {
    Self->Handel = (SPI_HandleTypeDef){
        .Instance = Self->SPIx,
        .Init.Mode = SPI_MODE_MASTER,
        .Init.Direction = SPI_DIRECTION_2LINES,
        .Init.DataSize = SPI_DATASIZE_8BIT,
        .Init.CLKPolarity = SPI_POLARITY_HIGH,
        .Init.CLKPhase = SPI_PHASE_2EDGE,
        .Init.NSS = SPI_NSS_SOFT,
        .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256,
        .Init.FirstBit = SPI_FIRSTBIT_MSB,
        .Init.TIMode = SPI_TIMODE_DISABLE,
        .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
        .Init.CRCPolynomial = 7,
    };
    HAL_SPI_Init(&Self->Handel);

    __HAL_SPI_ENABLE(&Self->Handel);
}

#define ICM42688_HWSPI_Start(Self) GPIO_Write(Self->CS_ODR, 0)

#define ICM42688_HWSPI_Stop(Self)  GPIO_Write(Self->CS_ODR, 1)

uint8_t ICM42688_HWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxData) {
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&Self->Handel, &TxData, &RxData, 1, HAL_MAX_DELAY);

    return RxData;
}

void ICM42688_HWSPI_ReadWriteBytes(ICM42688_t *Self, uint8_t *Bytes,
                                   uint8_t Length) {
    for (uint8_t i = 0; i < Length; i++) {
        *Bytes = ICM42688_HWSPI_ReadWriteByte(Self, *Bytes);
        Bytes++;
    }
}

uint8_t ICM42688_HWSPI_ReadReg(ICM42688_t *Self, uint8_t Reg) {
    Reg |= 0x80;

    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    uint8_t Value = ICM42688_HWSPI_ReadWriteByte(Self, 0xFF);
    ICM42688_HWSPI_Stop(Self);

    return Value;
}

void ICM42688_HWSPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Bytes,
                             uint16_t Length) {
    Reg |= 0x80;

    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    while (Length) {
        *Bytes = ICM42688_HWSPI_ReadWriteByte(Self, 0x00);
        Length--;
        Bytes++;
    }
    ICM42688_HWSPI_Stop(Self);
}

uint8_t ICM42688_HWSPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Byte) {
    ICM42688_HWSPI_Start(Self);
    ICM42688_HWSPI_ReadWriteByte(Self, Reg);
    ICM42688_HWSPI_ReadWriteByte(Self, Byte);
    ICM42688_HWSPI_Stop(Self);

    return 0;
}
