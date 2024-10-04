#include "Menu.h"

//void TextMenu_Init(TextMenu *menu) { TextPage_Init(menu->Page); }

//void TextPage_Init(TextPage *page) {
//    for (uint8_t i = 0; i < page->NumOfLowerPages; i++) {
//        page->LowerPages[i].UpperPage = page;
//        TextPage_Init(&page->LowerPages[i]);
//    }
//}

//void U8G2_ShowTextMenu(U8G2 *u8g2, TextMenu *menu) {
//    uint8_t begin = menu->Cursor >= 4 ? menu->Cursor - 3 : 0;

//    for (uint8_t i = 0; i < TEXT_COUNT_OF_PAGE; i++) {
//        if (begin + i == menu->Cursor) {
//            U8G2_Printf(u8g2, 0, 16 + i * 16, "%s <-",
//                        menu->Page->LowerPages[begin + i].Title);
//        } else {
//            U8G2_Printf(u8g2, 0, 16 + i * 16, "%s",
//                        menu->Page->LowerPages[begin + i].Title);
//        }
//    }
//}

//void TextMenu_CursorInc(TextMenu *menu) {
//    if (menu->Cursor == menu->Page->NumOfLowerPages - 1) {
//        menu->Cursor = 0;
//    } else {
//        menu->Cursor++;
//    }
//}

//void TextMenu_CursorDec(TextMenu *menu) {
//    if (menu->Cursor == 0) {
//        menu->Cursor = menu->Page->NumOfLowerPages - 1;
//    } else {
//        menu->Cursor--;
//    }
//}

//void TextMenu_EnterLowerPage(TextMenu *menu) {
//    if (menu->Page->NumOfLowerPages) {
//        menu->Page = &menu->Page->LowerPages[menu->Cursor];
//        menu->Cursor = 0;
//    }
//}

//void TextMenu_ReturnUpperPage(TextMenu *menu) {
//    if (menu->Page->UpperPage) {
//        menu->Page = menu->Page->UpperPage;
//        menu->Cursor = 0;
//    }
//}

//void U8G2_ShowImageMenu(U8G2 *u8g2, ImageMenu *menu, uint16_t delay) {
//    u8g2_DrawXBMP(
//        &u8g2->u8g2, 0, 16, 32, 32,
//        menu->Page[menu->Cursor != 0 ? menu->Cursor - 1 : menu->NumOfPages - 1]
//            .Image);

//    u8g2_DrawXBMP(&u8g2->u8g2, 48, 16, 32, 32, menu->Page[menu->Cursor].Image);

//    u8g2_DrawXBMP(
//        &u8g2->u8g2, 96, 16, 32, 32,
//        menu->Page[menu->Cursor != menu->NumOfPages - 1 ? menu->Cursor + 1 : 0]
//            .Image);
//}

//void ImageMenu_CursorInc(ImageMenu *menu) {
//    if (menu->Cursor == menu->NumOfPages - 1) {
//        menu->Cursor = menu->Cursor == 0;
//    } else {
//        menu->Cursor++;
//    }
//}

//void ImageMenu_CursorDec(ImageMenu *menu) {
//    if (menu->Cursor == 0) {
//        menu->Cursor = menu->NumOfPages - 1;
//    } else {
//        menu->Cursor--;
//    }
//}
