#include "Dri_Timer0.h"
#include <STC89C5xRC.H>
#include "Com_Util.h"
#include <STDIO.H>
#define MAX_CALLBACK_COUNT 4

static Timer0_Callback s_callbacks[MAX_CALLBACK_COUNT];

void Dri_Timer0_Init()
{
    u8 i;
    // 1.启用中断
    EA  = 1;
    ET0 = 1;

    // 2.工作模式：GATE=0；C/T=0；M1=0，M0=1
    TMOD &= 0xF0;
    TMOD |= 0x01;

    // 3.初始值
    TL0 = 64614;
    TH0 = 64614 >> 8;

    // 4.启动
    TR0 = 1;

    // 5.初始化函数指数组
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        s_callbacks[i] = NULL;
    }
}

bit Dri_Timer0_RegisterCallback(Timer0_Callback callback)
{
    u8 i;
    // 确保不会重复注册
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == callback) {
            return 1;
        }
    }

    // 寻找坑位
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == NULL) {
            s_callbacks[i] = callback;
            return 1;
        }
    }

    return 0;
}

bit Dri_Timer0_DeregisterCallback(Timer0_Callback callback)
{
    u8 i;
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] == callback) {
            s_callbacks[i] = NULL;
            return 1;
        }
    }

    return 0;
}

void Dri_Timer0_Handler() interrupt 1
{
    u8 i;
    // 1.重新装载初始值
    TL0 = 64614;
    TH0 = 64614 >> 8;

    // 2.轮询函数指针数组
    for (i = 0; i < MAX_CALLBACK_COUNT; i++) {
        if (s_callbacks[i] != NULL) {
            s_callbacks[i]();
        }
    }
}