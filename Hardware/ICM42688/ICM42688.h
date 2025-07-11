#ifndef ICM42688_H
#define ICM42688_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.H"

#define ICM42688_DEVICE_CONFIG        0x11
#define ICM42688_DRIVE_CONFIG         0x13
#define ICM42688_INT_CONFIG           0x14
#define ICM42688_FIFO_CONFIG          0x16

#define ICM42688_TMST_FSYNCH          0x2B
#define ICM42688_TMST_FSYNCL          0x2C
#define ICM42688_INT_STATUS           0x2D
#define ICM42688_FIFO_COUNTH          0x2E
#define ICM42688_FIFO_COUNTL          0x2F
#define ICM42688_FIFO_DATA            0x30
#define ICM42688_APEX_DATA0           0x31
#define ICM42688_APEX_DATA1           0x32
#define ICM42688_APEX_DATA2           0x33
#define ICM42688_APEX_DATA3           0x34
#define ICM42688_APEX_DATA4           0x35
#define ICM42688_APEX_DATA5           0x36
#define ICM42688_INT_STATUS2          0x37
#define ICM42688_INT_STATUS3          0x38
#define ICM42688_SIGNAL_PATH_RESET    0x4B
#define ICM42688_INTF_CONFIG0         0x4C
#define ICM42688_INTF_CONFIG1         0x4D
#define ICM42688_PWR_MGMT0            0x4E
#define ICM42688_GYRO_CONFIG0         0x4F
#define ICM42688_ACCEL_CONFIG0        0x50
#define ICM42688_GYRO_CONFIG1         0x51
#define ICM42688_GYRO_ACCEL_CONFIG0   0x52
#define ICM42688_ACCEL_CONFIG1        0x53
#define ICM42688_TMST_CONFIG          0x54
#define ICM42688_APEX_CONFIG0         0x56
#define ICM42688_SMD_CONFIG           0x57
#define ICM42688_FIFO_CONFIG1         0x5F
#define ICM42688_FIFO_CONFIG2         0x60
#define ICM42688_FIFO_CONFIG3         0x61
#define ICM42688_FSYNC_CONFIG         0x62
#define ICM42688_INT_CONFIG0          0x63
#define ICM42688_INT_CONFIG1          0x64
#define ICM42688_INT_SOURCE0          0x65
#define ICM42688_INT_SOURCE1          0x66
#define ICM42688_INT_SOURCE3          0x68
#define ICM42688_INT_SOURCE4          0x69
#define ICM42688_FIFO_LOST_PKT0       0x6C
#define ICM42688_FIFO_LOST_PKT1       0x6D
#define ICM42688_SELF_TEST_CONFIG     0x70
#define ICM42688_WHO_AM_I             0x75
#define ICM42688_REG_BANK_SEL         0x76

#define ICM42688_SENSOR_CONFIG0       0x03
#define ICM42688_GYRO_CONFIG_STATIC2  0x0B
#define ICM42688_GYRO_CONFIG_STATIC3  0x0C
#define ICM42688_GYRO_CONFIG_STATIC4  0x0D
#define ICM42688_GYRO_CONFIG_STATIC5  0x0E
#define ICM42688_GYRO_CONFIG_STATIC6  0x0F
#define ICM42688_GYRO_CONFIG_STATIC7  0x10
#define ICM42688_GYRO_CONFIG_STATIC8  0x11
#define ICM42688_GYRO_CONFIG_STATIC9  0x12
#define ICM42688_GYRO_CONFIG_STATIC10 0x13
#define ICM42688_XG_ST_DATA           0x5F
#define ICM42688_YG_ST_DATA           0x60
#define ICM42688_ZG_ST_DATA           0x61
#define ICM42688_TMSTVAL0             0x62
#define ICM42688_TMSTVAL1             0x63
#define ICM42688_TMSTVAL2             0x64
#define ICM42688_INTF_CONFIG4         0x7A
#define ICM42688_INTF_CONFIG5         0x7B
#define ICM42688_INTF_CONFIG6         0x7C

#define ICM42688_ACCEL_CONFIG_STATIC2 0x03
#define ICM42688_ACCEL_CONFIG_STATIC3 0x04
#define ICM42688_ACCEL_CONFIG_STATIC4 0x05
#define ICM42688_XA_ST_DATA           0x3B
#define ICM42688_YA_ST_DATA           0x3C
#define ICM42688_ZA_ST_DATA           0x3D

#define ICM42688_GYRO_ON_OFF_CONFIG   0x0E
#define ICM42688_APEX_CONFIG1         0x40
#define ICM42688_APEX_CONFIG2         0x41
#define ICM42688_APEX_CONFIG3         0x42
#define ICM42688_APEX_CONFIG4         0x43
#define ICM42688_APEX_CONFIG5         0x44
#define ICM42688_APEX_CONFIG6         0x45
#define ICM42688_APEX_CONFIG7         0x46
#define ICM42688_APEX_CONFIG8         0x47
#define ICM42688_APEX_CONFIG9         0x48
#define ICM42688_ACCEL_WOM_X_THR      0x4A
#define ICM42688_ACCEL_WOM_Y_THR      0x4B
#define ICM42688_ACCEL_WOM_Z_THR      0x4C

#define ICM42688_INT_SOURCE6          0x4D
#define ICM42688_INT_SOURCE7          0x4E
#define ICM42688_INT_SOURCE8          0x4F
#define ICM42688_INT_SOURCE9          0x50
#define ICM42688_INT_SOURCE10         0x51

#define ICM42688_OFFSET_USER0         0x77
#define ICM42688_OFFSET_USER1         0x78
#define ICM42688_OFFSET_USER2         0x79
#define ICM42688_OFFSET_USER3         0x7A
#define ICM42688_OFFSET_USER4         0x7B
#define ICM42688_OFFSET_USER5         0x7C
#define ICM42688_OFFSET_USER6         0x7D
#define ICM42688_OFFSET_USER7         0x7E
#define ICM42688_OFFSET_USER8         0x7F

// 温度数据寄存器
#define ICM42688_TEMP_DATA1           0x1D
#define ICM42688_TEMP_DATA0           0x1E

// 加速度计数据寄存器
#define ICM42688_ACCEL_DATA_X1        0x1F
#define ICM42688_ACCEL_DATA_X0        0x20
#define ICM42688_ACCEL_DATA_Y1        0x21
#define ICM42688_ACCEL_DATA_Y0        0x22
#define ICM42688_ACCEL_DATA_Z1        0x23
#define ICM42688_ACCEL_DATA_Z0        0x24

// 陀螺仪数据寄存器
#define ICM42688_GYRO_DATA_X1         0x25
#define ICM42688_GYRO_DATA_X0         0x26
#define ICM42688_GYRO_DATA_Y1         0x27
#define ICM42688_GYRO_DATA_Y0         0x28
#define ICM42688_GYRO_DATA_Z1         0x29
#define ICM42688_GYRO_DATA_Z0         0x2A

// // 加速计量程
typedef enum {
    ICM42688Acc_16G = 0x00, // 默认
    ICM42688Acc_8G = 0x01,
    ICM42688Acc_4G = 0x02,
    ICM42688Acc_2G = 0x03,
} ICM42688_AccRange;

// 加速度计输出速率
typedef enum {
    ICM42688Acc_8000Hz = 0x03,
    ICM42688Acc_4000Hz = 0x04,
    ICM42688Acc_2000Hz = 0x05,
    ICM42688Acc_1000Hz = 0x06, // 默认
    ICM42688Acc_200Hz = 0x07,
    ICM42688Acc_100Hz = 0x08,
    ICM42688Acc_50Hz = 0x09,
    ICM42688Acc_25Hz = 0x0A,
    ICM42688Acc_12_5Hz = 0x0B,
    ICM42688Acc_6_25Hz = 0x0C,
    ICM42688Acc_3_125Hz = 0x0D,
    ICM42688Acc_1_5625Hz = 0x0E,
    ICM42688Acc_500Hz = 0x0F,
} ICM42688_AccFreq;

// 陀螺仪量程
typedef enum {
    ICM42688Gyro_2000DPS = 0x00, // 默认
    ICM42688Gyro_1000DPS = 0x01,
    ICM42688Gyro_500DPS = 0x02,
    ICM42688Gyro_250DPS = 0x03,
    ICM42688Gyro_125DPS = 0x04,
    ICM42688Gyro_62_5DPS = 0x05,
    ICM42688Gyro_31_25DPS = 0x06,
    ICM42688Gyro_15_125DPS = 0x07,
} ICM42688_GyroRange;

// 陀螺仪输出速率
typedef enum {
    ICM42688Gyro_8000Hz = 0x03,
    ICM42688Gyro_4000Hz = 0x04,
    ICM42688Gyro_2000Hz = 0x05,
    ICM42688Gyro_1000Hz = 0x06, // 默认
    ICM42688Gyro_200Hz = 0x07,
    ICM42688Gyro_100Hz = 0x08,
    ICM42688Gyro_50Hz = 0x09,
    ICM42688Gyro_25Hz = 0x0A,
    ICM42688Gyro_12_5Hz = 0x0B,
    ICM42688Gyro_500Hz = 0x0F,
} ICM42688_GyroFreq;

// 传感器ID
#define ICM42688_ID      0x47

// 传感器I2C从机地址
#define ICM42688_ADDRESS 0xD2

typedef struct ICM42688_t {
    GPIOxPiny_t SCLK;
    GPIOxPiny_t MOSI;
    GPIOxPiny_t MISO;
    GPIOxPiny_t CS;
    uint32_t SCLK_ODR;
    uint32_t MISO_IDR;
    uint32_t MOSI_ODR;
    uint32_t CS_ODR;

    SPI_TypeDef *SPIx;
    FunctionalState SPI;

    ICM42688_AccRange AccRange;
    ICM42688_GyroRange GyroRange;
    ICM42688_AccFreq AccFreq;
    ICM42688_GyroFreq GyroFreq;

    float Kp;
    float Ki;

    uint8_t (*SPI_ReadWriteByte)(struct ICM42688_t *Self, uint8_t TxByte);

    float Angles[3];
    float RawAccGyro[6];
    float CalibratedAccGyro[9];

    FlagStatus CalibrationFinished;

    float q[4];
    float AccSensitivity;
    float GyroSensitivity;
    uint32_t LastUpdate, Now;
    float XIntegral, YIntegral, ZIntegral;

    SPI_HandleTypeDef Handel;
} ICM42688_t;

void ICM42688_Init(ICM42688_t *Self);
void ICM42688_GetAccGyro(ICM42688_t *Self, float *AccGyro);
void ICM42688_GetTemperature(ICM42688_t *Self, int16_t *Temperature);

#endif
