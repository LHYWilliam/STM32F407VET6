#include "main.h"

void TextPage_BackCallback(TextPage_t **TextPage) {
    if (TextPage_ReturnUpperPage(TextPage) == SUCCESS) {
        SelectioneBar_BindTextPage(
            &Bar, &(*TextPage)->LowerPages[(*TextPage)->Cursor]);
    }
}

void TextPage_EnterCallback(TextPage_t **TextPage) {
    if (TextPage_EnterLowerPage(TextPage) == SUCCESS) {
        SelectioneBar_BindTextPage(
            &Bar, &(*TextPage)->LowerPages[(*TextPage)->Cursor]);
    }
}

void TextPage_ParameterDoAdjustCallback(TextPage_t **TextPage) {
    switch ((*TextPage)->ParameterType) {
    case ParameterType_Int:
        if ((*TextPage)->IntParameter == 0) {
            (*TextPage)->IntParameter = 1;
            return;
        }

        *(*TextPage)->IntParameterPtr += (*TextPage)->IntParameter;
        break;

    case ParameterType_Float:
        if ((*TextPage)->FloatParameter == 0) {
            (*TextPage)->FloatParameter = 1;
            return;
        }

        *(*TextPage)->FloatParameterPtr += (*TextPage)->FloatParameter;
        break;
    }
}

void TextPage_ParameterMultiplyCallback(TextPage_t **TextPage) {
    switch ((*TextPage)->ParameterType) {
    case ParameterType_Int:
        (*TextPage)->IntParameter *= 10;
        break;

    case ParameterType_Float:
        (*TextPage)->FloatParameter *= 10;
        break;
    }
}

void TextPage_ParameterDivideCallback(TextPage_t **TextPage) {
    switch ((*TextPage)->ParameterType) {
    case ParameterType_Int:
        (*TextPage)->IntParameter /= 10;
        break;

    case ParameterType_Float:
        (*TextPage)->FloatParameter /= 10;
        break;
    }
}

void TextPage_ParameterPlusMinusCallback(TextPage_t **TextPage) {
    switch ((*TextPage)->ParameterType) {
    case ParameterType_Int:
        (*TextPage)->IntParameter = -(*TextPage)->IntParameter;
        break;

    case ParameterType_Float:
        (*TextPage)->FloatParameter = -(*TextPage)->FloatParameter;
        break;
    }
}

// void TextPage_SaveSettingCallback(void *pvParameters) {
//     uint8_t Setting[32];
//     for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
//         Setting[i - 1] = SettingPage.LowerPages[i].Setting;
//     }

//     W25Q64_SectorErase(&W25Q64, 0);
//     W25Q64_PageProgram(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

//     TextPage_EnterCallback(NULL);
// }

// void SettingLoad(void *pvParameters) {
//     uint8_t Setting[32];
//     W25Q64_ReadData(&W25Q64, 0, Setting, SettingPage.NumOfLowerPages - 1);

//     for (uint8_t i = 1; i < SettingPage.NumOfLowerPages; i++) {
//         SettingPage.LowerPages[i].Setting = Setting[i - 1];
//     }
// }

// void TextPage_LoadSettingCallback(void *pvParameters) {
//     SettingLoad(pvParameters);

//     TextPage_EnterCallback(NULL);
// }

// void TextPage_ReverseSettingCallback(void *pvParameters) {
//     TextPage_ReverseSetting(&TextPage->LowerPages[TextPage->Cursor]);
// }

void TextPage_RestartSettingCallback(void *pvParameters) {
    __NVIC_SystemReset();
}

// void TextPage_CursorSwitchEncoderSettingCallback(void *pvParameters) {
//     if (TextPage->LowerPages[TextPage->Cursor].RotationCallback ==
//         TextPage_CursorCallback) {
//         TextPage->LowerPages[TextPage->Cursor].RotationCallback =
//             TextPage_SettingEncoderCallback;

//     } else if (TextPage->LowerPages[TextPage->Cursor]
//                    .RotationCallback == TextPage_SettingEncoderCallback) {
//         TextPage->LowerPages[TextPage->Cursor].RotationCallback =
//             TextPage_CursorCallback;
//     }
// }
