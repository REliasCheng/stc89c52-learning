#include "Dri_UART.h"
#include "Com_Util.h"
#include <STC89C5xRC.H>
void main()
{
    char c;
    Dri_UART_Init();
    while (1) {
        if (Dri_UART_RecevieChar(&c)) {
            if (c == 'A') {
                P0 = 0x00;
                Dri_UART_SendStr("Ok: LED is on");
            } else if (c == 'B') {
                P0 = 0xFF;
                Dri_UART_SendStr("Ok: LED is off");
            } else {
                Dri_UART_SendStr("Error: Unknown command");
            }
        }
    }
}