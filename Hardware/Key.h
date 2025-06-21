#ifndef KEY_H
#define KEY_H

#include "RTE_Components.h"
#include CMSIS_device_header

#include "GPIO.h"

// typedef enum {
//     KeyState_Up,
//     KeyState_Down,
// } KeyState;
// static const char *KeKeyStateString[] = {
//     "KeyState_Up",
//     "KeyState_Down",
// };

// typedef enum {
//     KeyEvent_None,
//     KeyEvent_Click,
//     KeyEvent_DoubleClick,
//     KeyEvent_LongPress,
// } KeyEvent;
// static const char *KeyEventString[] = {
//     "KeyEvent_None",
//     "KeyEvent_Click",
//     "KeyEvent_DoubleClick",
//     "KeyEvent_LongPress",
// };

typedef enum {
    KeyMode_PullDown,
    KeyMode_PullUp,
} KeyMode_t;

typedef struct {
    GPIOxPiny_t GPIOxPiny;
    KeyMode_t Mode;

    uint32_t IDR;

    // KeyState State;
    // KeyState LastState;
    // uint32_t StateDuration;

    // KeyEvent Event;
    // KeyEvent LastEvent;
    // uint32_t EventDuration;
} Key_t;

void Key_Init(Key_t *Self);
uint8_t Key_IsPressing(Key_t *Self);
// KeyEvent Key_Read(Key_t *Self);
// KeyState Key_GetState(Key_t *Self);
// void Key_Update(Key_t *Self, uint32_t ms);

#endif