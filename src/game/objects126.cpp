// Recipe value / car cargo value / map distance, height and fog helpers in blocks 0x40..0x44 (batch 129). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t IsMultiplayer();
float __fastcall Net_CommonVersion();
void* __fastcall Obj_Train390(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
void __fastcall Obj_SetAmount39c(void* o, int, float v);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t Option2bbActive();
int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
void __fastcall Map_MarkTileDirty(uint8_t* m, int, int32_t x, int32_t y);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;

// Cell centre (n + 0.5) as a float.
static inline float Centre(int32_t n) { return (float)((double)n + D(0x5c8628)); }
// Cell centre in world units.
static inline float CentreWorld(int32_t n) { return (float)((double)Centre(n) * Fl(0x5c8700)); }

// 0x40faa0. Net value per unit of production recipe rec (0xbc bytes) at o's city:
// the output (+0x1c, x +4) at the city's quota minus the inputs (+0x2c items
// {cargo +0x44, amount +0x48}), divided by the output amount.
double __fastcall Recipe_NetValue(void* o, int, uint8_t* rec) {
    int32_t cellX, cellY;
    __asm {
        mov ecx, o
        mov eax, 0x455800
        call eax
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov cellX, eax
        mov ecx, o
        mov eax, 0x455810
        call eax
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov cellY, eax
    }
    uint8_t* m = Map();
    uint8_t* city = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * cellY + cellX) * 0x1d7;
    float out = (float)((double)(float)City_CargoQuota(city, 0, F<int32_t>(rec, 0x1c)) * F<float>(rec, 4));
    float in = 0.0f;
    for (int32_t k = 0; k < F<int32_t>(rec, 0x2c); ++k) {
        float q = (float)City_CargoQuota(city, 0, F<int32_t>(rec, 0x44 + k * 0x1c));
        in = (float)((double)q * F<float>(rec, 0x48 + k * 0x1c) + in);
    }
    return ((double)out - in) / F<float>(rec, 4);
}
RT3_REPLACE(0x40faa0, Recipe_NetValue);

// 0x41b9e0. Sets the car's cargo value (+0x39c) on arrival at city: the quota for
// its cargo (+0x38c) x amount (+0x398) less the paid price (+0x3a4); nothing when
// it is the region it came from (train +0x390, building +0x9c) or the cargo is not
// produced (multiplayer games of version 0x5c9800+ / single player). Then scales
// +0x3a0 by the mode's factor.
void __fastcall Car_SetCargoValue(uint8_t* car, int, uint8_t* city) {
    int32_t mode;
    if (IsMultiplayer()) mode = !(Net_CommonVersion() >= Fl(0x5c9800)) ? 0 : 1;
    else mode = 2;
    if (!F<uint32_t>(car, 0x38c)) return;
    if (!(F<float>(car, 0x398) > Fl(0x5c8598))) return;
    float value = 0.0f;
    if (city) {
        uint8_t* from = nullptr;
        int32_t tid = F<int32_t>(car, 0x390);
        uint8_t* t = tid != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, tid) : nullptr;
        if (Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(t, 0x9c)))
            from = Obj_BuildingRegion(Collection_Get(hook::Global<Collection*>(0x6cec20), 0,
                                                     F<uint32_t>(Obj_Train390(car), 0x9c)));
        if (mode < 1 || (city != from && F<int32_t>(Cargos(), 0x9a + F<int32_t>(car, 0x38c) * 4) == -1)) {
            float q = (float)City_CargoQuota(city, 0, F<int32_t>(car, 0x38c));
            value = (float)((double)q * F<float>(car, 0x398) - F<float>(car, 0x3a4));
        }
    }
    Obj_SetAmount39c(car, 0, value);
    static const uint32_t factor[3] = { 0x5c8750, 0x5c85a0, 0x5c97f8 };
    F<float>(car, 0x3a0) = (float)((double)F<float>(car, 0x3a0) * D(factor[mode]));
}
RT3_REPLACE(0x41b9e0, Car_SetCargoValue);

// 0x449cb0. Distance (at most 4) from (x, y) to the nearest cell within 4 whose
// terrain kind is not 1 or 4.
float __fastcall Map_DistanceToOtherKind(uint8_t* m, int, float x, float y) {
    float best = 4.0f;
    int32_t ix = (int32_t)x, iy = (int32_t)y;
    int32_t x0 = ix - 4 < 0 ? 0 : ix - 4;
    int32_t y0 = iy - 4 < 0 ? 0 : iy - 4;
    int32_t x1 = F<int32_t>(m, 0x2155) - 1;
    if (!(x1 < ix + 4)) x1 = ix + 4;
    int32_t y1 = F<int32_t>(m, 0x2159) - 1;
    if (!(y1 < iy + 4)) y1 = iy + 4;
    for (int32_t j = y0; j <= y1; ++j)
        for (int32_t i = x0; i <= x1; ++i) {
            if (Terrain_KindIs1Or4(m, 0, i, j)) continue;
            float d = (float)(Distance2D(x, y, Centre(i), Centre(j)) - D(0x5c8628));
            if (!(best < d)) best = d;
        }
    return best;
}
RT3_REPLACE(0x449cb0, Map_DistanceToOtherKind);

// 0x44a030. Sum over all cities (+0x2129, 0x1d7 bytes) with +0xeb set of the quota
// and stock (+0x103) of every enabled cargo (0x62ba8c +0x16e).
float __fastcall Map_TotalCargoValue(uint8_t* m, int, int32_t) {
    float sum = 0.0f;
    uint32_t n = (uint32_t)(F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145));
    uint8_t* city = F<uint8_t*>(m, 0x2129);
    for (; n; --n, city += 0x1d7) {
        if (!F<int32_t>(city, 0xeb)) continue;
        for (int32_t i = 0x34; i >= 0; --i) {
            if (!F<int32_t>(Cargos(), 0x16e + i * 4)) continue;
            float q = (float)City_CargoQuota(city, 0, i);
            sum = (float)((double)q + sum);
            sum = (float)((double)sum + F<float>(city, 0x103 + i * 4));
        }
    }
    return sum;
}
RT3_REPLACE(0x44a030, Map_TotalCargoValue);

// 0x44ae40. Terrain height at the cell (x, y) if its kind is 1 or 3..5, else at the
// first such cell found in the 4 directions within r rings; 0 when none.
double __fastcall Map_NearestLandHeight(uint8_t* m, int, int32_t x, int32_t y, int32_t r) {
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return Terrain_HeightAt(m, 0, CentreWorld(x), CentreWorld(y));
    for (int32_t k = 1; k <= r; ++k)
        for (int j = 0; j < 4; ++j) {
            int32_t nx = kDirX[j * 2] * k + x;
            int32_t ny = kDirY[j * 2] * k + y;
            if (nx < 0 || nx >= F<int32_t>(m, 0x2155) || ny < 0 || ny >= F<int32_t>(m, 0x2159)) continue;
            if (Terrain_KindIs1Or3to5(m, 0, nx, ny))
                return Terrain_HeightAt(m, 0, CentreWorld(nx), CentreWorld(ny));
        }
    return Fl(0x5c8598);
}
RT3_REPLACE(0x44ae40, Map_NearestLandHeight);

// 0x44afa0. Fog end distance at cell (x, y): 0x8c plus a far-camera term (cell
// distance to the view target 50..200 -> 0x5c8880 scale), or a zoom term above
// 0x5ca190 (+0x20), +50 with option 0x2bb.
int32_t __stdcall Map_FogEnd(int32_t x, int32_t y) {
    uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
    float tz = F<float>(cam, 0x24), tx = F<float>(cam, 0x1c);
    int32_t cellZ, cellX;
    __asm {
        fld tz
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellZ, eax
        fld tx
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellX, eax
    }
    int32_t dist = (uint16_t)DistanceInt(x, y, cellX, cellZ);
    int32_t fog = 0;
    if (dist > 0x32) {
        int32_t d = (dist >= 0xc8 ? 0xc8 : dist) - 0x32;
        __asm {
            fild d
            fmul qword ptr ds:[0x5c8880]
            mov eax, 0x5a10d0
            call eax
            mov fog, eax
        }
    }
    int32_t w = F<int32_t>(Map(), 0x15d9) + 1;
    const float* grid = F<const float*>(Map(), 0x1605);
    int32_t base = (int32_t)grid[w * y + x];
    int32_t top = base;
    for (int32_t k = 1; k <= 2; ++k)
        for (int j = 0; j < 4; ++j) {
            int32_t nx = kDirX[j * 2] * k * 7 + x;
            int32_t ny = kDirY[j * 2] * k * 7 + y;
            if (nx < 0 || ny < 0 || nx >= F<int32_t>(Map(), 0x2155) || ny >= F<int32_t>(Map(), 0x2159)) continue;
            int32_t v = (int32_t)grid[w * ny + nx];
            if (!(top > v)) top = v;
        }
    int32_t r = fog + (top - base);
    float zoom = F<float>(hook::Global<uint8_t*>(0x62be68), 0x20);
    if (zoom > Fl(0x5ca190)) {
        __asm {
            fld zoom
            fsub dword ptr ds:[0x5ca190]
            fmul qword ptr ds:[0x5c86b0]
            fiadd fog
            mov eax, 0x5a10d0
            call eax
            mov r, eax
        }
    }
    if (Option2bbActive()) r += 0x32;
    return r + 0x8c;
}
RT3_REPLACE(0x44afa0, Map_FogEnd);

// 0x44dcf0. Sets bit 9 of each cell word (+0x165d, 3 bytes, stride +0x15dd) in the
// clamped rect when it or one of its 8 neighbours is of kind 2/4/5 (neighbours
// off the inner map are skipped near the edge).
void __fastcall Map_MarkNearKind245(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    Map_ClampRect(m, 0, &x0, &y0, &x1, &y1);
    int32_t edge = 1;
    if (x0 > 2 && y0 > 2 && x1 < F<int32_t>(m, 0x2155) - 3 && y1 < F<int32_t>(m, 0x2159) - 3) edge = 0;
    for (int32_t x = x0; x <= x1; ++x)
        for (int32_t y = y0; y <= y1; ++y) {
            int32_t v = Terrain_KindIs2Or4Or5(m, 0, x, y);
            for (int j = 0; j < 8; ++j) {
                int32_t nx = kDirX[j] + x, ny = kDirY[j] + y;
                if (edge && !(nx > 0 && ny > 0 && nx < F<int32_t>(m, 0x2155) - 1 && ny < F<int32_t>(m, 0x2159) - 1))
                    continue;
                if (Terrain_KindIs2Or4Or5(m, 0, nx, ny)) v = 1;
            }
            uint16_t* cell = (uint16_t*)(F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3);
            *cell ^= (uint16_t)(((uint32_t)v << 9 ^ *cell) & 0x200);
        }
}
RT3_REPLACE(0x44dcf0, Map_MarkNearKind245);

// 0x44e7d0. Marks dirty every tile whose centre is within 0x5c8dc0 of (x, y).
void __fastcall Map_MarkDirtyAround(uint8_t* m, int, float x, float y) {
    if (!Map_Contains(m, 0, x, y)) return;
    int32_t x0, x1, y0, y1;
    __asm {
        fld x
        fsub qword ptr ds:[0x5c8dc0]
        mov eax, 0x5a10d0
        call eax
        mov x0, eax
        fld x
        fadd qword ptr ds:[0x5c8c30]
        mov eax, 0x5a10d0
        call eax
        mov x1, eax
        fld y
        fsub qword ptr ds:[0x5c8dc0]
        mov eax, 0x5a10d0
        call eax
        mov y0, eax
        fld y
        fadd qword ptr ds:[0x5c8c30]
        mov eax, 0x5a10d0
        call eax
        mov y1, eax
    }
    if (x0 < 0) x0 = 0;
    if (x1 > F<int32_t>(m, 0x2155) - 1) x1 = F<int32_t>(m, 0x2155) - 1;
    if (y0 < 0) y0 = 0;
    if (y1 > F<int32_t>(m, 0x2159) - 1) y1 = F<int32_t>(m, 0x2159) - 1;
    for (int32_t j = y0; j <= y1; ++j) {
        float fy = Centre(j);
        for (int32_t i = x0; i <= x1; ++i)
            if (Distance2D(Centre(i), fy, x, y) < D(0x5c8dc0)) Map_MarkTileDirty(m, 0, i, j);
    }
}
RT3_REPLACE(0x44e7d0, Map_MarkDirtyAround);

} // namespace rt3
