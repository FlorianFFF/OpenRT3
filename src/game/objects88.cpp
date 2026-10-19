// Vehicle body geometry / camera load in blocks 0x48..0x53 (batch 91). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void* __fastcall Engine_Get1C(void* e);
uint32_t __fastcall Camera_Init(uint8_t* c);
void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);

// The four body corners (Effect_GetVectors: front-left +0x14, front-right +0x20,
// rear-left +0x2c, rear-right +0x38), each {x, y, z}.
struct Corners { float a[3], b[3], c[3], d[3]; };
static inline void GetCorners(uint8_t* o, Corners& k) {
    Effect_GetVectors(o, 0, (uint32_t*)k.a, (uint32_t*)k.b, (uint32_t*)k.c, (uint32_t*)k.d);
}
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
static inline double Atan(double v) { return ((double(__cdecl*)(double))0x5a1270)(v); }

// 0x489e90. Pushes an undo record (0x42 bytes: state, flags and the corners) onto
// the chain +0x244 and sets the new state (+0x240, +0x218).
void __fastcall Vehicle_PushState(uint8_t* o, int, uint32_t state, uint8_t mode) {
    uint8_t* p = (uint8_t*)Malloc(0x42);
    F<uint32_t>(p, 0) = F<uint32_t>(o, 0x240);
    p[4] = o[0x218];
    p[5] = o[0x216];
    p[6] = o[0x44] != 0;
    p[7] = F<int32_t>(o, 0x248) != 0;
    F<uint32_t>(p, 0xe) = F<uint32_t>(o, 0x244);
    F<uint32_t>(p, 0xa) = F<uint32_t>(o, 0x230);
    p[8] = o[0x217];
    p[9] = (o[0x24c] >> 4) & 1;
    Effect_GetVectors(o, 0, (uint32_t*)(p + 0x12), (uint32_t*)(p + 0x1e), (uint32_t*)(p + 0x2a), (uint32_t*)(p + 0x36));
    F<uint8_t*>(o, 0x244) = p;
    F<uint32_t>(o, 0x240) = state;
    o[0x218] = mode;
}
RT3_REPLACE(0x489e90, Vehicle_PushState);

// 0x48a440. Point at fraction t / 0x5c97e0 between the front and rear axle
// midpoints (x into *x, z into *z), in tile units.
void __fastcall Vehicle_PointAlong(uint8_t* o, int, float t, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c;
    const double K = *(const double*)0x5c8c48;
    float m0 = (float)(((double)k.b[0] + k.a[0]) / two);
    float m1 = (float)(((double)k.d[0] + k.c[0]) / two);
    double tt = (double)t / *(const double*)0x5c97e0;
    if (m1 > m0) *x = (float)((((double)m1 - m0) * tt + m0) / K);
    else *x = (float)(((double)m0 - ((double)m0 - m1) * tt) / K);
    float n0 = (float)(((double)k.b[2] + k.a[2]) / two);
    float n1 = (float)(((double)k.d[2] + k.c[2]) / two);
    if (n1 > n0) *z = (float)((((double)n1 - n0) * tt + n0) / K);
    else *z = (float)(((double)n0 - ((double)n0 - n1) * tt) / K);
}
RT3_REPLACE(0x48a440, Vehicle_PointAlong);

// 0x48a790. Height difference front-left minus rear-left, scaled (absolute when asked).
float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute) {
    Corners k;
    GetCorners(o, k);
    float v = (float)((double)k.a[1] - k.c[1]);
    v = (float)((double)v * *(const double*)0x5cfd88);
    if (!absolute) return v;
    return (float)((double(__cdecl*)(double))0x5a152e)(v);
}
RT3_REPLACE(0x48a790, Vehicle_Tilt);

// 0x48aad0. Whether any corner, lowered by `off`, lies below the terrain.
int32_t __fastcall Vehicle_CornerBelowGround(uint8_t* o, int, float off) {
    Corners k;
    GetCorners(o, k);
    float* c[4] = { k.a, k.b, k.c, k.d };
    for (int i = 0; i < 4; ++i) {
        float v = (float)((double)c[i][1] - off);
        if (Terrain_HeightAt(Map(), 0, c[i][0], c[i][2]) > v) return 1;
    }
    return 0;
}
RT3_REPLACE(0x48aad0, Vehicle_CornerBelowGround);

// 0x48b900 / 0x48b950. Front / rear axle midpoint in tile units.
void __fastcall Vehicle_FrontMid(uint8_t* o, int, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    *x = (float)(((double)k.b[0] + k.a[0]) / two / tile);
    *z = (float)(((double)k.b[2] + k.a[2]) / two / tile);
}
RT3_REPLACE(0x48b900, Vehicle_FrontMid);
void __fastcall Vehicle_RearMid(uint8_t* o, int, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    *x = (float)(((double)k.d[0] + k.c[0]) / two / tile);
    *z = (float)(((double)k.d[2] + k.c[2]) / two / tile);
}
RT3_REPLACE(0x48b950, Vehicle_RearMid);

// 0x48b9a0. Axle distance in tile units (unrounded).
double __fastcall Vehicle_Length(uint8_t* o) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    float p0 = (float)(((double)k.b[0] + k.a[0]) / two / tile);
    float p1 = (float)(((double)k.b[2] + k.a[2]) / two / tile);
    float p2 = (float)(((double)k.d[0] + k.c[0]) / two / tile);
    float p3 = (float)(((double)k.d[2] + k.c[2]) / two / tile);
    return Distance2D(p0, p1, p2, p3);
}
RT3_REPLACE(0x48b9a0, Vehicle_Length);

// Wraps an angle into [0, 2 pi) (0x5c96b8) in float steps.
static inline float WrapAngle(float a) {
    const double twoPi = *(const double*)0x5c96b8, zero = *(const double*)0x5c8608;
    while ((double)a >= twoPi) a = (float)((double)a - twoPi);
    while ((double)a < zero) a = (float)((double)a + twoPi);
    return a;
}

// 0x48ba40. Heading of the side front-left -> rear-right (+ pi when reversed).
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward) {
    Corners k;
    Effect_GetVectors(o, 0, (uint32_t*)k.a, nullptr, nullptr, (uint32_t*)k.d);
    float dx = (float)((double)k.a[0] - k.d[0]);
    if (dx == *(const float*)0x5c8598) *(uint32_t*)&dx = 0xb8d1b717;
    float a = (float)Atan(((double)k.a[2] - k.d[2]) / dx);
    const double pi = *(const double*)0x5c96a8;
    if (k.d[0] < k.a[0]) a = (float)((double)a + pi);
    if (!forward) a = (float)((double)a + pi);
    return WrapAngle(a);
}
RT3_REPLACE(0x48ba40, Vehicle_Heading);

// 0x48bb40. Same, as a screen angle (0x5cfda0 - (atan + 0x5c96f0)).
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward) {
    Corners k;
    Effect_GetVectors(o, 0, (uint32_t*)k.a, nullptr, nullptr, (uint32_t*)k.d);
    float dx = (float)((double)k.a[0] - k.d[0]);
    if (dx == *(const float*)0x5c8598) *(uint32_t*)&dx = 0xb8d1b717;
    float a = (float)(Atan(((double)k.a[2] - k.d[2]) / dx) + *(const double*)0x5c96f0);
    a = (float)(*(const double*)0x5cfda0 - a);
    const double pi = *(const double*)0x5c96a8;
    if (k.d[0] < k.a[0]) a = (float)((double)a + pi);
    if (!forward) a = (float)((double)a + pi);
    return WrapAngle(a);
}
RT3_REPLACE(0x48bb40, Vehicle_ScreenHeading);

// 0x48bc50. Pitch atan(dh / length) between the axle midpoints (signed by direction).
float __fastcall Vehicle_Pitch(uint8_t* o, int, int32_t forward) {
    Corners k;
    GetCorners(o, k);
    const double half = *(const double*)0x5c85d0;
    float fx = (float)(((double)k.b[0] + k.a[0]) / half);
    float fz = (float)(((double)k.b[2] + k.a[2]) / half);
    float fy = (float)(((double)k.b[1] + k.a[1]) / half);
    float rx = (float)(((double)k.d[0] + k.c[0]) / half);
    float rz = (float)(((double)k.d[2] + k.c[2]) / half);
    float ry = (float)(((double)k.d[1] + k.c[1]) / half);
    float dist = (float)Distance2D(fx, fz, rx, rz);
    if ((double)dist == *(const double*)0x5c8608) return *(const float*)0x5c8598;
    if (forward) return (float)Atan(((double)ry - fy) / dist);
    return (float)Atan(((double)fy - ry) / dist);
}
RT3_REPLACE(0x48bc50, Vehicle_Pitch);

// 0x495580. Copies a body's corners into this object (+0x139..+0x15d).
void __fastcall Obj_CopyCorners(uint8_t* o, int, uint8_t* body) {
    Effect_GetVectors(body, 0, (uint32_t*)(o + 0x139), (uint32_t*)(o + 0x145), (uint32_t*)(o + 0x151),
                      (uint32_t*)(o + 0x15d));
}
RT3_REPLACE(0x495580, Obj_CopyCorners);

// 0x52d750. Loads a camera (eye, distance, angles, target; 20 bytes skipped) and
// re-attaches it to the engine's terrain. Returns the bytes read.
int32_t __fastcall Camera_Load(uint8_t* c, int, Stream s) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (e && F<uint8_t*>(e, 0x28) == c) F<uint8_t*>(e, 0x28) = nullptr;
    int32_t n = StreamRead(s, c + 4, 4, 0, 1);
    n += StreamRead(s, c + 8, 4, 0, 1);
    n += StreamRead(s, c + 0xc, 4, 0, 1);
    n += StreamRead(s, c + 0x18, 4, 0, 1);
    Camera_Setup(c, 0, (uint32_t)Engine_Get1C(hook::Global<void*>(0x6d4024)), F<uint32_t>(c, 4), F<uint32_t>(c, 8),
                 F<uint32_t>(c, 0xc), F<uint32_t>(c, 0x18));
    static const uint8_t rest[] = { 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (uint8_t off : rest) n += StreamRead(s, c + off, 4, 0, 1);
    uint8_t skip[0x14];
    return StreamRead(s, skip, 0x14, 0, 1) + n;
}
RT3_REPLACE(0x52d750, Camera_Load);

uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z,
                                          uint32_t dist) {
    F<uint32_t>(c, 0) = 0x5dd1c8;
    Camera_Init(c);
    Camera_Setup(c, 0, terrain, x, y, z, dist);
    return c;
}
RT3_REPLACE(0x52d860, Camera_ConstructSetup);

void __fastcall Effect_Release(uint8_t* e) {
    F<uint32_t>(e, 0) = 0x5dd3d8;
    Effect_Destroy(e, 0, 1);
}
RT3_REPLACE(0x53a590, Effect_Release);

void __fastcall FreeAndNull2(void** p) {
    if (!*p) return;
    Free(*p);
    *p = nullptr;
}
RT3_REPLACE(0x53aff0, FreeAndNull2);

void __fastcall Set_ccba28() { hook::Global<int32_t>(0xccba28) = 1; }
RT3_REPLACE(0x53b010, Set_ccba28);

} // namespace rt3
