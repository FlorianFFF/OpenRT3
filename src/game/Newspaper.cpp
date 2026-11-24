// Newspaper.cpp - RT3.exe 0x4f2000-0x4f6000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/containers.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx);
int32_t __fastcall Map_PointCount(void* m);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);
void* __fastcall Engine_Get1C(void* e);
int32_t __fastcall Cities_AssignBlocks(Collection* cities);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
void* __fastcall Obj_Ref3cc(void* o);
void __fastcall BuildingType_Model2D(uint8_t* o, int, char* out);
int32_t __fastcall Table_CountField3f(Collection* t);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
int32_t __fastcall Widget_TextLines(uint8_t* w);
int32_t __fastcall Obj_YearReached(void* o);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
double __fastcall Cargo_DemandWeight(void* c);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
int32_t CountFreeCompanySlots();
int32_t __fastcall NthFreeCompanySlot(int32_t n);
int32_t __fastcall Collection_Count(Collection* c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Table_RefreshAll(Collection* t);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);
void __fastcall Obj_DecA0(void* o);
void __fastcall Tracks_UpdateColor(Collection* c);
void __fastcall Table_ForAllCityChains(Collection* t);
void __fastcall Scene_PopBusy(uint8_t* s);
uint8_t* __fastcall Newspaper_Construct(uint8_t* w);
void __fastcall Newspaper_Fill(uint8_t* w, int, uint8_t* rec);
void __fastcall CityEditor_Show(uint8_t* w);
void __fastcall Window_HighlightTabs7And5(uint8_t* w);
void __fastcall Scene_PushBusy(uint8_t* s);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Terrain_RebuildWaterMask(uint8_t* t);
void __fastcall PickerWindow_Build(uint8_t* w);
void __fastcall Tracks_ColorSlope(Collection* c);
void __fastcall Camera_BeginA0(uint8_t* o);
void __fastcall Tracks_ColorWear(Collection* c);
void __fastcall Buildings_ColorProfit(Collection* c);
void __fastcall Trains_UpdateRatioColors(Collection* c);
void __fastcall Cities_ColorBooms(Collection* c, int, int32_t value);
void __fastcall Trains_ColorOwner(Collection* c, int, int32_t company);
void __fastcall Objects_HighlightGroup(Collection* t, int, int32_t group);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
int32_t Tutorial_Blink37a();
void __fastcall Picker_Hover90ec(int32_t idx, int32_t on);
void __fastcall Picker_Hover9376(int32_t idx, int32_t on);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
void __fastcall BuildingInfo_Show(uint32_t a, uint32_t building);
void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall Newspaper_Show(uint8_t* rec);
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
RT3_NOINLINE void __fastcall Terrain_RebuildWaterMask(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall Newspaper_DrawRules(uint8_t* w);

#define RT3_BRUSH_INT(addr, name, table) \
    void __fastcall name(uint8_t* o, int, int32_t i) { \
        F<int32_t>(o, 0x78) = i; \
        const float r = (float)((double)((const int32_t*)table)[i] + D(0x5c8628)); \
        Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, r); \
    } \
    RT3_REPLACE(addr, name);

void* __fastcall Table_FindField14Prev(Collection* t, int, int32_t v);

// 0x4f5960 / 0x4f59f0 (slider callbacks). Terrain paint weight / parameter
// i (slider 0x2922.. / 0x2936..) = v / 0x5c85d8 into 0x622728 / 0x622738,
// shown in percent on label 0x292c.. / 0x2940...
#define RT3_PAINT_SLIDER(addr, name, first, table, label) \
    void __fastcall name(uint8_t* w, int32_t v) { \
        if (!g_6d1334) return; \
        const int32_t i = F<uint16_t>(w, 4) - first; \
        const float f = (float)((double)v / D(0x5c85d8)); \
        ((float*)table)[i] = f; \
        char text[0x190]; \
        Num_Format(text, 1, (double)f * Fl(0x5c8c28), 10, 1, 0, 0); \
        Window_SendEvent5(g_6d1334, 0, 0x6f, i + label, (uint32_t)(uintptr_t)text, 0, 0); \
    } \
    RT3_REPLACE(addr, name);

void* __fastcall Table_FindField14Next(Collection* t, int, int32_t v);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
void __fastcall Brush_SetSizeI6226e8(uint8_t* o, int, int32_t i);
void __fastcall Brush_SetSizeI62270c(uint8_t* o, int, int32_t i);
void __fastcall Map_MarkBlocks0(uint8_t* m, int, float x, float y, float r, uint8_t v);
void __fastcall Map_MarkBlocks3(uint8_t* m, int, float x, float y, float r, uint8_t v);
void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value);
int32_t __fastcall MapEditor_IsTool4Mode2(uint8_t* e);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
RT3_NOINLINE void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
void __fastcall Newspaper_DrawCb(void*, int, float x, float y, float w, float, int32_t);
void __fastcall Building_AddScrollbars(uint8_t* win, int, float x, float y, int32_t, int32_t);
RT3_NOINLINE void __fastcall BuildingInfo_Show(uint32_t a, uint32_t building);
int32_t Global6d12bcSet();
void SetGlobal6d12cc();
RT3_NOINLINE void __fastcall Newspaper_Fill(uint8_t* w, int, uint8_t* rec);
RT3_NOINLINE void __fastcall Newspaper_DrawRules(uint8_t* w);
void __fastcall Newspaper_DrawRulesCb(void*, int, int32_t, int32_t, int32_t, int32_t, int32_t);
uint8_t* __fastcall Newspaper_Construct(uint8_t* w);
int32_t __fastcall Newspaper_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE void __fastcall Newspaper_Show(uint8_t* rec);
void __fastcall TrackWindow_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Picker_Hover90ec(int32_t idx, int32_t on);
void __fastcall Picker_HoverCb90ec(int32_t id, int32_t was, int32_t now, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Picker_Hover9376(int32_t idx, int32_t on);
void __fastcall Picker_HoverCb9376(int32_t id, int32_t was, int32_t now, int32_t, int32_t, int32_t, int32_t);
void __fastcall PickerWindow_Build(uint8_t* w);
RT3_NOINLINE void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value);
uint32_t __fastcall TrackPicker_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint8_t* __fastcall TrackPicker_Construct(uint8_t* w);
void __fastcall MapWindow10_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall CityEditor_Show(uint8_t* w);
int32_t __fastcall RegionBrush_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall CityToolWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall Window_HighlightTabs7And5(uint8_t* w);
uint8_t* __fastcall RegionToolWindow_Construct(uint8_t* w);
void __fastcall MapWindow11_Destruct(uint8_t* w);
int32_t __fastcall TerrainBrush_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall Editor_PushUndo(uint8_t* o);
RT3_NOINLINE void __fastcall Editor_UndoPaint(uint8_t* e);
RT3_NOINLINE void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx);
RT3_NOINLINE void __fastcall Map_SetStyle(uint8_t* o, int, int32_t style);
RT3_NOINLINE void __fastcall Window_BalancePercentages(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Screen_IsMode5_2(uint8_t* s);
RT3_NOINLINE int32_t __fastcall MapEditor_IsTool4Mode2(uint8_t* e);
void __fastcall MapEditorWindow_Destruct(uint8_t* w);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE int32_t __fastcall Widget_TextLines(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);

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

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

// Shared by 0x4f3b90 / 0x4f3c90: item `idx` of a picker (window 0x6d12f0, control
// base `base`) is hovered (`on`) or left. With the mouse button held (input
// +0xa8c bit 10) hovering a new item plays a tick (sound 0x20, centred) and arms
// it (0x6d12d0); otherwise a hover over the buttons 0..2 highlights it (0xa7,
// -0x867); leaving it un-highlights it (-1) and falls back to the window's current
// item when nothing was armed.
static void Picker_Hover(int32_t idx, int32_t on, int32_t base) {
    if (on) {
        uint32_t buttons = F<uint32_t>(g_input, 0xa8c);
        if (buttons & 0x400) {
            uint8_t* w = g_6d12f0;
            if (w && idx != F<int32_t>(w, 0x7c) && idx != g_6d12d4) Ui_PlaySound(0x20, 0, 0x3f000000);
            g_6d12d4 = idx;
            g_6d12d0 = 1;
            return;
        }
        if (!(buttons & 7)) return;
        Window_SendEvent5(g_6d12f0, 0, 0xa7, (uint32_t)(idx + base), 0xfffff799, 0, 0);
        g_6d12d4 = idx;
        g_6d12d0 = 0;
        return;
    }
    Window_SendEvent5(g_6d12f0, 0, 0xa7, (uint32_t)(idx + base), 0xffffffff, 0, 0);
    if (g_6d12d4 != -1) return;
    g_6d12d4 = F<int32_t>(g_6d12f0, 0x7c);
    g_6d12d0 = 0;
}

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

// Shared body of the map-mode window destructors 0x4f50d0 / 0x4f5750 /
// 0x4fcb40: drop the scene's busy mark and leave their overview mode.
static void MapModeWindow_Destruct(uint8_t* w, uint32_t vtbl, int32_t mode, uint32_t clear) {
    F<uint32_t>(w, 0) = vtbl;
    Scene_PopBusy((uint8_t*)Engine_Renderer(g_engine));
    if (clear) hook::Global<uint32_t>(clear) = 0;
    if (F<int32_t>(g_map, 0x2175) == mode) Overview_Close(g_map);
    Widget_SetVtbl(w);
}

// Shared opening of the editor tool windows 0x4f54d0 / 0x4f5670 / 0x4fcf30:
// vtable, brush size from the saved table (radius = sizes[i] +
// 0.5).
static void EditorToolWindow_Open(uint8_t* w, uint32_t vtbl, uint32_t sizeIdx, uint32_t sizes) {
    int32_t i = hook::Global<int32_t>(sizeIdx);
    F<uint32_t>(w, 0) = vtbl;
    F<int32_t>(w, 0x78) = i;
    float r = (float)((double)((const int32_t*)sizes)[i] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(g_engine), 0, r);
}

// Recolours the map for company `company` on the track picker's company tab.
static void TrackPicker_ColorCompany(int32_t company) {
    Table_RefreshAll(g_buildings);
    Table_ForAllCityChains(g_trains);
    Tracks_UpdateColor(g_tracks);
    Cities_ColorBooms(g_buildings, 0, company);
    Trains_ColorOwner(g_trains, 0, company);
    Objects_HighlightGroup(g_tracks, 0, company);
}

namespace {
uint32_t Message448(const char* text) {
    return Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

// 0x4f29c0 (a in ECX, `quick` in DL). Shows the company report dialog
// (panels 0x4f1490 / 0x4f1f90 / 0x4f14d0, fonts 2, 0xc and 1 held in
// 0x6d12a4..0x6d12ac) for a / b; a quick one is skipped when reports are off
// (options +0x26b) or the scenario hides them (+0x4aeb).
RT3_NOINLINE void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b) {
    if ((F<int32_t>(g_world, 0x4aeb) || !F<int32_t>(g_app, 0x26b)) && quick) return;
    g_6d129c = a;
    hook::As<uint32_t>(g_6d12a0) = b;
    g_6226d8 = quick;
    g_6d12a4 = Pack_AcquireFont(g_pack, 0, 2);
    g_6d12a8 = Pack_AcquireFont(g_pack, 0, 0xc);
    g_6d12ac = Pack_AcquireFont(g_pack, 0, 1);
    Dialog_RunMessage((const char*)0x5c87a8, 1, 0xbf800000, 0xbf800000, 0x4f1490,
                      (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4f1f90, 0x4f14d0, Prod_Bits(0x3f0aaaab), 0, 0x1e, 0, nullptr,
                      nullptr, 0, 0, nullptr, 0);
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d12a4));
    hook::As<uint32_t>(g_6d12a4) = 0;
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d12a8));
    hook::As<uint32_t>(g_6d12a8) = 0;
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d12ac));
    g_6d129c = 0;
}
RT3_REPLACE(0x4f29c0, CompanyReport_Show);

// 0x4f2ad0 (draw callback). Newspaper headline (context 0x6d12b0 in table
// 0x62b268, 0x6d12b4 in 0x62b26c): the story's localised title (+0x4f) and
// below it string 0xe7 naming it and the place nearest its subject.
void __fastcall Newspaper_DrawCb(void*, int, float x, float y, float w, float, int32_t) {
    uint8_t* story = (uint8_t*)Collection_Get(g_62b268, 0, g_6d12b0);
    uint8_t* subject = nullptr;
    if (Collection_IsValid(g_buildings, 0, g_6d12b4))
        subject = (uint8_t*)Collection_Get(g_buildings, 0, g_6d12b4);
    float ty = (float)((double)y + D(0x5c9cb0));
    Font_DrawTextBox(g_6d12b8, 0, LocalizeName((const char*)story + 0x4f), x, ty, w, BitsF(0x3ccccccd), 0xffffffff, 0, 0, 0, 1.0f);
    ty = (float)((double)ty + D(0x5d1358));
    const char* place = (const char*)0x5c87a8;
    if (subject) {
        const float cy = (float)Obj_CellY(subject);
        const float cx = (float)Obj_CellX(subject);
        place = Table_NearestName(g_cities, 0, cx, cy);
    }
    const char* title = story ? LocalizeName((const char*)story + 0x4f) : (const char*)0x5c87a8;
    char frame[0x1f8];
    char* text = frame + 4;
    FormatText(text, GetString(0xe7), title, place, 0, 0, 0, 0, 0, 0);
    Font_DrawTextBox(g_6d12b8, 0, text, x, ty, w, BitsF(0x3dddddde), 0xffffffff, 0, 1, 0, 1.0f);
}
RT3_REPLACE(0x4f2ad0, Newspaper_DrawCb);

// 0x4f2c20. Adds a scrollbar (style 0xb0, range 3..9) at (x, y) and, for the
// building type of object 0x6d12b4 (0x62b26c), a second one labelled with the
// type's 2D model name.
void __fastcall Building_AddScrollbars(uint8_t* win, int, float x, float y, int32_t, int32_t) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d1364));
    F<uint32_t>(def, 0xf) = 0x3e82e148;
    F<uint32_t>(def, 0x13) = 0x3e1f92c6;
    F<int32_t>(def, 0x61) = 3;
    F<float>(def, 0xb) = (float)((double)y + D(0x5ca1a8));
    F<int32_t>(def, 0x75) = 9;
    F<int32_t>(def, 0x2f) = 0xb0;
    uint8_t* bar = (uint8_t*)ExeNew(0x111);
    if (bar) bar = Scrollbar_Construct(bar, 0, def);
    Window_AddChild(win, 0, bar, 0);
    void* obj = Collection_Get(g_buildings, 0, g_6d12b4);
    if (!obj) return;
    char name[0x190];
    BuildingType_Model2D((uint8_t*)Obj_Ref3cc(obj), 0, name);
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d1360));
    F<float>(def, 0xb) = (float)((double)y + D(0x5d0a60));
    F<uint32_t>(def, 0xf) = 0x3e7b851f;
    F<uint32_t>(def, 0x13) = 0x3e11eb85;
    String_Assign(&F<char*>(def, 0x43), name);
    F<uint16_t>(def, 0x5f) = 0;
    F<int32_t>(def, 0x2f) = 0xb0;
    uint8_t* bar2 = (uint8_t*)ExeNew(0x111);
    if (bar2) bar2 = Scrollbar_Construct(bar2, 0, def);
    Window_AddChild(win, 0, bar2, 0);
    Free(F<void*>(def, 0x43));
}
RT3_REPLACE(0x4f2c20, Building_AddScrollbars);

// 0x4f2d80 (fastcall a, building). With building info on (0x6cec74+0x26f)
// and not in a cut scene (0x6cec78+0x4aeb): shows the building info dialog
// (0x4f2c20 lays it out, 0x4f2ad0 draws; context 0x6d12b0 / 0x6d12b4 and a
// held font 0x6d12b8); on OK (0x3f2) the building view opens on it and the
// camera goes to it.
RT3_NOINLINE void __fastcall BuildingInfo_Show(uint32_t a, uint32_t building) {
    if (!F<int32_t>(g_app, 0x26f) || F<int32_t>(g_world, 0x4aeb)) return;
    g_6d12b0 = a;
    g_6d12b4 = building;
    g_6d12b8 = Pack_AcquireFont(g_pack, 0, 2);
    static const uint32_t kExtraH = 0x3e2aaaab;
    typedef void(__fastcall * PlaceFn)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float);
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 0xa, 0xbf800000, 0xbf800000, 0, (PlaceFn)0x4f2c20, 0x4f2ad0, *(const float*)&kExtraH, 0, 0x1e, 0, nullptr,
                                   nullptr, 0, 0, nullptr, 0);
    Pack_Release(g_pack, 0, g_6d12b8);
    g_6d12b8 = nullptr;
    if (r == 0x3f2) {
        if (uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, g_6d12b4)) {
            ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(g_gameWindow, 0, 0xa, hook::As<int32_t>(g_6d12b4));
            Camera_FollowObject(b, 0);
        }
    }
    g_6d12b0 = 0;
    g_6d12b4 = 0;
}
RT3_REPLACE(0x4f2d80, BuildingInfo_Show);

int32_t Global6d12bcSet() { return hook::As<void*>(g_6d12bc) ? 1 : 0; }
RT3_REPLACE(0x4f2e80, Global6d12bcSet);

void SetGlobal6d12cc() { g_6d12cc = 1; }
RT3_REPLACE(0x4f2e90, SetGlobal6d12cc);

// 0x4f2ea0. Fills the newspaper with the story `rec` (+0x78; marks it read,
// +0x18): the paper's name (0x5f2b78, picked by the story's seed +8), the
// price by year (5 / 10 / 15 / 25 / 50 cents, string 0xea), the date; the
// headline (+0x19), the story (+0x92) and a caption (0x5f2b90) as text boxes
// stacked from +0x7c (the headline sets +0x7c), sides swapped by the seed; and
// six picture frames (0x88eb..0x88ef) whose images follow from the seed.
RT3_NOINLINE void __fastcall Newspaper_Fill(uint8_t* w, int, uint8_t* rec) {
    if (!rec) return;
    F<uint8_t*>(w, 0x78) = rec;
    rec[0x18] = 1;
    uint8_t* c = Widget_FindChild(w, 0, 0x88ba);
    String_Assign(&F<char*>(c, 0xba), GetString(hook::Global<int32_t>(0x5f2b78 + HashRand(F<int32_t>(F<uint8_t*>(w, 0x78), 8), 0, 4) * 4)));
    c = Widget_FindChild(w, 0, 0x88bb);
    uint16_t year = F<uint16_t>(F<uint8_t*>(w, 0x78), 0xc);
    int32_t price = year < 0x771 ? 5 : year < 0x79e ? 10 : year < 0x7a8 ? 15 : year < 0x7c1 ? 25 : 50;
    char text[0x120];
    Num_Format(text, 1, (double)price, 10, 1, 0, 0);
    FormatText(text + 0x10, GetString(0xea), text, 0, 0, 0, 0, 0, 0, 0);
    String_Assign(&F<char*>(c, 0xba), text + 0x10);
    c = Widget_FindChild(w, 0, 0x88bc);
    Date_Format(text + 0x10, F<uint8_t*>(w, 0x78) + 0xc, 2);
    String_Assign(&F<char*>(c, 0xba), text + 0x10);
    Window_DeleteChildren(w, 0, 0x88e8, 0x88f4, 0);
    auto textBox = [&](uint16_t id, uint32_t x, uint32_t y, uint32_t wd, uint8_t style, const char* s) {
        uint8_t def[0x91];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = y;
        F<uint32_t>(def, 0xf) = wd;
        F<uint32_t>(def, 0x34) = 0xff000000;
        F<uint32_t>(def, 0x2f) = 0xb0;
        String_Assign(&F<char*>(def, 0x77), s);
        def[0x33] = style;
        F<int32_t>(def, 0x6f) = 1;
        F<uint16_t>(def, 0) = id;
        uint8_t* b = (uint8_t*)ExeNew(0xf0);
        b = b ? EditBox_Construct(b, 0, def) : nullptr;
        struct R { uint8_t* box; void* str; };
        return R{ b, F<void*>(def, 0x77) };
    };
    auto r1 = textBox(0x88e8, 0x3e351eb8, 0x3ea6d3a0, 0x3f24f5c3, 0xf, (const char*)F<uint8_t*>(w, 0x78) + 0x19);
    float t = (float)Widget_TextLines(r1.box);
    t = (float)((double)t * Fl(0x5caa9c));
    F<float>(r1.box, 0x17) = (float)((double)t / Fl(0x5ca164));
    F<float>(w, 0x7c) = (float)((double)t + Fl(0x5d138c));
    Window_AddChild(w, 0, r1.box, 0);
    Free(r1.str);
    uint32_t a, b;
    if (HashRand(F<int32_t>(F<uint8_t*>(w, 0x78), 8), 0, 1)) a = 0x438f0000, b = 0x44648000;
    else a = 0x44328000, b = 0x438f0000;
    float ax = (float)((double)BitsF(a) / Fl(0x5ca198));
    auto r2 = textBox(0x88e9, FBits(ax), FBits((float)((double)F<float>(w, 0x7c) / Fl(0x5ca164))), 0x3ec00000, 0xd, (const char*)F<uint8_t*>(w, 0x78) + 0x92);
    t = (float)(Widget_TextLines(r2.box) * 0x38);
    F<float>(r2.box, 0x17) = (float)((double)t / Fl(0x5ca164));
    float u = (float)((double)t + F<float>(w, 0x7c) + Fl(0x5c85f8));
    Window_AddChild(w, 0, r2.box, 0);
    Free(r2.str);
    float bx = (float)((double)BitsF(b) / Fl(0x5ca198));
    uint32_t by = FBits((float)((double)F<float>(w, 0x7c) / Fl(0x5ca164)));
    auto r3 = textBox(0x88ea, FBits(bx), by, 0x3e800000, 0xe,
                      GetString(hook::Global<int32_t>(0x5f2b90 + HashRand(F<int32_t>(F<uint8_t*>(w, 0x78), 8), 0, 0x26) * 4)));
    t = (float)(Widget_TextLines(r3.box) * 40);
    F<float>(r3.box, 0x17) = (float)((double)t / Fl(0x5ca164));
    float u3 = (float)((double)t + F<float>(w, 0x7c) + Fl(0x5c85f8));
    Window_AddChild(w, 0, r3.box, 0);
    Free(r3.str);
    auto frame = [&](uint16_t id, uint32_t x, uint32_t y, uint32_t h, int32_t image, uint32_t cx, uint32_t cw) {
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = y;
        F<uint32_t>(def, 0xf) = 0x3e000000;
        F<uint32_t>(def, 0x13) = h;
        F<uint16_t>(def, 0) = id;
        F<uint16_t>(def, 0x5f) = (uint16_t)image;
        String_Assign(&F<char*>(def, 0x43), (const char*)0x5d1374);
        uint8_t* s = (uint8_t*)ExeNew(0x111);
        s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
        Widget_SetClip(s, 0, cx, y, cw, 0x3f4a3d71);
        Window_AddChild(w, 0, s, 0);
        Free(F<void*>(def, 0x43));
    };
    auto seed = [&] { return F<int32_t>(F<uint8_t*>(w, 0x78), 8); };
    float uy = (float)((double)u / Fl(0x5ca164));
    float hh = (float)((Fl(0x5d1388) - (double)u) / Fl(0x5ca164));
    float x2 = (float)(((double)BitsF(a) + Fl(0x5c9f00)) / Fl(0x5ca198));
    frame(0x88eb, FBits(ax), FBits(uy), FBits(hh), HashRand(seed(), 0, 4), FBits(ax), FBits(x2));
    int32_t v = HashRand(seed(), 5, 7);
    float x3 = (float)(((double)BitsF(a) + Fl(0x5c8904)) / Fl(0x5ca198));
    frame(0x88ec, FBits(x2), FBits(uy), FBits(hh), v, FBits(x2), FBits(x3));
    float x4 = (float)(((double)BitsF(a) + Fl(0x5c8cf8)) / Fl(0x5ca198));
    frame(0x88ed, FBits(x3), FBits(uy), FBits(hh), v == 7 ? 5 : v + 1, FBits(ax), FBits(x4));
    float uy2 = (float)((double)u3 / Fl(0x5ca164));
    float hh2 = (float)((Fl(0x5d1388) - (double)u3) / Fl(0x5ca164));
    v = HashRand(seed(), 0, 4);
    float x5 = (float)(((double)BitsF(b) + Fl(0x5c9f00)) / Fl(0x5ca198));
    frame(0x88ee, FBits(bx), FBits(uy2), FBits(hh2), v == 4 ? 0 : v + 1, FBits(bx), FBits(x5));
    v = HashRand(seed(), 5, 7);
    float x6 = (float)(((double)BitsF(b) + Fl(0x5c8904)) / Fl(0x5ca198));
    frame(0x88ef, FBits(x5), FBits(uy2), FBits(hh2), v == 5 ? 7 : v - 1, FBits(x5), FBits(x6));
}
RT3_REPLACE(0x4f2ea0, Newspaper_Fill);

// 0x4f3750. Newspaper page decoration (while the news list 0x6cea4c exists): a
// vertical and a horizontal rule (black, with glow) whose layout is picked by
// a coin flip seeded with the edition (+0x78 -> +8); the rule height follows +0x7c.
RT3_NOINLINE void __fastcall Newspaper_DrawRules(uint8_t* w) {
    if (!hook::As<int32_t>(g_6cea4c)) return;
    int32_t flip = HashRand(F<int32_t>(F<uint8_t*>(w, 0x78), 8), 0, 1);
    float yv = (float)(((double)F<float>(w, 0x7c) - Fl(0x5c878c)) / Fl(0x5ca164) + F<float>(w, 9));
    float x = (float)((double)F<float>(w, 5) + Fl(flip ? 0x5d13a0 : 0x5d1394));
    float y2 = (float)((double)F<float>(w, 9) + Fl(0x5d139c));
    Ui_Line(0xff000000, 1, x, yv, x, y2, 1.0f);
    float hx2 = (float)((double)F<float>(w, 5) + Fl(flip ? 0x5d1398 : 0x5d1394));
    float hx1 = (float)((double)F<float>(w, 5) + Fl(flip ? 0x5d13a0 : 0x5d1390));
    Ui_Line(0xff000000, 1, hx1, yv, hx2, yv, 1.0f);
}
RT3_REPLACE(0x4f3750, Newspaper_DrawRules);

// 0x4f3880 (draw callback). Draws the newspaper rules when shown.
void __fastcall Newspaper_DrawRulesCb(void*, int, int32_t, int32_t, int32_t, int32_t, int32_t) {
    if (hook::As<int32_t>(g_6cea4c)) Newspaper_DrawRules(g_6d12bc);
}
RT3_REPLACE(0x4f3880, Newspaper_DrawRulesCb);

// 0x4f38a0. Newspaper dialog constructor ("Newspaper.win" at (0, -0.0917), vtable
// 0x5d1368): no page yet (+0x7c), handler 0x4f3880 on control 0x88bf.
RT3_NOINLINE uint8_t* __fastcall Newspaper_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1368;
    Window_Load(w, 0, 0.0f, BitsF(0xbdbbbbbcu), (const char*)0x5d13a4);
    F<int32_t>(w, 0x7c) = 0;
    Window_SendEvent5(w, 0, 0x7a, 0x88bf, 0x4f3880, 0, 0);
    return w;
}
RT3_REPLACE(0x4f38a0, Newspaper_Construct);

// 0x4f38f0 (newspaper events). Presses of 0x88b9 / 0x88c0 click (sound
// 0x2b); releases of 0x88bd / 0x88be page to the previous / next paper of
// the log (string 0xe8 / 0xe9 at the end); an idle event closes a pending
// one (0x6d12cc). Returns 0 when it paged.
int32_t __fastcall Newspaper_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    switch (ev[0]) {
    case 0xc9:
        return 1;
    case 0:
        if (g_6d12cc) {
            g_6d12cc = 0;
            ev[0] = 0xc9;
        }
        return 1;
    case 0xca:
        if (ev[1] == 0x88b9 || ev[1] == 0x88c0) {
            Ui_PlaySound(0x2b, 0, 0x3f000000);
            ev[0] = 0xc9;
        }
        return 1;
    case 0xcb: {
        uint8_t* cur = F<uint8_t*>(w, 0x78);
        if (ev[1] == 0x88bd) {
            uint8_t* r = (uint8_t*)Table_FindField14Prev(g_6cea4c, 0, F<int32_t>(cur, 0x14));
            if (r && r != F<uint8_t*>(w, 0x78)) {
                Newspaper_Fill(w, 0, r);
                return 0;
            }
            Message448(GetString(0xe8));
            return 0;
        }
        if (ev[1] == 0x88be) {
            uint8_t* r = (uint8_t*)Table_FindField14Next(g_6cea4c, 0, F<int32_t>(cur, 0x14));
            if (r && r != F<uint8_t*>(w, 0x78)) {
                Newspaper_Fill(w, 0, r);
                return 0;
            }
            Message448(GetString(0xe9));
            return 0;
        }
        return 1;
    }
    }
    return 1;
}
RT3_REPLACE(0x4f38f0, Newspaper_HandleEvent);

// 0x4f3a10. Shows newspaper record `rec` modally (only when newspapers are on,
// 0x6cea4c, and none is open, 0x6d12bc) with its sound 0x2a, then frees it and
// restores the cursor when the view mode is unchanged.
RT3_NOINLINE void __fastcall Newspaper_Show(uint8_t* rec) {
    if (!hook::As<uint32_t>(g_6cea4c) || g_6d12bc) return;
    hook::As<uint32_t>(g_6d12cc) = 0;
    int32_t mode = F<int32_t>(g_gameWindow, 0x8c);
    int32_t cursor = F<int32_t>(g_cursorManager, 4);
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x80);
    w = w ? Newspaper_Construct(w) : nullptr;
    g_6d12bc = w;
    Newspaper_Fill(w, 0, rec);
    Ui_PlaySound(0x2a, 0, 0x3f000000);
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d12bc, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d12bc);
    if (uint8_t* p = g_6d12bc) {
        F<uint32_t>(p, 0) = 0x5d1368;
        g_6d12bc = nullptr;
        F<uint32_t>(p, 0x78) = 0;
        Widget_SetVtbl(p);
        ((void(__cdecl*)(void*))0x53b080)(p);
    }
    g_6d12bc = nullptr;
    if (mode == F<int32_t>(g_gameWindow, 0x8c) && F<int32_t>(g_cursorManager, 4) != 6)
        Cursor_Set(g_cursorManager, 0, cursor, 0, 0);
}
RT3_REPLACE(0x4f3a10, Newspaper_Show);

// 0x4f3b30. Track-laying window destructor: ends the track preview (+0x78 1),
// closes the overview mode, recolours the tracks, refreshes city chains and
// buildings, and forgets the window (0x6d12f0).
void __fastcall TrackWindow_Destruct(uint8_t* w) {
    int32_t previewing = F<int32_t>(w, 0x78) == 1;
    F<uint32_t>(w, 0) = 0x5d13b4;
    if (previewing) Obj_DecA0(hook::As<void*>(g_tracks));
    Overview_Close(g_map);
    Tracks_UpdateColor(g_tracks);
    Table_ForAllCityChains(g_trains);
    Table_RefreshAll(g_buildings);
    hook::As<uint32_t>(g_6d12f0) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4f3b30, TrackWindow_Destruct);

RT3_NOINLINE void __fastcall Picker_Hover90ec(int32_t idx, int32_t on) { Picker_Hover(idx, on, 0x90ec); }
RT3_REPLACE(0x4f3b90, Picker_Hover90ec);

// 0x4f3c60 / 0x4f3d60 (hover callbacks). Picker widgets 0x91b4.. / 0x93a8..
// get the new hover state when it changes.
void __fastcall Picker_HoverCb90ec(int32_t id, int32_t was, int32_t now, int32_t, int32_t, int32_t, int32_t) {
    if (g_6d12f0 && was != now) Picker_Hover90ec(id - 0x91b4, now);
}
RT3_REPLACE(0x4f3c60, Picker_HoverCb90ec);

RT3_NOINLINE void __fastcall Picker_Hover9376(int32_t idx, int32_t on) { Picker_Hover(idx, on, 0x9376); }
RT3_REPLACE(0x4f3c90, Picker_Hover9376);

void __fastcall Picker_HoverCb9376(int32_t id, int32_t was, int32_t now, int32_t, int32_t, int32_t, int32_t) {
    if (g_6d12f0 && was != now) Picker_Hover9376(id - 0x93a8, now);
}
RT3_REPLACE(0x4f3d60, Picker_HoverCb9376);

// 0x4f3d90. Fills the picker window for its tab +0x78 (tab buttons 0x9089..
// 0x908d and 0x908e / 0x908f / 0x9090; content 0x90ec..0x93d9). Tab 3: up to
// 12 companies in rows of 6 (the first picked when +0x7c is unset): logo (a
// company without one is given a free logo slot at random), name and a
// selectable frame (0x93a8 + id, callback 0x4f3d60). Tab 0: "all cargo" and
// every available cargo, 5 per row: name, icon and a selectable frame
// (0x91b4 + id, callback 0x4f3c60) whose tooltip gives the supply and demand
// (or the plain description for cargo without a demand entry).
RT3_NOINLINE void __fastcall PickerWindow_Build(uint8_t* w) {
    for (int32_t i = 0; i < 5; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0x9089 + i, 1, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(w, 0x78) != 1 ? 0x87 : 0x65, 0x908f, 1, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(w, 0x78) != 2 ? 0x87 : 0x65, 0x908e, 1, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(w, 0x78) != 4 ? 0x87 : 0x65, 0x9090, 1, 0, 0);
    Window_DeleteChildren(w, 0, 0x90ec, 0x93d9, 0);
    auto cos = [] { return g_companies; };
    auto scrollbar = [&](uint8_t* def) {
        uint8_t* s = (uint8_t*)ExeNew(0x111);
        return s ? Scrollbar_Construct(s, 0, def) : nullptr;
    };
    auto editBox = [&](uint8_t* def) {
        uint8_t* b = (uint8_t*)ExeNew(0xf0);
        b = b ? EditBox_Construct(b, 0, def) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(def, 0x77));
    };
    auto pick = [&](uint8_t* s, uint32_t id) {
        if (id == F<uint32_t>(w, 0x7c)) Widget_SetFlags(s, 0, 1, 0, 0);
        else Widget_ClearFlags(s, 0, 1);
    };
    if (F<int32_t>(w, 0x78) == 3) {
        int32_t n = Table_CountField3f(cos());
        if (n >= 12) n = 12;
        int32_t rows = (n - 1) / 6 + 1;
        if (!F<int32_t>(w, 0x7c) && n) F<uint32_t>(w, 0x7c) = Table_NthIdField3f(cos(), 0, 0);
        if (n > 0) {
            float colW = (float)(D(0x5d13e0) / (double)rows);
            for (int32_t i = 0; i < n; ++i) {
                uint32_t id = Table_NthIdField3f(cos(), 0, i);
                int32_t q = i / 6, r = i % 6;
                uint8_t def[0x89];
                memset(def, 0, sizeof def);
                float rf = (float)r;
                float rx = (float)((double)rf * Fl(0x5d13dc));
                F<float>(def, 7) = (float)(((double)rx + Fl(0x5d13d8)) / Fl(0x5ca198));
                F<uint32_t>(def, 0xf) = 0x3d75c28f;
                F<uint32_t>(def, 0x13) = 0x3d4ccccd;
                F<uint16_t>(def, 0) = (uint16_t)(id + 0x9344);
                def[0x27] = 0;
                def[0x28] = 0;
                float qf = (float)q;
                float ry = (float)((double)qf * colW);
                F<float>(def, 0xb) = (float)(((double)ry + Fl(0x5d13d4)) / Fl(0x5ca164));
                String_Assign(&F<char*>(def, 0x3f), (const char*)Collection_Get(cos(), 0, id) + 4);
                const char* img;
                if (F<int32_t>(Collection_Get(cos(), 0, id), 0x37) != -1) {
                    F<uint16_t>(def, 0x5f) = F<uint16_t>(Collection_Get(cos(), 0, id), 0x37);
                    img = (const char*)0x5c9f98;
                } else {
                    F<uint16_t>(def, 0x5f) = 0;
                    uint8_t* co = (uint8_t*)Collection_Get(cos(), 0, id);
                    if (NameTable_ContainsName(g_pack, 0, (const char*)co + 0xd65, 0, 0))
                        img = (const char*)Collection_Get(cos(), 0, id) + 0xd65;
                    else {
                        int32_t slot = NthFreeCompanySlot(Rand(0, CountFreeCompanySlots() - 1));
                        F<int32_t>(Collection_Get(cos(), 0, id), 0x37) = slot;
                        F<uint16_t>(def, 0x5f) = F<uint16_t>(Collection_Get(cos(), 0, id), 0x37);
                        img = (const char*)0x5c9f98;
                    }
                }
                String_Assign(&F<char*>(def, 0x43), img);
                uint8_t* s = scrollbar(def);
                F<uint32_t>(s, 0x100) = 0x3f000000;
                F<uint32_t>(s, 0x104) = 0x3f000000;
                Window_AddChild(w, 0, s, 0);
                Free(F<void*>(def, 0x43));
                Free(F<void*>(def, 0x3f));
                uint8_t eb[0x91];
                memset(eb, 0, sizeof eb);
                float ex = (float)(((double)rx + Fl(0x5d13d0)) / Fl(0x5ca198));
                F<uint32_t>(eb, 0xf) = 0x3dae147b;
                F<uint32_t>(eb, 0x13) = 0x3d4ccccd;
                eb[0x27] = 0;
                eb[0x28] = 1;
                F<int32_t>(eb, 0x34) = -1;
                F<float>(eb, 7) = ex;
                F<float>(eb, 0xb) = (float)(((double)ry + Fl(0x5ca210)) / Fl(0x5ca164));
                String_Assign(&F<char*>(eb, 0x77), (const char*)Collection_Get(cos(), 0, id) + 4);
                eb[0x29] = 0x20;
                eb[0x33] = 1;
                F<uint16_t>(eb, 0) = (uint16_t)(id + 0x9376);
                editBox(eb);
                memset(def, 0, sizeof def);
                F<float>(def, 7) = ex;
                F<uint32_t>(def, 0xf) = 0x3dae147b;
                F<uint32_t>(def, 0x13) = 0x3de740da;
                F<uint16_t>(def, 0) = (uint16_t)(id + 0x93a8);
                def[0x29] = 0x20;
                F<int32_t>(def, 0x61) = 2;
                F<float>(def, 0xb) = (float)(((double)ry + Fl(0x5c8568)) / Fl(0x5ca164));
                uint8_t* a = (uint8_t*)Collection_Get(cos(), 0, id);
                uint8_t* b = (uint8_t*)Collection_Get(cos(), 0, id);
                char tip[0x200];
                FormatText(tip, GetString(0xeb), (const char*)b + 4, (const char*)a + 4, 0, 0, 0, 0, 0, 0);
                String_Assign(&F<char*>(def, 0x3f), tip);
                s = scrollbar(def);
                pick(s, id);
                Window_AddChild(w, 0, s, 0);
                Free(F<void*>(def, 0x3f));
                Window_SendEvent5(w, 0, 0x7b, id + 0x93a8, 0x4f3d60, 0, 0);
            }
        }
    }
    if (F<int32_t>(w, 0x78)) return;
    auto cargo = [] { return g_cargoTypes; };
    int32_t rows = (F<int32_t>(cargo(), 0x242) - 1) / 5 + 2;
    int32_t shown = 0;
    if (Collection_Count(cargo()) <= -1) return;
    for (int32_t i = -1; i < Collection_Count(cargo()); ++i) {
        uint32_t id;
        int32_t row, col;
        if (i == -1) {
            id = 0;
            row = 0;
            col = 2;
        } else {
            id = Collection_IdAt(cargo(), 0, i, 0);
            if (!Obj_YearReached(Collection_Get(cargo(), 0, id))) continue;
            Collection_Get(cargo(), 0, id);
            row = shown / 5 + 1;
            col = shown % 5;
            ++shown;
        }
        float rowH = (float)(D(0x5d13c8) / (double)rows);
        uint8_t eb[0x91];
        memset(eb, 0, sizeof eb);
        F<uint32_t>(eb, 0xf) = 0x3da5e354;
        eb[0x27] = 3;
        eb[0x28] = 0;
        F<int32_t>(eb, 0x34) = -1;
        float cf = (float)col;
        float cx = (float)((double)cf * Fl(0x5d13c0));
        F<float>(eb, 7) = (float)(((double)cx - Fl(0x5c85b4) + Fl(0x5ca058) - Fl(0x5c8898)) / Fl(0x5ca198));
        float rf = (float)row;
        float ry = (float)((double)rf * rowH);
        F<float>(eb, 0xb) = (float)(((double)ry - Fl(0x5c878c) + Fl(0x5c8900)) / Fl(0x5ca164));
        double hd = (double)rowH / D(0x5ca2d0);
        F<float>(eb, 0x13) = (float)hd;
        const char* text = id ? LocalizeName((const char*)Collection_Get(cargo(), 0, id) + 4) : GetString(0xec);
        String_Assign(&F<char*>(eb, 0x77), text);
        char name[0x200];
        strcpy(name, F<const char*>(eb, 0x77));
        eb[0x29] = 0x20;
        eb[0x33] = 1;
        F<uint16_t>(eb, 0) = (uint16_t)(id + 0x90ec);
        editBox(eb);
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 0xf) = 0x3ccccccd;
        F<uint32_t>(def, 0x13) = 0x3cf5c28f;
        float c1 = (float)(col + 1);
        F<uint16_t>(def, 0) = (uint16_t)(id + 0x927c);
        F<float>(def, 7) = (float)(((double)c1 * Fl(0x5d13c0) + Fl(0x5ca058) - Fl(0x5c8d4c)) / Fl(0x5ca198));
        float ry2 = (float)((double)ry + Fl(0x5c8900));
        F<float>(def, 0xb) = (float)((hd - D(0x5c9b80)) / D(0x5c85d0) + (double)ry2 / Fl(0x5ca164));
        char img[0x200];
        FormatText(img, (const char*)0x5d01bc, id ? (const char*)Collection_Get(cargo(), 0, id) + 4 : (const char*)0x5d05d4, 0, 0, 0, 0, 0, 0, 0);
        String_Assign(&F<char*>(def, 0x43), NameTable_ContainsName(g_pack, 0, img, 0, 0) ? img : (const char*)0x5d0194);
        uint8_t* s = scrollbar(def);
        Window_AddChild(w, 0, s, 0);
        Free(F<void*>(def, 0x43));
        memset(def, 0, sizeof def);
        float padF = (float)(col ? 7 : 0);
        F<float>(def, 7) = (float)(((double)cx + Fl(0x5ca058) - padF) / Fl(0x5ca198));
        F<float>(def, 0xb) = (float)(((double)ry2 - Fl(0x5c8900)) / Fl(0x5ca164));
        float wpad = (float)(col ? 4 : 0);
        F<float>(def, 0xf) = (float)(((double)wpad + Fl(0x5d13c0)) / D(0x5ca2c8));
        float hh = Fl(0x5c8954) > rowH ? 33.0f : rowH;
        F<float>(def, 0x13) = (float)((double)hh / D(0x5ca2d0));
        def[0x33] = 1;
        F<uint16_t>(def, 0) = (uint16_t)(id + 0x91b4);
        def[0x29] = 0x20;
        F<int32_t>(def, 0x61) = 2;
        char tip[0x200], num1[0x40], num2[0x40], more[0x200];
        if (F<int32_t>(cargo(), 0x9a + id * 4) != -1) {
            float demand = (float)Cargo_DemandWeight(Collection_Get(cargo(), 0, id));
            float supply = F<float>(Collection_Get(cargo(), 0, id), 0x5a);
            float diff = (float)((double)supply - demand);
            float extra = Fl(0x5c8598) > diff ? 0.0f : diff;
            Num_Format(num2, 1, (double)extra, 10, 1, 0, 0);
            Num_Format(num1, 1, (double)demand, 10, 1, 0, 0);
            FormatText(tip, GetString(0xbf6), num1, name, num2, 0, 0, 0, 0, 0);
        } else {
            if (!id) FormatText(tip, name, 0, 0, 0, 0, 0, 0, 0, 0);
            else {
                float supply = F<float>(Collection_Get(cargo(), 0, id), 0x5a);
                Num_Format(num1, 1, (double)supply, 10, 1, 0, 0);
                FormatText(tip, GetString(0xed), num1, name, 0, 0, 0, 0, 0, 0);
            }
            FormatText(more, GetString(0xee), name, name, 0, 0, 0, 0, 0, 0);
            strcat(tip, more);
        }
        String_Assign(&F<char*>(def, 0x3f), tip);
        s = scrollbar(def);
        Window_AddChild(w, 0, s, 0);
        Free(F<void*>(def, 0x3f));
        pick(s, id);
        Window_SendEvent5(w, 0, 0x7b, id + 0x91b4, 0x4f3c60, 0, 0);
    }
}
RT3_REPLACE(0x4f3d90, PickerWindow_Build);

// 0x4f4970. Switches the track picker window w to `tab` with `value` (-1:
// the tab's last value, 0x6d12dc[tab]): rebuilds it, shows overview mode 3
// for the value (tab 0) or 7, recolours the buildings, city chains and
// tracks, and applies the tab's colouring (1 slope, with the camera hold
// while it lasts; 2 wear; 3 a company's boom / trains / track group; 4 profit).
RT3_NOINLINE void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value) {
    if (value == -1) value = ((int32_t*)0x6d12dc)[tab];
    int32_t old = F<int32_t>(w, 0x78);
    if (tab == old && value == F<int32_t>(w, 0x7c)) return;
    F<int32_t>(w, 0x78) = tab;
    g_6d12d8 = tab;
    F<int32_t>(w, 0x7c) = value;
    ((int32_t*)0x6d12dc)[F<int32_t>(w, 0x78)] = value;
    PickerWindow_Build(w);
    if (!F<int32_t>(w, 0x78)) Overview_SetMode(g_map, 0, 3, F<int32_t>(w, 0x7c), 0, 0, nullptr, 0, 0, 99999, 99999);
    else Overview_SetMode(g_map, 0, 7, 0, 0, 0, nullptr, 0, 0, 99999, 99999);
    Table_RefreshAll(g_buildings);
    Table_ForAllCityChains(g_trains);
    Tracks_UpdateColor(g_tracks);
    if (F<int32_t>(w, 0x78) == 1) {
        Tracks_ColorSlope(g_tracks);
        if (old != F<int32_t>(w, 0x78)) Camera_BeginA0(hook::As<uint8_t*>(g_tracks));
    } else if (old == 1)
        Obj_DecA0(hook::As<void*>(g_tracks));
    if (F<int32_t>(w, 0x78) == 2) {
        Tracks_ColorWear(g_tracks);
        g_6d12f4 = F<uint32_t>(g_world, 0x15);
    }
    if (F<int32_t>(w, 0x78) == 4) {
        Buildings_ColorProfit(g_buildings);
        Trains_UpdateRatioColors(g_trains);
    }
    if (F<int32_t>(w, 0x78) == 3) {
        Cities_ColorBooms(g_buildings, 0, F<int32_t>(w, 0x7c));
        Trains_ColorOwner(g_trains, 0, F<int32_t>(w, 0x7c));
        Objects_HighlightGroup(g_tracks, 0, F<int32_t>(w, 0x7c));
    }
    g_6d12d4 = -1;
}
RT3_REPLACE(0x4f4970, TrackPicker_SetTab);

// 0x4f4ac0. Track picker window w (0x6d12f0): event handler. Idle (0): in
// the tutorial blinks the three cargo buttons of both lists (0x6d12f8), shows
// the hovered entry 0x6d12d4 (as the tab's value when 0x6d12d0, else as the
// overview / company colouring) and on the wear tab recolours every 0x5a0
// minutes. Hover end (4, on panels 0x10 / 0x11) restores the shown value;
// hover (3) highlights the entry under the cursor. Changes (0xca): entries of
// the lists 0x90ec / 0x91b4 / 0x93a8 and the tabs 0x9089..0x908d (only the
// first one in the tutorial: 0xe8c). Returns 1 for unhandled and idle events.
uint32_t __fastcall TrackPicker_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint32_t type = ev[0];
    if (type == 0) {
        if (g_6d3b4c) {
            int32_t blink = Tutorial_Blink37a();
            if (blink != g_6d12f8) {
                g_6d12f8 = blink;
                uint32_t show = blink ? 0x87 : 0x65;
                for (uint32_t base = 0x90ec; base <= 0x927c; base += 0x190)
                    for (int32_t k = 0; k < 3; ++k)
                        Window_SendEvent5(w, 0, show, F<uint16_t>(hook::As<uint8_t*>(g_cargoTypes), 0x88 + k * 2) + base, 1, 0, 0);
            }
        }
        int32_t hover = g_6d12d4;
        if (hover != -1) {
            if (g_6d12d0) {
                TrackPicker_SetTab(w, 0, F<int32_t>(g_6d12f0, 0x78), hover);
            } else {
                uint8_t* map = g_map;
                if (F<int32_t>(map, 0x2179) != hover) {
                    if (F<int32_t>(w, 0x78) != 3) Overview_SetMode(map, 0, 3, hover, 0, 0, nullptr, 0, 0, 99999, 99999);
                    else TrackPicker_ColorCompany(g_6d12d4);
                }
            }
            g_6d12d4 = -1;
        }
        if (F<int32_t>(w, 0x78) == 2) {
            uint32_t now = F<uint32_t>(g_world, 0x15);
            int32_t d = (int32_t)(now - g_6d12f4);
            if ((d < 0 ? -d : d) > 0x5a0) {
                g_6d12f4 = now;
                Tracks_ColorWear(g_tracks);
            }
        }
        return 1;
    }
    if (type == 3 || type == 4) {
        if (ev[2] != 0x10 && ev[2] != 0x11) return 1;
        if (type == 4) {
            int32_t cur = F<int32_t>(g_map, 0x2179);
            if (cur == F<int32_t>(w, 0x7c)) return 0;
            if (!F<int32_t>(w, 0x78)) {
                Window_SendEvent5(g_6d12f0, 0, 0xa7, (uint32_t)cur + 0x90ec, 0xffffffff, 0, 0);
                Overview_SetMode(g_map, 0, 3, F<int32_t>(w, 0x7c), 0, 0, nullptr, 0, 0, 99999, 99999);
            } else if (F<int32_t>(w, 0x78) == 3) {
                Window_SendEvent5(g_6d12f0, 0, 0xa7, (uint32_t)cur + 0x9376, 0xffffffff, 0, 0);
                TrackPicker_ColorCompany(F<int32_t>(w, 0x7c));
            }
            return 0;
        }
        if (uint8_t* h = F<uint8_t*>(g_windowManager, 0xc6d)) {
            uint16_t id = F<uint16_t>(h, 4);
            if (id >= 0x91b4 && id <= 0x927b) Picker_Hover90ec(id - 0x91b4, 1);
            else if (id >= 0x93a8 && id <= 0x93d9) Picker_Hover9376(id - 0x93a8, 1);
        }
        return 0;
    }
    if (type != 0xca) return 1;
    int32_t id = (int32_t)ev[1];
    if (id >= 0x90ec && id <= 0x91b3) TrackPicker_SetTab(w, 0, F<int32_t>(w, 0x78), id - 0x90ec);
    else if (id >= 0x91b4 && id <= 0x927b) TrackPicker_SetTab(w, 0, F<int32_t>(w, 0x78), id - 0x91b4);
    else if (id >= 0x93a8 && id <= 0x93d9) TrackPicker_SetTab(w, 0, F<int32_t>(w, 0x78), id - 0x93a8);
    else if (id >= 0x9089 && id <= 0x908d) {
        if (g_6d3b4c && id > 0x9089)
            Dialog_RunMessage(GetString(0xe8c), 1, 0x3ef5c28f, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        else TrackPicker_SetTab(w, 0, id - 0x9089, -1);
    } else return 1;
    return 0;
}
RT3_REPLACE(0x4f4ac0, TrackPicker_HandleEvent);

// 0x4f5020. Track picker window constructor (0x6d12f0): layout 0x5d13e8 at
// (0.405, 0.707), no tab yet, then the last used tab (0x6d12d8).
uint8_t* __fastcall TrackPicker_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d13b4;
    g_6d12f0 = w;
    F<int32_t>(w, 0x78) = -1;
    F<int32_t>(w, 0x7c) = -1;
    g_6d12d4 = -1;
    Window_Load(w, 0, Prod_Bits(0x3ecf5c29), Prod_Bits(0x3f34e81b), (const char*)0x5d13e8);
    TrackPicker_SetTab(w, 0, g_6d12d8, -1);
    return w;
}
RT3_REPLACE(0x4f5020, TrackPicker_Construct);

RT3_BRUSH_INT(0x4f5080, Brush_SetSizeI6226e8, 0x6226e8)

// 0x4f50d0. Destructor of the overview window for mode 0x10.
void __fastcall MapWindow10_Destruct(uint8_t* w) { MapModeWindow_Destruct(w, 0x5d13f8, 0x10, 0); }
RT3_REPLACE(0x4f50d0, MapWindow10_Destruct);

// 0x4f5110. The city editor page: highlights +0x78 among 0x4651.. (7) and +0x80
// among 0x4664.. (1..24), reassigns the city blocks and shows each of the 24 cities
// (0x62bae0): name +0x356 on 0x46c8.., white when it has a size (+0x242, else grey)
// and size / type name (+0x272, table 0x620ce8) / +0x25a through string 0xe6 on
// 0x4664...
RT3_NOINLINE void __fastcall CityEditor_Show(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0x4651 + i, 1, 0, 0);
    for (int32_t i = 1; i <= 0x18; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x80) ? 0x87 : 0x65, 0x4663 + i, 1, 0, 0);
    Cities_AssignBlocks(g_cities);
    for (uint32_t id = 0x46c8; id < 0x46c8 + 0x18; ++id) {
        uint8_t* c = (uint8_t*)Collection_Get(g_cities, 0, id - 0x46c7);
        Window_SendEvent5(w, 0, 0x6f, id, (uint32_t)(uintptr_t)(c + 0x356), 0, 0);
        Window_SendEvent5(w, 0, 0xa7, id, F<int32_t>(c, 0x242) ? 0xffffffffu : 0xffafafafu, 0, 0);
        char a[0x20], b[0x20], text[0x160];
        char* sb = Num_Format(b, 1, (double)F<int32_t>(c, 0x242), 10, 1, 0, 0);
        const char* type = GetString(((const int32_t*)0x620ce8)[F<int32_t>(c, 0x272)]);
        char* sa = Num_Format(a, 1, (double)Ftol(F<float>(c, 0x25a)), 10, 1, 0, 0);
        FormatText(text, GetString(0xe6), sa, type, sb, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6e, id - 0x64, (uint32_t)(uintptr_t)text, 0, 0);
    }
}
RT3_REPLACE(0x4f5110, CityEditor_Show);

// 0x4f52c0 (event handler). The region brush of the map editor d (size +0x78
// from 0x6226e8, region +0x80, painting +0x7c): the region list 0x4664..
// picks the region (0x6226e0) and shows the region overlay (0x10), the
// sizes 0x4651..0x4657 set the brush (0x6226e4), the city buttons
// 0x472c.. open the city editor; pressing on the map (without Ctrl, flag
// 0x800) starts painting, mouse moves paint (Map_MarkBlocks0), the release
// stops.
int32_t __fastcall RegionBrush_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    auto overlay = [&]() {
        uint8_t* map = g_map;
        if (F<int32_t>(map, 0x2175) == 0x10)
            Overview_Redraw(map, 0, d + 0x80);
        else
            Overview_SetMode(map, 0, 0x10, 0, 0, 0, d + 0x80, 0, 0, 0x1869f, 0x1869f);
    };
    const uint32_t id = msg[1];
    switch (msg[0]) {
    case 0xcb:
        if ((int32_t)id >= 0x472c && (int32_t)id < 0x4744) {
            EditorDialog_Run(1, (int32_t)id - 0x472b);
            CityEditor_Show(d);
            return 0;
        }
        if (id == 0x7d6 && F<int32_t>(d, 0x7c)) {
            F<int32_t>(d, 0x7c) = 0;
            return 0;
        }
        return 1;
    case 0xca: {
        if ((int32_t)id >= 0x4664 && (int32_t)id < 0x467c) {
            const int32_t region = (int32_t)id - 0x4663;
            F<int32_t>(d, 0x80) = region;
            g_6226e0 = region;
            overlay();
            CityEditor_Show(d);
            return 0;
        }
        if (id == 0x7d6) {
            float x, y;
            if (!Scene_FocusFlagIs1(&x, &y) || (msg[8] & 0x800)) return 0;
            F<int32_t>(d, 0x7c) = 1;
            break;
        }
        if ((int32_t)id > 0x4650 && (int32_t)id <= 0x4657) {
            const int32_t size = (int32_t)id - 0x4651;
            Brush_SetSizeI6226e8(d, 0, size);
            g_6226e4 = size;
            CityEditor_Show(d);
            return 0;
        }
        return 1;
    }
    case 2:
        break;
    default:
        return 1;
    }
    if (!F<int32_t>(d, 0x7c)) return 1;
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    const float r = (float)((const int32_t*)0x6226e8)[F<int32_t>(d, 0x78)];
    Map_MarkBlocks0(g_map, 0, x, y, r, (uint8_t)F<int32_t>(d, 0x80));
    overlay();
    return 1;
}
RT3_REPLACE(0x4f52c0, RegionBrush_HandleEvent);

// 0x4f54d0. City editor tool window constructor: brush 0x6226e4 into the
// sizes 0x6226e8, setting 0x6226e0 into +0x80, layout 0x5d1404; shows overview
// mode 0x10 over that setting.
uint8_t* __fastcall CityToolWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0x80) = hook::As<uint32_t>(g_6226e0);
    EditorToolWindow_Open(w, 0x5d13f8, 0x6226e4, 0x6226e8);
    F<uint32_t>(w, 0x7c) = 0;
    Window_Load(w, 0, Prod_Bits(0x3ecf5c29), Prod_Bits(0x3f34e81b), (const char*)0x5d1404);
    CityEditor_Show(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(g_engine));
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2175) == 0x10) Overview_Redraw(map, 0, w + 0x80);
    else Overview_SetMode(map, 0, 0x10, 0, 0, 0, w + 0x80, 0, 0, 99999, 99999);
    return w;
}
RT3_REPLACE(0x4f54d0, CityToolWindow_Construct);

RT3_BRUSH_INT(0x4f55b0, Brush_SetSizeI62270c, 0x62270c)

// 0x4f5600. Highlights selection +0x78 among buttons 0x1195.. (7) and +0x7c among
// 0x119e.. (5).
RT3_NOINLINE void __fastcall Window_HighlightTabs7And5(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0x1195 + i, 1, 0, 0);
    for (int32_t i = 0; i < 5; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x7c) ? 0x87 : 0x65, 0x119e + i, 1, 0, 0);
}
RT3_REPLACE(0x4f5600, Window_HighlightTabs7And5);

// 0x4f5670. Region tool window constructor (0x6d12fc): brush 0x622704 into
// the sizes 0x62270c, region 0x622708 at +0x7c, layout 0x5d1414; shows
// overview mode 0x11 for that region.
uint8_t* __fastcall RegionToolWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    EditorToolWindow_Open(w, 0x5d1424, 0x622704, 0x62270c);
    F<uint32_t>(w, 0x80) = 0;
    Window_Load(w, 0, Prod_Bits(0x3ecf5c29), Prod_Bits(0x3f34e81b), (const char*)0x5d1414);
    F<uint32_t>(w, 0x7c) = g_622708;
    g_6d12fc = w;
    Window_HighlightTabs7And5(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(g_engine));
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2175) == 0x11) Overview_Redraw(map, 0, nullptr);
    else Overview_SetMode(map, 0, 0x11, F<int32_t>(w, 0x7c), 0, 0, nullptr, 0, 0, 99999, 99999);
    return w;
}
RT3_REPLACE(0x4f5670, RegionToolWindow_Construct);

// 0x4f5750. Destructor of the overview window for mode 0x11 (forgets 0x6cfe18).
void __fastcall MapWindow11_Destruct(uint8_t* w) { MapModeWindow_Destruct(w, 0x5d1424, 0x11, 0x6cfe18); }
RT3_REPLACE(0x4f5750, MapWindow11_Destruct);

// 0x4f57a0 (event handler). The terrain type brush of the map editor d
// (size +0x78 from 0x62270c, type +0x7c, painting +0x80): the types
// 0x119e..0x11a2 pick one (0x622708) and show the terrain overlay (0x11),
// the sizes 0x1195..0x119b set the brush (0x622704); pressing on the map
// (without Ctrl) starts painting, moves paint (Map_MarkBlocks3), the release
// stops.
int32_t __fastcall TerrainBrush_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    const uint32_t id = msg[1];
    switch (msg[0]) {
    case 0xcb:
        if (id == 0x7d6 && F<int32_t>(d, 0x80)) {
            F<int32_t>(d, 0x80) = 0;
            return 0;
        }
        return 1;
    case 0xca:
        if ((int32_t)id >= 0x119e && (int32_t)id <= 0x11a2) {
            const int32_t type = (int32_t)id - 0x119e;
            F<int32_t>(d, 0x7c) = type;
            hook::As<int32_t>(g_622708) = type;
            Overview_SetMode(g_map, 0, 0x11, type, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
            Window_HighlightTabs7And5(d);
            return 0;
        }
        if ((int32_t)id >= 0x1195 && (int32_t)id <= 0x119b) {
            const int32_t size = (int32_t)id - 0x1195;
            Brush_SetSizeI62270c(d, 0, size);
            g_622704 = size;
            Window_HighlightTabs7And5(d);
            return 0;
        }
        if (id == 0x7d6) {
            float x, y;
            if (!Scene_FocusFlagIs1(&x, &y) || (msg[8] & 0x800)) return 0;
            F<int32_t>(d, 0x80) = 1;
            break;
        }
        return 1;
    case 2:
        break;
    default:
        return 1;
    }
    if (!F<int32_t>(d, 0x80)) return 1;
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    const float r = (float)((const int32_t*)0x62270c)[F<int32_t>(d, 0x78)];
    Map_MarkBlocks3(g_map, 0, x, y, r, (uint8_t)F<int32_t>(d, 0x7c));
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2175) == 0x11)
        Overview_Redraw(map, 0, nullptr);
    else
        Overview_SetMode(map, 0, 0x11, F<int32_t>(d, 0x7c), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
    return 1;
}
RT3_REPLACE(0x4f57a0, TerrainBrush_HandleEvent);

RT3_PAINT_SLIDER(0x4f5960, Paint_WeightSliderCb, 0x2922, 0x622728, 0x292c)

RT3_PAINT_SLIDER(0x4f59f0, Paint_ParamSliderCb, 0x2936, 0x622738, 0x2940)

// 0x4f5a80. Pushes an undo snapshot (up to 10, +0x1e4 = newest) of the editor's
// image (+0x114, +0xfc x +0x100 pixels) and the map's 4 terrain layers.
RT3_NOINLINE void __fastcall Editor_PushUndo(uint8_t* o) {
    if (F<int32_t>(o, 0x1e4) < 9) {
        int32_t n = ++F<int32_t>(o, 0x1e4);
        memcpy(F<void*>(o, 0x11c + n * 4), F<void*>(o, 0x114), F<int32_t>(o, 0x100) * F<int32_t>(o, 0xfc) * 4);
        for (int k = 0; k < 4; ++k) {
            int32_t sz = Map_PointCount(hook::As<void*>(g_map));
            const void* src = (const void*)Terrain_Layer(g_map, 0, (uint8_t)k);
            memcpy(F<void*>(o, 0x144 + (k + F<int32_t>(o, 0x1e4) * 4) * 4), src, sz);
        }
        return;
    }
    for (int j = 0; j < 9; ++j) {
        memcpy(F<void*>(o, 0x11c + j * 4), F<void*>(o, 0x120 + j * 4), F<int32_t>(o, 0xfc) * F<int32_t>(o, 0x100) * 4);
        for (int k = 0; k < 4; ++k) {
            int32_t sz = Map_PointCount(hook::As<void*>(g_map));
            memcpy(F<void*>(o, 0x144 + (j * 4 + k) * 4), F<void*>(o, 0x154 + (j * 4 + k) * 4), sz);
        }
    }
    memcpy(F<void*>(o, 0x140), F<void*>(o, 0x114), F<int32_t>(o, 0xfc) * F<int32_t>(o, 0x100) * 4);
    for (int k = 0; k < 4; ++k) {
        int32_t sz = Map_PointCount(hook::As<void*>(g_map));
        const void* src = (const void*)Terrain_Layer(g_map, 0, (uint8_t)k);
        memcpy(F<void*>(o, 0x1d4 + k * 4), src, sz);
    }
}
RT3_REPLACE(0x4f5a80, Editor_PushUndo);

// 0x4f5c30. Undoes the last map texture paint of editor e (stack +0x1e4):
// restores the saved texture area (+0x11c[n] into +0x114, +0xfc x +0x100
// texels), pastes it back, and restores the four terrain layers
// (+0x144[n][k]).
RT3_NOINLINE void __fastcall Editor_UndoPaint(uint8_t* e) {
    if (!F<int32_t>(e, 0x1e4)) return;
    int32_t n = --F<int32_t>(e, 0x1e4);
    memcpy(F<void*>(e, 0x114), F<void*>(e, 0x11c + n * 4), (size_t)(F<int32_t>(e, 0xfc) * F<int32_t>(e, 0x100) * 4));
    Terrain_PasteTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0, 0, F<int32_t>(e, 0xfc) + 1, F<int32_t>(e, 0x100) + 1);
    for (int32_t k = 0; k < 4; ++k) {
        int32_t size = Map_PointCount(hook::As<void*>(g_map));
        const void* src = F<void*>(e, 0x144 + (k + F<int32_t>(e, 0x1e4) * 4) * 4);
        void* dst = (void*)(uintptr_t)Terrain_Layer(g_map, 0, (uint8_t)k);
        memcpy(dst, src, (size_t)size);
    }
}
RT3_REPLACE(0x4f5c30, Editor_UndoPaint);

// 0x4f5cf0. Sets the water preset idx (+0x98) from the table for the map style
// (+0x8c / +0x90) and passes the level (+0.5) to the renderer.
RT3_NOINLINE void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx) {
    F<int32_t>(o, 0x98) = idx;
    const double half = *(const double*)0x5c85b8;
    float v;
    if (F<int32_t>(o, 0x8c) == 1) {
        v = (float)((double)*(const float*)0x6227b4 + half);
    } else {
        int32_t style = F<int32_t>(o, 0x90);
        uint32_t table;
        if (style == 4) table = 0x6227e4;
        else if (style == 6) table = 0x622800;
        else if (style == 7) table = 0x62281c;
        else if (style == 3 || style == 2) table = 0x6227c8;
        else table = 0x6227ac;
        v = (float)((double)((const float*)table)[idx] + half);
    }
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, v);
}
RT3_REPLACE(0x4f5cf0, Map_SetWaterPreset);

// 0x4f5dd0. Sets the map style (+0x8c) and reapplies the water preset.
RT3_NOINLINE void __fastcall Map_SetStyle(uint8_t* o, int, int32_t style) {
    F<int32_t>(o, 0x8c) = style;
    Map_SetWaterPreset(o, 0, F<int32_t>(o, 0x98));
}
RT3_REPLACE(0x4f5dd0, Map_SetStyle);

// 0x4f5df0. Makes window w's three percentages (+0xdc / +0xe4 / +0xec, labels at
// -4) add up to 100 (the second clamped to what the first leaves, the third to the
// rest), shows them on sliders 0x2a32.. (event 0x6a, guard 0x6d1330 set meanwhile)
// and copies the pairs to table 0x622770.
RT3_NOINLINE void __fastcall Window_BalancePercentages(uint8_t* w) {
    int32_t a = F<int32_t>(w, 0xdc);
    if (F<int32_t>(w, 0xe4) > 100 - a) F<int32_t>(w, 0xe4) = 100 - a;
    int32_t b = F<int32_t>(w, 0xe4);
    if (F<int32_t>(w, 0xec) > 100 - b - a) F<int32_t>(w, 0xec) = 100 - b - a;
    if (b + a != 100) F<int32_t>(w, 0xec) = 100 - (b + a);
    g_6d1330 = 1;
    for (int32_t i = 0; i < 3; ++i) {
        Window_SendEvent5(w, 0, 0x6a, 0x2a32 + i, F<uint32_t>(w, 0xdc + i * 8), 0, 0);
        ((uint32_t*)0x622770)[i * 2] = F<uint32_t>(w, 0xd8 + i * 8);
        ((uint32_t*)0x622770)[i * 2 + 1] = F<uint32_t>(w, 0xdc + i * 8);
    }
    g_6d1330 = 0;
}
RT3_REPLACE(0x4f5df0, Window_BalancePercentages);

RT3_NOINLINE int32_t __fastcall Screen_IsMode5_2(uint8_t* s) { return F<int32_t>(s, 0x90) == 5 && F<int32_t>(s, 0x94) == 2 ? 1 : 0; }
RT3_REPLACE(0x4f5ea0, Screen_IsMode5_2);

// 0x4f5ec0 (vtable). Map editor tool 4 in mode 2.
RT3_NOINLINE int32_t __fastcall MapEditor_IsTool4Mode2(uint8_t* e) { return F<int32_t>(e, 0x90) == 4 && F<int32_t>(e, 0xb4) == 2; }
RT3_REPLACE(0x4f5ec0, MapEditor_IsTool4Mode2);

// 0x4f5ee0. Map editor window destructor (0x6d1304): leaves its overview
// modes, drops the scene busy mark, releases its image (+0x1f8), commits the
// terrain texture, frees its brush tables (+0x118, the 10 names +0x11c with
// 4 entries each at +0x144, +0xf4, +0xf8), reassigns city blocks, rebuilds
// the water mask, frees the shared buffers 0x6d132c / 0x6d1338 and tells the
// view (event 0xba / 0x808).
void __fastcall MapEditorWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1430;
    hook::As<uint32_t>(g_6d1304) = 0;
    int32_t mode = F<int32_t>(g_map, 0x2175);
    int32_t tab = F<int32_t>(w, 0x90);
    if (mode == 0x17 || (mode >= 0x1f && mode <= 0x23) || (tab >= 4 && tab <= 7)) Overview_Close(g_map);
    Scene_PopBusy((uint8_t*)Engine_Renderer(g_engine));
    Pack_Release(g_pack, 0, F<void*>(w, 0x1f8));
    Terrain_CommitTexture((uint8_t*)Engine_Get1C(g_engine), 0, 0);
    Free(F<void*>(w, 0x118));
    for (int32_t i = 0; i < 10; ++i) {
        Free(F<void*>(w, 0x11c + i * 4));
        for (int32_t j = 0; j < 4; ++j) Free(F<void*>(w, 0x144 + (i * 4 + j) * 4));
    }
    if (F<void*>(w, 0xf4)) Free(F<void*>(w, 0xf4));
    if (F<void*>(w, 0xf8)) Free(F<void*>(w, 0xf8));
    Cities_AssignBlocks(g_cities);
    Terrain_RebuildWaterMask(g_map);
    if (g_6d132c) Free(g_6d132c);
    if (hook::As<void*>(g_6d1338)) Free(hook::As<void*>(g_6d1338));
    hook::As<uint32_t>(g_6d1338) = 0;
    hook::As<uint32_t>(g_6d1334) = 0;
    if (g_gameWindow) Window_SendEvent5(g_gameWindow, 0, 0xba, 0x808, 0, 0, 0);
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4f5ee0, MapEditorWindow_Destruct);

}  // namespace rt3
