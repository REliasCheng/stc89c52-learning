# STC89C52RC 嵌入式开发实践

基于 STC89C52RC 教学板的 C51 外设开发与工程组织实践。仓库从端口控制推进到显示、输入、时间基准、通信和多器件协作，保留可打开的 Keil/EIDE 工程，并用板级信号路径和函数调用链解释实现。

## 项目简介

这套实践覆盖 GPIO、数码管与点阵显示、按键和蜂鸣器、外部中断与定时器、UART、软件 I²C、AT24C02、OLED、DS18B20 和 DS1302。展示重点是：如何从原理图确认接口，如何将总线时序与器件功能分层，以及如何沿构建、下载、驱动、应用四段链路定位问题。

仓库里的课程工程按原有版本保留在各模块的 src/course；我写的硬件分析、代码解读和调试记录放在 README、notes 与 docs。来源和许可集中见[资料说明](THIRD_PARTY_NOTICES.md)。

## 项目展示

| 教学扩展板 | 个人综合应用结构 |
| --- | --- |
| ![教学扩展板 V1.0 的原始 3D 渲染图](assets/images/extension-board-v1-render-a.png) | ![环境与时钟信息终端结构](assets/images/environment-clock-terminal-architecture.svg) |

左图为板卡资料中的渲染图；右图对应 `projects/13_环境与时钟信息终端` 的真实源码结构。[核心板图](assets/images/core-board-v1-render-a.png)与[原理图](hardware/README.md)可继续对照阅读。


## 核心技术能力

| 技术方向 | 工程中的实现与分析 |
| --- | --- |
| GPIO 与端口复用 | P0 低电平控制 LED；辨认 P0 在 LED、数码管段线、点阵数据之间的复用和各自使能。 |
| 显示驱动 | P1.3–P1.5 经 74HC138 位选，P0 经 74HC245 输出段码；用显示缓冲与循环刷新实现动态数码管。点阵使用 74HC595 与定时扫描。 |
| 输入与事件 | 独立按键消抖、矩阵行列扫描；比较轮询处理和 INT0 下降沿处理。 |
| 时间控制 | Timer0 重装、ISR 计数与回调注册；将点阵刷新和画面滚动放在不同时间尺度。 |
| 通信接口 | UART 中断收发与命令缓冲；软件 I²C 的起始、收发和应答；DS18B20 的 1-Wire 时隙及 DS1302 三线时序。 |
| 器件与组合应用 | AT24C02 分页写入、OLED 字符显示、DS18B20 温度读取、DS1302 日历读写及跨外设显示流程。 |

## 开发平台

- **主控与板卡**：STC89C52RC、11.0592 MHz 晶振、51 教学核心板与扩展板 V1.0；板载 CH340K 串口链路。
- **软件**：Windows、C51、Keil C51；早期工程使用 Keil，后续工程使用 VS Code + EIDE。下载使用 STC 串口工具或 EIDE 烧录任务。
- **板级接口**：P0 是多类输出资源的共享数据口；P1.6/P1.7 是 AT24C02 与 OLED 共用的软件 I²C 线。具体连接以[引脚说明](hardware/引脚映射.md)和原理图为准。

## 软件架构

课程工程展示了从直接控制寄存器到分层调用的演进。Application 是 main.c 中的应用流程，并非仓库中另建的文件夹：

    Application / main.c：初始化、输入处理、业务组合
                 ↓
    Int_*：键盘、显示、EEPROM、OLED、传感器等器件接口
                 ↓
    Dri_*：定时器、UART、I²C、1-Wire 等底层时序与寄存器操作
                 ↓
    STC89C52RC 端口与开发板外设

    Com_*：延时等公共组件，由需要它的层调用

架构随实验复杂度逐步演进：LED 等早期工程直接在 main.c 控制 IO，后续工程逐步引入 Com/Dri/Int。[工程结构](docs/工程结构.md)列出实际目录与版本差异，[架构演变](docs/架构演变.md)解释这种变化。

## 项目模块

20 个课程工程入口按硬件主题汇入 12 个模块；在这些驱动基础上，仓库增加了一个独立的个人综合应用。每个模块文档都给出信号路径、函数关系、关键代码和复现时的检查点。

| 模块 | 代表实践 | 入口 |
| --- | --- | --- |
| 01 LED | 单灯、闪烁、流水；GPIO 极性与整口输出 | [查看工程](projects/01_LED/) |
| 02 数码管 | 静态显示、动态扫描、接口封装 | [查看工程](projects/02_数码管/) |
| 03 按键 | 独立按键消抖、4×4 矩阵扫描 | [查看工程](projects/03_按键/) |
| 04 蜂鸣器 | 按键输入联动声音和显示 | [查看工程](projects/04_蜂鸣器/) |
| 05 外部中断 | P3.2 / INT0 下降沿触发 | [查看工程](projects/05_外部中断/) |
| 06 定时器 | Timer0 直接处理与回调封装 | [查看工程](projects/06_定时器/) |
| 07 点阵 | 74HC595、扫描刷新、滚动 | [查看工程](projects/07_点阵/) |
| 08 UART | 单字节与多字节命令接收 | [查看工程](projects/08_UART/) |
| 09 I²C / AT24C02 | 软件总线、分页写入、点阵数据读取 | [查看工程](projects/09_I2C与AT24C02/) |
| 10 OLED | I²C 命令/数据、字模显示 | [查看工程](projects/10_OLED/) |
| 11 DS18B20 | 1-Wire 温度读取与 OLED 输出 | [查看工程](projects/11_DS18B20/) |
| 12 DS1302 | RTC、温度和 OLED 的组合应用 | [查看工程](projects/12_DS1302/) |
| 13 环境与时钟信息终端 | RTC、温度、OLED、按键和EEPROM的个人应用层 | [查看工程](projects/13_环境与时钟信息终端/) |

## 获取、编译与下载

1. 克隆仓库或下载 ZIP，进入上表的模块目录。早期 LED 工程打开 .uvproj；后续工程打开对应 .code-workspace。
2. 在本机配置 Keil C51、EIDE 与 STC89C52RC 的工程环境，构建并确认本次生成的 HEX 路径。仓库不附带编译器和构建产物。
3. 连接教学板，确认设备管理器中的**当前** CH340K 串口，再用 STC 下载工具或工程中的 EIDE 烧录任务下载。端口号随电脑与插口变化，不应写死。
4. 按模块 README 核对使能脚、共享端口和显示/传感器连线。

[完整步骤](docs/编译与烧录.md) · [环境配置](docs/开发环境.md) · [常见问题](docs/常见问题.md)

## 工程记录

- [硬件理解](docs/我的硬件理解.md)：从板图追 P0 共享、译码位选、总线与器件连接。
- [工程思考](docs/我的工程思考.md)：Timer0 回调、驱动接口与多外设资源安排。
- [学习与调试记录](docs/学习记录.md)：曾遇到的 pyserial、HEX 路径、COM 口变化等环境问题及处理；[调试索引](docs/调试记录.md)按症状查找。
- [课程路线与项目总结](docs/学习路线.md)：按工程推进顺序查看技术跨度。

实物板照片、运行截图、串口或示波器记录与演示视频将按对应模块补入。现在可以先从工程和板图阅读实现，再按文档在自己的设备上复现。

## 技术路线中的位置

[Embedded-Systems-Foundations](https://github.com/REliasCheng/Embedded-Systems-Foundations) → [Embedded-C-Cpp-Learning](https://github.com/REliasCheng/Embedded-C-Cpp-Learning) → **stc89c52-learning** → [BlueBridgeCup-MCU](https://github.com/REliasCheng/BlueBridgeCup-MCU)
