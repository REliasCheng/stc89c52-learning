#include "Com_Util.h"
#include <INTRINS.H>

void Com_Util_Delay1ms(u16 count)
{
    u8 i, j;

    while (count > 0) {
        count--;
        _nop_();
        i = 2;
        j = 199;
        do {
            while (--j);
        } while (--i);
    }
}


void Com_Util_Delay480us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 218;
	while (--i);
}

void Com_Util_Delay60us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 25;
	while (--i);
}

void Com_Util_Delay420us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 191;
	while (--i);
}

void Com_Util_Delay10us(void)	//@11.0592MHz
{
	unsigned char data i;

	i = 2;
	while (--i);
}

void Com_Util_Delay50us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 20;
	while (--i);
}