// More simulation accessors (batch 7). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings  = hook::Global<Collection*>(0x62b26c);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c); // region types
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_table62bae0 = hook::Global<Collection*>(0x62bae0);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int __fastcall ToUpper(char c);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
int32_t __fastcall Obj_BuildingStateActive(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_Get21(void* o);
void* __fastcall Obj_Ref25(void* o);

// 0x414bd0. (x, y) lies inside the map grid [0, w-1] x [0, h-1].
int32_t __fastcall Map_Contains(void* m, int, float x, float y) {
    const float zero = *(const float*)0x5c8598;
    if (x < zero) return 0;
    float maxX = (float)(F<int32_t>(m, 0x15d9) - 1);
    if (!(x <= maxX)) return 0;
    if (y < zero) return 0;
    float maxY = (float)(F<int32_t>(m, 0x15d5) - 1);
    if (!(y <= maxY)) return 0;
    return 1;
}
RT3_REPLACE(0x414bd0, Map_Contains);

// 0x476bb0. Index (not ID) of this object's name (+0xc47) in the name table
// of object 0x6d4020 (+0x429b4), or -1.
int32_t __fastcall Obj_NameIndex(void* o) {
    auto table = [] { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b4); };
    for (int32_t i = 0; i < Collection_Count(table()); ++i) {
        Collection* t = table();
        const char* name = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (ExeStricmp((const char*)o + 0xc47, name) == 0) return i;
    }
    return -1;
}
RT3_REPLACE(0x476bb0, Obj_NameIndex);

// 0x475240. Sets elements' +0x1d8 flag: 0 when mode == 1, else 1; idx -1 = all.
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx) {
    uint8_t v = mode == 1 ? 0 : 1;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i)
        if (idx == -1 || i == idx) F<uint8_t>(F<uint8_t*>(o, 0xc + i * 4), 0x1d8) = v;
}
RT3_REPLACE(0x475240, Obj_SetElementFlag1d8);

// 0x48aa70. Region type at this object's centre.
uint32_t __fastcall Obj_CenterRegionType(void* o) {
    const float twoCells = 184.0f;
    float y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / twoCells);
    float x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / twoCells);
    return Map_RegionTypeAt(hook::Global<void*>(0x62c120), 0, x, y);
}
RT3_REPLACE(0x48aa70, Obj_CenterRegionType);

// 0x412af0. ID of the record named `name` (+4), first letters compared
// uppercased before the full (exe) stricmp; 0 if none.
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        const char* r = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)) + 4;
        if ((char)ToUpper(name[0]) != (char)ToUpper(r[0])) continue;
        if (ExeStricmp(r, name) == 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x412af0, Table_FindByName2);

// 0x42b310. Adds to float slot i (+0x103), not below zero.
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d) {
    float v = (float)((double)d + F<float>(o, 0x103 + i * 4));
    F<float>(o, 0x103 + i * 4) = v;
    if (v < *(const float*)0x5c8598) F<float>(o, 0x103 + i * 4) = 0.0f;
}
RT3_REPLACE(0x42b310, Obj_AddFloat103);

// 0x42c880. First active record in the 0x6cec20 chain starting at u16 +0xd4 (next u16 +0x3c).
void* __fastcall Obj_FirstActiveInChain(void* o) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; ) {
        void* r = Collection_Get(g_table6cec20, 0, id);
        if (Obj_BuildingStateActive(r)) return r;
        id = F<uint16_t>(Collection_Get(g_table6cec20, 0, id), 0x3c);
    }
    return nullptr;
}
RT3_REPLACE(0x42c880, Obj_FirstActiveInChain);

// 0x42c8f0. Sets bit 2 of +0xe6 if any building in the chain from u16 +0xd6
// (next u16 +0x2a2) has flag +0x3d4 == 1 and state byte +0xba set; else clears it.
void __fastcall Obj_UpdateFlagE6(void* o) {
    for (uint16_t id = F<uint16_t>(o, 0xd6); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        if (Obj_Flag3d4(b) == 1) {
            void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            if (F<uint8_t>(st, 0xba)) {
                F<uint8_t>(o, 0xe6) |= 4;
                return;
            }
        }
        id = F<uint16_t>(b, 0x2a2);
    }
    F<uint8_t>(o, 0xe6) &= 0xfb;
}
RT3_REPLACE(0x42c8f0, Obj_UpdateFlagE6);

// 0x433740. +0x29 rounded down to 1/100 (after adding 0.001).
double __fastcall Obj_Rounded29(void* o) {
    const double eps = *(const double*)0x5c8678, hundred = *(const double*)0x5c85d8;
    int32_t c = (int32_t)(((double)F<float>(o, 0x29) + eps) * hundred);
    float r = (float)((double)c / hundred);
    return r;
}
RT3_REPLACE(0x433740, Obj_Rounded29);

int32_t Options_Get10c() { return F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c); }
RT3_REPLACE(0x439ba0, Options_Get10c);

void ResetPair5ee4fc() {
    hook::Global<int32_t>(0x5ee4fc) = -1;
    hook::Global<int32_t>(0x5ee500) = -1;
}
RT3_REPLACE(0x43f610, ResetPair5ee4fc);

int32_t __fastcall Map_PointCount(void* m) { return (F<int32_t>(m, 0x15d9) + 1) * (F<int32_t>(m, 0x15d5) + 1); }
RT3_REPLACE(0x4490f0, Map_PointCount);

// 0x44ade0. Region type record at (x, y), following its +0x35 alias unless +0x31 is set.
void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    int32_t col = (int32_t)x;
    uint32_t type = (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
    uint8_t* rec = (uint8_t*)Collection_Get(g_table6cfc9c, 0, type);
    if (F<int32_t>(rec, 0x31) != 0) return rec;
    return Collection_Get(g_table6cfc9c, 0, F<uint32_t>(rec, 0x35));
}
RT3_REPLACE(0x44ade0, Map_RegionRecordAt);

// 0x469690. Singly linked list {head, tail, count}: insert after `after` (NULL = at head).
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node) {
    if (after) {
        *node = *after;
        *after = node;
        ((int32_t*)l)[2]++;
        if (after != (void**)l[1]) return;
    } else {
        *node = l[0];
        l[0] = node;
        ((int32_t*)l)[2]++;
        if (l[1]) return;
    }
    l[1] = node;
}
RT3_REPLACE(0x469690, SList_InsertAfter);

// 0x474820. Record (0x140 bytes, from +0xc of 0x6cea60) whose key is the
// case-insensitive CRC of `name`; NULL if none.
void* __fastcall FindRecordByCrc(const char* name) {
    uint8_t* t = hook::Global<uint8_t*>(0x6cea60);
    uint32_t crc = Crc32String(name, 1);
    uint8_t* r = t + 0xc;
    for (int32_t i = 0; i < F<int32_t>(t, 8); ++i, r += 0x140)
        if (*(uint32_t*)r == crc) return r;
    return nullptr;
}
RT3_REPLACE(0x474820, FindRecordByCrc);

void __fastcall Obj_SetAllBc(void* o, int32_t v) {
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) F<int32_t>(F<uint8_t*>(o, 0xc + i * 4), 0xbc) = v;
}
RT3_REPLACE(0x474fe0, Obj_SetAllBc);

// 0x476a60. World condition flags: +0x4acb with +0x21, +0x4ad7 with its player's
// +0x2a7 >= 1, or +0x4ad3.
int32_t World_AnyCondition() {
    if (F<int32_t>(g_world, 0x4acb) && Obj_Get21(g_world)) return 1;
    if (F<int32_t>(g_world, 0x4ad7) && F<int32_t>(Obj_Ref25(g_world), 0x2a7) >= 1) return 1;
    return F<int32_t>(g_world, 0x4ad3) ? 1 : 0;
}
RT3_REPLACE(0x476a60, World_AnyCondition);

void* __fastcall Obj_Node8(void* o) { return Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8)); }
RT3_REPLACE(0x47f310, Obj_Node8);

// 0x480160. 0x62bae0 record of the building at +4 (u16 +0x26e; 0 = the first record).
// QUIRK: without a building it reads address 0x26e, as the original.
void* __fastcall Obj_BuildingKind(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    volatile uint8_t* b = id ? (uint8_t*)Collection_Get(g_buildings, 0, id) : nullptr;
    uint32_t k = *(volatile uint16_t*)(b + 0x26e);
    if (!k) k = Collection_IdAt(g_table62bae0, 0, 0, 0);
    return Collection_Get(g_table62bae0, 0, k);
}
RT3_REPLACE(0x480160, Obj_BuildingKind);

void __fastcall Node_ResetKind(void* n) {
    int32_t k = F<int32_t>(n, 0x226);
    if (k == 12 || k == 13) {
        F<int32_t>(n, 0x226) = 0;
        F<int32_t>(n, 0x24d) = 0;
    }
}
RT3_REPLACE(0x48a400, Node_ResetKind);

// 0x48a620 / 0x48a660. Link pair slot: the second (+0x21e/+0x23c) unless `first`
// or the node is one-sided (+0x218 == 1).
void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first) {
    if (first == 0 && F<uint8_t>(n, 0x218) != 1) {
        F<int32_t>(n, 0x21e) = a;
        F<int32_t>(n, 0x23c) = b;
    } else {
        F<int32_t>(n, 0x21a) = a;
        F<int32_t>(n, 0x238) = b;
    }
}
RT3_REPLACE(0x48a620, Node_SetLinkPair);
int32_t __fastcall Node_GetLink(void* n, int, int32_t first) {
    return (first == 0 && F<uint8_t>(n, 0x218) != 1) ? F<int32_t>(n, 0x21e) : F<int32_t>(n, 0x21a);
}
RT3_REPLACE(0x48a660, Node_GetLink);

} // namespace rt3
