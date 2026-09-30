#include "terminal_app.h"

static TerminalU8 valid_config(const TerminalConfig *config)
{
    return config->page < (TerminalU8)TERMINAL_PAGE_COUNT &&
           config->temperature_offset >= -5 && config->temperature_offset <= 5;
}

static void render(TerminalApp *app)
{
    if (app->platform.display != 0) {
        app->platform.display(app->platform.context, (TerminalPage)app->config.page,
                              &app->date_time, app->temperature_tenths, &app->config);
    }
}

void terminal_app_init(TerminalApp *app, const TerminalPlatform *platform)
{
    TerminalConfig stored;
    if (app == 0 || platform == 0) {
        return;
    }
    app->platform = *platform;
    app->config.page = (TerminalU8)TERMINAL_PAGE_CLOCK;
    app->config.temperature_offset = 0;
    app->temperature_tenths = 0;
    app->tick_divider = 0;
    app->config_dirty = 0;
    if (app->platform.config_load != 0 &&
        app->platform.config_load(app->platform.context, &stored) && valid_config(&stored)) {
        app->config = stored;
    }
    if (app->platform.rtc_read != 0) {
        (void)app->platform.rtc_read(app->platform.context, &app->date_time);
    }
    if (app->platform.temperature_read != 0) {
        (void)app->platform.temperature_read(app->platform.context, &app->temperature_tenths);
    }
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
    } else if (key == TERMINAL_KEY_PREVIOUS) {
        app->config.page = app->config.page == 0U ?
                               (TerminalU8)(TERMINAL_PAGE_COUNT - 1) :
                               (TerminalU8)(app->config.page - 1U);
        app->config_dirty = 1;
    } else if (key == TERMINAL_KEY_ADJUST && app->config.page == (TerminalU8)TERMINAL_PAGE_SETTINGS) {
        ++app->config.temperature_offset;
        if (app->config.temperature_offset > 5) {
            app->config.temperature_offset = -5;
        }
        app->config_dirty = 1;
    } else if (key == TERMINAL_KEY_SAVE && app->config_dirty && app->platform.config_save != 0) {
        if (app->platform.config_save(app->platform.context, &app->config)) {
            app->config_dirty = 0;
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
        if (app->platform.rtc_read != 0) {
            (void)app->platform.rtc_read(app->platform.context, &app->date_time);
        }
        if (app->platform.temperature_read != 0) {
            (void)app->platform.temperature_read(app->platform.context, &app->temperature_tenths);
        }
    }
    render(app);
}
