// Functions across blocks 0x40..0x48 (batch 47). See objects.cpp for conventions.
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);

int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall DistanceSqInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
uint8_t* __fastcall Obj_Region(void* o);
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value);
void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);
void* __fastcall ReturnThis455290(void* o);
void __fastcall Nop4552a0(void*);
void __fastcall Entry21a_Init(uint8_t* e, int, int32_t a, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall Load_Unlink(uint32_t* o);

static inline double ExeSqrt(double v) { return ((double(__cdecl*)(double))0x5a1634)(v); }

// 0x405920. Distance (cells) from (x, y) to the company's nearest site; at least 1.
float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y) {
    int32_t best = 999999999;
    int32_t n = Collection_Count(g_table6cec20);
    for (int32_t i = 0; i < n; ++i) {
        void* r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != c[0]) continue;
        int32_t sy = (int32_t)Obj_BuildingCellY(r);
        int32_t sx = (int32_t)Obj_BuildingCellX(r);
        int32_t d = DistanceSqInt(x, y, sx, sy);
        if (d < best) best = d;
    }
    if (n > 0 && best < 1) best = 1;
    return (float)ExeSqrt((double)best);
}
RT3_REPLACE(0x405920, Company_NearestSiteDistance);

// 0x40db30. Registers this building (+0x2a4) with the value of every site in its region's key list.
void __fastcall Building_RegisterWithSites(uint8_t* b) {
    uint8_t* r = Obj_Region(b);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) {
        uint8_t* e = F<uint8_t*>(r, 0xef) + i * 6;
        uint8_t* site = (uint8_t*)Collection_Get(g_table6cec20, 0, F<uint16_t>(e, 0));
        e = F<uint8_t*>(r, 0xef) + i * 6;
        Obj_AddKeyUnique(site, 0, F<uint16_t>(b, 0x2a4), F<int32_t>(e, 2));
    }
}
RT3_REPLACE(0x40db30, Building_RegisterWithSites);

// 0x42c530. Adds every building of the chain +0xd6 (next +0x2a2) to target's key list with `value`.
void __fastcall Chain_RegisterBuildings(void* o, int, uint8_t* target, int32_t value) {
    for (uint16_t id = F<uint16_t>(o, 0xd6); id; id = F<uint16_t>(Collection_Get(g_buildings, 0, id), 0x2a2))
        Obj_AddKeyUnique(target, 0, id, value);
}
RT3_REPLACE(0x42c530, Chain_RegisterBuildings);

// 0x434130. Era factor +0x4ca2: 0.33 + 0.67 (1 - (year - 1850) / 150), clamped to [0.33, 1].
static void __fastcall World_UpdateEraFactor_Impl(uint8_t* w) {
    double t = (double)(F<uint16_t>(w, 0xd) - 0x73a) / *(const double*)0x5c9680;
    float f = (float)((*(const double*)0x5c8770 - t) * *(const double*)0x5c9ef8 + *(const double*)0x5c8d80);
    F<float>(w, 0x4ca2) = f;
    double d = (double)f < *(const double*)0x5c8d80 ? 0.33 : (double)f;
    float f2 = (float)d;
    F<float>(w, 0x4ca2) = f2;
    if ((double)f2 > *(const double*)0x5c8770) F<float>(w, 0x4ca2) = 1.0f;
}
// 0x4371a8 keeps using ECX (the world) after the call.
__declspec(naked) void World_UpdateEraFactor() { RT3_KEEP_ECX_EDX(World_UpdateEraFactor_Impl, 0) }
RT3_REPLACE(0x434130, World_UpdateEraFactor);

// 0x4554e0. Adds a chat/log entry (0x4552b0), first dropping the oldest while
// more than 50; entries more than 6 serials old are flagged (+0xc).
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    while (Collection_Count(t) > 0x32) {
        uint32_t id = F<uint32_t>(Table_Oldest(t, 0, 1), 0);
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
    uint8_t local[0x21c] = {};  // original: uninitialised stack (mostly overwritten by 0x4552b0)
    ReturnThis455290(local);
    uint32_t id = Collection_Allocate(t, 0, local, 0, 0, 0);
    uint8_t* e = (uint8_t*)Collection_Get(t, 0, id);
    Entry21a_Init(e, 0, (int32_t)id, name, b, c, d);
    for (uint8_t* o = (uint8_t*)Table_Oldest(t, 0, 0); F<int32_t>(e, 4) - F<int32_t>(o, 4) > 6;
         o = (uint8_t*)Table_Oldest(t, 0, 0))
        o[0xc] = 1;
    Nop4552a0(local);
    return id;
}
RT3_REPLACE(0x4554e0, Log_Add);

// 0x467f20. Moves a cargo load onto train `id` (front of its list +0x29).
void __fastcall Load_AttachToTrain(uint32_t* load, int, uint32_t id) {
    Load_Unlink(load);
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, id);
    F<uint16_t>(load, 8) = (uint16_t)id;
    load[1] = F<uint32_t>(t, 0x29);
    F<uint32_t>(t, 0x29) = load[0];
}
RT3_REPLACE(0x467f20, Load_AttachToTrain);

// 0x4680b0. Delivers a cargo load: moves 0.2 of its cargo type's amount
// (+0x5a) to delivered (+0x66, flag 1 of +0x12) or +0x62, then unlinks it.
void __fastcall Load_Deliver(uint32_t* load) {
    Collection* types = hook::Global<Collection*>(0x62ba8c);
    if (types) {
        uint8_t* ct = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(types, 0x8e + F<uint8_t>(load, 0x10) * 4));
        const double k = *(const double*)0x5c8c20;
        F<float>(ct, 0x5a) = (float)((double)F<float>(ct, 0x5a) - k);
        if (F<uint8_t>(load, 0x12) & 1) F<float>(ct, 0x66) = (float)((double)F<float>(ct, 0x66) + k);
        else F<float>(ct, 0x62) = (float)((double)F<float>(ct, 0x62) + k);
    }
    Load_Unlink(load);
}
RT3_REPLACE(0x4680b0, Load_Deliver);

// 0x481500. Whether an active site (only siteId, if given) lies within 7 cells of building `bid`.
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId) {
    void* b = Collection_Get(g_buildings, 0, bid);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint32_t id = Collection_IdAt(t, 0, i, 0);
        if (siteId && siteId != id) continue;
        void* r = Collection_Get(t, 0, id);
        if (!Obj_BuildingStateActive(r)) continue;
        float ry = (float)Obj_BuildingCellY(r);
        float rx = (float)Obj_BuildingCellX(r);
        float by = (float)Obj_CellY(b);
        float bx = (float)Obj_CellX(b);
        float d = (float)Distance2D(bx, by, rx, ry);
        if (d <= *(const float*)0x5c8738) return 1;
    }
    return 0;
}
RT3_REPLACE(0x481500, Table_ActiveSiteNear);

} // namespace rt3
