// Functions across blocks 0x47..0x4e (batch 62). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <initializer_list>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_Sum3(void* o, int, int32_t i);

// 0x477af0. Initialises the minimap/graph view settings; scale +0x11f is
// the smaller map side, at most 160.
void __fastcall MapView_Init(uint8_t* v) {
    F<uint16_t>(v, 6) = 0x1000;
    v[0xa2] = 0;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t w = F<int32_t>(map, 0x2155), h = F<int32_t>(map, 0x2159);
    int32_t m = w < h ? w : h;
    F<int32_t>(v, 0x11f) = m > 0xa0 ? 0xa0 : m;
    for (uint32_t off : { 0xdfu, 0xe3u, 0xe7u, 0xebu }) F<int32_t>(v, off) = 5;
    for (uint32_t off : { 0xbbu, 0xbfu, 0xcbu, 0xcfu }) F<int32_t>(v, off) = 10;
    for (uint32_t off : { 0xafu, 0xa3u, 0xa7u, 0xabu, 0xb3u, 0xb7u, 0xc3u, 0xc7u }) F<int32_t>(v, off) = 0;
    for (uint32_t off : { 0xdbu, 0xd3u, 0xd7u }) F<float>(v, off) = 1.0f;
    for (uint32_t k = 0; k < 5; ++k) {
        F<int32_t>(v, 0xef + k * 8) = 0;
        F<float>(v, 0xf3 + k * 8) = 1.0f;
    }
    F<int32_t>(v, 0x113) = 0;
    F<int32_t>(v, 0x117) = 0;
    F<int32_t>(v, 0x11b) = 0;
}
RT3_REPLACE(0x477af0, MapView_Init);

// 0x47db30. Records value v for entry i of list `list` (three 5-byte
// {float, used} lists +0x24, count +0x30) keeping the minimum; the lists
// grow to i + 3 entries (new entries 0x4de67b70 = 4.8e8) when needed.
void __fastcall Site_RecordMin(uint8_t* o, int, int32_t i, int32_t list, float v) {
    if (i >= F<int32_t>(o, 0x30)) {
        int32_t newBytes = (i + 3) * 5;
        for (int k = 0; k < 3; ++k) {
            int32_t oldBytes = F<int32_t>(o, 0x30) * 5;
            uint8_t* nb = (uint8_t*)Malloc(newBytes);
            if (oldBytes) {
                const uint8_t* old = F<uint8_t*>(o, 0x24 + k * 4);
                for (int32_t b = 0; b < oldBytes; ++b) nb[b] = old[b];
                Free(F<void*>(o, 0x24 + k * 4));
            }
            F<uint8_t*>(o, 0x24 + k * 4) = nb;
            for (int32_t b = oldBytes; b < newBytes; ++b) nb[b] = 0;
            for (int32_t j = F<int32_t>(o, 0x30); j < i + 3; ++j) F<uint32_t>(nb, j * 5) = 0x4de67b70;
        }
        F<int32_t>(o, 0x30) = i + 3;
    }
    uint8_t* e = F<uint8_t*>(o, 0x24 + list * 4) + i * 5;
    if (!e[4] || F<float>(e, 0) > v) {
        F<float>(e, 0) = v;
        e[4] = 1;
    }
}
RT3_REPLACE(0x47db30, Site_RecordMin);

// 0x4e0730. Takes the selected list item's command ID (0x7918..0x7921) from
// the dialog 0x6d401c (+0xc6d) into +0x78 and marks the item.
void __fastcall Obj_TakeListCommand(uint8_t* o) {
    F<int32_t>(o, 0x78) = 0;
    uint8_t* item = F<uint8_t*>(hook::Global<uint8_t*>(0x6d401c), 0xc6d);
    if (!item) return;
    uint16_t id = F<uint16_t>(item, 4);
    if (!id || id < 0x7918 || id > 0x7921) return;
    F<int32_t>(o, 0x78) = id;
    F<uint32_t>(F<uint8_t*>(hook::Global<uint8_t*>(0x6d401c), 0xc6d), 0x33) |= 0x20;
}
RT3_REPLACE(0x4e0730, Obj_TakeListCommand);

int32_t Global6d10a8Set() { return hook::Global<void*>(0x6d10a8) ? 1 : 0; }
RT3_REPLACE(0x4e1f50, Global6d10a8Set);
void SetGlobal6d10a4() { hook::Global<int32_t>(0x6d10a4) = 1; }
RT3_REPLACE(0x4e1f60, SetGlobal6d10a4);

// 0x4e52e0. Train lifetime total of the three stats (this year + totals), unrounded.
double __fastcall Train_LifetimeTotal(uint8_t* t) {
    double v = Obj_Sum3(t, 0, 0) + F<float>(t, 0x105);
    v = v + F<float>(t, 0x101);
    return v + F<float>(t, 0xfd);
}
RT3_REPLACE(0x4e52e0, Train_LifetimeTotal);

void __fastcall Obj_Set78(uint8_t* o, int, int32_t v) { F<int32_t>(o, 0x78) = v; }
RT3_REPLACE(0x4ea710, Obj_Set78);
int32_t Global6d10b0Set() { return hook::Global<void*>(0x6d10b0) ? 1 : 0; }
RT3_REPLACE(0x4ea720, Global6d10b0Set);

// 0x4eb230. Copies 0x2a bytes of this object into the global 0x6d10d8.
void __fastcall Obj_CopyTo6d10d8(const uint8_t* o) {
    uint8_t* d = (uint8_t*)0x6d10d8;
    for (int k = 0; k < 0x2a; ++k) d[k] = o[k];
}
RT3_REPLACE(0x4eb230, Obj_CopyTo6d10d8);

// 0x4ecb20. Clears the new-game player slots and the lobby buffer 0x6d1270.
void NewGameConfig_ClearSlots() {
    uint8_t* c = hook::Global<uint8_t*>(0x6cec7c);
    c[0x97] = 1;
    F<uint16_t>(c, 0x79) = 0;
    F<uint16_t>(c, 0x7b) = 0;
    for (int k = 0; k < 16; ++k) c[0x87 + k] = 0;
    for (int k = 0; k < 0x33; ++k) c[0x44 + k] = 0;
    if (uint8_t* l = hook::Global<uint8_t*>(0x6d1270))
        for (int k = 0; k < 0x100f0 * 4 + 2; ++k) l[k] = 0;
}
RT3_REPLACE(0x4ecb20, NewGameConfig_ClearSlots);

// 0x4ecb90. Free AI slot check: mode 1 tests whether an unassigned slot with
// a lobby entry exists (0xff); otherwise claims the highest one (slot id
// k + 100, +0x7b++). 0 when none (or in mode 0x6cd914 == 1).
int32_t __fastcall NewGameConfig_ClaimAiSlot(int32_t mode) {
    if (hook::Global<int32_t>(0x6cd914) == 1) return 0;
    uint8_t* c = hook::Global<uint8_t*>(0x6cec7c);
    uint8_t* l = hook::Global<uint8_t*>(0x6d1270);
    if (mode == 1) {
        if (!l) return 0;
        int32_t n7b = F<uint16_t>(c, 0x7b), n79 = F<uint16_t>(c, 0x79);
        if (F<int32_t>(l, 0x3ae) <= n79 + n7b || F<int32_t>(l, 0x3be) <= n7b) return 0;
        for (int32_t k = 15; k >= 0; --k)
            if (!c[0x87 + k] && l[0x31c + k * 9]) return 0xff;
        return 0;
    }
    for (int32_t k = 15; k >= 0; --k) {
        if (c[0x87 + k] || !l[0x31c + k * 9]) continue;
        c[0x87 + k] = (uint8_t)(k + 100);
        F<uint16_t>(hook::Global<uint8_t*>(0x6cec7c), 0x7b)++;
        return 0xff;
    }
    return 0;
}
RT3_REPLACE(0x4ecb90, NewGameConfig_ClaimAiSlot);

} // namespace rt3
