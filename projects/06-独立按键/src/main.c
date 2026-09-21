#include <STC89C5xRC.H>
#include "Int_Key.h"
#define LED1 P00
#define LED2 P01
#define LED3 P02
#define LED4 P03
void main()
{
    while (1) {
        if (Int_Key_IsSW1Pressed()) {
            LED1 = ~LED1;
        }
        if (Int_Key_IsSW2Pressed()) {
            LED2 = ~LED2;
        }
        if (Int_Key_IsSW3Pressed()) {
            LED3 = ~LED3;
        }
        if (Int_Key_IsSW4Pressed()) {
            LED4 = ~LED4;
        }
    }
}