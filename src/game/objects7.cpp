// More simulation accessors (batch 10). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trains      = hook::Global<Collection*>(0x6cfcbc);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);
static uint8_t*& g_cargoTypes = hook::Global<uint8_t*>(0x62ba8c); // cargo-type table object (+0x16e: available flags)

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int __fastcall ToUpper(char c);
int32_t __fastcall Obj_BuildingValue(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall Obj_YearReached(void* o);
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c);

// 0x426590. Cargo objects (0x6cec20) on buildings owned by this company with
// flag +0x3d4 == 1; with `activeOnly`, only those whose building is active.
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly) {
    int32_t total = Collection_Count(g_table6cec20), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        void* o = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(o) != F<int32_t>(c, 0)) continue;
        if (Obj_Flag3d4(Obj_Building(o)) != 1) continue;
        if (activeOnly && !Obj_BuildingStateActive(o)) continue;
        ++n;
    }
    return n;
}
RT3_REPLACE(0x426590, Company_CountCargoSites);

// 0x41e970. Recomputes the cargo-type availability arrays (53 types):
// +0x246[i] = type +0x56 (enabled), +0x16e[i] = its year reached; with counts
// +0x31a and +0x242.
void __fastcall CargoTypes_Recount(Collection* t) {
    uint8_t* o = (uint8_t*)t;
    F<int32_t>(o, 0x242) = 0;
    F<int32_t>(o, 0x31a) = 0;
    for (uint32_t i = 0; i < 0x35; ++i) {
        int32_t& enabled = F<int32_t>(o, 0x246 + i * 4);
        int32_t& reached = F<int32_t>(o, 0x16e + i * 4);
        if (!Collection_IsValid(t, 0, i)) {
            reached = 0;
            enabled = 0;
            continue;
        }
        enabled = F<int32_t>(Collection_Get(t, 0, i), 0x56);
        if (enabled) F<int32_t>(o, 0x31a)++;
        reached = Obj_YearReached(Collection_Get(t, 0, i));
        if (reached) F<int32_t>(o, 0x242)++;
    }
}
RT3_REPLACE(0x41e970, CargoTypes_Recount);

// 0x40d2d0. Refresh: virtual slot 21(0); then if the world option +0x4ae7 is on,
// virtual slot 28 passes, its 0x6cec20 record (+0x2a8) is active and flagged
// (+0x5bc): virtual slot 22(0x5f, 0x5f, 0xff).
void __fastcall Obj_RefreshHighlight(void* o) {
    void** vt = *(void***)o;
    ((void(__thiscall*)(void*, int32_t))vt[0x54 / 4])(o, 0);
    if (!F<int32_t>(g_world, 0x4ae7)) return;
    if (!((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) return;
    auto rec = [&]() -> uint8_t* {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        return id ? (uint8_t*)Collection_Get(g_table6cec20, 0, id) : nullptr;
    };
    if (!Obj_BuildingStateActive(rec())) return;
    volatile uint8_t* r = rec();
    if (!r[0x5bc]) return;
    ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))(*(void***)o)[0x58 / 4])(o, 0x5f, 0x5f, 0xff);
}
RT3_REPLACE(0x40d2d0, Obj_RefreshHighlight);

// 0x4a8690. Rank of this train among its company's trains in list-1 order,
// not counting special-kind trains (0x11/0xc/0xd) with flag bit 2 of +0x1ce.
int32_t __fastcall Train_Rank(void* self) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 1));
        if (t == self) return n;
        if (F<int32_t>(t, 0x51) != F<int32_t>(self, 0x51)) continue;
        int32_t k = F<int32_t>(t, 0xe8);
        if ((k == 0x11 || k == 0xc || k == 0xd) && (F<uint8_t>(t, 0x1ce) & 2)) continue;
        ++n;
    }
    return 0;
}
RT3_REPLACE(0x4a8690, Train_Rank);

// 0x412560. Is an engine/car type available: required cargo type enabled (if
// rec[0] == 0), within its year range, not barred by the flag/scenario bits,
// and all its listed cargo types enabled (unless world +0x4afb).
int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag) {
    const int32_t* avail = (const int32_t*)(g_cargoTypes + 0x16e);
    int32_t ok = 0;
    bool pass = true;
    if (F<int32_t>(rec, 0) == 0 && avail[F<int32_t>(rec, 0x1c)] == 0) pass = false;
    if (pass) {
        int32_t year = F<uint16_t>(g_world, 0xd);
        if (year < F<int16_t>(rec, 0x20) || year > F<int16_t>(rec, 0x22)) pass = false;
        else if (F<int32_t>(rec, 0x24) && flag == 0) pass = false;
        else if (F<int32_t>(rec, 0x28) && flag != 0) pass = false;
        else ok = 1;
    }
    if (F<int32_t>(g_world, 0x4afb) == 0 && ok) {
        int32_t n = F<int32_t>(rec, 0x2c);
        uint8_t* e = rec + 0x44;
        for (int32_t i = 0; i < n; ++i, e += 0x1c)
            if (avail[F<int32_t>(e, 0)] == 0) ok = 0;
    }
    return ok;
}
RT3_REPLACE(0x412560, Vehicle_Available);

// 0x434ea0 / 0x435030. Lookups by name (+0) in world tables +0x66b2 / +0x66b6:
// first char (name uppercased vs. stored) then exe stricmp; value +0x1e / +0x3d.
static int32_t FindNamed(Collection* t, const char* name, uint32_t valueOffset) {
    int32_t total = Collection_Count(t);
    char first = (char)ToUpper(name[0]);
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)r[0] != first) continue;
        if (ExeStricmp(name, (const char*)r) == 0) return F<int32_t>(r, valueOffset);
    }
    return 0;
}
int32_t __fastcall World_LookupNamed1(void* w, int, const char* name) {
    return FindNamed(F<Collection*>(w, 0x66b2), name, 0x1e);
}
RT3_REPLACE(0x434ea0, World_LookupNamed1);
int32_t __fastcall World_LookupNamed2(void* w, int, const char* name) {
    return FindNamed(F<Collection*>(w, 0x66b6), name, 0x3d);
}
RT3_REPLACE(0x435030, World_LookupNamed2);

// 0x4b2a50. Runs Obj_ForCityChain(0, 0, 0) for every record of the table.
void __fastcall Table_ForAllCityChains(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_ForCityChain(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, 0, 0, 0);
}
RT3_REPLACE(0x4b2a50, Table_ForAllCityChains);

// 0x426be0. Company entry i unflagged and region type i has +0x2d == 1.
int32_t __fastcall Company_EntryOpen(void* c, int, int32_t i) {
    if (F<uint8_t>(c, 0x1d9f + i * 0x38d)) return 0;
    return F<int32_t>(Collection_Get(g_table6cfc9c, 0, i), 0x2d) == 1 ? 1 : 0;
}
RT3_REPLACE(0x426be0, Company_EntryOpen);

// 0x4299f0. Number of live (+0x3f) records before the one with ID `id` (0 if absent).
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Collection_IdAt(t, 0, i, 0) == id) return n;
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f)) ++n;
    }
    return 0;
}
RT3_REPLACE(0x4299f0, Table_LiveRankOf);

// 0x42a0e0. Index of the largest float in the 12-byte entries at +0x67 (count +0x5b), -1 if none.
RT3_NOINLINE int32_t __fastcall Obj_ArgMax67(void* o) {
    int32_t best = -1, n = F<int32_t>(o, 0x5b);
    for (int32_t i = 0; i < n; ++i) {
        if (best == -1 || F<float>(o, 0x67 + i * 0xc) > F<float>(o, 0x67 + best * 0xc)) best = i;
    }
    return best;
}
RT3_REPLACE(0x42a0e0, Obj_ArgMax67);

// 0x42d6c0. Frees the four lists at +4..+0x10 (next at +0x24).
void __fastcall Obj_FreeFourLists(void* o) {
    for (int k = 0; k < 4; ++k) {
        uint8_t*& head = F<uint8_t*>(o, 4 + k * 4);
        while (head) {
            uint8_t* next = *(uint8_t**)(head + 0x24);
            ((void(__cdecl*)(void*))0x53b080)(head);
            head = next;
        }
        head = nullptr;
    }
}
RT3_REPLACE(0x42d6c0, Obj_FreeFourLists);

// 0x42d700. Cargo flags of the load list at *this (next +0x1a), from the cargo
// table 0x5f3e04 (0x81-byte records). 0x7f if +0x82b, or if an entry is -1.
// (AL only, as in the original.)
uint8_t __fastcall Load_CargoFlags(void* o) {
    if (F<uint8_t>(o, 0x82b)) return 0x7f;
    uint8_t flags = 0;
    for (int32_t* n = F<int32_t*>(o, 0); n; n = *(int32_t**)((uint8_t*)n + 0x1a)) {
        if (*n < 0) return *n == -1 ? 0x7f : 0;
        flags |= *((const uint8_t*)0x5f3e04 + *n * 0x81);
    }
    return flags;
}
RT3_REPLACE(0x42d700, Load_CargoFlags);

// 0x42d740. Flags of list i (+4 + i*4, next +0x24) from table 0x6103fd
// (0x6e-byte records). With `classify`, kinds 11/13/15/127 count as 8.
uint8_t __fastcall Obj_ListFlags(void* o, int, int32_t i, int32_t classify) {
    uint8_t flags = 0;
    for (int32_t* n = F<int32_t*>(o, 4 + i * 4); n; n = *(int32_t**)((uint8_t*)n + 0x24)) {
        uint8_t k = *((const uint8_t*)0x6103fd + *n * 0x6e);
        if (classify && (k == 11 || k == 13 || k == 15 || k == 127)) flags |= 8;
        else flags |= k;
    }
    return flags;
}
RT3_REPLACE(0x42d740, Obj_ListFlags);

} // namespace rt3
