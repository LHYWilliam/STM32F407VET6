#ifndef OLED_H
#define OLED_H

#include "stm32f4xx_hal.h"

typedef struct {
    char SCL[4];
    char SDA[4];

    GPIO_TypeDef *SCL_GPIOx;
    uint32_t SCL_GPIO_Pin;
    GPIO_TypeDef *SDA_GPIOx;
    uint32_t SDA_GPIO_Pin;
} OLED;

void OLED_Init(OLED *oled);
void OLED_Clear(OLED *oled);
void OLED_ShowChar(OLED *oled, uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(OLED *oled, uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(OLED *oled, uint8_t Line, uint8_t Column, uint32_t Number,
                  uint8_t Length);
void OLED_ShowSignedNum(OLED *oled, uint8_t Line, uint8_t Column,
                        int32_t Number, uint8_t Length);
void OLED_ShowHexNum(OLED *oled, uint8_t Line, uint8_t Column, uint32_t Number,
                     uint8_t Length);
void OLED_ShowBinNum(OLED *oled, uint8_t Line, uint8_t Column, uint32_t Number,
                     uint8_t Length);
void OLED_W_SCL(OLED *oled, uint8_t x);
void OLED_W_SDA(OLED *oled, uint8_t x);

#endif
