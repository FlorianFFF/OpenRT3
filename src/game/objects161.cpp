// Region cell refresh / cargo overlay range / building updates by cargo / random terrain level changes in blocks 0x44..0x45 (batch 165). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall City_Refresh(uint8_t* c);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Obj_Bit(void* o, int, int32_t i);
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo);
int32_t __fastcall Obj_CheckActive(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline float Bits(uint32_t u) { return *(float*)&u; }
static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }

// 0x44ffd0. Refreshes every region cell's cargo and flags.
void __fastcall Map_RefreshCells(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            City_Refresh(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44ffd0, Map_RefreshCells);

// 0x450160. For the cargo overlay (mode +0x2175 == 3, cargo +0x2179 % 53) works out
// the colour range (+0x2195 base, +0x2199 span) of the region quotas over land cells
// (200 without a cargo): from min to max, lowered towards the lowest supplying cell
// when that is far below the average, widened to at least a minimum span.
void __fastcall Map_UpdateQuotaRange(uint8_t* m) {
    int32_t cargo = F<int32_t>(m, 0x2179) % 0x35;
    if (F<int32_t>(m, 0x2175) != 3) return;
    float lo = 1e10f, hi = Bits(0xd01502f9), supply = 1e10f, sum = 0.0f;
    int32_t count = 0, found = 0, any = 0;
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x) {
            uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7;
            uint8_t f = cell[0xe6];
            if ((f & 1) && !(f & 4)) continue;
            float val = cargo ? (float)City_CargoQuota(cell, 0, cargo) : 200.0f;
            if (val > hi) hi = val;
            if (val < lo) lo = val;
            if (val < supply && Obj_Bit(cell, 0, cargo)) {
                supply = val;
                found = 1;
            }
            sum = (float)((double)val + sum);
            ++count;
            any = 1;
        }
    if (!any) lo = hi = 10000.0f;
    float avg = (float)((double)sum / (float)(count < 1 ? 1 : count));
    float t;
    if (avg < Fl(0x5c8db0)) t = 15000.0f;
    else if (avg < Fl(0x5caac8)) t = 20000.0f;
    else if (avg < Fl(0x5caac4)) t = 25000.0f;
    else if (avg < Fl(0x5caac0)) t = 30000.0f;
    else t = avg < Fl(0x5c88bc) ? 35000.0f : 40000.0f;
    if (found) {
        float s = (float)((double)supply - t);
        if (lo < s && (double)avg - (double)t * D(0x5c8750) > lo) lo = s;
    }
    if ((float)((double)hi - lo) < Fl(0x5c99c8)) {
        float mid = (float)(((double)hi + lo) * D(0x5c8628));
        hi = (float)(mid + D(0x5caab8));
        lo = (float)(mid - D(0x5caab8));
        if (lo < Fl(0x5c8598)) {
            hi = (float)(Fabs(lo) + hi);
            lo = 0.0f;
        }
    }
    F<float>(m, 0x2195) = lo;
    float span = (float)((double)hi - lo);
    F<float>(m, 0x2199) = span;
    if (span < Fl(0x5c85b4)) F<float>(m, 0x2199) = 1.0f;
}
RT3_REPLACE(0x450160, Map_UpdateQuotaRange);

// 0x452e10. Updates every building: those whose industry handles `cargo` (all with
// -1) or that are active take the vtable +0xc update, the rest +0x10.
void __stdcall Buildings_UpdateFor(int32_t cargo) {
    typedef void(__thiscall * Fn)(void*);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b26c)); ++i) {
        Collection* c = hook::Global<Collection*>(0x62b26c);
        uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        void** vt = *(void***)b;
        bool active = cargo == -1;
        if (!active && cargo) {
            uint8_t* ind = ((uint8_t*(__thiscall*)(void*))vt[0x80 / 4])(b);
            active = Industry_HandlesCargo(ind, 0, cargo) != 0;
        }
        if (!active) active = Obj_CheckActive(b) != 0;
        ((Fn)vt[(active ? 0xc : 0x10) / 4])(b);
    }
}
RT3_REPLACE(0x452e10, Buildings_UpdateFor);

// 0x454010. Raises (amount > 0) or lowers the terrain level (cell byte % 7, 0..6)
// of the tiles inside the circle by a random rounding of |amount|, keeping the
// cell's other part (byte / 7); retries up to 30 times until some tile changed.
void __fastcall Terrain_AdjustLevel(uint8_t* m, int, float x, float y, float r, float amount) {
    int32_t x0, x1, y0, y1;
    float t = (float)((double)x - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)y - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x2155) - 1);
    float u = (float)((double)x + r + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2159) - 1);
    u = (float)((double)y + r + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    int32_t done = 0;
    for (int32_t tries = 30; tries; --tries) {
        if (done) continue;
        for (int32_t ty = y0; ty <= y1; ++ty)
            for (int32_t tx = x0; tx <= x1; ++tx) {
                if (!(r == Fl(0x5c8598))) {
                    float py = (float)((double)ty + D(0x5c8628)), px = (float)((double)tx + D(0x5c8628));
                    float d = (float)(((double)r - Distance2D(px, py, x, y)) / r);
                    if (d <= Fl(0x5c8598)) continue;
                }
                bool up = amount > Fl(0x5c8598);
                int32_t rnd = SimRand(0, 1000);
                double q = (Fabs(amount) * D(0x5c87f8) + (double)rnd) / D(0x5c87f8);
                int32_t v;
                __asm {
                    fld q
                    mov eax, 0x5a10d0
                    call eax
                    mov v, eax
                }
                if (v <= 0) continue;
                if (!up) v = -v;
                int32_t lvl = (uint8_t)Terrain_CellByte(m, 0, tx, ty) % 7 + v;
                if (lvl < 0) lvl = 0;
                else if (lvl > 6) lvl = 6;
                uint8_t b = (uint8_t)Terrain_CellByte(m, 0, tx, ty);
                Terrain_SetCellByte(m, 0, tx, ty, (uint8_t)(b / 7 * 7 + lvl));
                done = 1;
            }
    }
}
RT3_REPLACE(0x454010, Terrain_AdjustLevel);

} // namespace rt3
