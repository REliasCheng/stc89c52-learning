#include "terminal_app.h"
#include "Int_DS18B20_Safe.h"

#include "Int_DS1302.h"
#include "Int_EEPROM.h"
#include "Int_Key.h"
#include "Int_OLED.h"
#include "Com_Util.h"

#define CONFIG_ADDRESS 0x40
#define CONFIG_MAGIC   0x52

static TerminalApp g_app;

static TerminalU8 platform_rtc_read(void *context, TerminalDateTime *date_time)
{
    Struct_Date value = {0};
    (void)context;
    Int_DS1302_GetDate(&value);
    date_time->year = value.year;
    date_time->month = value.month;
    date_time->day = value.day;
    date_time->weekday = value.day_of_week;
    date_time->hour = value.hour;
    date_time->minute = value.minute;
    date_time->second = value.second;
    /* The external driver has no error return; reject obviously invalid output. */
    return value.month >= 1U && value.month <= 12U && value.day >= 1U &&
           value.day <= 31U && value.hour <= 23U && value.minute <= 59U &&
           value.second <= 59U;
}

static TerminalU8 platform_temperature_read(void *context, TerminalS16 *temperature_tenths)
{
    (void)context;
    *temperature_tenths = Int_DS18B20_ReadTenths();
    return 1;
}

static TerminalU8 platform_config_load(void *context, TerminalConfig *config)
{
    unsigned char bytes[3] = {0};
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
    unsigned char readback[3];
    (void)context;
    bytes[0] = CONFIG_MAGIC;
    bytes[1] = config->page;
    bytes[2] = (unsigned char)config->temperature_offset;
    readback[0] = (unsigned char)~bytes[0];
    readback[1] = (unsigned char)~bytes[1];
    readback[2] = (unsigned char)~bytes[2];
    Int_EEPROM_WriteBytes(CONFIG_ADDRESS, bytes, 3);
    Int_EEPROM_ReadBytes(CONFIG_ADDRESS, readback, 3);
    /* Readback detects a mismatch, not power-loss-safe persistence. */
    return readback[0] == bytes[0] && readback[1] == bytes[1] && readback[2] == bytes[2];
}

static void write_two_digits(unsigned char *destination, TerminalU8 value)
{
    destination[0] = (unsigned char)('0' + value / 10U);
    destination[1] = (unsigned char)('0' + value % 10U);
}

static void format_temperature(unsigned char line[17], signed long value, TerminalU8 stale)
{
    unsigned long magnitude = value < 0 ? (unsigned long)-value : (unsigned long)value;
    unsigned long whole = magnitude / 10UL;
    unsigned char digits[5];
    unsigned char digit_count = 0;
    unsigned char length = 0;
    if (value < 0) line[length++] = '-';
    do {
        digits[digit_count++] = (unsigned char)('0' + whole % 10UL);
        whole /= 10UL;
    } while (whole != 0UL);
    while (digit_count != 0U) line[length++] = digits[--digit_count];
    line[length++] = '.';
    line[length++] = (unsigned char)('0' + magnitude % 10UL);
    line[length++] = ' ';
    line[length++] = 'C';
    if (stale) {
        line[length++] = ' ';
        line[length++] = 's'; line[length++] = 't'; line[length++] = 'a';
        line[length++] = 'l'; line[length++] = 'e';
    }
    line[length] = '\0'; /* maximum: "-3281.8 C stale" (15 characters) */
}

static void platform_display(void *context, TerminalPage page, const TerminalDateTime *date_time,
                             TerminalS16 temperature_tenths, const TerminalConfig *config,
                             const TerminalReadStatus *read_status)
{
    unsigned char line[17];
    signed long shown_temperature = (signed long)temperature_tenths +
                                    (signed long)config->temperature_offset * 10L;
    (void)context;
    Int_OLED_ShowStr(0, 0, "                ");
    Int_OLED_ShowStr(0, 1, "                ");
    if (page == TERMINAL_PAGE_CLOCK) {
        if (!read_status->rtc_has_value) {
            Int_OLED_ShowStr(0, 0, "RTC unavailable");
            return;
        }
        /* Core validation bounds every date/time field to two digits. */
        line[0] = '2'; line[1] = '0';
        write_two_digits(&line[2], date_time->year);
        line[4] = '/'; write_two_digits(&line[5], date_time->month);
        line[7] = '/'; write_two_digits(&line[8], date_time->day);
        line[10] = '\0';
        Int_OLED_ShowStr(0, 0, line);
        write_two_digits(&line[0], date_time->hour);
        line[2] = ':'; write_two_digits(&line[3], date_time->minute);
        line[5] = ':'; write_two_digits(&line[6], date_time->second);
        line[8] = '\0';
        if (!read_status->rtc_sample_ok) {
            line[8] = ' ';
            line[9] = 's'; line[10] = 't'; line[11] = 'a';
            line[12] = 'l'; line[13] = 'e'; line[14] = '\0';
        }
        Int_OLED_ShowStr(0, 1, line);
    } else if (page == TERMINAL_PAGE_TEMPERATURE) {
        Int_OLED_ShowStr(0, 0, "Temperature");
        if (!read_status->temperature_has_value) {
            Int_OLED_ShowStr(0, 1, "Unavailable");
            return;
        }
        format_temperature(line, shown_temperature, !read_status->temperature_sample_ok);
        Int_OLED_ShowStr(0, 1, line);
    } else {
        Int_OLED_ShowStr(0, 0, "Display offset");
        line[0] = config->temperature_offset < 0 ? '-' : '+';
        line[1] = (unsigned char)('0' + (config->temperature_offset < 0 ?
                                           -config->temperature_offset : config->temperature_offset));
        line[2] = ' '; line[3] = 'C'; line[4] = ' ';
        line[5] = ' '; line[6] = 'S'; line[7] = 'W'; line[8] = '4';
        line[9] = ' '; line[10] = 'S'; line[11] = 'a'; line[12] = 'v';
        line[13] = 'e'; line[14] = '\0';
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
