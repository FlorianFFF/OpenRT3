// TrainWindow.cpp - RT3.exe 0x510000-0x516000.
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
#include "core/ptrlist.h"
#include "hook/keepregs.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
double __fastcall Train_Float109(uint8_t* t, int, int32_t i);
double __fastcall Train_Float131(uint8_t* t, int, int32_t i);
int32_t __fastcall Obj_Get21(void* o);
void __fastcall Widget_SetVtbl(uint8_t* w);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
int32_t __fastcall Train_Rank(void* self);
void __fastcall ListView_RefreshVisible(uint8_t* tv);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Array33_At(void* a, int, int32_t i);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
int32_t Global6d40e4();
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
double One();
void* __fastcall Obj_City352(void* o);
void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
double __fastcall Train_Float159(uint8_t* t, int, int32_t i);
double __fastcall Obj_Sum3(void* o, int, int32_t i);
double __fastcall Train_Total109(uint8_t* t);
double __fastcall Train_Total131(uint8_t* t);
double __fastcall Train_Total159(uint8_t* t);
double __fastcall Train_LifetimeTotal(uint8_t* t);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void __fastcall Window_UpdateListButtons(uint8_t* w);
void __fastcall Train_SetBuffer1c6(uint8_t* t, int, const uint8_t* src);
void __fastcall Window_SetLargeLayout(uint8_t* w, int, int32_t large);
void __fastcall TrainList_Refill(uint8_t* w);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
uint32_t __fastcall EngineTypes_PickForYear(Collection* c);
void UpgradeDialog_UpdateCostText();
void __fastcall TrainList_UpdateButtons(uint8_t* w);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Widget_IsVisible(uint8_t* w);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t ClockMs();
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall TrainPicker_Fill(uint8_t* w, int, uint32_t exclude);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
void* __fastcall Train_Company(void* t);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void* __fastcall Obj_CityRef(void* o);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
void* SelectedTrain();
void __fastcall TrainWindow_Refresh(uint8_t* win);
int32_t __fastcall Obj_CityKindIs2(void* o);
void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
void* __fastcall Identity(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
int32_t SelectedTrainId();
uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
int32_t __fastcall Train_StateSpecial(uint8_t* t);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __fastcall OrderList_Init(uint8_t* o);
void __fastcall TrainBuy_Init(uint8_t* w);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall Panel_ReleaseImages(uint8_t* w);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void* __fastcall Cursor33_Init(uint8_t* c);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
int32_t __fastcall OrderList_CountPlain(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Widget_DeleteGeneratedRows(uint8_t* w, int, int32_t);
RT3_NOINLINE void __fastcall TrainList_UpdateButtons(uint8_t* w);
void __fastcall BuildingRow_AddCargoButtons(uint8_t* w, uint8_t* list, int32_t type, float x, float y, int32_t, int32_t);
RT3_NOINLINE uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
void __fastcall StopList_DrawLabel(void* stops, int32_t idx, float x, float y, int32_t, float h);
void __fastcall Map_ShowPickTooltip(uint8_t* o);
void __fastcall NetMsg_ReplaceEngines(uint8_t* m);
void __fastcall NetMsg_SellTrain(uint8_t* m);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall TrainList_DrawEntry(uint8_t* t, uint8_t highlight, float x, float y, float w, float h);
void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
uint32_t __fastcall Train_PromptName(uint8_t* t);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
void Vehicles_SelectNextFwd();
void Vehicles_SelectNextBack();
void __fastcall ListView_Refresh(uint8_t* l, int, int32_t toEnd);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
float __fastcall Train_RunningCost(uint8_t* t);
void __fastcall Widget_SetClip(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
int32_t __fastcall Train_ReportCb(void*, int, int32_t cmd, uint32_t id);
int32_t __fastcall TrainRunMode_StepCb(uint8_t* w, int32_t button, int32_t ev, uint32_t id);
void TrainPanel_RefreshTooltip();
int32_t HasSelectedTrain();
int32_t SelectedTrainIsOwn();
void TrainWindow_ShowOwnerMessage();
void __fastcall Vehicles_SelectNext(int32_t dir);
int32_t Obj_KindE8Special();
void __fastcall Train_OpenConsistEditor(void* self, int, uint32_t trainId, const char* message);
int32_t __fastcall StationPick_Run(uint8_t lower);
int32_t __fastcall Train_CanRun(uint8_t* t);
int32_t __fastcall Train_CanRunNonElectric(uint8_t* t);
void __fastcall TrainOrders_Copy(uint8_t* src, uint8_t* dst);
void __fastcall IndustryEditor_Run(uint32_t a, uint8_t* rec, uint32_t x);
void __fastcall TrainDetail_TooltipCb(int32_t id, int, int32_t, int32_t);
void* __fastcall Obj_Cargo72(uint8_t* o);
void __fastcall Editor_SelectionCost(int32_t* total, int32_t* count);
uint32_t TrainList_Selected();
void __fastcall Train_ReplaceEngine(uint8_t* train, int, uint32_t typeId, int32_t freeUpgrade);
void __fastcall Train_ApplyLivery(uint8_t* t);
void __fastcall UpgradeDialog_SendTrains(uint8_t* d);
uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
void __fastcall Collection_SetKey1(Collection* c, int, uint32_t id, uint32_t key);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void* __fastcall Obj_Cargo72(uint8_t* o);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall Train_OpenConsistEditor(void* self, int, uint32_t trainId, const char* message);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void ModelPreview_Close();
RT3_NOINLINE int32_t __fastcall Train_CanRun(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_CanRunNonElectric(uint8_t* t);
RT3_NOINLINE void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
RT3_NOINLINE void __fastcall Train_ApplyLivery(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
RT3_NOINLINE uint32_t __fastcall Train_PromptName(uint8_t* t);
RT3_NOINLINE uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE void __fastcall IndustryEditor_Run(uint32_t a, uint8_t* rec, uint32_t x);
RT3_NOINLINE void __fastcall TrainList_DrawEntry(uint8_t* t, uint8_t highlight, float x, float y, float w, float h);
RT3_NOINLINE void TrainPanel_RefreshTooltip();
RT3_NOINLINE void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
RT3_NOINLINE int32_t __fastcall Train_ReportCb(void*, int, int32_t cmd, uint32_t id);
RT3_NOINLINE int32_t __fastcall TrainRunMode_StepCb(uint8_t* w, int32_t button, int32_t ev, uint32_t id);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE int32_t __fastcall StationPick_Run(uint8_t lower);
RT3_NOINLINE uint32_t TrainList_Selected();
RT3_NOINLINE void UpgradeDialog_UpdateCostText();
void __fastcall TrainList_FilterCb(int32_t v, int32_t x);
void __fastcall TrainList_AgeFilterCb(int32_t on, int32_t years);
void Thunk510e20();
RT3_NOINLINE void __fastcall TrainList_DrawRow(int32_t, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall Dialog_AddFilterToggle(uint8_t* d, int, int32_t idx, float x, float y, int32_t, float h);
RT3_NOINLINE void __fastcall TrainList_UpdateButtons(uint8_t* w);
RT3_NOINLINE void __fastcall UpgradeDialog_SendTrains(uint8_t* d);
RT3_NOINLINE void __fastcall NetMsg_ReplaceEngines(uint8_t* m);
void TrainList_UpdateOpenButtons();
RT3_NOINLINE void __fastcall TrainBuy_Init(uint8_t* w);
RT3_NOINLINE void __fastcall TrainList_Refill(uint8_t* w);
int32_t __fastcall ConsistDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
RT3_NOINLINE void __fastcall TrainOrders_Copy(uint8_t* src, uint8_t* dst);
void __fastcall TrainDetail_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Window_SetLargeLayout(uint8_t* w, int, int32_t large);
RT3_NOINLINE void __fastcall NetMsg_SellTrain(uint8_t* m);
double __fastcall Train_Total109(uint8_t* t);
double __fastcall Train_Total131(uint8_t* t);
RT3_NOINLINE void* SelectedTrain();
RT3_NOINLINE int32_t SelectedTrainId();
RT3_NOINLINE int32_t HasSelectedTrain();
RT3_NOINLINE int32_t SelectedTrainIsOwn();
RT3_NOINLINE void TrainWindow_ShowOwnerMessage();
void __fastcall TrainView_DrawRowCb(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall TrainView_DrawStopCb(void*, int32_t idx, float x, float y, int32_t a, float h, int32_t);
void __fastcall TrainView_DeleteRowsCb(uint8_t* w, int32_t e, int32_t a);
void __fastcall TrainView_AddCargoButtonsCb(uint8_t* w, int32_t type, int32_t, float x, float y, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall TrainDetail_TooltipCb(int32_t id, int, int32_t, int32_t);
RT3_NOINLINE void __fastcall Window_UpdateListButtons(uint8_t* w);
void __fastcall TrainView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t);
void __fastcall TrainGauges_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall Train_PickTooltipCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall TrainWindow_Refresh(uint8_t* win);
RT3_NOINLINE void TrainWindow_RefreshOpen();
RT3_NOINLINE uint8_t* __fastcall TrainWindow_Construct(uint8_t* w);
RT3_NOINLINE int32_t __fastcall TrainCmd_Apply(uint32_t* cmd);
RT3_NOINLINE void __fastcall Vehicles_SelectNext(int32_t dir);
RT3_NOINLINE void Vehicles_SelectNextFwd();
RT3_NOINLINE void Vehicles_SelectNextBack();
int32_t __fastcall TrainWindow_HandleEvent(uint8_t* w, int, uint32_t* msg);
void __fastcall Page_ClearChildrenCb(uint8_t* w, int, int32_t);
void __fastcall Widget5157c0_Destruct(uint8_t* w);
RT3_NOINLINE int32_t Global6d3b30();
void __fastcall TrainList_DrawRowCb(void*, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall TrainList_SelectCb(void*, uint32_t id);
void __fastcall Page_BuildTrainRowCb(uint8_t* w, int32_t d, uint32_t train, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall TrainPicker_Fill(uint8_t* w, int, uint32_t exclude);
int32_t __fastcall TrainList_HandleEvent(uint8_t* w, int, uint32_t* msg);
uint8_t* __fastcall TrainListDialog_Construct(uint8_t* w);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

// 0x5130f0..0x5131a0. The train selected in the current screen (0x6d0818 +0x90).
static bool SelectedTrainValid() {
    uint8_t* s = g_gameWindow;
    return s && F<int32_t>(s, 0x90) > 0 && Collection_IsValid(g_trains, 0, F<uint32_t>(s, 0x90));
}

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

// Two-list picker dialogs (left / right lists share the selection +0x78; the
// swap buttons are enabled by the side flag).
static void PickerDialog_Setup(uint8_t* w, uint32_t listA, uint32_t listB, uint32_t btnA, uint32_t btnB, int32_t right) {
    Window_SendEvent5(w, 0, 0xaa, listA, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, 0xaa, listB, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, right ? 0x86 : 0x6d, btnA, 3, 0, 0);
    Window_SendEvent5(w, 0, right ? 0x6d : 0x86, btnB, 3, 0, 0);
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

namespace {
uint8_t* ShownChild466(int32_t id) {
    uint8_t* g = Widget_FindChild(g_trainWindow, 0, id);
    if (!Widget_IsVisible(g)) Widget_SetFlags(g, 0, 1, 0, 0);
    return g;
}
}

namespace {
uint32_t Message496(uint32_t text, int32_t type, uint32_t px = 0xbf800000) {
    return Dialog_RunMessage(GetString(text), type, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
uint8_t* Train496() { return (uint8_t*)SelectedTrain(); }
int32_t Special496(void* t) { return ((int32_t(__fastcall*)(void*))(void*)&Obj_KindE8Special)(t); }
// The shown stop list (0x65a3 / 0x65a4 by 0x6d3b24).
uint8_t* StopList496(uint8_t* w) { return Widget_FindChild(w, 0, g_6d3b24 ? 0x65a3 : 0x65a4); }
void SendHost496(uint32_t type, uint32_t a) {
    uint32_t req[3];
    req[0] = (uint32_t)Global6d40e4();
    req[1] = (uint32_t)SelectedTrainId();
    req[2] = a;
    Net_SendToHost(g_session, 0, type, 0xc, (const uint8_t*)req);
}
}

namespace {
uint32_t Message497(const char* text, uint32_t px = 0xbf800000) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
uint8_t* const kOrders497 = (uint8_t*)0x6d3ae8;
int32_t StopRow497(uint8_t* d) { return F<int32_t>(Widget_FindChild(d, 0, 0x2aff), 0x1d6); }
bool OwnTrain497(uint8_t* t) { return F<int32_t>(t, 0x51) == Obj_Get21(g_world); }
}

// 0x510b60 (filter callback). Marks which trains (0x6d1aa8 by +0x25) the
// train list shows: the player's own whose cargo kind (+0x126 of the city's
// 0x388 entry) is x (EDX, 0x3e8 any) and, with the age filter 0x275a, old
// enough (world year - +0xd4). Then refreshes the list and the cost text.
void __fastcall TrainList_FilterCb(int32_t v, int32_t x) {
    if (v == -1 || g_6d3ae0) return;
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* c = g_trains;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 1));
        const uint32_t slot = F<uint32_t>(t, 0x25);
        if (slot >= 0x2000) continue;
        uint8_t* mark = (uint8_t*)(0x6d1aa8 + slot);
        if (F<int32_t>(t, 0x51) != Obj_Get21(g_world)) {
            *mark = 0;
            continue;
        }
        uint8_t* e = (uint8_t*)Obj_Ref388(Obj_City41(t));
        if (F<int32_t>(e, 0x126) != x && x != 0x3e8) {
            *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = 0;
            continue;
        }
        if (F<int32_t>(Widget_FindChild(g_6d3ac8, 0, 0x275a), 0x1d6) <= 0) {
            *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = 1;
            continue;
        }
        const int32_t age = (int32_t)TreeView_SelectedData(Widget_FindChild(g_6d3ac8, 0, 0x275a));
        const int32_t years = F<uint16_t>(g_world, 0xd) - F<int32_t>(t, 0xd4);
        *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = years >= age ? 1 : 0;
    }
    ListView_Refresh(Widget_FindChild(g_6d3ac8, 0, 0x275d), 0, 0);
    UpgradeDialog_UpdateCostText();
}
RT3_REPLACE(0x510b60, TrainList_FilterCb);

// 0x510cc0 (filter callback). Train upgrade list filter: marks (0x6d1aa8 by
// +0x25) the player's trains, with the age filter on (on != 0) only those at
// least `years` old (world year - +0xd4), and with a cargo kind chosen in
// list 0x2759 (0x3e8 any) only those whose city's 0x388 entry (+0x126) has
// it. Then refreshes the list 0x275d and the cost text.
void __fastcall TrainList_AgeFilterCb(int32_t on, int32_t years) {
    if (on == -1 || g_6d3ae0) return;
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* c = g_trains;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 1));
        if (F<uint32_t>(t, 0x25) >= 0x2000) continue;
        if (F<int32_t>(t, 0x51) != Obj_Get21(g_world)) {
            *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = 0;
            continue;
        }
        if (on && F<uint16_t>(g_world, 0xd) - F<int32_t>(t, 0xd4) < years) {
            *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = 0;
            continue;
        }
        if (F<int32_t>(Widget_FindChild(g_6d3ac8, 0, 0x2759), 0x1d6) <= 0) {
            *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = 1;
            continue;
        }
        const int32_t kind = (int32_t)TreeView_SelectedData(Widget_FindChild(g_6d3ac8, 0, 0x2759));
        uint8_t* e = (uint8_t*)Obj_Ref388(Obj_City41(t));
        *(uint8_t*)(0x6d1aa8 + F<uint32_t>(t, 0x25)) = F<int32_t>(e, 0x126) == kind || kind == 0x3e8 ? 1 : 0;
    }
    ListView_Refresh(Widget_FindChild(g_6d3ac8, 0, 0x275d), 0, 0);
    UpgradeDialog_UpdateCostText();
}
RT3_REPLACE(0x510cc0, TrainList_AgeFilterCb);

// 0x510e20 (vtable stub).
// Forwards to UpgradeDialog_UpdateCostText (0x50f890).
void Thunk510e20() { UpgradeDialog_UpdateCostText(); }
RT3_REPLACE(0x510e20, Thunk510e20);

// 0x510e30. Train list row (draw callback for train `id` of 0x6cfcbc): the
// consist's car pictures left to right from x + 0.002 along the row bottom
// (y + h - 0.004), skipping cars of kind 3, until the next one would pass
// x + 0x5d1970; then "<rank>) <destination>" (format 0x5d06e8) in the world's
// label font.
RT3_NOINLINE void __fastcall TrainList_DrawRow(int32_t, uint32_t id, float x, float y, float w, float h, int32_t) {
    uint8_t* train = (uint8_t*)Collection_Get(g_trains, 0, id);
    if (!train) return;
    float cx = (float)((double)x + D(0x5c9ba0));
    for (uint8_t* car = (uint8_t*)Obj_City41(train); car; car = (uint8_t*)Obj_City352(car)) {
        uint8_t* type = (uint8_t*)Obj_Ref388(car);
        if (F<int32_t>(type, 0x41) == 3) continue;
        uint8_t* g = (uint8_t*)(uintptr_t)Car_SkinTexture(car);
        float s = (float)One();
        float right = (float)(Slider_StepA(g) * s + cx);
        float limit = (float)((double)x + D(0x5d1970));
        if (right > limit) break;
        double bottom = (double)y + h - D(0x5d0250);
        float gy = (float)(bottom - Slider_StepB(g) * s);
        Glyph_Queue(g, 0, cx, gy, 0x3f800000, s, s, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        cx = (float)(Slider_StepA(g) * s + cx);
    }
    char dest[0x12c], num[0x40], text[0x1ec];
    Train_DestinationText(train, 0, dest, 0x12b);
    const char* n = Num_Format(num, 1, (double)(Train_Rank(train) + 1), 10, 1, 0, 0);
    FormatText(text, (const char*)0x5d06e8, n, dest, 0, 0, 0, 0, 0, 0);
    float lx = (float)((double)x + D(0x5d1968));
    float lh = (float)(h * D(0x5c8628));
    float lw = (float)((double)w - D(0x5d1960));
    float ly = (float)((double)y - D(0x5d0250));
    Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, text, lx, ly, lw, lh, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x510e30, TrainList_DrawRow);

// 0x511050. Adds toggle button 0x2774 + `idx` (style 0x2b3f, 0.047 wide, `h`
// high) at (x + 0.005, y + 0.003) to the dialog's window, with the click handler
// 0x510e30, ticked by byte 0x6d1aa8 + idx.
void __fastcall Dialog_AddFilterToggle(uint8_t* d, int, int32_t idx, float x, float y, int32_t, float h) {
    uint8_t def[0xa1];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + D(0x5d0700));
    F<float>(def, 0xb) = (float)((double)y + D(0x5d01d8));
    F<uint16_t>(def, 0) = (uint16_t)(idx + 0x2774);
    F<uint32_t>(def, 0xf) = 0x3d400000;
    F<float>(def, 0x13) = h;
    F<uint16_t>(def, 5) = 0x2b3f;
    F<uint8_t*>(def, 0x47) = d;
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0ae0);
    F<int32_t>(def, 0x6b) = 5;
    F<int32_t>(def, 0x67) = 6;
    uint8_t* b = (uint8_t*)ExeNew(0xe8);
    if (b) b = Button_Construct(b, 0, def);
    F<int32_t>(b, 0x82) = idx;
    Window_AddChild(F<uint8_t*>(d, 0x86), 0, b, 0);
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x7a, F<uint16_t>(def, 0), 0x510e30, 0, 0);
    Free(F<void*>(def, 0x43));
    Window_SendEvent5(F<uint8_t*>(d, 0x86), 0, 0x74, idx + 0x2774, ((const uint8_t*)0x6d1aa8)[idx] ? 1 : 0, 0, 0);
}
RT3_REPLACE(0x511050, Dialog_AddFilterToggle);

// 0x511170. The train list's station buttons for the selected train (tree 0x2aff
// into the train array 0x6d3ae8; nothing while 0x6d3ae1 is set): 0x2b03 "go to
// <station>" (0x54d; 0x54c when the train has no station, +0x28 < 0; 0x54e and
// disabled without a selection) and 0x2b04 for an active station (0x54f, else 0x550
// and disabled); label 0x2b05 shows string 0x551 when the array is empty.
RT3_NOINLINE void __fastcall TrainList_UpdateButtons(uint8_t* w) {
    bool none = F<int32_t>(Widget_FindChild(w, 0, 0x2aff), 0x1d6) < 0;
    if (g_6d3ae1) return;
    uint8_t* r = nullptr;
    char text[0x1f8];
    if (!none) {
        r = (uint8_t*)Array33_At((void*)0x6d3ae8, 0, (int32_t)TreeView_SelectedData(Widget_FindChild(w, 0, 0x2aff)));
        if ((int8_t)r[0x28] < 0) FormatText(text, GetString(0x54c), 0, 0, 0, 0, 0, 0, 0, 0);
        else {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(r, 0x29));
            FormatText(text, GetString(0x54d), (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    Window_SendEvent5(w, 0, none ? 0x87 : 0x65, 0x2b03, 2, 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0x2b03, (uint32_t)(uintptr_t)(none ? GetString(0x54e) : text), 0, 0);
    bool none2 = none;
    if (!none) {
        if ((int8_t)r[0x28] < 0 ||
            !Obj_BuildingStateActive(Collection_Get(g_stations, 0, F<uint16_t>(r, 0x29))))
            none2 = true;
        else {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(r, 0x29));
            FormatText(text, GetString(0x54f), (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    Window_SendEvent5(w, 0, none2 ? 0x87 : 0x65, 0x2b04, 2, 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0x2b04, (uint32_t)(uintptr_t)(none2 ? GetString(0x550) : text), 0, 0);
    char empty[0x130];
    if (g_6d3aec > 0) empty[0] = 0;
    else strcpy(empty, GetString(0x551));
    Window_SendEvent5(w, 0, 0x6f, 0x2b05, (uint32_t)(uintptr_t)empty, 0, 0);
}
RT3_REPLACE(0x511170, TrainList_UpdateButtons);

 // namespace

// 0x511360. Upgrade dialog (0x6d3ac8) "upgrade" button: sets d+0x78, the
// normal cursor, then sends message 0x16 holding the trains queued in the
// array 0x6d3ae8 (0x33-byte records) after a 0x14-byte header {count, chosen
// engine of list 0x2afe (0 without a selection), 0x6d3aa8, 0x6d40e4, the
// player} and empties the array.
RT3_NOINLINE void __fastcall UpgradeDialog_SendTrains(uint8_t* d) {
    F<int32_t>(d, 0x78) = 1;
    uint8_t* cursor = g_cursorManager;
    g_6d1aa4 = 0;
    g_6d3ab8 = F<uint32_t>(cursor, 4);
    Cursor_Set(cursor, 0, 6, 0, 0);
    const int32_t count = g_6d3aec;
    const int32_t size = count * 0x33 + 0x14;
    uint32_t* buf = (uint32_t*)Malloc((size_t)size);
    buf[0] = (uint32_t)count;
    uint32_t engine = 0;
    if (g_6d3ac8) {
        uint8_t* list = Widget_FindChild(g_6d3ac8, 0, 0x2afe);
        if (F<int32_t>(list, 0x1d6) >= 0) engine = TreeView_SelectedData(list);
    }
    buf[1] = engine;
    buf[2] = g_6d3aa8;
    buf[4] = (uint32_t)Obj_Get21(g_world);
    buf[3] = (uint32_t)Global6d40e4();
    uint8_t* out = (uint8_t*)buf + 0x14;
    for (int32_t i = 0; i < count; ++i, out += 0x33) memcpy(out, Array33_At((void*)0x6d3ae8, 0, i), 0x33);
    for (int32_t i = count; i > 0; --i) TrainOrders_Remove((uint8_t*)0x6d3ae8, 0, 0);
    uint8_t* gs = g_session;
    Net_Send(gs, 0, (const char*)gs + 0x8f48, 0x16, size, (const uint8_t*)buf, 5, 0);
    Free(buf);
}
RT3_REPLACE(0x511360, UpgradeDialog_SendTrains);

// 0x511490. Network message m (+0 player, +4 engine type, +8 count, +0xa
// train ids): every listed train still there gets the engine (0x4aefb0);
// the sender's cursor resets.
RT3_NOINLINE void __fastcall NetMsg_ReplaceEngines(uint8_t* m) {
    for (int32_t i = 0; i < F<uint16_t>(m, 8); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint16_t>(m, 0xa + i * 2));
        if (t) ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x4aefb0)(t, 0, F<uint32_t>(m, 4), 0);
    }
    if (Obj_Get25(hook::As<void*>(g_world)) == F<int32_t>(m, 0)) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
}
RT3_REPLACE(0x511490, NetMsg_ReplaceEngines);

// 0x5114f0 (callback). Updates the buttons of the train list 0x6d3ac8.
void TrainList_UpdateOpenButtons() { TrainList_UpdateButtons(g_6d3ac8); }
RT3_REPLACE(0x5114f0, TrainList_UpdateOpenButtons);

// 0x511500. Opens the train buying window (it becomes 0x6d3ac8, with its three
// fonts): the engine list 0x2afe holds the available engines from the best
// (+0x105, then by name), preselecting 0x6d1aa0 (or the year's default when
// none is available). Upgrading a train (0x6d3acc) also fills the upgrade
// controls: the cost of the first engine, the 10..70 % rebuild choices
// (0x275a), the current and the other engine types in use (0x2759) and the
// company's trains (0x275d, marked in 0x6d1aa8); otherwise the consist list
// 0x2aff. Finally the era's speed steps (0x6d3aac, mph / km/h / ...) go to
// the unit buttons' tooltips.
RT3_NOINLINE void __fastcall TrainBuy_Init(uint8_t* w) {
    Window_Load(w, 0, g_6d3b4c ? BitsF(0x3e875c29) : BitsF(0x3dfd70a4), BitsF(0x3d64b17e), (const char*)0x5d1978);
    int32_t lang = g_language;
    if (lang == 4 || lang == 1) {
        Window_SendEvent5(w, 0, 0x78, 0x2afa, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x2afb, 0x12, 0, 0);
    }
    g_6d3ac8 = w;
    g_6d3abc = Pack_AcquireFont(g_pack, 0, 2);
    g_6d3ac0 = Pack_AcquireFont(g_pack, 0, 1);
    g_6d3ac4 = Pack_AcquireFont(g_pack, 0, 0x14);
    uint8_t used[0x5dc];
    memset(used, 0, sizeof used);
    int32_t shown = 0, preselect = 0;
    auto engines = [] { return g_6ada84; };
    auto types = [] { return g_carTypes; };
    auto who = [](uint32_t& company, uint32_t& player) {
        player = (uint32_t)Obj_Get25(g_world);
        company = (uint32_t)Obj_Get21(g_world);
    };
    for (;;) {
        int32_t best = -1, bestVal = -1;
        uint8_t* bestType = nullptr;
        for (int32_t j = 0; j < Collection_Count(engines()); ++j) {
            if (used[j]) continue;
            uint8_t* rec = (uint8_t*)Collection_Get(engines(), 0, Collection_IdAt(engines(), 0, j, 0));
            uint8_t* type = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(types(), 0, F<uint32_t>(rec, 0x72)) : nullptr;
            uint32_t co, pl;
            who(co, pl);
            if (!CarType_Available(type, 0, co, pl)) continue;
            bool take = F<int32_t>(type, 0x105) > bestVal;
            if (!take && F<int32_t>(type, 0x105) == bestVal) {
                const char* a = LocalizeName((const char*)type + 0x45);
                const char* b = LocalizeName((const char*)bestType + 0x45);
                take = Stricmp(a, b) < 0;
            }
            if (take) {
                bestVal = F<int32_t>(type, 0x105);
                best = j;
                bestType = type;
            }
        }
        if (best < 0) break;
        Window_SendEvent5(w, 0, 0x88, 0x2afe, 0x5c87a8, Collection_IdAt(engines(), 0, best, 0), 0);
        used[best] = 1;
        if (g_6d1aa0 == Collection_IdAt(engines(), 0, best, 0)) preselect = shown;
        ++shown;
    }
    if (!shown) Window_SendEvent5(w, 0, 0x88, 0x2afe, 0x5c87a8, EngineTypes_PickForYear(engines()), 0);
    Window_SendEvent5(w, 0, 0x7b, 0x2afc, 0x50f610, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x2afe, 0x50fc00, 0, 0);
    Window_SendEvent5(w, 0, 0x7e, 0x2afe, 0x50fa00, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x2afe, 0x50f690, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x2afd, 0x50fff0, 0, 0);
    if (uint8_t* lv = Widget_FindChild(w, 0, 0x2afe)) ListView_RefreshVisible(lv);
    Window_SendEvent5(w, 0, 0xba, 0x2afc, 0, 0, 0);
    if (Collection_Count(engines())) Window_SendEvent5(w, 0, 0x66, 0x2afe, preselect, 0, 0);
    char num[0x40], text[0x220], tip[0x220];
    if (g_6d3acc) {
        Window_SendEvent5(w, 0, 0x86, 0x2b3e, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6d, 0x2b3f, 3, 0, 0);
        bool simple = g_newGame[0x82] || F<int32_t>(g_world, 0x4aef);
        Window_SendEvent5(w, 0, simple ? 0x87 : 0x65, 0x275c, 3, 0, 0);
        uint8_t* c = Widget_FindChild(w, 0, 0x2afa);
        String_Assign(&F<char*>(c, 0xce), GetString(0x540));
        String_Assign(&F<char*>(c, 0x43), GetString(0x541));
        c = Widget_FindChild(w, 0, 0x2afb);
        String_Assign(&F<char*>(c, 0x43), GetString(0x542));
        uint8_t* rec0 = (uint8_t*)Collection_Get(engines(), 0, Collection_IdAt(engines(), 0, 0, 0));
        uint32_t co, pl;
        who(co, pl);
        int32_t cost = Ftol(Obj_EffectScale(rec0, 0, co, pl));
        uint8_t* box = Widget_FindChild(w, 0, 0x275c);
        Num_FormatSimple2(num, 0xb, (double)cost, 0xb);
        uint8_t* t0 = F<uint32_t>(rec0, 0x72) ? (uint8_t*)Collection_Get(types(), 0, F<uint32_t>(rec0, 0x72)) : nullptr;
        const char* nm = LocalizeName((const char*)t0 + 0x45);
        FormatText(text, GetString(0x543), nm, num, 0, 0, 0, 0, 0, 0);
        String_Assign(&F<char*>(box, 0xba), text);
        Window_SendEvent5(w, 0, 0x88, 0x275a, (uint32_t)(uintptr_t)GetString(0xe4), 0, 0);
        for (int32_t pct = 10, k = 0; k < 13; ++k, pct += 5) {
            Num_Format(num, 1, (double)pct, 10, 1, 0, 0);
            FormatText(tip, GetString(0x544), num, 0, 0, 0, 0, 0, 0, 0);
            Window_SendEvent5(w, 0, 0x88, 0x275a, (uint32_t)(uintptr_t)tip, pct, 0);
        }
        Window_SendEvent5(w, 0, 0x66, 0x275a, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x88, 0x2759, (uint32_t)(uintptr_t)GetString(0xe4), 0, 0);
        Window_SendEvent5(w, 0, 0x88, 0x2759, (uint32_t)(uintptr_t)GetString(0x545), 0x3e8, 0);
        uint8_t* cur = (uint8_t*)Collection_Get(g_trains, 0, g_6d3acc);
        if (cur) {
            const char* n = LocalizeName((const char*)Obj_Ref388(Obj_City41(cur)) + 0x45);
            Window_SendEvent5(w, 0, 0x88, 0x2759, (uint32_t)(uintptr_t)n, F<uint32_t>(Obj_Ref388(Obj_City41(cur)), 0x126), 0);
        }
        auto trains = [] { return g_trains; };
        for (int32_t i = 0; i < Collection_Count(engines()); ++i) {
            uint32_t typeId = Collection_IdAt(engines(), 0, i, 0);
            for (int32_t j = 0; j < Collection_Count(trains()); ++j) {
                void* loco = Obj_City41(Collection_Get(trains(), 0, Collection_IdAt(trains(), 0, j, 1)));
                uint32_t e = F<uint32_t>(Obj_Ref388(loco), 0x126);
                if (e != typeId) continue;
                if (cur && F<uint32_t>(Obj_Ref388(Obj_City41(cur)), 0x126) == e) continue;
                const char* n = LocalizeName((const char*)Obj_Ref388(loco) + 0x45);
                Window_SendEvent5(w, 0, 0x88, 0x2759, (uint32_t)(uintptr_t)n, F<uint32_t>(Obj_Ref388(loco), 0x126), 0);
                break;
            }
        }
        Window_SendEvent5(w, 0, 0x66, 0x2759, 0, 0, 0);
        memset((void*)0x6d1aa8, 0, 0x2000);
        Window_SendEvent5(w, 0, 0x7e, 0x275d, 0x511050, 0, 0);
        Window_SendEvent5(w, 0, 0x7f, 0x275d, 0x50f6b0, 0, 0);
        Window_SendEvent5(w, 0, 0x7a, 0x275d, 0x510e30, 0, 0);
        for (int32_t i = 0; i < Collection_Count(trains()); ++i) {
            uint8_t* t = (uint8_t*)Collection_Get(trains(), 0, Collection_IdAt(trains(), 0, i, 1));
            int32_t owner = F<int32_t>(t, 0x51);
            if (owner != Obj_Get21(g_world)) continue;
            uint32_t id = F<uint32_t>(t, 0x25);
            if (id >= 0x2000) continue;
            if (id == g_6d3acc) hook::Global<uint8_t>(0x6d1aa8 + id) = 1;
            Window_SendEvent5(w, 0, 0x88, 0x275d, 0x5c87a8, Collection_IdAt(trains(), 0, i, 1), 0);
        }
        Window_SendEvent5(w, 0, 0x79, 0x2759, 0x510b60, 0, 0);
        Window_SendEvent5(w, 0, 0x79, 0x275a, 0x510cc0, 0, 0);
        Window_SendEvent5(w, 0, 0x79, 0x2afe, 0x510e20, 0, 0);
        Window_SendEvent5(w, 0, 0x95, 0x2afe, 0, 0, 0);
        UpgradeDialog_UpdateCostText();
    } else {
        Window_SendEvent5(w, 0, 0x86, 0x2b3f, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6d, 0x2b3e, 3, 0, 0);
        for (int32_t i = 0; i < g_6d3aec; ++i) Window_SendEvent5(w, 0, 0x88, 0x2aff, 0, i, 0);
        Window_SendEvent5(w, 0, 0x7a, 0x2aff, 0x50f660, 0, 0);
        Window_SendEvent5(w, 0, 0x7e, 0x2aff, 0x50f650, 0, 0);
        Window_SendEvent5(w, 0, 0x7f, 0x2aff, 0x50f640, 0, 0);
        Window_SendEvent5(w, 0, 0x79, 0x2aff, 0x5114f0, 0, 0);
    }
    uint16_t year = F<uint16_t>(g_world, 0xd);
    int32_t rate = year < 0x73a ? 10 : year < 0x76c ? 20 : year < 0x79e ? 40 : 80;
    g_6d3aac = rate;
    g_6d3ab0 = Ftol((double)rate * D(0x5d12f8) + D(0x5c8628));
    int32_t sum = g_6d3ab0 + rate;
    g_6d3ab4 = Ftol((double)sum / D(0x5c85d0) + D(0x5c8628));
    for (int32_t k = 0; k < 3; ++k) {
        Num_Format(num, 1, (double)hook::Global<int32_t>(0x6d3aac + k * 4), 10, 1, 0, 0);
        FormatText(tip, GetString(0x546 + k), num, 0, 0, 0, 0, 0, 0, 0);
        strcat(tip, GetString(0xf09));
        Window_SendEvent5(w, 0, 0x6e, 0x2761 + k, (uint32_t)(uintptr_t)tip, 0, 0);
    }
    TrainList_UpdateButtons(w);
    Window_SendEvent5(w, 0, 0x6e, 0x2b02, (uint32_t)(uintptr_t)GetString(0xfe5), 0, 0);
}
RT3_REPLACE(0x511500, TrainBuy_Init);

// 0x512080. Refills the train list 0x2aff (busy flag 0x6d3ae1) with the 0x6d3aec
// trains, keeping the selection (clamped; first row when it was none) and the
// scroll position (+0x1e2), ticks sort button 0x2761 + 0x622b28, then updates the
// station buttons.
RT3_NOINLINE void __fastcall TrainList_Refill(uint8_t* w) {
    g_6d3ae1 = 1;
    int32_t sel = F<int32_t>(Widget_FindChild(w, 0, 0x2aff), 0x1d6);
    uint32_t scroll = F<uint32_t>(Widget_FindChild(w, 0, 0x2aff), 0x1e2);
    Window_SendEvent5(w, 0, 0x8b, 0x2aff, 0, 0, 0);
    for (int32_t i = 0; i < g_6d3aec; ++i) Window_SendEvent5(w, 0, 0x88, 0x2aff, 0, (uint32_t)i, 0);
    int32_t n = g_6d3aec;
    if (sel >= n) sel = n - 1;
    if (sel < 0 && n > 0) sel = 0;
    Window_SendEvent5(w, 0, 0x74, hook::As<uint32_t>(g_622b28) + 0x2761, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x67, 0x2aff, (uint32_t)sel, 0, 0);
    Window_SendEvent5(w, 0, 0x6b, 0x2aff, scroll, 0, 0);
    g_6d3ae1 = 0;
    TrainList_UpdateButtons(w);
}
RT3_REPLACE(0x512080, TrainList_Refill);

 // namespace

// 0x512160 (event handler). The consist editor d: buy the picked engine
// (0x6d3acc clear) with the orders 0x6d3ae8, or swap the engine of the
// trains ticked in 0x6d1aa8 (upgrade mode; multiplayer request 0x39, its
// trains with their ids); the price (the engine's effect-scaled cost, or the
// selection's total) against the cash plus 500 (strings 0xc70 / 0xe0a for
// several, 0xc71 without +0x4a8b; free in a tutorial or with +0x4aef); new
// trains need two plain stops (0x54b) and a drivable route (0xc0e with
// 0xc0f / 0xc10 / 0xc11), in multiplayer go to the host; a new train starts
// running (0x6d3aa8), takes the name 0x6d3ad8 (as +0x1a1 or +0x1ce kind)
// and the sort key 0x622b24, and is painted. While waiting for the host
// (+0x78) it closes on the answer (0x6d1aa4, -1 refused: string 0x549).
// Also: the engine scheme steps 0x28a0.. / 0x2904.., camera pan / zoom /
// rotate, stops (add 0x2b02 at a picked station, Shift before the
// selection; remove 0x2b03; edit 0x2b04 / 0x2aff), the current stop
// buttons 0x2b0c.., the train ticks 0x2774.. and the sort 0x2761...
int32_t __fastcall ConsistDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (F<int32_t>(d, 0x78)) {
        if (g_6d1aa4 == -1) {
            g_6d1aa4 = 0;
            Message497(GetString(0x549));
            msg[0] = 0xc9;
            return 1;
        }
        if (g_6d1aa4) msg[0] = 0xc9;
        return 1;
    }
    auto close = [&]() {
        msg[0] = 0xc9;
        return 1;
    };
    auto locked = [&](uint32_t text) {
        Message497(GetString(text), 0x3ef5c28f);
        return 0;
    };
    auto camera = [&](uint32_t type, uint32_t a, uint32_t b, uint32_t c) {
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, type, 0x2afc, a, b, c);
        return 0;
    };
    const int32_t id = (int32_t)msg[1];
    switch (msg[0]) {
    case 0:
        if (g_6d3adc) {
            msg[1] = 0x2afb;
            return close();
        }
        return 1;
    case 0xca:
        if (id >= 0x2774 && id <= 0x27d8) {
            const uint32_t train = F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x82);
            if (train < 0x2000) {
                uint8_t& tick = *(uint8_t*)(0x6d1aa8 + train);
                tick = tick == 0;
            }
            g_6d3ae0 = 1;
            Window_SendEvent5(d, 0, 0x66, 0x2759, 0, 0, 0);
            Window_SendEvent5(d, 0, 0x66, 0x275a, 0, 0, 0);
            g_6d3ae0 = 0;
            UpgradeDialog_UpdateCostText();
            return 0;
        }
        if (id > 0x2aff) {
            if (id < 0x2b0c || id > 0x2b0e) return 1;
            const int32_t top = F<int32_t>(Widget_FindChild(d, 0, 0x2aff), 0x1e2);
            Ui_PlaySound(0x21, 0, 0x3f000000);
            TrainOrders_SetCurrent(kOrders497, 0, top + id - 0x2b0c, 0);
            return 0;
        }
        if (id == 0x2aff) {
            if (g_6d3b4c && msg[1] != g_622b38) return locked(0xe03);
            TrainList_UpdateButtons(d);
            IndustryEditor_Run((uint32_t)(uintptr_t)kOrders497, (uint8_t*)Array33_At(kOrders497, 0, (int32_t)msg[2]), msg[2]);
            TrainList_Refill(d);
            return 0;
        }
        if (id >= 0x2761 && id <= 0x2763) {
            g_622b28 = id - 0x2761;
            return 0;
        }
        return 1;
    case 0xcb:
        break;
    default:
        return 1;
    }
    if (id >= 0x28a0 && id < 0x2968) {
        const bool up = id < 0x2904;
        uint8_t* scheme = (uint8_t*)Obj_Cargo72((uint8_t*)Collection_Get(g_6ada84, 0, (uint32_t)(id - (up ? 0x28a0 : 0x2904))));
        if (up) {
            ++F<int32_t>(scheme, 0x262);
            if (F<int32_t>(scheme, 0x262) == Collection_Count(F<Collection*>(scheme, 0x196))) F<int32_t>(scheme, 0x262) = -1;
        } else if (--F<int32_t>(scheme, 0x262) == -2) {
            F<int32_t>(scheme, 0x262) = Collection_Count(F<Collection*>(scheme, 0x196)) - 1;
        }
        return 0;
    }
    switch (id) {
    case 0x283c: return camera(0xbc, 0, 1, 0);
    case 0x283d: return camera(0xbc, 0, 0xffffffff, 0);
    case 0x283e: return camera(0xbc, 1, 0, 0);
    case 0x283f: return camera(0xbc, 0xffffffff, 0, 0);
    case 0x2b00: return camera(0xad, 0xfffff490, 0, 0);
    case 0x2b01: return camera(0xad, 0x102e, 0, 0);
    case 0x2afb:
        if (g_6d3b4c) return locked(0xe8c);
        return close();
    case 0x2b02: {
        if (g_6d3b4c) return locked(0xe8c);
        const bool insert = (F<uint8_t>(g_input, 0xa8c) & 3) != 0;
        const int32_t station = StationPick_Run(0);
        if (station <= -1) return 0;
        uint8_t stop[0x33] = {};
        stop[0x28] &= 0x7f;
        F<uint32_t>(stop, 0x24) = 0x384;
        stop[0x23] = 4;
        stop[0x22] = 0;
        F<uint16_t>(stop, 0x29) = (uint16_t)station;
        F<uint32_t>(stop, 0x2f) = 0x3e395810;
        int32_t idx = F<int32_t>(kOrders497, 4);
        if (insert) idx = StopRow497(d);
        if (idx < 0) idx = 0;
        TrainOrders_Insert(kOrders497, 0, idx, stop);
        return 0;
    }
    case 0x2b03: {
        if (g_6d3b4c) return locked(0xe8c);
        const int32_t row = StopRow497(d);
        if (row < 0) return 0;
        TrainOrders_Remove(kOrders497, 0, row);
        return 0;
    }
    case 0x2b04: {
        if (g_6d3b4c && msg[1] != g_622b38) return locked(0xe03);
        const int32_t row = StopRow497(d);
        if (row >= 0) IndustryEditor_Run((uint32_t)(uintptr_t)kOrders497, (uint8_t*)Array33_At(kOrders497, 0, row), (uint32_t)row);
        TrainList_Refill(d);
        return 0;
    }
    case 0x2afa:
        break;
    default:
        return 1;
    }
    // Buy / upgrade.
    if (g_6d3b4c && msg[1] != g_622b38) return locked(0xe03);
    int32_t total, count;
    if (hook::As<int32_t>(g_6d3acc)) {
        Editor_SelectionCost(&total, &count);
    } else {
        uint8_t* engine = (uint8_t*)Collection_Get(g_6ada84, 0, TrainList_Selected());
        const uint32_t player = (uint32_t)Obj_Get25(g_world);
        const uint32_t company = (uint32_t)Obj_Get21(g_world);
        total = (int32_t)Obj_EffectScale(engine, 0, company, player);
        count = 1;
    }
    const float cash = (float)Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329);
    const float price = (float)total;
    if (cash + BitsF(0x43fa0000) < price && !g_newGame[0x82] && !F<int32_t>(g_world, 0x4aef)) {
        char need[0xc8], have[0xc8], many[0xc8], text[0x1e4];
        Num_FormatSimple2(need, 0xb, (double)total, 0xb);
        Num_FormatSimple2(have, 0xb, cash, 0xb);
        if (count > 1) {
            Num_Format(many, 1, (double)count, 0xa, 1, 0, 0);
            FormatText(text, GetString(0xe0a), many, need, have, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            FormatText(text, GetString(0xc70), need, have, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        if (!F<int32_t>(g_world, 0x4a8b)) strcat(text, GetString(0xc71));
        Message497(text);
        return 0;
    }
    if (hook::As<int32_t>(g_6d3acc)) {
        if (!IsMultiplayer()) {
            for (int32_t i = 0; i < 0x2000; ++i) {
                if (!*(uint8_t*)(0x6d1aa8 + i)) continue;
                uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, (uint32_t)i);
                if (!t || !OwnTrain497(t)) continue;
                Train_ReplaceEngine(t, 0, TrainList_Selected(), 0);
                Train_ApplyLivery(t);
            }
            return close();
        }
        uint8_t* req = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(0x400a);
        F<int32_t>(req, 0) = Obj_Get25(g_world);
        F<uint32_t>(req, 4) = TrainList_Selected();
        int32_t n = 0;
        for (int32_t i = 0; i < 0x2000; ++i) {
            if (!*(uint8_t*)(0x6d1aa8 + i)) continue;
            uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, (uint32_t)i);
            if (!t || !OwnTrain497(t)) continue;
            F<uint16_t>(req, 0xa + n * 2) = (uint16_t)i;
            ++n;
        }
        F<uint16_t>(req, 8) = (uint16_t)n;
        Cursor_Set(g_cursorManager, 0, 6, 0, 0);
        Net_SendToHost(g_session, 0, 0x39, n * 2 + 0xa, req);
        ((void(__cdecl*)(void*))0x5a1145)(req);
        return close();
    }
    int32_t electric = 0;
    if (Collection_IsValid(g_6ada84, 0, TrainList_Selected()))
        electric = F<int32_t>(Collection_Get(g_6ada84, 0, TrainList_Selected()), 0x10) == 2;
    if (OrderList_CountPlain(kOrders497) < 2) {
        Message497(GetString(0x54b));
        return 0;
    }
    if (!Orders_Drivable((uint32_t*)kOrders497, 0, nullptr, -1, electric, 0, Obj_Get21(g_world))) {
        char text[0x1e4];
        if (!Orders_Drivable((uint32_t*)kOrders497, 0, nullptr, -1, 0, 0, 0)) {
            const char* why = GetString(0xc11);
            FormatText(text, (const char*)0x5c8730, GetString(0xc0e), why, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            const bool own = Orders_Drivable((uint32_t*)kOrders497, 0, nullptr, -1, 0, 0, Obj_Get21(g_world)) != 0;
            const char* why = GetString(own ? 0xc0f : 0xc10);
            FormatText(text, (const char*)0x5d1988, GetString(0xc0e), why, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        Message497(text);
        return 0;
    }
    if (IsMultiplayer()) {
        UpgradeDialog_SendTrains(d);
        return 0;
    }
    hook::As<uint32_t>(g_6d1aa4) = Train_Create(TrainList_Selected(), kOrders497, 1, -1);
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, hook::As<uint32_t>(g_6d1aa4));
    if (!t) return close();
    if (hook::As<int32_t>(g_6d3aa8) >= 0) Train_SetRunning(t, 0, hook::As<int32_t>(g_6d3aa8));
    bool named = false;
    if (hook::As<int32_t>(g_6d3ad0) && g_6d3ad8) {
        t[0x1a1] = 1;
        named = true;
    } else if (hook::As<int32_t>(g_6d3ad4) && g_6d3ad8) {
        t[0x1ce] |= 1;
        named = true;
    }
    if (named) StrCopyN((char*)t + 0x69, g_6d3ad8, 0x1e);
    if (g_622b24 != -1)
        Collection_SetKey1(g_trains, 0, hook::As<uint32_t>(g_6d1aa4), hook::As<uint32_t>(g_622b24));
    Train_ApplyLivery(t);
    return close();
}
RT3_REPLACE(0x512160, ConsistDialog_HandleEvent);

// 0x512c50 (fastcall owner, train; then orders, a2 .. a7). Runs the consist
// editor (once at a time, 0x6d3ae4): a working copy of the orders (the
// train's, else `orders`, into 0x6d3ae8 with current and flag), the engine
// (a2 / 0x6d1aa0, the train's city cargo's +0x126) and +0xd8 (a3 /
// 0x6d3aa8), single-player presets a4..a7 (0x6d3ad0.., 0x622b24), the owner's
// +0x25 (0x6d3acc). Without an owner the view resets first. The editor
// window (vtable 0x5d193c) runs modally with the game paused; cancelling
// (0x2afb, or 0x6d3adc) drops the copy. Afterwards the train view reopens
// on 0x6d1aa4 and the cursor follows 0x6d3ab8.
RT3_NOINLINE void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7) {
    if (g_6d3ae4) return;
    g_6d3ae4 = 1;
    g_6d3adc = 0;
    *(uint8_t*)0x6d3ae2 = 0;
    g_6d3ad0 = 0, g_6d3ad4 = 0, hook::As<uint32_t>(g_6d3ad8) = 0;
    g_622b24 = -1;
    if (!IsMultiplayer()) {
        g_6d3ad0 = a4;
        g_6d3ad4 = a5;
        hook::As<uint32_t>(g_6d3ad8) = a6;
        hook::As<uint32_t>(g_622b24) = a7;
    }
    g_6d3acc = owner ? F<uint32_t>(owner, 0x25) : 0;
    g_6d1aa0 = a2;
    g_6d3aa8 = a3;
    hook::As<uint32_t>(g_6d1aa4) = 0;
    uint8_t* work = (uint8_t*)0x6d3ae8;
    OrderList_Init(work);
    uint8_t* src = orders;
    if (train) {
        src = (uint8_t*)Identity(train);
        uint8_t* city = (uint8_t*)Obj_CityRef(train);
        uint32_t ty = F<uint32_t>(city, 0x72);
        uint8_t* t = ty ? (uint8_t*)Collection_Get(g_carTypes, 0, ty) : nullptr;
        g_6d1aa0 = F<uint32_t>(t, 0x126);
        g_6d3aa8 = F<uint32_t>(train, 0xd8);
    }
    if (src) {
        for (int32_t i = 0; i < F<int32_t>(src, 4); ++i) TrainOrders_Insert(work, 0, i, (uint8_t*)Array33_At(src, 0, i));
        g_6d3af0 = F<int32_t>(src, 8);
        *(uint8_t*)0x6d3af4 = src[0xc];
    }
    if (!owner) ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(g_gameWindow, 0, 0, 0);
    uint8_t* w = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x80);
    if (w) {
        Widget_SetVtblRet(w);
        F<uint32_t>(w, 0) = 0x5d193c;
        TrainBuy_Init(w);
        F<int32_t>(w, 0x78) = 0;
    }
    g_6d3ac8 = w;
    TrainList_Refill(w);
    PauseLock_Push(hook::As<int32_t*>(g_app));
    uint32_t r = Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d3ac8, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d3ac8);
    if ((r == 0x2afb || g_6d3adc) && g_6d3ae8) {
        ((void(__cdecl*)(void*))0x5a1145)(g_6d3ae8);
        OrderList_Init(work);
    }
    uint8_t* win = g_6d3ac8;
    g_6d3adc = 0;
    if (win) {
        F<uint32_t>(win, 0) = 0x5d193c;
        Panel_ReleaseImages(win);
        Widget_SetVtbl(win);
        ((void(__cdecl*)(void*))0x53b080)(win);
    }
    g_6d3ac8 = nullptr;
    if (g_6d1aa4)
        ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(g_gameWindow, 0, 2, g_6d1aa4);
    if (int32_t c = hook::As<int32_t>(g_6d3ab8)) Cursor_Set(g_cursorManager, 0, c, 0, 0);
    g_6d3ae4 = 0;
}
RT3_REPLACE(0x512c50, ConsistEditor_Run);

// 0x512ec0 (fastcall). Copies train order list src into dst (inserting each
// entry; count, current and flag +0xc copied over) for no train (+0xd = 0).
RT3_NOINLINE void __fastcall TrainOrders_Copy(uint8_t* src, uint8_t* dst) {
    for (int32_t i = 0; i < F<int32_t>(src, 4); ++i) TrainOrders_Insert(dst, 0, i, (uint8_t*)Array33_At(src, 0, i));
    F<int32_t>(dst, 4) = F<int32_t>(src, 4);
    F<int32_t>(dst, 8) = F<int32_t>(src, 8);
    dst[0xc] = src[0xc];
    F<uint32_t>(dst, 0xd) = 0;
}
RT3_REPLACE(0x512ec0, TrainOrders_Copy);

// 0x512f10. Train detail window ("TrainDetail.win") destructor: drops its
// buttons 0x65c2..0x65d4 and its three images (0x6d3b14 / 18 / 1c).
void __fastcall TrainDetail_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1990;
    Window_DeleteChildren(w, 0, 0x65c2, 0x65d4, 0);
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d3b14));
    Pack_Release(g_pack, 0, g_6d3b18);
    Pack_Release(g_pack, 0, g_6d3b1c);
    hook::As<int32_t>(g_trainWindow) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x512f10, TrainDetail_Destruct);

// 0x512f80. Switches window w's large layout (0x6d3b28) on or off: resizes panel
// 0x659d (event 0x9e) to 508 or 292 pixels (of 1600 x 1200) and swaps which of the
// buttons 0x6594 / 0x6593 is enabled.
RT3_NOINLINE void __fastcall Window_SetLargeLayout(uint8_t* w, int, int32_t large) {
    g_6d3b28 = large;
    double px = (double)(large ? 0x1fc : 0x124);
    uint32_t ev[12] = {};
    ev[0] = 0x9e;
    ev[1] = 0x659d;
    *(float*)&ev[6] = (float)(px / D(0x5ca2c8));
    *(float*)&ev[7] = (float)(px / D(0x5ca2d0));
    Window_HandleEvent(w, 0, ev);
    Window_SendEvent5(w, 0, g_6d3b28 ? 0x86 : 0x6d, 0x6594, 3, 0, 0);
    Window_SendEvent5(w, 0, g_6d3b28 ? 0x6d : 0x86, 0x6593, 3, 0, 0);
}
RT3_REPLACE(0x512f80, Window_SetLargeLayout);

// 0x513050. Network message m (+0 player, +4 train): the train is sold; for
// the sender the cursor resets, the view leaves the train window and input
// event 0x3e8 goes out.
RT3_NOINLINE void __fastcall NetMsg_SellTrain(uint8_t* m) {
    Collection* trains = g_trains;
    trains->vtbl->release(trains, F<uint32_t>(m, 4));
    if (Obj_Get25(hook::As<void*>(g_world)) != F<int32_t>(m, 0)) return;
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(g_gameWindow, 0, 2, 0);
    Game_PostInputEvent(0x3e8, 0, 0, 0, 0);
}
RT3_REPLACE(0x513050, NetMsg_SellTrain);

RT3_NOINLINE double __fastcall Train_Total109(uint8_t* t) { return Train_Float109(t, 0, 0) + F<float>(t, 0xfd); }
RT3_REPLACE(0x5130b0, Train_Total109);

RT3_NOINLINE double __fastcall Train_Total131(uint8_t* t) { return Train_Float131(t, 0, 0) + F<float>(t, 0x101); }
RT3_REPLACE(0x5130d0, Train_Total131);

RT3_NOINLINE void* SelectedTrain() {
    if (!SelectedTrainValid()) return nullptr;
    return Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
}
RT3_REPLACE(0x5130f0, SelectedTrain);

RT3_NOINLINE int32_t SelectedTrainId() { return SelectedTrainValid() ? F<int32_t>(g_gameWindow, 0x90) : -1; }
RT3_REPLACE(0x513130, SelectedTrainId);

RT3_NOINLINE int32_t HasSelectedTrain() { return SelectedTrainValid() ? 1 : 0; }
RT3_REPLACE(0x513170, HasSelectedTrain);

RT3_NOINLINE int32_t SelectedTrainIsOwn() {
    if (!SelectedTrainValid() || !SelectedTrainValid()) return 0;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!t) return 0;
    return F<int32_t>(t, 0x51) == Obj_Get21(hook::As<void*>(g_world)) ? 1 : 0;
}
RT3_REPLACE(0x5131a0, SelectedTrainIsOwn);

// 0x513220. Train window: message 0x250 for the selected train (0x6d0818
// +0x90 in 0x6cfcbc) with the name of its company.
RT3_NOINLINE void TrainWindow_ShowOwnerMessage() {
    auto selected = []() {
        uint8_t* v = g_gameWindow;
        return v && F<int32_t>(v, 0x90) > 0 && Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90));
    };
    if (!selected()) return;
    const char* name = (const char*)0x5c87a8;
    if (selected()) {
        if (void* t = Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90)))
            if (uint8_t* c = (uint8_t*)Train_Company(t)) name = (const char*)c + 4;
    }
    char text[0x1f4];
    FormatText(text, GetString(0x250), name, 0, 0, 0, 0, 0, 0, 0);
    Message_Show(text, 1, 0);
}
RT3_REPLACE(0x513220, TrainWindow_ShowOwnerMessage);

// 0x513310 (draw callback). Draws the focused train's row highlighted,
// inset by 0x5d0100.
void __fastcall TrainView_DrawRowCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* v = g_gameWindow;
    if (!v || F<int32_t>(v, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90))) return;
    uint8_t* t = nullptr;
    v = g_gameWindow;
    if (v && F<int32_t>(v, 0x90) > 0 && Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90)))
        t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    const float iw = (float)((double)w - Fl(0x5d0100));
    const float ix = (float)((double)x + Fl(0x5d0100));
    TrainList_DrawEntry(t, 1, ix, y, iw, h);
}
RT3_REPLACE(0x513310, TrainView_DrawRowCb);

// 0x5133b0 (draw callback). Draws stop label idx (EDX) of the focused
// train.
void __fastcall TrainView_DrawStopCb(void*, int32_t idx, float x, float y, int32_t a, float h, int32_t) {
    uint8_t* v = g_gameWindow;
    if (!v || F<int32_t>(v, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90))) return;
    if (!Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* t = nullptr;
    v = g_gameWindow;
    if (v && F<int32_t>(v, 0x90) > 0 && Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90)))
        t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    StopList_DrawLabel(Identity(t), idx, x, y, a, h);
}
RT3_REPLACE(0x5133b0, TrainView_DrawStopCb);

// 0x513460 (callback). Deletes widget w's generated rows while a train is
// focused.
void __fastcall TrainView_DeleteRowsCb(uint8_t* w, int32_t e, int32_t a) {
    uint8_t* v = g_gameWindow;
    if (!v || F<int32_t>(v, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90))) return;
    Widget_DeleteGeneratedRows(w, e, a);
}
RT3_REPLACE(0x513460, TrainView_DeleteRowsCb);

// 0x5134a0 (callback). Adds the focused train's cargo buttons of kind EDX
// to row w unless it is the active page button (0x65a4 with 0x6d3b24, else
// 0x65a3).
void __fastcall TrainView_AddCargoButtonsCb(uint8_t* w, int32_t type, int32_t, float x, float y, int32_t a, int32_t b) {
    uint8_t* v = g_gameWindow;
    if (!v || F<int32_t>(v, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90))) return;
    const uint16_t page = g_6d3b24 ? 0x65a4 : 0x65a3;
    if (F<uint16_t>(w, 4) == page) return;
    BuildingRow_AddCargoButtons(w, (uint8_t*)Identity(SelectedTrain()), type, x, y, a, b);
}
RT3_REPLACE(0x5134a0, TrainView_AddCargoButtonsCb);

// 0x513520 (tooltip callback). Tooltips of the selected train's (0x6d0818
// +0x90) detail fields 0x65b6 / 0x65b9 / 0x65bc (+0xdc / +0xe0 / +0xe4,
// strings 0x256..0x258) and 0x65bf (running cost x 0x5d19a0, string 0x259);
// other ids show an empty one.
RT3_NOINLINE void __fastcall TrainDetail_TooltipCb(int32_t id, int, int32_t, int32_t) {
    uint8_t* sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(sel, 0x90))) return;
    char frame[0x644];
    char* num = frame + 4;
    char* text = frame + 0x324;
    text[0] = 0;
    sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(sel, 0x90))) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!t) return;
    int32_t str = 0;
    if (id == 0x65b6) {
        Num_Format(num, 5, (double)F<float>(t, 0xdc), 10, 1, 0, 0);
        str = 0x256;
    } else if (id == 0x65b9) {
        Num_Format(num, 5, (double)F<float>(t, 0xe0), 10, 1, 0, 0);
        str = 0x257;
    } else if (id == 0x65bc) {
        Num_Format(num, 5, (double)F<float>(t, 0xe4), 10, 1, 0, 0);
        str = 0x258;
    } else if (id == 0x65bf) {
        Num_Format(num, 6, (double)Train_RunningCost(t) * D(0x5d19a0), 10, 1, 0, 0);
        str = 0x259;
    }
    if (str) FormatText(text, GetString(str), num, 0, 0, 0, 0, 0, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
}
RT3_REPLACE(0x513520, TrainDetail_TooltipCb);

// 0x513720. Enables buttons 0x65a1 / 0x65a2 only when list 0x65a3 (or 0x65a4 when
// 0x6d3b24 is clear) has a selection, and captions them accordingly (strings
// 0x25b / 0x25d without one, 0x25c / 0x25e with).
RT3_NOINLINE void __fastcall Window_UpdateListButtons(uint8_t* w) {
    uint8_t* list = Widget_FindChild(w, 0, g_6d3b24 ? 0x65a3 : 0x65a4);
    bool none = F<int32_t>(list, 0x1d6) < 0;
    uint32_t type = none ? 0x87 : 0x65;
    Window_SendEvent5(w, 0, type, 0x65a1, 2, 0, 0);
    Window_SendEvent5(w, 0, type, 0x65a2, 2, 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0x65a1, (uint32_t)(uintptr_t)GetString(none ? 0x25b : 0x25c), 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0x65a2, (uint32_t)(uintptr_t)GetString(none ? 0x25d : 0x25e), 0, 0);
}
RT3_REPLACE(0x513720, Window_UpdateListButtons);

// 0x5137d0. Train view: the income table of the selected train (0x6cfcbc,
// main view +0x90) at (x, y), in font 0x6d3b14 (not in sandbox / when editing):
// a label column (strings 3, 0x25f..0x262) and one column per year the train
// ran (at most 2 years back) plus a "total" column (0x263), with the three
// money rows +0x109 / +0x131 / +0x159 and their sum, negative values in red,
// the sum row underlined.
void __fastcall TrainView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(view, 0x90))) return;
    view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(view, 0x90))) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!t || g_newGame[0x82] || F<int32_t>(g_world, 0x4aef)) return;
    static const int32_t labels[5] = { 3, 0x25f, 0x260, 0x261, 0x262 };
    static const uint32_t colX[5] = { 0x3b83126f, 0x3dd2f1aa, 0x3e3126e9, 0x3e78d4fe, 0x3ea04189 };
    int32_t cols = F<uint16_t>(g_world, 0xd) - F<int32_t>(t, 0xd0) + 3;
    if (cols > 4) cols = 4;
    float top = (float)((double)y + D(0x5cfdd8));
    char text[0x184];
    for (int32_t c = 0; c < cols; ++c) {
        float rowY = top;
        float cx = (float)((double)x + BitsF(colX[c]));
        float cw = (float)((double)BitsF(colX[c + 1]) - BitsF(colX[c]));
        int32_t align = (c != 0) + 2;
        for (int32_t r = 0; r < 5; ++r) {
            int32_t flags = 0;
            uint32_t color = 0xffffffff;
            if (c == 0) {
                strcpy(text, GetString(labels[r]));
            } else if (r == 0) {
                if (c == cols - 1) strcpy(text, GetString(0x263));
                else Num_Format(text, 8, (double)(F<uint16_t>(g_world, 0xd) - (c - 1)), 10, 1, 0, 0);
            } else {
                bool total = c == cols - 1;
                float v = 0.0f;
                switch (r) {
                case 1: v = (float)(total ? Train_Total109(t) : Train_Float109(t, 0, c - 1)); break;
                case 2: v = (float)(total ? Train_Total131(t) : Train_Float131(t, 0, c - 1)); break;
                case 3: v = (float)(total ? Train_Total159(t) : Train_Float159(t, 0, c - 1)); break;
                case 4: v = (float)(total ? Train_LifetimeTotal(t) : Obj_Sum3(t, 0, c - 1)); break;
                }
                if (v < Fl(0x5c8598)) color = 0xffff5f5f;
                Num_FormatSimple2(text, 0xb, (double)v, 0xb);
                if (r == 3) flags = 4;
            }
            Font_DrawTextBox(g_6d3b14, 0, text, cx, rowY, cw, BitsF(0x3ca3d70a), color, align, 0, (uint32_t)flags, 1.0f);
            rowY = (float)((double)rowY + D(0x5d10f0));
            if (r == 0) rowY = (float)((double)rowY + D(0x5d1150));
            else if (r == 3) rowY = (float)((double)rowY + D(0x5d0818));
        }
    }
}
RT3_REPLACE(0x5137d0, TrainView_DrawIncomeTable);

 // namespace

// 0x513b50 (draw callback). Train details of the selected train (0x6d0818
// +0x90): its age (string 0x265, 0 with +0x4aeb) under the title 0x264,
// the running cost bar (0x65b1 / 0x65b2, capped at 1) and the level bars of
// +0xdc (0x65b4 / 0x65b5), +0xe0 (0x65b7 / 0x65b8, not for cargo classes
// with +0x10) and +0xe4 (0x65ba / 0x65bb), hiding empty ones; the second
// shows its legend 0x65b9 / 0x65bd swapped for such cargo.
void __fastcall TrainGauges_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(sel, 0x90))) return;
    sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(sel, 0x90))) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!t) return;
    const float tx = (float)((double)x + D(0x5d0818));
    const float ty = (float)((double)y + D(0x5d00a8));
    const float rh = BitsF(0x3ca3d70a);
    Font_DrawTextBox(g_6d3b14, 0, GetString(0x264), tx, ty, BitsF(0x3d3851ec), rh, 0xffffffff, 2, 0, 0, 1.0f);
    int32_t age = F<uint16_t>(g_world, 0xd) - F<int32_t>(t, 0xd4);
    if (F<int32_t>(g_world, 0x4aeb)) age = 0;
    char frame[0xcc];
    char* num = frame + 0xc;
    char* text = frame + 0x4c;
    Num_Format(num, 8, (double)age, 10, 1, 0, 0);
    FormatText(text, GetString(0x265), num, 0, 0, 0, 0, 0, 0, 0);
    const float ax = (float)((double)tx + D(0x5d19b8));
    Font_DrawTextBox(g_6d3b14, 0, text, ax, ty, BitsF(0x3d99999a), rh, 0xffffffff, 2, 0, 0, 1.0f);
    uint8_t* bar = ShownChild466(0x65b1);
    float cost = Train_RunningCost(t);
    cost = (float)((double)cost * D(0x5d19a0));
    if (cost > D(0x5c8770)) cost = 1.0f;
    cost = (float)((double)cost * Fl(0x5ca9f8) + Fl(0x5d19b0));
    cost = (float)(((double)cost + Fl(0x5c8568)) / Fl(0x5ca198));
    Widget_SetClip(bar, 0, 0x3f5ee148, 0x3f281b4f, Bits(cost), 0x3f2aaaab);
    F<float>(ShownChild466(0x65b2), 0xb) = cost;
    uint8_t* g = Widget_FindChild(g_trainWindow, 0, 0x65b4);
    if (F<float>(t, 0xdc) != Fl(0x5c8598)) {
        if (!Widget_IsVisible(g)) Widget_SetFlags(g, 0, 1, 0, 0);
        const float top = (float)(Fl(0x5d19a8) - (double)F<float>(t, 0xdc) * Fl(0x5d19ac));
        Widget_SetClip(g, 0, 0x3f5fae14, Bits((float)((double)top / Fl(0x5ca164))), 0x3f633333, 0x3f1d036a);
        F<float>(ShownChild466(0x65b5), 0xf) = (float)(((double)top - Fl(0x5c8554)) / Fl(0x5ca164));
    } else {
        Widget_ClearFlags(g, 0, 1);
        Window_SendEvent5(g_trainWindow, 0, 0x87, 0x65b5, 1, 0, 0);
    }
    uint8_t* cls = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(Obj_City41(t)), 0x126));
    g = Widget_FindChild(g_trainWindow, 0, 0x65b7);
    bool swapped = false;
    if (F<float>(t, 0xe0) != Fl(0x5c8598) && !F<uint32_t>(cls, 0x10)) {
        if (!Widget_IsVisible(g)) Widget_SetFlags(g, 0, 1, 0, 0);
        const float top = (float)(Fl(0x5d19a8) - (double)F<float>(t, 0xe0) * Fl(0x5d19ac));
        Widget_SetClip(g, 0, 0x3f6b0a3d, Bits((float)((double)top / Fl(0x5ca164))), 0x3f6e8f5c, 0x3f1d036a);
        F<float>(ShownChild466(0x65b8), 0xf) = (float)(((double)top - Fl(0x5c8554)) / Fl(0x5ca164));
    } else {
        Widget_ClearFlags(g, 0, 1);
        Window_SendEvent5(g_trainWindow, 0, 0x87, 0x65b8, 1, 0, 0);
        swapped = F<uint32_t>(cls, 0x10) != 0;
    }
    if (swapped) {
        Window_SendEvent5(g_trainWindow, 0, 0x87, 0x65b9, 1, 0, 0);
        Window_SendEvent5(g_trainWindow, 0, 0x65, 0x65bd, 3, 0, 0);
    } else {
        Window_SendEvent5(g_trainWindow, 0, 0x65, 0x65b9, 1, 0, 0);
        Window_SendEvent5(g_trainWindow, 0, 0x87, 0x65bd, 3, 0, 0);
    }
    g = Widget_FindChild(g_trainWindow, 0, 0x65ba);
    if (F<float>(t, 0xe4) != Fl(0x5c8598)) {
        if (!Widget_IsVisible(g)) Widget_SetFlags(g, 0, 1, 0, 0);
        const float top = (float)(Fl(0x5d19a8) - (double)F<float>(t, 0xe4) * Fl(0x5d19ac));
        Widget_SetClip(g, 0, 0x3f766666, Bits((float)((double)top / Fl(0x5ca164))), 0x3f79eb85, 0x3f1d036a);
        F<float>(ShownChild466(0x65bb), 0xf) = (float)(((double)top - Fl(0x5c8554)) / Fl(0x5ca164));
    } else {
        Widget_ClearFlags(g, 0, 1);
        Window_SendEvent5(g_trainWindow, 0, 0x87, 0x65bb, 1, 0, 0);
    }
}
RT3_REPLACE(0x513b50, TrainGauges_DrawCb);

// 0x514110 (hover callback). Shows the pick tooltip of child 0x659d of
// 0x6d3b20 for the focused train while the first argument is set.
void __fastcall Train_PickTooltipCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    uint8_t* v = g_gameWindow;
    if (!v || F<int32_t>(v, 0x90) <= 0) return;
    if (!Collection_IsValid(g_trains, 0, F<uint32_t>(v, 0x90)) || !on) return;
    Map_ShowPickTooltip(Widget_FindChild(g_trainWindow, 0, 0x659d));
}
RT3_REPLACE(0x514110, Train_PickTooltipCb);

// 0x514160. Refreshes the train window for the selected train (view +0x90): the
// two order lists (0x65a3 / 0x65a4, one row per order, keeping the selection
// +0x1d6 and scroll +0x1e2 of the visible one), their fillers and the cargo
// cell callbacks, the "no orders" text 0x25a, the header row, then the list
// buttons.
RT3_NOINLINE void __fastcall TrainWindow_Refresh(uint8_t* win) {
    uint32_t listId = g_6d3b24 ? 0x65a3 : 0x65a4;
    int32_t sel = F<int32_t>(Widget_FindChild(win, 0, (int32_t)listId), 0x1d6);
    int32_t scroll = F<int32_t>(Widget_FindChild(win, 0, (int32_t)listId), 0x1e2);
    auto send = [&](uint32_t type, uint32_t id, uint32_t a, uint32_t b) { Window_SendEvent5(win, 0, type, id, a, b, 0); };
    send(0x8b, 0x65a3, 0, 0);
    send(0x8b, 0x65a4, 0, 0);
    Window_DeleteChildren(win, 0, 0x65c2, 0x6946, 0);
    Collection* trains = g_trains;
    for (int k = 0; k < 2; ++k) {
        uint8_t* view = g_gameWindow;
        if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(trains, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    }
    uint8_t* t = (uint8_t*)Collection_Get(trains, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!t) return;
    for (int32_t i = 0; i < F<int32_t>(t, 4); ++i) {
        send(0x88, 0x65a3, 0, (uint32_t)i);
        send(0x88, 0x65a4, 0, (uint32_t)i);
    }
    send(0x7a, 0x65a3, 0x5133b0, 0);
    send(0x7a, 0x65a4, 0x5133b0, 0);
    for (uint32_t id : { 0x65b6u, 0x65b9u, 0x65bcu, 0x65bfu }) send(0x82, id, 0x513520, 0);
    char buf[0x100];
    if (F<int32_t>(t, 4) > 0) buf[0] = 0;
    else strcpy(buf, GetString(0x25a));
    send(0x6f, 0x659a, (uint32_t)(uintptr_t)buf, 0);
    send(0x6f, 0x659b, (uint32_t)(uintptr_t)buf, 0);
    int32_t n = F<int32_t>(t, 4);
    if (sel >= n) sel = n - 1;
    if (sel < 0 && n > 0) sel = 0;
    send(0x67, listId, (uint32_t)sel, 0);
    send(0x6b, listId, (uint32_t)scroll, 0);
    send(0x95, listId, 0, 0);
    TrainRow_Build(win, F<uint32_t>(t, 0x25), 1, 0, 0);
    Window_UpdateListButtons(win);
}
RT3_REPLACE(0x514160, TrainWindow_Refresh);

// 0x514410. Refreshes the train window when it is open.
RT3_NOINLINE void TrainWindow_RefreshOpen() {
    if (g_trainWindow) TrainWindow_Refresh(g_trainWindow);
}
RT3_REPLACE(0x514410, TrainWindow_RefreshOpen);

// 0x514420. Train window constructor (vtable 0x5d1990, the open window
// 0x6d3b20): fonts and an image from the pack, the layout 0x5d19c0, the list /
// panel callbacks, the two-tab state 0x6d3b24, the edit buttons (off in
// multiplayer viewing or the scenario editor), the layout size 0x6d3b28 (large
// in multiplayer), a refresh and the help text 0xfe5.
RT3_NOINLINE uint8_t* __fastcall TrainWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1990;
    g_trainWindow = w;
    g_6d3b14 = Pack_AcquireFont(g_pack, 0, 2);
    hook::As<uint8_t*>(g_6d3b18) = Pack_AcquireFont(g_pack, 0, 1);
    hook::As<uint8_t*>(g_6d3b1c) = Pack_AcquireNamed(g_pack, 0, (const char*)0x5c9ee8);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d19c0);
    static const uint32_t events[][3] = { { 0xba, 0x659d, 0 },        { 0x7e, 0x65a3, 0x5134a0 }, { 0x7f, 0x65a3, 0x513460 }, { 0x7e, 0x65a4, 0x5134a0 },
                                          { 0x7f, 0x65a4, 0x513460 }, { 0x7a, 0x65ab, 0x5137d0 }, { 0x7a, 0x65ac, 0x513b50 }, { 0x7a, 0x659c, 0x513310 },
                                          { 0x7b, 0x659d, 0x514110 } };
    for (const auto& e : events) Window_SendEvent5(w, 0, e[0], e[1], e[2], 0, 0);
    Window_SendEvent5(w, 0, g_6d3b24 ? 0x86 : 0x6d, 0x6592, 3, 0, 0);
    Window_SendEvent5(w, 0, g_6d3b24 ? 0x6d : 0x86, 0x6591, 3, 0, 0);
    uint32_t type = g_newGame[0x82] || F<int32_t>(g_world, 0x4aef) ? 0x87 : 0x65;
    Window_SendEvent5(w, 0, type, 0x65ab, 3, 0, 0);
    if (g_newGame[0x82]) g_6d3b28 = 1;
    Window_SetLargeLayout(w, 0, g_6d3b28);
    TrainWindow_Refresh(w);
    Window_SendEvent5(w, 0, 0x6e, 0x65a0, (uint32_t)(uintptr_t)GetString(0xfe5), 0, 0);
    return w;
}
RT3_REPLACE(0x514420, TrainWindow_Construct);

// 0x514620. Applies a train command (player cmd[0], train cmd[1]): the train
// takes the command's settings; the issuing local player's open train window is
// refreshed and the cursor reset. Returns whether the train exists.
RT3_NOINLINE int32_t __fastcall TrainCmd_Apply(uint32_t* cmd) {
    int32_t ok = 0;
    if (Collection_IsValid(g_trains, 0, cmd[1])) {
        Train_SetBuffer1c6((uint8_t*)Collection_Get(g_trains, 0, cmd[1]), 0, (const uint8_t*)cmd);
        ok = 1;
    }
    if (g_trainWindow && Global6d40e4() == (int32_t)cmd[0]) {
        TrainWindow_Refresh(g_trainWindow);
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    }
    return ok;
}
RT3_REPLACE(0x514620, TrainCmd_Apply);

// 0x514690. Steps the selected vehicle (0x6cfcbc list 1, id in 0x6d0818
// +0x90) by `dir`, wrapping around; on the train page (+0x8c 2) only to
// vehicles of the selected train's consist (+0x51), and when the camera
// follows that train's leader the camera follows the new one's leader
// instead. Otherwise it selects it, closes the model preview, refreshes the
// train window (0x6d3b20) and - unless the camera is busy - points the
// camera at its leader (mode option +0x311).
RT3_NOINLINE void __fastcall Vehicles_SelectNext(int32_t dir) {
    int32_t follow = 0, consist = 0;
    uint8_t* panel = g_gameWindow;
    if (F<int32_t>(panel, 0x8c) == 2) {
        if (F<int32_t>(panel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(panel, 0x90))) return;
        uint8_t* tr = (uint8_t*)SelectedTrain();
        if (Obj_City41(tr) && (int32_t)(uintptr_t)Obj_City41(tr) == F<int32_t>(g_62be68, 0x60)) follow = 1;
        consist = F<int32_t>(tr, 0x51);
        panel = g_gameWindow;
    }
    if (F<int32_t>(panel, 0x90) <= 0 || !Collection_IsValid(g_trains, 0, F<uint32_t>(panel, 0x90))) return;
    int32_t at = (int32_t)Collection_PositionOf(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90), 1);
    uint32_t sel = F<uint32_t>(g_gameWindow, 0x90);
    int32_t i = at, steps = 0;
    uint32_t id;
    for (;;) {
        ++steps;
        i = (Collection_Count(g_trains) + i + dir) % Collection_Count(g_trains);
        if (i == at) return;
        id = Collection_IdAt(g_trains, 0, (uint32_t)i, 1);
        if (!id) continue;
        if (consist && F<int32_t>(Collection_Get(g_trains, 0, id), 0x51) != consist) continue;
        if (id == sel && steps < Collection_Count(g_trains)) continue;
        break;
    }
    Panel_SetValue90(g_gameWindow, 0, (int32_t)id);
    ModelPreview_Close();
    if (uint8_t* w = g_trainWindow) TrainWindow_Refresh(w);
    if (follow) {
        uint8_t* tr = (uint8_t*)SelectedTrain();
        if (Obj_City41(tr)) World_FollowEntity(g_world, 0, 1, 1, F<int32_t>(Obj_City41(tr), 0x34a));
        return;
    }
    if (F<int32_t>(g_62be68, 0x60)) return;
    uint8_t* o = (uint8_t*)Collection_Get(g_trains, 0, id);
    int32_t mode = F<int32_t>(g_app, 0x311);
    Camera_FollowObject((uint8_t*)Obj_City41(o), mode);
}
RT3_REPLACE(0x514690, Vehicles_SelectNext);

// 0x514880 / 0x514890. Next / previous vehicle.
RT3_NOINLINE void Vehicles_SelectNextFwd() { Vehicles_SelectNext(1); }
RT3_REPLACE(0x514880, Vehicles_SelectNextFwd);

RT3_NOINLINE void Vehicles_SelectNextBack() { Vehicles_SelectNext(-1); }
RT3_REPLACE(0x514890, Vehicles_SelectNextBack);

 // namespace

// 0x5148a0 (event handler). The train window w of the selected train (the
// main view's +0x90): idle marks it for the world (+0x4cb6), refreshes the
// hover tooltips (every 150 ms the buttons 0x65b6 / 0x65b9 / 0x65bc /
// 0x65bf) and leaves the view when the train is gone; 0x3e9 for it reopens
// the train view. Clicks: the camera (0x659c: follow the lead car / name
// the train by the tool flags), the stop lists (open the stop's cargo
// editor; in multiplayer on a copy), the report rows 0x65c2.., the stop
// buttons 0x660c.. (make one current; request 0x3d). Buttons: zoom /
// pan / rotate the train camera (0x659e.., 0x6947..), add a stop at a
// picked station (0x65a0, Shift inserts at the selection; request 0x41;
// the train restarts or waits, states 0x13 / 0xa), remove one (0x65a1,
// request 0x3f), the list tabs, the large / small layout, sell (0x65a9,
// string 0x254; request 0x37), the consist editor (0x65aa / 0x65be),
// follow / unfollow the train, the next / previous vehicle. Tutorial and
// scenario locks (0x6d3b4c, the world's +0x4a97.. flags) show 0xe8c /
// 0x251, other owners' trains their owner, busy trains 0x252 .. 0x255.
int32_t __fastcall TrainWindow_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    uint8_t* world = g_world;
    auto locked = [&]() {
        Message496(0xe8c, 1, 0x3ef5c28f);
        return 0;
    };
    auto notOwn = [&]() {
        TrainWindow_ShowOwnerMessage();
        return 0;
    };
    // Opens the stop's cargo editor for the selected stop.
    auto editStop = [&]() {
        if (g_6d3b4c) return locked();
        if (!HasSelectedTrain()) return 0;
        if (F<int32_t>(g_world, 0x4a9f)) return Message496(0x251, 1), 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        if (Special496(Train496())) return Message496(0x255, 1), 0;
        const int32_t row = F<int32_t>(StopList496(w), 0x1d6);
        if (row >= 0 && Train496()) {
            if (IsMultiplayer()) {
                uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x25);
                uint8_t* copy = mem ? (uint8_t*)Cursor33_Init(mem) : nullptr;
                g_6d3b10 = copy;
                OrderList_Init(copy);
                TrainOrders_Copy((uint8_t*)Identity(Train496()), g_6d3b10);
                uint8_t* stop = (uint8_t*)Array33_At(g_6d3b10, 0, row);
                IndustryEditor_Run((uint32_t)(uintptr_t)g_6d3b10, stop, (uint32_t)row);
                for (int32_t i = F<int32_t>(g_6d3b10, 4) - 1; i >= 0; --i)
                    TrainOrders_Remove(g_6d3b10, 0, i);
                ((void(__cdecl*)(void*))0x53b080)(g_6d3b10);
            } else {
                uint8_t* stop = (uint8_t*)Array33_At(Train496(), 0, row);
                IndustryEditor_Run((uint32_t)(uintptr_t)Identity(Train496()), stop, (uint32_t)row);
            }
        }
        if (hook::As<int32_t>(g_trainWindow)) TrainWindow_Refresh(w);
        return 0;
    };
    const int32_t id = (int32_t)msg[1];
    switch (msg[0]) {
    case 0: {
        if (world) F<int32_t>(world, 0x4cb6) = SelectedTrainId();
        if (ClockMs() - g_6d3b2c > 0x96) {
            g_6d3b2c = ClockMs();
            if (uint8_t* h = F<uint8_t*>(g_windowManager, 0xc6d)) {
                const uint16_t hid = F<uint16_t>(h, 4);
                if (hid == 0x65bf || hid == 0x65b6 || hid == 0x65b9 || hid == 0x65bc) TrainDetail_TooltipCb(hid, 0, 0, 0);
            }
        }
        TrainPanel_RefreshTooltip();
        if (!Collection_IsValid(g_trains, 0, (uint32_t)SelectedTrainId()))
            View_SetToolMode(g_gameWindow, 0, 0, -1);
        return 0;
    }
    case 0xca:
        if (id == 0x659c) {
            uint8_t* t = Train496();
            const uint8_t flags = F<uint8_t>(g_input, 0xa8c);
            if (flags & 3) {
                if (Obj_City41(t))
                    World_FollowEntity(g_world, 0, 1, 1, F<int32_t>((uint8_t*)Obj_City41(t), 0x34a));
                return 0;
            }
            if (flags & 4) {
                Train_PromptName(t);
                return 0;
            }
        } else if (id > 0x65a2 && id <= 0x65a4) {
            Window_UpdateListButtons(w);
            return editStop();
        }
        if (id >= 0x65c2 && id <= 0x65d4) {
            if (!HasSelectedTrain()) return 0;
            return Train_ReportCb(w, id, id - 0x3296, F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
        }
        {
            const int32_t v = g_6d3b24 ? id : id - 0x64;
            if (v < 0x6608 || v > 0x660c) return 1;
            if (!HasSelectedTrain()) return 0;
            if (!SelectedTrainIsOwn()) return notOwn();
            const int32_t idx = F<int32_t>(StopList496(w), 0x1e2) + v - 0x6608;
            Ui_PlaySound(0x21, 0, 0x3f000000);
            if (!Train496()) return 0;
            if (!IsMultiplayer()) {
                TrainOrders_SetCurrent(Train496(), 0, idx, 1);
                return 0;
            }
            if (g_6d3ae4) return 0;
            SendHost496(0x3d, (uint32_t)idx);
            return 0;
        }
    case 0xcb:
        break;
    case 0x3e9:
        if (msg[2] == F<uint32_t>(g_gameWindow, 0x90) &&
            Collection_IsValid(g_trains, 0, F<uint32_t>(g_gameWindow, 0x90))) {
            View_SetToolMode(g_gameWindow, 0, 2, 0);
            Game_PostInputEvent(0x3e8, 0, 0, 0, 0);
        }
        return 1;
    default:
        return 1;
    }
    if (id >= 0x65c2 && id <= 0x65d4) {
        if (!HasSelectedTrain()) return 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        return TrainRunMode_StepCb(w, id, id - 0x3296, F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
    }
    auto camera = [&](uint32_t type, uint32_t a, uint32_t b, uint32_t c) {
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, type, 0x659d, a, b, c);
        return 0;
    };
    switch (id) {
    case 0x6947:
    case 0x694b: return camera(0xbc, 0, 1, 0);
    case 0x6948:
    case 0x694c: return camera(0xbc, 0, 0xffffffff, 0);
    case 0x6949:
    case 0x694d: return camera(0xbc, 1, 0, 0);
    case 0x694a:
    case 0x694e: return camera(0xbc, 0xffffffff, 0, 0);
    case 0x659e: return camera(0xad, 0x102e, 0, 0);
    case 0x659f: return camera(0xad, 0xfffff490, 0, 0);
    case 0x65a2: return editStop();
    case 0x65a0: {
        if (g_6d3b4c) return locked();
        if (!HasSelectedTrain()) return 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        if (Special496(Train496())) return Message496(0x255, 1), 0;
        const bool insert = (F<uint8_t>(g_input, 0xa8c) & 3) != 0;
        const int32_t before = SelectedTrainId();
        const int32_t station = StationPick_Run(1);
        if (before != SelectedTrainId() && Collection_IsValid(g_trains, 0, (uint32_t)before)) return 0;
        if (station <= -1) return 0;
        if (Train496()) {
            const uint32_t node = F<uint32_t>(Collection_Get(g_stations, 0, (uint32_t)station), 8);
            const uint32_t at = F<uint32_t>(Train496(), 0x49);
            if (Collection_IsValid(g_tracks, 0, node) && Collection_IsValid(g_tracks, 0, at) &&
                !Node_SameComponent((uint8_t*)Collection_Get(g_tracks, 0, node), 0, at)) {
                Message496(0xc12, 1);
                return 0;
            }
        }
        uint8_t stop[0x33] = {};
        F<uint32_t>(stop, 0x24) = 0x384;
        stop[0x23] = 4;
        stop[0x22] = 0;
        stop[0x28] &= 0x7f;
        F<uint16_t>(stop, 0x29) = (uint16_t)station;
        F<uint32_t>(stop, 0x2f) = 0x3e395810;
        if (IsMultiplayer() && !g_6d3ae4) {
            uint32_t* req = ((uint32_t*(__cdecl*)(size_t))0x5a125d)(0x3b);
            req[0] = (uint32_t)Global6d40e4();
            req[1] = (uint32_t)SelectedTrainId();
            memcpy(req + 2, stop, 0x33);
            Net_SendToHost(g_session, 0, 0x41, 0x3b, (const uint8_t*)req);
            ((void(__cdecl*)(void*))0x5a1145)(req);
            return 0;
        }
        int32_t idx = F<int32_t>(Identity(Train496()), 4);
        if (insert) idx = F<int32_t>(StopList496(w), 0x1d6);
        if (idx < 0) idx = 0;
        TrainOrders_Insert(Train496(), 0, idx, stop);
        if (Train_CanRun(Train496())) return 0;
        if (Obj_CityKindIs2(Train496()) && Train_CanRunNonElectric(Train496())) {
            Train_SetState(Train496(), 0, 0x13, 0.0f);
            return 0;
        }
        Train_SetState(Train496(), 0, 0xa, 0.0f);
        return 0;
    }
    case 0x65a1: {
        if (g_6d3b4c) return locked();
        if (!HasSelectedTrain()) return 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        if (Special496(Train496())) return Message496(0x255, 1), 0;
        const int32_t row = F<int32_t>(StopList496(w), 0x1d6);
        if (row < 0 || !Train496()) return 0;
        if (IsMultiplayer() && !g_6d3ae4) {
            SendHost496(0x3f, (uint32_t)row);
            return 0;
        }
        TrainOrders_Remove(Train496(), 0, row);
        return 0;
    }
    case 0x65a5:
    case 0x65a6: {
        const bool second = id == 0x65a5;
        g_6d3b24 = second ? 1 : 0;
        Window_SendEvent5(w, 0, 0x6d, second ? 0x6591 : 0x6592, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, second ? 0x6592 : 0x6591, 3, 0, 0);
        const uint32_t shown = second ? 0x65a3 : 0x65a4;
        Window_SendEvent5(w, 0, 0x66, shown, F<uint32_t>(Widget_FindChild(w, 0, second ? 0x65a4 : 0x65a3), 0x1d6), 0, 0);
        Window_SendEvent5(w, 0, 0x95, shown, 0, 0, 0);
        return 0;
    }
    case 0x65a7:
    case 0x65a8:
        Window_SetLargeLayout(w, 0, id == 0x65a7);
        return 0;
    case 0x65a9: {
        if (g_6d3b4c) return locked();
        if (!HasSelectedTrain()) return 0;
        if (F<int32_t>(g_world, 0x4a9b)) return Message496(0x251, 1), 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        if (Train_StateSpecial(Train496())) return Message496(0x253, 1), 0;
        const int32_t before = SelectedTrainId();
        const uint32_t answer = Message496(0x254, 2);
        if (before != SelectedTrainId() && Collection_IsValid(g_trains, 0, (uint32_t)before)) return 0;
        if (answer != 0x3f2) return 0;
        const uint32_t tid = F<uint32_t>(g_gameWindow, 0x90);
        if (!Collection_IsValid(g_trains, 0, tid)) return 0;
        if (!IsMultiplayer()) {
            Collection* trains = g_trains;
            ((void(__fastcall**)(Collection*, int, uint32_t))*(void***)trains)[2](trains, 0, tid);
            View_SetToolMode(g_gameWindow, 0, 2, 0);
            Game_PostInputEvent(0x3e8, 0, 0, 0, 0);
            return 0;
        }
        if (g_6d3ae4) return 0;
        uint32_t req[2];
        req[0] = (uint32_t)Obj_Get25(g_world);
        Cursor_Set(g_cursorManager, 0, 6, 0, 0);
        req[1] = tid;
        Net_SendToHost(g_session, 0, 0x37, 8, (const uint8_t*)req);
        return 0;
    }
    case 0x65aa: {
        if (g_6d3b4c) return locked();
        if (!HasSelectedTrain()) return 0;
        if (F<int32_t>(g_world, 0x4a97)) return Message496(0x251, 1), 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        if (Special496(Train496())) {
            if (Train496()[0x1ce] & 2) return Message496(0xf39, 1), 0;
            Train_OpenConsistEditor(g_world, 0, F<uint32_t>(Train496(), 0x25), nullptr);
            return 0;
        }
        if (Train_StateSpecial(Train496())) return Message496(0x252, 1), 0;
        ConsistEditor_Run(Train496(), nullptr, nullptr, 0xffffffff, 0xffffffff, 0, 0, 0, 0xffffffff);
        return 0;
    }
    case 0x65ad:
    case 0x65ae:
        if (g_6d3b4c) return locked();
        View_SetToolMode(g_gameWindow, 0, 0, -1);
        return 0;
    case 0x65af: {
        if (!HasSelectedTrain()) return 0;
        uint8_t* t = Train496();
        if (!Obj_City41(t)) return 1;
        Camera_FollowObject((uint8_t*)Obj_City41(t), 0);
        return 0;
    }
    case 0x65b0: {
        if (!HasSelectedTrain()) return 0;
        uint8_t* t = Train496();
        if (!Obj_City41(t)) return 1;
        uint8_t* wd = g_world;
        if (F<int32_t>(wd, 0x45) == 1 && F<int32_t>(wd, 0x49) == F<int32_t>((uint8_t*)Obj_City41(t), 0x34a)) {
            World_SetFollow(g_world, 0, 0, nullptr);
            return 0;
        }
        World_FollowEntity(g_world, 0, 1, 1, F<int32_t>((uint8_t*)Obj_City41(t), 0x34a));
        return 0;
    }
    case 0x65be:
        if (g_6d3b4c) return locked();
        if (F<int32_t>(g_world, 0x4aa3)) return Message496(0x251, 1), 0;
        if (!HasSelectedTrain()) return 0;
        if (!SelectedTrainIsOwn()) return notOwn();
        ConsistEditor_Run(nullptr, Train496(), nullptr, 0xffffffff, 0xffffffff, 0, 0, 0, 0xffffffff);
        return 0;
    case 0x65c0:
    case 0x65c1:
        Vehicles_SelectNext(id == 0x65c0 ? -1 : 1);
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x5148a0, TrainWindow_HandleEvent);

// 0x515780 (callback). Unless w is the active page button (0x32ca with
// 0x6d3b30, else 0x32c9), clears the window's children 0x332c..0x36b0.
void __fastcall Page_ClearChildrenCb(uint8_t* w, int, int32_t) {
    const uint16_t page = g_6d3b30 ? 0x32ca : 0x32c9;
    if (F<uint16_t>(w, 4) != page) Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0x332c, 0x36b0, 0);
}
RT3_REPLACE(0x515780, Page_ClearChildrenCb);

void __fastcall Widget5157c0_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d19d0;
    hook::As<int32_t>(g_6d3b34) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x5157c0, Widget5157c0_Destruct);

RT3_NOINLINE int32_t Global6d3b30() { return g_6d3b30; }
RT3_REPLACE(0x5157e0, Global6d3b30);

// 0x5157f0 (draw callback). Draws the train row of train `id` (EDX)
// unhighlighted.
void __fastcall TrainList_DrawRowCb(void*, uint32_t id, float x, float y, float w, float h, int32_t) {
    TrainList_DrawEntry((uint8_t*)Collection_Get(g_trains, 0, id), 0, x, y, w, h);
}
RT3_REPLACE(0x5157f0, TrainList_DrawRowCb);

// 0x515820 (list callback). Selects train `id` (EDX) in the panel; with
// input flags 3 the world follows its city object, with flag 4 a named
// train refreshes the picker (0x6d3b34).
void __fastcall TrainList_SelectCb(void*, uint32_t id) {
    if (!Collection_IsValid(g_trains, 0, id)) return;
    Panel_SetValue90(g_gameWindow, 0, (int32_t)id);
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
    const uint32_t flags = F<uint32_t>(g_input, 0xa8c);
    if (flags & 3) {
        if (Obj_City41(t)) World_FollowEntity(g_world, 0, 1, 1, F<int32_t>((uint8_t*)Obj_City41(t), 0x34a));
        return;
    }
    if ((flags & 4) && Train_PromptName(t) && g_6d3b34) TrainPicker_Fill(g_6d3b34, 0, 0xffffffff);
}
RT3_REPLACE(0x515820, TrainList_SelectCb);

// 0x5158b0 (callback). Unless w is the active page button, builds the train
// row of the first argument in slot d (EDX) - +0x1e2.
void __fastcall Page_BuildTrainRowCb(uint8_t* w, int32_t d, uint32_t train, int32_t, int32_t, int32_t, int32_t) {
    const uint16_t page = g_6d3b30 ? 0x32ca : 0x32c9;
    if (F<uint16_t>(w, 4) != page) TrainRow_Build(F<uint8_t*>(w, 0x86), train, 0, d - F<int32_t>(w, 0x1e2), 0);
}
RT3_REPLACE(0x5158b0, Page_BuildTrainRowCb);

// 0x5158f0. Fills the train picker's lists 0x32c9 / 0x32ca with the player's
// trains (0x6cfcbc, owner +0x51; data its id +0x25, text string 0xf2e) except
// `exclude` (sic: compared against list-0 order while the train comes from list 1),
// callbacks 0x5157f0 / 0x5158b0 / 0x515780 / 0x515820, keeping +0x78; labels
// 0x32cf / 0x32d0 show string 0xde without trains; scrolls to 0x622b2c (unless -1)
// and selects the world's current train +0x4cb6 when it is the player's.
RT3_NOINLINE void __fastcall TrainPicker_Fill(uint8_t* w, int, uint32_t exclude) {
    int32_t keep = F<int32_t>(w, 0x78);
    Window_SendEvent5(w, 0, 0x8b, 0x32c9, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x32ca, 0, 0, 0);
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* c = g_trains;
        if (Collection_IdAt(c, 0, i, 0) == exclude) continue;
        c = g_trains;
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 1));
        if (F<int32_t>(t, 0x51) != Obj_Get21(g_world)) continue;
        char text[0x1e0];
        FormatText(text, GetString(0xf2e), 0, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x88, 0x32c9, 0, F<uint32_t>(t, 0x25), (uint32_t)(uintptr_t)text);
        Window_SendEvent5(w, 0, 0x88, 0x32ca, 0, F<uint32_t>(t, 0x25), (uint32_t)(uintptr_t)text);
    }
    Window_SendEvent5(w, 0, 0x7a, 0x32c9, 0x5157f0, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x32ca, 0x5157f0, 0, 0);
    Window_SendEvent5(w, 0, 0x7e, 0x32c9, 0x5158b0, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x32c9, 0x515780, 0, 0);
    Window_SendEvent5(w, 0, 0x7e, 0x32ca, 0x5158b0, 0, 0);
    Window_SendEvent5(w, 0, 0x7f, 0x32ca, 0x515780, 0, 0);
    Window_SendEvent5(w, 0, 0x95, 0x32c9, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x95, 0x32ca, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x32c9, 0x515820, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x32ca, 0x515820, 0, 0);
    F<int32_t>(w, 0x78) = keep;
    if (Collection_Count(g_trains) >= 1) {
        Window_SendEvent5(w, 0, 0x66, 0x32c9, F<uint32_t>(w, 0x78), 0, 0);
        Window_SendEvent5(w, 0, 0x66, 0x32ca, F<uint32_t>(w, 0x78), 0, 0);
    }
    char none[0x130];
    if (Collection_Count(g_trains) > 0) none[0] = 0;
    else strcpy(none, GetString(0xde));
    Window_SendEvent5(w, 0, 0x6f, 0x32cf, (uint32_t)(uintptr_t)none, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x32d0, (uint32_t)(uintptr_t)none, 0, 0);
    if (g_622b2c != -1) {
        Window_SendEvent5(w, 0, 0x6b, 0x32ca, hook::As<uint32_t>(g_622b2c), 0, 0);
        Window_SendEvent5(w, 0, 0x6b, 0x32c9, hook::As<uint32_t>(g_622b2c), 0, 0);
    }
    uint32_t cur = F<uint32_t>(g_world, 0x4cb6);
    if (!Collection_IsValid(g_trains, 0, cur)) return;
    int32_t owner = F<int32_t>(Collection_Get(g_trains, 0, F<uint32_t>(g_world, 0x4cb6)), 0x51);
    if (owner != Obj_Get21(g_world)) return;
    Window_SendEvent5(w, 0, 0x69, 0x32ca, F<uint32_t>(g_world, 0x4cb6), 0, 0);
    Window_SendEvent5(w, 0, 0x69, 0x32c9, F<uint32_t>(g_world, 0x4cb6), 0, 0);
}
RT3_REPLACE(0x5158f0, TrainPicker_Fill);

// 0x515c60 (event handler). The train list window w (lists 0x32c9 / 0x32ca
// by 0x6d3b30, row buttons 0x332c..0x36b0 in columns of 18): idle refreshes
// the hover tooltip and, every 20 ms (0x6d3b38), publishes the shown
// list's top row (0x622b2c); clicks on a row's buttons run the report /
// run mode callbacks with the row's train (+0x4f of the widget); a train
// row selects it for the world (+0x4cb6) and a double click follows its
// lead car (camera mode from the options +0x311) in the train view; the
// tabs 0x32cd / 0x32ce switch lists, dropping the rows; 0x3e9 refills the
// train picker.
int32_t __fastcall TrainList_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    const int32_t id = (int32_t)msg[1];
    auto column = [&]() { return (id - 0x332c) % 0x12 + 0x332c; };
    switch (msg[0]) {
    case 0:
        TrainPanel_RefreshTooltip();
        if (ClockMs() - g_6d3b38 > 0x14) {
            g_6d3b38 = ClockMs();
            if (uint8_t* l = Widget_FindChild(w, 0, g_6d3b30 ? 0x32c9 : 0x32ca))
                g_622b2c = F<int32_t>(l, 0x1e2);
            else
                g_622b2c = -1;
        }
        return 1;
    case 0xca:
        if (id >= 0x332c && id <= 0x36b0) return Train_ReportCb(w, id, column(), F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
        if (id >= 0x32c9 && id <= 0x32ca) {
            uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, msg[3]);
            if (!t || !Obj_City41(t)) return 1;
            F<uint32_t>(g_world, 0x4cb6) = F<uint32_t>(t, 0x25);
            Camera_FollowObject((uint8_t*)Obj_City41(t), F<int32_t>(g_app, 0x311));
            View_SetToolMode(g_gameWindow, 0, 2, (int32_t)msg[3]);
            return 0;
        }
        return 1;
    case 0xcb: {
        if (id >= 0x332c && id <= 0x36b0) return TrainRunMode_StepCb(w, id, column(), F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
        if (id != 0x32cd && id != 0x32ce) return 1;
        const bool second = id == 0x32ce;
        g_6d3b30 = second ? 1 : 0;
        const uint32_t list = second ? 0x32c9 : 0x32ca;
        Window_SendEvent5(w, 0, 0x66, list, F<uint32_t>(w, 0x78), 0, 0);
        Window_SendEvent5(w, 0, 0x6d, second ? 0x32cc : 0x32cb, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, second ? 0x32cb : 0x32cc, 3, 0, 0);
        Window_DeleteChildren(w, 0, 0x332c, 0x36b0, 0);
        Window_SendEvent5(w, 0, 0x95, list, 0, 0, 0);
        return 0;
    }
    case 0xce:
        if (id < 0x32c9 || id > 0x32ca) return 1;
        F<uint32_t>(g_world, 0x4cb6) = msg[3];
        return 0;
    case 0x3e9:
        TrainPicker_Fill(w, 0, msg[2]);
        return 1;
    default:
        return 1;
    }
}
RT3_REPLACE(0x515c60, TrainList_HandleEvent);

// 0x515f50. Train list dialog constructor ("TrainList.win", vtable 0x5d19d0, kept
// in 0x6d3b34).
uint8_t* __fastcall TrainListDialog_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d19d0;
    g_6d3b34 = w;
    F<int32_t>(w, 0x78) = -1;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d19dc);
    PickerDialog_Setup(w, 0x32c9, 0x32ca, 0x32cb, 0x32cc, g_6d3b30);
    TrainPicker_Fill(w, 0, 0xffffffff);
    return w;
}
RT3_REPLACE(0x515f50, TrainListDialog_Construct);

}  // namespace rt3
