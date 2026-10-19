// Block 0x41xxxx, more functions (batch 14). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trackNodes  = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_table62ba88 = hook::Global<Collection*>(0x62ba88);

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int __fastcall ToUpper(char c);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
int32_t __stdcall Bracket5(int32_t value);
void __fastcall Obj_SetPriority(void* o);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY);

// 0x412ab0. Obj_SetPriority on every record.
void __fastcall Table_SetPriorities(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_SetPriority(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x412ab0, Table_SetPriorities);

// 0x412ca0. Weighted random pick (SimRand) of a kind-3 type name for size
// bracket Bracket5(value): weights +0x8d[b], total +0x8c[b]. Falls back to
// "Commercial" (0x5c92e8) after 5 passes.
void __fastcall Types_PickName(Collection* t, int, char* dst, int32_t value) {
    int32_t b = Bracket5(value);
    int32_t r = SimRand(0, F<int32_t>(t, 0x8c + b * 4) - 1);
    for (int32_t pass = 0; pass < 5; ++pass) {
        for (int32_t i = 0; i < Collection_Count(t); ++i) {
            uint8_t* rec = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
            if (F<uint8_t>(rec, 0x32) != 3) continue;
            r -= F<int32_t>(rec, 0x8d + b * 4);
            if (r < 0) {
                const char* s = (const char*)rec + 4;
                while ((*dst++ = *s++) != 0) {}
                return;
            }
        }
    }
    const uint8_t* def = (const uint8_t*)0x5c92e8; // "Commercial" (11 bytes)
    for (int k = 0; k < 11; ++k) dst[k] = def[k];
}
RT3_REPLACE(0x412ca0, Types_PickName);

// 0x414fa0. Gathers the float grid values inside the marked bounding box
// (0x62b7a8..0x62b7b4) whose mask bit is set; returns how many.
int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t) {
    int32_t n = 0;
    for (int32_t y = hook::Global<int32_t>(0x62b7b0); y <= hook::Global<int32_t>(0x62b7b4); ++y) {
        for (int32_t x = hook::Global<int32_t>(0x62b7a8); x <= hook::Global<int32_t>(0x62b7ac); ++x) {
            uint8_t* map = hook::Global<uint8_t*>(0x62c120);
            if (!(F<uint8_t*>(map, 0x2135)[(F<int32_t>(map, 0x2155) + 1) * y + x] & 1)) continue;
            out[n++] = (float)Map_GridFloat(map, 0, x, y);
        }
    }
    return n;
}
RT3_REPLACE(0x414fa0, Map_GatherMasked);

// 0x417610 / 0x4176d0. Records named `name` (+0x22) whose +0xc3 has `bit`
// (-1 = any): count / ID of the n-th (1-based).
int32_t __fastcall Table_CountNamedWithBit(Collection* t, int, const char* name, int32_t bit) {
    char first = (char)ToUpper(name[0]);
    int32_t mask = bit == -1 ? -1 : 1 << (bit & 31);
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)ToUpper((char)r[0x22]) != first) continue;
        if (!(F<int32_t>(r, 0xc3) & mask)) continue;
        if (ExeStricmp((const char*)r + 0x22, name) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x417610, Table_CountNamedWithBit);
uint32_t __fastcall Table_NthNamedWithBit(Collection* t, int, const char* name, int32_t bit, int32_t nth) {
    char first = (char)ToUpper(name[0]);
    int32_t mask = bit == -1 ? -1 : 1 << (bit & 31);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)ToUpper((char)r[0x22]) != first) continue;
        if (!(F<int32_t>(r, 0xc3) & mask)) continue;
        if (ExeStricmp((const char*)r + 0x22, name) != 0) continue;
        if (--nth <= 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x4176d0, Table_NthNamedWithBit);

// 0x417a70. Footprint offset i of record `id`, rotated by `angle`:
// x = f3[i]/48 - b8/2, y = 10b[i]/48 - b9/2 (both floats), then RotateNegated.
void __fastcall Type_RotatedOffset(Collection* t, int, uint32_t id, int32_t i, float* outX, float* outY, float angle) {
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    const double c48 = *(const double*)0x5c96c8, c2 = *(const double*)0x5c85d0;
    float y = (float)((double)F<int16_t>(r, 0x10b + i * 2) / c48 - (double)F<uint8_t>(r, 0xb9) / c2);
    float x = (float)((double)F<int16_t>(r, 0xf3 + i * 2) / c48 - (double)F<uint8_t>(r, 0xb8) / c2);
    RotateNegated(x, y, angle, outX, outY);
}
RT3_REPLACE(0x417a70, Type_RotatedOffset);

// 0x419200. Per-cell value from the buffer 0x62b300 (0 if absent).
int32_t __stdcall CellBuffer_Get(int32_t x, int32_t y) {
    int32_t* b = hook::Global<int32_t*>(0x62b300);
    if (!b) return 0;
    return b[F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155) * y + x];
}
RT3_REPLACE(0x419200, CellBuffer_Get);

void __fastcall Obj_Toggle35a(void* o) { F<int32_t>(o, 0x35a) = F<int32_t>(o, 0x35a) == 0 ? 1 : 0; }
RT3_REPLACE(0x41ab30, Obj_Toggle35a);

// 0x41afc0. Removes this vehicle's link (+0x390, +0x34a) from its track node
// (+0x356) and that node's three neighbours.
void __fastcall Obj_UnlinkFromNode(void* o) {
    if (F<int32_t>(o, 0x356) == -1) return;
    uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 0x356));
    Node_ClearLinks(n, 0, F<int32_t>(o, 0x390), F<int32_t>(o, 0x34a));
    static const uint32_t kNeighbours[3] = { 0x206, 0x20a, 0x20e };
    for (uint32_t off : kNeighbours) {
        int32_t id = F<int32_t>(n, off);
        if (id == -1) continue;
        Node_ClearLinks(Collection_Get(g_trackNodes, 0, id), 0, F<int32_t>(o, 0x390), F<int32_t>(o, 0x34a));
    }
}
RT3_REPLACE(0x41afc0, Obj_UnlinkFromNode);

double __fastcall Obj_Ref388Float10d(void* o) {
    return F<float>(Collection_Get(g_table62ba88, 0, F<uint32_t>(o, 0x388)), 0x10d);
}
RT3_REPLACE(0x41b910, Obj_Ref388Float10d);

// 0x41b930. Virtual slot 5 of this vehicle's track node (+0x356); 0 if none.
uint32_t __fastcall Obj_NodeVirtual5(void* o) {
    if (F<int32_t>(o, 0x356) == -1) return 0;
    void* n = Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 0x356));
    return ((uint32_t(__thiscall*)(void*))(*(void***)n)[0x14 / 4])(n);
}
RT3_REPLACE(0x41b930, Obj_NodeVirtual5);

// 0x41b960. Cargo type (+0x38c) record, or 0.
void* __fastcall Obj_CargoType(void* o) {
    uint32_t id = F<uint32_t>(o, 0x38c);
    if (!id) return nullptr;
    return Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, id);
}
RT3_REPLACE(0x41b960, Obj_CargoType);

// 0x41b980. Sets +0x39c; a value <= 0 becomes 1000..8000 (HashRand on +0x34a)
// when option +0x130 is on.
void __fastcall Obj_SetAmount39c(void* o, int, float v) {
    F<float>(o, 0x39c) = v;
    if (!(v <= *(const float*)0x5c8598)) return;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130)) return;
    F<float>(o, 0x39c) = (float)(HashRand(F<int32_t>(o, 0x34a), 1, 8) * 1000);
}
RT3_REPLACE(0x41b980, Obj_SetAmount39c);

// 0x41d2f0 / 0x41d320. Collection subclass 0x5c9810 (heap records, 10 + 20).
Collection* __fastcall Collection5c9810_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9810;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x41d2f0, Collection5c9810_Construct);
void __fastcall Collection5c9810_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9810;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x41d320, Collection5c9810_Destruct);

} // namespace rt3
