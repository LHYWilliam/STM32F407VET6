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
} OLED_Handler;

void OLED_Init(OLED_Handler *oled);
void OLED_Clear(OLED_Handler *oled);
void OLED_ShowChar(OLED_Handler *oled, uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(OLED_Handler *oled, uint8_t Line, uint8_t Column,
                     char *String);
void OLED_ShowNum(OLED_Handler *oled, uint8_t Line, uint8_t Column,
                  uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(OLED_Handler *oled, uint8_t Line, uint8_t Column,
                        int32_t Number, uint8_t Length);
void OLED_ShowHexNum(OLED_Handler *oled, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(OLED_Handler *oled, uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length);

#endif
