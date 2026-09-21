#include "Int_OLED.h"
#include "Int_DS18B20.h"
#include "Com_Util.h"
void main()
{
    Int_OLED_Init();
    Int_OLED_Clear();
    while (1) {
        Int_OLED_ShowNum(0, 0, Int_DS18B20_GetTemperature());
        Com_Util_Delay1ms(500);
    }
}