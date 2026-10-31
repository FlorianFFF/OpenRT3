// Larger game functions, part 8 (batch 206). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Obj_SumBuildings(void* o);
int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind);

// Weighted score of the six stat sums (+0x2b2..+0x2c6) per unit of the first.
static float Score(float s0, float s1, float s2, float s3, float s4, float s5, float ratio) {
    float t = (float)(((double)s5 + s4) * D(0x5c8628));
    t = (float)(((double)s3 + s2) * Fl(0x5c9b0c) + t);
    t = (float)(t * D(0x5c8dc0));
    float base = (float)((double)t + s0);
    float v = (float)((double)t * ratio + s1);
    float d = Fl(0x5c85b4) > base ? 1.0f : base;
    return (float)((double)v / d);
}

// 0x422320. Yearly city growth (not in sandbox modes): after refreshing each
// city's building sums, cities (then villages, +0x23e) are scored against their
// group's average; the difference sets the score (+0x2e2, -1 under the floor),
// gap (+0x2e6), clamped trend (+0x2ea, -0.7..1) and growth rate (+0x2ee, 7% / 5% of
// the trend, damped by size +0x25e, no shrinking without industry of kind 2 and
// halved / doubled with one), which then scales the population +0x25e.
void __fastcall Cities_Grow(Collection* cities) {
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82] || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) return;
    for (int32_t i = 0; i < Collection_Count(cities); ++i)
        Obj_SumBuildings(Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0)));
    for (int32_t pass = 0; pass < 2; ++pass) {
        float s[6] = {};
        float div = 1.0f;
        int32_t n = Collection_Count(cities);
        if (n > 0) {
            for (int32_t i = 0; i < Collection_Count(cities); ++i) {
                uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
                if ((pass == 0 && F<int32_t>(c, 0x23e)) || (pass == 1 && !F<int32_t>(c, 0x23e))) continue;
                for (int32_t k = 0; k < 6; ++k) s[k] = (float)((double)s[k] + F<float>(c, 0x2b2 + k * 4));
            }
            div = Fl(0x5c85b4) > s[0] ? 1.0f : s[0];
        }
        float ratio = (float)((double)s[1] / div);
        double r = (double)ratio + D(0x5c8628);
        ratio = (float)(D(0x5c8680) > r ? 0.7 : r);
        float avg = Score(s[0], s[1], s[2], s[3], s[4], s[5], ratio);
        for (int32_t i = 0; i < Collection_Count(cities); ++i) {
            uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
            if ((pass == 0 && F<int32_t>(c, 0x23e)) || (pass == 1 && !F<int32_t>(c, 0x23e))) continue;
            int32_t industry = Obj_CountBuildingsOfKind(c, 0, 2);
            auto st = [&](int32_t k) { return F<float>(c, 0x2b2 + k * 4); };
            float q = Score(st(0), st(1), st(2), st(3), st(4), st(5), ratio);
            F<float>(c, 0x2e2) = q;
            if (q < Fl(0x5c9b08)) F<uint32_t>(c, 0x2e2) = 0xbf800000;
            float diff = (float)((double)F<float>(c, 0x2e2) - avg);
            F<float>(c, 0x2e6) = diff;
            float g = (float)(diff * D(0x5c87d8));
            F<float>(c, 0x2ea) = g;
            if (g > Fl(0x5c85b4)) F<uint32_t>(c, 0x2ea) = 0x3f800000;
            else if (g < D(0x5c9b00)) F<uint32_t>(c, 0x2ea) = 0xbf333333;
            float gr = (float)(F<float>(c, 0x2ea) * (F<int32_t>(c, 0x23e) ? 0.05 : 0.07));
            F<float>(c, 0x2ee) = gr;
            float& rate = F<float>(c, 0x2ee);
            float pop = F<float>(c, 0x25e);
            if (gr > Fl(0x5c8598) && pop > Fl(0x5c8c28))
                rate = (float)((double)gr / ((double)pop / D(0x5c85d8)) * D(0x5c8680) + (double)gr * D(0x5c88c8));
            if (rate < Fl(0x5c8598) && F<float>(c, 0x25e) < Fl(0x5c8c28))
                rate = (float)((double)F<float>(c, 0x25e) / D(0x5c85d8) * rate * D(0x5c8680) + (double)rate * D(0x5c88c8));
            if (industry == 0) {
                if (Fl(0x5c8598) > rate) rate = 0.0f;
            } else if (industry == 1)
                rate = (float)(rate * (rate > Fl(0x5c8598) ? D(0x5c8680) : D(0x5c8628)));
            if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38))
                F<float>(c, 0x25e) = (float)(((double)rate + D(0x5c8770)) * F<float>(c, 0x25e));
        }
    }
}
RT3_REPLACE(0x422320, Cities_Grow);

} // namespace rt3
