// Ambient sound start / 3D provider switch / sound channel commands in blocks 0x40..0x55 (batch 150). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __fastcall Sound_ListenerIdle(uint8_t* s);
void __stdcall AmbientSound_Ensure(int32_t i);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Sound_ListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
void __fastcall Sound_ListenerPos(uint8_t* s, int, float x, float y, float z);
void* __fastcall Engine_Renderer(void* e);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name);
int32_t __fastcall Channel_Check(uint8_t* c);
void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist);

static inline uint8_t* Snd() { return hook::Global<uint8_t*>(0x6d402c); }

// 0x40b830. Starts the ambient sounds once the listener is idle: the 8 loops (with
// the cache off meanwhile), random start delays for the 16 one-shots (table
// 0x5ed088, 0x1b bytes: delay = rand(1000) x max / 2000), a default listener and
// the renderer flag +0x3698 = 0x30; sets 0x62b248.
void __fastcall Ambient_Start(void*) {
    if (Sound_ListenerIdle(Snd())) return;
    F<int32_t>(F<uint8_t*>(Snd(), 0), 0x14) = 0;
    for (int32_t i = 0; i < 8; ++i) AmbientSound_Ensure(i);
    for (uint32_t a = 0x5ed08c; a < 0x5ed23c; a += 0x1b) {
        int32_t r = Rand(0, 1000);
        int32_t max = *(const int32_t*)(a - 4);
        int32_t v;
        __asm {
            fild r
            fimul max
            fmul qword ptr ds:[0x5c8628]
            fdiv qword ptr ds:[0x5c87f8]
            mov eax, 0x5a10d0
            call eax
            mov v, eax
        }
        *(int32_t*)a = v;
    }
    Sound_ListenerDir(Snd(), 0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    Sound_ListenerPos(Snd(), 0, 0.0f, 0.0f, 0.0f);
    F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x3698) = 0x30;
    F<int32_t>(F<uint8_t*>(Snd(), 0), 0x14) = 1;
    hook::Global<int32_t>(0x62b248) = 1;
}
RT3_REPLACE(0x40b830, Ambient_Start);

// 0x532230. Restarts the sound system with 3D provider number n (if 3D is up).
int32_t __fastcall Sound_SelectProvider(uint8_t* snd, int, int32_t n) {
    if (!hook::Global<void*>(0xccb9fc)) return 0;
    return Sound_Restart(snd, 0, (const char*)Sound_Get3DProvider(n));
}
RT3_REPLACE(0x532230, Sound_SelectProvider);

// 0x5544b0. Sound channel command: 1 = play (count +0xc; resumes a paused sample,
// else starts name +0x10 at +0x114.. with volume +0x128, looping when the repeat
// count +0x130 is -1, otherwise consuming one), 2 = stop. +0x13c = active.
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count) {
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

} // namespace rt3
