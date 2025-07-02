#include "GPIO.h"
#include "Time.h"

#include "GWGray_I2C.h"

void GWGray_SWI2C_Init(GWGray_t *Self) {
    GPIO_t GPIO;

    GPIO.Mode = GPIO_MODE_OUTPUT_OD;

    GPIO_InitPin(&GPIO, Self->SCL);
    GPIO_InitPin(&GPIO, Self->SDA);

    Self->SCL_GPIO = GPIOx(Self->SCL);
    Self->SCL_Pin = GPIO_Pinx(Self->SCL);
    Self->SDA_GPIO = GPIOx(Self->SDA);
    Self->SDA_Pin = GPIO_Pinx(Self->SDA);

    HAL_GPIO_WritePin(Self->SCL_GPIO, Self->SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Self->SDA_GPIO, Self->SDA_Pin, GPIO_PIN_SET);
}

void GWGray_SWI2C_SDAOut(GWGray_t *Self, uint8_t Bit) {
    HAL_GPIO_WritePin(Self->SDA_GPIO, Self->SDA_Pin, (GPIO_PinState)Bit);

    Time_Delayus(10);
}

uint8_t GWGray_SWI2C_SDAIn(GWGray_t *Self) {
    uint8_t Bit;
    Bit = (uint8_t)HAL_GPIO_ReadPin(Self->SDA_GPIO, Self->SDA_Pin);

    Time_Delayus(10);
    return Bit;
}

void GWGray_SWI2C_SCLOut(GWGray_t *Self, uint8_t Bit) {
    HAL_GPIO_WritePin(Self->SCL_GPIO, Self->SCL_Pin, (GPIO_PinState)Bit);

    Time_Delayus(10);
}

void GWGray_SWI2C_Start(GWGray_t *Self) {
    GWGray_SWI2C_SDAOut(Self, HIGH);
    GWGray_SWI2C_SCLOut(Self, HIGH);
    GWGray_SWI2C_SDAOut(Self, LOW);
    GWGray_SWI2C_SCLOut(Self, LOW);
}

void GWGray_SWI2C_Stop(GWGray_t *Self) {
    GWGray_SWI2C_SDAOut(Self, LOW);
    GWGray_SWI2C_SCLOut(Self, HIGH);
    GWGray_SWI2C_SDAOut(Self, HIGH);
}

void GWGray_SWI2C_WriteBit(GWGray_t *Self, uint8_t Bit) {
    GWGray_SWI2C_SDAOut(Self, Bit);
    GWGray_SWI2C_SCLOut(Self, HIGH);
    GWGray_SWI2C_SCLOut(Self, LOW);
}

uint8_t GWGray_SWI2C_ReadBit(GWGray_t *Self) {
    GWGray_SWI2C_SDAOut(Self, HIGH);
    GWGray_SWI2C_SCLOut(Self, HIGH);
    uint8_t Bit = GWGray_SWI2C_SDAIn(Self);
    GWGray_SWI2C_SCLOut(Self, LOW);

    return Bit;
}

uint8_t GWGray_SWI2C_WriteByte(GWGray_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; ++i) {
        GWGray_SWI2C_WriteBit(Self, Byte & (0x80 >> i));
    }

    uint8_t Ack = GWGray_SWI2C_ReadBit(Self);

    return Ack;
}

uint8_t GWGray_SWI2C_ReadByte(GWGray_t *Self, uint8_t Ack) {
    uint8_t Byte = 0;

    GWGray_SWI2C_SDAOut(Self, HIGH);

    for (uint8_t i = 0; i < 8; ++i) {
        GWGray_SWI2C_SCLOut(Self, HIGH);
        Byte <<= 1;
        Byte |= GWGray_SWI2C_SDAIn(Self);
        GWGray_SWI2C_SCLOut(Self, LOW);
    }

    GWGray_SWI2C_WriteBit(Self, Ack);

    return Byte;
}

int8_t GWGray_SWI2C_NowAddrReadBytes(GWGray_t *Self, uint8_t DevAddr,
                                     uint8_t *Bytes, uint8_t Length) {

    GWGray_SWI2C_Start(Self);

    uint8_t Ack = GWGray_SWI2C_WriteByte(Self, DevAddr | I2C_READ);
    if (Ack) {
        GWGray_SWI2C_Stop(Self);
        return 1;
    }

    uint8_t i;
    for (i = 0; i < Length - 1; ++i) {
        Bytes[i] = GWGray_SWI2C_ReadByte(Self, ACK);
    }

    Bytes[i] = GWGray_SWI2C_ReadByte(Self, NACK);

    GWGray_SWI2C_Stop(Self);

    return 0;
}

int8_t GWGray_SWI2C_NowAddrWriteBytes(GWGray_t *Self, uint8_t DevAddr,
                                      const uint8_t *Bytes, uint8_t Length) {
    GWGray_SWI2C_Start(Self);

    uint8_t Ack = GWGray_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GWGray_SWI2C_Stop(Self);
        return 1;
    }

    for (uint8_t i = 0; i < Length; ++i) {
        Ack = GWGray_SWI2C_WriteByte(Self, Bytes[i]);
    }

    GWGray_SWI2C_Stop(Self);

    return 0;
}

int8_t GWGray_SWI2C_NowAddrReadByte(GWGray_t *Self, uint8_t DevAddr,
                                    uint8_t *data) {
    return GWGray_SWI2C_NowAddrReadBytes(Self, DevAddr, data, 1);
}

int8_t GraySacleSensor_SWI2C_NowAddrWriteByte(GWGray_t *Self, uint8_t DevAddr,
                                              const uint8_t Byte) {
    return GWGray_SWI2C_NowAddrWriteBytes(Self, DevAddr, &Byte, 1);
}

int8_t GWGray_SWI2C_SingedAddrReadBytes(GWGray_t *Self, uint8_t DevAddr,
                                        uint8_t MemAddr, uint8_t *Bytes,
                                        uint8_t Length) {
    GWGray_SWI2C_Start(Self);

    uint8_t Ack = GWGray_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GWGray_SWI2C_Stop(Self);
        return 1;
    }
    Ack = GWGray_SWI2C_WriteByte(Self, MemAddr);

    return GWGray_SWI2C_NowAddrReadBytes(Self, DevAddr, Bytes, Length);
}

int8_t GWGray_SWI2C_SingedAddrWriteBytes(GWGray_t *Self, uint8_t DevAddr,
                                         uint8_t MemAddr, const uint8_t *Bytes,
                                         uint8_t Length) {
    GWGray_SWI2C_Start(Self);

    uint8_t Ack = GWGray_SWI2C_WriteByte(Self, DevAddr | I2C_WRITE);
    if (Ack) {
        GWGray_SWI2C_Stop(Self);
        return 1;
    }
    Ack = GWGray_SWI2C_WriteByte(Self, MemAddr);

    return GWGray_SWI2C_NowAddrWriteBytes(Self, DevAddr, Bytes, Length);
}
