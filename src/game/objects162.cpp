// Terrain point painting / random map edge entry / engine type for the current year in blocks 0x45..0x46 (batch 166). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <stdlib.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

// 0x454320. Paints terrain points (trees etc.) of kind (kind % 28) * 7 + variant in the
// circle: each tile gets one with chance falloff x density, unless the tile is
// occupied (low 3 bits) and not `force`; kinds marked 3 keep at most 4 per tile.
// With `erase` removes up to 6 per tile instead. Retries up to 30 passes until a
// tile was touched, then redraws.
void __fastcall Terrain_PaintPoints(uint8_t* t, int, float x, float y, float r, int32_t erase, int32_t kind,
                                    int32_t variant, float density, int32_t force) {
    int32_t x0, x1, y0, y1;
    float v = (float)((double)x - r);
    if (Fl(0x5c8598) > v) v = 0.0f;
    x0 = (int32_t)v;
    v = (float)((double)y - r);
    if (Fl(0x5c8598) > v) v = 0.0f;
    y0 = (int32_t)v;
    float mx = (float)(F<int32_t>(t, 0x2155) - 1);
    float u = (float)((double)x + r);
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(t, 0x2159) - 1);
    u = (float)((double)y + r);
    if (my < u) u = my;
    y1 = (int32_t)u;
    int32_t count = 0;
    for (int32_t tries = 30; tries; --tries) {
        if (count) continue;
        for (int32_t ty = y0; ty <= y1; ++ty)
            for (int32_t tx = x0; tx <= x1; ++tx) {
                float w;
                if (r == Fl(0x5c8598)) w = 1.0f;
                else {
                    float py = (float)((double)ty + D(0x5c8628)), px = (float)((double)tx + D(0x5c8628));
                    w = (float)(((double)r - Distance2D(px, py, x, y)) / r);
                    if (w <= Fl(0x5c8598)) continue;
                }
                float p = (float)((double)w * Fl(0x5cadc0));
                p = (float)((double)p * density);
                if (erase) {
                    ++count;
                    Terrain_DecCellCount(t, 0, tx, ty, 6);
                    continue;
                }
                float rnd = (float)SimRand(0, 10000);
                if ((double)p * Fl(0x5c88c0) <= rnd) continue;
                ++count;
                kind %= 0x1c;
                int32_t k = kind * 7 + variant;
                if (!Map_CellLow3(Map(), 0, tx, ty) || force) Terrain_AddPoints(t, 0, tx, ty, (uint8_t)k, 1);
                if (((const uint8_t*)0x688058)[k] == 3)
                    while ((int32_t)Terrain_CellCount(t, 0, tx, ty) > 4) Terrain_DecCellCount(t, 0, tx, ty, 1);
            }
    }
    void* g = hook::Global<void*>(0x62be68);
    (*(void(__thiscall**)(void*))(*(void**)g))(g);
}
RT3_REPLACE(0x454320, Terrain_PaintPoints);

// 0x4575d0. Picks a random entry point and heading at the map edge (positions in
// world units, height above ground): over the land for +0x261 (north or south edge by
// season) else any of the four edges, heading towards a random point on the
// opposite edge.
void __fastcall Map_RandomEdgeEntry(uint8_t* o, int, float* outX, float* outY, float* outZ, float* heading) {
    int32_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    uint8_t* m;
    if (o[0x261]) {
        int32_t half = F<int32_t>(Map(), 0x2155) / 2;
        int32_t r = Rand(0, half);
        double q = (double)half * D(0x5c8628) + (double)r;
        __asm {
            fld q
            mov eax, 0x5a10d0
            call eax
            mov x0, eax
        }
        x1 = Rand(-10, 10) + x0;
        uint8_t month = hook::Global<uint8_t*>(0x6cec78)[0xf];
        if (month >= 1 && month <= 6) {
            y1 = F<int32_t>(Map(), 0x2159) - 2;
            y0 = 1;
        } else {
            y0 = F<int32_t>(Map(), 0x2159) - 2;
            y1 = 1;
        }
    } else {
        switch (Rand(0, 3)) {
        case 0:
            x0 = 1;
            y0 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            x1 = F<int32_t>(Map(), 0x2155) - 2;
            y1 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            break;
        case 1:
            x0 = F<int32_t>(Map(), 0x2155) - 2;
            y0 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            x1 = 1;
            y1 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            break;
        case 2:
            x0 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y0 = F<int32_t>(Map(), 0x2159) - 2;
            x1 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y1 = 1;
            break;
        case 3:
            x0 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y0 = 1;
            x1 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y1 = F<int32_t>(Map(), 0x2159) - 2;
            break;
        }
    }
    *outX = (float)(x0 * 0x5c);
    *outZ = (float)(y0 * 0x5c);
    F<uint32_t>(outY, 0) = F<uint32_t>(o, 0x26d);
    m = Map();
    if (x0 >= 0 && x0 < F<int32_t>(m, 0x15d9) - 1 && y0 >= 0 && y0 < F<int32_t>(m, 0x15d5) - 1)
        *outY = (float)(Terrain_HeightAtTile(m, 0, (float)x0, (float)y0) + *outY);
    float dx = (float)(x0 - x1), dy = (float)(y0 - y1);
    if (dy == Fl(0x5c8598)) dy = 0.01f;
    *heading = (float)((double(__cdecl*)(double, double))0x5a29d0)(dx, dy);
}
RT3_REPLACE(0x4575d0, Map_RandomEdgeEntry);

// 0x461cd0. The engine type closest to the current year: scored by the years from
// its introduction (+500 if not yet introduced), +50 if retired, +2000 if not
// available; kind 2 skipped. 1 if there are none.
uint32_t __fastcall EngineTypes_PickForYear(Collection* c) {
    uint32_t bestId = 1;
    int32_t best = 9999999;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x10) == 2) continue;
        auto ct = [&]() -> uint8_t* {
            return F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72))
                                        : nullptr;
        };
        int32_t intro = F<int32_t>(ct(), 0x105);
        int32_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
        int32_t score = abs(year - intro);
        if (intro > year) score += 0x1f4;
        if (F<int32_t>(ct(), 0x109) < (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd)) score += 0x32;
        if (!CarType_Available(ct(), 0, 0, 0)) score += 0x7d0;
        if (score < best) {
            best = score;
            bestId = Collection_IdAt(c, 0, i, 0);
        }
    }
    return bestId;
}
RT3_REPLACE(0x461cd0, EngineTypes_PickForYear);

} // namespace rt3
