// Functions across blocks 0x49..0x4a (batch 44). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_Float18(void* o);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
void __fastcall Obj_DecA4(uint8_t* o);
void __fastcall Obj_DecAc(uint8_t* o);

// 0x493360. As 0x4932a0 for the second camera pair (+0xa8/+0xac, flag +0xb4).
void __fastcall Camera_BeginAc(uint8_t* o) {
    uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
    if (!F<int32_t>(o, 0xa8) && !F<int32_t>(o, 0xac))
        F<int32_t>(o, 0xb4) = F<float>(cam, 0x66) < *(const float*)0x5c88e0 ? 1 : 0;
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::Global<void*>(0x62be68));
    F<int32_t>(o, 0xac)++;
}
RT3_REPLACE(0x493360, Camera_BeginAc);

// 0x4935a0. Owner (+0x22e) and flag (+0x44) of the first track piece at cell (x, y); 1 if found.
int32_t __fastcall Map_TrackOwnerAt(void* o, int, int32_t x, int32_t y, int32_t* owner, int32_t* flag) {
    uint8_t* e = (uint8_t*)Obj_CellEntry(o, 0, (uint16_t)x, (uint16_t)y);
    if (!e) return 0;
    uint32_t* list = F<uint32_t*>(e, 2);
    if (!list) return 0;
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, list[0]);
    if (!t) return 0;
    *owner = F<uint16_t>(t, 0x22e);
    *flag = t[0x44] ? 1 : 0;
    return 1;
}
RT3_REPLACE(0x4935a0, Map_TrackOwnerAt);

int32_t __fastcall Table_CountFlag201_40(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<uint8_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x201) & 0x40) ++n;
    return n;
}
RT3_REPLACE(0x493770, Table_CountFlag201_40);

// 0x494e40. Initialises a 0x42-byte route record; returns it.
uint8_t* __fastcall Route_Init(uint8_t* r) {
    F<int32_t>(r, 4) = -1;
    F<int32_t>(r, 8) = -1;
    F<int32_t>(r, 0x18) = -1;
    F<int32_t>(r, 0x1e) = -1;
    F<int32_t>(r, 0xc) = -1;
    r[0x1c] = 0;
    F<int32_t>(r, 0x10) = 0;
    F<int32_t>(r, 0x14) = 0;
    r[0x1d] = 1;
    F<uint16_t>(r, 0x22) = 0;
    for (int k = 0; k < 0x1e; ++k) r[0x24 + k] = 0;
    return r;
}
RT3_REPLACE(0x494e40, Route_Init);

void __fastcall Nop494e80(void*) {}
RT3_REPLACE(0x494e80, Nop494e80);
int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v) {
    F<int32_t>(r, 0) = v;
    F<int32_t>(r, 0x42) = 0;
    F<int32_t>(r, 0x46) = 0;
    return 0;
}
RT3_REPLACE(0x494e90, Route_SetOwner);
int32_t __fastcall Route_Add10(uint8_t* r, int, int32_t v) { return F<int32_t>(r, 0x10) += v; }
RT3_REPLACE(0x494eb0, Route_Add10);
void __fastcall Nop494ef0(void*, int, int32_t) {}
RT3_REPLACE(0x494ef0, Nop494ef0);

// 0x495170. Midpoint of (x0, y0) and (x1, y1).
void __fastcall Midpoint(float* ox, float* oy, float x0, float y0, float x1, float y1) {
    const double two = *(const double*)0x5c85d0;
    *ox = (float)(((double)x0 + x1) / two);
    *oy = (float)(((double)y0 + y1) / two);
}
RT3_REPLACE(0x495170, Midpoint);

// 0x4951a0. Point `pct` percent of the way from (x0, y0) to (x1, y1).
void __fastcall LerpPercent(int32_t pct, float* ox, float x0, float y0, float x1, float y1, float* oy) {
    float t = (float)pct;
    const float hundred = *(const float*)0x5c8c28;
    *ox = (float)(((double)x1 - x0) * t / hundred + x0);
    *oy = (float)(((double)y1 - y0) * t / hundred + y0);
}
RT3_REPLACE(0x4951a0, LerpPercent);

// 0x495420. a + b wrapped once into [0, 2 pi]; *wrapped tells whether it was.
float __fastcall AngleAdd(int32_t* wrapped, int, float a, float b) {
    float s = (float)((double)a + b);
    if (wrapped) *wrapped = 0;
    const double twoPi = *(const double*)0x5c96b8;
    if (s < *(const float*)0x5c8598) {
        s = (float)((double)s + twoPi);
    } else if ((double)s > twoPi) {
        s = (float)((double)s - twoPi);
    } else {
        return s;
    }
    if (wrapped) *wrapped = 1;
    return s;
}
RT3_REPLACE(0x495420, AngleAdd);

// 0x4955e0. Counter decay per the options +0x25b / +0x257.
void __fastcall Obj_DecayCounters(uint8_t* o) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x25b)) Obj_DecA4(o);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x257)) Obj_DecAc(o);
}
RT3_REPLACE(0x4955e0, Obj_DecayCounters);

// 0x4a3ca0 / 0x4a3cd0. Save / restore +0x98 in +0xfe (with +0x90 -> +0x102).
void __fastcall Obj_Stash98(uint8_t* o) {
    F<int32_t>(o, 0xfe) = F<int32_t>(o, 0x98);
    F<int32_t>(o, 0x98) = 0;
    F<int32_t>(o, 0x102) = F<int32_t>(o, 0x90);
}
RT3_REPLACE(0x4a3ca0, Obj_Stash98);
int32_t __fastcall Obj_Unstash98(uint8_t* o) {
    F<int32_t>(o, 0x98) = F<int32_t>(o, 0xfe);
    F<int32_t>(o, 0xfe) = 0;
    F<int32_t>(o, 0x102) = 0;
    return 0;
}
RT3_REPLACE(0x4a3cd0, Obj_Unstash98);

} // namespace rt3
