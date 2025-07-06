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

// void TextPage_ShowSettingCallback(void *pvParameters) {
//     ShowTitleAndTexts(
//         OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
//                     TextMenu.Page->LowerPages[i].Y, "%s",
//                     TextMenu.Page->LowerPages[i].Title);

//         if (TextMenu.Page->LowerPages[i].ClickCallback ==
//             TextPage_ReverseSettingCallback) {
//             OLED_ShowImage(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
//                            TextMenu.Page->LowerPages[i].Y, 8, 8,
//                            SettingImage[TextMenu.Page->LowerPages[i].Setting]);
//         }

//         if (TextMenu.Page->LowerPages[i].ClickCallback ==
//             TextPage_CursorSwitchEncoderSettingCallback) {
//             OLED_Printf(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 - 8,
//                         TextMenu.Page->LowerPages[i].Y, "%d",
//                         TextMenu.Page->LowerPages[i].Setting);

//             if (TextMenu.Page->LowerPages[i].RotationCallback ==
//                 TextPage_SettingEncoderCallback) {
//                 uint8_t number;
//                 if (TextMenu.Page->LowerPages[i].Setting == 0) {
//                     number = 1;
//                 } else {
//                     number = (uint8_t)log10(
//                                  abs(TextMenu.Page->LowerPages[i].Setting)) +
//                              1;
//                 }
//                 number += TextMenu.Page->LowerPages[i].Setting < 0 ? 1 : 0;

//                 OLED_DrawHLine(&OLED, OLED.Width - 1 - OLED.FontWidth * 6 -
//                 8,
//                                TextMenu.Page->LowerPages[i].Y +
//                                OLED.FontHeight, OLED.FontWidth * number, 1);
//             }
//         });
// }

// void TextPage_ShowDialogCallback(void *pvParameters) {
//     TextPage_BackCallback(NULL);
//     TextMenu.Page->ShowCallback(NULL);
//     TextPage_EnterCallback(NULL);

//     OLED_ClearBufferArea(&OLED, TextMenu.Page->TitleX, TextMenu.Page->TitleY,
//                          TextMenu.Page->TitleWidth,
//                          TextMenu.Page->TitleHeight);

//     OLED_DrawHollowRectangle(&OLED, TextMenu.Page->TitleX,
//                              TextMenu.Page->TitleY,
//                              TextMenu.Page->TitleWidth,
//                              TextMenu.Page->TitleHeight);

//     for (uint8_t i = 0; i < TextMenu.Page->NumOfLowerPages; i++) {
//         if (TextMenu.Page->LowerPages[i].X < TextMenu.Page->TitleX ||
//             TextMenu.Page->LowerPages[i].Y < TextMenu.Page->TitleY ||
//             TextMenu.Page->LowerPages[i].X +
//                     TextMenu.Page->LowerPages[i].Width >
//                 TextMenu.Page->TitleX + TextMenu.Page->TitleWidth ||
//             TextMenu.Page->LowerPages[i].Y +
//                     TextMenu.Page->LowerPages[i].Height >
//                 TextMenu.Page->TitleY + TextMenu.Page->TitleHeight) {
//             continue;
//         }

//         OLED_Printf(&OLED, TextMenu.Page->LowerPages[i].X,
//                     TextMenu.Page->LowerPages[i].Y,
//                     TextMenu.Page->LowerPages[i].Title);

//         if (i == TextMenu.Page->NumOfLowerPages - 1) {
//             OLED_Printf(&OLED, OLED.Width / 2 - 7 / 2 * OLED.FontWidth,
//                         TextMenu.Page->TitleY + OLED.FontHeight, "Success");
//         }
//     }
// }
