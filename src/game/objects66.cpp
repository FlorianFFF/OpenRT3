// Network layer helpers in block 0x52 (batch 69). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

const char* __fastcall GetString(int32_t id);
char* __stdcall Net_LocalAddress(char* out);

uint8_t __stdcall Net_MsgKind(const uint8_t* msg) { return F<uint8_t*>((void*)msg, 4)[9]; }
RT3_REPLACE(0x5217a0, Net_MsgKind);
void __fastcall Nop5217b0(void*) {}
RT3_REPLACE(0x5217b0, Nop5217b0);

// 0x5217c0 / 0x5217e0. Lock / unlock the second network list (critical
// section 0x6d4048, depth 0x6d4060); lock returns the list 0x6d4044.
int32_t Net_LockList2() {
    Import<void(__stdcall*)(void*)>(0x5c81fc)((void*)0x6d4048);
    hook::Global<int32_t>(0x6d4060)++;
    return hook::Global<int32_t>(0x6d4044);
}
RT3_REPLACE(0x5217c0, Net_LockList2);
void Net_UnlockList2() {
    hook::Global<int32_t>(0x6d4060)--;
    Import<void(__stdcall*)(void*)>(0x5c81f8)((void*)0x6d4048);
}
RT3_REPLACE(0x5217e0, Net_UnlockList2);

// 0x521800. Broadcasts the LAN game announcement "name[;tag];ip;port" to UDP port 1234.
void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name) {
    if (hook::Global<int32_t>(0x6d4100) >= 4) return;
    char msg[0x100], ip[0x100];
    auto sprintf = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    int32_t port = F<int32_t>(o, 0x216);
    if (o[0x21a]) {
        const char* addr = Net_LocalAddress(ip);
        sprintf(msg, (const char*)0x5dcf88, name, GetString(0xdba), addr, port);
    } else {
        const char* addr = Net_LocalAddress(ip);
        sprintf(msg, (const char*)0x5dcf7c, name, addr, port);
    }
    int32_t s = Import<int32_t(__stdcall*)(int32_t, int32_t, int32_t)>(0x5c8310)(2, 2, 0);
    if (s < 0) return;
    int32_t one = 1;
    if (Import<int32_t(__stdcall*)(int32_t, int32_t, int32_t, const void*, int32_t)>(0x5c830c)(s, 0xffff, 0x20, &one, 4) == 0) {
        uint8_t addr[16] = {};
        F<uint16_t>(addr, 0) = 2;
        F<uint32_t>(addr, 4) = Import<uint32_t(__stdcall*)(uint32_t)>(0x5c8308)(0xffffffff);
        F<uint16_t>(addr, 2) = Import<uint16_t(__stdcall*)(uint16_t)>(0x5c8304)(0x4d2);
        size_t len = 0;
        while (msg[len]) ++len;
        Import<int32_t(__stdcall*)(int32_t, const char*, int32_t, int32_t, const void*, int32_t)>(0x5c8300)(
            s, msg, (int32_t)len, 0, addr, 16);
    }
    Import<int32_t(__stdcall*)(int32_t)>(0x5c82fc)(s);
}
RT3_REPLACE(0x521800, Net_BroadcastGame);

void __stdcall SetGlobal6d4108(int32_t v) { hook::Global<int32_t>(0x6d4108) = v; }
RT3_REPLACE(0x521910, SetGlobal6d4108);

// 0x521990. Text (in 0x6d4120) for the last socket error 0x6d4118.
const char* Net_SocketErrorText() {
    int32_t id;
    switch (hook::Global<int32_t>(0x6d4118)) {
    case 10036: id = 0xffb; break;
    case 10067: id = 0xffc; break;
    case 10091: id = 0xff9; break;
    case 10092: id = 0xffa; break;
    default: id = 0xffd; break;
    }
    const char* s = GetString(id);
    char* d = (char*)0x6d4120;
    while ((*d++ = *s++) != 0) {}
    return (const char*)0x6d4120;
}
RT3_REPLACE(0x521990, Net_SocketErrorText);

// 0x521aa0. Copies the session GUID 0x6257e8.
uint32_t* __stdcall Net_GetGuid(uint32_t* out) {
    for (int k = 0; k < 4; ++k) out[k] = ((const uint32_t*)0x6257e8)[k];
    return out;
}
RT3_REPLACE(0x521aa0, Net_GetGuid);

// 0x521af0 / 0x521b10. Network queue entry by ID (+4) / by name CRC (+0x48).
uint8_t* __fastcall Net_FindById(int32_t id) {
    for (uint8_t* e = *hook::Global<uint8_t**>(0x6d408c); e; e = *(uint8_t**)e)
        if (F<int32_t>(e, 4) == id) return e;
    return nullptr;
}
RT3_REPLACE(0x521af0, Net_FindById);
uint8_t* __fastcall Net_FindByName(const char* name) {
    uint32_t crc = ((uint32_t(__fastcall*)(const char*, int32_t))0x53d810)(name, 1);
    for (uint8_t* e = *hook::Global<uint8_t**>(0x6d408c); e; e = *(uint8_t**)e)
        if (F<uint32_t>(e, 0x48) == crc) return e;
    return nullptr;
}
RT3_REPLACE(0x521b10, Net_FindByName);

} // namespace rt3
