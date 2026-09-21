# 为什么把 I²C 和 EEPROM 放一起

[16-EEPROM 的入口](../src/course/16-EEPROM/src/main.c) 用 `Int_EEPROM_WriteBytes(0, picture, 26)` 写入，再 `ReadBytes(0, buffer, 26)` 读回。`Dri_IIC` 是总线底层，不是一份可单独下载的 I²C 实验；点阵显示则是读取结果的可视化，不等于严格的数据校验。

后续本人复现时应加逐字节比较，记录错误地址、跨页写和上电再读结果；同时确认与 OLED 共用 P1.6/P1.7 时的地址和总线状态。现在这些是计划，不写成已验证结论。
