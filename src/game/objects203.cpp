// Larger game functions, part 10 (batch 210). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
static inline double ExeSqrt(double v) { return ((double(__cdecl*)(double))0x5a1634)(v); }
static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

void __fastcall Region_UpdateCargoEconomy(uint8_t* r);
void __fastcall City_UpdateCargoBases(uint8_t* o);
void __fastcall City_SpreadCargo(uint8_t* c);
void __fastcall Station_CapStocks(void* o);

// 0x42d580. A region's monthly economy: prices, supply and demand, cargo bases,
// spreading, then the stock caps.
void __fastcall Region_MonthlyEconomy(uint8_t* r) {
    Region_UpdateCargoEconomy(r);
    City_UpdateCargoBases(r);
    City_SpreadCargo(r);
    Station_CapStocks(r);
}
RT3_REPLACE(0x42d580, Region_MonthlyEconomy);

double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
float __fastcall Scene_GetLightValue0(uint8_t* s, int, int32_t i);
void __fastcall Scene_SetPair0(uint8_t* s, int, float a, float b);
void __fastcall Scene_SetPair1(uint8_t* s, int, float a, float b);
int32_t __fastcall Fx_Intensity(uint8_t* o);
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night);

// 0x43a940. Camera-height lighting: the camera's height above the ground (`h`, or
// measured when negative) and the daylight level of the current time (from the
// scene's light curve, 0..1) give a haze factor; when it changed by more than 0.01
// (cached in 0x5ee4d8) the scene's two light pairs and the time-of-day lighting
// are refreshed.
void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h) {
    if (h < Fl(0x5c8598))
        h = (float)((double)F<float>(cam, 0x20) -
                    Terrain_HeightAt(F<uint8_t*>(cam, 0x6e), 0, F<float>(cam, 0x1c), F<float>(cam, 0x24)));
    int32_t hh, mm, ss;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, &hh, &mm, &ss);
    float t = (float)((double)mm / D(0x5c88a8) + (double)ss / D(0x5c9f30) + (double)hh);
    int32_t idx = Ftol((double)t / D(0x5c9ab8) * D(0x5ca0e0));
    if (idx < 0) idx = 0;
    else if (idx > 0x8f) idx = 0x8f;
    auto light = [](int32_t i) { return Scene_GetLightValue0((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, i); };
    float now = light(idx);
    float lo = light(0);
    float span = light(0x48);
    span = (float)((double)span - lo);
    double den = span < D(0x5c85b8) ? 0.01 : (double)span;
    float r = (float)(((double)now - lo) / den);
    if (Fl(0x5c8598) > r) r = 0.0f;
    r = (float)(r > D(0x5c8770) ? 1.0 : (double)r);
    float dusk = (float)(D(0x5c8770) - r);
    dusk = (float)(((double)dusk - D(0x5c88e8)) / D(0x5c8880));
    if (Fl(0x5c8598) > dusk) dusk = 0.0f;
    float hc = Fl(0x5c85b4) > h ? 1.0f : h;
    float sq = (float)ExeSqrt(hc);
    float q = (float)(((double)sq - Fl(0x5c8550)) / ExeSqrt(6500.0));
    if (q < D(0x5c85b8)) q = 0.0f;
    else if (q > Fl(0x5c85b4)) q = 1.0f;
    float v = (float)((double)q * dusk);
    float& cache = hook::Global<float>(0x5ee4d8);
    if (!(ExeFabs((double)cache - v) > D(0x5c85b8))) return;
    cache = v;
    float w = (float)((double)v * D(0x5ca0d8) + D(0x5c8770));
    Scene_SetPair0((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, w, 1.0f);
    Scene_SetPair1((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, w, 1.0f);
    uint8_t* s = (uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024));
    int32_t night = Fx_Intensity(s);
    Renderer_SetTimeOfDay(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0xd), night);
}
RT3_REPLACE(0x43a940, Camera_UpdateHaze);

void __fastcall SetSeed(int32_t seed);
int32_t __fastcall Obj_MapInsertNew(void* o, int, uint16_t key, int32_t value);
void __fastcall Obj_Set8(void* o, int, int32_t v);

// 0x473280. Resets the multiplayer session state (0x6ce808 / 0x6ce290 blocks and
// flags, seed 7, timeout 0x5f2b4c = 1e6) and registers the game's message handlers
// on the lobby object (0x6cd8d8): the default 0x46f4a0 and 129 id -> handler pairs.
void MpSession_Reset() {
    memset((void*)0x6ce808, 0, 0x60 * 4);
    hook::Global<int32_t>(0x6ce9a0) = 0;
    hook::Global<uint32_t>(0x5f2b4c) = 0x49741cf0;
    static const uint32_t zero1[] = { 0x6ce998, 0x6ce99c, 0x6ce98c, 0x6ce990, 0x6ce9a8, 0x6ce9ac, 0x6ce9b0 };
    for (uint32_t a : zero1) hook::Global<int32_t>(a) = 0;
    hook::Global<int32_t>(0x5f2b6c) = 7;
    SetSeed(7);
    void* lobby = hook::Global<void*>(0x6cd8d8);
    memset((void*)0x6ce290, 0, 0xe6 * 4);
    static const uint32_t zero2[] = { 0x6ce800, 0x6ce628, 0x6ce9b4, 0x6ce9b8, 0x6ce9bc };
    for (uint32_t a : zero2) hook::Global<int32_t>(a) = 0;
    hook::Global<uint8_t>(0x6ce630) = 0;
    hook::Global<int32_t>(0x6ce9c0) = 0;
    hook::Global<int32_t>(0x6ce9c4) = 0;
    hook::Global<int32_t>(0x6ce9c8) = 0;
    Obj_Set8(lobby, 0, 0x46f4a0);
    static const uint32_t handlers[][2] = {
{ 0x1, 0x470210 },
{ 0x2, 0x470290 },
{ 0x3, 0x46c3c0 },
{ 0x4, 0x46c420 },
{ 0x5, 0x46c760 },
{ 0x6, 0x46c770 },
{ 0x7, 0x46c7c0 },
{ 0x8, 0x46c810 },
{ 0x9, 0x46c870 },
{ 0x68, 0x46c840 },
{ 0xa, 0x4703f0 },
{ 0xb, 0x4704d0 },
{ 0xc, 0x470530 },
{ 0xd, 0x46c8c0 },
{ 0xe, 0x46c990 },
{ 0xf, 0x470590 },
{ 0x10, 0x4705f0 },
{ 0x11, 0x46c390 },
{ 0x12, 0x470630 },
{ 0x13, 0x4706b0 },
{ 0x14, 0x4708c0 },
{ 0x15, 0x470950 },
{ 0x16, 0x470b20 },
{ 0x17, 0x470bb0 },
{ 0x18, 0x46c9d0 },
{ 0x19, 0x470da0 },
{ 0x1a, 0x470e30 },
{ 0x1b, 0x46c9e0 },
{ 0x1c, 0x470ed0 },
{ 0x1d, 0x471070 },
{ 0x1e, 0x46c9f0 },
{ 0x1f, 0x4710d0 },
{ 0x20, 0x46ca10 },
{ 0x21, 0x471130 },
{ 0x22, 0x46ca30 },
{ 0x23, 0x471190 },
{ 0x24, 0x46ca50 },
{ 0x25, 0x4711f0 },
{ 0x26, 0x46ca70 },
{ 0x27, 0x471250 },
{ 0x28, 0x46ca90 },
{ 0x29, 0x4712b0 },
{ 0x2a, 0x46cab0 },
{ 0x2b, 0x471310 },
{ 0x2c, 0x46cad0 },
{ 0x2d, 0x471370 },
{ 0x2e, 0x46caf0 },
{ 0x2f, 0x4713d0 },
{ 0x30, 0x46cb10 },
{ 0x31, 0x471430 },
{ 0x32, 0x46cb30 },
{ 0x33, 0x471490 },
{ 0x34, 0x46cb50 },
{ 0x35, 0x4714f0 },
{ 0x36, 0x46cb70 },
{ 0x37, 0x471550 },
{ 0x38, 0x46cb90 },
{ 0x39, 0x4715b0 },
{ 0x3a, 0x46cbb0 },
{ 0x3b, 0x471610 },
{ 0x3c, 0x471680 },
{ 0x3d, 0x4716c0 },
{ 0x3e, 0x471730 },
{ 0x3f, 0x471780 },
{ 0x40, 0x4717f0 },
{ 0x41, 0x471830 },
{ 0x42, 0x4718a0 },
{ 0x43, 0x471950 },
{ 0x44, 0x46cbd0 },
{ 0x46, 0x46cbf0 },
{ 0x47, 0x471c90 },
{ 0x48, 0x471a30 },
{ 0x49, 0x471b90 },
{ 0x4a, 0x46cc60 },
{ 0x4b, 0x471fd0 },
{ 0x4c, 0x471dc0 },
{ 0x4d, 0x471ed0 },
{ 0x4e, 0x46ccd0 },
{ 0x4f, 0x472080 },
{ 0x50, 0x4720b0 },
{ 0x51, 0x4720d0 },
{ 0x52, 0x46cd10 },
{ 0x53, 0x46cd30 },
{ 0x54, 0x46ce10 },
{ 0x55, 0x4721a0 },
{ 0x56, 0x4721c0 },
{ 0x57, 0x472200 },
{ 0x58, 0x472230 },
{ 0x59, 0x472290 },
{ 0x5a, 0x46ce90 },
{ 0x5b, 0x472360 },
{ 0x5c, 0x4723c0 },
{ 0x5d, 0x472450 },
{ 0x5e, 0x4724e0 },
{ 0x5f, 0x4725e0 },
{ 0x60, 0x472640 },
{ 0x61, 0x472700 },
{ 0x62, 0x4727e0 },
{ 0x63, 0x472870 },
{ 0x64, 0x472940 },
{ 0x65, 0x46cf10 },
{ 0x66, 0x46cf40 },
{ 0x67, 0x470d60 },
{ 0x69, 0x46cf70 },
{ 0x6a, 0x470fa0 },
{ 0x6b, 0x472db0 },
{ 0x6c, 0x472dc0 },
{ 0x6d, 0x472dd0 },
{ 0x6e, 0x472ea0 },
{ 0x6f, 0x46cce0 },
{ 0x70, 0x46cfa0 },
{ 0x71, 0x472aa0 },
{ 0x72, 0x472b00 },
{ 0x73, 0x4729f0 },
{ 0x74, 0x472a50 },
{ 0x75, 0x472f10 },
{ 0x76, 0x472f70 },
{ 0x77, 0x472fb0 },
{ 0x78, 0x473010 },
{ 0x79, 0x4730c0 },
{ 0x7a, 0x473120 },
{ 0x7b, 0x473180 },
{ 0x7c, 0x4731e0 },
{ 0x7d, 0x473220 },
{ 0x7e, 0x46cfb0 },
{ 0x7f, 0x46cfe0 },
{ 0x80, 0x46d090 },
{ 0x81, 0x46d1d0 },
{ 0x83, 0x46d230 }
    };
    for (auto& h : handlers) Obj_MapInsertNew(lobby, 0, (uint16_t)h[0], (int32_t)h[1]);
}
RT3_REPLACE(0x473280, MpSession_Reset);

int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// 0x478dc0. Fits the overview map (+0x33 flags 0x2000000 whole map / 0x30000 own
// area) to the cells marked 2 in the map grid (+0x2131): bounds +0xb3..+0xbf
// (widened by a margin, more when one of the player's stations is near the
// north-west corner of the area), centre +0xdf / +0xe3, fit zoom +0xd3 for the
// window (+0x11f cells), minimum zoom +0xd7 (snapped to 1 / 0.5 / 0.25) and the
// current zoom +0xdb kept between them.
void __fastcall Overview_FitArea(uint8_t* o) {
    uint32_t flags = F<uint32_t>(o, 0x33);
    if (!(flags & 0x2030000)) return;
    auto map = []() { return hook::Global<uint8_t*>(0x62c120); };
    int32_t& x0 = F<int32_t>(o, 0xb3);
    int32_t& y0 = F<int32_t>(o, 0xb7);
    int32_t& x1 = F<int32_t>(o, 0xbb);
    int32_t& y1 = F<int32_t>(o, 0xbf);
    if (flags & 0x2000000) {
        x0 = 0;
        y0 = 0;
        x1 = F<int32_t>(map(), 0x2155) - 1;
        y1 = F<int32_t>(map(), 0x2159) - 1;
        F<int32_t>(o, 0xdf) = Ftol((double)F<int32_t>(map(), 0x2155) * D(0x5c8628));
        F<int32_t>(o, 0xe3) = Ftol((double)F<int32_t>(map(), 0x2159) * D(0x5c8628));
    } else {
        x0 = F<int32_t>(map(), 0x2155) - 1;
        y0 = F<int32_t>(map(), 0x2159) - 1;
        x1 = 0;
        y1 = 0;
        for (int32_t y = F<int32_t>(map(), 0x2159) - 1; y >= 0; --y)
            for (int32_t x = F<int32_t>(map(), 0x2155) - 1; x >= 0; --x) {
                if (!(F<uint8_t*>(map(), 0x2131)[F<int32_t>(map(), 0x2155) * y + x] & 2)) continue;
                if (x < x0) x0 = x;
                if (y < y0) y0 = y;
                if (x > x1) x1 = x;
                if (y > y1) y1 = y;
            }
        int32_t near = 0;
        Collection* stations = hook::Global<Collection*>(0x6cec20);
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
            stations = hook::Global<Collection*>(0x6cec20);
            uint8_t* st = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
            int32_t me = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
            if (Obj_BuildingValue(st) != me) continue;
            float d = (float)Distance2D((float)Obj_BuildingCellX(st), (float)Obj_BuildingCellY(st), (float)x0, (float)y0);
            if (d < Fl(0x5c8898)) near = 1;
            else {
                d = (float)Distance2D((float)Obj_BuildingCellX(st), (float)Obj_BuildingCellY(st), (float)x0, (float)y1);
                if (d < Fl(0x5c8898)) near = 1;
            }
        }
        float grow = 1.0f;
        int32_t extra = 0;
        if (near) {
            float dy = (float)(y1 - y0);
            float d = (float)Distance2D((float)x0, (float)y0, (float)x1, (float)y1);
            if (d > Fl(0x5c8c28)) F<uint32_t>(&grow, 0) = 0x3fcccccd;
            else if (d > Fl(0x5c8990)) F<uint32_t>(&grow, 0) = 0x3fb33333;
            else if (d > Fl(0x5c8d4c)) F<uint32_t>(&grow, 0) = 0x3f99999a;
            if (dy > Fl(0x5c9aa8)) extra = Ftol((double)grow * Fl(0x5c8568));
        }
        float m = (float)((double)grow * Fl(0x5c873c));
        float v = (float)((double)(float)x0 - m - (float)extra);
        if (Fl(0x5c8598) > v) v = 0.0f;
        int32_t nx0 = Ftol(v);
        x0 = nx0;
        v = (float)((double)(float)y0 - m);
        if (Fl(0x5c8598) > v) v = 0.0f;
        int32_t ny0 = Ftol(v);
        y0 = ny0;
        float lim = (float)(F<int32_t>(map(), 0x2155) - 1);
        v = (float)((double)(float)x1 + m);
        int32_t nx1 = Ftol(lim < v ? lim : v);
        x1 = nx1;
        lim = (float)(F<int32_t>(map(), 0x2159) - 1);
        v = (float)((double)(float)y1 + m);
        int32_t ny1 = Ftol(lim < v ? lim : v);
        y1 = ny1;
        F<int32_t>(o, 0xdf) = Ftol((double)(nx1 + nx0) * D(0x5c8628));
        F<int32_t>(o, 0xe3) = Ftol((double)(ny1 + ny0) * D(0x5c8628));
    }
    int32_t mx = F<int32_t>(map(), 0x2159);
    if (F<int32_t>(map(), 0x2155) > mx) mx = F<int32_t>(map(), 0x2155);
    int32_t w = x1 - x0 + 1, h = y1 - y0 + 1, win = F<int32_t>(o, 0x11f);
    float fm = (float)mx;
    float z1 = (float)((double)fm / (float)(win > w ? win : w));
    float z2 = (float)((double)fm / (float)(win > h ? win : h));
    float fit = z1 < z2 ? z1 : z2;
    F<float>(o, 0xd3) = fit;
    float fw = (float)win;
    float z3 = (float)((double)fw / (float)(w < 1 ? 1 : w));
    float z4 = (float)((double)fw / (float)(h < 1 ? 1 : h));
    float zz = z3 > z4 ? z3 : z4;
    F<float>(o, 0xd7) = zz;
    zz = (float)(zz > D(0x5c8758) ? (double)zz : 0.25);
    F<float>(o, 0xd7) = zz;
    double cap = D(0x5ca9e8) / (double)mx;
    float mn = (float)(zz < cap ? (double)zz : cap);
    F<float>(o, 0xd7) = mn;
    if (mn > D(0x5c8888)) F<uint32_t>(o, 0xd7) = 0x3f800000;
    else if (mn > D(0x5c8c18) && mn < D(0x5c96e0)) F<uint32_t>(o, 0xd7) = 0x3f000000;
    else if (mn > D(0x5c8c20) && mn < D(0x5c88c8)) F<uint32_t>(o, 0xd7) = 0x3e800000;
    float cur = F<float>(o, 0xdb) < fit ? F<float>(o, 0xdb) : fit;
    F<float>(o, 0xdb) = cur;
    if (!(cur > F<float>(o, 0xd7))) F<float>(o, 0xdb) = F<float>(o, 0xd7);
}
RT3_REPLACE(0x478dc0, Overview_FitArea);

uint64_t __fastcall DateFromMinutes(int32_t m);
float __stdcall Daylight_Factor(float hour);
void __fastcall Clock_SetD(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
void __fastcall Clock_Set11(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
void __fastcall Economy_UpdateEraFactors(uint8_t* o);
void PushSeed();
void PopSeed();
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
static inline double ExeFmod(double a, double b) { return ((double(__cdecl*)(double, double))0x5a1390)(a, b); }

// Splits fractional hours into whole hours h and minutes / seconds (fractions x 60).
static void SplitHours(float t, int32_t* h, float* m, float* s) {
    *h = Ftol(t);
    float hf = (float)*h;
    *m = (float)(((double)t - hf) * Fl(0x5c8990));
    float mf = (float)Ftol(*m);
    *s = (float)(((double)*m - mf) * Fl(0x5c8990));
}

// 0x409e80. Sets the world clock to `minutes` (+0x15; date +0xd/+0x11): derives the
// local hour of day (time-zone offset +0xbfa, +1 h unless +5 & 1), sets the scene
// clocks to it and to its daylight-curve time (or the fixed time of the game
// options +0x128 / +0x237), refreshes the camera haze when the sun moved, the era
// factors on a new year and the engine's clock (+0x2c = 2 x minutes).
void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes) {
    F<int32_t>(w, 0x15) = minutes;
    uint16_t oldYear = F<uint16_t>(w, 0xd);
    uint64_t date = DateFromMinutes(minutes);
    F<uint32_t>(w, 0xd) = (uint32_t)date;
    F<uint32_t>(w, 0x11) = (uint32_t)(date >> 32);
    float h = (float)((double)F<int32_t>(w, 0x15) / D(0x5c89a8));
    h = (float)((double)h - Fl(0x5c89a4));
    while (h < Fl(0x5c8598)) h = (float)((double)h + Fl(0x5c89a0));
    float t = (float)((double)h + Fl(0x5c89a0));
    t = (float)((double)t + F<float>(w, 0xbfa));
    t = (float)ExeFmod(t, 24.0);
    t = (float)((double)t + D(0x5c8998));
    if (!(w[5] & 1)) t = (float)((double)t + Fl(0x5c8788));
    t = (float)ExeFmod(t, 24.0);
    int32_t hours;
    float mins, secs;
    hours = Ftol(t);
    float hf = (float)hours;
    mins = (float)(((double)t - hf) * Fl(0x5c8990));
    int32_t minsInt = Ftol(mins);
    secs = (float)(((double)mins - (float)minsInt) * Fl(0x5c8990));
    float sun = Daylight_Factor(t);
    int32_t sh;
    float sm, ss;
    SplitHours(sun, &sh, &sm, &ss);
    uint8_t* game = hook::Global<uint8_t*>(0x6cec74);
    float clockH = sun;
    if (F<int32_t>(game, 0x124)) {
        clockH = (float)F<int32_t>(game, 0x128);
        ss = 0.0f;
        sm = 1.0f;
    } else if (F<int32_t>(game, 0x233) && hook::Global<uint8_t*>(0x6cec7c)[0x82]) {
        clockH = (float)F<int32_t>(game, 0x237);
        ss = 0.0f;
        sm = 1.0f;
    }
    if (void* eng = hook::Global<void*>(0x6d4024)) {
        int32_t s3 = Ftol(ss), m3 = Ftol(sm), h3 = Ftol(clockH);
        Clock_SetD((uint8_t*)Engine_Renderer(eng), 0, h3, m3, s3);
        if (void* e2 = hook::Global<void*>(0x6d4024)) Clock_Set11((uint8_t*)Engine_Renderer(e2), 0, hours, minsInt, Ftol(secs));
    }
    float& last = hook::Global<float>(0x5ed080);
    if (ExeFabs((double)last - clockH) >= D(0x5c8988)) {
        last = clockH;
        if (uint8_t* cam = hook::Global<uint8_t*>(0x62be68)) Camera_UpdateHaze(cam, 0, -999.0f);
    }
    if (oldYear != F<uint16_t>(w, 0xd)) Economy_UpdateEraFactors(w);
    if (uint8_t* eng = hook::Global<uint8_t*>(0x6d4024)) F<int32_t>(eng, 0x2c) = F<int32_t>(w, 0x15) * 2;
}
RT3_REPLACE(0x409e80, World_SetTime);

void __fastcall Station_CapStocks(void* o);

// 0x450030. Monthly map economy (not in sandbox / scenario-war modes): every 4th
// block record (+0x2129, 0x1d7 bytes; from the day & 3) whose phase (a fixed
// table 0x62c150 of 0..13 drawn once with seed 0x61) matches the day % 14 runs its
// economy; then all blocks cap their stocks.
void __fastcall Map_MonthlyEconomy(uint8_t* m) {
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    if (F<int32_t>(world, 0x4af7) || F<int32_t>(world, 0x4aef)) return;
    if (!hook::Global<int32_t>(0x62c254)) {
        PushSeed();
        SetSeed(0x61);
        for (int32_t i = 0; i < 0x100; ++i) hook::Global<uint8_t>(0x62c150 + i) = (uint8_t)SimRand(0, 0xd);
        PopSeed();
        world = hook::Global<uint8_t*>(0x6cec78);
        hook::Global<int32_t>(0x62c254) = 1;
    }
    int32_t phase = world[0x11] % 14;
    int32_t n = F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145);
    for (int32_t i = world[0x14] & 3; i < n; i += 4)
        if (hook::Global<uint8_t>(0x62c150 + (i & 0xff)) == phase) Region_MonthlyEconomy(F<uint8_t*>(m, 0x2129) + i * 0x1d7);
    for (int32_t i = 0; i < n; ++i) Station_CapStocks(F<uint8_t*>(m, 0x2129) + i * 0x1d7);
}
RT3_REPLACE(0x450030, Map_MonthlyEconomy);

float __fastcall Obj_ScaledExtent(uint8_t* o);

// Clamps a [lo, hi] cell span into [0, size - 1], sliding it (and splitting any
// leftover overshoot) as the original does.
static void ClampSpan(int32_t& lo, int32_t& hi, int32_t size) {
    if (lo < 0) {
        hi = (lo < 0 ? -lo : lo) + hi;
        lo = 0;
    }
    if (hi > size - 1) {
        lo += size - hi - 1;
        hi = size - 1;
    }
    if (lo < 0) {
        int32_t a = (lo < 0 ? -lo : lo) / 2;
        hi += a;
        lo = a + lo;
    }
}

// 0x479510. Overview viewport: the visible cell rectangle (+0xc3..+0xcf) of the
// current zoom around the centre (+0xdf / +0xe3), kept on the map; its texture
// coordinates (+0xef..+0xfb within the map image span +0xff..+0x10b, kept off the
// edges); and the scroll limits for the centre (+0xe7 / +0xeb). Without an
// overview area the full image is shown.
void __fastcall Overview_UpdateView(uint8_t* o) {
    if (!(F<uint32_t>(o, 0x33) & 0x2030000)) {
        F<uint32_t>(o, 0xef) = 0;
        F<uint32_t>(o, 0xf3) = 0x3f800000;
        F<uint32_t>(o, 0xf7) = 0;
        F<uint32_t>(o, 0xfb) = 0x3f800000;
        return;
    }
    int32_t x0 = F<int32_t>(o, 0xb3), y0 = F<int32_t>(o, 0xb7), x1 = F<int32_t>(o, 0xbb), y1 = F<int32_t>(o, 0xbf);
    F<int32_t>(o, 0xcf) = y1;
    F<int32_t>(o, 0xcb) = x1;
    F<int32_t>(o, 0xc3) = x0;
    F<int32_t>(o, 0xc7) = y0;
    int32_t dw = x1 - x0, dh = y1 - y0;
    int32_t sz = Ftol(Obj_ScaledExtent(o));
    if (dw < sz) {
        double half = (double)(sz - dw) * D(0x5c8628);
        F<int32_t>(o, 0xc3) = Ftol((double)x0 - half);
        F<int32_t>(o, 0xcb) = Ftol((double)x1 + half + D(0x5c8880));
    }
    if (dh < sz) {
        double half = (double)(sz - dh) * D(0x5c8628);
        F<int32_t>(o, 0xc7) = Ftol((double)y0 - half);
        F<int32_t>(o, 0xcf) = Ftol((double)y1 + half + D(0x5c8880));
    }
    double cx = (double)F<int32_t>(o, 0xdf), half = (double)sz * D(0x5c8628);
    int32_t vx0 = Ftol(cx - half), vx1 = Ftol(half + cx);
    double cy = (double)F<int32_t>(o, 0xe3);
    int32_t vy0 = Ftol(cy - half), vy1 = Ftol(half + cy);
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    ClampSpan(vx0, vx1, F<int32_t>(m, 0x2155));
    ClampSpan(vy0, vy1, F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159));
    F<int32_t>(o, 0xc3) = vx0;
    F<int32_t>(o, 0xcb) = vx1;
    F<int32_t>(o, 0xc7) = vy0;
    F<int32_t>(o, 0xcf) = vy1;
    m = hook::Global<uint8_t*>(0x62c120);
    float sx = (float)(((double)F<float>(o, 0x103) - F<float>(o, 0xff)) / (float)F<int32_t>(m, 0x2155));
    float sy = (float)(((double)F<float>(o, 0x10b) - F<float>(o, 0x107)) / (float)F<int32_t>(m, 0x2159));
    float u0 = (float)((double)(float)vx0 * sx + F<float>(o, 0xff));
    F<float>(o, 0xef) = u0;
    float u1 = (float)((double)(float)vx1 * sx + F<float>(o, 0xff));
    F<float>(o, 0xf3) = u1;
    F<float>(o, 0xfb) = (float)((double)(float)(F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) - vy0 - 1) * sy + F<float>(o, 0x107));
    F<float>(o, 0xf7) = (float)((double)(float)(F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) - vy1 - 1) * sy + F<float>(o, 0x107));
    F<float>(o, 0xef) = (float)(D(0x5c8678) > u0 ? 0.001 : (double)u0);
    F<float>(o, 0xf3) = (float)(D(0x5caa68) < u1 ? 0.999 : (double)u1);
    F<float>(o, 0xf7) = (float)(F<float>(o, 0xf7) < D(0x5c8678) ? 0.001 : (double)F<float>(o, 0xf7));
    F<float>(o, 0xfb) = (float)(F<float>(o, 0xfb) > D(0x5caa68) ? 0.999 : (double)F<float>(o, 0xfb));
    double lo = (double)vx0 + half - D(0x5c8770);
    F<int32_t>(o, 0xe7) = Ftol(cx > lo ? cx : lo);
    double hi = (double)vx1 - half + D(0x5c8770);
    F<int32_t>(o, 0xe7) = Ftol(cx < hi ? cx : hi);
    double ey = (double)F<int32_t>(o, 0xeb);
    lo = (double)vy0 + half - D(0x5c8770);
    int32_t e = Ftol(ey > lo ? ey : lo);
    F<int32_t>(o, 0xeb) = e;
    double ed = (double)e;
    hi = (double)vy1 - half + D(0x5c8770);
    F<int32_t>(o, 0xeb) = Ftol(ed < hi ? ed : hi);
}
RT3_REPLACE(0x479510, Overview_UpdateView);

void __fastcall Obj_AddKey(void* o, int, uint16_t key, int32_t value);
void __fastcall Chain_RegisterBuildings(void* o, int, uint8_t* target, int32_t value);
float __fastcall Station_PathCost(void* o, int, int32_t x, int32_t y);
void __fastcall Obj_RemoveKey(void* o, int, uint16_t key);

// 0x47f320. (Re)builds the station's catchment: the blocks within its radius
// (kind +0x8c of its building type, or `kind`: 0 small 1.25, 1 medium 2, else large
// 3; centred on +0x4a8 / +0x4ac) get the station registered with a distance weight
// (and its buildings); with `remove` they are unregistered instead. The covered
// blocks are stored (+0x34, count +0x38) as {x, y, cost from the station} sorted by
// cost.
void __fastcall Station_BuildCatchment(uint8_t* st, int, int32_t remove, int32_t kindArg) {
    uint8_t* bt = F<uint32_t>(st, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(st, 4)) : nullptr;
    int32_t kind = ((uint8_t*)(*(uint8_t*(__fastcall**)(void*))(*(uint8_t**)bt + 0x80))(bt))[0x8c];
    if (kindArg != -1) kind = kindArg;
    float r, cx, cy;
    if (kind == 0) {
        r = 1.25f;
        cx = (float)((double)Ftol((double)F<float>(st, 0x4a8) + F<float>(st, 0x4a8)) * D(0x5c8628) + D(0x5c8758));
        cy = (float)((double)Ftol((double)F<float>(st, 0x4ac) + F<float>(st, 0x4ac)) * D(0x5c8628) + D(0x5c8758));
    } else {
        r = kind == 1 ? 2.0f : 3.0f;
        cx = (float)((double)Ftol(F<float>(st, 0x4a8)) + D(0x5c8628));
        cy = (float)((double)Ftol(F<float>(st, 0x4ac)) + D(0x5c8628));
    }
    int32_t ri = Ftol((double)r + Fl(0x5c85b4));
    if (F<void*>(st, 0x34)) Free(F<void*>(st, 0x34));
    F<void*>(st, 0x34) = nullptr;
    F<int32_t>(st, 0x38) = 0;
    int32_t n = 0;
    uint8_t* buf = nullptr;
    if (!remove) buf = (uint8_t*)Malloc(0x2ee0);
    if (F<void*>(st, 0x466)) Free(F<void*>(st, 0x466));
    F<void*>(st, 0x466) = nullptr;
    F<int32_t>(st, 0x462) = 0;
    float fr = (float)ri;
    int32_t x0 = Ftol((double)cx - fr), x1 = Ftol((double)fr + cx), y1 = Ftol((double)fr + cy);
    int32_t sx = Ftol(F<float>(st, 0x4a8)), sy = Ftol(F<float>(st, 0x4ac));
    int32_t y0 = Ftol((double)cy - fr);
    uint8_t* e = buf;
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (x < 0 || x >= F<int32_t>(m, 0x2145) || y < 0 || y >= F<int32_t>(m, 0x2149)) continue;
            float yf = (float)((double)y + D(0x5c8628)), xf = (float)((double)x + D(0x5c8628));
            float d = (float)Distance2D(cx, cy, xf, yf);
            if (!(d < r)) continue;
            if (kind == 0) d = 0.7f;
            float w = (float)(D(0x5c8770) - (double)d / r * D(0x5ca138));
            auto cell = [&]() {
                uint8_t* mm = hook::Global<uint8_t*>(0x62c120);
                return F<uint8_t*>(mm, 0x2129) + (F<int32_t>(mm, 0x2145) * y + x) * 0x1d7;
            };
            if (remove) {
                Obj_RemoveKey(cell(), 0, F<uint16_t>(st, 0));
                continue;
            }
            Obj_AddKey(cell(), 0, F<uint16_t>(st, 0), *(int32_t*)&w);
            Chain_RegisterBuildings(cell(), 0, st, *(int32_t*)&w);
            F<int32_t>(e, 0) = x;
            F<int32_t>(e, 4) = y;
            F<float>(e, 8) = Station_PathCost(cell(), 0, sx, sy);
            ++n;
            e += 0xc;
        }
    }
    if (remove) return;
    F<uint8_t*>(st, 0x34) = (uint8_t*)Malloc((n < 1 ? 1 : n) * 0xc);
    F<int32_t>(st, 0x38) = n;
    uint8_t used[1000] = {};
    for (int32_t k = 0; k < n; ++k) {
        int32_t best = -1;
        for (int32_t c = 0; c < n; ++c)
            if (!used[c] && (best == -1 || F<float>(buf, c * 0xc + 8) < F<float>(buf, best * 0xc + 8))) best = c;
        used[best] = 1;
        memcpy(F<uint8_t*>(st, 0x34) + k * 0xc, buf + best * 0xc, 0xc);
    }
    Free(buf);
}
RT3_REPLACE(0x47f320, Station_BuildCatchment);

} // namespace rt3
