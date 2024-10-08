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
} TextPage;

typedef struct {
    const uint8_t *Image U8X8_PROGMEM;

    TextPage *LowerPages;
} ImagePage;

typedef struct {
    TextPage *Page;

    uint8_t Cursor;
} TextMenu;

typedef struct {
    uint8_t NumOfPages;
    ImagePage *Page;

    uint8_t Cursor;
} ImageMenu;

void TextMenu_Init(TextMenu *self);
void TextPage_Init(TextPage *self);
void U8G2_ShowTextMenu(U8G2 *u8g2, TextMenu *menu);
void TextMenu_CursorInc(TextMenu *self);
void TextMenu_CursorDec(TextMenu *self);
void TextMenu_EnterLowerPage(TextMenu *self);
void TextMenu_ReturnUpperPage(TextMenu *self);

void U8G2_ShowImageMenu(U8G2 *u8g2, ImageMenu *menu, uint16_t delay);
void ImageMenu_CursorInc(ImageMenu *self);
void ImageMenu_CursorDec(ImageMenu *self);

#endif