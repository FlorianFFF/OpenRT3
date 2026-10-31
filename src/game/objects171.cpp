// Cargo delivery payout / region station connectivity / station track component tests in blocks 0x41..0x48 (batch 175). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t);
void* __fastcall Train_Company(void* t);
void __fastcall Obj_AddRevenue(void* o, int, float a, float b, float c, int32_t flag, int32_t idx);
void __fastcall Train_ShareRevenue(uint8_t* t, int, float revenue, int32_t kind, float units, int32_t cargo);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void* __fastcall Obj_Node8(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
int32_t __fastcall Company_HomeStation(int32_t* c);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }
static inline uint8_t* StationAt(int32_t i) { return (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0)); }
static inline uint8_t* Node(uint32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id); }

// 0x41cbc0. Pays out a delivered load (+0x398 units of cargo +0x38c at rate +0x3a0
// x +0x39c, scaled for passengers on +0x198 trains): shares the revenue,
// books it to the train's company with the train's speed and reliability, adds
// region cargos to the region's stock, and clears the load. Returns the revenue.
float __fastcall Load_PayOut(uint8_t* o) {
    uint8_t* t = F<int32_t>(o, 0x390) != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(o, 0x390)) : nullptr;
    uint8_t* r = Train_FirstCarRegion(t);
    float rev = 0.0f;
    if (F<float>(o, 0x398) > *(const float*)0x5c8598) {
        uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
        rev = (float)((double)F<float>(o, 0x3a0) * F<float>(o, 0x39c));
        int32_t cargo = F<int32_t>(o, 0x38c);
        if (cargo == F<uint16_t>(cargos, 0x88) && t[0x198]) rev = (float)(rev * D(0x5c8698));
        int32_t kind = F<int32_t>(cargos, 0x9a + cargo * 4) != -1;
        // Through the call site: DesyncWatch hooks it (track use income).
        hook::CallTarget<void(__fastcall*)(uint8_t*, int, float, int32_t, float, int32_t)>(0x41cc70)(
            t, 0, rev, kind, F<float>(o, 0x398), cargo);
        float h = (float)(F<float>(t, 0x39) * D(0x5c8628));
        float rel = F<float>(t, 0x3d);
        void* c = Train_Company(t);
        if (c) Obj_AddRevenue(c, 0, F<float>(o, 0x398), (float)((double)h * F<float>(o, 0x398)), rel, kind, F<int32_t>(o, 0x38c));
        int32_t cg = F<int32_t>(o, 0x38c);
        if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cg * 4) == -1)
            F<float>(r, 0x103 + cg * 4) = (float)((double)F<float>(r, 0x103 + cg * 4) + F<float>(o, 0x398));
    }
    F<uint32_t>(o, 0x3a0) = 0x3f800000;
    F<int32_t>(o, 0x39c) = 0;
    F<int32_t>(o, 0x398) = 0;
    F<int32_t>(o, 0x3a4) = 0;
    return rev;
}
RT3_REPLACE(0x41cbc0, Load_PayOut);

// 0x420030. Whether the region has a station (optionally active / with a ready
// building / of the given value) that, with `linked`, is track-connected to a
// station outside the region whose cell is in some (+0x23e clear) city's region.
int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value) {
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        uint8_t* s = StationAt(i);
        if (!Obj_HasKey(Obj_Region(o), 0, (uint16_t)F<int32_t>(s, 0))) continue;
        if (value && Obj_BuildingValue(s) != value) continue;
        if (!Obj_BuildingStateActive(s) && active) continue;
        if (Obj_Flag3d4(Obj_Building(s)) != 1 && ready) continue;
        if (!linked) return 1;
        uint32_t node = F<uint32_t>(s, 8);
        for (int32_t j = 0; j < Collection_Count(Stations()); ++j) {
            uint32_t id = Collection_IdAt(Stations(), 0, j, 0);
            if (Obj_HasKey(Obj_Region(o), 0, (uint16_t)id)) continue;
            if (!Obj_BuildingStateActive(StationAt(j)) && active) continue;
            if (Obj_Flag3d4(Obj_Building(StationAt(j))) != 1 && ready) continue;
            if (!Node_SameComponent((uint8_t*)Obj_Node8(StationAt(j)), 0, node)) continue;
            uint32_t sid = Collection_IdAt(Stations(), 0, j, 0);
            Collection* cities = hook::Global<Collection*>(0x62bae0);
            for (int32_t k = 0; k < Collection_Count(cities); ++k) {
                uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0,
                                                      Collection_IdAt(hook::Global<Collection*>(0x62bae0), 0, k, 0));
                if (!F<int32_t>(c, 0x23e) && Obj_HasKey(Obj_Region(c), 0, (uint16_t)sid)) return 1;
            }
        }
    }
    return 0;
}
RT3_REPLACE(0x420030, Region_HasConnectedStation);

// 0x47f2d0. Whether station id is track-connected to this station.
int32_t __fastcall Station_ConnectedTo(uint8_t* s, int, uint32_t id) {
    uint32_t other = F<uint32_t>(Collection_Get(Stations(), 0, id), 8);
    return Node_SameComponent(Node(F<uint32_t>(s, 8)), 0, other);
}
RT3_REPLACE(0x47f2d0, Station_ConnectedTo);

// 0x4801a0. Whether this active, ready station is track-connected to the company's
// home station.
int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company) {
    if (!Obj_BuildingStateActive(s)) return 0;
    void* b = F<uint32_t>(s, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(s, 4)) : nullptr;
    if (Obj_Flag3d4(b) != 1) return 0;
    int32_t home = Company_HomeStation((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, company));
    if (home == -1) return 0;
    return Node_SameComponent(Node(F<uint32_t>(s, 8)), 0, home);
}
RT3_REPLACE(0x4801a0, Station_ConnectedToHome);

} // namespace rt3
