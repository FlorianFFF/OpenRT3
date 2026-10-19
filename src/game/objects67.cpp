// Renderer / time-of-day helpers in blocks 0x52 (batch 70). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Engine_Set1146f6(void* e, int, int32_t v);

void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v) { F<int32_t>(s, 0x22b6b) = v < 1 ? 1 : v; }
RT3_REPLACE(0x526650, Scene_SetDetail);
void __fastcall Scene_MarkDirty(uint8_t* s) { s[0x3749] = 1; }
RT3_REPLACE(0x5270d0, Scene_MarkDirty);

// 0x5270e0. Insertion sort of the 16-byte draw entries (+0x374f, count
// +0x22b53) by their key +8, ascending.
void __fastcall Scene_SortEntries(uint8_t* s) {
    uint32_t* a = (uint32_t*)(s + 0x374f);
    int32_t n = F<int32_t>(s, 0x22b53);
    for (int32_t i = 0; i < n; ++i) {
        uint32_t tmp[4] = { a[i * 4], a[i * 4 + 1], a[i * 4 + 2], a[i * 4 + 3] };
        int32_t j = i;
        for (; j > 0 && (int32_t)a[(j - 1) * 4 + 2] > (int32_t)tmp[2]; --j)
            for (int k = 0; k < 4; ++k) a[j * 4 + k] = a[(j - 1) * 4 + k];
        for (int k = 0; k < 4; ++k) a[j * 4 + k] = tmp[k];
    }
}
RT3_REPLACE(0x5270e0, Scene_SortEntries);

// 0x5275b0. Camera focus in cells (+0x3734, +0x373c scaled by 1/92); returns the flag byte +0x3746.
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y) {
    const float k = *(const float*)0x5dcfe8;
    *x = (float)((double)F<float>(s, 0x3734) * k);
    *y = (float)((double)F<float>(s, 0x373c) * k);
    return s[0x3746];
}
RT3_REPLACE(0x5275b0, Scene_FocusCell);

// 0x5275e0. Splits seconds of day v into hours / minutes / seconds (any output may be null).
void __fastcall TimeOfDay_Split(int32_t v, int32_t* hours, int32_t* minutes, int32_t* seconds) {
    if (seconds) *seconds = v % 60;
    if (minutes) *minutes = (v / 60) % 60;
    if (hours) *hours = (v / 3600) % 24;
}
RT3_REPLACE(0x5275e0, TimeOfDay_Split);

static inline int32_t SecondsOfDay(int32_t h, int32_t m, int32_t s) {
    int32_t sec = s % 60;
    int32_t mm = m + s / 60;
    int32_t min = mm % 60;
    int32_t hour = (h + mm / 60) % 24;
    return (hour * 60 + min) * 60 + sec;
}
// 0x527c20 / 0x527c70. Sets a time of day (normalised) into +0x11 / into +0xd and the engine.
void __fastcall Clock_Set11(uint8_t* o, int, int32_t h, int32_t m, int32_t s) { F<int32_t>(o, 0x11) = SecondsOfDay(h, m, s); }
RT3_REPLACE(0x527c20, Clock_Set11);
void __fastcall Clock_SetD(uint8_t* o, int, int32_t h, int32_t m, int32_t s) {
    int32_t v = SecondsOfDay(h, m, s);
    F<int32_t>(o, 0xd) = v;
    Engine_Set1146f6(hook::Global<void*>(0x6d4024), 0, v);
}
RT3_REPLACE(0x527c70, Clock_SetD);

double __fastcall Obj_Scene36f9(uint8_t* o) { return F<float>(F<uint8_t*>(o, 0x2d), 0x36f9); }
RT3_REPLACE(0x528180, Obj_Scene36f9);

void __fastcall Obj_GetVec1c(uint8_t* o, int, uint32_t* out) {
    out[0] = F<uint32_t>(o, 0x1c);
    out[1] = F<uint32_t>(o, 0x20);
    out[2] = F<uint32_t>(o, 0x24);
}
RT3_REPLACE(0x52cd20, Obj_GetVec1c);
void __fastcall Obj_GetVec4(uint8_t* o, int, uint32_t* out) {
    out[0] = F<uint32_t>(o, 4);
    out[1] = F<uint32_t>(o, 8);
    out[2] = F<uint32_t>(o, 0xc);
}
RT3_REPLACE(0x52cd40, Obj_GetVec4);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1c);
    *y = F<uint32_t>(o, 0x20);
    *z = F<uint32_t>(o, 0x24);
}
RT3_REPLACE(0x52cd60, Obj_GetXYZ1c);

} // namespace rt3
