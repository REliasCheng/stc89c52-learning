# Third-Party Notices

This branch distributes the repository-authored application core, target adapter, tests, documentation and diagrams. It does not distribute historical course projects, vendor source/header files, external DS1302/DS18B20/OLED/EEPROM/key drivers, Keil runtime, SDCC binaries/runtime, or generated HEX.

The target adapter's sole device register declaration (`AUXR` at `0x8E`, `EXTRAM` bit 1) was written from the [STC89C51RC/RD+ series datasheet](https://www.stcmicro.com/datasheet/STC89C51RC-en.pdf), chapter 3; no vendor header was copied. SDCC and uCsim are external GPL-licensed build/test tools, obtained from the [official SDCC project](https://sdcc.sourceforge.net/) or the Ubuntu packages. The repository MIT license applies only to repository-owned materials, not to those external tools or device documentation. Earlier Git history remains reachable and requires separate rights review.
