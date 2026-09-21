#include "Int_DigitalTube.h"

void main()
{
    Int_DigitalTube_Init();
    Int_DigitalTube_DisplayNum(-2345678);
    while (1) {
        Int_DigitalTube_Refresh();
    }
}