# 串口下载脚本

[`stcflash.py`](stcflash.py) 来自原课程工程所带的同名文件，文件头保留 laborer 的署名与 GPL-3.0-or-later 声明。仓库只留一个副本；各主题下的 EIDE 工程从自身目录通过 `../../../../../tools/stcflash.py` 调用。课程基础模板仍在 `templates/eide-c51/`，它使用 `../../tools/stcflash.py`。

这个脚本需要 `pyserial`。上传前仍要确认 HEX、设备型号、当前 COM 口及供电，详见[编译与烧录](../docs/编译与烧录.md)。脚本不随浏览或克隆仓库自动执行；它的 GPL 许可不被根目录 MIT 替代。
