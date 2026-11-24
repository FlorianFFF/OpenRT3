// CompanyList.cpp - RT3.exe 0x4c6000-0x4ca000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Rand(int32_t lo, int32_t hi);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
int32_t ClockMs();

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t,
                            float, float, float, float);

void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_Get25(void* o);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
void* __fastcall Obj_Company21(void* o);
void* __fastcall Company_HeadquartersSite(void* c);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetVtbl(uint8_t* w);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall Music_Close(uint8_t* m);
double __fastcall Scroll_Total(uint8_t* s);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);

int32_t __fastcall Font_MoveCaretLines(uint8_t* f, int, int32_t pos, int32_t dir, const char* text, float x, float y, float w, float h, uint32_t color,
                                       int32_t align, int32_t valign, uint32_t flags, float scale);

uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Company_GoBankrupt(uint8_t* c);
void __fastcall Obj_Set4b(void* o, int, int32_t v);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht);
void __fastcall CompanyWindow_Refresh(uint8_t* win);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip,
                            float left, float top, float right, float bottom);

void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
uint32_t __fastcall Sound_Get2c(uint8_t* s);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);

void __fastcall Dialog_Setup(uint8_t* w, int, const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                             void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), float extraH, uint32_t buttonText,
                             int32_t count, const char** choices, int32_t radio, int32_t check, int32_t* checked);

void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall CompanyList_Fill(uint8_t* w);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall Ticker_PickNext(uint8_t* t);
void* __fastcall Obj_Building(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall Obj_ShowMessageDialog(uint32_t id);
void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
uint8_t* __fastcall CreditsWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Chairman_Resign(int32_t* r);
void __fastcall Credits_Draw(uint8_t* o, int, float x, float w);
void __fastcall Ticker_Draw(uint8_t* t, int, float x, float y, float, float);
void __fastcall TextPager_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);

// Dialog callbacks clearing a range of their window's child widgets.
#define RT3_CLEAR_CHILDREN(addr, name, lo, hi) \
    void __fastcall name(uint8_t* w, int, int32_t) { Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, lo, hi, (uint32_t)(uintptr_t)w); } \
    RT3_REPLACE(addr, name);

void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
void __fastcall CompanyCard_Draw(uint8_t* co, int, float x, float y, float w, float h);
int32_t __fastcall Music_IsPlaying(uint8_t* m);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void Tutorial_Advance();
uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
void Player_FoundCompanyDialog();
int32_t Scenario_GoalIs3c();
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Company_GoBankrupt(uint8_t* c);
RT3_NOINLINE void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
RT3_NOINLINE void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
RT3_NOINLINE int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
RT3_NOINLINE void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void Player_FoundCompanyDialog();
RT3_NOINLINE void __fastcall CompanyWindow_Refresh(uint8_t* win);
RT3_NOINLINE void __fastcall Chairman_Resign(int32_t* r);
RT3_NOINLINE void __fastcall CompanyCmd_IssueBond(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_RepayBond(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_Bankrupt(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_IssueShares(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_BuyBack(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_SetDividend(uint32_t* cmd);
RT3_NOINLINE void __fastcall CompanyCmd_BuyTerritory(uint32_t* cmd);
void __fastcall CompanyList_Destruct(uint8_t* w);
void __fastcall CompanyList_DrawCb(void*, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall CompanyList_SelectCb(void*, uint32_t id);
RT3_NOINLINE void __fastcall CompanyList_Fill(uint8_t* w);
int32_t __fastcall CompanyList_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall CompanyListDialog_Construct(uint8_t* w);
void __fastcall CityDialog_DrawTitleCb(void*, int, float x, float y, float w, int32_t, int32_t);
void __fastcall CompanyList_AddMapControls(uint8_t* win, int, float x, float y, int32_t, int32_t);
RT3_NOINLINE void __fastcall Obj_ShowMessageDialog(uint32_t id);
int32_t __fastcall CreditsScreen_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall Credits_Draw(uint8_t* o, int, float x, float w);
RT3_NOINLINE void __fastcall Ticker_PickNext(uint8_t* t);
void __fastcall Credits_DrawCb(void*, int, float x, float, float w, float, float);
void __fastcall Credits_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Ticker_Draw(uint8_t* t, int, float x, float y, float, float);
void __fastcall Ticker_DrawCb(void*, int, float x, float y, float a, float b, float);
uint8_t* __fastcall CreditsWindow_Construct(uint8_t* w);
int32_t __fastcall MsgDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall TextPager_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void SetGlobal6cfeac();
RT3_NOINLINE int32_t Global6cfeb4Positive();
int32_t __fastcall Return1_4c8690(void*);
void __fastcall TextPager_DrawCb(void*, uint8_t* p, float x, float y, float w, float h, float);
void __fastcall Dialog_Setup(uint8_t* w, int, const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), float extraH, uint32_t buttonText, int32_t count, const char** choices, int32_t radio, int32_t check, int32_t* checked);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
uint8_t* __fastcall PortraitDialog_Construct(uint8_t* d);
void __fastcall SetupWindow_Destroy(uint8_t* w);
void __fastcall PlayerList_DrawRowCb(void*, int32_t slot, float x, float y, float w, float h, int32_t);
void __fastcall CityEditor_DrawKindGlyph(void*, int, float x, float y, int32_t, int32_t, int32_t);
RT3_NOINLINE int32_t Scenario_GoalIs3c();
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

static inline float Kf(uint32_t a) { return *(const float*)a; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

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

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// Two-list picker dialogs (left / right lists share the selection +0x78; the
// swap buttons are enabled by the side flag).
static void PickerDialog_Setup(uint8_t* w, uint32_t listA, uint32_t listB, uint32_t btnA, uint32_t btnB, int32_t right) {
    Window_SendEvent5(w, 0, 0xaa, listA, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, 0xaa, listB, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, right ? 0x86 : 0x6d, btnA, 3, 0, 0);
    Window_SendEvent5(w, 0, right ? 0x6d : 0x86, btnB, 3, 0, 0);
}

// Shared tail of the company commands below: the issuing player gets the
// normal cursor back and an open company window is refreshed.
static void CompanyCmd_Done(uint32_t* cmd) {
    if (Obj_Get25(hook::Global<uint8_t*>(0x6cec78)) == (int32_t)cmd[0]) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 0);
    if (hook::Global<uint8_t*>(0x6cfe4c)) CompanyWindow_Refresh(hook::Global<uint8_t*>(0x6cfe4c));
}

static uint8_t* CompanyCmd_Target(uint32_t* cmd) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, cmd[1])) return nullptr;
    return (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, cmd[1]);
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

// 0x4c66f0. Resignation r (+0 player, +4 company): the player stops being
// the company's chairman; for the local player the cursor resets, and the
// company window refreshes.
RT3_NOINLINE void __fastcall Chairman_Resign(int32_t* r) {
    if (Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, (uint32_t)r[1]))
        Company_DropChairman((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)r[1]), 0, r[0]);
    if (Obj_Get25(hook::Global<void*>(0x6cec78)) == r[0]) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 0);
    if (uint8_t* w = hook::Global<uint8_t*>(0x6cfe4c)) CompanyWindow_Refresh(w);
}
RT3_REPLACE(0x4c66f0, Chairman_Resign);

// 0x4c6760: issue a bond of cmd[2] for cmd[3] years at rate cmd[4] (float bits).
RT3_NOINLINE void __fastcall CompanyCmd_IssueBond(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd))
        ((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, uint32_t))&Company_IssueBond)(c, 0, (int32_t)cmd[2], (int32_t)cmd[3], 1, cmd[4]);
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c6760, CompanyCmd_IssueBond);

// 0x4c67d0: repay bond cmd[2].
RT3_NOINLINE void __fastcall CompanyCmd_RepayBond(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) Company_RemoveEntry(c, 0, (int32_t)cmd[2], 1);
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c67d0, CompanyCmd_RepayBond);

// Company commands (player cmd[0], company cmd[1], arguments from cmd[2]).
// 0x4c6840: declare bankruptcy.
RT3_NOINLINE void __fastcall CompanyCmd_Bankrupt(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) Company_GoBankrupt(c);
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c6840, CompanyCmd_Bankrupt);

// 0x4c68a0: issue cmd[2] shares at price cmd[3].
RT3_NOINLINE void __fastcall CompanyCmd_IssueShares(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) Company_IssueShares(c, 0, (int32_t)cmd[2], 0, (int32_t)cmd[3]);
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c68a0, CompanyCmd_IssueShares);

// 0x4c6910: buy back cmd[2] shares, at most the free float.
RT3_NOINLINE void __fastcall CompanyCmd_BuyBack(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) {
        int32_t n = (int32_t)cmd[2];
        Company_BuyBackShares(c, 0, n < Company_FreeShares(c) ? n : Company_FreeShares(c));
    }
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c6910, CompanyCmd_BuyBack);

// 0x4c6990: set the dividend (+0x4b).
RT3_NOINLINE void __fastcall CompanyCmd_SetDividend(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) Obj_Set4b(c, 0, (int32_t)cmd[2]);
    CompanyCmd_Done(cmd);
}
RT3_REPLACE(0x4c6990, CompanyCmd_SetDividend);

// 0x4c6a00. Company command: buy into territory cmd[2] (entry flag) for cmd[3]
// dollars, then the usual cursor / window refresh; the issuing player's
// purchase list (0x948c) is cleared and refilled.
RT3_NOINLINE void __fastcall CompanyCmd_BuyTerritory(uint32_t* cmd) {
    if (uint8_t* c = CompanyCmd_Target(cmd)) {
        Company_SetEntryFlag(c, 0, (int32_t)cmd[2], 1);
        Company_AddStat(c, 0, 0xd, (float)-(int32_t)cmd[3]);
    }
    CompanyCmd_Done(cmd);
    if (Obj_Get25(hook::Global<uint8_t*>(0x6cec78)) == (int32_t)cmd[0]) F<uint32_t>(Widget_FindChild(hook::Global<uint8_t*>(0x6cfe4c), 0, 0x948c), 0x117) = 0;
    if (hook::Global<uint8_t*>(0x6cfe4c)) Window_SendEvent5(hook::Global<uint8_t*>(0x6cfe4c), 0, 0xba, 0x948c, 0, 0, 0);
}
RT3_REPLACE(0x4c6a00, CompanyCmd_BuyTerritory);

// 0x4c6ae0. Company list dialog: releases its two pack images (0x6cfe74, 0x6cfe78).
void __fastcall CompanyList_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0914;
    void* a = hook::Global<void*>(0x6cfe74);
    hook::Global<int32_t>(0x6cfe70) = 0;
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, a);
    void* b = hook::Global<void*>(0x6cfe78);
    hook::Global<void*>(0x6cfe74) = nullptr;
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, b);
    hook::Global<void*>(0x6cfe78) = nullptr;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4c6ae0, CompanyList_Destruct);

// 0x4c6b40 (draw callback). Company `id` (EDX) draws its card; 0x7fff
// draws string 0x10a instead.
void __fastcall CompanyList_DrawCb(void*, uint32_t id, float x, float y, float w, float h, int32_t) {
    if (id == 0x7fff) {
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6cfe78), 0, GetString(0x10a), x, y, w, h, 0xffffffff, 0, 0, 0, 1.0f);
        return;
    }
    CompanyCard_Draw((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id), 0, x, y, w, h);
}
RT3_REPLACE(0x4c6b40, CompanyList_DrawCb);

// 0x4c6bb0 (list callback). Selects company `id` (EDX) in the panel; for
// another company the camera can fly to its headquarters (input flags 3).
void __fastcall CompanyList_SelectCb(void*, uint32_t id) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, id)) return;
    Panel_SetValue90(hook::Global<uint8_t*>(0x6d0818), 0, (int32_t)id);
    if (id == (uint32_t)Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) return;
    if (!(hook::Global<uint8_t*>(0x6d4018)[0xa8c] & 3)) return;
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
    if (!Company_HeadquartersSite(c)) return;
    Camera_SetModeClampZoom(hook::Global<uint8_t*>(0x62be68), 0, (int32_t)(uintptr_t)Obj_Building(Company_HeadquartersSite(c)), 0x4b0, 0x3e8);
}
RT3_REPLACE(0x4c6bb0, CompanyList_SelectCb);

// 0x4c6c30. Fills the company lists 0x2ee1 / 0x2ee2 with the active companies
// (0x62be10, +0x3f): "your company" (0x10b), "<name>" without a chairman (0x10c)
// or "<chairman>'s <name>" (0x10d), plus string 0x10e and, for a rival with a
// headquarters, 0xbb0; callbacks 0x4c6b40 / 0x4c6bb0 (events 0x7a / 0x79). A "none"
// row (data 0x7fff) unless the world forbids it (+0x4ad3; +0x4ad7 while the player
// has bought stock, +0x2a7; +0x4acb while the player has a company). Keeps the
// selection +0x78, clamped to the list.
RT3_NOINLINE void __fastcall CompanyList_Fill(uint8_t* w) {
    int32_t keep = F<int32_t>(w, 0x78);
    Window_SendEvent5(w, 0, 0x8b, 0x2ee1, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x2ee2, 0, 0, 0);
    for (int32_t i = 0; i < Table_CountField3f(hook::Global<Collection*>(0x62be10)); ++i) {
        uint8_t* c = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, i);
        char text[0x1e0];
        if (c == Obj_Company21(hook::Global<void*>(0x6cec78))) FormatText(text, GetString(0x10b), (const char*)c + 4, 0, 0, 0, 0, 0, 0, 0);
        else if (!F<uint32_t>(c, 0x3b)) FormatText(text, GetString(0x10c), (const char*)c + 4, 0, 0, 0, 0, 0, 0, 0);
        else {
            uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b));
            FormatText(text, GetString(0x10d), (const char*)p + 8, (const char*)c + 4, 0, 0, 0, 0, 0, 0);
        }
        strcat(text, GetString(0x10e));
        if (F<int32_t>(c, 0) != Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) && Company_HeadquartersSite(c)) strcat(text, GetString(0xbb0));
        Window_SendEvent5(w, 0, 0x88, 0x2ee1, 0, F<uint32_t>(c, 0), (uint32_t)(uintptr_t)text);
        Window_SendEvent5(w, 0, 0x88, 0x2ee2, 0, F<uint32_t>(c, 0), (uint32_t)(uintptr_t)text);
    }
    Window_SendEvent5(w, 0, 0x7a, 0x2ee1, 0x4c6b40, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x2ee2, 0x4c6b40, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x2ee1, 0x4c6bb0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x2ee2, 0x4c6bb0, 0, 0);
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    bool none = !F<int32_t>(world, 0x4ad3);
    if (none && F<int32_t>(world, 0x4ad7) && F<int32_t>(Obj_Ref25(world), 0x2a7) >= 1) none = false;
    if (none && F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4acb) && Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) none = false;
    if (none) {
        Window_SendEvent5(w, 0, 0x88, 0x2ee1, 0, 0x7fff, 0);
        Window_SendEvent5(w, 0, 0x88, 0x2ee2, 0, 0x7fff, 0);
    }
    F<int32_t>(w, 0x78) = keep;
    if (Table_CountField3f(hook::Global<Collection*>(0x62be10)) < 1) return;
    int32_t sel = F<int32_t>(w, 0x78);
    if (sel >= Table_CountField3f(hook::Global<Collection*>(0x62be10)) - 1) sel = Table_CountField3f(hook::Global<Collection*>(0x62be10)) - 1;
    F<int32_t>(w, 0x78) = sel;
    Window_SendEvent5(w, 0, 0x66, 0x2ee1, (uint32_t)sel, 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x2ee2, F<uint32_t>(w, 0x78), 0, 0);
}
RT3_REPLACE(0x4c6c30, CompanyList_Fill);

// 0x4c6f30 (event handler). The company list window d (page +0x78): the tabs
// 0x2ee5 / 0x2ee6 switch lists 0x2ee2 / 0x2ee1 (0x6cfe7c; the second not in a
// tutorial, string 0xe8c); a row click shows that company (its
// headquarters, the company view 8) or, for the "new company" row 0x7fff,
// founds one (the tutorial step 0x3c advances; with a company already,
// string 0x110 confirms first) and refills the list.
int32_t __fastcall CompanyList_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    auto tutorialMessage = []() {
        Dialog_RunMessage(GetString(0xe8c), 1, 0x3ef5c28f, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        return 0;
    };
    if (msg[0] == 0xcb) {
        if (msg[1] == 0x2ee5) {
            hook::Global<int32_t>(0x6cfe7c) = 0;
            Window_SendEvent5(d, 0, 0x66, 0x2ee2, F<uint32_t>(d, 0x78), 0, 0);
            Window_SendEvent5(d, 0, 0x6d, 0x2ee3, 3, 0, 0);
            Window_SendEvent5(d, 0, 0x86, 0x2ee4, 3, 0, 0);
            return 0;
        }
        if (msg[1] == 0x2ee6) {
            if (hook::Global<int32_t>(0x6d3b4c)) return tutorialMessage();
            hook::Global<int32_t>(0x6cfe7c) = 1;
            Window_SendEvent5(d, 0, 0x66, 0x2ee1, F<uint32_t>(d, 0x78), 0, 0);
            Window_SendEvent5(d, 0, 0x6d, 0x2ee4, 3, 0, 0);
            Window_SendEvent5(d, 0, 0x86, 0x2ee3, 3, 0, 0);
            return 0;
        }
        return 1;
    }
    if (msg[0] != 0xca || msg[1] < 0x2ee1 || msg[1] > 0x2ee2) return 1;
    const uint32_t company = msg[3];
    if (company != 0x7fff) {
        if (hook::Global<int32_t>(0x6d3b4c)) return tutorialMessage();
        if (!Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, company)) return 0;
        if (company != (uint32_t)Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) {
            void* co = Collection_Get(hook::Global<Collection*>(0x62be10), 0, company);
            if (Company_HeadquartersSite(co))
                Camera_SetModeClampZoom(hook::Global<uint8_t*>(0x62be68), 0, (int32_t)(uintptr_t)Obj_Building(Company_HeadquartersSite(co)), 0x4b0, 0x3e8);
        }
        View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 8, (int32_t)company);
        return 0;
    }
    if (Scenario_GoalIs3c()) Tutorial_Advance();
    if (!Player_CheckCanFound((uint8_t*)Obj_Ref25(hook::Global<uint8_t*>(0x6cec78)))) return 0;
    if (Obj_Company21(hook::Global<uint8_t*>(0x6cec78))) {
        char text[0x2b0];
        FormatText(text, GetString(0x110), (const char*)Obj_Company21(hook::Global<uint8_t*>(0x6cec78)) + 4, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr, nullptr);
        if (Dialog_RunMessage(text, 2, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) != 0x3f2) return 0;
    }
    Player_FoundCompanyDialog();
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c) == 7) CompanyList_Fill(d);
    return 0;
}
RT3_REPLACE(0x4c6f30, CompanyList_HandleEvent);

// 0x4c7200. Company list dialog constructor ("CompanyList.win", vtable 0x5d0914,
// kept in 0x6cfe70): logo image (0x6cfe74), font 3 (0x6cfe78), then the lists.
uint8_t* __fastcall CompanyListDialog_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0914;
    hook::Global<uint8_t*>(0x6cfe70) = w;
    F<int32_t>(w, 0x78) = -1;
    hook::Global<uint8_t*>(0x6cfe74) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5c9f98);
    hook::Global<uint8_t*>(0x6cfe78) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d0920);
    PickerDialog_Setup(w, 0x2ee1, 0x2ee2, 0x2ee3, 0x2ee4, hook::Global<int32_t>(0x6cfe7c));
    CompanyList_Fill(w);
    return w;
}
RT3_REPLACE(0x4c7200, CompanyListDialog_Construct);

// 0x4c72f0 (draw callback). Draws the edited city's name and size (+0x276,
// string 0x108) 0.075 high.
void __fastcall CityDialog_DrawTitleCb(void*, int, float x, float y, float w, int32_t, int32_t) {
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cfe80));
    char num[0x80], text[0x200];
    const char* n = Num_FormatSimple(num, 9, (double)F<int32_t>(c, 0x276), 0xb);
    FormatText(text, GetString(0x108), (const char*)(c + 0x356), n, 0, 0, 0, 0, 0, 0);
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cfe88), 0, text, x, (float)((double)y + D(0x5d0930)), w, BitsF(0x3d99999a), 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4c72f0, CityDialog_DrawTitleCb);

// 0x4c73c0. Company list dialog: adds the scrollbar (style 0xb0, range 3..9) and
// the overview map 0xb9f6 (showing city 0x6cfe80, tooltip string 0x109) at
// (x, y), then selects the map.
void __fastcall CompanyList_AddMapControls(uint8_t* win, int, float x, float y, int32_t, int32_t) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d0168));
    F<uint32_t>(def, 0xf) = 0x3e9ccccd;
    F<uint32_t>(def, 0x13) = 0x3ed11111;
    F<int32_t>(def, 0x61) = 3;
    F<float>(def, 0xb) = (float)((double)y + D(0x5d01e0));
    F<int32_t>(def, 0x75) = 9;
    F<int32_t>(def, 0x2f) = 0xb0;
    uint8_t* bar = (uint8_t*)ExeNew(0x111);
    if (bar) bar = Scrollbar_Construct(bar, 0, def);
    Window_AddChild(win, 0, bar, 0);
    uint8_t mdef[0x60];
    memset(mdef, 0, sizeof mdef);
    F<float>(mdef, 7) = (float)((double)x + Fl(0x5d0940));
    F<float>(mdef, 0xb) = (float)((double)y + D(0x5d0938));
    F<uint32_t>(mdef, 0xf) = 0x3e97ae14;
    F<uint32_t>(mdef, 0x13) = 0x3eca3d71;
    F<uint32_t>(mdef, 0x2f) = 0x2000000;
    F<uint16_t>(mdef, 0) = 0xb9f6;
    String_Assign(&F<char*>(mdef, 0x3f), GetString(0x109));
    uint8_t* map = (uint8_t*)ExeNew(0x123);
    if (map) map = MapView_Construct(map, 0, mdef);
    F<uint32_t>(map, 0x11b) = hook::Global<uint32_t>(0x6cfe80);
    Window_AddChild(win, 0, map, 0);
    Free(F<void*>(mdef, 0x3f));
    Window_SendEvent5(win, 0, 0xba, 0xb9f6, 0, 0, 0);
}
RT3_REPLACE(0x4c73c0, CompanyList_AddMapControls);

// 0x4c7520. With option +0x277 shows the message dialog for object `id` of
// 0x62bae0 (kept in 0x6cfe80, its font 2 held in 0x6cfe88 meanwhile, page
// built by 0x4c73c0 / 0x4c72f0); "go there" (0x3f2) points the camera at it.
RT3_NOINLINE void __fastcall Obj_ShowMessageDialog(uint32_t id) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x277)) return;
    hook::Global<uint32_t>(0x6cfe80) = id;
    hook::Global<uint8_t*>(0x6cfe88) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 2);
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 10, 0xbf800000, 0xbf800000, 0,
                                   (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4c73c0, 0x4c72f0, 0.5f, 0, 0x1e, 0, nullptr,
                                   nullptr, 0, 0, nullptr, 0);
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6cfe88));
    hook::Global<uint8_t*>(0x6cfe88) = nullptr;
    if (r == 0x3f2)
        if (uint8_t* o = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cfe80))) Camera_FollowObject(o, 0);
    hook::Global<uint32_t>(0x6cfe80) = 0;
}
RT3_REPLACE(0x4c7520, Obj_ShowMessageDialog);

// 0x4c75e0 (event handler). Credits screen d: plays its four tracks
// (0x621f30, bits +0x3ed70; all played stops the music) on the player
// 0x6cfea0 like the jukebox (20 s grace, 0x6cfea4 / 0x6cfea8); key 3 leaves
// (event 0xcc), 8 / 0xca scroll the text (+0x174) by 20000.
int32_t __fastcall CreditsScreen_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    switch (msg[0]) {
    case 0: {
        if (!hook::Global<uint8_t*>(0x6cfea0)) return 0;
        if (Music_IsPlaying(hook::Global<uint8_t*>(0x6cfea0)) || hook::Global<uint8_t>(0x6cfea4) != 1) {
            if (ClockMs() - hook::Global<int32_t>(0x6cfea8) > 0x4e20) hook::Global<uint8_t>(0x6cfea4) = 1;
            return 0;
        }
        if (F<int32_t>(d, 0x3ed70) == 0xf) {
            hook::Global<uint8_t>(0x6cfea4) = 0;
            hook::Global<int32_t>(0x6cfea8) = ClockMs();
            return 0;
        }
        int32_t r = Rand(0, 3);
        while (F<uint32_t>(d, 0x3ed70) & (1u << (r & 31))) r = Rand(0, 3);
        F<uint32_t>(d, 0x3ed70) |= 1u << (r & 31);
        char frame[0x204];
        char* file = frame + 4;
        FormatText(file, (const char*)0x5d05a8, *(const char* const*)(0x621f30 + r * 4), 0, 0, 0, 0, 0, 0, 0);
        const float vol = (float)((double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x114702) * D(0x5c8640));
        Music_SetVolume(hook::Global<uint8_t*>(0x6cfea0), 0, vol);
        void* digital = (void*)(uintptr_t)Sound_Get2c(hook::Global<uint8_t*>(0x6d402c));
        Music_Play(hook::Global<uint8_t*>(0x6cfea0), 0, file, digital, nullptr);
        hook::Global<uint8_t>(0x6cfea4) = 0;
        hook::Global<int32_t>(0x6cfea8) = ClockMs();
        return 0;
    }
    case 3:
        Game_PostInputEvent(0xcc, 0, 2, 0, 0);
        return 0;
    case 0xca:
        F<int32_t>(d, 0x174) -= 0x4e20;
        return 0;
    case 8:
        F<int32_t>(d, 0x174) += 0x4e20;
        return 0;
    default:
        return 1;
    }
}
RT3_REPLACE(0x4c75e0, CreditsScreen_HandleEvent);

// 0x4c78a0. Draws credits screen o scrolling up with time (since +0x174,
// restarted if the clock went back): its lines (+0x184, 0x404 bytes, kind
// +0x400: 1 headings, 2 names one line further, others plain; fonts
// 0x6cfe90..0x6cfe98) in a column at x of width w, then the closing logo
// (0x6cfe9c); posts event 0xcc once the logo has scrolled off the top.
RT3_NOINLINE void __fastcall Credits_Draw(uint8_t* o, int, float x, float w) {
    float y = (float)(ClockMs() - F<int32_t>(o, 0x174));
    if (y < *(const float*)0x5c8598) {
        F<int32_t>(o, 0x174) = ClockMs();
        y = 0.0f;
    }
    y = (float)(*(const float*)0x5d0990 - (double)y / *(const float*)0x5d0994);
    const char* line = (const char*)o + 0x184;
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ed6c); ++i, line += 0x404) {
        int32_t kind = *(const int32_t*)(line + 0x400);
        uint8_t* font;
        if (kind == 1) {
            font = hook::Global<uint8_t*>(0x6cfe90);
            Font_DrawTextBox(font, 0, line, x, y, w, 1.0f, 0xffffff00, 0, 1, 0, 1.0f);
        } else if (kind == 2) {  // the line gap stays added for the lines below
            y = (float)(Scroll_Total(hook::Global<uint8_t*>(0x6cfe94)) * *(const double*)0x5c8880 + y);
            font = hook::Global<uint8_t*>(0x6cfe94);
            Font_DrawTextBox(font, 0, line, x, y, w, 1.0f, 0xffffffff, 0, 1, 1, 1.0f);
        } else {
            y = (float)(Scroll_Total(hook::Global<uint8_t*>(0x6cfe98)) + y);
            font = hook::Global<uint8_t*>(0x6cfe98);
            Font_DrawTextBox(font, 0, line, x, y, w, 1.0f, 0xffffff00, 0, 1, 0, 1.0f);
        }
        y = (float)(Font_TextHeight(font, 0, line, w, nullptr) + y);
    }
    float ly = (float)((double)y + *(const float*)0x5d098c);
    float lx = (float)((double)x + *(const float*)0x5d0988);
    uint8_t* logo = (uint8_t*)(uintptr_t)Table_Entry(hook::Global<uint8_t*>(0x6cfe9c), 0, 0);
    Glyph_Queue(logo, 0, lx, ly, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    uint8_t* logo2 = (uint8_t*)(uintptr_t)Table_Entry(hook::Global<uint8_t*>(0x6cfe9c), 0, 0);
    float end = (float)(Slider_StepB(logo2) + y + *(const float*)0x5d0984);
    if (end < *(const float*)0x5c8598) Game_PostInputEvent(0xcc, 0, 2, 0, 0);
}
RT3_REPLACE(0x4c78a0, Credits_Draw);

// 0x4c7ad0. Picks the next two ticker entries (+0x78 values, +0x7c cool-downs of
// 31 slots) avoiding recent ones; the second is dropped when it would not fit.
RT3_NOINLINE void __fastcall Ticker_PickNext(uint8_t* t) {
    for (int k = 0; k < 0x1f; ++k)
        if (F<int32_t>(t, 0x7c + k * 8) > 0) F<int32_t>(t, 0x7c + k * 8)--;
    int32_t prev = -1;
    for (int n = 0; n < 2; ++n) {
        int32_t r;
        do {
            r = Rand(0, 0x1e);
        } while (F<int32_t>(t, 0x7c + r * 8) || r == prev);
        F<uint32_t>(t, 0x17c + n * 4) = F<uint32_t>(t, 0x78 + r * 8);
        prev = r;
        F<int32_t>(t, 0x7c + r * 8) = 5;
    }
    float room = (float)((double)*(const float*)0x5d0998 -
                         Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0)));
    double w = Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0));
    if (w > room || F<int32_t>(t, 0x178) == 1) F<int32_t>(t, 0x180) = 0;
    if (++F<int32_t>(t, 0x178) == 3) F<int32_t>(t, 0x178) = 0;
}
RT3_REPLACE(0x4c7ad0, Ticker_PickNext);

// Draw callbacks forwarding to a fixed window.
void __fastcall Credits_DrawCb(void*, int, float x, float, float w, float, float) { Credits_Draw(hook::Global<uint8_t*>(0x6cfe8c), 0, x, w); }
RT3_REPLACE(0x4c7ba0, Credits_DrawCb);

// 0x4c7bc0. Credits window ("Credits.win") destructor: releases its 31 images
// (+0x78, 8 bytes apart) and the four logos (0x6cfe90..9c), ends its modal count
// and closes its music 0x6cfea0.
void __fastcall Credits_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d099c;
    for (int k = 0; k < 0x1f; ++k) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(w, 0x78 + k * 8));
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6cfe90));
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6cfe94));
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6cfe98));
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6cfe9c));
    hook::Global<int32_t>(0x6cfe8c) = 0;
    Obj_Decc60(hook::Global<uint8_t*>(0x6d401c));
    hook::Global<uint8_t>(0x6cfea4) = 0;
    if (uint8_t* m = hook::Global<uint8_t*>(0x6cfea0)) {
        Music_Close(m);
        ExeDelete(m);
        hook::Global<uint8_t*>(0x6cfea0) = nullptr;
    }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4c7bc0, Credits_Destruct);

// 0x4c7c80. Draws the ticker at (x, y): picks new entries every 8 s (10 s with
// two), the first fading in over 2 s, holding to 6 s and out by 8 s; a second
// entry (+0x180) fades in at 2..4 s and out after 8 s, sitting below the first.
// Phase 2 (+0x178) swaps the two fades. Entries are centred on 0.375 and drawn
// from 0.95 up.
RT3_NOINLINE void __fastcall Ticker_Draw(uint8_t* t, int, float x, float y, float, float) {
    int32_t el = ClockMs() - F<int32_t>(t, 0x170);
    if (el > 8000 && (!F<int32_t>(t, 0x180) || el > 10000)) {
        Ticker_PickNext(t);
        F<int32_t>(t, 0x170) = ClockMs();
    }
    el = ClockMs() - F<int32_t>(t, 0x170);
    float fel = (float)el;
    float a1 = fel;
    if (!F<int32_t>(t, 0x180)) a1 = (float)(el < 8000 ? el : 8000);
    if (a1 < Kf(0x5c88e0)) a1 = (float)((double)a1 / Kf(0x5c88e0));
    else if (a1 < Kf(0x5c9b0c)) a1 = 1.0f;
    else if (a1 <= Kf(0x5ca1c4)) a1 = (float)(*(const double*)0x5c8770 - ((double)a1 - Kf(0x5c9b0c)) / Kf(0x5c88e0));
    else a1 = 0.0f;
    float a2 = fel;
    if (F<int32_t>(t, 0x180)) {
        if (a2 < Kf(0x5c88e0)) a2 = 0.0f;
        else if (!(a2 < Kf(0x5ca1c8)) && a2 < Kf(0x5ca1c4)) a2 = 1.0f;
        else if (a2 < Kf(0x5ca1c8)) a2 = (float)(((double)a2 - Kf(0x5c88e0)) / Kf(0x5c88e0));
        else a2 = (float)(*(const double*)0x5c8770 - ((double)a2 - Kf(0x5ca1c4)) / Kf(0x5c88e0));
    }
    float alpha = F<int32_t>(t, 0x178) == 2 && F<int32_t>(t, 0x180) ? a2 : a1;
    float x1 = (float)(((double)Kf(0x5d09b4) - Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0))) / Kf(0x5c878c) + x);
    float top = (float)((double)Kf(0x5d09b0) - Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0)));
    if (F<int32_t>(t, 0x180))
        top = (float)((double)top - (Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0)) - Kf(0x5d09ac)));
    y = (float)((double)top / Kf(0x5c878c) + y);
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0), 0, x1, y, *(uint32_t*)&alpha, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (!F<int32_t>(t, 0x180)) return;
    float alpha2 = F<int32_t>(t, 0x178) == 2 ? a1 : a2;
    float x2 = (float)(((double)Kf(0x5d09b4) - Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0))) / Kf(0x5c878c) + x);
    double h1 = Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0));
    float y2 = (float)(h1 + ((double)top - Kf(0x5d09a8)) / Kf(0x5c878c) + y);
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0), 0, x2, y2, *(uint32_t*)&alpha2, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f,
                1.0f);
}
RT3_REPLACE(0x4c7c80, Ticker_Draw);

void __fastcall Ticker_DrawCb(void*, int, float x, float y, float a, float b, float) { Ticker_Draw(hook::Global<uint8_t*>(0x6cfe8c), 0, x, y, a, b); }
RT3_REPLACE(0x4c7f90, Ticker_DrawCb);

// 0x4c7fc0. Credits window constructor (0x6cfe8c): layout Credits.win, the 31
// pictures image1..image31.imb (+0x78 pairs), the picture ticker (event 0x7a
// 0xeda, callback 0x4c7f90; timers +0x170..+0x180 from 0x6cfea8) and up to 250
// text lines from data\Language\Credits.ln (+0x184 + i*0x404, size +0x584:
// a leading "*N" sets it, "-" is a blank line; message 0x107 when missing),
// the fonts 0x13 / 4 / 0xf and logos.imb (0x6cfe90..0x6cfe9c), the scroller
// (0xedb, 0x4c7ba0) and, with 3D sound, the fade object 0x6cfea0.
// QUIRK: the original copies the size digit with a 1-byte strncpy into an
// uninitialised buffer before atoi, so stack residue could join the number;
// here the buffer is zeroed.
uint8_t* __fastcall CreditsWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d099c;
    F<int32_t>(w, 0x3ed6c) = 0;
    hook::Global<uint8_t*>(0x6cfe8c) = w;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d09f8);
    Obj_Incc60(hook::Global<uint8_t*>(0x6d401c));
    typedef int(__cdecl * Sprintf)(char*, const char*, ...);
    Sprintf sprintf_ = (Sprintf)0x5a19c4;
    char line[0xbb0];
    for (int32_t i = 1; i <= 0x1f; ++i) {
        sprintf_(line, (const char*)0x5d09ec, i);
        F<uint8_t*>(w, 0x78 + (i - 1) * 8) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, line);
        F<uint32_t>(w, 0x7c + (i - 1) * 8) = 0;
    }
    Window_SendEvent5(w, 0, 0x7a, 0xeda, 0x4c7f90, 0, 0);
    F<uint32_t>(w, 0x17c) = 0;
    F<uint32_t>(w, 0x180) = 0;
    hook::Global<int32_t>(0x6cfea8) = ClockMs();
    F<int32_t>(w, 0x174) = hook::Global<int32_t>(0x6cfea8);
    F<int32_t>(w, 0x170) = hook::Global<int32_t>(0x6cfea8);
    F<uint32_t>(w, 0x178) = 0;
    Ticker_PickNext(w);
    uint8_t* f = ((uint8_t*(__cdecl*)(const char*, const char*))0x5a1887)((const char*)0x5d09d0, (const char*)0x5c89e8);
    if (!f) {
        Message_Show(GetString(0x107), 1, 0);
        return w;
    }
    F<int32_t>(w, 0x3ed6c) = 0;
    while (!(f[0xc] & 0x10) && F<int32_t>(w, 0x3ed6c) < 0xfa) {
        ((int(__cdecl*)(uint8_t*, const char*, ...))0x5a17e1)(f, (const char*)0x5d09c8, line);
        uint8_t* rec = w + F<int32_t>(w, 0x3ed6c) * 0x404;
        if (line[0] == '*') {
            char digit[0xc] = {};
            ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(digit, line + 1, 1);
            F<int32_t>(rec, 0x584) = ((int(__cdecl*)(const char*))0x5a1ea5)(digit);
            memmove(line, line + 2, strlen(line + 2) + 1);
        } else
            F<int32_t>(rec, 0x584) = 1;
        if (line[0] == '-') sprintf_((char*)rec + 0x184, (const char*)0x5d09c4);
        else StrCopyN((char*)rec + 0x184, line, 1000);
        ++F<int32_t>(w, 0x3ed6c);
    }
    ((int(__cdecl*)(uint8_t*))0x5a1740)(f);
    hook::Global<uint8_t*>(0x6cfe90) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 0x13);
    hook::Global<uint8_t*>(0x6cfe94) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 4);
    hook::Global<uint8_t*>(0x6cfe98) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 0xf);
    hook::Global<uint8_t*>(0x6cfe9c) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d09b8);
    Window_SendEvent5(w, 0, 0x7a, 0xedb, 0x4c7ba0, 0, 0);
    F<uint32_t>(w, 0x3ed70) = 0;
    if (hook::Global<uint8_t*>(0x6d402c) && Sound_Is3D(hook::Global<uint8_t*>(0x6d402c))) {
        uint8_t* o = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x108);
        if (o) {
            F<uint32_t>(o, 0) = 0;
            F<uint32_t>(o, 0x104) = 0x3f800000;
        }
        hook::Global<uint8_t*>(0x6cfea0) = o;
    }
    return w;
}
RT3_REPLACE(0x4c7fc0, CreditsWindow_Construct);

// 0x4c8290 (event handler). Message box: the box's own filter (+0x9c) runs
// first. Closing (0xc9) passes with that filter's result (QUIRK: not
// normalised to 1); kind 4 boxes close on key 9. OK / list buttons (0x3f6 /
// 0x3f8) copy the chosen entry's text (0x2c +0xba) to 0x6cfeb0, then close
// unless in the menu (no world) the timer label 0x3f3 shows for under 30 ms;
// 0x3fa / 0x3fb page the box (+0x98). Kinds 5 / 0xb time out (0x6cfeac):
// idle ticks redraw the 3D view first (0x62be68 +0x1c while 0x114226 is
// set), stamp 0xd973e8 and answer 0x3f4. Answers 0x3f2..0x3f4 close the box,
// kinds 9 / 0xd copying the text of their OK / list button first.
int32_t __fastcall MsgDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    int32_t r = 1;
    if (auto filter = F<int32_t(__fastcall*)(uint8_t*, uint32_t*)>(d, 0x9c)) {
        r = filter(d, msg);
        if (!r) return 0;
    }
    const uint32_t type = msg[0];
    if (type == 0xc9) return r;
    const int32_t mode = F<int32_t>(d, 0x80);
    if (mode == 4) {
        if (type == 9) msg[0] = 0xc9;
        return 1;
    }
    if (type == 0xca) {
        switch (msg[1]) {
        case 0x3f6:
        case 0x3f8: {
            StrCopyN(hook::Global<char*>(0x6cfeb0), F<const char*>((void*)msg[0xb], 0xba), hook::Global<size_t>(0x621f40));
            uint8_t* timer = Widget_FindChild(d, 0, 0x3f3);
            if (!hook::Global<uint8_t*>(0x6cec78) && timer && Widget_IsVisible(timer) && ClockMs() - hook::Global<int32_t>(0xd973ec) < 0x1e) return 0;
            msg[0] = 0xc9;
            return 1;
        }
        case 0x3fa:
            ++F<int32_t>(d, 0x98);
            Window_DeleteChildren(d, 0, 0x3fa, 0x3fb, 0);
            return 0;
        case 0x3fb:
            --F<int32_t>(d, 0x98);
            Window_DeleteChildren(d, 0, 0x3fa, 0x3fb, 0);
            return 0;
        default:
            return 1;
        }
    }
    if (type == 0) {
        if (mode == 5 || mode == 0xb) {
            uint8_t* view = hook::Global<uint8_t*>(0x62be68);
            if (view && hook::Global<uint8_t*>(0x6d4024) && hook::Global<uint8_t*>(0x6d4024)[0x114226])
                ((void(__thiscall*)(uint8_t*))(*(void***)view)[7])(view);
        }
        if (!hook::Global<int32_t>(0x6cfeac)) return 1;
        if (F<int32_t>(d, 0x80) == 5 || F<int32_t>(d, 0x80) == 0xb) hook::Global<int32_t>(0xd973e8) = ClockMs();
        msg[1] = 0x3f4;
    } else if (type != 0xcb) {
        return 1;
    }
    if ((int32_t)msg[1] < 0x3f2 || (int32_t)msg[1] > 0x3f4) return 1;
    const int32_t kind = F<int32_t>(d, 0x80);
    if (kind == 9 || kind == 0xd) {
        const size_t n = hook::Global<size_t>(0x621f40);
        uint8_t* button = Widget_FindChild(d, 0, kind == 9 ? 0x3f6 : 0x3f8);
        StrCopyN(hook::Global<char*>(0x6cfeb0), F<const char*>(button, 0xba), n);
    }
    msg[0] = 0xc9;
    return 1;
}
RT3_REPLACE(0x4c8290, MsgDialog_HandleEvent);

// 0x4c84b0. Draws page +0x98 of the +0x94 pages of text panel p's text
// (+0x78, font +0x7c, scrolled up by +0x84) into the box (x, y, w, h): with
// more than one page, a page holds h / line height - 1 lines (4 fewer from
// 21 on when 0x6d4034 is set), its start is found by moving the caret that
// many lines per page (skipping line breaks at the cut), and the previous /
// next page links (strings 0xbb1 / 0xbb2, tips 0xbb3 / 0xbb4, ids 0x3fb /
// 0x3fa) take the bottom line.
RT3_NOINLINE void __fastcall TextPager_Draw(uint8_t* p, int, float x, float y, float w, float h) {
    int32_t start = 0;
    float bottomH = h;
    if (F<int32_t>(p, 0x94) > 1) {
        float lineH = (float)Scroll_Total(F<uint8_t*>(p, 0x7c));
        int32_t lines = (int32_t)((double)h / lineH) - 1;
        if (hook::Global<int32_t>(0x6d4034) && lines >= 0x15) lines -= 4;
        int32_t page = F<int32_t>(p, 0x98);
        float pages = (float)(page + 1);
        start = Font_MoveCaretLines(F<uint8_t*>(p, 0x7c), 0, 0, page * lines, F<const char*>(p, 0x78), x, y, w, (float)((double)pages * h), 0, 1, 0x100, 0,
                                    1.0f) -
                1;
        if (start < 0)
            start = 0;
        else if (start > 0) {
            const char* text = F<const char*>(p, 0x78);
            while ((text[start] == 0xa || text[start] == 0xd) && text[start + 1]) {
                ++start;
                if (start <= 0) break;
            }
        }
        float linkY = (float)((double)y + h - lineH);
        if (F<int32_t>(p, 0x98)) {
            const char* tip = GetString(0xbb3);
            TextLink_Draw(p, F<uint8_t*>(p, 0x7c), GetString(0xbb1), x, linkY, w, lineH, 2, 4, 0x101, tip, 0x3fb, 0, 0);
        }
        if (F<int32_t>(p, 0x98) < F<int32_t>(p, 0x94) - 1) {
            const char* tip = GetString(0xbb4);
            linkY = (float)((double)y + h - lineH);
            TextLink_Draw(p, F<uint8_t*>(p, 0x7c), GetString(0xbb2), x, linkY, w, lineH, 3, 4, 1, tip, 0x3fa, 0, 0);
        }
        bottomH = (float)((double)h - lineH);
    }
    Font_DrawTextBox(F<uint8_t*>(p, 0x7c), 0, F<const char*>(p, 0x78) + start, x, (float)((double)y - F<float>(p, 0x84)), w, bottomH, 0xffffffff, 0, 1,
                     0x100, 1.0f);
}
RT3_REPLACE(0x4c84b0, TextPager_Draw);

RT3_NOINLINE void SetGlobal6cfeac() { hook::Global<int32_t>(0x6cfeac) = 1; }
RT3_REPLACE(0x4c8670, SetGlobal6cfeac);

RT3_NOINLINE int32_t Global6cfeb4Positive() { return hook::Global<int32_t>(0x6cfeb4) > 0 ? 1 : 0; }
RT3_REPLACE(0x4c8680, Global6cfeb4Positive);

// 0x4c8690 (vtable stub).
int32_t __fastcall Return1_4c8690(void*) { return 0x1; }
RT3_REPLACE(0x4c8690, Return1_4c8690);

void __fastcall TextPager_DrawCb(void*, uint8_t* p, float x, float y, float w, float h, float) { TextPager_Draw(p, 0, x, y, w, h); }
RT3_REPLACE(0x4c86a0, TextPager_DrawCb);

// 0x4c86c0. Sets up the generic message dialog (DefaultDialog.win): the text
// (+0x78), its type (+0x80: which of the buttons 0x3f2..0x3f8 show and their
// captions / keys), the position (-1 for the default), the callback that
// places extra content, and `count` choices (`choices`): as buttons (one per
// choice, +0x88 radio / +0x8c check boxes with the states in +0x90) or else a
// scrolling list 0x3ef. The window grows by 1..4 rows to fit the text and
// choices; long texts get pages (+0x94).
RT3_NOINLINE void __fastcall Dialog_Setup(uint8_t* w, int, const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                             void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), float extraH, uint32_t buttonText,
                             int32_t count, const char** choices, int32_t radio, int32_t check, int32_t* checked) {
    bool big = count > 12;
    F<int32_t>(w, 0x90) = (int32_t)(uintptr_t)checked;
    F<int32_t>(w, 0x8c) = check;
    F<int32_t>(w, 0x88) = radio;
    F<uint8_t*>(w, 0x7c) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    F<int32_t>(w, 0x80) = type;
    if (type == 0xb) extraH = (float)((double)extraH + D(0x5d0aa8));
    else if (type == 0xd) extraH = (float)((double)extraH + D(0x5d08a0));
    else if (type == 9) extraH = (float)((double)extraH + D(0x5d0aa0));
    float lastTextW = 0.0f;
    double lastRowD = 0.0;
    if (count > 0) {
        if (!F<int32_t>(w, 0x88) && !F<int32_t>(w, 0x8c)) extraH = (float)((double)(big ? 0x234 : 0x130) / D(0x5ca2d0) + extraH);
        else {
            float acc = 0.0f;
            for (int32_t i = 0; i < (count > 10 ? 10 : count); ++i) {
                float th = (float)Font_TextHeight(F<uint8_t*>(w, 0x7c), 0, choices[i], BitsF(0x3ea6147b), &lastTextW);
                double thd = (double)th + D(0x5ca2d8);
                lastRowD = D(0x5caaf0) > thd ? BitsD(0x3fa1111111111111ull) : thd;
                acc = (float)((double)acc + lastRowD);
            }
            extraH = (float)((double)acc + extraH + D(0x5caaf0));
        }
    }
    int32_t buttons = type != 4 && type != 5 && type != 0xb;
    F<int32_t>(w, 0x84) = 0;
    F<int32_t>(w, 0x78) = 0;
    String_Assign(&F<char*>(w, 0x78), text);
    F<int32_t>(w, 0x9c) = p4;
    float textH = 0.0f;
    int32_t lines = 0;
    float minH = BitsF(0x3d9d036a);
    if (strlen(F<char*>(w, 0x78))) {
        textH = (float)Font_TextHeight(F<uint8_t*>(w, 0x7c), 0, F<char*>(w, 0x78), BitsF(0x3eb4cccd), nullptr);
        lines = Font_CountLines(F<uint8_t*>(w, 0x7c), 0, F<char*>(w, 0x78), BitsF(0x3eb4cccd), nullptr);
    }
    textH = (float)((double)textH + extraH);
    int32_t t = F<int32_t>(w, 0x80);
    if (buttons || t == 5 || t == 9 || t == 0xd || t == 0xb) minH = BitsF(0x3d0f5c29);
    int32_t rows = 0;
    if (minH < textH) {
        do {
            ++rows;
            float rf = (float)rows;
        if (!(((double)rf * Fl(0x5d0a9c) + minH) < textH)) break;
        } while (true);
        if (rows < 1) rows = 1;
        else if (rows > 4) rows = 4;
    } else
        rows = 1;
    uint32_t wx = px == 0xbf800000 ? 0x3e6e147b : px;
    if (py == 0xbf800000) {
        float r1 = (float)(rows - 1);
        py = FBits((float)(D(0x5d0a90) - (double)r1 * Fl(0x5d0a98)));
    }
    if (BitsF(py) <= D(0x5c8918)) py = 0x3ca3d70a;
    Window_Load(w, 0, BitsF(wx), BitsF(py), (const char*)0x5d0a7c);
    float r1 = (float)(rows - 1);
    float extra = (float)((double)r1 * Fl(0x5d0a9c));
    Widget_SetRect(w, 0, BitsF(0xbf800000), BitsF(0xbf800000), BitsF(0xbf800000), (float)((double)extra + F<float>(w, 0x11)));
    uint32_t ev[0xc];
    memset(ev, 0, sizeof ev);
    ev[0] = 0xb6;
    ev[1] = 0x3ec;
    ev[2] = 0;
    ev[3] = (uint32_t)Ftol((double)extra * Fl(0x5ca164));
    Window_HandleEvent(w, 0, ev);
    int32_t n = hook::Global<int32_t>(0x621f40) > 3 ? hook::Global<int32_t>(0x621f40) : 3;
    Window_SendEvent5(w, 0, 0xbd, 0x3f6, n, 0, 0);
    n = hook::Global<int32_t>(0x621f40) > 3 ? hook::Global<int32_t>(0x621f40) : 3;
    Window_SendEvent5(w, 0, 0xbd, 0x3f8, n, 0, 0);
    float boxH = (float)((double)extra + minH + Fl(0x5d0a9c));
    F<float>(Widget_FindChild(w, 0, 0x3ed), 0x17) = boxH;
    Window_SendEvent5(w, 0, 0x9f, 0x3ed, (uint32_t)(uintptr_t)w, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x3ed, 0x4c86a0, 0, 0);
    F<int32_t>(w, 0x94) = 1;
    F<int32_t>(w, 0x98) = 0;
    float lineH = (float)Scroll_Total(F<uint8_t*>(w, 0x7c));
    int32_t per = Ftol((double)boxH / lineH);
    if (hook::Global<int32_t>(0x6d4034) && per >= 0x15) per -= 4;
    if (strlen(F<char*>(w, 0x78)) && lines > per) {
        int32_t pages = F<int32_t>(w, 0x94), rem = lines;
        do {
            if (pages == 1) {
                --per;
                if (per < 1) per = 1;
            }
            rem -= per;
            ++pages;
        } while (rem > per);
        F<int32_t>(w, 0x94) = pages;
    }
    for (int32_t i = 0; i < rows - 1; ++i) {
        uint8_t s[0x89];
        memset(s, 0, sizeof s);
        float fi = (float)(i + 1);
        F<uint32_t>(s, 7) = 0x3c23d70a;
        F<float>(s, 0xb) = (float)((double)fi * Fl(0x5d0a9c) + D(0x5c9cb0) - D(0x5cab60));
        String_Assign(&F<char*>(s, 0x43), (const char*)0x5d0a68);
        F<uint16_t>(s, 0x5f) = 1;
        uint8_t* b = (uint8_t*)ExeNew(0x111);
        b = b ? Scrollbar_Construct(b, 0, s) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(s, 0x43));
    }
    auto send = [&](uint32_t e, uint32_t id, uint32_t a, uint32_t b2 = 0) { Window_SendEvent5(w, 0, e, id, a, b2, 0); };
    auto str = [](int32_t id) { return (uint32_t)(uintptr_t)GetString(id); };
    send(0x87, 0x3f6, 3);
    send(0x87, 0x3f5, 3);
    send(0x87, 0x3f8, 3);
    send(0x87, 0x3f7, 3);
    switch (F<int32_t>(w, 0x80)) {
    case 1:
    case 6:
        send(0x87, 0x3f2, 3);
        send(0x87, 0x3f4, 3);
        send(0x6f, 0x3f3, str(F<int32_t>(w, 0x80) == 6 ? 0xa : 9));
        send(0x8d, 0x3f3, 0xd);
        send(0x8d, 0x3f3, 0x1b);
        break;
    case 2:
    case 7:
        send(0x87, 0x3f3, 3);
        send(0x6f, 0x3f2, str(F<int32_t>(w, 0x80) == 7 ? 0xe : 9));
        send(0x6f, 0x3f4, str(0xa));
        send(0x8d, 0x3f2, 0xd);
        send(0x8d, 0x3f2, 0x4f);
        send(0x8d, 0x3f4, 0x1b);
        send(0x8d, 0x3f4, 0x43);
        break;
    case 3:
        send(0x87, 0x3f3, 3);
        send(0x6f, 0x3f2, str(0xb));
        send(0x8d, 0x3f2, 0xd);
        send(0x8d, 0x3f2, 0x59);
        send(0x6f, 0x3f4, str(0xc));
        send(0x8d, 0x3f4, 0x1b);
        send(0x8d, 0x3f4, 0x4e);
        break;
    case 4:
        send(0x87, 0x3f2, 3);
        send(0x87, 0x3f4, 3);
        send(0x87, 0x3f3, 3);
        break;
    case 5:
    case 9:
        send(0x65, 0x3f6, 3);
        send(0x65, 0x3f5, 3);
        send(0x6f, 0x3f6, buttonText);
        send(0x87, 0x3f2, 3);
        send(0x87, 0x3f4, 3);
        send(0x87, 0x3f3, 3);
        if (F<int32_t>(w, 0x80) == 9) {
            send(0xb5, 0x3f6, 0, (uint32_t)-0x46);
            send(0xb5, 0x3f5, 0, (uint32_t)-0x46);
            send(0x65, 0x3f3, 3);
            send(0x6f, 0x3f3, str(9));
            send(0x8d, 0x3f3, 0xd);
            send(0x8d, 0x3f3, 0x1b);
        }
        break;
    case 10:
        send(0x87, 0x3f3, 3);
        send(0x6f, 0x3f2, str(0x106));
        send(0x8d, 0x3f2, 0xd);
        send(0x8d, 0x3f2, 0x56);
        send(0x6f, 0x3f4, str(0xd));
        send(0x8d, 0x3f4, 0x1b);
        send(0x8d, 0x3f4, 0x45);
        break;
    case 11:
        send(0x65, 0x3f8, 3);
        send(0x65, 0x3f7, 3);
        send(0x6f, 0x3f8, buttonText);
        send(0x87, 0x3f2, 3);
        send(0x87, 0x3f4, 3);
        send(0x87, 0x3f3, 3);
        break;
    case 12:
        send(0x87, 0x3f3, 3);
        send(0x6f, 0x3f2, str(0x105));
        send(0x8d, 0x3f2, 0xd);
        send(0x6f, 0x3f4, str(0xa));
        send(0x8d, 0x3f4, 0x1b);
        break;
    case 13:
        send(0x65, 0x3f8, 3);
        send(0x65, 0x3f7, 3);
        send(0x6f, 0x3f8, buttonText);
        send(0xb5, 0x3f7, 0, (uint32_t)-0x5a);
        send(0xb5, 0x3f8, 0, (uint32_t)-0x5a);
        send(0x6f, 0x3f3, str(9));
        send(0x8d, 0x3f3, 0xd);
        send(0x8d, 0x3f3, 0x1b);
        send(0x87, 0x3f2, 3);
        send(0x87, 0x3f4, 3);
        send(0x65, 0x3f3, 3);
        break;
    default: break;
    }
    if (F<int32_t>(w, 0x88) || F<int32_t>(w, 0x8c)) {
        if (count > 0) {
            float maxW = BitsF(0x3d99999a);
            float total = 0.0f;
            float textW;
            for (int32_t i = 0; i < (count > 10 ? 10 : count); ++i) {
                float th = (float)Font_TextHeight(F<uint8_t*>(w, 0x7c), 0, choices[i], BitsF(0x3ea6147b), &textW);
                double m = maxW;
                double wd = (double)textW + D(0x5d0700);
                maxW = (float)(m > wd ? m : wd);
                total = (float)((double)th + total);
            }
            maxW = (float)((double)maxW + D(0x5d0a60));
            maxW = (float)((double)maxW - D(0x5d0a58));
            float y0 = (float)((D(0x5d0a50) - total) + extra);
            for (int32_t i = 0; i < (count > 10 ? 10 : count); ++i) {
                float th = (float)Font_TextHeight(F<uint8_t*>(w, 0x7c), 0, choices[i], BitsF(0x3ea6147b), &textW);
                double thd = (double)th + D(0x5ca2d8);
                float h = (float)(D(0x5caaf0) > thd ? BitsD(0x3fa1111111111111ull) : thd);
                uint8_t b[0xa1];
                memset(b, 0, sizeof b);
                float half = (float)(((double)h - D(0x5caaf0)) * D(0x5c8628));
                F<float>(b, 0xb) = y0;
                F<float>(b, 0x13) = h;
                F<uint16_t>(b, 0) = (uint16_t)(0x406 + i);
                F<float>(b, 7) = (float)((double)maxW * D(0x5ca180) + D(0x5d0a48));
                F<float>(b, 0xf) = (float)((double)maxW + Fl(0x5d0a44));
                if (F<int32_t>(w, 0x88)) F<uint32_t>(b, 0x85) = 0x3f0;
                String_Assign(&F<char*>(b, 0x43), (const char*)0x5d0a30);
                b[0x29] = 0x24;
                F<int32_t>(b, 0x6b) = F<int32_t>(w, 0x8c) ? 2 : 0;
                F<int32_t>(b, 0x67) = F<int32_t>(w, 0x8c) ? 3 : 1;
                String_Assign(&F<char*>(b, 0x6f), choices[i]);
                F<float>(b, 0x63) = half;
                b[0x33] = 3;
                F<int32_t>(b, 0x34) = -1;
                if (F<int32_t>(w, 0x8c)) {
                    F<uint32_t>(b, 0x75) = 0x3ce66666;
                    F<uint32_t>(b, 0x79) = 0;
                } else {
                    F<uint32_t>(b, 0x75) = 0x3cb851ec;
                    F<uint32_t>(b, 0x79) = 0xbba3d70a;
                }
                F<float>(b, 0x81) = h;
                F<float>(b, 0x7d) = (float)((double)maxW + Fl(0x5d0a2c));
                uint8_t* bt = (uint8_t*)ExeNew(0xe8);
                bt = bt ? Button_Construct(bt, 0, b) : nullptr;
                Window_AddChild(w, 0, bt, 0);
                Free(F<void*>(b, 0x43));
                Free(F<void*>(b, 0x6f));
                y0 = (float)((double)y0 + h);
                if (F<int32_t>(w, 0x8c) && F<int32_t*>(w, 0x90)[i]) send(0x74, 0x406 + i, 1);
            }
            if (F<int32_t>(w, 0x88)) send(0x74, 0x406, 1);
        }
    } else if (count > 0) {
        int32_t bump = big ? 0x12c : 0;
        float lh = (float)(bump + 0x108);
        {
            uint8_t s[0x89];
            memset(s, 0, sizeof s);
            F<uint32_t>(s, 7) = 0x3dd0a3d7;
            F<uint32_t>(s, 0xb) = 0x3e64b17e;
            F<uint32_t>(s, 0xf) = 0x3e8f5c29;
            F<int32_t>(s, 0x61) = 6;
            F<int32_t>(s, 0x75) = 9;
            F<uint32_t>(s, 0x69) = 0xff1a5230;
            F<float>(s, 0x13) = (float)((double)(bump + 0xfe) / D(0x5ca2d0));
            uint8_t* b = (uint8_t*)ExeNew(0x111);
            b = b ? Scrollbar_Construct(b, 0, s) : nullptr;
            Window_AddChild(w, 0, b, 0);
        }
        uint8_t tv[0x12f];
        memset(tv, 0, sizeof tv);
        int32_t visible = big ? 0x12 : 6;
        F<int32_t>(tv, 0x5f) = visible;
        F<uint32_t>(tv, 7) = 0x3dd33333;
        F<uint32_t>(tv, 0xb) = 0x3e606d3a;
        F<uint32_t>(tv, 0xf) = 0x3e9e6666;
        tv[0x33] = 2;
        F<uint16_t>(tv, 0) = 0x3ef;
        F<uint32_t>(tv, 0x63) = 0x3ba3d70a;
        F<uint32_t>(tv, 0x67) = 0x3c3f258c;
        F<uint32_t>(tv, 0x6b) = 0x3e8947ae;
        F<float>(tv, 0x13) = (float)((double)lh / D(0x5ca2d0));
        int32_t vis = visible >= 1 ? visible : 1;
        float span = (float)(bump - visible * 3 + 0xef);
        F<int32_t>(tv, 0x9f) = 1;
        F<int32_t>(tv, 0xa3) = 1;
        F<int32_t>(tv, 0xc7) = 1;
        float vf = (float)vis;
        double rowH = (double)span / vf;
        F<uint32_t>(tv, 0x8b) = 0x3e93851f;
        F<uint32_t>(tv, 0x93) = 0x3e93851f;
        F<int32_t>(tv, 0xd7) = big ? 7 : 5;
        F<uint32_t>(tv, 0x73) = 0x3b23d70a;
        F<uint32_t>(tv, 0x77) = 0x3b4ccccd;
        F<uint32_t>(tv, 0x7b) = 0x3d088889;
        F<uint32_t>(tv, 0x83) = 0x3e9428f6;
        F<uint32_t>(tv, 0x87) = 0x3ce147ae;
        F<uint32_t>(tv, 0x8f) = 0;
        F<uint32_t>(tv, 0xc3) = 0;
        F<int32_t>(tv, 0xcb) = 2;
        F<int32_t>(tv, 0xcf) = 3;
        F<int32_t>(tv, 0xd3) = 4;
        F<float>(tv, 0x6f) = (float)(rowH / D(0x5ca2d0));
        F<float>(tv, 0x7f) = (float)((double)(bump + 0xe3) / D(0x5ca2d0));
        F<float>(tv, 0x97) = (float)((double)(bump + 0xe7) / D(0x5ca2d0));
        String_Assign(&F<char*>(tv, 0x43), (const char*)0x5d0a10);
        uint8_t* b = (uint8_t*)ExeNew(0x1ea);
        b = b ? TreeView_Construct(b, 0, tv) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(tv, 0x43));
        for (int32_t i = 0; i < count; ++i) send(0x88, 0x3ef, (uint32_t)(uintptr_t)choices[i]);
        send(0x66, 0x3ef, 0);
    }
    if (place) {
        uint8_t* c = Widget_FindChild(w, 0, 0x3ed);
        double off = buttons ? BitsD(0x3fa5555555555555ull) : 0.0;
        float rf = (float)rows;
        float cy = (float)((double)rf * Fl(0x5d0a9c) + minH - off);
        place(w, 0, F<uint32_t>(c, 0xb), F<uint32_t>(c, 0xf), 0x3eb4cccd, cy);
    }
    (void)lastRowD;
}
RT3_REPLACE(0x4c86c0, Dialog_Setup);

// 0x4c98a0. Shows message box `text` of `type` modally and returns how it
// was closed: builds the 0xa0-byte dialog (Dialog_Setup with position, layout
// callback, extra height, button text 0x6cfeb0 = buttonText, the choices and
// radio / check options), adds picture `pic` over slot 0x3ed, pauses the game
// (0x6cec74) and hides the 3D cursor for types 5 / 0xb, runs it (focus 0x3f6
// for types 5 / 9, 0x3f8 for 0xb / 0xd, timeout ms), then reads back the
// ticked boxes 0x406.. into checked[] and the chosen radio / list entry into
// *selected, and frees the dialog. The cursor is restored afterwards when the
// view mode is unchanged.
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout) {
    hook::Global<uint32_t>(0x621f40) = a8;
    hook::Global<uint32_t>(0x6cfeb0) = buttonText;
    int32_t mode = hook::Global<uint8_t*>(0x6d0818) ? F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c) : 0;
    int32_t cursor = F<int32_t>(hook::Global<uint8_t*>(0xccbb20), 4);
    Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 1);
    hook::Global<uint32_t>(0x6cfeac) = 0;
    uint8_t* dlg = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0xa0);
    if (dlg) {
        Widget_SetVtblRet(dlg);
        F<uint32_t>(dlg, 0) = 0x5d0a04;
    }
    Dialog_Setup(dlg, 0, text, type, px, py, p4, place, extraH, buttonText, count, choices, radio, check, checked);
    if (pic) {
        uint8_t* slot = Widget_FindChild(dlg, 0, 0x3ed);
        uint8_t d[0x89];
        memset(d, 0, sizeof d);
        F<uint32_t>(d, 7) = F<uint32_t>(slot, 0xb);
        F<uint32_t>(d, 0xb) = F<uint32_t>(slot, 0xf);
        F<uint32_t>(d, 0xf) = F<uint32_t>(slot, 0x13);
        F<uint32_t>(d, 0x13) = F<uint32_t>(slot, 0x17);
        F<uint16_t>(d, 0) = 0x3ee;
        F<uint32_t>(d, 0x2f) = 0xb0;
        uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x111);
        w = w ? Scrollbar_Construct(w, 0, d) : nullptr;
        Window_AddChild(dlg, 0, w, 0);
        Window_SendEvent5(dlg, 0, 0x7a, F<uint16_t>(d, 0), (uint32_t)pic, 0, 0);
    }
    int32_t cursor2 = F<int32_t>(hook::Global<uint8_t*>(0xccbb20), 4);
    if (type == 5 || type == 0xb) {
        uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
        uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
        if (cam && eng && eng[0x114226]) ((void(__thiscall*)(uint8_t*))(*(void***)cam)[0x1c / 4])(cam);
        Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 0, 0, 0);
    }
    PauseLock_Push(hook::Global<int32_t*>(0x6cec74));
    int32_t focus = 0;
    if (type == 5 || type == 9) focus = 0x3f6;
    if (type == 0xb || type == 0xd) focus = 0x3f8;
    ++hook::Global<int32_t>(0x6cfeb4);
    uint32_t result = Ui_RunModal(hook::Global<uint8_t**>(0x6d401c), 0, dlg, 1, timeout, 0x14, focus);
    --hook::Global<int32_t>(0x6cfeb4);
    PauseLock_Pop(hook::Global<int32_t*>(0x6cec74));
    if (check)
        for (int32_t i = 0; i < 10; ++i) {
            uint8_t* b = Windows_FindChild(hook::Global<uint8_t*>(0x6d401c), 0, 0x406 + i);
            if (b && F<int32_t>(b, 0xcc)) checked[i] = 1;
        }
    if (selected) {
        *selected = 0;
        if (radio) {
            for (int32_t i = 0; i < 10; ++i) {
                uint8_t* b = Windows_FindChild(hook::Global<uint8_t*>(0x6d401c), 0, 0x406 + i);
                if (b && F<int32_t>(b, 0xcc)) *selected = i;
            }
        } else if (uint8_t* l = Windows_FindChild(hook::Global<uint8_t*>(0x6d401c), 0, 0x3ef))
            *selected = F<int32_t>(l, 0x1d6);
    }
    if (type == 5 || type == 0xb) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, cursor2, 0, 0);
    Windows_Remove(hook::Global<uint8_t*>(0x6d401c), 0, dlg);
    if (dlg) {
        F<uint32_t>(dlg, 0) = 0x5d0a04;
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(dlg, 0x7c));
        Free(F<void*>(dlg, 0x78));
        Widget_SetVtbl(dlg);
        ((void(__cdecl*)(void*))0x53b080)(dlg);
    }
    uint8_t* view = hook::Global<uint8_t*>(0x6d0818);
    if (view && mode == F<int32_t>(view, 0x8c) && F<int32_t>(hook::Global<uint8_t*>(0xccbb20), 4) != 6)
        Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, cursor, 0, 1);
    return result;
}
RT3_REPLACE(0x4c98a0, Dialog_RunMessage);

// 0x4c9c10. Constructs the dialog kept in 0x6cff84 (vtable 0x5d0ac4): clears the
// status line, takes fonts 3 / 2 / 1 (0x6cff88..0x6cff90) and the 41 portraits
// "portraitL%02d01.imb" (0x6cfed0); no selection (+0x84 = -1).
uint8_t* __fastcall PortraitDialog_Construct(uint8_t* d) {
    Widget_SetVtblRet(d);
    F<uint32_t>(d, 0) = 0x5d0ac4;
    hook::Global<uint8_t*>(0x6cff84) = d;
    Status_SetMessage(hook::Global<uint8_t*>(0x6d401c), 0, (const char*)0x5c87a8);
    hook::Global<uint8_t*>(0x6cff88) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    hook::Global<uint8_t*>(0x6cff8c) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 2);
    hook::Global<uint8_t*>(0x6cff90) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 1);
    for (int32_t i = 0; i < 0x29; ++i) {
        char name[0x40];
        ExeSprintf()(name, (const char*)0x5d0ab0, i);
        ((uint8_t**)0x6cfed0)[i] = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, name);
    }
    F<int32_t>(d, 0x84) = -1;
    return d;
}
RT3_REPLACE(0x4c9c10, PortraitDialog_Construct);

// 0x4c9cc0. Destroys the game-setup window: releases its cached art (0x6cff88..
// 0x6cff90 and the 41 slots 0x6cfed0..), refreshes which industry types are
// available (the original 0x412c10, which stays unhooked) and becomes a plain widget.
void __fastcall SetupWindow_Destroy(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0ac4;
    // Each slot is read before the previous one is cleared, as in the original.
    for (uint32_t g = 0x6cff88; g <= 0x6cff90; g += 4) {
        void* v = hook::Global<void*>(g);
        hook::Global<void*>(g - 4) = nullptr;
        if (v) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, v);
    }
    hook::Global<void*>(0x6cff90) = nullptr;
    for (uint32_t* p = (uint32_t*)0x6cfed0; (uint32_t)(uintptr_t)p < 0x6cff74; ++p) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, (void*)(uintptr_t)*p);
    ((void(__fastcall*)(void*))0x412c10)(hook::Global<void*>(0x62b268));
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4c9cc0, SetupWindow_Destroy);

RT3_CLEAR_CHILDREN(0x4c9d60, Dialog_ClearChildren5208, 0x5208, 0x61a6)

RT3_CLEAR_CHILDREN(0x4c9d80, Dialog_ClearChildrenA028, 0xa028, 0xa40f)

// 0x4c9da0 (draw callback). Lobby player row `slot` (EDX; world 0x6cec78
// +0x69d8, 9 bytes each): "number. kind" (strings 0xe01 with the kind's
// name, pattern 0x5d0ad4), then ready / not ready (0xbb5 / 0xbb6) and the
// connection state (0xbb7 / 0xbb8 with +2 and +3, else 0xbb9).
void __fastcall PlayerList_DrawRowCb(void*, int32_t slot, float x, float y, float w, float h, int32_t) {
    const uint8_t* row = hook::Global<uint8_t*>(0x6cec78) + 0x69d8 + slot * 9;
    const char* kind = GetString(*(const int32_t*)(0x5f2d47 + (int8_t)row[0] * 0x27));
    char frame[0x148];
    char* text = frame + 8;
    char* num = frame + 0x108;
    Num_Format(num, 1, (double)(slot + 1), 10, 1, 0, 0);
    FormatText(text, (const char*)0x5d0ad4, GetString(0xe01), num, kind, 0, 0, 0, 0, 0);
    row = hook::Global<uint8_t*>(0x6cec78) + 0x69d8 + slot * 9;
    strcat(text, GetString(row[1] ? 0xbb5 : 0xbb6));
    row = hook::Global<uint8_t*>(0x6cec78) + 0x69d8 + slot * 9;
    strcat(text, GetString(row[2] ? (row[3] ? 0xbb7 : 0xbb8) : 0xbb9));
    const float tw = (float)((double)w - Fl(0x5d0ad0));
    const float tx = (float)((double)x + Fl(0x5d0ad0));
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, tx, y, tw, h, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x4c9da0, PlayerList_DrawRowCb);

// 0x4c9fa0 (draw callback). Draws the glyph (table 0x6cfed0 by the class of
// the city editor's chosen kind, child 0x5a7e +0x4f, 39-byte records at
// 0x5f2d29) at (x, y).
void __fastcall CityEditor_DrawKindGlyph(void*, int, float x, float y, int32_t, int32_t, int32_t) {
    uint8_t* w = Widget_FindChild(hook::Global<uint8_t*>(0x6cff84), 0, 0x5a7e);
    const uint8_t k = *(const uint8_t*)(0x5f2d29 + F<int32_t>(w, 0x4f) * 0x27);
    uint8_t* g = (uint8_t*)(uintptr_t)Table_Entry(((uint8_t**)0x6cfed0)[k], 0, 0);
    Glyph_Queue(g, 0, x, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x4c9fa0, CityEditor_DrawKindGlyph);

}  // namespace rt3
