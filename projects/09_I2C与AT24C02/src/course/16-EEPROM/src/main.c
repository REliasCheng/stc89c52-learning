#include "Dri_Timer0.h"
#include "Int_LEDMatrix.h"
#include "Int_EEPROM.h"
u8 picture[26] = {0xF8, 0x0A, 0xEC, 0xAF, 0xEC, 0x8A, 0xF8, 0x00,
                  0x10, 0xF9, 0x97, 0xF1, 0x88, 0xAA, 0xFF, 0xAA,
                  0x88, 0x00, 0x14, 0x0A, 0xF5, 0x92, 0x92, 0xF5,
                  0x0A, 0x14};
u8 buffer[26];
void main()
{
    u8 i;
    Dri_Timer0_Init();
    Int_LEDMatrix_Init();

    Int_EEPROM_WriteBytes(0, picture, 26);
    Int_EEPROM_ReadBytes(0, buffer, 26);

    while (1) {
        for (i = 0; i < 26; i++) {
            Int_LEDMatrix_Shift(buffer[i]);
            Com_Util_Delay1ms(200);
        }
    }
}