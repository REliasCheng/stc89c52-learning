# 17 · OLED 显示

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
以软件 I²C 初始化 OLED，显示字符串和有符号数字。

## 使用硬件与引脚连接
OLED 与 EEPROM 共用 P1.6 SDA / P1.7 SCL，靠 I²C 地址区分设备；见[扩展板图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)。

## 核心原理与程序流程
`Int_OLED_Init()`、`Clear()` 后，主程序调用 `ShowStr(0,0,"Hello World!")` 和 `ShowNum(0,1,-12345678)`。

## 代码结构
`Dri_IIC` 负责总线，`Int_OLED` 包含显示命令/字体与坐标接口，`main.c` 只编排显示，`Com` 为公共工具。

## 编译与烧录
EIDE/Keil_C51 Build；确认新 HEX 和端口后下载。[指南](../../docs/06_编译与烧录.md)。

## 实验现象与调试记录
预期 OLED 两行显示文本/数字；本次未实测。若全黑先查供电、设备地址、初始化命令与 I²C 应答，不要先改字体数组。

## 本节知识点
SSD1315/OLED 初始化、GDDRAM 地址、文本显示与总线复用。
