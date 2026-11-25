// SetupScreen.cpp - RT3.exe 0x502000-0x504000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/crt.h"
#include "core/imports.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);
void __fastcall Obj_FreeBuffer8(void* o);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Widget_SetPicture(uint8_t* g, int, const void* pixels, int32_t w, int32_t h);
int32_t __fastcall Rand(int32_t lo, int32_t hi);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

void __fastcall Section_Enter(uint8_t* s);
void __fastcall Cursor_UseSystem(uint8_t* c);
int32_t __fastcall OpenFileDialog(char* file, const char* filter, const char* initialDir);
void __fastcall Cursor_PopWindowsMode(uint8_t* c);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall App_SetViewMode(uint8_t* app, int, int32_t mode);
uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind);
void __fastcall ScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force);
void __fastcall ScenarioList_Fill(uint8_t* d);
void __fastcall MpSetupDialog_Show(uint8_t* w);
const char* __fastcall Game_VersionText();
void __fastcall MpNewGameDialog_Show(uint8_t* w);
void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
int32_t __fastcall Save_LoadOptions(const char* name);
int32_t Global6d1350Set();
void OptionsDialog_Open();
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
uint32_t __fastcall Engine_IsFullscreen(void* e);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);

// Options callbacks: choice 0 switches the option off, choice n on with
// level n - 1 (blocked by 0x6d135d / 0x6d14a8).
#define RT3_ONOFF(addr, name, guard, on, level) \
    void __fastcall name(int32_t v) { \
        if (v == -1 || guard) return; \
        uint8_t* app = g_app; \
        if (!v) { \
            F<int32_t>(app, on) = v; \
        } else { \
            F<int32_t>(app, on) = 1; \
            F<int32_t>(g_app, level) = v - 1; \
        } \
        Options_Save(g_app, 0, 1); \
    } \
    RT3_REPLACE(addr, name);

void Shell_OpenCommand();
void __fastcall SetupScreen_SetMode(uint8_t* s, int, int32_t mode);
uint32_t __fastcall MapSize_Prompt(uint32_t* width, uint32_t* height);
int32_t __fastcall Tga_IsPlain(const char* path);
int32_t __fastcall Return1_481fb0(void*);

// Functions of this file, in exe order.
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE uint32_t __fastcall MapSize_Prompt(uint32_t* width, uint32_t* height);
RT3_NOINLINE void OptionsDialog_Open();
void __fastcall Lobby_DrawSlotCb(void*, int32_t slot, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall ScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force);
RT3_NOINLINE void __fastcall MpNewGameDialog_Show(uint8_t* w);
void __fastcall Setup_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall ScenarioList_Fill(uint8_t* d);
RT3_NOINLINE void __fastcall MpSetupDialog_Show(uint8_t* w);
RT3_NOINLINE void __fastcall SetupScreen_SetMode(uint8_t* s, int, int32_t mode);
int32_t __fastcall SetupScreen_HandleEvent(uint8_t* s, int, uint32_t* msg);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);


static inline double D(uint32_t a) { return *(const double*)a; }

namespace {
uint32_t Message476(const char* text) {
    return Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// Shell_OpenCommand takes the kind in EAX and the target in EDI.
char* ShellCommand476(int32_t kind, const char* target) {
    char* r;
    __asm {
        mov eax, kind
        mov edi, target
        call Shell_OpenCommand
        mov r, eax
    }
    return r;
}
// Opens target (the readme with kind 1, a web page with 0) or shows `fail`.
int32_t Open476(int32_t kind, const char* target, uint32_t fail) {
    if (char* cmd = ShellCommand476(kind, target))
        imp::WinExec(cmd, 1);
    else
        Message476(GetString(fail));
    return 0;
}
}

 // namespace

// 0x502030 (draw callback). Draws lobby slot `slot` (EDX): its glyph (0x19
// for a filled slot, else 0x18) and the name string of its kind (0x6d14bc
// +0x31a, 39-byte records at 0x5f2d47).
void __fastcall Lobby_DrawSlotCb(void*, int32_t slot, float x, float y, float w, float h, int32_t) {
    const int8_t kind = *(const int8_t*)(g_6d14bc + slot * 9 + 0x31a);
    const int32_t glyph = (slot < F<uint16_t>(g_newGame, 0x79) ? 1 : 0) + 0x18;
    char text[0x170];
    FormatText(text, GetString(*(const int32_t*)(0x5f2d47 + kind * 0x27)), 0, 0, 0, 0, 0, 0, 0, 0);
    const float gy = (float)((double)y + D(0x5d1178));
    const float gx = (float)((double)x + D(0x5c9bc0));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(g_6d14b0, 0, glyph), 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    const float tw = (float)((double)w - D(0x5d1268));
    const float tx = (float)((double)x + D(0x5d1268));
    Font_DrawTextBox(g_6d14b4, 0, text, tx, y, tw, h, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x502030, Lobby_DrawSlotCb);

RT3_ONOFF(0x502160, Setup_Choice233Cb, g_6d14a8, 0x233, 0x237)

RT3_ONOFF(0x5021c0, Setup_Choice23bCb, g_6d14a8, 0x23b, 0x23f)

// 0x502220. Shows scenario `path` (title `name`; none = cleared) in setup
// dialog d, unless it is already shown (0x6cec7c +0x10 / +0x11) and not
// `force`: loads its header (0x6d14bc), applies it to the setup (keeping
// the start year +0x77 in dialog mode 10), keeps the player cash total in
// step (+0x7b, header +0x3ba), and fills in the year, title, description,
// map size (string 0x2b2, plus warning 0xe0c past 0x51000 cells) and the
// 256 x 256 preview.
RT3_NOINLINE void __fastcall ScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force) {
    uint8_t* g = g_newGame;
    if (path) {
        if (!force && g[0x10] && !crt::stricmp((const char*)g + 0x11, path)) return;
        g_newGame[0x10] = 1;
        StrCopyN((char*)g_newGame + 0x11, path, 0x32);
        StrCopyN((char*)g_newGame + 0x44, name, 0x32);
    } else {
        if (!force && g[0x10]) return;
        g[0x10] = 0;
    }
    if (!g_newGame[0x10]) {
        static const uint32_t ids[] = { 0xcf5, 0xe7f, 0xd4c, 0xd4e };
        for (uint32_t id : ids) Window_SendEvent5(d, 0, 0x6f, id, 0x5c87a8, 0, 0);
        return;
    }
    uint16_t year = F<uint16_t>(g_newGame, 0x77);
    Block_Load((const char*)g_newGame + 0x11, g_6d14bc, 0);
    Obj_ApplySetup(g_newGame, 0, g_6d14bc);
    if (F<int32_t>(d, 0x7b) == 10) F<uint16_t>(g_newGame, 0x77) = year;
    uint8_t* s = g_newGame;
    uint8_t* info = g_6d14bc;
    int32_t diff = F<int32_t>(info, 0x3aa) - F<uint16_t>(s, 0x79) - F<uint16_t>(s, 0x7b);
    F<uint16_t>(s, 0x7b) = (uint16_t)(F<uint16_t>(s, 0x7b) + diff);
    F<int32_t>(g_6d14bc, 0x3ba) += diff;
    char yearText[0x20], wText[0x20], hText[0x20], sizeText[0x200];
    Num_Format(yearText, 8, (double)F<uint16_t>(g_newGame, 0x77), 10, 1, 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xd4f, (uint32_t)(uintptr_t)yearText, 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xcf5, (uint32_t)(uintptr_t)(g_newGame + 0x44), 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xe7f, (uint32_t)(uintptr_t)(g_newGame + 0x44), 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xd4c, (uint32_t)(uintptr_t)(g_6d14bc + 0x70), 0, 0);
    const char* ws = Num_Format(wText, 8, (double)F<int32_t>(g_6d14bc, 5), 10, 1, 0, 0);
    const char* hs = Num_Format(hText, 8, (double)F<int32_t>(g_6d14bc, 1), 10, 1, 0, 0);
    FormatText(sizeText, GetString(0x2b2), hs, ws, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    info = g_6d14bc;
    if (F<int32_t>(info, 5) * F<int32_t>(info, 1) > 0x51000) strcat(sizeText, GetString(0xe0c));
    Window_SendEvent5(d, 0, 0x6f, 0xd4e, (uint32_t)(uintptr_t)sizeText, 0, 0);
    Widget_SetPicture(Widget_FindChild(d, 0, 0xd4d), 0, g_6d14bc + 0x3c2, 0x100, 0x100);
}
RT3_REPLACE(0x502220, ScenarioPanel_Update);

// 0x502550. The multiplayer new-game page (busy flag 0x6d14a8 while filling): game
// name (new game +0x44, 0xe7f) and +0x77 ("%.f", 0xe84), then the same lists and
// switches as the options' 0x7587 page on 0xe86 / 0xe87 / 0xe88 / 0xe89 / 0xe8a.
RT3_NOINLINE void __fastcall MpNewGameDialog_Show(uint8_t* w) {
    g_6d14a8 = 1;
    Window_SendEvent5(w, 0, 0x6f, 0xe7f, (uint32_t)(uintptr_t)(g_newGame + 0x44), 0, 0);
    char buf[0x1f8];
    Num_Format(buf, 8, (double)F<uint16_t>(g_newGame, 0x77), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xe84, (uint32_t)(uintptr_t)buf, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0xe86, 0, 0, 0);
    for (int32_t i = 0; i < 0x19; ++i)
        Window_SendEvent5(w, 0, 0x88, 0xe86, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x620dd8)[i]), (uint32_t)i, 0);
    uint8_t* o = g_app;
    Window_SendEvent5(w, 0, 0x66, 0xe86, F<int32_t>(o, 0x233) ? F<uint32_t>(o, 0x237) + 1 : 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0xe87, 0, 0, 0);
    for (int32_t i = 0; i < 0xa; ++i)
        Window_SendEvent5(w, 0, 0x88, 0xe87, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x620e3c)[i]), (uint32_t)i, 0);
    o = g_app;
    Window_SendEvent5(w, 0, 0x66, 0xe87, F<int32_t>(o, 0x23b) ? F<uint32_t>(o, 0x23f) + 1 : 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0xe88, F<uint32_t>(g_app, 0x247), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0xe89, F<uint32_t>(g_app, 0x24b), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0xe8a, F<int32_t>(g_app, 0x243) == 0, 0, 0);
    g_6d14a8 = 0;
}
RT3_REPLACE(0x502550, MpNewGameDialog_Show);

// 0x502720. New-game setup window ("Setup.win") destructor: releases its two
// images (0x6d14b0 / b4), deletes its scenario buffer 0x6d14b8 and frees 0x6d14bc.
void __fastcall Setup_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1664;
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d14b0));
    hook::As<void*>(g_6d14b0) = nullptr;
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d14b4));
    hook::As<void*>(g_6d14b4) = nullptr;
    if (void* o = hook::As<void*>(g_6d14b8)) {
        Obj_FreeBuffer8(o);
        crt::operator_delete(o);
    }
    hook::As<void*>(g_6d14b8) = nullptr;
    if (void* p = hook::As<void*>(g_6d14bc)) crt::free(p);
    hook::As<void*>(g_6d14bc) = nullptr;
    hook::As<int32_t>(g_6d14c0) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x502720, Setup_Destruct);

// 0x5027b0. Fills setup dialog d's scenario list (by dialog mode +0x7b:
// 0xd4b, 0xde8, 0xdf2 or 0xe85; other modes use the original's
// uninitialised local, which holds d) from table 0x6d14b8, selects the
// current scenario, highlights (0xffcfcfff) the entries named in 0x622a98
// and shows the current one.
RT3_NOINLINE void __fastcall ScenarioList_Fill(uint8_t* d) {
    uint32_t id;
    switch (F<int32_t>(d, 0x7b)) {
    case 4: id = 0xd4b; break;
    case 9: id = 0xde8; break;
    case 6:
    case 0xc:
    case 0xe: id = 0xdf2; break;
    case 0xa: id = 0xe85; break;
    default: id = (uint32_t)(uintptr_t)d; break;
    }
    Window_SendEvent5(d, 0, 0x8b, id, 0, 0, 0);
    for (int32_t i = 0; i < F<int32_t>(g_6d14b8, 0); ++i) {
        Window_SendEvent5(d, 0, 0x88, id, (uint32_t)(uintptr_t)(F<uint8_t*>(g_6d14b8, 8) + i * 0x25a + 0x12d), 0, 0);
        if (!crt::stricmp((const char*)F<uint8_t*>(g_6d14b8, 8) + i * 0x25a + 0x12d, (const char*)g_newGame + 0x44))
            Window_SendEvent5(d, 0, 0x66, id, (uint32_t)i, 0, 0);
        for (uint32_t p = 0x622a98; p < 0x622acc; p += 4)
            if (!crt::stricmp((const char*)F<uint8_t*>(g_6d14b8, 8) + i * 0x25a, hook::Global<const char*>(p)))
                Window_SendEvent5(d, 0, 0xaf, id, (uint32_t)i, 0xffcfcfff, 0);
    }
    uint8_t* g = g_newGame;
    if (g[0x10]) ScenarioPanel_Update(d, 0, (const char*)g + 0x11, (const char*)g + 0x44, 1);
    else ScenarioPanel_Update(d, 0, nullptr, nullptr, 1);
}
RT3_REPLACE(0x5027b0, ScenarioList_Fill);

// 0x502910. The multiplayer setup page (when +0x7b is set): AI count (new game
// +0x7b) on 0xcf3, an empty 0xcf4, the game name (+0x44) on 0xcf5 and +0x77 on
// 0xd4f, difficulty +0x7d among 0xce9.. (5); then rebuilds the slot list 0xce8
// from the setup screen's (0x6d14bc) rows of 9 bytes from +0x31b: free rows (the
// first always, the others when open, +0x31c) are listed and claimed (level 1 for
// row 0, 100 + row otherwise), the rest cleared; then up to 500 passes hand the
// remaining players and AIs (+0x79 + +0x7b) to open taken rows. The old selection
// is kept when still in range.
RT3_NOINLINE void __fastcall MpSetupDialog_Show(uint8_t* w) {
    if (!F<int32_t>(w, 0x7b)) return;
    char buf[0x1f0];
    Num_Format(buf, 1, (double)F<uint16_t>(g_newGame, 0x7b), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xcf3, (uint32_t)(uintptr_t)buf, 0, 0);
    FormatText(buf, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xcf4, (uint32_t)(uintptr_t)buf, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xcf5, (uint32_t)(uintptr_t)(g_newGame + 0x44), 0, 0);
    Num_Format(buf, 8, (double)F<uint16_t>(g_newGame, 0x77), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xd4f, (uint32_t)(uintptr_t)buf, 0, 0);
    for (int32_t i = 0; i < 5; ++i)
        Window_SendEvent5(w, 0, g_newGame[0x7d] != i ? 0x87 : 0x65, 0xce9 + i, 0x20, 0, 0);
    int32_t sel = F<int32_t>(Widget_FindChild(w, 0, 0xce8), 0x1d6);
    Window_SendEvent5(w, 0, 0x8b, 0xce8, 0, 0, 0);
    auto claim = [w](int32_t i) {
        Window_SendEvent5(w, 0, 0x88, 0xce8, 0, (uint32_t)i, 0);
        g_newGame[0x87 + i] = i ? (uint8_t)(i + 100) : 1;
    };
    int32_t taken = 0;
    for (int32_t i = 0; i < F<int32_t>(g_6d14bc, 0x3ae); ++i) {
        const uint8_t* row = g_6d14bc + i * 9;
        if (!row[0x31b] && (!i || row[0x31c])) {
            claim(i);
            ++taken;
        } else g_newGame[0x87 + i] = 0;
    }
    uint8_t* ng = g_newGame;
    int32_t left = F<uint16_t>(ng, 0x7b) + F<uint16_t>(ng, 0x79) - taken;
    for (int32_t tries = 0; left > 0 && tries++ < 0x1f4;)
        for (int32_t i = 0; i < F<int32_t>(g_6d14bc, 0x3ae); ++i) {
            const uint8_t* row = g_6d14bc + i * 9;
            if (!row[0x31b]) continue;
            if (left && row[0x31c]) {
                claim(i);
                --left;
            } else g_newGame[0x87 + i] = 0;
        }
    ng = g_newGame;
    if (sel >= 0 && sel < F<uint16_t>(ng, 0x7b) + F<uint16_t>(ng, 0x79)) Window_SendEvent5(w, 0, 0x66, 0xce8, (uint32_t)sel, 0, 0);
}
RT3_REPLACE(0x502910, MpSetupDialog_Show);

// 0x502c00. Setup screen s switches to page mode (s+0x7b; 0x11 pages):
// shows / hides the back button 0xd5e (all but pages 3..6, 9, 10, 12, 14;
// none for 11 and above 0x11), picks the view mode (pages 3..5, 10, 13 set
// 0x6cec74+0x6c and mode 0; 7..9 mode 1), sets lobby +0x97 (cleared on pages
// 6, 9, 12, 14) and clears the scenario choice (lobby +0x10) on page 2, then
// highlights the page buttons (0x6d / 0x86). Pages with a scenario list (3..5
// kind 5, 9 kind 4, 10 kind 6, 6 kind 8, 12 kind 10, 14 kind 9) build the
// list 0x6d14b8 once and preselect its first entry (the second when the
// first starts with "alt") into lobby +0x11 / +0x44; other pages free it.
// Buttons 0xce4 / 0xce5 (pages 4 / 5) and the list frames 0xd4d / 0xd4e
// (shown and resized on list pages 4, 6, 9, 10, 12, 14) follow; the list
// pages refill (0x5027b0), page 3 opens the multiplayer setup and page 1
// shows the version text without trailing blanks.
RT3_NOINLINE void __fastcall SetupScreen_SetMode(uint8_t* s, int, int32_t mode) {
    F<int32_t>(g_app, 0x6c) = 0;
    if ((uint32_t)mode <= 0x11) {
        static const uint8_t kBack[0x12] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 2, 1, 0, 1, 0, 0, 0 };
        if (kBack[mode] != 2) Window_SendEvent5(s, 0, kBack[mode] ? 0x87 : 0x65, 0xd5e, 1, 0, 0);
    }
    int32_t view = 0;
    if ((uint32_t)(mode - 3) <= 0xa) {
        static const uint8_t kView[0xb] = { 0, 0, 0, 2, 1, 1, 1, 0, 2, 2, 0 };
        uint8_t k = kView[mode - 3];
        if (k == 0) F<int32_t>(g_app, 0x6c) = 1;
        else if (k == 1) view = 1;
    }
    App_SetViewMode(g_app, 0, view);
    bool cleared = mode == 6 || mode == 9 || mode == 0xc || mode == 0xe;
    g_newGame[0x97] = cleared ? 0 : 1;
    if (mode == 2) g_newGame[0x10] = 0;
    F<int32_t>(s, 0x7b) = mode;
    auto hi = [&](bool on, uint32_t id) { Window_SendEvent5(s, 0, on ? 0x6d : 0x86, id, 3, 0, 0); };
    hi(mode == 1, 0xbb9);
    hi(F<int32_t>(s, 0x7b) == 0xf, 0xbc5);
    hi(F<int32_t>(s, 0x7b) == 0x10, 0xbc6);
    hi(F<int32_t>(s, 0x7b) == 2, 0xbba);
    hi(F<int32_t>(s, 0x7b) == 3, 0xbbb);
    hi(F<int32_t>(s, 0x7b) == 0xd, 0xbc3);
    int32_t m = F<int32_t>(s, 0x7b);
    hi(m == 3 || m == 4 || m == 5, 0xbbc);
    hi(F<int32_t>(s, 0x7b) == 4, 0xbbd);
    hi(F<int32_t>(s, 0x7b) == 5, 0xbbe);
    m = F<int32_t>(s, 0x7b);
    hi(m == 6 || m == 0xc || m == 0xe, 0xbbf);
    hi(F<int32_t>(s, 0x7b) == 7, 0xbc0);
    hi(F<int32_t>(s, 0x7b) == 8, 0xbc1);
    hi(F<int32_t>(s, 0x7b) == 9, 0xbc2);
    hi(F<int32_t>(s, 0x7b) == 0xa, 0xe75);
    hi(F<int32_t>(s, 0x7b) == 0x11, 0xf6e);
    m = F<int32_t>(s, 0x7b);
    if (m != 3 && m != 4 && m != 5 && m != 9 && m != 0xa && m != 6 && m != 0xc && m != 0xe) {
        if (uint8_t* list = g_6d14b8) {
            Obj_FreeBuffer8(list);
            crt::operator_delete(list);
        }
        g_6d14b8 = nullptr;
    } else if (!g_6d14b8) {
        int32_t kind = 4;
        if (m == 3 || m == 4 || m == 5) kind = 5;
        if (m == 0xa) kind = 6;
        if (m == 6) kind = 8;
        if (m == 0xc) kind = 10;
        if (m == 0xe) kind = 9;
        uint8_t* list = (uint8_t*)crt::operator_new(0xc);
        list = list ? SaveList_Init(list, 0, kind) : nullptr;
        g_6d14b8 = list;
        if (F<int32_t>(list, 0) > 0) {
            int32_t pick = 1;
            if (!crt::strnicmp((const char*)0x5d1670, F<const char*>(list, 8), 3) &&
                F<int32_t>(g_6d14b8, 0) > 1)
                pick = 2;
            g_newGame[0x10] = 1;
            StrCopyN((char*)g_newGame + 0x11, F<const char*>(g_6d14b8, 8) + (pick - 1) * 0x25a, 0x32);
            StrCopyN((char*)g_newGame + 0x44, F<const char*>(g_6d14b8, 8) + (pick - 1) * 0x25a + 0x12d, 0x32);
            uint8_t* lobby = g_newGame;
            if (lobby[0x10]) ScenarioPanel_Update(s, 0, (const char*)lobby + 0x11, (const char*)lobby + 0x44, 1);
            else ScenarioPanel_Update(s, 0, nullptr, nullptr, 1);
        }
    }
    Window_SendEvent5(s, 0, F<int32_t>(s, 0x7b) == 4 ? 0x65 : 0x87, 0xce4, 0x20, 0, 0);
    Window_SendEvent5(s, 0, F<int32_t>(s, 0x7b) == 5 ? 0x65 : 0x87, 0xce5, 0x20, 0, 0);
    m = F<int32_t>(s, 0x7b);
    if (m != 4 && m != 9 && m != 6 && m != 0xa && m != 0xc && m != 0xe) {
        Window_SendEvent5(s, 0, 0x87, 0xd4d, 3, 0, 0);
        Window_SendEvent5(s, 0, 0x87, 0xd4e, 3, 0, 0);
    } else {
        Window_SendEvent5(s, 0, 0x65, 0xd4d, 3, 0, 0);
        Window_SendEvent5(s, 0, 0x65, 0xd4e, 3, 0, 0);
        uint32_t ev[0xc] = {};
        ev[0] = 0x99;
        m = F<int32_t>(s, 0x7b);
        bool wide = m == 4 || m == 0xa;
        uint64_t top = wide ? 0x3fd4ae147ae147aeull : 0x3fd3c28f5c28f5c3ull, bottom = wide ? 0x3fe62fc962fc9630ull : 0x3fe5c962fc962fc9ull;
        F<float>(ev, 0x18) = (float)*(const double*)&top;
        F<float>(ev, 0x1c) = (float)*(const double*)&bottom;
        ev[1] = 0xd4d;
        Window_HandleEvent(s, 0, ev);
        ev[1] = 0xd4e;
        Window_HandleEvent(s, 0, ev);
    }
    if (F<int32_t>(s, 0x7b) == 4) ScenarioList_Fill(s);
    if (F<int32_t>(s, 0x7b) == 9) ScenarioList_Fill(s);
    m = F<int32_t>(s, 0x7b);
    if (m == 6 || m == 0xc || m == 0xe) ScenarioList_Fill(s);
    if (F<int32_t>(s, 0x7b) == 0xa) ScenarioList_Fill(s);
    if (F<int32_t>(s, 0x7b) == 3) MpSetupDialog_Show(s);
    if (F<int32_t>(s, 0x7b) == 1) {
        char text[0x1f4];
        strcpy(text, Game_VersionText());
        char* e = text + strlen(text);
        while (e > text + 1 && e[-1] == ' ') --e;
        *e = 0;
        Window_SendEvent5(s, 0, 0x6f, 0xc23, (uint32_t)(uintptr_t)text, 0, 0);
    }
}
RT3_REPLACE(0x502c00, SetupScreen_SetMode);

 // namespace

// 0x503310 (event handler). The setup screen s (main menu and its sub-pages,
// modes by SetupScreen_SetMode). The first idle tick checks the install
// (0x481fb0 always passes; failure clears 0x6d14c0 and exits) and stirs the
// counter 0x6d14a0 (bumped on every unhandled message, its parity sets
// 0x6d14a4); 0xce selections in the scenario lists 0xd4b / 0xde8 / 0xdf2 /
// 0xe85 show that entry (0x25a each, 0x6d14b8); double clicks (0xca) start
// a scenario or toggle the three options +0x243 / +0x247 / +0x24b; buttons
// (0xcb) switch pages, open the readme / web links (WinExec), start games
// (event 0xcc with the start kind at scenario +1: campaign / sandbox / map
// from a grayscale TGA / random map size, ...), step the multiplayer setup
// values (+0x77 money by 5 within the preset limits +0xc..+0x10 or
// 0x70d..0x834, +0x7b AI count within +0x3ba..+0x3be, +0x7d difficulty) or
// quit (0xcd).
int32_t __fastcall SetupScreen_HandleEvent(uint8_t* s, int, uint32_t* msg) {
    auto mode = [&](int32_t m) {
        SetupScreen_SetMode(s, 0, m);
        return 0;
    };
    auto start = [](uint32_t kind) {
        Game_PostInputEvent(0xcc, 0, kind, 0, 0);
        return 0;
    };
    // Starts the picked scenario (string 0x2ba when none is picked).
    auto play = [&]() {
        uint8_t* sc = g_newGame;
        if (!sc[0x10]) {
            Message476(GetString(0x2ba));
            return 0;
        }
        F<uint32_t>(sc, 1) = msg[1] == 0xcf6 || msg[1] == 0xe81 ? 5 : 3;
        g_newGame[0x82] = msg[1] == 0xe81;
        return start(Save_LoadOptions((const char*)g_newGame + 0x11) ? 7 : 1);
    };
    auto toggle = [](int32_t off) {
        uint8_t* o = g_app;
        F<int32_t>(o, off) = F<int32_t>(o, off) == 0;
        Options_Save(g_app, 0, 1);
        return 0;
    };
    auto setupShown = [&]() {
        MpSetupDialog_Show(s);
        return 0;
    };
    // Money in steps of 5 for the new multiplayer game, kept in 0x70d..0x834.
    auto money = [&](int32_t step) {
        F<uint16_t>(g_newGame, 0x77) += (uint16_t)step;
        uint8_t* p = g_newGame;
        uint16_t v = F<uint16_t>(p, 0x77);
        if (step > 0 && v > 0x834) v = 0x834;
        if (step < 0 && v < 0x70d) v = 0x70d;
        F<uint16_t>(p, 0x77) = v;
        MpNewGameDialog_Show(s);
        g_6d14ac = 1;
        return 0;
    };
    switch (msg[0]) {
    case 0: {
        if (g_6d149c) break;
        g_6d149c = 1;
        const int32_t ok = Return1_481fb0(s);
        g_622a20 = ok;
        if (!ok) memset(g_6d14c0, 0, 0x7f);
        const int32_t r = Rand(1, 5);
        g_6d14a0 = (r + g_6d14a0 + 4) * 3;
        g_6d14a0 += Rand(0, 2);
        if (g_6d14a0 & 1) g_6d14a4 = 1;
        if (!g_622a20) crt::exit(0);
        break;
    }
    case 0xce: {
        const uint32_t id = msg[1];
        if (id != 0xd4b && id != 0xde8 && id != 0xdf2 && id != 0xe85) break;
        if ((int32_t)msg[2] < 0) break;
        uint8_t* e = F<uint8_t*>(g_6d14b8, 8) + (int32_t)msg[2] * 0x25a;
        ScenarioPanel_Update(s, 0, (const char*)e, (const char*)e + 0x12d, 0);
        break;
    }
    case 0xca:
        switch (msg[1]) {
        case 0xd4b: return mode(3);
        case 0xde8:
        case 0xdf2: return play();
        case 0xe88: return toggle(0x247);
        case 0xe89: return toggle(0x24b);
        case 0xe8a: return toggle(0x243);
        }
        break;
    case 0xcb:
        switch (msg[1]) {
        case 0xbc1:
        case 0xbc8:
        case 0xdcc: return mode(0xf);
        case 0xbc2: return Open476(1, (const char*)0x5d16a8, 0xbf8);
        case 0xbc3: return mode(0x10);
        case 0xbc4:
        case 0xc86:
        case 0xf71: return mode(1);
        case 0xbc5: return Open476(0, GetString(0xbfa), 0xbf9);
        case 0xbc6: return Open476(0, GetString(0xbfb), 0xbf9);
        case 0xbc7: return Open476(0, GetString(0xbfc), 0xbf9);
        case 0xc1c: return mode(0x11);
        case 0xc1d:
        case 0xcf7:
        case 0xdf4:
        case 0xe80:
        case 0xf40: return mode(2);
        case 0xc1e: return start(5);
        case 0xc1f:
        case 0xde0:
        case 0xdea: return mode(7);
        case 0xc20:
            HighScores_Show(-1, 0, 0);
            return 0;
        case 0xc21: return start(6);
        case 0xc22:
            Game_PostInputEvent(0xcd, 0, 0, 0, 0);
            return 0;
        case 0xc24:
            if (!Global6d1350Set()) OptionsDialog_Open();
            return 0;
        case 0xc80: return mode(0xd);
        case 0xc81: return mode(0xe);
        case 0xc82:
        case 0xd48:
        case 0xdb6: return mode(3);
        case 0xc83: return mode(6);
        case 0xc84:
            g_6d14ac = 0;
            SetupScreen_SetMode(s, 0, 0xa);
            if (!g_6d14ac) F<uint16_t>(g_newGame, 0x77) = 0x78a;
            MpNewGameDialog_Show(s);
            return 0;
        case 0xc85: return mode(0xc);
        case 0xce4: return mode(F<uint8_t*>(msg, 0x2c)[0x6a] & 0x20 ? 4 : 3);
        case 0xce5: return mode(F<uint8_t*>(msg, 0x2c)[0x6a] & 0x20 ? 5 : 3);
        case 0xce6: {
            uint8_t* p = g_6d14bc;
            uint8_t* g = g_newGame;
            if (p && F<int32_t>(p, 0x3be) > (int32_t)F<uint16_t>(g, 0x7b))
                ++F<uint16_t>(g, 0x7b);
            else
                Message476(GetString(0x2b8));
            return setupShown();
        }
        case 0xce7: {
            uint8_t* p = g_6d14bc;
            uint8_t* g = g_newGame;
            if (p && F<int32_t>(p, 0x3ba) < (int32_t)F<uint16_t>(g, 0x7b))
                --F<uint16_t>(g, 0x7b);
            else
                Message476(GetString(0x2b9));
            return setupShown();
        }
        case 0xce9:
        case 0xcea:
        case 0xceb:
        case 0xcec:
        case 0xced:
            g_newGame[0x7d] = (uint8_t)(msg[1] + 0x17);
            return setupShown();
        case 0xcf6:
        case 0xde9:
        case 0xdf3:
        case 0xe81: return play();
        case 0xd49:
        case 0xd4a: {
            const bool up = msg[1] == 0xd49;
            F<uint16_t>(g_newGame, 0x77) += up ? 5 : 0xfffb;
            uint8_t* p = g_6d14bc;
            if (p && *p) {
                uint8_t* g = g_newGame;
                int32_t v = F<uint16_t>(g, 0x77);
                const int32_t lim = F<int32_t>(p, up ? 0x10 : 0xc);
                if (up ? v >= lim : v <= lim) v = lim;
                F<uint16_t>(g, 0x77) = (uint16_t)v;
            }
            return setupShown();
        }
        case 0xdca:
            F<uint32_t>(g_newGame, 1) = 4;
            return mode(8);
        case 0xdcb:
            F<uint32_t>(g_newGame, 1) = 3;
            return mode(9);
        case 0xdd4:
            if (!MapSize_Prompt((uint32_t*)0x6d14cc, (uint32_t*)0x6d14d0)) return 0;
            F<uint32_t>(g_newGame, 1) = 4;
            return start(1);
        case 0xddf: {
            if (Engine_IsFullscreen(hook::As<void*>(g_engine))) {
                Message476(GetString(0xbf7));
                return 0;
            }
            Section_Enter(g_cursorManager);
            Cursor_UseSystem(g_cursorManager);
            const int32_t picked = OpenFileDialog((char*)0x6d1370, (const char*)0x5d1674, (const char*)0x5d1690);
            Cursor_PopWindowsMode(g_cursorManager);
            if (!picked) return 0;
            if (!Tga_IsPlain((const char*)0x6d1370)) {
                Message476(GetString(0xe13));
                return 0;
            }
            F<uint32_t>(g_newGame, 1) = 2;
            return start(1);
        }
        case 0xe82: return money(5);
        case 0xe83: return money(-5);
        case 0xf3d:
        case 0xf3e:
        case 0xf3f:
            g_newGame[0x7d] = (uint8_t)(msg[1] - 0x3d);
            return start(7);
        case 0xf6f:
            F<uint32_t>(g_newGame, 1) = 1;
            return start(1);
        case 0xf70:
            F<uint32_t>(g_newGame, 1) = 7;
            return start(1);
        }
        break;
    }
    ++g_6d14a0;
    return 1;
}
RT3_REPLACE(0x503310, SetupScreen_HandleEvent);

}  // namespace rt3
