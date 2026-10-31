// City cargo quotas and prices / camera ground follow / city name check in blocks 0x41..0x44 (batch 128). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Industry_BuildCargoLists(uint8_t* o);
double __stdcall Effects_CargoMultiplier(int32_t idx);
void __fastcall Cargo_Produce(void* c, int, float a, float price);
int32_t __fastcall Obj_YearReached(void* o);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Camera_KeepAboveGround(uint8_t* c);
float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }
static inline Collection* Sites() { return hook::Global<Collection*>(0x6cec20); }

// ftol(cargo multiplier x (base word[i] << 4)), in x87 precision.
static int32_t CargoQuota(uint8_t* o, int32_t i) {
    double m = Effects_CargoMultiplier(i);
    float n = (float)((int32_t)F<uint16_t>(o, i * 2) << 4);
    int32_t r;
    __asm {
        fld m
        fmul n
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x412bd0. Rebuilds the cargo lists of every industry in the collection.
void __fastcall Industries_BuildCargoLists(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Industry_BuildCargoLists((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x412bd0, Industries_BuildCargoLists);

// 0x42c960. The city's quota for cargo i: its base (word i, x16) times the cargo multiplier.
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i) { return CargoQuota(o, i); }
RT3_REPLACE(0x42c960, City_CargoQuota);

// 0x42cab0. Produces amt of cargo i at the quota price, adds it to the city's
// total (+0x103) and returns its value.
double __fastcall City_ProduceCargo(uint8_t* o, int, int32_t i, float amt) {
    float price = (float)CargoQuota(o, i);
    Cargo_Produce(Collection_Get(Cargos(), 0, i), 0, amt, price);
    F<float>(o, 0x103 + i * 4) = (float)((double)amt + F<float>(o, 0x103 + i * 4));
    return (double)price * amt;
}
RT3_REPLACE(0x42cab0, City_ProduceCargo);

// 0x42cbb0. Whether any of the city's sites (chain from +0xd4, next +0x3c) has
// demand for cargo i (+0x3e) and more of it in stock (+0x112) than the quota.
int32_t __fastcall City_HasCargoSurplus(uint8_t* o, int, int32_t i) {
    for (uint32_t id = F<uint16_t>(o, 0xd4); id;) {
        uint8_t* s = (uint8_t*)Collection_Get(Sites(), 0, id);
        if ((double)F<float>(s, 0x3e + i * 4) > D(0x5c85b8)) {
            float q = (float)CargoQuota(o, i);
            if (F<float>(s, 0x112 + i * 4) > q) return 1;
        }
        id = F<uint16_t>(s, 0x3c);
    }
    return 0;
}
RT3_REPLACE(0x42cbb0, City_HasCargoSurplus);

// 0x42cc50. Re-prices the city's cargo bases (word i) from each site's surplus over
// the quota, scaled down as the city's total (+0x103) nears the site's demand.
void __fastcall City_UpdateCargoBases(uint8_t* o) {
    uint32_t id = F<uint16_t>(o, 0xd4);
    if (!id) return;
    int32_t n = Collection_Count(Cargos());
    while (id) {
        uint8_t* s = (uint8_t*)Collection_Get(Sites(), 0, id);
        for (int32_t i = 1; i <= n; ++i) {
            if (F<int32_t>(Cargos(), 0x9a + i * 4) != -1) continue;
            if (!Obj_YearReached(Collection_Get(Cargos(), 0, i))) continue;
            float demand = F<float>(s, 0x3e + i * 4);
            if ((double)demand <= D(0x5c85b8)) continue;
            float q = (float)CargoQuota(o, i);
            float surplus = (float)((double)F<float>(s, 0x112 + i * 4) - q);
            if (surplus <= Fl(0x5c8598)) continue;
            float ratio = (float)((double)F<float>(o, 0x103 + i * 4) / ((double)demand / D(0x5c85d0)));
            if (ratio > Fl(0x5c8900)) continue;
            float scale = (float)((D(0x5c85c8) - ratio) * D(0x5c9c68));
            int32_t v;
            __asm {
                fld scale
                fmul surplus
                fadd q
                mov eax, 0x5a10d0
                call eax
                mov v, eax
            }
            v >>= 4;
            if (v >= 0xfde8) v = 0xfde8;
            F<uint16_t>(o, i * 2) = (uint16_t)v;
        }
        id = F<uint16_t>(s, 0x3c);
    }
}
RT3_REPLACE(0x42cc50, City_UpdateCargoBases);

// Zoom (+0x18) floor, pitch for it (+0x14), then keep the eye above ground.
static void ApplyZoom(uint8_t* c) {
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<float>(c, 0x18) = 37.5f;
    F<float>(c, 0x14) = Camera_PitchForZoom(c, 0, F<float>(c, 0x18));
    Camera_KeepAboveGround(c);
}

// 0x43ad20. Re-applies the camera zoom.
void __fastcall Camera_ApplyZoom(uint8_t* c) { ApplyZoom(c); }
RT3_REPLACE(0x43ad20, Camera_ApplyZoom);

// 0x43c770. While ground following (+0x34e > 0) moves the eye height (+8) toward
// the terrain under it, blending from +0x34a by the factor, then re-applies the zoom.
void __fastcall Camera_FollowGround(uint8_t* c) {
    if ((double)F<float>(c, 0x34e) <= Fl(0x5c8598)) return;
    float h = (float)(Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc)) + D(0x5ca208));
    if (!(F<float>(c, 0x34e) >= D(0x5c8770)))
        F<float>(c, 8) = (float)(((double)h - F<float>(c, 0x34a)) * F<float>(c, 0x34e) + F<float>(c, 0x34a));
    else
        F<float>(c, 8) = h;
    ApplyZoom(c);
}
RT3_REPLACE(0x43c770, Camera_FollowGround);

// 0x442a40. Whether any element of 0x6cfcbc refers (city +0x41, then +0x388, name
// +0x82) to one of eight fixed names.
int32_t __cdecl AnyHasFixedName() {
    static const uint32_t names[8] = { 0x5ca3a4, 0x5ca39c, 0x5ca390, 0x5ca384, 0x5ca378, 0x5ca36c, 0x5ca364, 0x5ca35c };
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfcbc)); ++i) {
        Collection* c = hook::Global<Collection*>(0x6cfcbc);
        void* x = Obj_City41(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
        for (int k = 0; k < 8; ++k)
            if (!Stricmp((const char*)Obj_Ref388(x) + 0x82, (const char*)names[k])) return 1;
    }
    return 0;
}
RT3_REPLACE(0x442a40, AnyHasFixedName);

} // namespace rt3
