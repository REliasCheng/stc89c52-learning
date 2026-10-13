#include "Int_DS18B20_Safe.h"
#include "Dri_1Wire.h"
#include "Com_Util.h"

static unsigned char scratchpad_crc(const unsigned char *data, unsigned char length)
{
    unsigned char crc = 0;
    unsigned char index;
    unsigned char bit;
    for (index = 0; index < length; ++index) {
        unsigned char current = data[index];
        for (bit = 0; bit < 8U; ++bit) {
            unsigned char mix = (unsigned char)((crc ^ current) & 1U);
            crc >>= 1;
            if (mix) crc ^= 0x8CU;
            current >>= 1;
        }
    }
    return crc;
}

unsigned char Int_DS18B20_TryReadTenths(signed int *temperature_tenths)
{
    unsigned char scratchpad[9];
    unsigned char index;
    signed long raw;

    if (temperature_tenths == 0) return 0;

    Dri_1Wire_Init();
    Dri_1Wire_WriteByte(0xCC);
    Dri_1Wire_WriteByte(0x44);
    Com_Util_Delay1ms(750);

    Dri_1Wire_Init();
    Dri_1Wire_WriteByte(0xCC);
    Dri_1Wire_WriteByte(0xBE);
    for (index = 0; index < 9U; ++index) {
        scratchpad[index] = Dri_1Wire_ReadByte();
    }
    if (scratchpad_crc(scratchpad, 8U) != scratchpad[8]) return 0;
    raw = (signed long)(((unsigned long)scratchpad[1] << 8) | scratchpad[0]);
    if (raw >= 32768L) raw -= 65536L;
    *temperature_tenths = (signed int)((raw * 10L) / 16L);
    return 1;
}
