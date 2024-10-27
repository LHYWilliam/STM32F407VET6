#include <stdarg.h>
#include <stdio.h>

#include "Delay.h"
#include "GPIO.h"
#include "LCD.h"
#include "LCD_Font.h"

#define LCD_WriteREG(Reg)  LCD1->LCD_REG = (Reg);

#define LCD_WriteDATA(Ram) LCD1->LCD_RAM = (Ram);

#define LCD_WriteGRAM(lcd) LCD1->LCD_REG = lcd->GRAMCMD;

#define LCD_ReadDATA()     LCD1->LCD_RAM

void LCD_DisplayOn(void) { LCD_WriteREG(0X29); }

void LCD_DisplayOff(void) { LCD_WriteREG(0X28); }

void LCD_SetCursor(LCD_t *self, uint16_t x, uint16_t y) {
    LCD_WriteREG(self->SetXCMD);
    LCD_WriteDATA(x >> 8);
    LCD_WriteDATA(x & 0XFF);

    LCD_WriteREG(self->SetYCMD);
    LCD_WriteDATA(y >> 8);
    LCD_WriteDATA(y & 0XFF);
}

void LCD_SetWindow(LCD_t *self, uint16_t x1, uint16_t y1, uint16_t width,
                   uint16_t height) {
    uint16_t x2 = x1 + width - 1, y2 = y1 + height - 1;

    LCD_WriteREG(self->SetXCMD);
    LCD_WriteDATA(x1 >> 8);
    LCD_WriteDATA(x1 & 0XFF);
    LCD_WriteDATA(x2 >> 8);
    LCD_WriteDATA(x2 & 0XFF);

    LCD_WriteREG(self->SetYCMD);
    LCD_WriteDATA(y1 >> 8);
    LCD_WriteDATA(y1 & 0XFF);
    LCD_WriteDATA(y2 >> 8);
    LCD_WriteDATA(y2 & 0XFF);
}

void LCD_SetDisplayDirection(LCD_t *self) {
    if (self->Direction == LCD_Vertical) {
        self->Width = 320;
        self->Height = 480;
        self->GRAMCMD = 0X2C;
        self->SetXCMD = 0X2A;
        self->SetYCMD = 0X2B;
        self->ScanDirection = Vertical_ScanDirection;
    } else if (self->Direction == LCD_Horizontal) {
        self->Width = 480;
        self->Height = 320;
        self->GRAMCMD = 0X2C;
        self->SetXCMD = 0X2A;
        self->SetYCMD = 0X2B;
        self->ScanDirection = Horizontal_ScanDirection;
    }
}

void LCD_SetScanDirection(LCD_t *self) {
    uint16_t regval = 0;
    switch (self->ScanDirection) {
    case L2R_U2D:
        regval |= (0 << 7) | (0 << 6) | (0 << 5);
        break;

    case L2R_D2U:
        regval |= (1 << 7) | (0 << 6) | (0 << 5);
        break;

    case R2L_U2D:
        regval |= (0 << 7) | (1 << 6) | (0 << 5);
        break;

    case R2L_D2U:
        regval |= (1 << 7) | (1 << 6) | (0 << 5);
        break;

    case U2D_L2R:
        regval |= (0 << 7) | (0 << 6) | (1 << 5);
        break;

    case U2D_R2L:
        regval |= (0 << 7) | (1 << 6) | (1 << 5);
        break;

    case D2U_L2R:
        regval |= (1 << 7) | (0 << 6) | (1 << 5);
        break;

    case D2U_R2L:
        regval |= (1 << 7) | (1 << 6) | (1 << 5);
        break;
    }

    LCD_WriteREG(0X36);
    LCD_WriteDATA(regval);

    uint16_t temp;
    if (regval & 0X20) {
        if (self->Width < self->Height) {
            temp = self->Width;
            self->Width = self->Height;
            self->Height = temp;
        }
    } else {
        if (self->Width > self->Height) {
            temp = self->Width;
            self->Width = self->Height;
            self->Height = temp;
        }
    }
}

void LCD_SetPointColor(LCD_t *self, uint16_t color) {
    self->PointColor = color;
}

void LCD_SetBackColor(LCD_t *self, uint16_t color) { self->BackColor = color; }

void LCD_Clear(LCD_t *self, uint16_t color) {
    LCD_SetWindow(self, 0, 0, self->Width, self->Height);

    LCD_WriteGRAM(self);
    uint32_t totalPoint = self->Width * self->Height;
    for (uint32_t index = 0; index < totalPoint; index++) {
        LCD_WriteDATA(color);
    }
}

void LCD_Fill(LCD_t *self, uint16_t x, uint16_t y, uint16_t width,
              uint16_t height, uint16_t color) {
    LCD_SetWindow(self, x, y, width, height);

    LCD_WriteGRAM(self);
    uint32_t totalPoint = width * height;
    for (uint32_t i = 0; i < totalPoint; i++) {
        LCD_WriteDATA(color);
    }
}

void LCD_DrawPoint(LCD_t *self, uint16_t x, uint16_t y, uint16_t color) {
    LCD_SetCursor(self, x, y);
    LCD_WriteGRAM(self);
    LCD_WriteDATA(color);
}

void LCD_DrawLine(LCD_t *self, uint16_t x1, uint16_t y1, uint16_t x2,
                  uint16_t y2) {
    int32_t xerr = 0, yerr = 0, delta_x = x2 - x1, delta_y = y2 - y1, distance;
    int32_t incx, incy, uRow = x1, uCol = y1;

    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) {
        distance = delta_x;
    } else {
        distance = delta_y;
    }

    for (uint16_t t = 0; t <= distance + 1; t++) {
        LCD_DrawPoint(self, uRow, uCol, self->PointColor);

        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void LCD_DrawRectangle(LCD_t *self, uint16_t x1, uint16_t y1, uint16_t x2,
                       uint16_t y2) {
    LCD_DrawLine(self, x1, y1, x2, y1);
    LCD_DrawLine(self, x1, y1, x1, y2);
    LCD_DrawLine(self, x1, y2, x2, y2);
    LCD_DrawLine(self, x2, y1, x2, y2);
}

void LCD_DrawCircle(LCD_t *self, uint16_t x0, uint16_t y0, uint8_t r) {
    int32_t a = 0, b = r, di = 3 - (r << 1);

    while (a <= b) {
        LCD_DrawPoint(self, x0 + a, y0 - b, self->PointColor);
        LCD_DrawPoint(self, x0 + b, y0 - a, self->PointColor);
        LCD_DrawPoint(self, x0 + b, y0 + a, self->PointColor);
        LCD_DrawPoint(self, x0 + a, y0 + b, self->PointColor);
        LCD_DrawPoint(self, x0 - a, y0 + b, self->PointColor);
        LCD_DrawPoint(self, x0 - b, y0 + a, self->PointColor);
        LCD_DrawPoint(self, x0 - a, y0 - b, self->PointColor);
        LCD_DrawPoint(self, x0 - b, y0 - a, self->PointColor);
        a++;

        if (di < 0) {
            di += 4 * a + 6;
        } else {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

void LCD_ShowImage(LCD_t *self, uint16_t x, uint16_t y, uint16_t width,
                   uint16_t height, const uint8_t *image) {
    LCD_SetWindow(self, x, y, width, height);
    LCD_WriteGRAM(self);

    if (self->DMA.DMAx) {
        uint32_t restPoint = width * height, alreadyPoint = 0;
        do {
            uint32_t pointToShow = restPoint >= 65535 ? 65535 : restPoint;

            HAL_DMA_Start(&self->DMA.Handler,
                          (uint32_t)(image + alreadyPoint * 2),
                          (uint32_t)&LCD1->LCD_RAM, pointToShow);
            while (HAL_DMA_PollForTransfer(&self->DMA.Handler,
                                           HAL_DMA_FULL_TRANSFER,
                                           HAL_MAX_DELAY) != HAL_OK)
                ;

            restPoint -= pointToShow;
            alreadyPoint += pointToShow;
        } while (restPoint);

    } else {
        uint32_t totalPoint = width * height;
        for (uint32_t i = 0; i < totalPoint; i++) {
            LCD_WriteDATA((image[2 * i + 1] << 8) | image[2 * i]);
        }
    }
}

static void opt_delay(uint8_t i) {
    while (i--)
        ;
}

uint16_t LCD_ReadPoint(LCD_t *self, uint16_t x, uint16_t y) {
    uint16_t r = 0, g = 0, b = 0;
    if (x >= self->Width || y >= self->Height)
        return 0;
    LCD_SetCursor(self, x, y);

    LCD_WriteREG(0X2E);

    r = LCD_ReadDATA();

    opt_delay(2);
    r = LCD_ReadDATA();

    opt_delay(2);
    b = LCD_ReadDATA();
    g = r & 0XFF;

    g <<= 8;

    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
}

void LCD_ShowChar(LCD_t *self, uint16_t x, uint16_t y, uint8_t num,
                  uint8_t size, uint8_t mode) {
    uint16_t y0 = y;
    num = num - ' ';

    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    for (uint8_t t = 0; t < csize; t++) {
        uint8_t temp;
        if (size == 12) {
            temp = asc2_1206[num][t];
        } else if (size == 16) {
            temp = asc2_1608[num][t];
        } else if (size == 24) {
            temp = asc2_2412[num][t];
        } else if (size == 32) {
            temp = asc2_3216[num][t];
        } else {
            return;
        }

        for (uint8_t t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80) {
                LCD_DrawPoint(self, x, y, self->PointColor);
            } else if (mode == 0) {
                LCD_DrawPoint(self, x, y, self->BackColor);
            }

            temp <<= 1;
            y++;

            if (y >= self->Height) {
                return;
            }

            if ((y - y0) == size) {
                y = y0;
                x++;

                if (x >= self->Width) {
                    return;
                }

                break;
            }
        }
    }
}

uint32_t LCD_Pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--) {
        result *= m;
    }

    return result;
}

void LCD_ShowNum(LCD_t *self, uint16_t x, uint16_t y, uint32_t num,
                 uint8_t length, uint8_t size) {
    uint8_t temp, enshow = 0;
    for (uint8_t t = 0; t < length; t++) {
        temp = (num / LCD_Pow(10, length - t - 1)) % 10;

        if (enshow == 0 && t < (length - 1)) {
            if (temp == 0) {
                LCD_ShowChar(self, x + (size / 2) * t, y, ' ', size, 0);
                continue;
            } else {
                enshow = 1;
            }
        }

        LCD_ShowChar(self, x + (size / 2) * t, y, temp + '0', size, 0);
    }
}

void LCD_ShowxNum(LCD_t *self, uint16_t x, uint16_t y, uint32_t num,
                  uint8_t length, uint8_t size, uint8_t mode) {
    uint8_t temp, enshow = 0;
    for (uint8_t t = 0; t < length; t++) {
        temp = (num / LCD_Pow(10, length - t - 1)) % 10;
        if (enshow == 0 && t < (length - 1)) {
            if (temp == 0) {
                if (mode & 0X80) {
                    LCD_ShowChar(self, x + (size / 2) * t, y, '0', size,
                                 mode & 0X01);
                } else {
                    LCD_ShowChar(self, x + (size / 2) * t, y, ' ', size,
                                 mode & 0X01);
                }
                continue;
            } else {
                enshow = 1;
            }
        }
        LCD_ShowChar(self, x + (size / 2) * t, y, temp + '0', size,
                     mode & 0X01);
    }
}

void LCD_ShowString(LCD_t *self, uint16_t x, uint16_t y, uint16_t width,
                    uint16_t height, uint8_t size, char *string) {
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*string <= '~') && (*string >= ' ')) {
        if (x >= width) {
            x = x0;
            y += size;
        }

        if (y >= height) {
            break;
        }

        LCD_ShowChar(self, x, y, *string, size, 0);
        x += size / 2;
        string++;
    }
}

void LCD_Printf(LCD_t *self, uint16_t x, uint16_t y, uint16_t width,
                uint16_t height, uint8_t size, char *format, ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)self->Buffer, format, arg);
    va_end(arg);

    LCD_ShowString(self, x, y, width, height, size, (char *)self->Buffer);
}

void LCD_Init(LCD_t *self) {
    GPIO_t LCD_RST_ = {
        .GPIOxPiny = "D11 ",
        .Pull = GPIO_PULLUP,
        .Mode = GPIO_MODE_OUTPUT_PP,
    };
    GPIO_Init(&LCD_RST_);

    GPIO_t LCD_LED_ = {
        .GPIOxPiny = "D12",
        .Pull = GPIO_PULLUP,
        .Mode = GPIO_MODE_OUTPUT_PP,
    };
    GPIO_Init(&LCD_LED_);

    if (self->DMA.DMAx) {
        self->DMA.PeriphSize = 16;
        self->DMA.PeriphInc = ENABLE;
        self->DMA.MemSize = 16;
        self->DMA.MemInc = DISABLE;
        self->DMA.Mode = DMA_NORMAL;
        self->DMA.Direction = DMA_MEMORY_TO_MEMORY;
        DMA_Init(&self->DMA);
    }

    SRAM_HandleTypeDef TFTSRAM_Handler = {
        .Instance = FSMC_NORSRAM_DEVICE,
        .Extended = FSMC_NORSRAM_EXTENDED_DEVICE,
        .Init =
            {
                .MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16,
                .WriteOperation = FSMC_WRITE_OPERATION_ENABLE,
                .ExtendedMode = FSMC_EXTENDED_MODE_ENABLE,
                .ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC,
            },
    };

    FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim = {
        .AddressSetupTime = 0x0F,
        .AddressHoldTime = 0,
        .DataSetupTime = 60,
    };

    FSMC_NORSRAM_TimingTypeDef FSMC_WriteTim = {
        .BusTurnAroundDuration = 0,
        .AddressSetupTime = 9,
        .AddressHoldTime = 0,
        .DataSetupTime = 8,
    };

    HAL_SRAM_Init(&TFTSRAM_Handler, &FSMC_ReadWriteTim, &FSMC_WriteTim);

    Delay_ms(50);

    LCD_RST = 1;
    Delay_ms(10);
    LCD_RST = 0;
    Delay_ms(50);
    LCD_RST = 1;
    Delay_ms(200);

    LCD_WriteREG(0XD3);
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID <<= 8;
    self->ID |= LCD_ReadDATA();

    LCD_WriteREG(0X04);
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID <<= 8;
    self->ID |= LCD_ReadDATA() & 0XFF;

    LCD_WriteREG(0XD4);
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID = LCD_ReadDATA();
    self->ID <<= 8;
    self->ID |= LCD_ReadDATA();

    LCD_WriteREG(0xED);
    LCD_WriteDATA(0x01);
    LCD_WriteDATA(0xFE);

    LCD_WriteREG(0xEE);
    LCD_WriteDATA(0xDE);
    LCD_WriteDATA(0x21);

    LCD_WriteREG(0xF1);
    LCD_WriteDATA(0x01);
    LCD_WriteREG(0xDF);
    LCD_WriteDATA(0x10);

    LCD_WriteREG(0xC4);
    LCD_WriteDATA(0x8F);

    LCD_WriteREG(0xC6);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xE2);
    LCD_WriteDATA(0xE2);
    LCD_WriteDATA(0xE2);
    LCD_WriteREG(0xBF);
    LCD_WriteDATA(0xAA);

    LCD_WriteREG(0xB0);
    LCD_WriteDATA(0x0D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x0D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x11);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x19);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x21);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x5D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x5D);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB1);
    LCD_WriteDATA(0x80);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x8B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x96);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x02);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x03);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB4);
    LCD_WriteDATA(0x8B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x96);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA1);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB5);
    LCD_WriteDATA(0x02);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x03);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x04);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB6);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3F);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x5E);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x64);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x8C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xAC);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDC);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x70);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x90);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xEB);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDC);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xB8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xBA);
    LCD_WriteDATA(0x24);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC1);
    LCD_WriteDATA(0x20);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x54);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xFF);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC2);
    LCD_WriteDATA(0x0A);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x04);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC3);
    LCD_WriteDATA(0x3C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3A);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x39);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x37);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x36);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x32);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2F);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x29);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x26);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x24);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x24);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x23);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x36);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x32);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2F);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x29);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x26);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x24);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x24);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x23);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC4);
    LCD_WriteDATA(0x62);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x05);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x84);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF0);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x18);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA4);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x18);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x50);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x0C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x17);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x95);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xE6);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC5);
    LCD_WriteDATA(0x32);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x65);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x76);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC6);
    LCD_WriteDATA(0x20);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x17);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x01);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xC9);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE0);
    LCD_WriteDATA(0x16);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x1C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x21);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x36);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x46);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x52);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x64);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x7A);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x8B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB9);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC4);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xCA);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD9);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xE0);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE1);
    LCD_WriteDATA(0x16);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x1C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x22);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x36);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x45);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x52);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x64);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x7A);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x8B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB9);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC4);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xCA);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xE0);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE2);
    LCD_WriteDATA(0x05);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x0B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x1B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x34);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x4F);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x61);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x79);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x97);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA6);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD1);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD6);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDD);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);
    LCD_WriteREG(0xE3);
    LCD_WriteDATA(0x05);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x1C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x33);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x50);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x62);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x78);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x97);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA6);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC7);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD1);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD5);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDD);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE4);
    LCD_WriteDATA(0x01);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x01);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x02);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x2A);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x4B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x5D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x74);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x84);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x93);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xBE);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC4);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xCD);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDD);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);
    LCD_WriteREG(0xE5);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x02);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x29);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x3C);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x4B);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x5D);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x74);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x84);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x93);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xA2);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xB3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xBE);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xC4);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xCD);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xD3);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xDC);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xF3);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE6);
    LCD_WriteDATA(0x11);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x34);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x56);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x76);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x77);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x66);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xBB);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x66);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x45);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x43);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE7);
    LCD_WriteDATA(0x32);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x76);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x66);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x67);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x67);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x87);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xBB);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x77);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x56);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x23);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x33);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x45);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE8);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x87);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x77);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x66);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x88);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xAA);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0xBB);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x99);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x66);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x44);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x55);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xE9);
    LCD_WriteDATA(0xAA);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0x00);
    LCD_WriteDATA(0xAA);

    LCD_WriteREG(0xCF);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xF0);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x50);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xF3);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0xF9);
    LCD_WriteDATA(0x06);
    LCD_WriteDATA(0x10);
    LCD_WriteDATA(0x29);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0x3A);
    LCD_WriteDATA(0x55);

    LCD_WriteREG(0x11);
    Delay_ms(100);
    LCD_WriteREG(0x29);
    LCD_WriteREG(0x35);
    LCD_WriteDATA(0x00);

    LCD_WriteREG(0x51);
    LCD_WriteDATA(0xFF);
    LCD_WriteREG(0x53);
    LCD_WriteDATA(0x2C);
    LCD_WriteREG(0x55);
    LCD_WriteDATA(0x82);
    LCD_WriteREG(0x2c);

    FSMC_Bank1E->BWTR[0] &= ~(0XF << 0);
    FSMC_Bank1E->BWTR[0] &= ~(0XF << 8);
    FSMC_Bank1E->BWTR[0] |= 3 << 0;

    FSMC_Bank1E->BWTR[0] |= 2 << 8;

    LCD_SetDisplayDirection(self);
    LCD_SetScanDirection(self);
    LCD_Clear(self, WHITE);
    LCD_SetBackColor(self, WHITE);
    LCD_SetPointColor(self, BLACK);

    LCD_LED = 1;
}
