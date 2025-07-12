#include "Time.h"

#include "GWGray.h"
#include "GWGray_I2C.h"

ErrorStatus GWGray_ScanAddress(GWGray_t *Self);

ErrorStatus GWGray_Init(GWGray_t *Self) {
    Time_Delayms(100);

    GWGray_SWI2C_Init(Self);

    if (GWGray_ScanAddress(Self) == ERROR) {
        return ERROR;
    }

    Time_Delayms(100);

    return SUCCESS;
}

void GWGray_ReadDigital(GWGray_t *Self, uint8_t *Data) {
    uint8_t Byte;
    GWGray_SWI2C_SingedAddrReadBytes(Self, Self->DevAddr << 1,
                                     GW_GRAY_DIGITAL_MODE, &Byte, 1);

    SplitByteToArray(Byte, Data);
}

void GWGray_ReadAnalog(GWGray_t *Self, uint8_t *Data) {
    GWGray_SWI2C_SingedAddrReadBytes(Self, Self->DevAddr << 1,
                                     GW_GRAY_ANALOG_MODE, Data, 8);

    for (uint8_t i = 0; i < 8; i++) {
        Data[i] = 255 - Data[i];
    }
}

#define MAX(a, b)            ((a) > (b) ? (a) : (b))
#define OnLeftGroup(Index)   (0 <= (Index) && (Index) <= 2)
#define OnCenterGroup(Index) (3 <= (Index) && (Index) <= 4)
#define OnRightGroup(Index)  (5 <= (Index) && (Index) <= 7)

int32_t GWGray_CaculateAnalogError(GWGray_t *Self) {
    uint8_t DigitalData[8], AnalogData[8];
    // GWGray_ReadDigital(Self, DigitalData);
    GWGray_ReadAnalog(Self, AnalogData);

    uint8_t OnLineIndex = 8;
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (DigitalData[i] == 0) {
    //         OnLineIndex = i;
    //     }

    //     if (OnCenterGroup(OnLineIndex) || OnRightGroup(OnLineIndex)) {
    //         break;
    //     }
    // }
    uint8_t MaxData = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (AnalogData[i] > MaxData) {
            OnLineIndex = i;
            MaxData = AnalogData[i];
        }
    }

    if (MaxData < 185) {
        return 0xFFFF;
    }

    if (OnLineIndex == 8) {
        return 0xFFFF;
    }

    uint8_t LeftAnalog, RightAnalog;
    if (OnLeftGroup(OnLineIndex)) {
        LeftAnalog = AnalogData[OnLineIndex];
        RightAnalog = AnalogData[OnLineIndex + 1];
    } else if (OnCenterGroup(OnLineIndex)) {
        LeftAnalog = AnalogData[3];
        RightAnalog = AnalogData[4];
    } else if (OnRightGroup(OnLineIndex)) {
        LeftAnalog = AnalogData[OnLineIndex - 1];
        RightAnalog = AnalogData[OnLineIndex];
    }

    int16_t Error = LeftAnalog - RightAnalog;
    if (OnLeftGroup(OnLineIndex)) {
        Error += 128 * (3 - OnLineIndex);
    } else if (OnRightGroup(OnLineIndex)) {
        Error -= 128 * (OnLineIndex - 4);
    }

    return Error;
}

int16_t GWGray_CaculateAnalogError2(GWGray_t *Self) {
    uint8_t DigitalData[8], AnalogData[8];
    GWGray_ReadDigital(Self, DigitalData);
    GWGray_ReadAnalog(Self, AnalogData);

    uint8_t OnLineIndex = 8, CenterIndex = 8;
    for (uint8_t i = 0; i < 8; i++) {
        if (DigitalData[i] == 0) {
            OnLineIndex = i;
        }

        if (OnCenterGroup(OnLineIndex) || OnRightGroup(OnLineIndex)) {
            break;
        }
    }

    if (OnLineIndex == 8) {
        return 0;
    }

    uint8_t LeftAnalog, CenterAnalog, RightAnalog;
    if (OnLeftGroup(OnLineIndex)) {
        CenterIndex = OnLineIndex + 1;
        LeftAnalog = AnalogData[CenterIndex - 1];
        CenterAnalog = AnalogData[CenterIndex];
        RightAnalog = AnalogData[CenterIndex + 1];

    } else if (OnCenterGroup(OnLineIndex)) {
        LeftAnalog = AnalogData[3];
        RightAnalog = AnalogData[4];

    } else if (OnRightGroup(OnLineIndex)) {
        CenterIndex = OnLineIndex - 1;
        LeftAnalog = AnalogData[CenterIndex - 1];
        CenterAnalog = AnalogData[CenterIndex];
        RightAnalog = AnalogData[CenterIndex + 1];
    }

    int16_t Error;
    if (OnCenterGroup(OnLineIndex)) {
        Error = LeftAnalog - RightAnalog;

    } else if (OnLeftGroup(OnLineIndex) || OnRightGroup(OnLineIndex)) {
        uint8_t LeftRightMaxAnalog, WhenLRMCenterAnalog;
        LeftRightMaxAnalog = MAX(Self->LeftRightMaxAnalogs[CenterIndex - 1],
                                 Self->LeftRightMaxAnalogs[CenterIndex + 1]);
        WhenLRMCenterAnalog = Self->WhenLRMCenterAnalogs[CenterIndex];

        if (CenterAnalog > WhenLRMCenterAnalog) {
            Error = LeftAnalog - RightAnalog;

        } else if (LeftAnalog > RightAnalog) {
            Error = (2 * LeftRightMaxAnalog - LeftAnalog) - RightAnalog +
                    255 * (3 - CenterIndex);

        } else if (LeftAnalog < RightAnalog) {
            Error = LeftAnalog - (2 * LeftRightMaxAnalog - RightAnalog) -
                    255 * (CenterIndex - 4);
        }

    } else {
        Error = 0;
    }

    return Error;
}

ErrorStatus GWGray_ScanAddress(GWGray_t *Self) {
    uint8_t Temp;
    for (uint8_t i = 1; i < 127; i++) {
        if (GWGray_SWI2C_NowAddrReadBytes(Self, i << 1, &Temp, 1) == 0 &&
            Self->DevAddr == i) {

            return SUCCESS;
        }
    }

    return ERROR;
}
