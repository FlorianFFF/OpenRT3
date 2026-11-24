// StationWindow.cpp - RT3_mp.exe 0x504000-0x50a000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"
#include "core/crt.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void ResetPair5ee4fc();
void __fastcall Map_ClearFogMode(uint8_t* m);
int32_t __fastcall Game_EffectiveVersion();
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_BuildingValue(void* o);
void* __fastcall Obj_Ref3cc(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
void* __fastcall Obj_Building(void* o);
float __fastcall Building_RoundedValue(uint32_t id, int, float a);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void __fastcall BuildingType_Model2D(uint8_t* o, int, char* out);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetVtbl(uint8_t* w);
uint32_t IsEnglish();
void StationDialog_UpdateButtons();
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v);
void __fastcall SceneObj_ResetAttachmentLists(uint8_t* o);
int32_t __fastcall Obj_Int564(void* o, int, int32_t i);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Collection_Count(Collection* c);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t Global6d40e4();

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Table_RefreshAll(Collection* t);
int32_t ClockMs();
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

int32_t __fastcall Cities_AssignBlocks(Collection* cities);
void __stdcall SetGlobals5ee4fc(int32_t a, int32_t b);
void __fastcall Map_SetFogMode(uint8_t* m, int, int32_t mode);
void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
void __fastcall Overview_SetCargo(uint8_t* t, int, int32_t cargo);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
void __fastcall BuildingDialog_ShowTabs(uint8_t* w);
void __fastcall BuildingDialog_Select(uint8_t* w, int, int32_t id);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall Map_TerrainKindAt(void* m, int, float x, float y);
uint8_t* __fastcall StationPick_Construct(uint8_t* w);
void __fastcall Tool_Leave(uint8_t* t);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall ListView_RefreshVisible(uint8_t* tv);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
int32_t __fastcall Obj_TypeAvailable(void* o);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void* __fastcall Obj_Company21(void* o);

int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle, float* outX, float* outY,
                                       float* outAngle, int32_t update, int32_t* outNode);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name);
uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
void* __fastcall Obj_Ref2a8(void* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
uint32_t __fastcall Obj_SetWorld66ba(void* o, int, int32_t v);
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId);
void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
int32_t __fastcall IsSpecialCommand_Impl(int32_t v);
uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
uint32_t __fastcall SceneryEffects_Add(Collection* t, int, float x, float z, uint32_t kind);
uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
void RunDebugValidation();
void __fastcall Placement_SetRotation(uint8_t* w, int, uint32_t angle, int32_t keep);
void __fastcall BuildingDialog_ShowPrice(uint8_t* w);
void __fastcall SetSeed(int32_t seed);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall MpSetupDialog_Show(uint8_t* w);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall NewGameConfig_Reset(uint8_t* c);
void __fastcall MpNewGameDialog_Show(uint8_t* w);
void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight);
void __fastcall Cursor_PopBusy(uint8_t* c);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall Glyph_QueueRotated(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, float angle, float px, float py);
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall NewGameConfig_Reset(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
uint8_t* __fastcall SetupScreen_Construct(uint8_t* s);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
int32_t __fastcall Models_DisplayName(const char* name, char* out);
float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls);
void Buildings_SelectNextFwd();
void Buildings_SelectNextBack();
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall DateToMinutes(const GameDate* d);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
void PendingRows_Clear();
void __stdcall StationDetail_DrawRows(float x, float y, float w, int32_t);
void __fastcall StationDetail_AddKindButtons(void*, int, float x, float y, float, float);
double __fastcall Obj_Float514(void* o, int, int32_t i);
double __fastcall Obj_Float53c(void* o, int, int32_t i);
void __fastcall StationView_DrawCargoGauge(int32_t which, int, float x, float y, float w, float h, float amount);
void __fastcall StationView_DrawCargoGrid(void*, int, float x, float y, float, float);
void __fastcall StationDetail_DrawSummary(uint8_t* st, int, float x, float y, float w, int32_t);
void __fastcall SetupScreen_SetMode(uint8_t* s, int, int32_t mode);
int32_t __fastcall BuildingDialog_TrackMouse(uint8_t* dlg, int, uint32_t* ev);
void __fastcall BuildingDialog_FindSpot(uint8_t* dlg, int, uint32_t id, float* x, float* y, float* angle);
int32_t __fastcall BuildingDialog_CheckSpot(uint8_t* dlg, int, float x, float y, char* msg);
int32_t __fastcall Obj_Train2a8(uint8_t* o);
void __fastcall Placement_RotateToMouse(uint8_t* w);
void __fastcall BuildingDialog_SetPage(uint8_t* w, int, int32_t page);
void __fastcall BuildingDialog_UpdateTerrain(uint8_t* w, int, int32_t force);
void* __fastcall Obj_TrainOwner(uint8_t* o);
void __fastcall StationWindow_PickCargo(uint8_t* w, int, int32_t cargo);
void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value);
uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
int32_t View_FocusValid();
int32_t View_FocusIsOwnStation();
void Station_ShowOwnerMessage();
uint32_t __fastcall Records_FindComposedName(Collection* c, int, int32_t a, int32_t b);
void* __fastcall Obj_Station173(uint8_t* o);
int32_t __fastcall Building_ChangeType(uint8_t* b, int, uint32_t type, int32_t check, float* cost, char* text);
int32_t __fastcall StationPick_Run(uint8_t lower);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void* __fastcall Obj_Station173(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Building_ChangeType(uint8_t* b, int, uint32_t type, int32_t check, float* cost, char* text);
RT3_NOINLINE void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
RT3_NOINLINE int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE uint32_t __fastcall Records_FindComposedName(Collection* c, int, int32_t a, int32_t b);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
RT3_NOINLINE void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
RT3_NOINLINE void __fastcall Overview_SetCargo(uint8_t* t, int, int32_t cargo);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void ModelPreview_Close();
RT3_NOINLINE float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls);
RT3_NOINLINE void* __fastcall Obj_TrainOwner(uint8_t* o);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value);
RT3_NOINLINE void __fastcall SetupScreen_SetMode(uint8_t* s, int, int32_t mode);
uint8_t* __fastcall SetupScreen_Construct(uint8_t* s);
RT3_NOINLINE void PendingRows_Clear();
RT3_NOINLINE int32_t View_FocusValid();
RT3_NOINLINE int32_t View_FocusIsOwnStation();
RT3_NOINLINE void Station_ShowOwnerMessage();
RT3_NOINLINE void StationDialog_UpdateButtons();
RT3_NOINLINE void __stdcall StationDetail_DrawRows(float x, float y, float w, int32_t);
void __fastcall StationView_CargoTooltip(int32_t id, int, int32_t, int32_t);
RT3_NOINLINE void __fastcall Tool_Leave(uint8_t* t);
RT3_NOINLINE void __fastcall StationWindow_PickCargo(uint8_t* w, int, int32_t cargo);
void __fastcall StationWindow_CargoHover(int32_t id, int, int32_t entered, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall StationDetail_DrawSummary(uint8_t* st, int, float x, float y, float w, int32_t);
RT3_NOINLINE void __fastcall StationView_DrawCargoGauge(int32_t which, int, float x, float y, float w, float h, float amount);
RT3_NOINLINE void __fastcall StationDetail_AddKindButtons(void*, int, float x, float y, float, float);
RT3_NOINLINE void __fastcall StationView_DrawCargoGrid(void*, int, float x, float y, float, float);
void __fastcall StationDetail_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Buildings_SelectNext(int32_t dir);
RT3_NOINLINE void Buildings_SelectNextFwd();
RT3_NOINLINE void Buildings_SelectNextBack();
int32_t __fastcall StationWindow_HandleEvent(uint8_t* w, int, uint32_t* msg);
void __fastcall StationDetail_DrawCb(int32_t id, int, float x, float y, float w, float h, int32_t);
uint8_t* __fastcall StationDetail_Construct(uint8_t* w);
void __fastcall StationList_Destruct(uint8_t* w);
void __fastcall Station_DrawSummary(int32_t, uint32_t id, float x, float y, int32_t, float h, int32_t);
void __fastcall StationList_SelectCb(void*, uint32_t id);
void __stdcall Station_SupplyTooltipCb(uint32_t id, int32_t, int32_t);
void __fastcall StationPicker_Fill(uint8_t* w);
int32_t __fastcall StationList_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall StationListDialog_Construct(uint8_t* w);
void __fastcall Nop5075b0(void*);
int32_t __stdcall Dialog_Filter80e8(uint32_t* ev);
uint8_t* __fastcall StationPick_Construct(uint8_t* w);
RT3_NOINLINE int32_t __fastcall StationPick_Run(uint8_t lower);
int32_t __fastcall IsSpecialCommand_Impl(int32_t v);
int32_t IsSpecialCommand();
RT3_NOINLINE void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
void __fastcall Panel_SetDragging(uint8_t* w, int, int32_t on);
void __fastcall Placement_DrawCompassCb(void*, int, float x, float y, int32_t, int32_t, int32_t);
void __fastcall InfoWindow_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall BuildingDialog_ShowTabs(uint8_t* w);
void __fastcall Placement_SetRotation(uint8_t* w, int, uint32_t angle, int32_t keep);
RT3_NOINLINE void __fastcall Placement_RotateToMouse(uint8_t* w);
RT3_NOINLINE int32_t __fastcall BuildingDialog_CheckSpot(uint8_t* dlg, int, float x, float y, char* msg);
RT3_NOINLINE void __fastcall BuildingDialog_FindSpot(uint8_t* dlg, int, uint32_t id, float* x, float* y, float* angle);
RT3_NOINLINE float __fastcall Building_RoundedValue(uint32_t id, int, float a);
void __fastcall BuildingDialog_ShowPrice(uint8_t* w);
RT3_NOINLINE void __fastcall Placement_RotateSteps(void*, int, int32_t steps);
RT3_NOINLINE int32_t __fastcall Obj_Train2a8(uint8_t* o);
RT3_NOINLINE void __fastcall BuildingDialog_Select(uint8_t* w, int, int32_t id);
void __fastcall BuildingDialog_SelectCb(int32_t v, int32_t id);
RT3_NOINLINE void __fastcall BuildingDialog_SetPage(uint8_t* w, int, int32_t page);
RT3_NOINLINE void __fastcall BuildingDialog_UpdateTerrain(uint8_t* w, int, int32_t force);
RT3_NOINLINE int32_t __fastcall BuildingDialog_TrackMouse(uint8_t* dlg, int, uint32_t* ev);
int32_t __fastcall BuildingDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall BuildingDialog_Construct(uint8_t* w);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);

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

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline double Atan(double v) { return ((double(__cdecl*)(double))0x5a1270)(v); }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

static inline double CrtFmod(double a, double b) { return ((double(__cdecl*)(double, double))0x5a1390)(a, b); }

// Company finance statistics. Ids below 0x2b are kept per year: +0x1c47 + id * 8
// (this year so far) and +0xd7f + (id * 11 + n) * 8 (n years ago, 0 = live); larger
// ids are sums / ratios of those. Years: 0x2329 = live value, 0x232a = this year so
// far. World 0x6cec78: year +0xd, month +0xf, day +0x11, tick +0x15, difficulty +0x2d.
static inline uint8_t* WorldG() { return hook::Global<uint8_t*>(0x6cec78); }

// Two-list picker dialogs (left / right lists share the selection +0x78; the
// swap buttons are enabled by the side flag).
static void PickerDialog_Setup(uint8_t* w, uint32_t listA, uint32_t listB, uint32_t btnA, uint32_t btnB, int32_t right) {
    Window_SendEvent5(w, 0, 0xaa, listA, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, 0xaa, listB, (uint32_t)(uintptr_t)(w + 0x78), 0, 0);
    Window_SendEvent5(w, 0, right ? 0x86 : 0x6d, btnA, 3, 0, 0);
    Window_SendEvent5(w, 0, right ? 0x6d : 0x86, btnB, 3, 0, 0);
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

namespace {
void Free447(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int Stricmp447(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
uint32_t Message447(const char* text, int32_t type, uint32_t buttonText, uint32_t a8) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, buttonText, a8, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// Logs the display name of key name string `key` (message `msg`).
void Log_KeyName(int32_t key, int32_t msg) {
    char name[0x190], shown[0x190], text[0x190];
    FormatText(name, GetString(key), 0, 0, 0, 0, 0, 0, 0, 0);
    if (!Models_DisplayName(name, shown)) return;
    FormatText(text, GetString(msg), shown, 0, 0, 0, 0, 0, 0, 0);
    Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 4, 0, 0);
}
// The session's minute follows the next queued part (0 without one).
void Session_StampNextPart() {
    uint8_t* part = Net_CountMessagePart(hook::Global<uint8_t*>(0x6cd8d8), 0, 1);
    uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
}
}

namespace {
uint32_t Message477(const char* text, int32_t type, uint32_t buttonText, uint32_t a8) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, buttonText, a8, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// The building type of the dialog's building (vtable +0x80).
uint8_t* Type477(uint8_t* d) {
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c));
    return ((uint8_t*(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x20](b);
}
// Renames the city by id from collection `table` (string `text`, the name at
// +nameOff), counted as a modal dialog.
void Rename477(uint8_t* d, int32_t slot, uint32_t text, Collection* table, int32_t nameOff) {
    ++hook::Global<int32_t>(0x6d171c);
    const uint32_t id = F<uint32_t>(d, slot);
    F<int32_t>(d, slot) = -1;
    uint8_t* name = (uint8_t*)Collection_Get(table, 0, id) + nameOff;
    Message477(GetString(text), 9, (uint32_t)(uintptr_t)name, 0x28);
}
}

namespace {
uint32_t Message492(const char* text, int32_t type, uint32_t px) {
    return Dialog_RunMessage(text, type, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
uint8_t* Building492(uint8_t* st) { return (uint8_t*)Obj_Building(st); }
uint8_t* Type492(uint8_t* b) { return ((uint8_t*(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x20](b); }
}

// 0x504010. Constructs the setup screen s (vtable 0x5d1664, Setup.win;
// 0x6d14c0, the scenario list 0x6d14b8 cleared). Reseeds the sim random
// from Rand(100, 500000) + timeGetTime, acquires the new-game button images
// (0x6d14b0) and font 3 (0x6d14b4), a 0x403c2-byte buffer (0x6d14bc), resets
// the new-game config (lobby +0x7d = 1), lays out the multiplayer and
// new-game panels and opens page 1, then wires the callbacks of 0xce8 /
// 0xe86 / 0xe87. A long player name (0x622a28, over 10) sets 0x6d14a4 = 2;
// a pending high-score request (0x6d4000) shows the table. Outside English the
// buttons 0xc80..0xc86, 0xf6f..0xf71 and 0xe88 get styles (0xe88 shifted
// up, font scale 0.0583), 0xddf too in mode 0x6d3c84 == 2, and 0xd4c is
// hidden otherwise. With Setup_Coast_To_Coast.imb present a picture widget
// 0xd5e is added before 0xbd0. Returns s.
uint8_t* __fastcall SetupScreen_Construct(uint8_t* s) {
    Widget_SetVtblRet(s);
    F<uint32_t>(s, 0) = 0x5d1664;
    F<int32_t>(s, 0x7b) = 0;
    hook::Global<uint8_t*>(0x6d14b8) = nullptr;
    hook::Global<uint8_t*>(0x6d14c0) = s;
    Window_Load(s, 0, 0.0f, 0.0f, (const char*)0x5d16d4);
    int32_t r = Rand(0x64, 0x7a120);
    SetSeed(r + (int32_t)((uint32_t(__stdcall*)())hook::Global<void*>(0x5c8068))());
    hook::Global<uint8_t*>(0x6d14b0) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d1210);
    hook::Global<uint8_t*>(0x6d14b4) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    hook::Global<void*>(0x6d14bc) = Malloc(0x403c2);
    NewGameConfig_Reset(hook::Global<uint8_t*>(0x6cec7c));
    hook::Global<uint8_t*>(0x6cec7c)[0x7d] = 1;
    MpSetupDialog_Show(s);
    MpNewGameDialog_Show(s);
    SetupScreen_SetMode(s, 0, 1);
    Window_SendEvent5(s, 0, 0x7a, 0xce8, 0x502030, 0, 0);
    Window_SendEvent5(s, 0, 0x79, 0xe86, 0x502160, 0, 0);
    Window_SendEvent5(s, 0, 0x79, 0xe87, 0x5021c0, 0, 0);
    if (strlen((const char*)0x622a28) > 0xa) hook::Global<int32_t>(0x6d14a4) = 2;
    if (hook::Global<int32_t>(0x6d4000)) {
        uint8_t* c = hook::Global<uint8_t*>(0xccbb20);
        *(uint8_t*)0x6d1088 = 1;
        hook::Global<int32_t>(0x6d4000) = 0;
        if (c) Cursor_PopBusy(c);
        HighScores_Show(-1, 0, 1);
    }
    if (!(uint8_t)IsEnglish()) {
        for (int32_t id = 0xc80; id <= 0xc86; ++id) Window_SendEvent5(s, 0, 0x78, (uint32_t)id, 0xa, 0, 0);
        for (int32_t id = 0xf6f; id <= 0xf71; ++id) Window_SendEvent5(s, 0, 0x78, (uint32_t)id, 0xa, 0, 0);
        Window_SendEvent5(s, 0, 0x78, 0xe88, 1, 0, 0);
        uint8_t* b = Widget_FindChild(s, 0, 0xe88);
        float y = F<float>(b, 0xbc) - *(const float*)0x5d16d0;
        F<uint32_t>(b, 0xc4) = 0x3d6eeeef;
        F<float>(b, 0xbc) = y;
        if (hook::Global<int32_t>(0x6d3c84) == 2) Window_SendEvent5(s, 0, 0x78, 0xddf, 0xb, 0, 0);
        if (!(uint8_t)IsEnglish()) Window_SendEvent5(s, 0, 0x78, 0xd4c, 1, 0, 0);
    }
    if (NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d16b4, 0, nullptr)) {
        uint8_t def[0x89] = {};
        F<uint16_t>(def, 0) = 0xd5e;
        F<uint32_t>(def, 7) = 0x3eae147b;
        F<uint32_t>(def, 0xb) = 0x3e147ae1;
        F<uint32_t>(def, 0xf) = 0x3ea3d70a;
        F<uint32_t>(def, 0x13) = 0x3dda740e;
        F<uint16_t>(def, 0x5f) = 0;
        String_Assign(&F<char*>(def, 0x43), (const char*)0x5d16b4);
        uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x111);
        w = w ? Scrollbar_Construct(w, 0, def) : nullptr;
        Window_AddChild(s, 0, w, 0xbd0);
        ((void(__cdecl*)(void*))0x5a1145)(F<char*>(def, 0x43));
    }
    return s;
}
RT3_REPLACE(0x504010, SetupScreen_Construct);

// 0x5042c0. Once the pending flag 0x6d16f0 is taken, clears the generated rows of
// window 0x6d16d8 (ids 0xb400..0xb5a3 in four ranges, 0xb3f6 and 0xb3f7).
RT3_NOINLINE void PendingRows_Clear() {
    if (!hook::Global<int32_t>(0x6d16f0)) return;
    hook::Global<int32_t>(0x6d16f0) = 0;
    if (!hook::Global<uint8_t*>(0x6d16d8)) return;
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb414, 0xb4db, 0);
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb4dc, 0xb5a3, 0);
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb40a, 0xb413, 0);
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb400, 0xb409, 0);
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb3f6, 0xfff0, 0);
    Window_DeleteChildren(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb3f7, 0xfff0, 0);
}
RT3_REPLACE(0x5042c0, PendingRows_Clear);

// 0x504370. The focused station (view +0x90) still exists.
RT3_NOINLINE int32_t View_FocusValid() { return Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)); }
RT3_REPLACE(0x504370, View_FocusValid);

// 0x504390. The focused station (view +0x90) exists and belongs to the
// player's company.
RT3_NOINLINE int32_t View_FocusIsOwnStation() {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return 0;
    uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    if (!s) return 0;
    const int32_t own = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
    return Obj_BuildingValue(s) == own ? 1 : 0;
}
RT3_REPLACE(0x504390, View_FocusIsOwnStation);

// 0x5043f0. Shows the owner of the main view's station (+0x90), string 0xe0b.
RT3_NOINLINE void Station_ShowOwnerMessage() {
    uint8_t* view = hook::Global<uint8_t*>(0x6d0818);
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(view, 0x90))) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    const char* owner = (const char*)0x5c87a8;
    if (st)
        if (uint8_t* o = (uint8_t*)Obj_TrainOwner(st)) owner = (const char*)o + 4;
    char text[0x1f4];
    FormatText(text, GetString(0xe0b), owner, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
RT3_REPLACE(0x5043f0, Station_ShowOwnerMessage);

// 0x5044b0. Enables the station dialog's (0x6d16d8) buttons by the kind of the
// selected station's building (view 0x6d0818 +0x90): 0xb3bc / 0xb3bf for kind 1,
// 0xb3bb / 0xb3bd / 0xb3be for kind 0.
RT3_NOINLINE void StationDialog_UpdateButtons() {
    uint32_t sel = F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90);
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, sel)) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    uint8_t* b = (uint8_t*)Obj_Building(st);
    uint8_t kind = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b)[0x8c];
    uint32_t t1 = kind != 1 ? 0x87 : 0x65, t0 = kind != 0 ? 0x87 : 0x65;
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, t1, 0xb3bc, 3, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, t1, 0xb3bf, 3, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, t0, 0xb3bb, 3, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, t0, 0xb3bd, 3, 0, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, t0, 0xb3be, 3, 0, 0);
}
RT3_REPLACE(0x5044b0, StationDialog_UpdateButtons);

// 0x504590. Station detail window: for the selected station (main view +0x90 in
// 0x6cec20) lists every other station (format 0x5d16f0 with its name +0x46b)
// followed by three columns (tables +0x24 / +0x28 / +0x2c, 5-byte entries: a
// float value / 0x5c9f18 in format 0x5d16e8 when its flag byte is set, else
// 0x5d16e0), one row every 0x5d0100 down from y, in font 0x6d16e8.
RT3_NOINLINE void __stdcall StationDetail_DrawRows(float x, float y, float w, int32_t) {
    Collection* stations = hook::Global<Collection*>(0x6cec20);
    if (!Collection_IsValid(stations, 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    float rowY = y;
    float lx = (float)((double)x + D(0x5d0110));
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    char line[0x1ec], cell[0x1f8];
    for (int32_t i = 0; i < F<int32_t>(st, 0x30); ++i) {
        if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)i)) continue;
        if (i == F<int32_t>(st, 0)) continue;
        const char* name = (const char*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)i) + 0x46b;
        FormatText(line, (const char*)0x5d16f0, name, 0, 0, 0, 0, 0, 0, 0);
        for (int k = 0; k < 3; ++k) {
            const uint8_t* e = F<uint8_t*>(st, 0x24 + k * 4) + i * 5;
            if (e[4])
                ExeSprintf()(cell, (const char*)0x5d16e8, (double)F<float>((void*)e, 0) / Fl(0x5c9f18));
            else
                FormatText(cell, (const char*)0x5d16e0, 0, 0, 0, 0, 0, 0, 0, 0);
            strcat(line, cell);
        }
        Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16e8), 0, line, lx, rowY, w, BitsF(0x3ccccccd), 0xffffffff, 2, 0, 0, 1.0f);
        rowY = (float)((double)rowY + Fl(0x5d0100));
    }
}
RT3_REPLACE(0x504590, StationDetail_DrawRows);

// 0x504770. Tooltip for the station view's (0x6d0818 +0x90) two waiting-cargo
// buttons: 0xb3f6 shows the first pair of counters (+0x4c4, plus +0x4b0; values
// +0x514, plus +0x4b8; string 0x2a4), any other the second (+0x4ec / +0x4b4,
// +0x53c / +0x4bc; string 0x2a7), with the building's year (+0x3d5) and "car" /
// "cars" (0x2a5 / 0x2a6) for counts of one / more.
void __fastcall StationView_CargoTooltip(int32_t id, int, int32_t, int32_t) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    if (!st) return;
    int32_t a = Ftol(F<float>(st, 0x4c4));
    int32_t b = Ftol((double)F<float>(st, 0x4c4) + F<float>(st, 0x4b0));
    int32_t c = Ftol(F<float>(st, 0x4ec));
    int32_t d = Ftol((double)F<float>(st, 0x4ec) + F<float>(st, 0x4b4));
    bool first = id == 0xb3f6;
    int32_t n1 = first ? a : c, n2 = first ? b : d;
    float v = F<float>(st, first ? 0x514 : 0x53c), add = F<float>(st, first ? 0x4b8 : 0x4bc);
    const char* s2 = GetString(n2 == 1 ? 0x2a5 : 0x2a6);
    const char* s1 = GetString(n1 == 1 ? 0x2a5 : 0x2a6);
    int32_t year = F<int32_t>(Obj_Building(st), 0x3d5);
    char m2[0x40], ny2[0x40], ny[0x40], m1[0x40], nn1[0x40], text[0x1e0];
    char* sm2 = Num_FormatSimple2(m2, 0xb, (double)v + add, 0xb);
    char* sn2 = Num_Format(ny2, 1, (double)n2, 10, 1, 0, 0);
    char* sy = Num_Format(ny, 8, (double)year, 10, 1, 0, 0);
    char* sm1 = Num_FormatSimple2(m1, 0xb, v, 0xb);
    char* sn1 = Num_Format(nn1, 1, (double)n1, 10, 1, 0, 0);
    FormatText(text, GetString(first ? 0x2a4 : 0x2a7), sn1, s1, sm1, sy, sn2, s2, sm2, 0);
    Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, text, 1, 1, -1);
}
RT3_REPLACE(0x504770, StationView_CargoTooltip);

// 0x504a90. Leaves a tool mode: resets 0x5ee4fc/0x5ee500, drops this tool's user
// count (+0x7c, 0x62be84) and clears fog mode 1 unless a sub-mode (+0x2175) other
// than 0x15 is active.
RT3_NOINLINE void __fastcall Tool_Leave(uint8_t* t) {
    ResetPair5ee4fc();
    if (F<int32_t>(t, 0x7c)) {
        F<int32_t>(t, 0x7c) = 0;
        hook::Global<int32_t>(0x62be84)--;
    }
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (F<int32_t>(map, 0x2171) != 1) return;
    int32_t sub = F<int32_t>(map, 0x2175);
    if (sub && sub != 0x15) return;
    Map_ClearFogMode(map);
}
RT3_REPLACE(0x504a90, Tool_Leave);

// 0x504ae0. Picks cargo `cargo` in station window w: while the map shows no
// mode (0 / 0x15) and no fog (0 / 1) it switches the overview to that cargo
// around the selected station (view +0x90), counting the first use (+0x7c,
// 0x62be84) and setting fog mode 1; then remembers the pair (0x5ee4fc).
RT3_NOINLINE void __fastcall StationWindow_PickCargo(uint8_t* w, int, int32_t cargo) {
    ResetPair5ee4fc();
    if (!cargo) return;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t mode = F<int32_t>(map, 0x2175), fog = F<int32_t>(map, 0x2171);
    if ((mode == 0 || mode == 0x15) && (fog == 0 || fog == 1)) {
        Overview_SetCargo(map, 0, cargo);
        Overview_SetOption(hook::Global<uint8_t*>(0x62c120), 0, F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
        Overview_SetHighlight(hook::Global<uint8_t*>(0x62c120), 0, F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
        if (!F<int32_t>(w, 0x7c)) {
            F<int32_t>(w, 0x7c) = 1;
            ++hook::Global<int32_t>(0x62be84);
        }
        Map_SetFogMode(hook::Global<uint8_t*>(0x62c120), 0, 1);
    }
    SetGlobals5ee4fc(F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90), cargo);
}
RT3_REPLACE(0x504ae0, StationWindow_PickCargo);

// 0x504ba0 (Ghidra starts it at 0x504baf). Station window: hover callback of
// cargo button `id` (0xb4dc + cargo). On leave (!entered) shows the picked
// cargo 0x6d16f8 again, or leaves the tool. On enter, for the selected
// station (0x6cec20) and unless the input state (0x6d4018 +0xa8c) has
// 0x2800 set: while dragging (0x400) only marks the cargo in the station
// window 0x6d16d8 (+0x78, with sound 0x20) once the pointer moved from
// 0x6d16fc / 0x6d1700; otherwise shows that cargo (0x6d16f4) and a tooltip
// with its stock and quota at the station (0x2a9 / 0x2aa / 0xbfd, or 0xafd
// for cargo kinds +0x46).
void __fastcall StationWindow_CargoHover(int32_t id, int, int32_t entered, int32_t, int32_t, int32_t, int32_t) {
    hook::Global<int32_t>(0x6d16f4) = 0;
    if (!entered) {
        if (hook::Global<int32_t>(0x6d16f8)) StationWindow_PickCargo(hook::Global<uint8_t*>(0x6d16d8), 0, hook::Global<int32_t>(0x6d16f8));
        else Tool_Leave(hook::Global<uint8_t*>(0x6d16d8));
        return;
    }
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    if (F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 0x2800) return;
    int32_t cargoId = id - 0xb4dc;
    char text[0x1f4];
    text[0] = 0;
    uint8_t* cargo = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, (uint32_t)cargoId);
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    if (st && cargo) {
        uint8_t* in = hook::Global<uint8_t*>(0x6d4018);
        if (F<uint32_t>(in, 0xa8c) & 0x400) {
            if (!(Distance2D(hook::Global<float>(0x6d16fc), hook::Global<float>(0x6d1700), F<float>(in, 0xa94), F<float>(in, 0xa98)) < D(0x5d00e8))) {
                hook::Global<uint32_t>(0x6d1700) = 0xbf800000;
                hook::Global<uint32_t>(0x6d16fc) = 0xbf800000;
                uint8_t* win = hook::Global<uint8_t*>(0x6d16d8);
                if (win && cargoId != F<int32_t>(win, 0x78) && cargoId != hook::Global<int32_t>(0x6d16f8)) {
                    Ui_PlaySound(0x20, 0, 0x3f000000);
                    win = hook::Global<uint8_t*>(0x6d16d8);
                }
                F<int32_t>(win, 0x78) = cargoId;
            }
        } else {
            hook::Global<int32_t>(0x6d16f4) = cargoId;
            StationWindow_PickCargo(hook::Global<uint8_t*>(0x6d16d8), 0, cargoId);
            float stock = 0.0f, quota = 0.0f;
            Obj_CargoStockAndQuota(st, 0, cargoId, &stock, &quota);
            char num[0xc8];
            Num_FormatSimple(num, 9, (double)Ftol(quota / D(0x5c87f8)), 0xb);
            if (cargo[0x46]) {
                const char* a = LocalizeName((const char*)cargo + 4);
                const char* b = LocalizeName((const char*)cargo + 4);
                FormatText(text, GetString(0xafd), a, (const char*)st + 0x46b, b, (const char*)st + 0x46b, 0, 0, 0, 0);
            } else {
                char amount[0x64];
                ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(amount, (const char*)0x5d05ec, (double)stock);
                const char* name = LocalizeName((const char*)cargo + 4);
                FormatText(text, GetString(0x2a9), name, amount, 0, 0, 0, 0, 0, 0);
                char part[0xc8];
                FormatText(part, GetString(0x2aa), num, 0, 0, 0, 0, 0, 0, 0);
                strcat(text, part);
                name = LocalizeName((const char*)cargo + 4);
                FormatText(part, GetString(0xbfd), name, 0, 0, 0, 0, 0, 0, 0);
                strcat(text, part);
            }
        }
    }
    Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, text, 1, 1, -1);
}
RT3_REPLACE(0x504ba0, StationWindow_CargoHover);

// 0x504ec0. Station detail summary: the station's name (used as a format
// string, as the original does) and a 5 x 2 table in font 0x6d16e4: row labels
// (strings 3 / 0x2ac, or 0x2ad in the second row for station type +0x8c = 4),
// then the last three years (header year, value +0x564 per year) and a total
// column (string 0x263; value year 0 + +0x4c0).
RT3_NOINLINE void __fastcall StationDetail_DrawSummary(uint8_t* st, int, float x, float y, float w, int32_t) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    char buf[0x184];
    ExeSprintf()(buf, (const char*)(st + 0x46b));
    uint8_t* font = hook::Global<uint8_t*>(0x6d16e4);
    Font_DrawTextBox(font, 0, buf, x, y, w, BitsF(0x3ccccccd), 0xffffffff, 0, 1, 0, 1.0f);
    float rowY = (float)((double)y + Fl(0x5d0100));
    const uint32_t widths[5] = { 0x43750000, 0x43110000, 0x43110000, 0x43110000, 0x431b0000 };
    const int32_t labels[2] = { 3, 0x2ac };
    float top = (float)((double)rowY + D(0x5d0278));
    float cx = (float)((double)x + D(0x5d0700));
    for (int32_t col = 0; col < 5; ++col) {
        float cw = (float)((double)BitsF(widths[col]) / D(0x5ca2c8));
        float cy = top;
        int32_t align = (col != 0) + 2;
        for (int32_t row = 0; row < 2; ++row) {
            if (col == 0) {
                uint8_t* b = (uint8_t*)Obj_Building(st);
                uint8_t* type = (*(uint8_t*(__fastcall**)(uint8_t*))(*(uint8_t**)b + 0x80))(b);
                strcpy(buf, GetString(type[0x8c] == 4 && row == 1 ? 0x2ad : labels[row]));
            } else if (row == 0) {
                if (col == 4)
                    FormatText(buf, GetString(0x263), 0, 0, 0, 0, 0, 0, 0, 0);
                else
                    Num_Format(buf, 8, (double)(F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - col + 1), 10, 1, 0, 0);
            } else {
                int32_t v = col == 4 ? Obj_Int564(st, 0, 0) + F<int32_t>(st, 0x4c0) : Obj_Int564(st, 0, col - 1);
                Num_Format(buf, 1, (double)v, 10, 1, 0, 0);
            }
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16e4), 0, buf, cx, cy, cw, BitsF(0x3ccccccd), 0xffffffff, align, 0, 0, 1.0f);
            cy = (float)((double)cy + Fl(0x5d0100));
        }
        cx = (float)((double)cw + cx + D(0x5d00e8));
    }
}
RT3_REPLACE(0x504ec0, StationDetail_DrawSummary);

// 0x505150. Station view: one waiting-cargo gauge (`which` 1: string 0x2ae, 0:
// 0x2af, font 0x6d16ec) at (x, y, w, h): `amount` / 10000 (0..100) as rows of
// up to 10 cargo icons (world +0x46a84 entry 0x10) from the right edge leftwards
// and from the bottom up, the last icon clipped to its fraction; unless
// 0x6d16f0, a hover area 0xb3f6 / 0xb3f7 with the tooltip 0x504770.
RT3_NOINLINE void __fastcall StationView_DrawCargoGauge(int32_t which, int, float x, float y, float w, float h, float amount) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    float right = (float)((double)x + w);
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16ec), 0, GetString(which ? 0x2ae : 0x2af), x, y, w, h, 0xffffffff, 0, 4, 0, 1.0f);
    float v = (float)((double)amount / D(0x5c8d18));
    if (!(v > Fl(0x5c8598))) v = 0.0f;
    else if (!(v < Fl(0x5c8c28))) v = 100.0f;
    int32_t rows = 1 - Ftol(((double)v - D(0x5c8678)) / D(0x5d16f8));
    for (int32_t row = 0, base = 0; row < rows; ++row, base += 10) {
        float rem = (float)((double)v - (float)base);
        int32_t n = Ftol((double)rem - D(0x5c8678)) + 1;
        if (n >= 10) n = 10;
        for (int32_t k = 0; k < n; ++k) {
            float frac = (float)((double)rem - (float)k);
            if (frac < D(0x5c85b8)) continue;
            if (!(frac <= Fl(0x5c85b4))) frac = 1.0f;
            float gx = (float)((double)right - D(0x5d0708) - (double)k * D(0x5c9bc0));
            float gy = (float)((double)y + h - D(0x5c9b80) - (double)row * D(0x5ca2d8));
            float clip = (float)((D(0x5c8770) - frac) * D(0x5d0700) + gx);
            Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec78), 0x46a84), 0, 0x10), 0, gx, gy, 0x3f800000, 1.0f, 1.0f,
                        0xffffffff, 0, 1, 1, clip, 0.0f, 1.0f, 1.0f);
        }
    }
    if (hook::Global<int32_t>(0x6d16f0)) return;
    uint8_t def[0x8b];
    memset(def, 0, sizeof def);
    F<float>(def, 0xb) = (float)((double)y - D(0x5c9bc0));
    F<float>(def, 0x13) = (float)((double)h + D(0x5c85b8));
    F<float>(def, 7) = x;
    F<float>(def, 0xf) = w;
    F<uint16_t>(def, 0) = (uint16_t)(which ? 0xb3f6 : 0xb3f7);
    uint8_t* t = (uint8_t*)ExeNew(0xda);
    t = t ? TimerWidget_Construct(t, 0, def) : nullptr;
    Window_AddChild(hook::Global<uint8_t*>(0x6d16d8), 0, t, 0);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, 0x82, F<uint16_t>(def, 0), 0x504770, 0, 0);
}
RT3_REPLACE(0x505150, StationView_DrawCargoGauge);

// 0x505470. Station detail (0x6d16d8): for each of the 5 kinds with a count
// (station +0x590.., selected station in the main view +0x90), unless 0x6d16f0,
// a "StationDetail.imb" button 0xb40a + kind (image kind, tooltip string 0x2b0
// with the count and the kind's name 0xa81..), and for counts over 1 a count
// box 0xb400 + kind; rows step 0.046875 (buttons) / 0.04375 (boxes) right.
RT3_NOINLINE void __fastcall StationDetail_AddKindButtons(void*, int, float x, float y, float, float) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    int32_t row = 0;
    for (int32_t i = 0; i < 5; ++i) {
        int32_t count = Obj_Array590(st, 0, i);
        if (count <= 0) continue;
        if (!hook::Global<int32_t>(0x6d16f0)) {
            double rowD = (double)row;
            uint8_t def[0x89];
            memset(def, 0, sizeof def);
            F<float>(def, 7) = (float)(rowD * D(0x5d1718) + x + D(0x5d0830));
            F<uint32_t>(def, 0xf) = 0x3d333333;
            F<uint32_t>(def, 0x13) = 0x3d01b4e8;
            def[0x29] = 0x20;
            float by = (float)((double)y + D(0x5d0820));
            F<uint16_t>(def, 0x5f) = (uint16_t)i;
            F<uint16_t>(def, 0) = (uint16_t)(i + 0xb40a);
            F<float>(def, 0xb) = by;
            String_Assign(&F<char*>(def, 0x43), (const char*)0x5d1700);
            const char* plural = count > 1 ? GetString(0x2b1) : (const char*)0x5c87a8;
            double countD = (double)count;
            const char* name1 = GetString(((const int32_t*)0x620dc4)[i]);
            const char* name2 = GetString(((const int32_t*)0x620dc4)[i]);
            char num[0x40], text[0x200];
            const char* n = Num_Format(num, 1, countD, 10, 1, 0, 0);
            FormatText(text, GetString(0x2b0), n, name2, plural, name1, 0, 0, 0, 0);
            String_Assign(&F<char*>(def, 0x3f), text);
            uint8_t* b = (uint8_t*)ExeNew(0x111);
            b = b ? Scrollbar_Construct(b, 0, def) : nullptr;
            Window_AddChild(hook::Global<uint8_t*>(0x6d16d8), 0, b, 0);
            Free(F<void*>(def, 0x43));
            Free(F<void*>(def, 0x3f));
            if (count > 1) {
                uint8_t ed[0x91];
                memset(ed, 0, sizeof ed);
                F<float>(ed, 7) = (float)(rowD * D(0x5d08f0) + x + D(0x5d0830));
                F<float>(ed, 0xb) = by;
                F<uint32_t>(ed, 0xf) = 0x3d333333;
                F<uint32_t>(ed, 0x13) = 0x3d01b4e8;
                ed[0x27] = 0;
                ed[0x28] = 0;
                ed[0x33] = 1;
                F<int32_t>(ed, 0x34) = -1;
                F<int32_t>(ed, 0x2f) = 0xb0;
                F<uint16_t>(ed, 0) = (uint16_t)(i + 0xb400);
                Num_Format(text, 1, countD, 10, 1, 0, 0);
                String_Assign(&F<char*>(ed, 0x77), text);
                uint8_t* e = (uint8_t*)ExeNew(0xf0);
                e = e ? EditBox_Construct(e, 0, ed) : nullptr;
                Window_AddChild(hook::Global<uint8_t*>(0x6d16d8), 0, e, 0);
                Free(F<void*>(ed, 0x77));
            }
        }
        ++row;
    }
}
RT3_REPLACE(0x505470, StationDetail_AddKindButtons);

// 0x505760. Station view (selected station, main view +0x90): the cargo grid
// at (x, y), five cells a row over 273 / 1200 of height shared by (n - 1) / 5
// + 2 rows: each available cargo's stock ("%4.1f", grey under 0.05, yellow when
// selected in 0x6d16f4) and price ("---" when the cargo type has +0x46, else
// thousands) in font 0x6d16ec; unless 0x6d16f0, its "<name>.imb" icon (or
// Default.imb; AnyCargo for 0) 0xb414 + id dimmed when empty, and a hover area
// 0xb4dc + id (handler 0x504ba0) pressed for the cargo 0x6d16f8.
RT3_NOINLINE void __fastcall StationView_DrawCargoGrid(void*, int, float x, float y, float, float) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    int32_t n = Obj_CountAvailableCargos(st);
    float rowY = (float)((double)y - D(0x5d1740));
    int32_t rows = (n - 1) / 5 + 2;
    float x0 = (float)((double)x + D(0x5c9ee0) - D(0x5caae0));
    float rowH = (float)(D(0x5d13c8) / (double)rows / D(0x5ca2d0));
    rowY = (float)((double)rowH + rowY);
    if (rows <= 0) return;
    int32_t i = 0;
    char text[0x184], name[0x100];
    for (int32_t r = rows; r; --r) {
        float cx = x0;
        for (int32_t k = 5; k; --k) {
            if (i >= n) continue;
            float stock, quota;
            ++i;
            uint32_t id = Obj_NthAvailableCargo(st, 0, i, &stock, &quota, 0);
            uint8_t* cargo = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, id);
            uint32_t color = stock < D(0x5c9ba0) ? 0xff9f9f9f : 0xffffffff;
            if (hook::Global<uint32_t>(0x6d16f4) == id) color = stock < D(0x5c9ba0) ? 0xff9f9f3f : 0xfffff799;
            ExeSprintf()(text, (const char*)0x5d0eb0, (double)stock);
            float tx = (float)((double)cx - Fl(0x5ce838));
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16ec), 0, text, tx, rowY, BitsF(0x3d225700), rowH, color, 3, 0, 0, 1.0f);
            if (cargo[0x46]) ExeSprintf()(text, (const char*)0x5d1738);
            else Num_FormatSimple(text, 9, (double)Ftol((double)quota / D(0x5c87f8)), 0xb);
            float px = (float)((double)tx + D(0x5d1730));
            Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16ec), 0, text, px, rowY, BitsF(0x3d11b08a), rowH, color, 3, 0, 0, 1.0f);
            if (!hook::Global<int32_t>(0x6d16f0)) {
                uint8_t d[0x89];
                memset(d, 0, sizeof d);
                F<float>(d, 7) = (float)((double)tx + D(0x5d1728) - D(0x5c8918));
                F<uint32_t>(d, 0xf) = 0x3cae147b;
                F<float>(d, 0x13) = rowH;
                F<float>(d, 0xb) = (float)(((double)rowH - D(0x5d1720)) * D(0x5c8628) + rowY);
                F<uint16_t>(d, 0) = (uint16_t)(id + 0xb414);
                if (!id) FormatText(name, (const char*)0x5d01bc, (const char*)0x5d05d4, 0, 0, 0, 0, 0, 0, 0);
                else FormatText(name, (const char*)0x5d01bc, (const char*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, id) + 4, 0, 0, 0, 0, 0, 0, 0);
                String_Assign(&F<char*>(d, 0x43), NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, 0) ? name : (const char*)0x5d0194);
                uint8_t* icon = (uint8_t*)ExeNew(0x111);
                icon = icon ? Scrollbar_Construct(icon, 0, d) : nullptr;
                Window_AddChild(hook::Global<uint8_t*>(0x6d16d8), 0, icon, 0);
                Free(F<void*>(d, 0x43));
                memset(d, 0, sizeof d);
                F<float>(d, 7) = tx;
                F<float>(d, 0xb) = rowY;
                F<uint32_t>(d, 0xf) = 0x3dd020c5;
                F<float>(d, 0x13) = rowH;
                F<uint16_t>(d, 0) = (uint16_t)(id + 0xb4dc);
                d[0x29] = 0x20;
                F<int32_t>(d, 0x61) = 2;
                uint8_t* hot = (uint8_t*)ExeNew(0x111);
                hot = hot ? Scrollbar_Construct(hot, 0, d) : nullptr;
                Window_AddChild(hook::Global<uint8_t*>(0x6d16d8), 0, hot, 0);
                Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, 0x7b, F<uint16_t>(d, 0), 0x504ba0, 0, 0);
                if (id == hook::Global<uint32_t>(0x6d16f8)) Widget_SetFlags(hot, 0, 1, 0, 0);
                else Widget_ClearFlags(hot, 0, 1);
            }
            Window_SendEvent5(hook::Global<uint8_t*>(0x6d16d8), 0, 0xb8, id + 0xb414, stock < D(0x5c9ba0) ? 0x70 : 0xff, 0, 0);
            cx = (float)((double)cx + D(0x5d1728));
        }
        rowY = (float)((double)rowH + rowY);
    }
}
RT3_REPLACE(0x505760, StationView_DrawCargoGrid);

// 0x505bf0. Station detail window ("StationDetail.win") destructor: resets the
// selection pair 0x5ee4fc (twice, as the original does), drops its open-window
// count (+0x7c -> 0x62be84), leaves the station overlay fog mode (0 or 0x15) and
// releases its five images (0x6d16dc..ec).
void __fastcall StationDetail_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1748;
    ResetPair5ee4fc();
    ResetPair5ee4fc();
    if (F<int32_t>(w, 0x7c)) {
        F<int32_t>(w, 0x7c) = 0;
        --hook::Global<int32_t>(0x62be84);
    }
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (F<int32_t>(map, 0x2171) == 1 && (F<int32_t>(map, 0x2175) == 0 || F<int32_t>(map, 0x2175) == 0x15)) Map_ClearFogMode(map);
    for (uint32_t g : { 0x6d16dcu, 0x6d16e0u, 0x6d16e4u, 0x6d16e8u, 0x6d16ecu }) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(g));
    hook::Global<int32_t>(0x6d16d8) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x505bf0, StationDetail_Destruct);

// 0x505cb0. Steps the selected building entry (0x6cec20, id in 0x6d0818
// +0x90) by `dir` to the next one with the same value and state, wrapping
// around: closes the model preview, selects it and - unless the camera is
// busy (+0x60) - points the camera at its building (mode option +0x315) and
// opens the building's view (vcall 0x5c).
RT3_NOINLINE void __fastcall Buildings_SelectNext(int32_t dir) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    int32_t sel = F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90);
    if (sel <= 0 || !Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)sel)) return;
    int32_t value = Obj_BuildingValue(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)));
    int32_t at = (int32_t)Collection_PositionOf(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90), 0);
    Collection* c = hook::Global<Collection*>(0x6cec20);
    int32_t state = Obj_BuildingStateActive(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)at, 0)));
    int32_t i = at;
    for (;;) {
        i = (Collection_Count(hook::Global<Collection*>(0x6cec20)) + i + dir) % Collection_Count(hook::Global<Collection*>(0x6cec20));
        if (i == at) return;
        c = hook::Global<Collection*>(0x6cec20);
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (state != Obj_BuildingStateActive(o) || value != Obj_BuildingValue(o)) continue;
        ModelPreview_Close();
        Panel_SetValue90(hook::Global<uint8_t*>(0x6d0818), 0, (int32_t)Collection_IdAt(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)i, 0));
        if (F<int32_t>(hook::Global<uint8_t*>(0x62be68), 0x60)) return;
        int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x315);
        Camera_FollowObject((uint8_t*)Obj_Building(o), mode);
        uint8_t* b = (uint8_t*)Obj_Building(o);
        ((void(__fastcall*)(uint8_t*))(*(void***)b)[0x17])(b);
        return;
    }
}
RT3_REPLACE(0x505cb0, Buildings_SelectNext);

// 0x505e30 / 0x505e40. Next / previous building entry.
RT3_NOINLINE void Buildings_SelectNextFwd() { Buildings_SelectNext(1); }
RT3_REPLACE(0x505e30, Buildings_SelectNextFwd);

RT3_NOINLINE void Buildings_SelectNextBack() { Buildings_SelectNext(-1); }
RT3_REPLACE(0x505e40, Buildings_SelectNextBack);

 // namespace

// 0x505e50 (event handler). The station window w of the main view's
// station (+0x90). Idle marks it for the world (+0x4cba), applies a picked
// cargo (+0x78 -> 0x6d16f8, the cargo picker 0x6d16d8) and leaves the tool
// when the station is gone. Clicks note the spot (0x6d16fc / 0x6d1700):
// cargo buttons 0xb4dc.. pick / drop a cargo (or with the track tool flag
// 4 open the track picker on it), the served buildings 0xb40a.. show one
// (view 0xa, followed unless the camera follows something), 0xb3c0 renames
// the station (string 0xe94, 60 characters), 0xb3f8 flips its +0x5c9;
// 0x3ea refreshes the buttons; the tabs 0xb3b5 / 0xb3b6 swap the lists, the
// buttons 0xb3b9 / 0xb3ba close (not in a tutorial, 0xe8c) and 0xb3bd..
// change the station's size (0xb3bd smaller, else larger) after a price
// prompt (0xbfe) or tell why not (0xbff), also for other owners' stations.
int32_t __fastcall StationWindow_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    uint8_t* view = hook::Global<uint8_t*>(0x6d0818);
    uint8_t* st = Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(view, 0x90))
                      ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))
                      : nullptr;
    auto cargoButtons = [&](uint32_t type) {
        Window_SendEvent5(w, 0, type, hook::Global<uint32_t>(0x6d16f8) + 0xb4dc, 1, 0, 0);
    };
    const int32_t id = (int32_t)msg[1];
    switch (msg[0]) {
    case 0:
        if (uint8_t* world = hook::Global<uint8_t*>(0x6cec78)) F<uint32_t>(world, 0x4cba) = F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90);
        if (F<int32_t>(w, 0x78) != -1) {
            if (F<int32_t>(w, 0x78) != hook::Global<int32_t>(0x6d16f8)) {
                cargoButtons(0x87);
                hook::Global<int32_t>(0x6d16f8) = F<int32_t>(w, 0x78);
                StationWindow_PickCargo(hook::Global<uint8_t*>(0x6d16d8), 0, F<int32_t>(w, 0x78));
                cargoButtons(0x65);
            }
            F<int32_t>(w, 0x78) = -1;
        }
        if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)))
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
        return 1;
    case 0xca:
        hook::Global<uint32_t>(0x6d16fc) = msg[9];
        hook::Global<uint32_t>(0x6d1700) = msg[10];
        if (id >= 0xb4dc && id <= 0xb5a3) {
            const int32_t cargo = id - 0xb4dc;
            if (F<uint8_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 4) {
                if (!hook::Global<uint8_t*>(0x6d0818)) return 0;
                View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 9, 0);
                if (uint8_t* picker = hook::Global<uint8_t*>(0x6d12f0)) TrackPicker_SetTab(picker, 0, 0, cargo);
                return 0;
            }
            cargoButtons(0x87);
            if (hook::Global<int32_t>(0x6d16f8) == cargo) {
                hook::Global<int32_t>(0x6d16f8) = 0;
                Tool_Leave(hook::Global<uint8_t*>(0x6d16d8));
            } else {
                hook::Global<int32_t>(0x6d16f8) = cargo;
                hook::Global<uint32_t>(0x6d1700) = 0xbf800000;
                hook::Global<uint32_t>(0x6d16fc) = 0xbf800000;
                if (cargo)
                    StationWindow_PickCargo(hook::Global<uint8_t*>(0x6d16d8), 0, cargo);
                else
                    Tool_Leave(hook::Global<uint8_t*>(0x6d16d8));
            }
            cargoButtons(0x65);
            return 0;
        }
        if (id >= 0xb40a && id <= 0xb413) {
            const int32_t k = id - 0xb40a;
            if (!st) return 0;
            Obj_Array590(st, 0, k);
            uint8_t* pair = Pairs_At(st, 0, k, 1);
            if (!Collection_IsValid(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(pair, 0))) return 0;
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0xa, F<int32_t>(pair, 0));
            if (F<void*>(hook::Global<uint8_t*>(0x62be68), 0x60)) return 0;
            uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(pair, 0));
            Camera_FollowObject(b, F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x315));
            ((void(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x17](b);
            return 0;
        }
        if (id == 0x3e7) return 0;
        if (id == 0xb3c0) {
            char name[0x3e8];
            strcpy(name, (const char*)st + 0x46b);
            Dialog_RunMessage(GetString(0xe94), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)name, 0x1e, 0, nullptr, nullptr, 0, 0,
                              nullptr, 0);
            StrCopyN((char*)st + 0x46b, name, 0x3c);
            return 0;
        }
        if (id == 0xb3f8) {
            if (View_FocusValid()) {
                const uint32_t sid = F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90);
                uint8_t* a = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, sid);
                uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, sid);
                a[0x5c9] = b[0x5c9] == 0;
            }
            return 0;
        }
        return 1;
    case 0x3ea: {
        PendingRows_Clear();
        StationDialog_UpdateButtons();
        uint32_t on = 0;
        if (Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)) &&
            ((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)))[0x5c9])
            on = 1;
        Window_SendEvent5(w, 0, 0x74, 0xb3f8, on, 0, 0);
        if (hook::Global<int32_t>(0x6d16f8)) StationWindow_PickCargo(w, 0, hook::Global<int32_t>(0x6d16f8));
        return 1;
    }
    case 0xcb:
        switch (id) {
        case 0xb3b5:
        case 0xb3b6:
            PendingRows_Clear();
            Window_SendEvent5(w, 0, 0x6d, id == 0xb3b5 ? 0xb3b3 : 0xb3b4, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x86, id == 0xb3b5 ? 0xb3b4 : 0xb3b3, 3, 0, 0);
            return 0;
        case 0xb3b9:
        case 0xb3ba:
            if (hook::Global<int32_t>(0x6d3b4c)) {
                Message492(GetString(0xe8c), 1, 0x3ef5c28f);
                return 0;
            }
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
            return 0;
        case 0xb3bd:
        case 0xb3be:
        case 0xb3bf: {
            if (!st) return 0;
            if (!View_FocusIsOwnStation()) {
                Station_ShowOwnerMessage();
                return 0;
            }
            const int32_t dir = (id != 0xb3bd) + 1;
            const uint8_t sizes = ((uint8_t*)Obj_Ref3cc(Building492(st)))[0xc3];
            if (!Obj_BuildingStateActive(st)) return 0;
            int32_t level = 0;
            if (sizes & 2) level = 1;
            if (sizes & 4) level = 2;
            if (sizes & 8) level = 3;
            if (sizes & 0x10) level = 4;
            if (sizes & 0x20) level = 5;
            const uint32_t type = Records_FindComposedName(hook::Global<Collection*>(0x62b2fc), 0, level, dir);
            if (!type) return 0;
            uint8_t* rec = (uint8_t*)Obj_Station173((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, type));
            float cost;
            char why[0x3e8], text[0x3e8];
            if (Building_ChangeType(Building492(st), 0, type, 1, &cost, why)) {
                uint8_t* b = Building492(st);
                const char* newName = LocalizeName((const char*)rec + 0x4f);
                const char* oldName = LocalizeName((const char*)Type492(b) + 0x4f);
                char num[0x64];
                const char* price = Num_FormatSimple(num, 9, cost, 0xb);
                FormatText(text, GetString(0xbfe), oldName, newName, price, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (Message492(text, 3, 0xbf800000) != 0x3f2) return 0;
                Building_ChangeType(Building492(st), 0, type, 0, nullptr, nullptr);
                if (hook::Global<uint8_t*>(0x6d16d8)) StationDialog_UpdateButtons();
                return 0;
            }
            const char* oldName = LocalizeName((const char*)Type492(Building492(st)) + 0x4f);
            const char* newName = LocalizeName((const char*)rec + 0x4f);
            FormatText(text, GetString(0xbff), oldName, newName, why, nullptr, nullptr, nullptr, nullptr, nullptr);
            Message492(text, 1, 0xbf800000);
            return 0;
        }
        }
        return 1;
    default:
        return 1;
    }
}
RT3_REPLACE(0x505e50, StationWindow_HandleEvent);

// 0x506610 (draw callback). Station detail panel of the selected station
// (0x6d0818 +0x90 in 0x6cec20): its cargo count (+0x3d5, string 0x378); in
// list mode (0x6cec74 +0x1db) the rows of the other stations, for depot
// kinds 3 / 4 the summary, else its name (QUIRK: used as the format), the
// kind buttons, the two cargo gauges (+0x514 / +0x53c) and the cargo grid.
void __fastcall StationDetail_DrawCb(int32_t id, int, float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90))) return;
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    char frame[0x228];
    char* num = frame + 0x14;
    char* text = frame + 0x34;
    Num_Format(num, 8, (double)F<int32_t>(Obj_Building(st), 0x3d5), 10, 1, 0, 0);
    FormatText(text, GetString(0x378), num, 0, 0, 0, 0, 0, 0, 0);
    const float tw = (float)((double)w - D(0x5d0280));
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16ec), 0, text, x, y, tw, BitsF(0x3ccccccd), 0xffffffff, 3, 1, 0, 1.0f);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1db)) {
        PendingRows_Clear();
        StationDetail_DrawRows(x, y, w, Bits(h));
        return;
    }
    auto kind = [&]() {
        uint8_t* b = (uint8_t*)Obj_Building(st);
        return ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b)[0x8c];
    };
    if (kind() == 3 || kind() == 4) {
        PendingRows_Clear();
        StationDetail_DrawSummary(st, 0, x, y, w, Bits(h));
        return;
    }
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(text, (const char*)st + 0x46b);
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6d16e8), 0, text, x, y, w, BitsF(0x3ccccccd), 0xffffffff, 0, 1, 0, 1.0f);
    StationDetail_AddKindButtons(nullptr, 0, x, y, w, h);
    const double gauge = ((double)h - D(0x5d0238)) * D(0x5c8628) - D(0x5d01d8);
    const float gh = (float)gauge;
    const float a = (float)Obj_Float514(st, 0, 0);
    StationView_DrawCargoGauge(1, 0, x, (float)((double)y + D(0x5d1760)), BitsF(0x3d800000), gh, a);
    const float b = (float)Obj_Float53c(st, 0, 0);
    StationView_DrawCargoGauge(0, 0, x, (float)((double)y + gauge + D(0x5d1758)), BitsF(0x3d800000), gh, b);
    StationView_DrawCargoGrid(nullptr, 0, x, y, w, h);
    hook::Global<int32_t>(0x6d16f0) = 1;
}
RT3_REPLACE(0x506610, StationDetail_DrawCb);

// 0x5068c0. Station detail dialog constructor ("StationDetail.win", vtable
// 0x5d1748, kept in 0x6d16d8): images "CompanyLogo.imb" / "StationDetail.imb"
// (0x6d16dc / 0x6d16e0), fonts 3 / 2 / 1 (0x6d16e4..0x6d16ec), no selection
// (+0x78 = -1), handler 0x506610 on both lists, page buttons, and the "show on
// map" toggle 0xb3f8 (only with the option and world flag, ticked when the main
// view's building +0x90 has +0x5c9 set); then the button states.
uint8_t* __fastcall StationDetail_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1748;
    hook::Global<uint8_t*>(0x6d16d8) = w;
    hook::Global<int32_t>(0x6d16f4) = 0;
    hook::Global<int32_t>(0x6d16f8) = 0;
    F<int32_t>(w, 0x78) = -1;
    F<int32_t>(w, 0x7c) = 0;
    uint8_t* pack = hook::Global<uint8_t*>(0x6d4020);
    hook::Global<uint8_t*>(0x6d16dc) = Pack_AcquireNamed(pack, 0, (const char*)0x5c9f98);
    hook::Global<uint8_t*>(0x6d16e0) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d1700);
    hook::Global<uint8_t*>(0x6d16e4) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    hook::Global<uint8_t*>(0x6d16e8) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 2);
    hook::Global<uint8_t*>(0x6d16ec) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 1);
    hook::Global<int32_t>(0x6d16f0) = 0;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1768);
    Window_SendEvent5(w, 0, 0x7a, 0xb3b7, 0x506610, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0xb3b8, 0x506610, 0, 0);
    Window_SendEvent5(w, 0, 0x6d, 0xb3b3, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x86, 0xb3b4, 3, 0, 0);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68) && F<int32_t>(WorldG(), 0x4af7)) {
        Window_SendEvent5(w, 0, 0x65, 0xb3f8, 3, 0, 0);
        uint32_t id = F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90);
        bool on = Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, id) &&
                  ((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90)))[0x5c9];
        Window_SendEvent5(w, 0, 0x74, 0xb3f8, on ? 1 : 0, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x87, 0xb3f8, 3, 0, 0);
    }
    StationDialog_UpdateButtons();
    return w;
}
RT3_REPLACE(0x5068c0, StationDetail_Construct);

// 0x506a80. Station list dialog: releases its two pack images (0x6d1704, 0x6d170c).
void __fastcall StationList_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d177c;
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6d1704));
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6d170c));
    hook::Global<int32_t>(0x6d1708) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x506a80, StationList_Destruct);

// 0x506ac0. Draws a station's summary at (x, y): its building-type icon (sprite
// 0x622acc[first of 6 type bits at +0xc3] + 6 * (2 - size byte +0x8c), shifted by
// that * 0x5caad0), its name and the class 0x385 / 0x386 supply figures (strings
// 0xef / 0xf0) in text boxes of height h at fixed offsets.
void __fastcall Station_DrawSummary(int32_t, uint32_t id, float x, float y, int32_t, float h, int32_t) {
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id);
    uint8_t* type = (uint8_t*)Obj_Ref3cc(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(st, 4)));
    uint32_t mask = F<uint32_t>(type, 0xc3);
    int32_t bit = 0;
    for (; bit < 6; ++bit)
        if (mask & 1u << bit) break;
    uint8_t* b = (uint8_t*)Obj_Building(st);
    uint8_t* rec = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
    int32_t n = 2 - rec[0x8c];
    double shift = (double)n * D(0x5caad0);
    double dx = (double)x;
    float gy = (float)((double)y + shift + D(0x5caad0));
    float gx = (float)(dx + shift);
    uint8_t* g = (uint8_t*)Table_Entry(hook::Global<uint8_t*>(0x6d170c), 0, ((const int32_t*)0x622acc)[bit] + n * 6);
    Glyph_Queue(g, 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    char text[0x100], num[0x20];
    FormatText(text, (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0, 0);
    uint8_t* font = hook::Global<uint8_t*>(0x6d1704);
    Font_DrawTextBox(font, 0, text, (float)(dx + D(0x5c9730)), y, BitsF(0x3e4ccccd), h, 0xffffffff, 2, 0, 0, 1.0f);
    float s386 = Region_ClassSupply(st, 0, 0x386);
    float s385 = Region_ClassSupply(st, 0, 0x385);
    {
        const char* v = Num_Format(num, 4, (double)s385, 10, 1, 0, 0);
        FormatText(text, GetString(0xef), v, 0, 0, 0, 0, 0, 0, 0);
    }
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6d1704), 0, text, (float)(dx + D(0x5d0808)), y, BitsF(0x3e133333), h, 0xffffffff, 2, 0, 0, 1.0f);
    {
        const char* v = Num_Format(num, 4, (double)s386, 10, 1, 0, 0);
        FormatText(text, GetString(0xf0), v, 0, 0, 0, 0, 0, 0, 0);
    }
    Font_DrawTextBox(hook::Global<uint8_t*>(0x6d1704), 0, text, (float)(dx + D(0x5d1788)), y, BitsF(0x3e133333), h, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x506ac0, Station_DrawSummary);

// 0x506d50 (list callback). Selects station `id` (EDX); input flags 3 fly
// the camera to it, flag 4 renames it (string 0xe94, 60 characters).
void __fastcall StationList_SelectCb(void*, uint32_t id) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, id)) return;
    Panel_SetValue90(hook::Global<uint8_t*>(0x6d0818), 0, (int32_t)id);
    if (hook::Global<uint8_t*>(0x6d4018)[0xa8c] & 3)
        Camera_FollowObject((uint8_t*)Obj_Building(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id)), 0);
    if (!(hook::Global<uint8_t*>(0x6d4018)[0xa8c] & 4)) return;
    Ui_SendEvent5(hook::Global<uint8_t**>(0x6d401c), 0, 0xbe, 0, 0, 0, 0);
    char* name = (char*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id) + 0x46b;
    char buf[0x1f4];
    strcpy(buf, name);
    Message447(GetString(0xe94), 9, (uint32_t)(uintptr_t)buf, 0x1e);
    StrCopyN(name, buf, 0x3c);
}
RT3_REPLACE(0x506d50, StationList_SelectCb);

// 0x506e50 (hover callback). Station `id`'s supply of cargo classes 0x385 /
// 0x386 as a tooltip (string 0xf32 with its name).
void __stdcall Station_SupplyTooltipCb(uint32_t id, int32_t, int32_t) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, id)) return;
    uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id);
    if (!s) return;
    char a[0x64], b[0x64], text[0x1f4];
    Num_Format(a, 4, (double)Region_ClassSupply(s, 0, 0x385), 10, 1, 0, 0);
    Num_Format(b, 4, (double)Region_ClassSupply(s, 0, 0x386), 10, 1, 0, 0);
    const char* name = (const char*)(s + 0x46b);
    FormatText(text, GetString(0xf32), name, a, b, name, name, 0, 0, 0);
    Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, text, 1, 1, -1);
}
RT3_REPLACE(0x506e50, Station_SupplyTooltipCb);

// 0x506f30. Fills the station picker's two lists 0x61a9 / 0x61aa with the
// player's active rail stations (building +0x3d4 == 1, owned by world +0x21),
// callbacks 0x506e50 / 0x506ac0 / 0x506d50 (events 0xb1 / 0x7a / 0x79), keeping
// +0x78; scrolls to 0x622ae4 (event 0x6b, unless -1) and selects the world's
// current station +0x4cba when it is the player's; labels 0x61af / 0x61b0 show
// string 0xf2 when there are no stations at all.
void __fastcall StationPicker_Fill(uint8_t* w) {
    int32_t keep = F<int32_t>(w, 0x78);
    Window_SendEvent5(w, 0, 0x8b, 0x61a9, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x61aa, 0, 0, 0);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
        Collection* c = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!Obj_BuildingStateActive(s) || Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        int32_t me = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
        if (Obj_BuildingValue(s) != me) continue;
        Window_SendEvent5(w, 0, 0x88, 0x61a9, 0, F<uint32_t>(s, 0), 0);
        Window_SendEvent5(w, 0, 0x88, 0x61aa, 0, F<uint32_t>(s, 0), 0);
    }
    const uint32_t lists[2] = { 0x61a9, 0x61aa };
    for (uint32_t id : lists) Window_SendEvent5(w, 0, 0xb1, id, 0x506e50, 0, 0);
    for (uint32_t id : lists) Window_SendEvent5(w, 0, 0x7a, id, 0x506ac0, 0, 0);
    for (uint32_t id : lists) Window_SendEvent5(w, 0, 0x79, id, 0x506d50, 0, 0);
    F<int32_t>(w, 0x78) = keep;
    if (hook::Global<int32_t>(0x622ae4) != -1) {
        Window_SendEvent5(w, 0, 0x6b, 0x61aa, hook::Global<uint32_t>(0x622ae4), 0, 0);
        Window_SendEvent5(w, 0, 0x6b, 0x61a9, hook::Global<uint32_t>(0x622ae4), 0, 0);
    }
    if (Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba))) {
        uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba));
        int32_t owner = Obj_BuildingValue(s);
        if (owner == Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) {
            Window_SendEvent5(w, 0, 0x69, 0x61aa, F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba), 0, 0);
            Window_SendEvent5(w, 0, 0x69, 0x61a9, F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba), 0, 0);
        }
    }
    char text[0x124];
    if (Collection_Count(hook::Global<Collection*>(0x6cec20)) > 0) text[0] = 0;
    else strcpy(text, GetString(0xf2));
    Window_SendEvent5(w, 0, 0x6f, 0x61af, (uint32_t)(uintptr_t)text, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x61b0, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x506f30, StationPicker_Fill);

// 0x5071e0 (event handler). Station list d (two lists 0x61a9 / 0x61aa by
// 0x6d1710): idle ticks (every 20 ms, 0x6d1714) publish the visible list's
// top row (0x622ae4, -1 without); 0xce picks a station for the world
// (+0x4cba); 0xca also follows its building and opens its view (mode 5);
// the tabs 0x61ad / 0x61ae switch lists.
int32_t __fastcall StationList_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    switch (msg[0]) {
    case 0:
        if (ClockMs() - hook::Global<int32_t>(0x6d1714) > 0x14) {
            hook::Global<int32_t>(0x6d1714) = ClockMs();
            const int32_t id = hook::Global<int32_t>(0x6d1710) ? 0x61a9 : 0x61aa;
            if (uint8_t* l = Widget_FindChild(d, 0, id))
                hook::Global<int32_t>(0x622ae4) = F<int32_t>(l, 0x1e2);
            else
                hook::Global<int32_t>(0x622ae4) = -1;
        }
        return 1;
    case 0xce:
        if ((int32_t)msg[1] < 0x61a9 || (int32_t)msg[1] > 0x61aa) return 1;
        F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba) = msg[3];
        return 0;
    case 0xca: {
        if ((int32_t)msg[1] < 0x61a9 || (int32_t)msg[1] > 0x61aa) return 1;
        if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, msg[3])) return 0;
        uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, msg[3]));
        F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4cba) = msg[3];
        Camera_FollowObject(b, F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x315));
        View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 5, (int32_t)msg[3]);
        return 0;
    }
    case 0xcb: {
        const bool second = msg[1] == 0x61ae;
        if (!second && msg[1] != 0x61ad) return 1;
        hook::Global<int32_t>(0x6d1710) = second ? 1 : 0;
        const uint32_t shown = second ? 0x61a9 : 0x61aa, hidden = second ? 0x61aa : 0x61a9;
        Window_SendEvent5(d, 0, 0x66, shown, F<uint32_t>(d, 0x78), 0, 0);
        Window_SendEvent5(d, 0, 0x6d, second ? 0x61ac : 0x61ab, 3, 0, 0);
        Window_SendEvent5(d, 0, 0x86, second ? 0x61ab : 0x61ac, 3, 0, 0);
        (void)hidden;
        ListView_RefreshVisible(Widget_FindChild(d, 0, shown));
        return 0;
    }
    default:
        return 1;
    }
}
RT3_REPLACE(0x5071e0, StationList_HandleEvent);

// 0x5074c0. Station list dialog constructor ("StationList.win", vtable 0x5d177c,
// kept in 0x6d1708): font 3 (0x6d1704), "StationIcons.imb" (0x6d170c), the lists.
uint8_t* __fastcall StationListDialog_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d177c;
    hook::Global<uint8_t*>(0x6d1708) = w;
    F<int32_t>(w, 0x78) = -1;
    hook::Global<uint8_t*>(0x6d1704) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 3);
    hook::Global<uint8_t*>(0x6d170c) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d0258);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1790);
    PickerDialog_Setup(w, 0x61a9, 0x61aa, 0x61ab, 0x61ac, hook::Global<int32_t>(0x6d1710));
    StationPicker_Fill(w);
    return w;
}
RT3_REPLACE(0x5074c0, StationListDialog_Construct);

// 0x5075b0 (vtable stub).
void __fastcall Nop5075b0(void*) { __asm nop }
RT3_REPLACE(0x5075b0, Nop5075b0);

// 0x5075c0 (event filter). Swallows (turns into 0xc9) presses of 0x80ea and
// releases of 0x80e8 / 0x80e9 (the latter also clears 0x622ae8). Always 1.
int32_t __stdcall Dialog_Filter80e8(uint32_t* ev) {
    if (ev[0] == 0xca) {
        if (ev[1] == 0x80ea) ev[0] = 0xc9;
    } else if (ev[0] == 0xcb) {
        if (ev[1] == 0x80e8) {
            ev[0] = 0xc9;
        } else if (ev[1] == 0x80e9) {
            hook::Global<int32_t>(0x622ae8) = -1;
            ev[0] = 0xc9;
        }
    }
    return 1;
}
RT3_REPLACE(0x5075c0, Dialog_Filter80e8);

// 0x507620. Station picker window ("StationPick.win", vtable 0x5d17a0): list
// 0x80ea gets the stations (0x6cec20) by name (+0x46b, case-insensitive), first
// the active ones (0x47fd50), then the rest greyed (0xffcfcfcf instead of
// 0xfffff699); equal names list the later station first. Handlers 0x5075b0 and
// 0x622ae8; the two non-English buttons 0x80e8 / 0x80e9 are hidden in English;
// with any station the first one is selected.
RT3_NOINLINE uint8_t* __fastcall StationPick_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d17a0;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d17ac);
    Window_SendEvent5(w, 0, 0x79, 0x80ea, 0x5075b0, 0, 0);
    Window_SendEvent5(w, 0, 0xab, 0x80ea, 0x622ae8, 0, 0);
    if (!(uint8_t)IsEnglish()) {
        Window_SendEvent5(w, 0, 0x78, 0x80e8, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x80e9, 0x12, 0, 0);
    }
    uint8_t used[0xfa0];
    memset(used, 0, sizeof used);
    int32_t row = 0;
    auto station = [](int32_t i) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, Collection_IdAt(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)i, 0)); };
    for (int32_t pass = 0; pass < 2; ++pass) {
        for (int32_t k = 0; k < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++k) {
            int32_t best = -1;
            for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
                if (used[i]) continue;
                if (pass == 0) {
                    if (!Obj_BuildingStateActive(station(i))) continue;
                } else if (pass == 1) {
                    if (Obj_BuildingStateActive(station(i))) continue;
                }
                if (best >= 0) {
                    const char* b = (const char*)station(best) + 0x46b;
                    if (Stricmp((const char*)station(i) + 0x46b, b) > 0) continue;
                }
                best = i;
            }
            if (best == -1) continue;
            uint8_t* st = station(best);
            uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)best, 0);
            Window_SendEvent5(w, 0, 0x88, 0x80ea, (uint32_t)(uintptr_t)(st + 0x46b), id, 0);
            Window_SendEvent5(w, 0, 0xaf, 0x80ea, (uint32_t)row, pass ? 0xffcfcfcfu : 0xfffff699u, 0);
            ++row;
            if (best < 0xfa0) used[best] = 1;
        }
    }
    if (Collection_Count(hook::Global<Collection*>(0x6cec20)) >= 1) Window_SendEvent5(w, 0, 0x66, 0x80ea, 0, 0, 0);
    return w;
}
RT3_REPLACE(0x507620, StationPick_Construct);

// 0x5078c0. Runs the station picker 0x6d1718 (game paused), moved down by
// 0x5d17bc when `lower`; returns the pick 0x622ae8, or -1 without stations.
RT3_NOINLINE int32_t __fastcall StationPick_Run(uint8_t lower) {
    if (!Collection_Count(hook::Global<Collection*>(0x6cec20))) return -1;
    uint8_t* w = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x78);
    hook::Global<uint8_t*>(0x6d1718) = w ? StationPick_Construct(w) : nullptr;
    if (lower) F<float>(hook::Global<uint8_t*>(0x6d1718), 5) += Fl(0x5d17bc);
    PauseLock_Push(hook::Global<int32_t*>(0x6cec74));
    Ui_RunModal(hook::Global<uint8_t**>(0x6d401c), 0, hook::Global<uint8_t*>(0x6d1718), 1, 0, 0x14, 0);
    PauseLock_Pop(hook::Global<int32_t*>(0x6cec74));
    Windows_Remove(hook::Global<uint8_t*>(0x6d401c), 0, hook::Global<uint8_t*>(0x6d1718));
    if (uint8_t* p = hook::Global<uint8_t*>(0x6d1718)) {
        F<uint32_t>(p, 0) = 0x5d17a0;
        Widget_SetVtbl(p);
        ((void(__cdecl*)(void*))0x53b080)(p);
    }
    hook::Global<uint8_t*>(0x6d1718) = nullptr;
    return hook::Global<int32_t>(0x622ae8);
}
RT3_REPLACE(0x5078c0, StationPick_Run);

// 0x507980. Whether v is one of the special command codes -11..-17, -20..-22.
RT3_NOINLINE int32_t __fastcall IsSpecialCommand_Impl(int32_t v) {
    return (v <= -11 && v >= -17) || (v <= -20 && v >= -22) ? 1 : 0;
}

// Callers (0x508e07, 0x508c24) keep using ECX/EDX after the call.
__declspec(naked) int32_t IsSpecialCommand() { RT3_KEEP_ECX_EDX(IsSpecialCommand_Impl, 0) }
RT3_REPLACE(0x507980, IsSpecialCommand);

// 0x5079c0. Clears the highlights of info window w: closes the overview
// mode, refreshes the buildings and unhighlights its building (+0x7c),
// industry (+0x80, 0x6ada80), track (+0x88, 0x6cea50) and city (+0x84,
// 0x62bae0) through each table's vtable +8.
RT3_NOINLINE void __fastcall InfoWindow_ClearHighlights(uint8_t* w) {
    Overview_Close(hook::Global<uint8_t*>(0x62c120));
    Table_RefreshAll(hook::Global<Collection*>(0x62b26c));
    static const uint32_t slots[4][2] = { { 0x7c, 0x62b26c }, { 0x80, 0x6ada80 }, { 0x88, 0x6cea50 }, { 0x84, 0x62bae0 } };
    for (const auto& sl : slots) {
        int32_t id = F<int32_t>(w, sl[0]);
        if (id != -1) {
            uint8_t* t = hook::Global<uint8_t*>(sl[1]);
            ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)t)[2])(t, id);
        }
        F<int32_t>(w, sl[0]) = -1;
    }
}
RT3_REPLACE(0x5079c0, InfoWindow_ClearHighlights);

     // -> 0x53e2e0
RT3_REPLACE(0x507a50, InfoWindow_ClearHighlights);

// 0x507a60. Sets the drag state (+0x94); ending it restores the arrow cursor (2).
void __fastcall Panel_SetDragging(uint8_t* w, int, int32_t on) {
    if (!on) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 2, 0, 0);
    F<int32_t>(w, 0x94) = on;
}
RT3_REPLACE(0x507a60, Panel_SetDragging);

// 0x507a90 (draw callback). Draws the compass needle (glyph 1 of 0x6d1724)
// at the building rotation 0x6d172c relative to the camera's (+0x10),
// wrapped to [0, 2 pi).
void __fastcall Placement_DrawCompassCb(void*, int, float x, float y, int32_t, int32_t, int32_t) {
    const float rel = (float)((double)hook::Global<float>(0x6d172c) - ((double)F<float>(hook::Global<uint8_t*>(0x62be68), 0x10) - D(0x5c96a8)));
    const uint64_t twoPiBits = 0x401921fb54442eeaull;
    double twoPi;
    memcpy(&twoPi, &twoPiBits, 8);
    const float angle = (float)((double(__cdecl*)(double, double))0x5a1390)((double)rel + D(0x5c96f8), twoPi);
    const float gy = (float)((double)y + D(0x5d0a60));
    const float gx = (float)((double)x + D(0x5d1170));
    uint8_t* g = (uint8_t*)(uintptr_t)Table_Entry(hook::Global<uint8_t*>(0x6d1724), 0, 1);
    Glyph_QueueRotated(g, 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, angle, BitsF(0x3ef684be), BitsF(0x3f666666));
}
RT3_REPLACE(0x507a90, Placement_DrawCompassCb);

// 0x507b50. Info window destructor: clears its highlights, releases image
// 0x6d1724 and forgets 0x6d1720.
void __fastcall InfoWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d17c0;
    InfoWindow_ClearHighlights(w);
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, hook::Global<void*>(0x6d1724));
    hook::Global<uint32_t>(0x6d1720) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x507b50, InfoWindow_Destruct);

// 0x507b90. The building dialog's tabs: highlights page 0x622af4 among 0x697c..
// (6); on page 5 shows list 0x69c8 or 0x69c9 by 0x6d1734 (0x6d, else hidden 0x86)
// and takes the selected entry of tree 0x69df / 0x69e3 into +0x8c; buttons 0x6986 /
// 0x6985 by 0x6d1728, and 0x6983 enabled unless new game +0x82 or world +0x4aef.
RT3_NOINLINE void __fastcall BuildingDialog_ShowTabs(uint8_t* w) {
    for (int32_t i = 0; i < 6; ++i) Window_SendEvent5(w, 0, i != hook::Global<int32_t>(0x622af4) ? 0x87 : 0x65, 0x697c + i, 1, 0, 0);
    bool page5 = hook::Global<int32_t>(0x622af4) == 5;
    Window_SendEvent5(w, 0, page5 && hook::Global<int32_t>(0x6d1734) ? 0x6d : 0x86, 0x69c8, 3, 0, 0);
    page5 = hook::Global<int32_t>(0x622af4) == 5;
    Window_SendEvent5(w, 0, page5 && !hook::Global<int32_t>(0x6d1734) ? 0x6d : 0x86, 0x69c9, 3, 0, 0);
    if (hook::Global<int32_t>(0x622af4) == 5) {
        uint8_t* tree = Widget_FindChild(w, 0, 0x69df + (hook::Global<int32_t>(0x6d1734) != 0) * 4);
        if (F<int32_t>(tree, 0x1d6) >= 0) F<uint32_t>(w, 0x8c) = TreeView_SelectedData(tree);
    }
    Window_SendEvent5(w, 0, hook::Global<int32_t>(0x6d1728) ? 0x65 : 0x87, 0x6986, 1, 0, 0);
    Window_SendEvent5(w, 0, hook::Global<int32_t>(0x6d1728) ? 0x87 : 0x65, 0x6985, 1, 0, 0);
    bool on = !hook::Global<uint8_t*>(0x6cec7c)[0x82] && !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef);
    Window_SendEvent5(w, 0, on ? 0x65 : 0x87, 0x6983, 3, 0, 0);
}
RT3_REPLACE(0x507b90, BuildingDialog_ShowTabs);

// 0x507cf0. Building placement: sets the rotation 0x6d172c (and the remembered
// one 0x6d1730 when `keep`), then re-places the building being placed (+0x7c in
// 0x62b26c) at its own cell with that rotation and refreshes its attachments.
RT3_NOINLINE void __fastcall Placement_SetRotation(uint8_t* w, int, uint32_t angle, int32_t keep) {
    hook::Global<uint32_t>(0x6d172c) = angle;
    if (keep) hook::Global<uint32_t>(0x6d1730) = angle;
    Collection* buildings = hook::Global<Collection*>(0x62b26c);
    if (!buildings || !Collection_IsValid(buildings, 0, F<uint32_t>(w, 0x7c))) return;
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(w, 0x7c));
    if (!b) return;
    float x = (float)Obj_CellX(b);
    float z = (float)Obj_CellY(b);
    SceneObj_SetAngle2(b, 0, hook::Global<uint32_t>(0x6d172c));
    Building_MoveTo(b, 0, x, z, 0, 0.0f);
    SceneObj_ResetAttachmentLists(b);
}
RT3_REPLACE(0x507cf0, Placement_SetRotation);

// 0x507d80. Building placement: points the building at the mouse, seen from
// the screen spot (0.926875, 0.7825) (input +0xa94 / +0xa98, y squashed by
// 0.75), relative to the camera heading (+0x10 in 0x62be68), and keeps it.
RT3_NOINLINE void __fastcall Placement_RotateToMouse(uint8_t* w) {
    uint8_t* in = hook::Global<uint8_t*>(0x6d4018);
    float dx = (float)((double)F<float>(in, 0xa94) - D(0x5d17d8));
    float dy = (float)((double)F<float>(in, 0xa98) - D(0x5d17d0));
    dy = -dy;
    dy = (float)((double)dy * D(0x5c8750));
    if (dy == Fl(0x5c8598)) dy = BitsF(0x38d1b717);
    float a = (float)Atan((double)dx / dy);
    if (dy < Fl(0x5c8598)) a = (float)((double)a + Fl(0x5c8760));
    a = (float)((double)F<float>(hook::Global<uint8_t*>(0x62be68), 0x10) - D(0x5c96a8) + a);
    a = (float)CrtFmod((double)a + D(0x5c96f8), BitsD(0x401921fb54442eeaull));
    Placement_SetRotation(w, 0, FBits(a), 1);
}
RT3_REPLACE(0x507d80, Placement_RotateToMouse);

// 0x507e80. Building dialog dlg: can the previewed building (+0x7c of
// 0x62b26c) stand at (x, y)? Checks Sites_CanPlace with its type, angle
// and, for a track-side one (vcall 0x70, +0x2a8 +0x46a), its track piece,
// then tints the preview: red when not, else yellow for track-side
// buildings off their piece or industries without an active site near
// (with message 0x29c), green otherwise; message 0x29b when it can.
// Returns 1 when it can.
RT3_NOINLINE int32_t __fastcall BuildingDialog_CheckSpot(uint8_t* dlg, int, float x, float y, char* msg) {
    if (F<int32_t>(dlg, 0x7c) == -1) {
        if (msg) *msg = 0;
        return 0;
    }
    uint8_t* o = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(dlg, 0x7c));
    uint8_t* ref = (uint8_t*)Obj_Ref3cc(o);
    int32_t side = ((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x1c])(o);
    int32_t onTrack = 0, trackId = 0;
    if (side && ((uint8_t*)Obj_Ref2a8(o))[0x46a]) {
        onTrack = 1;
        trackId = F<int32_t>(Obj_Ref2a8(o), 8);
    }
    int32_t editor = side && ((uint8_t*)Obj_Ref2a8(o))[0x46a] ? 1 : 0;
    int32_t city = Obj_Get21(hook::Global<void*>(0x6cec78));
    float angle = SceneObj_Angle2(o);
    int32_t id = Collection_FindByName4(hook::Global<Collection*>(0x62b2fc), 0, (const char*)ref + 4);
    int32_t ok = Sites_CanPlace(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)id, x, y, angle, city, editor, msg, 0, onTrack, trackId, 0);
    if (!ok) {
        Obj_SetWorld66ba(o, 0, 0xff0000);
        return ok;
    }
    if (msg) strcpy(msg, GetString(0x29b));
    if (side && Obj_Ref2a8(o) && !((uint8_t*)Obj_Ref2a8(o))[0x46a]) {
        Obj_SetWorld66ba(o, 0, 0xffff00);
        return ok;
    }
    if (Obj_StateKindIs4(o) && !Table_ActiveSiteNear(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(dlg, 0x7c), 0)) {
        Obj_SetWorld66ba(o, 0, 0xffff00);
        if (msg) strcpy(msg, GetString(0x29c));
        return ok;
    }
    Obj_SetWorld66ba(o, 0, 0xff00);
    return ok;
}
RT3_REPLACE(0x507e80, BuildingDialog_CheckSpot);

// 0x508040. Building dialog dlg: searches a spot for building type `id`
// near (*x, *y) at angle 0x6d1730 (*angle). Stations first try snapping to
// track within 8 of it (pass 0, unless none is there), then (pass 1, not for
// class 3 / 4 stations) and in pass 2 free spots: rings of radius 0..3 (0.5
// apart, 12 directions) and turns (16 / 12 per circle), each scored by
// distance and turn from the snapped angle; the best placeable one
// (Sites_CanPlace) of a pass is taken.
// QUIRK: the angle's offset from 0x6d1730 is computed (fmod) but unused.
RT3_NOINLINE void __fastcall BuildingDialog_FindSpot(uint8_t* dlg, int, uint32_t id, float* x, float* y, float* angle) {
    *angle = hook::Global<float>(0x6d1730);
    float bx = *x, by = *y, ba = *angle;
    float best = BitsF(0x4e6e6b28);
    int32_t found = 0;
    uint8_t* btype = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, id);
    int32_t station = ((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(btype, 0x173)))[0x32] == 1;
    float snapA = 0.0f;
    int32_t owner = -1;
    if (station && F<int32_t>(dlg, 0x7c) != -1)
        owner = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(dlg, 0x7c)), 0x2a8);
    int32_t pass = 0;
    if (!station || Grid_NearestNodeWithin(hook::Global<Collection*>(0x6cfca8), 0, *x, *y, -1, 8.0f, 1, 0, 0) == -1) pass = 2;
    int32_t node = 0;
    for (; pass < 3; ++pass) {
        if (pass == 1) {
            uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(btype, 0x173));
            if (t[0x32] == 1 && t[0x8c] == 3) continue;
            t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(btype, 0x173));
            if (t[0x32] == 1 && t[0x8c] == 4) continue;
        }
        for (float r = 0.0f;;) {
            for (float th = 0.0f;;) {
                if (!(th > D(0x5c85b8)) || !(r < D(0x5c85b8))) {
                    float step = pass > 1 ? 12.0f : 16.0f;
                    double inc = D(0x5d1800) / step;
                    int32_t first = pass == 0 ? 1 : 0;
                    for (float a = 0.0f;;) {
                        float qx = (float)(((double(__cdecl*)(double))0x5a13e4)(th) * r + *x);
                        float qy = (float)(((double(__cdecl*)(double))0x5a1494)(th) * r + *y);
                        float ang = (float)(a + *angle);
                        float rot = 0.0f, sx, sy;
                        int32_t ok = Station_SnapToTrack(hook::Global<Collection*>(0x62b2fc), 0, owner, id, qx, qy, ang, &sx, &sy, &snapA, first, &node);
                        float px = qx, py = qy;
                        bool eval = pass > 1;
                        if (ok) {
                            if (pass == 0) px = sx, py = sy, ang = snapA;
                            const uint64_t twoPi = 0x401921fb60000000ull;
                            rot = (float)((double(__cdecl*)(double, double))0x5a1390)((double)snapA + Fl(0x5c96e8) - ang, *(const double*)&twoPi);
                            if (rot > Fl(0x5c8760)) rot = (float)(Fl(0x5c8768) - rot);
                            if (hook::Global<float>(0x5c8598) > rot) rot = 0.0f;
                            eval = true;
                        }
                        if (eval) {
                            float d = (float)Distance2D(*x, *y, px, py);
                            float s = (float)((rot + D(0x5c86b0)) * d);
                            s = (float)((rot + D(0x5c8c20)) * s);
                            if (s < best) {
                                uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(btype, 0x173));
                                int32_t tn = t[0x32] == 1 && t[0x8c] == 3 && ok ? node : -1;
                                int32_t city = Obj_Get21(hook::Global<void*>(0x6cec78));
                                if (Sites_CanPlace(hook::Global<Collection*>(0x62b2fc), 0, id, px, py, ang, city, pass <= 1 ? 1 : 0, nullptr, 0, ok, tn, 0)) {
                                    best = s;
                                    found = 1;
                                    bx = px, by = py, ba = ang;
                                }
                            }
                        }
                        a = (float)(a + inc);
                        if (!(a < D(0x5d17f0))) break;
                    }
                }
                th = (float)(th + D(0x5d17e8));
                if (!(th < D(0x5d17f0))) break;
            }
            r = (float)(r + Fl(0x5c8d48));
            if (!(r < D(0x5d17e0))) break;
        }
        if (found) {
            *x = bx, *y = by, *angle = ba;
            return;
        }
    }
}
RT3_REPLACE(0x508040, BuildingDialog_FindSpot);

// 0x5084a0. Rounded value for building `id` (0x62b268, +0x78c set; v1.03+ only):
// above 0x5c8d10 in steps of 100000, else of 50000.
RT3_NOINLINE float __fastcall Building_RoundedValue(uint32_t id, int, float a) {
    if (Game_EffectiveVersion() < 0x67) return *(const float*)0x5c8598;
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, id);
    if (!F<int32_t>(b, 0x78c)) return *(const float*)0x5c8598;
    float v = (float)((double)a * *(const double*)0x5d08a0);
    int32_t n;
    if (a > *(const float*)0x5c8d10) {
        __asm {
            fld v
            fadd dword ptr ds:[0x5c8db0]
            fdiv dword ptr ds:[0x5c8d00]
            mov eax, 0x5a10d0
            call eax
            mov n, eax
        }
        return (float)(n * 100000);
    }
    __asm {
        fld v
        fadd dword ptr ds:[0x5d1808]
        fdiv dword ptr ds:[0x5c8db0]
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    return (float)(n * 50000);
}
RT3_REPLACE(0x5084a0, Building_RoundedValue);

// 0x508550. The building dialog's price line (button 0x6983, string 0x29d): for
// the selected building (0x62b2fc, +0x8c) of type +0x173 (0x62b268) its base price
// (+0x22, unsigned) times the build-cost multiplier (effect 0x3b for type kind
// +0x32 == 1, else 0x3c; for the world's company / player and the map region at the
// cursor 0x6d1738 / 0x6d173c, 0 off the map), plus its rounded value; money style.
RT3_NOINLINE void __fastcall BuildingDialog_ShowPrice(uint8_t* w) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(w, 0x8c))) return;
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(w, 0x8c));
    float x = hook::Global<float>(0x6d1738), y = hook::Global<float>(0x6d173c);
    uint32_t region = 0;
    if (Map_Contains(hook::Global<void*>(0x62c120), 0, x, y)) region = Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(b, 0x173));
    float base = (float)(double)F<uint32_t>(t, 0x22);
    t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(b, 0x173));
    int32_t effect = t[0x32] == 1 ? 0x3b : 0x3c;
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t player = Obj_Get25(world);
    int32_t company = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
    float price = Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, effect, (uint32_t)company, (uint32_t)player, region) * base;
    float total = Building_RoundedValue(F<uint32_t>(b, 0x173), 0, price) + price;
    char num[0x200], text[0x200];
    FormatText(text, GetString(0x29d), Num_FormatSimple2(num, 0xb, total, 0xb), 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x6983, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x508550, BuildingDialog_ShowPrice);

// 0x5086c0. Building placement: turns the building by `steps` sixteenths of a
// turn (0x5d1810 = pi/16) and keeps the new rotation.
RT3_NOINLINE void __fastcall Placement_RotateSteps(void*, int, int32_t steps) {
    float a = (float)((double)steps * D(0x5d1810) + Fl(0x6d172c));
    Placement_SetRotation(hook::Global<uint8_t*>(0x6d1720), 0, FBits(a), 1);
}
RT3_REPLACE(0x5086c0, Placement_RotateSteps);

RT3_NOINLINE int32_t __fastcall Obj_Train2a8(uint8_t* o) {
    if (F<int32_t>(o, 0x7c) == -1) return -1;
    const int32_t v = F<int32_t>((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 0x7c)), 0x2a8);
    return v ? v : -1;
}
RT3_REPLACE(0x508700, Obj_Train2a8);

// 0x508730. Selects building `id` (+0x8c) in the building dialog: its localized
// type name (0x6982; used as a sprintf format), the price line and its 2D model
// (0x6984, event 0x75). Ids <= 0 are special pages: blank texts and one of the
// fixed model names (|id + 11| into a 12-entry table; other ids read past it).
RT3_NOINLINE void __fastcall BuildingDialog_Select(uint8_t* w, int, int32_t id) {
    const char* models[12] = { (const char*)0x5d1848, (const char*)0x5d1848, (const char*)0x5d1848, (const char*)0x5d1848,
                               (const char*)0x5d1848, (const char*)0x5d1848, (const char*)0x5d1848, (const char*)0x5d1830,
                               (const char*)0x5d1824, (const char*)0x5d183c, (const char*)0x5d183c, (const char*)0x5d1818 };
    F<int32_t>(w, 0x8c) = id;
    if (id <= 0) {
        Window_SendEvent5(w, 0, 0x6f, 0x6982, 0x5c87a8, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x6983, 0x5c87a8, 0, 0);
        int32_t k = id + 0xb;
        Window_SendEvent5(w, 0, 0x75, 0x6984, (uint32_t)(uintptr_t)models[k < 0 ? -k : k], 0, 0);
        return;
    }
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)id);
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(b, 0x173));
    char buf[0x1f0];
    ExeSprintf()(buf, LocalizeName((const char*)t + 0x4f));
    Window_SendEvent5(w, 0, 0x6f, 0x6982, (uint32_t)(uintptr_t)buf, 0, 0);
    BuildingDialog_ShowPrice(w);
    BuildingType_Model2D(b, 0, buf);
    Window_SendEvent5(w, 0, 0x75, 0x6984, (uint32_t)(uintptr_t)buf, 0, 0);
}
RT3_REPLACE(0x508730, BuildingDialog_Select);

void __fastcall BuildingDialog_SelectCb(int32_t v, int32_t id) {
    if (v != -1 && hook::Global<uint8_t*>(0x6d1720)) BuildingDialog_Select(hook::Global<uint8_t*>(0x6d1720), 0, id);
}
RT3_REPLACE(0x508860, BuildingDialog_SelectCb);

// 0x508880. Building dialog w: switches to page `page` (0x622af4): enables
// (0x87, pages 3+) or disables (0x65) buttons 0x6988 / 0x6989 / 0x698c; page
// 5 only reselects and closes the overview. Otherwise the page's building
// type name (format table 0x5f3c6c, for pages under 3 combined with the size
// 0x5f3c80[0x622af0] and captioned with 0x620ce8 / 0x29a on button 0x698c)
// selects that type of 0x62b2fc (+0x8c, events 0x69 to 0x69df / 0x69e3),
// refreshes the list (0x6d1720) and tabs, and shows the station overview
// (mode 6) for the station +0x7c, or closes it on pages 3 and 4.
RT3_NOINLINE void __fastcall BuildingDialog_SetPage(uint8_t* w, int, int32_t page) {
    hook::Global<int32_t>(0x622af4) = page;
    uint32_t ev = page > 2 ? 0x87 : 0x65;
    Window_SendEvent5(w, 0, ev, 0x6988, 3, 0, 0);
    Window_SendEvent5(w, 0, ev, 0x6989, 3, 0, 0);
    Window_SendEvent5(w, 0, ev, 0x698c, 3, 0, 0);
    if (hook::Global<int32_t>(0x622af4) == 5) {
        BuildingDialog_ShowTabs(w);
        BuildingDialog_Select(w, 0, F<int32_t>(w, 0x8c));
        Overview_Close(hook::Global<uint8_t*>(0x62c120));
        return;
    }
    int32_t m = hook::Global<int32_t>(0x622af4);
    if (m > 4) m = 4;
    char name[0x100], caption[0x12c];
    FormatText(name, ((const char**)0x5f3c6c)[m], 0, 0, 0, 0, 0, 0, 0, 0);
    if (hook::Global<int32_t>(0x622af4) < 3) {
        int32_t size = hook::Global<int32_t>(0x622af0);
        FormatText(caption, (const char*)0x5c8730, ((const char**)0x5f3c80)[size], name, 0, 0, 0, 0, 0, 0);
        FormatText(name, caption, 0, 0, 0, 0, 0, 0, 0, 0);
        if (hook::Global<int32_t>(0x622aec) == -1) {
            const char* sizeName = GetString(((const int32_t*)0x620ce8)[hook::Global<int32_t>(0x622af0)]);
            FormatText(caption, GetString(0x29a), sizeName, 0, 0, 0, 0, 0, 0, 0);
        } else
            FormatText(caption, GetString(((const int32_t*)0x620ce8)[hook::Global<int32_t>(0x622af0)]), 0, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6f, 0x698c, (uint32_t)(uintptr_t)caption, 0, 0);
    }
    F<int32_t>(w, 0x8c) = -1;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b2fc)); ++i) {
        Collection* types = hook::Global<Collection*>(0x62b2fc);
        uint8_t* rec = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, i, 0));
        if (Stricmp(name, (const char*)rec + 4)) continue;
        F<uint32_t>(w, 0x8c) = Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, i, 0);
        Window_SendEvent5(w, 0, 0x69, 0x69df, Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, i, 0), 0, 0);
        Window_SendEvent5(w, 0, 0x69, 0x69e3, Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, i, 0), 0, 0);
        break;
    }
    BuildingDialog_Select(hook::Global<uint8_t*>(0x6d1720), 0, F<int32_t>(w, 0x8c));
    BuildingDialog_ShowTabs(w);
    if (hook::Global<int32_t>(0x622af4) >= 3) {
        Overview_Close(hook::Global<uint8_t*>(0x62c120));
        return;
    }
    int32_t cargo = -1;
    int32_t st = F<int32_t>(w, 0x7c);
    if (st != -1) {
        int32_t v = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, st), 0x2a8);
        if (v) cargo = v;
    }
    Overview_SetMode(hook::Global<uint8_t*>(0x62c120), 0, 6, cargo, 0, 0, nullptr, 0, 0, 99999, 99999);
}
RT3_REPLACE(0x508880, BuildingDialog_SetPage);

// 0x508b60. Building dialog w: terrain kind at the placement point
// (0x6d1738, 0x6d173c), or the forced kind 0x622aec; on a change (or `force`)
// stores it in 0x622af0 and reshows page 0x622af4 if it is below 3.
RT3_NOINLINE void __fastcall BuildingDialog_UpdateTerrain(uint8_t* w, int, int32_t force) {
    int32_t kind = hook::Global<int32_t>(0x622aec);
    if (kind == -1) kind = Map_TerrainKindAt(hook::Global<void*>(0x62c120), 0, hook::Global<float>(0x6d1738), hook::Global<float>(0x6d173c));
    if (kind == hook::Global<int32_t>(0x622af0) && !force) return;
    hook::Global<int32_t>(0x622af0) = kind;
    if (hook::Global<int32_t>(0x622af4) < 3) BuildingDialog_SetPage(w, 0, hook::Global<int32_t>(0x622af4));
}
RT3_REPLACE(0x508b60, BuildingDialog_UpdateTerrain);

// 0x508bb0. Building dialog dlg: follows the mouse (ev: its control, else the
// place toggle 0x7d6 of the panel). Previews of a different kind are dropped
// first (+0x80 marker, +0x88 scenery effect, +0x84 city, +0x7c a building of
// another type). When placing, the ground under the cursor (0x6d1738 /
// 0x6d173c) gets the price and terrain page, then the preview moves there:
// a marker (-0x12, label 0x29e), a scenery effect (special kinds -0x16..-0xd),
// a city (-0x13, name 0xaa9), else the building at the best spot (unless
// 0x6d1728) with its check message as tooltip. Otherwise all previews hide.
// Then the station overview (mode 6) follows the preview. Returns 1 when not
// placing (or while 0x6d171c is up), else 0.
// QUIRK: hiding the building preview runs twice.
RT3_NOINLINE int32_t __fastcall BuildingDialog_TrackMouse(uint8_t* dlg, int, uint32_t* ev) {
    if (hook::Global<int32_t>(0x6d171c)) return 1;
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x3748] = 1;
    int32_t ret = 1;
    auto vcall = [](Collection* c, int slot, uint32_t id) { ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)c)[slot])(c, 0, id); };
    auto obj = [](uint32_t coll, uint32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(coll), 0, id); };
    auto v0 = [](uint8_t* o, int slot) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[slot])(o); };
    auto moveTo = [](uint8_t* o, float x, float y) { ((void(__fastcall*)(uint8_t*, int, float, float, int32_t))(*(void***)o)[0x14])(o, 0, x, y, 0); };
    if (F<int32_t>(dlg, 0x80) != -1 && F<int32_t>(dlg, 0x8c) != -0x12) {
        vcall(hook::Global<Collection*>(0x6ada80), 2, F<uint32_t>(dlg, 0x80));
        F<int32_t>(dlg, 0x80) = -1;
    }
    if (F<int32_t>(dlg, 0x88) != -1 && !IsSpecialCommand_Impl(F<int32_t>(dlg, 0x8c))) {
        vcall(hook::Global<Collection*>(0x6cea50), 2, F<uint32_t>(dlg, 0x88));
        F<int32_t>(dlg, 0x88) = -1;
    }
    if (F<int32_t>(dlg, 0x84) != -1 && F<int32_t>(dlg, 0x8c) != -0x13) {
        vcall(hook::Global<Collection*>(0x62bae0), 2, F<uint32_t>(dlg, 0x84));
        F<int32_t>(dlg, 0x84) = -1;
    }
    if (F<int32_t>(dlg, 0x7c) != -1) {
        bool keep = false;
        if (F<int32_t>(dlg, 0x8c) >= 0) {
            uint8_t* bt = obj(0x62b2fc, F<uint32_t>(dlg, 0x8c));
            keep = Obj_Ref3cc(obj(0x62b26c, F<uint32_t>(dlg, 0x7c))) == bt;
        }
        if (!keep) {
            vcall(hook::Global<Collection*>(0x62b26c), 2, F<uint32_t>(dlg, 0x7c));
            F<int32_t>(dlg, 0x7c) = -1;
        }
    }
    int32_t active;
    if (ev) active = ev[1] == 0x7d6 ? 1 : 0;
    else {
        uint8_t* w = hook::Global<uint8_t*>(0x6d0818) ? Widget_FindChild(hook::Global<uint8_t*>(0x6d0818), 0, 0x7d6) : nullptr;
        active = w ? (int8_t)w[0x6a] & 4 : 0;
    }
    if (active) {
        ret = 0;
        float x, y;
        if (Scene_FocusFlagIs1(&x, &y)) {
            hook::Global<float>(0x6d1738) = x;
            hook::Global<float>(0x6d173c) = y;
            BuildingDialog_ShowPrice(dlg);
            int32_t kind = hook::Global<int32_t>(0x622aec);
            if (kind == -1) kind = Map_TerrainKindAt(hook::Global<void*>(0x62c120), 0, hook::Global<float>(0x6d1738), hook::Global<float>(0x6d173c));
            if (kind != hook::Global<int32_t>(0x622af0)) {
                hook::Global<int32_t>(0x622af0) = kind;
                if (hook::Global<int32_t>(0x622af4) < 3) BuildingDialog_SetPage(dlg, 0, hook::Global<int32_t>(0x622af4));
            }
            int32_t k = F<int32_t>(dlg, 0x8c);
            uint32_t xb = *(uint32_t*)&x, yb = *(uint32_t*)&y;
            if (k == -0x12) {
                if (F<int32_t>(dlg, 0x80) == -1)
                    F<uint32_t>(dlg, 0x80) = Markers_Add(hook::Global<Collection*>(0x6ada80), 0, xb, yb, GetString(0x29e), 0);
                else {
                    uint8_t* o = obj(0x6ada80, F<uint32_t>(dlg, 0x80));
                    if (v0(o, 5)) v0(o, 3);
                    moveTo(o, x, y);
                }
            } else if (IsSpecialCommand_Impl(k)) {
                if (F<int32_t>(dlg, 0x88) == -1) {
                    static const int8_t codes[10] = { 0xa, 9, 8, -1, -1, 7, 6, 5, 4, 3 };
                    uint32_t i = (uint32_t)(k + 0x16);
                    if (i <= 9 && codes[i] >= 0) F<uint32_t>(dlg, 0x88) = SceneryEffects_Add(hook::Global<Collection*>(0x6cea50), 0, x, y, (uint32_t)codes[i]);
                } else {
                    uint8_t* o = obj(0x6cea50, F<uint32_t>(dlg, 0x88));
                    if (v0(o, 5)) v0(o, 3);
                    moveTo(o, x, y);
                }
            } else if (k == -0x13) {
                if (F<int32_t>(dlg, 0x84) == -1)
                    F<uint32_t>(dlg, 0x84) = Cities_Add(hook::Global<Collection*>(0x62bae0), 0, xb, yb, GetString(0xaa9), 0);
                else {
                    uint8_t* o = obj(0x62bae0, F<uint32_t>(dlg, 0x84));
                    v0(o, 3);
                    moveTo(o, x, y);
                }
            } else {
                if (!hook::Global<int32_t>(0x6d1728)) BuildingDialog_FindSpot(dlg, 0, (uint32_t)k, &x, &y, (float*)0x6d172c);
                if (F<int32_t>(dlg, 0x7c) == -1) {
                    RunDebugValidation();
                    uint8_t* bt = obj(0x62b2fc, F<uint32_t>(dlg, 0x8c));
                    int32_t owner = Obj_Get21(hook::Global<void*>(0x6cec78));
                    F<uint32_t>(dlg, 0x7c) = Buildings_Add(hook::Global<Collection*>(0x62b26c), 0, bt, x, y, 0, owner, 0);
                    uint8_t* o = obj(0x62b26c, F<uint32_t>(dlg, 0x7c));
                    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))(*(void***)o)[0x16])(o, 0, 0, 0xff, 0);
                    Placement_SetRotation(dlg, 0, hook::Global<uint32_t>(0x6d172c), 0);
                    RunDebugValidation();
                } else {
                    uint8_t* o = obj(0x62b26c, F<uint32_t>(dlg, 0x7c));
                    if (v0(o, 5)) v0(o, 3);
                    Building_MoveTo(o, 0, x, y, 0, 0.0f);
                    Placement_SetRotation(dlg, 0, hook::Global<uint32_t>(0x6d172c), 0);
                    SceneObj_ResetAttachmentLists(o);
                }
                char msg[0x200];
                BuildingDialog_CheckSpot(dlg, 0, x, y, msg);
                Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, msg, 1, 1, -1);
            }
        }
    } else {
        if (F<int32_t>(dlg, 0x7c) != -1) v0(obj(0x62b26c, F<uint32_t>(dlg, 0x7c)), 4);
        if (F<int32_t>(dlg, 0x7c) != -1) v0(obj(0x62b26c, F<uint32_t>(dlg, 0x7c)), 4);
        if (F<int32_t>(dlg, 0x80) != -1) v0(obj(0x6ada80, F<uint32_t>(dlg, 0x80)), 4);
        if (F<int32_t>(dlg, 0x88) != -1) v0(obj(0x6cea50, F<uint32_t>(dlg, 0x88)), 4);
        if (F<int32_t>(dlg, 0x84) != -1) v0(obj(0x62bae0, F<uint32_t>(dlg, 0x84)), 4);
    }
    if (hook::Global<int32_t>(0x622af4) < 3) {
        int32_t st = -1;
        if (F<int32_t>(dlg, 0x7c) != -1)
            if (int32_t s = F<int32_t>(obj(0x62b26c, F<uint32_t>(dlg, 0x7c)), 0x2a8)) st = s;
        Overview_SetMode(hook::Global<uint8_t*>(0x62c120), 0, 6, st, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
    }
    return ret;
}
RT3_REPLACE(0x508bb0, BuildingDialog_TrackMouse);

 // namespace

// 0x5091b0 (event handler). The building placement dialog d (building +0x7c
// in 0x62b26c, kind +0x8c; nothing while a placement request +0x94 is out).
// Idle / mouse moves (0 / 2) without a company (and not in the editor) leave
// the tool; while the engine is busy (0x6d401c +0xc64) it only clears the
// highlights every 500 ms; otherwise the ghost follows the focused cell
// (rotating while +0x90), re-tracked when it moved 0.1 or changed. Clicks:
// pages 0x697c..0x6981 (the region page 5 when the company may not build
// there, strings 0xb1e / 0x26b), the auto-spot toggles 0x6985 / 0x6986,
// rotate 0x6987 and place 0x7d6: city renames (kinds -0x12 / -0x13, strings
// 0x2a0 / 0x2a1), else the spot check (sound 0x30), the cost (type +0x22
// times the build-cost effect 0x3b / 0x3c by +0x32, plus the rounding) against
// the cash plus 500 (string 0xc6f, with 0xc71 when +0x4a8b is clear; free in
// the editor for +0x32 0 / 2, in a tutorial and with +0x4aef), and the
// placement (in multiplayer a 0x12 request to the host). Buttons: the two
// terrain lists 0x69e0 / 0x69e1 swap and the terrain steps 0x6988 / 0x6989
// cycle 0x622aec through -1..5.
int32_t __fastcall BuildingDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (F<int32_t>(d, 0x94)) return 1;
    if (hook::Global<Collection*>(0x62b26c) && Collection_IsValid(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c)))
        Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c));
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    auto noCompany = []() {
        return !Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, (uint32_t)Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) &&
               !F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68);
    };
    switch (msg[0]) {
    case 0:
        if (noCompany()) {
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
            return 1;
        }
        if (F<int32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc64) > 0) {
            F<int32_t>(d, 0x90) = 0;
            if (ClockMs() - hook::Global<int32_t>(0x6d1744) > 0x1f4) InfoWindow_ClearHighlights(d);
            hook::Global<int32_t>(0x6d1744) = ClockMs();
            return 1;
        }
        if (F<int32_t>(d, 0x90)) return 1;
        // fall through
    case 2: {
        if (F<int32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc64) > 0) return 1;
        if (noCompany()) return 1;
        if (F<int32_t>(d, 0x90)) Placement_RotateToMouse(d);
        float x, y;
        const int32_t focus = Scene_FocusFlagIs1(&x, &y);
        if (focus == hook::Global<int32_t>(0x6d1740)) {
            if (!focus) return 1;
            if (!(Distance2D(x, y, hook::Global<float>(0x622af8), hook::Global<float>(0x622afc)) > D(0x5c86b8))) return 1;
        }
        hook::Global<float>(0x622af8) = x;
        hook::Global<float>(0x622afc) = y;
        hook::Global<int32_t>(0x6d1740) = focus;
        return BuildingDialog_TrackMouse(d, 0, nullptr);
    }
    case 0xca: {
        if (F<int32_t>(d, 0x94) || hook::Global<int32_t>(0x6cd8e8)) return 1;
        const uint32_t id = msg[1];
        if (id == 0x6985 || id == 0x6986) {
            hook::Global<int32_t>(0x6d1728) = id == 0x6986;
            BuildingDialog_ShowTabs(d);
            return 0;
        }
        if (id == 0x6987) {
            F<int32_t>(d, 0x90) = 1;
            Placement_RotateToMouse(d);
            return 0;
        }
        if (id >= 0x697c && id <= 0x6981) {
            int32_t page = (int32_t)id - 0x697c;
            uint32_t text = 0;
            if ((!Obj_Get21(w) || F<int32_t>(w, 0x4ab3)) && page != 5) {
                page = 5;
                text = Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) ? 0x26b : 0xb1e;
            }
            w = hook::Global<uint8_t*>(0x6cec78);
            if (F<int32_t>(w, 0x4ab7) && F<int32_t>(w, 0x4abf) && page == 5) {
                page = hook::Global<int32_t>(0x622af4);
                text = 0x26b;
            }
            BuildingDialog_SetPage(d, 0, page);
            if (text) Message477(GetString(text), 1, 0, 0x1e);
            return 0;
        }
        if (id != 0x7d6) return 1;
        float x = hook::Global<float>(0x622af8), y = hook::Global<float>(0x622afc);
        if (!hook::Global<int32_t>(0x6d1740)) return 0;
        const int32_t kind = F<int32_t>(d, 0x8c);
        if (IsSpecialCommand_Impl(kind)) {
            F<int32_t>(d, 0x88) = -1;
            return 0;
        }
        if (kind == -0x12) {
            if (!Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(d, 0x80))) return 0;
            Rename477(d, 0x80, 0x2a0, hook::Global<Collection*>(0x6ada80), 0x2a2);
            --hook::Global<int32_t>(0x6d171c);
            return 0;
        }
        if (kind == -0x13) {
            if (!Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(d, 0x84))) return 0;
            Rename477(d, 0x84, 0x2a1, hook::Global<Collection*>(0x62bae0), 0x356);
            Cities_AssignBlocks(hook::Global<Collection*>(0x62bae0));
            --hook::Global<int32_t>(0x6d171c);
            return 0;
        }
        if (kind < 0) return 0;
        if (!hook::Global<int32_t>(0x6d1728)) BuildingDialog_FindSpot(d, 0, (uint32_t)kind, &x, &y, (float*)0x6d172c);
        if (!BuildingDialog_CheckSpot(d, 0, x, y, nullptr)) {
            Ui_PlaySound(0x30, 0, 0x3f000000);
            return 0;
        }
        float cost = (float)F<uint32_t>(Type477(d), 0x22);
        const uint32_t size = Type477(d)[0x32];
        const uint32_t region = Map_GridByte(hook::Global<uint8_t*>(0x62c120), 0, x, y);
        const uint32_t player = (uint32_t)Obj_Get25(hook::Global<uint8_t*>(0x6cec78));
        const uint32_t company = (uint32_t)Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
        cost = Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, size == 1 ? 0x3b : 0x3c, company, player, region) * cost;
        cost = Building_RoundedValue(F<uint32_t>(Type477(d), 0xe4), 0, cost) + cost;
        bool charge = Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) != 0;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68) && (size == 2 || size == 0)) {
            charge = false;
        } else if (charge) {
            const float cash = (float)Company_Stat((uint8_t*)Obj_Company21(hook::Global<uint8_t*>(0x6cec78)), 0, 0xd, 0x2329);
            if (cash + BitsF(0x43fa0000) < cost && !hook::Global<uint8_t*>(0x6cec7c)[0x82] &&
                !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) {
                char need[0xc8], have[0xc8], text[0x1f4];
                Num_FormatSimple2(need, 0xb, cost, 0xb);
                Num_FormatSimple2(have, 0xb, cash, 0xb);
                FormatText(text, GetString(0xc6f), need, have, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a8b)) strcat(text, GetString(0xc71));
                Message477(text, 1, 0, 0x1e);
                return 0;
            }
        }
        RunDebugValidation();
        if (IsMultiplayer()) {
            struct {
                float x, y, angle;
                int32_t company, kind, flag, peer;
            } req;
            req.peer = Global6d40e4();
            req.x = x, req.y = y;
            req.angle = SceneObj_Angle2((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c)));
            req.company = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
            req.kind = F<int32_t>(d, 0x8c);
            uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c));
            req.flag = ((int32_t(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x1c](b) &&
                               ((uint8_t*)Obj_Ref2a8(Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c))))[0x46a]
                           ? 1
                           : 0;
            F<int32_t>(d, 0x94) = 1;
            Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 6, 0, 0);
            b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(d, 0x7c));
            ((void(__fastcall**)(uint8_t*))F<void**>(b, 0))[4](b);
            Net_SendToHost(hook::Global<uint8_t*>(0x6cd8d8), 0, 0x12, 0x1c, (const uint8_t*)&req);
            return 0;
        }
        const uint32_t building = F<uint32_t>(d, 0x7c);
        F<int32_t>(d, 0x7c) = -1;
        const uint32_t owner = charge ? (uint32_t)Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) : 0;
        if (!Building_Place((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, building), 0, x, y, owner)) {
            F<uint32_t>(d, 0x7c) = building;
            return 0;
        }
        RunDebugValidation();
        if (hook::Global<int32_t>(0x622af4) < 3)
            Overview_SetMode(hook::Global<uint8_t*>(0x62c120), 0, 6, Obj_Train2a8(d), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
        Ui_PlaySound(0x2f, 0, 0x3f000000);
        return 0;
    }
    case 0xcb: {
        F<int32_t>(d, 0x90) = 0;
        if (F<int32_t>(d, 0x94) || hook::Global<int32_t>(0x6cd8e8)) return 1;
        const uint32_t id = msg[1];
        if (id == 0x69e0 || id == 0x69e1) {
            const bool first = id == 0x69e1;
            const uint32_t from = first ? 0x69e3 : 0x69df, to = first ? 0x69df : 0x69e3;
            Window_SendEvent5(d, 0, 0x66, to, F<uint32_t>(Widget_FindChild(d, 0, (int32_t)from), 0x1d6), 0, 0);
            Window_SendEvent5(d, 0, 0x6d, first ? 0x69c9 : 0x69c8, 3, 0, 0);
            Window_SendEvent5(d, 0, 0x86, first ? 0x69c8 : 0x69c9, 3, 0, 0);
            return 0;
        }
        if (id == 0x6988 || id == 0x6989) {
            const int32_t v = hook::Global<int32_t>(0x622aec) + (id == 0x6989 ? 1 : -1);
            hook::Global<int32_t>(0x622aec) = v;
            if (v < -1)
                hook::Global<int32_t>(0x622aec) = 5;
            else if (v >= 6)
                hook::Global<int32_t>(0x622aec) = -1;
            BuildingDialog_UpdateTerrain(d, 0, 1);
            return 0;
        }
        return 1;
    }
    default:
        return 1;
    }
}
RT3_REPLACE(0x5091b0, BuildingDialog_HandleEvent);

// 0x509d80. Building dialog constructor ("StationPlace.win", vtable 0x5d17c0,
// registered at 0x6d1720, frame image "StationPlace.imb" at 0x6d1724): stops
// following, nothing picked (+0x7c..+0x88 -1), the first building type of
// 0x62b2fc preselected (+0x8c). In multiplayer or with option +0x82 the two
// lists (0x69e3 / 0x69df) get the extra entries -0x12..-0xd (-0x13 only
// without the option). Then every building type the player may build is
// added to both lists, stations first (tag -0x967, with their size name),
// the rest after (tag -1); the first added one is selected. Finally the
// handlers, the brush radius 2 and page 0x622af4 (5 outside a scenario or
// with +0x4ab3) are set up, and the terrain under the cursor (0x6d1738 /
// 0x6d173c) is remembered in 0x622af0 unless 0x622aec fixes it.
// QUIRK: the localized type name is used as the sprintf format.
uint8_t* __fastcall BuildingDialog_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d17c0;
    World_SetFollow(hook::Global<uint8_t*>(0x6cec78), 0, 0, nullptr);
    hook::Global<uint8_t*>(0x6d1720) = w;
    hook::Global<uint8_t*>(0x6d1724) = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)0x5d1870);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d185c);
    F<int32_t>(w, 0x7c) = -1;
    F<int32_t>(w, 0x80) = -1;
    F<int32_t>(w, 0x88) = -1;
    F<int32_t>(w, 0x84) = -1;
    F<uint32_t>(w, 0x8c) = Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, 0, 0);
    F<int32_t>(w, 0x90) = 0;
    F<int32_t>(w, 0x94) = 0;
    Window_SendEvent5(w, 0, 0x7a, 0x6987, 0x507a90, 0, 0);
    if (Global6d07b4Set2() || F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0x82)) {
        static const int32_t extra[][2] = { { 0x296, -0x12 }, { 0x297, -0x13 }, { 0xc00, -0x14 }, { 0xc01, -0x15 }, { 0xc02, -0x16 },
                                            { 0xc03, -0xd },  { 0xc04, -0xe },  { 0xc05, -0xf },  { 0xc06, -0x10 }, { 0xc07, -0x11 } };
        for (int k = 0; k < 10; ++k) {
            if (k == 1 && F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0x82)) continue;
            Window_SendEvent5(w, 0, 0x88, 0x69e3, (uint32_t)(uintptr_t)GetString(extra[k][0]), (uint32_t)extra[k][1], 0);
            Window_SendEvent5(w, 0, 0x88, 0x69df, (uint32_t)(uintptr_t)GetString(extra[k][0]), (uint32_t)extra[k][1], 0);
        }
    }
    bool selected = false;
    for (int32_t pass = 0; pass < 2; ++pass) {
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b2fc)); ++i) {
            Collection* list = hook::Global<Collection*>(0x62b2fc);
            uint8_t* e = (uint8_t*)Collection_Get(list, 0, Collection_IdAt(list, 0, (uint32_t)i, 0));
            uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(e, 0x173));
            if (pass == 0 ? t[0x32] != 4 : t[0x32] == 4) continue;
            if (!Global6d07b4Set2()) {
                if (!Obj_TypeAvailable(e) || F<int32_t>(e, 0xe3)) continue;
                if (((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(e, 0x173)))[0x32] == 1) continue;
                if (!F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(e, 0x173)), 0x3b)) continue;
                if (t[0x32] == 2 && !F<int32_t>(t, 0x7ac)) continue;
            }
            uint8_t* g = hook::Global<uint8_t*>(0x6cec74);
            if (F<int32_t>(g, 0x267) && (t[0x32] == 1 || t[0x32] == 0 || F<int32_t>(t, 0x794) > 0)) continue;
            if (!F<uint8_t>(g, 0x2d0) && F<int32_t>(e, 0xe3)) continue;
            uint8_t* o = hook::Global<uint8_t*>(0x6cec78);
            if (F<int32_t>(o, 0x4ab7) && t[0x32] == 4 && t[0xb9] <= 3) continue;
            if (t[0xb9] == 4) continue;
            if (F<int32_t>(o, 0x4abf) && t[0x32] == 2) continue;
            char name[300];
            const char* loc = LocalizeName((const char*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(e, 0x173)) + 0x4f);
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, loc);
            uint32_t entry[8] = {};
            String_Assign((char**)&entry[0], name);
            entry[2] = Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, 0);
            entry[3] = pass ? (uint32_t)-1 : (uint32_t)-0x967;
            const char* size = (const char*)0x5d1858;
            list = hook::Global<Collection*>(0x62b2fc);
            uint8_t* t2 = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0,
                                                   F<uint32_t>(Collection_Get(list, 0, Collection_IdAt(list, 0, (uint32_t)i, 0)), 0x173));
            if (t2[0x32] == 4) {
                list = hook::Global<Collection*>(0x62b2fc);
                uint8_t* t3 = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0,
                                                       F<uint32_t>(Collection_Get(list, 0, Collection_IdAt(list, 0, (uint32_t)i, 0)), 0x173));
                switch ((int8_t)t3[0xb9]) {
                case 0: size = GetString(0x374); break;
                case 1: size = GetString(0x375); break;
                case 2: size = GetString(0x376); break;
                case 3: size = GetString(0xefa); break;
                case 4: size = GetString(0x372); break;
                }
            }
            String_Assign((char**)&entry[1], size);
            Window_SendEvent5(w, 0, 0x89, 0x69e3, (uint32_t)(uintptr_t)entry, 0, 0);
            Window_SendEvent5(w, 0, 0x89, 0x69df, (uint32_t)(uintptr_t)entry, 0, 0);
            Free((void*)(uintptr_t)entry[0]);
            Free((void*)(uintptr_t)entry[1]);
            if (!selected) {
                selected = true;
                Window_SendEvent5(w, 0, 0x68, 0x69e3, Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, 0), 0, 0);
                Window_SendEvent5(w, 0, 0x68, 0x69df, Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, 0), 0, 0);
            }
        }
    }
    Window_SendEvent5(w, 0, 0x79, 0x69e3, 0x508860, 0, 0);
    Window_SendEvent5(w, 0, 0x79, 0x69df, 0x508860, 0, 0);
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, 2.0f);
    if (!Obj_Get21(hook::Global<void*>(0x6cec78)) || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4ab3)) hook::Global<int32_t>(0x622af4) = 5;
    BuildingDialog_SetPage(w, 0, hook::Global<int32_t>(0x622af4));
    BuildingDialog_ShowTabs(w);
    int32_t kind = hook::Global<int32_t>(0x622aec);
    if (kind == -1)
        kind = Map_TerrainKindAt(hook::Global<void*>(0x62c120), 0, hook::Global<float>(0x6d1738), hook::Global<float>(0x6d173c));
    hook::Global<int32_t>(0x622af0) = kind;
    if (hook::Global<int32_t>(0x622af4) < 3) BuildingDialog_SetPage(w, 0, hook::Global<int32_t>(0x622af4));
    return w;
}
RT3_REPLACE(0x509d80, BuildingDialog_Construct);

}  // namespace rt3
