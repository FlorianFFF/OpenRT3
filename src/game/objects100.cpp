// Window placement / stream and alpha state / screen matrices / gamma in blocks 0x52..0x54 (batch 103). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Nop545df0(void*, int, int32_t, int32_t);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline void** Vt(void* p) { return *(void***)p; }
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}
static inline int32_t SetTss(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))Vt(d)[0xfc / 4])(d, stage, type, v);
}
static inline int32_t SetTransform(uint8_t* r, uint32_t state, const void* m) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, const void*))Vt(d)[0x94 / 4])(d, state, m);
}
static inline uint8_t* Cfg(uint8_t* r) { return F<uint8_t*>(r, 0x83c); }

// 0x520250. Moves the window client origin to (x, y) (window frame added).
void __fastcall Engine_PlaceWindow(uint8_t* e, int, int32_t x, int32_t y) {
    if (!F<void*>(e, 0xc)) return;
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t rc[4] = { 0, 0, mode[0] - 1, mode[1] - 1 };
    Import<int32_t(__stdcall*)(int32_t*, uint32_t, int32_t)>(0x5c82b4)(rc, 0x4cb0000, 0);
    int32_t wx = x + rc[0], wy = y + rc[1];
    F<int32_t>(e, 0x51) = wy;
    F<int32_t>(e, 0x4d) = wx;
    Nop545df0(F<void*>(e, 0xc), 0, wx, wy);
}
RT3_REPLACE(0x520250, Engine_PlaceWindow);

// 0x546a00. Clears the 64 cached texture slots (+0x266b, 0x2d bytes).
void __fastcall Render_ResetTextureSlots(uint8_t* r) {
    F<int32_t>(r, 0x2667) = 0;
    for (int k = 0; k < 0x40; ++k) {
        uint8_t* s = r + 0x266b + k * 0x2d;
        F<int32_t>(s, 4) = 0;
        F<int32_t>(s, 0) = 0;
        F<int32_t>(s, 0x18) = 0;
        F<int32_t>(s, 0x14) = 0;
        F<int32_t>(s, 0x25) = 0;
        s[0x20] = 0;
    }
}
RT3_REPLACE(0x546a00, Render_ResetTextureSlots);

// 0x5471f0. Binds a vertex buffer (stride 0x18 / 0x20 / other picks the FVF) and
// index buffer, skipping unchanged ones (+0x2637 / +0x263b).
void __fastcall Render_SetStreams(uint8_t* r, int, void* vb, uint32_t stride, void* ib) {
    if (!vb) {
        F<void*>(r, 0x2637) = nullptr;
        return;
    }
    if (F<void*>(r, 0x2637) != vb) {
        void* d = Dev(r);
        uint32_t fvf = stride == 0x18 ? 0x142 : stride == 0x20 ? 0x112 : 0x152;
        ((int32_t(__stdcall*)(void*, uint32_t))Vt(d)[0x130 / 4])(d, fvf);
        d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t))Vt(d)[0x14c / 4])(d, 0, vb, stride);
        F<void*>(r, 0x2637) = vb;
    }
    if (!ib) {
        F<void*>(r, 0x263b) = nullptr;
        return;
    }
    if (F<void*>(r, 0x263b) == ib) return;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*, uint32_t))Vt(d)[0x154 / 4])(d, ib, 0);
    F<void*>(r, 0x263b) = ib;
}
RT3_REPLACE(0x5471f0, Render_SetStreams);

int32_t __fastcall Render_BlendOff(uint8_t* r) { return SetRs(r, 0x1b, 0); }
RT3_REPLACE(0x5472a0, Render_BlendOff);
void __fastcall Render_AllowTranslucency(uint8_t* r) {
    if (Engine()[0x11422e] || Engine()[0x11422f]) r[0x36f8] = 1;
}
RT3_REPLACE(0x5472c0, Render_AllowTranslucency);
void __fastcall Render_NoTranslucency(uint8_t* r) { r[0x36f8] = 0; }
RT3_REPLACE(0x5472f0, Render_NoTranslucency);
int32_t __fastcall Render_BlendDefault(uint8_t* r) { return SetRs(r, 0x1b, Cfg(r)[0x21] & 1); }
RT3_REPLACE(0x547300, Render_BlendDefault);

// 0x547320 / 0x547380. Alpha test (ref, func 7 = greater-equal) when supported (+0x114252).
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref) {
    if (!Engine()[0x114252]) return;
    SetRs(r, 0xf, 1);
    SetRs(r, 0x18, ref);
    SetRs(r, 0x19, 7);
}
RT3_REPLACE(0x547320, Render_AlphaTestOn);
void __fastcall Render_AlphaTestOff(uint8_t* r) {
    if (!Engine()[0x114252]) return;
    SetRs(r, 0xf, 0);
    SetRs(r, 0x18, 0);
}
RT3_REPLACE(0x547380, Render_AlphaTestOff);

// Screen-space world matrix: x * 2 - 1, -y * 2 + 1.
static inline void ScreenMatrix(uint32_t* m) {
    for (int k = 0; k < 16; ++k) m[k] = 0;
    m[15] = 0x3f800000;
    m[10] = 0x3f800000;
    m[0] = 0x40000000;
    m[5] = 0xc0000000;
    m[12] = 0xbf800000;
    m[13] = 0x3f800000;
}

// 0x5473c0. Drops the saved state block (+0x25e7) and sets the screen-space world matrix.
void __fastcall Render_ScreenSpaceWorld(uint8_t* r) {
    if (void* sb = F<void*>(r, 0x25e7)) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, sb);
        Free(F<void*>(r, 0x25e7));
        F<void*>(r, 0x25e7) = nullptr;
    }
    uint32_t m[16];
    ScreenMatrix(m);
    SetTransform(r, 0x100, m);
}
RT3_REPLACE(0x5473c0, Render_ScreenSpaceWorld);

// 0x547610 / 0x547640. Restores the back buffer render target once (+0x254c).
RT3_NOINLINE void __fastcall Render_RestoreTarget(uint8_t* r) {
    if (!r[0x254c]) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, void*, void*))Vt(d)[0x7c / 4])(d, F<void*>(r, 0x2607), F<void*>(r, 0x260b));
    }
    r[0x254c] = 1;
}
RT3_REPLACE(0x547610, Render_RestoreTarget);
void __fastcall Render_TargetChanged(uint8_t* r) { r[0x254c] = 0; }
RT3_REPLACE(0x547640, Render_TargetChanged);

// 0x547c30. Stores the fog/texture offset (+0x36df..) and uploads it as vertex
// shader constant 7 ({-a, -b, c, d}, or {0, 0, 1, 1} when all zero).
void __fastcall Render_SetShaderOffset(uint8_t* r, int, float a, float b, float c, float d) {
    const float zero = *(const float*)0x5c8598;
    F<float>(r, 0x36df) = a;
    F<float>(r, 0x36e7) = c;
    F<float>(r, 0x36e3) = b;
    F<float>(r, 0x36eb) = d;
    float v[4];
    if (a == zero && b == zero && c == zero && d == zero) {
        F<uint32_t>(v, 0) = 0;
        F<uint32_t>(v, 4) = 0;
        F<uint32_t>(v, 8) = 0x3f800000;
        F<uint32_t>(v, 0xc) = 0x3f800000;
    } else {
        v[0] = -a;
        v[2] = c;
        v[1] = -b;
        v[3] = d;
    }
    void* dv = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, const void*, uint32_t))Vt(dv)[0x13c / 4])(dv, 7, v, 1);
}
RT3_REPLACE(0x547c30, Render_SetShaderOffset);

// 0x547d10. Stage 0 texture transform: scale (c, d) and offset (a, b) via the
// third row; disabled when all are zero.
void __fastcall Render_SetTextureTransform(uint8_t* r, int, float a, float b, float c, float d) {
    const float zero = *(const float*)0x5c8598;
    if (a == zero && b == zero && c == zero && d == zero) {
        SetTss(r, 0, 0x18, 0);
        return;
    }
    float m[16];
    for (int k = 0; k < 16; ++k) F<uint32_t>(m, k * 4) = 0;
    m[0] = c;
    m[8] = a;
    m[15] = 1.0f;
    m[5] = d;
    m[9] = b;
    SetTss(r, 0, 0x18, 2);
    SetTransform(r, 0x10, m);
}
RT3_REPLACE(0x547d10, Render_SetTextureTransform);

// 0x547e30. Identity view and projection, screen-space world.
void __fastcall Render_ScreenSpaceAll(uint8_t* r) {
    uint32_t m[16] = {};
    m[15] = m[10] = m[5] = m[0] = 0x3f800000;
    SetTransform(r, 2, m);
    SetTransform(r, 3, m);
    m[0] = 0x40000000;
    m[5] = 0xc0000000;
    m[12] = 0xbf800000;
    m[13] = 0x3f800000;
    SetTransform(r, 0x100, m);
}
RT3_REPLACE(0x547e30, Render_ScreenSpaceAll);

// 0x547f20. Sets a linear gamma ramp scaled by `gamma` (fullscreen with gamma
// support only) and remembers it (config +0x2f).
void __fastcall Render_SetGamma(uint8_t* r, int, float gamma) {
    uint8_t* e = Engine();
    if (!e[0x114226] || !e[0x114231]) return;
    uint16_t ramp[3][256];
    for (int32_t i = 0, v = 0; v < 0xff00; ++i, v += 0xff) {
        int32_t x = (int32_t)((double)v * gamma);
        if (x > 0xffff) x = 0xffff;
        ramp[2][i] = (uint16_t)x;
        ramp[1][i] = (uint16_t)x;
        ramp[0][i] = (uint16_t)x;
    }
    void* d = Dev(r);
    ((void(__stdcall*)(void*, uint32_t, const void*))Vt(d)[0x48 / 4])(d, 1, ramp);
    F<float>(Cfg(r), 0x2f) = gamma;
}
RT3_REPLACE(0x547f20, Render_SetGamma);

// 0x547fe0. Re-applies the stored view (+0x346f), projection (+0x352f) and world (+0x34ef).
void __fastcall Render_ApplyStoredTransforms(uint8_t* r) {
    SetTransform(r, 2, r + 0x346f);
    SetTransform(r, 3, r + 0x352f);
    SetTransform(r, 0x100, r + 0x34ef);
}
RT3_REPLACE(0x547fe0, Render_ApplyStoredTransforms);

} // namespace rt3
