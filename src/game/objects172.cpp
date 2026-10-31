// City service value / random city growth event / connected city count / lot route link bonus in blocks 0x40..0x46 (batch 176). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <stdlib.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company);
int32_t __fastcall Game_EffectiveVersion();
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d,
                                    int32_t e, int32_t f);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Station_ConnectedTo(uint8_t* s, int, uint32_t id);
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }
static inline Collection* Players() { return hook::Global<Collection*>(0x62be10); }
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

// 0x4010f0. What serving the city is worth to the company (0 for ghost cities +0x23e,
// regions the company may not enter, or cities it already serves from its home
// network): its demand (+0x306/+0x30a/+0x30e) shared among the ready stations in it,
// and cut when another company competes there (v1.03+).
float __fastcall City_ServiceValue(uint8_t* o, int, uint32_t company) {
    if (F<int32_t>(o, 0x23e)) return Fl(0x5c8598);
    float s = (float)((double)F<float>(o, 0x30a) + F<float>(o, 0x306));
    int32_t v = Ftol(((double)F<float>(o, 0x30e) * D(0x5c8590) + s) / D(0x5c8588) * D(0x5c8580));
    if (company) {
        float y = (float)Obj_CellY(o);
        uint32_t region = Map_RegionTypeAt(hook::Global<void*>(0x62c120), 0, (float)Obj_CellX(o), y);
        if (!Company_EntryFlag(Collection_Get(Players(), 0, company), 0, region)) return Fl(0x5c8598);
    }
    int32_t rival = 0, n = 0;
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        if (!Obj_RegionHasKey(o, 0, (uint16_t)Collection_IdAt(Stations(), 0, i, 0))) continue;
        uint8_t* st = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0));
        if (Obj_Flag3d4(Obj_Building(st)) != 1) continue;
        if (company && Station_ConnectedToHome(st, 0, company)) return Fl(0x5c8598);
        ++n;
        if (Game_EffectiveVersion() < 0x67 || !company || company == (uint32_t)Obj_BuildingValue(st)) continue;
        if (!Collection_IsValid(Players(), 0, Obj_BuildingValue(st))) continue;
        if (!Company_OwnerFlag1e1(Collection_Get(Players(), 0, Obj_BuildingValue(st)))) rival = 1;
    }
    if (n) v = Ftol(D(0x5c8578) / ((double)n + D(0x5c8578)) * (double)v);
    if (rival) v = Ftol((double)v * D(0x5c8570));
    return (float)v;
}
RT3_REPLACE(0x4010f0, City_ServiceValue);

static inline bool Unserved(uint8_t* e) {
    return !F<int32_t>(e, 0x23e) && !F<int32_t>(e, 0x276) && !F<int32_t>(e, 0x302) &&
           !Region_HasConnectedStation(e, 0, 1, 1, 0, 0);
}

// 0x422100. Once the game is past its first half year (not sandbox, not +0x4aeb),
// with a chance falling with the distance from 1860 and the map width, a random
// city without stations gets a growth boom (+0x276, 100000 x 1..6 by its size
// +0x25e) and a news item.
void __fastcall Cities_RandomBoom(Collection* c) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (F<int32_t>(w, 0x46c38)) return;
    uint16_t year = F<uint16_t>(w, 0xd);
    if (year < F<uint16_t>(w, 5)) return;
    if (year == F<uint16_t>(w, 5) && w[0xf] <= 6) return;
    if (F<int32_t>(w, 0x4aeb)) return;
    int32_t chance = (0x32 - abs(year - 0x744)) * (0x17318 / F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155)) / 70;
    if (SimRand(0, 10000) >= chance) return;
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (Unserved((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)))) ++n;
    if (n < 1) return;
    int32_t pick = SimRand(1, n);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!Unserved(e) || --pick > 0) continue;
        float size = F<float>(e, 0x25e);
        int32_t r;
        if (size > Fl(0x5c9af8)) r = SimRand(4, 6);
        else if (size > Fl(0x5c8c28)) r = SimRand(3, 5);
        else if (size > Fl(0x5c9af4)) r = SimRand(2, 4);
        else r = SimRand(1, 3);
        int32_t v = r * 100000;
        F<int32_t>(e, 0x276) = v;
        World_AddNamedEntry(hook::Global<void*>(0x6cec78), 0, (const char*)0x5c87a8, 7, 0, F<int32_t>(e, 0x23a), v, -1, -1);
        return;
    }
}
RT3_REPLACE(0x422100, Cities_RandomBoom);

// 0x422850. Number of real cities with an active, ready station track-connected
// to another city, of the given company.
int32_t __fastcall Cities_CountConnected(Collection* c, int, int32_t company) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!F<int32_t>(e, 0x23e) && Region_HasConnectedStation(e, 0, 1, 1, 1, company)) ++n;
    }
    return n;
}
RT3_REPLACE(0x422850, Cities_CountConnected);

// 0x467c30. Recomputes whether a cargo lot's source and destination buildings
// (+0xa/+0xe) have track-connected ready stations (+0x12 bit 1); when that flips,
// the source region's demand for its cargo class moves by +-0.2.
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t) {
    if (F<uint16_t>(o, 8)) return;
    void* b1 = Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(o, 0xa));
    if (!b1) return;
    void* b2 = Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(o, 0xe));
    if (!b2) return;
    int32_t old = (o[0x12] >> 1) & 1;
    o[0x12] &= 0xfd;
    uint8_t* r1 = Obj_Region(b1);
    uint8_t* r2 = Obj_Region(b2);
    for (int32_t i = 0; i < F<int32_t>(r1, 0xeb); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint16_t>(F<uint8_t*>(r1, 0xef), i * 6));
        if (Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        for (int32_t j = 0; j < F<int32_t>(r2, 0xeb); ++j) {
            uint16_t id2 = F<uint16_t>(F<uint8_t*>(r2, 0xef), j * 6);
            if (F<uint16_t>(F<uint8_t*>(r1, 0xef), i * 6) == id2) continue;
            if (Obj_Flag3d4(Obj_Building(Collection_Get(Stations(), 0, id2))) != 1) continue;
            if (Station_ConnectedTo(s, 0, F<uint16_t>(F<uint8_t*>(r2, 0xef), j * 6))) o[0x12] |= 2;
        }
    }
    int32_t now = (o[0x12] >> 1) & 1;
    if (old == now) return;
    float d = (float)(o[0x12] & 2 ? 0.2 : -0.2);
    Obj_AddFloat103(r1, 0, F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x8e + o[0x10] * 4), d);
}
RT3_REPLACE(0x467c30, Lot_UpdateLinked);

} // namespace rt3
