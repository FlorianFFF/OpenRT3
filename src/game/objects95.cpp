// Input modifiers / screen projection / texture addressing in blocks 0x43..0x54 (batch 98). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Engine_Input(void* e);
uint32_t __fastcall Input_Flag400();
uint32_t __fastcall Input_Flag2000();
uint32_t __fastcall Input_Flag800();
int32_t __fastcall Obj_Flag7dOrTracks118(uint8_t* o);
uint8_t __fastcall Render_TakeFlag36f7(uint8_t* r, int, uint8_t clear);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }
// The flag getters ignore their `this` (the engine's input object); the original
// still fetches it before each call.
static inline uint32_t Flag400() { Engine_Input(Engine()); return Input_Flag400(); }
static inline uint32_t Flag2000() { Engine_Input(Engine()); return Input_Flag2000(); }
static inline uint32_t Flag800() { Engine_Input(Engine()); return Input_Flag800(); }
static inline uint32_t InputButtons() { return F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c); }
static inline int32_t SwapModifiers() { return F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x120); }

// 0x43af60. Whether the "add" modifier is active for this tool (+0xa2 forces it).
int32_t __fastcall Tool_AddModifier(uint8_t* t) {
    if (F<int32_t>(t, 0xa2)) return 1;
    uint32_t shift = Flag400();
    if (uint8_t* g = hook::Global<uint8_t*>(0x6d1a8c))
        if (Obj_Flag7dOrTracks118(g)) shift = 0;
    int32_t alt;
    if (SwapModifiers() && Flag2000()) alt = 1;
    else if (!SwapModifiers() && Flag800()) alt = 1;
    else alt = 0;
    uint32_t ctrl = Flag800();
    if (ctrl && (InputButtons() & 3)) return 1;
    if (alt) return 1;
    if (shift && ctrl) return 1;
    return 0;
}
RT3_REPLACE(0x43af60, Tool_AddModifier);

// 0x43b030. Whether the "remove" modifier is active.
int32_t __fastcall Tool_RemoveModifier() {
    uint32_t shift = Flag400();
    Flag2000();
    uint32_t ctrl = Flag800();
    if (uint8_t* g = hook::Global<uint8_t*>(0x6d1a8c))
        if (Obj_Flag7dOrTracks118(g)) shift = 0;
    if (ctrl && (InputButtons() & 3)) return 0;
    if (SwapModifiers()) return ctrl && !shift ? 1 : 0;
    return Flag2000() ? 1 : 0;
}
RT3_REPLACE(0x43b030, Tool_RemoveModifier);

int32_t __fastcall Scene_NeedsRedraw(uint8_t* s) {
    if (Render_TakeFlag36f7(F<uint8_t*>(s, 0x2d), 0, 0)) return 1;
    return s[0x3749] || s[0x3748] ? 1 : 0;
}
RT3_REPLACE(0x526620, Scene_NeedsRedraw);

// 0x544660. Viewport scale/offset (+0x2523 sx, +0x252b sy, +0x2527 ox, +0x252f oy).
void __fastcall Render_GetViewport(uint8_t* r, int, uint32_t* sx, uint32_t* sy, uint32_t* ox, uint32_t* oy) {
    if (sx) *sx = F<uint32_t>(r, 0x2523);
    if (sy) *sy = F<uint32_t>(r, 0x252b);
    if (ox) *ox = F<uint32_t>(r, 0x2527);
    if (oy) *oy = F<uint32_t>(r, 0x252f);
}
RT3_REPLACE(0x544660, Render_GetViewport);

// Row-major 4x4 at r + base: m(row, col).
static inline double M(uint8_t* r, uint32_t base, int row, int col) { return F<float>(r, base + (row * 4 + col) * 4); }

// 0x5446b0. Projects (x, y, z) with the matrix +0x33ef (x/w, y/w).
int32_t __fastcall Render_ProjectSimple(uint8_t* r, int, float x, float y, float z, float* ox, float* oy) {
    const uint32_t b = 0x33ef;
    double py = (double)x * M(r, b, 0, 1) + (double)y * M(r, b, 1, 1) + (double)z * M(r, b, 2, 1) + M(r, b, 3, 1);
    double inv = *(const double*)0x5c8770 /
                 ((double)x * M(r, b, 0, 3) + (double)y * M(r, b, 1, 3) + (double)z * M(r, b, 2, 3) + M(r, b, 3, 3));
    double px = (double)x * M(r, b, 0, 0) + (double)y * M(r, b, 1, 0) + (double)z * M(r, b, 2, 0) + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)(py * inv);
    return 0;
}
RT3_REPLACE(0x5446b0, Render_ProjectSimple);

// 0x544750. Projects with the combined matrix +0x33af; 1 (culled) when w < 1 or
// depth > 1, else 0 with the screen x/y (and depth when asked).
int32_t __fastcall Render_Project(uint8_t* r, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    const uint32_t b = 0x33af;
    float py = (float)((double)x * M(r, b, 0, 1) + (double)y * M(r, b, 1, 1) + (double)z * M(r, b, 2, 1) + M(r, b, 3, 1));
    float pz = (float)((double)x * M(r, b, 0, 2) + (double)y * M(r, b, 1, 2) + (double)z * M(r, b, 2, 2) + M(r, b, 3, 2));
    double w = (double)x * M(r, b, 0, 3) + (double)y * M(r, b, 1, 3) + (double)z * M(r, b, 2, 3) + M(r, b, 3, 3);
    if (w < *(const float*)0x5c85b4) return 1;
    double inv = *(const double*)0x5c8770 / w;
    double d = (double)pz * inv;
    pz = (float)d;
    if (d > *(const double*)0x5c8770) return 1;
    double px = (double)x * M(r, b, 0, 0) + (double)y * M(r, b, 1, 0) + (double)z * M(r, b, 2, 0) + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)(inv * py);
    if (oz) *oz = pz;
    return 0;
}
RT3_REPLACE(0x544750, Render_Project);

// 0x544860. Transforms v in place by the world matrix +0x34ef (homogeneous).
void __fastcall Render_TransformWorld(uint8_t* r, int, float* v) {
    const uint32_t b = 0x34ef;
    double x = v[0], y = v[1], z = v[2];
    float py = (float)(z * M(r, b, 2, 1) + y * M(r, b, 1, 1) + x * M(r, b, 0, 1) + M(r, b, 3, 1));
    float pz = (float)(z * M(r, b, 2, 2) + y * M(r, b, 1, 2) + x * M(r, b, 0, 2) + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (z * M(r, b, 2, 3) + y * M(r, b, 1, 3) + x * M(r, b, 0, 3) + M(r, b, 3, 3)));
    double px = z * M(r, b, 2, 0) + y * M(r, b, 1, 0) + x * M(r, b, 0, 0) + M(r, b, 3, 0);
    v[0] = (float)(px * inv);
    v[1] = (float)((double)inv * py);
    v[2] = (float)((double)inv * pz);
}
RT3_REPLACE(0x544860, Render_TransformWorld);

// 0x544a80. Unprojects screen (sx, sy) at depth 0x5dd990 with the inverse matrix +0x342f.
void __fastcall Render_Unproject(uint8_t* r, int, float sx, float sy, float* ox, float* oy, float* oz) {
    const uint32_t b = 0x342f;
    const double k = *(const float*)0x5dd990;
    double u = ((double)sx - F<float>(r, 0x2527)) * F<float>(r, 0x2523);
    double v = ((double)sy - F<float>(r, 0x252f)) * F<float>(r, 0x252b);
    float py = (float)(u * M(r, b, 0, 1) + v * M(r, b, 1, 1) + M(r, b, 2, 1) * k + M(r, b, 3, 1));
    float pz = (float)(u * M(r, b, 0, 2) + v * M(r, b, 1, 2) + M(r, b, 2, 2) * k + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (u * M(r, b, 0, 3) + v * M(r, b, 1, 3) + M(r, b, 2, 3) * k + M(r, b, 3, 3)));
    double px = u * M(r, b, 0, 0) + v * M(r, b, 1, 0) + M(r, b, 2, 0) * k + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)((double)inv * py);
    *oz = (float)((double)inv * pz);
}
RT3_REPLACE(0x544a80, Render_Unproject);

// 0x544b80. Ray direction from the camera point `eye` through screen (sx, sy).
void __fastcall Render_UnprojectDir(uint8_t* r, int, float sx, float sy, const float* eye, float* out) {
    const uint32_t b = 0x342f;
    const double k = *(const float*)0x5dd990;
    double u = ((double)sx - F<float>(r, 0x2527)) * F<float>(r, 0x2523);
    double v = ((double)sy - F<float>(r, 0x252f)) * F<float>(r, 0x252b);
    float py = (float)(v * M(r, b, 1, 1) + u * M(r, b, 0, 1) + M(r, b, 2, 1) * k + M(r, b, 3, 1));
    float pz = (float)(v * M(r, b, 1, 2) + u * M(r, b, 0, 2) + M(r, b, 2, 2) * k + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (v * M(r, b, 1, 3) + u * M(r, b, 0, 3) + M(r, b, 2, 3) * k + M(r, b, 3, 3)));
    double px = v * M(r, b, 1, 0) + u * M(r, b, 0, 0) + M(r, b, 2, 0) * k + M(r, b, 3, 0);
    out[0] = (float)(px * inv - eye[0]);
    out[1] = (float)((double)inv * py - eye[1]);
    out[2] = (float)((double)inv * pz - eye[2]);
}
RT3_REPLACE(0x544b80, Render_UnprojectDir);

// Texture addressing (SetTextureStageState vtbl +0xfc, ADDRESSU 0xd / ADDRESSV 0xe).
static inline void SetTss(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = F<void*>(r, 0x25df);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}
void __fastcall Render_AddressBorder(uint8_t* r) {
    SetTss(r, 0, 0xd, 4);
    SetTss(r, 0, 0xe, 4);
}
RT3_REPLACE(0x544c80, Render_AddressBorder);
void __fastcall Render_AddressClamp(uint8_t* r, int, uint8_t stage) {
    SetTss(r, stage, 0xd, 3);
    SetTss(r, stage, 0xe, 3);
}
RT3_REPLACE(0x544cb0, Render_AddressClamp);
void __fastcall Render_AddressMirror(uint8_t* r) {
    SetTss(r, 0, 0xd, 2);
    SetTss(r, 0, 0xe, 2);
}
RT3_REPLACE(0x544cf0, Render_AddressMirror);
void __fastcall Render_AddressWrap(uint8_t* r) {
    SetTss(r, 0, 0xd, 1);
    SetTss(r, 0, 0xe, 1);
    SetTss(r, 1, 0xd, 1);
    SetTss(r, 1, 0xe, 1);
}
RT3_REPLACE(0x544d20, Render_AddressWrap);

// 0x544d80 / 0x544da0. Texture factor render state (0x3c): raw / background colour with alpha.
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = F<void*>(r, 0x25df);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, state, v);
}
int32_t __fastcall Render_SetTextureFactor(uint8_t* r, int, uint32_t v) { return SetRs(r, 0x3c, v); }
RT3_REPLACE(0x544d80, Render_SetTextureFactor);
int32_t __fastcall Render_SetFactorAlpha(uint8_t* r, int, uint8_t a) {
    return SetRs(r, 0x3c, (F<uint32_t>(r, 0x25a3) & 0xffffff) | ((uint32_t)a << 24));
}
RT3_REPLACE(0x544da0, Render_SetFactorAlpha);

} // namespace rt3
