// Terrain slope / camera ground follow / handle table in blocks 0x40..0x53 (batch 89). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Camera_UpdateTarget(uint8_t* c);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
int32_t ClockMs();
void __fastcall String_Assign(char** s, const char* src);

static inline double Fsin(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }
static inline double Fsqrt(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

// 0x402c00. Slope angle atan((h(a) - h(b)) / distance) between two tile points
// (distance in world units, 0.01 when the points coincide).
float __stdcall Map_SlopeAngle(float ax, float ay, float bx, float by) {
    float ha = (float)Terrain_HeightAtTile(Map(), 0, ax, ay);
    float hb = (float)Terrain_HeightAtTile(Map(), 0, bx, by);
    float d = (float)(Distance2D(ax, ay, bx, by) * *(const float*)0x5c8700);
    if (d == *(const float*)0x5c8598) *(uint32_t*)&d = 0x3c23d70a;
    return (float)((double(__cdecl*)(double))0x5a1270)(((double)ha - hb) / d);
}
RT3_REPLACE(0x402c00, Map_SlopeAngle);

// 0x42b050. Classifies the 4x4 tiles of a site (+0xe7/+0xe9 in 4-tile units):
// flag 1 of +0xe6 when at least 8 are water, flag 2 when at least 2 are shore.
void __fastcall Site_ClassifyWater(uint8_t* s) {
    int32_t y0 = (int32_t)F<int16_t>(s, 0xe9) << 2;
    int32_t x0 = (int32_t)F<int16_t>(s, 0xe7) << 2;
    int32_t water = 0, shore = 0;
    for (int32_t y = y0; y <= y0 + 3; ++y)
        for (int32_t x = x0; x <= x0 + 3; ++x) {
            if (Terrain_KindIs1Or4(Map(), 0, x, y) || Terrain_KindIs3Or5(Map(), 0, x, y)) ++water;
            if (Terrain_KindIs2Or4Or5(Map(), 0, x, y)) ++shore;
        }
    if (water >= 8) s[0xe6] |= 1;
    else s[0xe6] &= 0xfe;
    if (shore >= 2) s[0xe6] |= 2;
    else s[0xe6] &= 0xfd;
}
RT3_REPLACE(0x42b050, Site_ClassifyWater);

// 0x52d5a0. Camera defaults: pitch 0.2, distance 100, scale 1.
uint32_t __fastcall Camera_Init(uint8_t* c) {
    F<int32_t>(c, 0x6e) = 0;
    F<int32_t>(c, 0x66) = 0;
    F<int32_t>(c, 0x10) = 0;
    F<uint32_t>(c, 0x14) = 0x3e4ccccd;
    F<int32_t>(c, 0x44) = 0;
    F<int32_t>(c, 0x48) = 0;
    F<int32_t>(c, 0x60) = 0;
    Camera_UpdateTarget(c);
    uint32_t yaw = F<uint32_t>(c, 0x10), pitch = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x58) = 0x3f800000;
    F<uint32_t>(c, 0x5c) = 0x3f800000;
    F<uint32_t>(c, 0x30) = 0x3f800000;
    static const uint8_t zero[] = { 0x50, 0x54, 0x2c, 0x34, 0x40, 0x3c, 0x38, 0xc, 0x8, 0x4 };
    for (uint8_t off : zero) F<int32_t>(c, off) = 0;
    c[0x65] = 0;
    F<int32_t>(c, 0x7a) = 0;
    F<int32_t>(c, 0x7e) = 0;
    F<int32_t>(c, 0x82) = 0;
    c[0x64] = 1;
    F<int32_t>(c, 0x6a) = -1;
    F<int32_t>(c, 0x28) = 0x64;
    F<uint32_t>(c, 0x18) = 0x42c80000;
    F<uint32_t>(c, 0x4c) = 0x42c80000;
    F<uint32_t>(c, 0x72) = yaw;
    F<uint32_t>(c, 0x76) = pitch;
    F<uint32_t>(c, 0x86) = 0x42c80000;
    return 0x42c80000;
}
RT3_REPLACE(0x52d5a0, Camera_Init);

// 0x52d640. Keeps the target above the ground limit (vtbl +0xc): pulls the
// distance in by the shortfall / sin(pitch) (pitch sine at least 0.01).
void __fastcall Camera_KeepAboveGround(uint8_t* c) {
    Camera_UpdateTarget(c);
    auto limit = (int32_t(__thiscall*)(void*))(*(void***)c)[0xc / 4];
    if (!((double)limit(c) <= F<float>(c, 0x20))) return;
    double s = Fsin(F<float>(c, 0x14));
    if (s < *(const double*)0x5c85b8) s = 0.01;
    double dist = F<float>(c, 0x18);
    double g = (double)limit(c);
    F<float>(c, 0x18) = (float)(dist - ((double)F<float>(c, 0x20) - g) / s);
    Camera_UpdateTarget(c);
}
RT3_REPLACE(0x52d640, Camera_KeepAboveGround);

// 0x535f20. Frees the +0x167d block lists (items, lists and the grid).
RT3_NOINLINE void __fastcall Terrain_DestroyCell167d(uint8_t* r) {
    if (!F<void*>(r, 0x167d)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x167d)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    void* p = PtrList_Next(l);
                    do {
                        Free(p);
                        p = PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x167d)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ((void(__cdecl*)(void*))0x53b080)(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x167d));
    F<void*>(r, 0x167d) = nullptr;
}
RT3_REPLACE(0x535f20, Terrain_DestroyCell167d);

// 0x537170. Detail factor for the tile (x, z) from its distance to the camera
// (engine +0x28; distance at least 0x5c85b4): (c / d + sqrt(c / d)) * 0x5dd2f0.
RT3_NOINLINE void __stdcall Camera_DetailFactor(int32_t x, int32_t z, float* out) {
    float cx, cy, cz;
    Obj_GetXYZ1c(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, (uint32_t*)&cx, (uint32_t*)&cy,
                 (uint32_t*)&cz);
    double dx = (double)x - cx;
    double dz = (double)z - cz;
    double d = Fsqrt(dz * dz + dx * dx + (double)cy * cy);
    const float lo = *(const float*)0x5c85b4;
    if ((double)lo > d) d = lo;
    double q = (double)*(const float*)0x5c87c0 / d;
    *out = (float)((Fsqrt(q) + q) * *(const double*)0x5dd2f0);
}
RT3_REPLACE(0x537170, Camera_DetailFactor);

// 0x538640. Handle table constructor: 1000 slots (in-use bytes +0x20, ids +0x408).
uint8_t* __fastcall HandleTable_Construct(uint8_t* t) {
    for (int k = 0; k < 0xfa; ++k) F<int32_t>(t, 0x20 + k * 4) = 0;
    t[0xc68] = 0;
    Collection* c = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    F<Collection*>(t, 0xc69) = c ? Collection_Construct(c, 0, 1, 4, 10, 5, 0, 0, 0) : nullptr;
    static const uint16_t zero[] = { 0xc6d, 0xc71, 0xc75, 0x0, 0x4, 0x8, 0x10, 0xc, 0x14, 0x18, 0x1c, 0xc79, 0xc58, 0xc60, 0xc64 };
    for (uint16_t off : zero) F<int32_t>(t, off) = 0;
    F<uint32_t>(t, 0xc5c) = 0x3e800000;
    return t;
}
RT3_REPLACE(0x538640, HandleTable_Construct);

// 0x5387a0. Shows a status message (+0x10) for 10 s (+8), guarded against re-entry.
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text) {
    if (hook::Global<int32_t>(0xccba1c)) return;
    hook::Global<int32_t>(0xccba1c) = 1;
    F<int32_t>(s, 8) = ClockMs() + 10000;
    if (F<void*>(s, 0x10)) FreeAndNull((void**)(s + 0x10));
    size_t n = 0;
    while (text[n]) ++n;
    if (n) String_Assign((char**)(s + 0x10), text);
    hook::Global<int32_t>(0xccba1c)--;
}
RT3_REPLACE(0x5387a0, Status_SetMessage);

// 0x538840 / 0x538880 / 0x538890. Handles are slot + 60000.
void __fastcall HandleTable_ReleaseId(uint8_t* t, int, int32_t id) {
    for (int32_t i = 0; i < 1000; ++i)
        if ((int32_t)F<uint16_t>(t, 0x408 + i * 2) == id && t[0x20 + i]) t[0x20 + i] = 0;
}
RT3_REPLACE(0x538840, HandleTable_ReleaseId);
void __fastcall HandleTable_Release(uint8_t* t, int, int32_t h) { t[h - 0xea40] = 0; }
RT3_REPLACE(0x538880, HandleTable_Release);
RT3_NOINLINE int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id) {
    for (int32_t i = 0; i < 1000; ++i)
        if (!t[0x20 + i]) {
            t[0x20 + i] = 1;
            F<uint16_t>(t, 0x408 + i * 2) = id;
            return i + 60000;
        }
    return 0;
}
RT3_REPLACE(0x538890, HandleTable_Acquire);

// 0x538990. Calls vtbl[0](arg) along the chain (+0, next +0x58) until one returns 0.
int32_t __fastcall Chain_ForEach(uint8_t* o, int, uint32_t arg) {
    uint8_t* n = F<uint8_t*>(o, 0);
    int32_t r = 1;
    while (n) {
        uint8_t* next = F<uint8_t*>(n, 0x58);
        r = ((int32_t(__thiscall*)(void*, uint32_t))(*(void***)n)[0])(n, arg);
        if (!r) break;
        n = next;
    }
    return r;
}
RT3_REPLACE(0x538990, Chain_ForEach);

float __fastcall Obj_Getc5c(uint8_t* o) { return F<float>(o, 0xc5c); }
RT3_REPLACE(0x538a10, Obj_Getc5c);
void __fastcall Obj_Setc5c(uint8_t* o, int, uint32_t v) { F<uint32_t>(o, 0xc5c) = v; }
RT3_REPLACE(0x538a20, Obj_Setc5c);
void __fastcall Obj_Incc60(uint8_t* o) { F<int32_t>(o, 0xc60)++; }
RT3_REPLACE(0x538a30, Obj_Incc60);
void __fastcall Obj_Decc60(uint8_t* o) {
    if (--F<int32_t>(o, 0xc60) < 0) F<int32_t>(o, 0xc60) = 0;
}
RT3_REPLACE(0x538a40, Obj_Decc60);

} // namespace rt3
