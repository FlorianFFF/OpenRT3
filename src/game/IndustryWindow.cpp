// IndustryWindow.cpp - RT3.exe 0x4b6000-0x4be000.
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
#include "core/stream.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/crt.h"
#include "core/imports.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

// Rewritten functions called directly.
int32_t Obj_KindE8Special();

extern int32_t g_unwrittenReads[8];  // src/game/Train.cpp, before Train_Run

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void* __fastcall Array33_At(void* a, int, int32_t i);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
uint32_t __stdcall HeatColor(float t);
int32_t IsMultiplayer();
double __fastcall Effect_Blend(uint8_t* e, int, float t);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
int32_t ClockMs();
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
void* __fastcall Obj_Ref3cc(void* o);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
float __fastcall Building_Price(uint8_t* b, int, uint32_t company);
uint32_t __fastcall CargoTypes_NthFlagged(int32_t n);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
void* OrderEdit_NextActiveSite();
uint8_t* __fastcall Obj_BuildingRegion(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetVtbl(uint8_t* w);
void World_RefreshColors();
void __fastcall Map_ClearFogMode(uint8_t* m);
void __fastcall Window_HighlightTab7(uint8_t* w);
void __fastcall Window_HighlightViewTabs(uint8_t* w);
void __fastcall Map_SetFogMode(uint8_t* m, int, int32_t mode);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall MapTool_SetMode(uint8_t* t, int, int32_t mode);
void __fastcall Window_HighlightGridTabs(uint8_t* w);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
double __fastcall Obj_Float360(void* o, int, int32_t i);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Scene_PushBusy(uint8_t* s);
void __fastcall Scene_PopBusy(uint8_t* s);
int32_t __fastcall Obj_YearReached(void* o);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Obj_StateB9(void* o);
uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
double __fastcall Obj_Float242(void* o, int, int32_t i);
int32_t __fastcall Track_Link(uint8_t* t, int, int32_t second);
void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y);
void __fastcall Model_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Model_SetPartAngle2(uint8_t* o, int, int32_t part, uint32_t v);
void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
void __fastcall TrainOrders_UpdateButtons(uint8_t* w);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall IndustryEditor_ShowTab(uint8_t* w, int, int32_t tab);
int32_t __fastcall Collection_Count(Collection* c);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall File_Exists(const char* path);
uint32_t __fastcall Sound_Get2c(uint8_t* s);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
int32_t __fastcall Rand(int32_t lo, int32_t hi);


int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t Global6d40e4();
void __fastcall Music_Close(uint8_t* m);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
int32_t __fastcall Widget_IsVisible(uint8_t* w);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
uint8_t* __fastcall IndustryEditor_Construct(uint8_t* w);
int32_t __fastcall Train_Rank(void* self);
int32_t __fastcall Obj_Get25(void* o);
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
void __fastcall Obj_Incc60(uint8_t* o);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
int32_t __fastcall Obj_NameTableIndex(uint8_t* o);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void __fastcall IndustryView_UpdateButtons(uint8_t* w);
void __fastcall Company_InvestInBuilding(void* c, int, uint32_t building, float amount);
void __fastcall Company_ReleaseBuilding(void* c, int, uint32_t building, float price);
void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price);
void* __fastcall Train_Company(void* t);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_StopMusic(uint8_t* s, int, uint32_t notify);
int32_t NewGameConfig_FirstOpenSlot();
void __fastcall Campaign_Refresh(uint8_t* d);
int32_t __fastcall Game_EffectiveVersion();
void* __fastcall Array33_Current(void* a);
void* __fastcall Obj_CityRef(void* o);

int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign,
                                  int32_t anyTrack);

uint32_t __fastcall Company_Logo(uint8_t* c);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
uint8_t __fastcall Engine_KeyState(uint8_t* e, int, uint8_t k);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void* __fastcall Obj_Company21(void* o);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
int32_t Tutorial_Is19a();
void OrderEdit_ApplyToAll();
void* __fastcall Obj_City41(void* o);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);
void* __fastcall Obj_Ref388(void* o);
void* __fastcall Obj_City352(void* o);
uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alpha, int32_t deep);
int32_t SelectedTrainId();
uint32_t __fastcall Obj_GridByte(void* o);
uint32_t __fastcall Obj_CityGridByte(void* o);
void __fastcall Terrain_PaintPoints(uint8_t* t, int, float x, float y, float r, int32_t erase, int32_t kind, int32_t variant, float density, int32_t force);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall Obj_SetWaterPreset(uint8_t* o, int, int32_t idx);
void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
void __fastcall Map_SmoothAround(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, float strength);
void __fastcall Map_LevelAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float useCentre, float strength);
void __fastcall Map_RefreshBuildingsAround(uint8_t* o, int, int32_t x, int32_t y, int32_t r);
void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
int32_t __fastcall SceneObj_Transform(uint8_t* o, int, float* x, float* y, float* z);
void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
uint8_t __fastcall Channel_Playing(uint8_t* c);
void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
void __fastcall Train_ShiftGear(uint8_t* o, int, int32_t delta);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall IndustryWindow_Refresh(uint8_t* w);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Overview_Close(uint8_t* t);
double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Glyph_QueueRotated(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, float angle, float px, float py);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
double One();
int32_t __fastcall OrderList_CountPlain(uint8_t* o);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
uint32_t __stdcall OverlayColor(float t);

float __fastcall Train_ConsistWeight(uint8_t* t, int, int32_t quiet, int32_t slope, int32_t curves, int32_t withEngines, float* meanBend, float* maxBend,
                                     float* meanTilt);

float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region);
int32_t __fastcall Train_IsStopped(uint8_t* t);
int32_t __fastcall Train_StateBOr12(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall CampaignWindow_Destruct(uint8_t* w);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
uint8_t* __fastcall CampaignWindow_Construct(uint8_t* w);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall BuildingPacket_Invest(uint32_t* pkt);
void __fastcall BuildingPacket_Release(uint32_t* pkt);
void __fastcall BuildingPacket_Take(uint32_t* pkt);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall TrainList_DrawEntry(uint8_t* t, uint8_t highlight, float x, float y, float w, float h);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode, int32_t paused, void* data);
int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
void Trains_SelectNextFwd();
void Trains_SelectNextBack();
int32_t __fastcall Music_IsPlaying(uint8_t* m);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void Tutorial_Advance();
void __fastcall Industry_DrawProduction(uint8_t* o, int, float x, float y, float w, int32_t, int32_t);
void __fastcall BuildingPanel_DrawLine(uint8_t* b, int32_t kind, float x, float y, float w, float h, int32_t);
void __stdcall View_DrawSelectionLabel(float x, float y, float w, float h, int32_t);
void __fastcall IndustryFinance_Draw(uint8_t* ind, int, float x, float y, float w, float h, int32_t);
void __fastcall BuildingView_DrawIncomeTable(uint8_t* b, int32_t kind, float x, float y, float, float, int32_t);
void __fastcall Brush_SetSizeF621cc8(uint8_t* o, int, int32_t i);
void __fastcall Brush_SetSizeF621ecc(uint8_t* o, int, int32_t i);
void __fastcall Terrain_AdjustLevel(uint8_t* m, int, float x, float y, float r, float amount);
void __fastcall Consist_SendCb(void*, int, int32_t on);
void __fastcall IndustryEditor_Build(uint8_t* w);
void __fastcall IndustryEditor_BuildCargoPanel(uint8_t* w);
void __fastcall IndustryEditor_Run(uint32_t a, uint8_t* rec, uint32_t x);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void World_RefreshColors();
RT3_NOINLINE int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
RT3_NOINLINE void __fastcall Map_SmoothAround(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, float strength);
RT3_NOINLINE void __fastcall Map_LevelAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float useCentre, float strength);
RT3_NOINLINE void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
RT3_NOINLINE void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void ModelPreview_Close();
RT3_NOINLINE void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
RT3_NOINLINE int32_t __fastcall Node_Advance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* outPos, int32_t* dir, int32_t* count, float* accum, int32_t car, int32_t, int32_t front);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
RT3_NOINLINE void __fastcall Industry_DrawProduction(uint8_t* o, int, float x, float y, float w, int32_t, int32_t);
RT3_NOINLINE void __fastcall IndustryFinance_Draw(uint8_t* ind, int, float x, float y, float w, float h, int32_t);
void __fastcall BuildingDetail_DrawCb(int32_t id, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall Trains_SelectNext(int32_t dir);
RT3_NOINLINE void Trains_SelectNextFwd();
RT3_NOINLINE void Trains_SelectNextBack();
RT3_NOINLINE void __fastcall IndustryView_UpdateButtons(uint8_t* w);
RT3_NOINLINE void __fastcall IndustryWindow_Refresh(uint8_t* w);
void __fastcall Boat_Update(uint8_t* b, int, int32_t now);
int32_t __fastcall IndustryWindow_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint8_t* __fastcall IndustryWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall BuildingPacket_Invest(uint32_t* pkt);
RT3_NOINLINE void __fastcall BuildingPacket_Release(uint32_t* pkt);
RT3_NOINLINE void __fastcall BuildingPacket_Take(uint32_t* pkt);
void __fastcall Bulldoze_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Brush_SetSizeF621cc8(uint8_t* o, int, int32_t i);
RT3_NOINLINE void __fastcall Window_HighlightTab7(uint8_t* w);
int32_t __fastcall Bulldozer_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall Bulldoze_Construct(uint8_t* w);
void __fastcall Campaign_HoverSoundCb(void*, int32_t quiet, int32_t on, int32_t, int32_t, int32_t, int32_t);
int32_t __fastcall Campaign_HoverCb4(uint8_t* o, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
int32_t __fastcall Campaign_HoverCbA(uint8_t* o, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
void __fastcall Campaign_ScenarioHoverCb(int32_t id, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Campaign_Refresh(uint8_t* d);
void Campaign_RefreshOpen();
void __fastcall CampaignWindow_Destruct(uint8_t* w);
uint8_t* __fastcall CampaignWindow_Construct(uint8_t* w);
int32_t __fastcall CampaignScreen_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE uint32_t __fastcall CargoTypes_NthFlagged(int32_t n);
RT3_NOINLINE void __fastcall TrainOrders_UpdateButtons(uint8_t* w);
RT3_NOINLINE int32_t Global6cfe00Set();
void SetGlobal6cfe10();
void* OrderEdit_NextActiveSite();
RT3_NOINLINE void __fastcall Consist_SendCb(void*, int, int32_t on);
RT3_NOINLINE void OrderEdit_ApplyToAll();
void __fastcall TrainOrders_CargoTooltip(int32_t id, int, int32_t, int32_t);
void __fastcall OrderEdit_OnSlider(uint8_t* ctrl, int32_t value);
RT3_NOINLINE void __fastcall Car_UpdatePose(uint8_t* car, int, float dist);
RT3_NOINLINE void __fastcall IndustryEditor_BuildCargoPanel(uint8_t* w);
RT3_NOINLINE void __fastcall IndustryEditor_ShowTab(uint8_t* w, int, int32_t tab);
RT3_NOINLINE void __fastcall IndustryEditor_Build(uint8_t* w);
int32_t __fastcall TrainOrders_HandleEvent(uint8_t* w, int, uint32_t* msg);
uint8_t* __fastcall IndustryEditor_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall IndustryEditor_Run(uint32_t a, uint8_t* rec, uint32_t x);
void __fastcall ChangeHeight_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Obj_SetWaterPreset(uint8_t* o, int, int32_t idx);
void __fastcall Obj_GetDims(uint8_t* o, int, int32_t* a, int32_t* b, float* c);
RT3_NOINLINE void __fastcall Window_HighlightViewTabs(uint8_t* w);
int32_t __fastcall TerrainTools_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint8_t* __fastcall ChangeHeight_Construct(uint8_t* w);
void __fastcall ChangeTrees_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Brush_SetSizeF621ecc(uint8_t* o, int, int32_t i);
RT3_NOINLINE void __fastcall MapTool_SetMode(uint8_t* t, int, int32_t mode);
RT3_NOINLINE void __fastcall Window_HighlightGridTabs(uint8_t* w);
uint8_t* __fastcall ChangeTrees_Construct(uint8_t* w);
int32_t __fastcall HeightBrush_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE uint32_t __stdcall OverlayColor(float t);
RT3_NOINLINE void __fastcall Widget_DeleteGeneratedRows(uint8_t* w, int, int32_t);
RT3_NOINLINE void __fastcall TrainList_DrawEntry(uint8_t* t, uint8_t highlight, float x, float y, float w, float h);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
RT3_NOINLINE void __fastcall Scene_PushBusy(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_PopBusy(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline float Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline double Fl_205(uint32_t a) { return *(const float*)rt3::Rdata(a); }


static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }


// Map tool windows pause the 3D scene's busy depth while open; their destructors
// pop it.
static void Scene_PopEngineBusy() { Scene_PopBusy((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine))); }



// Front / rear bogie heading of Car_UpdatePose: near the end of the node the
// heading is blended with the next node's (wrapping across +-pi).
static float Car_BogieHeading(int32_t node, float at, int32_t dir, float heading, bool rear) {
    Collection* nodes = g_tracks;
    bool low;
    if (!rear) low = dir ? !(at < D(0x5c97d8)) : (at <= D(0x5c97d8));
    else low = dir ? (at <= D(0x5c97d8)) : !(at < D(0x5c97d8));
    int32_t flagged = low ? 0 : 1;
    // the front bogie looks along its direction when low, the rear one against it
    int32_t arg = rear ? (low ? dir == 0 : dir) : (low ? dir : dir == 0);
    int32_t nb = Track_Link((uint8_t*)Collection_Get(nodes, 0, (uint32_t)node), 0, arg);
    if (nb == -1) return heading;
    uint8_t* m = (uint8_t*)Collection_Get(nodes, 0, (uint32_t)nb);
    int32_t fwd = 0;
    if (!flagged) {
        if (F<int32_t>(m, 0x206) == node || (F<int32_t>(m, 0x20e) == node && (m[0x201] & 2))) fwd = 1;
    } else {
        if (F<int32_t>(m, 0x20a) == node || (F<int32_t>(m, 0x20e) == node && !(m[0x201] & 2))) fwd = 1;
    }
    float hn = Vehicle_ScreenHeading(m, 0, fwd);
    if (hn == heading) return heading;
    double w = at;
    if (!(at < D(0x5c97d8))) w = D(0x5c97e0) - at;
    float wf = (float)(w / D(0x5c97d8) * D(0x5c8628) + D(0x5c8628));
    if ((double)heading + D(0x5c96a8) < hn) hn = (float)((double)hn - D(0x5c96b8));
    else if ((double)heading - D(0x5c96a8) > hn) hn = (float)((double)hn + D(0x5c96b8));
    return (float)((D(0x5c8770) - wf) * hn + (double)wf * heading);
}

// Height of the car at `at` on `node`, rounded to 1/100 in multiplayer games
// with version 1.02 everywhere.
static float Car_PoseHeight(uint8_t* node, float pos) {
    float y = (float)(Effect_Blend(node, 0, (float)((double)pos / D(0x5c97e0))) + D(0x5c8750));
    if (g_world && IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f828f5c))) {
        float r = (float)Ftol((double)y * Fl_205(0x5c8c28));
        y = (float)((double)r / Fl_205(0x5c8c28));
    }
    return y;
}

static void Campaign_Speak(uint8_t* d, const char* file) {
    Music_Play(g_6cfd34, 0, file, (void*)(uintptr_t)Sound_Get2c(g_sound), nullptr);
    d[0x7e] = 1;
}

static inline double Fl_207(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

// Shared tail of the building packets: the sender gets the normal cursor
// back, and an open industry window 0x6cfd04 is refreshed.
static void BuildingPacket_Done(uint32_t player) {
    if (Obj_Get25(g_world) == (int32_t)player) Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    if (uint8_t* w = g_industryWindow) IndustryWindow_Refresh(w);
}

// (int)(v + 0.5) with the x87 sum and the exe's _ftol.
static int32_t TerrainTools_Round(float v) {
    int32_t n;
    __asm {
        fld v
        fadd qword ptr ds:[0x5c8628]
        call Rt3_Ftol
        mov n, eax
    }
    return n;
}

namespace {
inline float BoatF(uint32_t a) { return *(const float*)rt3::Rdata(a); }
inline double BoatD(uint32_t a) { return *(const double*)rt3::Rdata(a); }
inline uint32_t BoatBits(float f) { return *(uint32_t*)&f; }
inline uint8_t BoatVk(char c) { return (uint8_t)imp::VkKeyScanA(c); }
inline bool BoatKey(char c) { return Engine_KeyState(g_engine, 0, BoatVk(c)) != 0; }
inline void BoatKeyUp(char c) { Engine_KeyUp(g_engine, 0, BoatVk(c)); }

// One float of the hull (o at offset pos, y lowered by 0x5c89a4): depth d
// below the water (clamped below at -0.5, counted when not), its cube and
// roll moment summed, and the float raised by min(d, 3) and tipped by angle.
struct BoatSum {
    float* lat;
    float* cube;
    float* roll;
    float* depth;
    int32_t* count;
};
void BoatFloat(uint8_t* b, uint8_t* o, const uint8_t* pos, const BoatSum& s, float tilt, int32_t k) {
    float x, y = (float)((double)F<float>((void*)pos, 4) - BoatF(0x5c89a4)), z;
    memcpy(&x, pos, 4);
    memcpy(&z, pos + 8, 4);
    if (!SceneObj_Transform(o, 0, &x, &y, &z)) return;
    float d = (float)(Terrain_HeightAt(F<uint8_t*>(b, 0x206), 0, x, z) - y);
    if (d < BoatD(0x5ca180)) F<uint32_t>(&d, 0) = 0xbf000000;
    else ++*s.count;
    *s.cube = (float)((double)d * d * d + *s.cube);
    *s.roll = (float)((double)*s.lat * d + *s.roll);
    if (d > BoatF(0x5c8900)) F<uint32_t>(&d, 0) = 0x40400000;
    *s.depth = d + *s.depth;
    *s.lat = *s.lat + BoatF(0x5c85b4);
    float angle = (float)((double)tilt * BoatD(0x5c8d68) + (double)k);
    SceneObj_SetAngles(o, 0, BoatBits(angle), 0, 0);
    float ny = d + F<float>((void*)pos, 4);
    SceneObj_SetPos(o, 0, F<uint32_t>((void*)pos, 0), BoatBits(ny), F<uint32_t>((void*)pos, 8));
}
// A pontoon (o): as a float, but only its depth (>= 0) counts.
void BoatPontoon(uint8_t* b, uint8_t* o, float lat, float* cube, bool first, int32_t* count, float* depth, float* roll) {
    float x, y, z;
    SceneObj_GetPos2(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    y = (float)((double)y - BoatF(0x5c8898));
    if (!SceneObj_Transform(o, 0, &x, &y, &z)) return;
    float d = (float)(Terrain_HeightAt(F<uint8_t*>(b, 0x206), 0, x, z) - y);
    double c = (double)d * d * d;
    *cube = first ? (float)c : (float)(c + *cube);
    if (d < BoatF(0x5c8598)) F<uint32_t>(&d, 0) = 0;
    else *count = 1;
    F<uint32_t>(depth, 0) = F<uint32_t>(&d, 0);
    *roll = first ? (float)-((double)lat * d) : (float)(*roll - (double)lat * d);
}
}

namespace {
// The industry's type record (vtable slot 0x80) and its localized name.
inline uint8_t* IndType(uint8_t* ind) { return ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)ind)[0x80 / 4])(ind); }
inline const char* IndName(uint8_t* ind) { return LocalizeName((const char*)IndType(ind) + 0x4f); }
inline uint32_t IndMsg(const char* text, int32_t type) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// A confirmed deal: done locally (refreshing on 0x6cfd04) or sent to the
// host as {player, company, industry, amount}.
template <typename Fn> void IndDeal(uint8_t* w, uint32_t id, float amount, uint32_t msg, Fn local) {
    if (!IsMultiplayer()) {
        local(Obj_Company21(hook::As<void*>(g_world)), 0, id, amount);
        if (hook::As<int32_t>(g_industryWindow)) IndustryWindow_Refresh(w);
        return;
    }
    uint32_t data[4];
    data[0] = (uint32_t)Obj_Get25(hook::As<void*>(g_world));
    data[1] = F<uint32_t>(Obj_Company21(hook::As<void*>(g_world)), 0);
    data[2] = id;
    data[3] = *(uint32_t*)&amount;
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    Net_SendToHost(g_session, 0, msg, 0x10, (const uint8_t*)data);
}
}

namespace {
inline double RowD(uint32_t a) { return *(const double*)rt3::Rdata(a); }
inline float RowF(uint32_t a) { return *(const float*)rt3::Rdata(a); }
inline uint8_t* RowGlyph(int32_t k) { return (uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(g_world, 0x46a84), 0, k); }
inline void RowIcon(uint8_t* g, float x, float y) {
    Glyph_Queue(g, 0, x, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
// Status icon k left of x (moving x), at y - 0.001.
inline void RowStatus(float& x, float y, int32_t k) {
    double right = x;
    x = (float)(right - (Slider_StepA(RowGlyph(k)) + RowD(0x5caad0)));
    float gy = (float)((double)y - RowD(0x5c8678));
    RowIcon(RowGlyph(k), x, gy);
}
inline float RowBits(uint32_t b) { return *(const float*)&b; }
}

namespace {
// Plays narration `file` on the campaign voice 0x6cfd34 (flag d +0x7e).
void CampaignVoice462(uint8_t* d, const char* file) {
    if (!g_6cfd34) return;
    void* digital = (void*)(uintptr_t)Sound_Get2c(g_sound);
    Music_Play(g_6cfd34, 0, file, digital, nullptr);
    d[0x7e] = 1;
}
// One of the narration clips table[Rand(0, n)] in pattern 0x5d05a8.
void CampaignClip462(char* buf, uint32_t table, int32_t n) {
    FormatText(buf, (const char*)0x5d05a8, *(const char* const*)(table + Rand(0, n) * 4), 0, 0, 0, 0, 0, 0, 0);
}
// The next idle tip (0x621d80 by +0x81, pattern 0x5d04ec), cycling 0..3.
void CampaignTip462(uint8_t* d, char* buf) {
    crt::sprintf(buf, (const char*)0x5d04ec, *(const char* const*)(0x621d80 + F<int32_t>(d, 0x81) * 4));
    CampaignVoice462(d, buf);
    if (++F<int32_t>(d, 0x81) > 3) F<int32_t>(d, 0x81) = 0;
}
void ResetScenarioButtons462(uint8_t* d) {
    for (int32_t k = 0; k < 4; ++k) Window_SendEvent5(d, 0, 0x86, 0xc368 + k, 3, 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xc392, 0x5c87a8, 0, 0);
}
}

// 0x4b6090. The industry finance tab (selected industry, view +0x90): once per
// opening (0x6cfd08) a row of buttons 0xbd24.. for the stations that serve it
// (image by the cargo kind bit and the station size, tooltip 0x371 with the
// station name; their ids go to 0x6cfccc); then, unless the industry is of
// kind 3 / 4 (texts 0xefa / 0x372), a table of up to five columns (labels,
// the last three years, total 0x263) with revenue (in thousands scaled by
// kind), costs, profit and the remaining value, negatives red; footer
// 0x374..0x376 by kind.
RT3_NOINLINE void __fastcall IndustryFinance_Draw(uint8_t* ind, int, float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    int32_t kind = Obj_StateB9(ind);
    char buf[0x200];
    if (!g_6cfd08) {
        Window_DeleteChildren(g_industryWindow, 0, 0xbd24, 0xbd2d, 0);
        float cx = x;
        int32_t n = 0;
        Collection* sts = g_stations;
        for (int32_t i = 0; i < Collection_Count(sts); ++i) {
            uint8_t* st = (uint8_t*)Collection_Get(sts, 0, Collection_IdAt(sts, 0, i, 0));
            bool found = false;
            if (Obj_Array590(st, 0, kind) >= 1)
                for (int32_t k = 1;;) {
                    Pairs_At(st, 0, kind, k);
                    if (F<int32_t>(Pairs_At(st, 0, kind, k), 0) == F<int32_t>(ind, 0x2a4)) {
                        found = true;
                        break;
                    }
                    ++k;
                    if (k > Obj_Array590(st, 0, kind)) break;
                }
            if (!found) continue;
            uint32_t mask = F<uint32_t>(Obj_Ref3cc(Collection_Get(g_buildings, 0, F<uint32_t>(st, 4))), 0xc3);
            int32_t c = 0;
            while (c < 6 && !(mask & (1u << c))) ++c;
            uint8_t* b = (uint8_t*)Obj_Building(st);
            uint8_t* q = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
            int32_t sub = 2 - q[0x8c];
            uint8_t def[0x89];
            memset(def, 0, sizeof def);
            F<float>(def, 0xb) = (float)((double)sub * D(0x5caad0) + ((double)y - D(0x5d0270)));
            F<float>(def, 7) = cx;
            int32_t img = hook::Global<int32_t>(0x621cac + c * 4) + sub * 6;
            F<float>(def, 0xf) = (float)Slider_StepA((uint8_t*)(uintptr_t)Table_Entry(g_6cfd18, 0, img));
            F<uint16_t>(def, 0x5f) = (uint16_t)(sub * 6 + hook::Global<uint16_t>(0x621cac + c * 4));
            F<uint32_t>(def, 0x13) = 0x3d3851ec;
            def[0x29] = 0x20;
            F<uint16_t>(def, 0) = (uint16_t)(n + 0xbd24);
            String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0258);
            FormatText(buf, GetString(0x371), (const char*)(st + 0x46b), 0, 0, 0, 0, 0, 0, 0);
            String_Assign(&F<char*>(def, 0x3f), buf);
            uint8_t* s = (uint8_t*)crt::operator_new(0x111);
            s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
            Window_AddChild(g_industryWindow, 0, s, 0);
            crt::free(F<void*>(def, 0x43));
            crt::free(F<void*>(def, 0x3f));
            int32_t img2 = hook::Global<int32_t>(0x621cac + c * 4) + sub * 6;
            cx = (float)(Slider_StepA((uint8_t*)(uintptr_t)Table_Entry(g_6cfd18, 0, img2)) + cx + D(0x5d0250));
            hook::Global<int32_t>(0x6cfccc + n * 4) = F<int32_t>(st, 0);
            ++n;
        }
        g_6cfd08 = 1;
    }
    if (kind == 4 || kind == 3) {
        Font_DrawTextBox(g_6cfcf8, 0, GetString(kind == 4 ? 0x372 : 0xefa), x, y, w, h, 0xffffffff, 0, 0, 0, 1.0f);
        return;
    }
    float scale = kind == 0 ? 600.0f : kind == 1 ? 1800.0f : kind == 2 ? 300.0f : 1.0f;
    float colX = (float)((double)x + D(0x5d0110));
    float rowY = y;
    uint8_t* font = g_6cfcfc;
    static const uint32_t widths[5] = { 0x43610000, 0x43110000, 0x43110000, 0x43110000, 0x431b0000 };
    static const int32_t labels[5] = { 3, 0x303, 0x25f, 0x302, 0x262 };
    int32_t cols = F<uint16_t>(g_world, 0xd) - F<int32_t>(ind, 0x3d5) + 2;
    if (cols > 5) cols = 5;
    for (int32_t col = 0; col < cols; ++col) {
        float cwf = (float)((double)BitsF(widths[col]) / D(0x5ca2c8));
        double cw = cwf;
        int32_t align = (col != 0) + 2;
        rowY = y;
        for (int32_t row = 0; row < 5; ++row) {
            uint32_t flags = 0, color = 0xffffffff;
            double xoff = 0.0;
            if (col == 0) {
                strcpy(buf, GetString(labels[row]));
                xoff = BitsD(0x3fa3333333333333ull);
            } else if (row == 0) {
                if (col == 4) FormatText(buf, GetString(0x263), 0, 0, 0, 0, 0, 0, 0, 0);
                else Num_Format(buf, 8, (double)(F<uint16_t>(g_world, 0xd) - col + 1), 10, 1, 0, 0);
            } else {
                float v = 0.0f;
                if (col == 4) {
                    if (row <= 2) v = (float)(Obj_Float310(ind, 0, 0) + F<float>(ind, 0x2b4));
                    else if (row == 3) v = (float)-(Obj_Float360(ind, 0, 0) + F<float>(ind, 0x2bc));
                    else v = (float)(Obj_Remaining(ind, 0, 0) + (((double)F<float>(ind, 0x2b4) - F<float>(ind, 0x2b8)) - F<float>(ind, 0x2bc)));
                } else {
                    if (row <= 2) v = (float)Obj_Float310(ind, 0, col - 1);
                    else if (row == 3) v = (float)-Obj_Float360(ind, 0, col - 1);
                    else v = (float)Obj_Remaining(ind, 0, col - 1);
                }
                if (row == 1) {
                    Num_Format(buf, 1, (double)Ftol(((double)v + Fl_205(0x5c85b4)) / scale), 10, 1, 0, 0);
                    if (v < Fl_205(0x5c8598)) color = 0xffff5f5f;
                    else if (v < Fl_205(0x5cb45c)) color = 0xffff5f5f;
                } else {
                    Num_FormatSimple(buf, 0xb, (double)v, 0xb);
                    if (row == 3) flags = 4;
                    if (v < Fl_205(0x5cb45c)) color = 0xffff5f5f;
                }
            }
            float tw = (float)(cw + xoff);
            Font_DrawTextBox(font, 0, buf, colX, rowY, tw, BitsF(0x3cbc6a7f), color, align, 0, flags, 1.0f);
            rowY = (float)((double)rowY + Fl_205(0x5d0248));
            if (row == 3) rowY = (float)((double)rowY + D(0x5d0240));
        }
        colX = (float)((double)colX + cw + D(0x5d00e8));
    }
    if (kind == 0) FormatText(buf, GetString(0x374), 0, 0, 0, 0, 0, 0, 0, 0);
    else if (kind == 1) FormatText(buf, GetString(0x375), 0, 0, 0, 0, 0, 0, 0, 0);
    else if (kind == 2) FormatText(buf, GetString(0x376), 0, 0, 0, 0, 0, 0, 0, 0);
    float fh = (float)(((double)h + D(0x5ca2d8)) - ((double)rowY - y));
    float fy = (float)((double)rowY - D(0x5cab60));
    Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a9c), 0, buf, x, fy, w, fh, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4b6090, IndustryFinance_Draw);

// 0x4b6870 (draw callback). Building detail of the selected building
// (0x6d0818 +0x90 in 0x62b26c): its owner's logo (half size, right
// aligned), its type's localised name (QUIRK: used as the format; string
// 0x377 when under construction +0x246), its cargo count (+0x3d5, string
// 0x378) and the body for its kind (Obj_NameTableIndex; production with
// 0x6cfd0a, else lines / labels / finances / income tables). Widget 0xbd17
// asks for the compact form.
void __fastcall BuildingDetail_DrawCb(int32_t id, int, float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    const int32_t compact = id == 0xbd17 ? 1 : 0;
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90));
    float logoW = 0.0f;
    if (const uint32_t owner = F<uint32_t>(b, 0x276)) {
        uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, owner);
        const float gy = (float)((double)y + D(0x5d0290));
        logoW = BitsF(0x3d2e147b);
        const float gx = (float)((double)x + w - D(0x5d0238) - D(0x5d0288));
        Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, gx, gy, 0x3f800000, 0.5f, 0.5f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    char frame[0x13b4];
    char* text = frame + 0x2c;
    char* num = frame + 0xc;
    auto type = [&]() { return ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b); };
    crt::sprintf(text, LocalizeName((const char*)type() + 0x4f));
    if (F<uint32_t>(b, 0x246)) strcat(text, GetString(0x377));
    Font_DrawTextBox(g_6cfcf8, 0, text, x, y, w, BitsF(0x3ccccccd), 0xffffffff, 0, 1, 0, 1.0f);
    Num_Format(num, 8, (double)F<int32_t>(b, 0x3d5), 10, 1, 0, 0);
    FormatText(text, GetString(0x378), num, 0, 0, 0, 0, 0, 0, 0);
    const float cw = (float)((double)w - D(0x5d0280));
    Font_DrawTextBox(g_6cfd00, 0, text, (float)((double)x - logoW), y, cw, BitsF(0x3ccccccd), 0xffffffff, 3, 1, 0, 1.0f);
    float by = (float)((double)y + Fl_207(0x5d0100));
    by = (float)((double)by + D(0x5d0278));
    const int32_t kind = Obj_NameTableIndex(type());
    const float bh = (float)((double)h - ((double)by - y));
    if (g_6cfd0a) {
        Industry_DrawProduction(b, kind, x, by, w, (int32_t)Bits(bh), compact);
        return;
    }
    switch (kind) {
    case 0:
    case 1:
    case 18:
    case 19:
    case 20:
    case 21:
        BuildingView_DrawIncomeTable(b, kind, x, by, w, bh, compact);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 22:
        BuildingPanel_DrawLine(b, kind, x, by, w, bh, compact);
        break;
    case 11:
    case 12:
        View_DrawSelectionLabel(x, by, w, bh, compact);
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        IndustryFinance_Draw(b, kind, x, by, w, bh, compact);
        break;
    }
}
RT3_REPLACE(0x4b6870, BuildingDetail_DrawCb);

// 0x4b6e50. Steps the selected train (0x6d0818 +0x90) by `dir` to the next
// one of the same company (+0x276) that is not hidden (vcall 0x70), wrapping
// around: closes the model preview, selects it and - unless the camera is
// busy (+0x60) - points the camera at it (mode option +0x315) and opens its
// view (vcall 0x5c).
RT3_NOINLINE void __fastcall Trains_SelectNext(int32_t dir) {
    Collection* trains = g_buildings;
    if (!Collection_IsValid(trains, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    int32_t sel = F<int32_t>(g_gameWindow, 0x90);
    if (sel <= 0 || !Collection_IsValid(g_buildings, 0, (uint32_t)sel)) return;
    int32_t at = (int32_t)Collection_PositionOf(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90), 0);
    int32_t owner = F<int32_t>(Collection_Get(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90)), 0x276);
    int32_t i = (Collection_Count(g_buildings) + at + dir) % Collection_Count(g_buildings);
    while (i != at) {
        Collection* c = g_buildings;
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x1c])(o) && F<int32_t>(o, 0x276) == owner) {
            ModelPreview_Close();
            Panel_SetValue90(g_gameWindow, 0, (int32_t)Collection_IdAt(g_buildings, 0, (uint32_t)i, 0));
            if (F<int32_t>(g_62be68, 0x60)) return;
            Camera_FollowObject(o, F<int32_t>(g_app, 0x315));
            ((void(__fastcall*)(uint8_t*))(*(void***)o)[0x17])(o);
            return;
        }
        i = (Collection_Count(g_buildings) + i + dir) % Collection_Count(g_buildings);
    }
}
RT3_REPLACE(0x4b6e50, Trains_SelectNext);

// 0x4b6fc0 / 0x4b6fd0. Next / previous train.
RT3_NOINLINE void Trains_SelectNextFwd() { Trains_SelectNext(1); }
RT3_REPLACE(0x4b6fc0, Trains_SelectNextFwd);

RT3_NOINLINE void Trains_SelectNextBack() { Trains_SelectNext(-1); }
RT3_REPLACE(0x4b6fd0, Trains_SelectNextBack);

// 0x4b6fe0. The industry view's buttons (stamped +0x78 with the clock) for the
// selected building (0x6d0818 +0x90 in 0x62b26c): 0xbd2e enabled when world
// +0x4af7 and option +0x68 are set. "Buy <name> for $N" (string 0x37b, money
// style) on 0xbd19, enabled unless that pair is set, the building is owned
// (+0x276), its type can't be bought (+0x3f) or is kind 4, buying is off (world
// +0x4ac3) or there is no company.
RT3_NOINLINE void __fastcall IndustryView_UpdateButtons(uint8_t* w) {
    F<int32_t>(w, 0x78) = ClockMs();
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90));
    auto type = [&]() { return ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b); };
    auto locked = []() { return F<int32_t>(g_world, 0x4af7) && F<int32_t>(g_app, 0x68); };
    Window_SendEvent5(w, 0, locked() ? 0x65 : 0x87, 0xbd2e, 3, 0, 0);
    if (locked() || F<int32_t>(b, 0x276) || !F<int32_t>(type(), 0x3f) || F<int32_t>(g_world, 0x4ac3) ||
        Obj_StateKindIs4(b) || !Obj_Get21(g_world)) {
        Window_SendEvent5(w, 0, 0x87, 0xbd19, 3, 0, 0);
        return;
    }
    float price = Building_Price(b, 0, (uint32_t)Obj_Get21(g_world));
    Window_SendEvent5(w, 0, 0x65, 0xbd19, 3, 0, 0);
    const char* name = LocalizeName((const char*)type() + 0x4f);
    char num[0x190], text[0x190];
    FormatText(text, GetString(0x37b), name, Num_FormatSimple(num, 9, price, 0xb), 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6e, 0xbd19, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4b6fe0, IndustryView_UpdateButtons);

// 0x4b71b0. Industry window w for the selected building (view +0x90 of
// 0x62b26c): enables the production tabs 0xbd1d / 0xbd1e unless it is a
// non-producing kind (recipe +0x32 0 / 2, or name class 2, 8, 9, 0x12),
// clears its row widgets (0xbd42..0xbda5, 0xbd24..0xbd2d) and the overview
// mode, and captions the buy button 0xbd1b (0x379: price = recipe +0x43 x
// effects 0x3c, for a buyable industry of the player's company) and the sell
// button 0xbd1a (0x37a: value +0x27a) or disables them.
RT3_NOINLINE void __fastcall IndustryWindow_Refresh(uint8_t* w) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90));
    auto recipe = [&] { return ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b); };
    uint8_t kind = recipe()[0x32];
    hook::As<uint32_t>(g_6cfd14) = 0;
    int32_t cls = Obj_NameTableIndex(recipe());
    bool fixed = kind == 0 || kind == 2 || cls == 8 || cls == 9 || cls == 2 || cls == 0x12;
    Window_SendEvent5(w, 0, g_6cfd0a ? 0x65 : 0x87, 0xbd1d, 0x20, 0, 0);
    uint32_t ev = fixed ? 0x65 : 0x87;
    Window_SendEvent5(w, 0, ev, 0xbd1d, 3, 0, 0);
    Window_SendEvent5(w, 0, ev, 0xbd1e, 3, 0, 0);
    if (!fixed) g_6cfd0a = 0;
    Window_DeleteChildren(w, 0, 0xbd42, 0xbda5, 0);
    g_6cfd09 = 0;
    Window_DeleteChildren(w, 0, 0xbd24, 0xbd2d, 0);
    g_6cfd08 = 0;
    Overview_Close(g_map);
    char text[0x200], num[0x200];
    if (!F<int32_t>(b, 0x246) && F<int32_t>(recipe(), 0x4b) && F<int32_t>(b, 0x276) && F<int32_t>(b, 0x276) == Obj_Get21(g_world)) {
        Window_SendEvent5(w, 0, 0x65, 0xbd1b, 3, 0, 0);
        float base = (float)F<int32_t>(recipe(), 0x43);
        uint32_t player = (uint32_t)Obj_Get25(g_world);
        uint32_t company = (uint32_t)Obj_Get21(g_world);
        float price = (float)(Effects_Multiplier(g_world, 0, 0x3c, company, player, 0) * base);
        const char* nm = LocalizeName((const char*)recipe() + 0x4f);
        const char* sp = Num_FormatSimple(num, 9, price, 0xb);
        FormatText(text, GetString(0x379), nm, sp, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6e, 0xbd1b, (uint32_t)(uintptr_t)text, 0, 0);
    } else
        Window_SendEvent5(w, 0, 0x87, 0xbd1b, 3, 0, 0);
    if (F<int32_t>(b, 0x276) && F<int32_t>(b, 0x276) == Obj_Get21(g_world) && !Obj_StateKindIs4(b)) {
        Window_SendEvent5(w, 0, 0x65, 0xbd1a, 3, 0, 0);
        const char* nm = LocalizeName((const char*)recipe() + 0x4f);
        const char* sp = Num_FormatSimple(num, 9, (double)F<float>(b, 0x27a) * D(0x5c8628), 0xb);
        FormatText(text, GetString(0x37a), nm, sp, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(w, 0, 0x6e, 0xbd1a, (uint32_t)(uintptr_t)text, 0, 0);
    } else
        Window_SendEvent5(w, 0, 0x87, 0xbd1a, 3, 0, 0);
    IndustryView_UpdateButtons(w);
}
RT3_REPLACE(0x4b71b0, IndustryWindow_Refresh);

 // namespace

// 0x486240 (vtable 0x5cf048 slot 0). Boat b at time now (ms): dt = elapsed /
// 0x5c86a0 (+0x3e8 keeps the time; nothing more while hidden, +0x20 bit 1).
// Two pontoons (+0x23a / +0x23e) and two rows of floats (+0x24a at +0x24b /
// +0x27b, +0x30b at +0x30c / +0x33c) sample the water depth, giving buoyancy
// (sum of cubes, capped at 2048), roll and pitch targets (atan of depth
// differences) that steer the attitude rates +0x424 / +0x428 / +0x42c
// while any part is wet, and the attitude +0x418 / +0x41c / +0x420. Keys:
// A / D turn (+0x428), W throttles up by the gear table 0x621a08[+0x430]
// (else the throttle +0x3e0 eases back to 300), S runs the speed boost
// 0x6cfc94, Y / H shift gear, C toggles following the boat, J / L and I / K
// swing the crane (+0x3f4, +0x3f8; sounds +0x3d8 / +0x3dc), space spouts the
// fountain (+0x3ec, horn +0x3d4). The engine sounds and wake follow the speed
// +0x3e4; the boat moves by its heading and stays above the ground + 4.
// The wake's vertical rate (effect +0x74) is taken from the local that holds
// the gear-table value, which only the W path writes, so otherwise the
// original stores a stack leftover there; here it is 0 (counted, see Train_Run).
void __fastcall Boat_Update(uint8_t* b, int, int32_t now) {
    int32_t elapsed = now - F<int32_t>(b, 0x3e8);
    float fe = (float)elapsed;
    F<int32_t>(b, 0x3e8) = now;
    float dt = (float)((double)fe / BoatF(0x5c86a0));
    if (b[0x20] & 2) return;
    float yawA = (float)((double)F<float>(b, 0x3f0) + F<float>(b, 0x41c));
    float lat = (float)(-(int32_t)(b[0x24a] >> 1));
    float yawB = (float)((double)F<float>(b, 0x3f0) - F<float>(b, 0x41c));
    float depthB = 0.0f, depthA = 0.0f, roll = 0.0f, cube = 0.0f;
    int32_t countA = 0, countB = 0;
    BoatPontoon(b, F<uint8_t*>(b, 0x23a), lat, &cube, true, &countA, &depthA, &roll);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x23a), 0, BoatBits((float)((double)yawA * BoatD(0x5c8dc0))), 0, 0);
    BoatPontoon(b, F<uint8_t*>(b, 0x23e), lat, &cube, false, &countB, &depthB, &roll);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x23e), 0, BoatBits((float)((double)yawB * BoatD(0x5c8dc0))), 0, 0);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x242), 0, BoatBits((float)((double)yawA * BoatF(0x5c8554))), 0, 0);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x246), 0, BoatBits((float)((double)yawB * BoatF(0x5c8554))), 0, 0);
    if (uint8_t* p = F<uint8_t*>(b, 0x232)) {
        F<float>(p, 0x89) = (float)((double)yawA * BoatD(0x5cf0f8));
        F<float>(F<uint8_t*>(b, 0x236), 0x89) = (float)((double)yawB * BoatD(0x5cf0f8));
    }
    BoatSum a = { &lat, &cube, &roll, &depthA, &countA };
    for (int32_t k = 0; k < b[0x24a]; ++k) BoatFloat(b, F<uint8_t*>(b, 0x24b + k * 4), b + 0x27b + k * 0xc, a, yawA, k);
    if (countA) depthA = (float)((double)depthA / (float)countA);
    lat = (float)(-(int32_t)(b[0x30b] >> 1));
    BoatSum s = { &lat, &cube, &roll, &depthB, &countB };
    for (int32_t k = 0; k < b[0x30b]; ++k) BoatFloat(b, F<uint8_t*>(b, 0x30c + k * 4), b + 0x33c + k * 0xc, s, yawB, k);
    if (countB) depthB = (float)((double)depthB / (float)countB);
    float parts = (float)(b[0x30b] + b[0x24a] + 2);
    roll = (float)((double)roll / parts);
    float pitchT = (float)(crt::atan(((double)depthB - depthA) / BoatF(0x5c8c40)) * BoatD(0x5c8c20));
    float rollT = (float)(crt::atan((double)roll / BoatF(0x5c8c40)) * BoatD(0x5c8c20));
    bool wet = countB || countA;
    if (wet) {
        F<float>(b, 0x424) = (float)(crt::sin(F<float>(b, 0x418)) * BoatD(0x5c8918) + F<float>(b, 0x424) + rollT);
        F<float>(b, 0x424) = (float)((double)F<float>(b, 0x424) * BoatD(0x5c8628));
        F<float>(b, 0x428) = (float)((double)F<float>(b, 0x428) * BoatD(0x5c8750));
        double pt = pitchT;
        float r = (float)(pt - crt::sin((double)F<float>(b, 0x420) + BoatF(0x5c8760)) * BoatD(0x5c8918) + F<float>(b, 0x42c));
        F<float>(b, 0x42c) = (float)((double)r * BoatD(0x5c8628));
    }
    F<float>(b, 0x418) = F<float>(b, 0x424) + F<float>(b, 0x418);
    F<float>(b, 0x420) = F<float>(b, 0x42c) + F<float>(b, 0x420);
    F<float>(b, 0x41c) = F<float>(b, 0x428) + F<float>(b, 0x41c);
    float& lift = g_6cfc98;
    lift = (float)((double)lift - BoatD(0x5cf0f0));
    if (cube > BoatF(0x5cf0e8)) F<uint32_t>(&cube, 0) = 0x45000000;
    if (BoatKey('a')) {
        if (F<float>(b, 0x428) > BoatD(0x5cf0e0))
            F<float>(b, 0x428) = (float)(F<float>(b, 0x428) - (double)dt * BoatD(0x5c8758));
        if (F<float>(b, 0x3e0) < BoatF(0x5ca1c8)) F<float>(b, 0x3e0) = (float)((double)dt * BoatF(0x5c85b0) + F<float>(b, 0x3e0));
    } else if (BoatKey('d')) {
        if (F<float>(b, 0x428) < BoatD(0x5cecd0)) F<float>(b, 0x428) = (float)((double)dt * BoatD(0x5c8758) + F<float>(b, 0x428));
        if (F<float>(b, 0x3e0) < BoatF(0x5ca1c8)) F<float>(b, 0x3e0) = (float)((double)dt * BoatF(0x5c85b0) + F<float>(b, 0x3e0));
    }
    if (cube > BoatF(0x5c8598)) {
        lift = (float)((double)lift / ((double)cube * BoatD(0x5c8988)));
        lift = (float)((double)cube * BoatD(0x5cf0d8) + lift);
    }
    float headingCos = (float)crt::sin(F<float>(b, 0x418));
    float gearStep = 0.0f; // the original leaves this local unwritten unless W runs
    bool gearSet = false;
    if (BoatKey('w')) {
        if (int32_t gear = F<int32_t>(b, 0x430)) {
            float g = ((const float*)0x621a08)[gear < 0 ? -gear : gear];
            if (gear < 0) g = -g;
            gearStep = g;
            gearSet = true;
            float target = (float)(F<float>(b, 0x3e0) - (double)g * F<float>(b, 0x3e4) * BoatF(0x5cf0d0));
            float th = (float)(F<float>(b, 0x3e0) - (double)target * dt * BoatF(0x5c8554));
            F<float>(b, 0x3e0) = th;
            if (th < BoatF(0x5c87c0)) F<uint32_t>(b, 0x3e0) = 0x43960000;
            float eff = (float)(BoatD(0x5cf0c0) - crt::fabs((double)BoatF(0x5ca190) - F<float>(b, 0x3e0)) * BoatD(0x5cf0c8));
            headingCos = (float)(headingCos - (double)eff * g);
        } else if (F<float>(b, 0x3e0) < BoatF(0x5c8c04)) {
            F<float>(b, 0x3e0) = (float)((double)dt * BoatF(0x5c8c04) + F<float>(b, 0x3e0));
        }
    } else if (F<float>(b, 0x3e0) < BoatF(0x5c87c0)) {
        F<uint32_t>(b, 0x3e0) = 0x43960000;
    } else {
        F<float>(b, 0x3e0) = (float)(F<float>(b, 0x3e0) - (double)dt * BoatF(0x5ca1c8));
    }
    float& boost = g_6cfc94;
    if (BoatKey('s')) {
        if (boost < BoatF(0x5c86a0)) boost = (float)((double)dt * BoatF(0x5c8c28) + boost);
    } else {
        F<uint32_t>(&boost, 0) = 0;
    }
    hook::As<uint32_t>(g_6cfc90) = F<uint32_t>(b, 0x3e4);
    float bk = (float)((double)boost + BoatF(0x5c85b4));
    double mix = (crt::exp(((double)boost + BoatF(0x5c85b4)) / BoatF(0x5cb45c)) - BoatD(0x5c8770)) * headingCos + (double)bk * F<float>(b, 0x3e4);
    F<float>(b, 0x3e4) = (float)(crt::exp((double)bk / BoatF(0x5cb45c)) * mix / ((double)boost + BoatF(0x5c85b4)));
    if (BoatKey('y')) {
        BoatKeyUp('y');
        Train_ShiftGear(b, 0, 1);
    } else if (BoatKey('h')) {
        BoatKeyUp('h');
        Train_ShiftGear(b, 0, -1);
    }
    float wake = (float)(crt::fabs((double)F<float>(b, 0x3e4) - g_6cfc90) * ((double)F<float>(b, 0x3e0) / BoatF(0x5ca06c)));
    float wf = (float)((double)wake * BoatD(0x5c8988) + BoatD(0x5c8d68));
    float we = (float)((double)wake + BoatF(0x5c8554));
    int32_t rate = (int32_t)((BoatD(0x5c8578) - crt::fabs(F<float>(b, 0x3e4))) * F<float>(b, 0x3e0));
    View_SetParams(F<uint8_t*>(b, 0x216), 0, rate, 0, 0x40000000, 0x3e99999a, BoatBits(we), BoatBits(wf));
    if (!wet) {
        F<uint32_t>(b, 0x428) = 0;
    } else {
        F<float>(b, 0x424) = (float)(F<float>(b, 0x424) - ((double)g_6cfc90 - F<float>(b, 0x3e4)) * dt * BoatF(0x5c9ac4));
        double c41c = crt::sin(F<float>(b, 0x41c));
        float mx = (float)(crt::cos(F<float>(b, 0x418)) * c41c * F<float>(b, 0x3e4));
        float my = (float)(crt::sin(F<float>(b, 0x418)) * F<float>(b, 0x3e4));
        double s41c = crt::cos(F<float>(b, 0x41c));
        float mz = (float)(crt::cos(F<float>(b, 0x420)) * s41c * F<float>(b, 0x3e4));
        float ground = (float)(Terrain_HeightAt(F<uint8_t*>(b, 0x206), 0, F<float>(b, 0x400), F<float>(b, 0x408)) + BoatF(0x5c8568));
        if (F<float>(b, 0x404) < ground) {
            F<uint32_t>(b, 0x404) = BoatBits(ground);
            F<uint32_t>(&lift, 0) = 0;
            F<uint32_t>(&my, 0) = 0;
        }
        F<float>(b, 0x400) = F<float>(b, 0x400) - mx;
        uint8_t* e = F<uint8_t*>(b, 0x216);
        F<float>(b, 0x404) = my + F<float>(b, 0x404);
        F<float>(b, 0x408) = F<float>(b, 0x408) - mz;
        F<float>(e, 0x70) = (float)((double)mx * BoatD(0x5c8678));
        if (!gearSet) ++g_unwrittenReads[4];
        F<float>(e, 0x74) = (float)((double)gearStep * BoatD(0x5cf0b8)); // QUIRK: not my
        F<float>(e, 0x78) = (float)((double)mz * BoatD(0x5c8678));
        F<uint32_t>(e, 0x7c) = 0x3a83126f, F<uint32_t>(e, 0x80) = 0x3ba3d70a, F<uint32_t>(e, 0x84) = 0x3a83126f;
    }
    double c41c = crt::sin(F<float>(b, 0x41c));
    float s1 = (float)(crt::cos(F<float>(b, 0x418)) * c41c * F<float>(b, 0x3e4));
    float s2 = (float)(crt::sin(F<float>(b, 0x418)) * F<float>(b, 0x3e4));
    double s41c = crt::cos(F<float>(b, 0x41c));
    float s3 = (float)(crt::cos(F<float>(b, 0x420)) * s41c * F<float>(b, 0x3e4));
    Channel_SetRate(F<uint8_t*>(b, 0x3cc), 0, (int32_t)((double)F<float>(b, 0x3e0) / BoatF(0x5c8c04) * BoatF(0x5ca0a4) + BoatF(0x5ca1c0)));
    Channel_SetVolume(F<uint8_t*>(b, 0x3d0), 0, BoatBits((float)(crt::fabs(F<float>(b, 0x3e4)) * BoatD(0x5c85d8))));
    Channel_SetRate(F<uint8_t*>(b, 0x3d0), 0, (int32_t)(crt::fabs(F<float>(b, 0x3e4)) * BoatD(0x5cf0b0) + BoatD(0x5cf0a8)));
    float ground = (float)(Terrain_HeightAt(F<uint8_t*>(b, 0x206), 0, F<float>(b, 0x400), F<float>(b, 0x408)) + BoatF(0x5c8568));
    if (F<float>(b, 0x404) < ground) {
        F<uint32_t>(b, 0x404) = BoatBits(ground);
        F<uint32_t>(&lift, 0) = 0;
        F<uint32_t>(&s2, 0) = 0;
    }
    F<float>(b, 0x400) = F<float>(b, 0x400) - s1;
    F<float>(b, 0x408) = F<float>(b, 0x408) - s3;
    F<float>(b, 0x404) = (float)((double)s2 + lift + F<float>(b, 0x404));
    F<float>(b, 0x3f0) = (float)(F<float>(b, 0x3f0) - (double)F<float>(b, 0x3e4) * BoatD(0x5c86b0));
    if (BoatKey('c')) {
        BoatKeyUp('c');
        if (F<int32_t>(g_62be68, 0x60)) World_SetFollow(g_world, 0, 0, nullptr);
        else World_SetFollow(g_world, 0, 1, F<uint8_t*>(b, 0x20a));
    }
    uint32_t still = 1;
    if (BoatKey('j')) {
        F<float>(b, 0x3f4) = (float)(F<float>(b, 0x3f4) - (double)dt * BoatD(0x5c8750));
        still = 0;
    } else if (BoatKey('l')) {
        F<float>(b, 0x3f4) = (float)((double)dt * BoatD(0x5c8750) + F<float>(b, 0x3f4));
        still = 0;
    }
    Channel_SetLoop(F<uint8_t*>(b, 0x3d8), 0, still);
    still = 1;
    if (BoatKey('i')) {
        F<float>(b, 0x3f8) = (float)(F<float>(b, 0x3f8) - BoatD(0x5c8678));
        still = 0;
    } else if (BoatKey('k')) {
        F<float>(b, 0x3f8) = (float)(F<float>(b, 0x3f8) + BoatD(0x5c8678));
        still = 0;
    }
    Channel_SetLoop(F<uint8_t*>(b, 0x3dc), 0, still);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x20e), 0, 0, F<uint32_t>(b, 0x3f4), 0);
    SceneObj_SetAngles(F<uint8_t*>(b, 0x212), 0, F<uint32_t>(b, 0x3f8), 0, 0);
    uint8_t* jet = F<uint8_t*>(b, 0x22e);
    if (F<float>(b, 0x3ec) > BoatD(0x5cf0a0)) {
        if (F<float>(b, 0x3ec) > BoatD(0x5c9cc8)) {
            ((void(__fastcall*)(uint8_t*))(*(void***)jet)[3])(jet);
            SceneObj_SetAlpha(F<uint8_t*>(b, 0x22e), 0, 0xaf, 0);
        } else {
            SceneObj_SetAlpha(jet, 0, 0xff, 0);
            uint8_t* j2 = F<uint8_t*>(b, 0x22e);
            ((void(__fastcall*)(uint8_t*))(*(void***)j2)[4])(j2);
        }
        float f = F<float>(b, 0x3ec);
        double sq = (double)f * f;
        SceneObj_SetScale36(F<uint8_t*>(b, 0x22e), 0, (float)(sq + sq + BoatD(0x5c85b8)));
        SceneObj_SetAngles(F<uint8_t*>(b, 0x22e), 0, 0, 0, BoatBits((float)((double)F<float>(b, 0x3ec) * BoatF(0x5c8900))));
        SceneObj_SetTint(F<uint8_t*>(b, 0x22e), 0, 0xff, 0xff, 0xff, 1);
        if (F<float>(b, 0x3ec) > BoatD(0x5c9ba0)) {
            F<uint8_t*>(b, 0x21a)[0x1d8] = 0;
            float sn = (float)(crt::cos((double)F<float>(b, 0x41c) + F<float>(b, 0x3f4)) * BoatD(0x5cf098));
            float cs = (float)(crt::sin((double)F<float>(b, 0x41c) + F<float>(b, 0x3f4)) * BoatD(0x5cf098));
            uint8_t* e = F<uint8_t*>(b, 0x21a);
            F<uint32_t>(e, 0x74) = 0x3b83126f;
            F<float>(e, 0x70) = cs;
            F<float>(e, 0x78) = sn;
            F<uint32_t>(e, 0x7c) = 0x3ba3d70a, F<uint32_t>(e, 0x80) = 0x3b449ba6, F<uint32_t>(e, 0x84) = 0x3ba3d70a;
        } else {
            F<uint8_t*>(b, 0x21a)[0x1d8] = 1;
        }
        F<float>(b, 0x3ec) = (float)((double)F<float>(b, 0x3ec) * BoatD(0x5cf090));
    } else {
        ((void(__fastcall*)(uint8_t*))(*(void***)jet)[4])(jet);
        F<uint8_t*>(b, 0x21a)[0x1d8] = 1;
        F<uint32_t>(b, 0x3ec) = 0;
        if (BoatKey(' ')) {
            if (Channel_Playing(F<uint8_t*>(b, 0x3d4))) {
                Channel_SetRate(F<uint8_t*>(b, 0x3d4), 0, 0xabe0);
                F<int32_t>(F<uint8_t*>(b, 0x3d4), 0x130) = 1;
            }
            F<uint32_t>(b, 0x3ec) = 0x3f800000;
        }
    }
    float swell = (float)crt::cos((double)F<float>(b, 0x3ec) * BoatF(0x5c8760) + BoatF(0x5c8764));
    double r424 = F<float>(b, 0x424);
    double sinC = crt::cos(F<float>(b, 0x3f4));
    F<float>(b, 0x424) = (float)(r424 - sinC * ((double)swell * F<float>(b, 0x3ec)) * BoatD(0x5c85b8));
    double cosC = crt::sin(F<float>(b, 0x3f4));
    F<float>(b, 0x42c) = (float)(cosC * ((double)swell * F<float>(b, 0x3ec)) * BoatD(0x5c85b8) + F<float>(b, 0x42c));
    float jy = (float)(F<float>(b, 0x22a) - (double)swell * BoatF(0x5c8898));
    SceneObj_SetPos(F<uint8_t*>(b, 0x21e), 0, F<uint32_t>(b, 0x222), F<uint32_t>(b, 0x226), BoatBits(jy));
    SceneObj_SetAngles(b, 0, F<uint32_t>(b, 0x418), F<uint32_t>(b, 0x41c), F<uint32_t>(b, 0x420));
    Obj_MoveTo(b, 0, F<float>(b, 0x400), F<float>(b, 0x404), F<float>(b, 0x408));
}
RT3_REPLACE(0x486240, Boat_Update);

 // namespace

// 0x4b74e0 (vtable 0x5d00dc slot 1). Industry window w handles ev for the
// selected industry (0x6d0818+0x90 in 0x62b26c). Idle (0) refreshes the
// buttons every 750 ms; 0x3ea refreshes. Hover (0xca, mouse at 0x6cfd0c /
// 0x6cfd10): the station buttons 0xbd24..0xbd2d (0x6a083c) follow that
// station and select it; the cargo buttons 0xbd74..0xbda5 toggle the cargo
// overlay (the open one in 0x6cfd14, captions 0xb8b / 0xb8c). Clicks (0xcb):
// 0xbd15 / 0xbd16 switch tabs, 0xbd19 buys (price 0x40d540; not in a closed
// territory, 0xfdb; cash + 500 must cover it, 0x37e; sandbox 0x37d, else
// 0x37c), 0xbd1a sells for half the value +0x27a (0x37f), 0xbd1b expands by
// the type's +0x43 x effect 0x3c (0x382 / 0x381 / 0x380), each confirmed
// (0x3f2) and done locally or sent to the host (0x35 / 0x33 / 0x31);
// 0xbd1c leaves the tool, 0xbd1d toggles 0x6cfd0a, 0xbd2e edits +0x391.
// Returns 0 when handled.
int32_t __fastcall IndustryWindow_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    uint8_t* ind = nullptr;
    if (Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90)))
        ind = (uint8_t*)Collection_Get(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90));
    uint32_t type = ev[0];
    if (type > 0xcb) {
        if (type == 0x3ea) IndustryWindow_Refresh(w);
        return 1;
    }
    if (type == 0) {
        if (ClockMs() - F<int32_t>(w, 0x78) > 0x2ee) IndustryView_UpdateButtons(w);
        return 1;
    }
    char text[0x200];
    if (type == 0xca) {
        g_6cfd0c = ev[9];
        g_6cfd10 = ev[10];
        int32_t id = (int32_t)ev[1];
        const uint32_t* stations = (const uint32_t*)0x6a083c;
        if (id >= 0xbd24 && id <= 0xbd2d) {
            if (Collection_IsValid(g_stations, 0, stations[id])) {
                uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, stations[ev[1]]);
                uint8_t* b = (uint8_t*)Obj_Building(st);
                if (!F<int32_t>(g_62be68, 0x60)) {
                    int32_t mode = F<int32_t>(g_app, 0x315);
                    Camera_FollowObject((uint8_t*)Obj_Building(st), mode);
                    ((void(__fastcall*)(uint8_t*))(*(void***)b)[0x5c / 4])(b);
                }
                View_SetToolMode(g_gameWindow, 0, 5, (int32_t)stations[ev[1]]);
            }
            return 0;
        }
        if (id >= 0xbd74 && id <= 0xbda5) {
            uint8_t* btn = Widget_FindChild(w, 0, id);
            uint8_t* cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(btn, 0x4f));
            uint8_t* shown = cargo;
            if (int32_t open = g_6cfd14)
                shown = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(Widget_FindChild(w, 0, open), 0x4f));
            {
                const char* nm = LocalizeName((const char*)shown + 4);
                const char* fmt = GetString(0xb8b);
                FormatText(text, fmt, nm, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
            Window_SendEvent5(w, 0, 0x6e, hook::As<uint32_t>(g_6cfd14), (uint32_t)(uintptr_t)text, 0, 0);
            Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
            if (Widget_IsVisible(btn)) {
                Widget_ClearFlags(btn, 0, 1);
                Overview_Close(g_map);
                g_6cfd14 = 0;
            } else {
                if (uint32_t open = hook::As<uint32_t>(g_6cfd14)) Window_SendEvent5(w, 0, 0x87, open, 1, 0, 0);
                Widget_SetFlags(btn, 0, 1, 0, 0);
                {
                    const char* nm = LocalizeName((const char*)cargo + 4);
                    const char* fmt = GetString(0xb8c);
                    FormatText(text, fmt, nm, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                }
                Window_SendEvent5(w, 0, 0x6e, ev[1], (uint32_t)(uintptr_t)text, 0, 0);
                Overview_SetMode(g_map, 0, 3, F<int32_t>(btn, 0x4f), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
                Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
                hook::As<uint32_t>(g_6cfd14) = ev[1];
            }
            g_6cfd10 = 0xbf800000;
            g_6cfd0c = 0xbf800000;
            return 0;
        }
        return id == 0x3e7 ? 0 : 1;
    }
    if (type != 0xcb) return 1;
    char num1[0x1f8], num2[0x1f8];
    switch (ev[1]) {
    case 0xbd15:
        Window_SendEvent5(w, 0, 0x6d, 0xbd13, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, 0xbd14, 3, 0, 0);
        return 0;
    case 0xbd16:
        Window_SendEvent5(w, 0, 0x6d, 0xbd14, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, 0xbd13, 3, 0, 0);
        return 0;
    case 0xbd1c:
        View_SetToolMode(g_gameWindow, 0, 0, -1);
        return 0;
    case 0xbd1d:
        *(uint8_t*)0x6cfd0a = !*(uint8_t*)0x6cfd0a;
        IndustryWindow_Refresh(w);
        return 0;
    case 0xbd2e:
        if (ind) {
            char buf[0x60];
            crt::sprintf(buf, (const char*)0x5c9cd0, ind[0x391]);
            Dialog_RunMessage(GetString(0xf2d), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)buf, 0x1e, 0, nullptr, nullptr, 0, 0,
                              nullptr, 0);
            ind[0x391] = (uint8_t)crt::atoi(buf);
        }
        return 0;
    case 0xbd19: {
        if (!ind || !Obj_Company21(hook::As<void*>(g_world))) return 0;
        float price = Building_Price(ind, 0, (uint32_t)Obj_Get21(hook::As<void*>(g_world)));
        uint8_t* terr = (uint8_t*)Collection_Get(g_territories, 0, Obj_GridByte(ind));
        int32_t region = F<int32_t>(terr, 0x31) ? F<int32_t>(terr, 0) : F<int32_t>(terr, 0x35);
        if (Game_EffectiveVersion() >= 0x67 && !Company_EntryFlag(Obj_Company21(hook::As<void*>(g_world)), 0, region)) {
            const char* name = IndName(ind);
            const char* rn = (const char*)Collection_Get(g_territories, 0, (uint32_t)region) + 4;
            {
                const char* fmt = GetString(0xfdb);
                FormatText(text, fmt, name, rn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
            IndMsg(text, 1);
            return 0;
        }
        double cash = Company_Stat((uint8_t*)Obj_Company21(hook::As<void*>(g_world)), 0, 0xd, 0x2329) + *(const double*)rt3::Rdata(0x5d0298);
        bool sandbox = g_newGame[0x82] != 0;
        if (!(price < cash || price == cash)) {
            if (!sandbox) {
                uint8_t* co = (uint8_t*)Obj_Company21(hook::As<void*>(g_world));
                const char* name = IndName(ind);
                const char* have = Num_FormatSimple(num1, 9, Company_Stat(co, 0, 0xd, 0x2329), 0xb);
                const char* cost = Num_FormatSimple(num2, 9, price, 0xb);
                {
                    const char* fmt = GetString(0x37e);
                    FormatText(text, fmt, name, cost, have, nullptr, nullptr, nullptr, nullptr, nullptr);
                }
                IndMsg(text, 1);
                return 0;
            }
            {
                const char* fmt = GetString(0x37d);
                FormatText(text, fmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        } else if (sandbox) {
            {
                const char* fmt = GetString(0x37d);
                FormatText(text, fmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        } else {
            uint8_t* t1 = IndType(ind);
            const char* n2 = LocalizeName((const char*)IndType(ind) + 0x4f);
            const char* n1 = LocalizeName((const char*)t1 + 0x4f);
            const char* cost = Num_FormatSimple(num2, 9, price, 0xb);
            {
                const char* fmt = GetString(0x37c);
                FormatText(text, fmt, n2, cost, n1, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        }
        uint32_t id = F<uint32_t>(g_gameWindow, 0x90);
        uint32_t r = IndMsg(text, 2);
        if (!Collection_IsValid(g_buildings, 0, id) || r != 0x3f2) return 0;
        IndDeal(w, id, price, 0x35, Company_TakeBuilding);
        return 0;
    }
    case 0xbd1a: {
        if (!ind || !Obj_Company21(hook::As<void*>(g_world))) return 0;
        float half = (float)((double)F<float>(ind, 0x27a) * *(const double*)rt3::Rdata(0x5c8628));
        const char* name = IndName(ind);
        const char* value = Num_FormatSimple(num1, 9, half, 0xb);
        char msg[0x1f8];
        {
            const char* fmt = GetString(0x37f);
            FormatText(msg, fmt, name, value, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        uint32_t id = F<uint32_t>(g_gameWindow, 0x90);
        if (IndMsg(msg, 2) != 0x3f2) return 0;
        IndDeal(w, id, half, 0x33, Company_ReleaseBuilding);
        return 0;
    }
    case 0xbd1b: {
        if (!ind || !Obj_Company21(hook::As<void*>(g_world))) return 0;
        float cost = (float)F<int32_t>(IndType(ind), 0x43);
        uint32_t player = (uint32_t)Obj_Get25(hook::As<void*>(g_world));
        uint32_t company = (uint32_t)Obj_Get21(hook::As<void*>(g_world));
        cost = (float)((double)Effects_Multiplier(g_world, 0, 0x3c, company, player, 0) * cost);
        double cash = Company_Stat((uint8_t*)Obj_Company21(hook::As<void*>(g_world)), 0, 0xd, 0x2329) + *(const double*)rt3::Rdata(0x5d0298);
        bool sandbox = g_newGame[0x82] != 0;
        if (!(cost < cash || cost == cash)) {
            if (!sandbox) {
                uint8_t* co = (uint8_t*)Obj_Company21(hook::As<void*>(g_world));
                const char* name = IndName(ind);
                const char* have = Num_FormatSimple(text + 0x1f8, 9, Company_Stat(co, 0, 0xd, 0x2329), 0xb);
                const char* pct = Num_Format(num1, 5, F<float>(IndType(ind), 0x47), 0xa, 1, 0, 0);
                const char* amount = Num_FormatSimple(num2, 9, cost, 0xb);
                char msg[0x1f8];
                {
                    const char* fmt = GetString(0x382);
                    FormatText(msg, fmt, name, amount, pct, have, nullptr, nullptr, nullptr, nullptr);
                }
                IndMsg(msg, 1);
                return 0;
            }
            {
                const char* fmt = GetString(0x381);
                FormatText(text, fmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        } else if (sandbox) {
            {
                const char* fmt = GetString(0x381);
                FormatText(text, fmt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        } else {
            const char* name = IndName(ind);
            const char* amount = Num_FormatSimple(num1, 9, cost, 0xb);
            const char* pct = Num_Format(num2, 5, F<float>(IndType(ind), 0x47), 0xa, 1, 0, 0);
            {
                const char* fmt = GetString(0x380);
                FormatText(text, fmt, name, pct, amount, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        }
        uint32_t id = F<uint32_t>(g_gameWindow, 0x90);
        if (IndMsg(text, 2) != 0x3f2) return 0;
        IndDeal(w, id, cost, 0x31, Company_InvestInBuilding);
        return 0;
    }
    }
    return 1;
}
RT3_REPLACE(0x4b74e0, IndustryWindow_HandleEvent);

// 0x4b81d0. Industry window constructor (0x6cfd04): images 0x5c9f98 and
// 0x5d0258, fonts 3 / 2 / 1, layout 0x5d02a0. In the map editor (0x6d3c84 == 1)
// controls 0xbd19..0xbd1b get event 0x78; the lists 0xbd17 / 0xbd18 draw with
// 0x4b6870 and 0xbd13 / 0xbd14 get style 3.
uint8_t* __fastcall IndustryWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d00dc;
    g_industryWindow = w;
    F<uint32_t>(w, 0x78) = 0;
    g_6cfcf4 = Pack_AcquireNamed(g_pack, 0, (const char*)0x5c9f98);
    g_6cfd18 = Pack_AcquireNamed(g_pack, 0, (const char*)0x5d0258);
    g_6cfcf8 = Pack_AcquireFont(g_pack, 0, 3);
    g_6cfcfc = Pack_AcquireFont(g_pack, 0, 2);
    g_6cfd00 = Pack_AcquireFont(g_pack, 0, 1);
    g_6cfd08 = 0;
    g_6cfd09 = 0;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d02a0);
    if (g_language == 1) {
        Window_SendEvent5(w, 0, 0x78, 0xbd19, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0xbd1a, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0xbd1b, 0x12, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x7a, 0xbd17, 0x4b6870, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0xbd18, 0x4b6870, 0, 0);
    Window_SendEvent5(w, 0, 0x6d, 0xbd13, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x86, 0xbd14, 3, 0, 0);
    IndustryWindow_Refresh(w);
    return w;
}
RT3_REPLACE(0x4b81d0, IndustryWindow_Construct);

// 0x4b8320. Packet: company pkt[1] invests in building pkt[2] (float pkt[3]);
// pkt[0] is the sending player.
RT3_NOINLINE void __fastcall BuildingPacket_Invest(uint32_t* pkt) {
    Company_InvestInBuilding(Collection_Get(g_companies, 0, pkt[1]), 0, pkt[2], Prod_Bits(pkt[3]));
    BuildingPacket_Done(pkt[0]);
}
RT3_REPLACE(0x4b8320, BuildingPacket_Invest);

// 0x4b8380. Packet: company pkt[1] releases building pkt[2] at price pkt[3].
RT3_NOINLINE void __fastcall BuildingPacket_Release(uint32_t* pkt) {
    Company_ReleaseBuilding(Collection_Get(g_companies, 0, pkt[1]), 0, pkt[2], Prod_Bits(pkt[3]));
    BuildingPacket_Done(pkt[0]);
}
RT3_REPLACE(0x4b8380, BuildingPacket_Release);

// 0x4b83e0. Packet: company pkt[1] takes building pkt[2] at price pkt[3].
RT3_NOINLINE void __fastcall BuildingPacket_Take(uint32_t* pkt) {
    Company_TakeBuilding(Collection_Get(g_companies, 0, pkt[1]), 0, pkt[2], Prod_Bits(pkt[3]));
    BuildingPacket_Done(pkt[0]);
}
RT3_REPLACE(0x4b83e0, BuildingPacket_Take);

// 0x4b8440. Bulldoze tool window ("Bulldoze.win"); also refreshes the world's colours.
void __fastcall Bulldoze_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d02b4;
    Scene_PopEngineBusy();
    World_RefreshColors();
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4b8440, Bulldoze_Destruct);

// Brush size callbacks: o remembers choice i (+0x78) and the renderer gets
// the radius from the table entry plus 0x5c8628 (float or int tables).
RT3_NOINLINE void __fastcall Brush_SetSizeF621cc8(uint8_t* o, int, int32_t i) {
    F<int32_t>(o, 0x78) = i;
    const float r = (float)((double)((const float*)0x621cc8)[i] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, r);
}
RT3_REPLACE(0x4b8470, Brush_SetSizeF621cc8);

// 0x4b84b0. Highlights entry +0x78 of the seven buttons 0xbf69.. of window w
// (event 0x65 for the selected one, 0x87 for the others).
RT3_NOINLINE void __fastcall Window_HighlightTab7(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i)
        Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0xbf69 + i, 1, 0, 0);
}
RT3_REPLACE(0x4b84b0, Window_HighlightTab7);

// 0x4b84f0 (event handler). The bulldozer of the map d (size +0x78 from
// 0x621cc8, bulldozing +0x7c): sizes 0xbf69..0xbf6f (0x621cc4); pressing on
// the map (without Ctrl) starts with sound 0x39; while held it clears the
// cells around the focus each time it moved over 0x5c97c0 (0x621ce4 /
// 0x621ce8, -1 when idle), else hovering prices it (string 0xd8, not in a
// tutorial) or shows the reason it cannot.
int32_t __fastcall Bulldozer_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    const uint32_t id = msg[1];
    switch (msg[0]) {
    case 0xcb:
        if (id == 0x7d6 && F<int32_t>(d, 0x7c)) {
            F<int32_t>(d, 0x7c) = 0;
            return 0;
        }
        return 1;
    case 0xca:
        if (id == 0x7d6) {
            float fx, fy;
            if (!Scene_FocusFlagIs1(&fx, &fy) || (msg[8] & 0x800)) return 0;
            Ui_PlaySound(0x39, 0, 0x3f000000);
            F<int32_t>(d, 0x7c) = 1;
            break;
        }
        if ((int32_t)id > 0xbf68 && (int32_t)id <= 0xbf6f) {
            const int32_t size = (int32_t)id - 0xbf69;
            Brush_SetSizeF621cc8(d, 0, size);
            g_621cc4 = size;
            Window_HighlightTab7(d);
            return 0;
        }
        return 1;
    case 2:
        break;
    default:
        return 1;
    }
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    if (F<int32_t>(d, 0x7c)) {
        const float moved = (float)Distance2D(x, y, g_621ce4, g_621ce8);
        if (moved <= D(0x5c97c0)) return 0;
        g_621ce4 = x;
        g_621ce8 = y;
    } else {
        hook::As<uint32_t>(g_621ce4) = 0xbf800000;
        hook::As<uint32_t>(g_621ce8) = 0xbf800000;
    }
    const int32_t check = F<int32_t>(d, 0x7c) == 0;
    const uint32_t company = (uint32_t)Obj_Get21(g_world);
    const float r = (float)((double)((const float*)0x621cc8)[F<int32_t>(d, 0x78)] + D(0x5c8628));
    float cost;
    char text[0x3e8];
    if (Map_Bulldoze(g_map, 0, x, y, r, company, check, &cost, text)) {
        if (check && !g_newGame[0x82]) {
            char num[0x100];
            const char* price = Num_FormatSimple(num, 9, cost, 0xb);
            FormatText(text, GetString(0xd8), price, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            FormatText(text, (const char*)0x5c87a8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 0, -1);
    return 1;
}
RT3_REPLACE(0x4b84f0, Bulldozer_HandleEvent);

// 0x4b8720. Bulldoze tool window constructor ("Bulldoze.win"): brush size
// 0x621cc4 (+0x78) sets the scene's brush radius (table 0x621cc8 + 0.5), then
// the window is loaded at (0.405, 0.707), its tab highlighted and the scene paused.
uint8_t* __fastcall Bulldoze_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    int32_t size = g_621cc4;
    F<uint32_t>(w, 0) = 0x5d02b4;
    F<int32_t>(w, 0x78) = size;
    float radius = (float)((double)((const float*)0x621cc8)[size] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, radius);
    F<int32_t>(w, 0x7c) = 0;
    Window_Load(w, 0, BitsF(0x3ecf5c29), BitsF(0x3f34e81b), (const char*)0x5d02c0);
    Window_HighlightTab7(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    return w;
}
RT3_REPLACE(0x4b8720, Bulldoze_Construct);

// 0x4b87a0 (hover callback). Starts the campaign hover sound (0x5f0a24,
// 0x6cfd28 / its start time 0x6cfd2c) when hovered without EDX; stops it
// when not hovered.
void __fastcall Campaign_HoverSoundCb(void*, int32_t quiet, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    if (on && !quiet) {
        g_6cfd28 = Sound_Play2D(g_sound, 0, g_5f0a24, BitsF(0x3e99999a), 0x3e4ccccd, 1,
                                                        quiet, (void*)(intptr_t)quiet);
        g_6cfd2c = ClockMs();
        g_6cfd30 = 0;
        return;
    }
    if (g_6cfd28 && !on) {
        Sound_Stop(g_sound, 0, g_6cfd28, on);
        g_6cfd28 = 0;
    }
}
RT3_REPLACE(0x4b87a0, Campaign_HoverSoundCb);

// 0x4b8810 / 0x4b8850 (hover callbacks). The campaign window 0x6cfd1c gets
// event 0x65 (hovered) / 0x87 for the widget at o+4 / o+0xa, state 3.
int32_t __fastcall Campaign_HoverCb4(uint8_t* o, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    return Window_SendEvent5(g_6cfd1c, 0, on ? 0x65 : 0x87, (uint32_t)(uintptr_t)(o + 4), 3, 0, 0);
}
RT3_REPLACE(0x4b8810, Campaign_HoverCb4);

int32_t __fastcall Campaign_HoverCbA(uint8_t* o, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    return Window_SendEvent5(g_6cfd1c, 0, on ? 0x65 : 0x87, (uint32_t)(uintptr_t)(o + 0xa), 3, 0, 0);
}
RT3_REPLACE(0x4b8850, Campaign_HoverCbA);

// 0x4b8890 (hover callback). Campaign scenario button `id` (0xc352..):
// selected / hovered state (event 0x6c on 0xc372 + index) and its label
// (0xba3 for the current scenario, else 0xba4) with the medal string
// (0x621d6c by lobby +0xc6 + index) and the name (0x621d30).
void __fastcall Campaign_ScenarioHoverCb(int32_t id, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    const int32_t i = id - 0xc352;
    const int32_t sel = g_newGame[0xc4];
    const uint32_t state = on ? (i == sel ? 3 : 1) : (sel == i ? 2 : 0);
    Window_SendEvent5(g_6cfd1c, 0, 0x6c, (uint32_t)(i + 0xc372), state, 0, 0);
    char medal[0x100], text[0x200];
    FormatText(medal, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
    if (const uint8_t b = g_newGame[0xc6 + i]) FormatText(medal, GetString(((const int32_t*)0x621d6c)[b]), 0, 0, 0, 0, 0, 0, 0, 0);
    const int32_t name = ((const int32_t*)0x621d30)[i];
    if (i == g_newGame[0xc4]) {
        const char* n = GetString(name);
        FormatText(text, GetString(0xba3), medal, n, 0, 0, 0, 0, 0, 0);
    } else {
        const char* n = GetString(name);
        FormatText(text, GetString(0xba4), medal, n, 0, 0, 0, 0, 0, 0);
    }
    Window_SendEvent5(g_6cfd1c, 0, 0x6e, (uint32_t)id, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4b8890, Campaign_ScenarioHoverCb);

// 0x4b89c0. Refreshes campaign dialog d for its selected page +0x78
// (buttons 0xc368..0xc36b, 0xc36c..0xc371 reset). Past page 4: picks the
// newest scenario file of campaign 0x6cec7c +0xc4 (names 0x621cf0, tried
// as -2, -1, then 100 down to 1, default 0x5d0540) and starts it (event
// 0xcc). Otherwise marks the campaign's chapter and the 16 progress flags
// +0xc6, and when asked (+0x7c) shows the page text, plays the queued
// narration (intro +0x7d, or chapter 0x6cfd24) and every 90 s while the
// page's chapter is current nags to pick a fitting page.
RT3_NOINLINE void __fastcall Campaign_Refresh(uint8_t* d) {
    for (int32_t i = 0; i < 4; ++i) Window_SendEvent5(d, 0, i == F<int32_t>(d, 0x78) ? 0x6d : 0x86, 0xc368 + i, 3, 0, 0);
    for (int32_t i = 0; i < 6; ++i) Window_SendEvent5(d, 0, 0x87, 0xc36c + i, 3, 0, 0);
    char name[0x1f8], path[0x1fc];
    if (F<int32_t>(d, 0x78) > 4) {
        F<int32_t>(g_newGame, 1) = 6;
        int32_t n = -2;
        for (;;) {
            const char* camp = hook::Global<const char*>(0x621cf0 + g_newGame[0xc4] * 4);
            if (n == -2) crt::sprintf(name, (const char*)0x5ca9d8, camp);
            else if (n == -1) crt::sprintf(name, (const char*)0x5d0564, camp);
            else crt::sprintf(name, (const char*)0x5d0554, camp, 100 - n);
            crt::sprintf(path, (const char*)0x5d054c, (const char*)0x5ced78, name);
            if (File_Exists(path)) break;
            if (++n >= 100) {
                crt::sprintf(name, (const char*)0x5d0540);
                break;
            }
        }
        strcpy((char*)g_newGame + 0x11, name);
        g_newGame[0xc5] = 1;
        Game_PostInputEvent(0xcc, 0, 1, 0, 0);
        return;
    }
    Window_SendEvent5(d, 0, 0x6c, 0xc372 + g_newGame[0xc4], 2, 0, 0);
    for (int32_t i = 0; i < 0x10; ++i) Window_SendEvent5(d, 0, 0x6c, 0xc382 + i, g_newGame[0xc6 + i], 0, 0);
    if (d[0x7c]) {
        Window_SendEvent5(d, 0, 0x8c, 0xc351, F<int32_t>(d, 0x78) + 0x1c, 0, 0);
        if (d[0x7d]) {
            if (g_6cfd34) Campaign_Speak(d, (const char*)0x5d051c);
            d[0x7d] = 0;
        } else if (int32_t k = g_6cfd24) {
            if (g_newGame[k + 0xc5] > 1)
                crt::sprintf(path, (const char*)0x5d0500, k, hook::Global<const char*>(0x5f3d14 + k * 4));
            else
                FormatText(path, (const char*)0x5d04ec, hook::Global<const char*>(0x621de4 + Rand(0, 3) * 4), nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr);
            if (g_6cfd34) Campaign_Speak(d, path);
            g_6cfd24 = 0;
        }
        int32_t ch = g_newGame[0xc4];
        if (ch == F<int32_t>(d, 0x9d)) {
            int32_t sel = F<int32_t>(d, 0x78);
            int32_t last = F<int32_t>(d, 0x85);
            const char* nag = nullptr;
            bool due = ClockMs() - last > 90000 || !last;
            if (due && ((ch > 4 && sel == 0) || (ch > 9 && sel == 1) || (ch > 12 && sel == 2))) {
                nag = (const char*)0x5d04cc;
            } else {
                if (ch != F<int32_t>(d, 0x9d)) goto done;
                last = F<int32_t>(d, 0x85);
                if (!(ClockMs() - last > 90000 || !last)) goto done;
                if (!((ch < 13 && sel == 3) || (ch < 10 && sel == 2) || (ch < 5 && sel == 1))) goto done;
                nag = (const char*)0x5d04ac;
            }
            if (g_6cfd34) Campaign_Speak(d, nag);
            F<int32_t>(d, 0x85) = ClockMs();
        }
    }
done:
    d[0x7c] = 0;
}
RT3_REPLACE(0x4b89c0, Campaign_Refresh);

// 0x4b8db0 (callback). Refreshes the campaign window 0x6cfd1c.
void Campaign_RefreshOpen() { Campaign_Refresh(g_6cfd1c); }
RT3_REPLACE(0x4b8db0, Campaign_RefreshOpen);

// 0x4b8dc0. Campaign window destructor: back to the saved display size, the
// UI busy count down, the options redraw flag set, its sound 0x6cfd28 stopped
// and, unless kept (+0x7f), the music object 0x6cfd34 freed.
void __fastcall CampaignWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0570;
    uint8_t* eng = g_engine;
    Engine_SetDisplayMode(eng, 0, F<int32_t>(eng, 0x11468a), F<int32_t>(eng, 0x11468e), -1, 0);
    Obj_Decc60(g_windowManager);
    F<uint32_t>(F<uint8_t*>(g_app, 0x14), 0x110) = 1;
    if (uint32_t snd = g_6cfd28) Sound_Stop(g_sound, 0, snd, 0);
    g_6cfd28 = 0;
    if (!w[0x7f])
        if (uint8_t* m = g_6cfd34) {
            Music_Close(m);
            crt::operator_delete(m);
            g_6cfd34 = nullptr;
        }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4b8dc0, CampaignWindow_Destruct);

// 0x4b8e60. Campaign window constructor (0x6cfd1c): 640 x 480 display, the
// options redraw flag off, music stopped, layout Campaign.win with the
// handlers of its buttons (0xc351, 0xc362.., 0xc393.., 0xc39b, the sixteen
// level buttons 0xc352..), a music object at the music volume with 3D sound
// (0x6cfd34), the page by campaign progress (0x6cec7c +0xc4: 0..4, 5..9,
// 10..12, 13+), the reset state, +0x7d when nothing is unlocked yet; then
// refreshes and starts its clock (+0x95).
uint8_t* __fastcall CampaignWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0570;
    hook::As<uint32_t>(g_6cfd20) = 0;
    Engine_SetDisplayMode(g_engine, 0, 0x280, 0x1e0, -1, 0);
    F<uint32_t>(F<uint8_t*>(g_app, 0x14), 0x110) = 0;
    Sound_StopMusic(g_sound, 0, 1);
    g_6cfd28 = 0;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d057c);
    g_6cfd1c = w;
    Window_SendEvent5(w, 0, 0x80, 0xc351, 0x4b8db0, 0, 0);
    for (uint32_t id = 0xc36c; id < 0xc372; ++id) {
        Window_SendEvent5(w, 0, 0x7b, id - 10, 0x4b8850, 0, 0);
        Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
    }
    for (uint32_t id = 0xc397; id < 0xc39b; ++id) {
        Window_SendEvent5(w, 0, 0x7b, id - 4, 0x4b8810, 0, 0);
        Window_SendEvent5(w, 0, 0x87, id, 3, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x7b, 0xc39b, 0x4b87a0, 0, 0);
    g_6cfd34 = nullptr;
    if (uint8_t* snd = g_sound) {
        if (Sound_Is3D(snd)) {
            uint8_t* m = (uint8_t*)crt::operator_new(0x108);
            if (m) {
                F<uint32_t>(m, 0) = 0;
                F<uint32_t>(m, 0x104) = 0x3f800000;
            }
            g_6cfd34 = m;
        }
        if (uint8_t* m = g_6cfd34)
            Music_SetVolume(m, 0, (float)(F<float>(g_engine, 0x114702) * D(0x5c8640)));
    }
    g_newGame[0x97] = 1;
    uint8_t lvl = g_newGame[0xc4];
    F<int32_t>(w, 0x78) = lvl < 5 ? 0 : lvl < 10 ? 1 : lvl < 13 ? 2 : 3;
    F<uint32_t>(w, 0x95) = 0;
    F<uint32_t>(w, 0x81) = 0;
    F<uint32_t>(w, 0x8d) = 0;
    F<uint32_t>(w, 0x91) = 0;
    hook::As<uint32_t>(g_6cfd2c) = 0;
    F<uint32_t>(w, 0x85) = 0;
    w[0x7e] = 0;
    w[0x7f] = 0;
    w[0x7c] = 1;
    w[0x80] = 0;
    F<uint32_t>(w, 0x99) = 0;
    F<uint32_t>(w, 0x9d) = g_newGame[0xc4];
    for (uint32_t i = 0; i < 0x10; ++i) Window_SendEvent5(w, 0, 0x7b, 0xc352 + i, 0x4b8890, 0, 0);
    w[0x7d] = 0;
    if (!g_newGame[0xc4] && !NewGameConfig_FirstOpenSlot() && !g_newGame[0xc6]) w[0x7d] = 1;
    Campaign_Refresh(w);
    F<int32_t>(w, 0x95) = ClockMs();
    return w;
}
RT3_REPLACE(0x4b8e60, CampaignWindow_Construct);

 // namespace

// 0x4b90d0 (event handler). Campaign screen d: the scenario buttons
// 0xc352..0xc361 select a scenario (a second press starts it: page 5 with
// its intro clip 0x5f3d18; else its title, string 0xba3 with its state
// 0x621d6c / 0x621d30, and highlight), 0xc362..0xc367 switch the page
// (+0x78), 0xc393..0xc396 close it (stopping the voice, event 0xcc), 0xc39b
// plays a tip; Esc closes and space refreshes. Idle ticks run the narrator:
// restarted clips (+0x8d stage, +0x91 mood, +0x99 / +0x80 toggles, 15 s of
// silence, tips every 5 s more each time, 0x6cfd28..0x6cfd30) and pending
// page messages (0x624308).
int32_t __fastcall CampaignScreen_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    char frame[0x400];
    char* buf = frame;
    char* text = frame + 0x200;
    const uint32_t type = msg[0];
    if (type == 0xc9) return 1;
    if (type == 3) {
        F<int32_t>(d, 0x95) = ClockMs();
        if (F<int32_t>(d, 0x78) > 4) {
            d[0x7f] = 0;
            Campaign_Refresh(d);
            return 0;
        }
        if (msg[2] == 0x1b) {
            Game_PostInputEvent(0xcc, 0, 2, 0, 0);
            return 0;
        }
        if (msg[2] == 0x20) {
            Campaign_Refresh(g_6cfd1c);
            return 0;
        }
        return 1;
    }
    if (type == 0xca) {
        const int32_t now = ClockMs();
        F<int32_t>(d, 0x95) = now;
        if (F<int32_t>(d, 0x78) > 4 && now - F<int32_t>(d, 0x89) > 0x1f4) {
            d[0x7f] = 0;
            Campaign_Refresh(d);
            return 0;
        }
        if (F<int32_t>(d, 0x8d) == 3 && F<int32_t>(d, 0x91) == 3) {
            CampaignClip462(buf, 0x621dc0, 1);
            CampaignVoice462(d, buf);
            F<int32_t>(d, 0x91) = 4;
            F<int32_t>(d, 0x95) = ClockMs();
        }
        const uint32_t id = msg[1];
        if (id >= 0xc393 && id <= 0xc396) {
            if (uint8_t* v = g_6cfd34) {
                Music_Close(v);
                crt::operator_delete(v);
            }
            g_6cfd34 = nullptr;
            Game_PostInputEvent(0xcc, 0, 2, 0, 0);
            return 0;
        }
        if (id >= 0xc352 && id <= 0xc361) {
            const int32_t k = (int32_t)(id - 0xc352);
            if (k == g_newGame[0xc4]) {
                g_6cfd20 = 1;
                ResetScenarioButtons462(d);
                F<int32_t>(d, 0x78) = 5;
                Obj_Incc60(g_windowManager);
                Window_SendEvent5(d, 0, 0x8c, 0xc351, msg[1] - 0xc347, 0, 0);
                d[0x7f] = 1;
                const uint8_t sc = g_newGame[0xc4];
                crt::sprintf(buf, (const char*)0x5d058c, sc + 1, *(const char* const*)(0x5f3d18 + sc * 4));
                CampaignVoice462(d, buf);
                g_624308 = -1;
                F<int32_t>(d, 0x89) = ClockMs();
            } else {
                FormatText(buf, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
                if (const uint8_t state = g_newGame[0xc6 + k])
                    FormatText(buf, GetString(*(const int32_t*)(0x621d6c + state * 4)), 0, 0, 0, 0, 0, 0, 0, 0);
                const char* title = GetString(*(const int32_t*)(0x621d30 + k * 4));
                FormatText(text, GetString(0xba3), buf, title, 0, 0, 0, 0, 0, 0);
                Window_SendEvent5(d, 0, 0x6f, 0xc392, (uint32_t)(uintptr_t)text, 0, 0);
                Window_SendEvent5(d, 0, 0x6c, g_newGame[0xc4] + 0xc372, 0, 0, 0);
                Window_SendEvent5(d, 0, 0x6c, msg[1] + 0x20, 3, 0, 0);
            }
            g_newGame[0xc4] = (uint8_t)((uint8_t)msg[1] - 0x52);
            return 0;
        }
        if (id >= 0xc362 && id <= 0xc367) {
            static const int32_t kPage[6] = { 1, 0, 2, 1, 3, 2 };
            F<int32_t>(d, 0x78) = kPage[id - 0xc362];
            ResetScenarioButtons462(d);
            Window_SendEvent5(d, 0, 0x8c, 0xc351, msg[1] - 0xc342, 0, 0);
            g_624308 = -1;
            d[0x7c] = 1;
            return 0;
        }
        if (id == 0xc39b) {
            CampaignTip462(d, text);
            return 0;
        }
        return 1;
    }
    if (type != 0) return 1;
    if (F<int32_t>(d, 0x78) > 4) return 1;
    if (d[0x7e] && g_6cfd34 && !Music_IsPlaying(g_6cfd34)) {
        d[0x7e] = 0;
        if (F<int32_t>(d, 0x8d) == 3 && F<int32_t>(d, 0x91) > 2) {
            if (F<int32_t>(d, 0x91) == 3) {
                CampaignClip462(buf, 0x621db8, 1);
            } else {
                F<int32_t>(d, 0x91) = 2;
                CampaignClip462(buf, 0x621dd4, 3);
            }
            void* digital = (void*)(uintptr_t)Sound_Get2c(g_sound);
            Music_Play(g_6cfd34, 0, buf, digital, nullptr);
            d[0x7e] = 1;
        }
        g_6cfd2c = ClockMs();
        F<int32_t>(d, 0x95) = g_6cfd2c;
        return 0;
    }
    if (hook::As<int32_t>(g_6cfd28) && !d[0x7e] && ClockMs() - g_6cfd2c > g_6cfd30 * 0x1388 + 0x5dc) {
        CampaignTip462(d, text);
        if (g_6cfd30 < 2) ++g_6cfd30;
        return 0;
    }
    const int32_t quiet = ClockMs() - F<int32_t>(d, 0x95);
    if (g_6cfd34 && !Music_IsPlaying(g_6cfd34) && quiet > 0x3a98) {
        int32_t stage = F<int32_t>(d, 0x8d);
        if (stage == 3 && quiet < 0x61a8 && (!d[0x80] || quiet < 0x1f40)) return 0;
        bool advanced = false;
        if (stage < 3) {
            F<int32_t>(d, 0x8d) = stage + 1;
            advanced = true;
        }
        stage = F<int32_t>(d, 0x8d);
        if (stage == 1) {
            CampaignClip462(buf, 0x621d90, 5);
        } else if (stage == 2) {
            const int32_t r = Rand(0, 3);
            if (r == 1) d[0x80] = 1;
            FormatText(buf, (const char*)0x5d05a8, *(const char* const*)(0x621da8 + r * 4), 0, 0, 0, 0, 0, 0, 0);
        } else {
            if (d[0x80])
                F<int32_t>(d, 0x99) = 1;
            else if (advanced)
                F<int32_t>(d, 0x99) = Rand(0, 1);
            else
                F<int32_t>(d, 0x99) = F<int32_t>(d, 0x99) == 0 ? 1 : 0;
            if (F<int32_t>(d, 0x99)) {
                CampaignClip462(buf, 0x621db8, 1);
                F<int32_t>(d, 0x91) = 3;
            } else {
                CampaignClip462(buf, 0x621dc8, 2);
                F<int32_t>(d, 0x91) = 1;
            }
        }
        void* digital = (void*)(uintptr_t)Sound_Get2c(g_sound);
        Music_Play(g_6cfd34, 0, buf, digital, nullptr);
        d[0x7e] = 1;
        F<int32_t>(d, 0x95) = ClockMs();
    }
    if (g_624308 != -1) {
        Window_SendEvent5(d, 0, 0x8c, 0xc351, hook::As<uint32_t>(g_624308), 0, 0);
        g_624308 = -1;
    }
    return 1;
}
RT3_REPLACE(0x4b90d0, CampaignScreen_HandleEvent);

// 0x4b99c0. ID of the n-th (0-based) cargo type flagged in 0x6cfd38; 0 if none.
RT3_NOINLINE uint32_t __fastcall CargoTypes_NthFlagged(int32_t n) {
    Collection* types = g_cargoTypes;
    for (int32_t i = 0; i < Collection_Count(types); ++i) {
        if (!((const uint8_t*)0x6cfd38)[Collection_IdAt(types, 0, i, 0)]) continue;
        if (n == 0) return Collection_IdAt(types, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x4b99c0, CargoTypes_NthFlagged);

// 0x4b9a20. The train-order editor's buttons for the edited stop (0x6cfe04, flags
// +0x28: 0x10 / 0x20 two load modes, 0x40 no cargo choice; cargo +0x24): the mode
// buttons (both bits: 0x7d11 / 0x7d12 / 0x7d16; exactly one: 0x7d13 / 0x7d14;
// neither: 0x7d07 / 0x7d08, any: 0x7d15; 0x20: 0x7d17 / 0x7d1a; 0x10 only: 0x7d18 /
// 0x7d1b; both: 0x7d19 / 0x7d1c) and the cargo row 0x7f58.. matching the cargo
// (ids >= 900 are rows 0..; else 3 + its place among the flagged cargo types).
RT3_NOINLINE void __fastcall TrainOrders_UpdateButtons(uint8_t* w) {
    auto f = []() { return g_6cfe04[0x28]; };
    auto both = [&]() { return (f() & 0x30) == 0x30 ? 0x65u : 0x87u; };
    auto one = [&]() { return ((f() >> 5) & 1) != ((f() >> 4) & 1) ? 0x65u : 0x87u; };
    auto neither = [&]() { return f() & 0x30 ? 0x87u : 0x65u; };
    auto only10 = [&]() { return (f() & 0x30) == 0x10 ? 0x65u : 0x87u; };
    Window_SendEvent5(w, 0, both(), 0x7d11, 3, 0, 0);
    Window_SendEvent5(w, 0, one(), 0x7d13, 3, 0, 0);
    Window_SendEvent5(w, 0, neither(), 0x7d07, 3, 0, 0);
    Window_SendEvent5(w, 0, both(), 0x7d12, 3, 0, 0);
    Window_SendEvent5(w, 0, one(), 0x7d14, 3, 0, 0);
    Window_SendEvent5(w, 0, neither(), 0x7d08, 3, 0, 0);
    Window_SendEvent5(w, 0, both(), 0x7d16, 3, 0, 0);
    Window_SendEvent5(w, 0, f() & 0x30 ? 0x65 : 0x87, 0x7d15, 3, 0, 0);
    Window_SendEvent5(w, 0, f() & 0x20 ? 0x65 : 0x87, 0x7d17, 3, 0, 0);
    Window_SendEvent5(w, 0, f() & 0x20 ? 0x65 : 0x87, 0x7d1a, 3, 0, 0);
    Window_SendEvent5(w, 0, only10(), 0x7d18, 3, 0, 0);
    Window_SendEvent5(w, 0, only10(), 0x7d1b, 3, 0, 0);
    Window_SendEvent5(w, 0, both(), 0x7d19, 3, 0, 0);
    Window_SendEvent5(w, 0, both(), 0x7d1c, 3, 0, 0);
    uint32_t cargo = F<uint32_t>(g_6cfe04, 0x24);
    int32_t row;
    if (cargo >= 900) row = (int32_t)(cargo - 900);
    else {
        row = 3;
        for (int32_t i = 0; i < Collection_Count(g_cargoTypes); ++i) {
            uint32_t id = Collection_IdAt(g_cargoTypes, 0, i, 0);
            if (!((const uint8_t*)0x6cfd38)[id]) continue;
            if (id == F<uint32_t>(g_6cfe04, 0x24)) break;
            ++row;
        }
    }
    for (int32_t id = 0x7f58; id <= 0x801f; ++id)
        Window_SendEvent5(w, 0, id - 0x7f58 == row && !(f() & 0x40) ? 0x65 : 0x87, (uint32_t)id, 1, 0, 0);
}
RT3_REPLACE(0x4b9a20, TrainOrders_UpdateButtons);

RT3_NOINLINE int32_t Global6cfe00Set() { return hook::As<void*>(g_6cfe00) ? 1 : 0; }
RT3_REPLACE(0x4b9d70, Global6cfe00Set);

void SetGlobal6cfe10() { g_6cfe10 = 1; }
RT3_REPLACE(0x4b9d80, SetGlobal6cfe10);

// 0x4b9d90. Next active site among the orders of the edited list 0x6cfe08
// after the cursor 0x621df4 (wrapping); 0 if none.
RT3_NOINLINE void* OrderEdit_NextActiveSite() {
    uint8_t* list = g_6cfe08;
    int32_t cur = g_621df4;
    for (int32_t i = (cur + 1) % F<int32_t>(list, 4); i != g_621df4;
         i = (i + 1) % F<int32_t>(g_6cfe08, 4)) {
        uint8_t* e = (uint8_t*)Array33_At(hook::As<void*>(g_6cfe08), 0, i);
        if ((int8_t)e[0x28] < 0) continue;
        e = (uint8_t*)Array33_At(hook::As<void*>(g_6cfe08), 0, i);
        uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(e, 0x29));
        if (s && Obj_BuildingStateActive(s)) return s;
    }
    return nullptr;
}
RT3_REPLACE(0x4b9d90, OrderEdit_NextActiveSite);

// 0x4b9e10 (callback). Sends the consist being built (0x6cfe04, 51 bytes,
// for the selected train, with the button state and 0x621df4) to the host
// as message 0x43 and shows the wait cursor. Not with 0x6d3ae4 or without
// 0x6cfe0c.
RT3_NOINLINE void __fastcall Consist_SendCb(void*, int, int32_t on) {
    if (g_6d3ae4 || !g_6cfe0c) return;
    uint8_t* b = ((uint8_t*)crt::malloc(0x3f));
    F<int32_t>(b, 0) = Global6d40e4();
    F<int32_t>(b, 4) = SelectedTrainId();
    F<uint16_t>(b, 8) = on != 0;
    F<uint16_t>(b, 0xa) = hook::As<uint16_t>(g_621df4);
    memcpy(b + 0xc, hook::As<const void*>(g_6cfe04), 0x33);
    uint8_t* s = g_session;
    Net_Send(s, 0, (const char*)(s + 0x8f48), 0x43, 0x3f, b, 5, 0);
    crt::free(b);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
}
RT3_REPLACE(0x4b9e10, Consist_SendCb);

// 0x4b9ec0. Copies the edited order's settings (0x6cfe04) into every order of the list 0x6cfe08.
RT3_NOINLINE void OrderEdit_ApplyToAll() {
    for (int32_t i = 0; i < F<int32_t>(g_6cfe08, 4); ++i) {
        uint8_t* e = (uint8_t*)Array33_At(hook::As<void*>(g_6cfe08), 0, i);
        auto src = []() { return g_6cfe04; };
        F<uint32_t>(e, 0x24) = F<uint32_t>(src(), 0x24);
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x40);
        for (int k = 0; k < 8; ++k) F<uint32_t>(e, 1 + k * 4) = F<uint32_t>(src(), 1 + k * 4);
        e[0x23] = src()[0x23];
        e[0x22] = src()[0x22];
        e[0x21] = src()[0x21];
        e[0] = src()[0];
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x20);
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x10);
    }
}
RT3_REPLACE(0x4b9ec0, OrderEdit_ApplyToAll);

// 0x4b9fd0. Tooltip for the train orders' cargo buttons (ids from 0x7f58): the
// first three are fixed texts (table 0x621e04; sic: lower ids read before it);
// the others show the n-th flagged cargo at the edited stop's station (0x6cfe04
// +0x29): its stock ("%.1f"-style format 0x5d05ec) and demand (money style,
// strings 0xf52 / 0xba5 for cargo +0x46 or not) and, for non-+0x46 cargos, the
// demand at the next stop with a +/- difference (string 0xba6).
void __fastcall TrainOrders_CargoTooltip(int32_t id, int, int32_t, int32_t) {
    char text[0x1e0];
    text[0] = 0;
    int32_t k = id - 0x7f58;
    if (k < 3) FormatText(text, GetString(((const int32_t*)0x621e04)[k]), 0, 0, 0, 0, 0, 0, 0, 0);
    else {
        uint32_t cargo = CargoTypes_NthFlagged(k - 3);
        uint8_t* rec = (uint8_t*)Collection_Get(g_cargoTypes, 0, cargo);
        uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(g_6cfe04, 0x29));
        if (st && rec) {
            float stock = 0.0f, quota = 0.0f;
            Obj_CargoStockAndQuota(st, 0, (int32_t)cargo, &stock, &quota);
            int32_t demand = Ftol((double)quota / *(const double*)rt3::Rdata(0x5c87f8));
            char sd[0x1a0], ss[0x190];
            Num_FormatSimple(sd, 9, (double)demand, 0xb);
            crt::sprintf(ss, (const char*)0x5d05ec, (double)stock);
            const char* name = (const char*)st + 0x46b;
            if (rec[0x46]) FormatText(text, GetString(0xf52), LocalizeName((const char*)rec + 4), name, ss, 0, 0, 0, 0, 0);
            else FormatText(text, GetString(0xba5), LocalizeName((const char*)rec + 4), name, ss, sd, 0, 0, 0, 0);
            uint8_t* next = (uint8_t*)OrderEdit_NextActiveSite();
            if (next && !rec[0x46]) {
                int32_t there = Ftol((double)City_CargoQuota(Obj_BuildingRegion(next), 0, (int32_t)cargo) / *(const double*)rt3::Rdata(0x5c87f8));
                int32_t diff = there - demand;
                const char* sign = diff < 0 ? (const char*)0x5ca2b4 : diff > 0 ? (const char*)0x5ca2b8 : (const char*)0x5c87a8;
                char sa[0x1a0], sv[0x1a0], line[0x1e0];
                char* pa = Num_FormatSimple(sa, 9, (double)(diff < 0 ? -diff : diff), 0xb);
                char* pv = Num_FormatSimple(sv, 9, (double)there, 0xb);
                FormatText(line, GetString(0xba6), (const char*)next + 0x46b, pv, sign, pa, 0, 0, 0, 0);
                strcat(text, line);
            }
        }
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
}
RT3_REPLACE(0x4b9fd0, TrainOrders_CargoTooltip);

// 0x4ba270. Order editor (window 0x6cfe00, order 0x6cfe04, edit count
// 0x6cfe0c): slider ctrl moved to `value`. The minimum sliders (0x7d07 /
// 0x7d11 / 0x7d13, advancing tutorial step 0x19a) set +0x22, pushing the
// maximum +0x23 (and its slider, the next id) up; the maximum sliders
// (0x7d08 / 0x7d12 / 0x7d14) set +0x23, pushing the minimum down; 0x7d0e
// sets +0x21. Changes count as edits and, with "apply to all" (0x6cfe08
// +0xc), go to every order.
void __fastcall OrderEdit_OnSlider(uint8_t* ctrl, int32_t value) {
    static const uint8_t kind[14] = { 0, 1, 3, 3, 3, 3, 3, 2, 3, 3, 0, 1, 0, 1 };
    uint32_t k = (uint32_t)(F<uint16_t>(ctrl, 4) - 0x7d07);
    if (k > 0xd) return;
    uint8_t* r = g_6cfe04;
    switch (kind[k]) {
    case 0:
        if (Tutorial_Is19a()) Tutorial_Advance();
        r = g_6cfe04;
        if (value > r[0x23]) {
            ++g_6cfe0c;
            r[0x23] = (uint8_t)value;
            g_6cfe04[0x22] = (uint8_t)value;
            Window_SendEvent5(g_6cfe00, 0, 0x66, F<uint16_t>(ctrl, 4) + 1u, g_6cfe04[0x23], 0, 0);
        } else {
            if (value != r[0x22]) ++g_6cfe0c;
            r[0x22] = (uint8_t)value;
        }
        break;
    case 1:
        if (value < r[0x22]) {
            ++g_6cfe0c;
            r[0x23] = (uint8_t)value;
            g_6cfe04[0x22] = (uint8_t)value;
            Window_SendEvent5(g_6cfe00, 0, 0x66, F<uint16_t>(ctrl, 4) - 1u, g_6cfe04[0x22], 0, 0);
        } else {
            if (value != r[0x23]) ++g_6cfe0c;
            r[0x23] = (uint8_t)value;
        }
        break;
    case 2:
        ++g_6cfe0c;
        g_6cfe04[0x21] = (uint8_t)value;
        break;
    default:
        return;
    }
    if (g_6cfe08[0xc]) OrderEdit_ApplyToAll();
}
RT3_REPLACE(0x4ba270, OrderEdit_OnSlider);

// 0x41b080. Places a rail car on the track: both bogies are found by moving
// half the car length either way from its node position (+0x356, +0x35e,
// direction +0x35a); the car sits between them (offset sideways on curves of
// kinds 4 / 2), headed and pitched along them, with the bogie models turned
// to their track (+0x31a parts); then the wheels roll by `dist`. Off the
// track ends it keeps the node heading.
RT3_NOINLINE void __fastcall Car_UpdatePose(uint8_t* car, int, float dist) {
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)car)[0x14 / 4])(car)) return;
    float lenF = (float)(Obj_Float242(car, 0, 1) / Fl_205(0x5c8700));
    float lenR = (float)(Obj_Float242(car, 0, 0) / Fl_205(0x5c8700));
    Collection* nodes = g_tracks;
    uint8_t* node = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(car, 0x356));
    if (!node) return;
    int32_t dir1 = F<int32_t>(car, 0x35a), dir2 = dir1 == 0;
    // A bogie that runs off a track end gets no point from Node_Advance; the
    // original then uses its stack leftovers, here 0 (counted, see Train_Run).
    float x1 = 0.0f, z1 = 0.0f, x2 = 0.0f, z2 = 0.0f, at1 = 0.0f, at2 = 0.0f;
    int32_t r1 = Node_Advance(node, 0, F<float>(car, 0x35e), lenF, &x1, &z1, &at1, &dir1, nullptr, nullptr, -1, -1, 1);
    int32_t r2 = Node_Advance(node, 0, F<float>(car, 0x35e), lenR, &x2, &z2, &at2, &dir2, nullptr, nullptr, -1, -1, 0);
    if (r1 == -1 || r2 == -1) ++g_unwrittenReads[3];
    float sum = (float)((double)lenR + lenF);
    float t = (float)((double)lenR / sum);
    float mx = (float)(((double)x1 - x2) * t + x2);
    float mz = (float)(((double)z1 - z2) * t + z2);
    uint8_t kind = node[0x218];
    if (kind == 4 || kind == 2) {
        float ang = (float)(D(0x5c97e8) - ((double)Vehicle_ScreenHeading(node, 0, F<int32_t>(car, 0x35a)) + D(0x5c97f0)));
        if (kind == 4) {
            mx = (float)(crt::sin(ang) * D(0x5c9740) + mx);
            double mzd = mz;
            mz = (float)(mzd - crt::cos(ang) * D(0x5c9740));
        } else {
            double c = crt::sin(ang) * D(0x5c9740);
            mx = (float)(c * ((D(0x5c97e0) - F<float>(car, 0x35e)) / D(0x5c97e0)) + mx);
            double mzd = mz;
            double s = crt::cos(ang) * D(0x5c9740);
            mz = (float)(mzd - s * ((D(0x5c97e0) - F<float>(car, 0x35e)) / D(0x5c97e0)));
        }
    }
    if (r1 == -1 || r2 == -1) {
        if (!Map_Contains(g_map, 0, mx, mz)) return;
        float y = Car_PoseHeight(node, F<float>(car, 0x35e));
        Obj_MoveToCell(car, 0, mx, mz, y);
        float h = Vehicle_ScreenHeading(node, 0, F<int32_t>(car, 0x35a));
        Model_SetAngles(car, 0, 0, FBits(h), 0);
        return;
    }
    float h1 = Vehicle_ScreenHeading((uint8_t*)Collection_Get(nodes, 0, (uint32_t)r1), 0, dir1);
    float h2 = Vehicle_ScreenHeading((uint8_t*)Collection_Get(nodes, 0, (uint32_t)r2), 0, dir2 == 0);
    h1 = Car_BogieHeading(r1, at1, dir1, h1, false);
    h2 = Car_BogieHeading(r2, at2, dir2, h2, true);
    float delta = (float)crt::fabs(AngleDelta(nullptr, 0, h1, h2));
    float heading;
    if (!(delta <= D(0x5c97d0)) && delta < D(0x5c97c8)) heading = h1;
    else if ((double)h2 + D(0x5c96a8) < h1) heading = (float)(((double)h2 + D(0x5c96b8) + h1) / D(0x5c85d0));
    else if ((double)h1 + D(0x5c96a8) < h2) heading = (float)(((double)h1 + D(0x5c96b8) + h2) / D(0x5c85d0));
    else heading = (float)(((double)h2 + h1) / Fl_205(0x5c878c));
    float y1 = (float)(Effect_Blend((uint8_t*)Collection_Get(nodes, 0, (uint32_t)r1), 0, (float)((double)at1 / D(0x5c97e0))) / Fl_205(0x5c8700));
    float y2 = (float)(Effect_Blend((uint8_t*)Collection_Get(nodes, 0, (uint32_t)r2), 0, (float)((double)at2 / D(0x5c97e0))) / Fl_205(0x5c8700));
    float pitch = (float)crt::atan(((double)y1 - y2) / sum);
    float y = Car_PoseHeight(node, F<float>(car, 0x35e));
    Obj_MoveToCell(car, 0, mx, mz, y);
    Model_SetAngles(car, 0, FBits(pitch), FBits(heading), 0);
    int32_t parts = F<int32_t>(car, 0x31a);
    if (parts >= 2) {
        Model_SetPartAngle2(car, 0, 1, FBits((float)((double)h1 - heading)));
        if (parts > 2) Model_SetPartAngle2(car, 0, 2, FBits((float)((double)h2 - heading)));
    }
    Vehicle_RollWheels(car, 0, dist);
}

// When a bogie runs off a track end, Node_Advance returns -1 without writing
// its x / z outputs, and the original places the car using whatever its stack
// slots still held (its frame is realigned to 8 bytes, so not even the slots
// are fixed); here they start at 0. Exact otherwise (difftest game_CarUpdatePose).
RT3_REPLACE(0x41b080, Car_UpdatePose);

// 0x4ba3d0. The building type editor's cargo panel for the edited type
// (0x6cfe04: +0 cargo count, +1 cargo ids, +0x21 selection, +0x28 flags):
// per cargo a row (frame 0x7d64 + i, the cargo icon button 0x7d96 + i with
// tooltip 0x1f4, a hover cell 0x7fd6 + i), the extra rows for the flags 0x10 /
// 0x20, their hover cells 0x8026 / 0x8027, and the count list 0x7d0e (one more
// than the cargo count, filler 0x4ba270).
RT3_NOINLINE void __fastcall IndustryEditor_BuildCargoPanel(uint8_t* w) {
    uint8_t* rec = g_6cfe04;
    auto clear = [&](int32_t id) {
        if (Widget_FindChild(w, 0, id)) Window_DeleteChildren(w, 0, id, 0xfff0, 0);
    };
    auto scrollbar = [&](uint8_t* def) {
        uint8_t* s = (uint8_t*)crt::operator_new(0x111);
        s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
        Window_AddChild(w, 0, s, 0);
    };
    auto timer = [&](uint16_t id, uint32_t x) {
        uint8_t def[0x8b];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = 0x3e822222;
        F<uint32_t>(def, 0xf) = 0x3d4ccccd;
        F<uint32_t>(def, 0x13) = 0x3d50369d;
        F<uint16_t>(def, 5) = 0x7d06;
        def[0x29] = 0x21;
        F<uint16_t>(def, 0) = id;
        uint8_t* t = (uint8_t*)crt::operator_new(0xda);
        t = t ? TimerWidget_Construct(t, 0, def) : nullptr;
        Window_AddChild(w, 0, t, 0);
    };
    auto rowX = [](int32_t y) {
        float f = (float)y;
        return FBits((float)((double)f / Fl_205(0x5ca198)));
    };
    if (rec[0]) {
        int32_t id = 0x7d96, y = 0x217;
        uint32_t off = 0;
        do {
            clear(id - 0x32);
            uint8_t def[0x89];
            memset(def, 0, sizeof def);
            uint32_t x = rowX(y);
            F<uint16_t>(def, 0) = (uint16_t)(id - 0x32);
            F<uint32_t>(def, 0xb) = 0x3e822222;
            F<uint32_t>(def, 0xf) = 0x3d4ccccd;
            F<uint32_t>(def, 0x13) = 0x3d50369d;
            F<uint16_t>(def, 5) = 0x7d06;
            F<uint16_t>(def, 0x5f) = 9;
            F<uint32_t>(def, 7) = x;
            String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0658);
            scrollbar(def);
            crt::free(F<void*>(def, 0x43));
            clear(id);
            memset(def, 0, sizeof def);
            F<uint32_t>(def, 0xb) = 0x3e8740da;
            F<uint32_t>(def, 0xf) = 0x3ccccccd;
            F<uint32_t>(def, 0x13) = 0x3d088889;
            F<uint32_t>(def, 7) = rowX(y + 0x17);
            char buf[0x100];
            int32_t cargo = F<int32_t>(g_6cfe04, off + 1);
            if (Collection_IsValid(g_cargoTypes, 0, (uint32_t)cargo))
                crt::sprintf(
                    buf, (const char*)0x5d0650,
                    (const char*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(g_6cfe04, off + 1)) + 4);
            else {
                cargo = F<int32_t>(g_6cfe04, off + 1);
                const char* fmt = cargo == 0x384 ? (const char*)0x5d0640 : cargo == 0x385 ? (const char*)0x5d0630 : (const char*)0x5d0620;
                FormatText(buf, fmt, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            String_Assign(&F<char*>(def, 0x43), NameTable_ContainsName(g_pack, 0, buf, 0, 0) ? buf : (const char*)0x5d0194);
            F<uint16_t>(def, 0) = (uint16_t)id;
            F<uint16_t>(def, 5) = 0x7d06;
            F<uint32_t>(def, 0x2f) = 0x800;
            String_Assign(&F<char*>(def, 0x3f), GetString(0x1f4));
            scrollbar(def);
            crt::free(F<void*>(def, 0x43));
            crt::free(F<void*>(def, 0x3f));
            clear(id + 0x28a);
            timer((uint16_t)(id + 0x28a), x);
            off += 4;
            ++id;
            y += 0x58;
        } while (id - 0x7d96 < g_6cfe04[0]);
    }
    auto button = [&](uint16_t id, uint32_t x, bool narrow, uint32_t image, uint16_t kind, uint32_t flags) {
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = narrow ? 0x3e8740da : 0x3e822222;
        F<uint32_t>(def, 0xf) = narrow ? 0x3ccccccd : 0x3d4ccccd;
        F<uint32_t>(def, 0x13) = narrow ? 0x3d088889 : 0x3d50369d;
        F<uint16_t>(def, 5) = 0x7d06;
        F<uint16_t>(def, 0) = id;
        F<uint16_t>(def, 0x5f) = kind;
        if (flags) F<uint32_t>(def, 0x2f) = flags;
        String_Assign(&F<char*>(def, 0x43), (const char*)image);
        scrollbar(def);
        crt::free(F<void*>(def, 0x43));
    };
    uint8_t flags = g_6cfe04[0x28];
    if (flags & 0x30) button(0x7d6b, 0x3f3828f6, false, 0x5d0658, 9, 0);
    flags = g_6cfe04[0x28];
    if ((flags & 0x20) && (flags & 0x10)) button(0x7d6a, 0x3f2a147b, false, 0x5d0658, 9, 0);
    if (g_6cfe04[0x28] & 0x20) button(0x7d9d, 0x3f3bd70a, true, 0x5d0614, 0, 0x30);
    flags = g_6cfe04[0x28];
    if (flags & 0x10) {
        int32_t row = (flags & 0x20) ? 6 : 7;
        button((uint16_t)(row + 0x7d96), rowX(row * 0x58 + 0x22e), true, 0x5d0608, 0, 0x30);
    }
    flags = g_6cfe04[0x28];
    if ((flags & 0x10) && (flags & 0x20)) {
        clear(0x8026);
        timer(0x8026, 0x3f2a147b);
    }
    if (g_6cfe04[0x28] & 0x30) {
        clear(0x8027);
        timer(0x8027, 0x3f3828f6);
    }
    clear(0x7d0e);
    uint8_t def[0xe3];
    memset(def, 0, sizeof def);
    uint8_t* r = g_6cfe04;
    F<uint32_t>(def, 7) = 0x3e8e6666;
    F<uint32_t>(def, 0xb) = 0x3e6c5f93;
    F<float>(def, 0xf) = (float)(((double)(r[0] * 0x59) + D(0x5d0600)) / D(0x5ca2c8));
    F<uint32_t>(def, 0x13) = 0x3cda740e;
    F<int32_t>(def, 0x5f) = r[0] + 1;
    F<int32_t>(def, 0x63) = 0;
    F<int32_t>(def, 0x67) = 1;
    F<int32_t>(def, 0x97) = 5;
    F<int32_t>(def, 0x9b) = r[0] + 0xa;
    F<uint32_t>(def, 0xab) = 0xbc088889;
    F<uint32_t>(def, 0xaf) = 0x3c7ae148;
    F<float>(def, 0xb3) = (float)(((double)(r[0] * 0x59) + D(0x5d05f8)) / D(0x5ca2c8));
    F<uint32_t>(def, 0xb7) = 0;
    F<uint32_t>(def, 0xbb) = 0;
    F<uint16_t>(def, 0x3c) = 0xb07;
    F<uint32_t>(def, 0x2f) = 8;
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0658);
    F<uint16_t>(def, 0) = 0x7d0e;
    F<uint16_t>(def, 5) = 0x7d06;
    uint8_t* l = (uint8_t*)crt::operator_new(0x1b9);
    l = l ? ListBox_Construct(l, 0, def) : nullptr;
    Window_AddChild(w, 0, l, 0);
    crt::free(F<void*>(def, 0x43));
    Window_SendEvent5(w, 0, 0x84, 0x7d0e, 0x4ba270, 0, 0);
    Window_SendEvent5(w, 0, 0x73, 0x7d0e, g_6cfe04[0] + 1, 0, 0);
    Window_SendEvent5(w, 0, 0x66, 0x7d0e, g_6cfe04[0x21], 0, 0);
}
RT3_REPLACE(0x4ba3d0, IndustryEditor_BuildCargoPanel);

// 0x4bad20. Shows tab `tab` of the building type editor (tab buttons 0x7d05 /
// 0x7d06): tab 0 sets the two size lists for the edited type's shape (flags
// 0x20 / 0x10 of 0x6cfe04 +0x28: 0x7d11 / 0x7d12 when both, 0x7d07 / 0x7d08
// when neither, else 0x7d13 / 0x7d14) to +0x22 / +0x23 and updates the
// buttons; tab 1 sizes the cargo count list 0x7d0e and rebuilds the cargo panel.
RT3_NOINLINE void __fastcall IndustryEditor_ShowTab(uint8_t* w, int, int32_t tab) {
    for (int32_t i = 0; i < 2; ++i) Window_SendEvent5(w, 0, i == tab ? 0x6d : 0x86, 0x7d05 + i, 3, 0, 0);
    auto rec = [] { return g_6cfe04; };
    if (tab) {
        Window_SendEvent5(w, 0, 0x73, 0x7d0e, rec()[0] + 1, 0, 0);
        Window_SendEvent5(w, 0, 0x66, 0x7d0e, rec()[0x21], 0, 0);
        IndustryEditor_BuildCargoPanel(w);
        return;
    }
    uint8_t flags = rec()[0x28];
    bool b20 = (flags >> 5) & 1;
    int32_t first, count;
    if (b20 && (flags & 0x10)) first = 0x7d11, count = 7;
    else if (!b20 && !(flags & 0x10)) first = 0x7d07, count = 9;
    else first = 0x7d13, count = 8;
    Window_SendEvent5(w, 0, 0x73, first, count, 0, 0);
    Window_SendEvent5(w, 0, 0x66, first, rec()[0x22], 0, 0);
    Window_SendEvent5(w, 0, 0x73, first + 1, count, 0, 0);
    Window_SendEvent5(w, 0, 0x66, first + 1, rec()[0x23], 0, 0);
    TrainOrders_UpdateButtons(w);
}
RT3_REPLACE(0x4bad20, IndustryEditor_ShowTab);

// 0x4baef0. Rebuilds the building type editor for the edited type (0x6cfe04):
// the shape checkboxes, then the cargo grid: three header cells (strings
// 0x621df8, images 0x621e10, slider frames) and one cell per cargo already
// available (marked in 0x6cfd38 by id, counted in 0x6cfe14 plus the headers),
// five per row with the row height shrunk for more than 6 rows; each cell
// has the cargo name (greyed 0xffbfb789 when the edited station has no stock
// of it), its icon and a slider (0x7f58 + n, callback 0x4b9fd0). Then the
// cargo panel, the buttons, the station name (0x7d0b), the size list fillers
// and the current tab.
RT3_NOINLINE void __fastcall IndustryEditor_Build(uint8_t* w) {
    auto rec = [] { return g_6cfe04; };
    Window_SendEvent5(w, 0, 0x74, 0x7d02, ~(rec()[0x28] >> 6) & 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7d03, (rec()[0x28] >> 6) & 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7d0d, g_6cfe08[0xc], 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7d0f, (rec()[0x28] >> 5) & 1, 0, 0);
    Window_SendEvent5(w, 0, 0x74, 0x7d10, (rec()[0x28] >> 4) & 1, 0, 0);
    static const int32_t ranges[][2] = { { 0x7d0e, 0xfff0 }, { 0x7d96, 0x7dc7 }, { 0x7d64, 0x7d95 }, { 0x7dc8, 0x7e8f },
                                         { 0x7e90, 0x7f57 }, { 0x7f58, 0x801f }, { 0x8020, 0x8051 } };
    for (auto& r : ranges) Window_DeleteChildren(w, 0, r[0], r[1], 0);
    uint8_t* shown = (uint8_t*)0x6cfd38;
    memset(shown, 0, 200);
    g_6cfe14 = 0;
    auto cargo = [] { return g_cargoTypes; };
    for (int32_t j = 0; j < Collection_Count(cargo()); ++j)
        if (Obj_YearReached(Collection_Get(cargo(), 0, Collection_IdAt(cargo(), 0, j, 0)))) {
            shown[Collection_IdAt(cargo(), 0, j, 0)] = 1;
            ++g_6cfe14;
        }
    float y = 70.0f, off = 0.0f, span = 280.0f;
    int32_t n = 0;
    for (int32_t j = 0; j < Collection_Count(cargo()); ++j)
        if (shown[Collection_IdAt(cargo(), 0, j, 0)]) ++n;
    int32_t rows = (n - 1) / 5 + 1;
    float step = 40.0f;
    if (rows >= 8) {
        off = -15.0f;
        span = 292.0f;
    }
    if (rows > 7) {
        y = 50.0f;
        float rf = (float)rows;
        step = (float)((double)span / rf);
    } else if (rows > 6) {
        off = -7.0f;
        span = (float)((double)span + Fl_205(0x5c8554));
        y = 54.0f;
        float rf = (float)rows;
        step = (float)((double)span / rf);
    }
    float t = (float)((double)off + Fl_205(0x5d0680));
    g_6cfe14 += 2;
    float headH = (float)(((double)t + D(0x5c8800)) / D(0x5ca2d0));
    float headY = (float)((double)t / Fl_205(0x5ca164));
    char buf[0x100];
    int32_t cell = 0;
    auto editBox = [&](uint32_t x, uint32_t yy, uint32_t wd, uint32_t h, uint32_t color, uint8_t style, const char* text) {
        uint8_t def[0x91];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = yy;
        F<uint32_t>(def, 0xf) = wd;
        F<uint32_t>(def, 0x13) = h;
        def[0x27] = 3;
        def[0x28] = 0;
        F<uint32_t>(def, 0x34) = color;
        String_Assign(&F<char*>(def, 0x77), text);
        def[0x33] = style;
        F<uint16_t>(def, 0) = (uint16_t)(0x7dc8 + cell);
        uint8_t* b = (uint8_t*)crt::operator_new(0xf0);
        b = b ? EditBox_Construct(b, 0, def) : nullptr;
        Window_AddChild(w, 0, b, 0);
        crt::free(F<void*>(def, 0x77));
    };
    auto picture = [&](uint32_t x, uint32_t yy, uint32_t wd, uint32_t h, const char* fmtArg) {
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<uint16_t>(def, 0) = (uint16_t)(0x7e90 + cell);
        F<uint32_t>(def, 0xb) = yy;
        F<uint32_t>(def, 0xf) = wd;
        F<uint32_t>(def, 0x13) = h;
        F<uint32_t>(def, 0x2f) = 0x800;
        F<uint16_t>(def, 0x5f) = 0;
        F<uint32_t>(def, 7) = x;
        FormatText(buf, (const char*)0x5d01bc, fmtArg, 0, 0, 0, 0, 0, 0, 0);
        String_Assign(&F<char*>(def, 0x43), NameTable_ContainsName(g_pack, 0, buf, 0, 0) ? buf : (const char*)0x5d0194);
        uint8_t* s = (uint8_t*)crt::operator_new(0x111);
        s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
        Window_AddChild(w, 0, s, 0);
        crt::free(F<void*>(def, 0x43));
    };
    auto slider = [&](uint32_t x, uint32_t yy, uint32_t wd, uint32_t h) {
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<uint32_t>(def, 7) = x;
        F<uint32_t>(def, 0xb) = yy;
        F<uint32_t>(def, 0xf) = wd;
        F<uint32_t>(def, 0x13) = h;
        F<uint16_t>(def, 0) = (uint16_t)(0x7f58 + cell);
        F<uint32_t>(def, 0x2f) = 0x800;
        def[0x29] = 0x21;
        F<int32_t>(def, 0x61) = 2;
        uint8_t* s = (uint8_t*)crt::operator_new(0x111);
        s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
        Window_AddChild(w, 0, s, 0);
        Window_SendEvent5(w, 0, 0x82, F<uint16_t>(def, 0), 0x4b9fd0, 0, 0);
    };
    for (int32_t x = 0x242; x < 0x5bd; x += 0x129, ++cell) {
        float fx = (float)(x - 0x104);
        editBox(FBits((float)((double)fx / Fl_205(0x5ca198))), FBits(headY), 0x3e200000, 0x3d2aaaab, 0xfffff799,
                g_language != 3 ? 3 : 2, GetString(hook::Global<int32_t>(0x621df8 + cell * 4)));
        fx = (float)x;
        picture(FBits((float)((double)fx / Fl_205(0x5ca198))), FBits(headH), 0x3ccccccd, 0x3d2aaaab, hook::Global<const char*>(0x621e10 + cell * 4));
        float sx = (float)(x - 0xe6);
        slider(FBits((float)((double)sx / Fl_205(0x5ca198))), FBits(headY), 0x3e2ae148, 0x3d2aaaab);
    }
    y = (float)((double)off + y);
    int32_t shownCount = 0;
    for (int32_t j = 0; j < Collection_Count(cargo()); ++j) {
        if (!shown[Collection_IdAt(cargo(), 0, j, 0)]) continue;
        int32_t col = shownCount % 5;
        if (shownCount > 0 && !col) y = (float)((double)step + y);
        uint32_t color = 0xfffff799;
        if (uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(rec(), 0x29))) {
            float stock, quota;
            if (!Obj_CargoStockAndQuota(st, 0, (int32_t)Collection_IdAt(cargo(), 0, j, 0), &stock, &quota) || !(stock >= D(0x5c9ba0))) color = 0xffbfb789;
        }
        float cf = (float)col;
        uint32_t ex = FBits((float)(((double)cf * Fl_205(0x5d067c) + Fl_205(0x5d0678) - Fl_205(0x5d0674)) / Fl_205(0x5ca198)));
        uint32_t ey = FBits((float)(((double)y + Fl_205(0x5d0670)) / Fl_205(0x5ca164)));
        editBox(ex, ey, 0x3dc28f5c, 0x3cf5c28f, color, 2,
                LocalizeName((const char*)Collection_Get(cargo(), 0, Collection_IdAt(cargo(), 0, j, 0)) + 4));
        cf = (float)col;
        uint32_t px = FBits((float)(((double)cf * Fl_205(0x5d067c) + Fl_205(0x5d066c)) / Fl_205(0x5ca198)));
        uint32_t py = FBits((float)(((double)y + Fl_205(0x5d0668)) / Fl_205(0x5ca164)));
        picture(px, py, 0x3ccccccd, 0x3cf5c28f, (const char*)Collection_Get(cargo(), 0, Collection_IdAt(cargo(), 0, j, 0)) + 4);
        float pad = (float)(col ? 0 : 9);
        cf = (float)col;
        uint32_t sx = FBits((float)(((double)cf * Fl_205(0x5d067c) + Fl_205(0x5d0678) - Fl_205(0x5c8d60) + pad) / Fl_205(0x5ca198)));
        uint32_t sy = FBits((float)(((double)y + Fl_205(0x5d0680)) / D(0x5ca2d0)));
        uint32_t sw = FBits((float)(((double)Fl_205(0x5cfcfc) - pad) / Fl_205(0x5ca198)));
        float h = Fl_205(0x5d0664) > step ? 38.0f : step;
        slider(sx, sy, sw, FBits((float)((double)h / D(0x5ca2d0))));
        ++shownCount;
        ++cell;
    }
    IndustryEditor_BuildCargoPanel(w);
    TrainOrders_UpdateButtons(w);
    uint8_t* nameBox = Widget_FindChild(w, 0, 0x7d0b);
    String_Assign(&F<char*>(nameBox, 0xba), (const char*)Collection_Get(g_stations, 0, F<uint16_t>(rec(), 0x29)) + 0x46b);
    for (int32_t id : { 0x7d07, 0x7d08, 0x7d11, 0x7d12, 0x7d13, 0x7d14 }) Window_SendEvent5(w, 0, 0x84, id, 0x4ba270, 0, 0);
    IndustryEditor_ShowTab(w, 0, (rec()[0x28] >> 6) & 1);
}
RT3_REPLACE(0x4baef0, IndustryEditor_Build);

// 0x4bb9e0 (event handler). The train orders window w for orders 0x6cfe08
// (stop 0x621df4 -> 0x6cfe04; edits counted in 0x6cfe0c; +0xc = apply to
// all stops). A stop (0x33 bytes) holds up to 8 cargo slots (count +0,
// slots +1, shown limit +0x21), a single cargo +0x24, the flags +0x28
// (0x80 waypoint, 0x40 several cargos, 0x20 / 0x10 reserve one slot each
// for a caboose / mail) and the two limits +0x22 / +0x23. Close (0x7d01,
// in a tutorial only when awaited) and the stop steps 0x7d09 / 0x7d0a
// (skipping waypoints and inactive stations) first send the consist in
// multiplayer; slot clicks 0x8020.. drop a slot (or clear a reserve
// flag), cargo clicks 0x7f58.. set / add a cargo (the first three are the
// specials 0x384..), 0x7d02 / 0x7d03 pick single / several cargos, 0x7d0d
// toggles apply-to-all, 0x7d0f / 0x7d10 (and 0x7d1a..0x7d1c, with a click)
// toggle the reserve flags, then the limits and slots are clamped (8, 7
// with one reserve, 6 with both) and the panels rebuilt.
int32_t __fastcall TrainOrders_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    auto stop = []() { return g_6cfe04; };
    auto orders = []() { return g_6cfe08; };
    auto applyAll = [&]() {
        if (orders()[0xc]) OrderEdit_ApplyToAll();
    };
    auto sendConsist = [&]() {
        if (IsMultiplayer()) {
            Consist_SendCb(w, 0, orders()[0xc]);
            g_6cfe0c = 0;
        }
    };
    auto dropPanels = [&]() {
        Window_DeleteChildren(w, 0, 0x7d64, 0x7d95, 0);
        Window_DeleteChildren(w, 0, 0x7d96, 0x7dc7, 0);
        Window_DeleteChildren(w, 0, 0x8020, 0x8051, 0);
    };
    auto cargoOf = [](int32_t v) { return v < 3 ? (uint32_t)(v + 0x384) : CargoTypes_NthFlagged(v - 3); };
    const int32_t id = (int32_t)msg[1];
    switch (msg[0]) {
    case 0xc9:
        return 1;
    case 0:
        if (g_6cfe10) {
            g_6cfe10 = 0;
            msg[0] = 0xc9;
        }
        return 1;
    case 0xcb:
        if (id == 0x7d01) {
            sendConsist();
            if (g_6d3b4c && msg[1] != g_622b38) {
                Dialog_RunMessage(GetString(0xe03), 1, 0x3ef5c28f, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
                return 0;
            }
            msg[0] = 0xc9;
            return 1;
        }
        if (id <= 0x7d08 || id > 0x7d0a) return 1;
        sendConsist();
        for (;;) {
            int32_t cur = g_621df4;
            if ((int32_t)msg[1] == 0x7d09)
                cur = cur > 0 ? cur - 1 : F<int32_t>(orders(), 4) - 1;
            else
                cur = cur < F<int32_t>(orders(), 4) - 1 ? cur + 1 : 0;
            g_621df4 = cur;
            uint8_t* e = (uint8_t*)Array33_At(orders(), 0, cur);
            g_6cfe04 = e;
            if (e[0x28] & 0x80) continue;
            if (Obj_BuildingStateActive(Collection_Get(g_stations, 0, F<uint16_t>(e, 0x29)))) break;
        }
        IndustryEditor_Build(w);
        return 0;
    case 0xca:
        break;
    default:
        return 1;
    }
    int32_t both = 6;
    if (id >= 0x8020 && id <= 0x8051) {
        ++g_6cfe0c;
        const int32_t k = id - 0x8020;
        uint8_t* e = stop();
        const uint8_t fl = e[0x28];
        bool dropSlot = true;
        if (k == 6) {
            if ((fl & 0x20) && (fl & 0x10)) {
                e[0x28] = fl & 0xef;
                Window_SendEvent5(w, 0, 0x74, 0x7d10, 0, 0, 0);
                dropSlot = false;
            }
        } else if (k == 7) {
            if ((fl & 0x10) && !(fl & 0x20)) {
                e[0x28] = fl & 0xef;
                Window_SendEvent5(w, 0, 0x74, 0x7d10, 0, 0, 0);
                dropSlot = false;
            } else if (fl & 0x20) {
                e[0x28] = fl & 0xdf;
                Window_SendEvent5(w, 0, 0x74, 0x7d0f, 0, 0, 0);
                dropSlot = false;
            }
        }
        if (dropSlot) {
            F<uint32_t>(e, 1 + k * 4) = 0;
            int32_t i = k;
            for (; i < stop()[0] - 1; ++i) F<uint32_t>(stop(), 1 + i * 4) = F<uint32_t>(stop(), 5 + i * 4);
            F<uint32_t>(stop(), 1 + i * 4) = 0;
            --stop()[0];
            if (stop()[0x21] > stop()[0]) stop()[0x21] = stop()[0];
            dropPanels();
            IndustryEditor_BuildCargoPanel(w);
            applyAll();
            return 0;
        }
    } else if (id >= 0x7f58 && id <= 0x801f) {
        ++g_6cfe0c;
        uint8_t* e = stop();
        const int32_t v = id - 0x7f58;
        const uint8_t fl = e[0x28];
        if (!(fl & 0x40)) {
            if (v < 3)
                F<uint32_t>(e, 0x24) = (uint32_t)(v + 0x384);
            else
                F<uint32_t>(stop(), 0x24) = CargoTypes_NthFlagged(v - 3);
            TrainOrders_UpdateButtons(w);
            applyAll();
            return 0;
        }
        int32_t max = fl & 0x20 ? 7 : 8;
        if (fl & 0x10) --max;
        if (e[0] == max) return 0;
        ++e[0];
        if (v < 3)
            F<uint32_t>(stop(), stop()[0] * 4 - 3) = (uint32_t)(v + 0x384);
        else {
            const uint32_t c = cargoOf(v);
            F<uint32_t>(stop(), stop()[0] * 4 - 3) = c;
        }
        IndustryEditor_BuildCargoPanel(w);
        applyAll();
        return 0;
    } else {
        switch (id) {
        case 0x7d1a:
            ++g_6cfe0c;
            Ui_PlaySound(0x21, 0, 0x3f000000);
            stop()[0x28] &= 0xdf;
            Window_SendEvent5(w, 0, 0x74, 0x7d0f, 0, 0, 0);
            break;
        case 0x7d1b:
        case 0x7d1c:
            ++g_6cfe0c;
            Ui_PlaySound(0x21, 0, 0x3f000000);
            stop()[0x28] &= 0xef;
            Window_SendEvent5(w, 0, 0x74, 0x7d10, 0, 0, 0);
            break;
        case 0x7d02:
        case 0x7d03:
            ++g_6cfe0c;
            IndustryEditor_ShowTab(w, 0, id - 0x7d02);
            stop()[0x28] = (uint8_t)((stop()[0x28] & ~0x40) | ((id & 1) << 6));
            applyAll();
            TrainOrders_UpdateButtons(w);
            return 0;
        case 0x7d0d:
            ++g_6cfe0c;
            orders()[0xc] = orders()[0xc] == 0;
            applyAll();
            return 0;
        case 0x7d0f:
        case 0x7d10:
            ++g_6cfe0c;
            stop()[0x28] ^= id == 0x7d0f ? 0x20 : 0x10;
            break;
        default:
            return 1;
        }
    }
    // The limits and slots follow the reserve flags.
    uint8_t* e = stop();
    const uint8_t fl = e[0x28];
    int32_t limit = 8;
    uint32_t idHigh = 0x7d08, idLow = 0x7d07;
    if ((fl & 0x20) && (fl & 0x10)) {
        limit = both;
        idHigh = 0x7d12, idLow = 0x7d11;
    } else if ((fl & 0x20) || (fl & 0x10)) {
        limit = 7;
        idHigh = 0x7d14, idLow = 0x7d13;
    }
    if (limit < 8) {
        if (e[0x23] > limit) e[0x23] = (uint8_t)limit;
        if (stop()[0x22] > limit) stop()[0x22] = (uint8_t)limit;
    }
    Window_SendEvent5(g_6cfe00, 0, 0x66, idHigh, stop()[0x23], 0, 0);
    Window_SendEvent5(g_6cfe00, 0, 0x66, idLow, stop()[0x22], 0, 0);
    while (limit < stop()[0]) {
        F<uint32_t>(stop(), stop()[0] * 4 - 3) = 0;
        --stop()[0];
    }
    dropPanels();
    if (stop()[0x28] & 0x40)
        IndustryEditor_BuildCargoPanel(w);
    else
        TrainOrders_UpdateButtons(w);
    return 0;
}
RT3_REPLACE(0x4bb9e0, TrainOrders_HandleEvent);

// 0x4bc0a0. Constructs the building type editor window (vtable 0x5d05e0,
// loaded at x 0.15625 when 0x6d3b4c) and builds it.
RT3_NOINLINE uint8_t* __fastcall IndustryEditor_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d05e0;
    Window_Load(w, 0, g_6d3b4c ? BitsF(0x3e200000) : 0.0f, 0.0f, (const char*)0x5d0684);
    IndustryEditor_Build(w);
    return w;
}
RT3_REPLACE(0x4bc0a0, IndustryEditor_Construct);

// 0x4bc0f0 (a in ECX, record in EDX). Runs the industry editor dialog
// (0x6cfe00) for record `rec` (state 0x6cfe04..0x6cfe10, 0x621df4 = x) when
// its building (+0x29 in 0x6cec20, unless +0x28 is negative) is active.
RT3_NOINLINE void __fastcall IndustryEditor_Run(uint32_t a, uint8_t* rec, uint32_t x) {
    hook::As<uint32_t>(g_6cfe0c) = 0;
    hook::As<uint32_t>(g_6cfe08) = a;
    g_6cfe04 = rec;
    hook::As<uint32_t>(g_621df4) = x;
    if ((int8_t)rec[0x28] < 0) return;
    if (!Obj_BuildingStateActive(Collection_Get(g_stations, 0, F<uint16_t>(rec, 0x29)))) return;
    hook::As<uint32_t>(g_6cfe10) = 0;
    uint8_t* w = (uint8_t*)crt::operator_new(0x78);
    g_6cfe00 = w ? IndustryEditor_Construct(w) : nullptr;
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6cfe00, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6cfe00);
    if (uint8_t* p = g_6cfe00) {
        F<uint32_t>(p, 0) = 0x5d05e0;
        Widget_SetVtbl(p);
        crt::operator_delete(p);
    }
    g_6cfe00 = nullptr;
}
RT3_REPLACE(0x4bc0f0, IndustryEditor_Run);

// 0x4bc1d0. Terrain height tool window ("ChangeHeight.win"); also clears the map's
// fog mode and 0x6cfe18.
void __fastcall ChangeHeight_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0690;
    Scene_PopEngineBusy();
    Map_ClearFogMode(g_map);
    hook::As<int32_t>(g_6cfe18) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4bc1d0, ChangeHeight_Destruct);

// 0x4bc210. Selects water level preset idx (+0x78; table 0x621e24) for the renderer.
RT3_NOINLINE void __fastcall Obj_SetWaterPreset(uint8_t* o, int, int32_t idx) {
    F<int32_t>(o, 0x78) = idx;
    float v = (float)((double)((const int32_t*)0x621e24)[idx] + *(const double*)rt3::Rdata(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, v);
}
RT3_REPLACE(0x4bc210, Obj_SetWaterPreset);

// 0x4bc260. Outputs +0x80, +0x84 and the table value 0x621e24[+0x78] as float.
void __fastcall Obj_GetDims(uint8_t* o, int, int32_t* a, int32_t* b, float* c) {
    *a = F<int32_t>(o, 0x80);
    *b = F<int32_t>(o, 0x84);
    *c = (float)((const int32_t*)0x621e24)[F<int32_t>(o, 0x78)];
}
RT3_REPLACE(0x4bc260, Obj_GetDims);

// 0x4bc290. Highlights selection +0x78 among buttons 0xfa1.. (7) and the current
// view mode (0x6d0818 +0x8c; modes 14..19 map to 0..5) among 0xfaa.. (6).
RT3_NOINLINE void __fastcall Window_HighlightViewTabs(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0xfa1 + i, 1, 0, 0);
    int32_t mode = F<int32_t>(g_gameWindow, 0x8c);
    if (mode >= 0xe && mode <= 0x13) mode -= 0xe;
    for (int32_t i = 0; i < 6; ++i) Window_SendEvent5(w, 0, i != mode ? 0x87 : 0x65, 0xfaa + i, 1, 0, 0);
}
RT3_REPLACE(0x4bc290, Window_HighlightViewTabs);

// 0x4bc350. Map editor terrain tool panel w, event ev. Mouse move (0) tracks
// the cell under the cursor (+0x80 / +0x84) while no stroke runs (+0x7c).
// Button 0x7d6 down (0xca) starts a stroke on the picked cell (unless ev
// +0x20 has 0x800) and applies one strong step; drag (2) applies normal
// steps for the tool (0x6d0818+0x8c) with radius 0x621e24[+0x78], tripled
// with shift / ctrl (0x6d4018+0xa8c): 0xe raises by the vertical mouse
// motion (ev +0x28 since +0x88, x -4000 x 0.6), 0xf smooths, 0x10 / 0x11
// level (to the centre / the average), 0x12 / 0x13 raise / lower by the
// table 0x621e40; the stroke re-picks the cell first (not for 0xe) and
// refreshes the buildings around it. Clicks 0xfa1..0xfa7 pick a water
// preset (0x621e20), 0xfaa..0xfaf the tool; up (0xcb) on 0x7d6 ends the
// stroke, on 0xfb0 asks for a height rescale (24 choices 0x621e60: 0..7
// band 0, 8..13 band 1, the rest band 2; factor = table[choice % 8]).
// Returns 0 when handled.
int32_t __fastcall TerrainTools_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    bool first = false;
    switch (ev[0]) {
    case 0:
        if (!F<int32_t>(w, 0x7c)) Scene_FocusFlagIs1(&F<float>(w, 0x80), &F<float>(w, 0x84));
        return 1;
    case 2:
        break;
    case 0xca: {
        uint32_t id = ev[1];
        if (id >= 0xfa1 && id <= 0xfa7) {
            int32_t k = (int32_t)id - 0xfa1;
            Obj_SetWaterPreset(w, 0, k);
            g_621e20 = k;
            Window_HighlightViewTabs(w);
            return 0;
        }
        if (id >= 0xfaa && id <= 0xfaf) {
            View_SetToolMode(g_gameWindow, 0, id - 0xfaa + 0xe, -1);
            return 0;
        }
        if (id != 0x7d6) return 1;
        float x, y;
        if (!Scene_FocusFlagIs1(&x, &y)) return 0;
        if (ev[8] & 0x800) return 0;
        F<int32_t>(w, 0x7c) = 1;
        F<float>(w, 0x80) = x;
        F<float>(w, 0x84) = y;
        F<uint32_t>(w, 0x88) = ev[10];
        first = true;
        break;
    }
    case 0xcb:
        if (ev[1] == 0x7d6) {
            if (!F<int32_t>(w, 0x7c)) return 1;
            F<int32_t>(w, 0x7c) = 0;
            return 0;
        }
        if (ev[1] == 0xfb0) {
            char* names[0x18] = {};
            for (int32_t k = 0; k < 0x18; ++k) String_Assign(&names[k], GetString(((const int32_t*)0x621e60)[k]));
            int32_t sel = 0;
            if (Dialog_RunMessage(GetString(0xe4e), 2, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0, 0x18, (const char**)names, &sel, 0, 0, nullptr, 0) == 0x3f2) {
                int32_t band = 0;
                if (sel > 7) band = 1;
                if (sel > 0xd) band = 2;
                static const uint32_t kScale[8] = { 0x3d4ccccd, 0x3dcccccd, 0x3e800000, 0x3f000000, 0xbd4ccccd, 0xbdcccccd, 0xbe800000, 0xbf000000 };
                Map_RescaleHeights(g_map, 0, ((const float*)kScale)[sel % 8], band);
            }
            for (int32_t k = 0; k < 0x18; ++k) crt::free(names[k]);
            return 0;
        }
        return 1;
    default:
        return 1;
    }
    if (!F<int32_t>(w, 0x7c)) {
        Scene_FocusFlagIs1(&F<float>(w, 0x80), &F<float>(w, 0x84));
        return 1;
    }
    float dz = F<float>(ev, 0x28) - F<float>(w, 0x88);
    F<uint32_t>(w, 0x88) = ev[10];
    dz = (float)(dz * *(const double*)rt3::Rdata(0x5d06a0));
    float mult = (g_input[0xa8c] & 3) ? 3.0f : 1.0f;
    int32_t r = ((const int32_t*)0x621e24)[F<int32_t>(w, 0x78)];
    int32_t tool = F<int32_t>(g_gameWindow, 0x8c);
    switch (tool) {
    case 0xe:
        if (!(dz == *(const float*)rt3::Rdata(0x5c8598))) {
            float amount = (float)((double)mult * dz * *(const double*)rt3::Rdata(0x5c8880));
            int32_t y = TerrainTools_Round(F<float>(w, 0x84));
            Map_RaiseAround(g_map, 0, TerrainTools_Round(F<float>(w, 0x80)), y, ((const int32_t*)0x621e24)[F<int32_t>(w, 0x78)], amount, 0);
        }
        break;
    case 0xf:
    case 0x10:
    case 0x11: {
        static const uint64_t kStep[3][2] = { { 0x3f947ae147ae147bull, 0x3fb999999999999aull },
                                              { 0x3fa47ae147ae147bull, 0x3fc999999999999aull },
                                              { 0x3f947ae147ae147bull, 0x3fb999999999999aull } };
        float step = (float)*(const double*)&kStep[tool - 0xf][first];
        Scene_FocusFlagIs1(&F<float>(w, 0x80), &F<float>(w, 0x84));
        float amount = step * mult;
        r = ((const int32_t*)0x621e24)[F<int32_t>(w, 0x78)];
        int32_t y = TerrainTools_Round(F<float>(w, 0x84));
        int32_t x = TerrainTools_Round(F<float>(w, 0x80));
        if (tool == 0xf) Map_SmoothAround(g_map, 0, x, y, r, amount);
        else Map_LevelAround(g_map, 0, x, y, r, tool == 0x10 ? 1.0f : 0.0f, amount);
        break;
    }
    case 0x12:
    case 0x13: {
        uint64_t bits = first ? 0x4014000000000000ull : 0x3ff199999999999aull;
        float scale = (float)*(const double*)&bits;
        Scene_FocusFlagIs1(&F<float>(w, 0x80), &F<float>(w, 0x84));
        float v = (float)((const int32_t*)0x621e40)[F<int32_t>(w, 0x78)];
        float amount = (float)((double)v * scale * mult);
        if (tool == 0x13) amount = -(float)((double)v * scale * mult);
        r = ((const int32_t*)0x621e24)[F<int32_t>(w, 0x78)];
        int32_t y = TerrainTools_Round(F<float>(w, 0x84));
        Map_RaiseAround(g_map, 0, TerrainTools_Round(F<float>(w, 0x80)), y, r, amount, 0);
        break;
    }
    default:
        return 1;
    }
    r = ((const int32_t*)0x621e24)[F<int32_t>(w, 0x78)];
    int32_t y = TerrainTools_Round(F<float>(w, 0x84));
    Map_RefreshBuildingsAround(hook::As<uint8_t*>(g_buildingTypes), 0, TerrainTools_Round(F<float>(w, 0x80)), y, r);
    return 1;
}
RT3_REPLACE(0x4bc350, TerrainTools_HandleEvent);

// 0x4bcb50. Terrain height tool window constructor ("ChangeHeight.win"): brush
// size 0x621e20 (+0x78) sets the brush radius (table 0x621e24 + 0.5); the window
// becomes 0x6cfe18, highlights its view tabs, pauses the scene and switches the
// map to fog mode 6.
uint8_t* __fastcall ChangeHeight_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    int32_t size = g_621e20;
    F<uint32_t>(w, 0) = 0x5d0690;
    F<int32_t>(w, 0x78) = size;
    float radius = (float)((double)((const int32_t*)0x621e24)[size] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, radius);
    F<int32_t>(w, 0x7c) = 0;
    Window_Load(w, 0, BitsF(0x3ecf5c29), BitsF(0x3f34e81b), (const char*)0x5d06a8);
    g_6cfe18 = w;
    Window_HighlightViewTabs(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    Map_SetFogMode(g_map, 0, 6);
    return w;
}
RT3_REPLACE(0x4bcb50, ChangeHeight_Construct);

// 0x4bcbf0. Tree tool window ("ChangeTrees.win").
void __fastcall ChangeTrees_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d06bc;
    Scene_PopEngineBusy();
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4bcbf0, ChangeTrees_Destruct);

RT3_NOINLINE void __fastcall Brush_SetSizeF621ecc(uint8_t* o, int, int32_t i) {
    F<int32_t>(o, 0x78) = i;
    const float r = (float)((double)((const float*)0x621ecc)[i] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, r);
}
RT3_REPLACE(0x4bcc20, Brush_SetSizeF621ecc);

// 0x4bcc60. Sets the map tool mode (+0x84) and its cursor: 0 -> 0x16, 1 -> 7,
// 2 / 3 -> 1; other modes keep the cursor.
RT3_NOINLINE void __fastcall MapTool_SetMode(uint8_t* t, int, int32_t mode) {
    F<int32_t>(t, 0x84) = mode;
    uint8_t* cursor = g_cursorManager;
    switch (mode) {
    case 0: Cursor_Set(cursor, 0, 0x16, 0, 0); break;
    case 1: Cursor_Set(cursor, 0, 7, 0, 0); break;
    case 2:
    case 3: Cursor_Set(cursor, 0, 1, 0, 0); break;
    }
}
RT3_REPLACE(0x4bcc60, MapTool_SetMode);

// 0x4bccd0. Highlights window w's selections: +0x78 / +0x80 / +0x7c among three
// interleaved rows of seven (0x1389.., 0x13ed.., 0x1451..), +0x84 among 0x14b5..
// (4) and +0x8c among 0x139c.. (28).
RT3_NOINLINE void __fastcall Window_HighlightGridTabs(uint8_t* w) {
    for (int32_t i = 0; i < 7; ++i) {
        Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x78) ? 0x87 : 0x65, 0x1389 + i, 1, 0, 0);
        Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x80) ? 0x87 : 0x65, 0x13ed + i, 1, 0, 0);
        Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x7c) ? 0x87 : 0x65, 0x1451 + i, 1, 0, 0);
    }
    for (int32_t i = 0; i < 4; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x84) ? 0x87 : 0x65, 0x14b5 + i, 1, 0, 0);
    for (int32_t i = 0; i < 0x1c; ++i) Window_SendEvent5(w, 0, i != F<int32_t>(w, 0x8c) ? 0x87 : 0x65, 0x139c + i, 1, 0, 0);
}
RT3_REPLACE(0x4bccd0, Window_HighlightGridTabs);

// 0x4bcdd0. Tree tool window constructor ("ChangeTrees.win"): restores the tool
// mode (0x6cfe1c / 0x6cfe20), brush size 0x621ec0 (radius from the float table
// 0x621ecc + 0.5), density / kind (0x621ec8 / 0x621ec4), loads the window, pauses
// the scene and gives the 28 tree buttons (0x139c..) their "name\ndescription"
// tooltips from the string ids in 0x5ef3e8 (0xb0-byte records).
uint8_t* __fastcall ChangeTrees_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d06bc;
    F<uint32_t>(w, 0x8c) = g_6cfe20;
    MapTool_SetMode(w, 0, g_6cfe1c);
    int32_t size = g_621ec0;
    F<int32_t>(w, 0x78) = size;
    float radius = (float)((double)((const float*)0x621ecc)[size] + D(0x5c8628));
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, radius);
    F<uint32_t>(w, 0x7c) = g_621ec8;
    F<uint32_t>(w, 0x80) = g_621ec4;
    F<int32_t>(w, 0x88) = 0;
    Window_Load(w, 0, BitsF(0x3ecf5c29), BitsF(0x3f34e81b), (const char*)0x5d06c8);
    Window_HighlightGridTabs(w);
    Scene_PushBusy((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    char buf[0x12c];
    int32_t i = 0;
    for (const uint8_t* rec = (const uint8_t*)0x5ef3ec; rec < (const uint8_t*)0x5f072c; rec += 0xb0, ++i) {
        crt::sprintf(buf, GetString(F<int32_t>((void*)rec, -4 + 0)));
        memcpy(buf + strlen(buf), (const void*)0x5c9ce8, 2);
        strcat(buf, GetString(F<int32_t>((void*)rec, 0)));
        Window_SendEvent5(w, 0, 0x6e, 0x139c + i, (uint32_t)(uintptr_t)buf, 0, 0);
    }
    return w;
}
RT3_REPLACE(0x4bcdd0, ChangeTrees_Construct);

// 0x4bcf30 (event handler). The terrain brush of the map editor d: sizes
// 0x1389..0x138f (radius 0x621ecc by +0x78, 0x621ec0), strengths
// 0x13ed..0x13f3 (+0x80, 0x621ec4), shapes 0x139c.. (+0x8c, 0x6cfe20),
// variants 0x1451..0x1457 (+0x7c, 0x621ec8), modes 0x14b5..0x14b8 (+0x84,
// 0x6cfe1c). Pressing on the map (without Ctrl) starts (+0x88, the last
// spot +0x90 / +0x94 far away); each move of more than sqrt(0.1) applies
// the mode there: paint (0) / erase (1, doubled) terrain points with the
// strength scaled by the distance moved over the radius (at most 1, times
// 0x5c8660 and the strength 0x621ee8), or raise (2) / lower (3) the level
// by 0x621f04.
int32_t __fastcall HeightBrush_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    const int32_t id = (int32_t)msg[1];
    switch (msg[0]) {
    case 0xcb:
        if (id == 0x7d6 && F<int32_t>(d, 0x88)) {
            F<int32_t>(d, 0x88) = 0;
            return 0;
        }
        return 1;
    case 0xca:
        if (id >= 0x139c && id < 0x13b8) {
            hook::As<int32_t>(g_6cfe20) = id - 0x139c;
            F<int32_t>(d, 0x8c) = id - 0x139c;
            Window_HighlightGridTabs(d);
            return 0;
        }
        if (id >= 0x1451 && id <= 0x1457) {
            F<int32_t>(d, 0x7c) = id - 0x1451;
            hook::As<int32_t>(g_621ec8) = id - 0x1451;
            Window_HighlightGridTabs(d);
            return 0;
        }
        if (id >= 0x14b5 && id <= 0x14b8) {
            MapTool_SetMode(d, 0, id - 0x14b5);
            g_6cfe1c = id - 0x14b5;
            Window_HighlightGridTabs(d);
            return 0;
        }
        if (id >= 0x13ed && id <= 0x13f3) {
            F<int32_t>(d, 0x80) = id - 0x13ed;
            hook::As<int32_t>(g_621ec4) = id - 0x13ed;
            Window_HighlightGridTabs(d);
            return 0;
        }
        if (id == 0x7d6) {
            float fx, fy;
            if (!Scene_FocusFlagIs1(&fx, &fy) || (msg[8] & 0x800)) return 0;
            F<int32_t>(d, 0x88) = 1;
            F<uint32_t>(d, 0x90) = 0x461c3c00;
            F<uint32_t>(d, 0x94) = 0x461c3c00;
            break;
        }
        if (id > 0x1388 && id <= 0x138f) {
            Brush_SetSizeF621ecc(d, 0, id - 0x1389);
            g_621ec0 = id - 0x1389;
            Window_HighlightGridTabs(d);
            return 0;
        }
        return 1;
    case 2:
        break;
    default:
        return 1;
    }
    if (!F<int32_t>(d, 0x88)) return 0;
    const int32_t mode = F<int32_t>(d, 0x84);
    if (mode < 0 || mode > 3) return 0;
    const int32_t erase = mode == 1;
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    const float dx = x - F<float>(d, 0x90);
    const float dy = y - F<float>(d, 0x94);
    const float d2 = dy * dy + dx * dx;
    if (!(d2 > D(0x5c86b8))) return 0;
    const float dist = (float)crt::sqrt(d2);
    const float r = ((const float*)0x621ecc)[F<int32_t>(d, 0x78)];
    const double rr = (double)r < D(0x5c85b8) ? D(0x5c85b8) : (double)r;
    float t = (float)(dist / rr);
    if (BitsF(0x3f800000) < t) t = 1.0f;
    t = (float)(t * D(0x5c8660));
    t = t * ((const float*)0x621ee8)[F<int32_t>(d, 0x80)];
    if (mode == 2 || mode == 3) {
        const float sign = (float)(mode == 2 ? 1 : -1);
        Terrain_AdjustLevel(g_map, 0, x, y, r, sign * ((const float*)0x621f04)[F<int32_t>(d, 0x80)]);
    } else {
        if (erase) t = t + t;
        const int32_t force = (F<uint8_t>(g_input, 0xa8c) & 3) != 0;
        Terrain_PaintPoints(g_map, 0, x, y, r, erase, F<int32_t>(d, 0x8c), F<int32_t>(d, 0x7c), t, force);
    }
    F<float>(d, 0x90) = x;
    F<float>(d, 0x94) = y;
    return 0;
}
RT3_REPLACE(0x4bcf30, HeightBrush_HandleEvent);

// 0x4bd360. Overlay colour for t in 0..1: the heat ramp in special pixel formats,
// else red fading to green through yellow (opaque, blue 0x33).
RT3_NOINLINE uint32_t __stdcall OverlayColor(float t) {
    if (F<int32_t>(g_engine, 0x11474e)) return HeatColor(t) | 0xff000000;
    if (t < Fl(0x5c8598)) t = 0.0f;
    else if (t > Fl(0x5c85b4)) t = 1.0f;
    float a = (float)(t < D(0x5c8628) ? 1.0 : D(0x5c8770) - (((double)t - D(0x5c8628)) + ((double)t - D(0x5c8628))));
    float b = (float)(t > D(0x5c8628) ? 1.0 : (double)t + t);
    uint32_t r = Ftol((double)a * Fl(0x5ca0ec));
    uint32_t g = Ftol((double)b * Fl(0x5ca0ec));
    return (((r << 8) | g) << 8) | 0xff000033;
}
RT3_REPLACE(0x4bd360, OverlayColor);

// 0x4bd460. Deletes the generated rows next to widget w in its window: ids
// 0x2b0c..0x2b34 for widget 0x2aff, else 0x65f4..0x66bb (the argument is unused).
RT3_NOINLINE void __fastcall Widget_DeleteGeneratedRows(uint8_t* w, int, int32_t) {
    if (F<uint16_t>(w, 4) == 0x2aff) Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0x2b0c, 0x2b34, 0);
    else Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0x65f4, 0x66bb, 0);
}
RT3_REPLACE(0x4bd460, Widget_DeleteGeneratedRows);

 // namespace

// 0x4bd4a0 (fastcall train t, highlight; x, y, w, h). Draws train t's row in
// a train list: the consist strip (car skins, shrunk to fit, at least 0.7),
// the trip progress bar (route cost left from the first car's node to the
// next stop over the cost between the last two stops, refreshed every 256th
// draw via +0x1d3 into +0x2d / +0x31 / +0x35) coloured by profit (+0xcc
// over the payment for the engine's cargo), the cargo stars (rate / 10000,
// 50 per row; none in the sandbox or with 0x6cec78+0x4aef), the speed gauge
// (+0xcc / 60), the breakdown icons (+0xe0 / +0xe4 / +0xdc zero), the state
// icon, and the label "rank) destination" plus the state text (0x383..0x389,
// 0xba7..0xbac), the name first for named trains (+0x1a1 or +0x1ce bit 0).
RT3_NOINLINE void __fastcall TrainList_DrawEntry(uint8_t* t, uint8_t highlight, float x, float y, float w, float h) {
    if (!t) return;
    float scale = 1.0f;
    float x0 = (float)((double)x + RowD(0x5d0280));
    float cx = x0, start = x0;
    for (int32_t pass = 0; pass < 2; ++pass) {
        uint8_t* car = (uint8_t*)Obj_City41(t);
        cx = x0, start = x0;
        for (; car; car = (uint8_t*)Obj_City352(car)) {
            uint8_t* type = (uint8_t*)Obj_Ref388(car);
            if (F<int32_t>(type, 0x41) == 3) continue;
            uint8_t* skin = (uint8_t*)(uintptr_t)Car_SkinTexture(car);
            float cs = (float)(One() * scale);
            Obj_Ref388(car);
            if (pass == 1) {
                double base = (double)y + h - RowD(0x5d0250);
                float cy = (float)(base - Slider_StepB(skin) * cs);
                Glyph_Queue(skin, 0, cx, cy, 0x3f800000, cs, cs, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
            }
            cx = (float)(Slider_StepA(skin) * cs + cx);
        }
        float len = cx - start;
        float avail = (float)((double)w - ((double)start - x) - RowD(0x5cecd0));
        if (len > avail) {
            avail = (float)((double)avail / len);
            scale = avail < RowD(0x5c8680) ? (float)RowD(0x5c8680) : avail;
        }
    }
    uint8_t* eng = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(Obj_City41(t)), 0x126));
    Train_ConsistWeight(t, 0, (int32_t)(uintptr_t)eng, 1, 1, 1, nullptr, nullptr, nullptr);
    int32_t year = F<int32_t>(t, 0xd4);
    uint32_t company = F<uint32_t>(t, 0x51);
    float pay = Cargo_DeliveryPayment(eng, year, 0.0f, company, Obj_CityGridByte(t));
    float profit = (float)((double)F<float>(t, 0xcc) / pay);
    if (profit > RowD(0x5c8770)) profit = 1.0f;
    float frac = 0.0f;
    int32_t state = F<int32_t>(t, 0xe8);
    if (!(F<int32_t>(t, 4) < 2 || state == 1 || state == 2 || state == 8 || state == 9 || state == 0xa || state == 0x13 || state == 0xf)) {
        const uint8_t* cur = (const uint8_t*)Array33_Current(t);
        int32_t n = F<int32_t>(t, 8) ? F<int32_t>(t, 8) : F<int32_t>(t, 4);
        const uint8_t* prev = (const uint8_t*)Array33_At(t, 0, n - 1);
        int32_t to = (int8_t)cur[0x28] < 0 ? F<int32_t>((void*)cur, 0x2b)
                                           : F<int32_t>(Collection_Get(g_stations, 0, F<uint16_t>((void*)cur, 0x29)), 8);
        int32_t from2 = (int8_t)prev[0x28] < 0 ? F<int32_t>((void*)prev, 0x2b)
                                               : F<int32_t>(Collection_Get(g_stations, 0, F<uint16_t>((void*)prev, 0x29)), 8);
        uint32_t left = 0, total = 0;
        int32_t node = F<int32_t>(Obj_City41(t), 0x356);
        uint32_t span, rest;
        if (F<int32_t>(t, 0x35) && node == F<int32_t>(t, 0x35)) {
            span = F<uint32_t>(t, 0x2d), rest = F<uint32_t>(t, 0x31);
        } else {
            if (Collection_Count(g_routes) >= 0x32 && Collection_Count(g_routes) >= 0x64)
                Collection_Count(g_routes);
            uint8_t tick = ++t[0x1d3];
            if ((float)tick < RowF(0x5c8598)) {
                span = F<uint32_t>(t, 0x2d), rest = F<uint32_t>(t, 0x31);
            } else {
                t[0x1d3] = 0;
                uint8_t* co = (uint8_t*)Train_Company(t);
                int32_t flag = F<int32_t>(Obj_CityRef(t), 0x10) == 2;
                Route_NextStep(g_routes, 0, node, to, &left, flag, (uint32_t*)co, nullptr, 0);
                co = (uint8_t*)Train_Company(t);
                flag = F<int32_t>(Obj_CityRef(t), 0x10) == 2;
                Route_NextStep(g_routes, 0, from2, to, &total, flag, (uint32_t*)co, nullptr, 0);
                rest = left < 3 && !F<int32_t>(t, 0x31) ? 0 : left;
                span = total;
                F<uint32_t>(t, 0x2d) = total;
                F<uint32_t>(t, 0x31) = rest;
            }
            F<int32_t>(t, 0x35) = node;
        }
        if (span) {
            if (span < 1) span = 1;
            float fr = (float)(double)rest, fs = (float)(double)span;
            frac = (float)(RowD(0x5c8770) - (double)fr / fs);
        }
    }
    float bar = (float)((double)frac * w);
    if (bar > w) bar = w;
    if (bar > RowD(0x5c85b8)) {
        uint32_t color = OverlayColor(profit);
        float off = RowBits(y >= RowD(0x5c85a0) ? 0x3a83126fu : 0xba83126fu);
        if (!highlight) {
            float ry = (float)((double)y + h - RowF(0x5d0718) + off);
            Ui_FillRect(color, 1, (float)((double)x + w - bar), ry, bar, RowBits(0x3b088889), 3);
        } else {
            float bw = (float)((double)bar + RowD(0x5c9b50));
            float ry = (float)((double)y + h - RowF(0x5d0718) + off);
            float rx = (float)((double)x + w - bar - RowD(0x5c9b50));
            Ui_FillRect(color, 1, rx, ry, bw, RowBits(0x3b088889), 3);
        }
    }
    float right = (float)((double)x + w);
    float minX = right;
    float stars = (float)((double)Train_CargoRate(t, 0, 0) / RowD(0x5c8d18));
    if (g_newGame[0x82] || F<int32_t>(g_world, 0x4aef)) F<uint32_t>(&stars, 0) = 0;
    if (stars > RowD(0x5c85b8)) {
        if (stars > RowF(0x5c8598)) {
            if (!(stars < RowF(0x5c8c28))) F<uint32_t>(&stars, 0) = 0x42c80000;
        } else {
            F<uint32_t>(&stars, 0) = 0;
        }
        int32_t rows = 1 - (int32_t)(((double)stars - RowD(0x5c8678)) / RowD(0x5d0710));
        for (int32_t row = 0, base = 0; row < rows; ++row, base += 0x32) {
            float rem = stars - (float)base;
            int32_t n = (int32_t)((double)rem - RowD(0x5c8678)) + 1;
            if (n >= 0x32) n = 0x32;
            for (int32_t i = 0; i < n; ++i) {
                float fill = rem - (float)i;
                if (fill < RowD(0x5c85b8)) continue;
                if (fill > RowF(0x5c85b4)) fill = 1.0f;
                double di = (double)i;
                float gx = (float)((double)right - RowD(0x5d0708) - di * RowD(0x5c9bc0));
                double dr = (double)row;
                float gy = (float)(dr * RowD(0x5d01d8) + y + RowD(0x5caae0));
                float clip = (float)((RowD(0x5c8770) - fill) * RowD(0x5d0700) + gx);
                Glyph_Queue(RowGlyph(0x10), 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 1, clip, 0.0f, 1.0f, 1.0f);
                if (!(minX < gx)) minX = gx;
            }
        }
    }
    float y1 = (float)((double)y + RowD(0x5c9b50));
    float gx2 = right;
    {
        double r = gx2;
        gx2 = (float)(r - (Slider_StepA(RowGlyph(0xe)) + RowD(0x5caad0)));
        RowIcon(RowGlyph(0xe), gx2, y1);
    }
    float gauge = (float)((double)F<float>(t, 0xcc) / RowF(0x5c8990));
    if (gauge > RowD(0x5c8770)) gauge = 1.0f;
    float angle = (float)(((double)gauge - RowD(0x5c8628)) * RowD(0x5cb4a0));
    {
        float gy = (float)((double)y1 + RowD(0x5caae0));
        float gx = (float)((double)gx2 + RowD(0x5ca2f0));
        Glyph_QueueRotated(RowGlyph(0xf), 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, angle, 0.5f, RowBits(0x3f638e39));
    }
    float y2 = (float)((double)y1 - RowD(0x5d06f8));
    if (F<float>(t, 0xe0) == RowF(0x5c8598)) RowStatus(gx2, y2, 0);
    if (F<float>(t, 0xe4) == RowF(0x5c8598)) RowStatus(gx2, y2, 1);
    if (F<float>(t, 0xdc) == RowF(0x5c8598)) RowStatus(gx2, y2, 2);
    if (Train_StateBOr12(t)) RowStatus(gx2, y2, 3);
    else if (((int32_t(__fastcall*)(void*))&Obj_KindE8Special)(t)) RowStatus(gx2, y2, 4);
    else if (F<int32_t>(t, 0xe8) == 0x13) RowStatus(gx2, y2, 5);
    else if (F<int32_t>(t, 0xe8) == 0xe) RowStatus(gx2, y2, 6);
    char dest[0x134], label[0x1f8], tmp[0x190], stop[0x130];
    Train_DestinationText(t, 0, dest, 0x12b);
    if (!t[0x1a1] && !(t[0x1ce] & 1)) {
        if (((int32_t(__fastcall*)(void*))&Obj_KindE8Special)(t) && (t[0x1ce] & 2)) {
            FormatText(label, (const char*)0x5d06f0, dest, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            char num[0x20];
            const char* r = Num_Format(num, 1, (double)(Train_Rank(t) + 1), 0xa, 1, 0, 0);
            FormatText(label, (const char*)0x5d06e8, r, dest, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
    } else {
        FormatText(label, (const char*)0x5d06e0, dest, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    switch (F<int32_t>(t, 0xe8)) {
    case 1:
        if (!Train_IsStopped(t)) {
            strcat(label, GetString(0x384));
        } else if (t[0x1bf] && F<int16_t>(t, 0x1c0)) {
            uint8_t* cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, (uint32_t)(int32_t)F<int16_t>(t, 0x1c0));
            const char* nm = LocalizeName((const char*)cargo + 4);
            const char* fmt = GetString(0xba7);
            FormatText(tmp, fmt, nm, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            strcat(label, tmp);
        } else if (t[0x1c2] && F<int16_t>(t, 0x1c3)) {
            strcpy(stop, GetString(0xba8));
            if (uint8_t* st = TrainOrders_NextStop(t, 0, 0, 1)) strcpy(stop, (const char*)st + 0x46b);
            int32_t c = F<int16_t>(t, 0x1c3);
            uint8_t* col = hook::As<uint8_t*>(g_cargoTypes);
            int32_t id = c == F<uint16_t>(col, 0x8c) ? 0xba9 : c == F<uint16_t>(col, 0x8a) ? 0xbaa : 0xbab;
            const char* fmt = GetString(id);
            FormatText(tmp, fmt, stop, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            strcat(label, tmp);
        } else {
            strcat(label, GetString(0x383));
        }
        break;
    case 2: strcat(label, GetString(0x385)); break;
    case 3: case 4: case 5: case 6: strcat(label, GetString(0x386)); break;
    case 7: strcat(label, GetString(0x387)); break;
    case 9: strcat(label, GetString(0x388)); break;
    case 10:
        if (OrderList_CountPlain(t) > 0) strcat(label, GetString(0x389));
        break;
    case 0x13:
        if (OrderList_CountPlain(t) > 0) strcat(label, GetString(0xbac));
        break;
    }
    float tx = (float)((double)x + RowD(0x5d06d8));
    if (t[0x1a1]) tx = (float)((double)tx + RowF(0x5cb5c4));
    float nameW = 0.0f;
    if ((t[0x1ce] & 1) || t[0x1a1]) {
        const char* nm = Train_DisplayName(t, 0, 0);
        Font_TextHeight(F<uint8_t*>(g_world, 0x46a98), 0, nm, 1.0f, &nameW);
        tx = nameW + tx;
    }
    float hh = (float)((double)h * RowD(0x5c8628));
    float ty = (float)((double)y - RowD(0x5d0250));
    float tw = (float)((double)minX - tx - RowD(0x5caad0));
    Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, label, tx, ty, tw, hh, 0xffffffff, 2, 0, 0, 1.0f);
    if ((t[0x1ce] & 1) || t[0x1a1]) {
        float nw = (float)((double)nameW + RowD(0x5c85b8));
        float nx = tx - nameW;
        const char* nm = Train_DisplayName(t, 0, 0);
        Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, nm, nx, ty, nw, hh, 0xffffffff, 2, 0, 0, 1.0f);
    }
}
RT3_REPLACE(0x4bd4a0, TrainList_DrawEntry);

}  // namespace rt3
