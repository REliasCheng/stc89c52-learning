# 18 · DS18B20 温度读取

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
通过 1-Wire 读取温度，并在 OLED 上显示。

## 使用硬件与引脚连接
P4.4 为 DS18B20 数据线；OLED 仍使用 P1.6/P1.7 I²C。[扩展板原理图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)。

## 核心原理与程序流程
`Int_DS18B20_GetTemperature()` 调用 `Dri_1Wire` 完成复位、跳过 ROM、启动转换、读暂存器；主循环经 OLED 显示结果，间隔约 500 ms。

## 代码结构
`Dri_1Wire` 是时序底层，`Int_DS18B20` 是温度器件层，OLED 由 `Dri_IIC` + `Int_OLED` 实现，`main.c` 将它们组合。

## 编译与烧录
EIDE/Keil_C51 Build，检查 HEX 后按[指南](../../docs/06_编译与烧录.md)下载。

## 实验现象与调试记录
预期 OLED 显示温度整数；尚未实测。原代码启动转换后立即读，未等待本次转换完成，结果需结合[DS18B20 手册](../../resources/datasheets/DS18B20.pdf)和硬件验证；见[待验证项](../../docs/07_调试记录.md)。

## 本节知识点
1-Wire 复位/读写时隙、温度转换与跨模块调用。
