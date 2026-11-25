// Cities.cpp - RT3.exe 0x420000-0x424000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/stream.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/ptrlist.h"
#include "core/containers.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void* __fastcall Table_GetByName(Collection* t, int, const char* name);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y);
const char* __fastcall GetString(int32_t id);
void __fastcall Obj_UpdateAgeFactor(void* o);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __stdcall Bracket5(int32_t value);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void* __fastcall Obj_Ref3cc(void* o);
int __fastcall ToUpper(char c);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
int32_t __fastcall Obj_Count37f(void* o);
void __fastcall Obj_SumBuildingWeights(void* o);
uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
int32_t IsMultiplayer();

void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                         int32_t serial);

void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y,
                                int32_t freeStanding);

void __fastcall SceneObj_Register(uint8_t* o);

int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3,
                                     int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3, int32_t all);

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
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall City_Init(uint8_t* c, int, uint32_t id, uint32_t x, uint32_t y, int32_t rural);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall Obj_BuildingValue(void* o);
void* __fastcall Obj_Node8(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value);

void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d,
                                    int32_t e, int32_t f);

void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall City_LayStreets(uint8_t* c);
void __fastcall Obj_SumBuildings(void* o);
int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind);
int32_t __fastcall Obj_Get21(void* o);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

int32_t __fastcall Cities_AssignBlocks(Collection* cities);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall SceneObj_Teardown(uint8_t* o);
void __fastcall Collection_Destruct(Collection* c);
void __fastcall Obj_UnlinkBuildings(void* o);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void* __fastcall Obj_Ref2a8(void* o);
int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Queue6cd990_Push(uint16_t a, int32_t b, int32_t c, int32_t d, int32_t e);
uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
int32_t __fastcall Table_CountNamedWithBit(Collection* t, int, const char* name, int32_t bit);
uint32_t __fastcall Table_NthNamedWithBit(Collection* t, int, const char* name, int32_t bit, int32_t nth);
void PushSeed();
void __fastcall SetSeed(int32_t seed);
void PopSeed();
int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y);
int32_t __fastcall Obj_CountTypesAvailable(void* o, int, int32_t year, int32_t kind);
float __fastcall Obj_ProductionRate(void* o, int, int32_t i);
int32_t __fastcall Owner_PickBuildingToSell(uint8_t* o, int, int32_t kind);
void __fastcall TerrainTool_Flatten(uint8_t* t, int, int32_t x, int32_t y, int32_t r);
void __fastcall Types_PickName(Collection* t, int, char* dst, int32_t value);
void __fastcall Obj_Entry37f(void* o, int, int32_t i, char* dst, int32_t* out);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
int32_t __fastcall City_BuildIndustry(uint8_t* c, int, const char* name, int32_t weighted, int32_t checkOnly, uint32_t* outId);
void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
void __fastcall Cities_AddTestRow(Collection* t);
uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE void __fastcall Obj_SumBuildingWeights(void* o);
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE void* __fastcall Table_GetByName(Collection* t, int, const char* name);
RT3_NOINLINE uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall BuildingObj_Teardown(uint8_t* o);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
int32_t __fastcall Camera_FogAlpha(uint8_t* c, int, int32_t x, int32_t y);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

void __fastcall StrCatN(char* dst, const char* src, int32_t n);
int32_t Option2bbActive();
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);

// Functions of this file, in exe order.
RT3_NOINLINE uint32_t __fastcall Buildings_Add(Collection* c, int, uint8_t* t, float x, float y, uint32_t angle, int32_t a7, uint8_t place);
RT3_NOINLINE void __fastcall TerrainTool_Flatten(uint8_t* t, int, int32_t x, int32_t y, int32_t r);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE uint8_t* __fastcall NamedObj41f590_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall NamedObj41f590_Destructor(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value);
uint32_t __fastcall Obj_FindRegionSite(void* o, int, int32_t requireActive, int32_t requireFlag);
void __fastcall Obj_UpdateAgeFactor(void* o);
RT3_NOINLINE void __fastcall Obj_UpdatePortFlags(void* o);
RT3_NOINLINE void __fastcall Obj_NumberPorts(void* o, int, int32_t n);
RT3_NOINLINE void __fastcall Obj_SumBuildingWeights(void* o);
RT3_NOINLINE void __fastcall Obj_StartAgeClock(void* o);
RT3_NOINLINE void __fastcall BuildingObj_Teardown(uint8_t* o);
RT3_NOINLINE uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount);
int32_t __fastcall City_Tooltip(uint8_t* c, int, char* dst, int32_t n, uint32_t* color, uint32_t* scale, uint32_t* icon);
RT3_NOINLINE void __fastcall Obj_YearRollover(uint8_t* o);
void __fastcall Obj_SumBuildings(void* o);
void __fastcall Obj_CopyNamedAmounts(void* o, int, void* other);
RT3_NOINLINE void __fastcall City_ResetBuildingMix(uint8_t* c);
void __fastcall City_Init(uint8_t* c, int, uint32_t id, uint32_t x, uint32_t y, int32_t rural);
RT3_NOINLINE Collection* __fastcall Collection5c9ae0_Construct(Collection* c);
void __fastcall Collection5c9ae0_Destruct(Collection* c);
int32_t __stdcall Bracket5(int32_t value);
RT3_NOINLINE int32_t __fastcall NamedObjs41f590_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveWithRecords5209(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall Cities_LayStreets(Collection* cities);
RT3_NOINLINE int32_t __fastcall Table_CountListing(Collection* t, int, uint32_t typeId);
void* __fastcall Table_NthListing(Collection* t, int, uint32_t typeId, int32_t nth);
void __fastcall Objs_DropInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y);
RT3_NOINLINE const char* __fastcall Table_NearestName(Collection* t, int, float x, float y);
void __fastcall Cities_RandomBoom(Collection* c);
RT3_NOINLINE void __fastcall Cities_Grow(Collection* cities);
RT3_NOINLINE int32_t __fastcall Cities_CountConnected(Collection* c, int, int32_t company);
RT3_NOINLINE int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind);
RT3_NOINLINE int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3, int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3, int32_t all);
RT3_NOINLINE int32_t __fastcall Company_BuildBuilding(uint8_t* co, int, uint32_t id, float x, float y, uint32_t angle, int32_t checkOnly, uint32_t* outId);
RT3_NOINLINE int32_t __fastcall Owner_PickBuildingToSell(uint8_t* o, int, int32_t kind);
RT3_NOINLINE int32_t __fastcall City_BuildIndustry(uint8_t* c, int, const char* name, int32_t weighted, int32_t checkOnly, uint32_t* outId);
RT3_NOINLINE float __fastcall Obj_ProductionRate(void* o, int, int32_t i);
void __fastcall City_Grow(uint8_t* c, int, float dt, int32_t fast);
RT3_NOINLINE void __fastcall Obj_CountBuildings(uint8_t* o);
RT3_NOINLINE void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
RT3_NOINLINE double __fastcall Obj_WeightedAverage(void* o);
RT3_NOINLINE double __fastcall Company_FloatD7b(void* c);
RT3_NOINLINE void __fastcall Company_Add7680(void* c, int, int32_t d);
void __fastcall Obj_AddRevenue(void* o, int, float a, float b, float c, int32_t flag, int32_t idx);
RT3_NOINLINE void __fastcall Obj_Set4b(void* o, int, int32_t v);
void __fastcall Obj_Nop423fc0(void*);
void __fastcall Obj_IncD0f(void* o);
void __fastcall Obj_IncD0b(void* o);
RT3_NOINLINE void __stdcall SetGlobal62bae8(uint8_t v);
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);

static inline double D(uint32_t va) { return *(const double*)va; }

static inline uint32_t Obj_Refresh420410(void* o) { return ((uint32_t(__thiscall*)(void*))0x420410)(o); }

static inline void Obj_Entry41fac0(void* o, int32_t i, char* name, void* out) {
    Obj_Entry37f(o, 0, i, name, (int32_t*)(out));
}

static inline void* Virtual80(void* o) { return ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o); }

// Whether record r (used, or type +0x23e with +0x242 > 0) has an entry
// named `name` (first letter `first`) among its +0x37f entries.
static int HasEntryNamed(uint8_t* r, const char* name, char first) {
    if (F<int32_t>(r, 0x23e) && F<int32_t>(r, 0x242) <= 0) return 0;
    int32_t n = Obj_Count37f(r);
    for (int32_t j = 1; j <= n; ++j) {
        char buf[0x200];
        float out;
        Obj_Entry41fac0(r, j, buf, &out);
        if ((char)ToUpper(buf[0]) == first && crt::stricmp(buf, name) == 0) return 1;
    }
    return 0;
}

static inline Collection* Stations() { return g_stations; }

static inline uint8_t* StationAt(int32_t i) { return (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0)); }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline bool Unserved(uint8_t* e) {
    return !F<int32_t>(e, 0x23e) && !F<int32_t>(e, 0x276) && !F<int32_t>(e, 0x302) &&
           !Region_HasConnectedStation(e, 0, 1, 1, 0, 0);
}

static inline bool InCellRect(uint8_t* o, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    return Obj_CellX(o) >= (float)x0 && Obj_CellX(o) <= (float)x1 && Obj_CellY(o) >= (float)y0 &&
           Obj_CellY(o) <= (float)y1;
}

static inline double D_196(uint32_t a) { return *(const double*)a; }

static inline double Fl_196(uint32_t a) { return *(const float*)a; }

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

// A city's position in 4x4 blocks.
static inline float BlockOf(double cell) { return (float)((cell - Fl_196(0x5c878c)) / D_196(0x5c87b8)); }

static inline double D_200(uint32_t a) { return *(const double*)a; }

static inline double Fl_200(uint32_t a) { return *(const float*)a; }

// Weighted score of the six stat sums (+0x2b2..+0x2c6) per unit of the first.
static float Score(float s0, float s1, float s2, float s3, float s4, float s5, float ratio) {
    float t = (float)(((double)s5 + s4) * D_200(0x5c8628));
    t = (float)(((double)s3 + s2) * Fl_200(0x5c9b0c) + t);
    t = (float)(t * D_200(0x5c8dc0));
    float base = (float)((double)t + s0);
    float v = (float)((double)t * ratio + s1);
    float d = Fl_200(0x5c85b4) > base ? 1.0f : base;
    return (float)((double)v / d);
}

static inline double D_207(uint32_t a) { return *(const double*)a; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// Resets the building-type placement filter (table 0x62b2fc +0x9c..+0xb4).
static void BuildingFilter_Reset(uint8_t* f, int32_t on) {
    F<int32_t>(f, 0x9c) = on;
    F<uint32_t>(f, 0xa0) = 0x497423f0;
    F<uint32_t>(f, 0xa4) = 0xc97423f0;
    F<int32_t>(f, 0xa8) = 999999;
    F<int32_t>(f, 0xac) = -999999;
    F<int32_t>(f, 0xb0) = 999999;
    F<int32_t>(f, 0xb4) = -999999;
}

// 0x420030. Whether the region has a station (optionally active / with a ready
// building / of the given value) that, with `linked`, is track-connected to a
// station outside the region whose cell is in some (+0x23e clear) city's region.
RT3_NOINLINE int32_t __fastcall Region_HasConnectedStation(uint8_t* o, int, int32_t active, int32_t ready, int32_t linked, int32_t value) {
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        uint8_t* s = StationAt(i);
        if (!Obj_HasKey(Obj_Region(o), 0, (uint16_t)F<int32_t>(s, 0))) continue;
        if (value && Obj_BuildingValue(s) != value) continue;
        if (!Obj_BuildingStateActive(s) && active) continue;
        if (Obj_Flag3d4(Obj_Building(s)) != 1 && ready) continue;
        if (!linked) return 1;
        uint32_t node = F<uint32_t>(s, 8);
        for (int32_t j = 0; j < Collection_Count(Stations()); ++j) {
            uint32_t id = Collection_IdAt(Stations(), 0, j, 0);
            if (Obj_HasKey(Obj_Region(o), 0, (uint16_t)id)) continue;
            if (!Obj_BuildingStateActive(StationAt(j)) && active) continue;
            if (Obj_Flag3d4(Obj_Building(StationAt(j))) != 1 && ready) continue;
            if (!Node_SameComponent((uint8_t*)Obj_Node8(StationAt(j)), 0, node)) continue;
            uint32_t sid = Collection_IdAt(Stations(), 0, j, 0);
            Collection* cities = g_cities;
            for (int32_t k = 0; k < Collection_Count(cities); ++k) {
                uint8_t* c = (uint8_t*)Collection_Get(g_cities, 0,
                                                      Collection_IdAt(g_cities, 0, k, 0));
                if (!F<int32_t>(c, 0x23e) && Obj_HasKey(Obj_Region(c), 0, (uint16_t)sid)) return 1;
            }
        }
    }
    return 0;
}
RT3_REPLACE(0x420030, Region_HasConnectedStation);

// 0x420280. ID of the first 0x6cec20 site in this object's region that is
// active (or `anyState`) and whose building is flagged (or `anyFlag` is 0).
uint32_t __fastcall Obj_FindRegionSite(void* o, int, int32_t requireActive, int32_t requireFlag) {
    for (int32_t i = 0; i < Collection_Count(g_stations); ++i) {
        uint32_t id = Collection_IdAt(g_stations, 0, i, 0);
        if (!Obj_HasKey(Obj_Region(o), 0, (uint16_t)id)) continue;
        void* r = Collection_Get(g_stations, 0, Collection_IdAt(g_stations, 0, i, 0));
        if (!Obj_BuildingStateActive(r) && requireActive) continue;
        r = Collection_Get(g_stations, 0, Collection_IdAt(g_stations, 0, i, 0));
        if (Obj_Flag3d4(Obj_Building(r)) == 1 || !requireFlag) return Collection_IdAt(g_stations, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x420280, Obj_FindRegionSite);

// 0x420350. Age factor +0x31b from the years since +0x317: rises for the
// first 3 years, 1.0 up to year 6, then falls (capped at 20 years). Halved
// towards 1 when +0x23e is set.
RT3_NOINLINE void __fastcall Obj_UpdateAgeFactor(void* o) {
    int32_t since = F<int32_t>(o, 0x317);
    if (since == 0) {
        F<float>(o, 0x31b) = 1.0f;
    } else {
        int32_t age = F<uint16_t>(g_world, 0xd) - since;
        if (age < 0) age = 0;
        if (age > 20) age = 20;
        if (age < 3) F<float>(o, 0x31b) = (float)((double)(3 - age) * D(0x5c86b8) + D(0x5c8770));
        else if (age < 6) F<float>(o, 0x31b) = 1.0f;
        else F<float>(o, 0x31b) = (float)(D(0x5c8770) - (double)(age - 6) * D(0x5c9aa0));
    }
    if (F<int32_t>(o, 0x23e))
        F<float>(o, 0x31b) = (float)(((double)F<float>(o, 0x31b) + D(0x5c8770)) / D(0x5c85d0));
}
RT3_REPLACE(0x420350, Obj_UpdateAgeFactor);

// 0x420410. Port flags from the +0x37f type list: +0x2f6 any port/warehouse,
// +0x2fa a port (+0xba), +0x2fe a warehouse (+0xbb only).
RT3_NOINLINE void __fastcall Obj_UpdatePortFlags(void* o) {
    F<int32_t>(o, 0x2f6) = 0;
    F<int32_t>(o, 0x2fa) = 0;
    F<int32_t>(o, 0x2fe) = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        uint8_t* r = (uint8_t*)Table_GetByName(g_62b268, 0, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
        if (!r || (!F<uint8_t>(r, 0xba) && !F<uint8_t>(r, 0xbb))) continue;
        F<int32_t>(o, 0x2f6) = 1;
        if (F<uint8_t>(r, 0xba)) F<int32_t>(o, 0x2fa) = 1;
        else F<int32_t>(o, 0x2fe) = 1;
    }
}
RT3_REPLACE(0x420410, Obj_UpdatePortFlags);

// 0x4204c0. Renames port/warehouse entries of the +0x37f table to
// "Port%02d"/"Warehouse%02d" with number n (once per change of n, +0x2f2).
RT3_NOINLINE void __fastcall Obj_NumberPorts(void* o, int, int32_t n) {
    if (n == F<int32_t>(o, 0x2f2)) return;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        char* name = (char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        uint8_t* r = (uint8_t*)Table_GetByName(g_62b268, 0, name);
        if (!r) continue;
        if (F<uint8_t>(r, 0xba)) crt::sprintf(name, (const char*)0x5c93e8, n);      // "Port%02d"
        else if (F<uint8_t>(r, 0xbb)) crt::sprintf(name, (const char*)0x5c93d8, n); // "Warehouse%02d"
    }
    F<int32_t>(o, 0x2f2) = n;
}
RT3_REPLACE(0x4204c0, Obj_NumberPorts);

// 0x420560. Recomputes +0x312 from the buildings in the chain +0x383
// (by type kind +0x32) when the type +0x23e is 0.
RT3_NOINLINE void __fastcall Obj_SumBuildingWeights(void* o) {
    F<int32_t>(o, 0x312) = 0;
    if (F<int32_t>(o, 0x23e)) return;
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
        switch (F<uint8_t>(t, 0x32)) {
        case 0: {
            double v = ((double)F<int32_t>(Obj_Ref3cc(b), 0x141) + D(0x5c8758)) / D(0x5c87e8);
            F<float>(o, 0x312) = (float)(v + F<float>(o, 0x312));
            break;
        }
        case 2:
            if (F<int32_t>(t, 0x78c)) {
                double v = (F<int32_t>(b, 0x246) ? 1.75 : 1.0) * D(0x5c8d68);
                F<float>(o, 0x312) = (float)(v + F<float>(o, 0x312));
            } else {
                F<float>(o, 0x312) = (float)((double)F<float>(o, 0x312) + *(const float*)0x5c8900);
            }
            break;
        case 3:
            F<float>(o, 0x312) = (float)((double)F<float>(o, 0x312) + D(0x5c85d0));
            break;
        }
    }
}
RT3_REPLACE(0x420560, Obj_SumBuildingWeights);

// 0x420650. Starts the age clock (+0x317 = this year) if unset, then updates the factor.
RT3_NOINLINE void __fastcall Obj_StartAgeClock(void* o) {
    if (F<int32_t>(o, 0x317)) return;
    F<int32_t>(o, 0x317) = F<uint16_t>(g_world, 0xd);
    Obj_UpdateAgeFactor(o);
}
RT3_REPLACE(0x420650, Obj_StartAgeClock);

// 0x420670. Building object o: deletes its part table (+0x37f), unlinks its
// buildings and tears down the scene object.
RT3_NOINLINE void __fastcall BuildingObj_Teardown(uint8_t* o) {
    if (Collection* t = F<Collection*>(o, 0x37f)) {
        Collection_Destruct(t);
        crt::operator_delete(t);
    }
    F<void*>(o, 0x37f) = nullptr;
    Obj_UnlinkBuildings(o);
    SceneObj_Teardown(o);
}
RT3_REPLACE(0x420670, BuildingObj_Teardown);

// 0x4206b0. Sets the amount (+0x1e) of the named entry in +0x37f: removes
// it when amount <= 0, adds a new {name, amount} entry when missing and
// amount > 0; then refreshes (0x420410).
RT3_NOINLINE uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount) {
    Collection* c = F<Collection*>(o, 0x37f);
    int32_t i = 0;
    for (; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        c = F<Collection*>(o, 0x37f);
        if (crt::stricmp(name, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0))) == 0) break;
    }
    const float zero = *(const float*)0x5c8598;
    if (i >= Collection_Count(F<Collection*>(o, 0x37f))) {
        if (amount > zero) {
            uint8_t rec[0x24] = {};
            char* d = (char*)rec;
            const char* s = name;
            while ((*d++ = *s++) != 0) {}
            F<float>(rec, 0x1e) = amount;
            c = F<Collection*>(o, 0x37f);
            ((void(__thiscall*)(void*, void*, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, rec, 0, 0, 0);
        }
    } else if (amount <= zero) {
        c = F<Collection*>(o, 0x37f);
        void** vt = *(void***)c;
        uint32_t id = Collection_IdAt(c, 0, i, 0);
        ((void(__thiscall*)(void*, uint32_t))vt[2])(F<Collection*>(o, 0x37f), id);
    } else {
        c = F<Collection*>(o, 0x37f);
        F<float>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x1e) = amount;
    }
    return Obj_Refresh420410(o);
}
RT3_REPLACE(0x4206b0, Obj_SetNamedAmount);

// 0x4207d0 (vtable). Tooltip of city c into dst (n bytes): its name
// (QUIRK: used as the format) and population (+0x276, pattern 0x5c9acc),
// alpha from the fog (0x62be68) unless a hovered or panel-selected station
// building covers it (then opaque, string 0xcf, plus 0xf1c when it is not
// yet served); a station building near it (0x5c9ac8) adds whether it can be
// connected (0xf1d..0xf21). With the size option (0x41fff0) also the city
// icon *icon (0x6cec78 +0x46a88, by size: editor +0x25e, else +0x312
// scaled; returns 3, else 1); hidden cities (+0x23e) and blocked tooltips
// (0x62be84) get the generic / no tooltip.
int32_t __fastcall City_Tooltip(uint8_t* c, int, char* dst, int32_t n, uint32_t* color, uint32_t* scale, uint32_t* icon) {
    if (g_62be84) return 0;
    if (F<int32_t>(c, 0x23e))
        return SceneObj_DebugTooltip(c, 0, dst, n, (int32_t)(uintptr_t)color, (int32_t)(uintptr_t)scale, (int32_t)(uintptr_t)icon);
    bool keyed = false, nearStation = false;
    const int32_t fy = Ftol(Obj_CellY(c));
    const int32_t fx = Ftol(Obj_CellX(c));
    const int32_t fog = Camera_FogAlpha(g_62be68, 0, fx, fy);
    uint8_t* b = nullptr;
    if (uint8_t* panel = g_buildingDialog) {
        const uint32_t id = F<uint32_t>(panel, 0x7c);
        if (Collection_IsValid(g_buildings, 0, id)) b = (uint8_t*)Collection_Get(g_buildings, 0, id);
    }
    if (!b) {
        uint8_t* hover = F<uint8_t*>(g_world, 0x66a2);
        if (hover && ((int32_t(__thiscall*)(uint8_t*))(*(void***)hover)[0x2c / 4])(hover)) b = F<uint8_t*>(g_world, 0x66a2);
    }
    bool opaque = false;
    if (b && ((int32_t(__thiscall*)(uint8_t*))(*(void***)b)[0x70 / 4])(b) && Obj_BuildingStateActive(Obj_Ref2a8(b))) {
        const int32_t station = F<int32_t>(b, 0x2a8);
        nearStation = true;
        if (Obj_HasKey(Obj_Region(c), 0, (uint16_t)station)) {
            *color = (*color & 0xffffff7f) | 0xff000000;
            keyed = true;
            opaque = true;
        }
    }
    if (!opaque) {
        if (!fog) return SceneObj_DebugTooltip(c, 0, dst, n, (int32_t)(uintptr_t)color, (int32_t)(uintptr_t)scale, (int32_t)(uintptr_t)icon);
        if ((uint32_t)fog < 0xff) *color = (*color & 0xffffff) | ((uint32_t)fog << 24);
    }
    char frame[0x398];
    char* text = frame + 0x10;
    char* popText = frame + 0x140;
    char* popNum = frame + 0x26c;
    crt::sprintf(text, (const char*)c + 0x356);
    const int32_t pop = F<int32_t>(c, 0x276);
    if (pop) {
        Num_FormatSimple2(popNum, 0xb, (double)pop, 0xb);
        FormatText(popText, (const char*)0x5c9acc, popNum, 0, 0, 0, 0, 0, 0, 0);
        strcat(text, popText);
    }
    *scale = 0xbfc00000;
    StrCopyN(dst, text, (size_t)(n - 1));
    if (keyed) {
        StrCatN(dst, GetString(0xcf), n - 1);
        if (F<int32_t>(c, 0x276) && Obj_Flag3d4(b) == 1) {
            const int32_t ready = Obj_Flag3d4(b) == 1 ? 1 : 0;
            if (!Region_HasConnectedStation(c, 0, 1, ready, 1, 0)) StrCatN(dst, GetString(0xf1c), n - 1);
        }
    } else if (nearStation) {
        const float cy = (float)Obj_CellY(c), cx = (float)Obj_CellX(c), by = (float)Obj_CellY(b), bx = (float)Obj_CellX(b);
        if (Distance2D(bx, by, cx, cy) < Fl_207(0x5c9ac8)) {
            int32_t str;
            if (Region_HasConnectedStation(c, 0, 1, 1, 0, Obj_Get21(g_world)))
                str = Obj_Flag3d4(b) == 1 ? 0xf1d : 0xf1e;
            else if (Region_HasConnectedStation(c, 0, 1, 1, 0, 0))
                str = Obj_Flag3d4(b) == 1 ? 0xf1f : 0xf20;
            else
                str = 0xf21;
            StrCatN(dst, GetString(str), n - 1);
        }
    }
    if (!Option2bbActive()) return 1;
    if (F<int32_t>(g_app, 0x68)) {
        const int32_t idx0 = Ftol((double)F<float>(c, 0x25e) / Fl_207(0x5c9ac4) * Fl_207(0x5c9ac0));
        int32_t idx = idx0 < 0 ? 0 : idx0;
        if (idx > 0x95) idx = 0x95;
        *icon = Table_Entry(F<uint8_t*>(g_world, 0x46a88), 0, idx);
        return 3;
    }
    float size = (float)((double)F<float>(c, 0x312) / D_207(0x5c9ab8));
    if (size < D_207(0x5c8680))
        size = (float)((double)size * D_207(0x5c8da8) + D_207(0x5c9ab0));
    else if (size > D_207(0x5c8dc0))
        size = (float)(((double)size - D_207(0x5c8dc0)) * D_207(0x5c8750) + D_207(0x5c8dc0));
    const int32_t idx0 = Ftol((double)size * Fl_207(0x5c9aa8));
    int32_t idx = idx0 < 0 ? 0 : idx0;
    if (idx > 0x95) idx = 0x95;
    *icon = Table_Entry(F<uint8_t*>(g_world, 0x46a88), 0, idx);
    return 3;
}
RT3_REPLACE(0x4207d0, City_Tooltip);

// 0x420c30. Year roll-over: adds this year's six amounts (+0x2b2..) to the
// totals (+0x29a..), keeps them as last year's (+0x2ca..), clears them,
// recounts the buildings and updates the age factor.
RT3_NOINLINE void __fastcall Obj_YearRollover(uint8_t* o) {
    uint32_t cur[6];
    for (int k = 0; k < 6; ++k) cur[k] = F<uint32_t>(o, 0x2b2 + k * 4);
    for (int k = 0; k < 6; ++k)
        F<float>(o, 0x29a + k * 4) = (float)((double)F<float>(o, 0x29a + k * 4) + F<float>(o, 0x2b2 + k * 4));
    for (int k = 0; k < 6; ++k) F<uint32_t>(o, 0x2ca + k * 4) = cur[k];
    for (int k = 0; k < 6; ++k) F<uint32_t>(o, 0x2b2 + k * 4) = 0;
    F<int32_t>(o, 0x296) = Obj_BuildingStats(o, 0, &F<int32_t>(o, 0x28a), nullptr, &F<int32_t>(o, 0x28e),
                                             &F<int32_t>(o, 0x292), nullptr, nullptr, nullptr, nullptr, 1);
    Obj_UpdateAgeFactor(o);
}
RT3_REPLACE(0x420c30, Obj_YearRollover);

// 0x420d40. Sums the buildings of the chain +0x383 into +0x30a / +0x306
// (slot 0 values) and +0x30e (capacity of kind-0 types), then the weights.
RT3_NOINLINE void __fastcall Obj_SumBuildings(void* o) {
    F<int32_t>(o, 0x306) = 0;
    F<int32_t>(o, 0x30a) = 0;
    F<int32_t>(o, 0x30e) = 0;
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        F<float>(o, 0x30a) = (float)(Obj_Float2c0(b, 0, 0) + F<float>(o, 0x30a));
        F<float>(o, 0x306) = (float)(Obj_Float2e8(b, 0, 0) + F<float>(o, 0x306));
        if (F<uint8_t>(Virtual80(b), 0x32) == 0) {
            float cap = (float)F<int32_t>(Obj_Ref3cc(b), 0x141);
            F<float>(o, 0x30e) = (float)((double)cap + F<float>(o, 0x30e));
        }
    }
    Obj_SumBuildingWeights(o);
}
RT3_REPLACE(0x420d40, Obj_SumBuildings);

// 0x420e00. Replaces the named-amount list +0x37f with a copy of other's
// (a fresh inline collection of 0x22-byte records, 2 + 2).
void __fastcall Obj_CopyNamedAmounts(void* o, int, void* other) {
    Collection* c = F<Collection*>(o, 0x37f);
    if (c) {
        Collection_Destruct(c);
        crt::operator_delete(c);
    }
    F<Collection*>(o, 0x37f) = nullptr;
    Collection* n = (Collection*)crt::operator_new(0x88);
    if (n) n = Collection_Construct(n, 0, 1, 0x22, 2, 2, 0, 0, 0);
    F<Collection*>(o, 0x37f) = n;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(other, 0x37f)); ++i) {
        Collection* src = F<Collection*>(other, 0x37f);
        uint8_t* r = (uint8_t*)Collection_Get(src, 0, Collection_IdAt(src, 0, i, 0));
        char name[0xc8];
        char* d = name;
        const char* s = (const char*)r;
        while ((*d++ = *s++) != 0) {}
        Obj_SetNamedAmount(o, 0, name, F<float>(r, 0x1e));
    }
}
RT3_REPLACE(0x420e00, Obj_CopyNamedAmounts);

// 0x420ed0. Rebuilds the city's building mix (+0x37f, records of 0x22 bytes): a
// rural city (+0x23e) gets houses, farms, camps and mines at 0.2 each; a town
// commercial blocks at 0.3 and houses plus every factory type at 0.2.
RT3_NOINLINE void __fastcall City_ResetBuildingMix(uint8_t* c) {
    if (Collection* old = F<Collection*>(c, 0x37f)) {
        Collection_Destruct(old);
        crt::operator_delete(old);
    }
    F<Collection*>(c, 0x37f) = nullptr;
    Collection* t = (Collection*)crt::operator_new(0x88);
    F<Collection*>(c, 0x37f) = t ? Collection_Construct(t, 0, 1, 0x22, 2, 2, 0, 0, 0) : nullptr;
    static const uint32_t rural[] = { 0x5c9114, 0x5c9264, 0x5c9248, 0x5c923c, 0x5c9224, 0x5c920c, 0x5c91f4, 0x5c91e8, 0x5c91d0,
                                      0x5c91a0, 0x5c9188, 0x5c90d4, 0x5c8fdc, 0x5c935c, 0x5c9300, 0x5c8e74, 0x5c9108 };
    static const uint32_t town[] = { 0x5c9114, 0x5c929c, 0x5c8ff8, 0x5c93bc, 0x5c93a0, 0x5c938c, 0x5c9378, 0x5c9344, 0x5c9334, 0x5c92c8, 0x5c8df0,
                                     0x5c9168, 0x5c9080, 0x5c904c, 0x5c9038, 0x5c8e00, 0x5c8fc4, 0x5c8f08, 0x5c8eec, 0x5c8ecc, 0x5c8eac, 0x5c8e3c,
                                     0x5c9140 };
    uint32_t bits = 0x3e4ccccd, bits3 = 0x3e99999a;
    float fifth = *(float*)&bits, third = *(float*)&bits3;
    if (F<int32_t>(c, 0x23e)) {
        for (uint32_t n : rural) Obj_SetNamedAmount(c, 0, (const char*)n, fifth);
        return;
    }
    Obj_SetNamedAmount(c, 0, (const char*)0x5c92e8, third);
    for (uint32_t n : town) Obj_SetNamedAmount(c, 0, (const char*)n, fifth);
}
RT3_REPLACE(0x420ed0, City_ResetBuildingMix);

// 0x421200. Sets up city c with id at cell (x, y), rural or not: clears its
// statistics, size 100 x 100, a fresh building-mix table +0x37f, the default
// name (string 0xcd), the "Marker09" model, then registers it and fills the
// building mix.
RT3_NOINLINE void __fastcall City_Init(uint8_t* c, int, uint32_t id, uint32_t x, uint32_t y, int32_t rural) {
    F<uint32_t>(c, 0x23a) = id;
    F<uint32_t>(c, 0x272) = 5;
    F<uint16_t>(c, 0x383) = 0;
    F<int32_t>(c, 0x23e) = rural;
    memset(c + 0x242, 0, 0x14);
    F<uint32_t>(c, 0x256) = 0xa;
    memset(c + 0x276, 0, 0x24);
    memset(c + 0x29a, 0, 0x68);
    memset(c + 0x302, 0, 0x14);
    c[0x316] = 0;
    F<uint32_t>(c, 0x317) = 0;
    F<uint32_t>(c, 0x31b) = 0x3f800000;
    F<uint32_t>(c, 0x25e) = 0x42c80000;
    F<uint32_t>(c, 0x25a) = 0x42c80000;
    Collection* t = (Collection*)crt::operator_new(0x88);
    F<Collection*>(c, 0x37f) = t ? Collection_Construct(t, 0, 1, 0x22, 2, 2, 0, 0, 0) : nullptr;
    memset(c + 0x262, 0, 0x10);
    F<int32_t>(c, 0x385) = F<int32_t>(g_app, 0x174) != 0;
    memset(c + 0x31f, 0, 0xd * 4 + 3);
    strcpy((char*)c + 0x356, GetString(0xcd));
    WorldModel_Init(c, 0, (const char*)0x5c9ad4, nullptr, nullptr, x, y, 0);
    SceneObj_Register(c);
    City_ResetBuildingMix(c);
}
RT3_REPLACE(0x421200, City_Init);

// 0x421430 / 0x421470. Collection subclass 0x5c9ae0 (heap records, 10 + 20; +0x88 = 0).
RT3_NOINLINE Collection* __fastcall Collection5c9ae0_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9ae0;
    F<int32_t>(c, 0x88) = 0;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x421430, Collection5c9ae0_Construct);

void __fastcall Collection5c9ae0_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9ae0;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x421470, Collection5c9ae0_Destruct);

// 0x421490. Bracket 0..4 of a value against 50 / 62.5 / 87.5 / 117.5 / 155 (0x5f3a3c..).
RT3_NOINLINE int32_t __stdcall Bracket5(int32_t value) {
    const float* t = (const float*)0x5f3a3c;
    float v = (float)value;
    int32_t c = 0;
    if (!(v < t[1])) c = 1;
    if (!(v < t[2])) c = 2;
    if (!(v < t[3])) c = 3;
    return !(v < t[4]) ? 4 : c;
}
RT3_REPLACE(0x421490, Bracket5);

// 0x421510. Loads the 0x41f590 objects (frame 0x5209 / 0x520a / 0x520b).
RT3_NOINLINE int32_t __fastcall NamedObjs41f590_Load(Collection* t, int, Stream s, int32_t recordSize) {
    return LoadSceneObjects(t, s, recordSize, 0x5209, 0x520a, 0x520b, [](uint8_t* o) { NamedObj41f590_Construct(o); },
                            [](uint8_t* o) { NamedObj41f590_Destructor(o); });
}
RT3_REPLACE(0x421510, NamedObjs41f590_Load);

// 0x4215e0. As 0x41d4c0 with tags 0x5209/0x520a/0x520b.
RT3_NOINLINE void __fastcall Collection_SaveWithRecords5209(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4215e0, Collection_SaveWithRecords5209);

// 0x421660. Adds a city at cell (x, y) (rural or not, named `name` when
// given) to table t (0x389-byte records), marks the map's cities changed
// (+0x2201) and returns its id.
RT3_NOINLINE uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural) {
    uint8_t tmp[0x38c];
    NamedObj41f590_Construct(tmp);
    uint32_t id = Collection_Allocate(t, 0, tmp, 0x389, 0, 0);
    uint8_t* c = (uint8_t*)Collection_Get(t, 0, id);
    City_Init(c, 0, id, x, y, rural);
    if (name) StrCopyN((char*)c + 0x356, name, 0x28);
    F<int32_t>(g_map, 0x2201) = 1;
    NamedObj41f590_Destructor(tmp);
    return id;
}
RT3_REPLACE(0x421660, Cities_Add);

RT3_DELETE_RECORD(0x421700, BuildingObjs_Delete, BuildingObj_Teardown, uint8_t*)

// 0x421730. Assigns the map's 4x4 blocks (+0x212d, u16 city at +1) to cities: each
// city (not flagged +0x23e) claims the blocks within its radius (+0x256, in
// blocks) that are unclaimed or closer to it than to their owner. Then every land
// block counts towards its city's block count (+0x242) and extents (+0x246..+0x252;
// unclaimed blocks go to their region byte's city). Leaves the block height in EAX
// (a caller passes it on).
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities) {
    uint8_t* m = g_map;
    F<int32_t>(m, 0x2201) = 0;
    int32_t W = F<int32_t>(m, 0x214d), H = F<int32_t>(m, 0x2151);
    auto block = [&](int32_t x, int32_t y) {
        uint8_t* mm = g_map;
        return F<uint8_t*>(mm, 0x212d) + (F<int32_t>(mm, 0x214d) * y + x) * 4;
    };
    for (int32_t y = 0; y < H; ++y)
        for (int32_t x = 0; x < W; ++x) F<uint16_t>(block(x, y), 1) = 0;
    for (int32_t i = 0; i < Collection_Count(cities); ++i) {
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
        F<int32_t>(c, 0x246) = 9999;
        F<int32_t>(c, 0x24a) = 9999;
        F<int32_t>(c, 0x24e) = 0;
        F<int32_t>(c, 0x252) = 0;
        F<int32_t>(c, 0x242) = 0;
    }
    for (int32_t i = 0; i < Collection_Count(cities); ++i) {
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
        if (F<int32_t>(c, 0x23e)) continue;
        float r = (float)(((double)F<int32_t>(c, 0x256) + D_196(0x5c87d8)) / D_196(0x5c87b8));
        float r2 = (float)((double)r * r);
        float bx = BlockOf(Obj_CellX(c)), by = BlockOf(Obj_CellY(c));
        int32_t x0 = Ftol((double)bx - r), y0 = Ftol((double)by - r);
        int32_t x1 = Ftol((double)bx + r + Fl_196(0x5c85b4)), y1 = Ftol((double)by + r + Fl_196(0x5c85b4));
        if (x0 < 0) x0 = 0;
        if (y0 < 0) y0 = 0;
        if (x1 >= W) x1 = W - 1;
        if (y1 >= H) y1 = H - 1;
        for (int32_t y = y0; y <= y1; ++y) {
            float fy = (float)y;
            for (int32_t x = x0; x <= x1; ++x) {
                float fx = (float)x;
                float d = (float)DistanceSq2D(bx, by, fx, fy);
                if (!(d < r2)) continue;
                uint16_t& owner = F<uint16_t>(block(x, y), 1);
                if (!owner) {
                    owner = F<uint16_t>(c, 0x23a);
                    continue;
                }
                uint8_t* o = (uint8_t*)Collection_Get(cities, 0, owner);
                float ox = BlockOf(Obj_CellX(o)), oy = BlockOf(Obj_CellY(o));
                float d2 = (float)DistanceSq2D(ox, oy, fx, fy);
                if (d < d2) F<uint16_t>(block(x, y), 1) = F<uint16_t>(c, 0x23a);
            }
        }
    }
    for (int32_t y = 0; y < H; ++y) {
        for (int32_t x = 0; x < W; ++x) {
            if (Terrain_KindIs1Or4(g_map, 0, 2 + x * 4, 2 + y * 4)) continue;
            uint8_t* b = block(x, y);
            uint32_t id = F<uint16_t>(b, 1);
            if (!id) id = b[0];
            uint8_t* c = (uint8_t*)Collection_Get(cities, 0, id);
            ++F<int32_t>(c, 0x242);
            if (x < F<int32_t>(c, 0x246)) F<int32_t>(c, 0x246) = x;
            if (x > F<int32_t>(c, 0x24e)) F<int32_t>(c, 0x24e) = x;
            if (y < F<int32_t>(c, 0x24a)) F<int32_t>(c, 0x24a) = y;
            if (y > F<int32_t>(c, 0x252)) F<int32_t>(c, 0x252) = y;
        }
    }
    return H;
}
RT3_REPLACE(0x421730, Cities_AssignBlocks);

// 0x421b60. City table t: lays out 24 test cities named 0xb5c with "00".."23"
// along x = 4 (y = 1, 3, .. 47), each a block city (+0x23e), then assigns
// their blocks.
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t) {
    for (int32_t i = 0, y = 1; y < 0x31; ++i, y += 2) {
        Loading_Pump(g_app, 0, 0, 0);
        char num[0x20], name[0x120];
        crt::sprintf(num, (const char*)0x5c9aec, i);
        FormatText(name, GetString(0xb5c), num, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        float fy = (float)y;
        uint32_t id = Cities_Add(t, 0, 0x40800000, *(uint32_t*)&fy, name, 1);
        F<int32_t>(Collection_Get(t, 0, id), 0x23e) = 1;
    }
    Cities_AssignBlocks(t);
}
RT3_REPLACE(0x421b60, Cities_AddTestRow);

// 0x421c20. City table t (unless the world is busy, 0x6cec78+0x46c38): runs
// every city's update (0x4235c0, with a / b) inside a +0x88 nesting count,
// moving the loading bar 30 steps per city spread over the table.
RT3_NOINLINE void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b) {
    if (F<int32_t>(g_world, 0x46c38)) return;
    ++F<int32_t>(t, 0x88);
    int32_t base = g_skipCount;
    for (int32_t i = 0, step = 0x1e; i < Collection_Count(t); ++i, step += 0x1e) {
        uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4235c0)(c, 0, a, b);
        int32_t d = Collection_Count(t) < 1 ? 1 : Collection_Count(t);
        g_skipCount = step / d + base;
        Loading_Pump(g_app, 0, 0, 0);
    }
    --F<int32_t>(t, 0x88);
}
RT3_REPLACE(0x421c20, Cities_UpdateAll);

// 0x421ce0. Lays out every city's streets.
RT3_NOINLINE void __fastcall Cities_LayStreets(Collection* cities) {
    for (int32_t i = 0; i < Collection_Count(cities); ++i)
        City_LayStreets((uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0)));
}
RT3_REPLACE(0x421ce0, Cities_LayStreets);

// 0x421d20 / 0x421e30. Records that list the 0x62b268 type `typeId` by
// name: count / the n-th (1-based) record.
RT3_NOINLINE int32_t __fastcall Table_CountListing(Collection* t, int, uint32_t typeId) {
    const char* name = (const char*)Collection_Get(g_62b268, 0, typeId) + 4;
    char first = (char)ToUpper(name[0]);
    int32_t count = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (HasEntryNamed((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), name, first)) ++count;
    return count;
}
RT3_REPLACE(0x421d20, Table_CountListing);

void* __fastcall Table_NthListing(Collection* t, int, uint32_t typeId, int32_t nth) {
    const char* name = (const char*)Collection_Get(g_62b268, 0, typeId) + 4;
    char first = (char)ToUpper(name[0]);
    int32_t count = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (HasEntryNamed(r, name, first) && ++count == nth) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x421e30, Table_NthListing);

// 0x421f50. Drops every object of the collection inside the cell rectangle (x0, y0)..
// (x1, y1) to the ground, except those flagged +0x23e.
void __fastcall Objs_DropInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(o, 0x23e)) continue;
        if (InCellRect(o, x0, y0, x1, y1)) Obj_DropToGround(o, 0, 0);
    }
}
RT3_REPLACE(0x421f50, Objs_DropInRect);

// 0x422010. +0x23a of the nearest record (with +0x23e == 0) to (x, y) in cells.
RT3_NOINLINE int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y) {
    float best = 1.0e8f; // 0x4cbebc20
    int32_t result = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        float cy = (float)Obj_CellY(r);
        float cx = (float)Obj_CellX(r);
        float d = (float)Distance2D(x, y, cx, cy);
        if (d < best) {
            best = d;
            result = F<int32_t>(r, 0x23a);
        }
    }
    return result;
}
RT3_REPLACE(0x422010, Table_NearestValue23a);

// 0x4220b0. Name (+0x356) of the nearest record to (x, y); otherwise string
// 0xd0 copied into the static buffer 0x62ba90.
RT3_NOINLINE const char* __fastcall Table_NearestName(Collection* t, int, float x, float y) {
    int32_t id = Table_NearestValue23a(t, 0, x, y);
    if (id) return (const char*)Collection_Get(t, 0, id) + 0x356;
    const char* s = GetString(0xd0);
    char* d = (char*)0x62ba90;
    while ((*d++ = *s++) != 0) {}
    return (const char*)0x62ba90;
}
RT3_REPLACE(0x4220b0, Table_NearestName);

// 0x422100. Once the game is past its first half year (not sandbox, not +0x4aeb),
// with a chance falling with the distance from 1860 and the map width, a random
// city without stations gets a growth boom (+0x276, 100000 x 1..6 by its size
// +0x25e) and a news item.
void __fastcall Cities_RandomBoom(Collection* c) {
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x46c38)) return;
    uint16_t year = F<uint16_t>(w, 0xd);
    if (year < F<uint16_t>(w, 5)) return;
    if (year == F<uint16_t>(w, 5) && w[0xf] <= 6) return;
    if (F<int32_t>(w, 0x4aeb)) return;
    int32_t chance = (0x32 - abs(year - 0x744)) * (0x17318 / F<int32_t>(g_map, 0x2155)) / 70;
    if (SimRand(0, 10000) >= chance) return;
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (Unserved((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)))) ++n;
    if (n < 1) return;
    int32_t pick = SimRand(1, n);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!Unserved(e) || --pick > 0) continue;
        float size = F<float>(e, 0x25e);
        int32_t r;
        if (size > Fl(0x5c9af8)) r = SimRand(4, 6);
        else if (size > Fl(0x5c8c28)) r = SimRand(3, 5);
        else if (size > Fl(0x5c9af4)) r = SimRand(2, 4);
        else r = SimRand(1, 3);
        int32_t v = r * 100000;
        F<int32_t>(e, 0x276) = v;
        World_AddNamedEntry(hook::As<void*>(g_world), 0, (const char*)0x5c87a8, 7, 0, F<int32_t>(e, 0x23a), v, -1, -1);
        return;
    }
}
RT3_REPLACE(0x422100, Cities_RandomBoom);

// 0x422320. Yearly city growth (not in sandbox modes): after refreshing each
// city's building sums, cities (then villages, +0x23e) are scored against their
// group's average; the difference sets the score (+0x2e2, -1 under the floor),
// gap (+0x2e6), clamped trend (+0x2ea, -0.7..1) and growth rate (+0x2ee, 7% / 5% of
// the trend, damped by size +0x25e, no shrinking without industry of kind 2 and
// halved / doubled with one), which then scales the population +0x25e.
RT3_NOINLINE void __fastcall Cities_Grow(Collection* cities) {
    if (g_newGame[0x82] || F<int32_t>(g_world, 0x4aef)) return;
    for (int32_t i = 0; i < Collection_Count(cities); ++i)
        Obj_SumBuildings(Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0)));
    for (int32_t pass = 0; pass < 2; ++pass) {
        float s[6] = {};
        float div = 1.0f;
        int32_t n = Collection_Count(cities);
        if (n > 0) {
            for (int32_t i = 0; i < Collection_Count(cities); ++i) {
                uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
                if ((pass == 0 && F<int32_t>(c, 0x23e)) || (pass == 1 && !F<int32_t>(c, 0x23e))) continue;
                for (int32_t k = 0; k < 6; ++k) s[k] = (float)((double)s[k] + F<float>(c, 0x2b2 + k * 4));
            }
            div = Fl_200(0x5c85b4) > s[0] ? 1.0f : s[0];
        }
        float ratio = (float)((double)s[1] / div);
        double r = (double)ratio + D_200(0x5c8628);
        ratio = (float)(D_200(0x5c8680) > r ? 0.7 : r);
        float avg = Score(s[0], s[1], s[2], s[3], s[4], s[5], ratio);
        for (int32_t i = 0; i < Collection_Count(cities); ++i) {
            uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0));
            if ((pass == 0 && F<int32_t>(c, 0x23e)) || (pass == 1 && !F<int32_t>(c, 0x23e))) continue;
            int32_t industry = Obj_CountBuildingsOfKind(c, 0, 2);
            auto st = [&](int32_t k) { return F<float>(c, 0x2b2 + k * 4); };
            float q = Score(st(0), st(1), st(2), st(3), st(4), st(5), ratio);
            F<float>(c, 0x2e2) = q;
            if (q < Fl_200(0x5c9b08)) F<uint32_t>(c, 0x2e2) = 0xbf800000;
            float diff = (float)((double)F<float>(c, 0x2e2) - avg);
            F<float>(c, 0x2e6) = diff;
            float g = (float)(diff * D_200(0x5c87d8));
            F<float>(c, 0x2ea) = g;
            if (g > Fl_200(0x5c85b4)) F<uint32_t>(c, 0x2ea) = 0x3f800000;
            else if (g < D_200(0x5c9b00)) F<uint32_t>(c, 0x2ea) = 0xbf333333;
            float gr = (float)(F<float>(c, 0x2ea) * (F<int32_t>(c, 0x23e) ? 0.05 : 0.07));
            F<float>(c, 0x2ee) = gr;
            float& rate = F<float>(c, 0x2ee);
            float pop = F<float>(c, 0x25e);
            if (gr > Fl_200(0x5c8598) && pop > Fl_200(0x5c8c28))
                rate = (float)((double)gr / ((double)pop / D_200(0x5c85d8)) * D_200(0x5c8680) + (double)gr * D_200(0x5c88c8));
            if (rate < Fl_200(0x5c8598) && F<float>(c, 0x25e) < Fl_200(0x5c8c28))
                rate = (float)((double)F<float>(c, 0x25e) / D_200(0x5c85d8) * rate * D_200(0x5c8680) + (double)rate * D_200(0x5c88c8));
            if (industry == 0) {
                if (Fl_200(0x5c8598) > rate) rate = 0.0f;
            } else if (industry == 1)
                rate = (float)(rate * (rate > Fl_200(0x5c8598) ? D_200(0x5c8680) : D_200(0x5c8628)));
            if (!F<int32_t>(g_world, 0x46c38))
                F<float>(c, 0x25e) = (float)(((double)rate + D_200(0x5c8770)) * F<float>(c, 0x25e));
        }
    }
}
RT3_REPLACE(0x422320, Cities_Grow);

// 0x422850. Number of real cities with an active, ready station track-connected
// to another city, of the given company.
RT3_NOINLINE int32_t __fastcall Cities_CountConnected(Collection* c, int, int32_t company) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!F<int32_t>(e, 0x23e) && Region_HasConnectedStation(e, 0, 1, 1, 1, company)) ++n;
    }
    return n;
}
RT3_REPLACE(0x422850, Cities_CountConnected);

// 0x4228b0. Buildings in the chain from u16 +0x383 (next u16 +0x270) whose
// state kind (+0x32) equals `kind`.
RT3_NOINLINE int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind) {
    int32_t n = 0;
    for (uint16_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
        if (F<uint8_t>(st, 0x32) == kind) ++n;
    }
    return n;
}
RT3_REPLACE(0x4228b0, Obj_CountBuildingsOfKind);

// 0x422900. Counts the buildings of the chain +0x383 (only those without
// +0x29e unless `all`) by type kind 0..3, with their capacities; any output
// may be null. Returns the total count.
RT3_NOINLINE int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3,
                                     int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3, int32_t all) {
    int32_t total = 0, n[4] = {}, c[4] = {};
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        if (!all && F<int32_t>(b, 0x29e)) continue;
        int32_t cap = F<int32_t>(Obj_Ref3cc(b), 0x141);
        ++total;
        uint32_t kind = F<uint8_t>(Virtual80(b), 0x32);
        if (kind > 3) continue;
        ++n[kind];
        c[kind] += cap;
    }
    if (n0) *n0 = n[0];
    if (n1) *n1 = n[1];
    if (n2) *n2 = n[2];
    if (n3) *n3 = n[3];
    if (c0) *c0 = c[0];
    if (c1) *c1 = c[1];
    if (c2) *c2 = c[2];
    if (c3) *c3 = c[3];
    return total;
}
RT3_REPLACE(0x422900, Obj_BuildingStats);

// 0x422a70. Company co builds building type `id` at (x, y) turned by
// `angle` (in multiplayer with v1.02 peers: whole cells, no turn). It must
// be placeable and stand in the company's own city (+0x23a); with
// `checkOnly` only the placement is checked. In multiplayer (unless the
// world flag +0x46c3c) the build is queued for the network (*outId 0),
// otherwise the building is added (*outId its id). Returns 1 when it can.
// QUIRK: outId is written unchecked on success.
RT3_NOINLINE int32_t __fastcall Company_BuildBuilding(uint8_t* co, int, uint32_t id, float x, float y, uint32_t angle, int32_t checkOnly, uint32_t* outId) {
    if (outId) *outId = 0;
    if (IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f828f5c))) {
        angle = 0;
        x = (float)Ftol(x);
        y = (float)Ftol(y);
    }
    if (Sites_CanPlace(g_buildingTypes, 0, id, x, y, BitsF(angle), 0, 0, nullptr, 0, 1, -1, 0) != 1) return 0;
    if (checkOnly) return 1;
    int32_t yi = Ftol(y);
    uint16_t city = (uint16_t)Map_CellCity(g_map, 0, Ftol(x), yi);
    if ((int32_t)city != F<int32_t>(co, 0x23a)) return 0;
    if (IsMultiplayer() && !F<int32_t>(g_world, 0x46c3c)) {
        Queue6cd990_Push((uint16_t)F<int32_t>(co, 0x23a), (int32_t)id, *(int32_t*)&x, *(int32_t*)&y, (int32_t)angle);
        *outId = 0;
        return 1;
    }
    uint8_t* bt = (uint8_t*)Collection_Get(g_buildingTypes, 0, id);
    *outId = Buildings_Add(g_buildings, 0, bt, x, y, angle, 0, 1);
    return 1;
}
RT3_REPLACE(0x422a70, Company_BuildBuilding);

// 0x422be0. Picks the building of kind (state +0x32) to give up among o's
// buildings (list +0x383, next +0x270; idle +0x276, not locked +0x390) and
// returns the chosen one's type value (+0x141 of its type record). Kind 2 is
// scored by its last three years' weighted remaining (0x40cac0) over output
// (0x40ca90), randomised (a random 1..20 when under 3 years old, 1..5 when
// low); a building without state +0x790 instead gets a random 1..3, or about
// 1e9 one time in o+0x28e; other kinds score 1..1000. The best is removed (in
// multiplayer games, unless +0x46c3c, its id goes to the host as message 0x73).
RT3_NOINLINE int32_t __fastcall Owner_PickBuildingToSell(uint8_t* o, int, int32_t kind) {
    typedef uint8_t*(__thiscall * StateFn)(uint8_t*);
    uint32_t bestBits = 0xbf800000;
    int32_t bestId = -1;
    Collection* blds = g_buildings;
    for (uint32_t id = F<uint16_t>(o, 0x383); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(blds, 0, id);
        id = F<uint16_t>(b, 0x270);
        uint8_t* st = ((StateFn)(*(void***)b)[0x80 / 4])(b);
        if ((int32_t)st[0x32] != kind || F<uint32_t>(b, 0x276) != 0 || b[0x390]) continue;
        float score;
        if (kind == 2) {
            int32_t age = (int32_t)F<uint16_t>(g_world, 0xd) - F<int32_t>(b, 0x3d5) + 1;
            if (age < 0 || age < 3) {
                score = (float)SimRand(1, 0x14);
            } else {
                float acc = 0.0f, sum = 0.0f;
                if (age > 3) age = 3;
                int32_t w = 4;
                for (int32_t i = 1; i <= age; ++i) {
                    float fw = (float)w;
                    acc = (float)(Obj_Remaining(b, 0, i) * fw + acc);
                    sum = (float)(Obj_Float310(b, 0, i) * fw + sum);
                    --w;
                }
                float denom = (1.0f > sum) ? 1.0f : sum;
                score = (float)((double)acc / denom);
                if (*(const float*)0x5c9b18 > score) score = -3.0f;
                double r = (double)SimRand(0x12c, 0x3e8);
                score = (float)((*(const double*)0x5c9b10 - score) * r);
                if (score < 1.0f) score = (float)SimRand(1, 5);
            }
            uint8_t* st2 = ((StateFn)(*(void***)b)[0x80 / 4])(b);
            if (!F<int32_t>(st2, 0x790)) {
                int32_t n = F<int32_t>(o, 0x28e);
                if (n < 1) n = 1;
                if (SimRand(0, n) == 0) score = (float)(SimRand(0, 3) + 0x3b9ac9f6);
                else score = (float)(SimRand(0, 3) + 1);
            }
        } else {
            score = (float)SimRand(1, 0x3e8);
        }
        if (score > *(float*)&bestBits) {
            bestBits = *(uint32_t*)&score;
            bestId = F<int32_t>(b, 0x2a4);
        }
    }
    if (bestId <= 0) return 0;
    int32_t value = F<int32_t>(Obj_Ref3cc(Collection_Get(blds, 0, (uint32_t)bestId)), 0x141);
    if (IsMultiplayer() && !F<int32_t>(g_world, 0x46c3c)) {
        uint8_t* s = g_session;
        Net_Send(s, 0, (const char*)s + 0x8f48, 0x73, 4, (const uint8_t*)&bestId, 5, 0);
        return value;
    }
    uint8_t* c = hook::As<uint8_t*>(g_buildings);
    ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)c)[2])(c, bestId);
    return value;
}
RT3_REPLACE(0x422be0, Owner_PickBuildingToSell);

// 0x422ee0. City c builds a building of the class `name` (its era bit
// +0x272): up to 10 passes (15 with checkOnly or world flag +0x46c3c), each
// picking a type - weighted towards the era's typical year (0x5ee1e0 by era
// Bracket5(+0x25e) and `weighted`, which also changes the odds), or type year
// 1 within fixed bounds (+0x23e) for the first 9 tries - and then up to 35
// spots for it (350, 700 with +0x46c3c, for shore types): inside the city's
// block rect (+0x246..+0x252, own block +0x23a, at most 120 draws), on a ring
// for shaped types, or around the centre within a radius growing with the try
// (+0x256); quarter turns from a random start for the first 25 tries.
// Company_BuildBuilding decides; the next-to-last pass reseeds (0xa7) for
// shore types or with +0x46c3c. Returns the built type's year (+0x141), 0 if
// none.
RT3_NOINLINE int32_t __fastcall City_BuildIndustry(uint8_t* c, int, const char* name, int32_t weighted, int32_t checkOnly, uint32_t* outId) {
    float radius = (float)F<int32_t>(c, 0x256);
    if (outId) *outId = 0;
    int32_t era = Bracket5(Ftol(F<float>(c, 0x25e)));
    float typical = *(float*)(0x5ee1e0 + (weighted + era * 2) * 4);
    int32_t passes = checkOnly || F<int32_t>(g_world, 0x46c3c) ? 0xf : 10;
    int32_t shore = 0;
    for (int32_t pass = 1; pass <= passes; ++pass) {
        int32_t n = Table_CountNamedWithBit(g_buildingTypes, 0, name, F<int32_t>(c, 0x272));
        if (n <= 0) return 0;
        int32_t seeded = 0;
        if (pass == passes - 2 && (F<int32_t>(g_world, 0x46c3c) || shore)) {
            seeded = 1;
            PushSeed();
            SetSeed(0xa7);
        }
        uint32_t id;
        for (int32_t tries = 1;; ++tries) {
            int32_t r = n > 1 ? SimRand(0, n - 1) : 0;
            id = Table_NthNamedWithBit(g_buildingTypes, 0, name, F<int32_t>(c, 0x272), r + 1);
            int32_t year = F<int32_t>(Collection_Get(g_buildingTypes, 0, id), 0x141);
            if (F<int32_t>(c, 0x23e) && tries < 10) {
                if (year == 1) break;
            } else {
                float diff = (float)fabs((double)(float)year - typical);
                if (weighted) {
                    if (diff < D_207(0x5c8578)) break;
                    float w = (float)((D_207(0x5c85c8) - diff) / diff * D_207(0x5c85d8));
                    if (w > g_5c8598 && (float)SimRand(0, 100) < w) break;
                } else {
                    int32_t lim = Ftol(Fl_207(0x5c8c28) - (double)diff * Fl_207(0x5c8780));
                    if (tries > 0x14 && lim < 0x14) lim = 0x14;
                    if (SimRand(0, 100) < lim) break;
                }
            }
            if (tries >= 0x1e) break;
        }
        if (seeded) PopSeed();
        uint8_t* bt = (uint8_t*)Collection_Get(g_buildingTypes, 0, id);
        int32_t shaped = bt[0x123] ? 1 : 0;
        int32_t spots = 0x23;
        if (((uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(bt, 0x173)))[0xba]) {
            shore = 1;
            spots = F<int32_t>(g_world, 0x46c3c) ? 0x2bc : 0x15e;
        }
        float base = (float)((double)SimRand(0, 3) / D_207(0x5c87b8) * D_207(0x5c8c08));
        for (int32_t k = 1; k <= spots; ++k) {
            float fx, fy;
            if (F<int32_t>(c, 0x23e)) {
                int32_t xi, yi;
                for (int32_t j = 0;; ++j) {
                    if (j >= 0x78) return 0;
                    xi = SimRand(F<int32_t>(c, 0x246), F<int32_t>(c, 0x24e));
                    yi = SimRand(F<int32_t>(c, 0x24a), F<int32_t>(c, 0x252));
                    if ((int32_t)(uint16_t)Map_BlockCity(g_map, 0, xi, yi) == F<int32_t>(c, 0x23a)) break;
                }
                fx = (float)((double)SimRand(0, 0x27) / D_207(0x5c85f0) + (double)(xi * 4));
                fy = (float)((double)SimRand(0, 0x27) / D_207(0x5c85f0) + (double)(yi * 4));
            } else if (shaped) {
                int32_t hi = Ftol(radius * D_207(0x5c9b20));
                int32_t lo = Ftol(radius * D_207(0x5c87b8));
                float r = (float)((double)SimRand(lo, hi) / D_207(0x5c85f0));
                float a = (float)((double)SimRand(0, 0x3e8) / D_207(0x5c87f8) * D_207(0x5c8c08));
                double ox = crt::sin(a) * r;
                fx = (float)(Obj_CellX(c) + ox);
                double oy = crt::cos(a) * r;
                fy = (float)(Obj_CellY(c) + oy);
            } else {
                float rad = radius;
                if (k < 0x14) rad = (float)((double)(float)(k + 2) / Fl_207(0x5c8674) * radius);
                int32_t hi = Ftol(rad * Fl_207(0x5c85f8));
                int32_t lo = Ftol(rad * Fl_207(0x5c9b1c));
                int32_t a = SimRand(lo, hi);
                fx = (float)(Obj_CellX(c) + (double)a / D_207(0x5c85f0));
                int32_t b = SimRand(lo, hi);
                fy = (float)(Obj_CellY(c) + (double)b / D_207(0x5c85f0));
            }
            float angle;
            if (k <= 0x19) angle = (float)((double)(float)k * Fl_207(0x5c8760) * D_207(0x5c8628) + base);
            else angle = (float)((double)SimRand(0, 0x3e7) / D_207(0x5c87f8) * D_207(0x5c8c08));
            if (Company_BuildBuilding(c, 0, id, fx, fy, *(uint32_t*)&angle, checkOnly, outId)) return F<int32_t>(bt, 0x141);
        }
    }
    return 0;
}
RT3_REPLACE(0x422ee0, City_BuildIndustry);

// 0x4234e0. Production rate of slot i, from the type tables 0x5ee1b0 /
// 0x5ee138, the era tables 0x5f33e0 / 0x5f33cc and size +0x25e.
RT3_NOINLINE float __fastcall Obj_ProductionRate(void* o, int, int32_t i) {
    const float* typeTbl = (const float*)0x5ee1b0;
    int32_t era = F<int32_t>(g_world, 0x2d);
    int32_t type = F<int32_t>(o, 0x23e);
    if (type) {
        double v = (double)typeTbl[type + i * 2] * ((const float*)0x5f33e0)[era];
        v = (double)F<float>(o, 0x25e) / D(0x5c85d8) * v;
        v = v * F<float>(g_world, 0xbde);
        v = v * (double)F<int32_t>(o, 0x242);
        return (float)(v * D(0x5c8918));
    }
    int32_t b = Bracket5((int32_t)F<float>(o, 0x25e));
    double v = (double)((const float*)0x5ee138)[b + i * 5] * typeTbl[F<int32_t>(o, 0x23e) + i * 2];
    v = v * ((const float*)0x5f33cc)[era];
    v = (double)F<float>(o, 0x25e) / D(0x5c85d8) * v;
    v = v * F<float>(g_world, 0xbde);
    return (float)(v * D(0x5c8d68));
}
RT3_REPLACE(0x4234e0, Obj_ProductionRate);

// 0x4235c0. City c grows over dt (`fast` without the random parts; nothing
// in the editor, 0x6cec74+0x68, or for a block city without blocks, +0x23e /
// +0x242). For classes 2, 3, 0 (industry, commerce, housing): production
// (Obj_ProductionRate) against the target count (Obj_BuildingStats) gives
// how many surplus buildings to sell (Owner_PickBuildingToSell) and how many
// to build - scaled by dt over the period (9, or dt >= 1/12 with `fast` or
// 0x6cec74+0x168), damped near the target, rounded at random. A free city's
// first build flattens the land around it (+0x385). Each build tries up to
// 250 types - by era name for commerce, "House", or a weighted pick of its
// industries (Obj_Entry37f, kept by chance with several available) - that
// suit the class and year (+0x26, industries need +0x7ac) and, outside block
// cities, are not over-represented (+0xbc, 4 / 50 retries before a repeat);
// a new industry type is announced once (+0x7b0). Then the building weights
// are summed.
void __fastcall City_Grow(uint8_t* c, int, float dt, int32_t fast) {
    if (F<int32_t>(g_app, 0x68)) return;
    if (F<int32_t>(c, 0x23e) && F<int32_t>(c, 0x242) <= 0) return;
    float period = 9.0f;
    if (fast || F<int32_t>(g_app, 0x168)) {
        double p = dt > D_207(0x5c9b40) ? (double)dt : 0.08333333333333333;
        period = (float)p;
    }
    int32_t n0, n1, n2, n3, c0, c1, c2, c3;
    Obj_BuildingStats(c, 0, &n0, &n1, &n2, &n3, &c0, &c1, &c2, &c3, 0);
    static const int32_t kClasses[3] = { 2, 3, 0 };
    int32_t pumps = 0;
    int32_t entries = Obj_Count37f(c);
    int32_t avail = Obj_CountTypesAvailable(c, 0, F<uint16_t>(g_world, 0xd), 2);
    int32_t target = 0;
    for (int32_t k = 0; k < 3; ++k) {
        int32_t cls = kClasses[k];
        if (cls == 2 && !avail) continue;
        float cur = Obj_ProductionRate(c, 0, cls);
        if (cls == 2) target = c2;
        else if (cls == 3) target = c3;
        else if (cls == 0) target = c0;
        float ftarget = (float)target;
        float lack = 0.0f;
        if (ftarget > cur) lack = (float)(D_207(0x5c8770) - (double)cur / (float)(target < 1 ? 1 : target));
        if (fabs((double)cur - ftarget) <= D_207(0x5c9b38)) lack = (float)(lack * D_207(0x5c8c20));
        float rate = (float)((double)dt / period);
        float x = (float)((double)rate * lack);
        if (!fast) x = (float)((double)dt * D_207(0x5c8680) / D_207(0x5c85d8) + x);
        x = (float)((double)ftarget * x);
        int32_t sell = Ftol((double)x);
        x = (float)((double)x - (float)sell);
        if (!fast && (double)x > (double)SimRand(0, 0x7530) / D_207(0x5c9b30)) ++sell;
        if (sell >= target) sell = target;
        for (; sell > 0; --sell) {
            int32_t got = Owner_PickBuildingToSell(c, 0, cls);
            target -= got < 1 ? 1 : got;
        }
        float gap = (float)((double)cur - (float)target);
        float want = (float)((double)rate * gap);
        if (want < g_5c8598) want = 0.0f;
        if (fabs((double)gap) <= D_207(0x5c9b38) && !F<int32_t>(g_world, 0x46c38) &&
            !F<int32_t>(g_world, 0x46c3c)) {
            if (want > Fl_207(0x5c8568)) want = (float)(want * D_207(0x5c8c20));
            else if (want > D_207(0x5c87d8)) want = (float)(want * D_207(0x5c8628));
            else want = (float)(want * D_207(0x5c8da8));
        }
        if (!fast) want = (float)((double)cur * dt * D_207(0x5c9b28) + want);
        int32_t build = Ftol((double)want);
        float frac = (float)((double)want - (float)build);
        if (!fast && (double)frac > (double)SimRand(0, 0x2710) / D_207(0x5c8d18)) ++build;
        uint8_t* filter = hook::As<uint8_t*>(g_buildingTypes);
        if (!F<int32_t>(c, 0x23e)) {
            if (F<int32_t>(c, 0x385) == 1) {
                F<int32_t>(c, 0x385) = 0;
                int32_t cy = Ftol(Obj_CellY(c));
                int32_t cx = Ftol(Obj_CellX(c));
                TerrainTool_Flatten(hook::As<uint8_t*>(g_buildingTypes), 0, cx, cy, 0xa);
                BuildingFilter_Reset(hook::As<uint8_t*>(g_buildingTypes), 1);
            } else {
                F<int32_t>(filter, 0x9c) = 1;
            }
        } else {
            BuildingFilter_Reset(filter, 0);
        }
        while (build > 0) {
            if ((pumps++ & 3) == 3) Loading_Pump(g_app, 0, 0, 0);
            char name[0x20];
            uint8_t* ty = nullptr;
            bool found = false;
            for (int32_t tries = 0; tries < 0xfa;) {
                ++tries;
                int32_t limit = -1;
                if (cls == 3) {
                    limit = 4;
                    Types_PickName(g_62b268, 0, name, Ftol((double)F<float>(c, 0x25e)));
                } else if (cls == 0) {
                    memcpy(name, (const void*)0x5c9114, 6);
                } else if (cls == 2) {
                    limit = 0x32;
                    float weight;
                    Obj_Entry37f(c, 0, SimRand(1, entries), name, (int32_t*)&weight);
                    if (avail > 1 && weight < (double)SimRand(0, 1000) / D_207(0x5c87f8)) continue;
                }
                ty = (uint8_t*)Table_GetByName(g_62b268, 0, name);
                if (!ty || cls != ty[0x32] || F<uint16_t>(ty, 0x26) > F<uint16_t>(g_world, 0xd)) continue;
                if (ty[0x32] == 2 && !F<int32_t>(ty, 0x7ac)) continue;
                if (!F<int32_t>(c, 0x23e) && cls != 0) {
                    int32_t kind = F<int32_t>(ty, 0xe4), same = 0;
                    for (uint32_t id = F<uint16_t>(c, 0x383); id;) {
                        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
                        if (F<int32_t>(b, 0x3d0) == kind) ++same;
                        id = F<uint16_t>(b, 0x270);
                    }
                    if (tries < limit && same > 0) continue;
                    if (F<int32_t>(ty, 0xbc) > 0 && same >= F<int32_t>(ty, 0xbc)) continue;
                }
                found = true;
                break;
            }
            if (!found) break;
            uint32_t newId = 0;
            int32_t r = City_BuildIndustry(c, 0, name, cls == 0, 0, &newId);
            build -= r < 1 ? 1 : r;
            uint8_t* t2 = (uint8_t*)Table_GetByName(g_62b268, 0, name);
            if (r > 0 && t2[0x32] == 2 && !F<int32_t>(t2, 0x7b0) && !t2[0xba] && !t2[0xbb]) {
                if (!F<int32_t>(g_world, 0x46c3c) && !IsMultiplayer())
                    World_AddNamedEntry(hook::As<void*>(g_world), 0, (const char*)0x5c87a8, 8, 0, F<int32_t>(t2, 0xe4), (int32_t)newId, -1, -1);
                F<int32_t>(t2, 0x7b0) = 1;
            }
        }
        BuildingFilter_Reset(hook::As<uint8_t*>(g_buildingTypes), 0);
    }
    Obj_SumBuildingWeights(c);
}
RT3_REPLACE(0x4235c0, City_Grow);

// 0x423d30. Building counts of kinds 0, 2 and 3 into +0x27a/+0x27e/+0x282; total into +0x286.
RT3_NOINLINE void __fastcall Obj_CountBuildings(uint8_t* o) {
    F<int32_t>(o, 0x286) = Obj_BuildingStats(o, 0, &F<int32_t>(o, 0x27a), nullptr, &F<int32_t>(o, 0x27e),
                                             &F<int32_t>(o, 0x282), nullptr, nullptr, nullptr, nullptr, 1);
}
RT3_REPLACE(0x423d30, Obj_CountBuildings);

// 0x423d70. Removes entry idx of the 12-byte array +0x5f (count +0x5b),
// booking its amount (stats 0x12 / 0xd, and 0xc with cash when `post`).
RT3_NOINLINE void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post) {
    if (idx < 0 || idx >= F<int32_t>(c, 0x5b)) return;
    uint8_t* e = (uint8_t*)c + 0x5f + idx * 12;
    Company_AddStat(c, 0, 0x12, (float)F<int32_t>(e, 0));
    Company_AddStat(c, 0, 0xd, (float)(int32_t)(0u - F<uint32_t>(e, 0)));
    if (post) {
        double neg = (double)(int32_t)(0u - F<uint32_t>(e, 0));
        Company_PostStat(c, 0, 0xc, (float)(neg * D(0x5c8918)), 1);
    }
    int32_t n = F<int32_t>(c, 0x5b);
    if (n > idx + 1) {
        uint8_t* src = e + 12;
        for (int32_t k = 0; k < (n - idx - 1) * 12; ++k) e[k] = src[k];
    }
    F<int32_t>(c, 0x5b) = n - 1;
}
RT3_REPLACE(0x423d70, Company_RemoveEntry);

// 0x423e40. Weighted average over {int count, ?, float value} entries
// (+0x5f, count +0x5b), accumulated in float; 0 if inactive (+0x3f) or empty.
RT3_NOINLINE double __fastcall Obj_WeightedAverage(void* o) {
    int32_t n = F<int32_t>(o, 0x5b);
    if (F<int32_t>(o, 0x3f) == 0 || n == 0) return *(const float*)0x5c8598;
    float sum = 0.0f;
    int32_t total = 0;
    uint8_t* e = (uint8_t*)o + 0x5f;
    for (int32_t i = 0; i < n; ++i, e += 0xc) {
        int32_t c = F<int32_t>(e, 0);
        total += c;
        float fc = (float)c;
        sum = (float)((double)fc * F<float>(e, 8) + sum);
    }
    float ft = (float)total;
    return (double)sum / ft;
}
RT3_REPLACE(0x423e40, Obj_WeightedAverage);

RT3_NOINLINE double __fastcall Company_FloatD7b(void* c) { return F<float>(c, 0xd7b); }
RT3_REPLACE(0x423eb0, Company_FloatD7b);

// 0x423ec0. Adds to +0x7680 (unless -1), not below 0.
RT3_NOINLINE void __fastcall Company_Add7680(void* c, int, int32_t d) {
    int32_t v = F<int32_t>(c, 0x7680);
    if (v == -1) return;
    v += d;
    F<int32_t>(c, 0x7680) = v < 0 ? 0 : v;
}
RT3_REPLACE(0x423ec0, Company_Add7680);

// 0x423ef0. Revenue bookkeeping: amount `a`, units `b` at price `c`, into
// slot `idx` and the totals; `flag` selects the +0x32f or +0x33b group.
void __fastcall Obj_AddRevenue(void* o, int, float a, float b, float c, int32_t flag, int32_t idx) {
    F<float>(o, 0x24f + idx * 4) = (float)((double)a + F<float>(o, 0x24f + idx * 4));
    F<float>(o, 0x323) = (float)((double)a + F<float>(o, 0x323));
    F<float>(o, 0x327) = (float)((double)b + F<float>(o, 0x327));
    float bc = (float)((double)b * c);
    F<float>(o, 0x32b) = (float)((double)bc + F<float>(o, 0x32b));
    uint32_t g = flag ? 0x32f : 0x33b;
    F<float>(o, g)     = (float)((double)a + F<float>(o, g));
    F<float>(o, g + 4) = (float)((double)b + F<float>(o, g + 4));
    F<float>(o, g + 8) = (float)((double)bc + F<float>(o, g + 8));
}
RT3_REPLACE(0x423ef0, Obj_AddRevenue);

RT3_NOINLINE void __fastcall Obj_Set4b(void* o, int, int32_t v) { F<int32_t>(o, 0x4b) = v; }
RT3_REPLACE(0x423fb0, Obj_Set4b);

void __fastcall Obj_Nop423fc0(void*) { __asm nop }
RT3_REPLACE(0x423fc0, Obj_Nop423fc0);

void __fastcall Obj_IncD0f(void* o) { F<int32_t>(o, 0xd0f)++; }
RT3_REPLACE(0x423fd0, Obj_IncD0f);

void __fastcall Obj_IncD0b(void* o) { F<int32_t>(o, 0xd0b)++; }
RT3_REPLACE(0x423fe0, Obj_IncD0b);

RT3_NOINLINE void __stdcall SetGlobal62bae8(uint8_t v) { g_62bae8 = v; }
RT3_REPLACE(0x423ff0, SetGlobal62bae8);

}  // namespace rt3
