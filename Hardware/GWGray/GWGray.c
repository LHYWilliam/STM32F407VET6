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

    for (uint8_t i = 0; i < 8; i++) {
        Data[i] = !Data[i];
    }
}

void GWGray_ReadAnalog(GWGray_t *Self, uint8_t *Data) {
    GWGray_SWI2C_SingedAddrReadBytes(Self, Self->DevAddr << 1,
                                     GW_GRAY_ANALOG_MODE, Data, 8);

    for (uint8_t i = 0; i < 8; i++) {
        Data[i] = 255 - Data[i];
    }
}

void GWGray_Update(GWGray_t *Self) {
    GWGray_ReadDigital(Self, Self->DigitalData);
    GWGray_ReadAnalog(Self, Self->AnalogData);
}

#define MAX(a, b)            ((a) > (b) ? (a) : (b))
#define OnLeftGroup(Index)   (0 <= (Index) && (Index) <= 2)
#define OnCenterGroup(Index) (3 <= (Index) && (Index) <= 4)
#define OnRightGroup(Index)  (5 <= (Index) && (Index) <= 7)

int32_t GWGray_CaculateAnalogError(GWGray_t *Self) {
    uint8_t OnLineIndex = 8;

    {
        for (uint8_t i = 0; i < 8; i++) {
            if (Self->DigitalData[i]) {
                OnLineIndex = i;
            }

            if (OnCenterGroup(OnLineIndex) || OnRightGroup(OnLineIndex)) {
                break;
            }
        }
        if (OnLineIndex == 8) {
            return 0xFFFF;
        }
    }

    {
        // uint8_t MaxData = 0;
        // for (uint8_t i = 0; i < 8; i++) {
        //     if (AnalogData[i] > MaxData) {
        //         OnLineIndex = i;
        //         MaxData = AnalogData[i];
        //     }
        // }
        // static uint8_t LostCount = 0;
        // if (MaxData < 200 || OnLineIndex == 8) {
        //     LostCount++;
        //     if (LostCount >= 1) {
        //         return 0xFFFF;
        //     }
        // } else {
        //     LostCount = 0;
        // }
    }

    uint8_t LeftAnalog, RightAnalog;
    if (OnLeftGroup(OnLineIndex)) {
        LeftAnalog = Self->AnalogData[OnLineIndex];
        RightAnalog = Self->AnalogData[OnLineIndex + 1];
    } else if (OnCenterGroup(OnLineIndex)) {
        LeftAnalog = Self->AnalogData[3];
        RightAnalog = Self->AnalogData[4];
    } else if (OnRightGroup(OnLineIndex)) {
        LeftAnalog = Self->AnalogData[OnLineIndex - 1];
        RightAnalog = Self->AnalogData[OnLineIndex];
    }

    int16_t Error = LeftAnalog - RightAnalog;
    if (OnLeftGroup(OnLineIndex)) {
        Error += 128 * (3 - OnLineIndex);
    } else if (OnRightGroup(OnLineIndex)) {
        Error -= 128 * (OnLineIndex - 4);
    }

    return Error;
}

RoudStatus_t GWGray_GetRoudStatus(GWGray_t *Self) {
    uint8_t LeftOnRoad =
        Self->DigitalData[0] && Self->DigitalData[1] && Self->DigitalData[2];
    uint8_t MidOnRoad = Self->DigitalData[3] || Self->DigitalData[4];
    uint8_t RightOnRoad =
        Self->DigitalData[5] && Self->DigitalData[6] && Self->DigitalData[7];

    uint8_t OnRoad = 0;
    for (uint8_t i = 0; i < 8; i++) {
        OnRoad = OnRoad | Self->DigitalData[i];
    }

    RoudStatus_t RoudStatus =
        (RoudStatus_t)((LeftOnRoad << 2) | (MidOnRoad << 1) | RightOnRoad);
    if (RoudStatus == RoudStatus_DeadEnd && OnRoad) {
        RoudStatus = RoudStatus_OnRoad;
    }

    return RoudStatus;
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
