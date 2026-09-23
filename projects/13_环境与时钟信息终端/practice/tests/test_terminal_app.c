#include "terminal_app.h"

#include <assert.h>
#include <stdio.h>

typedef struct {
    unsigned rtc_reads;
    unsigned temperature_reads;
    unsigned saves;
    unsigned renders;
    TerminalConfig stored;
} MockPlatform;

static TerminalU8 mock_rtc(void *context, TerminalDateTime *date_time)
{
    MockPlatform *mock = context;
    ++mock->rtc_reads;
    date_time->year = 26;
    date_time->month = 9;
    date_time->day = 23;
    date_time->weekday = 3;
    date_time->hour = 10;
    date_time->minute = 30;
    date_time->second = (TerminalU8)mock->rtc_reads;
    return 1;
}

static TerminalU8 mock_temperature(void *context, TerminalS16 *temperature_tenths)
{
    MockPlatform *mock = context;
    ++mock->temperature_reads;
    *temperature_tenths = 253;
    return 1;
}

static TerminalU8 mock_load(void *context, TerminalConfig *config)
{
    MockPlatform *mock = context;
    *config = mock->stored;
    return 1;
}

static TerminalU8 mock_save(void *context, const TerminalConfig *config)
{
    MockPlatform *mock = context;
    mock->stored = *config;
    ++mock->saves;
    return 1;
}

static void mock_display(void *context, TerminalPage page, const TerminalDateTime *date_time,
                         TerminalS16 temperature_tenths, const TerminalConfig *config)
{
    MockPlatform *mock = context;
    (void)page;
    (void)date_time;
    (void)temperature_tenths;
    (void)config;
    ++mock->renders;
}

int main(void)
{
    MockPlatform mock = {0};
    TerminalPlatform platform;
    TerminalApp app;
    unsigned i;

    mock.stored.page = (TerminalU8)TERMINAL_PAGE_TEMPERATURE;
    mock.stored.temperature_offset = -1;
    platform.rtc_read = mock_rtc;
    platform.temperature_read = mock_temperature;
    platform.config_load = mock_load;
    platform.config_save = mock_save;
    platform.display = mock_display;
    platform.context = &mock;

    terminal_app_init(&app, &platform);
    assert(app.config.page == (TerminalU8)TERMINAL_PAGE_TEMPERATURE);
    assert(app.config.temperature_offset == -1);
    assert(mock.rtc_reads == 1U && mock.temperature_reads == 1U);

    terminal_app_handle_key(&app, TERMINAL_KEY_NEXT);
    assert(app.config.page == (TerminalU8)TERMINAL_PAGE_SETTINGS);
    terminal_app_handle_key(&app, TERMINAL_KEY_ADJUST);
    assert(app.config.temperature_offset == 0);
    assert(app.config_dirty == 1U);
    terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(mock.saves == 1U && app.config_dirty == 0U);

    for (i = 0U; i < 10U; ++i) {
        terminal_app_tick(&app);
    }
    assert(mock.rtc_reads == 2U && mock.temperature_reads == 2U);
    assert(app.temperature_tenths == 253);
    assert(mock.renders >= 13U);

    terminal_app_handle_key(&app, TERMINAL_KEY_PREVIOUS);
    assert(app.config.page == (TerminalU8)TERMINAL_PAGE_TEMPERATURE);
    puts("terminal application tests passed");
    return 0;
}
