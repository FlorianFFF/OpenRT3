// World.cpp - RT3_mp.exe 0x432000-0x438000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

// Global tables (Collection*) referenced by id from objects.
static Collection*& g_companies  = hook::Global<Collection*>(0x62be10);

static Collection*& g_table6ceb9c = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_players    = hook::Global<Collection*>(0x6ceb9c);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int __fastcall ToUpper(char c);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Collection_ReleaseAll(Collection* c);
const char* __fastcall GetString(int32_t id);
void* __fastcall Record42d5a0_Init(uint8_t* r);
void __fastcall Record42d5a0_SetName(uint8_t* r, int, int32_t v, const char* name);
void __fastcall Obj_FreeAllLists2(void* o);
int32_t __fastcall Table_CountClear82b(Collection* t);
void* __fastcall Collection_RecordAtList(Collection* t, int, uint32_t pos, int list);
int32_t __fastcall Obj_Has78(void* o);
int32_t ClockMs();
void __fastcall Table_Hook47d8d0(Collection* t);
void __fastcall Table_TouchRegionSites(Collection* t);
void __fastcall Table_Hook4a7350(Collection* t);
void __fastcall Track_Validate(Collection* t);
int32_t Global6d07b4Set();
int32_t __fastcall Options_Check1bf(void* self);
const char* __fastcall Options_SaveFolder(void* opt);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

int32_t __fastcall Game_EffectiveVersion();
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
int32_t __fastcall Save_IsCompatible(uint8_t* g, int, const char* name);
void __fastcall StripExtension(char* s);
const char* __fastcall LocalizeName(const char* name);
void __fastcall SortRecordsByName(uint8_t* records, int32_t count, uint32_t size, int32_t keyOffset);
void __fastcall Industries_RebuildPortsWarehouses(Collection* c);
void __fastcall SaveList_Scan(uint8_t* list, int, int32_t kind);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
void __fastcall Record_EncodeTags(uint8_t* r);
void __fastcall EventRules_Save(uint8_t* o, int, Stream st);
void __fastcall EventRules_Load(uint8_t* o, int, Stream st);
void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall Track_UpdateColor(uint8_t* o);
void __fastcall Obj_RefreshHighlight(void* o);
void __fastcall String_Assign(char** s, const char* src);
int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value);
uint32_t __fastcall Obj_FindRegionSite(void* o, int, int32_t requireActive, int32_t requireFlag);
int32_t __fastcall Obj_BuildingValue(void* o);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void __fastcall Collection_Destruct(Collection* c);
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
void __fastcall Templates_Normalize(uint8_t* w);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
void World_RefreshColors();
void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
int32_t CountHumanPlayers();
void __fastcall World_SetDifficulty(uint8_t* w, int, int32_t level);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src);
uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
void __fastcall Economy_UpdateEraFactors(uint8_t* o);
void __fastcall Templates_Normalize(uint8_t* self);
void __fastcall CargoTypes_Recount(Collection* t);
void __fastcall Industries_BuildCargoLists(Collection* c);
void World_UpdateEraFactor();
void __fastcall World_UpdateSpeedScale(uint8_t* o);
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall CompanyList_Fill(uint8_t* w);
void __fastcall CompanyList_Reset();
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall StationPicker_Fill(uint8_t* w);
void __fastcall TrainPicker_Fill(uint8_t* w, int, uint32_t exclude);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void* __fastcall Obj_Ref25(void* o);
int32_t IsMultiplayer();
int32_t __fastcall Obj_Get25(void* o);
void __fastcall FreeAndNull(void** p);
void __fastcall Obj_Incc60(uint8_t* o);
void* __fastcall Table_At(Collection* t, int, int32_t i);
void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai, int32_t clearHistory);
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall Table_RefreshNamedFlags(Collection* t);
void World_ProcessMarginCalls();
void __fastcall Player_MarginCheck(uint8_t* p);
void* __fastcall Company_Owner(void* c);
void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode);
double __fastcall Obj_Float18(void* o);
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void* __fastcall Obj_City41(void* o);
void* __fastcall Array33_At(void* a, int, int32_t i);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void* __fastcall Identity(void* o);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void __fastcall Events_AddTimed(uint8_t* self, int, uint8_t* cond, int32_t* a, int32_t* b, int32_t* c);
void __stdcall View_RefreshCompanyLists();
void __fastcall Session_Shutdown(uint8_t* s);
void __fastcall Obj_CountBuildings(uint8_t* o);
void MarkKind2Buildings();
int32_t __fastcall Train_FirstCarType126(uint8_t* t);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __fastcall OrderList_Init(uint8_t* o);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall Vehicles_Destruct(Collection* t);
void __fastcall Collection5d00c8_Destruct(Collection* c);
void __fastcall Companies_Destruct(Collection* c);
void __fastcall Players_Destruct(Collection* c);
void __fastcall Map_RuntimeDestructor(uint8_t* map);
void __fastcall Collection5c997c_Destruct(Collection* c);
void __fastcall Collection5ce068_Destruct(Collection* c);
void __fastcall Collection5c9824_Destruct(Collection* c);
void __fastcall Sites_Destruct(Collection* c);
void __fastcall Collection5c93cc_Destruct(Collection* c);
void __fastcall Collection5ce418_Destruct(Collection* c);
void __fastcall Collection5cae10_Destruct(Collection* c);
void MapPointers_Delete();
void __fastcall World_SetEndMode(uint8_t* w, int, int32_t mode);
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
void __fastcall Train_Crash(uint8_t* t, int, int32_t forced);
void __fastcall Obj_AssignRandomSlots(uint8_t* o);
int32_t __stdcall Net_SendSlot(int32_t slot, int32_t pick);
int32_t Net_AllPlayersFlag4();
void* __fastcall Cursor33_Init(uint8_t* c);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
void __fastcall Obj_ShowMessageDialog(uint32_t id);
void __fastcall Train_PromptRename(uint32_t id, uint32_t b);
void __fastcall BuildingInfo_Show(uint32_t a, uint32_t building);
void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
uint8_t __fastcall Load_CargoFlags(void* o);
int32_t __stdcall RuleVar_Get(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8);
uint32_t __fastcall Event_ShowText(uint8_t* e, int, uint8_t* pl, uint8_t* co, uint8_t* st, char* out, int32_t status);
int32_t __fastcall Effects_RunTrigger(uint8_t* fx, int, uint8_t* city, int32_t* company, int32_t* player, int32_t idx);
int32_t __fastcall World_IsSpecialScenario(void*, int, int32_t v);
void __stdcall View_RefreshCompanyAssets(int32_t, int32_t company);
void __fastcall World_SetupPlayers(uint8_t* w);
void __fastcall World_RunAhead(uint8_t* w);
RT3_NOINLINE int __fastcall ToUpper(char c);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void* __fastcall Company_Owner(void* c);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __fastcall Company_OwnerFlag1e1(void* c);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall Map_RuntimeDestructor(uint8_t* map);
RT3_NOINLINE void __fastcall World_ProcessEvents(uint8_t* w);
void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE void MarkKind2Buildings();
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall Obj_AssignRandomSlots(uint8_t* o);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
void __fastcall World_CheatPrompt(uint8_t* w);
void __fastcall Obj_FreeAllLists(void* o);
uint32_t __stdcall Scenario_CheckEditable();
uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
uint32_t __stdcall StockMarket_IsAllowed();
uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Nop436050(void*, int, int32_t);
void __fastcall Nop436060(void*, int, int32_t);
void __fastcall Train_OpenConsistEditor(void* self, int, uint32_t trainId, const char* message);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
int32_t Net_LockQueue();
void Net_UnlockQueue();
RT3_NOINLINE void __stdcall World_SetEditorButtons(uint32_t mode, uint32_t disabled);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Vehicles_Destruct(Collection* t);
RT3_NOINLINE float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
RT3_NOINLINE uint32_t __fastcall Event_ShowText(uint8_t* e, int, uint8_t* pl, uint8_t* co, uint8_t* st, char* out, int32_t status);
RT3_NOINLINE void __fastcall Effects_Dispatch(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company, uint8_t* player, int32_t slot);
RT3_NOINLINE int32_t __fastcall Effects_RunTrigger(uint8_t* fx, int, uint8_t* city, int32_t* company, int32_t* player, int32_t idx);
RT3_NOINLINE int32_t __fastcall Event_Check(uint8_t* ev, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE Collection* __fastcall Collection5c9cec_Construct(Collection* c);
void __fastcall Collection_ReleaseAll(Collection* c);
void* __fastcall Collection_RecordAtList(Collection* t, int, uint32_t pos, int list);
void __fastcall Collection5c9cec_Destruct(Collection* c);
uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __fastcall RuleSets_Save(Collection* c, int, Stream st, int32_t recordSize);
RT3_NOINLINE int32_t __fastcall RuleSets_Load(Collection* c, int, Stream st, int32_t recordSize);
int32_t __fastcall Table_CountClear82b(Collection* t);
RT3_NOINLINE void __fastcall Obj_FreeBuffer8(void* o);
int32_t __fastcall Save_IsCompatible(uint8_t* g, int, const char* name);
void __fastcall SaveList_Scan(uint8_t* list, int, int32_t kind);
RT3_NOINLINE uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind);
RT3_NOINLINE void* __fastcall World_Construct(uint8_t* w);
void __fastcall Nop433730(void*);
double __fastcall Obj_Rounded29(void* o);
RT3_NOINLINE void __fastcall Obj_Set21(void* o, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Obj_Get21(void* o);
RT3_NOINLINE int32_t __fastcall Obj_Get25(void* o);
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
RT3_NOINLINE void World_ProcessMarginCalls();
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __stdcall View_OnModeInput(const uint32_t* event);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE void __fastcall World_SetDifficulty(uint8_t* w, int, int32_t level);
RT3_NOINLINE uint32_t __stdcall Scenario_CheckEditable();
RT3_NOINLINE uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
RT3_NOINLINE void RunDebugValidation();
RT3_NOINLINE void __fastcall Economy_UpdateEraFactors(uint8_t* o);
RT3_NOINLINE void __stdcall World_SetEditorButtons(uint32_t modeArg, uint32_t disabledArg);
RT3_NOINLINE void __fastcall Nop433e90(void*);
RT3_NOINLINE void __fastcall World_RollStats(uint8_t* w);
void __fastcall Window_AddTrainDetailStrip(uint8_t* win, int, float x, float y, int32_t, int32_t);
void __fastcall Obj_SetSlot8a(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
RT3_NOINLINE int32_t IsMultiplayer();
int32_t __fastcall World_IsHostOrLocal(void* w);
RT3_NOINLINE float __fastcall World_Value4cbe(void* w);
uint32_t World46aa0();
RT3_NOINLINE void __fastcall Obj_SetName8(uint8_t* o, int, const char* s);
int32_t __fastcall Obj_Has78(void* o);
void World_UpdateEraFactor();
RT3_NOINLINE int32_t __stdcall Net_SendSlot(int32_t slot, int32_t pick);
RT3_NOINLINE void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
RT3_NOINLINE int32_t __fastcall World_StepSpeed(uint8_t* w, int, int32_t delta);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE void __fastcall Obj_SetPlayer(void* o, int, uint32_t player);
RT3_NOINLINE void* __fastcall Obj_Ref25(void* o);
RT3_NOINLINE int32_t UiBusy();
RT3_NOINLINE int32_t CountHumanPlayers();
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall World_EconomyTick(uint8_t* w, int, int32_t force);
RT3_NOINLINE void MarkKind2Buildings();
RT3_NOINLINE uint32_t __stdcall StockMarket_IsAllowed();
RT3_NOINLINE uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
RT3_NOINLINE int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
RT3_NOINLINE void __fastcall World_SetNamedValue(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE int32_t __fastcall World_LookupNamed2(void* w, int, const char* name);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
int32_t RendererMode15Above128();
RT3_NOINLINE int32_t RendererMode15Zero();
RT3_NOINLINE void __fastcall Weather_ApplyIntensity(uint8_t* w);
RT3_NOINLINE int32_t Global6d0818Is25();
RT3_NOINLINE void __fastcall Templates_Normalize(uint8_t* self);
RT3_NOINLINE void Cities_PayConnectionBonuses();
int32_t __fastcall Train_FollowSelectedCb(void*, const uint32_t* ev);
void __fastcall World_YearEvents(uint8_t* w);
RT3_NOINLINE void __fastcall Nop436040(void*, int, int32_t);
RT3_NOINLINE void __fastcall Nop436050(void*, int, int32_t);
RT3_NOINLINE void __fastcall Nop436060(void*, int, int32_t);
RT3_NOINLINE void __stdcall View_RefreshCompanyLists();
RT3_NOINLINE void __stdcall View_RefreshForCompany(int32_t id);
RT3_NOINLINE void __stdcall View_RefreshCompanyAssets(int32_t, int32_t company);
RT3_NOINLINE void World_RefreshColors();
RT3_NOINLINE void __fastcall Scenario_End(uint8_t* w);
RT3_NOINLINE float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
RT3_NOINLINE int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
RT3_NOINLINE void __fastcall World_SetEndMode(uint8_t* w, int, int32_t mode);
RT3_NOINLINE void __fastcall Train_OpenConsistEditor(void* self, int, uint32_t trainId, const char* message);
RT3_NOINLINE void __fastcall World_MarkBuildingTypes(uint8_t* w);
RT3_NOINLINE void __fastcall World_UpdateSpeedScale(uint8_t* o);
RT3_NOINLINE int32_t __fastcall World_IsSpecialScenario(void*, int, int32_t v);
RT3_NOINLINE void __fastcall World_Reset(uint8_t* w, int, int32_t setup);
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
RT3_NOINLINE void World_MarkKind2Types();
RT3_NOINLINE void __fastcall World_CreatePlayers(uint8_t* w);
RT3_NOINLINE int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall World_RunAhead(uint8_t* w);
RT3_NOINLINE int32_t __fastcall World_OpenEventItem(uint8_t* w, int, uint8_t* e);
RT3_NOINLINE double __stdcall Effects_CargoMultiplier(int32_t idx);
RT3_NOINLINE void __fastcall World_CheatPrompt(uint8_t* w);
RT3_NOINLINE void __fastcall World_ProcessEvents(uint8_t* w);
RT3_NOINLINE void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
RT3_NOINLINE void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void ModelPreview_Close();
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
RT3_NOINLINE void __fastcall Player_MarginCheck(uint8_t* p);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void __fastcall ObjList_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE void __fastcall Obj_ShowMessageDialog(uint32_t id);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Train_PromptRename(uint32_t id, uint32_t b);
RT3_NOINLINE void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
RT3_NOINLINE void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
RT3_NOINLINE void __fastcall BuildingInfo_Show(uint32_t a, uint32_t building);
RT3_NOINLINE void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
RT3_NOINLINE void MapPointers_Delete();
RT3_NOINLINE void __fastcall Scenario_ShowResult(int32_t level);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v);
RT3_NOINLINE void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);

static inline void* RefPositive(Collection* t, int32_t id) { return id > 0 ? Collection_Get(t, 0, id) : nullptr; }

 // cargo-type table object (+0x16e: available flags)

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }

// 0x434ea0 / 0x435030. Lookups by name (+0) in world tables +0x66b2 / +0x66b6:
// first char (name uppercased vs. stored) then exe stricmp; value +0x1e / +0x3d.
static int32_t FindNamed(Collection* t, const char* name, uint32_t valueOffset) {
    int32_t total = Collection_Count(t);
    char first = (char)ToUpper(name[0]);
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)r[0] != first) continue;
        if (ExeStricmp(name, (const char*)r) == 0) return F<int32_t>(r, valueOffset);
    }
    return 0;
}

// 0x434130. Era factor +0x4ca2: 0.33 + 0.67 (1 - (year - 1850) / 150), clamped to [0.33, 1].
static void __fastcall World_UpdateEraFactor_Impl(uint8_t* w) {
    double t = (double)(F<uint16_t>(w, 0xd) - 0x73a) / *(const double*)0x5c9680;
    float f = (float)((*(const double*)0x5c8770 - t) * *(const double*)0x5c9ef8 + *(const double*)0x5c8d80);
    F<float>(w, 0x4ca2) = f;
    double d = (double)f < *(const double*)0x5c8d80 ? 0.33 : (double)f;
    float f2 = (float)d;
    F<float>(w, 0x4ca2) = f2;
    if ((double)f2 > *(const double*)0x5c8770) F<float>(w, 0x4ca2) = 1.0f;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

static inline char* Fmt(char* dst, const char* fmt, const char* a = 0, const char* b = 0) {
    return FormatText(dst, fmt, a, b, 0, 0, 0, 0, 0, 0);
}

static inline void FirstCargoName(char* dst, int32_t pos) {
    Collection* c = hook::Global<Collection*>(0x62ba8c);
    strcpy(dst, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, pos, 0)) + 4);
}

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double Fl_204(uint32_t a) { return *(const float*)a; }

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

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

// Calls virtual slot +0x54 (with 0) on every record of a collection.
static void Table_CallSlot54(Collection* (*get)()) {
    for (int32_t i = 0; i < Collection_Count(get()); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(get(), 0, Collection_IdAt(get(), 0, i, 0));
        ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)r)[0x54 / 4])(r, 0, 0);
    }
}

// Number of buildings (0x62b26c) of the industry type named `name` (0x62b268).
static int32_t Industries_Count(const char* name) {
    return Table_CountField3d0(hook::Global<Collection*>(0x62b26c), 0,
                               (int32_t)Table_FindByName2(hook::Global<Collection*>(0x62b268), 0, name, 0));
}

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline int32_t View_Mode() { return F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c); }

// Shared tail of 0x436070 / 0x4360d0: the companies list (mode 0xb) and the overview (mode 9).
static void View_RefreshCompaniesAndOverview() {
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 0xb) CompanyList_Reset();
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 9) Overview_RedrawRect(hook::Global<uint8_t*>(0x62c120), 0, 0, 0, 0x270f, 0x270f);
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

namespace {
// Debug log line (string id) "event: count" when 0x6cec74+flag is set.
void EventLog(uint8_t* ev, int32_t n, uint32_t flag, int32_t id, int32_t kind) {
    if (!hook::Global<uint8_t*>(0x6cec74)[flag]) return;
    char num[0x20], line[0x200];
    const char* s = Num_Format(num, 1, (double)n, 0xa, 1, 0, 0);
    const char* fmt = GetString(id);
    FormatText(line, fmt, (const char*)ev + 0x18, s, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Log_Add(hook::Global<Collection*>(0x6acd34), 0, line, kind, 0, 0);
}
}

namespace {
// The music and effects volumes from the options (+0x1146fa, +0x1146fe x 0.24).
void Scenario_RestoreVolumes() {
    Sound_SetVolume(hook::Global<uint8_t*>(0x6d402c), 0, F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fa), 2);
    Sound_SetVolume(hook::Global<uint8_t*>(0x6d402c), 0, (float)((double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fe) * D(0x5c9c80)), 0);
}
}

static void Event(void* self, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    hook::Orig<void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x540120)(
        self, 0, type, id, a, b, c);
}

// 0x432130. Event fx: runs trigger idx's effect list (+4 + idx * 4, linked by
// +0x24) for its scope (+0x7fb + idx): 0 once for the given city, company
// and player, 1 once for none of them, 2 every company, 3 the human ones, 4
// the AI ones, 5 every city (or just city +0x80f + idx * 4 when set), 6
// every player, 7 the human ones, 8 the AI ones. A company's chairman stands
// in for a missing player - QUIRK: found once, then kept for the following
// companies. Effects with a delay (+9 / +0xd) are also queued. 1 when an
// effect list ran.
RT3_NOINLINE int32_t __fastcall Effects_RunTrigger(uint8_t* fx, int, uint8_t* city, int32_t* company, int32_t* player, int32_t idx) {
    int32_t fired = 0;
    uint8_t scope = fx[0x7fb + idx];
    int32_t* co = nullptr;
    int32_t* pl = nullptr;
    if (scope) {
        city = nullptr;
    } else {
        pl = player;
        co = company;
    }
    int32_t nCo = Table_CountField3f(hook::Global<Collection*>(0x62be10));
    int32_t nPl = Table_CountField4(hook::Global<Collection*>(0x6ceb9c));
    int32_t nCity = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
    int32_t n = 0;
    switch (fx[0x7fb + idx]) {
    case 0: case 1: n = 1; break;
    case 2: case 3: case 4: n = nCo; break;
    case 5: n = nCity; break;
    case 6: case 7: case 8: n = nPl; break;
    }
    for (int32_t i = n - 1; i >= 0; --i) {
        switch (fx[0x7fb + idx]) {
        case 2:
            co = (int32_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, i);
            if (!co) continue;
            break;
        case 3:
            co = (int32_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, i);
            if (!co || !Company_OwnerFlag1e1(co)) continue;
            break;
        case 4:
            co = (int32_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, i);
            if (!co || Company_OwnerFlag1e1(co)) continue;
            break;
        case 5: {
            Collection* cs = hook::Global<Collection*>(0x6cfc9c);
            city = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)i, 0));
            if (!city) continue;
            int32_t only = F<int32_t>(fx, 0x80f + idx * 4);
            if (only > 0 && F<int32_t>(city, 0) != only) continue;
            break;
        }
        case 6:
            pl = (int32_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i);
            if (!pl) continue;
            break;
        case 7:
            pl = (int32_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i);
            if (!pl || !F<int32_t>(pl, 0x1e1)) continue;
            break;
        case 8:
            pl = (int32_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i);
            if (!pl || F<int32_t>(pl, 0x1e1)) continue;
            break;
        }
        uint8_t* e = F<uint8_t*>(fx, 4 + idx * 4);
        if (!e) continue;
        fired = 1;
        do {
            if (co && !pl) pl = (int32_t*)Company_Owner(co);
            Effects_Dispatch(fx, 0, (const int32_t*)e, city, (uint8_t*)co, (uint8_t*)pl, idx);
            if (F<int32_t>(e, 9) != -1 || F<int32_t>(e, 0xd) != -1) Events_AddTimed(fx, 0, e, (int32_t*)city, co, pl);
            e = F<uint8_t*>(e, 0x24);
        } while (e);
    }
    return fired;
}
RT3_REPLACE(0x432130, Effects_RunTrigger);

 // namespace

// 0x4323a0. Scenario event ev (conditions at +0, trigger +0x7ef, scope +0x7f0
// (territory id, 99 none), +0x7f4 single / multiplayer only, company and
// player filters +0x7f9 / +0x7fa: 2 AI (in multiplayer the local one for
// trigger 9), 3 human, 99 = +0x82c / +0x830) checks trigger type for every
// territory, company (last first) and player its condition scope needs
// (Load_CargoFlags bits 4 / 1 / 2). A condition chain (next +0x1a) compares
// RuleVar_Get of condition +0 (table 0x5f3d98, kind 0x5f3de4) with its value
// (a constant, or the scope's / world's variable +9 per 0x6103fd) by +4 (=,
// !=, >, <, >=, <=, or the best of all scopes for 6 / 7), joined by +0xd
// (and / or / replace). Each match counts (+0x827, debug log 0xd3), runs the
// effects (after the text for +0x7ee == 1) and, once, the text (0x4317c0;
// appended to out, n bytes, after a line break that counts 2) and, for
// trigger 8 single player, the pending news. A one-shot event (+0x7f5)
// retires (+0x81f) on its first match. With the special scenario mode
// (0x6cec78+0x4a73) only trigger 9, flagged (+0x82b) or effect-16 events in
// 9..12 / 16..22 run. Returns whether an effect ran.
RT3_NOINLINE int32_t __fastcall Event_Check(uint8_t* ev, int, uint32_t trigger, char* out, int32_t outSize) {
    int32_t result = 0;
    if (F<int32_t>(ev, 0x81f) || ev[0x7ef] != (uint8_t)trigger || !F<int32_t>(ev, 0)) return 0;
    if (!ev[0x7f4] && IsMultiplayer()) return 0;
    if (ev[0x7f4] == 1 && !IsMultiplayer()) return 0;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a73)) {
        bool special = false;
        if (World_IsSpecialScenario(hook::Global<uint8_t*>(0x6cec78), 0, 0x10)) {
            uint32_t k = F<uint32_t>(ev, 0x14);
            if ((k >= 9 && k <= 0xc) || (k >= 0x10 && k <= 0x16)) special = true;
        }
        if (ev[0x7ef] != 9 && !ev[0x82b] && !special) return 0;
    }
    EventLog(ev, ++F<int32_t>(ev, 0x823), 0x198, 0xd2, 0xc);
    uint8_t scope = Load_CargoFlags(ev);
    int32_t nTerr = -1, nComp = -1, nPlayer = -1;
    uint8_t *terr = nullptr, *co = nullptr, *pl = nullptr;
    int32_t textLen = out ? (int32_t)strlen(out) : 0;
    int32_t prev = 0;
    bool shown = false;
    if (scope & 4) nTerr = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
    if (scope & 1) nComp = Table_CountField3f(hook::Global<Collection*>(0x62be10));
    if (scope & 2) nPlayer = Table_CountField4(hook::Global<Collection*>(0x6ceb9c));
    if (F<int32_t>(ev, 0x7f0) == 0x63) nTerr = -1;
    char text[0x1f8];
    for (int32_t ti = 0;; ++ti) {
        if (ti < nTerr) {
            Collection* ts = hook::Global<Collection*>(0x6cfc9c);
            terr = (uint8_t*)Collection_Get(ts, 0, Collection_IdAt(ts, 0, (uint32_t)ti, 0));
        } else {
            if (nTerr != -1) return result;
            nTerr = -2;
        }
        if (terr && ((F<int32_t>(ev, 0x7f0) && F<int32_t>(ev, 0x7f0) != F<int32_t>(terr, 0)) || !F<int32_t>(terr, 0x3d))) continue;
        if (!ev[0x7f9] || nComp == -2) nComp = -1;
        for (int32_t ci = 0;; ++ci) {
            if (ci < nComp) {
                co = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, nComp - ci - 1);
            } else {
                if (nComp != -1) break;
                nComp = -2;
            }
            if (co) {
                uint8_t f = ev[0x7f9];
                if (f == 2) {
                    if (!Company_OwnerFlag1e1(co)) continue;
                    if (ev[0x7ef] == 9 && IsMultiplayer() && F<int32_t>(co, 0) != Obj_Get21(hook::Global<uint8_t*>(0x6cec78))) continue;
                } else if (f == 3) {
                    if (Company_OwnerFlag1e1(co)) continue;
                } else if (f == 0x63) {
                    if (F<int32_t>(co, 0) != F<int32_t>(ev, 0x82c)) continue;
                }
            }
            if (!ev[0x7fa] || nPlayer == -2) nPlayer = -1;
            for (int32_t pi = 0;; ++pi) {
                if (pi < nPlayer) {
                    pl = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, pi);
                } else {
                    if (nPlayer != -1) break;
                    nPlayer = -2;
                }
                if (pl) {
                    uint8_t f = ev[0x7fa];
                    if (f == 2) {
                        if (!F<int32_t>(pl, 0x1e1)) continue;
                        if (ev[0x7ef] == 9 && IsMultiplayer() && F<int32_t>(pl, 0) != Obj_Get25(hook::Global<uint8_t*>(0x6cec78))) continue;
                    } else if (f == 3) {
                        if (F<int32_t>(pl, 0x1e1)) continue;
                    } else if (f == 0x63) {
                        if (F<int32_t>(pl, 0) != F<int32_t>(ev, 0x830)) continue;
                    }
                }
                int32_t acc = 1;
                bool first = true;
                for (uint8_t* c = F<uint8_t*>(ev, 0); c; c = F<uint8_t*>(c, 0x1a), first = false) {
                    int32_t type = F<int32_t>(c, 0);
                    int32_t ok = type != -2;
                    if (type >= 0) {
                        int32_t idx = F<int32_t>(c, 9), value;
                        uint8_t vf = ((const uint8_t*)0x6103fd)[idx * 0x6e];
                        if (!F<int32_t>(c, 5)) value = idx;
                        else if (co && (vf & 1)) value = F<int32_t>(co, idx * 4 + 0x35b);
                        else if (pl && (vf & 2)) value = F<int32_t>(pl, idx * 4 + 0x2ab);
                        else if (terr && (vf & 4)) value = F<int32_t>(terr, idx * 4 + 0x3b5);
                        else value = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), idx * 4 + 0x8a);
                        int32_t var = F<int32_t>((void*)0x5f3d98, type * 0x81);
                        int32_t r = RuleVar_Get(var, terr, co, pl, value, idx, F<int32_t>(c, 0xe), F<int32_t>(c, 0x12));
                        int32_t kind = F<int32_t>((void*)0x5f3de4, type * 0x81);
                        if (kind == 0 || kind == 2 || kind == 3) {
                            ok = r == value;
                        } else {
                            switch (c[4]) {
                            case 0: if (r != value) ok = 0; break;
                            case 1: if (r == value) ok = 0; break;
                            case 2: if (r <= value) ok = 0; break;
                            case 3: if (r >= value) ok = 0; break;
                            case 4: if (r < value) ok = 0; break;
                            case 5: if (r > value) ok = 0; break;
                            case 6:
                            case 7: {
                                int32_t mode, n;
                                if (co) mode = 1, n = Table_CountField3f(hook::Global<Collection*>(0x62be10));
                                else if (terr) mode = 3, n = Collection_Count(hook::Global<Collection*>(0x6cfc9c));
                                else if (pl) mode = 2, n = Table_CountField4(hook::Global<Collection*>(0x6ceb9c));
                                else {
                                    ok = 0;
                                    break;
                                }
                                if (!n) {
                                    ok = 0;
                                    break;
                                }
                                if (n == 1) break;
                                uint8_t *t2 = terr, *p2 = pl, *c2 = co;
                                int32_t base = RuleVar_Get(var, terr, co, pl, value, idx, F<int32_t>(c, 0xe), F<int32_t>(c, 0x12));
                                int32_t best = base;
                                bool most = c[4] == 6;
                                if (n <= 0) break;
                                for (int32_t j = 0; j < n; ++j) {
                                    if (mode == 1) c2 = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, j);
                                    else if (mode == 2) p2 = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, j);
                                    else if (mode == 3) t2 = (uint8_t*)Table_At(hook::Global<Collection*>(0x6cfc9c), 0, j);
                                    int32_t v = c2 == co && p2 == pl && t2 == terr
                                                    ? base
                                                    : RuleVar_Get(var, t2, c2, p2, value, idx, F<int32_t>(c, 0xe), F<int32_t>(c, 0x12));
                                    if (most ? !(best > v) : !(best < v)) best = v;
                                }
                                if (base != best) ok = 0;
                                break;
                            }
                            }
                        }
                    }
                    if (first) acc = ok;
                    else if (c[0xd] == 0) acc &= ok;
                    else if (c[0xd] == 1) acc |= ok;
                    else if (c[0xd] == 2) acc = ok;
                }
                if (!acc) continue;
                EventLog(ev, ++F<int32_t>(ev, 0x827), 0x199, 0xd3, 0xd);
                bool wasSpecial = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a73) != 0;
                bool late = ev[0x7ee] == 1;
                if (!late && Effects_RunTrigger(ev, 0, terr, (int32_t*)co, (int32_t*)pl, prev)) result = 1;
                int32_t nowSpecial = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a73) && !wasSpecial ? 1 : 0;
                if (!shown) {
                    if (!IsMultiplayer() && ev[0x7ef] == 8) World_ProcessEvents(hook::Global<uint8_t*>(0x6cec78));
                    prev = (int32_t)Event_ShowText(ev, 0, terr, co, pl, text, nowSpecial);
                    if (out && textLen < outSize - 2) {
                        if (textLen > 0) {
                            memcpy(out + strlen(out), (const void*)0x5c9ce8, 2);
                            textLen += 2;
                        }
                        ((char*(__cdecl*)(char*, const char*, size_t))0x5a1eb0)(out, text, (size_t)(outSize - textLen));
                        textLen += (int32_t)strlen(text);
                    }
                    shown = true;
                }
                if (late && Effects_RunTrigger(ev, 0, terr, (int32_t*)co, (int32_t*)pl, prev)) result = 1;
                if (F<int32_t>(ev, 0x7f5)) {
                    F<int32_t>(ev, 0x81f) = 1;
                    return result;
                }
            }
        }
    }
}
RT3_REPLACE(0x4323a0, Event_Check);

// 0x432de0 / 0x432e20. Collection subclass 0x5c9cec (heap records, 10 + 20,
// list 2) and its clear (releases every record through virtual slot 2).
RT3_NOINLINE Collection* __fastcall Collection5c9cec_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9cec;
    F<int32_t>(c, 0x88) = 0;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 1, 0);
    return c;
}
RT3_REPLACE(0x432de0, Collection5c9cec_Construct);

RT3_NOINLINE void __fastcall Collection_ReleaseAll(Collection* c) {
    while (Collection_Count(c)) {
        void** vt = *(void***)c;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(c, Collection_IdAt(c, 0, 0, 0));
    }
    Collection_Clear(c);
}
RT3_REPLACE(0x432e20, Collection_ReleaseAll);

RT3_NOINLINE void* __fastcall Collection_RecordAtList(Collection* t, int, uint32_t pos, int list) {
    return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, list));
}
RT3_REPLACE(0x432e60, Collection_RecordAtList);

void __fastcall Collection5c9cec_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9cec;
    Collection_ReleaseAll(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x432e80, Collection5c9cec_Destruct);

// 0x432ea0. Adds a fresh 0x88f-byte record (initialised by 0x42d5a0) under
// `key`, names it, and frees the template. Returns the new ID.
uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key) {
    uint8_t* tmp = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x88f);
    if (tmp) tmp = (uint8_t*)Record42d5a0_Init(tmp);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x432ed2)(t, 0, tmp, 0x88f, key, 0);
    Record42d5a0_SetName((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id, name);
    if (tmp) {
        Obj_FreeAllLists2(tmp);
        ((void(__cdecl*)(void*))0x53b080)(tmp);
    }
    return id;
}
RT3_REPLACE(0x432ea0, Table_AddNamedRecord);

RT3_DELETE_RECORD(0x432f10, Records_Delete432f10, Obj_FreeAllLists, void*)

// 0x432f40. Fires trigger type on every event of collection events (ids
// with flag 2), collecting their text into out (n bytes, cleared first);
// nothing while the game is over (0x6cec78+0x46c38), in the sandbox, or
// while paused (0x6cec74+0x68) except for trigger 9. If any effect ran the
// share prices are refreshed. A request (+0x88) repeats it as trigger 10.
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize) {
    for (;;) {
        if (out) out[0] = 0;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38) || hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68) && (uint8_t)trigger != 9) return;
        bool ran = false;
        for (int32_t i = 0; i < Collection_Count(events); ++i)
            if (Event_Check((uint8_t*)Collection_Get(events, 0, Collection_IdAt(events, 0, (uint32_t)i, 2)), 0, trigger, out, outSize)) ran = true;
        if (ran)
            for (int32_t j = 0; j < Table_CountField3f(hook::Global<Collection*>(0x62be10)); ++j)
                Company_SharePrice((uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, j), 0, 0, 1);
        if (!F<int32_t>(events, 0x88)) return;
        F<int32_t>(events, 0x88) = 0;
        outSize = 0;
        out = nullptr;
        trigger = (trigger & ~0xffu) | 0xa;
    }
}
RT3_REPLACE(0x432f40, Events_Fire);

// 0x433060. Saves the scenario rule sets: tag 0x4e99, version 1001, the collection
// (texts tag-encoded first), tag 0x4e9a, every set's rules, tag 0x4e9b.
RT3_NOINLINE void __fastcall RuleSets_Save(Collection* c, int, Stream st, int32_t recordSize) {
    StreamWriteU32(st, 0x4e99);
    int32_t version = 0x3e9;
    StreamWrite(st, &version, 4, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Record_EncodeTags((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
    Collection_Save(c, 0, st, recordSize);
    StreamWriteU32(st, 0x4e9a);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        EventRules_Save((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, st);
    StreamWriteU32(st, 0x4e9b);
}
RT3_REPLACE(0x433060, RuleSets_Save);

// 0x433130. Loads the scenario rule sets saved by RuleSets_Save; returns the sum of
// the three tag reads and the collection load (callers check it).
RT3_NOINLINE int32_t __fastcall RuleSets_Load(Collection* c, int, Stream st, int32_t recordSize) {
    int32_t ok = StreamSkip4(st, 0x4e99, 0, 0);
    int32_t version;
    StreamRead(st, &version, 4, 0, 1);
    ok += Collection_Load(c, 0, st, recordSize);
    ok += StreamSkip4(st, 0x4e9a, 0, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        EventRules_Load((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, st);
    ok += StreamSkip4(st, 0x4e9b, 0, 0);
    F<int32_t>(c, 0x88) = 0;
    return ok;
}
RT3_REPLACE(0x433130, RuleSets_Load);

// 0x4331e0. Records with byte +0x82b clear.
RT3_NOINLINE int32_t __fastcall Table_CountClear82b(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (!F<uint8_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x82b)) ++n;
    return n;
}
RT3_REPLACE(0x4331e0, Table_CountClear82b);

// 0x433230. Frees the buffer at +8 and clears +8 and +0.
RT3_NOINLINE void __fastcall Obj_FreeBuffer8(void* o) {
    if (F<void*>(o, 8)) ((void(__cdecl*)(void*))0x5a1145)(F<void*>(o, 8));
    F<void*>(o, 8) = nullptr;
    F<int32_t>(o, 0) = 0;
}
RT3_REPLACE(0x433230, Obj_FreeBuffer8);

// 0x433260. Whether a save in the save folder can be loaded in this game mode
// (+4): version 0x2583..0x620e98 and the header flags (+0x20, +0x34, +0x35, +0x3b6)
// matching the mode; always for option 0x1bf.
RT3_NOINLINE int32_t __fastcall Save_IsCompatible(uint8_t* g, int, const char* name) {
    if (Options_Check1bf(hook::Global<void*>(0x6cec74))) return 1;
    char path[0x12c];
    FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::Global<void*>(0x6cec74)), name, nullptr, nullptr,
               nullptr, nullptr, nullptr, nullptr);
    Stream s = StreamOpen(path, 1, 0x30d40, 1);
    int32_t ver;
    StreamRead(s, &ver, 4, 0, 1);
    int32_t ok = ver >= 0x2583 && ver <= hook::Global<int32_t>(0x620e98) ? 1 : 0;
    int32_t mode = F<int32_t>(g, 4);
    if (mode == 0xc && !((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)(name, (const char*)0x5c9cf8)) {
        ok = 0;
    } else if (ok) {
        uint8_t hdr[0x3c4];
        StreamSkip4(s, 0x2ee0, 1, 1);
        StreamRead(s, hdr, 0x3c2, 0, 1);
        mode = F<int32_t>(g, 4);
        uint8_t a = hdr[0x34], b = hdr[0x20];
        if ((mode == 0xc || mode == 8 || mode == 5 || mode == 0 || mode == 7) && (b || a)) ok = 0;
        else if ((mode == 9 || mode == 1) && (!b || a)) ok = 0;
        else if ((mode == 0xa || mode == 2) && !a) ok = 0;
        if (mode == 6 && hdr[0x35]) ok = 0;
        if (mode == 7 && F<int32_t>(hdr, 0x3b6) <= 1) ok = 0;
        StreamSkip4(s, 0x2ee1, 1, 1);
    }
    StreamClose(s);
    return ok;
}
RT3_REPLACE(0x433260, Save_IsCompatible);

// 0x4333f0. Lists the compatible saves (*.gm*, or *.smp with option +0x1bf) in the save
// folder into list (+0 count, +8 0x25a-byte entries: file name, display name at +0x12d,
// localized for kinds 5..7), sorted by display name. Quirk: the folder is scanned twice
// and the second pass is not bounded by the first count.
RT3_NOINLINE void __fastcall SaveList_Scan(uint8_t* list, int, int32_t kind) {
    auto findFirst = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190);
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    auto findClose = Import<int32_t(__stdcall*)(void*)>(0x5c819c);
    char fd[0x140];
    char* file = fd + 0x2c;
    F<int32_t>(list, 0) = 0;
    char pat[0x12c], dir[0x12c], path[0x190], full[0x1f4], path2[0x1f4], base[0x12c];
    Fmt(pat, (const char*)(Options_Check1bf(hook::Global<void*>(0x6cec74)) ? 0x5c9d1c : 0x5c9d14));
    Fmt(dir, (const char*)0x5c9d10, Options_SaveFolder(hook::Global<void*>(0x6cec74)));
    Fmt(path, (const char*)0x5c9d08, dir, pat);
    Fmt(full, path);
    void* h = findFirst(full, fd);
    if (h != (void*)-1) {
        do {
            if (*file != '.' && Save_IsCompatible(list, 0, file)) ++F<int32_t>(list, 0);
        } while (findNext(h, fd));
    }
    findClose(h);
    uint8_t* buf = (uint8_t*)Malloc(F<int32_t>(list, 0) * 0x25a + 1);
    F<uint8_t*>(list, 8) = buf;
    memset(buf, 0, F<int32_t>(list, 0) * 0x25a);
    Fmt(path2, (const char*)0x5c9d08, dir, pat);
    Fmt(full, path2);
    h = findFirst(full, fd);
    if (h != (void*)-1) {
        int32_t off = 0;
        do {
            if (*file == '.' || !Save_IsCompatible(list, 0, file)) continue;
            strcpy((char*)F<uint8_t*>(list, 8) + off, file);
            strcpy(base, file);
            StripExtension(base);
            const char* shown = (kind == 5 || kind == 6 || kind == 7) ? LocalizeName(base) : base;
            strcpy((char*)F<uint8_t*>(list, 8) + off + 0x12d, shown);
            off += 0x25a;
        } while (findNext(h, fd));
    }
    findClose(h);
    SortRecordsByName(F<uint8_t*>(list, 8), F<int32_t>(list, 0), 0x25a, 0x12d);
}
RT3_REPLACE(0x4333f0, SaveList_Scan);

// 0x4336a0. Constructs a save list of the given kind and scans the save folder.
RT3_NOINLINE uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind) {
    F<int32_t>(list, 4) = kind;
    F<void*>(list, 8) = nullptr;
    F<int32_t>(list, 0) = 0;
    SaveList_Scan(list, 0, kind);
    return list;
}
RT3_REPLACE(0x4336a0, SaveList_Init);

// 0x4336d0. World constructor: clears the fields set up later; returns this.
RT3_NOINLINE void* __fastcall World_Construct(uint8_t* w) {
    F<int32_t>(w, 0x4cae) = 0;
    F<int32_t>(w, 0x4cb2) = 0;
    for (int k = 0; k < 9; ++k) F<int32_t>(w, 0x46a80 + k * 4) = 0;
    F<int32_t>(w, 0x66b2) = 0;
    F<int32_t>(w, 0x66b6) = 0;
    F<int32_t>(w, 0x46c34) = 0;
    F<int32_t>(w, 0x66ae) = 0;
    return w;
}
RT3_REPLACE(0x4336d0, World_Construct);

void __fastcall Nop433730(void*) { __asm nop }
RT3_REPLACE(0x433730, Nop433730);

// 0x433740. +0x29 rounded down to 1/100 (after adding 0.001).
double __fastcall Obj_Rounded29(void* o) {
    const double eps = *(const double*)0x5c8678, hundred = *(const double*)0x5c85d8;
    int32_t c = (int32_t)(((double)F<float>(o, 0x29) + eps) * hundred);
    float r = (float)((double)c / hundred);
    return r;
}
RT3_REPLACE(0x433740, Obj_Rounded29);

RT3_NOINLINE void __fastcall Obj_Set21(void* o, int, int32_t v) { F<int32_t>(o, 0x21) = v; }
RT3_REPLACE(0x433790, Obj_Set21);

RT3_NOINLINE int32_t __fastcall Obj_Get21(void* o) { return F<int32_t>(o, 0x21); }
RT3_REPLACE(0x4337a0, Obj_Get21);

RT3_NOINLINE int32_t __fastcall Obj_Get25(void* o) { return F<int32_t>(o, 0x25); }
RT3_REPLACE(0x4337b0, Obj_Get25);

// 0x4337c0. Appends {next, name, a..f} (0x20 bytes) to the world's list at +0x66a6.
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c,
                                    int32_t d, int32_t e, int32_t f) {
    int32_t* n = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)(0x20);
    for (int i = 0; i < 8; ++i) n[i] = 0;
    ((void(__fastcall*)(char**, const char*))0x51d820)((char**)(n + 1), name); // String_Assign
    n[2] = a; n[3] = b; n[4] = c; n[5] = d; n[6] = e; n[7] = f;
    n[0] = 0;
    int32_t** head = &F<int32_t*>(w, 0x66a6);
    if (!*head) { *head = n; return; }
    int32_t* p = *head;
    while (*(int32_t**)p) p = *(int32_t**)p;
    *(int32_t**)p = n;
}
RT3_REPLACE(0x4337c0, World_AddNamedEntry);

// 0x433850. Runs the margin checks of all players until none of them traded
// (0x62be1c), guarded against re-entry (0x62be20).
RT3_NOINLINE void World_ProcessMarginCalls() {
    hook::Global<int32_t>(0x62be1c) = 1;
    if (hook::Global<int32_t>(0x62be20)) return;
    hook::Global<int32_t>(0x62be20) = 1;
    do {
        hook::Global<int32_t>(0x62be1c) = 0;
        int32_t n = Table_CountField4(hook::Global<Collection*>(0x6ceb9c));
        for (int32_t i = 0; i < n; ++i) Player_MarginCheck((uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i));
    } while (hook::Global<int32_t>(0x62be1c));
    hook::Global<int32_t>(0x62be20) = 0;
}
RT3_REPLACE(0x433850, World_ProcessMarginCalls);

RT3_NOINLINE int32_t Global6d07b4Set2() { return Global6d07b4Set() ? 1 : 0; }
RT3_REPLACE(0x4338c0, Global6d07b4Set2);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4338d0. Consume command 1000 by switching the active view to its payload
// mode. Other input events remain unhandled.
RT3_NOINLINE int32_t __stdcall View_OnModeInput(const uint32_t* event) {
    if (event[0] != 1000) return 1;
    View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, event[2], -1);
    return 0;
}
RT3_REPLACE(0x4338d0, View_OnModeInput);

// 0x433900. Points the camera at object o (unless `mode` 2): tells the list
// panel, then sets the camera mode to o with the zoom range of its kind -
// 120..250, 120..650 (vcall 0x70), 0..150 (0x6c), 1200..1000 (0x28) - or any
// zoom with `mode` 1.
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode) {
    if (mode == 2) return;
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d0820), 0, 0x87, 0x7922, 3, 0, 0);
    Obj_Float18(hook::Global<void*>(0x62be68));
    int32_t hi = 0xfa, lo = 0x78;
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x1c])(o)) hi = 0x28a;
    else if (((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x1b])(o)) hi = 0x96, lo = 0;
    else if (((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0xa])(o)) hi = 0x3e8, lo = 0x4b0;
    if (mode == 1) lo = 0, hi = 0xf423f;
    Camera_SetModeClampZoom(hook::Global<uint8_t*>(0x62be68), 0, (int32_t)(uintptr_t)o, lo, hi);
}
RT3_REPLACE(0x433900, Camera_FollowObject);

// 0x4339b0. Sets the difficulty (0..4; base interest rate 9% minus 1% per level)
// and reprices every active company's shares.
RT3_NOINLINE void __fastcall World_SetDifficulty(uint8_t* w, int, int32_t level) {
    if (F<int32_t>(w, 0x2d) == level) return;
    if (level < 0) level = 0;
    if (level > 4) level = 4;
    F<int32_t>(w, 0x2d) = level;
    F<float>(w, 0x29) = (float)(D(0x5c8658) - (double)level * D(0x5c85b8));
    Collection* cos = hook::Global<Collection*>(0x62be10);
    if (!cos) return;
    for (int32_t i = 0; i < Table_CountField3f(hook::Global<Collection*>(0x62be10)); ++i)
        Company_SharePrice((uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, i), 0, 0, 1);
}
RT3_REPLACE(0x4339b0, World_SetDifficulty);

// 0x433a40. In a locked scenario (date record +0x4aef) tells the player with
// message 0x14e and returns 0; otherwise returns 1.
RT3_NOINLINE uint32_t __stdcall Scenario_CheckEditable() {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) return 1;
    Message_Show(GetString(0x14e), 1, 0);
    return 0;
}
RT3_REPLACE(0x433a40, Scenario_CheckEditable);

// 0x433aa0. Whether player p may do this: no in a locked scenario (0x4aa7,
// message 0x14e) or without a company (+0x21, 0x14f).
RT3_NOINLINE uint32_t __fastcall Player_CheckHasCompany(uint8_t* p) {
    int32_t msg;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aa7)) msg = 0x14e;
    else if (!F<int32_t>(p, 0x21)) msg = 0x14f;
    else return 1;
    Message_Show(GetString(msg), 1, 0);
    return 0;
}
RT3_REPLACE(0x433aa0, Player_CheckHasCompany);

// 0x433b00. Whether player p may build trains: no in a locked scenario
// (0x4aa3, 0x14e), without a company (0x151) or with fewer than two tracks
// (0x6cfca8, 0x152).
RT3_NOINLINE uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p) {
    int32_t msg;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aa3)) msg = 0x14e;
    else if (F<int32_t>(p, 0x21) <= 0) msg = 0x151;
    else if (hook::Global<Collection*>(0x6cfca8) && Collection_Count(hook::Global<Collection*>(0x6cfca8)) < 2) msg = 0x152;
    else return 1;
    Message_Show(GetString(msg), 1, 0);
    return 0;
}
RT3_REPLACE(0x433b00, Player_CheckCanRunTrains);

// 0x433b80. Debug hooks/validation over the main tables (unless 0x62be40).
RT3_NOINLINE void RunDebugValidation() {
    if (hook::Global<int32_t>(0x62be40)) return;
    if (Collection* t = hook::Global<Collection*>(0x6cec20)) Table_Hook47d8d0(t);
    if (Collection* t = hook::Global<Collection*>(0x62b26c)) Table_TouchRegionSites(t);
    if (Collection* t = hook::Global<Collection*>(0x6cfcbc)) Table_Hook4a7350(t);
    if (Collection* t = hook::Global<Collection*>(0x6cfca8)) Track_Validate(t);
}
RT3_REPLACE(0x433b80, RunDebugValidation);

// 0x433bd0. Era economy factors: the base value interpolated from the year table
// (0x5f3980: 21 x {year, value}) for the year +0xd, scaled into +0x65..+0x6d and
// the derived rates +0x71.. (1 - rate) and +0x7d...
RT3_NOINLINE void __fastcall Economy_UpdateEraFactors(uint8_t* o) {
    int32_t year = F<uint16_t>(o, 0xd);
    const int32_t* tab = (const int32_t*)0x5f3978; // tab[i*2] = year of entry i-1, tab[i*2+1] = value
    double v;
    int32_t i = 0;
    while (i < 0x15 && !(year <= tab[i * 2 + 2])) ++i;
    if (i == 0x15) {
        v = (float)hook::Global<int32_t>(0x5f3a24);
    } else if (i == 0) {
        v = (float)hook::Global<int32_t>(0x5f3984);
    } else {
        int32_t y0 = tab[i * 2];
        int32_t span = tab[i * 2 + 2] - y0;
        if (span < 1) span = 1;
        float t = (float)((double)(float)(year - y0) / (float)span);
        if (t < *(const float*)0x5c8598 || (double)t > D(0x5c8770)) t = 0.0f;
        double prev = (double)tab[i * 2 + 1];
        float next = (float)tab[i * 2 + 3];
        v = (D(0x5c8770) - t) * prev + (double)next * t;
    }
    float base = (float)v;
    F<float>(o, 0x69) = (float)((double)base * D(0x5c9ed8));
    F<float>(o, 0x65) = (float)((double)base * D(0x5c8888));
    F<float>(o, 0x6d) = (float)((double)base * D(0x5c8680));
    for (int k = 0; k < 3; ++k) {
        float q = (float)(F<float>(o, 0x65 + k * 4) / D(0x5c8a20));
        float w = (float)(((double)q * D(0x5c8da8) + D(0x5c8618)) * D(0x5c9ed0));
        if (w < D(0x5c88a0)) *(uint32_t*)&w = 0x38d1b717;
        if (Game_EffectiveVersion() >= 0x67) w = (float)((double)w * D(0x5c8878));
        if ((double)w > D(0x5c8988)) *(uint32_t*)&w = 0x3e19999a;
        F<float>(o, 0x71 + k * 4) = (float)(D(0x5c8770) - w);
        float r = (float)(D(0x5c8a20) / F<float>(o, 0x65 + k * 4));
        F<float>(o, 0x7d + k * 4) = (float)(((double)r * D(0x5c8680) + D(0x5c88c8)) * D(0x5c9ec8));
    }
}
RT3_REPLACE(0x433bd0, Economy_UpdateEraFactors);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x433da0. Set editor-button groups, select mode 25, and apply its byte mode.
RT3_NOINLINE void __stdcall World_SetEditorButtons(uint32_t modeArg, uint32_t disabledArg) {
    Event(hook::Global<void*>(0x6d0820), (uint8_t)disabledArg ? 0x87 : 0x65, 0x7925, 3, 0, 0);
    uint8_t mode = (uint8_t)modeArg;
    Event(hook::Global<void*>(0x6d0820), mode == 1 ? 0x65 : 0x87, 0x7924, 3, 0, 0);
    uint32_t type = mode == 1 ? 0x87 : 0x65;
    Event(hook::Global<void*>(0x6d0820), type, 0x7923, 3, 0, 0);
    for (uint32_t id = 0x7db; id < 0x838; ++id) Event(hook::Global<void*>(0x6d0818), type, id, 3, 0, 0);
    if ((uint8_t)disabledArg) Event(hook::Global<void*>(0x6d0820), 0x87, 0x7924, 3, 0, 0);
    View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 25, -1);
    hook::Orig<void(__fastcall*)(void*, int, uint32_t)>(0x43a0a0)(hook::Global<void*>(0x62be68), 0, mode);
}
RT3_REPLACE(0x433da0, World_SetEditorButtons);

RT3_NOINLINE void __fastcall Nop433e90(void*) { __asm nop }
RT3_REPLACE(0x433e90, Nop433e90);

// 0x433ea0. Year roll-over of the world stats: this year's 53 cargo values
// (+0xa26) and three totals are added to the running sums and cleared.
RT3_NOINLINE void __fastcall World_RollStats(uint8_t* w) {
    for (int32_t i = 0; i < 0x35; ++i) {
        float t = F<float>(w, 0xa26 + i * 4);
        F<float>(w, 0xa26 + i * 4) = 0.0f;
        F<float>(w, 0xafa + i * 4) = (float)((double)t + F<float>(w, 0xafa + i * 4));
    }
    float t = F<float>(w, 0xbce);
    F<float>(w, 0xbce) = 0.0f;
    F<float>(w, 0xbd2) = (float)((double)t + F<float>(w, 0xbd2));
    t = F<float>(w, 0xbd6);
    F<float>(w, 0xbd6) = 0.0f;
    F<float>(w, 0xbda) = (float)((double)F<float>(w, 0xbda) + t);
    t = F<float>(w, 0x4caa);
    F<float>(w, 0x4caa) = 0.0f;
    F<float>(w, 0x4ca6) = (float)((double)F<float>(w, 0x4ca6) + t);
}
RT3_REPLACE(0x433ea0, World_RollStats);

// 0x433f20. Adds the train-detail strip (control 0x3f9, bitmap font
// "TrainDetail.imb", text 0x155) at (x + 0.15, y + 0.075) to window `win`.
void __fastcall Window_AddTrainDetailStrip(uint8_t* win, int, float x, float y, int32_t, int32_t) {
    uint8_t def[0xa5];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)(x + D(0x5c8988));
    F<float>(def, 0xb) = (float)(y + D(0x5c9ee0));
    F<uint32_t>(def, 0xf) = 0x3d4ccccd;
    F<uint32_t>(def, 0x13) = 0x3d266666;
    def[0x89] = 0xe;
    def[0x8a] = 0xf;
    F<uint16_t>(def, 0x5f) = 0x79;
    F<uint16_t>(def, 0x61) = 0x7a;
    F<uint16_t>(def, 0) = 0x3f9;
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5c9ee8);
    String_Assign(&F<char*>(def, 0x3f), GetString(0x155));
    uint8_t* c = (uint8_t*)ExeNew(0xe9);
    if (c) c = Control_Construct(c, 0, def);
    Window_AddChild(win, 0, c, 0);
    Free(F<void*>(def, 0x43));
    Free(F<void*>(def, 0x3f));
}
RT3_REPLACE(0x433f20, Window_AddTrainDetailStrip);

// 0x434000. As 0x424050 for the slots +0x8a with last key +0xa22.
void __fastcall Obj_SetSlot8a(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xa22)) return;
    if (add) F<int32_t>(o, 0x8a + idx * 4) += value;
    else F<int32_t>(o, 0x8a + idx * 4) = value;
    if (key) F<int32_t>(o, 0xa22) = key;
}
RT3_REPLACE(0x434000, Obj_SetSlot8a);

   // 0x5cfcfc

// 0x434050. True in a multiplayer game (the lobby object exists).
RT3_NOINLINE int32_t IsMultiplayer() { return hook::Global<void*>(0x6cd8d8) != nullptr ? 1 : 0; }
RT3_REPLACE(0x434050, IsMultiplayer);

// 0x434060. Not in a lobby, or +0x66ae == 1.
int32_t __fastcall World_IsHostOrLocal(void* w) {
    if (!hook::Global<void*>(0x6cd8d8)) return 1;
    return F<int32_t>(w, 0x66ae) == 1 ? 1 : 0;
}
RT3_REPLACE(0x434060, World_IsHostOrLocal);

// 0x434080. +0x4cbe if above the threshold 0x5c8a20, else 50.
RT3_NOINLINE float __fastcall World_Value4cbe(void* w) {
    float f = F<float>(w, 0x4cbe);
    if ((double)f > *(const double*)0x5c8a20) return f;
    return 50.0f;
}
RT3_REPLACE(0x434080, World_Value4cbe);

uint32_t World46aa0() { return g_world ? F<uint32_t>(g_world, 0x46aa0) : 0; }
RT3_REPLACE(0x4340d0, World46aa0);

RT3_NOINLINE void __fastcall Obj_SetName8(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 8;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x4340f0, Obj_SetName8);

RT3_NOINLINE int32_t __fastcall Obj_Has78(void* o) { return !F<uint8_t>(o, 0x82) && F<int32_t>(o, 0x78) != -1 ? 1 : 0; }
RT3_REPLACE(0x434110, Obj_Has78);

// 0x4371a8 keeps using ECX (the world) after the call.
__declspec(naked) void World_UpdateEraFactor() { RT3_KEEP_ECX_EDX(World_UpdateEraFactor_Impl, 0) }
RT3_REPLACE(0x434130, World_UpdateEraFactor);

// 0x434200. Tells the network player of slot (player record slot + 1 in
// 0x6ceb9c) its slot number (type 0x55, reliable): by name match (case-blind)
// in the engine's player list, or, with 0x6ce9bc clear, the pick-th entry,
// whose name then also becomes the record's. 1 when sent.
RT3_NOINLINE int32_t __stdcall Net_SendSlot(int32_t slot, int32_t pick) {
    char name[0x100];
    name[0] = 0;
    uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(slot + 1));
    uint8_t* n = *(uint8_t**)(uintptr_t)Net_LockQueue();
    for (int32_t i = 0; n; n = *(uint8_t**)n, ++i) {
        if (!hook::Global<int32_t>(0x6ce9bc)) {
            if (i != pick) continue;
            strcpy(name, (const char*)n + 8);
            strcpy((char*)p + 8, name);
            break;
        }
        if (((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)p + 8, (const char*)n + 8) == 0) {
            strcpy(name, (const char*)n + 8);
            break;
        }
    }
    Net_UnlockQueue();
    if (!name[0]) return 0;
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, name, 0x55, 4, (const uint8_t*)&slot, 1, 0);
    return 1;
}
RT3_REPLACE(0x434200, Net_SendSlot);

// 0x434300. World w is torn down (inside a +0x4c93 busy count): its two
// tables (+0x66b2, +0x66b6) and every global table in turn - vehicles,
// trains, companies, players, the map, cargo, car models, car types,
// building types, sites, news, the log - are destroyed, freed and cleared;
// its held images (+0x46a80..+0x46aa0 and one per 0x6d4020+0x429b0 entry
// from +0x46aa4) are released; the +0x66a6 list and the +0x66aa node are
// freed, +0x66ae cleared, and the tutorial's map pointers deleted.
RT3_NOINLINE void __fastcall World_Teardown(uint8_t* w) {
    auto Free = (void(__cdecl*)(void*))0x53b080;
    ++F<int32_t>(w, 0x4c93);
    if (Collection* c = F<Collection*>(w, 0x66b2)) {
        Collection_Destruct(c);
        Free(c);
    }
    F<Collection*>(w, 0x66b2) = nullptr;
    if (Collection* c = F<Collection*>(w, 0x66b6)) {
        Collection_Destruct(c);
        Free(c);
    }
    F<Collection*>(w, 0x66b6) = nullptr;
    struct Table {
        uint32_t at;
        void(__fastcall* destroy)(Collection*);
    };
    static const Table kTables[] = {
        { 0x62b244, &Vehicles_Destruct },           { 0x6cfcbc, &Collection5d00c8_Destruct },
        { 0x62be10, &Companies_Destruct },          { 0x6ceb9c, &Players_Destruct },
        { 0x62c120, (void(__fastcall*)(Collection*))&Map_RuntimeDestructor },
        { 0x62ba8c, &Collection5c997c_Destruct },   { 0x6ada84, &Collection5ce068_Destruct },
        { 0x62ba88, &Collection5c9824_Destruct },   { 0x62b2fc, &Sites_Destruct },
        { 0x62b268, &Collection5c93cc_Destruct },   { 0x6cea4c, &Collection5ce418_Destruct },
        { 0x6acd34, &Collection5cae10_Destruct },
    };
    for (const Table& t : kTables) {
        if (Collection* c = hook::Global<Collection*>(t.at)) {
            t.destroy(c);
            Free(c);
        }
        hook::Global<Collection*>(t.at) = nullptr;
    }
    static const uint16_t kImages[] = { 0x6a90, 0x6a8c, 0x6a88, 0x6a84, 0x6a80, 0x6a94, 0x6a98, 0x6a9c, 0x6aa0 };
    for (uint16_t o : kImages) {
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(w, 0x40000 + o));
        F<void*>(w, 0x40000 + o) = nullptr;
    }
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0)); ++i) {
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(w, 0x46aa4 + i * 4));
        F<void*>(w, 0x46aa4 + i * 4) = nullptr;
    }
    while (uint8_t* node = F<uint8_t*>(w, 0x66a6)) {
        uint8_t* next = F<uint8_t*>(node, 0);
        FreeAndNull((void**)(node + 4));
        FreeAndNull(&F<void*>(w, 0x66a6));
        F<uint8_t*>(w, 0x66a6) = next;
    }
    if (uint8_t* node = F<uint8_t*>(w, 0x66aa)) {
        FreeAndNull((void**)(node + 4));
        FreeAndNull(&F<void*>(w, 0x66aa));
        F<uint8_t*>(w, 0x66aa) = nullptr;
    }
    F<int32_t>(w, 0x66ae) = 0;
    if (hook::Global<int32_t>(0x6d3b4c)) MapPointers_Delete();
    --F<int32_t>(w, 0x4c93);
}
RT3_REPLACE(0x434300, World_Teardown);

// 0x434680. Sets the game speed (0..5, 0..9 with the fast-speeds option +0x2c8).
// In multiplayer a user request (`request` 1) goes to the host instead: a host
// that isn't already waiting (0x6cd924) sends message 0xb {speed, announce} (and
// shows the wait cursor 6 while announcing, keeping the old one in 0x62be24); a
// client gets string 0xe20; requests are dropped while a change is pending. A
// change updates +0x19 (and +0x1d, the speed to resume, when not pausing), ticks
// button 0x80c + speed in the main window and, if `announce`, shows the speed name
// (0x14d) in the status line for 3 s. Returns 1 when applied (or already set).
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce) {
    int32_t maxSpeed = hook::Global<uint8_t*>(0x6cec74)[0x2c8] ? 9 : 5;
    if (speed < 0) speed = 0;
    if (speed > maxSpeed) speed = maxSpeed;
    if (speed == F<int32_t>(w, 0x19)) return 1;
    uint8_t* lobby = hook::Global<uint8_t*>(0x6cd8d8);
    int32_t& pendingCursor = hook::Global<int32_t>(0x62be24);
    if (pendingCursor) {
        if (request == 1) return 1;
    } else if (request == 1 && lobby) {
        if (F<int32_t>(w, 0x66ae) != 1) return 0;
        if (!hook::Global<int32_t>(0x6d40dc)) {
            ((void(__fastcall*)(uint8_t*, int, const char*, int32_t, int32_t, int32_t, int32_t))0x469a50)(lobby, 0, GetString(0xe20), 1, 0, 0, 0);
            return 0;
        }
        if (hook::Global<int32_t>(0x6cd924)) return 0;
        hook::Global<int32_t>(0x6cd924) = 1;
        uint8_t msg[2] = { (uint8_t)speed, (uint8_t)announce };
        if (announce) {
            uint8_t* cursor = hook::Global<uint8_t*>(0xccbb20);
            pendingCursor = F<int32_t>(cursor, 4);
            Cursor_Set(cursor, 0, 6, 0, 0);
        }
        ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, uint8_t*))0x407870)(hook::Global<uint8_t*>(0x6cd8d8), 0, 0xb, 2, msg);
        return 1;
    }
    hook::Global<int32_t>(0x6cd924) = 0;
    if (hook::Global<uint8_t*>(0x6cd8d8) && pendingCursor) {
        Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, pendingCursor, 0, 0);
        pendingCursor = 0;
    }
    F<int32_t>(w, 0x19) = speed;
    if (speed) F<int32_t>(w, 0x1d) = speed;
    if (uint8_t* view = hook::Global<uint8_t*>(0x6d0818)) Window_SendEvent5(view, 0, 0x74, speed + 0x80c, 1, 0, 0);
    if (announce) {
        char text[0x120];
        const char* name = GetString(((const int32_t*)0x5ee278)[F<int32_t>(w, 0x19)]);
        FormatText(text, GetString(0x14d), name, 0, 0, 0, 0, 0, 0, 0);
        StatusLine_Set(hook::Global<uint8_t*>(0x6d401c), 0, text, 0xbb8);
    }
    return 1;
}
RT3_REPLACE(0x434680, World_SetSpeed);

// 0x434850. Steps the game speed by `delta` as a user request, announced.
RT3_NOINLINE int32_t __fastcall World_StepSpeed(uint8_t* w, int, int32_t delta) {
    return World_SetSpeed(w, 0, F<int32_t>(w, 0x19) + delta, 1, 1);
}
RT3_REPLACE(0x434850, World_StepSpeed);

RT3_NOINLINE void* __fastcall Obj_Company21(void* o) { return RefPositive(g_companies, F<int32_t>(o, 0x21)); }
RT3_REPLACE(0x434870, Obj_Company21);

// 0x434890. Sets the player (+0x25) and copies its company (+0x1dd) to +0x21.
RT3_NOINLINE void __fastcall Obj_SetPlayer(void* o, int, uint32_t player) {
    F<uint32_t>(o, 0x25) = player;
    F<int32_t>(o, 0x21) = F<int32_t>(Collection_Get(g_players, 0, player), 0x1dd);
}
RT3_REPLACE(0x434890, Obj_SetPlayer);

RT3_NOINLINE void* __fastcall Obj_Ref25(void* o) { return RefPositive(g_table6ceb9c, F<int32_t>(o, 0x25)); }
RT3_REPLACE(0x4348c0, Obj_Ref25);

// 0x4348e0. Whether the UI is busy (blocks game input). Some states keep it
// busy for another 1.5 s (deadline 0x62be38).
RT3_NOINLINE int32_t UiBusy() {
    if (hook::Global<int32_t>(0x6d4000)) return 1;
    bool stamp = false;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc64) > 0) {
        stamp = true;
    } else {
        int32_t screen = F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c);
        if (screen == 0xc) {
            if (F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 0x400) stamp = true;
            else if (void* d = hook::Global<void*>(0x6d1a8c)) stamp = Obj_Has78(d) != 0;
        }
        if (!stamp && screen == 6) {
            uint8_t* w = hook::Global<uint8_t*>(0x6d1720);
            if (w && F<int32_t>(w, 0x7c)) stamp = true;
        }
    }
    if (stamp) {
        hook::Global<int32_t>(0x62be38) = ClockMs() + 0x5dc;
        return 1;
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x62be68), 0x38e)) return 1;
    if (hook::Global<int32_t>(0x62be38) && hook::Global<int32_t>(0x62be38) > ClockMs()) return 1;
    hook::Global<int32_t>(0x62be38) = 0;
    return 0;
}
RT3_REPLACE(0x4348e0, UiBusy);

// 0x4349a0. Number of active players with the +0x1e1 flag (humans); 1 without a player table.
RT3_NOINLINE int32_t CountHumanPlayers() {
    if (!g_players) return 1;
    int32_t n = 0;
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i)
        if (F<int32_t>(Table_NthField4(g_players, 0, i), 0x1e1) != 0) ++n;
    return n;
}
RT3_REPLACE(0x4349a0, CountHumanPlayers);

// 0x4349f0. World w: stops following (+0x45 / +0x49 cleared) and sets what
// the camera (0x62be68) follows: with `on` the object `target` (zooming out
// to 200 when closer than 0x5c9f00, closing the model preview), else
// nothing; the follow button (0x6d0820, 0x7922) is updated either way.
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target) {
    F<int32_t>(w, 0x45) = 0;
    F<int32_t>(w, 0x49) = 0;
    if (!hook::Global<uint8_t*>(0x6d0820)) return;
    if (!on) {
        Window_SendEvent5(hook::Global<uint8_t*>(0x6d0820), 0, 0x87, 0x7922, 3, 0, 0);
        if (F<int32_t>(hook::Global<uint8_t*>(0x62be68), 0x60)) Camera_SetMode(hook::Global<uint8_t*>(0x62be68), 0, 0);
        return;
    }
    Window_SendEvent5(hook::Global<uint8_t*>(0x6d0820), 0, 0x65, 0x7922, 3, 0, 0);
    if (target == F<uint8_t*>(hook::Global<uint8_t*>(0x62be68), 0x60)) return;
    Camera_SetMode(hook::Global<uint8_t*>(0x62be68), 0, (int32_t)(uintptr_t)target);
    float zoom = (float)Obj_Float18(hook::Global<uint8_t*>(0x62be68));
    if (zoom > Fl_207(0x5c9f00)) Camera_SetZoom(hook::Global<uint8_t*>(0x62be68), 0, 0x43480000);
    ModelPreview_Close();
}
RT3_REPLACE(0x4349f0, World_SetFollow);

// 0x434aa0. Monthly economy drift (`force` -1), or a forced economy level
// (0..4). The momentum +0x31 drifts by a random -5..5 (+2) against the pressure
// +0x3d (months passed x (level +0x2d - 2), decaying by 0x5c9f08), halved in the
// first 22 months by chance, clamped to -5..5; with |momentum| above a random
// 0..10 the level steps one way. A change is announced (a headline when only one
// human plays and the option +0x27b is on, else a ticker line; strings 0x620c70)
// and applied. Auto drift skips sandbox / no-economy modes, the first 10 months,
// repeated calls in the same month and single-player pauses (+0x46c38).
RT3_NOINLINE void __fastcall World_EconomyTick(uint8_t* w, int, int32_t force) {
    if (force == -1) {
        uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
        uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
        if (F<int32_t>(opt, 0x170) || F<int32_t>(opt, 0x68) || hook::Global<int32_t>(0x6d3b4c) || F<int32_t>(world, 0x4aeb) ||
            F<int32_t>(world, 0x4aef))
            return;
    }
    int32_t last = F<uint16_t>(w, 0x35) * 12 + w[0x37];
    int32_t month = w[0xf];
    int32_t now = F<uint16_t>(w, 0xd) * 12 + month;
    if (now == last && force == -1) return;
    F<uint32_t>(w, 0x35) = F<uint32_t>(w, 0xd);
    F<uint32_t>(w, 0x39) = F<uint32_t>(w, 0x11);
    if (F<int32_t>(w, 0x46c38) && force == -1) return;
    int32_t elapsed = (F<uint16_t>(w, 0xd) - F<uint16_t>(w, 5)) * 12 + month - 1;
    if (elapsed < 10 && force == -1) return;
    int32_t decay = Ftol((double)F<int32_t>(w, 0x3d) * D(0x5c9f08));
    F<int32_t>(w, 0x3d) = (now - last) * (F<int32_t>(w, 0x2d) - 2) - decay;
    int32_t r = SimRand(-5, 5) + 2;
    F<int32_t>(w, 0x31) = Ftol((double)r - (double)F<int32_t>(w, 0x3d) / D(0x5c85f0) + (double)F<int32_t>(w, 0x31));
    if (elapsed < 0x16 && SimRand(0, 0x18) > elapsed) F<int32_t>(w, 0x31) = Ftol((double)F<int32_t>(w, 0x31) * D(0x5c8750));
    if (F<int32_t>(w, 0x31) > 5) F<int32_t>(w, 0x31) = 5;
    if (F<int32_t>(w, 0x31) < -5) F<int32_t>(w, 0x31) = -5;
    int32_t roll = SimRand(0, 10);
    int32_t m = F<int32_t>(w, 0x31);
    int32_t level = F<int32_t>(w, 0x2d);
    int32_t next = level;
    if ((m < 0 ? -m : m) - roll > 0) next = m > 0 ? level + 1 : level - 1;
    if (force != -1) next = force;
    if (next < 0) next = 0;
    else if (next > 4) next = 4;
    if (next == level) return;
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x27b) && CountHumanPlayers() <= 1) {
        int32_t idx = next + 5 * (next >= F<int32_t>(w, 0x2d) ? 1 : 0);
        const char* text = GetString(*(const int32_t*)(0x620c74 + idx * 8));
        int32_t idx2 = next + 5 * (next >= F<int32_t>(w, 0x2d) ? 1 : 0);
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, GetString(*(const int32_t*)(0x620c70 + idx2 * 8)), text, 0, 0);
    } else {
        int32_t idx = next + 5 * (next >= F<int32_t>(w, 0x2d) ? 1 : 0);
        char buf[0x204];
        strcpy(buf, GetString(*(const int32_t*)(0x620c70 + idx * 8)));
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, buf, 2, 0, 0);
    }
    World_SetDifficulty(w, 0, next);
}
RT3_REPLACE(0x434aa0, World_EconomyTick);

// 0x434d40. Marks the state object of every kind-2 building (+0x7b0 = 1).
RT3_NOINLINE void MarkKind2Buildings() {
    for (int32_t i = 0; i < Collection_Count(g_buildings); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0));
        void** vt = *(void***)b;
        uint8_t* st = (uint8_t*)((void*(__thiscall*)(void*))vt[0x80 / 4])(b);
        if (F<uint8_t>(st, 0x32) == 2) {
            st = (uint8_t*)((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            F<int32_t>(st, 0x7b0) = 1;
        }
    }
}
RT3_REPLACE(0x434d40, MarkKind2Buildings);

// 0x434db0. The stock market gate (Stock Market button, hotkey): refused
// when the scenario forbids it (Scenario_CheckEditable) or - with "That
// action is not allowed in this scenario" (string 0x14e) - when the world
// flag +0x4a7f is set. DesyncWatch's always_stock_market patches this
// function's start into "return 1".
RT3_NOINLINE uint32_t __stdcall StockMarket_IsAllowed() {
    if (!Scenario_CheckEditable()) return 0;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f)) return 1;
    Dialog_RunMessage(GetString(0x14e), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    return 0;
}
RT3_REPLACE(0x434db0, StockMarket_IsAllowed);

// 0x434e20. Whether player p may manage stock: no when all three locks are on
// (0x4ab3, 0x4ab7, 0x4abf: message 0x14e) or without a company unless allowed
// (options +0x68; 0x150).
RT3_NOINLINE uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p) {
    uint8_t* d = hook::Global<uint8_t*>(0x6cec78);
    int32_t msg;
    if (F<int32_t>(d, 0x4ab3) && F<int32_t>(d, 0x4ab7) && F<int32_t>(d, 0x4abf)) msg = 0x14e;
    else if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68) && !F<int32_t>(p, 0x21)) msg = 0x150;
    else return 1;
    Message_Show(GetString(msg), 1, 0);
    return 0;
}
RT3_REPLACE(0x434e20, Player_CheckStockAllowed);

RT3_NOINLINE int32_t __fastcall World_LookupNamed1(void* w, int, const char* name) {
    return FindNamed(F<Collection*>(w, 0x66b2), name, 0x1e);
}
RT3_REPLACE(0x434ea0, World_LookupNamed1);

// 0x434f20. Sets the scenario's named value name (list +0x66b2: records of a
// 0x1e-byte name and an int at +0x1e, matched by first letter and
// case-insensitively), adding a record with the name capitalized when missing;
// then refreshes which industry types are available (the original 0x412c10, which
// stays unhooked).
RT3_NOINLINE void __fastcall World_SetNamedValue(uint8_t* w, int, const char* name, int32_t value) {
    Collection* l = F<Collection*>(w, 0x66b2);
    int32_t n = Collection_Count(l);
    char up = (char)ToUpper(name[0]);
    bool found = false;
    for (int32_t i = 0; i < n; ++i) {
        Collection* t = F<Collection*>(w, 0x66b2);
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (up != (char)r[0]) continue;
        t = F<Collection*>(w, 0x66b2);
        r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (Stricmp(name, (const char*)r)) continue;
        t = F<Collection*>(w, 0x66b2);
        F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0)), 0x1e) = value;
        found = true;
        break;
    }
    if (!found) {
        uint8_t rec[0x22];
        memset(rec, 0, sizeof rec);
        F<int32_t>(rec, 0x1e) = value;
        strcpy((char*)rec, name);
        rec[0] = (uint8_t)ToUpper((char)rec[0]);
        Collection* t = F<Collection*>(w, 0x66b2);
        ((void(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)t)[4 / 4])(t, 0, rec, 0, 0, 0);
    }
    if (hook::Global<void*>(0x62b268)) ((void(__fastcall*)(void*))0x412c10)(hook::Global<void*>(0x62b268));
}
RT3_REPLACE(0x434f20, World_SetNamedValue);

RT3_NOINLINE int32_t __fastcall World_LookupNamed2(void* w, int, const char* name) {
    return FindNamed(F<Collection*>(w, 0x66b6), name, 0x3d);
}
RT3_REPLACE(0x435030, World_LookupNamed2);

// 0x4350b0. Sets the scenario's named flag name (list +0x66b6: records of a
// 0x3d-byte name and an int at +0x3d, matched by first letter and
// case-insensitively), adding a record with the name capitalized when
// missing; then re-applies the flags of the table 0x6ada84.
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value) {
    int32_t n = Collection_Count(F<Collection*>(w, 0x66b6));
    char up = (char)ToUpper(name[0]);
    for (int32_t i = 0; i < n; ++i) {
        Collection* t = F<Collection*>(w, 0x66b6);
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (up != (char)r[0]) continue;
        t = F<Collection*>(w, 0x66b6);
        r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (Stricmp(name, (const char*)r)) continue;
        t = F<Collection*>(w, 0x66b6);
        F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0)), 0x3d) = value;
        if (Collection* f = hook::Global<Collection*>(0x6ada84)) Table_RefreshNamedFlags(f);
        return;
    }
    uint8_t rec[0x41];
    memset(rec, 0, sizeof rec);
    F<int32_t>(rec, 0x3d) = value;
    strcpy((char*)rec, name);
    rec[0] = (uint8_t)ToUpper((char)rec[0]);
    Collection* t = F<Collection*>(w, 0x66b6);
    ((void(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)t)[4 / 4])(t, 0, rec, 0, 0, 0);
    if (Collection* f = hook::Global<Collection*>(0x6ada84)) Table_RefreshNamedFlags(f);
}
RT3_REPLACE(0x4350b0, World_SetNamedFlag);

// 0x4351c0. Schedules the next weather spell: a random kind (40% heavy 0xff, 30%
// 200, 30% 230 in +0x61), a length of 40..60 days and a start some hours ahead
// (aligned to the clock's time of day, maybe a day later); with `scaled` the start
// is spread by how far the scene's weather level (+0x15) is from the target. Sets
// the start (+0x51) and the build-up / peak / end times (+0x55 / +0x59 / +0x5d).
RT3_NOINLINE void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled) {
    int32_t roll = SimRand(0, 100);
    int32_t level = F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15);
    int32_t len, d;
    if (roll < 0x28) {
        F<int32_t>(w, 0x61) = 0xff;
        len = SimRand(0x28, 0x37) * 0x5a0;
        d = SimRand(0x898, 0xa28);
    } else if (roll < 0x46) {
        F<int32_t>(w, 0x61) = 200;
        len = SimRand(0x2d, 0x3c) * 0x5a0;
        d = SimRand(0x320, 0x41a);
    } else {
        F<int32_t>(w, 0x61) = 0xe6;
        len = SimRand(0x28, 0x37) * 0x5a0;
        d = SimRand(0x4b0, 0x640);
    }
    float ahead = (float)((double)d / D(0x5c85d8));
    World_SetTime(w, 0, F<int32_t>(w, 0x15));
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, &h, &m, &s);
    float now = (float)((double)m / D(0x5c88a8) + (double)s / D(0x5c9f30) + (double)h);
    ahead = (float)((double)ahead - now);
    if (SimRand(0, 100) < 0x32) ahead = (float)((double)ahead + D(0x5c9ab8));
    if (!(ahead > D(0x5c8978)) && ahead == ahead) ahead = (float)((double)ahead + D(0x5c9ab8));
    if (scaled) {
        int32_t kind = F<int32_t>(w, 0x61);
        if (kind <= level) kind = level;
        F<int32_t>(w, 0x61) = kind;
        if (level >= kind) ahead = (float)((double)len * D(0x5c9f28) / D(0x5c9f20));
        else {
            int32_t c = kind < 1 ? 1 : kind;
            ahead = (float)((double)(float)level / (float)c * ((double)len * D(0x5c9f28)) / D(0x5c9f20));
        }
    }
    int32_t start = Ftol((double)ahead * Fl_204(0x5c9f18) + (float)F<int32_t>(w, 0x15));
    F<int32_t>(w, 0x51) = start;
    double l = len, st = start;
    F<int32_t>(w, 0x55) = Ftol(l * D(0x5c9f28) + st);
    F<int32_t>(w, 0x59) = Ftol(l * D(0x5c9f10) + st);
    F<int32_t>(w, 0x5d) = Ftol(st + l);
}
RT3_REPLACE(0x4351c0, Weather_ScheduleNext);

// 0x435460 / 0x435480. Renderer depth/mode (+0x15) above 128 / zero.
int32_t RendererMode15Above128() { return F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15) > 0x80 ? 1 : 0; }
RT3_REPLACE(0x435460, RendererMode15Above128);

RT3_NOINLINE int32_t RendererMode15Zero() { return F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15) == 0 ? 1 : 0; }
RT3_REPLACE(0x435480, RendererMode15Zero);

// 0x4354a0. Sets the scene's weather effect intensity: 0 while 0x6d3b4c is set,
// the options' fixed level (+0x1e3, or +0x23f when that override and the view
// option allow it) times 32, else the weather event's envelope: fade in over
// [+0x51, +0x55], full until +0x59, fade out until +0x5d (then the next event is
// scheduled), scaled by its strength +0x61.
RT3_NOINLINE void __fastcall Weather_ApplyIntensity(uint8_t* w) {
    auto set = [](int32_t v) { Fx_SetIntensity((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, v); };
    if (hook::Global<int32_t>(0x6d3b4c)) {
        set(0);
        return;
    }
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    int32_t level;
    bool fixed = false;
    if (F<int32_t>(opt, 0x1df)) {
        level = F<int32_t>(opt, 0x1e3);
        fixed = true;
    } else if (F<int32_t>(opt, 0x23b) && hook::Global<uint8_t*>(0x6cec7c)[0x82]) {
        level = F<int32_t>(opt, 0x23f);
        fixed = true;
    }
    if (fixed) {
        int32_t v = level << 5;
        if (v > 0xff) v = 0xff;
        set(v);
        return;
    }
    int32_t t = F<int32_t>(w, 0x15), end = F<int32_t>(w, 0x5d);
    if (t > end) {
        set(0);
        Weather_ScheduleNext(w, 0, 0);
        return;
    }
    int32_t a = F<int32_t>(w, 0x51);
    if (!(t > a)) {
        set(0);
        return;
    }
    float f;
    if (t < F<int32_t>(w, 0x55)) {
        int32_t span = F<int32_t>(w, 0x55) - a;
        if (span < 1) span = 1;
        f = (float)((double)(float)(t - a) / (float)span);
    } else if (t < F<int32_t>(w, 0x59)) {
        f = 1.0f;
    } else {
        int32_t c = F<int32_t>(w, 0x59);
        int32_t span = end - c;
        if (span < 1) span = 1;
        f = (float)(D(0x5c8770) - (double)(float)(t - c) / (float)span);
    }
    float strength = (float)F<int32_t>(w, 0x61);
    set(Ftol((double)strength * f));
}
RT3_REPLACE(0x4354a0, Weather_ApplyIntensity);

RT3_NOINLINE int32_t Global6d0818Is25() { return F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c) == 0x19 ? 1 : 0; }
RT3_REPLACE(0x435610, Global6d0818Is25);

// 0x435630. Cleans up the 12 industry templates (+0x13d4, 0x4e1 bytes; five 0x30-byte
// recipe slots at +4: kind 1 = input only, 2 = output only, 3 = conversion, output
// name +8, input name +0x1c, rate +4): missing names get the first cargo, conversions
// get an output different from the input, low rates become 1 (the speed +0 becomes 4).
// Then rebuilds each template's recipe table (count -0x3b0, 0xbc-byte entries at
// -0x3ac) and the industries' port/warehouse lists.
RT3_NOINLINE void __fastcall Templates_Normalize(uint8_t* self) {
    const float lim = *(const float*)0x5c85b4;
    uint8_t* t = self + 0x13d4;
    for (int32_t k = 12; k; --k, t += 0x4e1) {
        if (F<float>(t, 0) < lim) F<uint32_t>(t, 0) = 0x40800000;
        int32_t count = 0;
        uint8_t* e = t + 4;
        for (int32_t j = 5; j; --j, e += 0x30) {
            int32_t kind = F<int32_t>(e, 0);
            if (kind) ++count;
            if (lim > F<float>(e, 4)) F<uint32_t>(e, 4) = 0x3f800000;
            if ((kind == 1 || kind == 3) && !e[0x1c]) FirstCargoName((char*)e + 0x1c, 0);
            kind = F<int32_t>(e, 0);
            if ((kind == 2 || kind == 3) && !e[8]) FirstCargoName((char*)e + 8, 0);
            if (F<int32_t>(e, 0) == 3) {
                int32_t i = 0;
                while (!Stricmp((const char*)e + 8, (const char*)e + 0x1c)) {
                    FirstCargoName((char*)e + 8, i++);
                    if (F<int32_t>(e, 0) != 3) break;
                }
            }
        }
        F<int32_t>(t, -0x3b0) = count;
        uint8_t* r = t - 0x3ac;
        e = t + 4;
        for (int32_t j = 5; j; --j, e += 0x30) {
            if (!F<int32_t>(e, 0)) continue;
            memset(r, 0, 0xbc);
            F<int32_t>(r, 0) = F<int32_t>(e, 0) == 1;
            F<uint16_t>(r, 0x20) = 0x708;
            F<uint16_t>(r, 0x22) = 0x270f;
            F<int32_t>(r, 0x24) = 0;
            F<int32_t>(r, 0x28) = 0;
            bool input = true;
            if (F<int32_t>(e, 0) != 1) {
                strcpy((char*)r + 8, (const char*)e + 8);
                F<uint32_t>(r, 0x1c) = Table_FindByName(hook::Global<Collection*>(0x62ba8c), 0, (const char*)e + 8);
                F<uint32_t>(r, 4) = F<uint32_t>(e, 4);
                input = F<int32_t>(e, 0) == 1 || F<int32_t>(e, 0) == 3;
            }
            if (input) {
                F<int32_t>(r, 0x2c) = 1;
                strcpy((char*)r + 0x30, (const char*)e + 0x1c);
                F<uint32_t>(r, 0x44) = Table_FindByName(hook::Global<Collection*>(0x62ba8c), 0, (const char*)e + 0x1c);
                F<uint32_t>(r, 0x48) = F<int32_t>(e, 0) == 3 ? 0x3f800000 : F<uint32_t>(e, 4);
            }
            r += 0xbc;
        }
    }
    if (Collection* inds = hook::Global<Collection*>(0x62b268)) Industries_RebuildPortsWarehouses(inds);
}
RT3_REPLACE(0x435630, Templates_Normalize);

// 0x4358d0. Pays the city connection bonuses: a city with a bonus pending (+0x276)
// and an active ready station pays it to the station's company once that station
// is also linked to another city ("%1 has collected a %2 bonus for connecting to
// %3!", string 0x154; company stat 4, bonus cleared, +0x302 set); until then it
// says once (+0x316) that the station must be connected to another city (0xb69).
RT3_NOINLINE void Cities_PayConnectionBonuses() {
    char money[0x1e0], text[0x200];
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
        Collection* cities = hook::Global<Collection*>(0x62bae0);
        uint8_t* city = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
        if (!F<int32_t>(city, 0x276) || !Region_HasConnectedStation(city, 0, 1, 1, 0, 0)) continue;
        int32_t linked = Region_HasConnectedStation(city, 0, 1, 1, 1, 0);
        uint8_t* site = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, Obj_FindRegionSite(city, 0, 1, 1));
        uint8_t* company = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)Obj_BuildingValue(site));
        const char* name = (const char*)city + 0x356;
        if (linked) {
            Num_FormatSimple(money, 9, (double)F<int32_t>(city, 0x276), 0xb);
            FormatText(text, GetString(0x154), (const char*)company + 4, money, name, 0, 0, 0, 0, 0);
            Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0xa, F<int32_t>(city, 0x23a), 0);
            Company_PostStat(company, 0, 4, (float)F<int32_t>(city, 0x276), 1);
            F<int32_t>(city, 0x276) = 0;
            F<int32_t>(city, 0x302) = 1;
        } else if (!city[0x316]) {
            city[0x316] = 1;
            Num_FormatSimple(money, 9, (double)F<int32_t>(city, 0x276), 0xb);
            FormatText(text, GetString(0xb69), (const char*)company + 4, name, money, 0, 0, 0, 0, 0);
            Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0xa, F<int32_t>(city, 0x23a), 0);
        }
    }
}
RT3_REPLACE(0x4358d0, Cities_PayConnectionBonuses);

 // namespace

// 0x435ac0 (event callback). Release of 0x3f9 follows the selected train
// (0x62be44): camera mode 0 on its city object and tool mode 2. Returns 0
// then, else 1.
int32_t __fastcall Train_FollowSelectedCb(void*, const uint32_t* ev) {
    if (ev[0] != 0xcb || ev[1] != 0x3f9) return 1;
    if (Collection_IsValid(hook::Global<Collection*>(0x6cfcbc), 0, hook::Global<uint32_t>(0x62be44))) {
        uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, hook::Global<uint32_t>(0x62be44));
        if (t && Obj_City41(t)) {
            Camera_SetMode(hook::Global<uint8_t*>(0x62be68), 0, 0);
            Camera_FollowObject((uint8_t*)Obj_City41(t), 0);
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 2, hook::Global<int32_t>(0x62be44));
        }
    }
    return 0;
}
RT3_REPLACE(0x435ac0, Train_FollowSelectedCb);

// 0x435b50. The yearly history events, once per new year (world year +0xd vs the
// last one +5; none with +0x46c38 set): newspaper headlines for steel bridges
// (1865), suspension bridges over water (1895), the automobile (1920) and the
// Boeing 707 (1958); bigger cargo cars in 1850 / 1900 / 1950 (10/20/30, 20/40/50,
// 40/80/70 tons, string 0x15f, logged on the world); cities refusing raw coal and
// oil (1910); and, where those industries exist, Tool and Die / Weapons /
// Munitions factories refusing iron (1877), toy factories wanting plastic (1956),
// munitions (1929) and weapons (1940) changing inputs (logged on `w`).
void __fastcall World_YearEvents(uint8_t* w) {
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    int32_t year = F<uint16_t>(world, 0xd);
    if (F<int32_t>(world, 0x46c38) || F<uint16_t>(world, 5) == year) return;
    auto headline = [](int32_t title, int32_t text) {
        const char* t2 = GetString(text);
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, GetString(title), t2, 0, 0);
    };
    switch (year) {
    case 1865: headline(0x157, 0x158); return;
    case 1895: headline(0x159, 0x15a); return;
    case 1920: headline(0x15b, 0x15c); return;
    case 1958: headline(0x15d, 0x15e); return;
    }
    auto entry = [](void* target, const char* text) { World_AddNamedEntry(target, 0, text, 0, 0, -1, -1, -1, -1); };
    if (year == 1850 || year == 1900 || year == 1950) {
        int32_t a = year == 1850 ? 10 : year == 1900 ? 20 : 40;
        int32_t b = year == 1850 ? 20 : year == 1900 ? 40 : 80;
        int32_t c = year == 1850 ? 30 : year == 1900 ? 50 : 70;
        char sc[0xcc], sb[0xcc], sa[0xcc], text[0x7d0];
        Num_Format(sc, 1, (double)c, 10, 1, 0, 0);
        Num_Format(sb, 1, (double)b, 10, 1, 0, 0);
        Num_Format(sa, 1, (double)a, 10, 1, 0, 0);
        FormatText(text, GetString(0x15f), sa, sb, sc, 0, 0, 0, 0, 0);
        entry(hook::Global<void*>(0x6cec78), text);
        return;
    }
    if (year == 1910) {
        entry(w, GetString(0x160));
        return;
    }
    if (year == 1877) {
        int32_t tools = Industries_Count((const char*)0x5c8eac);
        int32_t weapons = Industries_Count((const char*)0x5c8e3c);
        int32_t munitions = Industries_Count((const char*)0x5c9038);
        if (!tools && !weapons && !munitions) return;
        char list[0x400] = "";
        bool done = false;
        if (tools) {
            strcat(list, GetString(0x162));
            if (weapons && munitions) strcat(list, (const char*)0x5c9f38);
            else if (weapons || munitions) strcat(list, GetString(0x161));
            else done = true;
        }
        if (!done) {
            if (weapons) {
                strcat(list, GetString(0x163));
                if (munitions) strcat(list, GetString(0x161));
                else done = true;
            }
            if (!done && munitions) strcat(list, GetString(0x164));
        }
        char text[0x7d0];
        FormatText(text, GetString(0x165), list, 0, 0, 0, 0, 0, 0, 0);
        entry(w, text);
        return;
    }
    int32_t msg = 0;
    if (year == 1956 && Industries_Count((const char*)0x5c8e90)) msg = 0x166;
    else if (year == 1929 && Industries_Count((const char*)0x5c9038)) msg = 0x167;
    else if (year == 1940 && Industries_Count((const char*)0x5c8e3c)) msg = 0x168;
    if (msg) entry(w, GetString(msg));
}
RT3_REPLACE(0x435b50, World_YearEvents);

RT3_NOINLINE void __fastcall Nop436040(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x436040, Nop436040);

// 0x436050 (vtable stub).
RT3_NOINLINE void __fastcall Nop436050(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x436050, Nop436050);

// 0x436060 (vtable stub).
// The no-op asm keeps the optimiser from dropping calls to it (the tests
// fake it).
RT3_NOINLINE void __fastcall Nop436060(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x436060, Nop436060);

// 0x436070. Refreshes the open list view after company data changed: the
// company list (mode 7), the companies table (0xb) or the whole overview (9).
RT3_NOINLINE void __stdcall View_RefreshCompanyLists() {
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 7) CompanyList_Fill(hook::Global<uint8_t*>(0x6cfe70));
    View_RefreshCompaniesAndOverview();
}
RT3_REPLACE(0x436070, View_RefreshCompanyLists);

// 0x4360d0. As 0x436070 for company `id`, also closing its detail view
// (mode 8 showing it) by posting input event 0x3e8.
RT3_NOINLINE void __stdcall View_RefreshForCompany(int32_t id) {
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 7) CompanyList_Fill(hook::Global<uint8_t*>(0x6cfe70));
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 8 && F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90) == id) Game_PostInputEvent(0x3e8, 0, 0, 0, 0);
    View_RefreshCompaniesAndOverview();
}
RT3_REPLACE(0x4360d0, View_RefreshForCompany);

// 0x436170. A company's stations or trains changed: refreshes the company
// views (0x4360d0) and the open station picker (mode 4) or train picker (mode 1).
RT3_NOINLINE void __stdcall View_RefreshCompanyAssets(int32_t, int32_t company) {
    View_RefreshForCompany(company);
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 4 && hook::Global<uint8_t*>(0x6d1708)) StationPicker_Fill(hook::Global<uint8_t*>(0x6d1708));
    if (!hook::Global<uint8_t*>(0x6d0818)) return;
    if (View_Mode() == 1 && hook::Global<uint8_t*>(0x6d3b34)) TrainPicker_Fill(hook::Global<uint8_t*>(0x6d3b34), 0, 0xffffffff);
}
RT3_REPLACE(0x436170, View_RefreshCompanyAssets);

// 0x4361d0. Refreshes every track piece's colour and scenery tint (back to none),
// every 0x62b26c record's highlight, and calls virtual +0x54 on the records of
// 0x6ada80, 0x62bae0 (cities) and 0x6cea50.
RT3_NOINLINE void World_RefreshColors() {
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfca8)); ++i) {
        Collection* nodes = hook::Global<Collection*>(0x6cfca8);
        Track_UpdateColor((uint8_t*)Collection_Get(nodes, 0, Collection_IdAt(nodes, 0, i, 0)));
        nodes = hook::Global<Collection*>(0x6cfca8);
        ObjList_SetTint((uint8_t*)Collection_Get(nodes, 0, Collection_IdAt(nodes, 0, i, 0)), 0, 0, 0, 0);
    }
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b26c)); ++i) {
        Collection* t = hook::Global<Collection*>(0x62b26c);
        Obj_RefreshHighlight(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
    }
    Table_CallSlot54([]() { return hook::Global<Collection*>(0x6ada80); });
    Table_CallSlot54([]() { return hook::Global<Collection*>(0x62bae0); });
    Table_CallSlot54([]() { return hook::Global<Collection*>(0x6cea50); });
}
RT3_REPLACE(0x4361d0, World_RefreshColors);

 // namespace

// 0x436350. World w ends the scenario with result +0x4a73 (1..4; +0x4a77
// marks it ended): the result screen (Scenario_ShowResult, level 0..3).
// The last campaign scenario (15) instead rolls the credits (event 0xcc /
// 3). Otherwise a campaign (+0xc5) result asks whether to continue (0x169),
// a lone scenario first restores the volumes and shows the high scores
// (result - 2); declining ends the game: the session is closed and the menu
// opened (event 0xcc / 7 in a campaign, else 2) with the volumes restored.
RT3_NOINLINE void __fastcall Scenario_End(uint8_t* w) {
    const int32_t scenario = hook::Global<uint8_t*>(0x6cec7c)[0xc4];
    const int32_t result = F<int32_t>(w, 0x4a73);
    F<int32_t>(w, 0x4a77) = 1;
    if ((uint32_t)(result - 1) > 3) return;
    Scenario_ShowResult(result - 1);
    if (F<int32_t>(w, 0x4a73) != 1) {
        const uint8_t campaign = hook::Global<uint8_t*>(0x6cec7c)[0xc5];
        if (campaign && scenario >= 0xf) {
            hook::Global<int32_t>(0x6d4000) = 1;
            F<int32_t>(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec74), 0x14), 0x110) = 0;
            View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
            hook::Global<int32_t>(0x624308) = 0x1b;
            Game_PostInputEvent(0xcc, 0, 3, 0, 0);
            return;
        }
        if (!campaign) {
            Scenario_RestoreVolumes();
            HighScores_Show(hook::Global<uint8_t*>(0x6cec7c)[0x7d], F<int32_t>(w, 0x4a73) - 2, 0);
        }
    }
    if (Dialog_RunMessage(GetString(0x169), 3, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) == 0x3f2) return;
    View_SetToolMode(hook::Global<uint8_t*>(0x6d0818), 0, 0, -1);
    if (uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8)) {
        Session_Shutdown(s);
        ((void(__cdecl*)(void*))0x53b080)(s);
        hook::Global<uint8_t*>(0x6cd8d8) = nullptr;
    }
    Game_PostInputEvent(0xcc, 0, hook::Global<uint8_t*>(0x6cec7c)[0xc5] ? 7 : 2, 0, 0);
    Scenario_RestoreVolumes();
}
RT3_REPLACE(0x436350, Scenario_End);

// 0x436590. Product of the percentage modifiers for effect idx (world +0x8a, then
// company +0x35b, player +0x2ab (the company's own when none) and region +0x3b5),
// each at least -99%; at least 0x5c85b8.
RT3_NOINLINE float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region) {
    auto pct = [](int32_t v) { return (double)(v > -99 ? v : -99) / D(0x5c85d8) + D(0x5c8770); };
    float f = (float)pct(F<int32_t>(w, 0x8a + idx * 4));
    uint32_t pl = player;
    if (company && Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, company)) {
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, company);
        f = (float)(pct(F<int32_t>(c, 0x35b + idx * 4)) * f);
        if (!player) pl = F<uint32_t>(c, 0x3b);
    }
    if (pl && Collection_IsValid(hook::Global<Collection*>(0x6ceb9c), 0, pl)) {
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, pl);
        f = (float)(pct(F<int32_t>(p, 0x2ab + idx * 4)) * f);
    }
    if (region && Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, region)) {
        uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, region);
        f = (float)(pct(F<int32_t>(r, 0x3b5 + idx * 4)) * f);
    }
    if ((double)f < D(0x5c85b8)) *(uint32_t*)&f = 0x3c23d70a;
    return f;
}
RT3_REPLACE(0x436590, Effects_Multiplier);

// 0x436710. Cargo-i bonus: own value (+0x8a) plus the company's (+0x35b), the
// player's (given, or the company's owner; +0x2ab) and the region type's (+0x3b5).
RT3_NOINLINE int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region) {
    int32_t v = F<int32_t>(o, 0x8a + i * 4);
    uint32_t p = player;
    if (company && Collection_IsValid(g_companies, 0, company)) {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, company);
        v += F<int32_t>(c, 0x35b + i * 4);
        if (!p) p = F<uint32_t>(c, 0x3b);
    }
    if (p && Collection_IsValid(g_players, 0, p))
        v += F<int32_t>(Collection_Get(g_players, 0, p), 0x2ab + i * 4);
    if (region && Collection_IsValid(g_table6cfc9c, 0, region))
        v += F<int32_t>(Collection_Get(g_table6cfc9c, 0, region), 0x3b5 + i * 4);
    return v;
}
RT3_REPLACE(0x436710, Obj_CargoBonus);

// 0x4367c0. Sets the game-end mode (+0x4a73) and its message (string 0xb6b or 0xb6c).
RT3_NOINLINE void __fastcall World_SetEndMode(uint8_t* w, int, int32_t mode) {
    F<int32_t>(w, 0x4a73) = mode;
    F<int32_t>(w, 0x4d) = 0;
    F<int32_t>(w, 0x4c88) = F<int32_t>(w, 0x15);
    const char* s = GetString(mode == 1 ? 0xb6b : 0xb6c);
    char* d = (char*)w + 0x4b47;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x4367c0, World_SetEndMode);

// 0x436820 (thiscall, this unused; train id, message). Opens train id in the
// consist editor on a copy of its orders (current entry and flag included;
// the copy is never freed), with its engine type, +0xd8, +0x1a1, the +0x1ce
// bit 0, its index and its name. With a message: in the tutorial-like mode
// (world +0x4aa3) only the message is shown (type 1); otherwise string 0x156
// is appended and the editor opens only on button 0x3f2 (type 0xc). If the
// train still exists and is of a special kind (0x4a7220), +0x1ce bit 1 is set.
RT3_NOINLINE void __fastcall Train_OpenConsistEditor(void* self, int, uint32_t trainId, const char* message) {
    uint8_t* train = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, trainId);
    if (!train) return;
    char text[0x258], name[0x3e8];
    if (message) strcpy(text, message);
    uint8_t orders[0x28];
    Cursor33_Init(orders);
    OrderList_Init(orders);
    for (int32_t i = 0; i < F<int32_t>(Identity(train), 4); ++i) TrainOrders_Insert(orders, 0, i, (uint8_t*)Array33_At(train, 0, i));
    F<int32_t>(orders, 8) = F<int32_t>(Identity(train), 8);
    orders[0xc] = ((uint8_t*)Identity(train))[0xc];
    int32_t engine = Train_FirstCarType126(train);
    uint32_t d8 = F<uint32_t>(train, 0xd8), b1a1 = train[0x1a1], b1ce = train[0x1ce] & 1;
    uint32_t key = Collection_GetKey1(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(train, 0x25));
    uint32_t id = F<uint32_t>(train, 0x25);
    strcpy(name, Train_DisplayName(train, 0, 0));
    if (message) {
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aa3)) {
            Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0x435ac0, (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x433f20, 0, 0.0f, 0, 0x1e,
                              0, nullptr, nullptr, 0, 0, nullptr, 0);
            return;
        }
        strcat(text, GetString(0x156));
        if (Dialog_RunMessage(text, 0xc, 0xbf800000, 0xbf800000, 0x435ac0, (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x433f20, 0, 0.0f, 0,
                              0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) != 0x3f2)
            return;
    }
    ConsistEditor_Run(nullptr, nullptr, orders, (uint32_t)engine, d8, b1a1, b1ce, (uint32_t)(uintptr_t)name, key);
    if (Collection_IsValid(hook::Global<Collection*>(0x6cfcbc), 0, id) &&
        ((int32_t(__fastcall*)(void*))0x4a7220)(Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, id)))
        train[0x1ce] |= 2;
}
RT3_REPLACE(0x436820, Train_OpenConsistEditor);

// 0x436a70. Marks available in world w the types of the buildings (0x62b26c)
// that are flagged (+0x3d4 == 1) and whose type is kind 2 (+0x32) with neither
// +0xba nor +0xbb set.
RT3_NOINLINE void __fastcall World_MarkBuildingTypes(uint8_t* w) {
    Collection* c = hook::Global<Collection*>(0x62b26c);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        c = hook::Global<Collection*>(0x62b26c);
        uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (Obj_Flag3d4(b) == 1) {
            uint8_t* type = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
            if (type[0x32] == 2 && !type[0xba] && !type[0xbb]) World_SetNamedValue(w, 0, (const char*)type + 4, 1);
        }
        c = hook::Global<Collection*>(0x62b26c);
    }
}
RT3_REPLACE(0x436a70, World_MarkBuildingTypes);

// 0x436af0. Sets the speed scale (+0x4cbe) from the top speed of the engine types
// the player can buy (at least 100, 200 after 2000, 300 after 2010): k / speed,
// clamped to 50..100.
RT3_NOINLINE void __fastcall World_UpdateSpeedScale(uint8_t* o) {
    uint16_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    float v = 100.0f;
    if (year > 0x7d0) v = 200.0f;
    if (year > 0x7da) v = 300.0f;
    if (hook::Global<Collection*>(0x6ada84)) {
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) {
            Collection* c = hook::Global<Collection*>(0x6ada84);
            uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            uint8_t* ct = F<uint32_t>(e, 0x72)
                ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72)) : nullptr;
            bool ok = CarType_Available(ct, 0, 0, 0) != 0;
            if (!hook::Global<int32_t>(0x6cd8d8)) {
                uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
                if (CarType_Available(ct, 0, F<uint32_t>(w, 0x21), F<uint32_t>(w, 0x25))) ok = true;
            }
            if (ok && !(v > F<float>(e, 0x20))) v = F<float>(e, 0x20);
        }
    }
    if (Fl(0x5c85b4) > v) v = 1.0f;
    v = (float)(D(0x5c9f40) / v * D(0x5c85d8));
    if (Fl(0x5c8780) > v) v = 50.0f;
    if (Fl(0x5c8c28) < v) v = 100.0f;
    F<float>(o, 0x4cbe) = v;
}
RT3_REPLACE(0x436af0, World_UpdateSpeedScale);

// 0x436c70. Recognises one specific scenario (map 384x448, 65 regions with
// region 1 named 0x5c9f48, 22 open slots and slot 7 kind 6) when v == 16.
RT3_NOINLINE int32_t __fastcall World_IsSpecialScenario(void*, int, int32_t v) {
    if (v != 0x10) return 0;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (F<int32_t>(map, 0x2155) != 0x180 || F<int32_t>(map, 0x2159) != 0x1c0) return 0;
    Collection* regions = hook::Global<Collection*>(0x62bae0);
    if (Collection_Count(regions) != 0x41) return 0;
    if (Stricmp((const char*)Collection_Get(regions, 0, 1) + 0x356, (const char*)0x5c9f48) != 0) return 0;
    Collection* slots = hook::Global<Collection*>(0x62be18);
    if (Table_CountClear82b(slots) != 0x16) return 0;
    return F<uint8_t>(Collection_RecordAtList(slots, 0, 7, 0), 0x7ef) == 6 ? 1 : 0;
}
RT3_REPLACE(0x436c70, World_IsSpecialScenario);

// 0x436d10. Resets world w for a new game: clears its tables and counters,
// sets the default start (1830..1870, year limit 2000), the name string 0x136,
// sixteen 100000 limits, a random 0..39 seed (+0x41), difficulty 2, the twelve
// month labels ("%1" of 1..12, 0x4e1 apart from +0xfe7), the default scale
// factors and two fresh record lists (+0x66b2 / +0x66b6). With setup (when
// starting a tutorial or scenario of kind 1 / 7 from the setup 0x6cec7c) that
// file's options are applied. The start year (setup +0x77, keeping +0x66d2 when
// asked) sets the date; the clock starts a year (or three in mode 2) early
// outside free play, and the era, weather, templates and cargo lists follow.
RT3_NOINLINE void __fastcall World_Reset(uint8_t* w, int, int32_t setup) {
    memset(w + 0x4cc2, 0, 0x26d);
    memset(w + 0x66be, 0, 0x403c2);
    w[0x66be] = 1;
    F<int32_t>(w, 0x66ca) = 0x726;
    F<int32_t>(w, 0x66d2) = 0x74e;
    F<int32_t>(w, 0x66ce) = 0x7d0;
    w[0x69db] = 1;
    F<int32_t>(w, 0x6a74) = 1;
    F<int32_t>(w, 0x6a70) = 1;
    F<int32_t>(w, 0x6a68) = 1;
    F<int32_t>(w, 0x6a6c) = 1;
    strcpy((char*)w + 0x672e, GetString(0x136));
    for (int32_t i = 0; i < 0x10; ++i) F<int32_t>(w, 0x69dc + i * 9) = 0x186a0;
    F<int32_t>(w, 0xbfa) = 0;
    F<int32_t>(w, 0x25) = 0;
    F<int32_t>(w, 0x21) = 0;
    F<int32_t>(w, 0x45) = 0;
    F<int32_t>(w, 0x49) = 0;
    F<int32_t>(w, 0x46c38) = 0;
    F<int32_t>(w, 0x46c3c) = 0;
    F<int32_t>(w, 0x19) = 0;
    F<int32_t>(w, 0x41) = SimRand(0, 0x27);
    F<int32_t>(w, 0x31) = 0;
    F<int32_t>(w, 0x3d) = 0;
    F<uint32_t>(w, 0x29) = 0x3da3d70a;
    F<int32_t>(w, 0x2d) = 0;
    World_SetDifficulty(w, 0, 2);
    F<int32_t>(w, 0x4d) = 0;
    F<int32_t>(w, 0x66ba) = 0;
    w[0x89] = 0;
    F<int32_t>(w, 0x4cb6) = 0;
    F<int32_t>(w, 0x4cba) = 0;
    F<int32_t>(w, 0x4a73) = 0;
    F<int32_t>(w, 0x4a77) = 0;
    F<int32_t>(w, 0x4a7b) = 0;
    w[0x4f30] = 0;
    w[0x5ae9] = 0;
    F<int32_t>(w, 0x4c74) = 0;
    F<int32_t>(w, 0x4c88) = 0;
    for (uint32_t o = 0x4c8c; o <= 0x4c92; ++o) w[o] = 0;
    F<int32_t>(w, 0x4c93) = 0;
    F<int32_t>(w, 0x4ca6) = 0;
    F<int32_t>(w, 0x4caa) = 0;
    w[0x4c97] = 0;
    w[0x4c98] = 0;
    w[0x4c99] = 0;
    F<int32_t>(w, 0x4c9a) = 0;
    F<int32_t>(w, 0x4c9e) = 0;
    w[0x4b47] = 0;
    F<int32_t>(w, 0xbce) = 0;
    F<int32_t>(w, 0xbd2) = 0;
    F<int32_t>(w, 0xbd6) = 0;
    F<int32_t>(w, 0xbda) = 0;
    for (int32_t i = 0; i < 0x35; ++i) {
        F<int32_t>(w, 0xa26 + i * 4) = 0;
        F<int32_t>(w, 0xafa + i * 4) = 0;
    }
    memset(w + 0xfe7, 0, 0xea3 * 4);
    char* label = (char*)w + 0xfe7;
    for (int32_t i = 1; i <= 0xc; ++i, label += 0x4e1) {
        char num[0x10];
        FormatText(label, (const char*)0x5c9f78, Num_Format(num, 1, (double)i, 0xa, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
    }
    F<int32_t>(w, 0xa22) = 1;
    memset(w + 0x8a, 0, 0x266 * 4);
    memset(w + 0x4a7f, 0, 0x31 * 4);
    F<int32_t>(w, 0x4c78) = 1;
    F<int32_t>(w, 0x4c7c) = 1;
    F<int32_t>(w, 0x4b43) = 0;
    F<uint32_t>(w, 0xbe2) = 0x3f800000;
    F<uint32_t>(w, 0xbde) = 0x3f800000;
    F<uint32_t>(w, 0xbe6) = 0x3c75c28f;
    F<uint32_t>(w, 0xbea) = 0x3c75c28f;
    F<uint32_t>(w, 0xbee) = 0x3c75c28f;
    F<uint32_t>(w, 0xbf2) = 0x3c75c28f;
    F<uint32_t>(w, 0xbf6) = 0x3ba3d70a;
    for (uint32_t off : { 0x66b2u, 0x66b6u }) {
        if (Collection* old = F<Collection*>(w, off)) {
            Collection_Destruct(old);
            ((void(__cdecl*)(void*))0x53b080)(old);
        }
        Collection* c = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
        F<Collection*>(w, off) = c ? Collection_Construct(c, 0, 1, off == 0x66b2u ? 0x22 : 0x41, 2, 2, 0, 0, 0) : nullptr;
    }
    if (uint8_t* view = hook::Global<uint8_t*>(0x6d0818))
        Window_SendEvent5(view, 0, 0x74, (uint32_t)(F<int32_t>(w, 0x19) + 0x80c), 1, 0, 0);
    F<int32_t>(w, 0x1d) = F<int32_t>(w, 0x19) < 1 ? 1 : F<int32_t>(w, 0x19);
    F<int32_t>(w, 0x66a2) = 0;
    F<int32_t>(w, 0x66a6) = 0;
    F<int32_t>(w, 0x66aa) = 0;
    int32_t year = 0x74f;
    if (uint8_t* st = hook::Global<uint8_t*>(0x6cec7c)) {
        if (setup && (F<int32_t>(st, 1) == 1 || F<int32_t>(st, 1) == 7)) {
            F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x6c) = 2;
            uint8_t block[0x403c4];
            Block_Load(F<int32_t>(hook::Global<uint8_t*>(0x6cec7c), 1) == 1 ? (const char*)0x5c9f68 : (const char*)0x5c9f58, block, 1);
            Obj_ApplySetup(hook::Global<uint8_t*>(0x6cec7c), 0, block);
            st = hook::Global<uint8_t*>(0x6cec7c);
        }
        if (st[0xc5] && !st[0x82]) {
            F<uint16_t>(st, 0x77) = F<uint16_t>(w, 0x66d2);
            st = hook::Global<uint8_t*>(0x6cec7c);
        }
        year = F<uint16_t>(st, 0x77);
    }
    uint64_t date = MakeDate((uint16_t)year, 1, 1, 0, 0);
    F<uint64_t>(w, 5) = date;
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    if (!F<int32_t>(opt, 0x68)) {
        int32_t mode = F<int32_t>(opt, 0x178);
        if (mode == 1) --year;
        else if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4af7)) --year;
        else if (mode == 2) year -= 3;
    }
    World_SetTime(w, 0, MinutesFromParts(year, 1, 1, 0, 0));
    Economy_UpdateEraFactors(w);
    Weather_ScheduleNext(w, 0, 0);
    F<uint32_t>(w, 0x39) = F<uint32_t>(w, 0x11);
    F<uint32_t>(w, 0x35) = F<uint32_t>(w, 0xd);
    Templates_Normalize(w);
    if (Collection* c = hook::Global<Collection*>(0x62ba8c)) CargoTypes_Recount(c);
    if (Collection* c = hook::Global<Collection*>(0x62b268)) Industries_BuildCargoLists(c);
    ((void(__fastcall*)(uint8_t*))&World_UpdateEraFactor)(w);
    World_UpdateSpeedScale(w);
    if (uint8_t* dev = hook::Global<uint8_t*>(0x6d4024)) {
        F<int32_t>(dev, 0x11425e) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11425a) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114262) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114266) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11426a) = 0;
        F<int32_t>(w, 0x4cae) = 0;
        F<int32_t>(w, 0x4cb2) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11426e) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114276) = 0;
    }
}
RT3_REPLACE(0x436d10, World_Reset);

// 0x437220. World w sets up its 16 player slots from the scenario slots
// (+0x69d8, 9 bytes each: personality, two in-use flags at +2 / +3, cash at
// +4) and the lobby picks (0x6cec7c+0x87 per slot: 0 free choice, 1..99
// human from slot n-1, 100.. AI from slot n-100). In multiplayer it first
// announces itself (0x65) and waits - a client for its slot (session
// +0x9860, 0xe1e), the host for every player (0xe1f) - pumping the loading
// screen. Unused slots are switched off. A free slot draws an unused
// personality (Rand 0..40 within 0x6cec78+0x6987, 2500 tries; the scenario's
// own in the editor, +0xc5); the personality table (0x5f2d29, 0x27 bytes)
// gives the name, description and kind for Player_Init, unless a
// multiplayer client (0x6ce9bc) leaves it to the host; the cash comes from
// the slot. The host tells other humans their slot (Net_SendSlot); a refused
// one becomes AI (+0x293). Finally the local player (+0x25) and their
// company (+0x21) are set.
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w) {
    Obj_AssignRandomSlots(w + 0x66be);
    uint8_t taken[0x29] = {};
    for (int32_t k = 0; k < 0x10; ++k) {
        const uint8_t* s = w + 0x69d8 + k * 9;
        int32_t pi = (int8_t)s[0];
        if ((s[2] || s[3]) && pi >= 0 && pi < 0x29) taken[pi] = 1;
    }
    int32_t local = 0, nextSlot = 1;
    if (uint8_t* sess = hook::Global<uint8_t*>(0x6cd8d8)) {
        Net_Send(sess, 0, nullptr, 0x65, 0, nullptr, 1, 0);
        bool host = hook::Global<int32_t>(0x6d40dc) != 0;
        if (!host) {
            while (F<int32_t>(hook::Global<uint8_t*>(0x6cd8d8), 0x9860) == -1) {
                Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 0, 0);
                Session_Update(hook::Global<uint8_t*>(0x6cd8d8));
                if (hook::Global<int32_t>(0x6d40dc)) {
                    host = true;
                    break;
                }
                Window_SendEvent5(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec74), 0xc), 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(0xe1e), 0, 0);
            }
            if (!host) local = F<int32_t>(hook::Global<uint8_t*>(0x6cd8d8), 0x9860);
        }
        if (host) {
            while (!Net_AllPlayersFlag4()) {
                Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 0, 0);
                Session_Update(hook::Global<uint8_t*>(0x6cd8d8));
                Window_SendEvent5(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec74), 0xc), 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(0xe1f), 0, 0);
            }
        }
    }
    for (int32_t k = 0; k < 0x10; ++k) {
        uint8_t* slot = w + 0x69da + k * 9;
        if (!slot[0] && !slot[1]) continue;
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(k + 1));
        if (!p) slot[1] = 0, slot[0] = 0;
        uint8_t* lobby = hook::Global<uint8_t*>(0x6cec7c);
        if (!lobby[0x87 + k] && !lobby[0xc5]) {
            F<int32_t>(p, 4) = 0;
            if (hook::Global<uint8_t*>(0x6cd8d8)) p[0x293] = 0;
            continue;
        }
        F<int32_t>(p, 4) = 1;
        uint8_t pick = hook::Global<uint8_t*>(0x6cec7c)[0x87 + k];
        uint8_t human = pick < 100 ? 1 : 0;
        int32_t src, pi;
        if (!pick) {
            pi = 1;
            for (int32_t tries = 0; tries < 0x9c4; ++tries) {
                int32_t r = Rand(0, 0x28);
                if (hook::Global<uint8_t*>(0x6cec78)[0x6987 + r] && !taken[r]) {
                    pi = r;
                    break;
                }
            }
            if (hook::Global<uint8_t*>(0x6cec7c)[0xc5]) {
                int8_t own = (int8_t)slot[-2];
                if (own >= 0 && own < 0x29) pi = own;
            }
            human = k == 0 ? 1 : 0;
            src = k;
        } else {
            src = pick < 100 ? pick - 1 : pick - 100;
            pi = (int8_t)w[0x69d8 + src * 9];
        }
        if (pi < 0 || pi >= 0x29) pi = 1;
        taken[pi] = 1;
        if (!hook::Global<uint8_t*>(0x6cd8d8) || !hook::Global<int32_t>(0x6ce9bc)) {
            int32_t keep = F<int32_t>(p, 0x1dd);
            int32_t clear = hook::Global<uint8_t*>(0x6cec7c)[0xc5] || hook::Global<uint8_t*>(0x6cd8d8) ? 1 : 0;
            const uint8_t* per = (const uint8_t*)0x5f2d29 + pi * 0x27;
            const char* desc = GetString(F<int32_t>((void*)per, 0x22));
            const char* name = GetString(F<int32_t>((void*)per, 0x1e));
            Player_Init(p, 0, k + 1, per[0], name, desc, pi, clear);
            F<int32_t>(p, 0x1dd) = keep;
            F<int32_t>(p, 0x1e1) = human;
            F<double>(p, 0x154) = (double)F<uint32_t>(w, 0x69dc + src * 9);
        }
        if (k && human && hook::Global<uint8_t*>(0x6cd8d8) && hook::Global<int32_t>(0x6d40dc)) {
            int32_t ok = Net_SendSlot(k, nextSlot++);
            if (!ok) {
                F<int32_t>(p, 0x1e1) = 0;
                p[0x293] = 1;
            }
        }
    }
    Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 1, 0);
    if (!hook::Global<uint8_t*>(0x6cd8d8) || !hook::Global<int32_t>(0x6ce9bc)) {
        F<int32_t>(w, 0x25) = 0;
        F<int32_t>(w, 0x21) = 0;
    }
    Collection* players = hook::Global<Collection*>(0x6ceb9c);
    F<uint32_t>(w, 0x25) = Collection_IdAt(players, 0, (uint32_t)local, 0);
    F<int32_t>(w, 0x21) = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(w, 0x25)), 0x1dd);
    if (hook::Global<uint8_t*>(0x6cd8d8) && hook::Global<int32_t>(0x6ce9bc)) {
        uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)local, 0);
        F<int32_t>(w, 0x21) = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, id), 0x1dd);
    }
}
RT3_REPLACE(0x437220, World_SetupPlayers);

// 0x437740. Marks every kind-2 building type (0x62b268, +0x32) available in the
// world 0x6cec78.
RT3_NOINLINE void World_MarkKind2Types() {
    Collection* c = hook::Global<Collection*>(0x62b268);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        c = hook::Global<Collection*>(0x62b268);
        uint8_t* type = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (type[0x32] == 2) World_SetNamedValue(hook::Global<uint8_t*>(0x6cec78), 0, (const char*)type + 4, 1);
        c = hook::Global<Collection*>(0x62b268);
    }
}
RT3_REPLACE(0x437740, World_MarkKind2Types);

// 0x4377a0. Single-player version of World_SetupPlayers: world w creates a
// player for every lobby pick (0x6cec7c+0x87: 1..99 human from scenario slot
// n-1, 100.. AI from slot n-100). In the editor (0x6cec74 +0x68) the used
// scenario slots (+0x69d8, 9 bytes, in use if +2 or +3) are first moved to
// the front and become the picks: slot 0 human, the rest AI. Each player is
// added to 0x6ceb9c and set up from its personality (table 0x5f2d29, 0x27
// bytes) with the slot's cash; then the local player (+0x25) is the first
// one and +0x21 its company. Outside the editor with auto companies (+0x82)
// or a scenario that wants them (0x6cec74 +0x14c) the companies are created
// (0x47d440, kept original; it reads stack residue, which this frame changes
// as any rewritten callee already does). Does nothing in multiplayer.
RT3_NOINLINE void __fastcall World_CreatePlayers(uint8_t* w) {
    Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 1, 0);
    if (hook::Global<uint8_t*>(0x6cd8d8)) return;
    auto inUse = [w](int32_t k) { return w[0x69da + k * 9] || w[0x69db + k * 9]; };
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68)) {
        for (int32_t i = 0; i < 0x10; ++i)
            for (int32_t j = i + 1; j < 0x10; ++j)
                if (inUse(j) && !inUse(i)) {
                    uint8_t tmp[9];
                    memcpy(tmp, w + 0x69d8 + i * 9, 9);
                    memcpy(w + 0x69d8 + i * 9, w + 0x69d8 + j * 9, 9);
                    memcpy(w + 0x69d8 + j * 9, tmp, 9);
                }
        for (int32_t i = 0; i < 0x10; ++i)
            hook::Global<uint8_t*>(0x6cec7c)[0x87 + i] = !inUse(i) ? 0 : i ? (uint8_t)(i + 100) : 1;
    }
    for (int32_t i = 0; i < 0x10; ++i) {
        if (!hook::Global<uint8_t*>(0x6cec7c)[0x87 + i]) continue;
        Collection* players = hook::Global<Collection*>(0x6ceb9c);
        uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)players)[1])(players, 0, nullptr, 0, 0, 0);
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, id);
        uint8_t pick = hook::Global<uint8_t*>(0x6cec7c)[0x87 + i];
        const uint8_t* slot = w + 0x69d8 + (pick < 100 ? pick - 1 : pick - 100) * 9;
        int32_t pi = (int8_t)slot[0];
        const uint8_t* per = (const uint8_t*)0x5f2d29 + pi * 0x27;
        const char* desc = GetString(F<int32_t>((void*)per, 0x22));
        const char* name = GetString(F<int32_t>((void*)per, 0x1e));
        Player_Init(p, 0, (int32_t)id, per[0], name, desc, pi, 1);
        F<int32_t>(p, 0x1e1) = pick < 100 ? 1 : 0;
        F<double>(p, 0x154) = (double)F<uint32_t>((void*)slot, 4);
    }
    F<int32_t>(w, 0x25) = 0;
    F<int32_t>(w, 0x21) = 0;
    Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 1, 0);
    F<uint32_t>(w, 0x25) = Collection_IdAt(hook::Global<Collection*>(0x6ceb9c), 0, 0, 0);
    F<int32_t>(w, 0x21) = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(w, 0x25)), 0x1dd);
    if (!hook::Global<uint8_t*>(0x6cd8d8) &&
        (hook::Global<uint8_t*>(0x6cec7c)[0x82] ||
         (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x14c) && !F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68))))
        hook::Orig<void (*)()>(0x47d440)();
}
RT3_REPLACE(0x4377a0, World_CreatePlayers);

// 0x437a60. Pauses a running game or resumes at the last speed (+0x1d), as a
// request.
RT3_NOINLINE int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce) {
    if (F<int32_t>(w, 0x19) > 0) return World_SetSpeed(w, 0, 0, 1, announce);
    return World_SetSpeed(w, 0, F<int32_t>(w, 0x1d), 1, announce);
}
RT3_REPLACE(0x437a60, World_TogglePause);

// 0x437a90. World w: follows entity `id` of `kind` (1 vehicles 0x62ba84, 2
// 0x62b26c, 3 0x62b244) with World_SetFollow when it exists, then
// remembers the choice (+0x45 / +0x49).
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id) {
    uint32_t table = kind == 1 ? 0x62ba84 : kind == 2 ? 0x62b26c : kind == 3 ? 0x62b244 : 0;
    if (table && Collection_IsValid(hook::Global<Collection*>(table), 0, (uint32_t)id))
        if (uint8_t* o = (uint8_t*)Collection_Get(hook::Global<Collection*>(table), 0, (uint32_t)id)) World_SetFollow(w, 0, on, o);
    F<int32_t>(w, 0x45) = kind;
    F<int32_t>(w, 0x49) = id;
}
RT3_REPLACE(0x437a90, World_FollowEntity);

// 0x437b20. World w runs ahead while loading (busy flag +0x46c38 set): 0x150
// simulation steps of 0x5a0 (three times that unless the options' +0x178 is
// 1; just 0x150 with 0x6cec78+0x4af7), the loading bar moving 80 steps over
// them; then every industry recounts its buildings and kind-2 buildings are
// marked.
RT3_NOINLINE void __fastcall World_RunAhead(uint8_t* w) {
    int32_t base = hook::Global<int32_t>(0xccb9e8);
    F<int32_t>(w, 0x46c38) = 1;
    int32_t n = (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x178) != 1 ? 3 : 1) * 0x150;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4af7)) n = 0x150;
    for (int32_t k = 1; k <= n; ++k) {
        ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x40ab50)(w, 0, 0x5a0, 0);
        hook::Global<int32_t>(0xccb9e8) = k * 0x50 / n + base;
        Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 1, 0);
    }
    F<int32_t>(w, 0x46c38) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
        Collection* t = hook::Global<Collection*>(0x62bae0);
        Obj_CountBuildings((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0)));
    }
    MarkKind2Buildings();
}
RT3_REPLACE(0x437b20, World_RunAhead);

// 0x437c00. World w opens event item e (text +4, kind +8, ids +0x10 / +0x14):
// 0 / 4 show the text, 3 prompts to rename train +0x10, 5 opens company
// report +0x10, 6 opens train +0x10 (remembered in 0x62be44) in the consist
// editor with the text, or only shows the text when the train is gone, 7
// opens info dialog +0x10, 8 building info +0x14 of +0x10. Returns 0 after
// kinds 3 and 6 (the item is used up), else 1.
RT3_NOINLINE int32_t __fastcall World_OpenEventItem(uint8_t* w, int, uint8_t* e) {
    auto show = [&] {
        Dialog_RunMessage(F<const char*>(e, 4), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    switch (F<uint32_t>(e, 8)) {
    case 0:
    case 4:
        show();
        return 1;
    case 3:
        Train_PromptRename(F<uint32_t>(e, 0x10), F<uint32_t>(e, 0x14));
        return 0;
    case 5:
        CompanyReport_Show(F<uint32_t>(e, 0x10), 1, 0);
        return 1;
    case 6:
        hook::Global<uint32_t>(0x62be44) = F<uint32_t>(e, 0x10);
        if (!Collection_IsValid(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(e, 0x10)) ||
            !Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(e, 0x10))) {
            show();
            return 0;
        }
        Train_OpenConsistEditor(w, 0, hook::Global<uint32_t>(0x62be44), F<const char*>(e, 4));
        return 0;
    case 7:
        Obj_ShowMessageDialog(F<uint32_t>(e, 0x10));
        return 1;
    case 8:
        BuildingInfo_Show(F<uint32_t>(e, 0x10), F<uint32_t>(e, 0x14));
        return 1;
    }
    return 1;
}
RT3_REPLACE(0x437c00, World_OpenEventItem);

// 0x437d20. Combined multiplier for cargo idx: the global cargo effect (0x69) times
// the cargo's own effect (0x6a + idx, when in range).
RT3_NOINLINE double __stdcall Effects_CargoMultiplier(int32_t idx) {
    float base = Effects_Multiplier(World(), 0, 0x69, 0, 0, 0);
    int32_t id = idx + 0x6a;
    if (id < 0x6a || id > 0x15f) return base;
    return (double)Effects_Multiplier(World(), 0, id, 0, 0, 0) * base;
}
RT3_REPLACE(0x437d20, Effects_CargoMultiplier);

// 0x437d70. World w's cheat prompt (with at most one human player): asks
// for a code (0xb6a) and runs the first match in the table at 0x5ee2c8 (20
// bytes each: string id or 0, literal, kind, value, engine name). Kinds:
// 0 / 1 end the game (mode value + 1), 2 adds value to the player company's
// cash (stat 4), 3 to the player's own, 4 gives every player train the named
// engine (Katie, Danny, ... and two string ids), 5 opens every territory to
// the player company (refresh 0x948c), 6 / 8 / 9 / 10 set the world flags
// +0x4c8f / +0x4c8e / +0x4c8d / +0x4c8c, 7 crashes every train.
RT3_NOINLINE void __fastcall World_CheatPrompt(uint8_t* w) {
    if (CountHumanPlayers() > 1) return;
    char code[0x12c];
    code[0] = 0;
    Obj_Incc60(hook::Global<uint8_t*>(0x6d401c));
    Dialog_RunMessage(GetString(0xb6a), 0xb, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)code, 100, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    auto Stricmp = (int(__cdecl*)(const char*, const char*))0x5a57cf;
    for (int32_t i = 0; i < 0x1a; ++i) {
        const int32_t* e = (const int32_t*)(0x5ee2c8 + i * 0x14);
        const char* text = e[0] ? GetString(e[0]) : (const char*)(uintptr_t)e[1];
        if (Stricmp(code, text)) continue;
        uint32_t kind = (uint32_t)e[2];
        int32_t value = e[3];
        uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
        switch (kind) {
        case 0: case 1:
            World_SetEndMode(w, 0, value + 1);
            break;
        case 2:
            if (F<int32_t>(world, 0x21) > 0 && Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(world, 0x21)))
                Company_PostStat(Obj_Company21(hook::Global<void*>(0x6cec78)), 0, 4, (float)value, 1);
            break;
        case 3:
            if (F<int32_t>(world, 0x25) > 0 && Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(world, 0x25)))
                Obj_AddDouble154(Obj_Ref25(hook::Global<void*>(0x6cec78)), 0, value);
            break;
        case 4: {
            uint32_t engine = Table_FindByAltName(hook::Global<Collection*>(0x6ada84), 0, (const char*)(uintptr_t)e[4]);
            if (!engine) break;
            for (int32_t k = Collection_Count(hook::Global<Collection*>(0x6cfcbc)) - 1; k >= 0; --k) {
                Collection* ts = hook::Global<Collection*>(0x6cfcbc);
                uint8_t* t = (uint8_t*)Collection_Get(ts, 0, Collection_IdAt(ts, 0, (uint32_t)k, 0));
                if (F<int32_t>(t, 0x51) == F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x21))
                    ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x4aefb0)(t, 0, engine, 1);
            }
            break;
        }
        case 5:
            if (F<int32_t>(world, 0x21) > 0 && Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(world, 0x21))) {
                for (int32_t k = 0; k < Collection_Count(hook::Global<Collection*>(0x6cfc9c)); ++k) {
                    int32_t id = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x21);
                    void* co = id > 0 ? Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)id) : nullptr;
                    Company_SetEntryFlag(co, 0, (int32_t)Collection_IdAt(hook::Global<Collection*>(0x6cfc9c), 0, (uint32_t)k, 0), 1);
                }
                if (uint8_t* win = hook::Global<uint8_t*>(0x6cfe4c)) Window_SendEvent5(win, 0, 0xba, 0x948c, 0, 0, 0);
            }
            break;
        case 6:
            w[0x4c8f] = 1;
            break;
        case 7:
            for (int32_t k = Collection_Count(hook::Global<Collection*>(0x6cfcbc)) - 1; k >= 0; --k) {
                Collection* ts = hook::Global<Collection*>(0x6cfcbc);
                Train_Crash((uint8_t*)Collection_Get(ts, 0, Collection_IdAt(ts, 0, (uint32_t)k, 0)), 0, 1);
            }
            break;
        case 8:
            w[0x4c8e] = 1;
            break;
        case 9:
            w[0x4c8d] = 1;
            break;
        case 10:
            w[0x4c8c] = 1;
            break;
        }
        break;
    }
    Obj_Decc60(hook::Global<uint8_t*>(0x6d401c));
}
RT3_REPLACE(0x437d70, World_CheatPrompt);

}  // namespace rt3
