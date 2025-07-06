#include "main.h"

void TextPage_CursorCallback(TextPageRotation Direction) {
    switch (Direction) {
    case RotationUp:
        if (TextPage_CursorDec(TextMenu.Page) == SUCCESS) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }
        break;

    case RotationDown:
        if (TextPage_CursorInc(TextMenu.Page) == SUCCESS) {
            SelectioneBar_BindTextPage(
                &Bar, &TextMenu.Page->LowerPages[TextMenu.Page->Cursor]);
        }
        break;
    }
}

// void TextPage_SettingEncoderCallback(int16_t Encoder) {
//     if (Encoder >= 3) {
//         TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting++;

//     } else if (Encoder <= -3) {
//         TextMenu.Page->LowerPages[TextMenu.Page->Cursor].Setting--;
//     }
// }
