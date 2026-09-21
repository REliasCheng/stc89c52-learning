#include "Dri_IIC.h"

void Dri_IIC_Start()
{
    SCL = 1;
    SDA = 1;
    SDA = 0;
    SCL = 0;
}

void Dri_IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    SDA = 1;
}

void Dri_IIC_SendByte(u8 byte)
{
    u8 i;
    for (i = 0; i < 8; i++) {
        SDA = (byte & (0x80 >> i)) == 0 ? 0 : 1;
        SCL = 1;
        SCL = 0;
    }
}

u8 Dri_IIC_ReceiveByte()
{
    u8 byte = 0;
    u8 i;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        SCL  = 1;
        byte = (byte << 1) | SDA;
        SCL  = 0;
    }
    return byte;
}

void Dri_IIC_SendAck(bit ack)
{
    SDA = ack;
    SCL = 1;
    SCL = 0;
}

bit Dri_IIC_ReceiveAck()
{
    bit ack;
    SDA = 1;
    SCL = 1;
    ack = SDA;
    SCL = 0;
    return ack;
}
