# 来源与许可

仓库内容分为课程示例、板卡与器件资料、个人文档和个人实践代码。

| 内容 | 位置 | 来源与处理 |
| --- | --- | --- |
| 尚硅谷课程示例工程 | `projects/*/src/course/` | 保留课程源码的教学逻辑与注释；仓库调整了目录、注释编码和共享烧录脚本的相对路径。 |
| 板卡与器件资料 | `hardware/`、`assets/images/` | 来自教学板资料和器件厂商，用于核对引脚、接口和时序。 |
| 个人技术文档 | 根 README、`docs/`、各主题 README/notes | 根据现有代码、工程配置和原理图编写。 |
| 个人实践代码 | `projects/13_环境与时钟信息终端/practice/` | 包含应用状态、主机端测试、C51 适配入口和带转换等待的 DS18B20 接口。 |

根目录 MIT License 适用于仓库维护者有权授权的代码和文档。课程示例、板卡图纸、芯片手册及工具分别适用其原始许可条件。

其他第三方内容：

- `tools/stcflash.py` 保留原作者 laborer 的声明，采用 GPL-3.0-or-later；[GPL 条文](https://www.gnu.org/licenses/gpl-3.0.html)。
- `projects/01_LED/src/course/00-点亮LED` 至 `02-LED-流水` 的 `STARTUP.A51` 是 Keil 工程附带的启动代码，适用相应工具链条款。
- `templates/eide-c51/` 来自课程基础模板。

复用具体文件时，请同时查阅对应来源中的许可说明。
