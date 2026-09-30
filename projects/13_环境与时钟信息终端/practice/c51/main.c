#include "terminal_app.h"
#include "Int_DS18B20_Safe.h"

#include "Int_DS1302.h"
#include "Int_EEPROM.h"
#include "Int_Key.h"
#include "Int_OLED.h"
#include "Com_Util.h"
#include <STDIO.H>

#define CONFIG_ADDRESS 0x40
#define CONFIG_MAGIC   0x52

static TerminalApp g_app;

static TerminalU8 platform_rtc_read(void *context, TerminalDateTime *date_time)
{
    Struct_Date value;
    (void)context;
    Int_DS1302_GetDate(&value);
    date_time->year = value.year;
    date_time->month = value.month;
    date_time->day = value.day;
    date_time->weekday = value.day_of_week;
    date_time->hour = value.hour;
    date_time->minute = value.minute;
    date_time->second = value.second;
    return 1;
}

static TerminalU8 platform_temperature_read(void *context, TerminalS16 *temperature_tenths)
{
    (void)context;
    *temperature_tenths = Int_DS18B20_ReadTenths();
    return 1;
}

static TerminalU8 platform_config_load(void *context, TerminalConfig *config)
{
    unsigned char bytes[3];
    (void)context;
    Int_EEPROM_ReadBytes(CONFIG_ADDRESS, bytes, 3);
    if (bytes[0] != CONFIG_MAGIC) {
        return 0;
    }
    config->page = bytes[1];
    config->temperature_offset = (signed char)bytes[2];
    return 1;
}

static TerminalU8 platform_config_save(void *context, const TerminalConfig *config)
{
    unsigned char bytes[3];
    (void)context;
    bytes[0] = CONFIG_MAGIC;
    bytes[1] = config->page;
    bytes[2] = (unsigned char)config->temperature_offset;
    Int_EEPROM_WriteBytes(CONFIG_ADDRESS, bytes, 3);
    return 1;
}

static void platform_display(void *context, TerminalPage page, const TerminalDateTime *date_time,
                             TerminalS16 temperature_tenths, const TerminalConfig *config)
{
    unsigned char line[17];
    signed int shown_temperature = temperature_tenths + (signed int)config->temperature_offset * 10;
    (void)context;
    Int_OLED_ShowStr(0, 0, "                ");
    Int_OLED_ShowStr(0, 1, "                ");
    if (page == TERMINAL_PAGE_CLOCK) {
        sprintf(line, "20%02u/%02u/%02u", date_time->year, date_time->month, date_time->day);
        Int_OLED_ShowStr(0, 0, line);
        sprintf(line, "%02u:%02u:%02u", date_time->hour, date_time->minute, date_time->second);
        Int_OLED_ShowStr(0, 1, line);
    } else if (page == TERMINAL_PAGE_TEMPERATURE) {
        Int_OLED_ShowStr(0, 0, "Temperature");
        sprintf(line, "%d.%d C", shown_temperature / 10,
                shown_temperature < 0 ? -(shown_temperature % 10) : shown_temperature % 10);
        Int_OLED_ShowStr(0, 1, line);
    } else {
        Int_OLED_ShowStr(0, 0, "Display offset");
        sprintf(line, "%+d C  SW4 Save", (int)config->temperature_offset);
        Int_OLED_ShowStr(0, 1, line);
    }
}

static TerminalKey read_key(void)
{
    if (Int_Key_IsSW1Pressed()) return TERMINAL_KEY_NEXT;
    if (Int_Key_IsSW2Pressed()) return TERMINAL_KEY_PREVIOUS;
    if (Int_Key_IsSW3Pressed()) return TERMINAL_KEY_ADJUST;
    if (Int_Key_IsSW4Pressed()) return TERMINAL_KEY_SAVE;
    return TERMINAL_KEY_NONE;
}

void main(void)
{
    TerminalPlatform platform;
    Int_OLED_Init();
    Int_OLED_Clear();
    Int_DS1302_Init();

    platform.rtc_read = platform_rtc_read;
    platform.temperature_read = platform_temperature_read;
    platform.config_load = platform_config_load;
    platform.config_save = platform_config_save;
    platform.display = platform_display;
    platform.context = 0;
    terminal_app_init(&g_app, &platform);

    while (1) {
        terminal_app_handle_key(&g_app, read_key());
        Com_Util_Delay1ms(100);
        terminal_app_tick(&g_app);
    }
}
