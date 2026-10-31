// Territory tile counts / track clearance search / grade factor from the camera / straight line land check in blocks 0x48..0x49 (batch 171). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall Territories_BuildBorders(Collection* c);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Vehicle_RearMid(uint8_t* o, int, float* x, float* z);
void __fastcall Vehicle_FrontMid(uint8_t* o, int, float* x, float* z);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline double Bits(uint64_t u) { return *(double*)&u; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
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

// 0x4881b0. Counts the land tiles of each territory (+0x3d, tile flags >> 3), sums
// them into each group leader (+0x41) and rebuilds the borders.
void __fastcall Territories_Recount(Collection* c) {
    int32_t counts[1000];
    memset(counts, 0, sizeof counts);
    const uint8_t* t = F<uint8_t*>(Map(), 0x2131);
    for (int32_t y = 0; y < F<int32_t>(Map(), 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(Map(), 0x2155); ++x, ++t)
            if (!Map_CellLow3(Map(), 0, x, y)) ++counts[*t >> 3];
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        F<int32_t>(e, 0x3d) = counts[F<int32_t>(e, 0)];
        F<int32_t>(e, 0x41) = 0;
    }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x31)) continue;
        F<int32_t>(Collection_Get(c, 0, F<uint32_t>(e, 0x35)), 0x41) += F<int32_t>(e, 0x3d);
    }
    Territories_BuildBorders(hook::Global<Collection*>(0x6cfc9c));
}
RT3_REPLACE(0x4881b0, Territories_Recount);

// 0x492610. Finds the clearance for a vehicle on track piece p leaving away from
// `from`: tries heights (k x a + b) x c for k = 0..3, following the pieces (via
// +0x206/+0x20a) until a piece's terrain rises above the track height minus it. A
// bridge end (kind 3) or a blocked piece within 20 steps ends the search early.
// Returns 0 if a junction (+0x20e) is met or no height above the minimum fits.
int32_t __stdcall Track_FindClearance(uint8_t* p, int32_t from, float* height) {
    uint32_t a[3], b[3], c[3], d[3];
    float result = -1.0f;
    Effect_GetVectors(p, 0, a, b, c, d);
    int32_t next = F<int32_t>(p, 0x206);
    float h0;
    if (next == from) {
        next = F<int32_t>(p, 0x20a);
        h0 = (float)(((double)*(float*)&d[1] + *(float*)&c[1]) / D(0x5c85d0));
    } else h0 = (float)(((double)*(float*)&b[1] + *(float*)&a[1]) / D(0x5c85d0));
    for (int32_t k = 0; k < 4; ++k) {
        *height = (float)(((double)k * D(0x5c8758) + D(0x5c8770)) * D(0x5c8d68));
        int32_t cur = next, prev = F<int32_t>(p, 0x202), steps = 0;
        while (cur != -1) {
            uint8_t* q = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, cur);
            ++steps;
            bool hit;
            int32_t after;
            if (F<int32_t>(q, 0x226) == 3) hit = true;
            else {
                if (F<int32_t>(q, 0x20e) != -1) return 0;
                float lim = (float)((double)h0 - *height);
                Effect_GetVectors(q, 0, a, b, c, d);
                float x, z;
                bool rear = F<int32_t>(q, 0x206) == prev;
                if (rear) Vehicle_RearMid(q, 0, &x, &z);
                else Vehicle_FrontMid(q, 0, &x, &z);
                float k7 = Fl(0x5c8700);
                hit = Terrain_HeightAt(Map(), 0, (float)((double)x * k7), (float)((double)z * k7)) > lim;
                after = rear ? F<int32_t>(q, 0x20a) : F<int32_t>(q, 0x206);
            }
            if (hit) {
                if (steps < 0x14) return 1;
                result = *height;
                break;
            }
            prev = cur;
            cur = after;
        }
    }
    if (!(result > D(0x5c8608))) return 0;
    *height = result;
    return 1;
}
RT3_REPLACE(0x492610, Track_FindClearance);

// 0x493600. Route_GradeFactor from the camera focus (0x62be68 +0x1c..+0x24), or
// from the stored point (0x5f2b4c, 0x6cd988, 0x6ce6f8) when one is set.
float __stdcall Route_GradeFactorHere(float x, float z) {
    uint32_t sx = hook::Global<uint32_t>(0x5f2b4c);
    if (sx == 0x49741cf0) {
        uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
        return Route_GradeFactor(x, z, F<float>(cam, 0x1c), F<float>(cam, 0x20), F<float>(cam, 0x24));
    }
    return Route_GradeFactor(x, z, *(float*)&sx, hook::Global<float>(0x6cd988), hook::Global<float>(0x6ce6f8));
}
RT3_REPLACE(0x493600, Route_GradeFactorHere);

static inline bool LandAt(float x, float z) {
    if (!Map_Contains(Map(), 0, x, z)) return false;
    float k = Fl(0x5c8568);
    int32_t iz = Ftol((double)z * k);
    return !Map_CellFlag(Map(), 0, Ftol((double)x * k), iz, 2);
}

// 0x4980c0. Whether the straight line from (x0, z0) to (x1, z1) stays on the map and
// off water, sampled every tenth of a tile along the major axis.
int32_t __stdcall Map_LineOnLand(float x0, float z0, float x1, float z1) {
    float dx = (float)((double)x1 - x0), dz = (float)((double)z1 - z0);
    double major = Fabs(Fabs(dz) < Fabs(dx) ? dx : dz);
    int32_t n = Ftol(major / D(0x5c86b8));
    float slope = x1 == x0 ? 1e8f : (float)Fabs((double)dz / dx);
    float sx, sz;
    if (slope < D(0x5c8770)) {
        sx = (float)(x1 > x0 ? Bits(0x3fb999999999999aull) : Bits(0xbfb999999999999aull));
        sz = (float)((double)slope * (z1 > z0 ? D(0x5c86b8) : D(0x5cfe40)));
    } else {
        double inv = D(0x5c8770) / slope;
        sx = (float)(inv * (x1 > x0 ? D(0x5c86b8) : D(0x5cfe40)));
        sz = (float)(z1 > z0 ? Bits(0x3fb999999999999aull) : Bits(0xbfb999999999999aull));
    }
    for (int32_t i = 0; i <= n; ++i) {
        float fi = (float)i;
        float px = (float)((double)fi * sx + x0);
        float pz = (float)((double)fi * sz + z0);
        if (!LandAt(px, pz)) return 0;
    }
    return LandAt(x1, z1) ? 1 : 0;
}
RT3_REPLACE(0x4980c0, Map_LineOnLand);

} // namespace rt3
