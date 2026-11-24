// Emitter.cpp - RT3_mp.exe 0x554000-0x556000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
void __fastcall Widget_RemoveListNode(uint8_t* w, int, PtrListNode* n);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Emitter_SpawnParticle(uint8_t* e);
int32_t __fastcall Channel_Check(uint8_t* c);
void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s);

uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist);

struct PtrListNode;
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count);
PtrListNode* __fastcall Terrain_Add1679(uint8_t* r, int, void* v, int32_t x, int32_t y);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
uint32_t __fastcall Channel_Init(uint8_t* c);
int32_t ClockMs();
void** __fastcall SList_Unlink(void** l, int, void** prev, void** node);
float* __fastcall Vec3_Rotate(const float* v, int, float* out, const float* m);
uint32_t __fastcall Particle_Color(uint8_t* e, int, uint8_t* p, int32_t mode);

void __fastcall Emitter_Start(uint8_t* e, int, int32_t count, const uint32_t* from, const uint32_t* to, uint32_t start,
                              float life);

int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
void* __fastcall SList_PopFront(void** l);
void __fastcall SList_PushFront(void** l, int, void** node);
int32_t __fastcall Emitter_Update(uint8_t* e, int, int32_t dt);
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
void ReleaseResourceD9399c();

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Texture_SetD93990(const char* name);
void __fastcall Obj_Set8u(uint8_t* o, int, uint32_t v);
void __fastcall Channel_Detach(uint8_t* c);
uint32_t __fastcall Channel_Init(uint8_t* c);
void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s);
void __fastcall Channel_StopSample(uint8_t* c);
int32_t __fastcall Channel_Check(uint8_t* c);
RT3_NOINLINE uint8_t __fastcall Channel_Playing(uint8_t* c);
void __fastcall Channel_SetName(uint8_t* c, int, const char* name);
RT3_NOINLINE void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi);
void __fastcall Channel_SetSample50(uint8_t* c, int, uint32_t v);
RT3_NOINLINE void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);
RT3_NOINLINE void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count);
void __fastcall Channel_MoveTo(uint8_t* c, int, float x, float y, float z);
uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6, int32_t a7, float vol);
RT3_NOINLINE void __fastcall Nop5548c0(void*, int, int32_t, int32_t);
void __fastcall Clear_d939a0(void*, int, int32_t, int32_t);
RT3_NOINLINE void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt);
RT3_NOINLINE void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);
void __fastcall Obj_SetPair1ed(uint8_t* o, int, uint32_t v);
uint32_t __fastcall Get_d9399c();
RT3_NOINLINE void ReleaseResourceD9399c();
float* __fastcall Vec3_Rotate(const float* v, int, float* out, const float* m);
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode);
int32_t __fastcall Emitter_SpawnParticle(uint8_t* e);
RT3_NOINLINE void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t);
void __fastcall ListPair_Reset(uint8_t* o);
uint8_t* __fastcall Emitter_WorldPoints(uint8_t* e, int, int32_t* count);
void __fastcall Emitter_Draw(uint8_t* e, int, uint8_t* out, int32_t* count);
int32_t __fastcall Emitter_Update(uint8_t* e, int, int32_t dt);
uint32_t __fastcall Particle_Color(uint8_t* e, int, uint8_t* p, int32_t mode);
RT3_NOINLINE void __fastcall Emitter_RetintAll(uint8_t* e);
void __fastcall Emitter_LaunchParticle(uint8_t* e, int, uint8_t* p, const float* at, int32_t t);
RT3_NOINLINE void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);
int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt);
void __fastcall Particle_ToBillboard(uint8_t* p, int, uint32_t* out, int32_t* count, const float* at);

// The texture name from the main pack's cache (all mips, flag 0x7f).
static uint8_t* Texture_Acquire(const char* name) { return TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, 0, 0x7f); }

// Replaces the texture held in global slot by name's (the old one released).
static void Texture_Replace(uint32_t slot, const char* name) {
    if (void* old = hook::Global<void*>(slot)) {
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, old);
        hook::Global<void*>(slot) = nullptr;
    }
    hook::Global<uint8_t*>(slot) = Texture_Acquire(name);
}

// The channel's live sample, or 0 (forgetting it and marking the channel dirty).
static inline uint8_t* LiveSample(uint8_t* c) {
    uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
    if (!s) {
        F<int32_t>(c, 0x110) = 0;
        F<int32_t>(c, 0x134) = 1;
    }
    return s;
}

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// Node lists {head, tail, count}; nodes link through their first dword. The pool
// 0x6d4030 +0x14 = {free list*, lo, hi, active} recycles nodes inside its block.
static inline void PushFront(uint32_t* l, uint32_t* n) {
    *n = l[0];
    l[2]++;
    l[0] = (uint32_t)n;
    if (!l[1]) l[1] = (uint32_t)n;
}

static inline void FreeAll(uint32_t* l) {
    for (uint32_t* n = (uint32_t*)l[0]; n;) {
        uint8_t* pool = F<uint8_t*>(hook::Global<uint8_t*>(0x6d4030), 0x14);
        uint32_t* next = (uint32_t*)*n;
        if (F<int32_t>(pool, 0xc) && (uint32_t)n >= F<uint32_t>(pool, 4) && (uint32_t)n < F<uint32_t>(pool, 8))
            PushFront(F<uint32_t*>(pool, 0), n);
        else
            ExeDelete(n);
        n = next;
    }
}

static inline uint8_t* SoundSys() { return hook::Global<uint8_t*>(0x6d402c); }

static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline uint8_t* Snd() { return hook::Global<uint8_t*>(0x6d402c); }

static inline int32_t Cell16(float v) {
    double d = (double)v * *(const float*)0x5dcfe8;
    int32_t r;
    __asm {
        fld d
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r >> 4;
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

static inline double Fl(uint32_t a) { return *(const float*)a; }

// pow(base, 1.0) through the CRT's x87 helper (as the original computes it).
static inline double PowOne(double base) {
    double r;
    __asm {
        fld base
        fld1
        mov eax, 0x5a1ba0
        call eax
        fstp r
    }
    return r;
}

static inline double D(uint32_t a) { return *(const double*)a; }

// Copies a float through the x87 (FLD / FSTP), which quiets signalling NaNs as the
// original's copies do.
static inline void X87Copy(void* dst, const void* src) {
    __asm {
        mov eax, src
        mov ecx, dst
        fld dword ptr [eax]
        fstp dword ptr [ecx]
    }
}

static inline double Sqrt87(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

// Draw density for a point: (step + 0.15) - distance to the camera x step x 1e-4,
// kept within 0.15 .. step.
static inline void Emitter_Density(uint8_t* e, const float* pt, const float* cam, float k, float step) {
    double dx = (double)pt[0] - cam[0], dy = (double)pt[1] - cam[1], dz = (double)pt[2] - cam[2];
    double r = ((double)step + Fl(0x5ce500)) - Sqrt87((dz * dz + dy * dy) + dx * dx) * k;
    F<float>(e, 0xdc) = (float)r;
    if (r > F<float>(hook::Global<uint8_t*>(0x6d4030), 0x28)) F<uint32_t>(e, 0xdc) = F<uint32_t>(hook::Global<uint8_t*>(0x6d4030), 0x28);
    else if (Fl(0x5ce500) > r) F<uint32_t>(e, 0xdc) = 0x3e19999a;
}

// 0x553aa0 / 0x5549b0 / 0x565da0. Release the pack resources held in 0xd93990 /
// 0xd9399c / 0xd97ab0.
static void ReleaseResourceIn(uint32_t slot) {
    void*& r = hook::Global<void*>(slot);
    if (!r) return;
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, r);
    r = nullptr;
}

void __fastcall Texture_SetD93990(const char* name) { Texture_Replace(0xd93990, name); }
RT3_REPLACE(0x554060, Texture_SetD93990);

void __fastcall Obj_Set8u(uint8_t* o, int, uint32_t v) { F<uint32_t>(o, 8) = v; }
RT3_REPLACE(0x5540a0, Obj_Set8u);

// 0x5540b0. Detaches a sound channel: from its terrain block list, the renderer
// list (+0x13d) and stops its sample.
void __fastcall Channel_Detach(uint8_t* c) {
    if (uint8_t* t = F<uint8_t*>(c, 4)) {
        if (PtrListNode* node = F<PtrListNode*>(c, 0)) {
            int32_t y = CellOf(F<float>(c, 0x11c)) >> 4;
            int32_t x = CellOf(F<float>(c, 0x114)) >> 4;
            Terrain_Remove1679(t, 0, node, x, y);
            F<void*>(c, 4) = nullptr;
        }
    }
    if (PtrListNode* n = F<PtrListNode*>(c, 0x13d))
        Widget_RemoveListNode((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, n);
    if (uint32_t key = F<uint32_t>(c, 0x110)) {
        Sound_Stop(SoundSys(), 0, key, 1);
        F<uint32_t>(c, 0x110) = 0;
    }
}
RT3_REPLACE(0x5540b0, Channel_Detach);

// Sound channel: sample key +0x110, position +0x114, volume +0x128, range
// +0x120/+0x124, rate +0x12c, dirty +0x134, loop +0x138. Samples come from the
// 3D sample table of the sound system 0x6d402c (+0x44 change flags).
RT3_NOINLINE uint32_t __fastcall Channel_Init(uint8_t* c) {
    F<int32_t>(c, 0xc) = 0;
    F<int32_t>(c, 0x110) = 0;
    F<uint32_t>(c, 0x128) = 0x3f800000;
    F<uint32_t>(c, 0x120) = 0x42400000;
    F<uint32_t>(c, 0x124) = 0x47000000;
    F<int32_t>(c, 0x134) = 0;
    F<int32_t>(c, 0x12c) = 0;
    F<int32_t>(c, 0x130) = 0;
    F<int32_t>(c, 0x138) = 0;
    F<int32_t>(c, 0) = 0;
    c[0x13c] = 0;
    F<int32_t>(c, 0x114) = 0;
    F<int32_t>(c, 0x118) = 0;
    F<int32_t>(c, 0x11c) = 0;
    return 0;
}
RT3_REPLACE(0x554140, Channel_Init);

// 0x5541a0. Copies all channel parameters to the sample, flagging them changed.
RT3_NOINLINE void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s) {
    uint32_t fl = F<uint32_t>(s, 0x44);
    F<float>(s, 0x10) = F<float>(c, 0x114);
    F<float>(s, 0x14) = F<float>(c, 0x118);
    F<uint32_t>(s, 0x44) = fl | 2;
    F<float>(s, 0x18) = F<float>(c, 0x11c);
    F<uint32_t>(s, 0x3c) = F<uint32_t>(c, 0x12c);
    F<uint32_t>(s, 0x44) = fl | 0x82;
    F<uint32_t>(s, 0x44) = fl | 0x83;
    F<float>(s, 0x28) = F<float>(c, 0x128);
    F<float>(s, 0x30) = F<float>(c, 0x120);
    F<float>(s, 0x34) = F<float>(c, 0x124);
    F<uint32_t>(s, 0x44) = fl | 0x93;
    F<int32_t>(c, 0x134) = 0;
}
RT3_REPLACE(0x5541a0, Channel_ApplyTo);

void __fastcall Channel_StopSample(uint8_t* c) {
    if (uint32_t key = F<uint32_t>(c, 0x110)) Sound_Stop(SoundSys(), 0, key, 1);
}
RT3_REPLACE(0x554210, Channel_StopSample);

RT3_NOINLINE int32_t __fastcall Channel_Check(uint8_t* c) {
    if (!F<uint32_t>(c, 0x110)) return 0;
    return (int32_t)LiveSample(c);
}
RT3_REPLACE(0x554230, Channel_Check);

RT3_NOINLINE uint8_t __fastcall Channel_Playing(uint8_t* c) { return c[0x13c] != 0; }
RT3_REPLACE(0x554270, Channel_Playing);

// 0x554280. Sets the channel's sample name (+0x10), stopping the current sample.
void __fastcall Channel_SetName(uint8_t* c, int, const char* name) {
    char* d = (char*)c + 0x10;
    while ((*d++ = *name++) != 0) {}
    if (uint32_t key = F<uint32_t>(c, 0x110)) {
        Sound_Stop(SoundSys(), 0, key, 1);
        F<uint32_t>(c, 0x110) = 0;
    }
}
RT3_REPLACE(0x554280, Channel_SetName);

// 0x5542c0. Sets the playback rate (clamped to 8000..44100 Hz).
RT3_NOINLINE void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate) {
    if (F<int32_t>(c, 0x12c) == rate) return;
    int32_t r = rate < 0x1f40 ? 0x1f40 : rate > 0xac44 ? 0xac44 : rate;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = LiveSample(c);
        if (!s) {
            F<int32_t>(c, 0x12c) = r;
            return;
        }
        F<int32_t>(s, 0x3c) = r;
        F<uint32_t>(s, 0x44) |= 0x80;
    }
    F<int32_t>(c, 0x12c) = r;
}
RT3_REPLACE(0x5542c0, Channel_SetRate);

// 0x554340. Sets the min/max distance.
void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi) {
    if (F<uint32_t>(c, 0x110)) {
        if (uint8_t* s = LiveSample(c)) {
            F<uint32_t>(s, 0x30) = lo;
            F<uint32_t>(s, 0x34) = hi;
            F<uint32_t>(s, 0x44) |= 0x10;
        }
    }
    F<uint32_t>(c, 0x120) = lo;
    F<uint32_t>(c, 0x124) = hi;
}
RT3_REPLACE(0x554340, Channel_SetRange);

// 0x5543a0. Sets the sample's value +0x50 (flag 0x400).
void __fastcall Channel_SetSample50(uint8_t* c, int, uint32_t v) {
    if (!F<uint32_t>(c, 0x110)) return;
    uint8_t* s = LiveSample(c);
    if (!s) return;
    F<uint32_t>(s, 0x50) = v;
    F<uint32_t>(s, 0x44) |= 0x400;
}
RT3_REPLACE(0x5543a0, Channel_SetSample50);

// 0x5543f0. Sets the loop count (+0x138, sample +0xc, flag 8).
RT3_NOINLINE void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v) {
    if (F<uint32_t>(c, 0x138) == v) return;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = LiveSample(c);
        if (!s) {
            F<uint32_t>(c, 0x138) = v;
            return;
        }
        F<uint32_t>(s, 0xc) = v;
        F<uint32_t>(s, 0x44) |= 8;
    }
    F<uint32_t>(c, 0x138) = v;
}
RT3_REPLACE(0x5543f0, Channel_SetLoop);

// 0x554450. Sets the channel volume (+0x128, sample +0x28 flag 1).
RT3_NOINLINE void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v) {
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
        if (s) {
            F<uint32_t>(s, 0x28) = v;
            F<uint32_t>(s, 0x44) |= 1;
            F<uint32_t>(c, 0x128) = v;
            return;
        }
        F<int32_t>(c, 0x110) = 0;
        F<int32_t>(c, 0x134) = 1;
    }
    F<uint32_t>(c, 0x128) = v;
}
RT3_REPLACE(0x554450, Channel_SetVolume);

// 0x5544b0. Sound channel command: 1 = play (count +0xc; resumes a paused sample,
// else starts name +0x10 at +0x114.. with volume +0x128, looping when the repeat
// count +0x130 is -1, otherwise consuming one), 2 = stop. +0x13c = active.
RT3_NOINLINE void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count) {
    if (cmd == 1) {
        F<int32_t>(c, 0xc) = count;
        c[0x13c] = 1;
        if (F<uint32_t>(c, 0x110) && count != -1) {
            if (F<int32_t>(c, 0x138)) return;
            uint8_t* p = (uint8_t*)Channel_Check(c);
            if (!p || F<int32_t>(p, 0xc) != 1) return;
            F<int32_t>(p, 0xc) = 0;
            F<uint32_t>(p, 0x44) |= 8;
            return;
        }
        int32_t n = F<int32_t>(c, 0x130);
        if (n != -1 && n <= 0) return;
        uint32_t key = Sound_Play3D(Snd(), 0, (const char*)c + 0x10, F<float>(c, 0x114), F<float>(c, 0x118),
                                    F<float>(c, 0x11c), F<float>(c, 0x128), n == -1 ? 1 : 0, 1, 2, 0.0f);
        F<uint32_t>(c, 0x110) = key;
        if (!key) return;
        if (F<int32_t>(c, 0x130) != -1) --F<int32_t>(c, 0x130);
        Channel_ApplyTo(c, 0, (uint8_t*)Channel_Check(c));
    } else if (cmd == 2) {
        c[0x13c] = 0;
        if (uint32_t key = F<uint32_t>(c, 0x110)) {
            Sound_Stop(Snd(), 0, key, 1);
            F<uint32_t>(c, 0x110) = 0;
        }
    }
}
RT3_REPLACE(0x5544b0, Channel_Command);

// 0x5545d0. Moves a positional sound channel to (x, y, z): updates its playing sample
// (+0x110; re-applied when +0x134 is set, else just moved; one-shots not looping
// +0x138 lose their loop), starts it when it enters an audible area of the terrain's
// 16-cell grid (+4, flags +0x47c) and stops it when it leaves; re-files the channel
// in the grid's per-cell lists (+0) when it changes cell.
void __fastcall Channel_MoveTo(uint8_t* c, int, float x, float y, float z) {
    int32_t ox = Cell16(F<float>(c, 0x114));
    int32_t oz = Cell16(F<float>(c, 0x11c));
    F<float>(c, 0x114) = x;
    F<float>(c, 0x118) = y;
    F<float>(c, 0x11c) = z;
    int32_t nx = Cell16(x);
    int32_t nz = Cell16(z);
    uint8_t* grid = F<uint8_t*>(c, 4);
    uint8_t was = grid[0x47c + ox * 64 + oz] & 1;
    uint8_t now = grid[0x47c + nx * 64 + nz] & 1;
    bool playing = false;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
        if (s) {
            if (F<int32_t>(c, 0x134) == 1) Channel_ApplyTo(c, 0, s);
            else {
                F<float>(s, 0x10) = x;
                F<float>(s, 0x14) = y;
                F<float>(s, 0x18) = z;
                F<uint32_t>(s, 0x44) |= 2;
            }
            if (!F<int32_t>(c, 0x138) && F<int32_t>(s, 0xc) == 1) {
                F<uint32_t>(s, 0x44) |= 8;
                F<int32_t>(s, 0xc) = 0;
            }
            playing = true;
        } else {
            F<uint32_t>(c, 0x110) = 0;
            F<int32_t>(c, 0x134) = 1;
        }
    }
    if (!playing) F<uint32_t>(c, 0x110) = 0;
    if (was != now || !F<uint32_t>(c, 0x110)) {
        if (now) Channel_Command(c, 0, 1, F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0xd));
        else {
            c[0x13c] = 0;
            if (F<uint32_t>(c, 0x110)) {
                Sound_Stop(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
                F<uint32_t>(c, 0x110) = 0;
            }
        }
    }
    if (ox == nx && oz == nz) return;
    int32_t w = ((Terrain_Rows(F<uint8_t*>(c, 4)) * 0x5c - 0x5c) / 0x5c) >> 4;
    int32_t h = ((Terrain_Cols(F<uint8_t*>(c, 4)) * 0x5c - 0x5c) / 0x5c) >> 4;
    if (F<PtrListNode*>(c, 0)) Terrain_Remove1679(F<uint8_t*>(c, 4), 0, F<PtrListNode*>(c, 0), ox, oz);
    if (nx >= 0 && nx < w && nz >= 0 && nz < h) F<PtrListNode*>(c, 0) = Terrain_Add1679(F<uint8_t*>(c, 4), 0, c, nx, nz);
    else F<PtrListNode*>(c, 0) = nullptr;
}
RT3_REPLACE(0x5545d0, Channel_MoveTo);

// 0x554830. Sets up a positional sound channel: sample `name` on terrain `grid` at
// (x, y, z) with flags a6 (+0x130) / a7 (+0x138) and, if positive, volume `vol`
// (+0x120, scaled +0x124). Returns the channel.
uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6,
                                  int32_t a7, float vol) {
    Channel_Init(ch);
    if (vol > *(const float*)0x5c8598) {
        F<float>(ch, 0x120) = vol;
        F<float>(ch, 0x124) = (float)((double)vol * *(const float*)0x5ddb68);
    }
    F<int32_t>(ch, 0x138) = a7;
    F<uint8_t*>(ch, 4) = grid;
    F<int32_t>(ch, 0x13d) = 0;
    F<int32_t>(ch, 0x130) = a6;
    F<int32_t>(ch, 8) = 0;
    strcpy((char*)ch + 0x10, name);
    Channel_MoveTo(ch, 0, x, y, z);
    return ch;
}
RT3_REPLACE(0x554830, Channel_Setup);

RT3_NOINLINE void __fastcall Nop5548c0(void*, int, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x5548c0, Nop5548c0);

void __fastcall Clear_d939a0(void*, int, int32_t, int32_t) { hook::Global<int32_t>(0xd939a0) = 0; }
RT3_REPLACE(0x5548d0, Clear_d939a0);

// 0x5548e0. Sets the view parameters when the first (a) is positive.
RT3_NOINLINE void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    if (a <= 0) return;
    F<int32_t>(v, 0x54) = a;
    F<uint32_t>(v, 0x58) = b;
    F<uint32_t>(v, 0x9c) = c;
    F<uint32_t>(v, 0xa0) = d;
    F<uint32_t>(v, 0xa4) = e;
    F<uint32_t>(v, 0xa8) = f;
}
RT3_REPLACE(0x5548e0, View_SetParams);

// 0x554920. Frame time +0xb4: 0.05 s while dt is at most 0x5d0058, else dt when the
// frame budget (+0xb8) holds at least one frame of dt (x87 with the exe's _ftol).
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt) {
    if (!(*(const float*)0x5d0058 < dt) && !(dt != dt)) {
        F<uint32_t>(t, 0xb4) = 0x3d4ccccd;
        return;
    }
    int32_t budget = F<int32_t>(t, 0xb8), n;
    __asm {
        fild budget
        fdiv dt
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    if (n > 0) F<float>(t, 0xb4) = dt;
}
RT3_REPLACE(0x554920, Timer_SetFrameTime);

RT3_NOINLINE void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v) { F<uint32_t>(t, 0xb8) = v; }
RT3_REPLACE(0x554970, Timer_SetBudget);

void __fastcall Obj_SetPair1ed(uint8_t* o, int, uint32_t v) {
    F<uint32_t>(o, 0x1ed) = v;
    F<uint32_t>(o, 0x1f5) = v;
}
RT3_REPLACE(0x554980, Obj_SetPair1ed);

uint32_t __fastcall Get_d9399c() { return hook::Global<uint32_t>(0xd9399c); }
RT3_REPLACE(0x5549a0, Get_d9399c);

RT3_NOINLINE void ReleaseResourceD9399c() { ReleaseResourceIn(0xd9399c); }
RT3_REPLACE(0x5549b0, ReleaseResourceD9399c);

// 0x5549d0. out = v rotated by the 3x3 part of m (row vector).
RT3_NOINLINE float* __fastcall Vec3_Rotate(const float* v, int, float* out, const float* m) {
    double z = ((double)m[10] * v[2] + (double)m[2] * v[0]) + (double)m[6] * v[1];
    double y = ((double)m[9] * v[2] + (double)m[1] * v[0]) + (double)m[5] * v[1];
    double x = ((double)m[8] * v[2] + (double)m[4] * v[1]) + (double)m[0] * v[0];
    out[0] = (float)x;
    out[1] = (float)y;
    out[2] = (float)z;
    return out;
}
RT3_REPLACE(0x5549d0, Vec3_Rotate);

// 0x554a30. Clears the list pair +0/+4 (count +0x34): mode 1 frees all nodes and
// both lists, otherwise the second list's nodes move to the first.
RT3_NOINLINE void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode) {
    if (F<int32_t>(o, 0x34) > 0) {
        if (mode == 1) {
            FreeAll(F<uint32_t*>(o, 0));
            FreeAll(F<uint32_t*>(o, 4));
            F<int32_t>(o, 0x34) = 0;
        } else {
            for (uint32_t* n = (uint32_t*)*F<uint32_t*>(o, 4); n;) {
                uint32_t* next = (uint32_t*)*n;
                PushFront(F<uint32_t*>(o, 0), n);
                n = next;
            }
            uint32_t* b = F<uint32_t*>(o, 4);
            b[0] = b[1] = b[2] = 0;
            return;
        }
    } else if (mode != 1) {
        return;
    }
    if (void* a = F<void*>(o, 0)) {
        ExeDelete(a);
        F<void*>(o, 0) = nullptr;
    }
    if (void* b = F<void*>(o, 4)) {
        ExeDelete(b);
        F<void*>(o, 4) = nullptr;
    }
}
RT3_REPLACE(0x554a30, ListPair_Clear);

// 0x554b60. Spawns a particle for emitter e from the shared pool (0x6d4030 +0x14:
// {free list, ..., total +0xc, node size +0x10}; only with over 20% free unless
// flag 0x8000): pushed on the emitter's list (+0, count +0x34) with the emitter's
// colour (+0x48, jittered by the render LCG 0x624324 with flag 2), box +0xe0..+0xec,
// flags +0xac and size +0x98. Returns 1 if spawned.
RT3_NOINLINE int32_t __fastcall Emitter_SpawnParticle(uint8_t* e) {
    uint8_t* pool = F<uint8_t*>(hook::Global<uint8_t*>(0x6d4030), 0x14);
    uint32_t** freeList = F<uint32_t**>(pool, 0);
    double ratio = (double)(int32_t)((uint32_t*)freeList)[2] / F<int32_t>(pool, 0xc);
    if (!(ratio > *(const float*)0x5ddcb0) && !(ratio != ratio) && !(F<uint32_t>(e, 0xac) & 0x8000)) return 0;
    uint32_t* node;
    if (F<int32_t>(pool, 0xc)) {
        uint32_t* l = (uint32_t*)F<void*>(pool, 0);
        node = (uint32_t*)l[0];
        if (!node) return 0;
        uint32_t next = node[0];
        l[0] = next;
        if ((uint32_t)node == l[1]) l[1] = next;
        --l[2];
    } else {
        node = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(F<uint32_t>(pool, 0x10));
    }
    if (!node) return 0;
    uint32_t* alive = F<uint32_t*>(e, 0);
    ++F<int32_t>(e, 0x34);
    uint32_t color = F<uint32_t>(e, 0x48);
    node[0] = alive[0];
    alive[0] = (uint32_t)node;
    ++alive[2];
    if (!alive[1]) alive[1] = (uint32_t)node;
    if (F<uint32_t>(e, 0xac) & 2) {
        uint32_t& seed = hook::Global<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        int32_t d = (int8_t)(uint8_t)((seed & 0x3f) - 0x1f);
        int32_t c[3] = { (int32_t)((color >> 16) & 0xff) + d, (int32_t)((color >> 8) & 0xff) + d, (int32_t)(color & 0xff) + d };
        for (int k = 0; k < 3; ++k) c[k] = c[k] > 0xff ? 0xff : (c[k] < 0 ? 0 : c[k]);
        color = ((uint32_t)c[0] << 16) | ((uint32_t)c[1] << 8) | (uint32_t)c[2];
    }
    uint8_t* p = (uint8_t*)node[1];
    F<uint32_t>(p, 0x70) = color & 0xffffff;
    F<uint32_t>(p, 0x2c) = F<uint32_t>(e, 0xe0);
    F<uint32_t>(p, 0x30) = F<uint32_t>(e, 0xe4);
    F<uint32_t>(p, 0x34) = F<uint32_t>(e, 0xe8);
    F<uint32_t>(p, 0x38) = F<uint32_t>(e, 0xec);
    F<uint32_t>(p, 0x5c) = F<uint32_t>(e, 0xac);
    F<uint32_t>(p, 0x60) = F<uint32_t>(e, 0x98);
    return 1;
}
RT3_REPLACE(0x554b60, Emitter_SpawnParticle);

// 0x554ce0. Restarts the emitter with n particles: clears its lists (unless new and
// empty), makes two fresh lists (+0, +4) and spawns n.
RT3_NOINLINE void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t) {
    if (n <= 0 || F<int32_t>(e, 0x34) > 0) {
        ListPair_Clear(e, 0, 1);
        if (n <= 0) return;
    }
    for (int k = 0; k < 2; ++k) {
        uint32_t* l = (uint32_t*)ExeNew(0xc);
        if (l) {
            l[0] = 0;
            l[1] = 0;
            l[2] = 0;
        }
        F<uint32_t*>(e, k * 4) = l;
    }
    for (int32_t i = n; i; --i) Emitter_SpawnParticle(e);
}
RT3_REPLACE(0x554ce0, Emitter_Reset);

void __fastcall ListPair_Reset(uint8_t* o) {
    F<int32_t>(o, 0xc4) = 0;
    F<int32_t>(o, 0xc0) = 0;
    ListPair_Clear(o, 0, 0);
}
RT3_REPLACE(0x554d50, ListPair_Reset);

// 0x554d70. Transforms the emitter points (+0x10, +0x30 of them) by the owner's
// world matrix (+0xc +0xce) into +0x1dd (pointer table, then xyz); without points
// the owner's position. Returns the table and the count.
uint8_t* __fastcall Emitter_WorldPoints(uint8_t* e, int, int32_t* count) {
    const float* src = F<float*>(e, 0x10);
    int32_t n = F<int32_t>(e, 0x30);
    uint32_t* tab = F<uint32_t*>(e, 0x1dd);
    float* out = (float*)(tab + n);
    if (src) {
        for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i, src += 3, out += 3) {
            tab[i] = (uint32_t)out;
            const float* m = (const float*)(F<uint8_t*>(e, 0xc) + 0xce);
            double x = ((double)m[8] * src[2] + (double)m[4] * src[1]) + (double)src[0] * m[0] + m[12];
            double y = ((double)m[9] * src[2] + (double)m[1] * src[0]) + (double)m[5] * src[1] + m[13];
            float z = (float)(((double)m[10] * src[2] + (double)m[2] * src[0]) + (double)m[6] * src[1] + m[14]);
            out[0] = (float)x;
            out[2] = z;
            out[1] = (float)y;
        }
    } else {
        tab[0] = (uint32_t)out;
        const float* m = (const float*)(F<uint8_t*>(e, 0xc) + 0xce);
        out[0] = m[12];
        out[2] = m[14];
        out[1] = m[13];
    }
    F<int32_t>(e, 0x1e1) = 1;
    *count = F<int32_t>(e, 0x30);
    return F<uint8_t*>(e, 0x1dd);
}
RT3_REPLACE(0x554d70, Emitter_WorldPoints);

// 0x554e70. Draws the emitter's particles as billboards into out (6 vertices of
// 0x18 bytes each; *count running) unless the game is paused (frame step 0). Plain
// emitters draw every live particle; attached ones (0x400) draw their hash group's
// particles (0x6d4030 +4) at each attach point (+0x1dd, transformed by the object
// unless +0x1e1), thinned by distance to the camera (step accumulated per particle
// until it reaches the frame step, unless +0xb0 & 2).
void __fastcall Emitter_Draw(uint8_t* e, int, uint8_t* out, int32_t* count) {
    uint8_t* camObj = F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28);
    float cam[3];
    X87Copy(&cam[1], camObj + 0x20);
    F<uint32_t>(cam, 0) = F<uint32_t>(camObj, 0x1c);
    X87Copy(&cam[2], camObj + 0x24);
    uint8_t* clock = hook::Global<uint8_t*>(0x6d4030);
    float k = (float)((double)F<float>(clock, 0x28) * Fl(0x5dd230));
    if (Fl(0x5c8598) == F<float>(clock, 0x28)) return;
    if (!(F<uint32_t>(e, 0xac) & 0x400)) {
        float at[3] = { 0.0f, 0.0f, 0.0f };
        Emitter_Density(e, &F<float>(e, 0x14), cam, k, F<float>(clock, 0x28));
        for (void** n = *F<void***>(e, 4); n; n = (void**)n[0])
            Particle_ToBillboard((uint8_t*)n[1], 0, (uint32_t*)(out + *count * 0x18), count, at);
        return;
    }
    uint8_t* h = (uint8_t*)Hash_Get(F<HashMap*>(clock, 4), 0, (uint32_t)e);
    if (!h) return;
    uint8_t* group = F<uint8_t*>(h, 4);
    float** pts = F<float**>(e, 0x1dd);
    for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i) {
        float tmp[3], at[3];
        const float* pt = F<int32_t>(e, 0x1e1) ? *pts : Vec3_Transform(*pts, 0, tmp, (const float*)(F<uint8_t*>(e, 0xc) + 0xce));
        for (int j = 0; j < 3; ++j) ((uint32_t*)at)[j] = ((const uint32_t*)pt)[j];
        ++pts;
        Emitter_Density(e, at, cam, k, F<float>(hook::Global<uint8_t*>(0x6d4030), 0x28));
        double acc = F<float>(e, 0xdc);
        for (void** n = *F<void***>(group, 4); n; n = (void**)n[0]) {
            double step = F<float>(hook::Global<uint8_t*>(0x6d4030), 0x28);
            if (!(acc < step || acc != acc) || (e[0xb0] & 2)) {
                Particle_ToBillboard((uint8_t*)n[1], 0, (uint32_t*)(out + *count * 0x18), count, at);
                acc = Fl(0x5c8598);
            }
            acc = acc + F<float>(e, 0xdc);
        }
    }
    F<int32_t>(e, 0x1e1) = 0;
}
RT3_REPLACE(0x554e70, Emitter_Draw);

// 0x555120. Advances the emitter's particles by `dt` once per frame (frame counter
// 0x6d4030 +0x2c, kept in +0x1f9): an emitter attached to an object (+0xc; trails
// +0xac & 0x19) follows it (unless frozen, 0xd939a0) and drags its particles by the
// move (spread over life with 0x10 / 1; vertical damped without 1); flip-book
// sprites (+0x118 frames, +0x124 per row, cell +0x110 x +0x114 from +0x11c/+0x120,
// rate +0x128) step on; spent particles (unless immortal, 0x2000000) move from the
// live list (+4) to the free list (+0). 0 when skipped.
RT3_NOINLINE int32_t __fastcall Emitter_Update(uint8_t* e, int, int32_t dt) {
    if (dt < 0) return 0;
    int32_t frame = F<int32_t>(hook::Global<uint8_t*>(0x6d4030), 0x2c);
    if (F<int32_t>(e, 0x1f9) == frame) return 0;
    uint32_t trail = F<uint32_t>(e, 0xac) & 0x19;
    float mx = 0.0f, my = 0.0f, mz = 0.0f;
    F<int32_t>(e, 0x1f9) = frame;
    if (trail && F<uint8_t*>(e, 0xc)) {
        if (!hook::Global<int32_t>(0xd939a0)) {
            for (int32_t k = 0; k < 3; ++k) F<uint32_t>(e, 0x20 + k * 4) = F<uint32_t>(e, 0x14 + k * 4);
            for (int32_t k = 0; k < 3; ++k) F<uint32_t>(e, 0x14 + k * 4) = F<uint32_t>(F<uint8_t*>(e, 0xc), 0x1e2 + k * 4);
            uint8_t* g = hook::Global<uint8_t*>(0xd939a4);
            if ((F<uint32_t>(e, 0xac) & 0x800000) && g) {
                for (int32_t k = 0; k < 3; ++k) F<uint32_t>(g, 0x94 + k * 4) = F<uint32_t>(e, 0x14 + k * 4);
                for (int32_t k = 0; k < 3; ++k) F<uint32_t>(g, 0x88 + k * 4) = F<uint32_t>(e, 0x20 + k * 4);
            }
        }
        mx = (float)((double)F<float>(e, 0x14) - F<float>(e, 0x20));
        float dy = (float)((double)F<float>(e, 0x18) - F<float>(e, 0x24));
        mz = (float)((double)F<float>(e, 0x1c) - F<float>(e, 0x28));
        my = dy;
        if (!(e[0xac] & 1)) my = (float)((double)dy * Fl(0x5d006c));
    }
    int32_t now = ClockMs();
    void** prev = nullptr;
    void** node = *F<void***>(e, 4);
    if (!node) return 1;
    float fdt = (float)dt;
    while (node) {
        uint8_t* p = (uint8_t*)node[1];
        if (trail) {
            float px = F<float>(p, 0), py = F<float>(p, 4), pz = F<float>(p, 8);
            uint32_t fl = F<uint32_t>(e, 0xac);
            if (((fl & 0x10) && dt > 0) || (fl & 1)) {
                F<int32_t>(e, 0x2c) = 1;
                double m = PowOne((double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c));
                float ox = (float)(mx * m), oy = (float)(my * m);
                double oz = m * mz;
                double nx = (double)ox + px, ny = (double)oy + py;
                F<float>(p, 8) = (float)(oz + pz);
                F<float>(p, 0) = (float)nx;
                F<float>(p, 4) = (float)ny;
            } else {
                double nx = (double)px + mx, ny = (double)py + my;
                F<float>(p, 8) = (float)((double)pz + mz);
                F<float>(p, 0) = (float)nx;
                F<float>(p, 4) = (float)ny;
            }
        }
        int32_t frames = F<int32_t>(e, 0x118);
        if (frames > 1) {
            uint8_t* q = (uint8_t*)node[1];
            if (1000 / F<int32_t>(e, 0x128) < now - F<int32_t>(q, 0x58)) {
                int32_t fr = F<int32_t>(q, 0x54) % frames;
                int32_t col = fr % F<int32_t>(e, 0x124), row = fr / F<int32_t>(e, 0x124);
                const double k = Fl(0x5ddcb4);
                double u0 = (double)(col * F<int32_t>(e, 0x110) + F<int32_t>(e, 0x11c)) * k;
                double v0 = (double)(row * F<int32_t>(e, 0x114) + F<int32_t>(e, 0x120)) * k;
                double u1 = (double)F<int32_t>(e, 0x110) * k + u0;
                double v1 = (double)F<int32_t>(e, 0x114) * k + v0;
                F<float>(q, 0x2c) = (float)u0;
                F<float>(q, 0x30) = (float)v1;
                F<float>(q, 0x34) = (float)u1;
                F<float>(q, 0x38) = (float)v0;
                F<int32_t>((uint8_t*)node[1], 0x58) = now;
                F<int32_t>((uint8_t*)node[1], 0x54) = fr + 1;
            }
        }
        if (!Particle_Advance((uint8_t*)node[1], 0, fdt) && !(F<uint32_t>(e, 0xac) & 0x2000000)) {
            void** next = (void**)node[0];
            if (prev) SList_Unlink(F<void**>(e, 4), 0, prev, node);
            else {
                void** l = F<void**>(e, 4);
                if (void** h = (void**)l[0]) {
                    void** n2 = (void**)h[0];
                    bool last = h == (void**)l[1];
                    l[0] = n2;
                    if (last) l[1] = n2;
                    --*(int32_t*)&l[2];
                }
            }
            void** fl = F<void**>(e, 0);
            node[0] = fl[0];
            fl[0] = node;
            ++*(int32_t*)&fl[2];
            if (!fl[1]) fl[1] = node;
            node = next;
        } else {
            prev = node;
            node = (void**)node[0];
        }
    }
    return 1;
}
RT3_REPLACE(0x555120, Emitter_Update);

// 0x5554f0. Colour of particle p from emitter e (colours +0x48 / +0x4c, tint
// +0x3c..+0x44): mode 1 picks a random blend of the two (shared LCG 0x624324) and
// stores it in p +0x74; with e +0xac bit 27 the blend follows p's age (+0x48 of
// +0x4c); else p keeps its colour. Quirk: mode 1 leaves the random factor on the x87
// stack, as the original does.
RT3_NOINLINE uint32_t __fastcall Particle_Color(uint8_t* e, int, uint8_t* p, int32_t mode) {
    int32_t r, g, b;
    uint32_t ca = F<uint32_t>(e, 0x48), cb = F<uint32_t>(e, 0x4c);
    if (mode == 1) {
        uint32_t& seed = hook::Global<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        double t = (double)(int32_t)(seed & 0x7fff) * *(const float*)0x5ddcb8;
        double u = (double)*(const float*)0x5c85b4 - t;
        r = Ftol((double)(int32_t)((ca >> 16) & 0xff) * u + (double)(int32_t)((cb >> 16) & 0xff) * t);
        g = Ftol((double)(int32_t)((ca >> 8) & 0xff) * u + (double)(int32_t)((cb >> 8) & 0xff) * t);
        b = Ftol((double)(int32_t)(ca & 0xff) * u + (double)(int32_t)(cb & 0xff) * t);
        F<uint32_t>(p, 0x74) = (((uint32_t)r << 8 | (uint32_t)g) << 8 | (uint32_t)b) & 0xffffff;
    } else if (F<uint32_t>(e, 0xac) & 0x8000000) {
        double u = (double)*(const float*)0x5c85b4 - (double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c);
        double v = (double)*(const float*)0x5c85b4 - u;
        r = Ftol((double)(int32_t)((cb >> 16) & 0xff) * u + (double)(int32_t)((ca >> 16) & 0xff) * v);
        g = Ftol((double)(int32_t)((cb >> 8) & 0xff) * u + (double)(int32_t)((ca >> 8) & 0xff) * v);
        b = Ftol((double)(int32_t)(cb & 0xff) * u + (double)(int32_t)(ca & 0xff) * v);
        F<uint32_t>(p, 0x74) = (((uint32_t)r << 8 | (uint32_t)g) << 8 | (uint32_t)b) & 0xffffff;
    } else {
        uint32_t c = F<uint32_t>(p, 0x74);
        r = (c >> 16) & 0xff;
        g = (c >> 8) & 0xff;
        b = c & 0xff;
    }
    uint32_t out = (uint32_t)Ftol((double)r * F<float>(e, 0x3c)) << 8;
    out = (out | (uint32_t)Ftol((double)g * F<float>(e, 0x40))) << 8;
    out |= (uint32_t)Ftol((double)b * F<float>(e, 0x44));
    return out;
}
RT3_REPLACE(0x5554f0, Particle_Color);

// 0x555740. Re-tints every live particle of the emitter (+4 list) into +0x70.
RT3_NOINLINE void __fastcall Emitter_RetintAll(uint8_t* e) {
    for (uint8_t** n = *F<uint8_t***>(e, 4); n; n = (uint8_t**)n[0])
        F<uint32_t>(n[1], 0x70) = Particle_Color(e, 0, n[1], 0) & 0xffffff;
}
RT3_REPLACE(0x555740, Emitter_RetintAll);

// 0x555780. Launches particle p from `at` at time t: its offset (+0x88..+0x90) and
// velocity (+0x70..+0x78 plus +0x7c..+0x84) are jittered by the emitter LCG
// (0x624324), turned with the attached object (+0xc, matrix +0xce) when +0xac & 1;
// the velocity jitter's x / z signs may flip (0x80000000). Lifetime +0x54 +/- +0x58
// (scaled by +0xd4 for 0x200), size +0x9c..+0xa4 (random within for 0x20000), spin
// (0x20080000), flip-book frame (0x4000000), end velocity (0x40000000, from
// +0xf0..+0x104) and gravity (0x800) follow; then the particle starts and is advanced
// to the present. Without a turn or an end velocity the original copies an
// uninitialized stack buffer into p +0x18..+0x20 (Emitter_Start overwrites it unless
// the particle's flags have 0x40000000); the rewrite leaves those fields alone there.
void __fastcall Emitter_LaunchParticle(uint8_t* e, int, uint8_t* p, const float* at, int32_t t) {
    uint32_t& seed = hook::Global<uint32_t>(0x624324);
    auto next = [&]() { return seed = seed * 0x19660d + 0x3c6ef35f; };
    auto jitter = [&](uint32_t off) {
        int32_t v = (int32_t)(next() & 0xff) - 0x7f;
        return (float)((double)v * D(0x5ddcc0) * F<float>(e, off));
    };
    const double one = Fl(0x5c85b4);
    float vel[3] = { F<float>(e, 0x70), F<float>(e, 0x74), F<float>(e, 0x78) };
    float off[3], velJ[3];
    off[0] = jitter(0x88);
    off[1] = jitter(0x8c);
    off[2] = jitter(0x90);
    velJ[0] = jitter(0x7c);
    velJ[1] = jitter(0x80);
    velJ[2] = jitter(0x84);
    float rot[3];
    bool turned = false;
    if ((F<uint32_t>(e, 0xac) & 1) && F<uint8_t*>(e, 0xc)) {
        const float* m = (const float*)(F<uint8_t*>(e, 0xc) + 0xce);
        float* r = Vec3_Rotate(vel, 0, rot, m);
        for (int k = 0; k < 3; ++k) vel[k] = r[k];
        r = Vec3_Rotate(off, 0, rot, m);
        for (int k = 0; k < 3; ++k) off[k] = r[k];
        r = Vec3_Rotate(velJ, 0, rot, m);
        for (int k = 0; k < 3; ++k) velJ[k] = r[k];
        turned = true;
    }
    if ((int32_t)F<uint32_t>(e, 0xac) < 0) {
        if (next() & 1) F<float>(e, 0x7c) = -F<float>(e, 0x7c);
        if (next() & 1) F<float>(e, 0x84) = -F<float>(e, 0x84);
    }
    double life = (double)((int32_t)(next() & 0xff) - 0x7f) * D(0x5ddcc0) * F<int32_t>(e, 0x58) + F<int32_t>(e, 0x54);
    if (life <= Fl(0x5c8598)) life = one;
    if (F<uint32_t>(e, 0xac) & 0x200) {
        double s = (double)(next() & 0xff) * Fl(0x5dd17c);
        life = life * F<float>(e, 0xd4) * (one - s) + s * life;
    }
    double lo;
    float hi;
    if (F<uint32_t>(e, 0xac) & 0x20000) {
        double s1 = (double)(next() & 0xff) * Fl(0x5dd17c);
        float s2 = (float)((double)(next() & 0xff) * Fl(0x5dd17c));
        double m1 = (one - s1) * F<float>(e, 0x9c) + s1 * F<float>(e, 0xa4);
        float m2 = (float)((one - s2) * F<float>(e, 0x9c) + (double)s2 * F<float>(e, 0xa4));
        if (m1 > m2) {
            lo = m2;
            hi = (float)m1;
        } else {
            lo = m1;
            hi = m2;
        }
    } else {
        hi = F<float>(e, 0xa4);
        lo = F<float>(e, 0x9c);
    }
    if (F<uint32_t>(e, 0xac) & 0x20080000) {
        F<float>(p, 0x40) = (float)((double)((int32_t)(next() & 0x7fff) % 0x168 + 1) * Fl(0x5ce4fc));
        X87Copy(p + 0x3c, e + 0x94);
        if ((F<uint32_t>(e, 0xac) & 0x20000000) && !(next() & 1)) F<float>(p, 0x3c) = -F<float>(p, 0x3c);
    }
    if (F<uint32_t>(e, 0xac) & 0x4000000) F<int32_t>(p, 0x54) = (int32_t)(next() & 0x7fff) % F<int32_t>(e, 0x118);
    int32_t fr = F<int32_t>(p, 0x54) % F<int32_t>(e, 0x118);
    int32_t col = fr % F<int32_t>(e, 0x124), row = fr / F<int32_t>(e, 0x124);
    const double k = Fl(0x5ddcb4);
    double u0 = (double)(F<int32_t>(e, 0x110) * col + F<int32_t>(e, 0x11c)) * k;
    double v0 = (double)(F<int32_t>(e, 0x114) * row + F<int32_t>(e, 0x120)) * k;
    double u1 = (double)F<int32_t>(e, 0x110) * k + u0;
    double v1 = (double)F<int32_t>(e, 0x114) * k + v0;
    F<float>(p, 0x2c) = (float)u0;
    F<float>(p, 0x30) = (float)v1;
    F<float>(p, 0x34) = (float)u1;
    F<float>(p, 0x38) = (float)v0;
    float loS = (float)(lo * F<float>(e, 0xd0));
    float hiS = (float)((double)hi * F<float>(e, 0xd0));
    float lifeS = (float)(life * F<float>(e, 0xcc));
    if (lifeS <= Fl(0x5c8598)) lifeS = 1.0f;
    float rate = (float)(((double)hiS - loS) / lifeS);
    if (hook::Global<int32_t>(0xd939a8))
        F<float>(p, 0x60) = (float)((double)F<float>(hook::Global<uint8_t*>(0x6d4030), 0x30) * F<float>(e, 0x98));
    else
        X87Copy(p + 0x60, e + 0x98);
    uint32_t color;
    if (e[0xac] & 2) {
        color = Particle_Color(e, 0, p, 1);
    } else {
        F<uint32_t>(p, 0x74) = F<uint32_t>(e, 0x48) & 0xffffff;
        color = Particle_Color(e, 0, p, 0);
    }
    F<uint32_t>(p, 0x70) = color & 0xffffff;
    ++F<int32_t>(e, 0x50);
    if (F<uint32_t>(e, 0xac) & 0x40000000) {
        seed = 0xd1ccf6e9 - seed * 0x50b6f56b;
        double s = (double)(seed & 0xff) * D(0x5dd8f8);
        float q0 = (float)(F<float>(e, 0xfc) * s + F<float>(e, 0xf0));
        float q1 = (float)(F<float>(e, 0x100) * s + F<float>(e, 0xf4));
        double q2 = s * F<float>(e, 0x104) + F<float>(e, 0xf8);
        F<uint32_t>(p, 0x28) = F<uint32_t>(e, 0x108);
        F<float>(p, 0x18) = (float)((double)q0 * Fl(0x5c9b68));
        F<float>(p, 0x1c) = (float)((double)q1 * Fl(0x5c9b68));
        F<float>(p, 0x20) = (float)(q2 * Fl(0x5c9b68));
    } else {
        X87Copy(p + 0x28, e + 0x108);
        if (turned)
            for (int i = 0; i < 3; ++i) F<float>(p, 0x18 + i * 4) = rot[i];
    }
    if (F<uint32_t>(e, 0xac) & 0x800) X87Copy(p + 0x6c, e + 0x1e9);
    float to[3], from[3];
    for (int i = 0; i < 3; ++i) to[i] = (float)((double)vel[i] + velJ[i]);
    for (int i = 0; i < 3; ++i) from[i] = (float)((double)off[i] + at[i]);
    Emitter_Start(p, 0, Ftol(lifeS), (const uint32_t*)from, (const uint32_t*)to, *(uint32_t*)&loS, rate);
    Particle_Advance(p, 0, (float)(F<int32_t>(e, 0x6c) - t + hook::Global<int32_t>(0x6257d8)));
}
RT3_REPLACE(0x555780, Emitter_LaunchParticle);

// 0x555e50. Per-frame emitter step (once per frame, +0x1f9 vs clock 0x6d4030 +0x2c;
// never when disabled, 0x1000000): optionally takes the engine time (0x800000)
// into 0x6257d8 and picks its ring slot in 0xd939a4; then, while active (+0x34,
// not 0x400, game running), advances the particles and either fires bursts of +0x1ed
// (+0x40000: random timing) or emits one particle per period (+0xb8 / +0xb4 ms) at
// each attach point (+0x30, from the object +0xc / point list +0x10 or +0x1b0),
// accumulating the rate +0xd8 by the frame step; up to +0x38 per call while the
// budget +0xbc lasts, growing the pool when allowed (4). Records the times
// +0xc0 / +0xc4.
RT3_NOINLINE void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force) {
    uint32_t fl = F<uint32_t>(e, 0xac);
    if (fl & 0x1000000) return;
    uint8_t* clock = hook::Global<uint8_t*>(0x6d4030);
    if (F<int32_t>(e, 0x1f9) == F<int32_t>(clock, 0x2c)) return;
    int32_t& gnow = hook::Global<int32_t>(0x6257d8);
    int32_t now;
    if ((fl & 0x800000) && !hook::Global<int32_t>(0xd939a0)) {
        now = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x2c);
        gnow = now;
    } else {
        now = gnow;
    }
    if (fl & 0x4000) {
        int32_t a = F<int32_t>(e, 0x6c);
        if (a - F<int32_t>(e, 0xc0) + now <= 0 || a - F<int32_t>(e, 0xc4) + now <= 0) F<int32_t>(e, 0x6c) = a + 0x32;
    }
    int32_t start = F<int32_t>(e, 0x6c);
    int32_t dt = start - F<int32_t>(e, 0xc0) + now;
    if ((fl & 0x800000) && dt > 0) {
        int32_t c = F<int32_t>(e, 0x68);
        hook::Global<uint32_t>(0xd939a4) = (uint32_t)((c % 64) * 0xe8) + F<uint32_t>(e, 0x64);
        F<int32_t>(e, 0x68) = c + 1;
    }
    int32_t base = now;
    if (F<int32_t>(e, 0x34) > 0 && !(fl & 0x400) && Fl(0x5c8598) < F<float>(clock, 0x28)) {
        int32_t period = Ftol((double)F<int32_t>(e, 0xb8) / F<float>(e, 0xb4));
        int32_t span = start - F<int32_t>(e, 0xc4) + now;
        if ((e[0xb0] & 1) || span < 0) {
            F<int32_t>(e, 0xc4) = start + now;
            F<int32_t>(e, 0xc0) = start + now;
            return;
        }
        int32_t& reset = hook::Global<int32_t>(0xd97af4);
        reset = 0;
        if (dt > F<int32_t>(e, 0x54) && !e[0x1d8]) {
            reset = 1;
            ListPair_Clear(e, 0, 0);
            int32_t x = gnow + F<int32_t>(e, 0x6c);
            F<int32_t>(e, 0xc4) = x;
            if (!(F<uint32_t>(e, 0xac) & 0x10000) && !force) span = 0;
            else F<int32_t>(e, 0xc4) = x - F<int32_t>(e, 0x54);
        }
        F<int32_t>(e, 0xc8) = dt;
        if (Emitter_Update(e, 0, dt) != 1 || dt <= 0) {
            base = gnow;
        } else if (F<int32_t>(e, 0x1ed) > 0 && F<int32_t>(e, 0x1f1) >= F<int32_t>(e, 0x38)) {
            int32_t burst = F<int32_t>(e, 0x1ed);
            base = gnow;
            if (span > F<int32_t>(e, 0x1f5)) {
                int32_t t6c = F<int32_t>(e, 0x6c);
                F<int32_t>(e, 0x1f5) = burst;
                F<int32_t>(e, 0xc4) = t6c + base;
                F<int32_t>(e, 0x1f1) = 0;
                if (F<uint32_t>(e, 0xac) & 0x40000) {
                    uint32_t& seed = hook::Global<uint32_t>(0x624324);
                    seed = seed * 0x19660d + 0x3c6ef35f;
                    F<int32_t>(e, 0x1f1) = (int32_t)((seed & 0x7fff) << 1) % F<int32_t>(e, 0x38);
                    seed = seed * 0x19660d + 0x3c6ef35f;
                    F<int32_t>(e, 0x1f5) = Ftol((double)((int32_t)(seed & 0x7fff) % F<int32_t>(e, 0x1ed)) * Fl(0x5cb468));
                }
            }
        } else if (span < period || e[0x1d8]) {
            base = gnow;
        } else {
            int32_t n = span / period;
            if (n > F<int32_t>(e, 0x38)) n = F<int32_t>(e, 0x38);
            int32_t t = F<int32_t>(e, 0xc4) + period;
            float accF = F<float>(e, 0xd8);
            double acc = accF;
            float* list = F<float*>(e, 0x10);
            uint8_t* obj = F<uint8_t*>(e, 0xc);
            if (obj) {
                SceneObj_UpdateMatrix(obj);
                acc = accF;
            }
            for (int32_t k = 0; k < F<int32_t>(e, 0x30); ++k) {
                float pos[3], tmp[3];
                uint32_t ac = F<uint32_t>(e, 0xac);
                obj = F<uint8_t*>(e, 0xc);
                if (ac & 0x100000) {
                    for (int i = 0; i < 3; ++i) F<uint32_t>(pos, i * 4) = F<uint32_t>(e, 0x1b0 + i * 4);
                } else if (!obj) {
                    pos[0] = pos[1] = pos[2] = 0.0f;
                } else if (!list) {
                    F<uint32_t>(pos, 0) = F<uint32_t>(obj, 0xfe);
                    X87Copy(&pos[1], obj + 0x102);
                    X87Copy(&pos[2], obj + 0x106);
                } else {
                    uint8_t* g = hook::Global<uint8_t*>(0xd939a0);
                    const float* m = g ? (const float*)(g + 0x3c) : (ac & 0x10) ? (const float*)(obj + 0x14e) : (const float*)(obj + 0xce);
                    float* r = Vec3_Transform(list, 0, tmp, m);
                    for (int i = 0; i < 3; ++i) pos[i] = r[i];
                    list += 3;
                }
                acc = F<float>(e, 0xd8);
                for (int32_t j = n; j > 0; --j) {
                    acc = acc + F<float>(clock, 0x28);
                    accF = (float)acc;
                    if (!(Fl(0x5c85b4) > acc) && acc == acc && F<int32_t>(e, 0xbc)) {
                        --F<int32_t>(e, 0xbc);
                        void** p = (void**)SList_PopFront(F<void**>(e, 0));
                        while (!p) {
                            if (!(F<uint32_t>(e, 0xac) & 4) || !Emitter_SpawnParticle(e)) break;
                            p = (void**)SList_PopFront(F<void**>(e, 0));
                        }
                        if (p) {
                            ++F<int32_t>(e, 0x1f1);
                            SList_PushFront(F<void**>(e, 4), 0, p);
                            Emitter_LaunchParticle(e, 0, (uint8_t*)p[1], pos, t);
                        }
                        acc = (double)accF - Fl(0x5c85b4);
                    }
                    t += period;
                }
                t = F<int32_t>(e, 0xc4) + period;
            }
            int32_t x = F<int32_t>(e, 0x6c) + gnow;
            F<int32_t>(e, 0xc4) = x < 0 ? 0 : x;
            F<float>(e, 0xd8) = (float)acc;
            base = gnow;
        }
    }
    F<int32_t>(e, 0xc0) = F<int32_t>(e, 0x6c) + base;
}
RT3_REPLACE(0x555e50, Emitter_Tick);

}  // namespace rt3
