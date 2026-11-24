// OptionsDialog.cpp - RT3.exe 0x4fc000-0x502000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "game/collection.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

uint32_t __fastcall Obj_Get18(uint8_t* o);
int32_t __fastcall Obj_Zero18And14(uint8_t* o);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Ambient_StopSlots();
void __fastcall Ambient_Start(void*);
void ResetGlobals4686d0();
void* __fastcall Engine_Renderer(void* e);
void __fastcall Channels_PlayAll(uint8_t* o);
void __fastcall Territories_Recount(Collection* c);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Collection_Destruct(Collection* c);
int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i);
int32_t __fastcall Sound_SelectProvider(uint8_t* snd, int, int32_t n);
int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s);
uint32_t __fastcall Sound_ProviderFlagsAt(uint8_t* s, int, int32_t n);
void __fastcall Game_RestartSounds();
void __fastcall Engine_SaveConfig(uint8_t* e);
int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
int32_t __fastcall Sound_GetSpeakers(uint8_t* s);
int32_t __fastcall Engine_ModeCount(void* e);
void __fastcall Engine_ModeAt(void* e, int, int32_t i, int32_t* w, int32_t* h, int32_t* bpp);
uint32_t __fastcall Engine_IsFullscreen(void* e);
int32_t __fastcall Engine_Get1146ae(void* e);
int32_t __fastcall Engine_Get11470e(void* e);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetVtbl(uint8_t* w);
int32_t __fastcall Obj_Idlec60(uint8_t* o);
void __fastcall Obj_Decc60(uint8_t* o);
double One();
uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Widget_IsVisible(uint8_t* w);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t ClockMs();
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall Obj_DecA0(void* o);
void __fastcall Scene_PopBusy(uint8_t* s);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
void __fastcall TerritoryDialog_Show(uint8_t* w);
void __fastcall Scene_PushBusy(uint8_t* s);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Camera_BeginA0(uint8_t* o);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
void __fastcall Obj_Incc60(uint8_t* o);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
int32_t __fastcall Engine_ModeHasFlags(void* e);
void __fastcall Lighting_Rebuild(uint8_t* opts);
void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);
uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask);
uint8_t* Models_NextInOrder();
void __fastcall Engine_SetCursorObject(void* e, int, uint32_t v);
void __fastcall OptionsDialog_ShowGeneral(uint8_t* w);
void __fastcall OptionsDialog_ShowSound(uint8_t* w);
void __fastcall OptionsDialog_ShowVideo(uint8_t* w);
void __fastcall OptionsDialog_ShowDisplay(uint8_t* w);
void __fastcall OptionsDialog_FillList755f(uint8_t* w);
void __fastcall OptionsDialog_FillList7569(uint8_t* w);
void __fastcall OptionsDialog_ShowEngine(uint8_t* w);
void __fastcall OptionsDialog_ShowValues(uint8_t* w);
void __fastcall OptionsDialog_ShowLists7587(uint8_t* w);
void __fastcall OptionsDialog_ShowSwitches(uint8_t* w);
void __fastcall OptionsDialog_ShowGraphics(uint8_t* w);
void __fastcall Window_FillList759e(uint8_t* w);
void __fastcall Window_SelectOption755f(uint8_t* w);
void __fastcall Window_SelectOption7569(uint8_t* w);
void __fastcall SetupWindow_FillLabels(uint8_t* w);
uint8_t* __fastcall SettingsWindow_Construct(uint8_t* w);
void __fastcall SettingsWindow_Destruct(uint8_t* w);
void __fastcall Caps_Load(uint8_t* o);
void __fastcall Terrain_RefreshRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall Cursor_UseSystem(uint8_t* c);
void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1,
                                            int32_t list2, int32_t list3);

uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void OptionsDialog_Open();
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall Grid_MarkCell(uint8_t* g, int, int32_t x, int32_t y);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE void __fastcall Lighting_Rebuild(uint8_t* opts);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall OptionsDialog_ShowPage(uint8_t* w, int, int32_t page);

#define RT3_OPTION(addr, name, off) \
    void __fastcall name(int32_t v) { \
        if (v == -1 || g_6d135d) return; \
        F<int32_t>(g_app, off) = v; \
        Options_Save(g_app, 0, 1); \
    } \
    RT3_REPLACE(addr, name);

void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Overview_EndPlain(uint8_t* t);
void __fastcall Options_SetDetail(uint8_t* o, int, int32_t which, int32_t level, int32_t rebuild);
void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);

#define RT3_BRUSH_INT(addr, name, table) \
    void __fastcall name(uint8_t* o, int, int32_t i) { \
        F<int32_t>(o, 0x78) = i; \
        const float r = (float)((double)((const int32_t*)table)[i] + D(0x5c8628)); \
        Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, r); \
    } \
    RT3_REPLACE(addr, name);

void __fastcall OptionsDialog_ShowSliders2(uint8_t* w);
void __fastcall OptionsDialog_ShowSliders(uint8_t* w);
void __fastcall Cursor_UseGameDrawn(uint8_t* c);
void __fastcall Cursor_UseWindows(uint8_t* c);

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

void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
int32_t __fastcall Hotkeys_Save(const char* path);
void __fastcall OptionsDialog_FillModels(uint8_t* w, int, int32_t);
uint32_t __fastcall RepackFlags(uint32_t f);
void Models_AddCopy();
int32_t Options_ToggleFullscreen();
void __fastcall Trains_ReapplyGhosts(Collection* c);
void __fastcall OptionsDialog_SetLightingSet(uint8_t* d, int, uint8_t on, uint8_t apply);
uint8_t* __fastcall Window_EnableControl7f9(uint8_t* w, int, uint8_t on);
uint32_t __fastcall Obj_Busy18(uint8_t* o);
void __fastcall OptionsDialog_FillResolutions(uint8_t* w);
void __fastcall Engine_SetHardwareOption(uint8_t* e, int, int32_t on);
void __fastcall Engine_SetDisplayOption(uint8_t* e, int, int32_t v);
int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name);
void __fastcall Cursor_Reapply(uint8_t* c);
void __fastcall Map_ClearCellFlag10(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Map_MarkTiles(uint8_t* m, int, float x, float y, float r, uint8_t v);
void __fastcall Brush_SetSizeI622854(uint8_t* o, int, int32_t i);
void __fastcall MapBrush_PaintFlag10(uint8_t* d, int, float x, float y);
void __fastcall MapBrush_PaintBit3(uint8_t* b, int, float x, float y);
uint8_t __fastcall Grid_CellLow(uint8_t* g, int, int32_t x, int32_t y);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root);
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def);
uint32_t __fastcall HandleObj_Destruct(void* o);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Map_ClearCellFlag10(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall Overview_EndPlain(uint8_t* t);
RT3_NOINLINE void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE int32_t Options_ToggleFullscreen();
RT3_NOINLINE void __fastcall Options_SetDetail(uint8_t* o, int, int32_t which, int32_t level, int32_t rebuild);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
RT3_NOINLINE void __fastcall MapBrush_PaintFlag10(uint8_t* d, int, float x, float y);
RT3_NOINLINE void __fastcall MapBrush_PaintBit3(uint8_t* b, int, float x, float y);
RT3_NOINLINE void __fastcall MapEditor_Stroke(uint8_t* d, int, float x, float y, int32_t first, int32_t erase);
void __fastcall MapWindow1e_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall TerritoryDialog_Show(uint8_t* w);
int32_t __fastcall TerritoryBrush_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall TerritoryToolWindow_Construct(uint8_t* w);
void __fastcall OptionList_DrawCb(void*, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall Options_DisplayModeCb(int32_t v);
void __fastcall Options_SetSpeakersCb(void*, int32_t type);
void __fastcall Options_SetDetailACb(int32_t v);
void __fastcall Options_SetDetailBCb(int32_t v);
void __fastcall Options_SetEngine1146b3Cb(int32_t v);
void __fastcall Options_SetEngineMode(int32_t mode);
void __fastcall Options_SetLightingCb(int32_t v);
void __fastcall Options_SetEngine11474eCb(int32_t v);
RT3_NOINLINE void __fastcall OptionsDialog_ShowSliders2(uint8_t* w);
RT3_NOINLINE void __fastcall OptionsDialog_ShowSliders(uint8_t* w);
void __fastcall OptionsDialog_ShowGraphics(uint8_t* w);
void __fastcall Window_FillList759e(uint8_t* w);
void __fastcall OptionsDialog_ShowSwitches(uint8_t* w);
void __fastcall OptionsDialog_ShowGeneral(uint8_t* w);
void __fastcall OptionsDialog_ShowSound(uint8_t* w);
void __fastcall OptionsDialog_ShowEngine(uint8_t* w);
void __fastcall OptionsDialog_ShowValues(uint8_t* w);
void __fastcall OptionsDialog_ShowLists7587(uint8_t* w);
RT3_NOINLINE int32_t Global6d1350Set();
void SetGlobal6d1360();
RT3_NOINLINE uint32_t __fastcall Obj_Busy18(uint8_t* o);
RT3_NOINLINE void __fastcall Game_RestartSounds();
void __fastcall SoundDialog_PickProvider(int32_t idx);
void __fastcall Options_SetResolutionCb(int32_t v, int32_t res);
void __fastcall Options_SetDetail0Cb(int32_t v);
void __fastcall Options_SetDetail1Cb(int32_t v);
void __fastcall Options_SetDetail2Cb(int32_t v);
void __fastcall SettingsWindow_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall OptionsDialog_FillResolutions(uint8_t* w);
void __fastcall Window_SelectOption755f(uint8_t* w);
void __fastcall Window_SelectOption7569(uint8_t* w);
RT3_NOINLINE void __fastcall OptionsDialog_ShowVideo(uint8_t* w);
void __fastcall OptionsDialog_ShowDisplay(uint8_t* w);
void __fastcall OptionsDialog_FillList755f(uint8_t* w);
void __fastcall OptionsDialog_FillList7569(uint8_t* w);
RT3_NOINLINE void __fastcall OptionsDialog_FillModels(uint8_t* w, int, int32_t);
void __fastcall OptionsDialog_OnSlider(uint8_t* sl, int32_t v);
int32_t __fastcall OptionsDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall OptionsDialog_ShowPage(uint8_t* w, int, int32_t page);
void __fastcall Options_ShowPageCb(int32_t v);
uint8_t* __fastcall SettingsWindow_Construct(uint8_t* w);
RT3_NOINLINE void OptionsDialog_Open();
void Shell_OpenCommand();
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
RT3_NOINLINE void __fastcall Engine_SetHardwareOption(uint8_t* e, int, int32_t on);
RT3_NOINLINE void __fastcall Engine_SetDisplayOption(uint8_t* e, int, int32_t v);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

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

// 0x4fe6f0 / 0x4fe730. Select option list 0x755f / 0x7569 of window w (event 0x66)
// from the options' entry 1 / 2 (+0xac array) through table 0x622990 (-1: 0).
static void Window_SelectOption(uint8_t* w, int32_t entry, uint32_t id) {
    int32_t k = Obj_ArrayAc(hook::As<void*>(g_app), 0, entry);
    uint32_t v = k == -1 ? 0 : ((const uint32_t*)0x622990)[k];
    Window_SendEvent5(w, 0, 0x66, id, v, 0, 0);
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

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

static inline float BitsF(uint32_t b) { return *(float*)&b; }

namespace {
uint32_t Message478(uint32_t text, int32_t type) {
    return Dialog_RunMessage(GetString(text), type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

// 0x501f20 body. The command line (in 0x6d14d8) that opens target with the
// registered handler: kind 0 the web browser (HKCR http\shell\open\command; a
// Netscape command with a "%1" placeholder gets it filled in, any other has
// target appended), kind 1 the rtf viewer (rtffile, cut at its "%1, then
// appended). Null when no handler is registered.
static char* __cdecl Shell_OpenCommandImpl(int32_t kind, const char* target) {
    char* out = (char*)0x6d14d8;
    const char* key = kind == 0 ? (const char*)0x5d1634 : kind == 1 ? (const char*)0x5d1650 : nullptr;
    if (!key) return nullptr;
    uint32_t reg[3];
    RegKey_Construct(reg, 0, key, (void*)0x80000000);
    char* cmd = (char*)RegKey_GetString(reg, 0, (const char*)0x5d1648, (const char*)0x5c87a8, nullptr);
    if (!cmd) {
        HandleObj_Destruct(reg);
        return nullptr;
    }
    if (kind == 0 && StrFind(cmd, (const char*)0x5d162c, 0) && StrFind(cmd, (const char*)0x5ce074, 0)) {
        FormatText(out, cmd, target, 0, 0, 0, 0, 0, 0, 0);
    } else {
        if (kind == 1) {
            char* q = ((char*(__cdecl*)(const char*, const char*))0x5a1ff0)(cmd, (const char*)0x5d1644);
            if (q) *q = 0;
        }
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(out, (const char*)0x5ce184, cmd, target);
    }
    HandleObj_Destruct(reg);
    return out;
}

// 0x4fc280 (thiscall, map editor brush d). Over the cells within the brush
// radius (0x62281c by +0x98, truncated) of (x, y), clamped to the map, sets
// (+0xac) or clears flag 0x10 where it differs; then redraws that rectangle
// in the overview and refreshes the terrain 20 cells around it.
RT3_NOINLINE void __fastcall MapBrush_PaintFlag10(uint8_t* d, int, float x, float y) {
    const int32_t radius = (int32_t)((const float*)0x62281c)[F<int32_t>(d, 0x98)];
    const float r = (float)radius;
    const int32_t want = F<int32_t>(d, 0xac) == 1;
    bool changed = false;
    float x0 = x - r - BitsF(0x3f800000);
    if (x0 < 0.0f) x0 = 0.0f;
    const int32_t ix0 = (int32_t)x0;
    float y0 = y - r - BitsF(0x3f800000);
    if (y0 < 0.0f) y0 = 0.0f;
    const int32_t iy0 = (int32_t)y0;
    uint8_t* map = g_map;
    const float wmax = (float)(F<int32_t>(map, 0x2155) - 1);
    const float xr = r + x + BitsF(0x3f800000);
    const int32_t ix1 = (int32_t)(wmax < xr ? wmax : xr);
    const float hmax = (float)(F<int32_t>(map, 0x2159) - 1);
    const float yr = r + y + BitsF(0x3f800000);
    const int32_t iy1 = (int32_t)(hmax < yr ? hmax : yr);
    if (ix0 > ix1) return;
    for (int32_t cx = ix0; cx <= ix1; ++cx) {
        if (iy0 > iy1) continue;
        const int32_t py = (int32_t)y, px = (int32_t)x;
        for (int32_t cy = iy0; cy <= iy1; ++cy) {
            if ((int32_t)(uint16_t)DistanceInt(cx, cy, px, py) > radius) continue;
            if (Grid_CellMarked(g_map, 0, cx, cy) == want) continue;
            if (F<int32_t>(d, 0xac))
                Grid_MarkCell(g_map, 0, cx, cy);
            else
                Map_ClearCellFlag10(g_map, 0, cx, cy);
            changed = true;
        }
    }
    if (!changed) return;
    Overview_RedrawRect(g_map, 0, ix0, iy0, ix1, iy1);
    Terrain_RefreshRect(g_map, 0, ix0 - 0x14, iy0 - 0x14, ix1 + 0x14, iy1 + 0x14);
}
RT3_REPLACE(0x4fc280, MapBrush_PaintFlag10);

// 0x4fc4d0. Map brush (radius 0x622800[+0x98] cells) at (x, y) in world
// units (0x5c8568 per cell): sets fine-map bit 3 of every cell within the
// radius (cleared with +0xb0) that differs, then redraws the box when any
// changed.
RT3_NOINLINE void __fastcall MapBrush_PaintBit3(uint8_t* b, int, float x, float y) {
    const int32_t cx = Ftol((double)x * Fl(0x5c8568));
    const int32_t cy = Ftol((double)y * Fl(0x5c8568));
    const int32_t r = Ftol((double)*(const float*)(0x622800 + F<int32_t>(b, 0x98) * 4) * Fl(0x5c8568));
    const int32_t on = F<int32_t>(b, 0xb0) == 0 ? 1 : 0;
    bool changed = false;
    const int32_t x0 = cx - r - 1 < 0 ? 0 : cx - r - 1;
    const int32_t y0 = cy - r - 1 < 0 ? 0 : cy - r - 1;
    const int32_t w = F<int32_t>(g_map, 0x215d) - 1;
    const int32_t x1 = w < cx + r + 1 ? w : cx + r + 1;
    const int32_t h = F<int32_t>(g_map, 0x2161) - 1;
    const int32_t y1 = h < cy + r + 1 ? h : cy + r + 1;
    if (x0 > x1) return;
    for (int32_t i = x0; i <= x1; ++i)
        for (int32_t j = y0; j <= y1; ++j) {
            if ((int32_t)(uint16_t)DistanceInt(i, j, cx, cy) > r) continue;
            if (Map_CellFlag(g_map, 0, i, j, 3) == on) continue;
            Map_SetFineBit(g_map, 0, i, j, on, 3, 1);
            changed = true;
        }
    if (changed) Overview_RedrawRect(g_map, 0, x0, y0, x1, y1);
}
RT3_REPLACE(0x4fc4d0, MapBrush_PaintBit3);

// 0x4fc630 (thiscall). One stroke step of the map editor d's brushes from
// the last point (0x622844, 0x622848) to (x, y). A continued stroke ignores
// moves under 0.07 and jumps beyond 50 and ten brush radii. Except for tools
// 3 and 4 (+0x8c) the texture rows touched so far (+0x104..+0x110) are first
// restored from the backup +0x118. The path is split by the brush radius
// (0x6227ac[+0x98]) into steps, finer for small brushes, the height tool
// (+0x90 4) and weak strengths; the points go onto +0xf4/+0xf8 (+0xf0 of
// them; a fresh stroke also adds its start). The height brushes cache the
// terrain types (0x6d132c) first. Paints the whole path (0x4f8bb0) and
// makes (x, y) the last point.
RT3_NOINLINE void __fastcall MapEditor_Stroke(uint8_t* d, int, float x, float y, int32_t first, int32_t erase) {
    const float dist = (float)Distance2D(g_622844, g_622848, x, y);
    if (!first) {
        if (dist < D(0x5c8668)) return;
        if (dist > Fl(0x5c8780) && (double)((const float*)0x6227ac)[F<int32_t>(d, 0x98)] * Fl(0x5c85f8) < dist) return;
    }
    if (F<int32_t>(d, 0x8c) != 3 && F<int32_t>(d, 0x8c) != 4)
        for (int32_t row = F<int32_t>(d, 0x108); row <= F<int32_t>(d, 0x110); ++row) {
            const int32_t at = (F<int32_t>(d, 0xfc) * row + F<int32_t>(d, 0x104)) * 4;
            memcpy(F<uint8_t*>(d, 0x114) + at, F<uint8_t*>(d, 0x118) + at, (size_t)((F<int32_t>(d, 0x10c) - F<int32_t>(d, 0x104)) * 4 + 4));
        }
    const float len = (float)Distance2D(x, y, g_622844, g_622848);
    const int32_t size = F<int32_t>(d, 0x98);
    float step = size < 3 ? 4.0f : size < 4 ? 3.0f : 2.0f;
    if (F<int32_t>(d, 0x90) == 4) step = step + step;
    if (!F<int32_t>(d, 0x8c)) step = (float)(step * D(0x5c8578));
    const float strength = ((const float*)0x622790)[F<int32_t>(d, 0x9c)];
    if (strength <= D(0x5d0800))
        step = (float)(step * D(0x5c8680));
    else if (strength <= D(0x5d1478))
        step = (float)(step * D(0x5c85a0));
    const float radius = ((const float*)0x6227ac)[size];
    const double unit = radius < D(0x5c86b8) ? D(0x5c86b8) : (double)radius;
    int32_t n = Ftol((double)len / unit * step);
    if (n < 1) n = 1;
    const int32_t fresh = F<int32_t>(d, 0xf0) == 0;
    int32_t total = F<int32_t>(d, 0xf0) + n;
    if (fresh) ++total;
    float* xs = ((float*(__cdecl*)(size_t))0x5a125d)((size_t)(total * 4));
    float* ys = ((float*(__cdecl*)(size_t))0x5a125d)((size_t)(total * 4));
    if (F<int32_t>(d, 0xf0)) {
        memcpy(xs, F<void*>(d, 0xf4), (size_t)(F<int32_t>(d, 0xf0) * 4));
        memcpy(ys, F<void*>(d, 0xf8), (size_t)(F<int32_t>(d, 0xf0) * 4));
        ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0xf4));
        ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0xf8));
    }
    F<float*>(d, 0xf4) = xs;
    F<float*>(d, 0xf8) = ys;
    for (int32_t i = 0; i <= n; ++i) {
        if (i == 0 && !fresh) continue; // the start is already on the path
        const float a = (float)(n - i), b = (float)i, fn = (float)n;
        xs[F<int32_t>(d, 0xf0)] = (float)(((double)a * g_622844 + (double)b * x) / fn);
        ys[F<int32_t>(d, 0xf0)] = (float)(((double)a * g_622848 + (double)b * y) / fn);
        ++F<int32_t>(d, 0xf0);
    }
    float r = ((const float*)0x6227ac)[F<int32_t>(d, 0x98)];
    const int32_t tool = F<int32_t>(d, 0x90), sub = F<int32_t>(d, 0xb4);
    if (((tool == 4 && (sub == 0 || sub == 1 || sub == 2)) || (tool == 5 && F<int32_t>(d, 0x94) == 2)) && !hook::As<uint8_t*>(g_6d132c)) {
        // Cache the terrain types of the vertex grid.
        uint8_t* w = g_map;
        const size_t cells = (size_t)((F<int32_t>(w, 0x15d9) + 1) * (F<int32_t>(w, 0x15d5) + 1));
        uint8_t* cache = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(cells);
        hook::As<uint8_t*>(g_6d132c) = cache;
        memset(cache, 0, cells);
        for (int32_t j = 0; j < F<int32_t>(g_map, 0x15d9) + 1; ++j)
            for (int32_t i = 0; i < F<int32_t>(g_map, 0x15d5) + 1; ++i) {
                uint8_t* g = g_map;
                const int32_t stride = F<int32_t>(g, 0x15d9) + 1;
                const uint8_t v = Grid_CellLow(g, 0, j, i);
                hook::As<uint8_t*>(g_6d132c)[stride * i + j] = v;
            }
    }
    if (F<int32_t>(d, 0x90) == 4 && (F<int32_t>(d, 0xb4) == 0 || F<int32_t>(d, 0xb4) == 2 || F<int32_t>(d, 0xb4) == 1))
        r = ((const float*)0x6227e4)[F<int32_t>(d, 0x98)];
    ((void(__thiscall*)(uint8_t*, int32_t, float*, float*, float, float, int32_t, int32_t, uint32_t, int32_t, int32_t))0x4f8bb0)(
        d, F<int32_t>(d, 0xf0), F<float*>(d, 0xf4), F<float*>(d, 0xf8), r, ((const float*)0x622790)[F<int32_t>(d, 0x9c)], F<int32_t>(d, 0x88),
        F<int32_t>(d, 0xa0), F<uint32_t>(d, 0x80), F<int32_t>(d, 0x8c) == 0, erase);
    g_622844 = x;
    g_622848 = y;
}
RT3_REPLACE(0x4fc630, MapEditor_Stroke);

RT3_BRUSH_INT(0x4fcaf0, Brush_SetSizeI622854, 0x622854)

// 0x4fcb40. Destructor of the overview window for mode 0x1e.
void __fastcall MapWindow1e_Destruct(uint8_t* w) { MapModeWindow_Destruct(w, 0x5d1480, 0x1e, 0); }
RT3_REPLACE(0x4fcb40, MapWindow1e_Destruct);

// 0x4fcb80. The territory editor page: highlights +0x78 among 0xbb81.. (7) and
// +0x80 among 0xbb94.. (1..24), recounts the territories and shows each of the 24
// (0x6cfc9c): name +4 on 0xbbf8.., white when it has land (+0x3d + +0x41, else
// grey, event 0xa7) and "N tiles / M" (string 0xe5) on 0xbb94...
RT3_NOINLINE void __fastcall TerritoryDialog_Show(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0xbb81 + i, 1, 0, 0);
    for (int32_t i = 1; i <= 0x18; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x80) ? 0x87 : 0x65, 0xbb93 + i, 1, 0, 0);
    Territories_Recount(g_territories);
    for (uint32_t id = 0xbbf8; id < 0xbbf8 + 0x18; ++id) {
        uint8_t* r = (uint8_t*)Collection_Get(g_territories, 0, id - 0xbbf7);
        Window_SendEvent5(w, 0, 0x6f, id, (uint32_t)(uintptr_t)(r + 4), 0, 0);
        Window_SendEvent5(w, 0, 0xa7, id, F<int32_t>(r, 0x3d) + F<int32_t>(r, 0x41) ? 0xffffffffu : 0xffafafafu, 0, 0);
        char a[0x190], b[0x190], text[0x190];
        char* sb = Num_Format(b, 1, (double)F<int32_t>(r, 0x41), 10, 1, 0, 0);
        char* sa = Num_Format(a, 1, (double)F<int32_t>(r, 0x3d), 10, 1, 0, 0);
        FormatText(text, GetString(0xe5), sa, sb, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6e, id - 0x64, (uint32_t)(uintptr_t)text, 0, 0);
    }
}
RT3_REPLACE(0x4fcb80, TerritoryDialog_Show);

// 0x4fcd10 (event handler). The territory brush of the map editor d (size
// +0x78 from 0x622854, territory +0x80, painting +0x7c): the territory list
// 0xbb94.. picks one (0x62284c) and shows the territory overlay (0x1e), the
// sizes 0xbb81..0xbb87 set the brush (0x622850), the buttons 0xbc5c.. open
// the territory editor page; pressing on the map (without Ctrl) starts
// painting, moves paint (Map_MarkTiles), the release stops.
int32_t __fastcall TerritoryBrush_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    auto overlay = [&]() {
        uint8_t* map = g_map;
        if (F<int32_t>(map, 0x2175) == 0x1e)
            Overview_Redraw(map, 0, d + 0x80);
        else
            Overview_SetMode(map, 0, 0x1e, 0, 0, 0, d + 0x80, 0, 0, 0x1869f, 0x1869f);
        TerritoryDialog_Show(d);
    };
    const uint32_t id = msg[1];
    switch (msg[0]) {
    case 0xcb:
        if ((int32_t)id >= 0xbc5c && (int32_t)id < 0xbc74) {
            EditorDialog_Run(2, (int32_t)id - 0xbc5b);
            TerritoryDialog_Show(d);
            return 0;
        }
        if (id == 0x7d6 && F<int32_t>(d, 0x7c)) {
            F<int32_t>(d, 0x7c) = 0;
            return 0;
        }
        return 1;
    case 0xca:
        if ((int32_t)id >= 0xbb94 && (int32_t)id < 0xbbac) {
            const int32_t territory = (int32_t)id - 0xbb93;
            F<int32_t>(d, 0x80) = territory;
            g_62284c = territory;
            overlay();
            return 0;
        }
        if (id == 0x7d6) {
            float x, y;
            if (!Scene_FocusFlagIs1(&x, &y) || (msg[8] & 0x800)) return 0;
            F<int32_t>(d, 0x7c) = 1;
            break;
        }
        if ((int32_t)id > 0xbb80 && (int32_t)id <= 0xbb87) {
            const int32_t size = (int32_t)id - 0xbb81;
            Brush_SetSizeI622854(d, 0, size);
            g_622850 = size;
            TerritoryDialog_Show(d);
            return 0;
        }
        return 1;
    case 2:
        break;
    default:
        return 1;
    }
    if (!F<int32_t>(d, 0x7c)) return 1;
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    const float r = (float)((const int32_t*)0x622854)[F<int32_t>(d, 0x78)];
    Map_MarkTiles(g_map, 0, x, y, r, (uint8_t)F<int32_t>(d, 0x80));
    overlay();
    return 1;
}
RT3_REPLACE(0x4fcd10, TerritoryBrush_HandleEvent);

// 0x4fcf30. Territory tool window constructor: brush 0x622850 into the sizes
// 0x622854, setting 0x62284c into +0x80, layout 0x5d148c; shows overview
// mode 0x1e over that setting.
uint8_t* __fastcall TerritoryToolWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0x80) = hook::As<uint32_t>(g_62284c);
    EditorToolWindow_Open(w, 0x5d1480, 0x622850, 0x622854);
    F<uint32_t>(w, 0x7c) = 0;
    Window_Load(w, 0, Prod_Bits(0x3ecf5c29), Prod_Bits(0x3f34e81b), (const char*)0x5d148c);
    TerritoryDialog_Show(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(g_engine));
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2175) == 0x1e) Overview_Redraw(map, 0, w + 0x80);
    else Overview_SetMode(map, 0, 0x1e, 0, 0, 0, w + 0x80, 0, 0, 99999, 99999);
    return w;
}
RT3_REPLACE(0x4fcf30, TerritoryToolWindow_Construct);

// 0x4fd010 (draw callback). Draws entry `id` of 0x6d1354: its label (+0x88,
// right-aligned, inset by 0x5d0ad0) and its value (+8, at 0x5d14a0).
void __fastcall OptionList_DrawCb(void*, uint32_t id, float x, float y, float w, float h, int32_t) {
    uint8_t* e = (uint8_t*)Collection_Get(g_6d1354, 0, id);
    char text[0x100];
    FormatText(text, (const char*)(e + 0x88), 0, 0, 0, 0, 0, 0, 0, 0);
    const float lw = (float)((double)w - Fl(0x5d0ad0));
    const float lx = (float)((double)x + Fl(0x5d0ad0));
    Font_DrawTextBox(g_6d1358, 0, text, lx, y, lw, h, 0xffffffff, 2, 0, 0, 1.0f);
    FormatText(text, (const char*)(e + 8), 0, 0, 0, 0, 0, 0, 0, 0);
    const float vx = (float)((double)x + Fl(0x5d14a0));
    Font_DrawTextBox(g_6d1358, 0, text, vx, y, BitsF(0x3e81999a), h, 0xffffffff, 3, 0, 0, 1.0f);
}
RT3_REPLACE(0x4fd010, OptionList_DrawCb);

void __fastcall Options_DisplayModeCb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDisplayMode(g_app, 0, v);
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fd100, Options_DisplayModeCb);

RT3_OPTION(0x4fd130, Options_SetF4Cb, 0xf4)

RT3_OPTION(0x4fd160, Options_Set22fCb, 0x22f)

// 0x4fd190 (options callback). Speaker type (EDX), applied and saved.
void __fastcall Options_SetSpeakersCb(void*, int32_t type) {
    if (g_6d135d) return;
    Sound_Speakers(g_sound, 0, type, 0);
    F<int32_t>(g_engine, 0x11470a) = type;
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fd190, Options_SetSpeakersCb);

// 0x4fd1d0 / 0x4fd200 (options callbacks). Detail settings 0xa / 0xb.
void __fastcall Options_SetDetailACb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDetail(g_app, 0, 0xa, v, 1);
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fd1d0, Options_SetDetailACb);

void __fastcall Options_SetDetailBCb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDetail(g_app, 0, 0xb, v, 1);
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fd200, Options_SetDetailBCb);

void __fastcall Options_SetEngine1146b3Cb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    F<int32_t>(g_engine, 0x1146b3) = v;
    Engine_SaveConfig(g_engine);
}
RT3_REPLACE(0x4fd230, Options_SetEngine1146b3Cb);

RT3_ONOFF(0x4fd260, Options_Choice233Cb, g_6d135d, 0x233, 0x237)

// 0x4fd2c0. Engine option dialog: switches the engine mode +0x114736 to
// `mode` (not -1, not while 0x6d135d or re-entered via 0x6d1364). Positive
// modes need hardware support (+0x11422f): without it list 0x7573 goes back
// to 0 and 0xe14 is shown; otherwise in a game 0xe15 warns first. Saves the
// options either way.
void __fastcall Options_SetEngineMode(int32_t mode) {
    if (mode == -1 || g_6d135d) return;
    uint8_t* e = g_engine;
    if (mode == F<int32_t>(e, 0x114736) || g_6d1364) return;
    g_6d1364 = 1;
    if (mode > 0 && !F<uint8_t>(e, 0x11422f)) {
        Window_SendEvent5(g_optionsWindow, 0, 0x66, 0x7573, 0, 0, 0);
        Message_Show(GetString(0xe14), 1, 0);
    } else {
        if (hook::As<void*>(g_world)) {
            Message_Show(GetString(0xe15), 1, 0);
            e = g_engine;
        }
        F<int32_t>(e, 0x114736) = mode;
    }
    Options_Save(g_app, 0, 1);
    --g_6d1364;
}
RT3_REPLACE(0x4fd2c0, Options_SetEngineMode);

RT3_ONOFF(0x4fd3d0, Options_Choice23bCb, g_6d135d, 0x23b, 0x23f)

RT3_OPTION(0x4fd430, Options_Set31dCb, 0x31d)

// 0x4fd460 (options callback). Lighting option +0x319, saved and applied.
void __fastcall Options_SetLightingCb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    F<int32_t>(g_app, 0x319) = v;
    Options_Save(g_app, 0, 1);
    Lighting_Rebuild(g_app);
}
RT3_REPLACE(0x4fd460, Options_SetLightingCb);

// 0x4fd4a0 (options callback). Engine option +0x11474e, saved; redraws the
// overview.
void __fastcall Options_SetEngine11474eCb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    F<int32_t>(g_engine, 0x11474e) = v;
    Engine_SaveConfig(g_engine);
    if (uint8_t* m = g_map) Overview_RedrawRect(m, 0, 0, 0, 0x270f, 0x270f);
}
RT3_REPLACE(0x4fd4a0, Options_SetEngine11474eCb);

RT3_OPTION(0x4fd4f0, Options_Set311Cb, 0x311)

RT3_OPTION(0x4fd520, Options_Set315Cb, 0x315)

// 0x4fd550. Sets up option sliders 0x7566 / 0x7560 / 0x7562 / 0x7564 (range 21,
// event 0x73) from the options' entries 8 / 7 / 7 / 9 (sic: two sliders show entry
// 7) and slider 0x7596 as 4 - entry 13 (event 0x6a).
RT3_NOINLINE void __fastcall OptionsDialog_ShowSliders2(uint8_t* w) {
    void* o = hook::As<void*>(g_app);
    Window_SendEvent5(w, 0, 0x73, 0x7566, 0x15, 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7566, (uint32_t)Obj_ArrayAc(o, 0, 8), 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7596, (uint32_t)(4 - Obj_ArrayAc(hook::As<void*>(g_app), 0, 0xd)), 0, 0);
    Window_SendEvent5(w, 0, 0x73, 0x7560, 0x15, 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7560, (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, 7), 0, 0);
    Window_SendEvent5(w, 0, 0x73, 0x7562, 0x15, 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7562, (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, 7), 0, 0);
    Window_SendEvent5(w, 0, 0x73, 0x7564, 0x15, 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7564, (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, 9), 0, 0);
}
RT3_REPLACE(0x4fd550, OptionsDialog_ShowSliders2);

// 0x4fd650. Sets up four option sliders (range 21, event 0x73) and their values
// from the options' +0xac array entries 6 / 4 / 5 / 3 (event 0x6a).
RT3_NOINLINE void __fastcall OptionsDialog_ShowSliders(uint8_t* w) {
    const uint32_t ids[4] = { 0x756e, 0x756a, 0x756c, 0x7570 };
    const int32_t entries[4] = { 6, 4, 5, 3 };
    for (int k = 0; k < 4; ++k) {
        Window_SendEvent5(w, 0, 0x73, ids[k], 0x15, 0, 0);
        Window_SendEvent5(w, 0, 0x6a, ids[k], (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, entries[k]), 0, 0);
    }
}
RT3_REPLACE(0x4fd650, OptionsDialog_ShowSliders);

// 0x4fd730. Option list 0x7546 (four name / description pairs from 0x6228d8,
// current +0x10c capped at 3), switches engine +0x1146d7 / +0x283 / +0x28b on 0x7547
// / 0x7593 / 0x7594, and lists 0x759f / 0x75a0 (the three names from 0x6229fc,
// current +0x311 / +0x315).
RT3_NOINLINE void __fastcall OptionsDialog_ShowGraphics(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7546, 0, 0, 0);
    for (int32_t i = 0; i < 4; ++i) {
        const char* desc = GetString(((const int32_t*)0x6228dc)[i * 2]);
        const char* name = GetString(((const int32_t*)0x6228d8)[i * 2]);
        Window_SendEvent5(w, 0, 0x88, 0x7546, (uint32_t)(uintptr_t)name, (uint32_t)i, (uint32_t)(uintptr_t)desc);
    }
    int32_t q = F<int32_t>(g_app, 0x10c);
    Window_SendEvent5(w, 0, 0x66, 0x7546, (uint32_t)(q > 3 ? 3 : q), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7547, g_engine[0x1146d7], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7593, F<uint32_t>(g_app, 0x283), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7594, F<uint32_t>(g_app, 0x28b), 0, 0);
    const uint32_t lists[2] = { 0x759f, 0x75a0 };
    const int32_t sel[2] = { 0x311, 0x315 };
    for (int k = 0; k < 2; ++k) {
        Window_SendEvent5(w, 0, 0x8b, lists[k], 0, 0, 0);
        for (const int32_t* id = (const int32_t*)0x6229fc; id < (const int32_t*)0x622a08; ++id)
            Window_SendEvent5(w, 0, 0x88, lists[k], (uint32_t)(uintptr_t)GetString(*id), 0, 0);
        Window_SendEvent5(w, 0, 0x66, lists[k], F<uint32_t>(g_app, sel[k]), 0, 0);
    }
}
RT3_REPLACE(0x4fd730, OptionsDialog_ShowGraphics);

// 0x4fd8d0. Fills list 0x759e of window w (cleared with 0x8b) with the five names
// of table 0x622a08 (event 0x88) and selects the engine's +0x11474e (0x66).
RT3_NOINLINE void __fastcall Window_FillList759e(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x759e, 0, 0, 0);
    for (int32_t i = 0; i < 5; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x759e, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x622a08)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x66, 0x759e, F<uint32_t>(g_engine, 0x11474e), 0, 0);
}
RT3_REPLACE(0x4fd8d0, Window_FillList759e);

// 0x4fd940. Shows option +0xf8 on box 0x7545 and the inverted switches +0x26b..
// +0x27b on boxes 0x758c..0x7590 (event 0x74).
RT3_NOINLINE void __fastcall OptionsDialog_ShowSwitches(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x74, 0x7545, F<uint32_t>(g_app, 0xf8), 0, 0);
    for (int k = 0; k < 5; ++k)
        Window_SendEvent5(w, 0, 0x74, 0x758c + k, F<int32_t>(g_app, 0x26b + k * 4) == 0 ? 1 : 0, 0, 0);
}
RT3_REPLACE(0x4fd940, OptionsDialog_ShowSwitches);

// 0x4fda20. Option lists 0x7548 (six names from 0x6228f8, current +0xf4) and
// 0x7591 (three name / description pairs from 0x622910, current +0x22f), and the
// switches +0x263 / +0x2cc / +0x309 / +0xa8 / engine +0x114742 on 0x7592 / 0x7599 /
// 0x759c / 0x7544 / 0x759b.
RT3_NOINLINE void __fastcall OptionsDialog_ShowGeneral(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7548, 0, 0, 0);
    for (int32_t i = 0; i < 6; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x7548, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x6228f8)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x66, 0x7548, F<uint32_t>(g_app, 0xf4), 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x7591, 0, 0, 0);
    for (int32_t i = 0; i < 3; ++i) {
        const char* desc = GetString(((const int32_t*)0x622914)[i * 2]);
        const char* name = GetString(((const int32_t*)0x622910)[i * 2]);
        Window_SendEvent5(w, 0, 0x88, 0x7591, (uint32_t)(uintptr_t)name, (uint32_t)i, (uint32_t)(uintptr_t)desc);
    }
    uint8_t* o = g_app;
    Window_SendEvent5(w, 0, 0x66, 0x7591, F<uint32_t>(o, 0x22f), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7592, F<uint32_t>(g_app, 0x263), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7599, F<uint32_t>(g_app, 0x2cc), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x759c, F<uint32_t>(g_app, 0x309), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7544, F<uint32_t>(g_app, 0xa8), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x759b, F<uint32_t>(g_engine, 0x114742), 0, 0);
}
RT3_REPLACE(0x4fda20, OptionsDialog_ShowGeneral);

// 0x4fdb90. The sound options page: without any 3D provider hides 0x7534 and
// shows 0x753f; else lists the providers in 0x7551 (selecting the active one),
// box 0x7550 ticked when the active provider has no flags, speaker list 0x754f
// (string 0x584 as data 99, then six names from 0x6229e4; current speakers) and the
// three volume sliders 0x7549 / 0x754b / 0x754d (range 11, engine +0x1146fa.. x 10).
RT3_NOINLINE void __fastcall OptionsDialog_ShowSound(uint8_t* w) {
    const char* p = (const char*)(uintptr_t)Sound_Get3DProvider(0);
    if (!p) {
        Window_SendEvent5(w, 0, 0x86, 0x7534, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6d, 0x753f, 3, 0, 0);
        return;
    }
    Window_SendEvent5(w, 0, 0x8b, 0x7551, 0, 0, 0);
    for (int32_t i = 1; p; p = (const char*)(uintptr_t)Sound_Get3DProvider(i++))
        Window_SendEvent5(w, 0, 0x88, 0x7551, (uint32_t)(uintptr_t)p, 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x7551, (uint32_t)Sound_ActiveProviderIndex(g_sound), 0, 0);
    int32_t active = Sound_ActiveProviderIndex(g_sound);
    Window_SendEvent5(w, 0, 0x74, 0x7550, Sound_ProviderFlagsAt(g_sound, 0, active) == 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x754f, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x88, 0x754f, (uint32_t)(uintptr_t)GetString(0x584), 0x63, 0);
    for (int32_t i = 0; i < 6; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x754f, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x6229e4)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x68, 0x754f, (uint32_t)Sound_GetSpeakers(g_sound), 0, 0);
    for (uint32_t id = 0x7549; id < 0x754f; id += 2) Window_SendEvent5(w, 0, 0x73, id, 0xb, 0, 0);
    const float ten = *(const float*)0x5c85f8;
    Window_SendEvent5(w, 0, 0x6a, 0x7549, (uint32_t)Ftol((double)F<float>(g_engine, 0x1146fa) * ten), 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x754b, (uint32_t)Ftol((double)F<float>(g_engine, 0x1146fe) * ten), 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x754d, (uint32_t)Ftol((double)F<float>(g_engine, 0x114702) * ten), 0, 0);
}
RT3_REPLACE(0x4fdb90, OptionsDialog_ShowSound);

// 0x4fdd70. Engine option lists 0x7573 (strings 0xe18..0xe1a with descriptions
// 0xe1b..0xe1d, current +0x114736) and 0x7574 (three names from 0x622928, current
// +0x1146b3); switches: float +0x1146af == 0x5c8628, bytes +0x1146a8 / +0x1146e3 and
// options entry 12 set, on 0x7575 / 0x7576 / 0x7577 / 0x7586.
RT3_NOINLINE void __fastcall OptionsDialog_ShowEngine(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7573, 0, 0, 0);
    for (int32_t i = 0; i < 3; ++i) {
        const char* desc = GetString(0xe1b + i);
        const char* name = GetString(0xe18 + i);
        Window_SendEvent5(w, 0, 0x88, 0x7573, (uint32_t)(uintptr_t)name, (uint32_t)i, (uint32_t)(uintptr_t)desc);
    }
    Window_SendEvent5(w, 0, 0x66, 0x7573, F<uint32_t>(g_engine, 0x114736), 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x7574, 0, 0, 0);
    for (int32_t i = 0; i < 3; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x7574, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x622928)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x66, 0x7574, F<uint32_t>(g_engine, 0x1146b3), 0, 0);
    bool eq = (double)F<float>(g_engine, 0x1146af) == *(const double*)0x5c8628;
    Window_SendEvent5(w, 0, 0x74, 0x7575, eq ? 1 : 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7576, g_engine[0x1146a8], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7577, g_engine[0x1146e3], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7586, Obj_ArrayAc2(hook::As<void*>(g_app), 0, 0xc) != 0, 0, 0);
}
RT3_REPLACE(0x4fdd70, OptionsDialog_ShowEngine);

// 0x4fdf20. Shows option values +0x114 / +0x118 / +0x11c on sliders 0x757d / 0x757f /
// 0x7581 (0x6a) and the +0x120 switch as the radio pair 0x7584 / 0x7585 (0x74).
RT3_NOINLINE void __fastcall OptionsDialog_ShowValues(uint8_t* w) {
    uint8_t* o = g_app;
    Window_SendEvent5(w, 0, 0x6a, 0x757d, F<uint32_t>(o, 0x114), 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x757f, F<uint32_t>(g_app, 0x118), 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x7581, F<uint32_t>(g_app, 0x11c), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7584, F<uint32_t>(g_app, 0x120), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7585, F<int32_t>(g_app, 0x120) == 0 ? 1 : 0, 0, 0);
}
RT3_REPLACE(0x4fdf20, OptionsDialog_ShowValues);

// 0x4fdfd0. Option lists 0x7587 (25 names from 0x620dd8; selection +0x237 + 1 when
// +0x233 is set, else 0) and 0x7588 (10 names from 0x620e3c; +0x23f + 1 when +0x23b)
// and switches +0x247 / +0x24b / !+0x243 on 0x7589 / 0x758b / 0x758a.
RT3_NOINLINE void __fastcall OptionsDialog_ShowLists7587(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7587, 0, 0, 0);
    for (int32_t i = 0; i < 0x19; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x7587, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x620dd8)[i]), (uint32_t)i, 0);
    uint8_t* o = g_app;
    Window_SendEvent5(w, 0, 0x66, 0x7587, F<int32_t>(o, 0x233) ? F<uint32_t>(o, 0x237) + 1 : 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x7588, 0, 0, 0);
    for (int32_t i = 0; i < 0xa; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x7588, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x620e3c)[i]), (uint32_t)i, 0);
    o = g_app;
    Window_SendEvent5(w, 0, 0x66, 0x7588, F<int32_t>(o, 0x23b) ? F<uint32_t>(o, 0x23f) + 1 : 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7589, F<uint32_t>(g_app, 0x247), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x758b, F<uint32_t>(g_app, 0x24b), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x758a, F<int32_t>(g_app, 0x243) == 0, 0, 0);
}
RT3_REPLACE(0x4fdfd0, OptionsDialog_ShowLists7587);

RT3_NOINLINE int32_t Global6d1350Set() { return hook::As<void*>(g_optionsWindow) ? 1 : 0; }
RT3_REPLACE(0x4fe120, Global6d1350Set);

void SetGlobal6d1360() { g_6d1360 = 1; }
RT3_REPLACE(0x4fe130, SetGlobal6d1360);

RT3_NOINLINE uint32_t __fastcall Obj_Busy18(uint8_t* o) {
    if (Obj_Get18(o)) return 1;
    return Obj_Zero18And14(o) ? 1 : 0;
}
RT3_REPLACE(0x4fe140, Obj_Busy18);

// 0x4fe160. Restarts the world's ambient sounds, resets the per-session globals
// when the options say so (+0x14) and replays the renderer's sound channels.
RT3_NOINLINE void __fastcall Game_RestartSounds() {
    if (hook::As<void*>(g_world)) {
        Ambient_StopSlots();
        Ambient_Start(hook::As<void*>(g_world));
    }
    if (F<int32_t>(g_app, 0x14)) ResetGlobals4686d0();
    if (Engine_Renderer(hook::As<void*>(g_engine)))
        Channels_PlayAll((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
}
RT3_REPLACE(0x4fe160, Game_RestartSounds);

// 0x4fe1b0. Picks sound provider `idx` (none for -1 or while 0x6d135d is set):
// selects it, shows the provider actually chosen in list 0x7551 of the sound
// dialog (0x6d1350) if that differs, ticks box 0x7550 when its flags are zero,
// stores the index in the engine (+0x114706), restarts the sounds and saves the
// engine configuration.
void __fastcall SoundDialog_PickProvider(int32_t idx) {
    if (idx == -1 || g_6d135d) return;
    Sound_SelectProvider(g_sound, 0, idx);
    if (Sound_ActiveProviderIndex(g_sound) != idx)
        Window_SendEvent5(g_optionsWindow, 0, 0x66, 0x7551, (uint32_t)Sound_ActiveProviderIndex(g_sound), 0, 0);
    int32_t active = Sound_ActiveProviderIndex(g_sound);
    uint32_t flags = Sound_ProviderFlagsAt(g_sound, 0, active);
    Window_SendEvent5(g_optionsWindow, 0, 0x74, 0x7550, flags == 0 ? 1 : 0, 0, 0);
    F<int32_t>(g_engine, 0x114706) = Sound_ActiveProviderIndex(g_sound);
    Game_RestartSounds();
    Engine_SaveConfig(g_engine);
}
RT3_REPLACE(0x4fe1b0, SoundDialog_PickProvider);

// 0x4fe270 (options callback). Resolution choice EDX (width << 16 |
// height): applied (windowed state kept) unless it is the current one.
void __fastcall Options_SetResolutionCb(int32_t v, int32_t res) {
    if (v == -1 || g_6d135d) return;
    uint8_t* e = g_engine;
    const int32_t* cur = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    const int32_t w = res >> 16, h = res & 0xffff;
    if (w == cur[0] && h == cur[1]) return;
    Engine_SetDisplayMode(e, 0, w, h, -1, 1);
    Engine_SaveConfig(g_engine);
}
RT3_REPLACE(0x4fe270, Options_SetResolutionCb);

// 0x4fe2d0 (options callback). Detail setting 0 from the level table
// 0x6211d0, saved; the cursor buttons (0x757c / 0x759a) follow the cursor
// state.
void __fastcall Options_SetDetail0Cb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDetail(g_app, 0, 0, ((const int32_t*)0x6211d0)[v], 1);
    Options_Save(g_app, 0, 1);
    uint8_t* c = g_cursorManager;
    const uint32_t on = Obj_Get18(c) || Obj_Zero18And14(c) ? 1 : 0;
    Window_SendEvent5(g_optionsWindow, 0, 0x74, 0x757c, on, 0, 0);
    c = g_cursorManager;
    if (Obj_Get18(c) || Obj_Zero18And14(c)) Window_SendEvent5(g_optionsWindow, 0, 0x65, 0x759a, 3, 0, 0);
    else Window_SendEvent5(g_optionsWindow, 0, 0x87, 0x759a, 3, 0, 0);
    Window_SendEvent5(g_optionsWindow, 0, 0x74, 0x759a, (uint32_t)Obj_Zero18And14(g_cursorManager), 0, 0);
}
RT3_REPLACE(0x4fe2d0, Options_SetDetail0Cb);

// 0x4fe3b0 / 0x4fe400 (options callbacks). Detail settings 1 / 2 from the
// level table 0x6211d0; the dialog's sliders are refreshed with 0x6d135c set.
void __fastcall Options_SetDetail1Cb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDetail(g_app, 0, 1, ((const int32_t*)0x6211d0)[v], 1);
    g_6d135c = 1;
    OptionsDialog_ShowSliders2(g_optionsWindow);
    g_6d135c = 0;
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fe3b0, Options_SetDetail1Cb);

void __fastcall Options_SetDetail2Cb(int32_t v) {
    if (v == -1 || g_6d135d) return;
    Options_SetDetail(g_app, 0, 2, ((const int32_t*)0x6211d0)[v], 1);
    g_6d135c = 1;
    OptionsDialog_ShowSliders(g_optionsWindow);
    g_6d135c = 0;
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fe400, Options_SetDetail2Cb);

// 0x4fe450. Settings window ("SettingsWindow.win") destructor: deletes its
// collection 0x6d1354, undoes its modal count in the window manager (+0xc60)
// and releases its image 0x6d1358.
RT3_NOINLINE void __fastcall SettingsWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d14a4;
    if (Collection* c = g_6d1354) {
        Collection_Destruct(c);
        ExeDelete(c);
    }
    g_6d1354 = nullptr;
    if (!Obj_Idlec60(g_windowManager)) Obj_Decc60(g_windowManager);
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d1358));
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4fe450, SettingsWindow_Destruct);

// 0x4fe4c0. Fills the resolution list 0x7553 with the display modes between
// 640x480 and 1600x1200 (windowed: smaller than the desktop, engine +0x38 / +0x3c)
// as "W x H" (format 0x5d14b0, data W << 16 | H), skipping a mode when an earlier
// listed one has the same size and a depth other than 32 bits (sic: duplicates of
// a 32-bit mode stay); selects the current mode (engine +0x34 table, 21-byte
// records, index +0x11421e).
RT3_NOINLINE void __fastcall OptionsDialog_FillResolutions(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7553, 0, 0, 0);
    int32_t n = Engine_ModeCount(hook::As<void*>(g_engine));
    uint8_t* e = g_engine;
    const uint8_t* cur = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    int32_t curW = *(const int32_t*)cur, curH = *(const int32_t*)(cur + 4);
    int32_t listed = 0, sel = 0;
    static int32_t ok[0x1f0];
    for (int32_t i = 0; i < n; ++i) {
        int32_t mw, mh, bpp;
        Engine_ModeAt(hook::As<void*>(g_engine), 0, i, &mw, &mh, &bpp);
        bool fits = mw >= 0x280 && mw <= 0x640 && mh >= 0x1e0 && mh <= 0x4b0;
        if (fits && !Engine_IsFullscreen(hook::As<void*>(g_engine))) {
            uint8_t* en = g_engine;
            fits = mw < F<int32_t>(en, 0x38) && mh < F<int32_t>(en, 0x3c);
        }
        if (!fits) {
            ok[i] = 0;
            continue;
        }
        ok[i] = 1;
        for (int32_t j = 0; j < i; ++j) {
            if (!ok[j]) continue;
            int32_t w2, h2, b2;
            Engine_ModeAt(hook::As<void*>(g_engine), 0, j, &w2, &h2, &b2);
            if (w2 == mw && h2 == mh && b2 != 0x20) {
                ok[i] = 0;
                break;
            }
        }
        if (!ok[i]) continue;
        char a[0x20], b[0x20], text[0x200];
        char* sh = Num_Format(b, 8, (double)mh, 10, 1, 0, 0);
        char* sw = Num_Format(a, 8, (double)mw, 10, 1, 0, 0);
        FormatText(text, (const char*)0x5d14b0, sw, sh, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x88, 0x7553, (uint32_t)(uintptr_t)text, (uint32_t)(mw << 16 | mh), 0);
        if (mw == curW && mh == curH) sel = listed;
        ++listed;
    }
    Window_SendEvent5(w, 0, 0x66, 0x7553, (uint32_t)sel, 0, 0);
}
RT3_REPLACE(0x4fe4c0, OptionsDialog_FillResolutions);

RT3_NOINLINE void __fastcall Window_SelectOption755f(uint8_t* w) { Window_SelectOption(w, 1, 0x755f); }
RT3_REPLACE(0x4fe6f0, Window_SelectOption755f);

RT3_NOINLINE void __fastcall Window_SelectOption7569(uint8_t* w) { Window_SelectOption(w, 2, 0x7569); }
RT3_REPLACE(0x4fe730, Window_SelectOption7569);

// 0x4fe770. The video options page: fullscreen box 0x7554 (engine +0x114226),
// box 0x757c and button 0x759a (on the options window 0x6d1350) from the mode
// object 0xccbb20 (+0x18 set, or +0x14 / +0x18 both clear), 0x759a ticked by the
// latter alone, switches option +0x2c3 / engine +0x114206 on 0x7595 / 0x7556, the
// two fullscreen-only switches 0x7555 / 0x7578 (disabled when windowed), the
// resolution list and option list 0x7557 (ten names from 0x622964, entry 0).
RT3_NOINLINE void __fastcall OptionsDialog_ShowVideo(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x74, 0x7554, g_engine[0x114226], 0, 0);
    auto mode = []() {
        uint8_t* m = g_cursorManager;
        return Obj_Get18(m) ? 1 : Obj_Zero18And14(m);
    };
    Window_SendEvent5(w, 0, 0x74, 0x757c, (uint32_t)mode(), 0, 0);
    Window_SendEvent5(g_optionsWindow, 0, mode() ? 0x65 : 0x87, 0x759a, 1, 0, 0);
    Window_SendEvent5(g_optionsWindow, 0, 0x74, 0x759a, (uint32_t)Obj_Zero18And14(g_cursorManager), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7595, F<uint32_t>(g_app, 0x2c3), 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7556, F<uint32_t>(g_engine, 0x114206), 0, 0);
    if (g_engine[0x114226]) {
        Window_SendEvent5(w, 0, 0x74, 0x7555, (uint32_t)Engine_Get1146ae(hook::As<void*>(g_engine)), 0, 0);
        Window_SendEvent5(w, 0, 0x74, 0x7578, (uint32_t)Engine_Get11470e(hook::As<void*>(g_engine)), 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x87, 0x7555, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x7578, 3, 0, 0);
    }
    OptionsDialog_FillResolutions(w);
    Window_SendEvent5(w, 0, 0x8b, 0x7557, 0, 0, 0);
    for (const int32_t* id = (const int32_t*)0x622964; id < (const int32_t*)0x62298c; ++id)
        Window_SendEvent5(w, 0, 0x88, 0x7557, (uint32_t)(uintptr_t)GetString(*id), 0, 0);
    Window_SelectOption(w, 0, 0x7557);
}
RT3_REPLACE(0x4fe770, OptionsDialog_ShowVideo);

// 0x4fe960. Option lists 0x755a (five names from 0x622940, value entry 10, event
// 0x6a) and 0x755b (four from 0x622954, entry 11); slider 0x7558 (range 31, value
// (engine float +0x1146b9 - 0x5c8d48) * 0x5c8784) when engine +0x114226 is set, else
// 0x7558 / 0x7559 / 0x7579 disabled; switches +0x1146eb / +0x1146e9 / +0x114697 on
// 0x755c / 0x755d / 0x755e.
RT3_NOINLINE void __fastcall OptionsDialog_ShowDisplay(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x755a, 0, 0, 0);
    for (int32_t i = 0; i < 5; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x755a, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x622940)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x755a, (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, 10), 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x755b, 0, 0, 0);
    for (int32_t i = 0; i < 4; ++i)
        Window_SendEvent5(w, 0, 0x88, 0x755b, (uint32_t)(uintptr_t)GetString(((const int32_t*)0x622954)[i]), (uint32_t)i, 0);
    Window_SendEvent5(w, 0, 0x66, 0x755b, (uint32_t)Obj_ArrayAc(hook::As<void*>(g_app), 0, 11), 0, 0);
    if (g_engine[0x114226]) {
        Window_SendEvent5(w, 0, 0x73, 0x7558, 0x1f, 0, 0);
        float g = F<float>(g_engine, 0x1146b9);
        int32_t v = Ftol(((double)g - *(const float*)0x5c8d48) * *(const float*)0x5c8784);
        Window_SendEvent5(w, 0, 0x66, 0x7558, (uint32_t)v, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x87, 0x7558, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x7559, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x7579, 3, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x74, 0x755c, g_engine[0x1146eb], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x755d, g_engine[0x1146e9], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x755e, F<uint32_t>(g_engine, 0x114697), 0, 0);
}
RT3_REPLACE(0x4fe960, OptionsDialog_ShowDisplay);

// 0x4feb20. Fills option list 0x755f with the ten names of table 0x622964, selects
// the current entry (options entry 1), shows the second slider set and the engine
// switch +0x1146b8 on box 0x7568.
RT3_NOINLINE void __fastcall OptionsDialog_FillList755f(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x755f, 0, 0, 0);
    for (const int32_t* id = (const int32_t*)0x622964; id < (const int32_t*)0x62298c; ++id)
        Window_SendEvent5(w, 0, 0x88, 0x755f, (uint32_t)(uintptr_t)GetString(*id), 0, 0);
    Window_SelectOption(w, 1, 0x755f);
    OptionsDialog_ShowSliders2(w);
    Window_SendEvent5(w, 0, 0x74, 0x7568, g_engine[0x1146b8], 0, 0);
}
RT3_REPLACE(0x4feb20, OptionsDialog_FillList755f);

// 0x4febd0. Fills option list 0x7569 with the ten names of table 0x622964 (event
// 0x88), selects the current entry (options entry 2) and shows the sliders.
RT3_NOINLINE void __fastcall OptionsDialog_FillList7569(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x7569, 0, 0, 0);
    for (const int32_t* id = (const int32_t*)0x622964; id < (const int32_t*)0x62298c; ++id)
        Window_SendEvent5(w, 0, 0x88, 0x7569, (uint32_t)(uintptr_t)GetString(*id), 0, 0);
    Window_SelectOption(w, 2, 0x7569);
    OptionsDialog_ShowSliders(w);
}
RT3_REPLACE(0x4febd0, OptionsDialog_FillList7569);

// 0x4fec60. Options dialog w: refills list 0x757a with the models in list
// order (display name +8, or 0xb3e when empty, and file name +0x88) kept in
// the table 0x6d1354 (0x108-byte records), then re-selects its row. QUIRK:
// the original leaves the first 8 bytes of each record uninitialised (stack
// residue); here they are zero.
RT3_NOINLINE void __fastcall OptionsDialog_FillModels(uint8_t* w, int, int32_t) {
    if (Obj_Idlec60(g_windowManager)) Obj_Incc60(g_windowManager);
    uint8_t* list = Widget_FindChild(w, 0, 0x757a);
    int32_t sel = F<int32_t>(list, 0x1e2);
    Window_SendEvent5(w, 0, 0x8b, 0x757a, 0, 0, 0);
    if (Collection* old = g_6d1354) {
        Collection_Destruct(old);
        ((void(__cdecl*)(void*))0x53b080)(old);
    }
    g_6d1354 = nullptr;
    Collection* c = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    g_6d1354 = c ? Collection_Construct(c, 0, 1, 0x108, 10, 5, 0, 0, 0) : nullptr;
    while (uint8_t* m = Models_NextInOrder()) {
        uint8_t rec[0x108];
        memset(rec, 0, 8);
        const char* name = (const char*)m + 8;
        FormatText((char*)rec + 8, strlen(name) ? name : GetString(0xb3e), 0, 0, 0, 0, 0, 0, 0, 0);
        FormatText((char*)rec + 0x88, (const char*)m + 0x88, 0, 0, 0, 0, 0, 0, 0, 0);
        Collection* t = g_6d1354;
        uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)t)[1])(t, 0, rec, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x88, 0x757a, 0, id, 0);
    }
    ListBox_Select(list, 0, sel, 0);
}
RT3_REPLACE(0x4fec60, OptionsDialog_FillModels);

// 0x4fede0 (slider in ECX, value in EDX). Options dialog slider callback:
// shows the value in the label after the slider (id + 1) and, unless the
// page is being filled (0x6d135d), applies it: the three volumes (0x7549 /
// 0x754b / 0x754d, as value / 0x5c85f8), the speed offset (0x7558, as float bits), the
// detail settings 3..9 (7 for both 0x7560 / 0x7562, kept in step) and 13
// (0x7596, as 40..100 %), and the scroll speeds +0x114 / +0x118 / +0x11c
// (0x757d / 0x757f / 0x7581; a change to or from 0 of +0x118 relabels the
// setup window). Saves the options.
void __fastcall OptionsDialog_OnSlider(uint8_t* sl, int32_t v) {
    char text[0x1ec];
    typedef int(__cdecl * Sprintf)(char*, const char*, ...);
    Sprintf sprintf_ = (Sprintf)0x5a19c4;
    auto percent = [&]() {
        if (v > 0) sprintf_(text, (const char*)0x5d0b18, v * 10);
        else FormatText(text, GetString(8), 0, 0, 0, 0, 0, 0, 0, 0);
    };
    auto speedText = [&]() { sprintf_(text, (const char*)0x5d0b18, Ftol((double)((const float*)0x5f34b0)[v] * Fl(0x5c8c28) + D(0x5c85b8))); };
    auto number = [&]() { Num_Format(text, 1, (double)v, 10, 1, 0, 0); };
    auto detail = [&](int32_t which, int32_t level, bool list755f) {
        Options_SetDetail(g_app, 0, which, level, 1);
        if (!g_6d135c) {
            if (list755f) Window_SelectOption755f(g_optionsWindow);
            else Window_SelectOption7569(g_optionsWindow);
        }
    };
    switch (F<uint16_t>(sl, 4)) {
    case 0x7549:
    case 0x754b:
    case 0x754d: {
        percent();
        if (g_6d135d) break;
        uint32_t off = F<uint16_t>(sl, 4) == 0x7549 ? 0x1146fa : F<uint16_t>(sl, 4) == 0x754b ? 0x1146fe : 0x114702;
        float fv = (float)v;
        F<float>(g_engine, off) = (float)((double)fv / Fl(0x5c85f8));
        float vol = F<float>(g_engine, off);
        int32_t kind = 2;
        if (off == 0x1146fe) {
            vol = (float)(vol * D(0x5c9c80));
            kind = 0;
        } else if (off == 0x114702) {
            vol = (float)(vol * D(0x5c8640));
            kind = 1;
        }
        Sound_SetVolume(g_sound, 0, vol, kind);
        break;
    }
    case 0x7558: {
        float fv = (float)v;
        float off = (float)((double)fv / Fl(0x5c8784) + Fl(0x5c8d48));
        sprintf_(text, (const char*)0x5d14d0, (double)off);
        uint32_t bits;
        memcpy(&bits, &off, 4);
        if (!g_6d135d) Engine_SetCursorObject(hook::As<void*>(g_engine), 0, bits);
        break;
    }
    case 0x7596: {
        static const uint32_t names[] = { 0x5d14cc, 0x5d14c8, 0x5d14c4, 0x5d14c0, 0x5d14b8 };
        strcpy(text, (const char*)names[v]);
        if (!g_6d135d) detail(0xd, 4 - v, true);
        break;
    }
    case 0x7566:
        number();
        if (!g_6d135d) detail(8, v, true);
        break;
    case 0x7560:
    case 0x7562:
        number();
        if (!g_6d135d) {
            Options_SetDetail(g_app, 0, 7, v, 1);
            Window_SendEvent5(g_optionsWindow, 0, 0x66, F<uint16_t>(sl, 4) == 0x7560 ? 0x7562 : 0x7560, (uint32_t)v, 0, 0);
            if (!g_6d135c) Window_SelectOption755f(g_optionsWindow);
        }
        break;
    case 0x7564:
        number();
        if (!g_6d135d) detail(9, v, true);
        break;
    case 0x756e:
        number();
        if (!g_6d135d) detail(6, v, false);
        break;
    case 0x756a:
        number();
        if (!g_6d135d) detail(4, v, false);
        break;
    case 0x756c:
        number();
        if (!g_6d135d) detail(5, v, false);
        break;
    case 0x7570:
        number();
        if (!g_6d135d) detail(3, v, false);
        break;
    case 0x757d:
        if (v > 0) speedText();
        else FormatText(text, GetString(8), 0, 0, 0, 0, 0, 0, 0, 0);
        if (!g_6d135d) {
            F<int32_t>(g_app, 0x114) = v;
            Options_Save(g_app, 0, 1);
        }
        break;
    case 0x757f:
        speedText();
        if (!g_6d135d) {
            int32_t old = F<int32_t>(g_app, 0x118);
            F<int32_t>(g_app, 0x118) = v;
            Options_Save(g_app, 0, 1);
            if (uint8_t* setup = g_setupWindow)
                if (old != v && (!old || !v)) SetupWindow_FillLabels(setup);
        }
        break;
    case 0x7581:
        speedText();
        if (!g_6d135d) {
            F<int32_t>(g_app, 0x11c) = v;
            Options_Save(g_app, 0, 1);
        }
        break;
    default:
        return;
    }
    if (uint8_t* d = g_optionsWindow) Window_SendEvent5(d, 0, 0x6f, F<uint16_t>(sl, 4) + 1u, (uint32_t)(uintptr_t)text, 0, 0);
    Options_Save(g_app, 0, 1);
}
RT3_REPLACE(0x4fede0, OptionsDialog_OnSlider);

 // namespace

// 0x4ff510 (event handler). The options dialog d. Idle closes it when
// 0x6d1360 asks; key presses (3) rebind the hotkey picked in the list 0x757a
// (not Enter / Escape) and save the keys; buttons 0x7531 (close), 0x7552
// (sound test), 0x757b (default keys). Check boxes (0xca, 0x7544..0x759d)
// toggle game options (0x6cec74, saved) or engine settings (0x6d4024, saved
// to its config) and apply them: the sound provider (RSX / DirectSound3D),
// full screen (the mode controls enabled only in full screen), hardware
// rendering (first use asks with 0xe16, unsupported 0xe17 / 0xe93), coast
// rebuild, cursor kinds (at most once a second), detail 0xc, the overview
// and lighting options and the train ghosts; 0x759d warns once (0xfb9).
int32_t __fastcall OptionsDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (msg[0] == 0xc9) return 1;
    uint8_t* e = g_engine;
    auto opts = []() { return g_app; };
    auto save = [&]() {
        Options_Save(opts(), 0, 1);
        return 0;
    };
    auto toggle = [&](int32_t off) {
        F<int32_t>(opts(), off) = F<int32_t>(opts(), off) == 0;
        return save();
    };
    auto saveEngine = []() {
        Engine_SaveConfig(g_engine);
        return 0;
    };
    auto flipByte = [&](int32_t off) {
        e[off] = e[off] == 0;
        return saveEngine();
    };
    auto flipInt = [&](int32_t off) {
        F<int32_t>(e, off) = F<int32_t>(e, off) == 0;
        return saveEngine();
    };
    auto send = [&](uint32_t type, uint32_t id, uint32_t a) { Window_SendEvent5(d, 0, type, id, a, 0, 0); };
    switch (msg[0]) {
    case 0:
        if (g_6d1360) {
            g_6d1360 = 0;
            msg[0] = 0xc9;
        }
        return 1;
    case 3: {
        if (!Widget_IsVisible(Widget_FindChild(d, 0, 0x757a))) return 1;
        if (F<int32_t>(Widget_FindChild(d, 0, 0x757a), 0x1d6) <= -1) return 1;
        uint8_t* model = (uint8_t*)Collection_Get(g_6d1354, 0, TreeView_SelectedData(Widget_FindChild(d, 0, 0x757a)));
        if (!LookupSingleFlag((int32_t)msg[2], (int32_t)msg[8])) return 1;
        if (msg[2] != 0xd && msg[2] != 0x1b) {
            const uint32_t flags = RepackFlags(msg[8]);
            if (!((int32_t(__fastcall*)(int32_t, uint32_t, const char*))(void*)&Models_AddCopy)((int32_t)msg[2], flags, (const char*)model + 0x88))
                return 0;
            Hotkeys_Save((const char*)0x5cbc84);
        }
        OptionsDialog_FillModels(d, 0, 1);
        return 0;
    }
    case 0xcb:
        switch (msg[1]) {
        case 0x7531:
            msg[0] = 0xc9;
            return 1;
        case 0x7552:
            Ui_PlaySound(0x2d, 0, 0x3f000000);
            return 0;
        case 0x757b:
            Hotkeys_Init(0);
            Hotkeys_Save((const char*)0x5cbc84);
            OptionsDialog_FillModels(d, 0, 1);
            return 0;
        }
        return 1;
    case 0xca:
        break;
    default:
        return 1;
    }
    const uint32_t id = msg[1];
    switch (id) {
    case 0x7544: return toggle(0xa8);
    case 0x7545: return toggle(0xf8);
    case 0x7547: return flipByte(0x1146d7);
    case 0x7550: {
        uint8_t* snd = g_sound;
        const uint32_t fl = Sound_ProviderFlagsAt(snd, 0, Sound_ActiveProviderIndex(snd));
        Sound_Restart(g_sound, 0, fl ? (const char*)0x5d14f8 : (const char*)0x5d14d8);
        send(0x66, 0x7551, (uint32_t)Sound_ActiveProviderIndex(g_sound));
        Game_RestartSounds();
        return save();
    }
    case 0x7554: {
        Options_ToggleFullscreen();
        Engine_SaveConfig(g_engine);
        OptionsDialog_FillResolutions(d);
        static const uint32_t kModes[5] = { 0x7555, 0x7578, 0x7558, 0x7559, 0x7579 };
        if (g_engine[0x114226]) {
            send(0x65, 0x7555, 3);
            send(0x65, 0x7578, 3);
            send(0x74, 0x7555, (uint32_t)Engine_Get1146ae(g_engine));
            send(0x74, 0x7578, (uint32_t)Engine_Get11470e(g_engine));
            for (int k = 2; k < 5; ++k) send(0x65, kModes[k], 3);
        } else {
            for (int k = 0; k < 5; ++k) send(0x87, kModes[k], 3);
        }
        return 0;
    }
    case 0x7555: {
        if (Engine_Get1146ae(e)) {
            Engine_SetHardwareOption(g_engine, 0, 0);
            return saveEngine();
        }
        if (!Engine_ModeHasFlags(g_engine)) {
            send(0x74, 0x7555, 0);
            Message478(0xe17, 1);
            return 0;
        }
        if (!opts()[0x2d1]) {
            opts()[0x2d1] = 1;
            Options_Save(opts(), 0, 1);
            if (Message478(0xe16, 3) != 0x3f2) {
                send(0x74, 0x7555, 0);
                return 0;
            }
        }
        Engine_SetHardwareOption(g_engine, 0, 1);
        Engine_SaveConfig(g_engine);
        if (!Engine_Get1146ae(g_engine)) {
            send(0x74, 0x7555, 0);
            Message478(0xe93, 1);
        }
        return 0;
    }
    case 0x7556: return flipInt(0x114206);
    case 0x755c:
        e[0x1146eb] = e[0x1146eb] == 0;
        if (g_map && !g_engine[0x1146eb])
            Map_RebuildCoast(g_map, 0, 0, 0, 0x270f, 0x270f, nullptr);
        return saveEngine();
    case 0x755d: return flipByte(0x1146e9);
    case 0x755e: return flipInt(0x114697);
    case 0x7568: return flipByte(0x1146b8);
    case 0x7575:
        // 0.5 and 0 alternate (anything else, NaN too, becomes 0.5).
        F<uint32_t>(e, 0x1146af) = (double)F<float>(e, 0x1146af) == D(0x5c8628) ? 0 : 0x3f000000;
        return saveEngine();
    case 0x7576: return flipByte(0x1146a8);
    case 0x7577: return flipByte(0x1146e3);
    case 0x7578:
        Engine_SetDisplayOption(g_engine, 0, Engine_Get11470e(e) == 0 ? 1 : 0);
        return saveEngine();
    case 0x757c:
    case 0x759a: {
        if (ClockMs() < g_6d136c) return 0;
        g_6d136c = ClockMs() + 1000;
        uint8_t* c = g_cursorManager;
        if (id == 0x757c) {
            if (Obj_Busy18(c))
                Cursor_UseSystem(g_cursorManager);
            else
                Cursor_Reapply(g_cursorManager);
            Options_Save(opts(), 0, 1);
            OptionsDialog_ShowVideo(d);
        } else {
            if (Obj_Zero18And14(c))
                Cursor_UseGameDrawn(g_cursorManager);
            else
                Cursor_UseWindows(g_cursorManager);
            Options_Save(opts(), 0, 1);
        }
        g_6d136c = ClockMs() + 0x3c;
        return 0;
    }
    case 0x7584:
    case 0x7585:
        F<int32_t>(opts(), 0x120) = id == 0x7584;
        return save();
    case 0x7586:
        Options_SetDetail(opts(), 0, 0xc, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0xcc) != 0, 1);
        return saveEngine();
    case 0x7589: return toggle(0x247);
    case 0x758a: return toggle(0x243);
    case 0x758b: return toggle(0x24b);
    case 0x758c: return toggle(0x26b);
    case 0x758d: return toggle(0x26f);
    case 0x758e: return toggle(0x273);
    case 0x758f: return toggle(0x277);
    case 0x7590: return toggle(0x27b);
    case 0x7592: {
        F<int32_t>(opts(), 0x263) = F<int32_t>(opts(), 0x263) == 0;
        uint8_t* view = g_gameWindow;
        if (view && F<int32_t>(view, 0x8c) == 0xc) {
            uint8_t* o = g_map;
            F<int32_t>(o, 0x168d) = F<int32_t>(opts(), 0x263) ? F<int32_t>(o, 0x1689) + 1 : F<int32_t>(o, 0x1689);
        }
        return save();
    }
    case 0x7593:
        toggle(0x283);
        if (uint8_t* view = g_gameWindow) Window_EnableControl7f9(view, 0, F<int32_t>(opts(), 0x283) != 0);
        return 0;
    case 0x7594: return toggle(0x28b);
    case 0x7595:
        toggle(0x2c3);
        Lighting_Rebuild(opts());
        if (uint8_t* view = g_gameWindow) OptionsDialog_SetLightingSet(view, 0, (uint8_t)F<int32_t>(opts(), 0x2c3), 0);
        return 0;
    case 0x7599: {
        F<int32_t>(opts(), 0x2cc) = F<int32_t>(opts(), 0x2cc) == 0;
        if (uint8_t* o = g_map) {
            if (!F<int32_t>(opts(), 0x2cc) && F<int32_t>(o, 0x2175) == 0x15) {
                Overview_EndPlain(o);
                Overview_Close(g_map);
                o = g_map;
            }
            if (F<int32_t>(opts(), 0x2cc) && !F<int32_t>(o, 0x2175) && F<int32_t>(g_gameWindow, 0x8c) == 0xc) {
                Overview_EndPlain(o);
                Overview_SetMode(g_map, 0, 0x15, 0, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
            }
        }
        return save();
    }
    case 0x759b:
        F<int32_t>(e, 0x114742) = F<int32_t>(e, 0x114742) == 0;
        if (g_trains) Trains_ReapplyGhosts(g_trains);
        return save();
    case 0x759c: {
        F<int32_t>(opts(), 0x309) = F<int32_t>(opts(), 0x309) == 0;
        const int32_t on = F<int32_t>(opts(), 0x309);
        if (hook::As<int32_t>(g_trackTool) && hook::As<uint8_t*>(g_tracks)) {
            if (on)
                Camera_BeginA0(hook::As<uint8_t*>(g_tracks));
            else
                Obj_DecA0(hook::As<uint8_t*>(g_tracks));
        }
        return save();
    }
    case 0x759d:
        F<int32_t>(e, 0x11474a) = F<int32_t>(e, 0x11474a) == 0;
        Options_Save(opts(), 0, 1);
        if (!g_6d1368) {
            g_6d1368 = 1;
            Message478(0xfb9, 1);
        }
        return 0;
    default:
        return 1;
    }
}
RT3_REPLACE(0x4ff510, OptionsDialog_HandleEvent);

// 0x500410. Options dialog w: shows page `page` (tabs 0x7533.., pages 11 / 12
// trade tab buttons; 0x7540 marks page 11) and fills it, with 0x6d135d set
// while it does (the UI busy count drops unless idle).
RT3_NOINLINE void __fastcall OptionsDialog_ShowPage(uint8_t* w, int, int32_t page) {
    g_6d135d = 1;
    if (!Obj_Idlec60(g_windowManager)) Obj_Decc60(g_windowManager);
    int32_t tab = page == 0xc ? 0xb : page == 0xb ? 0xd : page;
    for (int32_t i = 0; i < 0xd; ++i) Window_SendEvent5(w, 0, i != tab ? 0x86 : 0x6d, 0x7533 + i, 3, 0, 0);
    Window_SendEvent5(w, 0, page != 0xb ? 0x86 : 0x6d, 0x7540, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x86, 0x753f, 3, 0, 0);
    typedef void(__fastcall * Fill)(uint8_t*);
    static const Fill pages[] = { OptionsDialog_ShowGeneral, OptionsDialog_ShowSound, OptionsDialog_ShowVideo, OptionsDialog_FillList7569,
                                  OptionsDialog_FillList755f, OptionsDialog_ShowEngine, OptionsDialog_ShowDisplay, nullptr,
                                  OptionsDialog_ShowValues, OptionsDialog_ShowGraphics, OptionsDialog_ShowSwitches, Window_FillList759e,
                                  OptionsDialog_ShowLists7587 };
    if ((uint32_t)page <= 0xc) {
        if (page == 7) OptionsDialog_FillModels(w, 0, 0);
        else pages[page](w);
    }
    g_6d135d = 0;
}
RT3_REPLACE(0x500410, OptionsDialog_ShowPage);

void __fastcall Options_ShowPageCb(int32_t v) {
    if (v != -1 && !g_6d135d && g_optionsWindow) OptionsDialog_ShowPage(g_optionsWindow, 0, v);
}
RT3_REPLACE(0x500620, Options_ShowPageCb);

// 0x500640. Settings window constructor ("SettingsWindow.win", vtable
// 0x5d14a4). Font 1 goes to 0x6d1358. The tab list 0x7532 gets the 13 entries
// of the table at 0x622870 ({text, depth}; the 13th only when 0x6cec7c +0x82
// is set). A nested entry names its parent: the last entry one level up,
// searched back from the previous parent. Then come the control handlers, the
// "Setup_Options_Buttons" button 0x759d, and five settings rows. Each row is
// an edit box caption plus a drop-down tree view (0x759e..0x75a2) with its
// handler, and the last two rows also get their choices and current value.
RT3_NOINLINE uint8_t* __fastcall SettingsWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d14a4;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1530);
    g_6d1358 = Pack_AcquireFont(g_pack, 0, 1);
    int32_t prev = 0;
    for (int32_t i = 0; i < 0xd; ++i) {
        if (i == 0xc && g_newGame[0x82] == 0) continue;
        uint8_t item[0x20];
        memset(item, 0, sizeof item);
        String_Assign(&F<char*>(item, 0), GetString(hook::Global<uint32_t>(0x622870 + i * 8)));
        int32_t depth = hook::Global<int32_t>(0x622874 + i * 8);
        if (depth == 0)
            prev = i;
        else {
            F<int32_t>(item, 0x10) = 1;
            if (depth != hook::Global<int32_t>(0x622874 + prev * 8) + 1) {
                int32_t j = i - 1;
                int32_t d;
                do {
                    d = hook::Global<int32_t>(0x622874 + j * 8);
                    prev = j--;
                } while (depth != d + 1);
            }
            F<int32_t>(item, 0x14) = prev;
        }
        Window_SendEvent5(w, 0, 0x89, 0x7532, (uint32_t)(uintptr_t)item, 0, 0);
        Free(F<void*>(item, 0));
    }
    Window_SendEvent5(w, 0, 0x73, 0x7596, 5, 0, 0);
    static const uint32_t handlers[][2] = {
        { 0x7532, 0x500620 }, { 0x7546, 0x4fd100 }, { 0x7548, 0x4fd130 }, { 0x7591, 0x4fd160 }, { 0x755a, 0x4fd1d0 },
        { 0x755b, 0x4fd200 }, { 0x7574, 0x4fd230 }, { 0x7553, 0x4fe270 }, { 0x7557, 0x4fe2d0 }, { 0x755f, 0x4fe3b0 },
        { 0x7569, 0x4fe400 }, { 0x754f, 0x4fd190 }, { 0x7551, 0x4fe1b0 }, { 0x7587, 0x4fd260 }, { 0x7588, 0x4fd3d0 },
        { 0x7573, 0x4fd2c0 },
    };
    for (auto& h : handlers) Window_SendEvent5(w, 0, 0x79, h[0], h[1], 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x7532, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x757a, 0x4fd010, 0, 0);
    for (uint32_t id = 0x7549; id < 0x754f; id += 2) Window_SendEvent5(w, 0, 0x84, id, 0x4fede0, 0, 0);
    static const uint32_t sliders[] = { 0x7558, 0x7566, 0x7560, 0x7562, 0x7564, 0x756e, 0x756a, 0x756c, 0x7570, 0x757d, 0x757f, 0x7581, 0x7596 };
    for (uint32_t id : sliders) Window_SendEvent5(w, 0, 0x84, id, 0x4fede0, 0, 0);
    {
        uint8_t b[0xa1];
        memset(b, 0, sizeof b);
        F<uint32_t>(b, 7) = 0x3f19c28f;
        F<uint32_t>(b, 0xb) = 0x3ea88889;
        F<uint32_t>(b, 0xf) = 0x3e19999a;
        F<uint32_t>(b, 0x13) = 0x3d792c60;
        F<uint32_t>(b, 0x75) = 0x3d147ae1;
        F<uint32_t>(b, 0x79) = 0;
        F<uint32_t>(b, 0x7d) = 0x3de8f5c3;
        F<uint32_t>(b, 0x81) = 0x3d792c60;
        b[0x33] = 2;
        b[0x27] = 2;
        F<uint16_t>(b, 0) = 0x759d;
        F<uint16_t>(b, 5) = 0x7535;
        F<uint32_t>(b, 0x63) = 0x3c4ccccd;
        String_Assign(&F<char*>(b, 0x43), (const char*)0x5d0ae0);
        F<int32_t>(b, 0x6b) = 5;
        F<int32_t>(b, 0x67) = 6;
        String_Assign(&F<char*>(b, 0x6f), GetString(0xfb7));
        String_Assign(&F<char*>(b, 0x3f), GetString(0xfb8));
        if (g_language == 4) {
            F<float>(b, 0xf) = (float)((double)F<float>(b, 0xf) + D(0x5d1528));
            F<float>(b, 0x7d) = (float)((double)F<float>(b, 0x7d) + D(0x5d1528));
        }
        uint8_t* c = (uint8_t*)ExeNew(0xe8);
        c = c ? Button_Construct(c, 0, b) : nullptr;
        Window_AddChild(w, 0, c, 0);
        Free(F<void*>(b, 0x43));
        Free(F<void*>(b, 0x6f));
        Free(F<void*>(b, 0x3f));
    }
    Window_SendEvent5(w, 0, 0x74, 0x759d, F<uint32_t>(g_engine, 0x11474a), 0, 0);
    Window_SendEvent5(w, 0, 0xb2, 0x7554, 0x2c7, 0, 0);
    Window_SendEvent5(w, 0, 0xa1, 0x7532, 0x8000, 0, 0);
    // One settings row: caption (text, tooltip) at height `labelY`, then the
    // drop-down `id` at `treeY` under the same tooltip, and its handler.
    auto row = [&](uint16_t parent, uint32_t labelY, uint32_t text, uint32_t tip, uint16_t id, uint32_t treeY, uint32_t handler) {
        {
            uint8_t e[0x91];
            memset(e, 0, sizeof e);
            F<uint32_t>(e, 7) = 0x3ee51eb8;
            F<uint32_t>(e, 0xb) = labelY;
            F<uint32_t>(e, 0xf) = 0x3ea3d70a;
            F<uint32_t>(e, 0x13) = 0x3d088889;
            e[0x27] = 0;
            e[0x28] = 4;
            F<int32_t>(e, 0x34) = -1;
            String_Assign(&F<char*>(e, 0x77), GetString(text));
            String_Assign(&F<char*>(e, 0x3f), GetString(tip));
            e[0x33] = 2;
            F<uint16_t>(e, 5) = parent;
            uint8_t* c = (uint8_t*)ExeNew(0xf0);
            c = c ? EditBox_Construct(c, 0, e) : nullptr;
            Window_AddChild(w, 0, c, 0);
            Free(F<void*>(e, 0x77));
            Free(F<void*>(e, 0x3f));
        }
        uint8_t t[0x12f];
        memset(t, 0, sizeof t);
        static const uint32_t tmpl[][2] = {
            { 0x5f, 0xa }, { 0x63, 0x3b75c28f }, { 0x6b, 0x3e9570a4 }, { 0x6f, 0x3cf5c28f }, { 0x73, 0x3ada740e },
            { 0x7b, 0x3cfc9630 }, { 0x7f, 0x3e9258bf }, { 0x83, 0x3e99999a }, { 0x8b, 0x3e99eb85 }, { 0x93, 0x3e99eb85 },
            { 0x97, 0x3e9258bf }, { 0x9f, 1 }, { 0xa3, 1 }, { 0xc7, 1 }, { 0xcb, 2 }, { 0xcf, 3 }, { 0xd3, 4 }, { 0xd7, 5 },
            { 0xe3, 1 }, { 0xe7, 0xb }, { 0xeb, 2 }, { 0xef, 3 }, { 0xf3, 7 }, { 0xf7, 8 }, { 0xfb, 9 }, { 0xff, 0xa },
            { 0x103, 0x3e99999a }, { 0x107, 0x3ba3d70a }, { 0x10b, 0x3b75c28f }, { 0x10f, 0x3bda740e }, { 0x113, 0x3e9570a4 },
            { 0x117, 0x3cf5c28f },
        };
        for (auto& f : tmpl) F<uint32_t>(t, f[0]) = f[1];
        F<uint32_t>(t, 7) = 0x3ee51eb8;
        F<uint32_t>(t, 0xb) = treeY;
        F<uint32_t>(t, 0xf) = 0x3ea3d70a;
        F<uint32_t>(t, 0x13) = 0x3d2aaaab;
        t[0x33] = 2;
        F<uint16_t>(t, 0) = id;
        F<uint16_t>(t, 5) = parent;
        t[0x3e] = 1;
        String_Assign(&F<char*>(t, 0x3f), GetString(tip));
        String_Assign(&F<char*>(t, 0x43), (const char*)0x5d1514);
        uint8_t* c = (uint8_t*)ExeNew(0x1ea);
        c = c ? TreeView_Construct(c, 0, t) : nullptr;
        Window_AddChild(w, 0, c, 0);
        Free(F<void*>(t, 0x43));
        Free(F<void*>(t, 0x3f));
        Window_SendEvent5(w, 0, 0x79, id, handler, 0, 0);
    };
    row(0x7540, 0x3e222222, 0xfe6, 0xfe7, 0x759e, 0x3e488889, 0x4fd4a0);
    row(0x753c, 0x3ecccccd, 0xfe8, 0xfe9, 0x759f, 0x3ee00000, 0x4fd4f0);
    row(0x753c, 0x3ef77777, 0xfea, 0xfeb, 0x75a0, 0x3f055555, 0x4fd520);
    for (int32_t id = 0x7593; id <= 0x7594; ++id) {
        uint8_t* c = Widget_FindChild(w, 0, id);
        F<float>(c, 0xf) = (float)((double)F<float>(c, 0xf) - Fl(0x5d09ac));
    }
    row(0x7540, 0x3e7f258c, 0x10db, 0x10dc, 0x75a1, 0x3e92c5f9, 0x4fd430);
    for (uint32_t s = 0x10df; s <= 0x10e9; ++s) Window_SendEvent5(w, 0, 0x88, 0x75a1, (uint32_t)(uintptr_t)GetString(s), 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x75a1, F<uint32_t>(g_app, 0x31d), 0, 0);
    row(0x7540, 0x3eae147b, 0x10dd, 0x10de, 0x75a2, 0x3ec147ae, 0x4fd460);
    for (uint32_t s = 0x10ea; s <= 0x10f4; ++s) Window_SendEvent5(w, 0, 0x88, 0x75a2, (uint32_t)(uintptr_t)GetString(s), 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x75a2, F<uint32_t>(g_app, 0x319), 0, 0);
    return w;
}
RT3_REPLACE(0x500640, SettingsWindow_Construct);

// 0x501e50. Opens the options dialog (0x6d1350) modally on page 2, with the
// game paused, then reloads the capabilities (0x62be68) and destroys it.
RT3_NOINLINE void OptionsDialog_Open() {
    if (g_optionsWindow) return;
    g_6d1360 = 0;
    uint8_t* o = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x78);
    g_optionsWindow = o ? SettingsWindow_Construct(o) : nullptr;
    Window_SendEvent5(g_optionsWindow, 0, 0x66, 0x7532, 2, 0, 0);
    OptionsDialog_ShowPage(g_optionsWindow, 0, 2);
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_optionsWindow, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    if (g_62be68) Caps_Load(g_62be68);
    Windows_Remove(g_windowManager, 0, g_optionsWindow);
    if (uint8_t* w = g_optionsWindow) {
        SettingsWindow_Destruct(w);
        ((void(__cdecl*)(void*))0x53b080)(w);
    }
    g_optionsWindow = nullptr;
}
RT3_REPLACE(0x501e50, OptionsDialog_Open);

// 0x501f20. Kind in EAX, target in EDI.
__declspec(naked) void Shell_OpenCommand() {
    __asm {
        push edi
        push eax
        call Shell_OpenCommandImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x501f20, Shell_OpenCommand);

}  // namespace rt3
