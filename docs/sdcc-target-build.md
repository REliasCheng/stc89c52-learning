# STC89C52RC SDCC Terminal Core Target Build

This is a software-only target foundation. It links the existing original `terminal_app.c` into an SDCC MCS-51 image with a repository-authored entry and diagnostic platform adapter. It is distinct from the unbuilt Keil board application in `practice/c51/main.c`, which still requires external peripheral drivers.

## Device and toolchain contract

The [STC89C52RC product listing](https://www.stcmicro.com/STC/STC89C52RC.html) and [STC89C51RC/RD+ series datasheet](https://www.stcmicro.com/datasheet/STC89C51RC-en.pdf), chapter 3, establish an 8051-compatible instruction set, 8 KiB internal application CODE (`0x0000`–`0x1FFF`), 256 bytes directly/indirectly addressable scratchpad RAM, and 256 bytes of RC on-chip auxiliary RAM at MOVX addresses `0x00`–`0xFF` when `AUXR.1` (`EXTRAM`) is clear. The 512-byte total is **256 scratchpad + 256 MOVX auxiliary**, not 512 bytes of ordinary IDATA or an assumed off-chip XRAM. `AUXR` at `0x8E` is the only MCU-specific register declared by this foundation. The original startup hook clears `EXTRAM` before SDCC initializes globals; the rest of startup and runtime come from SDCC. The generic C52 simulator does not model that STC-specific physical RAM switch.

Local Windows verification used the official [SDCC 4.6.4 #17092 snapshot](https://sourceforge.net/projects/sdcc/files/snapshot_builds/x86_64-w64-mingw32/sdcc-snapshot-x86_64-w64-mingw32-20261026-17092-cf-x86.zip/download), ZIP SHA-256 `88b5b308662517e2d9643b1103d9f5d748b44a934c75ba6f61d8ee0c3e965efd`. CI installs [Ubuntu 24.04 `sdcc`](https://packages.ubuntu.com/noble/sdcc) and [`sdcc-ucsim`](https://packages.ubuntu.com/noble/sdcc-ucsim), currently 4.2.0. These GPL-licensed tools are external build dependencies; no compiler, runtime, vendor header, course file, Keil component, or generated image is redistributed in this repository. See [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md) and the [SDCC manual](https://sdcc.sourceforge.net/doc/sdccman.pdf) for the `--stack-auto`, memory-limit, and startup contracts.

## What the image does

The three compiled source files are:

1. `projects/13_环境与时钟信息终端/practice/core/terminal_app.c` — unchanged original state machine;
2. `target/sdcc/stc89_platform.c` — `AUXR.EXTRAM` startup and a diagnostic display sink;
3. `target/sdcc/main.c` — static target application, software ticks and explicitly labelled `SYNTHETIC_INPUT` key events.

`TerminalPlatform` leaves RTC, temperature, config load and config save callbacks NULL. The core therefore retains unavailable read status and reports save unavailable; no fixed sensor value or fake EEPROM success is produced. The display callback writes **diagnostic RAM state**, not an OLED. The loop calls `terminal_app_tick()` on software iterations, without a timer or measured 100 ms cadence. Events exercise NEXT, ADJUST, SAVE and PREVIOUS paths. There is no ISR, physical key, UART, RTC, 1-Wire, OLED, EEPROM, or board GPIO in this image.

## Reproduce

Use a new output directory outside the repository for each build:

```powershell
py -3.10 scripts/build_sdcc_target.py --sdcc C:\path\to\sdcc.exe --output C:\path\to\fresh-build-1
py -3.10 scripts/build_sdcc_target.py --sdcc C:\path\to\sdcc.exe --output C:\path\to\fresh-build-2
py -3.10 scripts/run_sdcc_simulator.py --build C:\path\to\fresh-build-1 --simulator C:\path\to\s51.exe
```

On Linux use `python3`, `--sdcc sdcc`, and `--simulator s51`. Compare the two HEX files byte for byte. The script refuses an existing or in-repository output directory. Each output retains `build.log`, `build-report.json`, `.rel`, `.map`, `.mem`, `.ihx`, and packed `.hex`; the optional instruction test additionally writes `simulator.log` and `simulator-report.json`. Build artifacts remain outside the repository.

All three translation units use `-mmcs51 --std-c11 --model-small --stack-auto --Werror`. The linker uses `--iram-size 256 --xram-size 256 --code-size 8192 --stack-size 64 --out-fmt-ihx`. `packihx` creates the Intel HEX. The verifier checks record lengths, checksums, EOF, every code address below `0x2000`, identical IHX/packed data, linked `main` and all three `terminal_app_*` functions, the startup hook, memory report, and ABI sizes read from the actual linked CODE image. `--stack-auto` is applied consistently to support nested calls, but the linker reservation is not a proof of every possible runtime path's maximum stack depth.

## Local results, 2026-10-28

| Evidence | Result |
| --- | --- |
| SDCC 4.6.4 compile/link | PASS, three objects; 0 errors and 0 warnings |
| CODE | 3,921 / 8,192 bytes |
| Direct DATA / allocated IDATA | 1 / 0 bytes in linker layout; register bank and bit area separate |
| On-chip MOVX auxiliary RAM | 55 / 256 bytes; no external XRAM assumed |
| Stack | Starts `0x21`, 64 bytes reserved by linker; no hardware high-water measurement |
| Target ABI | `sizeof(TerminalApp)=32`, `TerminalPlatform=13`, function pointer=2, generic `void *`=3, diagnostics=10 bytes |
| Packed HEX | 11,147 bytes; SHA-256 `44aa5792bd42552e42f84db452bab0dfba73c14f0dd3a50fbdfcf7a23ddd14df` |
| Clean rebuild | Two fresh directories, byte-identical HEX |
| Host regression | GCC: 20 terminal-core tests plus DS18B20 adapter cases pass; Clang and ASan/UBSan run in CI |
| Generic C52 uCsim | 500,000 target instructions executed; 14 synthetic events, 135 diagnostic display calls, unavailable RTC/temperature/save observed; highest observed SP `0x5E` |

The simulator runs the linked MCS-51 image and checks the diagnostic XRAM bytes. It uses a generic C52 CPU/memory model, so it **does not** verify STC89C52RC AUXR mapping, actual timing, board wiring, or peripherals. CI records its own SDCC version, memory report, HEX SHA and simulator result; different compiler versions may produce different images, while each version's two fresh builds must match.

## Next evidence

The original Keil board application remains unbuilt. Actual board/package, oscillator and 6T/12T setting, legal DS1302/DS18B20/OLED/EEPROM/key drivers, real tick source, pin wiring, flash operation, physical sensor responses, display output, EEPROM persistence and worst-case stack/timing must be established separately before making board-runtime claims. No hardware action was authorized in this task.
