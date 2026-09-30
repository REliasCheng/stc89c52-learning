#ifndef TERMINAL_APP_H
#define TERMINAL_APP_H

typedef unsigned char TerminalU8;
typedef signed char TerminalS8;
typedef unsigned long TerminalU32;
typedef signed int TerminalS16;

typedef enum {
    TERMINAL_PAGE_CLOCK = 0,
    TERMINAL_PAGE_TEMPERATURE,
    TERMINAL_PAGE_SETTINGS,
    TERMINAL_PAGE_COUNT
} TerminalPage;

typedef enum {
    TERMINAL_KEY_NONE = 0,
    TERMINAL_KEY_NEXT,
    TERMINAL_KEY_PREVIOUS,
    TERMINAL_KEY_ADJUST,
    TERMINAL_KEY_SAVE
} TerminalKey;

typedef struct {
    TerminalU8 year;
    TerminalU8 month;
    TerminalU8 day;
    TerminalU8 weekday;
    TerminalU8 hour;
    TerminalU8 minute;
    TerminalU8 second;
} TerminalDateTime;

typedef struct {
    TerminalU8 page;
    TerminalS8 temperature_offset;
} TerminalConfig;

typedef struct {
    TerminalU8 (*rtc_read)(void *context, TerminalDateTime *date_time);
    TerminalU8 (*temperature_read)(void *context, TerminalS16 *temperature_tenths);
    TerminalU8 (*config_load)(void *context, TerminalConfig *config);
    TerminalU8 (*config_save)(void *context, const TerminalConfig *config);
    void (*display)(void *context, TerminalPage page, const TerminalDateTime *date_time,
                    TerminalS16 temperature_tenths, const TerminalConfig *config);
    void *context;
} TerminalPlatform;

typedef struct {
    TerminalPlatform platform;
    TerminalConfig config;
    TerminalDateTime date_time;
    TerminalS16 temperature_tenths;
    TerminalU8 tick_divider;
    TerminalU8 config_dirty;
} TerminalApp;

void terminal_app_init(TerminalApp *app, const TerminalPlatform *platform);
void terminal_app_handle_key(TerminalApp *app, TerminalKey key);
void terminal_app_tick(TerminalApp *app);

#endif
