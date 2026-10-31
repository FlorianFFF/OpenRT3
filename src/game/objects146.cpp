// Ambient / horn / UI / whistle sounds and the sound system ctor, dtor and restart in blocks 0x40..0x53 (batch 149). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data);
uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
uint8_t __fastcall Channel_Playing(uint8_t* c);
void* __fastcall Obj_Train390(void* o);
void* __fastcall Obj_CityRef(void* o);
int32_t IsMultiplayer();
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate);
int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name);
void __fastcall Sound_SetListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
void __fastcall Sound_SetListenerPos(uint8_t* s, int, float x, float y, float z);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol);
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
int32_t __fastcall SoundCache_Init(uint8_t* s, int, int32_t capacity);
void __fastcall SoundCache_ClearThunk(uint8_t* s);

static inline uint8_t* Snd() { return hook::Global<uint8_t*>(0x6d402c); }
static inline float FromBits(uint32_t u) { return *(float*)&u; }
static inline bool SoundUp(uint8_t* snd) { return F<int32_t>(snd, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

// 0x40b760. Keeps ambient sound i (table 0x5ed238, 32 bytes: key, ..., +0x10 prio,
// +0x14 sample, +0x1c name) alive: re-finds its sample, else starts it again paused
// and looping at volume 0.
void __stdcall AmbientSound_Ensure(int32_t i) {
    uint8_t* e = (uint8_t*)0x5ed238 + i * 32;
    if (F<uint32_t>(e, 0)) {
        uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, F<uint32_t>(e, 0), 1);
        F<uint8_t*>(e, 0x14) = p;
        if (p) {
            F<uint32_t>(p, 0x44) = 0;
            if (F<void*>(e, 0x14)) return;
        }
    }
    F<uint32_t>(e, 0) = Sound_Play3D(Snd(), 0, F<const char*>(e, 0x1c), 0.0f, 0.0f, 0.0f, 0.0f, 1, 1,
                                     F<int32_t>(e, 0x10), 0.0f);
}
RT3_REPLACE(0x40b760, AmbientSound_Ensure);

// 0x45d2e0. Updates the vehicle's horn sound (+0x346) at its position, or starts a
// requested one: crash (+0x33e, scale 2), breakdown (+0x342, 1.5) or its engine
// whistle (+0x33a while its channel +0x336 plays; whistle 0x5f3a84 by type +0x87);
// distances 92 / 5888 x scale. Clears the requests.
void __fastcall Vehicle_UpdateHorn(uint8_t* v, int, int32_t) {
    if (F<uint8_t*>(v, 0x336)) {
        uint32_t x, y, z;
        SceneObj_GetPos(v, 0, &x, &y, &z);
        if (uint32_t key = F<uint32_t>(v, 0x346)) {
            if (uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 1)) {
                F<uint32_t>(p, 0x10) = x;
                F<uint32_t>(p, 0x14) = y;
                F<uint32_t>(p, 0x18) = z;
                F<uint32_t>(p, 0x44) |= 2;
            } else {
                F<uint32_t>(v, 0x346) = 0;
            }
        } else if (!F<int32_t>(F<uint8_t*>(v, 0x336), 0x138)) {
            bool go;
            if (Channel_Playing(F<uint8_t*>(v, 0x336)) == 1 && F<int32_t>(v, 0x33a)) go = true;
            else go = F<int32_t>(v, 0x33e) || F<int32_t>(v, 0x342);
            if (go) {
                char name[0x100];
                float scale;
                if (F<int32_t>(v, 0x33e)) {
                    memcpy(name, (const void*)0x5cb6b4, 16);
                    scale = 2.0f;
                } else if (F<int32_t>(v, 0x342)) {
                    memcpy(name, (const void*)0x5cb6a0, 20);
                    scale = 1.5f;
                } else {
                    scale = 1.0f;
                    uint8_t* type = (uint8_t*)Obj_CityRef(Obj_Train390(v));
                    const char* w = ((const char* const*)0x5f3a84)[F<int32_t>(type, 0x87)];
                    char* d = name;
                    while ((*d++ = *w++) != 0) {}
                }
                uint32_t key = Sound_Play3D(Snd(), 0, name, FromBits(x), FromBits(y), FromBits(z), 1.0f, 0, 1, 2, 0.0f);
                F<uint32_t>(v, 0x346) = key;
                if (key) {
                    uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 1);
                    F<float>(p, 0x30) = (float)((double)scale * *(const double*)0x5c8c48);
                    F<int32_t>(p, 0xc) = 0;
                    F<float>(p, 0x34) = (float)((double)scale * *(const double*)0x5cb698);
                    F<uint32_t>(p, 0x44) |= 0x18;
                }
            }
        }
    }
    F<int32_t>(v, 0x33a) = 0;
    F<int32_t>(v, 0x33e) = 0;
    F<int32_t>(v, 0x342) = 0;
}
RT3_REPLACE(0x45d2e0, Vehicle_UpdateHorn);

// 0x45ea20. Plays interface sound id (2..64, table 0x5f0828 {volume, name}) at pan,
// unless muted (0xccba14).
RT3_NOINLINE void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan) {
    if (id <= 1 || hook::Global<int32_t>(0xccba14) || id <= 0 || id >= 0x41) return;
    const uint32_t* e = (const uint32_t*)(0x5f0828 + id * 8);
    Sound_Play2D(Snd(), 0, (const char*)e[1], FromBits(e[0]), pan, 0, 0, nullptr);
}
RT3_REPLACE(0x45ea20, Ui_PlaySound);

// 0x4a7070. Blows one of the train's three whistles (0x5f0828 entries 0x3a..0x3c,
// random) n times, 125 ms apart, at the whistle volume option (10 - +0x31d tenths);
// in multiplayer only for the local player's trains (+0x51).
void __fastcall Train_Whistle(uint8_t* o, int, int32_t n) {
    if (n <= 0) return;
    if (IsMultiplayer() && Obj_Get21(hook::Global<void*>(0x6cec78)) != F<int32_t>(o, 0x51)) return;
    int32_t idx = Rand(0, 3) + 0x3a;
    const uint32_t* e = (const uint32_t*)(0x5f0828 + idx * 8);
    int32_t delay = 0;
    for (; n; --n, delay += 0x7d) {
        int32_t level = 10 - F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x31d);
        float vol = (float)((double)level / *(const double*)0x5c85f0 * FromBits(e[0]));
        if (vol <= *(const float*)0x5c8598) continue;
        uint32_t key = Sound_Play2D(Snd(), 0, (const char*)e[1], vol, 0x3f000000, 0, 1, nullptr);
        if (!key) continue;
        uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 0);
        if (!p) continue;
        F<uint32_t>(p, 0x44) |= 0x408;
        F<int32_t>(p, 0x50) = delay;
        F<int32_t>(p, 0xc) = 0;
    }
}
RT3_REPLACE(0x4a7070, Train_Whistle);

// 0x531c00. Restarts the sound system with 3D provider `name` and re-applies the
// listener and the music (engine +0x1146fe x 0.24) / effects (+0x1146fa) volumes.
int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name) {
    if (!hook::Global<void*>(0xccb9fc)) return 0;
    char buf[0x1f8];
    char* d = buf;
    while ((*d++ = *name++) != 0) {}
    Sound_Shutdown(snd, 0, 1);
    if (Sound_Startup(snd, 0, buf) != 1) return 0;
    if (SoundUp(snd)) Sound_SetListenerDir(hook::Global<uint8_t*>(0xccb9fc), 0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    if (SoundUp(snd)) Sound_SetListenerPos(hook::Global<uint8_t*>(0xccb9fc), 0, 0.0f, 0.0f, 0.0f);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    float mv = (float)((double)F<float>(e, 0x1146fe) * *(const double*)0x5c9c80);
    if (SoundUp(snd)) Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, mv);
    float sv = F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fa);
    if (SoundUp(snd)) {
        Sound2D_SetVolume(hook::Global<uint8_t*>(0xccb9f8), 0, sv);
        Sound_SetEffectsVolume(hook::Global<uint8_t*>(0xccb9fc), 0, sv);
    }
    return 0;
}
RT3_REPLACE(0x531c00, Sound_Restart);

// 0x532150. Constructs the sound system: default volumes (1.0), no timer (+0x4c),
// starts it with 3D provider `provider` (-1 = none) and a 1 MB sound cache (+0).
uint8_t* __fastcall Sound_Construct(uint8_t* snd, int, int32_t provider) {
    F<int32_t>(snd, 4) = 0;
    hook::Global<void*>(0xccb9f8) = nullptr;
    hook::Global<void*>(0xccb9fc) = nullptr;
    hook::Global<void*>(0xccba00) = nullptr;
    F<void*>(snd, 0) = nullptr;
    hook::Global<uint32_t>(0x624d5c) = 0x3f800000;
    F<int32_t>(snd, 0x4c) = -1;
    F<int32_t>(snd, 0x50) = 0;
    memset(snd + 8, 0, 0x40);
    F<uint32_t>(snd, 0x44) = 0x3f800000;
    F<uint32_t>(snd, 0x30) = 0x3f800000;
    F<uint32_t>(snd, 0x1c) = 0x3f800000;
    F<uint32_t>(snd, 8) = 0x3f800000;
    F<int32_t>(snd, 0x48) = 0;
    const char* name = provider != -1 ? (const char*)Sound_Get3DProvider(provider) : nullptr;
    if (Sound_Startup(snd, 0, name) == 1) {
        uint8_t* c = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x44);
        if (c) F<int32_t>(c, 0x10) = 0;
        F<uint8_t*>(snd, 0) = c;
        SoundCache_Init(c, 0, 0x100000);
    }
    return snd;
}
RT3_REPLACE(0x532150, Sound_Construct);

// 0x532200. Destroys the sound system: shuts it down (fading) and frees the cache.
void __fastcall Sound_Destruct(uint8_t* snd) {
    if (F<int32_t>(snd, 4) != 1) return;
    Sound_Shutdown(snd, 0, 0);
    if (uint8_t* c = F<uint8_t*>(snd, 0)) {
        SoundCache_ClearThunk(c);
        ((void(__cdecl*)(void*))0x53b080)(c);
        F<void*>(snd, 0) = nullptr;
    }
}
RT3_REPLACE(0x532200, Sound_Destruct);

} // namespace rt3
