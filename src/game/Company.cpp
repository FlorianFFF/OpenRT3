// Company.cpp - RT3.exe 0x424000-0x42c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/stream.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static Collection*& g_players = hook::Global<Collection*>(0x6ceb9c);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
static Collection*& g_trains     = hook::Global<Collection*>(0x6cfcbc);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
static Collection*& g_buildings  = hook::Global<Collection*>(0x62b26c);
static Collection*& g_companies = hook::Global<Collection*>(0x62be10);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c);
int32_t __fastcall Obj_BuildingValue(void* o);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
static Collection*& g_trackNodes  = hook::Global<Collection*>(0x6cfca8);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
static uint8_t*& g_newGame   = hook::Global<uint8_t*>(0x6cec7c);
double __fastcall Obj_Rounded29(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
int32_t __fastcall Company_FreeShares(void* c);
int32_t __fastcall Company_PlayerDebt(int32_t* o);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
void __fastcall Section_Enter(uint8_t* s);
void __fastcall Cursor_UseSystem(uint8_t* c);
void __fastcall Cursor_PopWindowsMode(uint8_t* c);
void* __fastcall Engine_Get1C(void* e);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
static Collection*& g_cargoTypes  = hook::Global<Collection*>(0x62ba8c);
int32_t __fastcall Company_ClampDates(void* c, int, int32_t);
void* __fastcall Obj_BuildingKind(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void __fastcall Obj_Set3c(void* o, int, uint16_t v);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
int32_t CountFreeCompanySlots();
int32_t __fastcall NthFreeCompanySlot(int32_t n);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall Game_EffectiveVersion();
double __fastcall Building_CargoOutput(uint8_t* b, int, int32_t cargo);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
int32_t __fastcall Obj_Get21(void* o);
double __fastcall Obj_WeightedAverage(void* o);
double __fastcall Company_TrackLength(void* c);
int32_t __fastcall Company_TrainCount(void* c);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Company_LiveStat(uint8_t* c, int, int32_t id);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
float __fastcall Obj_ValueWithBonus(uint32_t* o);
float __fastcall Company_BondRate(uint8_t* c);
float __fastcall Company_CashRate(uint8_t* c);
float __fastcall Company_BondRateWithDebt(uint8_t* c, int, int32_t extraDebt, int32_t);
int32_t CountHumanPlayers();
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
int32_t __fastcall Company_HumanHoldings(int32_t* o);
float __fastcall Company_QualityFactor(void* c);
float __fastcall Company_CashInterestRate(uint8_t* c);
float __fastcall Company_MaxDividend(uint8_t* c);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void __fastcall Company_GoBankrupt(uint8_t* c);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void* __fastcall Engine_Renderer(void* e);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall Obj_Set21(void* o, int, int32_t v);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Obj_Get25(void* o);
void __fastcall Engine_SaveConfig(uint8_t* e);
void* __fastcall Company_Owner(void* c);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall ModelObj_Destructor(uint8_t* o);
void __fastcall ModelObj_Release(uint8_t* o);
uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8);
int32_t __fastcall OpenFileDialog(char* file, const char* filter, const char* initialDir);
void __fastcall BaseNameNoSuffix(char* dst, const char* src);
void __fastcall PartModel_Destructor(uint8_t* o);
uint8_t* __fastcall PartModel_Construct(uint8_t* o);
void __fastcall Car_LoadModel(uint8_t* c, int, const char* name, const char* tex, float x, float z, int32_t reset);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Player_CompanyAgePenalty(uint8_t* p);
void __fastcall Company_Dissolve(uint8_t* c);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
void __stdcall View_RefreshCompanyLists();
void __fastcall Company_SetChairman(int32_t* c, int, int32_t player);
void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
void __fastcall Company_AnnualReview(uint8_t* c);
void __fastcall Track_SetOwner(uint8_t* t, int, uint16_t owner);
void __stdcall View_RefreshCompanyAssets(int32_t, int32_t company);
void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE void* __fastcall Company_Owner(void* c);
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void Debug_DropModel();
void Debug_PlaceTestCar();
void Debug_PlaceTestModel();
uint32_t __fastcall Engine_IsFullscreen(void* e);

// 0x42a660 / 0x42a6c0 / 0x42a720 (dev commands). Debug placements, refused
// (string 0xd1) in full screen.
#define RT3_WINDOWED(addr, name, call) \
    int32_t name() { \
        if (Engine_IsFullscreen(hook::Global<void*>(0x6d4024))) \
            Dialog_RunMessage(GetString(0xd1), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0); \
        else \
            call(); \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall AiCompany_ReplanTrains(int32_t* c);
RT3_NOINLINE void __fastcall Company_BuildStationLinks(int32_t* co);
int32_t __stdcall Return0_424000(int32_t);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void __fastcall Obj_SetSlot35b(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
RT3_NOINLINE int32_t __fastcall Company_Get7680(void* c);
RT3_NOINLINE int32_t __fastcall Obj_Level34b(void* o);
double __fastcall Company_QualityRaw(void* c);
RT3_NOINLINE float __fastcall Company_QualityFactor(void* c);
float __fastcall Obj_DifficultyBlend(void* o);
RT3_NOINLINE void __fastcall Obj_ClearGrids(void* o);
RT3_NOINLINE void __fastcall Obj_Clear1c4f(void* o);
RT3_NOINLINE float __fastcall Obj_Sum12a7(void* o);
RT3_NOINLINE float __fastcall Obj_Sum12a7Scaled(void* o);
RT3_NOINLINE int32_t __fastcall Company_NameIndex(void* c);
RT3_NOINLINE int32_t CountFreeCompanySlots();
RT3_NOINLINE int32_t __fastcall NthFreeCompanySlot(int32_t n);
RT3_NOINLINE float __fastcall Obj_ValueWithBonus(uint32_t* o);
RT3_NOINLINE int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag);
float __fastcall Company_GrowthScore(uint8_t* c);
RT3_NOINLINE float __fastcall Company_BookValue(uint8_t* c);
RT3_NOINLINE float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
RT3_NOINLINE float __fastcall Company_CreditRating(uint8_t* c, int, int32_t extraDebt);
RT3_NOINLINE float __fastcall Company_BondRate(uint8_t* c);
RT3_NOINLINE float __fastcall Company_CashRate(uint8_t* c);
RT3_NOINLINE float __fastcall Company_CashInterestRate(uint8_t* c);
RT3_NOINLINE void __fastcall Company_GoBankrupt(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Company_OwnerFlag1e1(void* c);
RT3_NOINLINE void __fastcall Company_YearEnd(uint8_t* c);
int32_t __fastcall Company_PlayerDebt(int32_t* o);
RT3_NOINLINE int32_t __fastcall Company_FreeShares(void* c);
RT3_NOINLINE int32_t __fastcall Company_HumanHoldings(int32_t* o);
RT3_NOINLINE float __fastcall Company_MaxDividend(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Company_TrainCount(void* c);
RT3_NOINLINE uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
RT3_NOINLINE uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly);
RT3_NOINLINE int32_t __fastcall Company_CountIdleBuildings(void* c);
RT3_NOINLINE uint32_t __fastcall Company_NthBuilding(int32_t* o, int, int32_t n);
RT3_NOINLINE void __fastcall Company_CacheBuildings(int32_t* o);
RT3_NOINLINE int32_t __fastcall Company_LiveIndex(void* c);
int32_t __fastcall Company_ClampDates(void* c, int, int32_t);
RT3_NOINLINE void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price);
RT3_NOINLINE void __fastcall Company_ReleaseBuilding(void* c, int, uint32_t building, float price);
RT3_NOINLINE void __fastcall Company_InvestInBuilding(void* c, int, uint32_t building, float amount);
RT3_NOINLINE uint32_t __fastcall Company_Logo(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Company_EntryOpen(void* c, int, int32_t i);
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add);
RT3_NOINLINE void __fastcall Company_Dissolve(uint8_t* c);
RT3_NOINLINE void* __fastcall Company_Owner(void* c);
void __fastcall Train_AutoName(uint8_t* t, int, uint32_t a, uint32_t b);
RT3_NOINLINE void* __fastcall Company_HeadquartersSite(void* c);
RT3_NOINLINE int32_t __fastcall Company_HasAssets(void* c);
RT3_NOINLINE void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
RT3_NOINLINE void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
RT3_NOINLINE float __fastcall Company_BondRateWithDebt(uint8_t* c, int, int32_t extraDebt, int32_t);
int32_t __fastcall Company_CanIssueBond(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Company_MonthEnd(uint8_t* c);
RT3_NOINLINE void __fastcall Company_SetChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
RT3_NOINLINE void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash);
RT3_NOINLINE void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __fastcall Company_InstallChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __fastcall Company_AnnualReview(uint8_t* c);
int32_t __fastcall Company_SpareShares(int32_t* c, int, int32_t flag);
RT3_NOINLINE Collection* __fastcall Companies_Construct(Collection* c);
RT3_NOINLINE void __fastcall Companies_Destruct(Collection* c);
RT3_NOINLINE uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Companies_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Companies_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Companies_BuildStationLinks(Collection* cos);
double __fastcall Company_DerivedStat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
RT3_NOINLINE void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
RT3_NOINLINE int32_t __fastcall Obj_ArgMax67(void* o);
double __fastcall Company_YearEstimate(uint8_t* c, int, int32_t id);
RT3_NOINLINE double __fastcall Company_TrackLength(void* c);
double __fastcall Company_LiveStat(uint8_t* c, int, int32_t id);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t Cmd_ToggleEngine114206();
int32_t Cmd_ToggleEngine114206Save();
int32_t Cmd_SetRenderer22b57();
void __fastcall PathToBaseName(char* dst, const char* src);
void __fastcall BaseNameNoSuffix(char* dst, const char* src);
RT3_NOINLINE int32_t __fastcall OpenFileDialog(char* file, const char* filter, const char* initialDir);
RT3_NOINLINE void Debug_DropModel();
RT3_NOINLINE void Debug_PlaceTestCar();
RT3_NOINLINE void Debug_PlaceTestModel();
void __fastcall Obj_FreeKeyList(void* o);
RT3_NOINLINE void __fastcall Site_Load(uint8_t* o, int, int32_t s, int16_t x, int16_t y);
void __fastcall Site_ClassifyWater(uint8_t* s);
RT3_NOINLINE void __fastcall Obj_AddLevel(void* o, int, int32_t i, float f);
void __fastcall Obj_AddKey(void* o, int, uint16_t key, int32_t value);
void __fastcall Obj_RemoveKey(void* o, int, uint16_t key);
RT3_NOINLINE int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d);
int32_t __fastcall Obj_Bit(void* o, int, int32_t i);
RT3_NOINLINE float __fastcall Obj_Level(void* o, int, int32_t i);
void __fastcall Station_Reset(void* o, int, uint16_t x, uint16_t y);
void __fastcall Region_SaveCargo(uint8_t* r, int, Stream st);
void __fastcall Chain_Push(void* o, int, uint16_t id);
RT3_NOINLINE void __fastcall Chain_Remove(void* o, int, uint16_t id);
void __fastcall Station_UpdateEdgeFlag(uint8_t* o);
RT3_NOINLINE float __fastcall Station_TotalAvailable(void* o);
RT3_NOINLINE float __fastcall City_CargoOutput(uint8_t* c, int, int32_t cargo);
void __fastcall City_TransferCargo(uint8_t* c, int, uint8_t* dst, int32_t i, int32_t amount);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __stdcall View_RefreshCompanyLists();
RT3_NOINLINE void __stdcall View_RefreshForCompany(int32_t id);
RT3_NOINLINE void __stdcall View_RefreshCompanyAssets(int32_t, int32_t company);
RT3_NOINLINE void __fastcall ModelObj_Release(uint8_t* o);
RT3_NOINLINE void __fastcall Vehicle_Teardown(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }

static inline uint8_t ConfigByte82() { return hook::Global<uint8_t*>(0x6cec7c)[0x82]; }

static inline double D(uint32_t va) { return *(const double*)va; }

// Owned (+0x276), flagged building whose virtual slot 28 is 0.
static inline int OwnedIdleBuilding(int32_t* o, uint8_t* b) {
    if (F<int32_t>(b, 0x276) != o[0]) return 0;
    if (Obj_Flag3d4(b) != 1) return 0;
    return ((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b) == 0;
}

static inline size_t StrLen(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }

static inline void* Virtual80(void* o) { return ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o); }

static inline int32_t Virtual14(void* o) { return ((int32_t(__thiscall*)(void*))(*(void***)o)[0x14 / 4])(o); }

// Free company slots: 0x24 fixed slots plus one per entry of the name list
// (*0x6d4020)+0x429b0; a company takes its slot +0x37, or else the list
// entry matching its name +0xd65.
static void FreeCompanySlots(uint8_t freeSlot[0x88]) {
    for (int i = 0; i < 0x88; ++i) freeSlot[i] = i < 0x24 ? 1 : 0;
    auto names = []() { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0); };
    for (int32_t i = 0; i < Collection_Count(names()); ++i) freeSlot[0x24 + i] = 1;
    for (int32_t k = 0; k < Collection_Count(g_companies); ++k) {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, Collection_IdAt(g_companies, 0, k, 0));
        int32_t slot = F<int32_t>(c, 0x37);
        if (slot != -1) {
            freeSlot[slot] = 0;
            continue;
        }
        for (int32_t j = 0; j < Collection_Count(names()); ++j) {
            Collection* l = names();
            if (Stricmp((const char*)c + 0xd65, (const char*)Collection_Get(l, 0, Collection_IdAt(l, 0, j, 0))) == 0)
                freeSlot[0x24 + j] = 0;
        }
    }
}

static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

static inline double D_205(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

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

// Company finance statistics. Ids below 0x2b are kept per year: +0x1c47 + id * 8
// (this year so far) and +0xd7f + (id * 11 + n) * 8 (n years ago, 0 = live); larger
// ids are sums / ratios of those. Years: 0x2329 = live value, 0x232a = this year so
// far. World 0x6cec78: year +0xd, month +0xf, day +0x11, tick +0x15, difficulty +0x2d.
static inline uint8_t* WorldG() { return hook::Global<uint8_t*>(0x6cec78); }

static inline int32_t CurYear() { return F<uint16_t>(WorldG(), 0xd); }

static inline int32_t FoundedYear(uint8_t* c) { return F<uint16_t>(c, 0x157); }

static inline int32_t Shares(uint8_t* c) { return F<int32_t>(c, 0x47); }

static inline double Pick(double a, bool useA, double b) { return useA ? a : b; }

static inline double D_207(uint32_t a) { return *(const double*)a; }

namespace {
template <typename Fn> void AbsorbBack(uint32_t global, Fn fn) {
    for (int32_t i = Collection_Count(hook::Global<Collection*>(global)) - 1; i >= 0; --i) {
        Collection* t = hook::Global<Collection*>(global);
        fn((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0)));
    }
}
}

int32_t __stdcall Return0_424000(int32_t) { return 0; }
RT3_REPLACE(0x424000, Return0_424000);

// 0x424010. Per-entry flag of a company (0x38d-byte entries from +0x1d9f).
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i) { return F<uint8_t>(c, 0x1d9f + i * 0x38d) != 0 ? 1 : 0; }
RT3_REPLACE(0x424010, Company_EntryFlag);

RT3_NOINLINE void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v) { F<uint8_t>(c, 0x1d9f + i * 0x38d) = v; }
RT3_REPLACE(0x424030, Company_SetEntryFlag);

// 0x424050. Sets (or adds to) slot `idx` of +0x35b unless `key` equals the
// last key +0xcf3; a non-zero key is remembered.
void __fastcall Obj_SetSlot35b(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xcf3)) return;
    if (add) F<int32_t>(o, 0x35b + idx * 4) += value;
    else F<int32_t>(o, 0x35b + idx * 4) = value;
    if (key) F<int32_t>(o, 0xcf3) = key;
}
RT3_REPLACE(0x424050, Obj_SetSlot35b);

// 0x4240a0. +0x7680, or 0x752f when the world flag +0x4aaf is off.
RT3_NOINLINE int32_t __fastcall Company_Get7680(void* c) {
    return F<int32_t>(g_world, 0x4aaf) ? F<int32_t>(c, 0x7680) : 0x752f;
}
RT3_REPLACE(0x4240a0, Company_Get7680);

// 0x4240c0. (+0x34b + 58) / 9, clamped to 0..12.
RT3_NOINLINE int32_t __fastcall Obj_Level34b(void* o) {
    int32_t v = (F<int32_t>(o, 0x34b) + 0x3a) / 9;
    if (v <= 0) v = 0;
    return v < 12 ? v : 12;
}
RT3_REPLACE(0x4240c0, Obj_Level34b);

double __fastcall Company_QualityRaw(void* c) { return F<float>(c, 0xd31); }
RT3_REPLACE(0x4240f0, Company_QualityRaw);

// 0x424100. Difficulty-scaled quality factor: lo*(1-q) + hi*q with q = +0xd31
// and lo/hi from the tables 0x5f3cd8/0x5f3ce8 by difficulty (config +0x7d).
RT3_NOINLINE float __fastcall Company_QualityFactor(void* c) {
    uint32_t d = hook::Global<uint8_t*>(0x6cec7c)[0x7d];
    double lo = ((const float*)0x5f3cd8)[d], hi = ((const float*)0x5f3ce8)[d];
    double q = F<float>(c, 0xd31);
    float r = (float)(lo * (*(const double*)0x5c8770 - q) + hi * q);
    return r;
}
RT3_REPLACE(0x424100, Company_QualityFactor);

// 0x424140. Blend of the difficulty tables 0x5f3cf8 / 0x5f3d08 by +0xd31.
float __fastcall Obj_DifficultyBlend(void* o) {
    uint32_t d = F<uint8_t>(g_newGame, 0x7d);
    const float* lo = (const float*)0x5f3cf8;
    const float* hi = (const float*)0x5f3d08;
    double x = F<float>(o, 0xd31);
    return (float)((D(0x5c8770) - x) * lo[d] + (double)hi[d] * x);
}
RT3_REPLACE(0x424140, Obj_DifficultyBlend);

// 0x424180. Clears the 11 x 12 grid of pairs at +0xdd7 (row stride 0x58)
// and the 12 pairs at +0x1c4f.
RT3_NOINLINE void __fastcall Obj_ClearGrids(void* o) {
    for (int32_t j = 0; j < 11; ++j) {
        for (int32_t i = 0; i < 12; ++i) {
            F<int32_t>(o, 0xdd7 + j * 8 + i * 0x58) = 0;
            F<int32_t>(o, 0xdd7 + j * 8 + i * 0x58 + 4) = 0;
            F<int32_t>(o, 0x1c4f + i * 8) = 0;
            F<int32_t>(o, 0x1c4f + i * 8 + 4) = 0;
        }
    }
}
RT3_REPLACE(0x424180, Obj_ClearGrids);

RT3_NOINLINE void __fastcall Obj_Clear1c4f(void* o) {
    for (int32_t i = 0; i < 0x18; ++i) F<int32_t>(o, 0x1c4f + i * 4) = 0;
}
RT3_REPLACE(0x4241c0, Obj_Clear1c4f);

// 0x4241e0 / 0x424200. Sum of the doubles +0x12a7 and +0x124f (as float), and that scaled.
RT3_NOINLINE float __fastcall Obj_Sum12a7(void* o) { return (float)(F<double>(o, 0x12a7) + F<double>(o, 0x124f)); }
RT3_REPLACE(0x4241e0, Obj_Sum12a7);

RT3_NOINLINE float __fastcall Obj_Sum12a7Scaled(void* o) {
    float s = (float)(F<double>(o, 0x12a7) + F<double>(o, 0x124f));
    return (float)((double)s * D(0x5c9b48));
}
RT3_REPLACE(0x424200, Obj_Sum12a7Scaled);

// 0x424230. Index of this name (+0xd65) in the table of object 0x6d4020
// (+0x429b0); the table count if absent.
RT3_NOINLINE int32_t __fastcall Company_NameIndex(void* c) {
    auto table = [] { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0); };
    int32_t i = 0;
    for (; i < Collection_Count(table()); ++i) {
        Collection* t = table();
        if (ExeStricmp((const char*)c + 0xd65, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) break;
    }
    return i;
}
RT3_REPLACE(0x424230, Company_NameIndex);

// 0x4242a0 / 0x424430. Number of free company slots / index of the n-th (0 if none).
RT3_NOINLINE int32_t CountFreeCompanySlots() {
    uint8_t freeSlot[0x88];
    FreeCompanySlots(freeSlot);
    int32_t n = 0;
    for (int i = 0; i < 0x88; ++i)
        if (freeSlot[i]) ++n;
    return n;
}
RT3_REPLACE(0x4242a0, CountFreeCompanySlots);

RT3_NOINLINE int32_t __fastcall NthFreeCompanySlot(int32_t n) {
    uint8_t freeSlot[0x88];
    FreeCompanySlots(freeSlot);
    for (int32_t i = 0; i < 0x88; ++i) {
        if (freeSlot[i]) --n;
        if (n == 0) return i;
    }
    return 0;
}
RT3_REPLACE(0x424430, NthFreeCompanySlot);

// 0x424580. World value (0x433740) plus the 0x39 cargo bonus for this company/player, scaled.
RT3_NOINLINE float __fastcall Obj_ValueWithBonus(uint32_t* o) {
    float base = (float)Obj_Rounded29(g_world);
    int32_t bonus = Obj_CargoBonus(g_world, 0, 0x39, o[0], F<uint32_t>(o, 0x3b), 0);
    return (float)((double)bonus * D(0x5c85b8) + base);
}
RT3_REPLACE(0x424580, Obj_ValueWithBonus);

// 0x4245e0. Index of the player with the n-th largest positive holding
// +0x15d[*o]; -1 if none. QUIRK: with `flag` set it always returns -1.
RT3_NOINLINE int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag) {
    if (!F<int32_t>(o, 0x3f)) return -1;
    int32_t count = Table_CountField4(g_players);
    int32_t used[16] = {};
    int32_t bestIdx = 0;
    if (--n < 0) return -1;
    for (;;) {
        int32_t best = flag ? -2000000 : 0;
        for (int32_t j = 0; j < count; ++j) {
            if (used[j]) continue;
            int32_t v = F<int32_t>(Table_NthField4(g_players, 0, j), 0x15d + o[0] * 4);
            if (v == 0) used[j] = 1;
            else if (v > best) { best = v; bestIdx = j; }
        }
        if (best <= 0 || flag) return -1;
        if (n == 0) return bestIdx;
        used[bestIdx] = 1;
        if (--n < 0) return -1;
    }
}
RT3_REPLACE(0x4245e0, Company_NthHolder);

// 0x4246b0. Earnings growth score: for the last 5 years, (profit change +
// dividends) / last year's profit, weighted 5..1 (this year by month / 12).
float __fastcall Company_GrowthScore(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c8598);
    double sumW = 0, total = 0;
    for (int32_t k = 0, n = 5; n > 0; ++k, --n) {
        int32_t y = CurYear() - k;
        if (y < FoundedYear(c)) continue;
        double prev = y - 1 < FoundedYear(c) ? (double)F<float>(c, 0x57) : Company_Stat(c, 0, 0x1c, y - 1);
        double change = Company_Stat(c, 0, 0x1c, CurYear() - k) - prev;
        double paid = Company_Stat(c, 0, 0x23, CurYear() - k);
        double growth;
        if (!(prev > D_205(0x5c8608)) && prev == prev) {
            growth = 1.0;
        } else {
            double d = D_205(0x5c88a0) > prev ? D_205(0x5c88a0) : prev;
            growth = (paid + change) / d;
        }
        double wt = n < 5 ? 1.0 : (double)F<uint8_t>(WorldG(), 0xf) / D_205(0x5c8978);
        double nw = (double)n * wt;
        sumW = nw + sumW;
        double d = D_205(0x5c88a0) > wt ? D_205(0x5c88a0) : wt;
        total = D_205(0x5c8770) / d * nw * growth + total;
    }
    double d = D_205(0x5c88a0) > sumW ? D_205(0x5c88a0) : sumW;
    return (float)(total / d);
}
RT3_REPLACE(0x4246b0, Company_GrowthScore);

// 0x4248d0. Book value per share (cached per tick in +0xcfb / +0xcff): profit,
// revenue and dividend histories of the last years are weighted into four estimates,
// clamped, mixed with weights (0x5ee208, scaled for young companies by 0x5ee218 +
// age * 16; the best estimate counts 1.25x, the worst 0.8x) and scaled by
// difficulty and age. Also stores the dividend-free mix in +0xd19 (at least 0.5).
RT3_NOINLINE float __fastcall Company_BookValue(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c8598);
    uint8_t* w = WorldG();
    if (F<int32_t>(c, 0xcfb) == F<int32_t>(w, 0x15)) return (float)F<double>(c, 0xcff);
    int32_t sumW1 = 0, sumW2 = 0;
    double profit = 0, revenue = 0, divs = 0, rising = 0;
    for (int32_t k = 0; k < 5; ++k) {
        int32_t y = CurYear() - k;
        if (y < FoundedYear(c)) continue;
        int32_t wt = k == 0 ? F<uint8_t>(WorldG(), 0xf) * 5 - 5 : (5 - k) * 12;
        double dw = wt;
        sumW1 += wt;
        profit = Company_Stat(c, 0, 0x1f, y) * dw + profit;
        revenue = Company_Stat(c, 0, 0x1e, CurYear() - k) * dw + revenue;
    }
    for (int32_t k = 1, n = 9; n > 1; ++k, --n) {
        int32_t y = CurYear() - k;
        if (y < FoundedYear(c)) continue;
        double dn = n;
        sumW2 += 10 - k;
        divs = Company_Stat(c, 0, 0x20, y) * dn + divs;
        int32_t y2 = CurYear() - k;
        if (y2 - 1 < FoundedYear(c)) continue;
        double a = Company_Stat(c, 0, 0x20, y2);
        double b = Company_Stat(c, 0, 0x20, y2 - 1);
        if (b <= a) rising += dn;
    }
    double est[4] = { 0, 0, 0, 0 };
    if (sumW1) {
        double d = sumW1;
        est[0] = profit / d * D_205(0x5c85f0);
        est[1] = revenue / d;
    }
    if (sumW2) {
        double d = sumW2;
        float avg = (float)(divs / d);
        float last = (float)Company_Stat(c, 0, 0x20, CurYear() - 1);
        last = (float)(((double)last + last + avg) / Fl(0x5c8900));
        if (!(avg < last)) avg = last;
        est[2] = (rising / d * D_205(0x5c8628) + D_205(0x5c8880)) * avg * D_205(0x5c8588);
    }
    est[3] = Company_Stat(c, 0, 0x1d, 0x2329);
    // The dividend estimate is capped at 1.13x the best of the others (each step
    // rounded through a float).
    double m = Pick(est[0], !(D_205(0x5c8770) > est[0]), D_205(0x5c8770));
    double mf = (float)m;
    m = Pick(est[1], !(mf > est[1]), mf);
    mf = (float)m;
    m = Pick(est[3], !(mf > est[3]), mf);
    double cap = (double)(float)m * D_205(0x5c9b58);
    if (!(est[2] < cap)) est[2] = cap;
    for (double& e : est)
        if (e < D_205(0x5c8608)) e = 0;
    double wts[4];
    for (int k = 0; k < 4; ++k) wts[k] = hook::Global<int32_t>(0x5ee208 + k * 4);
    int32_t age = CurYear() - FoundedYear(c);
    if (age < 5)
        for (int k = 0; k < 4; ++k) wts[k] *= (double)hook::Global<int32_t>(0x5ee218 + age * 16 + k * 4);
    int best = 0, worst = 0;
    if (est[1] > est[0]) best = 1;
    if (est[1] < est[0]) worst = 1;
    if (est[2] > est[best]) best = 2;
    if (est[2] < est[worst]) worst = 2;
    if (est[3] > est[best]) best = 3;
    if (est[3] < est[worst]) worst = 3;
    wts[best] *= D_205(0x5c87e8);
    wts[worst] *= D_205(0x5c8640);
    double t0 = wts[0] * est[0];
    double all = t0, allW = wts[0], noDiv = t0, noDivW = wts[0];
    double t1 = wts[1] * est[1];
    all = t1 + all;
    allW = wts[1] + allW;
    noDiv = t1 + noDiv;
    noDivW = wts[1] + noDivW;
    all = wts[2] * est[2] + all;
    allW = wts[2] + allW;
    double t3 = wts[3] * est[3];
    all = t3 + all;
    allW = wts[3] + allW;
    noDiv = t3 + noDiv;
    noDivW = wts[3] + noDivW;
    float mix = (float)(noDiv / noDivW);
    F<float>(c, 0xd19) = mix;
    float diff = Fl(0x5f33b8 + F<int32_t>(WorldG(), 0x2d) * 4);
    mix = (float)(((double)diff + D_205(0x5c8770)) / D_205(0x5c85d0) * mix);
    F<float>(c, 0xd19) = mix;
    if (mix < D_205(0x5c8628)) F<uint32_t>(c, 0xd19) = 0x3f000000;
    double v = all / allW;
    v = (double)Fl(0x5f33b8 + F<int32_t>(WorldG(), 0x2d) * 4) * v;
    int32_t yrs = CurYear() - F<int32_t>(c, 0x163);
    if (yrs < 10) v = ((double)yrs * D_205(0x5c9b50) + D_205(0x5c8750)) * v;
    if (v < D_205(0x5c8770)) v = 1.0;
    F<int32_t>(c, 0xcfb) = F<int32_t>(WorldG(), 0x15);
    F<double>(c, 0xcff) = v;
    return (float)v;
}
RT3_REPLACE(0x4248d0, Company_BookValue);

// 0x424fd0. Share price, optionally after issuing (+) / buying back (-) `issued`
// shares (`store`: keep the new market cap +0x4f and the price +0xd7b). Young
// companies blend the founding price +0x57 in for their first 50 * 40320 ticks; the
// cap/value ratio is squashed into 0.3..6 around 0.6-1.7 before scaling the book
// value; at least 0.0001.
RT3_NOINLINE float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c9b68);
    double book = Company_BookValue(c);
    uint8_t* w = WorldG();
    if (F<float>(c, 0x57) < book && CurYear() - FoundedYear(c) <= 5) {
        int32_t t = F<int32_t>(w, 0x15) - F<int32_t>(c, 0xd07);
        if (t <= 0) t = 0;
        int32_t q = t / 40320;
        if (q >= 0x32) q = 0x32;
        float a = (float)(0x32 - q);
        book = ((double)a * F<float>(c, 0x57) + (double)(q + 0x32) * book) / D_205(0x5c85d8);
    }
    int32_t shares = Shares(c);
    float ratio = (float)((double)(float)issued / (float)shares);
    if (ratio > D_205(0x5c8c20)) ratio = 0.2f;
    if (ratio < D_205(0x5c9b60)) ratio = -0.2f;
    double cap;
    if (issued) {
        cap = (D_205(0x5c8770) - ratio) * F<float>(c, 0x4f) + (double)issued;
        if (store) F<float>(c, 0x4f) = (float)cap;
    } else {
        cap = F<float>(c, 0x4f);
    }
    double sh = shares;
    cap = ((double(__cdecl*)(double, double))0x5a1b60)(sh / D_205(0x5c8bf8), 0.33) * cap * D_205(0x5c88f0);
    sh = Shares(c);
    double x = (sh + cap) / sh;
    if (x < D_205(0x5c8880)) x = (x - D_205(0x5c8880)) * D_205(0x5c8628) + D_205(0x5c8880);
    if (x < D_205(0x5c8c18)) x = (x - D_205(0x5c8c18)) * D_205(0x5c8628) + D_205(0x5c8c18);
    if (x < D_205(0x5c88c8)) {
        x = 0.3;
    } else {
        if (x > D_205(0x5c8820)) x = (x - D_205(0x5c8820)) * D_205(0x5c8628) + D_205(0x5c8820);
        if (x > D_205(0x5c87d8)) x = (x - D_205(0x5c87d8)) * D_205(0x5c8628) + D_205(0x5c87d8);
        if (x > D_205(0x5c87b8)) x = (x - D_205(0x5c87b8)) * D_205(0x5c8628) + D_205(0x5c87b8);
        if (x > D_205(0x5c8610)) x = 6.0;
    }
    double price = x * book;
    price = Effects_Multiplier(WorldG(), 0, 0x37, F<uint32_t>(c, 0), F<uint32_t>(c, 0x3b), 0) * price;
    price = (double)Ftol(price + D_205(0x5c8628));
    if (D_205(0x5c88a0) > price) price = D_205(0x5c88a0);
    if (store) F<float>(c, 0xd7b) = (float)price;
    return (float)price;
}
RT3_REPLACE(0x424fd0, Company_SharePrice);

// 0x425320. Credit rating 0..10 (with `extraDebt` more debt): interest coverage of
// the 4-year weighted operating profit (bond interest plus, for negative cash,
// overdraft interest), the debt / equity ratio, scaled by difficulty; cut for
// < $1M equity, young companies (capped at age + 5) and recently re-founded ones
// (+0x163), plus the rating bonus effect 0x38; rounded.
RT3_NOINLINE float __fastcall Company_CreditRating(uint8_t* c, int, int32_t extraDebt) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c8598);
    int32_t sumW = 0;
    double acc = 0;
    for (int32_t k = 1, n = 4; n > 0; ++k, --n) {
        sumW += 5 - k;
        int32_t y = CurYear() - k;
        if (y < FoundedYear(c)) continue;
        double a = Company_Stat(c, 0, 0x2b, y);
        double b = Company_Stat(c, 0, 9, CurYear() - k);
        acc = (a - b) * (double)n + acc;
    }
    double avgProfit = acc / (double)sumW;
    double extra = extraDebt;
    double debt = Company_Stat(c, 0, 0x12, 0x2329) + extra;
    double interest = Obj_WeightedAverage(c) * debt;
    if (Company_Stat(c, 0, 0xd, 0x2329) < D_205(0x5c8608)) {
        float rate = (float)Obj_Rounded29(WorldG());
        int32_t n = Obj_CargoBonus(WorldG(), 0, 0x39, F<uint32_t>(c, 0), F<uint32_t>(c, 0x3b), 0);
        rate = (float)((double)n * D_205(0x5c85b8) + rate);
        interest = Company_Stat(c, 0, 0xd, 0x2329) * ((double)rate + D_205(0x5c9b80)) + interest;
    }
    double cov = interest == D_205(0x5c8608) ? 10.0 : avgProfit / -interest;
    double s = 10.0;
    if (cov < D_205(0x5c8d68)) {
        if (cov > D_205(0x5c85d0)) s = D_205(0x5c85f0) - (D_205(0x5c8d68) - cov);
        else if (cov > D_205(0x5c8770)) s = D_205(0x5c85f0) - ((D_205(0x5c85d0) - cov) * 2 + D_205(0x5c85c8));
        else if (cov > D_205(0x5c8608)) s = D_205(0x5c85f0) - ((D_205(0x5c8770) - cov) * D_205(0x5c87b8) + D_205(0x5c8d68));
        else s = 0;
    }
    int32_t age = CurYear() - FoundedYear(c);
    if (age < 5) s = (double)(5 - age) * D_205(0x5c86b8) * (D_205(0x5c85f0) - s) + s;
    double r = Company_Stat(c, 0, 0x30, 0x2329);
    if (r > D_205(0x5c8608)) r = (-Company_Stat(c, 0, 0x12, 0x2329) + extra) / r;
    else r = 1.0;
    double s2 = 10.0;
    if (r > D_205(0x5c86b8)) {
        if (r < D_205(0x5c8758)) s2 = D_205(0x5c85f0) - r * D_205(0x5c87b8);
        else if (r < D_205(0x5c8628)) s2 = D_205(0x5c85f0) - ((r - D_205(0x5c8758)) * D_205(0x5c8970) + D_205(0x5c8770));
        else if (r < D_205(0x5c8750)) s2 = D_205(0x5c85f0) - ((r - D_205(0x5c8628)) * D_205(0x5c8978) + D_205(0x5c85c8));
        else if (r < D_205(0x5c8770)) s2 = D_205(0x5c85f0) - ((r - D_205(0x5c8750)) * D_205(0x5c9b78) + D_205(0x5c8610));
        else s2 = 0;
    }
    double both = s2 * s / D_205(0x5c85f0);
    s = ((s2 + s) / D_205(0x5c85d0) + both * 2) / D_205(0x5c85c8);
    s = (double)Fl(0x5f33b8 + F<int32_t>(WorldG(), 0x2d) * 4) * s;
    float equity = (float)Company_Stat(c, 0, 0x31, 0x2329);
    if (equity < Fl(0x5c8d10)) s = s - (D_205(0x5c85f0) - equity / Fl(0x5c8d00));
    if (age < 1) age = 1;
    if (age < 5) {
        s = s - (double)(5 - age) * D_205(0x5c8628);
        double lim = age + 5;
        if (!(s < lim)) s = lim;
    }
    int32_t since = CurYear() - F<int32_t>(c, 0x163);
    if (since < 0xf) s = (double)since * s * D_205(0x5c9b70);
    s = (double)Obj_CargoBonus(WorldG(), 0, 0x38, F<uint32_t>(c, 0), F<uint32_t>(c, 0x3b), 0) + s;
    s = (double)Ftol(s + D_205(0x5c8628));
    if (s < D_205(0x5c8608)) s = 0;
    else if (s > D_205(0x5c85f0)) s = 10.0;
    return (float)s;
}
RT3_REPLACE(0x425320, Company_CreditRating);

// 0x425880. Interest rate on new bonds: the base rate (0x424580) plus 1% per
// credit-rating point below 10, plus 2%.
RT3_NOINLINE float __fastcall Company_BondRate(uint8_t* c) {
    double risk = (D_205(0x5c85f0) - Company_Stat(c, 0, 0x22, 0x2329)) / D_205(0x5c85d8);
    return (float)(Obj_ValueWithBonus((uint32_t*)c) + risk + D_205(0x5c8918));
}
RT3_REPLACE(0x425880, Company_BondRate);

// 0x4258c0. Rate paid on (or for) cash: the base rate minus 4%, capped by the cash
// pile (> $500M: 1%, > $20M: 2%, > $5M: 3%, > $2M: 4%) and at least 1%.
RT3_NOINLINE float __fastcall Company_CashRate(uint8_t* c) {
    float r = (float)(Obj_ValueWithBonus((uint32_t*)c) - D_205(0x5c9b98));
    float cash = (float)Company_Stat(c, 0, 0xd, 0x2329);
    if (cash > Fl(0x5c9b94)) r = (float)(r < D_205(0x5c85b8) ? (double)r : 0.01);
    else if (cash > Fl(0x5c9b90)) r = (float)(r < D_205(0x5c8918) ? (double)r : 0.02);
    else if (cash > Fl(0x5c9b8c)) r = (float)(r < D_205(0x5c9b80) ? (double)r : 0.03);
    else if (cash > Fl(0x5c9b88)) r = (float)(r < D_205(0x5c9b98) ? (double)r : 0.04);
    if (r > D_205(0x5c85b8)) return r;
    return 0.01f;
}
RT3_REPLACE(0x4258c0, Company_CashRate);

// 0x425a50. Interest rate on the company's cash: the bond rate while overdrawn,
// else the cash rate (0 for an inactive company).
RT3_NOINLINE float __fastcall Company_CashInterestRate(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c8598);
    if (Company_Stat(c, 0, 0xd, 0x2329) < D_205(0x5c8608)) return Company_BondRate(c);
    return Company_CashRate(c);
}
RT3_REPLACE(0x425a50, Company_CashInterestRate);

// 0x425a90. Bankruptcy: every bond is halved (half the face value comes off the
// debt stat 0x12), positive cash is lost entirely and a deficit halved, the share
// count grows by half (rounded down to thousands), the price is recomputed and the
// year noted in +0x163.
RT3_NOINLINE void __fastcall Company_GoBankrupt(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return;
    int32_t total = 0;
    for (int32_t i = 0; i < F<int32_t>(c, 0x5b); ++i) {
        int32_t& face = F<int32_t>(c, 0x5f + i * 0xc);
        total += face;
        face /= 2;
    }
    Company_AddStat(c, 0, 0x12, (float)(total / 2));
    float cash;
    if (Company_Stat(c, 0, 0xd, 0x2329) > D_205(0x5c8608)) cash = (float)-Company_Stat(c, 0, 0xd, 0x2329);
    else cash = (float)-(Company_Stat(c, 0, 0xd, 0x2329) / D_205(0x5c85d0));
    Company_AddStat(c, 0, 0xd, cash);
    F<int32_t>(c, 0x47) = Ftol((double)Shares(c) * D_205(0x5c8578) / D_205(0x5c87f8)) * 1000;
    Company_SharePrice(c, 0, 0, 0);
    F<int32_t>(c, 0x163) = CurYear();
}
RT3_REPLACE(0x425a90, Company_GoBankrupt);

// 0x425b90. Company: owning player (+0x3b) has the +0x1e1 flag set.
RT3_NOINLINE int32_t __fastcall Company_OwnerFlag1e1(void* c) {
    int32_t id = F<int32_t>(c, 0x3b);
    if (id <= 0) return 0;
    return F<int32_t>(Collection_Get(g_players, 0, id), 0x1e1) != 0 ? 1 : 0;
}
RT3_REPLACE(0x425b90, Company_OwnerFlag1e1);

// 0x425bc0. Company year end (active companies, +0x3f): the 43 yearly stat
// rows (+0xd7f, 11 doubles each) move one year back and rows 1..12 start the
// new year at 0; the per-year accumulators fold into their totals (the 25
// 0x38d-byte blocks from +0x1da0, the 53 counters +0x24f into +0x17b, +0xd5d
// into +0xd61) and the yearly counters reset. Unless paused (world +0x46c38),
// the book value +0x153 becomes the average of revenue + 1.6 x profit over
// the last 3 years, scaled by a damped square root of its hundred-thousands
// (at most 40) / 100, at least 1000; bonds maturing this year are repaid; and
// with a share price over 100 the stock splits by price / 50 (holdings, share
// count, per-share figures and six history rows; announced with string 0x131,
// as news 0x132 to the only human player who owns it).
RT3_NOINLINE void __fastcall Company_YearEnd(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return;
    int32_t year = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    for (int32_t s = 10; s >= 1; --s)
        for (int32_t r = 0; r < 0x2b; ++r) {
            uint8_t* e = c + 0xd7f + r * 0x58 + s * 8;
            ((uint32_t*)e)[0] = ((uint32_t*)e)[-2];
            ((uint32_t*)e)[1] = ((uint32_t*)e)[-1];
        }
    for (int32_t r = 1; r <= 12; ++r) {
        F<uint32_t>(c, 0xd7f + r * 0x58) = 0;
        F<uint32_t>(c, 0xd7f + r * 0x58 + 4) = 0;
    }
    F<uint32_t>(c, 0x53) = 0;
    for (int32_t b = 0; b < 0x19; ++b) {
        uint8_t* e = c + 0x1da0 + b * 0x38d;
        X87AddMem(e + 4, e + 4, e);
        F<uint32_t>(e, 0) = 0;
        for (uint32_t o = 8; o < 0x20; o += 8) {
            X87AddMem(e + o + 4, e + o, e + o + 4);
            F<uint32_t>(e, o) = 0;
        }
        for (int32_t j = 0; j < 0x35; ++j) {
            X87AddMem(e + 0x110 + j * 4, e + 0x3c + j * 4, e + 0x110 + j * 4);
            F<uint32_t>(e, 0x3c + j * 4) = 0;
            X87AddMem(e + 0x2b8 + j * 4, e + 0x1e4 + j * 4, e + 0x2b8 + j * 4);
            F<uint32_t>(e, 0x1e4 + j * 4) = 0;
        }
    }
    for (uint32_t o : { 0xd0bu, 0xd0fu, 0xd13u, 0x323u, 0x327u, 0x32bu }) F<uint32_t>(c, o) = 0;
    for (int32_t j = 0; j < 0x35; ++j) {
        X87AddMem(c + 0x17b + j * 4, c + 0x17b + j * 4, c + 0x24f + j * 4);
        F<uint32_t>(c, 0x24f + j * 4) = 0;
    }
    {
        for (uint32_t o : { 0x32fu, 0x333u, 0x337u, 0x33bu }) F<uint32_t>(c, o) = 0;
        X87AddMem(c + 0xd61, c + 0xd5d, c + 0xd61);
        F<uint32_t>(c, 0x33f) = 0;
        F<uint32_t>(c, 0x343) = 0;
        F<uint32_t>(c, 0xd5d) = 0;
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38)) return;
    F<int32_t>(c, 0x153) = 0;
    int32_t n = 0;
    int32_t y = year - 1;
    for (int k = 3; k; --k, --y) {
        if (F<uint16_t>(c, 0x157) > y) continue;
        F<int32_t>(c, 0x153) = Ftol(Company_Stat(c, 0, 0x2c, y) + (double)F<int32_t>(c, 0x153));
        F<int32_t>(c, 0x153) = Ftol(Company_Stat(c, 0, 0x2b, y) * D_205(0x5c8570) + (double)F<int32_t>(c, 0x153));
        ++n;
    }
    if (n < 1) n = 1;
    int32_t avg = F<int32_t>(c, 0x153) / n;
    F<int32_t>(c, 0x153) = avg;
    double h = (double)(avg / 100000);
    double r = h > D_205(0x5c8770) ? h : D_205(0x5c8770);
    float f = (float)r;
    f = (float)((double(__cdecl*)(double))0x5a1634)(f);
    if (f > Fl(0x5c85f8)) f = (float)(((double)f - Fl(0x5c85f8)) / Fl(0x5c878c) + Fl(0x5c85f8));
    if (f > Fl(0x5c873c)) f = (float)(((double)f - Fl(0x5c873c)) / Fl(0x5c878c) + Fl(0x5c873c));
    if (f > Fl(0x5c8784)) f = (float)(((double)f - Fl(0x5c8784)) / Fl(0x5c878c) + Fl(0x5c8784));
    if (Fl(0x5c8d4c) < f) f = 40.0f;
    F<int32_t>(c, 0x153) = Ftol((double)f * (double)F<int32_t>(c, 0x153) * D_205(0x5c85b8));
    if (F<int32_t>(c, 0x153) < 1000) F<int32_t>(c, 0x153) = 1000;
    for (int32_t k = F<int32_t>(c, 0x5b) - 1; k >= 0; --k)
        if (F<int32_t>(c, 0x63 + k * 12) == year) Company_RemoveEntry(c, 0, k, 0);
    if (!(Company_SharePrice(c, 0, 0, 1) > Fl(0x5c8c28))) return;
    int32_t factor = Ftol((double)F<float>(c, 0xd7b) / Fl(0x5c8780));
    for (int32_t i = 0; i < Table_CountField4(hook::Global<Collection*>(0x6ceb9c)); ++i) {
        uint8_t* p = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i);
        F<int32_t>(p, 0x15d + F<int32_t>(c, 0) * 4) *= factor;
    }
    float ff = (float)factor;
    F<int32_t>(c, 0x47) *= factor;
    F<float>(c, 0x4b) = (float)((double)F<float>(c, 0x4b) / ff);
    F<float>(c, 0x4f) = (float)((double)ff * F<float>(c, 0x4f));
    F<float>(c, 0x53) = (float)((double)F<float>(c, 0x53) / ff);
    F<float>(c, 0x57) = (float)((double)F<float>(c, 0x57) / ff);
    double df = (double)factor;
    for (int32_t i = 0; i < 12; ++i)
        for (uint32_t base : { 0x171fu, 0x1777u, 0x17cfu, 0x1827u, 0x187fu, 0x1987u }) F<double>(c, base + i * 8) = F<double>(c, base + i * 8) / df;
    F<uint32_t>(c, 0xcfb) = 0;
    Company_SharePrice(c, 0, 0, 1);
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82] || F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) return;
    char num[0x100], text[0x200];
    const char* ns = Num_Format(num, 1, df, 10, 1, 0, 0);
    FormatText(text, GetString(0x131), (const char*)(c + 4), ns, 0, 0, 0, 0, 0, 0);
    if (F<int32_t>(c, 0) == Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) && CountHumanPlayers() <= 1)
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, text, GetString(0x132), 0, 0);
    else
        Log_Add(hook::Global<Collection*>(0x6acd34), 0, text, 0x12, F<int32_t>(c, 0), 0);
}
RT3_REPLACE(0x425bc0, Company_YearEnd);

// 0x426150. Minus the sum of the negative player entries +0x15d[*o].
RT3_NOINLINE int32_t __fastcall Company_PlayerDebt(int32_t* o) {
    int32_t n = Table_CountField4(g_players);
    int32_t sum = 0;
    for (int32_t i = 0; i < n; ++i) {
        int32_t v = F<int32_t>(Table_NthField4(g_players, 0, i), 0x15d + o[0] * 4);
        sum += v < 0 ? v : 0;
    }
    return -sum;
}
RT3_REPLACE(0x426150, Company_PlayerDebt);

// 0x4261b0. Shares not held by any active player: outstanding (+0x47) minus
// each player's holding of this company (player +0x15d + company*4, if > 0).
RT3_NOINLINE int32_t __fastcall Company_FreeShares(void* c) {
    int32_t players = Table_CountField4(g_players);
    int32_t held = 0;
    for (int32_t i = 0; i < players; ++i) {
        int32_t h = F<int32_t>(Table_NthField4(g_players, 0, i), 0x15d + F<int32_t>(c, 0) * 4);
        if (h > 0) held += h;
    }
    return F<int32_t>(c, 0x47) - held;
}
RT3_REPLACE(0x4261b0, Company_FreeShares);

// 0x426200. Sum of the positive entries +0x15d[*o] of active human players.
RT3_NOINLINE int32_t __fastcall Company_HumanHoldings(int32_t* o) {
    int32_t n = Table_CountField4(g_players);
    int32_t sum = 0;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, i);
        if (!F<int32_t>(p, 4)) continue;
        int32_t v = F<int32_t>(p, 0x15d + o[0] * 4);
        if (v > 0 && F<int32_t>(p, 0x1e1)) sum += v;
    }
    return sum;
}
RT3_REPLACE(0x426200, Company_HumanHoldings);

// 0x426260. Highest dividend per share the board allows: half the best yearly
// profit or revenue of the last (up to 3) full years (young companies: at least
// 5% of their founding value), 1.25x per share; never more than the cash per share
// or 35% of last year's share price (the founding price in the first year).
RT3_NOINLINE float __fastcall Company_MaxDividend(uint8_t* c) {
    float cashPS = (float)(Company_Stat(c, 0, 0xd, 0x2329) / (double)(Shares(c) + 1));
    int32_t age = CurYear() - FoundedYear(c);
    if (age >= 3) age = 3;
    double profit = 0, revenue = 0;
    int32_t k = WorldG()[0xf] == 0xc ? 0 : 1;
    if (k < age + 1) {
        for (; k < age + 1; ++k) {
            if (!(Company_Stat(c, 0, 0x2b, CurYear() - k) < profit)) profit = Company_Stat(c, 0, 0x2b, CurYear() - k);
            if (!(Company_Stat(c, 0, 0x2c, CurYear() - k) < revenue)) revenue = Company_Stat(c, 0, 0x2c, CurYear() - k);
        }
        if (!(revenue > profit)) revenue = profit;
    } else {
        revenue = profit;
    }
    revenue = revenue / D_205(0x5c85d0);
    if (!(profit < revenue)) profit = revenue;
    if (age <= 1) {
        double founding = (double)(float)Shares(c) * F<float>(c, 0x57) * D_205(0x5c9ba0);
        if (!(profit > founding)) profit = founding;
    }
    profit = profit / (double)(Shares(c) + 1) * D_205(0x5c87e8);
    float d = (float)(cashPS < profit ? (double)cashPS : profit);
    float base = age == 0 ? F<float>(c, 0x57) : (float)Company_Stat(c, 0, 0x1c, CurYear() - 1);
    double cap = (double)base * D_205(0x5c8618);
    d = (float)(d < cap ? (double)d : cap);
    if (Fl(0x5c8598) > d) d = 0.0f;
    return d;
}
RT3_REPLACE(0x426260, Company_MaxDividend);

// 0x4264c0. Number of trains owned by this company (train +0x51 == company id *this).
RT3_NOINLINE int32_t __fastcall Company_TrainCount(void* c) {
    int32_t total = Collection_Count(g_trains), n = 0;
    for (int32_t i = 0; i < total; ++i)
        if (F<int32_t>(Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 0)), 0x51) == F<int32_t>(c, 0)) ++n;
    return n;
}
RT3_REPLACE(0x4264c0, Company_TrainCount);

// 0x426520. ID of the n-th (1-based) train owned by this company; 0 if none.
RT3_NOINLINE uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n) {
    int32_t count = Collection_Count(g_trains);
    for (int32_t i = 0; i < count; ++i) {
        if (F<int32_t>(Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 0)), 0x51) != o[0]) continue;
        if (--n == 0) return Collection_IdAt(g_trains, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426520, Company_NthTrain);

// 0x426590. Cargo objects (0x6cec20) on buildings owned by this company with
// flag +0x3d4 == 1; with `activeOnly`, only those whose building is active.
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly) {
    int32_t total = Collection_Count(g_table6cec20), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        void* o = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(o) != F<int32_t>(c, 0)) continue;
        if (Obj_Flag3d4(Obj_Building(o)) != 1) continue;
        if (activeOnly && !Obj_BuildingStateActive(o)) continue;
        ++n;
    }
    return n;
}
RT3_REPLACE(0x426590, Company_CountCargoSites);

// 0x426620. ID of the n-th 0x6cec20 record of this company whose building
// is flagged (and, if `activeOnly`, active); 0 if none.
RT3_NOINLINE uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly) {
    int32_t count = Collection_Count(g_table6cec20);
    for (int32_t i = 0; i < count; ++i) {
        void* r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != o[0]) continue;
        if (Obj_Flag3d4(Obj_Building(r)) != 1) continue;
        if (activeOnly && !Obj_BuildingStateActive(r)) continue;
        if (--n == 0) return Collection_IdAt(g_table6cec20, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426620, Company_NthSite);

// 0x4266c0. Buildings this company owns (+0x276) with flag +0x3d4 == 1 and
// virtual slot 28 returning 0.
RT3_NOINLINE int32_t __fastcall Company_CountIdleBuildings(void* c) {
    int32_t total = Collection_Count(g_buildings), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0));
        if (F<int32_t>(b, 0x276) != F<int32_t>(c, 0)) continue;
        if (Obj_Flag3d4(b) != 1) continue;
        if (((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x4266c0, Company_CountIdleBuildings);

// 0x426740. ID of the n-th such building; served from the cache 0x62baf0
// (filled by 0x4267f0) while 0x62bae8 is set and n is 1..400.
RT3_NOINLINE uint32_t __fastcall Company_NthBuilding(int32_t* o, int, int32_t n) {
    if (n - 1 < 400 && hook::Global<uint8_t>(0x62bae8)) return ((const uint16_t*)0x62baee)[n];
    int32_t count = Collection_Count(g_buildings);
    for (int32_t i = 0; i < count; ++i) {
        if (!OwnedIdleBuilding(o, (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0)))) continue;
        if (--n == 0) return Collection_IdAt(g_buildings, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x426740, Company_NthBuilding);

// 0x4267f0. Fills the cache 0x62baf0..0x62be10 (u16 IDs) used by 0x426740.
RT3_NOINLINE void __fastcall Company_CacheBuildings(int32_t* o) {
    int32_t count = Collection_Count(g_buildings);
    Collection_PositionOf(g_companies, 0, o[0], 0); // result unused in the original
    uint16_t* p = (uint16_t*)0x62baf0;
    for (int32_t i = 0; i < count; ++i) {
        if (!OwnedIdleBuilding(o, (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0)))) continue;
        if (p < (uint16_t*)0x62be10) *p = (uint16_t)Collection_IdAt(g_buildings, 0, i, 0);
        ++p;
    }
}
RT3_REPLACE(0x4267f0, Company_CacheBuildings);

// 0x426890. Position of this company among the live companies, or -1.
RT3_NOINLINE int32_t __fastcall Company_LiveIndex(void* c) {
    int32_t n = Table_CountField3f(g_companies);
    for (int32_t i = 0; i < n; ++i)
        if (F<int32_t>(Table_NthField3f(g_companies, 0, i), 0) == F<int32_t>(c, 0)) return i;
    return -1;
}
RT3_REPLACE(0x426890, Company_LiveIndex);

// 0x4268e0. Clamps +0x157 to [world +5, max(world +5, year)] and +0xd07 to
// [start-of-year minutes, max(that, now)]; clears +0xd3a / +0xd3e.
RT3_NOINLINE int32_t __fastcall Company_ClampDates(void* c, int, int32_t) {
    F<int32_t>(c, 0xd3a) = 0;
    F<int32_t>(c, 0xd3e) = 0;
    uint16_t lo = F<uint16_t>(g_world, 5);
    uint16_t v = F<uint16_t>(c, 0x157);
    if (v < lo) v = lo;
    F<uint16_t>(c, 0x157) = v;
    uint16_t a = F<uint16_t>(g_world, 5), y = F<uint16_t>(g_world, 0xd);
    uint16_t hi = a > y ? a : y;
    F<uint16_t>(c, 0x157) = v < hi ? v : hi;
    int32_t m = MinutesFromParts(F<uint16_t>(g_world, 5), 1, 1, 0, 0);
    int32_t t = F<int32_t>(c, 0xd07);
    if (t <= m) t = m;
    F<int32_t>(c, 0xd07) = t;
    int32_t now = F<int32_t>(g_world, 0x15);
    int32_t top = m > now ? m : now;
    F<int32_t>(c, 0xd07) = t < top ? t : top;
    return 0;
}
RT3_REPLACE(0x4268e0, Company_ClampDates);

// 0x4269b0. Gives a building to this company (+0x276 = company id); outside
// scenario-editor mode also records the price (+0x27a) and books it (stats 17, 13).
RT3_NOINLINE void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price) {
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, building);
    if (!b) return;
    F<int32_t>(b, 0x276) = F<int32_t>(c, 0);
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    F<float>(b, 0x27a) = price;
    Company_AddStat(c, 0, 0x11, price);
    Company_AddStat(c, 0, 0xd, -price);
}
RT3_REPLACE(0x4269b0, Company_TakeBuilding);

// 0x426a10. Releases a building from this company, booking the sale.
RT3_NOINLINE void __fastcall Company_ReleaseBuilding(void* c, int, uint32_t building, float price) {
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, building);
    if (!b) return;
    F<int32_t>(b, 0x276) = 0;
    if (ConfigByte82()) return;
    float paid = F<float>(b, 0x27a);
    Company_AddStat(c, 0, 0x11, -paid);
    Company_AddStat(c, 0, 0xd, price);
    float diff = (float)((double)paid - price);
    Company_PostStat(c, 0, 3, -diff, 0);
    F<int32_t>(b, 0x27a) = 0;
}
RT3_REPLACE(0x426a10, Company_ReleaseBuilding);

// 0x426aa0. Adds `amount` to a building's value (+0x27a) and books it.
RT3_NOINLINE void __fastcall Company_InvestInBuilding(void* c, int, uint32_t building, float amount) {
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, building);
    if (!b) return;
    ((void(__thiscall*)(void*))(*(void***)b)[0x74 / 4])(b);
    if (ConfigByte82()) return;
    F<float>(b, 0x27a) = (float)((double)amount + F<float>(b, 0x27a));
    Company_AddStat(c, 0, 0x11, amount);
    Company_AddStat(c, 0, 0xd, -amount);
}
RT3_REPLACE(0x426aa0, Company_InvestInBuilding);

// 0x426b10. The company's logo entry: its stored index (+0x37), the logo whose
// name matches +0xd65, or a random free slot (stored).
RT3_NOINLINE uint32_t __fastcall Company_Logo(uint8_t* c) {
    if (F<int32_t>(c, 0x37) > -1) return Table_Entry(F<uint8_t*>(World(), 0x46a80), 0, F<int32_t>(c, 0x37));
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0)); ++i) {
        Collection* names = F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0);
        const char* s = (const char*)Collection_Get(names, 0, Collection_IdAt(names, 0, i, 0));
        if (!Stricmp((const char*)c + 0xd65, s)) return Table_Entry(F<uint8_t*>(World(), 0x46aa4 + i * 4), 0, 0);
    }
    int32_t r = Rand(0, CountFreeCompanySlots() - 1);
    F<int32_t>(c, 0x37) = r;
    F<int32_t>(c, 0x37) = NthFreeCompanySlot(r);
    return Table_Entry(F<uint8_t*>(World(), 0x46a80), 0, F<int32_t>(c, 0x37));
}
RT3_REPLACE(0x426b10, Company_Logo);

// 0x426be0. Company entry i unflagged and region type i has +0x2d == 1.
RT3_NOINLINE int32_t __fastcall Company_EntryOpen(void* c, int, int32_t i) {
    if (F<uint8_t>(c, 0x1d9f + i * 0x38d)) return 0;
    return F<int32_t>(Collection_Get(g_table6cfc9c, 0, i), 0x2d) == 1 ? 1 : 0;
}
RT3_REPLACE(0x426be0, Company_EntryOpen);

// 0x426c20. Adds (or removes) a track piece to the company's counters by
// kind +0x218 (+0x7664/+0x766c/+0x7668) and class +0x226 (+0x7674/+0x7678,
// or +0x767c if virtual slot 5), and to the same counters of its region's
// per-region block (+0x1dc0.., stride 0x38d).
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    auto count = [&](uint8_t* b, uint32_t k1, uint32_t k2, uint32_t k0, uint32_t c3, uint32_t c1, uint32_t cv) {
        uint8_t kind = t[0x218];
        if (kind == 1) F<int32_t>(b, k1) += d;
        else if (kind == 2) F<int32_t>(b, k2) += d;
        else F<int32_t>(b, k0) += d;
        int32_t cls = F<int32_t>(t, 0x226);
        if (cls == 3) F<int32_t>(b, c3) += d;
        else if (cls == 1) F<int32_t>(b, c1) += d;
        else if (Virtual14(t)) F<int32_t>(b, cv) += d;
    };
    count(c, 0x7664, 0x766c, 0x7668, 0x7674, 0x7678, 0x767c);
    if (!hook::Global<void*>(0x6cfc9c)) return;
    int32_t y = (int32_t)Obj_CenterCellY(t);
    int32_t x = (int32_t)Obj_CenterCellX(t);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    uint32_t region = F<uint8_t*>(map, 0x2131)[F<int32_t>(map, 0x2155) * y + x] >> 3;
    count(c + region * 0x38d, 0x1dc0, 0x1dc8, 0x1dc4, 0x1dd0, 0x1dd4, 0x1dd8);
}
RT3_REPLACE(0x426c20, Company_CountTrack);

// 0x426d60. Dissolves company c once its chairman (+0x3f) is gone: clears
// every player's holdings slot for it and its player link (+0x3b), the
// player's company when it was this one, removes its trains (0x6cfcbc,
// +0x51) and frees its idle stations (+0x276), renames it with string 0x133
// (50 characters) and refreshes the company views.
RT3_NOINLINE void __fastcall Company_Dissolve(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return;
    for (int32_t i = 0; i < Table_CountField4(hook::Global<Collection*>(0x6ceb9c)); ++i)
        F<int32_t>(Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i), 0x15d + F<int32_t>(c, 0) * 4) = 0;
    if (uint32_t p = F<uint32_t>(c, 0x3b)) F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, p), 0x1dd) = 0;
    if (Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) == F<int32_t>(c, 0)) Obj_Set21(hook::Global<uint8_t*>(0x6cec78), 0, 0);
    F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62be10), 0, F<uint32_t>(c, 0)), 0x3f) = 0;
    F<int32_t>(c, 0x3f) = 0;
    for (int32_t i = Collection_Count(hook::Global<Collection*>(0x6cfcbc)) - 1; i >= 0; --i) {
        Collection* trains = hook::Global<Collection*>(0x6cfcbc);
        uint8_t* tr = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, i, 0));
        if (F<int32_t>(tr, 0x51) != F<int32_t>(c, 0)) continue;
        Collection* t2 = hook::Global<Collection*>(0x6cfcbc);
        void** vt = *(void***)t2;
        uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x6cfcbc), 0, i, 0);
        ((void(__thiscall*)(Collection*, uint32_t))vt[2])(hook::Global<Collection*>(0x6cfcbc), id);
    }
    for (int32_t i = Collection_Count(hook::Global<Collection*>(0x62b26c)) - 1; i >= 0; --i) {
        Collection* bs = hook::Global<Collection*>(0x62b26c);
        uint8_t* b = (uint8_t*)Collection_Get(bs, 0, Collection_IdAt(bs, 0, i, 0));
        if (F<int32_t>(b, 0x276) != F<int32_t>(c, 0)) continue;
        if (((int32_t(__thiscall*)(uint8_t*))(*(void***)b)[0x70 / 4])(b)) continue;
        if (Obj_StateKindIs4(b)) continue;
        F<int32_t>(b, 0x276) = 0;
    }
    char text[0x190];
    FormatText(text, GetString(0x133), (const char*)c + 4, 0, 0, 0, 0, 0, 0, 0);
    StrCopyN((char*)c + 4, text, 0x32);
    View_RefreshForCompany(F<int32_t>(c, 0));
}
RT3_REPLACE(0x426d60, Company_Dissolve);

// 0x426ef0. Company: owning player record, if valid.
RT3_NOINLINE void* __fastcall Company_Owner(void* c) {
    if (!Collection_IsValid(g_players, 0, F<uint32_t>(c, 0x3b))) return nullptr;
    return Collection_Get(g_players, 0, F<uint32_t>(c, 0x3b));
}
RT3_REPLACE(0x426ef0, Company_Owner);

// 0x426f20. Names train t (+4, up to 50 chars) after the route between cities a and
// b. The bigger city (+0x25e) leads; +0xd57 becomes the owner's (t+0) active station
// in the bigger city, else in the smaller one. 65% of the time the name is
// "big-lesser" (0x5c9ba8); otherwise, or when that is too long, a compass name
// (0xb5d..0xb60) from where the big city lies, in cells from the map centre plus
// the step towards the lesser city (east/west only beyond 10 cells), and as a last
// resort 0xb61/0xb62 (60% the latter).
void __fastcall Train_AutoName(uint8_t* t, int, uint32_t a, uint32_t b) {
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    uint8_t* big = (uint8_t*)Collection_Get(cities, 0, a);
    uint8_t* lesser = (uint8_t*)Collection_Get(cities, 0, b);
    if (F<float>(lesser, 0x25e) > F<float>(big, 0x25e)) {
        uint8_t* x = lesser;
        lesser = big;
        big = x;
    }
    Collection* sts = hook::Global<Collection*>(0x6cec20);
    for (int32_t i = 0; i < Collection_Count(sts); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(sts, 0, Collection_IdAt(sts, 0, i, 0));
        int32_t owner = F<int32_t>(t, 0);
        if (Obj_BuildingValue(s) != owner || !Obj_BuildingStateActive(s)) continue;
        if (Obj_BuildingKind(s) == big || (F<uint16_t>(t, 0xd57) == 0 && Obj_BuildingKind(s) == lesser))
            F<uint16_t>(t, 0xd57) = F<uint16_t>(s, 0);
    }
    char buf[0xc8];
    const char* name = (const char*)big + 0x356;
    if (SimRand(0, 100) >= 0x23) {
        FormatText(buf, (const char*)0x5c9ba8, name, (const char*)lesser + 0x356, 0, 0, 0, 0, 0, 0);
        if (strlen(buf) <= 0x32) goto done;
    }
    {
        double (__fastcall* cellX)(void*) = &Obj_CellX;
        double (__fastcall* cellY)(void*) = &Obj_CellY;
        uint8_t* map = hook::Global<uint8_t*>(0x62c120);
        int32_t halfW = F<int32_t>(map, 0x2155) / 2, halfH = F<int32_t>(map, 0x2159) / 2;
        int32_t stepX, stepY, ex, ey;
        float tmp;
        __asm {
            mov ecx, lesser
            call cellX
            fstp tmp
            mov ecx, big
            call cellX
            fsubr tmp
            mov eax, 0x5a10d0
            call eax
            mov stepX, eax
            mov ecx, lesser
            call cellY
            fstp tmp
            mov ecx, big
            call cellY
            fsubr tmp
            mov eax, 0x5a10d0
            call eax
            mov stepY, eax
            mov ecx, big
            call cellX
            fild halfW
            fstp tmp
            fsub tmp
            fdiv dword ptr ds:[0x5c85f8]
            fild stepX
            fstp tmp
            fadd tmp
            mov eax, 0x5a10d0
            call eax
            mov ex, eax
            mov ecx, big
            call cellY
            fild halfH
            fstp tmp
            fsub tmp
            fdiv dword ptr ds:[0x5c85f8]
            fild stepY
            fstp tmp
            fadd tmp
            mov eax, 0x5a10d0
            call eax
            mov ey, eax
        }
        int32_t ax = ex < 0 ? -ex : ex, ay = ey < 0 ? -ey : ey;
        buf[0] = 0;
        int32_t id = 0;
        if (ax > ay) {
            if (ax > 10) id = ex > 0 ? 0xb5d : 0xb5e;
        } else {
            id = ey > 0 ? 0xb5f : 0xb60;
        }
        if (id) FormatText(buf, GetString(id), name, 0, 0, 0, 0, 0, 0, 0);
    }
    {
        size_t len = strlen(buf);
        if (len > 0x32 || len < 2) {
            int32_t id = SimRand(0, 100) >= 0x28 ? 0xb62 : 0xb61;
            FormatText(buf, GetString(id), name, 0, 0, 0, 0, 0, 0, 0);
        }
    }
done:
    buf[0x32] = 0;
    strcpy((char*)t + 4, buf);
}
RT3_REPLACE(0x426f20, Train_AutoName);

// 0x427260. The company's headquarters site: +0xd57 if valid, otherwise its
// active site with the lowest 10 x level - type bonus (+0x8c: 0/1/2 -> 1/2/3).
RT3_NOINLINE void* __fastcall Company_HeadquartersSite(void* c) {
    uint32_t sel = 0;
    uint16_t hq = F<uint16_t>(c, 0xd57);
    if (hq && Collection_IsValid(g_table6cec20, 0, hq)) {
        sel = F<uint16_t>(c, 0xd57);
        if (sel) return Collection_Get(g_table6cec20, 0, sel);
    }
    int32_t best = 999999999;
    for (int32_t i = 0; i < Collection_Count(g_table6cec20); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != F<int32_t>(c, 0)) continue;
        if (!Obj_BuildingStateActive(r)) continue;
        int32_t s = F<int32_t>(Obj_Building(r), 0x3d5) * 10;
        switch (F<uint8_t>(Virtual80(Obj_Building(r)), 0x8c)) {
        case 0: s -= 1; break;
        case 1: s -= 2; break;
        case 2: s -= 3; break;
        }
        if (s < best) {
            best = s;
            sel = F<uint32_t>(r, 0);
        }
    }
    return sel ? Collection_Get(g_table6cec20, 0, sel) : nullptr;
}
RT3_REPLACE(0x427260, Company_HeadquartersSite);

// 0x427380. Company has anything: flag +0x3f, the counters +0x7664..+0x766c, or cargo sites.
RT3_NOINLINE int32_t __fastcall Company_HasAssets(void* c) {
    if (F<int32_t>(c, 0x3f)) return 1;
    if (F<int32_t>(c, 0x766c) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664) > 0) return 1;
    return Company_CountCargoSites(c, 0, 0) > 0 ? 1 : 0;
}
RT3_REPLACE(0x427380, Company_HasAssets);

// 0x4273c0. Buys back `n` shares at the price after the buyback (70% of them
// counted), paying 2% commission (stat 0xc) on top.
RT3_NOINLINE void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n) {
    double dn = n;
    double price = Company_SharePrice(c, 0, Ftol(dn * D_205(0x5c8680)), 1);
    double cost = dn * price;
    Company_PostStat(c, 0, 0xc, (float)(cost * D_205(0x5c9bb0)), 1);
    Company_AddStat(c, 0, 0xd, (float)-cost);
    F<int32_t>(c, 0x47) -= n;
}
RT3_REPLACE(0x4273c0, Company_BuyBackShares);

// 0x427450. Issues `n` new shares: at the market price after the issue (70% of
// them counted, 5% commission, date noted in +0x16b / +0x16f with the previous one
// kept in +0x173 / +0x177), or at `price` when `atPrice` is set.
RT3_NOINLINE void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price) {
    int32_t p = price;
    if (!atPrice) {
        double issued = (double)-n;
        p = Ftol(Company_SharePrice(c, 0, Ftol(issued * D_205(0x5c8680)), 1));
        int32_t cost = -(n * p);
        Company_PostStat(c, 0, 0xc, (float)((double)cost * D_205(0x5c9ba0)), 1);
        F<uint32_t>(c, 0x173) = F<uint32_t>(c, 0x16b);
        F<uint32_t>(c, 0x177) = F<uint32_t>(c, 0x16f);
        F<uint32_t>(c, 0x16b) = F<uint32_t>(WorldG(), 0xd);
        F<uint32_t>(c, 0x16f) = F<uint32_t>(WorldG(), 0x11);
    }
    Company_AddStat(c, 0, 0xd, (float)(n * p));
    F<int32_t>(c, 0x47) += n;
}
RT3_REPLACE(0x427450, Company_IssueShares);

// 0x427540. Company_BondRate for a rating with `extraDebt` more debt, without the
// 2% (0 for an inactive company; the second argument is unused).
RT3_NOINLINE float __fastcall Company_BondRateWithDebt(uint8_t* c, int, int32_t extraDebt, int32_t) {
    if (!F<int32_t>(c, 0x3f)) return Fl(0x5c8598);
    double risk = (D_205(0x5c85f0) - Company_CreditRating(c, 0, extraDebt)) / D_205(0x5c85d8);
    return (float)(Obj_ValueWithBonus((uint32_t*)c) + risk);
}
RT3_REPLACE(0x427540, Company_BondRateWithDebt);

// 0x427590. May issue a bond: fewer than 20 bonds, a credit rating of at least 5,
// and the scenario allows bonds (world +0x4a8b clear).
int32_t __fastcall Company_CanIssueBond(uint8_t* c) {
    if (F<int32_t>(WorldG(), 0x4a8b) || F<int32_t>(c, 0x5b) >= 0x14) return 0;
    return Ftol(Company_CreditRating(c, 0, 0)) >= 5 ? 1 : 0;
}
RT3_REPLACE(0x427590, Company_CanIssueBond);

// 0x4275c0. Issues a bond of `amount` for `years` at `rate` (0: the rate the
// company would get with that debt), paying 2% commission when asked; at most 20
// bonds (+0x5b, 12-byte entries at +0x5f: amount, maturity year, rate).
RT3_NOINLINE int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate) {
    if (F<int32_t>(c, 0x5b) >= 0x14) return 0;
    int32_t due = CurYear() + years;
    if (Fl(0x5c8598) == rate) rate = Company_BondRateWithDebt(c, 0, amount, years);
    if (commission) Company_PostStat(c, 0, 0xc, (float)((double)-amount * D_205(0x5c8918)), 1);
    Company_AddStat(c, 0, 0x12, (float)-amount);
    Company_AddStat(c, 0, 0xd, (float)amount);
    uint8_t* b = c + 0x5f + F<int32_t>(c, 0x5b) * 0xc;
    F<int32_t>(b, 0) = amount;
    F<int32_t>(b, 4) = due;
    F<float>(b, 8) = rate;
    ++F<int32_t>(c, 0x5b);
    return 1;
}
RT3_REPLACE(0x4275c0, Company_IssueBond);

// 0x4276d0. Company month end (active companies, not paused): the outside
// shareholders' mood +0xd31 (reset with a chairman who has +0x1e1, else 92% of
// it plus 8% of a holdings term); outside the editor / sandbox, the monthly
// interest on the two loan balances (stats 0xf / 0xe, 5% a year as negative
// figures, by effects 0x42 / 0x43 and the company quality) and the book-value
// fee (effect 0x44); interest on the cash (at most $500M); each quarter the
// bond interest and the dividend (+0x4b a share: cut to 0 when it would drain
// the cash past 20% of stat 0x31 or nothing can be paid, else capped at the
// maximum; the player is told, strings 0x127 / 0x128), paid to every holder;
// the chairman's salary (+0x14f a year); the share momentum +0x4f decays 10%.
void __fastcall Company_MonthEnd(uint8_t* c) {
    if (!F<int32_t>(c, 0x3f)) return;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38)) return;
    bool quarter = hook::Global<uint8_t*>(0x6cec78)[0xf] % 3 == 0;
    if (F<int32_t>(c, 0x3b) > 0 && F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)), 0x1e1)) {
        F<uint32_t>(c, 0xd31) = 0;
    } else {
        int32_t shares = F<int32_t>(c, 0x47);
        float held = (float)Company_HumanHoldings((int32_t*)c);
        float sf = (float)shares;
        float term = (float)(((D_205(0x5c8770) - (double)held / ((double)sf + Fl(0x5c85b4))) + D_205(0x5c8770)) * D_205(0x5c8628));
        F<float>(c, 0xd31) = (float)((double)F<float>(c, 0xd31) * D_205(0x5c8890) + (double)term * D_205(0x5c86e0));
    }
    if (!hook::Global<uint8_t*>(0x6cec7c)[0x82] && !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) {
        auto loan = [&](int32_t stat, int32_t effect, int32_t post) {
            double v = Company_Stat(c, 0, stat, 0x2329) * D_205(0x5c9bb8) / D_205(0x5c85d8);
            if (D_205(0x5c8608) < v) v = 0.0;
            v = Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, effect, F<uint32_t>(c, 0), F<uint32_t>(c, 0x3b), 0) * v;
            double q = Company_QualityFactor(c) * v;
            Company_PostStat(c, 0, post, (float)q, 1);
        };
        loan(0xf, 0x42, 5);
        loan(0xe, 0x43, 6);
        float fee = (float)((double)-F<int32_t>(c, 0x153) / D_205(0x5c8978));
        fee = (float)(Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, 0x44, F<uint32_t>(c, 0), F<uint32_t>(c, 0x3b), 0) * fee);
        Company_PostStat(c, 0, 0xa, fee, 1);
    }
    float cash = (float)Company_Stat(c, 0, 0xd, 0x2329);
    if (!(cash < Fl(0x5c9b94))) cash = BitsF(0x4dee6b28);
    Company_PostStat(c, 0, 9, (float)((double)Company_CashInterestRate(c) * cash / D_205(0x5c8978)), 1);
    if (quarter) {
        double bonds = Company_Stat(c, 0, 0x12, 0x2329);
        Company_PostStat(c, 0, 9, (float)(Obj_WeightedAverage(c) * bonds / D_205(0x5c87b8)), 1);
        double cashNow = Company_Stat(c, 0, 0xd, 0x2329);
        float sf = (float)F<int32_t>(c, 0x47);
        double after = cashNow + -((double)sf * F<float>(c, 0x4b)) / D_205(0x5c87b8);
        bool cut = false;
        if (F<float>(c, 0x4b) > Fl(0x5c8598)) {
            if (after < D_205(0x5c8608) && -(Company_Stat(c, 0, 0x31, 0x2329) * D_205(0x5c8c20)) > after) cut = true;
            else {
                double md = Company_MaxDividend(c);
                if (!(md > D_205(0x5c85b8) || md != md)) cut = true;
            }
        }
        if (cut) {
            F<uint32_t>(c, 0x4b) = 0;
            if (F<int32_t>(c, 0) == Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) && !hook::Global<uint8_t*>(0x6cec7c)[0x82] &&
                !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef))
                World_AddNamedEntry(hook::Global<uint8_t*>(0x6cec78), 0, GetString(0x127), 0, 0, -1, -1, -1, -1);
        } else {
            double cap = (double)F<float>(c, 0x4b) * D_205(0x5c8750);
            if (Company_MaxDividend(c) < cap) {
                F<float>(c, 0x4b) = Company_MaxDividend(c);
                if (F<int32_t>(c, 0) == Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) && !hook::Global<uint8_t*>(0x6cec7c)[0x82] &&
                    !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef)) {
                    char num[0x100], text[0x200];
                    const char* ns = Num_Format(num, 10, (double)F<float>(c, 0x4b), 10, 1, 0, 0);
                    FormatText(text, GetString(0x128), ns, 0, 0, 0, 0, 0, 0, 0);
                    World_AddNamedEntry(hook::Global<uint8_t*>(0x6cec78), 0, text, 0, 0, -1, -1, -1, -1);
                }
            }
        }
        if (F<float>(c, 0x4b) > Fl(0x5c8598)) {
            float sf2 = (float)F<int32_t>(c, 0x47);
            Company_AddStat(c, 0, 0xd, (float)(-((double)sf2 * F<float>(c, 0x4b)) / D_205(0x5c87b8)));
            F<float>(c, 0x53) = (float)((double)F<float>(c, 0x4b) / D_205(0x5c87b8) + F<float>(c, 0x53));
            int32_t np = Table_CountField4(hook::Global<Collection*>(0x6ceb9c));
            for (int32_t i = 0; i < np; ++i) {
                uint8_t* p = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, i);
                int32_t held = F<int32_t>(p, 0x15d + F<int32_t>(c, 0) * 4);
                if (!held) continue;
                float hf = (float)held;
                Obj_AddDouble154(p, 0, Ftol((double)hf * F<float>(c, 0x4b) / D_205(0x5c87b8)));
            }
        }
    }
    Company_PostStat(c, 0, 0xb, (float)((double)-F<int32_t>(c, 0x14f) / D_205(0x5c8978)), 1);
    if (F<uint32_t>(c, 0x3b)) {
        uint8_t* chair = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b));
        Obj_AddDouble154(chair, 0, Ftol((double)F<int32_t>(c, 0x14f) / D_205(0x5c8978)));
    }
    F<float>(c, 0x4f) = (float)((double)F<float>(c, 0x4f) * D_205(0x5c8888));
    Company_SharePrice(c, 0, 0, 1);
}
RT3_REPLACE(0x4276d0, Company_MonthEnd);

// 0x427c70. Company c gets chairman `player` (+0x3b; ids in 0x6ceb9c). The
// old chairman keeps the age penalty in his rating (+0x28d, at least -99) and
// leaves (+0x1dd = 0, and as the local player's company 0x6cec78+0x21). The
// AI mood +0xd31 drops to 0 under a human (+0x1e1), else decays (x 0x5c8640
// + 0x5c8c20). The new one is linked both ways and his rating copied to
// +0x34b; +0x14f = 10000, +0x43 = today. A company a human left is
// re-planned by the AI. The company views refresh (0x808).
RT3_NOINLINE void __fastcall Company_SetChairman(int32_t* c, int, int32_t player) {
    Collection* players = hook::Global<Collection*>(0x6ceb9c);
    int32_t old = F<int32_t>(c, 0x3b);
    if (old == player) return;
    int32_t wasHuman = old > 0 && F<int32_t>(Collection_Get(players, 0, (uint32_t)old), 0x1e1) ? 1 : 0;
    if (F<int32_t>(c, 0x3b)) {
        uint8_t* p = (uint8_t*)Collection_Get(players, 0, F<uint32_t>(c, 0x3b));
        int32_t r = F<int32_t>(p, 0x28d);
        r += Player_CompanyAgePenalty(p);
        if (r <= -0x63) r = -0x63;
        F<int32_t>(p, 0x28d) = r;
        F<int32_t>(p, 0x1dd) = 0;
        if (F<int32_t>(c, 0x3b) == Obj_Get25(hook::Global<void*>(0x6cec78))) Obj_Set21(hook::Global<void*>(0x6cec78), 0, 0);
    }
    F<int32_t>(c, 0x3b) = player;
    uint8_t* np = nullptr;
    if (player) np = Collection_IsValid(players, 0, (uint32_t)player) ? (uint8_t*)Collection_Get(players, 0, F<uint32_t>(c, 0x3b)) : nullptr;
    if (player && F<int32_t>(np, 0x1e1)) F<int32_t>(c, 0xd31) = 0;
    else F<float>(c, 0xd31) = (float)(F<float>(c, 0xd31) * D_207(0x5c8640) + D_207(0x5c8c20));
    if (F<int32_t>(c, 0x3b)) {
        F<int32_t>(Collection_Get(players, 0, F<uint32_t>(c, 0x3b)), 0x1dd) = c[0];
        if (F<int32_t>(c, 0x3b) == Obj_Get25(hook::Global<void*>(0x6cec78))) Obj_Set21(hook::Global<void*>(0x6cec78), 0, c[0]);
        F<int32_t>(c, 0x34b) = F<int32_t>(Collection_Get(players, 0, F<uint32_t>(c, 0x3b)), 0x28d);
    } else {
        F<int32_t>(c, 0x34b) = 0;
    }
    F<int32_t>(c, 0x14f) = 0x2710;
    F<int32_t>(c, 0x43) = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    bool humanNow = F<int32_t>(c, 0x3b) > 0 && F<int32_t>(Collection_Get(players, 0, F<uint32_t>(c, 0x3b)), 0x1e1);
    if (!humanNow && wasHuman) AiCompany_ReplanTrains(c);
    Ui_SendEvent5(hook::Global<uint8_t**>(0x6d401c), 0, 0xba, 0x808, 0, 0, 0);
}
RT3_REPLACE(0x427c70, Company_SetChairman);

 // namespace

// 0x427e20. Company c takes over company `id` at `price` a share. A company
// without stock (+0x3f) and with assets (+0x7664.. or cargo sites) is
// bought out of bankruptcy (news 0xfd6 / 0xfd7 naming c's chairman; c takes
// 2/3 of its +0x124f / +0x12a7 accounts and pays that much, then the rest
// is dissolved). Otherwise it is merged (news 0x134 / 0x135): every
// shareholder is paid price x shares, c pays shares x price, takes up to 20
// of its share issues (+0x5b, 12 bytes at +0x5f), folding the rest into its
// own (+0x143 count, +0x147 total, +0x14b rate rounded to 0.01) from
// version 0x68, adds its six accounts and takes its trains. Both then take
// its industries, tracks, routes and open territories; c's share price is
// refreshed, the asset views and window 0x808 update, and (version 0x67)
// trigger-6 events fire.
RT3_NOINLINE void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price) {
    uint8_t* o = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
    int32_t bankrupt = 0;
    float cost;
    char title[0x12c], text[0x12c];
    if (!F<int32_t>(o, 0x3f)) {
        if (F<int32_t>(o, 0x766c) + F<int32_t>(o, 0x7668) + F<int32_t>(o, 0x7664) <= 0 && Company_CountCargoSites(o, 0, 0) <= 0) return;
        bankrupt = 1;
        {
            const char* fmt = GetString(0xfd6);
            FormatText(title, fmt, (const char*)c + 4, (const char*)o + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        const char* chair = (const char*)0x5c87a8;
        if (Collection_IsValid(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)) &&
            Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)))
            chair = (const char*)Company_Owner(c) + 8;
        {
            const char* fmt = GetString(0xfd7);
            FormatText(text, fmt, chair, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, title, text, 1, 0);
        cost = (float)(F<double>(o, 0x124f) + F<double>(o, 0x12a7));
        cost = (float)((double)cost * *(const double*)0x5c9b48);
        F<double>(c, 0x124f) = F<double>(o, 0x124f) * *(const double*)0x5c9b48 + F<double>(c, 0x124f);
        F<double>(c, 0x12a7) = F<double>(o, 0x12a7) * *(const double*)0x5c9b48 + F<double>(c, 0x12a7);
    } else {
        {
            const char* fmt = GetString(0x134);
            FormatText(title, fmt, (const char*)c + 4, (const char*)o + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, title, GetString(0x135), 1, 0);
        cost = (float)((double)F<int32_t>(o, 0x47) * (double)price);
        for (int32_t k = 0; k < Table_CountField4(hook::Global<Collection*>(0x6ceb9c)); ++k) {
            uint8_t* pl = (uint8_t*)Table_NthField4(hook::Global<Collection*>(0x6ceb9c), 0, k);
            if (int32_t s = F<int32_t>(pl, (int32_t)id * 4 + 0x15d)) Obj_AddDouble154(pl, 0, s * price);
            F<int32_t>(pl, (int32_t)id * 4 + 0x15d) = 0;
        }
        float n = 0.0f, rate = 0.0f, total = 0.0f;
        for (int32_t k = 0; k < F<int32_t>(o, 0x5b); ++k) {
            const uint8_t* e = o + 0x5f + k * 0xc;
            if (F<int32_t>(c, 0x5b) < 0x14) {
                memcpy(c + 0x5f + F<int32_t>(c, 0x5b) * 0xc, e, 0xc);
                ++F<int32_t>(c, 0x5b);
            } else {
                int32_t a = F<int32_t>((void*)e, 0), b = F<int32_t>((void*)e, 4);
                float fa = (float)a;
                n = fa + n;
                rate = (float)((double)fa * F<float>((void*)e, 8) + rate);
                total = (float)(b * a) + total;
            }
        }
        if (n > *(const float*)0x5c85b4 && Game_EffectiveVersion() >= 0x68) {
            int32_t a = F<int32_t>(c, 0x143), b = F<int32_t>(c, 0x147);
            float fa = (float)a;
            n = fa + n;
            rate = (float)((double)fa * F<float>(c, 0x14b) + rate);
            total = (float)(b * a) + total;
            rate = (float)((double)rate / n);
            total = (float)((double)total / n);
            F<int32_t>(c, 0x143) = (int32_t)((double)n + *(const double*)0x5c86b8);
            F<int32_t>(c, 0x147) = (int32_t)((double)total + *(const float*)0x5c85b4);
            int32_t r = (int32_t)(((double)rate + *(const double*)0x5c9bc0) * *(const double*)0x5c85d8);
            F<float>(c, 0x14b) = (float)((double)r / *(const double*)0x5c85d8);
        }
        static const uint16_t kAccounts[] = { 0x11f7, 0x124f, 0x12a7, 0x12ff, 0x1357, 0x13af };
        for (uint16_t a : kAccounts) F<double>(c, a) = F<double>(o, a) + F<double>(c, a);
        AbsorbBack(0x6cfcbc, [&](uint8_t* t) {
            if (F<uint32_t>(t, 0x51) == id) F<int32_t>(t, 0x51) = F<int32_t>(c, 0);
        });
    }
    Company_AddStat(c, 0, 0xd, -cost);
    AbsorbBack(0x62b26c, [&](uint8_t* b) {
        if (F<uint32_t>(b, 0x276) == id) F<int32_t>(b, 0x276) = F<int32_t>(c, 0);
    });
    AbsorbBack(0x6cfca8, [&](uint8_t* t) {
        if (F<uint16_t>(t, 0x22e) == id) Track_SetOwner(t, 0, F<uint16_t>(c, 0));
    });
    AbsorbBack(0x6cfcb4, [&](uint8_t* r) {
        if (F<uint32_t>(r, 0x1e) == id) F<int32_t>(r, 0x1e) = F<int32_t>(c, 0);
    });
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfc9c)); ++i) {
        int32_t off = (int32_t)Collection_IdAt(hook::Global<Collection*>(0x6cfc9c), 0, (uint32_t)i, 0) * 0x38d + 0x1d9f;
        if (o[off] && !c[off]) c[off] = 1;
    }
    Company_SharePrice(c, 0, 0, 0);
    if (!bankrupt) Company_Dissolve(o);
    F<int32_t>(o, 0x7664) = 0, F<int32_t>(o, 0x7668) = 0, F<int32_t>(o, 0x766c) = 0;
    View_RefreshCompanyAssets(F<int32_t>(c, 0), (int32_t)id);
    Ui_SendEvent5(hook::Global<uint8_t**>(0x6d401c), 0, 0xba, 0x808, 0, 0, 0);
    if (Game_EffectiveVersion() >= 0x67) Events_Fire(hook::Global<Collection*>(0x62be18), 0, 6, nullptr, 0);
}
RT3_REPLACE(0x427e20, Company_Absorb);

// 0x428420. Sets up company c (id) for chairman `player`: clears its state
// and its 25 city territory slots (+0x1d9f, 0x38d each; open unless the city
// +0x2d is taken), sets the AI mood (+0xd31 1, 0 under a human), the top
// industry value (+0x347: half the best +0x20 of the cities with an
// available car type, at least 10), founding date and logo (+0x37: `logo`,
// a random free one for -1, or -1 with a custom name `logoName` formatted to
// +0xd65; 0 when out of range), the name (`name` or the next stock name,
// 0x5f2c58 by 0x6cec78+0x41), and starting cash (`cash`, 500000 for -1):
// shares of cash / 10000 halved below 0x5c8c28 (+0xd7b, +0x57) at
// 0x5c86a0 each in lots of 1000 (+0x47), booked as stat 0xd. Then the
// chairman joins (+0x2a3 date, +0x2a7 count), the share price and stats
// (+0xd87, 0x58 apart) are computed and the lists refresh.
RT3_NOINLINE void __fastcall Company_Init(uint8_t* c, int, int32_t id, int32_t player, int32_t logo, const char* name, const char* logoName, int32_t cash) {
    F<int32_t>(c, 0) = id;
    F<int32_t>(c, 0x3b) = 0;
    F<int32_t>(c, 0x323) = 0, F<int32_t>(c, 0x327) = 0, F<int32_t>(c, 0x32b) = 0;
    for (int k = 0; k < 0x35; ++k) F<int32_t>(c, 0x17b + 4 * k + 0xd4) = 0, F<int32_t>(c, 0x17b + 4 * k) = 0;
    c[0xd65] = 0;
    F<int32_t>(c, 0x32f) = 0, F<int32_t>(c, 0x333) = 0, F<int32_t>(c, 0x337) = 0;
    F<int32_t>(c, 0xd19) = 0;
    c[0xd18] = 0, c[0xd17] = 0;
    F<int32_t>(c, 0xd1d) = 0, F<int32_t>(c, 0xd25) = 0, F<int32_t>(c, 0xd29) = 0, F<int32_t>(c, 0xd2d) = 0;
    c[0xd39] = 0;
    F<int32_t>(c, 0xd5d) = 0, F<int32_t>(c, 0xd61) = 0;
    F<int32_t>(c, 0x33b) = 0, F<int32_t>(c, 0x33f) = 0, F<int32_t>(c, 0x343) = 0;
    F<int32_t>(c, 0xd35) = -1;
    F<uint16_t>(c, 0xd57) = 0;
    F<int32_t>(c, 0xd3a) = 0, F<int32_t>(c, 0xd3e) = 0;
    c[0xd56] = 0;
    F<uint32_t>(c, 0xd31) = 0x3f800000;
    if (player && F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)player), 0x1e1)) F<int32_t>(c, 0xd31) = 0;
    for (int32_t k = 0; k < 0x19; ++k) {
        uint8_t* e = c + 0x1da4 + k * 0x38d;
        e[-5] = 1;
        for (int q = -4; q <= 0x34; q += 4) F<int32_t>(e, q) = 0;
        for (int j = 0; j < 0x35; ++j) {
            uint8_t* b = e + 0x10c + 4 * j;
            F<int32_t>(b, -0xd4) = 0, F<int32_t>(b, 0) = 0, F<int32_t>(b, 0xd4) = 0, F<int32_t>(b, 0x1a8) = 0;
        }
        Collection* cities = hook::Global<Collection*>(0x6cfc9c);
        if (Collection_IsValid(cities, 0, (uint32_t)k) && F<int32_t>(Collection_Get(cities, 0, (uint32_t)k), 0x2d)) e[-5] = 0;
    }
    for (int q = 0x7664; q <= 0x767c; q += 4) F<int32_t>(c, q) = 0;
    F<uint32_t>(c, 0xcf7) = 0x3f800000;
    F<int32_t>(c, 0xcfb) = 0;
    F<int32_t>(c, 0x7680) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aaf) ? 0x32 : -1;
    for (int q = 0xd42; q <= 0xd52; q += 4) F<int32_t>(c, q) = 0;
    F<int32_t>(c, 0xd0b) = 0, F<int32_t>(c, 0xd0f) = 0, F<int32_t>(c, 0xd13) = 0;
    F<int32_t>(c, 0x347) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ada84)); ++i) {
        Collection* t = hook::Global<Collection*>(0x6ada84);
        uint8_t* o = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        uint32_t ty = F<uint32_t>(o, 0x72);
        uint8_t* type = ty ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, ty) : nullptr;
        if (CarType_Available(type, 0, 0, 0) && F<float>(o, 0x20) > F<float>(c, 0x347)) F<float>(c, 0x347) = F<float>(o, 0x20);
    }
    float best = (float)(F<float>(c, 0x347) * Fl(0x5c8d48));
    F<float>(c, 0x347) = best;
    F<float>(c, 0x347) = best > Fl(0x5c85f8) ? best : 10.0f;
    F<int32_t>(c, 0x53) = 0;
    F<int32_t>(c, 0x3f) = 1;
    uint64_t d = MakeDate(0x708, 1, 1, 0, 0);
    F<uint32_t>(c, 0x16f) = (uint32_t)(d >> 32);
    F<uint32_t>(c, 0x16b) = (uint32_t)d;
    d = MakeDate(0x708, 1, 1, 0, 0);
    F<int32_t>(c, 0xcf3) = 1;
    F<uint32_t>(c, 0x173) = (uint32_t)d;
    F<uint32_t>(c, 0x177) = (uint32_t)(d >> 32);
    F<int32_t>(c, 0x167) = 0xf;
    F<int32_t>(c, 0x163) = 0, F<int32_t>(c, 0x15f) = 0, F<int32_t>(c, 0x34b) = 0, F<int32_t>(c, 0x34f) = 0;
    memset(c + 0xd7f, 0, 0x3b2 * 4);
    memset(c + 0x1c47, 0, 0x56 * 4);
    memset((void*)0x62baf0, 0, 0xc8 * 4);
    *(uint8_t*)0x62bae8 = 0;
    F<int32_t>(c, 0x353) = 0;
    F<int32_t>(c, 0x14f) = 0x2710;
    F<int32_t>(c, 0xd59) = 0x2710;
    F<int32_t>(c, 0x37) = 0;
    if (logo == -1) {
        if (logoName) FormatText((char*)c + 0xd65, logoName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        else logo = NthFreeCompanySlot(SimRand(0, CountFreeCompanySlots() - 1) + 1);
    }
    if (logo < 0 ? !logoName : logo > 0x23) logo = 0;
    F<int32_t>(c, 0x37) = logo;
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    F<int32_t>(c, 0x157) = F<int32_t>(w, 0xd);
    F<int32_t>(c, 0x15b) = F<int32_t>(w, 0x11);
    F<int32_t>(c, 0x43) = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd);
    F<int32_t>(c, 0xd07) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    F<int32_t>(c, 0x153) = 0x1770;
    F<int32_t>(c, 0x4f) = 0;
    memset(c + 0x35b, 0, 0x266 * 4);
    F<uint16_t>(c, 0xd79) = 0;
    if (name) {
        FormatText((char*)c + 4, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else {
        int32_t n = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x41) % 0x28;
        StrCopyN((char*)c + 4, GetString(*(int32_t*)(0x5f2c58 + n * 4)), 0x32);
        ++F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x41);
    }
    F<int32_t>(c, 0x47) = 0;
    F<uint32_t>(c, 0x4b) = 0x3f000000;
    F<int32_t>(c, 0x5b) = 0;
    memset(c + 0x5f, 0, 0x3c * 4);
    if (cash == -1) cash = 0x7a120;
    float shares = (float)(cash / 10000);
    F<float>(c, 0xd7b) = shares;
    if (!(shares < Fl(0x5c8c28))) {
        double x = F<float>(c, 0xd7b);
        do x = x / Fl(0x5c878c);
        while (!(x < Fl(0x5c8c28)));
        F<float>(c, 0xd7b) = (float)x;
    }
    F<uint32_t>(c, 0x57) = F<uint32_t>(c, 0xd7b);
    float fc = (float)cash;
    int32_t lots = Ftol((double)fc / F<float>(c, 0xd7b) / Fl(0x5c86a0)) * 0x3e8;
    int32_t total = Ftol((double)F<float>(c, 0xd7b)) * lots;
    Company_AddStat(c, 0, 0xd, (float)total);
    F<int32_t>(c, 0x47) += lots;
    Company_SetChairman((int32_t*)c, 0, player);
    if (player) {
        int32_t today = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
        F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)player), 0x2a3) = today;
    }
    Company_SharePrice(c, 0, 0, 1);
    for (int32_t k = 0; k <= 0x2b; ++k) F<double>(c, 0xd87 + k * 0x58) = Company_Stat(c, 0, k, 0x2329);
    if (player) ++F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)player), 0x2a7);
    View_RefreshCompanyLists();
}
RT3_REPLACE(0x428420, Company_Init);

// 0x428a10. Company c drops its chairman when it is `player`.
RT3_NOINLINE void __fastcall Company_DropChairman(int32_t* c, int, int32_t player) {
    if (F<int32_t>(c, 0x3b) == player) Company_SetChairman(c, 0, 0);
}
RT3_REPLACE(0x428a10, Company_DropChairman);

// 0x428a30. Company c gets `player` as chairman with a news item (0x129 with
// his and the company's names, 0x12a); he first leaves his old company
// (+0x1dd).
RT3_NOINLINE void __fastcall Company_InstallChairman(int32_t* c, int, int32_t player) {
    uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)player);
    char buf[0x12c];
    FormatText(buf, GetString(0x129), (const char*)p + 8, (const char*)c + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    News_Add(hook::Global<Collection*>(0x6cea4c), 0, buf, GetString(0x12a), 1, 0);
    if (int32_t old = F<int32_t>(p, 0x1dd)) Company_SetChairman((int32_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, (uint32_t)old), 0, 0);
    Company_SetChairman(c, 0, player);
}
RT3_REPLACE(0x428a30, Company_InstallChairman);

// 0x428ad0. Company c's periodic review (skipped with 0x6cec78+0x46c38 or
// when it isn't active, +0x3f): its credit rating +0x34b follows a growth
// score - the year's revenue (stat 0x1c) and profit (0x1f) growth, each
// squashed by a square root beyond +/-1 and clamped to +/-2, mixed (the
// smaller one weighted 0x5c87b8) - smoothed by the company's age, and drags
// the chairman's rating (+0x28d) along. The bond limit +0x14f moves from
// the rating and the stock (stat 0x2c), never down for a good rating (9
// steps of 9) nor up for a poor one; while it grows, a player chairman may
// earn a bonus (SimRand 20..110 below the rating, at least 5000; +0x353,
// stat 0xb). +0xcf7 is the profit margin factor (0.8..1.2, eased in over 5
// years, halfway to 1 under a human). A bad chairman of a company in decline
// for two years whose outside holders outvote him is ousted (0x12b / 0x12c;
// a warning 0x12d first, +0x357). Three (four for an AI) bad years in a row
// bankrupt it (0x12f / 0x130; a warning 0x12e after two). Then the yearly
// stats roll up (+0x1407, +0x1cdf) and +0x1d77.. average when positive.
RT3_NOINLINE void __fastcall Company_AnnualReview(uint8_t* c) {
    auto Sqrt = (double(__cdecl*)(double))0x5a1634;
    auto year = [] { return (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd); };
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38) || !F<int32_t>(c, 0x3f)) return;

    double a = Company_Stat(c, 0, 0x1c, year());
    double b = F<uint16_t>(c, 0x157) == F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) ? (double)F<float>(c, 0x57) : Company_Stat(c, 0, 0x1c, year() - 1);
    if (!(b > D_207(0x5c85b8))) b = 0.01;
    float g = (float)(a / b - D_207(0x5c8770));
    if (g > D_207(0x5c8770)) g = (float)Sqrt((double)g);
    if (g < D_207(0x5c9bf0)) g = (float)-Sqrt(-(double)g);
    if (g > D_207(0x5c85d0)) g = 2.0f;
    if (g < D_207(0x5c9be8)) g = -2.0f;

    double p = Company_Stat(c, 0, 0x1f, year());
    double q = F<uint16_t>(c, 0x157) == F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) ? 1.0 : Company_Stat(c, 0, 0x1f, year() - 1);
    const double zero = D_207(0x5c8608), one = D_207(0x5c8770);
    if (p == zero) p = 0.01;
    if (q == zero) q = 0.01;
    double x;
    bool ratio = true;
    if (p > zero && q > zero) x = p / (q + one);
    else if (p < zero && q < zero) x = q / (p - one);
    else if (p > zero && q < zero) x = p / (-q + one);
    else {
        x = p < zero && q > zero ? -1.0 : 0.0;
        ratio = false;
    }
    if (ratio) {
        x = x - one;
        if (x > one) x = Sqrt(x);
        if (x < D_207(0x5c9bf0)) x = -Sqrt(-x);
        if (x > D_207(0x5c85d0)) x = 2.0;
        else if (x < D_207(0x5c9be8)) x = -2.0;
    }
    x = x * D_207(0x5c8628);
    double r = g < x ? (double)g * D_207(0x5c87b8) + x * D_207(0x5c8610) : (double)g * D_207(0x5c8610) + x * D_207(0x5c87b8);
    r = r / D_207(0x5c9be0) * D_207(0x5c85d8);
    if (r < zero) r = -(Sqrt(-r) * D_207(0x5c9bd8));
    else if (r > zero) r = Sqrt(r) * D_207(0x5c9bd8);
    r = r * D_207(0x5c88f8);
    int32_t age = year() - (int32_t)F<uint16_t>(c, 0x157);
    double w = 0.45;
    if (age < 3) w = (double)age * D_207(0x5c8988) * D_207(0x5c8c18) + D_207(0x5c9bd0);
    float rf = (float)r;
    if (a < D_207(0x5c85f0) && !(a < D_207(0x5c8628)) && rf > hook::Global<float>(0x5c8598)) rf = (float)((double)rf * a / D_207(0x5c85f0));
    F<int32_t>(c, 0x34b) = Ftol((one - w) * (double)F<int32_t>(c, 0x34b) + (double)rf * w);
    if (int32_t ch = F<int32_t>(c, 0x3b)) {
        int32_t pr = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)ch), 0x28d);
        int32_t v = Ftol((double)pr * D_207(0x5c96e0) + (double)rf * D_207(0x5c8c18));
        F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)ch), 0x28d) = v;
    }

    int32_t rating = F<int32_t>(c, 0x34b);
    double stock = Company_Stat(c, 0, 0x2c, year());
    int32_t v1 = Ftol((Sqrt(stock) + D_207(0x5c85e8)) * (double)(rating * 3 + 0x78) * D_207(0x5c86b8));
    int32_t old = F<int32_t>(c, 0x14f);
    F<int32_t>(c, 0xd59) = old;
    int32_t up = 1;
    int32_t v2 = Ftol((double)old * D_207(0x5c8da8) + (double)v1 * D_207(0x5c8618));
    int32_t lim = (v2 + 0x1f4) / 1000 * 1000;
    F<int32_t>(c, 0x14f) = lim < 1000 ? 1000 : lim;
    int32_t step = (rating + 0x3a) / 9;
    if (step <= 0) step = 0;
    if (step < 0xc && step <= 6) {
        up = 0;
        if (F<int32_t>(c, 0x14f) >= F<int32_t>(c, 0xd59)) F<int32_t>(c, 0x14f) = F<int32_t>(c, 0xd59);
    }
    if (step >= 8 && F<int32_t>(c, 0x14f) <= F<int32_t>(c, 0xd59)) F<int32_t>(c, 0x14f) = F<int32_t>(c, 0xd59);
    bool bonus = false;
    if (F<int32_t>(c, 0x14f) >= F<int32_t>(c, 0xd59) && up && year() - F<int32_t>(c, 0x34f) > 1 && F<int32_t>(c, 0x3b)) {
        int32_t rt = F<int32_t>(c, 0x34b);
        if (SimRand(0x14, 0x6e) < rt) {
            int32_t amount = F<int32_t>(c, 0x14f) * rt / 100 / 1000 + 1000;
            if (amount < 0x1388) amount = 0x1388;
            F<int32_t>(c, 0x34f) = year();
            Obj_AddDouble154(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)), 0, amount);
            Company_PostStat(c, 0, 0xb, (float)-amount, 1);
            F<int32_t>(c, 0x353) = amount;
            bonus = true;
        }
    }
    if (!bonus) F<int32_t>(c, 0x353) = 0;

    Company_SharePrice(c, 0, 0, 1);
    double s2c = Company_Stat(c, 0, 0x2c, year());
    double s2b = Company_Stat(c, 0, 0x2b, year());
    if (age > 0) {
        s2c = Company_Stat(c, 0, 0x2c, year() - 1) * D_207(0x5c8680) + s2c;
        s2b = Company_Stat(c, 0, 0x2b, year() - 1) * D_207(0x5c8680) + s2b;
    }
    if (age > 1) {
        s2c = Company_Stat(c, 0, 0x2c, year() - 2) * D_207(0x5c88e8) + s2c;
        s2b = Company_Stat(c, 0, 0x2b, year() - 2) * D_207(0x5c88e8) + s2b;
    }
    if (s2c < one) s2c = 1.0;
    double ratio2 = s2b / s2c;
    float m = (float)(one - (ratio2 - D_207(0x5c8c20)) / D_207(0x5c85c8));
    F<float>(c, 0xcf7) = m;
    if (m < D_207(0x5c8640)) F<uint32_t>(c, 0xcf7) = 0x3f4ccccd;
    else if (m > D_207(0x5c8698)) F<uint32_t>(c, 0xcf7) = 0x3f99999a;
    if (age < 5) F<float>(c, 0xcf7) = (float)(((double)(float)age * F<float>(c, 0xcf7) + (double)(float)(5 - age)) / D_207(0x5c8d68));
    if (F<int32_t>(c, 0x3b) > 0 && F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)), 0x1e1))
        F<float>(c, 0xcf7) = (float)((F<float>(c, 0xcf7) + one) / D_207(0x5c85d0));

    auto quiet = [] {
        return !F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x188) && !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4ac7) &&
               !F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68) && !hook::Global<uint8_t*>(0x6cec7c)[0x82];
    };
    char buf[0x2b0];
    if (F<int32_t>(c, 0x3b) && Company_FreeShares(c) > F<int32_t>(c, 0x47) >> 1) {
        int32_t bad = 0;
        for (int32_t i = 0, lim2 = -0xf; lim2 < 0; ++i, lim2 += 5) {
            int32_t y = year() - i;
            if (y < (int32_t)F<uint16_t>(c, 0x157) || y < F<int32_t>(c, 0x43)) break;
            if (!(Company_Stat(c, 0, 0x29, y) < (double)lim2)) break;
            if (!(Company_Stat(c, 0, 0x1f, year() - i) < one)) break;
            ++bad;
        }
        if (bad >= 2) {
            float anger = (float)((double)-F<int32_t>(c, 0x34b) * D_207(0x5c9bc8));
            if (anger > Fl(0x5c8c28)) anger = 100.0f;
            int32_t votes = Ftol((double)anger / D_207(0x5c85d8) * (double)Company_FreeShares(c));
            if (anger > Fl(0x5c8780)) {
                Collection* players = hook::Global<Collection*>(0x6ceb9c);
                for (int32_t i = 0; i < Table_CountField4(players); ++i) {
                    uint8_t* pl = (uint8_t*)Table_NthField4(players, 0, i);
                    int32_t held = F<int32_t>(pl, 0x15d + F<int32_t>(c, 0) * 4);
                    if (held > 0 && !F<int32_t>(pl, 0x1e1) && F<int32_t>(pl, 0) != F<int32_t>(c, 0x3b)) votes += held;
                    players = hook::Global<Collection*>(0x6ceb9c);
                }
            }
            if ((double)F<int32_t>(c, 0x47) * D_207(0x5c8628) < (double)votes && quiet()) {
                if (F<int32_t>(c, 0x357) >= year() - 1) {
                    uint8_t* pl = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b));
                    FormatText(buf, GetString(0x12b), (const char*)pl + 8, (const char*)c + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                    News_Add(hook::Global<Collection*>(0x6cea4c), 0, buf, GetString(0x12c), 1, 0);
                    Company_SetChairman((int32_t*)c, 0, 0);
                } else {
                    if (F<int32_t>(c, 0x3b) == Obj_Get25(hook::Global<void*>(0x6cec78)))
                        World_AddNamedEntry(hook::Global<void*>(0x6cec78), 0, GetString(0x12d), 0, 1, -1, -1, -1, -1);
                    F<int32_t>(c, 0x357) = year();
                }
            }
        }
    }

    int32_t years = 3, minCash = 0, minStock = 0;
    Collection* players = hook::Global<Collection*>(0x6ceb9c);
    if (Collection_IsValid(players, 0, F<uint32_t>(c, 0x3b)) && Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)) &&
        (F<int32_t>(c, 0x3b) <= 0 || !F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, F<uint32_t>(c, 0x3b)), 0x1e1))) {
        years = 4;
        minCash = 0x445c0;
        minStock = 0x7a120;
    }
    int32_t i = 0;
    for (int32_t lim3 = 5; i < years; ++i, lim3 += 2) {
        int32_t y = year() - i;
        if (y < (int32_t)F<uint16_t>(c, 0x157)) break;
        if (!(Company_Stat(c, 0, 0x1c, y) < (double)lim3)) break;
        if (!(Company_Stat(c, 0, 0x31, year() - i) < (double)minStock)) break;
        if (!(Company_Stat(c, 0, 0x2b, year() - i) < D_207(0x5c86f8))) break;
        if (!(Company_Stat(c, 0, 0xd, year() - i) < (double)minCash)) break;
    }
    if (i == 2 && Obj_Get21(hook::Global<void*>(0x6cec78)) == F<int32_t>(c, 0))
        World_AddNamedEntry(hook::Global<void*>(0x6cec78), 0, GetString(0x12e), 0, 0, -1, -1, -1, -1);
    if (i == years && quiet()) {
        FormatText(buf, GetString(0x12f), (const char*)c + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Company_Dissolve(c);
        News_Add(hook::Global<Collection*>(0x6cea4c), 0, buf, GetString(0x130), 1, 0);
    }

    for (int32_t k = 0x13; k <= 0x2a; ++k) F<double>(c, 0x1407 + (k - 0x13) * 0x58) = Company_Stat(c, 0, k, 0x2329);
    for (int32_t k = 0x13; k <= 0x1b; ++k) F<double>(c, 0x1cdf + (k - 0x13) * 8) = Company_Stat(c, 0, k, 0x2329) + F<double>(c, 0x1cdf + (k - 0x13) * 8);
    for (int32_t k = 0x26; k <= 0x28; ++k) {
        float v = (float)Company_Stat(c, 0, k, 0x2329);
        if (v < D_207(0x5c86b8) || v != v) continue;
        double& avg = F<double>(c, 0x1d77 + (k - 0x26) * 8);
        if (avg > D_207(0x5c86b8) || avg != avg) {
            int32_t n = year() - (int32_t)F<uint16_t>(c, 0x157);
            if (n < 1) n = 1;
            avg = ((double)n * avg + (double)v) / (double)(float)(n + 1);
        } else {
            avg = (double)v;
        }
    }
}
RT3_REPLACE(0x428ad0, Company_AnnualReview);

// 0x429910. Free shares (a quarter with `flag`, else half) minus player debt.
int32_t __fastcall Company_SpareShares(int32_t* c, int, int32_t flag) {
    int32_t s = Company_FreeShares(c);
    s = flag ? s >> 2 : s >> 1;
    return s - Company_PlayerDebt(c);
}
RT3_REPLACE(0x429910, Company_SpareShares);

// 0x429950 / 0x429980. Companies collection (vtbl 0x5c9bf8, inline 0x7684-byte records, 5 + 5, list 1).
RT3_NOINLINE Collection* __fastcall Companies_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9bf8;
    Collection_Init(c, 0, 1, 0x7684, 5, 5, 1, 0, 0);
    return c;
}
RT3_REPLACE(0x429950, Companies_Construct);

RT3_NOINLINE void __fastcall Companies_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9bf8;
    Collection_Destruct(c);
}
RT3_REPLACE(0x429980, Companies_Destruct);

// 0x429990. ID of the n-th (0-based) record with a nonzero +0x3f, or 0.
RT3_NOINLINE uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) == 0) continue;
        if (n == 0) return Collection_IdAt(t, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x429990, Table_NthIdField3f);

// 0x4299f0. Number of live (+0x3f) records before the one with ID `id` (0 if absent).
RT3_NOINLINE int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Collection_IdAt(t, 0, i, 0) == id) return n;
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f)) ++n;
    }
    return 0;
}
RT3_REPLACE(0x4299f0, Table_LiveRankOf);

// 0x429a50. Count of records with a nonzero dword at +0x3f (e.g. live companies).
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) != 0) ++n;
    return n;
}
RT3_REPLACE(0x429a50, Table_CountField3f);

// 0x429a90. The n-th (0-based) record with a nonzero dword at +0x3f, or NULL.
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) == 0) continue;
        if (n == 0) return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        --n;
    }
    return nullptr;
}
RT3_REPLACE(0x429a90, Table_NthField3f);

// 0x429af0. Companies load framed by 0x61a9/0x61aa/0x61ab; each record then
// clamps its dates. Returns the byte count.
RT3_NOINLINE int32_t __fastcall Companies_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x61a9, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x61aa, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        n += Company_ClampDates(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, (int32_t)s);
    return StreamSkip4(s, 0x61ab, 0, 0) + n;
}
RT3_REPLACE(0x429af0, Companies_Load);

// 0x429b90. Save framed by 0x61a9/0x61aa/0x61ab; per record the (empty) hook 0x424000.
RT3_NOINLINE void __fastcall Companies_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((int32_t(__thiscall*)(void*, Stream))0x424000)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x429b90, Companies_Save);

// 0x429c10. Rebuilds the station link tables of all active companies (+0x3f),
// recounting them before each one.
RT3_NOINLINE void __fastcall Companies_BuildStationLinks(Collection* cos) {
    for (int32_t i = 0;; ++i) {
        int32_t active = 0;
        for (int32_t k = 0; k < Collection_Count(cos); ++k)
            if (F<int32_t>(Collection_Get(cos, 0, Collection_IdAt(cos, 0, k, 0)), 0x3f)) ++active;
        if (i >= active) return;
        Company_BuildStationLinks((int32_t*)Table_NthField3f(cos, 0, i));
    }
}
RT3_REPLACE(0x429c10, Companies_BuildStationLinks);

// 0x429c70. Derived stats 0x2b..0x38: revenue / expense / profit totals and
// per-train, per-car and per-ton averages (0 when the divisor is 0); 1.0 for others.
double __fastcall Company_DerivedStat(uint8_t* c, int, int32_t id, int32_t year) {
    auto S = [&](int32_t i) { return Company_Stat(c, 0, i, year); };
    auto sum = [&](std::initializer_list<int32_t> ids) {
        const int32_t* p = ids.begin();
        double t = S(*p++);
        for (; p != ids.end(); ++p) t = S(*p) + t;
        return t;
    };
    auto per = [&](int32_t by, int32_t num) {
        int32_t n = Ftol(S(by));
        if (!n) return D_205(0x5c8608);
        return S(num) / (double)n;
    };
    switch (id) {
    case 0x2b: return sum({ 0x2d, 0x2c });
    case 0x2c: return sum({ 4, 3, 2, 1 });
    case 0x2d: return sum({ 0x2f, 0x2e });
    case 0x2e: return sum({ 0xc, 0xb, 0xa, 8, 7, 6, 5 });
    case 0x2f: return S(9);
    case 0x30: return sum({ 0x11, 0x10, 0xf, 0xe, 0xd });
    case 0x31: return sum({ 0x30, 0x12 });
    case 0x32: return per(0x24, 0x2c);
    case 0x33: return per(0x16, 0x2c);
    case 0x34: return per(0x17, 0x2c);
    case 0x35: return per(0x18, 1);
    case 0x36: return per(0x19, 1);
    case 0x37: return per(0x1a, 2);
    case 0x38: return per(0x1b, 2);
    default: return D_205(0x5c8770);
    }
}
RT3_REPLACE(0x429c70, Company_DerivedStat);

// 0x42a040. Adds to a company statistic: this period's slot (+0xd7f, 0x58
// bytes per stat) and the running total (+0x1c47, 8 per stat), both double.
RT3_NOINLINE void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount) {
    double& period = F<double>(c, 0xd7f + stat * 0x58);
    period = (double)amount + period;
    double& total = F<double>(c, 0x1c47 + stat * 8);
    total = (double)amount + total;
}
RT3_REPLACE(0x42a040, Company_AddStat);

// 0x42a080. Company statistic, and (withCash) the cash accumulators +0x11f7/+0x1caf.
RT3_NOINLINE void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash) {
    Company_AddStat(c, 0, stat, amount);
    if (withCash) {
        double& cash = F<double>(c, 0x11f7);
        cash = (double)amount + cash;
        double& total = F<double>(c, 0x1caf);
        total = (double)amount + total;
    }
}
RT3_REPLACE(0x42a080, Company_PostStat);

// 0x42a0e0. Index of the largest float in the 12-byte entries at +0x67 (count +0x5b), -1 if none.
RT3_NOINLINE int32_t __fastcall Obj_ArgMax67(void* o) {
    int32_t best = -1, n = F<int32_t>(o, 0x5b);
    for (int32_t i = 0; i < n; ++i) {
        if (best == -1 || F<float>(o, 0x67 + i * 0xc) > F<float>(o, 0x67 + best * 0xc)) best = i;
    }
    return best;
}
RT3_REPLACE(0x42a0e0, Obj_ArgMax67);

// 0x42a130. Stat `id` extrapolated to a full year: this year's value over the
// fraction of the year gone (0.05..0.95), blended with last year's (for profit
// stats also adding last year's revenue share); unchanged on the last day.
double __fastcall Company_YearEstimate(uint8_t* c, int, int32_t id) {
    double cur = Company_Stat(c, 0, id, 0x2329);
    uint8_t* w = WorldG();
    if (w[0xf] == 0xc && w[0x11] == 0x1c) return cur;
    int32_t days = w[0xf] * 0x1c + w[0x11] - 0x1c;
    float f = (float)((double)days / D_205(0x5c9a00));
    if (f < D_205(0x5c9ba0)) f = 0.05f;
    if (f > D_205(0x5c8650)) f = 0.95f;
    if ((uint16_t)F<uint16_t>(w, 0xd) > (uint16_t)F<uint16_t>(c, 0x157)) {
        if (id == 0x2c || id == 0x2b || id == 3) cur = Company_Stat(c, 0, 3, CurYear() - 1) * f + cur;
        double prev = Company_Stat(c, 0, id, CurYear() - 1);
        return (D_205(0x5c8770) - f) * prev + f * cur;
    }
    return cur / f;
}
RT3_REPLACE(0x42a130, Company_YearEstimate);

// 0x42a250. Company track length: its nodes (u16 +0x22e) of kind 2 (+0x216),
// double-track (+0x218 == 4) counting twice, times 0.5.
RT3_NOINLINE double __fastcall Company_TrackLength(void* c) {
    int32_t total = Collection_Count(g_trackNodes), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* nd = (uint8_t*)Collection_Get(g_trackNodes, 0, Collection_IdAt(g_trackNodes, 0, i, 0));
        if (F<uint16_t>(nd, 0x22e) != F<int32_t>(c, 0) || F<uint8_t>(nd, 0x216) != 2) continue;
        n += F<uint8_t>(nd, 0x218) == 4 ? 2 : 1;
    }
    float r = (float)((double)n * *(const double*)0x5c8628);
    return r;
}
RT3_REPLACE(0x42a250, Company_TrackLength);

// 0x42a2e0. Live values: counters (0x13..0x15 at +0xd0b.., 0x29 / 0x2a), averages
// (0x16..0x1b, 0x20, 0x23), per-share values (0x1c price, 0x1d book, 0x1e / 0x1f
// estimated revenue / profit), 0x21 growth, 0x22 credit rating, 0x24 trains, 0x25
// track, 0x26..0x28 ratios; others the n = 0 history slot.
double __fastcall Company_LiveStat(uint8_t* c, int, int32_t id) {
    auto ratio = [&](uint32_t den, uint32_t num) {
        float d = F<float>(c, den) > Fl(0x5c85b4) ? F<float>(c, den) : 1.0f;
        return (double)F<float>(c, num) / d;
    };
    switch (id) {
    case 0x13: return F<int32_t>(c, 0xd0b);
    case 0x14: return F<int32_t>(c, 0xd0f);
    case 0x15: return F<int32_t>(c, 0xd13);
    case 0x16: return F<float>(c, 0x323);
    case 0x17: return F<float>(c, 0x327);
    case 0x18: return F<float>(c, 0x32f);
    case 0x19: return F<float>(c, 0x333);
    case 0x1a: return F<float>(c, 0x33b);
    case 0x1b: return F<float>(c, 0x33f);
    case 0x1c: return Company_SharePrice(c, 0, 0, 0);
    case 0x1d: return Company_Stat(c, 0, 0x31, 0x2329) / (double)Shares(c);
    case 0x1e: return Company_YearEstimate(c, 0, 0x2c) / (double)Shares(c);
    case 0x1f: return Company_YearEstimate(c, 0, 0x2b) / (double)Shares(c);
    case 0x20: return F<float>(c, 0x4b);
    case 0x21: return Company_GrowthScore(c);
    case 0x22: return Company_CreditRating(c, 0, 0);
    case 0x23: return F<float>(c, 0x53);
    case 0x24: return (double)Company_TrainCount(c);
    case 0x25: return Company_TrackLength(c);
    case 0x26: return ratio(0x327, 0x32b);
    case 0x27: return ratio(0x333, 0x337);
    case 0x28: return ratio(0x33f, 0x343);
    case 0x29: return F<int32_t>(c, 0x34b);
    case 0x2a: return F<int32_t>(c, 0x14f);
    default: return F<double>(c, 0xd7f + id * 0x58);
    }
}
RT3_REPLACE(0x42a2e0, Company_LiveStat);

// 0x42a5d0. Company stat `id` for `year` (see above); this year's averages
// 0x13..0x1b add the live value; years out of the 10-year history give the live one.
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year) {
    if (id >= 0x2b) return Company_DerivedStat(c, 0, id, year);
    if (year == 0x232a) {
        float f = (float)F<double>(c, 0x1c47 + id * 8);
        if (id >= 0x13 && id <= 0x1b) f = (float)(Company_LiveStat(c, 0, id) + f);
        return f;
    }
    if (year == 0x2329) return Company_LiveStat(c, 0, id);
    int32_t d = CurYear() - year;
    if (d < 0 || d >= 0xb || d == 0) return Company_LiveStat(c, 0, id);
    return F<double>(c, 0xd7f + (id * 0xb + d) * 8);
}
RT3_REPLACE(0x42a5d0, Company_Stat);

RT3_WINDOWED(0x42a660, Cmd_DropModel, Debug_DropModel)

RT3_WINDOWED(0x42a6c0, Cmd_PlaceTestCar, Debug_PlaceTestCar)

RT3_WINDOWED(0x42a720, Cmd_PlaceTestModel, Debug_PlaceTestModel)

// 0x42a780 / 0x42a7c0 (dev commands). Toggle the engine's +0x114206 and set
// +0x114716 when it goes on / clear it and save the configuration.
int32_t Cmd_ToggleEngine114206() {
    if (uint8_t* e = hook::Global<uint8_t*>(0x6d4024)) {
        F<int32_t>(e, 0x114206) = !F<int32_t>(e, 0x114206);
        uint8_t* e2 = hook::Global<uint8_t*>(0x6d4024);
        if (F<int32_t>(e2, 0x114206)) F<int32_t>(e2, 0x114716) = 1;
    }
    return 0;
}
RT3_REPLACE(0x42a780, Cmd_ToggleEngine114206);

int32_t Cmd_ToggleEngine114206Save() {
    if (uint8_t* e = hook::Global<uint8_t*>(0x6d4024)) {
        F<int32_t>(e, 0x114206) = !F<int32_t>(e, 0x114206);
        if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114206)) {
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114716) = 0;
        }
        Engine_SaveConfig(hook::Global<uint8_t*>(0x6d4024));
    }
    return 0;
}
RT3_REPLACE(0x42a7c0, Cmd_ToggleEngine114206Save);

// 0x42a810 (dev command). Sets the renderer's +0x22b57 in single player.
int32_t Cmd_SetRenderer22b57() {
    if (hook::Global<uint8_t*>(0x6cec78) && CountHumanPlayers() <= 1 && Engine_Renderer(hook::Global<void*>(0x6d4024)))
        F<int32_t>((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x22b57) = 1;
    return 0;
}
RT3_REPLACE(0x42a810, Cmd_SetRenderer22b57);

// 0x42a850. Copies `src` into `dst`, drops its last character, keeps only the
// part after the last '\' and cuts at the first '.'. QUIRK: no '\' crashes.
void __fastcall PathToBaseName(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++) != 0) {}
    dst[StrLen(dst) - 1] = 0;
    const char* s = ((const char*(__cdecl*)(const char*, int))0x5a1df0)(dst, '\\') + 1;
    d = dst;
    while ((*d++ = *s++) != 0) {}
    if (StrLen(dst) == 0) return;
    uint16_t i = 0;
    do {
        if (dst[i] == '.') { dst[i] = 0; return; }
        ++i;
    } while (i < StrLen(dst));
}
RT3_REPLACE(0x42a850, PathToBaseName);

// 0x42a8e0. dst = file-name part of src minus its last character, cut at the
// first '_'. QUIRK: a src without '\\' makes the original read address 1.
RT3_NOINLINE void __fastcall BaseNameNoSuffix(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++) != 0) {}
    size_t len = 0;
    while (dst[len]) ++len;
    dst[len - 1] = 0;
    const char* slash = ((const char*(__cdecl*)(const char*, int))0x5a1df0)(dst, '\\'); // exe strrchr
    const char* s = slash + 1;
    d = dst;
    while ((*d++ = *s++) != 0) {}
    len = 0;
    while (dst[len]) ++len;
    for (uint16_t i = 0; len && i < len; ++i) {
        if (dst[i] == '_') { dst[i] = 0; return; }
    }
}
RT3_REPLACE(0x42a8e0, BaseNameNoSuffix);

// 0x42a970. "Open file" dialog (title 0x5c9c04) into `file` (MAX_PATH) with
// the given filter and initial directory; restores the current directory.
// Returns whether a file was chosen.
RT3_NOINLINE int32_t __fastcall OpenFileDialog(char* file, const char* filter, const char* initialDir) {
    char cwd[0x100];
    ((uint32_t(__stdcall*)(uint32_t, char*))hook::Global<void*>(0x5c807c))(0x100, cwd);
    uint32_t ofn[0x13] = {};
    file[0] = 0;
    ofn[0]  = 0x4c;
    ofn[1]  = 0;
    ofn[3]  = (uint32_t)filter;
    ofn[4]  = 0;
    ofn[5]  = 0;
    ofn[6]  = 0;
    ofn[7]  = (uint32_t)file;
    ofn[8]  = 0x104;
    ofn[11] = (uint32_t)initialDir;
    ofn[9]  = 0;
    ofn[10] = 0x104;
    ofn[12] = 0x5c9c04;
    ofn[15] = 0;
    ofn[13] = 0x1000;
    ((int32_t(__stdcall*)(void*))hook::Global<void*>(0x5c8390))(ofn);
    ((int32_t(__stdcall*)(const char*))hook::Global<void*>(0x5c80d8))(cwd);
    return StrLen(file) != 0 ? 1 : 0;
}
RT3_REPLACE(0x42a970, OpenFileDialog);

// 0x42aa30. Debug: picks a .3dp model from .\datad and drops it on the
// terrain (2 above ground) in a diagonal row from the map's centre, 40 units
// further for every model dropped (0x5ee268); its texture is the file name
// up to the first '_'.
RT3_NOINLINE void Debug_DropModel() {
    uint8_t* cursor = hook::Global<uint8_t*>(0xccbb20);
    Section_Enter(cursor);
    Cursor_UseSystem(hook::Global<uint8_t*>(0xccbb20));
    uint8_t* scene = (uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024));
    char file[0x104], name[0x100], path[0x80];
    if (OpenFileDialog(file, (const char*)0x5c9c18, (const char*)0x5c9c34)) {
        PathToBaseName(name, file);
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, (const char*)0x5c9c10, name);
        if (StrLen(name)) {
            uint32_t i = 0;
            do {
                if (name[i] == '_') { name[i] = 0; break; }
                ++i;
            } while (i < StrLen(name));
        }
        uint8_t* o = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x206);
        o = o ? SceneObj_Construct(o, 0, (uint32_t)(uintptr_t)scene, path, name) : nullptr;
        double off = (double)(hook::Global<int32_t>(0x5ee268) * 40);
        uint8_t* world = hook::Global<uint8_t*>(0x62c120);
        float x = (float)((double)F<int32_t>(world, 0x2155) / D(0x5c85d0) * D(0x5c8c48) + off);
        float z = (float)(((double)F<int32_t>(world, 0x2159) / D(0x5c85d0) + D(0x5c8d68)) * D(0x5c8c48) + off);
        float y = (float)(Terrain_HeightAt(scene, 0, x, z) + (double)*(const float*)0x5c878c);
        ++hook::Global<int32_t>(0x5ee268);
        Model_SetFrame(o, 0, 0);
        Obj_MoveTo(o, 0, x, y, z);
    }
    Cursor_PopWindowsMode(hook::Global<uint8_t*>(0xccbb20));
}
RT3_REPLACE(0x42aa30, Debug_DropModel);

// 0x42abd0. Debug: picks a .3dp model file (in .\data\3d) and places it as
// the test vehicle (0x6cfcc0) at the camera's target; counts placements
// (0x5ee26c).
RT3_NOINLINE void Debug_PlaceTestCar() {
    Section_Enter(hook::Global<uint8_t*>(0xccbb20));
    Cursor_UseSystem(hook::Global<uint8_t*>(0xccbb20));
    Engine_Get1C(hook::Global<void*>(0x6d4024));
    char file[0x80], name[0x100];
    if (OpenFileDialog(file, (const char*)0x5c9c18, (const char*)0x5c9c34)) {
        BaseNameNoSuffix(name, file);
        uint32_t x, y, z;
        Obj_GetXYZ4(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, &x, &y, &z);
        if (hook::Global<uint8_t*>(0x6cfcc0)) {
            Vehicle_Teardown(hook::Global<uint8_t*>(0x6cfcc0));
            if (uint8_t* m = hook::Global<uint8_t*>(0x6cfcc0)) {
                PartModel_Destructor(m);
                ((void(__cdecl*)(void*))0x53b080)(m);
            }
            hook::Global<uint32_t>(0x6cfcc0) = 0;
        }
        uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x34a);
        uint8_t* m = mem ? PartModel_Construct(mem) : nullptr;
        hook::Global<uint8_t*>(0x6cfcc0) = m;
        float fz = (float)((double)BitsF(z) / Fl(0x5c8700));
        float fx = (float)((double)BitsF(x) / Fl(0x5c8700));
        Car_LoadModel(m, 0, name, name, fx, fz, 1);
        ++hook::Global<int32_t>(0x5ee26c);
    }
    Cursor_PopWindowsMode(hook::Global<uint8_t*>(0xccbb20));
}
RT3_REPLACE(0x42abd0, Debug_PlaceTestCar);

// 0x42acf0. Debug: picks a .3dp model file (in .\data\3d) and places it as
// the test model (0x62be14) at the camera's target, refreshed when the
// options ask for it (+0x8c, vtable slot 29).
RT3_NOINLINE void Debug_PlaceTestModel() {
    Section_Enter(hook::Global<uint8_t*>(0xccbb20));
    Cursor_UseSystem(hook::Global<uint8_t*>(0xccbb20));
    Engine_Get1C(hook::Global<void*>(0x6d4024));
    char file[0x104], name[0x104];
    int32_t ok = OpenFileDialog(file, (const char*)0x5c9c18, (const char*)0x5c9c34);
    Cursor_PopWindowsMode(hook::Global<uint8_t*>(0xccbb20));
    if (!ok) return;
    BaseNameNoSuffix(name, file);
    uint32_t x, y, z;
    Obj_GetXYZ4(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, &x, &y, &z);
    if (hook::Global<uint8_t*>(0x62be14)) {
        ModelObj_Release(hook::Global<uint8_t*>(0x62be14));
        if (uint8_t* m = hook::Global<uint8_t*>(0x62be14)) {
            ModelObj_Destructor(m);
            ((void(__cdecl*)(void*))0x53b080)(m);
        }
        hook::Global<uint32_t>(0x62be14) = 0;
    }
    uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x26e);
    uint8_t* m = mem ? ModelObj_Construct(mem) : nullptr;
    hook::Global<uint8_t*>(0x62be14) = m;
    float fz = (float)((double)BitsF(z) / Fl(0x5c8700));
    float fx = (float)((double)BitsF(x) / Fl(0x5c8700));
    Building_Init(m, 0, name, name, fx, fz, 1, 0, 0, 0);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x8c)) {
        uint8_t* o = hook::Global<uint8_t*>(0x62be14);
        ((void(__fastcall*)(uint8_t*))(*(void***)o)[29])(o);
    }
}
RT3_REPLACE(0x42acf0, Debug_PlaceTestModel);

// 0x42ae20. Frees the key list +0xef.
void __fastcall Obj_FreeKeyList(void* o) {
    if (F<void*>(o, 0xef)) Free(F<void*>(o, 0xef));
    F<void*>(o, 0xef) = nullptr;
}
RT3_REPLACE(0x42ae20, Obj_FreeKeyList);

// 0x42ae50. Loads site record o from stream s, placed at (x, y) (+0xe7 / +0xe9): a
// 0x14-byte presence mask selects, for each of 53 slots, a word at +0 + 2k, a
// word at +0x6a + 2k and a dword at +0x103 + 4k (mask bits 3k + 0, +1, +2); then
// fixed fields +0xe6 (1 byte), +0xd4 / +0xd6 (words), +0xd8 (7 bytes), and a
// counted list (+0xeb) of 6-byte entries in a new buffer +0xef. +0xdf..+0xe5 are
// cleared.
RT3_NOINLINE void __fastcall Site_Load(uint8_t* o, int, int32_t s, int16_t x, int16_t y) {
    memset(o, 0, 0x6a);
    memset(o + 0x6a, 0, 0x6a);
    memset(o + 0x103, 0, 0x35 * 4);
    uint8_t mask[0x14];
    StreamRead(s, mask, 0x14, 0, 1);
    for (int32_t k = 0; k < 0x35; ++k) {
        int32_t b = k * 3;
        if (mask[b >> 3] & 1 << (b & 7)) StreamRead(s, o + k * 2, 2, 0, 1);
        if (mask[(b + 1) >> 3] & 1 << ((b + 1) & 7)) StreamRead(s, o + 0x6a + k * 2, 2, 0, 1);
        if (mask[(b + 2) >> 3] & 1 << ((b + 2) & 7)) StreamRead(s, o + 0x103 + k * 4, 4, 0, 1);
    }
    StreamRead(s, o + 0xe6, 1, 0, 1);
    StreamRead(s, o + 0xd4, 2, 0, 1);
    StreamRead(s, o + 0xd6, 2, 0, 1);
    StreamRead(s, o + 0xd8, 7, 0, 1);
    F<int32_t>(o, 0xdf) = 0;
    F<int16_t>(o, 0xe3) = 0;
    o[0xe5] = 0;
    F<int16_t>(o, 0xe7) = x;
    F<int16_t>(o, 0xe9) = y;
    int16_t n = 0;
    StreamRead(s, &n, 2, 0, 1);
    F<int32_t>(o, 0xeb) = n;
    F<void*>(o, 0xef) = ((void*(__cdecl*)(size_t))0x5a125d)((size_t)(n * 6));
    for (int32_t i = 0; i < F<int32_t>(o, 0xeb); ++i) StreamRead(s, F<uint8_t*>(o, 0xef) + i * 6, 6, 0, 1);
}
RT3_REPLACE(0x42ae50, Site_Load);

// 0x42b050. Classifies the 4x4 tiles of a site (+0xe7/+0xe9 in 4-tile units):
// flag 1 of +0xe6 when at least 8 are water, flag 2 when at least 2 are shore.
void __fastcall Site_ClassifyWater(uint8_t* s) {
    int32_t y0 = (int32_t)F<int16_t>(s, 0xe9) << 2;
    int32_t x0 = (int32_t)F<int16_t>(s, 0xe7) << 2;
    int32_t water = 0, shore = 0;
    for (int32_t y = y0; y <= y0 + 3; ++y)
        for (int32_t x = x0; x <= x0 + 3; ++x) {
            if (Terrain_KindIs1Or4(Map(), 0, x, y) || Terrain_KindIs3Or5(Map(), 0, x, y)) ++water;
            if (Terrain_KindIs2Or4Or5(Map(), 0, x, y)) ++shore;
        }
    if (water >= 8) s[0xe6] |= 1;
    else s[0xe6] &= 0xfe;
    if (shore >= 2) s[0xe6] |= 2;
    else s[0xe6] &= 0xfd;
}
RT3_REPLACE(0x42b050, Site_ClassifyWater);

// 0x42b140. Adds f (scaled by 0x5c9c40, rounded via +0x5c8628) to the u16
// level +0x6a[i], capped at 65000.
RT3_NOINLINE void __fastcall Obj_AddLevel(void* o, int, int32_t i, float f) {
    double v = (double)f * D(0x5c9c40) + D(0x5c8628);
    v = v + (double)F<uint16_t>(o, 0x6a + i * 2);
    int32_t n = (int32_t)v;
    if (n > 65000) n = 65000;
    F<uint16_t>(o, 0x6a + i * 2) = (uint16_t)n;
}
RT3_REPLACE(0x42b140, Obj_AddLevel);

// 0x42b190 / 0x42b230. Key list +0xef of 6-byte {u16 key, i32 value}
// entries, count +0xeb, reallocated on each change. Remove drops every
// entry with `key` but shrinks the count by one (QUIRK).
void __fastcall Obj_AddKey(void* o, int, uint16_t key, int32_t value) {
    int32_t n = F<int32_t>(o, 0xeb);
    uint8_t* p = (uint8_t*)Malloc((n + 1) * 6);
    if (n > 0) {
        const uint8_t* old = F<uint8_t*>(o, 0xef);
        for (int32_t k = 0; k < n * 6; ++k) p[k] = old[k];
    }
    F<uint16_t>(p, n * 6) = key;
    F<int32_t>(p, n * 6 + 2) = value;
    if (F<void*>(o, 0xef)) Free(F<void*>(o, 0xef));
    F<uint8_t*>(o, 0xef) = p;
    F<int32_t>(o, 0xeb) = n + 1;
}
RT3_REPLACE(0x42b190, Obj_AddKey);

void __fastcall Obj_RemoveKey(void* o, int, uint16_t key) {
    uint8_t* p = nullptr;
    int32_t n = F<int32_t>(o, 0xeb);
    if (n > 1) {
        p = (uint8_t*)Malloc((n - 1) * 6);
        uint8_t* d = p;
        for (int32_t k = 0; k < F<int32_t>(o, 0xeb); ++k) {
            uint8_t* e = F<uint8_t*>(o, 0xef) + k * 6;
            if (F<uint16_t>(e, 0) == key) continue;
            F<uint32_t>(d, 0) = F<uint32_t>(e, 0);
            F<uint16_t>(d, 4) = F<uint16_t>(e, 4);
            d += 6;
        }
    }
    Free(F<void*>(o, 0xef));
    F<uint8_t*>(o, 0xef) = p;
    F<int32_t>(o, 0xeb) = F<int32_t>(o, 0xeb) - 1;
}
RT3_REPLACE(0x42b230, Obj_RemoveKey);

// 0x42b2d0. True if the 6-byte record list (+0xef, count +0xeb) has key `k`.
RT3_NOINLINE int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k) {
    int32_t n = F<int32_t>(o, 0xeb);
    const uint8_t* r = F<uint8_t*>(o, 0xef);
    for (int32_t i = 0; i < n; ++i, r += 6)
        if (*(const uint16_t*)r == k) return 1;
    return 0;
}
RT3_REPLACE(0x42b2d0, Obj_HasKey);

// 0x42b310. Adds to float slot i (+0x103), not below zero.
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d) {
    float v = (float)((double)d + F<float>(o, 0x103 + i * 4));
    F<float>(o, 0x103 + i * 4) = v;
    if (v < *(const float*)0x5c8598) F<float>(o, 0x103 + i * 4) = 0.0f;
}
RT3_REPLACE(0x42b310, Obj_AddFloat103);

// 0x42b350. Bit i of the bitmap +0xdf (the masked byte, not 0/1).
int32_t __fastcall Obj_Bit(void* o, int, int32_t i) {
    return (1 << (i & 7)) & F<uint8_t>(o, 0xdf + (i >> 3));
}
RT3_REPLACE(0x42b350, Obj_Bit);

RT3_NOINLINE float __fastcall Obj_Level(void* o, int, int32_t i) {
    return (float)((double)F<uint16_t>(o, 0x6a + i * 2) / D(0x5c9c40));
}
RT3_REPLACE(0x42b380, Obj_Level);

// 0x42b3b0. Resets a cargo station record: position (+0xe7, +0xe9), flags,
// links, key list, and per cargo type the levels; stock cap +0[i] =
// cargo type +0x18 / 16.
void __fastcall Station_Reset(void* o, int, uint16_t x, uint16_t y) {
    F<uint16_t>(o, 0xe7) = x;
    F<uint16_t>(o, 0xe9) = y;
    F<uint8_t>(o, 0xe6) = 0;
    for (int k = 0; k < 7; ++k) F<uint8_t>(o, 0xd8 + k) = 0;
    for (int k = 0; k < 7; ++k) F<uint8_t>(o, 0xdf + k) = 0;
    F<uint16_t>(o, 0xd4) = 0;
    F<uint16_t>(o, 0xd6) = 0;
    F<int32_t>(o, 0xeb) = 0;
    F<int32_t>(o, 0xef) = 0;
    F<int32_t>(o, 0xf3) = 0;
    F<int32_t>(o, 0xf7) = 0;
    F<int32_t>(o, 0xfb) = 0;
    F<int32_t>(o, 0xff) = 0;
    for (int32_t i = 0; i < 0x34; ++i) {
        F<uint16_t>(o, 0x6a + i * 2) = 0;
        F<int32_t>(o, 0x103 + i * 4) = 0;
        F<uint16_t>(o, i * 2) = 0;
        if (i == 0 || !Collection_IsValid(g_cargoTypes, 0, i)) continue;
        Collection_Get(g_cargoTypes, 0, i); // the original reassigns +0x5a to itself here
        F<int32_t>(o, 0x103 + i * 4) = 0;
        float cap = F<float>(Collection_Get(g_cargoTypes, 0, i), 0x18);
        F<uint16_t>(o, i * 2) = (uint16_t)((uint32_t)(int32_t)cap >> 4);
    }
}
RT3_REPLACE(0x42b3b0, Station_Reset);

// 0x42b4a0. Saves a region's cargo economy compactly: a 3-bit-per-cargo mask of
// which of supply (+0 u16), demand (+0x6a u16) and price (+0x103 float, != 0) are set
// for available cargos (0x62ba8c +0x16e), then those values, then the trailer
// (+0xe6, +0xd4, +0xd6, +0xd8..), the entry count (+0xeb, as u16) and its 6-byte
// entries (+0xef).
void __fastcall Region_SaveCargo(uint8_t* r, int, Stream st) {
    uint8_t mask[0x14] = {};
    uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
    for (int32_t k = 0; k < 0x35; ++k) {
        int32_t avail = F<int32_t>(cargos, 0x16e + k * 4);
        if (F<uint16_t>(r, k * 2) && avail) mask[(k * 3) >> 3] |= (uint8_t)(1 << ((k * 3) & 7));
        if (F<uint16_t>(r, 0x6a + k * 2) && avail) mask[(k * 3 + 1) >> 3] |= (uint8_t)(1 << ((k * 3 + 1) & 7));
        if (!(F<float>(r, 0x103 + k * 4) == 0.0f) && avail) mask[(k * 3 + 2) >> 3] |= (uint8_t)(1 << ((k * 3 + 2) & 7));
    }
    StreamWrite(st, mask, 0x14, 0);
    for (int32_t k = 0; k < 0x35; ++k) {
        if (!F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x16e + k * 4)) continue;
        if (F<uint16_t>(r, k * 2)) StreamWrite(st, r + k * 2, 2, 0);
        if (F<uint16_t>(r, 0x6a + k * 2)) StreamWrite(st, r + 0x6a + k * 2, 2, 0);
        if (!(F<float>(r, 0x103 + k * 4) == 0.0f)) StreamWrite(st, r + 0x103 + k * 4, 4, 0);
    }
    StreamWrite(st, r + 0xe6, 1, 0);
    StreamWrite(st, r + 0xd4, 2, 0);
    StreamWrite(st, r + 0xd6, 2, 0);
    StreamWrite(st, r + 0xd8, 7, 0);
    int32_t n = F<uint16_t>(r, 0xeb);
    StreamWrite(st, &n, 2, 0);
    for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) StreamWrite(st, F<uint8_t*>(r, 0xef) + i * 6, 6, 0);
}
RT3_REPLACE(0x42b4a0, Region_SaveCargo);

// 0x42bbb0 / 0x42bbf0. Site chain (u16 head +0xd4, next +0x3c): push id at
// the front / unlink id (gives up after 1000 links).
void __fastcall Chain_Push(void* o, int, uint16_t id) {
    Obj_Set3c(Collection_Get(g_table6cec20, 0, id), 0, F<uint16_t>(o, 0xd4));
    F<uint16_t>(o, 0xd4) = id;
}
RT3_REPLACE(0x42bbb0, Chain_Push);

RT3_NOINLINE void __fastcall Chain_Remove(void* o, int, uint16_t id) {
    uint32_t prev = 0, cur = F<uint16_t>(o, 0xd4);
    for (int32_t n = 0; cur != id; ) {
        if (n++ > 1000) return;
        prev = cur;
        cur = F<uint16_t>(Collection_Get(g_table6cec20, 0, cur), 0x3c);
    }
    uint16_t next = F<uint16_t>(Collection_Get(g_table6cec20, 0, id), 0x3c);
    if (prev) Obj_Set3c(Collection_Get(g_table6cec20, 0, prev), 0, next);
    else F<uint16_t>(o, 0xd4) = next;
}
RT3_REPLACE(0x42bbf0, Chain_Remove);

// 0x42bc70. Flag 8 of +0xe6: own bit 0 clear, or any in-map neighbour cell
// (8 directions) with bit 0 of its +0xe6 clear.
void __fastcall Station_UpdateEdgeFlag(uint8_t* o) {
    uint8_t flags = o[0xe6];
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t x = F<int16_t>(o, 0xe7), y = F<int16_t>(o, 0xe9);
    int32_t found = ~flags & 1;
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (int d = 0; d < 8; ++d) {
        int32_t nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= F<int32_t>(map, 0x2145) || ny < 0 || ny >= F<int32_t>(map, 0x2149)) continue;
        uint8_t* cell = F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * ny + nx) * 0x1d7;
        if (!(cell[0xe6] & 1)) found = 1;
    }
    o[0xe6] = found ? (uint8_t)(flags | 8) : (uint8_t)(flags & 0xf7);
}
RT3_REPLACE(0x42bc70, Station_UpdateEdgeFlag);

// 0x42be20. Sum of +0x103[i] over the available cargo types (i = 0..52).
RT3_NOINLINE float __fastcall Station_TotalAvailable(void* o) {
    const uint8_t* ct = (const uint8_t*)g_cargoTypes;
    float sum = 0.0f;
    for (int32_t i = 0; i < 0x35; ++i)
        if (F<int32_t>((void*)ct, 0x16e + i * 4)) sum = (float)((double)sum + F<float>(o, 0x103 + i * 4));
    return sum;
}
RT3_REPLACE(0x42be20, Station_TotalAvailable);

// 0x42bf20. Total output of cargo by the city's buildings (chain from +0xd6, next +0x2a2).
RT3_NOINLINE float __fastcall City_CargoOutput(uint8_t* c, int, int32_t cargo) {
    float sum = 0.0f;
    for (uint32_t id = F<uint16_t>(c, 0xd6); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
        sum = (float)(Building_CargoOutput(b, 0, cargo) + sum);
        id = F<uint16_t>(b, 0x2a2);
    }
    return sum;
}
RT3_REPLACE(0x42bf20, City_CargoOutput);

// 0x42bf80. Moves part of cargo i's stock (+0x103) from this city to `dst`: a fixed
// share (0x5c9c50), reduced (v1.03+) when nearly empty and for over-capacity stock
// (+0x6a); the waiting count (+0) shrinks by `amount` (taken from dst's count).
void __fastcall City_TransferCargo(uint8_t* c, int, uint8_t* dst, int32_t i, int32_t amount) {
    float stock = F<float>(c, 0x103 + i * 4);
    float x = (float)((double)stock * *(const double*)0x5c9c50);
    if (c[0xdf + (i >> 3)] & (1 << (i & 7))) {
        if ((double)stock - x < *(const double*)0x5c9c48 && Game_EffectiveVersion() >= 0x67)
            x = (float)((double)x * *(const double*)0x5c8758);
    }
    float cap = (float)((double)F<uint16_t>(c, 0x6a + i * 2) / *(const double*)0x5c9c40);
    if (cap > F<float>(c, 0x103 + i * 4)) x = (float)((double)x * *(const double*)0x5c88e8);
    F<float>(c, 0x103 + i * 4) = (float)((double)F<float>(c, 0x103 + i * 4) - x);
    F<float>(dst, 0x103 + i * 4) = (float)((double)x + F<float>(dst, 0x103 + i * 4));
    uint16_t n = F<uint16_t>(dst, i * 2);
    F<uint16_t>(c, i * 2) = (int32_t)n > amount ? (uint16_t)(n - amount) : 0;
}
RT3_REPLACE(0x42bf80, City_TransferCargo);

}  // namespace rt3
