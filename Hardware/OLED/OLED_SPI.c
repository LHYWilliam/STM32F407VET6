#include "GPIO.h"
#include "Time.h"

#include "OLED_SPI.h"

void OLED_SWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length);
void OLED_SWSPI_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_SWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands, uint16_t Length);
void OLED_HWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length);
void OLED_HWSPI_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_HWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands, uint16_t Length);

void OLED_SWSPI_Init(OLED_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_MODE_OUTPUT_PP;

    Self->D0_ODR = GPIO_InitPin(&GPIO, Self->D0);
    Self->D1_ODR = GPIO_InitPin(&GPIO, Self->D1);
    Self->RES_ODR = GPIO_InitPin(&GPIO, Self->RES);
    Self->DC_ODR = GPIO_InitPin(&GPIO, Self->DC);
    Self->CS_ODR = GPIO_InitPin(&GPIO, Self->CS);

    GPIO_Write(Self->D0, 1);
    GPIO_Write(Self->D1, 1);
    GPIO_Write(Self->RES_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
    GPIO_Write(Self->CS_ODR, 1);

    Self->OLED_WriteDatas = OLED_SWSPI_WriteDatas;
    Self->OLED_WriteCommand = OLED_SWSPI_WriteCommand;
    Self->OLED_WriteCommands = OLED_SWSPI_WriteCommands;
}

void OLED_SWSPI_Start(OLED_t *Self, uint8_t DC) {
    GPIO_Write(Self->DC_ODR, DC);
    GPIO_Write(Self->CS_ODR, 0);
}

void OLED_SWSPI_Stop(OLED_t *Self) {
    GPIO_Write(Self->CS_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
}

void OLED_SWSPI_WriteByte(OLED_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        GPIO_Write(Self->D0_ODR, 0);
        GPIO_Write(Self->D1_ODR, Byte & 0x80);
        GPIO_Write(Self->D0_ODR, 1);
        Byte <<= 1;
    }
}

void OLED_SWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(Self, Datas[i]);
    }
    OLED_SWSPI_Stop(Self);
}

void OLED_SWSPI_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWSPI_Start(Self, SPI_Command);
    OLED_SWSPI_WriteByte(Self, Command);
    OLED_SWSPI_Stop(Self);
}

void OLED_SWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Command);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWSPI_WriteByte(Self, Commands[i]);
    }
    OLED_SWSPI_Stop(Self);
}

void OLED_HWSPI_Init(OLED_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_MODE_AF_PP;
    Self->D0_ODR = GPIO_InitPin(&GPIO, Self->D0);
    Self->D1_ODR = GPIO_InitPin(&GPIO, Self->D1);

    GPIO.Mode = GPIO_MODE_OUTPUT_PP;
    Self->RES_ODR = GPIO_InitPin(&GPIO, Self->RES);
    Self->DC_ODR = GPIO_InitPin(&GPIO, Self->DC);
    Self->CS_ODR = GPIO_InitPin(&GPIO, Self->CS);

    GPIO_Write(Self->RES_ODR, 1);
    GPIO_Write(Self->DC_ODR, 1);
    GPIO_Write(Self->CS_ODR, 1);

    Self->OLED_WriteDatas = OLED_HWSPI_WriteDatas;
    Self->OLED_WriteCommand = OLED_HWSPI_WriteCommand;
    Self->OLED_WriteCommands = OLED_HWSPI_WriteCommands;

    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    // SPI_InitTypeDef SPI_InitStructure;
    // SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    // SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    // SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    // SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_BaudRatePrescaler =
    // SPI_BaudRatePrescaler_2; SPI_InitStructure.SPI_CPOL =
    // SPI_CPOL_Low; SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    // SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    // SPI_Init(Self->SPIx, &SPI_InitStructure);

    // SPI_Cmd(Self->SPIx, ENABLE);
}

#define OLED_HWSPI_Delay() Time_Delayus(1)

void OLED_HWSPI_WriteByte(OLED_t *Self, uint8_t Byte) {
    // while (SPI_I2S_GetFlagStatus(Self->SPIx, SPI_I2S_FLAG_TXE) != SET)
    //     ;
    // SPI_I2S_SendData(Self->SPIx, Byte);
}

void OLED_HWSPI_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Data);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_HWSPI_WriteByte(Self, Datas[i]);
        OLED_HWSPI_Delay();
    }
    OLED_SWSPI_Stop(Self);
}

void OLED_HWSPI_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWSPI_Start(Self, SPI_Command);
    OLED_HWSPI_WriteByte(Self, Command);
    OLED_HWSPI_Delay();
    OLED_SWSPI_Stop(Self);
}

void OLED_HWSPI_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWSPI_Start(Self, SPI_Command);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_HWSPI_WriteByte(Self, Commands[i]);
    }
    OLED_SWSPI_Stop(Self);
}