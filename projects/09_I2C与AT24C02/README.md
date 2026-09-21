# 09 · 软件 I²C 与 AT24C02

[实验目录](../README.md) · [板级引脚](../../hardware/引脚映射.md)

原目录没有一个独立编号的“I²C 练习工程”；真正可运行的例子是[16-EEPROM](src/course/16-EEPROM/)。其中 `Dri_IIC` 负责 P1.6/P1.7 的总线动作，`Int_EEPROM` 负责 AT24C02 的地址与读写，`main.c` 写入 26 字节图案、读回，再让点阵滚动显示。

所以这里没有编造一个 08_I2C 工程。先沿 `main → Int_EEPROM → Dri_IIC` 追调用，再对照 AT24C02 手册看页写与等待；[笔记](notes/)列出需要验证的地方。当前没有 EEPROM 写入次数或读回逐字节比较的实测记录。
