// Small functions across blocks 0x40..0x46 (batch 30). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);

uint8_t* __fastcall Obj_Region(void* o);
void __fastcall Obj_FreeKeyList(void* o);
void __fastcall Station_Reset(void* o, int, uint16_t x, uint16_t y);
uint32_t __fastcall BlockIter_Begin(uint8_t* o);
int32_t ClockMs();

// 0x40cd10. With option +0x1c3, looks up (and discards) every site of the
// region's key list. Kept for exact behaviour.
void __fastcall Obj_TouchRegionSites(void* o) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1c3) || F<uint8_t>(o, 0x3d4) != 1) return;
    uint8_t* r = Obj_Region(o);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i)
        Collection_Get(g_table6cec20, 0, F<uint16_t>(F<uint8_t*>(r, 0xef), i * 6));
}
RT3_REPLACE(0x40cd10, Obj_TouchRegionSites);

// 0x434080. +0x4cbe if above the threshold 0x5c8a20, else 50.
float __fastcall World_Value4cbe(void* w) {
    float f = F<float>(w, 0x4cbe);
    if ((double)f > *(const double*)0x5c8a20) return f;
    return 50.0f;
}
RT3_REPLACE(0x434080, World_Value4cbe);

// 0x4422d0. Appends the current option bytes (+0x140, +0x13c, +0x138,
// +0x144) and world byte +0x4c74 to the 5-byte history 0x62be90.
void AppendOptionHistory() {
    int32_t n = hook::Global<int32_t>(0x62bedc);
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    uint8_t* d = (uint8_t*)0x62be90 + n * 5;
    d[0] = opt[0x140];
    d[1] = opt[0x13c];
    d[2] = opt[0x138];
    d[3] = opt[0x144];
    d[4] = hook::Global<uint8_t*>(0x6cec78)[0x4c74];
    hook::Global<int32_t>(0x62bedc) = n + 1;
}
RT3_REPLACE(0x4422d0, AppendOptionHistory);

// 0x44ff10 / 0x44ff70. Per map cell: free its key list / reset it.
void __fastcall Map_FreeCellKeyLists(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Obj_FreeKeyList(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44ff10, Map_FreeCellKeyLists);
void __fastcall Map_ResetCells(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Station_Reset(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7, 0, (uint16_t)x, (uint16_t)y);
}
RT3_REPLACE(0x44ff70, Map_ResetCells);

// 0x4552b0. Initialises a 0x21a-byte entry; +4 gets the next serial 0x6acd30.
void __fastcall Entry21a_Init(uint8_t* e, int, int32_t a, const char* name, int32_t b, int32_t c, int32_t d) {
    F<int32_t>(e, 0) = a;
    char* p = (char*)e + 0xd;
    while ((*p++ = *name++) != 0) {}
    F<int32_t>(e, 0x20e) = b;
    F<int32_t>(e, 0x212) = c;
    F<int32_t>(e, 0x216) = d;
    F<int32_t>(e, 8) = 0;
    e[0xc] = 0;
    F<int32_t>(e, 4) = hook::Global<int32_t>(0x6acd30);
    hook::Global<int32_t>(0x6acd30)++;
}
RT3_REPLACE(0x4552b0, Entry21a_Init);

// 0x455370. Record with the highest serial +4 (ID from +0); 0 if none.
void* __fastcall Table_Newest(Collection* t) {
    uint32_t sel = 0;
    int32_t best = -1;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) > best) {
            sel = F<uint32_t>(r, 0);
            best = F<int32_t>(r, 4);
        }
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x455370, Table_Newest);

// 0x4555b0. Stamps unstamped entries (+8) with the clock; flags (+0xc)
// those stamped more than 10 s ago.
RT3_NOINLINE void __fastcall Table_MarkStale(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        int32_t stamp = F<int32_t>(r, 8);
        int32_t now = ClockMs();
        if (stamp) {
            if ((uint32_t)(now - stamp) > 10000) r[0xc] = 1;
        } else {
            F<int32_t>(r, 8) = now;
        }
    }
}
RT3_REPLACE(0x4555b0, Table_MarkStale);

// 0x459cb0. Among the blocks of the current iteration (0x6acd4c, stride
// 0x168) flagged 0x10 with the same key as `b`: the first one after `b`,
// else the first one before it, else b.
uint8_t* __fastcall BlockIter_NextSameKey(uint8_t* b) {
    uint8_t* p = hook::Global<uint8_t*>(0x6acd4c);
    int32_t n = F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 8);
    uint8_t* r = b;
    for (int32_t i = 0; i < n; ++i, p += 0x168) {
        if (!(p[0x8c] & 0x10) || F<int32_t>(p, 0) != F<int32_t>(b, 0) || p == b) continue;
        if (p > b) return p;
        if (r == b) r = p;
    }
    return r;
}
RT3_REPLACE(0x459cb0, BlockIter_NextSameKey);

// 0x45b540. Begins the block iteration of +0x256 when +0x75 is set; 1 otherwise.
uint32_t __fastcall Obj_BeginBlocks(uint8_t* o) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return 1;
    return BlockIter_Begin(blocks);
}
RT3_REPLACE(0x45b540, Obj_BeginBlocks);

// 0x468260. Releases every record whose u16 +8 equals key.
void __fastcall Table_ReleaseKey8(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        if (key == F<uint16_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 8)) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x468260, Table_ReleaseKey8);

} // namespace rt3
