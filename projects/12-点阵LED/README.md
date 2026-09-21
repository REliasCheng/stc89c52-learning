# 12 · 点阵 LED

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
让 74HC595 与 Timer0 配合显示静态点阵图案。

## 使用硬件与引脚连接
P1.0/P1.1/P1.2 连接 74HC595 的数据/锁存/移位时钟；P0 是共享数据口，P3.5 为点阵使能。具体连接见[扩展板图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)。

## 核心原理与程序流程
`Dri_Timer0_Init()` 建立刷新节奏，`Int_LEDMatrix_Init()` 初始化，`Int_LEDMatrix_SetPic(pic)` 将图案交给显示层。

## 代码结构
`main.c` 提供图案和入口；`Dri_Timer0` 是时基；`Int_LEDMatrix` 负责点阵/74HC595；`Com` 为公共工具。

## 编译与烧录
EIDE/Keil_C51 Build 后确认 HEX，再按[指南](../../docs/06_编译与烧录.md)烧录。

## 实验现象与调试记录
预期稳定显示源码 `pic` 图案；未实测。图形反转/错列时核对 595 级联顺序、行列极性和 P3.5 使能。

## 本节知识点
串入并出、锁存、扫描刷新与图案缓冲。
