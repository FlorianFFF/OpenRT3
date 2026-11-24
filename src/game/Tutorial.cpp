// Tutorial.cpp - RT3.exe 0x516000-0x517c00.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

int32_t ClockMs();
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode);
void __fastcall Playlist_Tick(uint8_t* p);
const char* __fastcall GetString(int32_t id);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
uint32_t IsEnglish();
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall FreeAndNull(void** p);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
void __fastcall Obj_Decc60(uint8_t* o);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags,
                              const char* tip, int32_t id, uint32_t color, uint32_t hover);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

double __fastcall Obj_Float18(void* o);
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
void __fastcall Camera_Update(uint8_t* c);
void __fastcall MapPointer_Place(int32_t slot, int32_t kind, float x, float z, float lift);
void __fastcall Camera_MoveEyeTo(uint8_t* c, int, float x, float z);
void __fastcall Camera_ApplyZoom(uint8_t* c);
void* __fastcall Obj_Company21(void* o);
void __fastcall Tutorial_ShowPicture(int32_t page, int, float x, float y);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
int32_t Global6d10a8Set();

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __stdcall Tutorial_IndexOf(float v);
void __fastcall Weather_ApplyIntensity(uint8_t* w);
uint8_t* __fastcall TutorialWindow_Construct(uint8_t* w);
int32_t __fastcall Tutorial_OnBuildingPlaced(int32_t id);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void MapPointers_Delete();
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
uint8_t* __fastcall WinLose_Construct(uint8_t* w);
int32_t __fastcall HasString(int32_t id);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
int32_t Global6cfe00Set();
RT3_NOINLINE void __fastcall Camera_MouseUpdate(uint8_t* c, int, int32_t forced);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void Tutorial_Start();
void __fastcall VideoWindow_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
uint8_t* __fastcall VideoWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __cdecl Music_Resume();
void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
void Tutorial_Advance();
void __fastcall Engine_ResetTextureQueue(void* e);
void Scenario_Finish();
int32_t Scenario_GoalIs3c();
void MapPointers_Close();
int32_t __stdcall Track_NodeNear(float x, float y);
int32_t __fastcall Table_CountActive(Collection* t);
int32_t __fastcall Records_CountClass3(Collection* c);
int32_t __fastcall Records_CountClass4(Collection* c);
int32_t __fastcall Table_CountKind4Idle(Collection* t);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall Records_CountClass3(Collection* c);
RT3_NOINLINE int32_t __fastcall Records_CountClass4(Collection* c);
RT3_NOINLINE void __fastcall Camera_MouseUpdate(uint8_t* c, int, int32_t forced);
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall MapPointer_Place(int32_t slot, int32_t kind, float x, float z, float lift);
RT3_NOINLINE void PreviewModels_Free();
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
void __fastcall TutorialPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE int32_t __stdcall Track_NodeNear(float x, float y);
int32_t __stdcall Tutorial_IndexOf(float v);
void __fastcall Tutorial_ShowPicture(int32_t page, int, float x, float y);
int32_t __fastcall Dialog_HandleEvent(uint32_t* ev);
RT3_NOINLINE int32_t Tutorial_Blink348();
RT3_NOINLINE int32_t Tutorial_Blink352();
int32_t Tutorial_Blink37a();
int32_t Tutorial_Is19a();
int32_t Tutorial_Is1ea();
RT3_NOINLINE int32_t Scenario_GoalIs3c();
RT3_NOINLINE void MapPointers_Close();
RT3_NOINLINE void __fastcall Tutorial_ShowStep(int32_t step);
uint8_t* __fastcall TutorialWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall MapPointers_Destruct(uint8_t* w);
RT3_NOINLINE void MapPointers_Delete();
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE int32_t __fastcall Tutorial_OnBuildingPlaced(int32_t id);
int32_t __fastcall TutorialWindow_HandleEvent(void*, int, uint32_t* msg);
RT3_NOINLINE void Tutorial_Start();
RT3_NOINLINE void __cdecl Music_Resume();
RT3_NOINLINE void Scenario_Finish();
uint8_t* __fastcall VideoWindow_Construct(uint8_t* w);
void __fastcall VideoWindow_Destruct(uint8_t* w);
int32_t __fastcall ScenarioEnd_HandleEvent(uint8_t* w, int, uint32_t* msg);
RT3_NOINLINE uint8_t* __fastcall WinLose_Construct(uint8_t* w);
int32_t __fastcall IntroScreen_HandleEvent(uint8_t* s, int, uint32_t* msg);
RT3_NOINLINE void __fastcall Scenario_ShowResult(int32_t level);
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);

// 0x5164f0 / 0x516540. Blink phase (350 ms) while the current tutorial step
// (0x6d3b50) is 0x348 / 0x352 or 0x357.
static int32_t TutorialBlink(bool match) {
    if (!g_6d3b4c || !match) return 0;
    return (ClockMs() / 350) & 1 ? 1 : 0;
}

static inline int32_t TutorialStep() { return ((const int32_t*)0x622b48)[g_6d3b50 * 0x10]; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// Company finance statistics. Ids below 0x2b are kept per year: +0x1c47 + id * 8
// (this year so far) and +0xd7f + (id * 11 + n) * 8 (n years ago, 0 = live); larger
// ids are sums / ratios of those. Years: 0x2329 = live value, 0x232a = this year so
// far. World 0x6cec78: year +0xd, month +0xf, day +0x11, tick +0x15, difficulty +0x2d.
static inline uint8_t* WorldG() { return g_world; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// 0x516010 (draw callback). Tutorial panel: the backdrop (0x6d3b3c entry 0,
// scaled to the box), its black frame, the lesson text (0x6d3b48), and the
// links of the current page (0x6d3b50; 64-byte records at 0x622b48): "next"
// (string 0xe06 / 0xe07, 0xc79e) when it has one (+4), and for page kinds
// 0x1fe "exit" (0xe08 / 0xe09, 0xc7a1) plus "back" (0xe05 / 0xe04, 0xc7a0),
// for 0x38e only "back" on the right.
void __fastcall TutorialPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    const float sx = (float)((double)w / D(0x5cab68));
    const float sy = (float)((double)h / D(0x5d0820));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(g_6d3b3c, 0, 0), 0, x, y, 0x3f800000, sx, sy, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    Ui_FrameRect(0xff000000, 0, x, y, w, h, 2.5f, 3);
    Engine_ResetTextureQueue(hook::As<void*>(g_engine));
    const float tw = (float)((double)w - D(0x5d0170));
    const float ty = (float)((double)y + D(0x5d0110));
    const float tx = (float)((double)x + D(0x5d00e8));
    Font_DrawTextBox(g_6d3b40, 0, g_6d3b48, tx, ty, tw, BitsF(0x3eeccccd), 0xffffffaf, 2, 1, 0, 1.0f);
    char frame[0x3f0];
    char* next = frame + 8;
    FormatText(next, GetString(0xe06), 0, 0, 0, 0, 0, 0, 0, 0);
    const uint8_t* page = (const uint8_t*)(0x622b48 + g_6d3b50 * 0x40);
    if (F<uint32_t>((void*)page, 4)) {
        const char* tip = GetString(0xe07);
        const float lw = (float)((double)w - D(0x5d1528));
        const float ly = (float)((double)y + h - D(0x5d19f0));
        TextLink_Draw(g_dialogWindow, g_6d3b40, next, x, ly, lw, BitsF(0x3d2aaaab), 3, 0, 1, tip, 0xc79e, 0xff4fd54f,
                      0xff7fff7f);
    }
    const int32_t kind = *(const int32_t*)(0x622b48 + g_6d3b50 * 0x40);
    if (kind == 0x1fe) {
        const float lw = (float)((double)w - D(0x5d1528));
        const float ly = (float)((double)y + h - D(0x5d19f0));
        const char* tip = GetString(0xe09);
        const char* exit = GetString(0xe08);
        TextLink_Draw(g_dialogWindow, g_6d3b40, exit, x, ly, lw, BitsF(0x3d2aaaab), 3, 0, 1, tip, 0xc7a1,
                      0xff4fd54f, 0xff7fff7f);
        const char* tip2 = GetString(0xe04);
        const float bx = (float)((double)x + D(0x5d1528));
        const char* back = GetString(0xe05);
        TextLink_Draw(g_dialogWindow, g_6d3b40, back, bx, ly, lw, BitsF(0x3d2aaaab), 2, 0, 1, tip2, 0xc7a0,
                      0xff4fd54f, 0xff7fff7f);
    } else if (kind == 0x38e) {
        const char* tip = GetString(0xe04);
        const float lw = (float)((double)w - D(0x5d1528));
        const float ly = (float)((double)y + h - D(0x5d19f0));
        const float bx = (float)((double)x + D(0x5d1528));
        const char* back = GetString(0xe05);
        TextLink_Draw(g_dialogWindow, g_6d3b40, back, bx, ly, lw, BitsF(0x3d2aaaab), 2, 0, 1, tip, 0xc7a0,
                      0xff4fd54f, 0xff7fff7f);
    }
}
RT3_REPLACE(0x516010, TutorialPanel_DrawCb);

// 0x516380. A track node lies within 2.9 of (x, y).
RT3_NOINLINE int32_t __stdcall Track_NodeNear(float x, float y) {
    return Grid_NearestNodeWithin(g_tracks, 0, x, y, -1, BitsF(0x4039999a), 1, 0, 0) != -1;
}
RT3_REPLACE(0x516380, Track_NodeNear);

// 0x5163b0. Index of the tutorial step (0x622b48, 0x40 bytes each) with id v; 0 if none.
RT3_NOINLINE int32_t __stdcall Tutorial_IndexOf(float v) {
    int32_t i = 0;
    for (const int32_t* p = (const int32_t*)0x622b48; p < (const int32_t*)0x624308; p += 0x10, ++i)
        if ((float)*p == v) return i;
    return 0;
}
RT3_REPLACE(0x5163b0, Tutorial_IndexOf);

// 0x5163f0. Shows tutorial picture `page` ("tutorial%d.imb", scroll widget 0xc79f
// flags 0x40b0) at (x, y) in the dialog window 0x6d3b44, replacing the previous
// one; the page is kept in 0x622b30.
RT3_NOINLINE void __fastcall Tutorial_ShowPicture(int32_t page, int, float x, float y) {
    g_622b30 = -1;
    Window_DeleteChildren(g_dialogWindow, 0, 0xc79f, 0xc79f, 0);
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    char name[0x80];
    g_622b30 = page;
    F<float>(def, 7) = x;
    F<float>(def, 0xb) = y;
    F<uint16_t>(def, 0) = 0xc79f;
    F<uint32_t>(def, 0x2f) = 0x40b0;
    ExeSprintf()(name, (const char*)0x5d19f8, page);
    String_Assign(&F<char*>(def, 0x43), name);
    F<uint16_t>(def, 0x5f) = 0;
    uint8_t* s = (uint8_t*)ExeNew(0x111);
    if (s) s = Scrollbar_Construct(s, 0, def);
    Window_AddChild(g_dialogWindow, 0, s, 0);
    Free(F<void*>(def, 0x43));
}
RT3_REPLACE(0x5163f0, Tutorial_ShowPicture);

// 0x5164d0. Passes an event to the open dialog window (0x6d3b44); 1 without one.
int32_t __fastcall Dialog_HandleEvent(uint32_t* ev) {
    uint8_t* dlg = g_dialogWindow;
    if (!dlg) return 1;
    return Window_HandleEvent(dlg, 0, ev);
}
RT3_REPLACE(0x5164d0, Dialog_HandleEvent);

RT3_NOINLINE int32_t Tutorial_Blink348() { return TutorialBlink(g_6d3b4c && TutorialStep() == 0x348); }
RT3_REPLACE(0x5164f0, Tutorial_Blink348);

RT3_NOINLINE int32_t Tutorial_Blink352() {
    if (!g_6d3b4c) return 0;
    int32_t s = TutorialStep();
    return TutorialBlink(s == 0x352 || s == 0x357);
}
RT3_REPLACE(0x516540, Tutorial_Blink352);

int32_t Tutorial_Blink37a() {
    if (!g_6d3b4c || TutorialStep() != 0x37a) return 0;
    return (ClockMs() / 350) & 1 ? 1 : 0;
}
RT3_REPLACE(0x516590, Tutorial_Blink37a);

int32_t Tutorial_Is19a() { return g_6d3b4c && TutorialStep() == 0x19a ? 1 : 0; }
RT3_REPLACE(0x5165e0, Tutorial_Is19a);

int32_t Tutorial_Is1ea() { return g_6d3b4c && TutorialStep() == 0x1ea ? 1 : 0; }
RT3_REPLACE(0x516610, Tutorial_Is1ea);

// 0x516640. The selected scenario goal (0x6d3b50, 64-byte records at
// 0x622b48) is kind 0x3c while 0x6d3b4c is set.
RT3_NOINLINE int32_t Scenario_GoalIs3c() { return g_6d3b4c && *(const int32_t*)(0x622b48 + (hook::As<uint32_t>(g_6d3b50) << 6)) == 0x3c; }
RT3_REPLACE(0x516640, Scenario_GoalIs3c);

// 0x516670. Closes one level of the map pointer overlay (0x6d3b64): deletes
// its widgets from the window (0x6d3b44), forgets the hover (0x622b30),
// frees its buffer (0x6d3b48) and the preview models.
RT3_NOINLINE void MapPointers_Close() {
    if (!g_6d3b64) return;
    --g_6d3b64;
    Window_DeleteChildren(g_dialogWindow, 0, 0xc79f, 0xc79f, 0);
    g_622b30 = -1;
    Window_DeleteChildren(g_dialogWindow, 0, 0xc79f, 0xc79f, 0);
    Window_DeleteChildren(g_dialogWindow, 0, 0xc79c, 0xc7ff, 0);
    FreeAndNull((void**)0x6d3b48);
    PreviewModels_Free();
}
RT3_REPLACE(0x516670, MapPointers_Close);

// 0x5166e0. Shows tutorial step `step` (64-byte records at 0x622b48: text
// +0x3c, picture +0x28 at +0x2c / +0x30, two map pointers +0x18 / +0x20, the
// camera target +0x8 and zoom range +0x10..+0x14): closes the old pointers
// (counting steps in 0x6d3b64), places the pointers, moves / zooms the
// camera (twice, so it settles), formats the text (with the player's name for
// texts 0x64 / 0x29e / 0x2bc) into 0x6d3b48, shows the picture and adds the
// text box (0xc79d, width grown from 0.3 in 0x5d1160 steps until the text
// fits 0x5d1a08) to the pointer window 0x6d3b44.
// QUIRK: the fit is measured on the unformatted text.
RT3_NOINLINE void __fastcall Tutorial_ShowStep(int32_t step) {
    const uint8_t* t = (const uint8_t*)0x622b48 + step * 64;
    const char* text = GetString(F<int32_t>((void*)t, 0x3c));
    int32_t pic = F<int32_t>((void*)t, 0x28);
    float px = (float)(F<float>((void*)t, 0x2c) / D(0x5ca2c8));
    float py = (float)(F<float>((void*)t, 0x30) / D(0x5ca2d0));
    MapPointers_Close();
    ++g_6d3b64;
    const float zero = g_5c8598;
    if (F<float>((void*)t, 0x18) != zero && F<float>((void*)t, 0x1c) != zero) {
        MapPointer_Place(1, 1, F<float>((void*)t, 0x18), F<float>((void*)t, 0x1c), 50.0f);
        MapPointer_Place(2, 2, F<float>((void*)t, 0x18), F<float>((void*)t, 0x1c), 0.0f);
    }
    if (F<float>((void*)t, 0x20) != zero && F<float>((void*)t, 0x24) != zero) {
        MapPointer_Place(3, 1, F<float>((void*)t, 0x20), F<float>((void*)t, 0x24), 50.0f);
        MapPointer_Place(4, 2, F<float>((void*)t, 0x20), F<float>((void*)t, 0x24), 0.0f);
    }
    auto settle = [] {
        uint8_t* c = g_62be68;
        Camera_Update(c);
        c = g_62be68;
        ((void(__fastcall*)(uint8_t*))(*(void***)c)[0])(c);
        Camera_MouseUpdate(g_62be68, 0, 1);
        Camera_ApplyZoom(g_62be68);
        c = g_62be68;
        ((void(__fastcall*)(uint8_t*))(*(void***)c)[4])(c);
    };
    for (int pass = 0; pass < 2; ++pass) {
        if (F<float>((void*)t, 8) != zero) {
            Camera_MoveEyeTo(g_62be68, 0, F<float>((void*)t, 8), F<float>((void*)t, 0xc));
            settle();
        }
        if (F<float>((void*)t, 0x10) != zero) {
            uint8_t* c = g_62be68;
            if (F<float>(c, 0x14) < D(0x5c86b8)) {
                F<uint32_t>(c, 0x14) = 0x3dcccccd;
                c = g_62be68;
            }
            if (Obj_Float18(c) < F<float>((void*)t, 0x10)) Camera_SetZoom(g_62be68, 0, F<uint32_t>((void*)t, 0x10));
            if (Obj_Float18(g_62be68) > F<float>((void*)t, 0x14))
                Camera_SetZoom(g_62be68, 0, F<uint32_t>((void*)t, 0x14));
            settle();
        }
    }
    char name[0x190];
    if (Obj_Company21(hook::As<void*>(g_world))) strcpy(name, (const char*)Obj_Company21(hook::As<void*>(g_world)) + 4);
    else name[0] = 0;
    char out[0x7c4];
    int32_t kind = F<int32_t>((void*)t, 0);
    FormatText(out, text, kind == 0x64 || kind == 0x29e || kind == 0x2bc ? name : nullptr, 0, 0, 0, 0, 0, 0, 0);
    String_Assign((char**)0x6d3b48, out);
    if (pic >= 0) Tutorial_ShowPicture(pic, 0, px, py);
    float w = BitsF(0x3e99999a);
    for (;;) {
        float h = (float)Font_TextHeight(g_6d3b40, 0, text, (float)(w - D(0x5d0170)), nullptr);
        if (!(h > D(0x5d1a08))) break;
        w = (float)(w + D(0x5d1160));
        if (!(w < D(0x5c8750))) break;
    }
    uint8_t def[0x89] = {};
    *(uint16_t*)def = 0xc79d;
    *(uint32_t*)(def + 7) = 0x3c4ccccd;
    *(uint32_t*)(def + 0xb) = 0x3c888889;
    *(float*)(def + 0xf) = w;
    *(uint32_t*)(def + 0x13) = 0x3f0aaaab;
    *(uint32_t*)(def + 0x2f) = 0x40b0;
    uint8_t* box = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x111);
    if (box) box = Scrollbar_Construct(box, 0, def);
    Window_AddChild(g_dialogWindow, 0, box, 0);
    Window_SendEvent5(g_dialogWindow, 0, 0x7a, 0xc79d, 0x516010, 0, 0);
}
RT3_REPLACE(0x5166e0, Tutorial_ShowStep);

// 0x516b00. Tutorial window constructor (vtable 0x5d1a34): its frame image
// "TutorialWindow.imb" goes to 0x6d3b3c, layout "Tutorial.win", depth +0x21 = 0x15.
RT3_NOINLINE uint8_t* __fastcall TutorialWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1a34;
    g_6d3b3c = Pack_AcquireNamed(g_pack, 0, (const char*)0x5d1a20);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1a10);
    F<int32_t>(w, 0x21) = 0x15;
    return w;
}
RT3_REPLACE(0x516b00, TutorialWindow_Construct);

// 0x516b40. Destructs the map pointer overlay w: closes it, releases its two
// pack images (0x6d3b3c / 0x6d3b40) and forgets its window and state.
RT3_NOINLINE void __fastcall MapPointers_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1a34;
    MapPointers_Close();
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d3b3c));
    void* second = hook::As<void*>(g_6d3b40);
    hook::As<uint32_t>(g_6d3b3c) = 0;
    hook::As<uint32_t>(g_dialogWindow) = 0;
    Pack_Release(g_pack, 0, second);
    hook::As<uint32_t>(g_6d3b40) = 0;
    hook::As<uint32_t>(g_6d3b4c) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x516b40, MapPointers_Destruct);

// 0x516ba0. Removes and deletes the map pointer overlay window (0x6d3b44),
// unless it is pinned (0x6d3b60).
RT3_NOINLINE void MapPointers_Delete() {
    if (!g_dialogWindow || g_6d3b60) return;
    Windows_Remove(g_windowManager, 0, g_dialogWindow);
    if (uint8_t* w = g_dialogWindow) {
        MapPointers_Destruct(w);
        ((void(__cdecl*)(void*))0x53b080)(w);
    }
}
RT3_REPLACE(0x516ba0, MapPointers_Delete);

// 0x516be0. Advances the running tutorial (0x6d3b4c) to the next step
// (0x6d3b50), skipping steps that do not apply (0x226 without the panel
// 0x6d10a8, 0x2b2 without 0x6d19f4, 0x50 / 0x5a in a player's game), takes
// the step's two parameters (0x622b34 / 0x622b3c, the old one in 0x622b38),
// clicks (at most every 50 ms, 0x6d3b54) and shows the step - or ends with
// the pointers removed after step 0x5e.
RT3_NOINLINE void Tutorial_Advance() {
    if (!g_6d3b4c) return;
    int32_t& step = g_6d3b50;
    auto kind = [&] { return *(int32_t*)(0x622b48 + step * 64); };
    ++step;
    g_6d3b58 = 0;
    if (kind() == 0x226 && !Global6d10a8Set()) ++step;
    if (kind() == 0x2b2 && !g_6d19f4) ++step;
    if (kind() == 0x50 && Obj_Get21(hook::As<void*>(g_world))) ++step;
    if (kind() == 0x5a && Obj_Get21(hook::As<void*>(g_world))) ++step;
    g_622b38 = g_622b34;
    g_622b34 = *(uint32_t*)(0x622b7c + step * 64);
    g_622b3c = *(uint32_t*)(0x622b80 + step * 64);
    if (step > 0 && ClockMs() - g_6d3b54 > 50) {
        Ui_PlaySound(3, 0, 0x3f000000);
        g_6d3b54 = ClockMs();
    }
    if (step >= 0x5f) MapPointers_Delete();
    else Tutorial_ShowStep(step);
}
RT3_REPLACE(0x516be0, Tutorial_Advance);

// 0x516d00. Tutorial: a building was placed (id). Steps 0x172 (remembering
// it in 0x6d3b5c) and 0x17c (a second, different one) advance; at any other
// step it is refused with message 0xe03 (0).
RT3_NOINLINE int32_t __fastcall Tutorial_OnBuildingPlaced(int32_t id) {
    int32_t kind = *(int32_t*)(0x622b48 + g_6d3b50 * 64);
    if (kind == 0x172) {
        g_6d3b5c = id;
        Tutorial_Advance();
        return 1;
    }
    if (kind == 0x17c) {
        if (id != g_6d3b5c) Tutorial_Advance();
        return 1;
    }
    Dialog_RunMessage(GetString(0xe03), 1, 0x3ef5c28f, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    return 0;
}
RT3_REPLACE(0x516d00, Tutorial_OnBuildingPlaced);

// 0x516d90 (event handler). The tutorial window: Next (0xc79e) advances,
// 0xc7a1 restarts it (Tutorial_2.gmp, the options' mode +0x6c 2 meanwhile,
// 0x6d3b60), 0xc7a0 quits to the menu (event 0xcc 2). Idle advances when the
// current step's goal (0x622b48, 64 bytes a step, by 0x6d3b50) is met:
// the one train stopped (0x1ea), no modal window (0x24e / 0x276 / 0x302),
// a company (0x5a), track laid at the tutorial spots (0x64, which also
// keeps the player's company colour / name in the options, 0x96 .. 0xc3),
// buildings of the kinds counted (0xdc / 0xe6 / 0xfa / 0x118 / 0x136), the
// flag 0x6d3b58 (0x1f4), the camera zoomed out past 10000 (0x69 / 0x339)
// or the panel 0x6cfe00 open (0x186).
int32_t __fastcall TutorialWindow_HandleEvent(void*, int, uint32_t* msg) {
    auto goal = []() { return *(const int32_t*)(0x622b48 + (hook::As<uint32_t>(g_6d3b50) << 6)); };
    auto noModal = []() { return F<int32_t>(g_windowManager, 0xc64) <= 0; };
    auto trackAt = []() {
        return Grid_NearestNodeWithin(g_tracks, 0, BitsF(0x41e00000), BitsF(0x43c20000), -1, BitsF(0x4039999a), 1, 0, 0) != -1 &&
               Track_NodeNear(BitsF(0x420c0000), BitsF(0x43c20000));
    };
    if (msg[0] == 0xcb) {
        switch (msg[1]) {
        case 0xc79e:
            Tutorial_Advance();
            return 0;
        case 0xc7a1:
            g_6d3b60 = 1;
            MapPointers_Close();
            F<int32_t>(g_app, 0x6c) = 2;
            World_Load(g_world, 0, (const char*)0x5c9f58, 0, nullptr);
            F<int32_t>(g_app, 0x6c) = 0;
            g_6d3b60 = 0;
            Tutorial_Advance();
            return 0;
        case 0xc7a0:
            MapPointers_Delete();
            View_SetToolMode(g_gameWindow, 0, 0, -1);
            Game_PostInputEvent(0xcc, 0, 2, 0, 0);
            return 0;
        }
        return 1;
    }
    if (msg[0] != 0) return 1;
    if (g_6d3b4c && goal() == 0x1ea && Collection_Count(g_trains) == 1) {
        Collection* trains = g_trains;
        if (!F<int32_t>(Collection_Get(trains, 0, Collection_IdAt(trains, 0, 0, 0)), 0xe8)) Tutorial_Advance();
    }
    if (goal() == 0x24e && noModal()) Tutorial_Advance();
    if (goal() == 0x276 && noModal()) Tutorial_Advance();
    if (goal() == 0x302 && noModal()) Tutorial_Advance();
    if (goal() == 0x5a && Obj_Get21(g_world)) Tutorial_Advance();
    if (goal() == 0x64) {
        uint8_t* opts = g_app;
        if (F<uint32_t>(opts, 0x305) != F<uint32_t>(Obj_Company21(g_world), 0x37) ||
            ((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)g_app + 0x2d2,
                                                                (const char*)Obj_Company21(g_world) + 4)) {
            F<uint32_t>(g_app, 0x305) = F<uint32_t>(Obj_Company21(g_world), 0x37);
            strcpy((char*)g_app + 0x2d2, (const char*)Obj_Company21(g_world) + 4);
            Options_Save(g_app, 0, 1);
        }
        if (trackAt()) Tutorial_Advance();
    }
    const int32_t g = goal();
    switch (g) {
    case 0x96:
        if (trackAt()) Tutorial_Advance();
        return 0;
    case 0xa0:
        if (Track_NodeNear(BitsF(0x424a0000), BitsF(0x43c68000))) Tutorial_Advance();
        return 0;
    case 0xaa:
        if (Track_NodeNear(BitsF(0x42850000), BitsF(0x43ca4ccd))) Tutorial_Advance();
        return 0;
    case 0xc3:
        if (Track_NodeNear(BitsF(0x42960000), BitsF(0x43ca4ccd)) || Track_NodeNear(BitsF(0x42850000), BitsF(0x43ca4ccd))) Tutorial_Advance();
        return 0;
    case 0xbe:
        if (Track_NodeNear(BitsF(0x41e00000), BitsF(0x43c20000)) && Track_NodeNear(BitsF(0x42960000), BitsF(0x43ca4ccd))) Tutorial_Advance();
        return 0;
    }
    if (g == 0xdc && Table_CountActive(g_buildings) >= 1) Tutorial_Advance();
    if (goal() == 0xe6 && Table_CountActive(g_buildings) >= 2) Tutorial_Advance();
    if (goal() == 0xfa && Records_CountClass3(g_buildings) >= 1) Tutorial_Advance();
    if (goal() == 0x118 && Records_CountClass4(g_buildings) >= 1) Tutorial_Advance();
    if (goal() == 0x136 && Table_CountKind4Idle(g_buildings) >= 1) Tutorial_Advance();
    if (goal() == 0x1f4 && g_6d3b58) Tutorial_Advance();
    if ((goal() == 0x69 || goal() == 0x339) && Obj_Float18(g_62be68) > (double)BitsF(0x461c4000)) Tutorial_Advance();
    if (goal() == 0x186 && Global6cfe00Set()) Tutorial_Advance();
    return 1;
}
RT3_REPLACE(0x516d90, TutorialWindow_HandleEvent);

// 0x5172c0. Starts the tutorial (unless its window 0x6d3b44 is up): resets
// its state, in the tutorial scenario (0x6cec7c +1 == 7) jumps to step 510
// and takes the player's name (0x6cec74 +0x2d2, with its +0x305 face, a
// first rival with the same face getting 0x17), sets the world flag +0x4ad7,
// reapplies the weather, opens the tutorial window with font 3 (0x6d3b40)
// and shows the first step.
RT3_NOINLINE void Tutorial_Start() {
    if (g_dialogWindow) return;
    g_6d3b60 = 0;
    g_622af4 = 0;
    g_6d3b5c = 0;
    g_6d12d8 = 3;
    g_6d3b58 = 0;
    g_6d3b4c = 1;
    g_6d3b50 = -1;
    if (F<int32_t>(g_newGame, 1) == 7) {
        g_6d3b50 = Tutorial_IndexOf(510.0f);
        const char* name = (const char*)(g_app + 0x2d2);
        if (strlen(name) > 1) {
            StrCopyN((char*)Obj_Company21(hook::As<void*>(g_world)) + 4, name, 0x32);
            int32_t face = F<int32_t>(g_app, 0x305);
            F<int32_t>(Obj_Company21(hook::As<void*>(g_world)), 0x37) = face;
            Collection* cos = g_companies;
            if (F<int32_t>(Collection_Get(cos, 0, Collection_IdAt(cos, 0, 1, 0)), 0x37) == F<int32_t>(g_app, 0x305)) {
                cos = g_companies;
                F<int32_t>(Collection_Get(cos, 0, Collection_IdAt(cos, 0, 1, 0)), 0x37) = 0x17;
            }
        }
    }
    F<int32_t>(g_world, 0x4ad7) = 1;
    Weather_ApplyIntensity(g_world);
    g_6d3b40 = Pack_AcquireFont(g_pack, 0, 3);
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x78);
    if (w) w = TutorialWindow_Construct(w);
    g_dialogWindow = w;
    WidgetList_Insert(hook::As<uint8_t**>(g_windowManager), 0, w, 1);
    Tutorial_Advance();
}
RT3_REPLACE(0x5172c0, Tutorial_Start);

// 0x517420. Resumes the music: re-arms the playlist (+0x110 when it has tracks),
// restarts the fade timer and plays.
RT3_NOINLINE void __cdecl Music_Resume() {
    uint8_t* p = F<uint8_t*>(g_app, 0x14);
    if (F<int32_t>(p, 0x100) > 0) F<int32_t>(p, 0x110) = 1;
    Sound_StartFadeTimer(g_sound, 0, 0);
    Playlist_Tick(F<uint8_t*>(g_app, 0x14));
}
RT3_REPLACE(0x517420, Music_Resume);

// 0x517460 (callback). Ends the scenario once (guard 0x6d3b6c): with high
// scores (0x6d4000) they are shown (event 0xa5), then the title screen is
// requested (0xcc / 2) and the music resumes.
RT3_NOINLINE void Scenario_Finish() {
    if (g_6d3b6c) return;
    g_6d3b6c = 1;
    if (g_6d4000) {
        HighScores_Show(g_newGame[0x7d], 2, 0);
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xa5, 0, 0, 0, 0);
    }
    Game_PostInputEvent(0xcc, 0, 2, 0, 0);
    Music_Resume();
    g_6d3b6c = 0;
}
RT3_REPLACE(0x517460, Scenario_Finish);

// 0x5174e0. Video window constructor: counts a video in progress (engine
// +0x11427a), remembers whether music was on (0x6d3b68 from 0x624308), UI
// busy, 640 x 480 display, layout Video.win with the player 0xc096
// (0x517460); 0x6d3b78 cleared.
uint8_t* __fastcall VideoWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1a4c;
    ++F<int32_t>(g_engine, 0x11427a);
    g_6d3b68 = g_624308 == 1;
    Obj_Incc60(g_windowManager);
    Engine_SetDisplayMode(g_engine, 0, 0x280, 0x1e0, -1, 0);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1a40);
    hook::As<uint32_t>(g_6d3b78) = 0;
    Window_SendEvent5(w, 0, 0x80, 0xc096, 0x517460, 0, 0);
    return w;
}
RT3_REPLACE(0x5174e0, VideoWindow_Construct);

// 0x517570. Video window destructor: video count down, the saved display
// size back, UI busy down; when music was on, option +0xa8 is cleared and the
// options saved.
void __fastcall VideoWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1a4c;
    --F<int32_t>(g_engine, 0x11427a);
    uint8_t* eng = g_engine;
    Engine_SetDisplayMode(eng, 0, F<int32_t>(eng, 0x11468a), F<int32_t>(eng, 0x11468e), -1, 0);
    Obj_Decc60(g_windowManager);
    if (g_6d3b68) {
        F<uint32_t>(g_app, 0xa8) = 0;
        Options_Save(g_app, 0, 1);
    }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x517570, VideoWindow_Destruct);

// 0x5175e0 (event handler). The scenario end window w: idle sends the
// pending widget update (0x624308, unless 0x6d3b74) and, once asked
// (0x6d3b78), finishes; keys and buttons finish at once (or, while 0x6d3b74
// holds it, ask for it).
int32_t __fastcall ScenarioEnd_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    const uint32_t type = msg[0];
    if (type == 0) {
        if (g_624308 != -1 && !g_6d3b74) {
            Window_SendEvent5(w, 0, 0x8c, 0xc096, hook::As<uint32_t>(g_624308), 0, 0);
            g_624308 = -1;
        }
        if (!g_6d3b78) return 1;
    } else if (type != 3 && type != 0xcb) {
        return 1;
    }
    if (g_6d3b74) {
        g_6d3b78 = 1;
        return 1;
    }
    Scenario_Finish();
    return 0;
}
RT3_REPLACE(0x5175e0, ScenarioEnd_HandleEvent);

// 0x517660. Win / lose screen constructor ("WinLose.win", vtable 0x5d1a58): the
// scenario's own end text (world +0x4b47, trimmed; long ones get a smaller font and
// wrap) or the stock text for outcome 0x6d3b80 (strings 0xd9..0xdc); outcome
// button 0xb990 + outcome, the win / lose button pairs, a length limit outside
// English, and 0x624308 = 2 (+1 unless outcome 0).
RT3_NOINLINE uint8_t* __fastcall WinLose_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1a58;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1a64);
    bool foreign = !(uint8_t)IsEnglish();
    const char* own = (const char*)WorldG() + 0x4b47;
    int32_t outcome = g_6d3b80;
    if (strlen(own) > 1) {
        char text[0x3e8];
        strcpy(text, own);
        TrimSpaces(text, 1);
        Window_SendEvent5(w, 0, 0x6f, 0xb98e, (uint32_t)(uintptr_t)text, 0, 0);
        if (uint8_t* f = Widget_FindChild(w, 0, 0xb98e)) {
            F<uint32_t>(f, 0x17) = 0x3e0e81b5;
            if (strlen(text) > 0x6e) f[0x2c] = 1;
        }
    } else {
        Window_SendEvent5(w, 0, 0x6f, 0xb98e, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x62430c)[outcome]), 0, 0);
    }
    for (int32_t i = 0; i < 4; ++i) Window_SendEvent5(w, 0, g_6d3b80 == i ? 0x6d : 0x86, 0xb990 + i, 3, 0, 0);
    bool won = g_6d3b80 != 0;
    Window_SendEvent5(w, 0, won ? 0x87 : 0x65, 0xb995, 3, 0, 0);
    Window_SendEvent5(w, 0, g_6d3b80 ? 0x65 : 0x87, 0xb994, 3, 0, 0);
    Window_SendEvent5(w, 0, g_6d3b80 ? 0x87 : 0x65, 0xb996, 3, 0, 0);
    Window_SendEvent5(w, 0, g_6d3b80 ? 0x65 : 0x87, 0xb98d, 3, 0, 0);
    if (foreign) Window_SendEvent5(w, 0, 0x78, 0xb98e, 1, 0, 0);
    g_624308 = (g_6d3b80 != 0) + 2;
    return w;
}
RT3_REPLACE(0x517660, WinLose_Construct);

// 0x517850 (event handler). Intro screen: buttons 0xb98d / 0xb996 close it,
// advancing the shown page (+0xc4 of 0x6cec7c, below 15 while +0xc5 is set;
// one step with 0x6d3b80, then past pages whose +0xc6 count exceeds 1, up to
// 16). Idle ticks fade the volumes over 1.5 s (0x6d3b7c): factor f (1 when
// done) scales 0x6d3b8c / 0x6d3b84 into the music / sound volumes when it
// moved more than 0x5c85b8, and a pending page (0x624308) is shown (0x8c,
// 0xb98f; 0x6cfd24 = page + 1).
int32_t __fastcall IntroScreen_HandleEvent(uint8_t* s, int, uint32_t* msg) {
    if (msg[0] == 0xc9) return 1;
    if (msg[0] == 0) {
        int32_t left = g_6d3b7c - ClockMs() + 0x5dc;
        if (left < 0)
            left = 0;
        else
            left = g_6d3b7c - ClockMs() + 0x5dc;
        float f = (float)((double)left * D(0x5c96e0) / D(0x5d1a70) + D(0x5c8c18));
        if (!(f < Fl(0x5c85b4))) f = 1.0f;
        if (((double(__cdecl*)(double))0x5a152e)((double)f - g_6d3b88) > D(0x5c85b8)) {
            g_6d3b88 = f;
            uint8_t* e = g_engine;
            F<float>(e, 0x1146fa) = hook::As<float>(g_6d3b8c) * f;
            F<float>(g_engine, 0x1146fe) = hook::As<float>(g_6d3b84) * f;
            Sound_SetVolume(g_sound, 0, F<float>(g_engine, 0x1146fa), 2);
            Sound_SetVolume(g_sound, 0, (float)((double)F<float>(g_engine, 0x1146fe) * D(0x5c9c80)), 0);
        }
        if (g_624308 != -1) {
            g_6cfd24 = g_newGame[0xc4] + 1;
            Window_SendEvent5(s, 0, 0x8c, 0xb98f, hook::As<uint32_t>(g_624308), 0, 0);
            g_624308 = -1;
        }
        return 1;
    }
    if (msg[0] != 0xcb || (msg[1] != 0xb98d && msg[1] != 0xb996)) return 1;
    uint8_t* p = g_newGame;
    if (p[0xc5] && p[0xc4] < 0xf) {
        if (g_6d3b80) {
            ++p[0xc4];
            p = g_newGame;
        }
        while (p[0xc6 + p[0xc4]] > 1 && p[0xc4] < 0x10) {
            ++p[0xc4];
            p = g_newGame;
        }
    }
    msg[0] = 0xc9;
    return 1;
}
RT3_REPLACE(0x517850, IntroScreen_HandleEvent);

// 0x517a80. The scenario is over at `level` (win / lose screen): the
// campaign's best level for this scenario (lobby +0xc6 + +0xc4) rises, the
// screen (0x517660, 0x78 bytes in 0x6d3b90) runs modally with the game
// paused, the music settings (+0x1146fa / +0x1146fe) are restored and, in a
// campaign (+0xc5), the game is saved as "After_NN.gmc" (localized 0xf85,
// "Nach_" in German, "Despues_" in Spanish) with the campaign state copied
// into the world (+0x66df..+0x66f1).
RT3_NOINLINE void __fastcall Scenario_ShowResult(int32_t level) {
    uint8_t* opt = g_engine;
    g_6d3b8c = F<uint32_t>(opt, 0x1146fa);
    g_6d3b84 = F<uint32_t>(opt, 0x1146fe);
    g_6d3b7c = ClockMs();
    hook::As<uint32_t>(g_6d3b88) = 0xbf800000;
    g_6d3b80 = level;
    {
        uint8_t* lobby = g_newGame;
        uint8_t* best = lobby + lobby[0xc4] + 0xc6;
        if (level + 1 > *best) *best = (uint8_t)(level + 1);
    }
    uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x78);
    g_6d3b90 = mem ? WinLose_Construct(mem) : nullptr;
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d3b90, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d3b90);
    if (uint8_t* w = g_6d3b90) {
        F<uint32_t>(w, 0) = 0x5d1a58;
        Widget_SetVtbl(w);
        ((void(__cdecl*)(void*))0x53b080)(w);
    }
    g_6d3b90 = nullptr;
    F<uint32_t>(g_engine, 0x1146fa) = g_6d3b8c;
    F<uint32_t>(g_engine, 0x1146fe) = g_6d3b84;
    if (!g_newGame[0xc5]) return;
    g_world[0x66df] = 1;
    g_world[0x66e0] = g_newGame[0xc4];
    for (int32_t k = 0; k < 0x10; ++k) g_world[0x66e1 + k] = g_newGame[0xc6 + k];
    g_world[0x66f1] = g_newGame[0x7d];
    char name[0xc4];
    memcpy(name, (const void*)0x5d1a98, 7);
    if (g_language == 1) memcpy(name, (const void*)0x5d1a90, 6);
    else if (g_language == 4) memcpy(name, (const void*)0x5d1a84, 9);
    if (HasString(0xf85)) strcpy(name, GetString(0xf85));
    char file[0x190];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(file, (const char*)0x5d1a78, name, (uint32_t)g_newGame[0xc4]);
    World_Save(g_world, 0, file, 0, nullptr);
    g_world[0x66df] = 0;
}
RT3_REPLACE(0x517a80, Scenario_ShowResult);

}  // namespace rt3
