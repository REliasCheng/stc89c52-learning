# 烧录辅助工具

[首页](../README.md) · [编译与烧录](../docs/06_编译与烧录.md)

`stcflash.py` 是原工程携带的脚本，原文件头署名 laborer，按 GPL-3.0-or-later 授权；本仓库保留一个与原工程相同的副本，不以 MIT 重授权。EIDE 工程由各项目目录执行 `python ../../tools/stcflash.py -p ${port} "${hexFile}"`，依赖 Python 的 `pyserial` 和有效的串口/HEX。脚本**不会在浏览仓库时自动执行**；烧录前核对设备型号、端口和供电，参见 [FAQ](../docs/08_常见问题.md)。

历史 EIDE 工程曾各带同一脚本，仓库集中保存后仅调整工程配置相对路径。STC-ISP 也可作为独立 GUI 烧录方式，安装程序不随仓库分发。
