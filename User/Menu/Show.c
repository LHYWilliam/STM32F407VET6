#include "main.h"

#define ShowTitleAndTexts(...)                                                 \
    if (TextMenu.Page->TitleY + TextMenu.Page->TitleHeight >= 0) {             \
        OLED_Printf(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,       \
                    TextMenu.Page->Title);                                     \
    }                                                                          \
                                                                               \
    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {             \
        if (TextMenu.Page->LowerPages[i].Y +                                   \
                TextMenu.Page->LowerPages[i].Height <                          \
            0) {                                                               \
            continue;                                                          \
        }                                                                      \
        if (TextMenu.Page->LowerPages[i].Y > OLED.Height - 1) {                \
            break;                                                             \
        }                                                                      \
                                                                               \
        __VA_ARGS__                                                            \
    }

void TextPage_ShowCallback(void *pvParameters) {
    ShowTitleAndTexts(OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                                  TextMenu.Page->LowerPages[i].Y, "%s",
                                  TextMenu.Page->LowerPages[i].Title););
}
void TextPage_ShowParameterCallback(void *pvParameters) {
    ShowTitleAndTexts(
        OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                    TextMenu.Page->LowerPages[i].Y, "%s",
                    TextMenu.Page->LowerPages[i].Title);

        if (i > 0) {
            switch (TextMenu.Page->LowerPages[i].ParameterType) {
            case ParameterType_Int:
                OLED_Printf(&OLED, OLED.Width / 2,
                            TextMenu.Page->LowerPages[i].Y, "%d",
                            *TextMenu.Page->LowerPages[i].IntParameterPtr);
                break;

            case ParameterType_Float:
                OLED_Printf(&OLED, OLED.Width / 2,
                            TextMenu.Page->LowerPages[i].Y, "%.2f",
                            *TextMenu.Page->LowerPages[i].FloatParameterPtr);
                break;
            }
        });
}

void TextPage_ShowDialogCallback(void *pvParameters) {
    TextPage_BackCallback(NULL);
    TextMenu.Page->ShowCallback(NULL);
    TextPage_EnterCallback(NULL);

    OLED_ClearBufferArea(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,
                         TextMenu.Page->TitleWidth, TextMenu.Page->TitleHeight);

    OLED_DrawHollowRectangle(&OLED, TextMenu.Page->TitleX,
                             TextMenu.Page->TitleY, TextMenu.Page->TitleWidth,
                             TextMenu.Page->TitleHeight);

    for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {
        if (TextMenu.Page->LowerPages[i].X < TextMenu.Page->TitleX ||
            TextMenu.Page->LowerPages[i].Y < TextMenu.Page->TitleY ||
            TextMenu.Page->LowerPages[i].X +
                    TextMenu.Page->LowerPages[i].Width >
                TextMenu.Page->TitleX + TextMenu.Page->TitleWidth ||
            TextMenu.Page->LowerPages[i].Y +
                    TextMenu.Page->LowerPages[i].Height >
                TextMenu.Page->TitleY + TextMenu.Page->TitleHeight) {
            continue;
        }

        OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
                    TextMenu.Page->LowerPages[i].Y,
                    TextMenu.Page->LowerPages[i].Title);

        if (i == TextMenu.Page->NumOfLowerPages - 1) {
            switch (TextMenu.Page->ParameterType) {
            case ParameterType_Int:
                OLED_Printf(&OLED, OLED.Width / 2 - 7 / 2 * OLED.FontWidth,
                            TextMenu.Page->TitleY + OLED.FontHeight, "%s: %d",
                            TextMenu.Page->Title,
                            *TextMenu.Page->IntParameterPtr);
                break;

            case ParameterType_Float:
                OLED_Printf(&OLED, OLED.Width / 2 - 7 / 2 * OLED.FontWidth,
                            TextMenu.Page->TitleY + OLED.FontHeight, "%s: %.3f",
                            TextMenu.Page->Title,
                            *TextMenu.Page->FloatParameterPtr);
                break;
            }
        }
    }
}
