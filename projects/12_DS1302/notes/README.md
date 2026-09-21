# 综合工程先拆开看

[入口代码](../src/course/19-DS1302/src/main.c) 依次用 `Int_DS1302_SetDate` 设置示例时刻、`GetDate` 读回、`Int_DS18B20_GetTemperature` 取温度、`Int_OLED_ShowStr` 显示。先分别检查 RTC 和温度，再看格式化/显示；否则屏幕上一处错误可能来自任何一层。

上电重设时间是源文件行为，不是电池保持功能的验证。`WEEK_NAME[st_date.day_of_week - 1]` 依赖星期值落在 1–7，属于应用层需要守住的输入边界。仓库没有独立 DS1302 芯片手册和走时原始记录，故寄存器时序与保持性能不凭记忆补写。
