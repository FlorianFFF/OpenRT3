// Calendar conversions (0x51d390..0x51d5f0).
#include "calendar.h"
#include "hook/hook.h"
#include <string.h>

namespace rt3 {

static inline uint64_t Pack(const GameDate& d) {
    uint64_t v;
    memcpy(&v, &d, 8);
    return v;
}

static inline int32_t DaysOf(const GameDate& d) { // days since year 0, month 1, day 1
    return (d.year * 12 + d.month) * 28 + d.day - 29;
}

int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute) {
    return (((year * 12 + month) * 28 - 29 + day) * 24 + hour) * 60 + minute;
}
RT3_REPLACE(0x51d390, MinutesFromParts);

int32_t __fastcall DateToMinutes(const GameDate* d) {
    return (DaysOf(*d) * 24 + d->hour) * 60 + d->minute;
}
RT3_REPLACE(0x51d3c0, DateToMinutes);

uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
    GameDate d;
    d.year = year;
    d.month = month;
    d.week = (uint8_t)(day / 7 + 1);
    d.day = day;
    d.hour = hour;
    d.minute = minute;
    d.quarter = (uint8_t)(hour / 6 + 1);
    return Pack(d);
}
RT3_REPLACE(0x51d3f0, MakeDate);

uint64_t __fastcall DateFromMinutes(int32_t m) {
    GameDate d;
    d.year = (uint16_t)(m / kMinutesPerYear);
    m -= d.year * kMinutesPerYear; // QUIRK: uses the 16-bit year, as the original
    d.month = (uint8_t)(m / kMinutesPerMonth + 1);
    m -= (d.month - 1) * kMinutesPerMonth;
    d.day = (uint8_t)(m / kMinutesPerDay + 1);
    m -= (d.day - 1) * kMinutesPerDay;
    uint8_t hour = (uint8_t)(m / 60);
    d.week = (uint8_t)(d.day / 7 + 1);
    d.hour = hour;
    d.minute = (uint8_t)(m - hour * 60);
    d.quarter = (uint8_t)(hour / 6 + 1);
    return Pack(d);
}
RT3_REPLACE(0x51d460, DateFromMinutes);

uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d) {
    return DateFromMinutes(minutes + (DaysOf(d) * 24 + d.hour) * 60 + d.minute);
}
RT3_REPLACE(0x51d550, DateAddMinutes);

uint64_t __fastcall DateAddDays(int32_t days, GameDate d) {
    return DateFromMinutes(((days + DaysOf(d)) * 24 + d.hour) * 60 + d.minute);
}
RT3_REPLACE(0x51d5a0, DateAddDays);

int32_t __stdcall DateDiffMinutes(GameDate a, GameDate b) {
    return ((((a.year - b.year) * 12 - b.month + a.month) * 28 - b.day + a.day) * 24
            - b.hour + a.hour) * 60 - b.minute + a.minute;
}
RT3_REPLACE(0x51d5f0, DateDiffMinutes);

} // namespace rt3
