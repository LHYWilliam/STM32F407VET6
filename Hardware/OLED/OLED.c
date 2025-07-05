#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Time.h"

#include "OLED.h"
#include "OLED_Font.h"
#include "OLED_I2C.h"
#include "OLED_SPI.h"

void OLED_SetCursor(OLED_t *self, uint8_t Page, uint8_t X) {
    self->CursorCommandsBuffer[0] = 0xB0 | Page;
    self->CursorCommandsBuffer[1] = 0x10 | ((X & 0xF0) >> 4);
    self->CursorCommandsBuffer[2] = 0x00 | (X & 0x0F);
    self->OLED_WriteCommands(self, self->CursorCommandsBuffer, 3);
}

void OLED_Fill(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] = 0xFF;
        }
    }
}

void OLED_FillArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                   uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] |= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_Clear(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] = 0x00;
        }
    }
}

void OLED_ClearArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] &= ~(0x01 << (j % 8));
            }
        }
    }
}

void OLED_Reverse(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 0; i < self->Width; i++) {
            self->DisplayBuffer[j][i] ^= 0xFF;
        }
    }
}

void OLED_ReverseArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                      uint8_t Height) {
    for (int16_t j = Y; j < Y + Height; j++) {
        for (int16_t i = X; i < X + Width; i++) {
            if (i >= 0 && i < self->Width && j >= 0 && j < self->Height) {
                self->DisplayBuffer[j / 8][i] ^= 0x01 << (j % 8);
            }
        }
    }
}

void OLED_DrawPoint(OLED_t *self, int16_t X, int16_t Y) {
    if (X >= 0 && X < self->Width && Y >= 0 && Y < self->Height) {
        self->DisplayBuffer[Y / 8][X] |= 0x01 << (Y % 8);
    }
}

void OLED_DrawHLine(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t step) {
    for (uint8_t i = 0; i < Width; i += step) {
        OLED_DrawPoint(self, X + i, Y);
    }
}

void OLED_DrawVLine(OLED_t *self, int16_t X, int16_t Y, uint8_t Height,
                    uint8_t step) {
    for (uint8_t i = 0; i < Height; i += step) {
        OLED_DrawPoint(self, X, Y + i);
    }
}

void OLED_DrawLine(OLED_t *self, int16_t X1, int16_t Y1, int16_t X2,
                   int16_t Y2) {
    uint16_t dx, dy;
    if (X1 > X2) {
        dx = X1 - X2;
    } else {
        dx = X2 - X1;
    }
    if (Y1 > Y2) {
        dy = Y1 - Y2;
    } else {
        dy = Y2 - Y1;
    }

    uint16_t tmp;
    uint8_t swapxy = 0;
    if (dy > dx) {
        swapxy = 1;
        tmp = dx;
        dx = dy;
        dy = tmp;
        tmp = X1;
        X1 = Y1;
        Y1 = tmp;
        tmp = X2;
        X2 = Y2;
        Y2 = tmp;
    }
    if (X1 > X2) {
        tmp = X1;
        X1 = X2;
        X2 = tmp;
        tmp = Y1;
        Y1 = Y2;
        Y2 = tmp;
    }
    int16_t err = dx >> 1;
    int16_t ystep = Y2 > Y1 ? 1 : -1;

    uint16_t y = Y1;

    if (X2 == 255) {
        X2--;
    }

    for (uint16_t x = X1; x <= X2; x++) {
        if (swapxy == 0) {
            OLED_DrawPoint(self, x, y);
        } else {
            OLED_DrawPoint(self, y, x);
        }
        err -= (uint16_t)dy;
        if (err < 0) {
            y += (uint16_t)ystep;
            err += (uint16_t)dx;
        }
    }
}

void OLED_DrawHollowRectangle(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                              uint8_t Height) {
    OLED_DrawHLine(self, X, Y, Width, 1);
    OLED_DrawHLine(self, X, Y + Height - 1, Width, 1);
    OLED_DrawVLine(self, X, Y, Height, 1);
    OLED_DrawVLine(self, X + Width - 1, Y, Height, 1);
}

void OLED_ShowChart(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, uint16_t *Data, uint16_t Length,
                    int16_t Index) {
    Index = (Index + 1) % Length;
    for (uint8_t x = 0; x < self->Width - 1;
         x++, Index = (Index + 1) % Length) {
        OLED_DrawLine(self, OLED_IndexToX(x, Length, X, Width),
                      OLED_ADCToY(Data[Index], Y, Height),
                      OLED_IndexToX(x + 1, Length, X, Width),
                      OLED_ADCToY(Data[(Index + 1) % Length], Y, Height));
    }

    OLED_DrawHLine(self, X, Y, Width, 3);
    OLED_DrawHLine(self, X, Y + Height - 1, Width, 3);
    OLED_DrawVLine(self, X, Y, Height, 3);
    OLED_DrawVLine(self, X + Width - 1, Y, Height, 3);
}

void OLED_ShowImage(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                    uint8_t Height, const uint8_t *Image) {
    OLED_ClearArea(self, X, Y, Width, Height);

    for (uint8_t j = 0; j < (Height - 1) / 8 + 1; j++) {
        for (uint8_t i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i < self->Width) {
                int16_t Page = Y / 8;
                int16_t Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }

                if (Page + j >= 0 && Page + j <= 7) {
                    self->DisplayBuffer[Page + j][X + i] |= Image[j * Width + i]
                                                            << (Shift);
                }

                if (Page + j + 1 >= 0 && Page + j + 1 <= 7) {
                    self->DisplayBuffer[Page + j + 1][X + i] |=
                        Image[j * Width + i] >> (8 - Shift);
                }
            }
        }
    }
}

void OLED_SetFont(OLED_t *self, OLEDFont Font) {
    self->Font = Font;

    switch (self->Font) {
    case OLEDFont_6X8:
        self->FontWidth = 6;
        self->FontHeight = 8;
        break;

    case OLEDFont_8X16:
        self->FontWidth = 8;
        self->FontHeight = 16;
        break;

    case OLEDFont_Chinese12X12:
        self->FontWidth = 12;
        self->FontHeight = 12;
        break;
    }
}

void OLED_ShowChar(OLED_t *self, int16_t X, int16_t Y, char Char) {
    switch (self->Font) {
    case OLEDFont_6X8:
        OLED_ShowImage(self, X, Y, self->FontWidth, self->FontHeight,
                       OLED_Font6x8[Char - ' ']);
        break;

    case OLEDFont_8X16:
        OLED_ShowImage(self, X, Y, self->FontWidth, self->FontHeight,
                       OLED_Font8x16[Char - ' ']);
        break;

    default:
        break;
    }
}

void OLED_ShowString(OLED_t *self, int16_t X, int16_t Y, const char *String) {
    uint8_t ChineseFontLength =
        strlen(OLED_FontChinese12x12_Index) / OLED_ChineseBytesCount;

    for (uint8_t i = 0; String[i];) {

        if (IsChinese(String + i)) {
            uint8_t Index = 0;
            while (strncmp((char *)&String[i],
                           &OLED_FontChinese12x12_Index[Index *
                                                        OLED_ChineseBytesCount],
                           OLED_ChineseBytesCount) != 0 &&
                   ++Index < ChineseFontLength) {
            }

            if (Index == ChineseFontLength) {
                OLEDFont Font = self->Font;
                OLED_SetFont(self, OLEDFont_8X16);
                OLED_ShowChar(self, X, Y, '?');
                X += self->FontWidth;
                OLED_SetFont(self, Font);

            } else {
                OLEDFont Font = self->Font;
                OLED_SetFont(self, OLEDFont_Chinese12X12);
                OLED_ShowImage(self, X, Y, self->FontWidth, self->FontHeight,
                               OLED_FontChinese12x12[Index]);
                X += self->FontWidth;
                OLED_SetFont(self, Font);
            }

            i += OLED_ChineseBytesCount;

        } else {
            OLED_ShowChar(self, X, Y, String[i]);
            X += self->FontWidth;

            i += 1;
        }
    }
}

void OLED_Printf(OLED_t *self, int16_t x, int16_t y, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->PrintfBuffer, format, arg);
    va_end(arg);

    OLED_ShowString(self, x, y, (char *)self->PrintfBuffer);
}

void OLED_ClearBuffer(OLED_t *self) { OLED_Clear(self); }

void OLED_ClearBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                          uint8_t Height) {
    OLED_ClearArea(self, X, Y, Width, Height);
}

void OLED_SendBuffer(OLED_t *self) {
    for (uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(self, j, 0);
        self->OLED_WriteDatas(self, self->DisplayBuffer[j], self->Width);
    }
}

void OLED_SendBufferArea(OLED_t *self, int16_t X, int16_t Y, uint8_t Width,
                         uint8_t Height) {
    int16_t Page = Y / 8;
    int16_t Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) {
        Page -= 1;
        Page1 -= 1;
    }

    for (int16_t j = Page; j < Page1; j++) {
        if (X >= 0 && X < self->Width && j >= 0 && j <= 7) {

            OLED_SetCursor(self, j, X);
            self->OLED_WriteDatas(self, &self->DisplayBuffer[j][X], Width);
        }
    }
}

void OLED_Init(OLED_t *self) {
    if (self->I2C || self->I2Cx) {
        GPIO_t GPIO;
        GPIO.Mode = self->I2Cx ? GPIO_MODE_AF_OD : GPIO_MODE_OUTPUT_OD;
        self->SCL_ODR = GPIO_InitPin(&GPIO, self->SCL);
        self->SDA_ODR = GPIO_InitPin(&GPIO, self->SDA);

        if (self->I2C) {
            GPIO_Write(self->SCL_ODR, 1);
            GPIO_Write(self->SDA_ODR, 1);

            self->OLED_WriteDatas = OLED_SWI2C_WriteDatas;
            self->OLED_WriteCommand = OLED_SWI2C_WriteCommand;
            self->OLED_WriteCommands = OLED_SWI2C_WriteCommands;
        } else if (self->I2Cx) {

            self->OLED_WriteDatas = OLED_HWI2C_WriteDatas;
            self->OLED_WriteCommand = OLED_HWI2C_WriteCommand;
            self->OLED_WriteCommands = OLED_HWI2C_WriteCommands;
        }

        if (self->I2Cx) {
            // RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            // I2C_InitTypeDef I2C_InitStructure;
            // I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
            // I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
            // I2C_InitStructure.I2C_OwnAddress1 = 0x00;
            // I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
            // I2C_InitStructure.I2C_AcknowledgedAddress =
            //     I2C_AcknowledgedAddress_7bit;
            // I2C_InitStructure.I2C_ClockSpeed = 400000;
            // I2C_Init(self->I2Cx, &I2C_InitStructure);

            // I2C_Cmd(self->I2Cx, ENABLE);
        }

    } else if (self->SPI || self->SPIx) {
        GPIO_t GPIO;
        GPIO.Mode = self->SPIx ? GPIO_MODE_AF_PP : GPIO_MODE_OUTPUT_PP;
        self->D0_ODR = GPIO_InitPin(&GPIO, self->D0);
        self->D1_ODR = GPIO_InitPin(&GPIO, self->D1);
        GPIO.Mode = GPIO_MODE_OUTPUT_PP;
        self->RES_ODR = GPIO_InitPin(&GPIO, self->RES);
        self->DC_ODR = GPIO_InitPin(&GPIO, self->DC);
        self->CS_ODR = GPIO_InitPin(&GPIO, self->CS);

        if (self->SPI) {
            GPIO_Write(self->D0, 1);
            GPIO_Write(self->D1, 1);
            GPIO_Write(self->RES_ODR, 1);
            GPIO_Write(self->DC_ODR, 1);
            GPIO_Write(self->CS_ODR, 1);

            self->OLED_WriteDatas = OLED_SWSPI_WriteDatas;
            self->OLED_WriteCommand = OLED_SWSPI_WriteCommand;
            self->OLED_WriteCommands = OLED_SWSPI_WriteCommands;
        } else if (self->SPIx) {

            GPIO_Write(self->RES_ODR, 1);
            GPIO_Write(self->DC_ODR, 1);
            GPIO_Write(self->CS_ODR, 1);

            self->OLED_WriteDatas = OLED_HWSPI_WriteDatas;
            self->OLED_WriteCommand = OLED_HWSPI_WriteCommand;
            self->OLED_WriteCommands = OLED_HWSPI_WriteCommands;
        }

        if (self->SPIx) {
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
            // SPI_Init(self->SPIx, &SPI_InitStructure);

            // SPI_Cmd(self->SPIx, ENABLE);
        }
    }

    if (self->SPI || self->SPIx) {
        GPIO_Write(self->RES_ODR, 0);
    }
    Time_Delayms(100);
    if (self->SPI || self->SPIx) {
        GPIO_Write(self->RES_ODR, 1);
    }

    self->OLED_WriteCommand(self, 0xAE); // 关闭显示

    self->OLED_WriteCommand(self, 0xD5); // 设置时钟
    self->OLED_WriteCommand(self, 0x80);

    self->OLED_WriteCommand(self, 0xA8); // 设置多路复
    self->OLED_WriteCommand(self, 0x3F);

    self->OLED_WriteCommand(self, 0xD3); // 设置显示偏移
    self->OLED_WriteCommand(self, 0x00);

    self->OLED_WriteCommand(self, 0x40); // 设置起始行

    self->OLED_WriteCommand(self, 0xA1); // 设置左右方向

    self->OLED_WriteCommand(self, 0xC8); // 设置上下方向

    self->OLED_WriteCommand(self, 0xDA); // 设置COM硬件引脚配置
    self->OLED_WriteCommand(self, 0x12);

    self->OLED_WriteCommand(self, 0x81); // 调节亮度
    self->OLED_WriteCommand(self, 0xCF);

    self->OLED_WriteCommand(self, 0xD9); // 设置预充电周期
    self->OLED_WriteCommand(self, 0xF1);

    self->OLED_WriteCommand(self, 0xDB); // 设置VCOMH
    self->OLED_WriteCommand(self, 0x30);

    self->OLED_WriteCommand(self, 0xA4); // 全局显示开启

    self->OLED_WriteCommand(self, 0xA6); // 设置显示方式

    self->OLED_WriteCommand(self, 0x8D); // 设置电荷泵
    self->OLED_WriteCommand(self, 0x14);

    self->OLED_WriteCommand(self, 0xAF); // 打开显示

    OLED_Clear(self);
}
