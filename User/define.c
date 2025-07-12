#include "main.h"

LED_t LEDRed = {
    .GPIOxPiny = PD11,
    .Mode = LEDMode_PullUp,
};

LED_t LEDGreen = {
    .GPIOxPiny = PD12,
    .Mode = LEDMode_PullUp,
};

LED_t LEDBlue = {
    .GPIOxPiny = PD13,
    .Mode = LEDMode_PullUp,
};

Key_t Key1 = {
    .GPIOxPiny = PE14,
    .Mode = KeyMode_PullUp,
};

Key_t Key2 = {
    .GPIOxPiny = PE15,
    .Mode = KeyMode_PullUp,
};

Key_t Key3 = {
    .GPIOxPiny = PB12,
    .Mode = KeyMode_PullUp,
};

Key_t Key4 = {
    .GPIOxPiny = PB13,
    .Mode = KeyMode_PullUp,
};

// Serial_t SerialBoard = {
//     .USARTx = USART1,
//     .TX = PA9,
//     .RX = PA10,
//     .Baudrate = 115200,
//     .Default = ENABLE,
//     .Interrupt = ENABLE,
//     .RxITSize = 1,
//     .Priority = 1,
//     .PackLength = 2,
// };

Serial_t SerialBoard = {
    .USARTx = USART1,
    .TX = PA9,
    .RX = PA10,
    .Baudrate = 115200,
    .Interrupt = ENABLE,
    .IdleDMA = ENABLE,
    .Priority = 1,
    .DMA =
        {
            .DMAx = DMA2,
            .Stream = 2,
            .Channel = 4,
        },
    .PackLength = 2,
    .Default = ENABLE,
};

Serial_t SerialBluetooth = {
    .USARTx = USART2,
    .TX = PD5,
    .RX = PD6,
    .Baudrate = 115200,
    .Interrupt = ENABLE,
    .RxITSize = 1,
    .Priority = 1,
    .PackLength = 2,
};

Serial_t SerialK230 = {
    .USARTx = USART3,
    .TX = PD8,
    .RX = PD9,
    .Baudrate = 115200,
};

Serial_t SerialJY61P = {
    .USARTx = UART4,
    .TX = PC10,
    .RX = PC11,
    .Baudrate = 115200,
};

PWM_t MotorPWM;

Motor_t MotorLeft = {
    .TIMx = TIM1,
    .Channel = 2,
    .PWM = PE11,
    .IN1 = PE10,
    .IN2 = PE12,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .Invert = ENABLE,
    .PWM_Init = DISABLE,
};

Motor_t MotorRight = {
    .TIMx = TIM1,
    .Channel = 1,
    .PWM = PE9,
    .IN1 = PE8,
    .IN2 = PE7,
    .Range = 10000,
    ._PWM = &MotorPWM,
    .PWM_Init = ENABLE,
};

Encoder_t EncoderLeft = {
    .TIMx = TIM4,
    .Channel = {1, 2},
    .GPIOxPiny = {PB6, PB7},
};

Encoder_t EncoderRight = {
    .TIMx = TIM3,
    .Channel = {1, 2},
    .GPIOxPiny = {PB4, PB5},
};

PWM_t ServoPWM;

Servo_t Servo1 = {
    .TIMx = TIM8,
    .Channel = {1, 2},
    .GPIOxPiny = {PC6, PC7},
    .PWM = &ServoPWM,
    .PWM_Init = DISABLE,
};

Servo_t Servo2 = {
    .TIMx = TIM8,
    .Channel = {3, 4},
    .GPIOxPiny = {PC8, PC9},
    .PWM = &ServoPWM,
    .PWM_Init = ENABLE,
};

#define ADC_DataLength 3
uint32_t ADC_Data[ADC_DataLength];

Sampler_t Sampler = {
    .Data = ADC_Data,
    .Length = ADC_DataLength,
    .ADC =
        {
            .ADCx = ADC1,
            .Channel =
                {
                    3,
                    4,
                    5,
                },
            .GPIOxPiny = {PA3, PA4, PA5},
        },
    .DMA =
        {
            .DMAx = DMA2,
            .Channel = 0,
            .Stream = 0,
        },
    .Timer =
        {
            .TIMx = TIM2,
            .Hz = 100,
        },
};

OLED_t OLED = {
    .SCL = PD14,
    .SDA = PD15,
    .I2C = ENABLE,
};

GWGray_t GWGray = {
    .SCL = PB8,
    .SDA = PB9,
    .DevAddr = 0x4C,
};

ICM42688_t ICM42688 = {
    .SCLK = PB10,
    .MISO = PC2,
    .MOSI = PC3,
    .CS = PC1,
    .SPIx = SPI2,
    .AccRange = ICM42688Acc_4G,
    .GyroRange = ICM42688Gyro_1000DPS,
    .AccFreq = ICM42688Acc_100Hz,
    .GyroFreq = ICM42688Gyro_100Hz,
};

TaskHandle_t xMainTaskHandle;
void vMainTaskCode(void *pvParameters);

TaskHandle_t xInteractionTaskHandle;
void vInteractionTaskCode(void *pvParameters);

TaskHandle_t xOLEDTaskHandle;
void vOLEDTaskCode(void *pvParameters);

TimerHandle_t xLEDTimer;
void vLEDTimerCallback(TimerHandle_t pxTimer);

void SystemClock_Config(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP,
                        uint16_t PLLQ);

#define TextPage_Back(title)                                                   \
    (TextPage_t) {                                                             \
        .Title = title, .RotationCallback = TextPage_CursorCallback,           \
        .ClickCallback = TextPage_BackCallback,                                \
    }

#define TextPage_ParameterMonitorPage(title, Type)                             \
    (TextPage_t) {                                                             \
        .Title = title, .RotationCallback = TextPage_CursorCallback,           \
        .ParameterType = Type,                                                 \
    }

#define TextPage_ParameterDoAdjustPage()                                       \
    (TextPage_t) {                                                             \
        .Title = "*", .RotationCallback = TextPage_CursorCallback,             \
        .ClickCallback = TextPage_ParameterDoAdjustCallback,                   \
    }

#define TextPage_ParameterMultiplyPage()                                       \
    (TextPage_t) {                                                             \
        .Title = "x10", .RotationCallback = TextPage_CursorCallback,           \
        .ClickCallback = TextPage_ParameterMultiplyCallback,                   \
    }

#define TextPage_ParameterDividePage()                                         \
    (TextPage_t) {                                                             \
        .Title = "/10", .RotationCallback = TextPage_CursorCallback,           \
        .ClickCallback = TextPage_ParameterDivideCallback,                     \
    }

#define TextPage_ParameterPlusMinusPage()                                      \
    (TextPage_t) {                                                             \
        .Title = "+/-", .RotationCallback = TextPage_CursorCallback,           \
        .ClickCallback = TextPage_ParameterPlusMinusCallback,                  \
    }

#define TextPage_ParameterAdjustPage(title, Type)                              \
    (TextPage_t) {                                                             \
        .Title = title, .TitleX = 128 / 2, .TitleY = 64 / 2, .TitleWidth = 1,  \
        .TitleHeight = 1, .ShowCallback = TextPage_ShowDialogCallback,         \
        .UpdateCallback = TextPage_UpdateDialogCallback,                       \
        .ClickCallback = TextPage_EnterCallback,                               \
        .RotationCallback = TextPage_CursorCallback, .ParameterType = Type,    \
        .NumOfLowerPages = 5, .LowerPages = (TextPage_t[]) {                   \
            TextPage_Back("<"), TextPage_ParameterDoAdjustPage(),              \
                TextPage_ParameterMultiplyPage(),                              \
                TextPage_ParameterDividePage(),                                \
                TextPage_ParameterPlusMinusPage()                              \
        }                                                                      \
    }

#define TextPage_OptionPage(title)                                             \
    (TextPage_t) {                                                             \
        .Title = title, .RotationCallback = TextPage_CursorCallback,           \
        .ClickCallback = TextPage_ChooseOptionCallback,                        \
    }

#define TextPage_NavigationPageConfig()                                        \
    ShowCallback = TextPage_ShowCallback,                                      \
    .UpdateCallback = TextPage_UpdateCallback,                                 \
    .RotationCallback = TextPage_CursorCallback,                               \
    .ClickCallback = TextPage_EnterCallback

#define TextPage_MonitorPageConfig()                                           \
    ShowCallback = TextPage_ShowParameterCallback,                             \
    .UpdateCallback = TextPage_UpdateCallback,                                 \
    .RotationCallback = TextPage_CursorCallback,                               \
    .ClickCallback = TextPage_EnterCallback

#define TextPage_AdjustPageConfig()                                            \
    ShowCallback = TextPage_ShowParameterCallback,                             \
    .UpdateCallback = TextPage_UpdateCallback,                                 \
    .RotationCallback = TextPage_CursorCallback,                               \
    .ClickCallback = TextPage_EnterCallback

#define TextPage_OptiontPageConfig()                                           \
    ShowCallback = TextPage_ShowOptionCallback,                                \
    .UpdateCallback = TextPage_UpdateCallback,                                 \
    .RotationCallback = TextPage_CursorCallback,                               \
    .ClickCallback = TextPage_EnterCallback

TextMenu_t TextMenu;

SelectioneBar_t Bar;

TextPage_t ParameterPage = {
    .Title = "Parameter",
    .ShowCallback = TextPage_ShowCallback,
    .UpdateCallback = TextPage_UpdateCallback,
    .NumOfLowerPages = 5,
    .LowerPages =
        (TextPage_t[]){
            TextPage_Back("<"),
            (TextPage_t){
                .Title = "Monitor",
                .TextPage_NavigationPageConfig(),
                .NumOfLowerPages = 4,
                .LowerPages =
                    (TextPage_t[]){
                        TextPage_Back("<"),
                        (TextPage_t){
                            .Title = "ICM42688 Angle",
                            .TextPage_MonitorPageConfig(),
                            .NumOfLowerPages = 4,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterMonitorPage("Yaw", Float),
                                    TextPage_ParameterMonitorPage("Pitch",
                                                                  Float),
                                    TextPage_ParameterMonitorPage("Roll",
                                                                  Float),
                                },
                        },
                        (TextPage_t){
                            .Title = "GWGray Error",
                            .TextPage_MonitorPageConfig(),
                            .NumOfLowerPages = 2,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterMonitorPage("Error", Int),
                                },
                        },

                        (TextPage_t){
                            .Title = "Encoder Counter",
                            .TextPage_MonitorPageConfig(),
                            .NumOfLowerPages = 3,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterMonitorPage("Left", Int),
                                    TextPage_ParameterMonitorPage("Right", Int),
                                },
                        },
                    },
            },

            (TextPage_t){
                .Title = "Adjust",
                .TextPage_NavigationPageConfig(),
                .NumOfLowerPages = 4,
                .LowerPages =
                    (TextPage_t[]){
                        TextPage_Back("<"),
                        (TextPage_t){
                            .Title = "MotorLeftSpeed PID",
                            .TextPage_AdjustPageConfig(),
                            .NumOfLowerPages = 4,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterAdjustPage("Kp", Float),
                                    TextPage_ParameterAdjustPage("Ki", Float),
                                    TextPage_ParameterAdjustPage("Kd", Float),
                                },
                        },

                        (TextPage_t){
                            .Title = "MotorRightSpeed PID",
                            .TextPage_AdjustPageConfig(),
                            .NumOfLowerPages = 4,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterAdjustPage("Kp", Float),
                                    TextPage_ParameterAdjustPage("Ki", Float),
                                    TextPage_ParameterAdjustPage("Kd", Float),
                                },
                        },

                        (TextPage_t){
                            .Title = "GWGrayPosition PID",
                            .TextPage_AdjustPageConfig(),
                            .NumOfLowerPages = 4,
                            .LowerPages =
                                (TextPage_t[]){
                                    TextPage_Back("<"),
                                    TextPage_ParameterAdjustPage("Kp", Float),
                                    TextPage_ParameterAdjustPage("Ki", Float),
                                    TextPage_ParameterAdjustPage("Kd", Float),
                                },
                        },
                    },
            },

            (TextPage_t){
                .Title = "Chart",
                .RotationCallback = TextPage_CursorCallback,
            },

            (TextPage_t){
                .Title = "Option",
                .TextPage_OptiontPageConfig(),
                .NumOfLowerPages = 5,
                .LowerPages =
                    (TextPage_t[]){
                        TextPage_Back("<"),
                        TextPage_OptionPage("Stop"),
                        TextPage_OptionPage("Advance"),
                        TextPage_OptionPage("Round"),
                        TextPage_OptionPage("Trace"),
                    },
            },
        },
};

void TextPage_BackCallback(TextPage_t **TextPage);
void TextPage_EnterCallback(TextPage_t **TextPage);
void TextPage_ParameterDoAdjustCallback(TextPage_t **TextPage);
void TextPage_ParameterMultiplyCallback(TextPage_t **TextPage);
void TextPage_ParameterDivideCallback(TextPage_t **TextPage);
void TextPage_ParameterPlusMinusCallback(TextPage_t **TextPage);
void TextPage_ChooseOptionCallback(TextPage_t **TextPage);

void TextPage_CursorCallback(TextPage_t *TextPage, TextPageRotation Direction);

void TextPage_ShowCallback(TextPage_t *TextPage);
void TextPage_ShowParameterCallback(TextPage_t *TextPage);
void TextPage_ShowDialogCallback(TextPage_t *TextPage);
void TextPage_ShowOptionCallback(TextPage_t *TextPage);

void TextPage_UpdateCallback(TextPage_t *TextPage);
void TextPage_UpdateDialogCallback(TextPage_t *TextPage);