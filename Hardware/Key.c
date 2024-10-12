#include <string.h>

#include "Delay.h"
#include "GPIO.h"
#include "Key.h"

void Key_Init(Key_t *self) {
    GPIO_t gpio = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLDOWN,
    };
    strcpy(gpio.GPIOxPiny, self->GPIOxPiny);
    GPIO_Init(&gpio);

    self->LastState = KeyState_Up;
    self->LastEvent = KeyEvent_None;

    self->GPIOx = gpio.GPIOx;
    self->GPIO_Pin = gpio.GPIO_Pin;
}

KeyState Key_Read(Key_t *self) {
    if (HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin)) {
        Delay_ms(10);
        while (HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin)) {
        }

        Delay_ms(10);
        return KeyState_Down;
    }

    return KeyState_Up;
}

void Key_Update(Key_t *self, uint32_t ms) {
    self->State = Key_GetState(self);

    switch (self->State) {

    case KeyState_Down:
        switch (self->LastState) {

        case KeyState_Up:
            break;

        case KeyState_Down:
            if (self->StateDuration > 200) {
                self->Event = KeyEvent_LongPress;
            }
            break;
        }
        break;

    case KeyState_Up:

        switch (self->LastState) {

        case KeyState_Up:
            if (self->StateDuration > 100) {
                self->Event = KeyEvent_None;
            }
            break;

        case KeyState_Down:
            if ((self->StateDuration > 20 && self->StateDuration <= 200)) {
                if (self->LastEvent != KeyEvent_Click) {
                    self->Event = KeyEvent_Click;

                } else {
                    self->Event = KeyEvent_DoubleClick;
                }
            }
            break;
        }

        break;
    }

    if (self->LastState == self->State) {
        self->StateDuration += ms;
    } else {
        self->StateDuration = 0;
        self->LastState = self->State;
    }

    if (self->LastEvent == self->Event) {
        self->EventDuration += ms;
    } else {
        self->EventDuration = 0;
        self->LastEvent = self->Event;
    }
}

KeyState Key_GetState(Key_t *self) {
    return HAL_GPIO_ReadPin(self->GPIOx, self->GPIO_Pin) ? KeyState_Down
                                                         : KeyState_Up;
}
