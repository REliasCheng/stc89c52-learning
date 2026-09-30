#include "Int_DS18B20_Safe.h"
#include "Dri_1Wire.h"
#include "Com_Util.h"

signed int Int_DS18B20_ReadTenths(void)
{
    unsigned char low_byte;
    unsigned char high_byte;
    signed int raw;

    Dri_1Wire_Init();
    Dri_1Wire_WriteByte(0xCC);
    Dri_1Wire_WriteByte(0x44);
    Com_Util_Delay1ms(750);

    Dri_1Wire_Init();
    Dri_1Wire_WriteByte(0xCC);
    Dri_1Wire_WriteByte(0xBE);
    low_byte = Dri_1Wire_ReadByte();
    high_byte = Dri_1Wire_ReadByte();
    raw = (signed int)(((unsigned int)high_byte << 8) | low_byte);
    return (signed int)((raw * 10L) / 16L);
}
