// Renderer helpers in blocks 0x43..0x52 (batch 71). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Engine_Renderer(void* e);
double __fastcall Obj_Scene36f9(uint8_t* o);
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);
void __fastcall TimeOfDay_Split(int32_t v, int32_t* hours, int32_t* minutes, int32_t* seconds);

static inline uint8_t* Renderer() { return (uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)); }

// 0x43aef0. Base +0x14 plus twice the scene scale times t's offset from
// the span centre, relative to the span (+0x54..+0x5c).
float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t) {
    float d = (float)((double)F<float>(o, 0x5c) - F<float>(o, 0x54));
    float base = F<float>(o, 0x14);
    Obj_Scene36f9(Renderer());  // result discarded by the original
    float r = (float)(((double)t - (double)d * 0.5) / d);
    double v = Obj_Scene36f9(Renderer()) * r;
    v = v + v;
    return (float)(v + base);
}
RT3_REPLACE(0x43aef0, Obj_ScaledSpanOffset);

int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y) { return Scene_FocusCell(Renderer(), 0, x, y) == 1 ? 1 : 0; }
RT3_REPLACE(0x448ac0, Scene_FocusFlagIs1);

RT3_NOINLINE void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s) { TimeOfDay_Split(F<int32_t>(o, 0xd), h, m, s); }
RT3_REPLACE(0x527cc0, Clock_SplitD);

static inline void Get3(uint8_t* o, uint32_t off, uint32_t* a, uint32_t* b, uint32_t* c) {
    *a = F<uint32_t>(o, off);
    *b = F<uint32_t>(o, off + 4);
    *c = F<uint32_t>(o, off + 8);
}
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) { Get3(o, 4, a, b, c); }
RT3_REPLACE(0x52cd80, Obj_GetXYZ4);
void __fastcall Obj_GetXYZ2c(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) { Get3(o, 0x2c, a, b, c); }
RT3_REPLACE(0x52cda0, Obj_GetXYZ2c);
void __fastcall Obj_SetXYZ2c(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    F<uint32_t>(o, 0x2c) = a;
    F<uint32_t>(o, 0x30) = b;
    F<uint32_t>(o, 0x34) = c;
}
RT3_REPLACE(0x52cdc0, Obj_SetXYZ2c);

// 0x52cde0 / 0x52ce20. Sets a lock flag (+0x64 / +0x65); unlocking snapshots the current view state.
void __fastcall Camera_SetLock64(uint8_t* c, int, int32_t v) {
    c[0x64] = (uint8_t)v;
    if (v) return;
    F<uint32_t>(c, 0x44) = F<uint32_t>(c, 0x10);
    F<uint32_t>(c, 0x48) = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x38) = F<uint32_t>(c, 4);
    F<uint32_t>(c, 0x3c) = F<uint32_t>(c, 8);
    F<uint32_t>(c, 0x40) = F<uint32_t>(c, 0xc);
    F<uint32_t>(c, 0x4c) = F<uint32_t>(c, 0x18);
}
RT3_REPLACE(0x52cde0, Camera_SetLock64);
void __fastcall Camera_SetLock65(uint8_t* c, int, int32_t v) {
    c[0x65] = (uint8_t)v;
    if (v) return;
    F<uint32_t>(c, 0x72) = F<uint32_t>(c, 0x10);
    F<uint32_t>(c, 0x76) = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x7a) = F<uint32_t>(c, 4);
    F<uint32_t>(c, 0x7e) = F<uint32_t>(c, 8);
    F<uint32_t>(c, 0x82) = F<uint32_t>(c, 0xc);
    F<uint32_t>(c, 0x86) = F<uint32_t>(c, 0x18);
}
RT3_REPLACE(0x52ce20, Camera_SetLock65);

// 0x52ce70. Whether the camera changed (or the engine frame +0x114222 moved on); clears the flag.
int32_t __fastcall Camera_TakeChanged(uint8_t* c) {
    int32_t frame = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114222);
    if (frame != F<int32_t>(c, 0x6a)) {
        c[0x64] = 1;
        F<int32_t>(c, 0x6a) = frame;
    }
    int32_t r = c[0x64];
    c[0x64] = 0;
    return r;
}
RT3_REPLACE(0x52ce70, Camera_TakeChanged);

void __fastcall Obj_GetXY10(uint8_t* o, int, uint32_t* a, uint32_t* b) {
    *a = F<uint32_t>(o, 0x10);
    *b = F<uint32_t>(o, 0x14);
}
RT3_REPLACE(0x52cea0, Obj_GetXY10);

// 0x52d0f0. Point (x, y) inside the bounds +0x50..+0x5c (inclusive).
RT3_NOINLINE int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y) {
    if (!(x >= F<float>(c, 0x50)) || !(y >= F<float>(c, 0x54))) return 0;
    if (!(x <= F<float>(c, 0x58)) || !(y <= F<float>(c, 0x5c))) return 0;
    return 1;
}
RT3_REPLACE(0x52d0f0, Camera_InBounds);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e) {
    F<uint32_t>(c, 0x50) = a;
    F<uint32_t>(c, 0x54) = b;
    F<uint32_t>(c, 0x58) = d;
    F<uint32_t>(c, 0x5c) = e;
}
RT3_REPLACE(0x52d140, Camera_SetBounds);

// 0x52d160. Changes the mode +0x60 (after virtual slot 4) when different.
void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode) {
    if (F<int32_t>(c, 0x60) == mode) return;
    ((void(__thiscall*)(void*))(*(void***)c)[0x10 / 4])(c);
    F<int32_t>(c, 0x60) = mode;
}
RT3_REPLACE(0x52d160, Camera_SetMode);

} // namespace rt3
