# 点阵时间关系

[静态例子](../src/course/12-点阵LED/src/main.c) 的 `pic[8]` 只设置一次，后续显示靠 Timer0 和 `Int_LEDMatrix` 保持。[滚动例子](../src/course/13-点阵LED-滚动显示/src/main.c) 把 26 字节 `picture` 逐列送入，并在每次移位后延时 200 ms。

因此画面不稳时先看扫描中断/使能；滚动不顺时看 `Shift` 的缓冲更新和主循环延时。P0 复用与 74HC595 锁存顺序也应结合[扩展板原理图](../../../hardware/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)查，不能只看数组图案猜行列方向。
