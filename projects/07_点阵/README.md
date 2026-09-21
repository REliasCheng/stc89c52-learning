# 07 · 点阵：74HC595 与两种速度

[实验目录](../README.md) · [74HC595 接线](../../hardware/引脚映射.md)

[静态点阵](src/course/12-点阵LED/)将八字节图案交给 `Int_LEDMatrix_SetPic`，Timer0 负责持续扫描；[滚动显示](src/course/13-点阵LED-滚动显示/)每 200 ms 用 `Int_LEDMatrix_Shift` 推入一列新数据。P1.0/P1.1/P1.2 控制 74HC595，P3.5 是点阵使能，P0 与其他显示器件共享。

## 两条时间线不能混在一起

`P1.0 SER → 74HC595 数据`、`P1.2 SCK → 移位`、`P1.1 RCK → 锁存`，再由 P0 输出点阵数据，P3.5 控制点阵使能。`Int_LEDMatrix_Init()` 注册 Timer0 回调，`Int_LEDMatrix_RefreshByTimer0()` 每次处理中断时切到下一行；这条快时间线负责让图案持续可见。静态工程只调用一次 `SetPic(pic)`，主循环留空；滚动工程每隔 200 ms 调一次 `Shift(picture[i])`，是另一条慢时间线。

我从这里学到把“硬件扫描”与“画面内容变化”拆开：闪烁先追 Timer0/74HC595/使能，移动速度再追 26 字节 `picture` 和主循环延时。两个原工程和[时间关系笔记](notes/)都在；没有留存的点阵照片，因此不凭数组值宣称实物图案朝向。
