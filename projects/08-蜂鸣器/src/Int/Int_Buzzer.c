#include "Int_Buzzer.h"
#include "Com_Util.h"
#include <STC89C5xRC.H>
#define BUZZER P46

void Int_Buzzer_Buzz()
{
    u8 count = 100;
    while (count > 0) {
        BUZZER = ~BUZZER;
        Com_Util_Delay1ms(1);
        count--;
    }
}