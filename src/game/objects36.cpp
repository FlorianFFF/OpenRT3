// Functions across blocks 0x40..0x48 (batch 39). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);

uint32_t __fastcall Obj_NodeNotify(uint8_t* o, int, int32_t flag);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Obj_RefreshHighlight(void* o);
void __fastcall Nop47d8d0(void*);
void __fastcall Pairs_RemoveKey(uint8_t* o, int, int32_t key);
int32_t __fastcall Obj_BuildingStateActive(void* o);

// 0x40dba0 / 0x40dbf0. With virtual slot 28, notifies the site's node
// (slot 1 / 2) and sets +0x42 to 0 / 1.
static uint32_t NotifySite(uint8_t* o, int32_t flag) {
    uint32_t r = ((uint32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o);
    if (r) {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        uint8_t* site = id ? (uint8_t*)Collection_Get(g_table6cec20, 0, id) : nullptr;
        r = Obj_NodeNotify(site, 0, flag);
    }
    o[0x42] = flag ? 0 : 1;
    return r;
}
uint32_t __fastcall Obj_NotifySiteOn(uint8_t* o) { return NotifySite(o, 1); }
RT3_REPLACE(0x40dba0, Obj_NotifySiteOn);
uint32_t __fastcall Obj_NotifySiteOff(uint8_t* o) { return NotifySite(o, 0); }
RT3_REPLACE(0x40dbf0, Obj_NotifySiteOff);

static inline void* BuildingOf4(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return id ? Collection_Get(g_buildings, 0, id) : nullptr;
}

// 0x47f170. Region index (cell byte >> 3) at the building's cell.
uint32_t __fastcall Obj_BuildingRegionIndex(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    int32_t yi = (int32_t)Obj_CellY(b1);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    return row[(int32_t)x] >> 3;
}
RT3_REPLACE(0x47f170, Obj_BuildingRegionIndex);

// 0x47f1f0. Map query 0x44e270 at the building's position.
uint32_t __fastcall Obj_BuildingMapQuery(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    float y = (float)Obj_CellY(b1);
    return ((uint32_t(__thiscall*)(void*, float, float))0x44e270)(hook::Global<void*>(0x62c120), x, y);
}
RT3_REPLACE(0x47f1f0, Obj_BuildingMapQuery);

// 0x47f290. Sets the highlight byte +0x5bc and refreshes the building when it changes.
void __fastcall Obj_SetHighlight(uint8_t* o, int, int32_t v) {
    if (o[0x5bc] == v) return;
    o[0x5bc] = (uint8_t)v;
    Obj_RefreshHighlight(BuildingOf4(o));
}
RT3_REPLACE(0x47f290, Obj_SetHighlight);

// 0x4811c0 / 0x4811f0. Collection subclass 0x5ce90c (inline 0x60d-byte records, 10 + 20).
Collection* __fastcall Collection5ce90c_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce90c;
    Collection_Init(c, 0, 1, 0x60d, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x4811c0, Collection5ce90c_Construct);
void __fastcall Collection5ce90c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce90c;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x4811f0, Collection5ce90c_Destruct);

// 0x481430. With option +0x1c7, the (empty) per-record hook 0x47d8d0.
void __fastcall Table_Hook47d8d0(Collection* t) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1c7)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop47d8d0(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x481430, Table_Hook47d8d0);

void __fastcall Table_RemovePairKey(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Pairs_RemoveKey((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, key);
}
RT3_REPLACE(0x4814c0, Table_RemovePairKey);

// 0x4818b0. Active sites (highlighted only, when `highlighted`).
int32_t __fastcall Table_CountActiveSites(Collection* t, int, int32_t highlighted) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (!Obj_BuildingStateActive(r)) continue;
        if (!highlighted || r[0x5bc]) ++n;
    }
    return n;
}
RT3_REPLACE(0x4818b0, Table_CountActiveSites);

// 0x4819b0. Moves one reference (+0x5c1) from record `from` to record `to`.
void __fastcall Table_MoveRef(Collection* t, int, uint32_t from, uint32_t to) {
    if (from) {
        if (uint8_t* r = (uint8_t*)Collection_Get(t, 0, from)) F<int32_t>(r, 0x5c1)--;
    }
    if (to) {
        if (uint8_t* r = (uint8_t*)Collection_Get(t, 0, to)) F<int32_t>(r, 0x5c1)++;
    }
}
RT3_REPLACE(0x4819b0, Table_MoveRef);

int32_t __fastcall Return1_481fb0(void*) { return 1; }
RT3_REPLACE(0x481fb0, Return1_481fb0);
int32_t Option1df() { return F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1df); }
RT3_REPLACE(0x482840, Option1df);
int32_t Global6cfc8cSet() { return hook::Global<void*>(0x6cfc8c) ? 1 : 0; }
RT3_REPLACE(0x485750, Global6cfc8cSet);

} // namespace rt3
