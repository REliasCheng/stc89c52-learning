# 软件 I²C 与 AT24C02｜从总线时序到分页存储

[返回项目模块](../README.md) · [板卡引脚](../../hardware/引脚映射.md) · [工程源码](src/course/16-EEPROM/)

[16 EEPROM 工程](src/course/16-EEPROM/)实现完整的软件 I²C 与 AT24C02 数据链路：把 26 字节点阵图案写入 EEPROM，读回缓冲后送给点阵滚动显示。总线基本动作、分页写入和应用数据流分别由驱动层、接口层与 `main` 组织。

## 总线与器件

STC89C52RC P1.6/SDA、P1.7/SCL → AT24C02；扩展板图中 SDA 还有 10 kΩ 上拉。OLED 后续也挂在这两根线上，因此主控按器件地址访问不同设备。AT24C02 存数据，不负责点阵显示；读出的字节另经 P0 与 74HC595 相关通路送向点阵，Timer0 负责扫描。总线与显示是两个不同的硬件链路。

## 读写流程

应用链路：main → Int_EEPROM_WriteBytes/ReadBytes → Dri_IIC 的起始、发送、接收、应答、停止 → AT24C02。显示链路：main 的 buffer → Int_LEDMatrix 的 Shift/刷新 → Dri_Timer0 回调 → 点阵。Dri_IIC 不知道“页”或“图案”，Int_EEPROM 负责 AT24C02 控制字节和分页，main 决定这批数据用于滚动显示。

### 事务与分页

- **Dri_IIC_Start/Stop/SendByte/ReceiveByte/SendAck**：用 P1.6/P1.7 的电平变化实现总线时序，器件层通过它们组合一次完整事务。
- **0xA0 / 0xA1**：工程使用的 AT24C02 写/读控制字节，方向位不同。
- **WriteBytes → WritePage**：页大小按 16 字节处理。跨页时先算当前页还能容纳多少，再分段写下一页，避免数据在页内回卷。
- **写后约 5 ms 等待**：为 EEPROM 内部写周期留出时间；读回到 buffer 是后续显示的数据来源，而非逐字节对比断言。
- 本工程的 Dri_IIC 按位掩码发送；OLED 工程保留另一种高位移出的实现，两个驱动版本不合并。

## 页边界

总线层负责起始、字节收发和 ACK；EEPROM 接口处理控制字节、内部地址和页边界；应用决定写入的数据内容。

## I²C 检查

写入后显示异常时，先看 SDA/SCL 与上拉，再核 Start/ACK/器件地址；随后查是否跨 16 字节页、写后等待以及读回地址。点阵部分另查 Timer0 和显示使能，避免把显示故障直接归因于 EEPROM。[版本笔记](notes/)记录了本工程与 OLED 工程的 I²C 实现差别。若需要验证数据保持，应增加独立比对与断电重启记录，而不是仅凭滚动图案判断。
