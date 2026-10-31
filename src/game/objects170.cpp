// Track node component test / train revenue sharing / train running cost / train yearly upkeep and reliability in blocks 0x48..0x4a (batch 174). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Routes_SameComponent(Collection* c, int, int32_t a, int32_t b);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void* __fastcall Obj_BuildingRegionAt(void* o);
void* __fastcall Obj_BuildingKind(void* o);
void __fastcall Station_AddStat49(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat51(uint8_t* o, int, float a, uint8_t* company);
void __fastcall Station_AddStat45(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat4d(uint8_t* o, int, float a, uint8_t* company);
void* __fastcall Obj_Ref388(void* o);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_Get21(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
uint32_t __fastcall Obj_CityGridByte(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Train_AddExpense(uint8_t* t, int, float amount);
void __fastcall Train_EarnRevenue(uint8_t* o, int, float amount);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }
static inline Collection* Players() { return hook::Global<Collection*>(0x62be10); }
static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }
static inline uint8_t* Engine(uint8_t* t) {
    return F<int32_t>(t, 0x41) != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(t, 0x41)) : nullptr;
}
static inline uint8_t* EngineType(uint8_t* e) {
    return (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, F<uint32_t>(Obj_Ref388(e), 0x126));
}

// 0x48e3c0. Whether track node id is in the same block component as this node.
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id) {
    int32_t b = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id), 0x212);
    return Routes_SameComponent(hook::Global<Collection*>(0x6cfcb4), 0, F<int32_t>(n, 0x212), b);
}
RT3_REPLACE(0x48e3c0, Node_SameComponent);

// Share divisor: the train's +0x39, at least 1.
static inline float ShareCap(uint8_t* t) { return Fl(0x5c85b4) > F<float>(t, 0x39) ? 1.0f : F<float>(t, 0x39); }

// 0x4abf00. Shares a delivery's revenue: each track owner (+0x1ac ids, +0x1b2
// shares, +0x1b8 count) gets its share (stat 4), the train's company the rest
// (stat 1, or 2 when `kind` is 0; tallied in +0x109); the origin / destination
// stations (+0x9c/+0xa0), their regions and building kinds, and the world totals
// record the revenue and `units` of `cargo`.
void __fastcall Train_ShareRevenue(uint8_t* t, int, float revenue, int32_t kind, float units, int32_t cargo) {
    int32_t total = 0;
    for (int32_t i = 0; i < t[0x1b8]; ++i) {
        uint32_t own = F<uint16_t>(t, 0x1ac + i * 2);
        int32_t share = F<uint16_t>(t, 0x1b2 + i * 2);
        total += share;
        if (!Collection_IsValid(Players(), 0, own)) continue;
        float cap = ShareCap(t);
        float frac = (float)((double)(float)share / cap);
        if (!(frac > Fl(0x5c8598))) continue;
        Company_PostStat(Collection_Get(Players(), 0, own), 0, 4, (float)((double)frac * revenue), 1);
    }
    float cap = ShareCap(t);
    float frac = (float)((double)(float)total / cap);
    float rest = (float)((D(0x5c8770) - frac) * revenue);
    if (rest < Fl(0x5c8598)) rest = 0.0f;
    F<float>(t, 0x109) = (float)((double)rest + F<float>(t, 0x109));
    Company_PostStat(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0, kind == 0 ? 2 : 1, rest, 1);
    uint8_t *reg1 = nullptr, *reg2 = nullptr, *kind1 = nullptr, *kind2 = nullptr;
    if (Collection_IsValid(Stations(), 0, F<uint32_t>(t, 0x9c))) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint32_t>(t, 0x9c));
        F<float>(s, 0x53c) = (float)((double)rest + F<float>(s, 0x53c));
        F<float>(s, 0x4ec) = (float)((double)units + F<float>(s, 0x4ec));
        reg1 = (uint8_t*)Obj_BuildingRegionAt(s);
        kind1 = (uint8_t*)Obj_BuildingKind(s);
    }
    if (Collection_IsValid(Stations(), 0, F<uint32_t>(t, 0xa0))) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint32_t>(t, 0xa0));
        F<float>(s, 0x514) = (float)((double)rest + F<float>(s, 0x514));
        F<float>(s, 0x4c4) = (float)((double)units + F<float>(s, 0x4c4));
        reg2 = (uint8_t*)Obj_BuildingRegionAt(s);
        kind2 = (uint8_t*)Obj_BuildingKind(s);
    }
    if (reg1) {
        Station_AddStat49(reg1, 0, units, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), cargo);
        Station_AddStat51(reg1, 0, rest, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)));
    }
    if (reg2) {
        Station_AddStat45(reg2, 0, units, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), cargo);
        Station_AddStat4d(reg2, 0, rest, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)));
    }
    if (kind1) {
        F<float>(kind1, 0x2be) = (float)((double)units + F<float>(kind1, 0x2be));
        F<float>(kind1, 0x2c6) = (float)((double)rest + F<float>(kind1, 0x2c6));
    }
    if (kind2) {
        F<float>(kind2, 0x2ba) = (float)((double)units + F<float>(kind2, 0x2ba));
        F<float>(kind2, 0x2c2) = (float)((double)rest + F<float>(kind2, 0x2c2));
    }
    uint8_t* w = World();
    F<float>(w, 0xbce) = (float)((double)units + F<float>(w, 0xbce));
    F<float>(w, 0xa26 + cargo * 4) = (float)((double)units + F<float>(w, 0xa26 + cargo * 4));
    F<float>(World(), 0xbd6) = (float)((double)rest + F<float>(World(), 0xbd6));
}
RT3_REPLACE(0x4abf00, Train_ShareRevenue);

static inline uint32_t EngineRegion(uint8_t* t) {
    uint8_t* e = Engine(t);
    if (!e) return 0;
    float y = (float)Obj_CellY(e);
    return Map_GridByte(hook::Global<void*>(0x62c120), 0, (float)Obj_CellX(e), y);
}

// 0x4ac460. The train's running cost: base by age (new engines cheaper), times its
// load (+0xb8), less for idle engines of kind 0 (+0xe0 = 0), by condition (+0xdc),
// by the difficulty step (engine +0x18 plus the regional bonuses 0x55 / kind + 0x56)
// and halved for +0x197. Free with world +0x4b03.
float __fastcall Train_RunningCost(uint8_t* t) {
    if (F<int32_t>(World(), 0x4b03)) return Fl(0x5c8598);
    uint8_t* et = EngineType(Engine(t));
    int32_t age = F<uint16_t>(World(), 0xd) - F<int32_t>(t, 0xd4);
    float v = (float)((double)(age + 0x1e) / D(0x5c8588) * D(0x5d0080));
    if (age <= 0) v = (float)(v * D(0x5c88c8));
    else if (age <= 1) v = (float)(v * D(0x5c8880));
    int32_t n = (int32_t)F<float>(t, 0xb8);
    v = (float)(((double)n + D(0x5c9680)) / D(0x5c85e8) * v);
    if (!F<int32_t>(et, 0x10) && F<float>(t, 0xe0) == Fl(0x5c8598)) v = (float)(v * D(0x5c8698));
    if (F<float>(t, 0xdc) < D(0x5c85b8)) v = (float)((double)v * Fl(0x5c8900));
    else v = (float)((D(0x5c8770) - F<float>(t, 0xdc) + D(0x5c8770)) * v);
    uint32_t r1 = EngineRegion(t);
    uint32_t r2 = EngineRegion(t);
    void* w = hook::Global<void*>(0x6cec78);
    int32_t p = Obj_Get25(w);
    int32_t b = Obj_CargoBonus(w, 0, F<int32_t>(et, 0x10) + 0x56, Obj_Get21(w), p, r2);
    p = Obj_Get25(w);
    b += Obj_CargoBonus(w, 0, 0x55, Obj_Get21(w), p, r1);
    int32_t step = F<int32_t>(et, 0x18) + b;
    if (step < 0) step = 0;
    if (step > 10) step = 10;
    v = (float)(v * (double)((const float*)0x5f3430)[step]);
    if (t[0x197]) v = (float)(v * D(0x5c8628));
    return (float)(v * D(0x5c8640));
}
RT3_REPLACE(0x4ac460, Train_RunningCost);

// 0x4aead0. Yearly train upkeep (unless sandbox): the maintenance expense grows with
// age squared, the engine's base cost (+0xc) plus the regional bonuses and the
// distance run (+0x61); the condition charge (x3 when worn below k); then blends the
// reliability (+0x3d) towards the engine's (+0xcc) by the time since the last
// service (+0xb0) and resets the counters (+0x61/+0x65).
void __fastcall Train_YearlyUpkeep(uint8_t* t) {
    if (F<int32_t>(World(), 0x46c38)) return;
    uint8_t* e = Engine(t);
    if (!e) return;
    uint8_t* et = EngineType(e);
    if (!et) return;
    int32_t age = F<uint16_t>(World(), 0xd) - F<int32_t>(t, 0xd4);
    float agef = (float)((double)(age * 4 + 0x64) / D(0x5c85d8));
    agef = (float)((double)agef * agef);
    uint32_t pl1 = F<uint32_t>(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0x3b);
    uint32_t pl2 = F<uint32_t>(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0x3b);
    uint8_t* e2 = Engine(t);
    uint8_t* et2 = e2 ? EngineType(e2) : nullptr;
    void* w = hook::Global<void*>(0x6cec78);
    uint32_t r = Obj_CityGridByte(t);
    int32_t a = Obj_CargoBonus(w, 0, F<int32_t>(et2, 0x10) + 0x4a, F<uint32_t>(t, 0x51), pl2, r);
    r = Obj_CityGridByte(t);
    a *= Obj_CargoBonus(w, 0, 0x49, F<uint32_t>(t, 0x51), pl1, r);
    float c = (float)(F<int32_t>(et, 0xc) + a);
    if (c < Fl(0x5c8598)) c = 0.5f;
    float g = (float)((double)(age + 0x64) / D(0x5c85d8));
    g = (float)((double)g * g);
    float v = (float)-((double)g * F<float>(t, 0x61) * c * D(0x5d00a0));
    Train_AddExpense(t, 0, v);
    float m = F<float>(t, 0xdc) <= D(0x5c85b8) ? 3.0f : 1.0f;
    Train_EarnRevenue(t, 0, (float)(-((double)m * F<float>(et, 8) * agef) / Fl(0x5d009c)));
    int32_t dt = F<int32_t>(World(), 0x15) - F<int32_t>(t, 0xb0);
    if (dt > 0) {
        int32_t lo = dt < 0x2760 ? dt : 0x2760;
        int32_t hi = dt - 0x2760 < 0 ? 0 : dt - 0x2760;
        int32_t n = hi + lo >= 1 ? hi + lo : 1;
        double s = (double)(float)lo * F<float>(t, 0xcc) + (double)(float)hi * F<float>(t, 0x3d);
        F<float>(t, 0x3d) = (float)(s / (float)n);
    }
    F<int32_t>(t, 0x61) = 0;
    F<int32_t>(t, 0x65) = 0;
}
RT3_REPLACE(0x4aead0, Train_YearlyUpkeep);

} // namespace rt3
