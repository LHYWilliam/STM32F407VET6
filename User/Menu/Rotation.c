#include "main.h"

void TextPage_CursorCallback(TextPage_t *TextPage, TextPageRotation Direction) {
    switch (Direction) {
    case RotationUp:
        if (TextPage_CursorDec(TextPage) == SUCCESS) {
            SelectioneBar_BindTextPage(&Bar,
                                       &TextPage->LowerPages[TextPage->Cursor]);
        }
        break;

    case RotationDown:
        if (TextPage_CursorInc(TextPage) == SUCCESS) {
            SelectioneBar_BindTextPage(&Bar,
                                       &TextPage->LowerPages[TextPage->Cursor]);
        }
        break;
    }
}

// void TextPage_SettingEncoderCallback(int16_t Encoder) {
//     if (Encoder >= 3) {
//         TextPage->LowerPages[TextPage->Cursor].Setting++;

//     } else if (Encoder <= -3) {
//         TextPage->LowerPages[TextPage->Cursor].Setting--;
//     }
// }
