// Engine/system helpers in blocks 0x51..0x52 (batch 67). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Engine_ClampWindowSize(void* e, int, int32_t* w, int32_t* h);

static inline char* ExeStrncpy(char* d, const char* s, size_t n) { return ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(d, s, n); }

// 0x51e740. Writes the separator line to the debug log once per session
// (default file rt3_debug.txt in .\data\log\).
void DebugLog_WriteHeader() {
    if (!hook::Global<int32_t>(0x6d3ca0) || hook::Global<int32_t>(0x6d3d10)) return;
    char line[0x14];
    for (int k = 0; k < 0x14; ++k) line[k] = ((const char*)0x5dce90)[k];
    hook::Global<int32_t>(0x6d3d10) = 1;
    if (!hook::Global<char>(0x6d3ca8)) {
        ExeStrncpy((char*)0x6d3ca8, (const char*)0x5dcddc, 100);
        hook::Global<uint8_t>(0x6d3d0c) = 0;
    }
    char path[0x1f0];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, (const char*)0x5dce44, (const char*)0x6d3ca8);
    int32_t fd = ((int32_t(__cdecl*)(const char*, int32_t, ...))0x5a4c57)(path, 0x4301, 0x80);
    if (fd == -1) return;
    size_t len = 0;
    while (line[len]) ++len;
    ((int32_t(__cdecl*)(int32_t, const void*, uint32_t))0x5a464d)(fd, line, (uint32_t)len + 1);
    ((int32_t(__cdecl*)(int32_t))0x5a440d)(fd);
}
RT3_REPLACE(0x51e740, DebugLog_WriteHeader);

// 0x51eea0. Saves the engine configuration (version 0x41b, 0x1e4 bytes from
// +0x11468a, 0x1ec bytes of padding) to data\configuration\engine.cfg.
void __fastcall Engine_SaveConfig(uint8_t* e) {
    Stream s = StreamOpen((const char*)0x5dcec8, 0, 0x30d40, 1);
    if (s == (Stream)-1) return;
    uint32_t version = 0x41b;
    StreamWrite(s, &version, 4, 0);
    StreamWrite(s, e + 0x11468a, 0x1e4, 0);
    uint8_t pad[0x1ec] = {};
    StreamWrite(s, pad, 0x1ec, 0);
    StreamClose(s);
}
RT3_REPLACE(0x51eea0, Engine_SaveConfig);

// 0x5201e0. Window frame size for the window +0 (two user32 rectangle calls).
void __fastcall Window_FrameSize(void** o, int, int32_t* w, int32_t* h) {
    int32_t r[4] = { 0, 0, 0x12c, 0x12c };
    int32_t r2[4];
    Import<int32_t(__stdcall*)(void*, int32_t*)>(0x5c82b0)(o[0], r2);
    Import<int32_t(__stdcall*)(int32_t*, uint32_t, int32_t)>(0x5c82b4)(r, 0x4cb0000, 0);
    *w = r2[0] - r[0];
    *h = r2[1] - r[1];
}
RT3_REPLACE(0x5201e0, Window_FrameSize);

// 0x5204b0. Clears the engine's three render lists (the third holds objects
// whose own list +0xe4 is cleared and flag +0xe8 reset).
void __fastcall Engine_ClearRenderLists(uint8_t* e) {
    PtrList* a = F<PtrList*>(e, 0x1136a1);
    if (F<int32_t>(a, 0xc)) PtrList_Clear(a);
    PtrList* b = F<PtrList*>(e, 0x11369d);
    if (F<int32_t>(b, 0xc)) PtrList_Clear(b);
    PtrList_Rewind(F<PtrList*>(e, 0x1136a5));
    while (uint8_t* o = (uint8_t*)PtrList_Next(F<PtrList*>(e, 0x1136a5))) {
        F<int32_t>(o, 0xe8) = 0;
        PtrList_Clear(F<PtrList*>(o, 0xe4));
    }
    PtrList_Clear(F<PtrList*>(e, 0x1136a5));
}
RT3_REPLACE(0x5204b0, Engine_ClearRenderLists);

// 0x520530. Picks the display mode (0x15-byte entries +0x34, count +0x30)
// closest to w x h x bpp (16 if no 32-bit mode exists) and refresh +0x44;
// writes it back and returns its index. QUIRK: no candidate reads entry -1.
int32_t __fastcall Engine_PickDisplayMode(uint8_t* e, int, int32_t* w, int32_t* h, int32_t* bpp) {
    Engine_ClampWindowSize(e, 0, w, h);
    int32_t n = F<int32_t>(e, 0x30);
    bool has32 = false;
    for (int32_t i = 0; i < n; ++i)
        if (F<int32_t>(F<uint8_t*>(e, 0x34) + i * 0x15, 8) == 0x20) has32 = true;
    if (!has32) *bpp = 0x10;
    int32_t best = 0x7fffffff, sel = -1;
    for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i) {
        uint8_t* m = F<uint8_t*>(e, 0x34) + i * 0x15;
        if (!F<int32_t>(m, 0)) continue;
        int32_t d = (*bpp - F<int32_t>(m, 8)) * 0xffff - F<int32_t>(m, 4) - F<int32_t>(m, 0) + *w + *h;
        int32_t rate = F<int32_t>(m, 0xc), want = F<int32_t>(e, 0x44);
        if (rate > want) d += rate;
        else if (rate < want) d += want - rate;
        if (d >= 0 && d <= best) {
            best = d;
            sel = i;
        }
    }
    uint8_t* m = F<uint8_t*>(e, 0x34) + sel * 0x15;
    *w = F<int32_t>(m, 0);
    *h = F<int32_t>(m, 4);
    *bpp = F<int32_t>(m, 8);
    return sel;
}
RT3_REPLACE(0x520530, Engine_PickDisplayMode);

int32_t __fastcall Obj_Sub2543(uint8_t* o) { return F<int32_t>(F<uint8_t*>(o, 0xc), 0x2543); }
RT3_REPLACE(0x520790, Obj_Sub2543);

// 0x521590. Deletes the four network critical sections.
void Net_DeleteLocks() {
    auto del = Import<void(__stdcall*)(void*)>(0x5c81f4);
    del((void*)0x6d4090);
    del((void*)0x6d406c);
    del((void*)0x6d4048);
    del((void*)0x6d40b4);
}
RT3_REPLACE(0x521590, Net_DeleteLocks);

int32_t Global6d4100() { return hook::Global<int32_t>(0x6d4100); }
RT3_REPLACE(0x521670, Global6d4100);

// 0x521680 / 0x5216a0. Locks / unlocks the network queue (critical section
// 0x6d4090, depth 0x6d40a8); lock returns the queue 0x6d408c.
int32_t Net_LockQueue() {
    Import<void(__stdcall*)(void*)>(0x5c81fc)((void*)0x6d4090);
    hook::Global<int32_t>(0x6d40a8)++;
    return hook::Global<int32_t>(0x6d408c);
}
RT3_REPLACE(0x521680, Net_LockQueue);
void Net_UnlockQueue() {
    hook::Global<int32_t>(0x6d40a8)--;
    Import<void(__stdcall*)(void*)>(0x5c81f8)((void*)0x6d4090);
}
RT3_REPLACE(0x5216a0, Net_UnlockQueue);

} // namespace rt3
