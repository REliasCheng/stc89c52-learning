#ifndef __DRI_1WIRE_H__
#define __DRI_1WIRE_H__
#include "Com_Util.h"
/**
 * @brief 初始化
 *
 * @return bit 1:存在 0:不存在
 */
bit Dri_1Wire_Init();

void Dri_1Wire_WriteByte(u8 byte);

u8 Dri_1Wire_ReadByte();

#endif /* __DRI_1WIRE_H__ */