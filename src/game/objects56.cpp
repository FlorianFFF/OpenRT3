// Functions across blocks 0x46..0x4d (batch 59). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall Site_SaveExtra(uint8_t* o, int, Stream s);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

// 0x468490. Lists the music files "<dir>*.mp3" (dir at +0, count +0x100
// from 0x468420) into +0x108: 0x104-byte entries {path, CRC of the name}.
void __fastcall Music_BuildList(uint8_t* o) {
    if (F<void*>(o, 0x108)) {
        Free(F<void*>(o, 0x108));
        F<void*>(o, 0x108) = nullptr;
    }
    if (F<int32_t>(o, 0x100) <= 0) return;
    uint8_t* e = (uint8_t*)Malloc(F<int32_t>(o, 0x100) * 0x104);
    char pattern[0x100];
    uint8_t find[0x140];
    FormatText(pattern, (const char*)0x5ce13c, (const char*)o, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    F<uint8_t*>(o, 0x108) = e;
    void* h = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190)(pattern, find);
    if (h == (void*)-1) return;
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    const char* fileName = (const char*)find + 0x2c;
    do {
        F<uint32_t>(e, 0x100) = ((uint32_t(__fastcall*)(const char*, int32_t))0x53d810)(fileName, 1);
        FormatText((char*)e, (const char*)0x5c8730, (const char*)o, fileName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        e += 0x104;
    } while (findNext(h, find) == 1);
    Import<int32_t(__stdcall*)(void*)>(0x5c819c)(h);
}
RT3_REPLACE(0x468490, Music_BuildList);

// 0x481310. Sites save: tags 0x4a9d/0x4a3a/0x4a3b with each site's extra data (0x47da40).
void __fastcall Sites_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x4a9d);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x4a3a);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Site_SaveExtra((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, s);
    StreamWriteU32(s, 0x4a3b);
}
RT3_REPLACE(0x481310, Sites_Save);

// 0x4c16b0. Copies entry i of the 12-byte array +0x5f.
void __fastcall Obj_GetEntry5f(uint8_t* o, int, uint32_t* out, int32_t i) {
    const uint32_t* e = (const uint32_t*)(o + 0x5f + i * 12);
    out[0] = e[0];
    out[1] = e[1];
    out[2] = e[2];
}
RT3_REPLACE(0x4c16b0, Obj_GetEntry5f);

int32_t Screen90() { return F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90); }
RT3_REPLACE(0x4c16e0, Screen90);
void SetGlobal6cfeac() { hook::Global<int32_t>(0x6cfeac) = 1; }
RT3_REPLACE(0x4c8670, SetGlobal6cfeac);
int32_t Global6cfeb4Positive() { return hook::Global<int32_t>(0x6cfeb4) > 0 ? 1 : 0; }
RT3_REPLACE(0x4c8680, Global6cfeb4Positive);

// 0x4cb6b0. UI message handler: 0xcb messages 0x5143 / 0x5142 adjust the counter 0x6cff7c.
int32_t __fastcall Ui_HandleCounterMsg(void*, int, const int32_t* msg) {
    if (msg[0] != 0xcb) return 1;
    if (msg[1] == 0x5143) {
        hook::Global<int32_t>(0x6cff7c)++;
        return 0;
    }
    if (msg[1] == 0x5142) {
        hook::Global<int32_t>(0x6cff7c)--;
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4cb6b0, Ui_HandleCounterMsg);

// 0x4cb9f0. UI message handler: 0xca messages 0x5c3a..0x5c3c / 0x5c44..0x5c46
// set world +0x4c78 / +0x4c7c to 0, 1 or 2.
int32_t __fastcall Ui_HandleModeMsg(void*, int, const int32_t* msg) {
    if (msg[0] != 0xca) return 1;
    uint32_t i = (uint32_t)(msg[1] - 0x5c3a);
    if (i > 0xc) return 1;
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (i <= 2) F<int32_t>(w, 0x4c78) = (int32_t)i;
    else if (i >= 10) F<int32_t>(w, 0x4c7c) = (int32_t)(i - 10);
    return 1;
}
RT3_REPLACE(0x4cb9f0, Ui_HandleModeMsg);

void __fastcall Nop4cf900(void*) {}
RT3_REPLACE(0x4cf900, Nop4cf900);

// 0x4d03d0. Ratio +0x76 / +0x7a, with the divisor floored at 0.001.
float __fastcall Obj_Ratio76(uint8_t* o) {
    if ((double)F<float>(o, 0x7a) < *(const double*)0x5c8678) return (float)((double)F<float>(o, 0x76) / 0.001);
    return (float)((double)F<float>(o, 0x76) / (double)F<float>(o, 0x7a));
}
RT3_REPLACE(0x4d03d0, Obj_Ratio76);

int32_t Global6d07b4Set() { return hook::Global<void*>(0x6d07b4) ? 1 : 0; }
RT3_REPLACE(0x4d4150, Global6d07b4Set);

void __fastcall Obj_SetTextE0(uint8_t* o, int, const char* s) { StrCopyN((char*)o + 0xe0, s, 0x320); }
RT3_REPLACE(0x4d78b0, Obj_SetTextE0);

} // namespace rt3
