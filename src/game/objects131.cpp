// Route membership / signal sides / cargo rates / templates / grade factor / revenue / editor undo and texel blend in blocks 0x48..0x4f (batch 134). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id);
int32_t __fastcall Pair_Contains(void* p, int, int32_t v);
int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void* __fastcall Array33_At(void* a, int, int32_t i);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
void* __fastcall Obj_Ref388(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
float __fastcall Company_QualityFactor(void* c);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
int32_t __fastcall Map_PointCount(void* m);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }
static inline Collection* Routes() { return hook::Global<Collection*>(0x6cfcb4); }

// 0x48b870. Whether track node n is an end of its route (+0x212), splitting the
// route off first when it no longer exists.
int32_t __fastcall Node_IsRouteEnd(uint8_t* n) {
    if (!Collection_IsValid(Routes(), 0, F<uint32_t>(n, 0x212)) && !Routes_SplitAt(Routes(), 0, F<uint32_t>(n, 0x202)))
        return 0;
    return Pair_Contains(Collection_Get(Routes(), 0, F<uint32_t>(n, 0x212)), 0, F<int32_t>(n, 0x202));
}
RT3_REPLACE(0x48b870, Node_IsRouteEnd);

// 0x4a7bc0. For a signal node (+0x94, kind 4): 1 when `other`'s position (+0x1e2,
// +0x1ea) is behind it for the car's direction (+0x35a), else 0; 2 for other kinds.
int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other) {
    uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(o, 0x94));
    if (e[0x218] != 4) return 2;
    return !Effect_PointOnSide(e, 0, F<int32_t>(car, 0x35a), F<float>(other, 0x1e2), F<float>(other, 0x1ea)) ? 1 : 0;
}
RT3_REPLACE(0x4a7bc0, Signal_IsBehind);

// 0x4a7c10. Payment for delivering value v of cargo o (kind +0x10) loaded in year
// `year`: v over the speed/price effects (0x5a + kind, 0x59; at least 0.02), aged
// 0.5% a year, less the base (+0x24, not below 0); then 0.98 to the power of that
// over +0x28, x +0x20 and the effects 0x51 and 0x52 + kind; at least 1, doubled
// when world +0x4c8f.
float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region) {
    uint32_t player = company ? F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, company), 0x3b) : 0;
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x5a, company, player, region);
    float m = (float)((double)Effects_Multiplier(World(), 0, 0x59, company, player, region) * m1);
    double d = (double)m > D(0x5c8918) ? (double)m : D(0x5c8918);
    float x = (float)((double)v / d);
    int32_t age = (int32_t)F<uint16_t>(World(), 0xd) - year;
    x = (float)(((double)age * D(0x5c9bc0) + D(0x5c8770)) * x);
    float r = (float)((double)x - F<float>(o, 0x24));
    if ((double)r < D(0x5c8608)) r = 0.0f;
    float scale = F<float>(o, 0x20);
    double p = ((double(__cdecl*)(double, double))0x5a1b60)(0.98, (double)r / F<float>(o, 0x28));
    float q = (float)(p * scale);
    q = (float)((double)Effects_Multiplier(World(), 0, 0x51, company, player, region) * q);
    q = (float)((double)Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x52, company, player, region) * q);
    if (q < Fl(0x5c85b4)) q = 1.0f;
    if (World()[0x4c8f]) q = (float)((double)q + q);
    return q;
}
RT3_REPLACE(0x4a7c10, Cargo_DeliveryPayment);

// Copies template entry src into array entry e (0x33-byte entries).
static void CopyEntry(uint8_t* e, uint8_t* src) {
    F<uint32_t>(e, 0x24) = F<uint32_t>(src, 0x24);
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x40);
    for (int k = 0; k < 8; ++k) F<uint32_t>(e, 1 + k * 4) = F<uint32_t>(src, 1 + k * 4);
    e[0x23] = src[0x23];
    e[0x22] = src[0x22];
    e[0x21] = src[0x21];
    e[0] = src[0];
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x20);
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x10);
}

// 0x4a94b0. Applies the pending template (+0x1c6: flag word +8 = all entries, else
// entry +0xa; the entry data from +0xc) to o's entries and frees it.
void __fastcall Obj_ApplyPendingTemplate(uint8_t* o) {
    uint8_t* t = F<uint8_t*>(o, 0x1c6);
    if (!t) return;
    uint8_t* src = t + 0xc;
    if (F<uint16_t>(t, 8)) {
        for (int32_t i = 0; i < F<int32_t>(o, 4); ++i) CopyEntry((uint8_t*)Array33_At(o, 0, i), src);
    } else {
        CopyEntry((uint8_t*)Array33_At(o, 0, F<uint16_t>(t, 0xa)), src);
    }
    Free(F<void*>(o, 0x1c6));
    F<void*>(o, 0x1c6) = nullptr;
}
RT3_REPLACE(0x4a94b0, Obj_ApplyPendingTemplate);

// 0x4aa990. The vehicle's speed (+0xcc) adjusted for the grade of its model's
// heading (so +0x16e..+0x176): 1 - horizontal length^2 of it, times the speed over
// the engine's top speed (type +0x20), added downhill (x 0x5d007c) or taken off
// uphill (x 0x5d0078, at least 1).
float __fastcall Vehicle_GradeSpeed(uint8_t* o, int, uint8_t* so) {
    float base = F<float>(o, 0xcc);
    if (Fl(0x5c8598) == F<float>(o, 0xcc)) return base;
    SceneObj_UpdateMatrix(so);
    float a = F<float>(so, 0x16e), b = F<float>(so, 0x172), c = F<float>(so, 0x176);
    float s = (float)((double)a * a + (double)c * c + (double)b * Fl(0x5c8598));
    uint8_t* type = nullptr;
    int32_t id = F<int32_t>(o, 0x41);
    void* car = id != -1 ? Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id) : nullptr;
    if (car) type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    float ratio = (float)((double)F<float>(o, 0xcc) / F<float>(type, 0x20));
    double flat = (double)Fl(0x5c85b4) - s;
    if (b < Fl(0x5c8598)) {
        ratio = (float)(flat * Fl(0x5d007c) * ratio);
        return (float)((double)ratio + base);
    }
    ratio = (float)(flat * Fl(0x5d0078) * ratio);
    base = (float)((double)base - ratio);
    if (Fl(0x5c85b4) > base) base = 1.0f;
    return base;
}
RT3_REPLACE(0x4aa990, Vehicle_GradeSpeed);

// 0x4ac250. Books revenue `amount` for the train: scaled by the effects 0x4e + the
// engine kind (at the car's region) and 0x4d and the company's quality factor,
// added to +0x131 and posted to the company (+0x51) as stat 7. Not when paused (+0x82).
void __fastcall Train_EarnRevenue(uint8_t* o, int, float amount) {
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    auto regionOf = [&]() -> uint32_t {
        int32_t id = F<int32_t>(o, 0x41);
        void* car = id != -1 ? Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id) : nullptr;
        if (!car) return 0;
        float y = (float)Obj_CellY(car);
        float x = (float)Obj_CellX(car);
        return Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
    };
    Collection* companies = hook::Global<Collection*>(0x62be10);
    uint32_t region1 = regionOf();
    uint32_t player1 = F<uint32_t>(Collection_Get(companies, 0, F<uint32_t>(o, 0x51)), 0x3b);
    uint32_t region2 = regionOf();
    uint32_t player2 = F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(o, 0x51)), 0x3b);
    uint8_t* type = nullptr;
    int32_t id = F<int32_t>(o, 0x41);
    void* car = id != -1 ? Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id) : nullptr;
    if (car) type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(type, 0x10) + 0x4e, F<uint32_t>(o, 0x51), player2, region2);
    float m = (float)((double)Effects_Multiplier(World(), 0, 0x4d, F<uint32_t>(o, 0x51), player1, region1) * m1);
    amount = (float)((double)m * amount);
    void* comp = Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(o, 0x51));
    amount = (float)((double)Company_QualityFactor(comp) * amount);
    F<float>(o, 0x131) = (float)((double)amount + F<float>(o, 0x131));
    Company_PostStat(Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(o, 0x51)), 0, 7, amount, 1);
}
RT3_REPLACE(0x4ac250, Train_EarnRevenue);

// 0x4f5a80. Pushes an undo snapshot (up to 10, +0x1e4 = newest) of the editor's
// image (+0x114, +0xfc x +0x100 pixels) and the map's 4 terrain layers.
void __fastcall Editor_PushUndo(uint8_t* o) {
    if (F<int32_t>(o, 0x1e4) < 9) {
        int32_t n = ++F<int32_t>(o, 0x1e4);
        memcpy(F<void*>(o, 0x11c + n * 4), F<void*>(o, 0x114), F<int32_t>(o, 0x100) * F<int32_t>(o, 0xfc) * 4);
        for (int k = 0; k < 4; ++k) {
            int32_t sz = Map_PointCount(hook::Global<void*>(0x62c120));
            const void* src = (const void*)Terrain_Layer(hook::Global<uint8_t*>(0x62c120), 0, (uint8_t)k);
            memcpy(F<void*>(o, 0x144 + (k + F<int32_t>(o, 0x1e4) * 4) * 4), src, sz);
        }
        return;
    }
    for (int j = 0; j < 9; ++j) {
        memcpy(F<void*>(o, 0x11c + j * 4), F<void*>(o, 0x120 + j * 4), F<int32_t>(o, 0xfc) * F<int32_t>(o, 0x100) * 4);
        for (int k = 0; k < 4; ++k) {
            int32_t sz = Map_PointCount(hook::Global<void*>(0x62c120));
            memcpy(F<void*>(o, 0x144 + (j * 4 + k) * 4), F<void*>(o, 0x154 + (j * 4 + k) * 4), sz);
        }
    }
    memcpy(F<void*>(o, 0x140), F<void*>(o, 0x114), F<int32_t>(o, 0xfc) * F<int32_t>(o, 0x100) * 4);
    for (int k = 0; k < 4; ++k) {
        int32_t sz = Map_PointCount(hook::Global<void*>(0x62c120));
        const void* src = (const void*)Terrain_Layer(hook::Global<uint8_t*>(0x62c120), 0, (uint8_t)k);
        memcpy(F<void*>(o, 0x1d4 + k * 4), src, sz);
    }
}
RT3_REPLACE(0x4f5a80, Editor_PushUndo);

// One blended channel: ftol(src * alpha + dst * (1 - alpha)).
static inline uint8_t Blend(uint32_t s, uint32_t d, float alpha, double inv) {
    int32_t srcI = (int32_t)s, dstI = (int32_t)d, r;
    float sf = (float)srcI;
    __asm {
        fld inv
        fld sf
        fmul alpha
        fild dstI
        fmul st(0), st(2)
        faddp st(1), st(0)
        mov eax, 0x5a10d0
        call eax
        fstp st(0)
        mov r, eax
    }
    return (uint8_t)r;
}

// 0x4f81e0. Blends texel (x, y) of the editor image (+0x114, stride +0xfc) with
// texture tile idx (8 per row; tile +0x1f0 wide, rows +0x1f4 apart, stride +0x1e8,
// data +0x1fc, wrapping) by alpha.
void __fastcall Editor_BlendTexel(uint8_t* o, int, int32_t x, int32_t y, float alpha, int32_t idx) {
    uint8_t* dst = F<uint8_t*>(o, 0x114) + (F<int32_t>(o, 0xfc) * y + x) * 4;
    int32_t w = F<int32_t>(o, 0x1f0);
    int32_t base = (F<int32_t>(o, 0x1f4) * (idx / 8) + y % w) * F<int32_t>(o, 0x1e8) + (idx % 8) * w;
    const uint8_t* src = F<uint8_t*>(o, 0x1fc) + (base + x % w) * 4;
    double inv = D(0x5c8770) - alpha;
    uint32_t v = (uint32_t)Blend(src[3], dst[3], alpha, inv) << 8;
    v = (v | Blend(src[2], dst[2], alpha, inv)) << 8;
    v = (v | Blend(src[1], dst[1], alpha, inv)) << 8;
    v |= Blend(src[0], dst[0], alpha, inv);
    *(uint32_t*)dst = v;
}
RT3_REPLACE(0x4f81e0, Editor_BlendTexel);

} // namespace rt3
