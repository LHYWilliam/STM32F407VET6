#ifndef KEY_H
#define KEY_H

#include CMSIS_device_header

typedef enum {
    KeyState_Up,
    KeyState_Down,
} KeyState;
static const char *KeKeyStateString[] = {
    "KeyState_Up",
    "KeyState_Down",
};

typedef enum {
    KeyEvent_None,
    KeyEvent_Click,
    KeyEvent_DoubleClick,
    KeyEvent_LongPress,
} KeyEvent;
static const char *KeyEventString[] = {
    "KeyEvent_None",
    "KeyEvent_Click",
    "KeyEvent_DoubleClick",
    "KeyEvent_LongPress",
};

typedef struct {
    char GPIOxPiny[4];

    KeyState State;
    KeyState LastState;
    uint32_t StateDuration;

    KeyEvent Event;
    KeyEvent LastEvent;
    uint32_t EventDuration;

    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin;
} Key_t;

void Key_Init(Key_t *self);

KeyEvent Key_Read(Key_t *self);
KeyState Key_GetState(Key_t *self);
void Key_Update(Key_t *self, uint32_t ms);

#endif