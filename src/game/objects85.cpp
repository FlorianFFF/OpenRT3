// Terrain height / water-edge queries in blocks 0x41..0x53 (batch 88). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Engine_Get1C(void* e);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

static inline double Fsin(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }
static inline double Fcos(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
// Eight neighbour directions (dx 0x624b28, dy 0x624b48).
static inline int32_t DirX(int32_t d) { return ((const int32_t*)0x624b28)[d]; }
static inline int32_t DirY(int32_t d) { return ((const int32_t*)0x624b48)[d]; }
// Terrain vertex record (+0x165d, 3 bytes, stride +0x15dd).
static inline uint8_t* Rec(uint8_t* r, int32_t x, int32_t y) {
    return F<uint8_t*>(r, 0x165d) + (F<int32_t>(r, 0x15dd) * y + x) * 3;
}

// 0x415370. Renderer size in pixels per map tile (x: +0x215d, y: +0x2161).
void __stdcall Map_PixelsPerTile(float* sx, float* sy) {
    int32_t w, h;
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, &w, &h);
    float fw = (float)w, mw = (float)F<int32_t>(Map(), 0x215d);
    *sx = (float)((double)fw / mw);
    float fh = (float)h, mh = (float)F<int32_t>(Map(), 0x2161);
    *sy = (float)((double)fh / mh);
}
RT3_REPLACE(0x415370, Map_PixelsPerTile);

// 0x439d30. Height of the camera target (+0x1c/+0x20/+0x24) above the terrain (+0x6e).
double __fastcall Camera_TargetAboveGround(uint8_t* c) {
    return (double)F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24));
}
RT3_REPLACE(0x439d30, Camera_TargetAboveGround);

// 0x448bd0. Terrain height at tile coordinates (x 0x5c8700 world units).
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z) {
    const float k = *(const float*)0x5c8700;
    float wz = (float)((double)z * k);
    float wx = (float)((double)x * k);
    return Terrain_HeightAt(r, 0, wx, wz);
}
RT3_REPLACE(0x448bd0, Terrain_HeightAtTile);

// 0x449980. Whether the vertex is not plain land (kind != 0 or bit 9).
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y) {
    if (Rec(r, x, y)[0] & 7) return 1;
    return Terrain_RecBit9(r, 0, x, y) ? 1 : 0;
}
RT3_REPLACE(0x449980, Terrain_IsSpecial);

static inline bool OutsideGrid(int32_t cx, int32_t cy) {
    return cx < 0 || cx >= F<int32_t>(Map(), 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(Map(), 0x15d5) - 1;
}

// 0x449ab0. Whether water (kind 1 / 3..5) lies within `range` steps of (x, y) in
// any of the eight directions (bounds checked only near the map edge).
int32_t __fastcall Map_WaterWithin(uint8_t* m, int, int32_t x, int32_t y, int32_t range) {
    bool edge = true;
    if ((int32_t)((uint32_t)x - range) >= 0 && (int32_t)((uint32_t)y - range) >= 0 &&
        x + range <= F<int32_t>(m, 0x2155) - 1 && y + range <= F<int32_t>(m, 0x2159) - 1)
        edge = false;
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return 1;
    for (int32_t d = 0; d < 8; ++d) {
        for (int32_t s = 1; s <= range; ++s) {
            int32_t cx = DirX(d) * s + x, cy = DirY(d) * s + y;
            if (edge && OutsideGrid(cx, cy)) continue;
            if (Terrain_KindIs1Or3to5(m, 0, cx, cy)) return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x449ab0, Map_WaterWithin);

// 0x449bd0. For a bit-9 vertex: whether it or an orthogonal neighbour is water.
int32_t __fastcall Map_Bit9NearWater(uint8_t* m, int, int32_t x, int32_t y) {
    if (!Terrain_RecBit9(m, 0, x, y)) return 0;
    bool edge = !(x > 0 && y > 0 && x < F<int32_t>(m, 0x2155) - 1 && y < F<int32_t>(m, 0x2159) - 1);
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return 1;
    for (int32_t d = 0; d < 8; d += 2) {
        int32_t cx = DirX(d) + x, cy = DirY(d) + y;
        if (edge && OutsideGrid(cx, cy)) continue;
        if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x449bd0, Map_Bit9NearWater);

// 0x449f20. Counts the land vertices into +0x21a9.
void __fastcall Map_CountLand(uint8_t* m) {
    F<int32_t>(m, 0x21a9) = 0;
    for (int32_t y = 0; y < F<int32_t>(m, 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2155); ++x)
            if (!Terrain_KindIs1Or3to5(m, 0, x, y)) F<int32_t>(m, 0x21a9)++;
}
RT3_REPLACE(0x449f20, Map_CountLand);

// 0x44bdb0. Number of the eight neighbours of kind 2 / 4 / 5.
int32_t __fastcall Map_CountShoreNeighbours(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t n = 0;
    for (int32_t d = 0; d < 8; ++d) {
        int32_t cx = DirX(d) + x, cy = DirY(d) + y;
        if (cx < 0 || cx >= F<int32_t>(m, 0x2155) || cy < 0 || cy >= F<int32_t>(m, 0x2159)) continue;
        if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) ++n;
    }
    return n;
}
RT3_REPLACE(0x44bdb0, Map_CountShoreNeighbours);

// 0x44c570. Marks a free land tile (+0x2135 flags) as changed (bits 0xc3 kept,
// value 2) and grows the dirty rectangle +0x21c6..+0x21d2 (count +0x21d6).
void __fastcall Map_MarkTileDirty(uint8_t* m, int, int32_t x, int32_t y) {
    if (x < 0 || x >= F<int32_t>(m, 0x15d9) - 1) return;
    if (y < 0 || y >= F<int32_t>(m, 0x15d5) - 1) return;
    if (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 0x3e) return;
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return;
    int32_t n = F<int32_t>(m, 0x21d6);
    if (!n) {
        F<int32_t>(m, 0x21c6) = x;
        F<int32_t>(m, 0x21ca) = y;
        F<int32_t>(m, 0x21ce) = x;
        F<int32_t>(m, 0x21d2) = y;
    } else {
        if (x < F<int32_t>(m, 0x21c6)) F<int32_t>(m, 0x21c6) = x;
        if (y < F<int32_t>(m, 0x21ca)) F<int32_t>(m, 0x21ca) = y;
        if (x > F<int32_t>(m, 0x21ce)) F<int32_t>(m, 0x21ce) = x;
        if (y > F<int32_t>(m, 0x21d2)) F<int32_t>(m, 0x21d2) = y;
    }
    F<int32_t>(m, 0x21d6) = n + 1;
    uint8_t* cell = F<uint8_t*>(m, 0x2135) + (F<int32_t>(m, 0x2155) + 1) * y + x;
    *cell = (uint8_t)((*cell & 0xc3) | 2);
}
RT3_REPLACE(0x44c570, Map_MarkTileDirty);

// 0x48a020. Height of the object's middle (+0x1ed/+0x1f1) above the terrain at (+0xc, +0x10).
float __fastcall Obj_MidAboveGround(uint8_t* o) {
    float h = (float)Terrain_HeightAt(Map(), 0, F<float>(o, 0xc), F<float>(o, 0x10));
    return (float)(((double)F<float>(o, 0x1f1) + F<float>(o, 0x1ed)) / *(const double*)0x5c85d0 - h);
}
RT3_REPLACE(0x48a020, Obj_MidAboveGround);

// 0x4951f0. Slope angle |atan(dh / dist)| between the midpoints of two segments
// (tile units scaled by 0x5c8c48); 0 when they coincide.
float __stdcall Map_SlopeBetween(float ax0, float ay0, float ax1, float ay1, float bx0, float by0, float bx1,
                                 float by1) {
    const double half = *(const double*)0x5c85d0, k = *(const double*)0x5c8c48;
    float p0 = (float)(((double)ax0 + ax1) / half * k);
    float p1 = (float)(((double)ay0 + ay1) / half * k);
    float p2 = (float)(((double)bx0 + bx1) / half * k);
    float p3 = (float)(((double)by0 + by1) / half * k);
    float dist = (float)Distance2D(p0, p1, p2, p3);
    float h1 = (float)Terrain_HeightAt(Map(), 0, p0, p1);
    float h2 = (float)Terrain_HeightAt(Map(), 0, p2, p3);
    if ((double)dist == *(const double*)0x5c8608) return *(const float*)0x5c8598;
    double a = ((double(__cdecl*)(double))0x5a1270)(((double)h2 - h1) / dist);
    return (float)((double(__cdecl*)(double))0x5a152e)(a);
}
RT3_REPLACE(0x4951f0, Map_SlopeBetween);

// 0x52cec0. Terrain height under the object's position (+0x1c, +0x24), snapped
// through tile units (x87 products as in the original).
double __fastcall SceneObj_GroundHeight(uint8_t* o) {
    float x, z;
    __asm {
        mov ecx, o
        fld dword ptr [ecx + 0x1c]
        fmul dword ptr ds:[0x5dcfe8]
        fld dword ptr [ecx + 0x24]
        fmul dword ptr ds:[0x5dcfe8]
        fmul dword ptr ds:[0x5c8700]
        fstp z
        fmul dword ptr ds:[0x5c8700]
        fstp x
    }
    return Terrain_HeightAt(F<uint8_t*>(o, 0x6e), 0, x, z);
}
RT3_REPLACE(0x52cec0, SceneObj_GroundHeight);

// 0x52d1e0. Sets the camera target and its height above the terrain (+0x66).
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z) {
    F<float>(c, 0x20) = y;
    F<float>(c, 0x1c) = x;
    F<float>(c, 0x24) = z;
    if (!F<uint8_t*>(c, 0x6e)) {
        F<float>(c, 0x66) = y;
        return;
    }
    double h = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, x, z);
    F<float>(c, 0x66) = (float)((double)F<float>(c, 0x20) - h);
}
RT3_REPLACE(0x52d1e0, Camera_SetTarget);

// 0x52d450. Places the target at distance +0x18 from the eye (+4..) along yaw +0x10 /
// pitch +0x14 and updates its height above the terrain.
void __fastcall Camera_UpdateTarget(uint8_t* c) {
    double cp = Fcos(F<float>(c, 0x14));
    float x = (float)(Fsin(F<float>(c, 0x10)) * cp * F<float>(c, 0x18) + F<float>(c, 0x4));
    F<float>(c, 0x1c) = x;
    float y = (float)(Fsin(F<float>(c, 0x14)) * F<float>(c, 0x18) + F<float>(c, 0x8));
    F<float>(c, 0x20) = y;
    float z = (float)(Fcos(F<float>(c, 0x10)) * cp * F<float>(c, 0x18) + F<float>(c, 0xc));
    F<float>(c, 0x24) = z;
    if (!F<uint8_t*>(c, 0x6e)) {
        F<float>(c, 0x66) = y;
        return;
    }
    double h = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, x, z);
    F<float>(c, 0x66) = (float)((double)F<float>(c, 0x20) - h);
}
RT3_REPLACE(0x52d450, Camera_UpdateTarget);

int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 3 || k == 5 ? 1 : 0;
}
RT3_REPLACE(0x534f00, Terrain_KindIs3Or5);
uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y) { return Rec(r, x, y); }
RT3_REPLACE(0x534f40, Terrain_Rec);

} // namespace rt3
