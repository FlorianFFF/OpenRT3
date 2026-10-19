// Camera / scene helpers in blocks 0x48..0x52 (batch 73). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Engine_Renderer(void* e);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);

// 0x52d4d0. The camera bounds (+0x50..+0x5c, fractions) in pixels of the current display mode.
void __fastcall Camera_BoundsPixels(uint8_t* c, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    auto mode = []() {
        uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
        return F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    };
    *x0 = (int32_t)((double)F<int32_t>(mode(), 0) * F<float>(c, 0x50));
    *y0 = (int32_t)((double)F<int32_t>(mode(), 4) * F<float>(c, 0x54));
    *x1 = (int32_t)((double)F<int32_t>(mode(), 0) * F<float>(c, 0x58));
    *y1 = (int32_t)((double)F<int32_t>(mode(), 4) * F<float>(c, 0x5c));
}
RT3_REPLACE(0x52d4d0, Camera_BoundsPixels);

// 0x52d8a0. Attaches a child object: makes its position (+0x1e2..) relative
// to this object's (or to the parent transform +0x14), flags it 0x10 and
// appends it to the child list +0x75 (created on first use).
PtrListNode* __fastcall SceneObj_AddChild(uint8_t* o, int, uint8_t* child) {
    if (!F<PtrList*>(o, 0x75)) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(o, 0x75) = l ? PtrList_Init(l) : nullptr;
    }
    if (const float* p = F<float*>(o, 0x14)) {
        for (int k = 0; k < 3; ++k)
            F<float>(child, 0x1e2 + k * 4) = (float)((double)F<float>(child, 0x1e2 + k * 4) - F<float*>(o, 0x14)[k]);
    } else if (!(o[0x20] & 0x10)) {
        for (int k = 0; k < 3; ++k)
            F<float>(child, 0x1e2 + k * 4) = (float)((double)F<float>(child, 0x1e2 + k * 4) - F<float>(o, 0x1e2 + k * 4));
    }
    child[0x20] |= 0x10;
    return PtrList_Append(F<PtrList*>(o, 0x75), 0, child);
}
RT3_REPLACE(0x52d8a0, SceneObj_AddChild);

// 0x4825d0. Minutes of the scene clock since 5:15 (wrapping past midnight); 0 without a renderer.
float SceneClock_MinutesSinceDawn() {
    void* e = hook::Global<void*>(0x6d4024);
    if (!e || !Engine_Renderer(e)) return *(const float*)0x5c8598;
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, &h, &m, &s);
    double v = (double)m / *(const double*)0x5c88a8 + (double)s / *(const double*)0x5c9f30;
    float f = (float)(v + (double)h);
    if ((double)f < *(const double*)0x5cecf0) f = (float)((double)f + *(const float*)0x5c8968);
    return (float)(((double)f - *(const double*)0x5cecf0) * *(const double*)0x5c88a8);
}
RT3_REPLACE(0x4825d0, SceneClock_MinutesSinceDawn);

} // namespace rt3
