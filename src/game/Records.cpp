// Records.cpp - RT3.exe 0x42c000-0x430000.
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
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

 // region types

int32_t __fastcall Obj_BuildingStateActive(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Chain_MinLevel(void* o, int, int32_t i);
void* __fastcall Obj_FirstActiveInChain(void* o);
void __fastcall List1a_Free(uint8_t** head);
void __fastcall Obj_FreeFourLists(void* o);
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value);
double __stdcall Effects_CargoMultiplier(int32_t idx);
void __fastcall Cargo_Produce(void* c, int, float a, float price);
int32_t __fastcall Obj_YearReached(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
void __fastcall City_TransferCargo(uint8_t* c, int, uint8_t* dst, int32_t i, int32_t amount);
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p);
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p);
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;
void __fastcall Obj_Set2a2(void* o, int, uint16_t v);
void __fastcall City_UpdateCargoFlags(uint8_t* c);
void __fastcall Obj_UpdateFlagE6(void* o);
int __fastcall ToUpper(char c);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
const char* __fastcall GetString(int32_t id);


void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
uint32_t __fastcall Table_FindByName2(Collection* t, int, const char* name, int32_t);

// Reads a saved 100-byte name and returns the index the lookup gives for it.
template <typename Fn> static int32_t ReadIndex(Stream st, char* buf, Fn lookup) {
    Rd(st, buf, 0x64);
    return (int32_t)lookup(buf);
}

int32_t __fastcall Game_EffectiveVersion();
void __fastcall Cargo_MoveTo66(void* c, int, float a);
int32_t __fastcall City_HasCargoSurplus(uint8_t* o, int, int32_t i);
void __fastcall Region_UpdateCargoEconomy(uint8_t* r);
void __fastcall City_UpdateCargoBases(uint8_t* o);
void __fastcall City_SpreadCargo(uint8_t* c);
void __fastcall Station_CapStocks(void* o);
int32_t IsMultiplayer();
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);

// RT3_REPLACE(0x4b23d0, Trains_Create); -- see above

int32_t __fastcall Company_Get7680(void* c);

int32_t __fastcall Table_CountField3f(Collection* t);
double __fastcall Company_TrackLength(void* c);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t CountHumanPlayers();
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
int32_t __fastcall Cities_CountConnected(Collection* c, int, int32_t company);
int32_t __fastcall Territories_CountLinked(Collection* c, int, int32_t company);
int32_t __fastcall Company_Sum42e300(void* c);
int32_t __fastcall Company_Sum42e320(void* c);
double __fastcall Company_YearPair1da0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1da8(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1db0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1db8(uint8_t* c, int, int32_t y);
double __fastcall Company_YearStatPair(uint8_t* c, int, int32_t y, int32_t i);
double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i);
int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet1(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet3(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_TotalNet1(void* c);
int32_t __fastcall Company_TotalNet3(void* c);
int32_t __fastcall Obj_NetBc1(void* o);
int32_t __fastcall Obj_NetBc3(void* o);
int32_t __fastcall Table_CountActiveSites(Collection* t, int, int32_t highlighted);
int32_t __fastcall Table_CountFlag201_40(Collection* t);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
float __fastcall Obj_CenterCellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
uint32_t __fastcall Obj_GridByte(void* o);
uint32_t __fastcall Obj_CityGridByte(void* o);
uint32_t __fastcall Train_GridByteAtCar45(uint8_t* t);
void __fastcall Tracks_ClearHighlights(Collection* c);
uint32_t __fastcall Obj_CenterGridByte(void* o);
void __fastcall Tracks_HighlightRun(Collection* c, int, uint32_t id, int32_t on, int32_t follow);
double __fastcall Obj_BuildingCellY(void* o);
double __fastcall Obj_BuildingCellX(void* o);
int32_t __fastcall Obj_NodeNear8(uint8_t* o, int, int32_t id);
int32_t __fastcall Obj_NodeNear356(uint8_t* o, int, int32_t id);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Stock_Consume(uint16_t* o, int, const uint16_t* other, uint32_t id, float f);
int32_t __fastcall Chain_MinLevel(void* o, int, int32_t i);
void __fastcall City_SpreadCargo(uint8_t* c);
void __fastcall Chain_RegisterBuildings(void* o, int, uint8_t* target, int32_t value);
float __fastcall Station_PathCost(void* o, int, int32_t x, int32_t y);
void __fastcall City_UpdateCargoFlags(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Chain_Contains(void* o, int, uint16_t k);
RT3_NOINLINE int32_t __fastcall Chain_HasValue(void* o, int, int32_t v);
RT3_NOINLINE void* __fastcall Obj_FirstActiveInChain(void* o);
RT3_NOINLINE int32_t __fastcall Chain_AnyActive(void* o);
void __fastcall Obj_UpdateFlagE6(void* o);
RT3_NOINLINE int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
void __fastcall City_AddBuilding(uint8_t* c, int, uint16_t id);
RT3_NOINLINE void __fastcall City_RemoveBuilding(uint8_t* c, int, uint16_t id);
RT3_NOINLINE double __fastcall City_ProduceCargo(uint8_t* o, int, int32_t i, float amt);
void __fastcall Station_CapStocks(void* o);
int32_t __fastcall City_HasCargoSurplus(uint8_t* o, int, int32_t i);
void __fastcall City_UpdateCargoBases(uint8_t* o);
void __fastcall City_Refresh(uint8_t* c);
void __fastcall Region_UpdateCargoEconomy(uint8_t* r);
void __fastcall Region_MonthlyEconomy(uint8_t* r);
void* __fastcall Record42d5a0_Init(uint8_t* r);
void __fastcall Record42d5a0_SetName(uint8_t* r, int, int32_t v, const char* name);
void __fastcall List1a_Free(uint8_t** head);
void __fastcall Obj_FreeFourLists(void* o);
RT3_NOINLINE uint8_t __fastcall Load_CargoFlags(void* o);
RT3_NOINLINE uint8_t __fastcall Obj_ListFlags(void* o, int, int32_t i, int32_t classify);
void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
void __fastcall Record_EncodeTags(uint8_t* r);
void __fastcall EventRules_Load(uint8_t* o, int, Stream st);
int32_t __fastcall Record_AnyKind1(uint8_t* r);
int32_t __fastcall Record_AnyKind2(uint8_t* r);
int32_t __fastcall Record_AnyKind4(uint8_t* r);
int32_t __fastcall Record_AnyKind7(uint8_t* r);
void __fastcall Record_CopyFrom(uint8_t* dst, int, uint8_t* src);
int32_t __fastcall Obj_AnyKind4to7(void* o);
RT3_NOINLINE int32_t __fastcall Company_Sum42e300(void* c);
int32_t __fastcall Company_Sum42e320(void* c);
RT3_NOINLINE double __fastcall Company_YearPair1da0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1da8(uint8_t* c, int, int32_t y);
RT3_NOINLINE double __fastcall Company_YearStatPair(uint8_t* c, int, int32_t y, int32_t i);
RT3_NOINLINE double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i);
double __fastcall Company_YearPair1db0(uint8_t* c, int, int32_t y);
double __fastcall Company_YearPair1db8(uint8_t* c, int, int32_t y);
RT3_NOINLINE int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet1(uint8_t* c, int, int32_t y);
int32_t __fastcall Company_YearNet3(uint8_t* c, int, int32_t y);
RT3_NOINLINE int32_t __fastcall Company_TotalNet1(void* c);
int32_t __fastcall Company_TotalNet3(void* c);
RT3_NOINLINE int32_t __fastcall Obj_NetBc1(void* o);
int32_t __fastcall Obj_NetBc3(void* o);
RT3_NOINLINE void __fastcall Obj_FreeAllLists(void* o);
RT3_NOINLINE int32_t __stdcall RuleVar_Get(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8);
RT3_NOINLINE void __stdcall Editor_DeleteKind(int32_t* owner, uint32_t* region, int32_t kind);
RT3_NOINLINE double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
RT3_NOINLINE int32_t __fastcall Stations_CargoLinked(Collection* sts, int, int32_t a, int32_t b, int32_t aIsRegion, int32_t bIsRegion, int32_t value);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);

static inline double D(uint32_t va) { return *(const double*)va; }

// 0x42c880 is Obj_FirstActiveInChain (objects4.cpp).
static inline void* Chain_FirstActive(void* o) { return Obj_FirstActiveInChain(o); }

// 0x42df30..0x42dff0. Whether any entry of the list *r (next +0x1a) has a
// kind (+0) whose flags in table 0x5f3e04 (stride 0x81) match `mask`; kind
// -1 matches on the record's own condition instead.
static inline int32_t AnyKind(uint8_t* r, uint8_t mask, int anyCond) {
    for (uint8_t* e = F<uint8_t*>(r, 0); e; e = F<uint8_t*>(e, 0x1a)) {
        int32_t k = F<int32_t>(e, 0);
        if (k >= 0 && (((const uint8_t*)0x5f3e04)[k * 0x81] & mask)) return 1;
        if (k == -1 && anyCond) return 1;
    }
    return 0;
}

static inline double D_125(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline Collection* Cargos() { return g_cargoTypes; }

static inline Collection* Sites() { return g_stations; }

// ftol(cargo multiplier x (base word[i] << 4)), in x87 precision.
static int32_t CargoQuota(uint8_t* o, int32_t i) {
    double m = Effects_CargoMultiplier(i);
    float n = (float)((int32_t)F<uint16_t>(o, i * 2) << 4);
    int32_t r;
    __asm {
        fld m
        fmul n
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline double D_153(uint32_t a) { return *(const double*)a; }

static inline uint8_t* Map() { return g_map; }

static inline Collection* Buildings() { return g_buildings; }


static inline void Refresh(uint8_t* c) {
    City_UpdateCargoFlags(c);
    Obj_UpdateFlagE6(c);
}

// Deep-copies a singly linked list of n-byte nodes (next at +nextOff) into *head.
static void CopyList(const uint8_t* src, uint8_t** head, uint32_t size, uint32_t nextOff) {
    uint8_t* prev = nullptr;
    for (int32_t i = 0; src; src = F<const uint8_t*>((void*)src, nextOff), ++i) {
        uint8_t* n = (uint8_t*)crt::operator_new(size);
        memcpy(n, src, size);
        F<void*>(n, nextOff) = nullptr;
        if (!i) *head = n;
        else F<uint8_t*>(prev, nextOff) = n;
        prev = n;
    }
}

static inline void Rd(Stream st, void* p, uint32_t n) { StreamRead(st, p, n, 0, 1); }

static inline double D_202(uint32_t a) { return *(const double*)a; }

static inline double Fl_202(uint32_t a) { return *(const float*)a; }

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

static inline double D_205(uint32_t a) { return *(const double*)a; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// 0x42c080. Lowers the u16 stock this[id] towards other[id] minus the
// consumption (cargo weight +0x52 x stock, scaled) x f, never below its current value.
void __fastcall Stock_Consume(uint16_t* o, int, const uint16_t* other, uint32_t id, float f) {
    uint8_t* ct = (uint8_t*)Collection_Get(g_cargoTypes, 0, id);
    int32_t a = o[id];
    int32_t s = other[id];
    float t = (float)(((double)F<float>(ct, 0x52) * (double)a * D(0x5c99e8) + D(0x5c8db8)) / D(0x5c9b78));
    s -= (int32_t)((double)t * f);
    o[id] = (uint16_t)(a > s ? a : s);
}
RT3_REPLACE(0x42c080, Stock_Consume);

// 0x42c110. Smallest (min 4000) 16 x level of cargo i over this chain's
// sites (u16 links +0xd4 / +0x3c) visited within the last 0xec400 minutes.
RT3_NOINLINE int32_t __fastcall Chain_MinLevel(void* o, int, int32_t i) {
    int32_t best = 999999999;
    for (uint32_t id = F<uint16_t>(o, 0xd4); id; ) {
        uint8_t* r = (uint8_t*)Collection_Get(g_stations, 0, id);
        if (F<int32_t>(g_world, 0x15) - F<int32_t>(r, 0x38e + i * 4) < 0xec400 &&
            Collection_IsValid(g_stations, 0, F<uint32_t>(r, 0x2ba + i * 4))) {
            uint8_t* reg = Obj_BuildingRegion(Collection_Get(g_stations, 0, F<uint32_t>(r, 0x2ba + i * 4)));
            int32_t v = F<uint16_t>(reg, i * 2) << 4;
            if (v <= 4000) v = 4000;
            if (v < best) best = v;
        }
        id = F<uint16_t>(r, 0x3c);
    }
    return best;
}
RT3_REPLACE(0x42c110, Chain_MinLevel);

// 0x42c1b0. Spreads the city's cargo to its neighbours (8 around; 16 with the 2-step
// ring in scenario mode, world +0x46c38) whose distance cost (+0xf3 words) is under a
// random cap: per produced cargo, the neighbour with the largest price gap above the
// distance cost (cost x (price x weight x 0.00016 + 2.2) x 256 >> 12) gets cargo.
// Neighbours across water (+0xe6 bit 0) only by the near ring and with a port (bit 2).
// Off with option +0x148.
RT3_NOINLINE void __fastcall City_SpreadCargo(uint8_t* c) {
    if (F<int32_t>(g_app, 0x148)) return;
    int32_t ncargo = Collection_Count(Cargos());
    int32_t n = F<int32_t>(g_world, 0x46c38) ? 16 : 8;
    int32_t ring = 1;
    int32_t cap = SimRand(1000, 30000);
    if ((double)cap > D_153(0x5c9c60)) cap = 999999;
    int32_t ok[16], cost[16];
    uint8_t* target[16];
    uint8_t* cell[16];
    int32_t cx = F<int16_t>(c, 0xe7), cy = F<int16_t>(c, 0xe9);
    for (int32_t k = 0; k < n; ++k) {
        int32_t d = k;
        if (k >= 8) {
            ring = 4;
            d = k & 7;
        }
        int32_t dx = kDirX[d], dy = kDirY[d];
        int32_t x = dx * ring + cx, y = dy * ring + cy;
        ok[k] = 0;
        if (x < 0 || x >= F<int32_t>(Map(), 0x2145) || y < 0 || y >= F<int32_t>(Map(), 0x2149)) continue;
        int32_t dist = F<uint16_t>(c, 0xf3 + k * 2);
        if (dist > cap) continue;
        uint8_t* nb = F<uint8_t*>(Map(), 0x2129) + (F<int32_t>(Map(), 0x2145) * y + x) * 0x1d7;
        cell[k] = nb;
        uint8_t a = c[0xe6], b = nb[0xe6];
        if ((a ^ b) & 1) {
            if (!(a & 4) && !(b & 4)) continue;
            if (ring != 1) continue;
        }
        ok[k] = 1;
        target[k] = nb;
        if (k < 8) {
            cost[k] = dist;
            continue;
        }
        int32_t dd = kDirY[d], dr = kDirX[d];
        uint8_t* r1 = Grid_RegionAt(Map(), 0, cx + dr * 2 + dr, cy + dd * 2 + dd);
        int32_t s = F<uint16_t>(r1, 0xf3 + d * 2);
        uint8_t* r2 = Grid_RegionAt(Map(), 0, cx + dr * 2, cy + dd * 2);
        s += F<uint16_t>(r2, 0xf3 + d * 2);
        uint8_t* r3 = Grid_RegionAt(Map(), 0, cx + dr, cy + dd);
        s += F<uint16_t>(r3, 0xf3 + d * 2);
        cost[k] = s + F<uint16_t>(c, 0xf3 + d * 2);
    }
    int32_t best = 1;
    for (int32_t i = 1; i <= ncargo; ++i) {
        Collection* cg = Cargos();
        if (!F<int32_t>(cg, 0x16e + i * 4) || F<int32_t>(cg, 0x9a + i * 4) != -1) continue;
        int32_t price = F<uint16_t>(c, i * 2);
        int32_t gapMax = 0;
        uint8_t* t = (uint8_t*)Collection_Get(cg, 0, i);
        float weight = F<float>(t, 0x52);
        int32_t f;
        double v = ((double)weight * (double)price * D_153(0x5c99e8) + D_153(0x5c8db8)) * D_153(0x5c9c58);
        __asm {
            fld v
            mov eax, 0x5a10d0
            call eax
            mov f, eax
        }
        for (int32_t k = 0; k < n; ++k) {
            if (!ok[k]) continue;
            int32_t gap = F<uint16_t>(target[k], i * 2) - ((cost[k] * f) >> 12) - price;
            if (gap > gapMax) {
                gapMax = gap;
                best = k;
            }
        }
        if (gapMax > 0) City_TransferCargo(c, 0, cell[best], i, (cost[best] * f) >> 12);
    }
}
RT3_REPLACE(0x42c1b0, City_SpreadCargo);

// 0x42c530. Adds every building of the chain +0xd6 (next +0x2a2) to target's key list with `value`.
void __fastcall Chain_RegisterBuildings(void* o, int, uint8_t* target, int32_t value) {
    for (uint16_t id = F<uint16_t>(o, 0xd6); id; id = F<uint16_t>(Collection_Get(g_buildings, 0, id), 0x2a2))
        Obj_AddKeyUnique(target, 0, id, value);
}
RT3_REPLACE(0x42c530, Chain_RegisterBuildings);

// 0x42c580. Sum of the per-direction cell values (+0xf3[dir], u16) along
// the 8-way line from the station (+0xe7, +0xe9) to (x, y).
float __fastcall Station_PathCost(void* o, int, int32_t x, int32_t y) {
    int32_t cx = F<int16_t>(o, 0xe7), cy = F<int16_t>(o, 0xe9);
    int32_t ax = x - cx < 0 ? cx - x : x - cx;
    int32_t ay = y - cy < 0 ? cy - y : y - cy;
    int32_t steps = ax > ay ? ax : ay;
    float sum = 0.0f;
    if (steps <= 0) return sum;
    uint8_t* map = g_map;
    int32_t w = F<int32_t>(map, 0x2145);
    uint8_t* cells = F<uint8_t*>(map, 0x2129);
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (; steps; --steps) {
        int32_t dir;
        if (cx < x) dir = cy > y ? 1 : (cy != y) + 2;
        else if (cx > x) dir = cy > y ? 7 : (cy == y) + 5;
        else dir = cy > y ? 0 : 4;
        float v = (float)F<uint16_t>(cells + (w * cy + cx) * 0x1d7, 0xf3 + dir * 2);
        cx += dx[dir];
        cy += dy[dir];
        sum = (float)((double)v + sum);
    }
    return sum;
}
RT3_REPLACE(0x42c580, Station_PathCost);

// 0x42c690. Recomputes the city's cargo flags: +0xdf bit set for each available
// cargo produced by its active buildings (+0x3d4) of industries, +0xd8 for those
// demanded (industries with +0x78c inputs).
RT3_NOINLINE void __fastcall City_UpdateCargoFlags(uint8_t* c) {
    uint8_t* demand = c + 0xd8;
    F<uint32_t>(demand, 0) = 0;
    F<uint16_t>(demand, 4) = 0;
    demand[6] = 0;
    int32_t ncargo = Collection_Count(Cargos());
    for (uint32_t id = F<uint16_t>(c, 0xd6); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        if (Obj_Flag3d4(b) == 1) {
            uint8_t* ind = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            if (ind[0x32] == 2) {
                for (int32_t i = 1; i <= ncargo; ++i) {
                    if (F<int32_t>(Cargos(), 0x9a + i * 4) != -1) continue;
                    if (!Obj_YearReached(Collection_Get(Cargos(), 0, i))) continue;
                    if (Industry_OutputTotal(ind, 0, i, 0) > D_153(0x5c85b8)) c[0xdf + (i >> 3)] |= (uint8_t)(1 << (i & 7));
                    if (F<int32_t>(ind, 0x78c) && Industry_InputShare(ind, 0, i, 0) > D_153(0x5c85b8))
                        demand[i >> 3] |= (uint8_t)(1 << (i & 7));
                }
            }
        }
        id = F<uint16_t>(b, 0x2a2);
    }
}
RT3_REPLACE(0x42c690, City_UpdateCargoFlags);

// 0x42c7d0 / 0x42c820. Searches of the site chain (u16 +0xd4, next +0x3c).
RT3_NOINLINE int32_t __fastcall Chain_Contains(void* o, int, uint16_t k) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; id = F<uint16_t>(Collection_Get(g_stations, 0, id), 0x3c))
        if (id == k) return 1;
    return 0;
}
RT3_REPLACE(0x42c7d0, Chain_Contains);

RT3_NOINLINE int32_t __fastcall Chain_HasValue(void* o, int, int32_t v) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; id = F<uint16_t>(Collection_Get(g_stations, 0, id), 0x3c))
        if (Obj_BuildingValue(Collection_Get(g_stations, 0, id)) == v) return 1;
    return 0;
}
RT3_REPLACE(0x42c820, Chain_HasValue);

// 0x42c880. First active record in the 0x6cec20 chain starting at u16 +0xd4 (next u16 +0x3c).
RT3_NOINLINE void* __fastcall Obj_FirstActiveInChain(void* o) {
    for (uint16_t id = F<uint16_t>(o, 0xd4); id; ) {
        void* r = Collection_Get(g_stations, 0, id);
        if (Obj_BuildingStateActive(r)) return r;
        id = F<uint16_t>(Collection_Get(g_stations, 0, id), 0x3c);
    }
    return nullptr;
}
RT3_REPLACE(0x42c880, Obj_FirstActiveInChain);

RT3_NOINLINE int32_t __fastcall Chain_AnyActive(void* o) {
    if (!F<uint16_t>(o, 0xd4)) return 0;
    return Chain_FirstActive(o) ? 1 : 0;
}
RT3_REPLACE(0x42c8d0, Chain_AnyActive);

// 0x42c8f0. Sets bit 2 of +0xe6 if any building in the chain from u16 +0xd6
// (next u16 +0x2a2) has flag +0x3d4 == 1 and state byte +0xba set; else clears it.
RT3_NOINLINE void __fastcall Obj_UpdateFlagE6(void* o) {
    for (uint16_t id = F<uint16_t>(o, 0xd6); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        if (Obj_Flag3d4(b) == 1) {
            void* st = ((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            if (F<uint8_t>(st, 0xba)) {
                F<uint8_t>(o, 0xe6) |= 4;
                return;
            }
        }
        id = F<uint16_t>(b, 0x2a2);
    }
    F<uint8_t>(o, 0xe6) &= 0xfb;
}
RT3_REPLACE(0x42c8f0, Obj_UpdateFlagE6);

// 0x42c960. The city's quota for cargo i: its base (word i, x16) times the cargo multiplier.
RT3_NOINLINE int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i) { return CargoQuota(o, i); }
RT3_REPLACE(0x42c960, City_CargoQuota);

// 0x42c9a0. Puts building id at the head of the city's building chain (+0xd6,
// links +0x2a2) and refreshes the city's flags.
void __fastcall City_AddBuilding(uint8_t* c, int, uint16_t id) {
    Obj_Set2a2(Collection_Get(Buildings(), 0, id), 0, F<uint16_t>(c, 0xd6));
    F<uint16_t>(c, 0xd6) = id;
    Refresh(c);
}
RT3_REPLACE(0x42c9a0, City_AddBuilding);

// 0x42c9f0. Unlinks building id from the city's chain (giving up after 1000 links
// or a broken one) and refreshes the city's flags.
RT3_NOINLINE void __fastcall City_RemoveBuilding(uint8_t* c, int, uint16_t id) {
    uint32_t cur = F<uint16_t>(c, 0xd6);
    if (!cur) return;
    uint32_t prev = 0;
    int32_t steps = 0;
    bool found = true;
    while (cur != id) {
        if (steps++ > 1000) {
            found = false;
            break;
        }
        prev = cur;
        cur = F<uint16_t>(Collection_Get(Buildings(), 0, cur), 0x2a2);
        if (!Collection_IsValid(Buildings(), 0, cur)) {
            found = false;
            break;
        }
    }
    if (found) {
        uint16_t next = F<uint16_t>(Collection_Get(Buildings(), 0, id), 0x2a2);
        if (prev) Obj_Set2a2(Collection_Get(Buildings(), 0, prev), 0, next);
        else F<uint16_t>(c, 0xd6) = next;
    }
    Refresh(c);
}
RT3_REPLACE(0x42c9f0, City_RemoveBuilding);

// 0x42cab0. Produces amt of cargo i at the quota price, adds it to the city's
// total (+0x103) and returns its value.
RT3_NOINLINE double __fastcall City_ProduceCargo(uint8_t* o, int, int32_t i, float amt) {
    float price = (float)CargoQuota(o, i);
    Cargo_Produce(Collection_Get(Cargos(), 0, i), 0, amt, price);
    F<float>(o, 0x103 + i * 4) = (float)((double)amt + F<float>(o, 0x103 + i * 4));
    return (double)price * amt;
}
RT3_REPLACE(0x42cab0, City_ProduceCargo);

// 0x42cb30. With an active site, caps each unmapped available cargo's
// stock +0[i] at Chain_MinLevel / 16.
RT3_NOINLINE void __fastcall Station_CapStocks(void* o) {
    if (!F<uint16_t>(o, 0xd4) || !Chain_FirstActive(o)) return;
    int32_t n = Collection_Count(g_cargoTypes);
    for (int32_t i = 1; i <= n; ++i) {
        uint8_t* ct = (uint8_t*)g_cargoTypes;
        if (!F<int32_t>(ct, 0x16e + i * 4) || F<int32_t>(ct, 0x9a + i * 4) != -1) continue;
        int32_t m = Chain_MinLevel(o, 0, i) >> 4;
        int32_t cur = F<uint16_t>(o, i * 2);
        F<uint16_t>(o, i * 2) = (uint16_t)(cur < m ? cur : m);
    }
}
RT3_REPLACE(0x42cb30, Station_CapStocks);

// 0x42cbb0. Whether any of the city's sites (chain from +0xd4, next +0x3c) has
// demand for cargo i (+0x3e) and more of it in stock (+0x112) than the quota.
RT3_NOINLINE int32_t __fastcall City_HasCargoSurplus(uint8_t* o, int, int32_t i) {
    for (uint32_t id = F<uint16_t>(o, 0xd4); id;) {
        uint8_t* s = (uint8_t*)Collection_Get(Sites(), 0, id);
        if ((double)F<float>(s, 0x3e + i * 4) > D_125(0x5c85b8)) {
            float q = (float)CargoQuota(o, i);
            if (F<float>(s, 0x112 + i * 4) > q) return 1;
        }
        id = F<uint16_t>(s, 0x3c);
    }
    return 0;
}
RT3_REPLACE(0x42cbb0, City_HasCargoSurplus);

// 0x42cc50. Re-prices the city's cargo bases (word i) from each site's surplus over
// the quota, scaled down as the city's total (+0x103) nears the site's demand.
RT3_NOINLINE void __fastcall City_UpdateCargoBases(uint8_t* o) {
    uint32_t id = F<uint16_t>(o, 0xd4);
    if (!id) return;
    int32_t n = Collection_Count(Cargos());
    while (id) {
        uint8_t* s = (uint8_t*)Collection_Get(Sites(), 0, id);
        for (int32_t i = 1; i <= n; ++i) {
            if (F<int32_t>(Cargos(), 0x9a + i * 4) != -1) continue;
            if (!Obj_YearReached(Collection_Get(Cargos(), 0, i))) continue;
            float demand = F<float>(s, 0x3e + i * 4);
            if ((double)demand <= D_125(0x5c85b8)) continue;
            float q = (float)CargoQuota(o, i);
            float surplus = (float)((double)F<float>(s, 0x112 + i * 4) - q);
            if (surplus <= Fl(0x5c8598)) continue;
            float ratio = (float)((double)F<float>(o, 0x103 + i * 4) / ((double)demand / D_125(0x5c85d0)));
            if (ratio > Fl(0x5c8900)) continue;
            float scale = (float)((D_125(0x5c85c8) - ratio) * D_125(0x5c9c68));
            int32_t v;
            __asm {
                fld scale
                fmul surplus
                fadd q
                mov eax, 0x5a10d0
                call eax
                mov v, eax
            }
            v >>= 4;
            if (v >= 0xfde8) v = 0xfde8;
            F<uint16_t>(o, i * 2) = (uint16_t)v;
        }
        id = F<uint16_t>(s, 0x3c);
    }
}
RT3_REPLACE(0x42cc50, City_UpdateCargoBases);

// 0x42cdf0. Refreshes the city's cargo and +0xe6 flags.
void __fastcall City_Refresh(uint8_t* c) { Refresh(c); }
RT3_REPLACE(0x42cdf0, City_Refresh);

// 0x42ce00. Monthly cargo economy of a region (free cargos only): the price
// (+0x103) decays with the cargo's decay (+0x1c) faster as the map ages (moved to
// the cargo's +0x66 pool); with no demand (+0x6a) supply (+0) drifts by the price
// level; otherwise supply follows the price-to-demand ratio (a piecewise curve,
// boosted for local producers +0xd8 bits, shaped by how saturated the market is),
// capped between the cargo's min / max (+0x4e / +0x4a) x base price (+0x18 x
// difficulty table 0x5f33a4), and demand slowly decays (randomly when small).
// Changes are gentler when the economy is frozen (+0x46c38).
RT3_NOINLINE void __fastcall Region_UpdateCargoEconomy(uint8_t* r) {
    uint8_t* world = g_world;
    int32_t frozen = F<int32_t>(world, 0x46c38);
    int32_t n = Collection_Count(g_cargoTypes);
    world = g_world;
    float base = *(const float*)(0x5f33a4 + F<int32_t>(world, 0x2d) * 4);
    int32_t age = F<uint16_t>(world, 0xd) - F<uint16_t>(world, 0x5);
    float decay;
    if (age > 0x14) age = 0x14;
    if (age >= 5) decay = (float)((double)(age - 5) * D_202(0x5c8918) + D_202(0x5c88c8));
    else decay = 0.3f;
    int32_t v103 = Game_EffectiveVersion() >= 0x67;
    for (int32_t k = 1; k <= n; ++k) {
        uint8_t* cargos = hook::As<uint8_t*>(g_cargoTypes);
        if (!F<int32_t>(cargos, 0x16e + k * 4) || F<int32_t>(cargos, 0x9a + k * 4) != -1) continue;
        float& price = F<float>(r, 0x103 + k * 4);
        uint16_t& supply = F<uint16_t>(r, k * 2);
        uint16_t& demand = F<uint16_t>(r, 0x6a + k * 2);
        uint8_t* rec = nullptr;
        if (!(price == 0.0f)) {
            rec = (uint8_t*)Collection_Get(g_cargoTypes, 0, k);
            float old = price;
            if (price > D_202(0x5c9cc8)) {
                auto step = [&](float v) {
                    return (float)((D_202(0x5c8770) - (double)decay * F<float>(rec, 0x1c) / Fl_202(0x5c8968)) * v);
                };
                float p1 = step(price);
                price = p1;
                if (p1 > Fl_202(0x5c8900)) {
                    float p2 = step(p1);
                    price = p2;
                    if (p2 > Fl_202(0x5c89a4)) price = step(p2);
                }
                Cargo_MoveTo66(rec, 0, (float)((double)old - price));
            } else {
                price = 0.0f;
                Cargo_MoveTo66(rec, 0, old);
            }
        }
        if (demand == 0) {
            double q = price < D_202(0x5c8d68) ? (double)price : 5.0;
            double f;
            if (F<uint16_t>(r, 0xd4) && City_HasCargoSurplus(r, 0, k)) f = D_202(0x5c9cc0) - q / D_202(0x5c8d68) * D_202(0x5c8668);
            else if (r[0xdf + (k >> 3)] & (1 << (k & 7))) f = D_202(0x5c9cb8) - q / D_202(0x5c8d68) * D_202(0x5c8c20);
            else f = D_202(0x5c9b60) - q / D_202(0x5c8d68) * D_202(0x5c88c8);
            float g = (float)f;
            if (frozen) supply = (uint16_t)Ftol(((double)g * D_202(0x5c85c8) / D_202(0x5c9ab8) + D_202(0x5c8770)) * (double)supply);
            else supply = (uint16_t)Ftol(((double)g / Fl_202(0x5c8968) + D_202(0x5c8770)) * (double)supply);
            continue;
        }
        if (!rec) rec = (uint8_t*)Collection_Get(g_cargoTypes, 0, k);
        float d = (float)((double)demand / D_202(0x5c9c40));
        float ratio = (float)((double)price / d);
        float g;
        bool positive;
        if (ratio < D_202(0x5c85b8)) {
            g = 0.25f;
            positive = true;
        } else {
            if (ratio < D_202(0x5c8818)) g = (float)((D_202(0x5c8818) - ratio) / D_202(0x5c8818) * D_202(0x5c9cb0) + D_202(0x5c8658));
            else if (ratio < D_202(0x5c9ca8)) g = (float)((D_202(0x5c9ca8) - ratio) / D_202(0x5c8818) * D_202(0x5c8658));
            else if (ratio < D_202(0x5c9ca0)) g = (float)((D_202(0x5c9ca0) - ratio) / D_202(0x5c8818) * D_202(0x5c9c98));
            else if (ratio < D_202(0x5c9c90)) g = (float)(D_202(0x5c9c98) - (D_202(0x5c9c90) - ratio) / D_202(0x5c9ca8) * D_202(0x5c8590));
            else if (ratio < D_202(0x5c9c88)) g = (float)(D_202(0x5c9c78) - (D_202(0x5c9c88) - ratio) / D_202(0x5c9ca0) * D_202(0x5c9c80));
            else {
                F<uint32_t>(&g, 0) = 0xbecccccd;
                positive = false;
                goto shaped;
            }
            positive = g > Fl_202(0x5c8598);
        }
        if (positive && (r[0xd8 + (k >> 3)] & (1 << (k & 7)))) {
            g = (float)(g * D_202(0x5c88f8));
            if (!(d < D_202(0x5c87d8))) g = (float)(g * D_202(0x5c8570));
        }
    shaped:
        {
            int32_t s16 = supply << 4;
            float pbase = (float)((double)base * F<float>(rec, 0x18));
            double m = Effects_CargoMultiplier(k);
            float w = (float)Ftol(m * (double)(float)s16);
            w = (float)((double)w / pbase);
            if (w < D_202(0x5c8770) && g > Fl_202(0x5c8598)) g = (float)(((D_202(0x5c8770) - w) * D_202(0x5c8578) + D_202(0x5c8770)) * g);
            if (w > D_202(0x5c8770) && g < Fl_202(0x5c8598)) g = (float)(((double)w + D_202(0x5c8628)) / D_202(0x5c8578) * g);
            if (g > Fl_202(0x5c8598) && ratio > D_202(0x5c8678) && v103) g = (float)(g * D_202(0x5c8640));
            double sd = (double)s16;
            int32_t ns = Ftol(((double)g / Fl_202(0x5c8968) + D_202(0x5c8770)) * sd);
            if (frozen) ns = Ftol(((double)g / D_202(0x5c8970) + D_202(0x5c8770)) * sd);
            float nf = (float)ns;
            float lo = (float)((double)pbase * F<float>(rec, 0x4e));
            if (nf < lo) ns = Ftol(lo);
            else {
                float hi = (float)((double)pbase * F<float>(rec, 0x4a));
                if (nf > hi) {
                    int32_t c = Ftol(hi);
                    if (v103) {
                        int32_t lim = ratio < D_202(0x5c8678) ? s16 + 1000 : s16 - 3000;
                        int32_t a = c > lim ? c : lim;
                        c = a < c + 10000 ? a : c + 10000;
                    }
                    if (ns >= c) ns = c;
                }
            }
            int32_t v = ns >> 4;
            if (v >= 0xfde8) v = 0xfde8;
            supply = (uint16_t)v;
        }
        if (demand > 0x30) demand = (uint16_t)Ftol((double)(float)demand * Fl_202(0x5c9c70));
        else {
            uint16_t dm = demand;
            if (SimRand(0, 0x18) <= (int32_t)(dm >> 1)) demand = dm > 2 ? (uint16_t)(dm - 2) : 0;
        }
    }
}
RT3_REPLACE(0x42ce00, Region_UpdateCargoEconomy);

// 0x42d580. A region's monthly economy: prices, supply and demand, cargo bases,
// spreading, then the stock caps.
void __fastcall Region_MonthlyEconomy(uint8_t* r) {
    Region_UpdateCargoEconomy(r);
    City_UpdateCargoBases(r);
    City_SpreadCargo(r);
    Station_CapStocks(r);
}
RT3_REPLACE(0x42d580, Region_MonthlyEconomy);

// 0x42d5a0. Initialises a 0x88f-byte record (names, flags, four slots).
void* __fastcall Record42d5a0_Init(uint8_t* r) {
    static const uint32_t kBytes[] = { 0x18, 0xe0, 0x401, 0x4ca, 0x593, 0x65c, 0x725, 0x7ee, 0x7ef };
    for (uint32_t off : kBytes) r[off] = 0;
    F<int32_t>(r, 0x7f0) = -1;
    r[0x7f4] = 0;
    F<int32_t>(r, 0x7f5) = 0;
    r[0x7f9] = 0;
    r[0x7fa] = 0;
    F<int32_t>(r, 0x81f) = 0;
    F<int32_t>(r, 0x823) = 0;
    F<int32_t>(r, 0x827) = 0;
    r[0x82b] = 0;
    F<int32_t>(r, 0x82c) = 0;
    F<int32_t>(r, 0x830) = 0;
    for (int k = 0; k < 0x5b; ++k) r[0x834 + k] = 0;
    F<int32_t>(r, 0) = 0;
    for (int i = 0; i < 4; ++i) {
        F<int32_t>(r, 4 + i * 4) = 0;
        r[0x7fb + i] = 0;
        F<int32_t>(r, 0x80f + i * 4) = -1;
        F<int32_t>(r, 0x7ff + i * 4) = 0;
    }
    return r;
}
RT3_REPLACE(0x42d5a0, Record42d5a0_Init);

void __fastcall Record42d5a0_SetName(uint8_t* r, int, int32_t v, const char* name) {
    F<int32_t>(r, 0x14) = v;
    char* d = (char*)r + 0x18;
    while ((*d++ = *name++) != 0) {}
}
RT3_REPLACE(0x42d670, Record42d5a0_SetName);

// 0x42d690. Frees the list whose head is *head (next at +0x1a).
RT3_NOINLINE void __fastcall List1a_Free(uint8_t** head) {
    while (*head) {
        uint8_t* next = *(uint8_t**)(*head + 0x1a);
        crt::operator_delete(*head);
        *head = next;
    }
    *head = nullptr;
}
RT3_REPLACE(0x42d690, List1a_Free);

// 0x42d6c0. Frees the four lists at +4..+0x10 (next at +0x24).
RT3_NOINLINE void __fastcall Obj_FreeFourLists(void* o) {
    for (int k = 0; k < 4; ++k) {
        uint8_t*& head = F<uint8_t*>(o, 4 + k * 4);
        while (head) {
            uint8_t* next = *(uint8_t**)(head + 0x24);
            crt::operator_delete(head);
            head = next;
        }
        head = nullptr;
    }
}
RT3_REPLACE(0x42d6c0, Obj_FreeFourLists);

// 0x42d700. Cargo flags of the load list at *this (next +0x1a), from the cargo
// table 0x5f3e04 (0x81-byte records). 0x7f if +0x82b, or if an entry is -1.
// (AL only, as in the original.)
RT3_NOINLINE uint8_t __fastcall Load_CargoFlags(void* o) {
    if (F<uint8_t>(o, 0x82b)) return 0x7f;
    uint8_t flags = 0;
    for (int32_t* n = F<int32_t*>(o, 0); n; n = *(int32_t**)((uint8_t*)n + 0x1a)) {
        if (*n < 0) return *n == -1 ? 0x7f : 0;
        flags |= *((const uint8_t*)0x5f3e04 + *n * 0x81);
    }
    return flags;
}
RT3_REPLACE(0x42d700, Load_CargoFlags);

// 0x42d740. Flags of list i (+4 + i*4, next +0x24) from table 0x6103fd
// (0x6e-byte records). With `classify`, kinds 11/13/15/127 count as 8.
RT3_NOINLINE uint8_t __fastcall Obj_ListFlags(void* o, int, int32_t i, int32_t classify) {
    uint8_t flags = 0;
    for (int32_t* n = F<int32_t*>(o, 4 + i * 4); n; n = *(int32_t**)((uint8_t*)n + 0x24)) {
        uint8_t k = *((const uint8_t*)0x6103fd + *n * 0x6e);
        if (classify && (k == 11 || k == 13 || k == 15 || k == 127)) flags |= 8;
        else flags |= k;
    }
    return flags;
}
RT3_REPLACE(0x42d740, Obj_ListFlags);

// 0x42d820. Converts the [tags] of text in place (max length maxLen): with `encode`,
// names from the tag table (0x5f3d9c, 901 x 0x81 bytes) become their number
// (10000 + index) and the six ~~name] markers (strings 0x620b6c) become ~~2000x];
// otherwise the reverse. Quirk: a line break inside a tag restarts the scan one
// character after the tag's start.
RT3_NOINLINE void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode) {
    if (!*text) return;
    int32_t found = 0;
    char* p = text;
    for (;;) {
        bool replaced = false;
        while (*p == '[') {
            ++p;
            char* tag = p;
            char c;
            while ((c = *p) != 0 && c != ']' && c != '~' && c != '\n' && c != '\r') ++p;
            if (c == '\n' || c == '\r') {
                p = tag + 1;
                if (tag[1] == '[') continue;
                break;
            }
            found = 1;
            char first = (char)ToUpper(*tag);
            char term = *p;
            *p = 0;
            char buf[0x124];
            strcpy(buf, tag);
            if (encode) {
                int32_t idx = 0;
                for (const char* e = (const char*)0x5f3d9c; (uint32_t)e < 0x6103a1; e += 0x81, ++idx)
                    if ((char)ToUpper(*e) == first && !crt::stricmp(tag, e)) crt::sprintf(buf, (const char*)0x5c9cd0, idx + 10000);
            } else {
                int32_t v = crt::atoi(tag) - 10000;
                if (v >= 0 && v < 0x385) crt::sprintf(buf, (const char*)0x5f3d9c + v * 0x81);
            }
            *p = term;
            int32_t start = (int32_t)(tag - text);
            int32_t blen = (int32_t)strlen(buf);
            char big[0x7c0];
            strcpy(big, text);
            strcpy(big + start, buf);
            int32_t end = blen + start;
            strcpy(big + end, p);
            int32_t n = (int32_t)strlen(big);
            if (n <= maxLen) n = maxLen;
            StrCopyN(text, big, n);
            p = text + end;
            replaced = true;
            break;
        }
        if (!replaced) ++p;
        if (!*p) break;
    }
    if (!found) return;
    for (int32_t i = 0; i < 6; ++i) {
        char num[0x134], name[0x124];
        crt::sprintf(num, (const char*)0x5c9cdc, i + 20000);
        crt::sprintf(name, (const char*)0x5c9cd4, GetString(((const int32_t*)0x620b6c)[i]));
        if (encode) Str_Replace(text, maxLen, name, num, 1);
        else Str_Replace(text, maxLen, num, name, 1);
    }
}
RT3_REPLACE(0x42d820, Text_ConvertTags);

// 0x42da90. Encodes the [tags] of the record's six texts (+0xe0 up to 800 chars, the
// rest up to 200) to their numbers.
void __fastcall Record_EncodeTags(uint8_t* r) {
    Text_ConvertTags((char*)r + 0xe0, 0x320, 1);
    static const uint16_t offs[] = { 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
    for (uint16_t off : offs) Text_ConvertTags((char*)r + off, 0xc8, 1);
}
RT3_REPLACE(0x42da90, Record_EncodeTags);

// 0x42db20. Loads the scenario's event rules saved by EventRules_Save (version
// 0x526d+ effects / 0x526e+ triggers carry all fields; 0x526f+ texts tag-converted):
// re-links the trigger and four effect lists, re-numbering cargo / building /
// industry references by their saved names.
void __fastcall EventRules_Load(uint8_t* o, int, Stream st) {
    int32_t version;
    Rd(st, &version, 4);
    if (version >= 0x526f) {
        Text_ConvertTags((char*)o + 0xe0, 0x320, 0);
        static const uint32_t texts[] = { 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
        for (uint32_t t : texts) Text_ConvertTags((char*)o + t, 0xc8, 0);
    }
    char name[0x68] = {};
    int32_t triggers;
    Rd(st, &triggers, 4);
    auto alloc = [](size_t n) { return (uint8_t*)crt::operator_new(n); };
    Collection* cargos = g_cargoTypes;
    uint8_t* prev = nullptr;
    for (int32_t i = 0; i < triggers; ++i) {
        uint8_t* e = alloc(0x1e);
        Rd(st, e, 4);
        Rd(st, e + 4, 1);
        Rd(st, e + 5, 4);
        Rd(st, e + 9, 4);
        Rd(st, e + 0xd, 1);
        if (version >= 0x526e) {
            Rd(st, e + 0xe, 4);
            Rd(st, e + 0x12, 4);
        }
        int32_t kind = F<int32_t>(e, 0);
        if (kind >= 0x83 && kind <= 0x35e) {
            int32_t base = kind - (kind - 0x83) % 0x3d;
            int32_t idx = ReadIndex(st, name, [&](const char* n) { return Table_FindByName(cargos, 0, n); });
            if (idx > 0 && idx <= 0x46) F<int32_t>(e, 0) = idx + base;
        }
        F<uint8_t*>(e, 0x1a) = nullptr;
        if (i == 0) F<uint8_t*>(o, 0) = e;
        else F<uint8_t*>(prev, 0x1a) = e;
        prev = e;
    }
    StreamSkip4(st, 0x4eb8, 0, 0);
    int32_t counts[4];
    for (int32_t k = 0; k < 4; ++k) {
        Rd(st, &counts[k], 4);
        for (int32_t i = 0; i < counts[k]; ++i) {
            uint8_t* e = alloc(0x28);
            Rd(st, e, 4);
            Rd(st, e + 4, 4);
            Rd(st, e + 8, 1);
            Rd(st, e + 9, 4);
            Rd(st, e + 0xd, 4);
            if (version >= 0x526d) {
                Rd(st, e + 0x11, 4);
                Rd(st, e + 0x12, 4);
                Rd(st, e + 0x14, 4);
                Rd(st, e + 0x16, 4);
                Rd(st, e + 0x18, 0xc);
            }
            int32_t kind = F<int32_t>(e, 0), base = -1;
            if (kind >= 0xfb && kind <= 0x15f) base = 0xfb;
            else if (kind >= 0x160 && kind <= 0x1c4) base = 0x160;
            if (base != -1) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) {
                    return Table_FindByAltName(g_6ada84, 0, n);
                });
                if (idx > 0 && idx <= 0x64) F<int32_t>(e, 0) = idx + base;
            }
            kind = F<int32_t>(e, 0);
            base = -1;
            if (kind >= 0x6a && kind <= 0xb0) base = 0x6a;
            else if (kind >= 0xb4 && kind <= 0xfa) base = 0xb4;
            if (base != -1) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) { return Table_FindByName(cargos, 0, n); });
                if (idx > 0 && idx <= 0x46) F<int32_t>(e, 0) = idx + base;
            }
            kind = F<int32_t>(e, 0);
            if (kind >= 0x1f7 && kind <= 0x265) {
                int32_t idx = ReadIndex(st, name, [&](const char* n) {
                    return Table_FindByName2(g_62b268, 0, n, 0);
                });
                if (idx > 0 && idx <= 0x6e) F<int32_t>(e, 0) = idx + 0x1f7;
            }
            F<uint8_t*>(e, 0x24) = nullptr;
            if (i == 0) F<uint8_t*>(o, 4 + k * 4) = e;
            else F<uint8_t*>(prev, 0x24) = e;
            prev = e;
        }
    }
    StreamSkip4(st, 0x4eb9, 0, 0);
}
RT3_REPLACE(0x42db20, EventRules_Load);

int32_t __fastcall Record_AnyKind1(uint8_t* r) { return AnyKind(r, 1, r[0x7f9] != 0); }
RT3_REPLACE(0x42df30, Record_AnyKind1);

int32_t __fastcall Record_AnyKind2(uint8_t* r) { return AnyKind(r, 2, r[0x7fa] != 0); }
RT3_REPLACE(0x42df70, Record_AnyKind2);

int32_t __fastcall Record_AnyKind4(uint8_t* r) { return AnyKind(r, 4, F<int32_t>(r, 0x7f0) != 0x63); }
RT3_REPLACE(0x42dfb0, Record_AnyKind4);

int32_t __fastcall Record_AnyKind7(uint8_t* r) {
    return AnyKind(r, 7, r[0x7f9] || r[0x7fa] || F<int32_t>(r, 0x7f0) != 0x63);
}
RT3_REPLACE(0x42dff0, Record_AnyKind7);

// 0x42e050. Copies record src into dst: its six name strings, flags and the
// 4-slot tables, plus deep copies of its event list (+0, 0x1e-byte nodes) and four
// condition lists (+4.., 0x28-byte nodes).
void __fastcall Record_CopyFrom(uint8_t* dst, int, uint8_t* src) {
    static const uint16_t strs[] = { 0xe0, 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
    for (uint16_t off : strs) strcpy((char*)dst + off, (const char*)src + off);
    dst[0x7ee] = src[0x7ee];
    dst[0x7ef] = src[0x7ef];
    F<uint32_t>(dst, 0x7f0) = F<uint32_t>(src, 0x7f0);
    dst[0x7f4] = src[0x7f4];
    F<uint32_t>(dst, 0x7f5) = F<uint32_t>(src, 0x7f5);
    dst[0x7f9] = src[0x7f9];
    dst[0x7fa] = src[0x7fa];
    for (int k = 0; k < 4; ++k) {
        F<uint32_t>(dst, 4 + k * 4) = 0;
        dst[0x7fb + k] = src[0x7fb + k];
        F<uint32_t>(dst, 0x7ff + k * 4) = F<uint32_t>(src, 0x7ff + k * 4);
        F<uint32_t>(dst, 0x80f + k * 4) = F<uint32_t>(src, 0x80f + k * 4);
    }
    CopyList(F<uint8_t*>(src, 0), &F<uint8_t*>(dst, 0), 0x1e, 0x1a);
    for (int k = 0; k < 4; ++k) CopyList(F<uint8_t*>(src, 4 + k * 4), &F<uint8_t*>(dst, 4 + k * 4), 0x28, 0x24);
}
RT3_REPLACE(0x42e050, Record_CopyFrom);

// 0x42e2b0. Any entry of kind 4..7 in the four lists +4.. (next +0x24).
int32_t __fastcall Obj_AnyKind4to7(void* o) {
    for (int k = 0; k < 4; ++k)
        for (uint8_t* e = F<uint8_t*>(o, 4 + k * 4); e; e = F<uint8_t*>(e, 0x24))
            if (F<int32_t>(e, 0) >= 4 && F<int32_t>(e, 0) <= 7) return 1;
    return 0;
}
RT3_REPLACE(0x42e2b0, Obj_AnyKind4to7);

RT3_NOINLINE int32_t __fastcall Company_Sum42e300(void* c) {
    return F<int32_t>(c, 0xd59) - F<int32_t>(c, 0xd5d) + F<int32_t>(c, 0xd55) + F<int32_t>(c, 0xd51);
}
RT3_REPLACE(0x42e300, Company_Sum42e300);

RT3_NOINLINE int32_t __fastcall Company_Sum42e320(void* c) {
    return F<int32_t>(c, 0xd59) - F<int32_t>(c, 0xd69) - F<int32_t>(c, 0xd65) - F<int32_t>(c, 0xd61) +
           F<int32_t>(c, 0xd55) + F<int32_t>(c, 0xd51);
}
RT3_REPLACE(0x42e320, Company_Sum42e320);

// 0x42e350 / 0x42e370 / 0x42e390. Sums of float pairs in the per-year
// blocks (stride 0x38d) starting at +0x1da0; returned unrounded (ST0).
RT3_NOINLINE double __fastcall Company_YearPair1da0(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1da4) + F<float>(b, 0x1da0);
}
RT3_REPLACE(0x42e350, Company_YearPair1da0);

RT3_NOINLINE double __fastcall Company_YearPair1da8(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1dac) + F<float>(b, 0x1da8);
}
RT3_REPLACE(0x42e370, Company_YearPair1da8);

RT3_NOINLINE double __fastcall Company_YearStatPair(uint8_t* c, int, int32_t y, int32_t i) {
    uint8_t* b = c + y * 0x38d + i * 4;
    return (double)F<float>(b, 0x1eb0) + F<float>(b, 0x1ddc);
}
RT3_REPLACE(0x42e390, Company_YearStatPair);

// 0x42e3c0..0x42e4c3. Per-year block (stride 0x38d) sums: float pairs
// returned unrounded, integer sums of the +0x1dc0.. counters.
RT3_NOINLINE double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i) {
    uint8_t* b = c + y * 0x38d + i * 4;
    return (double)F<float>(b, 0x2058) + F<float>(b, 0x1f84);
}
RT3_REPLACE(0x42e3c0, Company_YearStatPair2);

RT3_NOINLINE double __fastcall Company_YearPair1db0(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1db4) + F<float>(b, 0x1db0);
}
RT3_REPLACE(0x42e3f0, Company_YearPair1db0);

RT3_NOINLINE double __fastcall Company_YearPair1db8(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1dbc) + F<float>(b, 0x1db8);
}
RT3_REPLACE(0x42e410, Company_YearPair1db8);

RT3_NOINLINE int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) + F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e430, Company_YearSum3);

RT3_NOINLINE int32_t __fastcall Company_YearNet1(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) - F<int32_t>(b, 0x1dcc) + F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e460, Company_YearNet1);

RT3_NOINLINE int32_t __fastcall Company_YearNet3(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) - F<int32_t>(b, 0x1dd8) - F<int32_t>(b, 0x1dd4) - F<int32_t>(b, 0x1dd0) +
           F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e490, Company_YearNet3);

// 0x42e4d0..0x42e540. The same sums over the totals (+0x7664..) and +0xbc..
RT3_NOINLINE int32_t __fastcall Company_TotalNet1(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x7670) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4d0, Company_TotalNet1);

RT3_NOINLINE int32_t __fastcall Company_TotalNet3(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x767c) - F<int32_t>(c, 0x7678) - F<int32_t>(c, 0x7674) +
           F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4f0, Company_TotalNet3);

RT3_NOINLINE int32_t __fastcall Obj_NetBc1(void* o) {
    return F<int32_t>(o, 0xc4) - F<int32_t>(o, 0xc8) + F<int32_t>(o, 0xc0) + F<int32_t>(o, 0xbc);
}
RT3_REPLACE(0x42e520, Obj_NetBc1);

RT3_NOINLINE int32_t __fastcall Obj_NetBc3(void* o) {
    return F<int32_t>(o, 0xc4) - F<int32_t>(o, 0xd4) - F<int32_t>(o, 0xd0) - F<int32_t>(o, 0xcc) +
           F<int32_t>(o, 0xc0) + F<int32_t>(o, 0xbc);
}
RT3_REPLACE(0x42e540, Obj_NetBc3);

// 0x42e570. Frees the list +0 and the four lists.
RT3_NOINLINE void __fastcall Obj_FreeAllLists(void* o) {
    List1a_Free((uint8_t**)o);
    Obj_FreeFourLists(o);
}
RT3_REPLACE(0x42e570, Obj_FreeAllLists);

// 0x42e580. The value of a scenario-rule variable (condition / effect operand) for
// the given player, company and city/station. Vars 0x83..0x35e are 12 groups of 0x3d
// per-cargo values; the rest are single values. Missing context objects give 0.
RT3_NOINLINE int32_t __stdcall RuleVar_Get(int32_t var, uint8_t* pl, uint8_t* co, uint8_t* st, int32_t p5, int32_t p6, int32_t p7, int32_t p8) {
    uint8_t* w = g_world;
    if (var >= 0x83 && var <= 0x35e) {
        int32_t k = (var - 0x83) % 0x3d;
        switch (var - k) {
        case 0x83: return Ftol(F<float>(w, 0xa26 + k * 4));
        case 0xc0: return Ftol(F<float>(w, 0xafa + k * 4) + (double)F<float>(w, 0xa26 + k * 4));
        case 0xfd: return co ? Ftol(F<float>(co, 0x24f + k * 4)) : 0;
        case 0x13a: return co ? Ftol(F<float>(co, 0x24f + k * 4) + (double)F<float>(co, 0x17b + k * 4)) : 0;
        case 0x177: return pl ? Ftol(F<float>(pl, 0x55 + k * 4)) : 0;
        case 0x1b4: return pl ? Ftol(F<float>(pl, 0x20d + k * 4) + (double)F<float>(pl, 0x55 + k * 4)) : 0;
        case 0x1f1: return pl ? Ftol(F<float>(pl, 0x129 + k * 4)) : 0;
        case 0x22e: return pl ? Ftol(F<float>(pl, 0x2e1 + k * 4) + (double)F<float>(pl, 0x129 + k * 4)) : 0;
        case 0x26b: return pl && co ? Ftol(F<float>(co + *(int32_t*)pl * 0x38d, 0x1ddc + k * 4)) : 0;
        case 0x2a8: return pl && co ? Ftol(Company_YearStatPair(co, 0, *(int32_t*)pl, k)) : 0;
        case 0x2e5: return pl && co ? Ftol(F<float>(co + *(int32_t*)pl * 0x38d, 0x1f84 + k * 4)) : 0;
        case 0x322: return pl && co ? Ftol(Company_YearStatPair2(co, 0, *(int32_t*)pl, k)) : 0;
        }
    }
    uint8_t* fin = hook::As<uint8_t*>(g_tracks);
    Collection* sites = g_stations;
    auto yc = [&](uint32_t off) { return F<float>(co + *(int32_t*)pl * 0x38d, off); };
    auto yi = [&](uint32_t off) { return F<int32_t>(co + *(int32_t*)pl * 0x38d, off); };
    switch (var) {
    case 1: return Ftol((float)CountHumanPlayers());
    case 2: return Ftol((float)Table_CountField3f(g_companies));
    case 3: return st ? F<int32_t>(st, 0x1dd) != 0 : 0;
    case 4: return st ? Ftol(F<double>(st, 0x154)) : 0;
    case 5: return Ftol((float)(F<uint16_t>(w, 0xd) - F<uint16_t>(w, 5)));
    case 6: return F<uint16_t>(w, 5);
    case 7: return F<uint16_t>(w, 0xd);
    case 8: return w[0xf];
    case 9: return Ftol(F<float>(w, 0xbce));
    case 10: return Ftol(F<float>(w, 0xbd2) + (double)F<float>(w, 0xbce));
    case 11: return Ftol(F<float>(w, 0xbd6));
    case 12: return Ftol(F<float>(w, 0xbda) + (double)F<float>(w, 0xbd6));
    case 13: return Ftol((float)F<int32_t>(w, 0x2d));
    case 14: return co ? Ftol(Company_Stat(co, 0, 0xd, 9001)) : 0;
    case 15: {
        // Before version 0x69 in multiplayer the debt is added with the wrong sign.
        if (!co) return 0;
        float cash = (float)Company_Stat(co, 0, 0xd, 9001);
        float neg = 0.0f < cash ? 0.0f : cash;
        if (IsMultiplayer() && Game_EffectiveVersion() < 0x69) return Ftol((double)neg - Company_Stat(co, 0, 0x12, 9001));
        return Ftol(-Company_Stat(co, 0, 0x12, 9001) - neg);
    }
    case 16: {
        if (!co) return 0;
        float cash = (float)Company_Stat(co, 0, 0xd, 9001);
        float neg = 0.0f < cash ? 0.0f : cash;
        return Ftol(Company_Stat(co, 0, 0x30, 9001) - neg);
    }
    case 17: return co ? Ftol(Company_Stat(co, 0, 0x31, 9001)) : 0;
    case 18: return co ? Ftol(Company_Stat(co, 0, 0x2c, 9001)) : 0;
    case 19: return co ? Ftol(Company_Stat(co, 0, 0x2c, 9002)) : 0;
    case 20: return co ? Ftol(Company_Stat(co, 0, 0x2b, 9001)) : 0;
    case 21: return co ? Ftol(Company_Stat(co, 0, 0x2b, 9002)) : 0;
    case 22: return co ? Ftol(Company_Stat(co, 0, 0x16, 9001)) : 0;
    case 23: return co ? Ftol(Company_Stat(co, 0, 0x16, 9002)) : 0;
    case 24: return co ? Ftol(Company_TrackLength(co)) : 0;
    case 25: return st ? Ftol(Player_Wealth(st, 0, 1)) : 0;
    case 26: return st ? Ftol(Player_Wealth(st, 0, 3)) : 0;
    case 27: case 28: case 29: case 30: return F<int32_t>(w, 0xba + var * 4);
    case 0x1f: case 0x20: case 0x21: case 0x22: return co ? F<int32_t>(co, 0x38b + var * 4) : 0;
    case 0x23: case 0x24: case 0x25: case 0x26: return st ? F<int32_t>(st, 0x2db + var * 4) : 0;
    case 0x27: case 0x28: case 0x29: case 0x2a: return pl ? F<int32_t>(pl, 0x3e5 + var * 4) : 0;
    case 0x2b: return Rand(1, 100);
    case 0x2c: return Rand(1, 10000);
    case 0x2d: return co ? Ftol((float)*(int32_t*)co) : 0;
    case 0x2e: return st ? Ftol((float)*(int32_t*)st) : 0;
    case 0x2f: return pl ? Ftol((float)*(uint32_t*)pl) : 0;
    case 0x30: return Collection_IsValid(g_companies, 0, (uint32_t)p5) ? p5 : p5 - 1;
    case 0x31: return Collection_IsValid(g_players, 0, (uint32_t)p5) ? p5 : p5 - 1;
    case 0x32: return st ? F<int32_t>(st, 0x1dd) : 0;
    case 0x33: return co ? Ftol(Company_Stat(co, 0, 3, 9001)) : 0;
    case 0x34: return co ? Ftol(Company_Stat(co, 0, 3, 9002)) : 0;
    case 0x35: return co ? Ftol(Company_Stat(co, 0, 0x26, 9001)) : 0;
    case 0x36: return co ? Ftol(Company_Stat(co, 0, 0x26, 9002)) : 0;
    case 0x37: return co ? Ftol(Company_Stat(co, 0, 0x27, 9001)) : 0;
    case 0x38: return co ? Ftol(Company_Stat(co, 0, 0x27, 9002)) : 0;
    case 0x39: return co ? Ftol(Company_Stat(co, 0, 0x28, 9001)) : 0;
    case 0x3a: return co ? Ftol(Company_Stat(co, 0, 0x28, 9002)) : 0;
    case 0x3b: return g_newGame[0x7d];
    case 0x3c: return F<uint16_t>(w, 0xd) * 100 + w[0xf];
    case 0x3d: return pl ? Ftol(F<float>(pl, 0x45)) : 0;
    case 0x3e: return pl ? Ftol(F<float>(pl, 0x1fd) + (double)F<float>(pl, 0x45)) : 0;
    case 0x3f: return hook::As<void*>(g_session) != nullptr;
    case 0x40: return pl ? Ftol(F<float>(pl, 0x49)) : 0;
    case 0x41: return pl ? Ftol(F<float>(pl, 0x201) + (double)F<float>(pl, 0x49)) : 0;
    // These check only the company, not the player.
    case 0x42: return co ? Ftol(yc(0x1da0)) : 0;
    case 0x43: return co ? Ftol(Company_YearPair1da0(co, 0, *(int32_t*)pl)) : 0;
    case 0x44: return co ? Ftol(yc(0x1da8)) : 0;
    case 0x45: return co ? Ftol(Company_YearPair1da8(co, 0, *(int32_t*)pl)) : 0;
    case 0x46: return co ? Ftol(yc(0x1db0)) : 0;
    case 0x47: return co ? Ftol(Company_YearPair1db0(co, 0, *(int32_t*)pl)) : 0;
    case 0x48: return co ? Ftol(yc(0x1db8)) : 0;
    case 0x49: return co ? Ftol(Company_YearPair1db8(co, 0, *(int32_t*)pl)) : 0;
    case 0x4a: return co ? Company_EntryFlag(co, 0, *(int32_t*)pl) : 0;
    case 0x4b: return pl ? Stations_CargoLinked(sites, 0, *(int32_t*)pl, p7, 1, 1, -1) != 0 : 0;
    case 0x4c: return pl ? Stations_CargoLinked(sites, 0, *(int32_t*)pl, p7, 1, 0, -1) != 0 : 0;
    case 0x4d: return pl && co ? Stations_CargoLinked(sites, 0, *(int32_t*)pl, p7, 1, 1, *(int32_t*)co) != 0 : 0;
    case 0x4e: return pl && co ? Stations_CargoLinked(sites, 0, *(int32_t*)pl, p7, 1, 0, *(int32_t*)co) != 0 : 0;
    case 0x4f: return F<int32_t>(fin, 0xc4) + F<int32_t>(fin, 0xc0) + F<int32_t>(fin, 0xbc);
    case 0x50: return F<int32_t>(fin, 0xbc);
    case 0x51: return F<int32_t>(fin, 0xc0);
    case 0x52: return F<int32_t>(fin, 0xc4);
    case 0x53: return F<int32_t>(fin, 0xc8);
    case 0x54: return Obj_NetBc1(fin);
    case 0x55: return F<int32_t>(fin, 0xcc);
    case 0x56: return F<int32_t>(fin, 0xd0);
    case 0x57: return F<int32_t>(fin, 0xd4);
    case 0x58: return Obj_NetBc3(fin);
    case 0x59: return co ? F<int32_t>(co, 0x766c) + F<int32_t>(co, 0x7668) + F<int32_t>(co, 0x7664) : 0;
    case 0x5a: return co ? F<int32_t>(co, 0x7664) : 0;
    case 0x5b: return co ? F<int32_t>(co, 0x7668) : 0;
    case 0x5c: return co ? F<int32_t>(co, 0x766c) : 0;
    case 0x5d: return co ? F<int32_t>(co, 0x7670) : 0;
    case 0x5e: return co ? Company_TotalNet1(co) : 0;
    case 0x5f: return co ? F<int32_t>(co, 0x7674) : 0;
    case 0x60: return co ? F<int32_t>(co, 0x7678) : 0;
    case 0x61: return co ? F<int32_t>(co, 0x767c) : 0;
    case 0x62: return co ? Company_TotalNet3(co) : 0;
    case 0x63: return pl ? F<int32_t>(pl, 0xd59) + F<int32_t>(pl, 0xd55) + F<int32_t>(pl, 0xd51) : 0;
    case 0x64: return pl ? F<int32_t>(pl, 0xd51) : 0;
    case 0x65: return pl ? F<int32_t>(pl, 0xd55) : 0;
    case 0x66: return pl ? F<int32_t>(pl, 0xd59) : 0;
    case 0x67: return pl ? F<int32_t>(pl, 0xd5d) : 0;
    case 0x68: return pl ? Company_Sum42e300(pl) : 0;
    case 0x69: return pl ? F<int32_t>(pl, 0xd61) : 0;
    case 0x6a: return pl ? F<int32_t>(pl, 0xd65) : 0;
    case 0x6b: return pl ? F<int32_t>(pl, 0xd69) : 0;
    case 0x6c: return pl ? Company_Sum42e320(pl) : 0;
    case 0x6d: return pl && co ? Company_YearSum3(co, 0, *(int32_t*)pl) : 0;
    case 0x6e: return pl && co ? yi(0x1dc0) : 0;
    case 0x6f: return pl && co ? yi(0x1dc4) : 0;
    case 0x70: return pl && co ? yi(0x1dc8) : 0;
    case 0x71: return pl && co ? yi(0x1dcc) : 0;
    case 0x72: return pl && co ? Company_YearNet1(co, 0, *(int32_t*)pl) : 0;
    case 0x73: return pl && co ? yi(0x1dd0) : 0;
    case 0x74: return pl && co ? yi(0x1dd4) : 0;
    case 0x75: return pl && co ? yi(0x1dd8) : 0;
    case 0x76: return pl && co ? Company_YearNet3(co, 0, *(int32_t*)pl) : 0;
    case 0x77: return Table_CountFlag201_40((Collection*)fin);
    case 0x78: {
        int32_t total = F<int32_t>(fin, 0xc4) + F<int32_t>(fin, 0xc0) + F<int32_t>(fin, 0xbc);
        int32_t n = total < 1 ? 1 : total;
        int32_t flagged = Table_CountFlag201_40((Collection*)fin);
        return Ftol((double)flagged * D_205(0x5c85d8) / (double)n);
    }
    case 0x79: return Table_CountActiveSites(sites, 0, 0);
    case 0x7a: return Table_CountActiveSites(sites, 0, 1);
    case 0x7b: {
        int32_t all = Table_CountActiveSites(sites, 0, 0);
        int32_t n = all < 1 ? 1 : all;
        int32_t lit = Table_CountActiveSites(g_stations, 0, 1);
        return Ftol((double)lit * D_205(0x5c85d8) / (double)n);
    }
    // Names, as string pointers; the empty string without the object.
    case 0x7c: return co ? (int32_t)(uintptr_t)(co + 4) : g_5ee270;
    case 0x7d: return st ? (int32_t)(uintptr_t)(st + 8) : g_5ee270;
    case 0x7e: return pl ? (int32_t)(uintptr_t)(pl + 4) : g_5ee270;
    case 0x7f: return st && co ? F<int32_t>(st, 0x1dd) == *(int32_t*)co : 0;
    case 0x80: return co != nullptr;
    case 0x81: return st != nullptr;
    case 0x82: return pl != nullptr;
    case 0x35f: return Stations_CargoLinked(sites, 0, p7, p8, 0, 0, -1) != 0;
    case 0x360: return co ? Stations_CargoLinked(sites, 0, p7, p8, 0, 0, *(int32_t*)co) != 0 : 0;
    case 0x361: return co ? Company_Get7680(co) : 0;
    case 0x362: return co ? Cities_CountConnected(g_cities, 0, *(int32_t*)co) : 0;
    case 0x363: return co ? Territories_CountLinked(g_territories, 0, *(int32_t*)co) : 0;
    case 0x364: return Ftol(F<float>(w, 0x4caa));
    case 0x365: return Ftol(F<float>(w, 0x4caa) + (double)F<float>(w, 0x4ca6));
    case 0x366: return co ? Ftol(F<float>(co, 0xd5d)) : 0;
    case 0x367: return co ? Ftol(F<float>(co, 0xd61) + (double)F<float>(co, 0xd5d)) : 0;
    case 0x368: return F<int32_t>(Map(), 0x21d6) * 2;
    }
    return 0;
}
RT3_REPLACE(0x42e580, RuleVar_Get);

// 0x42fc90. Map editor: deletes everything of `kind` - 0x14 / 0x15 the
// buildings (0x62b26c, placed: +0x3d4 == 1) without / with vtbl[0x1c],
// 0x13 the trains, 0x12 the built track (+0x216 == 2), 0x11 all of them -
// owned by *owner (when given: buildings +0x276, trains +0x51, track +0x22e)
// in region *region (when given: their grid byte; a train by either end).
// Track goes highlighted run by run, then each marked piece (+0x201 bit 5) is
// deleted with the stations (and their buildings) and trains (by their cars)
// on it, within 0x5c89a4 / 0x5c8900 cells; then the views refresh (0x808,
// 0x659d).
RT3_NOINLINE void __stdcall Editor_DeleteKind(int32_t* owner, uint32_t* region, int32_t kind) {
    if (kind == 0x14 || kind == 0x15 || kind == 0x11) {
        for (int32_t i = Collection_Count(g_buildings) - 1; i >= 0; --i) {
            Collection* bs = g_buildings;
            uint8_t* b = (uint8_t*)Collection_Get(bs, 0, Collection_IdAt(bs, 0, (uint32_t)i, 0));
            if (Obj_Flag3d4(b) != 1) continue;
            if (region && *region != Obj_GridByte(b)) continue;
            if (owner && *owner != F<int32_t>(b, 0x276)) continue;
            int32_t r = (*(int32_t(__thiscall**)(uint8_t*))(F<uint8_t*>(b, 0) + 0x70))(b);
            if (r ? kind == 0x15 : kind == 0x14) continue;
            bs = g_buildings;
            CollectionVtbl* vt = bs->vtbl;
            vt->release(g_buildings, Collection_IdAt(bs, 0, (uint32_t)i, 0));
        }
    }
    if (kind == 0x13 || kind == 0x11) {
        for (int32_t i = Collection_Count(g_trains) - 1; i >= 0; --i) {
            Collection* ts = g_trains;
            uint8_t* t = (uint8_t*)Collection_Get(ts, 0, Collection_IdAt(ts, 0, (uint32_t)i, 0));
            if (region && *region != Obj_CityGridByte(t) && *region != Train_GridByteAtCar45(t)) continue;
            if (owner && *owner != F<int32_t>(t, 0x51)) continue;
            ts = g_trains;
            CollectionVtbl* vt = ts->vtbl;
            vt->release(g_trains, Collection_IdAt(ts, 0, (uint32_t)i, 0));
        }
    }
    if (kind != 0x12 && kind != 0x11) return;
    Tracks_ClearHighlights(g_tracks);
    for (int32_t i = Collection_Count(g_tracks) - 1; i >= 0; --i) {
        Collection* ps = g_tracks;
        uint8_t* p = (uint8_t*)Collection_Get(ps, 0, Collection_IdAt(ps, 0, (uint32_t)i, 0));
        if (p[0x216] != 2) continue;
        if (region && *region != Obj_CenterGridByte(p)) continue;
        if (owner && *owner != (int32_t)F<uint16_t>(p, 0x22e)) continue;
        Tracks_HighlightRun(g_tracks, 0, Collection_IdAt(g_tracks, 0, (uint32_t)i, 0), 1, 1);
    }
    for (int32_t i = Collection_Count(g_tracks) - 1; i >= 0; --i) {
        Collection* ps = g_tracks;
        uint8_t* p = (uint8_t*)Collection_Get(ps, 0, Collection_IdAt(ps, 0, (uint32_t)i, 0));
        if (!(p[0x201] & 0x20)) continue;
        float px = Obj_CenterCellX(p);
        float py = Obj_CenterCellY(p);
        for (int32_t j = Collection_Count(g_stations) - 1; j >= 0; --j) {
            Collection* ss = g_stations;
            uint8_t* s = (uint8_t*)Collection_Get(ss, 0, Collection_IdAt(ss, 0, (uint32_t)j, 0));
            float sy = (float)Obj_BuildingCellY(s);
            float sx = (float)Obj_BuildingCellX(s);
            if (Distance2D(px, py, sx, sy) > Fl_207(0x5c89a4)) continue;
            if (!Obj_NodeNear8(s, 0, (int32_t)Collection_IdAt(g_tracks, 0, (uint32_t)i, 0))) continue;
            Collection* bs = g_buildings;
            bs->vtbl->release(bs, F<uint32_t>(s, 4));
        }
        for (bool again = true; again;) {
            again = false;
            for (int32_t j = Collection_Count(g_62ba84) - 1; j >= 0; --j) {
                Collection* cs = g_62ba84;
                uint8_t* c = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)j, 0));
                float cy = (float)Obj_CellY(c);
                float cx = (float)Obj_CellX(c);
                if (Distance2D(px, py, cx, cy) > Fl_207(0x5c8900)) continue;
                if (!Obj_NodeNear356(c, 0, (int32_t)Collection_IdAt(g_tracks, 0, (uint32_t)i, 0))) continue;
                Collection* ts = g_trains;
                ts->vtbl->release(ts, F<uint32_t>(c, 0x390));
                again = true;
                break;
            }
        }
        Tracks_Delete(g_tracks, 0, Collection_IdAt(g_tracks, 0, (uint32_t)i, 0), 0);
    }
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x659d, 0, 0, 0);
}
RT3_REPLACE(0x42fc90, Editor_DeleteKind);

}  // namespace rt3
