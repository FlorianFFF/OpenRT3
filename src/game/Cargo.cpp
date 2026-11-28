// Cargo.cpp - RT3.exe 0x41e000-0x420000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/imports.h"
#include "hook/keepregs.h"
#include "game/calendar.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

Collection* __fastcall Collection_SetVtbl(Collection* c);

void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                int32_t initialCapacity, int32_t growBy,
                                int32_t list1, int32_t list2, int32_t list3);

void __fastcall CarTypes_LoadAll(Collection* t);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_YearReached(void* o);

 // cargo-flow / demand matrix

void* __fastcall Table_GetByName(Collection* t, int, const char* name);
int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s);
int32_t __fastcall Collection_Load(Collection* c, int, int32_t s, int32_t recordSize);
int32_t Global6d07b4Set2();
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
void __fastcall NamedObj_Destructor(uint8_t* o);
int32_t __stdcall Map_FogEnd(int32_t x, int32_t y);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Obj_Available(void* o);
int32_t __fastcall Table_SumCapacity(Collection* t, int, int32_t v);

void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare);

const char* __fastcall GetString(int32_t id);
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);

void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d,
                                    int32_t e, int32_t f);

void __fastcall Cargo_UpdateAverages(uint8_t* o);
void __fastcall CargoTypes_Recount(Collection* t);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

void __fastcall StrCopyN(char* dst, const char* src, size_t n);
const char* __fastcall LocalizeName(const char* name);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
int __fastcall ToUpper(char c);
int32_t __fastcall Game_EffectiveVersion();

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall EditorDialog_Run(int32_t tab, int32_t city);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall Chain_Unlink(uint8_t* o, int, uint8_t* r);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
template <typename T> static inline T& F_runtime_wrappers(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

// Functions of this file, in exe order.
void __fastcall CarTypes_AnnounceNew(Collection* c);
uint32_t __fastcall Table_FindCurrentFor(Collection* t, int, int32_t v);
RT3_NOINLINE uint8_t* __fastcall CarTypes_ConstructRuntime(uint8_t* table);
void __fastcall Cargo_MoveTo66(void* c, int, float a);
RT3_NOINLINE void __fastcall Cargo_MoveTo62(void* c, int, float a);
void __fastcall Cargo_Produce(void* c, int, float a, float price);
RT3_NOINLINE int32_t __fastcall Obj_YearReached(void* o);
RT3_NOINLINE int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
void __fastcall Cargo_UpdateAverages(uint8_t* o);
uint32_t __fastcall CarType_ForCargo(uint8_t* o);
RT3_NOINLINE double __fastcall Cargo_DemandWeight(void* c);
RT3_NOINLINE void __fastcall Cargo_IndustryTotals(uint8_t* o, int, float* base, float* share, float* in, float* inShare);
RT3_NOINLINE void __fastcall Collection5c997c_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Collection_LoadTagged6979(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveTagged6979(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall CargoTypes_Recount(Collection* t);
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
RT3_NOINLINE void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all);
RT3_NOINLINE void __fastcall CargoTypes_MarkAvailable(Collection* c);
void __fastcall CargoTypes_Load(Collection* c);
RT3_NOINLINE Collection* __fastcall CargoTypes_Construct(Collection* c);
RT3_NOINLINE void __fastcall CargoTypes_Reload(Collection* c);
RT3_NOINLINE uint8_t* __fastcall NamedObj41f590_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall NamedObj41f590_Destructor(uint8_t* o);
int32_t __fastcall City_Load(uint8_t* c, int, int32_t s);
void __fastcall NamedObj_SaveWithList(uint8_t* o, int, Stream s);
void __fastcall CityObj_Select(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_Count37f(void* o);
int32_t __fastcall Obj_FogEnd(void* o);
RT3_NOINLINE int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_Byte42_41f6f0(uint8_t* o);
int32_t __fastcall Return0_41f700(void*);
int32_t __fastcall Return1_41f710(void*);
void __fastcall Obj_ShowCargoTooltip(uint8_t* o);
void Tooltip_ClearAndCloseOverview();
void __fastcall Obj_PushBuilding(void* o, int, void* b);
void* __fastcall Obj_FirstBuilding(void* o);
RT3_NOINLINE void __fastcall Chain_Unlink(uint8_t* o, int, uint8_t* r);
void __fastcall Obj_UnlinkBuildings(void* o);
int32_t __fastcall Obj_Visible41f910(uint8_t* o);
int32_t __fastcall Obj_CountFlagged7ac(void* o);
RT3_NOINLINE int32_t __fastcall Obj_CountTypesAvailable(void* o, int, int32_t year, int32_t kind);
RT3_NOINLINE double __fastcall Obj_NamedValue(void* o, int, const char* name);
RT3_NOINLINE void __fastcall Obj_Entry37f(void* o, int, int32_t i, char* dst, int32_t* out);
void __fastcall City_LayStreets(uint8_t* c);
RT3_NOINLINE int32_t Option2bbActive();
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall SceneObj_ShowPreview(uint8_t* o);
RT3_NOINLINE void __fastcall EditorDialog_Run(int32_t tab, int32_t city);


// Fog end at the object's cell (Obj_CellX / Obj_CellY truncated).
static int32_t ObjFogEnd(void* o) {
    int32_t cellX, cellY;
    __asm {
        mov ecx, o
        call Obj_CellY
        call Rt3_Ftol
        mov cellY, eax
        mov ecx, o
        call Obj_CellX
        call Rt3_Ftol
        mov cellX, eax
    }
    return Map_FogEnd(cellX, cellY);
}

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline float Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline uint8_t* World() { return g_world; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// Marks fine cell (x, y) (quarter cells) as street unless off the map or on water.
static void Street(int32_t x, int32_t y) {
    uint8_t* m = g_map;
    if (x < 0 || x >= F<int32_t>(m, 0x215d) || y < 0 || y >= F<int32_t>(m, 0x2161)) return;
    if (Terrain_KindIs1Or3to5(m, 0, x / 4, y / 4)) return;
    Map_SetFineBit(g_map, 0, x, y, 1, 3, 1);
}

static inline double Fl_202(uint32_t a) { return *(const float*)rt3::Rdata(a); }

namespace {
int32_t Tooltip_Clear() { return Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1); }
// A network message's handler gets the packet (+8) when it has one (+0xc).
template <typename P, typename Fn> int32_t NetMsg_Forward(uint8_t* m, Fn handler) {
    if (F<int32_t>(m, 0xc) > 0) handler(F<P>(m, 8));
    return 1;
}
}

// 0x41e010. In 1904 (first year) posts the news 0xcb/0xcc; from 1830 on announces
// the cargo cars (kind 2) introduced this year that the player can use.
void __fastcall CarTypes_AnnounceNew(Collection* c) {
    uint8_t* w = World();
    if (F<int32_t>(w, 0x46c38)) return;
    if (F<uint16_t>(w, 0xd) == 0x770 && F<uint16_t>(w, 5) < 0x770) {
        const char* text = GetString(0xcc);
        News_Add(g_news, 0, GetString(0xcb), text, 0, 0);
        w = World();
    }
    if (F<uint16_t>(w, 0xd) < 0x726) return;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x105) != F<uint16_t>(World(), 0xd) || F<int32_t>(e, 0x41) != 2) continue;
        int32_t player = Obj_Get25(World());
        if (!CarType_Available(e, 0, Obj_Get21(World()), player)) continue;
        World_AddNamedEntry(World(), 0, (const char*)0x5c87a8, 5, 0, F<int32_t>(e, 0x126), -1, -1, -1);
    }
}
RT3_REPLACE(0x41e010, CarTypes_AnnounceNew);

// 0x41e100. ID of the record with +0x41 == v whose years [+0x105, +0x109] contain now; 0 if none.
uint32_t __fastcall Table_FindCurrentFor(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x41) != v) continue;
        int32_t year = F<uint16_t>(g_world, 0xd);
        if (year >= F<int32_t>(r, 0x105) && year <= F<int32_t>(r, 0x109)) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x41e100, Table_FindCurrentFor);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x41e170. Build a 0x266-byte inline-record car-type table and load its files.
RT3_NOINLINE uint8_t* __fastcall CarTypes_ConstructRuntime(uint8_t* table) {
    Collection_SetVtbl((Collection*)(table));
    F_runtime_wrappers<uint32_t>(table, 0) = 0x5c9824;
    Collection_Init((Collection*)(table), 0, 1, 0x266, 10, 5, 1, 0, 0);
    CarTypes_LoadAll((Collection*)(table));
    return table;
}
RT3_REPLACE(0x41e170, CarTypes_ConstructRuntime);

// 0x41e1b0 / 0x41e1d0 / 0x41e1f0. Cargo amount bookkeeping (floats).
void __fastcall Cargo_MoveTo66(void* c, int, float a) {
    F<float>(c, 0x5a) = (float)((double)F<float>(c, 0x5a) - a);
    F<float>(c, 0x66) = (float)((double)a + F<float>(c, 0x66));
}
RT3_REPLACE(0x41e1b0, Cargo_MoveTo66);

RT3_NOINLINE void __fastcall Cargo_MoveTo62(void* c, int, float a) {
    F<float>(c, 0x5a) = (float)((double)F<float>(c, 0x5a) - a);
    F<float>(c, 0x62) = (float)((double)a + F<float>(c, 0x62));
}
RT3_REPLACE(0x41e1d0, Cargo_MoveTo62);

void __fastcall Cargo_Produce(void* c, int, float a, float price) {
    F<float>(c, 0x5a) = (float)((double)a + F<float>(c, 0x5a));
    F<float>(c, 0x5e) = (float)((double)a + F<float>(c, 0x5e));
    F<float>(c, 0x76) = (float)((double)a * price + F<float>(c, 0x76));
    F<float>(c, 0x7a) = (float)((double)a + F<float>(c, 0x7a));
}
RT3_REPLACE(0x41e1f0, Cargo_Produce);

// 0x41e220. Enabled (+0x56) and its year (+0x21) reached: compared with world
// +0x05 when world +0x46c38 is set, else with the current year (+0x0d).
RT3_NOINLINE int32_t __fastcall Obj_YearReached(void* o) {
    if (F<int32_t>(o, 0x56) == 0) return 0;
    int32_t year = F<int32_t>(g_world, 0x46c38) ? F<uint16_t>(g_world, 0x5) : F<uint16_t>(g_world, 0xd);
    return year >= F<int32_t>(o, 0x21) ? 1 : 0;
}
RT3_REPLACE(0x41e220, Obj_YearReached);

// 0x41e260. Cargo accepts class `id`: own class (+0x3e), 900 = any, 901 = plain
// freight (none of the passenger/mail/troop flags +0x47..+0x49), 902 = any of them.
RT3_NOINLINE int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id) {
    if (F<int32_t>(c, 0x3e) == id || id == 900) return 1;
    bool special = F<uint8_t>(c, 0x47) || F<uint8_t>(c, 0x48) || F<uint8_t>(c, 0x49);
    if (id == 901) return special ? 0 : 1;
    if (id == 902) return special ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x41e260, Cargo_MatchesClass);

// 0x41e2b0. Once the cargo (+0x56) exists (year +0x21), refreshes its averages over
// every third city: quota (+0x6a), quota weighted by stock (+0x6e) and by base price
// (+0x72) (falling back to the plain average), and decays the trend pair +0x76 / +0x7a.
RT3_NOINLINE void __fastcall Cargo_UpdateAverages(uint8_t* o) {
    if (!F<int32_t>(o, 0x56)) return;
    uint8_t* w = World();
    int32_t year = F<int32_t>(w, 0x46c38) ? F<uint16_t>(w, 5) : F<uint16_t>(w, 0xd);
    if (year < F<int32_t>(o, 0x21)) return;
    uint8_t* m = g_map;
    int32_t n = F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145);
    uint8_t* city = F<uint8_t*>(m, 0x2129);
    F<float>(o, 0x6a) = 0.0f;
    F<float>(o, 0x6e) = 0.0f;
    F<float>(o, 0x72) = 0.0f;
    float sumS = 0.0f, sumW = 0.0f;
    if (n > 0) {
        for (uint32_t cnt = (uint32_t)(n - 1) / 3 + 1; cnt; --cnt, city += 0x585) {
            int32_t c = F<int32_t>(o, 0x3e);
            float q = (float)City_CargoQuota(city, 0, c);
            float stock = F<float>(city, 0x103 + c * 4);
            float wt = (float)F<uint16_t>(city, 0x6a + c * 2);
            F<float>(o, 0x6a) = (float)((double)q + F<float>(o, 0x6a));
            if (stock != Fl(0x5c8598)) {
                F<float>(o, 0x6e) = (float)((double)stock * q + F<float>(o, 0x6e));
                sumS = (float)((double)stock + sumS);
            }
            if (wt != Fl(0x5c8598)) {
                F<float>(o, 0x72) = (float)((double)wt * q + F<float>(o, 0x72));
                sumW = (float)((double)wt + sumW);
            }
        }
    }
    float avg = (float)((double)F<float>(o, 0x6a) / (float)(n >= 1 ? n : 1));
    F<float>(o, 0x6a) = avg;
    auto norm = [&](uint32_t off, float sum) {
        if (!((double)sum > D(0x5c86b8))) {
            F<float>(o, off) = avg;
            return;
        }
        double d = (double)sum < D(0x5c86b8) ? 0.1 : (double)sum;
        F<float>(o, off) = (float)((double)F<float>(o, off) / d);
    };
    norm(0x6e, sumS);
    norm(0x72, sumW);
    float a = (float)((double)F<float>(o, 0x76) * D(0x5c8880));
    F<float>(o, 0x76) = a;
    float b = (float)((double)F<float>(o, 0x7a) * D(0x5c8880));
    F<float>(o, 0x7a) = b;
    if ((double)b < D(0x5c86b8)) {
        F<float>(o, 0x7a) = (float)((double)b + D(0x5c85b8));
        F<float>(o, 0x76) = (float)((double)avg * D(0x5c85b8) + a);
    }
}
RT3_REPLACE(0x41e2b0, Cargo_UpdateAverages);

// 0x41e500. The available freight car type (kind 1) for cargo name +0x25 (troops ride
// passenger cars); else the first freight car type.
uint32_t __fastcall CarType_ForCargo(uint8_t* o) {
    char name[0x12c];
    strcpy(name, (const char*)o + 0x25);
    uint32_t first = 0;
    for (;;) {
        Collection* types = g_carTypes;
        for (int32_t i = 0; i < Collection_Count(types); ++i) {
            uint8_t* e = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, i, 0));
            if (F<int32_t>(e, 0x41) != 1) continue;
            if (!first) first = Collection_IdAt(types, 0, i, 0);
            if (CarType_Available(e, 0, 0, 0) && !strcmp((const char*)e + 0x111, name))
                return Collection_IdAt(types, 0, i, 0);
        }
        if (crt::stricmp(name, (const char*)0x5c9974)) return first;
        memcpy(name, (const void*)0x5c9968, 10);
    }
}
RT3_REPLACE(0x41e500, CarType_ForCargo);

// 0x41e650. Cargo demand weight: +0x5a, or for cargoes flagged +0x46 the number
// of open demand entries for its type (0x6ada90: type byte +0x10, flag +0x12
// bit 1, u16 +8 == 0) divided by 5.
RT3_NOINLINE double __fastcall Cargo_DemandWeight(void* c) {
    if (!F<uint8_t>(c, 0x46)) return F<float>(c, 0x5a);
    int32_t type = F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x9a + F<int32_t>(c, 0x3e) * 4);
    int32_t total = Collection_Count(g_6ada90), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* d = (uint8_t*)Collection_Get(g_6ada90, 0, Collection_IdAt(g_6ada90, 0, i, 0));
        if (F<uint8_t>(d, 0x10) == type && (F<uint8_t>(d, 0x12) & 2) && F<uint16_t>(d, 8) == 0) ++n;
    }
    float fn = (float)n;
    return (double)fn / *(const float*)rt3::Rdata(0x5c8554);
}
RT3_REPLACE(0x41e650, Cargo_DemandWeight);

// 0x41e6e0. The cargo's industry totals (base, share, input, input share), summed
// over the available industries weighted by their capacity; cached per day (+0x8a,
// values +0x8e..+0x9a).
RT3_NOINLINE void __fastcall Cargo_IndustryTotals(uint8_t* o, int, float* base, float* share, float* in, float* inShare) {
    if (F<int32_t>(o, 0x8a) == F<int32_t>(World(), 0x15)) {
        *(uint32_t*)base = F<uint32_t>(o, 0x8e);
        *(uint32_t*)share = F<uint32_t>(o, 0x92);
        *(uint32_t*)in = F<uint32_t>(o, 0x96);
        *(uint32_t*)inShare = F<uint32_t>(o, 0x9a);
        return;
    }
    *inShare = 0.0f;
    *in = 0.0f;
    *share = 0.0f;
    *base = 0.0f;
    for (int32_t i = 0; i < Collection_Count(g_62b268); ++i) {
        uint32_t id = Collection_IdAt(g_62b268, 0, i, 0);
        uint8_t* ind = (uint8_t*)Collection_Get(g_62b268, 0, id);
        if (!Obj_Available(ind)) continue;
        float v[4];
        Industry_CargoStats(ind, 0, F<int32_t>(o, 0x3e), 0, &v[0], &v[1], &v[2], &v[3]);
        if (!(v[0] > D(0x5c85b8) || v[1] > D(0x5c85b8) || v[2] > D(0x5c85b8) || v[3] > D(0x5c85b8))) continue;
        int32_t cnt = Table_SumCapacity(g_buildings, 0, id);
        if (cnt <= 0) continue;
        float cf = (float)cnt;
        *base = (float)((double)cf * v[0] + *base);
        *share = (float)((double)cf * v[1] + *share);
        *in = (float)((double)cf * v[2] + *in);
        *inShare = (float)((double)cf * v[3] + *inShare);
    }
    F<int32_t>(o, 0x8a) = F<int32_t>(World(), 0x15);
    F<float>(o, 0x8e) = *base;
    F<float>(o, 0x92) = *share;
    F<float>(o, 0x96) = *in;
    F<float>(o, 0x9a) = *inShare;
}
RT3_REPLACE(0x41e6e0, Cargo_IndustryTotals);

RT3_NOINLINE void __fastcall Collection5c997c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c997c;
    Collection_Destruct(c);
}
RT3_REPLACE(0x41e8d0, Collection5c997c_Destruct);

// 0x41e8e0 / 0x41e930. Load / save framed by tags 0x6979/0x697a.
RT3_NOINLINE int32_t __fastcall Collection_LoadTagged6979(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x6979, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    return StreamSkip4(s, 0x697a, 0, 0) + n;
}
RT3_REPLACE(0x41e8e0, Collection_LoadTagged6979);

RT3_NOINLINE void __fastcall Collection_SaveTagged6979(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x6979);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x697a);
}
RT3_REPLACE(0x41e930, Collection_SaveTagged6979);

// 0x41e970. Recomputes the cargo-type availability arrays (53 types):
// +0x246[i] = type +0x56 (enabled), +0x16e[i] = its year reached; with counts
// +0x31a and +0x242.
RT3_NOINLINE void __fastcall CargoTypes_Recount(Collection* t) {
    uint8_t* o = (uint8_t*)t;
    F<int32_t>(o, 0x242) = 0;
    F<int32_t>(o, 0x31a) = 0;
    for (uint32_t i = 0; i < 0x35; ++i) {
        int32_t& enabled = F<int32_t>(o, 0x246 + i * 4);
        int32_t& reached = F<int32_t>(o, 0x16e + i * 4);
        if (!Collection_IsValid(t, 0, i)) {
            reached = 0;
            enabled = 0;
            continue;
        }
        enabled = F<int32_t>(Collection_Get(t, 0, i), 0x56);
        if (enabled) F<int32_t>(o, 0x31a)++;
        reached = Obj_YearReached(Collection_Get(t, 0, i));
        if (reached) F<int32_t>(o, 0x242)++;
    }
}
RT3_REPLACE(0x41e970, CargoTypes_Recount);

// 0x41e9f0. ID of the record whose name (+4) equals `name` (exe stricmp), or 0.
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (crt::stricmp(name, (const char*)r + 4) == 0)
            return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x41e9f0, Table_FindByName);

// 0x41ea50. Updates the cargo averages: all of them, or every sixth by month.
RT3_NOINLINE void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all) {
    int32_t phase = World()[0xf] % 6;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (all || i % 6 == phase) Cargo_UpdateAverages((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x41ea50, CargoTypes_UpdateAverages);

// 0x41eac0. Marks which cargos exist in the game (+0x56): those always present
// (+0x47/+0x48), troops with barracks (+0x49), or produced by an active industry
// (recipes without inputs; waste for recycling plants; ports/warehouses only where a
// city links them); all with option +0x25f; only six goods in the scenario mode
// world +0x4afb. Then recounts.
RT3_NOINLINE void __fastcall CargoTypes_MarkAvailable(Collection* c) {
    if (!g_62b268) return;
    uint8_t flags[0x3e8];
    memset(flags, 0, sizeof flags);
    int32_t barracks = 0;
    for (int32_t i = 0; i < Collection_Count(g_62b268); ++i) {
        Collection* inds = g_62b268;
        uint8_t* ind = (uint8_t*)Collection_Get(inds, 0, Collection_IdAt(inds, 0, i, 0));
        if (ind[0x32] != 2 || !F<int32_t>(ind, 0x7ac)) continue;
        if ((ind[0xba] || ind[0xbb]) && g_cities) {
            int32_t linked = 0;
            for (int32_t j = 0; j < Collection_Count(g_cities); ++j) {
                Collection* cs = g_cities;
                uint8_t* e = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, j, 0));
                if (F<int32_t>(e, 0x242) <= 0 || F<int32_t>(e, 0x2f2) != F<int32_t>(ind, 0x794)) continue;
                if ((ind[0xba] && F<int32_t>(e, 0x2fa)) || (ind[0xbb] && F<int32_t>(e, 0x2fe))) linked = 1;
            }
            if (!linked) continue;
        }
        const char* name = (const char*)ind + 4;
        if (!crt::stricmp((const char*)0x5c9378, name)) barracks = 1;
        if (!crt::stricmp((const char*)0x5c8de0, name)) {
            for (int32_t j = 0; j < Collection_Count(c); ++j)
                if (!crt::stricmp((const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0)) + 4, (const char*)0x5c99c0))
                    flags[Collection_IdAt(c, 0, j, 0)] = 1;
            continue;
        }
        uint8_t* r = F<uint8_t*>(ind, 0x37);
        for (int32_t n = F<int32_t>(ind, 0x33); n > 0; --n, r += 0xbc)
            if (!F<int32_t>(r, 0)) flags[F<int32_t>(r, 0x1c)] = 1;
    }
    F<int32_t>(c, 0x31a) = 0;
    static const uint32_t six[] = { 0x5c99b8, 0x5c99b0, 0x5c99a4, 0x5c999c, 0x5c9994, 0x5c9988 };
    for (int32_t j = 0; j < Collection_Count(c); ++j) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0));
        uint32_t id = Collection_IdAt(c, 0, j, 0);
        int32_t avail = (e[0x47] || e[0x48] || (e[0x49] && barracks) || flags[id]) ? 1 : 0;
        if (F<int32_t>(g_app, 0x25f)) avail = 1;
        if (F<int32_t>(World(), 0x4afb)) {
            bool keep = false;
            for (uint32_t s : six)
                if (!crt::stricmp((const char*)e + 4, (const char*)s)) {
                    keep = true;
                    break;
                }
            if (!keep) avail = 0;
        }
        F<int32_t>(e, 0x56) = avail;
    }
    CargoTypes_Recount(c);
}
RT3_REPLACE(0x41eac0, CargoTypes_MarkAvailable);

// 0x41ede0. Loads the cargo types: the Data\CargoTypes\*.cty files (up to 499,
// sorted by name) are read into records of 0xd0 bytes (at most 52 cargos): name,
// value, decay, flags, upper-cased abbreviation, and v1001+ extras; each gets its
// speed factor (+0x52), the Mail / Passengers / Troops markers (+0x8a / +0x88 /
// +0x8c, flags +0x46..+0x49), a value class (+0x4a / +0x4e) and, for the first 71,
// localized names in the cargo string tables (0x61312c / 0x6150f8, 12 more lists at
// 0x5f7f9f). Then the special cargo ids (+0x8e..+0x96) and the quota-slot table
// (+0x9a) are rebuilt and availability marked.
void __fastcall CargoTypes_Load(Collection* c) {
    static char names[0x1f3][0xc9];
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    char pattern[0x100];
    FormatText(pattern, (const char*)0x5c9a08, (const char*)0x5c9a10, 0, 0, 0, 0, 0, 0, 0);
    int32_t n = 0;
    WIN32_FIND_DATAA fd;
    HANDLE h = (((HANDLE(__stdcall*)(const char*, WIN32_FIND_DATAA*))imp::FindFirstFileA))(pattern, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        BOOL more = 1;
        while (more) {
            if (n < 0x1f3) {
                StrCopyN(names[n], fd.cFileName, 0xc8);
                more = (((BOOL(__stdcall*)(HANDLE, WIN32_FIND_DATAA*))imp::FindNextFileA))(h, &fd);
                ++n;
            }
        }
        (((BOOL(__stdcall*)(HANDLE))imp::FindClose))(h);
    }
    if (n <= 0) goto specials;
    for (int32_t i = 1; i <= n; ++i)
        for (int32_t j = i; j < n; ++j)
            if (crt::stricmp(names[i - 1], names[j]) >= 0) {
                char tmp[0xc9];
                strcpy(tmp, names[j]);
                strcpy(names[j], names[i - 1]);
                strcpy(names[i - 1], tmp);
            }
    for (int32_t k = 0; k < n; ++k) {
        char path[0x200];
        FormatText(path, (const char*)0x5c8730, (const char*)0x5c9a10, names[k], 0, 0, 0, 0, 0, 0);
        FILE* f = ((FILE*)crt::fopen(path, (const char*)0x5c9678));
        if (!f) continue;
        auto rd = [&](void* p, size_t sz) { crt::fread(p, 1, sz, f); };
        uint8_t rec[0xd0] = {};
        rd(rec, 4);
        rd(rec + 4, 0x14);
        rd(rec + 0x18, 4);
        rd(rec + 0x1c, 4);
        rd(rec + 0x20, 1);
        rd(rec + 0x21, 4);
        rd(rec + 0x25, 0x15);
        if (F<int32_t>(rec, 0) >= 0x3e9) rd(rec + 0x3a, 4);
        else F<int32_t>(rec, 0x3a) = 0;
        if (F<int32_t>(rec, 0) <= 0x3e9) rec[0x20] = 0;
        for (int32_t i = (int32_t)strlen((const char*)rec + 0x25) - 1; i >= 0; --i)
            rec[0x25 + i] = (uint8_t)ToUpper((char)rec[0x25 + i]);
        crt::fclose(f);
        double x = Game_EffectiveVersion() >= 0x67 ? (double)F<float>(rec, 0x1c) * D(0x5c85a8) / D(0x5c9a00)
                                                   : (double)F<float>(rec, 0x1c) / Fl_202(0x5c99fc);
        F<float>(rec, 0x42) = (float)(D(0x5c8770) - x);
        if (Collection_Count(c) >= 0x34) continue;
        uint32_t id = c->vtbl->allocate(c, rec, 0, 0, 0);
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, id);
        F<uint32_t>(r, 0x3e) = id;
        F<float>(r, 0x52) = (float)((double)F<float>(r, 0x1c) / Fl_202(0x5c99f8) * D(0x5c8570) / D(0x5c99e8) + D(0x5c8770));
        const char* nm = (const char*)rec + 4;
        if (!crt::stricmp(nm, (const char*)0x5c99f0)) {
            F<uint16_t>(c, 0x8a) = (uint16_t)id;
            r[0x48] = 1;
            r[0x46] = 1;
        }
        if (!crt::stricmp(nm, (const char*)0x5c99d8)) {
            F<uint16_t>(c, 0x88) = (uint16_t)id;
            r[0x47] = 1;
            r[0x46] = 1;
        }
        if (!crt::stricmp(nm, (const char*)0x5c99d0)) {
            F<uint16_t>(c, 0x8c) = (uint16_t)id;
            r[0x49] = 1;
            r[0x46] = 1;
        }
        float value = F<float>(rec, 0x18);
        if (value >= Fl_202(0x5c99cc)) {
            F<uint32_t>(r, 0x4a) = 0x3fcccccd;
            F<uint32_t>(r, 0x4e) = 0x3f000000;
        } else if (value >= Fl_202(0x5c99c8)) {
            F<uint32_t>(r, 0x4a) = 0x3fe66666;
            F<uint32_t>(r, 0x4e) = 0x3ecccccd;
        } else if (value >= Fl_202(0x5c8db0)) {
            F<uint32_t>(r, 0x4a) = 0x40000000;
            F<uint32_t>(r, 0x4e) = 0x3e4ccccd;
        } else {
            F<uint32_t>(r, 0x4a) = 0x400ccccd;
            F<uint32_t>(r, 0x4e) = 0x3dcccccd;
        }
        char text[0x200];
        if (id <= 0x46) {
            FormatText(text, GetString(0xc7), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN((char*)0x61312c + id * 0x6e, text, 0x46);
            F<uint32_t>((void*)0x613173, id * 0x6e) = id;
            FormatText(text, GetString(0xc8), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN((char*)0x6150f8 + id * 0x6e, text, 0x46);
            F<uint32_t>((void*)0x61513f, id * 0x6e) = id;
        }
        char* e = (char*)0x5f7f9f + id * 0x81;
        for (int32_t k2 = 0; k2 < 0xc; ++k2, e += 0x1ebd) {
            FormatText(text, GetString(F<int32_t>(e, 0x69)), LocalizeName(nm), 0, 0, 0, 0, 0, 0, 0);
            StrCopyN(e, text, 0x46);
            F<uint32_t>(e, -8) = id;
            F<uint32_t>(e, 0x5c) = 0;
        }
    }
specials:
    F<int32_t>(c, 0x8e) = -1;
    F<int32_t>(c, 0x92) = -1;
    F<int32_t>(c, 0x96) = -1;
    for (int32_t k = 0; k < 0x35; ++k) F<int32_t>(c, 0x9a + k * 4) = -1;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        auto name = [&]() { return (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4; };
        if (!crt::stricmp(name(), (const char*)0x5c99f0)) {
            F<uint32_t>(c, 0x92) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 1;
        }
        if (!crt::stricmp(name(), (const char*)0x5c99d8)) {
            F<uint32_t>(c, 0x8e) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 0;
        }
        if (!crt::stricmp(name(), (const char*)0x5c99d0)) {
            F<uint32_t>(c, 0x96) = Collection_IdAt(c, 0, i, 0);
            F<int32_t>(c, 0x9a + Collection_IdAt(c, 0, i, 0) * 4) = 2;
        }
    }
    CargoTypes_MarkAvailable(c);
}
RT3_REPLACE(0x41ede0, CargoTypes_Load);

// 0x41f4e0. Cargo type table constructor: 0xd0-byte records (10, +5), loaded from
// the cargo files, then counted.
RT3_NOINLINE Collection* __fastcall CargoTypes_Construct(Collection* c) {
    Collection_SetVtbl(c);
    *(uint32_t*)c = 0x5c997c;
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    Collection_Init(c, 0, 1, 0xd0, 10, 5, 1, 0, 0);
    CargoTypes_Load(c);
    CargoTypes_Recount(c);
    return c;
}
RT3_REPLACE(0x41f4e0, CargoTypes_Construct);

// 0x41f530. Releases every cargo type and reloads them from the files.
RT3_NOINLINE void __fastcall CargoTypes_Reload(Collection* c) {
    while (Collection_Count(c)) c->vtbl->release(c, Collection_IdAt(c, 0, 0, 0));
    F<uint16_t>(c, 0x88) = 0;
    F<uint16_t>(c, 0x8a) = 0;
    F<uint16_t>(c, 0x8c) = 0;
    CargoTypes_Load(c);
    CargoTypes_Recount(c);
}
RT3_REPLACE(0x41f530, CargoTypes_Reload);

// Named object subclasses: construct the base, then set the vtable and own fields.
RT3_NOINLINE uint8_t* __fastcall NamedObj41f590_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5c9a28;
    return o;
}
RT3_REPLACE(0x41f590, NamedObj41f590_Construct);

// Named object subclass destructors: set the vtable, then the base's.
RT3_NOINLINE void __fastcall NamedObj41f590_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5c9a28;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x41f5b0, NamedObj41f590_Destructor);

// 0x41f5c0. Loads city c from stream s: its world model and a fresh building
// mix table; an unset size factor +0x31b becomes 1. Returns the bytes read.
int32_t __fastcall City_Load(uint8_t* c, int, int32_t s) {
    if (F<float>(c, 0x31b) == *(const float*)rt3::Rdata(0x5c8598)) {
        F<uint32_t>(c, 0x31b) = 0x3f800000;
        F<uint32_t>(c, 0x317) = 0;
    }
    int32_t n = WorldModel_Load(c, 0, s);
    Collection* t = (Collection*)crt::operator_new(0x88);
    t = t ? Collection_Construct(t, 0, 1, 0x22, 2, 2, 0, 0, 0) : nullptr;
    F<Collection*>(c, 0x37f) = t;
    n += Collection_Load(t, 0, s, 0);
    F<int32_t>(c, 0x385) = 0;
    return n;
}
RT3_REPLACE(0x41f5c0, City_Load);

// 0x41f650 (vtable). Saves the named object o and its list +0x37f.
void __fastcall NamedObj_SaveWithList(uint8_t* o, int, Stream s) {
    NamedObj_Save(o, 0, s);
    Collection_Save(F<Collection*>(o, 0x37f), 0, s, 0);
}
RT3_REPLACE(0x41f650, NamedObj_SaveWithList);

// 0x41f680. City object o: previewed, then opens the editor on its city.
void __fastcall CityObj_Select(uint8_t* o) {
    SceneObj_ShowPreview(o);
    EditorDialog_Run(1, F<int32_t>(o, 0x23a));
}
RT3_REPLACE(0x41f680, CityObj_Select);

RT3_NOINLINE int32_t __fastcall Obj_Count37f(void* o) { return Collection_Count(F<Collection*>(o, 0x37f)); }
RT3_REPLACE(0x41f6a0, Obj_Count37f);

// 0x41f6b0. Fog end distance at the object's cell.
int32_t __fastcall Obj_FogEnd(void* o) { return ObjFogEnd(o); }
RT3_REPLACE(0x41f6b0, Obj_FogEnd);

// 0x41f6e0. The region under this object has key k.
RT3_NOINLINE int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k) { return Obj_HasKey(Obj_Region(o), 0, k); }
RT3_REPLACE(0x41f6e0, Obj_RegionHasKey);

// 0x41f6f0 (vtable stub).
int32_t __fastcall Obj_Byte42_41f6f0(uint8_t* o) { return o[0x42]; }
RT3_REPLACE(0x41f6f0, Obj_Byte42_41f6f0);

// 0x41f700 (vtable stub).
int32_t __fastcall Return0_41f700(void*) { return 0; }
RT3_REPLACE(0x41f700, Return0_41f700);

// 0x41f710 (vtable stub).
int32_t __fastcall Return1_41f710(void*) { return 0x1; }
RT3_REPLACE(0x41f710, Return1_41f710);

// 0x41f720 (vtable). Shows o's name (+0x356, string 0xce) as a tooltip and,
// with the overview off (or mode 0x15), highlights its cargo (+0x23a, mode
// 0x12).
void __fastcall Obj_ShowCargoTooltip(uint8_t* o) {
    char text[0x130];
    FormatText(text, GetString(0xce), (const char*)(o + 0x356), 0, 0, 0, 0, 0, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, text, 3, 0, -1);
    const int32_t mode = F<int32_t>(g_map, 0x2175);
    if (mode == 0 || mode == 0x15)
        Overview_SetMode(g_map, 0, 0x12, F<int32_t>(o, 0x23a), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
}
RT3_REPLACE(0x41f720, Obj_ShowCargoTooltip);

void Tooltip_ClearAndCloseOverview() {
    Tooltip_Clear();
    uint8_t* m = g_map;
    if (F<int32_t>(m, 0x2175) == 0x12) Overview_Close(m);
}
RT3_REPLACE(0x41f7b0, Tooltip_ClearAndCloseOverview);

// 0x41f7e0. Pushes building `b` on the front of this's building chain
// (u16 head +0x383, next +0x270, own ID +0x2a4).
void __fastcall Obj_PushBuilding(void* o, int, void* b) {
    F<uint16_t>(b, 0x270) = F<uint16_t>(o, 0x383);
    F<uint16_t>(o, 0x383) = F<uint16_t>(b, 0x2a4);
}
RT3_REPLACE(0x41f7e0, Obj_PushBuilding);

// 0x41f810. First building of the chain, or 0 if the head is not valid.
void* __fastcall Obj_FirstBuilding(void* o) {
    if (!Collection_IsValid(g_buildings, 0, F<uint16_t>(o, 0x383))) return nullptr;
    return Collection_Get(g_buildings, 0, F<uint16_t>(o, 0x383));
}
RT3_REPLACE(0x41f810, Obj_FirstBuilding);

// 0x41f850 (vtable). Unlinks record r from o's chain (+0x383, links +0x270
// in 0x62b26c, ending at r's id +0x2a4) and clears its links.
RT3_NOINLINE void __fastcall Chain_Unlink(uint8_t* o, int, uint8_t* r) {
    uint32_t id = F<uint16_t>(o, 0x383);
    if (id != F<uint32_t>(r, 0x2a4)) {
        uint32_t prev;
        do {
            prev = id;
            id = F<uint16_t>((uint8_t*)Collection_Get(g_buildings, 0, id), 0x270);
        } while (id != F<uint32_t>(r, 0x2a4));
        if (prev) {
            F<uint16_t>((uint8_t*)Collection_Get(g_buildings, 0, prev), 0x270) = F<uint16_t>(r, 0x270);
            F<uint16_t>(r, 0x26e) = 0;
            F<uint16_t>(r, 0x270) = 0;
            return;
        }
    }
    F<uint16_t>(o, 0x383) = F<uint16_t>(r, 0x270);
    F<uint16_t>(r, 0x26e) = 0;
    F<uint16_t>(r, 0x270) = 0;
}
RT3_REPLACE(0x41f850, Chain_Unlink);

// 0x41f8d0. Clears the links (+0x26e, +0x270) of every building in the chain.
// The head +0x383 itself is left as is.
void __fastcall Obj_UnlinkBuildings(void* o) {
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        F<uint16_t>(b, 0x26e) = 0;
        F<uint16_t>(b, 0x270) = 0;
    }
}
RT3_REPLACE(0x41f8d0, Obj_UnlinkBuildings);

// 0x41f910 / 0x461500 / 0x474430. Visibility tests against the 0x6d07b4 mode.
int32_t __fastcall Obj_Visible41f910(uint8_t* o) {
    if (!Global6d07b4Set2()) return 1;
    return F<int32_t>(o, 0x23e) ? 1 : o[0x42];
}
RT3_REPLACE(0x41f910, Obj_Visible41f910);

// 0x41f940. Records of the +0x37f collection whose 0x62b268 namesake has +0x7ac set.
int32_t __fastcall Obj_CountFlagged7ac(void* o) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* c = F<Collection*>(o, 0x37f);
        const char* name = (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        uint8_t* t = (uint8_t*)Table_GetByName(g_62b268, 0, name);
        if (t && F<int32_t>(t, 0x7ac)) ++n;
    }
    return n;
}
RT3_REPLACE(0x41f940, Obj_CountFlagged7ac);

// 0x41f9b0. Entries of the +0x37f name table whose 0x62b268 type is enabled
// (+0x7ac), of `kind` (+0x32), and available by `year` (u16 +0x26).
RT3_NOINLINE int32_t __fastcall Obj_CountTypesAvailable(void* o, int, int32_t year, int32_t kind) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        uint8_t* r = (uint8_t*)Table_GetByName(g_62b268, 0, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
        if (r && F<int32_t>(r, 0x7ac) && F<uint8_t>(r, 0x32) == kind && F<uint16_t>(r, 0x26) <= year) ++n;
    }
    return n;
}
RT3_REPLACE(0x41f9b0, Obj_CountTypesAvailable);

// 0x41fa30. Float +0x1e of the +0x37f entry named `name`, or 0.
RT3_NOINLINE double __fastcall Obj_NamedValue(void* o, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        if (crt::stricmp(name, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) {
            t = F<Collection*>(o, 0x37f);
            return F<float>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x1e);
        }
    }
    return *(const float*)rt3::Rdata(0x5c8598);
}
RT3_REPLACE(0x41fa30, Obj_NamedValue);

// 0x41fac0. i-th (1-based) record of the table at +0x37f: name to dst, +0x1e to *out.
RT3_NOINLINE void __fastcall Obj_Entry37f(void* o, int, int32_t i, char* dst, int32_t* out) {
    Collection* t = F<Collection*>(o, 0x37f);
    const char* r = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i - 1, 0));
    const char* s = r;
    char* d = dst;
    while ((*d++ = *s++) != 0) {}
    *out = F<int32_t>((void*)r, 0x1e);
}
RT3_REPLACE(0x41fac0, Obj_Entry37f);

// 0x41fb00. Lays out a city's streets on the fine (quarter-cell) grid: cross
// streets on rings around the centre every 2 directions out to radius 42 (longer
// further out, the extent per side from the city's pattern (x + y) & 3) and a
// mirrored 12 x 6 grid by the centre; then within 10 cells the building density is
// cleared (inside 3) or capped by distance, keeping each cell's flag.
void __fastcall City_LayStreets(uint8_t* c) {
    if (F<int32_t>(c, 0x23e)) return;
    const int32_t* dxs = (const int32_t*)0x624b28;
    const int32_t* dys = (const int32_t*)0x624b48;
    int32_t cx = Ftol(Obj_CellX(c) * *(const float*)rt3::Rdata(0x5c8568));
    int32_t cy = Ftol(Obj_CellY(c) * *(const float*)rt3::Rdata(0x5c8568));
    int32_t pat = (cx + cy) & 3;
    static const int8_t grow[4][7] = { { 1, 0, -1, 0, -1, 0, 1 }, { -1, 0, -1, 0, 1, 0, 1 },
                                       { -1, 0, 1, 0, 1, 0, -1 }, { 1, 0, 1, 0, -1, 0, -1 } };
    for (int32_t r = 0; r <= 0x2a; ++r) {
        for (int32_t d = 0; d < 8; d += 2) {
            int32_t py = dys[d] * r + cy, px = dxs[d] * r + cx;
            int32_t e = (d + 2) & 7;
            int32_t lo = r >= 0x1f ? 6 : r >= 0x1d ? 4 : r >= 0x1b ? 2 : r >= 0x19 ? 1 : 0;
            int32_t hi = lo + 2;
            if (e < 7) {
                if (grow[pat][e] > 0) ++hi;
                else if (grow[pat][e] < 0) ++lo;
            }
            lo = -lo;
            for (int32_t t = lo - 1; t < hi; ++t) Street(dxs[e] * t + px, dys[e] * t + py);
        }
    }
    for (int32_t swap = 0; swap < 2; ++swap)
        for (int32_t s = 0; s < 0xd; ++s)
            for (int32_t t = 1; t <= 6; ++t) {
                int32_t a = swap ? t : s - 1, b = swap ? s - 1 : t;
                if (pat == 1) b = -b;
                else if (pat == 2) {
                    a = -a;
                    b = -b;
                } else if (pat == 3) a = -a;
                Street(cx + a, cy + b);
            }
    int32_t ccx = Ftol(Obj_CellX(c)), ccy = Ftol(Obj_CellY(c));
    uint8_t* m = g_map;
    int32_t x0 = ccx - 10 < 0 ? 0 : ccx - 10, y0 = ccy - 10 < 0 ? 0 : ccy - 10;
    int32_t x1 = F<int32_t>(m, 0x2155) - 1, y1 = F<int32_t>(m, 0x2159) - 1;
    if (x1 >= ccx + 10) x1 = ccx + 10;
    if (y1 >= ccy + 10) y1 = ccy + 10;
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            float d = (float)Distance2D((float)x, (float)y, (float)ccx, (float)ccy);
            if (d > *(const float*)rt3::Rdata(0x5c85f8)) continue;
            uint8_t flag = Terrain_GetFlag(g_map, 0, x, y);
            if (!(d > *(const float*)rt3::Rdata(0x5c8900)) && d == d) Terrain_SetCellCount(g_map, 0, x, y, 0);
            else if (!(d > *(const float*)rt3::Rdata(0x5c85f8)) && d == d) {
                int32_t n = Ftol(((double)d - *(const float*)rt3::Rdata(0x5c8900)) * D(0x5c8970) / D(0x5c8bf0) + D(0x5c86b8));
                if (n < (int32_t)Terrain_CellCount(g_map, 0, x, y))
                    Terrain_SetCellCount(g_map, 0, x, y, (uint8_t)n);
            }
            Terrain_SetFlag(g_map, 0, x, y, flag);
        }
    }
}
RT3_REPLACE(0x41fb00, City_LayStreets);

// 0x41fff0. Option +0x2bb: 0 = off, 2 = on, otherwise on once the value
// at (*0x62be68)+0x20 reaches the threshold 0x5c9a9c.
RT3_NOINLINE int32_t Option2bbActive() {
    int32_t mode = F<int32_t>(g_app, 0x2bb);
    if (mode == 0) return 0;
    if (mode == 2) return 1;
    return F<float>(g_62be68, 0x20) >= *(const float*)rt3::Rdata(0x5c9a9c) ? 1 : 0;
}
RT3_REPLACE(0x41fff0, Option2bbActive);

}  // namespace rt3
