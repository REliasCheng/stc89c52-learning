/* Host stub only: compiles and exercises the original C51 adapter source. */
#include "Int_DS18B20_Safe.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned char bytes[9];
static unsigned char index_read;
static unsigned char reset_count;
static unsigned char writes[4];
static unsigned char write_count;
static unsigned int delayed_ms;

void Dri_1Wire_Init(void) { ++reset_count; }
void Dri_1Wire_WriteByte(unsigned char value) { writes[write_count++] = value; }
unsigned char Dri_1Wire_ReadByte(void) { return bytes[index_read++]; }
void Com_Util_Delay1ms(unsigned int milliseconds) { delayed_ms += milliseconds; }

static void setup(const unsigned char sample[9])
{
    memcpy(bytes, sample, 9U);
    index_read = 0;
    reset_count = 0;
    write_count = 0;
    delayed_ms = 0;
}

int main(void)
{
    /* Published DS18B20 power-on scratchpad vector, then a valid 0 C vector. */
    static const unsigned char warm[9] = {0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10, 0x1C};
    static const unsigned char zero[9] = {0x00, 0x00, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10, 0xC8};
    signed int reading = 123;

    setup(warm);
    assert(Int_DS18B20_TryReadTenths(&reading) == 1);
    assert(reading == 850);
    assert(reset_count == 2 && index_read == 9 && delayed_ms == 750);
    assert(write_count == 4 && writes[0] == 0xCC && writes[1] == 0x44 &&
           writes[2] == 0xCC && writes[3] == 0xBE);

    setup(zero);
    assert(Int_DS18B20_TryReadTenths(&reading) == 1);
    assert(reading == 0);

    setup(warm);
    bytes[3] ^= 1U;
    reading = 321;
    assert(Int_DS18B20_TryReadTenths(&reading) == 0);
    assert(reading == 321); /* The failed sample does not overwrite valid history. */
    assert(Int_DS18B20_TryReadTenths(0) == 0);
    puts("DS18B20 adapter host tests passed (valid, zero, CRC failure, null)");
    return 0;
}
