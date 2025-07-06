#include "main.h"

void TextPage_CursorCallback(int16_t Encoder) {
    if (Encoder >= 3) {
        if (TextPage_CursorInc(TextMenu.Page) == SUCCESS) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }

    } else if (Encoder <= -3) {
        if (TextPage_CursorDec(TextMenu.Page) == SUCCESS) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }
    }
}

// void TextPage_SettingEncoderCallback(int16_t Encoder) {
//     if (Encoder >= 3) {
//         TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting++;

//     } else if (Encoder <= -3) {
//         TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting--;
//     }
// }
