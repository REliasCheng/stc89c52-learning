# OLED 显示前先查什么

[示例 `main.c`](../src/course/17-OLED/src/main.c) 初始化后只写一次内容，没有持续刷屏；如果屏幕全黑，应从供电、P1.6/P1.7、I²C 地址与初始化命令查起，而不是直接改 `Hello World!` 或字模。

资料中有 OLED 与 SSD1315 的参考 PDF，存放在 [`hardware/datasheets`](../../../hardware/datasheets/)；实际控制器型号仍以器件标识为准。代码能确定示例地址 `0x78`、初始化与清屏命令，不能从没有留存的照片推断屏幕方向或亮度。
