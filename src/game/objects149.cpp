// Industry cargo stat views / cargo handling / site flatness check in block 0x41 (batch 152). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t max);
int32_t __fastcall Game_EffectiveVersion();
const char* __fastcall GetString(int32_t id);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x412960. The industry's total output of cargo (base + share) for p.
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return (double)base + share;
}
RT3_REPLACE(0x412960, Industry_OutputTotal);

// 0x4129a0. The industry's input of cargo per output share for p.
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return inShare;
}
RT3_REPLACE(0x4129a0, Industry_InputShare);

// 0x4129d0. Whether the industry deals in cargo: among its current cargos (+0x7a0),
// producing it (over 0.001), or holding demand / stock of it (+0xad / +0xa1).
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo) {
    for (int32_t i = 0; i < F<int32_t>(ind, 0x7a8); ++i)
        if (F<uint16_t*>(ind, 0x7a0)[i] == cargo) return 1;
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, 0, &base, &share, &in, &inShare);
    if ((double)base + share > D(0x5c8678)) return 1;
    int32_t idx = F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4);
    if (idx == -1) return 0;
    if (F<float>(ind, 0xad + idx * 4) > Fl(0x5c8598)) return 1;
    return F<float>(ind, 0xa1 + idx * 4) > Fl(0x5c8598) ? 1 : 0;
}
RT3_REPLACE(0x4129d0, Industry_HandlesCargo);

// 0x415020. Checks the placement rect (0x62b7a8 x0, 0x62b7ac x1, 0x62b7b0 y0,
// 0x62b7b4 y1) for flatness: heights must stay within 40 (4 for ports, 0x62b2fc
// +0x9c) of the masked mean (0x62b7c4) unless lenient (allowSteep) or the cell is
// flagged; min/max to 0x62b7b8/0x62b7bc, middle to 0x62b7c0. Failure sets the
// message 0x62b270 (string 0xb8) and returns 0.
int32_t __stdcall Site_CheckFlatness(int32_t allowSteep, int32_t ignoreFlags) {
    float x1f = (float)hook::Global<int32_t>(0x62b7ac);
    if (*(uint32_t*)&x1f == 0xc97423f0) return 1;
    hook::Global<uint32_t>(0x62b7bc) = 0xc97423f0;
    hook::Global<uint32_t>(0x62b7b8) = 0x497423f0;
    hook::Global<uint32_t>(0x62b7c4) = 0x497423f0;
    hook::Global<int32_t>(0x62b7c0) = 0;
    uint8_t* types = hook::Global<uint8_t*>(0x62b2fc);
    float tol = F<int32_t>(types, 0x9c) ? 4.0f : 40.0f;
    float buf[0x80];
    int32_t n = Map_GatherMasked(nullptr, 0, buf, 0x80);
    if (n > 0) {
        float sum = 0.0f;
        for (int32_t i = 0; i < n; ++i) sum = (float)((double)sum + buf[i]);
        hook::Global<float>(0x62b7c4) = (float)((double)sum / (float)n);
    }
    float& lo = hook::Global<float>(0x62b7b8);
    float& hi = hook::Global<float>(0x62b7bc);
    for (int32_t y = hook::Global<int32_t>(0x62b7b0); y <= hook::Global<int32_t>(0x62b7b4); ++y)
        for (int32_t x = hook::Global<int32_t>(0x62b7a8); x <= hook::Global<int32_t>(0x62b7ac); ++x) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            float h = (float)Map_GridFloat(m, 0, x, y);
            m = hook::Global<uint8_t*>(0x62c120);
            bool check = (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 1) ||
                         (!ignoreFlags && !F<int32_t>(hook::Global<uint8_t*>(0x62b2fc), 0x9c) &&
                          Game_EffectiveVersion() >= 0x68);
            if (check) {
                if (hook::Global<uint32_t>(0x62b7c4) == 0x497423f0) {
                    hook::Global<float>(0x62b7c4) = h;
                } else if (fabs((double)h - hook::Global<float>(0x62b7c4)) > tol && !allowSteep) {
                    const char* s = GetString(0xb8);
                    char* d = (char*)0x62b270;
                    while ((*d++ = *s++) != 0) {}
                    return 0;
                }
            }
            if (lo > h) lo = h;
            if (hi < h) hi = h;
        }
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x4fd) && F<int32_t>(hook::Global<uint8_t*>(0x62b2fc), 0x9c)) {
        double diff = (double)hi - lo;
        double lim = hook::Global<int32_t>(0x62ba3a) != -1 ? D(0x5c9680) : (double)Fl(0x5c8c28);
        if (diff > lim && !allowSteep) return 0;
    }
    hook::Global<float>(0x62b7c0) = (float)(((double)lo + hi) * Fl(0x5c8d48));
    return 1;
}
RT3_REPLACE(0x415020, Site_CheckFlatness);

} // namespace rt3
