// Heat-colour overlays for cities, buildings, track slopes and track wear in blocks 0x41..0x48 (batch 180). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __stdcall HeatColor(float t);
float __fastcall Building_ProfitScore(uint8_t* b);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Obj_SetRgbValue(uint8_t* o, int, uint32_t v);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline void** Vt(void* o) { return *(void***)o; }

// 0x413660. Colours the cities with a boom (+0x276): hot for the given boom value,
// cold for the others (vtable +0x58 takes r, g, b).
void __fastcall Cities_ColorBooms(Collection* c, int, int32_t value) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        int32_t v = F<int32_t>(e, 0x276);
        if (!v) continue;
        uint32_t col = HeatColor((float)(v == value ? 1 : 0));
        ((void(__thiscall*)(void*, uint32_t, uint32_t, uint32_t))Vt(e)[0x58 / 4])(e, col >> 16, col >> 8, col);
    }
}
RT3_REPLACE(0x413660, Cities_ColorBooms);

// 0x4136e0. Colours each idle (vtable +0x70) building of a producing industry
// (+0x3f) by its profit score (vtable +0x54).
void __fastcall Buildings_ColorProfit(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (((int32_t(__thiscall*)(void*))Vt(e)[0x70 / 4])(e)) continue;
        uint8_t* ind = ((uint8_t*(__thiscall*)(void*))Vt(e)[0x80 / 4])(e);
        if (!F<int32_t>(ind, 0x3f)) continue;
        float s = Building_ProfitScore(e);
        ((void(__thiscall*)(void*, uint32_t))Vt(e)[0x54 / 4])(e, HeatColor(s));
    }
}
RT3_REPLACE(0x4136e0, Buildings_ColorProfit);

// 0x48a7f0. Colours a track piece by its slope (height difference of its ends).
void __fastcall Track_ColorSlope(uint8_t* o) {
    uint32_t a[3], b[3], c[3], d[3];
    Effect_GetVectors(o, 0, a, b, c, d);
    float h = (float)((double)*(float*)&a[1] - *(float*)&c[1]);
    h = (float)(h * D(0x5cfd88));
    h = (float)((double(__cdecl*)(double))0x5a152e)(h);
    float t = (float)(D(0x5c8770) - (double)h * Fl(0x5c8c28) / D(0x5c8610));
    Obj_SetRgbValue(o, 0, HeatColor(t) | 0xff000000);
}
RT3_REPLACE(0x48a7f0, Track_ColorSlope);

// 0x48dfe0. Colours a track piece by its wear (+0x22a) relative to `full`
// (at least 1).
void __fastcall Track_ColorWear(uint8_t* o, int, float full) {
    if (Fl(0x5c85b4) > full) full = 1.0f;
    float r = (float)((double)F<float>(o, 0x22a) / full);
    float t = (float)(D(0x5c8770) - r);
    Obj_SetRgbValue(o, 0, HeatColor(t) | 0xff000000);
}
RT3_REPLACE(0x48dfe0, Track_ColorWear);

} // namespace rt3
