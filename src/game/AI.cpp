// AI.cpp - RT3_mp.exe 0x401000-0x40a000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/stream.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
int32_t __fastcall Obj_BuildingValue(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
int32_t __fastcall DistanceSqInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
int32_t __fastcall Game_EffectiveVersion();
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint32_t __fastcall EngineTypes_PickForYear(Collection* c);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Station_ConnectedToHome(uint8_t* s, int, uint32_t company);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o);
uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t);
double __stdcall Effects_CargoMultiplier(int32_t idx);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
void* __fastcall Engine_Renderer(void* e);
uint64_t __fastcall DateFromMinutes(int32_t m);
float __stdcall Daylight_Factor(float hour);
void __fastcall Clock_SetD(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
void __fastcall Clock_Set11(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
void __fastcall Economy_UpdateEraFactors(uint8_t* o);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_StateKindIs4(void* o);
float __fastcall Building_Price(uint8_t* b, int, uint32_t company);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
void* __fastcall Company_Owner(void* c);
int32_t __fastcall Obj_ArgMax67(void* o);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Chain_HasValue(void* o, int, int32_t v);
void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price);
const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
float __fastcall Company_MaxDividend(uint8_t* c);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Company_GoBankrupt(uint8_t* c);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
void __fastcall Company_RateStations(int32_t* co);
void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y);
int32_t __fastcall Company_BestSite(int32_t* c);
int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);
struct GameDate;
int32_t __fastcall DateToMinutes(const GameDate* d);
void __fastcall CompanyWindow_Refresh(uint8_t* win);
int32_t Screen90();
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
int __fastcall ToUpper(char c);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Table_CountField3f(Collection* t);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
float __fastcall Company_CreditRating(uint8_t* c, int, int32_t extraDebt);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
int32_t __fastcall Map_TerrainKindAt(void* m, int, float x, float y);
void* __fastcall Train_Company(void* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
void __fastcall Company_Add7680(void* c, int, int32_t d);
uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
int32_t __fastcall Company_Get7680(void* c);
int32_t __fastcall Track_Cost(uint8_t* t, int, uint32_t company);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

double __stdcall AngleOf(float x0, float y0, float x1, float y1);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
float __fastcall Obj_CenterCellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
void __fastcall Company_UpdateStationLinks(int32_t* co);
int32_t __fastcall Company_PickTrainOrder(int32_t* c, int, uint8_t* o, uint32_t from, uint8_t* train);
uint32_t __fastcall AI_PickEngineType(uint32_t company);
int32_t __fastcall Train_FirstCarType126(uint8_t* t);
uint32_t __fastcall Obj_RefName72(void* o);
void* __fastcall Cursor33_Init(uint8_t* c);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b);
float __fastcall Tracks_TotalCost(Collection* c, int, uint32_t company);
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y);
void Game_PauseTools();
uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
void __fastcall Track_Validate(Collection* t);
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE uint32_t __fastcall AI_PickEngineType(uint32_t company);
void __fastcall Obj_Stash98(uint8_t* o);
int32_t __fastcall Obj_Unstash98(uint8_t* o);
float __fastcall Obj_Rating296(void* o);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
void __fastcall Obj_PlatformOffset(void* o, int, int32_t alongX, float* outX, float* outY, int32_t* dir);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
float __stdcall Map_SlopeAngle(float ax, float ay, float bx, float by);
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y);
RT3_NOINLINE int __fastcall ToUpper(char c);
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
RT3_NOINLINE void __fastcall Company_BuildStationLinks(int32_t* co);
void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);
void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
void __fastcall Obj_SizeClass(void* o, int, int32_t* a, int32_t* b);
uint32_t __fastcall Obj_FindRegionSite(void* o, int, int32_t requireActive, int32_t requireFlag);
RT3_NOINLINE float __fastcall Obj_NodeCenterX(void* o);
RT3_NOINLINE float __fastcall Obj_NodeCenterY(void* o);
uint32_t __fastcall Tile_IsBlocked(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Company_QualityRaw(void* c);
float __fastcall City_ServiceValue(uint8_t* o, int, uint32_t company);
int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
void __fastcall Train_AutoName(uint8_t* t, int, uint32_t a, uint32_t b);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void* __fastcall Company_Owner(void* c);
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
int32_t __fastcall Company_CanIssueBond(uint8_t* c);
RT3_NOINLINE float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall Player_AiTurn(uint8_t* p);
void __fastcall AI_CompanyTurn(uint8_t* co);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __fastcall Company_OwnerFlag1e1(void* c);
void __fastcall Company_AiBuyIndustry(uint8_t* c);
void __fastcall Company_YearFinance(uint8_t* c);
RT3_NOINLINE void __fastcall Company_RateStations(int32_t* co);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void __fastcall AI_ReviewTrains(uint8_t* co, int, int32_t built);
float __fastcall Company_StationRate(int32_t* c);
RT3_NOINLINE int32_t __fastcall Company_TrainCount(void* c);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
int32_t __fastcall Company_TrainCount(void* c);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void* __fastcall Collection_GetAt(Collection* c, int, uint32_t pos, int32_t list);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
void __fastcall AiTrain_NextOrder(uint8_t* t);
float __fastcall Obj_NodeCenterX(void* o);
float __fastcall Obj_NodeCenterY(void* o);
void* __fastcall Obj_Station173(uint8_t* o);
void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
void* __fastcall Obj_Cargo72(uint8_t* o);
uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
void __fastcall Obj_SizeClass(void* o, int, int32_t* a, int32_t* b);
float __fastcall Obj_Rating296(void* o);
RT3_NOINLINE void* __fastcall Collection_GetAt(Collection* c, int, uint32_t pos, int32_t list);
void* __fastcall Collection_RecordAt(Collection* t, int, uint32_t pos);
float __fastcall City_ServiceValue(uint8_t* o, int, uint32_t company);
void __fastcall Obj_PlatformOffset(void* o, int, int32_t alongX, float* outX, float* outY, int32_t* dir);
RT3_NOINLINE int32_t AI_TestFirstRoute();
void __fastcall Company_AiBuyIndustry(uint8_t* c);
int32_t __fastcall Company_HomeStation(int32_t* c);
RT3_NOINLINE void __fastcall AiCompany_ReplanTrains(int32_t* c);
RT3_NOINLINE void __fastcall AI_CompanyTurn(uint8_t* co);
void __fastcall Company_YearFinance(uint8_t* c);
float __stdcall Map_SlopeAngle(float ax, float ay, float bx, float by);
RT3_NOINLINE void* __fastcall Obj_Station173(uint8_t* o);
RT3_NOINLINE int32_t __fastcall AI_BuildRoute(Collection* net, int, int32_t cityA, int32_t pieceA, int32_t typeA, int32_t cityB, int32_t pieceB, int32_t typeB, int32_t estimate, float* outCost, int32_t company);
RT3_NOINLINE int32_t __fastcall AI_ConnectCityToPiece(int32_t* ai, int, int32_t city, int32_t piece, int32_t estimate, float* outCost);
RT3_NOINLINE int32_t __fastcall AI_ConnectCity(int32_t* ai, int, int32_t city, int32_t estimate, float* outCost, int32_t* toPiece, int32_t* fromPiece);
RT3_NOINLINE int32_t __fastcall AI_ConnectCities(int32_t cityA, int32_t cityB, int32_t estimate, float* outCost, int32_t company);
RT3_NOINLINE int32_t __fastcall AI_PlanFirstRoute(int32_t test, int32_t rival, int32_t company);
RT3_NOINLINE float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall TrackNet_AiDoubleTrack(Collection* net, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall AI_PlanExpansion(uint8_t* co);
RT3_NOINLINE void* __fastcall Collection_RecordAt2(Collection* t, int, uint32_t pos);
RT3_NOINLINE void __fastcall AI_Tick(uint8_t* w);
RT3_NOINLINE void __fastcall Player_AiTradeStocks(uint8_t* p);
RT3_NOINLINE void __fastcall Player_AiTurn(uint8_t* p);
void __fastcall Obj_AllocEntries(void* o);
RT3_NOINLINE void __fastcall Obj_FreeEntries5bd(void* o);
RT3_NOINLINE void* __fastcall Obj_Cargo72(uint8_t* o);
RT3_NOINLINE void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
RT3_NOINLINE uint32_t __fastcall AI_PickEngineType(uint32_t company);
RT3_NOINLINE void __fastcall Company_RateStations(int32_t* co);
RT3_NOINLINE int32_t __fastcall Company_BestSite(int32_t* c);
RT3_NOINLINE int32_t __fastcall Company_PickTrainOrder(int32_t* c, int, uint8_t* o, uint32_t from, uint8_t* train);
float __fastcall Company_StationRate(int32_t* c);
RT3_NOINLINE void __fastcall AiCompany_ReplaceEngine(uint8_t* c, int, uint32_t train);
RT3_NOINLINE void __fastcall Company_BuildStationLinks(int32_t* co);
RT3_NOINLINE void __fastcall Company_UpdateStationLinks(int32_t* co);
RT3_NOINLINE void __fastcall AiTrain_NextOrder(uint8_t* t);
RT3_NOINLINE void __fastcall AI_BuyTrain(int32_t* co);
RT3_NOINLINE void __fastcall AI_ReviewTrains(uint8_t* co, int, int32_t built);
float __stdcall Daylight_Factor(float hour);
RT3_NOINLINE void* __fastcall Table_At(Collection* t, int, int32_t i);
RT3_NOINLINE void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
RT3_NOINLINE int32_t __fastcall Building_Place(uint8_t* b, int, float x, float y, uint32_t company);
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
RT3_NOINLINE void __fastcall Company_GoBankrupt(uint8_t* c);
RT3_NOINLINE float __fastcall Company_MaxDividend(uint8_t* c);
RT3_NOINLINE void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
RT3_NOINLINE void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
RT3_NOINLINE int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
RT3_NOINLINE void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h);
RT3_NOINLINE void Game_PauseTools();
RT3_NOINLINE void Options_Restore();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
RT3_NOINLINE int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
RT3_NOINLINE int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
RT3_NOINLINE void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
RT3_NOINLINE uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
RT3_NOINLINE void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
RT3_NOINLINE void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);
RT3_NOINLINE int32_t __fastcall TrackNet_LayLine(Collection* net, int, float x0, float y0, float x1, float y1, int32_t company, int32_t mode, int32_t doubleKind, int32_t* from, int32_t* to, float positive);
RT3_NOINLINE void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
RT3_NOINLINE int32_t __stdcall Route_CanReach(int32_t from, int32_t to, int32_t needFlag);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE void __fastcall CompanyWindow_Refresh(uint8_t* win);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);

static inline double ExeSqrt(double v) { return ((double(__cdecl*)(double))0x5a1634)(v); }

static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }

static inline Collection* Engines() { return hook::Global<Collection*>(0x6ada84); }

static inline uint8_t* CarTypeOf(uint8_t* e) {
    return F<uint32_t>(e, 0x72) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(e, 0x72))
                                : nullptr;
}

static inline Collection* Stations() { return hook::Global<Collection*>(0x6cec20); }

static inline Collection* Players() { return hook::Global<Collection*>(0x62be10); }

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

static inline double Fl_193(uint32_t a) { return *(const float*)a; }

static inline double Fl_202(uint32_t a) { return *(const float*)a; }

static inline double Fl_203(uint32_t a) { return *(const float*)a; }

static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

static inline double ExeFmod(double a, double b) { return ((double(__cdecl*)(double, double))0x5a1390)(a, b); }

// Splits fractional hours into whole hours h and minutes / seconds (fractions x 60).
static void SplitHours(float t, int32_t* h, float* m, float* s) {
    *h = Ftol(t);
    float hf = (float)*h;
    *m = (float)(((double)t - hf) * Fl_203(0x5c8990));
    float mf = (float)Ftol(*m);
    *s = (float)(((double)*m - mf) * Fl_203(0x5c8990));
}

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }

// Cars a train can carry in the current year (0x6cec78 +0xd).
static int32_t TrainCarsForYear() {
    uint32_t y = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    if (y < 0x749) return 4;
    if (y < 0x758) return 5;
    if (y < 0x76c) return 6;
    if (y < 0x780) return 5;
    if (y < 0x78f) return 6;
    if (y < 0x79e) return 7;
    if (y < 0x7b2) return 6;
    return 7 + (y >= 0x7c6);
}

// Bond i of company c: amount (+0x5f), rate (+0x67), 12 bytes each.
static inline int32_t& BondAmount(uint8_t* c, int32_t i) { return F<int32_t>(c, 0x5f + i * 0xc); }

static inline float BondRate(uint8_t* c, int32_t i) { return F<float>(c, 0x67 + i * 0xc); }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// Whether piece p of company id is plain unmarked track (states 1 / 2,
// no piece kind, +0x24c bit 2 clear).
static bool Track_DoubleCandidate(uint8_t* p, uint32_t id) {
    return F<uint16_t>(p, 0x22e) == id && !F<int32_t>(p, 0x226) && (p[0x218] == 1 || p[0x218] == 2) && !(p[0x24c] & 4);
}

namespace {
// The AI route builder's stack frame, kept as in the original: several slots
// change meaning on the way (see the QUIRK below).
struct RouteFrame {
    alignas(8) uint8_t b[0x1f0];
    int32_t& i(uint32_t o) { return *(int32_t*)(b + o); }
    float& f(uint32_t o) { return *(float*)(b + o); }
    double& d(uint32_t o) { return *(double*)(b + o); }
    uint8_t*& p(uint32_t o) { return *(uint8_t**)(b + o); }
};
inline double RtSin(double x) { return ((double(__cdecl*)(double))0x5a1494)(x); }
inline double RtCos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }
inline double RtAbs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
inline double RtMod(double x, double y) { return ((double(__cdecl*)(double, double))0x5a1390)(x, y); }
inline int RtStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
inline float RtBits(uint32_t v) { return *(const float*)&v; }
inline uint8_t* RtGet(uint32_t global, int32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(global), 0, (uint32_t)id); }
inline uint8_t* RtMap() { return hook::Global<uint8_t*>(0x62c120); }
}

namespace {
#pragma pack(push, 1)
// A candidate target piece of AI_ConnectCity (10 bytes in the original).
struct RouteTarget {
    int32_t id;
    int32_t dist;
    uint8_t usable;
    uint8_t foreign;
};
#pragma pack(pop)
}

namespace {
// AI_PlanFirstRoute's frame: city tables (score +0x4f8, x +0x778, y +0x9f8,
// id +0x278, taken bytes +0x1d8) and 20 trial slots (cost +0x48, tried
// +0x98, built +0xe8, city indices +0x188 / +0x138). Kept as one block: a
// search that ends without a trial reads slot -1, i.e. the neighbouring
// tables' slot 19.
struct PlanFrame {
    alignas(8) uint8_t b[0xc80];
    int32_t& i(int32_t o) { return *(int32_t*)(b + o); }
    float& f(int32_t o) { return *(float*)(b + o); }
};
}

// 0x401000. Size class of +0x30e: <= 5 -> 0, <= 13 -> 1, else 2 (written to both outputs).
RT3_NOINLINE void __fastcall Obj_SizeClass(void* o, int, int32_t* a, int32_t* b) {
    float v = F<float>(o, 0x30e);
    int32_t c = v <= *(const float*)0x5c8554 ? 0 : v <= *(const float*)0x5c8550 ? 1 : 2;
    *b = c;
    *a = c;
}
RT3_REPLACE(0x401000, Obj_SizeClass);

// 0x401070. Rating by +0x296: 9.4 / 8.4 / 7.4 / 6.4 from 85 / 70 / 50.
RT3_NOINLINE float __fastcall Obj_Rating296(void* o) {
    int32_t v = F<int32_t>(o, 0x296);
    if (v >= 0x55) return *(const float*)0x5c8564;
    if (v >= 0x46) return *(const float*)0x5c8560;
    if (v >= 0x32) return *(const float*)0x5c855c;
    return *(const float*)0x5c8558;
}
RT3_REPLACE(0x401070, Obj_Rating296);

// 0x4010b0. The record at position `pos` of list `list` of collection c.
RT3_NOINLINE void* __fastcall Collection_GetAt(Collection* c, int, uint32_t pos, int32_t list) { return Collection_Get(c, 0, Collection_IdAt(c, 0, pos, list)); }
RT3_REPLACE(0x4010b0, Collection_GetAt);

// 0x4010d0 / 0x406bf0 / 0x432e60. Record at position `pos` (of list 0 / `list`).
void* __fastcall Collection_RecordAt(Collection* t, int, uint32_t pos) { return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, 0)); }
RT3_REPLACE(0x4010d0, Collection_RecordAt);

// 0x4010f0. What serving the city is worth to the company (0 for ghost cities +0x23e,
// regions the company may not enter, or cities it already serves from its home
// network): its demand (+0x306/+0x30a/+0x30e) shared among the ready stations in it,
// and cut when another company competes there (v1.03+).
RT3_NOINLINE float __fastcall City_ServiceValue(uint8_t* o, int, uint32_t company) {
    if (F<int32_t>(o, 0x23e)) return Fl(0x5c8598);
    float s = (float)((double)F<float>(o, 0x30a) + F<float>(o, 0x306));
    int32_t v = Ftol(((double)F<float>(o, 0x30e) * D(0x5c8590) + s) / D(0x5c8588) * D(0x5c8580));
    if (company) {
        float y = (float)Obj_CellY(o);
        uint32_t region = Map_RegionTypeAt(hook::Global<void*>(0x62c120), 0, (float)Obj_CellX(o), y);
        if (!Company_EntryFlag(Collection_Get(Players(), 0, company), 0, region)) return Fl(0x5c8598);
    }
    int32_t rival = 0, n = 0;
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        if (!Obj_RegionHasKey(o, 0, (uint16_t)Collection_IdAt(Stations(), 0, i, 0))) continue;
        uint8_t* st = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0));
        if (Obj_Flag3d4(Obj_Building(st)) != 1) continue;
        if (company && Station_ConnectedToHome(st, 0, company)) return Fl(0x5c8598);
        ++n;
        if (Game_EffectiveVersion() < 0x67 || !company || company == (uint32_t)Obj_BuildingValue(st)) continue;
        if (!Collection_IsValid(Players(), 0, Obj_BuildingValue(st))) continue;
        if (!Company_OwnerFlag1e1(Collection_Get(Players(), 0, Obj_BuildingValue(st)))) rival = 1;
    }
    if (n) v = Ftol(D(0x5c8578) / ((double)n + D(0x5c8578)) * (double)v);
    if (rival) v = Ftol((double)v * D(0x5c8570));
    return (float)v;
}
RT3_REPLACE(0x4010f0, City_ServiceValue);

// 0x401300. Station platform offset for this object's diagonal parity
// ((4x - (-4)y) & 3 picks the signs of +-1.4375): with `alongX` the offset
// goes to *outX and the direction (0 or 4) to *dir, else to *outY (dir 2 / 6).
RT3_NOINLINE void __fastcall Obj_PlatformOffset(void* o, int, int32_t alongX, float* outX, float* outY, int32_t* dir) {
    int32_t cx = (int32_t)(Obj_CellX(o) * *(const float*)0x5c8568);
    int32_t cy = (int32_t)(Obj_CellY(o) * *(const float*)0x5c859c);
    int32_t m = (cx - cy) & 3;
    float a = 1.4375f, b = 1.4375f;
    if (m == 1) b = -1.4375f;
    else if (m == 2) a = b = -1.4375f;
    else if (m == 3) a = -1.4375f;
    if (alongX) {
        *dir = b < *(const float*)0x5c8598 ? 0 : 4;
        *outX = a;
        *outY = 0.0f;
    } else {
        *dir = a < *(const float*)0x5c8598 ? 6 : 2;
        *outX = 0.0f;
        *outY = b;
    }
}
RT3_REPLACE(0x401300, Obj_PlatformOffset);

// 0x4013f0. Whether a first AI route could be planned at all: with
// territory locks (world +0x4b07) the 24 territories' closed flags (+0x2d)
// are lifted for a test run of AI_PlanFirstRoute(1, 0, 0), then restored.
RT3_NOINLINE int32_t AI_TestFirstRoute() {
    int32_t closed[0x18] = {};
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4b07)) {
        for (uint32_t i = 0; i < 0x18; ++i) {
            if (!Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, i)) continue;
            closed[i] = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, i), 0x2d);
            F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, i), 0x2d) = 0;
        }
    }
    const int32_t ok = AI_PlanFirstRoute(1, 0, 0);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4b07)) {
        for (uint32_t i = 0; i < 0x18; ++i)
            if (Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, i)) F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, i), 0x2d) = closed[i];
    }
    return ok;
}
RT3_REPLACE(0x4013f0, AI_TestFirstRoute);

// 0x4014b0. AI: a company with at least two cargo sites and $300k cash (not in
// a no-industry game, world +0x4abf) may buy an industry. Its threshold is 9%
// return, adjusted by its chairman's personality (table 0x5f2d43, 0x27-byte
// rows) and raised to its best stock return (+0x67.. + 0.01). Candidates are
// unowned, buyable buildings (state +0x3f, not kind 4) at least 3 years old
// that it can afford: the weighted (5, 4, ...) average of up to 5 years of
// profit over the price, x1.1 where it already serves the region, else x0.85.
// The best one above the threshold is bought and announced (string 0xb40,
// log kind 0x11).
RT3_NOINLINE void __fastcall Company_AiBuyIndustry(uint8_t* c) {
    if (Company_CountCargoSites(c, 0, 1) < 2) return;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4abf)) return;
    if (Company_Stat(c, 0, 0xd, 0x2329) < D(0x5c85e0)) return;
    float limit = BitsF(0x3db851ec);
    if (uint8_t* chair = (uint8_t*)Company_Owner(c)) {
        int32_t trait = *(const int32_t*)(0x5f2d43 + (int8_t)chair[0x291] * 0x27);
        float f = (float)((double)(200 - trait) / D(0x5c85d8) + D(0x5c85d0));
        f = (float)((double)f / D(0x5c85c8));
        limit = (float)((double)f * Fl_205(0x5c85c0));
    }
    if (F<int32_t>(c, 0x5b) > 0) {
        int32_t k = Obj_ArgMax67(c);
        double q = (double)F<float>(c, 0x67 + k * 12) + D(0x5c85b8);
        limit = (float)(!(limit > q) ? q : (double)limit);
    }
    float best = limit;
    uint32_t bestPrice = 0, bestId = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b26c)); ++i) {
        Collection* bs = hook::Global<Collection*>(0x62b26c);
        uint8_t* b = (uint8_t*)Collection_Get(bs, 0, Collection_IdAt(bs, 0, (uint32_t)i, 0));
        if (F<int32_t>(b, 0x276)) continue;
        uint8_t* st = ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
        if (!F<int32_t>(st, 0x3f)) continue;
        if (Obj_StateKindIs4(b)) continue;
        float price = Building_Price(b, 0, F<uint32_t>(c, 0));
        if (Company_Stat(c, 0, 0xd, 0x2329) < (double)price) continue;
        int32_t age = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(b, 0x3d5);
        if (age < 3) continue;
        float sum = 0.0f, weights = 0.0f;
        int32_t w = 5;
        for (int32_t j = 0; j < (age > 5 ? 5 : age); ++j) {
            float wf = (float)w;
            weights = (float)((double)wf + weights);
            sum = (float)(Obj_Remaining(b, 0, j) * wf + sum);
            --w;
        }
        float denom = !(Fl_205(0x5c85b4) > weights) ? weights : 1.0f;
        float avg = (float)((double)sum / denom);
        if (avg < Fl_205(0x5c85b0)) continue;
        float div = price > Fl_205(0x5c85b4) ? price : 1.0f;
        float score = (float)((double)avg / div);
        int32_t served = Chain_HasValue(Obj_Region(b), 0, F<int32_t>(c, 0));
        score = (float)((double)score * (served ? D(0x5c85a8) : D(0x5c85a0)));
        if (score > best) {
            best = score;
            bestId = F<uint32_t>(b, 0x2a4);
            bestPrice = *(uint32_t*)&price;
        }
    }
    if (!bestId) return;
    Company_TakeBuilding(c, 0, bestId, *(float*)&bestPrice);
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, bestId);
    uint8_t* st = ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b);
    const char* name = LocalizeName((const char*)st + 0x4f);
    float cy = (float)Obj_CellY(b);
    float cx = (float)Obj_CellX(b);
    const char* city = Table_NearestName(hook::Global<Collection*>(0x62bae0), 0, cx, cy);
    char text[0x200];
    FormatText(text, GetString(0xb40), (const char*)(c + 4), name, city, 0, 0, 0, 0, 0);
    Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x11, F<int32_t>(c, 0), F<int32_t>(b, 0x2a4));
}
RT3_REPLACE(0x4014b0, Company_AiBuyIndustry);

// 0x401860. The company's home station node (+0xd35): kept while it is an
// own station piece (+0x216 == 2), else re-found among the track pieces.
int32_t __fastcall Company_HomeStation(int32_t* c) {
    bool ok = false;
    if (Collection_IsValid(g_trackNodes, 0, F<uint32_t>(c, 0xd35))) {
        uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(c, 0xd35));
        ok = F<uint16_t>(n, 0x22e) == c[0] && n[0x216] == 2;
    }
    if (!ok) F<int32_t>(c, 0xd35) = -1;
    if (F<int32_t>(c, 0xd35) == -1 && F<int32_t>(c, 0x766c) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664) > 0) {
        for (int32_t i = 0; i < Collection_Count(g_trackNodes); ++i) {
            uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, Collection_IdAt(g_trackNodes, 0, i, 0));
            if (F<uint16_t>(n, 0x22e) == c[0] && n[0x216] == 2) {
                F<int32_t>(c, 0xd35) = F<int32_t>(n, 0x202);
                break;
            }
        }
    }
    return F<int32_t>(c, 0xd35);
}
RT3_REPLACE(0x401860, Company_HomeStation);

// 0x401940. AI company c (id +0): clears +0xd3a / +0xd3e, refreshes its
// station links and re-plans its trains (from the last): each one runs
// (mode 2); one stopped for lack of a route (state 10 or 19) loses all its
// orders; one without orders gets a new one (AiTrain_NextOrder).
RT3_NOINLINE void __fastcall AiCompany_ReplanTrains(int32_t* c) {
    F<int32_t>(c, 0xd3e) = 0;
    F<int32_t>(c, 0xd3a) = 0;
    Company_UpdateStationLinks(c);
    for (int32_t i = Collection_Count(hook::Global<Collection*>(0x6cfcbc)) - 1; i >= 0; --i) {
        Collection* trains = hook::Global<Collection*>(0x6cfcbc);
        uint8_t* t = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0));
        if (F<int32_t>(t, 0x51) != c[0]) continue;
        Train_SetRunning(t, 0, 2);
        if (F<int32_t>(t, 0xe8) == 0xa || F<int32_t>(t, 0xe8) == 0x13)
            while (F<int32_t>(t, 4)) TrainOrders_Remove(t, 0, 0);
        if (!F<int32_t>(t, 4)) AiTrain_NextOrder(t);
    }
}
RT3_REPLACE(0x401940, AiCompany_ReplanTrains);

// 0x4019e0. AI company co's turn (+0x3f: computer run; none once its owner
// flag 0x425b90 is set). Its flags +0xd18 / +0xd17 / +0xd56 clear, +0xd17
// (electric, from the engine type 0x4078a0) stands in for the world's
// +0x4c74 during the turn. Unless the editor or world rules stop the AI
// (+0x29f, +0x4ab3, +0x4aa3, +0x4aa7), when its wait (+0xd1d) is over it
// waits SimRand(180, 360) x 1440 minutes again (x (300 - personality) / 200,
// or twice that without an owner) and builds: its first route without cargo
// sites (nothing more this turn when it still has none), else an expansion
// (on failure 0x1fa40 minutes more). Every 0x9d80 minutes (or after a build)
// it reviews its trains (0x409950); in November it doubles track, from
// November on (unless +0x2a7 / +0x4ac3) it may buy industry every 0x3b100
// minutes, and it does its year's finance. Then it rates its stations, in
// January linking them first.
RT3_NOINLINE void __fastcall AI_CompanyTurn(uint8_t* co) {
    if (!F<int32_t>(co, 0x3f)) return;
    const int32_t gone = Company_OwnerFlag1e1(co);
    if (gone) return;
    co[0xd18] = (uint8_t)gone, co[0xd17] = (uint8_t)gone, co[0xd56] = (uint8_t)gone;
    const int32_t engine = (int32_t)AI_PickEngineType(*(uint32_t*)co);
    if (engine > 0 && F<int32_t>(RtGet(0x6ada84, engine), 0x10) == 2) co[0xd17] = 1;
    auto world = [] { return hook::Global<uint8_t*>(0x6cec78); };
    const int32_t electric = F<int32_t>(world(), 0x4c74);
    F<int32_t>(world(), 0x4c74) = co[0xd17];
    const bool acted = [&] {
        int32_t built = 0;
        uint8_t* ed = hook::Global<uint8_t*>(0x6cec74);
        if (!F<int32_t>(ed, 0x29f) && !F<int32_t>(world(), 0x4ab3) && !F<int32_t>(world(), 0x4aa3) && !F<int32_t>(world(), 0x4aa7) &&
            F<int32_t>(co, 0xd1d) < F<int32_t>(world(), 0x15)) {
            const int32_t wait = SimRand(0xb4, 0x168) * 0x5a0;
            uint8_t* owner = (uint8_t*)Company_Owner(co);
            const int32_t delay = owner ? Ftol((double)(0x12c - hook::Global<int32_t>(0x5f2d3f + 0x27 * (int8_t)owner[0x291])) / D(0x5c85e8) * (double)wait)
                                        : wait * 2;
            F<int32_t>(co, 0xd1d) = F<int32_t>(world(), 0x15) + delay;
            if (Company_CountCargoSites(co, 0, 1) < 1) {
                AI_PlanFirstRoute(0, F<int32_t>(co, 0x3b), *(int32_t*)co);
                if (Company_CountCargoSites(co, 0, 1) < 1) return false;
                F<int32_t>(co, 0xd25) = 0;
            } else if (AI_PlanExpansion(co)) {
                built = 1;
                F<int32_t>(co, 0xd25) = 0;
            } else {
                F<int32_t>(co, 0xd1d) += 0x1fa40;
            }
        }
        if (!F<int32_t>(co, 0xd25) || F<int32_t>(co, 0xd25) + 0x9d80 <= F<int32_t>(world(), 0x15)) {
            F<int32_t>(co, 0xd25) = F<int32_t>(world(), 0x15);
            AI_ReviewTrains(co, 0, built);
        }
        if (world()[0xf] == 0xb) TrackNet_AiDoubleTrack(hook::Global<Collection*>(0x6cfca8), 0, *(uint32_t*)co);
        if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x2a7) && !F<int32_t>(world(), 0x4ac3) && world()[0xf] >= 0xb &&
            (!F<int32_t>(co, 0xd29) || F<int32_t>(co, 0xd29) + 0x3b100 < F<int32_t>(world(), 0x15))) {
            Company_AiBuyIndustry(co);
            F<int32_t>(co, 0xd29) = F<int32_t>(world(), 0x15);
        }
        Company_YearFinance(co);
        return true;
    }();
    (void)acted;
    if (world()[0xf] == 1) Company_BuildStationLinks((int32_t*)co);
    else Company_RateStations((int32_t*)co);
    F<int32_t>(world(), 0x4c74) = electric;
}
RT3_REPLACE(0x4019e0, AI_CompanyTurn);

// 0x401c50. A computer company's yearly money decisions (December):
//  - bankrupt when deep in debt, losing money and its stock is cheap;
//  - refinance: on a copy of the company it tries repaying its n dearest
//    bonds with new $500K ones for each n and keeps the best saving;
//  - borrow more while cash is short (repeating while the refinance asks);
//  - repay bonds of 6% and more while cash covers them, then buy back stock
//    in lots of 1000 while cash is plenty (owner's AI bonus +0x5f2d37);
//  - else possibly go bankrupt after years of losses, or issue stock when its
//    bonds are dear and the price is good;
//  - set the dividend from the last three years' profit;
// and puts what it did in the news (0x6acd34).
RT3_NOINLINE void __fastcall Company_YearFinance(uint8_t* c) {
    auto cur = [] { return (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd); };
    auto world = [] { return hook::Global<uint8_t*>(0x6cec78); };
    auto stat = [&](int32_t id, int32_t year) { return Company_Stat(c, 0, id, year); };
    auto news = [&](const char* text) { Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x12, F<int32_t>(c, 0), 0); };
    char text[0x1f8], msg[0x3f0], numA[0x1f8], numB[0x1f8];
    int32_t issuedCount = 0;
    float best = 0.0f;
    int32_t bestN = 0;
    if (Game_EffectiveVersion() >= 0x67 && F<int32_t>(world(), 0x4a8f) == 0 && world()[0xf] == 0xc &&
        cur() - F<int32_t>(c, 0x163) >= 0xd && cur() - F<uint16_t>(c, 0x157) >= 4) {
        int32_t lossYears = 0;
        float sum = 0.0f, peak = 0.0f;
        for (int32_t i = 0; i < 3; ++i) {
            if (stat(0x2b, cur() - i) < D(0x5c86f8)) ++lossYears;
            double p = peak;
            double v = stat(0x2c, cur() - i) < p ? (double)peak : stat(0x2c, cur() - i);
            peak = (float)v;
            sum = (float)(stat(0x2b, cur() - i) + sum);
        }
        float cap;
        if (peak < Fl_205(0x5c86f0))
            cap = BitsF(0xc9127c00u);
        else if (peak < Fl_205(0x5c86ec))
            cap = BitsF(0xc9864700u);
        else {
            cap = BitsF(0xc9c35000u);
            if (!(peak < Fl_205(0x5c86e8))) cap = BitsF(0xc9f42400u);
        }
        double cash = stat(0xd, 0x2329);
        if (stat(0x12, 0x2329) + cash < cap) {
            float price = Company_SharePrice(c, 0, 0, 0);
            float lim = (float)(int32_t)((((lossYears != 3) - 1) & 5) + 0xf);
            if (price <= lim && (double)cap * D(0x5c86e0) >= stat(9, 0x2329) && lossYears >= 2 && sum <= Fl_205(0x5c86d8)) {
                Company_GoBankrupt(c);
                FormatText(msg, GetString(0xb41), (const char*)c + 4, 0, 0, 0, 0, 0, 0, 0);
                news(msg);
                return;
            }
        }
    }
    uint8_t* backup = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)(0x7684);
    memcpy(backup, c, 0x7684);
    for (int32_t n = 1; n <= F<int32_t>(backup, 0x5b); ++n) {
        float paid = 0.0f, owed = 0.0f;
        for (int32_t k = n; k > 0; --k) {
            int32_t i = Obj_ArgMax67(c);
            float amt = (float)BondAmount(c, i);
            paid = (float)((double)amt * BondRate(c, i) + paid);
            Company_RemoveEntry(c, 0, i, 1);
        }
        bool ok = true;
        for (int32_t k = 0; k < n; ++k) {
            if (!Company_IssueBond(c, 0, 500000, 0x1e, 1, 0.0f)) {
                ok = false;
                break;
            }
            int32_t last = F<int32_t>(c, 0x5b) - 1;
            float amt = (float)BondAmount(c, last);
            owed = (float)((double)amt * BondRate(c, last) + owed);
        }
        if (ok) {
            float saved = paid - owed;
            double dn = (double)n;
            float score = (float)(saved - dn * D(0x5c86d0));
            if (score > best) {
                best = score;
                bestN = n;
            }
        }
        memcpy(c, backup, 0x7684);
    }
    ((void(__cdecl*)(void*))0x5a1145)(backup);
    int32_t refinance = F<int32_t>(world(), 0x4a8b) ? 0 : bestN;
    int32_t repaid = 0, issued = 0, bought = 0, bankrupt = 0, repaidCount = 0;
    if (refinance > 0) {
        for (int32_t k = refinance; k > 0; --k) {
            int32_t i = Obj_ArgMax67(c);
            int32_t a = BondAmount(c, i);
            repaid += a < 0 ? -a : a;
            Company_RemoveEntry(c, 0, i, 1);
        }
        repaidCount = refinance;
    }
    if (!F<int32_t>(world(), 0x4a8b)) {
        int32_t want = refinance;
        for (;;) {
            bool need = stat(0xd, 0x2329) < D(0x5c86c8);
            if (!need) need = stat(0xd, 0x2329) < D(0x5c86c0) && c[0xd56];
            if (!need && want <= 0) break;
            issued += 500000;
            int32_t r = Company_IssueBond(c, 0, 500000, 0x1e, 1, 0.0f);
            if (r) ++issuedCount;
            --want;
            if (!r) break;
        }
    }
    {
        int32_t payoff = c[0xd18];
        int32_t keep = 0x86470;
        if (F<int32_t>(c, 0x5b) > 0) {
            int32_t i = Obj_ArgMax67(c);
            if (BondRate(c, i) >= D(0x5c86b8)) payoff = 1;
            if (Game_EffectiveVersion() >= 0x67) {
                int32_t v = BondAmount(c, i) + 0x3a98;
                if (v <= 0x86470) keep = v;
            }
        }
        if (!F<int32_t>(world(), 0x4a8b) && payoff) {
            double thr = (double)keep;
            while (stat(0xd, 0x2329) > thr && F<int32_t>(c, 0x5b)) {
                int32_t i = Obj_ArgMax67(c);
                if (BondRate(c, i) < D(0x5c86b0)) break;
                int32_t a = BondAmount(c, i);
                repaid += a < 0 ? -a : a;
                Company_RemoveEntry(c, 0, i, 1);
                ++repaidCount;
            }
        }
    }
    if (c[0xd18]) {
        for (;;) {
            float f = 1.0f;
            uint8_t* own = (uint8_t*)Company_Owner(c);
            if (own) {
                double v = (double)(hook::Global<int32_t>(0x5f2d37 + (int8_t)own[0x291] * 0x27) + 300);
                f = (float)(v / D(0x5c86a8));
            }
            int32_t mode = F<int32_t>(world(), 0x4c7c);
            if (mode == 1) f = (float)(f * D(0x5c8570));
            if (mode == 2 || F<int32_t>(world(), 0x4a87)) break;
            if (!((double)f * Fl_205(0x5c86a4) < stat(0xd, 0x2329))) break;
            double cash = stat(0xd, 0x2329);
            if (!((double)Company_SharePrice(c, 0, 0, 0) * f * Fl_205(0x5c86a0) * D(0x5c8698) < cash)) break;
            if (Company_FreeShares(c) < 1000) break;
            Company_BuyBackShares(c, 0, 1000);
            bought += 1000;
        }
    }
    if (!issuedCount && !repaidCount && !bought) {
        int32_t age = cur() - F<uint16_t>(c, 0x157);
        if (!F<int32_t>(world(), 0x4a8f) && stat(0xd, 0x2329) < D(0x5c8690) && age >= 3) {
            int32_t recent = 0;
            for (int32_t i = 0; i < (age > 5 ? 5 : age); ++i)
                if (stat(0x2b, cur() - i - 1) > D(0x5c8688) && i < 3) recent = 1;
            if (!recent && cur() - F<int32_t>(c, 0x163) >= 5) {
                Company_GoBankrupt(c);
                FormatText(text, GetString(0xb41), (const char*)c + 4, 0, 0, 0, 0, 0, 0, 0);
                news(text);
                bankrupt = 1;
            }
        }
        if (Game_EffectiveVersion() >= 0x67 && !bankrupt && !F<int32_t>(world(), 0x4a8b) && !F<int32_t>(world(), 0x4a87) && F<int32_t>(c, 0x5b) > 1 &&
            cur() - F<uint16_t>(c, 0x157) >= 1) {
            int32_t n = F<int32_t>(c, 0x47) / 10 / 1000 * 1000;
            if (n < 2000) n = 2000;
            for (;;) {
                float price = Company_SharePrice(c, 0, 0, 0);
                float nf = (float)n;
                if (!((double)price * nf > Fl_205(0x5c86a4))) {
                    if (n < 2000) n = 2000;
                    break;
                }
                n -= 1000;
                if (n < 2000) {
                    n = 2000;
                    break;
                }
            }
            double dn = (double)-n;
            float np = Company_SharePrice(c, 0, Ftol(dn * D(0x5c8680)), 0);
            float eps = (float)stat(0x1d, 0x2329);
            if (Fl_205(0x5c85b4) > eps) eps = 1.0f;
            float ratio = np / eps;
            int32_t i = Obj_ArgMax67(c);
            float rate = (float)(BondRate(c, i) + D(0x5c8678));
            float nf = (float)n;
            float total = nf * np;
            if (np >= Fl_205(0x5c8674) && total >= Fl_205(0x5c8670)) {
                double cash = stat(0xd, 0x2329);
                if (cash < (double)(BondAmount(c, i) + 0x1388) &&
                    F<int32_t>(world(), 0x15) - DateToMinutes((const GameDate*)(c + 0x16b)) >= 0x76200) {
                    static const uint32_t ladder[][2] = { { 0x5c8668, 0x5c8660 }, { 0x5c86e0, 0x5c8698 }, { 0x5c8658, 0x5c85a8 }, { 0x5c86b8, 0x5c8650 },
                                                          { 0x5c8648, 0x5c8640 }, { 0x5c8590, 0x5c8638 }, { 0x5c8630, 0x5c8628 }, { 0x5c8620, 0x5c8618 } };
                    bool go = false;
                    for (auto& l : ladder)
                        if (rate >= D(l[0]) && ratio >= D(l[1])) {
                            go = true;
                            break;
                        }
                    if (go) {
                        int32_t at = Ftol(np);
                        int32_t m = Ftol((double)n * D(0x5c8628));
                        Company_IssueShares(c, 0, m, 0, at);
                        Company_IssueShares(c, 0, m, 0, at);
                        Num_Format(text, 1, (double)n, 10, 1, 0, 0);
                        Num_FormatSimple2(numB, 0xb, (double)total, 0xb);
                        FormatText(msg, GetString(0xfd5), (const char*)c + 4, text, numB, 0, 0, 0, 0, 0);
                        news(msg);
                    }
                }
            }
        }
    }
    if (!F<int32_t>(world(), 0x4a93) && world()[0xf] == 0xc && cur() - F<int32_t>(c, 0xd2d) >= 1 && cur() - F<uint16_t>(c, 0x157) >= 2) {
        F<int32_t>(c, 0xd2d) = cur();
        float profit = 0.0f;
        for (int32_t k = 3, i = 0; k > 0; --k, ++i) {
            double w = (double)k;
            profit = (float)(stat(0x2b, cur() - i) * w + profit);
        }
        profit = (float)(profit / D(0x5c8610));
        float per;
        if (Company_FreeShares(c) <= 1000) {
            double cash = stat(0xd, 0x2329) < D(0x5c8608) ? 0.0 : stat(0xd, 0x2329);
            per = (float)(((double)profit / D(0x5c85c8) + cash / D(0x5c85c8)) / (double)F<int32_t>(c, 0x47));
        } else
            per = (float)((double)profit / D(0x5c8610) / (double)F<int32_t>(c, 0x47));
        int32_t mode = F<int32_t>(world(), 0x4c7c);
        if (mode == 1) F<float>(c, 0x4b) = (float)(F<float>(c, 0x4b) * D(0x5c8600));
        else if (mode == 2) F<int32_t>(c, 0x4b) = 0;
        if (per < D(0x5c86b8))
            F<int32_t>(c, 0x4b) = 0;
        else {
            float v = (float)((((double)F<float>(c, 0x4b) + per) + D(0x5c86b8)) / D(0x5c85d0));
            F<float>(c, 0x4b) = (float)((double)Ftol((double)v * Fl_205(0x5c85f8)) / D(0x5c85f0));
        }
        if (!(Company_MaxDividend(c) > F<float>(c, 0x4b))) F<float>(c, 0x4b) = Company_MaxDividend(c);
    }
    if (bankrupt) return;
    if (!repaid && !issued && !bought) return;
    if (hook::Global<uint8_t*>(0x6cfe4c) && Screen90() == F<int32_t>(c, 0)) CompanyWindow_Refresh(hook::Global<uint8_t*>(0x6cfe4c));
    text[0] = 0;
    const char* name = (const char*)c + 4;
    if (repaid) {
        if (!issued)
            FormatText(text, GetString(0xb45), name, Num_FormatSimple(numA, 9, (double)repaid, 0xb), 0, 0, 0, 0, 0, 0);
        else if (issued == repaid)
            FormatText(text, GetString(0xb42), name, Num_FormatSimple(numA, 9, (double)repaid, 0xb), 0, 0, 0, 0, 0, 0);
        else if (issued > repaid) {
            char* d = Num_FormatSimple(msg, 9, (double)(issued - repaid), 0xb);
            FormatText(text, GetString(0xb43), name, Num_FormatSimple(numA, 9, (double)repaid, 0xb), d, 0, 0, 0, 0, 0);
        } else {
            char* d = Num_FormatSimple(msg, 9, (double)(repaid - issued), 0xb);
            FormatText(text, GetString(0xb44), name, Num_FormatSimple(numA, 9, (double)issued, 0xb), d, 0, 0, 0, 0, 0);
        }
    } else if (issued)
        FormatText(text, GetString(0xb46), name, Num_FormatSimple(numA, 9, (double)issued, 0xb), 0, 0, 0, 0, 0, 0);
    if (strlen(text) > 1) news(text);
    if (bought) {
        FormatText(text, GetString(0xb47), name, Num_Format(numA, 1, (double)bought, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        news(text);
    }
}
RT3_REPLACE(0x401c50, Company_YearFinance);

// 0x402c00. Slope angle atan((h(a) - h(b)) / distance) between two tile points
// (distance in world units, 0.01 when the points coincide).
RT3_NOINLINE float __stdcall Map_SlopeAngle(float ax, float ay, float bx, float by) {
    float ha = (float)Terrain_HeightAtTile(Map(), 0, ax, ay);
    float hb = (float)Terrain_HeightAtTile(Map(), 0, bx, by);
    float d = (float)(Distance2D(ax, ay, bx, by) * *(const float*)0x5c8700);
    if (d == *(const float*)0x5c8598) *(uint32_t*)&d = 0x3c23d70a;
    return (float)((double(__cdecl*)(double))0x5a1270)(((double)ha - hb) / d);
}
RT3_REPLACE(0x402c00, Map_SlopeAngle);

// 0x402c90 / 0x40cee0 / 0x40ccb0 (vtables). Train-related lookups.
RT3_NOINLINE void* __fastcall Obj_Station173(uint8_t* o) { return Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(o, 0x173)); }
RT3_REPLACE(0x402c90, Obj_Station173);

 // namespace

// 0x402cb0 (thiscall on the track network, RET 0x24). The AI lays a route
// for `company` between two ends; end k is the piece pieceK or, when that is
// -1, a new station site at city cityK of building type typeK (StationSml /
// Med / Lrg, ServiceTower, Maintenance; -1 none). Returns whether it was
// built; *outCost gets its cost (at least 1, 0 when it failed). In
// `estimate` mode nothing stays: the work is undone (from the stamp
// 0x4a3d00 pushes) and only the cost counts.
//  - A city end steps off the city towards the other end (the side that
//    differs most) by its platform offset (0x401300) and lays a stub there
//    from the city's rating radius along that side (0x624b28 / 0x624b48).
//    A piece end near a city (closer than 0.8 x its rating, with an open
//    end) whose heading points nearly at (< 0.1) or away from (> pi - 0.1)
//    the city first gets a stub around it (rating -+ distance, at least 2).
//  - Then the two ends are joined: closer than 12 directly (from the higher
//    end); up to 20 through a midpoint that follows both headings (fraction
//    0.25..0.75 by how much each turns, pushed 3 out along them); further
//    through the best of the points 30..70 % along (on plain cells, their
//    height off the straight grade - weight + 50, times (1 + slope)^3 where
//    the slope stays <= 0.52). Each half goes from the end nearer the
//    midpoint (or the lower one when both halves exceed 7).
//  - Station sites: the end's terrain style + type name ("%1%2", e.g.
//    ClpBrdStationSml) is looked up, turned by the end's side, snapped to a
//    piece within 0.724 and checked (0x4197e0); the estimate adds its
//    price x effect 0x3b minus the city's +0x276. A build pass then places
//    them (0x4134d0 / Building_Place).
//  - A built route walks its pieces from end to end (up to 999, newer than
//    the stamp) and spaces Maintenance sheds (length / 2.8 / 47) and
//    ServiceTowers (/ 30, before 1950 / 37) along it, each the nearest free
//    piece within 100 of its slot (alternating sides, sheds -0.15 / towers
//    +0.05 of a gap off when the counts match on routes over 35); a placed
//    one blocks the pieces 4 around it.
// Afterwards a build resets the build state, links the company's stations
// and relays the touched rectangle; a failed or estimated one is undone.
// QUIRK: an end given as a piece keeps the position, side and direction
// slots unset, so a station type there reads stale stack; the terrain style
// is looked up at (x, x) of the end.
RT3_NOINLINE int32_t __fastcall AI_BuildRoute(Collection* net, int, int32_t cityA, int32_t pieceA, int32_t typeA, int32_t cityB, int32_t pieceB, int32_t typeB,
                                 int32_t estimate, float* outCost, int32_t company) {
    uint8_t* const t = (uint8_t*)net;
    RouteFrame fr;
    memset(fr.b, 0, sizeof fr.b);
    t[0x10a] = 1;
    Track_Validate(net);
    t[0x10a] = 0;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c84) == 0) {
        F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c84) = 1;
        Game_PauseTools();
    }
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x140) = 2;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x13c) = 2;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x138) = 3;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x144) = 1;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c74) = 0;
    if (Game_EffectiveVersion() >= 0x67) {
        const int32_t engine = (int32_t)AI_PickEngineType((uint32_t)company);
        if (engine > 0 && F<int32_t>(RtGet(0x6ada84, engine), 0x10) == 2) F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c74) = 1;
    }
    Obj_Stash98(t);
    ++F<int32_t>(t, 0xe9);
    if (estimate) {
        ++F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114256);
        ++F<int32_t>(t, 0xed);
    }
    F<uint16_t>(t, 0xf6) = 9999;
    F<uint16_t>(t, 0xf8) = 9999;
    fr.i(0x40) = cityA, fr.i(0x44) = cityB;
    fr.i(0x74) = pieceA, fr.i(0x78) = pieceB;
    fr.i(0x90) = fr.i(0x94) = fr.i(0xb8) = fr.i(0xbc) = -1;
    fr.i(0xc0) = typeA, fr.i(0xc4) = typeB;
    t[0xf5] = 1;
    F<uint16_t>(t, 0xfa) = 0;
    F<uint16_t>(t, 0xfc) = 0;
    const int32_t mode = estimate == 0;
    fr.i(0x7c) = Obj_PushIdNode(hook::Global<Collection*>(0x6cfca8), 0, 0, company);
    for (uint32_t o = 0; o < 8; o += 4) fr.p(0xac + o) = fr.i(0x40 + o) ? RtGet(0x62bae0, fr.i(0x40 + o)) : nullptr;
    auto lay = [&](float x0, float y0, float x1, float y1, int32_t* from, int32_t* to) {
        return TrackNet_LayLine(hook::Global<Collection*>(0x6cfca8), 0, x0, y0, x1, y1, company, mode, 0, from, to, RtBits(1));
    };
    // 0: went through the station pass (cost kept at least 1), 1: a line
    // failed (cost 1, then 0).
    const int ended = [&]() -> int {
        for (uint32_t o = 0; o < 8; o += 4) {
            if (fr.i(0x74 + o) != -1) {
                fr.i(0x90 + o) = fr.i(0x74 + o);
                uint8_t* pc = RtGet(0x6cfca8, fr.i(0x74 + o));
                const int32_t py = Ftol(Obj_CenterCellY(pc));
                const int32_t px = Ftol(Obj_CenterCellX(pc));
                const uint16_t region = Regions_NearestValue(px, py);
                if (!region) continue;
                uint8_t* city = RtGet(0x62bae0, region);
                fr.p(0x38) = city;
                fr.f(0x18) = Obj_Rating296(city);
                const int32_t qy = Ftol(Obj_CenterCellY(pc));
                const int32_t qx = Ftol(Obj_CenterCellX(pc));
                fr.i(0x1c) = Ftol(Obj_CellY(city));
                const int32_t cx = Ftol(Obj_CellX(city));
                fr.i(0x1c) = (uint16_t)DistanceInt(cx, fr.i(0x1c), qx, qy);
                fr.f(0x20) = (float)fr.i(0x1c);
                if (!((double)fr.f(0x18) * D(0x5c8640) > fr.f(0x20))) continue;
                if (F<int32_t>(pc, 0x206) != -1 && F<int32_t>(pc, 0x20a) != -1) continue;
                fr.f(0x14) = Vehicle_ScreenHeading(pc, 0, 1);
                int32_t nb = F<int32_t>(pc, 0x206);
                if (nb == -1) nb = F<int32_t>(pc, 0x20a);
                if (nb != -1) {
                    uint8_t* next = RtGet(0x6cfca8, nb);
                    const float ny = Obj_CenterCellY(next), nx = Obj_CenterCellX(next);
                    const float my = Obj_CenterCellY(pc), mx = Obj_CenterCellX(pc);
                    fr.f(0x14) = (float)AngleOf(mx, my, nx, ny);
                }
                {
                    const float ty = (float)Obj_CellY(city), tx = (float)Obj_CellX(city);
                    const float my = Obj_CenterCellY(pc), mx = Obj_CenterCellX(pc);
                    fr.f(0x1c) = (float)AngleOf(mx, my, tx, ty);
                }
                fr.f(0x10) = (float)RtAbs(AngleDelta(nullptr, 0, fr.f(0x1c), fr.f(0x14)));
                const float turn = fr.f(0x10);
                if (!(turn < D(0x5c86b8)) && !(turn > D(0x5c8790))) continue;
                fr.f(0x10) = turn > D(0x5c8790) ? (float)((double)fr.f(0x20) + fr.f(0x18)) : (float)((double)fr.f(0x18) - fr.f(0x20));
                if (Fl_207(0x5c878c) > fr.f(0x10)) fr.i(0x10) = 0x40000000;
                fr.d(0x40) = RtSin(fr.f(0x14)) * fr.f(0x10);
                fr.f(0x38) = (float)(Obj_CenterCellX(pc) + fr.d(0x40));
                fr.d(0x40) = Obj_CenterCellY(pc);
                fr.f(0x1c) = (float)(fr.d(0x40) - RtCos(fr.f(0x14)) * fr.f(0x10));
                fr.i(0x28) = -1;
                fr.i(0x0c) = lay(0.0f, 0.0f, fr.f(0x38), fr.f(0x1c), &fr.i(0x74 + o), &fr.i(0x28));
                if (!fr.i(0x0c)) return 1;
                fr.i(0x90 + o) = fr.i(0x74 + o);
                fr.i(0x74 + o) = fr.i(0x28);
                continue;
            }
            if (fr.i(0x78 - o) != -1) {
                Obj_CenterCell(RtGet(0x6cfca8, fr.i(0x78 - o)), 0, &fr.f(0x5c), &fr.f(0x60));
            } else {
                uint8_t* other = fr.p(0xb0 - o);
                fr.f(0x5c) = (float)Obj_CellX(other);
                fr.f(0x60) = (float)Obj_CellY(other);
            }
            uint8_t* city = fr.p(0xac + o);
            fr.f(0x10) = (float)Obj_CellX(city);
            fr.f(0x14) = (float)Obj_CellY(city);
            fr.f(0x88 + o) = fr.f(0x10);
            fr.f(0x48) = (float)((double)fr.f(0x5c) - fr.f(0x10));
            fr.f(0xa4 + o) = fr.f(0x14);
            fr.f(0x18) = (float)((double)fr.f(0x60) - fr.f(0x14));
            fr.d(0x40) = RtAbs(fr.f(0x48));
            int32_t side, alongX;
            if (RtAbs(fr.f(0x18)) < fr.d(0x40)) side = fr.f(0x48) > Fl_207(0x5c8598) ? 2 : 6, alongX = 1;
            else side = fr.f(0x18) > Fl_207(0x5c8598) ? 4 : 0, alongX = 0;
            Obj_PlatformOffset(city, 0, alongX, &fr.f(0x24), &fr.f(0x2c), &fr.i(0xb8 + o));
            fr.f(0x88 + o) = (float)((double)fr.f(0x24) + fr.f(0x10));
            fr.f(0xa4 + o) = (float)((double)fr.f(0x2c) + fr.f(0x14));
            fr.f(0x0c) = Obj_Rating296(city);
            double reach = fr.f(0x0c);
            if (side == 2) reach -= fr.f(0x24);
            else if (side == 6) reach += fr.f(0x24);
            else if (side == 4) reach -= fr.f(0x2c);
            else reach += fr.f(0x2c);
            fr.f(0x0c) = (float)reach;
            fr.i(0x48) = hook::Global<int32_t>(0x624b28 + 4 * side);
            fr.i(0x10) = hook::Global<int32_t>(0x624b48 + 4 * side);
            fr.f(0x1c) = (float)fr.i(0x48);
            fr.i(0x74 + o) = -1;
            fr.f(0x80) = (float)((double)fr.f(0x1c) * fr.f(0x0c) + fr.f(0x88 + o));
            fr.i(0x98 + o) = side;
            fr.f(0x1c) = (float)fr.i(0x10);
            fr.f(0x54) = (float)((double)fr.f(0x1c) * fr.f(0x0c) + fr.f(0xa4 + o));
            fr.d(0x40) = (double)fr.i(0x48);
            fr.f(0x38) = (float)(fr.f(0x88 + o) - fr.d(0x40));
            fr.d(0x40) = (double)fr.i(0x10);
            fr.f(0x1c) = (float)(fr.f(0xa4 + o) - fr.d(0x40));
            fr.i(0x0c) = lay(fr.f(0x80), fr.f(0x54), fr.f(0x38), fr.f(0x1c), &fr.i(0x74 + o), &fr.i(0x90 + o));
            if (!fr.i(0x0c)) return 1;
        }
        // Join the ends.
        uint8_t* end0 = RtGet(0x6cfca8, fr.i(0x74));
        uint8_t* end1 = RtGet(0x6cfca8, fr.i(0x78));
        Obj_CenterCell(end0, 0, &fr.f(0x50), &fr.f(0x4c));
        Obj_CenterCell(end1, 0, &fr.f(0x58), &fr.f(0x64));
        fr.f(0x10) = (float)Distance2D(fr.f(0x50), fr.f(0x4c), fr.f(0x58), fr.f(0x64));
        int32_t* from;
        if (fr.f(0x10) < Fl_207(0x5c8788)) {
            fr.f(0x1c) = (float)Terrain_HeightAtTile(RtMap(), 0, fr.f(0x50), fr.f(0x4c));
            const int e = Terrain_HeightAtTile(RtMap(), 0, fr.f(0x58), fr.f(0x64)) > fr.f(0x1c) ? 0 : 1;
            fr.i(0x0c) = lay(0.0f, 0.0f, 0.0f, 0.0f, &fr.i(0x74 + 4 * e), &fr.i(0x78 - 4 * e));
            if (!fr.i(0x0c)) return 1;
        } else {
            fr.i(0x24) = 0x3f000000;
            if (fr.f(0x10) > Fl_207(0x5c8784)) {
                fr.i(0x20) = 0x4b18967f;
                fr.f(0x28) = (float)Terrain_HeightAtTile(RtMap(), 0, fr.f(0x50), fr.f(0x4c));
                fr.f(0x1c) = (float)Terrain_HeightAtTile(RtMap(), 0, fr.f(0x58), fr.f(0x64));
                fr.i(0x2c) = 0x1e;
                fr.f(0x1c) = (float)((double)fr.f(0x1c) - fr.f(0x28));
                fr.f(0x48) = (float)((double)fr.f(0x58) - fr.f(0x50));
                fr.f(0x5c) = (float)((double)fr.f(0x64) - fr.f(0x4c));
                fr.f(0x60) = (float)(D(0x5c8578) / fr.f(0x10));
                for (int32_t w = 0x14; w >= -0x14; w -= 5, fr.i(0x2c) += 5) {
                    fr.d(0x68) = (double)fr.i(0x2c);
                    fr.f(0x18) = (float)(fr.d(0x68) / D(0x5c85d8));
                    fr.f(0x14) = (float)((double)fr.f(0x18) * fr.f(0x48) + fr.f(0x50));
                    fr.f(0x34) = (float)((double)fr.f(0x18) * fr.f(0x5c) + fr.f(0x4c));
                    const int32_t cy = Ftol(fr.f(0x34));
                    const int32_t cx = Ftol(fr.f(0x14));
                    if (Map_CellLow3(RtMap(), 0, cx, cy)) continue;
                    fr.f(0x38) = (float)Terrain_HeightAtTile(RtMap(), 0, fr.f(0x14), fr.f(0x34));
                    fr.f(0x54) = (float)((double)fr.f(0x18) * fr.f(0x1c) + fr.f(0x28));
                    fr.f(0x0c) = (float)RtAbs((double)fr.f(0x38) - fr.f(0x54));
                    fr.i(0x38) = 0x14 - (w < 0 ? -w : w);
                    fr.d(0x68) = (double)fr.i(0x38);
                    fr.f(0x0c) = (float)(fr.f(0x0c) - fr.d(0x68));
                    fr.f(0x0c) = (float)((double)fr.f(0x0c) + Fl_207(0x5c8780));
                    if (!(fr.f(0x0c) < fr.f(0x20))) continue;
                    const float by = (float)((double)fr.f(0x60) * fr.f(0x5c) + fr.f(0x34));
                    const float bx = (float)((double)fr.f(0x60) * fr.f(0x48) + fr.f(0x14));
                    fr.f(0x14) = Map_SlopeAngle(fr.f(0x14), fr.f(0x34), bx, by);
                    fr.f(0x14) = (float)RtAbs(fr.f(0x14));
                    if (RtAbs(fr.f(0x14)) > D(0x5c8778)) continue;
                    fr.d(0x68) = (double)fr.f(0x14) + D(0x5c8770);
                    fr.d(0x40) = fr.d(0x68);
                    fr.f(0x0c) = (float)(fr.f(0x0c) * fr.d(0x40) * fr.d(0x40) * fr.d(0x68));
                    if (fr.f(0x0c) < fr.f(0x20)) fr.i(0x20) = fr.i(0x0c), fr.i(0x24) = fr.i(0x18);
                }
            } else {
                const double tau = Fl_207(0x5c8768), pi = Fl_207(0x5c8760);
                fr.f(0x20) = Vehicle_ScreenHeading(end0, 0, 1);
                fr.f(0x2c) = Vehicle_ScreenHeading(end1, 0, 1);
                fr.f(0x0c) = (float)AngleOf(fr.f(0x50), fr.f(0x4c), fr.f(0x58), fr.f(0x64));
                fr.f(0x20) = (float)RtMod((double)fr.f(0x20) + Fl_207(0x5c8768), tau);
                fr.f(0x2c) = (float)RtMod((double)fr.f(0x2c) + Fl_207(0x5c8768), tau);
                fr.f(0x0c) = (float)RtMod((double)fr.f(0x0c) + Fl_207(0x5c8768), tau);
                const float line = fr.f(0x0c);
                fr.f(0x18) = line > fr.f(0x20) ? (float)((double)line - fr.f(0x20)) : (float)((double)fr.f(0x20) - line);
                fr.f(0x14) = line > fr.f(0x2c) ? (float)((double)line - fr.f(0x2c)) : (float)((double)fr.f(0x2c) - line);
                fr.f(0x18) = (float)RtMod(fr.f(0x18), pi);
                fr.f(0x14) = (float)RtMod(fr.f(0x14), pi);
                if (fr.f(0x18) > Fl_207(0x5c8764)) fr.f(0x18) = (float)(Fl_207(0x5c8760) - fr.f(0x18));
                if (fr.f(0x14) > Fl_207(0x5c8764)) fr.f(0x14) = (float)(Fl_207(0x5c8760) - fr.f(0x14));
                fr.f(0x28) = (float)((double)fr.f(0x14) + fr.f(0x18));
                if (fr.f(0x28) == Fl_207(0x5c8598)) fr.i(0x24) = 0x3f000000;
                else fr.f(0x24) = (float)((double)fr.f(0x18) / fr.f(0x28));
                if (fr.f(0x24) < D(0x5c8758)) fr.i(0x24) = 0x3e800000;
                else if (fr.f(0x24) > D(0x5c8750)) fr.i(0x24) = 0x3f400000;
                // Each end's heading, flipped when it points away from the line.
                fr.f(0x34) = fr.f(0x20);
                fr.f(0x14) = fr.f(0x2c);
                fr.f(0x28) = (float)RtAbs((double)fr.f(0x20) - fr.f(0x0c));
                if (fr.f(0x28) > D(0x5c8748) && fr.f(0x28) < D(0x5c8740)) fr.f(0x34) = (float)RtMod((double)fr.f(0x20) + Fl_207(0x5c8760), tau);
                fr.f(0x28) = (float)RtAbs((double)fr.f(0x2c) - fr.f(0x0c));
                if (fr.f(0x28) > D(0x5c8748) && fr.f(0x28) < D(0x5c8740)) fr.f(0x14) = (float)RtMod((double)fr.f(0x2c) + Fl_207(0x5c8760), tau);
                fr.f(0x50) = (float)(RtSin(fr.f(0x34)) * fr.f(0x24) * D(0x5c85c8) + fr.f(0x50));
                fr.d(0x40) = fr.f(0x4c);
                fr.f(0x4c) = (float)(fr.d(0x40) - RtCos(fr.f(0x34)) * fr.f(0x24) * D(0x5c85c8));
                fr.d(0x40) = D(0x5c8770) - fr.f(0x24);
                fr.f(0x58) = (float)(RtSin(fr.f(0x14)) * fr.d(0x40) * D(0x5c85c8) + fr.f(0x58));
                fr.d(0x68) = fr.f(0x64);
                fr.f(0x64) = (float)(fr.d(0x68) - RtCos(fr.f(0x14)) * fr.d(0x40) * D(0x5c85c8));
            }
            fr.i(0x30) = -1;
            fr.f(0x1c) = (float)((double)fr.f(0x58) - fr.f(0x50));
            fr.f(0x38) = (float)((double)fr.f(0x64) - fr.f(0x4c));
            fr.f(0x54) = (float)((double)fr.f(0x1c) * fr.f(0x24) + fr.f(0x50));
            fr.f(0x38) = (float)((double)fr.f(0x38) * fr.f(0x24) + fr.f(0x4c));
            bool byHeight = false;
            if (!(fr.f(0x10) < Fl_207(0x5c873c))) {
                fr.f(0x28) = (float)((double)fr.f(0x24) * fr.f(0x10));
                byHeight = !(fr.f(0x28) <= Fl_207(0x5c8738)) && !((double)fr.f(0x10) - fr.f(0x28) <= Fl_207(0x5c8738));
            }
            int e;
            if (byHeight) {
                fr.f(0x1c) = (float)Terrain_HeightAtTile(RtMap(), 0, fr.f(0x50), fr.f(0x4c));
                e = Terrain_HeightAtTile(RtMap(), 0, fr.f(0x58), fr.f(0x64)) > fr.f(0x1c) ? 0 : 1;
            } else {
                e = fr.f(0x24) > D(0x5c8628) ? 0 : 1;
            }
            fr.i(0x0c) = lay(0.0f, 0.0f, fr.f(0x54), fr.f(0x38), &fr.i(0x74 + 4 * e), &fr.i(0x30));
            if (!fr.i(0x0c)) return 1;
            fr.i(0x0c) = lay(0.0f, 0.0f, 0.0f, 0.0f, &fr.i(0x78 - 4 * e), &fr.i(0x30));
            if (!fr.i(0x0c)) return 1;
        }
        // Station sites: an estimate pass, then (unless estimating) the build.
        fr.f(0x14) = Tracks_TotalCost(net, 0, (uint32_t)company);
        for (int32_t pass = 0; pass < 2; ++pass) {
            if (pass == 1 && estimate) continue;
            for (uint32_t o = 0; o < 8; o += 4) {
                const int32_t type = fr.i(0xc0 + o);
                if (type == -1) continue;
                const int32_t style = Map_TerrainKindAt(RtMap(), 0, fr.f(0x88 + o), fr.f(0x88 + o));
                char* name = (char*)fr.b + 0xc8;
                FormatText(name, (const char*)0x5c8730, hook::Global<const char*>(0x5f3c80 + 4 * style), hook::Global<const char*>(0x5f3c6c + 4 * type), 0, 0, 0, 0,
                           0, 0);
                int32_t id = -1;
                for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b2fc)); ++i) {
                    Collection* types = hook::Global<Collection*>(0x62b2fc);
                    const int r = RtStricmp(name, (const char*)Collection_Get(types, 0, Collection_IdAt(types, 0, (uint32_t)i, 0)) + 4);
                    if (r == 0) id = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, r);
                }
                if (id == -1) return fr.i(0x0c) = 0, 0;
                const int32_t dir = fr.i(0x98 + o), side = fr.i(0xb8 + o);
                uint32_t angle;
                if (dir == 2 || dir == 6) {
                    angle = side == -1 || side == 0 ? 0x40c90fdb : 0x40490fdb;
                } else {
                    *(uint64_t*)(fr.b + 0x80) = side == -1 || side == 6 ? 0x3ff921fb60000000ull : 0x4012d97c88000000ull;
                    fr.f(0x1c) = (float)fr.d(0x80);
                    angle = (uint32_t)fr.i(0x1c);
                }
                uint8_t* rec = RtGet(0x62b2fc, id);
                Obj_GetCell(rec, 0, (uint16_t*)(fr.b + 0x24), (uint16_t*)(fr.b + 0x2c));
                fr.f(0x18) = fr.f(0x88 + o);
                fr.f(0x20) = fr.f(0xa4 + o);
                const int32_t node =
                    Grid_NearestNodeWithin(hook::Global<Collection*>(0x6cfca8), 0, fr.f(0x88 + o), fr.f(0xa4 + o), -1, RtBits(0x3f395810), 1, 0, 0);
                if (node != -1) Obj_CenterCell(RtGet(0x6cfca8, node), 0, &fr.f(0x18), &fr.f(0x20));
                Type_RotatedCell(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)id, &fr.f(0x38), &fr.f(0x54), RtBits(angle));
                fr.f(0x18) = (float)((double)fr.f(0x38) + fr.f(0x18));
                fr.f(0x20) = (float)((double)fr.f(0x20) - fr.f(0x54));
                if (pass == 0) {
                    const uint32_t region = Map_RegionTypeAt(RtMap(), 0, fr.f(0x18), fr.f(0x20));
                    if (!company && F<int32_t>(RtGet(0x6cfc9c, (int32_t)region), 0x2d)) return fr.i(0x0c) = 0, 0;
                    fr.i(0x0c) = Sites_CanPlace(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)id, fr.f(0x18), fr.f(0x20), RtBits(angle), company, 1, nullptr, 0, 1,
                                                -1, 1);
                    if (!fr.i(0x0c)) return 0;
                    uint8_t* kind = RtGet(0x62b268, F<int32_t>(rec, 0x173));
                    double price = (double)F<int32_t>(kind, 0x22);
                    if (F<int32_t>(kind, 0x22) < 0) price += Fl_207(0x5c872c);
                    fr.f(0x34) = (float)price;
                    const uint32_t cell = Map_GridByte(RtMap(), 0, fr.f(0x18), fr.f(0x20));
                    const uint32_t player = company ? F<uint32_t>(RtGet(0x62be10, company), 0x3b) : 0;
                    fr.f(0x34) = (float)(Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, 0x3b, (uint32_t)company, player, cell) * fr.f(0x34));
                    fr.f(0x14) = (float)((double)fr.f(0x34) + fr.f(0x14));
                    fr.f(0x1c) = (float)F<int32_t>(fr.p(0xac + o), 0x276);
                    fr.f(0x14) = (float)((double)fr.f(0x14) - fr.f(0x1c));
                } else {
                    Sites_CanPlace(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)id, fr.f(0x18), fr.f(0x20), RtBits(angle), company, 1, nullptr, 0, 1, -1, 1);
                    const uint32_t b = Buildings_Add(hook::Global<Collection*>(0x62b26c), 0, rec, fr.f(0x18), fr.f(0x20), angle, company, 0);
                    Building_Place(RtGet(0x62b26c, (int32_t)b), 0, fr.f(0x18), fr.f(0x20), (uint32_t)company);
                }
                if (!fr.i(0x0c)) return 0;
            }
        }
        if (!fr.i(0x0c) || estimate) return 0;
        // Maintenance sheds and service towers along the route.
        fr.i(0x48) = 0, fr.i(0x34) = 0;
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b2fc)); ++i) {
            Collection* types = hook::Global<Collection*>(0x62b2fc);
            const char* name = (const char*)Collection_Get(types, 0, Collection_IdAt(types, 0, (uint32_t)i, 0)) + 4;
            if ((uint8_t)ToUpper(*name) != 'S' && (uint8_t)ToUpper(*name) != 'M') continue;
            if (!RtStricmp((const char*)0x5c871c, name)) fr.i(0x48) = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, 0);
            if (!RtStricmp((const char*)0x5c8710, name)) fr.i(0x34) = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)i, 0);
        }
        if (!fr.i(0x48) || !fr.i(0x34)) return fr.i(0x0c) = 0, 0;
        // Each end moves on to a neighbour laid by this route.
        for (uint32_t o = 0; o < 8; o += 4) {
            uint8_t* p = RtGet(0x6cfca8, fr.i(0x90 + o));
            if (F<uint32_t>(p, 0x240) >= (uint32_t)fr.i(0x7c)) continue;
            for (uint32_t s = 0; s < 0xc; s += 4) {
                const int32_t nb = F<int32_t>(p, 0x206 + s);
                if (nb != -1 && F<uint32_t>(RtGet(0x6cfca8, nb), 0x240) >= (uint32_t)fr.i(0x7c)) {
                    fr.i(0x90 + o) = nb;
                    break;
                }
            }
        }
        int32_t* path = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)(0xfb4);
        path[0] = fr.i(0x90);
        fr.i(0x30) = -1;
        int32_t n = 1;
        for (;;) {
            fr.i(0x18) = n;
            uint8_t* p = RtGet(0x6cfca8, path[n - 1]);
            if (n >= 0x3e7 || F<int32_t>(p, 0x202) == fr.i(0x94)) break;
            int32_t nb = -1;
            uint32_t s = 0;
            for (; s < 0xc; s += 4) {
                nb = F<int32_t>(p, 0x206 + s);
                if (nb != -1 && nb != fr.i(0x30) && F<uint32_t>(RtGet(0x6cfca8, nb), 0x240) >= (uint32_t)fr.i(0x7c)) break;
            }
            if (s == 0xc) break;
            path[n] = nb;
            fr.i(0x30) = F<int32_t>(p, 0x202);
            ++n;
        }
        fr.d(0x98) = (double)n;
        fr.f(0x28) = (float)(fr.d(0x98) / D(0x5c8708));
        fr.i(0x1c) = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) < 0x79e ? 0x25 : 0x1e;
        fr.f(0x1c) = (float)fr.i(0x1c);
        fr.f(0x30) = (float)((double)fr.f(0x28) / fr.f(0x1c));
        if (Fl_207(0x5c85b4) > fr.f(0x30)) fr.i(0x30) = 0x3f800000;
        fr.i(0x80) = Ftol(fr.f(0x30));
        fr.f(0x30) = (float)((double)fr.f(0x28) / Fl_207(0x5c8704));
        if (Fl_207(0x5c85b4) > fr.f(0x30)) fr.i(0x30) = 0x3f800000;
        fr.i(0x30) = Ftol(fr.f(0x30));
        for (int32_t tower = 0; tower < 2; ++tower) {
            const int32_t count = tower ? fr.i(0x80) : fr.i(0x30);
            fr.i(0x60) = tower ? fr.i(0x48) : fr.i(0x34);
            fr.p(0x68) = RtGet(0x62b2fc, fr.i(0x60));
            if (count <= 0) continue;
            const int32_t gap = n / (count + 1);
            fr.i(0x1c) = count, fr.i(0x38) = gap, fr.i(0x5c) = gap;
            do {
                fr.i(0x54) = fr.i(0x5c);
                if (fr.i(0x30) == fr.i(0x80) && n > 0x23) {
                    fr.d(0x40) = tower == 0 ? -0.15 : 0.05;
                    fr.d(0x98) = (double)fr.i(0x38);
                    const double shift = fr.d(0x98) * fr.d(0x40);
                    fr.d(0x98) = (double)fr.i(0x5c);
                    fr.i(0x54) = Ftol(shift + fr.d(0x98));
                }
                fr.i(0xac) = 0, fr.i(0xb0) = 0;
                for (int32_t off = 0; off < 100; off += 2) {
                    for (int32_t k = 0; k < 2; ++k) {
                        const int32_t at = (k == 0 ? 1 : -1) * off + fr.i(0x54);
                        if (at <= 0 || at >= n - 1 || path[at] == -1) continue;
                        uint8_t* p = RtGet(0x6cfca8, path[at]);
                        Obj_CenterCell(p, 0, &fr.f(0x24), &fr.f(0x20));
                        fr.f(0x10) = (float)(Vehicle_ScreenHeading(p, 0, 1) + D(0x5c8748));
                        const int32_t flip = fr.i(0xac + 4 * k);
                        if (flip) fr.f(0x10) = (float)((double)fr.f(0x10) + Fl_207(0x5c8760));
                        while (fr.f(0x10) > Fl_207(0x5c8768)) fr.f(0x10) = (float)((double)fr.f(0x10) - Fl_207(0x5c8768));
                        fr.i(0xac + 4 * k) = flip == 0;
                        Type_RotatedCell(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)fr.i(0x60), &fr.f(0xa4), &fr.f(0x88), fr.f(0x10));
                        fr.f(0x24) = (float)((double)fr.f(0xa4) + fr.f(0x24));
                        fr.f(0x20) = (float)((double)fr.f(0x20) - fr.f(0x88));
                        if (!Sites_CanPlace(hook::Global<Collection*>(0x62b2fc), 0, (uint32_t)fr.i(0x60), fr.f(0x24), fr.f(0x20), fr.f(0x10), company, 1, nullptr, 0, 1, -1,
                                            1))
                            continue;
                        const uint32_t b = Buildings_Add(hook::Global<Collection*>(0x62b26c), 0, fr.p(0x68), fr.f(0x24), fr.f(0x20), (uint32_t)fr.i(0x10), company, 0);
                        Building_Place(RtGet(0x62b26c, (int32_t)b), 0, fr.f(0x24), fr.f(0x20), (uint32_t)company);
                        for (int32_t c = at - 4; c <= at + 4; ++c)
                            if (c >= 0 && c < 1000) path[c] = -1;
                        k = 9999, off = 9999;
                    }
                }
                fr.i(0x5c) += fr.i(0x38);
            } while (--fr.i(0x1c) != 0);
        }
        ((void(__cdecl*)(void*))0x5a1145)(path);
        return 0;
    }();
    if (ended == 1 || Fl_207(0x5c85b4) > fr.f(0x14)) fr.i(0x14) = 0x3f800000;
    const int32_t ok = fr.i(0x0c);
    if (!ok) fr.i(0x14) = 0;
    t[0xf5] = 0;
    if (ok && !estimate) {
        TrackList_ResetBuildState(hook::Global<uint8_t*>(0x6cfca8));
        if (company) {
            Company_BuildStationLinks((int32_t*)RtGet(0x62be10, company));
            RtGet(0x62be10, company)[0xd56] = 1;
        }
        if (F<uint16_t>(t, 0xf6) != 9999) TrackNet_RelayRect(net, 0, F<uint16_t>(t, 0xf6), F<uint16_t>(t, 0xfa), F<uint16_t>(t, 0xf8), F<uint16_t>(t, 0xfc));
    } else {
        TrackNet_Undo(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)fr.i(0x7c));
    }
    if (outCost) *(int32_t*)outCost = fr.i(0x14);
    if (estimate) {
        --F<int32_t>(t, 0xed);
        --F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114256);
    }
    --F<int32_t>(t, 0xe9);
    Obj_Unstash98(t);
    t[0x10a] = 1;
    Track_Validate(net);
    t[0x10a] = 0;
    return ok;
}
RT3_REPLACE(0x402cb0, AI_BuildRoute);

// 0x404640 (thiscall, RET 0x10). AI ai (+0 its company) links city `city` to
// piece `piece` from a new station of the city's size class (0x401000).
RT3_NOINLINE int32_t __fastcall AI_ConnectCityToPiece(int32_t* ai, int, int32_t city, int32_t piece, int32_t estimate, float* outCost) {
    int32_t size, same;
    Obj_SizeClass(Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)city), 0, &size, &same);
    return AI_BuildRoute(hook::Global<Collection*>(0x6cfca8), 0, city, -1, size, 0, piece, -1, estimate, outCost, ai[0]);
}
RT3_REPLACE(0x404640, AI_ConnectCityToPiece);

 // namespace

// 0x4046a0 (thiscall, RET 0x14). AI ai (+0 company) links city `city` into
// the network, from the city's station building (Obj_FindRegionSite; its
// piece +8) when it has one, else from a new station of the city's size
// class. With *toPiece given (not -1) the route goes there (from *fromPiece,
// filled in from the building when -1). Otherwise a target is picked among
// the route ends (0x6cfcb4 +4 / +8) and every 6th piece (1996 at most): plain
// (+0x216 == 2), unbuilt and unpowered (+0x244 / +0x226), with no third or
// fourth link; a dead end counts its distance squared from the city x 0.94 -
// 15, a through piece only when not blocked. The company's nearest one
// outside the excluded boxes is tried, from the building piece or the
// nearest usable piece within distance squared 0x90 that is connected to it.
// A failure excludes the 13 x 13 box around the target and retries, 3 times
// at most. Returns 1 when built, with *toPiece (and *fromPiece from a
// building) set.
RT3_NOINLINE int32_t __fastcall AI_ConnectCity(int32_t* ai, int, int32_t city, int32_t estimate, float* outCost, int32_t* toPiece, int32_t* fromPiece) {
    Collection* const net = hook::Global<Collection*>(0x6cfca8);
    uint8_t* const c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)city);
    int32_t cx = Ftol(Obj_CellX(c));
    int32_t cy = Ftol(Obj_CellY(c));
    uint8_t* site = nullptr;
    if (const uint32_t id = Obj_FindRegionSite(c, 0, 1, 1)) {
        site = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, id);
        if (site) {
            cx = Ftol(Obj_NodeCenterX(site));
            cy = Ftol(Obj_NodeCenterY(site));
        }
    }
    if (toPiece && *toPiece != -1) {
        if (!site) return AI_ConnectCityToPiece(ai, 0, city, *toPiece, estimate, outCost) ? 1 : 0;
        if (*fromPiece == -1) *fromPiece = F<int32_t>(site, 8);
        return AI_BuildRoute(net, 0, 0, *fromPiece, -1, 0, *toPiece, -1, estimate, outCost, ai[0]) ? 1 : 0;
    }
    RouteTarget cand[0x7d0];
    int32_t n = 0;
    if (Collection_Count(hook::Global<Collection*>(0x6cfcb4)) > 0) {
        for (int32_t i = 0;;) {
            Collection* routes = hook::Global<Collection*>(0x6cfcb4);
            uint8_t* r = (uint8_t*)Collection_Get(routes, 0, Collection_IdAt(routes, 0, (uint32_t)i, 0));
            if (F<int32_t>(r, 4) != -1) cand[n++].id = F<int32_t>(r, 4);
            if (F<int32_t>(r, 8) != -1) cand[n++].id = F<int32_t>(r, 8);
            if (n > 0x7cb) break;
            if (++i >= Collection_Count(hook::Global<Collection*>(0x6cfcb4))) break;
        }
    }
    if (Collection_Count(hook::Global<Collection*>(0x6cfca8)) > 0) {
        for (int32_t i = 0;;) {
            cand[n++].id = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)i, 0);
            if (n > 0x7cb) break;
            i += 6;
            if (i >= Collection_Count(hook::Global<Collection*>(0x6cfca8))) break;
        }
    }
    for (int32_t k = 0; k < n; ++k) {
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)cand[k].id);
        cand[k].foreign = (int32_t)F<uint16_t>(p, 0x22e) != ai[0];
        if (p[0x216] != 2 || F<int32_t>(p, 0x244) || F<int32_t>(p, 0x226) || F<int32_t>(p, 0x20e) != -1 || F<int32_t>(p, 0x222) != -1) {
            cand[k].usable = 0;
            continue;
        }
        cand[k].usable = 1;
        const int32_t py = Ftol(Obj_CenterCellY(p));
        const int32_t px = Ftol(Obj_CenterCellX(p));
        const int32_t d = DistanceSqInt(px, py, cx, cy);
        cand[k].dist = d;
        if (F<int32_t>(p, 0x206) == -1 || F<int32_t>(p, 0x20a) == -1) cand[k].dist = Ftol((double)d * D(0x5c87a0) - D(0x5c8798));
        else if (Tile_IsBlocked(p)) cand[k].usable = 0;
    }
    int32_t xmax[3], ymin[3], ymax[3], xmin[3];
    int32_t target = 0, ty = 0;
    for (int32_t tries = 0, boxes = 0;;) {
        int32_t best = 0x773593ff, tx = -1;
        if (n <= 0) return 0;
        for (int32_t k = 0; k < n; ++k) {
            if (!cand[k].usable || cand[k].foreign || cand[k].dist >= best) continue;
            uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)cand[k].id);
            const int32_t px = Ftol(Obj_CenterCellX(p));
            const int32_t py = Ftol(Obj_CenterCellY(p));
            bool excluded = false;
            for (int32_t b = 0; b < boxes && !excluded; ++b) excluded = px >= xmin[b] && px <= xmax[b] && py >= ymin[b] && py <= ymax[b];
            if (excluded) {
                cand[k].usable = 0;
                continue;
            }
            best = cand[k].dist, tx = px, ty = py, target = F<int32_t>(p, 0x202);
        }
        if (tx == -1) return 0;
        if (site) {
            int32_t from = F<int32_t>(site, 8), nearDist = 0x90, nearId = -1;
            for (int32_t k = 0; k < n; ++k) {
                if (!cand[k].usable || cand[k].id == target || cand[k].dist >= nearDist) continue;
                if (Node_SameComponent((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)cand[k].id), 0, (uint32_t)from))
                    nearDist = cand[k].dist, nearId = cand[k].id;
            }
            if (nearId != -1) from = nearId;
            if (AI_BuildRoute(hook::Global<Collection*>(0x6cfca8), 0, 0, from, -1, 0, target, -1, estimate, outCost, ai[0])) {
                *toPiece = target;
                *fromPiece = from;
                return 1;
            }
        } else {
            int32_t size, same;
            Obj_SizeClass(Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)city), 0, &size, &same);
            if (AI_BuildRoute(hook::Global<Collection*>(0x6cfca8), 0, city, -1, size, 0, target, -1, estimate, outCost, ai[0])) {
                *toPiece = target;
                return 1;
            }
        }
        xmax[boxes] = tx + 6, ymax[boxes] = ty + 6, xmin[boxes] = tx - 6, ymin[boxes] = ty - 6;
        if (tries++ >= 5) return 0;
        if (++boxes >= 3) return 0;
    }
}
RT3_REPLACE(0x4046a0, AI_ConnectCity);

// 0x404c60 (RET 0xc). Links cities a and b, each from a new station of its
// size class (0x401000).
RT3_NOINLINE int32_t __fastcall AI_ConnectCities(int32_t cityA, int32_t cityB, int32_t estimate, float* outCost, int32_t company) {
    void* c[2] = { Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)cityA), Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)cityB) };
    int32_t size[2], same[2];
    for (int k = 0; k < 2; ++k) Obj_SizeClass(c[k], 0, &same[k], &size[k]);
    return AI_BuildRoute(hook::Global<Collection*>(0x6cfca8), 0, cityA, -1, size[0], cityB, -1, size[1], estimate, outCost, company);
}
RT3_REPLACE(0x404c60, AI_ConnectCities);

 // namespace

// 0x404ce0 (RET 4). Plans an AI company's first route between two cities
// (`company` 0: none). A company below 350000 in stat 0xd (year 9001) and of
// quality below 0.7 does not plan (returns 0). Each of the first 160 cities
// scores SimRand(70, 130) % of its service value (City_ServiceValue),
// weighted towards the map centre ((1 - distance / half diagonal + 4) / 4);
// cities with a connected station (0x420030), marked +0x23e or in a closed
// territory score 0. Pairs within 120 cells (taxicab) and 19..99 apart (below
// 37: d / 4 + 22) are worth score x score / d^2. Up to 20 trials: while fewer
// than 3 have a cost (estimated by AI_ConnectCities), the best pair of free
// cities is estimated and one of them taken (SimRand 1..10 < 5: the first);
// then the trial with the best worth / cost (cost at least 1) is built
// (`test`: estimated only), and a failed build drops that trial. Outside a
// test the larger city (+0x25e) gets the news: 0xb4a with the company and
// both cities, or for a company flagged +0xd39 (cleared) the 0xb49 text with
// `rival`'s name, the route then named by 0x426f20. Returns the build's
// result.
// QUIRK: with no company the news names the company at address 4.
RT3_NOINLINE int32_t __fastcall AI_PlanFirstRoute(int32_t test, int32_t rival, int32_t company) {
    PlanFrame fr;
    memset(fr.b, 0, sizeof fr.b);
    uint8_t* co = nullptr;
    if (company) {
        co = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)company);
        if (co && Company_Stat(co, 0, 0xd, 0x2329) < D(0x5c87b0) && Company_QualityRaw(co) < D(0x5c8680)) return 0;
    }
    int32_t result = 0;
    int32_t count = Collection_Count(hook::Global<Collection*>(0x62bae0));
    if (count >= 0xa0) count = 0xa0;
    for (int32_t i = 0; i < count; ++i) {
        Collection* cities = hook::Global<Collection*>(0x62bae0);
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0));
        float& score = fr.f(0x4f8 + 4 * i);
        if (!F<int32_t>(c, 0x23e) && !Region_HasConnectedStation(c, 0, 1, 1, 0, 0)) {
            const int32_t r = SimRand(0x46, 0x82);
            const float value = City_ServiceValue(c, 0, (uint32_t)company);
            const float base = (float)((double)r / D(0x5c85d8) * value);
            score = base;
            float hy = (float)((double)F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) * D(0x5c8628));
            float hx = (float)((double)F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155) * D(0x5c8628));
            const float cy = (float)Obj_CellY(c);
            const float cx = (float)Obj_CellX(c);
            const float centre = (float)Distance2D(cx, cy, hx, hy);
            hy = (float)((double)F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) * D(0x5c8628));
            hx = (float)((double)F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155) * D(0x5c8628));
            const float ratio = (float)(centre / Distance2D(0.0f, 0.0f, hx, hy));
            score = (float)((D(0x5c8770) - ratio + D(0x5c87b8)) / D(0x5c87b8) * base);
        } else {
            score = 0.0f;
        }
        fr.i(0x778 + 4 * i) = Ftol(Obj_CellX(c));
        fr.i(0x9f8 + 4 * i) = Ftol(Obj_CellY(c));
        fr.i(0x278 + 4 * i) = F<int32_t>(c, 0x23a);
        const uint32_t region = Map_RegionTypeAt(hook::Global<uint8_t*>(0x62c120), 0, (float)fr.i(0x778 + 4 * i), (float)fr.i(0x9f8 + 4 * i));
        if (!company) {
            if (F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, region), 0x2d)) score = 0.0f;
        } else if (!Company_EntryFlag(co, 0, (int32_t)region)) {
            score = 0.0f;
        }
    }
    // Pair worths, upper triangle.
    int32_t* const worth = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)((uint32_t)(count * count) * 4);
    memset(worth, 0, (uint32_t)(count * count) * 4);
    for (int32_t a = 0; a < count; ++a) {
        if (fr.f(0x4f8 + 4 * a) <= Fl_207(0x5c8598)) continue;
        for (int32_t b = a + 1; b < count; ++b) {
            if (fr.f(0x4f8 + 4 * b) <= Fl_207(0x5c8598)) continue;
            const int32_t dx = fr.i(0x778 + 4 * a) - fr.i(0x778 + 4 * b), dy = fr.i(0x9f8 + 4 * a) - fr.i(0x9f8 + 4 * b);
            if ((dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy) >= 0x78) continue;
            int32_t d = (uint16_t)DistanceInt(fr.i(0x778 + 4 * a), fr.i(0x9f8 + 4 * a), fr.i(0x778 + 4 * b), fr.i(0x9f8 + 4 * b));
            if (d <= 0x12) continue;
            if (d < 0x25) d = (d >> 2) + 0x16;
            if (d >= 0x64) continue;
            worth[a * count + b] = Ftol((double)fr.f(0x4f8 + 4 * a) * fr.f(0x4f8 + 4 * b) / (float)(d * d));
        }
    }
    [&] {
        memset(fr.b + 0x1d8, 0, 0xa0);
        memset(fr.b + 0x98, 0, 0x50);
        int32_t pick;
        for (;;) {
            int32_t costed = 0;
            for (int32_t o = 0; o < 0x50; o += 4) {
                if (fr.i(0x98 + o)) {
                    if (fr.i(0xe8 + o)) ++costed;
                    continue;
                }
                if (costed >= 3) {
                    fr.i(0x98 + o) = 0;
                    continue;
                }
                fr.i(0x98 + o) = 1;
                if (count <= 0) return;
                int32_t best = 0;
                for (int32_t a = 0; a < count; ++a) {
                    if (fr.b[0x1d8 + a]) continue;
                    for (int32_t b = a + 1; b < count; ++b)
                        if (!fr.b[0x1d8 + b] && worth[a * count + b] > best) best = worth[a * count + b], fr.i(0x44) = a, fr.i(0x2c) = b;
                }
                if (best <= 0) return;
                const int32_t a = fr.i(0x44), b = fr.i(0x2c);
                fr.i(0x188 + o) = a, fr.i(0x138 + o) = b;
                fr.i(0xe8 + o) = AI_ConnectCities(fr.i(0x278 + 4 * a), fr.i(0x278 + 4 * b), 1, &fr.f(0x48 + o), company);
                if (fr.i(0xe8 + o)) ++costed;
                if (SimRand(1, 0xa) < 5) fr.b[0x1d8 + a] = 1;
                else fr.b[0x1d8 + b] = 1;
            }
            pick = -1;
            float bestRatio = 0.0f;
            for (int32_t k = 0; k < 20; ++k) {
                if (!fr.i(0x98 + 4 * k) || !fr.i(0xe8 + 4 * k)) continue;
                const float cost = Fl_207(0x5c85b4) > fr.f(0x48 + 4 * k) ? 1.0f : fr.f(0x48 + 4 * k);
                const float w = (float)worth[fr.i(0x188 + 4 * k) * count + fr.i(0x138 + 4 * k)];
                const float ratio = (float)((double)w / cost);
                if (ratio > bestRatio) bestRatio = ratio, pick = k;
            }
            if (pick == -1) {
                if (costed) continue;
                break;
            }
            result = AI_ConnectCities(fr.i(0x278 + 4 * fr.i(0x188 + 4 * pick)), fr.i(0x278 + 4 * fr.i(0x138 + 4 * pick)), test, nullptr, company);
            if (result) break;
            fr.i(0xe8 + 4 * pick) = 0;
        }
        if (test) return;
        const int32_t idB = fr.i(0x278 + 4 * fr.i(0x138 + 4 * pick));
        const int32_t idA = fr.i(0x278 + 4 * fr.i(0x188 + 4 * pick));
        uint8_t* cb = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)idB);
        uint8_t* ca = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)idA);
        const int32_t larger = F<float>(ca, 0x25e) > F<float>(cb, 0x25e) ? idA : idB;
        char* text = (char*)fr.b + 0x278;
        const char* coName = (const char*)((uintptr_t)co + 4);
        if (co && co[0xd39]) {
            co[0xd39] = 0;
            if (result) Train_AutoName(co, 0, (uint32_t)idA, (uint32_t)idB);
            const char* name = rival ? (const char*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)rival) + 8 : (const char*)0x5c87a8;
            const char* fmt = GetString(0xb49);
            FormatText(text, fmt, name, coName, 0, 0, 0, 0, 0, 0);
        } else {
            if (!result) return;
            uint8_t* nb = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)idB);
            uint8_t* na = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)idA);
            const char* fmt = GetString(0xb4a);
            FormatText(text, fmt, coName, (const char*)na + 0x356, (const char*)nb + 0x356, 0, 0, 0, 0, 0);
        }
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x10, company, larger);
    }();
    if (worth) ((void(__cdecl*)(void*))0x5a1145)(worth);
    return result;
}
RT3_REPLACE(0x404ce0, AI_PlanFirstRoute);

// 0x405920. Distance (cells) from (x, y) to the company's nearest site; at least 1.
RT3_NOINLINE float __fastcall Company_NearestSiteDistance(int32_t* c, int, int32_t x, int32_t y) {
    int32_t best = 999999999;
    int32_t n = Collection_Count(g_table6cec20);
    for (int32_t i = 0; i < n; ++i) {
        void* r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != c[0]) continue;
        int32_t sy = (int32_t)Obj_BuildingCellY(r);
        int32_t sx = (int32_t)Obj_BuildingCellX(r);
        int32_t d = DistanceSqInt(x, y, sx, sy);
        if (d < best) best = d;
    }
    if (n > 0 && best < 1) best = 1;
    return (float)ExeSqrt((double)best);
}
RT3_REPLACE(0x405920, Company_NearestSiteDistance);

// 0x4059e0. Track network net: an AI company id (cash stat 0xd above
// 100000, single player) doubles its busy track. Marks (+0x24c bit 2) all
// double pieces (state 4) and collects its single pieces with traffic
// +0x22a above 300 (switches always), plus around each of its active
// stations the plain pieces next to marked ones (two steps out on each
// side). When the company can afford them all (Company_Get7680, then
// charged), 11 settling passes unmark pieces that would not join two
// marked neighbours; on the last the rest become double track (state 4,
// keeping +0x240 from version 103), their cost (Track_Cost) booked as stat
// 0xf and reported in the news (0xb4b). The marked ones are re-resolved.
// QUIRK: the second step out re-checks the same neighbour.
RT3_NOINLINE void __fastcall TrackNet_AiDoubleTrack(Collection* net, int, uint32_t id) {
    uint8_t* co = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
    if (Company_Stat(co, 0, 0xd, 0x2329) < D(0x5c87c8)) return;
    if (IsMultiplayer()) return;
    uint32_t* buf = (uint32_t*)Malloc(Collection_Count(net) * 4 + 0x3e8);
    int32_t n = 0;
    auto get = [&](uint32_t k) { return (uint8_t*)Collection_Get(net, 0, k); };
    auto take = [&](uint8_t* p) {
        buf[n++] = F<uint32_t>(p, 0x202);
        p[0x24c] |= 4;
    };
    for (int32_t i = 0; i < Collection_Count(net); ++i) {
        uint8_t* p = get(Collection_IdAt(net, 0, (uint32_t)i, 0));
        uint8_t s = p[0x218];
        if (s == 4) {
            p[0x24c] |= 4;
            continue;
        }
        p[0x24c] &= 0xfb;
        if (F<uint16_t>(p, 0x22e) != id || F<int32_t>(p, 0x226) || (s != 1 && s != 2)) continue;
        if (!(F<float>(p, 0x22a) > Fl_207(0x5c87c0)) && s != 2) continue;
        take(p);
    }
    for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++j) {
        Collection* stations = hook::Global<Collection*>(0x6cec20);
        uint8_t* st = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, (uint32_t)j, 0));
        if ((uint32_t)Obj_BuildingValue(st) != id || !Obj_BuildingStateActive(st)) continue;
        int32_t home = F<int32_t>(st, 8);
        uint8_t* P = get((uint32_t)home);
        for (int32_t k = 0; k < 2; ++k)
            if (Track_DoubleCandidate(P, id)) {
                int32_t nb = F<int32_t>(P, k ? 0x20a : 0x206);
                if (nb != -1 && (get((uint32_t)nb)[0x24c] & 4)) take(P);
            }
        for (int32_t d = 0; d < 2; ++d) {
            int32_t nb = F<int32_t>(P, d ? 0x20a : 0x206);
            if (nb == -1) continue;
            uint8_t* Q = get((uint32_t)nb);
            if (Track_DoubleCandidate(Q, id)) take(Q);
            for (int32_t c = 2; c; --c) {
                int32_t nb2 = F<int32_t>(Q, d ? 0x20a : 0x206);
                if (nb2 == -1 || nb2 == home) continue;
                uint8_t* R = get((uint32_t)nb2);
                if (Track_DoubleCandidate(R, id)) take(R);
            }
        }
    }
    if (Company_Get7680(co) >= n) {
        Company_Add7680(co, 0, -n);
        int32_t done = 0, cost = 0;
        for (int32_t pass = 10; pass >= 0; --pass)
            for (int32_t j = 0; j < n; ++j) {
                uint8_t* p = get(buf[j]);
                int32_t marked = 0, single = 0, nb = -1;
                for (int32_t s = 0; s < 3; ++s) {
                    nb = F<int32_t>(p, s == 0 ? 0x206 : s == 1 ? 0x20a : 0x20e);
                    if (nb != -1) {
                        uint8_t* q = get((uint32_t)nb);
                        if (q[0x24c] & 4) {
                            ++marked;
                            if (q[0x218] != 4) ++single;
                        }
                        if (F<int32_t>(q, 0x206) != -1 && F<int32_t>(q, 0x206) != F<int32_t>(p, 0x202) && (get(F<uint32_t>(q, 0x206))[0x24c] & 4)) ++marked;
                        if (F<int32_t>(q, 0x20a) != -1 && F<int32_t>(q, 0x20a) != F<int32_t>(p, 0x202) && (get(F<uint32_t>(q, 0x20a))[0x24c] & 4)) ++marked;
                    }
                    if (marked >= 2 && single > 0) break;
                }
                if (marked < 2) p[0x24c] &= 0xfb;
                else if (!single && p[0x218] == 2) p[0x24c] &= 0xfb;
                else if (!pass) {
                    ++done;
                    if (p[0x218] != 2) cost += Track_Cost(p, 0, id);
                    int32_t keep = F<int32_t>(p, 0x240);
                    Track_SetState(p, 0, 4, p[0x44] != 0, 1, 1);
                    if (Game_EffectiveVersion() >= 0x67) F<int32_t>(p, 0x240) = keep;
                }
            }
        for (int32_t j = 0; j < n; ++j) {
            uint8_t* p = get(buf[j]);
            if (!(p[0x24c] & 4)) continue;
            int32_t keep = F<int32_t>(p, 0x240);
            Track_Resolve(p, 0, 1);
            if (Game_EffectiveVersion() >= 0x67) F<int32_t>(p, 0x240) = keep;
        }
        if (done > 0) {
            Company_AddStat(co, 0, 0xf, (float)cost);
            char money[0x1f4], count[0x1f4], text[0x1f4];
            const char* m = Num_FormatSimple2(money, 0xb, (double)cost, 0xb);
            const char* c = Num_Format(count, 1, (double)done, 10, 1, 0, 0);
            FormatText(text, GetString(0xb4b), (const char*)co + 4, c, m, 0, 0, 0, 0, 0);
            Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 8, (int32_t)id, 0);
        }
    }
    Free(buf);
}
RT3_REPLACE(0x4059e0, TrackNet_AiDoubleTrack);

// 0x406050. AI company co (+0 its id) plans its next city link. Its cash
// (stat 0xd, scaled by the owner's personality 0x5f2d3b + 200 over 0x5c8870)
// with a small network (< 4 cargo sites) that can still borrow gains 0.4 ..
// 2 million by credit rating (+1 below 3 sites). Below 400000, or with more
// than 24 / 20 / 15 sites (difficulty +0x4c78 0 / 1 / 2), it does nothing.
// The first 400 cities score SimRand(80, 120) % of their service value,
// less beyond 50 cells from the company's nearest site (none beyond 120);
// their value is that x 0x5c8588 / (distance + 13, at least 21). The budget
// is 150000, or 350000 / 500000 (below 4 sites) after two years of good
// stat 0x2b (x 0x5c8820 when better still), scaled by difficulty, -100000
// when it cannot borrow. Up to 10 trials while fewer than 2 fit: the best
// city not tried yet (nor, on the first pass, one of the 10 recently failed
// +0xd42), worth its value scaled by cash, sites, personality (0x5f2d3f +
// 900), difficulty and version, is estimated (AI_ConnectCity) and fits when
// its cost is within cash + budget and the worth. The fitting trial with the
// best score / cost is built and announced (0xb48); failed trials become the
// recent list. +0xd18 is set when nothing fits or cash is above 1 million.
// QUIRK: a city is compared by score against the previous pick's value, not
// its score.
RT3_NOINLINE int32_t __fastcall AI_PlanExpansion(uint8_t* co) {
    int32_t cash = Ftol(Company_Stat(co, 0, 0xd, 0x2329));
    uint8_t* const owner = (uint8_t*)Company_Owner(co);
    if (owner) cash = Ftol((double)(hook::Global<int32_t>(0x5f2d3b + 0x27 * (int8_t)owner[0x291]) + 0xc8) / D(0x5c8870) * (double)cash);
    const int32_t sites = Company_CountCargoSites(co, 0, 1);
    if (Game_EffectiveVersion() >= 0x67 && sites < 4 && cash > 50000 && Company_CanIssueBond(co)) {
        float rating = Company_CreditRating(co, 0, 1000000);
        if (sites <= 2) rating = (float)((double)rating + Fl_207(0x5c85b4));
        const double r = rating;
        if (r >= D(0x5c8868)) cash += 2000000;
        else if (r >= D(0x5c8860)) cash += 1600000;
        else if (r >= D(0x5c8858)) cash += 1200000;
        else if (r >= D(0x5c8850)) cash += 800000;
        else if (r >= D(0x5c8848)) cash += 400000;
    }
    if (cash < 400000) return 0;
    {
        const int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c78);
        if ((mode == 0 && sites > 24) || (mode == 1 && sites > 20) || (mode == 2 && sites > 15)) return 0;
    }
    int32_t tried[10] = {}, built[10], toPiece[10], fromPiece[10];
    uint32_t chosen[10];
    float cost[10] = {}, worthOf[10];
    float score[400], value[400];
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
        if (i >= 400) continue;
        score[i] = 0.0f;
        Collection* cities = hook::Global<Collection*>(0x62bae0);
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x23e)) continue;
        const uint32_t id = *(uint32_t*)co;
        const int32_t r = SimRand(0x50, 0x78);
        const float v = City_ServiceValue(c, 0, id);
        float s = (float)((double)r / D(0x5c85d8) * v);
        if (s <= Fl_207(0x5c8598)) continue;
        const int32_t y = Ftol(Obj_CellY(c));
        const int32_t x = Ftol(Obj_CellX(c));
        int32_t d = Ftol(Company_NearestSiteDistance((int32_t*)co, 0, x, y));
        if (d > 0x78) continue;
        if (d > 0x32) s = (float)((double)(float)(0x78 - d) / Fl_207(0x5c8840) * s);
        score[i] = s;
        if (d <= 8) d = 8;
        value[i] = (float)(D(0x5c8588) / (double)(d + 0xd) * s);
    }
    int32_t budget = 150000;
    {
        auto stat = [&](int32_t year) { return Company_Stat(co, 0, 0x2b, year); };
        auto year = [] { return (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd); };
        if (year() - (int32_t)F<uint16_t>(co, 0x157) >= 2 && stat(0x2329) > D(0x5c8838) && stat(year() - 1) > D(0x5c8830) && stat(year() - 2) > D(0x5c8830)) {
            budget = sites >= 4 ? 350000 : 500000;
            if (stat(year() - 1) > D(0x5c8828) && stat(year() - 2) > D(0x5c8828)) budget = Ftol((double)budget * D(0x5c8820));
        }
        const int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c78);
        if (mode == 2) budget = Ftol((double)budget * D(0x5c8640));
        else if (mode == 0) budget = Ftol((double)budget * D(0x5c8818));
        if (!Company_CanIssueBond(co)) budget = -100000;
    }
    int32_t fits = 0;
    float lastScore = 0.0f;
    for (int32_t k = 0; k < 10; ++k) {
        built[k] = 0;
        if (fits >= 2) continue;
        float best = 0.0f;
        uint32_t pick = 0;
        for (int32_t pass = 0; pass < 2; ++pass) {
            if (pass == 1 && pick) continue;
            for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
                if (i >= 400 || !(score[i] > best)) continue;
                const uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)i, 0);
                for (int32_t c = 0; c < k; ++c)
                    if (chosen[c] == id) goto nextPass;
                if (pass == 0)
                    for (int32_t c = 0; c < 10; ++c)
                        if (F<uint16_t>(co, 0xd42 + 2 * c) == id) goto nextPass;
                lastScore = score[i], best = value[i], pick = id;
            }
        nextPass:;
        }
        float worth = (float)(((double)cash + D(0x5c8810)) / D(0x5c8808) * best);
        worth = (float)(D(0x5c8800) / (double)(sites + 9) * worth);
        if (owner) worth = (float)((double)(hook::Global<int32_t>(0x5f2d3f + 0x27 * (int8_t)owner[0x291]) + 0x384) / D(0x5c87f8) * worth);
        const int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c78);
        if (mode == 1) worth = (float)(worth * D(0x5c87f0));
        else if (mode == 0) worth = (float)(worth * D(0x5c87e8));
        worth = (float)(worth * D(0x5c87e0));
        if (cash > 0x895440) worth = (float)(worth * D(0x5c87d8));
        else if (cash > 0x4c4b40) worth = (float)((double)worth + worth);
        else if (cash > 0x2625a0) worth = (float)(worth * D(0x5c8578));
        else if (cash > 0x16e360) worth = (float)(worth * D(0x5c87e8));
        else if (cash > 0x10c8e0) worth = (float)(worth * D(0x5c85a8));
        if (Game_EffectiveVersion() >= 0x67) {
            if (sites <= 2) worth = (float)(worth * D(0x5c8820));
            else if (sites <= 3) worth = (float)(worth * D(0x5c8660));
        }
        if (pick && worth > Fl_207(0x5c87d0)) {
            fromPiece[k] = -1, toPiece[k] = -1;
            built[k] = AI_ConnectCity((int32_t*)co, 0, (int32_t)pick, 1, &cost[k], &toPiece[k], &fromPiece[k]);
            tried[k] = 1;
            const float limit = (float)(budget + cash);
            if (cost[k] > limit || cost[k] > worth) built[k] = 0;
            if (built[k]) ++fits;
            worthOf[k] = lastScore;
        }
        chosen[k] = pick;
    }
    int32_t pick = -1;
    float bestRatio = 0.0f;
    for (int32_t k = 0; k < 10; ++k) {
        if (!built[k]) continue;
        const float c = Fl_207(0x5c85b4) > cost[k] ? 1.0f : cost[k];
        const float ratio = (float)((double)worthOf[k] / c);
        if (ratio > bestRatio) bestRatio = ratio, pick = k;
    }
    memset(co + 0xd42, 0, 0x14);
    for (int32_t k = 9, n = 0; k >= 0; --k) {
        if (!tried[k] || built[k]) continue;
        F<uint16_t>(co, 0xd42 + 2 * n) = (uint16_t)chosen[k];
        if (++n >= 10) break;
    }
    if (pick == -1) {
        co[0xd18] = 1;
        return 0;
    }
    const uint32_t city = chosen[pick];
    if (AI_ConnectCity((int32_t*)co, 0, (int32_t)city, 0, nullptr, &toPiece[pick], &fromPiece[pick])) {
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, city);
        char text[0x1f8];
        const char* fmt = GetString(0xb48);
        FormatText(text, fmt, (const char*)co + 4, (const char*)c + 0x356, 0, 0, 0, 0, 0, 0);
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x10, *(int32_t*)co, (int32_t)city);
    }
    if (cash > 1000000) co[0xd18] = 1;
    return 1;
}
RT3_REPLACE(0x406050, AI_PlanExpansion);

RT3_NOINLINE void* __fastcall Collection_RecordAt2(Collection* t, int, uint32_t pos) { return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, 0)); }
RT3_REPLACE(0x406bf0, Collection_RecordAt2);

// 0x406c10. The AIs' turn on world w, unless paused for it (0x4338c0),
// auto companies (lobby +0x82), world +0x46c38 or 0x6d3b4c: every player
// (0x4073a0) and every company (0x4019e0) acts; when one of them changed the
// game options (+0x4c84) they are restored.
RT3_NOINLINE void __fastcall AI_Tick(uint8_t* w) {
    if (Global6d07b4Set2() || hook::Global<uint8_t*>(0x6cec7c)[0x82] || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38) || hook::Global<int32_t>(0x6d3b4c))
        return;
    F<int32_t>(w, 0x4c84) = 0;
    for (int32_t i = 0; i < Table_CountField4(hook::Global<Collection*>(0x6ceb9c)); ++i)
        Player_AiTurn((uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i));
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62be10)); ++i) {
        Collection* companies = hook::Global<Collection*>(0x62be10);
        AI_CompanyTurn((uint8_t*)Collection_Get(companies, 0, Collection_IdAt(companies, 0, (uint32_t)i, 0)));
    }
    if (F<int32_t>(w, 0x4c84)) Options_Restore();
}
RT3_REPLACE(0x406c10, AI_Tick);

// 0x406d00. AI player p: trades stock (not with option +0x297 or world
// +0x4a7f) in rounds of 1000 shares, at most 200: each round scores buying
// every active company (its +0xd19 per share price; own company x1.1, other
// people's companies scaled by the personality +0x291 table 0x5f2d2f
// (0x27-byte rows: unaffordable +0, short +4, foreign +8), damped when p is
// already mostly in stock) and selling it (when +0xd19 is above 8), and
// makes the best trade scoring at least 1.15. The net trades go to the news
// log 0x6acd34 (0xb4f with bought 0xb4c / sold short 0xb4d / sold 0xb4e).
// QUIRK: company id 0 is never traded (an id of 0 means "none"); the log
// kind compares the company index with world +0x21.
RT3_NOINLINE void __fastcall Player_AiTradeStocks(uint8_t* p) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x297) || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f)) return;
    uint8_t* ai = (uint8_t*)0x5f2d2f + (int8_t)p[0x291] * 0x27;
    auto trait = [&](int32_t off, double div, float s) { return (float)((double)F<int32_t>(ai, off) / div * ((double)s - 1.0) + 1.0); };
    int32_t nCo = Table_CountField3f(hook::Global<Collection*>(0x62be10));
    int32_t delta[0x20] = {};
    for (int32_t round = 0; round < 200; ++round) {
        float cashF = F<double>(p, 0x154) > D(0x5c87c8) ? (float)(F<double>(p, 0x154) / D(0x5c87c8)) : 1.0f;
        uint32_t best = 0;
        int32_t buy = 1;
        float bestScore = 0;
        double hv = Player_HoldingsValue(p, 0, 0);
        float ratio = (float)(hv / Player_NetWorth(p, 0, 0));
        for (int32_t k = 0; k < nCo; ++k) {
            uint8_t* c = (uint8_t*)Table_NthField3f(hook::Global<Collection*>(0x62be10), 0, k);
            int32_t canBuy = Player_CanBuyShares(p, 0, F<uint32_t>(c, 0), !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a83), 0);
            int32_t canSell = Player_CanSellShares(p, 0, F<uint32_t>(c, 0), !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a83), 0, 1);
            bool own = F<uint32_t>(c, 0x3b) == F<uint32_t>(p, 0);
            int32_t foreign = !own && F<int32_t>(p, 0x1dd) ? 1 : 0;
            if (!canBuy && !canSell) continue;
            float g = F<float>(c, 0xd19);
            float gr = (float)((double)g < D(0x5c85b8) ? D(0x5c85b8) : (double)g);
            if (own) gr = (float)((double)gr * D(0x5c85a8));
            if (canBuy) {
                float price = Company_SharePrice(c, 0, 1000, 0);
                double pd = (double)price < D(0x5c88a0) ? D(0x5c88a0) : (double)price;
                float s = (float)((double)gr / pd);
                if (foreign) s = trait(8, D(0x5c85d8), s);
                if ((double)price * Fl_207(0x5c86a0) > F<double>(p, 0x154)) {
                    s = trait(0, D(0x5c85e8), s);
                    if ((double)ratio > D(0x5c87e8)) {
                        double rd = (double)ratio < D(0x5c88a0) ? D(0x5c88a0) : (double)ratio;
                        s = (float)(((double)s - D(0x5c8770)) / rd + D(0x5c8770));
                    }
                }
                if ((double)s > D(0x5c8650)) s = (float)((double)s * cashF);
                if (s > bestScore) {
                    bestScore = s;
                    buy = 1;
                    best = F<uint32_t>(c, 0);
                }
            }
            if (!canSell || !((double)gr > Fl_207(0x5c8898))) continue;
            float price = Company_SharePrice(c, 0, -1000, 0);
            double grd = (double)gr < D(0x5c88a0) ? D(0x5c88a0) : (double)gr;
            float s = (float)((double)price / grd);
            if (foreign) s = trait(8, D(0x5c85e8), s);
            uint32_t id = F<uint32_t>(c, 0);
            int32_t held = F<int32_t>(p, 0x15d + id * 4);
            if (held <= 0) s = trait(4, D(0x5c85e8), s);
            else if ((double)s > D(0x5c8890)) {
                float f = (double)ratio > D(0x5c85a8) ? (float)(((double)ratio - D(0x5c85a8)) * D(0x5c85c8) + D(0x5c85a8)) : ratio;
                f = (float)((double)f < D(0x5c8628) ? D(0x5c8628) : (double)f);
                s = (float)(((double)s - D(0x5c8888)) * f + D(0x5c8888));
            }
            if (!foreign) {
                if (held <= 1000) s = 0;
                else s = (float)(((double)s - D(0x5c8770)) * D(0x5c8880) + D(0x5c8770));
            }
            if (s > bestScore) {
                bestScore = s;
                buy = 0;
                best = id;
            }
        }
        if (!best || (double)bestScore < D(0x5c8878)) break;
        if (buy) Player_BuyShares(p, 0, best, 1000, 1, 0);
        else Player_SellShares(p, 0, best, 1000, 1, 0);
        delta[best] += buy ? 1000 : -1000;
    }
    for (int32_t i = 0; i < 0x20; ++i) {
        int32_t d = delta[i];
        if (!d) continue;
        char verb[0x68], num[0x68], text[0x1e8];
        if (d > 0) strcpy(verb, GetString(0xb4c));
        else if (F<int32_t>(p, 0x15d + i * 4) < 0) strcpy(verb, GetString(0xb4d));
        else strcpy(verb, GetString(0xb4e));
        const char* company = (const char*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)i) + 4;
        const char* n = Num_Format(num, 8, (double)(d < 0 ? -d : d), 10, 1, 0, 0);
        FormatText(text, GetString(0xb4f), (const char*)p + 8, verb, n, company, 0, 0, 0, 0);
        int32_t pid = F<int32_t>(p, 0);
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0xe + (i != Obj_Get21(hook::Global<uint8_t*>(0x6cec78))), pid, i);
    }
}
RT3_REPLACE(0x406d00, Player_AiTradeStocks);

// 0x4073a0. AI player p's turn (+4 computer, +0x1e1 not out). While seats
// are left (companies + 3 < 32), world rules allow it (+0x4ad3; +0x4ad7 only
// for players without +0x2a7), it has no company (+0x1dd), its wait since
// the start date (+0x1e5) is over, it has more than 80000 (+0x154) and the
// editor allows (+0x29b), it may found a company: its urge is the number of
// companies over the map's size (+0x21a9 / 65000 + 2, x (60 - years since
// the start, at most 30) / 60), x (200 - personality) % and HashRand(80,
// 120) %. Above 1 (unless +0x4a7f) it waits 0x76200 minutes more; when no
// first route can be planned (AI_TestFirstRoute) 0x24ea00. Otherwise its
// cash (above 3 million only half counts, in thousands) plus a top-up to 1.5
// million (at most 9 x its cash) founds a company (virtual create, then
// Company_Init), trigger 7 fires and it buys shares for its cash + 8000 at
// the share price (in thousands, at most the company's +0x47), paying for
// them. The company takes its first turn; when it was flagged for news
// (+0xd39) the founding is announced (0xb49). A player with a company, or one
// that waited, trades stocks (0x406d00).
RT3_NOINLINE void __fastcall Player_AiTurn(uint8_t* p) {
    if (!F<int32_t>(p, 4) || F<int32_t>(p, 0x1e1)) return;
    [&] {
        Collection* const companies = hook::Global<Collection*>(0x62be10);
        uint8_t* const w = hook::Global<uint8_t*>(0x6cec78);
        if (Collection_Count(companies) + 3 >= 0x20) return;
        if (F<int32_t>(w, 0x4ad3)) return;
        if (F<int32_t>(w, 0x4ad7) && F<int32_t>(p, 0x2a7) > 0) return;
        if (F<int32_t>(p, 0x1dd)) return;
        if (F<int32_t>(w, 0x15) - DateToMinutes((const GameDate*)(w + 5)) < F<int32_t>(p, 0x1e5)) return;
        if (!(F<double>(p, 0x154) > D(0x5c8830))) return;
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x29b)) return;
        float scale = (float)((double)F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x21a9) / D(0x5c88b0) + D(0x5c85d0));
        int32_t years = (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 5);
        if (years >= 0x1e) years = 0x1e;
        scale = (float)((double)(0x3c - years) / D(0x5c88a8) * scale);
        const float density = (float)((double)(float)Table_CountField3f(hook::Global<Collection*>(0x62be10)) / scale);
        float urge = (float)((double)(0xc8 - hook::Global<int32_t>(0x5f2d2b + 0x27 * (int8_t)p[0x291])) / D(0x5c85d8) * density);
        urge = (float)((double)HashRand(F<int32_t>(p, 0x29a), 0x50, 0x78) / D(0x5c85d8) * urge);
        const bool always = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a7f) != 0;
        if (!(urge <= D(0x5c8770)) && !always) {
            F<int32_t>(p, 0x1e5) += 0x76200;
            p[0x29e] = 1;
            return;
        }
        if (!AI_TestFirstRoute()) {
            F<int32_t>(p, 0x1e5) += 0x24ea00;
            p[0x29e] = 1;
            return;
        }
        int32_t cash = Ftol(F<double>(p, 0x154));
        if (cash > 3000000) cash = ((cash - 3000000) >> 1) + 3000000;
        const int32_t base = cash / 1000 * 1000;
        int32_t topUp = base < 1500000 ? 1500000 - base : 0;
        if (topUp >= base * 9) topUp = base * 9;
        Collection* const all = hook::Global<Collection*>(0x62be10);
        const int32_t id = ((int32_t(__fastcall*)(Collection*, int, int32_t, int32_t, int32_t, int32_t))(*(void***)all)[1])(all, 0, 0, 0, 0, 0);
        uint8_t* const co = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)id);
        Company_Init(co, 0, id, *(int32_t*)p, -1, nullptr, nullptr, topUp / 1000 * 1000 + base);
        p[0x29e] = 0;
        Events_Fire(hook::Global<Collection*>(0x62be18), 0, 7, nullptr, 0);
        const double funds = F<double>(p, 0x154) + D(0x5c86d0);
        const int32_t price = Ftol(F<float>(co, 0x57));
        int32_t shares = Ftol(funds / (double)price / D(0x5c87f8)) * 1000;
        if (shares >= F<int32_t>(co, 0x47)) shares = F<int32_t>(co, 0x47);
        F<int32_t>(p, 0x15d + 4 * id) = shares;
        F<double>(p, 0x154) = F<double>(p, 0x154) - (double)(Ftol(F<float>(co, 0x57)) * shares);
        co[0xd39] = 1;
        AI_CompanyTurn(co);
        if (!co[0xd39]) return;
        co[0xd39] = 0;
        char text[0x1e8];
        const char* fmt = GetString(0xb49);
        FormatText(text, fmt, (const char*)p + 8, (const char*)co + 4, 0, 0, 0, 0, 0, 0);
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x12, id, 0);
    }();
    if (F<int32_t>(p, 0x1dd) || p[0x29e]) Player_AiTradeStocks(p);
}
RT3_REPLACE(0x4073a0, Player_AiTurn);

// 0x407780. Allocates 32 zeroed 0x1a-byte entries, pointers at +0x5bd.
void __fastcall Obj_AllocEntries(void* o) {
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    void** a = (void**)malloc_(0x80);
    F<void**>(o, 0x5bd) = a;
    for (int i = 0; i < 32; ++i) {
        uint8_t* e = (uint8_t*)malloc_(0x1a);
        F<void**>(o, 0x5bd)[i] = e;
        for (int k = 0; k < 0x1a; ++k) e[k] = 0;
    }
}
RT3_REPLACE(0x407780, Obj_AllocEntries);

// 0x4077e0. Frees the 32 entries of +0x5bd (and each entry's buffer +6), then the array.
RT3_NOINLINE void __fastcall Obj_FreeEntries5bd(void* o) {
    for (int32_t i = 0; i < 32; ++i) {
        uint8_t* e = F<uint8_t**>(o, 0x5bd)[i];
        if (F<void*>(e, 6)) Free(F<void*>(e, 6));
        Free(F<uint8_t**>(o, 0x5bd)[i]);
    }
    Free(F<void*>(o, 0x5bd));
    F<void*>(o, 0x5bd) = nullptr;
}
RT3_REPLACE(0x4077e0, Obj_FreeEntries5bd);

// 0x407850 / 0x47efb0 / 0x508700 (vtables). Lookups through +0x72 / +4 /
// +0x7c.
RT3_NOINLINE void* __fastcall Obj_Cargo72(uint8_t* o) { return F<uint32_t>(o, 0x72) ? Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(o, 0x72)) : nullptr; }
RT3_REPLACE(0x407850, Obj_Cargo72);

// 0x407870. Sends message type (size bytes at data) to the player named at
// +0x8f48, reliable (flags 5).
RT3_NOINLINE void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data) {
    Net_Send(s, 0, (const char*)s + 0x8f48, type, size, data, 5, 0);
}
RT3_REPLACE(0x407870, Net_SendToHost);

// 0x4078a0. The engine type the AI company buys: the best score of speed x
// sqrt(reliability + k) x (30 - +0xc) x (+0x18 + 10) x (+0x1c + 15) / (age + 30) /
// running cost, times a random 80..120%, halved for slow engines. Passes relax the
// rules: pass 0 only finds the top speed; kind 2 from pass 2, unavailable ones in
// pass 3. Falls back to the engine for the year.
RT3_NOINLINE uint32_t __fastcall AI_PickEngineType(uint32_t company) {
    int32_t best = -1;
    float bestScore = -1.0f, maxSpeed = 0.0f;
    for (int32_t pass = 0; pass < 4; ++pass) {
        if (best != -1) continue;
        for (int32_t i = 0; i < Collection_Count(Engines()); ++i) {
            uint8_t* e = (uint8_t*)Collection_Get(Engines(), 0, Collection_IdAt(Engines(), 0, i, 0));
            uint8_t* ct = CarTypeOf(e);
            if (F<int32_t>(e, 0x10) == 2 && pass < 2) continue;
            Collection* players = hook::Global<Collection*>(0x62be10);
            uint32_t player = Collection_IsValid(players, 0, company)
                ? F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, company), 0x3b) : 0;
            if (!CarType_Available(ct, 0, company, player) && pass < 3) continue;
            if (!(maxSpeed > F<float>(e, 0x20))) maxSpeed = F<float>(e, 0x20);
            if (!pass) continue;
            float s = F<float>(e, 0x20);
            if (!Strnicmp((const char*)e + 0x34, (const char*)0x5c88d0, 4) && Game_EffectiveVersion() >= 0x68)
                s = (float)(s * D(0x5c88c8));
            float rel = F<float>(e, 0x28) > Fl(0x5c85b4) ? F<float>(e, 0x28) : 1.0f;
            float q = (float)Sqrt((double)rel + Fl(0x5c8568));
            s = (float)((double)q * s);
            s = (float)((double)(float)(30 - F<int32_t>(e, 0xc)) * s);
            s = (float)((double)(float)(F<int32_t>(e, 0x18) + 10) * s);
            s = (float)((double)(float)(F<int32_t>(e, 0x1c) + 15) * s);
            int32_t age = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(CarTypeOf(e), 0x105);
            float denom = (float)((age <= 0 ? 0 : age) + 30);
            s = (float)((double)s / denom);
            s = (float)((double)s * Fl(0x5c88c0));
            s = (float)((double)s / ((double)Obj_EffectScale(e, 0, company, player) + Fl(0x5c88bc)));
            int32_t r = SimRand(0x50, 0x78);
            s = (float)((double)r / D(0x5c85d8) * s);
            float sp = F<float>(e, 0x20);
            if ((double)sp + sp <= maxSpeed && sp <= Fl(0x5c88b8)) s = (float)(s * D(0x5c8628));
            if (s > bestScore) {
                best = (int32_t)Collection_IdAt(Engines(), 0, i, 0);
                bestScore = s;
            }
        }
    }
    if (best == -1) best = (int32_t)EngineTypes_PickForYear(Engines());
    return best > 1 ? best : 1;
}
RT3_REPLACE(0x4078a0, AI_PickEngineType);

// 0x407bd0. Re-evaluates the company's (id c[0]) station records (+0x5bd[id]): for
// each active station, every free (non-city) cargo on offer is priced at its local
// value (quota / price multiplier) and matched with the best-paying linked
// destination (record list +6, 13 bytes: on, station, distance, rate) by gain per
// distance (+2000 margin); the totals go to +0x12 (supply) and +0xe (gain), and the
// best ratios fill up to 6 loads into +0x16 (expected rate). Dated +0xa / +0xd3a.
RT3_NOINLINE void __fastcall Company_RateStations(int32_t* co) {
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    F<int32_t>(co, 0xd3a) = F<int32_t>(world, 0x15);
    Collection* stations = hook::Global<Collection*>(0x6cec20);
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
        stations = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
        auto rec = [&]() { return F<uint8_t**>(s, 0x5bd)[co[0]]; };
        if (!rec()[0] || !rec()[1]) continue;
        F<int32_t>(rec(), 0xa) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
        F<int32_t>(rec(), 0xe) = 0;
        F<int32_t>(rec(), 0x12) = 0;
        F<int32_t>(rec(), 0x16) = 0;
        int32_t n = Obj_CountAvailableCargos(s);
        float stock[0x35] = {}, value[0x35] = {}, ratio[0x35], gain[0x35];
        uint8_t has[0x35] = {}, used[0x35];
        int32_t lo = 9999, hi = 0;
        for (int32_t k = 0; k < n; ++k) {
            float amount, quota;
            uint32_t id = Obj_NthAvailableCargo(s, 0, k + 1, &amount, &quota, 0);
            if (F<int32_t>(hook::Global<Collection*>(0x62ba8c), 0x9a + id * 4) != -1) continue;
            if (!(amount > Fl_202(0x5c8598))) continue;
            if (lo >= (int32_t)id) lo = id;
            if (hi <= (int32_t)id) hi = id;
            stock[id] = amount;
            float mul = (float)Effects_CargoMultiplier(id);
            double d = D(0x5c85b8) > (double)mul ? 0.01 : (double)mul;
            value[id] = (float)((double)quota / d);
        }
        for (int32_t j = 0; j < F<int32_t>(rec(), 2); ++j) {
            uint8_t* e = F<uint8_t*>(rec(), 6) + j * 0xd;
            if (!e[0]) continue;
            const uint16_t* want =
                (const uint16_t*)Obj_BuildingRegion(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(e, 1)));
            for (int32_t c = lo; c <= hi; ++c) {
                if (!(stock[c] > Fl_202(0x5c8598))) continue;
                float q = (float)((int32_t)want[c] << 4);
                if (!((double)value[c] + Fl_202(0x5c88e0) < q)) continue;
                float g = (float)(((double)q - value[c]) * F<float>(e, 9));
                float dist = (float)(uint32_t)(F<int32_t>(e, 5) + 0x28);
                float r = (float)((double)g / dist);
                if (has[c] && !(r > ratio[c])) continue;
                gain[c] = g;
                has[c] = 1;
                ratio[c] = r;
            }
        }
        for (int32_t c = lo; c <= hi; ++c) {
            if (!has[c]) continue;
            F<float>(rec(), 0x12) = (float)((double)stock[c] + F<float>(rec(), 0x12));
            F<float>(rec(), 0xe) = (float)((double)gain[c] + F<float>(rec(), 0xe));
        }
        memset(used, 0, sizeof used);
        float loads = 0.0f;
        for (;;) {
            int32_t best = -1;
            for (int32_t c = lo; c <= hi; ++c)
                if (has[c] && !used[c] && (best == -1 || ratio[c] > ratio[best])) best = c;
            if (best == -1) break;
            used[best] = 1;
            double room = D(0x5c8610) - loads;
            float take = (float)(room < (double)stock[best] ? room : (double)stock[best]);
            loads = (float)((double)take + loads);
            F<float>(rec(), 0x16) = (float)((double)take * ratio[best] + F<float>(rec(), 0x16));
            if (loads > D(0x5c88d8)) break;
        }
    }
}
RT3_REPLACE(0x407bd0, Company_RateStations);

// 0x408280. The company's best site by its per-company entry value (+0x16;
// +1000 for unshared own sites); -1 if none.
RT3_NOINLINE int32_t __fastcall Company_BestSite(int32_t* c) {
    int32_t bestId = -1;
    float bestVal = 0.0f;
    int32_t n = Collection_Count(g_table6cec20);
    if (n <= 0) return -1;
    for (int32_t i = 0; i < Collection_Count(g_table6cec20); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        uint8_t* e = F<uint8_t**>(s, 0x5bd)[c[0]];
        if (!e[0] || !e[1] || !F<int32_t>(e, 0xa) || Obj_BuildingValue(s) != c[0]) continue;
        float v = F<float>(F<uint8_t**>(s, 0x5bd)[c[0]], 0x16);
        if (Obj_BuildingValue(s) == c[0] && !F<int32_t>(s, 0x5c1) && !F<int32_t>(s, 0x5c5))
            v = (float)((double)v + *(const float*)0x5c86a0);
        if (bestId == -1 || v > bestVal) {
            bestId = F<int32_t>(s, 0);
            bestVal = v;
        }
    }
    return bestId;
}
RT3_REPLACE(0x408280, Company_BestSite);

// 0x408380. Picks where company `c` (its id at +0) should send a train
// loading at station `from`. It reads what `from` has on offer (cargo,
// amount, price corrected by the cargo multiplier). For every enabled link in
// the station's list for the company (+0x5bd[id]: count +2, 13-byte entries
// +6 of {on, station, distance, weight}) it fills a train of this year's
// length with the most profitable cargos. The score is (profit + bonus) *
// weight / (distance + 40), scaled down for weak links. The bonus grows
// shorter on longer links, is for an own station unless it has trains yet
// (+0x5c1, +0x5c5 when the last one left), and is cut by the trains already
// there. The best link becomes order `o` (0x33 bytes, station +0x29). If
// `train` is given and cannot reach it, the order goes to the nearest active
// station it can reach (own stations count 2000 closer). Returns 0 when there
// is nowhere to go.
RT3_NOINLINE int32_t __fastcall Company_PickTrainOrder(int32_t* c, int, uint8_t* o, uint32_t from, uint8_t* train) {
    int32_t best;
    uint8_t* st;
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, from) ||
        !(st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, from)))
        best = Company_BestSite(c);
    else {
        int32_t n = Obj_CountAvailableCargos(st);
        float avail[0x35], price[0x35], profit[0x35];
        memset(avail, 0, sizeof avail);
        memset(price, 0, sizeof price);
        int32_t lo = 9999, hi = 0;
        for (int32_t i = 1; i <= n; ++i) {
            float amount, value;
            int32_t cg = (int32_t)Obj_NthAvailableCargo(st, 0, i, &amount, &value, 0);
            if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cg * 4) != -1 || !(amount > Fl_205(0x5c8598))) continue;
            if (!(lo < cg)) lo = cg;
            if (!(hi > cg)) hi = cg;
            avail[cg] = amount;
            float m = (float)Effects_CargoMultiplier(cg);
            double d = m;
            double div = D(0x5c85b8) > d ? BitsD(0x3f847ae147ae147bull) : d;
            price[cg] = (float)(value / div);
        }
        auto list = [&] { return F<uint8_t*>(F<uint8_t*>(st, 0x5bd), *c * 4); };
        best = -1;
        float bestScore = BitsF(0xc61c3c00u);
        for (int32_t j = 0; j < F<int32_t>(list(), 2); ++j) {
            uint8_t* e = F<uint8_t*>(list(), 6) + j * 0xd;
            if (!e[0]) continue;
            memset(profit, 0, sizeof profit);
            uint8_t* to = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(e, 1));
            uint16_t* pays = (uint16_t*)Obj_BuildingRegion(to);
            for (int32_t cg = lo; cg <= hi; ++cg) {
                if (!(avail[cg] > Fl_205(0x5c8598))) continue;
                float pv = (float)(int32_t)((uint32_t)pays[cg] << 4);
                if ((double)price[cg] + Fl_205(0x5c88e0) < pv) profit[cg] = (float)(((double)pv - price[cg]) * F<float>(e, 9));
            }
            float cars = (float)TrainCarsForYear();
            float total = 0.0f, filled = 0.0f;
            uint8_t used[0x35];
            memset(used, 0, sizeof used);
            do {
                int32_t pick = -1;
                for (int32_t cg = lo; cg <= hi; ++cg) {
                    if (profit[cg] < Fl_205(0x5c8598) || profit[cg] == Fl_205(0x5c8598)) continue;
                    if (used[cg]) continue;
                    if (pick != -1 && !(profit[cg] > profit[pick])) continue;
                    pick = cg;
                }
                if (pick == -1) break;
                used[pick] = 1;
                float take = cars - filled;
                take = take < avail[pick] ? take : avail[pick];
                filled = take + filled;
                total = (float)((double)take * profit[pick] + total);
            } while (!(cars - D(0x5c8918) < filled));
            uint8_t* l = list();
            float bonus = (float)(F<float>(l, 0x16) * D(0x5c8628));
            uint32_t dist = F<uint32_t>(e, 5) + 100;
            double dd = (double)(int32_t)dist;
            if ((int32_t)dist < 0) dd += D(0x5c8910);
            float f = (float)(D(0x5c8908) / dd);
            dd = f;
            f = (float)(D(0x5c8660) < dd ? BitsD(0x3ff4cccccccccccdull) : dd);
            bonus = f * bonus;
            if (Obj_BuildingValue(st) == *c && F<int32_t>(st, 0x5c1) == 0) {
                int32_t left = F<int32_t>(st, 0x5c5);
                int32_t months = (left - F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15)) / 0x9d80;
                if (left == 0)
                    bonus = (float)((double)bonus * Fl_205(0x5c8898) + Fl_205(0x5c8904));
                else if (months > 0x3c)
                    bonus = bonus * Fl_205(0x5c8898);
                else if (months > 0x24)
                    bonus = bonus * Fl_205(0x5c8554);
                else if (months > 0x18)
                    bonus = bonus * Fl_205(0x5c8900);
                else if (months > 0xc)
                    bonus = (float)(bonus * D(0x5c88f8));
                else if (months > 6)
                    bonus = (float)(bonus * D(0x5c88f0));
            }
            int32_t trains = F<int32_t>(st, 0x5c1);
            if (trains > 0) {
                float k = (float)(trains < 1 ? 1 : trains);
                float r = F<float>(list(), 0x12) / k;
                double rd = r;
                if (rd < D(0x5c88e8))
                    bonus = 0.0f;
                else if (rd < D(0x5c8640))
                    bonus = (float)(bonus * D(0x5c88c8));
                else if (rd < D(0x5c8698))
                    bonus = (float)(bonus * D(0x5c8628));
                else if (rd < D(0x5c88f8))
                    bonus = (float)(bonus * D(0x5c8680));
                else if (rd < D(0x5c87d8))
                    bonus = (float)(bonus * D(0x5c85a0));
            }
            uint32_t len = F<uint32_t>(e, 5) + 0x28;
            double ld = (double)(int32_t)len;
            if ((int32_t)len < 0) ld += Fl_205(0x5c872c);
            float lf = (float)ld;
            float score = (float)(((double)bonus + total) * F<float>(e, 9) / lf);
            double w = F<float>(e, 9);
            if (w < D(0x5c86b8))
                score = (float)(score * D(0x5c85b8));
            else if (w < D(0x5c88c8))
                score = (float)(score * D(0x5c88c8));
            else if (w < D(0x5c8628))
                score = (float)(score * D(0x5c8750));
            else if (w < D(0x5c8750))
                score = (float)(score * D(0x5c8888));
            if (score > bestScore) {
                best = F<int32_t>(e, 1);
                bestScore = score;
            }
        }
    }
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)best)) return 0;
    uint8_t* dst = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, (uint32_t)best);
    memset(o, 0, 0x33);
    o[0x28] &= 0xf;
    F<uint32_t>(o, 0x2f) = 0x3e395810;
    o[0x22] = 0;
    o[0x23] = (uint8_t)TrainCarsForYear();
    F<uint16_t>(o, 0x29) = (uint16_t)best;
    F<int32_t>(o, 0x24) = 900;
    if (!train) return 1;
    int32_t needFlag = F<int32_t>((uint8_t*)Obj_CityRef(train), 0x10) == 2;
    if (Route_CanReach(F<int32_t>(train, 0x49), F<int32_t>(dst, 8), needFlag)) return 1;
    int32_t bestDist = -1, bestId = -1;
    float tx, ty;
    Train_RearPoint(train, 0, &tx, &ty);
    for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++j) {
        Collection* stations = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, j, 0));
        if (!Obj_BuildingStateActive(s)) continue;
        if (F<int32_t>(s, 0) == F<int32_t>(train, 0xa0)) continue;
        if (!Route_CanReach(F<int32_t>(train, 0x49), F<int32_t>(s, 8), needFlag)) continue;
        int32_t sy = Ftol(Obj_BuildingCellY(s));
        int32_t sx = Ftol(Obj_BuildingCellX(s));
        int32_t y1 = Ftol(ty);
        int32_t x1 = Ftol(tx);
        int32_t d = (uint16_t)DistanceInt(x1, y1, sx, sy);
        if (d <= 4) continue;
        int32_t score = 2000 - d;
        if (Obj_BuildingValue(s) == *c) score += 2000;
        if (score > bestDist) {
            bestDist = score;
            bestId = F<int32_t>(s, 0);
        }
    }
    if (bestId == -1) return 0;
    F<uint16_t>(o, 0x29) = (uint16_t)bestId;
    return 1;
}
RT3_REPLACE(0x408380, Company_PickTrainOrder);

// 0x408f70. Expected rate for company c (id c[0]) from its active stations' records
// (+0x5bd[id], +0x12): the average over a year-dependent divisor x 0.25, stretched by
// the distance between the first two stations when it has exactly two (v1.03+),
// scaled by era and station count, then clamped to [avg / 2, avg] (2 when <= the
// minimum).
RT3_NOINLINE float __fastcall Company_StationRate(int32_t* c) {
    Collection* stations = hook::Global<Collection*>(0x6cec20);
    float sum = 0.0f;
    int32_t count = 0;
    uint8_t* first = nullptr;
    uint8_t* second = nullptr;
    for (int32_t i = 0; i < Collection_Count(stations); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
        int32_t id = c[0];
        uint8_t* rec = F<uint8_t**>(s, 0x5bd)[id];
        if (!rec[0] || !rec[1] || Obj_BuildingValue(s) != id || !Obj_BuildingStateActive(s)) continue;
        if (!first) first = s;
        else if (!second) second = s;
        sum = (float)((double)sum + F<float>(F<uint8_t**>(s, 0x5bd)[c[0]], 0x12));
        ++count;
    }
    int32_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    int32_t div = year < 0x749 ? 4 : year < 0x758 ? 5 : year < 0x76c ? 6 : year < 0x780 ? 5 : year < 0x78f ? 6
                : year < 0x79e ? 7 : year < 0x7b2 ? 6 : (year >= 0x7c6) + 7;
    float avg = (float)((double)sum / (float)div);
    float rate = (float)(avg * D(0x5c8680));
    if (Game_EffectiveVersion() >= 0x67 && count == 2) {
        int32_t y2 = Ftol(Obj_BuildingCellY(second));
        int32_t x2 = Ftol(Obj_BuildingCellX(second));
        int32_t y1 = Ftol(Obj_BuildingCellY(first));
        int32_t x1 = Ftol(Obj_BuildingCellX(first));
        float dist = (float)(int32_t)(uint16_t)DistanceInt(x1, y1, x2, y2);
        uint16_t y = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
        float k = 0.85f;
        if (y >= 0x79e) k = 1.55f;
        else if (y >= 0x753) k = 1.15f;
        if (dist > Fl_193(0x5c8940)) rate = (float)(((double)dist / D(0x5c8938) + k) / ((double)k + D(0x5c8770)) * rate);
    }
    year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    uint32_t era = year < 0x73a ? 0x5c88f0 : year < 0x74e ? 0x5c8660 : year < 0x762 ? 0x5c8878 : year < 0x776 ? 0x5c85a8
                 : year < 0x78a ? 0x5c8930 : year < 0x79e ? 0x5c8928 : year < 0x7b2 ? 0x5c85a0 : 0x5c8920;
    rate = (float)(rate * D(era));
    if (count <= 4) rate = (float)(rate * D(0x5c8888));
    else if (count > 8) rate = (float)(rate * D(count <= 12 ? 0x5c85a8 : count <= 20 ? 0x5c8698 : 0x5c8660));
    if (!(rate < avg)) rate = avg;
    double half = avg * D(0x5c8628);
    float r = (float)(rate > half ? (double)rate : half);
    if (!(r > Fl_193(0x5c878c))) r = 2.0f;
    return r;
}
RT3_REPLACE(0x408f70, Company_StationRate);

// 0x409300. AI company c upgrades train `train` (unless 0x6cec78+0x4a97):
// it gets the company's preferred engine (AI_PickEngineType, 0x4aefb0) and
// a log line 0xb51 names the company, the old and the new engine; +0xd56.
RT3_NOINLINE void __fastcall AiCompany_ReplaceEngine(uint8_t* c, int, uint32_t train) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a97)) return;
    uint32_t engine = AI_PickEngineType(F<uint32_t>(c, 0));
    int32_t old = Train_FirstCarType126((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, train));
    ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x4aefb0)((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, train), 0, engine, 0);
    Collection* types = hook::Global<Collection*>(0x6ada84);
    void* oldT = Collection_Get(types, 0, (uint32_t)old);
    void* newT = Collection_Get(types, 0, engine);
    const char* newName = (const char*)(uintptr_t)Obj_RefName72(newT);
    const char* oldName = (const char*)(uintptr_t)Obj_RefName72(oldT);
    char buf[0x180];
    FormatText(buf, GetString(0xb51), (const char*)c + 4, oldName, newName, nullptr, nullptr, nullptr, nullptr, nullptr);
    Log_Add(hook::Global<Collection*>(0x6acd34), 0, buf, 8, 0, 0);
    c[0xd56] = 1;
}
RT3_REPLACE(0x409300, AiCompany_ReplaceEngine);

// 0x4093d0. AI company: rebuilds its station link table. Per station the
// company's entry (+0x5bd[company]: +0 valid, +1 usable, +2 count, +6 records)
// is reset; usable are its own active stations and others it is connected to
// that lie near one of its sites. Each usable station gets one 13-byte record
// per usable station: linked (+0), the other's id (+1), the route length (+5)
// and the share of it on own track (+9) - copied from the mirror record for
// earlier stations. Then the stations are rated.
RT3_NOINLINE void __fastcall Company_BuildStationLinks(int32_t* co) {
    F<uint32_t>(co, 0xd3e) = F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    Collection* sts = hook::Global<Collection*>(0x6cec20);
    int32_t n = Collection_Count(sts);
    auto st = [&](int32_t i) { return (uint8_t*)Collection_Get(sts, 0, Collection_IdAt(sts, 0, i, 0)); };
    auto entry = [&](uint8_t* s) { return F<uint8_t*>(F<uint8_t*>(s, 0x5bd), co[0] * 4); };
    int32_t marked = 0;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* s = st(i);
        entry(s)[0] = 1;
        entry(s)[1] = 0;
        if (F<void*>(entry(s), 6)) Free(F<void*>(entry(s), 6));
        F<void*>(entry(s), 6) = nullptr;
        if (!Obj_BuildingStateActive(s) || Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        bool use = Obj_BuildingValue(s) == co[0];
        if (!use && Station_ConnectedToHome(s, 0, (uint32_t)co[0])) {
            int32_t y = Ftol(Obj_BuildingCellY(s));
            int32_t x = Ftol(Obj_BuildingCellX(s));
            float d = Company_NearestSiteDistance(co, 0, x, y);
            use = !(d >= Fl_205(0x5c8944));
        }
        if (use) {
            entry(s)[1] = 1;
            ++marked;
        }
    }
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    uint32_t iofs = 0;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* si = st(i);
        if (!entry(si)[1]) continue;
        F<int32_t>(entry(si), 2) = marked;
        size_t size = marked * 0xd + 1;
        F<void*>(entry(si), 6) = malloc_(size);
        memset(F<void*>(entry(si), 6), 0, size);
        uint32_t jofs = 0;
        for (int32_t j = 0; j < n; ++j) {
            uint8_t* sj = st(j);
            if (!entry(sj)[1]) continue;
            uint8_t* rec = F<uint8_t*>(entry(si), 6) + jofs;
            if (j == i) rec[0] = 0;
            else if (j < i) {
                memcpy(rec, F<uint8_t*>(entry(sj), 6) + iofs, 0xd);
                F<uint32_t>(rec, 1) = F<uint32_t>(sj, 0);
            } else {
                F<uint32_t>(rec, 1) = F<uint32_t>(sj, 0);
                if (!Node_SameComponent((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(si, 8)), 0, F<uint32_t>(sj, 8)))
                    rec[0] = 0;
                else {
                    rec[0] = 1;
                    uint32_t cost, foreign;
                    Route_NextStep(hook::Global<Collection*>(0x6cfcb4), 0, F<int32_t>(si, 8), F<int32_t>(sj, 8), &cost, 0, (uint32_t*)co, &foreign, 0);
                    if (cost == 0xffffffff) rec[0] = 0;
                    else {
                        uint32_t own = cost - foreign;
                        F<uint32_t>(rec, 5) = cost;
                        uint32_t div = cost > 1 ? cost : 1;
                        double fo = (double)(int32_t)own;
                        if ((int32_t)own < 0) fo += Fl_205(0x5c872c);
                        double fd = (double)(int32_t)div;
                        if ((int32_t)div < 0) fd += Fl_205(0x5c872c);
                        F<float>(rec, 9) = (float)((double)(float)fo / (float)fd);
                    }
                }
            }
            jofs += 0xd;
        }
        iofs += 0xd;
    }
    Company_RateStations(co);
}
RT3_REPLACE(0x4093d0, Company_BuildStationLinks);

// 0x409720. AI company: rebuilds its station link table when it is missing or
// older than 0x7ff80 world ticks, and re-rates its stations every 0x31380.
RT3_NOINLINE void __fastcall Company_UpdateStationLinks(int32_t* co) {
    uint8_t* c = (uint8_t*)co;
    if (!F<int32_t>(c, 0xd3e) || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15) - F<int32_t>(c, 0xd3e) > 0x7ff80) Company_BuildStationLinks(co);
    if (!F<int32_t>(c, 0xd3a) || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15) - F<int32_t>(c, 0xd3a) > 0x31380) Company_RateStations(co);
}
RT3_REPLACE(0x409720, Company_UpdateStationLinks);

// 0x409770. AI train t (orders at +0): unless its company is player-owned
// (+0x1e1), and unless it already runs a two-stop route with both orders,
// it gets a new order from Company_PickTrainOrder (from its target +0xa0) -
// appended and made current with fewer than two orders, else overwriting the
// entry after the current one. Stops it (+0xc cleared, running 2) first.
RT3_NOINLINE void __fastcall AiTrain_NextOrder(uint8_t* t) {
    uint8_t e[0x34];
    if (Company_OwnerFlag1e1(Train_Company(t))) return;
    if (F<int32_t>(Obj_CityRef(t), 0x10) == 2 && F<int32_t>(t, 4) >= 2) return;
    t[0xc] = 0;
    Train_SetRunning(t, 0, 2);
    Company_UpdateStationLinks((int32_t*)Train_Company(t));
    if (!Company_PickTrainOrder((int32_t*)Train_Company(t), 0, e, F<uint32_t>(t, 0xa0), t)) return;
    int32_t n = F<int32_t>(t, 4);
    if (n < 2) {
        TrainOrders_Insert(t, 0, n, e);
        TrainOrders_SetCurrent(t, 0, F<int32_t>(t, 4) - 1, 0);
    } else {
        memcpy(F<uint8_t*>(t, 0) + (F<int32_t>(t, 8) + 1) % n * 0x33, e, 0x33);
    }
}
RT3_REPLACE(0x409770, AiTrain_NextOrder);

// 0x409830 (thiscall). AI company co buys a train (unless 0x6cec78 +0x4aa3
// forbids it): after updating its station links it picks a first stop and
// a second one from there (0x408380), buys the engine type it prefers
// (0x4078a0) running between the two, logs "company bought engine at
// station" (0xb50) and marks +0xd56.
RT3_NOINLINE void __fastcall AI_BuyTrain(int32_t* co) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aa3)) return;
    uint8_t orders[0x28];
    uint8_t first[0x34], second[0x34];
    char text[0x190];
    Cursor33_Init(orders);
    Company_UpdateStationLinks(co);
    const int32_t a = Company_PickTrainOrder(co, 0, first, 0, nullptr);
    const int32_t b = Company_PickTrainOrder(co, 0, second, *(const uint16_t*)(first + 0x29), nullptr);
    if (!a || !b) return;
    TrainOrders_Insert(orders, 0, 0, first);
    TrainOrders_Insert(orders, 0, 1, second);
    F<int32_t>(orders, 8) = 0;
    const uint32_t engine = AI_PickEngineType((uint32_t)*co);
    Train_Create(engine, orders, 1, *co);
    uint8_t* st = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, *(const uint16_t*)(first + 0x29));
    const char* name = (const char*)(uintptr_t)Obj_RefName72(Collection_Get(hook::Global<Collection*>(0x6ada84), 0, engine));
    FormatText(text, GetString(0xb50), (const char*)(co + 1), name, (const char*)(st + 0x46b), 0, 0, 0, 0, 0);
    Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 8, 0, 0);
    ((uint8_t*)co)[0xd56] = 1;
}
RT3_REPLACE(0x409830, AI_BuyTrain);

// 0x409950 (RET 4). AI company co (with cargo sites) reviews its trains
// against its station rate (0x408f70). Ages count by era (x 1.3 before 1850
// down to x 0.8 from 1950). With more trains than the rate, one aged 33+
// earning at most 30000 (0x4a88e0) is sold (not in multiplayer, nor with
// world +0x4a9b), or one aged 27+ when it is 3 over (one per review); with
// fewer, an aged 33+ one (27+ when 3 short) gets a new engine (0x409300).
// Then per 16 missing a train is bought (0x409830; in multiplayer the host
// is asked, message 0x75), a company without trains always gets one, and
// another with some still missing when `built`, with fewer than 2 trains, or
// by chance (missing x 10 % against SimRand(0, 100)).
RT3_NOINLINE void __fastcall AI_ReviewTrains(uint8_t* co, int, int32_t built) {
    if (Company_CountCargoSites(co, 0, 1) < 1) return;
    const float target = Company_StationRate((int32_t*)co);
    int32_t trains = Company_TrainCount(co);
    const int32_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    const float factor = RtBits(year < 0x73a ? 0x3fa66666 : year < 0x753 ? 0x3f99999a : year < 0x76c ? 0x3f8ccccd : year < 0x785 ? 0x3f800000 : year < 0x79e ? 0x3f666666
                                                                                                                                            : 0x3f4ccccd);
    auto sell = [&](uint8_t* t) {
        Collection* all = hook::Global<Collection*>(0x6cfcbc);
        ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)all)[2])(all, 0, F<uint32_t>(t, 0x25));
        --trains;
    };
    int32_t sold = 0;
    for (int32_t i = Collection_Count(hook::Global<Collection*>(0x6cfcbc)) - 1; i >= 0; --i) {
        Collection* all = hook::Global<Collection*>(0x6cfcbc);
        uint8_t* t = (uint8_t*)Collection_Get(all, 0, Collection_IdAt(all, 0, (uint32_t)i, 0));
        if (F<int32_t>(t, 0x51) != *(int32_t*)co) continue;
        const float age = (float)((double)(float)(year - F<int32_t>(t, 0xd4)) * factor);
        const float count = (float)trains;
        if (age >= Fl_207(0x5c8954)) {
            if (!(count > target)) {
                AiCompany_ReplaceEngine(co, 0, F<uint32_t>(t, 0x25));
            } else if (!IsMultiplayer() && Train_CargoRate(t, 0, 0) <= Fl_207(0x5c8950) && !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4a9b)) {
                sell(t);
                ++sold;
            }
        } else if (age >= Fl_207(0x5c894c)) {
            if (count > target) {
                if (!IsMultiplayer() && (double)target + Fl_207(0x5c8900) < count && !sold) {
                    sell(t);
                    sold = 1;
                }
            } else if ((double)target - Fl_207(0x5c8900) > count) {
                AiCompany_ReplaceEngine(co, 0, F<uint32_t>(t, 0x25));
            }
        }
    }
    auto buy = [&] {
        if (IsMultiplayer()) {
            if (hook::Global<int32_t>(0x6d40d0) && hook::Global<int32_t>(0x6d40dc)) {
                uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
                Net_Send(s, 0, (const char*)s + 0x8f48, 0x75, 4, co, 5, 0);
            }
        } else {
            ((void(__fastcall*)(uint8_t*))0x409830)(co);
        }
    };
    float missing = (float)((double)target - (float)trains);
    int32_t eager = built;
    if (missing >= Fl_207(0x5c8948)) {
        eager = 0;
        do {
            buy();
            ++trains;
            missing = (float)((double)missing - Fl_207(0x5c8948));
        } while (missing >= Fl_207(0x5c8948));
    }
    if (!trains) {
        buy();
        trains = 1;
        missing = (float)((double)missing - Fl_207(0x5c85b4));
    }
    if (missing > Fl_207(0x5c8598) && (eager || trains < 2)) {
        buy();
        return;
    }
    const float roll = (float)SimRand(0, 0x64);
    if ((double)missing * Fl_207(0x5c85f8) > roll) buy();
}
RT3_REPLACE(0x409950, AI_ReviewTrains);

// 0x409d00. Daylight factor for an hour of the day (mod 24): a piecewise-linear
// curve steeper by 2.2 (1 in arctic maps, world +0x4aeb), mirrored after midday.
RT3_NOINLINE float __stdcall Daylight_Factor(float hour) {
    float h = (float)((double(__cdecl*)(double, double))0x5a1390)(hour, 24.0);
    float slope = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aeb) ? 1.0f : 2.2f;
    if (h < Fl(0x5c8788)) {
        float v = (float)((double)slope * h);
        if (v < D(0x5c8980)) return v;
        float knee = (float)(D(0x5c8980) / slope);
        float s2 = (float)(D(0x5c8978) - knee);
        s2 = (float)(D(0x5c8970) / s2);
        return (float)(((double)h - knee) * s2 + D(0x5c8980));
    }
    h = (float)((double)Fl(0x5c8968) - h);
    float v = (float)((double)h * slope);
    if (v < D(0x5c8960)) {
        h = v;
    } else {
        float knee = (float)(D(0x5c8960) / slope);
        float s2 = (float)(D(0x5c8978) - knee);
        s2 = (float)(D(0x5c8958) / s2);
        h = (float)(((double)h - knee) * s2 + D(0x5c8960));
    }
    return (float)((double)Fl(0x5c8968) - h);
}
RT3_REPLACE(0x409d00, Daylight_Factor);

RT3_NOINLINE void* __fastcall Table_At(Collection* t, int, int32_t i) { return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)); }
RT3_REPLACE(0x409e60, Table_At);

// 0x409e80. Sets the world clock to `minutes` (+0x15; date +0xd/+0x11): derives the
// local hour of day (time-zone offset +0xbfa, +1 h unless +5 & 1), sets the scene
// clocks to it and to its daylight-curve time (or the fixed time of the game
// options +0x128 / +0x237), refreshes the camera haze when the sun moved, the era
// factors on a new year and the engine's clock (+0x2c = 2 x minutes).
RT3_NOINLINE void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes) {
    F<int32_t>(w, 0x15) = minutes;
    uint16_t oldYear = F<uint16_t>(w, 0xd);
    uint64_t date = DateFromMinutes(minutes);
    F<uint32_t>(w, 0xd) = (uint32_t)date;
    F<uint32_t>(w, 0x11) = (uint32_t)(date >> 32);
    float h = (float)((double)F<int32_t>(w, 0x15) / D(0x5c89a8));
    h = (float)((double)h - Fl_203(0x5c89a4));
    while (h < Fl_203(0x5c8598)) h = (float)((double)h + Fl_203(0x5c89a0));
    float t = (float)((double)h + Fl_203(0x5c89a0));
    t = (float)((double)t + F<float>(w, 0xbfa));
    t = (float)ExeFmod(t, 24.0);
    t = (float)((double)t + D(0x5c8998));
    if (!(w[5] & 1)) t = (float)((double)t + Fl_203(0x5c8788));
    t = (float)ExeFmod(t, 24.0);
    int32_t hours;
    float mins, secs;
    hours = Ftol(t);
    float hf = (float)hours;
    mins = (float)(((double)t - hf) * Fl_203(0x5c8990));
    int32_t minsInt = Ftol(mins);
    secs = (float)(((double)mins - (float)minsInt) * Fl_203(0x5c8990));
    float sun = Daylight_Factor(t);
    int32_t sh;
    float sm, ss;
    SplitHours(sun, &sh, &sm, &ss);
    uint8_t* game = hook::Global<uint8_t*>(0x6cec74);
    float clockH = sun;
    if (F<int32_t>(game, 0x124)) {
        clockH = (float)F<int32_t>(game, 0x128);
        ss = 0.0f;
        sm = 1.0f;
    } else if (F<int32_t>(game, 0x233) && hook::Global<uint8_t*>(0x6cec7c)[0x82]) {
        clockH = (float)F<int32_t>(game, 0x237);
        ss = 0.0f;
        sm = 1.0f;
    }
    if (void* eng = hook::Global<void*>(0x6d4024)) {
        int32_t s3 = Ftol(ss), m3 = Ftol(sm), h3 = Ftol(clockH);
        Clock_SetD((uint8_t*)Engine_Renderer(eng), 0, h3, m3, s3);
        if (void* e2 = hook::Global<void*>(0x6d4024)) Clock_Set11((uint8_t*)Engine_Renderer(e2), 0, hours, minsInt, Ftol(secs));
    }
    float& last = hook::Global<float>(0x5ed080);
    if (ExeFabs((double)last - clockH) >= D(0x5c8988)) {
        last = clockH;
        if (uint8_t* cam = hook::Global<uint8_t*>(0x62be68)) Camera_UpdateHaze(cam, 0, -999.0f);
    }
    if (oldYear != F<uint16_t>(w, 0xd)) Economy_UpdateEraFactors(w);
    if (uint8_t* eng = hook::Global<uint8_t*>(0x6d4024)) F<int32_t>(eng, 0x2c) = F<int32_t>(w, 0x15) * 2;
}
RT3_REPLACE(0x409e80, World_SetTime);

}  // namespace rt3
