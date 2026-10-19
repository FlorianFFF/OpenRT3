// Block 0x42xxxx, functions (batch 20). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_cargoTypes  = hook::Global<Collection*>(0x62ba8c);

static inline double D(uint32_t va) { return *(const double*)va; }

void* __fastcall Obj_Ref3cc(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall Company_ClampDates(void* c, int, int32_t);
int32_t __fastcall Chain_MinLevel(void* o, int, int32_t i);
void* __fastcall Obj_FirstActiveInChain(void* o);

// 0x420560. Recomputes +0x312 from the buildings in the chain +0x383
// (by type kind +0x32) when the type +0x23e is 0.
void __fastcall Obj_SumBuildingWeights(void* o) {
    F<int32_t>(o, 0x312) = 0;
    if (F<int32_t>(o, 0x23e)) return;
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
        switch (F<uint8_t>(t, 0x32)) {
        case 0: {
            double v = ((double)F<int32_t>(Obj_Ref3cc(b), 0x141) + D(0x5c8758)) / D(0x5c87e8);
            F<float>(o, 0x312) = (float)(v + F<float>(o, 0x312));
            break;
        }
        case 2:
            if (F<int32_t>(t, 0x78c)) {
                double v = (F<int32_t>(b, 0x246) ? 1.75 : 1.0) * D(0x5c8d68);
                F<float>(o, 0x312) = (float)(v + F<float>(o, 0x312));
            } else {
                F<float>(o, 0x312) = (float)((double)F<float>(o, 0x312) + *(const float*)0x5c8900);
            }
            break;
        case 3:
            F<float>(o, 0x312) = (float)((double)F<float>(o, 0x312) + D(0x5c85d0));
            break;
        }
    }
}
RT3_REPLACE(0x420560, Obj_SumBuildingWeights);

// 0x429af0. Companies load framed by 0x61a9/0x61aa/0x61ab; each record then
// clamps its dates. Returns the byte count.
int32_t __fastcall Companies_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x61a9, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x61aa, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        n += Company_ClampDates(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, (int32_t)s);
    return StreamSkip4(s, 0x61ab, 0, 0) + n;
}
RT3_REPLACE(0x429af0, Companies_Load);

// 0x42b3b0. Resets a cargo station record: position (+0xe7, +0xe9), flags,
// links, key list, and per cargo type the levels; stock cap +0[i] =
// cargo type +0x18 / 16.
void __fastcall Station_Reset(void* o, int, uint16_t x, uint16_t y) {
    F<uint16_t>(o, 0xe7) = x;
    F<uint16_t>(o, 0xe9) = y;
    F<uint8_t>(o, 0xe6) = 0;
    for (int k = 0; k < 7; ++k) F<uint8_t>(o, 0xd8 + k) = 0;
    for (int k = 0; k < 7; ++k) F<uint8_t>(o, 0xdf + k) = 0;
    F<uint16_t>(o, 0xd4) = 0;
    F<uint16_t>(o, 0xd6) = 0;
    F<int32_t>(o, 0xeb) = 0;
    F<int32_t>(o, 0xef) = 0;
    F<int32_t>(o, 0xf3) = 0;
    F<int32_t>(o, 0xf7) = 0;
    F<int32_t>(o, 0xfb) = 0;
    F<int32_t>(o, 0xff) = 0;
    for (int32_t i = 0; i < 0x34; ++i) {
        F<uint16_t>(o, 0x6a + i * 2) = 0;
        F<int32_t>(o, 0x103 + i * 4) = 0;
        F<uint16_t>(o, i * 2) = 0;
        if (i == 0 || !Collection_IsValid(g_cargoTypes, 0, i)) continue;
        Collection_Get(g_cargoTypes, 0, i); // the original reassigns +0x5a to itself here
        F<int32_t>(o, 0x103 + i * 4) = 0;
        float cap = F<float>(Collection_Get(g_cargoTypes, 0, i), 0x18);
        F<uint16_t>(o, i * 2) = (uint16_t)((uint32_t)(int32_t)cap >> 4);
    }
}
RT3_REPLACE(0x42b3b0, Station_Reset);

// 0x42be20. Sum of +0x103[i] over the available cargo types (i = 0..52).
float __fastcall Station_TotalAvailable(void* o) {
    const uint8_t* ct = (const uint8_t*)g_cargoTypes;
    float sum = 0.0f;
    for (int32_t i = 0; i < 0x35; ++i)
        if (F<int32_t>((void*)ct, 0x16e + i * 4)) sum = (float)((double)sum + F<float>(o, 0x103 + i * 4));
    return sum;
}
RT3_REPLACE(0x42be20, Station_TotalAvailable);

// 0x42c7d0 / 0x42c820. Searches of the site chain (u16 +0xd4, next +0x3c).
int32_t __fastcall Chain_Contains(void* o, int, uint16_t k) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; id = F<uint16_t>(Collection_Get(g_table6cec20, 0, id), 0x3c))
        if (id == k) return 1;
    return 0;
}
RT3_REPLACE(0x42c7d0, Chain_Contains);
int32_t __fastcall Chain_HasValue(void* o, int, int32_t v) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; id = F<uint16_t>(Collection_Get(g_table6cec20, 0, id), 0x3c))
        if (Obj_BuildingValue(Collection_Get(g_table6cec20, 0, id)) == v) return 1;
    return 0;
}
RT3_REPLACE(0x42c820, Chain_HasValue);
// 0x42c880 is Obj_FirstActiveInChain (objects4.cpp).
static inline void* Chain_FirstActive(void* o) { return Obj_FirstActiveInChain(o); }
int32_t __fastcall Chain_AnyActive(void* o) {
    if (!F<uint16_t>(o, 0xd4)) return 0;
    return Chain_FirstActive(o) ? 1 : 0;
}
RT3_REPLACE(0x42c8d0, Chain_AnyActive);

// 0x42cb30. With an active site, caps each unmapped available cargo's
// stock +0[i] at Chain_MinLevel / 16.
void __fastcall Station_CapStocks(void* o) {
    if (!F<uint16_t>(o, 0xd4) || !Chain_FirstActive(o)) return;
    int32_t n = Collection_Count(g_cargoTypes);
    for (int32_t i = 1; i <= n; ++i) {
        uint8_t* ct = (uint8_t*)g_cargoTypes;
        if (!F<int32_t>(ct, 0x16e + i * 4) || F<int32_t>(ct, 0x9a + i * 4) != -1) continue;
        int32_t m = Chain_MinLevel(o, 0, i) >> 4;
        int32_t cur = F<uint16_t>(o, i * 2);
        F<uint16_t>(o, i * 2) = (uint16_t)(cur < m ? cur : m);
    }
}
RT3_REPLACE(0x42cb30, Station_CapStocks);

// 0x42d5a0. Initialises a 0x88f-byte record (names, flags, four slots).
void* __fastcall Record42d5a0_Init(uint8_t* r) {
    static const uint32_t kBytes[] = { 0x18, 0xe0, 0x401, 0x4ca, 0x593, 0x65c, 0x725, 0x7ee, 0x7ef };
    for (uint32_t off : kBytes) r[off] = 0;
    F<int32_t>(r, 0x7f0) = -1;
    r[0x7f4] = 0;
    F<int32_t>(r, 0x7f5) = 0;
    r[0x7f9] = 0;
    r[0x7fa] = 0;
    F<int32_t>(r, 0x81f) = 0;
    F<int32_t>(r, 0x823) = 0;
    F<int32_t>(r, 0x827) = 0;
    r[0x82b] = 0;
    F<int32_t>(r, 0x82c) = 0;
    F<int32_t>(r, 0x830) = 0;
    for (int k = 0; k < 0x5b; ++k) r[0x834 + k] = 0;
    F<int32_t>(r, 0) = 0;
    for (int i = 0; i < 4; ++i) {
        F<int32_t>(r, 4 + i * 4) = 0;
        r[0x7fb + i] = 0;
        F<int32_t>(r, 0x80f + i * 4) = -1;
        F<int32_t>(r, 0x7ff + i * 4) = 0;
    }
    return r;
}
RT3_REPLACE(0x42d5a0, Record42d5a0_Init);

void __fastcall Record42d5a0_SetName(uint8_t* r, int, int32_t v, const char* name) {
    F<int32_t>(r, 0x14) = v;
    char* d = (char*)r + 0x18;
    while ((*d++ = *name++) != 0) {}
}
RT3_REPLACE(0x42d670, Record42d5a0_SetName);

// 0x42df30..0x42dff0. Whether any entry of the list *r (next +0x1a) has a
// kind (+0) whose flags in table 0x5f3e04 (stride 0x81) match `mask`; kind
// -1 matches on the record's own condition instead.
static inline int32_t AnyKind(uint8_t* r, uint8_t mask, int anyCond) {
    for (uint8_t* e = F<uint8_t*>(r, 0); e; e = F<uint8_t*>(e, 0x1a)) {
        int32_t k = F<int32_t>(e, 0);
        if (k >= 0 && (((const uint8_t*)0x5f3e04)[k * 0x81] & mask)) return 1;
        if (k == -1 && anyCond) return 1;
    }
    return 0;
}
int32_t __fastcall Record_AnyKind1(uint8_t* r) { return AnyKind(r, 1, r[0x7f9] != 0); }
RT3_REPLACE(0x42df30, Record_AnyKind1);
int32_t __fastcall Record_AnyKind2(uint8_t* r) { return AnyKind(r, 2, r[0x7fa] != 0); }
RT3_REPLACE(0x42df70, Record_AnyKind2);
int32_t __fastcall Record_AnyKind4(uint8_t* r) { return AnyKind(r, 4, F<int32_t>(r, 0x7f0) != 0x63); }
RT3_REPLACE(0x42dfb0, Record_AnyKind4);
int32_t __fastcall Record_AnyKind7(uint8_t* r) {
    return AnyKind(r, 7, r[0x7f9] || r[0x7fa] || F<int32_t>(r, 0x7f0) != 0x63);
}
RT3_REPLACE(0x42dff0, Record_AnyKind7);

} // namespace rt3
