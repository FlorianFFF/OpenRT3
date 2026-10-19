// Functions in block 0x47 (batch 37). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Rand(int32_t lo, int32_t hi);

// 0x477a10. Inverse linear map of y over [+0xc7, +0xcf] scaled by +0x17 plus the offsets (+0x86)->+9 and +0xf.
float __fastcall Obj_MapY(uint8_t* o, int, float y) {
    float lo = (float)F<int32_t>(o, 0xc7);
    float span = (float)(F<int32_t>(o, 0xcf) - F<int32_t>(o, 0xc7));
    double v = ((double)y - lo) / span;
    v = (*(const double*)0x5c8770 - v) * F<float>(o, 0x17);
    double off = (double)F<float>(F<uint8_t*>(o, 0x86), 9) + F<float>(o, 0xf);
    return (float)(v + off);
}
RT3_REPLACE(0x477a10, Obj_MapY);

// 0x477a70. Largest extent (+0xb3..+0xbf, at least +0x11f) times the scale +0xdb, truncated.
float __fastcall Obj_ScaledExtent(uint8_t* o) {
    int32_t w = F<int32_t>(o, 0xbb) - F<int32_t>(o, 0xb3);
    int32_t h = F<int32_t>(o, 0xbf) - F<int32_t>(o, 0xb7);
    if (w <= h) w = h;
    if (w < F<int32_t>(o, 0x11f)) w = F<int32_t>(o, 0x11f);
    float fw = (float)w;
    return (float)(int32_t)((double)fw * F<float>(o, 0xdb));
}
RT3_REPLACE(0x477a70, Obj_ScaledExtent);

void ClearGlobals6ceba4() {
    for (int i = 0; i < 10; ++i) ((int32_t*)0x6ceba4)[i] = 0;
}
RT3_REPLACE(0x47ba60, ClearGlobals6ceba4);

void* __fastcall ReturnThis47bbd0(void* o) { return o; }
RT3_REPLACE(0x47bbd0, ReturnThis47bbd0);
void __fastcall Nop47bbe0(void*) {}
RT3_REPLACE(0x47bbe0, Nop47bbe0);
void __fastcall Nop47d8d0(void*) {}
RT3_REPLACE(0x47d8d0, Nop47d8d0);

// 0x47bbf0. Resets the new-game config: start year 1830, month 1, a random
// map seed (Rand 0..32767) at +0x83 and the default flags.
void __fastcall NewGameConfig_Reset(uint8_t* c) {
    for (int i = 0; i < 0x42; ++i) ((uint32_t*)c)[i] = 0;
    c[0x7d] = 1;
    F<uint16_t>(c, 0x77) = 0x726;
    F<uint16_t>(c, 0x79) = 1;
    F<uint16_t>(c, 0x7b) = 0;
    int32_t seed = Rand(0, 0x7fff);
    c[0xc4] = 0;
    c[0xc5] = 0;
    c[0x82] = 0;
    F<int32_t>(c, 1) = 0;
    F<int32_t>(c, 0x83) = seed;
    c[0x87] = 1;
    c[0x97] = 1;
}
RT3_REPLACE(0x47bbf0, NewGameConfig_Reset);

// 0x47bc50. First of the 16 player slots (config +0xc6) whose state is below 2; 16 if none.
int32_t NewGameConfig_FirstOpenSlot() {
    const uint8_t* c = hook::Global<uint8_t*>(0x6cec7c);
    for (int32_t i = 0; i < 0x10; ++i)
        if (c[0xc6 + i] < 2) return i;
    return 0x10;
}
RT3_REPLACE(0x47bc50, NewGameConfig_FirstOpenSlot);

void __fastcall Obj_Set3c(void* o, int, uint16_t v) { F<uint16_t>(o, 0x3c) = v; }
RT3_REPLACE(0x47d6f0, Obj_Set3c);
double __fastcall Obj_Float4c4(void* o, int, int32_t i) { return F<float>(o, 0x4c4 + i * 4); }
RT3_REPLACE(0x47d7c0, Obj_Float4c4);
double __fastcall Obj_Float4ec(void* o, int, int32_t i) { return F<float>(o, 0x4ec + i * 4); }
RT3_REPLACE(0x47d7d0, Obj_Float4ec);
double __fastcall Obj_Float514(void* o, int, int32_t i) { return F<float>(o, 0x514 + i * 4); }
RT3_REPLACE(0x47d7e0, Obj_Float514);
double __fastcall Obj_Float53c(void* o, int, int32_t i) { return F<float>(o, 0x53c + i * 4); }
RT3_REPLACE(0x47d7f0, Obj_Float53c);
int32_t __fastcall Obj_Int564(void* o, int, int32_t i) { return F<int32_t>(o, 0x564 + i * 4); }
RT3_REPLACE(0x47d800, Obj_Int564);

} // namespace rt3
