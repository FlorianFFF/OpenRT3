// EditorDialogs.cpp - RT3.exe 0x4ca000-0x4d2000.
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
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall ListView_RefreshVisible(uint8_t* tv);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_CountFlagged7ac(void* o);
int32_t __fastcall Obj_CountTypesAvailable(void* o, int, int32_t year, int32_t kind);
void __fastcall Tracks_UpdateColor(Collection* c);
void __fastcall Table_RefreshAll(Collection* t);
void* __fastcall Obj_Ref72Plus45(void* o);
int32_t __fastcall World_LookupNamed2(void* w, int, const char* name);
int32_t __fastcall Table_CountListing(Collection* t, int, uint32_t typeId);
void* __fastcall Table_NthListing(Collection* t, int, uint32_t typeId, int32_t nth);
int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
double __fastcall Obj_NamedValue(void* o, int, const char* name);
void CityDialog_UpdateButtons();
void __fastcall Templates_Normalize(uint8_t* w);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
float __fastcall Obj_ProductionRate(void* o, int, int32_t i);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Nop4cf900(void*);
void __fastcall EditorDialog_ShowSettings(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w);
int32_t __fastcall Dialog_SetupControl5141(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w);
void __fastcall CityEditor_Fill(uint8_t* w, int, int32_t id, int32_t keep);
void __fastcall TerritoryEditor_Show(uint8_t* w, int, int32_t id);
void __fastcall RulesDialog_Init(uint8_t* w);
void __fastcall NewGameDialog_SetupRows(uint8_t* w);
void __fastcall EditorDialog_FillCargoList(uint8_t* w);
void __fastcall EditorDialog_ShowEconomy(uint8_t* w);
void __fastcall EditorDialog_FillIndustries(uint8_t* w, int, int32_t keep);
void __fastcall EditorDialog_FillHouses(uint8_t* w, int, int32_t keep);
void __fastcall EditorDialog_ShowAchievements(uint8_t* w, int, int32_t keep);
void __fastcall EditorDialog_ShowRegionTypes(uint8_t* w);
void __fastcall Window_ShowWorldPicks(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w);
void __fastcall Templates_Normalize(uint8_t* self);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
uint8_t* __fastcall PortraitDialog_Construct(uint8_t* d);
void __fastcall EditorDialog_Init(uint8_t* w, int, int32_t tab, int32_t city);
void __fastcall SetupWindow_Destroy(uint8_t* w);
void __fastcall EditTool_UpdateCursor(uint8_t* t);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
void __fastcall EditorDialog_SetTab(uint8_t* w, int, int32_t tab, int32_t city);
void* __fastcall Table_At(Collection* t, int, int32_t i);
void __fastcall Region_PickOther(uint8_t* o);
uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount);
void __fastcall Obj_CopyNamedAmounts(void* o, int, void* other);
void __fastcall Obj_NumberPorts(void* o, int, int32_t n);
void __fastcall NewGameDialog_StepPortrait(uint8_t* w, int, int32_t dir);
void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai, int32_t clearHistory);
void __fastcall NewGameDialog_ListHumanRows(uint8_t* w);
void __fastcall NewGameDialog_CountRows(uint8_t* w);
void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall SetSeed(int32_t seed);

int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3, int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3,
                                     int32_t all);

int32_t __fastcall Obj_Count37f(void* o);
void __fastcall Obj_Entry37f(void* o, int, int32_t i, char* dst, int32_t* out);
void* __fastcall Table_GetByName(Collection* t, int, const char* name);
int32_t GetSeed();
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
int32_t __fastcall City_BuildIndustry(uint8_t* c, int, const char* name, int32_t weighted, int32_t checkOnly, uint32_t* outId);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t GetSeed();
RT3_NOINLINE int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
RT3_NOINLINE void* __fastcall Obj_Ref72Plus45(void* o);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void* __fastcall Table_GetByName(Collection* t, int, const char* name);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall NewGameDialog_ShowRow(uint8_t* w, int, int32_t row);
RT3_NOINLINE void __fastcall EditorDialog_SetTab(uint8_t* w, int, int32_t tab, int32_t city);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);

// Dialog callbacks clearing a range of their window's child widgets.
#define RT3_CLEAR_CHILDREN(addr, name, lo, hi) \
    void __fastcall name(uint8_t* w, int, int32_t) { Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, lo, hi, (uint32_t)(uintptr_t)w); } \
    RT3_REPLACE(addr, name);

void CityDialog_ShowSummary();
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o);
int32_t __fastcall EditorDialog_OnSettingsEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall CityEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall TerritoryEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall NewGameDialog_HandleEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall EditorDialog_OnCargoEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall EditorDialog_OnIndustryEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall EditorDialog_HousesEvent(uint8_t* w, int, int32_t* ev);
int32_t __fastcall EditorDialog_OnAchievementsEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall EditorDialog_OnRegionTypesEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall Ui_HandleModeMsg(void*, int, const int32_t* msg);
uint32_t __fastcall EventEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall Ui_HandleCounterMsg(void*, int, const int32_t* msg);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall City_BuildIndustry(uint8_t* c, int, const char* name, int32_t weighted, int32_t checkOnly, uint32_t* outId);
RT3_NOINLINE void __fastcall World_SetNamedValue(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall NewGameDialog_ShowRow(uint8_t* w, int, int32_t row);
void __fastcall NewGameDialog_CountRows(uint8_t* w);
void __fastcall SetupToggle_AddWorldByte(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall EditorDialog_FillCargoList(uint8_t* w);
void __fastcall EditorDialog_ShowBriefings(uint8_t* w);
RT3_NOINLINE void __fastcall EditorDialog_ShowSettings(uint8_t* w);
RT3_NOINLINE void __fastcall Window_ShowWorldPicks(uint8_t* w);
void __fastcall Economy_SliderCb(uint8_t* w, int32_t v);
void __fastcall InflationTable_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall EditorDialog_ShowEconomy(uint8_t* w);
void __fastcall SetupToggle_AddWorldFlag(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall EditorDialog_ShowAchievements(uint8_t* w, int, int32_t keep);
RT3_NOINLINE int32_t __fastcall EditorDialog_OnSettingsEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Ui_HandleCounterMsg(void*, int, const int32_t* msg);
RT3_NOINLINE int32_t __fastcall EditorDialog_OnCargoEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall EditorDialog_OnAchievementsEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Ui_HandleModeMsg(void*, int, const int32_t* msg);
void __fastcall BuildingList_DrawRowCb(void*, int32_t id, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void CityDialog_ShowSummary();
RT3_NOINLINE void CityDialog_UpdateButtons();
void __fastcall CityDialog_SetAmountCb(uint8_t* w, int32_t v);
void __fastcall Setup_AddTypeRow(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall SetupToggle_AddNamed1(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall NewGame_ShowRowCb(int32_t v, int32_t row);
void __fastcall NewGameDialog_ListHumanRows(uint8_t* w);
RT3_NOINLINE void __fastcall NewGameDialog_SetupRows(uint8_t* w);
void __fastcall CityInfo_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall CityEditor_SetSizeCb(void*, int32_t v);
RT3_NOINLINE void __fastcall CityEditor_Fill(uint8_t* w, int, int32_t id, int32_t keep);
RT3_NOINLINE void __fastcall TerritoryEditor_Show(uint8_t* w, int, int32_t id);
void __fastcall SetupToggle_AddNamed2(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall EditorDialog_FillIndustries(uint8_t* w, int, int32_t keep);
RT3_NOINLINE void __fastcall EditorDialog_FillHouses(uint8_t* w, int, int32_t keep);
RT3_NOINLINE void __fastcall EditorDialog_SetTab(uint8_t* w, int, int32_t tab, int32_t city);
RT3_NOINLINE uint32_t __fastcall CityEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE uint32_t __fastcall TerritoryEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall NewGameDialog_StepPortrait(uint8_t* w, int, int32_t dir);
RT3_NOINLINE uint32_t __fastcall NewGameDialog_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE uint32_t __fastcall EditorDialog_OnIndustryEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall EditorDialog_HousesEvent(uint8_t* w, int, int32_t* ev);
void __fastcall Editor_SetTabCb(int32_t v);
void __fastcall EditorDialog_Init(uint8_t* w, int, int32_t tab, int32_t city);
int32_t __fastcall EditorDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
RT3_NOINLINE void __fastcall Nop4cf900(void*);
RT3_NOINLINE void __fastcall EditorDialog_ShowRegionTypes(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall EditorDialog_OnRegionTypesEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE float __fastcall Obj_Ratio76(uint8_t* o);
RT3_NOINLINE void __fastcall EngineStats_Draw(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall ScenarioCheck_Draw(void*, int, float x, float y0, float w0, float h, int32_t);
void __fastcall MapStats_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall LocoStats_DrawTable(void*, int, float x, float y, float, float, int32_t);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetupControl5141(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w);
RT3_NOINLINE int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_Init(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall EventEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

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

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// Company finance statistics. Ids below 0x2b are kept per year: +0x1c47 + id * 8
// (this year so far) and +0xd7f + (id * 11 + n) * 8 (n years ago, 0 = live); larger
// ids are sums / ratios of those. Years: 0x2329 = live value, 0x232a = this year so
// far. World 0x6cec78: year +0xd, month +0xf, day +0x11, tick +0x15, difficulty +0x2d.
static inline uint8_t* WorldG() { return hook::Global<uint8_t*>(0x6cec78); }

// The setup dialogs' toggle buttons ("Setup_Options_Buttons.imb"): button id
// `base + idx` with style `style`, placed right of (x, y, w, h) on the dialog's
// window, its index in +0x82; returns the window for the caller's tick event.
static uint8_t* SetupToggle_Add(uint8_t* d, int32_t idx, float x, float y, float w, float h, int32_t base, uint16_t style) {
    uint8_t def[0xa1];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + w - D(0x5d0b00));
    F<float>(def, 0xb) = (float)(y + D(0x5d0820));
    F<float>(def, 0xf) = w;
    F<float>(def, 0x13) = h;
    F<uint16_t>(def, 0) = (uint16_t)(idx + base);
    F<uint16_t>(def, 5) = style;
    F<uint8_t*>(def, 0x47) = d;
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0ae0);
    F<int32_t>(def, 0x6b) = 5;
    F<int32_t>(def, 0x67) = 6;
    uint8_t* b = (uint8_t*)ExeNew(0xe8);
    if (b) b = Button_Construct(b, 0, def);
    F<int32_t>(b, 0x82) = idx;
    Window_AddChild(F<uint8_t*>(d, 0x86), 0, b, 0);
    Free(F<void*>(def, 0x43));
    return F<uint8_t*>(d, 0x86);
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

// 0x4ca010. Shows player row `row` (+0x80) of the new-game setup (world +0x69d8 +
// row * 9: signed portrait, human flag, AI flag, a +0x69db / +0x69da pair, unsigned
// money +0x69dc): portrait (0x5a7e: index and kind), its name (0x5a7f), radio
// 0x5a77 / 0x5a78, 0x5a7b or 0x5a79 / 0x5a7a, the row-0-only controls disabled and
// the money (0x5a7c, "%.f").
RT3_NOINLINE void __fastcall NewGameDialog_ShowRow(uint8_t* w, int, int32_t row) {
    F<int32_t>(w, 0x80) = row;
    int32_t portrait = (int8_t)hook::Global<uint8_t*>(0x6cec78)[0x69d8 + row * 9];
    Window_SendEvent5(w, 0, 0x7c, 0x5a7e, (uint32_t)portrait, 0, 0);
    const uint8_t* s = (const uint8_t*)0x5f2d28 + portrait * 0x27;
    Window_SendEvent5(w, 0, 0x6c, 0x5a7e, s[1], 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5a7f, (uint32_t)(uintptr_t)GetString(*(const int32_t*)(s + 0x1f)), 0, 0);
    Window_SendEvent5(w, 0, 0x74, hook::Global<uint8_t*>(0x6cec78)[0x69d9 + row * 9] ? 0x5a77 : 0x5a78, 1, 0, 0);
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    uint8_t a = world[0x69db + row * 9];
    if (a && world[0x69da + row * 9]) Window_SendEvent5(w, 0, 0x74, 0x5a7b, 1, 0, 0);
    else Window_SendEvent5(w, 0, 0x74, a ? 0x5a79 : 0x5a7a, 1, 0, 0);
    uint32_t t = row ? 0x65 : 0x87;
    Window_SendEvent5(w, 0, t, 0x5a80, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0x5a77, 2, 0, 0);
    Window_SendEvent5(w, 0, t, 0x5a7a, 2, 0, 0);
    Window_SendEvent5(w, 0, t, 0x5a7b, 2, 0, 0);
    char buf[0x80];
    Num_Format(buf, 8, (double)F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x69dc + row * 9), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5a7c, (uint32_t)(uintptr_t)buf, 0, 0);
}
RT3_REPLACE(0x4ca010, NewGameDialog_ShowRow);

// 0x4ca1c0. Counts the new-game player rows (world +0x6a6c rows of 9 bytes from
// +0x69d8: human +1, AI-allowed +2, AI +3): rows without a human (+0x6a68), AI rows
// (+0x6a74), AI rows with neither flag (+0x6a70), AI-allowed rows (+0x6a7c) and
// AI-allowed rows with neither (+0x6a78); shows "a / b" (format 0x5cfda8) for
// no-human / rows, open / AI and open / AI-allowed on 0x5a6f..0x5a71.
RT3_NOINLINE void __fastcall NewGameDialog_CountRows(uint8_t* w) {
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t n = F<int32_t>(world, 0x6a6c);
    int32_t noHuman = 0, ai = 0, aiOpen = 0, allowed = 0, allowedOpen = 0;
    for (int32_t i = 0; i < n; ++i) {
        const uint8_t* p = world + 0x69d9 + i * 9;
        uint8_t human = p[0], allow = p[1], isAi = p[2];
        if (!human) ++noHuman;
        if (isAi) {
            ++ai;
            if (!human && !allow) ++aiOpen;
        }
        if (allow) {
            ++allowed;
            if (!human && !isAi) ++allowedOpen;
        }
    }
    auto line = [w](uint32_t id, int32_t a, int32_t b) {
        char sa[0x20], sb[0x20], text[0x70];
        char* s2 = Num_Format(sb, 1, (double)b, 10, 1, 0, 0);
        char* s1 = Num_Format(sa, 1, (double)a, 10, 1, 0, 0);
        FormatText(text, (const char*)0x5cfda8, s1, s2, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, id, (uint32_t)(uintptr_t)text, 0, 0);
    };
    line(0x5a6f, noHuman, n);
    line(0x5a70, aiOpen, ai);
    line(0x5a71, allowedOpen, allowed);
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a68) = noHuman;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a70) = aiOpen;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a74) = ai;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a78) = allowedOpen;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a7c) = allowed;
}
RT3_REPLACE(0x4ca1c0, NewGameDialog_CountRows);

RT3_CLEAR_CHILDREN(0x4ca430, Dialog_ClearChildren5aaa, 0x5aaa, 0x5b03)

// 0x4ca450. Toggle 0x5aaa + idx, ticked by world byte +0x6987 + idx.
void __fastcall SetupToggle_AddWorldByte(uint8_t* d, int, int32_t idx, float x, float y, float w, float h) {
    uint8_t* win = SetupToggle_Add(d, idx, x, y, w, h, 0x5aaa, 0x4e24);
    Window_SendEvent5(win, 0, 0x74, idx + 0x5aaa, WorldG()[0x6987 + idx], 0, 0);
}
RT3_REPLACE(0x4ca450, SetupToggle_AddWorldByte);

// 0x4ca540. Fills the scenario editor's cargo list 0x5aa0 (callbacks 0x4ca450 /
// 0x4ca430, events 0x7e / 0x7f; 40 cargo names from table 0x5f2d6e, event 0x88 with
// the cargo number) and shows "N out of 40 are selected." (string 0x40b, the world's
// +0x6987.. flags) in field 0x5aa4.
RT3_NOINLINE void __fastcall EditorDialog_FillCargoList(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x7e, 0x5aa0, 0x4ca450, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x5aa0, 0x4ca430, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x5aa0, 0, 0, 0);
    int32_t selected = 0, i = 1;
    for (const uint8_t* rec = (const uint8_t*)0x5f2d6e; rec < (const uint8_t*)0x5f3386; rec += 0x27, ++i) {
        Window_SendEvent5(w, 0, 0x88, 0x5aa0, (uint32_t)(uintptr_t)GetString(*(const int32_t*)rec), (uint32_t)i, 0);
        if (hook::Global<uint8_t*>(0x6cec78)[0x6987 + i]) ++selected;
    }
    char total[0x20], count[0x20], text[0xf0];
    Num_Format(total, 1, 40.0, 10, 1, 0, 0);
    Num_Format(count, 1, (double)selected, 10, 1, 0, 0);
    FormatText(text, GetString(0x40b), count, total, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5aa4, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4ca540, EditorDialog_FillCargoList);

// 0x4ca670. Shows the scenario's briefing texts (world +0x4f30 / +0x5ae9) in the
// editor's two text boxes 0x5b6d / 0x5b6f, with the single- or multiplayer heading
// (string 0x5d3 / 0xe02, by 0x621f50) and the matching box enabled; guarded against
// re-entry (0x6cff94).
void __fastcall EditorDialog_ShowBriefings(uint8_t* w) {
    if (hook::Global<int32_t>(0x6cff94)) return;
    hook::Global<int32_t>(0x6cff94) = 1;
    if (hook::Global<int32_t>(0x621f50)) {
        Window_SendEvent5(w, 0, 0x6f, 0x5b73, (uint32_t)(uintptr_t)GetString(0x5d3), 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x5b6d, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x5b6f, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x74, 0x5b71, 1, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x6f, 0x5b73, (uint32_t)(uintptr_t)GetString(0xe02), 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x5b6d, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x5b6f, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x74, 0x5b72, 1, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x6f, 0x5b6d, (uint32_t)(uintptr_t)(hook::Global<uint8_t*>(0x6cec78) + 0x4f30), 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5b6f, (uint32_t)(uintptr_t)(hook::Global<uint8_t*>(0x6cec78) + 0x5ae9), 0, 0);
    hook::Global<int32_t>(0x6cff94) = 0;
}
RT3_REPLACE(0x4ca670, EditorDialog_ShowBriefings);

// 0x4ca790. Fills the scenario editor's settings page: the briefings, the
// scenario's numbers (world +0x66ca twice, +0x66d2, +0x66ce; "%.f" style) in fields
// 0x5b69 / 0x5b6a / 0x5b6c / 0x5b6b and switches +0x66de / !+0x66f3 on 0x5b6e /
// 0x5b74. Quirk: the first number is formatted and then thrown away; field 0x5b69
// gets the world's text at +0x672e instead.
RT3_NOINLINE void __fastcall EditorDialog_ShowSettings(uint8_t* w) {
    EditorDialog_ShowBriefings(w);
    char buf[0x440];
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    Num_Format(buf, 8, (double)F<int32_t>(world, 0x66ca), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5b69, (uint32_t)(uintptr_t)(hook::Global<uint8_t*>(0x6cec78) + 0x672e), 0, 0);
    Num_Format(buf, 8, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ca), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5b6a, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66d2), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5b6c, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ce), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5b6b, (uint32_t)(uintptr_t)buf, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x5b6e, hook::Global<uint8_t*>(0x6cec78)[0x66de], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x5b74, hook::Global<uint8_t*>(0x6cec78)[0x66f3] == 0, 0, 0);
}
RT3_REPLACE(0x4ca790, EditorDialog_ShowSettings);

// 0x4ca910. Sets two of window w's text fields (event 0x74, ids 0x5c3a / 0x5c44 +
// the world's +0x4c78 / +0x4c7c selections).
RT3_NOINLINE void __fastcall Window_ShowWorldPicks(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x74, F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c78) + 0x5c3a, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c7c) + 0x5c44, 1, 0, 0);
}
RT3_REPLACE(0x4ca910, Window_ShowWorldPicks);

RT3_CLEAR_CHILDREN(0x4ca960, Dialog_ClearChildrenA410, 0xa410, 0xa7f7)

// 0x4ca980 (slider callback). Economy sliders 0x5bcd.. (every second id):
// the first sets world +0xbe2 = v / 0x5c85d8, the others +0xbe6..+0xbf6 =
// (v - 50) / 0x5c87f8; +0xbde follows +0xbe2 and the editor refreshes.
void __fastcall Economy_SliderCb(uint8_t* w, int32_t v) {
    if (hook::Global<int32_t>(0x6cff9c)) return;
    const uint32_t k = (uint32_t)F<uint16_t>(w, 4) - 0x5bcd;
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    if (k <= 10 && !(k & 1)) {
        if (k == 0) F<float>(world, 0xbe2) = (float)((double)v / D(0x5c85d8));
        else F<float>(world, 0xbe2 + k * 2) = (float)((double)(v - 0x32) / D(0x5c87f8));
    }
    F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0xbde) = F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0xbe2);
    if (uint8_t* d = hook::Global<uint8_t*>(0x6cff84)) EditorDialog_ShowEconomy(d);
}
RT3_REPLACE(0x4ca980, Economy_SliderCb);

// 0x4caaf0 (draw callback). Inflation table (string 0x418): every 10 years
// (rows up to year 51 and year 81, string 0x419) the price level starting
// at 1 (0x6cec78 +0xbe2) in percent (0x5d0b18), compounded each year by the
// decade's rate (+0xbe6, the fifth one from year 41 on).
void __fastcall InflationTable_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, GetString(0x418), x, y, w, BitsF(0x3d77ced9), 0xffffffff, 2, 0, 0, 1.0f);
    float row = (float)((double)y + D(0x5d0b28));
    float level = F<float>(hook::Global<uint8_t*>(0x6cec78), 0xbe2);
    char frame[0x17c];
    char* num = frame + 0x10;
    char* text = frame + 0x50;
    int32_t decade = 0;
    for (int32_t year = 1; year < 0x5b; year += 10, ++decade) {
        if (year <= 0x33 || year == 0x51) {
            Num_Format(num, 8, (double)year, 10, 1, 0, 0);
            FormatText(text, GetString(0x419), num, 0, 0, 0, 0, 0, 0, 0);
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, (float)((double)x + Fl(0x5d0b20)), row, w, BitsF(0x3ce147ae), 0xffffffff, 2, 0, 0, 1.0f);
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(text, (const char*)0x5d0b18, Ftol((double)level * Fl(0x5c8c28) + D(0x5c86b8)));
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, (float)((double)x - Fl(0x5d0b20) - D(0x5d0b10)), row, w, BitsF(0x3ce147ae), 0xffffffff, 3, 0, 0,
                             1.0f);
            row = (float)((double)row + Fl(0x5d0b08));
        }
        const float rate = *(const float*)(hook::Global<uint8_t*>(0x6cec78) + 0xbe6 + (year >= 0x29 ? 4 : decade) * 4);
        for (int32_t k = 0; k < 10; ++k) level = (float)(((double)rate + D(0x5c8770)) * level);
    }
}
RT3_REPLACE(0x4caaf0, InflationTable_DrawCb);

// 0x4cadf0. The scenario editor's economy sliders (busy flag 0x6cff9c): callbacks
// 0x4ca980 on the six sliders 0x5bcd..0x5bd7 and 0x4caaf0 on 0x5be1; slider 0x5bcd
// shows world +0xbe2 (x 0x5c85d8, rounded), the others +0xbe6.. (x 0x5c87f8,
// rounded, + 50); labels 0x5bce ("%d" style 0x5d0b18 of +0xbe2 x 0x5c8c28) and
// 0x5bd0.. (format 0x5d0b30 of value x 0x5c8c28 + 0x5c85b8).
RT3_NOINLINE void __fastcall EditorDialog_ShowEconomy(uint8_t* w) {
    hook::Global<int32_t>(0x6cff9c) = 1;
    for (uint32_t id = 0x5bcd; id <= 0x5bd7; id += 2) Window_SendEvent5(w, 0, 0x84, id, 0x4ca980, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x5be1, 0x4caaf0, 0, 0);
    const double half = *(const double*)0x5c86b8;
    auto fw = [](int32_t off) { return F<float>(hook::Global<uint8_t*>(0x6cec78), off); };
    Window_SendEvent5(w, 0, 0x66, 0x5bcd, (uint32_t)Ftol((double)fw(0xbe2) * *(const double*)0x5c85d8 + half), 0, 0);
    for (int32_t k = 1; k <= 5; ++k)
        Window_SendEvent5(w, 0, 0x66, 0x5bcd + k * 2, (uint32_t)(Ftol((double)fw(0xbe2 + k * 4) * *(const double*)0x5c87f8 + half) + 0x32), 0, 0);
    const float scale = *(const float*)0x5c8c28;
    char buf[0x130];
    ExeSprintf()(buf, (const char*)0x5d0b18, Ftol((double)fw(0xbe2) * scale + half));
    Window_SendEvent5(w, 0, 0x6f, 0x5bce, (uint32_t)(uintptr_t)buf, 0, 0);
    for (int32_t k = 1; k <= 5; ++k) {
        ExeSprintf()(buf, (const char*)0x5d0b30, (double)fw(0xbe2 + k * 4) * scale + *(const double*)0x5c85b8);
        Window_SendEvent5(w, 0, 0x6f, 0x5bce + k * 2, (uint32_t)(uintptr_t)buf, 0, 0);
    }
    hook::Global<int32_t>(0x6cff9c) = 0;
}
RT3_REPLACE(0x4cadf0, EditorDialog_ShowEconomy);

RT3_CLEAR_CHILDREN(0x4cb1a0, Dialog_ClearChildrenAbe0, 0xabe0, 0xafc7)

// 0x4cb1c0. Toggle 0xabe0 + idx, ticked by world dword +0x4a7f + idx * 4.
void __fastcall SetupToggle_AddWorldFlag(uint8_t* d, int, int32_t idx, float x, float y, float w, float h) {
    uint8_t* win = SetupToggle_Add(d, idx, x, y, w, h, 0xabe0, 0x4e28);
    Window_SendEvent5(win, 0, 0x74, idx + 0xabe0, F<uint32_t>(WorldG(), 0x4a7f + idx * 4), 0, 0);
}
RT3_REPLACE(0x4cb1c0, SetupToggle_AddWorldFlag);

// 0x4cb2b0. The scenario editor's achievements page: the goal number (world
// +0x4b43, "%.f") in 0xa7ff, controls 0xa7fd..0xa7ff enabled by +0x4aeb, list 0xa7fa
// (callbacks 0x4cb1c0 / 0x4cb1a0) refilled unless `keep` with the 49 entries of
// table 0x5f3ab0 (9 bytes: hidden flag, name / description strings) that aren't
// hidden, and "N of M selected" (string 0x41d; selected = world +0x4a7f.. flags) on
// label 0xa7f8 of the city dialog (0x6cff84).
RT3_NOINLINE void __fastcall EditorDialog_ShowAchievements(uint8_t* w, int, int32_t keep) {
    char goal[0x1a0], on[0x18], all[0x18], text[0x160];
    Num_Format(goal, 8, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4b43), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xa7ff, (uint32_t)(uintptr_t)goal, 0, 0);
    uint32_t t = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aeb) ? 0x65 : 0x87;
    Window_SendEvent5(w, 0, t, 0xa7fd, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0xa7fe, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0xa7ff, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x7e, 0xa7fa, 0x4cb1c0, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0xa7fa, 0x4cb1a0, 0, 0);
    if (!keep) Window_SendEvent5(w, 0, 0x8b, 0xa7fa, 0, 0, 0);
    int32_t total = 0, selected = 0, i = 0;
    for (const uint8_t* e = (const uint8_t*)0x5f3ab0; e < (const uint8_t*)0x5f3c69; e += 9, ++i) {
        if (e[0]) continue;
        ++total;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f + i * 4)) ++selected;
        if (keep) continue;
        const char* desc = GetString(*(const int32_t*)(e + 5));
        const char* name = GetString(*(const int32_t*)(e + 1));
        Window_SendEvent5(w, 0, 0x88, 0xa7fa, (uint32_t)(uintptr_t)name, (uint32_t)i, (uint32_t)(uintptr_t)desc);
    }
    char* sa = Num_Format(all, 1, (double)total, 10, 1, 0, 0);
    char* so = Num_Format(on, 1, (double)selected, 10, 1, 0, 0);
    FormatText(text, GetString(0x41d), so, sa, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x6f, 0xa7f8, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4cb2b0, EditorDialog_ShowAchievements);

// 0x4cb4a0. Click / edit handler (event 0xca) of the scenario editor's settings
// page: the scenario text (0x5b69, 600 chars), the three years (0x5b6a / 0x5b6b /
// 0x5b6c into world +0x66ca / +0x66ce / +0x66d2, clamped to 1829..2100; then
// +0x66ca <= +0x66d2 <= +0x66ce), the two briefings (0x5b6d / 0x5b6f, 3000 chars)
// and the switches 0x5b6e / 0x5b74 / briefing tabs 0x5b71 / 0x5b72. 0 if handled.
RT3_NOINLINE int32_t __fastcall EditorDialog_OnSettingsEvent(uint8_t* w, int, uint32_t* ev) {
    if (ev[0] != 0xca) return 1;
    auto text = [ev]() { return F<const char*>((uint8_t*)(uintptr_t)ev[0xb], 0xba); };
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    switch (ev[1]) {
    case 0x5b69:
        StrCopyN((char*)world + 0x672e, text(), 600);
        EditorDialog_ShowSettings(w);
        return 0;
    case 0x5b6a:
    case 0x5b6b:
    case 0x5b6c: {
        int32_t y = ((int(__cdecl*)(const char*))0x5a1ea5)(text());
        if (y < 0x725) y = 0x725;
        else if (y > 0x834) y = 0x834;
        F<int32_t>(world, ev[1] == 0x5b6a ? 0x66ca : ev[1] == 0x5b6b ? 0x66ce : 0x66d2) = y;
        world = hook::Global<uint8_t*>(0x6cec78);
        if (F<int32_t>(world, 0x66ca) >= F<int32_t>(world, 0x66d2)) F<int32_t>(world, 0x66ca) = F<int32_t>(world, 0x66d2);
        if (F<int32_t>(world, 0x66ce) <= F<int32_t>(world, 0x66d2)) F<int32_t>(world, 0x66ce) = F<int32_t>(world, 0x66d2);
        EditorDialog_ShowSettings(w);
        return 0;
    }
    case 0x5b6d:
        StrCopyN((char*)world + 0x4f30, text(), 3000);
        EditorDialog_ShowSettings(w);
        return 0;
    case 0x5b6e:
        world[0x66de] = !world[0x66de];
        return 0;
    case 0x5b6f:
        StrCopyN((char*)world + 0x5ae9, text(), 3000);
        EditorDialog_ShowSettings(w);
        return 0;
    case 0x5b71:
    case 0x5b72:
        hook::Global<int32_t>(0x621f50) = ev[1] == 0x5b71;
        EditorDialog_ShowBriefings(w);
        return 0;
    case 0x5b74:
        world[0x66f3] = !world[0x66f3];
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4cb4a0, EditorDialog_OnSettingsEvent);

// 0x4cb6b0. UI message handler: 0xcb messages 0x5143 / 0x5142 adjust the counter 0x6cff7c.
RT3_NOINLINE int32_t __fastcall Ui_HandleCounterMsg(void*, int, const int32_t* msg) {
    if (msg[0] != 0xcb) return 1;
    if (msg[1] == 0x5143) {
        hook::Global<int32_t>(0x6cff7c)++;
        return 0;
    }
    if (msg[1] == 0x5142) {
        hook::Global<int32_t>(0x6cff7c)--;
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4cb6b0, Ui_HandleCounterMsg);

// 0x4cb6f0. Event handler of the editor's cargo page: a tick (0xca) on rows
// 0x5aaa..0x5b03 sets the row's cargo (its widget +0x82) flag world +0x6987.. to
// the event value; the group buttons (0xcb) 0x5aa1..0x5aa3 select exactly the 40
// cargos of that group (cargo record +2). Then refreshes the list. 0 if handled.
RT3_NOINLINE int32_t __fastcall EditorDialog_OnCargoEvent(uint8_t* w, int, uint32_t* ev) {
    if (ev[0] == 0xca) {
        if ((int32_t)ev[1] < 0x5aaa || (int32_t)ev[1] > 0x5b03) return 1;
        hook::Global<uint8_t*>(0x6cec78)[0x6987 + F<int32_t>((uint8_t*)(uintptr_t)ev[0xb], 0x82)] = (uint8_t)ev[2];
        EditorDialog_FillCargoList(w);
        return 0;
    }
    if (ev[0] != 0xcb || (int32_t)ev[1] < 0x5aa1 || (int32_t)ev[1] > 0x5aa3) return 1;
    for (int32_t i = 1; i <= 40; ++i)
        hook::Global<uint8_t*>(0x6cec78)[0x6987 + i] = ((const uint8_t*)0x5f2d28)[i * 0x27 + 2] == ev[1] - 0x5aa1;
    EditorDialog_FillCargoList(w);
    return 0;
}
RT3_REPLACE(0x4cb6f0, EditorDialog_OnCargoEvent);

// 0x4cb8e0. Event handler of the editor's achievements page: ticking a row
// (0xca on 0xabe0..0xafc7) sets that goal's flag (world +0x4a7f + 4 x its widget
// +0x82; goals 25 / 26 also recolor the track / refresh the buildings); the goal
// number field 0xa7ff takes its text's value (+0x4b43); "all" / "none" (0xcb on
// 0xa7fb / 0xa7fc) set all 49 flags and refill the list. 0 if handled.
RT3_NOINLINE int32_t __fastcall EditorDialog_OnAchievementsEvent(uint8_t* w, int, uint32_t* ev) {
    if (ev[0] == 0xcb) {
        if ((int32_t)ev[1] < 0xa7fb || (int32_t)ev[1] > 0xa7fc) return 1;
        for (int32_t k = 0; k < 49; ++k) F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f + k * 4) = ev[1] == 0xa7fb;
        EditorDialog_ShowAchievements(w, 0, 0);
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    uint8_t* src = (uint8_t*)(uintptr_t)ev[0xb];
    if ((int32_t)ev[1] >= 0xabe0 && (int32_t)ev[1] <= 0xafc7) {
        F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f + F<int32_t>(src, 0x82) * 4) = ev[2];
        if (F<int32_t>(src, 0x82) == 0x19) Tracks_UpdateColor(hook::Global<Collection*>(0x6cfca8));
        if (F<int32_t>(src, 0x82) == 0x1a) Table_RefreshAll(hook::Global<Collection*>(0x62b26c));
    } else if (ev[1] == 0xa7ff) {
        F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4b43) = ((int(__cdecl*)(const char*))0x5a1ea5)(F<const char*>(src, 0xba));
    } else return 1;
    EditorDialog_ShowAchievements(w, 0, 1);
    return 0;
}
RT3_REPLACE(0x4cb8e0, EditorDialog_OnAchievementsEvent);

// 0x4cb9f0. UI message handler: 0xca messages 0x5c3a..0x5c3c / 0x5c44..0x5c46
// set world +0x4c78 / +0x4c7c to 0, 1 or 2.
RT3_NOINLINE int32_t __fastcall Ui_HandleModeMsg(void*, int, const int32_t* msg) {
    if (msg[0] != 0xca) return 1;
    uint32_t i = (uint32_t)(msg[1] - 0x5c3a);
    if (i > 0xc) return 1;
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (i <= 2) F<int32_t>(w, 0x4c78) = (int32_t)i;
    else if (i >= 10) F<int32_t>(w, 0x4c7c) = (int32_t)(i - 10);
    return 1;
}
RT3_REPLACE(0x4cb9f0, Ui_HandleModeMsg);

// 0x4cbac0 (draw callback). Building type list row `id` (0x62b268): its
// localised name with the count (+0x26, string 0x5d0b40) dimmed for unusable
// types (0x411d50: blue 0xbf, +0x78c: red 0xbf), a highlight when the
// selected city (0x6cff74) values it (0x41fa30 >= 0x5c8678) and that value.
void __fastcall BuildingList_DrawRowCb(void*, int32_t id, float x, float y, float w, float h, int32_t) {
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, (uint32_t)id);
    const char* name = LocalizeName((const char*)b + 0x4f);
    char num[0x40], text[0x12c];
    Num_Format(num, 8, (double)F<uint16_t>(b, 0x26), 10, 1, 0, 0);
    FormatText(text, (const char*)0x5d0b40, name, num, 0, 0, 0, 0, 0, 0);
    uint32_t red = 0xff, blue = 0xff;
    if (Obj_AllUnusedSomeUndefined(b))
        blue = 0xbf;
    else if (F<uint32_t>(b, 0x78c))
        red = 0xbf;
    uint8_t* city = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    const float value = (float)Obj_NamedValue(city, 0, (const char*)b + 4);
    if (value >= D(0x5c8678)) Ui_FillRect(0x22ffffff, 0, x, y, w, h, 3);
    const float tx = (float)((double)x + D(0x5d0b38));
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff88), 0, text, tx, y, BitsF(0x3e5950c8), h, (red << 16) | blue | 0xff00ff00, 2, 0, 0, 1.0f);
    Num_Format(text, 5, (double)value, 10, 1, 0, 0);
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff88), 0, text, x, y, BitsF(0x3e7d27d2), h, 0xffffffff, 3, 0, 0, 1.0f);
}
RT3_REPLACE(0x4cbac0, BuildingList_DrawRowCb);

// 0x4cbc60. The city dialog's (0x6cff84) summary line 0x5078 for the selected city
// (0x6cff74): its size (+0x242 * 16, string 0x408, when +0x23e is set; else string
// 0x409), the count of flagged industries and of building types available (0x726,
// kind 2), through string 0x40a.
RT3_NOINLINE void CityDialog_ShowSummary() {
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    char size[0x100], num[0x100], s1[0x128], s2[0x128], text[0x1d0];
    if (F<int32_t>(c, 0x23e)) FormatText(size, GetString(0x408), Num_Format(num, 1, (double)(F<int32_t>(c, 0x242) << 4), 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
    else strcpy(size, GetString(0x409));
    Num_Format(s1, 1, (double)Obj_CountFlagged7ac(c), 10, 1, 0, 0);
    Num_Format(s2, 1, (double)Obj_CountTypesAvailable(c, 0, 0x726, 2), 10, 1, 0, 0);
    FormatText(text, GetString(0x40a), size, s1, s2, 0, 0, 0, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x6f, 0x5078, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4cbc60, CityDialog_ShowSummary);

// 0x4cbdd0. Enables the city dialog's (0x6cff84) buttons 0x508a / 0x5089 by the
// selected city's (0x6cff74) flag +0x2f6 (0x65 on, 0x87 off).
RT3_NOINLINE void CityDialog_UpdateButtons() {
    uint8_t* city = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    if (!city) return;
    uint32_t type = F<int32_t>(city, 0x2f6) ? 0x65 : 0x87;
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, type, 0x508a, 3, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, type, 0x5089, 3, 0, 0);
}
RT3_REPLACE(0x4cbdd0, CityDialog_UpdateButtons);

// 0x4cbe30 (slider callback). The edited city (0x6cff74) gets v (EDX) /
// 0x5c85d8 of the building type of slider w (+0x82); the dialog updates.
void __fastcall CityDialog_SetAmountCb(uint8_t* w, int32_t v) {
    uint8_t* city = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    uint8_t* type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(w, 0x82));
    Obj_SetNamedAmount(city, 0, (const char*)(type + 4), (float)((double)v / D(0x5c85d8)));
    CityDialog_UpdateButtons();
    CityDialog_ShowSummary();
}
RT3_REPLACE(0x4cbe30, CityDialog_SetAmountCb);

// 0x4cbea0. Scenario setup row for type `idx` (0x62b268): its label frame
// 0x5208 + idx at (x, y, w, h) with the click handler 0x4cbac0, and the value
// list 0x55f0 + idx (6 choices 0x14..0x19, "...Buttons" image 0x5d0b48) set to
// the city 0x6cff74's value for the type's name (x 0x5c8c28, rounded) with the
// change handler 0x4cbe30.
void __fastcall Setup_AddTypeRow(uint8_t* d, int, int32_t idx, float x, float y, float w, float h) {
    uint8_t* city = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = x;
    F<float>(def, 0xb) = y;
    F<float>(def, 0xf) = w;
    F<float>(def, 0x13) = h;
    F<uint16_t>(def, 0) = (uint16_t)(idx + 0x5208);
    F<uint16_t>(def, 5) = 0x4e21;
    F<uint8_t*>(def, 0x47) = d;
    uint8_t* frame = (uint8_t*)ExeNew(0x111);
    if (frame) frame = Scrollbar_Construct(frame, 0, def);
    F<int32_t>(frame, 0x82) = idx;
    Window_AddChild(F<uint8_t*>(d, 0x86), 0, frame, 0);
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x7a, F<uint16_t>(def, 0), 0x4cbac0, 0, 0);
    uint8_t ld[0xe3];
    memset(ld, 0, sizeof ld);
    F<float>(ld, 7) = (float)((double)x + D(0x5d0b60));
    F<uint16_t>(ld, 0) = (uint16_t)(idx + 0x55f0);
    F<int32_t>(ld, 0x63) = 1;
    F<int32_t>(ld, 0x67) = 1;
    F<float>(ld, 0xb) = (float)((double)y + D(0x5d06f8));
    F<uint32_t>(ld, 0xf) = 0x3e088889;
    F<uint32_t>(ld, 0x13) = 0x3d051eb8;
    F<uint16_t>(ld, 5) = 0x4e21;
    F<uint8_t*>(ld, 0x47) = d;
    F<int32_t>(ld, 0x5f) = 0x65;
    for (int k = 0; k < 6; ++k) F<int32_t>(ld, 0x87 + k * 4) = 0x14 + k;
    F<uint32_t>(ld, 0xab) = 0x3bf5c28f;
    F<uint32_t>(ld, 0xaf) = 0x3cba9876;
    F<uint32_t>(ld, 0xb3) = 0x3e02468b;
    F<uint32_t>(ld, 0xbf) = 0x3bb60b61;
    F<uint32_t>(ld, 0xc3) = 0x3bf5c28f;
    F<uint32_t>(ld, 0xc7) = 0x3e0369d0;
    F<uint32_t>(ld, 0xcb) = 0x3bf5c28f;
    String_Assign(&F<char*>(ld, 0x43), (const char*)0x5d0b48);
    uint8_t* list = (uint8_t*)ExeNew(0x1b9);
    if (list) list = ListBox_Construct(list, 0, ld);
    Window_AddChild(F<uint8_t*>(d, 0x86), 0, list, 0);
    F<int32_t>(list, 0x82) = idx;
    Free(F<void*>(ld, 0x43));
    const char* name = (const char*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, (uint32_t)idx) + 4;
    int32_t v = Ftol(Obj_NamedValue(city, 0, name) * Fl(0x5c8c28) + D(0x5c8918));
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x66, F<uint16_t>(ld, 0), (uint32_t)v, 0, 0);
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x84, F<uint16_t>(ld, 0), 0x4cbe30, 0, 0);
}
RT3_REPLACE(0x4cbea0, Setup_AddTypeRow);

// 0x4cc130. Toggle 0xa028 + idx for record idx of 0x62b268, ticked when the world's
// list +0x66b2 has its name (+4).
void __fastcall SetupToggle_AddNamed1(uint8_t* d, int, int32_t idx, float x, float y, float w, float h) {
    uint8_t* win = SetupToggle_Add(d, idx, x, y, w, h, 0xa028, 0x4e27);
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, idx);
    int32_t on = World_LookupNamed1(WorldG(), 0, (const char*)(r + 4));
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x74, idx + 0xa028, (uint32_t)on, 0, 0);
    (void)win;
}
RT3_REPLACE(0x4cc130, SetupToggle_AddNamed1);

void __fastcall NewGame_ShowRowCb(int32_t v, int32_t row) {
    if (v != -1 && hook::Global<uint8_t*>(0x6cff84)) NewGameDialog_ShowRow(hook::Global<uint8_t*>(0x6cff84), 0, row);
}
RT3_REPLACE(0x4cc230, NewGame_ShowRowCb);

// 0x4cc250. Lists the new-game rows that can host a human (AI-allowed +0x69da or
// AI +0x69db set to 1) in list 0x5a6e, then recounts. Quirk: the list is cleared
// on the city dialog (0x6cff84) but filled on window w.
RT3_NOINLINE void __fastcall NewGameDialog_ListHumanRows(uint8_t* w) {
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x8b, 0x5a6e, 0, 0, 0);
    for (int32_t i = 0; i < 16; ++i) {
        const uint8_t* row = hook::Global<uint8_t*>(0x6cec78) + i * 9;
        if (row[0x69db] == 1 || row[0x69da] == 1) Window_SendEvent5(w, 0, 0x88, 0x5a6e, 0, (uint32_t)i, 0);
    }
    NewGameDialog_CountRows(w);
}
RT3_REPLACE(0x4cc250, NewGameDialog_ListHumanRows);

// 0x4cc2d0. Sets up the new-game player-row lists (callbacks 0x4c9da0 on 0x5a6e,
// 0x4c9fa0 on 0x5a7e, 0x4cc230 hover on 0x5a6e), lists the rows and selects the
// first.
RT3_NOINLINE void __fastcall NewGameDialog_SetupRows(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x7a, 0x5a6e, 0x4c9da0, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x5a7e, 0x4c9fa0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x5a6e, 0x4cc230, 0, 0);
    NewGameDialog_ListHumanRows(w);
    Window_SendEvent5(w, 0, 0x66, 0x5a6e, 0, 0, 0);
}
RT3_REPLACE(0x4cc2d0, NewGameDialog_SetupRows);

// 0x4cc340 (draw callback). City info of city 0x6cff74: the intro text
// (string 0x40d with the price level x 0x5c8c28 and, for an abandoned city,
// string 0x40c with its +0x242 x 16), then, with any buildings
// (Obj_BuildingStats), a table of three production kinds (strings
// 0x40e..0x410, headers 0x411..0x413): count, rate (Obj_ProductionRate) and
// rate scaled by 25 years of inflation (0x6cec78 +0xbe6 / +0xbea / +0xbee for
// 10 / 10 / 5 years), and the footer 0x414.
void __fastcall CityInfo_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    if (!hook::Global<uint32_t>(0x6cff74)) return;
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    if (!c) return;
    char frame[0xe0c];
    char* note = frame + 0x18c;
    char* intro = frame + 0x4ac;
    char* cell = frame + 0x60;
    note[0] = 0;
    if (F<uint32_t>(c, 0x23e)) {
        char* num = frame + 0x7cc;
        Num_Format(num, 1, (double)(F<int32_t>(c, 0x242) << 4), 10, 1, 0, 0);
        FormatText(note, GetString(0x40c), num, 0, 0, 0, 0, 0, 0, 0);
    }
    {
        char* num = frame + 0xaec;
        Num_Format(num, 1, (double)F<float>(hook::Global<uint8_t*>(0x6cec78), 0xbe2) * Fl(0x5c8c28), 10, 1, 0, 0);
        FormatText(intro, GetString(0x40d), num, note, 0, 0, 0, 0, 0, 0);
    }
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, intro, x, y, w, h, 0xffffffff, 2, 1, 0, 1.0f);
    float row = (float)(Font_TextHeight(hook::Global<uint8_t*>(0x6cff90), 0, intro, w, nullptr) + y);
    static const int32_t kKind[4] = { -1, 2, 0, 3 };
    static const int32_t kName[4] = { 3, 0x40e, 0x40f, 0x410 };
    row = (float)((double)row + D(0x5d0b70));
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    const double g0 = (double)F<float>(world, 0xbe6) + D(0x5c8770);
    float level = (float)g0;
    for (int32_t k = 0; k < 9; ++k) level = (float)((double)level * g0);
    const double g1 = (double)F<float>(world, 0xbea) + D(0x5c8770);
    for (int32_t k = 0; k < 10; ++k) level = (float)((double)level * g1);
    const double g2 = (double)F<float>(world, 0xbee) + D(0x5c8770);
    for (int32_t k = 0; k < 5; ++k) level = (float)((double)level * g2);
    int32_t n0, n1, n2, n3, c0, c1, c2, c3;
    Obj_BuildingStats(c, 0, &n0, &n1, &n2, &n3, &c0, &c1, &c2, &c3, 1);
    const bool any = n2 + n0 + n3 > 0;
    const float rh = BitsF(0x3ce147ae);
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    for (int32_t i = 0; i < 4; ++i) {
        const float colw = any ? BitsF(0x3d6eeeef) : BitsF(0x3db05b06);
        if (i == 0) {
            if (any) {
                const char* s = GetString(0x411);
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, s, (float)((double)x - ((double)colw + colw)), row, w, rh, 0xffffffff, 3, 0, 1, 1.0f);
            }
            const char* s2 = GetString(0x412);
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, s2, (float)((double)x - colw), row, w, rh, 0xffffffff, 3, 0, 1, 1.0f);
            const char* s3 = GetString(0x413);
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, s3, x, row, w, rh, 0xffffffff, 3, 0, 1, 1.0f);
            row = (float)((double)row + D(0x5d0b68));
        } else {
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, GetString(kName[i]), x, row, w, rh, 0xffffffff, 2, 0, 0, 1.0f);
            const int32_t kind = kKind[i];
            if (any) {
                const int32_t count = kind == 2 ? c2 : kind == 0 ? c0 : kind == 3 ? c3 : 0;
                sprintf_(cell, (const char*)0x5ca2ac, (double)(float)count);
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, cell, (float)((double)x - ((double)colw + colw)), row, w, rh, 0xffffffff, 3, 0, 0, 1.0f);
            }
            sprintf_(cell, (const char*)0x5ca2ac, (double)Obj_ProductionRate(c, 0, kind));
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, cell, (float)((double)x - colw), row, w, rh, 0xffffffff, 3, 0, 0, 1.0f);
            sprintf_(cell, (const char*)0x5ca2ac, (double)Obj_ProductionRate(c, 0, kind) * level);
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, cell, x, row, w, rh, 0xffffffff, 3, 0, 0, 1.0f);
        }
        row = (float)((double)row + Fl(0x5d0b08));
    }
    const float fy = (float)((double)row + D(0x5d0b70));
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, GetString(0x414), x, fy, w, h, 0xffffffff, 2, 1, 0, 1.0f);
}
RT3_REPLACE(0x4cc340, CityInfo_DrawCb);

// 0x4cc930 (slider callback). Sets the edited city's two sizes (+0x25e /
// +0x25a) to v (EDX) and refills the editor.
void __fastcall CityEditor_SetSizeCb(void*, int32_t v) {
    if (hook::Global<int32_t>(0x6cff98) || !hook::Global<int32_t>(0x6cff74)) return;
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74));
    const float f = (float)v;
    F<float>(c, 0x25e) = f;
    F<float>(c, 0x25a) = f;
    CityEditor_Fill(hook::Global<uint8_t*>(0x6cff84), 0, hook::Global<int32_t>(0x6cff74), 1);
}
RT3_REPLACE(0x4cc930, CityEditor_SetSizeCb);

// 0x4cc980. The scenario editor's city page for city `id` (> 0 makes it current,
// 0x6cff74; an invalid or no current city falls back to the first; also 0x6cff78;
// busy flag 0x6cff98). Without cities only the "no cities" text (0x415). Else: the
// size slider 0x5086 (+0x25a, at most 400) and "%d%%" label, the "city N" title
// (0xbba, of `id` as passed); unless `keep`: the region-type list 0x508a (twelve
// names at world +0xfe7, stride 0x4e1, current +0x2f2), the house list 0x5079
// (enabled kind-2 types not tied to another region type, by the city's weight for
// them (Obj_NamedValue) highest first, ties by localized name), the style list
// 0x5083 (six names from 0x620ce8, current +0x272), the city list 0x507e sorted by
// name (marking the current; blocked cities +0x23e tinted) and the summary line.
RT3_NOINLINE void __fastcall CityEditor_Fill(uint8_t* w, int, int32_t id, int32_t keep) {
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    if (id > 0) hook::Global<int32_t>(0x6cff74) = id;
    if (!Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74))) hook::Global<int32_t>(0x6cff74) = 0;
    if (!hook::Global<int32_t>(0x6cff74) && Collection_Count(hook::Global<Collection*>(0x62bae0)) > 0)
        hook::Global<uint32_t>(0x6cff74) = Collection_IdAt(hook::Global<Collection*>(0x62bae0), 0, 0, 0);
    int32_t cur = hook::Global<int32_t>(0x6cff74);
    hook::Global<int32_t>(0x6cff78) = cur;
    if (!cur) {
        Window_SendEvent5(w, 0, 0x86, 0x4e21, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x4a3c, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4a3c, (uint32_t)(uintptr_t)GetString(0x415), 0, 0);
        return;
    }
    hook::Global<int32_t>(0x6cff98) = 1;
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)cur);
    Window_SendEvent5(w, 0, 0x7a, 0x5088, 0x4cc340, 0, 0);
    Window_SendEvent5(w, 0, 0x84, 0x5086, 0x4cc930, 0, 0);
    float size = *(const float*)0x5c8904 < F<float>(c, 0x25a) ? 400.0f : F<float>(c, 0x25a);
    Window_SendEvent5(w, 0, 0x66, 0x5086, (uint32_t)Ftol((double)size + *(const double*)0x5c86b8), 0, 0);
    char buf[0x200], num[0x60];
    ExeSprintf()(buf, (const char*)0x5d0b18, Ftol(F<float>(c, 0x25a)));
    Window_SendEvent5(w, 0, 0x6f, 0x5087, (uint32_t)(uintptr_t)buf, 0, 0);
    FormatText(buf, GetString(0xbba), Num_Format(num, 1, (double)id, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x4a44, (uint32_t)(uintptr_t)buf, 0, 0);
    if (keep) {
        hook::Global<int32_t>(0x6cff98) = 0;
        return;
    }
    Window_SendEvent5(w, 0, 0x7e, 0x5079, 0x4cbea0, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x5079, 0x4c9d60, 0, 0);
    CityDialog_UpdateButtons();
    Window_SendEvent5(w, 0, 0x8b, 0x508a, 0, 0, 0);
    for (int32_t off = 0; off < 0x3a8c; off += 0x4e1)
        Window_SendEvent5(w, 0, 0x88, 0x508a, (uint32_t)(uintptr_t)(hook::Global<uint8_t*>(0x6cec78) + 0xfe7 + off), 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x508a, F<uint32_t>(c, 0x2f2), 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x5079, 0, 0, 0);
    // Houses: the city's weight per type, then a selection sort.
    auto types = []() { return hook::Global<Collection*>(0x62b268); };
    auto type = [&](int32_t pos) { return (uint8_t*)Collection_Get(types(), 0, Collection_IdAt(types(), 0, (uint32_t)pos, 0)); };
    static float weight[1000];
    for (int32_t i = 0; i < Collection_Count(types()); ++i) weight[i] = (float)Obj_NamedValue(c, 0, (const char*)type(i) + 4);
    uint8_t* list = Widget_FindChild(w, 0, 0x5079);
    F<int32_t>(list, 0x182) = 1;
    uint8_t used[1000];
    memset(used, 0, sizeof used);
    for (int32_t i = 0; i < Collection_Count(types()); ++i) {
        uint8_t* t = type(i);
        if ((t[0xba] || t[0xbb]) && F<int32_t>(t, 0x794) != F<int32_t>(c, 0x2f2)) used[i] = 1;
    }
    char bestName[0x200];
    for (int32_t round = 0; round < Collection_Count(types()); ++round) {
        int32_t best = -1;
        for (int32_t k = 0; k < Collection_Count(types()); ++k) {
            if (used[k]) continue;
            uint8_t* t = type(k);
            if (t[0x32] != 2 || !F<int32_t>(t, 0x7ac)) continue;
            if (best != -1 && !(weight[k] > weight[best])) {
                if (weight[k] != weight[best]) continue;
                if (Stricmp(LocalizeName((const char*)t + 0x4f), bestName) >= 0) continue;
            }
            strcpy(bestName, LocalizeName((const char*)t + 0x4f));
            best = k;
        }
        if (best == -1) break;
        used[best] = 1;
        Window_SendEvent5(w, 0, 0x88, 0x5079, 0x5c87a8, Collection_IdAt(types(), 0, (uint32_t)best, 0), 0);
    }
    F<int32_t>(list, 0x182) = 0;
    ListView_RefreshVisible(list);
    Window_SendEvent5(w, 0, 0x8b, 0x5083, 0, 0, 0);
    for (const int32_t* s = (const int32_t*)0x620ce8; s < (const int32_t*)0x620d00; ++s)
        Window_SendEvent5(w, 0, 0x88, 0x5083, (uint32_t)(uintptr_t)GetString(*s), 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x5083, F<uint32_t>(c, 0x272), 0, 0);
    // Cities by name.
    Window_SendEvent5(w, 0, 0x8b, 0x507e, 0, 0, 0);
    memset(used, 0, sizeof used);
    int32_t n = Collection_Count(cities = hook::Global<Collection*>(0x62bae0));
    if (n > 999) n = 999;
    for (int32_t row = 0; row < n; ++row) {
        int32_t best = -1;
        for (int32_t k = 0; k < n; ++k) {
            if (used[k]) continue;
            if (best != -1) {
                Collection* cc = hook::Global<Collection*>(0x62bae0);
                uint8_t* a = (uint8_t*)Collection_Get(cc, 0, Collection_IdAt(cc, 0, (uint32_t)k, 0));
                uint8_t* b = (uint8_t*)Collection_Get(cc, 0, Collection_IdAt(cc, 0, (uint32_t)best, 0));
                if (Stricmp((const char*)a + 0x356, (const char*)b + 0x356) >= 0) continue;
            }
            best = k;
        }
        Collection* cc = hook::Global<Collection*>(0x62bae0);
        uint8_t* e = (uint8_t*)Collection_Get(cc, 0, Collection_IdAt(cc, 0, (uint32_t)best, 0));
        Window_SendEvent5(w, 0, 0x88, 0x507e, (uint32_t)(uintptr_t)(e + 0x356), F<uint32_t>(e, 0x23a), 0);
        used[best] = 1;
        if (F<int32_t>(e, 0x23a) == hook::Global<int32_t>(0x6cff74)) Window_SendEvent5(w, 0, 0x66, 0x507e, (uint32_t)row, 0, 0);
        Window_SendEvent5(w, 0, 0xaf, 0x507e, (uint32_t)row, F<int32_t>(e, 0x23e) ? 0xffffcfffu : 0xffffffffu, 0);
    }
    CityDialog_ShowSummary();
    hook::Global<int32_t>(0x6cff98) = 0;
}
RT3_REPLACE(0x4cc980, CityEditor_Fill);

// 0x4cd070. The territory editor for territory `id` (<= 0: the last one,
// 0x621f4c; at least 1; kept in 0x621f48 / 0x621f4c): list 0x50dc with every
// territory sorted by name (case-insensitive selection sort, at most 999; marks
// via event 0xaf) selecting it, list 0x50e5 with the other group leaders (+0x31)
// selecting its leader (+0x35; else the first); switches leader +0x31 / +0x2d on
// 0x50e0 / 0x50e1, value +0x39 in 0x50e2 and the controls enabled to match; the
// group line (string 0x417) on 0x50e5 and the "territory N" title (0xbbb, of the
// id as passed, before the clamp to 1) on 0x50e7.
RT3_NOINLINE void __fastcall TerritoryEditor_Show(uint8_t* w, int, int32_t id) {
    if (id <= 0) id = hook::Global<int32_t>(0x621f4c);
    int32_t shown = id;
    if (id < 1) id = 1;
    hook::Global<int32_t>(0x621f48) = id;
    hook::Global<int32_t>(0x621f4c) = id;
    // (The original's branch for id 0 here can't be reached after the clamp.)
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, (uint32_t)id);
    Window_SendEvent5(w, 0, 0xb0, 0x50e5, 0, 0, 0);
    Window_SendEvent5(w, 0, 0xb0, 0x50dc, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x50dc, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x50e5, 0, 0, 0);
    uint8_t used[1000];
    memset(used, 0, sizeof used);
    int32_t n = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
    if (n > 999) n = 999;
    else n = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
    int32_t leaders = 0;
    for (int32_t row = 0; row < n; ++row) {
        int32_t best = -1;
        for (int32_t k = 0; k < n; ++k) {
            if (used[k]) continue;
            if (best != -1) {
                Collection* c = hook::Global<Collection*>(0x6cfc9c);
                uint8_t* a = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, k, 0));
                uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, best, 0));
                if (Stricmp((const char*)a + 4, (const char*)b + 4) >= 0) continue;
            }
            best = k;
        }
        Collection* c = hook::Global<Collection*>(0x6cfc9c);
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, best, 0));
        Window_SendEvent5(w, 0, 0x88, 0x50dc, (uint32_t)(uintptr_t)(e + 4), F<uint32_t>(e, 0), 0);
        if (F<int32_t>(e, 0x31) && F<int32_t>(e, 0) != hook::Global<int32_t>(0x621f48)) {
            Window_SendEvent5(w, 0, 0x88, 0x50e5, (uint32_t)(uintptr_t)(e + 4), F<uint32_t>(e, 0), 0);
            if (F<int32_t>(e, 0) == F<int32_t>(t, 0x35) || !leaders) Window_SendEvent5(w, 0, 0x66, 0x50e5, (uint32_t)leaders, 0, 0);
            ++leaders;
        }
        used[best] = 1;
        if (F<int32_t>(e, 0) == hook::Global<int32_t>(0x621f48)) Window_SendEvent5(w, 0, 0x66, 0x50dc, (uint32_t)row, 0, 0);
        Window_SendEvent5(w, 0, 0xaf, 0x50dc, (uint32_t)row, 0xffffffff, 0);
    }
    Window_SendEvent5(w, 0, 0x74, 0x50e0, F<uint32_t>(t, 0x31), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x50e1, F<uint32_t>(t, 0x2d), 0, 0);
    char buf[0x1a0], text[0x1e0];
    Num_Format(buf, 8, (double)F<int32_t>(t, 0x39), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x50e2, (uint32_t)(uintptr_t)buf, 0, 0);
    if (!F<int32_t>(t, 0x31)) {
        for (uint32_t i = 0x50e1; i <= 0x50e4; ++i) Window_SendEvent5(w, 0, 0x87, i, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x50e5, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x50e6, 3, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x65, 0x50e1, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x50e5, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x50e6, 3, 0, 0);
        uint32_t s = F<int32_t>(t, 0x2d) ? 0x65 : 0x87;
        for (uint32_t i = 0x50e2; i <= 0x50e4; ++i) Window_SendEvent5(w, 0, s, i, 3, 0, 0);
    }
    uint8_t* leader = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, F<uint32_t>(t, 0x35));
    FormatText(text, GetString(0x417), (const char*)t + 4, (const char*)leader + 4, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0x50e5, (uint32_t)(uintptr_t)text, 0, 0);
    FormatText(text, GetString(0xbbb), Num_Format(buf, 1, (double)shown, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x50e7, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4cd070, TerritoryEditor_Show);

// 0x4cd570. Toggle 0xa410 + idx for record idx of 0x6ada84, ticked when the world's
// list +0x66b6 has its name (+0x45 of its +0x72 record).
void __fastcall SetupToggle_AddNamed2(uint8_t* d, int, int32_t idx, float x, float y, float w, float h) {
    uint8_t* win = SetupToggle_Add(d, idx, x, y, w, h, 0xa410, 0x4e26);
    void* r = Collection_Get(hook::Global<Collection*>(0x6ada84), 0, idx);
    int32_t on = World_LookupNamed2(WorldG(), 0, (const char*)Obj_Ref72Plus45(r));
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x74, idx + 0xa410, (uint32_t)on, 0, 0);
    (void)win;
}
RT3_REPLACE(0x4cd570, SetupToggle_AddNamed2);

// 0x4cd680. The scenario editor's industry list 0x5b05 (callbacks 0x4cd570 /
// 0x4ca960; refilled unless `keep`): the 0x6ada84 entries sorted by their type's
// (0x62ba88 via +0x72) +0x105, highest first (selection sort, ties keep order), as
// "<name> (<+0x105> - <+0x109>) [NA, E, W]" (flags +0x30 / +0x31 / +0x32), marked
// via event 0xaf; switches world +0x4c97..+0x4c99 on 0x5b09..0x5b0b; "N of M
// enabled" (string 0x41a; enabled = listed in the world's +0x66b6 table) on label
// 0x5b04 of the city dialog (0x6cff84).
RT3_NOINLINE void __fastcall EditorDialog_FillIndustries(uint8_t* w, int, int32_t keep) {
    Window_SendEvent5(w, 0, 0x7e, 0x5b05, 0x4cd570, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x5b05, 0x4ca960, 0, 0);
    for (int32_t k = 0; k < 3; ++k)
        Window_SendEvent5(w, 0, 0x74, 0x5b09 + k, hook::Global<uint8_t*>(0x6cec78)[0x4c97 + k], 0, 0);
    if (!keep) Window_SendEvent5(w, 0, 0x8b, 0x5b05, 0, 0, 0);
    uint8_t used[1000];
    memset(used, 0, sizeof used);
    auto list = []() { return hook::Global<Collection*>(0x6ada84); };
    auto entry = [&](int32_t pos) { return (uint8_t*)Collection_Get(list(), 0, Collection_IdAt(list(), 0, (uint32_t)pos, 0)); };
    auto type = [](uint8_t* e) {
        return F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72)) : nullptr;
    };
    int32_t enabled = 0, listed = 0, row = 0;
    for (int32_t round = 0; round < Collection_Count(list()); ++round) {
        int32_t best = -1;
        for (int32_t k = 0; k < Collection_Count(list()); ++k) {
            if (used[k]) continue;
            uint8_t* t = type(entry(k));
            if (best != -1 && F<int32_t>(t, 0x105) <= F<int32_t>(type(entry(best)), 0x105)) continue;
            best = k;
        }
        if (best == -1) break;
        ++listed;
        used[best] = 1;
        if (!keep) {
            uint8_t* e = entry(best);
            int32_t hi = F<int32_t>(type(e), 0x109);
            int32_t lo = F<int32_t>(type(e), 0x105);
            const char* name = LocalizeName((const char*)type(e) + 0x45);
            char a[0x60], b[0x60], text[0x700], fl[0x320];
            char* sh = Num_Format(a, 8, (double)hi, 10, 1, 0, 0);
            char* sl = Num_Format(b, 8, (double)lo, 10, 1, 0, 0);
            FormatText(text, (const char*)0x5d0b80, name, sl, sh, 0, 0, 0, 0, 0);
            fl[0] = 0;
            if (e[0x30]) memcpy(fl + strlen(fl), (const void*)0x5d0b7c, 3);
            if (e[0x31]) {
                if (strlen(fl)) memcpy(fl + strlen(fl), (const void*)0x5c9f38, 3);
                memcpy(fl + strlen(fl), (const void*)0x5cbc30, 2);
            }
            if (e[0x32]) {
                if (strlen(fl)) memcpy(fl + strlen(fl), (const void*)0x5c9f38, 3);
                memcpy(fl + strlen(fl), (const void*)0x5cbbe8, 2);
            }
            memcpy(text + strlen(text), (const void*)0x5d09c4, 2);
            strcat(text, fl);
            memcpy(text + strlen(text), (const void*)0x5d0b78, 2);
            Window_SendEvent5(w, 0, 0x88, 0x5b05, (uint32_t)(uintptr_t)text, Collection_IdAt(list(), 0, (uint32_t)best, 0), 0);
            Window_SendEvent5(w, 0, 0xaf, 0x5b05, (uint32_t)row, 0xffffffff, 0);
            ++row;
        }
        if (World_LookupNamed2(hook::Global<void*>(0x6cec78), 0, (const char*)Obj_Ref72Plus45(entry(best)))) ++enabled;
    }
    char se[0x60], sn[0x60], text[0x200];
    char* pn = Num_Format(sn, 1, (double)listed, 10, 1, 0, 0);
    char* pe = Num_Format(se, 1, (double)enabled, 10, 1, 0, 0);
    FormatText(text, GetString(0x41a), pe, pn, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x6f, 0x5b04, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4cd680, EditorDialog_FillIndustries);

// 0x4cdbd0. The scenario editor's house list 0x9c42 (callbacks 0x4cc130 /
// 0x4c9d80; refilled unless `keep`): the kind-2 building types (0x62b268, not
// +0xba / +0xbb) sorted by localized name (case-insensitive selection sort) as
// "<name> (<cities>)" with a "<n> in: <city>, <city>..." line (string 0x41b, up to
// eight cities, "..." beyond; sic: each city name is used as a format string),
// greyed (event 0xaf) when no city has it; "N of M enabled" (string 0x41c; enabled
// = in the world's +0x66b2 table) on label 0x9c41 of the city dialog (0x6cff84).
RT3_NOINLINE void __fastcall EditorDialog_FillHouses(uint8_t* w, int, int32_t keep) {
    Window_SendEvent5(w, 0, 0x7e, 0x9c42, 0x4cc130, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x9c42, 0x4c9d80, 0, 0);
    if (!keep) Window_SendEvent5(w, 0, 0x8b, 0x9c42, 0, 0, 0);
    uint8_t used[1000];
    memset(used, 0, sizeof used);
    auto types = []() { return hook::Global<Collection*>(0x62b268); };
    auto entry = [&](int32_t pos) { return (uint8_t*)Collection_Get(types(), 0, Collection_IdAt(types(), 0, (uint32_t)pos, 0)); };
    int32_t enabled = 0, picks = 0, row = 0;
    for (int32_t round = 0; round < Collection_Count(types()); ++round) {
        int32_t best = -1;
        for (int32_t k = 0; k < Collection_Count(types()); ++k) {
            if (used[k]) continue;
            uint8_t* t = entry(k);
            if (t[0x32] != 2 || t[0xba] || t[0xbb]) continue;
            if (best != -1) {
                uint8_t* b = entry(best);
                const char* nk = LocalizeName((const char*)t + 0x4f);
                const char* nb = LocalizeName((const char*)b + 0x4f);
                if (Stricmp(nk, nb) >= 0) continue;
            }
            best = k;
        }
        if (best == -1) break;
        ++picks;
        used[best] = 1;
        if (!keep) {
            int32_t cities = Table_CountListing(hook::Global<Collection*>(0x62bae0), 0, Collection_IdAt(types(), 0, (uint32_t)best, 0));
            const char* name = LocalizeName((const char*)entry(best) + 0x4f);
            char num[0x60], title[0x3e0], line[0x3e8], city[0x110];
            FormatText(title, (const char*)0x5d0b40, name, Num_Format(num, 1, (double)cities, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
            const char* raw = (const char*)entry(best) + 4;
            FormatText(line, GetString(0x41b), Num_Format(num, 1, (double)cities, 10, 1, 0, 0), raw, 0, 0, 0, 0, 0, 0);
            for (int32_t i = 0; i < (cities > 8 ? 8 : cities); ++i) {
                memcpy(line + strlen(line), (const void*)(i ? 0x5c9f38 : 0x5c9ce8), i ? 3 : 2);
                uint8_t* c = (uint8_t*)Table_NthListing(hook::Global<Collection*>(0x62bae0), 0,
                                                       Collection_IdAt(types(), 0, (uint32_t)best, 0), i + 1);
                FormatText(city, (const char*)c + 0x356, 0, 0, 0, 0, 0, 0, 0, 0);
                strcat(line, city);
            }
            if (cities > 8) memcpy(line + strlen(line), (const void*)0x5d0b8c, 4);
            Window_SendEvent5(w, 0, 0x88, 0x9c42, (uint32_t)(uintptr_t)title, Collection_IdAt(types(), 0, (uint32_t)best, 0),
                              (uint32_t)(uintptr_t)line);
            Window_SendEvent5(w, 0, 0xaf, 0x9c42, (uint32_t)row, cities ? 0xffffffffu : 0xff9f9f9fu, 0);
            ++row;
        }
        if (World_LookupNamed1(hook::Global<void*>(0x6cec78), 0, (const char*)entry(best) + 4)) ++enabled;
    }
    char se[0x60], sp[0x60], text[0x200];
    char* pp = Num_Format(sp, 1, (double)picks, 10, 1, 0, 0);
    char* pe = Num_Format(se, 1, (double)enabled, 10, 1, 0, 0);
    FormatText(text, GetString(0x41c), pe, pp, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x6f, 0x9c41, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4cdbd0, EditorDialog_FillHouses);

// 0x4ce070. Switches the map editor to tab 0..19 (+0x78, also 0x6cff80) unless
// locked (+0x7c): leaving tab 9 runs its exit hook, leaving tab 11 (rules) only
// when the rule could be stored. city is the city for tab 1 (-1: the last edited,
// 0x6cff74, else the first). Resets panel 0x4a3c, highlights tab button 0x4e20 +
// tab (0x6d, others 0x86) and sets the tab up.
RT3_NOINLINE void __fastcall EditorDialog_SetTab(uint8_t* w, int, int32_t tab, int32_t city) {
    if (F<int32_t>(w, 0x7c)) return;
    int32_t cur = F<int32_t>(w, 0x78);
    if (cur == 9) Nop4cf900(w);
    else if (cur == 11 && !RulesDialog_StoreRule(w)) return;
    if (city == -1 && tab == 1) {
        if (Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74)))
            city = hook::Global<int32_t>(0x6cff74);
        else if (Collection_Count(hook::Global<Collection*>(0x62bae0)))
            city = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x62bae0), 0, 0, 0);
        else
            city = 0;
    }
    Window_SendEvent5(w, 0, 0x6f, 0x4a3c, 0x5c87a8, 0, 0);
    Window_SendEvent5(w, 0, 0x87, 0x4a3c, 3, 0, 0);
    for (int32_t i = 0; i < 0x14; ++i) Window_SendEvent5(w, 0, i == tab ? 0x6d : 0x86, 0x4e20 + i, 3, 0, 0);
    switch ((uint32_t)tab) {
    case 0: EditorDialog_ShowSettings(w); break;
    case 1: CityEditor_Fill(w, 0, city, 0); break;
    case 2: TerritoryEditor_Show(w, 0, city); break;
    case 3: NewGameDialog_SetupRows(w); break;
    case 4: EditorDialog_FillCargoList(w); break;
    case 5: EditorDialog_ShowEconomy(w); break;
    case 6: EditorDialog_FillIndustries(w, 0, 0); break;
    case 7: EditorDialog_FillHouses(w, 0, 0); break;
    case 8: EditorDialog_ShowAchievements(w, 0, 0); break;
    case 9: EditorDialog_ShowRegionTypes(w); break;
    case 10: Window_ShowWorldPicks(w); break;
    case 11: RulesDialog_Init(w); break;
    case 12: Dialog_SetCallback4a43(w); break;
    case 13: Dialog_SetCallback4a42(w); break;
    case 14: Dialog_SetCallback4a41(w); break;
    case 15: Dialog_SetCallback4a3e(w); break;
    case 16: Dialog_SetCallback4a3b(w); break;
    case 17: Dialog_SetupControl5141(w); break;
    case 18: Dialog_SetCallback4a3f(w); break;
    case 19: Dialog_SetCallback4a40(w); break;
    }
    F<int32_t>(w, 0x78) = tab;
    hook::Global<int32_t>(0x6cff80) = tab;
}
RT3_REPLACE(0x4ce070, EditorDialog_SetTab);

// 0x4ce380. City page of the scenario editor dialog w: event handler for the
// selected city 0x6cff74 of 0x62bae0. Clicks (0xcb): clear / fill every cargo
// amount (0x507a / 0x507b, 0 or 1 for each cargo of 0x62b268 by name), copy the
// amounts of another city picked from a name-sorted list (0x507c, string 0x41f,
// at most 999 cities), rename (0x507f, text box 9, string 0x41e, 40
// characters), previous / next city (0x5084 / 0x5085, wrapping in list
// 0x507e). Changes (0xca): list pick (0x507e), the value +0x272 (0x5083) and
// the port count (0x508a). Returns 1 for unhandled events.
RT3_NOINLINE uint32_t __fastcall CityEditor_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    if (ev[0] == 0xca) {
        switch (ev[1]) {
        case 0x507e:
            EditorDialog_SetTab(w, 0, 1, (int32_t)ev[3]);
            return 0;
        case 0x5083:
            if (hook::Global<uint32_t>(0x6cff74))
                F<uint32_t>((uint8_t*)Collection_Get(cities, 0, hook::Global<uint32_t>(0x6cff74)), 0x272) = ev[2];
            return 0;
        case 0x508a:
            if (hook::Global<uint32_t>(0x6cff74))
                Obj_NumberPorts(Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74)), 0, (int32_t)ev[2]);
            CityEditor_Fill(w, 0, hook::Global<int32_t>(0x6cff74), 0);
            return 0;
        }
        return 1;
    }
    if (ev[0] != 0xcb) return 1;
    switch (ev[1]) {
    case 0x507a:
    case 0x507b: {
        void* city = Collection_Get(cities, 0, hook::Global<uint32_t>(0x6cff74));
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
            Collection* cargos = hook::Global<Collection*>(0x62b268);
            uint8_t* cargo = (uint8_t*)Collection_Get(cargos, 0, Collection_IdAt(cargos, 0, i, 0));
            double amount = ev[1] == 0x507a ? 0.0 : 1.0;
            Obj_SetNamedAmount(city, 0, (const char*)cargo + 4, (float)amount);
        }
        CityEditor_Fill(w, 0, hook::Global<int32_t>(0x6cff74), 0);
        return 0;
    }
    case 0x507c: {
        int32_t n = Collection_Count(cities);
        if (n > 999) n = 999;
        uint8_t used[1000];
        memset(used, 0, sizeof used);
        const char** names = (const char**)Malloc(n * 4);
        uint32_t* ids = (uint32_t*)Malloc(n * 4);
        int32_t count = 0;
        for (int32_t pass = 0; pass < n; ++pass) {
            int32_t best = -1;
            for (int32_t j = 0; j < n; ++j) {
                if (used[j]) continue;
                if (best != -1) {
                    Collection* c = hook::Global<Collection*>(0x62bae0);
                    uint8_t* a = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0));
                    uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, best, 0));
                    if (Stricmp((const char*)a + 0x356, (const char*)b + 0x356) >= 0) continue;
                }
                best = j;
            }
            Collection* c = hook::Global<Collection*>(0x62bae0);
            uint8_t* rec = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, best, 0));
            if (F<uint32_t>(rec, 0x23a) != hook::Global<uint32_t>(0x6cff74)) {
                names[count] = (const char*)rec + 0x356;
                ids[count] = F<uint32_t>(rec, 0x23a);
                ++count;
            }
            used[best] = 1;
        }
        int32_t sel = 0;
        if (Dialog_RunMessage(GetString(0x41f), 2, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0, count, names, &sel, 0, 0, nullptr, 0) == 0x3f2) {
            void* other = Collection_Get(hook::Global<Collection*>(0x62bae0), 0, ids[sel]);
            Obj_CopyNamedAmounts(Collection_Get(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff74)), 0, other);
            EditorDialog_SetTab(w, 0, 1, -1);
        }
        Free(names);
        Free(ids);
        return 0;
    }
    case 0x507f:
        if (!hook::Global<uint32_t>(0x6cff74)) return 0;
        Dialog_RunMessage(GetString(0x41e), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f,
                          (uint32_t)(uintptr_t)((uint8_t*)Collection_Get(cities, 0, hook::Global<uint32_t>(0x6cff74)) + 0x356), 0x28, 0, nullptr, nullptr, 0, 0,
                          nullptr, 0);
        CityEditor_Fill(w, 0, hook::Global<int32_t>(0x6cff74), 0);
        return 0;
    case 0x5084:
    case 0x5085:
        if (uint8_t* list = Widget_FindChild(w, 0, 0x507e)) {
            int32_t dir = ev[1] != 0x5084 ? 1 : -1;
            int32_t n = F<int32_t>(list, 0x1d2);
            Window_SendEvent5(w, 0, 0x66, 0x507e, (uint32_t)((dir + F<int32_t>(list, 0x1d6) + n) % n), 0, 0);
            EditorDialog_SetTab(w, 0, 1, (int32_t)TreeView_SelectedData(list));
        }
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4ce380, CityEditor_HandleEvent);

// 0x4ce820. Territory page of the scenario editor dialog w: event handler for
// the selected territory 0x621f48 of 0x6cfc9c. Clicks (0xcb): rename (0x50dd,
// text box 9, string 0x41e, 40 characters), previous / next territory
// (0x50de / 0x50df, wrapping in list 0x50dc). Changes (0xca): list pick
// (0x50dc), the region toggle +0x31 (0x50e0; turning it off is refused while
// other territories belong to it: 0x420 / 0x421 with their count), the toggle
// +0x2d (0x50e1), the value +0x39 from a text box (0x50e2, absolute) and the
// owner +0x35 (0x50e5). Returns 1 for unhandled events.
RT3_NOINLINE uint32_t __fastcall TerritoryEditor_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint8_t* rec = hook::Global<uint32_t>(0x621f48) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, hook::Global<uint32_t>(0x621f48)) : nullptr;
    if (ev[0] == 0xcb) {
        uint32_t id = ev[1];
        if (id == 0x50dd) {
            if (hook::Global<uint32_t>(0x621f48)) {
                Dialog_RunMessage(GetString(0x41e), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)(rec + 4), 0x28, 0, nullptr, nullptr, 0, 0,
                                  nullptr, 0);
                TerritoryEditor_Show(w, 0, hook::Global<int32_t>(0x621f48));
            }
            return 0;
        }
        if ((int32_t)id < 0x50dd || (int32_t)id > 0x50df) return 1;
        if (uint8_t* list = Widget_FindChild(w, 0, 0x50dc)) {
            int32_t dir = id != 0x50de ? 1 : -1;
            int32_t n = F<int32_t>(list, 0x1d2);
            Window_SendEvent5(w, 0, 0x66, 0x50dc, (uint32_t)((dir + F<int32_t>(list, 0x1d6) + n) % n), 0, 0);
            EditorDialog_SetTab(w, 0, 2, (int32_t)TreeView_SelectedData(list));
        }
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    switch (ev[1]) {
    case 0x50dc:
        EditorDialog_SetTab(w, 0, 2, (int32_t)ev[3]);
        return 0;
    case 0x50e0: {
        if (!rec) return 0;
        F<int32_t>(rec, 0x31) = F<int32_t>(rec, 0x31) == 0;
        if (!F<int32_t>(rec, 0x31)) {
            int32_t count = 0;
            char first[0x1b0];
            for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfc9c)); ++i) {
                uint8_t* t = (uint8_t*)Table_At(hook::Global<Collection*>(0x6cfc9c), 0, i);
                if (F<int32_t>(t, 0x31) || F<uint32_t>(t, 0) == hook::Global<uint32_t>(0x621f48) || F<uint32_t>(t, 0x35) != hook::Global<uint32_t>(0x621f48))
                    continue;
                if (!count) strcpy(first, (const char*)t + 4);
                ++count;
            }
            if (count >= 1) {
                F<int32_t>(rec, 0x31) = 1;
                char text[0x160];
                if (count == 1)
                    FormatText(text, GetString(0x420), first, 0, 0, 0, 0, 0, 0, 0);
                else {
                    char num[0x100];
                    const char* sn = Num_Format(num, 1, (double)count, 10, 1, 0, 0);
                    FormatText(text, GetString(0x421), sn, first, 0, 0, 0, 0, 0, 0);
                }
                Message_Show(text, 1, 0);
            }
        }
        Region_PickOther(rec);
        TerritoryEditor_Show(w, 0, hook::Global<int32_t>(0x621f48));
        return 0;
    }
    case 0x50e1:
        if (!rec) return 0;
        F<int32_t>(rec, 0x2d) = F<int32_t>(rec, 0x2d) == 0;
        TerritoryEditor_Show(w, 0, hook::Global<int32_t>(0x621f48));
        return 0;
    case 0x50e2: {
        int32_t v = ((int(__cdecl*)(const char*))0x5a1ea5)(F<const char*>((uint8_t*)(uintptr_t)ev[11], 0xba));
        F<int32_t>(rec, 0x39) = v < 0 ? -v : v;
        TerritoryEditor_Show(w, 0, hook::Global<int32_t>(0x621f48));
        return 0;
    }
    case 0x50e5:
        F<uint32_t>(rec, 0x35) = ev[3];
        TerritoryEditor_Show(w, 0, hook::Global<int32_t>(0x621f48));
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4ce820, TerritoryEditor_HandleEvent);

// 0x4ceb90. Steps the player portrait picker (control 0x5a7e, current +0x4f) by
// `dir` through the 41 portrait slots (table 0x5f2d28, 0x27 bytes: taken flag, kind,
// name / description strings at +0x1f / +0x23), skipping taken ones (at most 2000
// tries); slot 0 is never marked taken. Shows the portrait and its name (0x5a7f),
// stores it for player row +0x80 (world +0x69d8 + row * 9) and, when that player
// exists, copies portrait, kind (+0x15c), name (+8) and description (+0x27) into it.
RT3_NOINLINE void __fastcall NewGameDialog_StepPortrait(uint8_t* w, int, int32_t dir) {
    uint8_t* slots = (uint8_t*)0x5f2d28;
    int32_t cur = F<int32_t>(Widget_FindChild(w, 0, 0x5a7e), 0x4f);
    slots[cur * 0x27] = 0;
    for (int32_t tries = 0;;) {
        cur = (cur + dir + 0x29) % 0x29;
        if (slots[cur * 0x27] != 1) break;
        if (tries++ >= 0x7d0) break;
    }
    if (cur) slots[cur * 0x27] = 1;
    uint8_t* s = slots + cur * 0x27;
    Window_SendEvent5(w, 0, 0x7c, 0x5a7e, (uint32_t)cur, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5a7f, (uint32_t)(uintptr_t)GetString(F<int32_t>(s, 0x1f)), 0, 0);
    int32_t row = F<int32_t>(w, 0x80);
    hook::Global<uint8_t*>(0x6cec78)[0x69d8 + row * 9] = (uint8_t)cur;
    if (!Collection_IsValid(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(F<int32_t>(w, 0x80) + 1))) return;
    uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(F<int32_t>(w, 0x80) + 1));
    p[0x291] = (uint8_t)cur;
    strcpy((char*)p + 8, GetString(F<int32_t>(s, 0x1f)));
    strcpy((char*)p + 0x27, GetString(F<int32_t>(s, 0x23)));
    p[0x15c] = s[1];
}
RT3_REPLACE(0x4ceb90, NewGameDialog_StepPortrait);

// 0x4cecc0. Player page of the new-game dialog w: event handler for the 16
// player rows (world +0x69d8, 9 bytes: portrait, human, AI-allowed, AI, value
// +4; count +0x6a6c). Clicks (0xcb): add a row in the first free slot (0x5a72;
// its player record in 0x6ceb9c is made if needed and set from the portrait
// table 0x5f2d28), portrait back / forward (0x5a73 / 0x5a74), remove the row
// picked in list 0x5a6e (0x5a80; frees its portrait, resets the value to
// 100000 and clears the player). Changes (0xca) need a selected row +0x80
// (else 0x422), except the value text box 0x5a7c: human on / off (0x5a77 /
// 0x5a78), AI only (0x5a79), AI-allowed only (0x5a7a), both (0x5a7b).
// Returns 1 for unhandled events.
RT3_NOINLINE uint32_t __fastcall NewGameDialog_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    auto row = [](int32_t i) { return hook::Global<uint8_t*>(0x6cec78) + 0x69d8 + i * 9; };
    if (ev[0] == 0xca) {
        uint32_t id = ev[1];
        if (id != 0x5a7d && id != 0x5a7c && !F<int32_t>(w, 0x80)) {
            Message_Show(GetString(0x422), 1, 0);
            return 0;
        }
        switch (id) {
        case 0x5a77: row(F<int32_t>(w, 0x80))[1] = 1; break;
        case 0x5a78: row(F<int32_t>(w, 0x80))[1] = 0; break;
        case 0x5a79:
            row(F<int32_t>(w, 0x80))[3] = 1;
            row(F<int32_t>(w, 0x80))[2] = 0;
            break;
        case 0x5a7a:
            row(F<int32_t>(w, 0x80))[3] = 0;
            row(F<int32_t>(w, 0x80))[2] = 1;
            break;
        case 0x5a7b:
            row(F<int32_t>(w, 0x80))[3] = 1;
            row(F<int32_t>(w, 0x80))[2] = 1;
            break;
        case 0x5a7c: {
            int32_t v = ((int(__cdecl*)(const char*))0x5a1ea5)(F<const char*>((uint8_t*)(uintptr_t)ev[11], 0xba));
            F<int32_t>(row(F<int32_t>(w, 0x80)), 4) = v;
            return 0;
        }
        default:
            return 1;
        }
        NewGameDialog_CountRows(w);
        return 0;
    }
    if (ev[0] != 0xcb) return 1;
    switch (ev[1]) {
    case 0x5a72: {
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c) >= 0x10) return 0;
        int32_t i = 0;
        while (row(i)[3] || row(i)[2])
            if (++i >= 0x10) return 1;
        uint32_t pid = (uint32_t)(i + 1);
        Collection* players = hook::Global<Collection*>(0x6ceb9c);
        if (!Collection_IsValid(players, 0, pid)) {
            players = hook::Global<Collection*>(0x6ceb9c);
            pid = ((uint32_t(__thiscall*)(Collection*, int32_t, int32_t, int32_t, int32_t))(*(void***)players)[1])(players, 0, 0, 0, 0);
        }
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, pid);
        int32_t k = (int8_t)row(i)[0];
        uint8_t* slot = (uint8_t*)0x5f2d28 + k * 0x27;
        const char* desc = GetString(F<int32_t>(slot, 0x23));
        const char* name = GetString(F<int32_t>(slot, 0x1f));
        Player_Init(p, 0, (int32_t)pid, slot[1], name, desc, k, 1);
        ++F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c);
        row(i)[3] = 1;
        row(i)[2] = 1;
        row(i)[1] = 1;
        NewGameDialog_ListHumanRows(w);
        Window_SendEvent5(w, 0, 0x68, 0x5a6e, (uint32_t)i, 0, 0);
        return 0;
    }
    case 0x5a73:
        NewGameDialog_StepPortrait(w, 0, -1);
        return 0;
    case 0x5a74:
        NewGameDialog_StepPortrait(w, 0, 1);
        return 0;
    case 0x5a80: {
        uint8_t* list = Widget_FindChild(w, 0, 0x5a6e);
        int32_t sel = F<int32_t>(list, 0x1d6);
        int32_t i = (int32_t)TreeView_SelectedData(list);
        uint8_t* r = row(i);
        *((uint8_t*)0x5f2d28 + (int8_t)r[0] * 0x27) = 0;
        F<uint32_t>(r, 0) = 0;
        F<uint32_t>(r, 4) = 0;
        r[8] = 0;
        F<int32_t>(row(i), 4) = 100000;
        if (Collection_IsValid(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(i + 1))) {
            uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(i + 1));
            F<uint32_t>(p, 4) = 0;
            memset(p + 0x15d, 0, 0x80);
        }
        --F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c);
        if (sel == F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c)) --sel;
        NewGameDialog_ListHumanRows(w);
        Window_SendEvent5(w, 0, 0x66, 0x5a6e, (uint32_t)sel, 0, 0);
        return 0;
    }
    }
    return 1;
}
RT3_REPLACE(0x4cecc0, NewGameDialog_HandleEvent);

// 0x4cf0d0. Industry page of the scenario editor dialog w (table 0x6ada84,
// world flags +0x4c97..+0x4c99 for the three groups +0x30..+0x32):
// clicks 0x5b06..0x5b08 set every type's flag to its group membership,
// 0x5b0c clears the groups and all flags; changes 0xa410.. toggle one type
// (turning off a type of an enabled group is refused with 0xbbc), 0x5b09..
// 0x5b0b toggle a group (turning one on enables its types). Returns 1 for
// unhandled events.
RT3_NOINLINE uint32_t __fastcall EditorDialog_OnIndustryEvent(uint8_t* w, int, uint32_t* ev) {
    Collection* types = hook::Global<Collection*>(0x6ada84);
    auto typeAt = [](int32_t i) {
        Collection* t = hook::Global<Collection*>(0x6ada84);
        return (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
    };
    auto setFlag = [](uint8_t* r, int32_t v) { World_SetNamedFlag(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)Obj_Ref72Plus45(r), v); };
    int32_t id = (int32_t)ev[1];
    if (ev[0] == 0xcb) {
        if (id < 0x5b06) return 1;
        if (id <= 0x5b08) {
            for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) {
                uint8_t* r = typeAt(i);
                setFlag(r, r[0x30 + (id - 0x5b06)]);
            }
            EditorDialog_FillIndustries(w, 0, 0);
            return 0;
        }
        if (id != 0x5b0c) return 1;
        hook::Global<uint8_t*>(0x6cec78)[0x4c99] = 0;
        hook::Global<uint8_t*>(0x6cec78)[0x4c98] = 0;
        hook::Global<uint8_t*>(0x6cec78)[0x4c97] = 0;
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) setFlag(typeAt(i), 0);
        EditorDialog_FillIndustries(w, 0, 0);
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    if (id >= 0xa410 && id <= 0xa7f7) {
        uint8_t* box = (uint8_t*)(uintptr_t)ev[11];
        uint8_t* r = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(box, 0x82));
        int32_t on = (int32_t)ev[2];
        uint8_t* wd = hook::Global<uint8_t*>(0x6cec78);
        if (!on && ((r[0x30] && wd[0x4c97]) || (r[0x31] && wd[0x4c98]) || (r[0x32] && wd[0x4c99]))) {
            F<int32_t>(box, 0xcc) = 1;
            Message_Show(GetString(0xbbc), 1, 0);
            EditorDialog_FillIndustries(w, 0, 1);
            return 0;
        }
        setFlag(r, on);
        EditorDialog_FillIndustries(w, 0, 1);
        return 0;
    }
    if (id < 0x5b09 || id > 0x5b0b) return 1;
    uint8_t& group = hook::Global<uint8_t*>(0x6cec78)[0x4c97 + (id - 0x5b09)];
    group = group == 0;
    if (!hook::Global<uint8_t*>(0x6cec78)[0x4c97 + (id - 0x5b09)]) return 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) {
        uint8_t* r = typeAt(i);
        if (int32_t v = r[0x30 + (id - 0x5b09)]) setFlag(r, v);
    }
    EditorDialog_FillIndustries(w, 0, 0);
    return 0;
}
RT3_REPLACE(0x4cf0d0, EditorDialog_OnIndustryEvent);

// 0x4cf430. The editor's house-types page event handler: a click (0xca) on a type
// checkbox (0xa028..0xa40f; the type id at the control's +0x82) sets that type's
// availability to the event's value, and the all / none buttons (0x9c43 / 0x9c44,
// click or 0xcb) set every kind-2 building type; the page is refilled (keeping
// its scroll position for a single checkbox). 0 when handled, else 1.
RT3_NOINLINE int32_t __fastcall EditorDialog_HousesEvent(uint8_t* w, int, int32_t* ev) {
    if (ev[0] != 0xca && ev[0] != 0xcb) return 1;
    if (ev[0] == 0xca && ev[1] >= 0xa028 && ev[1] <= 0xa40f) {
        uint8_t* type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>((uint8_t*)(uintptr_t)ev[0xb], 0x82));
        World_SetNamedValue(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)type + 4, ev[2]);
        EditorDialog_FillHouses(w, 0, 1);
        return 0;
    }
    if (ev[1] < 0x9c43 || ev[1] > 0x9c44) return 1;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
        Collection* c = hook::Global<Collection*>(0x62b268);
        uint8_t* type = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (type[0x32] == 2) World_SetNamedValue(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)type + 4, ev[1] == 0x9c43 ? 1 : 0);
    }
    EditorDialog_FillHouses(w, 0, 0);
    return 0;
}
RT3_REPLACE(0x4cf430, EditorDialog_HousesEvent);

void __fastcall Editor_SetTabCb(int32_t v) {
    if (v != -1 && hook::Global<uint8_t*>(0x6cff84)) EditorDialog_SetTab(hook::Global<uint8_t*>(0x6cff84), 0, v, -1);
}
RT3_REPLACE(0x4cf530, Editor_SetTabCb);

// 0x4cf550. Sets up the map editor (layout 0x5d0b90): the tab list 0x4a3a with
// its callback and the 20 tabs of table 0x621f5c {name id, tip id, value}, the
// language tweaks (wider controls in languages 1, 2 and 4), the title 0xf3d,
// then selects tab (list notifications muted by +0x7c) and opens it on city.
RT3_NOINLINE void __fastcall EditorDialog_Init(uint8_t* w, int, int32_t tab, int32_t city) {
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d0b90);
    F<int32_t>(w, 0x7c) = 0;
    F<int32_t>(w, 0x78) = tab;
    Window_SendEvent5(w, 0, 0x79, 0x4a3a, 0x4cf530, 0, 0);
    for (const int32_t* e = (const int32_t*)0x621f58; e < (const int32_t*)0x622048; e += 3) {
        uint32_t item[8];
        memset(item, 0, sizeof item);
        String_Assign((char**)&item[0], GetString(e[0]));
        String_Assign((char**)&item[1], GetString(e[1]));
        item[3] = (uint32_t)e[2];
        Window_SendEvent5(w, 0, 0x89, 0x4a3a, (uint32_t)(uintptr_t)item, 0, 0);
        ((void(__cdecl*)(void*))0x5a1145)((void*)(uintptr_t)item[0]);
        ((void(__cdecl*)(void*))0x5a1145)((void*)(uintptr_t)item[1]);
    }
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    if (lang == 4 || lang == 2) {
        Window_SendEvent5(w, 0, 0xb3, 0x4a44, 5, 0, 0);
        lang = hook::Global<int32_t>(0x6d3c84);
    }
    if (lang == 1) {
        Window_SendEvent5(w, 0, 0x78, 0x5b0c, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x9c43, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x9c44, 0x12, 0, 0);
        lang = hook::Global<int32_t>(0x6d3c84);
    }
    if (lang == 2) {
        Window_SendEvent5(w, 0, 0x78, 0x9c43, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x9c44, 0x12, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x6f, 0x5b74, (uint32_t)(uintptr_t)GetString(0xf3d), 0, 0);
    ++F<int32_t>(w, 0x7c);
    Window_SendEvent5(w, 0, 0x66, 0x4a3a, (uint32_t)tab, 0, 0);
    --F<int32_t>(w, 0x7c);
    EditorDialog_SetTab(w, 0, tab, city);
}
RT3_REPLACE(0x4cf550, EditorDialog_Init);

// 0x4cf700 (event handler). Editor dialog d: Done (0x4a39) closes it
// (rules page 0xb only once the rule stores); other events go to its page's
// handler (+0x78).
int32_t __fastcall EditorDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (msg[0] == 0xcb && msg[1] == 0x4a39) {
        const int32_t page = F<int32_t>(d, 0x78);
        if (page == 9) {
            Nop4cf900(d);
        } else if (page == 0xb) {
            if (!RulesDialog_StoreRule(d)) return 1;
        }
        msg[0] = 0xc9;
        return 1;
    }
    switch (F<int32_t>(d, 0x78)) {
    case 0: return EditorDialog_OnSettingsEvent(d, 0, msg);
    case 1: return (int32_t)CityEditor_HandleEvent(d, 0, msg);
    case 2: return (int32_t)TerritoryEditor_HandleEvent(d, 0, msg);
    case 3: return (int32_t)NewGameDialog_HandleEvent(d, 0, msg);
    case 4: return EditorDialog_OnCargoEvent(d, 0, msg);
    case 6: return (int32_t)EditorDialog_OnIndustryEvent(d, 0, msg);
    case 7: return EditorDialog_HousesEvent(d, 0, (int32_t*)msg);
    case 8: return EditorDialog_OnAchievementsEvent(d, 0, msg);
    case 9: return (int32_t)EditorDialog_OnRegionTypesEvent(d, 0, msg);
    case 10: return Ui_HandleModeMsg(d, 0, (const int32_t*)msg);
    case 11: return (int32_t)EventEditor_HandleEvent(d, 0, msg);
    case 17: return Ui_HandleCounterMsg(d, 0, (const int32_t*)msg);
    default: return 1;
    }
}
RT3_REPLACE(0x4cf700, EditorDialog_HandleEvent);

// 0x4cf830 (tab in ECX, city in EDX). Runs the scenario editor dialog on
// `tab` (-1: the last one, 0x6cff80) for `city`, taking the chosen city
// 0x6cff78 into 0x6cff74 when it is valid; then updates the edit tool cursor.
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city) {
    if (tab == -1) tab = hook::Global<int32_t>(0x6cff80);
    Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 0);
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0xa0);
    w = w ? PortraitDialog_Construct(w) : nullptr;
    EditorDialog_Init(w, 0, tab, city);
    if (Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, hook::Global<uint32_t>(0x6cff78))) hook::Global<uint32_t>(0x6cff74) = hook::Global<uint32_t>(0x6cff78);
    PauseLock_Push(hook::Global<int32_t*>(0x6cec74));
    Ui_RunModal(hook::Global<uint8_t**>(0x6d401c), 0, w, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::Global<int32_t*>(0x6cec74));
    Windows_Remove(hook::Global<uint8_t*>(0x6d401c), 0, w);
    if (w) {
        SetupWindow_Destroy(w);
        ((void(__cdecl*)(void*))0x53b080)(w);
    }
    if (hook::Global<uint8_t*>(0x6d0818)) EditTool_UpdateCursor(hook::Global<uint8_t*>(0x6d0818));
}
RT3_REPLACE(0x4cf830, EditorDialog_Run);

RT3_NOINLINE void __fastcall Nop4cf900(void*) { __asm nop }
RT3_REPLACE(0x4cf900, Nop4cf900);

// 0x4cf910. The scenario editor's region-type page (nesting counter 0x6cffa0):
// normalizes the twelve templates (world +0xfe7, stride 0x4e1), lists them in
// 0x59d8 as rich rows (event 0x89: name, "used by N: <cities>" (0x1fc, up to eight
// enabled cities of that type) or "unused" (0x1fd, greyed)), selects 0x6cffa4 and
// shows its five trade entries (+0x3f1, 0x30 bytes: kind 0..3 = none / sells /
// buys / converts, amount +4, cargo names +8 / +0x1c) on controls 0x5a0a.. (kind
// list from 0x622048; cargo lists +0x14 / +0x28 with the non-+0x46 cargo types;
// layout event 0xb3 by kind), the converter controls 0x59dc..0x59de when any entry
// converts, and the template's +0x3ed in 0x59de.
RT3_NOINLINE void __fastcall EditorDialog_ShowRegionTypes(uint8_t* w) {
    ++hook::Global<int32_t>(0x6cffa0);
    Templates_Normalize(hook::Global<uint8_t*>(0x6cec78));
    Window_SendEvent5(w, 0, 0x8b, 0x59d8, 0, 0, 0);
    for (int32_t i = 0, off = 0; off < 0x3a8c; ++i, off += 0x4e1) {
        int32_t count = 0;
        char names[0x3f0];
        names[0] = 0;
        for (int32_t k = 0; k < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++k) {
            Collection* cc = hook::Global<Collection*>(0x62bae0);
            uint8_t* c = (uint8_t*)Collection_Get(cc, 0, Collection_IdAt(cc, 0, (uint32_t)k, 0));
            if (F<int32_t>(c, 0x242) <= 0 || !F<int32_t>(c, 0x2f6) || F<int32_t>(c, 0x2f2) != i) continue;
            if (count < 8) {
                if (count > 0) memcpy(names + strlen(names), (const void*)0x5c9f38, 3);
                strcat(names, (const char*)c + 0x356);
            }
            ++count;
        }
        uint32_t item[8] = {};
        String_Assign((char**)&item[0], (const char*)hook::Global<uint8_t*>(0x6cec78) + 0xfe7 + off);
        char num[0x60], text[0x400];
        if (count > 0) FormatText(text, GetString(0x1fc), Num_Format(num, 1, (double)count, 10, 1, 0, 0), names, 0, 0, 0, 0, 0, 0);
        else FormatText(text, GetString(0x1fd), 0, 0, 0, 0, 0, 0, 0, 0);
        String_Assign((char**)&item[1], text);
        if (!count) item[3] = 0xffafafaf;
        Window_SendEvent5(w, 0, 0x89, 0x59d8, (uint32_t)(uintptr_t)item, 0, 0);
        ((void(__cdecl*)(void*))0x5a1145)((void*)(uintptr_t)item[0]);
        ((void(__cdecl*)(void*))0x5a1145)((void*)(uintptr_t)item[1]);
    }
    Window_SendEvent5(w, 0, 0x66, 0x59d8, hook::Global<uint32_t>(0x6cffa4), 0, 0);
    uint8_t* tmpl = hook::Global<uint8_t*>(0x6cec78) + 0xfe7 + hook::Global<int32_t>(0x6cffa4) * 0x4e1;
    bool converts = false;
    uint8_t* e = tmpl + 0x3f1;
    for (int32_t j = 0, lay = 0; j < 5; ++j, e += 0x30, lay += 0xbe) {
        uint32_t id = 0x5a0a + j;
        Window_SendEvent5(w, 0, 0x8b, id, 0, 0, 0);
        for (const int32_t* s = (const int32_t*)0x622048; s < (const int32_t*)0x622058; ++s)
            Window_SendEvent5(w, 0, 0x88, id, (uint32_t)(uintptr_t)GetString(*s), 0, 0);
        int32_t kind = F<int32_t>(e, 0);
        Window_SendEvent5(w, 0, 0x66, id, (uint32_t)kind, 0, 0);
        uint32_t sells = kind == 3 || kind == 1 ? 0x65 : 0x87, buys = kind == 3 || kind == 2 ? 0x65 : 0x87;
        uint32_t simple = kind == 1 || kind == 2 ? 0x65 : 0x87;
        Window_SendEvent5(w, 0, sells, id + 0xa, 3, 0, 0);
        Window_SendEvent5(w, 0, sells, id + 0x28, 3, 0, 0);
        Window_SendEvent5(w, 0, buys, id + 0x1e, 3, 0, 0);
        Window_SendEvent5(w, 0, buys, id + 0x14, 3, 0, 0);
        Window_SendEvent5(w, 0, simple, id + 0x3c, 3, 0, 0);
        Window_SendEvent5(w, 0, simple, id + 0x46, 3, 0, 0);
        Window_SendEvent5(w, 0, simple, id + 0x32, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, id + 0x32, (uint32_t)(uintptr_t)GetString(kind == 1 ? 0x1fe : 0x1ff), 0, 0);
        if (kind == 3) converts = true;
        if (!F<int32_t>(e, 0)) continue;
        char buf[0x1f0];
        Num_Format(buf, 1, (double)Ftol(F<float>(e, 4)), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, id + 0x46, (uint32_t)(uintptr_t)buf, 0, 0);
        Window_SendEvent5(w, 0, 0x8b, id + 0x14, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x8b, id + 0x28, 0, 0, 0);
        int32_t sel1 = 0, sel2 = 0, row = 0;
        for (int32_t k = 0; k < Collection_Count(hook::Global<Collection*>(0x62ba8c)); ++k) {
            Collection* ct = hook::Global<Collection*>(0x62ba8c);
            uint8_t* c = (uint8_t*)Collection_Get(ct, 0, Collection_IdAt(ct, 0, (uint32_t)k, 0));
            if (c[0x46]) continue;
            const char* raw = (const char*)c + 4;
            const char* loc = LocalizeName(raw);
            Window_SendEvent5(w, 0, 0x88, id + 0x14, (uint32_t)(uintptr_t)loc, Collection_IdAt(hook::Global<Collection*>(0x62ba8c), 0, (uint32_t)k, 0), 0);
            loc = LocalizeName(raw);
            Window_SendEvent5(w, 0, 0x88, id + 0x28, (uint32_t)(uintptr_t)loc, Collection_IdAt(hook::Global<Collection*>(0x62ba8c), 0, (uint32_t)k, 0), 0);
            if (!Stricmp(raw, (const char*)e + 8)) sel1 = row;
            if (!Stricmp(raw, (const char*)e + 0x1c)) sel2 = row;
            ++row;
        }
        Window_SendEvent5(w, 0, 0x66, id + 0x14, (uint32_t)sel1, 0, 0);
        Window_SendEvent5(w, 0, 0x66, id + 0x28, (uint32_t)sel2, 0, 0);
        uint32_t a = (uint32_t)(lay + 0xbe - (kind == 3 ? 0x28 : 0)), b = (uint32_t)(lay + 0xbe + (kind == 3 ? 0x28 : 0));
        Window_SendEvent5(w, 0, 0xb3, id + 0xa, a, 0, 0);
        Window_SendEvent5(w, 0, 0xb3, id + 0x28, a, 0, 0);
        Window_SendEvent5(w, 0, 0xb3, id + 0x1e, b, 0, 0);
        Window_SendEvent5(w, 0, 0xb3, id + 0x14, b, 0, 0);
    }
    uint32_t t = converts ? 0x65 : 0x87;
    Window_SendEvent5(w, 0, t, 0x59dc, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0x59dd, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0x59de, 3, 0, 0);
    char buf[0x1f0];
    Num_Format(buf, 1, (double)Ftol(F<float>(tmpl, 0x3ed)), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x59de, (uint32_t)(uintptr_t)buf, 0, 0);
    --hook::Global<int32_t>(0x6cffa0);
}
RT3_REPLACE(0x4cf910, EditorDialog_ShowRegionTypes);

// 0x4d0040. Region-type page of the scenario editor dialog w: event handler
// for template 0x6cffa4 (world +0xfe7, stride 0x4e1); ignored (1) while the
// page is being filled (0x6cffa0). Clicks (0xcb): rename (0x59d9, text box 9,
// string 0x1fb, 60 characters), previous / next of the twelve (0x59da /
// 0x59db). Changes (0xca): list pick (0x59d8), the value +0x3ed (0x59de,
// warning 0x1f9 above 8) and for trade entry j (+0x3f1 + j * 0x30) its kind
// (0x5a0a + j), cargo names +8 / +0x1c (0x5a1e + j / 0x5a32 + j, cargo
// 0x62ba8c) and amount +4 (0x5a50 + j, warning 0x1fa above 6). Every handled
// event reshows the page.
RT3_NOINLINE uint32_t __fastcall EditorDialog_OnRegionTypesEvent(uint8_t* w, int, uint32_t* ev) {
    if (hook::Global<int32_t>(0x6cffa0) > 0) return 1;
    auto tmpl = []() { return hook::Global<uint8_t*>(0x6cec78) + 0xfe7 + hook::Global<int32_t>(0x6cffa4) * 0x4e1; };
    uint32_t id = ev[1];
    if (ev[0] == 0xcb) {
        if (id == 0x59d9) {
            Dialog_RunMessage(GetString(0x1fb), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)tmpl(), 0x3c, 0, nullptr, nullptr, 0, 0,
                              nullptr, 0);
        } else {
            if ((int32_t)id < 0x59d9 || (int32_t)id > 0x59db) return 1;
            int32_t dir = id != 0x59da ? 1 : -1;
            hook::Global<int32_t>(0x6cffa4) = (dir + hook::Global<int32_t>(0x6cffa4) + 12) % 12;
        }
        EditorDialog_ShowRegionTypes(w);
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    auto atoi_ = [&]() { return ((int(__cdecl*)(const char*))0x5a1ea5)(F<const char*>((uint8_t*)(uintptr_t)ev[11], 0xba)); };
    if (id == 0x59d8) {
        hook::Global<uint32_t>(0x6cffa4) = ev[2];
    } else if (id == 0x59de) {
        int32_t v = atoi_();
        if (v > 8) Message_Show(GetString(0x1f9), 1, 0);
        F<float>(tmpl(), 0x3ed) = (float)v;
    } else if (id >= 0x5a0a && id <= 0x5a0e) {
        F<uint32_t>(tmpl(), 0x3f1 + (id - 0x5a0a) * 0x30) = ev[2];
    } else if (id >= 0x5a1e && id <= 0x5a22) {
        uint8_t* cargo = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, ev[3]);
        strcpy((char*)tmpl() + 0x3f9 + (id - 0x5a1e) * 0x30, (const char*)cargo + 4);
    } else if (id >= 0x5a32 && id <= 0x5a36) {
        uint8_t* cargo = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, ev[3]);
        strcpy((char*)tmpl() + 0x40d + (id - 0x5a32) * 0x30, (const char*)cargo + 4);
    } else if (id >= 0x5a50 && id <= 0x5a54) {
        int32_t v = atoi_();
        if (v > 6) Message_Show(GetString(0x1fa), 1, 0);
        F<float>(tmpl(), 0x3f5 + (id - 0x5a50) * 0x30) = (float)v;
    } else {
        return 1;
    }
    EditorDialog_ShowRegionTypes(w);
    return 0;
}
RT3_REPLACE(0x4d0040, EditorDialog_OnRegionTypesEvent);

// 0x4d03d0. Ratio +0x76 / +0x7a, with the divisor floored at 0.001.
RT3_NOINLINE float __fastcall Obj_Ratio76(uint8_t* o) {
    if ((double)F<float>(o, 0x7a) < *(const double*)0x5c8678) return (float)((double)F<float>(o, 0x76) / 0.001);
    return (float)((double)F<float>(o, 0x76) / (double)F<float>(o, 0x7a));
}
RT3_REPLACE(0x4d03d0, Obj_Ratio76);

// 0x4d0420. The engine statistics summary (computed once, 0x6cffb0): the map's
// open cells (0x6cffb4), the counts of current (0x6cffc8) and obsolete
// (0x6cffac) engine types, their production totals (0x6cffc0 / 0x6cffcc), the
// average speed of current types (0x6cffd4) and the count-weighted one of
// obsolete types (0x6cffd0); then ten labelled lines (0x424..0x42d, densities
// per 1000 cells) under title 0x423, and footer 0x42e.
RT3_NOINLINE void __fastcall EngineStats_Draw(void*, int, float x, float y, float w, float h, int32_t) {
    if (!hook::Global<int32_t>(0x6cffb0)) {
        hook::Global<int32_t>(0x6cffb0) = 1;
        hook::Global<int32_t>(0x6cffb4) = 0;
        for (int32_t cy = 0; cy < F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159); ++cy)
            for (int32_t cx = 0; cx < F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155); ++cx)
                if (!Map_CellLow3(hook::Global<uint8_t*>(0x62c120), 0, cx, cy)) ++hook::Global<int32_t>(0x6cffb4);
        hook::Global<int32_t>(0x6cffc8) = 0;
        hook::Global<int32_t>(0x6cffac) = 0;
        hook::Global<float>(0x6cffcc) = 0.0f;
        hook::Global<float>(0x6cffc0) = 0.0f;
        float sumCur = 0.0f, weighted = 0.0f, weight = 0.0f;
        Collection* locos = hook::Global<Collection*>(0x62bae0);
        for (int32_t j = 0; j < Collection_Count(locos); ++j) {
            uint8_t* r = (uint8_t*)Collection_Get(locos, 0, Collection_IdAt(locos, 0, j, 0));
            hook::Global<float>(0x6cffc0) = (float)((double)Obj_ProductionRate(r, 0, 2) + hook::Global<float>(0x6cffc0));
            hook::Global<float>(0x6cffcc) = (float)((double)Obj_ProductionRate(r, 0, 2) + hook::Global<float>(0x6cffcc));
            hook::Global<float>(0x6cffcc) = (float)((double)Obj_ProductionRate(r, 0, 3) + hook::Global<float>(0x6cffcc));
            hook::Global<float>(0x6cffcc) = (float)((double)Obj_ProductionRate(r, 0, 0) + hook::Global<float>(0x6cffcc));
            if (F<int32_t>(r, 0x23e)) {
                float f = (float)(F<int32_t>(r, 0x242) << 4);
                ++hook::Global<int32_t>(0x6cffac);
                weight = (float)((double)f + weight);
                weighted = (float)((double)f * F<float>(r, 0x25a) + weighted);
            } else {
                sumCur = (float)((double)sumCur + F<float>(r, 0x25a));
                ++hook::Global<int32_t>(0x6cffc8);
            }
        }
        int32_t nCur = hook::Global<int32_t>(0x6cffc8) < 1 ? 1 : hook::Global<int32_t>(0x6cffc8);
        hook::Global<float>(0x6cffd4) = (float)((double)sumCur / (float)nCur);
        float d = Fl(0x5c85b4) > weight ? 1.0f : weight;
        hook::Global<float>(0x6cffd0) = (float)((double)weighted / d);
    }
    uint8_t* font = hook::Global<uint8_t*>(0x6cff88);
    float rowY = (float)((double)y + D(0x5d01d8));
    float lx = (float)((double)x + D(0x5d0be0));
    float lw = (float)((double)w - D(0x5d0bd8));
    Font_DrawTextBox(font, 0, GetString(0x423), lx, rowY, lw, BitsF(0x3cfc9630), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d0bd0));
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    auto cells = [] { return hook::Global<int32_t>(0x6cffb4) < 1 ? 1 : hook::Global<int32_t>(0x6cffb4); };
    char buf[0x200];
    for (int32_t i = 0; i < 10; ++i) {
        uint32_t color = (i == 2 || i == 8) ? 0xffffff00 : 0xffffffff;
        Font_DrawTextBox(font, 0, GetString(0x424 + i), lx, rowY, lw, BitsF(0x3cfc9630), color, 2, 0, 0, 1.0f);
        float c0 = hook::Global<float>(0x6cffc0), cc = hook::Global<float>(0x6cffcc);
        switch (i) {
        case 0: Num_Format(buf, 1, (double)hook::Global<int32_t>(0x6cffb4), 10, 1, 0, 0); break;
        case 1: Num_Format(buf, 1, (double)hook::Global<int32_t>(0x6cffc8), 10, 1, 0, 0); break;
        case 2: {
            int32_t m = cells();
            sprintf_(buf, (const char*)0x5ca2ac, (double)hook::Global<int32_t>(0x6cffc8) * D(0x5c8d18) / (double)m);
            break;
        }
        case 3: sprintf_(buf, (const char*)0x5d0bc4, (double)hook::Global<float>(0x6cffd4)); break;
        case 4: {
            int32_t m = cells();
            sprintf_(buf, (const char*)0x5d0bbc, (double)hook::Global<int32_t>(0x6cffc8) * D(0x5c8d18) * hook::Global<float>(0x6cffd4) / (double)m);
            break;
        }
        case 5: sprintf_(buf, (const char*)0x5d0bc4, (double)hook::Global<float>(0x6cffd0)); break;
        case 6: Num_Format(buf, 1, (double)Ftol((double)c0), 10, 1, 0, 0); break;
        case 7: Num_Format(buf, 1, (double)Ftol((double)cc - c0), 10, 1, 0, 0); break;
        case 8: {
            int32_t m = cells();
            sprintf_(buf, (const char*)0x5d0104, (double)c0 * Fl(0x5c88c0) / (float)m);
            break;
        }
        case 9: {
            int32_t m = cells();
            sprintf_(buf, (const char*)0x5d0104, ((double)cc - c0) * Fl(0x5c88c0) / (float)m);
            break;
        }
        }
        float off = (float)(hook::Global<int32_t>(0x6d3c84) == 1 ? BitsD(0x3f9999999999999aull) : 0.0);
        float vx = (float)((double)off + lx);
        Font_DrawTextBox(font, 0, buf, vx, rowY, lw, BitsF(0x3cfc9630), color, 3, 0, 0, 1.0f);
        rowY = (float)((double)rowY + Fl(0x5d0bb8));
        if (i == 2 || i == 5 || i == 7) rowY = (float)((double)rowY + D(0x5d0bb0));
    }
    float fh = (float)((double)y + h - rowY);
    float fw = (float)((double)lw + D(0x5c9688));
    float fx = (float)((double)lx - D(0x5d0ba8));
    Font_DrawTextBox(font, 0, GetString(0x42e), fx, rowY, fw, fh, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4d0420, EngineStats_Draw);

// 0x4d0a90 (draw callback of the editor's check page 0x4a41). Lists the
// scenario's problems at (x, y) in width w (font 0x6cff90): cities and
// regions with no industry type available at the start year (0x42f),
// fewer AI personalities enabled than open slots (0x431), a missing
// description (0x432) or briefing (0x433), more than 80% of the engines
// enabled (0x434), the first year (1829..2028, from the start) with at most
// one engine (none before 1856; 0x435) and, from 1926, none electric
// (0x436) or none other (0x437), industries of no city (0x438 header, ports
// and warehouses aside), demanded cargos no industry makes (0x439), and,
// computed once (0x6d07ac) into 0x6cffd8 with the build seed 0xa7, the
// planned industries a city cannot place (0x43b / 0x43a), shown in at most
// the remaining height h. Without problems it says so (0x43d). QUIRK: the
// last loop reads each city's map cell and drops the result.
void __fastcall ScenarioCheck_Draw(void*, int, float x, float y0, float w0, float h, int32_t) {
    float y = (float)((double)y0 + *(const double*)0x5d01d8);
    int32_t start = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ca);
    float w = (float)((double)w0 - *(const double*)0x5d0110);
    int32_t any = 0;
    uint8_t* font = nullptr;
    auto draw = [&](const char* text, uint32_t hb) {
        font = hook::Global<uint8_t*>(0x6cff90);
        Font_DrawTextBox(font, 0, text, x, y, w, *(const float*)&hb, 0xffffffff, 0, 0, 0, 1.0f);
    };
    char msg[0x200], num1[0x20], num2[0x20], list[0x200];
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
        cities = hook::Global<Collection*>(0x62bae0);
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x23e) && F<int32_t>(c, 0x242) <= 0) continue;
        if (Obj_CountTypesAvailable(c, 0, start, 2)) continue;
        any = 1;
        const char* kind = GetString(F<int32_t>(c, 0x23e) ? 0x430 : 0x409);
        const char* yr = Num_Format(num1, 8, (double)start, 0xa, 1, 0, 0);
        const char* fmt = GetString(0x42f);
        FormatText(msg, fmt, kind, (const char*)c + 0x356, yr, nullptr, nullptr, nullptr, nullptr, nullptr);
        draw(msg, 0x3ca3d70a);
        y = y + *(const float*)0x5cb5c4;
    }
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t open = 0;
    for (int32_t k = 0; k < F<int32_t>(world, 0x6a6c); ++k)
        if (!world[0x69d8 + k * 9]) ++open;
    int32_t ais = 0;
    for (int32_t k = 0; k < 0x29; ++k)
        if (world[0x6987 + k]) ++ais;
    if (open > ais) {
        const char* sa = Num_Format(num1, 1, (double)ais, 0xa, 1, 0, 0);
        const char* so = Num_Format(num2, 1, (double)open, 0xa, 1, 0, 0);
        const char* fmt = GetString(0x431);
        FormatText(msg, fmt, so, sa, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        any = 1;
        draw(msg, 0x3da3d70a);
        y = y + *(const float*)0x5d0c10;
        world = hook::Global<uint8_t*>(0x6cec78);
    }
    if (strlen((const char*)world + 0x4f30) < 2) {
        draw(GetString(0x432), 0x3da3d70a);
        y = (float)((double)y + *(const double*)0x5d0c08);
        world = hook::Global<uint8_t*>(0x6cec78);
    }
    if (strlen((const char*)world + 0x672e) < 2) {
        draw(GetString(0x433), 0x3da3d70a);
        y = (float)((double)y + *(const double*)0x5d0c08);
    }
    int32_t total = Collection_Count(hook::Global<Collection*>(0x6ada84));
    static int32_t other[200], two[200], all[200];
    memset(other, 0, sizeof other);
    memset(two, 0, sizeof two);
    memset(all, 0, sizeof all);
    int32_t used = 0;
    for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++j) {
        Collection* engines = hook::Global<Collection*>(0x6ada84);
        uint8_t* e = (uint8_t*)Collection_Get(engines, 0, Collection_IdAt(engines, 0, (uint32_t)j, 0));
        if (!World_LookupNamed2(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)Obj_Ref72Plus45(e))) continue;
        uint8_t* t = F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72)) : nullptr;
        int32_t first = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ca);
        ++used;
        for (int32_t k = 0; k < 200; ++k) {
            int32_t yr = 0x725 + k;
            if (yr < first || F<int32_t>(t, 0x105) > yr || F<int32_t>(t, 0x109) < yr) continue;
            ++all[k];
            if (F<int32_t>(e, 0x10) == 2) ++two[k];
            else ++other[k];
        }
    }
    if ((double)total * *(const double*)0x5c8640 < (double)used) {
        const char* st = Num_Format(num1, 1, (double)total, 0xa, 1, 0, 0);
        const char* su = Num_Format(num2, 1, (double)used, 0xa, 1, 0, 0);
        const char* fmt = GetString(0x434);
        FormatText(msg, fmt, su, st, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        draw(msg, 0x3d75c28f);
        y = y + *(const float*)0x5d0c00;
        any = 1;
    }
    bool fewAll = false, fewTwo = false, fewOther = false;
    for (int32_t k = 0; k < 200; ++k) {
        int32_t yr = 0x725 + k;
        if (yr < F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ca)) continue;
        if (!fewAll && all[k] <= (yr > 0x73f ? 1 : 0)) {
            const char* sn = Num_Format(num1, 1, (double)all[k], 0xa, 1, 0, 0);
            const char* sy = Num_Format(num2, 8, (double)yr, 0xa, 1, 0, 0);
            const char* fmt = GetString(0x435);
            FormatText(msg, fmt, sy, sn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            draw(msg, 0x3d75c28f);
            y = y + *(const float*)0x5d0c00;
            fewAll = true, any = 1;
        }
        if (!fewTwo && two[k] < (yr > 0x785 ? 1 : 0)) {
            const char* sy = Num_Format(num1, 8, (double)yr, 0xa, 1, 0, 0);
            const char* fmt = GetString(0x436);
            FormatText(msg, fmt, sy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            draw(msg, 0x3d75c28f);
            y = y + *(const float*)0x5d0c00;
            fewTwo = true, any = 1;
        }
        if (!fewOther && other[k] < (yr > 0x785 ? 1 : 0)) {
            const char* sy = Num_Format(num1, 8, (double)yr, 0xa, 1, 0, 0);
            const char* fmt = GetString(0x437);
            FormatText(msg, fmt, sy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            draw(msg, 0x3d75c28f);
            y = y + *(const float*)0x5d0c00;
            fewOther = true, any = 1;
        }
    }
    bool header = false;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b268)); ++i) {
        Collection* types = hook::Global<Collection*>(0x62b268);
        uint8_t* b = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, (uint32_t)i, 0));
        const char* name = (const char*)b + 4;
        auto strnicmp = (int(__cdecl*)(const char*, const char*, size_t))0x5a5f26;
        if (!World_LookupNamed1(hook::Global<uint8_t*>(0x6cec78), 0, name) || !strnicmp(name, (const char*)0x5c8f94, 4) || !strnicmp(name, (const char*)0x5c8e4c, 9))
            continue;
        uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x62b268), 0, (uint32_t)i, 0);
        if (Table_CountListing(hook::Global<Collection*>(0x62bae0), 0, id)) continue;
        if (!header) {
            draw(GetString(0x438), 0x3d4ccccc);
            y = (float)((double)y + *(const double*)0x5d0bf8);
            header = true;
        }
        const char* fmt = LocalizeName((const char*)b + 0x4f);
        FormatText(msg, fmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        draw(msg, 0x3d75c28f);
        y = y + *(const float*)0x5cb5c4;
        any = 1;
    }
    if (header) y = y + *(const float*)0x5cb5c4;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62ba8c)); ++i) {
        Collection* cargos = hook::Global<Collection*>(0x62ba8c);
        uint8_t* cg = (uint8_t*)Collection_Get(cargos, 0, Collection_IdAt(cargos, 0, (uint32_t)i, 0));
        if (!F<int32_t>(cg, 0x56)) continue;
        int32_t made = 0;
        for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x62b268)); ++j) {
            Collection* types = hook::Global<Collection*>(0x62b268);
            uint8_t* b = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, (uint32_t)j, 0));
            if (!World_LookupNamed1(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)b + 4) && b[0x32]) continue;
            int32_t n = F<int32_t>(b, 0x7a4);
            if (n <= 0) continue;
            const uint16_t* out = F<const uint16_t*>(b, 0x79c);
            for (; n; --n, ++out)
                if (*out == F<uint32_t>(cg, 0x3e)) made = 1;
        }
        if (cg[0x47] || cg[0x48] || cg[0x49] || made) continue;
        const char* nm = LocalizeName((const char*)cg + 4);
        const char* fmt = GetString(0x439);
        FormatText(msg, fmt, nm, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        draw(msg, 0x3d75c28f);
        y = y + *(const float*)0x5d0c00;
        any = 1;
    }
    char* report = (char*)0x6cffd8;
    if (!hook::Global<int32_t>(0x6d07ac)) {
        hook::Global<int32_t>(0x6d07ac) = 1;
        report[0] = 0;
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
            Collection* cs = hook::Global<Collection*>(0x62bae0);
            uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)i, 0));
            if (F<int32_t>(c, 0x242) <= 1) continue;
            for (int32_t k = 0; k < Obj_Count37f(c);) {
                ++k;
                char name[0x60];
                float amount;
                Obj_Entry37f(c, 0, k, name, (int32_t*)&amount);
                if (amount <= *(const float*)0x5c8598) continue;
                uint8_t* type = (uint8_t*)Table_GetByName(hook::Global<Collection*>(0x62b268), 0, name);
                if (!type) continue;
                int32_t seed = GetSeed();
                SetSeed(0xa7);
                int32_t ok = City_BuildIndustry(c, 0, name, 0, 1, nullptr);
                SetSeed(seed);
                if (ok) continue;
                const char* kind = GetString(F<int32_t>(c, 0x23e) ? 0x430 : 0x409);
                const char* tn = LocalizeName((const char*)type + 0x4f);
                const char* sa = Num_Format(num1, 5, amount, 0xa, 1, 0, 0);
                const char* fmt = GetString(0x43a);
                FormatText(list, fmt, kind, (const char*)c + 0x356, sa, tn, nullptr, nullptr, nullptr, nullptr);
                size_t len = strlen(report);
                if ((int32_t)(0x7cb - len) <= 0x64) continue;
                if ((int32_t)len > 1) {
                    memcpy(report + len, (const void*)0x5c9ce8, 2);
                    ++len;
                } else {
                    strcpy(report, GetString(0x43b));
                    len = strlen(report);
                }
                StrCopyN(report + len, list, 0x7cb - len);
            }
        }
    }
    if (report[0]) {
        y = (float)((double)y + *(const double*)0x5d0bf0);
        any = 1;
        float room = (float)((double)h - ((double)y - y0));
        float th = (float)Font_TextHeight(hook::Global<uint8_t*>(0x6cff90), 0, report, w, nullptr);
        if (room < th) th = room;
        float bh = (float)((double)th + *(const double*)0x5caae0);
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, report, x, y, w, bh, 0xffffffff, 0, 0, 0, 1.0f);
        y = bh + y;
        y = (float)((double)y + *(const double*)0x5d0be8);
    }
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
        Collection* cs = hook::Global<Collection*>(0x62bae0);
        uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x23e)) continue;
        Obj_CellX(c);
        Obj_CellY(c);
        int32_t cy = (int32_t)Obj_CellY(c);
        int32_t cx = (int32_t)Obj_CellX(c);
        uint8_t* map = hook::Global<uint8_t*>(0x62c120);
        int32_t cell = cy / 4 * F<int32_t>(map, 0x214d) + cx / 4;
        Collection_Get(hook::Global<Collection*>(0x62bae0), 0, F<uint8_t*>(map, 0x212d)[cell * 4]);
    }
    if (!any) draw(GetString(0x43d), 0x3d23d70a);
    (void)font;
}
RT3_REPLACE(0x4d0a90, ScenarioCheck_Draw);

// 0x4d1b80 (draw callback). Map statistics: counted once (0x6cffc4), the
// land cells (0x6cffa8, Map_CellLow3 clear) and their terrain total
// (0x6cffb8); shows the total with its averages per map cell and per land
// cell (string 0x43e, averages as 0x5d0c14).
void __fastcall MapStats_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    if (!hook::Global<int32_t>(0x6cffc4)) {
        hook::Global<int32_t>(0x6cffc4) = 1;
        hook::Global<int32_t>(0x6cffb8) = 0;
        hook::Global<int32_t>(0x6cffa8) = 0;
        for (int32_t j = 0; j < F<int32_t>(m, 0x2159); ++j) {
            for (int32_t i = 0; i < F<int32_t>(m, 0x2155); ++i) {
                if (!Map_CellLow3(m, 0, i, j)) {
                    ++hook::Global<int32_t>(0x6cffa8);
                    hook::Global<int32_t>(0x6cffb8) += (int32_t)Terrain_CellCount(hook::Global<uint8_t*>(0x62c120), 0, i, j);
                }
                m = hook::Global<uint8_t*>(0x62c120);
            }
            m = hook::Global<uint8_t*>(0x62c120);
        }
    }
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    char perCell[0x40], perLand[0x40], num[0xc8], text[0x5dc];
    const float total = (float)hook::Global<int32_t>(0x6cffb8);
    const float cells = (float)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155));
    sprintf_(perCell, (const char*)0x5d0c14, (double)total / cells);
    const float total2 = (float)hook::Global<int32_t>(0x6cffb8);
    const float land = (float)hook::Global<int32_t>(0x6cffa8);
    sprintf_(perLand, (const char*)0x5d0c14, (double)total2 / land);
    Num_Format(num, 1, (double)hook::Global<int32_t>(0x6cffb8), 10, 1, 0, 0);
    FormatText(text, GetString(0x43e), num, perCell, perLand, 0, 0, 0, 0, 0);
    const float tw = (float)((double)w - D(0x5c8758));
    const float tx = (float)((double)x + D(0x5d08a0));
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff88), 0, text, tx, y, tw, h, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4d1b80, MapStats_DrawCb);

// 0x4d1d60. The locomotive statistics table: the available engine types of
// 0x62bae0 (+0x242) sorted by name (+0x356), 35 per page (page 0x6cff7c,
// wrapped): eleven columns (headers 3, 0x43f..0x448: name, number in service,
// costs, speed / power / reliability figures), rows of obsolete types (+0x23e)
// tinted; a column block wraps to the right below 0x5d0c28, with a rule
// every 5 rows.
RT3_NOINLINE void __fastcall LocoStats_DrawTable(void*, int, float x, float y, float, float, int32_t) {
    Collection* locos = hook::Global<Collection*>(0x62bae0);
    static uint8_t used[1000];
    uint32_t order[1000];
    char best[0x190];
    memset(used, 0, sizeof used);
    int32_t n = 0;
    for (int32_t k = 0; Collection_Count(locos) > 0 && k < Collection_Count(locos); ++k) {
        int32_t pick = -1;
        for (int32_t j = 0; j < Collection_Count(locos); ++j) {
            if (used[j]) continue;
            uint8_t* r = (uint8_t*)Collection_Get(locos, 0, Collection_IdAt(locos, 0, j, 0));
            if (!F<int32_t>(r, 0x242)) continue;
            if (pick != -1 && strcmp((const char*)(r + 0x356), best) >= 0) continue;
            pick = j;
            strcpy(best, (const char*)(r + 0x356));
        }
        if (pick == -1) break;
        ++n;
        used[pick] = 1;
        order[k] = Collection_IdAt(locos, 0, pick, 0);
    }
    float rowY = (float)((double)y - D(0x5ca2e0));
    float colX = (float)((double)x + D(0x5d0b38));
    int32_t pages = (n - 1) / 35 + 1;
    int32_t page = (hook::Global<int32_t>(0x6cff7c) + pages) % pages;
    hook::Global<int32_t>(0x6cff7c) = page;
    static const int32_t heads[11] = { 3, 0x43f, 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447, 0x448 };
    static const uint32_t widths[11] = { 0x43340000, 0x42c5c28f, 0x42e2999a, 0x42e2999a, 0x42de7ae1, 0x42e6b852, 0x42ce0000, 0x42c5c28f, 0x42c5c28f,
                                         0x42b547ae, 0x42bd851f };
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    char buf[0x200];
    int32_t inPage = 0;
    for (int32_t i = -1; i < n; ++i) {
        uint8_t* r = nullptr;
        uint32_t color = 0xffffffff;
        if (i >= 0) {
            r = (uint8_t*)Collection_Get(locos, 0, order[i]);
            if (F<int32_t>(r, 0x23e)) color = 0xffffffcf;
        }
        bool visible = i == -1 || i / 35 == page;
        float cx = colX, total = 0.0f;
        for (int32_t k = 0; k < 11; ++k) {
            float wk = BitsF(widths[k]);
            if (i == -1)
                FormatText(buf, GetString(heads[k]), 0, 0, 0, 0, 0, 0, 0, 0);
            else
                switch (k) {
                case 0: FormatText(buf, (const char*)(r + 0x356), 0, 0, 0, 0, 0, 0, 0, 0); break;
                case 1: Num_Format(buf, 1, (double)Obj_CountBuildingsOfKind(r, 0, 2), 10, 1, 0, 0); break;
                case 2: Num_FormatSimple2(buf, 0xb, (double)F<float>(r, 0x2ca), 0xb); break;
                case 3: Num_FormatSimple2(buf, 0xb, (double)F<float>(r, 0x2ce), 0xb); break;
                case 4:
                    Num_FormatSimple2(buf, 0xb,
                                      ((double)F<float>(r, 0x2de) + F<float>(r, 0x2da)) * D(0x5c8628) +
                                          ((double)F<float>(r, 0x2d6) + F<float>(r, 0x2d2)) * D(0x5d0298),
                                      0xb);
                    break;
                case 5: sprintf_(buf, (const char*)0x5d0c14, (double)F<float>(r, 0x2e2)); break;
                case 6: sprintf_(buf, (const char*)0x5d0b30, (double)F<float>(r, 0x2ee) * Fl(0x5c8c28)); break;
                case 7: sprintf_(buf, (const char*)0x5d0c48, (double)F<float>(r, 0x25a)); break;
                case 8: sprintf_(buf, (const char*)0x5d0c48, (double)F<float>(r, 0x25e)); break;
                case 9: Num_Format(buf, 1, (double)F<int32_t>(r, 0x286), 10, 1, 0, 0); break;
                case 10: Num_Format(buf, 1, (double)F<int32_t>(r, 0x296), 10, 1, 0, 0); break;
                }
            if (visible) {
                double hmul = i == -1 ? BitsD(0x400f333333333333ull) : 1.0;
                double wAdd = i == -1 ? 0.0 : 12.0;
                double xAdd = i == -1 ? 12.0 : 0.0;
                float h = (float)(hmul * D(0x5d0c40));
                float w = (float)(((double)wk + wAdd) / D(0x5ca158));
                float tx = (float)(xAdd / D(0x5ca2c8) + cx);
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, buf, tx, rowY, w, h, color, (k != 0) + 2, 4, 0, 1.0f);
                cx = (float)(((double)wk + D(0x5c8978)) / D(0x5ca158) + cx);
            }
            total = (float)(((double)wk + D(0x5c8978)) / D(0x5ca158) + total);
        }
        if (!visible) continue;
        rowY = (float)((double)rowY + Fl(0x5d0c38));
        if (i == -1) rowY = (float)((double)rowY + D(0x5d0c30));
        else ++inPage;
        if (rowY >= D(0x5d0c28)) {
            rowY = (float)((double)y + D(0x5d01d8));
            colX = (float)((double)total + colX + D(0x5d0b00));
        } else if (inPage % 5 == 0 || i == -1) {
            float ly = (float)((double)rowY + D(0x5caae0));
            Ui_Line(0xffaaaaaa, 0, colX, ly, (float)((double)total + colX), ly, 1.0f);
            rowY = (float)((double)rowY + D(0x5d0c20));
        }
    }
}
RT3_REPLACE(0x4d1d60, LocoStats_DrawTable);

}  // namespace rt3
