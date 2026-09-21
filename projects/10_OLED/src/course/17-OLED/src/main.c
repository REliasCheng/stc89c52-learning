#include "Int_OLED.h"
void main()
{
    Int_OLED_Init();
    Int_OLED_Clear();
    Int_OLED_ShowStr(0, 0, "Hello World!");
    Int_OLED_ShowNum(0, 1, -12345678);
    while (1) {
    }
}