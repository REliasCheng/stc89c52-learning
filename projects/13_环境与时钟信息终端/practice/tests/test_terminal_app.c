#include "terminal_app.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    TerminalU8 rtc_ok, temp_ok, load_ok, save_ok, partial;
    TerminalDateTime rtc;
    TerminalS16 temp;
    TerminalConfig stored;
    TerminalReadStatus displayed;
    TerminalDateTime displayed_date;
    TerminalS16 displayed_temp;
    unsigned rtc_calls, temp_calls, load_calls, save_calls, renders;
} Fake;

static Fake fake_default(void)
{
    Fake f = {0};
    f.rtc_ok = f.temp_ok = f.load_ok = f.save_ok = 1;
    f.rtc.year = 26; f.rtc.month = 9; f.rtc.day = 23; f.rtc.weekday = 3;
    f.rtc.hour = 10; f.rtc.minute = 30; f.rtc.second = 1;
    f.temp = 253;
    f.stored.page = TERMINAL_PAGE_TEMPERATURE;
    f.stored.temperature_offset = -1;
    return f;
}

static TerminalU8 read_rtc(void *context, TerminalDateTime *out)
{
    Fake *f = context;
    ++f->rtc_calls;
    if (!f->rtc_ok) { if (f->partial) out->month = 255; return 0; }
    *out = f->rtc;
    return 1;
}
static TerminalU8 read_temp(void *context, TerminalS16 *out)
{
    Fake *f = context;
    ++f->temp_calls;
    if (!f->temp_ok) { if (f->partial) *out = 999; return 0; }
    *out = f->temp;
    return 1;
}
static TerminalU8 load_config(void *context, TerminalConfig *out)
{
    Fake *f = context;
    ++f->load_calls;
    if (!f->load_ok) { if (f->partial) out->page = 255; return 0; }
    *out = f->stored;
    return 1;
}
static TerminalU8 save_config(void *context, const TerminalConfig *value)
{
    Fake *f = context;
    ++f->save_calls;
    if (!f->save_ok) return 0;
    f->stored = *value;
    return 1;
}
static void display(void *context, TerminalPage page, const TerminalDateTime *date,
                    TerminalS16 temp, const TerminalConfig *config, const TerminalReadStatus *status)
{
    Fake *f = context;
    (void)page; (void)config;
    ++f->renders;
    f->displayed = *status;
    f->displayed_date = *date;
    f->displayed_temp = temp;
}
static TerminalPlatform platform_for(Fake *f)
{
    TerminalPlatform p;
    p.rtc_read = read_rtc; p.temperature_read = read_temp;
    p.config_load = load_config; p.config_save = save_config;
    p.display = display; p.context = f;
    return p;
}
static void next_sample(TerminalApp *app)
{
    unsigned i;
    for (i = 0; i < 10U; ++i) terminal_app_tick(app);
}

static void normal_flow(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    assert(app.config_loaded && app.config.page == TERMINAL_PAGE_TEMPERATURE);
    assert(app.config.temperature_offset == -1 && app.read_status.rtc_sample_ok);
    terminal_app_handle_key(&app, TERMINAL_KEY_NEXT);
    terminal_app_handle_key(&app, TERMINAL_KEY_ADJUST);
    assert(app.config.page == TERMINAL_PAGE_SETTINGS && app.config.temperature_offset == 0);
    terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(f.save_calls == 1U && !app.config_dirty && app.save_status == TERMINAL_SAVE_OK);
    next_sample(&app);
    assert(f.rtc_calls == 2U && f.temp_calls == 2U);
    terminal_app_handle_key(&app, TERMINAL_KEY_PREVIOUS);
    assert(app.config.page == TERMINAL_PAGE_TEMPERATURE);
}
static void null_arguments(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(0, &p); terminal_app_init(&app, 0);
    terminal_app_handle_key(0, TERMINAL_KEY_SAVE); terminal_app_tick(0);
    assert(f.renders == 0U);
}
static void first_failure(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    memset(&app, 0xA5, sizeof app);
    f.rtc_ok = f.temp_ok = 0; f.partial = 1;
    terminal_app_init(&app, &p);
    assert(!app.read_status.rtc_has_value && !app.read_status.rtc_sample_ok);
    assert(!app.read_status.temperature_has_value && !app.read_status.temperature_sample_ok);
    assert(app.date_time.year == 0 && app.date_time.month == 0 && app.date_time.day == 0);
    assert(app.date_time.weekday == 0 && app.date_time.hour == 0);
    assert(app.date_time.minute == 0 && app.date_time.second == 0);
    assert(app.temperature_tenths == 0 && !f.displayed.rtc_has_value);
    assert(!app.config_dirty && app.save_status == TERMINAL_SAVE_NOT_REQUESTED);
}
static void rtc_failure_recovery(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    f.rtc_ok = 0; f.partial = 1; next_sample(&app);
    assert(app.read_status.rtc_has_value && !app.read_status.rtc_sample_ok);
    assert(app.date_time.month == 9 && f.displayed_date.month == 9);
    f.rtc_ok = 1; f.rtc.second = 42; next_sample(&app);
    assert(app.read_status.rtc_sample_ok && app.date_time.second == 42);
}
static void rtc_first_failure_recovery(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.rtc_ok = 0; terminal_app_init(&app, &p);
    f.rtc_ok = 1; next_sample(&app);
    assert(app.read_status.rtc_has_value && app.read_status.rtc_sample_ok);
}
static void invalid_rtc(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.rtc.month = 13; terminal_app_init(&app, &p);
    assert(!app.read_status.rtc_has_value);
    f.rtc.month = 2; f.rtc.day = 30; next_sample(&app);
    assert(!app.read_status.rtc_has_value);
    f.rtc.day = 28; f.rtc.hour = 24; next_sample(&app);
    assert(!app.read_status.rtc_has_value);
    f.rtc.hour = 23; f.rtc.minute = 60; next_sample(&app);
    assert(!app.read_status.rtc_has_value);
    f.rtc.minute = 59; f.rtc.second = 60; next_sample(&app);
    assert(!app.read_status.rtc_has_value);
    f.rtc.second = 59; next_sample(&app);
    assert(app.read_status.rtc_sample_ok && app.date_time.day == 28);
}
static void leap_day(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.rtc.year = 24; f.rtc.month = 2; f.rtc.day = 29;
    terminal_app_init(&app, &p);
    assert(app.read_status.rtc_sample_ok);
    f.rtc.year = 25; next_sample(&app);
    assert(!app.read_status.rtc_sample_ok && app.date_time.year == 24);
}
static void zero_and_negative_temperature(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.temp = 0; terminal_app_init(&app, &p);
    assert(app.read_status.temperature_has_value && app.read_status.temperature_sample_ok);
    assert(app.temperature_tenths == 0);
    f.temp = -55; next_sample(&app);
    assert(app.read_status.temperature_sample_ok && app.temperature_tenths == -55);
}
static void temperature_failure_recovery(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    f.temp_ok = 0; f.partial = 1; next_sample(&app);
    assert(app.read_status.temperature_has_value && !app.read_status.temperature_sample_ok);
    assert(app.temperature_tenths == 253 && f.displayed_temp == 253);
    f.temp_ok = 1; f.temp = 0; next_sample(&app);
    assert(app.read_status.temperature_sample_ok && app.temperature_tenths == 0);
}
static void temperature_first_failure_recovery(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.temp_ok = 0; terminal_app_init(&app, &p);
    assert(!app.read_status.temperature_has_value);
    f.temp_ok = 1; next_sample(&app);
    assert(app.read_status.temperature_has_value && app.read_status.temperature_sample_ok);
}
static void missing_read_callbacks(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    p.rtc_read = 0; p.temperature_read = 0;
    terminal_app_init(&app, &p); next_sample(&app);
    assert(!f.rtc_calls && !f.temp_calls);
    assert(!app.read_status.rtc_has_value && !app.read_status.temperature_has_value);
}
static void save_failure_retry(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    terminal_app_handle_key(&app, TERMINAL_KEY_NEXT);
    f.save_ok = 0; terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(app.config_dirty && app.save_status == TERMINAL_SAVE_FAILED);
    assert(f.stored.page == TERMINAL_PAGE_TEMPERATURE);
    f.save_ok = 1; terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(!app.config_dirty && app.save_status == TERMINAL_SAVE_OK);
    assert(f.stored.page == TERMINAL_PAGE_SETTINGS && f.save_calls == 2U);
}
static void missing_save_callback(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    p.config_save = 0; terminal_app_init(&app, &p);
    terminal_app_handle_key(&app, TERMINAL_KEY_NEXT);
    terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(app.config_dirty && app.save_status == TERMINAL_SAVE_UNAVAILABLE);
    assert(!f.save_calls);
}
static void load_failure_and_invalid_config(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    f.load_ok = 0; f.partial = 1; terminal_app_init(&app, &p);
    assert(!app.config_loaded && app.config.page == TERMINAL_PAGE_CLOCK);
    f.load_ok = 1; f.stored.page = 255; terminal_app_init(&app, &p);
    assert(!app.config_loaded && app.config.page == TERMINAL_PAGE_CLOCK);
    f.stored.page = TERMINAL_PAGE_SETTINGS; f.stored.temperature_offset = 6;
    terminal_app_init(&app, &p);
    assert(!app.config_loaded && app.config.temperature_offset == 0);
    f.stored.temperature_offset = -5; terminal_app_init(&app, &p);
    assert(app.config_loaded && app.config.page == TERMINAL_PAGE_SETTINGS);
}
static void missing_load_callback(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    p.config_load = 0; terminal_app_init(&app, &p);
    assert(!app.config_loaded && app.config.page == TERMINAL_PAGE_CLOCK);
}
static void load_success_then_failure(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    assert(app.config_loaded && app.config.page == TERMINAL_PAGE_TEMPERATURE);
    f.load_ok = 0; f.partial = 1;
    terminal_app_init(&app, &p);
    assert(!app.config_loaded && app.config.page == TERMINAL_PAGE_CLOCK);
    assert(app.config.temperature_offset == 0);
    f.load_ok = 1; terminal_app_init(&app, &p);
    assert(app.config_loaded && app.config.page == TERMINAL_PAGE_TEMPERATURE);
}
static void page_offset_boundaries(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    unsigned i;
    terminal_app_init(&app, &p);
    terminal_app_handle_key(&app, TERMINAL_KEY_PREVIOUS);
    assert(app.config.page == TERMINAL_PAGE_CLOCK);
    terminal_app_handle_key(&app, TERMINAL_KEY_PREVIOUS);
    assert(app.config.page == TERMINAL_PAGE_SETTINGS);
    for (i = 0; i < 7U; ++i) terminal_app_handle_key(&app, TERMINAL_KEY_ADJUST);
    assert(app.config.temperature_offset == -5);
    terminal_app_handle_key(&app, (TerminalKey)255);
    assert(app.config.page == TERMINAL_PAGE_SETTINGS && app.config.temperature_offset == -5);
}
static void save_status_reset(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    terminal_app_init(&app, &p);
    terminal_app_handle_key(&app, TERMINAL_KEY_NEXT);
    terminal_app_handle_key(&app, TERMINAL_KEY_SAVE);
    assert(app.save_status == TERMINAL_SAVE_OK);
    terminal_app_handle_key(&app, TERMINAL_KEY_ADJUST);
    assert(app.save_status == TERMINAL_SAVE_NOT_REQUESTED && app.config_dirty);
}
static void sampling_cadence_reinit(void)
{
    Fake f = fake_default(); TerminalPlatform p = platform_for(&f); TerminalApp app;
    unsigned i;
    terminal_app_init(&app, &p);
    for (i = 0; i < 9U; ++i) terminal_app_tick(&app);
    assert(f.rtc_calls == 1U && f.temp_calls == 1U);
    terminal_app_tick(&app);
    assert(f.rtc_calls == 2U && f.temp_calls == 2U);
    terminal_app_init(&app, &p);
    assert(app.tick_divider == 0U && app.save_status == TERMINAL_SAVE_NOT_REQUESTED);
}

static unsigned count;
static void run(const char *name, void (*test)(void))
{
    test(); ++count; printf("PASS %s\n", name);
}
int main(void)
{
    run("normal_flow", normal_flow);
    run("null_arguments", null_arguments);
    run("first_failure", first_failure);
    run("rtc_failure_recovery", rtc_failure_recovery);
    run("rtc_first_failure_recovery", rtc_first_failure_recovery);
    run("invalid_rtc", invalid_rtc);
    run("leap_day", leap_day);
    run("zero_and_negative_temperature", zero_and_negative_temperature);
    run("temperature_failure_recovery", temperature_failure_recovery);
    run("temperature_first_failure_recovery", temperature_first_failure_recovery);
    run("missing_read_callbacks", missing_read_callbacks);
    run("save_failure_retry", save_failure_retry);
    run("missing_save_callback", missing_save_callback);
    run("load_failure_and_invalid_config", load_failure_and_invalid_config);
    run("missing_load_callback", missing_load_callback);
    run("load_success_then_failure", load_success_then_failure);
    run("page_offset_boundaries", page_offset_boundaries);
    run("save_status_reset", save_status_reset);
    run("sampling_cadence_reinit", sampling_cadence_reinit);
    printf("%u terminal application tests passed\n", count);
    return 0;
}
