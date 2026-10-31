// Screen mapping / texture filtering / light state in blocks 0x52..0x54 (batch 99). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Render_Project(uint8_t* r, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Render_GetViewport(uint8_t* r, int, uint32_t* sx, uint32_t* sy, uint32_t* ox, uint32_t* oy);

static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline int32_t SetTss(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, state, v);
}
static inline uint8_t* Cfg(uint8_t* r) { return F<uint8_t*>(r, 0x83c); }

// 0x527450. Projects a world point to normalised screen coordinates
// (x: 0..1 left to right, y: 1..0 bottom-up); 0 when culled.
RT3_NOINLINE int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    if (Render_Project(F<uint8_t*>(s, 0x2d), 0, x, y, z, ox, oy, oz)) return 0;
    float sx, sy;
    Render_GetViewport(F<uint8_t*>(s, 0x2d), 0, (uint32_t*)&sx, (uint32_t*)&sy, nullptr, nullptr);
    const double half = *(const double*)0x5c8628;
    *ox = (float)((double)*ox / sx * half + half / sx);
    *oy = (float)(half / sy - (double)*oy / sy * half);
    return 1;
}
RT3_REPLACE(0x527450, Scene_ProjectNormalised);

int32_t __fastcall Render_SetTexCoordIndex(uint8_t* r, int, uint32_t stage, uint32_t v) { return SetTss(r, stage, 0xb, v); }
RT3_REPLACE(0x544dd0, Render_SetTexCoordIndex);

// 0x544f30. Texture filtering for all stages (+0x258f) or one: point, linear, or
// anisotropic (config +0x1e; max anisotropy 2), with dithering when filtered.
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage) {
    int32_t end = F<int32_t>(r, 0x258f), start = 0;
    uint32_t mode = 1;
    if (stage >= 0) {
        start = stage;
        end = stage + 1;
    }
    if (filtered) {
        mode = 2;
        if (Cfg(r)[0x1e]) {
            mode = 3;
            for (int32_t n = end - start; start < end && n; --n) SetTss(r, 0, 0x15, 2);
        }
        SetRs(r, 0x1a, 1);
    }
    for (int32_t s = start; s < end; ++s) {
        SetTss(r, s, 0x11, mode);
        SetTss(r, s, 0x10, mode);
    }
}
RT3_REPLACE(0x544f30, Render_SetFiltering);

int32_t __fastcall Render_LightingOff(uint8_t* r) { return SetRs(r, 0x89, 0); }
RT3_REPLACE(0x545000, Render_LightingOff);
int32_t __fastcall Render_LightingDefault(uint8_t* r) { return SetRs(r, 0x89, Cfg(r)[0x22]); }
RT3_REPLACE(0x545020, Render_LightingDefault);
int32_t __fastcall Render_LightingAndSun(uint8_t* r) {
    SetRs(r, 0x89, Cfg(r)[0x22]);
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, int32_t))(*(void***)d)[0xb8 / 4])(d, 0, Cfg(r)[0x22]);
}
RT3_REPLACE(0x545040, Render_LightingAndSun);
int32_t __fastcall Render_ZWriteOff(uint8_t* r) { return SetRs(r, 0xe, 0); }
RT3_REPLACE(0x545080, Render_ZWriteOff);
int32_t __fastcall Render_ZWriteOn(uint8_t* r) { return SetRs(r, 0xe, 1); }
RT3_REPLACE(0x5450a0, Render_ZWriteOn);

void __fastcall Render_GetFogBlock(uint8_t* r, int, uint8_t* dst) {
    for (int k = 0; k < 0x33; ++k) dst[k] = r[0x24f0 + k];
}
RT3_REPLACE(0x5450c0, Render_GetFogBlock);

// 0x5450e0 / 0x545130. Sun direction (negated +0x2517/+0x251b/+0x251f; z scaled
// by 0x5c8758, height at least 0.5).
void __fastcall Render_SunDir(uint8_t* r, int, float* x, float* y, float* z) {
    *x = -F<float>(r, 0x2517);
    *z = (float)(-(double)F<float>(r, 0x251f) * *(const double*)0x5c8758);
    double h = -(double)F<float>(r, 0x251b);
    *y = (float)h;
    if (h < *(const double*)0x5c8628) *(uint32_t*)y = 0x3f000000;
}
RT3_REPLACE(0x5450e0, Render_SunDir);
void __fastcall Render_SunDirVec(uint8_t* r, int, float* v) {
    v[0] = -F<float>(r, 0x2517);
    v[2] = (float)(-(double)F<float>(r, 0x251f) * *(const double*)0x5c8758);
    double h = -(double)F<float>(r, 0x251b);
    v[1] = (float)h;
    if (h < *(const double*)0x5c8628) *(uint32_t*)&v[1] = 0x3f000000;
}
RT3_REPLACE(0x545130, Render_SunDirVec);

// Light records (0x33 bytes at +0x840) and the two-value table +0x25f7/+0x25ff.
void __fastcall Render_SetLightRange(uint8_t* r, int, int32_t i, float v) { F<float>(r, 0x863 + i * 0x33) = v; }
RT3_REPLACE(0x545400, Render_SetLightRange);
void __fastcall Render_SetPair25f7(uint8_t* r, int, int32_t i, float a, float b) {
    F<float>(r, 0x25f7 + i * 4) = a;
    F<float>(r, 0x25ff + i * 4) = b;
}
RT3_REPLACE(0x545420, Render_SetPair25f7);
void __fastcall Render_GetPair25f7(uint8_t* r, int, int32_t i, uint32_t* a, uint32_t* b) {
    if (a) *a = F<uint32_t>(r, 0x25f7 + i * 4);
    if (b) *b = F<uint32_t>(r, 0x25ff + i * 4);
}
RT3_REPLACE(0x545440, Render_GetPair25f7);
void __fastcall Render_SetLightPair(uint8_t* r, int, int32_t i, int32_t j, float a, float b) {
    F<float>(r, 0x840 + i * 0x33 + j * 4) = a;
    F<float>(r, 0x848 + i * 0x33 + j * 4) = b;
}
RT3_REPLACE(0x545470, Render_SetLightPair);

} // namespace rt3
