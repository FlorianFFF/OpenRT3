// Block 0x42xxxx, functions (batch 23). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_companies   = hook::Global<Collection*>(0x62be10);

void* __fastcall Obj_Ref3cc(void* o);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);

static inline void* Virtual80(void* o) { return ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o); }
static inline int32_t Virtual14(void* o) { return ((int32_t(__thiscall*)(void*))(*(void***)o)[0x14 / 4])(o); }

// 0x422900. Counts the buildings of the chain +0x383 (only those without
// +0x29e unless `all`) by type kind 0..3, with their capacities; any output
// may be null. Returns the total count.
int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3,
                                     int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3, int32_t all) {
    int32_t total = 0, n[4] = {}, c[4] = {};
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        if (!all && F<int32_t>(b, 0x29e)) continue;
        int32_t cap = F<int32_t>(Obj_Ref3cc(b), 0x141);
        ++total;
        uint32_t kind = F<uint8_t>(Virtual80(b), 0x32);
        if (kind > 3) continue;
        ++n[kind];
        c[kind] += cap;
    }
    if (n0) *n0 = n[0];
    if (n1) *n1 = n[1];
    if (n2) *n2 = n[2];
    if (n3) *n3 = n[3];
    if (c0) *c0 = c[0];
    if (c1) *c1 = c[1];
    if (c2) *c2 = c[2];
    if (c3) *c3 = c[3];
    return total;
}
RT3_REPLACE(0x422900, Obj_BuildingStats);

// Free company slots: 0x24 fixed slots plus one per entry of the name list
// (*0x6d4020)+0x429b0; a company takes its slot +0x37, or else the list
// entry matching its name +0xd65.
static void FreeCompanySlots(uint8_t freeSlot[0x88]) {
    for (int i = 0; i < 0x88; ++i) freeSlot[i] = i < 0x24 ? 1 : 0;
    auto names = []() { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0); };
    for (int32_t i = 0; i < Collection_Count(names()); ++i) freeSlot[0x24 + i] = 1;
    for (int32_t k = 0; k < Collection_Count(g_companies); ++k) {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, Collection_IdAt(g_companies, 0, k, 0));
        int32_t slot = F<int32_t>(c, 0x37);
        if (slot != -1) {
            freeSlot[slot] = 0;
            continue;
        }
        for (int32_t j = 0; j < Collection_Count(names()); ++j) {
            Collection* l = names();
            if (Stricmp((const char*)c + 0xd65, (const char*)Collection_Get(l, 0, Collection_IdAt(l, 0, j, 0))) == 0)
                freeSlot[0x24 + j] = 0;
        }
    }
}

// 0x4242a0 / 0x424430. Number of free company slots / index of the n-th (0 if none).
int32_t CountFreeCompanySlots() {
    uint8_t freeSlot[0x88];
    FreeCompanySlots(freeSlot);
    int32_t n = 0;
    for (int i = 0; i < 0x88; ++i)
        if (freeSlot[i]) ++n;
    return n;
}
RT3_REPLACE(0x4242a0, CountFreeCompanySlots);
int32_t __fastcall NthFreeCompanySlot(int32_t n) {
    uint8_t freeSlot[0x88];
    FreeCompanySlots(freeSlot);
    for (int32_t i = 0; i < 0x88; ++i) {
        if (freeSlot[i]) --n;
        if (n == 0) return i;
    }
    return 0;
}
RT3_REPLACE(0x424430, NthFreeCompanySlot);

// 0x426c20. Adds (or removes) a track piece to the company's counters by
// kind +0x218 (+0x7664/+0x766c/+0x7668) and class +0x226 (+0x7674/+0x7678,
// or +0x767c if virtual slot 5), and to the same counters of its region's
// per-region block (+0x1dc0.., stride 0x38d).
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    auto count = [&](uint8_t* b, uint32_t k1, uint32_t k2, uint32_t k0, uint32_t c3, uint32_t c1, uint32_t cv) {
        uint8_t kind = t[0x218];
        if (kind == 1) F<int32_t>(b, k1) += d;
        else if (kind == 2) F<int32_t>(b, k2) += d;
        else F<int32_t>(b, k0) += d;
        int32_t cls = F<int32_t>(t, 0x226);
        if (cls == 3) F<int32_t>(b, c3) += d;
        else if (cls == 1) F<int32_t>(b, c1) += d;
        else if (Virtual14(t)) F<int32_t>(b, cv) += d;
    };
    count(c, 0x7664, 0x766c, 0x7668, 0x7674, 0x7678, 0x767c);
    if (!hook::Global<void*>(0x6cfc9c)) return;
    int32_t y = (int32_t)Obj_CenterCellY(t);
    int32_t x = (int32_t)Obj_CenterCellX(t);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    uint32_t region = F<uint8_t*>(map, 0x2131)[F<int32_t>(map, 0x2155) * y + x] >> 3;
    count(c + region * 0x38d, 0x1dc0, 0x1dc8, 0x1dc4, 0x1dd0, 0x1dd4, 0x1dd8);
}
RT3_REPLACE(0x426c20, Company_CountTrack);

} // namespace rt3
