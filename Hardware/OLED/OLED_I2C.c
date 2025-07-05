#include "GPIO.h"

#include "OLED_I2C.h"

void OLED_SWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length);
void OLED_SWI2C_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length);
void OLED_SWI2C_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_SWI2C_WriteCommands(OLED_t *Self, uint8_t *Commands, uint16_t Length);
void OLED_HWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length);
void OLED_HWI2C_WriteDatas(OLED_t *Self, uint8_t *Data, uint16_t Length);
void OLED_HWI2C_WriteCommand(OLED_t *Self, uint8_t Command);
void OLED_HWI2C_WriteCommands(OLED_t *Self, uint8_t *Command, uint16_t Length);

#define OLED_WriteSCL(Self, x) GPIO_Write(Self->SCL_ODR, x)
#define OLED_WriteSDA(Self, x) GPIO_Write(Self->SDA_ODR, x)

void OLED_SWI2C_Init(OLED_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = Self->I2Cx ? GPIO_MODE_AF_OD : GPIO_MODE_OUTPUT_OD;
    Self->SCL_ODR = GPIO_InitPin(&GPIO, Self->SCL);
    Self->SDA_ODR = GPIO_InitPin(&GPIO, Self->SDA);

    GPIO_Write(Self->SCL_ODR, 1);
    GPIO_Write(Self->SDA_ODR, 1);

    Self->OLED_WriteDatas = OLED_SWI2C_WriteDatas;
    Self->OLED_WriteCommand = OLED_SWI2C_WriteCommand;
    Self->OLED_WriteCommands = OLED_SWI2C_WriteCommands;
}

void OLED_SWI2C_Delay() {
    // Time_Delayus(1);

    static volatile uint8_t i;

    i = 2;
    while (i--)
        ;
}

void OLED_SWI2C_Start(OLED_t *Self) {
    OLED_WriteSDA(Self, 1);
    OLED_WriteSCL(Self, 1);
    // OLED_SWI2C_Delay();
    OLED_WriteSDA(Self, 0);
    OLED_SWI2C_Delay(); // DO NOT REMOVE
    OLED_WriteSCL(Self, 0);
    // OLED_SWI2C_Delay();
}

void OLED_SWI2C_Stop(OLED_t *Self) {
    OLED_WriteSDA(Self, 0);
    OLED_WriteSCL(Self, 1);
    // OLED_SWI2C_Delay();
    OLED_WriteSDA(Self, 1);
}

void OLED_I2C_WaitAck(OLED_t *Self) {
    OLED_WriteSDA(Self, 1);
    // OLED_SWI2C_Delay();
    OLED_WriteSCL(Self, 1);
    OLED_SWI2C_Delay(); // DO NOT REMOVE
    OLED_WriteSCL(Self, 0);
    // OLED_SWI2C_Delay();
}

void OLED_SWI2C_WriteByte(OLED_t *Self, uint8_t Byte) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteSDA(Self, Byte & (0x80 >> i));
        OLED_SWI2C_Delay(); // DO NOT REMOVE
        OLED_WriteSCL(Self, 1);
        OLED_SWI2C_Delay(); // DO NOT REMOVE
        OLED_WriteSCL(Self, 0);
    }
    OLED_WriteSCL(Self, 1);
    OLED_WriteSCL(Self, 0);
}

void OLED_SWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_I2C_WaitAck(Self);
    OLED_SWI2C_WriteByte(Self, 0x40);
    OLED_I2C_WaitAck(Self);
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Data);
        OLED_I2C_WaitAck(Self);
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteDatas(OLED_t *Self, uint8_t *Datas, uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    OLED_SWI2C_WriteByte(Self, 0x40);
    OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Datas[i]);
        OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteCommand(OLED_t *Self, uint8_t Command) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_I2C_WaitAck(Self);
    OLED_SWI2C_WriteByte(Self, 0x00);
    OLED_I2C_WaitAck(Self);
    OLED_SWI2C_WriteByte(Self, Command);
    OLED_I2C_WaitAck(Self);
    OLED_SWI2C_Stop(Self);
}

void OLED_SWI2C_WriteCommands(OLED_t *Self, uint8_t *Commands,
                              uint16_t Length) {
    OLED_SWI2C_Start(Self);
    OLED_SWI2C_WriteByte(Self, 0x78);
    OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    OLED_SWI2C_WriteByte(Self, 0x00);
    OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    for (uint16_t i = 0; i < Length; i++) {
        OLED_SWI2C_WriteByte(Self, Commands[i]);
        OLED_I2C_WaitAck(Self); // DO NOT REMOVE
    }
    OLED_SWI2C_Stop(Self);
}

void OLED_HWI2C_Init(OLED_t *Self) {
    GPIO_t GPIO;
    GPIO.Mode = GPIO_MODE_AF_OD;
    Self->SCL_ODR = GPIO_InitPin(&GPIO, Self->SCL);
    Self->SDA_ODR = GPIO_InitPin(&GPIO, Self->SDA);

    Self->OLED_WriteDatas = OLED_HWI2C_WriteDatas;
    Self->OLED_WriteCommand = OLED_HWI2C_WriteCommand;
    Self->OLED_WriteCommands = OLED_HWI2C_WriteCommands;

    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    // I2C_InitTypeDef I2C_InitStructure;
    // I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    // I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    // I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    // I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
    // I2C_InitStructure.I2C_AcknowledgedAddress =
    //     I2C_AcknowledgedAddress_7bit;
    // I2C_InitStructure.I2C_ClockSpeed = 400000;
    // I2C_Init(Self->I2Cx, &I2C_InitStructure);

    // I2C_Cmd(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteData(OLED_t *Self, uint8_t Data, uint16_t Length) {
    // I2C_GenerateSTART(Self->I2Cx, ENABLE);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) !=
    // SUCCESS)
    //     ;

    // I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    // while (I2C_CheckEvent(Self->I2Cx,
    //                       I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
    //        SUCCESS)
    //     ;

    // I2C_SendData(Self->I2Cx, 0x40);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
    //        SUCCESS)
    //     ;

    // for (uint16_t i = 0; i < Length; i++) {
    //     I2C_SendData(Self->I2Cx, Data);
    //     while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
    //     !=
    //            SUCCESS)
    //         ;
    // }

    // I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteDatas(OLED_t *Self, uint8_t *Data, uint16_t Length) {
    // I2C_GenerateSTART(Self->I2Cx, ENABLE);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) !=
    // SUCCESS)
    //     ;

    // I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    // while (I2C_CheckEvent(Self->I2Cx,
    //                       I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
    //        SUCCESS)
    //     ;

    // I2C_SendData(Self->I2Cx, 0x40);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
    //        SUCCESS)
    //     ;

    // for (uint16_t i = 0; i < Length; i++) {
    //     I2C_SendData(Self->I2Cx, Data[i]);
    //     while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
    //     !=
    //            SUCCESS)
    //         ;
    // }

    // I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommand(OLED_t *Self, uint8_t Command) {
    // I2C_GenerateSTART(Self->I2Cx, ENABLE);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) !=
    // SUCCESS)
    //     ;

    // I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    // while (I2C_CheckEvent(Self->I2Cx,
    //                       I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
    //        SUCCESS)
    //     ;

    // I2C_SendData(Self->I2Cx, 0x00);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
    //        SUCCESS)
    //     ;

    // I2C_SendData(Self->I2Cx, Command);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
    //        SUCCESS)
    //     ;

    // I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}

void OLED_HWI2C_WriteCommands(OLED_t *Self, uint8_t *Command, uint16_t Length) {
    // I2C_GenerateSTART(Self->I2Cx, ENABLE);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) !=
    // SUCCESS)
    //     ;

    // I2C_Send7bitAddress(Self->I2Cx, 0x78, I2C_Direction_Transmitter);
    // while (I2C_CheckEvent(Self->I2Cx,
    //                       I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) !=
    //        SUCCESS)
    //     ;

    // I2C_SendData(Self->I2Cx, 0x00);
    // while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) !=
    //        SUCCESS)
    //     ;

    // for (uint16_t i = 0; i < Length; i++) {
    //     I2C_SendData(Self->I2Cx, Command[i]);
    //     while (I2C_CheckEvent(Self->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
    //     !=
    //            SUCCESS)
    //         ;
    // }

    // I2C_GenerateSTOP(Self->I2Cx, ENABLE);
}
