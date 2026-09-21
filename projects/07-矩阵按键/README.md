# 07 · 矩阵按键

[实验索引](../README.md) · [引脚映射](../../docs/04_引脚映射.md)

## 实验目标
扫描矩阵按键，并把键值显示到数码管。

## 使用硬件与引脚连接
P2.0–P2.3 是行，P2.4–P2.7 是列；显示用 P0、P1.3–P1.5 和 P3.6。

## 核心原理与程序流程
`Int_KeyMatrix` 逐行/列读取，`main.c` 得到键值后调用 `Int_DigitalTube_DisplayNum(key)`，循环 `Refresh()`。

## 代码结构
`src/Int` 包含 `Int_KeyMatrix` 和 `Int_DigitalTube`；`src/Com` 供延时/类型；`main.c` 组合两者。

## 编译与烧录
EIDE 选 Keil_C51 Build，检查 HEX 后按[指南](../../docs/06_编译与烧录.md)烧录。

## 实验现象与调试记录
预期按不同键显示对应键号；未实测。若串键/误判，按扩展板原理图核对行列顺序与同时按多键行为。

## 本节知识点
行列扫描、键值映射、显示与输入任务协同。
