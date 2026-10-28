#include "stc89_platform.h"

/* STC89C52RC AUXR=0x8E, EXTRAM=AUXR.1; see STC89C51RC-en.pdf 3.2.2/3.2.4. */
__sfr __at (0x8E) AUXR;

__xdata Stc89TargetDiagnostics stc89_diagnostics;

/* Parsed from the linked image by the build verifier, not Host sizeof(). */
const __code unsigned char stc89_abi_sizes[5] = {
    (unsigned char)sizeof(TerminalApp),
    (unsigned char)sizeof(TerminalPlatform),
    (unsigned char)sizeof(((TerminalPlatform *)0)->rtc_read),
    (unsigned char)sizeof(void *),
    (unsigned char)sizeof(Stc89TargetDiagnostics)
};

/* SDCC invokes this before static/XDATA initialization. EXTRAM=0 maps the
 * RC device's 256-byte on-chip auxiliary RAM to MOVX addresses 0x00..0xFF. */
unsigned char __sdcc_external_startup(void)
{
    AUXR &= (unsigned char)~0x02U;
    return 0U;
}

static void diagnostic_display(void *context, TerminalPage page,
                               const TerminalDateTime *date_time,
                               TerminalS16 temperature_tenths,
                               const TerminalConfig *config,
                               const TerminalReadStatus *read_status)
{
    Stc89TargetDiagnostics *diagnostics = (Stc89TargetDiagnostics *)context;
    (void)date_time;
    (void)temperature_tenths;
    (void)config;
    ++diagnostics->display_calls;
    diagnostics->last_page = (TerminalU8)page;
    diagnostics->rtc_unavailable = (TerminalU8)!read_status->rtc_has_value;
    diagnostics->temperature_unavailable =
        (TerminalU8)!read_status->temperature_has_value;
}

void stc89_platform_init(TerminalPlatform *platform)
{
    /* NULL means unavailable; no success-only placeholder driver exists. */
    platform->rtc_read = 0;
    platform->temperature_read = 0;
    platform->config_load = 0;
    platform->config_save = 0;
    platform->display = diagnostic_display;
    platform->context = &stc89_diagnostics;
}
