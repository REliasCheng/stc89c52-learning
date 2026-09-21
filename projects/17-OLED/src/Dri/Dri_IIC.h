#ifndef __DRI_IIC_H__
#define __DRI_IIC_H__
#include <STC89C5xRC.H>
#include "Com_Util.h"
#define SCL P17
#define SDA P16
void Dri_IIC_Start();
void Dri_IIC_Stop();

void Dri_IIC_SendByte(u8 byte);
u8 Dri_IIC_ReceiveByte();

void Dri_IIC_SendAck(bit ack);
bit Dri_IIC_ReceiveAck();

#endif /* __DRI_IIC_H__ */
