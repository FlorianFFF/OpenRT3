// Cargo lot attach / lot link refresh / region station link test in blocks 0x46..0x48 (batch 177). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Load_Unlink(uint32_t* o);
uint8_t* __fastcall Obj_RegionAt(void* o);
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t);
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d);
uint32_t __fastcall Obj_BuildingRegionIndex(void* o);
uint32_t __fastcall Obj_BuildingMapQuery(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void* __fastcall Obj_Node8(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);

static inline uint8_t* Cargos() { return hook::Global<uint8_t*>(0x62ba8c); }
static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }

// 0x467eb0. Moves the lot to building id (head of its +0x272 lot chain), refreshes
// its link flag and, if linked, raises the building region's demand for its class.
void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id) {
    Load_Unlink(o);
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
    F<uint16_t>(o, 0xa) = (uint16_t)id;
    o[1] = F<uint32_t>(b, 0x272);
    F<uint32_t>(b, 0x272) = o[0];
    Lot_UpdateLinked((uint8_t*)o, 0, 0);
    if (!(((uint8_t*)o)[0x12] & 2)) return;
    int32_t cls = F<int32_t>(Cargos(), 0x8e + ((uint8_t*)o)[0x10] * 4);
    Obj_AddFloat103(Obj_RegionAt(b), 0, cls, 0.2f);
}
RT3_REPLACE(0x467eb0, Lot_AttachTo);

// 0x4682c0. Refreshes every lot's link flag.
void __fastcall Lots_UpdateLinked(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Lot_UpdateLinked((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, 1);
}
RT3_REPLACE(0x4682c0, Lots_UpdateLinked);

// 0x468300. Clears the region demand for the three lot cargo classes (cargo
// collection +0x88/+0x8a/+0x8c) and rebuilds it from the lots' link flags.
void __fastcall Lots_RebuildDemand(Collection* c) {
    uint32_t a = F<uint16_t>(Cargos(), 0x88), b = F<uint16_t>(Cargos(), 0x8a), k = F<uint16_t>(Cargos(), 0x8c);
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    uint8_t* cell = F<uint8_t*>(m, 0x2129);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2145); ++x, cell += 0x1d7) {
            F<int32_t>(cell, 0x103 + a * 4) = 0;
            F<int32_t>(cell, 0x103 + b * 4) = 0;
            F<int32_t>(cell, 0x103 + k * 4) = 0;
            m = hook::Global<uint8_t*>(0x62c120);
        }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* lot = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        lot[0x12] &= 0xfd;
        Lot_UpdateLinked(lot, 0, 1);
    }
}
RT3_REPLACE(0x468300, Lots_RebuildDemand);

static inline bool Usable(uint8_t* s, int32_t company) {
    return Obj_BuildingStateActive(s) && Obj_Flag3d4(Obj_Building(s)) == 1 &&
           (!company || company == Obj_BuildingValue(s));
}

// 0x487a60. Whether a usable station (optionally of the company) in this region
// is track-connected to one in another map area.
int32_t __fastcall Region_StationsLinked(uint32_t* o, int, int32_t company) {
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0));
        if (Obj_BuildingRegionIndex(s) != o[0] || !Usable(s, company)) continue;
        uint32_t area = Obj_BuildingMapQuery(s);
        uint32_t node = F<uint32_t>(s, 8);
        for (int32_t j = 0; j < Collection_Count(Stations()); ++j) {
            uint8_t* s2 = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, j, 0));
            if (Obj_BuildingMapQuery(s2) == area || !Usable(s2, company)) continue;
            if (Node_SameComponent((uint8_t*)Obj_Node8(s2), 0, node)) return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x487a60, Region_StationsLinked);

} // namespace rt3
