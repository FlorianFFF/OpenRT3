// Small functions across blocks 0x40..0x47 (batch 31). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_TouchRegionSites(void* o);
const char* __fastcall GetString(int32_t id);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);

// 0x4077e0. Frees the 32 entries of +0x5bd (and each entry's buffer +6), then the array.
void __fastcall Obj_FreeEntries5bd(void* o) {
    for (int32_t i = 0; i < 32; ++i) {
        uint8_t* e = F<uint8_t**>(o, 0x5bd)[i];
        if (F<void*>(e, 6)) Free(F<void*>(e, 6));
        Free(F<uint8_t**>(o, 0x5bd)[i]);
    }
    Free(F<void*>(o, 0x5bd));
    F<void*>(o, 0x5bd) = nullptr;
}
RT3_REPLACE(0x4077e0, Obj_FreeEntries5bd);

// 0x413860. With option +0x1c3, 0x40cd10 on every record.
void __fastcall Table_TouchRegionSites(Collection* t) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1c3)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_TouchRegionSites(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x413860, Table_TouchRegionSites);

// 0x4367c0. Sets the game-end mode (+0x4a73) and its message (string 0xb6b or 0xb6c).
void __fastcall World_SetEndMode(uint8_t* w, int, int32_t mode) {
    F<int32_t>(w, 0x4a73) = mode;
    F<int32_t>(w, 0x4d) = 0;
    F<int32_t>(w, 0x4c88) = F<int32_t>(w, 0x15);
    const char* s = GetString(mode == 1 ? 0xb6b : 0xb6c);
    char* d = (char*)w + 0x4b47;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x4367c0, World_SetEndMode);

// 0x44a930. Darkness (see 0x449050) of the brightness grid `layer` (+0x1631) at (x, y).
float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer) {
    const uint8_t* grid = F<uint8_t*>(g, 0x1631 + layer * 4);
    int32_t d = 0xff - grid[(F<int32_t>(g, 0x15d9) + 1) * y + x];
    if (d <= 0x5a) return 0.0f;
    float f = (float)(d - 0x5a);
    return (float)((double)f / *(const float*)0x5ca9f8);
}
RT3_REPLACE(0x44a930, Grid_Darkness);

// 0x45f370. Finds the entry with key +0xa0 in the hash buckets 0x6acd78..0x6ad960
// (stride 12): outputs its +0xc and repacked flags +0x94. 1 if found.
int32_t __fastcall Buckets_Find(int32_t key, int32_t* value, uint32_t* flags) {
    for (uint8_t** b = (uint8_t**)0x6acd78; b <= (uint8_t**)0x6ad960; b += 3) {
        for (uint8_t* e = *b; e; e = *(uint8_t**)e) {
            if (F<int32_t>(e, 0xa0) != key) continue;
            *value = F<int32_t>(e, 0xc);
            uint32_t f = F<uint32_t>(e, 0x94);
            *flags = ((((f & 2) << 3) | (f & 1)) << 1) | (f & 4);
            return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x45f370, Buckets_Find);

// 0x46a660. Whether rect {x0, x1, y0, y1} overlaps none of the list +0x9878's
// rects other than `key`'s (always 1 when x0 is 999999).
int32_t __fastcall Obj_RectFree(void* o, int, const int32_t* r, int32_t key) {
    uint8_t** list = F<uint8_t**>(o, 0x9878);
    if (!list || r[0] == 999999) return 1;
    for (uint8_t* e = *list; e; e = *(uint8_t**)e) {
        if (F<int32_t>(e, 0x14) == key) continue;
        if (r[3] < F<int32_t>(e, 0xc) || r[2] > F<int32_t>(e, 0x10)) continue;
        if (r[1] < F<int32_t>(e, 4) || r[0] > F<int32_t>(e, 8)) continue;
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x46a660, Obj_RectFree);

// 0x473dc0. Record with the highest serial +0x14 (ID from +0); 0 if none.
void* __fastcall Table_Newest14(Collection* t) {
    uint32_t sel = 0;
    int32_t best = -1;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) > best) {
            sel = F<uint32_t>(r, 0);
            best = F<int32_t>(r, 0x14);
        }
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x473dc0, Table_Newest14);

int32_t __fastcall Obj_SumEntries6c(uint8_t* o) {
    int32_t sum = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) sum += F<int32_t>(F<uint8_t*>(o, 0xc + i * 4), 0x6c);
    return sum;
}
RT3_REPLACE(0x4750e0, Obj_SumEntries6c);

int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i) {
    if (i >= F<int32_t>(o, 8)) return 0;
    return F<uint8_t>(F<uint8_t*>(o, 0xc + i * 4), 0x1d8) == 0 ? 1 : 0;
}
RT3_REPLACE(0x4752b0, Obj_EntryClear1d8);

void __fastcall Nop476080(void*) {}
RT3_REPLACE(0x476080, Nop476080);
void __fastcall Nop476090(void*) {}
RT3_REPLACE(0x476090, Nop476090);

// 0x4760a0. As 0x424050 for the slots +0x2ab with last key +0xc43.
void __fastcall Obj_SetSlot2ab(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xc43)) return;
    if (add) F<int32_t>(o, 0x2ab + idx * 4) += value;
    else F<int32_t>(o, 0x2ab + idx * 4) = value;
    if (key) F<int32_t>(o, 0xc43) = key;
}
RT3_REPLACE(0x4760a0, Obj_SetSlot2ab);

// 0x4760f0. Claims a random free slot of the 41-entry table 0x5f2d28
// (stride 0x27); after 100 probes the probed slot is taken anyway.
int32_t ClaimRandomSlot() {
    int32_t r = SimRand(0, 0x28);
    uint8_t* t = (uint8_t*)0x5f2d28;
    for (int32_t probes = 0;; ++probes) {
        if (probes > 100) t[r * 0x27] = 0;
        if (!t[r * 0x27]) break;
        r = (r + 1) % 0x29;
    }
    t[r * 0x27] = 1;
    return r;
}
RT3_REPLACE(0x4760f0, ClaimRandomSlot);

} // namespace rt3
