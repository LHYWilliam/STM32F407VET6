#include "Key.h"

void Key_Init(Key_t *Self) {
    GPIO_t GPIO = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = Self->Mode == KeyMode_PullDown ? GPIO_PULLDOWN : GPIO_PULLUP,
    };

    Self->IDR = GPIO_InitPin(&GPIO, Self->GPIOxPiny);

    // Self->LastState = KeyState_Up;
    // Self->LastEvent = KeyEvent_None;
}

uint8_t Key_IsPressing(Key_t *Self) {
    return GPIO_ReadInput(Self->IDR) != Self->Mode;
}

// KeyEvent Key_Read(Key_t *Self) {
//     if (GPIO_ReadInput(Self->IDR)) {
//         Delay_ms(10);
//         while (GPIO_ReadInput(Self->IDR)) {
//         }

//         Delay_ms(10);
//         return KeyEvent_Click;
//     }

//     return KeyEvent_None;
// }

// void Key_Update(Key_t *self, uint32_t ms) {
//     self->State = Key_GetState(self);

//     switch (self->State) {
//     case KeyState_Down:
//         if (self->LastState == KeyState_Down && self->StateDuration > 200) {
//             self->Event = KeyEvent_LongPress;
//         }
//         break;

//     case KeyState_Up:
//         if (self->LastState == KeyState_Up && self->StateDuration > 100) {
//             self->Event = KeyEvent_None;

//         } else if (self->LastState == KeyState_Down &&
//                    self->StateDuration > 20 && self->StateDuration <= 200) {
//             if (self->LastEvent == KeyEvent_Click) {
//                 self->Event = KeyEvent_DoubleClick;

//             } else {
//                 self->Event = KeyEvent_Click;
//             }
//         }
//         break;
//     }

//     if (self->LastState == self->State) {
//         self->StateDuration += ms;
//     } else {
//         self->StateDuration = 0;
//         self->LastState = self->State;
//     }

//     if (self->LastEvent == self->Event) {
//         self->EventDuration += ms;
//     } else {
//         self->EventDuration = 0;
//         self->LastEvent = self->Event;
//     }
// }

// KeyState Key_GetState(Key_t *self) {
//     return HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin) ? KeyState_Down
//                                                          : KeyState_Up;
// }
