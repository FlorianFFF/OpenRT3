// Distance helpers and the millisecond clock (0x51d870..0x51dc00).
#include "hook/hook.h"
#include <windows.h>
#include <math.h>
#include <stdint.h>

namespace rt3 {

static DWORD& g_clockStart = hook::Global<DWORD>(0x6d3d14);
// Through the exe's import slot, like the original (the harness fakes it).
static inline DWORD Ticks() { return ((DWORD(WINAPI*)())*(void**)0x5c8068)(); }

// 0x51d870. Starts the game's millisecond clock (once).
void StartClock() {
    if (g_clockStart == 0) g_clockStart = Ticks();
}
RT3_REPLACE(0x51d870, StartClock);

// 0x51d890. Milliseconds since StartClock.
RT3_NOINLINE int32_t ClockMs() { return (int32_t)(Ticks() - g_clockStart); }
RT3_REPLACE(0x51d890, ClockMs);

// 0x51db40
double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1, dz = (double)z2 - z1;
    return sqrt((dz * dz + dy * dy) + dx * dx); // x87 order of the original
}
RT3_REPLACE(0x51db40, Distance3D);

// 0x51db80
double __stdcall Distance2D(float x1, float y1, float x2, float y2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1;
    return sqrt(dx * dx + dy * dy);
}
RT3_REPLACE(0x51db80, Distance2D);

// 0x51dbb0
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2) {
    double dx = (double)x2 - x1, dy = (double)y2 - y1;
    return dx * dx + dy * dy;
}
RT3_REPLACE(0x51dbb0, DistanceSq2D);

// 0x51dbe0
int32_t __fastcall DistanceSqInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    return (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1);
}
RT3_REPLACE(0x51dbe0, DistanceSqInt);

// 0x51dc00. Integer distance; small offsets come from a precomputed
// 1048x1048 sqrt table (0x6d9098, u16), larger ones are computed and truncated.
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    int32_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
    int32_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
    if (dx < 0x418 && dy < 0x418)
        return (int32_t)(((uint32_t)dy & 0xffff0000u) | ((const uint16_t*)0x6d9098)[dx * 0x418 + dy]);
    return (int32_t)sqrt((double)(dy * dy + dx * dx));
}
RT3_REPLACE(0x51dc00, DistanceInt);

} // namespace rt3

namespace rt3 {

// 0x51de10. Per-channel blend of two 0xAARRGGBB colours: a*t + b*(1-t), truncated.
uint32_t __fastcall BlendColor(uint32_t a, uint32_t b, float t) {
    double u = 1.0 - t;
    uint32_t out;
    static const int kShift[4] = { 16, 24, 8, 0 }; // order of the original
    uint32_t ch[4];
    for (int i = 0; i < 4; ++i) {
        double x = (double)(int32_t)((a >> kShift[i]) & 0xff) * t + (double)(int32_t)((b >> kShift[i]) & 0xff) * u;
        ch[i] = (uint32_t)(int32_t)x;
    }
    out = ((ch[0] | ch[1] << 8) << 8 | ch[2]) << 8 | ch[3]; // = A<<24 | R<<16 | G<<8 | B
    return out;
}
RT3_REPLACE(0x51de10, BlendColor);

} // namespace rt3
