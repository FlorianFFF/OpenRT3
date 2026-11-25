// Vehicles.cpp - RT3.exe 0x40a000-0x40c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Trains_Advance(Collection* c, int, float dt);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);

// A collection of scene objects loaded from a save between the frame tags
// `t0` / `t1` / `t2`: the records get the vtable of a freshly constructed
// object of their class, then load their own extra data (vtbl +0x40).
template <typename Ctor, typename Dtor>
static int32_t LoadSceneObjects(Collection* t, Stream s, int32_t recordSize, int32_t t0, int32_t t1, int32_t t2, Ctor ctor, Dtor dtor) {
    int32_t n = StreamSkip4(s, t0, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, t1, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        uint8_t tmp[0x400];
        ctor(tmp);
        F<uint32_t>(r, 0) = F<uint32_t>(tmp, 0);
        n += ((int32_t(__thiscall*)(uint8_t*, Stream))(*(void***)tmp)[0x40 / 4])(r, s);
        dtor(tmp);
    }
    return StreamSkip4(s, t2, 0, 0) + n;
}

struct Collection;
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
uint8_t* __fastcall NamedObj456900_Construct(uint8_t* o);
void __fastcall NamedObj456900_Destructor(uint8_t* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
const char* __fastcall GetString(int32_t id);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Fatal_Exit(const char* msg);

// One AirplaneTypes.txt record (0x5c bytes).
struct PlaneRec {
    char name[0x20];
    int32_t i[3];
    float f[12];
};

void __fastcall Airplanes_LoadTypes(uint8_t* o);

uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist);

uint32_t __fastcall Sound_ListenerIdle(uint8_t* s);
void __stdcall AmbientSound_Ensure(int32_t i);
void __fastcall Sound_ListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
void __fastcall Sound_ListenerPos(uint8_t* s, int, float x, float y, float z);
void* __fastcall Engine_Renderer(void* e);
void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
int32_t ClockMs();
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
float __cdecl Sky_NightFactor();
float SceneClock_MinutesSinceDawn();
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __fastcall Table_CountField3f(Collection* t);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t IsMultiplayer();
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
void __fastcall World_UpdateSpeedScale(uint8_t* o);
void World_ProcessMarginCalls();
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
void __fastcall Collection_Destruct(Collection* c);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void __fastcall Collection_Clear(Collection* c);
void __fastcall Weather_ApplyIntensity(uint8_t* w);
void RunDebugValidation();
void PushSeed();
void __fastcall SetSeed(int32_t seed);
void PopSeed();
uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
int32_t GetSeed();
void __fastcall Vehicles_Destruct(Collection* t);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall Nop433e90(void*);
void __fastcall Company_AnnualReview(uint8_t* c);
void __fastcall Nop476090(void*);
void __fastcall Nop4a7320(void*);
void __fastcall Nop40ca60(void*);
void __fastcall Cities_Grow(Collection* cities);
void __fastcall World_RollStats(uint8_t* w);
void __fastcall Industries_BuildCargoLists(Collection* c);
void World_UpdateEraFactor();
void __fastcall Company_YearEnd(uint8_t* c);
void __fastcall Player_RecordWealth(uint8_t* p);
void __fastcall Train_RollStats(uint8_t* t);
void __fastcall Building_RollStats(uint8_t* o);
void __fastcall Site_RollStats(uint8_t* o);
void __fastcall Station_RollStats(uint8_t* o);
void __fastcall Obj_YearRollover(uint8_t* o);
void __fastcall CargoTypes_Recount(Collection* t);
void __fastcall Lots_RebuildDemand(Collection* c);
void __fastcall Map_RefreshCells(uint8_t* m);
void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all);
void Game_PauseTools();
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall Vehicles_MaybeSpawn(Collection* t, int, float chance);
uint32_t __fastcall Vehicles_MaybeSpawnFlight(Collection* t, int, float chance, int32_t force);
RT3_NOINLINE void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
void Attach_TickAll();
void __fastcall Industries_ProduceQuarter(Collection* t);
void __fastcall Loads_Age(Collection* t);
void __fastcall Train_YearlyUpkeep(uint8_t* t);
void __fastcall World_EndOfMonth(uint8_t* w);
void __fastcall World_EconomyTick(uint8_t* w, int, int32_t force);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
void __fastcall Obj_Nop423fc0(void*);
void __fastcall Nop476080(void*);
void __fastcall Train_SmoothSpeed(uint8_t* t);
RT3_NOINLINE void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all);
void __fastcall Station_UpdateRoutes(uint8_t* o);
void __fastcall Cities_RandomBoom(Collection* c);
void __fastcall World_YearEvents(uint8_t* w);
void __fastcall CarTypes_AnnounceNew(Collection* c);
void __fastcall Company_MonthEnd(uint8_t* c);
void __fastcall Company_UpdateCreditRate(uint8_t* o);
void Net_SendPendingRecords();
void __fastcall Table_DecayWear(Collection* t);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t GetSeed();
void __fastcall Car_ApplyPriceFactor(uint8_t* car);
void __fastcall Net_WeeklyCheck(uint8_t* s);
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Map_MonthlyEconomy(uint8_t* m);
uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d);
RT3_NOINLINE uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
int32_t __stdcall DateDiffMinutes(GameDate a, GameDate b);
RT3_NOINLINE void __fastcall Industries_BuildCargoLists(Collection* c);
RT3_NOINLINE void __fastcall World_UpdateSpeedScale(uint8_t* o);
RT3_NOINLINE void __fastcall CargoTypes_Recount(Collection* t);
RT3_NOINLINE void __fastcall Lots_RebuildDemand(Collection* c);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall AI_Tick(uint8_t* w);
void __fastcall World_RefreshTime(uint8_t* w);
RT3_NOINLINE void __fastcall World_YearEnd(uint8_t* w);
RT3_NOINLINE void __fastcall World_EndOfMonth(uint8_t* w);
RT3_NOINLINE void __fastcall World_EndOfWeek(uint8_t* w);
RT3_NOINLINE void __fastcall World_EndOfDay(uint8_t* w);
RT3_NOINLINE void __fastcall World_AdvanceMinutes(uint8_t* w, int, int32_t minutes);
RT3_NOINLINE void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);
void __fastcall Airplanes_LoadTypes(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall Airplanes_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Vehicles_Remove(Collection* t, int, uint32_t id);
RT3_NOINLINE uint8_t* __fastcall Vehicles_Add(Collection* t, int, int32_t type);
RT3_NOINLINE uint8_t* __fastcall Vehicles_AddNamed(Collection* t, int, const char* name);
RT3_NOINLINE void __fastcall Vehicles_DriveAll(Collection* c, int, uint32_t dt);
void __fastcall Vehicles_MaybeSpawn(Collection* t, int, float chance);
RT3_NOINLINE uint32_t __fastcall Vehicles_MaybeSpawnFlight(Collection* t, int, float chance, int32_t force);
RT3_NOINLINE int32_t __fastcall NamedObjs456900_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveWithRecords2af9(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Vehicles_Destruct(Collection* t);
void __stdcall AmbientSound_Ensure(int32_t i);
void __fastcall Ambient_StopSlots();
RT3_NOINLINE void __fastcall Ambient_Start(void*);
RT3_NOINLINE void Ambient_Update();
RT3_NOINLINE void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall Company_AnnualReview(uint8_t* c);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void World_ProcessMarginCalls();
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void Game_PauseTools();
RT3_NOINLINE void Options_Restore();
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall Terrain_AgeCorners(uint8_t* t);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
void __fastcall NamedObj456900_Destructor(uint8_t* o);
uint8_t* __fastcall NamedObj456900_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Vehicle_Init(uint8_t* v, int, uint32_t id, const uint32_t* type, int32_t atEdge, float x, float y, float z, float heading);
RT3_NOINLINE void __fastcall CameraObj_Teardown(uint8_t* o);
RT3_NOINLINE bool __fastcall Vehicle_DriveUpdate(uint8_t* v, int, float dt);
RT3_NOINLINE void __fastcall Net_RunFrameMessages(uint8_t* s);
RT3_NOINLINE void __fastcall Stations_CountTargets(Collection* stations);

// Save of a collection whose records save themselves (virtual slot 17), between three tags.
static void SaveWithRecords(Collection* t, Stream s, int32_t recordSize, uint32_t tag) {
    StreamWriteU32(s, tag);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, tag + 1);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, tag + 2);
}

static inline uint8_t* SoundSys() { return g_sound; }

static inline double D(uint32_t a) { return *(const double*)a; }

// Formats string id with one argument and exits with it.
static void FatalFormat(int32_t id, const char* arg) {
    char msg[0x12c];
    FormatText(msg, GetString(id), arg, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Fatal_Exit(msg);
}

static inline uint8_t* Snd() { return g_sound; }

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

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// div() of the exe's CRT: quotient in the low, remainder in the high half.
static inline int32_t Prod_Rem(int32_t a, int32_t b) { return (int32_t)(crt::div(a, b) >> 32); }

namespace {
template <typename Fn> void YearEach(uint32_t global, Fn fn) {
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(global)); ++i) {
        Collection* c = hook::Global<Collection*>(global);
        fn((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)));
    }
}
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
inline float RtBits(uint32_t v) { return *(const float*)&v; }
inline uint8_t* RtGet(uint32_t global, int32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(global), 0, (uint32_t)id); }
inline uint8_t* RtMap() { return g_map; }
}

// 0x40a150. Re-applies the world clock (refreshes the derived times).
void __fastcall World_RefreshTime(uint8_t* w) { World_SetTime(w, 0, F<int32_t>(w, 0x15)); }
RT3_REPLACE(0x40a150, World_RefreshTime);

 // namespace

// 0x40a160. World w closes the year: the yearly reviews of the companies,
// players, trains and industries and the city growth, trigger-1 events;
// unless the game is over or scripted (0x6cec74+0xf8, +0x46c38, +0x4aeb,
// sandbox) it marks the year review due (+0x4d; in the special scenario
// mode only two weeks after +0x4c88). Then the clock advances a minute, the
// stats roll, cargo lists, era and speed factors refresh, the inflation
// rate (+0xbde) grows by 0x6cec78+0xbe6 for the decades played (none from
// 150, x0.4 from 100, x0.75 from 70), everything rolls its stats, trigger-0
// events fire, and cargo counts, demand and the map cells refresh.
RT3_NOINLINE void __fastcall World_YearEnd(uint8_t* w) {
    Nop433e90(w);
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i)
        Company_AnnualReview((uint8_t*)Table_NthField3f(g_companies, 0, i));
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i) Nop476090(Table_NthField4(g_players, 0, i));
    YearEach(0x6cfcbc, [](uint8_t* o) { Nop4a7320(o); });
    YearEach(0x62b26c, [](uint8_t* o) { Nop40ca60(o); });
    Cities_Grow(g_cities);
    Events_Fire(g_events, 0, 1, nullptr, 0);
    uint8_t* world = g_world;
    if (!F<int32_t>(g_app, 0xf8) && !F<int32_t>(w, 0x46c38) && !F<int32_t>(world, 0x4aeb) && !g_newGame[0x82]) {
        if (!F<int32_t>(w, 0x4a73)) {
            F<int32_t>(w, 0x4d) = 1;
        } else {
            int32_t d = F<int32_t>(world, 0x15) - F<int32_t>(world, 0x4c88);
            if (d < 0) d = -d;
            if (!((double)d < *(const double*)0x5c89b0)) F<int32_t>(w, 0x4d) = 1;
        }
    }
    World_SetTime(w, 0, F<int32_t>(w, 0x15) + 1);
    World_RollStats(w);
    Industries_BuildCargoLists(g_62b268);
    World_UpdateEraFactor();
    World_UpdateSpeedScale(w);
    int32_t decades = ((int32_t)F<uint16_t>(w, 0xd) - (int32_t)F<uint16_t>(w, 5)) / 10;
    float rate = F<float>(g_world, (decades < 4 ? decades : 4) * 4 + 0xbe6);
    if (decades >= 0x96) rate = 0.0f;
    else if (decades >= 0x64) rate = (float)((double)rate * *(const double*)0x5c88e8);
    else if (decades >= 0x46) rate = (float)((double)rate * *(const double*)0x5c8750);
    F<float>(w, 0xbde) = (float)(((double)rate + *(const double*)0x5c8770) * F<float>(w, 0xbde));
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i)
        Company_YearEnd((uint8_t*)Table_NthField3f(g_companies, 0, i));
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i)
        Player_RecordWealth((uint8_t*)Table_NthField4(g_players, 0, i));
    YearEach(0x6cfcbc, [](uint8_t* o) { Train_RollStats(o); });
    YearEach(0x62b26c, [](uint8_t* o) { Building_RollStats(o); });
    YearEach(0x6cec20, [](uint8_t* o) { Site_RollStats(o); });
    YearEach(0x6cfc9c, [](uint8_t* o) { Station_RollStats(o); });
    YearEach(0x62bae0, [](uint8_t* o) { Obj_YearRollover(o); });
    Events_Fire(g_events, 0, 0, nullptr, 0);
    CargoTypes_Recount(g_cargoTypes);
    Lots_RebuildDemand(g_6ada90);
    Map_RefreshCells(g_map);
}
RT3_REPLACE(0x40a160, World_YearEnd);

// 0x40a590. World w's month end: the economy ticks in February and August;
// companies (two no-op hooks), trains smooth their speed, cargo averages
// and station targets / routes update and trigger 3 fires; cities may boom
// (not in March, December, February or August); March has the year's
// events, December the year end (else the clock +0x15 steps on), June new
// car types; companies close the month, players their credit and trigger 2
// fires, then margin calls. Unless paused for it (editor +0x90, world
// +0x4aeb, auto companies with +0x24b) the cities update by 1/12 (in
// multiplayer only with 0x6d40dc, seeded apart and the pending records
// sent). A single-player game autosaves (World_Save, flags 2) by the option
// 0x6cec74 +0xf4: 1 quarterly, 2 half-yearly, 3 yearly, 4 every 2nd and 5
// every 5th year (by the month this started in). Then the AIs act, track
// wear decays and the overview redraws.
RT3_NOINLINE void __fastcall World_EndOfMonth(uint8_t* w) {
    const int32_t month = w[0xf];
    if (w[0xf] == 2 || w[0xf] == 8) World_EconomyTick(w, 0, -1);
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i)
        Obj_Nop423fc0(Table_NthField3f(g_companies, 0, i));
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i)
        Nop476080(Table_NthField4(g_players, 0, i));
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* trains = g_trains;
        Train_SmoothSpeed((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)));
    }
    CargoTypes_UpdateAverages(g_cargoTypes, 0, 0);
    Stations_CountTargets(g_stations);
    for (int32_t i = 0; i < Collection_Count(g_stations); ++i) {
        Collection* stations = g_stations;
        Station_UpdateRoutes((uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, (uint32_t)i, 0)));
    }
    Events_Fire(g_events, 0, 3, nullptr, 0);
    {
        const uint8_t m = w[0xf];
        if (m != 3 && m != 0xc && m != 2 && m != 8) Cities_RandomBoom(g_cities);
    }
    if (w[0xf] == 3) World_YearEvents(w);
    if (w[0xf] == 0xc) World_YearEnd(w);
    else World_SetTime(w, 0, F<int32_t>(w, 0x15) + 1);
    if (w[0xf] == 6) CarTypes_AnnounceNew(g_carTypes);
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i)
        Company_MonthEnd((uint8_t*)Table_NthField3f(g_companies, 0, i));
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i)
        Company_UpdateCreditRate((uint8_t*)Table_NthField4(g_players, 0, i));
    Events_Fire(g_events, 0, 2, nullptr, 0);
    World_ProcessMarginCalls();
    {
        uint8_t* ed = g_app;
        if (!F<int32_t>(ed, 0x90) && !F<int32_t>(g_world, 0x4aeb) &&
            (!g_newGame[0x82] || !F<int32_t>(ed, 0x24b))) {
            if (!IsMultiplayer()) {
                Cities_UpdateAll(g_cities, 0, 0x3daaaaab, 0);
            } else if (g_6d40dc) {
                PushSeed();
                Cities_UpdateAll(g_cities, 0, 0x3daaaaab, 0);
                Net_SendPendingRecords();
                PopSeed();
            }
        }
    }
    if (!IsMultiplayer() && !F<int32_t>(w, 0x46c38)) {
        const int32_t every = F<int32_t>(g_app, 0xf4);
        bool save = false;
        if (every < 3) {
            if (every && (month == 6 || month == 0xc || (every == 1 && (month == 3 || month == 9)))) save = !IsMultiplayer();
        } else if (month == 0xc) {
            if (every == 3) save = true;
            else if (every == 4 && Prod_Rem(F<uint16_t>(w, 0xd), 2) == 0) save = true;
            else if (F<int32_t>(g_app, 0xf4) == 5) save = Prod_Rem(F<uint16_t>(w, 0xd), 5) == 0;
        }
        if (save) World_Save(w, 0, nullptr, 2, nullptr);
    }
    AI_Tick(w);
    Table_DecayWear(g_tracks);
    Overview_Redraw(g_map, 0, nullptr);
}
RT3_REPLACE(0x40a590, World_EndOfMonth);

// 0x40a910. World w's week end (every 7th day): industries produce, trigger 5 fires, on
// day 28 (+0x11) 0x40a590 runs (else the clock +0x15 steps on by one), loads
// age, every train pays its upkeep and trigger 4 fires; debug validation
// before and after.
RT3_NOINLINE void __fastcall World_EndOfWeek(uint8_t* w) {
    RunDebugValidation();
    Industries_ProduceQuarter(g_buildings);
    Events_Fire(g_events, 0, 5, nullptr, 0);
    if (w[0x11] == 0x1c) World_EndOfMonth(w);
    else World_SetTime(w, 0, F<int32_t>(w, 0x15) + 1);
    Loads_Age(g_6ada90);
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* trains = g_trains;
        Train_YearlyUpkeep((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)));
    }
    Events_Fire(g_events, 0, 4, nullptr, 0);
    RunDebugValidation();
}
RT3_REPLACE(0x40a910, World_EndOfWeek);

// 0x40a9c0. World w's day end: in a running multiplayer game (+0x66ae) the
// options are saved and the shared seed (0x5f2b6c) set; terrain corners age,
// every 7th day is a week end (else the clock steps on), car prices update,
// and multiplayer runs its frame messages and weekly check and restores the
// options. Unless every peer runs version 1.02, the seed then moves on by
// the clock and day (world +0x15 + seed + day x 37).
RT3_NOINLINE void __fastcall World_EndOfDay(uint8_t* w) {
    if (IsMultiplayer() && F<int32_t>(w, 0x66ae)) {
        Game_PauseTools();
        SetSeed(g_5f2b6c);
    }
    Terrain_AgeCorners(g_map);
    if (w[0x11] % 7 == 0) World_EndOfWeek(w);
    else World_SetTime(w, 0, F<int32_t>(w, 0x15) + 1);
    for (int32_t i = 0; i < Collection_Count(g_62ba84); ++i) {
        Collection* cars = g_62ba84;
        Car_ApplyPriceFactor((uint8_t*)Collection_Get(cars, 0, Collection_IdAt(cars, 0, (uint32_t)i, 0)));
    }
    if (IsMultiplayer() && F<int32_t>(w, 0x66ae)) {
        Net_RunFrameMessages(g_session);
        Net_WeeklyCheck(g_session);
        Options_Restore();
    }
    if (IsMultiplayer() && Net_AllHaveVersion(RtBits(0x3f828f5c))) return;
    uint8_t* const world = g_world;
    const int32_t clock = F<int32_t>(world, 0x15);
    SetSeed(world[0x11] * 0x25 + clock + GetSeed());
}
RT3_REPLACE(0x40a9c0, World_EndOfDay);

// 0x40aae0 (RET 4). World w moves on by `minutes`: trains (0x4b24a0),
// vehicle spawns, ground and air (0x62b244), and their driving advance by
// that many minutes, then the clock (+0x15), attachments and the weather.
// QUIRK: Vehicles_DriveAll gets the minutes as float bits.
RT3_NOINLINE void __fastcall World_AdvanceMinutes(uint8_t* w, int, int32_t minutes) {
    const float f = (float)minutes;
    Trains_Advance(g_trains, 0, f);
    Vehicles_MaybeSpawn(g_62b244, 0, f);
    Vehicles_MaybeSpawnFlight(g_62b244, 0, f, 0);
    Vehicles_DriveAll(g_62b244, 0, Bits(f));
    World_SetTime(w, 0, F<int32_t>(w, 0x15) + minutes);
    Attach_TickAll();
    Weather_ApplyIntensity(w);
}
RT3_REPLACE(0x40aae0, World_AdvanceMinutes);

// 0x40ab50 (RET 8). World w advances its clock by `minutes` (not while an
// outer advance runs, 0x62b240, unless `nested`). Every quarter-day boundary
// crossed on the way (year, month, day or quarter of the target later) is
// stepped: the minutes up to it (a nested advance), the map's quarter
// economy, then the day end on the 4th quarter or one clock step, until the
// target's quarter; the rest advances directly (World_AdvanceMinutes).
RT3_NOINLINE void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested) {
    if (!nested && g_62b240) return;
    ++g_62b240;
    int32_t left = minutes;
    const uint64_t to = DateAddMinutes(minutes, *(GameDate*)(w + 0xd));
    GameDate target;
    memcpy(&target, &to, sizeof target);
    auto now = [&] { return (const GameDate*)(w + 0xd); };
    auto step = [&] {
        Map_MonthlyEconomy(g_map);
        if (w[0x14] == 4) World_EndOfDay(w);
        else World_SetTime(w, 0, F<int32_t>(w, 0x15) + 1);
    };
    if (target.year > now()->year || target.month > now()->month || target.day > now()->day || target.quarter > now()->quarter) {
        const uint64_t endv = MakeDate(now()->year, now()->month, now()->day, 0x17, 0x3b);
        GameDate end;
        memcpy(&end, &endv, sizeof end);
        const int32_t first = DateDiffMinutes(end, *now()) % 0x168;
        World_Advance(w, 0, first, 1);
        step();
        left += -1 - first;
        while (now()->year != target.year || now()->month != target.month || now()->day != target.day || now()->quarter != target.quarter) {
            World_Advance(w, 0, 0x167, 1);
            step();
            left -= 0x168;
        }
    }
    if (left > 0) World_AdvanceMinutes(w, 0, left);
    --g_62b240;
}
RT3_REPLACE(0x40ab50, World_Advance);

// 0x40acb0. Loads up to 32 airplane types (+0x90, 0x5c each; count +0x88) from
// data\Other\AirplaneTypes.txt (';' lines are comments): name, 3 ints, 11 floats;
// field +0x3c = 1 / +0x38 (at least 0.1). +0x8c = the earliest year (+0x24) of
// the types without flag +0x20.
RT3_NOINLINE void __fastcall Airplanes_LoadTypes(uint8_t* o) {
    char path[0x100];
    FormatText(path, (const char*)0x5c89ec, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    auto fgetc_ = (int(__cdecl*)(FILE*))0x5a1791;
    FILE* f = ((FILE*)crt::fopen(path, (const char*)0x5c89e8));
    if (!f) FatalFormat(0xac, path);
    F<int32_t>(o, 0x8c) = 9999;
    F<int32_t>(o, 0x88) = 0;
    auto eof = [&]() { return (F<uint8_t>(f, 0xc) & 0x10) != 0; };
    while (!eof()) {
        char line[0x400];
        crt::fscanf(f, (const char*)0x5c89e4, line);
        if (line[0] == ';') {
            if (fgetc_(f) != '\n')
                while (!eof() && fgetc_(f) != '\n') {}
            continue;
        }
        PlaneRec r;
        char* d = r.name;
        for (const char* s = line; (*d++ = *s++) != 0;) {}
        crt::fscanf(f, (const char*)0x5c89b8, &r.i[0], &r.i[1], &r.i[2], &r.f[0], &r.f[1], &r.f[2], &r.f[7], &r.f[5],
                &r.f[6], &r.f[3], &r.f[8], &r.f[9], &r.f[10], &r.f[11]);
        if ((double)r.f[3] < D(0x5c86b8)) *(uint32_t*)&r.f[3] = 0x3dcccccd;
        r.f[4] = (float)(D(0x5c8770) / r.f[3]);
        int32_t n = F<int32_t>(o, 0x88);
        memcpy(o + 0x90 + n * 0x5c, &r, 0x5c);
        uint8_t* e = o + n * 0x5c;
        if (F<int32_t>(o, 0x8c) > F<int32_t>(e, 0xb4) && !F<int32_t>(e, 0xb0)) F<int32_t>(o, 0x8c) = F<int32_t>(e, 0xb4);
        F<int32_t>(o, 0x88) = ++n;
        if (n >= 0x20) break;
    }
    crt::fclose(f);
}
RT3_REPLACE(0x40acb0, Airplanes_LoadTypes);

// 0x40aeb0. Constructs the airplane type table (a collection, vtable 0x5c8a0c) and
// loads it.
RT3_NOINLINE uint8_t* __fastcall Airplanes_Construct(uint8_t* o) {
    Collection_SetVtbl((Collection*)o);
    F<uint32_t>(o, 0) = 0x5c8a0c;
    Collection_Init((Collection*)o, 0, 0, 0, 5, 10, 0, 0, 0);
    for (int k = 0; k < 0x2e0; ++k) F<uint32_t>(o, 0x90 + k * 4) = 0;
    F<int32_t>(o, 0x8c) = 9999;
    F<int32_t>(o, 0x88) = 0;
    Airplanes_LoadTypes(o);
    return o;
}
RT3_REPLACE(0x40aeb0, Airplanes_Construct);

// 0x40af00. Vehicle table t: removes vehicle `id` - one fewer of its kind
// counted in the world (+0x4c9e when +0x261, else +0x4c9a; not below 0),
// unfollowed and torn down, then released.
RT3_NOINLINE void __fastcall Vehicles_Remove(Collection* t, int, uint32_t id) {
    uint8_t* o = (uint8_t*)Collection_Get(t, 0, id);
    uint8_t* w = g_world;
    if (o[0x261]) --F<int32_t>(w, 0x4c9e);
    else --F<int32_t>(w, 0x4c9a);
    w = g_world;
    if (F<int32_t>(w, 0x4c9e) < 0) F<int32_t>(w, 0x4c9e) = 0;
    if (F<int32_t>(w, 0x4c9a) < 0) F<int32_t>(w, 0x4c9a) = 0;
    if (o == F<uint8_t*>(g_62be68, 0x60)) World_SetFollow(g_world, 0, 0, nullptr);
    CameraObj_Teardown(o);
    Collection_Release(t, 0, id);
}
RT3_REPLACE(0x40af00, Vehicles_Remove);

// 0x40afa0. Adds a vehicle of type `type` (one of the 0x5c-byte type records
// at +0x90) to table t, entering at a random map edge, and returns it.
RT3_NOINLINE uint8_t* __fastcall Vehicles_Add(Collection* t, int, int32_t type) {
    uint8_t tmp[0x398];
    NamedObj456900_Construct(tmp);
    uint32_t id = Collection_Allocate(t, 0, tmp, 0x396, 0, 0);
    uint8_t* v = (uint8_t*)Collection_Get(t, 0, id);
    Vehicle_Init(v, 0, id, (const uint32_t*)((uint8_t*)t + 0x90 + type * 0x5c), 1, 0.0f, 0.0f, 0.0f, 0.0f);
    NamedObj456900_Destructor(tmp);
    return v;
}
RT3_REPLACE(0x40afa0, Vehicles_Add);

// 0x40b010. Adds a vehicle of the type (of the 32 at +0x90) named `name`,
// case-insensitively; null when there is none.
RT3_NOINLINE uint8_t* __fastcall Vehicles_AddNamed(Collection* t, int, const char* name) {
    for (int32_t k = 0; k < 0x20; ++k)
        if (!crt::stricmp((const char*)t + 0x90 + k * 0x5c, name)) return k == -1 ? nullptr : Vehicles_Add(t, 0, k);
    return nullptr;
}
RT3_REPLACE(0x40b010, Vehicles_AddNamed);

// 0x40b060. Drives every vehicle of collection c (last first) by dt and
// removes (vtable slot 2) those whose move failed.
RT3_NOINLINE void __fastcall Vehicles_DriveAll(Collection* c, int, uint32_t dt) {
    for (int32_t i = Collection_Count(c) - 1; i >= 0; --i) {
        uint8_t* v = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (((bool(__fastcall*)(uint8_t*, int, uint32_t))&Vehicle_DriveUpdate)(v, 0, dt)) continue;
        void* vt = *(void**)c;
        ((void(__fastcall*)(Collection*, int, uint32_t))((void**)vt)[2])(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
    }
}
RT3_REPLACE(0x40b060, Vehicles_DriveAll);

// 0x40b0b0. Maybe launches a random ambient vehicle (a 1 in N chance against
// `chance`), while fewer than the year's cap (1 before 1931, 2 to 1950, 4 to
// 1980, 6 after; scaled 0x5c8578 on big maps) fly and the year is at least
// +0x8c: of the types in service this year (+0xb4 .. +0xb8, not +0xb0) the
// one with the best random score, logged (string 0xaa, kind 7) at its cell.
RT3_NOINLINE void __fastcall Vehicles_MaybeSpawn(Collection* t, int, float chance) {
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x46c38)) return;
    uint16_t year = F<uint16_t>(w, 0xd);
    int32_t cap = year > 0x7bc ? 6 : year > 0x79e ? 4 : (year > 0x78a) + 1;
    uint8_t* map = g_map;
    if (F<int32_t>(map, 0x2159) * F<int32_t>(map, 0x2155) > 0x31704) cap = (int32_t)((double)cap * *(const double*)0x5c8578);
    if (F<int32_t>(w, 0x4c9a) > cap) return;
    int32_t y = year;
    if (F<int32_t>((uint8_t*)t, 0x8c) > y) return;
    float r = (float)Rand(0, (int32_t)(*(const double*)0x5c8a18 / (double)cap));
    if (r > chance) return;
    int32_t best = -1, bestScore = -1;
    for (int32_t k = 0; k < F<int32_t>((uint8_t*)t, 0x88); ++k) {
        uint8_t* rec = (uint8_t*)t + 0xb0 + k * 0x5c;
        if (F<int32_t>(rec, 4) > y || F<int32_t>(rec, 8) < y || F<int32_t>(rec, 0)) continue;
        int32_t s = Rand(10, 100);
        if (s > bestScore) {
            bestScore = s;
            best = k;
        }
    }
    uint8_t* v = Vehicles_Add(t, 0, best);
    F<int32_t>(g_world, 0x4c9a) += 1;
    char ys[0x20], xs[0x20], msg[0x12c];
    const char* yt = Num_Format(ys, 1, (double)(int32_t)Obj_CellY(v), 10, 1, 0, 0);
    const char* xt = Num_Format(xs, 1, (double)(int32_t)Obj_CellX(v), 10, 1, 0, 0);
    FormatText(msg, GetString(0xaa), (const char*)t + 0x90 + best * 0x5c, xt, yt, nullptr, nullptr, nullptr, nullptr, nullptr);
    Log_Add(g_messageLog, 0, msg, 7, F<int32_t>(v, 0x23a), 0);
}
RT3_REPLACE(0x40b0b0, Vehicles_MaybeSpawn);

// 0x40b2d0. Maybe launches a flight of the vehicle type named at 0x5c8a2c
// (always with `force`, else while fewer than 97 fly and a random draw up to
// 0x2f400 is at most `chance`): a leader and 2..5 more in a V behind it (rows
// a step 0x5c8a20 back along heading -/+ 0x5c8a28, jittered +-30 / 0x5c8554,
// a unit lower per row), all on its heading; logged (string 0xab, kind 7) at
// the leader's cell. Returns the leader's id, or 0.
RT3_NOINLINE uint32_t __fastcall Vehicles_MaybeSpawnFlight(Collection* t, int, float chance, int32_t force) {
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x46c38)) return 0;
    if (!force) {
        if (F<int32_t>(w, 0x4c9e) > 0x60) return 0;
        float r = (float)Rand(0, 0x2f400);
        if (r > chance) return 0;
    }
    uint8_t* lead = Vehicles_AddNamed(t, 0, (const char*)0x5c8a2c);
    F<int32_t>(g_world, 0x4c9e) += 1;
    float heading = SceneObj_Angle2(lead);
    float x, y, z;
    SceneObj_GetPos(lead, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    const float spread = *(const float*)0x5c8a28;
    const double step = *(const double*)0x5c8a20;
    float ha = (float)((double)heading - spread);
    float hb = (float)((double)heading + spread);
    float c[2], s[2];
    c[0] = (float)(crt::sin(ha) * step);
    c[1] = (float)(crt::sin(hb) * step);
    s[0] = (float)(crt::cos(ha) * step);
    s[1] = (float)(crt::cos(hb) * step);
    int32_t n = Rand(3, 6) - 1;
    const float jitter = *(const float*)0x5c8554;
    for (int32_t k = 0; k < n; ++k) {
        int32_t side = k & 1;
        float row = (float)((k >> 1) + 1);
        float j1 = (float)Rand(-30, 30);
        float dx = (float)((double)j1 / jitter + (double)row * c[side]);
        float j2 = (float)Rand(-30, 30);
        float dz = (float)((double)j2 / jitter + (double)row * s[side]);
        uint8_t* v = Vehicles_AddNamed(t, 0, (const char*)0x5c8a2c);
        Obj_MoveTo(v, 0, (float)((double)dx + x), (float)((double)y - row), (float)((double)dz + z));
        F<float>(v, 0x2a1) = heading;
        SceneObj_SetAngles(v, 0, 0, *(uint32_t*)&heading, 0);
        F<int32_t>(g_world, 0x4c9e) += 1;
    }
    char ys[0x20], xs[0x20], msg[0x12c];
    const char* yt = Num_Format(ys, 1, (double)(int32_t)Obj_CellY(lead), 10, 1, 0, 0);
    const char* xt = Num_Format(xs, 1, (double)(int32_t)Obj_CellX(lead), 10, 1, 0, 0);
    FormatText(msg, GetString(0xab), xt, yt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Log_Add(g_messageLog, 0, msg, 7, F<int32_t>(lead, 0x23a), 0);
    return F<uint32_t>(lead, 0x23a);
}
RT3_REPLACE(0x40b2d0, Vehicles_MaybeSpawnFlight);

// 0x40b5d0. Loads the 0x456900 objects (frame 0x2af9 / 0x2afa / 0x2afb).
RT3_NOINLINE int32_t __fastcall NamedObjs456900_Load(Collection* t, int, Stream s, int32_t recordSize) {
    return LoadSceneObjects(t, s, recordSize, 0x2af9, 0x2afa, 0x2afb, [](uint8_t* o) { NamedObj456900_Construct(o); },
                            [](uint8_t* o) { NamedObj456900_Destructor(o); });
}
RT3_REPLACE(0x40b5d0, NamedObjs456900_Load);

RT3_NOINLINE void __fastcall Collection_SaveWithRecords2af9(Collection* t, int, Stream s, int32_t recordSize) { SaveWithRecords(t, s, recordSize, 0x2af9); }
RT3_REPLACE(0x40b6a0, Collection_SaveWithRecords2af9);

// 0x40b720. Destructs vehicle table t: removes every vehicle, clears and
// destructs the table.
RT3_NOINLINE void __fastcall Vehicles_Destruct(Collection* t) {
    F<uint32_t>(t, 0) = 0x5c8a0c;
    while (Collection_Count(t)) Vehicles_Remove(t, 0, Collection_IdAt(t, 0, 0, 0));
    Collection_Clear(t);
    Collection_Destruct(t);
}
RT3_REPLACE(0x40b720, Vehicles_Destruct);

// 0x40b760. Keeps ambient sound i (table 0x5ed238, 32 bytes: key, ..., +0x10 prio,
// +0x14 sample, +0x1c name) alive: re-finds its sample, else starts it again paused
// and looping at volume 0.
RT3_NOINLINE void __stdcall AmbientSound_Ensure(int32_t i) {
    uint8_t* e = (uint8_t*)0x5ed238 + i * 32;
    if (F<uint32_t>(e, 0)) {
        uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, F<uint32_t>(e, 0), 1);
        F<uint8_t*>(e, 0x14) = p;
        if (p) {
            F<uint32_t>(p, 0x44) = 0;
            if (F<void*>(e, 0x14)) return;
        }
    }
    F<uint32_t>(e, 0) = Sound_Play3D(Snd(), 0, F<const char*>(e, 0x1c), 0.0f, 0.0f, 0.0f, 0.0f, 1, 1,
                                     F<int32_t>(e, 0x10), 0.0f);
}
RT3_REPLACE(0x40b760, AmbientSound_Ensure);

// 0x40b7d0. Stops and clears the eight ambient sound slots (0x5ed238, 0x20 bytes).
void __fastcall Ambient_StopSlots() {
    for (uint8_t* s = (uint8_t*)0x5ed238; s < (uint8_t*)0x5ed338; s += 0x20) {
        if (uint32_t key = F<uint32_t>(s, 0)) {
            int32_t smp = Sound_LookupSample(SoundSys(), 0, key, 1);
            F<int32_t>(s, 0x14) = smp;
            if (smp) Sound_Stop(SoundSys(), 0, F<uint32_t>(s, 0), 1);
        }
        F<uint32_t>(s, 0) = 0;
        F<int32_t>(s, 0x14) = 0;
    }
    g_62b248 = 0;
}
RT3_REPLACE(0x40b7d0, Ambient_StopSlots);

// 0x40b830. Starts the ambient sounds once the listener is idle: the 8 loops (with
// the cache off meanwhile), random start delays for the 16 one-shots (table
// 0x5ed088, 0x1b bytes: delay = rand(1000) x max / 2000), a default listener and
// the renderer flag +0x3698 = 0x30; sets 0x62b248.
RT3_NOINLINE void __fastcall Ambient_Start(void*) {
    if (Sound_ListenerIdle(Snd())) return;
    F<int32_t>(F<uint8_t*>(Snd(), 0), 0x14) = 0;
    for (int32_t i = 0; i < 8; ++i) AmbientSound_Ensure(i);
    for (uint32_t a = 0x5ed08c; a < 0x5ed23c; a += 0x1b) {
        int32_t r = Rand(0, 1000);
        int32_t max = *(const int32_t*)(a - 4);
        int32_t v;
        __asm {
            fild r
            fimul max
            fmul qword ptr ds:[0x5c8628]
            fdiv qword ptr ds:[0x5c87f8]
            mov eax, 0x5a10d0
            call eax
            mov v, eax
        }
        *(int32_t*)a = v;
    }
    Sound_ListenerDir(Snd(), 0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    Sound_ListenerPos(Snd(), 0, 0.0f, 0.0f, 0.0f);
    F<int32_t>(Engine_Renderer(hook::As<void*>(g_engine)), 0x3698) = 0x30;
    F<int32_t>(F<uint8_t*>(Snd(), 0), 0x14) = 1;
    g_62b248 = 1;
}
RT3_REPLACE(0x40b830, Ambient_Start);

// 0x40b920. Ambient sound. When the camera moved (or every 20 ms) it samples
// every other cell within 32 of the camera cell, weighted by nearness, for
// water, trees (+0x... bit 9), and the block kinds (0 grass, 2 farm, 1 town,
// 3 rock) with their buildings. From that it sets the target volumes of the
// eight loops at 0x5ed240 (stride 0x20; 0x5ed2a0 is the wind by height,
// 0x5ed2e0 the night chorus), shaded by night (0x482690) and faded out above
// height 10..60 (sqrt of view +0x66). One of the 16 one-shots at 0x5ed088
// (0x1b bytes: interval, counter, volume, range, height, where allowed, wav)
// may play at a random spot around the camera, at most every 2.5 s. Each loop
// then eases towards its target (by elapsed ms / 650), starting, pausing or
// stopping its sample.
RT3_NOINLINE void Ambient_Update() {
    uint8_t* snd = g_sound;
    if (Sound_ListenerIdle(snd)) return;
    if (!g_62b248) return;
    int32_t dt = ClockMs() - g_62b24c;
    uint8_t* view = g_62be68;
    float vx = F<float>(view, 0x1c), vy = F<float>(view, 0x20), vz = F<float>(view, 0x24);
    double m = crt::fabs((double)vz - g_62b260);
    m = crt::fabs((double)vy - g_62b25c) + m;
    float moved = (float)(crt::fabs((double)vx - g_62b258) + m);
    if (dt <= 0x14 && !(moved > D(0x5c8c48))) return;
    view = g_62be68;
    g_62b258 = F<float>(view, 0x1c);
    g_62b25c = F<float>(view, 0x20);
    g_62b260 = F<float>(view, 0x24);
    int32_t cx = Ftol((double)F<float>(view, 0x1c) / Fl(0x5c8700));
    int32_t cz = Ftol((double)F<float>(view, 0x24) / Fl(0x5c8700));
    uint8_t* grid = g_map;
    int32_t x0 = cx - 0x20 < 0 ? 0 : cx - 0x20, x1 = cx + 0x20;
    if (F<int32_t>(grid, 0x2155) - 1 < x1) x1 = F<int32_t>(grid, 0x2155) - 1;
    int32_t z0 = cz - 0x20 < 0 ? 0 : cz - 0x20, z1 = cz + 0x20;
    if (F<int32_t>(grid, 0x2159) - 1 < z1) z1 = F<int32_t>(grid, 0x2159) - 1;
    float total = 0.0f, water = 0.0f, grass = 0.0f, farm = 0.0f, town = 0.0f, trees = 0.0f, rock = 0.0f, open = 0.0f;
    float shore = 9999.0f, nearest = 9999.0f;
    if (z0 > z1)
        total = 1.0f;
    else {
        for (int32_t z = z0; z <= z1; z += 2)
            for (int32_t x = x0; x <= x1; x += 2) {
                float d = (float)(uint16_t)DistanceInt(x, z, cx, cz);
                float w = Fl(0x5c8c40) - d;
                if (w < Fl(0x5c878c)) continue;
                grid = g_map;
                total = w + total;
                if (Terrain_KindIs1Or3to5(grid, 0, x, z)) {
                    water = w + water;
                    if (!(nearest < d)) nearest = d;
                    grid = g_map;
                    bool edge = (x > 0 && !Terrain_KindIs1Or3to5(grid, 0, x - 1, z)) || (z > 0 && !Terrain_KindIs1Or3to5(grid, 0, x, z - 1)) ||
                                (x < F<int32_t>(grid, 0x2155) - 1 && !Terrain_KindIs1Or3to5(grid, 0, x + 1, z)) ||
                                (z < F<int32_t>(grid, 0x2159) - 1 && !Terrain_KindIs1Or3to5(grid, 0, x, z + 1));
                    if (edge && !(shore < d)) shore = d;
                    continue;
                }
                if (Terrain_RecBit9(g_map, 0, x, z)) {
                    float k = BitsF(0x400ccccdu);
                    if (d < Fl(0x5c873c)) k = (float)((((double)Fl(0x5c873c) - d) / D(0x5c8610) + D(0x5c8770)) * D(0x5c8c38));
                    trees = (float)((double)k * w + trees);
                }
                int32_t n = (int32_t)Terrain_CellCount(g_map, 0, x, z);
                grid = g_map;
                uint8_t kind = F<uint8_t*>(grid, 0x212d)[(F<int32_t>(grid, 0x214d) * (z / 4) + x / 4) * 4 + 3];
                double dn = (double)n;
                if (kind == 0) grass = (float)(dn / D(0x5c87b8) * w + grass);
                else if (kind == 2) farm = (float)(dn / D(0x5c87b8) * w + farm);
                else if (kind == 1) town = (float)((dn + D(0x5c8628)) / D(0x5c8c30) * w + town);
                else if (kind == 3) rock = w + rock;
                if (n < 2 && (kind == 0 || kind == 2)) open = w + open;
            }
        if (Fl(0x5c85b4) > total) total = 1.0f;
    }
    float* T = (float*)0x5ed240; // T[k * 8]: the eight loops' target volumes
    double tt = total;
    T[6 * 8] = (float)(water / tt);
    T[7 * 8] = (float)(trees / tt);
    T[0] = (float)(grass / tt);
    T[1 * 8] = (float)(farm / tt);
    T[2 * 8] = (float)(town / tt);
    T[4 * 8] = (float)(rock / tt);
    float openF = (float)(open / tt);
    float night = Sky_NightFactor();
    view = g_62be68;
    float alt = F<float>(view, 0x66);
    float camY = F<float>(view, 0x20);
    uint8_t on[16];
    memset(on, 0, sizeof on);
    if (alt < Fl(0x5c8c28) && camY < Fl(0x5c88e0) && T[6 * 8] < D(0x5c88e8) && T[4 * 8] < D(0x5c8c20)) on[15] = 1;
    double w6 = T[6 * 8];
    if (w6 < D(0x5c88e8)) on[14] = 1;
    double w1 = T[1 * 8];
    if (w1 > D(0x5c8680)) on[13] = 1;
    if (w6 > D(0x5c8628) && night <= D(0x5c8880)) on[12] = 1;
    if (T[4 * 8] > D(0x5c8680) && night <= D(0x5c8888)) on[11] = 1;
    double wo = openF;
    if (wo > D(0x5c8c18)) on[0] = 1;
    if (wo > D(0x5c8880)) {
        if (night <= D(0x5c8880)) on[10] = 1;
        else on[8] = on[9] = 1;
    }
    double w2 = T[2 * 8];
    if (w2 > D(0x5c8880) || w1 > D(0x5c8750)) on[5] = on[6] = 1;
    double w0 = T[0];
    if (w0 > D(0x5c8680) || w2 > D(0x5c8680) || w1 > D(0x5c8680)) on[7] = 1;
    if ((w0 > D(0x5c88e8) || w2 > D(0x5c88e8) || w1 > D(0x5c88e8)) && w6 < D(0x5c88e8) && night <= D(0x5c8680)) on[1] = on[2] = 1;
    if (w0 > D(0x5c8988) && w6 < D(0x5c88e8) && night <= D(0x5c8680)) on[3] = on[4] = 1;
    int32_t since = ClockMs() - g_62b254;
    bool roll = since >= 0x9c4;
    if (roll && since < 0xfa0) roll = Rand(0, 0x64) < 0x21;
    else if (roll && since < 0x157c) roll = Rand(0, 0x64) < 0x42;
    if (roll) {
        int32_t played = 0;
        int32_t step = dt < 0x64 ? dt : 0x64;
        for (int32_t k = 0; k < 0x10; ++k) {
            if (!on[k] || played) continue;
            uint8_t* e = (uint8_t*)0x5ed088 + k * 0x1b;
            float r = (float)(crt::fabs((double)alt - F<float>(e, 0x10)) / D(0x5c8c48) / F<float>(e, 0xc));
            if (r > Fl(0x5c85f8)) continue;
            int32_t& counter = F<int32_t>(e, 4);
            counter += step;
            bool go;
            if (k == 0 && counter > 0x14c08) {
                float t = SceneClock_MinutesSinceDawn();
                go = t >= Fl(0x5c8c14) && t <= Fl(0x5c8c10);
            } else
                go = counter > F<int32_t>(e, 0) >> 1 && Rand(0, F<int32_t>(e, 0)) < step;
            if (!go) continue;
            counter = 0;
            for (int32_t tries = 0; tries < 0x14 && !played;) {
                ++tries;
                int32_t a = Rand(3, 0xf);
                a += Rand(3, 0xf);
                a += Rand(3, 0xf);
                double ad = (double)a;
                float dist = (float)(ad / D(0x5c8798) * F<float>(e, 0xc));
                int32_t rr = Rand(0, 1000);
                view = g_62be68;
                float camX = F<float>(view, 0x1c);
                double rd = (double)rr;
                float ang = (float)(rd / D(0x5c87f8) * D(0x5c8c08));
                double dd = dist;
                float px = (float)(crt::sin(ang) * dd + camX);
                float camZ = F<float>(g_62be68, 0x24);
                float pz = (float)(crt::cos(ang) * dd + camZ);
                int32_t tx = Ftol((double)px / Fl(0x5c8700));
                int32_t tz = Ftol((double)pz / Fl(0x5c8700));
                grid = g_map;
                if (tx < 0 || tx >= F<int32_t>(grid, 0x2155) || tz < 0 || tz >= F<int32_t>(grid, 0x2159)) continue;
                uint8_t ok;
                if (Terrain_KindIs1Or3to5(grid, 0, tx, tz)) ok = e[0x14];
                else ok = (int32_t)Terrain_CellCount(g_map, 0, tx, tz) > 0 ? e[0x16] : e[0x15];
                if (!ok) continue;
                float fz = (float)tz, fx = (float)tx;
                double h = Terrain_HeightAtTile(g_map, 0, fx, fz);
                float y = (float)(h + (double)F<float>(e, 0x10) * Fl(0x5c8700));
                Sound_Play3D(g_sound, 0, F<const char*>(e, 0x17), px, y, pz, F<float>(e, 8), 0, 0, 2,
                             F<float>(e, 0xc) * Fl(0x5c8700));
                played = 1;
                g_62b254 = ClockMs();
            }
        }
    }
    T[5 * 8] = 0.0f;
    if (night > Fl(0x5c8598)) T[5 * 8] = (float)((((wo * D(0x5c8628) + T[2 * 8]) + T[1 * 8]) + T[0]) * night);
    T[0] = (float)(wo * D(0x5c8988) + T[0]);
    if (night > Fl(0x5c8598)) {
        double dark = D(0x5c8770) - night;
        T[0] = (float)(T[0] * dark);
        T[1 * 8] = (float)((dark * D(0x5c8640) + D(0x5c8c20)) * T[1 * 8]);
        T[2 * 8] = (float)((dark * D(0x5c8680) + D(0x5c88c8)) * T[2 * 8]);
    }
    if (alt < Fl(0x5c88e0))
        T[3 * 8] = 0.0f;
    else if (alt < Fl(0x5c8c04))
        T[3 * 8] = (float)(((double)alt - Fl(0x5c88e0)) * D(0x5c88e8) / D(0x5c8838));
    else if (alt < Fl(0x5c8c00))
        T[3 * 8] = (float)(((double)alt - Fl(0x5c8c04)) * D(0x5c8880) / D(0x5c8bf8) + D(0x5c88e8));
    else
        T[3 * 8] = 1.0f;
    T[3 * 8] = (float)(T[3 * 8] * D(0x5c8888));
    double wind = T[3 * 8];
    T[3 * 8] = (float)(D(0x5c8640) < wind ? BitsD(0x3fe999999999999aull) : wind);
    double sq = alt == Fl(0x5c8598) ? 0.0 : crt::sqrt((double)alt);
    float hf = (float)sq;
    float fade;
    if (hf > Fl(0x5c8990))
        fade = 0.0f;
    else if (!(hf > Fl(0x5c85f8)))
        fade = 1.0f;
    else
        fade = (float)(D(0x5c8770) - ((double)hf - Fl(0x5c85f8)) / Fl(0x5c8780));
    if (shore <= Fl(0x5c8738)) {
        double a = shore, b = (double)nearest * D(0x5c87e8);
        float mm = (float)(a > b ? a : b);
        if (mm <= Fl(0x5c8738)) T[6 * 8] = (float)(((double)Fl(0x5c8738) - mm) / D(0x5c8bf0) * D(0x5c8628) + T[6 * 8]);
    } else if (shore < Fl(0x5c873c))
        T[6 * 8] = (float)((((double)Fl(0x5c873c) - shore) / Fl(0x5c8898) * D(0x5c8758) + D(0x5c8750)) * T[6 * 8]);
    else
        T[6 * 8] = (float)(T[6 * 8] * D(0x5c8750));
    for (int k : { 0, 1, 2, 4, 5, 6, 7 }) T[k * 8] = fade * T[k * 8];
    for (int k = 0; k < 8; ++k) {
        if (T[k * 8] > D(0x5c8770)) T[k * 8] = 1.0f;
        if (T[k * 8] < D(0x5c8918)) T[k * 8] = 0.0f;
    }
    double el = (double)(ClockMs() - g_62b24c);
    float stepV = (float)(el / D(0x5c8be8));
    double stepD = stepV;
    for (int k = 0; k < 8; ++k) {
        uint8_t* s = (uint8_t*)0x5ed244 + k * 0x20; // current volume; target at -4
        float& cur = F<float>(s, 0);
        float target = F<float>(s, -4);
        if (crt::fabs((double)target - cur) <= stepD)
            cur = target;
        else
            cur = cur > target ? cur - stepV : stepV + cur;
        float vol = F<float>(s, -8) * cur;
        uint32_t& handle = F<uint32_t>(s, -0xc);
        uint8_t* snd2 = g_sound;
        if (vol < D(0x5c85b8)) {
            if (F<int32_t>(s, 0xc)) {
                if (handle) {
                    F<int32_t>(s, 8) = Sound_LookupSample(snd2, 0, handle, 1);
                    if (F<int32_t>(s, 8)) Sound_Stop(g_sound, 0, handle, 1);
                }
                handle = 0;
                F<int32_t>(s, 8) = 0;
            } else {
                uint8_t* smp = (uint8_t*)(uintptr_t)Sound_LookupSample(snd2, 0, handle, 1);
                if (smp) {
                    F<int32_t>(smp, 0xc) = 1;
                    F<uint32_t>(smp, 0x44) |= 8;
                }
            }
            continue;
        }
        bool have = false;
        if (handle) {
            F<int32_t>(s, 8) = Sound_LookupSample(snd2, 0, handle, 1);
            if (F<int32_t>(s, 8)) {
                F<uint32_t>((uint8_t*)(uintptr_t)F<int32_t>(s, 8), 0x44) = 0;
                have = F<int32_t>(s, 8) != 0;
            }
        }
        if (!have) handle = Sound_Play3D(g_sound, 0, F<const char*>(s, 0x10), 0.0f, 0.0f, 0.0f, 0.0f, 1, 1, F<int32_t>(s, 4), 0.0f);
        uint8_t* smp = (uint8_t*)(uintptr_t)Sound_LookupSample(g_sound, 0, handle, 1);
        if (smp) {
            if (F<int32_t>(smp, 0xc)) {
                F<uint32_t>(smp, 0x44) |= 8;
                F<int32_t>(smp, 0xc) = 0;
            }
            F<float>(smp, 0x28) = vol;
            F<uint32_t>(smp, 0x44) |= 1;
        }
    }
    g_62b24c = ClockMs();
}
RT3_REPLACE(0x40b920, Ambient_Update);

}  // namespace rt3
