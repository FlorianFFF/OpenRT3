// Music start / clipboard paste / system information in blocks 0x53..0x55 (batch 112). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <intrin.h>
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
int32_t __fastcall Music_IsPlaying(uint8_t* m);
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size);
void __fastcall EditBox_InsertCursor(uint8_t* w, int, const char* text, int32_t pos);

static inline size_t Len(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }
static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }

// 0x531d30. Starts a music file at the configured music volume (engine +0x1146fe).
int32_t __fastcall Sound_PlayMusic(uint8_t* s, int, const char* file, void* onEnd) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return 0;
    float vol = (float)((double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fe) * *(const double*)0x5c9c80);
    Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, vol);
    uint8_t* m = hook::Global<uint8_t*>(0xccba00);
    if (!(F<float>(m, 0x104) > *(const float*)0x5c8598)) return 0;
    if (Music_IsPlaying(m) == 1) Music_Stop(hook::Global<uint8_t*>(0xccba00), 0, 0);
    return Music_Play(hook::Global<uint8_t*>(0xccba00), 0, file,
                      F<void*>(hook::Global<uint8_t*>(0xccb9f8), 0x2c), onEnd);
}
RT3_REPLACE(0x531d30, Sound_PlayMusic);

// 0x55be40. Pastes the clipboard text (cut to the box limit +0xaa - 2) into the
// emptied edit text `dst`, cursor after it.
void __fastcall EditBox_Paste(uint8_t* w, int, char* dst) {
    char buf[0x1388];
    if (!Clipboard_GetText(buf, 0x1388)) return;
    int32_t limit = F<int32_t>(w, 0xaa);
    int32_t cap = limit > 0x1388 ? 0x1388 : limit;
    buf[cap - 2 < 0 ? 0 : cap - 2] = 0;
    int32_t len = (int32_t)Len(buf);
    dst[0] = 0;
    F<int32_t>(w, 0xd8) = 0;
    int32_t dlen = (int32_t)Len(dst);
    if (len <= 0 || dlen + len + 1 >= F<int32_t>(w, 0xaa)) return;
    F<char*>(w, 0xba) = (char*)Malloc(Len(dst) + len + 10);
    Copy(dst, buf);
    Copy(F<char*>(w, 0xba), buf);
    F<int32_t>(w, 0xd8) += len;
    EditBox_InsertCursor(w, 0, dst, F<int32_t>(w, 0xd8));
}
RT3_REPLACE(0x55be40, EditBox_Paste);

// 0x55d2f0. CPU clock in MHz: time-stamp counter ticks over one second of the
// performance counter.
uint64_t __fastcall SysInfo_CpuMhz() {
    int64_t freq, t;
    auto qpc = Import<int32_t(__stdcall*)(int64_t*)>(0x5c81e0);
    if (!Import<int32_t(__stdcall*)(int64_t*)>(0x5c81dc)(&freq)) return 0;
    qpc(&t);
    int64_t end = t + freq;
    uint64_t tsc0 = __rdtsc();
    do {
        qpc(&t);
    } while (t <= end);
    uint64_t tsc1 = __rdtsc();
    return ((uint64_t(__stdcall*)(uint64_t, uint64_t))0x5a4240)(tsc1 - tsc0, 1000000);
}
RT3_REPLACE(0x55d2f0, SysInfo_CpuMhz);

int32_t __fastcall SysInfo_Zero() { return 0; }
RT3_REPLACE(0x55d390, SysInfo_Zero);
void __fastcall SysInfo_Nop() {}
RT3_REPLACE(0x55d3a0, SysInfo_Nop);
const char* __fastcall SysInfo_CpuVendor() { return (const char*)0xd973f8; }
RT3_REPLACE(0x55d3b0, SysInfo_CpuVendor);
const char* __fastcall SysInfo_CpuName() { return (const char*)0xd97405; }
RT3_REPLACE(0x55d3c0, SysInfo_CpuName);
uint32_t __fastcall SysInfo_CpuSignature() { return hook::Global<uint32_t>(0xd9743b); }
RT3_REPLACE(0x55d3d0, SysInfo_CpuSignature);
uint32_t __fastcall SysInfo_HasFeature0() { return hook::Global<uint32_t>(0xd97433) & 1; }
RT3_REPLACE(0x55d3e0, SysInfo_HasFeature0);
uint32_t __fastcall SysInfo_HasFeature1() { return (hook::Global<uint32_t>(0xd97433) >> 1) & 1; }
RT3_REPLACE(0x55d3f0, SysInfo_HasFeature1);
uint32_t __fastcall SysInfo_HasFeature2() { return (hook::Global<uint32_t>(0xd97433) >> 2) & 1; }
RT3_REPLACE(0x55d400, SysInfo_HasFeature2);
uint32_t __fastcall SysInfo_HasFeature3() { return (hook::Global<uint32_t>(0xd97433) >> 3) & 1; }
RT3_REPLACE(0x55d410, SysInfo_HasFeature3);
uint32_t __fastcall SysInfo_TotalMemory() { return hook::Global<uint32_t>(0xd97550); }
RT3_REPLACE(0x55d430, SysInfo_TotalMemory);

// 0x55d440. Refreshes the memory status (0xd97548) and returns the free physical memory.
uint32_t __fastcall SysInfo_FreeMemory() {
    Import<void(__stdcall*)(void*)>(0x5c81d8)((void*)0xd97548);
    return hook::Global<uint32_t>(0xd97554);
}
RT3_REPLACE(0x55d440, SysInfo_FreeMemory);

// 0x55d460. Whether the OS is not NT-based (clears the 0xd97768 buffer first).
uint8_t __fastcall SysInfo_IsWin9x() {
    for (int k = 0; k < 0x40; ++k) ((uint32_t*)0xd97768)[k] = 0;
    uint32_t v[0x27] = {};
    v[0] = 0x9c;
    if (!Import<int32_t(__stdcall*)(void*)>(0x5c81d4)(v)) return 0;
    return v[4] != 2 ? 1 : 0;
}
RT3_REPLACE(0x55d460, SysInfo_IsWin9x);

// 0x55d8d0. Name of the primary display adapter (0xd97448, empty if none).
const char* __fastcall SysInfo_DisplayName() {
    uint8_t dd[0x1a8] = {};
    *(uint32_t*)dd = 0x1a8;
    char* out = (char*)0xd97448;
    out[0] = 0;
    auto enumDev = Import<int32_t(__stdcall*)(const char*, uint32_t, void*, uint32_t)>(0x5c8250);
    if (!enumDev(nullptr, 0, dd, 0)) return out;
    for (uint32_t i = 0;;) {
        if (*(uint32_t*)(dd + 0xa4) & 4) {
            Copy(out, (const char*)dd + 0x24);
            return out;
        }
        if (!enumDev(nullptr, ++i, dd, 0)) return out;
    }
}
RT3_REPLACE(0x55d8d0, SysInfo_DisplayName);

uint64_t __fastcall SysInfo_Zero64a() { return 0; }
RT3_REPLACE(0x55d960, SysInfo_Zero64a);
uint64_t __fastcall SysInfo_Zero64b() { return 0; }
RT3_REPLACE(0x55d970, SysInfo_Zero64b);

} // namespace rt3
