// Sound system startup / shutdown and play gates / terrain point grid upkeep in blocks 0x53 (batch 148). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Module_RegisterCleanup();
uint8_t* __fastcall Mixer_Construct(uint8_t* m);
int32_t __fastcall Sound2D_Open(uint8_t* s);
uint8_t* __fastcall Sound2D_Construct(uint8_t* s);
int32_t __fastcall Sound3D_Open(uint8_t* s, int, const char* name);
void __fastcall Sound3D_Destruct(uint8_t* s);
void __fastcall Sound2D_Destruct(uint8_t* s);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol);
void __fastcall Sound_PauseAll(uint8_t* s);
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
void __fastcall Sound2D_StopAll(uint8_t* s);
void __fastcall Sound_StopAll(uint8_t* s);
void __fastcall Sound3D_Close(uint8_t* s);
void __fastcall Sound2D_Close(uint8_t* s);
void __fastcall Music_Close(uint8_t* m);
uint32_t __fastcall Sound2D_Play(uint8_t* s, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data);
uint32_t __fastcall Sound3D_Play(uint8_t* s, int, const char* name, float x, float y, float z, float vol, int32_t mode,
                                 int32_t paused, int32_t prio, float maxDist);
void __fastcall Grid_RefreshPoints(uint8_t* g, int, uint8_t* t);
void __fastcall Grid_AddPoint(uint8_t* g, int, uint8_t* t, int32_t x, int32_t z, uint8_t kind);
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h);

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline uint8_t*& Snd2D() { return hook::Global<uint8_t*>(0xccb9f8); }
static inline uint8_t*& Snd3D() { return hook::Global<uint8_t*>(0xccb9fc); }
static inline uint8_t*& Music() { return hook::Global<uint8_t*>(0xccba00); }
static inline void Sleep_(uint32_t ms) { Import<void(__stdcall*)(uint32_t)>(0x5c81f0)(ms); }

// Destroys and clears one of the sound singletons.
static void DestroySnd(uint8_t*& p, void(__fastcall* dtor)(uint8_t*)) {
    if (uint8_t* o = p) {
        dtor(o);
        ExeDelete(o);
    }
    p = nullptr;
}

// 0x5318f0. Starts Miles (redist data\miles\) and creates the 2D (0xccb9f8), 3D
// (0xccb9fc, provider `name`) and music (0xccba00, volume 1) systems; +4 = 1 when
// all came up (the failed ones are torn down again).
int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name) {
    Import<void(__stdcall*)(const char*)>(0x5c847c)((const char*)0x5dd240);
    Module_RegisterCleanup();
    Import<int32_t(__stdcall*)()>(0x5c83b4)();
    uint8_t* m = (uint8_t*)ExeNew(0x30);
    Snd2D() = m ? Mixer_Construct(m) : nullptr;
    if (Sound2D_Open(Snd2D()) == 1) {
        uint8_t* p = (uint8_t*)ExeNew(0x64);
        Snd3D() = p ? Sound2D_Construct(p) : nullptr;
        if (Sound3D_Open(Snd3D(), 0, name) == 1) {
            uint8_t* q = (uint8_t*)ExeNew(0x108);
            if (q) {
                F<uint32_t>(q, 0) = 0;
                F<uint32_t>(q, 0x104) = 0x3f800000;
            }
            Music() = q;
            if (Snd2D() && Snd3D() && q) F<int32_t>(snd, 4) = 1;
            return F<int32_t>(snd, 4);
        }
        DestroySnd(Snd3D(), Sound3D_Destruct);
    }
    DestroySnd(Snd2D(), Sound2D_Destruct);
    return F<int32_t>(snd, 4);
}
RT3_REPLACE(0x5318f0, Sound_Startup);

// 0x531a10. Shuts the sound system down (0xccb9f4 set meanwhile): unless
// `immediate`, fades music and effects out over steps of 0x5d12c8 / 0x5d0058 of
// their volume (16 ms apart, then 200 ms), waits for the worker (0xccb9f0), stops
// everything, releases the timer (+0x4c), closes Miles and frees the systems.
void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate) {
    hook::Global<int32_t>(0xccb9f4) = 1;
    if (F<int32_t>(snd, 4) != 1) {
        F<int32_t>(snd, 4) = 0;
        hook::Global<int32_t>(0xccb9f4) = 0;
        return;
    }
    if (!immediate) {
        float mv = F<float>(Music(), 0x104), sv = F<float>(Snd3D(), 0x58);
        float ms = (float)((double)mv * *(const float*)0x5d12c8), ss = (float)((double)sv * *(const float*)0x5d0058);
        const float zero = *(const float*)0x5c8598;
        while (zero < mv || zero < sv) {
            mv = mv > ms ? (float)((double)mv - ms) : 0.0f;
            sv = sv > ss ? (float)((double)sv - ss) : 0.0f;
            Music_SetVolume(Music(), 0, mv);
            Sound_SetEffectsVolume(Snd3D(), 0, sv);
            Sound_PauseAll(Snd3D());
            Sleep_(0x10);
        }
        Sleep_(200);
    }
    while (hook::Global<volatile int32_t>(0xccb9f0)) {}
    Music_Stop(Music(), 0, 0);
    Sound2D_StopAll(Snd2D());
    Sound_StopAll(Snd3D());
    if (F<int32_t>(snd, 0x4c) != -1) {
        Import<void(__stdcall*)(int32_t)>(0x5c83b8)(F<int32_t>(snd, 0x4c));
        Import<void(__stdcall*)(int32_t)>(0x5c8450)(F<int32_t>(snd, 0x4c));
        F<int32_t>(snd, 0x4c) = -1;
    }
    Sound3D_Close(Snd3D());
    Sound2D_Close(Snd2D());
    Import<void(__stdcall*)()>(0x5c83ac)();
    DestroySnd(Music(), Music_Close);
    DestroySnd(Snd3D(), Sound3D_Destruct);
    DestroySnd(Snd2D(), Sound2D_Destruct);
    F<int32_t>(snd, 4) = 0;
    hook::Global<int32_t>(0xccb9f4) = 0;
}
RT3_REPLACE(0x531a10, Sound_Shutdown);

// Whether sound is up (+4) and not shutting down.
static inline bool SoundUp(uint8_t* snd) { return F<int32_t>(snd, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

// 0x531e10. Plays a 2D sound when the 2D volume (+0x20) is up; else 0.
uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data) {
    if (!SoundUp(snd) || !(F<float>(Snd2D(), 0x20) > *(const float*)0x5c8598)) return 0;
    return Sound2D_Play(Snd2D(), 0, name, vol, pan, mode, paused, data);
}
RT3_REPLACE(0x531e10, Sound_Play2D);

// 0x531e50. Plays a named 3D sound when the effects volume (+0x58) is up; else 0.
uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist) {
    if (!SoundUp(snd) || !(F<float>(Snd3D(), 0x58) > *(const float*)0x5c8598) || !name[0]) return 0;
    return Sound3D_Play(Snd3D(), 0, name, x, y, z, vol, mode, paused, prio, maxDist);
}
RT3_REPLACE(0x531e50, Sound_Play3D);

// Point grid of 16x16-cell block (bx, by).
static inline uint8_t*& PointGrid(uint8_t* t, int32_t bx, int32_t by) {
    return F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * by + bx];
}

// 0x533e70. Refreshes the point grids of the blocks covering cells (x0, y0)-(x1, y1).
void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    if (F<int32_t>(t, 0x15d9) - 2 < x1) x1 = F<int32_t>(t, 0x15d9) - 2;
    if (F<int32_t>(t, 0x15d5) - 2 < y1) y1 = F<int32_t>(t, 0x15d5) - 2;
    y1 >>= 4;
    x0 >>= 4;
    x1 >>= 4;
    y0 >>= 4;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx)
            if (uint8_t* g = PointGrid(t, bx, by)) Grid_RefreshPoints(g, 0, t);
}
RT3_REPLACE(0x533e70, Terrain_RefreshPointGrids);

// 0x534160. Adds up to 6 points of `kind` at cell (x, z), creating the block's point
// grid (0x54 bytes, origin = block x 0x5c0) on demand.
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count) {
    int32_t bx = x >> 4, bz = z >> 4;
    if (!PointGrid(t, bx, bz)) {
        uint8_t* g = (uint8_t*)ExeNew(0x54);
        if (g) g = BlockGrid_Init(g, 0, (float)(bx * 0x5c0), (float)(bz * 0x5c0));
        PointGrid(t, bx, bz) = g;
    }
    int32_t lx = x % 16, lz = z % 16;
    int32_t n = count > 6 ? 6 : count;
    for (; n > 0; --n) Grid_AddPoint(PointGrid(t, bx, bz), 0, t, lx, lz, kind);
}
RT3_REPLACE(0x534160, Terrain_AddPoints);

} // namespace rt3
