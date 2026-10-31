// Port / warehouse building types / car type availability / cargo price averages and industry totals in block 0x41 (batch 154). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Obj_Available(void* o);
int32_t __fastcall Table_SumCapacity(Collection* t, int, int32_t v);
void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x419230. Rebuilds the 12 port (pass 0) and warehouse (pass 1) building types
// (0x1a7 bytes; template +0x187, industry +0x173): reuses or creates (vtable slot 1)
// the one for each template by cloning the last of its kind with its four tile
// tables (+0xcf.. when +0x177.., w +0xb8 x h +0xb9 x 32 bytes), names it and links
// the industry of the same name.
void __fastcall BuildingTypes_RebuildPortsWarehouses(Collection* c) {
    if (!World() || !hook::Global<Collection*>(0x62b268)) return;
    for (int32_t pass = 0; pass < 2; ++pass)
        for (int32_t k = 0; k < 0xc; ++k) {
            int32_t exact = -1, any = -1;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
                uint8_t* ind = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(e, 0x173));
                if (!(pass == 0 ? ind[0xba] : pass == 1 && ind[0xbb])) continue;
                any = Collection_IdAt(c, 0, i, 0);
                if (F<int32_t>(e, 0x187) == k) exact = Collection_IdAt(c, 0, i, 0);
            }
            if (exact == -1) {
                uint8_t tmp[0x1a8];
                memcpy(tmp, Collection_Get(c, 0, any), 0x1a7);
                exact = ((int32_t(__thiscall*)(Collection*, void*, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, tmp, 0, 0, 0);
            }
            uint8_t* src = (uint8_t*)Collection_Get(c, 0, any);
            uint8_t* dst = (uint8_t*)Collection_Get(c, 0, exact);
            if (exact != any) {
                memcpy(dst, src, 0x1a7);
                for (int t = 0; t < 4; ++t) {
                    if (!F<int32_t>(src, 0x177 + t * 4)) continue;
                    uint32_t size = (uint32_t)dst[0xb9] * dst[0xb8] * 32;
                    void* p = ((void*(__cdecl*)(size_t))0x53b070)(size);
                    F<void*>(dst, 0xcf + t * 4) = p;
                    memcpy(p, F<void*>(src, 0xcf + t * 4), size);
                }
            }
            F<int32_t>(dst, 0x187) = k;
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)dst + 0x22, (const char*)(pass ? 0x5c93d8 : 0x5c93e8), k);
            strcpy((char*)dst + 4, (const char*)dst + 0x22);
            int32_t link = -1;
            for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
                Collection* inds = hook::Global<Collection*>(0x62b268);
                if (!Stricmp((const char*)dst + 0x22, (const char*)Collection_Get(inds, 0, Collection_IdAt(inds, 0, i, 0)) + 4))
                    link = Collection_IdAt(hook::Global<Collection*>(0x62b268), 0, i, 0);
            }
            F<int32_t>(dst, 0x173) = link;
        }
}
RT3_REPLACE(0x419230, BuildingTypes_RebuildPortsWarehouses);

// 0x41d550. Whether car type o (kind +0x41: 2/3 = cargo cars checked against the
// cargo's effects and era options) is available to company / player in this year
// (within +0x105..+0x109; a cargo car from its first year's July on).
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player) {
    int32_t bonus = 0;
    int32_t k = F<int32_t>(o, 0x41);
    if (k == 2 || k == 3) {
        uint8_t* opts = hook::Global<uint8_t*>(0x6cec74);
        if (F<int32_t>(opts, 0x180)) return 1;
        if (World() && World()[0x4c8c]) return 1;
        if (hook::Global<uint8_t*>(0x6cec7c)[0x82] && F<int32_t>(opts, 0x247)) return 1;
        Collection* cargos = hook::Global<Collection*>(0x6ada84);
        if (!Collection_IsValid(cargos, 0, F<uint32_t>(o, 0x126))) return 0;
        uint8_t* t = (uint8_t*)Collection_Get(cargos, 0, F<uint32_t>(o, 0x126));
        int32_t id = F<int32_t>(o, 0x126) + 0xfb;
        int32_t allowed = 0;
        if (F<int32_t>(t, 0x10) == 0) bonus = Obj_CargoBonus(World(), 0, 0x1c6, company, player, 0);
        if (F<int32_t>(t, 0x10) == 1) bonus = Obj_CargoBonus(World(), 0, 0x1c7, company, player, 0);
        if (F<int32_t>(t, 0x10) == 2) bonus = Obj_CargoBonus(World(), 0, 0x1c8, company, player, 0);
        if (!Stricmp((const char*)t + 0x34, (const char*)0x5c981c)) bonus = 0;
        if (id >= 0xfb && id <= 0x15f) {
            int32_t b = Obj_CargoBonus(World(), 0, id, company, player, 0);
            if (b) {
                if (b < 0) return 0;
                return 1;
            }
        }
        if (bonus < 0) return 0;
        uint8_t* w = World();
        if (bonus > 0) {
            if (!w[0x4c97] && !w[0x4c98] && !w[0x4c99]) allowed = 1;
            else if ((t[0x30] && w[0x4c97]) || (t[0x31] && w[0x4c98]) || (t[0x32] && w[0x4c99])) allowed = 1;
        }
        if (!F<int32_t>(t, 0x7b) && !allowed) return 0;
        bonus = 1;
    }
    uint8_t* w = World();
    int32_t year = F<uint16_t>(w, 0xd);
    if (year < 0x726) year = 0x726;
    int32_t from = F<int32_t>(o, 0x105);
    bool fromOk;
    if (year == from) fromOk = w[0xf] >= 6 || !bonus;
    else fromOk = year > from;
    if (!fromOk) return 0;
    return year > F<int32_t>(o, 0x109) ? 0 : 1;
}
RT3_REPLACE(0x41d550, CarType_Available);

// 0x41e2b0. Once the cargo (+0x56) exists (year +0x21), refreshes its averages over
// every third city: quota (+0x6a), quota weighted by stock (+0x6e) and by base price
// (+0x72) (falling back to the plain average), and decays the trend pair +0x76 / +0x7a.
void __fastcall Cargo_UpdateAverages(uint8_t* o) {
    if (!F<int32_t>(o, 0x56)) return;
    uint8_t* w = World();
    int32_t year = F<int32_t>(w, 0x46c38) ? F<uint16_t>(w, 5) : F<uint16_t>(w, 0xd);
    if (year < F<int32_t>(o, 0x21)) return;
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t n = F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145);
    uint8_t* city = F<uint8_t*>(m, 0x2129);
    F<float>(o, 0x6a) = 0.0f;
    F<float>(o, 0x6e) = 0.0f;
    F<float>(o, 0x72) = 0.0f;
    float sumS = 0.0f, sumW = 0.0f;
    if (n > 0) {
        for (uint32_t cnt = (uint32_t)(n - 1) / 3 + 1; cnt; --cnt, city += 0x585) {
            int32_t c = F<int32_t>(o, 0x3e);
            float q = (float)City_CargoQuota(city, 0, c);
            float stock = F<float>(city, 0x103 + c * 4);
            float wt = (float)F<uint16_t>(city, 0x6a + c * 2);
            F<float>(o, 0x6a) = (float)((double)q + F<float>(o, 0x6a));
            if (stock != Fl(0x5c8598)) {
                F<float>(o, 0x6e) = (float)((double)stock * q + F<float>(o, 0x6e));
                sumS = (float)((double)stock + sumS);
            }
            if (wt != Fl(0x5c8598)) {
                F<float>(o, 0x72) = (float)((double)wt * q + F<float>(o, 0x72));
                sumW = (float)((double)wt + sumW);
            }
        }
    }
    float avg = (float)((double)F<float>(o, 0x6a) / (float)(n >= 1 ? n : 1));
    F<float>(o, 0x6a) = avg;
    auto norm = [&](uint32_t off, float sum) {
        if (!((double)sum > D(0x5c86b8))) {
            F<float>(o, off) = avg;
            return;
        }
        double d = (double)sum < D(0x5c86b8) ? 0.1 : (double)sum;
        F<float>(o, off) = (float)((double)F<float>(o, off) / d);
    };
    norm(0x6e, sumS);
    norm(0x72, sumW);
    float a = (float)((double)F<float>(o, 0x76) * D(0x5c8880));
    F<float>(o, 0x76) = a;
    float b = (float)((double)F<float>(o, 0x7a) * D(0x5c8880));
    F<float>(o, 0x7a) = b;
    if ((double)b < D(0x5c86b8)) {
        F<float>(o, 0x7a) = (float)((double)b + D(0x5c85b8));
        F<float>(o, 0x76) = (float)((double)avg * D(0x5c85b8) + a);
    }
}
RT3_REPLACE(0x41e2b0, Cargo_UpdateAverages);

// 0x41e6e0. The cargo's industry totals (base, share, input, input share), summed
// over the available industries weighted by their capacity; cached per day (+0x8a,
// values +0x8e..+0x9a).
RT3_NOINLINE void __fastcall Cargo_IndustryTotals(uint8_t* o, int, float* base, float* share, float* in, float* inShare) {
    if (F<int32_t>(o, 0x8a) == F<int32_t>(World(), 0x15)) {
        *(uint32_t*)base = F<uint32_t>(o, 0x8e);
        *(uint32_t*)share = F<uint32_t>(o, 0x92);
        *(uint32_t*)in = F<uint32_t>(o, 0x96);
        *(uint32_t*)inShare = F<uint32_t>(o, 0x9a);
        return;
    }
    *inShare = 0.0f;
    *in = 0.0f;
    *share = 0.0f;
    *base = 0.0f;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
        uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x62b268), 0, i, 0);
        uint8_t* ind = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, id);
        if (!Obj_Available(ind)) continue;
        float v[4];
        Industry_CargoStats(ind, 0, F<int32_t>(o, 0x3e), 0, &v[0], &v[1], &v[2], &v[3]);
        if (!(v[0] > D(0x5c85b8) || v[1] > D(0x5c85b8) || v[2] > D(0x5c85b8) || v[3] > D(0x5c85b8))) continue;
        int32_t cnt = Table_SumCapacity(hook::Global<Collection*>(0x62b26c), 0, id);
        if (cnt <= 0) continue;
        float cf = (float)cnt;
        *base = (float)((double)cf * v[0] + *base);
        *share = (float)((double)cf * v[1] + *share);
        *in = (float)((double)cf * v[2] + *in);
        *inShare = (float)((double)cf * v[3] + *inShare);
    }
    F<int32_t>(o, 0x8a) = F<int32_t>(World(), 0x15);
    F<float>(o, 0x8e) = *base;
    F<float>(o, 0x92) = *share;
    F<float>(o, 0x96) = *in;
    F<float>(o, 0x9a) = *inShare;
}
RT3_REPLACE(0x41e6e0, Cargo_IndustryTotals);

} // namespace rt3
