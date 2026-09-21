#include "Dri_Timer0.h"
#include <STC89C5xRC.H>
#include "Com_Util.h"

void LED_Blink()
{
    static u16 count = 0;
    count++;
    if (count >= 500) {
        P00   = ~P00;
        count = 0;
    }
}

void main()
{
    Dri_Timer0_Init();
    Dri_Timer0_RegisterCallback(LED_Blink);
    while (1) {
    }
}