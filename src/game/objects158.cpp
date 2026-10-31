// Camera eye move / camera shake / camera reset / camera-driven scene detail in block 0x43 (batch 162). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_ClampToGround(uint8_t* c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t ClockMs();
void __fastcall Sound_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
uint32_t __fastcall Camera_Init(uint8_t* c);
void __fastcall Caps_Load(uint8_t* o);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint32_t U(float f) { return *(uint32_t*)&f; }
static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }
static inline double Cos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }
static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
static inline uint8_t* SoundSys() { return hook::Global<uint8_t*>(0x6d402c); }

// 0x43a1a0. Moves the camera eye to (x, z) scaled by k (keeping its height), with
// no easing, and keeps it above ground.
void __fastcall Camera_MoveEyeTo(uint8_t* c, int, float x, float z) {
    float k = Fl(0x5c8700);
    Camera_SetEye(c, 0, U((float)((double)x * k)), F<uint32_t>(c, 0xc), U((float)((double)z * k)));
    F<float>(c, 0x34e) = 1.0f;
    Camera_ClampToGround(c);
}
RT3_REPLACE(0x43a1a0, Camera_MoveEyeTo);

// 0x43a1e0. Camera shake (+0x38e on, strength +0x392, start ms +0x396): a cosine-
// decaying offset (+0x3a6..+0x3ae, scaled down near the ground) added to the base eye
// (+0x39a..+0x3a2), and the rumble sound (+0x3b2) volume follows it; when it runs out
// the sound stops. Quirk: the remembered volume +0x3b6 is reset to -1 every frame.
void __fastcall Camera_UpdateShake(uint8_t* c) {
    if (!F<int32_t>(c, 0x38e)) return;
    float a = (float)((Sqrt(F<float>(c, 0x392)) + F<float>(c, 0x392)) / D(0x5c85d0));
    float lim1 = (float)((double)a * Fl(0x5ca074));
    float lim2 = (float)((double)a * Fl(0x5ca070));
    float tf = (float)ClockMs();
    float e = (float)((double)tf - F<float>(c, 0x396));
    if (!(e > lim2) && !(e < Fl(0x5c8598))) {
        float q = (float)((double)e * Fl(0x5c8760));
        float cr = (float)Cos((double)q / lim2);
        float amp = (float)((double)cr * a);
        double vv = amp < D(0x5c85b8) ? 0.01 : (double)amp;
        amp = (float)vv;
        float vol = (float)((Sqrt(amp) + amp) / D(0x5c85d0));
        if (Fabs((double)vol - F<float>(c, 0x3b6)) > D(0x5c9b98)) {
            F<float>(c, 0x3b6) = vol;
            Sound_SetSampleVolume(SoundSys(), 0, F<uint32_t>(c, 0x3b2), vol);
        }
        F<uint32_t>(c, 0x3b6) = 0xbf800000;
        float d = (float)(F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24)));
        uint32_t f;
        if (d < Fl(0x5c8c28)) f = 0x3e99999a;
        else if (d < Fl(0x5c87c0)) f = 0x3ecccccd;
        else if (d < Fl(0x5ca06c)) f = 0x3f000000;
        else if (d < Fl(0x5ca068)) f = 0x3f19999a;
        else if (d < Fl(0x5c86a0)) f = 0x3f400000;
        else if (d < Fl(0x5c88e0)) f = 0x3f59999a;
        else f = d < Fl(0x5c85b0) ? 0x3f800000 : 0x3f99999a;
        float fac = *(float*)&f;
        float s = 0.0f;
        if (e < lim1) s = (float)(Cos((double)q / lim1) * ((double)fac * F<float>(c, 0x392)));
        float e2 = (float)((double)e * Fl(0x5c8768));
        F<float>(c, 0x3a6) = (float)(Cos((double)e2 / Fl(0x5c8c28)) * s * D(0x5ca060));
        F<float>(c, 0x3aa) = (float)(Cos((double)e2 / Fl(0x5ca05c)) * s * D(0x5c9ab8));
        F<float>(c, 0x3ae) = (float)(Cos((double)e2 / Fl(0x5ca058)) * s * D(0x5ca050));
    } else {
        F<int32_t>(c, 0x38e) = 0;
        F<int32_t>(c, 0x3a6) = 0;
        F<int32_t>(c, 0x3aa) = 0;
        F<int32_t>(c, 0x3ae) = 0;
        Sound_Stop(SoundSys(), 0, F<uint32_t>(c, 0x3b2), 0);
        F<int32_t>(c, 0x3b2) = 0;
    }
    float z = (float)((double)F<float>(c, 0x3a2) + F<float>(c, 0x3ae));
    float y = (float)((double)F<float>(c, 0x39e) + F<float>(c, 0x3aa));
    float x = (float)((double)F<float>(c, 0x39a) + F<float>(c, 0x3a6));
    Camera_SetEye(c, 0, U(x), U(y), U(z));
    F<float>(c, 0x34e) = 1.0f;
}
RT3_REPLACE(0x43a1e0, Camera_UpdateShake);

// 0x43a5a0. Resets the camera state and reloads the render capabilities.
void __fastcall Camera_Reset(uint8_t* c) {
    Camera_Init(c);
    for (int32_t i = 0; i < 10; ++i) F<int32_t>(c, 0x10e + i * 0x30) = 0;
    F<int32_t>(c, 0x9e) = 1;
    static const uint16_t zero[] = { 0xa2, 0x2ee, 0x346, 0x36a, 0x386, 0x36e, 0x372, 0x376, 0x37a, 0x37e, 0x382,
                                     0x38e, 0x3b2, 0x92, 0x8e, 0x8a, 0x9a, 0x96 };
    for (uint16_t off : zero) F<int32_t>(c, off) = 0;
    F<uint32_t>(c, 0x38a) = 0xc479c000;
    for (uint32_t off = 0x2f2; off <= 0x342; off += 4)
        if (off == 0x2f2 || off >= 0x302) F<int32_t>(c, off) = 0;
    for (uint32_t off = 0x352; off <= 0x366; off += 4) F<int32_t>(c, off) = 0;
    F<float>(c, 0x34e) = 1.0f;
    for (uint32_t off = 0xe6; off <= 0x10a; off += 4) F<int32_t>(c, off) = 0;
    for (uint32_t off = 0xa6; off < 0xe6; off += 4) F<int32_t>(c, off) = 0;
    Caps_Load(c);
}
RT3_REPLACE(0x43a5a0, Camera_Reset);

// 0x43b920. Picks the scene detail level from the camera's height above ground (2 low
// .. 32 high); when the camera moved more than a height-based distance since the last
// snapshot (+0x36e..+0x382) it drops to 1 and takes a new snapshot, except that after
// 12 small moves it only halves; forced to 1 once after +0x36a.
void __fastcall Camera_UpdateDetail(uint8_t* c) {
    float alt = (float)(F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24)));
    double sum = Fabs((double)F<float>(c, 0x24) - F<float>(c, 0x382));
    sum = Fabs((double)F<float>(c, 0x20) - F<float>(c, 0x37e)) + sum;
    sum = Fabs((double)F<float>(c, 0x1c) - F<float>(c, 0x37a)) + sum;
    sum = Fabs((double)F<float>(c, 0xc) - F<float>(c, 0x376)) + sum;
    sum = Fabs((double)F<float>(c, 8) - F<float>(c, 0x372)) + sum;
    sum = Fabs((double)F<float>(c, 4) - F<float>(c, 0x36e)) + sum;
    int32_t moved;
    __asm {
        fld sum
        mov eax, 0x5a10d0
        call eax
        mov moved, eax
    }
    int32_t level;
    if (alt <= Fl(0x5c8cf8)) level = 2;
    else if (alt <= Fl(0x5ca19c)) level = 3;
    else if (alt <= Fl(0x5ca164)) level = 4;
    else if (alt <= Fl(0x5ca198)) level = 5;
    else if (alt <= Fl(0x5ca194)) level = 8;
    else if (alt <= Fl(0x5ca160)) level = 0xc;
    else level = alt <= Fl(0x5c85b0) ? 0x10 : 0x20;
    float th;
    if (alt < Fl(0x5ca06c)) th = 30.0f;
    else if (alt < Fl(0x5c86a0)) th = 80.0f;
    else if (alt < Fl(0x5c88e0)) th = 200.0f;
    else th = alt < Fl(0x5ca190) ? 350.0f : 700.0f;
    if (F<int32_t>(c, 0x36a)) {
        level = 1;
        F<int32_t>(c, 0x36a) = 0;
    }
    float mv = (float)moved;
    if (mv > th) {
        int32_t n = F<int32_t>(c, 0x386);
        if (n > 12 && !((double)th * Fl(0x5c8568) < mv)) {
            level >>= 1;
            F<int32_t>(c, 0x386) = n + 1;
        } else {
            F<uint32_t>(c, 0x36e) = F<uint32_t>(c, 4);
            F<uint32_t>(c, 0x372) = F<uint32_t>(c, 8);
            F<uint32_t>(c, 0x376) = F<uint32_t>(c, 0xc);
            F<uint32_t>(c, 0x37a) = F<uint32_t>(c, 0x1c);
            F<uint32_t>(c, 0x37e) = F<uint32_t>(c, 0x20);
            F<uint32_t>(c, 0x382) = F<uint32_t>(c, 0x24);
            level = 1;
            F<int32_t>(c, 0x386) = 0;
            F<int32_t>(c, 0x36a) = 1;
            ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x3748] = 1;
        }
    }
    Scene_SetDetail((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, level);
}
RT3_REPLACE(0x43b920, Camera_UpdateDetail);

} // namespace rt3
