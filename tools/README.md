# 串口下载脚本

[`stcflash.py`](stcflash.py) 保留 laborer 的署名与 GPL-3.0-or-later 声明。仓库只保留一个副本；各主题下的 EIDE 工程从自身目录通过 `../../../../../tools/stcflash.py` 调用，基础模板从 `templates/eide-c51/` 使用 `../../tools/stcflash.py`。

这个脚本需要 `pyserial`。上传前仍要确认 HEX、设备型号、当前 COM 口及供电，详见[编译与烧录](../docs/编译与烧录.md)。脚本不随浏览或克隆仓库自动执行；它的 GPL 许可不被根目录 MIT 替代。
