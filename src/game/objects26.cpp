// Small functions across blocks 0x40..0x47 (batch 29). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_BuildingStateActive(void* o);

// 0x40e030. Not (virtual slot 28 and its site +0x2a8 active).
int32_t __fastcall Obj_NotActiveSite(void* o) {
    int32_t on = 0;
    if (((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        void* site = id ? Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id) : nullptr;
        if (Obj_BuildingStateActive(site)) on = 1;
    }
    return on == 0 ? 1 : 0;
}
RT3_REPLACE(0x40e030, Obj_NotActiveSite);

// 0x4490a0. Brightness byte of a darkness fraction: 0xff for <= 0, 0 for >= 1,
// else 165 - 165 f (AL only on the first two paths).
int32_t __stdcall BrightnessOf(float f) {
    if (!(f > *(const float*)0x5c8598)) return 0xff;
    if (!((double)f < *(const double*)0x5c8770)) return 0;
    const float k = *(const float*)0x5ca9f8;
    int32_t v = (int32_t)((double)k - (double)f * k);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x4490a0, BrightnessOf);

// 0x45a660. Starts an iteration over the block list o: resets the cursor
// globals 0x6acd44..0x6acd54 and returns the first block's +0x84.
uint32_t __fastcall BlockIter_Begin(uint8_t* o) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<int32_t>(0x6acd4c) = 0;
    hook::Global<uint8_t*>(0x6acd50) = o;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    hook::Global<uint8_t*>(0x6acd4c) = p;
    return F<uint32_t>(p, 0x84);
}
RT3_REPLACE(0x45a660, BlockIter_Begin);

// 0x4686f0. Registry key wrapper (vtbl 0x5ce15c): creates/opens `subKey`
// under `root` with full access; +8 is set when that fails.
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root) {
    o[0] = 0x5ce15c;
    o[2] = 0;
    uint32_t disposition;
    int32_t r = ((int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*))
                     hook::Global<void*>(0x5c8008))(root, subKey, 0, nullptr, 0, 0xf003f, nullptr, &o[1], &disposition);
    if (r) o[2] = 1;
    return o;
}
RT3_REPLACE(0x4686f0, RegKey_Construct);

// 0x468c50. Entry of row +0x17c of the 9-byte table (see 0x468c10) matching byte 0 and dword +5.
uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e) {
    uint8_t* p = o + F<int32_t>(o, 0x17c) * 9 * 512 + 0x180;
    for (int32_t i = 0; i < F<int32_t>(o, 0x178); ++i, p += 9)
        if (p[0] == e[0] && F<int32_t>(p, 5) == F<int32_t>((void*)e, 5)) return p;
    return nullptr;
}
RT3_REPLACE(0x468c50, Table9_Find);

// 0x46d260. Appends an 18-byte record {u16, 4 dwords} to the table 0x6cd990 (max 128).
void __fastcall Queue6cd990_Push(uint16_t a, int32_t b, int32_t c, int32_t d, int32_t e) {
    int32_t n = hook::Global<int32_t>(0x6ce9a0);
    if (n >= 0x80) return;
    uint8_t* r = (uint8_t*)0x6cd990 + n * 18;
    F<uint16_t>(r, 0) = a;
    F<int32_t>(r, 2) = b;
    F<int32_t>(r, 6) = c;
    F<int32_t>(r, 0xa) = d;
    F<int32_t>(r, 0xe) = e;
    hook::Global<int32_t>(0x6ce9a0) = n + 1;
}
RT3_REPLACE(0x46d260, Queue6cd990_Push);

// 0x46da70. Position-weighted checksum of the map bitmap +0x213d.
int32_t MapBitmapChecksum() {
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t n = (F<int32_t>(map, 0x2161) * F<int32_t>(map, 0x215d) + 7) / 8;
    int32_t sum = 0;
    const uint8_t* buf = F<uint8_t*>(map, 0x213d);
    for (int32_t off = 0; off < n; ) {
        int32_t v = *(const int32_t*)(buf + off);
        off += 4;
        if (v) sum += v * off;
    }
    return sum;
}
RT3_REPLACE(0x46da70, MapBitmapChecksum);

void* __fastcall ReturnThis473cc0(void* o) { return o; }
RT3_REPLACE(0x473cc0, ReturnThis473cc0);
void __fastcall Nop473cd0(void*) {}
RT3_REPLACE(0x473cd0, Nop473cd0);

// 0x473d70 / 0x473da0. Collection subclass 0x5ce418 (inline 0x187-byte records, 10 + 5).
Collection* __fastcall Collection5ce418_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce418;
    Collection_Init(c, 0, 1, 0x187, 10, 5, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x473d70, Collection5ce418_Construct);
void __fastcall Collection5ce418_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce418;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x473da0, Collection5ce418_Destruct);

// 0x473e20. Any record with byte +0x18 clear and +4 set.
int32_t __fastcall Table_AnyPending(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (!r[0x18] && F<int32_t>(r, 4)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x473e20, Table_AnyPending);

// 0x473ee0 / 0x473f30. Record whose +0x14 is v - 1 / v + 1; 0 if none.
void* __fastcall Table_FindField14Prev(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) == v - 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x473ee0, Table_FindField14Prev);
void* __fastcall Table_FindField14Next(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) == v + 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x473f30, Table_FindField14Next);

// 0x474450 / 0x474480. Collection subclass 0x5ce4a8 (heap records, 10 + 20).
Collection* __fastcall Collection5ce4a8_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce4a8;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x474450, Collection5ce4a8_Construct);
void __fastcall Collection5ce4a8_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce4a8;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x474480, Collection5ce4a8_Destruct);

// 0x4749a0. Name +0x44 equals the string 0x5ce4f4 (case-insensitive).
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o) { return Stricmp((const char*)o + 0x44, (const char*)0x5ce4f4) == 0 ? 1 : 0; }
RT3_REPLACE(0x4749a0, Obj_NameIs5ce4f4);

// 0x4750a0. Sum over the entries (+0xc, count +8) of their sizes: +0x38 when
// flag 0x400 of +0xac, otherwise (+4)->+8.
int32_t __fastcall Obj_SumEntrySizes(uint8_t* o) {
    int32_t sum = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0xc + i * 4);
        int32_t v = 0;
        if (F<uint32_t>(e, 0xac) & 0x400) v = F<int32_t>(e, 0x38);
        else if (uint8_t* s = F<uint8_t*>(e, 4)) v = F<int32_t>(s, 8);
        sum += v;
    }
    return sum;
}
RT3_REPLACE(0x4750a0, Obj_SumEntrySizes);

} // namespace rt3
