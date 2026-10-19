// Blend modes / stage-state stack / stream presets / message pump in blocks 0x52..0x54 (batch 104). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Render_RestoreTarget(uint8_t* r);
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetStreams(uint8_t* r, int, void* vb, uint32_t stride, void* ib);

static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline void** Vt(void* p) { return *(void***)p; }
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}
static inline double Fsqrt(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

void __fastcall Scene_RestoreTarget(uint8_t* s) { Render_RestoreTarget(F<uint8_t*>(s, 0x2d)); }
RT3_REPLACE(0x5271a0, Scene_RestoreTarget);

// 0x548030 / 0x548060. Blend factors: (ZERO, INVSRCALPHA) / (SRCALPHA, INVSRCALPHA).
int32_t __fastcall Render_BlendDarken(uint8_t* r) {
    SetRs(r, 0x13, 1);
    return SetRs(r, 0x14, 6);
}
RT3_REPLACE(0x548030, Render_BlendDarken);
int32_t __fastcall Render_BlendAlpha(uint8_t* r) {
    SetRs(r, 0x13, 5);
    return SetRs(r, 0x14, 6);
}
RT3_REPLACE(0x548060, Render_BlendAlpha);

double __fastcall Render_ScaleByUnit(uint8_t*, int, float v) { return (double)v * *(const float*)0x5ce4fc; }
RT3_REPLACE(0x548090, Render_ScaleByUnit);

// 0x5480a0. Distance from q to the line p + t d (t = (q - p) . d).
double __stdcall PointLineDistance(float px, float py, float pz, float dx, float dy, float dz, float qx, float qy,
                                   float qz) {
    double a = (double)qx - px, b = (double)qy - py, c = (double)qz - pz;
    double t = (a * dx + c * dz) + b * dy;
    float ex = (float)((double)dx * t + px - qx);
    float ey = (float)((double)dy * t + py - qy);
    double ez = t * dz + pz - qz;
    return Fsqrt((double)ex * ex + (double)ey * ey + ez * ez);
}
RT3_REPLACE(0x5480a0, PointLineDistance);

// 0x548120. Identity world (D3DX translation 0) and the projection for the stored size.
void __fastcall Render_ResetWorld(uint8_t* r) {
    uint8_t* m = r + 0x34ef;
    ((void*(__stdcall*)(void*, float, float, float))0x56dd2a)(m, 0.0f, 0.0f, 0.0f);
    F<uint32_t>(r, 0x352b) = 0x3f800000;
    F<uint32_t>(r, 0x3517) = 0x3f800000;
    F<uint32_t>(r, 0x3503) = 0x3f800000;
    F<uint32_t>(m, 0) = 0x3f800000;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0x94 / 4])(d, 0x100, m);
    Render_SetProjection(r, 0, F<int32_t>(r, 0x25c7), F<int32_t>(r, 0x25cb));
}
RT3_REPLACE(0x548120, Render_ResetWorld);

// 0x548820. Resets the stage cache to MODULATE(TEXTURE, DIFFUSE) for colour and alpha.
void __fastcall Render_ResetStageCache(uint8_t* r) {
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s) {
        uint32_t* st = (uint32_t*)(F<uint8_t*>(r, 0x2597) + s * 24);
        st[0] = 1;
        st[1] = 2;
        st[2] = 1;
        if (s < F<int32_t>(r, 0x258f)) {
            st = (uint32_t*)(F<uint8_t*>(r, 0x2597) + s * 24);
            st[3] = 1;
            st[4] = 2;
            st[5] = 1;
        }
    }
}
RT3_REPLACE(0x548820, Render_ResetStageCache);

static inline void ApplyStages(uint8_t* r, const uint32_t* st) {
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s, st += 6) {
        Render_SetColorOp(r, 0, s, st[1], st[0], st[2], 1);
        Render_SetAlphaOp(r, 0, s, st[4], st[3], st[5], 1);
    }
}

// 0x5488a0. Saves the stage cache on the stack +0x259b and applies `states` when given.
void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states) {
    uint32_t n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    uint8_t* p = (uint8_t*)Malloc(n);
    n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    const uint8_t* src = F<uint8_t*>(r, 0x2597);
    for (uint32_t k = 0; k < n; ++k) p[k] = src[k];
    PtrList_Prepend(F<PtrList*>(r, 0x259b), 0, p);
    if (states) ApplyStages(r, states);
}
RT3_REPLACE(0x5488a0, Render_PushStages);

// 0x548940. Pops n saved stage caches (at most the stack size) and applies the last.
void __fastcall Render_PopStages(uint8_t* r, int, int32_t n) {
    int32_t cnt = F<int32_t>(F<uint8_t*>(r, 0x259b), 0xc);
    if (!cnt) return;
    if (n > cnt) n = cnt;
    uint32_t* p = nullptr;
    for (int32_t i = n; i; --i) {
        if (p) Free(p);
        p = (uint32_t*)PtrList_PopFront(F<PtrList*>(r, 0x259b));
    }
    ApplyStages(r, p);
    Free(p);
}
RT3_REPLACE(0x548940, Render_PopStages);

// 0x54bab0 / 0x54bb10 / 0x54bb70. Bind a fixed vertex buffer without indices.
static inline void BindFixed(uint8_t* r, uint32_t slot, uint32_t fvf, uint32_t stride) {
    void* vb = F<void*>(r, slot);
    if (!vb) {
        F<void*>(r, 0x2637) = nullptr;
        return;
    }
    if (F<void*>(r, 0x2637) != vb) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t))Vt(d)[0x130 / 4])(d, fvf);
        d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t))Vt(d)[0x14c / 4])(d, 0, vb, stride);
        F<void*>(r, 0x2637) = vb;
    }
    F<void*>(r, 0x263b) = nullptr;
}
void __fastcall Render_BindVb2643(uint8_t* r) { BindFixed(r, 0x2643, 0x112, 0x20); }
RT3_REPLACE(0x54bab0, Render_BindVb2643);
void __fastcall Render_BindVb263f(uint8_t* r) { BindFixed(r, 0x263f, 0x112, 0x20); }
RT3_REPLACE(0x54bb10, Render_BindVb263f);
void __fastcall Render_BindVb2647(uint8_t* r) { BindFixed(r, 0x2647, 0x142, 0x18); }
RT3_REPLACE(0x54bb70, Render_BindVb2647);
void __fastcall Render_BindPair264f(uint8_t* r) { Render_SetStreams(r, 0, F<void*>(r, 0x264f), 0x18, F<void*>(r, 0x2653)); }
RT3_REPLACE(0x54bbd0, Render_BindPair264f);
void __fastcall Render_BindPair2647(uint8_t* r) { Render_SetStreams(r, 0, F<void*>(r, 0x2647), 0x18, F<void*>(r, 0x264b)); }
RT3_REPLACE(0x54bbf0, Render_BindPair2647);

// 0x54c880. Effective value +0x19 = +0x15 reduced by (+0x36a0 - 0x5c88e0) x 0x5cb4a8
// above the threshold, clamped to [+0x15 / 2, 255] (x87 with the exe's _ftol).
void __fastcall Fx_UpdateIntensity(uint8_t* o) {
    int32_t base = F<int32_t>(o, 0x15);
    F<int32_t>(o, 0x19) = base;
    if (!(F<float>(o, 0x36a0) > *(const float*)0x5c88e0)) return;
    float f = F<float>(o, 0x36a0);
    int32_t v;
    __asm {
        fild base
        fld f
        fsub dword ptr ds:[0x5c88e0]
        fmul dword ptr ds:[0x5cb4a8]
        fsubp st(1), st
        mov eax, 0x5a10d0
        call eax
        mov v, eax
    }
    F<int32_t>(o, 0x19) = v;
    int32_t lo = base >> 1;
    if (v < lo) F<int32_t>(o, 0x19) = lo;
    else if (v > 0xff) F<int32_t>(o, 0x19) = 0xff;
}
RT3_REPLACE(0x54c880, Fx_UpdateIntensity);
int32_t __fastcall Fx_Intensity(uint8_t* o) { return F<int32_t>(o, 0x19); }
RT3_REPLACE(0x54c8f0, Fx_Intensity);

// 0x54e5d0. Dispatches all pending window messages.
void __fastcall PumpMessages() {
    uint8_t msg[0x1c];
    auto peek = Import<int32_t(__stdcall*)(void*, void*, uint32_t, uint32_t, uint32_t)>(0x5c8288);
    if (!peek(msg, nullptr, 0, 0, 1)) return;
    auto translate = Import<int32_t(__stdcall*)(void*)>(0x5c8290);
    auto dispatch = Import<int32_t(__stdcall*)(void*)>(0x5c828c);
    do {
        translate(msg);
        dispatch(msg);
    } while (peek(msg, nullptr, 0, 0, 1));
}
RT3_REPLACE(0x54e5d0, PumpMessages);

} // namespace rt3
