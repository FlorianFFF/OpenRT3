// Shadow view setup in block 0x54 (batch 206). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename T> static inline T& G(uint32_t a) { return *(T*)a; }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
static inline double Fsqrt(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Obj_GetXYZ2c(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);

static inline void LookAt(void* out, const float* eye, const float* at, const float* up) {
    ((void*(__stdcall*)(void*, const float*, const float*, const float*))0x56e2d2)(out, eye, at, up);
}

// 0x548350. Builds the view matrices from camera `cam`: the camera view (+0x34af),
// the reflection view (+0x346f; mirrored in the water plane 2 x +0x2537 when
// `reflect`), the light-direction view (+0x35ef) and the shadow map: an
// orthographic projection (+0x356f) sized by the camera's height above the ground
// (max 1536) and a view from the light (+0x362f) placed back from the camera
// position snapped to the terrain cell grid (stored in +0x3701..). Changed shadow
// parameters (cached at 0x625438..) mark the shadow map dirty (+0x36f7).
void __fastcall Renderer_SetupViews(uint8_t* r, int, uint8_t* cam, uint8_t reflect) {
    float at[3], eye[3], up[3];
    Obj_GetXYZ4(cam, 0, (uint32_t*)&at[0], (uint32_t*)&at[1], (uint32_t*)&at[2]);
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&eye[0], (uint32_t*)&eye[1], (uint32_t*)&eye[2]);
    Obj_GetXYZ2c(cam, 0, (uint32_t*)&up[0], (uint32_t*)&up[1], (uint32_t*)&up[2]);
    LookAt(r + 0x34af, eye, at, up);
    if (reflect) {
        double w2 = (double)F<float>(r, 0x2537) + F<float>(r, 0x2537);
        at[1] = (float)(w2 - at[1]);
        eye[1] = (float)(w2 - eye[1]);
        up[0] = (float)(up[0] * D(0x5c9bf0));
        up[2] = (float)(up[2] * D(0x5c9bf0));
    }
    LookAt(r + 0x346f, eye, at, up);
    {
        double dx = (double)at[0] - eye[0];
        at[0] = (float)dx;
        double dy = (double)at[1] - eye[1];
        float ndx = (float)dx;
        double dz = (double)at[2] - eye[2];
        double len = Fsqrt(dz * dz + dy * dy + (double)at[0] * at[0]);
        if (Fl(0x5c8598) < len) {
            double inv = Fl(0x5c85b4) / len;
            ndx = (float)((double)at[0] * inv);
            dy = dy * inv;
            dz = dz * inv;
        }
        float dir[3] = { ndx, (float)dy, (float)dz };
        float zero[3] = { 0.0f, 0.0f, 0.0f };
        LookAt(r + 0x35ef, zero, dir, up);
    }
    Obj_GetXYZ4(cam, 0, (uint32_t*)&eye[0], (uint32_t*)&eye[1], (uint32_t*)&eye[2]);
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&at[0], (uint32_t*)&at[1], (uint32_t*)&at[2]);
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, at[0], at[2]);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    double g = F<int32_t>(e, 0x11420e);
    int32_t n = Ftol(((double)at[1] - h) / g * g);
    double sz = (double)n * Fl(0x5dda44) + Fl(0x5cb490);
    float size = (float)sz;
    if (sz > Fl(0x5dda40)) size = 1536.0f;
    float d[3];
    d[0] = (float)((double)at[0] - eye[0]);
    d[1] = (float)((double)at[1] - eye[1]);
    d[2] = (float)((double)at[2] - eye[2]);
    float ny = d[1], nz = d[2];
    float back = (float)((double)size * D(0x5dda38) * D(0x5c8c48));
    float len = (float)Fsqrt((double)d[2] * d[2] + (double)d[1] * d[1] + (double)d[0] * d[0]);
    double nx = d[0];
    if (Fl(0x5c8598) < len) {
        float inv = (float)(Fl(0x5c85b4) / len);
        nx = (double)d[0] * inv;
        ny = (float)((double)d[1] * inv);
        nz = (float)((double)d[2] * inv);
    }
    float px = (float)((double)at[0] - nx * back);
    float py = (float)((double)at[1] - (double)ny * back);
    float pz = (float)((double)at[2] - (double)nz * back);
    int32_t gi = F<int32_t>(e, 0x11420e);
    px = (float)(Ftol(px) / gi * gi);
    pz = (float)(Ftol(pz) / gi * gi);
    py = (float)(Ftol(py) / gi * gi);
    uint8_t* lvl = r + F<int32_t>(r, 0x36f3) * 0x33;
    float sx = (float)((double)px - F<float>(lvl, 0x867));
    float sz2 = (float)((double)pz - F<float>(lvl, 0x86f) * D(0x5c8758));
    ((void*(__stdcall*)(void*, float, float, float, float))0x56e4bb)(r + 0x356f, size, size, -65535.0f, 65535.0f);
    F<float>(r, 0x3701) = px;
    F<float>(r, 0x3705) = py;
    F<float>(r, 0x3709) = pz;
    double floor = F<float>(r + F<int32_t>(r, 0x36f3) * 0x33, 0x86b);
    if (floor > D(0x5ca180)) floor = Fl(0x5dda30);
    float sy = (float)((double)py - floor);
    float sp[3] = { sx, sy, sz2 };
    if (sp[0] != G<float>(0x625450) || sp[1] != G<float>(0x62544c) || G<float>(0x625448) != size ||
        sp[2] != G<float>(0x625444) || up[0] != G<float>(0x625440) || up[1] != G<float>(0x62543c) ||
        up[2] != G<float>(0x625438)) {
        G<float>(0x625450) = sp[0];
        G<float>(0x625444) = sp[2];
        G<float>(0x62544c) = sp[1];
        G<float>(0x625448) = size;
        r[0x36f7] = 1;
        G<float>(0x625440) = up[0];
        G<float>(0x62543c) = up[1];
        G<float>(0x625438) = up[2];
    }
    float lp[3] = { px, py, pz };
    LookAt(r + 0x362f, sp, lp, up);
}
RT3_REPLACE(0x548350, Renderer_SetupViews);

void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb);

// 0x54bea0. Lighting for time of day `secs` (0..86400) and night amount `night`:
// blends the two nearest of the 144 keyframes (+0x840, 0x33 bytes each; index
// +0x36f3) into the sun / ambient terms (+0x24f0..+0x251f, fog colour bytes
// +0x2510..), darkened by the night factor (+0x25af), then sets the directional
// light (sun direction, diffuse, ambient, specular when +0x11422e), the fog colour
// +0x25a3 and the fog. Only when the device does lighting (+0x83c +0x22).
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night) {
    if (secs < 0 || secs > 0x15180 || !F<uint8_t*>(r, 0x83c)[0x22]) return;
    double n = (double)night * Fl(0x5ddb08);
    F<float>(r, 0x25af) = (float)n;
    if (n > D(0x5c8770)) F<uint32_t>(r, 0x25af) = 0x3f800000;
    else if (n < D(0x5c8608)) F<uint32_t>(r, 0x25af) = 0;
    double inv = Fl(0x5c85b4) - (double)F<float>(r, 0x25af);
    double t = (double)secs * Fl(0x5dcfb8) * Fl(0x5cece8);
    int32_t i = Ftol(t);
    int32_t j = (i + 1) % 0x90;
    F<int32_t>(r, 0x36f3) = i;
    double frac = t - (double)i;
    double w0 = Fl(0x5c85b4) - frac;
    uint8_t* a = r + i * 0x33;
    uint8_t* b = r + j * 0x33;
    auto mix = [&](uint32_t off) { return frac * F<float>(b, off) + w0 * F<float>(a, off); };
    auto night1 = [&](uint32_t off, double k) {
        F<float>(r, off) = (float)((double)F<float>(r, 0x25af) * F<float>(r, off) * k + inv * F<float>(r, off));
    };
    F<float>(r, 0x24f0) = (float)mix(0x840);
    F<float>(r, 0x24f8) = (float)mix(0x848);
    night1(0x24f0, D(0x5ddb00));
    night1(0x24f8, D(0x5c8758));
    F<float>(r, 0x24f4) = (float)mix(0x844);
    F<float>(r, 0x24fc) = (float)mix(0x84c);
    double x = (double)F<float>(r, 0x25af) * F<float>(r, 0x24f0) * D(0x5ddb00) + inv * F<float>(r, 0x24f0);
    F<float>(r, 0x24f0) = (float)x;
    double nf = F<float>(r, 0x25af);
    F<float>(r, 0x24f8) = (float)((double)F<float>(r, 0x24f8) * nf * D(0x5c8758) + inv * F<float>(r, 0x24f8));
    float l0c = (float)((double)F<float>(r, 0x2500) * nf * D(0x5c8c18));
    double y = x * D(0x5c8880) * F<float>(r, 0x25af);
    float l14 = (float)y;
    float l10 = (float)(D(0x5c8650) * y);
    float l18 = (float)(y * D(0x5c85a0));
    F<float>(r, 0x2500) = (float)mix(0x850);
    F<float>(r, 0x2504) = (float)mix(0x854);
    F<float>(r, 0x2508) = (float)mix(0x858);
    F<float>(r, 0x250c) = (float)mix(0x85c);
    F<float>(r, 0x2504) = (float)(inv * F<float>(r, 0x2504) + l18);
    F<float>(r, 0x2508) = (float)(inv * F<float>(r, 0x2508) + l14);
    F<float>(r, 0x250c) = (float)(inv * F<float>(r, 0x250c) + l10);
    F<float>(r, 0x2500) = (float)(inv * F<float>(r, 0x2500) + l0c);
    F<float>(r, 0x2513) = (float)(mix(0x863) * inv * Fl(0x5d1808) + (double)F<float>(r, 0x25af) * Fl(0x5ddaf8));
    F<float>(r, 0x2517) = (float)mix(0x867);
    F<float>(r, 0x251b) = (float)mix(0x86b);
    F<float>(r, 0x251f) = (float)mix(0x86f);
    for (int32_t k = 0; k < 3; ++k)
        r[0x2510 + k] = (uint8_t)Ftol((double)b[0x860 + k] * frac + (double)a[0x860 + k] * w0);
    r[0x2510] = (uint8_t)Ftol((double)r[0x2510] * inv + (double)l18 * Fl(0x5ca0ec));
    r[0x2511] = (uint8_t)Ftol((double)r[0x2511] * inv + (double)l14 * Fl(0x5ca0ec));
    r[0x2512] = (uint8_t)Ftol((double)r[0x2512] * inv + (double)l10 * Fl(0x5ca0ec));
    float light[0x1a] = {};
    F<uint32_t>(light, 0) = 3;
    if (hook::Global<uint8_t*>(0x6d4024)[0x11422e]) {
        float sp = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4) * D(0x5c8758));
        for (int32_t k = 0; k < 4; ++k) light[5 + k] = sp;
    }
    float diff = (float)((double)F<float>(r, 0x2603) * F<float>(r, 0x24fc));
    for (int32_t k = 0; k < 4; ++k) light[1 + k] = diff;
    F<uint32_t>(light, 0x48) = F<uint32_t>(r, 0x251f);
    float amb = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4));
    F<uint32_t>(light, 0x4c) = 0x447a0000;
    F<uint32_t>(light, 0x40) = F<uint32_t>(r, 0x2517);
    for (int32_t k = 0; k < 4; ++k) light[9 + k] = amb;
    F<float>(light, 0x44) = -F<float>(r, 0x251b);
    uint8_t* d = F<uint8_t*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, const void*))(*(uint8_t**)d + 0xb0))(d, 0, light);
    F<uint32_t>(r, 0x25a3) = (((0xffffff00u | r[0x2510]) << 8 | r[0x2511]) << 8) | r[0x2512];
    Renderer_SetFog(r, 0, 1.0f, 1.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x54bea0, Renderer_SetTimeOfDay);

} // namespace rt3
