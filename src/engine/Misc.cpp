// Misc.cpp - RT3_mp.exe 0x51e000-0x520000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Engine_ResetState_Impl(uint8_t* e);
template <typename Fn> static inline Fn Slot(void* p, uint32_t off) { return (Fn)Vt(p)[off / 4]; }
void __fastcall KeyTable_Set(uint8_t* t, int, uint8_t k);
void __fastcall KeyTable_Clear(uint8_t* t, int, uint8_t k);
uint8_t __fastcall KeyTable_Get(uint8_t* t, int, uint8_t k);
void* __fastcall Engine_Input(void* e);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
uint32_t __fastcall Engine_IsFullscreen(void* e);
void __fastcall Mouse_Acquire(uint8_t* m);
void __fastcall Mouse_Unacquire(uint8_t* m);
void __fastcall Cursor_Show(int32_t show, int, int32_t);

void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                         int32_t f, int32_t g, int32_t h, int32_t i);

const char* __fastcall GetString(int32_t id);
void __fastcall Keyboard_RestoreScrollLock();
double __fastcall Render_ScaleByUnit(uint8_t*, int, float v);
uint32_t __stdcall HeatColor(float t);
int32_t __fastcall FileExists(const char* path);
void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind);
void __fastcall Engine_SaveConfig(uint8_t* e);
void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint8_t* __fastcall Cursor33_Construct(uint8_t* c);

// RT3_REPLACE(0x4b23d0, Trains_Create); -- see above

int32_t __fastcall Company_Get7680(void* c);

void __fastcall Company_Add7680(void* c, int, int32_t d);
uint8_t* __fastcall TrackLight_Construct(uint8_t* o);
void __fastcall Effect489e80_Release(uint8_t* e);

// RT3_REPLACE(0x493ec0, Tracks_AddPiece); -- see above

void __fastcall Fatal_Exit(const char* msg);

void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall Mouse_Destroy(uint8_t* m);
void __fastcall Display_DestroyRenderer(uint8_t* o);
template <typename T> static inline T& F_engine(void* e, uint32_t off) { return *(T*)((uint8_t*)e + off); }
static int32_t& g_asianText = hook::Global<int32_t>(0x6d4034);

 // DBCS language: text goes to the sorted queue
static int32_t& g_frameCounter = hook::Global<int32_t>(0x6d403c);

static void*& g_engine = hook::Global<void*>(0x6d4024);
static const int32_t* const kTexMemOverride = (const int32_t*)0x624c34;

// Functions of this file, in exe order.
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint32_t __stdcall HeightColor(float t);
RT3_NOINLINE int32_t __fastcall File_Exists(const char* path);
int32_t __fastcall File_Size(const char* path);
int32_t __fastcall Clipboard_SetText(const char* text);
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
int32_t __fastcall Str_IsNumber(const char* s);
RT3_NOINLINE void __fastcall DebugLog_SetFile(const char* name);
RT3_NOINLINE void DebugLog_WriteHeader();
void __fastcall Cursor_Show(int32_t show, int, int32_t);
RT3_NOINLINE void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall Engine_ResetState_Impl(uint8_t* e);
int32_t Engine_ResetState();
RT3_NOINLINE void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind);
RT3_NOINLINE void __fastcall Engine_SaveConfig(uint8_t* e);
void __fastcall Engine_LoadConfig(uint8_t* o);
void __fastcall Engine_SetC(void* e, int, int32_t v);
ATOM __fastcall Engine_RegisterWindowClass(HINSTANCE inst);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall Engine_SetRenderer(void* e, int, void* v);
RT3_NOINLINE void* __fastcall Engine_Get1C(void* e);
void __fastcall Engine_Set1C(void* e, int, void* v);
RT3_NOINLINE void* __fastcall Engine_Input(void* e);
void __fastcall Engine_KeyDown(uint8_t* e, int, uint8_t k);
RT3_NOINLINE void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k);
RT3_NOINLINE uint8_t __fastcall Engine_KeyState(uint8_t* e, int, uint8_t k);
RT3_NOINLINE HWND __fastcall Engine_Window(void* e);
void __fastcall Engine_CurrentMode(void* e, int, int32_t* w, int32_t* h, int32_t* bpp, int32_t* refresh, uint32_t* flags);
RT3_NOINLINE uint32_t __fastcall Engine_IsFullscreen(void* e);
int32_t __fastcall Engine_Get1146f6(void* e);
void __fastcall Engine_Set1146f6(void* e, int, int32_t v);
RT3_NOINLINE void __fastcall Engine_ResetTextureQueue(void* e);
void __fastcall Engine_Enqueue(void* e, int, uint8_t* texture, void* item);
RT3_NOINLINE void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
RT3_NOINLINE void __fastcall Engine_QueueType6(void* e);
RT3_NOINLINE void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex, uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16, uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);
RT3_NOINLINE void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t p11, uint32_t p12);
void __fastcall Engine_SetCursorObject(void* e, int, uint32_t v);
RT3_NOINLINE uint32_t __fastcall Trains_Create(Collection* c, int, int32_t owner);
int32_t __fastcall Tracks_AddPiece(Collection* c, int, float x0, float z0, float x1, float z1, float x2, float z2, float x3, float z3, int32_t kind, int32_t owner, int32_t style);
void __fastcall Engine_EnumDisplayModes(uint8_t* e);
int32_t __fastcall Engine_ModeCount(void* e);
void __fastcall Engine_ModeAt(void* e, int, int32_t i, int32_t* w, int32_t* h, int32_t* bpp);
void __fastcall Engine_ClampWindowSize(void* e, int, int32_t* w, int32_t* h);
void __fastcall Engine_ClearRepaint(void* e);
void __fastcall Engine_RequestRepaint(void* e);
void Engine_Nop();
void __fastcall Engine_SampleStartTextureMem(void* e);
void __fastcall Engine_SampleTextureMem(void* e);
RT3_NOINLINE void __fastcall Engine_SetFreeTexMemOverride(void* e, int, int32_t v);
RT3_NOINLINE void __fastcall Engine_SetTotalTexMemOverride(void* e, int, int32_t v);
RT3_NOINLINE void __fastcall Engine_SetTexMemOverride3(void* e, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Engine_FreeTextureMem(void* e);
RT3_NOINLINE int32_t __fastcall Engine_TotalTextureMem(void* e);
int32_t __fastcall Engine_TextureMemLimit(void* e);
void __fastcall Engine_BuildTrigTables(void* e);
uint8_t __fastcall Engine_CaptureFrontBuffer(uint8_t* e, int, uint8_t* dst, int32_t x, int32_t y, int32_t stride);
void __fastcall Engine_QueueText(void* e, int, uint32_t p1, float x, float y, uint32_t p4, float w, float h, uint32_t p7, uint32_t p8, uint32_t p9);
RT3_NOINLINE int32_t __fastcall Engine_ModeHasFlags(void* e);
RT3_NOINLINE int32_t __fastcall Engine_Get1146ae(void* e);
RT3_NOINLINE int32_t __fastcall Engine_Get11470e(void* e);
void __fastcall Engine_UpdateFrameTime(void* e);
double __fastcall Engine_AverageFrameMs(void* e);
uint8_t* __fastcall Engine_Construct(uint8_t* e);
RT3_NOINLINE void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full);
RT3_NOINLINE void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
RT3_NOINLINE void __fastcall Scene_Shutdown(uint8_t* s);

static inline int32_t ExeOpen(const char* p, int32_t mode) { return ((int32_t(__cdecl*)(const char*, int32_t, ...))0x5a4c57)(p, mode); }

static inline int32_t ExeClose(int32_t fd) { return ((int32_t(__cdecl*)(int32_t))0x5a440d)(fd); }

static inline char* ExeStrncpy(char* d, const char* s, size_t n) { return ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(d, s, n); }

// COM vtable call helpers (D3D8 interfaces, stdcall with `this` first).
static inline void** Vt(void* p) { return *(void***)p; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// One channel of the terrain ramp for v in 0..1 (rising in three linear pieces).
static inline double Rise(double v) {
    if (v < D(0x5c8c20)) return v * D(0x5c8d68) * D(0x5c8628);
    if (v < D(0x5c88e8)) return (v - D(0x5c8c20)) * D(0x5c8d68) * D(0x5c8618) + D(0x5c8628);
    if (v < D(0x5c8880)) return (v - D(0x5c88e8)) * D(0x5c8d68) * D(0x5c8988) + D(0x5c85a0);
    return Fl(0x5c85b4);
}

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// Through the exe's import slot, like the original (lets tests fake the clock).
static inline DWORD Ticks() { return ((DWORD(WINAPI*)())*(void**)0x5c8068)(); }

static inline uint8_t* NewItem(void* e) {
    int32_t n = F_engine<int32_t>(e, 0x58);
    if (n >= 12000) return nullptr;
    F_engine<int32_t>(e, 0x58) = n + 1;
    return (uint8_t*)e + 0x5c + n * 0x5e;
}

// The original does two 4-bit field updates that together copy the whole byte.
static inline void SetLayer(void* e, uint8_t* item) { item[0x5d] = F_engine<uint8_t>(e, 0x114227); }

static inline void EnqueueUntextured(void* e, uint8_t* item) {
    if (g_asianText && F_engine<uint8_t>(e, 0x11369c)) PtrList_Prepend(F_engine<PtrList*>(e, 0x11369d), 0, item);
    else PtrList_Append(F_engine<PtrList*>(e, g_asianText ? 0x11369d : 0x1136a1), 0, item);
}

// IDirect3DDevice8::GetAvailableTextureMem is vtable slot 4.
static inline int32_t AvailableTextureMem(void* dev) {
    return ((int32_t(__stdcall*)(void*))(*(void***)dev)[4])(dev);
}

// 0x51e1d0. Terrain height overlay colour for t in 0..1 (heat ramp in special pixel
// formats): green rising with t, red with 1 - t, blue peaking off mid-range; the top
// end blends towards white. Alpha 0xcf.
uint32_t __stdcall HeightColor(float t) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11474e)) return HeatColor(t);
    if (t < Fl(0x5c8598)) t = 0.0f;
    else if (t > Fl(0x5c85b4)) t = 1.0f;
    double a = Rise(t);
    double u = (double)Fl(0x5c85b4) - t;
    float r;
    if (u < D(0x5c8c20)) r = (float)(D(0x5c8d68) * u * D(0x5c8628));
    else if (u < D(0x5c88e8)) r = (float)((u - D(0x5c8c20)) * D(0x5c8d68) * D(0x5c8618) + D(0x5c8628));
    else if (u < D(0x5c8880)) r = (float)((u - D(0x5c88e8)) * D(0x5c8d68) * D(0x5c8988) + D(0x5c85a0));
    else r = 1.0f;
    float b = 0.4f;
    double dd = D(0x5c8628) - u;
    if (dd < 0) dd = -dd;
    float df = (float)dd;
    if (dd < D(0x5c86e0)) b = 0.0f;
    else if (df < D(0x5dce88)) b = (float)(((double)df - D(0x5c86e0)) * D(0x5dce80));
    if (u > D(0x5c8880)) {
        double x = (D(0x5c8770) - (D(0x5c8770) - u) * D(0x5c87d8)) * D(0x5c8c20);
        b = (float)(b + x);
        a = a + x;
        if (a > Fl(0x5c85b4)) a = Fl(0x5c85b4);
        if (b > Fl(0x5c85b4)) b = 1.0f;
    }
    uint32_t px = ((uint32_t)Ftol((double)r * Fl(0x5ca0ec)) | 0xffffcf00) << 8;
    px = (px | (uint32_t)Ftol(a * Fl(0x5ca0ec))) << 8;
    return (uint32_t)Ftol((double)b * Fl(0x5ca0ec)) | px;
}
RT3_REPLACE(0x51e1d0, HeightColor);

// 0x51e430 / 0x51e480. File exists / file size (exe CRT open, read-only binary).
RT3_NOINLINE int32_t __fastcall File_Exists(const char* path) {
    char buf[0x190];
    char* d = buf;
    while ((*d++ = *path++) != 0) {}
    int32_t fd = ExeOpen(buf, 0x8000);
    if (fd == -1) return 0;
    ExeClose(fd);
    return 1;
}
RT3_REPLACE(0x51e430, File_Exists);

int32_t __fastcall File_Size(const char* path) {
    char buf[0x190];
    char* d = buf;
    while ((*d++ = *path++) != 0) {}
    int32_t fd = ExeOpen(buf, 0x8000);
    if (fd == -1) return 0;
    int32_t size = ((int32_t(__cdecl*)(int32_t, int32_t, int32_t))0x5ac18d)(fd, 0, 2);
    ExeClose(fd);
    return size;
}
RT3_REPLACE(0x51e480, File_Size);

// 0x51e4e0. Copies text to the clipboard (CF_TEXT); 1 on success.
int32_t __fastcall Clipboard_SetText(const char* text) {
    if (!Import<int32_t(__stdcall*)(void*)>(0x5c825c)(nullptr)) return 0;
    if (!Import<int32_t(__stdcall*)()>(0x5c8258)()) return 0;
    size_t len = 0;
    while (text[len]) ++len;
    void* h = Import<void*(__stdcall*)(uint32_t, size_t)>(0x5c8208)(2, len + 1);
    void* p = Import<void*(__stdcall*)(void*)>(0x5c820c)(h);
    Import<char*(__stdcall*)(void*, const char*)>(0x5c8210)(p, text);
    Import<int32_t(__stdcall*)(void*)>(0x5c8214)(h);
    void* ok = Import<void*(__stdcall*)(uint32_t, void*)>(0x5c8224)(1, h);
    Import<int32_t(__stdcall*)()>(0x5c82d8)();
    return ok ? 1 : 0;
}
RT3_REPLACE(0x51e4e0, Clipboard_SetText);

// 0x51e560. Reads clipboard text into buf (size bytes); 1 on success.
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size) {
    if (!Import<int32_t(__stdcall*)(void*)>(0x5c825c)(nullptr)) return 0;
    void* h = Import<void*(__stdcall*)(uint32_t)>(0x5c8260)(1);
    void* p = h ? Import<void*(__stdcall*)(void*)>(0x5c820c)(h) : nullptr;
    if (!p) {
        Import<int32_t(__stdcall*)()>(0x5c82d8)();
        return 0;
    }
    Import<char*(__stdcall*)(char*, const void*, int32_t)>(0x5c8204)(buf, p, size - 1);
    buf[size - 1] = 0;
    Import<int32_t(__stdcall*)(void*)>(0x5c8214)(h);
    Import<int32_t(__stdcall*)()>(0x5c82d8)();
    return 1;
}
RT3_REPLACE(0x51e560, Clipboard_GetText);

// 0x51e5d0. Replaces the first (or, with `all`, every) occurrence of `find`
// in str (buffer of `size` bytes) by `repl`.
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all) {
    for (;;) {
        char* tmp = (char*)Malloc(size + 0x1f4);
        char* p = (char*)StrFind(str, find, 0);
        int32_t found = 0;
        if (p) {
            *p = 0;
            char* d = tmp;
            const char* s = str;
            while ((*d++ = *s++) != 0) {}
            size_t flen = 0;
            while (find[flen]) ++flen;
            d = tmp;
            while (*d) ++d;
            for (s = repl; (*d++ = *s++) != 0;) {}
            d = tmp;
            while (*d) ++d;
            for (s = p + flen; (*d++ = *s++) != 0;) {}
            ExeStrncpy(str, tmp, size - 1);
            str[size - 1] = 0;
            found = 1;
        }
        Free(tmp);
        if (!found || !all) return;
    }
}
RT3_REPLACE(0x51e5d0, Str_Replace);

// 0x51e6e0. Non-empty and all decimal digits.
int32_t __fastcall Str_IsNumber(const char* s) {
    if (!s) return 0;
    size_t n = 0;
    while (s[n]) ++n;
    if (!n) return 0;
    for (size_t i = 0; i < n; ++i)
        if (s[i] < '0' || s[i] > '9') return 0;
    return 1;
}
RT3_REPLACE(0x51e6e0, Str_IsNumber);

// 0x51e720. Sets the debug log file name (0x6d3ca8, 100 chars) and resets its state.
RT3_NOINLINE void __fastcall DebugLog_SetFile(const char* name) {
    ExeStrncpy((char*)0x6d3ca8, name, 100);
    hook::Global<uint8_t>(0x6d3d0c) = 0;
    hook::Global<int32_t>(0x6d3d10) = 0;
}
RT3_REPLACE(0x51e720, DebugLog_SetFile);

// 0x51e740. Writes the separator line to the debug log once per session
// (default file rt3_debug.txt in .\data\log\).
RT3_NOINLINE void DebugLog_WriteHeader() {
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

// 0x51e810. Shows or hides the Windows cursor (nesting count 0x624b20, kept at 0/1)
// by calling ShowCursor until its count agrees, (un)acquires the mouse and, in
// fullscreen, puts the cursor at the game's mouse position.
RT3_NOINLINE void __fastcall Cursor_Show(int32_t show, int, int32_t) {
    int32_t& cnt = hook::Global<int32_t>(0x624b20);
    cnt = show ? cnt + 1 : cnt - 1;
    if (cnt && cnt != 1) cnt = 1;
    int32_t px, py;
    void* eng = hook::Global<void*>(0x6d4024);
    if (eng && Engine_Input(eng)) {
        float fx, fy;
        Mouse_GetPos((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)), 0, (uint32_t*)&fx, (uint32_t*)&fy);
        uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
        const int32_t* res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
        px = (int32_t)((double)res[0] * fx);
        py = (int32_t)((double)res[1] * fy);
    } else {
        px = py = 100;
    }
    int32_t on = hook::Global<int32_t>(0x624b20) > 0 ? 1 : 0;
    auto showCursor = Import<int32_t(__stdcall*)(int32_t)>(0x5c82d4);
    int32_t r = showCursor(on);
    int32_t k = 0;
    if (on) {
        if (r <= 0)
            while (k++ < 0x3e7 && showCursor(on) <= 0) {}
    } else if (r >= 1) {
        while (k++ < 0x3e7 && showCursor(0) >= 1) {}
    }
    showCursor(on);
    showCursor(on);
    showCursor(on);
    if (!hook::Global<void*>(0x6d4024) || !Engine_Input(hook::Global<void*>(0x6d4024))) return;
    if (Engine_IsFullscreen(hook::Global<void*>(0x6d4024)) && !on)
        Mouse_Acquire((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)));
    else
        Mouse_Unacquire((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)));
    if (Engine_IsFullscreen(hook::Global<void*>(0x6d4024))) Import<int32_t(__stdcall*)(int32_t, int32_t)>(0x5c8264)(px, py);
}
RT3_REPLACE(0x51e810, Cursor_Show);

// 0x51e980. Fatal error: once (0x6d3d18), flags the engine (+0x57), shows the
// cursor, logs msg (or string 0x45) and shows it in a message box (title string
// 0x44), restores the keyboard and exits.
RT3_NOINLINE void __fastcall Fatal_Exit(const char* msg) {
    if (hook::Global<int32_t>(0x6d3d18)) return;
    hook::Global<int32_t>(0x6d3d18) = 1;
    if (uint8_t* e = hook::Global<uint8_t*>(0x6d4024)) e[0x57] = 1;
    Cursor_Show(1, 0x5dcea4, 0x110);
    const int32_t k = (int32_t)0xfffffb2e;
    if (msg) {
        char buf[0x300];
        char* d = buf;
        while ((*d++ = *msg++) != 0) {}
        DebugLog(buf, k, k, k, k, k, k, k, k, k);
        Import<int32_t(__stdcall*)(void*, const char*, const char*, uint32_t)>(0x5c8220)(nullptr, buf, GetString(0x44), 0x10);
    } else {
        DebugLog(GetString(0x45), k, k, k, k, k, k, k, k, k);
    }
    Keyboard_RestoreScrollLock();
    ((void(__cdecl*)(int))0x5a2e9c)(0);
}
RT3_REPLACE(0x51e980, Fatal_Exit);

// 0x51ea80. Resets the engine state fields.
RT3_NOINLINE int32_t __fastcall Engine_ResetState_Impl(uint8_t* e) {
    e[0x114253] = 0xff;
    e[0x11422e] = 0xff;
    e[0x114250] = 0xff;
    e[0x114251] = 0xff;
    F<int32_t>(e, 0x114212) = 0;
    e[0x114255] = 1;
    F<int32_t>(e, 0xc) = 0;
    F<int32_t>(e, 0x114243) = 0;
    F<int32_t>(e, 0x114247) = 0;
    F<int32_t>(e, 0x11427e) = 0;
    F<int32_t>(e, 0x114282) = 0;
    F<int32_t>(e, 0x1141f5) = 0;
    F<int32_t>(e, 0x1141f9) = 0;
    F<int32_t>(e, 0x1141fd) = 0;
    F<int32_t>(e, 0x4d) = 0;
    F<int32_t>(e, 0x51) = 0;
    F<int32_t>(e, 0x28) = 0;
    e[0x56] = 0;
    F<int32_t>(e, 0x58) = 0;
    F<int32_t>(e, 0x4) = 0;
    F<int32_t>(e, 0x0) = 0;
    F<int32_t>(e, 0x18) = 0;
    F<int32_t>(e, 0x1c) = 0;
    F<int32_t>(e, 0x8) = 0;
    F<int32_t>(e, 0x11420a) = 0;
    F<int32_t>(e, 0x11421a) = 1;
    F<int32_t>(e, 0x11420e) = 0x5c;
    F<int32_t>(e, 0x114216) = 0;
    F<int32_t>(e, 0x114233) = 0;
    e[0x11422c] = 0;
    e[0x114252] = 0;
    e[0x114231] = 0;
    e[0x11422d] = 0;
    e[0x114230] = 0;
    e[0x11422f] = 0;
    e[0x114254] = 1;
    e[0x114232] = 1;
    F<int32_t>(e, 0x114237) = 0;
    F<int32_t>(e, 0x114228) = 2;
    e[0x57] = 0;
    F<int32_t>(e, 0x1136ad) = 0;
    F<int32_t>(e, 0x1136a9) = 0;
    F<int32_t>(e, 0x2c) = 0;
    F<int32_t>(e, 0x114276) = 0;
    F<int32_t>(e, 0x11425a) = 0;
    F<int32_t>(e, 0x11425e) = 0;
    F<int32_t>(e, 0x114262) = 0;
    F<int32_t>(e, 0x114266) = 0;
    F<int32_t>(e, 0x11426a) = 0;
    F<int32_t>(e, 0x11426e) = 0;
    F<int32_t>(e, 0x114272) = 0;
    F<int32_t>(e, 0x11427a) = 0;
    F<int32_t>(e, 0x114256) = 0;
    e[0x114227] = 0x11;
    return 0;
}

// 0x51ffa5 keeps using ECX (the engine) after the call.
__declspec(naked) int32_t Engine_ResetState() { RT3_KEEP_ECX_EDX(Engine_ResetState_Impl, 0) }
RT3_REPLACE(0x51ea80, Engine_ResetState);

// 0x51ebc0. Resets the renderer settings (+0x11468a..) to defaults for the screen
// mode (0x6d4028: 0/9 = 1024x768 windowed defaults, else 640x480; 3/4 no colour
// scale) and the session kind `kind` (0 = single player).
RT3_NOINLINE void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind) {
    memset(o + 0x11468a, 0, 0x79 * 4);
    F<uint32_t>(o, 0x1146fa) = 0x3f800000;
    F<uint32_t>(o, 0x1146fe) = 0x3f800000;
    F<uint32_t>(o, 0x114702) = 0x3f800000;
    int32_t mode = hook::Global<int32_t>(0x6d4028);
    if (mode != 0 && mode != 9) {
        F<int32_t>(o, 0x11473e) = 0;
        F<int32_t>(o, 0x11473a) = 0;
        F<int32_t>(o, 0x11470e) = 1;
        F<int32_t>(o, 0x11468a) = 0x280;
        F<int32_t>(o, 0x11468e) = 0x1e0;
        mode = hook::Global<int32_t>(0x6d4028);
        o[0x114696] = (mode == 1 || mode == 3) ? 1 : 0;
        F<int32_t>(o, 0x114706) = 0xc;
        F<int32_t>(o, 0x11470a) = 0x63;
        mode = hook::Global<int32_t>(0x6d4028);
        if (mode == 4 || mode == 3) {
            F<int32_t>(o, 0x1146fa) = 0;
            F<int32_t>(o, 0x1146fe) = 0;
            F<int32_t>(o, 0x114702) = 0;
        }
    } else {
        F<int32_t>(o, 0x11470e) = 0;
        F<int32_t>(o, 0x11468a) = 0x400;
        F<int32_t>(o, 0x11468e) = 0x300;
        o[0x114696] = 1;
        F<int32_t>(o, 0x114706) = 3;
        F<int32_t>(o, 0x11470a) = 0x63;
        F<int32_t>(o, 0x11473e) = 0;
        F<int32_t>(o, 0x11473a) = 1;
    }
    F<int32_t>(o, 0x114692) = 0x20;
    static const uint32_t ones1[] = { 0x1146ec, 0x1146e9, 0x1146ea, 0x1146ef, 0x1146f0, 0x11469b, 0x1146a5, 0x1146a6, 0x1146a7 };
    for (uint32_t off : ones1) o[off] = 1;
    o[0x1146a8] = 0;
    o[0x1146a9] = 1;
    o[0x1146aa] = 1;
    F<int32_t>(o, 0x114697) = 1;
    F<int32_t>(o, 0x114736) = 1;
    F<int32_t>(o, 0x11469c) = 0;
    F<int32_t>(o, 0x1146be) = 0;
    F<int32_t>(o, 0x1146c2) = -1;
    F<int32_t>(o, 0x1146c6) = 0;
    F<int32_t>(o, 0x1146ca) = 0;
    F<int32_t>(o, 0x1146ce) = 0;
    o[0x1146d7] = 0;
    o[0x1146d8] = 0;
    o[0x1146bd] = 0;
    o[0x1146d2] = 0;
    F<float>(o, 0x1146d3) = (float)Render_ScaleByUnit(nullptr, 0, 60.0f);
    o[0x1146a4] = 0;
    o[0x1146a3] = 0;
    o[0x1146a2] = 0;
    o[0x1146a1] = 0;
    static const uint32_t ones2[] = { 0x1146e1, 0x1146e2, 0x1146e3, 0x1146e6, 0x1146e4, 0x1146e5, 0x1146e7,
                                      0x1146ed, 0x1146ee, 0x1146e8, 0x1146eb, 0x1146ab, 0x1146ac, 0x1146ad };
    for (uint32_t off : ones2) o[off] = 1;
    o[0x1146ae] = 0;
    F<int32_t>(o, 0x1146b3) = 2;
    F<int32_t>(o, 0x1146af) = 0;
    o[0x1146b7] = 1;
    o[0x1146b8] = 1;
    F<int32_t>(o, 0x1146f6) = 0;
    F<uint32_t>(o, 0x1146b9) = 0x3f800000;
    o[0x1146f1] = 1;
    F<int32_t>(o, 0x1146f2) = 0;
    F<int32_t>(o, 0x114732) = 0;
    F<int32_t>(o, 0x1146d9) = 0;
    F<int32_t>(o, 0x1146dd) = 0;
    F<int32_t>(o, 0x114742) = 0;
    hook::Global<int32_t>(0x6d4028) = 0;
    F<int32_t>(o, 0x114712) = 1;
    if (kind == 0) {
        F<int32_t>(o, 0x114716) = 1;
        F<int32_t>(o, 0x11472a) = 1;
        F<int32_t>(o, 0x11472e) = 0;
    } else if (kind == 2 || kind == 1) {
        o[0x114696] = 0;
        F<int32_t>(o, 0x114716) = 0;
        F<int32_t>(o, 0x11472a) = 0;
        F<int32_t>(o, 0x11472e) = 0;
    }
    F<int32_t>(o, 0x11471a) = 0;
    F<int32_t>(o, 0x114722) = 0;
    F<int32_t>(o, 0x11471e) = 1;
    F<int32_t>(o, 0x114726) = 0x96;
}
RT3_REPLACE(0x51ebc0, Renderer_ResetSettings);

// 0x51eea0. Saves the engine configuration (version 0x41b, 0x1e4 bytes from
// +0x11468a, 0x1ec bytes of padding) to data\configuration\engine.cfg.
RT3_NOINLINE void __fastcall Engine_SaveConfig(uint8_t* e) {
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

// 0x51ef20. Loads the renderer settings from data\configuration\engine.cfg (version
// 0x41b; not in a forced screen mode), else resets them to defaults and saves.
void __fastcall Engine_LoadConfig(uint8_t* o) {
    const char* path = (const char*)0x5dcec8;
    if (FileExists(path) && !hook::Global<int32_t>(0x6d4028)) {
        Stream s = StreamOpen(path, 1, 0x30d40, 1);
        int32_t version;
        uint8_t rest[0x1ec];
        StreamRead(s, &version, 4, 0, 1);
        if (version == 0x41b) {
            StreamRead(s, o + 0x11468a, 0x1e4, 0, 1);
            StreamRead(s, rest, 0x1ec, 0, 1);
            StreamClose(s);
            if (F<int32_t>(o, 0x11474a)) hook::Global<uint8_t*>(0x6d4024)[0x11422e] = 0;
            return;
        }
        StreamClose(s);
    }
    Renderer_ResetSettings(o, 0, 0);
    Engine_SaveConfig(o);
}
RT3_REPLACE(0x51ef20, Engine_LoadConfig);

void __fastcall Engine_SetC(void* e, int, int32_t v) { F_engine<int32_t>(e, 0xc) = v; }
RT3_REPLACE(0x51eff0, Engine_SetC);

 // 0x54e3a0 (original for now)

ATOM __fastcall Engine_RegisterWindowClass(HINSTANCE inst) {
    WNDCLASSEXA wc;
    wc.cbSize = sizeof wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)0x54e3a0;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(inst, (LPCSTR)0x7f00);
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "World";
    wc.hIconSm = nullptr;
    return RegisterClassExA(&wc);
}
RT3_REPLACE(0x51f000, Engine_RegisterWindowClass);

RT3_NOINLINE void* __fastcall Engine_Renderer(void* e) { return F_engine<void*>(e, 0x18); }
RT3_REPLACE(0x51f070, Engine_Renderer);

void __fastcall Engine_SetRenderer(void* e, int, void* v) { F_engine<void*>(e, 0x18) = v; }
RT3_REPLACE(0x51f080, Engine_SetRenderer);

RT3_NOINLINE void* __fastcall Engine_Get1C(void* e) { return F_engine<void*>(e, 0x1c); }
RT3_REPLACE(0x51f090, Engine_Get1C);

void __fastcall Engine_Set1C(void* e, int, void* v) { F_engine<void*>(e, 0x1c) = v; }
RT3_REPLACE(0x51f0a0, Engine_Set1C);

RT3_NOINLINE void* __fastcall Engine_Input(void* e) { return F_engine<void*>(e, 0x20); }
RT3_REPLACE(0x51f0b0, Engine_Input);

// 0x51f0c0..0x51f0e0. Engine key table (+0x24) forwarders.
void __fastcall Engine_KeyDown(uint8_t* e, int, uint8_t k) { KeyTable_Set(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0c0, Engine_KeyDown);

RT3_NOINLINE void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k) { KeyTable_Clear(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0d0, Engine_KeyUp);

RT3_NOINLINE uint8_t __fastcall Engine_KeyState(uint8_t* e, int, uint8_t k) { return KeyTable_Get(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0e0, Engine_KeyState);

RT3_NOINLINE HWND __fastcall Engine_Window(void* e) { return F_engine<HWND>(e, 0); }
RT3_REPLACE(0x51f0f0, Engine_Window);

// Current display mode. Bpp is reported as 0 in windowed-with-desktop mode.
void __fastcall Engine_CurrentMode(void* e, int, int32_t* w, int32_t* h, int32_t* bpp, int32_t* refresh, uint32_t* flags) {
    uint8_t* m = F_engine<uint8_t*>(e, 0x34) + F_engine<int32_t>(e, 0x11421e) * 0x15;
    *w = *(int32_t*)m;
    *h = *(int32_t*)(m + 4);
    if (bpp) *bpp = *(int32_t*)(m + 8);
    if (refresh) *refresh = F_engine<int32_t>(e, 0x11470e) == 0 ? *(int32_t*)(m + 0xc) : 0;
    if (flags) *flags = m[0x14];
}
RT3_REPLACE(0x51f100, Engine_CurrentMode);

RT3_NOINLINE uint32_t __fastcall Engine_IsFullscreen(void* e) { return F_engine<uint8_t>(e, 0x114226); }
RT3_REPLACE(0x51f1a0, Engine_IsFullscreen);

int32_t __fastcall Engine_Get1146f6(void* e) { return F_engine<int32_t>(e, 0x1146f6); }
RT3_REPLACE(0x51f1b0, Engine_Get1146f6);

void __fastcall Engine_Set1146f6(void* e, int, int32_t v) { F_engine<int32_t>(e, 0x1146f6) = v; }
RT3_REPLACE(0x51f1c0, Engine_Set1146f6);

// Unlinks every texture that has queued items from the texture queue.
RT3_NOINLINE void __fastcall Engine_ResetTextureQueue(void* e) {
    PtrList* textures = F_engine<PtrList*>(e, 0x1136a5);
    PtrList_Rewind(textures);
    for (uint8_t* tex = (uint8_t*)PtrList_Next(textures); tex; tex = (uint8_t*)PtrList_Next(textures)) {
        if (PtrList_TailValue(*(PtrList**)(tex + 0xe4))) {
            *(void**)(tex + 0xe8) = nullptr;
            PtrList_Append(*(PtrList**)(tex + 0xe4), 0, nullptr);
        }
    }
}
RT3_REPLACE(0x51f1d0, Engine_ResetTextureQueue);

// Routes a queue item: textured items go to their texture's list (registering
// the texture once), untextured ones to the default queue; in DBCS mode with
// sorting on, everything goes to the sorted queue.
void __fastcall Engine_Enqueue(void* e, int, uint8_t* texture, void* item) {
    if (g_asianText) {
        if (F_engine<uint8_t>(e, 0x11369c)) PtrList_Prepend(F_engine<PtrList*>(e, 0x11369d), 0, item);
        else PtrList_Append(F_engine<PtrList*>(e, 0x11369d), 0, item);
        return;
    }
    if (texture) {
        if (*(void**)(texture + 0xe8) == nullptr)
            *(PtrListNode**)(texture + 0xe8) = PtrList_Append(F_engine<PtrList*>(e, 0x1136a5), 0, texture);
        PtrList_Append(*(PtrList**)(texture + 0xe4), 0, item);
        return;
    }
    PtrList_Append(F_engine<PtrList*>(e, 0x1136a1), 0, item);
}
RT3_REPLACE(0x51f230, Engine_Enqueue);

// Item type 5 (4 dwords).
RT3_NOINLINE void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 1) = a;
    *(uint32_t*)(it + 5) = b;
    *(uint32_t*)(it + 9) = c;
    it[0] = 5;
    *(uint32_t*)(it + 0xd) = d;
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51f2b0, Engine_QueueType5);

// Item type 6 (no data).
RT3_NOINLINE void __fastcall Engine_QueueType6(void* e) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    it[0] = 6;
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51f320, Engine_QueueType6);

// Item type 4: textured quad (20 dwords; p11 = texture).
RT3_NOINLINE void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 0x01) = p1;  *(uint32_t*)(it + 0x05) = p2;  *(uint32_t*)(it + 0x09) = p3;
    *(uint32_t*)(it + 0x0d) = p4;  *(uint32_t*)(it + 0x11) = p5;  *(uint32_t*)(it + 0x15) = p6;
    *(uint32_t*)(it + 0x19) = p7;  *(uint32_t*)(it + 0x45) = p12; *(uint32_t*)(it + 0x49) = p13;
    *(uint32_t*)(it + 0x4d) = p14; *(uint32_t*)(it + 0x51) = p15; *(uint32_t*)(it + 0x55) = p16;
    *(uint32_t*)(it + 0x59) = p17; *(uint32_t*)(it + 0x29) = p18; *(uint32_t*)(it + 0x2d) = p19;
    *(uint32_t*)(it + 0x31) = p20; *(uint32_t*)(it + 0x1d) = p8;  *(uint32_t*)(it + 0x21) = p9;
    *(uint32_t*)(it + 0x25) = p10;
    it[0] = 4;
    SetLayer(e, it);
    *(uint32_t*)(it + 0x41) = tex;
    Engine_Enqueue(e, 0, (uint8_t*)tex, it);
}
RT3_REPLACE(0x51f370, Engine_QueueQuad);

// Item type 1: textured sprite (12 dwords; p7 = texture).
RT3_NOINLINE void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 0x01) = p1;  *(uint32_t*)(it + 0x05) = p2;  *(uint32_t*)(it + 0x09) = p3;
    *(uint32_t*)(it + 0x0d) = p4;  *(uint32_t*)(it + 0x19) = p5;  *(uint32_t*)(it + 0x45) = p8;
    *(uint32_t*)(it + 0x49) = p9;  *(uint32_t*)(it + 0x4d) = p10; *(uint32_t*)(it + 0x51) = p11;
    *(uint32_t*)(it + 0x29) = p12; *(uint32_t*)(it + 0x1d) = p6;
    it[0] = 1;
    SetLayer(e, it);
    *(uint32_t*)(it + 0x41) = tex;
    Engine_Enqueue(e, 0, (uint8_t*)tex, it);
}
RT3_REPLACE(0x51f460, Engine_QueueSprite);

void __fastcall Engine_SetCursorObject(void* e, int, uint32_t v) {
    if (F_engine<int32_t>(e, 0xc) != 0) ((void(__fastcall*)(void*, uint32_t))0x547f20)(F_engine<void*>(e, 0xc), v);
    F_engine<uint32_t>(e, 0x1146b9) = v;
}
RT3_REPLACE(0x51f510, Engine_SetCursorObject);

// 0x4b23d0. Adds a train to table c for `owner`: a new record copied from a local
// whose only initialised part is its stop cursor, then Train_Init; returns its id.
// NOT INSTALLED: every byte neither part writes is copied from uninitialized stack
// into the (saved) record, so the original's result depends on stack residue. The
// exe's code stays; here the local starts zeroed.
RT3_NOINLINE uint32_t __fastcall Trains_Create(Collection* c, int, int32_t owner) {
    uint8_t local[0x1d8] = {};
    Cursor33_Construct(local);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x4b23f0)(
        c, 0, local, 0, 0, 0);
    hook::CallTarget<void(__fastcall*)(uint8_t*, int, int32_t, int32_t)>(0x4b240a)((uint8_t*)Collection_Get(c, 0, id), 0, (int32_t)id,
                                                                                   owner);
    return id;
}

// 0x493ec0. Adds a track piece with corners (x, z cell pairs) to table c for
// company `owner`: every coordinate must lie on the map (quirk: the sixth value's
// lower bound check tests the eighth instead), and a valid owner must have pieces
// left (+0x7680 counter); the owner is charged one (two for style 4); the piece
// gets the next group stamp (c +0x90 += 2), is initialised by Track_Init (the exe's
// code), registered in the node grid when placed or live, and the first group
// becomes the table's (+0x88). Returns its id, or -1.
// NOT INSTALLED: the record is copied from a local of which only the track-light
// part is initialised, so leftover bytes come from stack residue (and Track_Init
// itself reads uninitialized heights). The exe's code stays; the local starts
// zeroed here.
int32_t __fastcall Tracks_AddPiece(Collection* c, int, float x0, float z0, float x1, float z1, float x2, float z2, float x3, float z3,
                                   int32_t kind, int32_t owner, int32_t style) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    float w = (float)F<int32_t>(m, 0x2155), h = (float)F<int32_t>(m, 0x2159);
    auto bad = [](float v, float hi) { return v < 0.0f || v > hi; };
    if (bad(x0, w) || bad(z0, h) || bad(x1, w) || bad(z1, h) || bad(x2, w) || z3 < 0.0f || z2 > h || bad(x3, w) || bad(z3, h))
        return -1;
    uint16_t co = (uint16_t)owner;
    Collection* companies = hook::Global<Collection*>(0x62be10);
    if (Collection_IsValid(companies, 0, co) && !Company_Get7680(Collection_Get(hook::Global<Collection*>(0x62be10), 0, co)))
        return -1;
    uint8_t local[0x260] = {};
    TrackLight_Construct(local);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x4940a1)(
        c, 0, local, 0x257, 0, 0);
    if (co) Company_Add7680(Collection_Get(hook::Global<Collection*>(0x62be10), 0, co), 0, (uint8_t)style != 4 ? -1 : -2);
    uint8_t* t = (uint8_t*)Collection_Get(c, 0, id);
    F<int32_t>(c, 0x90) += 2;
    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, float, float, float, float, float, float, float, float, int32_t, int32_t,
                        int32_t))0x48f2b0)(t, 0, (int32_t)id, F<int32_t>(c, 0x90), x0, z0, x1, z1, x2, z2, x3, z3, kind, owner, style);
    if (t[0x216] || F<int32_t>(c, 0xe9)) Grid_AddObject((uint8_t*)c, 0, t);
    if (F<int32_t>(c, 0x88) == -1) F<int32_t>(c, 0x88) = F<int32_t>(t, 0x240);
    Effect489e80_Release(local);
    return (int32_t)id;
}

// 0x51f540. Builds the engine's display mode list (+0x34, count +0x30; 0x15-byte
// entries {w, h, bpp, refresh, format, supported}) from Direct3D 8: records the
// desktop mode (+0x38 w, +0x3c h, +0x44 refresh, +0x40 bpp: 32 for R8G8B8 /
// X8R8G8B8, 16 for R5G6B5 / X1R5G5B5; any other desktop format, A8R8G8B8
// included, is fatal), then keeps every adapter mode of at least 600x400, not
// taller than wide, at most 100 Hz, in X8R8G8B8 (32 bits) or R5G6B5 / X1R5G5B5
// (16), flagged when the HAL device accepts it windowed. No Direct3D, no modes, no
// landscape 16/32-bit mode or a leaked interface are fatal (strings 0x17..0x1b).
void __fastcall Engine_EnumDisplayModes(uint8_t* e) {
    void* d3d = ((void*(__stdcall*)(uint32_t))hook::Global<void*>(0x5c8398))(0xdc);
    if (!d3d) Fatal_Exit(GetString(0x17));
    void** vt = *(void***)d3d;
    F<int32_t>(e, 0x30) = ((int32_t(__stdcall*)(void*, uint32_t))vt[0x18 / 4])(d3d, 0);
    if (F<int32_t>(e, 0x30) <= 0) Fatal_Exit(GetString(0x18));
    uint32_t mode[4]; // width, height, refresh, format
    ((long(__stdcall*)(void*, uint32_t, uint32_t*))vt[0x20 / 4])(d3d, 0, mode);
    switch (mode[3]) {
    case 0x14:
    case 0x16:
        F<uint32_t>(e, 0x38) = mode[0];
        F<uint32_t>(e, 0x3c) = mode[1];
        F<uint32_t>(e, 0x44) = mode[2];
        F<int32_t>(e, 0x40) = 0x20;
        break;
    case 0x17:
    case 0x18:
        F<uint32_t>(e, 0x38) = mode[0];
        F<uint32_t>(e, 0x3c) = mode[1];
        F<uint32_t>(e, 0x44) = mode[2];
        F<int32_t>(e, 0x40) = 0x10;
        break;
    default: {
        ((uint32_t(__stdcall*)(void*))vt[0x8 / 4])(d3d);
        char num[0x40], text[0x80];
        Num_Format(num, 1, (double)(int32_t)mode[3], 10, 1, 0, 0);
        FormatText(text, GetString(0x19), num, 0, 0, 0, 0, 0, 0, 0);
        Fatal_Exit(text);
    }
    }
    int32_t usable = 0;
    for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i) {
        ((long(__stdcall*)(void*, uint32_t, uint32_t, uint32_t*))vt[0x1c / 4])(d3d, 0, (uint32_t)i, mode);
        if ((int32_t)mode[3] >= 0x16 && (int32_t)mode[3] <= 0x18 && mode[0] > mode[1]) ++usable;
    }
    if (F<int32_t>(e, 0x30) <= 0 || usable <= 0) {
        ((uint32_t(__stdcall*)(void*))vt[0x8 / 4])(d3d);
        Fatal_Exit(GetString(0x1a));
    }
    int32_t n = usable > F<int32_t>(e, 0x30) ? usable : F<int32_t>(e, 0x30);
    size_t bytes = (size_t)(n + 5) * 0x15;
    uint8_t* list = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)(bytes);
    F<uint8_t*>(e, 0x34) = list;
    memset(list, 0, bytes);
    int32_t kept = 0;
    for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i) {
        ((long(__stdcall*)(void*, uint32_t, uint32_t, uint32_t*))vt[0x1c / 4])(d3d, 0, (uint32_t)i, mode);
        if (mode[0] < 600 || mode[1] < 400 || mode[0] < mode[1]) continue;
        // The 100 Hz cap is the JA at 0x51f71b, which DesyncWatch's all_resolutions
        // option NOPs out at runtime: follow the live code.
        if (mode[2] > 100 && *(const uint8_t*)0x51f71b != 0x90) continue;
        uint32_t bpp;
        if (mode[3] == 0x16) bpp = 0x20;
        else if ((int32_t)mode[3] > 0x16 && (int32_t)mode[3] <= 0x18) bpp = 0x10;
        else continue;
        uint8_t* m = F<uint8_t*>(e, 0x34) + kept * 0x15;
        F<uint32_t>(m, 0) = mode[0];
        F<uint32_t>(F<uint8_t*>(e, 0x34) + kept * 0x15, 4) = mode[1];
        F<uint32_t>(F<uint8_t*>(e, 0x34) + kept * 0x15, 0xc) = mode[2];
        F<uint32_t>(F<uint8_t*>(e, 0x34) + kept * 0x15, 8) = bpp;
        F<uint32_t>(F<uint8_t*>(e, 0x34) + kept * 0x15, 0x10) = mode[3];
        F<uint8_t*>(e, 0x34)[kept * 0x15 + 0x14] = 0;
        if (!((long(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, int32_t))vt[0x2c / 4])(d3d, 0, 1, mode[3], 0, 2))
            F<uint8_t*>(e, 0x34)[kept * 0x15 + 0x14] = 1;
        ++kept;
    }
    F<int32_t>(e, 0x30) = kept;
    if (((uint32_t(__stdcall*)(void*))vt[0x8 / 4])(d3d)) Fatal_Exit(GetString(0x1b));
}
RT3_REPLACE(0x51f540, Engine_EnumDisplayModes);

int32_t __fastcall Engine_ModeCount(void* e) { return F_engine<int32_t>(e, 0x30); }
RT3_REPLACE(0x51f820, Engine_ModeCount);

void __fastcall Engine_ModeAt(void* e, int, int32_t i, int32_t* w, int32_t* h, int32_t* bpp) {
    uint8_t* m = F_engine<uint8_t*>(e, 0x34) + i * 0x15;
    *w = *(int32_t*)m;
    *h = *(int32_t*)(m + 4);
    *bpp = *(int32_t*)(m + 8);
}
RT3_REPLACE(0x51f830, Engine_ModeAt);

// Windowed sizes that reach the desktop size are snapped down to the next
// smaller standard size (512x384 .. 1600x1200).
void __fastcall Engine_ClampWindowSize(void* e, int, int32_t* w, int32_t* h) {
    static const int32_t kW[6] = { 512, 640, 800, 1024, 1280, 1600 };
    static const int32_t kH[6] = { 384, 480, 600, 768, 1024, 1200 };
    if (F_engine<uint8_t>(e, 0x114696) != 0) return;
    if (F_engine<int32_t>(e, 0x38) > *w && F_engine<int32_t>(e, 0x3c) > *h) return;
    int i = 0;
    for (; i < 6; ++i) {
        if (*w <= kW[i] && *h <= kH[i]) {
            if (--i < 0) i = 0;
            break;
        }
    }
    if (i > 5) i = 5;
    *w = kW[i];
    *h = kH[i];
}
RT3_REPLACE(0x51f870, Engine_ClampWindowSize);

void __fastcall Engine_ClearRepaint(void* e) { F_engine<uint8_t>(e, 0x56) = 0; }
RT3_REPLACE(0x51f930, Engine_ClearRepaint);

void __fastcall Engine_RequestRepaint(void* e) { if (F_engine<int32_t>(e, 0x1c)) F_engine<uint8_t>(e, 0x56) = 1; }
RT3_REPLACE(0x51f940, Engine_RequestRepaint);

void Engine_Nop() { __asm nop }
RT3_REPLACE(0x51f950, Engine_Nop);

void __fastcall Engine_SampleStartTextureMem(void* e) {
    if (F_engine<int32_t>(e, 0x1136a9) == 0 && F_engine<void*>(e, 8)) F_engine<int32_t>(e, 0x1136a9) = AvailableTextureMem(F_engine<void*>(e, 8));
}
RT3_REPLACE(0x51f960, Engine_SampleStartTextureMem);

void __fastcall Engine_SampleTextureMem(void* e) {
    if (F_engine<void*>(e, 8)) F_engine<int32_t>(e, 0x1136ad) = AvailableTextureMem(F_engine<void*>(e, 8));
}
RT3_REPLACE(0x51f990, Engine_SampleTextureMem);

RT3_NOINLINE void __fastcall Engine_SetFreeTexMemOverride(void* e, int, int32_t v) { F_engine<int32_t>(e, 0x1146ca) = v; }
RT3_REPLACE(0x51f9b0, Engine_SetFreeTexMemOverride);

RT3_NOINLINE void __fastcall Engine_SetTotalTexMemOverride(void* e, int, int32_t v) {
    F_engine<int32_t>(e, 0x1146c6) = v;
    if (hook::Global<int32_t>(0x6d401c)) ((void(*)())0x538ec0)();
}
RT3_REPLACE(0x51f9c0, Engine_SetTotalTexMemOverride);

RT3_NOINLINE void __fastcall Engine_SetTexMemOverride3(void* e, int, int32_t v) {
    F_engine<int32_t>(e, 0x1146ce) = v;
    if (hook::Global<int32_t>(0x6d401c)) ((void(*)())0x538ec0)();
}
RT3_REPLACE(0x51f9e0, Engine_SetTexMemOverride3);

// Free texture memory, honouring the settings overrides (table 0x624c34).
RT3_NOINLINE int32_t __fastcall Engine_FreeTextureMem(void* e) {
    if (F_engine<int32_t>(e, 0x1146ca)) return kTexMemOverride[F_engine<int32_t>(e, 0x1146ca)];
    int32_t v = F_engine<int32_t>(e, 0x1136ad);
    if (F_engine<int32_t>(e, 0x1146c6)) v += kTexMemOverride[F_engine<int32_t>(e, 0x1146c6)] - F_engine<int32_t>(e, 0x1136a9);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x51fa00, Engine_FreeTextureMem);

RT3_NOINLINE int32_t __fastcall Engine_TotalTextureMem(void* e) {
    if (F_engine<int32_t>(e, 0x1146c6)) return kTexMemOverride[F_engine<int32_t>(e, 0x1146c6)];
    return F_engine<int32_t>(e, 0x1136a9);
}
RT3_REPLACE(0x51fa40, Engine_TotalTextureMem);

int32_t __fastcall Engine_TextureMemLimit(void* e) {
    if (F_engine<int32_t>(e, 0x1146ce)) return kTexMemOverride[F_engine<int32_t>(e, 0x1146ce)];
    int32_t v = F_engine<int32_t>(e, 0x1146c6) ? kTexMemOverride[F_engine<int32_t>(e, 0x1146c6)] : F_engine<int32_t>(e, 0x1136a9);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x51fa60, Engine_TextureMemLimit);

// sin/cos per whole degree, float. x87 fsin/fcos as in the original.
void __fastcall Engine_BuildTrigTables(void* e) {
    float* sinT = (float*)((uint8_t*)e + 0x1136b1);
    float* cosT = (float*)((uint8_t*)e + 0x113c51);
    for (int32_t i = 0; i < 360; ++i) {
        float s, c;
        __asm {
            fild  i
            fmul  qword ptr ds:[0x5d0c70]
            fmul  qword ptr ds:[0x5cb4a0]
            fld   st(0)
            fsin
            fstp  s
            fcos
            fstp  c
        }
        sinT[i] = s;
        cosT[i] = c;
    }
}
RT3_REPLACE(0x51faa0, Engine_BuildTrigTables);

// 0x51faf0. Copies the front buffer (via a scratch A8R8G8B8 texture of the current
// display mode's size) into dst at (x, y) with a row stride in pixels. The source
// rows are advanced by width*4, not by the locked pitch.
uint8_t __fastcall Engine_CaptureFrontBuffer(uint8_t* e, int, uint8_t* dst, int32_t x, int32_t y, int32_t stride) {
    if (!e[0x114226]) return 0;
    uint8_t* mode = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    int32_t w = F<int32_t>(mode, 0), h = F<int32_t>(mode, 4);
    void* dev = F<void*>(e, 8);
    void* tex;
    void* surf;
    Slot<int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void**)>(dev, 0x50)(
        dev, w, h, 1, 0, 0x15, 3, &tex);
    Slot<int32_t(__stdcall*)(void*, int32_t, void**)>(tex, 0x3c)(tex, 0, &surf);
    void* d = F<void*>(e, 8);
    if (Slot<int32_t(__stdcall*)(void*, void*)>(d, 0x78)(d, surf)) {
        Slot<uint32_t(__stdcall*)(void*)>(surf, 8)(surf);
        Slot<uint32_t(__stdcall*)(void*)>(tex, 8)(tex);
        return 0;
    }
    struct { int32_t pitch; uint8_t* bits; } lr;
    Slot<int32_t(__stdcall*)(void*, void*, void*, uint32_t)>(surf, 0x24)(surf, &lr, nullptr, 0);
    const uint8_t* src = lr.bits;
    uint8_t* out = dst + (y * stride + x) * 4;
    if (h > 0) {
        uint32_t row = w * 4;
        for (int32_t r = h; r; --r) {
            for (uint32_t k = 0; k < row; ++k) out[k] = src[k];
            out += stride * 4;
            src += row;
        }
    }
    Slot<int32_t(__stdcall*)(void*)>(surf, 0x28)(surf);
    Slot<uint32_t(__stdcall*)(void*)>(surf, 8)(surf);
    Slot<uint32_t(__stdcall*)(void*)>(tex, 8)(tex);
    return 1;
}
RT3_REPLACE(0x51faf0, Engine_CaptureFrontBuffer);

void __fastcall Engine_QueueText(void* e, int, uint32_t p1, float x, float y, uint32_t p4, float w, float h,
                                 uint32_t p7, uint32_t p8, uint32_t p9) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(float*)(it + 9) = x + w;
    *(float*)(it + 1) = x;
    *(uint32_t*)(it + 0x19) = p4;
    *(float*)(it + 0xd) = y + h;
    *(float*)(it + 5) = y;
    *(uint32_t*)(it + 0x39) = p8;
    *(uint32_t*)(it + 0x1d) = p7;
    *(uint32_t*)(it + 0x35) = p9;
    it[0] = 3;
    *(uint32_t*)(it + 0x29) = 0x3f800000; // 1.0f
    SetLayer(e, it);
    *(char**)(it + 0x3d) = nullptr;
    ((void(__fastcall*)(char**, const char*))0x51d820)((char**)(it + 0x3d), (const char*)p1); // String_Assign
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51fc20, Engine_QueueText);

RT3_NOINLINE int32_t __fastcall Engine_ModeHasFlags(void* e) { // full 0/1 in EAX
    if (!F_engine<uint8_t>(e, 0x114226)) return 0;
    int32_t w, h, bpp, refresh;
    uint32_t flags;
    Engine_CurrentMode(e, 0, &w, &h, &bpp, &refresh, &flags);
    return flags != 0 ? 1 : 0;
}
RT3_REPLACE(0x51fd00, Engine_ModeHasFlags);

RT3_NOINLINE int32_t __fastcall Engine_Get1146ae(void* e) {
    return F_engine<uint8_t>(e, 0x1146ae) != 0 && F_engine<int32_t>(e, 0x11427a) == 0;
}
RT3_REPLACE(0x51fd40, Engine_Get1146ae);

RT3_NOINLINE int32_t __fastcall Engine_Get11470e(void* e) { return F_engine<int32_t>(e, 0x11470e); }
RT3_REPLACE(0x51fd60, Engine_Get11470e);

// Frame timing: records this frame's ms in a 256-entry ring and, when enabled,
// averages the most recent frames until 1000 ms worth (constant 0x5c85b0) are
// covered. The sum stays on the x87 stack, so it is a double here.
void __fastcall Engine_UpdateFrameTime(void* e) {
    if (F_engine<DWORD>(e, 0x114286) == 0) F_engine<DWORD>(e, 0x114286) = Ticks();
    int32_t* ring = (int32_t*)((uint8_t*)e + 0x11428a);
    ring[g_frameCounter % 256] = (int32_t)(Ticks() - F_engine<DWORD>(e, 0x114286));
    F_engine<DWORD>(e, 0x114286) = Ticks();
    int32_t c = ++g_frameCounter;
    if (F_engine<int32_t>((uint8_t*)g_engine, 0x114206) == 0) return;
    const float limit = *(const float*)0x5c85b0;
    double sum = *(const float*)0x5c8598;
    int32_t frames = 0;
    c += 0x1fe;
    for (int32_t done = 0; done < 0x100; done += 8, c -= 8) {
        // QUIRK: the frame that crosses the limit is counted, except the very first of a group of 8.
        if ((sum += ring[(c + 1) % 256]) > limit) break;
        int k = 0;
        for (; k < 6; ++k)
            if ((sum += ring[(c - k) % 256]) > limit) break;
        if (k < 6) { frames += k + 2; break; }
        sum += ring[(c - 6) % 256];
        frames += 8;
        if (sum > limit) break;
    }
    F_engine<float>(e, 0x114282) = (float)(sum / frames);
}
RT3_REPLACE(0x51fd70, Engine_UpdateFrameTime);

double __fastcall Engine_AverageFrameMs(void* e) { return F_engine<float>(e, 0x114282); }
RT3_REPLACE(0x51ff80, Engine_AverageFrameMs);

// 0x51ff90. Engine constructor: registers itself as the global engine.
uint8_t* __fastcall Engine_Construct(uint8_t* e) {
    F<int32_t>(e, 0x114201) = 0;
    hook::Global<uint8_t*>(0x6d4024) = e;
    Engine_ResetState_Impl(e);
    return e;
}
RT3_REPLACE(0x51ff90, Engine_Construct);

// 0x51ffb0. Engine shutdown: releases the pack's font handles, deletes the
// mouse (+0x20), then the scene (+0x18; with `full`, else only its renderer),
// runs the shutdown hook 0x6d4010, destroys the window (+0) and unregisters
// the "World" window class (instance +4).
RT3_NOINLINE void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full) {
    Pack_ReleaseFontHandles(hook::Global<uint8_t*>(0x6d4020));
    if (uint8_t* m = F<uint8_t*>(e, 0x20)) {
        Mouse_Destroy(m);
        ExeDelete(m);
    }
    F<uint8_t*>(e, 0x20) = nullptr;
    if (full) {
        if (uint8_t* s = F<uint8_t*>(e, 0x18)) {
            Scene_Shutdown(s);
            ExeDelete(s);
        }
        F<uint8_t*>(e, 0x18) = nullptr;
    } else {
        Display_DestroyRenderer(F<uint8_t*>(e, 0x18));
    }
    if (void (*hookFn)() = hook::Global<void (*)()>(0x6d4010)) hookFn();
    e[0x55] = 0;
    ((BOOL(__stdcall*)(HWND))hook::Global<void*>(0x5c82c4))(F<HWND>(e, 0));
    ((BOOL(__stdcall*)(const char*, HINSTANCE))hook::Global<void*>(0x5c82c8))((const char*)0x5dcee8, F<HINSTANCE>(e, 4));
    F<HWND>(e, 0) = nullptr;
}
RT3_REPLACE(0x51ffb0, Engine_Shutdown);

}  // namespace rt3
