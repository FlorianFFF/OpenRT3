// Block 0x42xxxx, small functions (batch 17). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static uint8_t*& g_world     = hook::Global<uint8_t*>(0x6cec78);
static uint8_t*& g_newGame   = hook::Global<uint8_t*>(0x6cec7c);

static inline double D(uint32_t va) { return *(const double*)va; }

int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y);
const char* __fastcall GetString(int32_t id);

// 0x420350. Age factor +0x31b from the years since +0x317: rises for the
// first 3 years, 1.0 up to year 6, then falls (capped at 20 years). Halved
// towards 1 when +0x23e is set.
void __fastcall Obj_UpdateAgeFactor(void* o) {
    int32_t since = F<int32_t>(o, 0x317);
    if (since == 0) {
        F<float>(o, 0x31b) = 1.0f;
    } else {
        int32_t age = F<uint16_t>(g_world, 0xd) - since;
        if (age < 0) age = 0;
        if (age > 20) age = 20;
        if (age < 3) F<float>(o, 0x31b) = (float)((double)(3 - age) * D(0x5c86b8) + D(0x5c8770));
        else if (age < 6) F<float>(o, 0x31b) = 1.0f;
        else F<float>(o, 0x31b) = (float)(D(0x5c8770) - (double)(age - 6) * D(0x5c9aa0));
    }
    if (F<int32_t>(o, 0x23e))
        F<float>(o, 0x31b) = (float)(((double)F<float>(o, 0x31b) + D(0x5c8770)) / D(0x5c85d0));
}
RT3_REPLACE(0x420350, Obj_UpdateAgeFactor);

// 0x421430 / 0x421470. Collection subclass 0x5c9ae0 (heap records, 10 + 20; +0x88 = 0).
Collection* __fastcall Collection5c9ae0_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9ae0;
    F<int32_t>(c, 0x88) = 0;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x421430, Collection5c9ae0_Construct);
void __fastcall Collection5c9ae0_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9ae0;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x421470, Collection5c9ae0_Destruct);

// 0x4215e0. As 0x41d4c0 with tags 0x5209/0x520a/0x520b.
void __fastcall Collection_SaveWithRecords5209(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4215e0, Collection_SaveWithRecords5209);

// 0x4220b0. Name (+0x356) of the nearest record to (x, y); otherwise string
// 0xd0 copied into the static buffer 0x62ba90.
RT3_NOINLINE const char* __fastcall Table_NearestName(Collection* t, int, float x, float y) {
    int32_t id = Table_NearestValue23a(t, 0, x, y);
    if (id) return (const char*)Collection_Get(t, 0, id) + 0x356;
    const char* s = GetString(0xd0);
    char* d = (char*)0x62ba90;
    while ((*d++ = *s++) != 0) {}
    return (const char*)0x62ba90;
}
RT3_REPLACE(0x4220b0, Table_NearestName);

// 0x423ef0. Revenue bookkeeping: amount `a`, units `b` at price `c`, into
// slot `idx` and the totals; `flag` selects the +0x32f or +0x33b group.
void __fastcall Obj_AddRevenue(void* o, int, float a, float b, float c, int32_t flag, int32_t idx) {
    F<float>(o, 0x24f + idx * 4) = (float)((double)a + F<float>(o, 0x24f + idx * 4));
    F<float>(o, 0x323) = (float)((double)a + F<float>(o, 0x323));
    F<float>(o, 0x327) = (float)((double)b + F<float>(o, 0x327));
    float bc = (float)((double)b * c);
    F<float>(o, 0x32b) = (float)((double)bc + F<float>(o, 0x32b));
    uint32_t g = flag ? 0x32f : 0x33b;
    F<float>(o, g)     = (float)((double)a + F<float>(o, g));
    F<float>(o, g + 4) = (float)((double)b + F<float>(o, g + 4));
    F<float>(o, g + 8) = (float)((double)bc + F<float>(o, g + 8));
}
RT3_REPLACE(0x423ef0, Obj_AddRevenue);

void __fastcall Obj_Nop423fc0(void*) {}
RT3_REPLACE(0x423fc0, Obj_Nop423fc0);
void __fastcall Obj_IncD0f(void* o) { F<int32_t>(o, 0xd0f)++; }
RT3_REPLACE(0x423fd0, Obj_IncD0f);
void __fastcall Obj_IncD0b(void* o) { F<int32_t>(o, 0xd0b)++; }
RT3_REPLACE(0x423fe0, Obj_IncD0b);
void __stdcall SetGlobal62bae8(uint8_t v) { hook::Global<uint8_t>(0x62bae8) = v; }
RT3_REPLACE(0x423ff0, SetGlobal62bae8);
int32_t __stdcall Return0_424000(int32_t) { return 0; }
RT3_REPLACE(0x424000, Return0_424000);

// 0x424050. Sets (or adds to) slot `idx` of +0x35b unless `key` equals the
// last key +0xcf3; a non-zero key is remembered.
void __fastcall Obj_SetSlot35b(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xcf3)) return;
    if (add) F<int32_t>(o, 0x35b + idx * 4) += value;
    else F<int32_t>(o, 0x35b + idx * 4) = value;
    if (key) F<int32_t>(o, 0xcf3) = key;
}
RT3_REPLACE(0x424050, Obj_SetSlot35b);

// 0x4240c0. (+0x34b + 58) / 9, clamped to 0..12.
int32_t __fastcall Obj_Level34b(void* o) {
    int32_t v = (F<int32_t>(o, 0x34b) + 0x3a) / 9;
    if (v <= 0) v = 0;
    return v < 12 ? v : 12;
}
RT3_REPLACE(0x4240c0, Obj_Level34b);

// 0x424140. Blend of the difficulty tables 0x5f3cf8 / 0x5f3d08 by +0xd31.
float __fastcall Obj_DifficultyBlend(void* o) {
    uint32_t d = F<uint8_t>(g_newGame, 0x7d);
    const float* lo = (const float*)0x5f3cf8;
    const float* hi = (const float*)0x5f3d08;
    double x = F<float>(o, 0xd31);
    return (float)((D(0x5c8770) - x) * lo[d] + (double)hi[d] * x);
}
RT3_REPLACE(0x424140, Obj_DifficultyBlend);

} // namespace rt3
