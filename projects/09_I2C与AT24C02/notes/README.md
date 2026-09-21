# 为什么把 I²C 和 EEPROM 放一起

[16-EEPROM 的入口](../src/course/16-EEPROM/src/main.c) 用 `Int_EEPROM_WriteBytes(0, picture, 26)` 写入，再 `ReadBytes(0, buffer, 26)` 读回。`Dri_IIC` 是总线底层，不是一份可单独下载的 I²C 实验；点阵显示则是读取结果的可视化，不等于严格的数据校验。

这份工程体现了跨页写和读回显示，但没有逐字节比较，所以点阵显示不能代替存储校验。错误地址、断电保持和与 OLED 共线时的总线状态均未留存结果；这里把它们作为接口的可靠性边界，而不是编成“已通过”测试。
