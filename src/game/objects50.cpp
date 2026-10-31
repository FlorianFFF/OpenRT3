// Functions across blocks 0x40..0x4b (batch 53). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint8_t* __fastcall Obj_RegionAt(void* o);
float* __fastcall Vec3_Normalize(float* v);
int32_t __fastcall Obj_AddMessage(uint8_t* o, int, int32_t a, int32_t b, int32_t c, const char* text);
double __fastcall Obj_Float18(void* o);
void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
void __fastcall Collection_ReleaseAll2(Collection* c);
void __fastcall Nop4a7350(void*);

// 0x40fbe0. Whether this object's coarse cell (4 cells) and building bid's
// region share a key in their key lists (+0xeb count, +0xef 6-byte entries).
int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid) {
    int32_t cx = (int32_t)(Obj_CellX(o) / *(const float*)0x5c8568);
    int32_t cy = (int32_t)(Obj_CellY(o) / *(const float*)0x5c8568);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    uint8_t* cell = F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * cy + cx) * 0x1d7;
    if (F<int32_t>(cell, 0xeb) <= 0) return 0;
    uint8_t* r = Obj_RegionAt(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, bid));
    for (int32_t i = 0; i < F<int32_t>(cell, 0xeb); ++i) {
        uint16_t k = F<uint16_t>(F<uint8_t*>(cell, 0xef), i * 6);
        for (int32_t j = 0; j < F<int32_t>(r, 0xeb); ++j)
            if (k == F<uint16_t>(F<uint8_t*>(r, 0xef), j * 6)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x40fbe0, Obj_SharesRegionKey);

// 0x457fb0. Bending offset: (+0x2ad, +0x2b1, +0x2b5) scaled by -(+0x27d)
// and by (2 cos((a.b) pi/2))^2 + (+0x289) for unit a, b.
float* __fastcall Obj_BendOffset(uint8_t* o, int, float* out, float ax, float ay, float az, float bx, float by, float bz) {
    float a[3] = { ax, ay, az }, b[3] = { bx, by, bz };
    Vec3_Normalize(a);
    Vec3_Normalize(b);
    double dot = (double)a[2] * b[2] + (double)a[1] * b[1];
    dot = dot + (double)a[0] * b[0];
    float t = (float)(dot * *(const float*)0x5c8764);
    double c = ((double(__cdecl*)(double))0x5a13e4)(t);
    float k = (float)(c + c);
    float s = (float)-(double)F<float>(o, 0x27d);
    float l0 = (float)((double)s * F<float>(o, 0x2b5));
    float l1 = (float)((double)s * F<float>(o, 0x2b1));
    float l2 = (float)((double)s * F<float>(o, 0x2ad));
    float m = (float)((double)k * k + F<float>(o, 0x289));
    out[2] = (float)((double)l0 * m);
    out[1] = (float)((double)l1 * m);
    out[0] = (float)((double)l2 * m);
    return out;
}
RT3_REPLACE(0x457fb0, Obj_BendOffset);

// 0x473a60. Registers the ten multiplayer sync-check categories (names
// 0x5ce40c.., checksum callbacks) with the lobby.
void RegisterSyncChecks() {
    static const uint32_t kChecks[10][2] = {
        { 0x5ce40c, 0x46de80 }, { 0x5ce404, 0x46e030 }, { 0x5ce3fc, 0x46e250 }, { 0x5ce3f0, 0x46de40 },
        { 0x5ce3e8, 0x46dd10 }, { 0x5ce3e0, 0x46dbd0 }, { 0x5ce3d4, 0x46db90 }, { 0x5ce3cc, 0x46dac0 },
        { 0x5ce3bc, 0x46db50 }, { 0x5ce3ac, 0x46db10 },
    };
    uint8_t* lobby = hook::Global<uint8_t*>(0x6cd8d8);
    for (int i = 0; i < 10; ++i) Obj_AddMessage(lobby, 0, i + 1, 0, (int32_t)kChecks[i][1], (const char*)kChecks[i][0]);
}
RT3_REPLACE(0x473a60, RegisterSyncChecks);

// 0x4933c0. Updates the camera zoom flags (+0xb0 / +0xb4, hysteresis around
// 2000) for the active camera actions and stores the camera value +0x18.
void __fastcall Camera_UpdateZoomFlags(uint8_t* o) {
    float cam66 = F<float>(hook::Global<uint8_t*>(0x62be68), 0x66);
    const float h = *(const float*)0x5c88e0;
    if (F<int32_t>(o, 0xa0) || F<int32_t>(o, 0xa4)) {
        float k = F<int32_t>(o, 0xb0) ? 1.05f : 0.95f;
        F<int32_t>(o, 0xb0) = (double)k * h > cam66 ? 1 : 0;
    }
    if (F<int32_t>(o, 0xa8) || F<int32_t>(o, 0xac)) {
        cam66 = F<float>(hook::Global<uint8_t*>(0x62be68), 0x66);
        float k = F<int32_t>(o, 0xb4) ? 1.1f : 0.9f;
        F<int32_t>(o, 0xb4) = (double)k * h > cam66 ? 1 : 0;
    }
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::Global<void*>(0x62be68));
}
RT3_REPLACE(0x4933c0, Camera_UpdateZoomFlags);

// 0x4a4c00. Whether two track pieces' routes (+0x212) can be merged: both
// open (+0x1d) with equal region type, owner and flag, and neither piece
// has +0x20e / +0x222 links.
int32_t __fastcall Routes_CanMerge(Collection* routes, int, uint8_t* a, uint8_t* b) {
    if (F<int32_t>(routes, 0x90)) return 0;
    if (!Collection_IsValid(routes, 0, F<uint32_t>(a, 0x212))) return 0;
    if (!Collection_IsValid(routes, 0, F<uint32_t>(b, 0x212))) return 0;
    uint8_t* ra = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(a, 0x212));
    uint8_t* rb = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(b, 0x212));
    if (!ra || !rb || !ra[0x1d] || !rb[0x1d]) return 0;
    if (F<int32_t>(rb, 0x18) != F<int32_t>(ra, 0x18) || F<int32_t>(rb, 0x1e) != F<int32_t>(ra, 0x1e)) return 0;
    if (rb[0x1c] != ra[0x1c]) return 0;
    if (F<int32_t>(a, 0x20e) != -1 || F<int32_t>(b, 0x20e) != -1) return 0;
    if (F<int32_t>(a, 0x222) != -1 || F<int32_t>(b, 0x222) != -1) return 0;
    return 1;
}
RT3_REPLACE(0x4a4c00, Routes_CanMerge);

// 0x4a51a0. Recomputes route rid's end (+8) and length (+0x10/+0x14) by
// walking its pieces from the start piece (+4), unless that start lies inside.
void __fastcall Routes_Recount(Collection* routes, int, int32_t rid) {
    if (!Collection_IsValid(routes, 0, rid)) return;
    uint8_t* r = (uint8_t*)Collection_Get(routes, 0, rid);
    uint32_t start = F<uint32_t>(r, 4);
    int32_t count = 1;
    uint8_t* cur = (uint8_t*)Collection_Get(g_trackNodes, 0, start);
    uint8_t* prev;
    uint8_t* e206 = (uint8_t*)Obj_Node206(cur);
    if (e206 && F<int32_t>(Obj_Node206(cur), 0x212) == rid) {
        uint8_t* e20a = (uint8_t*)Obj_Node20a(cur);
        if (e20a && F<int32_t>(Obj_Node20a(cur), 0x212) == rid) return;
        prev = (uint8_t*)Obj_Node20a(cur);
    } else {
        prev = (uint8_t*)Obj_Node206(cur);
    }
    for (;;) {
        uint8_t* next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        prev = cur;
        if (!next || F<int32_t>(next, 0x212) != rid) break;
        ++count;
        cur = next;
    }
    F<uint32_t>(r, 4) = start;
    F<int32_t>(r, 8) = F<int32_t>(cur, 0x202);
    F<int32_t>(r, 0x10) = count;
    F<int32_t>(r, 0x14) = count;
}
RT3_REPLACE(0x4a51a0, Routes_Recount);

void __fastcall Collection5d00c8_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5d00c8;
    Collection_ReleaseAll2(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x4b23b0, Collection5d00c8_Destruct);

void __fastcall Table_Hook4a7350(Collection* t) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1cf)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop4a7350(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x4b2a90, Table_Hook4a7350);

// 0x4b2b30. Initialises an order list header.
void __fastcall OrderList_Init(uint8_t* o) {
    F<int32_t>(o, 0) = 0;
    F<int32_t>(o, 4) = 0;
    F<int32_t>(o, 8) = -1;
    F<int32_t>(o, 0xd) = 0;
    o[0xc] = 0;
    for (int k = 0; k < 5; ++k) F<int32_t>(o, 0x11 + k * 4) = 0;
}
RT3_REPLACE(0x4b2b30, OrderList_Init);

} // namespace rt3
