// Frame begin/end, device creation and render-state toggles in block 0x54 (batch 102). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

struct HWND__;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

::HWND__* __fastcall Engine_Window(void* e);

static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline void** Vt(void* p) { return *(void***)p; }
static inline int32_t Call0(void* d, uint32_t off) { return ((int32_t(__stdcall*)(void*))Vt(d)[off / 4])(d); }
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}
static inline int32_t Clear(void* d, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t, uint32_t, uint32_t, uint32_t))Vt(d)[0x90 / 4])(
        d, 0, nullptr, flags, 0, 0x3f800000, 0);
}
static inline int32_t Present(void* d) {
    return ((int32_t(__stdcall*)(void*, void*, void*, void*, void*))Vt(d)[0x3c / 4])(d, nullptr, nullptr, nullptr, nullptr);
}
static inline uint8_t* Cfg(uint8_t* r) { return F<uint8_t*>(r, 0x83c); }

// 0x545d60. Starts a frame: resets the counters (+0x370d/+0x3711), clears z when asked.
int32_t __fastcall Render_BeginFrame(uint8_t* r, int, int32_t clear) {
    F<int32_t>(r, 0x370d) = 0;
    F<int32_t>(r, 0x3711) = 0;
    if (clear) Clear(Dev(r), 2);
    return Call0(Dev(r), 0x88);
}
RT3_REPLACE(0x545d60, Render_BeginFrame);

// 0x545db0. Ends the frame, presenting unless asked not to.
void __fastcall Render_EndFrame(uint8_t* r, int, int32_t noPresent) {
    Call0(Dev(r), 0x8c);
    if (!noPresent) Present(Dev(r));
}
RT3_REPLACE(0x545db0, Render_EndFrame);

void __fastcall Nop545df0(void*, int, int32_t, int32_t) {}
RT3_REPLACE(0x545df0, Nop545df0);

// 0x545e00. Device state: 0 ok, 1 lost, 2 needs reset, 3 other error.
uint8_t __fastcall Render_DeviceState(uint8_t* r) {
    uint32_t hr = (uint32_t)Call0(Dev(r), 0xc);
    if (hr == 0x88760868) return 1;
    if (hr == 0x88760869) return 2;
    return hr == 0 ? 0 : 3;
}
RT3_REPLACE(0x545e00, Render_DeviceState);

// 0x546390. Creates the HAL device (+0x25df) on the engine window.
uint8_t __fastcall Render_CreateDevice(uint8_t* r, int, uint32_t behaviour, void* params) {
    void* d3d = F<void*>(r, 0x25db);
    ::HWND__* wnd = Engine_Window(hook::Global<void*>(0x6d4024));
    int32_t hr = ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, ::HWND__*, uint32_t, void*, void*))Vt(d3d)[0x3c / 4])(
        F<void*>(r, 0x25db), 0, 1, wnd, behaviour, params, r + 0x25df);
    return hr >= 0 ? 1 : 0;
}
RT3_REPLACE(0x546390, Render_CreateDevice);

// 0x5464d0. Clears and presents four blank frames.
void __fastcall Render_BlankFrames(uint8_t* r) {
    for (int k = 0; k < 4; ++k) {
        Call0(Dev(r), 0x88);
        Clear(Dev(r), 3);
        Call0(Dev(r), 0x8c);
        Present(Dev(r));
    }
}
RT3_REPLACE(0x5464d0, Render_BlankFrames);

int32_t __fastcall Render_ZFuncAlways(uint8_t* r) { return SetRs(r, 0x17, 7); }
RT3_REPLACE(0x546540, Render_ZFuncAlways);
int32_t __fastcall Render_ZFuncLessEqual(uint8_t* r) { return SetRs(r, 0x17, 4); }
RT3_REPLACE(0x546560, Render_ZFuncLessEqual);
int32_t __fastcall Render_ColorWriteOff(uint8_t* r) { return SetRs(r, 0xa8, 0); }
RT3_REPLACE(0x546580, Render_ColorWriteOff);
int32_t __fastcall Render_ColorWriteOn(uint8_t* r) { return SetRs(r, 0xa8, 0xf); }
RT3_REPLACE(0x5465a0, Render_ColorWriteOn);
int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v) { return SetRs(r, 7, v); }
RT3_REPLACE(0x546760, Render_SetZEnable);
int32_t __fastcall Render_SetShadeMode(uint8_t* r, int, uint32_t v) { return SetRs(r, 9, v + 1); }
RT3_REPLACE(0x546780, Render_SetShadeMode);

// 0x5467a0 / 0x5467e0. Fog on (when configured, +0x2e) / off, cached in +0x2549.
void __fastcall Render_FogOn(uint8_t* r) {
    if (r[0x2549] || !Cfg(r)[0x2e]) return;
    r[0x2549] = 1;
    SetRs(r, 0x1c, 1);
}
RT3_REPLACE(0x5467a0, Render_FogOn);
void __fastcall Render_FogOff(uint8_t* r) {
    if (!r[0x2549]) return;
    r[0x2549] = 0;
    SetRs(r, 0x1c, 0);
}
RT3_REPLACE(0x5467e0, Render_FogOff);

// 0x546810 / 0x546850 / 0x546890. Cull mode cache +0x254a: CCW (2, when culling
// is configured, +0x1c), CW (1), none (0).
void __fastcall Render_CullCcw(uint8_t* r) {
    if (r[0x254a] == 2 || !Cfg(r)[0x1c]) return;
    SetRs(r, 0x16, 3);
    r[0x254a] = 2;
}
RT3_REPLACE(0x546810, Render_CullCcw);
void __fastcall Render_CullCw(uint8_t* r) {
    if (r[0x254a] == 1 || !Cfg(r)[0x1c]) return;
    SetRs(r, 0x16, 2);
    r[0x254a] = 1;
}
RT3_REPLACE(0x546850, Render_CullCw);
void __fastcall Render_CullNone(uint8_t* r) {
    if (!r[0x254a]) return;
    SetRs(r, 0x16, 1);
    r[0x254a] = 0;
}
RT3_REPLACE(0x546890, Render_CullNone);

} // namespace rt3
