# 03 · 数码管静态显示

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
通过段码和位选点亮指定的一位数码管。

## 使用硬件与引脚连接
P0 输出段码，P1.3–P1.5 接 74HC138 位选；P3.6 是数码管使能，P3.4 是 LED 使能。详见[扩展板图](../../resources/schematics/extension-board/SCH_51教学扩展板V1.0_2024-05-30.pdf)。

## 核心原理与程序流程
段码表 `codes[]` 将数字转为七段位型；`DigitalTube_DisplaySingle(7, codes[6])` 选择一位并写 P0。

## 代码结构
`src/main.c`；`.eide/eide.yml` 是旧版 EIDE 格式，`.code-workspace` 配置 GBK 等编辑选项。

## 编译与烧录
用 EIDE 打开工程并选择 Keil_C51 Build；旧格式若不兼容，按 EIDE 提示迁移副本。检查 `build/Release/` HEX 后可用 STC-ISP；EIDE 上传命令已指向共享脚本。[指南](../../docs/06_编译与烧录.md)。

## 实验现象与调试记录
预期第 7 位显示 6（以源代码位序为准）；本次未实测。先核对 74HC138 地址和使能电平。

## 本节知识点
段选、位选、P0 数据复用与 EIDE 工程格式。
