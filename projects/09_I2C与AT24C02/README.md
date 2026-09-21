# 09 · 软件 I²C 与 AT24C02

[实验目录](../README.md) · [板级引脚](../../hardware/引脚映射.md)

原目录没有一个独立编号的“I²C 练习工程”；真正可运行的例子是[16-EEPROM](src/course/16-EEPROM/)。其中 `Dri_IIC` 负责 P1.6/P1.7 的总线动作，`Int_EEPROM` 负责 AT24C02 的地址与读写，`main.c` 写入 26 字节图案、读回，再让点阵滚动显示。

## 从电平时序到存储数据

`P1.6 SDA / P1.7 SCL → AT24C02`；[扩展板图第 8 页](../../hardware/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)还画出 SDA 的 10 kΩ 上拉。`Dri_IIC` 只负责起始、停止、逐位发送/读取和 ACK；`Int_EEPROM` 才加入器件控制字节 `0xA0`（写）/`0xA1`（读）、内部字节地址、16 字节页边界与写后 5 ms 等待。`main()` 写入 26 字节点阵图案、读回到 `buffer`，再通过 `Int_LEDMatrix` 与 Timer0 让图案滚动。调用链是 `main → Int_EEPROM → Dri_IIC`，显示另走点阵接口；读回用于显示，不等于逐字节数据校验。

这个工程让我把“总线能发字节”与“器件能按页可靠写入”分开。跨页写时 `WriteBytes` 先算当前页剩余量，再分段调用 `WritePage`；这是比只背 I²C 起始条件更重要的器件约束。课程资料没有独立编号的 I²C 可下载工程，所以本主题以 `16-EEPROM` 为真实入口。[笔记](notes/)标出读回校验、页边界和断电保持这几个测试维度；仓库没有对应的原始测试数值，不能编造。
