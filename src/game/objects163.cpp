// AI engine choice / station load share / random building visit / scenario lookup by name in blocks 0x40..0x46 (batch 167). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
int32_t __fastcall Game_EffectiveVersion();
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint32_t __fastcall EngineTypes_PickForYear(Collection* c);
uint8_t* __fastcall Obj_RegionAt(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_ListEntry(uint8_t* o, int, int32_t i, int32_t list, int32_t* out);
float __fastcall Obj_NodeCenterY(void* o);
float __fastcall Obj_NodeCenterX(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall Pairs_Total(uint8_t* o);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
void __fastcall Building_ChargeUpkeep(uint8_t* b);
uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind);
void __fastcall Obj_FreeBuffer8(void* o);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }
static inline Collection* Engines() { return hook::Global<Collection*>(0x6ada84); }
static inline uint8_t* CarTypeOf(uint8_t* e) {
    return F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72))
                                : nullptr;
}
static inline int32_t Ftol(float f) {
    int32_t r;
    __asm {
        fld f
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x4078a0. The engine type the AI company buys: the best score of speed x
// sqrt(reliability + k) x (30 - +0xc) x (+0x18 + 10) x (+0x1c + 15) / (age + 30) /
// running cost, times a random 80..120%, halved for slow engines. Passes relax the
// rules: pass 0 only finds the top speed; kind 2 from pass 2, unavailable ones in
// pass 3. Falls back to the engine for the year.
uint32_t __fastcall AI_PickEngineType(uint32_t company) {
    int32_t best = -1;
    float bestScore = -1.0f, maxSpeed = 0.0f;
    for (int32_t pass = 0; pass < 4; ++pass) {
        if (best != -1) continue;
        for (int32_t i = 0; i < Collection_Count(Engines()); ++i) {
            uint8_t* e = (uint8_t*)Collection_Get(Engines(), 0, Collection_IdAt(Engines(), 0, i, 0));
            uint8_t* ct = CarTypeOf(e);
            if (F<int32_t>(e, 0x10) == 2 && pass < 2) continue;
            Collection* players = hook::Global<Collection*>(0x62be10);
            uint32_t player = Collection_IsValid(players, 0, company)
                ? F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, company), 0x3b) : 0;
            if (!CarType_Available(ct, 0, company, player) && pass < 3) continue;
            if (!(maxSpeed > F<float>(e, 0x20))) maxSpeed = F<float>(e, 0x20);
            if (!pass) continue;
            float s = F<float>(e, 0x20);
            if (!Strnicmp((const char*)e + 0x34, (const char*)0x5c88d0, 4) && Game_EffectiveVersion() >= 0x68)
                s = (float)(s * D(0x5c88c8));
            float rel = F<float>(e, 0x28) > Fl(0x5c85b4) ? F<float>(e, 0x28) : 1.0f;
            float q = (float)Sqrt((double)rel + Fl(0x5c8568));
            s = (float)((double)q * s);
            s = (float)((double)(float)(30 - F<int32_t>(e, 0xc)) * s);
            s = (float)((double)(float)(F<int32_t>(e, 0x18) + 10) * s);
            s = (float)((double)(float)(F<int32_t>(e, 0x1c) + 15) * s);
            int32_t age = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(CarTypeOf(e), 0x105);
            float denom = (float)((age <= 0 ? 0 : age) + 30);
            s = (float)((double)s / denom);
            s = (float)((double)s * Fl(0x5c88c0));
            s = (float)((double)s / ((double)Obj_EffectScale(e, 0, company, player) + Fl(0x5c88bc)));
            int32_t r = SimRand(0x50, 0x78);
            s = (float)((double)r / D(0x5c85d8) * s);
            float sp = F<float>(e, 0x20);
            if ((double)sp + sp <= maxSpeed && sp <= Fl(0x5c88b8)) s = (float)(s * D(0x5c8628));
            if (s > bestScore) {
                best = (int32_t)Collection_IdAt(Engines(), 0, i, 0);
                bestScore = s;
            }
        }
    }
    if (best == -1) best = (int32_t)EngineTypes_PickForYear(Engines());
    return best > 1 ? best : 1;
}
RT3_REPLACE(0x4078a0, AI_PickEngineType);

// 0x4676a0. How much of the cargo waiting at the station (o: building +0xe, cargo
// +0x10, amount +0x18, rate +0x13) the train picks up for the route from `from` to
// `to`: 0 unless one of the region's stations on the route is closer to the cargo's
// destination than `from`; the share is the distance ratio towards that station
// times (1 - waiting / k), capped. Scaled by the difficulty table for city cargos.
// With `apply` records the pickup (+0x17) and adds it to the amount.
int32_t __fastcall Station_LoadShare(uint8_t* o, int, uint8_t* from, uint8_t* to, uint8_t* city, float* out, int32_t apply) {
    uint8_t* reg = Obj_RegionAt(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(o, 0xe)));
    *out = 0.0f;
    float bestFrom = 4.83e8f, bestTo = 4.83e8f;
    uint8_t* via = nullptr;
    if (F<int32_t>(reg, 0xeb) <= 0) return 0;
    for (int32_t i = 0; i < F<int32_t>(reg, 0xeb); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint16_t>(F<uint8_t*>(reg, 0xef), i * 6));
        if (Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        float d;
        if (Obj_ListEntry(from, 0, F<int32_t>(s, 0), o[0x10], (int32_t*)&d) && d < bestFrom) bestFrom = d;
        if (s == to) {
            bestTo = 0.0f;
            via = s;
        } else if (Obj_ListEntry(to, 0, F<int32_t>(s, 0), o[0x10], (int32_t*)&d) && d < bestTo) {
            bestTo = d;
            via = s;
        }
    }
    if (!(bestTo < bestFrom)) return 0;
    float ratio;
    if (via == to) ratio = 1.0f;
    else {
        int32_t d1 = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(from)), Ftol(Obj_NodeCenterY(from)), Ftol(Obj_NodeCenterX(to)),
                                           Ftol(Obj_NodeCenterY(to)));
        int32_t d2 = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(to)), Ftol(Obj_NodeCenterY(to)), Ftol(Obj_NodeCenterX(via)),
                                           Ftol(Obj_NodeCenterY(via)));
        if (d1 < 1) d1 = 1;
        if (d2 < 1) d2 = 1;
        ratio = (float)((double)(float)d1 / (float)(d2 + d1));
    }
    int32_t have = o[0x18];
    float q = (float)((double)have / D(0x5ca110));
    if (q > D(0x5c8650)) q = (float)0.95;
    float f = (float)((D(0x5c8770) - q) * ratio);
    double fk = (double)f * Fl(0x5ca0ec);
    int32_t n;
    __asm {
        fld fk
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    if (n < 1) n = 1;
    if (have + n > 0xff) {
        n = 0xff - have;
        if (n < 1) n = 1;
    }
    *out = (float)((double)n / D(0x5ca110) * F<float>(o, 0x13));
    if (!o[0x10] && city) {
        uint8_t* p = (uint8_t*)Obj_CityRef(city);
        if (p) {
            const float* tab = (const float*)(Game_EffectiveVersion() >= 0x67 ? 0x5f349c : 0x5f348c);
            *out = tab[F<int32_t>(p, 0x14)] * *out;
        }
        if (city[0x1a1]) *out = (float)(*out * D(0x5c8698));
    }
    if (apply) {
        o[0x17] = (uint8_t)n;
        int32_t t = o[0x18] + (uint8_t)n;
        o[0x18] = (uint8_t)(t > 0xff ? 0xff : t);
    }
    return 1;
}
RT3_REPLACE(0x4676a0, Station_LoadShare);

// 0x467a10. For a non-city cargo picks a random list of the route's buildings by the
// weights at 0x5f2ae8[kind] and charges the upkeep of its cheapest active building
// (cost +4 + k, times a random 1..1000), if under the limit.
void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind) {
    if (o[0x10]) return;
    if (!Pairs_Total(r)) return;
    float v = (float)((double)SimRand(1, 0x63) / D(0x5c85d8));
    const float* w = (const float*)0x5f2ae8 + kind;
    int32_t list = 0;
    for (;; ++list, w += 3) {
        if (list >= 5) return;
        v = (float)((double)v - *w);
        if (v < Fl(0x5c8598)) break;
    }
    if (!Obj_Array590(r, 0, list)) return;
    float best = 9999999.0f;
    int32_t pick = -1;
    Collection* blds = hook::Global<Collection*>(0x62b26c);
    if (Obj_Array590(r, 0, list) == 1) {
        if (Obj_Flag3d4(Collection_Get(blds, 0, F<uint32_t>(Pairs_At(r, 0, list, 1), 0))) == 1) pick = 0;
        float c = (float)((double)F<float>(Pairs_At(r, 0, list, 1), 4) + Fl(0x5c8900));
        float rnd = (float)SimRand(1, 1000);
        best = (float)((double)c * rnd);
    } else {
        if (Obj_Array590(r, 0, list) <= 0) return;
        for (int32_t j = 0; j < Obj_Array590(r, 0, list); ++j) {
            if (Obj_Flag3d4(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(Pairs_At(r, 0, list, j + 1), 0))) != 1)
                continue;
            float c = (float)((double)F<float>(Pairs_At(r, 0, list, j + 1), 4) + Fl(0x5c8900));
            float rnd = (float)SimRand(1, 1000);
            float s = (float)((double)c * rnd);
            if (s < best) {
                best = s;
                pick = j;
            }
        }
    }
    if (pick == -1 || !(best < Fl(0x5c85b0))) return;
    Building_ChargeUpkeep((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0,
                                                   F<uint32_t>(Pairs_At(r, 0, list, pick + 1), 0)));
}
RT3_REPLACE(0x467a10, Route_ChargeRandomBuilding);

// Looks up name in the save list of the given kind; on a match stores its display
// name at 0x6ce670.
static int32_t FindInList(const char* name, int32_t kind) {
    void* p = ((void*(__cdecl*)(size_t))0x53b070)(0xc);
    uint8_t* list = p ? SaveList_Init((uint8_t*)p, 0, kind) : nullptr;
    int32_t found = 0;
    for (int32_t i = 1; F<int32_t>(list, 0) > 0 && i <= F<int32_t>(list, 0); ++i) {
        if (Stricmp((const char*)F<uint8_t*>(list, 8) + (i - 1) * 0x25a, name)) continue;
        found = 1;
        hook::Global<int32_t>(0x6ce9bc) = kind == 0xc;
        if (kind == 0xc) strcpy((char*)0x6ce630, name);
        strcpy((char*)0x6ce670, (const char*)F<uint8_t*>(list, 8) + (i - 1) * 0x25a + 0x12d);
        break;
    }
    Obj_FreeBuffer8(list);
    ((void(__cdecl*)(void*))0x53b080)(list);
    return found;
}

// 0x46d7f0. Selects the scenario with this file name: from the scenario list (kind
// 7, option +0x6c set), else from the kind-12 list (option +0x6c clear, name kept
// at 0x6ce630). Returns whether it was found.
int32_t __fastcall Scenario_SelectByName(const char* name) {
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x6c) = 1;
    if (FindInList(name, 7)) return 1;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x6c) = 0;
    return FindInList(name, 0xc);
}
RT3_REPLACE(0x46d7f0, Scenario_SelectByName);

} // namespace rt3
