// Ray against an object's mesh in its local space in block 0x53 (batch 185). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __stdcall Ray_HitsTriangle_Impl(float a0, float a1, float a2, float b0, float b1, float b2, float c0, float c1,
                                        float c2, float o0, float o1, float o2, float d0, float d1, float d2);

// The mesh walk state the original keeps in globals (current LOD, sub-mesh,
// triangle pointer and triangles left).
static uint8_t*& g_lod = hook::Global<uint8_t*>(0xcc3b6c);
static uint8_t*& g_sub = hook::Global<uint8_t*>(0xcc3b58);
static uint8_t*& g_tri = hook::Global<uint8_t*>(0xcc3b64);
static int32_t& g_left = hook::Global<int32_t>(0xcc3b5c);

// 0x530030. Whether the ray (origin, direction) hits a triangle of the object's
// current sub-mesh (+0x21 of LOD +0x3a): both are taken into the object's space
// with its inverse matrix (+0x8e, rebuilt from +0xce when +0x20 bit 2 is set) after
// subtracting its position (+0x1e2).
int32_t __fastcall Mesh_RayHit(uint8_t* o, int, const float* dir, const float* origin) {
    g_lod = F<uint8_t*>(o, 0x14) + F<int32_t>(o, 0x3a) * 0x18;
    g_sub = F<uint8_t*>(g_lod, 0xc) + F<int32_t>(o, 0x21) * 0x45;
    g_tri = F<uint8_t*>(g_sub, 4);
    g_left = F<int32_t>(g_sub, 0xc);
    float x = dir[0], y = dir[1], z = dir[2];
    if (o[0x20] & 4) {
        o[0x20] &= 0xfb;
        float det = 0.0f;
        ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(o + 0x8e, &det, o + 0xce);
    }
    const float* m = (const float*)(o + 0x8e);
    float dx = (float)((double)z * m[8] + (double)x * m[0] + (double)y * m[4]);
    float dy = (float)((double)x * m[1] + (double)y * m[5] + (double)z * m[9]);
    float dz = (float)((double)y * m[6] + (double)z * m[10] + (double)x * m[2]);
    double px = (double)origin[0] - F<float>(o, 0x1e2);
    double py = (double)origin[1] - F<float>(o, 0x1e6);
    double pz = (double)origin[2] - F<float>(o, 0x1ea);
    float ox = (float)(pz * m[8] + px * m[0] + py * m[4]);
    float oy = (float)(px * m[1] + py * m[5] + pz * m[9]);
    float oz = (float)(py * m[6] + pz * m[10] + px * m[2]);
    if (!g_left) return 0;
    do {
        const float* a = F<const float*>(g_tri, 0);
        const float* b = F<const float*>(g_tri, 4);
        const float* c = F<const float*>(g_tri, 8);
        if (Ray_HitsTriangle_Impl(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], ox, oy, oz, dx, dy, dz)) return 1;
        g_tri += 0x4c;
    } while (--g_left);
    return 0;
}
RT3_REPLACE(0x530030, Mesh_RayHit);

} // namespace rt3

namespace rt3 {

void __fastcall Camera_Angles(uint8_t* c, int, float* yaw, float* pitch, float* dist);

// 0x52f970. Turns sub-mesh `idx` (clamped to the LOD's count) of the object to face
// camera `cam`: when `key` changed (+0x31) the vertices (+0, count +8) are rotated
// by -pitch/2 then yaw into the copy at +0x35 (allocated on first use).
void __fastcall Mesh_FaceCamera(uint8_t* o, int, uint8_t* cam, int32_t key, int32_t idx) {
    uint8_t* lod = F<uint8_t*>(o, 0x14);
    if (idx > F<int32_t>(lod, 0x14) - 1) idx = F<int32_t>(lod, 0x14) - 1;
    uint8_t* sub = F<uint8_t*>(lod, 0xc) + idx * 0x45;
    if (F<int32_t>(sub, 0x31) == key) return;
    F<int32_t>(sub, 0x31) = key;
    if (!F<void*>(sub, 0x35)) F<void*>(sub, 0x35) = ((void*(__cdecl*)(size_t))0x5a125d)(F<int32_t>(sub, 8) * 0xc);
    float yaw, pitch;
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    float r[16];
    Camera_Angles(cam, 0, &yaw, &pitch, nullptr);
    auto mul = (void*(__stdcall*)(float*, const float*, const float*))0x56d3cd;
    ((void*(__stdcall*)(float*, float))0x56dda8)(r, (float)(-(double)pitch * *(const double*)0x5c8628));
    mul(m, m, r);
    ((void*(__stdcall*)(float*, float))0x56de44)(r, yaw);
    mul(m, m, r);
    const float* src = F<const float*>(sub, 0);
    float* dst = F<float*>(sub, 0x35);
    for (int32_t i = 0; i < F<int32_t>(sub, 8); ++i, src += 3, dst += 3) {
        double x = src[0], y = src[1], z = src[2];
        dst[0] = (float)((double)m[8] * z + y * m[4] + x * m[0]);
        dst[1] = (float)((double)m[9] * z + y * m[5] + x * m[1]);
        dst[2] = (float)(z * m[10] + y * m[6] + x * m[2]);
    }
}
RT3_REPLACE(0x52f970, Mesh_FaceCamera);

} // namespace rt3
