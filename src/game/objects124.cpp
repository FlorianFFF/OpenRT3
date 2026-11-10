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
RT3_NOINLINE float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom) {
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
RT3_NOINLINE void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget) {
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
RT3_NOINLINE void __fastcall Camera_UpdateViewSettings(uint8_t* c) {
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

void __fastcall View_AccumulateMotion(uint8_t* o);
double __fastcall SceneObj_GroundHeight(uint8_t* o);
void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v);
void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);

static inline double CamFabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
static inline double CamBitsD(uint64_t b) { return *(double*)&b; }

// 0x43bde0. Per-frame camera update (unless frozen, +0x64): measures how far the
// camera still has to travel to its target (position +0x38.., yaw +0x44, pitch
// +0x48, zoom +0x4c against the current +0x4.., +0x10, +0x14, +0x18) in units
// of the per-frame step, which depends on the altitude over the ground and on
// how far the yaw has to turn; moves it a step (vtbl +0) while it is not
// there; refreshes the view settings when the altitude changed enough; sets
// the far fog distances from the altitude and the detail options; and flags
// +0x65 when the camera and its target are both over water.
RT3_NOINLINE void __fastcall Camera_Update(uint8_t* c) {
    View_AccumulateMotion(c);
    if (c[0x64]) return;
    float ground = (float)SceneObj_GroundHeight(c);
    float alt = (float)((double)F<float>(c, 0x20) - ground);
    float yawSpeed = FromBits(0x44b80000);
    float rate = FromBits(0x3e6147ae);
    if (alt > Fl(0x5c85b0)) rate = FromBits(0x3de147ae);
    else if (alt > Fl(0x5ca190)) rate = FromBits(0x3e0bac71);
    else if (alt > Fl(0x5c86a0)) rate = FromBits(0x3e28f5c2);
    float dx = (float)((double)F<float>(c, 0x38) - F<float>(c, 4));
    float dz = (float)((double)F<float>(c, 0x40) - F<float>(c, 0xc));
    if (dz == Fl(0x5c8598)) dz = FromBits(0x3a83126f);
    float yaw = F<float>(c, 0x10);
    float target = (float)((double(__cdecl*)(double))0x5a1270)((double)dx / dz);
    if ((dx > Fl(0x5c8598) && dz < Fl(0x5c8598)) || (dx < Fl(0x5c8598) && dz < Fl(0x5c8598))) target = (float)((double)target + Fl(0x5c8760));
    while (target > Fl(0x5c8760)) target = (float)((double)target - Fl(0x5c8768));
    while (yaw > Fl(0x5c8760)) yaw = (float)((double)yaw - Fl(0x5c8768));
    while (yaw < Fl(0x5ca1f0)) yaw = (float)((double)yaw + Fl(0x5c8768));
    float turn = (float)CamFabs((double)target - yaw);
    if (turn > Fl(0x5c8760)) turn = (float)(Fl(0x5c8768) - (double)turn);
    if (turn > D(0x5ca1e8)) yawSpeed = FromBits(0x4400cccd);
    else if (turn > D(0x5ca1e0)) yawSpeed = FromBits(0x4425999a);
    else if (turn > D(0x5ca1d8)) yawSpeed = FromBits(0x448a0000);
    else if (turn > D(0x5ca1d0)) yawSpeed = FromBits(0x44a5999a);
    float zoomSpeed = F<float>(c, 0x4c) > F<float>(c, 0x18) ? FromBits(0x44898000) : FromBits(0x443b8000);
    static const uint32_t bands[] = { 0x5ca068, 0x5c86a0, 0x5c88e0, 0x5ca190, 0x5ca1c8 };
    static const uint32_t scales[] = { 0x5c8590, 0x5c8c20, 0x5c88e8, 0x5c8880, 0x5c85a0 };
    bool scaled = false;
    for (int k = 0; k < 5 && !scaled; ++k)
        if (alt < Fl(bands[k])) {
            zoomSpeed = (float)((double)zoomSpeed * D(scales[k]));
            scaled = true;
        }
    if (!scaled && !(alt < Fl(0x5c9b0c))) {
        if (alt < Fl(0x5ca1c4)) zoomSpeed = (float)((double)zoomSpeed * D(0x5c8878));
        else if (alt < Fl(0x5ca1c0)) zoomSpeed = (float)((double)zoomSpeed * D(0x5c8818));
        else zoomSpeed = (float)((double)zoomSpeed * D(0x5ca1b8));
    }
    float posDiv = Fl(0x5c85b4) > yawSpeed ? 1.0f : yawSpeed;
    double moveD = CamFabs((double)F<float>(c, 0x3c) - F<float>(c, 8));
    moveD = CamFabs((double)F<float>(c, 0x40) - F<float>(c, 0xc)) + moveD;
    float steps = (float)((CamFabs((double)F<float>(c, 0x38) - F<float>(c, 4)) + moveD) / posDiv);
    float pitchRate = (float)(CamFabs((double)F<float>(c, 0x14) - D(0x5ca1b0)) * D(0x5c8c18) + D(0x5ca1a8));
    if (pitchRate > D(0x5c8618)) pitchRate = FromBits(0x3eb33333);
    double rateD = rate < D(0x5c8678) ? CamBitsD(0x3f50624dd2f1a9fcull) : (double)rate;
    steps = (float)(CamFabs((double)F<float>(c, 0x44) - F<float>(c, 0x10)) / rateD + steps);
    steps = (float)(CamFabs((double)F<float>(c, 0x48) - F<float>(c, 0x14)) / pitchRate + steps);
    steps = (float)(CamFabs((double)F<float>(c, 0x4c) - F<float>(c, 0x18)) / zoomSpeed + steps);
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c);
    if (!hook::Global<uint8_t>(0x5ee4d0 + mode) && F<float>(c, 0x14) > D(0x5c8628) && F<float>(c, 0x18) < Fl(0x5c86a0)) {
        float q = (float)((double)F<float>(c, 0x18) / (((double)F<float>(c, 0x14) - D(0x5c8628)) * D(0x5c85c8) + D(0x5c8770)));
        steps = (float)((Fl(0x5ca1a4) - (double)q) / Fl(0x5ca06c) * steps);
    }
    if (!(steps < Fl(0x5c85b4)) || F<int32_t>(c, 0xa2)) ((void(__fastcall*)(uint8_t*))(*(void***)c)[0])(c);
    float limit;
    if (alt < Fl(0x5c9ac0)) limit = 30.0f;
    else if (alt < Fl(0x5ca06c)) limit = 80.0f;
    else limit = alt < Fl(0x5ca1a4) ? 160.0f : 350.0f;
    if ((double)limit < CamFabs((double)F<float>(c, 0x38a) - alt)) Camera_UpdateViewSettings(c);
    float z = (float)(((double(__cdecl*)(double, double))0x5a1b60)(alt, CamBitsD(0x3fd99999a0000000ull)) / D(0x5c87e0));
    z = (float)(z < D(0x5c8d68) ? (double)z : 5.0);
    z = (float)(z > D(0x5c8770) ? (double)z : 1.0);
    uint8_t* opts = hook::Global<uint8_t*>(0x6cec74);
    float far1 = (float)((double)hook::Global<float>(0x621960 + Obj_ArrayAc2(opts, 0, 4) * 4) * z * D(0x5c8578));
    Render_SetLayerValue(hook::Global<uint8_t*>(0x62c120), 0, 0, far1);
    float far2 = (float)((double)hook::Global<float>(0x621960 + Obj_ArrayAc2(hook::Global<uint8_t*>(0x6cec74), 0, 5) * 4) * z * D(0x5c8578));
    Render_SetLayerValue(hook::Global<uint8_t*>(0x62c120), 0, 0xa, far2);
    Camera_UpdateHaze(c, 0, FromBits(0xc479c000));
    if (!(steps > D(0x5c9ba0))) return;
    int32_t ix = CamFtol((double)F<float>(c, 0x1c) / Fl(0x5c8700));
    int32_t iy = CamFtol((double)F<float>(c, 0x24) / Fl(0x5c8700));
    int32_t jx = CamFtol((double)F<float>(c, 4) / Fl(0x5c8700));
    int32_t jy = CamFtol((double)F<float>(c, 0xc) / Fl(0x5c8700));
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (ix >= 0 && ix < F<int32_t>(map, 0x2155) && iy >= 0 && iy < F<int32_t>(map, 0x2159) && !Terrain_KindIs1Or4(map, 0, ix, iy)) {
        map = hook::Global<uint8_t*>(0x62c120);
        if (jx >= 0 && jx < F<int32_t>(map, 0x2155) && jy >= 0 && jy < F<int32_t>(map, 0x2159) && !Terrain_KindIs1Or4(map, 0, jx, jy)) return;
    }
    // The original also computes these distances and drops them.
    CamFabs((double)F<float>(c, 0x82) - F<float>(c, 0xc));
    CamFabs((double)F<float>(c, 0x7e) - F<float>(c, 8));
    CamFabs((double)F<float>(c, 0x7a) - F<float>(c, 4));
    CamFabs((double)F<float>(c, 0x14) - D(0x5ca1b0));
    CamFabs((double)F<float>(c, 0x72) - F<float>(c, 0x10));
    CamFabs((double)F<float>(c, 0x76) - F<float>(c, 0x14));
    CamFabs((double)F<float>(c, 0x86) - F<float>(c, 0x18));
    if (F<float>(c, 0x7a) > D(0x5c9ba0)) c[0x65] = 1;
}
RT3_REPLACE(0x43bde0, Camera_Update);

void __fastcall Camera_KeepAboveGround(uint8_t* c);
void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget);
void __fastcall Camera_SmoothHeight(uint8_t* c);
void __fastcall Camera_ClampToGround(uint8_t* c);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
void __fastcall Camera_UpdateEye(uint8_t* c);

// The common tail of the camera setters: a zoom (+0x18) below the minimum
// becomes 37.5, the pitch follows the zoom, and the camera is lifted above the
// ground.
static void Camera_ApplyZoom(uint8_t* c) {
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
    F<float>(c, 0x14) = Camera_PitchForZoom(c, 0, F<float>(c, 0x18));
    Camera_KeepAboveGround(c);
}

// 0x43c810. Turns the camera by `d` (yaw +0x10).
void __fastcall Camera_Turn(uint8_t* c, int, float d) {
    F<float>(c, 0x10) = (float)((double)d + F<float>(c, 0x10));
    Camera_ApplyZoom(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c810, Camera_Turn);

// 0x43d050. Moves the camera to (x, y, z) (+0x4..), keeps it on the map and
// over the ground, then updates it.
RT3_NOINLINE void __fastcall Camera_MoveTo(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z) {
    F<uint32_t>(c, 8) = y;
    F<uint32_t>(c, 4) = x;
    F<uint32_t>(c, 0xc) = z;
    Camera_ClampToMap(c, 0, 1);
    Camera_SmoothHeight(c);
    Camera_ApplyZoom(c);
    Camera_ClampToGround(c);
    Camera_ClampToMap(c, 0, 1);
    Camera_Update(c);
}
RT3_REPLACE(0x43d050, Camera_MoveTo);

// 0x43c610. Sets the camera's yaw, pitch (wrapped to pi and clamped to
// [0.1, 1.47]) and zoom (-1 keeps it; at least 37.5, at most vtbl +0xc); does
// nothing when all three are unchanged.
RT3_NOINLINE void __fastcall Camera_SetAngles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom) {
    pitch = (float)((double(__cdecl*)(double, double))0x5a1390)(pitch, CamBitsD(0x400921fafc8b007aull));
    if (pitch < Fl(0x5ca200)) pitch = FromBits(0xbfc90fdb);
    else if (pitch >= D(0x5ca1f8)) pitch = FromBits(0x3fc8f5c3);
    if (pitch < D(0x5c86b8)) pitch = FromBits(0x3dcccccd);
    else if (pitch >= D(0x5ca028)) pitch = FromBits(0x3fbc4303);
    if (FromBits(yaw) == F<float>(c, 0x10) && pitch == F<float>(c, 0x14) && zoom == 0xbf800000) return;
    F<uint32_t>(c, 0x10) = yaw;
    F<float>(c, 0x14) = pitch;
    if (zoom != 0xbf800000) {
        F<uint32_t>(c, 0x18) = zoom;
        if (FromBits(zoom) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
        else {
            auto maxZoom = [&] { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)c)[3])(c); };
            float m = (float)maxZoom();
            if (F<float>(c, 0x18) > m) F<float>(c, 0x18) = (float)maxZoom();
        }
    }
    Camera_ApplyZoom(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c610, Camera_SetAngles);

// 0x43c860. Orbits the camera by `d` around its centre (+0xee.., radius +0xfa,
// angle +0x106, yaw offset +0x10a) when orbiting is on (+0xea) and the step is
// large enough (or NaN).
RT3_NOINLINE void __fastcall Camera_Orbit(uint8_t* c, int, float d) {
    if (CamFabs(d) <= D(0x5c88a0) || !F<int32_t>(c, 0xea)) return;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float a = (float)((double)d + F<float>(c, 0x106));
    F<float>(c, 0x106) = a;
    double s = sin_(a);
    float z = (float)(sin_(0.0) * s * F<float>(c, 0xfa) + F<float>(c, 0xf6));
    float y = (float)(cos_(0.0) * F<float>(c, 0xfa) + F<float>(c, 0xf2));
    double k = cos_(F<float>(c, 0x106));
    float x = (float)(sin_(0.0) * k * F<float>(c, 0xfa) + F<float>(c, 0xee));
    Camera_SetTarget(c, 0, x, y, z);
    F<float>(c, 0x10) = (float)((double)F<float>(c, 0x10a) + F<float>(c, 0x106));
    Camera_UpdateEye(c);
    Camera_ClampToGround(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c860, Camera_Orbit);

// 0x43cb00. Snaps the camera's yaw to the next quarter turn in direction `dir`
// (rounding up a fraction past 0.1 / 0.9 first).
void __fastcall Camera_SnapTurn(uint8_t* c, int, int32_t dir) {
    float q = (float)((double)F<float>(c, 0x10) / D(0x5c8c08) * D(0x5c87b8));
    q = (float)((double)q + Fl(0x5ca210));
    float frac = (float)((double(__cdecl*)(double, double))0x5a1390)(q, 1.0);
    if (dir > 0 && frac > D(0x5c86b8)) q = (float)(CamFtol(q) + 1);
    if (dir < 0 && frac > D(0x5c8888)) q = (float)(CamFtol(q) + 1);
    float fd = (float)dir;
    q = (float)((double)q - fd);
    q = (float)((double)q + D(0x5c85b8));
    q = (float)(CamFtol(q) % 4);
    F<float>(c, 0x10) = (float)((double)q * D(0x5c8748));
    Camera_ApplyZoom(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43cb00, Camera_SnapTurn);

int32_t Options_Get10c();
void __fastcall Camera_UpdateTarget(uint8_t* c);

// 0x43e2c0. Applies the camera's momentum over `dt`: zoom (+0x18, rate +0x92,
// scaled with the altitude unless in camera mode 4), yaw (+0x10, +0x8a), pitch
// (+0x14, +0x8e, clamped; at least 0.1 unless options +0x104) and, with `move`,
// position (+0x4 / +0xc, +0x96 / +0x9a; followed onto the tracked object +0x60
// or the ground), then lets the rates decay.
void __fastcall Camera_ApplyMomentum(uint8_t* c, int, float dt, int32_t move) {
    if (Options_Get10c() == 4)
        F<float>(c, 0x18) = (float)((double)dt * F<float>(c, 0x92) + F<float>(c, 0x18));
    else {
        float k = F<float>(c, 0x18);
        if (k < D(0x5ca0d0)) k = FromBits(0x42160000);
        k = (float)((double)k - (F<int32_t>(c, 0x60) ? D(0x5c8d58) : D(0x5ca268)));
        k = (float)(((double(__cdecl*)(double))0x5a1634)((double)k / Fl(0x5c9ac4)) * D(0x5c88e8) + D(0x5c85b8));
        if (F<float>(c, 0x18) < Fl(0x5ca06c)) k = (float)((double)k * D(0x5c88c8));
        else if (F<float>(c, 0x18) < Fl(0x5ca1a4))
            k = (float)((((double)F<float>(c, 0x18) - Fl(0x5ca06c)) / D(0x5c87f8) * D(0x5c8680) + D(0x5c88c8)) * k);
        k = (float)((double)F<float>(c, 0x92) * k * dt * D(0x5c85a8));
        F<float>(c, 0x18) = (float)((double)k + F<float>(c, 0x18));
    }
    F<float>(c, 0x10) = (float)((double)dt * F<float>(c, 0x8a) + F<float>(c, 0x10));
    float pitch = (float)((double)dt * F<float>(c, 0x8e) + F<float>(c, 0x14));
    F<float>(c, 0x14) = pitch;
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
    else if (F<float>(c, 0x18) > Fl(0x5ca260)) F<uint32_t>(c, 0x18) = 0x4703a400;
    if (pitch < D(0x5ca258)) F<uint32_t>(c, 0x14) = 0xbf8ccccd;
    else if (pitch > D(0x5ca028)) F<uint32_t>(c, 0x14) = 0x3fbc4303;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x104) && F<float>(c, 0x14) < D(0x5c86b8)) F<uint32_t>(c, 0x14) = 0x3dcccccd;
    F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
    F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
    if (move) {
        uint8_t* follow = F<uint8_t*>(c, 0x60);
        if (follow) {
            ((void(__fastcall*)(uint8_t*, int, float*, float*, float*))(*(void***)follow)[2])(follow, 0, &F<float>(c, 4), &F<float>(c, 8), &F<float>(c, 0xc));
            Camera_ClampToMap(c, 0, 1);
        } else {
            Camera_ClampToMap(c, 0, 1);
            F<float>(c, 8) = (float)(Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc)) + Fl(0x5c8898));
        }
        Camera_UpdateTarget(c);
        Camera_ClampToGround(c);
        Camera_Update(c);
    }
    if (Options_Get10c() == 4) {
        double k = D(0x5c8770) - (double)dt * D(0x5c86b8);
        F<float>(c, 0x8a) = (float)((double)F<float>(c, 0x8a) * k);
        F<float>(c, 0x8e) = (float)((double)F<float>(c, 0x8e) * k);
        F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5ca250)) * F<float>(c, 0x92));
        k = D(0x5c8770) - (double)dt * D(0x5c8648);
        F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * k);
        F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * k);
    } else {
        F<float>(c, 0x8a) = (float)((D(0x5c8770) - (double)dt * D(0x5c88c8)) * F<float>(c, 0x8a));
        F<float>(c, 0x8e) = (float)((D(0x5c8770) - (double)dt * D(0x5c8758)) * F<float>(c, 0x8e));
        F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5c8618)) * F<float>(c, 0x92));
        double k = D(0x5c8770) - (double)dt * D(0x5c8590);
        F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * k);
        F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * k);
    }
}
RT3_REPLACE(0x43e2c0, Camera_ApplyMomentum);

void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);

// 0x43c980. Sets the camera's zoom, keeping yaw and pitch.
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom) {
    Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), zoom);
}
RT3_REPLACE(0x43c980, Camera_SetZoom);

// 0x43cdf0. Puts the camera on a new map: sets it up, then the default view
// (yaw pi, pitch 0.2, zoom 2048) and bounds, and notifies it (vtbl +0x10).
RT3_NOINLINE void __fastcall Camera_ResetView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    Camera_Setup(c, 0, terrain, x, y, z, dist);
    Camera_ApplyZoom(c);
    Camera_SetAngles(c, 0, 0x40490fdb, FromBits(0x3e4ccccd), 0x45000000);
    Camera_SetBounds(c, 0, 0, 0, 0x3f800000, 0x3f333333);
    ((void(__fastcall*)(uint8_t*))(*(void***)c)[4])(c);
}
RT3_REPLACE(0x43cdf0, Camera_ResetView);

// 0x43d0c0. Moves the camera to map cell (x, z), on the ground.
void __fastcall Camera_MoveToCell(uint8_t* c, int, float x, float z) {
    uint32_t h = 0;
    if (F<uint8_t*>(c, 0x6e)) {
        float wz = (float)((double)z * Fl(0x5c8700));
        float hh = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)x * Fl(0x5c8700)), wz);
        h = Bits(hh);
    }
    uint32_t wz = Bits((float)((double)z * Fl(0x5c8700)));
    Camera_MoveTo(c, 0, Bits((float)((double)x * Fl(0x5c8700))), h, wz);
    Camera_Update(c);
}
RT3_REPLACE(0x43d0c0, Camera_MoveToCell);

// 0x43d130. Pans the camera by (dx, dz) screen units, scaled with the square
// root of the zoom and turned by the yaw.
RT3_NOINLINE void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz) {
    auto sqrt_ = (double(__cdecl*)(double))0x5a1634;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float s = Fl(0x5c85b4) > F<float>(c, 0x18) ? 1.0f : F<float>(c, 0x18);
    double r = sqrt_((double)s + D(0x5c9680));
    float f = (float)(r / sqrt_(62.0));
    f = (float)((double)f * Fl(0x5c8900));
    dx = (float)((double)f * dx);
    dz = (float)((double)f * dz);
    float a = (float)((double)dz * Fl(0x5c8904));
    float t1 = (float)(cos_(F<float>(c, 0x10)) * a);
    float t2 = (float)(sin_(F<float>(c, 0x10)) * a);
    float b = (float)((double)dx * Fl(0x5c8904));
    float mx = (float)((double)t1 - sin_(F<float>(c, 0x10)) * b);
    float mz = (float)(cos_(F<float>(c, 0x10)) * b + t2);
    uint32_t z = Bits((float)((double)mz + F<float>(c, 0xc)));
    Camera_MoveTo(c, 0, Bits((float)((double)mx + F<float>(c, 4))), F<uint32_t>(c, 8), z);
    Camera_Update(c);
}
RT3_REPLACE(0x43d130, Camera_Pan);

// 0x43cc30. Zooms one step in direction `dir` along the zoom ladder 0x5ee4dc
// (7 levels, compared by square root, 0x62be4c caches the roots): the current
// position on the ladder (fractions past 0.9 / below 0.1 snap), plus `dir`;
// the bottom is 37.5, the top 1e6.
void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir) {
    auto sqrt_ = (double(__cdecl*)(double))0x5a1634;
    float* roots = (float*)0x62be4c;
    for (int32_t i = 0; i < 7; ++i) roots[i] = (float)sqrt_((double)hook::Global<int32_t>(0x5ee4dc + i * 4));
    float s = Fl(0x5c85b4) > F<float>(c, 0x18) ? 1.0f : F<float>(c, 0x18);
    float q = (float)sqrt_(s);
    float pos = 6.0f;
    for (int32_t i = 0; i < 7; ++i)
        if (q < roots[i]) {
            if (!i) pos = 0.0f;
            else {
                float base = (float)(i - 1);
                pos = (float)(((double)q - roots[i - 1]) / ((double)roots[i] - roots[i - 1]) + base);
            }
            break;
        }
    float frac = (float)((double(__cdecl*)(double, double))0x5a1390)(pos, 1.0);
    if (frac > D(0x5ca218)) pos = (float)(CamFtol(pos) + 1);
    if (frac < D(0x5c9cb0)) pos = (float)CamFtol(pos);
    float d = (float)dir;
    int32_t idx = CamFtol((double)d + pos + D(0x5c85b8));
    if (idx < 0) idx = 0;
    uint32_t zoom;
    if (idx > 6 || idx == 6) zoom = 0x497423f0;
    else if (!idx) zoom = 0x42160000;
    else zoom = Bits((float)hook::Global<int32_t>(0x5ee4dc + idx * 4));
    Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), zoom);
    Camera_ClampToGround(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43cc30, Camera_ZoomStep);

void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);

// 0x43f050. The camera step of the follow / orbit modes over `dt`: tilts the
// view vector (+0x2c..+0x34) with the yaw rate, turns (orbiting when options
// +0x108), pitches (the pitch rate +0x8e is used up), zooms (scaled with the
// altitude, at least 37.5); with `move` follows the tracked object +0x60 (moving
// when it is more than 0.5 away) or drifts by the position rates; refreshes the
// view when anything moved or +0x9e asks for it; then lets the rates decay.
void __fastcall Camera_Step(uint8_t* c, int, float dt, int32_t move) {
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    double t = F<float>(c, 0x2c);
    double s14 = sin_(F<float>(c, 0x14));
    F<float>(c, 0x2c) = (float)(t - sin_(F<float>(c, 0x10)) * s14 * F<float>(c, 0x8a) * D(0x5c8628));
    t = F<float>(c, 0x34);
    double k = cos_(-(double)F<float>(c, 0x10));
    F<float>(c, 0x34) = (float)(t - sin_(F<float>(c, 0x14)) * k * F<float>(c, 0x8a) * D(0x5c8628));
    k = cos_(F<float>(c, 0x10));
    k = sin_(F<float>(c, 0x14)) * k;
    F<float>(c, 0x30) = (float)(D(0x5c8770) - sin_(F<float>(c, 0x10)) * k * F<float>(c, 0x8a));
    F<float>(c, 0x2c) = (float)((double)F<float>(c, 0x2c) * D(0x5c8680));
    F<float>(c, 0x34) = (float)((double)F<float>(c, 0x34) * D(0x5c8680));
    double turn = (double)dt * F<float>(c, 0x8a);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x108)) Camera_Orbit(c, 0, (float)turn);
    else F<float>(c, 0x10) = (float)(turn + F<float>(c, 0x10));
    double pitch = (double)dt * F<float>(c, 0x8e);
    F<uint32_t>(c, 0x8e) = 0;
    F<float>(c, 0x14) = (float)(pitch + F<float>(c, 0x14));
    float l = F<float>(c, 0x18);
    if (l < D(0x5ca0d0)) l = FromBits(0x42160000);
    l = (float)((double)l - (F<int32_t>(c, 0x60) ? D(0x5c8d58) : D(0x5ca268)));
    l = (float)(((double(__cdecl*)(double))0x5a1634)((double)l / Fl(0x5c9ac4)) * D(0x5c88e8) + D(0x5c85b8));
    if (l > D(0x5c8820)) l = (float)((((double)l - D(0x5c8820)) * D(0x5c8680) + D(0x5c8770)) * l);
    float m = (float)((double)F<float>(c, 0x92) * l * dt * D(0x5c85a8));
    float z = (float)((double)m + F<float>(c, 0x18));
    F<float>(c, 0x18) = z;
    F<float>(c, 0x18) = (float)(z > D(0x5ca0d0) ? (double)z : 37.5);
    if (!move) {
        F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
        F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
    } else {
        if (uint8_t* follow = F<uint8_t*>(c, 0x60)) {
            float fx, fy, fz;
            ((void(__fastcall*)(uint8_t*, int, float*, float*, float*))(*(void***)follow)[2])(follow, 0, &fx, &fy, &fz);
            fy = (float)((double)fy + Fl(0x5c873c));
            if (CamFabs((double)fx - F<float>(c, 4)) > D(0x5c85b8) || CamFabs((double)fy - F<float>(c, 8)) > D(0x5c85b8) ||
                CamFabs((double)fz - F<float>(c, 0xc)) > D(0x5c85b8))
                Camera_MoveTo(c, 0, Bits(fx), Bits(fy), Bits(fz));
            uint32_t a, b, d;
            SceneObj_GetAngles(F<uint8_t*>(c, 0x60), 0, &d, &b, &a);
        } else if (F<float>(c, 0x96) != Fl(0x5c8598) || F<float>(c, 0x9a) != Fl(0x5c8598)) {
            F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
            F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
            Camera_ClampToMap(c, 0, 1);
        }
        const float zero = Fl(0x5c8598);
        if (F<int32_t>(c, 0x9e) || F<float>(c, 0x8a) != zero || F<float>(c, 0x8e) != zero || F<float>(c, 0x92) != zero || F<float>(c, 0x96) != zero ||
            F<float>(c, 0x9a) != zero) {
            F<int32_t>(c, 0x9e) = 0;
            Camera_SmoothHeight(c);
            Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), F<uint32_t>(c, 0x18));
            Camera_ClampToGround(c);
            Camera_Update(c);
        }
    }
    F<float>(c, 0x8a) = (float)((D(0x5c8770) - (double)dt * D(0x5c88c8)) * F<float>(c, 0x8a));
    F<float>(c, 0x8e) = (float)((D(0x5c8770) - (double)dt * D(0x5c8758)) * F<float>(c, 0x8e));
    F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5c8618)) * F<float>(c, 0x92));
    double r = D(0x5c8770) - (double)dt * D(0x5c8590);
    F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * r);
    F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * r);
}
RT3_REPLACE(0x43f050, Camera_Step);

double __fastcall Obj_Scene36f9(uint8_t* o);
float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t);
int32_t __fastcall Camera_PickGround(uint8_t* cam, int, float sx, float sy, float height, float* cx, float* cz);
int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// 0x43ea40. Drags the map: the mouse moved from (x0, y0) (kept in +0x32e /
// +0x332) to (x1, y1). Picks the ground under both points at a few heights
// (the drag plane +0x31a above the camera's +0x20, falling back to 50 below,
// one unit below, and the ground) along the screen's middle column and the
// row band (+0x326, +0x32a), blends the height-based shift (weight by the
// altitude) with the direct one, and moves the camera by it. With +0x312 set
// and the blend nearly direct, it checks that the grabbed tile (+0x31e /
// +0x322) is back under the mouse and, when not, recurses once (`nested`)
// with a correction of at most 0.0015 (or half the drag distance).
RT3_NOINLINE void __fastcall Camera_DragPick(uint8_t* c, int, uint32_t x0, uint32_t y0, float x1, float y1, int32_t nested) {
    F<uint32_t>(c, 0x32e) = x0;
    F<uint32_t>(c, 0x332) = y0;
    Obj_Scene36f9((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)));
    float lowY = F<float>(c, 0x332) < y1 ? F<float>(c, 0x332) : y1;
    float probe = lowY;
    if (F<float>(c, 0x32a) < lowY && F<float>(c, 0x31a) > Fl(0x5c9f80)) probe = F<float>(c, 0x32a);
    float alt = Obj_ScaledSpanOffset(c, 0, probe);
    float frac;
    if (alt < D(0x5c86b8)) frac = 0.0f;
    else if (alt < D(0x5c8988)) frac = (float)(((double)alt - D(0x5c86b8)) / D(0x5ca290));
    else frac = 1.0f;
    float shift[2] = { 0.0f, 0.0f }, direct[2] = { 0.0f, 0.0f };
    int32_t found = 0;
    float ax0 = 0.0f, ax1 = 0.0f, az0 = 0.0f, az1 = 0.0f;  // pick outputs (+0x2c / +0x34 for the start, +0x28 / +0x30 for the end)
    auto plane = [&](int32_t pass) {
        float lvl = Fl(0x5c9f80) < F<float>(c, 0x31a) ? FromBits(0xc2480000) : F<float>(c, 0x31a);
        float v = (float)((double)lvl + F<float>(c, 0x20));
        uint32_t h = v < Fl(0x5c8598) ? 0 : Bits(v);
        if (pass == 1) h = Bits((float)((double)F<float>(c, 0x20) - Fl(0x5c8780)));
        else if (pass == 2) h = 0;
        return FromBits(h);
    };
    for (int32_t pass = 0; pass < 3; ++pass) {
        float h = plane(pass);
        float half = (float)(((double)F<float>(c, 0x5c) - F<float>(c, 0x54)) * D(0x5c8628));
        float sy = (float)(pass ? (double)half : ((double)half + F<float>(c, 0x32a)) * D(0x5c8628));
        int32_t r1 = Camera_PickGround(c, 0, F<float>(c, 0x32e), sy, h, &ax0, &az0);
        sy = (float)(pass ? (double)half : ((double)half + F<float>(c, 0x32a)) * D(0x5c8628));
        int32_t r2 = Camera_PickGround(c, 0, x1, sy, h, &ax1, &az1);
        if (!r1 || !r2) continue;
        direct[0] = (float)((double)ax1 - ax0);
        direct[1] = (float)((double)az1 - az0);
        float h2 = plane(pass);
        r1 = Camera_PickGround(c, 0, F<float>(c, 0x326), half, h2, &ax0, &az0);
        float hp = (float)((double)half + D(0x5c85b8));
        r2 = Camera_PickGround(c, 0, F<float>(c, 0x326), hp, h2, &ax1, &az1);
        if (r1 && r2) {
            found = 1;
            float q = (float)(((double)y1 - F<float>(c, 0x332)) / D(0x5c85b8));
            direct[0] = (float)(((double)ax1 - ax0) * q + direct[0]);
            direct[1] = (float)(((double)az1 - az0) * q + direct[1]);
            break;
        }
    }
    float top = (float)((double)F<float>(c, 0x31a) + F<float>(c, 0x20));
    int32_t r1 = Camera_PickGround(c, 0, F<float>(c, 0x32e), F<float>(c, 0x332), top, &ax0, &az0);
    int32_t r2 = Camera_PickGround(c, 0, x1, y1, top, &ax1, &az1);
    if (r1 && r2) {
        shift[0] = (float)((double)ax1 - ax0);
        shift[1] = (float)((double)az1 - az0);
        if (!found) frac = 1.0f;
    } else {
        if (!found) return;
        frac = 0.0f;
    }
    double k = D(0x5c8770) - frac;
    float dx = (float)((double)direct[0] * k + (double)shift[0] * frac);
    float dz = (float)((double)direct[1] * k + (double)shift[1] * frac);
    uint32_t z = Bits((float)((double)F<float>(c, 0xc) - (double)dz * Fl(0x5c8700)));
    Camera_MoveTo(c, 0, Bits((float)((double)F<float>(c, 4) - (double)dx * Fl(0x5c8700))), F<uint32_t>(c, 8), z);
    float sx, sy;
    if (F<int32_t>(c, 0x312) && frac >= D(0x5ca288) && !nested && Map_TileToScreen(F<float>(c, 0x31e), F<float>(c, 0x322), top, &sx, &sy)) {
        float e1 = (float)((double)sx - x1);
        float e2 = (float)((double)sy - y1);
        if (!(CamFabs(e1) > D(0x5c8758)) && !(CamFabs(e2) > D(0x5c8758))) {
            float lim = FromBits(0x3ac49ba6);
            double d = (float)Distance2D(x1, y1, F<float>(c, 0x32e), F<float>(c, 0x332)) * D(0x5c8628);
            if (D(0x5ca280) > d) lim = (float)d;
            if (e1 > lim) e1 = lim;
            float nl = -lim;
            if (e1 < nl) e1 = nl;
            if (e2 > lim) e2 = lim;
            if (e2 < nl) e2 = nl;
            float nx = (float)((double)e1 + x1);
            F<float>(c, 0x32e) = nx;
            float nz = (float)((double)e2 + y1);
            F<float>(c, 0x332) = nz;
            Camera_DragPick(c, 0, Bits(nx), Bits(nz), x1, y1, 1);
            F<float>(c, 0x32e) = x1;
            F<float>(c, 0x332) = y1;
        }
    }
    Camera_Update(c);
}
RT3_REPLACE(0x43ea40, Camera_DragPick);

uint8_t* __fastcall Camera_Construct(uint8_t* c);
void __fastcall Camera_Reset(uint8_t* c);
int32_t __fastcall Tool_RemoveModifier();
int32_t __fastcall Tool_AddModifier(uint8_t* t);
void __fastcall Camera_UpdateFocus(uint8_t* c);
void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t);

// 0x43d260. Constructs the map camera (vtable 0x5ca078) and puts it on the
// map (terrain, position, distance).
uint8_t* __fastcall Camera_ConstructView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    Camera_Construct(c);
    F<uint32_t>(c, 0) = 0x5ca078;
    Camera_Reset(c);
    Camera_ResetView(c, 0, terrain, x, y, z, dist);
    return c;
}
RT3_REPLACE(0x43d260, Camera_ConstructView);

// 0x43f4c0. Continues a map drag to the current mouse position (0x6d4018
// +0xa94 / +0xa98).
RT3_NOINLINE void __fastcall Camera_DragToMouse(uint8_t* c) {
    uint8_t* m = hook::Global<uint8_t*>(0x6d4018);
    Camera_DragPick(c, 0, F<uint32_t>(c, 0x32e), F<uint32_t>(c, 0x332), F<float>(m, 0xa94), F<float>(m, 0xa98), 0);
}
RT3_REPLACE(0x43f4c0, Camera_DragToMouse);

// 0x43f4f0. Applies the mouse to the camera: while the drag modifier is held
// (Tool_RemoveModifier) it is a free drag by (dx, dy) (slower unless the
// screen's +0x1146d7), otherwise orbiting stops and, with the grab modifier,
// the map follows the mouse; the wheel `wheel` adds to the zoom rate scaled
// by the zoom speed option +0x118.
void __fastcall Camera_MouseInput(uint8_t* c, int, float dx, float dy, float wheel) {
    int32_t freeDrag = Tool_RemoveModifier();
    int32_t grab = Tool_AddModifier(c);
    if (!freeDrag) {
        if (!grab && CamFabs(wheel) <= D(0x5c8678)) return;
        F<int32_t>(c, 0xea) = 0;
        F<int32_t>(c, 0x8a) = 0;
        F<int32_t>(c, 0x306) = 0;
    } else {
        if (!F<int32_t>(c, 0xe6)) Camera_UpdateFocus(c);
        if (!hook::Global<uint8_t*>(0x6d4024)[0x1146d7]) dy = (float)((double)dy * Fl(0x5c9b08));
        Camera_Drag(c, 0, dx, dy, *(int32_t*)&wheel);
    }
    F<int32_t>(c, 0xe6) = freeDrag;
    if (grab) {
        uint8_t* m = hook::Global<uint8_t*>(0x6d4018);
        Camera_DragPick(c, 0, F<uint32_t>(c, 0x32e), F<uint32_t>(c, 0x332), F<float>(m, 0xa94), F<float>(m, 0xa98), 0);
    } else
        F<int32_t>(c, 0x302) = 0;
    if (wheel == Fl(0x5c8598)) return;
    float k = hook::Global<float>(0x5f34b0 + F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x118) * 4);
    double v = k * ((double)wheel * D(0x5c8628));
    F<float>(c, 0x92) = (float)(v + v + F<float>(c, 0x92));
}
RT3_REPLACE(0x43f4f0, Camera_MouseInput);

void __fastcall Camera_DragToMouse(uint8_t* c);

// 0x43e610. Mouse input for the momentum cameras: a free drag adds (dx, dy)
// to the turn and pitch rates (finer in camera mode 4); with the grab
// modifier mode 4 pushes the position rates (+0x96 / +0x9a) along the view,
// scaled with the height over the ground, other modes drag the map to the
// mouse; the wheel adds to the zoom rate (scaled with the height in mode 4).
void __fastcall Camera_MouseInputMomentum(uint8_t* c, int, float dx, float dy, float wheel) {
    int32_t freeDrag = Tool_RemoveModifier();
    int32_t grab = Tool_AddModifier(c);
    if (!freeDrag && !grab && CamFabs(wheel) <= D(0x5c8678)) return;
    float ground = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc));
    float k = (float)(CamFabs((double)F<float>(c, 0x20) - ground) / D(0x5c87f8) + D(0x5c9ba0));
    if (freeDrag) {
        if (Options_Get10c() == 4) {
            F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5c86a0) + F<float>(c, 0x8a));
            F<float>(c, 0x8e) = (float)((double)dy / Fl(0x5c88e0) + F<float>(c, 0x8e));
        } else {
            F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5ca19c) + F<float>(c, 0x8a));
            F<float>(c, 0x8e) = (float)((double)dy / Fl(0x5ca270) + F<float>(c, 0x8e));
        }
    } else if (grab) {
        if (Options_Get10c() == 4) {
            auto sin_ = (double(__cdecl*)(double))0x5a1494;
            auto cos_ = (double(__cdecl*)(double))0x5a13e4;
            float a = (float)((double)k * dy);
            double t = F<float>(c, 0x96);
            F<float>(c, 0x96) = (float)(t - cos_(F<float>(c, 0x10)) * a);
            t = F<float>(c, 0x9a);
            F<float>(c, 0x9a) = (float)(t - sin_(F<float>(c, 0x10)) * a);
            float b = (float)((double)k * dx);
            F<float>(c, 0x96) = (float)(sin_(F<float>(c, 0x10)) * b + F<float>(c, 0x96));
            t = F<float>(c, 0x9a);
            F<float>(c, 0x9a) = (float)(t - cos_(F<float>(c, 0x10)) * b);
        } else
            Camera_DragToMouse(c);
    }
    if (wheel == Fl(0x5c8598)) return;
    double v;
    if (Options_Get10c() == 4)
        v = (double)k * D(0x5c86b8) * wheel * hook::Global<float>(0x5f34b0 + F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x118) * 4);
    else {
        float s = hook::Global<float>(0x5f34b0 + F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x118) * 4);
        v = s * ((double)wheel * D(0x5c8628));
    }
    F<float>(c, 0x92) = (float)(v + v + F<float>(c, 0x92));
}
RT3_REPLACE(0x43e610, Camera_MouseInputMomentum);

} // namespace rt3
