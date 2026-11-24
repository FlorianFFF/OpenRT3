// Stations.cpp - RT3_mp.exe 0x47c000-0x482000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <initializer_list>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "hook/hook.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "game/calendar.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/containers.h"
#include "core/ptrlist.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static Collection*& g_buildings = hook::Global<Collection*>(0x62b26c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint8_t* __fastcall Obj_Region(void* o);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
static Collection*& g_table62bae0 = hook::Global<Collection*>(0x62bae0);
void __fastcall Obj_RefreshHighlight(void* o);
void __fastcall Nop47d8d0(void*);
void __fastcall Pairs_RemoveKey(uint8_t* o, int, int32_t key);
int32_t __fastcall Obj_BuildingStateActive(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall Site_SaveExtra(uint8_t* o, int, Stream s);
int32_t __fastcall Obj_StateB9(void* o);
void __fastcall Station_RecordNearby(uint8_t* s, int, uint8_t* obj);
void __fastcall Obj_AllocEntries(void* o);
int32_t __fastcall Obj_YearReached(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall Obj_LoadArrays(uint8_t* o, int, Stream s);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
int32_t __fastcall Station_LoadShare(uint8_t* o, int, uint8_t* from, uint8_t* to, uint8_t* city, float* out, int32_t apply);
void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind);
void __fastcall Load_AttachToTrain(uint32_t* load, int, uint32_t id);

void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);

void __fastcall Stock_Consume(uint16_t* o, int, const uint16_t* other, uint32_t id, float f);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
int32_t __fastcall Company_HomeStation(int32_t* c);
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall Obj_AddKey(void* o, int, uint16_t key, int32_t value);
void __fastcall Chain_RegisterBuildings(void* o, int, uint8_t* target, int32_t value);
float __fastcall Station_PathCost(void* o, int, int32_t x, int32_t y);
void __fastcall Obj_RemoveKey(void* o, int, uint16_t key);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
int32_t __fastcall Game_EffectiveVersion();
void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Obj_BuildingValue(void* o);

// RT3_REPLACE(0x4b23d0, Trains_Create); -- see above

int32_t __fastcall Company_Get7680(void* c);

void __fastcall Company_Add7680(void* c, int, int32_t d);
uint8_t* __fastcall TrackLight_Construct(uint8_t* o);
void __fastcall Effect489e80_Release(uint8_t* e);

// RT3_REPLACE(0x493ec0, Tracks_AddPiece); -- see above

void __fastcall Fatal_Exit(const char* msg);

// RT3_REPLACE(0x493cf0, Tracks_AddPieceRotated); -- see above

uint32_t __fastcall SysInfo_HasFeature0();

uint32_t __fastcall SysInfo_HasFeature1();
uint32_t __fastcall SysInfo_HasFeature2();
uint32_t __fastcall SysInfo_HasFeature3();
uint32_t __fastcall SysInfo_CpuSignature();
const char* __fastcall SysInfo_CpuName();
const char* __fastcall SysInfo_CpuVendor();
uint32_t __fastcall Sys_Call5c81d0();
const char* __fastcall Error_Message(uint32_t code);
char* Os_VersionName();
uint32_t __fastcall File_Version(const char* path);
uint32_t __fastcall SysInfo_FreeMemory();
uint32_t __fastcall SysInfo_TotalMemory();
const char* __fastcall SysInfo_DisplayName();
uint64_t __fastcall SysInfo_Zero64a();
uint64_t __fastcall SysInfo_Zero64b();
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);
float __fastcall SceneObj_Angle2(uint8_t* o);
void* __fastcall Obj_Ref3cc(void* o);
void __fastcall Node_Set222(void* n, int, int32_t v);
void __fastcall Chain_Push(void* o, int, uint16_t id);
void __fastcall Station_BuildCatchment(uint8_t* st, int, int32_t remove, int32_t kindArg);
void __fastcall Pairs_Clear(uint8_t* o);
void __fastcall Chain_Remove(void* o, int, uint16_t id);
void __fastcall Station_CollectIndustries(uint8_t* o);
int32_t __fastcall Node_HasLink(void* n, int, int32_t any);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
void* __fastcall Obj_Building(void* o);
void* __fastcall Obj_Ref25(void* o);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
uint32_t __fastcall Obj_BuildingRegionIndex(void* o);
int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall NthFreeCompanySlot(int32_t n);
void __fastcall Dialog_SetupWorthSlider(int32_t select);
void __fastcall MoneySlider_Setup(int32_t reset);
void __fastcall MoneySliders_Read(int32_t rerange);
void StreamInit();
void Language_Load();
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall Obj_Set21(void* o, int, int32_t v);
int32_t IsMultiplayer();
int32_t __fastcall Obj_Get25(void* o);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void __fastcall Obj_AttachmentPoint(Collection* c, int, int32_t mode, uint32_t id, float* x, float* z, float angle);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Obj_FreeEntries5bd(void* o);
void __fastcall Nop436040(void*, int, int32_t);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
int32_t CountFreeCompanySlots();
void __fastcall Company_SetChairman(int32_t* c, int, int32_t player);
void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y);
uint32_t __fastcall Obj_BuildingState8c(void* o);
void __fastcall Lots_UpdateLinked(Collection* c);
void Cities_PayConnectionBonuses();
void __fastcall Obj_StartAgeClock(void* o);
void* Engine_FirstListEntry();
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall Companies_BuildStationLinks(Collection* cos);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void* __fastcall Obj_OwnerCompany(void* o);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
void __fastcall Companies_BuildStationLinks(Collection* cos);
void Profiles_IndexOf();
void __fastcall Station_Remove(uint8_t* s);
void __fastcall Msg_FoundCompany(int32_t* m);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void* __fastcall Collection_GetAt(Collection* c, int, uint32_t pos, int32_t list);
void Player_FoundCompanyDialog();
void __fastcall Entries6_Remove(uint8_t* o, int, int16_t key);
void* __fastcall Obj_TrainOwner(uint8_t* o);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void* __fastcall Collection_GetAt(Collection* c, int, uint32_t pos, int32_t list);
int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle, float* outX, float* outY, float* outAngle, int32_t update, int32_t* outNode);
RT3_NOINLINE void __fastcall Company_SetChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE double __fastcall Obj_CellX(void* o);
RT3_NOINLINE double __fastcall Obj_CellY(void* o);
RT3_NOINLINE uint8_t* __fastcall Obj_Region(void* o);
RT3_NOINLINE double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE void __fastcall MoneySlider_Setup(int32_t reset);
RT3_NOINLINE void __fastcall Dialog_SetupWorthSlider(int32_t select);
void Window_ShowMoney7149();
RT3_NOINLINE void __fastcall MoneySliders_Read(int32_t rerange);
int32_t __fastcall MoneySliders_ReadCb(uint8_t* w);
int32_t __fastcall NewCompanyDialog_HandleEvent(uint8_t* d, uint32_t* msg);
void __fastcall PlayerSetup_Build(uint8_t* w, int, float x, float y, float wd, float h);
RT3_NOINLINE void Player_FoundCompanyDialog();
RT3_NOINLINE void __fastcall Msg_FoundCompany(int32_t* m);
void __fastcall Obj_Set3c(void* o, int, uint16_t v);
RT3_NOINLINE void __fastcall Site_RollStats(uint8_t* o);
double __fastcall Obj_Float4c4(void* o, int, int32_t i);
double __fastcall Obj_Float4ec(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float514(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float53c(void* o, int, int32_t i);
int32_t __fastcall Obj_Int564(void* o, int, int32_t i);
RT3_NOINLINE void __fastcall Entries6_Remove(uint8_t* o, int, int16_t key);
void __fastcall Nop47d8d0(void*);
int32_t __fastcall Obj_LoadArrays(uint8_t* o, int, Stream s);
void __fastcall Site_SaveExtra(uint8_t* o, int, Stream s);
void __fastcall Site_RecordMin(uint8_t* o, int, int32_t i, int32_t list, float v);
int32_t __fastcall Obj_ListEntry(uint8_t* o, int, int32_t i, int32_t list, int32_t* out);
RT3_NOINLINE int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
int32_t __fastcall Pairs_Total(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
RT3_NOINLINE void __fastcall Pairs_Clear(uint8_t* o);
void __fastcall Pairs_RemoveKey(uint8_t* o, int, int32_t key);
uint32_t __fastcall Obj_NodeNotify(uint8_t* o, int, int32_t flag);
RT3_NOINLINE void* __fastcall Obj_Building(void* o);
RT3_NOINLINE uint8_t* __fastcall Obj_BuildingRegion(void* o);
RT3_NOINLINE int32_t __fastcall Obj_BuildingQuarterX(void* o);
RT3_NOINLINE int32_t __fastcall Obj_BuildingQuarterY(void* o);
RT3_NOINLINE double __fastcall Obj_BuildingCellX(void* o);
RT3_NOINLINE double __fastcall Obj_BuildingCellY(void* o);
RT3_NOINLINE float __fastcall Obj_NodeCenterX(void* o);
RT3_NOINLINE float __fastcall Obj_NodeCenterY(void* o);
void __fastcall Station_UpdateRoutes(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
RT3_NOINLINE int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
RT3_NOINLINE float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls);
RT3_NOINLINE uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
RT3_NOINLINE void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount, float* outAmount, float* outFlagged);
void __fastcall Station_PickUp(uint8_t* o, int, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount, float* outValue, float* outAmount);
RT3_NOINLINE void __fastcall Station_LearnDemand(uint8_t* st, int, uint32_t from, uint8_t* consist);
RT3_NOINLINE void* __fastcall Obj_TrainOwner(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_BuildingValue(void* o);
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value);
void* __fastcall Obj_BuildingRegionAt(void* o);
RT3_NOINLINE uint32_t __fastcall Obj_BuildingRegionIndex(void* o);
uint32_t __fastcall Obj_BuildingMapQuery(void* o);
RT3_NOINLINE int32_t __fastcall Obj_NodeNear8(uint8_t* o, int, int32_t id);
RT3_NOINLINE void __fastcall Obj_SetHighlight(uint8_t* o, int, int32_t v);
int32_t __fastcall Station_ConnectedTo(uint8_t* s, int, uint32_t id);
RT3_NOINLINE void* __fastcall Obj_Node8(void* o);
RT3_NOINLINE void __fastcall Station_BuildCatchment(uint8_t* st, int, int32_t remove, int32_t kindArg);
int32_t __fastcall Station_PickCargo(uint8_t* st, int, int32_t tag, uint32_t destId, int32_t* lost, int32_t* fallback);
RT3_NOINLINE int32_t __fastcall Obj_BuildingStateActive(void* o);
void __fastcall Station_RecordNearby(uint8_t* s, int, uint8_t* obj);
void __fastcall Station_CollectIndustries(uint8_t* o);
void* __fastcall Obj_BuildingKind(void* o);
RT3_NOINLINE int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company);
void __fastcall Station_Init(uint8_t* st, int, int32_t id, uint32_t building, float x, float y);
RT3_NOINLINE void __fastcall Station_Remove(uint8_t* s);
RT3_NOINLINE void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind);
RT3_NOINLINE void __fastcall Station_Place(uint8_t* s, int, float x, float y);
RT3_NOINLINE Collection* __fastcall Collection5ce90c_Construct(Collection* c);
void __fastcall Collection5ce90c_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Collection_LoadWithArrays(Collection* c, int, Stream s, int32_t);
RT3_NOINLINE void __fastcall Sites_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Stations_Add(Collection* c, int, uint32_t building, float x, float y);
void __fastcall Stations_Delete(Collection* c, int, uint32_t id);
void __fastcall Table_Hook47d8d0(Collection* t);
void __fastcall Collection_RecordNearby(Collection* c, int, uint8_t* obj);
RT3_NOINLINE void __fastcall Table_RemovePairKey(Collection* t, int, int32_t key);
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId);
RT3_NOINLINE int32_t __fastcall Stations_CargoLinked(Collection* sts, int, int32_t a, int32_t b, int32_t aIsRegion, int32_t bIsRegion, int32_t value);
RT3_NOINLINE int32_t __fastcall Table_CountActiveSites(Collection* t, int, int32_t highlighted);
RT3_NOINLINE void __fastcall Stations_CountTargets(Collection* stations);
void __fastcall Table_MoveRef(Collection* t, int, uint32_t from, uint32_t to);
RT3_NOINLINE int32_t __fastcall Tracks_AddPieceRotated(Collection* c, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style, int32_t force);
const char* SysInfo_Report();
void __fastcall Cmdline_Parse(const char* p);
int32_t __fastcall Return1_481fb0(void*);
RT3_NOINLINE uint8_t* __fastcall Game_InitStart(uint8_t* g);
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from);
void __fastcall Track_InitRotated(uint8_t* t, int, int32_t id, int32_t group, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style);
void __fastcall Track_PlaceRotated(uint8_t* t, int, float cx, float cy, float angle);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
RT3_NOINLINE int32_t __fastcall TrackNet_LayLine(Collection* net, int, float x0, float y0, float x1, float y1, int32_t company, int32_t mode, int32_t doubleKind, int32_t* from, int32_t* to, float positive);
RT3_NOINLINE void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
RT3_NOINLINE int32_t __fastcall Train_TargetStation(uint8_t* t);
RT3_NOINLINE void __fastcall Trains_DropStation(Collection* trains, int, int32_t station);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
char* Os_VersionName();

// 0x47de50 / 0x47de90. Building (+4) position in quarter cells. QUIRK: no
// building reads through null as in the original.
static inline void* BuildingOf4(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return id ? Collection_Get(g_buildings, 0, id) : nullptr;
}

static inline void AddInto(uint8_t* o, uint32_t sum, uint32_t cur) {
    F<float>(o, sum) = (float)((double)F<float>(o, sum) + F<float>(o, cur));
}

// Shifts five 10-entry histories (stride 0x28 from `first`) by one and clears entry 0.
static void ShiftHistories(uint8_t* o, uint32_t first) {
    for (int k = 9; k >= 1; --k)
        for (int a = 0; a < 5; ++a) F<uint32_t>(o, first + a * 0x28 + k * 4) = F<uint32_t>(o, first + a * 0x28 + (k - 1) * 4);
    for (int a = 0; a < 5; ++a) F<uint32_t>(o, first + a * 0x28) = 0;
}

static inline Collection* Coll(uint32_t a) { return hook::Global<Collection*>(a); }

static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline void* Memmove(void* d, const void* s, size_t n) { return ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(d, s, n); }

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

static inline uint8_t* BuildingOf(uint8_t* o) {
    return F<uint32_t>(o, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
}

// Copies a switch argument after the first space or quote (skipping a second) into
// buf, stopping at a '-' or '/' (returns the position there) or the end (nullptr).
static const char* SwitchName(const char* p, char* buf) {
    int32_t n = 0;
    char c = *p;
    if (!c) return nullptr;
    do {
        if (c == '-' || c == '/') return p;
        if ((c == ' ' || c == '"') && !n) {
            char c2 = *++p;
            if (c2 == ' ' || c2 == '"') ++p;
            buf[0] = *p;
            n = 1;
        } else if (n && c != '"') buf[n++] = c;
        c = *++p;
    } while (c);
    return nullptr;
}

static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

static inline uint8_t* RegionOf(uint8_t* o) {
    return Obj_Region(F<uint32_t>(o, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr);
}

static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }

static inline uint8_t* Node(uint32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id); }

static inline double Fl_196(uint32_t a) { return *(const float*)a; }

static uint8_t* StationRegion(uint8_t* st) {
    return Obj_Region(F<uint32_t>(st, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(st, 4)) : nullptr);
}

static inline double Fl_203(uint32_t a) { return *(const float*)a; }

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

// dst = FLD a; FADD b (memory operand); FSTP dst - the operand order decides which
// NaN payload survives, so MSVC must not commute it.
static inline void X87AddMem(void* dst, const void* a, const void* b) {
    __asm {
        mov eax, a
        mov edx, b
        mov ecx, dst
        fld dword ptr [eax]
        fadd dword ptr [edx]
        fstp dword ptr [ecx]
    }
}

static inline uint8_t* Station_BuildingOf(uint8_t* st) {
    uint32_t id = F<uint32_t>(st, 4);
    return id ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id) : nullptr;
}

static inline uint8_t* Station_RegionAt(float fx, float fy) {
    int32_t ry = Ftol(fy);
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t idx = F<int32_t>(m, 0x2145) * ry + Ftol(fx);
    return F<uint8_t*>(m, 0x2129) + idx * 0x1d7;
}

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

namespace {
// The original's frame: its slots are reused with other types (ints, floats,
// doubles, a text buffer), so the translation keeps them at their offsets.
struct BuildFrame {
    alignas(8) uint8_t b[0x198];
    int32_t& i(uint32_t o) { return *(int32_t*)(b + o); }
    float& f(uint32_t o) { return *(float*)(b + o); }
    double& d(uint32_t o) { return *(double*)(b + o); }
    uint8_t& u8(uint32_t o) { return b[o]; }
    uint32_t* p(uint32_t o) { return (uint32_t*)(b + o); }
};
inline double BlCos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }
inline double BlSin(double x) { return ((double(__cdecl*)(double))0x5a1494)(x); }
inline double BlAbs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
inline double BlAtan(double x) { return ((double(__cdecl*)(double))0x5a1270)(x); }
inline float BlCell(float v) { return (float)((double)v / Fl_207(0x5c8700)); }
inline uint32_t BlBits(float v) { return *(uint32_t*)&v; }
inline uint8_t* BlTrack(Collection* c, int32_t id) { return (uint8_t*)Collection_Get(c, 0, (uint32_t)id); }
inline int32_t BlVirt5(uint8_t* o) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x14 / 4])(o); }
inline void BlVirt2(Collection* net, int32_t id) { ((void(__fastcall*)(Collection*, int, int32_t))(*(void***)net)[8 / 4])(net, 0, id); }
}

namespace {
uint32_t Message451(const char* text, uint32_t px, uint32_t py) {
    return Dialog_RunMessage(text, 1, px, py, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
int32_t MyProfileIndex451() {
    const char* me = (const char*)Engine_FirstListEntry() + 8;
    int32_t k;
    __asm {
        push ebx
        mov ebx, me
        call Profiles_IndexOf
        pop ebx
        mov k, eax
    }
    return k;
}
}

// 0x47c070. Sets up the amount slider 0x714f of window 0x6cec18 for the current
// player's collateral worth (capped at 1e8): the step 0x6cebd4 / base
// 0x6cec0c rounding (10,000 with base 80,000 under 1e6, then 100,000, 1e6, 1e7),
// the number of positions 0x5f2c4c (event 0x73) and, if `select`, the last
// position (event 0x66).
RT3_NOINLINE void __fastcall Dialog_SetupWorthSlider(int32_t select) {
    double worth = Player_CollateralWorth((uint8_t*)Obj_Ref25(hook::Global<uint8_t*>(0x6cec78)), 0, 1);
    double v = worth < D(0x5ce8a8) ? worth : 1e8;
    int32_t amount = Ftol(v);
    int32_t step, base;
    if (amount < 1000000) {
        step = 10000;
        base = 80000;
    } else {
        if (amount < 10000000)
            step = 100000;
        else
            step = amount >= 100000000 ? 10000000 : 1000000;
        base = step;
    }
    hook::Global<int32_t>(0x6cebd4) = base;
    int32_t top = amount - amount % step;
    hook::Global<int32_t>(0x6cec0c) = top;
    int32_t n = (top - base) / step + 1;
    if (n < 1) n = 1;
    hook::Global<int32_t>(0x5f2c4c) = n;
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x73, 0x714f, n, 0, 0);
    if (select) Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x66, 0x714f, hook::Global<int32_t>(0x5f2c4c) - 1, 0, 0);
}
RT3_REPLACE(0x47c070, Dialog_SetupWorthSlider);

// 0x47c190. Shows the two money amounts 0x6cec14 / 0x6cec10 (money-K style) in
// fields 0x7149 / 0x714a of window 0x6cec18 (event 0x6f).
void Window_ShowMoney7149() {
    char text[0x130];
    Num_FormatSimple2(text, 0xb, (double)hook::Global<int32_t>(0x6cec14), 0xb);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x6f, 0x7149, (uint32_t)(uintptr_t)text, 0, 0);
    Num_FormatSimple2(text, 0xb, (double)hook::Global<int32_t>(0x6cec10), 0xb);
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x6f, 0x714a, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x47c190, Window_ShowMoney7149);

// 0x47c220. Reads the loan window's (0x6cec18) two sliders back into amounts: the
// first (0x714f, knob +0x185 of 0x5f2c4c notches) from 0x6cebd4 up to 0x6cec0c into
// 0x6cec14, the second (0x7150, 0x5f2c50 notches) over 0x6cebcc..0x6cebd0 into
// 0x6cec10 (re-ranging the second first when `rerange`), then refreshes the money.
RT3_NOINLINE void __fastcall MoneySliders_Read(int32_t rerange) {
    if (!Widget_FindChild(hook::Global<uint8_t*>(0x6cec18), 0, 0x714f)) return;
    hook::Global<int32_t>(0x6cec14) = hook::Global<int32_t>(0x6cebd4);
    if (hook::Global<int32_t>(0x5f2c4c) > 1) {
        double knob = (double)F<int32_t>(Widget_FindChild(hook::Global<uint8_t*>(0x6cec18), 0, 0x714f), 0x185);
        double range = (double)(hook::Global<int32_t>(0x6cec0c) - hook::Global<int32_t>(0x6cebd4));
        hook::Global<int32_t>(0x6cec14) =
            Ftol(range * knob / (double)(hook::Global<int32_t>(0x5f2c4c) - 1) + (double)hook::Global<int32_t>(0x6cec14));
    }
    if (rerange) MoneySlider_Setup(0);
    hook::Global<int32_t>(0x6cec10) = hook::Global<int32_t>(0x6cebcc);
    if (hook::Global<int32_t>(0x5f2c50) > 1) {
        double knob = (double)F<int32_t>(Widget_FindChild(hook::Global<uint8_t*>(0x6cec18), 0, 0x7150), 0x185);
        double range = (double)(hook::Global<int32_t>(0x6cebd0) - hook::Global<int32_t>(0x6cebcc));
        hook::Global<int32_t>(0x6cec10) =
            Ftol(range * knob / (double)(hook::Global<int32_t>(0x5f2c50) - 1) + (double)hook::Global<int32_t>(0x6cec10));
    }
    MoneySlider_Setup(0);
    Window_ShowMoney7149();
}
RT3_REPLACE(0x47c220, MoneySliders_Read);

// 0x47c360 (slider callback). Reads the money sliders once (re-entry guard
// 0x6cec1c), re-ranging for widget 0x714f.
// The original returns the guard when it is set and whatever the reader left
// otherwise.
int32_t __fastcall MoneySliders_ReadCb(uint8_t* w) {
    if (const int32_t busy = hook::Global<int32_t>(0x6cec1c)) return busy;
    hook::Global<int32_t>(0x6cec1c) = 1;
    MoneySliders_Read(w && F<uint16_t>(w, 4) == 0x714f);
    --hook::Global<int32_t>(0x6cec1c);
    return 0;
}
RT3_REPLACE(0x47c360, MoneySliders_ReadCb);

// 0x47c3a0 (event handler). New company dialog: the name box 0x714b goes to
// 0x6cebd8 (50 chars) on edits and on OK (0x3f2), which in the campaign
// (0x6d3b4c) needs 1300000 in cash and bonds (0x6cec10 + 0x6cec14, else
// string 0xfa6); help (0x3f4, campaign only) shows string 0xe8c; 0x714c /
// 0x714d step the logo through the free company slots (0x5f2c48), showing
// slot 0..0x23 as a picture or the logo record of the rest (0x6d4020
// +0x429b0) in 0x714e.
int32_t __fastcall NewCompanyDialog_HandleEvent(uint8_t* d, uint32_t* msg) {
    if (msg[0] == 0xca) {
        if (msg[1] == 0x714b) {
            const char* text = F<const char*>((void*)(uintptr_t)msg[0xb], 0xba);
            if (*text) StrCopyN((char*)0x6cebd8, text, 0x32);
        }
        return 1;
    }
    if (msg[0] != 0xcb) return 1;
    const int32_t id = (int32_t)msg[1];
    if (id > 0x3f4) {
        if (id < 0x714c || id > 0x714d) return 1;
        const int32_t v = hook::Global<int32_t>(0x5f2c48) + CountFreeCompanySlots() + (msg[1] == 0x714c ? 1 : -1);
        const int32_t r = v % CountFreeCompanySlots();
        hook::Global<int32_t>(0x5f2c48) = r;
        const int32_t slot = NthFreeCompanySlot(r + 1);
        if (slot < 0x24) {
            Window_SendEvent5(d, 0, 0x6c, 0x714e, (uint32_t)slot, 0, 0);
            Window_SendEvent5(d, 0, 0x75, 0x714e, 0x5ce8b0, 0, 0);
        } else {
            Window_SendEvent5(d, 0, 0x6c, 0x714e, 0, 0, 0);
            void* logo = Collection_GetAt(F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0), 0, (uint32_t)(slot - 0x24), 0);
            Window_SendEvent5(d, 0, 0x75, 0x714e, (uint32_t)(uintptr_t)logo, 0, 0);
        }
        return 0;
    }
    if (id == 0x3f4) {
        if (!hook::Global<int32_t>(0x6d3b4c)) return 1;
        Message451(GetString(0xe8c), 0x3ef5c28f, 0xbf800000);
        return 0;
    }
    if (id != 0x3f2) return 1;
    if (hook::Global<int32_t>(0x6d3b4c) && hook::Global<int32_t>(0x6cec10) + hook::Global<int32_t>(0x6cec14) < 0x13d620) {
        Message451(GetString(0xfa6), 0xbf800000, 0xbf800000);
        return 0;
    }
    uint8_t* box = Widget_FindChild(d, 0, 0x714b);
    if (box && *F<const char*>(box, 0xba)) StrCopyN((char*)0x6cebd8, F<const char*>(box, 0xba), 0x32);
    return 1;
}
RT3_REPLACE(0x47c3a0, NewCompanyDialog_HandleEvent);

// 0x47c590. Builds the player setup panel in the box (x, y, w, h) of `w` (it
// becomes 0x6cec18), from the bottom up: two money sliders with their labels
// and value boxes (0x714f / 0x7149, 0x7150 / 0x714a), a frame, the player
// name box 0x714b (0x6cebd8), the company logo picker (0x714e, the next free
// logo slot after 0x5f2c48; slots from 0x24 are the pack's extra logos) with
// its arrows 0x714c / 0x714d and the 0xf82 caption; then sets the sliders.
void __fastcall PlayerSetup_Build(uint8_t* w, int, float x, float y, float wd, float h) {
    hook::Global<uint8_t*>(0x6cec18) = w;
    auto editBox = [&](uint16_t id, uint32_t px, uint32_t py, uint32_t pw, uint32_t ph, uint8_t align, uint8_t style, const char* text, bool scroll) {
        uint8_t e[0x91];
        memset(e, 0, sizeof e);
        F<uint32_t>(e, 7) = px;
        F<uint32_t>(e, 0xb) = py;
        F<uint32_t>(e, 0xf) = pw;
        F<uint32_t>(e, 0x13) = ph;
        if (id) F<uint16_t>(e, 0) = id;
        e[0x27] = align;
        e[0x28] = 0;
        F<int32_t>(e, 0x34) = -1;
        if (scroll) {
            F<int32_t>(e, 0x5f) = 1;
            F<int32_t>(e, 0x63) = 1;
            F<int32_t>(e, 0x67) = 0x26;
        }
        String_Assign(&F<char*>(e, 0x77), text);
        e[0x33] = style;
        uint8_t* b = (uint8_t*)ExeNew(0xf0);
        b = b ? EditBox_Construct(b, 0, e) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(e, 0x77));
    };
    auto listBox = [&](uint16_t id, float py) {
        uint8_t l[0xe3];
        memset(l, 0, sizeof l);
        F<float>(l, 7) = x;
        F<int32_t>(l, 0x63) = 1;
        F<int32_t>(l, 0x67) = 1;
        F<int32_t>(l, 0x97) = 1;
        F<float>(l, 0xb) = py;
        F<uint32_t>(l, 0xf) = 0x3e700000;
        F<uint32_t>(l, 0x13) = 0x3cb851ec;
        F<uint16_t>(l, 0) = id;
        F<int32_t>(l, 0x5f) = 0xa;
        F<int32_t>(l, 0x87) = 2;
        F<int32_t>(l, 0x8b) = 3;
        F<int32_t>(l, 0x8f) = 4;
        F<int32_t>(l, 0x93) = 5;
        F<int32_t>(l, 0x9b) = 0;
        F<uint32_t>(l, 0xab) = 0x3c088889;
        F<uint32_t>(l, 0xaf) = 0x3ceb851f;
        F<uint32_t>(l, 0xb3) = 0x3e500000;
        F<uint32_t>(l, 0xb7) = 0;
        F<uint32_t>(l, 0xbb) = 0;
        F<uint32_t>(l, 0xbf) = 0x3bcccccd;
        F<uint32_t>(l, 0xc3) = 0x3c088889;
        F<uint32_t>(l, 0xc7) = 0x3e51eb85;
        F<uint32_t>(l, 0xcb) = 0x3c088889;
        String_Assign(&F<char*>(l, 0x43), (const char*)0x5ce8d8);
        uint8_t* b = (uint8_t*)ExeNew(0x1b9);
        b = b ? ListBox_Construct(b, 0, l) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(l, 0x43));
        Window_SendEvent5(w, 0, 0x84, id, 0x47c360, 0, 0);
    };
    float row = (float)((double)y + h - D(0x5ce8f0));
    editBox(0, FBits(x), FBits(row), FBits(wd), 0x3cf5c28f, 0, 2, GetString(0x211), false);
    row = (float)((double)row + D(0x5c9b80));
    listBox(0x714f, row);
    float x1 = (float)((double)x + D(0x5c9c80));
    float w1 = (float)((double)wd - D(0x5c9c80));
    editBox(0x7149, FBits(x1), FBits(row), FBits(w1), 0x3cf5c28f, 3, 3, GetString(0x212), false);
    row = (float)((double)row + D(0x5c9b80));
    row = (float)((double)row + D(0x5ca1a8));
    editBox(0, FBits(x), FBits(row), FBits(wd), 0x3cf5c28f, 0, 2, GetString(0x213), false);
    row = (float)((double)row + D(0x5c9b80));
    listBox(0x7150, row);
    editBox(0x714a, FBits(x1), FBits(row), FBits(w1), 0x3cf5c28f, 3, 3, GetString(0x214), false);
    row = (float)((double)row + D(0x5c9b80));
    row = (float)((double)row + D(0x5ca1a8));
    editBox(0, FBits(x), FBits(row), FBits(wd), 0x3cf5c28f, 0, 2, GetString(0x215), false);
    row = (float)((double)row + D(0x5c9b80));
    {
        uint8_t s[0x89];
        memset(s, 0, sizeof s);
        F<float>(s, 7) = x;
        F<float>(s, 0xb) = row;
        F<float>(s, 0xf) = wd;
        F<uint32_t>(s, 0x13) = 0x3d23d70a;
        F<int32_t>(s, 0x61) = 3;
        F<int32_t>(s, 0x75) = 6;
        uint8_t* b = (uint8_t*)ExeNew(0x111);
        b = b ? Scrollbar_Construct(b, 0, s) : nullptr;
        Window_AddChild(w, 0, b, 0);
    }
    editBox(0x714b, FBits((float)((double)x - D(0x5ce8d0))), FBits(row), FBits((float)((double)wd + D(0x5cab68))), 0x3d23d70a, 0, 3,
            (const char*)0x6cebd8, true);
    row = (float)((double)row + D(0x5c9b98));
    row = (float)((double)row + D(0x5ca1a8));
    {
        uint8_t s[0x89];
        memset(s, 0, sizeof s);
        int32_t slotFrom = hook::Global<int32_t>(0x5f2c48) + 1;
        F<float>(s, 7) = (float)((double)x + D(0x5c8c20));
        F<uint32_t>(s, 0xf) = 0x3d866666;
        F<uint32_t>(s, 0x13) = 0x3db33333;
        F<float>(s, 0xb) = (float)((double)row - D(0x5c9bc0));
        int32_t slot = NthFreeCompanySlot(slotFrom);
        F<uint16_t>(s, 0x5f) = (uint16_t)slot;
        String_Assign(&F<char*>(s, 0x43), (const char*)0x5c9f98);
        if (F<int16_t>(s, 0x5f) >= 0x24) {
            Collection* logos = F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0);
            String_Assign(&F<char*>(s, 0x43), (const char*)Collection_Get(logos, 0, Collection_IdAt(logos, 0, F<int16_t>(s, 0x5f) - 0x24, 0)));
            F<uint16_t>(s, 0x5f) = 0;
        }
        F<uint16_t>(s, 0) = 0x714e;
        uint8_t* b = (uint8_t*)ExeNew(0x111);
        b = b ? Scrollbar_Construct(b, 0, s) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(s, 0x43));
    }
    float cx = (float)((double)x + D(0x5ce8f0));
    auto arrow = [&](uint16_t id, double dy, uint16_t i0, uint16_t i1) {
        uint8_t c[0xa5];
        memset(c, 0, sizeof c);
        F<float>(c, 0xb) = (float)((double)row + dy);
        F<float>(c, 7) = cx;
        F<uint32_t>(c, 0xf) = 0x3d0ccccd;
        F<uint32_t>(c, 0x13) = 0x3cae147b;
        F<uint16_t>(c, 0x5f) = i0;
        F<uint16_t>(c, 0x61) = i1;
        c[0x89] = 6;
        c[0x8a] = 7;
        F<uint16_t>(c, 0) = id;
        String_Assign(&F<char*>(c, 0x43), (const char*)0x5ce8d8);
        uint8_t* b = (uint8_t*)ExeNew(0xe9);
        b = b ? Control_Construct(b, 0, c) : nullptr;
        Window_AddChild(w, 0, b, 0);
        Free(F<void*>(c, 0x43));
    };
    arrow(0x714c, D(0x5ce8c8), 0xa, 0xb);
    arrow(0x714d, D(0x5ce8c0), 0xc, 0xd);
    editBox(0, FBits(x), FBits(row), 0x3e19999a, 0x3da3d70a, 3, 2, GetString(0xf82), false);
    Dialog_SetupWorthSlider(1);
    MoneySlider_Setup(1);
    MoneySliders_Read(0);
    MoneySlider_Setup(1);
    MoneySliders_Read(0);
    MoneySlider_Setup(0);
}
RT3_REPLACE(0x47c590, PlayerSetup_Build);

// 0x47d080. The player founds a company: a random free logo (0x5f2c48,
// kept while the dialog repeats), a stock name (0x5f2c58 by the player and
// 0x6cec78+0x41, which then advances) into 0x6cebd8, and the dialog 0x23e
// (0x47c3a0 / 0x47c590 lay out and handle it). On OK the chosen logo
// (0x424430 of 0x5f2c48 + 1; a pack image name from 0x24 on) and the cash
// 0x6cec10 + 0x6cec14 found the company as in 0x47d320, buying shares for
// 0x6cec14 (which then holds their cost); in multiplayer this is sent to the
// host (0x19) instead. QUIRK: the custom logo name buffer stays uninitialised
// for stock logos.
RT3_NOINLINE void Player_FoundCompanyDialog() {
    if (hook::Global<int32_t>(0x5f2c48) == -1) hook::Global<int32_t>(0x5f2c48) = Rand(0, CountFreeCompanySlots() - 1);
    int32_t count = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x41);
    int32_t pick = (Obj_Get25(hook::Global<uint8_t*>(0x6cec78)) * 0x11 + count) % 0x28;
    StrCopyN((char*)0x6cebd8, GetString(((const int32_t*)0x5f2c58)[pick]), 0x32);
    ++F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x41);
    uint32_t extraBits = 0x3ea3d70a;
    uint32_t r = Dialog_RunMessage(GetString(0x23e), 2, 0xbf800000, 0xbf800000, 0x47c3a0, (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x47c590,
                                   0, *(const float*)&extraBits, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    hook::Global<int32_t>(0x6cec18) = 0;
    if (r != 0x3f2) {
        hook::Global<int32_t>(0x5f2c48) = -1;
        return;
    }
    int32_t logo = NthFreeCompanySlot(hook::Global<int32_t>(0x5f2c48) + 1);
    char logoName[0x10];
    if (logo >= 0x24) {
        Collection* images = F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0);
        const char* img = (const char*)Collection_Get(images, 0, Collection_IdAt(images, 0, (uint32_t)(logo - 0x24), 0));
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(logoName, img);
        logo = -1;
    }
    uint8_t* pl = (uint8_t*)Obj_Ref25(hook::Global<uint8_t*>(0x6cec78));
    if (!IsMultiplayer()) {
        if (pl && F<int32_t>(pl, 0x1dd))
            Company_SetChairman((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(pl, 0x1dd)), 0, 0);
        Collection* cos = hook::Global<Collection*>(0x62be10);
        uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, int32_t, int32_t, int32_t, int32_t))(*(void***)cos)[1])(cos, 0, 0, 0, 0, 0);
        uint8_t* co = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
        int32_t cash = hook::Global<int32_t>(0x6cec10) + hook::Global<int32_t>(0x6cec14);
        Company_Init(co, 0, (int32_t)id, Obj_Get25(hook::Global<uint8_t*>(0x6cec78)), logo, (const char*)0x6cebd8, logoName, cash);
        int32_t price = (int32_t)F<float>(co, 0x57);
        int32_t shares = (hook::Global<int32_t>(0x6cec14) / price + 0x1f4) / 0x3e8 * 0x3e8;
        if (shares < 0) shares = 0;
        if (shares > F<int32_t>(co, 0x47)) shares = F<int32_t>(co, 0x47);
        hook::Global<int32_t>(0x6cec14) = price * shares;
        F<int32_t>(pl, 0x15d + (int32_t)id * 4) = shares;
        Obj_AddDouble154(pl, 0, -hook::Global<int32_t>(0x6cec14));
        Obj_Set21(hook::Global<uint8_t*>(0x6cec78), 0, (int32_t)id);
        Events_Fire(hook::Global<Collection*>(0x62be18), 0, 7, nullptr, 0);
        hook::Global<int32_t>(0x5f2c48) = -1;
        return;
    }
    int32_t msg[0x14];
    msg[0] = F<int32_t>(pl, 0), msg[1] = logo, msg[2] = hook::Global<int32_t>(0x6cec14), msg[3] = hook::Global<int32_t>(0x6cec10);
    strcpy((char*)(msg + 4), (const char*)0x6cebd8);
    Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 6, 0, 0);
    uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
    Net_Send(s, 0, (const char*)(s + 0x8f48), 0x19, 0x43, (const uint8_t*)msg, 5, 0);
    hook::Global<int32_t>(0x5f2c48) = -1;
}
RT3_REPLACE(0x47d080, Player_FoundCompanyDialog);

// 0x47d320 (thiscall on message m: player, logo, cash, extra cash, name).
// Player m+0 founds a company: resigns any chairmanship it holds, gets a new
// company (vtable slot 1) set up with the cash m+8 + m+0xc, and buys
// round(m+8 / price, thousands) of its shares (at most +0x47 issued; price
// +0x57), paying for them. The local player switches to it (cursor 1).
// Fires the trigger-7 events.
RT3_NOINLINE void __fastcall Msg_FoundCompany(int32_t* m) {
    uint8_t* pl = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)m[0]);
    if (pl && F<int32_t>(pl, 0x1dd))
        Company_SetChairman((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(pl, 0x1dd)), 0, 0);
    Collection* cos = hook::Global<Collection*>(0x62be10);
    uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, int32_t, int32_t, int32_t, int32_t))(*(void***)cos)[1])(cos, 0, 0, 0, 0, 0);
    uint8_t* co = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
    Company_Init(co, 0, (int32_t)id, m[0], m[1], (const char*)(m + 4), nullptr, m[3] + m[2]);
    int32_t price = (int32_t)F<float>(co, 0x57);
    int32_t shares = (m[2] / price + 0x1f4) / 0x3e8 * 0x3e8;
    if (shares < 0) shares = 0;
    if (shares > F<int32_t>(co, 0x47)) shares = F<int32_t>(co, 0x47);
    F<int32_t>(pl, 0x15d + (int32_t)id * 4) = shares;
    Obj_AddDouble154(pl, 0, -(price * shares));
    if (Obj_Get25(hook::Global<uint8_t*>(0x6cec78)) == m[0]) {
        Obj_Set21(hook::Global<uint8_t*>(0x6cec78), 0, (int32_t)id);
        Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 0);
    }
    Events_Fire(hook::Global<Collection*>(0x62be18), 0, 7, nullptr, 0);
}
RT3_REPLACE(0x47d320, Msg_FoundCompany);

void __fastcall Obj_Set3c(void* o, int, uint16_t v) { F<uint16_t>(o, 0x3c) = v; }
RT3_REPLACE(0x47d6f0, Obj_Set3c);

// 0x47d700. Same for the site stats (+0x4c4..; the fifth history +0x564 is an integer).
RT3_NOINLINE void __fastcall Site_RollStats(uint8_t* o) {
    AddInto(o, 0x4b0, 0x4c4);
    F<int32_t>(o, 0x4c0) += F<int32_t>(o, 0x564);
    F<float>(o, 0x4b4) = (float)((double)F<float>(o, 0x4ec) + F<float>(o, 0x4b4));
    AddInto(o, 0x4b8, 0x514);
    AddInto(o, 0x4bc, 0x53c);
    ShiftHistories(o, 0x4c4);
}
RT3_REPLACE(0x47d700, Site_RollStats);

double __fastcall Obj_Float4c4(void* o, int, int32_t i) { return F<float>(o, 0x4c4 + i * 4); }
RT3_REPLACE(0x47d7c0, Obj_Float4c4);

double __fastcall Obj_Float4ec(void* o, int, int32_t i) { return F<float>(o, 0x4ec + i * 4); }
RT3_REPLACE(0x47d7d0, Obj_Float4ec);

RT3_NOINLINE double __fastcall Obj_Float514(void* o, int, int32_t i) { return F<float>(o, 0x514 + i * 4); }
RT3_REPLACE(0x47d7e0, Obj_Float514);

RT3_NOINLINE double __fastcall Obj_Float53c(void* o, int, int32_t i) { return F<float>(o, 0x53c + i * 4); }
RT3_REPLACE(0x47d7f0, Obj_Float53c);

int32_t __fastcall Obj_Int564(void* o, int, int32_t i) { return F<int32_t>(o, 0x564 + i * 4); }
RT3_REPLACE(0x47d800, Obj_Int564);

// 0x47d810 (vtable). Removes the first entry with key `key` from o's list of
// 6-byte entries (+0x466, count +0x462), keeping at most count - 1.
RT3_NOINLINE void __fastcall Entries6_Remove(uint8_t* o, int, int16_t key) {
    uint8_t* out = nullptr;
    if (F<int32_t>(o, 0x462) > 1) {
        out = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)((F<int32_t>(o, 0x462) * 3 - 3) * 2));
        int32_t kept = 0;
        uint8_t* dst = out;
        for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i) {
            const uint8_t* e = F<uint8_t*>(o, 0x466) + i * 6;
            if (F<int16_t>((uint8_t*)e, 0) == key || kept >= F<int32_t>(o, 0x462) - 1) continue;
            F<uint32_t>(dst, 0) = F<uint32_t>((uint8_t*)e, 0);
            F<uint16_t>(dst, 4) = F<uint16_t>((uint8_t*)e, 4);
            ++kept;
            dst += 6;
        }
    }
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(o, 0x466));
    F<uint8_t*>(o, 0x466) = out;
    F<int32_t>(o, 0x462) = F<int32_t>(o, 0x462) - 1;
}
RT3_REPLACE(0x47d810, Entries6_Remove);

RT3_NOINLINE void __fastcall Nop47d8d0(void*) { __asm nop }
RT3_REPLACE(0x47d8d0, Nop47d8d0);

// 0x47d8e0. Loads o's arrays from stream s (chunk 0x4e21..0x4e24): +0x462 entries of
// 6 bytes (+0x466), 3 x +0x30 x 5 bytes (+0x24..), 5 lists of 8-byte items (counts
// +0x590, data +0x5a4) and +0x38 x 12 bytes (+0x34).
RT3_NOINLINE int32_t __fastcall Obj_LoadArrays(uint8_t* o, int, Stream s) {
    StreamSkip4(s, 0x4e21, 0, 0);
    StreamRead(s, o + 0x462, 4, 0, 1);
    F<void*>(o, 0x466) = nullptr;
    if (F<int32_t>(o, 0x462) > 0) {
        F<uint8_t*>(o, 0x466) = (uint8_t*)Malloc((F<int32_t>(o, 0x462) * 3 + 3) * 2);
        for (int32_t k = 0; k < F<int32_t>(o, 0x462); ++k) StreamRead(s, F<uint8_t*>(o, 0x466) + k * 6, 6, 0, 1);
    }
    if (F<int32_t>(o, 0x30) > 0) {
        int32_t n = F<int32_t>(o, 0x30) * 5;
        for (int j = 0; j < 3; ++j) {
            void* p = Malloc(n);
            F<void*>(o, 0x24 + j * 4) = p;
            StreamRead(s, p, n, 0, 1);
        }
    }
    for (int j = 0; j < 5; ++j) {
        int32_t n = F<int32_t>(o, 0x590 + j * 4);
        if (n <= 0) continue;
        void* p = Malloc(n * 8);
        F<void*>(o, 0x5a4 + j * 4) = p;
        StreamRead(s, p, n * 8, 0, 1);
    }
    if (F<int32_t>(o, 0x38) > 0) {
        int32_t n = F<int32_t>(o, 0x38) * 12;
        void* p = Malloc(n);
        F<void*>(o, 0x34) = p;
        StreamRead(s, p, n, 0, 1);
    }
    F<int32_t>(o, 0x5bd) = 0;
    Obj_AllocEntries(o);
    StreamSkip4(s, 0x4e24, 0, 0);
    return 0;
}
RT3_REPLACE(0x47d8e0, Obj_LoadArrays);

// 0x47da40. Site extra data save (tags 0x4e21/0x4e24): key list +0x466,
// the three 5-byte lists +0x24 (count +0x30), the five pair arrays +0x5a4
// and the 12-byte array +0x34 (count +0x38).
RT3_NOINLINE void __fastcall Site_SaveExtra(uint8_t* o, int, Stream s) {
    StreamWriteU32(s, 0x4e21);
    StreamWrite(s, o + 0x462, 4, 0);
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i) StreamWrite(s, F<uint8_t*>(o, 0x466) + i * 6, 6, 0);
    if (F<int32_t>(o, 0x30))
        for (int k = 0; k < 3; ++k) StreamWrite(s, F<void*>(o, 0x24 + k * 4), F<int32_t>(o, 0x30) * 5, 0);
    for (int k = 0; k < 5; ++k)
        if (F<int32_t>(o, 0x590 + k * 4) > 0) StreamWrite(s, F<void*>(o, 0x5a4 + k * 4), F<int32_t>(o, 0x590 + k * 4) * 8, 0);
    if (F<int32_t>(o, 0x38) > 0) StreamWrite(s, F<void*>(o, 0x34), F<int32_t>(o, 0x38) * 12, 0);
    StreamWriteU32(s, 0x4e24);
}
RT3_REPLACE(0x47da40, Site_SaveExtra);

// 0x47db30. Records value v for entry i of list `list` (three 5-byte
// {float, used} lists +0x24, count +0x30) keeping the minimum; the lists
// grow to i + 3 entries (new entries 0x4de67b70 = 4.8e8) when needed.
void __fastcall Site_RecordMin(uint8_t* o, int, int32_t i, int32_t list, float v) {
    if (i >= F<int32_t>(o, 0x30)) {
        int32_t newBytes = (i + 3) * 5;
        for (int k = 0; k < 3; ++k) {
            int32_t oldBytes = F<int32_t>(o, 0x30) * 5;
            uint8_t* nb = (uint8_t*)Malloc(newBytes);
            if (oldBytes) {
                const uint8_t* old = F<uint8_t*>(o, 0x24 + k * 4);
                for (int32_t b = 0; b < oldBytes; ++b) nb[b] = old[b];
                Free(F<void*>(o, 0x24 + k * 4));
            }
            F<uint8_t*>(o, 0x24 + k * 4) = nb;
            for (int32_t b = oldBytes; b < newBytes; ++b) nb[b] = 0;
            for (int32_t j = F<int32_t>(o, 0x30); j < i + 3; ++j) F<uint32_t>(nb, j * 5) = 0x4de67b70;
        }
        F<int32_t>(o, 0x30) = i + 3;
    }
    uint8_t* e = F<uint8_t*>(o, 0x24 + list * 4) + i * 5;
    if (!e[4] || F<float>(e, 0) > v) {
        F<float>(e, 0) = v;
        e[4] = 1;
    }
}
RT3_REPLACE(0x47db30, Site_RecordMin);

// 0x47dc50. Value of entry i of list `list` (+0x24, 5-byte {value, used}); 1 if present.
int32_t __fastcall Obj_ListEntry(uint8_t* o, int, int32_t i, int32_t list, int32_t* out) {
    if (i >= F<int32_t>(o, 0x30)) return 0;
    uint8_t* e = F<uint8_t*>(o, 0x24 + list * 4) + i * 5;
    if (!e[4]) return 0;
    *out = F<int32_t>(e, 0);
    return 1;
}
RT3_REPLACE(0x47dc50, Obj_ListEntry);

RT3_NOINLINE int32_t __fastcall Obj_Array590(void* o, int, int32_t i) { return F<int32_t>(o, 0x590 + i * 4); }
RT3_REPLACE(0x47dc90, Obj_Array590);

// 0x47dca0..0x47dd10. Five arrays of 8-byte {key, value} pairs (+0x5a4,
// counts +0x590, total +0x5b8).
int32_t __fastcall Pairs_Total(uint8_t* o) { return F<int32_t>(o, 0x5b8); }
RT3_REPLACE(0x47dca0, Pairs_Total);

RT3_NOINLINE uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i) { return F<uint8_t*>(o, 0x5a4 + list * 4) + i * 8 - 8; }
RT3_REPLACE(0x47dcb0, Pairs_At);

RT3_NOINLINE void __fastcall Pairs_Clear(uint8_t* o) {
    for (int k = 0; k < 5; ++k) {
        void*& p = F<void*>(o, 0x5a4 + k * 4);
        if (p) {
            Free(p);
            p = nullptr;
        }
        F<int32_t>(o, 0x590 + k * 4) = 0;
    }
    F<int32_t>(o, 0x5b8) = 0;
}
RT3_REPLACE(0x47dcd0, Pairs_Clear);

RT3_NOINLINE void __fastcall Pairs_RemoveKey(uint8_t* o, int, int32_t key) {
    for (int k = 0; k < 5; ++k) {
        int32_t& count = F<int32_t>(o, 0x590 + k * 4);
        for (int32_t j = count - 1; j >= 0; --j) {
            uint8_t* buf = F<uint8_t*>(o, 0x5a4 + k * 4);
            uint8_t* e = buf + j * 8;
            if (F<int32_t>(e, 0) != key) continue;
            if (count == 1) {
                Free(buf);
                F<void*>(o, 0x5a4 + k * 4) = nullptr;
            } else if (count - j - 1 > 0) {
                ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(e, e + 8, (count - j - 1) * 8);
            }
            --count;
            F<int32_t>(o, 0x5b8)--;
        }
    }
}
RT3_REPLACE(0x47dd10, Pairs_RemoveKey);

// 0x47dda0. Unless its track node (+8) reports 0x48a090(1), calls the node's
// virtual slot 1 (`flag`) or 2.
uint32_t __fastcall Obj_NodeNotify(uint8_t* o, int, int32_t flag) {
    int32_t n = F<int32_t>(o, 8);
    if (n == -1) return (uint32_t)n;
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    if (!Collection_IsValid(nodes, 0, n)) return 0;
    void* node = Collection_Get(nodes, 0, F<uint32_t>(o, 8));
    uint32_t r = ((uint32_t(__thiscall*)(void*, int32_t))0x48a090)(node, 1);
    if (r) return r;
    return ((uint32_t(__thiscall*)(void*))(*(void***)node)[flag ? 1 : 2])(node);
}
RT3_REPLACE(0x47dda0, Obj_NodeNotify);

// 0x47de00. The building referenced at +4 (NULL if none).
RT3_NOINLINE void* __fastcall Obj_Building(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return id ? Collection_Get(g_buildings, 0, id) : nullptr;
}
RT3_REPLACE(0x47de00, Obj_Building);

// 0x47de20. Region record under the building referenced at +4.
RT3_NOINLINE uint8_t* __fastcall Obj_BuildingRegion(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_Region(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47de20, Obj_BuildingRegion);

RT3_NOINLINE int32_t __fastcall Obj_BuildingQuarterX(void* o) { return (int32_t)(Obj_CellX(BuildingOf4(o)) / *(const float*)0x5c8568); }
RT3_REPLACE(0x47de50, Obj_BuildingQuarterX);

RT3_NOINLINE int32_t __fastcall Obj_BuildingQuarterY(void* o) { return (int32_t)(Obj_CellY(BuildingOf4(o)) / *(const float*)0x5c8568); }
RT3_REPLACE(0x47de90, Obj_BuildingQuarterY);

// 0x47ded0 / 0x47df00. Cell position of the building referenced at +4.
RT3_NOINLINE double __fastcall Obj_BuildingCellX(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_CellX(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47ded0, Obj_BuildingCellX);

RT3_NOINLINE double __fastcall Obj_BuildingCellY(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_CellY(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47df00, Obj_BuildingCellY);

// 0x47df30 / 0x47df50. Centre (cells) of the track node at +8.
RT3_NOINLINE float __fastcall Obj_NodeCenterX(void* o) { return Obj_CenterCellX(Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8))); }
RT3_REPLACE(0x47df30, Obj_NodeCenterX);

RT3_NOINLINE float __fastcall Obj_NodeCenterY(void* o) { return Obj_CenterCellY(Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8))); }
RT3_REPLACE(0x47df50, Obj_NodeCenterY);

// 0x47df70. Ages the station's 53 cargo ratings (+0x3e, zeroed after 0x1d8800 ticks
// without service, else decayed; +0x112 too) and relaxes its three route distance
// tables (+0x24/+0x28/+0x2c, 5-byte entries: distance, known flag) through each
// known neighbour's table: d(i) + neighbour d(j) beats d(j) for j not i or itself.
void __fastcall Station_UpdateRoutes(uint8_t* o) {
    uint8_t* b = F<uint32_t>(o, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
    Obj_Region(b);
    for (int32_t k = 0; k < 0x35; ++k) {
        float* p = &F<float>(o, 0x3e + k * 4);
        if (*p <= *(const float*)0x5c8598) continue;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15) - F<int32_t>(p, 0x1a8) > 0x1d8800) *p = 0.0f;
        else {
            *p = (float)(*p * D(0x5ce900));
            F<float>(p, 0xd4) = (float)(F<float>(p, 0xd4) * D(0x5ce8f8));
        }
    }
    for (uint32_t t = 0x24; t < 0x30; t += 4) {
        for (int32_t i = 0; i < F<int32_t>(o, 0x30); ++i) {
            uint8_t* e = F<uint8_t*>(o, t) + i * 5;
            if (!e[4]) continue;
            F<float>(e, 0) = (float)(F<float>(e, 0) + (double)*(const float*)0x5c9f18);
            float d = F<float>(F<uint8_t*>(o, t) + i * 5, 0);
            uint8_t* other = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, i);
            int32_t n = F<int32_t>(o, 0x30);
            if (n >= F<int32_t>(other, 0x30)) n = F<int32_t>(other, 0x30);
            for (int32_t j = 0; j < n; ++j) {
                uint8_t* oe = F<uint8_t*>(other, t) + j * 5;
                if (!oe[4]) continue;
                uint8_t* me = F<uint8_t*>(o, t) + j * 5;
                if (!((double)d + F<float>(oe, 0) < F<float>(me, 0)) || j == i || j == F<int32_t>(o, 0)) continue;
                me[4] = 1;
                F<float>(F<uint8_t*>(o, t) + j * 5, 0) = (float)((double)d + F<float>(F<uint8_t*>(other, t) + j * 5, 0));
            }
        }
    }
}
RT3_REPLACE(0x47df70, Station_UpdateRoutes);

// 0x47e240. For cargo i: the stock (+0x103) summed over o's cells (+0x38 x 12
// bytes at +0x34, {x, y}) and the quota of o's region city; 0 / 1.0 and 0 when
// the cargo is not available yet.
RT3_NOINLINE int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota) {
    if (!Obj_YearReached(Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, i))) {
        if (stock) *stock = 0.0f;
        if (quota) *quota = 1.0f;
        return 0;
    }
    void* owner = F<uint32_t>(o, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
    float sum = 0.0f;
    float q = (float)City_CargoQuota(Obj_Region(owner), 0, i);
    int32_t n = F<int32_t>(o, 0x38);
    if (n > 0) {
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        const int32_t* c = F<const int32_t*>(o, 0x34);
        for (; n; --n, c += 3) {
            uint8_t* city = F<uint8_t*>(m, 0x2129) + (c[1] * F<int32_t>(m, 0x2145) + c[0]) * 0x1d7;
            sum = (float)((double)sum + F<float>(city, 0x103 + i * 4));
        }
    }
    if (stock) *stock = sum;
    if (quota) *quota = q;
    return 1;
}
RT3_REPLACE(0x47e240, Obj_CargoStockAndQuota);

// 0x47e330. Number of cargos currently available to o.
RT3_NOINLINE int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(Cargos()); ++i)
        if (Obj_CargoStockAndQuota(o, 0, Collection_IdAt(Cargos(), 0, i, 0), nullptr, nullptr)) ++n;
    return n;
}
RT3_REPLACE(0x47e330, Obj_CountAvailableCargos);

// 0x47e390 (with its tail 0x47e41f, reached only by falling through). The supply
// of cargo class cls (0x384..0x386) over a region's cells (+0x34, count +0x38,
// { x, y, - }): the sum of the cells' per-cargo amounts (+0x103) of every enabled
// cargo in the class. The three classes are cached (+0x14 / +0xc / +0x10 with
// stamps +0x20 / +0x18 / +0x1c) for 0xb40 minutes.
RT3_NOINLINE float __fastcall Region_ClassSupply(uint8_t* r, int, int32_t cls) {
    int32_t now = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    if (cls == 0x384 && F<int32_t>(r, 0x20) > 0 && now - F<int32_t>(r, 0x20) < 0xb40) return F<float>(r, 0x14);
    if (cls == 0x386 && F<int32_t>(r, 0x18) > 0 && now - F<int32_t>(r, 0x18) < 0xb40) return F<float>(r, 0xc);
    if (cls == 0x385 && F<int32_t>(r, 0x1c) > 0 && now - F<int32_t>(r, 0x1c) < 0xb40) return F<float>(r, 0x10);
    uint8_t in[0x35];
    memset(in, 0, sizeof in);
    for (int32_t i = 0; i < 0x35; ++i) {
        if (!Collection_IsValid(hook::Global<Collection*>(0x62ba8c), 0, (uint32_t)i)) continue;
        void* c = Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, (uint32_t)i);
        if (!Cargo_MatchesClass(c, 0, cls)) continue;
        if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x16e + i * 4)) in[i] = 1;
    }
    float sum = 0.0f;
    if (F<int32_t>(r, 0x38) > 0) {
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        int32_t w = F<int32_t>(m, 0x2145);
        uint8_t* cells = F<uint8_t*>(m, 0x2129);
        int32_t* p = F<int32_t*>(r, 0x34);
        for (int32_t n = F<int32_t>(r, 0x38); n; --n, p += 3) {
            uint8_t* cell = cells + (p[1] * w + p[0]) * 0x1d7;
            for (int32_t i = 0; i < 0x35; ++i)
                if (in[i]) X87AddMem(&sum, &sum, cell + 0x103 + i * 4);
        }
    }
    if (cls == 0x384) {
        F<float>(r, 0x14) = sum;
        F<int32_t>(r, 0x20) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    } else if (cls == 0x386) {
        F<float>(r, 0xc) = sum;
        F<int32_t>(r, 0x18) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    } else if (cls == 0x385) {
        F<float>(r, 0x10) = sum;
        F<int32_t>(r, 0x1c) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    }
    return sum;
}
RT3_REPLACE(0x47e390, Region_ClassSupply);

// 0x47e620. The n-th (1-based) cargo available to o, with its stock and quota; 0 if none.
RT3_NOINLINE uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t) {
    for (int32_t i = 0; i < Collection_Count(Cargos()); ++i)
        if (Obj_CargoStockAndQuota(o, 0, Collection_IdAt(Cargos(), 0, i, 0), stock, quota) && !--n)
            return Collection_IdAt(Cargos(), 0, i, 0);
    return 0;
}
RT3_REPLACE(0x47e620, Obj_NthAvailableCargo);

// 0x47e690. Gathers the cargo lots of `cargo` waiting at the station's buildings
// (+0x466 list, lot chains from building +0x272) that the train going to `to` can
// take, best share first, at most min(40, capacity) lots. Reports the number of
// lots / k (outCount), the summed share (outAmount) and the lots flagged 2 / k
// (outFlagged); unless `simulate`, loads them (charging upkeep on fresh lots).
RT3_NOINLINE void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged) {
    int32_t room = Ftol(((double)cap + D(0x5c8678)) * D(0x5c8d68));
    *outCount = 0.0f;
    *outAmount = 0.0f;
    if (outFlagged) *outFlagged = 0.0f;
    if (!room) return;
    int32_t maxN = room > 0x28 ? 0x28 : room;
    int32_t cls = F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4);
    int32_t n = 0, flagged = 0;
    float vals[0x28];
    uint32_t ids[0x28];
    float v;
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(F<uint8_t*>(o, 0x466), i * 6));
        for (uint32_t id = F<uint32_t>(b, 0x272); id;) {
            uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, id);
            uint32_t next = F<uint32_t>(c, 4);
            if (c[0x10] == cls) {
                if (c[0x12] & 2) ++flagged;
                if (Station_LoadShare(c, 0, o, to, city, &v, 0)) {
                    int32_t pos = 0;
                    while (pos < n && !(v > vals[pos])) ++pos;
                    if (pos < maxN) {
                        int32_t last = maxN - 1 > n ? n : maxN - 1;
                        int32_t cnt = last - pos;
                        if (cnt) {
                            Memmove(&ids[pos + 1], &ids[pos], cnt * 4);
                            Memmove(&vals[pos + 1], &vals[pos], cnt * 4);
                        }
                        vals[pos] = v;
                        ids[pos] = id;
                        if (++n > maxN) n = maxN;
                    }
                }
            }
            id = next;
        }
    }
    for (int32_t j = 0; j < n; ++j) {
        *outAmount = (float)(vals[j] + (double)*outAmount);
        if (simulate) continue;
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, ids[j]);
        if (F<uint16_t>(c, 0xa) == F<uint16_t>(c, 0xc)) Route_ChargeRandomBuilding(c, 0, o, simulate);
        Station_LoadShare(c, 0, o, to, city, &v, 1);
        Load_AttachToTrain((uint32_t*)c, 0, F<uint32_t>(city, 0x25));
    }
    *outCount = (float)((double)(float)n / Fl(0x5c8554));
    if (outFlagged) *outFlagged = (float)((double)(float)flagged / Fl(0x5c8554));
}
RT3_REPLACE(0x47e690, Train_GatherLoads);

// 0x47e9a0. Picks up `cargo` at the station for a train with room `cap`: city cargos
// via the waiting lots; others straight from the stock of the station's catchment
// cells (+0x34 list of {x, y, share}), reporting the amount (outCount, all of it with
// option +0x130) and its value at the region's quota (outValue), stamping the pickup
// time (+0x1e6) and, with `from`, its source (+0x2ba / +0x38e).
void __fastcall Station_PickUp(uint8_t* o, int, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount,
                               float* outValue, float* outAmount) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4) != -1) {
        Train_GatherLoads(o, 0, 0, city, to, cargo, cap, outCount, outAmount, nullptr);
        return;
    }
    uint8_t* region = RegionOf(o);
    *outCount = 0.0f;
    for (int32_t i = 0; i < F<int32_t>(o, 0x38); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0x34) + i * 0xc;
        uint8_t* m = Map();
        uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(e, 4) * F<int32_t>(m, 0x2145) + F<int32_t>(e, 0)) * 0x1d7;
        float* stock = &F<float>(cell, 0x103 + cargo * 4);
        if (!(*stock > Fl(0x5c8598))) continue;
        float room = (float)((double)cap - *outCount);
        float take = *stock < room ? *stock : room;
        Stock_Consume((uint16_t*)cell, 0, (const uint16_t*)region, cargo, F<float>(e, 8));
        *outCount = (float)((double)take + *outCount);
        *stock = (float)((double)*stock - take);
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130)) *outCount = cap;
    F<int32_t>(o, 0x1e6 + cargo * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    if (!(*outCount > Fl(0x5c8598))) {
        *outValue = 0.0f;
        return;
    }
    int32_t q = City_CargoQuota(RegionOf(o), 0, cargo);
    *outValue = (float)((double)(float)q * *outCount);
    if (to) {
        F<int32_t>(o, 0x2ba + cargo * 4) = F<int32_t>(to, 0);
        F<int32_t>(o, 0x38e + cargo * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    }
}
RT3_REPLACE(0x47e9a0, Station_PickUp);

// 0x47eb90. A train from station `from` unloaded at this station: for each car's
// cargo class (consist +0x28 & 0x40: per-car list +1, else one class +0x24) the
// cargo this station could best have supplied (stock beyond what is already
// counted, demand at `from`'s region minus here, weighted by availability) is
// counted, and the cargos more wanted there than here (by more than that) are
// noted with their demand. The station's per-cargo trip counts (+0x3e), dates
// (+0x1e6) and demand averages (+0x112; a higher new demand gets weight x 0x5c8900)
// are then updated.
RT3_NOINLINE void __fastcall Station_LearnDemand(uint8_t* st, int, uint32_t from, uint8_t* consist) {
    if (from == F<uint32_t>(st, 0)) return;
    uint8_t* here = StationRegion(st);
    uint8_t* there = StationRegion((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, from));
    float demand[0x35] = {}, wanted[0x35] = {}, counted[0x35] = {};
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    int32_t n = consist[0x28] & 0x40 ? consist[0] : consist[0x23];
    float best = 0.0f;
    for (int32_t car = 0; car < n; ++car) {
        int32_t tag = consist[0x28] & 0x40 ? F<int32_t>(consist, 1 + car * 4) : F<int32_t>(consist, 0x24);
        best = 0.0f;
        int32_t pick = -1;
        for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
            int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
            if (F<int32_t>(cargos, 0x9a + id * 4) != -1) continue;
            if (!Cargo_MatchesClass(Collection_Get(cargos, 0, id), 0, tag)) continue;
            float stock, quota;
            if (!Obj_CargoStockAndQuota(st, 0, id, &stock, &quota)) continue;
            stock = (float)((double)stock - counted[id]);
            if (stock < D(0x5c8628)) continue;
            float v = (float)City_CargoQuota(there, 0, id);
            v = (float)((double)v - quota);
            v = (float)(v * (stock < D(0x5c8770) ? (double)stock : 1.0));
            if (v > best) {
                best = v;
                pick = id;
            }
        }
        if (pick != -1) counted[pick] = (float)((double)counted[pick] + D(0x5c8770));
        for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
            int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
            if (F<int32_t>(cargos, 0x9a + id * 4) != -1) continue;
            uint8_t* rec = (uint8_t*)Collection_Get(cargos, 0, id);
            if (!Cargo_MatchesClass(rec, 0, tag) || !Obj_YearReached(rec)) continue;
            int32_t qa = City_CargoQuota(there, 0, id);
            float d = (float)(qa - City_CargoQuota(here, 0, id));
            if (!(d > best)) continue;
            wanted[id] = (float)((double)wanted[id] + Fl_196(0x5c85b4));
            demand[id] = (float)City_CargoQuota(there, 0, id);
        }
    }
    for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
        int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
        if (!(counted[j] > Fl_196(0x5c8598)) && counted[j] == counted[j] && !(wanted[j] > Fl_196(0x5c8598)) && wanted[j] == wanted[j])
            continue;
        float trips = (float)((double)wanted[id] + F<float>(st, 0x3e + id * 4));
        F<int32_t>(st, 0x1e6 + id * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
        float w = wanted[id];
        F<float>(st, 0x3e + id * 4) = trips;
        if (demand[id] > F<float>(st, 0x112 + id * 4)) w = (float)(w * Fl_196(0x5c8900));
        float sum = (float)((double)trips + w);
        double den = sum < D(0x5c86b8) ? 0.1 : (double)sum;
        float& avg = F<float>(st, 0x112 + id * 4);
        avg = (float)(((double)trips * avg + (double)w * demand[id]) / den);
    }
}
RT3_REPLACE(0x47eb90, Station_LearnDemand);

RT3_NOINLINE void* __fastcall Obj_TrainOwner(uint8_t* o) {
    return Obj_OwnerCompany(F<uint32_t>(o, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr);
}
RT3_REPLACE(0x47efb0, Obj_TrainOwner);

// 0x47efe0. Cargo value (+0x276) of the building referenced at +4.
// QUIRK: with no building it reads address 0x276 (crashes), as the original.
RT3_NOINLINE int32_t __fastcall Obj_BuildingValue(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    volatile uint8_t* b = id ? (uint8_t*)Collection_Get(g_buildings, 0, id) : nullptr;
    return *(volatile int32_t*)(b + 0x276);
}
RT3_REPLACE(0x47efe0, Obj_BuildingValue);

// 0x47f010. Adds {key, value} to the 6-byte list +0x466 (count +0x462) unless the key is present.
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value) {
    int32_t n = F<int32_t>(o, 0x462);
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i)
        if (F<uint16_t>(F<uint8_t*>(o, 0x466), i * 6) == key) return;
    uint8_t* p = (uint8_t*)Malloc((n + 1) * 6);
    n = F<int32_t>(o, 0x462);
    if (n > 0) {
        const uint8_t* old = F<uint8_t*>(o, 0x466);
        for (int32_t k = 0; k < n * 6; ++k) p[k] = old[k];
    }
    F<uint16_t>(p, n * 6) = key;
    F<int32_t>(p, n * 6 + 2) = value;
    if (F<void*>(o, 0x466)) Free(F<void*>(o, 0x466));
    F<uint8_t*>(o, 0x466) = p;
    F<int32_t>(o, 0x462) = n + 1;
}
RT3_REPLACE(0x47f010, Obj_AddKeyUnique);

// 0x47f0e0. Region record (0x6cfc9c) at the building's cell.
void* __fastcall Obj_BuildingRegionAt(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    int32_t yi = (int32_t)Obj_CellY(b1);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    int32_t xi = (int32_t)x;
    return Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, row[xi] >> 3);
}
RT3_REPLACE(0x47f0e0, Obj_BuildingRegionAt);

// 0x47f170. Region index (cell byte >> 3) at the building's cell.
RT3_NOINLINE uint32_t __fastcall Obj_BuildingRegionIndex(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    int32_t yi = (int32_t)Obj_CellY(b1);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    return row[(int32_t)x] >> 3;
}
RT3_REPLACE(0x47f170, Obj_BuildingRegionIndex);

// 0x47f1f0. Map query 0x44e270 at the building's position.
uint32_t __fastcall Obj_BuildingMapQuery(void* o) {
    void* b1 = BuildingOf4(o);
    void* b2 = BuildingOf4(o);
    float x = (float)Obj_CellX(b2);
    float y = (float)Obj_CellY(b1);
    return ((uint32_t(__thiscall*)(void*, float, float))0x44e270)(hook::Global<void*>(0x62c120), x, y);
}
RT3_REPLACE(0x47f1f0, Obj_BuildingMapQuery);

// 0x47f250 / 0x41c010. Whether the track node at +8 / +0x356 is node `id` or links to
// it within two steps.
RT3_NOINLINE int32_t __fastcall Obj_NodeNear8(uint8_t* o, int, int32_t id) {
    int32_t n = F<int32_t>(o, 8);
    if (n == -1) return 0;
    if (id == n) return 1;
    return Track_LinksWithin((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, n), 0, id, 2, -1) ? 1 : 0;
}
RT3_REPLACE(0x47f250, Obj_NodeNear8);

// 0x47f290. Sets the highlight byte +0x5bc and refreshes the building when it changes.
RT3_NOINLINE void __fastcall Obj_SetHighlight(uint8_t* o, int, int32_t v) {
    if (o[0x5bc] == v) return;
    o[0x5bc] = (uint8_t)v;
    Obj_RefreshHighlight(BuildingOf4(o));
}
RT3_REPLACE(0x47f290, Obj_SetHighlight);

// 0x47f2d0. Whether station id is track-connected to this station.
int32_t __fastcall Station_ConnectedTo(uint8_t* s, int, uint32_t id) {
    uint32_t other = F<uint32_t>(Collection_Get(Stations(), 0, id), 8);
    return Node_SameComponent(Node(F<uint32_t>(s, 8)), 0, other);
}
RT3_REPLACE(0x47f2d0, Station_ConnectedTo);

RT3_NOINLINE void* __fastcall Obj_Node8(void* o) { return Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8)); }
RT3_REPLACE(0x47f310, Obj_Node8);

// 0x47f320. (Re)builds the station's catchment: the blocks within its radius
// (kind +0x8c of its building type, or `kind`: 0 small 1.25, 1 medium 2, else large
// 3; centred on +0x4a8 / +0x4ac) get the station registered with a distance weight
// (and its buildings); with `remove` they are unregistered instead. The covered
// blocks are stored (+0x34, count +0x38) as {x, y, cost from the station} sorted by
// cost.
RT3_NOINLINE void __fastcall Station_BuildCatchment(uint8_t* st, int, int32_t remove, int32_t kindArg) {
    uint8_t* bt = F<uint32_t>(st, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(st, 4)) : nullptr;
    int32_t kind = ((uint8_t*)(*(uint8_t*(__fastcall**)(void*))(*(uint8_t**)bt + 0x80))(bt))[0x8c];
    if (kindArg != -1) kind = kindArg;
    float r, cx, cy;
    if (kind == 0) {
        r = 1.25f;
        cx = (float)((double)Ftol((double)F<float>(st, 0x4a8) + F<float>(st, 0x4a8)) * D(0x5c8628) + D(0x5c8758));
        cy = (float)((double)Ftol((double)F<float>(st, 0x4ac) + F<float>(st, 0x4ac)) * D(0x5c8628) + D(0x5c8758));
    } else {
        r = kind == 1 ? 2.0f : 3.0f;
        cx = (float)((double)Ftol(F<float>(st, 0x4a8)) + D(0x5c8628));
        cy = (float)((double)Ftol(F<float>(st, 0x4ac)) + D(0x5c8628));
    }
    int32_t ri = Ftol((double)r + Fl_203(0x5c85b4));
    if (F<void*>(st, 0x34)) Free(F<void*>(st, 0x34));
    F<void*>(st, 0x34) = nullptr;
    F<int32_t>(st, 0x38) = 0;
    int32_t n = 0;
    uint8_t* buf = nullptr;
    if (!remove) buf = (uint8_t*)Malloc(0x2ee0);
    if (F<void*>(st, 0x466)) Free(F<void*>(st, 0x466));
    F<void*>(st, 0x466) = nullptr;
    F<int32_t>(st, 0x462) = 0;
    float fr = (float)ri;
    int32_t x0 = Ftol((double)cx - fr), x1 = Ftol((double)fr + cx), y1 = Ftol((double)fr + cy);
    int32_t sx = Ftol(F<float>(st, 0x4a8)), sy = Ftol(F<float>(st, 0x4ac));
    int32_t y0 = Ftol((double)cy - fr);
    uint8_t* e = buf;
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (x < 0 || x >= F<int32_t>(m, 0x2145) || y < 0 || y >= F<int32_t>(m, 0x2149)) continue;
            float yf = (float)((double)y + D(0x5c8628)), xf = (float)((double)x + D(0x5c8628));
            float d = (float)Distance2D(cx, cy, xf, yf);
            if (!(d < r)) continue;
            if (kind == 0) d = 0.7f;
            float w = (float)(D(0x5c8770) - (double)d / r * D(0x5ca138));
            auto cell = [&]() {
                uint8_t* mm = hook::Global<uint8_t*>(0x62c120);
                return F<uint8_t*>(mm, 0x2129) + (F<int32_t>(mm, 0x2145) * y + x) * 0x1d7;
            };
            if (remove) {
                Obj_RemoveKey(cell(), 0, F<uint16_t>(st, 0));
                continue;
            }
            Obj_AddKey(cell(), 0, F<uint16_t>(st, 0), *(int32_t*)&w);
            Chain_RegisterBuildings(cell(), 0, st, *(int32_t*)&w);
            F<int32_t>(e, 0) = x;
            F<int32_t>(e, 4) = y;
            F<float>(e, 8) = Station_PathCost(cell(), 0, sx, sy);
            ++n;
            e += 0xc;
        }
    }
    if (remove) return;
    F<uint8_t*>(st, 0x34) = (uint8_t*)Malloc((n < 1 ? 1 : n) * 0xc);
    F<int32_t>(st, 0x38) = n;
    uint8_t used[1000] = {};
    for (int32_t k = 0; k < n; ++k) {
        int32_t best = -1;
        for (int32_t c = 0; c < n; ++c)
            if (!used[c] && (best == -1 || F<float>(buf, c * 0xc + 8) < F<float>(buf, best * 0xc + 8))) best = c;
        used[best] = 1;
        memcpy(F<uint8_t*>(st, 0x34) + k * 0xc, buf + best * 0xc, 0xc);
    }
    Free(buf);
}
RT3_REPLACE(0x47f320, Station_BuildCatchment);

// 0x47f910. Picks the cargo of class `tag` this station should load for station
// `dest`: city cargos (with a quota slot) by their simulated pickup value, others by
// the destination's demand less what is already promised, x availability (scenario
// mode +0x4af7: a flat 10000, 20000 for Diesel / Weapons / Ammunition). In
// randomised modes (+0x130 / +0x82 / sandbox) a value may be swapped for a 1..100
// x k roll. Returns the best above the floor (1500, 0 in those modes) or -1; *lost
// tells whether any candidate lost, *fallback gets a city cargo with nothing waiting
// (preferring 0x62ba8c +0x88/+0x8a).
int32_t __fastcall Station_PickCargo(uint8_t* st, int, int32_t tag, uint32_t destId, int32_t* lost, int32_t* fallback) {
    uint8_t* dest = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, destId);
    uint8_t* region = Obj_Region(F<uint32_t>(dest, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(dest, 4))
                                                      : nullptr);
    auto game = []() { return hook::Global<uint8_t*>(0x6cec74); };
    auto opts = []() { return hook::Global<uint8_t*>(0x6cec7c); };
    auto world = []() { return hook::Global<uint8_t*>(0x6cec78); };
    auto sandbox = [&]() { return F<int32_t>(world(), 0x4aef) && !F<int32_t>(world(), 0x4af7); };
    float floor = 1500.0f;
    if (F<int32_t>(game(), 0x130) || opts()[0x82] || sandbox()) floor = 0.0f;
    if (fallback) *fallback = 0;
    int32_t best = -1, rolled = 0, anyLost = 0;
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    for (int32_t j = 0; j < Collection_Count(cargos); ++j) {
        int32_t id = (int32_t)Collection_IdAt(cargos, 0, j, 0);
        if (!Cargo_MatchesClass(Collection_Get(cargos, 0, id), 0, tag)) continue;
        float value = 0.0f;
        bool randomMode = F<int32_t>(game(), 0x130) || opts()[0x82] || sandbox();
        if (F<int32_t>(cargos, 0x9a + id * 4) != -1) {
            if (st[0x5c9] || F<int32_t>(world(), 0x4afb)) continue;
            float count, flagged;
            Train_GatherLoads(st, 0, 1, nullptr, dest, id, 1.0f, &count, &value, &flagged);
            if (count < D(0x5c8628) && !randomMode) {
                if (flagged < D(0x5c8628) || flagged != flagged || !fallback) continue;
                uint32_t c88 = F<uint16_t>(cargos, 0x88), c8a = F<uint16_t>(cargos, 0x8a);
                if ((uint32_t)*fallback == c88) continue;
                if ((uint32_t)*fallback == c8a && (uint32_t)id != c88) continue;
                *fallback = id;
                continue;
            }
        } else {
            float stock, quota;
            if (!Obj_CargoStockAndQuota(st, 0, id, &stock, &quota)) continue;
            if (stock < D(0x5c8628) && !randomMode) continue;
            value = (float)City_CargoQuota(region, 0, id);
            value = (float)((double)value - quota);
            if (F<int32_t>(world(), 0x4af7)) {
                if (st[0x5c9]) value = 0.0f;
                else {
                    value = 10000.0f;
                    if (F<int32_t>(world(), 0x4afb)) {
                        const char* name = (const char*)Collection_Get(cargos, 0, id) + 4;
                        if (!Stricmp(name, (const char*)0x5c99b8) || !Stricmp(name, (const char*)0x5c99b0) ||
                            !Stricmp(name, (const char*)0x5c99a4))
                            value = 20000.0f;
                    }
                }
            }
            value = (float)(value * (stock < D(0x5c8770) ? (double)stock : 1.0));
        }
        int32_t roll = 0;
        if (opts()[0x82] || sandbox() || F<int32_t>(game(), 0x130)) {
            if (best == -1 || rolled) {
                double v = value;
                double r = (double)SimRand(1, 100) * D(0x5c8678);
                if (!(r < v)) r = (double)SimRand(1, 100) * D(0x5c8678);
                else r = value;
                value = (float)r;
                roll = 1;
            }
        }
        if (value > floor) {
            rolled = roll;
            floor = value;
            best = id;
        } else anyLost = 1;
    }
    if (lost) *lost = anyLost;
    return best;
}
RT3_REPLACE(0x47f910, Station_PickCargo);

// 0x47fd50. True if the referenced building's state object (vtable slot 32)
// has state byte +0x8c in 0..2.
RT3_NOINLINE int32_t __fastcall Obj_BuildingStateActive(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    void* b = id ? Collection_Get(g_buildings, 0, id) : nullptr;
    void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    uint8_t s = F<uint8_t>(st, 0x8c);
    return s <= 2 ? 1 : 0;
}
RT3_REPLACE(0x47fd50, Obj_BuildingStateActive);

// 0x47fdb0. Records a nearby object (within 0x5c8738 cells of the building +4)
// in the per-kind (Obj_StateB9) list +0x5a4 / count +0x590 as {+0x2a4, distance}.
RT3_NOINLINE void __fastcall Station_RecordNearby(uint8_t* s, int, uint8_t* obj) {
    if (!Obj_BuildingStateActive(s)) return;
    uint32_t id = F<uint32_t>(s, 4);
    void* b1 = id ? Collection_Get(Coll(0x62b26c), 0, id) : nullptr;
    void* b2 = F<uint32_t>(s, 4) ? Collection_Get(Coll(0x62b26c), 0, F<uint32_t>(s, 4)) : nullptr;
    float bx = (float)Obj_CellX(b2);
    float by = (float)Obj_CellY(b1);
    float oy = (float)Obj_CellY(obj);
    float ox = (float)Obj_CellX(obj);
    float d = (float)Distance2D(ox, oy, bx, by);
    if (!(d <= *(const float*)0x5c8738)) return;
    int32_t k = Obj_StateB9(obj);
    int32_t cnt = ++F<int32_t>(s, 0x590 + k * 4);
    uint32_t* rec = (uint32_t*)Malloc(cnt * 8);
    if (F<int32_t>(s, 0x590 + k * 4) > 1) {
        uint8_t* old = F<uint8_t*>(s, 0x5a4 + k * 4);
        uint32_t n = F<int32_t>(s, 0x590 + k * 4) * 8 - 8;
        for (uint32_t j = 0; j < n; ++j) ((uint8_t*)rec)[j] = old[j];
        Free(F<void*>(s, 0x5a4 + k * 4));
    }
    rec[F<int32_t>(s, 0x590 + k * 4) * 2 - 2] = F<uint32_t>(obj, 0x2a4);
    rec[F<int32_t>(s, 0x590 + k * 4) * 2 - 1] = *(uint32_t*)&d;
    F<uint32_t*>(s, 0x5a4 + k * 4) = rec;
    F<int32_t>(s, 0x5b8)++;
}
RT3_REPLACE(0x47fdb0, Station_RecordNearby);

// 0x47fef0. For an active station collects the industries (kind 4) of the nearby
// region cells within range into the per-type lists (+0x590 counts, +0x5a4 arrays
// of {building id, distance}, +0x5b8 total). Quirk: the cells scanned are offset
// -2..2 minus the station's cell, not plus.
RT3_NOINLINE void __fastcall Station_CollectIndustries(uint8_t* o) {
    if (!Obj_BuildingStateActive(o)) return;
    float x = (float)Obj_CellX(BuildingOf(o));
    float y = (float)Obj_CellY(BuildingOf(o));
    for (int32_t dy = -2; dy <= 2; ++dy)
        for (int32_t dx = -2; dx <= 2; ++dx) {
            int32_t cx = dx - Ftol(Obj_CellX(BuildingOf(o)) / Fl(0x5c859c));
            int32_t cy = dy - Ftol(Obj_CellY(BuildingOf(o)) / Fl(0x5c859c));
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (cx < 0 || cx >= F<int32_t>(m, 0x2145) || cy < 0 || cy >= F<int32_t>(m, 0x2149)) continue;
            for (uint32_t id = F<uint16_t>(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * cy + cx) * 0x1d7, 0xd6); id;) {
                uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
                uint8_t* ind = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
                if (ind[0x32] == 4) {
                    int32_t kind = (int8_t)ind[0xb9];
                    float py = (float)Obj_CellY(b);
                    float d = (float)Distance2D((float)Obj_CellX(b), py, x, y);
                    if (d <= Fl(0x5c8738)) {
                        ++F<int32_t>(o, 0x5b8);
                        int32_t cnt = ++F<int32_t>(o, 0x590 + kind * 4);
                        uint8_t* arr = (uint8_t*)Malloc(cnt * 8);
                        if (F<int32_t>(o, 0x590 + kind * 4) > 1) {
                            memcpy(arr, F<uint8_t*>(o, 0x5a4 + kind * 4), F<int32_t>(o, 0x590 + kind * 4) * 8 - 8);
                            Free(F<uint8_t*>(o, 0x5a4 + kind * 4));
                        }
                        F<uint32_t>(arr, F<int32_t>(o, 0x590 + kind * 4) * 8 - 8) = id;
                        F<float>(arr, F<int32_t>(o, 0x590 + kind * 4) * 8 - 4) = d;
                        F<uint8_t*>(o, 0x5a4 + kind * 4) = arr;
                    }
                }
                id = F<uint16_t>(b, 0x2a2);
            }
        }
}
RT3_REPLACE(0x47fef0, Station_CollectIndustries);

// 0x480160. 0x62bae0 record of the building at +4 (u16 +0x26e; 0 = the first record).
// QUIRK: without a building it reads address 0x26e, as the original.
void* __fastcall Obj_BuildingKind(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    volatile uint8_t* b = id ? (uint8_t*)Collection_Get(g_buildings, 0, id) : nullptr;
    uint32_t k = *(volatile uint16_t*)(b + 0x26e);
    if (!k) k = Collection_IdAt(g_table62bae0, 0, 0, 0);
    return Collection_Get(g_table62bae0, 0, k);
}
RT3_REPLACE(0x480160, Obj_BuildingKind);

// 0x4801a0. Whether this active, ready station is track-connected to the company's
// home station.
RT3_NOINLINE int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company) {
    if (!Obj_BuildingStateActive(s)) return 0;
    void* b = F<uint32_t>(s, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(s, 4)) : nullptr;
    if (Obj_Flag3d4(b) != 1) return 0;
    int32_t home = Company_HomeStation((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, company));
    if (home == -1) return 0;
    return Node_SameComponent(Node(F<uint32_t>(s, 8)), 0, home);
}
RT3_REPLACE(0x4801a0, Station_ConnectedToHome);

// 0x480210. Sets up station st (id, building) at cell (x, y): clears its state and
// tables, then puts its platform on the track: onto a free straight piece when
// Station_SnapToTrack finds one, else by laying a new turned piece there (kind 2
// for a rail station building +0x3d4 == 1, else 0, owned by the building's
// company; checked against the company's limits only for rail stations). The
// piece is claimed (+0x222); the station joins its map region's chain and builds
// its catchment.
void __fastcall Station_Init(uint8_t* st, int, int32_t id, uint32_t building, float x, float y) {
    F<uint32_t>(st, 4) = building;
    F<int32_t>(st, 0) = id;
    F<int32_t>(st, 0x18) = F<int32_t>(st, 0x1c) = F<int32_t>(st, 0x20) = 0;
    st[0x5bc] = 0;
    st[0x46b] = 0;
    F<int32_t>(st, 0x34) = F<int32_t>(st, 0x38) = 0;
    F<int32_t>(st, 0x5c5) = F<int32_t>(st, 0x5bd) = 0;
    Obj_AllocEntries(st);
    F<uint16_t>(st, 0x3c) = 0;
    F<int32_t>(st, 0x462) = F<int32_t>(st, 0x466) = F<int32_t>(st, 0x58c) = 0;
    st[0x5c9] = 0;
    float angle = SceneObj_Angle2(Station_BuildingOf(st));
    float ox, oy;
    Type_RotatedCell(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(Station_BuildingOf(st), 0x3cc), &ox, &oy, angle);
    Obj_Ref3cc(Station_BuildingOf(st));
    for (int k = 0; k < 0x35; ++k) {
        F<int32_t>(st, 0x3e + k * 4) = 0;
        F<int32_t>(st, 0x112 + k * 4) = 0;
        F<int32_t>(st, 0x1e6 + k * 4) = 0;
        F<int32_t>(st, 0x2ba + k * 4) = 0;
        F<int32_t>(st, 0x38e + k * 4) = 0;
    }
    float sx, sy, sa;
    int32_t node = id;
    int32_t snapped = Station_SnapToTrack(hook::Global<Collection*>(0x62b2fc), 0, F<int32_t>(st, 0),
                                          F<uint32_t>(Station_BuildingOf(st), 0x3cc), x, y, angle, &sx, &sy, &sa, 0, &node);
    st[0x46a] = (uint8_t)snapped;
    if (!(snapped && node != -1 &&
          F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)node), 0x222) == -1)) {
        st[0x46a] = 0;
        uint8_t* b = Station_BuildingOf(st);
        int32_t kind = Obj_Flag3d4(b) == 1 ? 2 : 0;
        int32_t owner = F<int32_t>(Station_BuildingOf(st), 0x276);
        uint8_t* b2 = Station_BuildingOf(st);
        int32_t force = Obj_Flag3d4(b) != 1 ? 1 : 0;
        float pieceAngle = SceneObj_Angle2(b2);
        float cy = (float)((double)oy + y), cx = (float)((double)x - ox);
        node = ((int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t))0x493cf0)(
            hook::Global<Collection*>(0x6cfca8), 0, cx, cy, pieceAngle, kind, owner, 1, force);
    }
    F<int32_t>(st, 8) = node;
    if (node != -1) Node_Set222(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)node), 0, F<int32_t>(st, 0));
    for (uint32_t off = 0x4b0; off <= 0x4c0; off += 4) F<int32_t>(st, off) = 0;
    F<int32_t>(st, 0x30) = 0;
    F<float>(st, 0x4a8) = (float)((double)x / Fl_205(0x5c8568));
    F<float>(st, 0x4ac) = (float)((double)y / Fl_205(0x5c8568));
    F<int32_t>(st, 0x24) = F<int32_t>(st, 0x28) = F<int32_t>(st, 0x2c) = 0;
    for (int k = 0; k < 10; ++k) {
        F<int32_t>(st, 0x4c4 + k * 4) = 0;
        F<int32_t>(st, 0x4ec + k * 4) = 0;
        F<int32_t>(st, 0x514 + k * 4) = 0;
        F<int32_t>(st, 0x53c + k * 4) = 0;
        F<int32_t>(st, 0x564 + k * 4) = 0;
    }
    uint16_t sid = F<uint16_t>(st, 0);
    int32_t ry = Ftol(F<float>(st, 0x4ac));
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t idx = F<int32_t>(m, 0x2145) * ry + Ftol(F<float>(st, 0x4a8));
    Chain_Push(F<uint8_t*>(m, 0x2129) + idx * 0x1d7, 0, sid);
    Station_BuildCatchment(st, 0, 0, -1);
    F<int32_t>(st, 0x5c1) = 0;
    for (int k = 0; k < 5; ++k) {
        F<int32_t>(st, 0x590 + k * 4) = 0;
        F<int32_t>(st, 0x5a4 + k * 4) = 0;
    }
    F<int32_t>(st, 0x5b8) = 0;
    memset(st + 0x5ca, 0, 0x43);
}
RT3_REPLACE(0x480210, Station_Init);

// 0x480590. Station s (id +0) leaves the world: frees its entries, pairs and
// buffer +0x34, takes back its catchment, trains drop it, its track node +8
// is deleted (or just unlinked, +0x222 = -1, while still connected), it
// leaves its map cell's chain (cell of +0x4a8 / +0x4ac) and frees +0x24..+0x2c;
// every other station clears its 5-byte entry for s in those three tables.
RT3_NOINLINE void __fastcall Station_Remove(uint8_t* s) {
    Obj_FreeEntries5bd(s);
    Pairs_Clear(s);
    if (void* b = F<void*>(s, 0x34)) ((void(__cdecl*)(void*))0x5a1145)(b);
    F<void*>(s, 0x34) = nullptr;
    F<int32_t>(s, 0x38) = 0;
    Station_BuildCatchment(s, 0, 1, -1);
    if (hook::Global<Collection*>(0x6cfcbc)) Trains_DropStation(hook::Global<Collection*>(0x6cfcbc), 0, F<int32_t>(s, 0));
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    if (F<int32_t>(s, 8) != -1 && Collection_IsValid(nodes, 0, F<uint32_t>(s, 8))) {
        if (!Node_HasLink(Collection_Get(nodes, 0, F<uint32_t>(s, 8)), 0, 1)) Tracks_Delete(nodes, 0, F<uint32_t>(s, 8), 0);
        else F<int32_t>(Collection_Get(nodes, 0, F<uint32_t>(s, 8)), 0x222) = -1;
    }
    uint16_t id = F<uint16_t>(s, 0);
    F<int32_t>(s, 8) = -1;
    int32_t y = Ftol((double)F<float>(s, 0x4ac));
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t x = Ftol((double)F<float>(s, 0x4a8));
    Chain_Remove(F<uint8_t*>(m, 0x2129) + (y * F<int32_t>(m, 0x2145) + x) * 0x1d7, 0, id);
    for (int k = 0; k < 3; ++k)
        if (void* b = F<void*>(s, 0x24 + 4 * k)) ((void(__cdecl*)(void*))0x5a1145)(b);
    for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++j) {
        Collection* ss = hook::Global<Collection*>(0x6cec20);
        uint8_t* o = (uint8_t*)Collection_Get(ss, 0, Collection_IdAt(ss, 0, (uint32_t)j, 0));
        int32_t me = F<int32_t>(s, 0);
        if (me == F<int32_t>(o, 0) || me >= F<int32_t>(o, 0x30)) continue;
        F<uint8_t*>(o, 0x24)[me * 5 + 4] = 0;
        F<uint8_t*>(o, 0x28)[me * 5 + 4] = 0;
        F<uint8_t*>(o, 0x2c)[me * 5 + 4] = 0;
    }
    Nop436040(hook::Global<void*>(0x6cec78), 0, F<int32_t>(s, 0));
}
RT3_REPLACE(0x480590, Station_Remove);

// 0x480710. Moves station st to cell (x, y): it leaves its old catchment and map
// region, joins the new ones and recollects its industries; a rail station
// (building +0x3d4 == 1) or a `keepTrack` move stops there. Otherwise its platform
// moves too: onto a free straight piece found by Station_SnapToTrack (releasing
// the old piece, deleting it when it has no links); failing that, an old piece
// with links stays unless a second snap also fails, in which case a new turned
// piece is laid (style 1, forced) and the old one released; a lone old piece is
// re-placed at the new spot, and a station without one lays a new piece there.
RT3_NOINLINE void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind) {
    Pairs_Clear(st);
    Station_BuildCatchment(st, 0, 1, kind);
    Chain_Remove(Station_RegionAt(F<float>(st, 0x4a8), F<float>(st, 0x4ac)), 0, F<uint16_t>(st, 0));
    float fx = (float)((double)x / Fl_205(0x5c8568));
    F<float>(st, 0x4a8) = fx;
    float fy = (float)((double)y / Fl_205(0x5c8568));
    F<float>(st, 0x4ac) = fy;
    Chain_Push(Station_RegionAt(fx, fy), 0, F<uint16_t>(st, 0));
    Station_BuildCatchment(st, 0, 0, -1);
    Station_CollectIndustries(st);
    if (Obj_Flag3d4(Station_BuildingOf(st)) == 1 || keepTrack) return;
    float angle = SceneObj_Angle2(Station_BuildingOf(st));
    float ox, oy;
    Type_RotatedCell(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(Station_BuildingOf(st), 0x3cc), &ox, &oy, angle);
    Collection* types = hook::Global<Collection*>(0x62b2fc);
    uint32_t type = F<uint32_t>(Station_BuildingOf(st), 0x3cc);
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    float sx, sy, sa;
    int32_t node;
    st[0x46a] = (uint8_t)Station_SnapToTrack(types, 0, F<int32_t>(st, 0), type, x, y, angle, &sx, &sy, &sa, 0, &node);
    auto claimedByOther = [&](int32_t n) {
        int32_t c = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)n), 0x222);
        return c != -1 && c != F<int32_t>(st, 0);
    };
    if (node != -1 && claimedByOther(node)) node = -1;
    st[0x46a] = node != -1;
    if (node != -1) {
        int32_t old = F<int32_t>(st, 8);
        if (old != -1) {
            if (Node_HasLink(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)old), 0, 1))
                F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8)), 0x222) = -1;
            else
                ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)hook::Global<Collection*>(0x6cfca8))[2])(
                    hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8));
        }
        F<int32_t>(st, 8) = node;
        Node_Set222(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)node), 0, F<int32_t>(st, 0));
        return;
    }
    if (F<int32_t>(st, 8) != -1 && Node_HasLink(Collection_Get(nodes, 0, F<uint32_t>(st, 8)), 0, 1)) {
        float tx, ty, ta;
        st[0x46a] = (uint8_t)Station_SnapToTrack(types, 0, F<int32_t>(st, 0), type, x, y, angle, &tx, &ty, &ta, 0, &node);
        if (node != -1) {
            if (!claimedByOther(node)) return;
            node = -1;
        }
        int32_t pieceKind = Obj_Flag3d4(Station_BuildingOf(st)) == 1 ? 2 : 0;
        int32_t owner = Obj_BuildingValue(st);
        float cy = (float)((double)oy + y), cx = (float)((double)x - ox);
        node = ((int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t))0x493cf0)(
            hook::Global<Collection*>(0x6cfca8), 0, cx, cy, -angle, pieceKind, owner, 1, 1);
        if (!Collection_IsValid(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)node)) return;
        F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8)), 0x222) = -1;
        F<int32_t>(st, 8) = node;
        Node_Set222(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8)), 0, F<int32_t>(st, 0));
        return;
    }
    float cx = (float)((double)x - ox), cy = (float)((double)oy + y);
    if (!Map_Contains(hook::Global<void*>(0x62c120), 0, cx, cy)) return;
    if (F<int32_t>(st, 8) == -1) {
        int32_t pieceKind = Obj_Flag3d4(Station_BuildingOf(st)) == 1 ? 2 : 0;
        int32_t force = Obj_Flag3d4(Station_BuildingOf(st)) != 1 ? 1 : 0;
        int32_t owner = Obj_BuildingValue(st);
        float pieceAngle = SceneObj_Angle2(Station_BuildingOf(st));
        F<int32_t>(st, 8) = ((int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t))0x493cf0)(
            hook::Global<Collection*>(0x6cfca8), 0, cx, cy, pieceAngle, pieceKind, owner, 1, force);
        if (F<int32_t>(st, 8) == -1) return;
        Node_Set222(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8)), 0, F<int32_t>(st, 0));
        return;
    }
    Track_PlaceRotated((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(st, 8)), 0, cx, cy, -angle);
}
RT3_REPLACE(0x480710, Station_Move);

// 0x480bb0 (thiscall, RET 8). Station s placed at (x, y): an unlinked track
// node (+8) of its building (+4, table 0x62b26c) is replaced by the stretch
// between the building's two attachment points (0x417910, turned by its
// angle) for the building's company, and the node there (within 0.724 of
// its cell 0x417840) taken (+0x222 = station id +0); the node's route is
// split. The station is named (+0x46b) after the nearest city (else 0x24f)
// plus the city's district suffix (+0x266, table 0x5f2d18) if it fits in 59
// characters, then a kind suffix: a type-3 building counts the city's halts
// (+0x26a, 0x24d, numbered from the second), building state 4 its yards
// (+0x26e, 0x24e), others take the next compass name (+0x262, table
// 0x5f2cf8, nine per district, three districts). Trailing blanks go. An
// active station updates the lots (0x6ada90) and, after the connection
// bonuses, starts the age clock of the city at its cell.
RT3_NOINLINE void __fastcall Station_Place(uint8_t* s, int, float x, float y) {
    auto building = [&]() -> uint8_t* {
        return F<int32_t>(s, 4) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(s, 4)) : nullptr;
    };
    if (F<int32_t>(s, 8) != -1) {
        ++hook::Global<int32_t>(0xccba14);
        Collection* tracks = hook::Global<Collection*>(0x6cfca8);
        if (!Node_HasLink(Collection_Get(tracks, 0, F<uint32_t>(s, 8)), 0, 1)) {
            BlVirt2(hook::Global<Collection*>(0x6cfca8), F<int32_t>(s, 8));
            if (F<int32_t>(s, 4)) Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(s, 4));
            const float angle = SceneObj_Angle2(building());
            float dx, dz;
            Obj_AttachmentPoint(hook::Global<Collection*>(0x62b2fc), 0, 0, F<uint32_t>(building(), 0x3cc), &dx, &dz, angle);
            const float ax = (float)((double)x - dx), ay = (float)((double)dz + y);
            Obj_AttachmentPoint(hook::Global<Collection*>(0x62b2fc), 0, 1, F<uint32_t>(building(), 0x3cc), &dx, &dz, angle);
            const float bx = (float)((double)x - dx), by = (float)((double)dz + y);
            TrackNet_LayLine(hook::Global<Collection*>(0x6cfca8), 0, ax, ay, bx, by, F<int32_t>(building(), 0x276), 2, 0, nullptr, nullptr, 0.0f);
            TrackList_ResetBuildState(hook::Global<uint8_t*>(0x6cfca8));
            Type_RotatedCell(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(building(), 0x3cc), &dx, &dz, angle);
            const int32_t id = Grid_NearestNodeWithin(hook::Global<Collection*>(0x6cfca8), 0, (float)((double)x - dx), (float)((double)dz + y), -1, 0.724f, 1, 0, 0);
            F<int32_t>(s, 8) = id;
            Node_Set222(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)id), 0, F<int32_t>(s, 0));
        }
        Routes_SplitAtNode(hook::Global<Collection*>(0x6cfcb4), 0, (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(s, 8)), 0);
        --hook::Global<int32_t>(0xccba14);
    }
    const int32_t iy = Ftol(y), ix = Ftol(x);
    char* name = (char*)s + 0x46b;
    const uint16_t cityId = Regions_NearestValue(ix, iy);
    if (!cityId) {
        strcpy(name, GetString(0x24f));
    } else {
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, cityId);
        char suffix[400], number[0x40], more[300];
        strcpy(name, (const char*)c + 0x356);
        suffix[0] = 0;
        if (F<int32_t>(c, 0x266)) {
            FormatText(suffix, (const char*)0x5ce908, GetString(((const int32_t*)0x5f2d18)[F<int32_t>(c, 0x266)]), nullptr, nullptr, nullptr, nullptr, nullptr,
                       nullptr, nullptr);
            if (strlen(suffix) + strlen(name) < 0x3b) strcat(name, suffix);
        }
        // A count from the second one on: " N".
        auto numbered = [&](int32_t count) {
            if (count <= 1) return;
            const char* num = Num_Format(number, 1, (double)count, 10, 1, 0, 0);
            FormatText(more, (const char*)0x5ce908, num, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            strcat(suffix, more);
        };
        uint8_t* b = building();
        uint8_t* type = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
        if (type[0x8c] == 3) {
            ++F<int32_t>(c, 0x26a);
            FormatText(suffix, GetString(0x24d), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            numbered(F<int32_t>(c, 0x26a));
        } else if (Obj_BuildingState8c(s) == 4) {
            ++F<int32_t>(c, 0x26e);
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(suffix, GetString(0x24e));
            numbered(F<int32_t>(c, 0x26e));
        } else {
            FormatText(suffix, (const char*)0x5ce908, GetString(((const int32_t*)0x5f2cf8)[F<int32_t>(c, 0x262)]), nullptr, nullptr, nullptr, nullptr, nullptr,
                       nullptr, nullptr);
            if (++F<int32_t>(c, 0x262) == 9) {
                F<int32_t>(c, 0x262) = 0;
                if (++F<int32_t>(c, 0x266) == 3) F<int32_t>(c, 0x266) = 0;
            }
        }
        strcat(name, suffix);
    }
    for (int32_t k = (int32_t)strlen(name); k > 0 && name[k - 1] == ' '; --k) name[k - 1] = 0;
    if (Obj_BuildingStateActive(s)) Lots_UpdateLinked(hook::Global<Collection*>(0x6ada90));
    Cities_PayConnectionBonuses();
    if (Obj_BuildingStateActive(s)) {
        const uint16_t here = (uint16_t)Map_CellCity(hook::Global<uint8_t*>(0x62c120), 0, ix, iy);
        if (Collection_IsValid(hook::Global<Collection*>(0x62bae0), 0, here))
            Obj_StartAgeClock(Collection_Get(hook::Global<Collection*>(0x62bae0), 0, here));
    }
}
RT3_REPLACE(0x480bb0, Station_Place);

// 0x4811c0 / 0x4811f0. Collection subclass 0x5ce90c (inline 0x60d-byte records, 10 + 20).
RT3_NOINLINE Collection* __fastcall Collection5ce90c_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce90c;
    Collection_Init(c, 0, 1, 0x60d, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x4811c0, Collection5ce90c_Construct);

void __fastcall Collection5ce90c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce90c;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x4811f0, Collection5ce90c_Destruct);

// 0x481210. Loads the collection (chunks 0x4a39..0x4a3b) and each element's arrays;
// old saves (version 0x4a39 with 0x465-byte records) get their +0x2ba block moved
// to +0x462 and the new fields cleared.
RT3_NOINLINE int32_t __fastcall Collection_LoadWithArrays(Collection* c, int, Stream s, int32_t) {
    int32_t ver;
    StreamRead(s, &ver, 4, 0, 1);
    Collection_Load(c, 0, s, ver == 0x4a39 ? 0x465 : 0);
    int32_t r = StreamSkip4(s, 0x4a3a, 0, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (ver == 0x4a39) {
            ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(e + 0x462, e + 0x2ba, 0x168);
            for (int k = 0; k < 0x35; ++k) F<int32_t>(e, 0x2ba + k * 4) = 0;
            for (int k = 0; k < 0x35; ++k) F<int32_t>(e, 0x38e + k * 4) = 0;
            for (int k = 0; k < 0x43; ++k) e[0x5ca + k] = 0;
        }
        Obj_LoadArrays(e, 0, s);
    }
    return StreamSkip4(s, 0x4a3b, 0, 0) + r;
}
RT3_REPLACE(0x481210, Collection_LoadWithArrays);

// 0x481310. Sites save: tags 0x4a9d/0x4a3a/0x4a3b with each site's extra data (0x47da40).
RT3_NOINLINE void __fastcall Sites_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x4a9d);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x4a3a);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Site_SaveExtra((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, s);
    StreamWriteU32(s, 0x4a3b);
}
RT3_REPLACE(0x481310, Sites_Save);

// 0x481390. Adds a station for `building` at cell (x, y) to table c; returns its id.
RT3_NOINLINE uint32_t __fastcall Stations_Add(Collection* c, int, uint32_t building, float x, float y) {
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x48139c)(
        c, 0, nullptr, 0, 0, 0);
    Station_Init((uint8_t*)Collection_Get(c, 0, id), 0, (int32_t)id, building, x, y);
    return id;
}
RT3_REPLACE(0x481390, Stations_Add);

// 0x4813d0. Deletes station `id`: removes it, frees the slot and relinks the
// companies' stations when it had a building of state 1 (not while
// +0x4c93).
void __fastcall Stations_Delete(Collection* c, int, uint32_t id) {
    uint8_t* s = (uint8_t*)Collection_Get(c, 0, id);
    const int32_t relink = Obj_Flag3d4(Obj_Building(s)) == 1 ? 1 : 0;
    Station_Remove(s);
    Collection_Release(c, 0, id);
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c93) && relink) Companies_BuildStationLinks(hook::Global<Collection*>(0x62be10));
}
RT3_REPLACE(0x4813d0, Stations_Delete);

// 0x481430. With option +0x1c7, the (empty) per-record hook 0x47d8d0.
void __fastcall Table_Hook47d8d0(Collection* t) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1c7)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop47d8d0(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x481430, Table_Hook47d8d0);

// 0x481480. Station_RecordNearby(obj) for every element of this collection.
void __fastcall Collection_RecordNearby(Collection* c, int, uint8_t* obj) {
    for (int32_t j = 0; j < Collection_Count(c); ++j)
        Station_RecordNearby((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0)), 0, obj);
}
RT3_REPLACE(0x481480, Collection_RecordNearby);

RT3_NOINLINE void __fastcall Table_RemovePairKey(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Pairs_RemoveKey((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, key);
}
RT3_REPLACE(0x4814c0, Table_RemovePairKey);

// 0x481500. Whether an active site (only siteId, if given) lies within 7 cells of building `bid`.
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId) {
    void* b = Collection_Get(g_buildings, 0, bid);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint32_t id = Collection_IdAt(t, 0, i, 0);
        if (siteId && siteId != id) continue;
        void* r = Collection_Get(t, 0, id);
        if (!Obj_BuildingStateActive(r)) continue;
        float ry = (float)Obj_BuildingCellY(r);
        float rx = (float)Obj_BuildingCellX(r);
        float by = (float)Obj_CellY(b);
        float bx = (float)Obj_CellX(b);
        float d = (float)Distance2D(bx, by, rx, ry);
        if (d <= *(const float*)0x5c8738) return 1;
    }
    return 0;
}
RT3_REPLACE(0x481500, Table_ActiveSiteNear);

// 0x4815d0. Whether some station accepting cargo `a` is connected by track to
// some station supplying cargo `b` (this = station collection): candidates are
// open stations (+0x3d4 == 1; from save version 0x67 also active ones) whose
// region (`aIsRegion` / `bIsRegion`: the region index equals a / b, else the
// cargo type's region list holds the station) and value (unless `value` is -1)
// match. 0 when a and b are the same.
RT3_NOINLINE int32_t __fastcall Stations_CargoLinked(Collection* sts, int, int32_t a, int32_t b, int32_t aIsRegion, int32_t bIsRegion, int32_t value) {
    if (a == b && aIsRegion == bIsRegion) return 0;
    int32_t n = Collection_Count(sts);
    if (!n) return 0;
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    uint32_t* listA = (uint32_t*)malloc_(n * 4);
    uint32_t* listB = (uint32_t*)malloc_(n * 4);
    memset(listA, 0, n * 4);
    memset(listB, 0, n * 4);
    int32_t cntA = 0, cntB = 0, result = 0;
    auto st = [&](int32_t i) { return (uint8_t*)Collection_Get(sts, 0, Collection_IdAt(sts, 0, i, 0)); };
    Collection* cargo = hook::Global<Collection*>(0x62bae0);
    for (int32_t i = 0; i < n; ++i) {
        if (Obj_Flag3d4(Obj_Building(st(i))) != 1) continue;
        if (Game_EffectiveVersion() >= 0x67 && !Obj_BuildingStateActive(st(i))) continue;
        uint32_t region = Obj_BuildingRegionIndex(st(i));
        bool ok = aIsRegion ? region == (uint32_t)a
                            : Obj_RegionHasKey(Collection_Get(cargo, 0, (uint32_t)a), 0, (uint16_t)Collection_IdAt(sts, 0, i, 0)) != 0;
        if (ok && (value == -1 || Obj_BuildingValue(st(i)) == value)) listA[cntA++] = Collection_IdAt(sts, 0, i, 0);
        ok = bIsRegion ? region == (uint32_t)b
                       : Obj_RegionHasKey(Collection_Get(cargo, 0, (uint32_t)b), 0, (uint16_t)Collection_IdAt(sts, 0, i, 0)) != 0;
        if (ok && (value == -1 || Obj_BuildingValue(st(i)) == value)) listB[cntB++] = Collection_IdAt(sts, 0, i, 0);
    }
    if (cntA && cntB) {
        for (int32_t x = 0; x < cntA && !result; ++x) {
            int32_t na = F<int32_t>(Collection_Get(sts, 0, listA[x]), 8);
            for (int32_t y = 0; y < cntB; ++y) {
                int32_t nb = F<int32_t>(Collection_Get(sts, 0, listB[y]), 8);
                if (na == nb) continue;
                uint32_t cost;
                Route_NextStep(hook::Global<Collection*>(0x6cfcb4), 0, na, nb, &cost, 0, nullptr, nullptr, 0);
                if (cost != 0xffffffff) {
                    result = 1;
                    break;
                }
            }
        }
    }
    Free(listA);
    Free(listB);
    return result;
}
RT3_REPLACE(0x4815d0, Stations_CargoLinked);

// 0x4818b0. Active sites (highlighted only, when `highlighted`).
RT3_NOINLINE int32_t __fastcall Table_CountActiveSites(Collection* t, int, int32_t highlighted) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (!Obj_BuildingStateActive(r)) continue;
        if (!highlighted || r[0x5bc]) ++n;
    }
    return n;
}
RT3_REPLACE(0x4818b0, Table_CountActiveSites);

// 0x481910. Station table: counts the trains heading for each station
// (Train_TargetStation) into +0x5c1.
RT3_NOINLINE void __fastcall Stations_CountTargets(Collection* stations) {
    for (int32_t i = 0; i < Collection_Count(stations); ++i)
        F<int32_t>(Collection_Get(stations, 0, Collection_IdAt(stations, 0, (uint32_t)i, 0)), 0x5c1) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfcbc)); ++i) {
        Collection* trains = hook::Global<Collection*>(0x6cfcbc);
        int32_t id = Train_TargetStation((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)));
        if (!id) continue;
        if (uint8_t* st = (uint8_t*)Collection_Get(stations, 0, (uint32_t)id)) ++F<int32_t>(st, 0x5c1);
    }
}
RT3_REPLACE(0x481910, Stations_CountTargets);

// 0x4819b0. Moves one reference (+0x5c1) from record `from` to record `to`.
void __fastcall Table_MoveRef(Collection* t, int, uint32_t from, uint32_t to) {
    if (from) {
        if (uint8_t* r = (uint8_t*)Collection_Get(t, 0, from)) F<int32_t>(r, 0x5c1)--;
    }
    if (to) {
        if (uint8_t* r = (uint8_t*)Collection_Get(t, 0, to)) F<int32_t>(r, 0x5c1)++;
    }
}
RT3_REPLACE(0x4819b0, Table_MoveRef);

// 0x493cf0. Adds a turned track piece (Track_InitRotated around cell (cx, cy)) to
// table c: the square (half-size 0.181) must lie inside the map; unless `force`, a
// valid owner must have pieces left. Charges the owner, stamps the group, registers
// the piece and sets the table's first group as in Tracks_AddPiece. Returns its id
// or -1. NOT INSTALLED, for the same reason as Tracks_AddPiece (record copied from
// a mostly uninitialized local).
RT3_NOINLINE int32_t __fastcall Tracks_AddPieceRotated(Collection* c, int, float cx, float cy, float angle, int32_t kind, int32_t owner,
                                          int32_t style, int32_t force) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    const double r = D(0x5c97d8);
    float w = (float)F<int32_t>(m, 0x2155), h = (float)F<int32_t>(m, 0x2159);
    if ((double)cx - r < 0.0 || !((double)w > (double)cx + r || w != w)) return -1;
    if ((double)cy - r < 0.0 || !((double)h > (double)cy + r || h != h)) return -1;
    uint16_t co = (uint16_t)owner;
    if (!force && Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, co) &&
        !Company_Get7680(Collection_Get(hook::Global<Collection*>(0x62be10), 0, co)))
        return -1;
    uint8_t local[0x260] = {};
    TrackLight_Construct(local);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x493de4)(
        c, 0, local, 0x257, 0, 0);
    if (co) Company_Add7680(Collection_Get(hook::Global<Collection*>(0x62be10), 0, co), 0, (uint8_t)style != 4 ? -1 : -2);
    uint8_t* t = (uint8_t*)Collection_Get(c, 0, id);
    F<int32_t>(c, 0x90) += 2;
    Track_InitRotated(t, 0, (int32_t)id, F<int32_t>(c, 0x90), cx, cy, angle, kind, owner, style);
    if (t[0x216] || F<int32_t>(c, 0xe9)) Grid_AddObject((uint8_t*)c, 0, t);
    if (F<int32_t>(c, 0x88) == -1) F<int32_t>(c, 0x88) = F<int32_t>(t, 0x240);
    Effect489e80_Release(local);
    return (int32_t)id;
}

// 0x4819f0. The system report (static buffer 0x6cec88), built in three passes
// that each format into the buffer with the previous text as %1 (strings 0x2f1..
// 0x2f3): game version (file 0x5cec18, major.minor), OS, process priority, CPU
// vendor / name / speed, MMX / SSE / SSE2 / 3DNow (yes/no, strings 0xb / 0xc), the
// cycle counter text, video card, total and free memory and the disk reads /
// writes (always 0) in MB.
const char* SysInfo_Report() {
    char* out = (char*)0x6cec88;
    auto yesNo = [](uint32_t f) { return GetString(f ? 0xb : 0xc); };
    char speed[0x50], minor[0x50], major[0x50];
    const char* mmx = yesNo(SysInfo_HasFeature0());
    Num_Format(speed, 1, (double)(int32_t)SysInfo_CpuSignature(), 10, 1, 0, 0);
    const char* name = SysInfo_CpuName();
    const char* vendor = SysInfo_CpuVendor();
    const char* priority = Error_Message(Sys_Call5c81d0());
    const char* os = Os_VersionName();
    Num_Format(minor, 1, (double)(File_Version((const char*)0x5cec18) & 0xffff), 10, 1, 0, 0);
    Num_Format(major, 1, (double)(File_Version((const char*)0x5cec18) >> 16), 10, 1, 0, 0);
    FormatText(out, GetString(0x2f1), major, minor, os, priority, vendor, name, speed, mmx);
    const char* f3 = yesNo(SysInfo_HasFeature3());
    const char* f2 = yesNo(SysInfo_HasFeature2());
    const char* f1 = yesNo(SysInfo_HasFeature1());
    char freeMb[0x50], totalMb[0x50];
    Num_Format(freeMb, 1, (double)(SysInfo_FreeMemory() >> 20), 10, 1, 0, 0);
    Num_Format(totalMb, 1, (double)(SysInfo_TotalMemory() >> 20), 10, 1, 0, 0);
    FormatText(out, GetString(0x2f2), out, f1, f2, f3, (const char*)0xd97668, SysInfo_DisplayName(), totalMb, freeMb);
    char writes[0x50], reads[0x50];
    Num_Format(writes, 1, (double)(SysInfo_Zero64b() >> 20), 10, 1, 0, 0);
    Num_Format(reads, 1, (double)(SysInfo_Zero64a() >> 20), 10, 1, 0, 0);
    FormatText(out, GetString(0x2f3), out, reads, writes, 0, 0, 0, 0, 0);
    return out;
}
RT3_REPLACE(0x4819f0, SysInfo_Report);

// 0x481d00. Parses the command line switches: -S<n> screen mode (1,2,3,4,9 else 1),
// -L logging, -H host, -N "name" player name, -P "name" password, -C name join
// address. Logging (0x6d3ca0) ends up on in any case.
void __fastcall Cmdline_Parse(const char* p) {
    int32_t named = 0, passworded = 0;
    if (*p) {
        for (;;) {
            char c = *p;
            if (c == '-' || c == '/') {
                char sw = p[1];
                ++p;
                switch (sw) {
                case 'S': case 's': {
                    int32_t v = p[1] - '0';
                    hook::Global<int32_t>(0x6cec6c) = v;
                    if (v != 1 && v != 2 && v != 4 && v != 3 && v != 9) {
                        v = 1;
                        hook::Global<int32_t>(0x6cec6c) = 1;
                    }
                    hook::Global<int32_t>(0x6cec70) = v;
                    hook::Global<int32_t>(0x6d4028) = v;
                    break;
                }
                case 'L': case 'l':
                    hook::Global<int32_t>(0x6d3ca0) = 1;
                    break;
                case 'H': case 'h':
                    hook::Global<int32_t>(0x6cd920) = 1;
                    hook::Global<int32_t>(0x6ae3c4) = 0;
                    hook::Global<int32_t>(0x6ae1c0) = 1;
                    if (!named) memset((void*)0x6ae2c4, 0, 0x100);
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    break;
                case 'N': case 'n':
                    memset((void*)0x6ae2c4, 0, 0x100);
                    named = 1;
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    if (!(p = SwitchName(p, (char*)0x6ae2c4))) goto done;
                    break;
                case 'P': case 'p':
                    memset((void*)0x6ae3cc, 0, 0x100);
                    passworded = 1;
                    if (!(p = SwitchName(p, (char*)0x6ae3cc))) goto done;
                    break;
                case 'C': case 'c': {
                    hook::Global<int32_t>(0x6ae3c4) = 0;
                    hook::Global<int32_t>(0x6ae1c0) = 0;
                    hook::Global<int32_t>(0x6cd920) = 1;
                    memset((void*)0x6ae1c4, 0, 0x100);
                    if (!passworded) memset((void*)0x6ae3cc, 0, 0x100);
                    if (!named) memset((void*)0x6ae2c4, 0, 0x100);
                    int32_t n = 0;
                    char ch = *p;
                    if (!ch) goto done;
                    bool stop = false;
                    do {
                        if (ch == ' ') {
                            if (n) {
                                stop = true;
                                break;
                            }
                            ((char*)0x6ae1c4)[0] = *++p;
                            n = 1;
                        } else if (n) ((char*)0x6ae1c4)[n++] = ch;
                        ch = *++p;
                    } while (ch);
                    if (!stop) goto done;
                    break;
                }
                }
            }
            c = *p;
            if (!c) break;
            if (c != '-' && c != '/') ++p;
            if (!*p) break;
        }
    }
done:
    hook::Global<int32_t>(0x6d3ca0) = 1;
}
RT3_REPLACE(0x481d00, Cmdline_Parse);

int32_t __fastcall Return1_481fb0(void*) { return 1; }
RT3_REPLACE(0x481fb0, Return1_481fb0);

// 0x481fd0. Start of the game object's setup. It finds the game CD: the drive
// 0x625a90 holding "_Autorun\rt3d1.txt" (or rt3d2.txt). QUIRK: a disc-1 hit
// keeps scanning, so the last such drive wins, while a disc-2 hit stops at
// the first. Then it clears the object's state, sets the handler 0xd97968,
// starts the streams and loads the language. For the Taiwanese text the
// table 0x5ee8e0 is filled with 'c'; under language 12 this exe refuses to
// run.
RT3_NOINLINE uint8_t* __fastcall Game_InitStart(uint8_t* g) {
    auto driveType = Import<uint32_t(__stdcall*)(const char*)>(0x5c806c);
    auto open_ = (int(__cdecl*)(const char*, int))0x5a4c57;
    auto close_ = (int(__cdecl*)(int))0x5a440d;
    int32_t disc1 = 0;
    for (int32_t d = 0; d < 26; ++d) {
        char root[4];
        F<uint32_t>(root, 0) = 0x5c3a41;
        root[0] = (char)('A' + d);
        if (driveType(root) != 5) continue;
        char path[0x1e8];
        FormatText(path, (const char*)0x5ceca4, root, 0, 0, 0, 0, 0, 0, 0);
        int fd = open_(path, 0x8000);
        if (fd != -1)
            disc1 = 1;
        else {
            FormatText(path, (const char*)0x5cec8c, root, 0, 0, 0, 0, 0, 0, 0);
            fd = open_(path, 0x8000);
            if (fd == -1) continue;
        }
        close_(fd);
        hook::Global<int32_t>(0x625a90) = d;
        if (!disc1) break;
    }
    hook::Global<uint32_t>(0xd97968) = 0x481fc0;
    F<int32_t>(g, 0x18) = 0;
    F<int32_t>(g, 0x64) = 0;
    F<int32_t>(g, 0x501) = 0;
    memset(g + 0x1c, 0, 0x11 * 4);
    StreamInit();
    Language_Load();
    if (StrFind(GetString(0x5be), (const char*)0x5cec80, 0)) memset((void*)0x5ee8e0, 0x63, 0x25b * 4 + 3);
    if (hook::Global<int32_t>(0x6d3c84) == 0xc) Fatal_Exit((const char*)0x5cec20);
    return g;
}
RT3_REPLACE(0x481fd0, Game_InitStart);

}  // namespace rt3
