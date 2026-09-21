#include "Dri_UART.h"
#include "Com_Util.h"
#include "Dri_Timer0.h"
#include <STC89C5xRC.H>
#include <STRING.H>
void main()
{
    char str[16];
    Dri_Timer0_Init();
    Dri_UART_Init();
    while (1) {
        if (Dri_UART_RecevieStr(str)) {
            if (strcmp(str, "on") == 0) {
                P0 = 0x00;
                Dri_UART_SendStr("Ok: LED is on");
            } else if (strcmp(str, "off") == 0) {
                P0 = 0xFF;
                Dri_UART_SendStr("Ok: LED is off");
            } else {
                Dri_UART_SendStr("Error: Unknown command");
            }
        }
    }
}