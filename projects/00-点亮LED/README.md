# 00 · 点亮 LED

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
建立可编译、可下载、可观察的最小 51 程序。

## 使用硬件与引脚连接
STC89C52RC 核心板、扩展板 LED；`P0.0` 对应 LED 数据，P3.4 是 LED 使能。具体高低有效以[扩展板原理图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)核对。

## 核心原理与程序流程
初始化外设使能后，在 `main()` 将 `P00` 置为 0；输出保持，观察对应 LED。

## 代码结构
课程原名 `helloworld`；根目录 `main.c`、`STARTUP.A51`、`helloworld.uvproj` 是 Keil 平铺工程。此阶段没有 `Com/Dri/Int`。

## 编译与烧录
Keil 打开 `.uvproj`，Build 得到 `Objects/` 中的 HEX；确认当前串口/芯片/晶振，用 STC-ISP 下载。详见[操作指南](../../docs/06_编译与烧录.md)。

## 实验现象与调试记录
预期指定 LED 常亮；尚未在本次仓库整理中实测。如无现象先查 P3.4 使能、LED 极性、上电和烧录成功信息。

## 本节知识点
GPIO 输出、Keil 工程与“源码→HEX→下载→观察”闭环。
