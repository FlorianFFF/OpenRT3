// Game calendar. Time is counted in minutes; a year has 12 months of 28 days.
#pragma once
#include <stdint.h>

namespace rt3 {

// 8 bytes, passed by value on the stack and returned in EDX:EAX.
struct GameDate {
    uint16_t year;
    uint8_t  month;   // 1..12
    uint8_t  week;    // day / 7 + 1
    uint8_t  day;     // 1..28
    uint8_t  hour;    // 0..23
    uint8_t  minute;  // 0..59
    uint8_t  quarter; // hour / 6 + 1 (night, morning, afternoon, evening)
};
static_assert(sizeof(GameDate) == 8, "GameDate layout");

const int32_t kMinutesPerDay   = 24 * 60;
const int32_t kMinutesPerMonth = 28 * kMinutesPerDay;
const int32_t kMinutesPerYear  = 12 * kMinutesPerMonth;

int32_t  __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute); // 0x51d390
int32_t  __fastcall DateToMinutes(const GameDate* d);                                                       // 0x51d3c0
uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);       // 0x51d3f0
uint64_t __fastcall DateFromMinutes(int32_t minutes);                                                       // 0x51d460
uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d);                                            // 0x51d550
uint64_t __fastcall DateAddDays(int32_t days, GameDate d);                                                  // 0x51d5a0
int32_t  __stdcall  DateDiffMinutes(GameDate a, GameDate b);                                                // 0x51d5f0

} // namespace rt3
