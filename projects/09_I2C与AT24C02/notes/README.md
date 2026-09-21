# 为什么把 I²C 和 EEPROM 放一起

[16-EEPROM 的入口](../src/course/16-EEPROM/src/main.c) 用 `Int_EEPROM_WriteBytes(0, picture, 26)` 写入，再 `ReadBytes(0, buffer, 26)` 读回。`Dri_IIC` 是总线底层，`Int_EEPROM` 处理器件控制字节和页边界；点阵将读回内容可视化。

这份工程体现了跨页写和读回显示。若要把它扩展为存储可靠性实验，下一步应增加逐字节比较、跨页边界测试，以及断电重启后按地址读回的记录。
