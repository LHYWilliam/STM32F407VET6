#ifndef MENU_H
#define MENU_H

#include "stm32f4xx_hal.h"

#include <u8g2.h>

#define TEXT_COUNT_OF_PAGE 4

typedef struct TextPage {
    char Title[32];

    uint8_t NumOfLowerPages;
    struct TextPage *LowerPages;
    struct TextPage *UpperPage;
} TextPage_t;

typedef struct {
    const uint8_t *Image U8X8_PROGMEM;

    TextPage_t *LowerPages;
} ImagePage_t;

typedef struct {
    TextPage_t *Page;

    uint8_t Cursor;
} TextMenu_t;

typedef struct {
    uint8_t NumOfPages;
    ImagePage_t *Page;

    uint8_t Cursor;
} ImageMenu_t;

void TextMenu_Init(TextMenu_t *self);
void TextPage_Init(TextPage_t *self);
void U8G2_ShowTextMenu(U8G2_t *u8g2, TextMenu_t *menu);
void TextMenu_CursorInc(TextMenu_t *self);
void TextMenu_CursorDec(TextMenu_t *self);
void TextMenu_EnterLowerPage(TextMenu_t *self);
void TextMenu_ReturnUpperPage(TextMenu_t *self);

void U8G2_ShowImageMenu(U8G2_t *u8g2, ImageMenu_t *menu, uint16_t delay);
void ImageMenu_CursorInc(ImageMenu_t *self);
void ImageMenu_CursorDec(ImageMenu_t *self);

#endif