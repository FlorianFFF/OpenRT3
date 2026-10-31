// New car type news / car type for a cargo / cargo averages and availability in block 0x41 (batch 155). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

const char* __fastcall GetString(int32_t id);
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d,
                                    int32_t e, int32_t f);
void __fastcall Cargo_UpdateAverages(uint8_t* o);
void __fastcall CargoTypes_Recount(Collection* t);

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x41e010. In 1904 (first year) posts the news 0xcb/0xcc; from 1830 on announces
// the cargo cars (kind 2) introduced this year that the player can use.
void __fastcall CarTypes_AnnounceNew(Collection* c) {
    uint8_t* w = World();
    if (F<int32_t>(w, 0x46c38)) return;
    if (F<uint16_t>(w, 0xd) == 0x770 && F<uint16_t>(w, 5) < 0x770) {
        const char* text = GetString(0xcc);
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, GetString(0xcb), text, 0, 0);
        w = World();
    }
    if (F<uint16_t>(w, 0xd) < 0x726) return;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x105) != F<uint16_t>(World(), 0xd) || F<int32_t>(e, 0x41) != 2) continue;
        int32_t player = Obj_Get25(World());
        if (!CarType_Available(e, 0, Obj_Get21(World()), player)) continue;
        World_AddNamedEntry(World(), 0, (const char*)0x5c87a8, 5, 0, F<int32_t>(e, 0x126), -1, -1, -1);
    }
}
RT3_REPLACE(0x41e010, CarTypes_AnnounceNew);

// 0x41e500. The available freight car type (kind 1) for cargo name +0x25 (troops ride
// passenger cars); else the first freight car type.
uint32_t __fastcall CarType_ForCargo(uint8_t* o) {
    char name[0x12c];
    strcpy(name, (const char*)o + 0x25);
    uint32_t first = 0;
    for (;;) {
        Collection* types = hook::Global<Collection*>(0x62ba88);
        for (int32_t i = 0; i < Collection_Count(types); ++i) {
            uint8_t* e = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, i, 0));
            if (F<int32_t>(e, 0x41) != 1) continue;
            if (!first) first = Collection_IdAt(types, 0, i, 0);
            if (CarType_Available(e, 0, 0, 0) && !strcmp((const char*)e + 0x111, name))
                return Collection_IdAt(types, 0, i, 0);
        }
        if (Stricmp(name, (const char*)0x5c9974)) return first;
        memcpy(name, (const void*)0x5c9968, 10);
    }
}
RT3_REPLACE(0x41e500, CarType_ForCargo);

// 0x41ea50. Updates the cargo averages: all of them, or every sixth by month.
void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all) {
    int32_t phase = World()[0xf] % 6;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (all || i % 6 == phase) Cargo_UpdateAverages((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x41ea50, CargoTypes_UpdateAverages);

// 0x41eac0. Marks which cargos exist in the game (+0x56): those always present
// (+0x47/+0x48), troops with barracks (+0x49), or produced by an active industry
// (recipes without inputs; waste for recycling plants; ports/warehouses only where a
// city links them); all with option +0x25f; only six goods in the scenario mode
// world +0x4afb. Then recounts.
void __fastcall CargoTypes_MarkAvailable(Collection* c) {
    if (!hook::Global<Collection*>(0x62b268)) return;
    uint8_t flags[0x3e8];
    memset(flags, 0, sizeof flags);
    int32_t barracks = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
        Collection* inds = hook::Global<Collection*>(0x62b268);
        uint8_t* ind = (uint8_t*)Collection_Get(inds, 0, Collection_IdAt(inds, 0, i, 0));
        if (ind[0x32] != 2 || !F<int32_t>(ind, 0x7ac)) continue;
        if ((ind[0xba] || ind[0xbb]) && hook::Global<Collection*>(0x62bae0)) {
            int32_t linked = 0;
            for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++j) {
                Collection* cs = hook::Global<Collection*>(0x62bae0);
                uint8_t* e = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, j, 0));
                if (F<int32_t>(e, 0x242) <= 0 || F<int32_t>(e, 0x2f2) != F<int32_t>(ind, 0x794)) continue;
                if ((ind[0xba] && F<int32_t>(e, 0x2fa)) || (ind[0xbb] && F<int32_t>(e, 0x2fe))) linked = 1;
            }
            if (!linked) continue;
        }
        const char* name = (const char*)ind + 4;
        if (!Stricmp((const char*)0x5c9378, name)) barracks = 1;
        if (!Stricmp((const char*)0x5c8de0, name)) {
            for (int32_t j = 0; j < Collection_Count(c); ++j)
                if (!Stricmp((const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0)) + 4, (const char*)0x5c99c0))
                    flags[Collection_IdAt(c, 0, j, 0)] = 1;
            continue;
        }
        uint8_t* r = F<uint8_t*>(ind, 0x37);
        for (int32_t n = F<int32_t>(ind, 0x33); n > 0; --n, r += 0xbc)
            if (!F<int32_t>(r, 0)) flags[F<int32_t>(r, 0x1c)] = 1;
    }
    F<int32_t>(c, 0x31a) = 0;
    static const uint32_t six[] = { 0x5c99b8, 0x5c99b0, 0x5c99a4, 0x5c999c, 0x5c9994, 0x5c9988 };
    for (int32_t j = 0; j < Collection_Count(c); ++j) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0));
        uint32_t id = Collection_IdAt(c, 0, j, 0);
        int32_t avail = (e[0x47] || e[0x48] || (e[0x49] && barracks) || flags[id]) ? 1 : 0;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x25f)) avail = 1;
        if (F<int32_t>(World(), 0x4afb)) {
            bool keep = false;
            for (uint32_t s : six)
                if (!Stricmp((const char*)e + 4, (const char*)s)) {
                    keep = true;
                    break;
                }
            if (!keep) avail = 0;
        }
        F<int32_t>(e, 0x56) = avail;
    }
    CargoTypes_Recount(c);
}
RT3_REPLACE(0x41eac0, CargoTypes_MarkAvailable);

} // namespace rt3
