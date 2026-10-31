// Camera pitch curve / distance fog alpha / view bounds in block 0x43 (batch 127). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }
static inline float FromBits(uint32_t u) { return *(float*)&u; }

// 0x43a750. The camera pitch (+0x14) for its zoom level per camera mode (options
// +0x10c): a free camera is clamped to [-0.4, max]; mode 0 follows a power
// curve, mode 1 a two-segment line; mode 2 is fixed. vtbl +0xc updates the camera.
static float Camera_PitchForZoomMode(uint8_t* c, float zoom, int32_t mode);
float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom) {
    if (zoom < D(0x5ca0d0)) zoom = 37.5f;
    float pitch = F<float>(c, 0x14);
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c);
    if (!hook::Global<uint8_t>(0x5ee4d0 + mode) && hook::Global<uint8_t*>(0x6d4024)[0x11422e]) {
        if (pitch < D(0x5ca0c8)) pitch = FromBits(0xbecccccd);
        if ((double)pitch > D(0x5ca028)) pitch = FromBits(0x3fbc4303);
        return pitch;
    }
    return Camera_PitchForZoomMode(c, zoom, mode);
}
RT3_REPLACE(0x43a750, Camera_PitchForZoom);

// Body of 0x43a750 for the fixed camera modes. Kept separate: inline, MSVC 2019
// /O2 drops the whole mode 0 branch after the early return above.
static __declspec(noinline) float Camera_PitchForZoomMode(uint8_t* c, float zoom, int32_t mode) {
    float pitch = F<float>(c, 0x14);
    auto powf_ = (double(__cdecl*)(double, double))0x5a1b60;
    static const uint64_t kExpo = 0x3fd6666666666666ull, kDenom = 0x40d090a000000000ull; // 0.35, 16962.5
    if (mode == 0) {
        pitch = (float)((double)zoom - D(0x5ca0d0));
        pitch = (float)powf_(pitch, *(const double*)&kExpo);
        float top = (float)powf_(*(const double*)&kDenom, *(const double*)&kExpo);
        pitch = (float)((double)pitch / top * D(0x5ca0c0) + D(0x5c86b8));
        if ((double)pitch > D(0x5ca028)) pitch = FromBits(0x3fbc4303);
        if ((double)pitch < D(0x5c86b8)) pitch = FromBits(0x3dcccccd);
    } else if (mode == 2) {
        pitch = FromBits(0x3f47ae14);
    } else if (mode == 1) {
        if (zoom < Fl(0x5ca0b8))
            pitch = (float)(((double)zoom - D(0x5ca0d0)) / D(0x5ca0b0) * D(0x5ca0a8) + D(0x5c86b8));
        else if (zoom < Fl(0x5ca0a4))
            pitch = FromBits(0x3f47ae14);
        else if (zoom < Fl(0x5ca0a0))
            pitch = (float)(((double)zoom - Fl(0x5ca0a4)) / Fl(0x5c86a0) * D(0x5ca098) + D(0x5c8920));
        else
            pitch = FromBits(0x3fbc4303);
    }
    ((void(__thiscall*)(void*))(*(void***)c)[0xc / 4])(c);
    ((double(__cdecl*)(double))0x5a13e4)(pitch);
    return pitch;
}


// 0x43ad50. Fog alpha (0..255) of point (x, y) for the camera: a height-based base
// (pitch +0x14 bands) plus a distance term from the target cell (+0x1c, +0x24).
int32_t __fastcall Camera_FogAlpha(uint8_t* c, int, int32_t x, int32_t y) {
    float v = F<float>(c, 0x14);
    int32_t base = 0;
    if (!((double)v <= D(0x5ca120))) return 0xff;
    if ((double)v > D(0x5ca118)) {
        __asm {
            fld v
            fsub qword ptr ds:[0x5ca118]
            fmul qword ptr ds:[0x5ca110]
            fdiv qword ptr ds:[0x5c8668]
            mov eax, 0x5a10d0
            call eax
            mov base, eax
        }
    } else {
        if ((double)v < D(0x5ca108)) return 0;
        if ((double)v < D(0x5ca100)) {
            __asm {
                fld v
                fsubr qword ptr ds:[0x5ca100]
                fmul qword ptr ds:[0x5ca0f8]
                fdiv qword ptr ds:[0x5c8668]
                mov eax, 0x5a10d0
                call eax
                mov base, eax
            }
        }
    }
    float r = (float)(((double)v - D(0x5c8630)) / D(0x5c8880));
    if ((double)r < D(0x5c85b8)) r = FromBits(0x3c23d70a);
    float tz = F<float>(c, 0x24), tx = F<float>(c, 0x1c);
    int32_t cellZ, cellX;
    __asm {
        fld tz
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellZ, eax
        fld tx
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellX, eax
    }
    float d = (float)(uint16_t)DistanceInt(x, y, cellX, cellZ);
    float q = (float)((double)d / ((double)r + D(0x5c88e8)));
    q = (float)((double)q - Fl(0x5ca0f0));
    int32_t a;
    if ((double)q > Fl(0x5c873c)) {
        a = 0;
    } else if ((double)q > Fl(0x5c8598)) {
        __asm {
            fld dword ptr ds:[0x5c873c]
            fsub q
            fmul dword ptr ds:[0x5ca0ec]
            fdiv dword ptr ds:[0x5c873c]
            mov eax, 0x5a10d0
            call eax
            mov a, eax
        }
    } else {
        a = 0xff;
    }
    if (a <= 0) a = 0;
    if (a >= 0xff) a = 0xff;
    a += base;
    if (a <= 0) a = 0;
    if (a >= 0xff) a = 0xff;
    return a;
}
RT3_REPLACE(0x43ad50, Camera_FogAlpha);

// 0x43b0e0. Keeps the camera eye (+4/+0xc) on the map and, when asked (and not
// detached, +0x60), the target (+0x1c/+0x24) within a margin from the zoom (+0x20).
void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget) {
    float w = (float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) * 0x5c);
    float h = (float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) * 0x5c);
    const float zero = Fl(0x5c8598);
    if (F<float>(c, 4) < zero) F<int32_t>(c, 4) = 0;
    if (F<float>(c, 0xc) < zero) F<int32_t>(c, 0xc) = 0;
    if (F<float>(c, 4) > w) F<float>(c, 4) = w;
    if (F<float>(c, 0xc) > h) F<float>(c, 0xc) = h;
    if (!keepTarget || F<int32_t>(c, 0x60)) return;
    float m = (float)(((double)F<float>(c, 0x20) / D(0x5c85e8) + D(0x5c8970)) * D(0x5c8c48));
    float lo = -m;
    auto shift = [&](uint32_t eye, uint32_t tgt, float d) {
        F<float>(c, eye) = (float)((double)d + F<float>(c, eye));
        F<float>(c, tgt) = (float)((double)d + F<float>(c, tgt));
    };
    if (F<float>(c, 0x1c) < lo) {
        F<int32_t>(c, 0x312) = 0;
        shift(4, 0x1c, (float)((double)lo - F<float>(c, 0x1c)));
    }
    if (F<float>(c, 0x24) < lo) {
        F<int32_t>(c, 0x312) = 0;
        shift(0xc, 0x24, (float)((double)lo - F<float>(c, 0x24)));
    }
    float hiX = (float)((double)(float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) * 0x5c - 0x5c) + m);
    if (F<float>(c, 0x1c) > hiX) {
        F<int32_t>(c, 0x312) = 0;
        shift(4, 0x1c, (float)((double)hiX - F<float>(c, 0x1c)));
    }
    float hiZ = (float)((double)(float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) * 0x5c - 0x5c) + m);
    if (F<float>(c, 0x24) > hiZ) {
        F<int32_t>(c, 0x312) = 0;
        shift(0xc, 0x24, (float)((double)hiZ - F<float>(c, 0x24)));
    }
}
RT3_REPLACE(0x43b0e0, Camera_ClampToMap);

double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void* __fastcall Engine_Renderer(void* e);
double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
float __fastcall Render_Get14e0(uint8_t* r);
int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i);
int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i);
double __stdcall Render_Scaled1540(uint8_t* r);
void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v);
void __fastcall Render_Set1544(uint8_t* r, int, uint32_t v);

static inline int32_t CamFtol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x43b350. Per-frame view settings from the camera: its altitude over the
// terrain (+0x38a); the scene's detail level by the fog factor (scene
// +0x22b5b / +0x22b5f, full detail when the option is off or the renderer's
// +0x14e0 is high); the fog start by altitude, stretched by the view-distance
// option (scene +0x25, negated); the draw distance (option 7 table 0x6218a0,
// grown with altitude or by pitch) and the far plane by pitch (renderer +0x1534
// / +0x1544); the scene's +0x22b8b by pitch and option 8.
void __fastcall Camera_UpdateViewSettings(uint8_t* c) {
    uint8_t* dev = hook::Global<uint8_t*>(0x62c120);
    uint8_t* opts = hook::Global<uint8_t*>(0x6cec74);
    float alt = (float)((double)F<float>(c, 0x20) -
                        ((double(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))&Terrain_HeightAt)(F<uint8_t*>(c, 0x6e), 0, F<uint32_t>(c, 0x1c), F<uint32_t>(c, 0x24)));
    F<float>(c, 0x38a) = alt;
    uint8_t* s = (uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024));
    float fog = (float)Scene_FogFactor(s, 0, dev);
    float level;
    if (fog < D(0x5ca188)) level = FromBits(0x3e19999a);
    else if (fog < D(0x5c9f08)) level = FromBits(0x3e99999a);
    else if (fog < D(0x5ca180)) level = FromBits(0x3ee66666);
    else if (fog < D(0x5c9cb8)) level = FromBits(0x3f0ccccd);
    else if (fog < D(0x5ca178)) level = FromBits(0x3f266666);
    else level = fog < D(0x5ca170) ? 0.75f : 1.0f;
    if (!hook::Global<uint8_t*>(0x6d4024)[0x1146eb]) level = 1.0f;
    if ((double)Render_Get14e0(dev) >= D(0x5ca168)) level = 1.0f;
    else if ((double)Render_Get14e0(dev) >= D(0x5c9ed8)) level = (float)(((double)level + D(0x5c8770)) / D(0x5c85d0));
    F<float>(s, 0x22b5b) = level;
    F<float>(s, 0x22b5f) = (float)(((double)level + D(0x5c8770)) / D(0x5c85d0));
    float d;
    if (alt < Fl(0x5c9ac0)) d = 0.0f;
    else if (alt < Fl(0x5ca164)) d = (float)(((double)alt - Fl(0x5c9ac0)) / D(0x5c85d8));
    else if (alt < Fl(0x5ca160)) d = (float)(((double)alt - Fl(0x5ca164)) / D(0x5c8a20) + D(0x5c85f0));
    else if (alt < Fl(0x5c85b0)) d = (float)(((double)alt - Fl(0x5ca160)) / D(0x5ca158) * D(0x5c9be0) + D(0x5c8a20));
    else d = 70.0f;
    auto grow = [&](double div, double cap, double over) {
        if (!(alt > Fl(0x5ca148))) return;
        double t = ((double)alt - Fl(0x5ca148)) / div;
        alt = (float)((double)alt + (t < cap ? t : over));
    };
    if (Obj_ArrayAc(opts, 0, 3) <= 3) {
        d = (float)((double)d * D(0x5ca150));
        grow(D(0x5c85f0), D(0x5c88a8), 60.0);
    } else if (Obj_ArrayAc(opts, 0, 3) <= 6) {
        d = (float)((double)d * D(0x5ca140));
        grow(D(0x5c8798), D(0x5c96c0), 40.0);
    } else if (Obj_ArrayAc(opts, 0, 3) <= 8) {
        d = (float)((double)d * D(0x5c8820));
    } else if (Obj_ArrayAc(opts, 0, 3) <= 0xb) {
        d = (float)((double)d * D(0x5c8698));
    }
    d = (float)-(double)d;
    F<float>((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x25) = d;
    float dist = hook::Global<float>(0x6218a0 + Obj_ArrayAc2(opts, 0, 7) * 4);
    if (fog <= D(0x5c8608)) {
        float t = (float)((double)alt - Render_Scaled1540(dev));
        if (Fl(0x5c8598) > t) t = 0.0f;
        dist = (float)((double)t * D(0x5ca138) / D(0x5ca130) + dist);
    } else if (dist >= D(0x5c8770)) {
        float t = (float)(((double)F<float>(c, 0x14) - D(0x5c86b8) - D(0x5c8c20)) / D(0x5c85a0));
        if (t > Fl(0x5c8598) && t < D(0x5c8770)) {
            t = (float)((double)t * D(0x5c85a0) + D(0x5c8988));
            dist = (float)(((double)dist - D(0x5c8770)) * t + D(0x5c8770));
        }
    }
    Render_Set1534(dev, 0, (uint32_t)CamFtol((double)dist * Fl(0x5ca128)));
    float p = (float)(((double)F<float>(c, 0x14) - D(0x5c86b8)) / D(0x5c8618));
    p = (float)(p > D(0x5c8770) ? 1.0 : (double)p);
    p = (float)(p < D(0x5c8608) ? 0.0 : (double)p);
    p = (float)((double)p * D(0x5c8970) + D(0x5c9b78));
    Render_Set1544(dev, 0, (uint32_t)CamFtol(p));
    float pitch = (float)((double)F<float>(c, 0x14) - D(0x5c86b8));
    int32_t e = 0x14;
    if (pitch < D(0x5c8c20)) e = 0x28;
    else if (pitch < D(0x5c88c8)) e = 0x1e;
    int32_t q = Obj_ArrayAc2(opts, 0, 8);
    if (q < 10) e += 8;
    else if (q < 14) e += 4;
    F<int32_t>((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x22b8b) = e;
}
RT3_REPLACE(0x43b350, Camera_UpdateViewSettings);

} // namespace rt3
