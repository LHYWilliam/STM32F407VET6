#include "GPIO.h"

#include "ICM42688_SPI.h"

uint8_t ICM42688_HWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxByte);
uint8_t ICM42688_SWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxByte);

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

    Self->SPI_ReadWriteByte = ICM42688_HWSPI_ReadWriteByte;
}

void ICM42688_SWSPI_Init(ICM42688_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_MODE_OUTPUT_PP;
    Self->SCLK_ODR = GPIO_InitPin(&GPIO, Self->SCLK);
    Self->MOSI_ODR = GPIO_InitPin(&GPIO, Self->MOSI);
    Self->CS_ODR = GPIO_InitPin(&GPIO, Self->CS);

    GPIO.Mode = GPIO_MODE_INPUT;
    Self->MISO_IDR = GPIO_InitPin(&GPIO, Self->MISO);

    GPIO_Write(Self->SCLK_ODR, 1);
    GPIO_Write(Self->MOSI_ODR, 1);
    GPIO_Write(Self->CS_ODR, 1);

    Self->SPI_ReadWriteByte = ICM42688_SWSPI_ReadWriteByte;
}

void ICM42688_SPI_Start(ICM42688_t *Self) { GPIO_Write(Self->CS_ODR, 0); }

void ICM42688_SPI_Stop(ICM42688_t *Self) { GPIO_Write(Self->CS_ODR, 1); }

uint8_t ICM42688_HWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxByte) {
    uint8_t RxByte;
    HAL_SPI_TransmitReceive(&Self->Handel, &TxByte, &RxByte, 1, HAL_MAX_DELAY);

    return RxByte;
}

uint8_t ICM42688_SWSPI_ReadWriteByte(ICM42688_t *Self, uint8_t TxByte) {
    uint8_t RxByte = 0x00;

    GPIO_Write(Self->SCLK_ODR, 0);

    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(Self->MOSI_ODR, TxByte & (0x80 >> i));

        GPIO_Write(Self->SCLK_ODR, 1);

        if (GPIO_ReadInput(Self->MISO_IDR)) {
            RxByte |= (0x80 >> i);
        }

        GPIO_Write(Self->SCLK_ODR, 0);
    }

    return RxByte;
}

uint8_t ICM42688_SPI_ReadReg(ICM42688_t *Self, uint8_t Reg) {
    Reg |= 0x80;

    ICM42688_SPI_Start(Self);
    Self->SPI_ReadWriteByte(Self, Reg);
    uint8_t Value = Self->SPI_ReadWriteByte(Self, 0xFF);
    ICM42688_SPI_Stop(Self);

    return Value;
}

void ICM42688_SPI_ReadRegs(ICM42688_t *Self, uint8_t Reg, uint8_t *Bytes,
                           uint16_t Length) {
    Reg |= 0x80;

    ICM42688_SPI_Start(Self);
    Self->SPI_ReadWriteByte(Self, Reg);
    while (Length) {
        *Bytes = Self->SPI_ReadWriteByte(Self, 0x00);
        Length--;
        Bytes++;
    }
    ICM42688_SPI_Stop(Self);
}

uint8_t ICM42688_SPI_WriteReg(ICM42688_t *Self, uint8_t Reg, uint8_t Byte) {
    ICM42688_SPI_Start(Self);
    Self->SPI_ReadWriteByte(Self, Reg);
    Self->SPI_ReadWriteByte(Self, Byte);
    ICM42688_SPI_Stop(Self);

    return 0;
}
