// Train.cpp - RT3.exe 0x4a6000-0x4b2000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/containers.h"
#include "core/ptrlist.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

// How often a path ran where the original reads a local it never wrote (a stack
// leftover of its caller; the rewrite starts the local at 0): [0] Train_Run's
// arrival test, [1] Train_AppendCar's direction, [2] Car_FollowLeader with the
// leading car off the track, [3] Car_UpdatePose with a bogie off a track end,
// [4] Boat_Update's wake without the W key. The harness logs them.
int32_t g_unwrittenReads[8];

// Rewritten car and train functions this file calls directly.
int32_t __fastcall Car_Advance(uint8_t* c, int, uint32_t dist, int32_t* moved, int32_t* count, int32_t p5);
int32_t __fastcall Car_FollowLeader(uint8_t* c, int, float* pos);
void __fastcall Car_SetPiece(uint8_t* car, int, int32_t piece, int32_t pos);
void __fastcall Car_UpdatePose(uint8_t* car, int, float dist);
uint32_t __fastcall Train_AppendCar(uint8_t* t, int, uint32_t type, uint32_t cargo);
void __fastcall Train_Run(uint8_t* t, int, float slice);

uint32_t __fastcall Cars_Add(Collection* t, int, uint8_t* type, uint32_t owner, float x, float z, uint32_t a6);
uint32_t __fastcall Table_FindCurrentFor(Collection* t, int, int32_t v);

void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);

void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);

void __fastcall Car_LoadAt(uint8_t* car, int, uint8_t* at, uint8_t* to);

int32_t __fastcall Station_PickCargo(uint8_t* st, int, int32_t tag, uint32_t destId, int32_t* lost, int32_t* fallback);

void __fastcall Obj_SetHighlight(uint8_t* o, int, int32_t v);

void* __fastcall Obj_Company21(void* o);

char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);

uint32_t __fastcall CarType_ForCargo(uint8_t* o);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

// Global tables (Collection*) referenced by id from objects.


     // records with +4 != 0 are active

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void* __fastcall Obj_Ref388(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first);
void* __fastcall Cursor33_Init(uint8_t* c);
void* __fastcall Obj_City352(void* o);
double __fastcall Obj_Ref388Float10d(void* o);
int32_t __fastcall Node_GetLink(void* n, int, int32_t first);
void* __fastcall Array33_Current(void* a);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_CityRef10(void* o);
float __fastcall Company_QualityFactor(void* c);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
uint8_t* __fastcall OrderList_Current(uint8_t* o);
int32_t __fastcall Train_Rank(void* self);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Car_RearPoint(uint8_t* c, int, float* x, float* y);
uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o);
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt);
int32_t __stdcall Obj_FarFromSiteBuilding(uint8_t* obj, void* site);
int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void* __fastcall Array33_At(void* a, int, int32_t i);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other);
void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);

uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data);

int32_t IsMultiplayer();
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Obj_SetAllBc(void* o, int32_t v);
void* __fastcall Obj_City41(void* o);
void __fastcall Store3Globals(int32_t* v);
void __fastcall Obj_ApplyStored3(void* o, int32_t idx);
void __fastcall Effect_Free(uint32_t* e);
float __fastcall Train_SumCarValues(uint8_t* t);
int32_t __fastcall Train_IsRunning(void*, int, uint32_t id);
int32_t __fastcall Track_Link2(uint8_t* t, int, int32_t second);
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
void __fastcall Site_RecordMin(uint8_t* o, int, int32_t i, int32_t list, float v);
int32_t __fastcall Obj_BuildingStateActive(void* o);
float __fastcall Obj_NodeCenterY(void* o);
float __fastcall Obj_NodeCenterX(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall Routes_NumberComponents(Collection* c);
int32_t __fastcall Vehicle_SetEngineSound(uint8_t* v, int, float scale, uint32_t volume);
uint32_t __fastcall Obj_NodeVirtual5(void* o);
void __fastcall ListPair_Reset(uint8_t* o);
void __fastcall Train_SetTarget(uint8_t* t, int, float v);
void* __fastcall Obj_BuildingRegionAt(void* o);
void* __fastcall Obj_BuildingKind(void* o);
void __fastcall Station_AddStat49(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat51(uint8_t* o, int, float a, uint8_t* company);
void __fastcall Station_AddStat45(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat4d(uint8_t* o, int, float a, uint8_t* company);
int32_t __fastcall Obj_Get25(void* o);
uint32_t __fastcall Obj_CityGridByte(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Train_AddExpense(uint8_t* t, int, float amount);
void __fastcall Train_EarnRevenue(uint8_t* o, int, float amount);
void __fastcall Obj_MoveChannels(uint8_t* o);
void __fastcall Vehicle_UpdateHorn(uint8_t* v, int, int32_t);
float __fastcall Vehicle_GradeSpeed(uint8_t* o, int, uint8_t* so);
void __fastcall Train_UpdateEngineEffects(uint8_t* t, int, float throttle, float speed);
float __fastcall Car_TiltValue(uint8_t* c);
double __stdcall SpeedFactor(float v);
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
int32_t __fastcall Game_EffectiveVersion();
void __fastcall Car_SetBrokenDown(uint8_t* c, int, int32_t on);
void __fastcall Vehicle_SelectSound(uint8_t* v, int, int32_t kind);
void __fastcall Car_SetCrashed(uint8_t* c, int, int32_t on);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);
void __fastcall Model_SetOpacity(uint8_t* m, int, float a);
void __fastcall Station_RelinkNodes(void* s);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
void __fastcall Obj_IncD0f(void* o);
void __fastcall Obj_IncD0b(void* o);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
float __fastcall Train_RunningCost(uint8_t* t);
int32_t __fastcall Node_TurnsLeft(Collection* nodes, int, uint32_t id);
int32_t __fastcall Node_IsRouteEnd(uint8_t* n);

int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                   int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void __fastcall Station_UnlinkNodes(void* s);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall Model_SetAttachmentsFlag(uint8_t* o, int, int32_t on);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
void* __fastcall Obj_CityRef(void* o);

// RT3_REPLACE(0x55ed9a -- the tail of Movie_Stop above, not a separate function.

int32_t Global6d3b30();

uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i);
int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);

// 0x412c10 is installed as Types_RefreshAvailable (src/game/Buildings.cpp).
// RT3_REPLACE(0x412c10, IndustryTypes_MarkAvailable);

double __fastcall Vec3_Length(const float* v);

float* __fastcall Vec3_Normalize(float* v);
int32_t __fastcall Obj_TestBlock(uint8_t* o, int, uint32_t mask, int32_t idx);
uint32_t __fastcall Obj_SetBlockFlags(uint8_t* o, int, uint32_t mask, int32_t mode);
uint32_t __fastcall Obj_BeginBlocks(uint8_t* o);
void __fastcall Obj_Dec58c(uint8_t* o);
int32_t __fastcall Obj_ClearAttachSlot(uint8_t* o, int, int32_t slot);
void __fastcall Vehicle_PassSignal(uint8_t* o);
void __fastcall Vec3_Sub(const float* a, int, float* out, const float* b);
int32_t __fastcall Train_StateBOr12(uint8_t* t);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
int32_t __fastcall Str_IsNumber(const char* s);
void __fastcall Train_SetRank(uint8_t* t, int, int32_t rank);
int32_t __fastcall Routes_CanMerge(Collection* routes, int, uint8_t* a, uint8_t* b);
void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);

int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign,
                                  int32_t anyTrack);

void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Routes_Merge(Collection* t, int, uint32_t a, uint32_t b);
int32_t __stdcall Net_AllHaveVersion(float v);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t Tutorial_Is1ea();
void __fastcall Train_ClearBreakdown(uint8_t* t);
int32_t __fastcall Train_CheckPendingNode(uint8_t* t);
int32_t __fastcall Train_SignalsClear(uint8_t* t);
void __fastcall Obj_Toggle35a(void* o);
int32_t __fastcall Obj_CityKindIs2(void* o);
float __fastcall Load_PayOut(uint8_t* o);
void __fastcall Train_Whistle(uint8_t* o, int, int32_t n);
void __fastcall Station_ShowCargoMoney(uint8_t* s, int, int32_t amount);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
void __fastcall Car_SetCargoValue(uint8_t* car, int, uint8_t* city);
int32_t __fastcall Obj_BuildingQuarterX(void* o);
int32_t __fastcall Obj_BuildingQuarterY(void* o);
void __fastcall Route_LinkStations(uint8_t* o);
uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
void __fastcall Train_RetargetRegion(uint8_t* t);
double __fastcall Obj_Rate(void* o);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alpha, int32_t deep);
void __fastcall Train_StopAndGhost(uint8_t* train);

int32_t __fastcall Node_Advance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* outPos, int32_t* dir, int32_t* count, float* accum,
                                int32_t car, int32_t, int32_t front);

void __fastcall Train_TickWear(uint8_t* t);
int32_t __fastcall Train_NextNode(uint8_t* t, int, int32_t forward, uint32_t prev, uint32_t curId, int32_t* switched);
int32_t __fastcall Train_StateSpecial(uint8_t* t);
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
uint32_t __fastcall Train_PromptName(uint8_t* t);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
void __fastcall Train_AutoAddCars(uint8_t* t);
void __fastcall Station_LearnDemand(uint8_t* st, int, uint32_t from, uint8_t* consist);
void Tutorial_Advance();
int32_t __fastcall Train_LoadConsist(uint8_t* train, int, uint8_t* station, uint32_t* destination);
void __fastcall Train_DetachCar(uint8_t* t, int, uint32_t id);
void __fastcall Load_Unlink(uint32_t* o);
uint8_t* __fastcall Obj_RegionAt(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind);
void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id);
void __fastcall AiTrain_NextOrder(uint8_t* t);
void __fastcall Train_SetupNextStop(uint8_t* t);
void __fastcall Train_Route(uint8_t* t);
void __fastcall Train_LeaveBlock(uint8_t* t);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
void __fastcall Table_ReleaseKey8(Collection* t, int, int32_t key);
void __fastcall Nop436060(void*, int, int32_t);
void __fastcall Obj_SetTexture(uint8_t* o, int, const char* name);
void __fastcall Train_ApplyLivery(uint8_t* t);
float __fastcall World_Value4cbe(void* w);
void __fastcall Train_ResetStop(uint8_t* t);
void __fastcall Train_ResetCarAnimations(uint8_t* t);
int32_t __fastcall Train_CanReachOrder(uint8_t* t, int, int32_t anyElectric);
void __fastcall Train_PassSignal(uint8_t* t);
void __fastcall Train_Arrive(uint8_t* t);
float __fastcall Train_StepAtStation(uint8_t* t, int, float dt);
void __fastcall Train_Advance(uint8_t* t);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall Train_ScanCarKinds(uint8_t* t);
void __fastcall Train_RefreshRows(uint8_t* t);
void __fastcall Train_EnterBlock(uint8_t* t, int, uint8_t* car, uint8_t* other);
void __fastcall Station_UnlinkNodes(void* t);
template <typename T> static inline T& F_train_initialization(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
template <typename T> static inline T& F_train_loading(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
template <typename T> static inline T& F_train_station(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall AiTrain_NextOrder(uint8_t* t);
RT3_NOINLINE void __fastcall Obj_SetTexture(uint8_t* o, int, const char* name);
RT3_NOINLINE void* __fastcall Obj_Ref388(void* o);
RT3_NOINLINE void __fastcall Nop436060(void*, int, int32_t);
RT3_NOINLINE uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
RT3_NOINLINE double __fastcall Obj_CellX(void* o);
RT3_NOINLINE double __fastcall Obj_CellY(void* o);
RT3_NOINLINE uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed);
RT3_NOINLINE void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
RT3_NOINLINE int32_t __fastcall Routes_Merge(Collection* t, int, uint32_t a, uint32_t b);
int32_t __fastcall Route_Find(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
int32_t __fastcall Routes_SameComponent(Collection* c, int, int32_t a, int32_t b);
RT3_NOINLINE int32_t __fastcall Routes_Component(Collection* c, int, int32_t id);
RT3_NOINLINE void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);
int32_t __fastcall Route_FindForVersion(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
RT3_NOINLINE int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);
void __fastcall Nop4a6e50(void*);
RT3_NOINLINE int32_t __fastcall Obj_ByteF9(uint8_t* o);
void __fastcall Train_AddRevenue(uint8_t* t, int, int32_t amount, int32_t key);
RT3_NOINLINE double __stdcall SpeedFactor(float v);
void __fastcall Vec3_Sub(const float* a, int, float* out, const float* b);
RT3_NOINLINE int32_t __fastcall Train_StateIs7(uint8_t* t);
void __fastcall Train_Clear1a3(uint8_t* t);
void __fastcall Array33_Save(uint32_t* o, int, Stream s);
void __fastcall Train_Whistle(uint8_t* o, int, int32_t n);
void __fastcall Train_SetTarget(uint8_t* t, int, float v);
RT3_NOINLINE void __fastcall Train_ResetStop(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_StateSpecial(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_StateBOr12(uint8_t* t);
int32_t Obj_KindE8Special();
double __fastcall Train_Float109(uint8_t* t, int, int32_t i);
double __fastcall Train_Float131(uint8_t* t, int, int32_t i);
RT3_NOINLINE double __fastcall Train_Float159(uint8_t* t, int, int32_t i);
RT3_NOINLINE double __fastcall Obj_Sum3(void* o, int, int32_t i);
RT3_NOINLINE void __fastcall Train_RollStats(uint8_t* t);
RT3_NOINLINE void __fastcall Nop4a7320(void*);
int32_t __fastcall Train_State1Or2(uint8_t* t);
void __fastcall Nop4a7350(void*);
RT3_NOINLINE int32_t __fastcall Train_IsStopped(uint8_t* t);
RT3_NOINLINE void __fastcall Train_SetBuffer1c6(uint8_t* t, int, const uint8_t* src);
RT3_NOINLINE void __fastcall Obj_Dec58c(uint8_t* o);
uint8_t* __fastcall Cursor33_Construct(uint8_t* c);
void __fastcall Train_Init(uint8_t* t, int, int32_t id, int32_t owner);
RT3_NOINLINE void* __fastcall Train_NthCarKind1(uint8_t* t, int, int32_t n);
RT3_NOINLINE void* __fastcall Train_NthCarOther(uint8_t* t, int, int32_t n);
int32_t __fastcall Train_CountCarsOther(uint8_t* t);
RT3_NOINLINE void* __fastcall Obj_City41(void* o);
RT3_NOINLINE void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y);
RT3_NOINLINE void __fastcall Train_ResetCarAnimations(uint8_t* t);
void Train_StepsToNode();
int32_t __stdcall Obj_FarFromSiteBuilding(uint8_t* obj, void* site);
void __fastcall Train_FindStationAhead(uint8_t* o, int, int32_t* out, int32_t kind, int32_t check);
RT3_NOINLINE int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other);
RT3_NOINLINE float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region);
float __fastcall Train_SumCarValues(uint8_t* t);
int32_t __fastcall Train_IsRunning(void*, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Train_SignalsClear(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_CheckPendingNode(uint8_t* t);
RT3_NOINLINE void __fastcall Station_RelinkNodes(void* s);
RT3_NOINLINE void __fastcall Station_UnlinkNodes(void* s);
void __fastcall Train_SetGhost(uint8_t* train, int, int32_t ghost, int32_t force);
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c);
RT3_NOINLINE void __fastcall Train_RefreshWindowIfShown(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_OrderTarget(uint8_t* t, int, int32_t* dist, int32_t preferStop);
RT3_NOINLINE int32_t __fastcall Train_CurrentOrderNode(uint8_t* t);
int32_t __fastcall Train_LoadExtra(uint8_t* train, int, int32_t st);
RT3_NOINLINE void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
RT3_NOINLINE int32_t __fastcall Train_Rank(void* self);
void __fastcall Station_ShowCargoMoney(uint8_t* s, int, int32_t amount);
RT3_NOINLINE void* __fastcall Obj_CityRef(void* o);
RT3_NOINLINE int32_t __fastcall Train_FirstCarType126(uint8_t* t);
uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE void __fastcall Route_LinkStations(uint8_t* o);
RT3_NOINLINE void* __fastcall Train_Company(void* t);
void __fastcall Train_UpdateEngineEffects(uint8_t* t, int, float throttle, float speed);
void __fastcall Train_SmoothSpeed(uint8_t* t);
RT3_NOINLINE void __fastcall Train_RefreshRows(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Obj_CityRef10(void* o);
void __fastcall Train_ScanCarKinds(uint8_t* t);
void __fastcall Train_DefaultName(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_CanReachOrder(uint8_t* t, int, int32_t anyElectric);
RT3_NOINLINE uint32_t __fastcall Obj_CityGridByte(void* o);
RT3_NOINLINE uint32_t __fastcall Train_GridByteAtCar45(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_TargetStation(uint8_t* t);
RT3_NOINLINE char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
RT3_NOINLINE int32_t __fastcall Train_OnOwnTrack(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Obj_CityKindIs2(void* o);
void __fastcall Obj_ApplyPendingTemplate(uint8_t* o);
float __fastcall Obj_YearlyRatio(uint8_t* o);
RT3_NOINLINE void __fastcall Train_ReanchorConsist(uint8_t* t);
RT3_NOINLINE void __fastcall Train_ApplyLivery(uint8_t* t);
RT3_NOINLINE void __fastcall Train_DetachCar(uint8_t* t, int, uint32_t id);
void __fastcall Train_HeadForNearestStop(uint8_t* t, int, uint8_t* v);
int32_t __fastcall Train_UpdateApproachFactor(uint8_t* o, int, uint8_t* obj);
float __fastcall Vehicle_GradeSpeed(uint8_t* o, int, uint8_t* so);
void __fastcall Train_EnterBlock(uint8_t* t, int, uint8_t* car, uint8_t* other);
RT3_NOINLINE void __fastcall Train_LeaveBlock(uint8_t* t);
RT3_NOINLINE void __fastcall Vehicle_PassSignal(uint8_t* o);
void __fastcall Train_LeaveSite(uint8_t* t);
void __fastcall Train_EnterSite(uint8_t* t);
RT3_NOINLINE void __fastcall Train_PassSignal(uint8_t* t);
RT3_NOINLINE float __fastcall Train_ConsistWeight(uint8_t* t, int, int32_t quiet, int32_t slope, int32_t curves, int32_t withEngines, float* meanBend, float* maxBend, float* meanTilt);
RT3_NOINLINE int32_t __fastcall Train_NextNode(uint8_t* t, int, int32_t forward, uint32_t prev, uint32_t curId, int32_t* switched);
void __fastcall Train_SetRank(uint8_t* t, int, int32_t rank);
void __fastcall Train_ClearBreakdown(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
RT3_NOINLINE void __fastcall Train_RetargetRegion(uint8_t* t);
RT3_NOINLINE void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
void __fastcall Train_ShareRevenue(uint8_t* t, int, float revenue, int32_t kind, float units, int32_t cargo);
void __fastcall Train_EarnRevenue(uint8_t* o, int, float amount);
void __fastcall Train_AddExpense(uint8_t* t, int, float amount);
RT3_NOINLINE float __fastcall Train_RunningCost(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Train_LoadConsist(uint8_t* train, int, uint8_t* station, uint32_t* destination);
RT3_NOINLINE void __fastcall Train_AutoAddCars(uint8_t* t);
RT3_NOINLINE uint32_t __fastcall Train_PromptName(uint8_t* t);
void __fastcall Train_Teardown(uint8_t* t);
RT3_NOINLINE void __fastcall Train_UpdateSound(uint8_t* t);
RT3_NOINLINE void __fastcall Train_SetState3(uint8_t* t, int, uint8_t* car, uint8_t* target, uint32_t flag);
RT3_NOINLINE void __fastcall Train_SetState5(uint8_t* t, int, void*, uint8_t* target, uint32_t flag);
RT3_NOINLINE void __fastcall Train_StopAndGhost(uint8_t* train);
RT3_NOINLINE void __fastcall Train_Route(uint8_t* t);
RT3_NOINLINE void __fastcall Train_Crash(uint8_t* t, int, int32_t forced);
RT3_NOINLINE void __fastcall Train_BreakDown(uint8_t* t, int, int32_t forced);
RT3_NOINLINE void __fastcall Train_SetupNextStop(uint8_t* t);
RT3_NOINLINE void __fastcall Train_Arrive(uint8_t* t);
float __fastcall Train_ArriveStation(uint8_t* t, int, uint32_t station);
RT3_NOINLINE int32_t __fastcall Train_ProcessStop(uint8_t* t, int, int32_t force);
void __fastcall Train_YearlyUpkeep(uint8_t* t);
RT3_NOINLINE float __fastcall Train_StepAtStation(uint8_t* t, int, float dt);
RT3_NOINLINE void __fastcall Train_ReplaceEngine(uint8_t* train, int, uint32_t typeId, int32_t freeUpgrade);
RT3_NOINLINE void __fastcall Train_DropStation(uint8_t* t, int, int32_t station);
RT3_NOINLINE void __fastcall Train_DropWaypoint(uint8_t* t, int, int32_t node);
RT3_NOINLINE void __fastcall Train_RandomMishap(uint8_t* t);
RT3_NOINLINE void __fastcall Train_TickWear(uint8_t* t);
RT3_NOINLINE void __fastcall Train_Advance(uint8_t* t);
RT3_NOINLINE void __fastcall Train_PlaceInitialEngine(uint8_t* t, int, uint32_t typeId, uint8_t* node);
RT3_NOINLINE void __fastcall Train_Step(uint8_t* t, int, float dt, int32_t force);
RT3_NOINLINE int32_t __fastcall Trains_ResolveClash(Collection* objs, int, uint32_t a, uint32_t b);
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
RT3_NOINLINE void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void TrainWindow_RefreshOpen();
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);

static inline void* RefNotMinus1(Collection* t, int32_t id) { return id != -1 ? Collection_Get(t, 0, id) : nullptr; }

static int32_t __fastcall Obj_KindE8Special_Impl(void* o) {
    int32_t k = F<int32_t>(o, 0xe8);
    return (k == 0x11 || k == 0xc || k == 0xd) ? 1 : 0;
}

static inline void* FirstCar(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x41);
    return id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
}

// 0x4a8ec0. Smooths the displayed speeds +0x18e (and +0x18a in state 0)
// towards the target +0xcc (snapping when below 0.001).
static void Smooth(uint8_t* t, uint32_t off) {
    if ((double)F<float>(t, off) < *(const double*)0x5c8678) {
        F<uint32_t>(t, off) = F<uint32_t>(t, 0xcc);
        return;
    }
    F<float>(t, off) = (float)((double)F<float>(t, 0xcc) * *(const double*)0x5c86b8 +
                               (double)F<float>(t, off) * *(const double*)0x5c8888);
}

// 0x4aadd0 / 0x4aae40. Leaves / enters the station site +0xa4 (its train counter +0x58c).
static uint8_t* StationSite(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0xa4);
    if (id == -1 || !Collection_IsValid(g_stations, 0, id) || Obj_CityRef10(t)) return nullptr;
    return (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(t, 0xa4));
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline uint8_t* World() { return g_world; }

// Copies template entry src into array entry e (0x33-byte entries).
static void CopyEntry(uint8_t* e, uint8_t* src) {
    F<uint32_t>(e, 0x24) = F<uint32_t>(src, 0x24);
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x40);
    for (int k = 0; k < 8; ++k) F<uint32_t>(e, 1 + k * 4) = F<uint32_t>(src, 1 + k * 4);
    e[0x23] = src[0x23];
    e[0x22] = src[0x22];
    e[0x21] = src[0x21];
    e[0] = src[0];
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x20);
    e[0x28] ^= (uint8_t)((e[0x28] ^ src[0x28]) & 0x10);
}

static inline uint8_t* Snd() { return g_sound; }

static inline float FromBits(uint32_t u) { return *(float*)&u; }

static inline Collection* Nodes() { return g_tracks; }

static inline uint8_t* Node(uint32_t id) { return (uint8_t*)Collection_Get(Nodes(), 0, id); }

static inline int32_t Ftol(float v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// Whether signal s ahead blocks train t (none, its own, or one passed when the
// train may run through it counts as clear).
static inline int32_t Blocking(uint8_t* t, int32_t s) {
    if (s != -1 && s != F<int32_t>(t, 0x25) && !Train_IsRunning(t, 0, s)) s = -1;
    return s;
}

static inline Collection* Players() { return g_companies; }

static inline Collection* Stations() { return g_stations; }

static inline uint8_t* Engine(uint8_t* t) {
    return F<int32_t>(t, 0x41) != -1 ? (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)) : nullptr;
}

static inline uint8_t* EngineType(uint8_t* e) {
    return (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(e), 0x126));
}

// Share divisor: the train's +0x39, at least 1.
static inline float ShareCap(uint8_t* t) { return Fl(0x5c85b4) > F<float>(t, 0x39) ? 1.0f : F<float>(t, 0x39); }

static inline uint32_t EngineRegion(uint8_t* t) {
    uint8_t* e = Engine(t);
    if (!e) return 0;
    float y = (float)Obj_CellY(e);
    return Map_GridByte(hook::As<void*>(g_map), 0, (float)Obj_CellX(e), y);
}

static inline double Fl_195(uint32_t a) { return *(const float*)a; }

// Bend at a track node between headings h and the neighbour's: |delta|, beyond the
// threshold folded back (v1.03+) or reduced.
static float Bend(float h, uint8_t* next) {
    float h2 = Vehicle_Heading(next, 0, 1);
    float b = (float)crt::fabs(AngleDelta(nullptr, 0, h, h2));
    if (b > Fl_195(0x5c8764)) {
        if (Game_EffectiveVersion() >= 0x67) b = (float)(Fl_195(0x5c8760) - b);
        else b = (float)((double)b - Fl_195(0x5c8764));
    }
    return b;
}

namespace {
inline uint8_t* TrainLead(uint8_t* t) {
    return F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
}
// Stops train t for good: the lead car flagged (+carFlag) with its effect
// and sound, the train's ghosting (+0xf4) undone on every car, state `state`
// for 18720 ticks, then the news (string `msg`, kind 1) for its owner and
// the owner's tally. Not (unless forced) while the game is paused (0x6cec74
// +0xfc), with +0x4c8e, or in the editor with +0x243; nor twice (+0xe8).
void TrainStop(uint8_t* t, int32_t forced, uint32_t carFlag, void(__fastcall* effect)(uint8_t*, int, int32_t), int32_t sound, int32_t state, int32_t msg,
               void(__fastcall* tally)(void*), bool event) {
    if (!forced) {
        if (F<int32_t>(g_app, 0xfc)) return;
        if (g_world[0x4c8e]) return;
        if (g_newGame[0x82] && F<int32_t>(g_app, 0x243)) return;
    }
    if (F<int32_t>(t, 0xe8)) return;
    F<int32_t>(TrainLead(t), carFlag) = 1;
    effect(TrainLead(t), 0, 1);
    Vehicle_SelectSound(TrainLead(t), 0, sound);
    F<int32_t>(t, 0x55) = 0;
    F<int32_t>(t, 0x1a7) = 0;
    F<int32_t>(t, 0x59) = 0;
    F<int32_t>(t, 0x5d) = 0;
    IsMultiplayer();
    int32_t was = t[0xf4];
    if (was) {
        t[0xf4] = 0;
        for (uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c;) {
            SceneObj_SetAlpha(c, 0, 0xff, 1);
            Model_SetOpacity(c, 0, (float)(t[0xf4] ? 0.25 : 1.0));
            if (F<int32_t>(c, 0x352) == -1) break;
            c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(c, 0x352));
        }
        if (was != t[0xf4]) Station_RelinkNodes(t);
    }
    F<int32_t>(t, 0xe8) = state;
    IsMultiplayer();
    F<uint32_t>(t, 0xec) = 0x46924000;
    char text[0xc0];
    FormatText(text, GetString(msg), Train_DisplayName(t, 0, 0), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (F<int32_t>(t, 0x51) == Obj_Get21(hook::As<void*>(g_world))) Log_Add(g_messageLog, 0, text, 1, F<int32_t>(t, 0x25), 0);
    if (Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)))
        tally(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)));
    if (event && !forced && F<int32_t>(t, 0x51) == Obj_Get21(hook::As<void*>(g_world)))
        World_AddNamedEntry(hook::As<void*>(g_world), 0, text, 6, 1, F<int32_t>(t, 0x25), -1, -1, -1);
}
}

namespace {
// The map region byte under train t's lead car (0 without one).
uint32_t TrainRegion(uint8_t* t) {
    if (F<int32_t>(t, 0x41) == -1) return 0;
    uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    if (!c) return 0;
    float y = (float)Obj_CellY(c);
    float x = (float)Obj_CellX(c);
    return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
}
// Wears meter *m down by `by` (more, x 0x5c88e8, below 0x5c8628 when
// `harsh`), not below 0.
void TrainWear(float* m, float by, bool harsh) {
    if (harsh && *m < *(const double*)0x5c8628) by = (float)((double)by * *(const double*)0x5c88e8);
    float v = (float)((double)*m - by);
    *m = v;
    if (v < *(const float*)0x5c8598) *m = 0.0f;
}
}

// Notes that the train took the switch at `node` (+0x1cf) on side `side` (+0x1ce bit 2).
static void TookSwitch(uint8_t* t, uint32_t node, bool side, int32_t* switched) {
    F<uint32_t>(t, 0x1cf) = node;
    t[0x1ce] = (uint8_t)((t[0x1ce] & ~4) | (side ? 4 : 0));
    *switched = 1;
}

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol_205(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// Shared by the train ghosting functions: every car (vehicles 0x62ba84 from +0x41,
// next car +0x352) gets scene alpha `alpha` and opacity 0.25 (ghosted, +0xf4) or 1.
static void Train_ApplyGhost(uint8_t* train, uint32_t alpha) {
    Collection* cars = g_62ba84;
    for (uint8_t* car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(train, 0x41)); car;) {
        SceneObj_SetAlpha(car, 0, alpha, 1);
        Model_SetOpacity(car, 0, (float)(train[0xf4] ? 0.25 : 1.0));
        int32_t next = F<int32_t>(car, 0x352);
        if (next == -1) break;
        car = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)next);
    }
}

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol_207(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline uint8_t* TrackNode(int32_t id) { return (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// The destination node of train t's current order (+8 the order list): the
// order's own node when flagged (+0x28 bit 7, node +0x2b), else that of its
// station (+0x29 in 0x6cec20, node +8) or, for a missing station, the
// train's stored one (+0x94); -1 when there is none (*bad: no order or a
// null station).
static int32_t Train_OrderNode(uint8_t* t, bool* bad) {
    *bad = true;
    if (F<int32_t>(t, 8) == -1) return -1;
    uint8_t* o = (uint8_t*)Array33_Current(t);
    if (!o) return -1;
    *bad = false;
    if ((int8_t)o[0x28] < 0) return F<int32_t>(o, 0x2b);
    if (!Collection_IsValid(g_stations, 0, F<uint16_t>(o, 0x29))) return F<int32_t>(t, 0x94);
    uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(o, 0x29));
    if (!s) return *bad = true, -1;
    return F<int32_t>(s, 8);
}

namespace {
}

// 0x4a78d0 body. Walks a train's track segments (table 0x6cfca8, links +0x206 /
// +0x20a, node +0x202) from its current one (+0x356, direction +0x35a) for up to
// max steps and returns the steps taken when node target is reached, -1 otherwise.
// The node checked trails the walk by one segment, as in the original.
static int32_t __cdecl Train_StepsToNodeImpl(uint8_t* o, int32_t target, int32_t max) {
    uint8_t* r = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(o, 0x356));
    int32_t node = F<int32_t>(r, 0x202);
    int32_t flag = F<int32_t>(o, 0x35a);
    int32_t n = 0;
    for (int32_t i = 0; i < max; ++i) {
        if (node == target) return n;
        int32_t here = F<int32_t>(r, 0x202), a = F<int32_t>(r, 0x206), b = F<int32_t>(r, 0x20a);
        bool useA = i == 0 ? flag == 0 : b == node;
        int32_t next = useA ? a : b;
        if (next == -1) return -1;
        r = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)next);
        ++n;
        node = here;
    }
    return -1;
}

static uint8_t* Record(uint32_t slot, uint32_t id) {
    return ((uint8_t*)Collection_Get((Collection*)(hook::Global<void*>(slot)), 0, id));
}

static uint8_t* Ref(uint8_t* car) { return ((uint8_t*)Obj_Ref388((void*)(car))); }

static uint8_t* Next(uint8_t* car) { return ((uint8_t*)Obj_City352((void*)(car))); }

static uint8_t* Asset(uint32_t id) { return id ? Record(0x62ba88, id) : nullptr; }

static float CellX(uint8_t* car) { return (float)Obj_CellX((void*)(car)); }

static float CellY(uint8_t* car) { return (float)Obj_CellY((void*)(car)); }

static float Price(uint8_t* type, uint32_t company, uint32_t player) {
    return Obj_EffectScale(type, 0, company, player);
}

static void Stat(uint8_t* train, int32_t stat, float amount) {
    uint8_t* company = Record(0x62be10, F<uint32_t>(train, 0x51));
    Company_AddStat((void*)(company), 0, stat, amount);
}

static uint32_t AddCar(uint8_t* type, float x, float y) {
    return Cars_Add((Collection*)(hook::As<void*>(g_62ba84)), 0, type, 0, x, y, 0);
}

static void DeleteCar(uint8_t* car) {
    void* table = hook::As<void*>(g_62ba84);
    void** vtable = F<void**>(table, 0);
    ((void(__fastcall*)(void*, int, uint32_t))vtable[2])(table, 0, F<uint32_t>(car, 0x34a));
}

static void RefreshPose(uint8_t* car) {
    Car_Advance(car, 0, 0x38d1b717, nullptr, nullptr, -1);
}

static uint8_t* Get(uint32_t slot, uint32_t id) {
    return ((uint8_t*)Collection_Get((Collection*)(hook::Global<void*>(slot)), 0, id));
}

static void Append(uint8_t* t, uint32_t type) {
    Train_AppendCar(t, 0, type, 0);
}

static uint32_t FindType(int32_t category) {
    return Table_FindCurrentFor((Collection*)(hook::As<void*>(g_carTypes)), 0, category);
}

static void Load(uint8_t* car, uint8_t* station, uint32_t* destination) {
    Car_LoadAt(car, 0, station, (uint8_t*)(destination));
}

static uint32_t Coordinate(uint8_t* car, uint32_t target) {
    uint32_t bits;
    __asm {
        mov ecx, car
        mov eax, target
        call eax
        fstp dword ptr bits
    }
    return bits;
}

static inline float Fl_train_station(uint32_t a) { return *(float*)a; }

static int32_t Valid(uint32_t id) {
    return Collection_IsValid((Collection*)(hook::As<void*>(g_stations)), 0, id);
}

static uint32_t Location(uint8_t* row) {
    if ((int8_t)row[0x28] < 0) return F_train_station<uint32_t>(row, 0x2b);
    return F_train_station<uint32_t>(Get(0x6cec20, F_train_station<uint16_t>(row, 0x29)), 8);
}

static void Next_train_station(uint8_t* t) {
    int32_t next = (int32_t)(F_train_station<uint32_t>(t, 8) + 1u) % F_train_station<int32_t>(t, 4);
    TrainOrders_SetCurrent((uint8_t*)(t), 0, next, 0);
}

static uint8_t Kind(uint8_t* site) {
    uint8_t* building = ((uint8_t*)Obj_Building((void*)(site)));
    auto fn = (uint8_t*(__fastcall*)(uint8_t*))F_train_station<uint32_t>(F_train_station<void*>(building, 0), 0x80);
    return fn(building)[0x8c];
}

static void Format(char* out, const char* fmt, const char* a, const char* b = nullptr) {
    FormatText(out, fmt, a, b, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

static const char* Text(int32_t id) { return GetString(id); }

static char* Name(uint8_t* t) { return Train_DisplayName(t, 0, 0); }

// 0x4a62c0. Whether blocks a and b are connected (same component), renumbering
// first if needed.
int32_t __fastcall Routes_SameComponent(Collection* c, int, int32_t a, int32_t b) {
    if (a == -1 || b == -1) return 0;
    if (F<int32_t>(c, 0x8c)) Routes_NumberComponents(c);
    int32_t cb = F<int32_t>(Collection_Get(c, 0, b), 0xc);
    return F<int32_t>(Collection_Get(c, 0, a), 0xc) == cb ? 1 : 0;
}
RT3_REPLACE(0x4a62c0, Routes_SameComponent);

// 0x4a6320. The component of block id (-1 if none), renumbering first if needed.
RT3_NOINLINE int32_t __fastcall Routes_Component(Collection* c, int, int32_t id) {
    if (F<int32_t>(c, 0x8c)) Routes_NumberComponents(c);
    if (id == -1 || !Collection_IsValid(c, 0, id)) return -1;
    return F<int32_t>(Collection_Get(c, 0, id), 0xc);
}
RT3_REPLACE(0x4a6320, Routes_Component);

// 0x4a6360. Route table t: tries to merge valid route `id` (+0x1d) with
// routes at both ends whose end flags +0x44 agree: for each neighbour of each
// end node in another route that Routes_CanMerge allows, merges the current
// route into it (and follows it). Not while frozen (+0x90 / nodes +0xf1).
RT3_NOINLINE void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id) {
    if (F<int32_t>(t, 0x90) || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xf1)) return;
    if (!Collection_IsValid(t, 0, id)) return;
    uint8_t* S = (uint8_t*)Collection_Get(t, 0, id);
    if (!S[0x1d]) return;
    int32_t e1 = F<int32_t>(S, 8), e0 = F<int32_t>(S, 4);
    uint8_t* P0 = TrackNode(e0);
    if ((TrackNode(e1)[0x44] != 0) != (P0[0x44] != 0)) return;
    uint32_t cur = id;
    auto tryEnd = [&](int32_t e, bool follow) {
        uint8_t* P = TrackNode(e);
        for (uint32_t side = 0x206; side <= 0x20a; side += 4) {
            if (F<int32_t>(P, side) == -1) continue;
            if (F<uint32_t>(TrackNode(F<int32_t>(P, side)), 0x212) == cur) continue;
            if (!Routes_CanMerge(t, 0, TrackNode(F<int32_t>(P, side)), P)) continue;
            Routes_Merge(t, 0, cur, F<uint32_t>(TrackNode(F<int32_t>(P, side)), 0x212));
            if (follow || side == 0x206) cur = F<uint32_t>(P, 0x212);
        }
    };
    tryEnd(e0, true);
    if (e0 == e1) return;
    tryEnd(e1, false);
}
RT3_REPLACE(0x4a6360, Routes_TryMergeAt);

// 0x4a65b0. Route search by save version: games from version 0x67 on use the
// distance-ordered Route_Find, older ones keep the plain search so they replay
// the same.
int32_t __fastcall Route_FindForVersion(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                        int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack) {
    if (Game_EffectiveVersion() >= 0x67) return Route_Find(routes, 0, cur, target, cost, prev, needFlag, company, foreign, anyTrack);
    return Route_FindPlain(routes, 0, cur, target, cost, prev, needFlag, company, foreign, anyTrack);
}
RT3_REPLACE(0x4a65b0, Route_FindForVersion);

// 0x4a6630. The next track node to take from node `from` towards `to` (this =
// route collection, nodes 0x6cfca8; +0x202 id, +0x206 / +0x20a / +0x20e links,
// +0x212 route, +0x22e owner). Counts the nodes on the way in *cost and those
// not owned by *company in *foreign. Nodes on one route are walked directly;
// otherwise the cheapest of the four route-end pairings (Route_FindForVersion)
// is taken and the stretches from `from` to its route end and from the far
// route end to `to` are added. A start on a route end picks the link that
// leaves its route (single-node routes: the cheaper link, junctions: the third
// link when it is cheaper). -1 when there is no way (or `needFlag` and the
// start node lacks +0x44). Quirk: stepping along a route counts the node after
// the first step twice for *foreign.
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company,
                                               uint32_t* foreign, int32_t anyTrack) {
    uint32_t dummy = 0;
    if (!foreign) foreign = &dummy;
    uint32_t owner = company ? *company : 0;
    *cost = 0;
    *foreign = 0;
    if (from == -1 || to == -1) {
        *cost = 0xffffffff;
        return -1;
    }
    Collection* nodes = g_tracks;
    auto node = [&](int32_t id) { return (uint8_t*)Collection_Get(nodes, 0, (uint32_t)id); };
    uint8_t* p1 = node(from);
    uint8_t* p2 = node(to);
    if (!Collection_IsValid(g_routes, 0, F<uint32_t>(p1, 0x212)) ||
        !Collection_IsValid(g_routes, 0, F<uint32_t>(p2, 0x212))) {
        *cost = 0xffffffff;
        return -1;
    }
    if (needFlag && !p1[0x44]) {
        *cost = 0xffffffff;
        return -1;
    }
    if (from == to) return from;
    if (Node_IsRouteEnd(p1) && Node_IsRouteEnd(p2)) return Route_FindForVersion(routes, 0, from, to, cost, -1, needFlag, company, foreign, anyTrack);
    uint8_t* r1 = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(p1, 0x212));
    int32_t a = F<int32_t>(r1, 4), b = F<int32_t>(r1, 8);
    uint8_t* r2 = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(p2, 0x212));
    int32_t c = F<int32_t>(r2, 4), d = F<int32_t>(r2, 8);
    auto other = [&](int32_t cur, int32_t prev) { return F<int32_t>(node(cur), 0x206) == prev ? F<int32_t>(node(cur), 0x20a) : F<int32_t>(node(cur), 0x206); };
    if (r1 == r2) {
        for (int32_t dir = 0; dir < 2; ++dir) {
            if (dir) {
                *cost = 0;
                *foreign = 0;
            }
            int32_t first = F<int32_t>(p1, dir ? 0x20a : 0x206), cur = first, prev = from;
            while (cur != -1) {
                if (F<int32_t>(node(cur), 0x212) != F<int32_t>(p1, 0x212)) break;
                ++*cost;
                if (!owner || F<uint16_t>(node(cur), 0x22e) != owner) ++*foreign;
                if (cur == to) return first;
                int32_t nxt = other(cur, prev);
                prev = cur;
                cur = nxt;
            }
        }
    }
    uint32_t dAC, dAD, dBC, dBD, fAC, fAD, fBC, fBD;
    if (Route_FindForVersion(routes, 0, a, c, &dAC, -1, needFlag, company, &fAC, anyTrack) == -1) dAC = 0xffffffff;
    if (Route_FindForVersion(routes, 0, a, d, &dAD, -1, needFlag, company, &fAD, anyTrack) == -1) dAD = 0xffffffff;
    if (Route_FindForVersion(routes, 0, b, c, &dBC, -1, needFlag, company, &fBC, anyTrack) == -1) dBC = 0xffffffff;
    if (Route_FindForVersion(routes, 0, b, d, &dBD, -1, needFlag, company, &fBD, anyTrack) == -1) dBD = 0xffffffff;
    if (dAC == 0xffffffff && dAD == 0xffffffff && dBC == 0xffffffff && dBD == 0xffffffff) {
        *cost = 0xffffffff;
        return -1;
    }
    uint8_t *sp, *gp;
    if (dAC < dAD && dAC < dBC && dAC < dBD) {
        *cost = dAC;
        *foreign = fAC;
        sp = node(a);
        gp = node(c);
    } else if (dAD < dBC && dAD < dBD) {
        *cost = dAD;
        *foreign = fAD;
        sp = node(a);
        gp = node(d);
    } else if (dBC < dBD) {
        *cost = dBC;
        *foreign = fBC;
        sp = node(b);
        gp = node(c);
    } else {
        *cost = dBD;
        *foreign = fBD;
        sp = node(b);
        gp = node(d);
    }
    auto countForeign = [&](int32_t cur) {
        if (!owner || (cur != -1 && F<uint16_t>(node(cur), 0x22e) != owner)) ++*foreign;
    };
    auto walk = [&](uint8_t* end, int32_t stop) {
        int32_t prev = F<int32_t>(end, 0x202);
        int32_t cur = F<int32_t>(end, 0x206);
        if (cur == -1 || F<int32_t>(node(cur), 0x212) != F<int32_t>(end, 0x212)) cur = F<int32_t>(end, 0x20a);
        ++*cost;
        countForeign(cur);
        while (cur != stop) {
            int32_t nxt = other(cur, prev);
            ++*cost;
            countForeign(cur);
            prev = cur;
            cur = nxt;
        }
        return prev;
    };
    int32_t result;
    if (F<int32_t>(sp, 0x202) != from) {
        result = walk(sp, from);
    } else {
        int32_t cand;
        if (F<int32_t>(r1, 0x10) > 1) {
            cand = F<int32_t>(sp, 0x20a);
            int32_t n6 = F<int32_t>(sp, 0x206);
            if (n6 == -1 || F<int32_t>(node(n6), 0x212) != F<int32_t>(sp, 0x212)) cand = F<int32_t>(sp, 0x206);
        } else {
            uint32_t da, db;
            Route_FindForVersion(routes, 0, F<int32_t>(sp, 0x206), F<int32_t>(gp, 0x202), &da, -1, needFlag, company, nullptr, anyTrack);
            Route_FindForVersion(routes, 0, F<int32_t>(sp, 0x20a), F<int32_t>(gp, 0x202), &db, -1, needFlag, company, nullptr, anyTrack);
            cand = da < db ? F<int32_t>(sp, 0x206) : F<int32_t>(sp, 0x20a);
        }
        if (F<int32_t>(sp, 0x20e) != -1) {
            uint32_t d1, d2;
            if (Route_FindForVersion(routes, 0, F<int32_t>(gp, 0x202), cand, &d1, -1, needFlag, company, nullptr, anyTrack) == -1) d1 = 0xffffffff;
            if (Route_FindForVersion(routes, 0, F<int32_t>(gp, 0x202), F<int32_t>(sp, 0x20e), &d2, -1, needFlag, company, nullptr, anyTrack) == -1)
                d2 = 0xffffffff;
            if (d2 < d1) cand = F<int32_t>(sp, 0x20e);
        }
        result = cand;
    }
    if (F<int32_t>(gp, 0x202) != to) walk(gp, to);
    return result;
}
RT3_REPLACE(0x4a6630, Route_NextStep);

// 0x4a6e00. 1 when track node `to` can be reached from `from` (optionally only
// over routes with +0x1c, `needFlag`) and the first step is a real node.
RT3_NOINLINE int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag) {
    uint32_t cost;
    int32_t step = Route_NextStep(g_routes, 0, from, to, &cost, needFlag, nullptr, nullptr, 0);
    return cost != 0xffffffff && Collection_IsValid(g_tracks, 0, (uint32_t)step) ? 1 : 0;
}
RT3_REPLACE(0x4a6e00, Route_CanReach);

void __fastcall Nop4a6e50(void*) { __asm nop }
RT3_REPLACE(0x4a6e50, Nop4a6e50);

RT3_NOINLINE int32_t __fastcall Obj_ByteF9(uint8_t* o) { return o[0xf9]; }
RT3_REPLACE(0x4a6e60, Obj_ByteF9);

// 0x4a6e70. Books `amount` of revenue on a train: per paying company `key`
// (other than its own +0x51) in the 3-slot table +0x1ac/+0x1b2 (count
// +0x1b8), then into +0x39 and the running +0x65 / scaled +0x61.
void __fastcall Train_AddRevenue(uint8_t* t, int, int32_t amount, int32_t key) {
    if (key != F<int32_t>(t, 0x51)) {
        int found = 0;
        for (int32_t i = 0; i < t[0x1b8]; ++i) {
            if (key != F<uint16_t>(t, 0x1ac + i * 2)) continue;
            F<uint16_t>(t, 0x1b2 + i * 2) += (uint16_t)amount;
            found = 1;
        }
        if (!found && t[0x1b8] < 3) {
            F<uint16_t>(t, 0x1b2 + t[0x1b8] * 2) = (uint16_t)amount;
            F<uint16_t>(t, 0x1ac + t[0x1b8] * 2) = (uint16_t)key;
            t[0x1b8]++;
        }
    }
    float f = (float)amount;
    F<float>(t, 0x39) = (float)((double)f + F<float>(t, 0x39));
    float v = (float)((double)f + F<float>(t, 0x65));
    F<float>(t, 0x65) = v;
    F<float>(t, 0x61) = (float)((double)v * F<float>(t, 0xb4));
}
RT3_REPLACE(0x4a6e70, Train_AddRevenue);

// 0x4a6f20. Speed factor of a gradient/curve value: positive: (1 + v/(pi/2)*24.5)^2;
// otherwise 1 + 12v, at least 0.2.
RT3_NOINLINE double __stdcall SpeedFactor(float v) {
    if (v > *(const float*)0x5c8598) {
        float r = (float)((double)v / *(const float*)0x5c8764 * *(const double*)0x5d0050 + *(const double*)0x5c8770);
        r = (float)((double)r * r);
        return r;
    }
    float r = (float)((double)v * *(const float*)0x5c8788 + *(const double*)0x5c8770);
    if (r < *(const double*)0x5c8c20) return (float)0.2;
    return (float)(double)r;
}
RT3_REPLACE(0x4a6f20, SpeedFactor);

// 0x4a6fd0. out = a - b (float vec3).
RT3_NOINLINE void __fastcall Vec3_Sub(const float* a, int, float* out, const float* b) {
    float z = (float)((double)a[2] - b[2]);
    float y = (float)((double)a[1] - b[1]);
    float x = (float)((double)a[0] - b[0]);
    out[1] = y;
    out[2] = z;
    out[0] = x;
}
RT3_REPLACE(0x4a6fd0, Vec3_Sub);

RT3_NOINLINE int32_t __fastcall Train_StateIs7(uint8_t* t) { return F<int32_t>(t, 0xe8) == 7 ? 1 : 0; }
RT3_REPLACE(0x4a7010, Train_StateIs7);

void __fastcall Train_Clear1a3(uint8_t* t) { F<int32_t>(t, 0x1a3) = 0; }
RT3_REPLACE(0x4a7020, Train_Clear1a3);

// 0x4a7030. Saves an array of 0x33-byte records {data +0, count +4} between tags 0x5dc1/0x5dc2.
void __fastcall Array33_Save(uint32_t* o, int, Stream s) {
    StreamWriteU32(s, 0x5dc1);
    if (o[1]) StreamWrite(s, (const void*)o[0], o[1] * 0x33, 0);
    StreamWriteU32(s, 0x5dc2);
}
RT3_REPLACE(0x4a7030, Array33_Save);

// 0x4a7070. Blows one of the train's three whistles (0x5f0828 entries 0x3a..0x3c,
// random) n times, 125 ms apart, at the whistle volume option (10 - +0x31d tenths);
// in multiplayer only for the local player's trains (+0x51).
RT3_NOINLINE void __fastcall Train_Whistle(uint8_t* o, int, int32_t n) {
    if (n <= 0) return;
    if (IsMultiplayer() && Obj_Get21(hook::As<void*>(g_world)) != F<int32_t>(o, 0x51)) return;
    int32_t idx = Rand(0, 3) + 0x3a;
    const uint32_t* e = (const uint32_t*)(0x5f0828 + idx * 8);
    int32_t delay = 0;
    for (; n; --n, delay += 0x7d) {
        int32_t level = 10 - F<int32_t>(g_app, 0x31d);
        float vol = (float)((double)level / *(const double*)0x5c85f0 * FromBits(e[0]));
        if (vol <= *(const float*)0x5c8598) continue;
        uint32_t key = Sound_Play2D(Snd(), 0, (const char*)e[1], vol, 0x3f000000, 0, 1, nullptr);
        if (!key) continue;
        uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 0);
        if (!p) continue;
        F<uint32_t>(p, 0x44) |= 0x408;
        F<int32_t>(p, 0x50) = delay;
        F<int32_t>(p, 0xc) = 0;
    }
}
RT3_REPLACE(0x4a7070, Train_Whistle);

// 0x4a7170. Sets the speed target +0xcc (clearing +0x1d8) unless v is at most 0x5d0058.
RT3_NOINLINE void __fastcall Train_SetTarget(uint8_t* t, int, float v) {
    if (*(const float*)0x5d0058 >= v) {
        t[0x1d8] = 1;
        return;
    }
    t[0x1d8] = 0;
    F<float>(t, 0xcc) = v;
}
RT3_REPLACE(0x4a7170, Train_SetTarget);

RT3_NOINLINE void __fastcall Train_ResetStop(uint8_t* t) {
    F<uint32_t>(t, 0x8c) = 0xbf800000;  // -1.0f
    F<int32_t>(t, 0x1ba) = F<int32_t>(t, 0x94);
    F<int32_t>(t, 0x94) = -1;
}
RT3_REPLACE(0x4a71a0, Train_ResetStop);

RT3_NOINLINE int32_t __fastcall Train_StateSpecial(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 0x12 || s == 0x11 || s == 0xb || s == 0xc || s == 0xd ? 1 : 0;
}
RT3_REPLACE(0x4a71d0, Train_StateSpecial);

RT3_NOINLINE int32_t __fastcall Train_StateBOr12(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 0xb || s == 0x12 ? 1 : 0;
}
RT3_REPLACE(0x4a7200, Train_StateBOr12);

// 0x4aa62f keeps using ECX after the call.
__declspec(naked) int32_t Obj_KindE8Special() { RT3_KEEP_ECX_EDX(Obj_KindE8Special_Impl, 0) }
RT3_REPLACE(0x4a7220, Obj_KindE8Special);

double __fastcall Train_Float109(uint8_t* t, int, int32_t i) { return F<float>(t, 0x109 + i * 4); }
RT3_REPLACE(0x4a7240, Train_Float109);

double __fastcall Train_Float131(uint8_t* t, int, int32_t i) { return F<float>(t, 0x131 + i * 4); }
RT3_REPLACE(0x4a7250, Train_Float131);

RT3_NOINLINE double __fastcall Train_Float159(uint8_t* t, int, int32_t i) { return F<float>(t, 0x159 + i * 4); }
RT3_REPLACE(0x4a7260, Train_Float159);

// 0x4a7270. Sum of three float arrays at i: (+0x159 + +0x131) + +0x109.
RT3_NOINLINE double __fastcall Obj_Sum3(void* o, int, int32_t i) {
    return ((double)F<float>(o, 0x159 + i * 4) + F<float>(o, 0x131 + i * 4)) + F<float>(o, 0x109 + i * 4);
}
RT3_REPLACE(0x4a7270, Obj_Sum3);

// 0x4a72b0. Train year roll-over: three stats (first entries of +0x109,
// +0x131, +0x159) into the totals +0xfd.., histories shifted by one.
RT3_NOINLINE void __fastcall Train_RollStats(uint8_t* t) {
    F<float>(t, 0xfd) = (float)((double)F<float>(t, 0xfd) + F<float>(t, 0x109));
    F<float>(t, 0x101) = (float)((double)F<float>(t, 0x131) + F<float>(t, 0x101));
    F<float>(t, 0x105) = (float)((double)F<float>(t, 0x105) + F<float>(t, 0x159));
    for (int k = 9; k >= 1; --k)
        for (int a = 0; a < 3; ++a) F<uint32_t>(t, 0x109 + a * 0x28 + k * 4) = F<uint32_t>(t, 0x109 + a * 0x28 + (k - 1) * 4);
    for (int a = 0; a < 3; ++a) F<uint32_t>(t, 0x109 + a * 0x28) = 0;
}
RT3_REPLACE(0x4a72b0, Train_RollStats);

RT3_NOINLINE void __fastcall Nop4a7320(void*) { __asm nop }
RT3_REPLACE(0x4a7320, Nop4a7320);

int32_t __fastcall Train_State1Or2(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 1 || s == 2 ? 1 : 0;
}
RT3_REPLACE(0x4a7330, Train_State1Or2);

void __fastcall Nop4a7350(void*) { __asm nop }
RT3_REPLACE(0x4a7350, Nop4a7350);

// 0x4a7360. State 1 with speed +0xf0 at most 0.01 (stopped).
RT3_NOINLINE int32_t __fastcall Train_IsStopped(uint8_t* t) {
    if (F<int32_t>(t, 0xe8) != 1) return 0;
    return (double)F<float>(t, 0xf0) <= *(const double*)0x5c85b8 ? 1 : 0;
}
RT3_REPLACE(0x4a7360, Train_IsStopped);

// 0x4a7390. Replaces the 63-byte buffer +0x1c6 with a copy of src.
RT3_NOINLINE void __fastcall Train_SetBuffer1c6(uint8_t* t, int, const uint8_t* src) {
    if (F<void*>(t, 0x1c6)) {
        crt::free(F<void*>(t, 0x1c6));
        F<void*>(t, 0x1c6) = nullptr;
    }
    uint8_t* p = (uint8_t*)crt::malloc(0x3f);
    F<uint8_t*>(t, 0x1c6) = p;
    for (int k = 0; k < 0x3f; ++k) p[k] = src[k];
}
RT3_REPLACE(0x4a7390, Train_SetBuffer1c6);

RT3_NOINLINE void __fastcall Obj_Dec58c(uint8_t* o) {
    if (F<int32_t>(o, 0x58c) > 0) F<int32_t>(o, 0x58c)--;
}
RT3_REPLACE(0x4a73e0, Obj_Dec58c);

uint8_t* __fastcall Cursor33_Construct(uint8_t* c) {
    Cursor33_Init(c);
    return c;
}
RT3_REPLACE(0x4a7400, Cursor33_Construct);

// 0x4a7410. Sets up train record t: id (+0xd / +0x25) and owner (+0x51), no cars
// or route, created / serviced today (+0xd0 / +0xd4 = world date), ghosted
// (+0xf4) and stopped in state 0xf, full condition multipliers, every running
// value cleared, and named "Train <id>" (string 0x111, the id read unsigned).
void __fastcall Train_Init(uint8_t* t, int, int32_t id, int32_t owner) {
    t[0x1d4] = 0;
    uint8_t flags = t[0x1ce] & 7;
    F<int32_t>(t, 0xd) = id;
    F<int32_t>(t, 0x25) = id;
    t[0x1ce] = flags;
    F<int32_t>(t, 0x51) = owner;
    F<int32_t>(t, 0x1c6) = 0;
    F<int32_t>(t, 0x45) = 0;
    F<int32_t>(t, 0xcc) = 0;
    const uint32_t minusOne1[4] = { 0x1ca, 0x41, 0x49, 0x4d };
    for (uint32_t o : minusOne1) F<int32_t>(t, o) = -1;
    F<int32_t>(t, 0xd0) = F<uint16_t>(g_world, 0xd);
    F<int32_t>(t, 0xd4) = F<uint16_t>(g_world, 0xd);
    t[0x1ce] = flags & 0xfd;
    F<uint32_t>(t, 0xdc) = F<uint32_t>(t, 0xe0) = F<uint32_t>(t, 0xe4) = 0x3f800000;
    F<int32_t>(t, 4) = 0;
    F<int32_t>(t, 8) = -1;
    F<int32_t>(t, 0) = 0;
    t[0xf4] = 1;
    F<int32_t>(t, 0xf5) = 0;
    t[0xf9] = t[0xfa] = t[0xfb] = 0;
    F<int32_t>(t, 0x29) = 0;
    t[0x1ab] = 0;
    F<int32_t>(t, 0xd8) = 2;
    F<int32_t>(t, 0xe8) = 0xf;
    const uint32_t zero1[] = { 0xec, 0x55, 0x59, 0x5d, 0x1a7 };
    for (uint32_t o : zero1) F<int32_t>(t, o) = 0;
    t[0x196] = 0;
    F<int32_t>(t, 0x2d) = F<int32_t>(t, 0x31) = F<int32_t>(t, 0x35) = 0;
    t[0x189] = 0;
    F<int32_t>(t, 0x1a3) = 0;
    F<int32_t>(t, 0xf0) = 0;
    F<int32_t>(t, 0x98) = -1;
    t[0xfc] = 0;
    F<int32_t>(t, 0xfd) = F<int32_t>(t, 0x101) = F<int32_t>(t, 0x105) = 0;
    F<int32_t>(t, 0x181) = -1;
    F<int32_t>(t, 0x185) = -1;
    F<int32_t>(t, 0x18a) = 0;
    F<int32_t>(t, 0x18e) = 0;
    t[0x1a2] = 0;
    F<uint16_t>(t, 0x88) = 0;
    t[0x8a] = 0;
    for (int k = 0; k < 10; ++k) {
        F<int32_t>(t, 0x109 + k * 4) = 0;
        F<int32_t>(t, 0x131 + k * 4) = 0;
        F<int32_t>(t, 0x159 + k * 4) = 0;
    }
    F<int32_t>(t, 0x61) = 0;
    F<int32_t>(t, 0x65) = 0;
    t[0x1bf] = 0;
    F<uint16_t>(t, 0x1c0) = 0;
    t[0x1b9] = 0;
    t[0x1c2] = 0;
    F<uint16_t>(t, 0x1c3) = 0;
    F<int32_t>(t, 0x39) = 0;
    t[0x1c5] = 0;
    t[0x1b8] = 0;
    const uint32_t zero2[] = { 0x3d, 0xb4, 0xb8, 0xc8 };
    for (uint32_t o : zero2) F<int32_t>(t, o) = 0;
    const uint32_t minusOne2[] = { 0xac, 0xa4, 0xa8, 0x1ba, 0x94 };
    for (uint32_t o : minusOne2) F<int32_t>(t, o) = -1;
    F<uint32_t>(t, 0x8c) = 0xbf800000;
    F<int32_t>(t, 0x90) = 0;
    double n = (double)F<int32_t>(t, 0x25);
    if (F<int32_t>(t, 0x25) < 0) n += D(0x5c8910);
    char num[0x40];
    Num_Format(num, 1, n, 10, 1, 0, 0);
    FormatText((char*)t + 0x69, GetString(0x111), num, 0, 0, 0, 0, 0, 0, 0);
    F<int32_t>(t, 0x1cf) = -1;
    F<int32_t>(t, 0x9c) = 0;
    F<int32_t>(t, 0xa0) = 0;
    F<int32_t>(t, 0xb0) = 0;
    F<int32_t>(t, 0x199) = 0;
    F<int32_t>(t, 0x19d) = 0;
    t[0x1a1] = 0;
    t[0x1d3] = 0;
    t[0x1ce] &= 0xfe;
}
RT3_REPLACE(0x4a7410, Train_Init);

// 0x4a76a0 / 0x4a7700 / 0x4a7760. Over the consist chain from city record
// +0x41 (next via 0x41ad90): n-th car whose type (+0x388)->+0x41 is 1 /
// is not 2 or 3, and the count of the latter.
RT3_NOINLINE void* __fastcall Train_NthCarKind1(uint8_t* t, int, int32_t n) {
    if (n > t[0xfa]) return nullptr;
    for (void* c = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 1) --n;
        if (n <= 0) return c;
    }
    return nullptr;
}
RT3_REPLACE(0x4a76a0, Train_NthCarKind1);

RT3_NOINLINE void* __fastcall Train_NthCarOther(uint8_t* t, int, int32_t n) {
    for (void* c = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) != 2 && F<int32_t>(Obj_Ref388(c), 0x41) != 3) --n;
        if (n <= 0) return c;
    }
    return nullptr;
}
RT3_REPLACE(0x4a7700, Train_NthCarOther);

int32_t __fastcall Train_CountCarsOther(uint8_t* t) {
    int32_t n = 0;
    for (void* c = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c))
        if (F<int32_t>(Obj_Ref388(c), 0x41) != 2 && F<int32_t>(Obj_Ref388(c), 0x41) != 3) ++n;
    return n;
}
RT3_REPLACE(0x4a7760, Train_CountCarsOther);

RT3_NOINLINE void* __fastcall Obj_City41(void* o) { return RefNotMinus1(g_62ba84, F<int32_t>(o, 0x41)); }
RT3_REPLACE(0x4a77b0, Obj_City41);

// 0x4a77d0. Rear point of the train's first car; (10, 10) without cars.
RT3_NOINLINE void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y) {
    int32_t id = F<int32_t>(t, 0x41);
    if (id != -1) {
        Car_RearPoint((uint8_t*)Collection_Get(g_62ba84, 0, id), 0, x, y);
        return;
    }
    *y = 10.0f;
    *x = 10.0f;
}
RT3_REPLACE(0x4a77d0, Train_RearPoint);

// 0x4a7810. Restarts the animated sub-objects of the train's first car (8 per part):
// the special-named ones at full speed, the others paused (+0x1d8).
RT3_NOINLINE void __fastcall Train_ResetCarAnimations(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x41);
    uint8_t* car = id != -1 ? (uint8_t*)Collection_Get(g_62ba84, 0, id) : nullptr;
    for (int32_t i = 0; i < F<int32_t>(car, 0x31a); ++i)
        for (int k = 0; k < 8; ++k) {
            uint8_t* sub = F<uint8_t*>(F<uint8_t*>(car, 0x316), 0x10 + i * 0xc0 + k * 4);
            if (!sub) continue;
            uint8_t* a = F<uint8_t*>(sub, 0x60);
            if (a && Obj_NameIs5ce4f4(a)) {
                sub[0x1d8] = 0;
                F<uint32_t>(sub, 0xcc) = 0x3f800000;
                Timer_SetFrameTime(sub, 0, 1.0f);
            } else {
                sub[0x1d8] = 1;
            }
        }
}
RT3_REPLACE(0x4a7810, Train_ResetCarAnimations);

// 0x4a78d0. Train in EAX, target and max on the stack.
__declspec(naked) void Train_StepsToNode() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Train_StepsToNodeImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x4a78d0, Train_StepsToNode);

// 0x4a7970. Whether obj is at least sqrt(0x5d005c) away from the site's flagged building.
RT3_NOINLINE int32_t __stdcall Obj_FarFromSiteBuilding(uint8_t* obj, void* site) {
    uint8_t* b = (uint8_t*)Obj_Building(site);
    if (Obj_Flag3d4(b) != 1) return 0;
    float p1[3], p2[3];
    SceneObj_GetPos(b, 0, (uint32_t*)&p1[0], (uint32_t*)&p1[1], (uint32_t*)&p1[2]);
    SceneObj_GetPos(obj, 0, (uint32_t*)&p2[0], (uint32_t*)&p2[1], (uint32_t*)&p2[2]);
    float dz = (float)((double)p2[2] - p1[2]);
    float dy = (float)((double)p2[1] - p1[1]);
    float dx = (float)((double)p2[0] - p1[0]);
    float d = (float)((double)dx * dx + (double)dy * dy + (double)dz * dz);
    return d >= *(const float*)0x5d005c ? 1 : 0;
}
RT3_REPLACE(0x4a7970, Obj_FarFromSiteBuilding);

// 0x4a7a30. Looks up to 15 track nodes ahead of the train's car (0x62ba84 by +0x41;
// node +0x356, direction +0x35a) for a station (+0x222) of building kind `kind`:
// sets *out to the first one (or the next one after *out; with `check`, only those
// far enough per Obj_FarFromSiteBuilding); -1 at a junction (+0x20e) or after 15.
void __fastcall Train_FindStationAhead(uint8_t* o, int, int32_t* out, int32_t kind, int32_t check) {
    uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(o, 0x41));
    Collection* nodes = g_tracks;
    uint8_t* s = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(car, 0x356));
    int32_t prevId = F<int32_t>(s, 0x202);
    int32_t dir = F<int32_t>(car, 0x35a);
    int32_t steps = 0;
    for (;;) {
        int32_t sid = F<int32_t>(s, 0x222);
        if (sid != -1) {
            void* bld = Collection_Get(g_stations, 0, sid);
            if (bld) {
                uint8_t* b = (uint8_t*)Obj_Building(bld);
                uint8_t* type = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
                if (kind == type[0x8c]) {
                    if (*out == -1) {
                        if (!check || Obj_FarFromSiteBuilding(car, bld) == 1) {
                            *out = F<int32_t>(s, 0x222);
                            break;
                        }
                    } else {
                        if (Collection_Get(g_stations, 0, *out) == bld) break;
                        if (!check || Obj_FarFromSiteBuilding(car, bld) == 1) {
                            *out = F<int32_t>(s, 0x222);
                            break;
                        }
                    }
                }
            }
        }
        int32_t self = F<int32_t>(s, 0x202);
        int32_t a = F<int32_t>(s, 0x206), b = F<int32_t>(s, 0x20a);
        if (F<int32_t>(s, 0x20e) != -1) {
            *out = -1;
            return;
        }
        int32_t next;
        if (!steps) {
            next = dir ? b : a;
            if (next == -1) return;
        } else {
            next = b == prevId ? a : b;
            if (next == -1) break;
        }
        s = (uint8_t*)Collection_Get(g_tracks, 0, next);
        ++steps;
        prevId = self;
        if (steps >= 0xf) break;
    }
    if (steps == 0xf) *out = -1;
}
RT3_REPLACE(0x4a7a30, Train_FindStationAhead);

// 0x4a7bc0. For a signal node (+0x94, kind 4): 1 when `other`'s position (+0x1e2,
// +0x1ea) is behind it for the car's direction (+0x35a), else 0; 2 for other kinds.
RT3_NOINLINE int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other) {
    uint8_t* e = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(o, 0x94));
    if (e[0x218] != 4) return 2;
    return !Effect_PointOnSide(e, 0, F<int32_t>(car, 0x35a), F<float>(other, 0x1e2), F<float>(other, 0x1ea)) ? 1 : 0;
}
RT3_REPLACE(0x4a7bc0, Signal_IsBehind);

// 0x4a7c10. Payment for delivering value v of cargo o (kind +0x10) loaded in year
// `year`: v over the speed/price effects (0x5a + kind, 0x59; at least 0.02), aged
// 0.5% a year, less the base (+0x24, not below 0); then 0.98 to the power of that
// over +0x28, x +0x20 and the effects 0x51 and 0x52 + kind; at least 1, doubled
// when world +0x4c8f.
RT3_NOINLINE float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region) {
    uint32_t player = company ? F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b) : 0;
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x5a, company, player, region);
    float m = (float)((double)Effects_Multiplier(World(), 0, 0x59, company, player, region) * m1);
    double d = (double)m > D(0x5c8918) ? (double)m : D(0x5c8918);
    float x = (float)((double)v / d);
    int32_t age = (int32_t)F<uint16_t>(World(), 0xd) - year;
    x = (float)(((double)age * D(0x5c9bc0) + D(0x5c8770)) * x);
    float r = (float)((double)x - F<float>(o, 0x24));
    if ((double)r < D(0x5c8608)) r = 0.0f;
    float scale = F<float>(o, 0x20);
    double p = crt::pow(0.98, (double)r / F<float>(o, 0x28));
    float q = (float)(p * scale);
    q = (float)((double)Effects_Multiplier(World(), 0, 0x51, company, player, region) * q);
    q = (float)((double)Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x52, company, player, region) * q);
    if (q < Fl(0x5c85b4)) q = 1.0f;
    if (World()[0x4c8f]) q = (float)((double)q + q);
    return q;
}
RT3_REPLACE(0x4a7c10, Cargo_DeliveryPayment);

// 0x4a7db0. Sum of the cars' type value +0x10d over the consist chain (next +0x352).
RT3_NOINLINE float __fastcall Train_SumCarValues(uint8_t* t) {
    float s = 0.0f;
    for (uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c; ) {
        s = (float)(Obj_Ref388Float10d(c) + s);
        int32_t next = F<int32_t>(c, 0x352);
        if (next == -1) break;
        c = (uint8_t*)Collection_Get(g_62ba84, 0, next);
    }
    return s;
}
RT3_REPLACE(0x4a7db0, Train_SumCarValues);

// 0x4a7e10. Train id exists and is in none of the idle/stopped states.
RT3_NOINLINE int32_t __fastcall Train_IsRunning(void*, int, uint32_t id) {
    if (!Collection_IsValid(g_trains, 0, id)) return 0;
    switch (F<int32_t>(Collection_Get(g_trains, 0, id), 0xe8)) {
    case 0x13: case 0xa: case 9: case 8: case 7: case 0x10: case 1: case 2: case 3: case 5: case 4: case 6:
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4a7e10, Train_IsRunning);

// 0x4a7e90. Signal look-ahead for train t (node +0x49, direction from its engine
// +0x35a): checks the signals at its node, the connected node (+0x253) both ways,
// then nodes ahead up to its stopping distance (car values) until +0x4d. A red
// signal for someone else stops it unless option +0x1b3 lets it through.
RT3_NOINLINE int32_t __fastcall Train_SignalsClear(uint8_t* t) {
    int32_t cur = F<int32_t>(t, 0x49);
    if (cur == -1) return 1;
    int32_t fwd = F<int32_t>(Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)), 0x35a) == 0;
    int32_t s = Node_GetLink(Node(F<uint32_t>(t, 0x49)), 0, !fwd);
    float lim = Train_SumCarValues(t);
    float dist = 0.0f;
    if (s != -1 && s != F<int32_t>(t, 0x25)) {
        if (!Train_IsRunning(t, 0, s)) s = -1;
        else goto check;
    }
    {
        uint8_t* p = Node(F<uint32_t>(t, 0x49));
        if (Collection_IsValid(Nodes(), 0, F<uint32_t>(p, 0x253))) {
            s = Node_GetLink(Node(F<uint32_t>(p, 0x253)), 0, 1);
            if (s != -1 && s != F<int32_t>(t, 0x25) && Train_IsRunning(t, 0, s) && s != F<int32_t>(t, 0x25)) goto check;
            s = Node_GetLink(Node(F<uint32_t>(p, 0x253)), 0, 0);
            if (s == -1) goto end;
            if (s != F<int32_t>(t, 0x25)) s = Blocking(t, s);
        }
    }
    for (;;) {
check:
        if (s != -1 && s != F<int32_t>(t, 0x25)) return F<int32_t>(g_app, 0x1b3) != 0;
end:
        if (cur == F<int32_t>(t, 0x4d)) return 1;
        int32_t prev = cur;
        cur = Track_Link2(Node(cur), 0, fwd);
        if (cur == -1 || dist > lim) return 1;
        s = Node_GetLink(Node(cur), 0, !fwd);
        if (s != -1 && !Train_IsRunning(t, 0, s)) s = -1;
        uint8_t* q = Node(cur);
        dist = (float)(dist + D(0x5c97e0));
        fwd = F<int32_t>(q, 0x20a) != prev;
    }
}
RT3_REPLACE(0x4a7e90, Train_SignalsClear);

// 0x4a80a0. Drops the pending node +0x1cf once its link (side from +0x1ce bit 2)
// is gone or points at +0x25. 0 while still pending.
RT3_NOINLINE int32_t __fastcall Train_CheckPendingNode(uint8_t* t) {
    if (!Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x1cf))) return 1;
    void* n = Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x1cf));
    int32_t link = Node_GetLink(n, 0, (t[0x1ce] >> 2) & 1);
    if (link != -1 && link != F<int32_t>(t, 0x25)) return 0;
    F<int32_t>(t, 0x1cf) = -1;
    return 1;
}
RT3_REPLACE(0x4a80a0, Train_CheckPendingNode);

// 0x4a8100. Re-links this station into its two track nodes (+0x49/+0x4d)
// with the cities' +0x35a values.
RT3_NOINLINE void __fastcall Station_RelinkNodes(void* s) {
    if (F<int32_t>(s, 0x49) == -1) return;
    int32_t a = F<int32_t>(Collection_Get(g_62ba84, 0, F<uint32_t>(s, 0x41)), 0x35a);
    int32_t b = F<int32_t>(Collection_Get(g_62ba84, 0, F<uint32_t>(s, 0x45)), 0x35a);
    int32_t n1 = F<int32_t>(s, 0x49);
    if (n1 != -1 && Collection_IsValid(g_tracks, 0, n1))
        Node_SetLinkPair(Collection_Get(g_tracks, 0, F<uint32_t>(s, 0x49)), 0, F<int32_t>(s, 0x25), -2, a);
    int32_t n2 = F<int32_t>(s, 0x4d);
    if (n2 != -1 && Collection_IsValid(g_tracks, 0, n2))
        Node_SetLinkPair(Collection_Get(g_tracks, 0, F<uint32_t>(s, 0x4d)), 0, F<int32_t>(s, 0x25), -3, b);
}
RT3_REPLACE(0x4a8100, Station_RelinkNodes);

// 0x4a81b0. Unhooks this station from its two track nodes (+0x49/+0x4d).
RT3_NOINLINE void __fastcall Station_UnlinkNodes(void* s) {
    int32_t a = F<int32_t>(s, 0x49);
    if (a == -1) return;
    if (Collection_IsValid(g_tracks, 0, a))
        Node_ClearLinks(Collection_Get(g_tracks, 0, F<uint32_t>(s, 0x49)), 0, F<int32_t>(s, 0x25), -2);
    int32_t b = F<int32_t>(s, 0x4d);
    if (b == -1) return;
    if (Collection_IsValid(g_tracks, 0, b))
        Node_ClearLinks(Collection_Get(g_tracks, 0, F<uint32_t>(s, 0x4d)), 0, F<int32_t>(s, 0x25), -3);
}
RT3_REPLACE(0x4a81b0, Station_UnlinkNodes);

// 0x4a8220. Ghosts (translucent cars, unlinked from the track nodes) or restores a
// train; nothing when unchanged unless `force`.
RT3_NOINLINE void __fastcall Train_SetGhost(uint8_t* train, int, int32_t ghost, int32_t force) {
    uint8_t old = train[0xf4];
    if ((uint32_t)ghost == old && !force) return;
    train[0xf4] = (uint8_t)ghost;
    Train_ApplyGhost(train, ghost ? 0x3f : 0xff);
    if (old == train[0xf4]) return;
    if (ghost) Station_UnlinkNodes(train);
    else Station_RelinkNodes(train);
}
RT3_REPLACE(0x4a8220, Train_SetGhost);

// 0x4a8310. Calls virtual slot 22 (a, b, c) on this object's city and each
// city linked from it via +0x352.
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c) {
    uint8_t* city = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(o, 0x41));
    while (city) {
        ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))(*(void***)city)[0x58 / 4])(city, a, b, c);
        int32_t next = F<int32_t>(city, 0x352);
        if (next == -1) break;
        city = (uint8_t*)Collection_Get(g_62ba84, 0, next);
    }
}
RT3_REPLACE(0x4a8310, Obj_ForCityChain);

// 0x4a8370. Refreshes the train window when it shows this train (view mode 2).
RT3_NOINLINE void __fastcall Train_RefreshWindowIfShown(uint8_t* t) {
    uint8_t* view = g_gameWindow;
    if (view && F<int32_t>(view, 0x8c) == 2 && F<int32_t>(view, 0x90) == F<int32_t>(t, 0x25)) TrainWindow_RefreshOpen();
}
RT3_REPLACE(0x4a8370, Train_RefreshWindowIfShown);

// 0x4a83a0. Destination of the train's current order (+8 array): a track
// node for flagged orders, else the site's +8; `preferStop` picks +0x94
// when valid. *dist gets the order's +0x2f. -1 if none.
RT3_NOINLINE int32_t __fastcall Train_OrderTarget(uint8_t* t, int, int32_t* dist, int32_t preferStop) {
    if (F<int32_t>(t, 8) == -1) return -1;
    uint8_t* c = (uint8_t*)Array33_Current(t);
    if (!c) return -1;
    if ((int8_t)c[0x28] < 0) {
        if (preferStop && Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x94))) return F<int32_t>(t, 0x94);
        if (dist) *dist = F<int32_t>(c, 0x2f);
        return F<int32_t>(c, 0x2b);
    }
    Collection* sites = g_stations;
    if (!Collection_IsValid(sites, 0, F<uint16_t>(c, 0x29))) return F<int32_t>(t, 0x94);
    uint8_t* s = (uint8_t*)Collection_Get(sites, 0, F<uint16_t>(c, 0x29));
    if (!s) return -1;
    if (dist) *dist = F<int32_t>(c, 0x2f);
    if (preferStop && F<int32_t>(t, 0x94) != -1) return F<int32_t>(t, 0x94);
    return F<int32_t>(s, 8);
}
RT3_REPLACE(0x4a83a0, Train_OrderTarget);

// 0x4a8470. Target node of the current order: flagged orders' +0x2b, else the site's +8; -1 if none.
RT3_NOINLINE int32_t __fastcall Train_CurrentOrderNode(uint8_t* t) {
    if (F<int32_t>(t, 8) == -1) return -1;
    uint8_t* c = OrderList_Current(t);
    if (!c) return -1;
    if ((int8_t)c[0x28] < 0) return F<int32_t>(c, 0x2b);
    uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(c, 0x29));
    return s ? F<int32_t>(s, 8) : -1;
}
RT3_REPLACE(0x4a8470, Train_CurrentOrderNode);

// 0x4a84b0. Loads a train's extra block: tag 0x5dc1, its 0x33-byte entries (+4 of
// them, into a new array at +0), tag 0x5dc2; resets +0x1c6 / +0xc8 and re-applies a
// saved ghost state. Returns the bytes read.
int32_t __fastcall Train_LoadExtra(uint8_t* train, int, int32_t st) {
    int32_t n = StreamSkip4(st, 0x5dc1, 0, 0);
    if (F<int32_t>(train, 4)) {
        void* a = crt::malloc((size_t)F<int32_t>(train, 4) * 0x33);
        F<void*>(train, 0) = a;
        n += StreamRead(st, a, (uint32_t)(F<int32_t>(train, 4) * 0x33), 0, 1);
    }
    F<int32_t>(train, 0x1c6) = 0;
    F<int32_t>(train, 0xc8) = 0;
    if (train[0xf4]) Train_SetGhost(train, 0, 1, 1);
    return n + StreamSkip4(st, 0x5dc2, 0, 0);
}
RT3_REPLACE(0x4a84b0, Train_LoadExtra);

// 0x4a8540. A train's destination text into `dst` (at most n-1 chars): "[No
// destination - waiting for a route]" (string 0x112) without a current stop, else
// "Waypoint (x, y)" (string 0x113) for a waypoint or the city's name.
RT3_NOINLINE void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n) {
    uint8_t* stop = (uint8_t*)Array33_Current(train);
    if (!stop) {
        StrCopyN(dst, GetString(0x112), n - 1);
        return;
    }
    if ((int8_t)stop[0x28] < 0) {
        uint8_t* node = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(stop, 0x2b));
        char ys[0x12c], xs[0x130], text[0x12c];
        Num_Format(ys, 1, (double)Ftol_205(Obj_CenterCellY(node)), 10, 1, 0, 0);
        Num_Format(xs, 1, (double)Ftol_205(Obj_CenterCellX(node)), 10, 1, 0, 0);
        FormatText(text, GetString(0x113), xs, ys, 0, 0, 0, 0, 0, 0);
        StrCopyN(dst, text, n - 1);
        return;
    }
    StrCopyN(dst, (const char*)Collection_Get(g_stations, 0, F<uint16_t>(stop, 0x29)) + 0x46b, n - 1);
}
RT3_REPLACE(0x4a8540, Train_DestinationText);

// 0x4a8690. Rank of this train among its company's trains in list-1 order,
// not counting special-kind trains (0x11/0xc/0xd) with flag bit 2 of +0x1ce.
RT3_NOINLINE int32_t __fastcall Train_Rank(void* self) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 1));
        if (t == self) return n;
        if (F<int32_t>(t, 0x51) != F<int32_t>(self, 0x51)) continue;
        int32_t k = F<int32_t>(t, 0xe8);
        if ((k == 0x11 || k == 0xc || k == 0xd) && (F<uint8_t>(t, 0x1ce) & 2)) continue;
        ++n;
    }
    return 0;
}
RT3_REPLACE(0x4a8690, Train_Rank);

// 0x4a8720. Shows the dollar sign for a delivery worth `amount` over station
// s's building (+0xac in 0x6cec20), at its city's position, replacing the
// last one (+0xc8); not in the editor (0x6cec7c +0x82) or with +0x4aef, and
// in multiplayer only for the local player's (+0x51) stations.
RT3_NOINLINE void __fastcall Station_ShowCargoMoney(uint8_t* s, int, int32_t amount) {
    if (g_newGame[0x82]) return;
    if (F<int32_t>(g_world, 0x4aef)) return;
    if (amount <= 0) return;
    if (IsMultiplayer() && Obj_Get21(hook::As<void*>(g_world)) != F<int32_t>(s, 0x51)) return;
    if (F<uint32_t*>(s, 0xc8)) Effect_Free(F<uint32_t*>(s, 0xc8));
    if (F<int32_t>(s, 0xac) == -1) return;
    if (!Collection_IsValid(g_stations, 0, F<uint32_t>(s, 0xac))) return;
    uint8_t* parent = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, F<uint32_t>(s, 0xac)));
    uint8_t* e = Effect_Create(parent, 7, 1.0f, 0x100000, 0);
    F<uint8_t*>(s, 0xc8) = e;
    if (!e) return;
    Obj_SetAllBc(e, amount);
    uint32_t pos[3];
    SceneObj_GetPos((uint8_t*)Obj_City41(s), 0, &pos[0], &pos[1], &pos[2]);
    Store3Globals((int32_t*)pos);
    Obj_ApplyStored3(F<uint8_t*>(s, 0xc8), 0);
}
RT3_REPLACE(0x4a8720, Station_ShowCargoMoney);

// 0x4a8840. The 0x6ada84 record of this object's city (via the city's +0x388 ref).
RT3_NOINLINE void* __fastcall Obj_CityRef(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (!city) return nullptr;
    return Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126));
}
RT3_REPLACE(0x4a8840, Obj_CityRef);

// 0x4a8880 / 0x4a88b0. Of the train's first car: its type's +0x126 / its region.
RT3_NOINLINE int32_t __fastcall Train_FirstCarType126(uint8_t* t) {
    void* c = FirstCar(t);
    return c ? F<int32_t>(Obj_Ref388(c), 0x126) : 0;
}
RT3_REPLACE(0x4a8880, Train_FirstCarType126);

uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t) {
    void* c = FirstCar(t);
    return c ? Obj_Region(c) : nullptr;
}
RT3_REPLACE(0x4a88b0, Train_FirstCarRegion);

// 0x4a88e0. Train t: the summed rate (Obj_Rate) of its cars whose cargo has
// class +0x41 == 1 and a non-zero +0x398 - with onlyLocal, only those of
// cargo kind 0x62ba8c+0x88 (+0x38c); 0 without a next stop.
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal) {
    uint8_t* st = TrainOrders_NextStop(t, 0, 0, 0);
    if (!st) return g_5c8598;
    Obj_BuildingRegion(st);
    float sum = 0.0f;
    if (F<int32_t>(t, 0x41) == -1) return sum;
    for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car; car = (uint8_t*)Obj_City352(car)) {
        if (F<int32_t>(Obj_Ref388(car), 0x41) != 1) continue;
        if (onlyLocal && F<int32_t>(car, 0x38c) != F<uint16_t>(hook::As<uint8_t*>(g_cargoTypes), 0x88)) continue;
        if (!(F<float>(car, 0x398) == g_5c8598)) sum = (float)(Obj_Rate(car) + sum);
    }
    return sum;
}
RT3_REPLACE(0x4a88e0, Train_CargoRate);

// 0x4a89a0. Records, for each pair of consecutive active stations on the route
// (+0 stops of 0x33 bytes, +4 count; stop +0x28 bit 7 skips, bit 6 = cargo list),
// the scaled distance from the previous station for each of the three special
// cargos it loads there; goes round the list twice.
RT3_NOINLINE void __fastcall Route_LinkStations(uint8_t* o) {
    int32_t loop = 1, wrapped = 0, i = 0, prev = -1;
    int32_t flags[3] = { 0, 0, 0 };
    int32_t px = 0, py = 0;
    do {
        int32_t next = i + 1;
        if (i >= F<int32_t>(o, 4)) {
            next = 0;
            if (wrapped) loop = 0;
            wrapped = 1;
        } else {
            uint8_t* e = F<uint8_t*>(o, 0) + i * 0x33;
            if ((int8_t)e[0x28] >= 0) {
                int32_t st = F<uint16_t>(e, 0x29);
                uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, st);
                if (Obj_BuildingStateActive(s) && st != prev) {
                    for (int32_t k = 0; k < 3; ++k) {
                        if (prev == -1 || !flags[k]) continue;
                        int32_t d = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(s)), Ftol(Obj_NodeCenterY(s)), px, py);
                        if (d < 1) d = 1;
                        float t1 = (float)((double)d / D(0x5caad8));
                        float f = (float)(((double)t1 + D(0x5c88e8)) * D(0x5d0060));
                        Site_RecordMin((uint8_t*)Collection_Get(g_stations, 0, prev), 0, st, k, f);
                    }
                    prev = st;
                    for (int32_t k = 0; k < 3; ++k) {
                        uint8_t* cargos = hook::As<uint8_t*>(g_cargoTypes);
                        void* ct = Collection_Get((Collection*)cargos, 0, F<uint32_t>(cargos, 0x8e + k * 4));
                        px = Ftol(Obj_NodeCenterX(s));
                        py = Ftol(Obj_NodeCenterY(s));
                        flags[k] = 0;
                        if (e[0x28] & 0x40) {
                            for (int32_t m = 0; m < e[0]; ++m)
                                if (Cargo_MatchesClass(ct, 0, F<int32_t>(e, 1 + m * 4))) flags[k] = 1;
                        } else flags[k] = Cargo_MatchesClass(ct, 0, F<int32_t>(e, 0x24));
                    }
                    if (wrapped) loop = 0;
                }
            }
        }
        i = next;
    } while (loop);
}
RT3_REPLACE(0x4a89a0, Route_LinkStations);

// 0x4a8bd0. Owning company (+0x51) of a train.
RT3_NOINLINE void* __fastcall Train_Company(void* t) { return Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)); }
RT3_REPLACE(0x4a8bd0, Train_Company);

// 0x4a8be0. Sets the train's engine sound from speed / top speed (+0x20 of its engine
// type, else 100) and, unless the train is in a non-moving state, the animation
// speed of its cars' animated parts from throttle / top speed (0.1..1).
RT3_NOINLINE void __fastcall Train_UpdateEngineEffects(uint8_t* t, int, float throttle, float speed) {
    uint8_t* et = nullptr;
    float top;
    bool have = false;
    if (F<int32_t>(t, 0x41) != -1) {
        void* e = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
        if (e) {
            et = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(e), 0x126));
            if (et) {
                top = F<float>(et, 0x20);
                have = true;
            }
        }
    }
    if (!have) {
        et = nullptr;
        top = 100.0f;
    }
    if (F<int32_t>(t, 0x41) == -1) return;
    uint8_t* e = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    if (!e) return;
    int32_t kind = F<int32_t>(et, 0x10);
    float k1, k2;
    if (kind == 2) {
        k1 = 0.13f;
        k2 = 0.3f;
    } else k1 = k2 = 0.2f;
    float r = (float)((double)speed / top);
    float a = (float)(((double)k1 + k1) * r + (D(0x5c8770) - k1));
    float b = (float)(D(0x5c8770) - (D(0x5c8770) - r) * k2);
    if (kind == 2 && speed <= Fl(0x5c8554))
        b = (float)(((double)speed / D(0x5c8d68) + D(0x5c8770)) * b * D(0x5c8628));
    Vehicle_SetEngineSound(e, 0, a, *(uint32_t*)&b);
    float v = (float)(((double)throttle + throttle) / top);
    if (Fl(0x5cb4a8) > v) v = 0.1f;
    else if (Fl(0x5c85b4) < v) v = 1.0f;
    if (throttle == Fl(0x5c8598)) return;
    switch (F<int32_t>(t, 0xe8)) {
    case 0x11: case 0xc: case 0xd: case 0xb: case 0x12: case 3: case 4: case 5: case 6: return;
    }
    for (int32_t j = 0; j < F<int32_t>(e, 0x31a); ++j)
        for (int32_t m = 0; m < 8; ++m) {
            uint8_t* s = F<uint8_t*>(F<uint8_t*>(e, 0x316), 0x10 + j * 0xc0 + m * 4);
            if (!s) continue;
            if (Obj_NodeVirtual5(e) == 1) {
                s[0x1d8] = 1;
                continue;
            }
            if (s[0x1d8]) {
                ListPair_Reset(s);
                s[0x1d8] = 0;
            }
            uint32_t k = F<void*>(s, 0x60) && Obj_NameIs5ce4f4(F<uint8_t*>(s, 0x60)) ? 0x5d006c : 0x5d0068;
            Train_SetTarget(s, 0, (float)((double)Fl(k) - v));
            Timer_SetFrameTime(s, 0, v);
        }
}
RT3_REPLACE(0x4a8be0, Train_UpdateEngineEffects);

void __fastcall Train_SmoothSpeed(uint8_t* t) {
    Smooth(t, 0x18e);
    if (F<int32_t>(t, 0xe8) == 0) Smooth(t, 0x18a);
}
RT3_REPLACE(0x4a8ec0, Train_SmoothSpeed);

// 0x4a8f50. Rebuilds a train's row wherever it is shown: the train window's
// header (view mode 2 on this train, window 0x6d3b20) and its row in the train
// list (view mode 1, window 0x6d3b34) when the row is on screen.
RT3_NOINLINE void __fastcall Train_RefreshRows(uint8_t* t) {
    uint8_t* view = g_gameWindow;
    if (!view) return;
    if (F<int32_t>(view, 0x8c) == 2 && F<int32_t>(view, 0x90) == F<int32_t>(t, 0x25) && g_trainWindow)
        TrainRow_Build(g_trainWindow, F<uint32_t>(t, 0x25), 1, 0, 1);
    view = g_gameWindow;
    if (F<int32_t>(view, 0x8c) != 1 || !g_6d3b34) return;
    uint8_t* list = Widget_FindChild(g_6d3b34, 0, Global6d3b30() ? 0x32c9 : 0x32ca);
    for (int32_t i = 0; i < F<int32_t>(list, 0xa2); ++i) {
        int32_t row = F<int32_t>(list, 0x1e2) + i;
        if (row < F<int32_t>(list, 0x1d2) && TreeView_NodeData(list, 0, row) == F<uint32_t>(t, 0x25)) {
            TrainRow_Build(g_6d3b34, F<uint32_t>(t, 0x25), 0, i, 1);
            return;
        }
    }
}
RT3_REPLACE(0x4a8f50, Train_RefreshRows);

// 0x4a9020. Field +0x10 of this object's city's 0x6ada84 record (0 if no city).
RT3_NOINLINE int32_t __fastcall Obj_CityRef10(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (!city) return 0;
    return F<int32_t>(Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126)), 0x10);
}
RT3_REPLACE(0x4a9020, Obj_CityRef10);

// 0x4a9060. Flags whether the consist has a car of type kind 0 (+0x197) / kind 4 (+0x198).
RT3_NOINLINE void __fastcall Train_ScanCarKinds(uint8_t* t) {
    t[0x198] = 0;
    t[0x197] = 0;
    for (void* c = FirstCar(t); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 0) t[0x197] = 1;
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 4) t[0x198] = 1;
    }
}
RT3_REPLACE(0x4a9060, Train_ScanCarKinds);

// 0x4a90d0. Gives the train a default name "<region> <random suffix>" (+0x69,
// 30 chars) from the region nearest to its rear (unless flag 1 of +0x1ce).
void __fastcall Train_DefaultName(uint8_t* t) {
    t[0x1a1] = 1;
    if (t[0x1ce] & 1) return;
    float x, y;
    int32_t id = F<int32_t>(t, 0x41);
    if (id != -1) {
        Car_RearPoint((uint8_t*)Collection_Get(g_62ba84, 0, id), 0, &x, &y);
    } else {
        x = 10.0f;
        y = 10.0f;
    }
    int32_t yi = (int32_t)y;
    int32_t xi = (int32_t)x;
    uint16_t r = Regions_NearestValue(xi, yi);
    const char* name = (const char*)0x5c87a8;
    if (r) {
        uint8_t* reg = (uint8_t*)Collection_Get(g_cities, 0, r);
        if (reg) name = (const char*)reg + 0x356;
    }
    const char* suffix = GetString(((const int32_t*)0x621aa8)[Rand(0, 4)]);
    char buf[0x7c];
    FormatText(buf, (const char*)0x5d0070, name, suffix, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    StrCopyN((char*)t + 0x69, buf, 0x1e);
}
RT3_REPLACE(0x4a90d0, Train_DefaultName);

// 0x4a91b0. 1 when the train (at node +0x49) can reach its current order's
// node (an explicit node +0x2b, or the station's node), over its company's
// allowed track (electrified only for electric engines unless `anyElectric`).
RT3_NOINLINE int32_t __fastcall Train_CanReachOrder(uint8_t* t, int, int32_t anyElectric) {
    if (F<int32_t>(t, 0x49) == -1 || F<int32_t>(t, 4) <= 0) return 0;
    uint8_t* order = (uint8_t*)Array33_Current(t);
    int32_t target;
    if ((int8_t)order[0x28] < 0) target = F<int32_t>(order, 0x2b);
    else {
        if (!Collection_IsValid(g_stations, 0, F<uint16_t>(order, 0x29))) return 0;
        target = F<int32_t>(Collection_Get(g_stations, 0, F<uint16_t>(order, 0x29)), 8);
    }
    if (!Collection_IsValid(g_tracks, 0, (uint32_t)target)) return 0;
    if (!Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x49))) return 0;
    int32_t need = F<int32_t>(Obj_CityRef(t), 0x10) == 2;
    if (anyElectric) need = 0;
    uint32_t* co = (uint32_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51));
    uint32_t cost;
    int32_t step = Route_NextStep(g_routes, 0, target, F<int32_t>(t, 0x49), &cost, need, co, nullptr, 0);
    return cost != 0xffffffff && step != -1 ? 1 : 0;
}
RT3_REPLACE(0x4a91b0, Train_CanReachOrder);

// 0x4a92a0. Region-grid byte at this object's city.
RT3_NOINLINE uint32_t __fastcall Obj_CityGridByte(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (!city) return 0;
    float y = (float)Obj_CellY(city);
    float x = (float)Obj_CellX(city);
    return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
}
RT3_REPLACE(0x4a92a0, Obj_CityGridByte);

// 0x4a92f0. Map grid byte at the position of car +0x45; 0 if none.
RT3_NOINLINE uint32_t __fastcall Train_GridByteAtCar45(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x45);
    if (id == -1) return 0;
    void* c = Collection_Get(g_62ba84, 0, id);
    if (!c) return 0;
    float y = (float)Obj_CellY(c);
    float x = (float)Obj_CellX(c);
    return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
}
RT3_REPLACE(0x4a92f0, Train_GridByteAtCar45);

// 0x4a9340. Train t: its target station id (+0xa0), else the next stop's.
RT3_NOINLINE int32_t __fastcall Train_TargetStation(uint8_t* t) {
    if (int32_t id = F<int32_t>(t, 0xa0)) return id;
    uint8_t* st = TrainOrders_NextStop(t, 0, 0, 0);
    return st ? F<int32_t>(st, 0) : 0;
}
RT3_REPLACE(0x4a9340, Train_TargetStation);

// 0x4a9360. The train's display name (+0x69): unless it has a custom name (+0x1a1,
// or flag +0x1ce bit 0) it is rebuilt as "Train N" (string 0x111), or just "N"
// with `numberOnly`, N being its rank + 1.
RT3_NOINLINE char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly) {
    char* name = (char*)train + 0x69;
    if (train[0x1a1] || (train[0x1ce] & 1)) return name;
    double n = (double)(Train_Rank(train) + 1);
    if (numberOnly) {
        Num_Format(name, 1, n, 10, 1, 0, 0);
        return name;
    }
    char num[0x40];
    Num_Format(num, 1, n, 10, 1, 0, 0);
    FormatText(name, GetString(0x111), num, 0, 0, 0, 0, 0, 0, 0);
    return name;
}
RT3_REPLACE(0x4a9360, Train_DisplayName);

// 0x4a9430. The track piece +0x49 belongs to the train's company (+0x51).
RT3_NOINLINE int32_t __fastcall Train_OnOwnTrack(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x49);
    if (id == -1) return 0;
    uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, id);
    if (!n) return 0;
    return F<uint16_t>(n, 0x22e) == F<int32_t>(t, 0x51) ? 1 : 0;
}
RT3_REPLACE(0x4a9430, Train_OnOwnTrack);

// 0x4a9460. This object's city record (0x6ada84) has +0x10 == 2.
RT3_NOINLINE int32_t __fastcall Obj_CityKindIs2(void* o) {
    int32_t v = 0;
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (city) v = F<int32_t>(Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126)), 0x10);
    return v == 2 ? 1 : 0;
}
RT3_REPLACE(0x4a9460, Obj_CityKindIs2);

// 0x4a94b0. Applies the pending template (+0x1c6: flag word +8 = all entries, else
// entry +0xa; the entry data from +0xc) to o's entries and frees it.
void __fastcall Obj_ApplyPendingTemplate(uint8_t* o) {
    uint8_t* t = F<uint8_t*>(o, 0x1c6);
    if (!t) return;
    uint8_t* src = t + 0xc;
    if (F<uint16_t>(t, 8)) {
        for (int32_t i = 0; i < F<int32_t>(o, 4); ++i) CopyEntry((uint8_t*)Array33_At(o, 0, i), src);
    } else {
        CopyEntry((uint8_t*)Array33_At(o, 0, F<uint16_t>(t, 0xa)), src);
    }
    crt::free(F<void*>(o, 0x1c6));
    F<void*>(o, 0x1c6) = nullptr;
}
RT3_REPLACE(0x4a94b0, Obj_ApplyPendingTemplate);

// 0x4a9630. Weighted ratio of the yearly totals over up to the last five years
// (+0x109/+0x131/+0x159, newest first; weights 6 - age, this year scaled by the
// month): (a + b + c) / a, scaled, plus 0.5, clamped to 0..1.
float __fastcall Obj_YearlyRatio(uint8_t* o) {
    uint8_t* w = g_world;
    int32_t n = F<uint16_t>(w, 0xd) - F<int32_t>(o, 0xd0) + 1;
    if (n > 5) n = 5;
    float W = 0.0f, X = 0.0f, Y = 0.0f;
    for (int32_t i = 0; i < (n > 10 ? 10 : n); ++i) {
        float a = F<float>(o, 0x109 + i * 4);
        float s2 = (float)((double)F<float>(o, 0x159 + i * 4) + F<float>(o, 0x131 + i * 4));
        float s3 = (float)((double)s2 + a);
        float wt = (float)(6 - i);
        wt = i == 0 ? (float)((double)(float)w[0xf] * wt) : (float)((double)wt * Fl(0x5c8788));
        W = (float)((double)wt + W);
        X = (float)((double)wt * a + X);
        Y = (float)((double)wt * s3 + Y);
    }
    float d1 = Y > Fl(0x5c85b4) ? W : 1.0f;
    Y = (float)((double)Y * Fl(0x5c8788) / d1);
    float d2 = W > Fl(0x5c85b4) ? W : 1.0f;
    X = (float)((double)X * Fl(0x5c8788) / d2);
    double r;
    float d = Fl(0x5c85b4) > X ? 1.0f : X;
    if (!(Y < Fl(0x5c8598))) r = (double)Y / d * D(0x5c8da8);
    else r = (double)Y / d;
    float f = (float)(r + D(0x5c8628));
    if (f > Fl(0x5c85b4)) return 1.0f;
    if (f < Fl(0x5c8598)) f = 0.0f;
    return f;
}
RT3_REPLACE(0x4a9630, Obj_YearlyRatio);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4a9840. Recenter the first car on its node, refresh it, then make every
// car follow its predecessor and refresh its pose. Keeps the repeated head
// lookup of the original.
RT3_NOINLINE void __fastcall Train_ReanchorConsist(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x41);
    if (id == -1 || !Collection_Get(g_62ba84, 0, (uint32_t)id)) return;
    id = F<int32_t>(t, 0x41);
    uint8_t* car = id == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)id);
    Car_SetPiece(car, 0, F<int32_t>(car, 0x356), 0x3e395810);
    Car_UpdatePose(car, 0, 0.0f);
    do {
        Car_FollowLeader(car, 0, nullptr);
        Car_UpdatePose(car, 0, 0.0f);
        car = (uint8_t*)Obj_City352(car);
    } while (car);
}
RT3_REPLACE(0x4a9840, Train_ReanchorConsist);

// 0x4a98c0. Paints train t in its company scheme (not in multiplayer): the
// lead car (+0x41 in 0x62ba84) of engine type (+0x126 of its car type in
// 0x6ada84) gets the name of the scheme's entry +0x262 (in the list +0x196
// of the scheme +0x72 in 0x62ba88), and so does a tender behind it (car
// kind 3); the texture of that name in the pack list (0x6d4020 +0x429b8)
// is applied (+0x60) and, for engines with a second skin (+0x7a), the
// second skin (+0xa0) goes on the second car (QUIRK: on null for a
// single car).
RT3_NOINLINE void __fastcall Train_ApplyLivery(uint8_t* t) {
    if (IsMultiplayer()) return;
    auto lead = [&]() -> uint8_t* {
        return F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    };
    auto engine = [&]() -> uint8_t* {
        uint8_t* car = lead();
        return car ? (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(car), 0x126)) : nullptr;
    };
    const uint32_t schemeId = F<uint32_t>(engine(), 0x72);
    uint8_t* scheme = schemeId ? (uint8_t*)Collection_Get(g_carTypes, 0, schemeId) : nullptr;
    const int32_t entry = F<int32_t>(scheme, 0x262);
    if (entry == -1) return;
    auto name = [&]() {
        Collection* list = F<Collection*>(scheme, 0x196);
        return (const char*)Collection_Get(list, 0, Collection_IdAt(list, 0, (uint32_t)F<int32_t>(scheme, 0x262), 0));
    };
    uint8_t* car = lead();
    FormatText((char*)car + 0x3ac, name(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    lead()[0x3cc] = 1;
    if (uint8_t* tender = (uint8_t*)Obj_City352(lead())) {
        if (F<int32_t>(Obj_Ref388(tender), 0x41) == 3) {
            FormatText((char*)tender + 0x3ac, name(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            tender[0x3cc] = 1;
        }
    }
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b8)); ++i) {
        Collection* skins = F<Collection*>(g_pack, 0x429b8);
        uint8_t* skin = (uint8_t*)Collection_Get(skins, 0, Collection_IdAt(skins, 0, (uint32_t)i, 0));
        if (crt::stricmp((const char*)lead() + 0x3ac, (const char*)skin)) continue;
        Obj_SetTexture(lead(), 0, (const char*)skin + 0x60);
        if (!engine()[0x7a]) return;
        uint8_t* second = nullptr;
        if (t[0xf9] >= 2) {
            second = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
            if (second) second = (uint8_t*)Obj_City352(second);
        }
        Obj_SetTexture(second, 0, (const char*)skin + 0xa0);
        return;
    }
}
RT3_REPLACE(0x4a98c0, Train_ApplyLivery);

// 0x4a9b70 (RET 8). Appends a car of type `type` carrying cargo `cargo` (0:
// none) to train t behind its last car (+0x45; no train cars +0x41: 0). Unless
// the last car is coupling (virtual +0x14) or off the track, the new car's
// spot is found half the two cars' lengths behind it (Node_Advance, from the
// front). The car is created there (Cars_Add), joins the train (+0x390, chain
// +0x34e / +0x352, the train's last car), adds its weight (type +0xfd) to
// +0xf5 and counts in +0xf9 (cargo cars, type +0x41 == 1, also in +0xfa, and
// those with cargo flag +0x47 in +0xfb). It faces like the last car when that
// one couples, else as the walk ended; it is drawn faded (alpha 0x3f, opacity
// 0.25) when +0xf4 is set; placed on the piece found (0x41c700 and a zero
// move, 0x41ce80) or else off the track (virtual +0x10). Then the train's car
// kinds and rows are refreshed. Returns the new car's id.
// With the last car off the track (and not coupling) the original takes the
// new car's direction from a local it never wrote (a stack leftover of its
// caller); it starts at 0 here (facing 1).
RT3_NOINLINE uint32_t __fastcall Train_AppendCar(uint8_t* t, int, uint32_t type, uint32_t cargo) {
    if (F<int32_t>(t, 0x41) == -1) return 0;
    uint32_t x = 0, z = 0, at = 0;
    int32_t dir = 0;
    uint8_t* last = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x45));
    int32_t piece = -1;
    bool walked = false;
    if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)last)[0x14 / 4])(last) && F<int32_t>(last, 0x356) != -1) {
        walked = true;
        uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(last, 0x356));
        const uint32_t pos = F<uint32_t>(last, 0x35e);
        const float lastLen = F<float>(Obj_Ref388(last), 0x10d);
        const float newLen = F<float>(Collection_Get(g_carTypes, 0, type), 0x10d);
        const float dist = (float)(((double)newLen + lastLen) / Fl(0x5c878c));
        dir = F<int32_t>(last, 0x35a) == 0;
        piece = ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, float, uint32_t*, uint32_t*, uint32_t*, int32_t*, int32_t*, float*, int32_t,
                                       int32_t, int32_t))&Node_Advance)(n, 0, pos, dist, &x, &z, &at, &dir, nullptr, nullptr, -1, -1, 1);
    }
    uint8_t* ct = (uint8_t*)Collection_Get(g_carTypes, 0, type);
    const uint32_t id = ((uint32_t(__fastcall*)(Collection*, int, uint8_t*, uint32_t, uint32_t, uint32_t, uint32_t))&Cars_Add)(
        g_62ba84, 0, ct, cargo, x, z, 0);
    uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, id);
    F<int32_t>(car, 0x390) = F<int32_t>(t, 0x25);
    F<float>(t, 0xf5) = (float)((double)F<float>(Obj_Ref388(car), 0xfd) + F<float>(t, 0xf5));
    t[0xf9]++;
    if (F<int32_t>((uint8_t*)Obj_Ref388(car), 0x41) == 1) {
        t[0xfa]++;
        if (cargo && ((uint8_t*)Collection_Get(g_cargoTypes, 0, cargo))[0x47]) t[0xfb]++;
    }
    F<int32_t>(car, 0x352) = -1;
    F<int32_t>(car, 0x34e) = F<int32_t>(t, 0x45);
    F<uint32_t>((uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x45)), 0x352) = id;
    F<uint32_t>(t, 0x45) = id;
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)last)[0x14 / 4])(last)) {
        F<int32_t>(car, 0x35a) = F<int32_t>(last, 0x35a);
    } else {
        if (!walked) ++g_unwrittenReads[1];
        F<int32_t>(car, 0x35a) = dir == 0;
    }
    SceneObj_SetAlpha(car, 0, t[0xf4] ? 0x3f : 0xff, 1);
    Model_SetOpacity(car, 0, (float)(t[0xf4] ? 0.25 : 1.0));
    if (piece == -1) {
        ((void(__fastcall*)(uint8_t*))(*(void***)car)[0x10 / 4])(car);
    } else {
        Car_SetPiece(car, 0, piece, (int32_t)at);
        Car_Advance(car, 0, 0, nullptr, nullptr, -1);
    }
    Train_ScanCarKinds(t);
    Train_RefreshRows(t);
    return id;
}
RT3_REPLACE(0x4a9b70, Train_AppendCar);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4a9e00. Unlink and release a car, maintaining the train's ends, total
// weight, car/cargo counts, and the remaining cars' predecessor positions.
// +0x1ab suppresses the final positioning and UI refresh while bulk-editing.
RT3_NOINLINE void __fastcall Train_DetachCar(uint8_t* t, int, uint32_t id) {
    uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, id);
    int32_t prev = F<int32_t>(car, 0x34e);
    if (prev != -1) {
        void* p = Collection_Get(g_62ba84, 0, (uint32_t)prev);
        F<uint32_t>(p, 0x352) = F<uint32_t>(car, 0x352);
    }
    int32_t next = F<int32_t>(car, 0x352);
    if (next != -1) {
        void* p = Collection_Get(g_62ba84, 0, (uint32_t)next);
        F<uint32_t>(p, 0x34e) = F<uint32_t>(car, 0x34e);
    }
    if (F<uint32_t>(t, 0x41) == id) F<uint32_t>(t, 0x41) = F<uint32_t>(car, 0x352);
    if (F<uint32_t>(t, 0x45) == id) F<uint32_t>(t, 0x45) = F<uint32_t>(car, 0x34e);
    uint8_t* type = (uint8_t*)Obj_Ref388(car);
    F<float>(t, 0xf5) = F<float>(t, 0xf5) - F<float>(type, 0xfd);
    --t[0xf9];
    type = (uint8_t*)Obj_Ref388(car);
    if (F<int32_t>(type, 0x41) == 1) {
        --t[0xfa];
        uint32_t cargo = F<uint32_t>(car, 0x38c);
        if (cargo && ((uint8_t*)Collection_Get(g_cargoTypes, 0, cargo))[0x47]) --t[0xfb];
    }
    Collection* cars = g_62ba84;
    cars->vtbl->release(cars, id);
    if (!t[0x1ab]) {
        int32_t head = F<int32_t>(t, 0x41);
        if (head != -1) {
            uint8_t* first = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)head);
            for (uint8_t* c = (uint8_t*)Obj_City352(first); c; c = (uint8_t*)Obj_City352(c))
                Car_FollowLeader(c, 0, nullptr);
        }
        Train_ScanCarKinds(t);
        Train_RefreshRows(t);
    }
}
RT3_REPLACE(0x4a9e00, Train_DetachCar);

// 0x4a9f40. Train t whose vehicle v stands at node v+0x356: heads for the
// nearest (squared distance from v) of the current schedule stop's station
// (unless a waypoint, +0x28 bit 7), the stop +0xa4 and the stop +0xa8 (each
// only when the current one is not of kind 3 / 4). Picking +0xa4 that has
// no free slot reserves its blocks (flags 2). The chosen station (+0xac,
// once per change unless no target) gets its route split at its track node,
// and the target +0x94 (old one in +0x1ba) becomes the node itself, or - when
// none of it and its two neighbours is a one-way node (+0x20e) - the
// neighbour with the higher route cost. Station positions come from the
// building in multiplayer before version 101, else from the track node (x,
// z, with both heights taken as 0).
void __fastcall Train_HeadForNearestStop(uint8_t* t, int, uint8_t* v) {
    Collection* stations = g_stations;
    float p0[3], p1[3];
    SceneObj_GetPos(v, 0, (uint32_t*)&p0[0], (uint32_t*)&p0[1], (uint32_t*)&p0[2]);
    int32_t haveA = 0, haveB = 0;
    float dCur = 0, dA = 0, dB = 0;
    int32_t target = -1;
    uint8_t* seg = (uint8_t*)Array33_Current(t);
    if (!seg) return;
    auto dist = [&](int32_t id) {
        uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, (uint32_t)id));
        if (IsMultiplayer() && Game_EffectiveVersion() <= 100) SceneObj_GetPos(b, 0, (uint32_t*)&p1[0], (uint32_t*)&p1[1], (uint32_t*)&p1[2]);
        else {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, (uint32_t)id);
            uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(st, 8));
            memcpy(&p1[0], n + 0xc, 4);
            memcpy(&p1[2], n + 0x10, 4);
            p1[1] = 0;
            p0[1] = 0;
        }
        float dz = (float)((double)p1[2] - p0[2]);
        float dy = (float)((double)p1[1] - p0[1]);
        float dx = (float)((double)p1[0] - p0[0]);
        return (float)((double)dx * dx + (double)dy * dy + (double)dz * dz);
    };
    auto curKind = [&]() {
        uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, (uint32_t)target));
        uint8_t* k = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
        return k[0x8c];
    };
    if (!(seg[0x28] & 0x80)) {
        target = F<uint16_t>(seg, 0x29);
        if (Collection_IsValid(stations, 0, (uint32_t)target)) dCur = dist(target);
        else dCur = 0;
    }
    if (F<int32_t>(t, 0xa4) != -1 && Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0xa4)) &&
        ((seg[0x28] & 0x80) || curKind() != 3)) {
        dA = dist(F<int32_t>(t, 0xa4));
        haveA = 1;
    }
    if (F<int32_t>(t, 0xa8) != -1 && Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0xa8)) &&
        ((seg[0x28] & 0x80) || curKind() != 4)) {
        dB = dist(F<int32_t>(t, 0xa8));
        haveB = 1;
    }
    bool pickB = false;
    if (haveA == 1 && (dA < dCur || !(dCur != 0.0f))) {
        target = F<int32_t>(t, 0xa4);
        if (haveB == 1 && dB < dA) pickB = true;
        else {
            uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, (uint32_t)target));
            if (!Obj_ClearAttachSlot(b, 0, -1) && !F<int32_t>(Collection_Get(g_stations, 0, (uint32_t)target), 0x58c) &&
                Obj_BeginBlocks(b) == 0xffffffff)
                Obj_SetBlockFlags(b, 0, 2, 1);
        }
    } else if (haveB == 1 && (dB < dCur || !(dCur != 0.0f)))
        pickB = true;
    if (pickB) target = F<int32_t>(t, 0xa8);
    if (target == -1) return;
    if (F<int32_t>(t, 0xac) == target && F<int32_t>(t, 0x94) != -1) return;
    if (!Collection_IsValid(g_stations, 0, (uint32_t)target)) return;
    F<int32_t>(t, 0xac) = target;
    int32_t nodeId = F<int32_t>((uint8_t*)Collection_Get(g_stations, 0, (uint32_t)target), 8);
    if (nodeId == -1) return;
    uint8_t* node = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)nodeId);
    if (F<int32_t>(Collection_Get(g_routes, 0, F<uint32_t>(node, 0x212)), 0x10) > 1) {
        if (F<int32_t>(node, 0x206) != -1) {
            uint32_t r = F<uint32_t>(node, 0x212);
            if (r == F<uint32_t>(Collection_Get(g_tracks, 0, F<uint32_t>(node, 0x206)), 0x212))
                Routes_SplitAtNode(g_routes, 0, node, 1);
        }
        if (F<int32_t>(node, 0x20a) != -1) {
            uint32_t r = F<uint32_t>(node, 0x212);
            if (r == F<uint32_t>(Collection_Get(g_tracks, 0, F<uint32_t>(node, 0x20a)), 0x212))
                Routes_SplitAtNode(g_routes, 0, node, 0);
        }
    }
    int32_t n206 = F<int32_t>(node, 0x206), n20a = F<int32_t>(node, 0x20a);
    uint32_t cost206, cost20a;
    int32_t from = F<int32_t>(v, 0x356);
    uint32_t* co = (uint32_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51));
    Route_NextStep(g_routes, 0, from, n206, &cost206, F<int32_t>(Obj_CityRef(t), 0x10) == 2, co, nullptr, 0);
    from = F<int32_t>(v, 0x356);
    co = (uint32_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51));
    Route_NextStep(g_routes, 0, from, n20a, &cost20a, F<int32_t>(Obj_CityRef(t), 0x10) == 2, co, nullptr, 0);
    int32_t pick = cost206 > cost20a ? n206 : n20a;
    int32_t dest;
    if (F<int32_t>(node, 0x20e) == -1 && F<int32_t>(Collection_Get(g_tracks, 0, (uint32_t)n206), 0x20e) == -1 &&
        F<int32_t>(Collection_Get(g_tracks, 0, (uint32_t)n20a), 0x20e) == -1)
        dest = pick;
    else
        dest = F<int32_t>(node, 0x202);
    Collection_Get(g_tracks, 0, (uint32_t)dest);
    F<int32_t>(t, 0x1ba) = F<int32_t>(t, 0x94);
    F<int32_t>(t, 0x94) = dest;
}
RT3_REPLACE(0x4a9f40, Train_HeadForNearestStop);

// 0x4aa590. Updates o's speed factor +0xcc from its progress towards node +0x94
// (0x6cfca8) along the segment from node +0x1ca, for scene object obj: the factor
// starts from base +0x8c (-1 = unset: take +0xcc and the start node +0x49, or -1
// again when obj is more than 5 steps from the node or o is in a special state)
// and is scaled by the remaining share of the segment (distance projected on the
// segment; multiplayer games before version 0x69 compare squared distances from
// the origin instead), or by steps / +0x90 without a start node. The result is
// kept at least 1 and at most the city's limit (+0x20). Always returns 0.
int32_t __fastcall Train_UpdateApproachFactor(uint8_t* o, int, uint8_t* obj) {
    F<uint8_t>(o, 0x8b) = 0;
    if (F<int32_t>(o, 0xac) == -1 || F<int32_t>(o, 0x94) == -1) return 0;
    if (!Collection_IsValid(g_stations, 0, F<uint32_t>(o, 0xac))) return 0;
    int32_t target = F<int32_t>(o, 0x94), steps;
    __asm {
        push 5
        push target
        mov eax, obj
        mov ecx, 0x4a78d0
        call ecx
        mov steps, eax
    }
    if (steps == -1) {
        F<uint32_t>(o, 0x8c) = 0xbf800000;
        return 0;
    }
    if (F<uint32_t>(o, 0x8c) == 0xbf800000) {
        F<uint32_t>(o, 0x8c) = F<uint32_t>(o, 0xcc);
        F<uint32_t>(o, 0x1ca) = F<uint32_t>(o, 0x49);
        return 0;
    }
    if (((int32_t(__fastcall*)(uint8_t*))&Obj_KindE8Special)(o) || Train_StateBOr12(o)) {
        F<uint32_t>(o, 0x8c) = 0xbf800000;
        return 0;
    }
    Collection* nodes = g_tracks;
    uint32_t from = F<uint32_t>(o, 0x1ca);
    double share;
    if (from == 0xffffffff || from == 0 || !Collection_IsValid(nodes, 0, from)) {
        float s = (float)(double)(uint32_t)steps;
        float n = (float)(double)F<uint32_t>(o, 0x90);
        share = (double)s / n;
    } else if (IsMultiplayer() && Game_EffectiveVersion() <= 0x68) {
        float f = 1.0f;
        float a[3], pos[3];
        uint8_t* n1 = (uint8_t*)Collection_Get(nodes, 0, from);
        a[0] = F<float>(n1, 0xc);
        a[2] = F<float>(n1, 0x10);
        SceneObj_GetPos(obj, 0, (uint32_t*)&pos[0], (uint32_t*)&pos[1], (uint32_t*)&pos[2]);
        pos[1] = 0.0f;
        uint8_t* n2 = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(o, 0x94));
        float bx = F<float>(n2, 0xc), bz = F<float>(n2, 0x10);
        float d1 = (float)((double)a[2] * a[2] + (double)a[0] * a[0]);
        float d2 = (float)((double)bz * bz + (double)bx * bx);
        float d3 = (float)((double)pos[2] * pos[2] + (double)pos[0] * pos[0]);
        float span = (float)crt::fabs((double)d2 - d1);
        float done = (float)crt::fabs((double)d3 - d1);
        if (span > Fl_205(0x5c8598)) {
            f = (float)((double)done / span);
            if (Fl_205(0x5c85b4) < f) f = 1.0f;
        }
        share = Fl_205(0x5c85b4) - f;
    } else {
        float a[3], b[3], dir[3], pos[3], v[3];
        uint8_t* n1 = (uint8_t*)Collection_Get(nodes, 0, from);
        a[0] = F<float>(n1, 0xc);
        a[1] = 0.0f;
        a[2] = F<float>(n1, 0x10);
        uint8_t* n2 = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(o, 0x94));
        b[0] = F<float>(n2, 0xc);
        b[1] = 0.0f;
        b[2] = F<float>(n2, 0x10);
        Vec3_Sub(b, 0, dir, a);
        float len = (float)Vec3_Length(dir);
        if (!(len > Fl_205(0x5c8598))) goto clamp;
        Vec3_Normalize(dir);
        SceneObj_GetPos(obj, 0, (uint32_t*)&pos[0], (uint32_t*)&pos[1], (uint32_t*)&pos[2]);
        pos[1] = 0.0f;
        Vec3_Sub(pos, 0, v, a);
        float dot = (float)(((double)v[2] * dir[2] + (double)v[1] * dir[1]) + (double)v[0] * dir[0]);
        float f = (float)((double)dot / len);
        if (Fl_205(0x5c85b4) < f) {
            share = Fl_205(0x5c85b4) - 1.0f;
        } else {
            if (!(f >= Fl_205(0x5c8598))) goto clamp;
            share = Fl_205(0x5c85b4) - f;
        }
    }
    F<float>(o, 0xcc) = (float)(share * F<float>(o, 0x8c));
clamp:
    if (Fl_205(0x5c85b4) > F<float>(o, 0xcc)) {
        F<uint32_t>(o, 0xcc) = 0x3f800000;
        return 0;
    }
    if (F<float>(o, 0xcc) > F<float>(Obj_CityRef(o), 0x20)) F<uint32_t>(o, 0xcc) = F<uint32_t>(Obj_CityRef(o), 0x20);
    return 0;
}
RT3_REPLACE(0x4aa590, Train_UpdateApproachFactor);

// 0x4aa990. The vehicle's speed (+0xcc) adjusted for the grade of its model's
// heading (so +0x16e..+0x176): 1 - horizontal length^2 of it, times the speed over
// the engine's top speed (type +0x20), added downhill (x 0x5d007c) or taken off
// uphill (x 0x5d0078, at least 1).
RT3_NOINLINE float __fastcall Vehicle_GradeSpeed(uint8_t* o, int, uint8_t* so) {
    float base = F<float>(o, 0xcc);
    if (Fl(0x5c8598) == F<float>(o, 0xcc)) return base;
    SceneObj_UpdateMatrix(so);
    float a = F<float>(so, 0x16e), b = F<float>(so, 0x172), c = F<float>(so, 0x176);
    float s = (float)((double)a * a + (double)c * c + (double)b * Fl(0x5c8598));
    uint8_t* type = nullptr;
    int32_t id = F<int32_t>(o, 0x41);
    void* car = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (car) type = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    float ratio = (float)((double)F<float>(o, 0xcc) / F<float>(type, 0x20));
    double flat = (double)Fl(0x5c85b4) - s;
    if (b < Fl(0x5c8598)) {
        ratio = (float)(flat * Fl(0x5d007c) * ratio);
        return (float)((double)ratio + base);
    }
    ratio = (float)(flat * Fl(0x5d0078) * ratio);
    base = (float)((double)base - ratio);
    if (Fl(0x5c85b4) > base) base = 1.0f;
    return base;
}
RT3_REPLACE(0x4aa990, Vehicle_GradeSpeed);

// 0x4aaad0. A train t reaching block object other: unless its engine (+0x41) is a
// type that ignores blocks, and while it is not moving faster than 1 (+0xe0),
// records which side it is on (+0x192, from 0x4a7bc0) and marks other's block
// slots accordingly (side 2: both ways; else claims the slot, releasing a stale
// attachment, and flags the block when only side 2 remains free).
RT3_NOINLINE void __fastcall Train_EnterBlock(uint8_t* t, int, uint8_t* car, uint8_t* other) {
    if (F<int32_t>(t, 0x41) != -1) {
        void* e = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
        if (e && F<int32_t>(Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(e), 0x126)), 0x10)) return;
    }
    if (!(Fl_205(0x5c85b4) > F<float>(t, 0xe0))) {
        F<int32_t>(t, 0x192) = -1;
        return;
    }
    int32_t side = Signal_IsBehind(t, 0, car, other);
    F<int32_t>(t, 0x192) = side;
    if (side == 2) {
        Obj_MarkBlockSlot(other, 0, 0, side, 1);
        Obj_SetBlockFlags(other, 0, 2, 1);
        return;
    }
    int32_t front = side == 0;
    Obj_MarkBlockSlot(other, 0, 0, side, 1);
    if (Obj_BeginBlocks(other) == 0xffffffff) {
        Obj_MarkBlockSlot(other, 0, 1, F<int32_t>(t, 0x192), 0);
        return;
    }
    if (Obj_ClearAttachSlot(other, 0, front) == 1) {
        Obj_MarkBlockSlot(other, 0, 1, F<int32_t>(t, 0x192), 0);
        return;
    }
    if (Obj_BeginBlocks(other) == 1 && Obj_TestBlock(other, 0, 2, 0) == 1) Obj_SetBlockFlags(other, 0, 2, 1);
}
RT3_REPLACE(0x4aaad0, Train_EnterBlock);

// 0x4aac00. A train t leaving its station block (+0xac, while +0x1a2 is set):
// frees the station building's block for its side unless a city reference keeps
// it, then releases the previous block (+0xa4; its claim count +0x58c, the side
// reset when still claimed).
RT3_NOINLINE void __fastcall Train_LeaveBlock(uint8_t* t) {
    if (F<int32_t>(t, 0xac) == -1 || t[0x1a2] != 1) return;
    if (!Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0xac))) return;
    void* st = Collection_Get(g_stations, 0, F<uint32_t>(t, 0xac));
    if (!Obj_CityRef10(t)) {
        uint8_t* b = (uint8_t*)Obj_Building(st);
        if (!Obj_ClearAttachSlot(b, 0, -1) && F<int32_t>(t, 0x192) == F<int32_t>(b, 0x262) && Obj_BeginBlocks(b) == 0xffffffff)
            Obj_SetBlockFlags(b, 0, 2, 1);
    }
    t[0x1a2] = 0;
    Collection* sts = g_stations;
    if (Collection_IsValid(sts, 0, F<uint32_t>(t, 0xa4)) && F<int32_t>(Collection_Get(sts, 0, F<uint32_t>(t, 0xa4)), 0x58c)) {
        Obj_Dec58c((uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(t, 0xa4)));
        if (F<int32_t>(Collection_Get(g_stations, 0, F<uint32_t>(t, 0xa4)), 0x58c)) F<int32_t>(t, 0x192) = -1;
    }
    F<int32_t>(t, 0xa4) = -1;
}
RT3_REPLACE(0x4aac00, Train_LeaveBlock);

// 0x4aad20. Once (+0x1a2) when the vehicle has entered a signal's block (+0xa4) and
// is not in a city: clears the signal's block slot for the side it came from.
RT3_NOINLINE void __fastcall Vehicle_PassSignal(uint8_t* o) {
    int32_t id = F<int32_t>(o, 0xa4);
    Collection* sites = g_stations;
    if (id == -1 || !Collection_IsValid(sites, 0, id)) return;
    if (!Obj_CityRef10(o) && !o[0x1a2]) {
        uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, F<uint32_t>(o, 0xa4)));
        uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(o, 0xa4));
        Collection_Get(g_tracks, 0, F<uint32_t>(s, 8));
        int32_t car = F<int32_t>(o, 0x41);
        uint8_t* c = car != -1 ? (uint8_t*)Collection_Get(g_62ba84, 0, car) : nullptr;
        Obj_MarkBlockSlot(b, 0, 0, Signal_IsBehind(o, 0, c, b), 0);
    }
    o[0x1a2] = 1;
}
RT3_REPLACE(0x4aad20, Vehicle_PassSignal);

void __fastcall Train_LeaveSite(uint8_t* t) {
    uint8_t* s = StationSite(t);
    if (!s || F<int32_t>(s, 0x58c) <= 0) return;
    s = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(t, 0xa4));
    if (F<int32_t>(s, 0x58c) > 0) F<int32_t>(s, 0x58c)--;
}
RT3_REPLACE(0x4aadd0, Train_LeaveSite);

void __fastcall Train_EnterSite(uint8_t* t) {
    if (uint8_t* s = StationSite(t)) F<int32_t>(s, 0x58c)++;
}
RT3_REPLACE(0x4aae40, Train_EnterSite);

// 0x4aae90. A train waiting at a signal (state +0xe8 4 or 5) passes it: its
// speed limit +0x8c becomes -1.0, the held block +0x94 moves to +0x1ba and the
// train leaves its block.
RT3_NOINLINE void __fastcall Train_PassSignal(uint8_t* t) {
    int32_t st = F<int32_t>(t, 0xe8);
    if (st != 5 && st != 4) return;
    Vehicle_PassSignal(t);
    F<uint32_t>(t, 0x8c) = 0xbf800000;
    F<int32_t>(t, 0x1ba) = F<int32_t>(t, 0x94);
    F<int32_t>(t, 0x94) = -1;
    Train_LeaveBlock(t);
}
RT3_REPLACE(0x4aae90, Train_PassSignal);

// 0x4aaee0. Sums the train's car weights (+0x394; engines skipped unless
// `withEngines`): with `slope` each is scaled by its tilt's speed factor (doubled
// uphill when the train is stopped), with `curves` by 1 + the bend at its track node
// (capped 1.5). Outputs the mean / max bend and the weight-averaged tilt.
RT3_NOINLINE float __fastcall Train_ConsistWeight(uint8_t* t, int, int32_t quiet, int32_t slope, int32_t curves, int32_t withEngines,
                                     float* meanBend, float* maxBend, float* meanTilt) {
    if (F<int32_t>(t, 0x41) == -1) return 0.0f;
    Collection* cars = g_62ba84;
    float weightSum = 0.0f, tiltSum = 0.0f;
    if (!quiet) {
        if (uint8_t* c = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(t, 0x41)))
            Collection_Get(g_6ada84, 0, F<uint32_t>((uint8_t*)Obj_Ref388(c), 0x126));
    }
    uint8_t* car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(t, 0x41));
    float total = 0.0f, bendSum = 0.0f, bendCount = 0.001f, bendMax = 0.0f;
    if (!withEngines) {
        while (car && (F<int32_t>((uint8_t*)Obj_Ref388(car), 0x41) == 2 || F<int32_t>((uint8_t*)Obj_Ref388(car), 0x41) == 3)) {
            if (F<int32_t>(car, 0x352) == -1) {
                car = nullptr;
                break;
            }
            car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(car, 0x352));
        }
    }
    while (car) {
        float w = F<float>(car, 0x394);
        if (slope || meanTilt) {
            float tilt = Car_TiltValue(car);
            if (meanTilt) {
                weightSum = (float)((double)w + weightSum);
                tiltSum = (float)((double)tilt * w + tiltSum);
            }
            if (slope) {
                tilt = Car_TiltValue(car);
                if (F<float>(t, 0xe4) == Fl_195(0x5c8598) && tilt > Fl_195(0x5c8598)) tilt = (float)((double)tilt + tilt);
                w = (float)(SpeedFactor(tilt) * w);
            }
        }
        if (curves) {
            float k = 1.0f;
            if (F<int32_t>(car, 0x356) != -1) {
                Collection* nodes = g_tracks;
                uint8_t* node = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(car, 0x356));
                float h = Vehicle_Heading(node, 0, 1);
                float bend = 0.0f;
                if (F<int32_t>(node, 0x206) != -1)
                    bend = Bend(h, (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(node, 0x206)));
                if (F<int32_t>(node, 0x20a) != -1) {
                    float b = Bend(h, (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(node, 0x20a)));
                    bend = (float)((double)b + bend);
                }
                k = (float)((double)bend + Fl_195(0x5c85b4));
                k = (float)(k < D(0x5c8578) ? (double)k : 1.5);
                bendSum = (float)((double)bend + bendSum);
                bendCount = (float)((double)bendCount + Fl_195(0x5c85b4));
                if (!(bendMax > bend)) bendMax = bend;
            }
            w = (float)((double)k * w);
        }
        total = (float)((double)w + total);
        if (F<int32_t>(car, 0x352) == -1) break;
        car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(car, 0x352));
    }
    if (meanBend) *meanBend = (float)((double)bendSum / bendCount);
    if (maxBend) *maxBend = bendMax;
    if (meanTilt) *meanTilt = (float)((double)tiltSum / (Fl_195(0x5c85b4) > weightSum ? 1.0f : weightSum));
    return total;
}
RT3_REPLACE(0x4aaee0, Train_ConsistWeight);

// 0x4ab2d0. Next track node for a train (+0x25 = node to avoid) arriving at `cur`
// from `prev` (-1 = none): the link on the far side, else the alternative route at
// +0x253 (either side), else via a switch (+0x218 == 4, branch +0x20e) at the
// previous or current node - only where the switch does not turn (left) the wrong way
// and the far ends agree. Taking a switch records it. The result must be a node the
// train can enter, else -1 (switch record cleared).
RT3_NOINLINE int32_t __fastcall Train_NextNode(uint8_t* t, int, int32_t forward, uint32_t prev, uint32_t curId, int32_t* switched) {
    Collection* nodes = g_tracks;
    uint8_t* prevN = prev == 0xffffffff ? nullptr : (uint8_t*)Collection_Get(nodes, 0, prev);
    uint8_t* cur = (uint8_t*)Collection_Get(nodes, 0, curId);
    *switched = 0;
    int32_t side;
    if (F<uint32_t>(cur, 0x206) == prev) side = 1;
    else if (F<uint32_t>(cur, 0x20a) == prev) side = 0;
    else if (F<uint32_t>(cur, 0x20e) == prev) side = (cur[0x201] >> 1) & 1;
    else return -1;
    int32_t n = Node_GetLink(cur, 0, side);
    if (n == F<int32_t>(t, 0x25)) n = -1;
    if (n != -1) goto finish;
    if (Collection_IsValid(nodes, 0, F<uint32_t>(cur, 0x253))) {
        int32_t s = 1;
        n = Node_GetLink(Collection_Get(nodes, 0, F<uint32_t>(cur, 0x253)), 0, 1);
        if (n == -1) {
            s = 0;
            n = Node_GetLink(Collection_Get(nodes, 0, F<uint32_t>(cur, 0x253)), 0, 0);
        }
        if (n != -1) {
            if (n != F<int32_t>(t, 0x25)) TookSwitch(t, F<uint32_t>(cur, 0x253), s != 0, switched);
            goto finish;
        }
    }
    if (!prevN) return n;
    if (prevN[0x218] == 4 && F<uint32_t>(prevN, 0x20e) == curId) {
        int32_t s = forward == 0;
        if (Node_GetLink(prevN, 0, s) != -1 && !Node_TurnsLeft(nodes, 0, F<uint32_t>(prevN, 0x202))) {
            n = Node_GetLink(prevN, 0, s);
            TookSwitch(t, F<uint32_t>(prevN, 0x202), forward == 0, switched);
        }
    }
    if (n != -1) goto finish;
    if (cur[0x218] != 4) return n;
    if (F<uint32_t>(cur, 0x20e) == prev) {
        int32_t s = side == 0;
        if (Node_GetLink(cur, 0, s) != n && Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) {
            n = Node_GetLink(cur, 0, s);
            TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
            if (n != -1) goto finish;
        }
    }
    if (cur[0x218] != 4 || F<int32_t>(cur, 0x20e) == -1) return n;
    {
        int32_t s = side == 0;
        if (Node_GetLink(cur, 0, s) == -1) return n;
        uint8_t* b = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(cur, 0x20e));
        int32_t c = F<uint32_t>(b, 0x206) == curId;
        int32_t r1 = Node_GetLink(cur, 0, s);
        if (r1 == Node_GetLink(b, 0, c)) {
            if (Node_GetLink(b, 0, c == 0) != F<int32_t>(t, 0x25) && !Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) {
                n = Node_GetLink(cur, 0, s);
                if (n != -1) {
                    TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
                    goto finish;
                }
            }
        }
        int32_t r4 = Node_GetLink(b, 0, c == 0);
        if (Node_GetLink(cur, 0, s) != r4) return n;
        if (!Node_TurnsLeft(nodes, 0, F<uint32_t>(cur, 0x202))) return n;
        n = Node_GetLink(cur, 0, s);
        if (n == -1) return n;
        TookSwitch(t, F<uint32_t>(cur, 0x202), side == 0, switched);
    }
finish:
    if (!Train_IsRunning(t, 0, (uint32_t)n)) {
        *switched = 0;
        F<int32_t>(t, 0x1cf) = -1;
        return -1;
    }
    return n;
}
RT3_REPLACE(0x4ab2d0, Train_NextNode);

// 0x4ab730. Moves the train to position `rank` among its company's trains
// by giving it a sort key (list 1) between its new neighbours.
RT3_NOINLINE void __fastcall Train_SetRank(uint8_t* t, int, int32_t rank) {
    if (rank == Train_Rank(t)) return;
    Collection* trains = g_trains;
    uint32_t prevKey = 1;
    for (int32_t i = 0; i < Collection_Count(trains); ++i) {
        uint8_t* tr = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, i, 1));
        if (tr == t || F<int32_t>(tr, 0x51) != F<int32_t>(t, 0x51)) continue;
        int32_t s = F<int32_t>(tr, 0xe8);
        if ((s == 0x11 || s == 0xc || s == 0xd) && (tr[0x1ce] & 2)) continue;
        int32_t rk = Train_Rank(tr);
        uint32_t id = F<uint32_t>(tr, 0x25);
        if (rank < rk) {
            int32_t k = (int32_t)Collection_GetKey1(trains, 0, id);
            int32_t v = k - (int32_t)prevKey > 2 ? (k + (int32_t)prevKey) / 2 : k - 1;
            if (v < 1) v = 1;
            Collection_SetKey1(trains, 0, F<uint32_t>(t, 0x25), (uint32_t)v);
            return;
        }
        prevKey = Collection_GetKey1(trains, 0, id);
    }
    Collection_SetKey1(trains, 0, F<uint32_t>(t, 0x25), prevKey + 1000);
}
RT3_REPLACE(0x4ab730, Train_SetRank);

// 0x4ab830. Ends train t's breakdown (+0xa0, kept as +0x9c, from the day
// +0xb0): clears its state, recomputes the consist's summed +0x394 (+0xb4)
// and weight (+0xb8), then stops the lead car's steam and resets its sound.
RT3_NOINLINE void __fastcall Train_ClearBreakdown(uint8_t* t) {
    if (F<int32_t>(t, 0xa0)) {
        F<int32_t>(t, 0x9c) = F<int32_t>(t, 0xa0);
        F<uint32_t>(t, 0xb0) = F<uint32_t>(g_world, 0x15);
        F<int32_t>(t, 0xa0) = 0;
        F<int32_t>(t, 0x39) = 0;
        t[0x1b8] = 0;
        F<int32_t>(t, 0x3d) = 0;
        float sum = 0.0f;
        if (F<int32_t>(t, 0x41) != -1) {
            if (uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)))
                Collection_Get(g_6ada84, 0, F<uint32_t>((uint8_t*)Obj_Ref388(c), 0x126));
            for (uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); c;) {
                sum = (float)((double)F<float>(c, 0x394) + sum);
                if (F<int32_t>(c, 0x352) == -1) break;
                c = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(c, 0x352));
            }
        }
        F<float>(t, 0xb4) = sum;
        F<float>(t, 0xb8) = Train_ConsistWeight(t, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr);
    }
    if (F<int32_t>(t, 0x41) == -1 || !Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41))) return;
    Car_SetBrokenDown(F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)), 0, 0);
    Vehicle_SelectSound(F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)), 0, 0);
}
RT3_REPLACE(0x4ab830, Train_ClearBreakdown);

// 0x4ab980. Train t: enters state `state` with value v (+0xe8 / +0xec,
// whole numbers in multiplayer), clearing the wait fields. State 0 (run,
// advancing tutorial step 0x1ea) first clears a breakdown; without a route
// (+0xd8) it becomes state 0x10, with a blocked first node or signal state
// 7. Running states show the train (+0xf4 0: its cars opaque, relinking its
// stations), the hidden states (1..10, 15, 16, 19) hide it (cars at alpha
// 0x3f, its stations unlinked). Returns the whole value in multiplayer, else 0
// (callers pass it on).
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v) {
    for (;;) {
        F<int32_t>(t, 0x55) = 0;
        F<int32_t>(t, 0x1a7) = 0;
        F<int32_t>(t, 0x59) = 0;
        F<int32_t>(t, 0x5d) = 0;
        if (!state && Tutorial_Is1ea()) Tutorial_Advance();
        if (IsMultiplayer()) v = (float)Ftol_207(v);
        if (state != 0) break;
        Train_ClearBreakdown(t);
        if (!F<int32_t>(t, 0xd8)) {
            v = 0.0f, state = 0x10;
            continue;
        }
        if (!Train_CheckPendingNode(t) || !Train_SignalsClear(t)) {
            v = 0.0f, state = 7;
            continue;
        }
        break;
    }
    static const uint8_t kind[20] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1 };
    if ((uint32_t)state <= 0x13) {
        int32_t hide = kind[state] == 1;
        uint8_t old = t[0xf4];
        if (old != (hide ? 1 : 0)) {
            t[0xf4] = hide ? 1 : 0;
            for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car;) {
                SceneObj_SetAlpha(car, 0, hide ? 0x3f : 0xff, 1);
                Model_SetOpacity(car, 0, t[0xf4] ? 0.25f : 1.0f);
                int32_t next = F<int32_t>(car, 0x352);
                if (next == -1) break;
                car = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)next);
            }
            if (old != t[0xf4]) {
                if (hide) Station_UnlinkNodes(t);
                else Station_RelinkNodes(t);
            }
        }
    }
    F<int32_t>(t, 0xe8) = state;
    if (IsMultiplayer()) {
        int32_t r = Ftol_207(v);
        F<float>(t, 0xec) = (float)r;
        return r;
    }
    F<uint32_t>(t, 0xec) = *(uint32_t*)&v;
    return 0;
}
RT3_REPLACE(0x4ab980, Train_SetState);

// 0x4abca0. Train t (its order list at +0): when the next stop's region
// differs from the stored cell (+0x181 / +0x185), every car gets the new
// region for its cargo value; the cell becomes the stop's quarter cell (-1
// without a stop). Then the stations are relinked.
RT3_NOINLINE void __fastcall Train_RetargetRegion(uint8_t* t) {
    uint8_t* cell = nullptr;
    if (F<int32_t>(t, 0x181) != -1) {
        uint8_t* m = g_map;
        cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * F<int32_t>(t, 0x185) + F<int32_t>(t, 0x181)) * 0x1d7;
    }
    uint8_t* st = TrainOrders_NextStop(t, 0, 0, 0);
    uint8_t* region = st ? Obj_BuildingRegion(st) : nullptr;
    if (region != cell && F<int32_t>(t, 0x41) != -1)
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car; car = (uint8_t*)Obj_City352(car))
            Car_SetCargoValue(car, 0, region);
    if (st) {
        F<int32_t>(t, 0x181) = Obj_BuildingQuarterX(st);
        F<int32_t>(t, 0x185) = Obj_BuildingQuarterY(st);
    } else {
        F<int32_t>(t, 0x181) = -1;
        F<int32_t>(t, 0x185) = -1;
    }
    Route_LinkStations(t);
}
RT3_REPLACE(0x4abca0, Train_RetargetRegion);

// 0x4abd70. Sets a train's running flag (+0xd8): with option +0x287 its lead car's
// attachments are shown only while stopped; stopping a train in state 0 halts and
// ghosts it into state 0x10 (as Train_StopAndGhost).
RT3_NOINLINE void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running) {
    if (F<int32_t>(train, 0xd8) == running) return;
    F<int32_t>(train, 0xd8) = running;
    if (F<int32_t>(g_app, 0x287) && F<int32_t>(train, 0x41) != -1 &&
        Collection_Get(g_62ba84, 0, F<uint32_t>(train, 0x41))) {
        int32_t lead = F<int32_t>(train, 0x41);
        uint8_t* car = lead == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)lead);
        Model_SetAttachmentsFlag(car, 0, F<int32_t>(train, 0xd8) ? 0 : 1);
    }
    if (F<int32_t>(train, 0xd8) || F<int32_t>(train, 0xe8)) return;
    F<int32_t>(train, 0x55) = 0;
    F<int32_t>(train, 0x1a7) = 0;
    F<int32_t>(train, 0x59) = 0;
    F<int32_t>(train, 0x5d) = 0;
    uint8_t old = train[0xf4];
    if (old != 1) {
        train[0xf4] = 1;
        Train_ApplyGhost(train, 0x3f);
        if (old != train[0xf4]) Station_UnlinkNodes(train);
    }
    F<int32_t>(train, 0xe8) = 0x10;
    F<int32_t>(train, 0xec) = 0;
}
RT3_REPLACE(0x4abd70, Train_SetRunning);

// 0x4abf00. Shares a delivery's revenue: each track owner (+0x1ac ids, +0x1b2
// shares, +0x1b8 count) gets its share (stat 4), the train's company the rest
// (stat 1, or 2 when `kind` is 0; tallied in +0x109); the origin / destination
// stations (+0x9c/+0xa0), their regions and building kinds, and the world totals
// record the revenue and `units` of `cargo`.
void __fastcall Train_ShareRevenue(uint8_t* t, int, float revenue, int32_t kind, float units, int32_t cargo) {
    int32_t total = 0;
    for (int32_t i = 0; i < t[0x1b8]; ++i) {
        uint32_t own = F<uint16_t>(t, 0x1ac + i * 2);
        int32_t share = F<uint16_t>(t, 0x1b2 + i * 2);
        total += share;
        if (!Collection_IsValid(Players(), 0, own)) continue;
        float cap = ShareCap(t);
        float frac = (float)((double)(float)share / cap);
        if (!(frac > Fl(0x5c8598))) continue;
        Company_PostStat(Collection_Get(Players(), 0, own), 0, 4, (float)((double)frac * revenue), 1);
    }
    float cap = ShareCap(t);
    float frac = (float)((double)(float)total / cap);
    float rest = (float)((D(0x5c8770) - frac) * revenue);
    if (rest < Fl(0x5c8598)) rest = 0.0f;
    F<float>(t, 0x109) = (float)((double)rest + F<float>(t, 0x109));
    Company_PostStat(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0, kind == 0 ? 2 : 1, rest, 1);
    uint8_t *reg1 = nullptr, *reg2 = nullptr, *kind1 = nullptr, *kind2 = nullptr;
    if (Collection_IsValid(Stations(), 0, F<uint32_t>(t, 0x9c))) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint32_t>(t, 0x9c));
        F<float>(s, 0x53c) = (float)((double)rest + F<float>(s, 0x53c));
        F<float>(s, 0x4ec) = (float)((double)units + F<float>(s, 0x4ec));
        reg1 = (uint8_t*)Obj_BuildingRegionAt(s);
        kind1 = (uint8_t*)Obj_BuildingKind(s);
    }
    if (Collection_IsValid(Stations(), 0, F<uint32_t>(t, 0xa0))) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint32_t>(t, 0xa0));
        F<float>(s, 0x514) = (float)((double)rest + F<float>(s, 0x514));
        F<float>(s, 0x4c4) = (float)((double)units + F<float>(s, 0x4c4));
        reg2 = (uint8_t*)Obj_BuildingRegionAt(s);
        kind2 = (uint8_t*)Obj_BuildingKind(s);
    }
    if (reg1) {
        Station_AddStat49(reg1, 0, units, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), cargo);
        Station_AddStat51(reg1, 0, rest, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)));
    }
    if (reg2) {
        Station_AddStat45(reg2, 0, units, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), cargo);
        Station_AddStat4d(reg2, 0, rest, (uint8_t*)Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)));
    }
    if (kind1) {
        F<float>(kind1, 0x2be) = (float)((double)units + F<float>(kind1, 0x2be));
        F<float>(kind1, 0x2c6) = (float)((double)rest + F<float>(kind1, 0x2c6));
    }
    if (kind2) {
        F<float>(kind2, 0x2ba) = (float)((double)units + F<float>(kind2, 0x2ba));
        F<float>(kind2, 0x2c2) = (float)((double)rest + F<float>(kind2, 0x2c2));
    }
    uint8_t* w = World();
    F<float>(w, 0xbce) = (float)((double)units + F<float>(w, 0xbce));
    F<float>(w, 0xa26 + cargo * 4) = (float)((double)units + F<float>(w, 0xa26 + cargo * 4));
    F<float>(World(), 0xbd6) = (float)((double)rest + F<float>(World(), 0xbd6));
}
RT3_REPLACE(0x4abf00, Train_ShareRevenue);

// 0x4ac250. Books revenue `amount` for the train: scaled by the effects 0x4e + the
// engine kind (at the car's region) and 0x4d and the company's quality factor,
// added to +0x131 and posted to the company (+0x51) as stat 7. Not when paused (+0x82).
RT3_NOINLINE void __fastcall Train_EarnRevenue(uint8_t* o, int, float amount) {
    if (g_newGame[0x82]) return;
    auto regionOf = [&]() -> uint32_t {
        int32_t id = F<int32_t>(o, 0x41);
        void* car = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
        if (!car) return 0;
        float y = (float)Obj_CellY(car);
        float x = (float)Obj_CellX(car);
        return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
    };
    Collection* companies = g_companies;
    uint32_t region1 = regionOf();
    uint32_t player1 = F<uint32_t>(Collection_Get(companies, 0, F<uint32_t>(o, 0x51)), 0x3b);
    uint32_t region2 = regionOf();
    uint32_t player2 = F<uint32_t>(Collection_Get(g_companies, 0, F<uint32_t>(o, 0x51)), 0x3b);
    uint8_t* type = nullptr;
    int32_t id = F<int32_t>(o, 0x41);
    void* car = id != -1 ? Collection_Get(g_62ba84, 0, id) : nullptr;
    if (car) type = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(type, 0x10) + 0x4e, F<uint32_t>(o, 0x51), player2, region2);
    float m = (float)((double)Effects_Multiplier(World(), 0, 0x4d, F<uint32_t>(o, 0x51), player1, region1) * m1);
    amount = (float)((double)m * amount);
    void* comp = Collection_Get(g_companies, 0, F<uint32_t>(o, 0x51));
    amount = (float)((double)Company_QualityFactor(comp) * amount);
    F<float>(o, 0x131) = (float)((double)amount + F<float>(o, 0x131));
    Company_PostStat(Collection_Get(g_companies, 0, F<uint32_t>(o, 0x51)), 0, 7, amount, 1);
}
RT3_REPLACE(0x4ac250, Train_EarnRevenue);

// 0x4ac3f0. Books a train expense (scaled by the company's quality factor)
// into +0x159 and the company's stat 8 (not in the editor).
RT3_NOINLINE void __fastcall Train_AddExpense(uint8_t* t, int, float amount) {
    if (g_newGame[0x82]) return;
    float v = (float)((double)Company_QualityFactor(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51))) * amount);
    F<float>(t, 0x159) = (float)((double)v + F<float>(t, 0x159));
    Company_PostStat(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0, 8, v, 1);
}
RT3_REPLACE(0x4ac3f0, Train_AddExpense);

// 0x4ac460. The train's running cost: base by age (new engines cheaper), times its
// load (+0xb8), less for idle engines of kind 0 (+0xe0 = 0), by condition (+0xdc),
// by the difficulty step (engine +0x18 plus the regional bonuses 0x55 / kind + 0x56)
// and halved for +0x197. Free with world +0x4b03.
RT3_NOINLINE float __fastcall Train_RunningCost(uint8_t* t) {
    if (F<int32_t>(World(), 0x4b03)) return Fl(0x5c8598);
    uint8_t* et = EngineType(Engine(t));
    int32_t age = F<uint16_t>(World(), 0xd) - F<int32_t>(t, 0xd4);
    float v = (float)((double)(age + 0x1e) / D(0x5c8588) * D(0x5d0080));
    if (age <= 0) v = (float)(v * D(0x5c88c8));
    else if (age <= 1) v = (float)(v * D(0x5c8880));
    int32_t n = (int32_t)F<float>(t, 0xb8);
    v = (float)(((double)n + D(0x5c9680)) / D(0x5c85e8) * v);
    if (!F<int32_t>(et, 0x10) && F<float>(t, 0xe0) == Fl(0x5c8598)) v = (float)(v * D(0x5c8698));
    if (F<float>(t, 0xdc) < D(0x5c85b8)) v = (float)((double)v * Fl(0x5c8900));
    else v = (float)((D(0x5c8770) - F<float>(t, 0xdc) + D(0x5c8770)) * v);
    uint32_t r1 = EngineRegion(t);
    uint32_t r2 = EngineRegion(t);
    void* w = hook::As<void*>(g_world);
    int32_t p = Obj_Get25(w);
    int32_t b = Obj_CargoBonus(w, 0, F<int32_t>(et, 0x10) + 0x56, Obj_Get21(w), p, r2);
    p = Obj_Get25(w);
    b += Obj_CargoBonus(w, 0, 0x55, Obj_Get21(w), p, r1);
    int32_t step = F<int32_t>(et, 0x18) + b;
    if (step < 0) step = 0;
    if (step > 10) step = 10;
    v = (float)(v * (double)((const float*)0x5f3430)[step]);
    if (t[0x197]) v = (float)(v * D(0x5c8628));
    return (float)(v * D(0x5c8640));
}
RT3_REPLACE(0x4ac460, Train_RunningCost);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4ac6d0. Load existing freight cars and fill the order's remaining slots.
RT3_NOINLINE int32_t __fastcall Train_LoadConsist(uint8_t* train, int, uint8_t* station, uint32_t* destination) {
    uint32_t cargos[8];
    int32_t count = 0;
    uint8_t* row = ((uint8_t*)Array33_Current((void*)(train)));
    uint8_t* car = F_train_loading<uint32_t>(train, 0x41) == 0xffffffff ? nullptr : Get(0x62ba84, F_train_loading<uint32_t>(train, 0x41));
    while (car) {
        uint8_t* type = ((uint8_t*)Obj_Ref388((void*)(car)));
        if (F_train_loading<int32_t>(type, 0x41) == 1) {
            cargos[count++] = F_train_loading<uint32_t>(car, 0x38c);
            if (F_train_loading<float>(car, 0x398) < *(const float*)0x5c85b4) Load(car, station, destination);
        }
        car = ((uint8_t*)Obj_City352((void*)(car)));
    }
    F_train_loading<uint16_t>(train, 0x1c0) = 0;
    train[0x1c2] = 0;
    F_train_loading<uint16_t>(train, 0x1c3) = 0;
    train[0x1bf] = 0;
    int32_t minimum = row[row[0x28] & 0x40 ? 0x21 : 0x22];
    int32_t limit = row[0x28] & 0x40 ? 8 : row[0x23];
    while (count < limit) {
        int32_t lost = 0, fallback = 0, selected = -1, cls = 900;
        if (row[0x28] & 0x40) {
            uint8_t used[8] = {};
            for (int32_t k = 0; k < count; ++k) {
                uint8_t* cargo = Get(0x62ba8c, cargos[k]);
                int32_t best = -1, score = -1;
                for (int32_t i = 0; i < limit; ++i) {
                    if (!used[i] && Cargo_MatchesClass((void*)(cargo), 0, F_train_loading<int32_t>(row, 1 + i * 4))) {
                        int32_t tag = F_train_loading<int32_t>(row, 1 + i * 4);
                        int32_t rank = tag == 900 ? 10 : tag == 901 || tag == 902 ? 20 : 30;
                        if (rank > score) { score = rank; best = i; }
                    }
                }
                if (best != -1) used[best] = 1;
            }
            for (int32_t i = 0; i < limit; ++i) {
                if (used[i]) continue;
                cls = F_train_loading<int32_t>(row, 1 + i * 4);
                selected = Station_PickCargo(
                    station, 0, cls, *destination, &lost, &fallback);
                if (selected != -1) break;
            }
        } else {
            cls = F_train_loading<int32_t>(row, 0x24);
            selected = Station_PickCargo(
                station, 0, cls, *destination, &lost, &fallback);
        }
        if (selected == -1) {
            if (lost && cls != 900 && cls != 901 && cls != 902) {
                train[0x1bf] = 1; F_train_loading<uint16_t>(train, 0x1c0) = (uint16_t)cls;
            }
            if (fallback && cls != 900) {
                train[0x1c2] = 1; F_train_loading<uint16_t>(train, 0x1c3) = (uint16_t)fallback;
            }
            break;
        }
        float cost = count ? 3456.0f : 8064.0f;
        if (F_train_loading<int32_t>(hook::As<void*>(g_app), 0x24f)) {
            __asm {
                fld cost
                fmul qword ptr ds:[0x5c9ba0]
                fstp cost
            }
        }
        uint8_t* world = g_world;
        __asm {
            mov eax, world
            fld cost
            fmul dword ptr [eax + 0x4ca2]
            fstp cost
        }
        uint32_t region = 0;
        uint32_t head = F_train_loading<uint32_t>(train, 0x41);
        car = head == 0xffffffff ? nullptr : Get(0x62ba84, head);
        if (car) {
            uint32_t y = Coordinate(car, (uint32_t)(uintptr_t)&Obj_CellY);
            uint32_t x = Coordinate(car, (uint32_t)(uintptr_t)&Obj_CellX);
            region = ((uint32_t(__fastcall*)(void*, int, uint32_t, uint32_t))&Map_GridByte)(
                hook::As<void*>(g_map), 0, x, y);
        }
        uint8_t* company = Get(0x62be10, F_train_loading<uint32_t>(train, 0x51));
        uint32_t player = F_train_loading<uint32_t>(company, 0x3b), owner = F_train_loading<uint32_t>(train, 0x51);
        world = g_world;
        __asm {
            push region
            push player
            push owner
            push 0x61
            mov ecx, world
            call Effects_Multiplier
            fmul cost
            fstp cost
        }
        if (IsMultiplayer()) {
            int32_t amount;
            __asm {
                fld cost
                call Rt3_Ftol
                mov amount, eax
                fild amount
                fstp cost
            }
        }
        __asm {
            mov eax, train
            fld cost
            fadd dword ptr [eax + 0xf0]
            fstp dword ptr [eax + 0xf0]
        }
        uint8_t* cargo = Get(0x62ba8c, (uint32_t)selected);
        uint32_t typeId = CarType_ForCargo(cargo);
        uint32_t carId = Train_AppendCar(
            train, 0, typeId, (uint32_t)selected);
        car = Get(0x62ba84, carId);
        cargos[count++] = (uint32_t)selected;
        Load(car, station, destination);
    }
    int32_t result = count >= minimum ? 1 : 0;
    if (!(F_train_loading<float>(train, 0xf0) < *(const float*)0x5d0088)) F_train_loading<uint32_t>(train, 0xf0) = 0;
    return result;
}
RT3_REPLACE(0x4ac6d0, Train_LoadConsist);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4acaf0. Add the requested service cars and optionally fill to eight cars.
RT3_NOINLINE void __fastcall Train_AutoAddCars(uint8_t* t) {
    uint8_t* row = ((uint8_t*)Array33_Current((void*)(t)));
    if ((row[0x28] & 0x10) && !t[0x198]) {
        uint32_t type = FindType(4);
        if (type) Append(t, type);
    }
    if ((row[0x28] & 0x20) && !t[0x197]) {
        uint32_t type = FindType(0);
        if (type) Append(t, type);
    }
    if (F_train_initialization<int32_t>(hook::As<void*>(g_world), 0x4af3)) {
        uint32_t type = FindType(5);
        if (type) {
            while (Train_CountCarsOther(t) < 8) Append(t, type);
        }
    }
}
RT3_REPLACE(0x4acaf0, Train_AutoAddCars);

// 0x4acb90. Renames train t through text box 9 (string 0xf2b, plus 0xf2c
// while it has a custom name, +0x1ce bit 0): a number moves the train to that
// rank (returns 1); an empty name restores the numbered default; anything
// else becomes its custom name (+0x69, 30 characters). Unnamed trains
// (+0x1a1 clear, no custom name) show their rank number first.
RT3_NOINLINE uint32_t __fastcall Train_PromptName(uint8_t* t) {
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xbe, 0, 0, 0, 0);
    if (!t[0x1a1] && !(t[0x1ce] & 1)) Num_Format((char*)t + 0x69, 1, (double)(Train_Rank(t) + 1), 10, 1, 0, 0);
    char name[0x1f4], prompt[0x1f4], orig[0x1f4];
    strcpy(name, (const char*)t + 0x69);
    TrimSpaces(name, 0);
    strcpy(orig, name);
    strcpy(prompt, GetString(0xf2b));
    if ((t[0x1ce] & 1) && !t[0x1a1]) strcat(prompt, GetString(0xf2c));
    Dialog_RunMessage(prompt, 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)name, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    TrimSpaces(name, 0);
    if (!crt::stricmp(orig, name)) return 0;
    if (Str_IsNumber(name)) {
        Train_SetRank(t, 0, crt::atoi(name));
        return 1;
    }
    if (!strlen(name)) {
        if ((t[0x1ce] & 1) && !t[0x1a1]) t[0x1ce] &= 0xfe;
        return 0;
    }
    if (!t[0x1a1]) t[0x1ce] |= 1;
    StrCopyN((char*)t + 0x69, name, 0x1e);
    return 0;
}
RT3_REPLACE(0x4acb90, Train_PromptName);

 // namespace

// 0x4acda0. Tears train t down (+0x1ab marks it): a train in a block (state
// +0xe8 4 / 5) passes its signal and leaves the block (+0x94 saved to
// +0x1ba); the path +0x1c6 and the consist +0 are freed; its owner (+0x51,
// not in a tutorial) gets the engine's value back (stat 0x10, posted 0xc;
// Obj_EffectScale of the lead car's engine type +0x126 in 0x6ada84); the
// stations forget it, the cars detach, it leaves the key table 0x6ada90
// and its effect +0xc8 goes.
void __fastcall Train_Teardown(uint8_t* t) {
    t[0x1ab] = 1;
    if (F<int32_t>(t, 0xe8) == 5 || F<int32_t>(t, 0xe8) == 4) {
        Vehicle_PassSignal(t);
        F<uint32_t>(t, 0x8c) = 0xbf800000;
        F<uint32_t>(t, 0x1ba) = F<uint32_t>(t, 0x94);
        F<int32_t>(t, 0x94) = -1;
        Train_LeaveBlock(t);
    }
    if (void* path = F<void*>(t, 0x1c6)) {
        crt::free(path);
        F<void*>(t, 0x1c6) = nullptr;
    }
    const uint32_t player = F<uint32_t>(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0x3b);
    uint8_t* engine = nullptr;
    if (F<int32_t>(t, 0x41) != -1) {
        if (void* car = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)))
            engine = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    }
    const float value = Obj_EffectScale(engine, 0, F<uint32_t>(t, 0x51), player);
    if (Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)) && !g_newGame[0x82]) {
        const float refund = -value;
        Company_AddStat(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0, 0x10, refund);
        Company_PostStat(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0, 0xc, refund, 0);
    }
    Station_UnlinkNodes(t);
    while (F<int32_t>(t, 0x41) != -1) Train_DetachCar(t, 0, F<uint32_t>(t, 0x41));
    if (void* cars = F<void*>(t, 0)) {
        crt::free(cars);
        F<void*>(t, 0) = nullptr;
    }
    if (g_6ada90) Table_ReleaseKey8(g_6ada90, 0, F<int32_t>(t, 0x25));
    if (uint32_t* fx = F<uint32_t*>(t, 0xc8)) Effect_Free(fx);
    Nop436060(g_world, 0, F<int32_t>(t, 0x25));
}
RT3_REPLACE(0x4acda0, Train_Teardown);

// 0x4acf30. Moves the train engine's sounds along, updates its horn and sets the
// engine sound and animation from its grade-adjusted speed and the train speed.
RT3_NOINLINE void __fastcall Train_UpdateSound(uint8_t* t) {
    if (F<int32_t>(t, 0x41) == -1) return;
    uint8_t* e = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    if (!e) return;
    Obj_MoveChannels(e);
    Vehicle_UpdateHorn(e, 0, 0x3f800000);
    float v = Vehicle_GradeSpeed(t, 0, e);
    Train_UpdateEngineEffects(t, 0, v, F<float>(t, 0xcc));
}
RT3_REPLACE(0x4acf30, Train_UpdateSound);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4acf90. Enter state 3, stop, attach to the target block, count the target
// visit, reset animations, and reanchor cars on network versions >= 1.02.
RT3_NOINLINE void __fastcall Train_SetState3(uint8_t* t, int, uint8_t* car, uint8_t* target, uint32_t flag) {
    if (F<int32_t>(t, 0xe8) == 3) return;
    F<uint32_t>(t, 0xcc) = 0;
    t[0xfc] = (uint8_t)flag;
    Train_SetState(t, 0, 3, 4320.0f);
    uint8_t* block = (uint8_t*)Obj_Building(target);
    Train_EnterBlock(t, 0, car, block);
    ++F<uint32_t>(target, 0x564);
    t[0x1a2] = 0;
    Train_ResetCarAnimations(t);
    if (IsMultiplayer() && Net_AllHaveVersion(1.02f)) Train_ReanchorConsist(t);
}
RT3_REPLACE(0x4acf90, Train_SetState3);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4ad020. Enter state 5: stop, reset route counters, ghost and unlink the
// consist if needed, set its timeout to 7200, and reset/reanchor animations.
// All three multiplayer probes are observable calls, even the unused results.
RT3_NOINLINE void __fastcall Train_SetState5(uint8_t* t, int, void*, uint8_t* target, uint32_t flag) {
    if (F<int32_t>(t, 0xe8) == 5) return;
    t[0xfc] = (uint8_t)flag;
    F<uint32_t>(t, 0xcc) = 0;
    ++F<uint32_t>(target, 0x564);
    F<uint32_t>(t, 0x55) = 0;
    F<uint32_t>(t, 0x1a7) = 0;
    F<uint32_t>(t, 0x59) = 0;
    F<uint32_t>(t, 0x5d) = 0;
    IsMultiplayer();
    uint8_t old = t[0xf4];
    if (old != 1) {
        t[0xf4] = 1;
        uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
        while (car) {
            SceneObj_SetAlpha(car, 0, 0x3f, 1);
            Model_SetOpacity(car, 0, t[0xf4] ? 0.25f : 1.0f);
            int32_t next = F<int32_t>(car, 0x352);
            if (next == -1) break;
            car = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)next);
        }
        if (old != t[0xf4]) Station_UnlinkNodes(t);
    }
    F<uint32_t>(t, 0xe8) = 5;
    IsMultiplayer();
    F<uint32_t>(t, 0xec) = 0x45e10000;
    Train_ResetCarAnimations(t);
    if (IsMultiplayer() && Net_AllHaveVersion(1.02f)) Train_ReanchorConsist(t);
}
RT3_REPLACE(0x4ad020, Train_SetState5);

// 0x4ad170. Stops a train and ghosts it: clears its speeds (+0x55, +0x59, +0x5d,
// +0x1a7), ghosts it (as Train_SetGhost) and sets state 7 (+0xe8) with +0xec
// cleared. (The original also asks IsMultiplayer twice and ignores the answer.)
RT3_NOINLINE void __fastcall Train_StopAndGhost(uint8_t* train) {
    F<int32_t>(train, 0x55) = 0;
    F<int32_t>(train, 0x1a7) = 0;
    F<int32_t>(train, 0x59) = 0;
    F<int32_t>(train, 0x5d) = 0;
    uint8_t old = train[0xf4];
    if (old != 1) {
        train[0xf4] = 1;
        Train_ApplyGhost(train, 0x3f);
        if (old != train[0xf4]) Station_UnlinkNodes(train);
    }
    F<int32_t>(train, 0xe8) = 7;
    F<int32_t>(train, 0xec) = 0;
}
RT3_REPLACE(0x4ad170, Train_StopAndGhost);

// 0x4ad260. Train t: routes toward its current order's node from the lead
// car's node (+0x356, direction +0x35a): leaving a stop state (8 / 9 / 10 /
// 0x13) it runs again (state 0); the next step (Route_NextStep, fast engines
// - engine kind 2 - may need the flag) turns the lead car round when it lies
// behind; without a route it retries plainly and stops: lost (0x13) for a
// city-bound train with a cost, else blocked (10); no order at all stops it
// (8).
RT3_NOINLINE void __fastcall Train_Route(uint8_t* t) {
    bool bad;
    int32_t dest = Train_OrderNode(t, &bad);
    if (bad || dest == -1) {
        Train_SetState(t, 0, 8, 0.0f);
        return;
    }
    int32_t st = F<int32_t>(t, 0xe8);
    if (st == 8 || st == 9 || st == 0xa || st == 0x13) Train_SetState(t, 0, 0, 0.0f);
    uint8_t* lead = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    uint32_t companyId = F<uint32_t>(t, 0x51);
    int32_t node = F<int32_t>(lead, 0x356);
    Collection* companies = g_companies;
    uint8_t* engine = nullptr;
    if (F<int32_t>(t, 0x41) != -1)
        if (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)))
            engine = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(Obj_Ref388(car), 0x126));
    int32_t to = Train_OrderNode(t, &bad);
    uint32_t* co = (uint32_t*)Collection_Get(companies, 0, companyId);
    uint32_t cost;
    int32_t next = Route_NextStep(g_routes, 0, node, to, &cost, F<int32_t>(engine, 0x10) == 2 ? 1 : 0, co, nullptr, 0);
    int32_t dir = F<int32_t>(lead, 0x35a);
    uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(lead, 0x356));
    Station_UnlinkNodes(t);
    if ((F<int32_t>(n, 0x206) == next && dir) || (F<int32_t>(n, 0x20a) == next && !dir)) Obj_Toggle35a(lead);
    F<int32_t>(t, 0x35) = 0;
    if (next != -1 && (int32_t)cost != -1) return;
    companyId = F<uint32_t>(t, 0x51);
    companies = g_companies;
    to = Train_OrderNode(t, &bad);
    co = (uint32_t*)Collection_Get(companies, 0, companyId);
    Route_NextStep(g_routes, 0, node, to, &cost, 0, co, nullptr, 0);
    if ((int32_t)cost != -1 && Obj_CityKindIs2(t)) Train_SetState(t, 0, 0x13, 0.0f);
    else Train_SetState(t, 0, 0xa, 0.0f);
}
RT3_REPLACE(0x4ad260, Train_Route);

 // namespace

// 0x4ad7a0. Train t crashes (lead car +0x33e, crash smoke, sound 1, state
// 0x11, string 0x114, owner tally +0xd0f, a world event when not forced).
RT3_NOINLINE void __fastcall Train_Crash(uint8_t* t, int, int32_t forced) {
    TrainStop(t, forced, 0x33e, &Car_SetCrashed, 1, 0x11, 0x114, (void(__fastcall*)(void*))&Obj_IncD0f, true);
}
RT3_REPLACE(0x4ad7a0, Train_Crash);

// 0x4ada00. Train t breaks down (lead car +0x342, steam, sound 2, state 0x12,
// string 0x115, owner tally +0xd0b).
RT3_NOINLINE void __fastcall Train_BreakDown(uint8_t* t, int, int32_t forced) {
    TrainStop(t, forced, 0x342, &Car_SetBrokenDown, 2, 0x12, 0x115, (void(__fastcall*)(void*))&Obj_IncD0b, false);
}
RT3_REPLACE(0x4ada00, Train_BreakDown);

// 0x4adc30. Sets train t up for its next stop: a train in the round house
// (+0x1b9) just switches to state 1; else it clears the leg state, picks the
// next stop (none: auto-adds cars in a world allowing it, +0x4af3), lets the
// stop's station learn the consist's demand, aims at the stop's building
// quarter (+0x181 / +0x185) and flags (+0x88..) which of the three cargo
// classes (0x62ba8c +0x8e..) the consist carries, then runs (state 1).
RT3_NOINLINE void __fastcall Train_SetupNextStop(uint8_t* t) {
    if (t[0x1b9]) {
        Train_SetState(t, 0, 1, 0.0f);
        return;
    }
    if (F<int32_t>(t, 0x41) != -1)
        if (void* r = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41))) Obj_Region(r);
    t[0x1bf] = 0;
    F<uint16_t>(t, 0x1c0) = 0;
    t[0x1c2] = 0;
    F<uint16_t>(t, 0x1c3) = 0;
    F<uint32_t>(t, 0xf0) = 0;
    uint8_t* stop = TrainOrders_NextStop(t, 0, 1, 1);
    if (!stop) {
        if (F<int32_t>(g_world, 0x4af3)) Train_AutoAddCars(t);
        return;
    }
    uint8_t* cur = (uint8_t*)Array33_Current(t);
    if (cur && (int8_t)cur[0x28] >= 0) {
        uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(cur, 0x29));
        Station_LearnDemand(st, 0, F<uint32_t>(stop, 0), cur);
        F<int32_t>(t, 0x181) = Obj_BuildingQuarterX(stop);
        F<int32_t>(t, 0x185) = Obj_BuildingQuarterY(stop);
        for (int32_t k = 0; k < 3; ++k) {
            uint8_t* flag = t + 0x88 + k;
            *flag = 0;
            Collection* classes = g_cargoTypes;
            void* cls = Collection_Get(classes, 0, F<uint32_t>(classes, 0x8e + k * 4));
            if (cur[0x28] & 0x40) {
                for (int32_t j = 0; j < cur[0]; ++j)
                    if (Cargo_MatchesClass(cls, 0, F<int32_t>(cur, 1 + j * 4))) *flag = 1;
            } else if (Cargo_MatchesClass(cls, 0, F<int32_t>(cur, 0x24))) {
                *flag = 1;
            }
        }
    }
    Train_SetState(t, 0, 1, 0.0f);
}
RT3_REPLACE(0x4adc30, Train_SetupNextStop);

// 0x4adde0. Train t arrives: cars of kinds 0 / 1 / 4 / 5 (their 0x388 entry
// +0x41) along its consist (+0x41, linked by +0x352) are detached, its
// delivery loads (+0x29 chain in 0x6ada90) at the station (+0xa0) are
// delivered (kept on the lot when the building's region has the station's
// key, else charged and dropped), AI trains pick their next order (unless
// 0x6cfcb8), then it sets up for its next stop.
RT3_NOINLINE void __fastcall Train_Arrive(uint8_t* t) {
    if (F<int32_t>(t, 0x41) != -1) {
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car;) {
            uint8_t* next = (uint8_t*)Obj_City352(car);
            if (F<int32_t>(Obj_Ref388(car), 0x41) == 1 || F<int32_t>(Obj_Ref388(car), 0x41) == 4 || F<int32_t>(Obj_Ref388(car), 0x41) == 5 ||
                !F<int32_t>(Obj_Ref388(car), 0x41))
                Train_DetachCar(t, 0, F<uint32_t>(car, 0x34a));
            car = next;
        }
    }
    uint8_t* st = nullptr;
    if (Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0xa0)))
        st = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(t, 0xa0));
    for (uint32_t id = F<uint32_t>(t, 0x29); id;) {
        uint8_t* load = (uint8_t*)Collection_Get(g_6ada90, 0, id);
        const uint32_t next = F<uint32_t>(load, 4);
        Load_Unlink((uint32_t*)load);
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint16_t>(load, 0xe));
        if (st) {
            if (Obj_HasKey(Obj_RegionAt(b), 0, F<uint16_t>(st, 0))) {
                Route_ChargeRandomBuilding(load, 0, st, 2);
                Collection* loads = g_6ada90;
                ((void(__thiscall*)(Collection*, uint32_t))(*(void***)loads)[2])(loads, id);
            } else {
                Route_ChargeRandomBuilding(load, 0, st, 1);
                Lot_AttachTo((uint32_t*)load, 0, F<uint32_t>(st, 4));
            }
        }
        id = next;
    }
    if (!g_6cfcb8) AiTrain_NextOrder(t);
    Train_SetupNextStop(t);
}
RT3_REPLACE(0x4adde0, Train_Arrive);

// 0x4adf30. Train t arrives at station `station` (+0xa0; arrivals counted in
// 0x6d3b58). A delivering train (+0x189) coming from another station (+0x9c)
// records the trip time (now - +0xb0) for each cargo it carries (+0x88..).
// Its cargo cars (car kind 1) give the load (+0x398) and payout: the stop
// time - load scaled (0x5c8568 / 0x5c8900 / 0x5cb3f4 / 0x5c8640), varied by
// SimRand(0x4b, 0x7d)% outside multiplayer, 0 when empty, cut by option
// +0x24f, scaled by the world's rate (+0x4ca2) and the region's effect 0x61 -
// is the wait (state 2). A delivery whistles (up to 10, the player's trains)
// and shows the money. Returns the payout.
float __fastcall Train_ArriveStation(uint8_t* t, int, uint32_t station) {
    ++g_6d3b58;
    F<uint32_t>(t, 0xa0) = station;
    if (t[0x189] && Collection_IsValid(g_stations, 0, station) &&
        Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0x9c)) && F<uint32_t>(t, 0xa0) != F<uint32_t>(t, 0x9c))
        for (int32_t k = 0; k < 3; ++k) {
            if (!t[0x88 + k]) continue;
            int32_t now = F<int32_t>(g_world, 0x15);
            uint8_t* from = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(t, 0x9c));
            Site_RecordMin(from, 0, F<int32_t>(t, 0xa0), k, (float)(now - F<int32_t>(t, 0xb0)));
        }
    float load = 0.0f, pay = 0.0f;
    if (F<int32_t>(t, 0x41) != -1)
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car; car = (uint8_t*)Obj_City352(car)) {
            if (F<int32_t>(Obj_Ref388(car), 0x41) != 1) continue;
            load = (float)(F<float>(car, 0x398) + (double)load);
            pay = (float)(Load_PayOut(car) + (double)pay);
        }
    int32_t wait = Ftol_207((((double)load * Fl_207(0x5c8568)) + Fl_207(0x5c8900)) * Fl_207(0x5cb3f4) * D(0x5c8640));
    if (!IsMultiplayer()) {
        double r = (double)SimRand(0x4b, 0x7d) / D(0x5c85d8);
        wait = Ftol_207(r * (double)wait);
    }
    if (load == g_5c8598) wait = 0;
    if (F<int32_t>(g_app, 0x24f)) wait = Ftol_207((double)wait * D(0x5c9ba0));
    wait = Ftol_207((double)(float)wait * F<float>(g_world, 0x4ca2));
    uint32_t region = 0;
    if (F<int32_t>(t, 0x41) != -1)
        if (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41))) {
            float cy = (float)Obj_CellY(car);
            float cx = (float)Obj_CellX(car);
            region = Map_GridByte(hook::As<void*>(g_map), 0, cx, cy);
        }
    uint32_t player = F<uint32_t>(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0x3b);
    float f = Effects_Multiplier(g_world, 0, 0x61, F<uint32_t>(t, 0x51), player, region);
    wait = Ftol_207((double)f * (float)wait);
    Train_SetState(t, 0, 2, (float)wait);
    if (!t[0x189]) return pay;
    int32_t whistles = Ftol_207(pay / D(0x5c8d08) + D(0x5c8628));
    int32_t money = whistles;
    if (!whistles && pay > Fl_207(0x5c85b4)) whistles = 1;
    if (!money && pay > Fl_207(0x5c85b4)) money = 1;
    if (whistles && F<int32_t>(t, 0x51) == Obj_Get21(hook::As<void*>(g_world))) Train_Whistle(t, 0, whistles > 10 ? 10 : whistles);
    if (money) Station_ShowCargoMoney(t, 0, money);
    return pay;
}
RT3_REPLACE(0x4adf30, Train_ArriveStation);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4ae250. Collapse redundant stops, enter the appropriate station state,
// settle arrival income and emit the player's delivery message.
RT3_NOINLINE int32_t __fastcall Train_ProcessStop(uint8_t* t, int, int32_t force) {
    auto current = [](uint8_t* o) { return (uint8_t*)Array33_Current(o); };
    uint8_t* row = current(t);
    float fare = 0.0f;
    if (IsMultiplayer()) {
        if (Net_AllHaveVersion(1.02f))
            Train_ReanchorConsist(t);
        else if (F_train_station<int32_t>(t, 0x41) != -1 && Get(0x62ba84, F_train_station<uint32_t>(t, 0x41))) {
            uint8_t* car = F_train_station<int32_t>(t, 0x41) == -1 ? nullptr : Get(0x62ba84, F_train_station<uint32_t>(t, 0x41));
            uint32_t position = F_train_station<uint32_t>(car, 0x35e), node = F_train_station<uint32_t>(car, 0x356);
            Car_SetPiece(car, 0, (int32_t)node, (int32_t)position);
            Car_Advance(car, 0, 0, nullptr, nullptr, -1);
        }
    }
    if (F_train_station<int32_t>(t, 4) > 1) {
        uint32_t first = Location(F_train_station<uint8_t*>(t, 0));
        bool different = false;
        for (int32_t i = 1; i < F_train_station<int32_t>(t, 4); ++i)
            if (Location(F_train_station<uint8_t*>(t, 0) + i * 0x33) != first) different = true;
        if (!different) {
            while (F_train_station<int32_t>(t, 4) > 1)
                TrainOrders_Remove(t, 0, 1);
            row = current(t);
        }
    }
    if (((int8_t)row[0x28] < 0 && !force) || (!Valid(F_train_station<uint16_t>(row, 0x29)) && !force)) {
        Next_train_station(t);
        return 1;
    }
    uint32_t id = F_train_station<uint32_t>(t, 0xac);
    uint8_t* site = nullptr;
    if (Valid(F_train_station<uint16_t>(row, 0x29))) {
        site = Get(0x6cec20, F_train_station<uint16_t>(row, 0x29));
        Obj_SetHighlight(site, 0, 1);
    }
    uint32_t other = F_train_station<uint32_t>(t, 0xac);
    if (other != F_train_station<uint16_t>(row, 0x29) && other != 0xffffffff && Valid(other)) {
        id = F_train_station<uint32_t>(t, 0xac);
        site = Get(0x6cec20, id);
    }
    bool same = id == F_train_station<uint16_t>(row, 0x29);
    int32_t held;
    if (Kind(site) == 3) {
        if (F_train_station<int32_t>(t, 0x94) == -1) goto finishSpecial;
        if (F_train_station<float>(t, 0xe0) <= Fl_train_station(0x5d0098) ||
            (Obj_CityRef10(t) && F_train_station<float>(t, 0xe4) <= Fl_train_station(0x5d0098))) {
            if (same) {
                Train_LeaveSite(t);
                F_train_station<uint32_t>(t, 0xa4) = id;
                Train_EnterSite(t);
            }
            uint32_t head = F_train_station<uint32_t>(t, 0x41);
            uint8_t* car = head == 0xffffffff ? nullptr : Get(0x62ba84, head);
            Train_SetState3(t, 0, car, site, same);
            return 0;
        }
        held = F_train_station<int32_t>(t, 0x94);
        F_train_station<uint32_t>(t, 0xac) = F_train_station<uint32_t>(t, 0xa4) = F_train_station<uint32_t>(t, 0x94) = 0xffffffff;
    } else if (Kind(site) == 4) {
        held = F_train_station<int32_t>(t, 0x94);
        if (held == -1) goto finishSpecial;
        if (F_train_station<float>(t, 0xdc) <= Fl_train_station(0x5d0098)) {
            uint32_t head = F_train_station<uint32_t>(t, 0x41);
            uint8_t* car = head == 0xffffffff ? nullptr : Get(0x62ba84, head);
            Train_SetState5(t, 0, (void*)(car), site, same);
            return 0;
        }
        F_train_station<uint32_t>(t, 0xac) = F_train_station<uint32_t>(t, 0xa8) = F_train_station<uint32_t>(t, 0x94) = 0xffffffff;
    } else {
        if (!t[0x1a1]) {
            uint8_t* company = Get(0x62be10, F_train_station<uint32_t>(t, 0x51));
            if (F_train_station<float>(company, 0x347) < F_train_station<float>(t, 0x199) && t[0xfb] &&
                !F_train_station<int32_t>(hook::As<void*>(g_world), 0x4aeb)) {
                company = Get(0x62be10, F_train_station<uint32_t>(t, 0x51));
                uint32_t old = F_train_station<uint32_t>(company, 0x347);
                float delta;
                __asm {
                    mov eax, t
                    fld dword ptr [eax + 0x199]
                    fsub dword ptr old
                    fstp delta
                }
                float profit = *(float*)&old;
                if ((profit < Fl_train_station(0x5c8c28) && delta > Fl_train_station(0x5c8554)) || (profit >= Fl_train_station(0x5c8c28) && delta > Fl_train_station(0x5c85f8))) {
                    Train_DefaultName(t);
                    company = Get(0x62be10, F_train_station<uint32_t>(t, 0x51));
                    if (company == Obj_Company21(hook::As<void*>(g_world))) {
                        int32_t amount;
                        __asm {
                            mov eax, t
                            fld dword ptr [eax + 0x199]
                            call Rt3_Ftol
                            mov amount, eax
                        }
                        World_AddNamedEntry(hook::As<void*>(g_world), 0, (const char*)0x5c87a8, 3, 1, (int32_t)(F_train_station<uint32_t>(t, 0x25)), amount, -1, -1);
                    }
                    company = Get(0x62be10, F_train_station<uint32_t>(t, 0x51));
                    F_train_station<uint32_t>(company, 0x347) = F_train_station<uint32_t>(t, 0x199);
                }
            }
        }
        F_train_station<uint32_t>(t, 0x199) = F_train_station<uint32_t>(t, 0x19d) = 0;
        if (Obj_BuildingStateActive((void*)(site))) {
            F_train_station<uint32_t>(t, 0xcc) = 0;
            Train_ResetCarAnimations(t);
            fare = Train_ArriveStation(t, 0, F_train_station<uint32_t>(site, 0));
        } else if (id == F_train_station<uint16_t>(row, 0x29)) Next_train_station(t);
        if (g_newGame[0x82] || F_train_station<int32_t>(hook::As<void*>(g_world), 0x4aef)) fare = 0.0f;
        uint32_t owner = F_train_station<uint32_t>(t, 0x51);
        uint32_t player = ((uint32_t)Obj_Get21(hook::As<void*>(g_world)));
        if (owner == player && t[0x189] && Obj_BuildingStateActive((void*)(site))) {
            char message[200], line[200], money[200], arrival[200];
            message[0] = 0;
            if (Str_IsNumber(Name(t))) strcat(message, Text(0xf2a));
            const char* stationName = (const char*)site + 0x46b;
            const char* trainName = Name(t);
            const char* fmt = Text(0x117);
            Format(arrival, fmt, trainName, stationName);
            strcat(message, arrival);
            if (fare > Fl_train_station(0x5c85b4)) {
                int32_t total = 0;
                uint32_t n = t[0x1b8];
                for (uint32_t i = 0; i < n; ++i) total += F_train_station<uint16_t>(t, 0x1b2 + 2 * i);
                uint32_t denominator = F_train_station<uint32_t>(t, 0x39);
                if (F_train_station<float>(t, 0x39) < Fl_train_station(0x5c85b4)) denominator = 0x3f800000;
                float ratio;
                double net;
                // The native code rounds the fraction to float, but keeps
                // the subtraction/product extended until the double argument.
                __asm {
                    fild total
                    fstp ratio
                    fld ratio
                    fdiv dword ptr denominator
                    fstp ratio
                    fld ratio
                    mov eax, 0x5c8770
                    fsubr qword ptr [eax]
                    fmul fare
                    fstp net
                }
                Num_FormatSimple2(money, 11, net, 11);
                Format(line, (const char*)0x5d0090, money);
                float tax;
                __asm {
                    fld ratio
                    fmul fare
                    fstp tax
                }
                strcat(message, line);
                if (tax >= Fl_train_station(0x5c8c28)) {
                    uint32_t bits = tax < Fl_train_station(0x5d008c) ? 0x43fa8000 : *(uint32_t*)&tax;
                    double value;
                    __asm {
                        fld dword ptr bits
                        fstp value
                    }
                    Num_FormatSimple2(money, 11, value, 11);
                    Format(line, Text(0xf84), money);
                    strcat(message, line);
                }
            }
            Log_Add((Collection*)(hook::As<void*>(g_messageLog)), 0, message, 0, (int32_t)(F_train_station<uint32_t>(t, 0x25)), 0);
        }
        t[0x189] = 1;
        F_train_station<uint32_t>(t, 0x8c) = 0xbf800000;
        F_train_station<uint32_t>(t, 0x1ba) = F_train_station<uint32_t>(t, 0x94);
        F_train_station<uint32_t>(t, 0x94) = 0xffffffff;
        Train_ResetCarAnimations(t);
        return 1;
    }
    F_train_station<uint32_t>(t, 0x8c) = 0xbf800000;
    F_train_station<int32_t>(t, 0x1ba) = held;
finishSpecial:
    if (same) {
        Next_train_station(t);
        Train_SetState(t, 0, 0, 0.0f);
    }
    return 0;
}
RT3_REPLACE(0x4ae250, Train_ProcessStop);

// 0x4aead0. Yearly train upkeep (unless sandbox): the maintenance expense grows with
// age squared, the engine's base cost (+0xc) plus the regional bonuses and the
// distance run (+0x61); the condition charge (x3 when worn below k); then blends the
// reliability (+0x3d) towards the engine's (+0xcc) by the time since the last
// service (+0xb0) and resets the counters (+0x61/+0x65).
void __fastcall Train_YearlyUpkeep(uint8_t* t) {
    if (F<int32_t>(World(), 0x46c38)) return;
    uint8_t* e = Engine(t);
    if (!e) return;
    uint8_t* et = EngineType(e);
    if (!et) return;
    int32_t age = F<uint16_t>(World(), 0xd) - F<int32_t>(t, 0xd4);
    float agef = (float)((double)(age * 4 + 0x64) / D(0x5c85d8));
    agef = (float)((double)agef * agef);
    uint32_t pl1 = F<uint32_t>(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0x3b);
    uint32_t pl2 = F<uint32_t>(Collection_Get(Players(), 0, F<uint32_t>(t, 0x51)), 0x3b);
    uint8_t* e2 = Engine(t);
    uint8_t* et2 = e2 ? EngineType(e2) : nullptr;
    void* w = hook::As<void*>(g_world);
    uint32_t r = Obj_CityGridByte(t);
    int32_t a = Obj_CargoBonus(w, 0, F<int32_t>(et2, 0x10) + 0x4a, F<uint32_t>(t, 0x51), pl2, r);
    r = Obj_CityGridByte(t);
    a *= Obj_CargoBonus(w, 0, 0x49, F<uint32_t>(t, 0x51), pl1, r);
    float c = (float)(F<int32_t>(et, 0xc) + a);
    if (c < Fl(0x5c8598)) c = 0.5f;
    float g = (float)((double)(age + 0x64) / D(0x5c85d8));
    g = (float)((double)g * g);
    float v = (float)-((double)g * F<float>(t, 0x61) * c * D(0x5d00a0));
    Train_AddExpense(t, 0, v);
    float m = F<float>(t, 0xdc) <= D(0x5c85b8) ? 3.0f : 1.0f;
    Train_EarnRevenue(t, 0, (float)(-((double)m * F<float>(et, 8) * agef) / Fl(0x5d009c)));
    int32_t dt = F<int32_t>(World(), 0x15) - F<int32_t>(t, 0xb0);
    if (dt > 0) {
        int32_t lo = dt < 0x2760 ? dt : 0x2760;
        int32_t hi = dt - 0x2760 < 0 ? 0 : dt - 0x2760;
        int32_t n = hi + lo >= 1 ? hi + lo : 1;
        double s = (double)(float)lo * F<float>(t, 0xcc) + (double)(float)hi * F<float>(t, 0x3d);
        F<float>(t, 0x3d) = (float)(s / (float)n);
    }
    F<int32_t>(t, 0x61) = 0;
    F<int32_t>(t, 0x65) = 0;
}
RT3_REPLACE(0x4aead0, Train_YearlyUpkeep);

// 0x4aed60. One loading step of train t at its station over dt: a train in
// the round house stops (marking its next stop's city +0x33a). Else, with a
// next stop in another region, it loads the consist at the stop's station
// (stamping its visit +0x5c5) spending the loading time +0xf0; finished
// (or nothing to load), it adds cars, stops, marks its city, advances its
// orders and, for a busy station, slows its city's sound channel (+0x336
// rate above 8400). Returns the time used (all of dt while loading, 0 when
// it cannot load).
RT3_NOINLINE float __fastcall Train_StepAtStation(uint8_t* t, int, float dt) {
    if (t[0x1b9]) {
        Train_SetState(t, 0, 0, 0.0f);
        if (TrainOrders_NextStop(t, 0, 1, 0)) {
            uint8_t* r = F<int32_t>(t, 0x41) != -1 ? (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)) : nullptr;
            F<int32_t>(r, 0x33a) = 1;
        }
        return Fl_207(0x5c8598);
    }
    uint8_t* stop = TrainOrders_NextStop(t, 0, 1, 1);
    if (!stop) return Fl_207(0x5c8598);
    uint8_t* here = nullptr;
    if (F<int32_t>(t, 0x41) != -1)
        if (void* r = Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41))) here = Obj_Region(r);
    uint8_t* there = Obj_BuildingRegion(stop);
    uint8_t* cur = (uint8_t*)Array33_Current(t);
    if (here == there || (int8_t)cur[0x28] < 0) {
        Train_SetState(t, 0, 0, 0.0f);
        return Fl_207(0x5c8598);
    }
    uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(cur, 0x29));
    F<uint32_t>(st, 0x5c5) = F<uint32_t>(g_world, 0x15);
    if (Obj_BuildingRegion(st) == there) {
        Train_SetState(t, 0, 0, 0.0f);
        return Fl_207(0x5c8598);
    }
    const int32_t loaded = Train_LoadConsist(t, 0, st, (uint32_t*)stop);
    float used;
    if (dt > F<float>(t, 0xf0)) {
        used = F<float>(t, 0xf0);
        F<uint32_t>(t, 0xf0) = 0;
    } else {
        used = dt;
        F<float>(t, 0xf0) = (float)((double)F<float>(t, 0xf0) - dt);
    }
    if (F<float>(t, 0xf0) < Fl_207(0x5c8598)) F<uint32_t>(t, 0xf0) = 0;
    if (used >= dt) used = dt;
    if (!loaded || !(F<float>(t, 0xf0) <= Fl_207(0x5c85b4))) return dt;
    Train_AutoAddCars(t);
    Train_SetState(t, 0, 0, 0.0f);
    F<int32_t>(Obj_City41(t), 0x33a) = 1;
    TrainOrders_SetCurrent(t, 0, (F<int32_t>(t, 8) + 1) % F<int32_t>(t, 4), 0);
    uint8_t* city = (uint8_t*)Obj_City41(t);
    if (t[0x189] && Obj_BuildingStateActive(st) && city) {
        if (uint8_t* ch = F<uint8_t*>(city, 0x336)) {
            const int32_t rate = F<int32_t>(ch, 0x12c);
            if (rate > 0x20d0) {
                const float over = (float)(rate - 0x1f40);
                Channel_SetRate(ch, 0, rate - Ftol_207((double)over * Fl_207(0x5ce500)));
            }
        }
    }
    return used;
}
RT3_REPLACE(0x4aed60, Train_StepAtStation);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4aefb0. Replace the locomotive, charge/refund its value, replace/insert/drop
// a tender, and rebuild the consist. Keep raw pose bits and fresh lookups across
// callbacks. Ghidra's stack accounting is wrong here: the final flags are the
// old/new engine-kind booleans, and the follow flag is the initial selection.
RT3_NOINLINE void __fastcall Train_ReplaceEngine(uint8_t* train, int, uint32_t typeId, int32_t freeUpgrade) {
    uint32_t head = F<uint32_t>(train, 0x41);
    if (head == 0xffffffff) return;
    bool selected = false;
    uint8_t* world = g_world;
    if (F<int32_t>(world, 0x45) == 1) {
        uint32_t selectedId = F<uint32_t>(world, 0x49);
        selected = selectedId == F<uint32_t>(Record(0x62ba84, head), 0x34a);
    }
    uint8_t* old = Record(0x62ba84, F<uint32_t>(train, 0x41));
    uint32_t node = F<uint32_t>(old, 0x356);
    Train_PassSignal(train);
    uint8_t* following = F<int32_t>(old, 0x352) == -1 ? nullptr : Record(0x62ba84, F<uint32_t>(old, 0x352));
    if (!F<uint8_t>(hook::As<void*>(g_newGame), 0x82) && !freeUpgrade) {
        uint32_t player = F<uint32_t>(Record(0x62be10, F<uint32_t>(train, 0x51)), 0x3b);
        uint8_t* definition = Record(0x6ada84, typeId);
        float cost = Price(definition, F<uint32_t>(train, 0x51), player);
        Stat(train, 0x10, cost);
        Stat(train, 0xd, -cost);
        player = F<uint32_t>(Record(0x62be10, F<uint32_t>(train, 0x51)), 0x3b);
        uint32_t company = F<uint32_t>(train, 0x51);
        definition = ((uint8_t*)Obj_CityRef((void*)(train)));
        float refund = -Price(definition, company, player);
        Stat(train, 0x10, refund);
        uint8_t* co = Record(0x62be10, F<uint32_t>(train, 0x51));
        Company_PostStat((void*)(co), 0, 0xc, refund, 0);
    }
    float x = CellX(old), y = CellY(old);
    uint8_t* definition = Record(0x6ada84, typeId);
    uint8_t* type = Asset(F<uint32_t>(definition, 0x72));
    uint32_t id = AddCar(type, x, y);
    uint8_t* lead = Record(0x62ba84, id);
    F<uint32_t>(lead, 0x35a) = F<uint32_t>(old, 0x35a);
    F<uint32_t>(lead, 0x35e) = F<uint32_t>(old, 0x35e);
    bool newElectric = F<int32_t>(definition, 0x10) == 2;
    uint8_t* oldDefinition = Record(0x6ada84, F<uint32_t>(Ref(old), 0x126));
    bool oldElectric = F<int32_t>(oldDefinition, 0x10) == 2;
    F<uint32_t>(train, 0x41) = id;
    F<uint32_t>(lead, 0x352) = F<uint32_t>(old, 0x352);
    if (following) F<uint32_t>(following, 0x34e) = id;
    else F<uint32_t>(train, 0x45) = id;
    uint8_t* ref = Ref(old);
    F<float>(train, 0xf5) = F<float>(train, 0xf5) - F<float>(ref, 0xfd);
    DeleteCar(old);
    type = Asset(F<uint32_t>(definition, 0x72));
    F<float>(train, 0xf5) = F<float>(type, 0xfd) + F<float>(train, 0xf5);
    uint32_t stamp = F<uint16_t>(hook::As<void*>(g_world), 0xd);
    F<uint32_t>(train, 0xdc) = F<uint32_t>(train, 0xe0) = F<uint32_t>(train, 0xe4) = 0x3f800000;
    F<uint32_t>(train, 0xd4) = stamp;
    F<uint32_t>(lead, 0x390) = F<uint32_t>(train, 0x25);
    Car_SetPiece(lead, 0, (int32_t)node, 0x3e395810);
    RefreshPose(lead);
    uint8_t* afterFollowing = following ? Next(following) : nullptr;
    uint8_t* tenderType = Asset(F<uint32_t>(definition, 0x76));
    bool oldTender = following && F<int32_t>(Ref(following), 0x41) == 3;
    if (oldTender && !tenderType) {
        F<uint32_t>(lead, 0x352) = F<uint32_t>(following, 0x352);
        if (afterFollowing) F<uint32_t>(afterFollowing, 0x34e) = F<uint32_t>(following, 0x34e);
        else F<uint32_t>(train, 0x45) = F<uint32_t>(train, 0x41);
        ref = Ref(following);
        F<float>(train, 0xf5) = F<float>(train, 0xf5) - F<float>(ref, 0xfd);
        --train[0xf9];
        DeleteCar(following);
    } else if (tenderType) {
        uint8_t* basis = oldTender ? following : lead;
        x = CellX(basis); y = CellY(basis);
        id = AddCar(tenderType, x, y);
        uint8_t* tender = Record(0x62ba84, id);
        F<uint32_t>(tender, 0x390) = F<uint32_t>(train, 0x25);
        ref = Ref(tender);
        F<float>(train, 0xf5) = F<float>(ref, 0xfd) + F<float>(train, 0xf5);
        ++train[0xf9];
        F<uint32_t>(lead, 0x352) = id;
        if (oldTender) {
            if (afterFollowing) F<uint32_t>(afterFollowing, 0x34e) = id;
            else F<uint32_t>(train, 0x45) = id;
            F<uint32_t>(tender, 0x34e) = F<uint32_t>(following, 0x34e);
            F<uint32_t>(tender, 0x352) = F<uint32_t>(following, 0x352);
            ref = Ref(following);
            F<float>(train, 0xf5) = F<float>(train, 0xf5) - F<float>(ref, 0xfd);
            --train[0xf9];
            DeleteCar(following);
        } else {
            if (following) F<uint32_t>(following, 0x34e) = id;
            F<uint32_t>(tender, 0x34e) = F<uint32_t>(lead, 0x34a);
            F<int32_t>(tender, 0x352) = following ? F<int32_t>(following, 0x34a) : -1;
            if (F<uint32_t>(train, 0x41) == F<uint32_t>(train, 0x45)) F<uint32_t>(train, 0x45) = id;
        }
        F<uint32_t>(tender, 0x35a) = F<uint32_t>(lead, 0x35a);
        RefreshPose(tender);
    }
    for (uint8_t* car = Next(lead); car; car = Next(car))
        Car_FollowLeader(car, 0, nullptr);
    if (selected) {
        uint8_t* car = F<int32_t>(train, 0x41) == -1 ? nullptr : Record(0x62ba84, F<uint32_t>(train, 0x41));
        uint32_t carId = F<uint32_t>(car, 0x34a);
        World_FollowEntity((uint8_t*)(hook::As<void*>(g_world)), 0, 1, 1, (int32_t)(carId));
    }
    if (F<int32_t>(train, 0xe8) != 1 && F<int32_t>(train, 0xe8) != 2 && oldElectric != newElectric)
        Train_Route(train);
    ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))&Train_UpdateEngineEffects)(train, 0, 0, F<uint32_t>(train, 0xcc));
    void* owner = hook::As<void*>(g_62be68);
    void** vtable = F<void**>(owner, 0);
    ((void(__fastcall*)(void*))vtable[0])(owner);
    Train_SetGhost(train, 0, (int32_t)(train[0xf4]), 1);
}
RT3_REPLACE(0x4aefb0, Train_ReplaceEngine);

// 0x4af610. Train t: removes (from the back) every order for station
// `station` (+0x29 of a non-node entry). With orders left it re-routes when
// one went; without, it stops and ghosts as Train_DropWaypoint does (state 8).
RT3_NOINLINE void __fastcall Train_DropStation(uint8_t* t, int, int32_t station) {
    int32_t removed = 0;
    for (int32_t i = F<int32_t>(t, 4) - 1; i >= 0; --i)
        if ((int8_t)((uint8_t*)Array33_At(t, 0, i))[0x28] >= 0 && F<uint16_t>(Array33_At(t, 0, i), 0x29) == station) {
            TrainOrders_Remove(t, 0, i);
            removed = 1;
        }
    if (F<int32_t>(t, 4)) {
        if (removed) Train_Route(t);
        return;
    }
    F<int32_t>(t, 0x55) = 0;
    F<int32_t>(t, 0x1a7) = 0;
    F<int32_t>(t, 0x59) = 0;
    F<int32_t>(t, 0x5d) = 0;
    uint8_t old = t[0xf4];
    if (old != 1) {
        t[0xf4] = 1;
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car;) {
            SceneObj_SetAlpha(car, 0, 0x3f, 1);
            Model_SetOpacity(car, 0, (float)(t[0xf4] ? 0.25 : 1.0));
            int32_t next = F<int32_t>(car, 0x352);
            if (next == -1) break;
            car = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)next);
        }
        if (old != t[0xf4]) Station_UnlinkNodes(t);
    }
    F<int32_t>(t, 0xe8) = 8;
    F<int32_t>(t, 0xec) = 0;
}
RT3_REPLACE(0x4af610, Train_DropStation);

// 0x4af760. Train t: removes (from the back) every waypoint order - a track
// node entry, flag +0x28 bit 7 - for node `node` (+0x2b). With orders left it
// re-routes; without, it stops and ghosts as Train_StopAndGhost but in state 8.
// (The original asks IsMultiplayer twice and ignores the answer.)
RT3_NOINLINE void __fastcall Train_DropWaypoint(uint8_t* t, int, int32_t node) {
    for (int32_t i = F<int32_t>(t, 4) - 1; i >= 0; --i)
        if ((int8_t)((uint8_t*)Array33_At(t, 0, i))[0x28] < 0 && F<int32_t>(Array33_At(t, 0, i), 0x2b) == node) TrainOrders_Remove(t, 0, i);
    if (F<int32_t>(t, 4)) {
        Train_Route(t);
        return;
    }
    F<int32_t>(t, 0x55) = 0;
    F<int32_t>(t, 0x1a7) = 0;
    F<int32_t>(t, 0x59) = 0;
    F<int32_t>(t, 0x5d) = 0;
    uint8_t old = t[0xf4];
    if (old != 1) {
        t[0xf4] = 1;
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car;) {
            SceneObj_SetAlpha(car, 0, 0x3f, 1);
            Model_SetOpacity(car, 0, (float)(t[0xf4] ? 0.25 : 1.0));
            int32_t next = F<int32_t>(car, 0x352);
            if (next == -1) break;
            car = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)next);
        }
        if (old != t[0xf4]) Station_UnlinkNodes(t);
    }
    F<int32_t>(t, 0xe8) = 8;
    F<int32_t>(t, 0xec) = 0;
}
RT3_REPLACE(0x4af760, Train_DropWaypoint);

// 0x4af8a0. A random mishap for train t (none with +0x4b03): a crash with a
// chance per mille of the year's base (200 before 1920, 170, 140 from 1970;
// scaled down for crowded maps, 0x6cfcbc) x 0x5c8680, never for a train
// built within 3 years (+0xd4), else a breakdown; only breakdowns with
// +0x4aff or in multiplayer.
RT3_NOINLINE void __fastcall Train_RandomMishap(uint8_t* t) {
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x4b03)) return;
    uint16_t year = F<uint16_t>(w, 0xd);
    int32_t base = year >= 0x7b2 ? 0x8c : (year < 0x780 ? 0xc8 : 0xaa);
    int32_t n = Collection_Count(g_trains);
    uint32_t k = n > 0x2bc ? 0x5c8988 : n > 0x1f4 ? 0x5c9c50 : n > 0xfa ? 0x5c88c8 : n > 0x50 ? 0x5c8628 : n > 0x28 ? 0x5c8680 : n > 0x14 ? 0x5c85a0 : 0;
    if (k) base = (int32_t)((double)base * *(const double*)k);
    int32_t chance = (int32_t)((double)base * *(const double*)0x5c8680);
    if ((int32_t)F<uint16_t>(g_world, 0xd) <= F<int32_t>(t, 0xd4) + 3) chance = -1;
    if (F<int32_t>(g_world, 0x4aff) || IsMultiplayer()) {
        Train_BreakDown(t, 0, 0);
        return;
    }
    if (SimRand(0, 1000) < chance)
        Train_Crash(t, 0, 0);
    else
        Train_BreakDown(t, 0, 0);
}
RT3_REPLACE(0x4af8a0, Train_RandomMishap);

 // namespace

// 0x4afa10. Train t's periodic wear: its three meters (+0xe0 unless the
// engine type's +0x10, +0xdc, +0xe4 with the lead car's tilt) run down by
// the owner's effect multipliers 100 / 98 / 99 (faster below 0x5c8628 for
// owners without +0x1e1 from game version 0x67); every 40 calls a mishap
// may strike, at a chance from the running cost (less on others' track,
// none on friendly track, for new trains or while stopped / wrecked).
RT3_NOINLINE void __fastcall Train_TickWear(uint8_t* t) {
    bool harsh = false;
    if (Game_EffectiveVersion() >= 0x67 && !Company_OwnerFlag1e1(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51))))
        harsh = true;
    uint8_t* tp = (uint8_t*)Obj_Ref388(F<int32_t>(t, 0x41) == -1 ? nullptr : Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)));
    uint8_t* eng = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>(tp, 0x126));
    const float zero = *(const float*)0x5c8598;
    if (!F<int32_t>(eng, 0x10) && F<float>(t, 0xe0) > zero) {
        uint32_t player = F<uint32_t>((uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0x3b);
        uint32_t grid = Obj_CityGridByte(t);
        float by = (float)((double)Effects_Multiplier(g_world, 0, 100, F<uint32_t>(t, 0x51), player, grid) * *(const float*)0x5d00b4);
        TrainWear(&F<float>(t, 0xe0), by, harsh);
    }
    if (F<float>(t, 0xdc) > zero) {
        uint32_t region = TrainRegion(t);
        uint32_t player = F<uint32_t>((uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0x3b);
        float by = (float)((double)Effects_Multiplier(g_world, 0, 0x62, F<uint32_t>(t, 0x51), player, region) * *(const float*)0x5d00b0);
        TrainWear(&F<float>(t, 0xdc), by, harsh);
    }
    if (F<float>(t, 0xe4) > zero) {
        float tilt;
        Model_GetAngles(F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)), 0,
                        (uint32_t*)&tilt, nullptr, nullptr);
        tilt = (float)crt::fabs(tilt);
        uint32_t region = TrainRegion(t);
        uint32_t player = F<uint32_t>((uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0x3b);
        float mult = Effects_Multiplier(g_world, 0, 0x63, F<uint32_t>(t, 0x51), player, region);
        float by = (float)((double)tilt * *(const double*)0x5d00a8 * mult);
        by = (float)((double)by * *(const double*)0x5c87f0);
        TrainWear(&F<float>(t, 0xe4), by, harsh);
    }
    if (++t[0x1c5] < 0x28) return;
    t[0x1c5] = 0;
    float chance = Train_RunningCost(t);
    if (F<int32_t>(t, 0x41) != -1 && Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41))) {
        uint8_t* lead = F<int32_t>(t, 0x41) == -1 ? nullptr : (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
        uint32_t track = F<uint32_t>(lead, 0x356);
        if (Collection_IsValid(g_tracks, 0, track)) {
            uint32_t owner = F<uint16_t>((uint8_t*)Collection_Get(g_tracks, 0, track), 0x22e);
            if (owner != F<uint32_t>(t, 0x51)) {
                if (Collection_IsValid(g_companies, 0, owner) &&
                    Company_OwnerFlag1e1(Collection_Get(g_companies, 0, owner)))
                    chance = 0.0f;
                else
                    chance = (float)((double)chance * *(const double*)0x5c8628);
            }
        }
    }
    chance = (float)((double)chance * *(const float*)0x5c8d4c);
    uint8_t* w = g_world;
    int32_t year = F<uint16_t>(w, 0xd);
    if (year <= F<int32_t>(t, 0xd4) || (year == F<int32_t>(t, 0xd4) + 1 && w[0xf] <= 6)) chance = 0.0f;
    int32_t st = F<int32_t>(t, 0xe8);
    if (st == 0x12 || st == 0x11 || st == 0xb || st == 0xc || st == 0xd) chance = 0.0f;
    chance = (float)((double)chance * *(const double*)0x5c85a0);
    if (!(chance > zero)) return;
    float r = (float)SimRand(0, 30000);
    if (!((double)chance * *(const float*)0x5c8950 >= r)) return;
    Train_RandomMishap(t);
}
RT3_REPLACE(0x4afa10, Train_TickWear);

// 0x4afeb0. Train t: finds the track nodes under its two ends - the lead car
// (+0x41) half a car length (Obj_Ref388 +0x10d) on from its position (+0x356
// node, +0x35e pos, +0x35a direction) into +0x49, the last car (+0x45, unless
// off track or its vtbl[5] says so) back into +0x4d. When the lead entered a
// new node: wear ticks, and a train in its way (Train_NextNode) makes it wait
// for it (+0x98, +0xcc = 0) if that one is in a special state, else one of
// them stops - this one when the switch was set against it, else per
// Trains_ResolveClash; QUIRK: on its own id the old +0x4d decides whether to
// go on. The nodes' train links move along (-2 lead, -3 tail).
RT3_NOINLINE void __fastcall Train_Advance(uint8_t* t) {
    Collection* nodes = g_tracks;
    int32_t oldA = F<int32_t>(t, 0x49);
    int32_t oldB = F<int32_t>(t, 0x4d);
    uint8_t* lead = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41));
    uint8_t* last = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x45));
    float hA = (float)(F<float>(Obj_Ref388(lead), 0x10d) / Fl_207(0x5c878c));
    float hB = (float)(F<float>(Obj_Ref388(last), 0x10d) / Fl_207(0x5c878c));
    uint8_t* nA = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(lead, 0x356));
    uint8_t* nB = nullptr;
    if (F<int32_t>(last, 0x356) != -1 && !(*(int32_t(__thiscall**)(uint8_t*))(F<uint8_t*>(last, 0) + 0x14))(last))
        nB = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(last, 0x356));
    float x, z, pos;
    int32_t dir = F<int32_t>(lead, 0x35a);
    int32_t a = Node_Advance(nA, 0, F<float>(lead, 0x35e), hA, &x, &z, &pos, &dir, nullptr, nullptr, F<int32_t>(t, 0x41), -1, 1);
    F<int32_t>(t, 0x49) = a;
    if (a == -1) return;
    if (a != oldA) {
        Train_TickWear(t);
        int32_t sw = oldA;
        int32_t other = Train_NextNode(t, 0, dir, (uint32_t)oldA, F<uint32_t>(t, 0x49), &sw);
        if (other != -1) {
            if (Train_StateSpecial((uint8_t*)Collection_Get(g_trains, 0, (uint32_t)other)) && other != F<int32_t>(t, 0x25)) {
                F<int32_t>(t, 0x98) = other;
                F<int32_t>(t, 0xcc) = 0;
                return;
            }
            int32_t go;
            if (other != F<int32_t>(t, 0x25)) {
                if (sw) {
                    Train_StopAndGhost(t);
                    return;
                }
                go = Trains_ResolveClash(g_trains, 0, F<uint32_t>(t, 0x25), (uint32_t)other);
            } else {
                go = oldB;
            }
            if (!go) return;
        }
        if (oldA != -1) Node_ClearLinks(Collection_Get(nodes, 0, (uint32_t)oldA), 0, F<int32_t>(t, 0x25), -2);
        if (F<int32_t>(t, 0x49) != -1) Node_SetLinkPair(Collection_Get(nodes, 0, F<uint32_t>(t, 0x49)), 0, F<int32_t>(t, 0x25), -2, dir);
    }
    if (!nB) return;
    dir = F<int32_t>(last, 0x35a) == 0;
    F<int32_t>(t, 0x4d) = Node_Advance(nB, 0, F<float>(last, 0x35e), hB, &x, &z, &pos, &dir, nullptr, nullptr, -1, -1, 1);
    if (oldB != -1 && Collection_IsValid(nodes, 0, (uint32_t)oldB)) Node_ClearLinks(Collection_Get(nodes, 0, (uint32_t)oldB), 0, F<int32_t>(t, 0x25), -3);
    if (F<int32_t>(t, 0x4d) != -1) Node_SetLinkPair(Collection_Get(nodes, 0, F<uint32_t>(t, 0x4d)), 0, F<int32_t>(t, 0x25), -3, dir == 0);
}
RT3_REPLACE(0x4afeb0, Train_Advance);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4b0140. Place the initial locomotive, update its value and book its cost.
RT3_NOINLINE void __fastcall Train_PlaceInitialEngine(uint8_t* t, int, uint32_t typeId, uint8_t* node) {
    if (F_train_initialization<uint32_t>(t, 0x41) != 0xffffffff) return;
    uint32_t x = (uint32_t)(uintptr_t)t, z = (uint32_t)(uintptr_t)node;
    Obj_CenterCell((void*)(node), 0, (float*)(&x), (float*)(&z));
    uint8_t* model = Get(0x6ada84, typeId);
    uint32_t end = F_train_initialization<uint32_t>(node, 0x206) == 0xffffffff;
    uint32_t assetId = F_train_initialization<uint32_t>(model, 0x72);
    uint8_t* asset = assetId ? Get(0x62ba88, assetId) : nullptr;
    uint32_t carId = ((uint32_t(__fastcall*)(void*, int, uint8_t*, uint32_t, uint32_t, uint32_t, uint32_t))&Cars_Add)(
        hook::As<void*>(g_62ba84), 0, asset, 0, x, z, end);
    F_train_initialization<uint32_t>(t, 0x41) = carId;
    F_train_initialization<uint32_t>(t, 0x45) = carId;
    uint8_t* car = Get(0x62ba84, carId);
    F_train_initialization<uint32_t>(car, 0x390) = F_train_initialization<uint32_t>(t, 0x25);
    ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))&Car_SetPiece)(
        car, 0, F_train_initialization<uint32_t>(node, 0x202), 0x3e395810);
    if (hook::As<void*>(g_world) &&
        IsMultiplayer() &&
        Net_AllHaveVersion(1.02f)) {
        Car_UpdatePose(car, 0, 0.0f);
    } else {
        Car_Advance(car, 0, 0, nullptr, nullptr, -1);
    }
    assetId = F_train_initialization<uint32_t>(model, 0x72);
    asset = assetId ? Get(0x62ba88, assetId) : nullptr;
    __asm {
        mov eax, asset
        mov ecx, t
        fld dword ptr [eax + 0xfd]
        fadd dword ptr [ecx + 0xf5]
        inc byte ptr [ecx + 0xf9]
        fstp dword ptr [ecx + 0xf5]
    }
    if (model[0x7a]) Append(t, F_train_initialization<uint32_t>(model, 0x76));
    if (!F_train_initialization<uint8_t>(hook::As<void*>(g_newGame), 0x82)) {
        uint8_t* pricedModel = Get(0x6ada84, typeId);
        uint8_t* company = Get(0x62be10, F_train_initialization<uint32_t>(t, 0x51));
        float price = Obj_EffectScale(
            pricedModel, 0, F_train_initialization<uint32_t>(t, 0x51), F_train_initialization<uint32_t>(company, 0x3b));
        uint32_t bits = *(uint32_t*)&price;
        company = Get(0x62be10, F_train_initialization<uint32_t>(t, 0x51));
        ((void(__fastcall*)(uint8_t*, int, int32_t, uint32_t))&Company_AddStat)(company, 0, 0x10, bits);
        __asm {
            fld price
            fchs
            fstp dword ptr bits
        }
        company = Get(0x62be10, F_train_initialization<uint32_t>(t, 0x51));
        ((void(__fastcall*)(uint8_t*, int, int32_t, uint32_t))&Company_AddStat)(company, 0, 0xd, bits);
    }
    Train_Advance(t);
}
RT3_REPLACE(0x4b0140, Train_PlaceInitialEngine);

double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t RendererMode15Above128();
RT3_NOINLINE uint32_t __fastcall Obj_BuildingState8c(void* o);
void __fastcall Station_UnlinkNodes(void* s);
int32_t __fastcall Car_Advance(uint8_t* c, int, uint32_t dist, int32_t* moved, int32_t* count, int32_t p5);

// 0x4b0310 (RET 4). Runs train t for `slice` (paused worlds +0x46c38: nothing):
// - Arrival: the head car on the order's target (Train_OrderTarget, the stop
//   preferred) outside states 0xc / 0xd / 0x11 stops (Train_ProcessStop 1) at a
//   station of state 3 / 4 that is its stop +0x94 (+0xac), else (0) once past
//   the order's distance in its direction (+0x35a: at or beyond, else at or
//   before).
// - On a piece: the stations ahead are looked for (+0xa4 kind 3 when either
//   factor +0xe0 / +0xe4 is below 0x5c970c, re-entering; +0xa8 kind 4 by +0xdc),
//   else cleared.
// - Target speed (+0x55, kept with the head's cell +0x59 / +0x5d and tilt
//   +0x1a7 while the head moved less than 0x5c8648 in single player): the
//   engine's (+0x126 of the car type, 0x6ada84) payment-scaled pull for the
//   consist's weight (Train_ConsistWeight; bends only in single player or
//   multiplayer before 1.02), cut by the bends (the larger of mean x 0x5c8da8 +
//   max x 0x5c8618 and max x 0x5c8880, scaled by bands of the current speed
//   +0xcc and the game version, x 0x5c8660, x 0x5c87f0 from version 1.03, at
//   most 0.8: speed x (1 - cut), at least 1), and in those same games by the
//   mean track-type factor of the pieces under the cars (0x5f3a50 by +0x219
//   for type 1, else 1; at least 0.1). Faster renderers scale it (0x5c8640),
//   factor +0xe0 == 0 halves it (0x5c878c), states 0x11 / 0xc / 0xd / 0xb /
//   0x12 zero it.
// - Speed +0xcc: above the target it falls towards it (time constant 2736, 2052
//   from version 1.03, x 0x5c8880 in those states, by year bands; only in those
//   states or without +0x8b), snapping to 0 when both are tiny and never below
//   the target; below it (without +0x8b) it rises by the engine's acceleration
//   (0x5f345c by engine +0x1c) over the cargo/company effects (at least 0.01),
//   steeper downhill, capped at the target.
// - Then the train heads for the nearest stop and updates its approach
//   (multiplayer: pending template). With an order target the cars move:
//   the head car by slice / 0x5c88a8 x speed / 0x5c86a8 (speed rounded to
//   1 / 0x5c8c28, at least 1, in multiplayer), revenue for the nodes crossed
//   going to the owner of its piece, the others by the distance the head moved;
//   a head car that could not move on its target stops (Train_ProcessStop 0).
//   Train_Advance runs and the mean speed +0x199 over +0x19d samples grows.
// - Without an order target: speeds and cell cleared, the cars faded once
//   (+0xf4; the stations relinked when it changed) and the state becomes 8.
// When Train_OrderTarget returns the stop without writing the distance, the
// original compares with a stack leftover; here the distance is 0.
RT3_NOINLINE void __fastcall Train_Run(uint8_t* t, int, float slice) {
    if (F<int32_t>(g_world, 0x46c38)) return;
    t[0x1b9] = 0;
    uint8_t* head = F<int32_t>(t, 0x41) != -1 ? (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)) : nullptr;
    int32_t cont = 1;
    auto stopped = [](int32_t e8) { return e8 == 0x11 || e8 == 0xc || e8 == 0xd || e8 == 0xb || e8 == 0x12; };
    if (head) {
        int32_t distBits = 0x7fc00001;  // not written yet (a NaN the order distance never is)
        const int32_t piece = F<int32_t>(head, 0x356);
        const int32_t e8 = F<int32_t>(t, 0xe8);
        if (piece == Train_OrderTarget(t, 0, &distBits, 1) && e8 != 0xc && e8 != 0xd && e8 != 0x11) {
            if (Collection_IsValid(g_stations, 0, F<uint32_t>(t, 0xac))) {
                if (void* st = Collection_Get(g_stations, 0, F<uint32_t>(t, 0xac))) {
                    if ((Obj_BuildingState8c(st) == 3 || Obj_BuildingState8c(st) == 4) && F<int32_t>(head, 0x356) == F<int32_t>(t, 0x94)) {
                        Train_ProcessStop(t, 0, 1);
                        return;
                    }
                }
            }
            if (distBits == 0x7fc00001) {
                ++g_unwrittenReads[0];
                distBits = 0;
            }
            const float d = *(const float*)&distBits, pos = F<float>(head, 0x35e);
            const bool arrived = F<int32_t>(head, 0x35a) ? pos >= d : pos <= d;
            if (arrived) {
                Train_ProcessStop(t, 0, 0);
                return;
            }
        }
        if (F<int32_t>(head, 0x356) != -1) {
            Collection_Get(g_tracks, 0, F<uint32_t>(head, 0x356));
            const float k = Fl(0x5c970c);
            if (k > F<float>(t, 0xe0) || k > F<float>(t, 0xe4)) {
                Train_LeaveSite(t);
                Train_FindStationAhead(t, 0, &F<int32_t>(t, 0xa4), 3, 1);
                Train_EnterSite(t);
            } else {
                F<int32_t>(t, 0xa4) = -1;
            }
            if (k > F<float>(t, 0xdc)) Train_FindStationAhead(t, 0, &F<int32_t>(t, 0xa8), 4, 1);
            else F<int32_t>(t, 0xa8) = -1;
        }
        uint8_t* engine = (uint8_t*)Collection_Get(g_6ada84, 0, F<uint32_t>((uint8_t*)Obj_Ref388(head), 0x126));
        float tilt = 0.0f, speed;
        bool recompute = true;
        if (!IsMultiplayer() && F<float>(t, 0x55) > Fl(0x5c8598)) {
            const float y = (float)Obj_CellY(head);
            const float x = (float)Obj_CellX(head);
            if (Distance2D(x, y, F<float>(t, 0x59), F<float>(t, 0x5d)) < D(0x5c8648)) {
                speed = F<float>(t, 0x55);
                tilt = F<float>(t, 0x1a7);
                recompute = false;
            }
        }
        if (recompute) {
            const bool v102 = IsMultiplayer() && Net_AllHaveVersion(1.02f);
            float meanBend = 0.0f, maxBend = 0.0f;
            speed = v102 ? Train_ConsistWeight(t, 0, (int32_t)(uintptr_t)engine, 1, 0, 1, nullptr, nullptr, &tilt)
                         : Train_ConsistWeight(t, 0, (int32_t)(uintptr_t)engine, 1, 1, 1, &meanBend, &maxBend, &tilt);
            speed = Cargo_DeliveryPayment(engine, F<int32_t>(t, 0xd4), speed, F<uint32_t>(t, 0x51), Obj_CityGridByte(t));
            float cut = (float)((double)meanBend * D(0x5c8da8) + (double)maxBend * D(0x5c8618));
            const double maxCut = (double)maxBend * D(0x5c8880);
            cut = (double)cut > maxCut ? cut : (float)maxCut;
            if (cut > Fl(0x5c8598)) {
                const float v = F<float>(t, 0xcc);
                double m;
                if (v < Fl(0x5c85f8)) m = Game_EffectiveVersion() >= 0x67 ? 0.4 : 0.3;
                else if (v < Fl(0x5c8784)) m = Game_EffectiveVersion() >= 0x67 ? 0.6 : 0.5;
                else if (v < Fl(0x5c8d4c)) m = D(0x5c8750);
                else if (v < Fl(0x5c8990)) m = D(0x5c8888);
                else if (v < Fl(0x5c9af4)) m = D(0x5c85a8);
                else if (v < Fl(0x5ca058)) m = D(0x5c8660);
                else m = D(0x5c8578);
                cut = (float)((double)cut * m);
            }
            cut = (float)((double)cut * D(0x5c8660));
            if (Game_EffectiveVersion() >= 0x67) cut = (float)((double)cut * D(0x5c87f0));
            cut = (double)cut < D(0x5c8640) ? cut : 0.8f;
            speed = (float)((D(0x5c8770) - cut) * speed);
            if (Fl(0x5c85b4) > speed) speed = 1.0f;
            if (!v102) {
                int32_t n = 0;
                float sum = 0.0f;
                for (uint8_t* car = head;;) {
                    if (F<int32_t>(car, 0x356) != -1) {
                        uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(car, 0x356));
                        if (F<int32_t>(p, 0x226) == 1) sum = (float)((double)sum + ((const float*)0x5f3a50)[(int8_t)p[0x219]]);
                        else sum = (float)((double)sum + D(0x5c8770));
                        ++n;
                    }
                    if (F<int32_t>(car, 0x352) == -1) break;
                    car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(car, 0x352));
                    if (!car) break;
                }
                const float cnt = (float)(n >= 1 ? n : 1);
                float mean = (float)((double)sum / cnt);
                if ((double)mean < D(0x5c86b8)) mean = 0.1f;
                speed = (float)((double)mean * speed);
            }
            F<float>(t, 0x55) = speed;
            F<float>(t, 0x59) = (float)Obj_CellX(head);
            F<float>(t, 0x5d) = (float)Obj_CellY(head);
            F<float>(t, 0x1a7) = tilt;
        }
        if (RendererMode15Above128()) speed = (float)((double)speed * D(0x5c8640));
        if (F<float>(t, 0xe0) == Fl(0x5c8598)) speed = (float)((double)speed / Fl(0x5c878c));
        if (stopped(F<int32_t>(t, 0xe8))) speed = 0.0f;
        float& cc = F<float>(t, 0xcc);
        if (cc >= speed) {  // unordered: accelerates, as in the original
            float tc = Game_EffectiveVersion() >= 0x67 ? 2052.0f : 2736.0f;
            const int32_t st = F<int32_t>(t, 0xe8);
            if (stopped(st)) tc = (float)((double)tc * D(0x5c8880));
            const uint16_t year = F<uint16_t>(g_world, 0xd);
            if (year >= 0x79e) tc = (float)((double)tc * D(0x5c8628));
            else if (year >= 0x776) tc = (float)((double)tc * D(0x5c8880));
            else if (year >= 0x758) tc = (float)((double)tc * D(0x5c8640));
            else if (year < 0x744) tc = (float)((double)tc * D(0x5c85a8));
            if (stopped(st) || !t[0x8b]) cc = (float)(cc - ((double)cc - speed) * ((double)slice / tc));
            if ((double)speed <= D(0x5c85b8) && (double)cc <= D(0x5c86b8)) cc = 0.0f;
            if (cc <= speed) cc = speed;
        } else if (!t[0x8b]) {
            const uint32_t company = F<uint32_t>(t, 0x51);
            const int32_t kind = F<int32_t>(engine, 0x10) + 0x5e;
            uint32_t p1 = F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b);
            uint32_t p2 = F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b);
            Obj_CargoBonus(g_world, 0, kind, company, p2, Obj_CityGridByte(t));
            Obj_CargoBonus(g_world, 0, 0x5d, company, p1, Obj_CityGridByte(t));
            p1 = F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b);
            p2 = F<uint32_t>(Collection_Get(g_companies, 0, company), 0x3b);
            const float f1 = Effects_Multiplier(g_world, 0, kind, company, p2, Obj_CityGridByte(t));
            float m = (float)((double)Effects_Multiplier(g_world, 0, 0x5d, company, p1, Obj_CityGridByte(t)) * f1);
            if (Game_EffectiveVersion() >= 0x67 && !Company_OwnerFlag1e1(Collection_Get(g_companies, 0, company)))
                m = (float)((double)m * D(0x5c8660));
            const double md = (double)m > D(0x5c85b8) ? (double)m : 0.01;
            float acc = (float)((double)((const float*)0x5f345c)[F<int32_t>(engine, 0x1c)] / md);
            if (tilt < Fl(0x5c8598)) {
                const float g = (float)((double)tilt * Fl(0x5c9aa8) + D(0x5c8770));
                const float gf = (float)((double)g > D(0x5c86b8) ? (double)g : 0.1);
                acc = (float)((double)gf * acc);
            }
            const float next = (float)((double)slice / Fl(0x5cb3f4) * speed / acc + cc);
            cc = next;
            if (next >= speed) cc = speed;
        }
        Train_HeadForNearestStop(t, 0, head);
        Train_UpdateApproachFactor(t, 0, head);
        if (IsMultiplayer()) Obj_ApplyPendingTemplate(t);
    }
    // The order's target node (-1: none).
    auto target = [&](bool forMove) -> int32_t {
        if (F<int32_t>(t, 8) == -1) return -1;
        uint8_t* ord = (uint8_t*)Array33_Current(t);
        if (!ord) return -1;
        if ((int8_t)ord[0x28] < 0) {
            if (!forMove) return F<int32_t>(ord, 0x2b);
            return Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x94)) ? F<int32_t>(t, 0x94) : F<int32_t>(ord, 0x2b);
        }
        if (!Collection_IsValid(g_stations, 0, F<uint16_t>(ord, 0x29))) return F<int32_t>(t, 0x94);
        uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(ord, 0x29));
        if (!s) return -1;
        if (!forMove) return F<int32_t>(s, 8);
        return F<int32_t>(t, 0x94) != -1 ? F<int32_t>(t, 0x94) : F<int32_t>(s, 8);
    };
    if (target(false) != -1) {
        int32_t count = 0, moved = 0;
        bool first = true;
        for (uint8_t* car = head; car && cont;) {
            const int32_t to = target(true);
            if (first) {
                uint32_t owner = F<uint32_t>(t, 0x51);
                if (Collection_IsValid(g_tracks, 0, F<uint32_t>(car, 0x356)))
                    owner = F<uint16_t>(Collection_Get(g_tracks, 0, F<uint32_t>(car, 0x356)), 0x22e);
                float& cc = F<float>(t, 0xcc);
                if (IsMultiplayer()) {
                    const float r = (float)Ftol((double)cc * Fl(0x5c8c28));
                    cc = (float)((double)r / Fl(0x5c8c28));
                    if (Fl(0x5c85b4) > cc) cc = 1.0f;
                }
                const float dist = (float)((double)slice / D(0x5c88a8) * cc / D(0x5c86a8));
                cont = Car_Advance(car, 0, *(const uint32_t*)&dist, &moved, &count, to);
                if (count) Train_AddRevenue(t, 0, count, (int32_t)owner);
                first = false;
                if (!cont && F<int32_t>(car, 0x356) == Train_OrderTarget(t, 0, nullptr, 1)) Train_ProcessStop(t, 0, 0);
            } else {
                cont = Car_Advance(car, 0, (uint32_t)moved, nullptr, &count, to);
            }
            if (F<int32_t>(car, 0x352) == -1) break;
            car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(car, 0x352));
        }
        Train_Advance(t);
        const int32_t n = F<int32_t>(t, 0x19d);
        const float fn = (float)n;
        t[0x196] = 0;
        F<int32_t>(t, 0x19d) = n + 1;
        F<float>(t, 0x199) = (float)((double)fn * F<float>(t, 0x199));
        F<float>(t, 0x199) = (float)(((double)F<float>(t, 0xcc) + F<float>(t, 0x199)) / (float)(n + 1));
        return;
    }
    F<int32_t>(t, 0x55) = 0;
    F<int32_t>(t, 0x1a7) = 0;
    F<int32_t>(t, 0x59) = 0;
    F<int32_t>(t, 0x5d) = 0;
    IsMultiplayer();
    const uint8_t old = t[0xf4];
    if (old != 1) {
        t[0xf4] = 1;
        for (uint8_t* car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(t, 0x41)); car;) {
            SceneObj_SetAlpha(car, 0, 0x3f, 1);
            Model_SetOpacity(car, 0, (float)(t[0xf4] ? 0.25 : 1.0));
            if (F<int32_t>(car, 0x352) == -1) break;
            car = (uint8_t*)Collection_Get(g_62ba84, 0, F<uint32_t>(car, 0x352));
        }
        if (old != t[0xf4]) Station_UnlinkNodes(t);
    }
    F<int32_t>(t, 0xe8) = 8;
    IsMultiplayer();
    F<int32_t>(t, 0xec) = 0;
}
RT3_REPLACE(0x4b0310, Train_Run);

// 0x4b10d0 (thiscall). Runs train t's state machine (+0xe8) for dt game
// seconds (once more with force even when none are left): each pass takes a
// slice of at most the world's top step (0x6cec78, capped by the speed +0xcc
// unless every multiplayer peer has version 1.02, then whole numbers) and
// the pending wait +0xec, and lets the state use it:
//   0, 0x11, 0x12: run (0x4b0310), the last two counting down the wait to
//   the crash (0x12 -> 0xb) or the breakdown (0x11 -> 0xc) repair;
//   1 at a station, 2 arrive, 3/0xe/0xf/0x10 start, 4 (signal fade in +0xe0
//   /+0xe4) and 6 (loading +0xdc, at the world's load rate) count up to 1,
//   5 hide the cars and wait, 7 wait for a free signal, 8/9 end of orders,
//   0xa/0x13 wait out +0x1a3 for a reachable order, 0xb/0xc recover.
// A train following another (+0x98) only checks that the other still runs.
RT3_NOINLINE void __fastcall Train_Step(uint8_t* t, int, float dt, int32_t force) {
    if (F<int32_t>(t, 0x98) != -1 && F<int32_t>(t, 0x98) != F<int32_t>(t, 0x25)) {
        Collection* trains = g_trains;
        uint8_t* other;
        if (!Collection_IsValid(trains, 0, F<uint32_t>(t, 0x98)) ||
            !(other = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(t, 0x98))) || !Train_StateSpecial(other))
            F<int32_t>(t, 0x98) = -1;
        return;
    }
    auto cars = []() { return g_62ba84; };
    // Shows (0x3f, a quarter opaque) or hides (0xff) the cars and links or
    // unlinks the station nodes when that changes.
    auto setCars = [&](uint8_t hidden) {
        F<int32_t>(t, 0x55) = 0;
        F<int32_t>(t, 0x1a7) = 0;
        F<int32_t>(t, 0x59) = 0;
        F<int32_t>(t, 0x5d) = 0;
        IsMultiplayer();
        const uint32_t was = F<uint8_t>(t, 0xf4);
        if (was == hidden) return;
        F<uint8_t>(t, 0xf4) = hidden;
        for (uint8_t* car = (uint8_t*)Collection_Get(cars(), 0, F<uint32_t>(t, 0x41)); car;) {
            SceneObj_SetAlpha(car, 0, hidden ? 0x3f : 0xff, 1);
            Model_SetOpacity(car, 0, (float)(F<uint8_t>(t, 0xf4) ? 0.25 : 1.0));
            const int32_t next = F<int32_t>(car, 0x352);
            if (next == -1) break;
            car = (uint8_t*)Collection_Get(cars(), 0, (uint32_t)next);
        }
        if (was != F<uint8_t>(t, 0xf4)) {
            if (hidden)
                Station_UnlinkNodes(t);
            else
                Station_RelinkNodes(t);
        }
    };
    auto leadCar = [&]() { return F<int32_t>(t, 0x41) != -1 ? (uint8_t*)Collection_Get(cars(), 0, F<uint32_t>(t, 0x41)) : nullptr; };
    auto modern = []() { return IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f828f5c)); };
    // The next order (with +0xfc) and the start.
    auto nextOrder = [&]() {
        if (F<uint8_t>(t, 0xfc)) {
            TrainOrders_SetCurrent(t, 0, (F<int32_t>(t, 8) + 1) % F<int32_t>(t, 4), 0);
            F<int32_t>(t, 0xac) = *(const uint16_t*)((const uint8_t*)Array33_Current(t) + 0x29);
        }
    };
    auto clamp1 = [](float v) { return (float)(v > D(0x5c8770) ? D(0x5c8770) : (double)v); };
    auto rate = [](double scale) {
        const float k = F<float>(g_world, 0x4ca2);
        return (k < D(0x5c85b8) ? D(0x5c85b8) : (double)k) * scale;
    };
    for (;;) {
        if (!(dt > 0.0f) && !force) return;
        if (F<float>(t, 0xec) > 0.0f && F<int32_t>(t, 0xe8) != 0x11 && F<int32_t>(t, 0xe8) != 0x12) {
            // The wait eats the time first.
            if (F<float>(t, 0xec) > dt) {
                F<float>(t, 0xec) = F<float>(t, 0xec) - dt;
                dt = 0.0f;
                if (!force) return;
                goto run;
            }
            dt = dt - F<float>(t, 0xec);
            F<float>(t, 0xec) = 0.0f;
        }
        if (!(dt > 0.0f) && !(dt != dt) && !force) return;
    run:
        force = 0;
        float top = World_Value4cbe(hook::As<void*>(g_world));
        if (IsMultiplayer()) top = (float)Ftol_207(top);
        if (!modern()) {
            const float speed = F<float>(t, 0xcc);
            if (speed > Fl_207(0x5c96b0)) {
                if (Fl_207(0x5c8d4c) < top) top = BitsF(0x42200000);
            } else if (speed > Fl_207(0x5c9ac0)) {
                if (Fl_207(0x5c8780) < top) top = BitsF(0x42480000);
            } else if (speed > Fl_207(0x5c8c28)) {
                if (Fl_207(0x5c8840) < top) top = BitsF(0x428c0000);
            } else if (speed > Fl_207(0x5c8840)) {
                if (Fl_207(0x5c8944) < top) top = BitsF(0x42aa0000);
            }
        }
        float slice = dt > top ? top : dt;
        if (F<float>(t, 0xec) > 0.0f && !(slice < F<float>(t, 0xec))) slice = F<float>(t, 0xec);
        if (modern()) slice = (float)Ftol_207(slice);
        switch ((uint32_t)F<int32_t>(t, 0xe8)) {
        case 0:
            Train_Run(t, 0, slice);
            dt = dt - slice;
            break;
        case 1: {
            const float used = Train_StepAtStation(t, 0, dt);
            dt = dt - used;
            break;
        }
        case 2:
            Train_Arrive(t);
            break;
        case 3:
            Train_ResetCarAnimations(t);
            Train_SetState(t, 0, 4, 0.0f);
            break;
        case 4: {
            // The signal fade in (+0xe0 / +0xe4 up to 1), then go.
            auto smaller = [&]() { return F<float>(t, 0xe4) < F<float>(t, 0xe0) ? F<float>(t, 0xe4) : F<float>(t, 0xe0); };
            if (modern()) {
                const float least = smaller();
                if (least + Fl_207(0x5ce838) > Fl_207(0x5c85b4)) {
                    Train_PassSignal(t);
                    dt = 0.0f;
                } else {
                    F<float>(t, 0xe4) = clamp1(F<float>(t, 0xe4) + Fl_207(0x5ce838));
                    F<float>(t, 0xe0) = clamp1(F<float>(t, 0xe0) + Fl_207(0x5ce838));
                    dt = dt - slice;
                    break;
                }
            } else {
                const float least = smaller();
                const float step = (float)(slice / rate(D(0x5d00c0)));
                if (step + least > Fl_207(0x5c85b4)) {
                    Train_PassSignal(t);
                    slice = (float)((Fl_207(0x5c85b4) - least) / step * slice);
                    dt = dt - slice;
                } else {
                    F<float>(t, 0xe4) = clamp1(step + F<float>(t, 0xe4));
                    F<float>(t, 0xe0) = clamp1(step + F<float>(t, 0xe0));
                    dt = dt - slice;
                    break;
                }
            }
            F<float>(t, 0xe4) = 1.0f;
            F<float>(t, 0xe0) = 1.0f;
            nextOrder();
            Train_SetState(t, 0, 0, BitsF(0x45870000));
            break;
        }
        case 5:
            Train_ResetCarAnimations(t);
            setCars(1);
            F<int32_t>(t, 0xe8) = 6;
            IsMultiplayer();
            F<float>(t, 0xec) = 0.0f;
            break;
        case 6: {
            // Loading (+0xdc up to 1).
            if (modern()) {
                const float v = F<float>(t, 0xdc) + Fl_207(0x5ce838);
                if (!(v > Fl_207(0x5c85b4))) {
                    F<float>(t, 0xdc) = v;
                    dt = dt - slice;
                    break;
                }
                Train_ResetStop(t);
                F<int32_t>(t, 0xa8) = -1;
                dt = 0.0f;
            } else {
                const float step = (float)(slice / rate(D(0x5d00b8)));
                const float v = step + F<float>(t, 0xdc);
                if (!(v > Fl_207(0x5c85b4))) {
                    F<float>(t, 0xdc) = v;
                    dt = dt - slice;
                    break;
                }
                const double part = (Fl_207(0x5c85b4) - F<float>(t, 0xdc)) / step;
                const int32_t node = F<int32_t>(t, 0x94);
                F<uint32_t>(t, 0x8c) = 0xbf800000;
                F<int32_t>(t, 0x1ba) = node;
                F<int32_t>(t, 0x94) = -1;
                F<int32_t>(t, 0xa8) = -1;
                slice = (float)(part * slice);
                dt = dt - slice;
            }
            F<float>(t, 0xdc) = 1.0f;
            nextOrder();
            Train_SetState(t, 0, 0, 0.0f);
            break;
        }
        case 7:
            if (Train_CheckPendingNode(t) && Train_SignalsClear(t)) {
                Train_SetState(t, 0, 0, 0.0f);
                break;
            }
            Train_ResetCarAnimations(t);
            F<float>(t, 0xcc) = 0.0f;
            dt = dt - slice;
            break;
        case 8:
        case 9:
            // The end of the orders.
            if (F<int32_t>(t, 4) == 0) {
                Train_ResetCarAnimations(t);
                setCars(1);
                F<int32_t>(t, 0xe8) = 8;
                IsMultiplayer();
                F<float>(t, 0xec) = 0.0f;
                dt = 0.0f;
                F<float>(t, 0xcc) = 0.0f;
            } else if (F<int32_t>(t, 4) == 1) {
                Train_ResetCarAnimations(t);
                int32_t home = -1;
                if (Obj_City41(t)) home = F<int32_t>(Obj_City41(t), 0x356);
                if (Obj_City41(t) && F<int32_t>(t, 0x1ba) == home) {
                    setCars(1);
                    F<int32_t>(t, 0xe8) = 9;
                    IsMultiplayer();
                    F<float>(t, 0xec) = 0.0f;
                    F<float>(t, 0xcc) = 0.0f;
                    dt = dt - slice;
                } else {
                    Train_SetState(t, 0, 0, 0.0f);
                }
            } else if (Train_CanReachOrder(t, 0, 0)) {
                Train_SetState(t, 0, 0, 0.0f);
                Train_Route(t);
            } else {
                const int32_t wait = Obj_CityKindIs2(t) && Train_CanReachOrder(t, 0, 1);
                setCars(1);
                F<int32_t>(t, 0xe8) = wait ? 0x13 : 0xa;
                IsMultiplayer();
                F<float>(t, 0xec) = 0.0f;
            }
            break;
        case 0xa:
        case 0x13: {
            F<float>(t, 0xcc) = 0.0f;
            const float left = F<float>(t, 0x1a3) - dt;
            F<float>(t, 0x1a3) = left;
            if (left > 0.0f || left != left) {
                dt = 0.0f;
                break;
            }
            if (F<int32_t>(t, 0x49) == -1) Train_Advance(t);
            if (Train_CanReachOrder(t, 0, 0)) {
                Train_Route(t);
                Train_SetState(t, 0, 0, 0.0f);
            } else {
                F<uint32_t>(t, 0x1a3) = 0x461d8000;
            }
            break;
        }
        case 0xb:
            Car_SetBrokenDown(leadCar(), 0, 0);
            Vehicle_SelectSound(leadCar(), 0, 0);
            Train_SetState(t, 0, 0, 0.0f);
            goto clearTarget;
        case 0xc:
            Car_SetCrashed(leadCar(), 0, 0);
            setCars(0);
            F<int32_t>(t, 0xe8) = 0xd;
            IsMultiplayer();
            F<float>(t, 0xec) = 0.0f;
        clearTarget: {
            const int32_t node = F<int32_t>(t, 0x94);
            F<uint32_t>(t, 0x8c) = 0xbf800000;
            F<int32_t>(t, 0x1ba) = node;
            F<int32_t>(t, 0x94) = -1;
            break;
        }
        case 0xd:
            dt = 0.0f;
            break;
        case 0xe:
        case 0xf:
            Train_SetState(t, 0, 0, 0.0f);
            break;
        case 0x10:
            if (F<int32_t>(t, 0xd8)) {
                Train_SetState(t, 0, 0, 0.0f);
                break;
            }
            Train_ResetCarAnimations(t);
            F<float>(t, 0xcc) = 0.0f;
            dt = dt - slice;
            break;
        case 0x11:
        case 0x12: {
            // Stopped by a breakdown (0x11) or a crash (0x12).
            const int32_t state = F<int32_t>(t, 0xe8);
            F<float>(t, 0xec) = F<float>(t, 0xec) - slice;
            Train_Run(t, 0, slice);
            dt = dt - slice;
            if (F<float>(t, 0xec) > 0.0f || F<float>(t, 0xec) != F<float>(t, 0xec)) break;
            setCars(0);
            F<int32_t>(t, 0xe8) = state == 0x11 ? 0xc : 0xb;
            IsMultiplayer();
            F<uint32_t>(t, 0xec) = 0x46b40000;
            break;
        }
        default:
            // QUIRK: an unknown state spins on the same time forever.
            break;
        }
    }
}
RT3_REPLACE(0x4b10d0, Train_Step);

}  // namespace rt3
