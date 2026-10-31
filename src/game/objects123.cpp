// Building upkeep / cargo era lists / site placement / effect multipliers in blocks 0x40..0x43 (batch 126). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
int32_t __stdcall Site_FootprintFlat(const int32_t* pos, int32_t shape);
void* __fastcall FindStaticRecord(void*, int, uint8_t* obj);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x40d380. Charges a building's upkeep for its class (type +0xb9: 0/1/2 ->
// effect 0x68/0x66/0x67 x 0x5c8cf8/0x5c8cfc/0x5c87c0) to its owner company (stat 3)
// and accumulates it in +0x310.
void __fastcall Building_ChargeUpkeep(uint8_t* b) {
    uint8_t* type = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    int32_t cls = (int8_t)type[0xb9];
    uint8_t* comp = nullptr;
    uint32_t player = 0;
    float cost = 0.0f;
    if (F<uint32_t>(b, 0x276)) {
        comp = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(b, 0x276));
        if (comp) player = F<uint32_t>(comp, 0x3b);
    }
    static const int32_t effect[3] = { 0x68, 0x66, 0x67 };
    static const uint32_t scale[3] = { 0x5c8cf8, 0x5c8cfc, 0x5c87c0 };
    if (cls >= 0 && cls <= 2) {
        float y = (float)Obj_CellY(b);
        float x = (float)Obj_CellX(b);
        uint32_t region = Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
        float m = Effects_Multiplier(World(), 0, effect[cls], F<uint32_t>(b, 0x276), player, region);
        cost = (float)((double)m * *(const float*)scale[cls]);
    }
    if (comp) Company_PostStat(comp, 0, 3, cost, 1);
    F<float>(b, 0x310) = (float)((double)cost + F<float>(b, 0x310));
}
RT3_REPLACE(0x40d380, Building_ChargeUpkeep);

// 0x411ee0. Rebuilds the lists of cargo types the industry (+0x37: +0x33 entries
// of 0xbc bytes with cargo items +0x44, 0x1c each) can ever (+0x79c) and currently
// (+0x7a0, within the entry's years +0x20..+0x22) handle.
void __fastcall Industry_BuildCargoLists(uint8_t* o) {
    FreeAndNull((void**)(o + 0x79c));
    FreeAndNull((void**)(o + 0x7a0));
    F<int32_t>(o, 0x7a4) = 0;
    F<int32_t>(o, 0x7a8) = 0;
    uint8_t marks[0x35] = {};
    int32_t year = World() ? F<uint16_t>(World(), 0xd) : 0x709;
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0x37) + i * 0xbc;
        int32_t n = F<int32_t>(e, 0x2c);
        if (n <= 0) continue;
        int32_t from = F<int16_t>(e, 0x20);
        for (int32_t j = 0; j < n; ++j) {
            int32_t k = F<int32_t>(e, 0x44 + j * 0x1c);
            int32_t v = (year >= from && year <= F<int16_t>(F<uint8_t*>(o, 0x37) + i * 0xbc, 0x22)) ? 2 : 1;
            if (!(marks[k] > v)) marks[k] = (uint8_t)v;
        }
    }
    for (int k = 0; k < 0x35; ++k) {
        if (marks[k] >= 1) F<int32_t>(o, 0x7a4)++;
        if (marks[k] >= 2) F<int32_t>(o, 0x7a8)++;
    }
    F<uint16_t*>(o, 0x79c) = (uint16_t*)Malloc(F<int32_t>(o, 0x7a4) * 2 + 1);
    F<uint16_t*>(o, 0x7a0) = (uint16_t*)Malloc(F<int32_t>(o, 0x7a8) * 2 + 1);
    for (int32_t k = 0; k < F<int32_t>(o, 0x7a4); ++k) F<uint16_t*>(o, 0x79c)[k] = 0;
    for (int32_t k = 0; k < F<int32_t>(o, 0x7a8); ++k) F<uint16_t*>(o, 0x7a0)[k] = 0;
    if (F<int32_t>(o, 0x7a4) <= 0 && F<int32_t>(o, 0x7a8) <= 0) return;
    int32_t a = 0, b = 0;
    for (int k = 0; k < 0x35; ++k) {
        if (marks[k] >= 1) F<uint16_t*>(o, 0x79c)[a++] = (uint16_t)k;
        if (marks[k] >= 2) F<uint16_t*>(o, 0x7a0)[b++] = (uint16_t)k;
    }
}
RT3_REPLACE(0x411ee0, Industry_BuildCargoLists);

// 0x415570. Chooses a footprint shape (0x62ba3a) for placing obj at 0x62b7a8: the
// preferred one (0x5ee030, consumed) or a random one, else the first that fits.
int32_t __stdcall Site_ChooseShape(uint8_t* obj) {
    if (!hook::Global<uint32_t*>(0x62b300)) {
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        uint32_t n = (uint32_t)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155)) << 2;
        uint8_t* p = (uint8_t*)Malloc(n);
        hook::Global<uint8_t*>(0x62b300) = p;
        for (uint32_t k = 0; k < n; ++k) p[k] = 0;
    }
    hook::Global<int32_t>(0x62ba3a) = -1;
    void* rec = FindStaticRecord(nullptr, 0, obj);
    hook::Global<void*>(0x62ba3e) = rec;
    if (!rec) return 0;
    const int32_t* pos = (const int32_t*)0x62b7a8;
    int32_t pref = hook::Global<int32_t>(0x5ee030);
    if (pref != -1) {
        if (!Site_FootprintFlat(pos, pref)) {
            for (uint32_t s = 0; s < 0xc; ++s)
                if (Site_FootprintFlat(pos, s) == 1) {
                    hook::Global<int32_t>(0x5ee030) = -1;
                    hook::Global<int32_t>(0x62ba3a) = (int32_t)s;
                    return 1;
                }
        }
        int32_t p = hook::Global<int32_t>(0x5ee030);
        hook::Global<int32_t>(0x5ee030) = -1;
        hook::Global<int32_t>(0x62ba3a) = p;
        return 1;
    }
    int32_t s = SimRand(0, 0xb);
    if (Site_FootprintFlat(pos, s)) {
        hook::Global<int32_t>(0x62ba3a) = s;
        return 1;
    }
    for (uint32_t t = 0; t < 0xc; ++t)
        if (Site_FootprintFlat(pos, t) == 1) {
            hook::Global<int32_t>(0x62ba3a) = (int32_t)t;
            return 1;
        }
    return 0;
}
RT3_REPLACE(0x415570, Site_ChooseShape);

// 0x437d20. Combined multiplier for cargo idx: the global cargo effect (0x69) times
// the cargo's own effect (0x6a + idx, when in range).
double __stdcall Effects_CargoMultiplier(int32_t idx) {
    float base = Effects_Multiplier(World(), 0, 0x69, 0, 0, 0);
    int32_t id = idx + 0x6a;
    if (id < 0x6a || id > 0x15f) return base;
    return (double)Effects_Multiplier(World(), 0, id, 0, 0, 0) * base;
}
RT3_REPLACE(0x437d20, Effects_CargoMultiplier);

} // namespace rt3
