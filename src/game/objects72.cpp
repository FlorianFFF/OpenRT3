// Scene object transform helpers in blocks 0x4a..0x52 (batch 75). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);

// 0x4a7970. Whether obj is at least sqrt(0x5d005c) away from the site's flagged building.
int32_t __stdcall Obj_FarFromSiteBuilding(uint8_t* obj, void* site) {
    uint8_t* b = (uint8_t*)Obj_Building(site);
    if (Obj_Flag3d4(b) != 1) return 0;
    float p1[3], p2[3];
    SceneObj_GetPos(b, 0, (uint32_t*)&p1[0], (uint32_t*)&p1[1], (uint32_t*)&p1[2]);
    SceneObj_GetPos(obj, 0, (uint32_t*)&p2[0], (uint32_t*)&p2[1], (uint32_t*)&p2[2]);
    float dz = (float)((double)p2[2] - p1[2]);
    float dy = (float)((double)p2[1] - p1[1]);
    float dx = (float)((double)p2[0] - p1[0]);
    float d = (float)((double)dx * dx + (double)dy * dy + (double)dz * dz);
    return d >= *(const float*)0x5d005c ? 1 : 0;
}
RT3_REPLACE(0x4a7970, Obj_FarFromSiteBuilding);

void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1e2);
    *y = F<uint32_t>(o, 0x1e6);
    *z = F<uint32_t>(o, 0x1ea);
}
RT3_REPLACE(0x52e780, SceneObj_GetPos2);

// 0x52e7b0. Transforms a point by the object's world matrix (+0x10e, rows of
// 4 floats, translation +0x13e); 0 while the matrix is dirty (flag 2).
int32_t __fastcall SceneObj_Transform(uint8_t* o, int, float* x, float* y, float* z) {
    if (o[0x20] & 2) return 0;
    double px = *x, py = *y, pz = *z;
    const float* m = (const float*)(o + 0x10e);
    *x = (float)(pz * m[8] + py * m[4] + px * m[0] + m[12]);
    *y = (float)(pz * m[9] + py * m[5] + px * m[1] + m[13]);
    *z = (float)(pz * m[10] + py * m[6] + px * m[2] + m[14]);
    return 1;
}
RT3_REPLACE(0x52e7b0, SceneObj_Transform);

void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z) {
    o[0x20] |= 2;
    F<uint32_t>(o, 0x1e2) = x;
    F<uint32_t>(o, 0x1e6) = y;
    F<uint32_t>(o, 0x1ea) = z;
}
RT3_REPLACE(0x52e850, SceneObj_SetPos);

void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    if (a) *a = F<uint32_t>(o, 0x4b);
    if (b) *b = F<uint32_t>(o, 0x4f);
    if (c) *c = F<uint32_t>(o, 0x53);
}
RT3_REPLACE(0x52e880, SceneObj_GetAngles);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    o[0x20] = (uint8_t)((o[0x20] & 0x7d) | 2);
    F<uint32_t>(o, 0x4b) = a;
    F<uint32_t>(o, 0x4f) = b;
    F<uint32_t>(o, 0x53) = c;
}
RT3_REPLACE(0x52e8b0, SceneObj_SetAngles);

// 0x52e8e0. Rotates by (da, db, dc), wrapping each angle once past 2 pi
// (ignored for quaternion-driven objects, flag 0x80).
void __fastcall SceneObj_Rotate(uint8_t* o, int, float da, float db, float dc) {
    if (o[0x20] & 0x80) return;
    o[0x20] |= 2;
    double a = (double)da + F<float>(o, 0x4b);
    F<float>(o, 0x4b) = (float)a;
    float b = (float)((double)db + F<float>(o, 0x4f));
    F<float>(o, 0x4f) = b;
    float c = (float)((double)dc + F<float>(o, 0x53));
    F<float>(o, 0x53) = c;
    const float twoPi = *(const float*)0x5c8768;
    if (a > twoPi) F<float>(o, 0x4b) = (float)(a - twoPi);
    if (b > twoPi) F<float>(o, 0x4f) = (float)((double)b - twoPi);
    if (c > twoPi) F<float>(o, 0x53) = (float)((double)c - twoPi);
}
RT3_REPLACE(0x52e8e0, SceneObj_Rotate);

void __fastcall SceneObj_ResetQuat(uint8_t* o) {
    F<int32_t>(o, 0x1d6) = 0;
    F<int32_t>(o, 0x1d2) = 0;
    F<int32_t>(o, 0x1ce) = 0;
    F<uint32_t>(o, 0x1da) = 0x3f800000;
}
RT3_REPLACE(0x52e970, SceneObj_ResetQuat);

// 0x52e990. For quaternion-driven objects: local matrix +0x18e = rotation(+0x1ce) x local (D3DX).
void __fastcall SceneObj_ApplyQuat(uint8_t* o) {
    if (!(o[0x20] & 0x80)) return;
    o[0x20] |= 2;
    float rot[16], res[16];
    ((void*(__stdcall*)(float*, const void*))0x56e092)(rot, o + 0x1ce);
    ((void*(__stdcall*)(float*, const float*, const void*))0x56d3cd)(res, rot, o + 0x18e);
    for (int k = 0; k < 16; ++k) F<float>(o, 0x18e + k * 4) = res[k];
}
RT3_REPLACE(0x52e990, SceneObj_ApplyQuat);

} // namespace rt3
