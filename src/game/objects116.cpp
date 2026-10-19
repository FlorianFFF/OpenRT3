// 3D/2D sound system setup, sample stop/volume, block flag grid in blocks 0x53..0x56 (batch 119). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/containers.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint32_t __fastcall Sound_Get3DProvider(int32_t n);
uint32_t __fastcall Sound_ProviderFlags(uint8_t* s, int, const char* name);
void __fastcall Sound_EnumProviders();
uint8_t* __stdcall Sound_FindProvider(const char* name);
int32_t __fastcall Sound_Open3DProvider(uint8_t* s);
int32_t __fastcall Sound_OpenPreferredProvider(uint8_t* s);
void __fastcall Voices_Release(uint8_t* v, int, uint32_t key);
void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e);
void __fastcall Sound_WaitIdle();

static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }
static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }
static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }
static inline uint32_t* NewZeroed(size_t n) {
    uint32_t* p = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(n);
    if (p) for (size_t k = 0; k < n / 4; ++k) p[k] = 0;
    return p;
}

// 0x5316d0. Flags of the n-th 3D provider (0 without 3D sound).
uint32_t __fastcall Sound_ProviderFlagsAt(uint8_t* s, int, int32_t n) {
    if (!Is3D(s)) return 0;
    const char* name = (const char*)Sound_Get3DProvider(n);
    return Sound_ProviderFlags(hook::Global<uint8_t*>(0xccb9fc), 0, name);
}
RT3_REPLACE(0x5316d0, Sound_ProviderFlagsAt);

// 0x5698a0. Opens 3D sound: the named provider (or a preferred one), the system
// (vtbl[0]); then the sample table (+0x14), voice lookup (+0x10) and the pool of
// 64 voice records (+0xc, 0x38 bytes).
int32_t __fastcall Sound3D_Open(uint8_t* s, int, const char* name) {
    Sound_EnumProviders();
    if (hook::Global<int32_t>(0xd97dc0) <= 0) return 0;
    if (name) {
        F<uint8_t*>(s, 0x28) = Sound_FindProvider(name);
        Sound_Open3DProvider(s);
    }
    if (Sound_OpenPreferredProvider(s) != 1) return 0;
    ((void(__thiscall*)(void*, uint32_t))(*(void***)s)[0])(s, 0);
    if (!F<int32_t>(s, 0x54)) {
        Import<void(__stdcall*)(uint32_t)>(0x5c8404)(*F<uint32_t*>(s, 0x28));
        F<void*>(s, 0x28) = nullptr;
        return 0;
    }
    Hash_Init((HashMap*)(s + 0x14), 0, 0x11, 0x40);
    F<uint32_t*>(s, 0x10) = NewZeroed(0x14);
    F<uint32_t*>(s, 0xc) = NewZeroed(0x1c);
    void* buf = Malloc(0xe00);
    Pool_Init(F<NodePool*>(s, 0xc), 0, buf, 0x38, 0x40, 0);
    Hash_Init(F<HashMap*>(s, 0x10), 0, 0x11, 0x40);
    return 1;
}
RT3_REPLACE(0x5698a0, Sound3D_Open);

// 0x569be0. Retires a finished 3D sample: frees its voice, removes it from the
// table and drops its shared data when unused.
void __fastcall Sound3D_Retire(uint8_t* s, int, uint32_t key, uint32_t* smp) {
    if (smp[2] == 1) F<int32_t>(s, 0x5c)--;
    Voices_Release(s, 0, key);
    Hash_Remove((HashMap*)(s + 0x14), 0, smp[0]);
    F<uint32_t>(smp, 0x44) &= 0xfffffeff;
    uint32_t* info = (uint32_t*)smp[1];
    info[3]--;
    if ((int32_t)((uint32_t*)smp[1])[3] <= 0) {
        SampleQueue_Free(s, 0, (uint32_t*)smp[1]);
        smp[1] = 0;
    }
}
RT3_REPLACE(0x569be0, Sound3D_Retire);

// 0x56a7c0. Allocates n Miles sample handles (records of 0x5c bytes) with a
// handle -> record lookup (+4) and a free pool (+8).
void __fastcall Sound2D_AllocSamples(uint8_t* s, int, int32_t n) {
    F<uint32_t*>(s, 4) = NewZeroed(0x14);
    Hash_Init(F<HashMap*>(s, 4), 0, 7, n);
    uint8_t* recs = (uint8_t*)Malloc(n * 0x5c);
    uint8_t* r = recs;
    for (int32_t i = 0; i < n; ++i, r += 0x5c) {
        uint32_t h = Import<uint32_t(__stdcall*)(uint32_t)>(0x5c8448)(F<uint32_t>(s, 0x2c));
        F<uint32_t>(r, 4) = h;
        Hash_Insert(F<HashMap*>(s, 4), 0, h, (int32_t)r);
    }
    F<uint32_t*>(s, 8) = NewZeroed(0x1c);
    Pool_Init(F<NodePool*>(s, 8), 0, recs, 0x5c, n, 0);
}
RT3_REPLACE(0x56a7c0, Sound2D_AllocSamples);

static inline void VisitAll(uint8_t* s, int32_t mode, int32_t flag) {
    int32_t ctx[4] = { mode, -1 };
    hook::Global<int32_t>(0xccba04) = flag;
    Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
}

// 0x56a880. Stops every 2D sample and resets the table.
void __fastcall Sound2D_StopAll(uint8_t* s) {
    Sound_WaitIdle();
    AilLock();
    VisitAll(s, 1, 0);
    AilUnlock();
    Hash_Init((HashMap*)(s + 0xc), 0, 0x11, 0x40);
    F<int32_t>(s, 0x24) = 0;
}
RT3_REPLACE(0x56a880, Sound2D_StopAll);

// 0x56a8e0. Sets the 2D effects volume (+0x20, clamped to 0..1) and updates the samples.
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol) {
    const float zero = *(const float*)0x5c8598, one = *(const float*)0x5c85b4;
    float v;
    if (zero > vol) v = zero;
    else if (one < vol) v = one;
    else v = vol;
    F<float>(s, 0x20) = v;
    Sound_WaitIdle();
    AilLock();
    if (F<int32_t>(s, 0x24) > 0) {
        int32_t ctx[4] = { 2 };
        hook::Global<int32_t>(0xccba04) = 0;
        Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
    }
    AilUnlock();
}
RT3_REPLACE(0x56a8e0, Sound2D_SetVolume);

// 0x56ac50. Stops sample `key` (end callback cleared) and frees its data.
void __fastcall Sound2D_Stop(uint8_t* s, int, uint32_t key) {
    Sound_WaitIdle();
    AilLock();
    uint8_t* e = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, key);
    if (e) {
        uint32_t h = F<uint32_t>(e, -4);
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8474)(h);
        if (F<int32_t>(e, 8) == 1) F<int32_t>(s, 0x24)--;
        Voices_Release(s, 0, h);
        Hash_Remove((HashMap*)(s + 0xc), 0, key);
        if (void* d = F<void*>(e, 4)) {
            Free(d);
            F<void*>(e, 4) = nullptr;
        }
    }
    AilUnlock();
}
RT3_REPLACE(0x56ac50, Sound2D_Stop);

// 0x56acd0. Sets sample `key`'s volume (scaled by the effects volume).
void __fastcall Sound2D_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol) {
    Sound_WaitIdle();
    AilLock();
    uint8_t* e = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, key);
    if (e) {
        float v = (float)((double)vol * F<float>(s, 0x20));
        Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(F<uint32_t>(e, -4), v, v);
    }
    AilUnlock();
}
RT3_REPLACE(0x56acd0, Sound2D_SetSampleVolume);

// 0x56ae30. Pauses every 2D sample.
void __fastcall Sound2D_PauseAll(uint8_t* s) {
    Sound_WaitIdle();
    AilLock();
    int32_t ctx[4] = { 3 };
    hook::Global<int32_t>(0xccba04) = 0;
    Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
    AilUnlock();
}
RT3_REPLACE(0x56ae30, Sound2D_PauseAll);

void __fastcall Sound2D_FlagVoice(uint8_t* s, int, uint32_t key) {
    uint8_t* v = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    uint8_t* smp = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, F<uint32_t>(v, 8));
    if (smp) F<uint32_t>(smp, 0x44) |= 0x100;
}
RT3_REPLACE(0x56ae80, Sound2D_FlagVoice);

// 0x56aee0. Sound mixer object constructor.
uint8_t* __fastcall Mixer_Construct(uint8_t* m) {
    for (int k = 1; k < 8; ++k) F<int32_t>(m, k * 4) = 0;
    F<uint32_t>(m, 0) = 0x5de4d4;
    F<uint32_t>(m, 0x20) = 0x3f800000;
    F<int32_t>(m, 0x24) = 0;
    F<int32_t>(m, 0x2c) = 0;
    F<int32_t>(m, 0x28) = 8;
    return m;
}
RT3_REPLACE(0x56aee0, Mixer_Construct);

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

// 0x56af70. Block flag grid for a w x h world area (cells +0/+4; 4x4 blocks +0x14).
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h) {
    for (int k = 0; k < 16; ++k) F<int32_t>(g, 0x14 + k * 4) = 0;
    F<float>(g, 8) = w;
    F<float>(g, 0xc) = h;
    F<int32_t>(g, 0) = CellOf(w);
    F<int32_t>(g, 4) = CellOf(h);
    F<int32_t>(g, 0x10) = 0;
    return g;
}
RT3_REPLACE(0x56af70, BlockGrid_Init);

void __fastcall BlockGrid_Free(uint8_t* g) {
    for (int k = 0; k < 16; ++k)
        if (void* p = F<void*>(g, 0x14 + k * 4)) Free(p);
}
RT3_REPLACE(0x56afc0, BlockGrid_Free);

// 0x56b000 / 0x56b070. Low nibble of cell (x, y) (block (x/4, y/4), 8-byte cells +0xa).
static inline uint8_t* Cell(uint8_t* g, int32_t a, int32_t b) {
    uint8_t* blk = F<uint8_t*>(g, 0x14 + (b / 4 + (a / 4) * 4) * 4);
    if (!blk) return nullptr;
    return blk + (b % 4 + (a % 4) * 4) * 8 + 0xa;
}
void __fastcall BlockGrid_Set(uint8_t* g, int, int32_t, int32_t a, int32_t b, uint8_t v) {
    if (uint8_t* c = Cell(g, a, b)) *c = (uint8_t)((*c & 0xf0) | (v & 0xf));
}
RT3_REPLACE(0x56b000, BlockGrid_Set);
uint8_t __fastcall BlockGrid_Get(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* c = Cell(g, a, b);
    return c ? (uint8_t)(*c & 0xf) : 0;
}
RT3_REPLACE(0x56b070, BlockGrid_Get);

} // namespace rt3
