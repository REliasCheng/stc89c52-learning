#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__
#include "Com_Util.h"

/**
 * @brief 发送多个字节
 *
 * @param addr 字节地址
 * @param bytes 待发送的数据
 * @param len 数组长度
 */
void Int_EEPROM_WriteBytes(u8 addr, u8 bytes[], u8 len);

/**
 * @brief 接收多个字节
 *
 * @param addr 字节地址
 * @param bytes 用于接收结果的数组
 * @param len 要读取的长度
 */
void Int_EEPROM_ReadBytes(u8 addr, u8 bytes[], u8 len);

#endif /* __INT_EEPROM_H__ */