// MouseCursor.cpp - RT3.exe 0x53e000-0x540000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
int32_t ClockMs();
void Engine_Nop();
void __fastcall Engine_ResetTextureQueue(void* e);
void __fastcall Engine_ClearRepaint(void* e);
void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_Attach(uint8_t* w, int, uint8_t* parent);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall NameTable_PushPos(uint8_t* t);
void __fastcall NameTable_PopPosition(uint8_t* t);

struct InputEvent {
    uint32_t v[12];
};

void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
void __fastcall Widget_Move(uint8_t* w, int, float x, float y);
uint32_t __fastcall Engine_IsFullscreen(void* e);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out);
void __fastcall Cursor_Show(int32_t show, int, int32_t);
void __fastcall Cursor_Load(int32_t colour, int32_t idx);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
HWND __fastcall Engine_Window(void* e);
int32_t __fastcall Cursor_InWindow();
void __fastcall Nop552130();
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Skip4(uint8_t* t, int, int32_t);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip,
                            float left, float top, float right, float bottom);

double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Cursor_Update(uint8_t* c);
void __fastcall Fade_Save();
void __fastcall Fade_StartVerySlow();
void __fastcall Fade_Restore();
void __fastcall PumpMessages();
void* __fastcall Engine_Input(void* e);
void __fastcall Mouse_Poll(uint8_t* m);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
double __fastcall Slider_StepA(uint8_t* s);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall TokenizeLine(char* base, int32_t* pos, int32_t* count, char** tokens);
int __fastcall ToUpper(char c);

uint8_t* __fastcall Slider_Construct(uint8_t* s, int, float x0, float y0, float x1, float y1, uint32_t a, uint32_t b, uint32_t c, uint32_t d,
                                     uint32_t font);

void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void __fastcall TextureDesc_Load(uint8_t* d);
void __fastcall String_Assign(char** s, const char* src);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

RT3_NOINLINE int __fastcall ToUpper(char c);
void __fastcall BitmapFont_Destroy(uint8_t* f);

// Functions of this file, in exe order.
uint8_t* __fastcall MapView_Load(uint8_t* w, int, int32_t);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall TextureDesc_Load(uint8_t* d);
RT3_NOINLINE void __fastcall TextureDesc_Reload(uint8_t* d);
void __fastcall BitmapFont_Destroy(uint8_t* f);
RT3_NOINLINE int32_t __fastcall Imb_Load(uint8_t* d, int, int32_t id);
uint8_t* __fastcall Imb_Construct(uint8_t* d, int, int32_t id);
void __fastcall Cursor_Load(int32_t colour, int32_t idx);
void __fastcall Timer_Restart(uint8_t* t);
RT3_NOINLINE void __fastcall Section_Enter(uint8_t* s);
RT3_NOINLINE uint32_t __fastcall Obj_Get18(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_Zero18And14(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Cursor_UseGameDrawn(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_UseSystem(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_UseWindows(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_Reapply(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_PushBusy(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
uint8_t* __fastcall Cursor_Construct(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_Shutdown(uint8_t* c);
RT3_NOINLINE void __fastcall Cursor_PopWindowsMode(uint8_t* c);
void __fastcall Cursor_Update(uint8_t* c);
void __fastcall Cursor_Draw(uint8_t* c);
RT3_NOINLINE void __fastcall Widget_SetVtbl(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetValue(uint8_t* w, int, int32_t v);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Window_DeleteChild(uint8_t* win, int, uint8_t* w);
RT3_NOINLINE void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht);
int32_t __fastcall Return1_53f9a0(void*, int, int32_t);
int32_t __fastcall Return1_53f9b0(void*, int, int32_t);
RT3_NOINLINE void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void __fastcall Widget_Redraw(uint8_t* w);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Widget_Move(uint8_t* w, int, float x, float y);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint8_t* __fastcall Control_Load(uint8_t* w, int, int32_t);
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Widget_ReleaseResources(uint8_t* w);
uint8_t* __fastcall Scrollbar_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall ListBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall EditBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Picture_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TreeView_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TimerWidget_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Button_Load(uint8_t* w, int, int32_t);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline void* ExeMalloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }

// Cursor manager (0xccbb20): +0 cursor font, +4 cursor, +8 shown, +0xc busy count,
// +0x10 cursor to restore after busy, +0x14 system cursor mode, +0x18 game-drawn
// mode, +0x1c buffer, +0x24 last change time.

// The engine's mouse device (+0x28) is told to reset (vtbl +0x1c) on a mode change.
static void Engine_ResetMouse() {
    uint8_t* e = g_engine;
    if (e && F<void*>(e, 0x28)) {
        void* m = F<void*>(e, 0x28);
        ((void(__fastcall*)(void*))(*(void***)m)[0x1c / 4])(m);
    }
}

// 0x53f170 / 0x53f210. Switch to the system cursor mode (+0x14 set, engine +0x11473a
// clear) or back to plain Windows cursors (+0x14 clear, +0x11473a set): fullscreen
// captures the mouse, the cursor is reset to 0 and its number kept, and a shown
// cursor is made visible again.
static void Cursor_LeaveGameDrawn(uint8_t* c, int32_t systemMode, int32_t line) {
    if (Engine_IsFullscreen(hook::As<void*>(g_engine)))
        Import<HWND(__stdcall*)(HWND)>(0x5c82ac)(Engine_Window(hook::As<void*>(g_engine)));
    Engine_ResetMouse();
    int32_t idx = F<int32_t>(c, 4);
    Cursor_Set(c, 0, 0, 0, 0);
    F<int32_t>(c, 0x18) = 0;
    F<int32_t>(c, 0x14) = systemMode;
    F<int32_t>(g_engine, 0x11473a) = !systemMode;
    F<int32_t>(g_engine, 0x11473e) = 0;
    F<int32_t>(c, 4) = idx;
    if (F<int32_t>(c, 8)) Cursor_Show(1, 0x5dd828, line);
}

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

static inline double Atof(const char* s) { return ((double(__cdecl*)(const char*))0x5a2076)(s); }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// (a + b - 1 + 1) / n, the inclusive pixel extent the image files use.
static inline float ImbSpan(const char* a, const char* b, int32_t n) {
    double first = Atof(a);
    return (float)((Atof(b) + first - D(0x5c8770) + D(0x5c8770)) / n);
}

// Appends image piece rect (x0,y0)-(x1,y1) (origin a,b; scale c,d) of
// interface image d to its piece list +0x24 (count +0x28), or a null entry
// when out of memory.
static void Imb_AddPiece(uint8_t* d, uint8_t* mem, float x0, float y0, float x1, float y1, uint32_t a, uint32_t b, uint32_t c, uint32_t e) {
    uint8_t* p = mem ? Slider_Construct(mem, 0, x0, y0, x1, y1, a, b, c, e, (uint32_t)(uintptr_t)d) : nullptr;
    F<uint8_t**>(d, 0x24)[F<uint16_t>(d, 0x28)] = p;
    ++F<uint16_t>(d, 0x28);
}

static inline uint8_t* Imb_New() { return ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x25); }

// The two scaled forms: pixel origin (x, y) and size (w, h) in the image,
// scaled by +0x10 / +0x14, the far edge capped at 1.
static void Imb_AddScaled(uint8_t* d, double ox, double oy, int32_t w, int32_t h) {
    float x0 = (float)(ox / F<float>(d, 0x10) / F<int32_t>(d, 8));
    float th = (float)F<int32_t>(d, 0xc);
    float y0 = (float)(oy / F<float>(d, 0x14) / th);
    double x1 = (double)w / F<float>(d, 0x10) / F<int32_t>(d, 8) + x0;
    float y1 = (float)((double)h / F<float>(d, 0x14) / th + y0);
    float fx1 = (float)(x1 > D(0x5c8770) ? D(0x5c8770) : x1);
    double y1d = y1 > D(0x5c8770) ? 1.0 : (double)y1;
    uint8_t* mem = Imb_New();
    Imb_AddPiece(d, mem, x0, y0, fx1, (float)y1d, Bits(x0), Bits(y0), F<uint32_t>(d, 0x10), F<uint32_t>(d, 0x14));
}

// 0x53e2b0. Releases descriptor d's texture (+0x2a) and loads it again.
RT3_NOINLINE void __fastcall TextureDesc_Reload(uint8_t* d) {
    if (void* tex = F<void*>(d, 0x2a)) Pack_Release(g_pack, 0, tex);
    F<void*>(d, 0x2a) = nullptr;
    TextureDesc_Load(d);
}
RT3_REPLACE(0x53e2b0, TextureDesc_Reload);

// 0x53e2e0. Destroys a bitmap font: its glyph sliders (+0x24, count +0x28; the
// array is freed but not cleared), the pack texture +0x2a, the buffer +0x20, and
// its entry +0x36 in the window manager's font list (0x6d401c +0xc69).
RT3_NOINLINE void __fastcall BitmapFont_Destroy(uint8_t* f) {
    for (int32_t i = 0; i < F<uint16_t>(f, 0x28); ++i) {
        uint8_t* g = F<uint8_t**>(f, 0x24)[i];
        if (!g) continue;
        Nop552130();
        ExeDelete(g);
    }
    if (F<void*>(f, 0x24)) Free(F<void*>(f, 0x24));
    if (F<void*>(f, 0x2a)) Pack_Release(g_pack, 0, F<void*>(f, 0x2a));
    F<void*>(f, 0x2a) = nullptr;
    if (F<void*>(f, 0x20)) Free(F<void*>(f, 0x20));
    uint8_t* mgr = g_windowManager;
    if (mgr && F<Collection*>(mgr, 0xc69)) {
        Collection* fonts = F<Collection*>(mgr, 0xc69);
        fonts->vtbl->release(fonts, F<uint32_t>(f, 0x36));
    }
}
RT3_REPLACE(0x53e2e0, BitmapFont_Destroy);

// 0x53e370. Loads interface image description `id` (.IMB text in pack
// 0x6d4020) into d: TGA name / size / target screen size, scale modifiers,
// Scaleable, the VRAM share (at least 0x5c88a0) and its image pieces
// (Image, ImageWH, ImageWHWithRef, ImageWHForceScale, ImageScaled,
// ImageWHScaled); unknown keys are logged with the line. Then the screen
// scale (+0x2e / +0x32), the texture handle +0x36 and the texture.
RT3_NOINLINE int32_t __fastcall Imb_Load(uint8_t* d, int, int32_t id) {
    F<int32_t>(d, 0x2a) = 0;
    F<int32_t>(d, 8) = 0;
    F<int32_t>(d, 0xc) = 0;
    F<uint32_t>(d, 0x10) = 0x3f800000;
    F<uint32_t>(d, 0x14) = 0x3f800000;
    F<int32_t>(d, 0x18) = 0;
    F<uint32_t>(d, 0x1c) = 0x3e4ccccd;
    F<int32_t>(d, 0) = 1;
    F<int32_t>(d, 4) = 1;
    F<uint16_t>(d, 0x28) = 0;
    F<void*>(d, 0x24) = ((void*(__cdecl*)(size_t))0x5a125d)(4000);
    F<void*>(d, 0x20) = nullptr;
    NameTable_Seek(g_pack, 0, id, 0);
    int32_t size = NameTable_EntrySize(g_pack, 0, id);
    char* buf = ((char*(__cdecl*)(size_t))0x5a125d)((size_t)size + 1);
    NameTable_Read(g_pack, 0, buf, size);
    buf[size] = 0;
    int32_t pos = 0, count, line = 0;
    char* t[64] = {};
    while (TokenizeLine(buf, &pos, &count, t)) {
        ++line;
        if (!count) continue;
        bool known = true;
        switch ((char)ToUpper(t[0][0])) {
        case 'V':
            if (Stricmp(t[0], (const char*)0x5dd7c4)) known = false;
            else F<float>(d, 0x14) = (float)Atof(t[1]);
            break;
        case 'H':
            if (Stricmp(t[0], (const char*)0x5dd7ac)) known = false;
            else F<float>(d, 0x10) = (float)Atof(t[1]);
            break;
        case 'T':
            if (!Stricmp(t[0], (const char*)0x5dd7a4)) String_Assign((char**)(d + 0x20), t[1]);
            else if (!Stricmp(t[0], (const char*)0x5dd798)) F<int32_t>(d, 8) = Ftol(Atof(t[1]));
            else if (!Stricmp(t[0], (const char*)0x5dd78c)) F<int32_t>(d, 0xc) = Ftol(Atof(t[1]));
            else if (!Stricmp(t[0], (const char*)0x5dd774)) F<int32_t>(d, 0) = Ftol(Atof(t[1]));
            else if (!Stricmp(t[0], (const char*)0x5dd75c)) F<int32_t>(d, 4) = Ftol(Atof(t[1]));
            else known = false;
            break;
        case 'S':
            if (Stricmp(t[0], (const char*)0x5dd750)) known = false;
            else F<int32_t>(d, 0x18) = Ftol(Atof(t[1]));
            break;
        case 'M':
            if (Stricmp(t[0], (const char*)0x5dd730) && Stricmp(t[0], (const char*)0x5dd714)) known = false;
            else {
                double v = Atof(t[1]);
                F<float>(d, 0x1c) = (float)(v > D(0x5c88a0) ? v : D(0x5c88a0));
            }
            break;
        case 'I':
            if (!Stricmp(t[0], (const char*)0x5dd704)) {
                int32_t w = Ftol(Atof(t[3]));
                int32_t h = Ftol(Atof(t[4]));
                double ox = Atof(t[1]);
                Imb_AddScaled(d, ox, Atof(t[2]), w, h);
            } else if (!Stricmp(t[0], (const char*)0x5dd6f8)) {
                double right = Atof(t[3]);
                int32_t w = Ftol(right - Atof(t[1]) + D(0x5c8770));
                double bottom = Atof(t[4]);
                int32_t h = Ftol(bottom - Atof(t[2]) + D(0x5c8770));
                double ox = Atof(t[1]);
                Imb_AddScaled(d, ox, Atof(t[2]), w, h);
            } else if (!Stricmp(t[0], (const char*)0x5dd6e8)) {
                uint8_t* mem = Imb_New();
                if (!mem) {
                    Imb_AddPiece(d, nullptr, 0, 0, 0, 0, 0, 0, 0, 0);
                    break;
                }
                float rb = (float)(Atof(t[6]) / F<int32_t>(d, 0xc));
                float ra = (float)(Atof(t[5]) / F<int32_t>(d, 8));
                float y1 = ImbSpan(t[2], t[4], F<int32_t>(d, 0xc));
                float x1 = ImbSpan(t[3], t[1], F<int32_t>(d, 8));
                float y0 = (float)(Atof(t[2]) / F<int32_t>(d, 0xc));
                float x0 = (float)(Atof(t[1]) / F<int32_t>(d, 8));
                Imb_AddPiece(d, mem, x0, y0, x1, y1, Bits(ra), Bits(rb), F<uint32_t>(d, 0x10), F<uint32_t>(d, 0x14));
            } else if (!Stricmp(t[0], (const char*)0x5dd6d4)) {
                float fx = (float)(Atof(t[1]) / F<int32_t>(d, 8));
                float fy = (float)(Atof(t[2]) / F<int32_t>(d, 0xc));
                uint8_t* mem = Imb_New();
                if (!mem) {
                    Imb_AddPiece(d, nullptr, 0, 0, 0, 0, 0, 0, 0, 0);
                    break;
                }
                float sd = (float)(Atof(t[6]) * F<float>(d, 0x14));
                float sc = (float)(Atof(t[5]) * F<float>(d, 0x10));
                float y1 = ImbSpan(t[2], t[4], F<int32_t>(d, 0xc));
                float x1 = ImbSpan(t[3], t[1], F<int32_t>(d, 8));
                Imb_AddPiece(d, mem, fx, fy, x1, y1, Bits(fx), Bits(fy), Bits(sc), Bits(sd));
            } else if (!Stricmp(t[0], (const char*)0x5dd6cc)) {
                float fx = (float)(Atof(t[1]) / F<int32_t>(d, 8));
                float fy = (float)(Atof(t[2]) / F<int32_t>(d, 0xc));
                uint8_t* mem = Imb_New();
                if (!mem) {
                    Imb_AddPiece(d, nullptr, 0, 0, 0, 0, 0, 0, 0, 0);
                    break;
                }
                float y1 = ImbSpan(t[2], t[4], F<int32_t>(d, 0xc));
                float x1 = ImbSpan(t[3], t[1], F<int32_t>(d, 8));
                Imb_AddPiece(d, mem, fx, fy, x1, y1, Bits(fx), Bits(fy), F<uint32_t>(d, 0x10), F<uint32_t>(d, 0x14));
            } else if (!Stricmp(t[0], (const char*)0x5dd6c4)) {
                float a = (float)Atof(t[1]);
                float b = (float)Atof(t[2]);
                float c = (float)Atof(t[3]);
                double dd = Atof(t[4]);
                double a2 = a, c2 = c, b2 = b;
                if (c > a) c2 = c + (double)Fl(0x5c85b4);
                else a2 = a + (double)Fl(0x5c85b4);
                if (dd > b) dd += Fl(0x5c85b4);
                else b2 = b + (double)Fl(0x5c85b4);
                double kx = Fl(0x5c85b4) / F<int32_t>(d, 8);
                float x0 = (float)(kx * a2);
                double ky = Fl(0x5c85b4) / F<int32_t>(d, 0xc);
                float kyf = (float)ky;
                float y0 = (float)(ky * b2);
                float x1 = (float)(kx * c2);
                float y1 = (float)((double)kyf * dd);
                uint8_t* mem = Imb_New();
                if (!mem) {
                    Imb_AddPiece(d, nullptr, 0, 0, 0, 0, 0, 0, 0, 0);
                    break;
                }
                Imb_AddPiece(d, mem, x0, y0, x1, y1, Bits(x0), Bits(y0), F<uint32_t>(d, 0x10), F<uint32_t>(d, 0x14));
            } else {
                known = false;
            }
            break;
        default: known = false; break;
        }
        if (!known) {
            char num[0x40], msg[0x200];
            const char* file = (const char*)(g_pack + 0x427c0);
            Num_Format(num, 1, (double)line, 10, 1, 0, 0);
            FormatText(msg, GetString(0x66), t[0], file, num, nullptr, nullptr, nullptr, nullptr, nullptr);
            DebugLog(msg, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
        }
    }
    ((void(__cdecl*)(void*))0x5a1145)(buf);
    F<float>(d, 0x2e) = (float)((double)F<int32_t>(d, 8) / F<int32_t>(d, 0));
    F<float>(d, 0x32) = (float)((double)F<int32_t>(d, 0xc) / F<int32_t>(d, 4));
    uint8_t* self = d;
    uint8_t* tm = F<uint8_t*>(g_windowManager, 0xc69);
    F<uint32_t>(d, 0x36) = (*(uint32_t(__fastcall**)(uint8_t*, int, void*, int32_t, int32_t, int32_t))(*(uint8_t**)tm + 4))(tm, 0, &self, 4, 0, 0);
    TextureDesc_Load(d);
    return 1;
}
RT3_REPLACE(0x53e370, Imb_Load);

  // -> 0x530820
RT3_REPLACE(0x53ee60, BitmapFont_Destroy);

// 0x53ee70. Constructs IMB d by loading entry `id`.
uint8_t* __fastcall Imb_Construct(uint8_t* d, int, int32_t id) {
    Imb_Load(d, 0, id);
    return d;
}
RT3_REPLACE(0x53ee70, Imb_Construct);

// 0x53ee90. Loads cursor idx (colour or b/w set) from data\cursors, falling back to the arrow.
RT3_NOINLINE void __fastcall Cursor_Load(int32_t colour, int32_t idx) {
    char path[0x12c];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, colour ? (const char*)0x5dd7f0 : (const char*)0x5dd80c,
                                                      F<uint32_t>((void*)0x625258, idx * 0x14));
    void* h = Import<void*(__stdcall*)(const char*)>(0x5c829c)(path);
    void** slot = (void**)0xccba60 + (colour * 0x17 + idx);
    *slot = h;
    if (!h) *slot = Import<void*(__stdcall*)(void*, uint32_t)>(0x5c82a0)(nullptr, 0x7f00);
}
RT3_REPLACE(0x53ee90, Cursor_Load);

// 0x53ef00. Restarts the timer (+0x24 = now, +0x28 = 0).
void __fastcall Timer_Restart(uint8_t* t) {
    F<int32_t>(t, 0x24) = ClockMs();
    F<int32_t>(t, 0x28) = 0;
}
RT3_REPLACE(0x53ef00, Timer_Restart);

// 0x53ef20. Enters a nested section; the outermost saves +0x18/+0x14 and the
// engine values +0x11473e/+0x11473a in globals.
RT3_NOINLINE void __fastcall Section_Enter(uint8_t* s) {
    if (++g_ccbb24 > 1) return;
    g_ccbb1c = F<uint32_t>(s, 0x18);
    g_ccba58 = F<uint32_t>(s, 0x14);
    uint8_t* e = g_engine;
    g_ccba5c = F<uint32_t>(e, 0x11473e);
    g_ccbb18 = F<uint32_t>(e, 0x11473a);
}
RT3_REPLACE(0x53ef20, Section_Enter);

RT3_NOINLINE uint32_t __fastcall Obj_Get18(uint8_t* o) { return F<uint32_t>(o, 0x18); }
RT3_REPLACE(0x53ef60, Obj_Get18);

RT3_NOINLINE int32_t __fastcall Obj_Zero18And14(uint8_t* o) { return !F<int32_t>(o, 0x18) && !F<int32_t>(o, 0x14) ? 1 : 0; }
RT3_REPLACE(0x53ef70, Obj_Zero18And14);

// 0x53ef90. Sets the text (+0x1c, heap copy) and value +0x20.
RT3_NOINLINE void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v) {
    if (F<void*>(o, 0x1c)) Free(F<void*>(o, 0x1c));
    F<void*>(o, 0x1c) = nullptr;
    if (s) {
        size_t n = 0;
        while (s[n]) ++n;
        char* p = (char*)Malloc(n + 1);
        F<char*>(o, 0x1c) = p;
        for (size_t k = 0; (p[k] = s[k]) != 0; ++k) {}
    }
    F<uint32_t>(o, 0x20) = v;
}
RT3_REPLACE(0x53ef90, Obj_SetText1c);

// 0x53f000. Switches the mouse cursor to `idx` (negative = 0, the hidden/default
// one). Unless forced, nothing happens for the current cursor, and while +0xc
// (busy) holds it is only queued in +0x10 unless `now`. The Windows cursor is shown
// / hidden on the transitions, the colour (+0x14 == 0) or b/w cursor set unless
// +0x18 (game-drawn), and the cursor's bitmap font (0x625248, 20 bytes each) is
// taken from the pack in place of the old one (+0).
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now) {
    if (idx < 0) idx = 0;
    if (!force) {
        if (idx == F<int32_t>(c, 4)) return;
        if (F<int32_t>(c, 0xc) && !now) {
            F<int32_t>(c, 0x10) = idx;
            return;
        }
        F<int32_t>(c, 0x24) = ClockMs();
    }
    if (!idx) {
        if (!F<int32_t>(c, 0x18)) Cursor_Show(0, 0x5dd828, 0xa6);
    } else if (!F<int32_t>(c, 4) && !F<int32_t>(c, 0x18)) {
        Cursor_Show(1, 0x5dd828, 0xac);
    }
    if (!F<int32_t>(c, 0x18)) {
        int32_t colour = F<int32_t>(c, 0x14) == 0;
        void** slot = (void**)0xccba60 + colour * 0x17 + idx;
        if (!*slot) Cursor_Load(colour, idx);
        Import<void*(__stdcall*)(void*)>(0x5c8298)(*slot);
    }
    uint8_t* obj = idx ? Pack_AcquireNamed(g_pack, 0, *(const char**)(0x625248 + idx * 20)) : nullptr;
    if (F<void*>(c, 0)) Pack_Release(g_pack, 0, F<void*>(c, 0));
    F<uint8_t*>(c, 0) = obj;
    F<int32_t>(c, 4) = idx;
}
RT3_REPLACE(0x53f000, Cursor_Set);

// 0x53f0f0. Switches to the game-drawn cursor: mouse capture released, engine flag
// +0x11473e set, the Windows cursor hidden and the current cursor re-applied.
RT3_NOINLINE void __fastcall Cursor_UseGameDrawn(uint8_t* c) {
    if (F<int32_t>(c, 0x18)) return;
    Engine_ResetMouse();
    Import<BOOL(__stdcall*)()>(0x5c82a8)();
    F<int32_t>(c, 0x18) = 1;
    F<int32_t>(c, 0x14) = 0;
    F<int32_t>(g_engine, 0x11473e) = 1;
    F<int32_t>(c, 8) = 1;
    if (!F<int32_t>(c, 4)) return;
    Cursor_Show(0, 0x5dd828, 0x105);
    int32_t idx = F<int32_t>(c, 4);
    F<int32_t>(c, 4) = -1;
    Cursor_Set(c, 0, idx, 0, 0);
}
RT3_REPLACE(0x53f0f0, Cursor_UseGameDrawn);

RT3_NOINLINE void __fastcall Cursor_UseSystem(uint8_t* c) {
    if (F<int32_t>(c, 0x14)) return;
    Cursor_LeaveGameDrawn(c, 1, 0x126);
}
RT3_REPLACE(0x53f170, Cursor_UseSystem);

RT3_NOINLINE void __fastcall Cursor_UseWindows(uint8_t* c) {
    if (!F<int32_t>(c, 0x14) && !F<int32_t>(c, 0x18)) return;
    Cursor_LeaveGameDrawn(c, 0, 0x140);
}
RT3_REPLACE(0x53f210, Cursor_UseWindows);

// 0x53f2c0. In system mode, re-applies the engine's mode (game-drawn or Windows).
RT3_NOINLINE void __fastcall Cursor_Reapply(uint8_t* c) {
    if (!F<int32_t>(c, 0x14)) return;
    if (F<int32_t>(g_engine, 0x11473e)) Cursor_UseGameDrawn(c);
    else Cursor_UseWindows(c);
}
RT3_REPLACE(0x53f2c0, Cursor_Reapply);

// 0x53f2f0 / 0x53f310. Busy nesting: the first level shows the wait cursor (6) and
// keeps the current one; the last restores it (1 if there was none).
RT3_NOINLINE void __fastcall Cursor_PushBusy(uint8_t* c) {
    if (!F<int32_t>(c, 0xc)) {
        F<int32_t>(c, 0x10) = F<int32_t>(c, 4);
        Cursor_Set(c, 0, 6, 0, 0);
    }
    ++F<int32_t>(c, 0xc);
}
RT3_REPLACE(0x53f2f0, Cursor_PushBusy);

RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c) {
    if (--F<int32_t>(c, 0xc)) return;
    int32_t idx = F<int32_t>(c, 0x10);
    Cursor_Set(c, 0, idx ? idx : 1, 0, 0);
}
RT3_REPLACE(0x53f310, Cursor_PopBusy);

// 0x53f330. Cursor manager constructor: shown, no cursor (reset to 0), cursor
// cache 0xccba60 cleared, mode from the engine (game-drawn +0x11473e, else system
// unless +0x11473a).
uint8_t* __fastcall Cursor_Construct(uint8_t* c) {
    F<int32_t>(c, 0xc) = 0;
    F<int32_t>(c, 0x10) = 1;
    F<int32_t>(c, 8) = 1;
    F<int32_t>(c, 0x24) = 0;
    F<int32_t>(c, 0x28) = 0;
    F<int32_t>(c, 0) = 0;
    F<int32_t>(c, 0x18) = 0;
    F<int32_t>(c, 0x14) = 0;
    F<int32_t>(c, 4) = -1;
    Cursor_Set(c, 0, 0, 0, 0);
    F<int32_t>(c, 0x1c) = 0;
    F<int32_t>(c, 0x20) = -1;
    if (F<int32_t>(g_engine, 0x11473e)) {
        Cursor_UseGameDrawn(c);
    } else {
        F<int32_t>(c, 0x18) = 0;
        F<int32_t>(c, 0x14) = F<int32_t>(g_engine, 0x11473a) == 0;
    }
    Engine_ResetMouse();
    memset((void*)0xccba60, 0, 0x2e * 4);
    return c;
}
RT3_REPLACE(0x53f330, Cursor_Construct);

// 0x53f3d0. Shuts the cursor manager down: no cursor, buffer +0x1c freed.
RT3_NOINLINE void __fastcall Cursor_Shutdown(uint8_t* c) {
    Cursor_Set(c, 0, 0, 0, 0);
    if (F<void*>(c, 0x1c)) Free(F<void*>(c, 0x1c));
    F<void*>(c, 0x1c) = nullptr;
}
RT3_REPLACE(0x53f3d0, Cursor_Shutdown);

// 0x53f400. Leaves a nested Windows-cursor section (count 0xccbb24): at the outer
// level the saved mode comes back (game-drawn 0xccbb1c / system 0xccba58) with the
// saved engine flags (0xccba5c / 0xccbb18).
RT3_NOINLINE void __fastcall Cursor_PopWindowsMode(uint8_t* c) {
    if (--g_ccbb24 > 0) return;
    if (hook::As<int32_t>(g_ccbb1c)) Cursor_UseGameDrawn(c);
    else if (hook::As<int32_t>(g_ccba58)) Cursor_UseSystem(c);
    F<int32_t>(g_engine, 0x11473e) = hook::As<int32_t>(g_ccba5c);
    F<int32_t>(g_engine, 0x11473a) = hook::As<int32_t>(g_ccbb18);
}
RT3_REPLACE(0x53f400, Cursor_PopWindowsMode);

// 0x53f450. Follows the mouse entering / leaving the window (input 0x6d4018): the
// game-drawn mode hides / shows the Windows cursor; otherwise the cursor manager
// (sic: the global one, 0xccbb20) re-applies its cursor if the Windows cursor is
// shown, or just notes the time.
RT3_NOINLINE void __fastcall Cursor_Update(uint8_t* c) {
    int32_t in = Cursor_InWindow();
    if (in == F<int32_t>(c, 8)) return;
    if (F<int32_t>(c, 0x18)) {
        F<int32_t>(c, 8) = in;
        if (in) Cursor_Show(0, 0x5dd828, 0x169);
        else Cursor_Show(1, 0x5dd828, 0x16b);
        return;
    }
    uint8_t* m = g_cursorManager;
    if (!F<int32_t>(m, 0x18)) {
        if (g_624b20 <= 0) {
            F<int32_t>(m, 0x24) = ClockMs();
            F<int32_t>(c, 8) = in;
            return;
        }
        Cursor_Set(m, 0, F<int32_t>(m, 4), 1, 0);
    }
    F<int32_t>(c, 8) = in;
}
RT3_REPLACE(0x53f450, Cursor_Update);

// 0x53f4e0. Draws mouse cursor c: unless it is a custom (+0x18) cursor,
// refreshes it, and every 150 ms of the first 15 s since +0x24 re-sets it
// (0x624b20 > 0) or restarts that window; then (custom cursors with an
// image table *c) the animated frame (table 0x62524c by cursor +4: first,
// count, fps) at the mouse, and the tooltip text +0x1c (colour +0x20) by
// the cursor glyph or, without one, past the system cursor.
void __fastcall Cursor_Draw(uint8_t* c) {
    if (!F<int32_t>(c, 0x18)) Cursor_Update(c);
    if (ClockMs() - F<int32_t>(c, 0x24) < 15000 && ClockMs() - F<int32_t>(c, 0x28) > 0x96) {
        F<int32_t>(c, 0x28) = ClockMs();
        if (!F<int32_t>(c, 0x18)) {
            if (g_624b20 > 0) Cursor_Set(c, 0, F<int32_t>(c, 4), 1, 0);
            else F<int32_t>(c, 0x24) = ClockMs();
        }
    }
    uint8_t* glyph = nullptr;
    Fade_Save();
    Fade_StartVerySlow();
    float x, y;
    if (F<int32_t>(c, 0x18) && F<int32_t>(c, 4) && F<int32_t>(c, 4) != -1) {
        if (!F<uint8_t*>(c, 0)) {
            Fade_Restore();
            return;
        }
        PumpMessages();
        Mouse_Poll((uint8_t*)Engine_Input(hook::As<void*>(g_engine)));
        Cursor_Update(c);
        Mouse_GetPos((uint8_t*)Engine_Input(hook::As<void*>(g_engine)), 0, (uint32_t*)&x, (uint32_t*)&y);
        int32_t k = F<int32_t>(c, 4);
        int32_t count = hook::Global<int32_t>(0x625250 + k * 0x14);
        int32_t frame = hook::Global<int32_t>(0x62524c + k * 0x14);
        if (count > 1) frame += ClockMs() / (1000 / hook::Global<int32_t>(0x625254 + F<int32_t>(c, 4) * 0x14)) % count;
        glyph = (uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(c, 0), 0, frame);
        Glyph_Queue(glyph, 0, x, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    const char* text = F<const char*>(c, 0x1c);
    uint8_t* e = g_engine;
    if (text && strlen(text) && e && F<uint8_t*>(e, 0x1141fd)) {
        Mouse_GetPos((uint8_t*)Engine_Input(e), 0, (uint32_t*)&x, (uint32_t*)&y);
        double yv;
        if (glyph) {
            x = (float)(Slider_StepA(glyph) + x);
            yv = Slider_StepB(glyph) + y;
            int32_t k = F<int32_t>(c, 4);
            if (k == 4 || k == 5) {
                x = (float)(x - D(0x5dd858));
                yv -= D(0x5d0700);
            } else {
                x = (float)(x - D(0x5ca2a0));
            }
        } else {
            uint8_t* en = g_engine;
            uint32_t full = Engine_IsFullscreen(en);
            en = g_engine;
            uint8_t* mode = F<uint8_t*>(en, 0x34) + F<int32_t>(en, 0x11421e) * 0x15;
            x = (float)(Fl(0x5c8d60) / F<int32_t>(mode, 0) + x);
            if (full) yv = Fl(0x5c9aa8) / F<int32_t>(mode, 4) + y;
            else yv = Fl(0x5c9aa8) / F<int32_t>(mode, 4) + y + D(0x5cab60);
        }
        x = (float)(x - D(0x5ca2a0));
        y = (float)(yv + D(0x5c9bc0));
        Font_DrawTextBox(F<uint8_t*>(g_engine, 0x1141fd), 0, F<const char*>(c, 0x1c), (float)(x - D(0x5dd850)), y, 0.2375f,
                         0.083333336f, F<uint32_t>(c, 0x20), 0, 1, 0, 1.0f);
    }
    Fade_Restore();
}
RT3_REPLACE(0x53f4e0, Cursor_Draw);

RT3_NOINLINE void __fastcall Widget_SetVtbl(uint8_t* w) { F<uint32_t>(w, 0) = 0x5dd860; }
RT3_REPLACE(0x53f7d0, Widget_SetVtbl);

RT3_NOINLINE uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5dd860;
    return w;
}
RT3_REPLACE(0x53f7e0, Widget_SetVtblRet);

// 0x53f7f0. Sets widget w's value (+0x5c, mirrored to +0x60). Clearing it while
// the focused widget (UI manager 0x6d401c +0xc6d) belongs to w (+0x86) takes the
// focus away from that widget.
void __fastcall Widget_SetValue(uint8_t* w, int, int32_t v) {
    if (F<int32_t>(w, 0x5c) == v) return;
    F<int32_t>(w, 0x5c) = v;
    F<int32_t>(w, 0x60) = v;
    if (v) return;
    uint8_t* focus = F<uint8_t*>(g_windowManager, 0xc6d);
    if (focus && F<uint8_t*>(focus, 0x86) == w) Widget_ClearFlags(focus, 0, 4);
}
RT3_REPLACE(0x53f7f0, Widget_SetValue);

// 0x53f830. Child widget (+0x70, next +0x8e) with id +4.
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id) {
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e))
        if ((int32_t)F<uint16_t>(c, 4) == id) return c;
    return nullptr;
}
RT3_REPLACE(0x53f830, Widget_FindChild);

// 0x53f860. Deletes widget `w` from the window's child list (first +0x70 / last
// +0x74, links next +0x8e / previous +0x8a) after releasing its resources.
void __fastcall Window_DeleteChild(uint8_t* win, int, uint8_t* w) {
    if (!w) return;
    Widget_ReleaseResources(w);
    if (w == F<uint8_t*>(win, 0x70)) F<uint8_t*>(win, 0x70) = F<uint8_t*>(w, 0x8e);
    if (w == F<uint8_t*>(win, 0x74)) F<uint8_t*>(win, 0x74) = F<uint8_t*>(w, 0x8a);
    if (uint8_t* n = F<uint8_t*>(w, 0x8a)) F<uint8_t*>(n, 0x8e) = F<uint8_t*>(w, 0x8e);
    if (uint8_t* p = F<uint8_t*>(w, 0x8e)) F<uint8_t*>(p, 0x8a) = F<uint8_t*>(w, 0x8a);
    ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)w)[0])(w, 0, 1);
}
RT3_REPLACE(0x53f860, Window_DeleteChild);

// 0x53f8d0. Sets the rectangle (+5/+9/+0xd/+0x11; 0x5c9b08 = keep); a size change
// is passed to every child (vtbl +8 with +0x64, +0x68, w, h).
RT3_NOINLINE void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht) {
    const float keep = *(const float*)0x5c9b08;
    if (x == keep) x = F<float>(w, 5);
    if (y == keep) y = F<float>(w, 9);
    if (wd == keep) wd = F<float>(w, 0xd);
    if (ht == keep) ht = F<float>(w, 0x11);
    bool changed = !(F<float>(w, 0xd) == wd) || !(ht == F<float>(w, 0x11));
    F<float>(w, 0x11) = ht;
    F<float>(w, 5) = x;
    F<float>(w, 9) = y;
    F<float>(w, 0xd) = wd;
    if (!changed) return;
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e))
        ((void(__thiscall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)c)[8 / 4])(
            c, F<uint32_t>(w, 0x64), F<uint32_t>(w, 0x68), F<uint32_t>(w, 0xd), F<uint32_t>(w, 0x11));
}
RT3_REPLACE(0x53f8d0, Widget_SetRect);

// 0x53f9a0 (vtable stub).
int32_t __fastcall Return1_53f9a0(void*, int, int32_t) { return 0x1; }
RT3_REPLACE(0x53f9a0, Return1_53f9a0);

// 0x53f9b0 (vtable stub).
int32_t __fastcall Return1_53f9b0(void*, int, int32_t) { return 0x1; }
RT3_REPLACE(0x53f9b0, Return1_53f9b0);

// 0x53f9c0. Adds a child widget: ordered by layer (+8), or right after the child
// with id `afterId`; doubly linked through +0x8a / +0x8e (head +0x70, tail +0x74).
RT3_NOINLINE void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId) {
    Widget_Attach(w, 0, win);
    F<uint8_t*>(w, 0x8a) = nullptr;
    uint8_t* c = F<uint8_t*>(win, 0x70);
    if (c) {
        int8_t layer = (int8_t)w[8];
        do {
            if ((int8_t)c[8] > layer) break;
            int32_t id = F<uint16_t>(c, 4);
            F<uint8_t*>(w, 0x8a) = c;
            c = F<uint8_t*>(c, 0x8e);
            if (afterId && afterId == id) break;
        } while (c);
    }
    if (c == F<uint8_t*>(win, 0x70)) F<uint8_t*>(win, 0x70) = w;
    if (!c) F<uint8_t*>(win, 0x74) = w;
    F<uint8_t*>(w, 0x8e) = c;
    if (c) F<uint8_t*>(c, 0x8a) = w;
    if (uint8_t* prev = F<uint8_t*>(w, 0x8a)) F<uint8_t*>(prev, 0x8e) = w;
}
RT3_REPLACE(0x53f9c0, Window_AddChild);

// 0x53fa50. Loads dialog `name` from the pack into window `win` at (x, y): the
// 0x31-byte header (+4..; tags 0x7d0 / 0x7d1), the name (+0x35), then widget
// records until tag 0x7d4: kind, the two names (pack blocks 0xbb8 / 0xbb9; the
// second also loaded as a bitmap font), the kind's own record (1 control, 2 edit
// box, 4 scrollbar, 8 tree view, 0x10 timer, 0x20 button, 0x40 list box, 0x80
// picture, 0x1000 map view), tag 0x7d3. (sic) An unknown kind reuses the previous
// widget (the name pointer for the first). A demo game (0x6d3b4c) clears +0x15.
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name) {
    F<uint32_t>(win, 0x74) = 0;
    F<uint32_t>(win, 0x70) = 0;
    StrCopyN((char*)win + 0x35, (const char*)0x5d1038, 0x1e);
    F<int32_t>(win, 0x21) = 0x14;
    for (uint32_t o : { 0x58u, 0x54u, 0x6cu, 0x5cu, 0x60u, 5u, 9u, 0xdu, 0x11u, 0x15u, 0x1du, 0x64u, 0x68u }) F<uint32_t>(win, o) = 0;
    uint8_t* pack = g_pack;
    NameTable_Seek(pack, 0, NameTable_Hash(pack, 0, name), 0);
    NameTable_Skip4(g_pack, 0, 0x7d0);
    uint8_t* hdr = (uint8_t*)ExeMalloc(0x31);
    NameTable_Read(g_pack, 0, hdr, 0x31);
    NameTable_Skip4(g_pack, 0, 0x7d1);
    memcpy(win + 4, hdr, 0x31);
    Free(hdr);
    StrCopyN((char*)win + 0x35, name, 0x1e);
    F<float>(win, 5) = x;
    F<float>(win, 9) = y;
    F<uint32_t>(win, 0x64) = F<uint32_t>(win, 0xd);
    F<uint32_t>(win, 0x68) = F<uint32_t>(win, 0x11);
    uint8_t* w = (uint8_t*)name;
    for (;;) {
        int32_t tag;
        NameTable_Read(g_pack, 0, &tag, 4);
        if (tag == 0x7d4) break;
        int32_t kind;
        char* name1 = nullptr;
        char* name2 = nullptr;
        NameTable_Read(g_pack, 0, &kind, 4);
        NameTable_ReadBlock(g_pack, 0, 0xbb8, (void**)&name1);
        NameTable_ReadBlock(g_pack, 0, 0xbb9, (void**)&name2);
        typedef uint8_t*(__fastcall * LoadFn)(uint8_t*, int, int32_t);
        LoadFn load = nullptr;
        size_t size = 0;
        switch (kind) {
        case 1: load = Control_Load; size = 0xe9; break;
        case 2: load = EditBox_Load; size = 0xf0; break;
        case 4: load = Scrollbar_Load; size = 0x111; break;
        case 8: load = TreeView_Load; size = 0x1ea; break;
        case 0x10: load = TimerWidget_Load; size = 0xda; break;
        case 0x20: load = Button_Load; size = 0xe8; break;
        case 0x40: load = ListBox_Load; size = 0x1b9; break;
        case 0x80: load = Picture_Load; size = 0x10a; break;
        case 0x1000: load = MapView_Load; size = 0x123; break;
        }
        if (load) {
            uint8_t* m = (uint8_t*)ExeNew(size);
            w = m ? load(m, 0, (int32_t)(uintptr_t)win) : nullptr;
        }
        F<char*>(w, 0x43) = name1;
        F<char*>(w, 0x47) = name2;
        if (name2) {
            NameTable_PushPos(g_pack);
            F<uint8_t*>(w, 0x92) = Pack_AcquireNamed(g_pack, 0, F<char*>(w, 0x47));
            NameTable_PopPosition(g_pack);
        }
        Window_AddChild(win, 0, w, 0);
        NameTable_Skip4(g_pack, 0, 0x7d3);
    }
    if (g_6d3b4c) F<int32_t>(win, 0x15) = 0;
}
RT3_REPLACE(0x53fa50, Window_Load);

// 0x53fda0. Redraws the visible widget's children (vtbl +0x18), resetting the
// texture queue for flagged ones (+0x42).
void __fastcall Widget_Redraw(uint8_t* w) {
    if (!F<int32_t>(w, 0x5c)) return;
    Engine_Nop();
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e)) {
        if (c[0x42] && F<int32_t>(w, 0x5c)) Engine_ResetTextureQueue(hook::As<void*>(g_engine));
        ((void(__thiscall*)(void*))(*(void***)c)[0x18 / 4])(c);
    }
    if (F<int32_t>(w, 0x1d)) Engine_ClearRepaint(hook::As<void*>(g_engine));
}
RT3_REPLACE(0x53fda0, Widget_Redraw);

// 0x53fe00. Deletes the children with ids lo..hi (hi 0xfff0 = just lo) whose
// parent (+0x4b) is `parent`, walking back from the last child; each is deleted
// once the walk has moved past it.
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent) {
    if (hi == 0xfff0) hi = lo;
    uint8_t* pending = nullptr;
    for (uint8_t* w = F<uint8_t*>(win, 0x74); w; w = F<uint8_t*>(w, 0x8a)) {
        int32_t id = F<uint16_t>(w, 4);
        if (id < lo || id > hi || F<uint32_t>(w, 0x4b) != parent) continue;
        if (pending) Window_DeleteChild(win, 0, pending);
        pending = w;
    }
    if (pending) Window_DeleteChild(win, 0, pending);
}
RT3_REPLACE(0x53fe00, Window_DeleteChildren);

RT3_NOINLINE void __fastcall Widget_Move(uint8_t* w, int, float x, float y) {
    Widget_SetRect(w, 0, x, y, F<float>(w, 0xd), F<float>(w, 0x11));
}
RT3_REPLACE(0x53fe70, Widget_Move);

// 0x53fe90. A window's event handler: children (+0x74, next +0x8a) see the event
// first and any of them returning 0 ends it (0). A press (type 6) on a draggable
// window (+0x60) inside its rectangle returns 1, unless the window may be dragged
// (+0x15) and the press hit its own title part (a type-4 child of it without flag
// 0x800, or any widget with flag 0x200000): then the window follows the mouse
// (screen coordinates clamped to 0..1, keeping the grab offset) in a modal loop
// until the button is released (type 7), calling the idle callback (0x6d4004) at
// least every 20 ms; returns 0. Everything else goes to the window's own handler
// (vtbl +4).
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint32_t type = ev[0];
    for (uint8_t* c = F<uint8_t*>(w, 0x74); c; c = F<uint8_t*>(c, 0x8a))
        if (!((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))(*(void***)c)[1])(c, 0, ev)) return 0;
    auto base = [&]() { return ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))(*(void***)w)[1])(w, 0, ev); };
    if (type != 6 || !F<int32_t>(w, 0x60)) return base();
    double rx = (double)*(float*)&ev[9] - F<float>(w, 5); // compared unrounded, as the original
    float ry = (float)((double)*(float*)&ev[10] - F<float>(w, 9));
    if (!(rx > 0.0) || !(ry > 0.0f)) return 1;
    if (!(rx < F<float>(w, 0xd)) || !(ry < F<float>(w, 0x11))) return 1;
    if (!F<int32_t>(w, 0x15)) return base();
    uint8_t* hit = (uint8_t*)(uintptr_t)ev[0xb];
    if (!hit) return base();
    bool title = F<uint8_t*>(hit, 0x86) == w && F<uint16_t>(hit, 6) == 4 && !(F<uint32_t>(hit, 0x33) & 0x800);
    if (!title && !(F<uint32_t>(hit, 0x33) & 0x200000)) return base();
    float grabX = *(float*)&ev[9], grabY = *(float*)&ev[10];
    float startX = F<float>(w, 5), startY = F<float>(w, 9);
    int32_t last = ClockMs();
    uint8_t* input = g_input;
    Input_Block(input);
    while (ev[0] != 7) {
        Engine_Nop();
        uint32_t next[12];
        memcpy(ev, Input_NextEvent(g_input, 0, (InputEvent*)next), 0x30);
        if (ev[0] == 2) {
            auto clamp01 = [](float v) {
                double r = v > 0.0f ? (double)v : (double)Fl(0x5c8598);
                return r < D(0x5c8770) ? r : D(0x5c8770);
            };
            double cx = clamp01(*(float*)&ev[9]);
            double cy = clamp01(*(float*)&ev[10]);
            float ny = (float)((cy + startY) - grabY);
            float nx = (float)((cx + startX) - grabX);
            Widget_Move(w, 0, nx, ny);
        }
        if (ClockMs() - last > 0x14 || ev[0] == 0) {
            if (void* idle = g_6d4004) ((void(*)())idle)();
            last = ClockMs();
        }
    }
    Input_Unblock(g_input);
    return 0;
}
RT3_REPLACE(0x53fe90, Window_HandleEvent);

}  // namespace rt3
