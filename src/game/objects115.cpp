// Sound system front-end / provider selection / voice pools / terrain sound grid in blocks 0x53..0x56 (batch 118). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/containers.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint32_t __fastcall Sound_ProviderInfo(uint8_t* s);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
void __fastcall Sound_SetListenerPos(uint8_t* s, int, float x, float y, float z);
void __fastcall Sound_SetListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
void __fastcall Sound_SetSpeakerType(uint8_t* s, int, int32_t type, int32_t noApply);
int32_t __fastcall Sound_SpeakerType();
void __fastcall Lists_Destroy(uint8_t* o);
uint8_t* __stdcall Sound_FindProvider(const char* name);
int32_t __fastcall Sound_Open3DProvider(uint8_t* s);
void __fastcall Sound_WaitIdle();

static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }
static inline uint8_t* Sound3D() { return hook::Global<uint8_t*>(0xccb9fc); }
static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }
static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }

// 0x531620. The active 3D provider's info (or the empty default 0xccba0c).
uint32_t __fastcall Sound_ActiveProvider(uint8_t* s) {
    if (Is3D(s)) return Sound_ProviderInfo(Sound3D());
    return 0xccba0c;
}
RT3_REPLACE(0x531620, Sound_ActiveProvider);

// 0x531640. Index of the active provider among the enumerated ones (the count if absent).
int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s) {
    int32_t i = 0;
    if (!Is3D(s)) return 0;
    const char* cur = (const char*)Sound_ProviderInfo(Sound3D());
    for (const char* p = (const char*)Sound_Get3DProvider(0); p; p = (const char*)Sound_Get3DProvider(++i)) {
        const char* a = cur;
        const char* b = p;
        while (*a == *b && *a) ++a, ++b;
        if (*a == *b) return i;
    }
    return i;
}
RT3_REPLACE(0x531640, Sound_ActiveProviderIndex);

void __fastcall Sound_ListenerPos(uint8_t* s, int, float x, float y, float z) {
    if (Is3D(s)) Sound_SetListenerPos(Sound3D(), 0, x, y, z);
}
RT3_REPLACE(0x531710, Sound_ListenerPos);
void __fastcall Sound_ListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz) {
    if (Is3D(s)) Sound_SetListenerDir(Sound3D(), 0, fx, fy, fz, ux, uy, uz);
}
RT3_REPLACE(0x531730, Sound_ListenerDir);
RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply) {
    if (Is3D(s)) Sound_SetSpeakerType(Sound3D(), 0, type, noApply);
}
RT3_REPLACE(0x531750, Sound_Speakers);
int32_t __fastcall Sound_GetSpeakers(uint8_t* s) { return Is3D(s) ? Sound_SpeakerType() : 0; }
RT3_REPLACE(0x531770, Sound_GetSpeakers);

// 0x535c70. Destroys the per-block sound lists (+0x1681) and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1681(uint8_t* r) {
    if (!F<void*>(r, 0x1681)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                uint8_t* l = F<uint8_t**>(r, 0x1681)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (!l) continue;
                Lists_Destroy(l);
                ((void(__cdecl*)(void*))0x53b080)(l);
                F<uint8_t**>(r, 0x1681)[(F<int32_t>(r, 0x15d9) >> 4) * j + i] = nullptr;
            }
        }
    }
    Free(F<void*>(r, 0x1681));
    F<void*>(r, 0x1681) = nullptr;
}
RT3_REPLACE(0x535c70, Terrain_DestroyCell1681);

// 0x569730. Flags (+8) of the named provider, or of the open one.
uint32_t __fastcall Sound_ProviderFlags(uint8_t* s, int, const char* name) {
    if (!name) return F<uint32_t>(F<uint8_t*>(s, 0x28), 8);
    return F<uint32_t>(Sound_FindProvider(name), 8);
}
RT3_REPLACE(0x569730, Sound_ProviderFlags);

// 0x569830. Opens the first available of the preferred 3D providers.
int32_t __fastcall Sound_OpenPreferredProvider(uint8_t* s) {
    if (F<void*>(s, 0x28)) return 1;
    static const uint32_t names[4] = { 0x5d14d8, 0x5de468, 0x5de494, 0x5de4b4 };
    for (int i = 0; i < 4; ++i) {
        F<uint8_t*>(s, 0x28) = Sound_FindProvider((const char*)names[i]);
        Sound_Open3DProvider(s);
        if (F<void*>(s, 0x28)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x569830, Sound_OpenPreferredProvider);

// 0x569ae0. Sets the effects volume (+0x58, clamped to 0..1) and updates all samples.
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol) {
    int32_t ctx[4] = { 2 };
    const float zero = *(const float*)0x5c8598, one = *(const float*)0x5c85b4;
    float v;
    if (zero > vol) v = zero;
    else if (one < vol) v = one;
    else v = vol;
    F<float>(s, 0x58) = v;
    Sound_WaitIdle();
    AilLock();
    if (F<int32_t>(s, 0x5c) > 0) {
        hook::Global<int32_t>(0xccba04) = 1;
        Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, ctx);
    }
    AilUnlock();
}
RT3_REPLACE(0x569ae0, Sound_SetEffectsVolume);

// 0x569b70. Frees a queued sample entry and recycles it in the pool (+0xc).
void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e) {
    ((void(__cdecl*)(void*))0x5a1145)((void*)e[4]);
    e[4] = 0;
    Hash_Remove(F<HashMap*>(q, 0x10), 0, e[0]);
    uint8_t* pool = F<uint8_t*>(q, 0xc);
    if (F<int32_t>(pool, 0xc) && (uint32_t)e >= F<uint32_t>(pool, 4) && (uint32_t)e < F<uint32_t>(pool, 8)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        e[0] = fl[0];
        fl[0] = (uint32_t)e;
        fl[2]++;
        if (!fl[1]) fl[1] = (uint32_t)e;
        return;
    }
    ((void(__cdecl*)(void*))0x53b080)(e);
}
RT3_REPLACE(0x569b70, SampleQueue_Free);

// 0x569ca0. Pauses every sample (visitor mode 3).
void __fastcall Sound_PauseAll(uint8_t* s) {
    int32_t ctx[4] = { 3 };
    Sound_WaitIdle();
    AilLock();
    hook::Global<int32_t>(0xccba04) = 1;
    Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, ctx);
    F<int32_t>(s, 0x60) = 0;
    AilUnlock();
}
RT3_REPLACE(0x569ca0, Sound_PauseAll);

// 0x569cf0. Flags the sample of voice `key` for restart (0x100).
void __fastcall Sound_FlagVoice(uint8_t* s, int, uint32_t key) {
    uint8_t* v = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    uint8_t* smp = (uint8_t*)Hash_Get((HashMap*)(s + 0x14), 0, F<uint32_t>(v, 8));
    if (smp) F<uint32_t>(smp, 0x44) |= 0x100;
}
RT3_REPLACE(0x569cf0, Sound_FlagVoice);

// 0x56a6e0. 2D sound system constructor.
uint8_t* __fastcall Sound2D_Construct(uint8_t* s) {
    static const uint8_t zero[] = { 4, 8, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x54, 0x28, 0x5c, 0x60, 0xc, 0x10,
                                    0x2c, 0x30, 0x34, 0x38, 0x3c, 0x40, 0x48, 0x50 };
    for (uint8_t off : zero) F<int32_t>(s, off) = 0;
    F<uint32_t>(s, 0) = 0x5de4cc;
    F<uint32_t>(s, 0x58) = 0x3f800000;
    F<uint32_t>(s, 0x44) = 0x3f800000;
    F<uint32_t>(s, 0x4c) = 0x3f800000;
    return s;
}
RT3_REPLACE(0x56a6e0, Sound2D_Construct);

// 0x56a770. Opens the 22 kHz 16-bit stereo digital driver (+0x2c) and initialises.
int32_t __fastcall Sound2D_Open(uint8_t* s) {
    Import<int32_t(__stdcall*)(uint32_t, int32_t)>(0x5c8444)(0, 100);
    void* drv = Import<void*(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c8440)(0x5622, 0x10, 2, 0);
    F<void*>(s, 0x2c) = drv;
    if (!drv) return 0;
    Hash_Init((HashMap*)(s + 0xc), 0, 0x11, 0x40);
    ((void(__thiscall*)(void*, uint32_t))(*(void***)s)[0])(s, F<uint32_t>(s, 0x28));
    return 1;
}
RT3_REPLACE(0x56a770, Sound2D_Open);

} // namespace rt3
