// Company credit rate drift / station route distance relaxation in block 0x47 (batch 168). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_Rounded29(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
uint8_t* __fastcall Obj_Region(void* o);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Bits(uint64_t u) { return *(double*)&u; }

// 0x476560. Drifts the company's credit rate (+0x154, double) towards the market
// rate plus its bonus (stat 0x39): the gap is capped by the current band (0.01 above
// k1 .. 0.04 above k4) and a k-th of it is applied, truncated. Off in sandbox mode.
void __fastcall Company_UpdateCreditRate(uint8_t* o) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (F<int32_t>(w, 0x46c38)) return;
    float a = (float)Obj_Rounded29(w);
    int32_t n = Obj_CargoBonus(hook::Global<void*>(0x6cec78), 0, 0x39, 0, F<uint32_t>(o, 0), 0);
    a = (float)((double)n * D(0x5c85b8) + a);
    double rate = F<double>(o, 0x154);
    if (rate > D(0x5c8608)) {
        double b = (double)a - D(0x5c9b98);
        double q = D(0x5c8608) > b ? 0.0 : b;
        a = (float)q;
        double v;
        bool set = true;
        if (rate > D(0x5ce820)) v = a < D(0x5c85b8) ? (double)a : Bits(0x3f847ae147ae147bull);
        else if (rate > D(0x5ce818)) v = a < D(0x5c8918) ? (double)a : Bits(0x3f947ae147ae147bull);
        else if (rate > D(0x5ce810)) v = a < D(0x5c9b80) ? (double)a : Bits(0x3f9eb851eb851eb8ull);
        else if (rate > D(0x5c8810)) v = a < D(0x5c9b98) ? (double)a : Bits(0x3fa47ae147ae147bull);
        else set = false;
        if (set) a = (float)v;
    }
    double s = (double)a * F<double>(o, 0x154) / D(0x5c8978);
    int32_t step;
    __asm {
        fld s
        mov eax, 0x5a10d0
        call eax
        mov step, eax
    }
    F<double>(o, 0x154) = (double)step + F<double>(o, 0x154);
}
RT3_REPLACE(0x476560, Company_UpdateCreditRate);

// 0x47df70. Ages the station's 53 cargo ratings (+0x3e, zeroed after 0x1d8800 ticks
// without service, else decayed; +0x112 too) and relaxes its three route distance
// tables (+0x24/+0x28/+0x2c, 5-byte entries: distance, known flag) through each
// known neighbour's table: d(i) + neighbour d(j) beats d(j) for j not i or itself.
void __fastcall Station_UpdateRoutes(uint8_t* o) {
    uint8_t* b = F<uint32_t>(o, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
    Obj_Region(b);
    for (int32_t k = 0; k < 0x35; ++k) {
        float* p = &F<float>(o, 0x3e + k * 4);
        if (*p <= *(const float*)0x5c8598) continue;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15) - F<int32_t>(p, 0x1a8) > 0x1d8800) *p = 0.0f;
        else {
            *p = (float)(*p * D(0x5ce900));
            F<float>(p, 0xd4) = (float)(F<float>(p, 0xd4) * D(0x5ce8f8));
        }
    }
    for (uint32_t t = 0x24; t < 0x30; t += 4) {
        for (int32_t i = 0; i < F<int32_t>(o, 0x30); ++i) {
            uint8_t* e = F<uint8_t*>(o, t) + i * 5;
            if (!e[4]) continue;
            F<float>(e, 0) = (float)(F<float>(e, 0) + (double)*(const float*)0x5c9f18);
            float d = F<float>(F<uint8_t*>(o, t) + i * 5, 0);
            uint8_t* other = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, i);
            int32_t n = F<int32_t>(o, 0x30);
            if (n >= F<int32_t>(other, 0x30)) n = F<int32_t>(other, 0x30);
            for (int32_t j = 0; j < n; ++j) {
                uint8_t* oe = F<uint8_t*>(other, t) + j * 5;
                if (!oe[4]) continue;
                uint8_t* me = F<uint8_t*>(o, t) + j * 5;
                if (!((double)d + F<float>(oe, 0) < F<float>(me, 0)) || j == i || j == F<int32_t>(o, 0)) continue;
                me[4] = 1;
                F<float>(F<uint8_t*>(o, t) + j * 5, 0) = (float)((double)d + F<float>(F<uint8_t*>(other, t) + j * 5, 0));
            }
        }
    }
}
RT3_REPLACE(0x47df70, Station_UpdateRoutes);

} // namespace rt3
