// Texture teardown / fade / faded quads / slider geometry in blocks 0x53..0x55 (batch 106). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Texture_Destroy(uint8_t* t);
void __fastcall Keyboard_SetScrollLock(int32_t on);
float __fastcall Fade_Step_Impl();
void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);
void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Engine_QueueType6(void* e);
double __fastcall Font_ScaleY(uint8_t* f, int, float v);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline void* Engine() { return hook::Global<void*>(0x6d4024); }

// 0x532760. Destroys the current overview texture (+8 + +0x1558 * 4).
void __fastcall Render_DestroyCurrentTexture(uint8_t* r) {
    int32_t i = F<int32_t>(r, 0x1558);
    if (!i) return;
    uint8_t* t = F<uint8_t*>(r, 8 + i * 4);
    if (!t) return;
    Texture_Destroy(t);
    ExeDelete(t);
    F<uint8_t*>(r, 8 + F<int32_t>(r, 0x1558) * 4) = nullptr;
}
RT3_REPLACE(0x532760, Render_DestroyCurrentTexture);

// 0x534f80. Destroys the terrain colour map texture (+0x478).
void __fastcall Render_DestroyColourMap(uint8_t* r) {
    uint8_t* t = F<uint8_t*>(r, 0x478);
    if (!t) return;
    Texture_Destroy(t);
    ExeDelete(t);
    F<uint8_t*>(r, 0x478) = nullptr;
    r[0x159b] = 0;
}
RT3_REPLACE(0x534f80, Render_DestroyColourMap);

void __fastcall Keyboard_RestoreScrollLock() { Keyboard_SetScrollLock(hook::Global<int32_t>(0xd93844)); }
RT3_REPLACE(0x54e700, Keyboard_RestoreScrollLock);

// Fade value 0xd93850 and its saved copy 0x625464 (-1 = none).
void __fastcall Fade_StartVerySlow() { hook::Global<uint32_t>(0xd93850) = 0x3951b717; }
RT3_REPLACE(0x54f6c0, Fade_StartVerySlow);
void __fastcall Fade_Save() { hook::Global<uint32_t>(0x625464) = hook::Global<uint32_t>(0xd93850); }
RT3_REPLACE(0x54f6d0, Fade_Save);
void __fastcall Fade_Restore() {
    hook::Global<uint32_t>(0xd93850) = hook::Global<uint32_t>(0x625464);
    hook::Global<uint32_t>(0x625464) = 0xbf800000;
}
RT3_REPLACE(0x54f6e0, Fade_Restore);
void __stdcall Fade_Set(uint32_t v) { hook::Global<uint32_t>(0xd93850) = v; }
RT3_REPLACE(0x54f700, Fade_Set);

// 0x54fdd0. Queues a quad with the current fade as its alpha (p7); with `clip`
// (s5) it is wrapped in a clip rectangle (s8..s11).
void __fastcall Fx_QueueFadedQuad(uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3,
                                  uint32_t s4, uint32_t clip, uint32_t s6, uint32_t, uint32_t s8, uint32_t s9,
                                  uint32_t s10, uint32_t s11) {
    const uint32_t one = 0x3f800000;
    if (clip) Engine_QueueType5(Engine(), 0, s8, s9, s10, s11);
    float fade = Fade_Step_Impl();
    Engine_QueueQuad(Engine(), 0, s0, s1, s2, s3, s4, clip, *(uint32_t*)&fade, a, b, s6, 0, 0, 0, one, one, one, 0,
                     one, one, one);
    if (clip) Engine_QueueType6(Engine());
}
RT3_REPLACE(0x54fdd0, Fx_QueueFadedQuad);

// 0x550090 / 0x5500b0. Scrollbar extents: (+0x28 + +0x1c) or +0x1c, scaled by +0x14;
// +8 x +0x14 when not scrolling (+0x10 == 0).
double __fastcall Scroll_Total(uint8_t* s) {
    if (F<int32_t>(s, 0x10)) return ((double)F<float>(s, 0x28) + F<float>(s, 0x1c)) * F<float>(s, 0x14);
    return (double)F<float>(s, 0x14) * F<float>(s, 8);
}
RT3_REPLACE(0x550090, Scroll_Total);
double __fastcall Scroll_Visible(uint8_t* s) {
    if (F<int32_t>(s, 0x10)) return (double)F<float>(s, 0x1c) * F<float>(s, 0x14);
    return (double)F<float>(s, 0x14) * F<float>(s, 8);
}
RT3_REPLACE(0x5500b0, Scroll_Visible);

// 0x5520a0. Slider constructor: track (x0, y0)-(x1, y1), extra values, font (+0);
// +0x24 = 0 when the track runs diagonally down-right or up-left, else 1.
uint8_t* __fastcall Slider_Construct(uint8_t* s, int, float x0, float y0, float x1, float y1, uint32_t a,
                                     uint32_t b, uint32_t c, uint32_t d, uint32_t font) {
    F<float>(s, 4) = x0;
    F<float>(s, 0xc) = x1;
    F<uint32_t>(s, 0x14) = a;
    F<uint32_t>(s, 0x1c) = c;
    F<float>(s, 8) = y0;
    F<float>(s, 0x10) = y1;
    F<uint32_t>(s, 0) = font;
    F<uint32_t>(s, 0x18) = b;
    F<uint32_t>(s, 0x20) = d;
    bool straight = (x0 < x1 && y0 < y1) || (x0 > x1 && y0 > y1);
    s[0x24] = straight ? 0 : 1;
    return s;
}
RT3_REPLACE(0x5520a0, Slider_Construct);

void __fastcall Nop552130() {}
RT3_REPLACE(0x552130, Nop552130);
void __fastcall Nop552ff0() {}
RT3_REPLACE(0x552ff0, Nop552ff0);

double __fastcall Slider_HeightScaled(uint8_t* s) {
    return Font_ScaleY(F<uint8_t*>(s, 0), 0, (float)((double)F<float>(s, 8) - F<float>(s, 0x18)));
}
RT3_REPLACE(0x552140, Slider_HeightScaled);

// 0x552160 / 0x5521a0. Absolute slider step along / across the track.
double __fastcall Slider_StepA(uint8_t* s) {
    uint8_t* f = F<uint8_t*>(s, 0);
    double v = s[0x24] ? ((double)F<float>(s, 8) - F<float>(s, 0x10)) * F<float>(f, 0x32) * *(const double*)0x5c8750
                       : ((double)F<float>(s, 4) - F<float>(s, 0xc)) * F<float>(f, 0x2e);
    v *= F<float>(s, 0x1c);
    if (v < *(const float*)0x5c8598) v = -v;
    return v;
}
RT3_REPLACE(0x552160, Slider_StepA);
double __fastcall Slider_StepB(uint8_t* s) {
    uint8_t* f = F<uint8_t*>(s, 0);
    double v = s[0x24] ? ((double)F<float>(s, 4) - F<float>(s, 0xc)) * F<float>(f, 0x2e) * *(const double*)0x5ddc78
                       : ((double)F<float>(s, 8) - F<float>(s, 0x10)) * F<float>(f, 0x32);
    v *= F<float>(s, 0x20);
    if (v < *(const float*)0x5c8598) v = -v;
    return v;
}
RT3_REPLACE(0x5521a0, Slider_StepB);

} // namespace rt3
