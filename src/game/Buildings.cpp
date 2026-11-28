// Buildings.cpp - RT3.exe 0x40c000-0x414000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/stream.h"
#include "core/containers.h"
#include "core/ptrlist.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);

void __fastcall ModelObj_Select(uint8_t* o);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);

 // region types

int32_t __fastcall Obj_BuildingStateActive(void* o);
int __fastcall ToUpper(char c);
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);
void* __fastcall Obj_Building(void* o);
void __fastcall Obj_RefreshHighlight(void* o);
int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_CheckActive(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Obj_StateB9(void* o);
void* __fastcall Obj_Ref3cc(void* o);
int32_t __fastcall Obj_Available(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
int32_t __stdcall Bracket5(int32_t value);
void __fastcall Obj_SetPriority(void* o);
uint8_t* __fastcall Obj_Region(void* o);
void __fastcall Obj_TouchRegionSites(void* o);
uint32_t __fastcall Obj_NodeNotify(uint8_t* o, int, int32_t flag);
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value);
void __fastcall Save_LogProgress(Stream s, const char* label);
uint8_t* __fastcall Obj_RegionAt(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid);
void __fastcall Building_DropSharedDemand(uint8_t* b, int, uint16_t k);
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId);
uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void __fastcall Industry_BuildCargoLists(uint8_t* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);


double __fastcall Obj_Remaining(void* o, int, int32_t i);
int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag);
void __fastcall Obj_UpdateEntryFlags(void* o);

void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare);

double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p);
void __fastcall Obj_SetAllBc(void* o, int32_t v);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Store3Globals(int32_t* v);
void __fastcall Obj_ApplyStored3(void* o, int32_t idx);
void __fastcall Obj_FreeEffects(uint8_t* o, int, int32_t force);
int32_t __fastcall Shape_Bounds(void* self, int, uint8_t* shape, int32_t* out);
void __fastcall Obj_ResolveType3ae(uint8_t* o);
void __fastcall Lot_Create(uint32_t src, uint32_t dst, int32_t kind);
uint32_t __stdcall HeatColor(float t);
float __fastcall Building_ProfitScore(uint8_t* b);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
float __fastcall Obj_Mid1ed(uint8_t* o);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t IsMultiplayer();
double __fastcall Map_NearestLandHeight(uint8_t* m, int, int32_t x, int32_t y, int32_t r);
void __fastcall Obj_SettleOnNode(uint8_t* o);
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
int32_t __fastcall Game_EffectiveVersion();
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
const char* __fastcall LocalizeName(const char* name);
uint32_t __fastcall Obj_GridByte(void* o);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Table_RemovePairKey(Collection* t, int, int32_t key);
void __fastcall City_RemoveBuilding(uint8_t* c, int, uint16_t id);
void __fastcall City_AddBuilding(uint8_t* c, int, uint16_t id);
void __fastcall Collection_RecordNearby(Collection* c, int, uint8_t* obj);
void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind);
const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
void* __fastcall Obj_OwnerCompany(void* o);
double __fastcall Recipe_NetValue(void* o, int, uint8_t* rec);
void __fastcall Building_SpawnLot(uint8_t* b, int, int32_t kind, float factor);
void __fastcall Cargo_MoveTo62(void* c, int, float a);
void __fastcall Obj_AddLevel(void* o, int, int32_t i, float f);
double __fastcall City_ProduceCargo(uint8_t* o, int, int32_t i, float amt);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall BuildingType_Load(uint8_t* o, int, const char* path, int32_t id);
void __fastcall Industries_RebuildPortsWarehouses(Collection* c);
void __fastcall Table_SetPriorities(Collection* t);
int32_t __fastcall Obj_Get21(void* o);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall Table_RefreshAll(Collection* t);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void* __fastcall Obj_Node8(void* o);
void __fastcall Track_UpdateColor(uint8_t* o);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t Global6d07b4Set2();
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
uint32_t __fastcall Company_Logo(uint8_t* c);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Node_HasLink(void* n, int, int32_t any);
void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
void __fastcall Effect_Free(uint32_t* e);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
void __fastcall ModelObj_Release(uint8_t* o);
void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
int32_t __fastcall StreamSkip4(int32_t s, int tag, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, int32_t s, int32_t recordSize);
void __fastcall Obj_Reground(uint8_t* o);
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name);
void* __fastcall Obj_Ref2a8(void* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __stdcall SetGlobal5ee030(int32_t v);
void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v);
void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
uint32_t __fastcall Stations_Add(Collection* c, int, uint32_t building, float x, float y);
int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Obj_PushBuilding(void* o, int, void* b);
void __fastcall Building_RegisterWithSites(uint8_t* b);
uint8_t* __fastcall ModelObj40c950_Construct(uint8_t* o);
void __fastcall ModelObj40c950_Destructor(uint8_t* o);
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
void RunDebugValidation();
int32_t GetQueueCount62ba64();
int32_t PopQueue62ba70();
void __fastcall Obj_RestoreSite(uint8_t* o);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall Overview_Close(uint8_t* t);
int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
uint8_t* __fastcall Sites_Create(uint8_t* mgr, int, const char* name, float x, float y, float angle, uint8_t* o, uint32_t* height);
uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed);
void __fastcall Type_RotatedOffset(Collection* t, int, uint32_t id, int32_t i, float* outX, float* outY, float angle);
float __fastcall Building_RoundedValue(uint32_t id, int, float a);
void __fastcall Industry_Produce(uint8_t* ind, int, int32_t bulk);
void __fastcall Buildings_DropSharedDemand(Collection* t, int, uint16_t k);
uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE float __fastcall Obj_NodeCenterX(void* o);
RT3_NOINLINE float __fastcall Obj_NodeCenterY(void* o);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
RT3_NOINLINE void __fastcall CargoTypes_MarkAvailable(Collection* c);
RT3_NOINLINE int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall Buildings_Load(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Buildings_RestoreQueued(Collection* t);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall Obj_FreeBuffers(void* o);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

uint8_t Global622b0cIs64();
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
int32_t Option2bfActive();
void __fastcall Obj_SaveNamedPair(uint8_t* o, int, Stream s);
void __fastcall Entries6_Remove(uint8_t* o, int, int16_t key);
void __fastcall Table_ReleaseKeyAny(Collection* t, int, int32_t key);
void __fastcall Chain_Unlink(uint8_t* o, int, uint8_t* r);
void __fastcall Nop436050(void*, int, int32_t);
int32_t __fastcall Obj_TypeValue(uint8_t* o, int, int32_t* out);
int32_t __fastcall Obj_TypeRecord(uint8_t* o, int, char* name, int32_t* a, int32_t* b, int32_t* count);
float __fastcall Obj_NodeCenterX(void* o);
float __fastcall Obj_NodeCenterY(void* o);
void __stdcall Map_ClearFineBits(const uint32_t* cells);
void __stdcall Map_SetFineBits(const uint32_t* cells);
void __fastcall StationWindow_PickCargo(uint8_t* w, int, int32_t cargo);
int32_t __fastcall Building_ChangeType(uint8_t* b, int, uint32_t type, int32_t check, float* cost, char* text);
int32_t __fastcall Records_CountClass3(Collection* c);
int32_t __fastcall Records_CountClass4(Collection* c);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
template <typename T> static inline T& F_runtime_wrappers(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

// Functions of this file, in exe order.
RT3_NOINLINE uint8_t* __fastcall ModelObj40c950_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall ModelObj40c950_Destructor(uint8_t* o);
void __fastcall Thunk40c980(uint8_t* p0, int p1, Stream p2);
RT3_NOINLINE uint8_t __fastcall Obj_Flag3d4(void* o);
RT3_NOINLINE void __fastcall Building_RollStats(uint8_t* o);
RT3_NOINLINE void __fastcall Nop40ca60(void*);
RT3_NOINLINE double __fastcall Obj_Float2c0(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float2e8(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float310(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float338(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Float360(void* o, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Remaining(void* o, int, int32_t i);
void __fastcall Obj_Set2a2(void* o, int, uint16_t v);
uint32_t __fastcall Obj_SetWorld66ba(void* o, int, int32_t v);
float __fastcall Const5c8900_40cb40(void*);
int32_t __fastcall Obj_Slot68Rounded_40cb50(uint8_t* o);
int32_t __fastcall Obj_Slot68Rounded_40cb60(uint8_t* o);
RT3_NOINLINE uint32_t __fastcall Obj_GridByte(void* o);
RT3_NOINLINE uint32_t __fastcall Obj_AnimFrame(void* o);
void __fastcall Obj_SetByte42_40cbc0(uint8_t* o);
void __fastcall Obj_SetByte42_40cbd0(uint8_t* o);
int32_t __fastcall Return0_40cbe0(void*);
int32_t __fastcall Obj_Byte42_40cbf0(uint8_t* o);
int32_t __fastcall Return1_40cc00(void*);
int32_t __fastcall Return0_40cc10(void*);
int32_t __fastcall Return0_40cc20(void*);
int32_t __fastcall Return0_40cc30(void*);
int32_t __fastcall Obj_TypeKindIs1(void* o);
int32_t __fastcall Obj_TypeByteBa(void* o);
int32_t __fastcall Obj_TypeKind1Class3(void* o);
int32_t __fastcall Obj_Has2a8(uint8_t* o);
int32_t __fastcall Return1_40ccc0(void*);
RT3_NOINLINE uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t __fastcall Obj_BuildingState8c(void* o);
void __fastcall Obj_TouchRegionSites(void* o);
RT3_NOINLINE void __fastcall Obj_ResolveType3ae(uint8_t* o);
int32_t __fastcall Building_LoadRenamed(uint8_t* o, int, int32_t s);
RT3_NOINLINE void* __fastcall Obj_Ref3cc(void* o);
void* __fastcall Obj_Station3d0(uint8_t* o);
void __fastcall Obj_ShowConstructCursor(uint8_t* o, int, float scale);
RT3_NOINLINE void __fastcall ModelObj_SelectTrackTool(uint8_t* object);
void __fastcall Building_RefreshUpgrade(uint8_t* o);
void __fastcall IndustryObj_DropModel(uint8_t* o);
RT3_NOINLINE void* __fastcall Obj_Ref2a8(void* o);
RT3_NOINLINE void* __fastcall Obj_OwnerCompany(void* o);
RT3_NOINLINE int32_t __fastcall Obj_CheckActive(void* o);
void __fastcall Building_DropSharedDemand(uint8_t* b, int, uint16_t k);
RT3_NOINLINE void __fastcall Obj_RefreshHighlight(void* o);
RT3_NOINLINE int32_t __fastcall Obj_StateB9(void* o);
RT3_NOINLINE int32_t __fastcall Obj_StateKindIs4(void* o);
void __fastcall Building_ChargeUpkeep(uint8_t* b);
float __fastcall Model_FrameTimeWithAttached(uint8_t* o);
RT3_NOINLINE float __fastcall Building_Price(uint8_t* b, int, uint32_t company);
RT3_NOINLINE void __fastcall Building_RegisterWithSites(uint8_t* b);
uint32_t __fastcall Obj_NotifySiteOn(uint8_t* o);
uint32_t __fastcall Obj_NotifySiteOff(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Building_ChangeType(uint8_t* b, int, uint32_t type, int32_t check, float* cost, char* text);
int32_t __fastcall Obj_NotActiveSite(void* o);
void __fastcall Building_Teardown(uint8_t* b);
void __fastcall Obj_SettleOnNode(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_RestoreSite(uint8_t* o);
void __fastcall StationBuilding_Hover(uint8_t* o);
void __fastcall Station_OnStateChanged(uint8_t* b);
int32_t __fastcall OwnedObj_Tooltip(uint8_t* o, int, char* dst, int32_t n, int32_t a, int32_t b, int32_t logo);
RT3_NOINLINE void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
RT3_NOINLINE void __fastcall Obj_Reground(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
RT3_NOINLINE void __fastcall Building_Setup(uint8_t* o, int, int32_t owner, uint8_t* t, float x, float y, uint32_t angle, uint8_t place, int32_t a7);
RT3_NOINLINE uint8_t* __fastcall Obj_RegionAt(void* o);
RT3_NOINLINE double __fastcall Recipe_NetValue(void* o, int, uint8_t* rec);
uint32_t __fastcall Obj_TypeQuery(void* o, int, int32_t a);
double __fastcall Building_CargoOutput(uint8_t* b, int, int32_t cargo);
int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid);
float __fastcall Building_ProfitScore(uint8_t* b);
RT3_NOINLINE void __fastcall Building_SpawnLot(uint8_t* b, int, int32_t kind, float factor);
RT3_NOINLINE void __fastcall Industry_Produce(uint8_t* ind, int, int32_t bulk);
RT3_NOINLINE void __fastcall Obj_FreeBuffers(void* o);
void __fastcall Obj_UpdateEntryFlags(void* o);
RT3_NOINLINE int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o);
RT3_NOINLINE void __fastcall Obj_Nop411d90(void*);
int32_t __fastcall Obj_StaticEntry(void* o, int, char* name, int32_t* a, int32_t* b);
RT3_NOINLINE int32_t __fastcall Obj_TypeValue(uint8_t* o, int, int32_t* out);
RT3_NOINLINE int32_t __fastcall Obj_TypeRecord(uint8_t* o, int, char* name, int32_t* a, int32_t* b, int32_t* count);
void __fastcall Industry_BuildCargoLists(uint8_t* o);
RT3_NOINLINE void __fastcall BuildingType_Load(uint8_t* o, int, const char* path, int32_t id);
int32_t __fastcall Obj_Available(void* o);
RT3_NOINLINE int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag);
RT3_NOINLINE int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag);
void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share, float* in, float* inShare);
uint32_t __fastcall Obj_FindRecord62b2fc(void* o);
void __fastcall Obj_SetPriority(void* o);
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p);
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p);
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo);
RT3_NOINLINE void __fastcall Collection5c93cc_Destruct(Collection* c);
RT3_NOINLINE void __fastcall Table_SetPriorities(Collection* t);
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);
RT3_NOINLINE void* __fastcall Table_GetByName(Collection* t, int, const char* name);
RT3_NOINLINE void __fastcall Industries_BuildCargoLists(Collection* c);
RT3_NOINLINE void __fastcall Types_RefreshAvailable(Collection* c);
RT3_NOINLINE void __fastcall Types_PickName(Collection* t, int, char* dst, int32_t value);
RT3_NOINLINE void __fastcall Industries_RebuildPortsWarehouses(Collection* c);
RT3_NOINLINE void __fastcall BuildingTypes_LoadAll(Collection* t);
RT3_NOINLINE Collection* __fastcall BuildingTypes_Construct(Collection* t);
RT3_NOINLINE Collection* __fastcall Collection5c9414_Construct(Collection* c);
void __fastcall Collection5c9414_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Buildings_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Buildings_RestoreQueued(Collection* t);
RT3_NOINLINE void __fastcall Buildings_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
void __fastcall Buildings_Remove(Collection* c, int, uint32_t id);
void __fastcall Industries_ProduceQuarter(Collection* t);
void __fastcall Table_RefreshAll(Collection* t);
void __fastcall Cities_ColorBooms(Collection* c, int, int32_t value);
void __fastcall Buildings_ColorProfit(Collection* c);
void __fastcall Buildings_RefreshHighlight(Collection* t, int, int32_t key);
void __fastcall Table_TouchRegionSites(Collection* t);
RT3_NOINLINE void __fastcall Buildings_DropSharedDemand(Collection* t, int, uint16_t k);
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v);
int32_t __fastcall Table_SumCapacity(Collection* t, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Table_CountActive(Collection* t);
RT3_NOINLINE int32_t __fastcall Records_CountClass3(Collection* c);
RT3_NOINLINE int32_t __fastcall Records_CountClass4(Collection* c);
RT3_NOINLINE int32_t __fastcall Table_CountKind4Idle(Collection* t);
void __fastcall BuildingType_Model2D(uint8_t* o, int, char* out);
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y);
int32_t __fastcall Obj_TypeAvailable(void* o);
int32_t __cdecl DirtyCells_AddImpl(int32_t a, int32_t x, int32_t y);
void DirtyCells_Add();
void* __fastcall FindStaticRecord(void*, int, uint8_t* obj);
RT3_NOINLINE void FreeBuffer62b2f8();
RT3_NOINLINE int32_t PopQueue62ba70();
RT3_NOINLINE int32_t __stdcall Sites_LoadTable(Stream s);
RT3_NOINLINE void __stdcall Map_ClearFineBits(const uint32_t* cells);
RT3_NOINLINE void __stdcall Map_SetFineBits(const uint32_t* cells);
RT3_NOINLINE uint8_t* __fastcall Sites_Restore(uint8_t* mgr, int, uint32_t id, uint8_t* o, int32_t);
RT3_NOINLINE uint8_t* __fastcall Sites_Create(uint8_t* mgr, int, const char* name, float x, float y, float angle, uint8_t* o, uint32_t* height);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE void __fastcall Chain_Unlink(uint8_t* o, int, uint8_t* r);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __fastcall Nop436050(void*, int, int32_t);
RT3_NOINLINE uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
RT3_NOINLINE void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE double __fastcall Obj_CellX(void* o);
RT3_NOINLINE double __fastcall Obj_CellY(void* o);
float __fastcall Model_FrameTime(uint8_t* o);
RT3_NOINLINE void __fastcall ModelObj_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall ModelObj_Release(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_SaveNamedPair(uint8_t* o, int, Stream s);
RT3_NOINLINE int32_t __fastcall Building_Load(uint8_t* o, int, int32_t s);
RT3_NOINLINE void __fastcall Building_AddUpgradeModel(uint8_t* o);
RT3_NOINLINE void __fastcall BuildingObj_DropModel(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed);
RT3_NOINLINE void __fastcall Entries6_Remove(uint8_t* o, int, int16_t key);
RT3_NOINLINE void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind);
RT3_NOINLINE void __fastcall Station_Place(uint8_t* s, int, float x, float y);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall StationWindow_PickCargo(uint8_t* w, int, int32_t cargo);


static inline uint8_t* TypeOf173(void* o) { return (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(o, 0x173)); }

// 0x40dba0 / 0x40dbf0. With virtual slot 28, notifies the site's node
// (slot 1 / 2) and sets +0x42 to 0 / 1.
static uint32_t NotifySite(uint8_t* o, int32_t flag) {
    uint32_t r = ((uint32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o);
    if (r) {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        uint8_t* site = id ? (uint8_t*)Collection_Get(g_stations, 0, id) : nullptr;
        r = Obj_NodeNotify(site, 0, flag);
    }
    o[0x42] = flag ? 0 : 1;
    return r;
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

static inline uint8_t* World() { return g_world; }

static inline uint8_t* Map() { return g_map; }

// Formats string id with one argument and exits with it.
static void FatalFormat(int32_t id, const char* arg) {
    char msg[0x12c];
    FormatText(msg, GetString(id), arg, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Fatal_Exit(msg);
}

static inline uint8_t* Packs() { return g_pack; }

static inline void Format0(char* dst, uint32_t fmt, const char* a1 = nullptr) {
    FormatText(dst, (const char*)fmt, a1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

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

static inline void** Vt(void* o) { return *(void***)o; }

static inline double Fl_188(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline double Fl_189(uint32_t a) { return *(const float*)rt3::Rdata(a); }

// Raises the record's minimum year (+0x20, s16) to that of cargo `id`.
static void RaiseYear(uint8_t* rec, uint32_t id) {
    int32_t v = F<int32_t>(Collection_Get(g_cargoTypes, 0, id), 0x21);
    int32_t own = F<int16_t>(rec, 0x20);
    if (own > v) v = own;
    F<int16_t>(rec, 0x20) = (int16_t)v;
}

static inline double Fl_202(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline double Fl_205(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double AtLeastMilli(float v) { return v < D(0x5c8678) ? 0.001 : (double)v; }

static inline double Fl_207(uint32_t a) { return *(const float*)rt3::Rdata(a); }

namespace {
union Slot32 {
    float f;
    int32_t i;
    uint32_t u;
};
inline uint8_t* BuildingType(uint8_t* b) { return ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b); }
inline int32_t BuildingIsOwned(uint8_t* b) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)b)[0x70 / 4])(b); }
// The original's unsigned divide by 0x1b5e (multiply by 0x4f8b588f, 33-bit).
inline uint32_t BuildingSparkCount(uint32_t v) {
    const uint32_t hi = (uint32_t)(((uint64_t)v * 0x4f8b588fu) >> 32);
    return (((v - hi) >> 1) + hi) >> 16;
}
}

namespace {
uint32_t Message_Dialog444(const char* text) {
    return Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// The current frame time of model o (Model_FrameTime), 0 without a frame.
uint32_t Model_FrameTimeBits(uint8_t* o, int* has) {
    *has = 0;
    uint8_t* list = F<uint8_t*>(o, 0x14);
    if (!list) return 0;
    uint8_t* rec = F<uint8_t*>(list + 24 * F<int32_t>(o, 0x3a), 0xc) + F<int32_t>(o, 0x21) * 0x45;
    if (!rec) return 0;
    *has = 1;
    return F<uint32_t>(rec, 0x18);
}
}

namespace {
// Counts the records of c whose building state is 1, that pass slot 0x70
// and whose city's building class (slot 0x80, +0x8c) is `cls`.
int32_t Count_RecordsOfClass(Collection* c, uint8_t cls) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        if (Obj_Flag3d4(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))) != 1) continue;
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)r)[0x70 / 4])(r)) continue;
        uint8_t* b = (uint8_t*)Obj_Building(Obj_Ref2a8(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))));
        if (((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b)[0x8c] == cls) ++n;
    }
    return n;
}
// Rebuilds a vote state from packet d: two 16-bit values (+4 / +5 bytes),
// optionally a third (+6), then per player slot a "present" mark and a
// "for" mark (inactive or voting in `mask`).
void Vote_BuildState(uint8_t* st, const uint8_t* d, uint32_t mask, uint32_t present, uint32_t voted) {
    F<uint16_t>(st, 4) = d[4];
    F<uint16_t>(st, 6) = d[5];
    for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
        st[present + i] = 1;
        Collection* c = g_players;
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!F<int32_t>(e, 0x1e1) || !F<int32_t>(e, 4) || (mask & (1u << i))) st[voted + i] = 1;
    }
}
}

RT3_NOINLINE uint8_t* __fastcall ModelObj40c950_Construct(uint8_t* o) {
    ModelObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5c8c50;
    return o;
}
RT3_REPLACE(0x40c950, ModelObj40c950_Construct);

// 0x40c970 / 0x41ab00. Destructors of the 0x40c950 model and the 0x41aae0 part
// model subclasses.
RT3_NOINLINE void __fastcall ModelObj40c950_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5c8c50;
    ModelObj_Destructor(o);
}
RT3_REPLACE(0x40c970, ModelObj40c950_Destructor);

// 0x40c980 (vtable stub).
// Forwards to Obj_SaveNamedPair (0x45b560).
void __fastcall Thunk40c980(uint8_t* p0, int p1, Stream p2) { Obj_SaveNamedPair(p0, p1, p2); }
RT3_REPLACE(0x40c980, Thunk40c980);

RT3_NOINLINE uint8_t __fastcall Obj_Flag3d4(void* o) { return F<uint8_t>(o, 0x3d4); }
RT3_REPLACE(0x40c990, Obj_Flag3d4);

// 0x40c9a0. Building year roll-over: adds this year's five stats (+0x2c0,
// +0x2e8, ... first entries) to the totals +0x2ac.. and shifts the histories.
RT3_NOINLINE void __fastcall Building_RollStats(uint8_t* o) {
    AddInto(o, 0x2ac, 0x2c0);
    F<float>(o, 0x2b0) = (float)((double)F<float>(o, 0x2e8) + F<float>(o, 0x2b0));
    AddInto(o, 0x2b4, 0x310);
    AddInto(o, 0x2b8, 0x338);
    AddInto(o, 0x2bc, 0x360);
    ShiftHistories(o, 0x2c0);
}
RT3_REPLACE(0x40c9a0, Building_RollStats);

RT3_NOINLINE void __fastcall Nop40ca60(void*) { __asm nop }
RT3_REPLACE(0x40ca60, Nop40ca60);

RT3_NOINLINE double __fastcall Obj_Float2c0(void* o, int, int32_t i) { return F<float>(o, 0x2c0 + i * 4); }
RT3_REPLACE(0x40ca70, Obj_Float2c0);

RT3_NOINLINE double __fastcall Obj_Float2e8(void* o, int, int32_t i) { return F<float>(o, 0x2e8 + i * 4); }
RT3_REPLACE(0x40ca80, Obj_Float2e8);

RT3_NOINLINE double __fastcall Obj_Float310(void* o, int, int32_t i) { return F<float>(o, 0x310 + i * 4); }
RT3_REPLACE(0x40ca90, Obj_Float310);

RT3_NOINLINE double __fastcall Obj_Float338(void* o, int, int32_t i) { return F<float>(o, 0x338 + i * 4); }
RT3_REPLACE(0x40caa0, Obj_Float338);

RT3_NOINLINE double __fastcall Obj_Float360(void* o, int, int32_t i) { return F<float>(o, 0x360 + i * 4); }
RT3_REPLACE(0x40cab0, Obj_Float360);

// 0x40cac0. a[i] - b[i] - c[i] over three float arrays (+0x310/+0x338/+0x360).
RT3_NOINLINE double __fastcall Obj_Remaining(void* o, int, int32_t i) {
    return (double)F<float>(o, 0x310 + i * 4) - F<float>(o, 0x338 + i * 4) - F<float>(o, 0x360 + i * 4);
}
RT3_REPLACE(0x40cac0, Obj_Remaining);

void __fastcall Obj_Set2a2(void* o, int, uint16_t v) { F<uint16_t>(o, 0x2a2) = v; }
RT3_REPLACE(0x40cb00, Obj_Set2a2);

// 0x40cb10. Stores v in world +0x66ba, then forwards it to virtual slot 21.
uint32_t __fastcall Obj_SetWorld66ba(void* o, int, int32_t v) {
    F<int32_t>(g_world, 0x66ba) = v;
    return ((uint32_t(__thiscall*)(void*, int32_t))(*(void***)o)[0x54 / 4])(o, F<int32_t>(g_world, 0x66ba));
}
RT3_REPLACE(0x40cb10, Obj_SetWorld66ba);

// 0x40cb40 (vtable stub).
float __fastcall Const5c8900_40cb40(void*) { return g_5c8900; }
RT3_REPLACE(0x40cb40, Const5c8900_40cb40);

 // namespace

// 0x40cb50 / 0x40cb60 (vtable). Object o's value (slot 0x68) plus 0x5c8784,
// truncated.
int32_t __fastcall Obj_Slot68Rounded_40cb50(uint8_t* o) {
    return Ftol(((double(__fastcall*)(uint8_t*))(*(void***)o)[0x68 / 4])(o) + Fl_207(0x5c8784));
}
RT3_REPLACE(0x40cb50, Obj_Slot68Rounded_40cb50);

int32_t __fastcall Obj_Slot68Rounded_40cb60(uint8_t* o) {
    return Ftol(((double(__fastcall*)(uint8_t*))(*(void***)o)[0x68 / 4])(o) + Fl_207(0x5c8784));
}
RT3_REPLACE(0x40cb60, Obj_Slot68Rounded_40cb60);

// 0x40cb70. Region-grid byte at this object's own position.
RT3_NOINLINE uint32_t __fastcall Obj_GridByte(void* o) {
    float y = (float)Obj_CellY(o);
    float x = (float)Obj_CellX(o);
    return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
}
RT3_REPLACE(0x40cb70, Obj_GridByte);

// 0x40cba0. Address of frame +0x21 (0x45 bytes each) in animation +0x3a of the table +0x14; 0 if none.
RT3_NOINLINE uint32_t __fastcall Obj_AnimFrame(void* o) {
    uint8_t* t = F<uint8_t*>(o, 0x14);
    if (!t) return 0;
    return F<uint32_t>(t, F<int32_t>(o, 0x3a) * 24 + 0xc) + F<int32_t>(o, 0x21) * 0x45;
}
RT3_REPLACE(0x40cba0, Obj_AnimFrame);

// 0x40cbc0 (vtable stub).
void __fastcall Obj_SetByte42_40cbc0(uint8_t* o) { o[0x42] = 0x0; }
RT3_REPLACE(0x40cbc0, Obj_SetByte42_40cbc0);

// 0x40cbd0 (vtable stub).
void __fastcall Obj_SetByte42_40cbd0(uint8_t* o) { o[0x42] = 0x1; }
RT3_REPLACE(0x40cbd0, Obj_SetByte42_40cbd0);

// 0x40cbe0 (vtable stub).
int32_t __fastcall Return0_40cbe0(void*) { return 0; }
RT3_REPLACE(0x40cbe0, Return0_40cbe0);

// 0x40cbf0 (vtable stub).
int32_t __fastcall Obj_Byte42_40cbf0(uint8_t* o) { return o[0x42]; }
RT3_REPLACE(0x40cbf0, Obj_Byte42_40cbf0);

// 0x40cc00 (vtable stub).
int32_t __fastcall Return1_40cc00(void*) { return 0x1; }
RT3_REPLACE(0x40cc00, Return1_40cc00);

// 0x40cc10 (vtable stub).
int32_t __fastcall Return0_40cc10(void*) { return 0; }
RT3_REPLACE(0x40cc10, Return0_40cc10);

// 0x40cc20 (vtable stub).
int32_t __fastcall Return0_40cc20(void*) { return 0; }
RT3_REPLACE(0x40cc20, Return0_40cc20);

// 0x40cc30 (vtable stub).
int32_t __fastcall Return0_40cc30(void*) { return 0; }
RT3_REPLACE(0x40cc30, Return0_40cc30);

// 0x40cc40 / 0x40cc60 / 0x40cc80. Queries of the 0x62b268 type (+0x173).
int32_t __fastcall Obj_TypeKindIs1(void* o) { return F<uint8_t>(TypeOf173(o), 0x32) == 1 ? 1 : 0; }
RT3_REPLACE(0x40cc40, Obj_TypeKindIs1);

int32_t __fastcall Obj_TypeByteBa(void* o) { return F<uint8_t>(TypeOf173(o), 0xba); }
RT3_REPLACE(0x40cc60, Obj_TypeByteBa);

int32_t __fastcall Obj_TypeKind1Class3(void* o) {
    uint8_t* t = TypeOf173(o);
    return F<uint8_t>(t, 0x32) == 1 && F<uint8_t>(t, 0x8c) == 3 ? 1 : 0;
}
RT3_REPLACE(0x40cc80, Obj_TypeKind1Class3);

int32_t __fastcall Obj_Has2a8(uint8_t* o) { return F<int32_t>(o, 0x2a8) != 0; }
RT3_REPLACE(0x40ccb0, Obj_Has2a8);

// 0x40ccc0 (vtable stub).
int32_t __fastcall Return1_40ccc0(void*) { return 0x1; }
RT3_REPLACE(0x40ccc0, Return1_40ccc0);

RT3_NOINLINE uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x2129) + (F<int32_t>(g, 0x2145) * y + x) * 0x1d7;
}
RT3_REPLACE(0x40ccd0, Grid_RegionAt);

// 0x40ccf0. State byte +0x8c of the referenced building's state object.
RT3_NOINLINE uint32_t __fastcall Obj_BuildingState8c(void* o) {
    void* b = Obj_Building(o);
    void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    return F<uint8_t>(st, 0x8c);
}
RT3_REPLACE(0x40ccf0, Obj_BuildingState8c);

// 0x40cd10. With option +0x1c3, looks up (and discards) every site of the
// region's key list. Kept for exact behaviour.
RT3_NOINLINE void __fastcall Obj_TouchRegionSites(void* o) {
    if (!F<int32_t>(g_app, 0x1c3) || F<uint8_t>(o, 0x3d4) != 1) return;
    uint8_t* r = Obj_Region(o);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i)
        Collection_Get(g_stations, 0, F<uint16_t>(F<uint8_t*>(r, 0xef), i * 6));
}
RT3_REPLACE(0x40cd10, Obj_TouchRegionSites);

// 0x40cd70. Resolves the object's type name (+0x3ae) in 0x62b2fc (names +4): id to
// +0x3cc and the type's +0x173 to +0x3d0; fatal (string 0xad) if unknown.
RT3_NOINLINE void __fastcall Obj_ResolveType3ae(uint8_t* o) {
    F<uint32_t>(o, 0x3cc) = 0;
    F<uint32_t>(o, 0x3d0) = 0;
    for (int32_t i = 0; i < Collection_Count(g_buildingTypes); ++i) {
        Collection* c = g_buildingTypes;
        if (crt::stricmp((const char*)o + 0x3ae, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4)) continue;
        F<uint32_t>(o, 0x3cc) = Collection_IdAt(g_buildingTypes, 0, i, 0);
        c = g_buildingTypes;
        F<uint32_t>(o, 0x3d0) = F<uint32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x173);
        break;
    }
    if (!F<uint32_t>(o, 0x3cc)) FatalFormat(0xad, (const char*)o + 0x3ae);
}
RT3_REPLACE(0x40cd70, Obj_ResolveType3ae);

// 0x40ce60. Loads building o from stream s, first renaming its old type
// name "Radio Station" (+0x3ae) to "RadioStation".
int32_t __fastcall Building_LoadRenamed(uint8_t* o, int, int32_t s) {
    char* name = (char*)o + 0x3ae;
    if (!crt::stricmp(name, (const char*)0x5c8ce4)) memcpy(name, (const void*)0x5c8cd4, 13);
    Obj_ResolveType3ae(o);
    return Building_Load(o, 0, s);
}
RT3_REPLACE(0x40ce60, Building_LoadRenamed);

RT3_NOINLINE void* __fastcall Obj_Ref3cc(void* o) { return Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc)); }
RT3_REPLACE(0x40cec0, Obj_Ref3cc);

void* __fastcall Obj_Station3d0(uint8_t* o) { return Collection_Get(g_62b268, 0, F<uint32_t>(o, 0x3d0)); }
RT3_REPLACE(0x40cee0, Obj_Station3d0);

// 0x40cf00. For an object being placed (+0x3d4 == 1) with a shape (+0x24e):
// replaces its effects with a listed construction cursor (+0x25a, sized
// `scale` x its frame +0x14 x 0x5c8cf4, values 2): the centre at the object
// and the four arrows at the corners of the shape's cell bounds (2 cells in,
// 23 units a cell), all at the object's height.
void __fastcall Obj_ShowConstructCursor(uint8_t* o, int, float scale) {
    if (o[0x3d4] != 1 || !F<uint8_t*>(o, 0x24e)) return;
    Obj_FreeEffects(o, 0, 1);
    uint8_t* m = F<uint8_t*>(o, 0x14);
    uint8_t* fr = m ? *(uint8_t**)(m + 0xc + F<int32_t>(o, 0x3a) * 0x18) + F<int32_t>(o, 0x21) * 0x45 : nullptr;
    float size = (float)((double)scale * F<float>(fr, 0x14) * Fl(0x5c8cf4));
    uint8_t* e = Effect_Create(o, 8, size, 0x100000, 1);
    F<uint8_t*>(o, 0x25a) = e;
    Obj_SetAllBc(e, 2);
    int32_t b[4];
    Shape_Bounds(hook::As<void*>(g_buildingTypes), 0, F<uint8_t*>(o, 0x24e), b);
    float pos[3];
    SceneObj_GetPos(o, 0, (uint32_t*)&pos[0], (uint32_t*)&pos[1], (uint32_t*)&pos[2]);
    Store3Globals((int32_t*)pos);
    Obj_ApplyStored3(F<uint8_t*>(o, 0x25a), 0);
    float x0 = (float)((b[0] + 2) * 0x17), x1 = (float)((b[1] - 2) * 0x17);
    float z0 = (float)((b[2] + 2) * 0x17), z1 = (float)((b[3] - 2) * 0x17);
    const float corners[4][2] = { { x0, z0 }, { x0, z1 }, { x1, z1 }, { x1, z0 } };
    for (int32_t k = 0; k < 4; ++k) {
        float p[3] = { corners[k][0], pos[1], corners[k][1] };
        Store3Globals((int32_t*)p);
        Obj_ApplyStored3(F<uint8_t*>(o, 0x25a), k + 1);
    }
}
RT3_REPLACE(0x40cf00, Obj_ShowConstructCursor);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x40d120. Query slot 0x70, choose tool 5/+0x2a8 or 10/+0x2a4, then select.
RT3_NOINLINE void __fastcall ModelObj_SelectTrackTool(uint8_t* object) {
    void** vtable = F_runtime_wrappers<void**>(object, 0);
    int32_t result = ((int32_t(__fastcall*)(uint8_t*))vtable[0x70 / 4])(object);
    int32_t mode = result ? 5 : 10;
    int32_t id = F_runtime_wrappers<int32_t>(object, result ? 0x2a8 : 0x2a4);
    View_SetToolMode((uint8_t*)(hook::As<void*>(g_gameWindow)), 0, (uint32_t)(mode), id);
    ModelObj_Select(object);
}
RT3_REPLACE(0x40d120, ModelObj_SelectTrackTool);

// 0x40d170. Gives building o its upgraded model when its type's (+0x3cc in
// 0x62b2fc) upgrade group (+0x173 in 0x62b268) has +0x4b.
void __fastcall Building_RefreshUpgrade(uint8_t* o) {
    uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
    uint8_t* g = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(type, 0x173));
    if (F<int32_t>(g, 0x4b)) Building_AddUpgradeModel(o);
}
RT3_REPLACE(0x40d170, Building_RefreshUpgrade);

// 0x40d1b0. Industry building o: drops its station model when its type
// (site +0x3cc -> 0x62b2fc +0x173 -> 0x62b268) has one (+0x4b).
void __fastcall IndustryObj_DropModel(uint8_t* o) {
    uint8_t* site = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
    uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(site, 0x173));
    if (F<int32_t>(type, 0x4b)) BuildingObj_DropModel(o);
}
RT3_REPLACE(0x40d1b0, IndustryObj_DropModel);

RT3_NOINLINE void* __fastcall Obj_Ref2a8(void* o) {
    uint32_t id = F<uint32_t>(o, 0x2a8);
    return id ? Collection_Get(g_stations, 0, id) : nullptr;
}
RT3_REPLACE(0x40d1f0, Obj_Ref2a8);

// 0x40d210. Owning company (+0x276), or 0.
RT3_NOINLINE void* __fastcall Obj_OwnerCompany(void* o) {
    uint32_t id = F<uint32_t>(o, 0x276);
    return id ? Collection_Get(g_companies, 0, id) : nullptr;
}
RT3_REPLACE(0x40d210, Obj_OwnerCompany);

// 0x40d230. Virtual check (slot 28) and the 0x6cec20 record at +0x2a8 is active.
RT3_NOINLINE int32_t __fastcall Obj_CheckActive(void* o) {
    if (!((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) return 0;
    uint32_t id = F<uint32_t>(o, 0x2a8);
    void* r = id ? Collection_Get(g_stations, 0, id) : nullptr;
    return Obj_BuildingStateActive(r) ? 1 : 0;
}
RT3_REPLACE(0x40d230, Obj_CheckActive);

// 0x40d270. When this building's region holds key k, drops the demand
// records of its chain (+0x272) whose source (+0xe) shares a region key.
RT3_NOINLINE void __fastcall Building_DropSharedDemand(uint8_t* b, int, uint16_t k) {
    if (!Obj_HasKey(Obj_RegionAt(b), 0, k)) return;
    Collection* demand = g_6ada90;
    for (uint32_t id = F<uint32_t>(b, 0x272); id; ) {
        uint8_t* rec = (uint8_t*)Collection_Get(demand, 0, id);
        uint32_t next = F<uint32_t>(rec, 4);
        if (Obj_SharesRegionKey(b, 0, F<uint16_t>(rec, 0xe))) {
            void** vt = *(void***)demand;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(demand, id);
        }
        id = next;
    }
}
RT3_REPLACE(0x40d270, Building_DropSharedDemand);

// 0x40d2d0. Refresh: virtual slot 21(0); then if the world option +0x4ae7 is on,
// virtual slot 28 passes, its 0x6cec20 record (+0x2a8) is active and flagged
// (+0x5bc): virtual slot 22(0x5f, 0x5f, 0xff).
RT3_NOINLINE void __fastcall Obj_RefreshHighlight(void* o) {
    void** vt = *(void***)o;
    ((void(__thiscall*)(void*, int32_t))vt[0x54 / 4])(o, 0);
    if (!F<int32_t>(g_world, 0x4ae7)) return;
    if (!((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) return;
    auto rec = [&]() -> uint8_t* {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        return id ? (uint8_t*)Collection_Get(g_stations, 0, id) : nullptr;
    };
    if (!Obj_BuildingStateActive(rec())) return;
    volatile uint8_t* r = rec();
    if (!r[0x5bc]) return;
    ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))(*(void***)o)[0x58 / 4])(o, 0x5f, 0x5f, 0xff);
}
RT3_REPLACE(0x40d2d0, Obj_RefreshHighlight);

// 0x40d350. Signed byte +0xb9 of the state object (vtable slot 32).
RT3_NOINLINE int32_t __fastcall Obj_StateB9(void* o) {
    void* st = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return (int32_t)F<int8_t>(st, 0xb9);
}
RT3_REPLACE(0x40d350, Obj_StateB9);

// 0x40d360. The state object (vtable slot 32) has kind byte +0x32 == 4.
RT3_NOINLINE int32_t __fastcall Obj_StateKindIs4(void* o) {
    void* st = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return F<uint8_t>(st, 0x32) == 4 ? 1 : 0;
}
RT3_REPLACE(0x40d360, Obj_StateKindIs4);

// 0x40d380. Charges a building's upkeep for its class (type +0xb9: 0/1/2 ->
// effect 0x68/0x66/0x67 x 0x5c8cf8/0x5c8cfc/0x5c87c0) to its owner company (stat 3)
// and accumulates it in +0x310.
void __fastcall Building_ChargeUpkeep(uint8_t* b) {
    uint8_t* type = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    int32_t cls = (int8_t)type[0xb9];
    uint8_t* comp = nullptr;
    uint32_t player = 0;
    float cost = 0.0f;
    if (F<uint32_t>(b, 0x276)) {
        comp = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(b, 0x276));
        if (comp) player = F<uint32_t>(comp, 0x3b);
    }
    static const int32_t effect[3] = { 0x68, 0x66, 0x67 };
    static const uint32_t scale[3] = { 0x5c8cf8, 0x5c8cfc, 0x5c87c0 };
    if (cls >= 0 && cls <= 2) {
        float y = (float)Obj_CellY(b);
        float x = (float)Obj_CellX(b);
        uint32_t region = Map_GridByte(hook::As<void*>(g_map), 0, x, y);
        float m = Effects_Multiplier(World(), 0, effect[cls], F<uint32_t>(b, 0x276), player, region);
        cost = (float)((double)m * *(const float*)scale[cls]);
    }
    if (comp) Company_PostStat(comp, 0, 3, cost, 1);
    F<float>(b, 0x310) = (float)((double)cost + F<float>(b, 0x310));
}
RT3_REPLACE(0x40d380, Building_ChargeUpkeep);

// 0x40d4c0 (vtable). Model o's frame time, at least that of its attached
// model (+0x246).
float __fastcall Model_FrameTimeWithAttached(uint8_t* o) {
    int has;
    uint32_t v = Model_FrameTimeBits(o, &has);
    if (uint8_t* sub = F<uint8_t*>(o, 0x246)) {
        int subHas;
        const uint32_t t = Model_FrameTimeBits(sub, &subHas);
        float fv, ft;
        memcpy(&fv, &v, 4), memcpy(&ft, &t, 4);
        if (subHas && !(fv > ft)) v = t;
    }
    float f;
    memcpy(&f, &v, 4);
    return f;
}
RT3_REPLACE(0x40d4c0, Model_FrameTimeWithAttached);

// 0x40d540. Purchase price of the building for `company`: its type's value (+0x22,
// plus +0x43 when +0x246) blended over its first 5 years with its average yearly
// profit (revenue +0x310 less costs +0x338 / +0x360, recent years weighted 6..2, the
// current year scaled by the months elapsed), at least 0.8 x the value, rounded up
// to 10K / 50K / 100K steps and scaled by the price effects for its region.
RT3_NOINLINE float __fastcall Building_Price(uint8_t* b, int, uint32_t company) {
    auto type = [&]() { return (*(uint8_t*(__fastcall**)(void*))(*(uint8_t**)b + 0x80))(b); };
    float base = (float)F<uint32_t>(type(), 0x22);
    if (F<int32_t>(b, 0x246)) base = (float)((double)(float)F<int32_t>(type(), 0x43) + base);
    uint32_t player = 0;
    if (company) player = F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b);
    uint8_t* world = g_world;
    int32_t years = F<uint16_t>(world, 0xd) - F<int32_t>(b, 0x3d5) + 1;
    float weighted = 0.0f, weights = 0.0f;
    int32_t span = 0, n = 0;
    if (years >= 0) {
        span = years;
        n = years > 5 ? 5 : years;
    }
    for (int32_t k = 0; k < n; ++k) {
        float profit = (float)((double)F<float>(b, 0x310 + k * 4) - F<float>(b, 0x338 + k * 4) - F<float>(b, 0x360 + k * 4));
        float w = (float)(6 - k);
        if (k == 0) {
            int32_t m = world[0xf] - 1;
            if (m < 1) continue;
            profit = (float)(D(0x5c8978) / (double)m * profit);
            w = (float)((double)m / D(0x5c8978) * w);
        }
        weights = (float)((double)w + weights);
        weighted = (float)((double)w * profit + weighted);
    }
    float div = Fl_202(0x5c85b4) > weights ? 1.0f : weights;
    float avg = (float)((double)weighted / div);
    avg = (float)(avg / D(0x5c8658));
    double r = (double)span * D(0x5c8c20);
    float k = (float)(D(0x5c8770) < r ? 1.0 : r);
    float v = (float)((D(0x5c8770) - k) * (double)base + (double)k * avg);
    double cap = (double)base * D(0x5c8680);
    if (v < cap) v = (float)cap;
    int32_t price;
    if (v < Fl_202(0x5c8d20)) {
        price = Ftol((double)v / D(0x5c8d18) + D(0x5c8628)) * 10000;
        if (price < 10000) price = 10000;
    } else if (v < Fl_202(0x5c8d10)) {
        price = Ftol((double)v / D(0x5c8d08) + D(0x5c8628)) * 50000;
        if (price < 50000) price = 50000;
    } else {
        price = Ftol((double)v / Fl_202(0x5c8d00) + D(0x5c8628)) * 100000;
        if (price < 100000) price = 100000;
    }
    float fp = (float)price;
    float cy = (float)Obj_CellY(b), cx = (float)Obj_CellX(b);
    uint32_t region = Map_GridByte(g_map, 0, cx, cy);
    return (float)(Effects_Multiplier(g_world, 0, 0x3c, company, player, region) * (double)fp);
}
RT3_REPLACE(0x40d540, Building_Price);

// 0x40db30. Registers this building (+0x2a4) with the value of every site in its region's key list.
RT3_NOINLINE void __fastcall Building_RegisterWithSites(uint8_t* b) {
    uint8_t* r = Obj_Region(b);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) {
        uint8_t* e = F<uint8_t*>(r, 0xef) + i * 6;
        uint8_t* site = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(e, 0));
        e = F<uint8_t*>(r, 0xef) + i * 6;
        Obj_AddKeyUnique(site, 0, F<uint16_t>(b, 0x2a4), F<int32_t>(e, 2));
    }
}
RT3_REPLACE(0x40db30, Building_RegisterWithSites);

uint32_t __fastcall Obj_NotifySiteOn(uint8_t* o) { return NotifySite(o, 1); }
RT3_REPLACE(0x40dba0, Obj_NotifySiteOn);

uint32_t __fastcall Obj_NotifySiteOff(uint8_t* o) { return NotifySite(o, 0); }
RT3_REPLACE(0x40dbf0, Obj_NotifySiteOff);

// 0x40dc40. Changes the type of station building b (station +0x2a8 in
// 0x6cec20, active) to site type `type` (0x62b2fc): the price is the type's
// cost (+0x22 of its building type +0x173) times the build cost effect 0x3b
// of the owner (+0x276) at b's region (into *cost). With `check` it only
// tests: the owner's cash (else string 0xb52 into text), then the new site's
// spot at the station (Sites_CanPlace, b's own fine cells cleared meanwhile).
// Otherwise it rebuilds b as the new type at that spot (name, model, angle,
// site +0x24e), books the price (stat 0xe / 0x11 by kind, 0xd negated, the
// value +0x27a) and moves the station. Returns 1 when possible / done.
RT3_NOINLINE int32_t __fastcall Building_ChangeType(uint8_t* b, int, uint32_t type, int32_t check, float* cost, char* text) {
    if (cost) *cost = 0.0f;
    if (text) *text = 0;
    uint8_t* st = F<uint32_t>(b, 0x2a8) ? (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(b, 0x2a8)) : nullptr;
    if (!st || !Obj_BuildingStateActive(st)) return 0;
    uint8_t* bld = (uint8_t*)Obj_Building(st);
    const uint32_t kind = ((uint8_t*(__fastcall**)(uint8_t*))F<void**>(bld, 0))[0x20](bld)[0x8c];
    uint8_t* rec = (uint8_t*)Collection_Get(g_buildingTypes, 0, type);
    float price = (float)F<uint32_t>(Collection_Get(g_62b268, 0, F<uint32_t>(rec, 0x173)), 0x22);
    uint8_t* owner = nullptr;
    uint32_t player = 0;
    if (F<uint32_t>(b, 0x276)) {
        owner = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(b, 0x276));
        if (owner) player = F<uint32_t>(owner, 0x3b);
    }
    const uint32_t company = F<uint32_t>(b, 0x276);
    const float cy = (float)Obj_CellY(b);
    const float cx = (float)Obj_CellX(b);
    const uint32_t region = Map_GridByte(g_map, 0, cx, cy);
    price = Effects_Multiplier(g_world, 0, 0x3b, company, player, region) * price;
    if (cost) *cost = price;
    if (check && owner && Company_Stat(owner, 0, 0xd, 0x2329) < (double)price) {
        if (text) {
            char cash[0x1f4], need[0x1f4];
            const char* has = Num_FormatSimple(cash, 9, Company_Stat(owner, 0, 0xd, 0x2329), 0xb);
            const char* costs = Num_FormatSimple(need, 9, price, 0xb);
            FormatText(text, GetString(0xb52), costs, has, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        return 0;
    }
    const float nx = Obj_NodeCenterX(st);
    const float ny = Obj_NodeCenterY(st);
    const float angle = SceneObj_Angle2(b);
    float dx, dy;
    Type_RotatedCell(g_buildingTypes, 0, type, &dx, &dy, angle);
    const float x = dx + nx;
    const float y = ny - dy;
    if (F<uint32_t>(b, 0x24e)) Map_ClearFineBits((const uint32_t*)(F<uint8_t*>(b, 0x24e) + 0x10));
    const int32_t ok = Sites_CanPlace(g_buildingTypes, 0, type, x, y, angle, 0, 1, text, 1, 1, -1, 0);
    if (F<uint32_t>(b, 0x24e)) Map_SetFineBits((const uint32_t*)(F<uint8_t*>(b, 0x24e) + 0x10));
    if (!ok) return 0;
    if (check) return 1;
    Station_Move((uint8_t*)Obj_Ref2a8(b), 0, x, y, 1, -1);
    ModelObj_Release(b);
    strcpy((char*)b + 0x3ae, (const char*)rec + 4);
    Building_Init(b, 0, (const char*)rec + 0x40, (const char*)rec + 0x7c, x, y, 1, 0, 0, 0);
    uint32_t bits;
    memcpy(&bits, &angle, 4);
    SceneObj_SetAngle2(b, 0, bits);
    uint8_t* site = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(b, 0x3cc));
    F<uint8_t*>(b, 0x24e) = Sites_Create(hook::As<uint8_t*>(g_buildingTypes), 0, (const char*)site + 4, x, y, SceneObj_Angle2(b), b, nullptr);
    Obj_ResolveType3ae(b);
    if (owner) {
        Company_AddStat(owner, 0, ((int32_t(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x1c](b) ? 0xe : 0x11, price);
        Company_AddStat(owner, 0, 0xd, -price);
        F<float>(b, 0x27a) = price + F<float>(b, 0x27a);
    }
    Station_Move((uint8_t*)Obj_Ref2a8(b), 0, x, y, 1, (int32_t)kind);
    return 1;
}
RT3_REPLACE(0x40dc40, Building_ChangeType);

// 0x40e030. Not (virtual slot 28 and its site +0x2a8 active).
int32_t __fastcall Obj_NotActiveSite(void* o) {
    int32_t on = 0;
    if (((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) {
        uint32_t id = F<uint32_t>(o, 0x2a8);
        void* site = id ? Collection_Get(g_stations, 0, id) : nullptr;
        if (Obj_BuildingStateActive(site)) on = 1;
    }
    return on == 0 ? 1 : 0;
}
RT3_REPLACE(0x40e030, Obj_NotActiveSite);

// 0x40e080. Tears building b down: a station (type +0x32 == 4) leaves the
// station pairs (0x6cec20 +0x2a4); with the region list built (+0x3d4) the
// region's stations drop it, and its owner (+0x276, not in a tutorial) books
// its value back (+0x27a, or the type cost +0x22 plus +0x43 when +0x246 when
// unset): stat 0xe / 0x11 and posted 6 / 3 by the building kind (vtable
// +0x70). It leaves the key table 0x6ada90, its city's chain (+0x26e), and
// for a station building (type +0x32 == 1 of the record +0x3cc) the trains'
// effects bound to it and its station record (+0x2a8). Then the map cell
// (+0x388 / +0x38c) forgets it and the model goes.
void __fastcall Building_Teardown(uint8_t* b) {
    auto type = [&]() { return ((uint8_t*(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x20](b); };
    auto kind = [&]() { return ((int32_t(__fastcall**)(uint8_t*))F<void**>(b, 0))[0x1c](b); };
    if (type()[0x32] == 4) Table_RemovePairKey(g_stations, 0, F<int32_t>(b, 0x2a4));
    kind();
    if (b[0x3d4] == 1) {
        uint8_t* r = Obj_Region(b);
        for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(F<uint8_t*>(r, 0xef), i * 6));
            Entries6_Remove(st, 0, (int16_t)F<uint16_t>(b, 0x2a4));
        }
        if (g_companies && F<uint32_t>(b, 0x276) &&
            Collection_Get(g_companies, 0, F<uint32_t>(b, 0x276)) && !g_newGame[0x82]) {
            float value = F<float>(b, 0x27a);
            if (value <= 0.0f) {
                const uint32_t extra = F<int32_t>(b, 0x246) ? F<uint32_t>(type(), 0x43) : 0;
                value = (float)(F<uint32_t>(type(), 0x22) + extra);
            }
            auto owner = [&]() {
                return F<uint32_t>(b, 0x276) ? Collection_Get(g_companies, 0, F<uint32_t>(b, 0x276)) : nullptr;
            };
            const float refund = -value;
            void* co = owner();
            Company_AddStat(co, 0, kind() ? 0xe : 0x11, refund);
            co = owner();
            Company_PostStat(co, 0, kind() ? 6 : 3, refund, 0);
        }
    }
    if (g_6ada90) Table_ReleaseKeyAny(g_6ada90, 0, F<int32_t>(b, 0x2a4));
    if (const uint16_t city = F<uint16_t>(b, 0x26e)) Chain_Unlink((uint8_t*)Collection_Get(g_cities, 0, city), 0, b);
    uint8_t* rec = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(b, 0x3cc));
    if (((uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(rec, 0x173)))[0x32] == 1) {
        if (Collection* trains = g_trains) {
            for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
                trains = g_trains;
                uint8_t* t = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0));
                uint32_t* fx = F<uint32_t*>(t, 0xc8);
                if (fx && fx[1] == (uint32_t)(uintptr_t)b) {
                    Effect_Free(fx);
                    F<uint32_t*>(t, 0xc8) = nullptr;
                }
            }
        }
        Collection* stations = g_stations;
        ((void(__fastcall**)(Collection*, int, uint32_t))*(void***)stations)[2](stations, 0, F<uint32_t>(b, 0x2a8));
        F<uint32_t>(b, 0x2a8) = 0;
    }
    uint8_t* map = g_map;
    uint8_t* cell = F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * F<int32_t>(b, 0x38c) + F<int32_t>(b, 0x388)) * 0x1d7;
    City_RemoveBuilding(cell, 0, F<uint16_t>(b, 0x2a4));
    ModelObj_Release(b);
    Nop436050(g_world, 0, F<int32_t>(b, 0x2a4));
}
RT3_REPLACE(0x40e080, Building_Teardown);

// 0x40e360. For an object whose type's kind record is a 1/3 kind and that sits on a
// live track node (+0x2a8 -> +8), drops it to the node's height minus a margin,
// re-snapping x/z to its own position (via cell units).
RT3_NOINLINE void __fastcall Obj_SettleOnNode(uint8_t* o) {
    uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
    uint8_t* kind = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(type, 0x173));
    if (kind[0x32] != 1 || kind[0x8c] != 3) return;
    Collection* places = g_stations;
    if (!F<uint32_t>(o, 0x2a8) || !Collection_Get(places, 0, F<uint32_t>(o, 0x2a8))) return;
    uint8_t* place = F<uint32_t>(o, 0x2a8) ? (uint8_t*)Collection_Get(places, 0, F<uint32_t>(o, 0x2a8)) : nullptr;
    uint32_t node = F<uint32_t>(place, 8);
    Collection* nodes = g_tracks;
    if (node == 0xffffffff || !Collection_IsValid(nodes, 0, node)) return;
    float h = Obj_Mid1ed((uint8_t*)Collection_Get(nodes, 0, node));
    h = (float)((double)h - Fl_188(0x5c8738));
    float z = (float)(Obj_CellY(o) * Fl_188(0x5c8700));
    float x = (float)(Obj_CellX(o) * Fl_188(0x5c8700));
    Obj_MoveTo(o, 0, x, h, z);
}
RT3_REPLACE(0x40e360, Obj_SettleOnNode);

// 0x40e450. Building o: recreates its site from the saved records (type by
// the name of its building type +0x3cc, shape number +0x252 into 0x5ee030):
// the site goes to +0x24e, its shape to +0x252, and links back (+0x73a).
RT3_NOINLINE void __fastcall Obj_RestoreSite(uint8_t* o) {
    char name[0x100];
    strcpy(name, (const char*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc)) + 4);
    int32_t id = Collection_FindByName4(g_buildingTypes, 0, name);
    SetGlobal5ee030(F<int32_t>(o, 0x252));
    uint8_t* s = Sites_Restore(hook::As<uint8_t*>(g_buildingTypes), 0, (uint32_t)id, o, 0);
    F<uint8_t*>(o, 0x24e) = s;
    F<int32_t>(o, 0x252) = F<int32_t>(s, 0x732);
    F<uint8_t*>(s, 0x73a) = o;
}
RT3_REPLACE(0x40e450, Obj_RestoreSite);

// 0x40e4e0 (vtable). Hover over placed station building o (+0x3d4 == 1):
// for one that is selectable (slot 0x70, outside the build tool 0x6d1a8c in
// mode 0x40) it tints its node, highlights its station on the overview
// (mode 3, or the station overlay 6 in modes 0 / 0x15 unless the finder is in
// modes 3 / 4) and tooltips the station's cargo (string 0xafd / 0xaf with the
// overview cargo 0x2179 and the station names), its owner (0xb1 / 0xb2 in
// the tool) or its name (0xb0); others show their type's name (0xb3).
void __fastcall StationBuilding_Hover(uint8_t* o) {
    if (o[0x3d4] != 1) return;
    char frame[0x1f8];
    char* text = frame;
    text[0] = 0;
    auto toolActive = []() { return g_trackTool && Global622b0cIs64(); };
    const bool selectable = ((int32_t(__thiscall*)(uint8_t*))(*(void***)o)[0x70 / 4])(o) != 0 && !toolActive();
    if (selectable) {
        if (o[0x3d4] == 1) {
            void* st = F<uint32_t>(o, 0x2a8) ? Collection_Get(g_stations, 0, F<uint32_t>(o, 0x2a8)) : nullptr;
            uint8_t* node = (uint8_t*)Obj_Node8(st);
            if (!Option2bfActive()) Obj_SetArgb(node, 0, 0xff, 0, 0xff, 0);
            if (Obj_CheckActive(o)) {
                const int32_t mode = F<int32_t>(g_map, 0x2175);
                if (mode == 3) {
                    F<uint32_t>(g_world, 0x66ba) = 0xff00;
                    Overview_SetHighlight(g_map, 0, F<int32_t>(o, 0x2a8));
                } else if (mode == 0 || mode == 0x15) {
                    uint8_t* f = g_6d12f0;
                    if (!f || (F<int32_t>(f, 0x78) != 4 && F<int32_t>(f, 0x78) != 3)) {
                        F<uint32_t>(g_world, 0x66ba) = 0xff00;
                        Overview_SetMode(g_map, 0, 6, F<int32_t>(o, 0x2a8), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
                    }
                }
            }
        }
        uint8_t* map = g_map;
        if (F<int32_t>(map, 0x2171) == 1 && F<int32_t>(map, 0x2179) > 0) {
            if (F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x9a + F<int32_t>(map, 0x2179) * 4) != -1) {
                Overview_SetOption(map, 0, F<int32_t>(o, 0x2a8));
                Overview_SetHighlight(g_map, 0, F<int32_t>(o, 0x2a8));
                uint8_t* a = (uint8_t*)Obj_Ref2a8(o);
                uint8_t* cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(g_map, 0x2179));
                uint8_t* b = (uint8_t*)Obj_Ref2a8(o);
                uint8_t* cargo2 = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(g_map, 0x2179));
                const char* n1 = LocalizeName((const char*)cargo2 + 4);
                const char* n2 = LocalizeName((const char*)cargo + 4);
                FormatText(text, GetString(0xafd), n1, (const char*)b + 0x46b, n2, (const char*)a + 0x46b, 0, 0, 0, 0);
            } else {
                uint8_t* s = (uint8_t*)Obj_Ref2a8(o);
                uint8_t* cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(g_map, 0x2179));
                uint8_t* cargo2 = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(g_map, 0x2179));
                const char* n1 = LocalizeName((const char*)cargo2 + 4);
                const char* n2 = LocalizeName((const char*)cargo + 4);
                FormatText(text, GetString(0xaf), n1, n2, (const char*)s + 0x46b, 0, 0, 0, 0, 0);
            }
        } else if (toolActive()) {
            if (F<int32_t>(o, 0x276) == Obj_Get21(g_world)) {
                const char* name = (const char*)Obj_Ref2a8(o) + 0x46b;
                FormatText(text, GetString(0xb1), name, 0, 0, 0, 0, 0, 0, 0);
            } else {
                FormatText(text, GetString(0xb2), 0, 0, 0, 0, 0, 0, 0, 0);
            }
        } else {
            uint8_t* s = F<uint32_t>(o, 0x2a8) ? (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(o, 0x2a8)) : nullptr;
            FormatText(text, GetString(0xb0), (const char*)s + 0x46b, 0, 0, 0, 0, 0, 0, 0);
        }
    } else if (!toolActive()) {
        uint8_t* site = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
        uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(site, 0x173));
        const char* name = LocalizeName((const char*)type + 0x4f);
        FormatText(text, GetString(0xb3), name, 0, 0, 0, 0, 0, 0, 0);
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 3, 0, -1);
}
RT3_REPLACE(0x40e4e0, StationBuilding_Hover);

// 0x40e880. Station b changed hands or closed: drops the overview option of
// the cargo view, clears the tooltip, recolours its track and the buildings
// (unless the track picker shows tab 3 / 4), and once inactive leaves the
// overview modes centred on it (6: also re-picking the station window's
// cargo; 3: its highlight); with auto-overview on (+0x2cc, 0x6d1a8c) shows
// the plain mode again.
void __fastcall Station_OnStateChanged(uint8_t* b) {
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2171) == 1 && F<int32_t>(map, 0x2179) > 0) Overview_SetOption(map, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1);
    if (!((int32_t(__thiscall*)(uint8_t*))(*(void***)b)[0x70 / 4])(b)) return;
    if (b[0x3d4] != 1) return;
    uint8_t* tp = g_6d12f0;
    if (!tp || (F<int32_t>(tp, 0x78) != 4 && F<int32_t>(tp, 0x78) != 3)) {
        uint32_t id = F<uint32_t>(b, 0x2a8);
        void* o = id ? Collection_Get(g_stations, 0, id) : nullptr;
        Track_UpdateColor((uint8_t*)Obj_Node8(o));
        Table_RefreshAll(g_buildings);
    }
    if (!Obj_CheckActive(b)) return;
    map = g_map;
    int32_t mode = F<int32_t>(map, 0x2175);
    if (mode == 6 && F<int32_t>(map, 0x2179) == F<int32_t>(b, 0x2a8)) {
        Overview_Close(map);
        if (g_stationWindow) StationWindow_PickCargo(g_stationWindow, 0, g_6d16f8);
        map = g_map;
    } else if (mode == 3 && F<int32_t>(map, 0x2181) == F<int32_t>(b, 0x2a8)) {
        Overview_SetHighlight(map, 0, 0);
        map = g_map;
    }
    if (F<int32_t>(g_app, 0x2cc) && hook::As<uint32_t>(g_trackTool) && !F<int32_t>(map, 0x2175))
        Overview_SetMode(map, 0, 0x15, 0, 0, 0, nullptr, 0, 0, 99999, 99999);
}
RT3_REPLACE(0x40e880, Station_OnStateChanged);

// 0x40e9d0 (vtable). Tooltip of owned object o into dst (n bytes): nothing
// when it is hidden (slot 0x14); in debug mode (0x6cec74 +0x74) string 0xb4
// with its type name (slot 0x80, +4) and its index (+0x2a4); otherwise the
// owner's logo in *logo (returning 2) when ownership shows: map mode 2
// (+0x253), mode 1 for unowned (slot 0x70) or other players' objects, the
// open finder (0x6d12f0) in mode 4 for unowned or mode 3 on this owner (+0x7c);
// else the generic tooltip.
int32_t __fastcall OwnedObj_Tooltip(uint8_t* o, int, char* dst, int32_t n, int32_t a, int32_t b, int32_t logo) {
    auto slot = [&](uint32_t off) { return ((int32_t(__thiscall*)(uint8_t*))(*(void***)o)[off / 4])(o); };
    if (slot(0x14)) return 0;
    uint8_t* app = g_app;
    if (F<int32_t>(app, 0x74)) {
        char text[0x130], index[0x130];
        const char* type = (const char*)((uint8_t*)(uintptr_t)slot(0x80) + 4);
        crt::sprintf(text, GetString(0xb4), type);
        crt::sprintf(index, (const char*)0x5c8d24, F<int32_t>(o, 0x2a4));
        strcat(text, index);
        StrCopyN(dst, text, (size_t)(n - 1));
        return 1;
    }
    if (F<int32_t>(o, 0x276)) {
        const int32_t mode = F<int32_t>(app, 0x253);
        int32_t show = 0;
        if (mode == 2)
            show = 1;
        else if (mode == 1)
            show = !slot(0x70) || F<int32_t>(o, 0x276) != Obj_Get21(g_world) ? 1 : 0;
        if (uint8_t* f = g_6d12f0) {
            if (F<int32_t>(f, 0x78) == 4 && !slot(0x70)) show = 1;
            f = g_6d12f0;
            if (f && F<int32_t>(f, 0x78) == 3) show = F<int32_t>(o, 0x276) == F<int32_t>(f, 0x7c) ? 1 : 0;
        }
        if (show) {
            const uint32_t owner = F<uint32_t>(o, 0x276);
            uint8_t* c = owner ? (uint8_t*)Collection_Get(g_companies, 0, owner) : nullptr;
            *(uint32_t*)(uintptr_t)logo = Company_Logo(c);
            return 2;
        }
    }
    return SceneObj_DebugTooltip(o, 0, dst, n, a, b, logo);
}
RT3_REPLACE(0x40e9d0, OwnedObj_Tooltip);

// 0x40eba0. Moves a building to map position (x, z) (in cells): re-files it in
// the map's cell table (+0x388 / +0x38c) and, for station buildings (type +0x32
// = 4), in the station index; its height is the terrain height plus `lift`, or
// the nearest land height minus 0x5c8840 for water types (type +0x173 ->
// +0xba), whole units in multiplayer. A nonzero `level` pins the height
// (+0x392 / +0x396) and keeps it while the new height stays within half a unit.
// Its station (+0x2a8) moves along, then it settles on its track node.
RT3_NOINLINE void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level) {
    auto type = [&]() { return (*(uint8_t*(__fastcall**)(uint8_t*))(*(uint8_t**)o + 0x80))(o); };
    if (type()[0x32] == 4) Table_RemovePairKey(g_stations, 0, F<int32_t>(o, 0x2a4));
    auto cell = [&]() {
        uint8_t* map = g_map;
        return F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * F<int32_t>(o, 0x38c) + F<int32_t>(o, 0x388)) * 0x1d7;
    };
    City_RemoveBuilding(cell(), 0, F<uint16_t>(o, 0x2a4));
    F<int32_t>(o, 0x388) = Ftol((double)x / Fl_205(0x5c8568));
    F<int32_t>(o, 0x38c) = Ftol((double)z / Fl_205(0x5c8568));
    City_AddBuilding(cell(), 0, F<uint16_t>(o, 0x2a4));
    if (type()[0x32] == 4) Collection_RecordNearby(g_stations, 0, o);
    float wz = (float)((double)z * Fl_205(0x5c8700));
    float wx = (float)((double)x * Fl_205(0x5c8700));
    double ground = Terrain_HeightAt((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, wx, wz);
    float liftF = (float)lift;
    float y = (float)(ground + liftF);
    uint8_t* r = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
    uint8_t* t = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(r, 0x173));
    if (t[0xba]) {
        float lh = (float)Map_NearestLandHeight(g_map, 0, Ftol(x), Ftol(z), 6);
        y = (float)((double)lh - Fl_205(0x5c8840));
    }
    if (IsMultiplayer()) y = (float)Ftol(y);
    float py = y;
    if (!(level == Fl_205(0x5c8598))) {
        F<float>(o, 0x392) = level;
        F<float>(o, 0x396) = y;
    }
    if (!(F<float>(o, 0x392) == Fl_205(0x5c8598)) && fabs((double)F<float>(o, 0x396) - level) < D(0x5c8628)) py = F<float>(o, 0x392);
    Obj_MoveTo(o, 0, wx, py, wz);
    if (F<uint32_t>(o, 0x2a8) && Collection_Get(g_stations, 0, F<uint32_t>(o, 0x2a8))) {
        uint8_t* st = F<uint32_t>(o, 0x2a8) ? (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(o, 0x2a8)) : nullptr;
        Station_Move(st, 0, x, z, 0, -1);
    }
    Obj_SettleOnNode(o);
}
RT3_REPLACE(0x40eba0, Building_MoveTo);

// 0x40ee10. Puts the object back on the ground at its own cell position: on the
// terrain, or (types whose kind has +0xba) on the nearest land within 6 cells minus a
// margin; whole units in multiplayer. Then settles it on its track node.
RT3_NOINLINE void __fastcall Obj_Reground(uint8_t* o) {
    float cx = (float)Obj_CellX(o);
    float cz = (float)Obj_CellY(o);
    float z = (float)((double)cz * Fl_189(0x5c8700));
    float x = (float)((double)cx * Fl_189(0x5c8700));
    float y = (float)Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
    uint8_t* kind = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(type, 0x173));
    if (kind[0xba]) {
        int32_t iz = Ftol(cz);
        float h = (float)Map_NearestLandHeight(g_map, 0, Ftol(cx), iz, 6);
        y = (float)((double)h - Fl_189(0x5c8840));
    }
    if (IsMultiplayer()) y = (float)Ftol(y);
    Obj_MoveTo(o, 0, x, y, z);
    Obj_SettleOnNode(o);
}
RT3_REPLACE(0x40ee10, Obj_Reground);

 // namespace

// 0x40ef10 (thiscall, RET 0xc). Places building b at (x, y) for `company`
// (0 = none): a building on a site (+0x2a8, table 0x6cec20) needs the
// company to own the site's track and, with the track limit (+0x4aab) on an
// unlinked node, no pending pieces and an idle network. b takes the editor
// flag (+0x390), loses its owner when placed in the editor without one,
// becomes built this year (+0x3d4 / +0x3d5), gets its site record (+0x24e,
// height +0x252), moves there, and gets its smoke (8, five points around
// its footprint) and, outside auto companies and the 1 vehicle world
// (+0x4aef), its sparkle effect (9). Debug labels mark it (0xb5) and its
// attachment points (0xb6 n). A company pays the type's price scaled by
// effect 0x3b / 0x3c and the round-up value (stats 0xe or 0x11 and cash);
// b records the owner and price (+0x276 / +0x27a). Its station is named,
// sites registered, production starts (unless +0x29e), the overview
// redraws in its build modes and an owned building drops shared demand and
// fires trigger 6. Returns 0 when refused, else 1.
// QUIRK: a debug label's x is offset by the last footprint z, not by the
// point's own x offset (its slot is overwritten first).
RT3_NOINLINE int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company) {
    Collection* const sites = g_stations;
    auto site = [&]() -> uint8_t* { return F<int32_t>(b, 0x2a8) ? (uint8_t*)Collection_Get(sites, 0, F<uint32_t>(b, 0x2a8)) : nullptr; };
    if (F<int32_t>(g_world, 0x4aab) && F<int32_t>(b, 0x2a8) && Collection_Get(sites, 0, F<uint32_t>(b, 0x2a8))) {
        uint8_t* st = site();
        if (!Node_HasLink(Collection_Get(g_tracks, 0, F<uint32_t>(st, 8)), 0, 1) && company) {
            uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
            if (F<int32_t>(co, 0x766c) + F<int32_t>(co, 0x7668) + F<int32_t>(co, 0x7664) > 0) return 0;
            if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x98)) return 0;
        }
    }
    if (F<int32_t>(b, 0x2a8) && Collection_Get(sites, 0, F<uint32_t>(b, 0x2a8))) {
        uint8_t* st = site();
        if (F<uint16_t>(Collection_Get(g_tracks, 0, F<uint32_t>(st, 8)), 0x22e) != company) return 0;
    }
    ++g_62be40;
    b[0x390] = g_app[0x68];
    if (!company && F<int32_t>(g_app, 0x68)) F<int32_t>(b, 0x276) = 0;
    b[0x3d4] = 1;
    F<int32_t>(b, 0x3d5) = F<uint16_t>(g_world, 0xd);
    Obj_RefreshHighlight(b);
    uint32_t height = 0;
    {
        uint8_t* type = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(b, 0x3cc));
        const float angle = SceneObj_Angle2(b);
        uint8_t* rec = Sites_Create(hook::As<uint8_t*>(g_buildingTypes), 0, (const char*)type + 4, x, y, angle, b, &height);
        F<uint8_t*>(b, 0x24e) = rec;
        F<int32_t>(b, 0x252) = F<int32_t>(rec, 0x732);
    }
    Building_MoveTo(b, 0, x, y, 0, *(float*)&height);
    if ((int32_t)company > 0) F<int32_t>(b, 0x29e) = 1;
    Slot32 s10, s14, s18;
    int32_t vec[3];
    {
        uint8_t* tbl = F<uint8_t*>(b, 0x14);
        uint8_t* ent = tbl ? F<uint8_t*>(tbl, F<int32_t>(b, 0x3a) * 0x18 + 0xc) + F<int32_t>(b, 0x21) * 0x45 : nullptr;
        const float scale = (float)((double)F<float>(ent, 0x14) * Fl_207(0x5c8cf4));
        uint8_t* smoke = Effect_Create(b, 8, scale, 0x100000, 1);
        F<uint8_t*>(b, 0x25a) = smoke;
        Obj_SetAllBc(smoke, 2);
    }
    int32_t rect[4];
    Shape_Bounds(hook::As<uint8_t*>(g_buildingTypes), 0, F<uint8_t*>(b, 0x24e), rect);
    SceneObj_GetPos(b, 0, &s10.u, &s18.u, &s14.u);
    vec[0] = s10.i, vec[1] = s18.i, vec[2] = s14.i;
    Store3Globals(vec);
    Obj_ApplyStored3(F<uint8_t*>(b, 0x25a), 0);
    // The four footprint corners (cells x 23).
    auto corner = [&](int32_t idx) {
        vec[0] = s10.i, vec[1] = s18.i, vec[2] = s14.i;
        Store3Globals(vec);
        Obj_ApplyStored3(F<uint8_t*>(b, 0x25a), idx);
    };
    s10.f = (float)((rect[0] + 2) * 0x17);
    s14.f = (float)((rect[2] + 2) * 0x17);
    corner(1);
    s14.f = (float)((rect[3] - 2) * 0x17);
    corner(2);
    s10.f = (float)((rect[1] - 2) * 0x17);
    corner(3);
    s14.f = (float)((rect[2] + 2) * 0x17);
    corner(4);
    if (!g_newGame[0x82] && !F<int32_t>(g_world, 0x4aef)) {
        uint8_t* spark = Effect_Create(b, 9, 1.0f, 0x100000, 1);
        F<uint8_t*>(b, 0x25e) = spark;
        uint32_t n = BuildingSparkCount(F<uint32_t>(BuildingType(b), 0x22)) < 1 ? 1 : BuildingSparkCount(F<uint32_t>(BuildingType(b), 0x22));
        Obj_SetAllBc(F<uint8_t*>(b, 0x25e), (int32_t)n);
        SceneObj_GetPos(b, 0, &s10.u, &s18.u, &s14.u);
        vec[0] = s10.i, vec[1] = s18.i, vec[2] = s14.i;
        Store3Globals(vec);
        Obj_ApplyStored3(F<uint8_t*>(b, 0x25e), 0);
    }
    if (F<int32_t>(g_app, 0x78)) {
        Markers_Add(g_6ada80, 0, *(uint32_t*)&x, *(uint32_t*)&y, GetString(0xb5), 1);
        for (int32_t k = 0; k < 0xc; ++k) {
            if (!((uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(b, 0x3cc)))[0xe7 + k]) continue;
            const uint32_t id = F<uint32_t>(b, 0x3cc);
            const float angle = SceneObj_Angle2(b);
            Type_RotatedOffset(g_buildingTypes, 0, id, k, &s14.f, &s10.f, angle);
            char num[0x20], label[0xd0];
            const char* sn = Num_Format(num, 1, (double)(k + 1), 10, 1, 0, 0);
            const char* fmt = GetString(0xb6);
            FormatText(label, fmt, sn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            const float ly = (float)((double)s10.f + y);
            const float lx = (float)((double)x - s14.f);
            Markers_Add(g_6ada80, 0, *(const uint32_t*)&lx, *(const uint32_t*)&ly, label, 1);
        }
    }
    if ((int32_t)company > 0 && (BuildingIsOwned(b) || (!g_newGame[0x82] && !Global6d07b4Set2()))) {
        uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
        float price = (float)(double)F<uint32_t>(BuildingType(b), 0x22);
        const int32_t owned = BuildingIsOwned(b);
        const uint32_t player = F<uint32_t>(co, 0x3b);
        float cy = (float)Obj_CellY(b);
        float cx = (float)Obj_CellX(b);
        const uint32_t region = Map_GridByte(g_map, 0, cx, cy);
        price = (float)((double)Effects_Multiplier(g_world, 0, owned ? 0x3b : 0x3c, company, player, region) * price);
        price = (float)((double)Building_RoundedValue(F<uint32_t>(BuildingType(b), 0xe4), 0, price) + price);
        Company_AddStat(co, 0, BuildingIsOwned(b) ? 0xe : 0x11, price);
        Company_AddStat(co, 0, 0xd, -price);
        F<uint32_t>(b, 0x276) = company;
        F<float>(b, 0x27a) = price;
    }
    if (F<int32_t>(b, 0x2a8) && Collection_Get(sites, 0, F<uint32_t>(b, 0x2a8))) Station_Place(site(), 0, x, y);
    Building_RegisterWithSites(b);
    if (!F<int32_t>(b, 0x29e)) Industry_Produce(b, 0, 1);
    {
        const int32_t mode = F<int32_t>(g_map, 0x2175);
        if (mode == 0x29 || mode == 0x2a || mode == 0x2b || mode == 0x2c) Overview_Redraw(g_map, 0, nullptr);
    }
    if (BuildingIsOwned(b)) {
        Buildings_DropSharedDemand(g_buildings, 0, F<uint16_t>(b, 0x2a8));
        if (b[0x3d4] == 1) Events_Fire(g_events, 0, 6, nullptr, 0);
    }
    --g_62be40;
    RunDebugValidation();
    return 1;
}
RT3_REPLACE(0x40ef10, Building_Place);

// 0x40f6d0. Building o: sets it up as type t (0x62b2fc record: name +4, model
// +0x40 / +0x7c, scale +0x1a3) for owner `owner` (+0x2a4) at (x, y) turned
// by `angle`: clears its state, builds and places the model, records its cell
// (+0x388 / +0x38c) - joining the owner's city unless a station in the
// editor - copies its type's production (+0xa1.. / +0xad..), and with
// `place` creates its site (+0x24e, shape +0x252) and moves it there.
// Stations join the station list (+0x2a8); types with +0x4b and option +0x8c
// get vcall 0x74; the cell's city (+0x26e) adopts it; in-game stations note
// their neighbourhood; placed ones register with the sites; two random
// digits (+0x3d9 / +0x3dd) end it.
RT3_NOINLINE void __fastcall Building_Setup(uint8_t* o, int, int32_t owner, uint8_t* t, float x, float y, uint32_t angle, uint8_t place, int32_t a7) {
    F<int32_t>(o, 0x2a4) = owner;
    for (uint32_t off = 0x39a; off <= 0x3aa; off += 4) F<int32_t>(o, off) = 0;
    F<int32_t>(o, 0x392) = 0;
    F<int32_t>(o, 0x396) = 0;
    o[0x391] = 0;
    o[0x390] = 0;
    strcpy((char*)o + 0x3ae, (const char*)t + 4);
    Obj_ResolveType3ae(o);
    Building_Init(o, 0, (const char*)t + 0x40, (const char*)t + 0x7c, x, y, 1, 0, 0, 0);
    SceneObj_SetAngle2(o, 0, angle);
    SceneObj_SetScale36(o, 0, F<float>(t, 0x1a3));
    o[0x3d4] = place;
    F<int32_t>(o, 0x276) = a7;
    F<int32_t>(o, 0x2a8) = 0;
    F<int32_t>(o, 0x272) = 0;
    F<int32_t>(o, 0x27a) = 0;
    F<int32_t>(o, 0x29e) = 0;
    F<uint32_t>(o, 0x3d5) = F<uint16_t>(g_world, 0xd);
    F<int32_t>(o, 0x296) = 0;
    F<int32_t>(o, 0x29a) = 0;
    for (uint32_t off = 0x2ac; off <= 0x2bc; off += 4) F<int32_t>(o, off) = 0;
    F<uint16_t>(o, 0x2a2) = 0;
    F<int32_t>(o, 0x388) = Ftol(x / Fl_207(0x5c8568));
    F<int32_t>(o, 0x38c) = Ftol(y / Fl_207(0x5c8568));
    auto type = [&] { return ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)o)[0x20])(o); };
    if (type()[0x32] != 4 || !g_6ce9ac) {
        uint8_t* m = g_map;
        uint8_t* tile = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * F<int32_t>(o, 0x38c) + F<int32_t>(o, 0x388)) * 0x1d7;
        City_AddBuilding(tile, 0, F<uint16_t>(o, 0x2a4));
    }
    for (int32_t k = 0; k < 0xc; k += 4) {
        F<uint32_t>(o, 0x27e + k) = F<uint32_t>(type(), 0xa1 + k);
        F<uint32_t>(o, 0x28a + k) = F<uint32_t>(type(), 0xad + k);
    }
    for (int32_t k = 0; k < 10; ++k) {
        F<int32_t>(o, 0x2c0 + k * 4) = 0;
        F<int32_t>(o, 0x2e8 + k * 4) = 0;
        F<int32_t>(o, 0x310 + k * 4) = 0;
        F<int32_t>(o, 0x338 + k * 4) = 0;
        F<int32_t>(o, 0x360 + k * 4) = 0;
    }
    if (place == 1) {
        uint8_t* bt = (uint8_t*)Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc));
        float a = SceneObj_Angle2(o);
        F<uint8_t*>(o, 0x24e) = Sites_Create(hook::As<uint8_t*>(g_buildingTypes), 0, (const char*)bt + 4, x, y, a, o, nullptr);
        Building_MoveTo(o, 0, x, y, 0, 0.0f);
        F<int32_t>(o, 0x252) = F<int32_t>(F<uint8_t*>(o, 0x24e), 0x732);
    }
    auto btype = [&] {
        return (uint8_t*)Collection_Get(g_62b268, 0,
                                        F<uint32_t>(Collection_Get(g_buildingTypes, 0, F<uint32_t>(o, 0x3cc)), 0x173));
    };
    if (btype()[0x32] == 1) F<uint32_t>(o, 0x2a8) = Stations_Add(g_stations, 0, (uint32_t)owner, x, y);
    if (F<int32_t>(btype(), 0x4b) && F<int32_t>(g_app, 0x8c)) ((void(__fastcall*)(uint8_t*))(*(void***)o)[0x1d])(o);
    int32_t yi = Ftol(y);
    uint16_t city = (uint16_t)Map_CellCity(g_map, 0, Ftol(x), yi);
    F<uint16_t>(o, 0x26e) = city;
    F<uint16_t>(o, 0x270) = 0;
    if (city) Obj_PushBuilding(Collection_Get(g_cities, 0, city), 0, o);
    if (type()[0x32] == 4 && !g_6ce9ac) Collection_RecordNearby(g_stations, 0, o);
    if (place == 1) Building_RegisterWithSites(o);
    F<int32_t>(o, 0x3d9) = Rand(0, 9);
    F<int32_t>(o, 0x3dd) = Rand(0, 9);
}
RT3_REPLACE(0x40f6d0, Building_Setup);

// 0x40fa50. Region record (4x4 cells) at this object's position.
RT3_NOINLINE uint8_t* __fastcall Obj_RegionAt(void* o) {
    int32_t col = (int32_t)(Obj_CellX(o) / 4.0f);
    int32_t row = (int32_t)(Obj_CellY(o) / 4.0f);
    uint8_t* grid = g_map;
    return F<uint8_t*>(grid, 0x2129) + (F<int32_t>(grid, 0x2145) * row + col) * 0x1d7;
}
RT3_REPLACE(0x40fa50, Obj_RegionAt);

// 0x40faa0. Net value per unit of production recipe rec (0xbc bytes) at o's city:
// the output (+0x1c, x +4) at the city's quota minus the inputs (+0x2c items
// {cargo +0x44, amount +0x48}), divided by the output amount.
RT3_NOINLINE double __fastcall Recipe_NetValue(void* o, int, uint8_t* rec) {
    int32_t cellX, cellY;
    __asm {
        mov ecx, o
        call Obj_CellX
        fdiv dword ptr ds:[0x5c8568]
        call Rt3_Ftol
        mov cellX, eax
        mov ecx, o
        call Obj_CellY
        fdiv dword ptr ds:[0x5c8568]
        call Rt3_Ftol
        mov cellY, eax
    }
    uint8_t* m = Map();
    uint8_t* city = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * cellY + cellX) * 0x1d7;
    float out = (float)((double)(float)City_CargoQuota(city, 0, F<int32_t>(rec, 0x1c)) * F<float>(rec, 4));
    float in = 0.0f;
    for (int32_t k = 0; k < F<int32_t>(rec, 0x2c); ++k) {
        float q = (float)City_CargoQuota(city, 0, F<int32_t>(rec, 0x44 + k * 0x1c));
        in = (float)((double)q * F<float>(rec, 0x48 + k * 0x1c) + in);
    }
    return ((double)out - in) / F<float>(rec, 4);
}
RT3_REPLACE(0x40faa0, Recipe_NetValue);

// 0x40fb70. Forwards to the type's 0x412560 with flag (+0x246 != 0).
uint32_t __fastcall Obj_TypeQuery(void* o, int, int32_t a) {
    int32_t flag = F<int32_t>(o, 0x246) != 0;
    void* t = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return ((uint32_t)Vehicle_Available(t, 0, (uint8_t*)(a), flag));
}
RT3_REPLACE(0x40fb70, Obj_TypeQuery);

// 0x40fba0. The building's output of cargo: its type's industry total (second set
// when +0x246) times the building's count (+0x3cc type +0x141).
double __fastcall Building_CargoOutput(uint8_t* b, int, int32_t cargo) {
    float n = (float)F<int32_t>(Obj_Ref3cc(b), 0x141);
    uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    return Industry_OutputTotal(t, 0, cargo, F<int32_t>(b, 0x246) ? 1 : 0) * n;
}
RT3_REPLACE(0x40fba0, Building_CargoOutput);

// 0x40fbe0. Whether this object's coarse cell (4 cells) and building bid's
// region share a key in their key lists (+0xeb count, +0xef 6-byte entries).
RT3_NOINLINE int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid) {
    int32_t cx = (int32_t)(Obj_CellX(o) / *(const float*)rt3::Rdata(0x5c8568));
    int32_t cy = (int32_t)(Obj_CellY(o) / *(const float*)rt3::Rdata(0x5c8568));
    uint8_t* map = g_map;
    uint8_t* cell = F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * cy + cx) * 0x1d7;
    if (F<int32_t>(cell, 0xeb) <= 0) return 0;
    uint8_t* r = Obj_RegionAt(Collection_Get(g_buildings, 0, bid));
    for (int32_t i = 0; i < F<int32_t>(cell, 0xeb); ++i) {
        uint16_t k = F<uint16_t>(F<uint8_t*>(cell, 0xef), i * 6);
        for (int32_t j = 0; j < F<int32_t>(r, 0xeb); ++j)
            if (k == F<uint16_t>(F<uint8_t*>(r, 0xef), j * 6)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x40fbe0, Obj_SharesRegionKey);

// 0x40fcc0. Profit score of the building (0..1, 0.5 = neutral or not applicable):
// its weighted mean yearly result over up to 10 years (the current year weighted by
// the year's progress) against its capacity (type +0x22, + +0x43 when +0x246).
RT3_NOINLINE float __fastcall Building_ProfitScore(uint8_t* b) {
    void** vt = *(void***)b;
    if (((int32_t(__thiscall*)(void*))vt[0x70 / 4])(b)) return Fl(0x5c8d48);
    uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    if (!F<int32_t>(t, 0x3f)) return Fl(0x5c8d48);
    float cap = (float)(double)F<uint32_t>(t, 0x22);
    if (F<int32_t>(b, 0x246)) cap = (float)((double)(float)F<int32_t>(t, 0x43) + cap);
    int32_t years = F<uint16_t>(World(), 0xd) - F<int32_t>(b, 0x3d5) + 1;
    float sumV = 0.0f, sumW = 0.0f;
    for (int32_t i = 0; i < (years > 10 ? 10 : years); ++i) {
        float v = (float)Obj_Remaining(b, 0, i);
        if (i == 0) {
            float m = (float)((double)World()[0xf] / D(0x5c8978));
            float w = (float)((double)m * D(0x5c85f0));
            double d = (double)m > D(0x5c8c20) ? (double)m : 0.2;
            sumV = (float)((double)w * v * (D(0x5c8770) / d) + sumV);
            sumW = (float)((double)w + sumW);
        } else {
            float w = (float)(10 - i);
            sumW = (float)((double)w + sumW);
            sumV = (float)((double)w * v + sumV);
        }
    }
    float denom = sumW > Fl(0x5c85b4) ? sumW : 1.0f;
    float avg = (float)((double)sumV / denom);
    float r = (float)((double)avg / cap * D(0x5c8d40));
    if (r < Fl(0x5c8598)) r = (float)((double)r * D(0x5c8d38));
    r = (float)((double)r * D(0x5c8628) + D(0x5c8628));
    if (r > Fl(0x5c85b4)) return 1.0f;
    if (r < Fl(0x5c8598)) return 0.0f;
    return r;
}
RT3_REPLACE(0x40fcc0, Building_ProfitScore);

// 0x40fef0. Sends a cargo lot of `kind` from this building to a random other ready
// building that wants it (+0x27e per kind), within a random range (50..400 tiles, or
// anywhere, times `factor`, at least 50) but over 15 away: starting at a random
// index, the best of the random-weighted wants (scaled by k when it has
// stations) wins; a strong want or scanning past a tenth of the buildings ends the
// search. No lot if the two share a region key.
RT3_NOINLINE void __fastcall Building_SpawnLot(uint8_t* b, int, int32_t kind, float factor) {
    Collection* blds = g_buildings;
    int32_t n = Collection_Count(blds);
    int32_t start = SimRand(0, n - 1);
    float best = -1.0f;
    int32_t bestId = -1;
    int32_t r = SimRand(0, 100), range;
    if (r < 0x14) range = 0x32;
    else if (r < 0x1e) range = 0x46;
    else if (r < 0x28) range = 0x64;
    else if (r < 0x32) range = 0x8c;
    else if (r < 0x3c) range = 0xbe;
    else if (r < 0x46) range = 0xfa;
    else if (r < 0x50) range = 0x140;
    else range = r >= 0x5a ? 0x270f : 0x190;
    float fr = (float)((double)(float)range * factor);
    if (Fl(0x5c8780) > fr) fr = 50.0f;
    range = (int32_t)fr;
    int32_t lim = n / 10 + 1;
    bool found = false;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(g_buildings, 0,
                                              Collection_IdAt(g_buildings, 0, (i + start) % n, 0));
        if (F<float>(o, 0x27e + kind * 4) <= Fl(0x5c8598)) continue;
        float oy = (float)Obj_CellY(o);
        double dy = crt::fabs(oy - Obj_CellY(b));
        float ox = (float)Obj_CellX(o);
        int32_t d = Ftol(crt::fabs(ox - Obj_CellX(b)) + dy);
        if (d <= 0xf || d > range) continue;
        if (Obj_Flag3d4(o) != 1 || o == b) continue;
        float v = F<float>(o, 0x27e + kind * 4);
        float rnd = (float)SimRand(1, 0x14);
        v = (float)((double)rnd * v);
        if (F<int32_t>(Obj_Region(o), 0xeb) > 0) v = (float)(v * D(0x5c88f8));
        if (Ftol(v) >= Ftol(best)) {
            bestId = F<int32_t>(o, 0x2a4);
            if (!(v < Fl(0x5c8d4c))) {
                found = true;
                break;
            }
            best = v;
        }
        if (i > lim) {
            found = true;
            break;
        }
    }
    if (!found && bestId == -1) return;
    if (!Obj_SharesRegionKey(b, 0, bestId)) Lot_Create(F<uint32_t>(b, 0x2a4), bestId, kind);
}
RT3_REPLACE(0x40fef0, Building_SpawnLot);

// 0x4101e0. An industry's production step: spawns new lots from its growth rates,
// splits demand over its recipes (by profit when several vehicles/recipes are
// available), consumes inputs from the region and produces outputs, books revenue,
// costs and the owner's profit, and adjusts the two trend values (+0x296 profit,
// +0x29a input supply) that can make the industry announce an expansion. bulk (10x
// levels, no finances) is the catch-up mode.
RT3_NOINLINE void __fastcall Industry_Produce(uint8_t* ind, int, int32_t bulk) {
    typedef uint8_t*(__fastcall * RecFn)(uint8_t*);
    auto recOf = [&]() { return ((RecFn)(*(void***)ind)[0x80 / 4])(ind); };
    if (Obj_Flag3d4(ind) != 1) return;
    if (g_newGame[0x82]) return;
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x4af7)) {
        if (!F<int32_t>(w, 0x46c38)) return;
    } else if (F<int32_t>(w, 0x4aef)) return;
    uint8_t* rec = recOf();
    bool nuclear = false, electric = false, recycle = false;
    if (crt::stricmp((const char*)rec + 4, (const char*)0x5c8e00) == 0) nuclear = true;
    else if (crt::stricmp((const char*)rec + 4, (const char*)0x5c8df0) == 0) electric = true;
    else if (crt::stricmp((const char*)rec + 4, (const char*)0x5c8de0) == 0) recycle = true;
    if (Game_EffectiveVersion() < 0x68) recycle = false;
    float made = 0.0f;
    float scale = (float)F<int32_t>(Obj_Ref3cc(ind), 0x141);
    uint8_t* region = Obj_RegionAt(ind);
    int32_t vehicles = Obj_CountAvailableVehicles(recOf(), 0, F<int32_t>(ind, 0x246) != 0);
    int32_t pop = Collection_Count(g_tracks);
    if (IsMultiplayer()) pop = 1000;
    if (F<int32_t>(g_world, 0x4af7)) pop = 0x514;
    // Rate by the number of industries.
    uint32_t rateBits;
    if (Game_EffectiveVersion() < 0x67) {
        rateBits = pop < 0x96 ? 0x3f000000 : pop < 0x12c ? 0x3f19999a : pop < 0x258 ? 0x3f333333 : pop < 0x384 ? 0x3f4ccccd
                 : pop < 0x4b0 ? 0x3f666666 : pop < 0x5dc ? 0x3f800000 : 0x3f8ccccd;
    } else {
        rateBits = pop < 0x96 ? 0x3f000000 : pop < 0x12c ? 0x3f19999a : pop < 0x258 ? 0x3f333333 : pop < 0x384 ? 0x3f4ccccd
                 : pop < 0x4b0 ? 0x3f5c28f6 : pop < 0x76c ? 0x3f6b851f : pop < 0xbb8 ? 0x3f63d70a : pop < 0x1194 ? 0x3f5c28f6
                 : pop < 0x1770 ? 0x3f547ae1 : pop < 0x1f40 ? 0x3f4ccccd : 0x3f451eb8;
    }
    float rate = BitsF(rateBits);
    uint32_t company = F<uint32_t>(ind, 0x276);
    uint32_t player = company ? F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b) : 0;
    uint32_t cell = Obj_GridByte(ind);
    float mult = Effects_Multiplier(g_world, 0, 0xb1, F<uint32_t>(ind, 0x276), player, cell);
    int32_t e2 = F<int32_t>(rec, 0x78c) ? 0xb2 : 0xb3;
    mult = (float)((double)Effects_Multiplier(g_world, 0, e2, F<uint32_t>(ind, 0x276), player, cell) * mult);
    if (g_world[0x4c8d]) {
        rate = (float)((double)rate + rate);
        mult = (float)((double)mult + mult);
    }
    for (int32_t i = 0; i < 3; ++i) {
        float g = (float)((double)F<float>(ind, 0x28a + i * 4) * *(float*)(0x5f3424 + i * 4) * rate);
        if (!(g > Fl_205(0x5c8598))) continue;
        int32_t k = F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x8e + i * 4) + 0xb4;
        if (k >= 0xb4 && k <= 0xfa)
            g = (float)((double)Effects_Multiplier(g_world, 0, k, F<uint32_t>(ind, 0x276), player, cell) * g);
        g = (float)((double)g * mult);
        if (i == 0) {
            int32_t y = F<uint16_t>(g_world, 0xd);
            if (y >= 0x7a6) g = (float)(g * D(0x5c8dd8));
            else if (y >= 0x780) g = (float)(g * D(0x5c8dd0));
        }
        for (;;) {
            if (!(g > D(0x5c8770))) {
                float r = (float)SimRand(0, 1000);
                if (!((double)g * Fl_205(0x5c86a0) >= r)) break;
            }
            g = (float)(g - D(0x5c8770));
            Building_SpawnLot(ind, 0, i, rate);
        }
    }
    float produced = 0.0f, revenue = 0.0f, costs = 0.0f, value = 0.0f, inputValue = 0.0f;
    float lvl = 1.0f;
    if (F<int32_t>(ind, 0x246)) lvl = (float)(F<float>(rec, 0x47) + D(0x5c8770));
    float worstSupply = 0.0f;
    // Per-recipe demand shares (only with several vehicles available).
    char usable[32], counted[32];
    float share[30], profit[30];
    memset(usable, 0, sizeof usable);
    memset(share, 0, sizeof share);
    memset(profit, 0, sizeof profit);
    if (vehicles > 1) {
        float total = 0.0f, weighted = 0.0f;
        for (int32_t i = 0; i < F<int32_t>(rec, 0x33); ++i) {
            uint8_t* e = F<uint8_t*>(rec, 0x37) + i * 0xbc;
            counted[i] = 0;
            if (!Vehicle_Available(recOf(), 0, e, F<int32_t>(ind, 0x246) != 0)) continue;
            if (F<int32_t>(e, 0) != 0 || F<int32_t>(e, 0x2c) == 0) continue;
            usable[i] = 1;
            counted[i] = 1;
            double per = AtLeastMilli(F<float>(e, 4));
            float need = 0.0f;
            float base = (float)((double)F<float>(rec, 0x2a) / per * lvl * scale / D(0x5c8978));
            base = (float)((double)base * mult);
            float avail = base;
            for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                uint8_t* in = e + 0x44 + j * 0x1c;
                float want = (float)((double)avail * F<float>(in, 4));
                int32_t q = City_CargoQuota(region, 0, F<int32_t>(in, 0));
                float have = F<float>(region, 0x103 + F<int32_t>(in, 0) * 4);
                float qf = (float)q;
                need = (float)((double)qf * F<float>(in, 4) + need);
                if (have < want) avail = (float)((double)have / want * avail);
            }
            if (avail < D(0x5c8dc8)) usable[i] = 0;
            base = (float)AtLeastMilli(base);
            float frac = (float)((double)avail / base);
            float s = Fl_205(0x5c8598) > frac ? 0.0f : frac;
            share[i] = s;
            double per2 = F<float>(e, 4) < D(0x5c85b8) ? 0.01 : (double)F<float>(e, 4);
            float qf = (float)City_CargoQuota(region, 0, F<int32_t>(e, 0x1c));
            float p = (float)(((double)qf * F<float>(e, 4) - need) / per2);
            profit[i] = p;
            if (usable[i]) {
                float v = (float)(base * D(0x5c8758) + s);
                total = (float)((double)v + total);
                share[i] = v;
                weighted = (float)((double)p * v + weighted);
            }
        }
        float avg = (float)(weighted / AtLeastMilli(total));
        avg = (float)AtLeastMilli(avg);
        int32_t n = F<int32_t>(rec, 0x33);
        float sum = 0.0f;
        if (n > 0) {
            for (int32_t i = 0; i < n; ++i) {
                if (!counted[i]) continue;
                if (usable[i]) {
                    float f = (float)(profit[i] / AtLeastMilli(avg));
                    if (f > Fl_205(0x5c878c)) f = 2.0f;
                    else if (f < D(0x5c8628)) f = 0.5f;
                    share[i] = (float)((double)f * share[i]);
                }
                sum = (float)((double)sum + share[i]);
            }
            if (sum <= Fl_205(0x5c8598)) sum = 0.1f;
        } else sum = 0.1f;
        for (int32_t i = 0; i < n; ++i) {
            if (counted[i]) share[i] = (float)((double)share[i] / sum);
            if (!usable[i]) share[i] = BitsF(0x3e4ccccd);
        }
    }
    for (int32_t i = 0; i < F<int32_t>(rec, 0x33); ++i) {
        uint8_t* e = F<uint8_t*>(rec, 0x37) + i * 0xbc;
        float used = 0.0f;
        if (!Vehicle_Available(recOf(), 0, e, F<int32_t>(ind, 0x246) != 0)) continue;
        float worst = 999.0f, inCost = 0.0f;
        float f = (float)((double)lvl * scale / D(0x5c8978));
        if (F<int32_t>(g_world, 0x4af7)) {
            uint8_t b = ind[0x391];
            if (!b) continue;
            f = (float)b;
        }
        uint8_t* wv = g_world;
        if (F<int32_t>(e, 0) != 0) {
            // A raw-material producer: rate by how well the region takes its first input.
            int32_t id = F<int32_t>(e, 0x44);
            float price = F<float>(Collection_Get(g_cargoTypes, 0, id), 0x18);
            double pr = AtLeastMilli(price);
            float x = (float)((double)City_CargoQuota(region, 0, id) / pr);
            if (x < Fl_205(0x5c85b4)) x = (float)((x + D(0x5c8770)) / D(0x5c85d0));
            else if (x < Fl_205(0x5c878c)) x = (float)((x + D(0x5c88c8)) / D(0x5c8660));
            else x = (float)((x + D(0x5c88c8)) / D(0x5c8660) * (x - D(0x5c8770)));
            if (nuclear) x = (float)((x + D(0x5c8dc0)) / D(0x5c8c30));
            else if (electric) x = (float)((x + D(0x5c8698)) / D(0x5c8db8));
            wv = g_world;
            if (!F<int32_t>(wv, 0x4af7)) {
                f = (float)((double)f / x);
                f = (float)((double)f * *(float*)(0x5f337c + F<int32_t>(wv, 0x2d) * 4));
            }
        } else if (F<int32_t>(e, 0x2c) == 0) {
            // No inputs: rate by the region's demand for the output.
            int32_t id = F<int32_t>(e, 0x1c);
            float price = F<float>(Collection_Get(g_cargoTypes, 0, id), 0x18);
            double pr = AtLeastMilli(price);
            float x = (float)((double)City_CargoQuota(region, 0, id) / pr);
            uint8_t* w2 = g_world;
            if (!F<int32_t>(w2, 0x4af7)) {
                double y;
                if (x < Fl_205(0x5c85b4)) y = (x + D(0x5c8770)) / D(0x5c85d0);
                else {
                    double d = (double)x - D(0x5c8770);
                    double dd = D(0x5c8770) < d ? 1.0 : d;
                    y = dd * D(0x5c8988) + D(0x5c8770);
                }
                f = (float)(y * f);
                f = (float)((double)f * *(float*)(0x5f3368 + F<int32_t>(w2, 0x2d) * 4));
            }
            f = (float)((double)f * mult);
            int32_t k = F<int32_t>(e, 0x1c) + 0xb4;
            if (k >= 0xb4 && k <= 0xfa) f = (float)((double)Effects_Multiplier(w2, 0, k, F<uint32_t>(ind, 0x276), player, cell) * f);
        } else {
            // A converter: rate by its recipe size and net value.
            int32_t flag = F<int32_t>(wv, 0x4af7);
            f = (float)((double)F<float>(rec, 0x2a) / AtLeastMilli(F<float>(e, 4)) * f);
            f = (float)((double)f * mult);
            if (flag) f = (float)((double)(int32_t)ind[0x391] / AtLeastMilli(F<float>(e, 4)));
            int32_t k = F<int32_t>(e, 0x1c) + 0xb4;
            if (k >= 0xb4 && k <= 0xfa) f = (float)((double)Effects_Multiplier(wv, 0, k, F<uint32_t>(ind, 0x276), player, cell) * f);
            float net = (float)Recipe_NetValue(ind, 0, e);
            uint8_t* w2 = g_world;
            if (!recycle && !F<int32_t>(w2, 0x4af7)) {
                if (net < Fl_205(0x5c8598)) f = (float)((double)f * D(0x5c8758));
                else {
                    net = (float)((double)net / Fl_205(0x5c8db0));
                    if (net < Fl_205(0x5c85b4)) f = (float)(((double)net / D(0x5c8da8) + D(0x5c8618)) * f);
                    f = (float)((double)f * *(float*)(0x5f3368 + F<int32_t>(w2, 0x2d) * 4));
                }
            }
        }
        float base = f;
        // Pass 0 limits the rate by the region's inputs, pass 1 consumes them.
        for (int32_t pass = 0; pass < 2; ++pass) {
            if (recycle) f = base;
            if (vehicles > 1) {
                uint8_t* e2p = F<uint8_t*>(rec, 0x37) + i * 0xbc;
                if (F<int32_t>(e2p, 0) == 0 && F<int32_t>(e2p, 0x2c) != 0) {
                    if (F<int32_t>(g_world, 0x4af7)) {
                        float b = (float)(int32_t)ind[0x391];
                        float nv = (float)vehicles;
                        f = (float)((double)b / nv);
                    } else if (pass == 0 || (IsMultiplayer() && Game_EffectiveVersion() < 0x69)) {
                        f = (float)((double)f * share[i]);
                    }
                }
            }
            if (pass == 1 && f < Fl_205(0x5c8598)) f = 0.0f;
            for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                uint8_t* in = e + 0x44 + j * 0x1c;
                float t = (float)((double)f * F<float>(in, 4));
                float* stock = (float*)(region + 0x103 + F<int32_t>(in, 0) * 4);
                if (pass == 0) {
                    if (F<int32_t>(g_world, 0x4af7)) {
                        if (!(*stock > t)) *stock = t;
                    } else {
                        float have = *stock;
                        if (have < t) f = (float)((double)have / AtLeastMilli(t) * f);
                    }
                    continue;
                }
                if (t > Fl_205(0x5c8598)) {
                    if (t > *stock) t = *stock;
                    *stock = (float)((double)*stock - t);
                    uint8_t* cg = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<int32_t>(in, 0));
                    costs = (float)((double)t * F<float>(cg, 0x18) + costs);
                    float qf = (float)City_CargoQuota(region, 0, F<int32_t>(in, 0));
                    inCost = (float)((double)qf * t + inCost);
                    F<float>(ind, 0x2c0) = (float)((double)t + F<float>(ind, 0x2c0));
                    made = (float)((double)t + made);
                    used = (float)((double)t + used);
                    Cargo_MoveTo62(Collection_Get(g_cargoTypes, 0, F<int32_t>(in, 0)), 0, t);
                    float left = (float)((double)*stock / AtLeastMilli(t));
                    if (Fl_205(0x5c8c28) < left) left = 100.0f;
                    if (left < worst) worst = left;
                }
                float lv = (float)(bulk ? 10 : 1);
                Obj_AddLevel(region, 0, F<int32_t>(in, 0), (float)((double)lv * F<float>(in, 4) * base));
            }
        }
        float out = 0.0f;
        if (recycle) out = (float)(used * D(0x5c8628));
        if (F<int32_t>(e, 0) != 0) {
            if (f > Fl_205(0x5c8598) && F<int32_t>(rec, 0x78c)) {
                float gain, waste;
                bool power = true;
                if (nuclear) {
                    gain = (float)(made * Fl_205(0x5c8da4));
                    gain = (float)((inCost * D(0x5c8570) + gain) / D(0x5c85d0));
                    waste = (float)(made * D(0x5c8610));
                } else if (electric) {
                    gain = (float)(made * Fl_205(0x5c8da0));
                    waste = made;
                    gain = (float)((inCost * D(0x5c8570) + gain) / D(0x5c85d0));
                } else power = false;
                if (power) {
                    if (!(waste == Fl_205(0x5c8598))) {
                        waste = (float)(waste * Fl_205(0x5c8784));
                        F<float>(ind, 0x2e8) = (float)((double)waste + F<float>(ind, 0x2e8));
                        uint8_t* w3 = g_world;
                        F<float>(w3, 0x4caa) = (float)((double)waste + F<float>(w3, 0x4caa));
                        if (F<int32_t>(ind, 0x276)) {
                            uint8_t* oc = (uint8_t*)Obj_OwnerCompany(ind);
                            F<float>(oc, 0xd5d) = (float)((double)waste + F<float>(oc, 0xd5d));
                        }
                    }
                    if (!(gain == Fl_205(0x5c8598))) {
                        value = (float)((double)gain + value);
                        F<float>(ind, 0x310) = (float)((double)gain + F<float>(ind, 0x310));
                        F<float>(ind, 0x338) = (float)((double)inCost + F<float>(ind, 0x338));
                        revenue = (float)((double)gain + revenue);
                        inputValue = (float)((double)inCost + inputValue);
                    }
                }
            }
        } else if (!recycle) out = (float)((double)f * F<float>(e, 4));
        if (out > Fl_205(0x5c8598)) {
            F<float>(ind, 0x2e8) = (float)((double)out + F<float>(ind, 0x2e8));
            float earned = (float)City_ProduceCargo(region, 0, F<int32_t>(e, 0x1c), out);
            if (recycle) earned = (float)(earned * D(0x5c8d98) + inCost);
            uint8_t* cg = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<int32_t>(e, 0x1c));
            value = (float)((double)out * F<float>(cg, 0x18) + value);
            F<float>(ind, 0x310) = (float)((double)earned + F<float>(ind, 0x310));
            F<float>(ind, 0x338) = (float)((double)inCost + F<float>(ind, 0x338));
            revenue = (float)((double)earned + revenue);
            inputValue = (float)((double)inCost + inputValue);
            produced = (float)((double)out + produced);
        }
        if (worst < Fl_205(0x5c8d90) && !(worstSupply > worst)) worstSupply = worst;
    }
    float upkeep = (float)F<uint32_t>(rec, 0x22);
    if (F<int32_t>(ind, 0x246)) {
        float extra = (float)F<int32_t>(rec, 0x43);
        upkeep = (float)((double)extra + upkeep);
    }
    if (bulk) goto done;
    {
        float scale2 = scale;
        if (rec[0x32] == 2) {
            // Processing industries also pay running costs that follow their margins.
            float margin = (float)((double)value - costs);
            float den = Fl_205(0x5c85b4) > value ? 1.0f : value;
            float cover = (float)(D(0x5c8770) - (double)costs / den);
            if (cover < Fl_205(0x5c8598)) cover = 0.0f;
            else if (cover > Fl_205(0x5c85b4)) cover = 1.0f;
            float ratio = 1.0f;
            if (!(revenue == Fl_205(0x5c8598))) {
                float den2 = Fl_205(0x5c85b4) > margin ? 1.0f : margin;
                ratio = (float)(((double)revenue - inputValue) / den2);
            }
            float k = nuclear ? BitsF(0x3f19999a) : 0.5f;
            if (electric) k = (float)(k * D(0x5c85a8));
            if (ratio < Fl_205(0x5c85b4)) k = (float)((D(0x5c8770) - ratio) * D(0x5c88e8) + k);
            else {
                double r = ratio > D(0x5c85d0) ? 2.0 : (double)ratio;
                k = (float)(k - (r - D(0x5c8770)) * D(0x5c86b8));
            }
            float c = (float)(cover - D(0x5c88c8));
            c = (float)(c < D(0x5c86b8) ? 0.1 : (double)c);
            k = (float)((double)c / D(0x5c8628) * k);
            scale2 = (float)((double)scale * F<float>(rec, 0x2a) / D(0x5c8978));
            if (F<int32_t>(ind, 0x246)) scale2 = (float)((F<float>(rec, 0x47) + D(0x5c8770)) * scale2);
            double outDen = produced < D(0x5c88a0) ? 0.0001 : (double)produced;
            float perUnit = (float)(((double)revenue - inputValue) / outDen);
            float run = (float)((double)perUnit * k * produced);
            float floorRun = (float)((double)scale2 * Fl_205(0x5c85b0));
            if (!(run > floorRun)) run = floorRun;
            inputValue = (float)((double)run + inputValue);
            F<float>(ind, 0x360) = (float)((double)run + F<float>(ind, 0x360));
        }
        float maint = (float)(upkeep * D(0x5c8d88));
        if (nuclear || electric) maint = (float)((double)maint * Fl_205(0x5c8900));
        F<float>(ind, 0x360) = (float)((double)maint + F<float>(ind, 0x360));
        float spent = (float)((double)maint + inputValue);
        float net = (float)((double)revenue - spent);
        if (F<int32_t>(ind, 0x276) && !((int32_t(__fastcall*)(uint8_t*))(*(void***)ind)[0x70 / 4])(ind))
            Company_PostStat(Collection_Get(g_companies, 0, F<uint32_t>(ind, 0x276)), 0, 3, net, 1);
        if (F<uint16_t>(ind, 0x26e)) {
            uint8_t* city = (uint8_t*)Collection_Get(g_cities, 0, F<uint16_t>(ind, 0x26e));
            if (F<int32_t>(rec, 0x790)) {
                F<float>(city, 0x2b2) = (float)((double)revenue + F<float>(city, 0x2b2));
                F<float>(city, 0x2b6) = (float)((double)net + F<float>(city, 0x2b6));
            }
        }
        if (F<int32_t>(ind, 0x276) || F<int32_t>(ind, 0x246) || !F<int32_t>(recOf(), 0x4b)) goto done;
        float den = Fl_205(0x5c85b4) > spent ? 1.0f : spent;
        float m = (float)((double)net / den);
        float& trend = F<float>(ind, 0x296);
        if (m >= D(0x5c8770)) trend = (float)((double)trend + Fl_205(0x5c878c));
        else if (m >= D(0x5c8d80)) trend = (float)((double)trend + Fl_205(0x5c85b4));
        else if (!(m >= Fl_205(0x5c8598))) {
            if (m >= D(0x5c8d78)) trend = (float)((double)trend - Fl_205(0x5c8900));
            else trend = (float)((double)trend - Fl_205(0x5c89a4));
        }
        if (trend < Fl_205(0x5c8d74)) F<uint32_t>(ind, 0x296) = 0xc0c00000;
        else if (trend >= Fl_205(0x5c8d70)) F<uint32_t>(ind, 0x296) = 0x42100000;
        float& supply = F<float>(ind, 0x29a);
        if (worstSupply >= D(0x5c8978)) supply = (float)((double)supply + D(0x5c8578));
        else if (worstSupply >= D(0x5c8d68)) supply = (float)((double)supply + Fl_205(0x5c85b4));
        else if (!(worstSupply >= Fl_205(0x5c878c))) {
            if (worstSupply >= D(0x5c8628)) supply = (float)((double)supply - Fl_205(0x5c8900));
            else supply = (float)((double)supply - Fl_205(0x5c89a4));
        }
        if (supply < Fl_205(0x5c8d74)) F<uint32_t>(ind, 0x29a) = 0xc0c00000;
        else if (supply >= Fl_205(0x5c8d70)) F<uint32_t>(ind, 0x29a) = 0x42100000;
        if (trend > Fl_205(0x5c8d60) && supply >= Fl_205(0x5c8d60) && worstSupply >= Fl_205(0x5c85f8)) {
            float chance = (float)((((double)trend - Fl_205(0x5c8d60)) + ((double)supply - Fl_205(0x5c8d60))) / D(0x5c8d58) * D(0x5c8d50));
            float roll = (float)SimRand(0, 1000);
            if (roll < chance) {
                // "<industry> in <city> is expanding" news.
                const char* name = LocalizeName((const char*)recOf() + 0x4f);
                float cy = (float)Obj_CellY(ind);
                float cx = (float)Obj_CellX(ind);
                const char* town = Table_NearestName(g_cities, 0, cx, cy);
                char msg[0x184];
                FormatText(msg, GetString(0xb7), name, town, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                Log_Add(g_messageLog, 0, msg, 9, F<int32_t>(ind, 0x2a4), 0);
                ((void(__fastcall*)(uint8_t*))(*(void***)ind)[0x74 / 4])(ind);
            }
        }
    }
done:
    if (F<int32_t>(g_world, 0x4af7)) ind[0x391] = 0;
}
RT3_REPLACE(0x4101e0, Industry_Produce);

// 0x411cb0. Frees the three buffers at +0x79c, +0x7a0 and +0x37.
RT3_NOINLINE void __fastcall Obj_FreeBuffers(void* o) {
    FreeAndNull(&F<void*>(o, 0x79c));
    FreeAndNull(&F<void*>(o, 0x7a0));
    FreeAndNull(&F<void*>(o, 0x37));
}
RT3_REPLACE(0x411cb0, Obj_FreeBuffers);

// 0x411ce0. For kind 2: +0x790 = some entry (0xbc bytes, +0x37, count +0x33) has
// first dword 0; +0x78c = no entry has +0x2c == 0, or +0x790 is clear.
RT3_NOINLINE void __fastcall Obj_UpdateEntryFlags(void* o) {
    F<int32_t>(o, 0x78c) = 0;
    F<int32_t>(o, 0x790) = 0;
    if (F<uint8_t>(o, 0x32) != 2) return;
    F<int32_t>(o, 0x78c) = 1;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc) {
        if (F<int32_t>(e, 0) == 0) F<int32_t>(o, 0x790) = 1;
        if (F<int32_t>(e, 0x2c) == 0) F<int32_t>(o, 0x78c) = 0;
    }
    if (F<int32_t>(o, 0x790) == 0) F<int32_t>(o, 0x78c) = 1;
}
RT3_REPLACE(0x411ce0, Obj_UpdateEntryFlags);

// 0x411d50. All vehicle entries unused (+0 == 0) and at least one undefined (+0x2c == 0).
RT3_NOINLINE int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o) {
    int32_t found = 0;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc) {
        if (F<int32_t>(e, 0) != 0) return 0;
        if (F<int32_t>(e, 0x2c) == 0) found = 1;
    }
    return found ? 1 : 0;
}
RT3_REPLACE(0x411d50, Obj_AllUnusedSomeUndefined);

RT3_NOINLINE void __fastcall Obj_Nop411d90(void*) { __asm nop }
RT3_REPLACE(0x411d90, Obj_Nop411d90);

// 0x411da0. Entry +0x798 of the static table 0x5ed33c (9 dwords each):
// copies its name, outputs its two values; 0 if none.
int32_t __fastcall Obj_StaticEntry(void* o, int, char* name, int32_t* a, int32_t* b) {
    int32_t i = F<int32_t>(o, 0x798);
    if (i == -1) return 0;
    const int32_t* rec = (const int32_t*)0x5ed33c + i * 9;
    if (rec[1] == 0) return 0;
    const char* s = (const char*)rec[0];
    while ((*name++ = *s++) != 0) {}
    *a = rec[2];
    *b = rec[3];
    return 1;
}
RT3_REPLACE(0x411da0, Obj_StaticEntry);

// 0x411e10 (vtable). The positive count of type record +0x798 (36-byte
// records at 0x5ed34c) gives its value (+0xc) in *out; 0 when none.
RT3_NOINLINE int32_t __fastcall Obj_TypeValue(uint8_t* o, int, int32_t* out) {
    const int32_t i = F<int32_t>(o, 0x798);
    if (i == -1 || *(const int32_t*)(0x5ed34c + i * 36) <= 0) return 0;
    *out = *(const int32_t*)(0x5ed358 + i * 36);
    return 1;
}
RT3_REPLACE(0x411e10, Obj_TypeValue);

// 0x411e50 (vtable). The type record (+0x798, 36 bytes at 0x5ed33c) with a
// positive count: its name, two values and the count. 0 when none.
RT3_NOINLINE int32_t __fastcall Obj_TypeRecord(uint8_t* o, int, char* name, int32_t* a, int32_t* b, int32_t* count) {
    const int32_t i = F<int32_t>(o, 0x798);
    if (i == -1 || *(const int32_t*)(0x5ed34c + i * 36) <= 0) return 0;
    strcpy(name, *(const char* const*)(0x5ed33c + i * 36));
    *a = *(const int32_t*)(0x5ed350 + F<int32_t>(o, 0x798) * 36);
    *b = *(const int32_t*)(0x5ed354 + F<int32_t>(o, 0x798) * 36);
    *count = *(const int32_t*)(0x5ed34c + F<int32_t>(o, 0x798) * 36);
    return 1;
}
RT3_REPLACE(0x411e50, Obj_TypeRecord);

// 0x411ee0. Rebuilds the lists of cargo types the industry (+0x37: +0x33 entries
// of 0xbc bytes with cargo items +0x44, 0x1c each) can ever (+0x79c) and currently
// (+0x7a0, within the entry's years +0x20..+0x22) handle.
RT3_NOINLINE void __fastcall Industry_BuildCargoLists(uint8_t* o) {
    FreeAndNull((void**)(o + 0x79c));
    FreeAndNull((void**)(o + 0x7a0));
    F<int32_t>(o, 0x7a4) = 0;
    F<int32_t>(o, 0x7a8) = 0;
    uint8_t marks[0x35] = {};
    int32_t year = World() ? F<uint16_t>(World(), 0xd) : 0x709;
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0x37) + i * 0xbc;
        int32_t n = F<int32_t>(e, 0x2c);
        if (n <= 0) continue;
        int32_t from = F<int16_t>(e, 0x20);
        for (int32_t j = 0; j < n; ++j) {
            int32_t k = F<int32_t>(e, 0x44 + j * 0x1c);
            int32_t v = (year >= from && year <= F<int16_t>(F<uint8_t*>(o, 0x37) + i * 0xbc, 0x22)) ? 2 : 1;
            if (!(marks[k] > v)) marks[k] = (uint8_t)v;
        }
    }
    for (int k = 0; k < 0x35; ++k) {
        if (marks[k] >= 1) F<int32_t>(o, 0x7a4)++;
        if (marks[k] >= 2) F<int32_t>(o, 0x7a8)++;
    }
    F<uint16_t*>(o, 0x79c) = (uint16_t*)crt::malloc(F<int32_t>(o, 0x7a4) * 2 + 1);
    F<uint16_t*>(o, 0x7a0) = (uint16_t*)crt::malloc(F<int32_t>(o, 0x7a8) * 2 + 1);
    for (int32_t k = 0; k < F<int32_t>(o, 0x7a4); ++k) F<uint16_t*>(o, 0x79c)[k] = 0;
    for (int32_t k = 0; k < F<int32_t>(o, 0x7a8); ++k) F<uint16_t*>(o, 0x7a0)[k] = 0;
    if (F<int32_t>(o, 0x7a4) <= 0 && F<int32_t>(o, 0x7a8) <= 0) return;
    int32_t a = 0, b = 0;
    for (int k = 0; k < 0x35; ++k) {
        if (marks[k] >= 1) F<uint16_t*>(o, 0x79c)[a++] = (uint16_t)k;
        if (marks[k] >= 2) F<uint16_t*>(o, 0x7a0)[b++] = (uint16_t)k;
    }
}
RT3_REPLACE(0x411ee0, Industry_BuildCargoLists);

// 0x4120b0. Loads a building type from file `path` (type id `id`, +0xe4): name and
// header, the production recipes (+0x37, 0xbc each; cargo names resolved, minimum
// year raised to theirs), from version 1005 the extra settings (1010+: 0x24 bytes of
// flags at +0xc0); the build cost of ones with +0x4b follows the capacity. Then its
// cargo lists and flags are rebuilt and +0x798 set to its slot in the art table
// 0x5ed338 (ports and warehouses by their prefix), or -1.
RT3_NOINLINE void __fastcall BuildingType_Load(uint8_t* o, int, const char* path, int32_t id) {
    F<int32_t>(o, 0xe4) = id;
    for (int32_t k = 0; k < 5; ++k) F<int32_t>(o, 0x7b4 + k * 4) = 0;
    F<int32_t>(o, 0x79c) = 0;
    F<int32_t>(o, 0x7a0) = 0;
    F<int32_t>(o, 0x78c) = 0;
    F<int32_t>(o, 0x790) = 0;
    F<int32_t>(o, 0x794) = 0;
    F<int32_t>(o, 0x7b0) = 0;
    F<int32_t>(o, 0x788) = -1;
    Stream st = StreamOpen(path, 1, 0x30d40, 1);
    auto rd = [&](uint32_t off, uint32_t n) { StreamRead(st, o + off, n, 0, 1); };
    int32_t version;
    StreamRead(st, &version, 4, 0, 1);
    rd(4, 0x1e);
    rd(0x22, 4);
    rd(0x26, 2);
    rd(0x28, 2);
    rd(0x2a, 4);
    rd(0x2e, 4);
    rd(0x32, 1);
    rd(0x33, 4);
    F<uint8_t*>(o, 0x37) = (uint8_t*)crt::malloc(F<int32_t>(o, 0x33) * 0xbc + 1);
    Collection* cargos = g_cargoTypes;
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i) {
        uint8_t* rec = F<uint8_t*>(o, 0x37) + i * 0xbc;
        memset(rec, 0, 0xbc);
        StreamRead(st, F<uint8_t*>(o, 0x37) + i * 0xbc, 0xbc, 0, 1);
        rec = F<uint8_t*>(o, 0x37) + i * 0xbc;
        if (F<int32_t>(rec, 0) == 0) {
            F<uint32_t>(rec, 0x1c) = Table_FindByName(cargos, 0, (const char*)rec + 8);
            RaiseYear(rec, F<uint32_t>(rec, 0x1c));
        }
        for (int32_t k = 0; k < F<int32_t>(F<uint8_t*>(o, 0x37) + i * 0xbc, 0x2c); ++k) {
            rec = F<uint8_t*>(o, 0x37) + i * 0xbc;
            F<uint32_t>(rec, 0x44 + k * 0x1c) = Table_FindByName(cargos, 0, (const char*)rec + 0x30 + k * 0x1c);
            RaiseYear(F<uint8_t*>(o, 0x37) + i * 0xbc, F<uint32_t>(F<uint8_t*>(o, 0x37) + i * 0xbc, 0x44 + k * 0x1c));
        }
    }
    if (version >= 0x3ed) {
        rd(0x3b, 4);
        rd(0x3f, 4);
        rd(0x43, 4);
        rd(0x47, 4);
        rd(0x4b, 4);
        rd(0x4f, 0x3d);
        rd(0x8c, 1);
        rd(0x8d, 0x14);
        for (int32_t k = 0; k < 3; ++k) {
            rd(0xa1 + k * 4, 4);
            rd(0xad + k * 4, 4);
        }
        rd(0xb9, 1);
        rd(0xba, 1);
        rd(0xbb, 1);
        rd(0xc0, version >= 0x3f2 ? 0x24 : 2);
        if (F<int32_t>(o, 0x4b)) {
            F<uint32_t>(o, 0x47) = 0x3f800000;
            double cap = (double)F<uint32_t>(o, 0x22);
            int32_t cost = Ftol(cap * D(0x5c8da8) / D(0x5c8838)) * 0x1388;
            if (cost < 0x1388) cost = 0x1388;
            F<int32_t>(o, 0x43) = cost;
        }
    }
    StreamClose(st);
    Industry_BuildCargoLists(o);
    Obj_UpdateEntryFlags(o);
    F<int32_t>(o, 0x798) = -1;
    char name[0x20];
    const char* own = (const char*)o + 4;
    if (!crt::strnicmp(own, (const char*)0x5c8f94, 4)) memcpy(name, (const void*)0x5c8f94, 5);
    else if (!crt::strnicmp(own, (const char*)0x5c8e4c, 9)) memcpy(name, (const void*)0x5c8e4c, 10);
    else strcpy(name, own);
    int32_t slot = 0;
    for (uint32_t e = 0x5ed338; e < 0x5edca4; e += 0x24, ++slot)
        if (!crt::stricmp(name, *(const char**)e)) {
            F<int32_t>(o, 0x798) = slot;
            return;
        }
}
RT3_REPLACE(0x4120b0, BuildingType_Load);

// 0x412510. Available: enabled (+0x7ac) and (option +0x17c, or its year (u16 +0x26) reached).
RT3_NOINLINE int32_t __fastcall Obj_Available(void* o) {
    if (F<int32_t>(o, 0x7ac) == 0) return 0;
    if (F<int32_t>(g_app, 0x17c) != 0) return 1;
    uint16_t year = F<int32_t>(g_world, 0x46c38) ? F<uint16_t>(g_world, 0x5) : F<uint16_t>(g_world, 0xd);
    return year >= F<uint16_t>(o, 0x26) ? 1 : 0;
}
RT3_REPLACE(0x412510, Obj_Available);

// 0x412560. Is an engine/car type available: required cargo type enabled (if
// rec[0] == 0), within its year range, not barred by the flag/scenario bits,
// and all its listed cargo types enabled (unless world +0x4afb).
RT3_NOINLINE int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag) {
    const int32_t* avail = (const int32_t*)(hook::As<uint8_t*>(g_cargoTypes) + 0x16e);
    int32_t ok = 0;
    bool pass = true;
    if (F<int32_t>(rec, 0) == 0 && avail[F<int32_t>(rec, 0x1c)] == 0) pass = false;
    if (pass) {
        int32_t year = F<uint16_t>(g_world, 0xd);
        if (year < F<int16_t>(rec, 0x20) || year > F<int16_t>(rec, 0x22)) pass = false;
        else if (F<int32_t>(rec, 0x24) && flag == 0) pass = false;
        else if (F<int32_t>(rec, 0x28) && flag != 0) pass = false;
        else ok = 1;
    }
    if (F<int32_t>(g_world, 0x4afb) == 0 && ok) {
        int32_t n = F<int32_t>(rec, 0x2c);
        uint8_t* e = rec + 0x44;
        for (int32_t i = 0; i < n; ++i, e += 0x1c)
            if (avail[F<int32_t>(e, 0)] == 0) ok = 0;
    }
    return ok;
}
RT3_REPLACE(0x412560, Vehicle_Available);

// 0x4125f0. Vehicle entries (0xbc bytes, +0x37, count +0x33) that are unused
// (+0 == 0), defined (+0x2c) and available.
RT3_NOINLINE int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag) {
    int32_t n = 0;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc)
        if (F<int32_t>(e, 0) == 0 && F<int32_t>(e, 0x2c) != 0 && Vehicle_Available(o, 0, e, flag)) ++n;
    return n;
}
RT3_REPLACE(0x4125f0, Obj_CountAvailableVehicles);

// 0x412650. The industry's yearly cargo figures for (cargo, p) - recomputed once a
// year (+0x788) over its recipes (+0x37, +0x33 x 0xbc) for both p: base output
// (+0xe8), output share of the industry size +0x2a (+0x290), inputs (+0x438) and
// inputs per output share (+0x5e0); each array 0x6a floats by cargo*2 + p.
RT3_NOINLINE void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare) {
    int32_t year = F<uint16_t>(World(), 0xd);
    auto A = [&](uint32_t idx) -> float& { return F<float>(ind, idx * 4); };
    if (year != F<int32_t>(ind, 0x788)) {
        F<int32_t>(ind, 0x788) = year;
        memset(ind + 0xe8, 0, 0x6a * 4);
        memset(ind + 0x290, 0, 0x6a * 4);
        memset(ind + 0x438, 0, 0x6a * 4);
        memset(ind + 0x5e0, 0, 0x6a * 4);
        for (int32_t q = 0; q < 2; ++q) {
            int32_t count = 0;
            for (int32_t i = 0; i < F<int32_t>(ind, 0x33); ++i) {
                uint8_t* e = F<uint8_t*>(ind, 0x37) + i * 0xbc;
                if (!Vehicle_Available(ind, 0, e, q)) continue;
                if (F<int32_t>(e, 0)) {
                    for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                        uint8_t* it = e + 0x44 + j * 0x1c;
                        float& a = A(q + F<int32_t>(it, 0) * 2 + 0x10e);
                        a = (float)((double)F<float>(it, 4) + a);
                    }
                } else if (F<int32_t>(e, 0x2c) > 0) {
                    if (!count) {
                        count = Obj_CountAvailableVehicles(ind, 0, q);
                        if (!count) count = 1;
                    }
                    float s = (float)((double)F<float>(ind, 0x2a) / (float)count);
                    float& a = A(q + F<int32_t>(e, 0x1c) * 2 + 0xa4);
                    a = (float)((double)s + a);
                    double rate = (double)F<float>(e, 4) < D(0x5c86b8) ? 0.1 : (double)F<float>(e, 4);
                    s = (float)((double)s / rate);
                    for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j) {
                        uint8_t* it = e + 0x44 + j * 0x1c;
                        float& b = A(q + F<int32_t>(it, 0) * 2 + 0x178);
                        b = (float)((double)s * F<float>(it, 4) + b);
                    }
                } else {
                    float& a = A(q + F<int32_t>(e, 0x1c) * 2 + 0x3a);
                    a = (float)((double)F<float>(e, 4) + a);
                }
            }
        }
    }
    F<int32_t>(ind, 0x788) = F<uint16_t>(World(), 0xd);
    uint32_t k = p + cargo * 2;
    *(uint32_t*)base = F<uint32_t>(ind, (k + 0x3a) * 4);
    *(uint32_t*)share = F<uint32_t>(ind, (k + 0xa4) * 4);
    *(uint32_t*)in = F<uint32_t>(ind, (k + 0x10e) * 4);
    *(uint32_t*)inShare = F<uint32_t>(ind, (k + 0x178) * 4);
}
RT3_REPLACE(0x412650, Industry_CargoStats);

// 0x412880. ID of the 0x62b2fc record whose +0x173 equals this +0xe4, or 0.
uint32_t __fastcall Obj_FindRecord62b2fc(void* o) {
    for (int32_t i = 0; i < Collection_Count(g_buildingTypes); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(g_buildingTypes, 0, Collection_IdAt(g_buildingTypes, 0, i, 0));
        if (F<int32_t>(r, 0x173) == F<int32_t>(o, 0xe4)) return Collection_IdAt(g_buildingTypes, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x412880, Obj_FindRecord62b2fc);

// 0x4128e0. Sort priority +0xbc: 3 if +0x78c, 4 for kind 2, else 99999;
// overridden by the static name-prefix table 0x5edca8 ({prefix, value} x 15).
RT3_NOINLINE void __fastcall Obj_SetPriority(void* o) {
    F<int32_t>(o, 0xbc) = 0x1869f;
    if (F<int32_t>(o, 0x78c)) F<int32_t>(o, 0xbc) = 3;
    else if (F<uint8_t>(o, 0x32) == 2) F<int32_t>(o, 0xbc) = 4;
    for (const int32_t* e = (const int32_t*)0x5edca8; e < (const int32_t*)0x5edd20; e += 2) {
        const char* prefix = (const char*)e[0];
        size_t n = 0;
        while (prefix[n]) ++n;
        if (crt::strnicmp((const char*)o + 4, prefix, n) == 0) F<int32_t>(o, 0xbc) = e[1];
    }
}
RT3_REPLACE(0x4128e0, Obj_SetPriority);

// 0x412960. The industry's total output of cargo (base + share) for p.
RT3_NOINLINE double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return (double)base + share;
}
RT3_REPLACE(0x412960, Industry_OutputTotal);

// 0x4129a0. The industry's input of cargo per output share for p.
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return inShare;
}
RT3_REPLACE(0x4129a0, Industry_InputShare);

// 0x4129d0. Whether the industry deals in cargo: among its current cargos (+0x7a0),
// producing it (over 0.001), or holding demand / stock of it (+0xad / +0xa1).
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo) {
    for (int32_t i = 0; i < F<int32_t>(ind, 0x7a8); ++i)
        if (F<uint16_t*>(ind, 0x7a0)[i] == cargo) return 1;
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, 0, &base, &share, &in, &inShare);
    if ((double)base + share > D(0x5c8678)) return 1;
    int32_t idx = F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x9a + cargo * 4);
    if (idx == -1) return 0;
    if (F<float>(ind, 0xad + idx * 4) > Fl(0x5c8598)) return 1;
    return F<float>(ind, 0xa1 + idx * 4) > Fl(0x5c8598) ? 1 : 0;
}
RT3_REPLACE(0x4129d0, Industry_HandlesCargo);

// 0x412a90 / 0x413260. Destructors of two Collection subclasses (vtables
// 0x5c93cc / 0x5c9414): clear, then the base destructor.
RT3_NOINLINE void __fastcall Collection5c93cc_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c93cc;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x412a90, Collection5c93cc_Destruct);

// 0x412ab0. Obj_SetPriority on every record.
RT3_NOINLINE void __fastcall Table_SetPriorities(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_SetPriority(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x412ab0, Table_SetPriorities);

// 0x412af0. ID of the record named `name` (+4), first letters compared
// uppercased before the full (exe) stricmp; 0 if none.
RT3_NOINLINE uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        const char* r = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)) + 4;
        if ((char)ToUpper(name[0]) != (char)ToUpper(r[0])) continue;
        if (crt::stricmp(r, name) == 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x412af0, Table_FindByName2);

// 0x412b70. Record named `name`, or NULL.
RT3_NOINLINE void* __fastcall Table_GetByName(Collection* t, int, const char* name) {
    uint32_t id = Table_FindByName2(t, 0, name, 0);
    return id ? Collection_Get(t, 0, id) : nullptr;
}
RT3_REPLACE(0x412b70, Table_GetByName);

RT3_DELETE_RECORD(0x412ba0, Records_Delete412ba0, Obj_FreeBuffers, void*)

// 0x412bd0. Rebuilds the cargo lists of every industry in the collection.
RT3_NOINLINE void __fastcall Industries_BuildCargoLists(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Industry_BuildCargoLists((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x412bd0, Industries_BuildCargoLists);

// 0x412c10. With a world loaded, each record of types c that is not
// flagged (+0xba / +0xbb) and of kind 2 (+0x32) takes its availability
// (+0x7ac) from the world's named setting for its name (+4); the others are
// available (1). Then the cargo types mark theirs (0x41eac0).
RT3_NOINLINE void __fastcall Types_RefreshAvailable(Collection* c) {
    if (!g_world) return;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!r[0xba] && !r[0xbb] && r[0x32] == 2) F<int32_t>(r, 0x7ac) = World_LookupNamed1(g_world, 0, (const char*)r + 4);
        else F<int32_t>(r, 0x7ac) = 1;
    }
    if (Collection* cargo = g_cargoTypes) CargoTypes_MarkAvailable(cargo);
}
RT3_REPLACE(0x412c10, Types_RefreshAvailable);

// 0x412ca0. Weighted random pick (SimRand) of a kind-3 type name for size
// bracket Bracket5(value): weights +0x8d[b], total +0x8c[b]. Falls back to
// "Commercial" (0x5c92e8) after 5 passes.
RT3_NOINLINE void __fastcall Types_PickName(Collection* t, int, char* dst, int32_t value) {
    int32_t b = Bracket5(value);
    int32_t r = SimRand(0, F<int32_t>(t, 0x8c + b * 4) - 1);
    for (int32_t pass = 0; pass < 5; ++pass) {
        for (int32_t i = 0; i < Collection_Count(t); ++i) {
            uint8_t* rec = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
            if (F<uint8_t>(rec, 0x32) != 3) continue;
            r -= F<int32_t>(rec, 0x8d + b * 4);
            if (r < 0) {
                const char* s = (const char*)rec + 4;
                while ((*dst++ = *s++) != 0) {}
                return;
            }
        }
    }
    const uint8_t* def = (const uint8_t*)0x5c92e8; // "Commercial" (11 bytes)
    for (int k = 0; k < 11; ++k) dst[k] = def[k];
}
RT3_REPLACE(0x412ca0, Types_PickName);

// 0x412d70. Rebuilds the 12 port (pass 0, flag +0xba) and warehouse (pass 1, +0xbb)
// industries from the world's templates (+0x1024.., 0x4e1 bytes each: recipe count,
// recipes, size +0x13d4): reuses the one for template k (+0x794) or creates one,
// cloning the last industry of its kind, and names it Port%02d / Warehouse%02d.
RT3_NOINLINE void __fastcall Industries_RebuildPortsWarehouses(Collection* c) {
    if (!World()) return;
    for (int32_t pass = 0; pass < 2; ++pass) {
        int32_t k = 0;
        for (int32_t off = 0; off < 0x3a8c; off += 0x4e1, ++k) {
            int32_t any = -1, exact = -1;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
                if (!(pass == 0 ? e[0xba] : pass == 1 && e[0xbb])) continue;
                any = Collection_IdAt(c, 0, i, 0);
                if (F<int32_t>(e, 0x794) == k) exact = Collection_IdAt(c, 0, i, 0);
            }
            int32_t id = exact;
            uint8_t* e;
            if (id == -1) {
                id = ((int32_t(__thiscall*)(Collection*, int32_t, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, 0, 0, 0, 0);
                e = (uint8_t*)Collection_Get(c, 0, id);
                memset(e, 0, 0x1f2 * 4);
            } else {
                e = (uint8_t*)Collection_Get(c, 0, id);
                FreeAndNull((void**)(e + 0x79c));
                FreeAndNull((void**)(e + 0x7a0));
                FreeAndNull((void**)(e + 0x37));
            }
            if (id != any) memcpy(e, Collection_Get(c, 0, any), 0x1f2 * 4);
            F<void*>(e, 0x79c) = nullptr;
            F<void*>(e, 0x7a0) = nullptr;
            F<int32_t>(e, 0x794) = k;
            F<int32_t>(e, 0xe4) = id;
            int32_t n = F<int32_t>(World(), off + 0x1024);
            F<int32_t>(e, 0x33) = n;
            F<uint8_t*>(e, 0x37) = (uint8_t*)crt::malloc(n * 0xbc + 1);
            for (int32_t j = 0; j < F<int32_t>(e, 0x33); ++j)
                memcpy(F<uint8_t*>(e, 0x37) + j * 0xbc, World() + off + 0x1028 + j * 0xbc, 0xbc);
            F<uint32_t>(e, 0x2a) = F<uint32_t>(World(), off + 0x13d4);
            crt::sprintf((char*)e + 4, (const char*)(e[0xba] ? 0x5c93e8 : 0x5c93d8), k);
            Industry_BuildCargoLists(e);
            Obj_UpdateEntryFlags(e);
        }
    }
}
RT3_REPLACE(0x412d70, Industries_RebuildPortsWarehouses);

// 0x412fb0. Loads every building type file (".\Data\BuildingTypes\*.bca") into
// the table t (a new record each, via its vtable +4), rebuilds the port and
// warehouse lists and the priorities, totals five counters (+0x8c..+0x9c) over the
// kind-3 types, writes the display names (string 0xb53) of the kind-2 types with
// ids up to 0x6e into the table 0x61dbc2 (0x6e bytes each, id at +0x47) and marks
// the available industry types.
RT3_NOINLINE void __fastcall BuildingTypes_LoadAll(Collection* t) {
    char pattern[0x60], file[0xc8];
    WIN32_FIND_DATAA fd;
    FormatText(pattern, (const char*)0x5c93f4, (const char*)0x5c93fc, 0, 0, 0, 0, 0, 0, 0);
    HANDLE h = ((HANDLE(WINAPI*)(const char*, WIN32_FIND_DATAA*))imp::FindFirstFileA)(pattern, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        auto next = ((BOOL(WINAPI*)(HANDLE, WIN32_FIND_DATAA*))imp::FindNextFileA);
        do {
            int32_t id = ((int32_t(__fastcall*)(Collection*, int, int32_t, int32_t, int32_t, int32_t))(*(void***)t)[1])(t, 0, 0, 0, 0, 0);
            FormatText(file, (const char*)0x5c8730, (const char*)0x5c93fc, fd.cFileName, 0, 0, 0, 0, 0, 0);
            BuildingType_Load((uint8_t*)Collection_Get(t, 0, (uint32_t)id), 0, file, id);
        } while (next(h, &fd));
        ((BOOL(WINAPI*)(HANDLE))imp::FindClose)(h);
    }
    Industries_RebuildPortsWarehouses(t);
    Table_SetPriorities(t);
    uint8_t* o = (uint8_t*)t;
    for (int k = 0; k < 5; ++k) F<int32_t>(o, 0x8c + k * 4) = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (r[0x32] != 3) continue;
        for (int k = 0; k < 5; ++k) F<int32_t>(o, 0x8c + k * 4) += F<int32_t>(r, 0x8d + k * 4);
    }
    char name[0x100];
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (r[0x32] != 2 || r[0xba] || r[0xbb]) continue;
        uint32_t id = Collection_IdAt(t, 0, (uint32_t)i, 0);
        if (id > 0x6e) continue;
        const char* loc = LocalizeName((const char*)r + 0x4f);
        FormatText(name, GetString(0xb53), loc, 0, 0, 0, 0, 0, 0, 0);
        StrCopyN((char*)0x61dbc2 + id * 0x6e, name, 0x46);
        *(uint32_t*)(0x61dc09 + id * 0x6e) = id;
    }
    Types_RefreshAvailable(t);
}
RT3_REPLACE(0x412fb0, BuildingTypes_LoadAll);

// 0x4131f0. Constructs the building type table (vtable 0x5c93cc): inline records
// of 0x7c8 bytes, 10 to start, growing by 5, one list; then loads every type.
RT3_NOINLINE Collection* __fastcall BuildingTypes_Construct(Collection* t) {
    Collection_SetVtbl(t);
    *(uint32_t*)t = 0x5c93cc;
    Collection_Init(t, 0, 1, 0x7c8, 10, 5, 1, 0, 0);
    BuildingTypes_LoadAll(t);
    return t;
}
RT3_REPLACE(0x4131f0, BuildingTypes_Construct);

// 0x413230. Constructor of the 0x5c9414 collection (heap records, 10 + 20).
RT3_NOINLINE Collection* __fastcall Collection5c9414_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9414;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x413230, Collection5c9414_Construct);

void __fastcall Collection5c9414_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9414;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x413260, Collection5c9414_Destruct);

// 0x413280. Building table t: loads from stream s (chunks 0x36b1..0x36b3
// around the table of recordSize records); each building gets the vtable of
// a freshly constructed one and loads its own data (vtbl[0x10]), advancing
// the loading bar 20 steps across the table. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Buildings_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t total = StreamSkip4(s, 0x36b1, 0, 0);
    total += Collection_Load(t, 0, s, recordSize);
    total += StreamSkip4(s, 0x36b2, 0, 0);
    int32_t base = g_skipCount;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        alignas(8) uint8_t tmp[0x3f0];
        ModelObj40c950_Construct(tmp);
        F<uint32_t>(b, 0) = F<uint32_t>(tmp, 0);
        total += (*(int32_t(__thiscall**)(uint8_t*, Stream))(F<uint8_t*>(b, 0) + 0x40))(b, s);
        int32_t d = Collection_Count(t) - 1 < 1 ? 1 : Collection_Count(t) - 1;
        g_skipCount = i * 0x14 / d + base;
        Loading_Pump(g_app, 0, 0, 0);
        ModelObj40c950_Destructor(tmp);
    }
    total += StreamSkip4(s, 0x36b3, 0, 0);
    return total;
}
RT3_REPLACE(0x413280, Buildings_Load);

// 0x4133b0. Building table t: restores the site of every queued building
// (0x62ba64 of them, popped from 0x62ba70), pumping the loading screen, then
// regrounds every placed building (0x62b26c).
RT3_NOINLINE void __fastcall Buildings_RestoreQueued(Collection* t) {
    for (int32_t n = GetQueueCount62ba64(); n > 0; --n) {
        Obj_RestoreSite((uint8_t*)Collection_Get(t, 0, (uint32_t)PopQueue62ba70()));
        Loading_Pump(g_app, 0, 0, 0);
    }
    for (int32_t i = 0; i < Collection_Count(g_buildings); ++i) {
        Collection* bs = g_buildings;
        Obj_Reground((uint8_t*)Collection_Get(bs, 0, Collection_IdAt(bs, 0, (uint32_t)i, 0)));
    }
}
RT3_REPLACE(0x4133b0, Buildings_RestoreQueued);

// 0x413440. Buildings save: tags 0x36b1/0x36b2/0x36b3, records saving
// themselves, with debug progress lines.
RT3_NOINLINE void __fastcall Buildings_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x36b1);
    Collection_Save(t, 0, s, recordSize);
    Save_LogProgress(s, (const char*)0x5c9438);
    StreamWriteU32(s, 0x36b2);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    Save_LogProgress(s, (const char*)0x5c9420);
    StreamWriteU32(s, 0x36b3);
}
RT3_REPLACE(0x413440, Buildings_Save);

// 0x4134d0. Building collection c: adds a new building (a 0x3e1-byte record
// copied from a freshly constructed template, which is then destroyed) and
// sets it up as type t at (x, y) turned by `angle`. Returns its id.
// QUIRK: the last two arguments arrive swapped (place / a7 of Building_Setup).
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place) {
    uint8_t* tmp = (uint8_t*)crt::operator_new(0x3e1);
    if (tmp) tmp = ModelObj40c950_Construct(tmp);
    uint32_t id = Collection_Allocate(c, 0, tmp, 0x3e1, 0, 0);
    uint8_t* o = (uint8_t*)Collection_Get(c, 0, id);
    Building_Setup(o, 0, (int32_t)id, t, x, y, angle, place, a7);
    if (tmp) {
        ModelObj40c950_Destructor(tmp);
        crt::operator_delete(tmp);
    }
    return id;
}
RT3_REPLACE(0x4134d0, Buildings_Add);

// 0x413550 / 0x487db0 / 0x4a4300 (collection vtables 0x5c941c / 0x5cf114 /
// 0x5d0048). Remove record id: tear it down, then free its slot (0x4a4300
// only for a valid id, and it marks the collection changed, +0x8c).
void __fastcall Buildings_Remove(Collection* c, int, uint32_t id) {
    Building_Teardown((uint8_t*)Collection_Get(c, 0, id));
    Collection_Release(c, 0, id);
}
RT3_REPLACE(0x413550, Buildings_Remove);

// 0x413580. Runs the production step for a quarter of the industries, the quarter
// picked by the week of the month (day / 7), and rerolls their animation offsets.
void __fastcall Industries_ProduceQuarter(Collection* t) {
    int32_t n = Collection_Count(t);
    int32_t week = g_world[0x11] / 7;
    for (int32_t i = week % 4; i < n; i += 4) {
        uint8_t* ind = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (Obj_Flag3d4(ind) != 1) continue;
        Industry_Produce(ind, 0, 0);
        F<int32_t>(ind, 0x3d9) = Rand(0, 9);
        F<int32_t>(ind, 0x3dd) = Rand(0, 9);
    }
}
RT3_REPLACE(0x413580, Industries_ProduceQuarter);

// 0x413620. Obj_RefreshHighlight on every record.
RT3_NOINLINE void __fastcall Table_RefreshAll(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_RefreshHighlight(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x413620, Table_RefreshAll);

// 0x413660. Colours the cities with a boom (+0x276): hot for the given boom value,
// cold for the others (vtable +0x58 takes r, g, b).
void __fastcall Cities_ColorBooms(Collection* c, int, int32_t value) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        int32_t v = F<int32_t>(e, 0x276);
        if (!v) continue;
        uint32_t col = HeatColor((float)(v == value ? 1 : 0));
        ((void(__thiscall*)(void*, uint32_t, uint32_t, uint32_t))Vt(e)[0x58 / 4])(e, col >> 16, col >> 8, col);
    }
}
RT3_REPLACE(0x413660, Cities_ColorBooms);

// 0x4136e0. Colours each idle (vtable +0x70) building of a producing industry
// (+0x3f) by its profit score (vtable +0x54).
void __fastcall Buildings_ColorProfit(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (((int32_t(__thiscall*)(void*))Vt(e)[0x70 / 4])(e)) continue;
        uint8_t* ind = ((uint8_t*(__thiscall*)(void*))Vt(e)[0x80 / 4])(e);
        if (!F<int32_t>(ind, 0x3f)) continue;
        float s = Building_ProfitScore(e);
        ((void(__thiscall*)(void*, uint32_t))Vt(e)[0x54 / 4])(e, HeatColor(s));
    }
}
RT3_REPLACE(0x4136e0, Buildings_ColorProfit);

// 0x413750. Refreshes every building's highlight for `key` (0: any): lit
// (virtual slot 21, world colour) when an active site is near (kind-4
// buildings) or its region lists the key; otherwise slot 22 / 0x40d2d0.
void __fastcall Buildings_RefreshHighlight(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        uint8_t* r = Obj_Region(b);
        void** vt = *(void***)b;
        bool lit;
        if (Obj_StateKindIs4(b)) {
            lit = Table_ActiveSiteNear(g_stations, 0, F<uint32_t>(b, 0x2a4), key) != 0;
            if (!lit) {
                ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))vt[0x58 / 4])(b, 0, 0, 0);
                continue;
            }
        } else {
            lit = false;
            for (int32_t k = 0; k < F<int32_t>(r, 0xeb); ++k)
                if (!key || F<uint16_t>(F<uint8_t*>(r, 0xef), k * 6) == key) lit = true;
            if (!lit) {
                Obj_RefreshHighlight(b);
                continue;
            }
        }
        ((void(__thiscall*)(void*, int32_t))vt[0x54 / 4])(b, F<int32_t>(g_world, 0x66ba));
    }
}
RT3_REPLACE(0x413750, Buildings_RefreshHighlight);

// 0x413860. With option +0x1c3, 0x40cd10 on every record.
void __fastcall Table_TouchRegionSites(Collection* t) {
    if (!F<int32_t>(g_app, 0x1c3)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_TouchRegionSites(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x413860, Table_TouchRegionSites);

RT3_NOINLINE void __fastcall Buildings_DropSharedDemand(Collection* t, int, uint16_t k) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Building_DropSharedDemand((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, k);
}
RT3_REPLACE(0x4138b0, Buildings_DropSharedDemand);

// 0x4138f0. Number of records with +0x3d0 == v.
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3d0) == v) ++n;
    return n;
}
RT3_REPLACE(0x4138f0, Table_CountField3d0);

// 0x413940. Sum over records with +0x3d0 == v of max(1, their 0x62b2fc record's +0x141).
int32_t __fastcall Table_SumCapacity(Collection* t, int, int32_t v) {
    int32_t sum = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x3d0) != v) continue;
        int32_t c = F<int32_t>(Obj_Ref3cc(r), 0x141);
        sum += c < 1 ? 1 : c;
    }
    return sum;
}
RT3_REPLACE(0x413940, Table_SumCapacity);

// 0x4139a0. Records with flag +0x3d4 == 1 that pass Obj_CheckActive.
RT3_NOINLINE int32_t __fastcall Table_CountActive(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Obj_Flag3d4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) != 1) continue;
        if (Obj_CheckActive(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) ++n;
    }
    return n;
}
RT3_REPLACE(0x4139a0, Table_CountActive);

// 0x413a00 / 0x413aa0. The number of records of c in building class 3 / 4.
RT3_NOINLINE int32_t __fastcall Records_CountClass3(Collection* c) { return Count_RecordsOfClass(c, 3); }
RT3_REPLACE(0x413a00, Records_CountClass3);

RT3_NOINLINE int32_t __fastcall Records_CountClass4(Collection* c) { return Count_RecordsOfClass(c, 4); }
RT3_REPLACE(0x413aa0, Records_CountClass4);

// 0x413b40. Records with flag +0x3d4 == 1, state kind 4 and state byte +0xb9 == 0.
RT3_NOINLINE int32_t __fastcall Table_CountKind4Idle(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Obj_Flag3d4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) != 1) continue;
        if (!Obj_StateKindIs4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) continue;
        if (Obj_StateB9(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x413b40, Table_CountKind4Idle);

// 0x413bc0. The 2D model file of the building type into out: "2D<name>.imb" if it
// exists, else the house set model of its style (+0x7c "<style>houses"; multi-family
// from size 2, +0x141), else the commercial S/M/L model for names CommercialX00..09,
// else the default building.
void __fastcall BuildingType_Model2D(uint8_t* o, int, char* out) {
    char buf[0x120];
    const char* name = (const char*)o + 0x40;
    Format0(out, 0x5c9510, name);
    if (NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) return;
    for (uint32_t a = 0x5f3c80; a < 0x5f3c98; a += 4) {
        const char* style = *(const char* const*)a;
        crt::sprintf(buf, (const char*)0x5c9504, style);
        if (crt::stricmp((const char*)o + 0x7c, buf)) continue;
        Format0(out, F<int32_t>(o, 0x141) >= 2 ? 0x5c94ec : 0x5c94d4, *(const char* const*)a);
    }
    if (NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) return;
    static const uint32_t pat[3] = { 0x5c94c4, 0x5c94a0, 0x5c947c }, model[3] = { 0x5c94b0, 0x5c948c, 0x5c9468 };
    for (int32_t i = 0; i < 10; ++i)
        for (int k = 0; k < 3; ++k) {
            crt::sprintf(buf, (const char*)pat[k], i);
            if (!crt::stricmp(name, buf)) Format0(out, model[k]);
        }
    if (!NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) Format0(out, 0x5c9450);
}
RT3_REPLACE(0x413bc0, BuildingType_Model2D);

// 0x413d80. If the 0x62b268 record (+0x173) has kind 1: outputs u16 +0xcb/+0xcd, returns 1.
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y) {
    uint8_t* r = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(o, 0x173));
    if (F<uint8_t>(r, 0x32) != 1) return 0;
    *x = F<uint16_t>(o, 0xcb);
    *y = F<uint16_t>(o, 0xcd);
    return 1;
}
RT3_REPLACE(0x413d80, Obj_GetCell);

// 0x413dd0. Its 0x62b268 type (+0x173) is available.
int32_t __fastcall Obj_TypeAvailable(void* o) {
    return Obj_Available(Collection_Get(g_62b268, 0, F<uint32_t>(o, 0x173)));
}
RT3_REPLACE(0x413dd0, Obj_TypeAvailable);

// 0x413df0 body. Adds cell (x, y) with flag a to the dirty-cell list 0x62b318
// (-1 ended, count 0x62b7a4; entries ((x & 0x7fff) | a << 15) << 16 | (y & 0x7fff))
// unless (x << 16 | y) is already there, and widens the dirty block bounds
// (0x62b7a8..0x62b7b4, in 4-cell blocks). 1 when it was added.
int32_t __cdecl DirtyCells_AddImpl(int32_t a, int32_t x, int32_t y) {
    uint32_t key = (uint32_t)x << 16 | (uint32_t)y;
    for (uint32_t* e = (uint32_t*)0x62b318; *e != 0xffffffff; ++e)
        if ((*e & 0x7fffffff) == key) return 0;
    int32_t& n = g_62b7a4;
    ((uint32_t*)0x62b318)[n] = ((uint32_t)(x & 0x7fff) | (uint32_t)a << 15) << 16 | (uint32_t)(y & 0x7fff);
    ++n;
    ((uint32_t*)0x62b318)[n] = 0xffffffff;
    int32_t by = y >> 2, bx = x >> 2;
    if (g_62b7a8 > by) g_62b7a8 = by;
    if (g_62b7ac < by) g_62b7ac = by;
    if (g_62b7b0 > bx) g_62b7b0 = bx;
    if (g_62b7b4 < bx) g_62b7b4 = bx;
    return 1;
}

// 0x413df0. Flag in EAX, x in ECX, y in EDX. Like the original, returns with ECX
// and EDX shifted right by 2 when the cell was added and unchanged otherwise.
__declspec(naked) void DirtyCells_Add() {
    __asm {
        push ecx
        push edx
        push edx
        push ecx
        push eax
        call DirtyCells_AddImpl
        add esp, 12
        pop edx
        pop ecx
        test eax, eax
        jz done
        sar edx, 2
        sar ecx, 2
    done:
        ret
    }
}
RT3_REPLACE(0x413df0, DirtyCells_Add);

// 0x413eb0. Static record (28 x 0xb0 bytes at 0x5ef3e0) whose name (+0x10) equals obj +0x123.
void* __fastcall FindStaticRecord(void*, int, uint8_t* obj) {
    for (int k = 0; k < 28; ++k) {
        const char* name = *(const char**)(0x5ef3f0 + k * 0xb0);
        if (name && crt::stricmp(name, (const char*)obj + 0x123) == 0) return (void*)(0x5ef3e0 + k * 0xb0);
    }
    return nullptr;
}
RT3_REPLACE(0x413eb0, FindStaticRecord);

RT3_NOINLINE void FreeBuffer62b2f8() {
    void*& p = g_62b2f8;
    if (p) {
        crt::free(p);
        p = nullptr;
    }
}
RT3_REPLACE(0x413f30, FreeBuffer62b2f8);

// 0x413f50. Pops the next id from the global queue (0x62ba70, count 0x62ba64); -1 if empty.
RT3_NOINLINE int32_t PopQueue62ba70() {
    if (!hook::As<int32_t>(g_62ba6c) || g_62ba64 <= 0) return -1;
    int32_t*& p = g_62ba70;
    int32_t v = *p++;
    g_62ba64--;
    return v;
}
RT3_REPLACE(0x413f50, PopQueue62ba70);

// 0x413f80. Loads the site table: a 12-byte header (count third), the site ids
// (0x62ba6c/0x62ba70) and the 0x74e-byte site records (0x62b2f8) with their
// variable parts. Returns the bytes of the fixed part read.
RT3_NOINLINE int32_t __stdcall Sites_LoadTable(Stream s) {
    int32_t hdr[3] = { 0, 0, 0 };
    StreamRead(s, hdr, 0xc, 0, 1);
    int32_t n = hdr[2];
    g_62ba64 = n;
    if (n <= 0) {
        g_62ba74 = 0;
        g_62ba68 = n;
        return 0xc;
    }
    uint32_t* ids = (uint32_t*)crt::malloc(n * 4);
    g_62ba6c = ids;
    hook::As<uint32_t*>(g_62ba70) = ids;
    if (g_62b2f8) {
        crt::free(g_62b2f8);
        g_62b2f8 = nullptr;
    }
    hook::As<uint8_t*>(g_62b2f8) = (uint8_t*)crt::malloc(g_62ba64 * 0x74e);
    for (int32_t i = 0; i < g_62ba64; ++i) {
        uint8_t* r = hook::As<uint8_t*>(g_62b2f8) + i * 0x74e;
        memset(r, 0, 0x74e);
        StreamRead(s, &g_62ba6c[i], 4, 0, 1);
        static const uint16_t fixed[] = { 4, 8, 0xc, 0x4b8, 0x4bc, 0x49c };
        for (uint16_t off : fixed) StreamRead(s, r + off, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x49c) + 1; ++k) StreamRead(s, r + 0x10 + k * 4, 4, 0, 1);
        StreamRead(s, r + 0x4a0, 0x10, 0, 1);
        StreamRead(s, r + 0x4c4, 4, 0, 1);
        if (F<int32_t>(r, 0x4c4) > 0) {
            void* p = crt::malloc(F<int32_t>(r, 0x4c4));
            F<void*>(r, 0x4c0) = p;
            StreamRead(s, p, F<int32_t>(r, 0x4c4), 0, 1);
        }
        StreamRead(s, r + 0x65c, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x65c); ++k) StreamRead(s, r + 0x4c8 + k * 4, 4, 0, 1);
        StreamRead(s, r + 0x72e, 4, 0, 1);
        for (uint32_t k = 0; k < F<uint32_t>(r, 0x72e); ++k) {
            StreamRead(s, r + 0x664 + k, 1, 0, 1);
            StreamRead(s, r + 0x6c9 + k, 1, 0, 1);
        }
        StreamRead(s, r + 0x732, 4, 0, 1);
    }
    int32_t cnt = g_62ba64;
    g_62ba74 = 0;
    g_62ba68 = cnt;
    return cnt * 4 + 0xc;
}
RT3_REPLACE(0x413f80, Sites_LoadTable);

}  // namespace rt3
