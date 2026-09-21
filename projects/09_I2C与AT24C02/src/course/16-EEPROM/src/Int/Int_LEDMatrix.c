#include "Int_LEDMatrix.h"
#include "Dri_Timer0.h"
#include <STC89C5xRC.H>
#define LED_MATRIX_EN P35
#define LED_EN        P34

#define SER           P10
#define SCK           P12
#define RCK           P11

static u8 s_buffer[8];
void Int_LEDMatrix_RefreshByTimer0();
void Int_LEDMatrix_Init()
{
    LED_MATRIX_EN = 0;
    LED_EN        = 0;

    Dri_Timer0_RegisterCallback(Int_LEDMatrix_RefreshByTimer0);
}

void Int_LEDMatrix_SetPic(u8 pic[])
{
    u8 i;
    for (i = 0; i < 8; i++) {
        s_buffer[i] = pic[i];
    }
}

void Int_LEDMatrix_Shift(u8 pic)
{
    u8 i = 7;
    for (i = 7; i >= 1; i--) {
        s_buffer[i] = s_buffer[i - 1];
    }
    s_buffer[0] = pic;
}

void Int_LEDMatrix_RefreshByTimer0()
{
    static u8 i = 0;
    P0          = 0xFF;
    // 1.设置显示哪行
    if (i == 0) {
        SER = 1;
    } else {
        SER = 0;
    }

    SCK = 0;
    SCK = 1;

    RCK = 0;
    RCK = 1;

    // 2.设置当前行的显示内容
    P0 = ~s_buffer[i];

    i++;
    if (i == 8) {
        i = 0;
    }
}