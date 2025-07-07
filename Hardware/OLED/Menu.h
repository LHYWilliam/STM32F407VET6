#ifndef MENU_H
#define MENU_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "OLED.h"

#define PositionUpdate(now, target, step)                                      \
    ((now) += ((now) < (target) ? step : (now) > (target) ? -step : 0))

typedef enum {
    RotationUp,
    RotationDown,
} TextPageRotation;

typedef enum {
    ParameterType_Int,
    ParameterType_Float,
} TextPageParameterType;

typedef struct TextPage {
    char Title[32];

    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    int16_t TitleX;
    int16_t TitleY;
    uint8_t TitleWidth;
    uint8_t TitleHeight;

    uint8_t Cursor;
    uint8_t NumOfLowerPages;
    struct TextPage *LowerPages;
    struct TextPage *UpperPage;

    TextPageParameterType ParameterType;
    union {
        int32_t IntParameter;
        float FloatParameter;
    };
    union {
        int32_t *IntParameterPtr;
        float *FloatParameterPtr;
    };

    void (*ShowCallback)(struct TextPage *TextPage);
    void (*UpdateCallback)(struct TextPage *TextPage);
    void (*ClickCallback)(struct TextPage **TextPage);
    void (*RotationCallback)(struct TextPage *TextPage, TextPageRotation);
} TextPage_t;

typedef struct {
    TextPage_t *Page;
} TextMenu_t;

typedef struct {
    char Title[32];
    const uint8_t *Image;

    int16_t ImageX;
    int16_t ImageY;
    uint8_t ImageWidth;
    uint8_t ImageHeight;

    int16_t TitleX;
    int16_t TitleY;
    uint8_t TitleWidth;
    uint8_t TitleHeight;

    TextPage_t *TextPage;
} ImagePage_t;

typedef struct {
    int16_t X;
    int16_t Y;
    uint8_t Width;
    uint8_t Height;

    int16_t *TargetX;
    int16_t *TargetY;
    uint8_t *TargetWidth;
    uint8_t *TargetHeight;
} SelectioneBar_t;

void TextPage_Init(TextPage_t *Self, OLED_t *OLED);
ErrorStatus TextPage_CursorInc(TextPage_t *Self);
ErrorStatus TextPage_CursorDec(TextPage_t *Self);
ErrorStatus TextPage_EnterLowerPage(TextPage_t **Self);
ErrorStatus TextPage_ReturnUpperPage(TextPage_t **Self);
void TextPage_Reset(TextPage_t *Self);

void TextMenu_Init(TextMenu_t *Self, OLED_t *OLED);

void SelectioneBar_BindTextPage(SelectioneBar_t *Self, TextPage_t *Page);
void SelectioneBar_BindImagePage(SelectioneBar_t *Self, ImagePage_t *Page);
void SelectioneBar_Update(SelectioneBar_t *Self);
void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar);

#endif