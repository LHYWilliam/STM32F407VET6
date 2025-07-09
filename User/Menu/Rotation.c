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
