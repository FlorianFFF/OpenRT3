// Block 0x42xxxx, small functions (batch 18). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static uint8_t*& g_world          = hook::Global<uint8_t*>(0x6cec78);
static Collection*& g_players     = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_trains      = hook::Global<Collection*>(0x6cfcbc);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_companies   = hook::Global<Collection*>(0x62be10);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);

static inline double D(uint32_t va) { return *(const double*)va; }

void __fastcall Obj_UpdateAgeFactor(void* o);
double __fastcall Obj_Rounded29(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall Obj_BuildingValue(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);

// 0x420650. Starts the age clock (+0x317 = this year) if unset, then updates the factor.
void __fastcall Obj_StartAgeClock(void* o) {
    if (F<int32_t>(o, 0x317)) return;
    F<int32_t>(o, 0x317) = F<uint16_t>(g_world, 0xd);
    Obj_UpdateAgeFactor(o);
}
RT3_REPLACE(0x420650, Obj_StartAgeClock);

// 0x424180. Clears the 11 x 12 grid of pairs at +0xdd7 (row stride 0x58)
// and the 12 pairs at +0x1c4f.
void __fastcall Obj_ClearGrids(void* o) {
    for (int32_t j = 0; j < 11; ++j) {
        for (int32_t i = 0; i < 12; ++i) {
            F<int32_t>(o, 0xdd7 + j * 8 + i * 0x58) = 0;
            F<int32_t>(o, 0xdd7 + j * 8 + i * 0x58 + 4) = 0;
            F<int32_t>(o, 0x1c4f + i * 8) = 0;
            F<int32_t>(o, 0x1c4f + i * 8 + 4) = 0;
        }
    }
}
RT3_REPLACE(0x424180, Obj_ClearGrids);

void __fastcall Obj_Clear1c4f(void* o) {
    for (int32_t i = 0; i < 0x18; ++i) F<int32_t>(o, 0x1c4f + i * 4) = 0;
}
RT3_REPLACE(0x4241c0, Obj_Clear1c4f);

// 0x4241e0 / 0x424200. Sum of the doubles +0x12a7 and +0x124f (as float), and that scaled.
float __fastcall Obj_Sum12a7(void* o) { return (float)(F<double>(o, 0x12a7) + F<double>(o, 0x124f)); }
RT3_REPLACE(0x4241e0, Obj_Sum12a7);
float __fastcall Obj_Sum12a7Scaled(void* o) {
    float s = (float)(F<double>(o, 0x12a7) + F<double>(o, 0x124f));
    return (float)((double)s * D(0x5c9b48));
}
RT3_REPLACE(0x424200, Obj_Sum12a7Scaled);

// 0x424580. World value (0x433740) plus the 0x39 cargo bonus for this company/player, scaled.
float __fastcall Obj_ValueWithBonus(uint32_t* o) {
    float base = (float)Obj_Rounded29(g_world);
    int32_t bonus = Obj_CargoBonus(g_world, 0, 0x39, o[0], F<uint32_t>(o, 0x3b), 0);
    return (float)((double)bonus * D(0x5c85b8) + base);
}
RT3_REPLACE(0x424580, Obj_ValueWithBonus);

// 0x426150. Minus the sum of the negative player entries +0x15d[*o].
int32_t __fastcall Company_PlayerDebt(int32_t* o) {
    int32_t n = Table_CountField4(g_players);
    int32_t sum = 0;
    for (int32_t i = 0; i < n; ++i) {
        int32_t v = F<int32_t>(Table_NthField4(g_players, 0, i), 0x15d + o[0] * 4);
        sum += v < 0 ? v : 0;
    }
    return -sum;
}
RT3_REPLACE(0x426150, Company_PlayerDebt);

// 0x426200. Sum of the positive entries +0x15d[*o] of active human players.
RT3_NOINLINE int32_t __fastcall Company_HumanHoldings(int32_t* o) {
    int32_t n = Table_CountField4(g_players);
    int32_t sum = 0;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, i);
        if (!F<int32_t>(p, 4)) continue;
        int32_t v = F<int32_t>(p, 0x15d + o[0] * 4);
        if (v > 0 && F<int32_t>(p, 0x1e1)) sum += v;
    }
    return sum;
}
RT3_REPLACE(0x426200, Company_HumanHoldings);

// 0x426520. ID of the n-th (1-based) train owned by this company; 0 if none.
uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n) {
    int32_t count = Collection_Count(g_trains);
    for (int32_t i = 0; i < count; ++i) {
        if (F<int32_t>(Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 0)), 0x51) != o[0]) continue;
        if (--n == 0) return Collection_IdAt(g_trains, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426520, Company_NthTrain);

// 0x426620. ID of the n-th 0x6cec20 record of this company whose building
// is flagged (and, if `activeOnly`, active); 0 if none.
uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly) {
    int32_t count = Collection_Count(g_table6cec20);
    for (int32_t i = 0; i < count; ++i) {
        void* r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != o[0]) continue;
        if (Obj_Flag3d4(Obj_Building(r)) != 1) continue;
        if (activeOnly && !Obj_BuildingStateActive(r)) continue;
        if (--n == 0) return Collection_IdAt(g_table6cec20, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426620, Company_NthSite);

// Owned (+0x276), flagged building whose virtual slot 28 is 0.
static inline int OwnedIdleBuilding(int32_t* o, uint8_t* b) {
    if (F<int32_t>(b, 0x276) != o[0]) return 0;
    if (Obj_Flag3d4(b) != 1) return 0;
    return ((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b) == 0;
}

// 0x426740. ID of the n-th such building; served from the cache 0x62baf0
// (filled by 0x4267f0) while 0x62bae8 is set and n is 1..400.
uint32_t __fastcall Company_NthBuilding(int32_t* o, int, int32_t n) {
    if (n - 1 < 400 && hook::Global<uint8_t>(0x62bae8)) return ((const uint16_t*)0x62baee)[n];
    int32_t count = Collection_Count(g_buildings);
    for (int32_t i = 0; i < count; ++i) {
        if (!OwnedIdleBuilding(o, (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0)))) continue;
        if (--n == 0) return Collection_IdAt(g_buildings, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426740, Company_NthBuilding);

// 0x4267f0. Fills the cache 0x62baf0..0x62be10 (u16 IDs) used by 0x426740.
void __fastcall Company_CacheBuildings(int32_t* o) {
    int32_t count = Collection_Count(g_buildings);
    Collection_PositionOf(g_companies, 0, o[0], 0); // result unused in the original
    uint16_t* p = (uint16_t*)0x62baf0;
    for (int32_t i = 0; i < count; ++i) {
        if (!OwnedIdleBuilding(o, (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0)))) continue;
        if (p < (uint16_t*)0x62be10) *p = (uint16_t)Collection_IdAt(g_buildings, 0, i, 0);
        ++p;
    }
}
RT3_REPLACE(0x4267f0, Company_CacheBuildings);

// 0x427380. Company has anything: flag +0x3f, the counters +0x7664..+0x766c, or cargo sites.
RT3_NOINLINE int32_t __fastcall Company_HasAssets(void* c) {
    if (F<int32_t>(c, 0x3f)) return 1;
    if (F<int32_t>(c, 0x766c) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664) > 0) return 1;
    return Company_CountCargoSites(c, 0, 0) > 0 ? 1 : 0;
}
RT3_REPLACE(0x427380, Company_HasAssets);

// 0x429950 / 0x429980. Companies collection (vtbl 0x5c9bf8, inline 0x7684-byte records, 5 + 5, list 1).
Collection* __fastcall Companies_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9bf8;
    Collection_Init(c, 0, 1, 0x7684, 5, 5, 1, 0, 0);
    return c;
}
RT3_REPLACE(0x429950, Companies_Construct);
void __fastcall Companies_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9bf8;
    Collection_Destruct(c);
}
RT3_REPLACE(0x429980, Companies_Destruct);

// 0x429b90. Save framed by 0x61a9/0x61aa/0x61ab; per record the (empty) hook 0x424000.
void __fastcall Companies_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((int32_t(__thiscall*)(void*, Stream))0x424000)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x429b90, Companies_Save);

} // namespace rt3
