#include <STC89C5xRC.H> //包含STC89C52的头文件

void main()
{
    // 让P00引脚输出低电平
    P00 = 0;
    // 死循环卡住单片机，让它不要继续往下执行
    while (1);
}
