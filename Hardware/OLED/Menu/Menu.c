#include "Menu.h"

void TextMenu_Init(TextMenu *self) { TextPage_Init(self->Page); }

void TextPage_Init(TextPage *self) {
    for (uint8_t i = 0; i < self->NumOfLowerPages; i++) {
        self->LowerPages[i].UpperPage = self;
        TextPage_Init(&self->LowerPages[i]);
    }
}

void U8G2_ShowTextMenu(U8G2 *u8g2, TextMenu *menu) {
    uint8_t begin = menu->Cursor >= 4 ? menu->Cursor - 3 : 0;

    for (uint8_t i = 0; i < TEXT_COUNT_OF_PAGE; i++) {
        if (begin + i == menu->Cursor) {
            U8G2_Printf(u8g2, 0, 16 + i * 16, "%s <-",
                        menu->Page->LowerPages[begin + i].Title);
        } else {
            U8G2_Printf(u8g2, 0, 16 + i * 16, "%s",
                        menu->Page->LowerPages[begin + i].Title);
        }
    }
}

void TextMenu_CursorInc(TextMenu *self) {
    if (self->Cursor == self->Page->NumOfLowerPages - 1) {
        self->Cursor = 0;
    } else {
        self->Cursor++;
    }
}

void TextMenu_CursorDec(TextMenu *self) {
    if (self->Cursor == 0) {
        self->Cursor = self->Page->NumOfLowerPages - 1;
    } else {
        self->Cursor--;
    }
}

void TextMenu_EnterLowerPage(TextMenu *self) {
    if (self->Page->NumOfLowerPages) {
        self->Page = &self->Page->LowerPages[self->Cursor];
        self->Cursor = 0;
    }
}

void TextMenu_ReturnUpperPage(TextMenu *self) {
    if (self->Page->UpperPage) {
        self->Page = self->Page->UpperPage;
        self->Cursor = 0;
    }
}

void U8G2_ShowImageMenu(U8G2 *u8g2, ImageMenu *menu, uint16_t delay) {
    u8g2_DrawXBMP(
        &u8g2->u8g2, 0, 16, 32, 32,
        menu->Page[menu->Cursor != 0 ? menu->Cursor - 1 : menu->NumOfPages - 1]
            .Image);

    u8g2_DrawXBMP(&u8g2->u8g2, 48, 16, 32, 32, menu->Page[menu->Cursor].Image);

    u8g2_DrawXBMP(
        &u8g2->u8g2, 96, 16, 32, 32,
        menu->Page[menu->Cursor != menu->NumOfPages - 1 ? menu->Cursor + 1 : 0]
            .Image);
}

void ImageMenu_CursorInc(ImageMenu *self) {
    if (self->Cursor == self->NumOfPages - 1) {
        self->Cursor = self->Cursor == 0;
    } else {
        self->Cursor++;
    }
}

void ImageMenu_CursorDec(ImageMenu *self) {
    if (self->Cursor == 0) {
        self->Cursor = self->NumOfPages - 1;
    } else {
        self->Cursor--;
    }
}
