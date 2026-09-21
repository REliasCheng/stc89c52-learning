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