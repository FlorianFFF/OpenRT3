// Adapter quirks / texture limits / version info / widget types / tree view / list rows in blocks 0x48..0x56 (batch 113). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

const char* __fastcall SysInfo_DisplayName();
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
int32_t __fastcall Engine_TotalTextureMem(void* e);
uint32_t __fastcall SysInfo_TotalMemory();
int32_t __fastcall PowerOfTwo(int32_t v, int32_t roundUp);
const char* __fastcall GetString(int32_t id);
void __fastcall Texture_Destroy(uint8_t* t);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }
static inline const int32_t* DisplayMode() {
    return (const int32_t*)(F<uint8_t*>(Engine(), 0x34) + F<int32_t>(Engine(), 0x11421e) * 0x15);
}

// 0x484a60. Index of the known-adapter entry (44 x {name, extra} at 0x621428,
// 0x18 bytes) matching the display adapter name; -1 if none.
int32_t __fastcall Adapter_KnownIndex() {
    char name[0x6c] = {};
    ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(name, SysInfo_DisplayName(), 100);
    int32_t i = 0;
    for (const char* const* e = (const char* const*)0x62142c; e < (const char* const*)0x62184c;
         e = (const char* const*)((const uint8_t*)e + 0x18), ++i) {
        if (!StrFind(name, e[-1], 0)) continue;
        const char* extra = e[0];
        if (!*extra || StrFind(name, extra, 0)) return i;
    }
    return -1;
}
RT3_REPLACE(0x484a60, Adapter_KnownIndex);

// 0x5327a0. Clamps a texture size to a power of two within the card's budget
// (256 / 512 / 1024 by texture memory and RAM) and the engine maximum.
void __stdcall Render_ClampTextureSize(int32_t* w, int32_t* h) {
    int32_t mem = Engine_TotalTextureMem(Engine());
    int32_t maxs;
    if (mem >= 0x3d00000 && SysInfo_TotalMemory() > 0xb400000) maxs = 0x400;
    else maxs = mem < 0x1a00000 ? 0x100 : 0x200;
    *w = PowerOfTwo(*w, 0);
    *h = PowerOfTwo(*h, 0);
    if (maxs < *w) *w = maxs;
    if (maxs < *h) *h = maxs;
    int32_t mw = F<int32_t>(Engine(), 0x114243);
    if (*w / 4 > mw) *w = mw << 2;
    int32_t mh = F<int32_t>(Engine(), 0x114247);
    if (*h / 4 > mh) *h = mh * 4;
}
RT3_REPLACE(0x5327a0, Render_ClampTextureSize);

uint32_t __fastcall Sys_Call5c81d0() {
    uint32_t v = Import<uint32_t(__stdcall*)()>(0x5c81cc)();
    return Import<uint32_t(__stdcall*)(uint32_t)>(0x5c81d0)(v);
}
RT3_REPLACE(0x55d980, Sys_Call5c81d0);

// 0x55d990. Message for an error code (table 0x6257f8 of {code, string id},
// searched over the first `code` entries; entry 0 as the fallback).
const char* __fastcall Error_Message(uint32_t code) {
    for (uint32_t i = 0; i < code; ++i)
        if (code == ((const uint32_t*)0x6257f8)[i * 2]) return GetString(((const int32_t*)0x6257fc)[i * 2]);
    return GetString(hook::Global<int32_t>(0x6257fc));
}
RT3_REPLACE(0x55d990, Error_Message);

// 0x55d9c0. The file's product version (low word << 16 | high word), 0 if unknown.
uint32_t __fastcall File_Version(const char* path) {
    uint32_t handle, result = 0;
    uint32_t size = ((uint32_t(__stdcall*)(const char*, uint32_t*))0x5b31ec)(path, &handle);
    if (!size) return 0;
    void* buf = ((void*(__cdecl*)(size_t))0x53b070)(size);
    if (((int32_t(__stdcall*)(const char*, uint32_t, uint32_t, void*))0x5b31e6)(path, 0, size, buf)) {
        uint8_t* info;
        uint32_t len;
        if (((int32_t(__stdcall*)(void*, const char*, void*, uint32_t*))0x5b31e0)(buf, (const char*)0x5cbc58, &info, &len))
            result = (F<uint32_t>(info, 0x14) << 16) | (F<uint32_t>(info, 0x10) & 0xffff);
    }
    ((void(__cdecl*)(void*))0x53b080)(buf);
    return result;
}
RT3_REPLACE(0x55d9c0, File_Version);

// 0x55e9f0. Picture widget defaults (type 0x80).
void __fastcall Picture_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0xe6, 0xd6, 0xea, 0xa2, 0xa6, 0xb2, 0xf2, 0xfe, 0xbe, 0x102, 0xaa, 0xae, 0x106 };
    F<int32_t>(w, 0xe6) = 0;
    F<int32_t>(w, 0xda) = 1;
    F<uint16_t>(w, 6) = 0x80;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
}
RT3_REPLACE(0x55e9f0, Picture_Init);

// 0x55ed40. Releases the picture's texture (+0xe6) and re-enables the engine flag.
void __fastcall Picture_FreeTexture(uint8_t* w) {
    uint8_t* t = F<uint8_t*>(w, 0xe6);
    if (!t) return;
    Texture_Destroy(t);
    ((void(__cdecl*)(void*))0x53b080)(t);
    F<void*>(w, 0xe6) = nullptr;
    uint8_t v = Engine()[0x114227];
    hook::Global<int32_t>(0xd97984) = 0;
    Engine()[0x114227] = (uint8_t)((v & 0xf1) | 1);
}
RT3_REPLACE(0x55ed40, Picture_FreeTexture);

// 0x55f9f0. Tree view defaults (type 8).
void __fastcall TreeView_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0x182, 0x186, 0x18a, 0x18e, 0x1da, 0x1d2, 0x176, 0x17a, 0x1e2, 0x1e6, 0x1de,
                                     0x17e, 0x1be, 0x1ce, 0x1ba, 0x1ca, 0x1c6, 0x1c2, 0xde,  0xe2,  0xe6,  0xee,
                                     0xf2,  0xa2,  0x19a, 0xea,  0x1a8, 0x1ac, 0x1b2, 0x1b6, 0x19e, 0x1a2, 0xfe,
                                     0x102, 0xfa };
    F<uint16_t>(w, 6) = 8;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
    F<int32_t>(w, 0x1d6) = -1;
    F<int32_t>(w, 0x192) = -1;
    F<int32_t>(w, 0x172) = 1;
    F<uint16_t>(w, 0x1a6) = 0;
}
RT3_REPLACE(0x55f9f0, TreeView_Init);

// Tree nodes (+0x176, 32 bytes): +0 value, +8 data, +0x10 has parent, +0x14 parent, +0x18 expanded.
static inline uint8_t* Node(uint8_t* w, int32_t i) { return F<uint8_t*>(w, 0x176) + i * 32; }
uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i) { return F<uint32_t>(Node(w, i), 8); }
RT3_REPLACE(0x55fb80, TreeView_NodeData);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w) { return F<uint32_t>(Node(w, F<int32_t>(w, 0x1d6)), 8); }
RT3_REPLACE(0x55fba0, TreeView_SelectedData);
// 0x55fbc0. Whether some ancestor of node i is collapsed.
int32_t __fastcall TreeView_IsHidden(uint8_t* w, int, int32_t i) {
    uint8_t* n = Node(w, i);
    if (!F<int32_t>(n, 0x10)) return 0;
    do {
        n = Node(w, F<int32_t>(n, 0x14));
        if (!F<int32_t>(n, 0x18)) return 1;
    } while (F<int32_t>(n, 0x10));
    return 0;
}
RT3_REPLACE(0x55fbc0, TreeView_IsHidden);
int32_t __fastcall TreeView_Depth(uint8_t* w, int, int32_t i) {
    int32_t d = 0;
    for (uint8_t* n = Node(w, i); F<int32_t>(n, 0x10); ++d) n = Node(w, F<int32_t>(n, 0x14));
    return d;
}
RT3_REPLACE(0x55fc10, TreeView_Depth);
uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i) { return F<uint32_t>(Node(w, i), 0); }
RT3_REPLACE(0x55fca0, TreeView_NodeValue);

// 0x55ffb0. Row height as a fraction of the screen, snapped to whole pixels
// (+0x5d0250) (x87 with the exe's _ftol).
double __fastcall List_RowHeight(uint8_t* w) {
    int32_t h = DisplayMode()[1];
    float rh = F<float>(w, 0xb6);
    int32_t px;
    __asm {
        fld rh
        fimul h
        mov eax, 0x5a10d0
        call eax
        mov px, eax
    }
    return ((double)px + *(const double*)0x5d0250) / h;
}
RT3_REPLACE(0x55ffb0, List_RowHeight);

// 0x560350. Screen position of row `row` (x into *x, y into *y).
void __fastcall List_RowPos(uint8_t* w, int, int32_t row, float* x, float* y) {
    int32_t h = DisplayMode()[1];
    float rh = F<float>(w, 0xb6);
    uint8_t* p = F<uint8_t*>(w, 0x86);
    *x = (float)(((double)F<float>(p, 5) + F<float>(w, 0xa6)) + F<float>(w, 0xb));
    int32_t px;
    __asm {
        fild h
        fmul rh
        mov eax, 0x5a10d0
        call eax
        mov px, eax
    }
    p = F<uint8_t*>(w, 0x86);
    *y = (float)((((((double)px + *(const double*)0x5d0250) / h + F<float>(w, 0xb2)) * row + F<float>(p, 9)) +
                  F<float>(w, 0xaa)) + F<float>(w, 0xf));
}
RT3_REPLACE(0x560350, List_RowPos);

// 0x562d30. Timer widget defaults (type 0x10, 500 ms).
void __fastcall TimerWidget_Init(uint8_t* w) {
    F<uint16_t>(w, 6) = 0x10;
    F<int32_t>(w, 0xa2) = 0;
    F<int32_t>(w, 0xa6) = 0x1f4;
    for (int k = 0; k < 8; ++k) F<uint16_t>(w, 0xaa + k * 2) = 0;
    F<int32_t>(w, 0xd6) = 0;
    F<int32_t>(w, 0xce) = 0;
    F<int32_t>(w, 0xd2) = 0;
}
RT3_REPLACE(0x562d30, TimerWidget_Init);

void __fastcall Reset_625a94() { hook::Global<int32_t>(0x625a94) = -1; }
RT3_REPLACE(0x563250, Reset_625a94);

} // namespace rt3
