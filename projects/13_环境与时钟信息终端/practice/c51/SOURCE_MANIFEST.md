# C51工程源文件清单

新建Keil/EIDE工程时加入以下源文件，并将对应目录加入头文件搜索路径。

## 个人应用

- `practice/c51/main.c`
- `practice/c51/Int_DS18B20_Safe.c`
- `practice/core/terminal_app.c`

## 复用的课程驱动

- `projects/12_DS1302/src/course/19-DS1302/src/Com/Com_Util.c`
- `projects/12_DS1302/src/course/19-DS1302/src/Dri/Dri_1Wire.c`
- `projects/12_DS1302/src/course/19-DS1302/src/Dri/Dri_IIC.c`
- `projects/12_DS1302/src/course/19-DS1302/src/Int/Int_DS1302.c`
- `projects/12_DS1302/src/course/19-DS1302/src/Int/Int_OLED.c`
- `projects/09_I2C与AT24C02/src/course/16-EEPROM/src/Int/Int_EEPROM.c`
- `projects/03_按键/src/course/06-独立按键/src/Int/Int_Key.c`

不要同时加入课程版`Int_DS18B20.c`，本项目使用`Int_DS18B20_Safe.c`。OLED与EEPROM共用19号工程中的`Dri_IIC.c`，避免把两个同名I²C实现重复链接。
