// Scene object angle helpers in blocks 0x45..0x52 (batch 76). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);

static inline double Fsin(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }
static inline double Fcos(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// 0x455730 / 0x455790 / 0x455750. Second / first angle; set the second angle.
float __fastcall SceneObj_Angle2(uint8_t* o) {
    float a, b, c;
    SceneObj_GetAngles(o, 0, (uint32_t*)&a, (uint32_t*)&b, (uint32_t*)&c);
    return b;
}
RT3_REPLACE(0x455730, SceneObj_Angle2);
float __fastcall SceneObj_Angle1(uint8_t* o) {
    float a, b, c;
    SceneObj_GetAngles(o, 0, (uint32_t*)&a, (uint32_t*)&b, (uint32_t*)&c);
    return a;
}
RT3_REPLACE(0x455790, SceneObj_Angle1);
void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v) {
    uint32_t a, b, c;
    SceneObj_GetAngles(o, 0, &a, &b, &c);
    SceneObj_SetAngles(o, 0, a, v, c);
}
RT3_REPLACE(0x455750, SceneObj_SetAngle2);

// 0x456500. Spins the six wheel models (+0x302) by speed x (+0x2a5 / +0x285) x 30,
// around the third axis (or the second when +0x2f5 is set), wrapping past 2 pi.
void __fastcall Vehicle_SpinWheels(uint8_t* v, int, float speed) {
    for (int k = 0; k < 6; ++k) {
        uint8_t* w = F<uint8_t*>(v, 0x302 + k * 4);
        if (!w) continue;
        float ang;
        if (!v[0x2f5]) SceneObj_GetAngles(w, 0, nullptr, nullptr, (uint32_t*)&ang);
        else SceneObj_GetAngles(w, 0, nullptr, (uint32_t*)&ang, nullptr);
        double r = (double)F<float>(v, 0x2a5) / F<float>(v, 0x285);
        ang = (float)(r * ((double)speed * *(const float*)0x5c9aa8) + ang);
        const float twoPi = *(const float*)0x5c8768;
        if (ang > twoPi) ang = (float)((double)ang - twoPi);
        if (!v[0x2f5]) SceneObj_SetAngles(F<uint8_t*>(v, 0x302 + k * 4), 0, 0, 0, Bits(ang));
        else SceneObj_SetAngles(F<uint8_t*>(v, 0x302 + k * 4), 0, 0, Bits(ang), 0);
    }
}
RT3_REPLACE(0x456500, Vehicle_SpinWheels);

// 0x45c470 / 0x45c480 / 0x45c4d0. Angles of the model part list +0x316 (0xc0-byte parts, object +0xc).
void __fastcall Model_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    SceneObj_SetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xc), 0, a, b, c);
}
RT3_REPLACE(0x45c470, Model_SetAngles);
void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    uint32_t x, y, z;
    SceneObj_GetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xc), 0, &x, &y, &z);
    if (a) *a = x;
    if (b) *b = y;
    if (c) *c = z;
}
RT3_REPLACE(0x45c480, Model_GetAngles);
void __fastcall Model_SetPartAngle2(uint8_t* o, int, int32_t part, uint32_t v) {
    if (!part) return;
    SceneObj_SetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), part * 0xc0 + 0xc), 0, 0, v, 0);
}
RT3_REPLACE(0x45c4d0, Model_SetPartAngle2);

// 0x52e9f0. Rotates the object's quaternion (+0x1ce) by `angle` about the
// axis (ax, ay, az) (D3DX), switching it to quaternion mode (flags 0x82).
void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle) {
    double h = (double)angle * *(const double*)0x5c8628;
    o[0x20] |= 0x82;
    double s = Fsin(h);
    float q[4] = { (float)((double)ax * s), (float)((double)ay * s), (float)((double)az * s), (float)Fcos(h) };
    auto normalize = (void*(__stdcall*)(void*, const void*))0x56e979;
    normalize(q, q);
    float r[4];
    ((void*(__stdcall*)(void*, const void*, const void*))0x56e8d0)(r, q, o + 0x1ce);
    for (int k = 0; k < 4; ++k) F<float>(o, 0x1ce + k * 4) = r[k];
    normalize(o + 0x1ce, o + 0x1ce);
}
RT3_REPLACE(0x52e9f0, SceneObj_RotateAxis);

void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v) {
    if (v == F<float>(o, 0x36)) return;
    o[0x20] |= 2;
    F<float>(o, 0x36) = v;
}
RT3_REPLACE(0x52ea90, SceneObj_SetScale36);

} // namespace rt3
