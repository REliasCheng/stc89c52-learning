#include "stc89_platform.h"

#define SYNTHETIC_INPUT 1

static __xdata TerminalApp target_app;
static __xdata TerminalPlatform target_platform;

void main(void)
{
    unsigned char software_divider = 0U;
    unsigned char event_index = 0U;
    TerminalKey key;

    stc89_platform_init(&target_platform);
    terminal_app_init(&target_app, &target_platform);

    for (;;) {
        /* This is a software iteration, not a measured 100 ms tick. */
        terminal_app_tick(&target_app);
        ++stc89_diagnostics.loop_ticks;
        ++software_divider;
        if (software_divider != 0U) {
            continue;
        }

#if SYNTHETIC_INPUT
        /* Deterministic events exercise page, adjustment and unavailable save.
         * They do not represent a physical key or a real timebase. */
        if (event_index == 0U || event_index == 1U) {
            key = TERMINAL_KEY_NEXT;
        } else if (event_index == 2U) {
            key = TERMINAL_KEY_ADJUST;
        } else if (event_index == 3U) {
            key = TERMINAL_KEY_SAVE;
        } else {
            key = TERMINAL_KEY_PREVIOUS;
        }
        terminal_app_handle_key(&target_app, key);
        ++stc89_diagnostics.synthetic_events;
        if (target_app.save_status == TERMINAL_SAVE_UNAVAILABLE) {
            stc89_diagnostics.save_unavailable = 1U; /* sticky diagnostic */
        }
        event_index = (unsigned char)((event_index + 1U) % 5U);
#endif
    }
}
