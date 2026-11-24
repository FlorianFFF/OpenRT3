// SysInfo.cpp - RT3_mp.exe 0x55c000-0x55e000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <intrin.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/crt.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);
void __fastcall Render_RestoreTarget(uint8_t* r);
void __fastcall Textures_ReleaseTargets();
void __fastcall Render_SaveAndShutdown(uint8_t* r);
char* Os_VersionName();
void* __fastcall Render_LockVb263f(uint8_t* r);
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin);
void __fastcall Model_EmitVertices(uint8_t* m, int, uint8_t* out, int32_t* count);
int32_t __fastcall Render_UnlockVb263f(uint8_t* r);
void __fastcall Render_CullCcw(uint8_t* r);
void __fastcall Scene_ApplyLighting(uint8_t* s);
void __fastcall Scene_ApplyLighting(uint8_t* r);
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
int32_t __fastcall EditBox_HitTest(uint8_t* w, int, float px, float py);
void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);
void* __fastcall Render_PushStageState(uint8_t* r);
void __fastcall Render_ResetStageCache(uint8_t* r);
int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
uint32_t World46aa0();
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary);
int32_t __fastcall EditBox_ActivateClick(uint8_t* w, int, const uint32_t* ev);
uint64_t __fastcall SysInfo_CpuMhz();
void __stdcall Cpu_Name_Impl(char* out, const char* vendor, int32_t model, int32_t family);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Scene_ApplyLighting(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Render_ValidateStates(uint8_t* r, int, const uint32_t* states);
RT3_NOINLINE void __fastcall EditBox_RunModal(uint8_t* w, int, int32_t caret);
int32_t __fastcall EditBox_ActivateKey(uint8_t* w, int, int32_t caret);
RT3_NOINLINE int32_t __fastcall EditBox_ActivateClick(uint8_t* w, int, const uint32_t* ev);
int32_t __fastcall EditBox_HandleEvent(uint8_t* w, int, uint32_t* ev);
int32_t Cpu_HasCpuid();
int32_t __stdcall Cpu_ProbeFeature(int32_t kind);
void __stdcall Cpu_Name_Impl(char* out, const char* vendor, int32_t model, int32_t family);
void Cpu_Name();
uint64_t __fastcall SysInfo_CpuMhz();
RT3_NOINLINE int32_t __fastcall SysInfo_Zero();
void __fastcall SysInfo_Nop();
const char* __fastcall SysInfo_CpuVendor();
const char* __fastcall SysInfo_CpuName();
RT3_NOINLINE uint32_t __fastcall SysInfo_CpuSignature();
uint32_t __fastcall SysInfo_HasFeature0();
uint32_t __fastcall SysInfo_HasFeature1();
uint32_t __fastcall SysInfo_HasFeature2();
uint32_t __fastcall SysInfo_HasFeature3();
uint32_t __fastcall SysInfo_TotalMemory();
uint32_t __fastcall SysInfo_FreeMemory();
uint8_t __fastcall SysInfo_IsWin9x();
char* Os_VersionName();
const char* __fastcall SysInfo_DisplayName();
uint64_t __fastcall SysInfo_Zero64a();
uint64_t __fastcall SysInfo_Zero64b();
uint32_t __fastcall Sys_Call5c81d0();
const char* __fastcall Error_Message(uint32_t code);
uint32_t __fastcall File_Version(const char* path);
void SysInfo_Init();
void __fastcall Scene_RebuildStaticBuffer(uint8_t* s);
RT3_NOINLINE void __fastcall Display_DestroyRenderer(uint8_t* o);
void __fastcall View_RecordStagePresets(uint8_t* s);

static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// Posts "edit box w finished" (event 0xca with its id +4 and itself).
static void EditBox_PostDone(uint8_t* w) {
    uint32_t ev[12] = {};
    ev[1] = F<uint16_t>(w, 4);
    ev[0] = 0xca;
    ev[11] = (uint32_t)(uintptr_t)w;
    Input_PushEvent(hook::Global<uint8_t*>(0x6d4018), 0, ev);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

// 0x55c8b0. Activates edit box w from the keyboard: edits it modally when
// editable (+0xa2) with the caret at `caret`, then posts event 0xca.
int32_t __fastcall EditBox_ActivateKey(uint8_t* w, int, int32_t caret) {
    if (F<int32_t>(w, 0xa2)) EditBox_RunModal(w, 0, caret);
    EditBox_PostDone(w);
    return 0;
}
RT3_REPLACE(0x55c8b0, EditBox_ActivateKey);

// 0x55c910. Activates edit box w from mouse event ev: edits it modally
// with the caret at the clicked character, then posts event 0xca.
RT3_NOINLINE int32_t __fastcall EditBox_ActivateClick(uint8_t* w, int, const uint32_t* ev) {
    if (F<int32_t>(w, 0xa2) && ev) EditBox_RunModal(w, 0, EditBox_HitTest(w, 0, *(const float*)&ev[9], *(const float*)&ev[10]));
    EditBox_PostDone(w);
    return 0;
}
RT3_REPLACE(0x55c910, EditBox_ActivateClick);

// 0x55c980. Edit box w's event handler. A click inside (vtable +0x30 /
// +0x20) goes to the click callback +0x9a at box-relative coordinates or
// activates the box; events 0x6f / 0x76 (to this box) and 0xae (with +0x33
// 0x80000) set its text, refitting the 0x11 font to 0x15 when it wraps
// past two lines (returns 0 only for 0x6f); 0xa4 / 0xb9 / 0xbd set +0xa2 /
// +0xec / +0xaa. Everything else goes to the widget default.
int32_t __fastcall EditBox_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint32_t id = F<uint16_t>(w, 4);
    switch (ev[0]) {
    case 6:
        if (!((int32_t(__thiscall*)(uint8_t*))(*(void***)w)[0x30 / 4])(w)) break;
        if (!((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)w)[0x20 / 4])(w, ev[9], ev[10], 1)) break;
        if (void(__fastcall * cb)(uint32_t, int, float, float) = F<void(__fastcall*)(uint32_t, int, float, float)>(w, 0x9a)) {
            uint8_t* win = F<uint8_t*>(w, 0x86);
            float y = (float)((double)*(float*)&ev[10] - ((double)F<float>(win, 9) + F<float>(w, 0xf)));
            float x = (float)((double)*(float*)&ev[9] - ((double)F<float>(win, 5) + F<float>(w, 0xb)));
            cb(id, 0, x, y);
            return 0;
        }
        return EditBox_ActivateClick(w, 0, ev);
    case 0x6f:
    case 0x76:
        if (ev[1] != id) break;
        goto setText;
    case 0xa4:
        if (ev[1] != id) break;
        F<uint32_t>(w, 0xa2) = ev[2];
        return 0;
    case 0xae:
        if (!(F<uint32_t>(w, 0x33) & 0x80000)) return Widget_HandleEvent(w, 0, (uint8_t*)ev);
        goto setText;
    case 0xb9:
        if (ev[1] == id) F<uint32_t>(w, 0xec) = ev[2];
        break;
    case 0xbd:
        if (ev[1] != id) break;
        F<uint32_t>(w, 0xaa) = ev[2];
        return Widget_HandleEvent(w, 0, (uint8_t*)ev);
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)ev);
setText : {
    const char* src = (const char*)ev[2];
    char*& text = F<char*>(w, 0xba);
    if (text) {
        if (!strcmp(src, text)) return ev[0] != 0x6f;
        Free(text);
    }
    text = (char*)Malloc(strlen(src) + 2);
    strcpy(text, src);
    if (!(F<uint32_t>(w, 0x33) & 0x80000) || !strlen(text) || !F<uint8_t*>(w, 0x63) || w[0x67] != 0x11) return ev[0] != 0x6f;
    uint8_t* font = (uint8_t*)(uintptr_t)World46aa0();
    if (!font) return ev[0] != 0x6f;
    int32_t size;
    if (Font_CountLines(font, 0, text, F<float>(w, 0x13), nullptr) > 2) {
        F<uint32_t>(w, 0x17) = 0x3d681b4f;
        F<uint32_t>(w, 0xf) = 0x3f717da3;
        if (Fl(0x5d11fc) < F<float>(w, 0x78)) {
            F<uint32_t>(w, 0xf) = F<uint32_t>(w, 0x70);
            F<uint32_t>(w, 0x17) = F<uint32_t>(w, 0x78);
        }
        size = 0x15;
    } else {
        if (!(F<float>(w, 0x78) < D(0x5caaf0))) {
            F<uint32_t>(w, 0xf) = F<uint32_t>(w, 0x70);
            F<uint32_t>(w, 0x17) = F<uint32_t>(w, 0x78);
        }
        size = w[0x67];
    }
    if (size != w[0x37]) Widget_SetFont(w, 0, size, 1);
    return ev[0] != 0x6f;
}
}
RT3_REPLACE(0x55c980, EditBox_HandleEvent);

// 0x55ccc0. Whether the CPUID instruction runs.
int32_t Cpu_HasCpuid() {
    __try {
        __asm {
            xor eax, eax
            cpuid
        }
    } __except (1) {
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x55ccc0, Cpu_HasCpuid);

// 0x55cd40. Whether an instruction of feature kind runs without faulting: 1 MMX,
// 2 SSE, 4 SSE2, 8 3DNow!; other kinds are taken as present.
int32_t __stdcall Cpu_ProbeFeature(int32_t kind) {
    __try {
        switch (kind) {
        case 1:
            __asm {
                pxor mm0, mm0
                emms
            }
            break;
        case 2: __asm { xorps xmm0, xmm0 } break;
        case 4: __asm { xorpd xmm0, xmm0 } break;
        case 8:
            __asm {
                _emit 0x0f
                _emit 0x0f
                _emit 0xc0
                _emit 0x96
                emms
            }
            break;
        }
    } __except (1) {
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x55cd40, Cpu_ProbeFeature);

// 0x55ce60 (body). Names the CPU from its CPUID vendor string, family and model:
// AMD 486 / K5 / K6 / K6-2 / K6-3 / Athlon and Intel 486 variants through the
// Pentium 4; anything else gets the "unknown" text (string 0x1d).
RT3_NOINLINE void __stdcall Cpu_Name_Impl(char* out, const char* vendor, int32_t model, int32_t family) {
    const char* name = nullptr;
    out[0] = 0;
    if (!strncmp((const char*)0x5ddf34, vendor, 0xc)) { // AuthenticAMD
        if (family == 4) {
            name = (const char*)0x5ddef4;
        } else if (family == 5) {
            if ((uint32_t)model <= 3) name = (const char*)0x5ddf20;
            else if (model == 6 || model == 7) name = (const char*)0x5ddf18;
            else if (model == 8) name = (const char*)0x5ddf0c;
            else if (model >= 9 && model <= 0xf) name = (const char*)0x5ddf00;
        } else if (family == 6) {
            name = (const char*)0x5ddf28;
        }
    } else if (!strncmp((const char*)0x5ddee4, vendor, 0xc)) { // GenuineIntel
        if (family == 4) {
            static const uint32_t i486[9] = { 0x5dded8, 0x5dded8, 0x5ddecc, 0x5ddebc, 0x5ddeb0, 0x5ddea0, 0, 0x5dde90, 0x5dde80 };
            if ((uint32_t)model <= 8) name = (const char*)i486[model];
        } else if (family == 5) {
            if (model > 0 && model <= 3) name = (const char*)0x5dde5c;
            else if (model == 4) name = (const char*)0x5dde6c;
        } else if (family == 6) {
            static const uint32_t p6[10] = { 0x5dde48, 0, 0x5dde34, 0, 0x5dde34, 0x5dde24, 0x5dde10, 0x5dde10, 0, 0x5dde10 };
            if ((uint32_t)(model - 1) <= 9) name = (const char*)p6[model - 1];
        } else if (family == 0xf) {
            name = (const char*)0x5dddfc;
        }
    }
    if (name) strcpy(out, name);
    if (!out[0]) strcpy(out, GetString(0x1d));
}

// 0x55ce60. Register arguments: EAX = out, EDI = vendor, EBX = model; family on
// the stack.
__declspec(naked) void Cpu_Name() {
    __asm {
        push dword ptr [esp + 4]
        push ebx
        push edi
        push eax
        call Cpu_Name_Impl
        ret 4
    }
}
RT3_REPLACE(0x55ce60, Cpu_Name);

// 0x55d2f0. CPU clock in MHz: time-stamp counter ticks over one second of the
// performance counter.
RT3_NOINLINE uint64_t __fastcall SysInfo_CpuMhz() {
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

RT3_NOINLINE int32_t __fastcall SysInfo_Zero() { return 0; }
RT3_REPLACE(0x55d390, SysInfo_Zero);

void __fastcall SysInfo_Nop() { __asm nop }
RT3_REPLACE(0x55d3a0, SysInfo_Nop);

const char* __fastcall SysInfo_CpuVendor() { return (const char*)0xd973f8; }
RT3_REPLACE(0x55d3b0, SysInfo_CpuVendor);

const char* __fastcall SysInfo_CpuName() { return (const char*)0xd97405; }
RT3_REPLACE(0x55d3c0, SysInfo_CpuName);

RT3_NOINLINE uint32_t __fastcall SysInfo_CpuSignature() { return hook::Global<uint32_t>(0xd9743b); }
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

// 0x55d4c0. Describes the Windows version in a static buffer (0xd97868): 95 / 98 /
// ME by minor version; on NT, workstations are NT 4.0, XP (minor 1; Home or
// Professional Edition) or else 2000, and servers .Net (minor 2) or 2000 plus their
// edition, then the service pack in parentheses. Null when GetVersionExA fails.
char* Os_VersionName() {
    char* out = (char*)0xd97868;
    memset(out, 0, 0x100);
    OSVERSIONINFOEXA vi;
    memset(&vi, 0, sizeof vi);
    vi.dwOSVersionInfoSize = 0x9c;
    if (!((BOOL(WINAPI*)(OSVERSIONINFOA*))hook::Global<void*>(0x5c81d4))((OSVERSIONINFOA*)&vi)) return nullptr;
    if (vi.dwPlatformId == 1) {
        strcpy(out, (const char*)(vi.dwMinorVersion < 10 ? 0x5ddf5c : vi.dwMinorVersion < 90 ? 0x5ddf50 : 0x5ddf44));
        return out;
    }
    if (vi.dwPlatformId != 2) return out;
    if (vi.wProductType == 1) {
        if (vi.dwMajorVersion < 5) {
            strcpy(out, (const char*)0x5ddfc8); // Windows NT 4.0
        } else if (vi.dwMinorVersion == 1) {
            strcpy(out, (const char*)0x5de010); // Windows XP
            strcat(out, (const char*)(vi.wSuiteMask & 0x200 ? 0x5de000 : 0x5ddfe8));
        } else {
            strcpy(out, (const char*)0x5ddfd8); // Windows 2000
        }
    } else if (vi.wProductType == 3) {
        if (vi.dwMajorVersion >= 5) {
            strcpy(out, (const char*)(vi.dwMinorVersion == 2 ? 0x5ddfb8 : 0x5ddfd8));
            if ((int8_t)vi.wSuiteMask < 0) strcat(out, (const char*)0x5ddfa4); // DataCenter Server
            else if (vi.wSuiteMask & 2) strcat(out, (const char*)(vi.dwMajorVersion == 4 ? 0x5ddf90 : 0x5ddf7c));
            else if (vi.wSuiteMask == 0x400) strcat(out, (const char*)0x5ddf70); // Web Server
            else strcat(out, (const char*)0x5ddf68); // Server
        }
    } else {
        strcpy(out, (const char*)0x5ddfc8);
        strcat(out, (const char*)0x5ddf68);
    }
    if (vi.szCSDVersion[0]) {
        strcat(out, (const char*)0x5d0d34);
        strcat(out, vi.szCSDVersion);
        strcat(out, (const char*)0x5d0b78);
    }
    return out;
}
RT3_REPLACE(0x55d4c0, Os_VersionName);

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

// 0x55da40. Fills the system-info block: memory status (0xd97548), CPU MHz
// (0xd9743b) and, when CPUID runs, the vendor (0xd973f8), family / model /
// stepping (0xd97423 / 0xd97427 / 0xd9742b, extended family for family 0xf), the
// supported (0xd9742f) and usable (0xd97433) MMX 1 / SSE 2 / SSE2 4 / 3DNow 8
// features (SSE2 sets 0xd9743f, passed to the CRT) and the CPU name (0xd97405).
// 0xd97437 ends up 0xf (the original reuses that register).
void SysInfo_Init() {
    memset((void*)0xd973f8, 0, 0x4b);
    memset((void*)0xd97548, 0, 0x20);
    hook::Global<void(__stdcall*)(void*)>(0x5c81d8)((void*)0xd97548);
    hook::Global<uint32_t>(0xd9743b) = (uint32_t)SysInfo_CpuMhz();
    if (!Cpu_HasCpuid()) return;
    int r[4];
    char vendor[13];
    __cpuid(r, 0);
    memcpy(vendor, &r[1], 4);
    memcpy(vendor + 4, &r[3], 4);
    memcpy(vendor + 8, &r[2], 4);
    vendor[12] = 0;
    __cpuid(r, 1);
    uint32_t sig = (uint32_t)r[0], feat = (uint32_t)r[3], ext = 0;
    __cpuid(r, (int)0x80000000);
    if ((uint32_t)r[0] >= 0x80000000) {
        __cpuid(r, (int)0x80000001);
        ext = (uint32_t)r[3];
    }
    uint32_t have = 0, usable = 0;
    if (feat & 0x800000) {
        have = 1;
        if (Cpu_ProbeFeature(1)) usable = 1;
    }
    if (ext & 0x80000000) {
        have |= 8;
        if (Cpu_ProbeFeature(8)) usable |= 8;
    }
    if (feat & 0x2000000) {
        have |= 2;
        if (Cpu_ProbeFeature(2)) usable |= 2;
    }
    if (feat & 0x4000000) {
        have |= 4;
        if (Cpu_ProbeFeature(4)) {
            usable |= 4;
            hook::Global<int32_t>(0xd9743f) = 1;
        }
    }
    ((void(__cdecl*)(int32_t))0x5a4376)(hook::Global<int32_t>(0xd9743f));
    hook::Global<uint32_t>(0xd97433) = usable;
    hook::Global<uint32_t>(0xd9742f) = have;
    hook::Global<uint32_t>(0xd9742b) = sig & 0xf;
    uint32_t family = sig >> 8 & 0xf;
    hook::Global<uint32_t>(0xd97423) = family;
    if (family == 0xf) hook::Global<uint32_t>(0xd97423) = ((sig & 0xff00000) | 0xf0000) >> 16;
    hook::Global<uint32_t>(0xd97427) = sig >> 4 & 0xf;
    strcpy((char*)0xd973f8, vendor);
    Cpu_Name_Impl((char*)0xd97405, (const char*)0xd973f8, hook::Global<int32_t>(0xd97427), hook::Global<int32_t>(0xd97423));
    hook::Global<int32_t>(0xd97437) = 0xf;
}
RT3_REPLACE(0x55da40, SysInfo_Init);

// 0x55dbf0. Rebuilds the scene's static pick / overlay vertex buffer: the meshes
// +0x99, +0x9d, +0xa1, +0xa5 (vertices only) and +0x95 write their triangles one
// after another, each recording its first (+0x1ee) and end (+0x1fa) index; then
// lighting, render target and culling are restored and the fade distance
// +0x22b67 set to 33700. Skipped without the first mesh or before setup (+1).
void __fastcall Scene_RebuildStaticBuffer(uint8_t* s) {
    s[0x374b] = 0;
    if (!F<uint32_t>(s, 0x99) || !F<uint32_t>(s, 1)) return;
    uint8_t* out = (uint8_t*)Render_LockVb263f(F<uint8_t*>(s, 0x2d));
    int32_t first = 0;
    for (uint32_t off : { 0x99u, 0x9du, 0xa1u, 0xa5u, 0x95u }) {
        int32_t n = 0;
        F<int32_t>(F<uint8_t*>(s, off), 0x1ee) = first;
        if (off == 0xa5)
            Model_EmitVertices(F<uint8_t*>(s, off), 0, out, &n);
        else
            Mesh_EmitPickTriangles(F<uint8_t*>(s, off), 0, out, &n, nullptr, nullptr);
        first += n;
        F<int32_t>(F<uint8_t*>(s, off), 0x1fa) = first;
        out += n * 32;
    }
    Render_UnlockVb263f(F<uint8_t*>(s, 0x2d));
    Scene_ApplyLighting(s);
    Render_RestoreTarget(F<uint8_t*>(s, 0x2d));
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    F<uint32_t>(s, 0x22b67) = 0x4703a400;
}
RT3_REPLACE(0x55dbf0, Scene_RebuildStaticBuffer);

// 0x55dd80. Destroys the renderer (+0x2d): restores the render target, releases the
// target textures, saves its settings and shuts it down.
RT3_NOINLINE void __fastcall Display_DestroyRenderer(uint8_t* o) {
    Render_RestoreTarget(F<uint8_t*>(o, 0x2d));
    Textures_ReleaseTargets();
    if (uint8_t* r = F<uint8_t*>(o, 0x2d)) {
        Render_SaveAndShutdown(r);
        ExeDelete(r);
    }
    F<void*>(o, 0x2d) = nullptr;
}
RT3_REPLACE(0x55dd80, Display_DestroyRenderer);

// 0x55ddc0. Records view s's texture stage presets (colour / alpha op
// blocks +0x35..+0x59; modulate x2 on screens above 256 pixels, else
// plain modulate), then probes the card: detail texturing (0x114251) when
// the +0x49 preset validates, otherwise falls back to a simpler +0x4d and
// +0x59; multitexture (0x114250, unless already known) by the +0x35 preset.
void __fastcall View_RecordStagePresets(uint8_t* s) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    uint32_t mod = (F<int32_t>(e, 0x11423f) > 0x100 || F<int32_t>(e, 0x11423b) > 0x100) ? 5 : 4;
    struct Op {
        uint8_t alpha;
        int32_t stage;
        uint32_t a1, op, a2;
    };
    // op 0 means the modulate chosen above
    static const Op ops[] = { { 0, 0, 2, 4, 1 }, { 1, 0, 2, 4, 1 }, { 0xff, 0x41, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 6, 3 }, { 0xff, 0x3d, 0, 0, 0 },
                              { 0, 0, 2, 4, 1 }, { 1, 0, 2, 4, 3 }, { 0xff, 0x51, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 4, 1 }, { 0xff, 0x45, 0, 0, 0 },
                              { 0, 0, 3, 4, 0x12 }, { 0xff, 0x49, 0, 0, 0 }, { 0, 0, 3, 4, 1 }, { 0xff, 0x4d, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 4, 1 },
                              { 0xff, 0x55, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 1, 1 }, { 0xff, 0x59, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 1, 1 },
                              { 0, 1, 2, 8, 1 }, { 0xff, 0x35, 0, 0, 0 }, { 0, 0, 2, 0, 1 }, { 1, 0, 2, 7, 1 }, { 0, 1, 3, 0, 1 }, { 1, 1, 3, 0xa, 2 },
                              { 0xff, 0x39, 0, 0, 0 } };
    for (const Op& o : ops) {
        if (o.alpha == 0xff) F<void*>(s, (uint32_t)o.stage) = Render_PushStageState(F<uint8_t*>(s, 0x2d));
        else if (o.alpha) Render_SetAlphaOp(F<uint8_t*>(s, 0x2d), 0, o.stage, o.a1, o.op, o.a2, 0);
        else Render_SetColorOp(F<uint8_t*>(s, 0x2d), 0, o.stage, o.a1, o.op ? o.op : mod, o.a2, 0);
    }
    Render_ResetStageCache(F<uint8_t*>(s, 0x2d));
    hook::Global<uint8_t*>(0x6d4024)[0x114251] = 0;
    if ((uint8_t)Render_ValidateStates(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x49))) {
        hook::Global<uint8_t*>(0x6d4024)[0x114251] = 1;
    } else if (!(uint8_t)Render_ValidateStates(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x4d))) {
        F<uint32_t*>(s, 0x4d)[1] = 2;
        F<uint32_t*>(s, 0x4d)[2] = 1;
        Render_ValidateStates(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x4d));
        uint32_t* p = F<uint32_t*>(s, 0x59);
        p[0] = mod;
        F<uint32_t*>(s, 0x59)[1] = 0;
        F<uint32_t*>(s, 0x59)[2] = 0;
        F<uint32_t*>(s, 0x59)[3] = 4;
        F<uint32_t*>(s, 0x59)[4] = 0;
        F<uint32_t*>(s, 0x59)[5] = 0;
    }
    if (hook::Global<uint8_t*>(0x6d4024)[0x114250] == 0xff)
        hook::Global<uint8_t*>(0x6d4024)[0x114250] = (uint8_t)Render_ValidateStates(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x35));
}
RT3_REPLACE(0x55ddc0, View_RecordStagePresets);

}  // namespace rt3
