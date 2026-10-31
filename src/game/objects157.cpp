// Save list constructor / speed scale from engine types / camera ground clamp / render caps remap in block 0x43 (batch 161). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall SaveList_Scan(uint8_t* list, int, int32_t kind);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
float __fastcall Camera_TargetGround(uint8_t* c);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
int32_t __fastcall Buckets_Find(int32_t key, int32_t* value, uint32_t* flags);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline float Bits(uint32_t u) { return *(float*)&u; }

// 0x4336a0. Constructs a save list of the given kind and scans the save folder.
uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind) {
    F<int32_t>(list, 4) = kind;
    F<void*>(list, 8) = nullptr;
    F<int32_t>(list, 0) = 0;
    SaveList_Scan(list, 0, kind);
    return list;
}
RT3_REPLACE(0x4336a0, SaveList_Init);

// 0x436af0. Sets the speed scale (+0x4cbe) from the top speed of the engine types
// the player can buy (at least 100, 200 after 2000, 300 after 2010): k / speed,
// clamped to 50..100.
void __fastcall World_UpdateSpeedScale(uint8_t* o) {
    uint16_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    float v = 100.0f;
    if (year > 0x7d0) v = 200.0f;
    if (year > 0x7da) v = 300.0f;
    if (hook::Global<Collection*>(0x6ada84)) {
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) {
            Collection* c = hook::Global<Collection*>(0x6ada84);
            uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            uint8_t* ct = F<uint32_t>(e, 0x72)
                ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72)) : nullptr;
            bool ok = CarType_Available(ct, 0, 0, 0) != 0;
            if (!hook::Global<int32_t>(0x6cd8d8)) {
                uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
                if (CarType_Available(ct, 0, F<uint32_t>(w, 0x21), F<uint32_t>(w, 0x25))) ok = true;
            }
            if (ok && !(v > F<float>(e, 0x20))) v = F<float>(e, 0x20);
        }
    }
    if (Fl(0x5c85b4) > v) v = 1.0f;
    v = (float)(D(0x5c9f40) / v * D(0x5c85d8));
    if (Fl(0x5c8780) > v) v = 50.0f;
    if (Fl(0x5c8c28) < v) v = 100.0f;
    F<float>(o, 0x4cbe) = v;
}
RT3_REPLACE(0x436af0, World_UpdateSpeedScale);

// 0x439820. Keeps the camera (+0x1c/+0x20/+0x24) above the ground: the highest of
// five terrain samples around it plus a margin, eased in by +0x34e unless option
// +0x1146d8; in free mode (+0x60) the ease restarts. Also capped by the view limit
// (vtable +0xc).
void __fastcall Camera_ClampToGround(uint8_t* c) {
    uint8_t* r = F<uint8_t*>(c, 0x6e);
    float x = F<float>(c, 0x1c), z = F<float>(c, 0x24);
    float h0 = (float)Terrain_HeightAt(r, 0, x, z);
    float h1 = (float)Terrain_HeightAt(r, 0, (float)(x + D(0x5ca020)), z);
    float h2 = (float)Terrain_HeightAt(r, 0, (float)(x - D(0x5ca020)), z);
    float h3 = (float)Terrain_HeightAt(r, 0, x, (float)(z + D(0x5ca020)));
    float h4 = (float)Terrain_HeightAt(r, 0, x, (float)(z - D(0x5ca020)));
    float m = h0;
    if (!(m > h1)) m = h1;
    if (!(m > h2)) m = h2;
    if (!(m > h3)) m = h3;
    if (!(m > h4)) m = h4;
    m = (float)(m + Fl(0x5c85f8));
    float g = (float)(Camera_TargetGround(c) + Fl(0x5ca01c));
    if (!(g > m)) g = m;
    float t = F<float>(c, 0x20);
    if (F<int32_t>(c, 0x60)) {
        if (t < m) t = (float)((double)m - t + t);
        F<uint32_t>(c, 0x34a) = F<uint32_t>(c, 8);
        F<uint32_t>(c, 0x34e) = 0;
    } else {
        float g2 = (float)(((double)g - m) * F<float>(c, 0x34e) + m);
        if (hook::Global<uint8_t*>(0x6d4024)[0x1146d8]) g2 = m;
        if (t < g2) {
            float d = (float)((double)g2 - t);
            float s = (float)((double)d * F<float>(c, 0x34e) + F<float>(c, 8));
            Camera_SetEye(c, 0, F<uint32_t>(c, 4), *(uint32_t*)&s, F<uint32_t>(c, 0xc));
            t = (float)((double)d + t);
        }
    }
    typedef int32_t(__thiscall * LimitFn)(void*);
    float lim = (float)(*(LimitFn*)(*(uint8_t**)c + 0xc))(c);
    if (t > lim) t = (float)(*(LimitFn*)(*(uint8_t**)c + 0xc))(c);
    if (!(t == F<float>(c, 0x20))) Camera_SetTarget(c, 0, F<float>(c, 0x1c), t, F<float>(c, 0x24));
}
RT3_REPLACE(0x439820, Camera_ClampToGround);

// 0x439e40. Reads the eight render capability pairs (keys 0x43d2a0..0x43d310, value
// +0xa6, flags +0xaa, 8 bytes each) and remaps the flags 2 -> 0x10, 0x20 -> 0x12,
// 4 -> 0x11.
void __fastcall Caps_Load(uint8_t* o) {
    for (uint32_t off = 0xa6; off < 0xe6; off += 4) F<int32_t>(o, off) = 0;
    for (int32_t i = 0; i < 8; ++i)
        Buckets_Find(0x43d2a0 + i * 0x10, &F<int32_t>(o, 0xa6 + i * 8), &F<uint32_t>(o, 0xaa + i * 8));
    static const uint32_t from[] = { 2, 0x20, 4 }, to[] = { 0x10, 0x12, 0x11 };
    uint32_t v[8];
    for (int32_t i = 0; i < 8; ++i) v[i] = F<uint32_t>(o, 0xaa + i * 8);
    for (int32_t k = 0; k < 3; ++k)
        for (int32_t i = 0; i < 8; ++i)
            if (v[i] == from[k]) v[i] = to[k];
    for (int32_t i = 0; i < 8; ++i) F<uint32_t>(o, 0xaa + i * 8) = v[i];
}
RT3_REPLACE(0x439e40, Caps_Load);

} // namespace rt3
