// Functions across blocks 0x42..0x47 (batch 38). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

void __fastcall Obj_Set3c(void* o, int, uint16_t v);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);

// 0x42bbb0 / 0x42bbf0. Site chain (u16 head +0xd4, next +0x3c): push id at
// the front / unlink id (gives up after 1000 links).
void __fastcall Chain_Push(void* o, int, uint16_t id) {
    Obj_Set3c(Collection_Get(g_table6cec20, 0, id), 0, F<uint16_t>(o, 0xd4));
    F<uint16_t>(o, 0xd4) = id;
}
RT3_REPLACE(0x42bbb0, Chain_Push);
void __fastcall Chain_Remove(void* o, int, uint16_t id) {
    uint32_t prev = 0, cur = F<uint16_t>(o, 0xd4);
    for (int32_t n = 0; cur != id; ) {
        if (n++ > 1000) return;
        prev = cur;
        cur = F<uint16_t>(Collection_Get(g_table6cec20, 0, cur), 0x3c);
    }
    uint16_t next = F<uint16_t>(Collection_Get(g_table6cec20, 0, id), 0x3c);
    if (prev) Obj_Set3c(Collection_Get(g_table6cec20, 0, prev), 0, next);
    else F<uint16_t>(o, 0xd4) = next;
}
RT3_REPLACE(0x42bbf0, Chain_Remove);

// 0x43b2c0. Fills slot i of the 48-byte array +0x10e: in use, world +0x45 /
// +0x49 and nine of this object's fields.
void __fastcall Obj_FillEntry10e(uint8_t* o, int, int32_t i) {
    uint8_t* e = o + i * 0x30 + 0x10e;
    F<int32_t>(e, 0) = 1;
    F<uint32_t>(e, 4) = F<uint32_t>(g_world, 0x45);
    F<uint32_t>(e, 8) = F<uint32_t>(g_world, 0x49);
    static const uint8_t kSrc[9] = { 0x4, 0x8, 0xc, 0x18, 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (int k = 0; k < 9; ++k) F<uint32_t>(e, 0xc + k * 4) = F<uint32_t>(o, kSrc[k]);
}
RT3_REPLACE(0x43b2c0, Obj_FillEntry10e);

// 0x473ce0. Initialises a 0x187-byte news entry: kind, title (0x78),
// text (0xf0), stamps the current date and the next serial 0x6cea48.
void __fastcall News_Init(uint8_t* e, int, int32_t kind, const char* title, const char* text, int32_t a, int32_t b) {
    F<int32_t>(e, 0) = kind;
    StrCopyN((char*)e + 0x19, title, 0x78);
    StrCopyN((char*)e + 0x92, text, 0xf0);
    F<int32_t>(e, 0x183) = b;
    F<int32_t>(e, 8) = DateToMinutes((const GameDate*)(g_world + 0xd));
    F<uint16_t>(e, 0xc) = F<uint16_t>(g_world, 0xd);
    e[0xe] = g_world[0xf];
    e[0x10] = g_world[0x11];
    e[0x18] = 0;
    F<int32_t>(e, 4) = a;
    F<int32_t>(e, 0x14) = hook::Global<int32_t>(0x6cea48);
    hook::Global<int32_t>(0x6cea48)++;
}
RT3_REPLACE(0x473ce0, News_Init);

// 0x47dc50. Value of entry i of list `list` (+0x24, 5-byte {value, used}); 1 if present.
int32_t __fastcall Obj_ListEntry(uint8_t* o, int, int32_t i, int32_t list, int32_t* out) {
    if (i >= F<int32_t>(o, 0x30)) return 0;
    uint8_t* e = F<uint8_t*>(o, 0x24 + list * 4) + i * 5;
    if (!e[4]) return 0;
    *out = F<int32_t>(e, 0);
    return 1;
}
RT3_REPLACE(0x47dc50, Obj_ListEntry);

// 0x47dca0..0x47dd10. Five arrays of 8-byte {key, value} pairs (+0x5a4,
// counts +0x590, total +0x5b8).
int32_t __fastcall Pairs_Total(uint8_t* o) { return F<int32_t>(o, 0x5b8); }
RT3_REPLACE(0x47dca0, Pairs_Total);
RT3_NOINLINE uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i) { return F<uint8_t*>(o, 0x5a4 + list * 4) + i * 8 - 8; }
RT3_REPLACE(0x47dcb0, Pairs_At);
void __fastcall Pairs_Clear(uint8_t* o) {
    for (int k = 0; k < 5; ++k) {
        void*& p = F<void*>(o, 0x5a4 + k * 4);
        if (p) {
            Free(p);
            p = nullptr;
        }
        F<int32_t>(o, 0x590 + k * 4) = 0;
    }
    F<int32_t>(o, 0x5b8) = 0;
}
RT3_REPLACE(0x47dcd0, Pairs_Clear);
void __fastcall Pairs_RemoveKey(uint8_t* o, int, int32_t key) {
    for (int k = 0; k < 5; ++k) {
        int32_t& count = F<int32_t>(o, 0x590 + k * 4);
        for (int32_t j = count - 1; j >= 0; --j) {
            uint8_t* buf = F<uint8_t*>(o, 0x5a4 + k * 4);
            uint8_t* e = buf + j * 8;
            if (F<int32_t>(e, 0) != key) continue;
            if (count == 1) {
                Free(buf);
                F<void*>(o, 0x5a4 + k * 4) = nullptr;
            } else if (count - j - 1 > 0) {
                ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(e, e + 8, (count - j - 1) * 8);
            }
            --count;
            F<int32_t>(o, 0x5b8)--;
        }
    }
}
RT3_REPLACE(0x47dd10, Pairs_RemoveKey);

// 0x47dda0. Unless its track node (+8) reports 0x48a090(1), calls the node's
// virtual slot 1 (`flag`) or 2.
uint32_t __fastcall Obj_NodeNotify(uint8_t* o, int, int32_t flag) {
    int32_t n = F<int32_t>(o, 8);
    if (n == -1) return (uint32_t)n;
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    if (!Collection_IsValid(nodes, 0, n)) return 0;
    void* node = Collection_Get(nodes, 0, F<uint32_t>(o, 8));
    uint32_t r = ((uint32_t(__thiscall*)(void*, int32_t))0x48a090)(node, 1);
    if (r) return r;
    return ((uint32_t(__thiscall*)(void*))(*(void***)node)[flag ? 1 : 2])(node);
}
RT3_REPLACE(0x47dda0, Obj_NodeNotify);

// 0x47de50 / 0x47de90. Building (+4) position in quarter cells. QUIRK: no
// building reads through null as in the original.
static inline void* BuildingOf4(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return id ? Collection_Get(g_buildings, 0, id) : nullptr;
}
int32_t __fastcall Obj_BuildingQuarterX(void* o) { return (int32_t)(Obj_CellX(BuildingOf4(o)) / *(const float*)0x5c8568); }
RT3_REPLACE(0x47de50, Obj_BuildingQuarterX);
int32_t __fastcall Obj_BuildingQuarterY(void* o) { return (int32_t)(Obj_CellY(BuildingOf4(o)) / *(const float*)0x5c8568); }
RT3_REPLACE(0x47de90, Obj_BuildingQuarterY);

// 0x47f0e0. Region record (0x6cfc9c) at the building's cell.
void* __fastcall Obj_BuildingRegionAt(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    int32_t yi = (int32_t)Obj_CellY(b1);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    int32_t xi = (int32_t)x;
    return Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, row[xi] >> 3);
}
RT3_REPLACE(0x47f0e0, Obj_BuildingRegionAt);

} // namespace rt3
