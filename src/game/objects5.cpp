// More simulation accessors (batch 8). See objects.cpp for conventions.
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_cities     = hook::Global<Collection*>(0x62ba84);
static Collection*& g_table6ada84 = hook::Global<Collection*>(0x6ada84);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
void* __fastcall Obj_Ref388(void* o);
void* __fastcall Obj_Building(void* o);

// 0x412b70. Record named `name`, or NULL.
void* __fastcall Table_GetByName(Collection* t, int, const char* name) {
    uint32_t id = Table_FindByName2(t, 0, name, 0);
    return id ? Collection_Get(t, 0, id) : nullptr;
}
RT3_REPLACE(0x412b70, Table_GetByName);

// 0x48aa30. Region-grid byte at this object's centre.
uint32_t __fastcall Obj_CenterGridByte(void* o) {
    const float twoCells = 184.0f;
    float y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / twoCells);
    float x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / twoCells);
    return Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
}
RT3_REPLACE(0x48aa30, Obj_CenterGridByte);

void __fastcall Node_Set222(void* n, int, int32_t v) { F<int32_t>(n, 0x222) = v; }
RT3_REPLACE(0x48abc0, Node_Set222);

void __fastcall Obj_DecA0(void* o) {
    if (--F<int32_t>(o, 0xa0) < 0) F<int32_t>(o, 0xa0) = 0;
}
RT3_REPLACE(0x491880, Obj_DecA0);

// 0x4a3d00. Pushes {id, a, b, next} onto the list at +0x98 and returns the id:
// the reserved id at +0x88, or else the next one from the counter +0x90.
int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b) {
    int32_t* n = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)(0x10);
    if (F<int32_t>(o, 0x88) == -1) {
        int32_t c = F<int32_t>(o, 0x90);
        F<int32_t>(o, 0x88) = c + 1;
        F<int32_t>(o, 0x90) = c + 2;
    }
    n[0] = F<int32_t>(o, 0x88);
    n[1] = a;
    n[2] = b;
    n[3] = F<int32_t>(o, 0x98);
    F<int32_t*>(o, 0x98) = n;
    F<int32_t>(o, 0x88) = -1;
    return n[0];
}
RT3_REPLACE(0x4a3d00, Obj_PushIdNode);

static int32_t __fastcall Obj_KindE8Special_Impl(void* o) {
    int32_t k = F<int32_t>(o, 0xe8);
    return (k == 0x11 || k == 0xc || k == 0xd) ? 1 : 0;
}
// 0x4aa62f keeps using ECX after the call.
__declspec(naked) int32_t Obj_KindE8Special() { RT3_KEEP_ECX_EDX(Obj_KindE8Special_Impl, 0) }
RT3_REPLACE(0x4a7220, Obj_KindE8Special);

// 0x4a7270. Sum of three float arrays at i: (+0x159 + +0x131) + +0x109.
double __fastcall Obj_Sum3(void* o, int, int32_t i) {
    return ((double)F<float>(o, 0x159 + i * 4) + F<float>(o, 0x131 + i * 4)) + F<float>(o, 0x109 + i * 4);
}
RT3_REPLACE(0x4a7270, Obj_Sum3);

// 0x4a8310. Calls virtual slot 22 (a, b, c) on this object's city and each
// city linked from it via +0x352.
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c) {
    uint8_t* city = (uint8_t*)Collection_Get(g_cities, 0, F<uint32_t>(o, 0x41));
    while (city) {
        ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))(*(void***)city)[0x58 / 4])(city, a, b, c);
        int32_t next = F<int32_t>(city, 0x352);
        if (next == -1) break;
        city = (uint8_t*)Collection_Get(g_cities, 0, next);
    }
}
RT3_REPLACE(0x4a8310, Obj_ForCityChain);

// 0x4a9460. This object's city record (0x6ada84) has +0x10 == 2.
int32_t __fastcall Obj_CityKindIs2(void* o) {
    int32_t v = 0;
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_cities, 0, id) : nullptr;
    if (city) v = F<int32_t>(Collection_Get(g_table6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126)), 0x10);
    return v == 2 ? 1 : 0;
}
RT3_REPLACE(0x4a9460, Obj_CityKindIs2);

// 0x407780. Allocates 32 zeroed 0x1a-byte entries, pointers at +0x5bd.
void __fastcall Obj_AllocEntries(void* o) {
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    void** a = (void**)malloc_(0x80);
    F<void**>(o, 0x5bd) = a;
    for (int i = 0; i < 32; ++i) {
        uint8_t* e = (uint8_t*)malloc_(0x1a);
        F<void**>(o, 0x5bd)[i] = e;
        for (int k = 0; k < 0x1a; ++k) e[k] = 0;
    }
}
RT3_REPLACE(0x407780, Obj_AllocEntries);

void* __fastcall Table_At(Collection* t, int, int32_t i) { return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)); }
RT3_REPLACE(0x409e60, Table_At);

double __fastcall Obj_Float338(void* o, int, int32_t i) { return F<float>(o, 0x338 + i * 4); }
RT3_REPLACE(0x40caa0, Obj_Float338);

void __fastcall Obj_Set2a2(void* o, int, uint16_t v) { F<uint16_t>(o, 0x2a2) = v; }
RT3_REPLACE(0x40cb00, Obj_Set2a2);

uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x2129) + (F<int32_t>(g, 0x2145) * y + x) * 0x1d7;
}
RT3_REPLACE(0x40ccd0, Grid_RegionAt);

// 0x40ccf0. State byte +0x8c of the referenced building's state object.
uint32_t __fastcall Obj_BuildingState8c(void* o) {
    void* b = Obj_Building(o);
    void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    return F<uint8_t>(st, 0x8c);
}
RT3_REPLACE(0x40ccf0, Obj_BuildingState8c);

void* __fastcall Obj_Ref2a8(void* o) {
    uint32_t id = F<uint32_t>(o, 0x2a8);
    return id ? Collection_Get(g_table6cec20, 0, id) : nullptr;
}
RT3_REPLACE(0x40d1f0, Obj_Ref2a8);

// 0x411ce0. For kind 2: +0x790 = some entry (0xbc bytes, +0x37, count +0x33) has
// first dword 0; +0x78c = no entry has +0x2c == 0, or +0x790 is clear.
void __fastcall Obj_UpdateEntryFlags(void* o) {
    F<int32_t>(o, 0x78c) = 0;
    F<int32_t>(o, 0x790) = 0;
    if (F<uint8_t>(o, 0x32) != 2) return;
    F<int32_t>(o, 0x78c) = 1;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc) {
        if (F<int32_t>(e, 0) == 0) F<int32_t>(o, 0x790) = 1;
        if (F<int32_t>(e, 0x2c) == 0) F<int32_t>(o, 0x78c) = 0;
    }
    if (F<int32_t>(o, 0x790) == 0) F<int32_t>(o, 0x78c) = 1;
}
RT3_REPLACE(0x411ce0, Obj_UpdateEntryFlags);

// 0x412510. Available: enabled (+0x7ac) and (option +0x17c, or its year (u16 +0x26) reached).
int32_t __fastcall Obj_Available(void* o) {
    if (F<int32_t>(o, 0x7ac) == 0) return 0;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x17c) != 0) return 1;
    uint16_t year = F<int32_t>(g_world, 0x46c38) ? F<uint16_t>(g_world, 0x5) : F<uint16_t>(g_world, 0xd);
    return year >= F<uint16_t>(o, 0x26) ? 1 : 0;
}
RT3_REPLACE(0x412510, Obj_Available);

} // namespace rt3
