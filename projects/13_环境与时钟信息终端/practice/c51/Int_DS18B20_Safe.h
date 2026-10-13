#ifndef __INT_DS18B20_SAFE_H__
#define __INT_DS18B20_SAFE_H__

/* Returns 0 if the scratchpad CRC fails. A valid 0 C reading returns 1. */
unsigned char Int_DS18B20_TryReadTenths(signed int *temperature_tenths);

#endif
