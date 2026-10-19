// Vector helpers in block 0x45 (batch 35). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
float* __fastcall Vec3_Normalize(float* v);

// 0x457da0. (0, -2.5, 0) through the matrix +0x18e, scaled by +0x27d.
void __fastcall Obj_DownVector(uint8_t* o, int, float* out) {
    float v[3] = { 0.0f, -2.5f, 0.0f };
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform(v, 0, r, (const float*)(o + 0x18e));
    float z = (float)((double)s * t[2]);
    float y = (float)((double)s * t[1]);
    out[1] = y;
    out[2] = z;
    out[0] = (float)((double)s * t[0]);
}
RT3_REPLACE(0x457da0, Obj_DownVector);

// 0x457f40. For unit vectors a, b with t = (a.b) * pi/2: sin t * cos t * pi
// (exe CRT 0x5a13e4 / 0x5a1494), returned unrounded.
double __stdcall Vec3_AngleTerm(float a0, float a1, float a2, float b0, float b1, float b2) {
    float a[3] = { a0, a1, a2 }, b[3] = { b0, b1, b2 };
    Vec3_Normalize(a);
    Vec3_Normalize(b);
    double dot = (double)a[2] * b[2] + (double)a[1] * b[1];
    dot = dot + (double)a[0] * b[0];
    float t = (float)(dot * *(const float*)0x5c8764);
    float f1 = (float)((double(__cdecl*)(double))0x5a13e4)(t);
    float f2 = (float)((double(__cdecl*)(double))0x5a1494)(t);
    return (double)f2 * f1 * *(const float*)0x5c8760;
}
RT3_REPLACE(0x457f40, Vec3_AngleTerm);

} // namespace rt3
