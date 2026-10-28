#ifndef STC89_TARGET_PLATFORM_H
#define STC89_TARGET_PLATFORM_H

#include "terminal_app.h"

/* Software-only diagnostics; no sensor, EEPROM, display, or board I/O. */
typedef struct {
    volatile TerminalU32 loop_ticks;
    volatile TerminalU8 synthetic_events;
    volatile TerminalU8 display_calls;
    volatile TerminalU8 last_page;
    volatile TerminalU8 rtc_unavailable;
    volatile TerminalU8 temperature_unavailable;
    volatile TerminalU8 save_unavailable;
} Stc89TargetDiagnostics;

extern __xdata Stc89TargetDiagnostics stc89_diagnostics;
extern const __code unsigned char stc89_abi_sizes[5];
void stc89_platform_init(TerminalPlatform *platform);

#endif
