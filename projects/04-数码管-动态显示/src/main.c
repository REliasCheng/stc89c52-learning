#include <STC89C5xRC.H>
#include <INTRINS.H>
#define SMG_EN P36
#define LED_EN P34
typedef unsigned char u8;
typedef unsigned char u16;
typedef unsigned long u32;

// 数字0-9的编码
static u8 codes[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

u8 buffer[8];

void Delay1ms(u16 count) //@11.0592MHz
{
    u8 i, j;

    while (count > 0) {
        count--;
        _nop_();
        i = 2;
        j = 199;
        do {
            while (--j);
        } while (--i);
    }
}

/**
 * @brief 内部方法，让数码管某一位显示特定数字
 *
 * @param position 片选, 从左到右[0-7]
 * @param num_code 显示想要的数字编码
 */
void DigitalTube_DisplaySingle(u8 position, u8 num_code)
{
    P0 = 0x00;
    // 位选：P15 P14 P13
    position <<= 3;
    P1 &= 0xC7;
    P1 |= position;

    // 段选：P0
    P0 = num_code;
}

/**
 * @brief 设置显存数组
 *
 * @param num 待展示数字
 */
void DigitalTube_DisplayNum(u32 num)
{
    u8 i;
    for (i = 0; i < 8; i++) {
        buffer[i] = 0x00;
    }

    if (num == 0) {
        buffer[7] = codes[0];
        return;
    }

    i = 7;
    while (num > 0) {
        buffer[i] = codes[num % 10];
        num /= 10;
        i--;
    }
}

/**
 * @brief 动态扫描
 *
 */
void DigitalTube_Refresh()
{
    u8 i;
    for (i = 0; i < 8; i++) {
        DigitalTube_DisplaySingle(i, buffer[i]);
        Delay1ms(1);
    }
}

void main()
{
    SMG_EN = 0;
    LED_EN = 0;
    DigitalTube_DisplayNum(1234567);
    while (1) {
        DigitalTube_Refresh();
    }
}