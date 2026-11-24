// Widgets.cpp - RT3.exe 0x558000-0x55c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <intrin.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);
void __fastcall String_Assign(char** s, const char* src);
int32_t __fastcall Clipboard_GetText(char* buf, int32_t size);
void __fastcall EditBox_InsertCursor(uint8_t* w, int, const char* text, int32_t pos);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
int32_t __fastcall IsSingleByte(uint16_t c);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall NameTable_PushPos(uint8_t* t);
void __fastcall NameTable_PopPosition(uint8_t* t);
double __fastcall Widget_Pan_Impl(uint8_t* w);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Texture_Destroy(uint8_t* t);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Widget_Init(uint8_t* w);
void __fastcall Scrollbar_Init(uint8_t* w);
void __fastcall ListBox_Init(uint8_t* w);
void __fastcall EditBox_Init(uint8_t* w);
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
void __fastcall String_Free(char** s);
void __fastcall HandleTable_Release(uint8_t* t, int, int32_t h);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
double __fastcall Scroll_Total(uint8_t* s);

int32_t __fastcall Font_HitTest(uint8_t* f, int, float px, float py, const char* text, float x, float y, float w, float h, uint32_t color,
                                int32_t align, int32_t valign, uint32_t flags, float scale);

uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
int32_t __fastcall Widget_TextLines(uint8_t* w);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Widget_SetState(uint8_t* w, int, uint32_t mask);
void __fastcall Widget_ClearState(uint8_t* w, int, uint32_t mask);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary);
void __fastcall Widget_Destruct(uint8_t* w);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
int32_t ClockMs();

struct InputEvent {
    uint32_t v[12];
};

void __fastcall TextBox_MoveCursor(uint8_t* e, int, const char* str, int32_t delta);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
void Engine_Nop();
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
int32_t __fastcall EditBox_HitTest(uint8_t* w, int, float px, float py);
void __fastcall EditBox_DeleteAt(uint8_t* w, int, char* s, int32_t pos);
void __fastcall EditBox_Paste(uint8_t* w, int, char* dst);
int32_t __fastcall Clipboard_SetText(const char* text);
uint32_t IsEnglish();
void __fastcall EditBox_ScrollStates(uint8_t* w, int, int32_t* up, int32_t* down);

int32_t __fastcall Font_MoveCaretLines(uint8_t* f, int, int32_t pos, int32_t dir, const char* text, float x, float y, float w, float h, uint32_t color,
                                       int32_t align, int32_t valign, uint32_t flags, float scale);

void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
int32_t __fastcall Widget_IsEnabled(uint8_t* w);
int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row);
void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

void __fastcall Dropdown_Select(uint8_t* d, int, int32_t idx, int32_t sound, int32_t renotify);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
int32_t __fastcall Widget_HitTest(uint8_t* w, int, float x, float y);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
void Widget_Pan();
void Tutorial_Advance();
void Fade_Step();

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

void __fastcall Fx_QueueFadedQuad(uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, uint32_t s4, uint32_t clip, uint32_t s6, uint32_t,
                                  uint32_t s8, uint32_t s9, uint32_t s10, uint32_t s11);

void __fastcall Nop558750(void*);

// Functions of this file, in exe order.
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Window_DeleteChild(uint8_t* win, int, uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
RT3_NOINLINE int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
RT3_NOINLINE int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec);
RT3_NOINLINE void __fastcall Widget_Destruct(uint8_t* w);
void __fastcall Widget_Init(uint8_t* w);
void __fastcall Widget_Attach(uint8_t* w, int, uint8_t* parent);
RT3_NOINLINE void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary);
RT3_NOINLINE int32_t __fastcall Widget_HitTest(uint8_t* w, int, float x, float y);
int32_t __fastcall Widget_Contains(uint8_t* w, int, float x, float y, int32_t mode);
void __fastcall Nop558400(void*, int, int32_t, int32_t, int32_t, int32_t);
void __fastcall Nop558410(void*);
void __fastcall Nop558420(void*);
void __fastcall Nop558430(void*);
void __fastcall Nop558440(void*, int, int32_t, int32_t);
void __fastcall Nop558450(void*, int, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Nop558460(void*, int, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Widget_SetState(uint8_t* w, int, uint32_t mask);
RT3_NOINLINE void __fastcall Widget_ClearState(uint8_t* w, int, uint32_t mask);
double __fastcall Widget_Pan_Impl(uint8_t* w);
void Widget_Pan();
RT3_NOINLINE int32_t __fastcall Widget_Send(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);
RT3_NOINLINE uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE int32_t __fastcall Widget_IsVisible(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Widget_IsEnabled(uint8_t* w);
int32_t __fastcall Widget_Usable10(uint8_t* w);
int32_t __fastcall Widget_Usable20(uint8_t* w);
RT3_NOINLINE void __fastcall Nop558750(void*);
RT3_NOINLINE void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
RT3_NOINLINE void __fastcall Widget_OnLeave(uint8_t* w);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Widget_ReleaseResources(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
RT3_NOINLINE int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
void __fastcall Scrollbar_Init(uint8_t* w);
void __fastcall Slider_ApplyQueued(uint8_t* w);
void __fastcall Widget_QueueAction(uint8_t* w);
void __fastcall Widget_SetPicture(uint8_t* g, int, const void* pixels, int32_t w, int32_t h);
void __fastcall Panel_Draw(uint8_t* w);
int32_t __fastcall IconButton_HandleEvent(uint8_t* w, int, uint32_t* msg);
RT3_NOINLINE void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint8_t* __fastcall Scrollbar_Load(uint8_t* w, int, int32_t);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Thunk55a080(void* p0);
uint8_t* __fastcall Scrollbar_Delete(uint8_t* w, int, int32_t flags);
void __fastcall ListBox_Destruct(uint8_t* w);
void __fastcall Slider_Layout(uint8_t* s);
void __fastcall ListBox_Init(uint8_t* w);
RT3_NOINLINE void __fastcall Scrollbar_SetPos(uint8_t* w, int, uint32_t v);
RT3_NOINLINE void __fastcall Dropdown_Select(uint8_t* d, int, int32_t idx, int32_t sound, int32_t renotify);
void __fastcall Slider_Draw(uint8_t* w);
uint8_t* __fastcall ListBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall ListBox_DeletingDestructor(uint8_t* w, int, uint32_t flags);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Thunk55ab90(void* p0);
RT3_NOINLINE int32_t __fastcall Scrollbar_HandleMouse(uint8_t* sb, int, uint32_t* ev);
int32_t __fastcall Scrollbar_HandleEvent(uint8_t* sb, int, uint32_t* ev);
void __fastcall EditBox_Init(uint8_t* w);
void __fastcall TextWidget_Layout(uint8_t* w);
int32_t __fastcall Widget_LinesToCaret(uint8_t* w);
void __fastcall EditBox_DeleteAt(uint8_t* w, int, char* s, int32_t pos);
void __fastcall TextBox_MoveCursor(uint8_t* e, int, const char* str, int32_t delta);
RT3_NOINLINE int32_t __fastcall Widget_TextLines(uint8_t* w);
uint8_t* __fastcall EditBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall EditBox_Delete(uint8_t* w, int, int32_t flags);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall EditBox_ScrollStates(uint8_t* w, int, int32_t* up, int32_t* down);
void __fastcall EditBox_Draw(uint8_t* w);
void __fastcall EditBox_InsertCursor(uint8_t* w, int, const char* text, int32_t pos);
void __fastcall EditBox_Paste(uint8_t* w, int, char* dst);
int32_t __fastcall EditBox_HitTest(uint8_t* w, int, float px, float py);
RT3_NOINLINE void __fastcall EditBox_RunModal(uint8_t* w, int, int32_t caret);
RT3_NOINLINE void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);

static inline void MemMove(void* d, const void* s, size_t n) { ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(d, s, n); }

static inline size_t Len(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }

static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline void* ExeMalloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }

// Pack-loaded widgets (the constructors take an unused argument): a bare base, the
// kind's init, then an `n`-byte record read from the pack's current position.
static uint8_t* Widget_ReadRecord(uint8_t* w, int32_t n) {
    uint8_t* rec = (uint8_t*)ExeMalloc(n);
    NameTable_Read(g_pack, 0, rec, n);
    Widget_LoadBase(w, 0, rec);
    return rec;
}

// 0x55b900 / 0x55ba70. Edit box (vtable 0x5ddd98): fields +0xa2.., its text +0xba
// (from pack block 0xbbb, else 0x5d09c4) duplicated with a spare byte.
static void EditBox_DupText(uint8_t* w) {
    char* s = F<char*>(w, 0xba);
    char* d = (char*)ExeMalloc(strlen(s) + 2);
    strcpy(d, s);
    F<char*>(w, 0xba) = d;
}

// Widget scalar-deleting destructors: own cleanup, the base (0x558000), then
// delete when `flags` bit 0 is set.
static uint8_t* Widget_Finish(uint8_t* w, int32_t flags) {
    Widget_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}

static void FreeField(uint8_t* w, uint32_t off) {
    if (F<void*>(w, off)) Free(F<void*>(w, off));
    F<void*>(w, off) = nullptr;
}

static inline double Fl(uint32_t a) { return *(const float*)a; }

// Makes room for `extra` more characters in edit box w's string +0xba
// (reallocated to the work buffer's length + extra + 10).
static void EditBox_GrowText(uint8_t* w, const char* buf, size_t extra) {
    char** text = (char**)(w + 0xba);
    char* p = ((char*(__cdecl*)(size_t))0x5a125d)(strlen(buf) + extra + 10);
    strcpy(p, *text);
    ((void(__cdecl*)(void*))0x5a1145)(*text);
    *text = p;
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

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// Ui_PlaySound for widget w: its pan as float bits.
static void Widget_PlaySound(uint8_t* w, int32_t id) {
    float pan = (float)((double(__fastcall*)(uint8_t*))0x5584d0)(w);
    uint32_t bits;
    memcpy(&bits, &pan, 4);
    Ui_PlaySound(id, 0, bits);
}

namespace {
// Widget slot 0x20: whether (x, y) hits it (third argument 1).
int32_t WidgetHit453(uint8_t* w, uint32_t x, uint32_t y) {
    return ((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)w)[0x20 / 4])(w, x, y, 1);
}
int32_t WidgetSlot453(uint8_t* w, uint32_t off) { return ((int32_t(__thiscall*)(uint8_t*))(*(void***)w)[off / 4])(w); }
// The click callback +0x9a gets the widget id (ECX), itself (EDX) and the
// click relative to the widget.
void WidgetClickCb453(uint8_t* w, uint32_t* msg) {
    auto cb = F<void(__fastcall*)(uint32_t, void*, float, float)>(w, 0x9a);
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float ry = (float)((double)*(const float*)&msg[10] - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
    const float rx = (float)((double)*(const float*)&msg[9] - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
    cb(F<uint16_t>(w, 4), (void*)cb, rx, ry);
}
}

// 0x558000. Widget destructor body: forgets the window manager's focus / hover
// pointers to it (+0xc6d / +0xc71), frees its names and drops its fonts.
RT3_NOINLINE void __fastcall Widget_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5ddcd0;
    uint8_t* m = g_windowManager;
    if (F<uint8_t*>(m, 0xc6d) == w) {
        F<uint8_t*>(m, 0xc6d) = nullptr;
        m = g_windowManager;
    }
    if (F<uint8_t*>(m, 0xc71) == w) F<uint8_t*>(m, 0xc71) = nullptr;
    String_Free(&F<char*>(w, 0x43));
    String_Free(&F<char*>(w, 0x47));
    if (F<void*>(w, 0x63)) Pack_Release(g_pack, 0, F<void*>(w, 0x63));
    F<void*>(w, 0x63) = nullptr;
    if (F<void*>(w, 0x92)) Pack_Release(g_pack, 0, F<void*>(w, 0x92));
    F<void*>(w, 0x92) = nullptr;
}
RT3_REPLACE(0x558000, Widget_Destruct);

// 0x558080. Widget defaults.
RT3_NOINLINE void __fastcall Widget_Init(uint8_t* w) {
    static const uint8_t zero32[] = { 0xb, 0xf, 0x13, 0x17, 0x43, 0x47, 0x63, 0x4b, 0x33, 0x1b, 0x1f, 0x23, 0x27,
                                      0x4f, 0x6c, 0x70, 0x74, 0x78 };
    F<int32_t>(w, 0x86) = 0;
    F<int32_t>(w, 0x8a) = 0;
    F<int32_t>(w, 0x8e) = 0;
    F<uint16_t>(w, 4) = 0;
    F<uint16_t>(w, 0x7c) = 0;
    F<uint16_t>(w, 9) = 0;
    for (uint8_t off : zero32) F<int32_t>(w, off) = 0;
    w[0x2b] = w[0x2c] = 0;
    F<int32_t>(w, 0x92) = 0;
    w[0x37] = 0;
    w[0x67] = 0;
    F<uint16_t>(w, 6) = 0;
    F<int32_t>(w, 0x38) = -1;
    F<uint16_t>(w, 0x6a) = 3;
    F<uint16_t>(w, 0x68) = 3;
    F<uint16_t>(w, 0x40) = 0;
    w[0x42] = 0;
    w[0x2d] = w[0x2e] = w[0x2f] = w[0x30] = 0;
    F<uint16_t>(w, 0x7e) = 0xffff;
    F<uint16_t>(w, 0x80) = 0xffff;
    F<int32_t>(w, 0x96) = 0;
    F<int32_t>(w, 0x9a) = 0;
    F<int32_t>(w, 0x9e) = 0;
}
RT3_REPLACE(0x558080, Widget_Init);

// 0x558130. Attaches the widget to its parent window: caption string (+0x40 ->
// +0x43), default id, then vtbl +0x14 and +0x3c.
void __fastcall Widget_Attach(uint8_t* w, int, uint8_t* parent) {
    F<uint8_t*>(w, 0x86) = parent;
    if (uint16_t id = F<uint16_t>(w, 0x40)) String_Assign((char**)(w + 0x43), GetString(id));
    if (!F<int32_t>(w, 0x38)) F<int32_t>(w, 0x38) = -1;
    ((void(__thiscall*)(void*))(*(void***)w)[0x14 / 4])(w);
    ((void(__thiscall*)(void*))(*(void***)w)[0x3c / 4])(w);
}
RT3_REPLACE(0x558130, Widget_Attach);

// 0x558180. Switches a widget to font `font` (+0x37, acquired into +0x63); unless
// `temporary`, it also becomes the widget's normal font (+0x67).
RT3_NOINLINE void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary) {
    if (F<void*>(w, 0x63)) Pack_Release(g_pack, 0, F<void*>(w, 0x63));
    w[0x37] = (uint8_t)font;
    NameTable_PushPos(g_pack);
    F<uint8_t*>(w, 0x63) = Pack_AcquireFont(g_pack, 0, w[0x37]);
    NameTable_PopPosition(g_pack);
    if (!temporary) w[0x67] = w[0x37];
}
RT3_REPLACE(0x558180, Widget_SetFont);

// 0x5581e0. Whether (x, y) hits the widget: always for flag 2, never for 0x4000;
// the padded rectangle (+0x1b.., 0x5ddd10 margin) or, for types 1 and 4, +0xb..
RT3_NOINLINE int32_t __fastcall Widget_HitTest(uint8_t* w, int, float x, float y) {
    uint32_t fl = F<uint32_t>(w, 0x33);
    if (fl & 2) return 1;
    if (fl & 0x4000) return 0;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const double m = *(const double*)0x5ddd10;
    if (!(F<float>(w, 0x23) == *(const float*)0x5c8598)) {
        if (!(((double)F<float>(p, 5) + F<float>(w, 0x1b)) - m <= x)) return 0;
        if (!(((double)F<float>(w, 0x1f) + F<float>(p, 9)) - m <= y)) return 0;
        if (!((((double)F<float>(p, 5) + F<float>(w, 0x23)) + F<float>(w, 0x1b)) + m >= x)) return 0;
        return (((double)F<float>(w, 0x27) + F<float>(w, 0x1f)) + F<float>(p, 9)) + m >= y ? 1 : 0;
    }
    int16_t t = F<int16_t>(w, 6);
    if (t != 1 && t != 4) return 0;
    p = F<uint8_t*>(w, 0x86);
    if (!((double)F<float>(p, 5) + F<float>(w, 0xb) <= x)) return 0;
    if (!((double)F<float>(p, 9) + F<float>(w, 0xf) <= y)) return 0;
    if (!(((double)F<float>(w, 0x13) + F<float>(p, 5)) + F<float>(w, 0xb) >= x)) return 0;
    return (((double)F<float>(w, 0x17) + F<float>(p, 9)) + F<float>(w, 0xf)) >= y ? 1 : 0;
}
RT3_REPLACE(0x5581e0, Widget_HitTest);

// 0x5582f0 (vtable). Whether point (x, y) hits widget w (visible: slot 0x2c,
// or slot 0x30 with a mode) within its padded box (0x5ddd10); buttons
// (kinds 1 / 4) test their shape, scroll bars (kind 8 in mode != 2, no
// +0x126, with +0x172) only up to +0xce from the left.
int32_t __fastcall Widget_Contains(uint8_t* w, int, float x, float y, int32_t mode) {
    auto slot = [&](uint32_t off) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)w)[off / 4])(w); };
    if (!mode ? !slot(0x2c) : !slot(0x30)) return 0;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float left = (float)((double)F<float>(p, 5) + (double)F<float>(w, 0xb));
    const double pad = D(0x5ddd10);
    if (!((double)left - pad <= (double)x)) return 0;
    if (!((double)F<float>(w, 0xf) + (double)F<float>(p, 9) - pad <= (double)y)) return 0;
    if (!((double)F<float>(w, 0x13) + (double)F<float>(p, 5) + (double)F<float>(w, 0xb) + pad >= (double)x)) return 0;
    if (!((double)F<float>(w, 0x17) + (double)F<float>(w, 0xf) + (double)F<float>(p, 9) + pad >= (double)y)) return 0;
    const uint16_t kind = F<uint16_t>(w, 6);
    if (kind == 1 || kind == 4) return Widget_HitTest(w, 0, x, y) ? 1 : 0;
    if (kind == 8 && mode != 2 && !F<int32_t>(w, 0x126) && F<int32_t>(w, 0x172))
        if ((double)F<float>(w, 0xce) < (double)x - (double)left) return 0;
    return 1;
}
RT3_REPLACE(0x5582f0, Widget_Contains);

// 0x558400 (vtable stub).
void __fastcall Nop558400(void*, int, int32_t, int32_t, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x558400, Nop558400);

// 0x558410 (vtable stub).
void __fastcall Nop558410(void*) { __asm nop }
RT3_REPLACE(0x558410, Nop558410);

// 0x558420 (vtable stub).
void __fastcall Nop558420(void*) { __asm nop }
RT3_REPLACE(0x558420, Nop558420);

// 0x558430 (vtable stub).
void __fastcall Nop558430(void*) { __asm nop }
RT3_REPLACE(0x558430, Nop558430);

// 0x558440 (vtable stub).
void __fastcall Nop558440(void*, int, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x558440, Nop558440);

// 0x558450 (vtable stub).
void __fastcall Nop558450(void*, int, int32_t, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x558450, Nop558450);

RT3_NOINLINE void __fastcall Nop558460(void*, int, int32_t, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x558460, Nop558460);

// 0x558470 / 0x5584a0. Set / clear state bits (+0x68), notifying (vtbl +0x1c) on change.
RT3_NOINLINE void __fastcall Widget_SetState(uint8_t* w, int, uint32_t mask) {
    uint32_t v = F<uint16_t>(w, 0x68);
    if ((v & mask) == mask) return;
    F<uint16_t>(w, 0x68) = (uint16_t)(v | mask);
    ((void(__thiscall*)(void*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x558470, Widget_SetState);

RT3_NOINLINE void __fastcall Widget_ClearState(uint8_t* w, int, uint32_t mask) {
    uint32_t v = F<uint16_t>(w, 0x68);
    if (!(mask & v)) return;
    F<uint16_t>(w, 0x68) = (uint16_t)(~mask & v);
    ((void(__thiscall*)(void*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x5584a0, Widget_ClearState);

// 0x5584d0. Horizontal position of the widget centre as a 0..1 pan value.
RT3_NOINLINE double __fastcall Widget_Pan_Impl(uint8_t* w) {
    uint8_t* p = F<uint8_t*>(w, 0x86);
    if (!p) return *(const float*)0x5c8d48;
    double v = (((double)F<float>(p, 5) + F<float>(w, 0xb)) + F<float>(w, 0x13) * *(const double*)0x5c8628) *
                   *(const double*)0x5c8680 + *(const double*)0x5c8988;
    if (v < *(const float*)0x5c8598) return *(const float*)0x5c8598;
    if (v > *(const float*)0x5c85b4) return *(const float*)0x5c85b4;
    return v;
}

// Callers (0x55876a, 0x55884a, 0x558fe6) keep a value in DL across the call.
__declspec(naked) void Widget_Pan() { RT3_KEEP_ECX_EDX(Widget_Pan_Impl, 0) }
RT3_REPLACE(0x5584d0, Widget_Pan);

// 0x558530. Sends a 48-byte message {a, b, c, d, e, 0...} to the widget (vtbl +4).
RT3_NOINLINE int32_t __fastcall Widget_Send(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint32_t msg[12] = { a, b, c, d, e };
    return ((int32_t(__thiscall*)(void*, void*))(*(void***)w)[1])(w, msg);
}
RT3_REPLACE(0x558530, Widget_Send);

// Widget definition records (from a dialog template or the pack): the common
// 0x5f-byte head (copied to +4: id +4, rectangle +0xb..+0x17, font +0x37, the two
// names +0x3f / +0x43) followed by the kind's own fields at +0x5f.

// 0x558580. Base widget constructor from definition `def` (or none): the names are
// duplicated, the second one also loaded as a bitmap font (+0x92); font +0x37
// (default 3) is acquired into +0x63 and the rectangle kept as the original one.
RT3_NOINLINE uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def) {
    F<uint32_t>(w, 0) = 0x5ddcd0;
    Widget_Init(w);
    if (!def) return w;
    memcpy(w + 4, def, 0x5f);
    F<char*>(w, 0x43) = nullptr;
    F<char*>(w, 0x47) = nullptr;
    String_Assign(&F<char*>(w, 0x43), *(const char* const*)(def + 0x3f));
    String_Assign(&F<char*>(w, 0x47), *(const char* const*)(def + 0x43));
    uint8_t* pack = g_pack;
    if (F<char*>(w, 0x47)) {
        NameTable_PushPos(pack);
        F<uint8_t*>(w, 0x92) = Pack_AcquireNamed(pack, 0, F<char*>(w, 0x47));
        NameTable_PopPosition(g_pack);
    }
    if (!w[0x37]) w[0x37] = 3;
    w[0x67] = w[0x37];
    NameTable_PushPos(g_pack);
    F<uint8_t*>(w, 0x63) = Pack_AcquireFont(g_pack, 0, w[0x37]);
    NameTable_PopPosition(g_pack);
    memcpy(w + 0x6c, w + 0xb, 0x10);
    F<uint16_t>(w, 0x7c) = F<uint16_t>(w, 4);
    return w;
}
RT3_REPLACE(0x558580, Widget_Construct);

// 0x558670 / 0x5586c0. Whether widget w and all its parents (+0x4b) are visible
// (flag 1) / enabled (flag 2) in both their state (+0x6a) and mask (+0x68), have a
// handler for it (+0x86 -> +0x5c / +0x60), and the engine isn't busy (+0x57).
RT3_NOINLINE int32_t __fastcall Widget_IsVisible(uint8_t* w) {
    if (!(w[0x6a] & 1) || !(w[0x68] & 1) || !F<uint8_t*>(w, 0x86) || !F<void*>(F<uint8_t*>(w, 0x86), 0x5c)) return 0;
    if (F<uint8_t*>(w, 0x4b) && !Widget_IsVisible(F<uint8_t*>(w, 0x4b))) return 0;
    return g_engine[0x57] ? 0 : 1;
}
RT3_REPLACE(0x558670, Widget_IsVisible);

RT3_NOINLINE int32_t __fastcall Widget_IsEnabled(uint8_t* w) {
    if (!(w[0x6a] & 2) || !(w[0x68] & 2) || !F<uint8_t*>(w, 0x86) || !F<void*>(F<uint8_t*>(w, 0x86), 0x60)) return 0;
    if (F<uint8_t*>(w, 0x4b) && !Widget_IsEnabled(F<uint8_t*>(w, 0x4b))) return 0;
    return g_engine[0x57] ? 0 : 1;
}
RT3_REPLACE(0x5586c0, Widget_IsEnabled);

// 0x558710 / 0x558730 (vtables). Enabled widget without flag 0x10 / 0x20 of
// +0x33.
int32_t __fastcall Widget_Usable10(uint8_t* w) { return !(w[0x33] & 0x10) && Widget_IsEnabled(w) ? 1 : 0; }
RT3_REPLACE(0x558710, Widget_Usable10);

int32_t __fastcall Widget_Usable20(uint8_t* w) { return !(w[0x33] & 0x20) && Widget_IsEnabled(w) ? 1 : 0; }
RT3_REPLACE(0x558730, Widget_Usable20);

// 0x558750 (vtable stub).
RT3_NOINLINE void __fastcall Nop558750(void*) { __asm nop }
RT3_REPLACE(0x558750, Nop558750);

// 0x558760. Mouse over widget w at (x, y): plays its hover sound (+0x2f, panned);
// with `moved` calls the move callback +0x9e (id, then x / y relative to the
// widget: x - (parent +5 + own +0xb)); with `track` calls the drag callback +0x96
// (id, w is the hovered widget, 1, x - parent +5 + own +0xb (sic: the offset is
// added here), likewise y, -1, -1); and when w just became the hovered widget
// (ui +0xc6d) shows its tooltip +0x43 (sticky).
RT3_NOINLINE void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track) {
    if (uint8_t snd = w[0x2f]) {
        float pan = (float)Widget_Pan_Impl(w);
        Ui_PlaySound(snd, 0, FBits(pan));
    }
    if (moved && F<void*>(w, 0x9e)) {
        uint8_t* p = F<uint8_t*>(w, 0x86);
        float rx = (float)((double)x - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
        float ry = (float)((double)y - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
        F<void(__fastcall*)(uint32_t, void*, float, float)>(w, 0x9e)(F<uint16_t>(w, 4), F<void*>(w, 0x9e), rx, ry);
    }
    if (track && F<void*>(w, 0x96)) {
        uint8_t* p = F<uint8_t*>(w, 0x86);
        float ry = (float)((double)y - F<float>(p, 9) + F<float>(w, 0xf));
        float rx = (float)((double)x - F<float>(p, 5) + F<float>(w, 0xb));
        uint32_t hovered = F<uint8_t*>(g_windowManager, 0xc6d) == w;
        F<void(__fastcall*)(uint32_t, uint32_t, int32_t, float, float, int32_t, int32_t)>(w, 0x96)(F<uint16_t>(w, 4), hovered, 1, rx, ry, -1, -1);
    }
    uint8_t* ui = g_windowManager;
    if (F<uint8_t*>(ui, 0xc6d) == w) return;
    F<uint8_t*>(ui, 0xc6d) = w;
    Ui_ShowTooltip(g_windowManager, 0, F<const char*>(w, 0x43), 1, 1, -1);
}
RT3_REPLACE(0x558760, Widget_OnHover);

// 0x558840. Mouse left widget w: plays its leave sound (+0x30, panned), tells the
// drag callback +0x96 (id, 1, 0, 0, 0, -1, -1), stops being the hovered widget and
// clears the tooltip.
RT3_NOINLINE void __fastcall Widget_OnLeave(uint8_t* w) {
    if (uint8_t snd = w[0x30]) {
        float pan = (float)Widget_Pan_Impl(w);
        Ui_PlaySound(snd, 0, FBits(pan));
    }
    if (F<void*>(w, 0x96))
        F<void(__fastcall*)(uint32_t, uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t)>(w, 0x96)(F<uint16_t>(w, 4), 1, 0, 0, 0, -1, -1);
    uint8_t* ui = g_windowManager;
    if (F<uint8_t*>(ui, 0xc6d) == w) F<uint8_t*>(ui, 0xc6d) = nullptr;
    Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 1, 0, -1);
}
RT3_REPLACE(0x558840, Widget_OnLeave);

// 0x5588b0. Clears state flags `mask` of widget w (+0x6a): losing focus (4) while
// enabled notifies it (vtbl +0x10, unless it stays visible and focused), losing 0x10
// resets the global hover (0x6d401c +0xc71); then the widget redraws (vtbl +0x1c).
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask) {
    uint16_t f = F<uint16_t>(w, 0x6a);
    if (!(mask & f)) return;
    uint32_t nf = ~mask & f;
    if (f & 4) {
        if (Widget_IsEnabled(w) && (!(nf & 4) || !(f & 1))) {
            nf &= ~4u;
            ((void(__fastcall*)(uint8_t*))(*(void***)w)[0x10 / 4])(w);
        }
    }
    if ((w[0x6a] & 0x10) && !(nf & 0x10)) {
        nf &= ~0x10u;
        F<int32_t>(g_windowManager, 0xc71) = 0;
    }
    F<uint16_t>(w, 0x6a) = (uint16_t)nf;
    ((void(__fastcall*)(uint8_t*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x5588b0, Widget_ClearFlags);

// 0x558930. Drops a widget's focus (flag 4) and its pack references (font +0x63,
// bitmap font +0x92).
void __fastcall Widget_ReleaseResources(uint8_t* w) {
    if (w[0x6a] & 4) Widget_ClearFlags(w, 0, 4);
    if (void* f = F<void*>(w, 0x63)) {
        Pack_Release(g_pack, 0, f);
        F<void*>(w, 0x63) = nullptr;
    }
    if (void* f = F<void*>(w, 0x92)) {
        Pack_Release(g_pack, 0, f);
        F<void*>(w, 0x92) = nullptr;
    }
}
RT3_REPLACE(0x558930, Widget_ReleaseResources);

// 0x558980. Sets state flags `mask` of widget w (+0x6a); nothing (returns the old
// flags) when all are already set. Gaining focus (4) takes it from the previous
// focus widget (UI manager +0xc6d) and notifies w (vtbl +0xc with a, b, 1, 1);
// gaining 0x10 makes w the hover widget (+0xc71, clearing the old one's 0x10).
// Then the widget redraws (vtbl +0x1c, whose result is returned).
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b) {
    uint32_t old = F<uint16_t>(w, 0x6a);
    if ((old & mask) == mask) return old;
    uint32_t nf = old | mask;
    F<uint16_t>(w, 0x6a) = (uint16_t)nf;
    if (!(old & 4) && (nf & 4)) {
        if (uint8_t* focus = F<uint8_t*>(g_windowManager, 0xc6d)) Widget_ClearFlags(focus, 0, 4);
        ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t, int32_t))(*(void***)w)[0xc / 4])(w, 0, a, b, 1, 1);
    }
    if (!(old & 0x10) && (w[0x6a] & 0x10)) {
        if (uint8_t* hover = F<uint8_t*>(g_windowManager, 0xc71)) Widget_ClearFlags(hover, 0, 0x10);
        F<uint8_t*>(g_windowManager, 0xc71) = w;
    }
    return ((uint32_t(__fastcall*)(uint8_t*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x558980, Widget_SetFlags);

// 0x558a10. The base widget's event handler (event: +0 type, +4 target id,
// +8 / +0xc arguments, +0x18 / +0x1c float arguments, +0x24 / +0x28 mouse,
// +0x2c handling widget). Handles the generic commands addressed to its id
// (+4; some to the group id +9 or +0x7c): image (+0x47, re-acquired from the
// image pack), tooltip (+0x43, shown at once when hovered), flags and state
// bits, position / size moves (in 1/1600 and 1/1200 units or raw floats),
// colours, callbacks, font; and the mouse: hover (2), press (6, sound +0x2d,
// click callback +0x9a with widget-relative coordinates, else a 0xca event),
// release (7, sound +0x2e, 0xcb event) and the drag / wheel hits 0x1e..0x22.
// Returns 0 when the event is consumed.
RT3_NOINLINE int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e) {
    typedef int32_t(__fastcall * HitFn)(uint8_t*, int, uint32_t, uint32_t, int32_t);
    auto vt = [&](uint32_t off) { return (*(void***)w)[off / 4]; };
    auto hit = [&](int32_t inside) { return ((HitFn)vt(0x20))(w, 0, F<uint32_t>(e, 0x24), F<uint32_t>(e, 0x28), inside); };
    auto enabled = [&](uint32_t off) { return ((int32_t(__fastcall*)(uint8_t*))vt(off))(w); };
    uint32_t id = F<uint16_t>(w, 4), group = F<uint16_t>(w, 9);
    uint32_t target = F<uint32_t>(e, 4);
    auto setImage = [&](bool redrawAlways) {
        String_Free(&F<char*>(w, 0x47));
        if (F<void*>(w, 0x92)) Pack_Release(g_pack, 0, F<void*>(w, 0x92));
        F<void*>(w, 0x92) = nullptr;
        String_Assign(&F<char*>(w, 0x47), F<const char*>(e, 8));
        if (F<char*>(w, 0x47)) {
            NameTable_PushPos(g_pack);
            F<uint8_t*>(w, 0x92) = Pack_AcquireNamed(g_pack, 0, F<char*>(w, 0x47));
            NameTable_PopPosition(g_pack);
            if (!redrawAlways) ((void(__fastcall*)(uint8_t*))vt(0x34))(w);
        }
        if (redrawAlways) ((void(__fastcall*)(uint8_t*))vt(0x34))(w);
    };
    auto setTooltip = [&]() -> int32_t {
        if (F<void*>(w, 0x43)) Free(F<void*>(w, 0x43));
        F<void*>(w, 0x43) = nullptr;
        if (F<const char*>(e, 8)) String_Assign(&F<char*>(w, 0x43), F<const char*>(e, 8));
        if (F<uint8_t*>(g_windowManager, 0xc6d) == w)
            Ui_ShowTooltip(g_windowManager, 0, F<char*>(w, 0x43), 1, 1, -1);
        return F<int32_t>(e, 0) != 0x6e;
    };
    auto sound = [&](uint8_t snd) {
        if (!snd) return;
        float pan = (float)Widget_Pan_Impl(w);
        Ui_PlaySound(snd, 0, FBits(pan));
    };
    auto handled = [&](uint32_t type) {
        F<uint32_t>(e, 0) = type;
        F<uint32_t>(e, 4) = id;
        F<uint8_t*>(e, 0x2c) = w;
    };
    switch (F<int32_t>(e, 0)) {
    case 0x9f:
        if (target != id) break;
        F<uint32_t>(w, 0x82) = F<uint32_t>(e, 8);
        return 1;
    case 0x75: {
        if (target != id) break;
        const char* cur = F<const char*>(w, 0x47);
        const char* nw = F<const char*>(e, 8);
        if (!cur && !nw) break;
        if (cur && nw && !strcmp(cur, nw)) break;
        setImage(true);
        return 1;
    }
    case 0x91:
        if (target != group) break;
        F<uint16_t>(w, 4) = 0;
        return 1;
    case 0x92:
        if (target != group) break;
        F<uint16_t>(w, 4) = F<uint16_t>(w, 0x7c);
        return 1;
    case 0x7c:
        if (target != id) break;
        F<uint32_t>(w, 0x4f) = F<uint32_t>(e, 8);
        return 0;
    case 0x65:
        if (target != id) break;
        Widget_SetFlags(w, 0, F<uint32_t>(e, 8), 0, 0);
        return 0;
    case 0x87:
        if (target != id) break;
        Widget_ClearFlags(w, 0, F<uint32_t>(e, 8));
        return 0;
    case 0x6d:
        if (target != group) break;
        Widget_SetState(w, 0, F<uint32_t>(e, 8));
        return 1;
    case 0x86:
        if (target != group) break;
        Widget_ClearState(w, 0, F<uint32_t>(e, 8));
        return 1;
    case 0xb6:
    case 0xb5:
        if (target != (F<int32_t>(e, 0) == 0xb6 ? group : (uint32_t)F<uint16_t>(w, 0x7c))) break;
        F<float>(w, 0xb) = (float)((double)F<int32_t>(e, 8) * D(0x5ce8d0) + F<float>(w, 0xb));
        F<float>(w, 0xf) = (float)((double)F<int32_t>(e, 0xc) * D(0x5d0770) + F<float>(w, 0xf));
        return 1;
    case 0xb2:
        if (target != id) break;
        F<float>(w, 0xb) = (float)((double)F<int32_t>(e, 8) * D(0x5ce8d0));
        return 0;
    case 0xb3:
        if (target != id) break;
        F<float>(w, 0xf) = (float)((double)F<int32_t>(e, 8) * D(0x5d0770));
        return 0;
    case 0xb4:
        if (target != id) break;
        F<float>(w, 0xb) = (float)((double)F<int32_t>(e, 8) * D(0x5ce8d0));
        F<float>(w, 0xf) = (float)((double)F<int32_t>(e, 0xc) * D(0x5d0770));
        return 0;
    case 0x97:
        if (target != id) break;
        F<uint32_t>(w, 0xb) = F<uint32_t>(e, 0x18);
        return 0;
    case 0x98:
        if (target != id) break;
        F<uint32_t>(w, 0xf) = F<uint32_t>(e, 0x18);
        return 0;
    case 0x99:
        if (target != id) break;
        F<uint32_t>(w, 0xb) = F<uint32_t>(e, 0x18);
        F<uint32_t>(w, 0xf) = F<uint32_t>(e, 0x1c);
        return 0;
    case 0x70:
        if (target != id) break;
        setImage(false);
        return 0;
    case 0xa7:
        if (target != id) break;
        if (F<uint32_t>(w, 0x38) != F<uint32_t>(e, 8)) F<uint32_t>(w, 0x38) = F<uint32_t>(e, 8);
        return 0;
    case 0x9e:
        if (target != id) break;
        F<uint32_t>(w, 0x13) = F<uint32_t>(e, 0x18);
        F<uint32_t>(w, 0x17) = F<uint32_t>(e, 0x1c);
        return 0;
    case 0x9a:
        if (target != id) break;
        F<uint32_t>(w, 0x13) = F<uint32_t>(e, 0x18);
        return 0;
    case 0x9b:
        if (target != id) break;
        F<uint32_t>(w, 0x17) = F<uint32_t>(e, 0x18);
        return 0;
    case 0x7b:
        if (target != id) break;
        F<uint32_t>(w, 0x96) = F<uint32_t>(e, 8);
        return 1;
    case 0x81:
        if (target != id) break;
        F<uint32_t>(w, 0x9a) = F<uint32_t>(e, 8);
        return 1;
    case 0x82:
        if (target != id) break;
        F<uint32_t>(w, 0x9e) = F<uint32_t>(e, 8);
        return 1;
    case 0xa3:
        if (target != id) {
            uint8_t* parent = F<uint8_t*>(w, 0x4b);
            if (!parent || target != F<uint16_t>(parent, 4)) break;
        }
        return setTooltip();
    case 0x6e:
    case 0x77:
        if (target != id) break;
        return setTooltip();
    case 0xa2:
        if (target != id) break;
        F<uint32_t>(w, 0x33) &= ~F<uint32_t>(e, 8);
        return 1;
    case 0xa0:
        if (target != id) break;
        F<uint32_t>(w, 0x33) &= F<uint32_t>(e, 8);
        return 1;
    case 0xa1:
        if (target != id) break;
        F<uint32_t>(w, 0x33) |= F<uint32_t>(e, 8);
        return 1;
    case 0x1e:
    case 0x20:
    case 0x22:
        if (!enabled(0x30) || !hit(1)) break;
        if (F<int32_t>(e, 0) == 0x1e) F<uint32_t>(e, 0) = 0xcf;
        else F<uint32_t>(e, 0) = (F<int32_t>(e, 0) == 0x22) + 0xd0;
        F<uint32_t>(e, 4) = id;
        F<uint8_t*>(e, 0x2c) = w;
        return 1;
    case 0x6: {
        if (!enabled(0x30) || F<uint16_t>(w, 6) == 8 || !hit(1)) break;
        sound(w[0x2d]);
        void* cb = F<void*>(w, 0x9a);
        if (cb) {
            uint8_t* parent = F<uint8_t*>(w, 0x86);
            float ry = (float)((double)F<float>(e, 0x28) - ((double)F<float>(parent, 9) + F<float>(w, 0xf)));
            float rx = (float)((double)F<float>(e, 0x24) - ((double)F<float>(parent, 5) + F<float>(w, 0xb)));
            ((void(__fastcall*)(uint32_t, uint32_t, float, float))cb)(F<uint16_t>(w, 4), (uint32_t)(uintptr_t)cb, rx, ry);
            return 0;
        }
        Widget_SetFlags(w, 0, 0x10, 0, 0);
        handled(0xca);
        return 1;
    }
    case 0x7:
        if (!enabled(0x30) || F<uint16_t>(w, 6) == 1 || F<uint16_t>(w, 6) == 8 || !(w[0x6a] & 0x10)) break;
        sound(w[0x2e]);
        Widget_ClearFlags(w, 0, 0x10);
        handled(0xcb);
        F<uint32_t>(e, 8) = F<uint16_t>(w, 4);
        return 1;
    case 0xcb:
        if (!enabled(0x30) || F<uint32_t>(e, 8) || !hit(1)) break;
        F<uint32_t>(e, 8) = id;
        return 1;
    case 0x2:
        if (!enabled(0x2c)) break;
        if (!F<uint8_t*>(e, 0x2c) && hit(0)) {
            uint32_t mx = F<uint32_t>(e, 0x24), my = F<uint32_t>(e, 0x28);
            if (!(w[0x6a] & 4)) Widget_SetFlags(w, 0, 4, mx, my);
            else ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t, int32_t))vt(0xc))(w, 0, mx, my, 1, 1);
            F<uint8_t*>(e, 0x2c) = w;
            F<uint32_t>(e, 4) = F<uint16_t>(w, 4);
            return 1;
        }
        if ((w[0x6a] & 4) && F<uint8_t*>(e, 0x2c) != w) Widget_ClearFlags(w, 0, 4);
        return 1;
    case 0x78:
        if (target != id) break;
        Widget_SetFont(w, 0, F<int32_t>(e, 8), 0);
        return 1;
    }
    return 1;
}
RT3_REPLACE(0x558a10, Widget_HandleEvent);

// 0x559340. Scrollbar widget defaults (type 4).
RT3_NOINLINE void __fastcall Scrollbar_Init(uint8_t* w) {
    F<uint16_t>(w, 6) = 4;
    F<uint16_t>(w, 0xae) = 0;
    F<int32_t>(w, 0xb0) = 0;
    F<int32_t>(w, 0x10d) = 0;
    F<int32_t>(w, 0xb4) = 0;
    F<int32_t>(w, 0xb8) = -1;
    F<int32_t>(w, 0xf8) = 0;
    F<int32_t>(w, 0xfc) = 0;
    F<int32_t>(w, 0xec) = 0;
    F<int32_t>(w, 0xf0) = 0;
    F<int32_t>(w, 0xf4) = 0;
    F<uint32_t>(w, 0x100) = 0x3f800000;
    F<uint32_t>(w, 0x104) = 0x3f800000;
    F<uint16_t>(w, 0xbc) = 0;
    F<uint16_t>(w, 0xbe) = 0;
    F<uint16_t>(w, 0xc0) = 0;
    F<uint16_t>(w, 0xc2) = 0;
    F<uint32_t>(w, 0x108) = 0x3f800000;
    F<int32_t>(w, 0xa2) = 0;
    F<int32_t>(w, 0xa6) = 0;
    F<int32_t>(w, 0xaa) = 0;
    w[0x10c] = 0xff;
}
RT3_REPLACE(0x559340, Scrollbar_Init);

// 0x5593e0 (vtable). A slider widget with a queued position (+0xf4, index
// +0xae >= 0) takes it (+0xb / +0xf) and, scaled (+0x33 bit 0x800000),
// derives its step ratios (+0x100 / +0x104) from the glyph's steps, else
// takes the steps as its size (+0x13 / +0x17).
void __fastcall Slider_ApplyQueued(uint8_t* w) {
    if (!F<int32_t>(w, 0xf4) || F<int16_t>(w, 0xae) < 0) return;
    F<uint32_t>(w, 0xb) = F<uint32_t>(w, 0xf8);
    F<uint32_t>(w, 0xf) = F<uint32_t>(w, 0xfc);
    if (F<uint32_t>(w, 0x33) & 0x800000) {
        const float a = (float)Slider_StepA((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int16_t>(w, 0xae)));
        const double b = Slider_StepB((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int16_t>(w, 0xae)));
        if ((double)a > Fl(0x5c8598)) F<float>(w, 0x100) = (float)((double)F<float>(w, 0x13) / (double)a);
        if (b > Fl(0x5c8598)) F<float>(w, 0x104) = (float)((double)F<float>(w, 0x17) / b);
        return;
    }
    F<float>(w, 0x13) = (float)Slider_StepA((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int16_t>(w, 0xae)));
    F<float>(w, 0x17) = (float)Slider_StepB((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int16_t>(w, 0xae)));
}
RT3_REPLACE(0x5593e0, Slider_ApplyQueued);

// 0x5594c0 (vtable). A widget with a zero size (or flag 0x800000 of +0x33)
// and an action (+0x92) queues it: +0xf8 / +0xfc take +0xb / +0xf, +0xf4 is
// set and slot 0x34 runs.
void __fastcall Widget_QueueAction(uint8_t* w) {
    if (!(F<float>(w, 0x13) == 0.0f || F<float>(w, 0x17) == 0.0f || (F<uint32_t>(w, 0x33) & 0x800000))) return;
    if (!F<int32_t>(w, 0x92)) return;
    const uint32_t b = F<uint32_t>(w, 0xb), f = F<uint32_t>(w, 0xf);
    F<uint32_t>(w, 0xf8) = b;
    F<int32_t>(w, 0xf4) = 1;
    F<uint32_t>(w, 0xfc) = f;
    ((void(__fastcall*)(uint8_t*))(*(void***)w)[0x34 / 4])(w);
}
RT3_REPLACE(0x5594c0, Widget_QueueAction);

// 0x559520. Shows the w x h 32-bit picture `pixels` in widget g (style 8):
// a memory bitmap (+0xaa, size +0xa2 / +0xa6) is created on first use;
// the picture is copied in and uploaded.
void __fastcall Widget_SetPicture(uint8_t* g, int, const void* pixels, int32_t w, int32_t h) {
    F<int32_t>(g, 0xb0) = 8;
    uint32_t* dst;
    if (!F<uint8_t*>(g, 0xaa)) {
        F<int32_t>(g, 0xa2) = w;
        F<int32_t>(g, 0xa6) = h;
        uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
        uint8_t* t = p ? Texture_InitMemory(p, 0, nullptr, w, h, 1) : nullptr;
        F<uint8_t*>(g, 0xaa) = t;
        dst = Texture_Pixels32(t, 0, nullptr, nullptr, 0);
    } else
        dst = Texture_Pixels32(F<uint8_t*>(g, 0xaa), 0, nullptr, nullptr, 0);
    memcpy(dst, pixels, (size_t)(w * h * 4));
    Texture_SetPixels(F<uint8_t*>(g, 0xaa), 0, dst);
}
RT3_REPLACE(0x559520, Widget_SetPicture);

// 0x5595d0 (vtable). Draws panel w when visible by its style (+0xb0): 1 / 4
// / 5 filled (grey, black, colour +0xb8); 2 / 7 / 9 / 10 framed (2 and 9
// filled first); 3 / 6 a bevel +0xc4 thick (6 filled with +0xb8) with lit
// top / right and shaded left / bottom edges and corner triangles; 8 a
// texture (+0xaa; QUIRK: its far corner ignores the parent offset); else
// the draw callback +0x10d or its glyph (+0x92 frame +0xae, darkened when
// disabled or hovered, at +0xec / +0xf0 scaled +0x100 / +0x104, alpha +0x10c,
// clipped to +0xd8..+0xe4 with style 0x40000). Then the debug frames.
void __fastcall Panel_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    const int32_t style = F<int32_t>(w, 0xb0);
    uint8_t* p = F<uint8_t*>(w, 0x86);
    auto boxX = [&]() { return (float)((double)F<float>(p, 5) + F<float>(w, 0xb)); };
    auto boxY = [&]() { return (float)((double)F<float>(p, 9) + F<float>(w, 0xf)); };
    if (style == 1 || style == 4 || style == 5) {
        const uint32_t color = style == 1 ? 0x80303030 : style == 4 ? 0xff000000 : F<uint32_t>(w, 0xb8);
        const float y = boxY();
        Ui_FillRect(color, 0, boxX(), y, F<float>(w, 0x13), F<float>(w, 0x17), 3);
    } else if (style == 2 || style == 7 || style == 9 || style == 10) {
        if (style == 2 || style == 9) {
            const float y = boxY();
            Ui_FillRect(style == 9 ? 0x50fff799 : 0x80303030, 0, boxX(), y, F<float>(w, 0x13), F<float>(w, 0x17), 3);
        }
        const uint32_t edge = style == 9 || style == 10 ? 0xffbfc2b2 : 0xff80a080;
        const float y = boxY();
        Ui_FrameRect(edge, 0, boxX(), y, F<float>(w, 0x13), F<float>(w, 0x17), 2.0f, 3);
    } else if (style == 6 || style == 3) {
        const float x0 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5));
        const float y0 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9));
        const float x2 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5) + F<float>(w, 0x13));
        const float y2 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9) + F<float>(w, 0x17));
        const double t = (double)F<int32_t>(w, 0xc4);
        const float bx = (float)(D(0x5ce8d0) * t);
        const float by = (float)(t * D(0x5d0770));
        const uint32_t fill = style == 6 ? F<uint32_t>(w, 0xb8) : 0x40000000;
        const float by2 = (float)((double)by + by);
        const float yi = (float)((double)by + y0);
        const float xi = (float)((double)bx + x0);
        const float ih = (float)((double)F<float>(w, 0x17) - by2);
        Ui_FillRect(fill, 0, xi, yi, (float)((double)F<float>(w, 0x13) - ((double)bx + bx)), ih, 3);
        Ui_FillRect(0x70000000, 0, x0, y0, F<float>(w, 0x13), by, 3);
        Ui_FillRect(0x90000000, 0, x0, yi, bx, (float)((double)F<float>(w, 0x17) - by2), 3);
        const float yb = (float)((double)y2 - by);
        Ui_FillRect(0x30ffffff, 0, x0, yb, F<float>(w, 0x13), by, 3);
        const float xr = (float)((double)x2 - bx);
        Ui_FillRect(0x50ffffff, 0, xr, yi, bx, (float)((double)F<float>(w, 0x17) - by2), 3);
        Fx_QueueFadedQuad(0x50ffffff, 0x50ffffff, Bits(x2), Bits(y0), Bits(xr), Bits(yi), Bits(x2), Bits(yi), 0x50ffffff, 0, 0, 0, 0x3f800000, 0x3f800000);
        Fx_QueueFadedQuad(0x30ffffff, 0x30ffffff, Bits(xr), Bits(yb), Bits(x2), Bits(yb), Bits(x2), Bits(y2), 0x30ffffff, 0, 0, 0, 0x3f800000, 0x3f800000);
        Fx_QueueFadedQuad(0x90000000, 0x90000000, Bits(x0), Bits(yb), Bits(xi), Bits(yb), Bits(x0), Bits(y2), 0x90000000, 0, 0, 0, 0x3f800000, 0x3f800000);
        Fx_QueueFadedQuad(0x90000000, 0x90000000, Bits(x0), Bits(y0), Bits(x0), Bits(yi), Bits(xi), Bits(yi), 0x90000000, 0, 0, 0, 0x3f800000, 0x3f800000);
    } else if (style == 8) {
        const uint32_t tex = F<uint32_t>(w, 0xaa);
        const float fade = ((float(__cdecl*)())(void*)&Fade_Step)();
        const float y1 = (float)((double)F<float>(w, 0x17) + F<float>(w, 0xf));
        const float x1 = (float)((double)F<float>(w, 0x13) + F<float>(w, 0xb));
        const float y = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
        const float x = (float)((double)F<float>(p, 5) + F<float>(w, 0xb));
        Engine_QueueSprite(hook::As<void*>(g_engine), 0, Bits(x), Bits(y), Bits(x1), Bits(y1), Bits(fade), 0xffffffff, tex, 0, 0, 0x3f800000, 0x3f800000,
                           0x3f800000);
    } else if (auto cb = F<void(__fastcall*)(uint32_t, uint32_t, float, float, float, float, int32_t)>(w, 0x10d)) {
        const float y = boxY();
        cb(F<uint16_t>(w, 4), F<uint32_t>(w, 0x82), boxX(), y, F<float>(w, 0x13), F<float>(w, 0x17), 0);
    } else if (F<uint8_t*>(w, 0x92)) {
        float tint = F<float>(w, 0x108);
        const uint32_t fl = F<uint32_t>(w, 0x33);
        if ((fl & 0x40) || ((fl & 1) && F<uint8_t*>(g_windowManager, 0xc6d) == w)) tint = (float)((double)tint * D(0x5c87e8));
        const int32_t clip = fl & 0x40000;
        if (clip && (!(F<float>(w, 0xdc) < F<float>(w, 0xe4)) || !(F<float>(w, 0xd8) < F<float>(w, 0xe0)))) return;
        const float bottom = (float)((double)F<float>(w, 0xe4) + F<float>(p, 9));
        const uint32_t color = ((uint32_t)w[0x10c] << 24) | 0xffffff;
        const float right = (float)((double)F<float>(w, 0xe0) + F<float>(p, 5));
        const float top = (float)((double)F<float>(w, 0xdc) + F<float>(p, 9));
        const float left = (float)((double)F<float>(w, 0xd8) + F<float>(p, 5));
        const float gy = (float)((double)F<float>(w, 0xf0) + F<float>(w, 0xf) + F<float>(p, 9));
        const float gx = (float)((double)F<float>(w, 0xec) + F<float>(w, 0xb) + F<float>(p, 5));
        Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int16_t>(w, 0xae)), 0, gx, gy, Bits(tint), F<float>(w, 0x100), F<float>(w, 0x104),
                    color, 0, 1, clip, left, top, right, bottom);
    }
    if (!(F<uint32_t>(g_windowManager, 0xc79) & F<uint16_t>(w, 6))) return;
    p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
    if (!(F<uint32_t>(w, 0x33) & 0x40000)) return;
    p = F<uint8_t*>(w, 0x86);
    const float ch = (float)((double)F<float>(w, 0xe4) - F<float>(w, 0xdc));
    const float cw = (float)((double)F<float>(w, 0xe0) - F<float>(w, 0xd8));
    const float cy = (float)((double)F<float>(p, 9) + F<float>(w, 0xdc));
    const float cx = (float)((double)F<float>(p, 5) + F<float>(w, 0xd8));
    Ui_FrameRect(0xffff0000, 0, cx, cy, cw, ch, 1.0f, 3);
}
RT3_REPLACE(0x5595d0, Panel_Draw);

// 0x559c40 (vtable). Icon button w: hotkeys (+0xbc..+0xc2) click it like a
// push button; mouse clicks inside run the click callback (+0x9a) and advance
// the tutorial (0x622b34); event 7 polls slot 0x30; 0x6c sets the state
// (+0xae, slot 0x34 on change), 0xa8 / 0xa9 / 0xb8 / 0x7a set +0xb8 / +0x108
// / +0x10c / +0x10d. Then the widget default.
int32_t __fastcall IconButton_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    const bool mine = msg[1] == F<uint16_t>(w, 4);
    switch (msg[0]) {
    case 3: {
        if (!WidgetSlot453(w, 0x30) || (*(const uint8_t*)&msg[8] & 4)) break;
        const int32_t key = (int32_t)msg[2];
        const int16_t k1 = F<int16_t>(w, 0xbc), k2 = F<int16_t>(w, 0xbe), k3 = F<int16_t>(w, 0xc0), k4 = F<int16_t>(w, 0xc2);
        if (!((k1 && k1 == key) || (k2 && k2 == key) || (k3 && k3 == key) || (k4 && k4 == key))) return 1;
        if (w[0x2d]) {
            const float pan = (float)((double(__fastcall*)(uint8_t*))(void*)&Widget_Pan)(w);
            Ui_PlaySound(w[0x2d], 0, Bits(pan));
        }
        msg[0] = 0xca;
        msg[1] = F<uint16_t>(w, 4);
        return 1;
    }
    case 6:
        if (!WidgetSlot453(w, 0x30) || !Widget_IsEnabled(w) || !WidgetHit453(w, msg[9], msg[10])) break;
        if (F<void*>(w, 0x9a)) WidgetClickCb453(w, msg);
        if (g_6d3b4c && F<uint16_t>(w, 4) == g_622b34) Tutorial_Advance();
        break;
    case 7:
        WidgetSlot453(w, 0x30);
        break;
    case 0x6c:
        if (mine && F<int16_t>(w, 0xae) != (int32_t)msg[2]) {
            F<uint16_t>(w, 0xae) = (uint16_t)msg[2];
            WidgetSlot453(w, 0x34);
        }
        break;
    case 0xa8:
        if (mine) F<uint32_t>(w, 0xb8) = msg[2];
        break;
    case 0xa9:
        if (mine) F<uint32_t>(w, 0x108) = msg[6];
        break;
    case 0xb8:
        if (mine) w[0x10c] = (uint8_t)msg[2];
        break;
    case 0x7a:
        if (mine) F<uint32_t>(w, 0x10d) = msg[2];
        break;
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)msg);
}
RT3_REPLACE(0x559c40, IconButton_HandleEvent);

RT3_NOINLINE void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    F<uint32_t>(w, 0x33) |= 0x40000;
    F<uint32_t>(w, 0xd8) = a;
    F<uint32_t>(w, 0xdc) = b;
    F<uint32_t>(w, 0xe0) = c;
    F<uint32_t>(w, 0xe4) = d;
}
RT3_REPLACE(0x559f50, Widget_SetClip);

// 0x559f90 / 0x55a040. Scrollbar (vtable 0x5ddd18): fields +0xae..+0xc7.
uint8_t* __fastcall Scrollbar_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5ddd18;
    Scrollbar_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0x89);
    memcpy(w + 0xae, rec + 0x5f, 0x1a);
    Free(rec);
    return w;
}
RT3_REPLACE(0x559f90, Scrollbar_Load);

RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5ddd18;
    Scrollbar_Init(w);
    memcpy(w + 0xae, def + 0x5f, 0x2a);
    F<uint16_t>(w, 6) = 4;
    return w;
}
RT3_REPLACE(0x55a040, Scrollbar_Construct);

void __fastcall Thunk55a080(void* p0) { Nop558750(p0); }
RT3_REPLACE(0x55a080, Thunk55a080);

// 0x55a090. Scrollbar: its texture +0xaa.
uint8_t* __fastcall Scrollbar_Delete(uint8_t* w, int, int32_t flags) {
    F<uint32_t>(w, 0) = 0x5ddd18;
    if (uint8_t* t = F<uint8_t*>(w, 0xaa)) {
        Texture_Destroy(t);
        ExeDelete(t);
        F<uint8_t*>(w, 0xaa) = nullptr;
    }
    return Widget_Finish(w, flags);
}
RT3_REPLACE(0x55a090, Scrollbar_Delete);

// 0x55a0e0. List box destructor body: its two scrollbars (+0x1ad / +0x1b1; handle
// +0x7c released) are deleted from the window, then the name +0x47 and the base.
void __fastcall ListBox_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5ddd58;
    for (uint32_t off : { 0x1adu, 0x1b1u }) {
        uint8_t* s = F<uint8_t*>(w, off);
        if (!s) continue;
        HandleTable_Release(g_windowManager, 0, F<uint16_t>(s, 0x7c));
        Window_DeleteChild(F<uint8_t*>(w, 0x86), 0, F<uint8_t*>(w, off));
    }
    F<uint8_t*>(w, 0x1ad) = nullptr;
    F<uint8_t*>(w, 0x1b1) = nullptr;
    String_Free(&F<char*>(w, 0x47));
    Widget_Destruct(w);
}
RT3_REPLACE(0x55a0e0, ListBox_Destruct);

// 0x55a170. Lays out a slider (image table +0x92): the thumb size (+0x191 /
// +0x195) from its image +0x139; unless arrow-less (flag 8 in +0x33) the
// slider's thickness (+0x13 or +0x17, vertical when +0x109 is clear) from the
// larger of the track (+0x13d) and first arrow (+0x129) images and its length
// from the second arrow (+0x131) plus its offset (+0x169 / +0x16d); the free
// track length +0x199. With +0x119 an arrow-less slider drops the arrow
// offsets. Otherwise the two arrow buttons are created (ids +0x1a9 / +0x1ab,
// controls +0x1ad / +0x1b1) at the offsets +0x161 / +0x165 and +0x169 /
// +0x16d, with the slider's images, owner, colours (+0x121 / +0x125, default
// 6 / 7) and, with +0x111, the arrow styles 0x25..0x28. Step +0x11d defaults
// to 31.
void __fastcall Slider_Layout(uint8_t* s) {
    auto img = [&](uint32_t off) { return (uint8_t*)Table_Entry(F<uint8_t*>(s, 0x92), 0, F<int32_t>(s, off)); };
    F<float>(s, 0x191) = (float)Slider_StepA(img(0x139));
    F<float>(s, 0x195) = (float)Slider_StepB(img(0x139));
    if (!(s[0x33] & 8)) {
        float wA = (float)Slider_StepA(img(0x13d));
        float hA = (float)Slider_StepB(img(0x13d));
        float wB = (float)Slider_StepA(img(0x129));
        float hB = (float)Slider_StepB(img(0x129));
        float wC = (float)Slider_StepA(img(0x131));
        double hC = Slider_StepB(img(0x131));
        if (F<int32_t>(s, 0x109)) {
            F<float>(s, 0x17) = hA > hB ? hA : hB;
            F<float>(s, 0x13) = (float)((double)wC + F<float>(s, 0x169));
        } else {
            F<float>(s, 0x13) = wA > wB ? wA : wB;
            F<float>(s, 0x17) = (float)(hC + F<float>(s, 0x16d));
        }
    }
    if (F<int32_t>(s, 0x109))
        F<float>(s, 0x199) = (float)((double)F<float>(s, 0x155) - F<float>(s, 0x151) - F<float>(s, 0x191));
    else
        F<float>(s, 0x199) = (float)((double)F<float>(s, 0x149) - F<float>(s, 0x145) - F<float>(s, 0x195));
    bool arrows = !(s[0x33] & 8);
    if (F<int32_t>(s, 0x119) && !arrows) {
        F<uint32_t>(s, 0x159) = 0;
        F<uint32_t>(s, 0x15d) = 0;
        F<uint32_t>(s, 0x169) = 0;
        F<uint32_t>(s, 0x16d) = F<uint32_t>(s, 0x17);
        F<float>(s, 0x199) = (float)((double)F<float>(s, 0x149) - F<float>(s, 0x145) - F<float>(s, 0x195));
    }
    if (arrows) {
        uint8_t d[0xa5];
        memset(d, 0, sizeof d);
        F<uint16_t>(d, 5) = F<uint16_t>(s, 9);
        d[4] = (uint8_t)(s[8] + 1);
        String_Assign(&F<char*>(d, 0x43), F<const char*>(s, 0x47));
        F<uint8_t*>(d, 0x47) = s;
        F<float>(d, 7) = (float)((double)F<float>(s, 0x161) + F<float>(s, 0xb));
        F<uint16_t>(d, 0x5f) = F<uint16_t>(s, 0x129);
        F<uint16_t>(d, 0x61) = F<uint16_t>(s, 0x12d);
        F<float>(d, 0xb) = (float)((double)F<float>(s, 0x165) + F<float>(s, 0xf));
        uint16_t id = (uint16_t)HandleTable_Acquire(g_windowManager, 0, 0);
        F<uint16_t>(d, 0) = id;
        d[0x89] = F<int32_t>(s, 0x121) ? (uint8_t)F<int32_t>(s, 0x121) : 6;
        d[0x8a] = F<int32_t>(s, 0x125) ? (uint8_t)F<int32_t>(s, 0x125) : 7;
        if (F<int32_t>(s, 0x111)) F<uint16_t>(d, 0x67) = F<int32_t>(s, 0x109) ? 0x25 : 0x26;
        F<uint16_t>(s, 0x1a9) = id;
        uint8_t* c = (uint8_t*)ExeNew(0xe9);
        c = c ? Control_Construct(c, 0, d) : nullptr;
        F<uint8_t*>(s, 0x1ad) = c;
        Window_AddChild(F<uint8_t*>(s, 0x86), 0, c, 0);
        F<float>(d, 7) = (float)((double)F<float>(s, 0x169) + F<float>(s, 0xb));
        F<uint16_t>(d, 0x61) = F<uint16_t>(s, 0x135);
        F<uint16_t>(d, 0x5f) = F<uint16_t>(s, 0x131);
        F<float>(d, 0xb) = (float)((double)F<float>(s, 0x16d) + F<float>(s, 0xf));
        id = (uint16_t)HandleTable_Acquire(g_windowManager, 0, 0);
        F<uint16_t>(d, 0) = id;
        if (F<const char*>(s, 0x43)) String_Assign(&F<char*>(d, 0x3f), F<const char*>(s, 0x43));
        F<uint8_t*>(d, 0x47) = s;
        if (F<int32_t>(s, 0x111)) F<uint16_t>(d, 0x67) = F<int32_t>(s, 0x109) ? 0x27 : 0x28;
        F<uint16_t>(s, 0x1ab) = id;
        c = (uint8_t*)ExeNew(0xe9);
        c = c ? Control_Construct(c, 0, d) : nullptr;
        F<uint8_t*>(s, 0x1b1) = c;
        Window_AddChild(F<uint8_t*>(s, 0x86), 0, c, 0);
        Free(F<void*>(d, 0x43));
        if (F<const char*>(s, 0x43)) Free(F<void*>(d, 0x3f));
    }
    if (!F<int32_t>(s, 0x11d)) F<int32_t>(s, 0x11d) = 0x1f;
}
RT3_REPLACE(0x55a170, Slider_Layout);

// 0x55a560. List box defaults (type 0x40).
RT3_NOINLINE void __fastcall ListBox_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0x101, 0x105, 0x109, 0x10d, 0x111, 0x185, 0x141, 0x145, 0x149, 0x14d, 0x151, 0x155,
                                     0x189, 0x18d, 0x191, 0x195, 0x199, 0x159, 0x15d, 0x161, 0x165, 0x169, 0x16d, 0x115,
                                     0x119, 0x19d, 0x1a1, 0x1a5, 0x1ad, 0x1b1, 0x121, 0x125, 0x11d, 0x1b5 };
    F<uint16_t>(w, 6) = 0x40;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
}
RT3_REPLACE(0x55a560, ListBox_Init);

RT3_NOINLINE void __fastcall Scrollbar_SetPos(uint8_t* w, int, uint32_t v) {
    if (v != F<uint32_t>(w, 0x101)) F<uint32_t>(w, 0x101) = v;
}
RT3_REPLACE(0x55a640, Scrollbar_SetPos);

// 0x55a660. Drop-down d: selects entry `idx` (clamped to the +0x101 entries;
// current +0x185). Re-selecting only calls the change callback +0x1b5
// (fastcall d, index) when `renotify`. Otherwise the open list +0x4b (kind
// 8, same id) follows, the callback runs and, unless muted (+0x115), `sound`
// plays its click (+0x11d).
RT3_NOINLINE void __fastcall Dropdown_Select(uint8_t* d, int, int32_t idx, int32_t sound, int32_t renotify) {
    typedef void(__fastcall * Changed)(uint8_t*, int32_t);
    if (idx < 0) idx = 0;
    if (idx >= F<int32_t>(d, 0x101)) idx = F<int32_t>(d, 0x101) - 1;
    if (idx == F<int32_t>(d, 0x185)) {
        if (renotify)
            if (Changed cb = F<Changed>(d, 0x1b5)) cb(d, F<int32_t>(d, 0x185));
        return;
    }
    int32_t click = F<int32_t>(d, 0x115) ? 0 : sound;
    uint8_t* list = F<uint8_t*>(d, 0x4b);
    F<int32_t>(d, 0x185) = idx;
    if (list && F<uint16_t>(list, 6) == 8 && F<uint16_t>(list, 0x1b0) == F<uint16_t>(d, 4))
        ListBox_Select(list, 0, TreeView_NodeOfRow(list, 0, idx), 0);
    if (Changed cb = F<Changed>(d, 0x1b5)) cb(d, F<int32_t>(d, 0x185));
    if (click) Widget_PlaySound(d, F<int32_t>(d, 0x11d));
}
RT3_REPLACE(0x55a660, Dropdown_Select);

// 0x55a720 (vtable). Draws slider w when visible: its track glyph (+0x13d,
// unless +0x119) at +0x159 / +0x15d, then places the thumb (+0x189 /
// +0x18d) along the track by the position +0x185 of +0x101 steps over the
// length +0x199 (vertical with +0x109, reversed with +0x10d; centred for a
// single step) and draws it (+0x139); with the debug mask the frames.
void __fastcall Slider_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float x = (float)((double)F<float>(p, 5) + F<float>(w, 0xb));
    const float y = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
    if (!F<uint32_t>(w, 0x119)) {
        const float gy = (float)((double)y + F<float>(w, 0x15d));
        const float gx = (float)((double)x + F<float>(w, 0x159));
        Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int32_t>(w, 0x13d)), 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    const int32_t steps = F<int32_t>(w, 0x101);
    if (F<uint32_t>(w, 0x109)) {
        F<float>(w, 0x18d) = (float)((double)F<float>(w, 0x15d) + F<float>(w, 0x14d));
        if (steps > 1)
            F<float>(w, 0x189) = (float)((double)F<int32_t>(w, 0x185) * F<float>(w, 0x199) / (steps - 1) + F<float>(w, 0x151));
        else
            F<float>(w, 0x189) = (float)((double)F<float>(w, 0x199) * Fl(0x5c8d48) + F<float>(w, 0x151));
    } else {
        F<float>(w, 0x189) = (float)((double)F<float>(w, 0x159) + F<float>(w, 0x141));
        double v;
        if (steps > 1)
            v = (double)(F<uint32_t>(w, 0x10d) ? steps - F<int32_t>(w, 0x185) : F<int32_t>(w, 0x185)) * F<float>(w, 0x199) / (steps - 1);
        else
            v = (double)F<float>(w, 0x199) * Fl(0x5c8d48);
        F<float>(w, 0x18d) = (float)(v + F<float>(w, 0x145));
    }
    const float ty = (float)((double)y + F<float>(w, 0x18d));
    const float tx = (float)((double)x + F<float>(w, 0x189));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, F<int32_t>(w, 0x139)), 0, tx, ty, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (!(F<uint32_t>(g_windowManager, 0xc79) & F<uint16_t>(w, 6))) return;
    p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
}
RT3_REPLACE(0x55a720, Slider_Draw);

// 0x55a990 / 0x55ab50. List box (vtable 0x5ddd58): fields +0x101.. (the loader
// runs the init twice).
uint8_t* __fastcall ListBox_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5ddd58;
    ListBox_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0xe3);
    ListBox_Init(w);
    memcpy(w + 0x101, rec + 0x5f, 0x70);
    Free(rec);
    return w;
}
RT3_REPLACE(0x55a990, ListBox_Load);

// 0x55ab30. Deleting destructor of the list box.
uint8_t* __fastcall ListBox_DeletingDestructor(uint8_t* w, int, uint32_t flags) {
    ListBox_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}
RT3_REPLACE(0x55ab30, ListBox_DeletingDestructor);

RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5ddd58;
    ListBox_Init(w);
    memcpy(w + 0x101, def + 0x5f, 0x84);
    F<uint16_t>(w, 6) = 0x40;
    return w;
}
RT3_REPLACE(0x55ab50, ListBox_Construct);

void __fastcall Thunk55ab90(void* p0) { Nop558750(p0); }
RT3_REPLACE(0x55ab90, Thunk55ab90);

// 0x55aba0. Scroll bar sb (horizontal +0x109, else vertical, reversed +0x10d;
// count +0x101, position +0x185, page step +0x105) mouse handler, in
// coordinates relative to the bar: a press (6) on the track (+0x159 / +0x15d
// up to +0x169 / +0x16d, or anywhere with flag 8) grabs the thumb (at +0x189 /
// +0x18d, length +0x191 / +0x195; +0x1a5, advancing a tutorial waiting for a
// horizontal one) or pages by the step (jumps there with flag 0x100000 or no
// step); a move (2) drags the grabbed thumb; a release (7) ends any drag
// (+0x19d..+0x1a5). Positions map as (p - start - thumb / 2) * count / length
// (0x5c8d48 the half, +0x151 / +0x145 the start, +0x199 the length).
// Returns 0 when handled, 1 otherwise.
RT3_NOINLINE int32_t __fastcall Scrollbar_HandleMouse(uint8_t* sb, int, uint32_t* ev) {
    uint8_t* p = F<uint8_t*>(sb, 0x86);
    float lx = (float)(*(float*)&ev[9] - ((double)F<float>(p, 5) + F<float>(sb, 0xb)));
    float ly = (float)(*(float*)&ev[10] - ((double)F<float>(p, 9) + F<float>(sb, 0xf)));
    auto setPos = [&](int32_t v) { Dropdown_Select(sb, 0, v, 1, 0); };
    // The position at coordinate c of the track starting at `start` with thumb `len`.
    auto at = [&](float c, uint32_t start, uint32_t len, int32_t n) {
        return Ftol((((double)c - F<float>(sb, start)) - (double)F<float>(sb, len) * Fl(0x5c8d48)) * n / F<float>(sb, 0x199));
    };
    auto atReversed = [&](float c, int32_t n) {
        double nd = (double)n;
        return Ftol(nd - (((double)ly - F<float>(sb, 0x145)) - (double)F<float>(sb, 0x195) * Fl(0x5c8d48)) * nd / F<float>(sb, 0x199));
    };
    switch (ev[0]) {
    case 7:
        if (!F<int32_t>(sb, 0x19d) && !F<int32_t>(sb, 0x1a1) && !F<int32_t>(sb, 0x1a5)) return 1;
        F<int32_t>(sb, 0x1a5) = 0;
        F<int32_t>(sb, 0x1a1) = 0;
        F<int32_t>(sb, 0x19d) = 0;
        return 0;
    case 6: {
        bool horz = F<int32_t>(sb, 0x109) != 0;
        float c = horz ? lx : ly;
        uint32_t lo = horz ? 0x159 : 0x15d, hi = horz ? 0x169 : 0x16d, thumb = horz ? 0x189 : 0x18d, len = horz ? 0x191 : 0x195,
                 start = horz ? 0x151 : 0x145;
        if (c < F<float>(sb, lo)) return 1;
        if (!(c < F<float>(sb, hi) || c != c) && !(sb[0x33] & 8)) return 1;
        int32_t n = F<int32_t>(sb, 0x101);
        if (n <= 1) return 0;
        bool inv = !horz && F<int32_t>(sb, 0x10d);
        if (c >= F<float>(sb, thumb) && (double)F<float>(sb, len) + F<float>(sb, thumb) > c) {
            F<int32_t>(sb, 0x1a5) = 1;
            if (horz && g_6d3b4c && (int32_t)F<uint16_t>(sb, 4) == hook::As<int32_t>(g_622b34)) Tutorial_Advance();
            setPos(inv ? atReversed(c, n) : at(c, start, len, n));
            return 0;
        }
        int32_t step = F<int32_t>(sb, 0x105);
        if (step && !(F<uint32_t>(sb, 0x33) & 0x100000)) {
            int32_t pos = F<int32_t>(sb, 0x185);
            if (c < F<float>(sb, thumb)) setPos(inv ? step - pos + n : pos - step);
            else setPos(inv ? n - pos - step : pos + step);
            return 0;
        }
        setPos(inv ? atReversed(c, n) : at(c, start, len, n));
        return 0;
    }
    case 2: {
        if (!F<int32_t>(sb, 0x1a5)) return 1;
        int32_t n = F<int32_t>(sb, 0x101);
        int32_t v;
        if (F<int32_t>(sb, 0x109)) v = at(lx, 0x151, 0x191, n);
        else if (F<int32_t>(sb, 0x10d)) v = atReversed(ly, n);
        else v = at(ly, 0x145, 0x195, n);
        if (v < 0) v = 0;
        if (v >= n) v = n - 1;
        if (F<int32_t>(sb, 0x185) != v) setPos(v);
        return 0;
    }
    }
    return 1;
}
RT3_REPLACE(0x55aba0, Scrollbar_HandleMouse);

// 0x55b010. Scroll bar sb event handler: mouse events (2 while dragging, 6
// inside, 7) go to Scrollbar_HandleMouse when visible; 0x66 / 0x67 (in
// range) / 0x6a select a position (0x6a quietly); 0x73 sets the count
// (keeping the position from the end); 0x84 sets +0x1b5; 0xcb is a click of
// its up / down button (+0x1a9 / +0x1ab with tags +0x1ad / +0x1b1) moving one
// step (with 0x6d3ffc held up). Everything else (and the setters) also go to
// Widget_HandleEvent.
int32_t __fastcall Scrollbar_HandleEvent(uint8_t* sb, int, uint32_t* ev) {
    auto visible = [&] { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)sb)[0xc])(sb); };
    switch (ev[0]) {
    case 2:
        if (!F<int32_t>(sb, 0x1a5) || !visible()) break;
        return Scrollbar_HandleMouse(sb, 0, ev);
    case 6:
        if (!visible()) break;
        if (!((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t))(*(void***)sb)[8])(sb, 0, ev[9], ev[10], 1)) return 1;
        return Scrollbar_HandleMouse(sb, 0, ev);
    case 7:
        if (!visible()) break;
        return Scrollbar_HandleMouse(sb, 0, ev);
    case 0x67:
        if ((int32_t)ev[2] >= F<int32_t>(sb, 0x101)) break;
        [[fallthrough]];
    case 0x66:
    case 0x6a:
        if (ev[1] != F<uint16_t>(sb, 4)) break;
        Dropdown_Select(sb, 0, (int32_t)ev[2], 0, ev[0] == 0x6a ? 1 : 0);
        break;
    case 0x73: {
        if (ev[1] != F<uint16_t>(sb, 4)) break;
        int32_t n = (int32_t)ev[2];
        int32_t pos = n - F<int32_t>(sb, 0x185);
        F<int32_t>(sb, 0x101) = n;
        if (pos >= n - 1) pos = n - 1;
        F<int32_t>(sb, 0x185) = pos;
        break;
    }
    case 0x84:
        if (ev[1] != F<uint16_t>(sb, 4)) break;
        F<uint32_t>(sb, 0x1b5) = ev[2];
        break;
    case 0xcb: {
        int32_t n = F<int32_t>(sb, 0x101);
        if (!n) break;
        int32_t pos = F<int32_t>(sb, 0x185), c;
        if (ev[1] == F<uint16_t>(sb, 0x1a9) && ev[0xb] == F<uint32_t>(sb, 0x1ad)) c = pos - 1;
        else if (ev[1] == F<uint16_t>(sb, 0x1ab) && ev[0xb] == F<uint32_t>(sb, 0x1b1)) c = pos + 1;
        else break;
        int32_t v;
        if (F<int32_t>(sb, 0x10d)) {
            int32_t m = n - c < n - 1 ? n - c : n - 1;
            v = n - m;
        } else
            v = c < n - 1 ? c : n - 1;
        if (v <= 0) v = 0;
        if (v != pos) {
            ++g_6d3ffc;
            Dropdown_Select(sb, 0, v, 1, 0);
            --g_6d3ffc;
        }
        return 0;
    }
    }
    return Widget_HandleEvent(sb, 0, (uint8_t*)ev);
}
RT3_REPLACE(0x55b010, Scrollbar_HandleEvent);

// 0x55b2d0. Edit box defaults (type 2).
RT3_NOINLINE void __fastcall EditBox_Init(uint8_t* w) {
    static const uint8_t zero[] = { 0xba, 0xa2, 0xa6, 0xaa, 0xae, 0xb2, 0xb6, 0xe4, 0xd8, 0xdc, 0xe0, 0xe8, 0xd4 };
    F<uint16_t>(w, 6) = 2;
    for (uint8_t off : zero) F<int32_t>(w, off) = 0;
    F<uint16_t>(w, 0xbe) = 0;
}
RT3_REPLACE(0x55b2d0, EditBox_Init);

// 0x55b330. Lays out text widget w: its line count +0xe0 (height / line
// height, plus one when the rest still fits a line; at least 1), the
// language fixes (0x6d3c84; widget ids / string ids +0xbe / dialog +9: box
// sizes, the small font 2 or 1) and its string (+0xba from +0xbe); timed
// texts (+0xa2) get 500 ms unless their delay +0xaa is within 1..3999.
void __fastcall TextWidget_Layout(uint8_t* w) {
    F<int32_t>(w, 0xe0) = Ftol(F<float>(w, 0x17) / Scroll_Total(F<uint8_t*>(w, 0x63)));
    if (F<int32_t>(w, 0xe0) >= 1) {
        const float rest = (float)(F<float>(w, 0x17) - Scroll_Total(F<uint8_t*>(w, 0x63)) * F<int32_t>(w, 0xe0));
        if (Scroll_Total(F<uint8_t*>(w, 0x63)) <= rest) ++F<int32_t>(w, 0xe0);
    }
    if (F<int32_t>(w, 0xe0) < 1) F<int32_t>(w, 0xe0) = 1;
    if (F<uint16_t>(w, 0x40) == 0xf46 && !F<uint16_t>(w, 0xbe) && F<uint16_t>(w, 9) == 0x7539) F<uint16_t>(w, 0xbe) = 0x746;
    auto lang = []() { return g_language; };
    const uint16_t id = F<uint16_t>(w, 4);
    if ((lang() == 3 || lang() == 4) && (F<uint16_t>(w, 0xbe) == 0x7fe || F<uint16_t>(w, 0xbe) == 0x800)) F<uint32_t>(w, 0x13) = 0x3de66666;
    if ((lang() == 3 || lang() == 4) && (id == 0x986e || id == 0x986d)) {
        F<uint32_t>(w, 0xb) = 0x3f511eb8;
        F<uint32_t>(w, 0x13) = 0x3df9999a;
    }
    if (lang() == 1) {
        bool done = false;
        if (w[0x37] == 3) {
            const uint16_t s = F<uint16_t>(w, 0xbe);
            if (id == 0x986d || id == 0x986e || id == 0x9870 || id == 0x9871 || s == 0x804 || s == 0x802 || s == 0x800 || s == 0x7fe) {
                Widget_SetFont(w, 0, 2, 0);
                done = lang() != 1;
            }
        }
        if (!done && id == 0x986e) {
            F<float>(w, 0xb) = (float)((double)F<float>(w, 0xb) + D(0x5d0170));
            done = lang() != 1;
        }
        if (!done) {
            const uint16_t s = F<uint16_t>(w, 0xbe);
            if (s == 0x7e0) {
                F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + D(0x5d0830));
            } else if (s == 0x800) {
                F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + D(0x5d0170));
            }
        }
    }
    if (!(uint8_t)IsEnglish() && F<uint16_t>(w, 0xbe) == 0x4d5 && F<uint16_t>(w, 9) == 0x4e23) {
        F<float>(w, 0xb) = (float)((double)F<float>(w, 0xb) - D(0x5d1160));
        F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + D(0x5d1160));
        F<float>(w, 0xf) = (float)((double)F<float>(w, 0xf) - D(0x5d01e0));
        F<float>(w, 0x17) = (float)((double)F<float>(w, 0x17) + D(0x5caaf0));
        Widget_SetFont(w, 0, 2, 0);
    }
    if (lang() == 4 || lang() == 2) {
        const uint16_t s = F<uint16_t>(w, 0xbe);
        if (s == 0x696 && F<uint16_t>(w, 9) == 0x4e2a) {
            F<float>(w, 0xf) = (float)((double)F<float>(w, 0xf) - D(0x5d0aa8));
            F<float>(w, 0x17) = (float)((double)F<float>(w, 0x17) + D(0x5d0aa8));
        }
        if (lang() == 2) {
            bool done = false;
            if (s == 0x5db) {
                if (F<uint16_t>(w, 9) == 0x4e29) {
                    F<float>(w, 0xb) = (float)((double)F<float>(w, 0xb) - D(0x5d0170));
                    F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + D(0x5d0238));
                    Widget_SetFont(w, 0, 1, 0);
                } else {
                    Widget_SetFont(w, 0, 2, 0);
                }
                done = lang() != 2;
            }
            if (!done) {
                const uint16_t s2 = F<uint16_t>(w, 0xbe);
                if (s2 == 0x66a) {
                    F<float>(w, 0xb) = (float)((double)F<float>(w, 0xb) - D(0x5d0110));
                    F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + D(0x5c9b50));
                } else if (s2 == 0x7f0) {
                    F<float>(w, 0xf) = (float)((double)F<float>(w, 0xf) - D(0x5d00e8));
                    F<float>(w, 0x17) = (float)((double)F<float>(w, 0x17) + D(0x5d0170));
                    F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) - D(0x5d0110));
                }
            }
        }
    }
    if (F<uint16_t>(w, 0xbe)) String_Assign((char**)(w + 0xba), GetString(F<uint16_t>(w, 0xbe)));
    if (F<uint32_t>(w, 0xa2) && (F<int32_t>(w, 0xaa) <= 0 || F<int32_t>(w, 0xaa) >= 0xfa0)) F<int32_t>(w, 0xaa) = 0x1f4;
}
RT3_REPLACE(0x55b330, TextWidget_Layout);

// 0x55b650. Lines the widget's text takes up to the caret (+0xd8), 0 when the
// caret is outside the text.
int32_t __fastcall Widget_LinesToCaret(uint8_t* w) {
    char* text = F<char*>(w, 0xba);
    int32_t len = (int32_t)strlen(text), caret = F<int32_t>(w, 0xd8);
    if (caret < 0 || caret >= len) return 0;
    char saved = text[caret];
    text[caret] = 0;
    int32_t n = Font_CountLines(F<uint8_t*>(w, 0x63), 0, F<const char*>(w, 0xba), F<float>(w, 0x13), nullptr);
    F<char*>(w, 0xba)[F<int32_t>(w, 0xd8)] = saved;
    return n;
}
RT3_REPLACE(0x55b650, Widget_LinesToCaret);

// 0x55b6b0. Deletes the character at pos (a whole [..] tag in tag mode, flag
// 0x4000000) and moves the cursor (+0xd8) there.
RT3_NOINLINE void __fastcall EditBox_DeleteAt(uint8_t* w, int, char* s, int32_t pos) {
    int32_t len = 0;
    while (s[len]) ++len;
    if (pos < 0 || pos >= len) return;
    int32_t from = pos, to = pos;
    if (F<uint32_t>(w, 0x33) & 0x4000000) {
        char c = s[to];
        if (c == ']' && to > 0)
            while (s[from] != '[') {
                if (--from <= 0) break;
            }
        if (c == '[' && to < len - 1)
            while (s[to] != ']') {
                if (++to >= len - 1) break;
            }
    }
    if (to < len - 1) MemMove(s + from, s + to + 1, len - to - 1);
    s[from - to + len - 1] = 0;
    F<int32_t>(w, 0xd8) = from;
}
RT3_REPLACE(0x55b6b0, EditBox_DeleteAt);

// 0x55b750. Moves the text box's cursor (+0xd8) by delta characters in str,
// stepping over DBCS characters (0x6d4034, code page 0x624328) and, with flag
// 0x4000000 (+0x33), over whole [bracketed] tags.
RT3_NOINLINE void __fastcall TextBox_MoveCursor(uint8_t* e, int, const char* str, int32_t delta) {
    int32_t count = delta < 0 ? -delta : delta;
    int32_t step = delta > 0 ? 1 : -1;
    int32_t inTag = 0;
    int32_t len = 0;
    while (str[len]) ++len;
    while (count > 0 || inTag) {
        int32_t pos = F<int32_t>(e, 0xd8);
        int32_t np = pos + step;
        if (np < 0 || np > len) break;
        if ((F<uint32_t>(e, 0x33) & 0x4000000) && step > 0) {
            char ch = str[pos];
            if (ch == '[') inTag = 1;
            if (ch == ']') inTag = 0;
        }
        if (g_6d4034 && (step < 0 ? -step : step) <= 1) {
            const char* cur = str + pos;
            const char* q;
            uint16_t cp = hook::As<uint16_t>(g_624328);
            if (step == 1) q = Import<const char*(__stdcall*)(uint16_t, const char*, uint32_t)>(0x5c8244)(cp, cur, 0);
            else q = Import<const char*(__stdcall*)(uint16_t, const char*, const char*, uint32_t)>(0x5c8254)(cp, str, cur, 0);
            int32_t d = (int32_t)(q - cur);
            if ((d < 0 ? -d : d) <= 2) F<int32_t>(e, 0xd8) += d;
            else F<int32_t>(e, 0xd8) += step;
        } else {
            F<int32_t>(e, 0xd8) = np;
        }
        if ((step < 0 ? -step : step) > 2 && F<int32_t>(e, 0xd8) > 0) {
            int32_t at = F<int32_t>(e, 0xd8);
            if (IsSingleByte((uint16_t)(int16_t)(int8_t)str[at])) F<int32_t>(e, 0xd8) += step > 0 ? 1 : -1;
        }
        if ((F<uint32_t>(e, 0x33) & 0x4000000) && step < 0) {
            char ch = str[F<int32_t>(e, 0xd8)];
            if (ch == '[') inTag = 0;
            if (ch == ']') inTag = 1;
        }
        --count;
    }
}
RT3_REPLACE(0x55b750, TextBox_MoveCursor);

// 0x55b8e0. Lines the widget's text (+0xba) takes in its font (+0x63) at its width (+0x13).
RT3_NOINLINE int32_t __fastcall Widget_TextLines(uint8_t* w) {
    return Font_CountLines(F<uint8_t*>(w, 0x63), 0, F<const char*>(w, 0xba), F<float>(w, 0x13), nullptr);
}
RT3_REPLACE(0x55b8e0, Widget_TextLines);

uint8_t* __fastcall EditBox_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5ddd98;
    EditBox_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0x91);
    memcpy(w + 0xa2, rec + 0x5f, 0x18);
    F<uint16_t>(w, 0xbe) = F<uint16_t>(rec, 0x7b);
    F<uint32_t>(w, 0xec) = 0;
    F<char*>(w, 0xba) = nullptr;
    NameTable_ReadBlock(g_pack, 0, 0xbbb, &F<void*>(w, 0xba));
    if (!F<char*>(w, 0xba)) {
        String_Assign(&F<char*>(w, 0xba), (const char*)0x5d09c4);
    } else {
        char* old = F<char*>(w, 0xba);
        EditBox_DupText(w);
        Free(old);
    }
    Free(rec);
    return w;
}
RT3_REPLACE(0x55b900, EditBox_Load);

// 0x55ba20. Edit box: its text +0xba.
uint8_t* __fastcall EditBox_Delete(uint8_t* w, int, int32_t flags) {
    F<uint32_t>(w, 0) = 0x5ddd98;
    FreeField(w, 0xba);
    return Widget_Finish(w, flags);
}
RT3_REPLACE(0x55ba20, EditBox_Delete);

RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5ddd98;
    EditBox_Init(w);
    memcpy(w + 0xa2, def + 0x5f, 0x32);
    EditBox_DupText(w);
    return w;
}
RT3_REPLACE(0x55ba70, EditBox_Construct);

// 0x55bae0. Scroll button states of a multi-line text widget: *up / *down start
// as the widget's +0x2b / +0x2c; when it scrolls (+0xe8) and its text is taller
// than the box (+0x17), down turns 0 or 4 depending on how far the caret line
// (Widget_LinesToCaret) is from the bottom, and an up state of 2 / 0 becomes
// 5 / 6 once the line height (0x5dd6b8 x) no longer fits.
RT3_NOINLINE void __fastcall EditBox_ScrollStates(uint8_t* w, int, int32_t* up, int32_t* down) {
    *up = w[0x2b];
    *down = w[0x2c];
    if (F<int32_t>(w, 0xe8) <= 0) return;
    double hd = Font_TextHeight(F<uint8_t*>(w, 0x63), 0, F<const char*>(w, 0xba), F<float>(w, 0x13), nullptr);
    float h = (float)hd;
    if (!(hd > F<float>(w, 0x17))) return;
    if (Font_CountLines(F<uint8_t*>(w, 0x63), 0, F<const char*>(w, 0xba), F<float>(w, 0x13), nullptr) <= 1) return;
    double box = F<float>(w, 0x17);
    if (!(Scroll_Total(F<uint8_t*>(w, 0x63)) * D(0x5dd6b8) < box)) {
        if (w[0x2b] == 2)
            *up = 5;
        else if (w[0x2b] == 0)
            *up = 6;
        return;
    }
    int32_t n = Widget_LinesToCaret(w);
    float t = (float)(Scroll_Total(F<uint8_t*>(w, 0x63)) * n);
    double caretBottom = Scroll_Total(F<uint8_t*>(w, 0x63)) + t;
    float cb = (float)caretBottom;
    if (!(F<float>(w, 0x17) * D(0x5c9b10) < caretBottom)) return;
    if (!(F<float>(w, 0x17) * D(0x5dd6b8) < h)) {
        *down = 4;
        return;
    }
    if (F<float>(w, 0x17) * D(0x5dddd8) > cb) {
        *down = 0;
        return;
    }
    *down = 4;
}
RT3_REPLACE(0x55bae0, EditBox_ScrollStates);

// 0x55bc30 (vtable). Draws edit box w when visible: its text (+0xba, font
// +0x63) in colour +0x38 (+0x3c when focused, flag 4), the scroll states as
// alignment and the flags 2 / 0x10 / 0x40 for +0xb2 / +0xb6 / style bit
// 0x4000000; with the debug mask (0x6d401c +0xc79) its frame and inner box.
void __fastcall EditBox_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float x = (float)((double)F<float>(p, 5) + F<float>(w, 0xb));
    uint32_t color = F<uint32_t>(w, 0x38);
    const float y = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
    if (F<uint32_t>(w, 0x3c) && (w[0x6a] & 4)) color = F<uint32_t>(w, 0x3c);
    uint32_t flags = F<uint32_t>(w, 0xb2) ? 2 : 0;
    if (F<uint32_t>(w, 0xb6)) flags += 0x10;
    if (F<uint32_t>(w, 0x33) & 0x4000000) flags += 0x40;
    int32_t up, down;
    EditBox_ScrollStates(w, 0, &up, &down);
    if (const char* text = F<const char*>(w, 0xba))
        Font_DrawTextBox(F<uint8_t*>(w, 0x63), 0, text, x, y, F<float>(w, 0x13), F<float>(w, 0x17), color, up, down, flags, 1.0f);
    if (!(F<uint32_t>(g_windowManager, 0xc79) & F<uint16_t>(w, 6))) return;
    p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
}
RT3_REPLACE(0x55bc30, EditBox_Draw);

// 0x55bd80. Rebuilds the display text (+0xba, 5000 bytes) from `text` with the
// cursor character inserted at pos ('|', or 0x0f / ' ' for custom fonts +0xe4).
RT3_NOINLINE void __fastcall EditBox_InsertCursor(uint8_t* w, int, const char* text, int32_t pos) {
    char* buf = (char*)Malloc(0x1388);
    const char* old = F<const char*>(w, 0xba);
    for (size_t k = 0; (buf[k] = old[k]) != 0; ++k) {}
    Free(F<void*>(w, 0xba));
    F<char*>(w, 0xba) = buf;
    if (pos) MemMove(buf, text, pos);
    char cursor;
    if (F<int32_t>(w, 0xe4)) cursor = F<int32_t>(F<uint8_t*>(w, 0x63), 0x10) ? 0x0f : 0x20;
    else cursor = 0x7c;
    F<char*>(w, 0xba)[pos] = cursor;
    const char* rest = text + pos;
    size_t n = 0;
    while (rest[n]) ++n;
    MemMove(F<char*>(w, 0xba) + pos + 1, rest, n + 1);
}
RT3_REPLACE(0x55bd80, EditBox_InsertCursor);

// 0x55be40. Pastes the clipboard text (cut to the box limit +0xaa - 2) into the
// emptied edit text `dst`, cursor after it.
RT3_NOINLINE void __fastcall EditBox_Paste(uint8_t* w, int, char* dst) {
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

// 0x55bf60. Index of the character of text widget w's text (+0xba, font
// +0x63) nearest the point (px, py), laid out in its box (+0xb.. relative to
// the parent +0x86) with the alignment EditBox_ScrollStates gives.
RT3_NOINLINE int32_t __fastcall EditBox_HitTest(uint8_t* w, int, float px, float py) {
    int32_t a, b;
    EditBox_ScrollStates(w, 0, &b, &a);
    uint8_t* parent = F<uint8_t*>(w, 0x86);
    float x = F<float>(parent, 0x5) + F<float>(w, 0xb);
    float y = F<float>(parent, 0x9) + F<float>(w, 0xf);
    return Font_HitTest(F<uint8_t*>(w, 0x63), 0, px, py, F<const char*>(w, 0xba), x, y, F<float>(w, 0x13), F<float>(w, 0x17), 0xffffffff, b, a, 0,
                        1.0f);
}
RT3_REPLACE(0x55bf60, EditBox_HitTest);

// 0x55bfd0. Runs edit box w modally: edits a copy of its text +0xba (kept
// unless +0xa6; caret `caret` capped at the end) until Enter, Esc
// (restores), a click outside (re-posted, with event 0xca for the box +4
// unless its window +0x86 +0x6c refuses), event 0xcd, the 4 s timeout of
// +0x33 0x200 before any key, or a newer modal start 0xd973e8 (then the
// text is left as it was). Keys: caret moves (lines via the font +0x63),
// Backspace / Delete, Ctrl+C / Ctrl+V / Shift+Insert, the paste source
// +0xec (Insert, or '[' on English systems with +0x33 0x4000000), Tab as
// Enter with +0x33 0x2000, characters up to +0xaa long except {}[]| and
// for file names (+0xae) ./\:*?"<>. The caret blinks every 400 ms.
RT3_NOINLINE void __fastcall EditBox_RunModal(uint8_t* w, int, int32_t caret) {
    F<int32_t>(w, 0xd4) = ClockMs();
    char** text = (char**)(w + 0xba);
    char orig[5004], paste[5008], buf[5008];
    strcpy(orig, *text);
    int32_t timeout = (F<uint32_t>(w, 0x33) & 0x200) ? 4000 : 0;
    if (!F<int32_t>(w, 0xa6)) **text = 0;
    F<int32_t>(w, 0xd8) = 0;
    if (caret) {
        int32_t len = (int32_t)strlen(*text);
        if (caret >= len - 1) caret = len;
        TextBox_MoveCursor(w, 0, *text, caret);
    }
    strcpy(buf, *text);
    EditBox_InsertCursor(w, 0, buf, F<int32_t>(w, 0xd8));
    int32_t start = ClockMs();
    bool running = true;
    Obj_Incc60(g_windowManager);
    Input_Block(g_input);
    int32_t lastPump = ClockMs();
    ++F<int32_t>(w, 0xe8);
    do {
        if (timeout && start + timeout < ClockMs()) running = false;
        if (g_d973f0 < ClockMs()) {
            F<int32_t>(w, 0xe4) = F<int32_t>(w, 0xe4) == 0;
            g_d973f0 = ClockMs() + 400;
            EditBox_InsertCursor(w, 0, buf, F<int32_t>(w, 0xd8));
        }
        Engine_Nop();
        --F<int32_t>(w, 0xe8);
        InputEvent scratch;
        InputEvent ev = *Input_NextEvent(g_input, 0, &scratch);
        if (F<int32_t>(w, 0xd4) < g_d973e8 && ClockMs() - g_d973e8 < 60000) {
            Input_Unblock(g_input);
            Obj_Decc60(g_windowManager);
            return;
        }
        ++F<int32_t>(w, 0xe8);
        if (ClockMs() - lastPump > 0x14 || !ev.v[0]) {
            if (void (*pump)() = hook::Global<void (*)()>(0x6d4004)) pump();
            lastPump = ClockMs();
        }
        if (ev.v[0] == 6) {
            if (!(*(int32_t(__fastcall**)(uint8_t*, int, uint32_t, uint32_t, int32_t))(*(uint8_t**)w + 0x20))(w, 0, ev.v[9], ev.v[10], 1)) {
                uint8_t* win = F<uint8_t*>(w, 0x86);
                if (!F<int32_t>(win, 0x6c) || !(*(int32_t(__fastcall**)(uint8_t*))(*(uint8_t**)win + 8))(win)) {
                    g_d973ec = ClockMs();
                    memset(&scratch, 0, sizeof scratch);
                    scratch.v[0] = 0xca;
                    scratch.v[1] = F<uint16_t>(w, 4);
                    scratch.v[11] = (uint32_t)(uintptr_t)w;
                    Input_PushEvent(g_input, 0, scratch.v);
                }
                Input_PushEvent(g_input, 0, ev.v);
                break;
            }
            int32_t pos = EditBox_HitTest(w, 0, *(float*)&ev.v[9], *(float*)&ev.v[10]);
            F<int32_t>(w, 0xd8) = 0;
            TextBox_MoveCursor(w, 0, *text, pos);
            continue;
        }
        if (ev.v[0] == 0xcd) {
            Input_PushEvent(g_input, 0, ev.v);
            running = false;
        }
        if (ev.v[0] != 3) continue;
        uint32_t opts = F<uint32_t>(w, 0x33);
        if ((opts & 0x2000) && ev.v[2] == 9) ev.v[2] = ev.v[3] = 0xd;
        uint32_t key = ev.v[2], ch = ev.v[3];
        uint8_t mods = (uint8_t)ev.v[8];
        bool pasteSource = false, typed = false;
        switch (key) {
        case 8: EditBox_DeleteAt(w, 0, buf, F<int32_t>(w, 0xd8) - 1); break;
        case 9:
            ch = 10;
            typed = true;
            break;
        case 0x1b:
            strcpy(buf, orig);
            running = false;
            break;
        case 0x23:
            if (F<int32_t>(w, 0xd8) < (int32_t)strlen(buf)) F<int32_t>(w, 0xd8) = (int32_t)strlen(buf);
            break;
        case 0x24:
            if (F<int32_t>(w, 0xd8) > 0) {
                F<int32_t>(w, 0xd8) = 0;
                if (F<int32_t>(w, 0xdc) > 0) F<int32_t>(w, 0xdc) = 0;
            }
            break;
        case 0x25: TextBox_MoveCursor(w, 0, buf, -1); break;
        case 0x26:
        case 0x28: {
            int32_t dir = key != 0x26 ? 1 : -1;
            int32_t up, down;
            EditBox_ScrollStates(w, 0, &up, &down);
            uint8_t* win = F<uint8_t*>(w, 0x86);
            int32_t pos = Font_MoveCaretLines(F<uint8_t*>(w, 0x63), 0, F<int32_t>(w, 0xd8), dir, *text, F<float>(win, 5) + F<float>(w, 0xb),
                                              F<float>(win, 9) + F<float>(w, 0xf), F<float>(w, 0x13), (float)(F<float>(w, 0x17) * Fl(0x5c8784)), 0xffffffff,
                                              up, down, 0, 1.0f);
            if (dir > 0 && pos != F<int32_t>(w, 0xd8)) --pos;
            F<int32_t>(w, 0xd8) = 0;
            TextBox_MoveCursor(w, 0, buf, pos);
            break;
        }
        case 0x27: TextBox_MoveCursor(w, 0, buf, 1); break;
        case 0x2d:
            if (mods & 3) EditBox_Paste(w, 0, buf);
            else if (mods & 4) Clipboard_SetText(buf);
            else if (!(IsEnglish() & 0xff) && (F<uint32_t>(w, 0x33) & 0x4000000)) pasteSource = true;
            break;
        case 0x2e: EditBox_DeleteAt(w, 0, buf, F<int32_t>(w, 0xd8)); break;
        default:
            if (key == 0x43 && (mods & 4)) {
                Clipboard_SetText(buf);
                break;
            }
            if (key == 0x56 && (mods & 4)) {
                EditBox_Paste(w, 0, buf);
                break;
            }
            if ((opts & 0x4000000) && (IsEnglish() & 0xff) && ch == 0x5b) pasteSource = true;
            else typed = true;
            break;
        }
        if (pasteSource) {
            paste[0] = 0;
            if (void(__fastcall * src)(uint32_t, char*) = F<void(__fastcall*)(uint32_t, char*)>(w, 0xec)) src(F<uint16_t>(w, 4), paste);
            size_t n = strlen(paste);
            if ((int32_t)n > 0 && (int32_t)(strlen(buf) + n + 1) < F<int32_t>(w, 0xaa)) {
                EditBox_GrowText(w, buf, n);
                int32_t cur = F<int32_t>(w, 0xd8);
                memmove(buf + cur + n, buf + cur, strlen(buf + cur) + 1);
                memmove(buf + F<int32_t>(w, 0xd8), paste, n);
                F<int32_t>(w, 0xd8) += (int32_t)n;
                EditBox_InsertCursor(w, 0, buf, F<int32_t>(w, 0xd8));
            }
            typed = true;
        }
        if (typed && key != 9) {
            static const char banned[] = "{}[]|";
            static const char bannedFile[] = "./\\:*?\"<>";
            if (strchr(banned, (int)ch) && ch) typed = false;
            else if (F<int32_t>(w, 0xae) && ch && strchr(bannedFile, (int)ch)) typed = false;
            else if (ch == 0xd) {
                running = false;
                typed = false;
            }
        }
        if (typed && (int32_t)strlen(buf) < F<int32_t>(w, 0xaa) && ch && (char)ch) {
            EditBox_GrowText(w, buf, 0);
            int32_t cur = F<int32_t>(w, 0xd8);
            memmove(buf + cur + 1, buf + cur, strlen(buf + cur) + 1);
            buf[F<int32_t>(w, 0xd8)] = (char)ch;
            ++F<int32_t>(w, 0xd8);
            EditBox_InsertCursor(w, 0, buf, F<int32_t>(w, 0xd8));
        }
        EditBox_InsertCursor(w, 0, buf, F<int32_t>(w, 0xd8));
        timeout = 0;
    } while (running);
    --F<int32_t>(w, 0xe8);
    Input_Unblock(g_input);
    Obj_Decc60(g_windowManager);
    String_Assign(text, buf);
}
RT3_REPLACE(0x55bfd0, EditBox_RunModal);

}  // namespace rt3
