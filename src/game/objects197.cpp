// Larger game functions, part 5 (batch 203). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }

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

void __fastcall EventRules_Load(uint8_t* o, int, Stream st);

// 0x433130. Loads the scenario rule sets saved by RuleSets_Save; returns the sum of
// the three tag reads and the collection load (callers check it).
int32_t __fastcall RuleSets_Load(Collection* c, int, Stream st, int32_t recordSize) {
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

int32_t Global6d07b4Set2();
int32_t __fastcall Cities_AssignBlocks(Collection* cities);

// 0x44a7a0. City owning 4x4 block (x, y) (its region byte when unclaimed); the
// assignment is redone first when marked stale (+0x2201) in the editor.
int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y) {
    if (Global6d07b4Set2() && F<int32_t>(m, 0x2201)) Cities_AssignBlocks(hook::Global<Collection*>(0x62bae0));
    uint8_t* g = hook::Global<uint8_t*>(0x62c120);
    uint8_t* b = F<uint8_t*>(g, 0x212d) + (F<int32_t>(g, 0x214d) * y + x) * 4;
    int32_t id = F<uint16_t>(b, 1);
    return id ? id : b[0];
}
RT3_REPLACE(0x44a7a0, Map_BlockCity);

uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
void __fastcall Industry_BuildCargoLists(uint8_t* o);
void __fastcall Obj_UpdateEntryFlags(void* o);

// Raises the record's minimum year (+0x20, s16) to that of cargo `id`.
static void RaiseYear(uint8_t* rec, uint32_t id) {
    int32_t v = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, id), 0x21);
    int32_t own = F<int16_t>(rec, 0x20);
    if (own > v) v = own;
    F<int16_t>(rec, 0x20) = (int16_t)v;
}

// 0x4120b0. Loads a building type from file `path` (type id `id`, +0xe4): name and
// header, the production recipes (+0x37, 0xbc each; cargo names resolved, minimum
// year raised to theirs), from version 1005 the extra settings (1010+: 0x24 bytes of
// flags at +0xc0); the build cost of ones with +0x4b follows the capacity. Then its
// cargo lists and flags are rebuilt and +0x798 set to its slot in the art table
// 0x5ed338 (ports and warehouses by their prefix), or -1.
void __fastcall BuildingType_Load(uint8_t* o, int, const char* path, int32_t id) {
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
    F<uint8_t*>(o, 0x37) = (uint8_t*)Malloc(F<int32_t>(o, 0x33) * 0xbc + 1);
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
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
    if (!Strnicmp(own, (const char*)0x5c8f94, 4)) memcpy(name, (const void*)0x5c8f94, 5);
    else if (!Strnicmp(own, (const char*)0x5c8e4c, 9)) memcpy(name, (const void*)0x5c8e4c, 10);
    else strcpy(name, own);
    int32_t slot = 0;
    for (uint32_t e = 0x5ed338; e < 0x5edca4; e += 0x24, ++slot)
        if (!Stricmp(name, *(const char**)e)) {
            F<int32_t>(o, 0x798) = slot;
            return;
        }
}
RT3_REPLACE(0x4120b0, BuildingType_Load);

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// Marks fine cell (x, y) (quarter cells) as street unless off the map or on water.
static void Street(int32_t x, int32_t y) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    if (x < 0 || x >= F<int32_t>(m, 0x215d) || y < 0 || y >= F<int32_t>(m, 0x2161)) return;
    if (Terrain_KindIs1Or3to5(m, 0, x / 4, y / 4)) return;
    Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, x, y, 1, 3, 1);
}

// 0x41fb00. Lays out a city's streets on the fine (quarter-cell) grid: cross
// streets on rings around the centre every 2 directions out to radius 42 (longer
// further out, the extent per side from the city's pattern (x + y) & 3) and a
// mirrored 12 x 6 grid by the centre; then within 10 cells the building density is
// cleared (inside 3) or capped by distance, keeping each cell's flag.
void __fastcall City_LayStreets(uint8_t* c) {
    if (F<int32_t>(c, 0x23e)) return;
    const int32_t* dxs = (const int32_t*)0x624b28;
    const int32_t* dys = (const int32_t*)0x624b48;
    int32_t cx = Ftol(Obj_CellX(c) * *(const float*)0x5c8568);
    int32_t cy = Ftol(Obj_CellY(c) * *(const float*)0x5c8568);
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
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t x0 = ccx - 10 < 0 ? 0 : ccx - 10, y0 = ccy - 10 < 0 ? 0 : ccy - 10;
    int32_t x1 = F<int32_t>(m, 0x2155) - 1, y1 = F<int32_t>(m, 0x2159) - 1;
    if (x1 >= ccx + 10) x1 = ccx + 10;
    if (y1 >= ccy + 10) y1 = ccy + 10;
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            float d = (float)Distance2D((float)x, (float)y, (float)ccx, (float)ccy);
            if (d > *(const float*)0x5c85f8) continue;
            uint8_t flag = Terrain_GetFlag(hook::Global<uint8_t*>(0x62c120), 0, x, y);
            if (!(d > *(const float*)0x5c8900) && d == d) Terrain_SetCellCount(hook::Global<uint8_t*>(0x62c120), 0, x, y, 0);
            else if (!(d > *(const float*)0x5c85f8) && d == d) {
                int32_t n = Ftol(((double)d - *(const float*)0x5c8900) * D(0x5c8970) / D(0x5c8bf0) + D(0x5c86b8));
                if (n < (int32_t)Terrain_CellCount(hook::Global<uint8_t*>(0x62c120), 0, x, y))
                    Terrain_SetCellCount(hook::Global<uint8_t*>(0x62c120), 0, x, y, (uint8_t)n);
            }
            Terrain_SetFlag(hook::Global<uint8_t*>(0x62c120), 0, x, y, flag);
        }
    }
}
RT3_REPLACE(0x41fb00, City_LayStreets);

} // namespace rt3
