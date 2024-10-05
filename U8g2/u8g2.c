#include "u8g2.h"

#include "stdio.h"
#include <stdarg.h>
#include <string.h>

#include "Delay.h"
#include "GPIO.h"

GPIO_TypeDef *SCL_GPIOx;
uint32_t SCL_GPIO_Pin;
GPIO_TypeDef *SDA_GPIOx;
uint32_t SDA_GPIO_Pin;

void U8G2_Init(U8G2 *u8g2) {
    GPIO SCL = {
        .Mode = GPIO_MODE_OUTPUT_OD,
        .Pull = GPIO_PULLUP,
    };
    strcpy(SCL.GPIOxPiny, u8g2->SCL);
    GPIO_Init(&SCL);
    SCL_GPIOx = SCL.GPIOx;
    SCL_GPIO_Pin = SCL.GPIO_Pin;

    GPIO SDA = {
        .Mode = GPIO_MODE_OUTPUT_OD,
        .Pull = GPIO_PULLUP,
    };
    strcpy(SDA.GPIOxPiny, u8g2->SDA);
    GPIO_Init(&SDA);
    SDA_GPIOx = SDA.GPIOx;
    SDA_GPIO_Pin = SDA.GPIO_Pin;

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &u8g2->u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(&u8g2->u8g2); // send init sequence to the display, display
                                   // is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2->u8g2, 0); // wake up display
}

void U8G2_Printf(U8G2 *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *format,
                 ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)u8g2->SendBuffer, format, arg);
    va_end(arg);

    u8g2_DrawUTF8(&u8g2->u8g2, x, y, (char *)u8g2->SendBuffer);
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg,
                                  U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
    // Initialize SPI peripheral
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* HAL initialization contains all what we need so we can skip this
         * part. */

        break;

    // Function which implements a delay, arg_int contains the amount of ms
    case U8X8_MSG_DELAY_MILLI:
        Delay_ms(arg_int);

        break;
    // Function which delays 10us
    case U8X8_MSG_DELAY_10MICRO:
        Delay_us(10);

        break;
    // Function which delays 100ns
    case U8X8_MSG_DELAY_100NANO:
        __NOP();

        break;
    // Function to define the logic level of the clockline
    case U8X8_MSG_GPIO_I2C_CLOCK:
        if (arg_int)
            HAL_GPIO_WritePin(SCL_GPIOx, SCL_GPIO_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(SCL_GPIOx, SCL_GPIO_Pin, GPIO_PIN_RESET);

        break;
    // Function to define the logic level of the data line to the display
    case U8X8_MSG_GPIO_I2C_DATA:
        if (arg_int)
            HAL_GPIO_WritePin(SDA_GPIOx, SDA_GPIO_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(SDA_GPIOx, SDA_GPIO_Pin, GPIO_PIN_RESET);

        break;
    default:
        return 0; // A message was received which is not implemented, return 0
                  // to indicate an error
    }

    return 1; // command processed successfully.
}