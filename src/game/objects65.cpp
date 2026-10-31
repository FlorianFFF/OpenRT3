// Network/player-list helpers in blocks 0x46..0x52 (batch 68). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t Net_LockQueue();
void Net_UnlockQueue();
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall Engine_SaveConfig(uint8_t* e);

// The network player list (head from Net_LockQueue): nodes {next, ?, name +8, flags +0x5c}.
static inline uint8_t* PlayerHead() { return *(uint8_t**)Net_LockQueue(); }

// 0x469b90. Whether every network player has flag 4 (1 when not networked).
int32_t Net_AllPlayersFlag4() {
    if (!hook::Global<int32_t>(0x6d40dc)) return 1;
    int32_t ok = 1;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (!(e[0x5c] & 4)) {
            ok = 0;
            break;
        }
    }
    Net_UnlockQueue();
    return ok;
}
RT3_REPLACE(0x469b90, Net_AllPlayersFlag4);

// 0x46a200. Message (0x6adfc0) naming the players still without flag 8
// (string 0xb71 with the ", "-joined names) or string 0xb72 if none.
const char* Net_WaitingPlayersText() {
    char names[0x100];
    names[0] = 0;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (e[0x5c] & 8) continue;
        char* d = names;
        while (*d) ++d;
        for (const char* s = (const char*)e + 8; (*d++ = *s++) != 0;) {}
        d = names;
        while (*d) ++d;
        d[0] = ((const char*)0x5c9f38)[0];
        d[1] = ((const char*)0x5c9f38)[1];
        d[2] = ((const char*)0x5c9f38)[2];
    }
    if (names[0]) {
        size_t len = 0;
        while (names[len]) ++len;
        names[len - 2] = 0;
        FormatText((char*)0x6adfc0, GetString(0xb71), names, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else {
        const char* s = GetString(0xb72);
        char* d = (char*)0x6adfc0;
        while ((*d++ = *s++) != 0) {}
    }
    Net_UnlockQueue();
    return (const char*)0x6adfc0;
}
RT3_REPLACE(0x46a200, Net_WaitingPlayersText);

// 0x46a440. Whether a network player has this name.
int32_t __fastcall Net_HasPlayer(void*, int, const char* name) {
    int32_t found = 0;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e) {
        if (Stricmp((const char*)e + 8, name) == 0) {
            found = 1;
            break;
        }
    }
    Net_UnlockQueue();
    return found;
}
RT3_REPLACE(0x46a440, Net_HasPlayer);

// 0x484910. Saves the options (version 0x429, 0x48d bytes from +0x70) and,
// on request, the engine configuration.
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine) {
    if (withEngine && hook::Global<void*>(0x6d4024)) Engine_SaveConfig(hook::Global<uint8_t*>(0x6d4024));
    Stream s = StreamOpen((const char*)0x5ceda0, 0, 0x30d40, 1);
    if (s == (Stream)-1) return;
    uint32_t version = 0x429;
    StreamWrite(s, &version, 4, 0);
    StreamWrite(s, o + 0x70, 0x48d, 0);
    StreamClose(s);
}
RT3_REPLACE(0x484910, Options_Save);

// 0x4ee500. Whether every network player has flag 1 (AL only).
uint8_t Net_AllPlayersFlag1() {
    uint8_t ok = 1;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e)
        if (!(e[0x5c] & 1)) ok = 0;
    Net_UnlockQueue();
    return ok;
}
RT3_REPLACE(0x4ee500, Net_AllPlayersFlag1);

int32_t Global6d40e4() { return hook::Global<int32_t>(0x6d40e4); }
RT3_REPLACE(0x5216c0, Global6d40e4);

// 0x5216d0. Unlock variant that pops one (unused) argument.
void __stdcall Net_UnlockQueueArg(int32_t) {
    hook::Global<int32_t>(0x6d40a8)--;
    Import<void(__stdcall*)(void*)>(0x5c81f8)((void*)0x6d4090);
}
RT3_REPLACE(0x5216d0, Net_UnlockQueueArg);

// 0x5216f0. This machine's IP address as text (winsock), or "" .
char* __stdcall Net_LocalAddress(char* out) {
    char host[0x104];
    out[0] = 0;
    Import<int32_t(__stdcall*)(char*, int32_t)>(0x5c82f8)(host, 0x104);
    uint8_t* h = Import<uint8_t*(__stdcall*)(const char*)>(0x5c82f4)(host);
    if (!h) return out;
    uint32_t addr = **F<uint32_t**>(h, 0xc);
    const char* ip = Import<const char*(__stdcall*)(uint32_t)>(0x5c8320)(addr);
    if (!ip) return out;
    char* d = out;
    while ((*d++ = *ip++) != 0) {}
    return out;
}
RT3_REPLACE(0x5216f0, Net_LocalAddress);

// 0x521750. Text for a network error code (table 0x624c60: {code, string id, ..}).
const char* __fastcall Net_ErrorText(int32_t code) {
    for (int32_t i = 0; i < 0xd8 / 12; ++i)
        if (((const int32_t*)0x624c60)[i * 3] == code) return GetString(((const int32_t*)0x624c64)[i * 3]);
    return GetString(((const int32_t*)0x624c58)[(0xd8 / 12) * 3]);
}
RT3_REPLACE(0x521750, Net_ErrorText);

int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t) { return 1; }
RT3_REPLACE(0x521790, Return1_521790);

} // namespace rt3
