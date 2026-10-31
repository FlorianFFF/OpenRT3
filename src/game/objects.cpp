// Small accessors on game objects and tables (simulation band, 0x40xxxx-0x4fxxxx).
// World units: one map cell is 92 units.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings = hook::Global<Collection*>(0x62b26c);  // industries/stations/houses
static Collection*& g_table62ba88 = hook::Global<Collection*>(0x62ba88);

static const float kCellUnits = 92.0f;   // 0x5c8700
static const float kTwoCells = 184.0f;   // 0x5cfcfc

// 0x434050. True in a multiplayer game (the lobby object exists).
RT3_NOINLINE int32_t IsMultiplayer() { return hook::Global<void*>(0x6cd8d8) != nullptr ? 1 : 0; }
RT3_REPLACE(0x434050, IsMultiplayer);

// 0x455800 / 0x455810. Object position in cells.
// x87: FLD float / FDIV float, kept in the FPU, so computed in double here.
RT3_NOINLINE double __fastcall Obj_CellX(void* o) { return (double)F<float>(o, 0x1e2) / kCellUnits; }
RT3_REPLACE(0x455800, Obj_CellX);
RT3_NOINLINE double __fastcall Obj_CellY(void* o) { return (double)F<float>(o, 0x1ea) / kCellUnits; }
RT3_REPLACE(0x455810, Obj_CellY);

RT3_NOINLINE int32_t __fastcall Obj_Get21(void* o) { return F<int32_t>(o, 0x21); }
RT3_REPLACE(0x4337a0, Obj_Get21);
RT3_NOINLINE int32_t __fastcall Obj_Get25(void* o) { return F<int32_t>(o, 0x25); }
RT3_REPLACE(0x4337b0, Obj_Get25);

uint8_t __fastcall Obj_Flag3d4(void* o) { return F<uint8_t>(o, 0x3d4); }
RT3_REPLACE(0x40c990, Obj_Flag3d4);

// 0x41adb0. The table-0x62ba88 record referenced at +0x388.
RT3_NOINLINE void* __fastcall Obj_Ref388(void* o) { return Collection_Get(g_table62ba88, 0, F<uint32_t>(o, 0x388)); }
RT3_REPLACE(0x41adb0, Obj_Ref388);

// 0x47de00. The building referenced at +4 (NULL if none).
RT3_NOINLINE void* __fastcall Obj_Building(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return id ? Collection_Get(g_buildings, 0, id) : nullptr;
}
RT3_REPLACE(0x47de00, Obj_Building);

// 0x47efe0. Cargo value (+0x276) of the building referenced at +4.
// QUIRK: with no building it reads address 0x276 (crashes), as the original.
int32_t __fastcall Obj_BuildingValue(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    volatile uint8_t* b = id ? (uint8_t*)Collection_Get(g_buildings, 0, id) : nullptr;
    return *(volatile int32_t*)(b + 0x276);
}
RT3_REPLACE(0x47efe0, Obj_BuildingValue);

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

// 0x429a50. Count of records with a nonzero dword at +0x3f (e.g. live companies).
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) != 0) ++n;
    return n;
}
RT3_REPLACE(0x429a50, Table_CountField3f);

// 0x477820. Count of records with a nonzero dword at +4.
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t) {
    int32_t total = Collection_Count(t), n = 0;
    for (int32_t i = 0; i < total; ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4) != 0) ++n;
    return n;
}
RT3_REPLACE(0x477820, Table_CountField4);

// 0x477860. The n-th (0-based) record with a nonzero dword at +4, or NULL.
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n) {
    int32_t total = Collection_Count(t);
    for (int32_t i = 0; i < total; ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4) == 0) continue;
        if (n == 0) return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        --n;
    }
    return nullptr;
}
RT3_REPLACE(0x477860, Table_NthField4);

// 0x42a040. Adds to a company statistic: this period's slot (+0xd7f, 0x58
// bytes per stat) and the running total (+0x1c47, 8 per stat), both double.
RT3_NOINLINE void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount) {
    double& period = F<double>(c, 0xd7f + stat * 0x58);
    period = (double)amount + period;
    double& total = F<double>(c, 0x1c47 + stat * 8);
    total = (double)amount + total;
}
RT3_REPLACE(0x42a040, Company_AddStat);

// 0x48a170. Centre of a two-corner object, in cells.
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y) {
    *x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / kTwoCells);
    *y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / kTwoCells);
}
RT3_REPLACE(0x48a170, Obj_CenterCell);

} // namespace rt3

namespace rt3 {

// Global tables (Collection*) referenced by id from objects.
static Collection*& g_companies  = hook::Global<Collection*>(0x62be10);
static Collection*& g_cities     = hook::Global<Collection*>(0x62ba84);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_table6ceb9c = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_table62b2fc = hook::Global<Collection*>(0x62b2fc);

static inline void* RefPositive(Collection* t, int32_t id) { return id > 0 ? Collection_Get(t, 0, id) : nullptr; }
static inline void* RefNotMinus1(Collection* t, int32_t id) { return id != -1 ? Collection_Get(t, 0, id) : nullptr; }

RT3_NOINLINE void* __fastcall Obj_Company21(void* o) { return RefPositive(g_companies, F<int32_t>(o, 0x21)); }
RT3_REPLACE(0x434870, Obj_Company21);
void* __fastcall Obj_Ref25(void* o) { return RefPositive(g_table6ceb9c, F<int32_t>(o, 0x25)); }
RT3_REPLACE(0x4348c0, Obj_Ref25);
RT3_NOINLINE void* __fastcall Obj_Ref3cc(void* o) { return Collection_Get(g_table62b2fc, 0, F<uint32_t>(o, 0x3cc)); }
RT3_REPLACE(0x40cec0, Obj_Ref3cc);
RT3_NOINLINE void* __fastcall Obj_City41(void* o) { return RefNotMinus1(g_cities, F<int32_t>(o, 0x41)); }
RT3_REPLACE(0x4a77b0, Obj_City41);
RT3_NOINLINE void* __fastcall Obj_City352(void* o) { return RefNotMinus1(g_cities, F<int32_t>(o, 0x352)); }
RT3_REPLACE(0x41ad90, Obj_City352);
void* __fastcall Obj_Node206(void* o) { return RefNotMinus1(g_trackNodes, F<int32_t>(o, 0x206)); }
RT3_REPLACE(0x48e140, Obj_Node206);
void* __fastcall Obj_Node20a(void* o) { return RefNotMinus1(g_trackNodes, F<int32_t>(o, 0x20a)); }
RT3_REPLACE(0x48e160, Obj_Node20a);

// 0x45ea60. MSVC "vector constructor iterator": calls fn(elem) for count elements.
void __stdcall VectorConstruct(uint8_t* base, int32_t stride, int32_t count, void(__thiscall* fn)(void*)) {
    for (int32_t i = count - 1; i >= 0; --i, base += stride) fn(base);
}
RT3_REPLACE(0x45ea60, VectorConstruct);

// 0x44ad60. Map byte grid (+0x2131, row stride +0x2155) at (x, y), top 5 bits.
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    uint8_t* p = F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row;
    return p[(int32_t)x] >> 3;
}
RT3_REPLACE(0x44ad60, Map_GridByte);

// 0x448af0. Map cell flags: which 0 = A or B, 1 = B, 2 = A, 3 = C (bitsets
// +0x2139/+0x213d/+0x2141). Outside the map counts as set.
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which) {
    int32_t stride = F<int32_t>(m, 0x215d);
    int32_t idx = stride * y + x;
    if (idx < 0 || idx >= F<int32_t>(m, 0x2161) * stride) return 1;
    auto bit = [&](uint32_t off) { return F<uint8_t*>(m, off)[idx >> 3] & (1 << (idx & 7)); };
    int32_t a = 0, b = 0, c = 0;
    if (which == 0 || which == 2) a = bit(0x2139);
    if (which == 0 || which == 1) b = bit(0x213d);
    if (which == 3) c = bit(0x2141);
    return (a || b || c) ? 1 : 0;
}
RT3_REPLACE(0x448af0, Map_CellFlag);

// 0x448aa0. Float grid (+0x1605, row length +0x15d9 + 1) at (x, y).
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y) {
    return F<float*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * y + x];
}
RT3_REPLACE(0x448aa0, Map_GridFloat);

// 0x4b2bd0 / 0x4b2b60. Elements of a 0x33-byte record array {base, ?, current}.
void* __fastcall Array33_Current(void* a) {
    int32_t i = F<int32_t>(a, 8);
    return i >= 0 ? F<uint8_t*>(a, 0) + i * 0x33 : nullptr;
}
RT3_REPLACE(0x4b2bd0, Array33_Current);
void* __fastcall Array33_At(void* a, int, int32_t i) { return F<uint8_t*>(a, 0) + i * 0x33; }
RT3_REPLACE(0x4b2b60, Array33_At);

// 0x424010. Per-entry flag of a company (0x38d-byte entries from +0x1d9f).
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i) { return F<uint8_t>(c, 0x1d9f + i * 0x38d) != 0 ? 1 : 0; }
RT3_REPLACE(0x424010, Company_EntryFlag);

// 0x48a1a0 / 0x48a1c0. Centre x / y in cells, rounded to float.
float __fastcall Obj_CenterCellX(void* o) { float r = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / kTwoCells); return r; }
RT3_REPLACE(0x48a1a0, Obj_CenterCellX);
float __fastcall Obj_CenterCellY(void* o) { float r = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / kTwoCells); return r; }
RT3_REPLACE(0x48a1c0, Obj_CenterCellY);

double __fastcall Obj_Float18(void* o) { return F<float>(o, 0x18); }
RT3_REPLACE(0x439bd0, Obj_Float18);

// 0x4337c0. Appends {next, name, a..f} (0x20 bytes) to the world's list at +0x66a6.
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c,
                                    int32_t d, int32_t e, int32_t f) {
    int32_t* n = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)(0x20);
    for (int i = 0; i < 8; ++i) n[i] = 0;
    ((void(__fastcall*)(char**, const char*))0x51d820)((char**)(n + 1), name); // String_Assign
    n[2] = a; n[3] = b; n[4] = c; n[5] = d; n[6] = e; n[7] = f;
    n[0] = 0;
    int32_t** head = &F<int32_t*>(w, 0x66a6);
    if (!*head) { *head = n; return; }
    int32_t* p = *head;
    while (*(int32_t**)p) p = *(int32_t**)p;
    *(int32_t**)p = n;
}
RT3_REPLACE(0x4337c0, World_AddNamedEntry);

} // namespace rt3

namespace rt3 {

static Collection*& g_players = hook::Global<Collection*>(0x6ceb9c);     // records with +4 != 0 are active
static Collection*& g_table6ada84 = hook::Global<Collection*>(0x6ada84);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);

// 0x455f60. Region record (0x1d7 bytes, 4x4 cells each) under an object, in
// the region grid 0x62c120. QUIRK kept: computed with negative coordinates
// and subtracted from the base, as in the original.
RT3_NOINLINE uint8_t* __fastcall Obj_Region(void* o) {
    const float neg92 = -92.0f, four = 4.0f; // 0x5cb24c, 0x5c8568
    int32_t row = (int32_t)((double)F<float>(o, 0x1ea) / neg92 / four);
    int32_t col = (int32_t)((double)F<float>(o, 0x1e2) / neg92 / four);
    uint8_t* grid = hook::Global<uint8_t*>(0x62c120);
    return F<uint8_t*>(grid, 0x2129) - (row * F<int32_t>(grid, 0x2145) + col) * 0x1d7;
}
RT3_REPLACE(0x455f60, Obj_Region);

void* __fastcall Obj_Ref388(void* o);

// 0x4a8840. The 0x6ada84 record of this object's city (via the city's +0x388 ref).
RT3_NOINLINE void* __fastcall Obj_CityRef(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_cities, 0, id) : nullptr;
    if (!city) return nullptr;
    return Collection_Get(g_table6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126));
}
RT3_REPLACE(0x4a8840, Obj_CityRef);

// 0x4839e0. Folder for saves/maps from the options object.
const char* __fastcall Options_SaveFolder(void* opt) {
    int32_t kind = F<int32_t>(opt, 0x6c);
    if (kind == 2) return (const char*)0x5ca338;                     // "data\tutorial"
    if (F<int32_t>(opt, 0x68) != 0 || kind == 1) return (const char*)0x5ced78; // "maps"
    return (const char*)0x5ced80;                                     // "saved games"
}
RT3_REPLACE(0x4839e0, Options_SaveFolder);

// 0x425b90. Company: owning player (+0x3b) has the +0x1e1 flag set.
int32_t __fastcall Company_OwnerFlag1e1(void* c) {
    int32_t id = F<int32_t>(c, 0x3b);
    if (id <= 0) return 0;
    return F<int32_t>(Collection_Get(g_players, 0, id), 0x1e1) != 0 ? 1 : 0;
}
RT3_REPLACE(0x425b90, Company_OwnerFlag1e1);

// 0x426ef0. Company: owning player record, if valid.
RT3_NOINLINE void* __fastcall Company_Owner(void* c) {
    if (!Collection_IsValid(g_players, 0, F<uint32_t>(c, 0x3b))) return nullptr;
    return Collection_Get(g_players, 0, F<uint32_t>(c, 0x3b));
}
RT3_REPLACE(0x426ef0, Company_Owner);

// 0x4261b0. Shares not held by any active player: outstanding (+0x47) minus
// each player's holding of this company (player +0x15d + company*4, if > 0).
int32_t __fastcall Company_FreeShares(void* c) {
    int32_t players = Table_CountField4(g_players);
    int32_t held = 0;
    for (int32_t i = 0; i < players; ++i) {
        int32_t h = F<int32_t>(Table_NthField4(g_players, 0, i), 0x15d + F<int32_t>(c, 0) * 4);
        if (h > 0) held += h;
    }
    return F<int32_t>(c, 0x47) - held;
}
RT3_REPLACE(0x4261b0, Company_FreeShares);

// 0x476050. +0x154 (double) += whole amount.
RT3_NOINLINE void __fastcall Obj_AddDouble154(void* o, int, int32_t amount) {
    double& d = F<double>(o, 0x154);
    d = (double)amount + d;
}
RT3_REPLACE(0x476050, Obj_AddDouble154);

// 0x40d360. The state object (vtable slot 32) has kind byte +0x32 == 4.
RT3_NOINLINE int32_t __fastcall Obj_StateKindIs4(void* o) {
    void* st = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return F<uint8_t>(st, 0x32) == 4 ? 1 : 0;
}
RT3_REPLACE(0x40d360, Obj_StateKindIs4);

// 0x41e220. Enabled (+0x56) and its year (+0x21) reached: compared with world
// +0x05 when world +0x46c38 is set, else with the current year (+0x0d).
RT3_NOINLINE int32_t __fastcall Obj_YearReached(void* o) {
    if (F<int32_t>(o, 0x56) == 0) return 0;
    int32_t year = F<int32_t>(g_world, 0x46c38) ? F<uint16_t>(g_world, 0x5) : F<uint16_t>(g_world, 0xd);
    return year >= F<int32_t>(o, 0x21) ? 1 : 0;
}
RT3_REPLACE(0x41e220, Obj_YearReached);

// 0x47ded0 / 0x47df00. Cell position of the building referenced at +4.
double __fastcall Obj_BuildingCellX(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_CellX(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47ded0, Obj_BuildingCellX);
double __fastcall Obj_BuildingCellY(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_CellY(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47df00, Obj_BuildingCellY);

int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i) { return F<int32_t>(o, 0xac + i * 4); }
RT3_REPLACE(0x484ae0, Obj_ArrayAc);

// 0x423ec0. Adds to +0x7680 (unless -1), not below 0.
void __fastcall Company_Add7680(void* c, int, int32_t d) {
    int32_t v = F<int32_t>(c, 0x7680);
    if (v == -1) return;
    v += d;
    F<int32_t>(c, 0x7680) = v < 0 ? 0 : v;
}
RT3_REPLACE(0x423ec0, Company_Add7680);

// 0x4240a0. +0x7680, or 0x752f when the world flag +0x4aaf is off.
int32_t __fastcall Company_Get7680(void* c) {
    return F<int32_t>(g_world, 0x4aaf) ? F<int32_t>(c, 0x7680) : 0x752f;
}
RT3_REPLACE(0x4240a0, Company_Get7680);

// 0x48a090. Track node has a link: (+0x206 or +0x20a) set; with `any`, +0x20e counts too.
int32_t __fastcall Node_HasLink(void* n, int, int32_t any) {
    if (F<int32_t>(n, 0x206) != -1) return 1;
    if (F<int32_t>(n, 0x20a) != -1) return 1;
    if (any && F<int32_t>(n, 0x20e) != -1) return 1;
    return 0;
}
RT3_REPLACE(0x48a090, Node_HasLink);

} // namespace rt3

namespace rt3 {

uint8_t* __fastcall Obj_Region(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
static inline void Free53(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x47de20. Region record under the building referenced at +4.
uint8_t* __fastcall Obj_BuildingRegion(void* o) {
    uint32_t id = F<uint32_t>(o, 4);
    return Obj_Region(id ? Collection_Get(g_buildings, 0, id) : nullptr);
}
RT3_REPLACE(0x47de20, Obj_BuildingRegion);

// 0x4a8bd0. Owning company (+0x51) of a train.
void* __fastcall Train_Company(void* t) { return Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)); }
RT3_REPLACE(0x4a8bd0, Train_Company);

// 0x4c16f0. Company selected in the game window (0x6d0818 +0x90), if valid.
RT3_NOINLINE void* SelectedCompany() {
    uint8_t* w = hook::Global<uint8_t*>(0x6d0818);
    if (!w || F<int32_t>(w, 0x90) <= 0) return nullptr;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x90))) return nullptr;
    return Collection_Get(g_companies, 0, F<uint32_t>(w, 0x90));
}
RT3_REPLACE(0x4c16f0, SelectedCompany);

// 0x40cac0. a[i] - b[i] - c[i] over three float arrays (+0x310/+0x338/+0x360).
RT3_NOINLINE double __fastcall Obj_Remaining(void* o, int, int32_t i) {
    return (double)F<float>(o, 0x310 + i * 4) - F<float>(o, 0x338 + i * 4) - F<float>(o, 0x360 + i * 4);
}
RT3_REPLACE(0x40cac0, Obj_Remaining);

// 0x4839b0. Option +0x1bf on, and (option +0x68 or this +0x6c == 1).
int32_t __fastcall Options_Check1bf(void* self) {
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    if (F<int32_t>(opt, 0x1bf) == 0) return 0;
    return (F<int32_t>(opt, 0x68) != 0 || F<int32_t>(self, 0x6c) == 1) ? 1 : 0;
}
RT3_REPLACE(0x4839b0, Options_Check1bf);

void __fastcall Node_Set212(void* n, int, int32_t v) { F<int32_t>(n, 0x212) = v; }
RT3_REPLACE(0x489f80, Node_Set212);

// 0x4a92a0. Region-grid byte at this object's city.
uint32_t __fastcall Obj_CityGridByte(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_cities, 0, id) : nullptr;
    if (!city) return 0;
    float y = (float)Obj_CellY(city);
    float x = (float)Obj_CellX(city);
    return Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
}
RT3_REPLACE(0x4a92a0, Obj_CityGridByte);

// 0x40cb70. Region-grid byte at this object's own position.
uint32_t __fastcall Obj_GridByte(void* o) {
    float y = (float)Obj_CellY(o);
    float x = (float)Obj_CellX(o);
    return Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
}
RT3_REPLACE(0x40cb70, Obj_GridByte);

// 0x4e39e0. +0x9c if set, else the current year.
RT3_NOINLINE int32_t __fastcall Obj_YearOrNow(void* o) {
    int32_t v = F<int32_t>(o, 0x9c);
    return v ? v : F<uint16_t>(g_world, 0xd);
}
RT3_REPLACE(0x4e39e0, Obj_YearOrNow);

// 0x448f00. Low 3 bits of the cell's 3-byte record (+0x165d, stride +0x15dd) set.
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y) {
    int32_t i = F<int32_t>(m, 0x15dd) * y + x;
    return (F<uint8_t*>(m, 0x165d)[i * 3] & 7) ? 1 : 0;
}
RT3_REPLACE(0x448f00, Map_CellLow3);

// 0x461c80. Address of +0x45 in the 0x62ba88 record referenced at +0x72.
void* __fastcall Obj_Ref72Plus45(void* o) {
    uint32_t id = F<uint32_t>(o, 0x72);
    if (!id) return nullptr;
    uint8_t* r = (uint8_t*)Collection_Get(g_table62ba88, 0, id);
    return r ? r + 0x45 : nullptr;
}
RT3_REPLACE(0x461c80, Obj_Ref72Plus45);

// 0x48a0e0. Track node has a free link slot (+0x206/+0x20a; with `any`, +0x20e too).
int32_t __fastcall Node_HasFreeLink(void* n, int, int32_t any) {
    if (F<int32_t>(n, 0x206) == -1) return 1;
    if (F<int32_t>(n, 0x20a) == -1) return 1;
    if (any && F<int32_t>(n, 0x20e) == -1) return 1;
    return 0;
}
RT3_REPLACE(0x48a0e0, Node_HasFreeLink);

// 0x495540. Frees the singly linked list at +0x1c9 (next at +8).
void __fastcall Obj_FreeList1c9(void* o) {
    uint8_t*& head = F<uint8_t*>(o, 0x1c9);
    for (uint8_t* n = head; n;) {
        uint8_t* next = *(uint8_t**)(n + 8);
        Free53(head);
        head = next;
        n = next;
    }
    head = nullptr;
}
RT3_REPLACE(0x495540, Obj_FreeList1c9);

void* __fastcall Identity(void* o) { return o; }
RT3_REPLACE(0x4b2b70, Identity);

void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v) { F<uint8_t>(c, 0x1d9f + i * 0x38d) = v; }
RT3_REPLACE(0x424030, Company_SetEntryFlag);

// 0x42b2d0. True if the 6-byte record list (+0xef, count +0xeb) has key `k`.
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k) {
    int32_t n = F<int32_t>(o, 0xeb);
    const uint8_t* r = F<uint8_t*>(o, 0xef);
    for (int32_t i = 0; i < n; ++i, r += 6)
        if (*(const uint16_t*)r == k) return 1;
    return 0;
}
RT3_REPLACE(0x42b2d0, Obj_HasKey);

// 0x433230. Frees the buffer at +8 and clears +8 and +0.
void __fastcall Obj_FreeBuffer8(void* o) {
    if (F<void*>(o, 8)) ((void(__cdecl*)(void*))0x5a1145)(F<void*>(o, 8));
    F<void*>(o, 8) = nullptr;
    F<int32_t>(o, 0) = 0;
}
RT3_REPLACE(0x433230, Obj_FreeBuffer8);

void __fastcall Obj_Set21(void* o, int, int32_t v) { F<int32_t>(o, 0x21) = v; }
RT3_REPLACE(0x433790, Obj_Set21);

} // namespace rt3
