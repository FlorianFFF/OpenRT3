// Functions across blocks 0x45..0x47 (batch 33). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_companies = hook::Global<Collection*>(0x62be10);

// 0x45a380. Starts a block iteration of o (see 0x45a660) and tests `mask`
// against the flags +0x8c of the first block. QUIRK: a negative index walks
// 2^31 blocks back as in the original.
int32_t __fastcall BlockIter_BeginTest(uint8_t* o, uint32_t mask, int32_t idx) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<uint8_t*>(0x6acd50) = o;
    hook::Global<int32_t>(0x6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    hook::Global<uint8_t*>(0x6acd4c) = p;
    uint32_t u = (uint32_t)idx;
    if ((int32_t)u < 0) {
        do {
            --u;
            p += 0x168;
        } while ((int32_t)u < 0);
    }
    return (F<uint32_t>(p, 0x8c) & mask) ? 1 : 0;
}
RT3_REPLACE(0x45a380, BlockIter_BeginTest);

// 0x473e70. Record with the lowest serial +0x14 (skipping flagged +0x18 ones unless `all`); 0 if none.
void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all) {
    uint32_t sel = 0;
    int32_t best = 0x7fffffff;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x14) >= best) continue;
        if (!all && r[0x18]) continue;
        sel = F<uint32_t>(r, 0);
        best = F<int32_t>(r, 0x14);
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x473e70, Table_Oldest14);

// 0x4768c0. Whether this player's holding +0x15d[id] equals company id's total +0x47 (sole owner).
int32_t __fastcall Player_OwnsAllOf(uint8_t* p, int, uint32_t id) {
    if (!id) return 0;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    if (!c) return 0;
    return F<int32_t>(c, 0x47) == F<int32_t>(p, 0x15d + id * 4) ? 1 : 0;
}
RT3_REPLACE(0x4768c0, Player_OwnsAllOf);

// 0x476900. Penalty -7 per year (up to 10) since company +0x1dd's founding (+0x43) short of 10.
int32_t __fastcall Player_CompanyAgePenalty(uint8_t* p) {
    uint32_t id = F<uint32_t>(p, 0x1dd);
    if (!id) return 0;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    int32_t d = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(c, 0x43);
    if (d <= 0) d = 0;
    if (d >= 10) d = 10;
    return (d - 10) * 7;
}
RT3_REPLACE(0x476900, Player_CompanyAgePenalty);

// 0x477740 / 0x477770. Players collection (vtbl 0x5ce828, inline 0xcab-byte records, 4 + 2).
Collection* __fastcall Players_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce828;
    Collection_Init(c, 0, 1, 0xcab, 4, 2, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x477740, Players_Construct);
void __fastcall Players_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce828;
    Collection_Destruct(c);
}
RT3_REPLACE(0x477770, Players_Destruct);

// 0x477780. Players load framed by 0x5209/0x520a/0x520b.
int32_t __fastcall Players_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x5209, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x520a, 0, 0);
    return StreamSkip4(s, 0x520b, 0, 0) + n;
}
RT3_REPLACE(0x477780, Players_Load);

void __fastcall Players_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4777e0, Players_Save);

} // namespace rt3
