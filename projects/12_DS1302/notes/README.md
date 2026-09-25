# 综合工程先拆开看

[入口代码](../src/course/19-DS1302/src/main.c) 依次用 `Int_DS1302_SetDate` 设置示例时刻、`GetDate` 读回、`Int_DS18B20_GetTemperature` 取温度、`Int_OLED_ShowStr` 显示。先分别检查 RTC 和温度，再看格式化/显示；否则屏幕上一处错误可能来自任何一层。

源文件在上电时重设时间，因此该流程用于显示链路检查。`WEEK_NAME[st_date.day_of_week - 1]` 依赖星期值落在 1–7，应用层需要校验这一输入。持续走时和后备电源可通过断电重启记录验证。
