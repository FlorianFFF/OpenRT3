// RulesDialog.cpp - RT3.exe 0x4d2000-0x4dc000.
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
#include "core/ptrlist.h"
#include "hook/keepregs.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall StrCopyN(char* dst, const char* src, size_t n);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Widget_SetVtbl(uint8_t* w);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Table_CountField4(Collection* t);
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v);
uint8_t __fastcall Obj_ListFlags(void* o, int, int32_t i, int32_t classify);
int32_t __fastcall Table_CountPositive3d(Collection* t);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
uint8_t __fastcall Load_CargoFlags(void* o);
void __fastcall Obj_SetTextE0(uint8_t* o, int, const char* s);
int32_t __fastcall Record_AnyKind1(uint8_t* r);
int32_t __fastcall Record_AnyKind2(uint8_t* r);
int32_t __fastcall Record_AnyKind4(uint8_t* r);
int32_t __fastcall Record_AnyKind7(uint8_t* r);
void __fastcall RulesDialog_UpdateButton(uint8_t* dlg);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Window_HighlightViewModes(uint8_t* w);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall RulesDialog_SwitchPage(uint8_t* w);
void __fastcall RulesDialog_ShowMessage(uint8_t* w);
void __fastcall RulesDialog_UpdateScope(uint8_t* w);
void __fastcall RulesDialog_ShowSlot(uint8_t* w);
void __fastcall RulesDialog_FillConditions(uint8_t* w);
int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
void __fastcall RulesDialog_FillList(uint8_t* w);
void CityDialog_ShowMapParameters();
void __fastcall RulesDialog_FillTerritories(uint8_t* w);
void __fastcall RulesDialog_Reset(uint8_t* w);
int32_t __fastcall Obj_YearReached(void* o);
void __fastcall Cargo_IndustryTotals(uint8_t* o, int, float* base, float* share, float* in, float* inShare);
float __fastcall Obj_Ratio76(uint8_t* o);
int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w);
int32_t __fastcall Dialog_SetupControl5141(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w);
void __fastcall RulesDialog_Init(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w);
void __fastcall String_Assign(char** s, const char* src);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
uint8_t* __fastcall RulesDialog_Construct(uint8_t* w, int, uint32_t a, int32_t* rule);
void __fastcall Widget4d4840_Destruct(uint8_t* w);
uint8_t* __fastcall ConditionDialog_Construct(uint8_t* w, int, uint8_t* r);
void __fastcall Widget4d5b90_Destruct(uint8_t* w);
void __fastcall List1a_Free(uint8_t** head);
void __fastcall RulesDialog_FillTriggers(uint8_t* w);
void __fastcall RulesDialog_UpdateSlot(uint8_t* w);
void __fastcall RulesDialog_Refresh(uint8_t* w);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key);
void __fastcall Record_CopyFrom(uint8_t* dst, int, uint8_t* src);
void __fastcall RulesDialog_MoveRuleUp(uint8_t* w);
void __fastcall RulesDialog_MoveRuleDown(uint8_t* w);
void __fastcall RulesDialog_DeleteRule(uint8_t* dlg);
void __fastcall RulesDialog_ShowMessageKind(uint8_t* w);
void __fastcall RulesDialog_DeleteTrigger(uint8_t* w);
void __fastcall RulesDialog_DeleteCondition(uint8_t* w);
void __fastcall RulesDialog_Store(uint8_t* dlg);
void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall RulesDialog_ShowEffectType(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w);
RT3_NOINLINE void __fastcall ConditionDialog_ShowValue(uint8_t* w);
RT3_NOINLINE int32_t __fastcall ConditionDialog_Store(uint8_t* w);
int32_t __fastcall ConditionDialog_Store(uint8_t* w);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall ConditionDialog_ShowValue(uint8_t* w);
void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w);
void __fastcall RulesDialog_FillConditionTypes(uint8_t* w, int, int32_t sel);
uint32_t __fastcall EventEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall RuleDialog_StoreCondition(uint8_t* d);
void __fastcall RulesDialog_ShowEffect(uint8_t* w);
void __fastcall RulesDialog_ShowEffectType(uint8_t* w);
void __fastcall RulesDialog_FillEffectTypes(uint8_t* w, int, int32_t sel);
int32_t __fastcall Scenario_CheckSetup(uint32_t* s, int, char* out);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall Scenario_CheckSetup(uint32_t* s, int, char* out);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
RT3_NOINLINE void __fastcall BuildingCounts_DrawTable(void*, int, float x, float y, float, float, int32_t);
void __fastcall CargoStats_Draw(void*, int, float x, float y, float, float, int32_t);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetupControl5141(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w);
RT3_NOINLINE void __fastcall ScenarioVars_Draw(void*, int, float x, float y, float, float, int32_t);
RT3_NOINLINE void __fastcall Players_DrawTable(void*, int, float x, float y, float, float h, int32_t);
void __fastcall ScenarioRecords_Draw(void* self, int edx, float x, float y, float w, float h, int32_t a5);
void __fastcall ScenarioCheck_DrawCb(void*, int, float x, float y, float, float, int32_t);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w);
int32_t Global6d07b4Set();
void __fastcall Window_HighlightViewModes(uint8_t* w);
int32_t __fastcall ReportMenu_HandleEvent(void*, int, uint32_t* msg);
void ViewModesPanel_Close();
void Window_HighlightViewModesIfOpen();
void EditorPanel_Open();
RT3_NOINLINE void __fastcall RulesDialog_ShowEffect(uint8_t* w);
void __fastcall Widget4d4840_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_FillEffectTypes(uint8_t* w, int, int32_t sel);
RT3_NOINLINE void __fastcall RulesDialog_ShowEffectType(uint8_t* w);
RT3_NOINLINE int32_t __fastcall RuleDialog_StoreCondition(uint8_t* d);
void __fastcall Rules_SetEffectType(int32_t v, int32_t value);
void __fastcall Set6d07b8_7c(void*, int32_t v);
uint8_t* __fastcall RulesDialog_Construct(uint8_t* w, int, uint32_t a, int32_t* rule);
int32_t __fastcall RuleDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
void __fastcall Widget4d5b90_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_ShowLimits(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_FillConditionTypes(uint8_t* w, int, int32_t sel);
RT3_NOINLINE void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w);
RT3_NOINLINE void __fastcall ConditionDialog_ShowValue(uint8_t* w);
RT3_NOINLINE int32_t __fastcall ConditionDialog_Store(uint8_t* w);
void __fastcall Condition_SetValue(int32_t v, int32_t value);
uint8_t* __fastcall ConditionDialog_Construct(uint8_t* w, int, uint8_t* r);
int32_t __fastcall ConditionDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
void __fastcall TextTag_Pick(void*, char* out);
RT3_NOINLINE void CityDialog_ShowMapParameters();
void __fastcall RulesDialog_UpdateButtons4fed(uint8_t* dlg);
void __fastcall Obj_SetTextE0(uint8_t* o, int, const char* s);
void __fastcall ConditionDialog_SetValueCb(void*, int32_t v);
void __fastcall Set6cff84_8c(void*, int32_t v);
void __fastcall Set6cff84_94(int32_t v);
void __fastcall Set6cff84_98(int32_t v);
void __fastcall ScenarioCombos_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall RulesDialog_FillList(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_FillTerritories(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_FillTriggers(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_FillConditions(uint8_t* w);
void __fastcall RulesDialog_Store(uint8_t* dlg);
RT3_NOINLINE int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
void __fastcall RulesDialog_DeleteRule(uint8_t* dlg);
void __fastcall RulesDialog_ShowMessageKind(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_UpdateScope(uint8_t* w);
void __fastcall RulesDialog_UpdateButton(uint8_t* dlg);
RT3_NOINLINE uint32_t __stdcall RulesDialog_Run(uint32_t a, int32_t* rule);
RT3_NOINLINE uint32_t __stdcall ConditionDialog_Run(uint8_t* r);
void __fastcall RulesDialog_DeleteTrigger(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_ShowMessage(uint8_t* w);
void __fastcall RulesDialog_UpdateSlot(uint8_t* w);
RT3_NOINLINE void __fastcall EventEditor_AddRule(uint8_t* w);
RT3_NOINLINE void __fastcall EventEditor_EditRule(uint8_t* w);
RT3_NOINLINE void __fastcall EventEditor_AddCondition(uint8_t* w);
RT3_NOINLINE void __fastcall EventEditor_EditCondition(uint8_t* w);
void __fastcall RulesDialog_DeleteCondition(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_ShowSlot(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_SwitchPage(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_Reset(uint8_t* w);
RT3_NOINLINE void __fastcall RulesDialog_Refresh(uint8_t* w);
void __fastcall RulesDialog_MoveRuleUp(uint8_t* w);
void __fastcall RulesDialog_MoveRuleDown(uint8_t* w);
void __fastcall RulesDialog_SelectCb(void*, int32_t v);
RT3_NOINLINE void __fastcall RulesDialog_Init(uint8_t* w);
RT3_NOINLINE void __fastcall EventEditor_NewEvent(uint8_t* w, int, int32_t duplicate);
RT3_NOINLINE void __fastcall EventEditor_Rename(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall EventEditor_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE int32_t __fastcall Widget_IsVisible(uint8_t* w);

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

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// Closes the panel held in `slot`: out of the window manager (0x6d401c), back to
// the base vtable and deleted.
static void Panel_Close(uint32_t slot, uint32_t vtbl, bool clearFirst) {
    uint8_t*& p = hook::Global<uint8_t*>(slot);
    if (!p) return;
    Windows_Remove(hook::Global<uint8_t*>(0x6d401c), 0, p);
    if (uint8_t* w = p) {
        F<uint32_t>(w, 0) = vtbl;
        if (clearFirst) p = nullptr;
        Widget_SetVtbl(w);
        ExeDelete(w);
    }
    p = nullptr;
}

// Opens a pack-defined panel once: window of `size` bytes with vtable `vtbl`,
// loaded from `name` at (0, 0), kept in `slot` and inserted into the window
// manager (the insert's select flag `select`).
static uint8_t* Panel_Open(uint32_t slot, size_t size, uint32_t vtbl, const char* name, int32_t select, bool highlight) {
    if (hook::Global<uint8_t*>(slot)) return nullptr;
    uint8_t* w = (uint8_t*)ExeNew(size);
    if (w) {
        Widget_SetVtblRet(w);
        F<uint32_t>(w, 0) = vtbl;
        Window_Load(w, 0, 0.0f, 0.0f, name);
        if (highlight) Window_HighlightViewModes(w);
    }
    hook::Global<uint8_t*>(slot) = w;
    WidgetList_Insert(hook::Global<uint8_t**>(0x6d401c), 0, w, select);
    return w;
}

// 0x4db520 / 0x4db5e0. Moves the selected rule (+0x84) one place up / down in
// the rule order (list 2 of 0x62be18; +0x90 rules), storing the edits first,
// refilling the list and keeping the selection on it.
static void RulesDialog_MoveRule(uint8_t* w, int32_t dir) {
    if (F<int32_t>(w, 0x84) == -1) return;
    Collection* rules = hook::Global<Collection*>(0x62be18);
    Collection_Get(rules, 0, F<uint32_t>(w, 0x84));
    uint32_t pos = Collection_PositionOf(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84), 2);
    if (dir < 0 ? pos == 0 : (int32_t)pos == F<int32_t>(w, 0x90) - 1) return;
    RulesDialog_StoreRule(w);
    rules = hook::Global<Collection*>(0x62be18);
    uint8_t* other = (uint8_t*)Collection_Get(rules, 0, Collection_IdAt(rules, 0, pos + dir, 2));
    Collection_SwapOrder(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84), F<uint32_t>(other, 0x14), 2);
    int32_t sel = F<int32_t>(w, 0x84);
    RulesDialog_FillList(w);
    F<int32_t>(w, 0x84) = sel;
    RulesDialog_Refresh(w);
    Window_SendEvent5(w, 0, 0x66, 0x4e84, Collection_PositionOf(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84), 2), 0, 0);
}

// Runs dialog w (game paused) and frees it with `destruct`; returns its result.
static uint32_t Dialog_RunAndFree(uint8_t* w, void(__fastcall* destruct)(uint8_t*)) {
    PauseLock_Push(hook::Global<int32_t*>(0x6cec74));
    uint32_t r = Ui_RunModal(hook::Global<uint8_t**>(0x6d401c), 0, w, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::Global<int32_t*>(0x6cec74));
    Windows_Remove(hook::Global<uint8_t*>(0x6d401c), 0, w);
    if (w) {
        destruct(w);
        ((void(__cdecl*)(void*))0x53b080)(w);
    }
    return r;
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

static inline uint8_t** EventEditor_Event(uint8_t* w) {
    return (uint8_t**)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
}

// Appends node n to the list at *head linked through +next.
static inline void EventEditor_Append(uint8_t** head, uint8_t* n, uint32_t next) {
    uint8_t* last = nullptr;
    for (uint8_t* p = *head; p; p = F<uint8_t*>(p, next)) last = p;
    if (last) F<uint8_t*>(last, next) = n;
    else *head = n;
}

// 0x4d2430. The building count table (two columns, wrapping to a new column
// block below 0x5c8888): per building type of 0x62b268 that is placeable (no
// +0x794, kind not 1 / 4 / 5) its name and how many exist in 0x62b26c (types
// sharing the +0xba / +0xbb group count together; none shows "n/a" 0xe4 or,
// before the type's year +0x26, string 0x44a, greyed), then five totals by
// kind (0x40f / 0x410 / 0x44b / 0x44c / 0x44d).
RT3_NOINLINE void __fastcall BuildingCounts_DrawTable(void*, int, float x, float y, float, float, int32_t) {
    uint8_t* font = hook::Global<int32_t>(0x6d4034) ? hook::Global<uint8_t*>(0x6cff8c) : hook::Global<uint8_t*>(0x6cff88);
    Collection* types = hook::Global<Collection*>(0x62b268);
    int32_t sumKind0 = 0, sumFlag78c = 0, sumUnused = 0, sumKind3 = 0;
    int32_t count = Collection_Count(types);
    float width[4];
    width[0] = 230.0f;
    width[1] = 80.0f;
    width[2] = (float)((double)y + D(0x5d0c68));
    width[3] = (float)((double)x + D(0x5d0c70));
    const int32_t heads[2] = { 0x449, 0xf };
    const int32_t labels[5] = { 0x40f, 0x410, 0x44b, 0x44c, 0x44d };
    float acc = 0.0f;
    char buf[0x3e8];
    for (int32_t c = 0; c < 2; ++c) {
        float colW = (float)(width[c] / D(0x5ca158));
        float rowX = width[3];
        int32_t align = (c != 0) + 2;
        Font_DrawTextBox(font, 0, GetString(heads[c]), (float)((double)rowX + acc), y, colW, BitsF(0x3cb17e4b), 0xffffffff, align, 0, 0, 1.0f);
        float rowY = width[2];
        float total = 0.0f;
        for (int32_t i = 0; i < count; ++i) {
            float nf = 0.0f;
            uint32_t id = Collection_IdAt(types, 0, i, 0);
            uint8_t* r = (uint8_t*)Collection_Get(types, 0, id);
            if (F<int32_t>(r, 0x794) > 0 || r[0x32] == 1 || r[0x32] == 4 || r[0x32] == 5) continue;
            int32_t n = 0;
            if (!r[0xba] && !r[0xbb])
                n = Table_CountField3d0(hook::Global<Collection*>(0x62b26c), 0, (int32_t)id);
            else
                for (int32_t j = 0; j < count; ++j) {
                    uint8_t* o = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, j, 0));
                    if ((o[0xba] && r[0xba]) || (o[0xbb] && r[0xbb])) n += Table_CountField3d0(hook::Global<Collection*>(0x62b26c), 0, F<int32_t>(o, 0xe4));
                }
            if (c == 0) {
                FormatText(buf, LocalizeName((const char*)(r + 0x4f)), 0, 0, 0, 0, 0, 0, 0, 0);
            } else {
                nf = (float)n;
                if (F<int32_t>(r, 0x78c)) sumFlag78c += n;
                else if (Obj_AllUnusedSomeUndefined(r)) sumUnused += n;
                else if (r[0x32] == 0) sumKind0 += n;
                else if (r[0x32] == 3) sumKind3 += n;
                total = (float)((double)nf + total);
                if (n) Num_Format(buf, 1, (double)Ftol((double)nf + D(0x5c85b8)), 10, 1, 0, 0);
                else if (!F<int32_t>(r, 0x7ac))
                    FormatText(buf, hook::Global<int32_t>(0x6d3c84) == 1 ? (const char*)0x5ca2b4 : GetString(0xe4), 0, 0, 0, 0, 0, 0, 0, 0);
                else if (F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) < F<uint16_t>(r, 0x26))
                    FormatText(buf, GetString(0x44a), 0, 0, 0, 0, 0, 0, 0, 0);
                else
                    Num_Format(buf, 1, (double)Ftol((double)nf + D(0x5c85b8)), 10, 1, 0, 0);
            }
            float w2 = (float)(((double)width[c] + Fl(0x5c9aa8)) / D(0x5ca158));
            int32_t indent = c ? 0x1e : 0;
            float tx = (float)((double)rowX + acc - (double)indent / D(0x5ca158));
            Font_DrawTextBox(font, 0, buf, tx, rowY, w2, BitsF(0x3cb17e4b), n ? 0xffffffff : 0xff9f9f9f, align, 0, 0, 1.0f);
            rowY = (float)((double)rowY + Fl(0x5d0c60));
            if (rowY > D(0x5c8888)) {
                rowX = (float)((double)rowX + D(0x5d0c58));
                rowY = y;
            }
        }
        rowY = (float)((double)rowY + D(0x5d0c50));
        for (int32_t k = 0; k < 5; ++k) {
            if (c == 0)
                FormatText(buf, GetString(labels[k]), 0, 0, 0, 0, 0, 0, 0, 0);
            else {
                int32_t v = k == 0 ? sumKind0 : k == 1 ? sumKind3 : k == 2 ? sumUnused : k == 3 ? sumFlag78c : Ftol((double)total + D(0x5c85b8));
                Num_Format(buf, 1, (double)v, 10, 1, 0, 0);
            }
            Font_DrawTextBox(font, 0, buf, (float)((double)rowX + acc), rowY, colW, BitsF(0x3cb17e4b), 0xffffffff, align, 0, 0, 1.0f);
            rowY = (float)((double)rowY + Fl(0x5d0c60));
            if (k == 3) rowY = (float)((double)rowY + D(0x5d0c50));
            if (rowY > D(0x5c8888)) {
                rowX = (float)((double)rowX + D(0x5d0c58));
                rowY = y;
            }
        }
        acc = (float)(((double)width[c] + Fl(0x5c85f8)) / D(0x5ca158) + acc);
    }
}
RT3_REPLACE(0x4d2430, BuildingCounts_DrawTable);

// 0x4d2a60. Cargo statistics table at (x, y): a yellow header row (strings 3,
// 0x44e..0x459, font 0x6cff90), then two groups (cargos without / with +0x46)
// of the available cargos, one row each with its name and 12 figures (the
// industry totals, +0x5a, +0x7e/+0x5e .. sums, +0x6a..+0x72 and the +0x76
// ratio; "%5.1f" up to column 8, thousands after) in font 0x6cff8c, a grey
// rule every 5 rows, and a summary row (0x44d: the sums, or averages for the
// money columns). Columns are 142 / 90 pixels of 1800.
void __fastcall CargoStats_Draw(void*, int, float x, float y, float, float, int32_t) {
    float x0 = (float)((double)x + D(0x5d0c70));
    float widths[13];
    widths[0] = BitsF(0x430e0000);
    for (int k = 1; k < 13; ++k) widths[k] = BitsF(0x42b40000);
    static const int32_t labels[13] = { 3, 0x44e, 0x44f, 0x450, 0x451, 0x452, 0x453, 0x454, 0x455, 0x456, 0x457, 0x458, 0x459 };
    float rowY = 0.0f;
    char text[0x200];
    for (int32_t g = 0; g < 2; ++g) {
        float cx = x0;
        float colY = g == 0 ? y : rowY;
        int32_t sep0 = g ? 5 : 0;
        for (int32_t i = 0; i < 13; ++i) {
            int32_t sep = sep0;
            rowY = colY;
            if (g == 0) {
                float cw = (float)((double)widths[i] / D(0x5ca158));
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, GetString(labels[i]), cx, rowY, cw, BitsF(0x3dcc1e09), 0xffffff00, (i != 0) + 2, 4, 0,
                                 1.0f);
                rowY = (float)((double)colY + D(0x5d0c80));
            }
            float sum = 0.0f;
            int32_t count = 0;
            for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x62ba8c)); ++j) {
                Collection* cs = hook::Global<Collection*>(0x62ba8c);
                uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)j, 0));
                if (!Obj_YearReached(c)) continue;
                if (g == 0 ? c[0x46] != 0 : c[0x46] == 0) continue;
                if (sep <= 0) {
                    sep = 5;
                    if (i == 0) {
                        float ly = (float)((double)rowY + D(0x5cab60));
                        Ui_Line(0xffaaaaaa, 0, cx, ly, BitsF(0x3f7a6666), ly, 1.0f);
                    }
                    rowY = (float)((double)rowY + D(0x5ca2d8));
                }
                --sep;
                if (i == 0) {
                    FormatText(text, LocalizeName((const char*)(c + 4)), 0, 0, 0, 0, 0, 0, 0, 0);
                } else {
                    float v = 0.0f;
                    if (i <= 4) {
                        float base, share, in, inShare;
                        Cargo_IndustryTotals(c, 0, &base, &share, &in, &inShare);
                        v = i == 1 ? base : i == 2 ? share : i == 3 ? in : inShare;
                    } else {
                        switch (i) {
                        case 5: v = F<float>(c, 0x5a); break;
                        case 6: v = (float)((double)F<float>(c, 0x7e) + F<float>(c, 0x5e)); break;
                        case 7: v = (float)((double)F<float>(c, 0x82) + F<float>(c, 0x62)); break;
                        case 8: v = (float)((double)F<float>(c, 0x86) + F<float>(c, 0x66)); break;
                        case 9: v = F<float>(c, 0x6a); break;
                        case 10: v = F<float>(c, 0x6e); break;
                        case 11: v = F<float>(c, 0x72); break;
                        case 12: v = Obj_Ratio76(c); break;
                        }
                    }
                    sum = (float)((double)v + sum);
                    ++count;
                    if (i <= 8) ExeSprintf()(text, (const char*)0x5d0104, (double)v);
                    else Num_FormatSimple(text, 9, (double)v / D(0x5c87f8), 0xb);
                }
                float cw = (float)((double)widths[i] / D(0x5ca158));
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, cx, rowY, cw, BitsF(0x3cb17e4b), 0xffffffff, (i != 0) + 2, 0, 0, 1.0f);
                rowY = (float)((double)rowY + Fl(0x5d0c60));
            }
            rowY = (float)((double)rowY + D(0x5d0c50));
            if (i == 0) FormatText(text, GetString(0x44d), 0, 0, 0, 0, 0, 0, 0, 0);
            else if (i <= 8) ExeSprintf()(text, (const char*)0x5d0104, (double)sum);
            else {
                float n = (float)count;
                Num_FormatSimple(text, 9, (double)sum / n / D(0x5c87f8), 0xb);
            }
            float cw = (float)((double)widths[i] / D(0x5ca158));
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, cx, rowY, cw, BitsF(0x3cb17e4b), 0xffffffff, (i != 0) + 2, 0, 0, 1.0f);
            cx = (float)(((double)widths[i] + Fl(0x5c85f8)) / D(0x5ca158) + cx);
        }
        rowY = (float)((double)rowY + D(0x5d0c78));
    }
}
RT3_REPLACE(0x4d2a60, CargoStats_Draw);

// Dialog setup helpers: install a callback (event 0x7a) on one control.
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w) { return Window_SendEvent5(w, 0, 0x7a, 0x4a3e, 0x4d2a60, 0, 0); }
RT3_REPLACE(0x4d2fe0, Dialog_SetCallback4a3e);

RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w) { return Window_SendEvent5(w, 0, 0x7a, 0x4a3b, 0x4d2430, 0, 0); }
RT3_REPLACE(0x4d3000, Dialog_SetCallback4a3b);

// 0x4d3020. Callback for control 0x5141 and its caption (string 0x45a, event 0x6e).
RT3_NOINLINE int32_t __fastcall Dialog_SetupControl5141(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x7a, 0x5141, 0x4d1d60, 0, 0);
    return Window_SendEvent5(w, 0, 0x6e, 0x5141, (uint32_t)(uintptr_t)GetString(0x45a), 0, 0);
}
RT3_REPLACE(0x4d3020, Dialog_SetupControl5141);

// (These three also reset a dialog global first.)
RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w) {
    hook::Global<int32_t>(0x6cffc4) = 0;
    return Window_SendEvent5(w, 0, 0x7a, 0x4a3f, 0x4d1b80, 0, 0);
}
RT3_REPLACE(0x4d3060, Dialog_SetCallback4a3f);

RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w) {
    hook::Global<int32_t>(0x6d07ac) = 0;
    return Window_SendEvent5(w, 0, 0x7a, 0x4a41, 0x4d0a90, 0, 0);
}
RT3_REPLACE(0x4d3080, Dialog_SetCallback4a41);

RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w) {
    hook::Global<int32_t>(0x6cffb0) = 0;
    return Window_SendEvent5(w, 0, 0x7a, 0x4a40, 0x4d0420, 0, 0);
}
RT3_REPLACE(0x4d30a0, Dialog_SetCallback4a40);

// 0x4d30c0. Scenario variables table at (x, y) in font 0x6cff90: a divider
// line, a title (0x45d), a header (0x45b / 0x45c), then every non-zero
// condition counter (types 0x26..0x1f6 without 0x27..0x36; names in the
// 0x6103a0 table) of the world (+0x8a, string 0x45e), each active company
// (+0x35b, 0x45f), each player (+0x2ab, 0x460) and each city with +0x3d
// (0x6cfc9c, +0x3b5, 0x461), with a gap between the groups. The walk is the
// original's state machine, which also steps through every type once with no
// owner (index -1) before each owner list.
RT3_NOINLINE void __fastcall ScenarioVars_Draw(void*, int, float x, float y, float, float, int32_t) {
    float cy = (float)((double)y + D(0x5c85b8));
    const uint32_t widths[2] = { 0x3e9b3333, 0x3daccccd };
    float x0 = (float)((double)x + D(0x5d0830));
    float ly = (float)((double)cy - D(0x5d06f8));
    Ui_Line(0xff8f8f8f, 0, x0, ly, (float)((double)x0 + D(0x5d0c98)), ly, 2.0f);
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, GetString(0x45d), x0, cy, BitsF(0x3ecf0a3d), BitsF(0x3cb851ec), 0xffffffff, 0, 1, 0, 1.0f);
    int32_t idx = 0, phase = 0, lastPhase = 0, type = 0;
    static const int32_t header[2] = { 0x45b, 0x45c };
    cy = (float)((double)cy + D(0x5d0c90));
    char text[2][0x320];
    for (;;) {
        if (idx >= 0) {
            if (type >= 0x27 && type <= 0x36) type = 0x37;
            uint32_t flags = 0;
            bool draw = false;
            const char* name = (const char*)0x6103a0 + type * 0x6e;
            switch (phase) {
            case 0:
                flags = 1;
                for (int k = 0; k < 2; ++k) ExeSprintf()(text[k], GetString(header[k]));
                draw = true;
                break;
            case 1: {
                int32_t v = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x8a + type * 4);
                if (!v) break;
                FormatText(text[0], GetString(0x45e), name, 0, 0, 0, 0, 0, 0, 0);
                Num_Format(text[1], 1, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x8a + type * 4), 10, 1, 0, 0);
                draw = true;
                break;
            }
            case 2: {
                uint8_t* co = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, idx);
                if (!F<int32_t>(co, 0x35b + type * 4)) break;
                FormatText(text[0], GetString(0x45f), (const char*)(co + 4), name, 0, 0, 0, 0, 0, 0);
                Num_Format(text[1], 1, (double)F<int32_t>(co, 0x35b + type * 4), 10, 1, 0, 0);
                draw = true;
                break;
            }
            case 3: {
                uint8_t* p = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, idx);
                if (!F<int32_t>(p, 0x2ab + type * 4)) break;
                FormatText(text[0], GetString(0x460), (const char*)(p + 8), name, 0, 0, 0, 0, 0, 0);
                Num_Format(text[1], 1, (double)F<int32_t>(p, 0x2ab + type * 4), 10, 1, 0, 0);
                draw = true;
                break;
            }
            case 4: {
                Collection* cs = hook::Global<Collection*>(0x6cfc9c);
                uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)idx, 0));
                if (!F<int32_t>(c, 0x3b5 + type * 4)) break;
                FormatText(text[0], GetString(0x461), (const char*)(c + 4), name, 0, 0, 0, 0, 0, 0);
                Num_Format(text[1], 1, (double)F<int32_t>(c, 0x3b5 + type * 4), 10, 1, 0, 0);
                draw = true;
                break;
            }
            }
            if (draw) {
                if (phase != lastPhase) {
                    lastPhase = phase;
                    cy = (float)((double)cy + D(0x5d0c88));
                }
                uint32_t valign = phase ? 0 : 4;
                float cx = x0;
                for (int k = 0; k < 2; ++k) {
                    double extra = k == 0 ? BitsD(0x3fa999999999999aull) : 0.0;
                    float w = (float)((double)BitsF(widths[k]) + extra);
                    Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, text[k], cx, cy, w, BitsF(0x3cb851ec), 0xffffffff, (k != 0) + 2, (int32_t)valign, flags,
                                     1.0f);
                    cx = (float)((double)cx + BitsF(widths[k]));
                }
                cy = (float)((double)cy + Fl(0x5d0c38));
            }
        }
        switch (phase) {
        case 0:
            idx = 0;
            phase = 1;
            type = 0x26;
            continue;
        case 1:
            if (++type <= 0x1f6) continue;
            idx = -1;
            phase = 2;
            type = 0x26;
            continue;
        case 2:
            if (++type <= 0x1f6) continue;
            ++idx;
            type = 0x26;
            if (idx < Table_CountField3f(hook::Global<Collection*>(0x62be10))) continue;
            idx = -1;
            phase = 3;
            continue;
        case 3:
            if (++type <= 0x1f6) continue;
            ++idx;
            type = 0x26;
            if (idx < Table_CountField4(hook::Global<Collection*>(0x6ceb9c))) continue;
            idx = -1;
            phase = 4;
            continue;
        case 4:
            if (++type <= 0x1f6) continue;
            type = 0x26;
            for (;;) {
                ++idx;
                if (idx >= Collection_Count(hook::Global<Collection*>(0x6cfc9c))) return;
                Collection* cs = hook::Global<Collection*>(0x6cfc9c);
                if (F<int32_t>(Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)idx, 0)), 0x3d) > 0) break;
            }
            continue;
        }
    }
}
RT3_REPLACE(0x4d30c0, ScenarioVars_Draw);

// 0x4d3600. Draws the player table at (x, y) in a box of height h: a grey
// divider line, then a header row (strings 0x462..0x464, centred, 0x37 high)
// and one row per player record (0x62be18) without +0x82b: name (+0x18), and
// the numbers +0x823 / +0x827, in columns 0.15 / 0.084375 / 0.084375 wide.
RT3_NOINLINE void __fastcall Players_DrawTable(void*, int, float x, float y, float, float h, int32_t) {
    float lineY = (float)((double)y + D(0x5d0cb0));
    float colX = (float)((double)x + D(0x5d0ca8));
    float lineX = (float)((double)colX - D(0x5d0ca0));
    float lineEnd = (float)((double)y + h - D(0x5d01d8));
    Ui_Line(0xff8f8f8f, 0, lineX, lineY, lineX, lineEnd, 2.0f);
    const float widths[3] = { BitsF(0x3e19999a), BitsF(0x3daccccd), BitsF(0x3daccccd) };
    char text[3][0x320];
    for (int32_t i = -1; i < Collection_Count(hook::Global<Collection*>(0x62be18)); ++i) {
        if (i == -1) {
            FormatText(text[0], GetString(0x462), 0, 0, 0, 0, 0, 0, 0, 0);
            FormatText(text[1], GetString(0x463), 0, 0, 0, 0, 0, 0, 0, 0);
            FormatText(text[2], GetString(0x464), 0, 0, 0, 0, 0, 0, 0, 0);
        } else {
            Collection* t = hook::Global<Collection*>(0x62be18);
            uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
            if (r[0x82b]) continue;
            FormatText(text[0], (const char*)(r + 0x18), 0, 0, 0, 0, 0, 0, 0, 0);
            Num_Format(text[1], 1, (double)F<int32_t>(r, 0x823), 10, 1, 0, 0);
            Num_Format(text[2], 1, (double)F<int32_t>(r, 0x827), 10, 1, 0, 0);
        }
        bool header = i == -1;
        float extra = (float)(header ? 0x37 : 0);
        float rowH = (float)((double)extra + Fl(0x5d0c38));
        float rowY = (float)((double)lineY - extra);
        float cx = colX;
        for (int k = 0; k < 3; ++k) {
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, text[k], cx, rowY, widths[k], rowH, 0xffffffff, k ? 3 : 2, header ? 4 : 0,
                             header ? 1 : 0, 1.0f);
            cx = (float)((double)cx + widths[k]);
        }
        lineY = (float)((double)lineY + Fl(0x5d0c38));
        if (header) lineY = (float)((double)lineY + D(0x5d0c88));
    }
}
RT3_REPLACE(0x4d3600, Players_DrawTable);

// 0x4d38c0. Scenario records panel at (x, y): the player table, then a table
// (strings 0x465, 0x10..0x13; columns 0.175 / 4 x 0.053) of the four record
// values of the world (+0x126, string 0x466), each active company (+0x407),
// each player (+0x367) and each city with +0x3d (0x6cfc9c, +0x481), and the
// scenario variables below it.
void __fastcall ScenarioRecords_Draw(void* self, int edx, float x, float y, float w, float h, int32_t a5) {
    Players_DrawTable(self, edx, x, y, w, h, a5);
    float cy = (float)((double)y + D(0x5d0cb0));
    int32_t phase = 0, idx = 0, lastPhase = 0;
    const uint32_t widths[5] = { 0x3e333333, 0x3d59999a, 0x3d59999a, 0x3d59999a, 0x3d59999a };
    float x0 = (float)((double)x + D(0x5d0830));
    static const int32_t labels[5] = { 0x465, 0x10, 0x11, 0x12, 0x13 };
    char text[5][0x320];
    for (;;) {
        if (idx >= 0) {
            uint32_t flags = 0;
            auto fill = [&](const char* name, uint8_t* base, uint32_t off) {
                FormatText(text[0], name, 0, 0, 0, 0, 0, 0, 0, 0);
                for (int k = 0; k < 4; ++k) Num_Format(text[k + 1], 1, (double)F<int32_t>(base, off + k * 4), 10, 1, 0, 0);
            };
            switch (phase) {
            case 0:
                flags = 1;
                for (int k = 0; k < 5; ++k) FormatText(text[k], GetString(labels[k]), 0, 0, 0, 0, 0, 0, 0, 0);
                break;
            case 1:
                FormatText(text[0], GetString(0x466), 0, 0, 0, 0, 0, 0, 0, 0);
                for (int k = 0; k < 4; ++k)
                    Num_Format(text[k + 1], 1, (double)F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x126 + k * 4), 10, 1, 0, 0);
                break;
            case 2: {
                uint8_t* co = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, idx);
                fill((const char*)(co + 4), co, 0x407);
                break;
            }
            case 3: {
                uint8_t* p = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, idx);
                fill((const char*)(p + 8), p, 0x367);
                break;
            }
            case 4: {
                Collection* cs = hook::Global<Collection*>(0x6cfc9c);
                uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)idx, 0));
                fill((const char*)(c + 4), c, 0x481);
                break;
            }
            }
            if (phase != lastPhase) {
                lastPhase = phase;
                cy = (float)((double)cy + D(0x5d0c88));
            }
            int32_t valign = phase ? 0 : 4;
            float cx = x0;
            for (int k = 0; k < 5; ++k) {
                Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff90), 0, text[k], cx, cy, BitsF(widths[k]), BitsF(0x3cb851ec), 0xffffffff, (k != 0) + 2, valign, flags,
                                 1.0f);
                cx = (float)((double)cx + BitsF(widths[k]));
            }
            cy = (float)((double)cy + Fl(0x5d0c38));
        }
        bool done = false;
        switch (phase) {
        case 0:
            phase = 1;
            idx = 0;
            break;
        case 1:
            phase = 2;
            idx = -1;
            break;
        case 2:
            if (++idx < Table_CountField3f(hook::Global<Collection*>(0x62be10))) break;
            phase = 3;
            idx = -1;
            break;
        case 3:
            if (++idx < Table_CountField4(hook::Global<Collection*>(0x6ceb9c))) break;
            phase = 4;
            idx = -1;
            break;
        case 4:
            for (;;) {
                ++idx;
                if (idx >= Collection_Count(hook::Global<Collection*>(0x6cfc9c))) {
                    done = true;
                    break;
                }
                Collection* cs = hook::Global<Collection*>(0x6cfc9c);
                if (F<int32_t>(Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)idx, 0)), 0x3d) > 0) break;
            }
            break;
        }
        if (done) break;
    }
    ScenarioVars_Draw(self, edx, x, (float)((double)cy + D(0x5d01e0)), w, h, a5);
}
RT3_REPLACE(0x4d38c0, ScenarioRecords_Draw);

// 0x4d3d90 (draw callback). The map editor's scenario check at (x, y): over
// the setups (0x62be18) it notes a kind 9 setup (+0x7ef) and the record
// kinds 4 / 6 / 7 in their four lists (+4.., linked by +0x24), and lists
// what is missing (strings 0x467 without kind 9, 0x468 without 7, 0x469 and
// 0x46a without 6, 0x46b without 4), then each setup that fails its check
// (0x46c with its name +0x18 and the reason), or 0x46d when all is fine.
void __fastcall ScenarioCheck_DrawCb(void*, int, float x, float y, float, float, int32_t) {
    bool kind9 = false, has4 = false, has6 = false, has7 = false, any = false;
    Collection* setups = hook::Global<Collection*>(0x62be18);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62be18)); ++i) {
        setups = hook::Global<Collection*>(0x62be18);
        uint8_t* e = (uint8_t*)Collection_Get(setups, 0, Collection_IdAt(setups, 0, (uint32_t)i, 0));
        if (e[0x7ef] == 9) kind9 = true;
        for (int32_t k = 0; k < 4; ++k)
            for (uint8_t* n = F<uint8_t*>(e, 4 + k * 4); n; n = F<uint8_t*>(n, 0x24)) {
                const int32_t kind = F<int32_t>(n, 0);
                if (kind == 4) has4 = true;
                if (kind == 6) has6 = true;
                if (kind == 7) has7 = true;
            }
    }
    const float tx = (float)(x + D(0x5d0830));
    float ty = (float)(y + D(0x5d0830));
    auto line = [&](uint32_t text) {
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, GetString(text), tx, ty, BitsF(0x3f3ccccd), BitsF(0x3d2aaaab), 0xffffffff, 2, 0, 0, 1.0f);
        ty = ty + BitsF(0x3d2aaaab);
    };
    if (!kind9) {
        any = true;
        line(0x467);
    }
    if (!has7) {
        any = true;
        line(0x468);
    }
    if (!has6) {
        line(0x469);
        any = true;
        line(0x46a);
    }
    if (!has4) {
        any = true;
        line(0x46b);
    }
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62be18)); ++i) {
        setups = hook::Global<Collection*>(0x62be18);
        uint8_t* e = (uint8_t*)Collection_Get(setups, 0, Collection_IdAt(setups, 0, (uint32_t)i, 0));
        char why[0x3e8], text[0x5cc];
        if (Scenario_CheckSetup((uint32_t*)e, 0, why)) continue;
        any = true;
        FormatText(text, GetString(0x46c), (const char*)e + 0x18, why, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, text, tx, ty, BitsF(0x3f3ccccd), 1.0f, 0xffffffff, 2, 1, 0, 1.0f);
        ty = (float)(Font_TextHeight(hook::Global<uint8_t*>(0x6cff8c), 0, text, BitsF(0x3f3ccccd), nullptr) + ty + D(0x5c85b8));
    }
    if (!any)
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6cff8c), 0, GetString(0x46d), tx, ty, BitsF(0x3f3ccccd), BitsF(0x3d2aaaab), 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x4d3d90, ScenarioCheck_DrawCb);

RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w) { return Window_SendEvent5(w, 0, 0x7a, 0x4a43, 0x4d38c0, 0, 0); }
RT3_REPLACE(0x4d4110, Dialog_SetCallback4a43);

RT3_NOINLINE int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w) { return Window_SendEvent5(w, 0, 0x7a, 0x4a42, 0x4d3d90, 0, 0); }
RT3_REPLACE(0x4d4130, Dialog_SetCallback4a42);

int32_t Global6d07b4Set() { return hook::Global<void*>(0x6d07b4) ? 1 : 0; }
RT3_REPLACE(0x4d4150, Global6d07b4Set);

// 0x4d4160. Highlights the view-mode buttons of window w by the main view's mode
// (0x6d0818 +0x8c; field 0x20): 0x2329 for modes 14..19, 0x232a 20, 0x232b 21,
// 0x232c 23, 0x232d 13, 0x232f 22, 0x2331 26; 0x232e always off.
void __fastcall Window_HighlightViewModes(uint8_t* w) {
    int32_t m = F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c);
    auto t = [m](int32_t x) { return m == x ? 0x65u : 0x87u; };
    Window_SendEvent5(w, 0, m >= 0xe && m <= 0x13 ? 0x65 : 0x87, 0x2329, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x14), 0x232a, 0x20, 0, 0);
    Window_SendEvent5(w, 0, 0x87, 0x232e, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x15), 0x232b, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x17), 0x232c, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0xd), 0x232d, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x16), 0x232f, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x1a), 0x2331, 0x20, 0, 0);
}
RT3_REPLACE(0x4d4160, Window_HighlightViewModes);

// 0x4d42b0 (event handler). Report menu buttons 0x2329..0x2331 toggle the
// main view's (0x6d0818) report modes (0xe..0x13 group, 0x14, 0x15, 0x17,
// 0xd, 0x16, 0x1a: back to mode 0 when already in it), open the editor
// (0x2330) or show string 0x104 and return to mode 0 (0x232e).
int32_t __fastcall ReportMenu_HandleEvent(void*, int, uint32_t* msg) {
    if (msg[0] != 0xcb) return 1;
    uint8_t* view = hook::Global<uint8_t*>(0x6d0818);
    auto toggle = [&](uint32_t mode) {
        if (F<uint32_t>(view, 0x8c) == mode)
            View_SetToolMode(view, 0, 0, -1);
        else
            View_SetToolMode(view, 0, mode, -1);
        return 0;
    };
    switch (msg[1]) {
    case 0x2329: {
        const uint32_t m = F<uint32_t>(view, 0x8c);
        View_SetToolMode(view, 0, m >= 0xe && m <= 0x13 ? 0 : 0xe, -1);
        return 0;
    }
    case 0x232a: return toggle(0x14);
    case 0x232b: return toggle(0x15);
    case 0x232c: return toggle(0x17);
    case 0x232d: return toggle(0xd);
    case 0x232e:
        Dialog_RunMessage(GetString(0x104), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
        return 0;
    case 0x232f: return toggle(0x16);
    case 0x2330:
        EditorDialog_Run(-1, 0);
        return 0;
    case 0x2331: return toggle(0x1a);
    default: return 1;
    }
}
RT3_REPLACE(0x4d42b0, ReportMenu_HandleEvent);

// 0x4d44a0. Closes the view-modes panel (0x6d07b4).
void ViewModesPanel_Close() { Panel_Close(0x6d07b4, 0x5d0cb8, false); }
RT3_REPLACE(0x4d44a0, ViewModesPanel_Close);

// 0x4d44f0. Window_HighlightViewModes on window 0x6d07b4, if open.
void Window_HighlightViewModesIfOpen() {
    if (uint8_t* w = hook::Global<uint8_t*>(0x6d07b4)) Window_HighlightViewModes(w);
}
RT3_REPLACE(0x4d44f0, Window_HighlightViewModesIfOpen);

// 0x4d4500. Opens the editor panel ("EditorPanel.win", 0x6d07b4).
void EditorPanel_Open() { Panel_Open(0x6d07b4, 0x7c, 0x5d0cb8, (const char*)0x5d0cc4, 0x1e, true); }
RT3_REPLACE(0x4d4500, EditorPanel_Open);

// 0x4d4560. Shows effect record w+0x84 of rule w+0x78 in the rules dialog: the
// rule in list 0x4f56, the effect's target kind (+4, 0..7) on radios 0x4f57..,
// numbers +0xe / +0x12 ("%.f", 0x4f76 / 0x4f79); a linked effect (+5) just selects
// +0x7c in 0x5032. Otherwise by the rule type (table 0x5f3de4, 0x81 bytes): a
// yes/no value (+9, radios 0x4f60 / 0x4f61) or a number (+9 in 0x4f6a, type 1).
// Controls 0x4f6f / 0x4f88 / 0x4f89 are disabled with +0x80 set, else enabled and
// the mode +0xd picks 0x4f88 or 0x4f89.
RT3_NOINLINE void __fastcall RulesDialog_ShowEffect(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x68, 0x4f56, F<uint32_t>(w, 0x78), 0, 0);
    uint32_t k = F<uint8_t*>(w, 0x84)[4];
    if (k <= 7) Window_SendEvent5(w, 0, 0x74, 0x4f57 + k, 1, 0, 0);
    char buf[0xc8], num[0xc8];
    Num_Format(buf, 8, (double)F<int32_t>(F<uint8_t*>(w, 0x84), 0xe), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x4f76, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)F<int32_t>(F<uint8_t*>(w, 0x84), 0x12), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x4f79, (uint32_t)(uintptr_t)buf, 0, 0);
    uint8_t* e = F<uint8_t*>(w, 0x84);
    if (F<int32_t>(e, 5)) {
        Window_SendEvent5(w, 0, 0x69, 0x5032, F<uint32_t>(w, 0x7c), 0, 0);
        return;
    }
    switch (*(const int32_t*)(0x5f3de4 + F<int32_t>(w, 0x78) * 0x81)) {
    case 0:
    case 2:
    case 3: Window_SendEvent5(w, 0, 0x74, F<int32_t>(e, 9) ? 0x4f60 : 0x4f61, 1, 0, 0); break;
    case 1:
        Num_Format(num, 8, (double)F<int32_t>(e, 9), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4f6a, (uint32_t)(uintptr_t)num, 0, 0);
        break;
    }
    uint32_t t = F<int32_t>(w, 0x80) ? 0x87 : 0x65;
    Window_SendEvent5(w, 0, t, 0x4f6f, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0x4f88, 3, 0, 0);
    Window_SendEvent5(w, 0, t, 0x4f89, 3, 0, 0);
    if (F<int32_t>(w, 0x80)) return;
    switch (F<uint8_t*>(w, 0x84)[0xd]) {
    case 0: Window_SendEvent5(w, 0, 0x74, 0x4f88, 1, 0, 0); break;
    case 1: Window_SendEvent5(w, 0, 0x74, 0x4f89, 1, 0, 0); break;
    }
}
RT3_REPLACE(0x4d4560, RulesDialog_ShowEffect);

RT3_NOINLINE void __fastcall Widget4d4840_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0cd4;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4d4840, Widget4d4840_Destruct);

// 0x4d4850. Fills the rules dialog's effect type list 0x4f56 with the effect types
// (table 0x5f3d90, 0x385 records of 0x81 bytes) in ascending order of their sort
// key +0 (ties keep table order): the ones in use (+0x64) and not hidden (+0x68),
// and with checkbox 0x5034 (+0xcc, kept in 0x6d07bc) only those flagged +0x58.
// Type sel becomes the dialog's effect (+0x78) and the list's selection.
RT3_NOINLINE void __fastcall RulesDialog_FillEffectTypes(uint8_t* w, int, int32_t sel) {
    int32_t pos = 0, selPos = 0;
    int32_t filter = F<int32_t>(Widget_FindChild(w, 0, 0x5034), 0xcc);
    hook::Global<int32_t>(0x6d07bc) = filter;
    Window_SendEvent5(w, 0, 0x8b, 0x4f56, 0, 0, 0);
    uint8_t* const table = (uint8_t*)0x5f3d90;
    char used[0x385];
    memset(used, 0, sizeof used);
    for (int32_t n = 0; n < 0x385; ++n) {
        int32_t best = -1;
        for (int32_t i = 0; i < 0x385; ++i) {
            uint8_t* r = table + i * 0x81;
            if (F<int32_t>(r, 0x68) || !F<int32_t>(r, 0x64) || used[i]) continue;
            if (filter && !F<int32_t>(r, 0x58)) continue;
            if (best == -1 || F<float>(r, 0) < F<float>(table + best * 0x81, 0)) best = i;
        }
        if (best == -1) break;
        uint32_t name = (uint32_t)(table + best * 0x81 + 0xc);
        Window_SendEvent5(w, 0, 0x88, 0x4f56, name, best, name);
        if (best == sel) {
            F<int32_t>(w, 0x78) = sel;
            selPos = pos;
        }
        ++pos;
        used[best] = 1;
    }
    Window_SendEvent5(w, 0, 0x66, 0x4f56, selPos, 0, 0);
}
RT3_REPLACE(0x4d4850, RulesDialog_FillEffectTypes);

// 0x4d4d10. The rules dialog's effect panel by the effect radio group 0x4f4f
// (re-entry guard 0x6d07c0): 0x4f4c / 0x4f4d pick the special effects -1 / -2
// (+0x78) and hide 0x4e48; 0x4f4e shows the effect type +0x78 (table 0x5f3d90,
// 0x81 bytes: kind +0x54 0..3, value range +0x6c / +0x70, unit +0x53, name strings
// +0x79 / +0x7d, scope flags +0x74): its value labels 0x4f74.. / 0x4f77.., then a
// yes / no type (radios 0x4f60 / 0x4f61) or a number type (range text 0x103, the
// target radios 0x4f57..0x4f5e, the amount controls 0x5029..0x5032 unless a
// whole-company target 0x4f5d / 0x4f5e is picked in group 0x4fd6, unit labels
// 0x4f70 / 0x4f71). Both radio queries share one event whose other fields the
// original leaves uninitialized (zero here).
RT3_NOINLINE void __fastcall RulesDialog_ShowEffectType(uint8_t* w) {
    if (hook::Global<int32_t>(0x6d07c0)) return;
    hook::Global<int32_t>(0x6d07c0) = 1;
    auto send = [w](uint32_t type, uint32_t id) { Window_SendEvent5(w, 0, type, id, 3, 0, 0); };
    send(0x87, 0x4f71);
    send(0x87, 0x4f70);
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x4f4f;
    Window_HandleEvent(w, 0, ev);
    if (ev[2] == 0x4f4c || ev[2] == 0x4f4d) {
        F<int32_t>(w, 0x78) = ev[2] == 0x4f4c ? -1 : -2;
        send(0x86, 0x4e48);
        hook::Global<int32_t>(0x6d07c0) = 0;
        return;
    }
    if (ev[2] != 0x4f4e) {
        hook::Global<int32_t>(0x6d07c0) = 0;
        return;
    }
    send(0x6d, 0x4e48);
    if (F<int32_t>(w, 0x80)) {
        send(0x87, 0x4f6f);
        send(0x87, 0x4f88);
        send(0x87, 0x4f89);
    }
    auto rec = [w]() { return (const uint8_t*)0x5f3d90 + F<int32_t>(w, 0x78) * 0x81; };
    int32_t kind = *(const int32_t*)(rec() + 0x54);
    bool b1 = kind == 2 || kind == 3;
    for (uint32_t id = 0x4f74; id <= 0x4f76; ++id) send(b1 ? 0x65 : 0x87, id);
    if (b1) Window_SendEvent5(w, 0, 0x6f, 0x4f74, (uint32_t)(uintptr_t)GetString(*(const int32_t*)(rec() + 0x79)), 0, 0);
    bool b2 = *(const int32_t*)(rec() + 0x54) == 3;
    for (uint32_t id = 0x4f77; id <= 0x4f79; ++id) send(b2 ? 0x65 : 0x87, id);
    if (b2) Window_SendEvent5(w, 0, 0x6f, 0x4f77, (uint32_t)(uintptr_t)GetString(*(const int32_t*)(rec() + 0x7d)), 0, 0);
    kind = *(const int32_t*)(rec() + 0x54);
    if (kind == 0 || kind == 2 || kind == 3) {
        Window_SendEvent5(w, 0, 0x6f, 0x4f6b, (uint32_t)(uintptr_t)GetString(0x100), 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4f6d, (uint32_t)(uintptr_t)GetString(0x101), 0, 0);
        for (uint32_t id = 0x4f57; id <= 0x4f5e; ++id) send(0x87, id);
        static const uint16_t amount[5] = { 0x5029, 0x502a, 0x4f6a, 0x4f6e, 0x5032 };
        for (uint16_t id : amount) send(0x87, id);
        send(0x65, 0x4f60);
        send(0x65, 0x4f61);
        if (!F<int32_t>(Widget_FindChild(w, 0, 0x4f61), 0xcc)) Window_SendEvent5(w, 0, 0x74, 0x4f60, 1, 0, 0);
    } else if (kind == 1) {
        Window_SendEvent5(w, 0, 0x6f, 0x4f6b, (uint32_t)(uintptr_t)GetString(0x102), 0, 0);
        char hi[0x30], lo[0x30], text[0xa0];
        char* sh = Num_Format(hi, 1, (double)*(const int32_t*)(rec() + 0x70), 0xf, 1, 0, 0);
        char* sl = Num_Format(lo, 1, (double)*(const int32_t*)(rec() + 0x6c), 0xf, 1, 0, 0);
        FormatText(text, GetString(0x103), sl, sh, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4f6d, (uint32_t)(uintptr_t)text, 0, 0);
        for (uint32_t id = 0x4f57; id <= 0x4f5c; ++id) send(0x65, id);
        uint32_t t = rec()[0x74] & 7 ? 0x65 : 0x87;
        send(t, 0x4f5d);
        send(t, 0x4f5e);
        ev[0] = 0x94;
        ev[1] = 0x4fd6;
        Window_HandleEvent(w, 0, ev);
        if (ev[2] == 0x4f5d || ev[2] == 0x4f5e) {
            static const uint16_t amount[5] = { 0x5029, 0x502a, 0x4f6a, 0x4f6e, 0x5032 };
            for (uint16_t id : amount) send(0x87, id);
            send(0x87, 0x4f70);
            send(0x87, 0x4f71);
        } else {
            send(0x65, 0x5029);
            send(0x65, 0x502a);
            bool rel = F<int32_t>(Widget_FindChild(w, 0, 0x5029), 0xcc) != 0;
            send(rel ? 0x65 : 0x87, 0x4f6a);
            send(rel ? 0x65 : 0x87, 0x4f6e);
            send(rel ? 0x87 : 0x65, 0x5032);
            uint8_t unit = rec()[0x53];
            if (unit == 2) send(0x65, 0x4f70);
            else if (unit == 1) send(0x65, 0x4f71);
        }
        send(0x87, 0x4f60);
        send(0x87, 0x4f61);
    }
    hook::Global<int32_t>(0x6d07c0) = 0;
}
RT3_REPLACE(0x4d4d10, RulesDialog_ShowEffectType);

// 0x4d54d0. Stores rule dialog d's condition into its record (+0x84): the
// rule (+0x78), the comparison (list 0x4fd6, 0x4f57.. -> 0..7), then by the
// rule's kind (0x5f3de4, 0x81 each) the operand (kinds 0 / 2 / 3: the sign
// list 0x4f62 and the numbers 0x4f76 / 0x4f79; kind 1: number 0x4f6a, or
// the picked object +0x7c unless the field 0x5029 is set), and the scope
// (+0xd: 2 with +0x80 or a negative rule, else list 0x4f8a). Returns 0 for
// an unknown comparison, else 1.
RT3_NOINLINE int32_t __fastcall RuleDialog_StoreCondition(uint8_t* d) {
    uint8_t* rec = F<uint8_t*>(d, 0x84);
    F<int32_t>(rec, 0) = F<int32_t>(d, 0x78);
    if (F<int32_t>(d, 0x78) < 0) {
        rec[0xd] = 2;
        F<uint32_t>(F<uint8_t*>(d, 0x84), 0x1a) = 0;
        return 1;
    }
    uint32_t ev[0x25];
    char num[0x64];
    auto atoi_ = (int(__cdecl*)(const char*))0x5a1ea5;
    ev[0] = 0x94, ev[1] = 0x4fd6;
    Window_HandleEvent(d, 0, ev);
    const uint32_t cmp = ev[2] - 0x4f57;
    if (cmp > 7) return 0;
    F<uint8_t*>(d, 0x84)[4] = (uint8_t)cmp;
    const int32_t kind = *(const int32_t*)(0x5f3de4 + F<int32_t>(d, 0x78) * 0x81);
    if (kind == 0 || kind == 2 || kind == 3) {
        ev[0] = 0x94, ev[1] = 0x4f62;
        Window_HandleEvent(d, 0, ev);
        if (ev[2] == 0x4f60)
            F<int32_t>(F<uint8_t*>(d, 0x84), 9) = 1;
        else if (ev[2] == 0x4f61)
            F<int32_t>(F<uint8_t*>(d, 0x84), 9) = 0;
        F<int32_t>(F<uint8_t*>(d, 0x84), 5) = 0;
        strcpy(num, F<const char*>(Widget_FindChild(d, 0, 0x4f76), 0xba));
        F<int32_t>(F<uint8_t*>(d, 0x84), 0xe) = atoi_(num);
        strcpy(num, F<const char*>(Widget_FindChild(d, 0, 0x4f79), 0xba));
        F<int32_t>(F<uint8_t*>(d, 0x84), 0x12) = atoi_(num);
    } else if (kind == 1) {
        if (F<uint32_t>(Widget_FindChild(d, 0, 0x5029), 0xcc)) {
            strcpy(num, F<const char*>(Widget_FindChild(d, 0, 0x4f6a), 0xba));
            F<int32_t>(F<uint8_t*>(d, 0x84), 5) = 0;
            F<int32_t>(F<uint8_t*>(d, 0x84), 9) = atoi_(num);
        } else {
            F<int32_t>(F<uint8_t*>(d, 0x84), 5) = 1;
            F<uint32_t>(F<uint8_t*>(d, 0x84), 9) = F<uint32_t>(d, 0x7c);
        }
    }
    if (F<uint32_t>(d, 0x80)) {
        F<uint8_t*>(d, 0x84)[0xd] = 2;
    } else {
        ev[0] = 0x94, ev[1] = 0x4f8a;
        Window_HandleEvent(d, 0, ev);
        if (ev[2] == 0x4f88)
            F<uint8_t*>(d, 0x84)[0xd] = 0;
        else if (ev[2] == 0x4f89)
            F<uint8_t*>(d, 0x84)[0xd] = 1;
    }
    F<uint32_t>(F<uint8_t*>(d, 0x84), 0x1a) = 0;
    return 1;
}
RT3_REPLACE(0x4d54d0, RuleDialog_StoreCondition);

// Widget callbacks: ECX is the new value (-1 none), EDX a second one.
void __fastcall Rules_SetEffectType(int32_t v, int32_t value) {
    if (v == -1) return;
    F<int32_t>(hook::Global<uint8_t*>(0x6d07b8), 0x78) = value;
    RulesDialog_ShowEffectType(hook::Global<uint8_t*>(0x6d07b8));
}
RT3_REPLACE(0x4d5790, Rules_SetEffectType);

// 0x4d57b0 (callback). Stores the value (EDX) at +0x7c of 0x6d07b8.
void __fastcall Set6d07b8_7c(void*, int32_t v) { F<int32_t>(hook::Global<uint8_t*>(0x6d07b8), 0x7c) = v; }
RT3_REPLACE(0x4d57b0, Set6d07b8_7c);

// 0x4d57c0. Constructs the rules dialog (vtable 0x5d0cd4, kept in 0x6d07b8) for
// owner a (+0x80) editing rule (+0x84): its effect type rule+0 goes to +0x78 and
// its value rule+9 (when rule+5 is set) to +0x7c. Loads layout 0x5d0ce0, hides
// two controls in languages 1 / 4, ticks filter 0x5034 when filtering and the
// type is flagged, sets up the lists 0x4f56 (effect types) and 0x5032 (the 614
// records at 0x6103a0, 0x6e bytes: in use +0x4f, not hidden +0x57) with their
// callbacks, picks the radio for the effect (-2 / -1 / a type, then 0x502a /
// 0x5029 by rule+5) and shows the effect.
RT3_NOINLINE uint8_t* __fastcall RulesDialog_Construct(uint8_t* w, int, uint32_t a, int32_t* rule) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0cd4;
    hook::Global<uint8_t*>(0x6d07b8) = w;
    F<uint32_t>(w, 0x80) = a;
    F<int32_t*>(w, 0x84) = rule;
    F<int32_t>(w, 0x78) = rule[0];
    int32_t selPos = 0;
    F<int32_t>(w, 0x7c) = F<int32_t>(rule, 5) ? F<int32_t>(rule, 9) : 0;
    uint32_t xb = 0x3e9b851f, yb = 0x3d64b17e;
    Window_Load(w, 0, *(float*)&xb, *(float*)&yb, (const char*)0x5d0ce0);
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    if (lang == 4 || lang == 1) {
        Window_SendEvent5(w, 0, 0x78, 0x4f73, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x4fd1, 0x12, 0, 0);
    }
    Widget_FindChild(w, 0, 0x5034);
    if (hook::Global<int32_t>(0x6d07bc) && rule[0] >= 0 && F<int32_t>((uint8_t*)0x5f3d90 + rule[0] * 0x81, 0x58) == 1)
        F<int32_t>(Widget_FindChild(w, 0, 0x5034), 0xcc) = 1;
    Window_SendEvent5(w, 0, 0x86, 0x4e48, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x4f56, 0x4d5790, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x5032, 0x4d57b0, 0, 0);
    RulesDialog_FillEffectTypes(w, 0, F<int32_t>(w, 0x78));
    int32_t pos = 0;
    int32_t i = 0;
    for (uint32_t r = 0x6103a0; r < 0x620b74; r += 0x6e, ++i) {
        if (F<int32_t>((void*)r, 0x57) || !F<int32_t>((void*)r, 0x4f)) continue;
        Window_SendEvent5(w, 0, 0x88, 0x5032, r, (uint32_t)i, r);
        if (i == F<int32_t>(w, 0x7c)) selPos = pos;
        ++pos;
    }
    Window_SendEvent5(w, 0, 0x66, 0x5032, (uint32_t)selPos, 0, 0);
    int32_t e = F<int32_t>(w, 0x78);
    if (e == -2) {
        Window_SendEvent5(w, 0, 0x74, 0x4f4d, 1, 0, 0);
    } else if (e == -1) {
        Window_SendEvent5(w, 0, 0x74, 0x4f4c, 1, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x74, 0x4f4e, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x74, F<int32_t>(F<uint8_t*>(w, 0x84), 5) ? 0x502a : 0x5029, 1, 0, 0);
    }
    RulesDialog_ShowEffectType(w);
    if (rule[0] < 0) {
        Window_SendEvent5(w, 0, 0x74, 0x4f4c + (F<int32_t>(w, 0x78) != -1 ? 1 : 0), 1, 0, 0);
        RulesDialog_ShowEffectType(w);
        return w;
    }
    RulesDialog_ShowEffect(w);
    RulesDialog_ShowEffectType(w);
    return w;
}
RT3_REPLACE(0x4d57c0, RulesDialog_Construct);

// 0x4d59e0 (event handler). The rule dialog d: the rule lists (0x4f4c ..,
// 0x5029 / 0x502a) refresh the effect type, 0x4f4e (default 0xe) also the
// effect, 0x5034 refills the types; OK (0x4f73) stores the condition and
// closes when it is complete, Cancel (0x4fd1) closes.
int32_t __fastcall RuleDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    switch (msg[0]) {
    case 0xca:
        switch (msg[1]) {
        case 0x4f4c:
        case 0x4f4d:
        case 0x4f57:
        case 0x4f58:
        case 0x4f59:
        case 0x4f5a:
        case 0x4f5b:
        case 0x4f5c:
        case 0x4f5d:
        case 0x4f5e:
        case 0x5029:
        case 0x502a:
            RulesDialog_ShowEffectType(d);
            return 0;
        case 0x4f4e:
            if (F<int32_t>(d, 0x78) < 0) F<int32_t>(d, 0x78) = 0xe;
            RulesDialog_ShowEffect(d);
            RulesDialog_ShowEffectType(d);
            return 0;
        case 0x5034:
            RulesDialog_FillEffectTypes(d, 0, F<int32_t>(d, 0x78));
            return 0;
        }
        return 1;
    case 0xcb:
        if (msg[1] == 0x4f73) {
            if (RuleDialog_StoreCondition(d)) msg[0] = 0xc9;
        } else if (msg[1] == 0x4fd1) {
            msg[0] = 0xc9;
        }
        return 1;
    default:
        return 1;
    }
}
RT3_REPLACE(0x4d59e0, RuleDialog_HandleEvent);

RT3_NOINLINE void __fastcall Widget4d5b90_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0cf4;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4d5b90, Widget4d5b90_Destruct);

// 0x4d5ba0. Shows rule record w+0x78's limits ("%.f"): byte +0x11 (at least 1) in
// 0x503e, words +0x12 / +0x14 in 0x5041 / 0x5044 and +0x16 (at least 5) in 0x5047.
RT3_NOINLINE void __fastcall RulesDialog_ShowLimits(uint8_t* w) {
    char buf[0x258];
    uint8_t a = F<uint8_t*>(w, 0x78)[0x11];
    Num_Format(buf, 8, (double)(int32_t)(a < 1 ? 1 : a), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x503e, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)F<uint16_t>(F<uint8_t*>(w, 0x78), 0x12), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5041, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)F<uint16_t>(F<uint8_t*>(w, 0x78), 0x14), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5044, (uint32_t)(uintptr_t)buf, 0, 0);
    uint16_t d = F<uint16_t>(F<uint8_t*>(w, 0x78), 0x16);
    Num_Format(buf, 8, (double)(int32_t)(d < 5 ? 5 : d), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x5047, (uint32_t)(uintptr_t)buf, 0, 0);
}
RT3_REPLACE(0x4d5ba0, RulesDialog_ShowLimits);

// 0x4d5d00. Fills the rules dialog's condition list 0x4fb2 with the 614 condition
// types (table 0x610398, 0x6e bytes: sort key float +0, name +8, -1 = unused +0x4f,
// multiplayer-ok +0x5b, hidden +0x5f, hint string +0x66) sorted by key (selection
// sort; ties keep table order), leaving out hidden ones and, with box 0x5033 ticked
// (also kept in 0x6d07c8), the ones not allowed in multiplayer. Rows show
// "name<sep>hint" (separator 0x5c9ce8); type `sel` is selected (+0x7c).
RT3_NOINLINE void __fastcall RulesDialog_FillConditionTypes(uint8_t* w, int, int32_t sel) {
    int32_t mpOnly = F<int32_t>(Widget_FindChild(w, 0, 0x5033), 0xcc);
    hook::Global<int32_t>(0x6d07c8) = mpOnly;
    Window_SendEvent5(w, 0, 0x8b, 0x4fb2, 0, 0, 0);
    uint8_t used[0x266];
    memset(used, 0, sizeof used);
    const uint8_t* table = (const uint8_t*)0x610398;
    int32_t listed = 0, selRow = 0;
    for (int32_t round = 0; round < 0x266; ++round) {
        int32_t best = -1;
        for (int32_t k = 0; k < 0x266; ++k) {
            const uint8_t* r = table + k * 0x6e;
            if (used[k] || *(const int32_t*)(r + 0x5f) || *(const int32_t*)(r + 0x4f) == -1) continue;
            if (mpOnly && !*(const int32_t*)(r + 0x5b)) continue;
            if (best != -1 && !(*(const float*)r < *(const float*)(table + best * 0x6e))) continue;
            best = k;
        }
        if (best == -1) break;
        const uint8_t* r = table + best * 0x6e;
        used[best] = 1;
        char text[0x310];
        strcpy(text, (const char*)r + 8);
        if (strlen(GetString(*(const int32_t*)(r + 0x66)))) {
            memcpy(text + strlen(text), (const void*)0x5c9ce8, 2);
            strcat(text, GetString(*(const int32_t*)(r + 0x66)));
        }
        Window_SendEvent5(w, 0, 0x88, 0x4fb2, (uint32_t)(uintptr_t)(r + 8), (uint32_t)best, (uint32_t)(uintptr_t)text);
        if (best == sel) {
            F<int32_t>(w, 0x7c) = sel;
            selRow = listed;
        }
        ++listed;
    }
    Window_SendEvent5(w, 0, 0x66, 0x4fb2, (uint32_t)selRow, 0, 0);
}
RT3_REPLACE(0x4d5d00, RulesDialog_FillConditionTypes);

// 0x4d5f50. Enables buttons 0x4ff7..0x4ffc (0x65, else 0x87) when box 0x4ff6 is
// ticked and condition type +0x7c (table 0x6103eb, 0x6e-byte records) allows it.
RT3_NOINLINE void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w) {
    bool on = F<int32_t>(Widget_FindChild(w, 0, 0x4ff6), 0xcc) && *(const int32_t*)(0x6103eb + F<int32_t>(w, 0x7c) * 0x6e);
    for (uint32_t id = 0x4ff7; id <= 0x4ffc; ++id) Window_SendEvent5(w, 0, on ? 0x65 : 0x87, id, 3, 0, 0);
}
RT3_REPLACE(0x4d5f50, RulesDialog_UpdateButtons4ff7);

// 0x4d6090. Enables the condition editor's value controls for type +0x7c (table
// 0x610398: flags +0x64, unit +0x63 1 % / 2 $ / 3 other, year range +0x53) and
// shows condition +0x78's value (re-entry guard 0x6d07cc): everything off, then
// flag 8 the place fields 0x503c..0x5047 (with the limits), flag 1 the yes / no
// radios, flag 2 the more / less radios with the amount 0x4fc7 (kind 2: the radio
// by its sign, zero keeps "less" when picked) and unit labels 0x4fc9 / 0x4fca /
// 0x4fc5, flag 4 the plain amount 0x4fce (kind 4) and unit labels 0x4fcb / 0x4fcc /
// 0x4fc6; the year range box 0x4ff6 (and its fields) only for types that have one.
RT3_NOINLINE void __fastcall ConditionDialog_ShowValue(uint8_t* w) {
    if (hook::Global<int32_t>(0x6d07cc)) return;
    hook::Global<int32_t>(0x6d07cc) = 1;
    static const uint16_t off[] = { 0x4fba, 0x4fbb, 0x4fbf, 0x4fc0, 0x4fc1, 0x4fd2, 0x4fc7, 0x4fd3,
                                    0x4fcd, 0x4fce, 0x4fcc, 0x4fcb, 0x4fc6, 0x4fca, 0x4fc9, 0x4fc5 };
    for (uint16_t id : off) Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
    for (uint32_t id = 0x503c; id <= 0x5047; ++id) Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
    auto rec = [w]() { return (const uint8_t*)0x610398 + F<int32_t>(w, 0x7c) * 0x6e; };
    auto on = [w](uint32_t id) { Window_SendEvent5(w, 0, 0x65, id, 3, 0, 0); };
    auto tick = [w](uint32_t id) { Window_SendEvent5(w, 0, 0x74, id, 1, 0, 0); };
    char buf[0xc8];
    if (rec()[0x64] & 8) {
        for (uint32_t id = 0x503c; id <= 0x5047; ++id) on(id);
        RulesDialog_ShowLimits(w);
    }
    if (rec()[0x64] & 1) {
        on(0x4fba);
        on(0x4fbb);
    }
    if (rec()[0x64] & 2) {
        on(0x4fc0);
        on(0x4fc1);
        uint8_t* c = F<uint8_t*>(w, 0x78);
        if (c[8] == 2) {
            int32_t v = F<int32_t>(c, 4);
            if (v < 0) tick(0x4fc1);
            else if (v > 0 || !F<int32_t>(Widget_FindChild(w, 0, 0x4fc1), 0xcc)) tick(0x4fc0);
            v = F<int32_t>(F<uint8_t*>(w, 0x78), 4);
            Num_Format(buf, 8, (double)(v < 0 ? -v : v), 10, 1, 0, 0);
            Window_SendEvent5(w, 0, 0x6f, 0x4fc7, (uint32_t)(uintptr_t)buf, 0, 0);
            on(0x4fd2);
            on(0x4fc7);
            if (rec()[0x63] == 2) on(0x4fc9);
            if (rec()[0x63] == 1) on(0x4fca);
            if (rec()[0x63] == 3) on(0x4fc5);
        }
    }
    if (rec()[0x64] & 4) {
        on(0x4fbf);
        on(0x4fd3);
        if (F<uint8_t*>(w, 0x78)[8] == 4) {
            tick(0x4fbf);
            Num_Format(buf, 8, (double)F<int32_t>(F<uint8_t*>(w, 0x78), 4), 10, 1, 0, 0);
            Window_SendEvent5(w, 0, 0x6f, 0x4fce, (uint32_t)(uintptr_t)buf, 0, 0);
            on(0x4fcd);
            on(0x4fce);
            if (rec()[0x63] == 2) on(0x4fcb);
            if (rec()[0x63] == 1) on(0x4fcc);
            if (rec()[0x63] == 3) on(0x4fc6);
        }
    }
    if (*(const int32_t*)(rec() + 0x53)) {
        on(0x4ff6);
        RulesDialog_UpdateButtons4ff7(w);
    } else
        for (uint32_t id = 0x4ff6; id <= 0x4ffc; ++id) Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
    hook::Global<int32_t>(0x6d07cc) = 0;
}
RT3_REPLACE(0x4d6090, ConditionDialog_ShowValue);

// 0x4d67f0. Stores the condition editor into condition w+0x78 (type +0x7c into +0,
// re-entry guard 0x6d07d0, returns 0 when busy). By the type's value flags (table
// 0x610398 +0x64): first makes sure a valid comparison radio is picked (group
// 0x4fc2: 0x4fc0 / 0x4fc1 more / less for flag 2, 0x4fbf for 4, 0x4fba for 1);
// then flag 8 reads a place (count +0x11 1..100, cell x / y +0x12 / +0x14 within
// the map, radius +0x16 1..1000; kind 8), flag 2 a signed amount from 0x4fc7
// (negated for "less"; kind 2), flag 4 an amount from 0x4fce (kind 4), flag 1 a
// yes / no from group 0x4fbc (kind 1); the year range 0x4ff9 / 0x4ffc when box
// 0x4ff6 is ticked (else -1 / -1), and clears +0x24. The radio queries share one
// event whose other fields the original leaves uninitialized (zero here).
RT3_NOINLINE int32_t __fastcall ConditionDialog_Store(uint8_t* w) {
    if (hook::Global<int32_t>(0x6d07d0)) return 0;
    hook::Global<int32_t>(0x6d07d0) = 1;
    F<int32_t>(F<uint8_t*>(w, 0x78), 0) = F<int32_t>(w, 0x7c);
    auto num = [w](uint32_t id) { return ((int(__cdecl*)(const char*))0x5a1ea5)(F<const char*>(Widget_FindChild(w, 0, id), 0xba)); };
    auto flags = [w]() { return ((const uint8_t*)0x6103fc)[F<int32_t>(w, 0x7c) * 0x6e]; };
    uint32_t ev[12] = {};
    auto query = [&](uint32_t id) {
        ev[0] = 0x94;
        ev[1] = id;
        Window_HandleEvent(w, 0, ev);
        return ev[2];
    };
    uint32_t pick = query(0x4fc2);
    uint8_t fl = flags();
    bool ok = false;
    if ((fl & 2) && (pick == 0x4fc0 || pick == 0x4fc1)) ok = true;
    if ((fl & 4) && pick == 0x4fbf) ok = true;
    if ((fl & 1) && pick == 0x4fba) ok = true;
    if (!(fl & 8) && !ok) {
        if (fl & 2) Window_SendEvent5(w, 0, 0x74, 0x4fc0, 1, 0, 0);
        else if (fl & 4) Window_SendEvent5(w, 0, 0x74, 0x4fbf, 1, 0, 0);
        if (flags() & 2) Window_SendEvent5(w, 0, 0x74, 0x4fba, 1, 0, 0);
    }
    pick = query(0x4fc2);
    fl = flags();
    uint8_t* c = F<uint8_t*>(w, 0x78);
    char buf[0x100];
    if (fl & 8) {
        c[0x11] = (uint8_t)num(0x503e);
        if (c[0x11] < 1) c[0x11] = 1;
        if (c[0x11] > 100) c[0x11] = 100;
        F<uint16_t>(c, 0x12) = (uint16_t)num(0x5041);
        int32_t mx = F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155) - 1;
        if (mx < F<uint16_t>(c, 0x12)) F<uint16_t>(c, 0x12) = (uint16_t)mx;
        F<uint16_t>(c, 0x14) = (uint16_t)num(0x5044);
        int32_t my = F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) - 1;
        if (my < F<uint16_t>(c, 0x14)) F<uint16_t>(c, 0x14) = (uint16_t)my;
        F<uint16_t>(c, 0x16) = (uint16_t)num(0x5047);
        if (F<uint16_t>(c, 0x16) < 1) F<uint16_t>(c, 0x16) = 1;
        if (F<uint16_t>(c, 0x16) > 1000) F<uint16_t>(c, 0x16) = 1000;
        RulesDialog_ShowLimits(w);
        F<uint8_t*>(w, 0x78)[8] = 8;
    } else if ((fl & 2) && (pick == 0x4fc0 || pick == 0x4fc1)) {
        F<int32_t>(c, 4) = num(0x4fc7);
        if (query(0x4fc2) == 0x4fc1) F<int32_t>(F<uint8_t*>(w, 0x78), 4) = -F<int32_t>(F<uint8_t*>(w, 0x78), 4);
        F<uint8_t*>(w, 0x78)[8] = 2;
        int32_t v = F<int32_t>(F<uint8_t*>(w, 0x78), 4);
        Num_Format(buf, 8, (double)(v < 0 ? -v : v), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4fc7, (uint32_t)(uintptr_t)buf, 0, 0);
    } else if ((fl & 4) && (pick == 0x4fbf || pick == 0x4fc1)) {
        F<int32_t>(c, 4) = num(0x4fce);
        F<uint8_t*>(w, 0x78)[8] = 4;
        int32_t v = F<int32_t>(F<uint8_t*>(w, 0x78), 4);
        Num_Format(buf, 8, (double)(v < 0 ? -v : v), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4fce, (uint32_t)(uintptr_t)buf, 0, 0);
    } else if (fl & 1) {
        uint32_t yn = query(0x4fbc);
        if (yn == 0x4fba) F<int32_t>(F<uint8_t*>(w, 0x78), 4) = 1;
        else if (yn == 0x4fbb) F<int32_t>(F<uint8_t*>(w, 0x78), 4) = 0;
        F<uint8_t*>(w, 0x78)[8] = 1;
    }
    int32_t lo = -1, hi = -1;
    if (F<int32_t>(Widget_FindChild(w, 0, 0x4ff6), 0xcc)) {
        lo = num(0x4ff9);
        F<int32_t>(F<uint8_t*>(w, 0x78), 9) = lo;
        hi = num(0x4ffc);
    } else F<int32_t>(F<uint8_t*>(w, 0x78), 9) = lo;
    F<int32_t>(F<uint8_t*>(w, 0x78), 0xd) = hi;
    F<int32_t>(F<uint8_t*>(w, 0x78), 0x24) = 0;
    hook::Global<int32_t>(0x6d07d0) = 0;
    return 1;
}
RT3_REPLACE(0x4d67f0, ConditionDialog_Store);

void __fastcall Condition_SetValue(int32_t v, int32_t value) {
    if (v == -1) return;
    F<int32_t>(hook::Global<uint8_t*>(0x6d07c4), 0x7c) = value;
    ConditionDialog_Store(hook::Global<uint8_t*>(0x6d07c4));
    ConditionDialog_ShowValue(hook::Global<uint8_t*>(0x6d07c4));
    RulesDialog_UpdateButtons4ff7(hook::Global<uint8_t*>(0x6d07c4));
}
RT3_REPLACE(0x4d6d30, Condition_SetValue);

// 0x4d6d60. Condition dialog constructor ("EventEffects.win", vtable 0x5d0cf4,
// becomes 0x6d07c4) for condition record `r` (+0x78; type r+0 in +0x7c). Hides
// the two text-only buttons in languages 1 and 4; ticks the multiplayer-only box
// 0x5033 when 0x6d07c8 is set and the type isn't multiplayer-ok (0x610398 row
// +0x5b); fills the type list (selection handler 0x4d6d30), restoring the record
// fields that filling resets; ticks 0x4ff6 when either limit (+9, +0xd) is set
// and the comparison button of the mode (+8: 2 by the sign of +4, 4, or 1 by
// +4 != 0); shows the value, the limits and the amount (+4, unsigned in mode 2).
RT3_NOINLINE uint8_t* __fastcall ConditionDialog_Construct(uint8_t* w, int, uint8_t* r) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0cf4;
    hook::Global<uint8_t*>(0x6d07c4) = w;
    F<int32_t>(w, 0x7c) = F<int32_t>(r, 0);
    F<uint8_t*>(w, 0x78) = r;
    int32_t amount = F<int32_t>(r, 4);
    int32_t mode = r[8];
    int32_t b11 = r[0x11];
    int32_t lo = F<int32_t>(r, 9);
    int32_t hi = F<int32_t>(r, 0xd);
    uint16_t w12 = F<uint16_t>(r, 0x12), w14 = F<uint16_t>(r, 0x14), w16 = F<uint16_t>(r, 0x16);
    Window_Load(w, 0, BitsF(0x3e89eb85), BitsF(0x3e2c5f93), (const char*)0x5d0d00);
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    if (lang == 4 || lang == 1) {
        Window_SendEvent5(w, 0, 0x78, 0x4fb1, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x4fd4, 0x12, 0, 0);
    }
    if (hook::Global<int32_t>(0x6d07c8)) {
        int32_t t = F<int32_t>(r, 0);
        if (t < 0 || F<int32_t>((uint8_t*)0x610398, t * 0x6e + 0x5b)) F<int32_t>(Widget_FindChild(w, 0, 0x5033), 0xcc) = 1;
    }
    Window_SendEvent5(w, 0, 0x79, 0x4fb2, 0x4d6d30, 0, 0);
    RulesDialog_FillConditionTypes(w, 0, F<int32_t>(w, 0x7c));
    if (mode) r[8] = (uint8_t)mode;
    F<int32_t>(r, 4) = amount;
    r[0x11] = (uint8_t)b11;
    F<uint16_t>(r, 0x12) = w12;
    F<uint16_t>(r, 0x14) = w14;
    F<uint16_t>(r, 0x16) = w16;
    F<int32_t>(r, 9) = lo;
    F<int32_t>(r, 0xd) = hi;
    Window_SendEvent5(w, 0, 0x74, 0x4ff6, lo > 0 || hi > 0 ? 1 : 0, 0, 0);
    if (mode == 2) Window_SendEvent5(w, 0, 0x74, 0x4fc0 + (amount < 0 ? 1 : 0), 1, 0, 0);
    else if (mode == 4) Window_SendEvent5(w, 0, 0x74, 0x4fbf, 1, 0, 0);
    else if (mode == 1) Window_SendEvent5(w, 0, 0x74, amount ? 0x4fba : 0x4fbb, 1, 0, 0);
    ConditionDialog_ShowValue(w);
    RulesDialog_UpdateButtons4ff7(w);
    char text[0x100];
    if (F<int32_t>(r, 9) > 0 || F<int32_t>(r, 0xd) > 0) {
        Num_Format(text, 8, (double)F<int32_t>(r, 9), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4ff9, (uint32_t)(uintptr_t)text, 0, 0);
        Num_Format(text, 8, (double)F<int32_t>(r, 0xd), 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4ffc, (uint32_t)(uintptr_t)text, 0, 0);
    }
    ConditionDialog_Store(w);
    if (mode == 2) {
        int32_t a = amount < 0 ? -amount : amount;
        Num_Format(text, 8, (double)a, 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4fc7, (uint32_t)(uintptr_t)text, 0, 0);
    } else {
        Num_Format(text, 8, (double)amount, 10, 1, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x4fce, (uint32_t)(uintptr_t)text, 0, 0);
    }
    RulesDialog_ShowLimits(w);
    return w;
}
RT3_REPLACE(0x4d6d60, ConditionDialog_Construct);

// 0x4d7060 (event handler). Condition dialog d: OK (0x4fb1) closes once its
// condition stores; Cancel (0x4fd4) closes; the value fields (0x4fbf..0x4fc1,
// 0x4fc7, 0x4fce, 0x503e / 0x5041 / 0x5044 / 0x5047) store and refresh, 0x4ff6
// updates the buttons and 0x5033 refills the condition types.
int32_t __fastcall ConditionDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (msg[0] == 0xca) {
        switch (msg[1]) {
        case 0x4fbf:
        case 0x4fc0:
        case 0x4fc1:
        case 0x4fc7:
        case 0x4fce:
        case 0x503e:
        case 0x5041:
        case 0x5044:
        case 0x5047:
            ConditionDialog_Store(d);
            ConditionDialog_ShowValue(d);
            break;
        case 0x4ff6:
            RulesDialog_UpdateButtons4ff7(d);
            break;
        case 0x5033:
            RulesDialog_FillConditionTypes(d, 0, F<int32_t>(d, 0x7c));
            break;
        }
        return 1;
    }
    if (msg[0] != 0xcb) return 1;
    if (msg[1] == 0x4fb1) {
        if (!ConditionDialog_Store(d)) return 0;
    } else if (msg[1] != 0x4fd4) {
        return 1;
    }
    msg[0] = 0xc9;
    return 1;
}
RT3_REPLACE(0x4d7060, ConditionDialog_HandleEvent);

// 0x4d71b0. Event text editor: lets the user pick a [tag] for `out`. Lists
// the usable tags of the tag table (0x5f3d90, 901 x 0x81 bytes: sort key +0,
// name +0xc, usable +0x5c, has variants +0x60, hidden +0x68) in key order
// (string 0x46e); on OK writes "[name]" (0x5d0d14), or for tags with
// variants "[name~~variant]" (0x5d0d1c) after picking one of the six variant
// names (0x620b6c, string 0xbbd). Quirk: the variant check uses the picked
// list index as a table index.
void __fastcall TextTag_Pick(void*, char* out) {
    *out = 0;
    const char** names = (const char**)Malloc(0xfa0);
    uint8_t used[0x385];
    memset(used, 0, sizeof used);
    uint8_t* tags = (uint8_t*)0x5f3d90;
    int32_t count = 0;
    for (int32_t pass = 0; pass < 0x385; ++pass) {
        int32_t best = -1;
        for (int32_t i = 0; i < 0x385; ++i) {
            uint8_t* t = tags + i * 0x81;
            if (F<int32_t>(t, 0x68) || !F<int32_t>(t, 0x5c) || used[i]) continue;
            if (best != -1 && !(F<float>(t, 0) < F<float>(tags + best * 0x81, 0))) continue;
            best = i;
        }
        if (best == -1) break;
        names[count++] = (const char*)tags + best * 0x81 + 0xc;
        used[best] = 1;
    }
    int32_t sel = 0;
    if (Dialog_RunMessage(GetString(0x46e), 2, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0, count, names, &sel, 0, 0, nullptr, 0) == 0x3f2) {
        char* variants[6] = {};
        for (int32_t k = 0; k < 6; ++k) String_Assign(&variants[k], GetString(((const int32_t*)0x620b6c)[k]));
        int32_t pick = 0;
        if (F<int32_t>(tags + sel * 0x81, 0x60))
            Dialog_RunMessage(GetString(0xbbd), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0, 6, (const char**)variants, &pick, 1, 0, nullptr, 0);
        if (pick) FormatText(out, (const char*)0x5d0d1c, names[sel], variants[pick], 0, 0, 0, 0, 0, 0);
        else FormatText(out, (const char*)0x5d0d14, names[sel], 0, 0, 0, 0, 0, 0, 0);
        for (int32_t k = 0; k < 6; ++k) Free(variants[k]);
    }
    Free(names);
}
RT3_REPLACE(0x4d71b0, TextTag_Pick);

// 0x4d76a0. "Assuming current map parameters of N likely companies, P likely players
// and T active territories." (string 0x477; P = world +0x6a6c, at least 1;
// companies = P * 1.5 truncated; T = territories with a positive +0x3d) on label
// 0x4eb4 of the city dialog (0x6cff84), if open.
RT3_NOINLINE void CityDialog_ShowMapParameters() {
    int32_t territories = Table_CountPositive3d(hook::Global<Collection*>(0x6cfc9c));
    int32_t players = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c);
    if (players <= 1) players = 1;
    double p = (double)players;
    char ts[0x40], ps[0x24], cs[0x30], text[0x2a0];
    Num_Format(ts, 1, (double)territories, 10, 1, 0, 0);
    Num_Format(ps, 1, p, 10, 1, 0, 0);
    Num_Format(cs, 1, (double)Ftol(p * D(0x5c8578)), 10, 1, 0, 0);
    FormatText(text, GetString(0x477), cs, ps, ts, 0, 0, 0, 0, 0);
    if (uint8_t* dlg = hook::Global<uint8_t*>(0x6cff84)) Window_SendEvent5(dlg, 0, 0x6e, 0x4eb4, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4d76a0, CityDialog_ShowMapParameters);

// 0x4d77b0. Enables the four buttons 0x4fed..0x4ff0 (0x65, else 0x87) when box
// 0x4e8f is ticked and radio group 0x5000 answers 0x5002 (in its third field; the
// original's query leaves that field uninitialized when nothing answers, zero here).
void __fastcall RulesDialog_UpdateButtons4fed(uint8_t* dlg) {
    uint8_t* box = Widget_FindChild(dlg, 0, 0x4e8f);
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x5000;
    Window_HandleEvent(dlg, 0, ev);
    uint32_t type = F<int32_t>(box, 0xcc) && ev[2] == 0x5002 ? 0x65 : 0x87;
    for (uint32_t id = 0x4fed; id <= 0x4ff0; ++id) Window_SendEvent5(dlg, 0, type, id, 3, 0, 0);
}
RT3_REPLACE(0x4d77b0, RulesDialog_UpdateButtons4fed);

RT3_NOINLINE void __fastcall Obj_SetTextE0(uint8_t* o, int, const char* s) { StrCopyN((char*)o + 0xe0, s, 0x320); }
RT3_REPLACE(0x4d78b0, Obj_SetTextE0);

// 0x4d78d0 (list callback). The condition dialog (0x6cff84) takes value v
// (EDX, +0x88); 0x63 for a condition whose cargo has flag 0x40 clear asks
// for the other button (event 0x68 / 0x4eae) instead.
// QUIRK: 0x63 without the dialog writes through a null pointer.
void __fastcall ConditionDialog_SetValueCb(void*, int32_t v) {
    if (v == 0x63) {
        uint8_t* dlg = hook::Global<uint8_t*>(0x6cff84);
        if (dlg && Collection_IsValid(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(dlg, 0x84))) {
            uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6cff84), 0x84));
            if (r && !(Load_CargoFlags(r) & 0x40)) {
                Window_SendEvent5(hook::Global<uint8_t*>(0x6cff84), 0, 0x68, 0x4eae, 0, 0, 0);
                return;
            }
        }
    }
    F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x88) = v;
}
RT3_REPLACE(0x4d78d0, ConditionDialog_SetValueCb);

// 0x4d7950 / 0x4d7960 / 0x4d7970 (callbacks). Store a register argument in
// 0x6cff84 +0x8c (EDX), +0x94 / +0x98 (ECX).
void __fastcall Set6cff84_8c(void*, int32_t v) { F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x8c) = v; }
RT3_REPLACE(0x4d7950, Set6cff84_8c);

void __fastcall Set6cff84_94(int32_t v) { F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x94) = v; }
RT3_REPLACE(0x4d7960, Set6cff84_94);

void __fastcall Set6cff84_98(int32_t v) { F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x98) = v; }
RT3_REPLACE(0x4d7970, Set6cff84_98);

// 0x4d7980 (draw callback). Scenario setup window (0x6cff84, scenario +0x84
// in 0x62be18): the number of possible start combinations (string 0x46f):
// for each kind the scenario lets vary (Load_CargoFlags 1 / 2 / 4) the
// choices its list widget allows (0x4fd7: all x 0x5c8578 / one / x - 1 of
// the companies 0x6cec78 +0x6a6c; 0x4fd8: of the companies; territories
// unless fixed, +0x88) as "n kind" (strings 0x470..0x475) joined by " X ",
// then " = total" or, with nothing varying, string 0x476.
void __fastcall ScenarioCombos_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* win = hook::Global<uint8_t*>(0x6cff84);
    if (!win || !Collection_IsValid(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(win, 0x84))) return;
    uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6cff84), 0x84));
    if (!s) return;
    Load_CargoFlags(s);
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    int32_t product = 1;
    int32_t companies = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x6a6c);
    if (companies <= 1) companies = 1;
    const int32_t all = Ftol((double)companies * D(0x5c8578));
    const int32_t territories = Table_CountPositive3d(hook::Global<Collection*>(0x6cfc9c));
    const uint8_t flags = Load_CargoFlags(s);
    char frame[0x6ac];
    char* text = frame + 0x260;
    char* num = frame + 0x50;
    char* part = frame + 0xd0;
    char* total = frame + 0x51c;
    char* totalNum = frame + 0x90;
    FormatText(text, GetString(0x46f), 0, 0, 0, 0, 0, 0, 0, 0);
    bool any = false;
    auto term = [&](int32_t n, int32_t many, int32_t one) {
        if (any) strcat(text, (const char*)0x5d0d30);
        const char* label = GetString(n > 1 ? many : one);
        Num_Format(num, 1, (double)n, 10, 1, 0, 0);
        FormatText(part, (const char*)0x5d0070, num, label, 0, 0, 0, 0, 0, 0);
        strcat(text, part);
    };
    if (flags & 1) {
        ev[1] = 0x4fd7;
        int32_t n = 1;
        Window_HandleEvent(hook::Global<uint8_t*>(0x6cff84), 0, ev);
        bool skip = false;
        switch (ev[2] - 0x4ec0) {
        case 0: n = all; break;
        case 1: skip = true; break;
        case 3: n = 1; break;
        case 4: n = all - 1; break;
        default: break;
        }
        if (!skip) {
            term(n, 0x470, 0x471);
            any = true;
            product = n;
        }
    }
    if (flags & 2) {
        ev[1] = 0x4fd8;
        int32_t n = 1;
        Window_HandleEvent(hook::Global<uint8_t*>(0x6cff84), 0, ev);
        bool skip = false;
        switch (ev[2] - 0x4eb6) {
        case 0: skip = true; break;
        case 1: n = companies; break;
        case 2: n = 1; break;
        case 3: n = companies - 1; break;
        default: break;
        }
        if (!skip) {
            term(n, 0x472, 0x473);
            product *= n;
            any = true;
        }
    }
    bool last = false;
    if (flags & 4) {
        const int32_t fixed = F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x88);
        if (fixed != 0x63) {
            const int32_t n = fixed ? 1 : territories;
            term(n, 0x474, 0x475);
            product *= n;
            last = true;
        }
    }
    if (last || any) {
        Num_Format(totalNum, 1, (double)product, 10, 1, 0, 0);
        FormatText(total, (const char*)0x5d0d28, totalNum, 0, 0, 0, 0, 0, 0, 0);
    } else {
        Num_Format(totalNum, 1, (double)product, 10, 1, 0, 0);
        FormatText(total, GetString(0x476), totalNum, 0, 0, 0, 0, 0, 0, 0);
    }
    strcat(text, total);
    const float tx = (float)((double)x + D(0x5d0b38));
    Font_DrawTextBox(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec78), 0x46a9c), 0, text, tx, y, BitsF(0x3e5950c8), h, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x4d7980, ScenarioCombos_DrawCb);

// 0x4d7ef0. Fills the rules dialog's list 0x4e84 with every scenario rule (0x62be18
// in list-2 order: name +0x18, id +0x14, plus a copy of the name), selects the first
// row when there is one and makes the first rule (+0x14, -1 if none) current
// (+0x84); +0x90 counts the rows.
RT3_NOINLINE void __fastcall RulesDialog_FillList(uint8_t* w) {
    int32_t first = -1;
    F<int32_t>(w, 0x90) = 0;
    F<int32_t>(w, 0x84) = -1;
    F<int32_t>(w, 0x88) = 0;
    F<int32_t>(w, 0x8c) = 0;
    Window_SendEvent5(w, 0, 0x8b, 0x4e84, 0, 0, 0);
    int32_t n = Collection_Count(hook::Global<Collection*>(0x62be18));
    for (int32_t i = 0; i < n; ++i) {
        Collection* rules = hook::Global<Collection*>(0x62be18);
        uint8_t* r = (uint8_t*)Collection_Get(rules, 0, Collection_IdAt(rules, 0, i, 2));
        if (i == 0) first = F<int32_t>(r, 0x14);
        char name[0x24c];
        strcpy(name, (const char*)r + 0x18);
        Window_SendEvent5(w, 0, 0x88, 0x4e84, (uint32_t)(uintptr_t)(r + 0x18), F<uint32_t>(r, 0x14), (uint32_t)(uintptr_t)name);
        ++F<int32_t>(w, 0x90);
    }
    if (F<int32_t>(w, 0x90) > 0) Window_SendEvent5(w, 0, 0x66, 0x4e84, 0, 0, 0);
    F<int32_t>(w, 0x84) = first;
}
RT3_REPLACE(0x4d7ef0, RulesDialog_FillList);

// 0x4d7fe0. Fills the territory lists 0x4eae / 0x500b: "all" (string 0x478, data
// 99, first list only) and "none" (0x479) entries, then every active territory
// (0x6cfc9c with +0x3d: name +4, id +0); then selects (0x68) in both.
RT3_NOINLINE void __fastcall RulesDialog_FillTerritories(uint8_t* w) {
    int32_t n = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
    Window_SendEvent5(w, 0, 0x8b, 0x4eae, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x500b, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x88, 0x4eae, (uint32_t)(uintptr_t)GetString(0x478), 0x63, 0);
    Window_SendEvent5(w, 0, 0x88, 0x4eae, (uint32_t)(uintptr_t)GetString(0x479), 0, 0);
    Window_SendEvent5(w, 0, 0x88, 0x500b, (uint32_t)(uintptr_t)GetString(0x479), 0, 0);
    for (int32_t i = 0; i < n; ++i) {
        Collection* regions = hook::Global<Collection*>(0x6cfc9c);
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (!F<int32_t>(r, 0x3d)) continue;
        Window_SendEvent5(w, 0, 0x88, 0x4eae, (uint32_t)(uintptr_t)(r + 4), F<uint32_t>(r, 0), 0);
        Window_SendEvent5(w, 0, 0x88, 0x500b, (uint32_t)(uintptr_t)(r + 4), F<uint32_t>(r, 0), 0);
    }
    Window_SendEvent5(w, 0, 0x68, 0x4eae, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x68, 0x500b, 0, 0, 0);
}
RT3_REPLACE(0x4d7fe0, RulesDialog_FillTerritories);

// 0x4d8120. Lists rule +0x84's trigger expression (chain at rule +0, next +0x1a;
// +0x94 = -1) in list 0x4eaf: -1 / -2 entries are fixed texts (0x47e / 0x47f);
// others show the trigger type's name (table 0x5f3d90, 0x81 bytes: unit +0x53,
// kind +0x54) with comparison +4 (=, !=, >, <, >=, <=, two string ones), value
// +9 (or condition +9 when +5 is set), and its city / territory arguments (+0xe /
// +0x12) for kinds 0 / 2 / 3, followed by the joiner to the next entry ("AND" /
// "OR" by the next one's +0xd, ") " where the grouping changes); the first row is
// prefixed with one "(" per group change. Quirks: unknown kinds (> 3) and
// comparisons (> 7) reuse the previous row's text / operator (the original's first
// row would show stale stack; empty here), and kind-0 names are used as format.
RT3_NOINLINE void __fastcall RulesDialog_FillTriggers(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x4eaf, 0, 0, 0);
    F<int32_t>(w, 0x94) = -1;
    if (F<int32_t>(w, 0x84) == -1) return;
    uint8_t* rule = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    uint8_t* head = F<uint8_t*>(rule, 0);
    uint8_t mark[500];
    memset(mark, 0, sizeof mark);
    int32_t groups = 0;
    {
        int32_t prev = -0x63, i = 0;
        for (uint8_t* n = head; n; n = F<uint8_t*>(n, 0x1a), ++i) {
            uint8_t* next = F<uint8_t*>(n, 0x1a);
            int32_t v = !next || next[0xd] == 2 ? -0x63 : next[0xd];
            if (v != -0x63 && prev != -0x63 && v != prev) {
                ++groups;
                mark[i] = 1;
            }
            prev = v;
        }
    }
    char text[0x518], op[0xd4];
    text[0] = 0;
    op[0] = 0;
    int32_t row = 0;
    for (uint8_t* n = head; n; n = F<uint8_t*>(n, 0x1a), ++row) {
        int32_t type = F<int32_t>(n, 0);
        if (type == -1 || type == -2) {
            FormatText(text, GetString(type == -1 ? 0x47e : 0x47f), 0, 0, 0, 0, 0, 0, 0, 0);
            Window_SendEvent5(w, 0, 0x88, 0x4eaf, (uint32_t)(uintptr_t)text, 0, 0);
            continue;
        }
        const uint8_t* rec = (const uint8_t*)0x5f3d90 + type * 0x81;
        const char* name = (const char*)rec + 0xc;
        char unit[0x14] = { 0 }, unit2[2] = { 0 }, sep[0x2c];
        if (rec[0x53] == 2) strcpy(unit, "$");
        if (rec[0x53] == 1) strcpy(unit, "%");
        switch (n[4]) {
        case 0: strcpy(op, "="); break;
        case 1: memcpy(op, (const void*)0x5d0d98, 3); break;
        case 2: strcpy(op, ">"); break;
        case 3: strcpy(op, "<"); break;
        case 4: memcpy(op, (const void*)0x5d0d94, 3); break;
        case 5: memcpy(op, (const void*)0x5d0d90, 3); break;
        case 6: strcpy(op, GetString(0xbbe)); break;
        case 7: strcpy(op, GetString(0xbbf)); break;
        }
        sep[0] = 0;
        if (mark[row]) memcpy(sep + strlen(sep), (const void*)0x5d0d8c, 3);
        uint8_t* next = F<uint8_t*>(n, 0x1a);
        if (next) {
            if (next[0xd] == 0) memcpy(sep + strlen(sep), (const void*)0x5d0d84, 4);
            else if (next[0xd] == 1) memcpy(sep + strlen(sep), (const void*)0x5d0d88, 3);
        }
        int32_t kind = *(const int32_t*)(rec + 0x54);
        if (kind == 1) {
            if (n[4] == 6 || n[4] == 7) FormatText(text, (const char*)0x5d0d5c, name, op, sep, 0, 0, 0, 0, 0);
            else if (F<int32_t>(n, 5)) {
                const char* cond = (const char*)0x6103a0 + F<int32_t>(n, 9) * 0x6e;
                FormatText(text, (const char*)0x5d0d78, name, op, cond, sep, 0, 0, 0, 0);
            } else {
                char num[0x40];
                int32_t style = type == 6 || type == 7 || type == 0x3c ? 8 : 1;
                Num_Format(num, style, (double)F<int32_t>(n, 9), 10, 1, 0, 0);
                FormatText(text, (const char*)0x5d0d68, name, op, unit, num, unit2, sep, 0, 0);
            }
        } else if (kind == 0 || kind == 2 || kind == 3) {
            char s1[0x1f4], s2[0x200], t2[0x200], num1[0x40], num2[0x40];
            s1[0] = 0;
            s2[0] = 0;
            if (type == 0x4b || type == 0x4d) {
                if (Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, F<uint32_t>(n, 0xe)))
                    ExeSprintf()(s1, (const char*)0x5d0d54,
                                 (const char*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, F<uint32_t>(n, 0xe)) + 4);
                else ExeSprintf()(s1, GetString(0xbc0));
            }
            if (type == 0x4c || type == 0x4e || type == 0x35f || type == 0x360) {
                if (Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(n, 0xe)))
                    ExeSprintf()(s1, (const char*)0x5d0d54,
                                 (const char*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(n, 0xe)) + 0x356);
                else ExeSprintf()(s1, GetString(0xbc1));
            }
            if (type == 0x35f || type == 0x360) {
                if (Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(n, 0x12)))
                    ExeSprintf()(s2, (const char*)0x5d0d54,
                                 (const char*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(n, 0x12)) + 0x356);
                else ExeSprintf()(s2, GetString(0xbc2));
            }
            if (kind == 0) FormatText(t2, name, 0, 0, 0, 0, 0, 0, 0, 0);
            else if (kind == 2)
                FormatText(t2, (const char*)0x5d0d48, name, Num_Format(num1, 1, (double)F<int32_t>(n, 0xe), 10, 1, 0, 0), s1, 0, 0, 0, 0, 0);
            else {
                char* b = Num_Format(num2, 1, (double)F<int32_t>(n, 0x12), 10, 1, 0, 0);
                char* a = Num_Format(num1, 1, (double)F<int32_t>(n, 0xe), 10, 1, 0, 0);
                FormatText(t2, (const char*)0x5d0d38, name, a, s1, b, s2, 0, 0, 0);
            }
            FormatText(text, GetString(F<int32_t>(n, 9) ? 0x481 : 0x480), t2, sep, 0, 0, 0, 0, 0, 0);
        }
        if (row == 0 && groups) {
            char pre[0x400];
            pre[0] = 0;
            for (int32_t g = 0; g < groups; ++g) memcpy(pre + strlen(pre), (const void*)0x5d0d34, 2);
            memcpy(pre + strlen(pre), (const void*)0x5d09c4, 2);
            strcat(pre, text);
            strcpy(text, pre);
            groups = 0;
        }
        Window_SendEvent5(w, 0, 0x88, 0x4eaf, (uint32_t)(uintptr_t)text, 0, 0);
    }
    if (F<uint8_t*>(rule, 0)) Window_SendEvent5(w, 0, 0x66, 0x4eaf, 0, 0, 0);
}
RT3_REPLACE(0x4d8120, RulesDialog_FillTriggers);

// 0x4d88f0. Lists the conditions of slot +0x9c of rule +0x84 (0x62be18, chain at
// +4 + slot * 4, next +0x24) in list 0x4ed5 (+0x98 = -1): the condition type's name
// (table 0x610398, 0x6e bytes, name +8, unit +0x63: 1 "%", 2 "$") with, by kind
// (+8): 1 a yes / no text (0x482 / 0x483 by +4), 2 a signed amount ("%1 +%2%3%4"
// or "%1 %2%3%4"), 4 an amount (0x485), 8 a date (+0x11 / +0x12 / +0x14 / +0x16,
// 0xbc3); then a range part when +9 != -1 (0x484 "at most +0xd", 0x486 "at least
// +9", or both through 0x487). Quirk: kinds 3 / 5..7 format nothing and resend the
// previous row's text (on the first row the original sends stale stack; empty here).
RT3_NOINLINE void __fastcall RulesDialog_FillConditions(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    uint8_t* rule = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    uint8_t* n = F<uint8_t*>(rule, 4 + F<int32_t>(w, 0x9c) * 4);
    Window_SendEvent5(w, 0, 0x8b, 0x4ed5, 0, 0, 0);
    F<int32_t>(w, 0x98) = -1;
    char text[0x200];
    text[0] = 0;
    for (; n; n = F<uint8_t*>(n, 0x24)) {
        char pct[2] = { 0, 0 }, cur[2] = { 0, 0 };
        const uint8_t* type = (const uint8_t*)0x610398 + F<int32_t>(n, 0) * 0x6e;
        if (type[0x63] == 1) pct[0] = '%';
        else if (type[0x63] == 2) cur[0] = '$';
        const char* name = (const char*)type + 8;
        char num[0x20];
        switch (n[8]) {
        case 1: FormatText(text, GetString(F<int32_t>(n, 4) ? 0x483 : 0x482), name, 0, 0, 0, 0, 0, 0, 0); break;
        case 2:
        case 4: {
            char* s = Num_Format(num, 1, (double)F<int32_t>(n, 4), 10, 1, 0, 0);
            const char* fmt = n[8] == 4 ? GetString(0x485) : F<int32_t>(n, 4) < 0 ? (const char*)0x5d0da8 : (const char*)0x5d0d9c;
            FormatText(text, fmt, name, cur, s, pct, 0, 0, 0, 0);
            break;
        }
        case 8: {
            char a[0x20], b[0x20], c[0x20], d[0x20];
            char* sa = Num_Format(a, 1, (double)F<uint16_t>(n, 0x16), 10, 1, 0, 0);
            char* sb = Num_Format(b, 1, (double)F<uint16_t>(n, 0x14), 10, 1, 0, 0);
            char* sc = Num_Format(c, 1, (double)F<uint16_t>(n, 0x12), 10, 1, 0, 0);
            char* sd = Num_Format(d, 1, (double)n[0x11], 10, 1, 0, 0);
            FormatText(text, GetString(0xbc3), name, sd, sc, sb, sa, 0, 0, 0);
            break;
        }
        }
        int32_t lo = F<int32_t>(n, 9);
        if (lo != -1) {
            char range[0x50], part[0x50], hi[0x20];
            int32_t up = F<int32_t>(n, 0xd);
            if (!lo) FormatText(range, GetString(0x484), Num_Format(num, 1, (double)up, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
            else if (!up) FormatText(range, GetString(0x486), Num_Format(num, 1, (double)lo, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
            else {
                FormatText(part, GetString(0x486), Num_Format(num, 1, (double)lo, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
                FormatText(range, GetString(0x487), part, Num_Format(hi, 1, (double)up, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
            }
            strcat(text, range);
        }
        Window_SendEvent5(w, 0, 0x88, 0x4ed5, (uint32_t)(uintptr_t)text, 0, 0);
    }
    if (F<uint8_t*>(rule, 4 + F<int32_t>(w, 0x9c) * 4)) Window_SendEvent5(w, 0, 0x66, 0x4ed5, 0, 0, 0);
}
RT3_REPLACE(0x4d88f0, RulesDialog_FillConditions);

// 0x4d8d50. Stores the game-rules dialog's choices into the rule record (0x62be18,
// id +0x84; slot +0x9c): the value of child 0x500a, the option picked in radio
// group 0x5006 (its answer, 0x5014..0x501c in the third field, becomes 0..8) and
// +0x8c. The original's query event leaves its other fields uninitialized, so with
// no answer it reads a stale value there (zero here: nothing is stored).
RT3_NOINLINE void __fastcall RulesDialog_Store(uint8_t* dlg) {
    if (F<int32_t>(dlg, 0x84) == -1) return;
    uint8_t* rule = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(dlg, 0x84));
    F<int32_t>(rule, 0x7ff + F<int32_t>(dlg, 0x9c) * 4) = F<int32_t>(Widget_FindChild(dlg, 0, 0x500a), 0xcc);
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x5006;
    Window_HandleEvent(dlg, 0, ev);
    uint32_t pick = ev[2] - 0x5014;
    if (pick <= 8) rule[0x7fb + F<int32_t>(dlg, 0x9c)] = (uint8_t)pick;
    F<int32_t>(rule, 0x80f + F<int32_t>(dlg, 0x9c) * 4) = F<int32_t>(dlg, 0x8c);
}
RT3_REPLACE(0x4d8d50, RulesDialog_Store);

// 0x4d8ea0. Stores the rules dialog's settings into rule +0x84 (0x62be18): the
// message kind (+0x7ee, radios 0x4e8e..0x4e92) with its texts (+0xe0 800 chars;
// kind 1 also four 200-char lines +0x4ca.. from 0x4fdf..0x4fe2, kind 2 +0x401 from
// 0x4edd), the first ticked of 0x4e98..0x4ea2 (+0x7ef), the answers of radio
// groups 0x4fd7 / 0x4fd8 mapped to +0x7f9 / +0x7fa, box 0x4ead (+0x7f5), +0x88
// (+0x7f0), the slot settings (RulesDialog_Store) and the first ticked of
// 0x4eca..0x4ecc (+0x7f4). Returns 1. The original's query event leaves its other
// fields uninitialized and both queries share it (zero here).
RT3_NOINLINE int32_t __fastcall RulesDialog_StoreRule(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return 1;
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    auto ticked = [w](uint32_t id) { return F<int32_t>(Widget_FindChild(w, 0, id), 0xcc) != 0; };
    auto text = [w](uint32_t id) { return F<const char*>(Widget_FindChild(w, 0, id), 0xba); };
    if (ticked(0x4e8e)) {
        r[0x7ee] = 0;
        StrCopyN((char*)r + 0xe0, text(0x4eac), 0x320);
    } else if (ticked(0x4e8f)) {
        r[0x7ee] = 1;
        StrCopyN((char*)r + 0xe0, text(0x4ed9), 0x320);
        StrCopyN((char*)r + 0x4ca, text(0x4fdf), 0xc8);
        StrCopyN((char*)r + 0x593, text(0x4fe0), 0xc8);
        StrCopyN((char*)r + 0x65c, text(0x4fe1), 0xc8);
        StrCopyN((char*)r + 0x725, text(0x4fe2), 0xc8);
    } else if (ticked(0x4e90)) {
        r[0x7ee] = 2;
        StrCopyN((char*)r + 0xe0, text(0x4edb), 0x320);
        StrCopyN((char*)r + 0x401, text(0x4edd), 0xc8);
    } else if (ticked(0x4e91)) {
        r[0x7ee] = 3;
        Obj_SetTextE0(r, 0, text(0x4eac));
    } else if (ticked(0x4e92)) {
        r[0x7ee] = 4;
        Obj_SetTextE0(r, 0, text(0x4eac));
    }
    for (int32_t i = 0; i <= 10; ++i)
        if (ticked(0x4e98 + i)) {
            r[0x7ef] = (uint8_t)i;
            break;
        }
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x4fd7;
    Window_HandleEvent(w, 0, ev);
    static const uint8_t map1[5] = { 1, 0, 2, 2, 3 }, map2[5] = { 0, 1, 2, 3, 2 };
    if (ev[2] - 0x4ec0 <= 4) r[0x7f9] = map1[ev[2] - 0x4ec0];
    ev[0] = 0x94;
    ev[1] = 0x4fd8;
    Window_HandleEvent(w, 0, ev);
    if (ev[2] - 0x4eb6 <= 4) r[0x7fa] = map2[ev[2] - 0x4eb6];
    F<int32_t>(r, 0x7f5) = F<int32_t>(Widget_FindChild(w, 0, 0x4ead), 0xcc);
    F<int32_t>(r, 0x7f0) = F<int32_t>(w, 0x88);
    RulesDialog_Store(w);
    for (int32_t i = 0; i < 3; ++i)
        if (ticked(0x4eca + i)) {
            r[0x7f4] = (uint8_t)i;
            break;
        }
    return 1;
}
RT3_REPLACE(0x4d8ea0, RulesDialog_StoreRule);

// 0x4d9360. Deletes the rules dialog's current rule (if any) and refills the list.
RT3_NOINLINE void __fastcall RulesDialog_DeleteRule(uint8_t* dlg) {
    if (F<int32_t>(dlg, 0x84) == -1) return;
    Collection* rules = hook::Global<Collection*>(0x62be18);
    ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)rules)[2])(rules, 0, F<uint32_t>(dlg, 0x84));
    RulesDialog_FillList(dlg);
}
RT3_REPLACE(0x4d9360, RulesDialog_DeleteRule);

// 0x4d9390. Enables the rules dialog's message controls for the picked message
// kind: plain text (0x4e8e / 0x4e91 / 0x4e92: 0x4ed7 / 0x4eac), a newspaper (0x4e8f:
// 0x4ed8 / 0x4ed9 and lines 0x4fdb..0x4fe6) or a two-part text (0x4e90: 0x4eda..
// 0x4edd), the rest disabled; then the buttons. Kinds other than the newspaper also
// restart at slot 0, refill the conditions and tick 0x4fed. Nothing without a pick.
RT3_NOINLINE void __fastcall RulesDialog_ShowMessageKind(uint8_t* w) {
    static const uint16_t plain[2] = { 0x4ed7, 0x4eac };
    static const uint16_t paper[2] = { 0x4ed8, 0x4ed9 };
    static const uint16_t split[4] = { 0x4eda, 0x4edb, 0x4edc, 0x4edd };
    auto show = [w](uint32_t onPlain, uint32_t onPaper, uint32_t onSplit) {
        for (uint16_t id : plain) Window_SendEvent5(w, 0, onPlain, id, 3, 0, 0);
        for (uint16_t id : paper) Window_SendEvent5(w, 0, onPaper, id, 3, 0, 0);
        for (uint32_t id = 0x4fdb; id <= 0x4fe6; ++id) Window_SendEvent5(w, 0, onPaper, id, 3, 0, 0);
        for (uint16_t id : split) Window_SendEvent5(w, 0, onSplit, id, 3, 0, 0);
    };
    uint8_t* a = Widget_FindChild(w, 0, 0x4e8e);
    uint8_t* b = Widget_FindChild(w, 0, 0x4e92);
    if (!F<int32_t>(a, 0xcc) && !F<int32_t>(b, 0xcc)) {
        if (F<int32_t>(Widget_FindChild(w, 0, 0x4e8f), 0xcc)) {
            show(0x87, 0x65, 0x87);
            RulesDialog_UpdateButtons4fed(w);
            return;
        }
        if (F<int32_t>(Widget_FindChild(w, 0, 0x4e90), 0xcc)) show(0x87, 0x87, 0x65);
        else if (F<int32_t>(Widget_FindChild(w, 0, 0x4e91), 0xcc)) show(0x65, 0x87, 0x87);
        else return;
    } else show(0x65, 0x87, 0x87);
    RulesDialog_UpdateButtons4fed(w);
    F<int32_t>(w, 0x9c) = 0;
    RulesDialog_FillConditions(w);
    Window_SendEvent5(w, 0, 0x74, 0x4fed, 1, 0, 0);
}
RT3_REPLACE(0x4d9390, RulesDialog_ShowMessageKind);

// 0x4d9970. Enables the rules dialog's scope controls for rule +0x84 (0x62be18)
// while box 0x5001 is ticked, by the flags of its conditions (Load_CargoFlags):
// 4 the territory list 0x4eae (else reset to row 0 and disabled), any flag label
// 0x4eb4; flag 1 the company group 0x4fd9 / 0x4ec0..0x4ec4 (0x4ec1 only with 0x10),
// flag 2 the player group 0x4fda / 0x4eb6..0x4eba (0x4eb6 only with 0x20). Without
// rule +0x7f4 the "this one" choice (0x4ec3 / 0x4eb8) is disabled and, when it was
// picked, moved to 0x4ec2 / 0x4eba; a disabled group falls back to 0x4ec0 / 0x4eb7.
RT3_NOINLINE void __fastcall RulesDialog_UpdateScope(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    if (!F<int32_t>(Widget_FindChild(w, 0, 0x5001), 0xcc)) return;
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    uint8_t f = Load_CargoFlags(r);
    if (f & 4) Window_SendEvent5(w, 0, 0x65, 0x4eae, 3, 0, 0);
    else {
        Window_SendEvent5(w, 0, 0x66, 0x4eae, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x4eae, 3, 0, 0);
    }
    Window_SendEvent5(w, 0, f ? 0x65 : 0x87, 0x4eb4, 3, 0, 0);
    struct Group {
        uint8_t bit, extraBit;
        uint32_t head, extra, fallback, self, alt, last;
    };
    const Group groups[2] = { { 1, 0x10, 0x4fd9, 0x4ec1, 0x4ec0, 0x4ec3, 0x4ec4, 0x4ec2 },
                              { 2, 0x20, 0x4fda, 0x4eb6, 0x4eb7, 0x4eb8, 0x4eb9, 0x4eba } };
    for (const Group& g : groups) {
        if (f & g.bit) {
            Window_SendEvent5(w, 0, 0x65, g.head, 3, 0, 0);
            Window_SendEvent5(w, 0, f & g.extraBit ? 0x65 : 0x87, g.extra, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x65, g.fallback, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x65, g.self, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x65, g.alt, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x65, g.last, 3, 0, 0);
            if (!r[0x7f4]) {
                Window_SendEvent5(w, 0, 0x87, g.self, 3, 0, 0);
                if (F<int32_t>(Widget_FindChild(w, 0, g.self), 0xcc)) Window_SendEvent5(w, 0, 0x74, g.last, 1, 0, 0);
            }
        } else {
            Window_SendEvent5(w, 0, 0x74, g.fallback, 1, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.head, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.extra, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.fallback, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.self, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.alt, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x87, g.last, 3, 0, 0);
        }
    }
}
RT3_REPLACE(0x4d9970, RulesDialog_UpdateScope);

// 0x4d9d10. Enables button 0x500b of the rules dialog (0x65, else 0x87) when the
// rule's slot +0x9c has list flag 4 and controls 0x5019 and 0x5002 are both set.
// (It first sends the radio group a query event, ignoring the answer; the
// original leaves the event's other fields uninitialized, zero here.)
RT3_NOINLINE void __fastcall RulesDialog_UpdateButton(uint8_t* dlg) {
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x5006;
    Window_HandleEvent(dlg, 0, ev);
    uint8_t flags = 0;
    if (F<int32_t>(dlg, 0x84) != -1)
        flags = Obj_ListFlags(Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(dlg, 0x84)), 0, F<int32_t>(dlg, 0x9c), 0);
    bool on = (flags & 4) && F<int32_t>(Widget_FindChild(dlg, 0, 0x5019), 0xcc) && F<int32_t>(Widget_FindChild(dlg, 0, 0x5002), 0xcc);
    Window_SendEvent5(dlg, 0, on ? 0x65 : 0x87, 0x500b, 3, 0, 0);
}
RT3_REPLACE(0x4d9d10, RulesDialog_UpdateButton);

// 0x4d9dc0. Runs the rules dialog (0x88 bytes) for `rule`; returns its result.
RT3_NOINLINE uint32_t __stdcall RulesDialog_Run(uint32_t a, int32_t* rule) {
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    w = w ? RulesDialog_Construct(w, 0, a, rule) : nullptr;
    return Dialog_RunAndFree(w, &Widget4d4840_Destruct);
}
RT3_REPLACE(0x4d9dc0, RulesDialog_Run);

// 0x4d9e40. Runs the condition dialog (0x80 bytes) for record r; returns its result.
RT3_NOINLINE uint32_t __stdcall ConditionDialog_Run(uint8_t* r) {
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x80);
    w = w ? ConditionDialog_Construct(w, 0, r) : nullptr;
    return Dialog_RunAndFree(w, &Widget4d5b90_Destruct);
}
RT3_REPLACE(0x4d9e40, ConditionDialog_Run);

RT3_REPLACE(0x4d9ec0, RulesDialog_StoreRule);

// 0x4d9ed0. Deletes trigger row +0x94 of rule +0x84 (chain at rule +0, next
// +0x1a; nothing when the row doesn't exist), then refreshes the trigger list and
// the scope controls.
RT3_NOINLINE void __fastcall RulesDialog_DeleteTrigger(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1 || F<int32_t>(w, 0x94) == -1) return;
    uint8_t* rule = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    uint8_t** head = (uint8_t**)rule;
    uint8_t *prev = nullptr, *cur = *head;
    bool found = true;
    for (int32_t i = 0; i < F<int32_t>(w, 0x94); ++i) {
        if (!cur) {
            found = false;
            break;
        }
        prev = cur;
        cur = F<uint8_t*>(cur, 0x1a);
    }
    if (found && cur) {
        if (prev) F<uint8_t*>(prev, 0x1a) = F<uint8_t*>(cur, 0x1a);
        else *head = F<uint8_t*>(cur, 0x1a);
        ExeDelete(cur);
    }
    RulesDialog_FillTriggers(w);
    RulesDialog_UpdateScope(w);
}
RT3_REPLACE(0x4d9ed0, RulesDialog_DeleteTrigger);

// 0x4d9f50. Shows rule +0x84's message (sic: no check for "no rule"): its kind
// (+0x7ee, radios 0x4e8e..0x4e92), the radios enabled unless the last kind is
// picked (then only that one), the kind's controls and the texts (+0xe0 on 0x4eac /
// 0x4ed9 / 0x4edb, the newspaper lines +0x4ca.. on 0x4fdf..0x4fe2, +0x401 on
// 0x4edd).
RT3_NOINLINE void __fastcall RulesDialog_ShowMessage(uint8_t* w) {
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    if (r[0x7ee] <= 4) Window_SendEvent5(w, 0, 0x74, 0x4e8e + r[0x7ee], 1, 0, 0);
    bool last = F<int32_t>(Widget_FindChild(w, 0, 0x4e92), 0xcc) != 0;
    for (uint32_t id = 0x4e8e; id <= 0x4e92; ++id) {
        bool on = id == 0x4e92 ? last : !last;
        Window_SendEvent5(w, 0, on ? 0x65 : 0x87, id, 3, 0, 0);
    }
    RulesDialog_ShowMessageKind(w);
    const uint16_t ids[8] = { 0x4eac, 0x4ed9, 0x4fdf, 0x4fe0, 0x4fe1, 0x4fe2, 0x4edb, 0x4edd };
    const uint16_t offs[8] = { 0xe0, 0xe0, 0x4ca, 0x593, 0x65c, 0x725, 0xe0, 0x401 };
    for (int k = 0; k < 8; ++k) Window_SendEvent5(w, 0, 0x6f, ids[k], (uint32_t)(uintptr_t)(r + offs[k]), 0, 0);
}
RT3_REPLACE(0x4d9f50, RulesDialog_ShowMessage);

// 0x4da0f0. Updates the rules dialog's slot controls (re-entry guard 0x6d07d4)
// while box 0x5002 is ticked, by rule +0x84's slot +0x9c condition flags: with
// conditions listed (0x4ed5 +0x1d2) and a target kind (Record_AnyKind7) the target
// line 0x5014 reads "... <kinds>" (string 0x48c; parts 0x488..0x48b joined by
// 0x5d0db4) and 0x5005 / 0x500a / 0x5014 are enabled; target groups 0x5015 (flag
// 8), 0x5016..0x5018 (1), 0x501a..0x501c (2), 0x500b / 0x5019 (4; else the list
// reselects +0x8c and is disabled); a picked target radio (group 0x5006) that is
// hidden moves to the first visible of 0x5014..0x501c. Then the main buttons.
RT3_NOINLINE void __fastcall RulesDialog_UpdateSlot(uint8_t* w) {
    if (hook::Global<int32_t>(0x6d07d4)) return;
    hook::Global<int32_t>(0x6d07d4) = 1;
    uint8_t flags = 0;
    int32_t on = F<int32_t>(Widget_FindChild(w, 0, 0x5002), 0xcc);
    uint8_t* r = nullptr;
    if (F<int32_t>(w, 0x84) != -1) {
        r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
        flags = Obj_ListFlags(r, 0, F<int32_t>(w, 0x9c), 0);
    }
    if (F<int32_t>(Widget_FindChild(w, 0, 0x4ed5), 0x1d2) && r && Record_AnyKind7(r) && on) {
        char kinds[0x1f8], text[0x3c8];
        kinds[0] = 0;
        auto sep = [&]() {
            if (strlen(kinds)) memcpy(kinds + strlen(kinds), (const void*)0x5d0db4, 4);
        };
        if (Record_AnyKind1(r)) strcat(kinds, GetString(0x488));
        if (Record_AnyKind2(r)) {
            sep();
            strcat(kinds, GetString(0x489));
        } else if (Record_AnyKind1(r) && (Obj_ListFlags(r, 0, F<int32_t>(w, 0x9c), 0) & 2)) {
            sep();
            strcat(kinds, GetString(0x48a));
        }
        if (Record_AnyKind4(r)) {
            sep();
            strcat(kinds, GetString(0x48b));
        }
        FormatText(text, GetString(0x48c), kinds, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x5014, (uint32_t)(uintptr_t)text, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x5005, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x500a, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x5014, 3, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x87, 0x5005, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x500a, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x5014, 3, 0, 0);
    }
    Window_SendEvent5(w, 0, (flags & 8) && on ? 0x65 : 0x87, 0x5015, 3, 0, 0);
    uint32_t t1 = (flags & 1) && on ? 0x65 : 0x87, t2 = (flags & 2) && on ? 0x65 : 0x87;
    for (uint32_t id = 0x5016; id <= 0x5018; ++id) Window_SendEvent5(w, 0, t1, id, 3, 0, 0);
    for (uint32_t id = 0x501a; id <= 0x501c; ++id) Window_SendEvent5(w, 0, t2, id, 3, 0, 0);
    if ((flags & 4) && on) {
        Window_SendEvent5(w, 0, 0x65, 0x500b, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x65, 0x5019, 3, 0, 0);
    } else {
        if (F<int32_t>(w, 0x8c) >= 0) Window_SendEvent5(w, 0, 0x68, 0x500b, F<uint32_t>(w, 0x8c), 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x500b, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x5019, 3, 0, 0);
    }
    if (flags && on) {
        uint32_t ev[12] = {};
        ev[0] = 0x94;
        ev[1] = 0x5006;
        ev[2] = 0;
        Window_HandleEvent(w, 0, ev);
        if (ev[2] && !Widget_IsVisible(Widget_FindChild(w, 0, ev[2])))
            for (uint32_t id = 0x5014; id <= 0x501c; ++id)
                if (Widget_IsVisible(Widget_FindChild(w, 0, id))) {
                    Window_SendEvent5(w, 0, 0x74, id, 1, 0, 0);
                    break;
                }
    }
    RulesDialog_UpdateButton(w);
    hook::Global<int32_t>(0x6d07d4) = 0;
}
RT3_REPLACE(0x4da0f0, RulesDialog_UpdateSlot);

// 0x4da640. Event editor w: adds a rule (0x1e bytes, kind 0xe, flag 4) to the
// selected event (+0x84 in 0x62be18) after the rules dialog accepts it (0x4f73);
// a rule of negative kind replaces the whole list. Not after a terminal rule.
RT3_NOINLINE void __fastcall EventEditor_AddRule(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    uint8_t** ev = EventEditor_Event(w);
    if (ev[0] && F<int32_t>(ev[0], 0) < 0) return;
    uint8_t* r = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x1e);
    memset(r, 0, 0x1e);
    r[4] = 4;
    F<int32_t>(r, 0) = 0xe;
    if (RulesDialog_Run(ev[0] == nullptr, (int32_t*)r) != 0x4f73) return;
    if (F<int32_t>(r, 0) < 0) List1a_Free(ev);
    EventEditor_Append(ev, r, 0x1a);
    RulesDialog_FillTriggers(w);
    RulesDialog_UpdateScope(w);
}
RT3_REPLACE(0x4da640, EventEditor_AddRule);

// 0x4da700. Event editor w: edits rule +0x94 of the selected event; a rule
// turned negative replaces the list with (a copy of) itself.
RT3_NOINLINE void __fastcall EventEditor_EditRule(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1 || F<int32_t>(w, 0x94) == -1) return;
    uint8_t** ev = EventEditor_Event(w);
    uint8_t* head = ev[0];
    uint8_t* r = head;
    for (int32_t i = 0; i < F<int32_t>(w, 0x94); ++i) {
        if (!r) return;
        r = F<uint8_t*>(r, 0x1a);
    }
    if (!r) return;
    uint8_t* next = F<uint8_t*>(r, 0x1a);
    if (RulesDialog_Run(r == head, (int32_t*)r) != 0x4f73) return;
    if (F<int32_t>(r, 0) < 0) {
        uint8_t* copy = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x1e);
        memcpy(copy, r, 0x1e);
        List1a_Free(ev);
        ev[0] = copy;
    } else
        F<uint8_t*>(r, 0x1a) = next;
    RulesDialog_FillTriggers(w);
    RulesDialog_UpdateScope(w);
}
RT3_REPLACE(0x4da700, EventEditor_EditRule);

// 0x4da7c0. Event editor w: adds a condition (0x28 bytes, kind 2) to list
// +0x9c of the selected event after the condition dialog accepts it (0x4fb1).
RT3_NOINLINE void __fastcall EventEditor_AddCondition(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    uint8_t** ev = EventEditor_Event(w);
    uint8_t* c = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x28);
    memset(c, 0, 0x28);
    F<int32_t>(c, 0) = 2;
    if (ConditionDialog_Run(c) != 0x4fb1) return;
    EventEditor_Append(&ev[1 + F<int32_t>(w, 0x9c)], c, 0x24);
    RulesDialog_FillConditions(w);
    RulesDialog_UpdateSlot(w);
}
RT3_REPLACE(0x4da7c0, EventEditor_AddCondition);

// 0x4da860. Event editor w: edits condition +0x98 of list +0x9c, restoring
// it when the dialog is cancelled.
RT3_NOINLINE void __fastcall EventEditor_EditCondition(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1 || F<int32_t>(w, 0x98) == -1) return;
    uint8_t** ev = EventEditor_Event(w);
    uint8_t* c = ev[1 + F<int32_t>(w, 0x9c)];
    for (int32_t i = 0; i < F<int32_t>(w, 0x98); ++i) {
        if (!c) return;
        c = F<uint8_t*>(c, 0x24);
    }
    if (!c) return;
    uint8_t* next = F<uint8_t*>(c, 0x24);
    uint8_t backup[0x28];
    memcpy(backup, c, sizeof backup);
    if (ConditionDialog_Run(c) == 0x4fb1) {
        F<uint8_t*>(c, 0x24) = next;
        RulesDialog_FillConditions(w);
        RulesDialog_UpdateSlot(w);
        return;
    }
    memcpy(c, backup, sizeof backup);
    F<uint8_t*>(c, 0x24) = next;
}
RT3_REPLACE(0x4da860, EventEditor_EditCondition);

// 0x4da920. Deletes condition row +0x98 of rule +0x84's slot +0x9c (chain at rule
// +4 + slot * 4, next +0x24; nothing when the row doesn't exist), then refreshes
// the list and the slot controls.
RT3_NOINLINE void __fastcall RulesDialog_DeleteCondition(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1 || F<int32_t>(w, 0x98) == -1) return;
    uint8_t* rule = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    uint8_t** head = (uint8_t**)(rule + 4 + F<int32_t>(w, 0x9c) * 4);
    uint8_t *prev = nullptr, *cur = *head;
    bool found = true;
    for (int32_t i = 0; i < F<int32_t>(w, 0x98); ++i) {
        if (!cur) {
            found = false;
            break;
        }
        prev = cur;
        cur = F<uint8_t*>(cur, 0x24);
    }
    if (found && cur) {
        if (prev) F<uint8_t*>(prev, 0x24) = F<uint8_t*>(cur, 0x24);
        else *head = F<uint8_t*>(cur, 0x24);
        ExeDelete(cur);
    }
    RulesDialog_FillConditions(w);
    RulesDialog_UpdateSlot(w);
}
RT3_REPLACE(0x4da920, RulesDialog_DeleteCondition);

// 0x4da9a0. Shows rule +0x84's slot +0x9c settings: box 0x500a (+0x7ff == 1), the
// territory list selection (+0x80f into +0x8c, list 0x500b when >= 0) and the
// target radio 0x5014 + (+0x7fb, 0..8); then the slot controls.
RT3_NOINLINE void __fastcall RulesDialog_ShowSlot(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    Window_SendEvent5(w, 0, 0x74, 0x500a, F<int32_t>(r, 0x7ff + F<int32_t>(w, 0x9c) * 4) == 1, 0, 0);
    F<int32_t>(w, 0x8c) = F<int32_t>(r, 0x80f + F<int32_t>(w, 0x9c) * 4);
    if (F<int32_t>(w, 0x8c) >= 0) Window_SendEvent5(w, 0, 0x68, 0x500b, F<uint32_t>(w, 0x8c), 0, 0);
    uint8_t target = r[0x7fb + F<int32_t>(w, 0x9c)];
    if (target <= 8) Window_SendEvent5(w, 0, 0x74, 0x5014 + target, 1, 0, 0);
    RulesDialog_UpdateSlot(w);
}
RT3_REPLACE(0x4da9a0, RulesDialog_ShowSlot);

// 0x4dab60. Switches the rules dialog between its two pages by radio group 0x5000:
// page 0x5001 enables the scope controls and disables the slot ones, any other
// answer the reverse; then refreshes the scope (0x5001) or the slot and buttons
// (0x5002). The original's query event leaves its other fields uninitialized
// (zero here).
RT3_NOINLINE void __fastcall RulesDialog_SwitchPage(uint8_t* w) {
    uint32_t ev[12] = {};
    ev[0] = 0x94;
    ev[1] = 0x5000;
    Window_HandleEvent(w, 0, ev);
    static const uint16_t scope[] = { 0x4eb4, 0x4ead, 0x4fd9, 0x4ec1, 0x4ec0, 0x4ec3, 0x4ec4, 0x4ec2, 0x4eae, 0x4fda, 0x4eb6, 0x4eb7,
                                      0x4eb8, 0x4eb9, 0x4eba, 0x4eb0, 0x4eb1, 0x4eb2, 0x4eb3, 0x4eca, 0x4ecb, 0x4ecc, 0x5004, 0x4eaf };
    static const uint16_t slot[] = { 0x5003, 0x4ed4, 0x4fd5, 0x4fed, 0x4fee, 0x4fef, 0x4ff0, 0x4ed5, 0x5005, 0x500a, 0x5014,
                                     0x5015, 0x5016, 0x5017, 0x5018, 0x500b, 0x5019, 0x501a, 0x501b, 0x501c, 0x5024 };
    uint32_t t = ev[2] == 0x5001 ? 0x65 : 0x87;
    for (uint16_t id : scope) Window_SendEvent5(w, 0, t, id, 3, 0, 0);
    t = ev[2] == 0x5001 ? 0x87 : 0x65;
    for (uint16_t id : slot) Window_SendEvent5(w, 0, t, id, 3, 0, 0);
    if (ev[2] == 0x5001) RulesDialog_UpdateScope(w);
    else if (ev[2] == 0x5002) {
        RulesDialog_UpdateSlot(w);
        RulesDialog_UpdateButtons4fed(w);
    }
}
RT3_REPLACE(0x4dab60, RulesDialog_SwitchPage);

// 0x4daf40. Resets the rules dialog for a new rule: empty trigger list and texts,
// default radios (0x4e8e, 0x4e98, 0x4eca, 0x4ec1, 0x4eb6, 0x4fed and page 0x622074;
// 0x4ead / 0x500a off), first rows of 0x4eae / 0x500b, slot 0; then refreshes the
// page, message, scope and slot controls.
RT3_NOINLINE void __fastcall RulesDialog_Reset(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x8b, 0x4eaf, 0, 0, 0);
    const uint16_t texts[8] = { 0x4eac, 0x4ed9, 0x4fdf, 0x4fe0, 0x4fe1, 0x4fe2, 0x4edb, 0x4edd };
    for (uint16_t id : texts) Window_SendEvent5(w, 0, 0x6f, id, 0x5c87a8, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4e8e, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4e98, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4eca, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4ead, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4ec1, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4eb6, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x68, 0x4eae, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x68, 0x500b, 0, 0, 0);
    F<int32_t>(w, 0x9c) = 0;
    Window_SendEvent5(w, 0, 0x74, 0x4fed, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, hook::Global<uint32_t>(0x622074), 1, 0, 0);
    RulesDialog_SwitchPage(w);
    Window_SendEvent5(w, 0, 0x74, 0x500a, 0, 0, 0);
    RulesDialog_ShowMessageKind(w);
    RulesDialog_UpdateScope(w);
    RulesDialog_UpdateSlot(w);
}
RT3_REPLACE(0x4daf40, RulesDialog_Reset);

// 0x4db120. Refreshes the rules dialog for the selected rule (+0x84 in
// 0x62be18): with none, every control 0x4e84..0x5077 but the list (0x4e84),
// 0x4e87 and 0x5036 is disabled. Otherwise all are enabled and the rule's
// settings shown: frequency radio 0x4e98 + (+0x7ef), the flag +0x7f5 (0x4ead),
// the value +0x7f0 (0x4eae, kept in +0x88), the two scope radios (+0x7f9:
// 0x4ec1 / 0x4ec0 / 0x4ec3 / 0x4ec4, +0x7fa: 0x4eb6..), the join radio
// 0x4eca + (+0x7f4), its triggers and conditions; delete / move (0x4e85 /
// 0x4e86) only with more than one rule.
RT3_NOINLINE void __fastcall RulesDialog_Refresh(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) {
        for (uint32_t id = 0x4e84; id <= 0x5077; ++id)
            if (id != 0x4e84 && id != 0x4e87 && id != 0x5036) Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
        return;
    }
    for (uint32_t id = 0x4e84; id <= 0x5077; ++id) Window_SendEvent5(w, 0, 0x65, id, 3, 0, 0);
    RulesDialog_SwitchPage(w);
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
    RulesDialog_ShowMessage(w);
    uint8_t freq = r[0x7ef];
    for (uint32_t id : { 0x4e98u, 0x4e99u, 0x4e9au, 0x4e9bu, 0x4e9eu, 0x4e9fu, 0x4ea0u, 0x4ea1u, 0x4ea2u })
        Window_SendEvent5(w, 0, 0x74, id, 0, 0, 0);
    if (freq <= 10) Window_SendEvent5(w, 0, 0x74, 0x4e98 + freq, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4ead, F<uint32_t>(r, 0x7f5), 0, 0);
    F<uint32_t>(w, 0x88) = F<uint32_t>(r, 0x7f0);
    Window_SendEvent5(w, 0, 0x68, 0x4eae, F<uint32_t>(r, 0x7f0), 0, 0);
    static const uint32_t scope[4] = { 0x4ec1, 0x4ec0, 0x4ec3, 0x4ec4 };
    if (r[0x7f9] <= 3) Window_SendEvent5(w, 0, 0x74, scope[r[0x7f9]], 1, 0, 0);
    if (r[0x7fa] <= 3) Window_SendEvent5(w, 0, 0x74, 0x4eb6 + r[0x7fa], 1, 0, 0);
    RulesDialog_UpdateScope(w);
    RulesDialog_ShowSlot(w);
    uint8_t join = r[0x7f4];
    Window_SendEvent5(w, 0, 0x74, 0x4eca, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4ecb, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x4ecc, 0, 0, 0);
    if (join <= 2) Window_SendEvent5(w, 0, 0x74, 0x4eca + join, 1, 0, 0);
    RulesDialog_FillTriggers(w);
    RulesDialog_FillConditions(w);
    uint32_t type = Collection_Count(hook::Global<Collection*>(0x62be18)) > 1 ? 0x65 : 0x87;
    Window_SendEvent5(w, 0, type, 0x4e85, 3, 0, 0);
    Window_SendEvent5(w, 0, type, 0x4e86, 3, 0, 0);
}
RT3_REPLACE(0x4db120, RulesDialog_Refresh);

RT3_NOINLINE void __fastcall RulesDialog_MoveRuleUp(uint8_t* w) { RulesDialog_MoveRule(w, -1); }
RT3_REPLACE(0x4db520, RulesDialog_MoveRuleUp);

RT3_NOINLINE void __fastcall RulesDialog_MoveRuleDown(uint8_t* w) { RulesDialog_MoveRule(w, 1); }
RT3_REPLACE(0x4db5e0, RulesDialog_MoveRuleDown);

// 0x4db6a0 (rules dialog callback). Selects rule v (EDX) after storing the
// current one (when one is shown); 0x622070 remembers it.
void __fastcall RulesDialog_SelectCb(void*, int32_t v) {
    uint8_t* dlg = hook::Global<uint8_t*>(0x6cff84);
    if (F<int32_t>(dlg, 0x84) == -1 || RulesDialog_StoreRule(dlg)) {
        F<int32_t>(hook::Global<uint8_t*>(0x6cff84), 0x84) = v;
        RulesDialog_Reset(hook::Global<uint8_t*>(0x6cff84));
        RulesDialog_Refresh(hook::Global<uint8_t*>(0x6cff84));
    }
    hook::Global<int32_t>(0x622070) = v;
}
RT3_REPLACE(0x4db6a0, RulesDialog_SelectCb);

// 0x4db6f0. Rules dialog setup: list draw callbacks (0x4d71b0) for the eight
// lists, the handlers of the rule list (0x4db6a0), value box (0x4d78d0), the
// lists 0x500b / 0x4eaf / 0x4ed5 and button 0x4eb4; ticks 0x4fed, clears +0x9c,
// fills territories and rules, resets and shows the first rule, and scrolls the
// rule list to the remembered row 0x622070.
RT3_NOINLINE void __fastcall RulesDialog_Init(uint8_t* w) {
    int32_t row = hook::Global<int32_t>(0x622070);
    for (uint32_t id : { 0x4eacu, 0x4ed9u, 0x4fdfu, 0x4fe0u, 0x4fe1u, 0x4fe2u, 0x4edbu, 0x4eddu }) Window_SendEvent5(w, 0, 0xb9, id, 0x4d71b0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x4e84, 0x4db6a0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x4eae, 0x4d78d0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x500b, 0x4d7950, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x4eaf, 0x4d7960, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x4ed5, 0x4d7970, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x4eb4, 0x4d7980, 0, 0);
    CityDialog_ShowMapParameters();
    F<int32_t>(w, 0x9c) = 0;
    Window_SendEvent5(w, 0, 0x74, 0x4fed, 1, 0, 0);
    RulesDialog_FillTerritories(w);
    RulesDialog_FillList(w);
    RulesDialog_Reset(w);
    RulesDialog_Refresh(w);
    if (row >= 0) Window_SendEvent5(w, 0, 0x69, 0x4e84, (uint32_t)row, 0, 0);
}
RT3_REPLACE(0x4db6f0, RulesDialog_Init);

// 0x4db8b0. Event editor w: asks for the name of a new event (default string
// 0x47a with the next number) or of a copy of the selected one (0x47b), adds
// it (0x62be18, key +0x90, copying the original's rules when duplicating),
// lists and selects it, and advances +0x90.
RT3_NOINLINE void __fastcall EventEditor_NewEvent(uint8_t* w, int, int32_t duplicate) {
    uint32_t source = 0;
    char text[0x194], num[0x108];
    if (!duplicate) {
        int32_t n = 1;
        if (Collection_Count(hook::Global<Collection*>(0x62be18)))
            n = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x62be18), 0, Collection_Count(hook::Global<Collection*>(0x62be18)) - 1, 0) + 1;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(num, (const char*)0x5c9aec, n);
        FormatText(text, GetString(0x47a), num, 0, 0, 0, 0, 0, 0, 0);
    } else {
        if (F<int32_t>(w, 0x84) == -1) return;
        uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
        FormatText(text, GetString(0x47b), (const char*)e + 0x18, 0, 0, 0, 0, 0, 0, 0);
        source = F<uint32_t>(w, 0x84);
    }
    Dialog_RunMessage(GetString(0x47c), 0xd, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)text, 0xaa, 0, nullptr, nullptr, 0, 0, nullptr,
                      0);
    if (strlen(text)) {
        F<uint32_t>(w, 0x84) = Table_AddNamedRecord(hook::Global<Collection*>(0x62be18), 0, text, F<uint32_t>(w, 0x90));
        if (duplicate) {
            uint8_t* src = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, source);
            uint8_t* dst = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84));
            Record_CopyFrom(dst, 0, src);
        }
    }
    Window_SendEvent5(w, 0, 0x88, 0x4e84, (uint32_t)(uintptr_t)text, F<uint32_t>(w, 0x84), (uint32_t)(uintptr_t)text);
    Window_SendEvent5(w, 0, 0x66, 0x4e84, F<uint32_t>(w, 0x90), 0, 0);
    ++F<int32_t>(w, 0x90);
    if (!duplicate) RulesDialog_Reset(w);
}
RT3_REPLACE(0x4db8b0, EventEditor_NewEvent);

// 0x4dba90. Event editor w: renames the selected event (+0x18, up to 199
// characters) through text box 9 (string 0x47d), then refills the list and
// reselects it (event 0x66 / 0x4e84 with its position).
RT3_NOINLINE void __fastcall EventEditor_Rename(uint8_t* w) {
    if (F<int32_t>(w, 0x84) == -1) return;
    char* name = (char*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84)) + 0x18;
    char buf[0x12c];
    strcpy(buf, name);
    Dialog_RunMessage(GetString(0x47d), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)buf, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    if (!strlen(buf)) return;
    StrCopyN(name, buf, 0xc7);
    int32_t id = F<int32_t>(w, 0x84);
    RulesDialog_FillList(w);
    F<int32_t>(w, 0x84) = id;
    RulesDialog_Refresh(w);
    uint32_t pos = Collection_PositionOf(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84), 2);
    Window_SendEvent5(w, 0, 0x66, 0x4e84, pos, 0, 0);
}
RT3_REPLACE(0x4dba90, EventEditor_Rename);

// 0x4dbb80. Event editor dialog w (event +0x84 of 0x62be18): event handler.
// Clicks (0xcb): move the event up / down (0x4e85 / 0x4e86), new (0x4e87,
// after storing the current one), duplicate (0x4e88; quirk: stores on the
// dialog 0x6cff84), rename (0x4e89), delete (0x4e8a), add / edit / delete a
// rule (0x4eb0 / 0x4eb3 / 0x4eb2), add / edit / delete a condition (0x4ed4 /
// 0x5024 / 0x4fd5). Double clicks (0xcf) edit the rule (0x4eaf) or condition
// (0x4ed5). Changes (0xca): rename on a double click of the list (0x4e84),
// message kind (0x4e8e..0x4e91), the message text 0x4eac (+0xe0, kind 4 for
// the custom choice 0x4ea1, back to 0 for the others), the shared text boxes
// 0x4eac / 0x4ed9 / 0x4edb (unless 0x6d07d8), refused choices 0x4ecb / 0x4ecc
// while 0x4e8f is on (0xf2f), the trigger slot (0x4fed..0x4ff0, +0x9c), the
// page (0x5001 / 0x5002, 0x622074) and the buttons 0x5014..0x501c.
// Returns 1 for unhandled events.
RT3_NOINLINE uint32_t __fastcall EventEditor_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint32_t id = ev[1];
    if (ev[0] == 0xcf) {
        if (id == 0x4eaf) EventEditor_EditRule(w);
        else if (id == 0x4ed5) EventEditor_EditCondition(w);
        else return 1;
        return 0;
    }
    if (ev[0] == 0xcb) {
        switch (id) {
        case 0x4e85: RulesDialog_MoveRuleUp(w); break;
        case 0x4e86: RulesDialog_MoveRuleDown(w); break;
        case 0x4e87:
            if (RulesDialog_StoreRule(w)) EventEditor_NewEvent(w, 0, 0);
            break;
        case 0x4e88:
            if (F<int32_t>(w, 0x84) != -1) RulesDialog_StoreRule(hook::Global<uint8_t*>(0x6cff84));
            EventEditor_NewEvent(w, 0, 1);
            break;
        case 0x4e89: EventEditor_Rename(w); break;
        case 0x4e8a: RulesDialog_DeleteRule(w); break;
        case 0x4eb0: EventEditor_AddRule(w); break;
        case 0x4eb2: RulesDialog_DeleteTrigger(w); break;
        case 0x4eb3: EventEditor_EditRule(w); break;
        case 0x4ed4:
            EventEditor_AddCondition(w);
            RulesDialog_UpdateSlot(w);
            break;
        case 0x4fd5:
            RulesDialog_DeleteCondition(w);
            RulesDialog_UpdateSlot(w);
            break;
        case 0x5024:
            EventEditor_EditCondition(w);
            RulesDialog_UpdateSlot(w);
            break;
        default:
            return 1;
        }
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    switch (id) {
    case 0x4e84:
        if (((uint8_t*)ev)[0x20] & 2) EventEditor_Rename(w);
        return 0;
    case 0x4e8f:
        Window_SendEvent5(w, 0, 0x74, 0x4eca, 1, 0, 0);
        // fall through
    case 0x4e8e:
    case 0x4e90:
    case 0x4e91:
        RulesDialog_ShowMessageKind(w);
        return 0;
    case 0x4ea1:
        if (uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84))) {
            Obj_SetTextE0(e, 0, F<const char*>(Widget_FindChild(w, 0, 0x4eac), 0xba));
            e[0x7ee] = 4;
            RulesDialog_ShowMessage(w);
        }
        return 0;
    case 0x4e98:
    case 0x4e99:
    case 0x4e9a:
    case 0x4e9b:
    case 0x4e9e:
    case 0x4e9f:
    case 0x4ea0:
    case 0x4ea2:
        if (uint8_t* e = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0, F<uint32_t>(w, 0x84))) {
            if (e[0x7ee] == 4) {
                Obj_SetTextE0(e, 0, F<const char*>(Widget_FindChild(w, 0, 0x4eac), 0xba));
                e[0x7ee] = 0;
                RulesDialog_ShowMessage(w);
            }
        }
        return 0;
    case 0x4eac:
    case 0x4ed9:
    case 0x4edb:
        if (!hook::Global<int32_t>(0x6d07d8)) {
            char text[0x7d0];
            StrCopyN(text, F<const char*>((uint8_t*)(uintptr_t)ev[11], 0xba), 0x7d0);
            Window_SendEvent5(w, 0, 0x6f, 0x4eac, (uint32_t)(uintptr_t)text, 0, 0);
            Window_SendEvent5(w, 0, 0x6f, 0x4ed9, (uint32_t)(uintptr_t)text, 0, 0);
            Window_SendEvent5(w, 0, 0x6f, 0x4edb, (uint32_t)(uintptr_t)text, 0, 0);
            hook::Global<int32_t>(0x6d07d8) = 0;
        }
        return 0;
    case 0x4ecb:
    case 0x4ecc:
        if (F<int32_t>(Widget_FindChild(w, 0, 0x4e8f), 0xcc)) {
            Window_SendEvent5(w, 0, 0x74, 0x4eca, 1, 0, 0);
            Message_Show(GetString(0xf2f), 1, 0);
        }
        return 0;
    case 0x4fed:
    case 0x4fee:
    case 0x4fef:
    case 0x4ff0:
        RulesDialog_Store(w);
        F<int32_t>(w, 0x9c) = (int32_t)(ev[1] - 0x4fed);
        RulesDialog_FillConditions(w);
        RulesDialog_UpdateSlot(w);
        RulesDialog_ShowSlot(w);
        return 0;
    case 0x5001:
    case 0x5002:
        RulesDialog_StoreRule(w);
        RulesDialog_SwitchPage(w);
        hook::Global<uint32_t>(0x622074) = ev[1];
        return 0;
    }
    if (id >= 0x5014 && id <= 0x501c) {
        RulesDialog_UpdateButton(w);
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4dbb80, EventEditor_HandleEvent);

}  // namespace rt3
