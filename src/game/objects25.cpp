// Small functions across blocks 0x43..0x46 (batch 28). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "game/calendar.h"
#include <stdint.h>

struct HWND__;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

::HWND__* __fastcall Engine_Window(void* e);

// 0x4331e0. Records with byte +0x82b clear.
int32_t __fastcall Table_CountClear82b(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (!F<uint8_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x82b)) ++n;
    return n;
}
RT3_REPLACE(0x4331e0, Table_CountClear82b);

// 0x434000. As 0x424050 for the slots +0x8a with last key +0xa22.
void __fastcall Obj_SetSlot8a(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xa22)) return;
    if (add) F<int32_t>(o, 0x8a + idx * 4) += value;
    else F<int32_t>(o, 0x8a + idx * 4) = value;
    if (key) F<int32_t>(o, 0xa22) = key;
}
RT3_REPLACE(0x434000, Obj_SetSlot8a);

// 0x449050. Darkness of a brightness byte: (255 - b - 90) / 165, 0 when b >= 165.
float __stdcall DarknessOf(uint8_t b) {
    int32_t d = 0xff - b;
    if (d <= 0x5a) return 0.0f;
    float f = (float)(d - 0x5a);
    return (float)((double)f / *(const float*)0x5ca9f8);
}
RT3_REPLACE(0x449050, DarknessOf);

// 0x44ada0. Region record (0x6cfc9c) of the cell at (x, y).
void* __fastcall Map_RegionAt(uint8_t* map, int, float x, float y) {
    int32_t yi = (int32_t)y;
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    int32_t xi = (int32_t)x;
    return Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, row[xi] >> 3);
}
RT3_REPLACE(0x44ada0, Map_RegionAt);

// 0x455440 / 0x455490. Record whose +4 is v - 1 / v + 1; 0 if none.
void* __fastcall Table_FindField4Prev(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) == v - 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x455440, Table_FindField4Prev);
void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) == v + 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x455490, Table_FindField4Next);

// 0x45a6b0. Unlinks `node` (after `prev`, or the head when prev is null)
// from a singly linked list {head, tail, count}; returns node.
void** __fastcall SList_Unlink(void** l, int, void** prev, void** node) {
    if (prev) {
        *prev = *node;
        *node = nullptr;
        ((int32_t*)l)[2]--;
        if (node == (void**)l[1]) l[1] = prev;
        return node;
    }
    void** n = (void**)l[0];
    if (n) {
        l[0] = *n;
        if (n == (void**)l[1]) l[1] = *n;
        ((int32_t*)l)[2]--;
    }
    return node;
}
RT3_REPLACE(0x45a6b0, SList_Unlink);

// 0x45ed90. Value of entry `key` in the table 0x5f0a30 ({value, key, ..}
// x 91) if `mask` has at most one bit set; 0 otherwise.
uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask) {
    uint32_t i = 0;
    for (; i < 0x5b; ++i)
        if (((const int32_t*)0x5f0a34)[i * 4] == key) break;
    if (i >= 0x5b) return 0;
    int32_t bits = 0;
    for (int32_t m = mask; m; m >>= 1)
        if (m & 1) ++bits;
    if (bits > 1) return 0;
    return ((const uint32_t*)0x5f0a30)[i * 4];
}
RT3_REPLACE(0x45ed90, LookupSingleFlag);

// 0x462570 / 0x464130. Releases a GDI pair {object, dc}.
static void ReleaseGdiPair(void** p) {
    if (!p[1]) return;
    p[1] = nullptr;
    if (void* e = hook::Global<void*>(0x6d4024))
        ((int32_t(__stdcall*)(void*, void*))hook::Global<void*>(0x5c822c))(Engine_Window(e), nullptr);
    if (p[0]) ((int32_t(__stdcall*)(void*))hook::Global<void*>(0x5c8228))(p[0]);
    p[0] = nullptr;
}
void __fastcall GdiPair_Release(void** p) { ReleaseGdiPair(p); }
RT3_REPLACE(0x462570, GdiPair_Release);
void __fastcall GdiPair_Release2(void** p) { ReleaseGdiPair(p); }
RT3_REPLACE(0x464130, GdiPair_Release2);

// 0x468c10. Appends a 9-byte entry to row +0x17c of the table +0x180 (512 per row), count +0x178.
void __stdcall Table9_Append(uint8_t* o, const uint8_t* src) {
    int32_t i = F<int32_t>(o, 0x178) + F<int32_t>(o, 0x17c) * 512;
    F<int32_t>(o, 0x178)++;
    uint8_t* d = o + i * 9 + 0x180;
    F<uint32_t>(d, 0) = F<uint32_t>((void*)src, 0);
    F<uint32_t>(d, 4) = F<uint32_t>((void*)src, 4);
    d[8] = src[8];
}
RT3_REPLACE(0x468c10, Table9_Append);

// 0x4695d0. Next non-empty 256-byte line of the buffer 0x6ae4d8 (cursor
// 0x6cd97c, reset when +0x9874 is set); 0 at the end or while 0x6cd920 is set.
const char* __fastcall Obj_NextBufferedLine(void* o) {
    if (hook::Global<int32_t>(0x6cd920)) return nullptr;
    if (F<int32_t>(o, 0x9874) == 1) {
        hook::Global<int32_t>(0x6cd97c) = 0;
        F<int32_t>(o, 0x9874) = 0;
    }
    const char* p = (const char*)0x6ae4d8 + hook::Global<int32_t>(0x6cd97c) * 256;
    if (!*p) return nullptr;
    hook::Global<int32_t>(0x6cd97c)++;
    return p;
}
RT3_REPLACE(0x4695d0, Obj_NextBufferedLine);

// 0x469a10. Counts the list's entries (from +8) whose item is not kind 'g' and lacks flag 4 at +0xb.
void __fastcall Obj_CountItems(void* o, int, uint8_t* list) {
    F<int32_t>(o, 0x9894) = 0;
    if (!list) return;
    for (uint8_t** n = F<uint8_t**>(list, 8); n; n = (uint8_t**)*n) {
        uint8_t* e = (uint8_t*)n[1];
        if (e[9] != 0x67 && !(e[0xb] & 4)) F<int32_t>(o, 0x9894)++;
    }
}
RT3_REPLACE(0x469a10, Obj_CountItems);

// 0x46c280. Detail level for value v (EDX) under setting 0x5f2b70.
int32_t __fastcall DetailLevelFor(int32_t, int32_t v) {
    int32_t s = hook::Global<int32_t>(0x5f2b70);
    if (s < 1000) return v <= 1000 ? 0xff : 9;
    if (s < 2000) {
        if (v <= 1000) return 8;
        return v <= 3000 ? 0xff : 9;
    }
    return 0xff;
}
RT3_REPLACE(0x46c280, DetailLevelFor);

// 0x46c330. First free slot of the table 0x6ce2e8 (10 x 0x5c bytes), or 10.
int32_t FirstFreeSlot6ce2e8() {
    int32_t i = 0;
    for (const uint8_t* p = (const uint8_t*)0x6ce2e8; p < (const uint8_t*)0x6ce680; p += 0x5c, ++i)
        if (*(const int32_t*)p == 0) return i;
    return 10;
}
RT3_REPLACE(0x46c330, FirstFreeSlot6ce2e8);

void CloseFile6ce994() {
    void*& f = hook::Global<void*>(0x6ce994);
    if (!f) return;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    f = nullptr;
}
RT3_REPLACE(0x46d240, CloseFile6ce994);

void __fastcall Obj_SetDateMinutes(uint8_t* o, int, const GameDate* d) {
    F<int32_t>(o, 0x8f38) = d ? DateToMinutes(d) : 0;
}
RT3_REPLACE(0x46d610, Obj_SetDateMinutes);

} // namespace rt3
