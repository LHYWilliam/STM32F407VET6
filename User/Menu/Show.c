#include "main.h"

#define ShowTitleAndTexts(...)                                                 \
    if (TextPage->TitleY + TextPage->TitleHeight >= 0) {                       \
        OLED_Printf(&OLED, TextPage->TitleX, TextPage->TitleY,                 \
                    TextPage->Title);                                          \
    }                                                                          \
                                                                               \
    for (uint8_t i = 0; i < TextPage->NumOfLowerPages; i++) {                  \
        if (TextPage->LowerPages[i].Y + TextPage->LowerPages[i].Height < 0) {  \
            continue;                                                          \
        }                                                                      \
        if (TextPage->LowerPages[i].Y > OLED.Height - 1) {                     \
            break;                                                             \
        }                                                                      \
                                                                               \
        __VA_ARGS__                                                            \
    }

void TextPage_ShowCallback(TextPage_t *TextPage) {
    ShowTitleAndTexts(OLED_Printf(&OLED, TextPage->LowerPages[i].X,
                                  TextPage->LowerPages[i].Y, "%s",
                                  TextPage->LowerPages[i].Title););
}
void TextPage_ShowParameterCallback(TextPage_t *TextPage) {
    ShowTitleAndTexts(
        OLED_Printf(&OLED, TextPage->LowerPages[i].X, TextPage->LowerPages[i].Y,
                    "%s", TextPage->LowerPages[i].Title);

        if (i > 0) {
            switch (TextPage->LowerPages[i].ParameterType) {
            case Int:
                OLED_Printf(&OLED, OLED.Width / 2, TextPage->LowerPages[i].Y,
                            "%d", *TextPage->LowerPages[i].IntParameterPtr);
                break;

            case Float:
                OLED_Printf(&OLED, OLED.Width / 2, TextPage->LowerPages[i].Y,
                            "%.2f", *TextPage->LowerPages[i].FloatParameterPtr);
                break;
            }
        });
}

void TextPage_ShowDialogCallback(TextPage_t *TextPage) {
    TextPage->UpperPage->ShowCallback(TextPage->UpperPage);

    OLED_ClearBufferArea(&OLED, TextPage->TitleX, TextPage->TitleY,
                         TextPage->TitleWidth, TextPage->TitleHeight);

    OLED_DrawHollowRectangle(&OLED, TextPage->TitleX, TextPage->TitleY,
                             TextPage->TitleWidth, TextPage->TitleHeight);

    for (uint8_t i = 0; i < TextPage->NumOfLowerPages; i++) {
        if (TextPage->LowerPages[i].X < TextPage->TitleX ||
            TextPage->LowerPages[i].Y < TextPage->TitleY ||
            TextPage->LowerPages[i].X + TextPage->LowerPages[i].Width >
                TextPage->TitleX + TextPage->TitleWidth ||
            TextPage->LowerPages[i].Y + TextPage->LowerPages[i].Height >
                TextPage->TitleY + TextPage->TitleHeight) {
            continue;
        }

        OLED_Printf(&OLED, TextPage->LowerPages[i].X, TextPage->LowerPages[i].Y,
                    TextPage->LowerPages[i].Title);
        OLED_DrawHollowRectangle(&OLED, TextPage->LowerPages[i].X - 2,
                                 TextPage->LowerPages[i].Y - 2,
                                 TextPage->LowerPages[i].Width + 4,
                                 TextPage->LowerPages[i].Height + 4);

        if (i == TextPage->NumOfLowerPages - 1) {
            switch (TextPage->ParameterType) {
            case Int:
                OLED_Printf(&OLED,
                            TextPage->LowerPages[0].X +
                                TextPage->LowerPages[0].Width + OLED.FontWidth,
                            TextPage->TitleY + OLED.FontHeight, "%s: %d",
                            TextPage->Title, *TextPage->IntParameterPtr);
                OLED_Printf(&OLED,
                            TextPage->LowerPages[0].X +
                                TextPage->LowerPages[0].Width + OLED.FontWidth,
                            TextPage->TitleY + OLED.FontHeight * 3, "Step: %d",
                            TextPage->IntParameter);
                break;

            case Float:
                OLED_Printf(&OLED,
                            TextPage->LowerPages[0].X +
                                TextPage->LowerPages[0].Width + OLED.FontWidth,
                            TextPage->TitleY + OLED.FontHeight, "%s: %g",
                            TextPage->Title, *TextPage->FloatParameterPtr);
                OLED_Printf(&OLED,
                            TextPage->LowerPages[0].X +
                                TextPage->LowerPages[0].Width + OLED.FontWidth,
                            TextPage->TitleY + OLED.FontHeight * 3, "Step: %+g",
                            TextPage->FloatParameter);
                break;
            }
        }
    }
}

void TextPage_ShowOptionCallback(TextPage_t *TextPage) {
    ShowTitleAndTexts(
        OLED_Printf(&OLED, TextPage->LowerPages[i].X, TextPage->LowerPages[i].Y,
                    "%s", TextPage->LowerPages[i].Title);
        if (i > 0) {
            OLED_DrawHollowRectangle(&OLED, OLED.Width - 16,
                                     TextMenu.Page->LowerPages[i].Y, 8, 8);
            if (*TextPage->IntParameterPtr == i) {
                OLED_DrawSolidRectangle(&OLED, OLED.Width - 16 + 2,
                                        TextMenu.Page->LowerPages[i].Y + 2, 4,
                                        4);
            }
        });
}
