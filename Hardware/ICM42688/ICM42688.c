#include "Time.h"

#include "ICM42688.h"
#include "ICM42688_AHRS.h"
#include "ICM42688_SPI.h"

void ICM42688_ConfigReg(ICM42688_t *Self);

void ICM42688_Init(ICM42688_t *Self) {
    if (Self->SPI) {
        ICM42688_SWSPI_Init(Self);

    } else if (Self->SPIx) {
        ICM42688_HWSPI_Init(Self);
    }

    ICM42688_ConfigReg(Self);

    ICM42688_AHRS_Init(Self);
}

void ICM42688_GetAccGyro(ICM42688_t *Self, float *AccGyro) {
    uint8_t Buffer[12];
    ICM42688_SPI_ReadRegs(Self, ICM42688_ACCEL_DATA_X1, Buffer, 12);

    AccGyro[0] = (float)((int16_t)(((uint16_t)Buffer[0] << 8) | Buffer[1]) *
                         Self->AccSensitivity);
    AccGyro[1] = (float)((int16_t)(((uint16_t)Buffer[2] << 8) | Buffer[3]) *
                         Self->AccSensitivity);
    AccGyro[2] = (float)((int16_t)(((uint16_t)Buffer[4] << 8) | Buffer[5]) *
                         Self->AccSensitivity);
    AccGyro[3] = (float)((int16_t)(((uint16_t)Buffer[6] << 8) | Buffer[7]) *
                         Self->GyroSensitivity);
    AccGyro[4] = (float)((int16_t)(((uint16_t)Buffer[8] << 8) | Buffer[9]) *
                         Self->GyroSensitivity);
    AccGyro[5] = (float)((int16_t)(((uint16_t)Buffer[10] << 8) | Buffer[11]) *
                         Self->GyroSensitivity);
}

void ICM42688_GetTemperature(ICM42688_t *Self, int16_t *Temperature) {
    uint8_t Buffer[2];
    ICM42688_SPI_ReadRegs(Self, ICM42688_TEMP_DATA1, Buffer, 2);

    *Temperature =
        (int16_t)(((int16_t)((Buffer[0] << 8) | Buffer[1])) / 132.48 + 25);
}

void ICM42688_GetAccSensitivity(ICM42688_t *Self, uint8_t AccScale) {
    const float AccSensitivity[] = {
        16000 / 32768.0f, // AFS_16G
        8000 / 32768.0f,  // AFS_8G
        4000 / 32768.0f,  // AFS_4G
        2000 / 32768.0f,  // AFS_2G
    };

    Self->AccSensitivity = AccSensitivity[AccScale];
}

void ICM42688_GetGyroSensitivity(ICM42688_t *Self, uint8_t GyroScale) {
    const float GyroSensitivity[] = {
        2000.0f / 32768.0f, // GFS_2000DPS
        1000.0f / 32768.0f, // GFS_1000DPS
        500.0f / 32768.0f,  // GFS_500DPS
        250.0f / 32768.0f,  // GFS_250DPS
        125.0f / 32768.0f,  // GFS_125DPS
        62.5f / 32768.0f,   // GFS_62_5DPS
        31.25f / 32768.0f,  // GFS_31_25DPS
        15.125f / 32768.0f, // GFS_15_125DPS
    };

    Self->GyroSensitivity = GyroSensitivity[GyroScale];
}

void ICM42688_ConfigReg(ICM42688_t *Self) {
    uint8_t Value = 0;

    Value = ICM42688_SPI_ReadReg(Self, ICM42688_WHO_AM_I); // 读取传感器ID
    if (Value != ICM42688_ID) {
        return;
    }

    ICM42688_SPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0);
    ICM42688_SPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x01); // 软件复位传感器
    Time_Delayms(100);

    // 配置加速度计量程
    ICM42688_GetAccSensitivity(Self, (uint8_t)Self->AccRange);
    ICM42688_SPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);

    Value = (uint8_t)Self->AccRange << 5;
    Value |= (uint8_t)Self->AccFreq;
    ICM42688_SPI_WriteReg(Self, ICM42688_ACCEL_CONFIG0, Value);

    // 配置陀螺仪量程
    ICM42688_GetGyroSensitivity(Self, (uint8_t)Self->GyroRange);
    ICM42688_SPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);

    Value = (uint8_t)Self->GyroRange << 5;
    Value |= (uint8_t)Self->GyroFreq;
    ICM42688_SPI_WriteReg(Self, ICM42688_GYRO_CONFIG0, Value);

    ICM42688_SPI_WriteReg(Self, ICM42688_REG_BANK_SEL, 0x00);
    Value = ICM42688_SPI_ReadReg(Self, ICM42688_PWR_MGMT0);
    Value &= ~(1 << 5);  // 使能温度测量
    Value |= ((3) << 2); // 设置陀螺仪低噪声
    Value |= (3);        // 设置加速度计低噪声
    ICM42688_SPI_WriteReg(Self, ICM42688_PWR_MGMT0, Value);
    Time_Delayms(1);
}
