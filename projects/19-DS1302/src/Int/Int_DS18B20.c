#include "Int_DS18B20.h"
#include "Dri_1Wire.h"
char Int_DS18B20_GetTemperature()
{
    u8 ls_byte;
    u8 ms_byte;

    // 初始化
    Dri_1Wire_Init();
    // 发送ROM命令
    Dri_1Wire_WriteByte(0xCC);
    // 发送测温命令
    Dri_1Wire_WriteByte(0x44);

    // 初始化
    Dri_1Wire_Init();
    // 发送ROM命令
    Dri_1Wire_WriteByte(0xCC);
    // 发送读取温度命令
    Dri_1Wire_WriteByte(0xBE);

    ls_byte = Dri_1Wire_ReadByte();
    ms_byte = Dri_1Wire_ReadByte();

    return (ls_byte >> 4) | (ms_byte << 4);
}