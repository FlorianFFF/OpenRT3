// 3D / 2D sample playback and teardown / terrain point grid in blocks 0x56 (batch 147). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include "core/containers.h"
#include <stdint.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint8_t* __fastcall Sound3D_PrepareSample(uint8_t* s, int, const char* name, int32_t prio, float x, float y, float z);
uint8_t* __fastcall Sound_PrepareSample(uint8_t* s, int, const char* name, int32_t load);
void __fastcall Sound_WaitIdle();
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z);
int32_t __fastcall Sound_RegisterEos(uint8_t* s, int, uint32_t sample, int32_t kind);
void __fastcall Sound3D_Close(uint8_t* s);
void __fastcall Sound2D_StopAll(uint8_t* s);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Voices_Free(uint8_t* v);
void __fastcall Block_HeightRange(uint8_t* o, int, int32_t row, int32_t col, uint8_t* t);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);

static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }
static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }
static inline uint8_t* Snd() { return *hook::Global<uint8_t**>(0x6d402c); }

// Registers the voice res (key res[0]) in hash h under the Miles lock.
static void Register(HashMap* h, uint32_t* res) {
    Sound_WaitIdle();
    AilLock();
    Hash_Insert(h, 0, res[0], (int32_t)res);
    AilUnlock();
}

// 0x56a520. Plays 3D sound `name` at (x, y, z) (transformed to listener space
// unless the origin): volume vol x the 3D master (0x624d5c x +0x58), distances
// maxDist (2048 by default) / x 16, looping when mode == 1 (counted in +0x5c),
// started unless `paused`. Returns the voice key (0 if none).
uint32_t __fastcall Sound3D_Play(uint8_t* s, int, const char* name, float x, float y, float z, float vol, int32_t mode,
                                 int32_t paused, int32_t prio, float maxDist) {
    uint32_t* res = (uint32_t*)Sound3D_PrepareSample(s, 0, name, prio, x, y, z);
    if (!res) return 0;
    uint32_t smp = res[-1];
    Register((HashMap*)(s + 0x14), res);
    F<uint32_t>(res, 0x44) = 0;
    F<uint32_t>(res, 0x50) = 0;
    F<float>(res, 0x14) = y;
    F<int32_t>(res, 0x40) = prio;
    F<float>(res, 0x10) = x;
    F<float>(res, 0x18) = z;
    float px = x, py = y, pz = z;
    const float zero = *(const float*)0x5c8598;
    if (zero != x || zero != y || zero != z) Sound_TransformPoint(Snd(), 0, &px, &py, &pz);
    float d = maxDist > zero ? maxDist : *(const float*)0x5cf0e8;
    F<float>(res, 0x30) = d;
    F<float>(res, 0x34) = (float)((double)d * *(const float*)0x5c8948);
    F<float>(res, 0x28) = vol;
    F<int32_t>(res, 8) = mode;
    F<int32_t>(res, 0xc) = paused;
    Import<void(__stdcall*)(uint32_t, void*)>(0x5c843c)(smp, F<void*>((uint8_t*)res[1], 0x10));
    int32_t loop = 1;
    if (mode == 1) {
        ++F<int32_t>(s, 0x5c);
        loop = 0;
    }
    Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c8438)(smp, loop);
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c83f0)(smp, Bits(px), Bits(py), Bits(pz));
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c8424)(smp, 0);
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c8434)(smp, 0);
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8428)(smp, F<uint32_t>(res, 0x34), F<uint32_t>(res, 0x30));
    float v = (float)((double)*(const float*)0x624d5c * F<float>(s, 0x58) * F<float>(res, 0x28));
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c842c)(smp, Bits(v));
    Sound_RegisterEos(Snd(), 0, smp, 1);
    if (!paused) Import<void(__stdcall*)(uint32_t)>(0x5c8430)(smp);
    F<uint32_t>(res, 0x4c) = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    return res[0];
}
RT3_REPLACE(0x56a520, Sound3D_Play);

// 0x56a740. Destructor of the 3D sound system (vtable 0x5de4cc, base 0x5de460).
void __fastcall Sound3D_Destruct(uint8_t* s) {
    F<uint32_t>(s, 0) = 0x5de4cc;
    Sound3D_Close(s);
    Hash_DestroyThunk((HashMap*)(s + 0x14));
    F<uint32_t>(s, 0) = 0x5de460;
    Voices_Free(s);
}
RT3_REPLACE(0x56a740, Sound3D_Destruct);

// 0x56ad20. Plays 2D sound `name` (or the given data) at volume vol x the 2D master
// (+0x20) and pan, looping when mode == 1 (counted in +0x24), started unless
// `paused`. Returns the voice key (0 if none).
uint32_t __fastcall Sound2D_Play(uint8_t* s, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data) {
    uint32_t* res = (uint32_t*)Sound_PrepareSample(s, 0, name, data == nullptr);
    if (!res) return 0;
    uint32_t smp = res[-1];
    Register((HashMap*)(s + 0xc), res);
    F<uint32_t>(res, 0x50) = 0;
    F<uint32_t>(res, 0x44) = 0;
    F<float>(res, 0x28) = vol;
    F<uint32_t>(res, 0x2c) = pan;
    F<int32_t>(res, 8) = mode;
    F<int32_t>(res, 0xc) = paused;
    Import<void(__stdcall*)(uint32_t)>(0x5c83b0)(smp);
    if (!data) data = (void*)res[1];
    Import<void(__stdcall*)(uint32_t, void*, int32_t)>(0x5c8480)(smp, data, 0);
    float v = (float)((double)vol * F<float>(s, 0x20));
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8470)(smp, Bits(v), Bits(v));
    float v2 = (float)((double)vol * F<float>(s, 0x20));
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8458)(smp, Bits(v2), pan);
    int32_t loop = 1;
    if (mode == 1) {
        ++F<int32_t>(s, 0x24);
        loop = 0;
    }
    Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c845c)(smp, loop);
    Sound_RegisterEos(Snd(), 0, smp, 0);
    if (!paused) Import<void(__stdcall*)(uint32_t)>(0x5c8478)(smp);
    F<uint32_t>(res, 0x4c) = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    return res[0];
}
RT3_REPLACE(0x56ad20, Sound2D_Play);

// 0x56af20. Destructor of the 2D sound system (vtable 0x5de4d4, base 0x5de460).
void __fastcall Sound2D_Destruct(uint8_t* s) {
    F<uint32_t>(s, 0) = 0x5de4d4;
    if (F<void*>(s, 0x2c)) {
        Sound2D_StopAll(s);
        Hash_Destroy((HashMap*)(s + 0xc));
        ((void(__thiscall*)(void*))(*(void***)s)[1])(s);
        F<void*>(s, 0x2c) = nullptr;
    }
    Hash_DestroyThunk((HashMap*)(s + 0xc));
    F<uint32_t>(s, 0) = 0x5de460;
    Voices_Free(s);
}
RT3_REPLACE(0x56af20, Sound2D_Destruct);

// Stores point k of cell (cell +2 + k) as its height above the block's range
// (block +2 middle, +6 spread), from terrain point (x, z) + the grid offset.
static void StorePoint(uint8_t* g, uint8_t* t, uint8_t* blk, uint8_t* cell, int32_t k, float px, float pz) {
    double h = Terrain_HeightAt(t, 0, (float)((double)px + F<float>(g, 8)), (float)((double)pz + F<float>(g, 0xc)));
    if (fabs((double)F<float>(blk, 6)) > *(const double*)0x5c88a0) {
        double v = (h - F<float>(blk, 2)) / F<float>(blk, 6);
        int32_t r;
        __asm {
            fld v
            mov eax, 0x5a10d0
            call eax
            mov r, eax
        }
        cell[2 + k] = (uint8_t)r;
    } else {
        cell[2 + k] = 0;
    }
}

// Terrain vtable slot 1: point k of kind `kind` in cell (x, z) -> (px, pz).
static inline void PointPos(uint8_t* t, int32_t x, int32_t z, int32_t k, int32_t kind, float* px, float* pz) {
    ((void(__thiscall*)(void*, int32_t, int32_t, int32_t, int32_t, float*, float*, int32_t, int32_t))(*(void***)t)[1])(
        t, x, z, k, kind, px, pz, 0, 0);
}

// 0x56b580. Adds a point of `kind` to cell (x, z) of the 4x4-block grid (blocks +0x14,
// created on demand with their height range and the terrain region refreshed;
// 16 cells of {flags (count in bits 4-6, max 6), kind, heights}), counting +0x10.
void __fastcall Grid_AddPoint(uint8_t* g, int, uint8_t* t, int32_t x, int32_t z, uint8_t kind) {
    int32_t bx = x / 4, bz = z / 4;
    uint8_t** slot = &F<uint8_t*>(g, 0x14 + (bz + bx * 4) * 4);
    if (!*slot) {
        uint8_t* blk = (uint8_t*)Malloc(0x8a);
        *slot = blk;
        for (int k = 0; k < 0x8a; ++k) blk[k] = 0;
        Block_HeightRange(g, 0, bx, bz, t);
        int32_t x0 = F<int32_t>(g, 0) + bx * 4, z0 = F<int32_t>(g, 4) + bz * 4;
        ((void(__thiscall*)(void*, int32_t, int32_t, int32_t, int32_t))(*(void***)t)[0])(t, x0, z0, x0 + 3, z0 + 3);
    }
    uint8_t* blk = *slot;
    uint8_t* cell = blk + 0xa + (z % 4 + (x % 4) * 4) * 8;
    uint8_t flags = cell[0];
    cell[1] = kind;
    if ((uint8_t)(flags & 0x70) >= 0x60) return;
    float px, pz;
    PointPos(t, x, z, (flags >> 4) & 7, kind, &px, &pz);
    StorePoint(g, t, blk, cell, (cell[0] >> 4) & 7, px, pz);
    ++F<uint16_t>(blk, 0);
    uint8_t f = cell[0];
    cell[0] = (uint8_t)(((((f & 0xf0) + 0x10) ^ f) & 0x70) ^ f);
    ++F<int32_t>(g, 0x10);
}
RT3_REPLACE(0x56b580, Grid_AddPoint);

// 0x56b770. Recomputes the height range of every block and the heights of all
// their points.
void __fastcall Grid_RefreshPoints(uint8_t* g, int, uint8_t* t) {
    for (int32_t row = 0; row < 4; ++row)
        for (int32_t col = 0; col < 4; ++col) {
            uint8_t* blk = F<uint8_t*>(g, 0x14 + (row * 4 + col) * 4);
            if (!blk) continue;
            Block_HeightRange(g, 0, row, col, t);
            uint8_t* cell = blk + 0xa;
            for (int32_t sx = 0; sx < 4; ++sx)
                for (int32_t sz = 0; sz < 4; ++sz, cell += 8) {
                    uint8_t n = (cell[0] >> 4) & 7;
                    for (uint8_t k = 0; k < n; n = (cell[0] >> 4) & 7, ++k) {
                        float px, pz;
                        PointPos(t, sx + row * 4, sz + col * 4, k, cell[1], &px, &pz);
                        StorePoint(g, t, blk, cell, k, px, pz);
                    }
                }
        }
}
RT3_REPLACE(0x56b770, Grid_RefreshPoints);

} // namespace rt3
