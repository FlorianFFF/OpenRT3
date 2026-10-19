// Engine sound pitch / wheel animation / string helpers / config holder in blocks 0x45..0x55 (batch 109). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/containers.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Channel_Check(uint8_t* c);
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o);
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt);
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

// 0x45d260. Sets the engine sound (+0x336) pitch (sample base rate x scale, at
// least 8000 Hz) and volume, stopping a looping sample; returns the rate (0 if none).
int32_t __fastcall Vehicle_SetEngineSound(uint8_t* v, int, float scale, uint32_t volume) {
    uint8_t* ch = F<uint8_t*>(v, 0x336);
    if (!ch) return 0;
    uint8_t* s = (uint8_t*)Channel_Check(ch);
    if (!s) return 0;
    float base = (float)F<int32_t>(F<uint8_t*>(s, 4), 0x20);
    int32_t rate = (int32_t)((double)base * scale);
    if (rate < 0x1f40) rate = 0x1f40;
    Channel_SetRate(F<uint8_t*>(v, 0x336), 0, rate);
    Channel_SetVolume(F<uint8_t*>(v, 0x336), 0, volume);
    if (F<int32_t>(F<uint8_t*>(v, 0x336), 0x138) == 1) Channel_SetLoop(F<uint8_t*>(v, 0x336), 0, 0);
    return rate;
}
RT3_REPLACE(0x45d260, Vehicle_SetEngineSound);

// 0x4a7810. Restarts the animated sub-objects of the train's first car (8 per part):
// the special-named ones at full speed, the others paused (+0x1d8).
void __fastcall Train_ResetCarAnimations(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x41);
    uint8_t* car = id != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id) : nullptr;
    for (int32_t i = 0; i < F<int32_t>(car, 0x31a); ++i)
        for (int k = 0; k < 8; ++k) {
            uint8_t* sub = F<uint8_t*>(F<uint8_t*>(car, 0x316), 0x10 + i * 0xc0 + k * 4);
            if (!sub) continue;
            uint8_t* a = F<uint8_t*>(sub, 0x60);
            if (a && Obj_NameIs5ce4f4(a)) {
                sub[0x1d8] = 0;
                F<uint32_t>(sub, 0xcc) = 0x3f800000;
                Timer_SetFrameTime(sub, 0, 1.0f);
            } else {
                sub[0x1d8] = 1;
            }
        }
}
RT3_REPLACE(0x4a7810, Train_ResetCarAnimations);

void __fastcall ListPair_Reset(uint8_t* o) {
    F<int32_t>(o, 0xc4) = 0;
    F<int32_t>(o, 0xc0) = 0;
    ListPair_Clear(o, 0, 0);
}
RT3_REPLACE(0x554d50, ListPair_Reset);

char* __fastcall StrCopy(char* dst, const char* src) {
    char* p = dst;
    *p = *src;
    while (*src) *++p = *++src;
    return dst;
}
RT3_REPLACE(0x557230, StrCopy);

// 0x557250. Whether the string is empty or whitespace only.
int32_t __fastcall Str_IsBlank(const char* s) {
    if (*s)
        while (IsSpace(*s)) {
            if (!*++s) break;
        }
    return *s ? 0 : 1;
}
RT3_REPLACE(0x557250, Str_IsBlank);

// 0x5572e0. Trims trailing whitespace in place.
void __fastcall Str_TrimRight(char* s) {
    int32_t n = 0;
    while (s[n]) ++n;
    if (n <= 0) return;
    char* p = s + n - 1;
    if (*p)
        while (IsSpace(*p)) {
            char prev = p[-1];
            --p;
            if (!prev) break;
        }
    p[1] = 0;
}
RT3_REPLACE(0x5572e0, Str_TrimRight);

void __fastcall Set_6257e0(uint32_t a, uint32_t b) {
    hook::Global<uint32_t>(0x6257e0) = a;
    hook::Global<uint32_t>(0x6257e4) = b;
}
RT3_REPLACE(0x557320, Set_6257e0);

uint8_t* __fastcall Holder_Init(uint8_t* h, int, uint32_t a, uint32_t b) {
    F<uint32_t>(h, 0x10) = b;
    F<uint32_t>(h, 0xc) = a;
    F<int32_t>(h, 8) = 0;
    return h;
}
RT3_REPLACE(0x557490, Holder_Init);

// 0x5574b0. Frees the two pools (+4, +0; each with a buffer +4) and the block +8.
static inline void FreePool(uint8_t* o, uint32_t off) {
    if (!F<uint8_t*>(o, off)) return;
    if (F<void*>(F<uint8_t*>(o, off), 4)) Free(F<void*>(F<uint8_t*>(o, off), 4));
    if (NodePool* p = F<NodePool*>(o, off)) {
        Pool_Free(p);
        ExeDelete(p);
        F<void*>(o, off) = nullptr;
    }
}
void __fastcall Holder_Free(uint8_t* h) {
    FreePool(h, 4);
    FreePool(h, 0);
    if (void* p = F<void*>(h, 8)) {
        ExeDelete(p);
        F<void*>(h, 8) = nullptr;
    }
}
RT3_REPLACE(0x5574b0, Holder_Free);
void __fastcall Holder_Destruct(uint8_t* h) { Holder_Free(h); }
RT3_REPLACE(0x557540, Holder_Destruct);

void __fastcall Set_d973b0(uint32_t v) { hook::Global<uint32_t>(0xd973b0) = v; }
RT3_REPLACE(0x557920, Set_d973b0);

// 0x557930 / 0x557970. ANSI <-> UTF-16 conversion into a buffer of n characters
// (n = -1: the source length + 1), always terminated.
wchar_t* __fastcall Str_ToWide(wchar_t* dst, const char* src, int32_t n) {
    if (n == -1) {
        int32_t k = 0;
        while (src[k]) ++k;
        n = k + 1;
    }
    Import<int32_t(__stdcall*)(uint32_t, uint32_t, const char*, int32_t, wchar_t*, int32_t)>(0x5c81e8)(0, 0, src, -1, dst,
                                                                                                    n - 1);
    dst[n - 1] = 0;
    return dst;
}
RT3_REPLACE(0x557930, Str_ToWide);
char* __fastcall Str_FromWide(char* dst, const wchar_t* src, int32_t n) {
    if (n == -1) n = (int32_t)((size_t(__cdecl*)(const wchar_t*))0x5a3e74)(src) + 1;
    Import<int32_t(__stdcall*)(uint32_t, uint32_t, const wchar_t*, int32_t, char*, int32_t, const char*, int32_t*)>(
        0x5c81e4)(0, 0, src, -1, dst, n - 1, nullptr, nullptr);
    dst[n - 1] = 0;
    return dst;
}
RT3_REPLACE(0x557970, Str_FromWide);

} // namespace rt3
