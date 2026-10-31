// Block component queries / train engine sound and animation speed / weighted yearly ratio in block 0x4a (batch 173). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Routes_NumberComponents(Collection* c);
void* __fastcall Obj_Ref388(void* o);
int32_t __fastcall Vehicle_SetEngineSound(uint8_t* v, int, float scale, uint32_t volume);
uint32_t __fastcall Obj_NodeVirtual5(void* o);
void __fastcall ListPair_Reset(uint8_t* o);
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o);
void __fastcall Train_SetTarget(uint8_t* t, int, float v);
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x4a62c0. Whether blocks a and b are connected (same component), renumbering
// first if needed.
int32_t __fastcall Routes_SameComponent(Collection* c, int, int32_t a, int32_t b) {
    if (a == -1 || b == -1) return 0;
    if (F<int32_t>(c, 0x8c)) Routes_NumberComponents(c);
    int32_t cb = F<int32_t>(Collection_Get(c, 0, b), 0xc);
    return F<int32_t>(Collection_Get(c, 0, a), 0xc) == cb ? 1 : 0;
}
RT3_REPLACE(0x4a62c0, Routes_SameComponent);

// 0x4a6320. The component of block id (-1 if none), renumbering first if needed.
RT3_NOINLINE int32_t __fastcall Routes_Component(Collection* c, int, int32_t id) {
    if (F<int32_t>(c, 0x8c)) Routes_NumberComponents(c);
    if (id == -1 || !Collection_IsValid(c, 0, id)) return -1;
    return F<int32_t>(Collection_Get(c, 0, id), 0xc);
}
RT3_REPLACE(0x4a6320, Routes_Component);

// 0x4a8be0. Sets the train's engine sound from speed / top speed (+0x20 of its engine
// type, else 100) and, unless the train is in a non-moving state, the animation
// speed of its cars' animated parts from throttle / top speed (0.1..1).
void __fastcall Train_UpdateEngineEffects(uint8_t* t, int, float throttle, float speed) {
    uint8_t* et = nullptr;
    float top;
    bool have = false;
    if (F<int32_t>(t, 0x41) != -1) {
        void* e = Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(t, 0x41));
        if (e) {
            et = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, F<uint32_t>(Obj_Ref388(e), 0x126));
            if (et) {
                top = F<float>(et, 0x20);
                have = true;
            }
        }
    }
    if (!have) {
        et = nullptr;
        top = 100.0f;
    }
    if (F<int32_t>(t, 0x41) == -1) return;
    uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(t, 0x41));
    if (!e) return;
    int32_t kind = F<int32_t>(et, 0x10);
    float k1, k2;
    if (kind == 2) {
        k1 = 0.13f;
        k2 = 0.3f;
    } else k1 = k2 = 0.2f;
    float r = (float)((double)speed / top);
    float a = (float)(((double)k1 + k1) * r + (D(0x5c8770) - k1));
    float b = (float)(D(0x5c8770) - (D(0x5c8770) - r) * k2);
    if (kind == 2 && speed <= Fl(0x5c8554))
        b = (float)(((double)speed / D(0x5c8d68) + D(0x5c8770)) * b * D(0x5c8628));
    Vehicle_SetEngineSound(e, 0, a, *(uint32_t*)&b);
    float v = (float)(((double)throttle + throttle) / top);
    if (Fl(0x5cb4a8) > v) v = 0.1f;
    else if (Fl(0x5c85b4) < v) v = 1.0f;
    if (throttle == Fl(0x5c8598)) return;
    switch (F<int32_t>(t, 0xe8)) {
    case 0x11: case 0xc: case 0xd: case 0xb: case 0x12: case 3: case 4: case 5: case 6: return;
    }
    for (int32_t j = 0; j < F<int32_t>(e, 0x31a); ++j)
        for (int32_t m = 0; m < 8; ++m) {
            uint8_t* s = F<uint8_t*>(F<uint8_t*>(e, 0x316), 0x10 + j * 0xc0 + m * 4);
            if (!s) continue;
            if (Obj_NodeVirtual5(e) == 1) {
                s[0x1d8] = 1;
                continue;
            }
            if (s[0x1d8]) {
                ListPair_Reset(s);
                s[0x1d8] = 0;
            }
            uint32_t k = F<void*>(s, 0x60) && Obj_NameIs5ce4f4(F<uint8_t*>(s, 0x60)) ? 0x5d006c : 0x5d0068;
            Train_SetTarget(s, 0, (float)((double)Fl(k) - v));
            Timer_SetFrameTime(s, 0, v);
        }
}
RT3_REPLACE(0x4a8be0, Train_UpdateEngineEffects);

// 0x4a9630. Weighted ratio of the yearly totals over up to the last five years
// (+0x109/+0x131/+0x159, newest first; weights 6 - age, this year scaled by the
// month): (a + b + c) / a, scaled, plus 0.5, clamped to 0..1.
float __fastcall Obj_YearlyRatio(uint8_t* o) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    int32_t n = F<uint16_t>(w, 0xd) - F<int32_t>(o, 0xd0) + 1;
    if (n > 5) n = 5;
    float W = 0.0f, X = 0.0f, Y = 0.0f;
    for (int32_t i = 0; i < (n > 10 ? 10 : n); ++i) {
        float a = F<float>(o, 0x109 + i * 4);
        float s2 = (float)((double)F<float>(o, 0x159 + i * 4) + F<float>(o, 0x131 + i * 4));
        float s3 = (float)((double)s2 + a);
        float wt = (float)(6 - i);
        wt = i == 0 ? (float)((double)(float)w[0xf] * wt) : (float)((double)wt * Fl(0x5c8788));
        W = (float)((double)wt + W);
        X = (float)((double)wt * a + X);
        Y = (float)((double)wt * s3 + Y);
    }
    float d1 = Y > Fl(0x5c85b4) ? W : 1.0f;
    Y = (float)((double)Y * Fl(0x5c8788) / d1);
    float d2 = W > Fl(0x5c85b4) ? W : 1.0f;
    X = (float)((double)X * Fl(0x5c8788) / d2);
    double r;
    float d = Fl(0x5c85b4) > X ? 1.0f : X;
    if (!(Y < Fl(0x5c8598))) r = (double)Y / d * D(0x5c8da8);
    else r = (double)Y / d;
    float f = (float)(r + D(0x5c8628));
    if (f > Fl(0x5c85b4)) return 1.0f;
    if (f < Fl(0x5c8598)) f = 0.0f;
    return f;
}
RT3_REPLACE(0x4a9630, Obj_YearlyRatio);

} // namespace rt3
