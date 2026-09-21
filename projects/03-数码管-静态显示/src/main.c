#include <STC89C5xRC.H>
#define SMG_EN P36
#define LED_EN P34
typedef unsigned char u8;

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

/**
 * @brief 内部方法，让数码管某一位显示特定数字
 *
 * @param position 片选, 从左到右[0-7]
 * @param num_code 显示想要的数字编码
 */
void DigitalTube_DisplaySingle(u8 position, u8 num_code)
{
    // 位选：P15 P14 P13
    position <<= 3;
    P1 &= 0xC7;
    P1 |= position;

    // 段选：P0
    P0 = num_code;
}

void main()
{
    SMG_EN = 0;
    LED_EN = 0;
    DigitalTube_DisplaySingle(7, codes[6]);
    while (1) {
    }
}