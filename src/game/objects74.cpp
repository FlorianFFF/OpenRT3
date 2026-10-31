// Model / vehicle helpers in blocks 0x41..0x52 (batch 77). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void* __fastcall Obj_Ref388(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);

static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }
static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }

// 0x41ab80. First model angle scaled by 0.18.
float __fastcall Car_TiltValue(uint8_t* c) {
    float a;
    Model_GetAngles(c, 0, (uint32_t*)&a, nullptr, nullptr);
    return (float)((double)a * *(const double*)0x5c97c0);
}
RT3_REPLACE(0x41ab80, Car_TiltValue);

// 0x41add0. The car's rear coupling point: its cell position moved back by
// half its type length (+0x10d) along its heading.
void __fastcall Car_RearPoint(uint8_t* c, int, float* x, float* y) {
    float half = (float)((double)F<float>(Obj_Ref388(c), 0x10d) * *(const double*)0x5c8628);
    *x = (float)Obj_CellX(c);
    *y = (float)Obj_CellY(c);
    float a = SceneObj_Angle2(c);
    float h = (float)((double(__cdecl*)(double, double))0x5a1390)((double)a + *(const double*)0x5c8740, 6.2831854820251465);
    *x = (float)((double)*x - ExeSin(h) * half);
    *y = (float)((double)*y - ExeCos(h) * half);
}
RT3_REPLACE(0x41add0, Car_RearPoint);

// 0x527de0. Sets the map-tool brush radius: the brush marker's scale (x 5.5) and +0x3728 (x 4).
RT3_NOINLINE void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v) {
    SceneObj_SetScale36(F<uint8_t*>(s, 0x364d), 0, (float)((double)v * *(const double*)0x5dd158));
    F<float>(s, 0x3728) = (float)((double)v * *(const float*)0x5c8568);
}
RT3_REPLACE(0x527de0, Scene_SetBrushRadius);

// 0x52eab0 / 0x52eb20. Texture transform {u, v, su, sv} (+0x26..); +0x25
// tells whether it differs from the identity.
void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv) {
    F<float>(o, 0x26) = u;
    F<float>(o, 0x2e) = su;
    F<float>(o, 0x2a) = v;
    F<float>(o, 0x32) = sv;
    bool identity = (double)su == *(const double*)0x5c8770 && (double)sv == *(const double*)0x5c8770 &&
                    (double)u == *(const double*)0x5c8608 && (double)v == *(const double*)0x5c8608;
    o[0x25] = identity ? 0 : 1;
}
RT3_REPLACE(0x52eab0, SceneObj_SetUvTransform);
uint8_t __fastcall SceneObj_GetUvTransform(uint8_t* o, int, uint32_t* u, uint32_t* v, uint32_t* su, uint32_t* sv) {
    if (!o[0x25]) return 0;
    *u = F<uint32_t>(o, 0x26);
    *v = F<uint32_t>(o, 0x2a);
    *su = F<uint32_t>(o, 0x2e);
    *sv = F<uint32_t>(o, 0x32);
    return 1;
}
RT3_REPLACE(0x52eb20, SceneObj_GetUvTransform);

// 0x52dc00. Adds a tint layer (+0x69, up to 2) and marks every vertex of the
// current animation inside the rectangle (x0, y0)-(x1, y1) (by z / y) with it.
void __fastcall Model_AddTintRect(uint8_t* m, int, float x0, float y0, float x1, float y1, float tint) {
    if (!m[0x8d] || m[0x5b] >= 2) return;
    m[0x5b]++;
    F<float>(m, 0x69 + m[0x5b] * 4) = tint;
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    hook::Global<uint8_t*>(0xcc3b6c) = anim;
    for (int32_t i = 0; i < F<int32_t>(anim, 0x14); ++i) {
        uint8_t* fr = F<uint8_t*>(anim, 0xc) + i * 0x45;
        hook::Global<uint8_t*>(0xcc3b58) = fr;
        int32_t n = F<int32_t>(fr, 8);
        hook::Global<int32_t>(0xcc3b60) = n;
        uint8_t* buf = F<uint8_t*>(fr, 0x35);
        uint8_t* marks = buf + n * 12;
        for (int32_t k = 0; hook::Global<int32_t>(0xcc3b60); ++k) {
            float vz = F<float>(buf, k * 12 + 8), vy = F<float>(buf, k * 12 + 4);
            if (vz > x0 && vz < x1 && vy > y0 && vy < y1) marks[k] = m[0x5b];
            hook::Global<int32_t>(0xcc3b60)--;
        }
    }
}
RT3_REPLACE(0x52dc00, Model_AddTintRect);

} // namespace rt3
