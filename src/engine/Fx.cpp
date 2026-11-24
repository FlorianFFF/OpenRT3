// Fx.cpp - RT3_mp.exe 0x54c000-0x54e000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Render_BindVb2647(uint8_t* r);
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny);
int32_t __fastcall Render_BlendDefault(uint8_t* r);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
void __fastcall Render_CullNone(uint8_t* r);
int32_t __fastcall Render_LightingOff(uint8_t* r);
int32_t __fastcall Render_ZWriteOff(uint8_t* r);
void __fastcall Render_WorldIdentity(uint8_t* r);
void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex);
void* __fastcall Scene_BeginBatch(uint8_t* s, int, int32_t count);
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end);
int32_t __fastcall Render_ZWriteOn(uint8_t* r);
int32_t __fastcall Render_LightingDefault(uint8_t* r);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Scene_EndBatch(uint8_t* s, int, uint8_t draw);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage);
void __fastcall Render_GetFogBlock(uint8_t* r, int, uint8_t* dst);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
uint32_t __fastcall Terrain_Get15e5(uint8_t* r);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);
void __fastcall Render_DrawIndexed(uint8_t* r, int, int32_t indices, uint32_t minIndex, uint32_t vertices);
template <typename Fn> static inline Fn DevSlot(void* dev, uint32_t off) { return *(Fn*)(*(uint8_t**)dev + off); }
void __fastcall Render_ScreenSpaceAll(uint8_t* r);
void __fastcall Render_BindPair2647(uint8_t* r);
void* __fastcall Render_LockVb264b(uint8_t* r);
int32_t __fastcall Render_UnlockVb264b(uint8_t* r);
void __fastcall Render_FogOff(uint8_t* r);
void __fastcall Render_FogOn(uint8_t* r);
void __fastcall Channel_StopSample(uint8_t* c);
void __fastcall Channel_MoveTo(uint8_t* c, int, float x, float y, float z);
void __fastcall Channel_SetSample50(uint8_t* c, int, uint32_t v);
void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi);
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);
uint32_t __fastcall Render_AnimFrameB(uint8_t* r);
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);

// Functions of this file, in exe order.
double __fastcall Render_Benchmark(uint8_t* r);
void __fastcall Fx_UpdateIntensity(uint8_t* o);
int32_t __fastcall Fx_Intensity(uint8_t* o);
RT3_NOINLINE void __fastcall Fireworks_Update(uint8_t* s, int, uint8_t* sounds);
void __fastcall Rain_Update(uint8_t* s, int, uint8_t* w);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void DevRenderState(uint8_t* r, uint32_t state, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)dev + 0xc8))(dev, state, value);
}

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

static inline double Fsqrt(double v) {
    __asm {
        fld v
        fsqrt
        fstp v
    }
    return v;
}

// m * sin(a) / m * cos(a) with the 64-bit FSIN/FCOS result, as the x87 code does.
static inline double Fw_MulSin(double m, float a) {
    double r;
    __asm {
        fld a
        fsin
        fmul m
        fstp r
    }
    return r;
}

static inline double Fw_MulCos(double m, float a) {
    double r;
    __asm {
        fld a
        fcos
        fmul m
        fstp r
    }
    return r;
}

static inline double Fw_Cos(float a) {
    double r;
    __asm {
        fld a
        fcos
        fstp r
    }
    return r;
}

static inline uint32_t Fw_Rand() {
    uint32_t s = hook::Global<uint32_t>(0x624324) * 0x19660d + 0x3c6ef35f;
    hook::Global<uint32_t>(0x624324) = s;
    return s;
}

static inline uint8_t* Fw_Channel(uint8_t* sounds, int32_t idx) { return idx >= 0 ? F<uint8_t*>(sounds, 0x40c + idx * 4) : nullptr; }

static inline void Fw_Vertex(float* v, float x, float y, float z, uint32_t col, uint32_t u, uint32_t w) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    *(uint32_t*)&v[3] = col;
    *(uint32_t*)&v[4] = u;
    *(uint32_t*)&v[5] = w;
}

static inline uint32_t Rain_Grey(int32_t a, int32_t c) { return (((uint32_t)a << 8 | c) << 8 | c) << 8 | c; }

static inline int32_t Rain_Clamp(int32_t v, int32_t lo, int32_t hi) { return v > hi ? hi : v < lo ? lo : v; }

// 0x54c420. Fill-rate benchmark for renderer r: draws a 40 x 40 grid of
// screen quads (texture 0x5dda48, z off) twice per frame for a second of
// timeGetTime and returns the pixel rate (triangles x 0x5dd14c per ms x
// 0x5c9b68). Like the original it first draws 65000 rand() values it never
// uses; restores lighting (+0x83c +0x22), z and the render target.
double __fastcall Render_Benchmark(uint8_t* r) {
    for (int i = 0; i < 0xfde8; ++i) ((int(__cdecl*)())0x5a29ab)();
    typedef int32_t(__stdcall * Clear)(void*, uint32_t, const void*, uint32_t, uint32_t, float, uint32_t);
    typedef int32_t(__stdcall * Plain)(void*);
    typedef int32_t(__stdcall * Present)(void*, const void*, const void*, void*, const void*);
    typedef int32_t(__stdcall * Draw)(void*, uint32_t, uint32_t, uint32_t);
    F<int32_t>(r, 0x370d) = 0;
    F<int32_t>(r, 0x3711) = 0;
    void* dev = F<void*>(r, 0x25df);
    DevSlot<Clear>(dev, 0x90)(dev, 0, nullptr, 2, 0, 1.0f, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Plain>(dev, 0x88)(dev);
    dev = F<void*>(r, 0x25df);
    DevSlot<Clear>(dev, 0x90)(dev, 0, nullptr, 3, 0, 1.0f, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Plain>(dev, 0x8c)(dev);
    dev = F<void*>(r, 0x25df);
    DevSlot<Present>(dev, 0x3c)(dev, nullptr, nullptr, nullptr, nullptr);
    uint8_t* tex = TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5dda48, 0, 0, 0x7f);
    uint32_t tris = 0;
    Render_ScreenSpaceAll(r);
    if (r[0x254a]) {
        DevRenderState(r, 0x16, 1);
        r[0x254a] = 0;
    }
    DevRenderState(r, 0x89, 0);
    void* stream = F<void*>(r, 0x2647);
    if (!stream) {
        F<void*>(r, 0x2637) = nullptr;
    } else {
        if (F<void*>(r, 0x2637) != stream) {
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x142);
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(dev, 0x14c)(dev, 0, stream, 0x18);
            F<void*>(r, 0x2637) = stream;
        }
        F<int32_t>(r, 0x263b) = 0;
    }
    DevRenderState(r, 7, 0);
    void* vb = F<void*>(r, 0x2647);
    uint32_t* v = nullptr;
    DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t**, uint32_t)>(vb, 0x2c)(vb, 0, 0, &v, 0x2800);
    ++hook::Global<int32_t>(0xccd824);
    double c = Fl(0x5d0100);
    for (int32_t row = 0; row < 0x28; ++row) {
        double y0 = row * c, y1 = c + y0;
        for (int32_t col = 0; col < 0x28; ++col) {
            double x0 = col * c, x1 = c + x0;
            const double q[6][2] = { { x0, y0 }, { x1, y0 }, { x0, y1 }, { x1, y0 }, { x1, y1 }, { x0, y1 } };
            for (int k = 0; k < 6; ++k, v += 6) {
                float x = (float)q[k][0], y = (float)q[k][1];
                v[0] = *(uint32_t*)&x;
                v[1] = *(uint32_t*)&y;
                v[2] = 0;
                v[3] = 0xff000000;
                v[4] = *(uint32_t*)&x;
                v[5] = *(uint32_t*)&y;
            }
        }
    }
    --hook::Global<int32_t>(0xccd824);
    vb = F<void*>(r, 0x2647);
    DevSlot<Plain>(vb, 0x30)(vb);
    typedef uint32_t(__stdcall * Ticks)();
    uint32_t start = (*(Ticks*)0x5c8068)();
    uint32_t now = (*(Ticks*)0x5c8068)();
    while (now < start + 1000) {
        F<int32_t>(r, 0x370d) = 0;
        F<int32_t>(r, 0x3711) = 0;
        dev = F<void*>(r, 0x25df);
        DevSlot<Clear>(dev, 0x90)(dev, 0, nullptr, 2, 0, 1.0f, 0);
        dev = F<void*>(r, 0x25df);
        DevSlot<Plain>(dev, 0x88)(dev);
        dev = F<void*>(r, 0x25df);
        DevSlot<Clear>(dev, 0x90)(dev, 0, nullptr, 2, 0, 1.0f, 0);
        tris += 0xc80;
        Renderer_SetTexture(r, 0, 0, tex);
        dev = F<void*>(r, 0x25df);
        DevSlot<Draw>(dev, 0x118)(dev, 4, 0, 0xc80);
        if (F<int32_t>(r, 0x258f) > 0) {
            if (F<void*>(F<uint8_t*>(r, 0x2593), 0)) {
                ++F<int32_t>(r, 0x370d);
                dev = F<void*>(r, 0x25df);
                DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*)>(dev, 0xf4)(dev, 0, nullptr);
            }
            F<void*>(F<uint8_t*>(r, 0x2593), 0) = nullptr;
        }
        dev = F<void*>(r, 0x25df);
        tris += 0xc80;
        DevSlot<Draw>(dev, 0x118)(dev, 4, 0, 0xc80);
        dev = F<void*>(r, 0x25df);
        DevSlot<Plain>(dev, 0x8c)(dev);
        dev = F<void*>(r, 0x25df);
        DevSlot<Present>(dev, 0x3c)(dev, nullptr, nullptr, nullptr, nullptr);
        now = (*(Ticks*)0x5c8068)();
    }
    float rate = (float)(((double)tris * Fl(0x5dd14c)) / ((double)(now - start) * Fl(0x5c9b68)));
    if (!r[0x254c]) {
        dev = F<void*>(r, 0x25df);
        DevSlot<int32_t(__stdcall*)(void*, void*, void*)>(dev, 0x7c)(dev, F<void*>(r, 0x2607), F<void*>(r, 0x260b));
    }
    r[0x254c] = 1;
    DevRenderState(r, 0x89, F<uint8_t*>(r, 0x83c)[0x22]);
    DevRenderState(r, 7, 1);
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, tex);
    return rate;
}
RT3_REPLACE(0x54c420, Render_Benchmark);

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

// 0x54c900. Runs and draws view s's fireworks (16 rockets of 0x31a bytes
// from +0x4ad: fuse +0, bursts left +4, size +5, sound channel +6 into
// sounds +0x40c, life +0xa, velocity +0xe, 64 trail points +0x1a) when
// the effect intensity (+0x19) is 0xe0 or more or a show is requested
// (+0x22b57, also clearing the sky): launches new rockets at random times,
// steps their flight every 12 ms (drag, jitter, split into smaller bursts
// high up), and draws each one's flash and trail. Uses the game random
// generator 0x624324.
RT3_NOINLINE void __fastcall Fireworks_Update(uint8_t* s, int, uint8_t* sounds) {
    if (F<int32_t>(s, 0x19) < 0xe0 && !F<int32_t>(s, 0x22b57)) return;
    Render_FogOff(F<uint8_t*>(s, 0x2d));
    Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    float intensity = (float)(((double)F<int32_t>(s, 0x19) - Fl(0x5ddb38)) * Fl(0x5dd3d4));
    typedef uint32_t(__stdcall * Ticks)();
    Ticks ticks = *(Ticks*)0x5c8068;
    if (!(hook::Global<uint32_t>(0xd93834) & 1)) {
        hook::Global<uint32_t>(0xd93834) |= 1;
        hook::Global<uint32_t>(0xd93830) = ticks();
    }
    if (!(hook::Global<uint8_t>(0xd93834) & 2)) {
        hook::Global<uint32_t>(0xd93834) |= 2;
        hook::Global<uint32_t>(0xd9382c) = ticks();
    }
    int32_t elapsed = (int32_t)(ticks() - hook::Global<uint32_t>(0xd93830));
    int32_t request = F<int32_t>(s, 0x22b57);
    int32_t since = (int32_t)(ticks() + (1 - hook::Global<uint32_t>(0xd9382c)));
    if (!s[0x3692]) {
        bool launch = false;
        if (!F<int32_t>(s, 0x22b57)) {
            uint32_t seed = Fw_Rand();
            int32_t r = (int32_t)(seed & 0x7fff) % since;
            double th = ((double)Fl(0x5dd1e8) - intensity) * D(0x5ddb30);
            if ((double)r > th) launch = true;
        } else {
            for (int32_t k = 0; k < 0x10; ++k) {
                F<int32_t>(s, 0x4ad + k * 0x31a) = 0;
                F<int32_t>(s, 0x4b7 + k * 0x31a) = 0;
            }
            launch = true;
        }
        if (launch) {
            F<int32_t>(s, 0x22b57) = 0;
            hook::Global<uint32_t>(0xd9382c) = ticks();
            for (int32_t k = 0; k < 0x10; ++k) {
                uint8_t* r = s + 0x4ad + k * 0x31a;
                if (F<int32_t>(r, 0xa)) continue;
                F<int32_t>(r, 6) = hook::Global<int32_t>(0xd93828) + 1;
                hook::Global<int32_t>(0xd93828) = (hook::Global<int32_t>(0xd93828) + 1) % 6;
                r[5] = 0xff;
                F<int32_t>(r, 0xa) = 0x41;
                int32_t v = (int32_t)(Fw_Rand() & 0x7fff) & 0xf;
                r[4] = (uint8_t)Ftol((double)v * intensity * Fl(0x5c9704) + D(0x5c8770));
                break;
            }
        }
    }
    for (int32_t k = 0; k < 0x10; ++k) {
        uint8_t* r = s + 0x4ad + k * 0x31a;
        if (!s[0x3692] && (elapsed > 0xc || F<int32_t>(s, 0x22b57))) {
            int32_t steps = elapsed / 12 + 1;
            hook::Global<uint32_t>(0xd93830) = ticks();
            for (; steps; --steps) {
                int32_t life = F<int32_t>(r, 0xa);
                if (life > 0x3f) {
                    F<int32_t>(r, 0xa) = 0x3f;
                    uint32_t s1 = hook::Global<uint32_t>(0x624324) * 0x19660d + 0x3c6ef35f;
                    int32_t a = (int32_t)(s1 & 0x7fff) % 0x31df;
                    hook::Global<uint32_t>(0x624324) = s1;
                    uint32_t s2 = s1 * 0x19660d + 0x3c6ef35f;
                    int32_t b = (int32_t)(s2 & 0x7fff) % 0x31df;
                    float px = (float)((double)F<float>(s, 0x369c) - a + Fl(0x5ddb2c));
                    hook::Global<uint32_t>(0x624324) = s2;
                    float pz = (float)((double)b + F<float>(s, 0x36a4) - Fl(0x5ddb2c));
                    if (request) {
                        request = 0;
                        float head = F<float>(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0x10);
                        head = (float)(Rand(-0x32, 0x32) * D(0x5c85b8) * D(0x5c88e8) + head);
                        int32_t d = Rand(0x14, 0x3c) * 0x5c;
                        px = (float)((double)F<float>(s, 0x369c) - Fw_MulSin(d, head));
                        pz = (float)((double)F<float>(s, 0x36a4) - Fw_MulCos(d, head));
                    }
                    uint32_t s3 = 0xd1ccf6e9 - hook::Global<uint32_t>(0x624324) * 0x50b6f56b;
                    hook::Global<uint32_t>(0x624324) = s3;
                    uint32_t v = s3 & 0x7fff;
                    float vx = (float)(int32_t)((v & 0x7f) - 0x40);
                    float vy = (float)(int32_t)(-0x40 - (int32_t)(v & 0x1f));
                    F<float>(r, 0x12) = vy;
                    F<float>(r, 0xe) = vx;
                    F<float>(r, 0x16) = vx;
                    if (uint8_t* ch = Fw_Channel(sounds, F<int32_t>(r, 6))) {
                        Channel_StopSample(ch);
                        ch = Fw_Channel(sounds, F<int32_t>(r, 6));
                        F<int32_t>(ch, 0x130) = 1;
                        Channel_MoveTo(ch, 0, px, F<float>(s, 0x36a0), pz);
                        float dx = (float)((double)F<float>(s, 0x369c) - px);
                        double dy = (double)F<float>(s, 0x36a0) - Fl(0x5ca190);
                        float camz = F<float>(s, 0x36a4);
                        double dz = (double)camz - pz;
                        double dist = Fsqrt(dz * dz + dy * dy + (double)dx * dx);
                        Channel_SetSample50(Fw_Channel(sounds, F<int32_t>(r, 6)), 0, (uint32_t)Ftol(dist * D(0x5c88e8)));
                        Channel_SetRange(Fw_Channel(sounds, F<int32_t>(r, 6)), 0, 0x453b8000, 0x47fa0000);
                        Channel_SetRate(Fw_Channel(sounds, F<int32_t>(r, 6)), 0, (int32_t)(Fw_Rand() & 0x3fff) + 16000);
                        Channel_SetLoop(Fw_Channel(sounds, F<int32_t>(r, 6)), 0, 0);
                    }
                    F<int32_t>(r, 0) = (int32_t)(Fw_Rand() & 0xf) + 0x20;
                    F<float>(r, 0x30e) = px;
                    F<uint32_t>(r, 0x312) = 0x453b8000;
                    F<float>(r, 0x316) = pz;
                    continue;
                }
                if (life <= 0) continue;
                --life;
                F<int32_t>(r, 0xa) = life;
                double t = (double)life * Fl(0x5ddb28);
                double sq = t * t;
                float ox = F<float>(r, 0xe), oy = F<float>(r, 0x12), oz = F<float>(r, 0x16);
                double spd = Fsqrt((double)ox * ox + (double)oy * oy + (double)oz * oz);
                if (spd > Fl(0x5ddb24)) {
                    F<float>(r, 0xe) = (float)(F<float>(r, 0xe) * D(0x5c8c20));
                    F<float>(r, 0x16) = (float)(F<float>(r, 0x16) * D(0x5c8c20));
                }
                double nvx = F<float>(r, 0xe), nvy = F<float>(r, 0x12);
                float nvz = F<float>(r, 0x16);
                double len = Fsqrt((double)nvz * nvz + nvy * nvy + nvx * nvx);
                if (Fl(0x5c8598) < len) {
                    double inv = Fl(0x5c85b4) / len;
                    nvx = inv * nvx;
                    nvy = inv * nvy;
                    nvz = (float)(inv * nvz);
                }
                uint32_t r1 = Fw_Rand();
                float jx = (float)((double)((int32_t)(r1 & 0x3ff) - 0x200) + nvx);
                uint32_t r2 = Fw_Rand();
                double jy = (double)((int32_t)(r2 & 0x1ff) - 0x100) + nvy;
                uint32_t r3 = Fw_Rand();
                double jz = (double)((int32_t)(r3 & 0x3ff) - 0x200) + nvz;
                float mx = (float)(jx * sq);
                float my = (float)(jy * sq);
                double mz = jz * sq;
                float* e = (float*)(r + 0x1a + life * 0xc);
                double X = (double)ox + e[3];
                float Y = (float)((double)oy + e[4]);
                float Z = (float)((double)oz + e[5]);
                float nz = (float)(Z - mz);
                e[0] = (float)(X - mx);
                e[2] = nz;
                e[1] = (float)((double)Y - my);
                float ex = e[0], ey = e[1], ez = e[2];
                if (uint8_t* ch = Fw_Channel(sounds, F<int32_t>(r, 6))) Channel_MoveTo(ch, 0, ex, F<float>(s, 0x36a0), ez);
                const float* cur = (const float*)(r + 0x1a + F<int32_t>(r, 0xa) * 0xc);
                int32_t h = Ftol((double)cur[1] - Fl(0x5ca190));
                int32_t lim = ((h < 0 ? -h : h) >> 4) - 8;
                uint32_t r4 = Fw_Rand();
                if (!((int32_t)(r4 & 0xf) < lim && F<int32_t>(r, 0xa) > 0x10 && r[5] > 0x2e && ey > Fl(0x5ddb20) && r[4])) continue;
                uint8_t* c = nullptr;
                for (int32_t m = 0; m < 0x10; ++m)
                    if (!F<int32_t>(s, 0x4b7 + m * 0x31a)) {
                        c = s + 0x4ad + m * 0x31a;
                        break;
                    }
                if (!c) continue;
                F<int32_t>(c, 0xa) = 0x3f;
                F<int32_t>(c, 6) = -1;
                F<float>(c, 0x30e) = ex;
                F<float>(c, 0x316) = ez;
                F<float>(c, 0x312) = ey;
                c[4] = 0;
                c[5] = (uint8_t)(r[5] >> 2);
                F<int32_t>(c, 0) = F<int32_t>(r, 0);
                float ang = (float)((double)(int32_t)(Fw_Rand() & 0xff) * D(0x5dd8f8) * D(0x5c8c08));
                float cvx = F<float>(c, 0xe), cvy = F<float>(c, 0x12), cvz = F<float>(c, 0x16);
                float clen = (float)Fsqrt((double)cvx * cvx + (double)cvy * cvy + (double)cvz * cvz);
                F<float>(c, 0xe) = (float)Fw_MulSin(clen, ang);
                double cs = Fw_Cos(ang);
                F<float>(c, 0x16) = (float)(clen * cs);
                F<float>(c, 0x12) = (float)(F<float>(r, 0x12) * D(0x5c9ba0));
                if (!(ey > Fl(0x5ca198))) continue;
                if (!((int32_t)(Fw_Rand() & 0xf) < (lim >> 1))) continue;
                c[5] = (uint8_t)(r[5] >> 1);
                c[4] = (uint8_t)(r[4] - 1);
                // x uses the unrounded speed still on the x87 stack, z the stored float.
                double m2d = ((double)(int32_t)(Fw_Rand() & 0xff) * D(0x5ddb18) + D(0x5c8888)) * clen;
                float m2 = (float)m2d;
                F<float>(c, 0xe) = (float)Fw_MulSin(m2d, ang);
                F<float>(c, 0x16) = (float)(m2 * cs);
                F<float>(c, 0x12) = (float)(F<float>(r, 0x12) * D(0x5c86b8));
                --r[4];
            }
        }
        int32_t life = F<int32_t>(r, 0xa);
        if (!((life > 0 || F<int32_t>(r, 0) > 0) && life < 0x3f)) continue;
        --F<int32_t>(r, 0);
        bool vis = true;
        if (life < 0x20) vis = (ticks() & 1) != 0;
        float half = (float)(((int32_t)(r[5] * r[5]) >> 4) + (0x200 - F<int32_t>(r, 0)) * 4);
        if (!vis) continue;
        float* v = (float*)Scene_BeginBatch(s, 0, 0x2000);
        int32_t a = ((int32_t)r[4] * 12 - F<int32_t>(r, 0xa)) * 2 + 0x57;
        if (s[0x3692]) a >>= 4;
        if (a < 0) a = 0;
        double yy = Fl(0x5ca190) - (Fl(0x5ca190) - (double)F<float>(r, 0x312)) * Fl(0x5c8d48);
        float px = F<float>(r, 0x30e), pz = F<float>(r, 0x316);
        uint32_t col = (uint32_t)a << 24 | 0xffefff;
        float x0 = (float)((double)px - half), x1 = (float)((double)px + half);
        float z0 = (float)((double)pz - half), z1 = (float)((double)pz + half);
        float fy = (float)yy;
        const uint32_t lo = 0x3c23d70a, hi = 0x3f7d70a4;
        Fw_Vertex(v + 0, x0, fy, z0, col, lo, lo);
        Fw_Vertex(v + 6, x1, fy, z0, col, hi, lo);
        Fw_Vertex(v + 12, x1, fy, z1, col, hi, hi);
        Fw_Vertex(v + 18, x0, fy, z0, col, lo, lo);
        Fw_Vertex(v + 24, x1, fy, z1, col, hi, hi);
        Fw_Vertex(v + 30, x0, fy, z1, col, lo, hi);
        v += 36;
        F<int32_t>(s, 0x71) += 6;
        int32_t quadEnd = F<int32_t>(s, 0x71);
        int32_t fadeBase = (int32_t)r[5] - 0xff;
        int32_t fade = 0x2e0;
        const float w = *(const float*)0x5ddb0c;
        for (int32_t i = 0x3e; i >= F<int32_t>(r, 0xa); --i, fade -= 0x10) {
            const float* e0 = (const float*)(r + 0x1a + i * 0xc);
            const float* e1 = e0 + 3;
            float y0 = e0[1], z0e = e0[2], y1 = e1[1], z1e = e1[2];
            int32_t a0 = fadeBase - Ftol(((double)Fl(0x5ca190) - y0) * Fl(0x5ddb10));
            int32_t a1 = fadeBase - Ftol(((double)Fl(0x5ca190) - y1) * Fl(0x5ddb10));
            if (a0 > 0xff) a0 = 0xff;
            if (a1 > 0xff) a1 = 0xff;
            if (fade < 0) {
                a0 = a0 + fade - 0x10;
                a1 += fade;
            }
            if (a1 < 0) a1 = 0;
            if (a0 < 0) a0 = 0;
            if (s[0x3692]) {
                a0 >>= 6;
                a1 >>= 6;
            }
            uint32_t c0 = (uint32_t)a0 << 24 | 0xffefff, c1 = (uint32_t)a1 << 24 | 0xffefff;
            float e0x = e0[0], e1x = e1[0];
            float x0m = (float)((double)e0x - w), x0p = (float)((double)e0x + w);
            float x1p = (float)((double)e1x + w), x1m = (float)((double)e1x - w);
            float z0m = (float)((double)z0e - w), z0p = (float)((double)z0e + w);
            float z1p = (float)((double)z1e + w), z1m = (float)((double)z1e - w);
            const uint32_t one = 0x3f800000;
            Fw_Vertex(v + 0, x0m, y0, z0e, c0, 0, 0);
            Fw_Vertex(v + 6, x0p, y0, z0e, c0, one, 0);
            Fw_Vertex(v + 12, x1p, y1, z1e, c1, one, one);
            Fw_Vertex(v + 18, x0m, y0, z0e, c0, 0, 0);
            Fw_Vertex(v + 24, x1p, y1, z1e, c1, one, one);
            Fw_Vertex(v + 30, x1m, y1, z1e, c1, 0, one);
            Fw_Vertex(v + 36, e0x, y0, z0m, c0, 0, 0);
            Fw_Vertex(v + 42, e0x, y0, z0p, c0, one, 0);
            Fw_Vertex(v + 48, e1x, y1, z1p, c1, one, one);
            Fw_Vertex(v + 54, e0x, y0, z0m, c0, 0, 0);
            Fw_Vertex(v + 60, e1x, y1, z1p, c1, one, one);
            Fw_Vertex(v + 66, e1x, y1, z1m, c1, 0, one);
            v += 72;
            F<int32_t>(s, 0x71) += 12;
            s[0x3747] = 1;
        }
        Scene_EndBatch(s, 0, 0);
        Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(s, 0x3655));
        Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), quadEnd);
        F<int32_t>(s, 0x75) = quadEnd;
        if (F<int32_t>(s, 0x71) > quadEnd) {
            Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 0, -1);
            Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(s, 0x3651));
            Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
            Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
        }
    }
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
    Render_FogOn(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x54c900, Fireworks_Update);

// 0x54d810. Rain of view s over world w (sound channels from +0x40c) while
// the weather intensity (+0x19) is 0x80 or more: 2048 falling drops (0xd8d828,
// seeded once from the game LCG, respawned high with the CRT rand) drawn as
// streaks around the camera, a 6x6 splash grid of ground ripples around the
// view cell (+0x3682, +0x3686), the fireworks pass, and the rain sound at the
// camera. Below 0x80 the rain and the rocket sounds are stopped.
void __fastcall Rain_Update(uint8_t* s, int, uint8_t* w) {
    uint8_t* ch = F<uint8_t*>(w, 0x40c);
    if (F<int32_t>(s, 0x19) < 0x80) {
        Channel_SetLoop(ch, 0, 1);
        for (int32_t k = 0; k < 0x10; ++k) {
            int32_t& idx = F<int32_t>(s, 0x4b3 + k * 0x31a);
            if (idx < 0) continue;
            if (uint8_t* c = F<uint8_t*>(w, 0x40c + idx * 4)) Channel_StopSample(c);
            idx = -1;
        }
        return;
    }
    float level = (float)(((double)F<int32_t>(s, 0x19) - Fl(0x5ddb68)) * Fl(0x5ddb28));
    float* drops = (float*)0xd8d828;
    if (!hook::Global<uint8_t>(0xd8d820)) {
        hook::Global<uint8_t>(0xd8d820) = 1;
        uint32_t seed = hook::Global<uint32_t>(0x624324);
        for (float* d = drops; d < (float*)0xd93828; d += 3) {
            seed = seed * 0x19660d + 0x3c6ef35f;
            d[0] = (float)((int32_t)(seed & 0xff) - 0x7f);
            seed = seed * 0x19660d + 0x3c6ef35f;
            d[2] = (float)((int32_t)(seed & 0xff) - 0x7f);
            seed = seed * 0x19660d + 0x3c6ef35f;
            d[1] = (float)(int32_t)((seed & 0x1f) + 0x1f);
        }
        hook::Global<uint32_t>(0x624324) = seed;
    }
    uint8_t* r = F<uint8_t*>(s, 0x2d);
    Render_BindVb2647(r);
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    if (!s[0x3692]) {
        Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, nullptr);
        float* v = (float*)Scene_BeginBatch(s, 0, 0x3424);
        typedef uint32_t(__stdcall * Ticks)();
        Ticks ticks = *(Ticks*)0x5c8068;
        if (!(hook::Global<uint8_t>(0xd9383c) & 1)) {
            hook::Global<uint32_t>(0xd9383c) |= 1;
            hook::Global<uint32_t>(0xd93838) = ticks();
        }
        int32_t elapsed = (int32_t)(ticks() - hook::Global<uint32_t>(0xd93838));
        double fe = (double)elapsed;
        if (elapsed < 0) fe += Fl(0x5c872c);
        float dt = (float)(fe * Fl(0x5cb5c0));
        hook::Global<uint32_t>(0xd93838) = ticks();
        double t = dt;
        if (dt > Fl(0x5c8554)) t = Fl(0x5c8554);
        float vx = (float)(D(0x5c8578) * t);
        float vy = (float)(D(0x5ddb60) * t);
        float vz = (float)(t * D(0x5ca180));
        uint8_t fog[0x34];
        Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
        int32_t c = Rain_Clamp(Ftol((double)F<float>(fog, 0) * Fl(0x5ca0ec)), 0x30, 0xff);
        uint32_t tail = Rain_Grey(Ftol((double)level * Fl(0x5ddb58)), c);
        uint32_t head = Rain_Grey(Ftol((double)level * Fl(0x5ddb54)), c);
        float n = (float)((double)level * Fl(0x5cf0e8));
        if (Fl(0x5c8598) < n) {
            const double d = D(0x5cecd0);
            float* p = drops;
            int32_t i = 0;
            do {
                F<uint32_t>(v, 0xc) = tail;
                double Y = (double)p[1] + F<float>(s, 0x36a0);
                double Z = (double)F<float>(s, 0x36a4) + p[2];
                double X = (double)F<float>(s, 0x369c) + p[0];
                v[0] = (float)(X - vx);
                v[1] = (float)(Y - vy);
                v[2] = (float)(Z - vz);
                double x1 = (double)vx + p[0];
                p[0] = (float)x1;
                double y1 = (double)vy + p[1];
                p[1] = (float)y1;
                double z1 = (double)vz + p[2];
                float z1f = (float)z1;
                p[2] = z1f;
                float X1 = (float)(x1 + F<float>(s, 0x369c));
                double Y1 = y1 + F<float>(s, 0x36a0);
                double Z1 = (double)z1f + F<float>(s, 0x36a4);
                F<uint32_t>(v, 0x24) = head;
                v[6] = (float)((double)X1 - d);
                v[7] = (float)Y1;
                v[8] = (float)Z1;
                F<uint32_t>(v, 0x3c) = head;
                v[12] = (float)((double)X1 + d);
                v[13] = (float)Y1;
                v[14] = (float)Z1;
                F<uint32_t>(v, 0x54) = tail;
                v[18] = (float)((double)X1 - vx);
                v[19] = (float)(Y1 - vy);
                v[20] = (float)(Z1 - vz);
                double x2 = (double)vx + p[0];
                p[0] = (float)x2;
                float y2f = (float)((double)vy + p[1]);
                p[1] = y2f;
                float z2f = (float)((double)vz + p[2]);
                p[2] = z2f;
                double X2 = x2 + F<float>(s, 0x369c);
                float Y2 = (float)((double)y2f + F<float>(s, 0x36a0));
                double Z2 = (double)z2f + F<float>(s, 0x36a4);
                F<uint32_t>(v, 0x6c) = head;
                v[24] = (float)X2;
                v[25] = Y2;
                v[26] = (float)(Z2 - d);
                F<uint32_t>(v, 0x84) = head;
                v[31] = Y2;
                v[30] = (float)X2;
                v[32] = (float)(Z2 + d);
                v += 36;
                if (p[1] < Fl(0x5ddb50)) {
                    int (*crand)() = (int (*)())0x5a29ab;
                    p[0] = (float)((crand() & 0xff) - 0x7f);
                    p[2] = (float)((crand() & 0xff) - 0x7f);
                    p[1] = (float)((crand() & 0x1f) + 0x1f);
                }
                F<int32_t>(s, 0x71) += 6;
                p += 3;
            } while ((double)++i < n);
        }
        int32_t end = F<int32_t>(s, 0x71);
        int32_t k = Ftol((double)level * Fl(0x5ddb4c));
        int32_t a = Ftol((double)k - F<float>(s, 0x368e) * D(0x5c8628));
        if (a < 0) {
            Scene_EndBatch(s, 0, 0);
            Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), end);
        } else {
            if (a > 0xff) a = 0xff;
            int32_t rows = Terrain_Rows(w) + 1;
            int32_t cols = Terrain_Cols(w);
            const float* xs = (const float*)(uintptr_t)Terrain_Get15e5(w);
            const float* hs = *(const float**)Terrain_HeightsSlot(w);
            int32_t x0 = F<int32_t>(s, 0x3682) - 3, y0 = F<int32_t>(s, 0x3686) - 3;
            int32_t x1 = F<int32_t>(s, 0x3682) + 3, y1 = F<int32_t>(s, 0x3686) + 3;
            typedef uint32_t(__stdcall * Ticks)();
            float phase = (float)((double)(int32_t)((*(Ticks*)0x5c8068)() & 0xffff) * Fl(0x5ddae4));
            int32_t c2 = c * 2 > 0xff ? 0xff : c * 2;
            uint32_t col = Rain_Grey(a, c2);
            for (int32_t y = y0; y < y1; ++y) {
                float fy = y < 0 ? -xs[-y] : xs[y];
                float wv = (float)((double)Fl(0x5ddb48) * fy + phase);
                for (int32_t x = x0; x < x1; ++x) {
                    float fx = x < 0 ? -xs[-x] : xs[x];
                    double wx = (double)Fl(0x5ddb48) * fx + phase;
                    double h = (x > 0 && x < rows && y > 0 && y < cols) ? (double)hs[y * rows + x] + D(0x5c8628) : (double)Fl(0x5c8d48);
                    v[0] = fx;
                    F<float>(v, 0x14) = wv;
                    F<uint32_t>(v, 0xc) = col;
                    v[1] = (float)h;
                    v[2] = fy;
                    v[4] = (float)wx;
                    v += 6;
                    ++F<int32_t>(s, 0x71);
                }
            }
            Scene_EndBatch(s, 0, 0);
            Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), end);
            F<int32_t>(s, 0x75) = end;
            uint16_t* ib = (uint16_t*)Render_LockVb264b(F<uint8_t*>(s, 0x2d));
            static const uint8_t strip[30] = { 0, 1, 7, 0, 7, 6, 1, 2, 8, 1, 8, 7, 2, 3, 9, 2, 9, 8, 3, 4, 10, 3, 10, 9, 4, 5, 11, 4, 11, 10 };
            for (int32_t row = 0; row < 0x1e; row += 6) {
                uint16_t b = (uint16_t)(row + F<int32_t>(s, 0x75));
                for (int32_t i = 0; i < 30; ++i) *ib++ = (uint16_t)(b + strip[i]);
            }
            Render_UnlockVb264b(F<uint8_t*>(s, 0x2d));
            uint32_t tex = Render_AnimFrameB(w);
            Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, (uint8_t*)(uintptr_t)tex);
            Render_BlendDefault(F<uint8_t*>(s, 0x2d));
            Render_BindPair2647(F<uint8_t*>(s, 0x2d));
            Render_DrawIndexed(F<uint8_t*>(s, 0x2d), 0, 0x96, F<int32_t>(s, 0x75), 0);
        }
    }
    Fireworks_Update(s, 0, w);
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    if (s[0x3692]) return;
    Channel_MoveTo(ch, 0, F<float>(s, 0x369c), F<float>(s, 0x36a0), F<float>(s, 0x36a4));
    Channel_SetLoop(ch, 0, 0);
    F<int32_t>(ch, 0x130) = -1;
    float vol = (float)((double)level * D(0x5ddb40));
    Channel_SetVolume(ch, 0, *(uint32_t*)&vol);
}
RT3_REPLACE(0x54d810, Rain_Update);

}  // namespace rt3
