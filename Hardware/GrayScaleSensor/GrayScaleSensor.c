#include "Time.h"

#include "GrayScaleSensor.h"
#include "GrayScaleSensor_I2C.h"

ErrorStatus GrayScaleSensor_ScanAddress(GrayScaleSensor_t *Self);

ErrorStatus GrayScaleSensor_Init(GrayScaleSensor_t *Self) {
    Time_Delayms(100);

    GraySacleSensor_SWI2C_Init(Self);

    if (GrayScaleSensor_ScanAddress(Self) == ERROR) {
        return ERROR;
    }

    Time_Delayms(100);

    return SUCCESS;
}

void GrayScaleSensor_ReadDigital(GrayScaleSensor_t *Self, uint8_t *Data) {
    uint8_t Byte;
    GraySacleSensor_SWI2C_SingedAddrReadBytes(Self, Self->DevAddr << 1,
                                              GW_GRAY_DIGITAL_MODE, &Byte, 1);

    SplitByteToArray(Byte, Data);
}

void GrayScaleSensor_ReadAnalog(GrayScaleSensor_t *Self, uint8_t *Data) {
    GraySacleSensor_SWI2C_SingedAddrReadBytes(Self, Self->DevAddr << 1,
                                              GW_GRAY_ANALOG_MODE, Data, 8);
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int16_t GrayScaleSensor_CaculateAnalogError(GrayScaleSensor_t *Self) {
    uint8_t DigitalData[8], AnalogData[8];
    GrayScaleSensor_ReadDigital(Self, DigitalData);
    GrayScaleSensor_ReadAnalog(Self, AnalogData);

    uint8_t OnLineIndex = 8, CenterIndex = 8;
    for (uint8_t i = 0; i < 8; i++) {
        if (DigitalData[i] == 0) {
            OnLineIndex = i;
        }

        if (2 < OnLineIndex && OnLineIndex < 8) {
            break;
        }
    }

    if (OnLineIndex == 8) {
        return 0;
    }

    uint8_t LeftAnalog, CenterAnalog, RightAnalog;
    if (0 <= OnLineIndex && OnLineIndex <= 2) {
        CenterIndex = OnLineIndex + 1;
        LeftAnalog = AnalogData[CenterIndex - 1];
        CenterAnalog = AnalogData[CenterIndex];
        RightAnalog = AnalogData[CenterIndex + 1];

    } else if (3 <= OnLineIndex && OnLineIndex <= 4) {
        LeftAnalog = AnalogData[3];
        RightAnalog = AnalogData[4];

    } else if (5 <= OnLineIndex && OnLineIndex <= 7) {
        CenterIndex = OnLineIndex - 1;
        LeftAnalog = AnalogData[CenterIndex - 1];
        CenterAnalog = AnalogData[CenterIndex];
        RightAnalog = AnalogData[CenterIndex + 1];
    }

    int16_t Error;
    if (3 <= OnLineIndex && OnLineIndex <= 4) {
        Error = LeftAnalog - RightAnalog;

    } else if ((0 <= OnLineIndex && OnLineIndex <= 2) ||
               (5 <= OnLineIndex && OnLineIndex <= 7)) {
        uint8_t LeftRightMaxAnalog, WhenLRMCenterAnalog;
        LeftRightMaxAnalog = MAX(Self->LeftRightMaxAnalogs[CenterIndex - 1],
                                 Self->LeftRightMaxAnalogs[CenterIndex + 1]);
        WhenLRMCenterAnalog = Self->WhenLRMCenterAnalogs[CenterIndex];

        if (CenterAnalog > WhenLRMCenterAnalog) {
            Error = LeftAnalog - RightAnalog;

        } else if (LeftAnalog > RightAnalog) {
            Error = (2 * LeftRightMaxAnalog - LeftAnalog) - RightAnalog;

        } else if (LeftAnalog < RightAnalog) {
            Error = LeftAnalog - (2 * LeftRightMaxAnalog - RightAnalog);
        }

    } else {
        Error = 0;
    }

    return Error;
}

ErrorStatus GrayScaleSensor_ScanAddress(GrayScaleSensor_t *Self) {
    uint8_t Temp;
    for (uint8_t i = 1; i < 127; i++) {
        if (GraySacleSensor_SWI2C_NowAddrReadBytes(Self, i << 1, &Temp, 1) ==
                0 &&
            Self->DevAddr == i) {

            return SUCCESS;
        }
    }

    return ERROR;
}
