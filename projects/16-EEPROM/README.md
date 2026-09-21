# 16 · AT24C02 EEPROM

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
经软件 I²C 写入并读回 EEPROM 数据，再在点阵上展示。

## 使用硬件与引脚连接
P1.6 SDA / P1.7 SCL 接 AT24C02；点阵沿用 P1.0–P1.2 的 74HC595、P0 和 P3.5。参照[扩展板原理图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)。

## 核心原理与程序流程
`main.c` 把 26 字节 `picture` 写到地址 0，再读入 `buffer`；循环 `Int_LEDMatrix_Shift(buffer[i])` 观察读回内容。

## 代码结构
`Dri_IIC` 实现总线位操作；`Int_EEPROM` 封装器件读写；`Dri_Timer0` + `Int_LEDMatrix` 提供显示；`main.c` 做读写验证。

## 编译与烧录
EIDE/Keil_C51 Build 并检查 HEX，用[指南](../../docs/06_编译与烧录.md)下载。

## 实验现象与调试记录
预期 EEPROM 数据读回后点阵滚动显示；未实测。注意 EEPROM 写周期、页边界与设备地址，读回成功不应仅靠有图案推断，后续可加逐字节比较记录。

## 本节知识点
I²C 起停、应答、AT24C02 写/读与非易失数据。
