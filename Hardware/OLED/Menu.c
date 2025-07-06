#include <string.h>

#include "Menu.h"
#include "OLED.h"

void EmptyCallbackPtr(struct TextPage *TextPage) {}
void EmptyCallbackPtrPtr(struct TextPage **TextPage) {}
void EmptyCallbackRotation(struct TextPage *TextPage,
                           TextPageRotation Direction) {}

void TextPage_Init(TextPage_t *Self, OLED_t *OLED) {
    if (Self->TitleX == 0) {
        Self->TitleX =
            OLED->Width / 2 - strlen(Self->Title) * OLED->FontWidth / 2;
    }
    if (Self->TitleWidth == 0) {
        Self->TitleWidth = strlen(Self->Title) * OLED->FontWidth;
    }
    if (Self->TitleHeight == 0) {
        Self->TitleHeight = OLED->FontHeight * 2;
    }

    for (uint8_t i = 0; i < Self->NumOfLowerPages; i++) {
        Self->LowerPages[i].Width =
            strlen(Self->LowerPages[i].Title) * OLED->FontWidth;
        Self->LowerPages[i].Height = OLED->FontHeight;

        if (i == 0) {
            Self->LowerPages[i].UpperPage = Self->UpperPage;

        } else {
            Self->LowerPages[i].UpperPage = Self;
        }

        if (Self->LowerPages[i].UpdateCallback == NULL) {
            Self->LowerPages[i].UpdateCallback = EmptyCallbackPtr;
        }
        if (Self->LowerPages[i].ShowCallback == NULL) {
            Self->LowerPages[i].ShowCallback = EmptyCallbackPtr;
        }
        if (Self->LowerPages[i].ClickCallback == NULL) {
            Self->LowerPages[i].ClickCallback = EmptyCallbackPtrPtr;
        }
        if (Self->LowerPages[i].RotationCallback == NULL) {
            Self->LowerPages[i].RotationCallback = EmptyCallbackRotation;
        }

        TextPage_Init(&Self->LowerPages[i], OLED);
    }
}

void TextPage_Reset(TextPage_t *Self) {
    for (uint8_t i = Self->Cursor; i < Self->NumOfLowerPages; i++) {
        Self->LowerPages[i].Y = 0;
    }
    for (int8_t i = Self->Cursor - 1; i >= 0; i--) {
        Self->LowerPages[i].Y =
            Self->LowerPages[i + 1].Y - Self->LowerPages[i].Height - 2;
    }
}

void TextMenu_Init(TextMenu_t *Self, OLED_t *OLED) {
    if (Self->Page != NULL) {
        TextPage_Init(Self->Page, OLED);
    }
}

ErrorStatus TextPage_CursorInc(TextPage_t *Self) {
    if (Self->NumOfLowerPages >= 2) {
        Self->Cursor = (Self->Cursor + 1) % Self->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_CursorDec(TextPage_t *Self) {
    if (Self->NumOfLowerPages >= 2) {
        Self->Cursor =
            (Self->Cursor + Self->NumOfLowerPages - 1) % Self->NumOfLowerPages;

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_EnterLowerPage(TextPage_t **Self) {
    if ((*Self)->LowerPages[(*Self)->Cursor].NumOfLowerPages != 0) {
        *Self = &(*Self)->LowerPages[(*Self)->Cursor];

        return SUCCESS;
    }

    return ERROR;
}

ErrorStatus TextPage_ReturnUpperPage(TextPage_t **Self) {
    if ((*Self)->UpperPage != NULL) {
        *Self = (*Self)->UpperPage;

        return SUCCESS;
    }

    return ERROR;
}

void SelectioneBar_BindTextPage(SelectioneBar_t *Self, TextPage_t *Page) {
    Self->TargetX = &Page->X;
    Self->TargetY = &Page->Y;
    Self->TargetWidth = &Page->Width;
    Self->TargetHeight = &Page->Height;
}

void SelectioneBar_Update(SelectioneBar_t *Self) {
    PositionUpdate(Self->X, *Self->TargetX - 1, 1);
    PositionUpdate(Self->Y, *Self->TargetY - 1, 1);
    PositionUpdate(Self->Width, *Self->TargetWidth + 2, 1);
    PositionUpdate(Self->Height, *Self->TargetHeight + 2, 1);
}

void OLED_ShowSelectioneBar(OLED_t *OLED, SelectioneBar_t *SelectioneBar) {
    OLED_ReverseArea(OLED, SelectioneBar->X, SelectioneBar->Y,
                     SelectioneBar->Width, SelectioneBar->Height);
}