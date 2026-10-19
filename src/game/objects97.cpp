// Light/stage state caching / material / vertex buffer locks in blocks 0x43..0x54 (batch 100). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Render_SetLightPair(uint8_t* r, int, int32_t i, int32_t j, float a, float b);
void __fastcall Render_SetLightRange(uint8_t* r, int, int32_t i, float v);
void __fastcall Render_SetPair25f7(uint8_t* r, int, int32_t i, float a, float b);

static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline int32_t SetTss(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}
static inline int32_t VbLock(void* vb, uint32_t off, uint32_t size, void* pp, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))(*(void***)vb)[0x2c / 4])(vb, off, size, pp, flags);
}

// 0x439b20. Normalised screen position of a tile point (tile x, tile y, height).
int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy) {
    const float k = *(const float*)0x5c8700;
    float wz = (float)((double)ty * k);
    float wx = (float)((double)tx * k);
    return Scene_ProjectNormalised((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, wx, h, wz, ox, oy, nullptr);
}
RT3_REPLACE(0x439b20, Map_TileToScreen);

static inline int32_t ClampLight(int32_t i) { return i >= 0x90 ? 0x8f : i < 0 ? 0 : i; }
void __fastcall Scene_SetLightPair0(uint8_t* s, int, int32_t i, float a, float b) {
    Render_SetLightPair(F<uint8_t*>(s, 0x2d), 0, ClampLight(i), 0, a, b);
}
RT3_REPLACE(0x5274e0, Scene_SetLightPair0);
void __fastcall Scene_SetLightPair1(uint8_t* s, int, int32_t i, float a, float b) {
    Render_SetLightPair(F<uint8_t*>(s, 0x2d), 0, ClampLight(i), 1, a, b);
}
RT3_REPLACE(0x527510, Scene_SetLightPair1);
void __fastcall Scene_SetLightRange(uint8_t* s, int, int32_t i, float v) { Render_SetLightRange(F<uint8_t*>(s, 0x2d), 0, i, v); }
RT3_REPLACE(0x527560, Scene_SetLightRange);
void __fastcall Scene_SetPair0(uint8_t* s, int, float a, float b) { Render_SetPair25f7(F<uint8_t*>(s, 0x2d), 0, 0, a, b); }
RT3_REPLACE(0x527570, Scene_SetPair0);
void __fastcall Scene_SetPair1(uint8_t* s, int, float a, float b) { Render_SetPair25f7(F<uint8_t*>(s, 0x2d), 0, 1, a, b); }
RT3_REPLACE(0x527590, Scene_SetPair1);

float __fastcall Render_GetLightValue(uint8_t* r, int, int32_t i, int32_t j) { return F<float>(r + i * 0x33, 0x840 + j * 4); }
RT3_REPLACE(0x5454a0, Render_GetLightValue);

// 0x5454c0. Pushes a copy of the texture stage state cache (+0x2597, 24 bytes per
// stage, +0x258f stages) onto the list +0x259f.
void* __fastcall Render_PushStageState(uint8_t* r) {
    uint32_t n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    uint8_t* p = (uint8_t*)Malloc(n);
    const uint8_t* src = F<uint8_t*>(r, 0x2597);
    n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    for (uint32_t k = 0; k < n; ++k) p[k] = src[k];
    PtrList_Append(F<PtrList*>(r, 0x259f), 0, p);
    return p;
}
RT3_REPLACE(0x5454c0, Render_PushStageState);

// 0x5455e0 / 0x5456d0. Colour / alpha op of a stage (op, arg1, arg2), set on the
// device only where the cache differs (or just cached when not applying).
static inline void SetStage3(uint8_t* r, int32_t stage, uint32_t base, uint32_t tss, uint32_t arg1, uint32_t op,
                             uint32_t arg2, uint8_t apply) {
    if (stage >= F<int32_t>(r, 0x258f)) return;
    uint32_t at = stage * 24 + base;
    if (!apply) {
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at) = op;
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) = arg1;
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) = arg2;
        return;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at) != op) {
        SetTss(r, stage, tss, op);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at) = op;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) != arg1) {
        SetTss(r, stage, tss + 1, arg1);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) = arg1;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) != arg2) {
        SetTss(r, stage, tss + 2, arg2);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) = arg2;
    }
}
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply) {
    SetStage3(r, stage, 0, 1, arg1, op, arg2, apply);
}
RT3_REPLACE(0x5455e0, Render_SetColorOp);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply) {
    SetStage3(r, stage, 0xc, 4, arg1, op, arg2, apply);
}
RT3_REPLACE(0x5456d0, Render_SetAlphaOp);

// 0x5457c0. Default material (diffuse 1, ambient 0.1; specular 0.3 with power
// +0x25fb x +0x24f4 x 0x5c8568 when shiny and supported) and ambient light 0xffafafaf.
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny) {
    uint32_t m[17] = {};
    if (shiny && hook::Global<uint8_t*>(0x6d4024)[0x11422e]) {
        for (int k = 8; k < 12; ++k) m[k] = 0x3e99999a;
        float p = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4) * *(const float*)0x5c8568);
        m[16] = *(uint32_t*)&p;
    }
    for (int k = 0; k < 4; ++k) m[k] = 0x3f800000;
    for (int k = 4; k < 8; ++k) m[k] = 0x3dcccccd;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, 0x8b, 0xffafafaf);
    d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))(*(void***)d)[0xa8 / 4])(d, m);
}
RT3_REPLACE(0x5457c0, Render_SetDefaultMaterial);

// 0x5458a0 / 0x5458d0. Locks / unlocks the whole vertex buffer +0x265b (lock count 0xccd824).
void* __fastcall Render_LockVb2(uint8_t* r) {
    void* p;
    hook::Global<int32_t>(0xccd824)++;
    VbLock(F<void*>(r, 0x265b), 0, 0, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x5458a0, Render_LockVb2);
int32_t __fastcall Render_UnlockVb2(uint8_t* r) {
    hook::Global<int32_t>(0xccd824)--;
    void* vb = F<void*>(r, 0x265b);
    return ((int32_t(__stdcall*)(void*))(*(void***)vb)[0x30 / 4])(vb);
}
RT3_REPLACE(0x5458d0, Render_UnlockVb2);

// 0x5458f0. Locks vertices first..end (32 bytes each; to 0xffff when end is 0) of
// the dynamic buffer +0x2643 (once, flag +0x254e) without overwriting; returns +0x2657.
void* __fastcall Render_LockVbRange(uint8_t* r, int, int32_t first, int32_t end) {
    if (r[0x254e]) return nullptr;
    uint8_t desc[0x18];
    void* vb = F<void*>(r, 0x2643);
    r[0x254e] = 1;
    ((int32_t(__stdcall*)(void*, void*))(*(void***)vb)[0x34 / 4])(vb, desc);
    hook::Global<int32_t>(0xccd824)++;
    uint32_t size = end ? (uint32_t)(end << 5) - (uint32_t)(first << 5) : (uint32_t)(0xffff - first) << 5;
    VbLock(F<void*>(r, 0x2643), (uint32_t)first << 5, size, r + 0x2657, 0x1800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x5458f0, Render_LockVbRange);

// 0x5459a0. Locks the first n vertices of the dynamic buffer with discard.
void* __fastcall Render_LockVbDiscard(uint8_t* r, int, int32_t n) {
    if (r[0x254e]) return nullptr;
    r[0x254e] = 1;
    hook::Global<int32_t>(0xccd824)++;
    VbLock(F<void*>(r, 0x2643), 0, (uint32_t)n << 5, r + 0x2657, 0x2800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x5459a0, Render_LockVbDiscard);

} // namespace rt3
