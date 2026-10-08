#include "terminal_app.h"

static TerminalU8 valid_config(const TerminalConfig *config)
{
    return config->page < (TerminalU8)TERMINAL_PAGE_COUNT &&
           config->temperature_offset >= -5 && config->temperature_offset <= 5;
}

static TerminalU8 valid_date_time(const TerminalDateTime *value)
{
    static const TerminalU8 days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    TerminalU8 last_day;
    if (value->year > 99U || value->month < 1U || value->month > 12U ||
        value->hour > 23U || value->minute > 59U || value->second > 59U) {
        return 0;
    }
    last_day = days_in_month[value->month - 1U];
    if (value->month == 2U && value->year % 4U == 0U) {
        ++last_day; /* year is 00..99 and represents 2000..2099 */
    }
    return value->day >= 1U && value->day <= last_day;
}

static void sample_inputs(TerminalApp *app)
{
    TerminalDateTime date_time = {0};
    TerminalS16 temperature_tenths = 0;

    app->read_status.rtc_sample_ok = 0;
    if (app->platform.rtc_read != 0 &&
        app->platform.rtc_read(app->platform.context, &date_time) &&
        valid_date_time(&date_time)) {
        app->date_time = date_time;
        app->read_status.rtc_has_value = 1;
        app->read_status.rtc_sample_ok = 1;
    }

    app->read_status.temperature_sample_ok = 0;
    if (app->platform.temperature_read != 0 &&
        app->platform.temperature_read(app->platform.context, &temperature_tenths)) {
        app->temperature_tenths = temperature_tenths;
        app->read_status.temperature_has_value = 1;
        app->read_status.temperature_sample_ok = 1;
    }
}

static void render(TerminalApp *app)
{
    if (app->platform.display != 0) {
        app->platform.display(app->platform.context, (TerminalPage)app->config.page,
                              &app->date_time, app->temperature_tenths, &app->config,
                              &app->read_status);
    }
}

void terminal_app_init(TerminalApp *app, const TerminalPlatform *platform)
{
    TerminalConfig stored = {0};
    if (app == 0 || platform == 0) {
        return;
    }
    app->platform = *platform;
    app->config.page = (TerminalU8)TERMINAL_PAGE_CLOCK;
    app->config.temperature_offset = 0;
    app->date_time.year = 0;
    app->date_time.month = 0;
    app->date_time.day = 0;
    app->date_time.weekday = 0;
    app->date_time.hour = 0;
    app->date_time.minute = 0;
    app->date_time.second = 0;
    app->temperature_tenths = 0;
    app->read_status.rtc_has_value = 0;
    app->read_status.rtc_sample_ok = 0;
    app->read_status.temperature_has_value = 0;
    app->read_status.temperature_sample_ok = 0;
    app->save_status = TERMINAL_SAVE_NOT_REQUESTED;
    app->config_loaded = 0;
    app->tick_divider = 0;
    app->config_dirty = 0;
    if (app->platform.config_load != 0 &&
        app->platform.config_load(app->platform.context, &stored) && valid_config(&stored)) {
        app->config = stored;
        app->config_loaded = 1;
    }
    sample_inputs(app);
    render(app);
}

void terminal_app_handle_key(TerminalApp *app, TerminalKey key)
{
    if (app == 0) {
        return;
    }
    if (key == TERMINAL_KEY_NEXT) {
        app->config.page = (TerminalU8)((app->config.page + 1U) % (TerminalU8)TERMINAL_PAGE_COUNT);
        app->config_dirty = 1;
        app->save_status = TERMINAL_SAVE_NOT_REQUESTED;
    } else if (key == TERMINAL_KEY_PREVIOUS) {
        app->config.page = app->config.page == 0U ?
                               (TerminalU8)(TERMINAL_PAGE_COUNT - 1) :
                               (TerminalU8)(app->config.page - 1U);
        app->config_dirty = 1;
        app->save_status = TERMINAL_SAVE_NOT_REQUESTED;
    } else if (key == TERMINAL_KEY_ADJUST && app->config.page == (TerminalU8)TERMINAL_PAGE_SETTINGS) {
        ++app->config.temperature_offset;
        if (app->config.temperature_offset > 5) {
            app->config.temperature_offset = -5;
        }
        app->config_dirty = 1;
        app->save_status = TERMINAL_SAVE_NOT_REQUESTED;
    } else if (key == TERMINAL_KEY_SAVE && app->config_dirty) {
        if (app->platform.config_save == 0) {
            app->save_status = TERMINAL_SAVE_UNAVAILABLE;
        } else if (app->platform.config_save(app->platform.context, &app->config)) {
            app->config_dirty = 0;
            app->save_status = TERMINAL_SAVE_OK;
        } else {
            app->save_status = TERMINAL_SAVE_FAILED;
        }
    }
    render(app);
}

void terminal_app_tick(TerminalApp *app)
{
    if (app == 0) {
        return;
    }
    ++app->tick_divider;
    if (app->tick_divider >= 10U) {
        app->tick_divider = 0;
        sample_inputs(app);
    }
    render(app);
}
