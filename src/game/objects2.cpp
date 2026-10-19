// More simulation accessors (batch 5). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_players    = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_cities     = hook::Global<Collection*>(0x62ba84);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_trains     = hook::Global<Collection*>(0x6cfcbc); // record +0x51 = owning company
static Collection*& g_table6ada84 = hook::Global<Collection*>(0x6ada84);

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void* __fastcall Obj_Ref388(void* o);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);

// 0x42a080. Company statistic, and (withCash) the cash accumulators +0x11f7/+0x1caf.
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash) {
    Company_AddStat(c, 0, stat, amount);
    if (withCash) {
        double& cash = F<double>(c, 0x11f7);
        cash = (double)amount + cash;
        double& total = F<double>(c, 0x1caf);
        total = (double)amount + total;
    }
}
RT3_REPLACE(0x42a080, Company_PostStat);

// 0x429a90. The n-th (0-based) record with a nonzero dword at +0x3f, or NULL.
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) == 0) continue;
        if (n == 0) return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        --n;
    }
    return nullptr;
}
RT3_REPLACE(0x429a90, Table_NthField3f);

// 0x4953c0. a - b (as float) wrapped into [-pi, pi]; *wrapped (optional) = 1 if it was.
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b) {
    const double pi = *(const double*)0x5c96a8, twoPi = *(const double*)0x5c96b8, negPi = *(const double*)0x5cfe30;
    float d = (float)((double)a - b);
    if (wrapped) *wrapped = 0;
    if (d > pi) {
        d = (float)(d - twoPi);
    } else if (d < negPi) {
        d = (float)(d + twoPi);
    } else {
        return d;
    }
    if (wrapped) *wrapped = 1;
    return d;
}
RT3_REPLACE(0x4953c0, AngleDelta);

// 0x40fa50. Region record (4x4 cells) at this object's position.
uint8_t* __fastcall Obj_RegionAt(void* o) {
    int32_t col = (int32_t)(Obj_CellX(o) / 4.0f);
    int32_t row = (int32_t)(Obj_CellY(o) / 4.0f);
    uint8_t* grid = hook::Global<uint8_t*>(0x62c120);
    return F<uint8_t*>(grid, 0x2129) + (F<int32_t>(grid, 0x2145) * row + col) * 0x1d7;
}
RT3_REPLACE(0x40fa50, Obj_RegionAt);

// 0x4349a0. Number of active players with the +0x1e1 flag (humans); 1 without a player table.
int32_t CountHumanPlayers() {
    if (!g_players) return 1;
    int32_t n = 0;
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i)
        if (F<int32_t>(Table_NthField4(g_players, 0, i), 0x1e1) != 0) ++n;
    return n;
}
RT3_REPLACE(0x4349a0, CountHumanPlayers);

// 0x41e260. Cargo accepts class `id`: own class (+0x3e), 900 = any, 901 = plain
// freight (none of the passenger/mail/troop flags +0x47..+0x49), 902 = any of them.
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id) {
    if (F<int32_t>(c, 0x3e) == id || id == 900) return 1;
    bool special = F<uint8_t>(c, 0x47) || F<uint8_t>(c, 0x48) || F<uint8_t>(c, 0x49);
    if (id == 901) return special ? 0 : 1;
    if (id == 902) return special ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x41e260, Cargo_MatchesClass);

// 0x448a70. Writes a byte in the coarse grid (+0x162d; row length (width >> shift) + 1).
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v) {
    int32_t row = (F<int32_t>(m, 0x15d9) >> (F<int32_t>(m, 0x15e1) & 31)) + 1;
    F<uint8_t*>(m, 0x162d)[row * y + x] = v;
}
RT3_REPLACE(0x448a70, Map_SetCoarseByte);

// 0x475010. Copies 3 dwords to globals 0x6cea68..0x6cea70.
void __fastcall Store3Globals(int32_t* v) {
    hook::Global<int32_t>(0x6cea68) = v[0];
    hook::Global<int32_t>(0x6cea6c) = v[1];
    hook::Global<int32_t>(0x6cea70) = v[2];
}
RT3_REPLACE(0x475010, Store3Globals);

// 0x47df30 / 0x47df50. Centre (cells) of the track node at +8.
float __fastcall Obj_NodeCenterX(void* o) { return Obj_CenterCellX(Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8))); }
RT3_REPLACE(0x47df30, Obj_NodeCenterX);
float __fastcall Obj_NodeCenterY(void* o) { return Obj_CenterCellY(Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 8))); }
RT3_REPLACE(0x47df50, Obj_NodeCenterY);

// 0x48a5c0. Clears the node's link pairs (+0x21a/+0x238 and +0x21e/+0x23c)
// that match (a, b); -1 matches anything.
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b) {
    if ((a == -1 || F<int32_t>(n, 0x21a) == a) && (b == -1 || F<int32_t>(n, 0x238) == b)) {
        F<int32_t>(n, 0x21a) = -1;
        F<int32_t>(n, 0x238) = -1;
    }
    if ((a == -1 || F<int32_t>(n, 0x21e) == a) && (b == -1 || F<int32_t>(n, 0x23c) == b)) {
        F<int32_t>(n, 0x21e) = -1;
        F<int32_t>(n, 0x23c) = -1;
    }
}
RT3_REPLACE(0x48a5c0, Node_ClearLinks);

int32_t __fastcall Pair_Contains(void* p, int, int32_t v) { return (v == F<int32_t>(p, 4) || v == F<int32_t>(p, 8)) ? 1 : 0; }
RT3_REPLACE(0x494ed0, Pair_Contains);

// 0x4a9020. Field +0x10 of this object's city's 0x6ada84 record (0 if no city).
int32_t __fastcall Obj_CityRef10(void* o) {
    int32_t id = F<int32_t>(o, 0x41);
    void* city = id != -1 ? Collection_Get(g_cities, 0, id) : nullptr;
    if (!city) return 0;
    return F<int32_t>(Collection_Get(g_table6ada84, 0, F<uint32_t>(Obj_Ref388(city), 0x126)), 0x10);
}
RT3_REPLACE(0x4a9020, Obj_CityRef10);

double __fastcall Obj_Float2c0(void* o, int, int32_t i) { return F<float>(o, 0x2c0 + i * 4); }
RT3_REPLACE(0x40ca70, Obj_Float2c0);
double __fastcall Obj_Float2e8(void* o, int, int32_t i) { return F<float>(o, 0x2e8 + i * 4); }
RT3_REPLACE(0x40ca80, Obj_Float2e8);
double __fastcall Obj_Float310(void* o, int, int32_t i) { return F<float>(o, 0x310 + i * 4); }
RT3_REPLACE(0x40ca90, Obj_Float310);

void* __fastcall Obj_Train390(void* o) {
    int32_t id = F<int32_t>(o, 0x390);
    return id != -1 ? Collection_Get(g_trains, 0, id) : nullptr;
}
RT3_REPLACE(0x41b8f0, Obj_Train390);

double One() { return *(const float*)0x5c85b4; } // 1.0f
RT3_REPLACE(0x41d540, One);

// 0x41e9f0. ID of the record whose name (+4) equals `name` (exe stricmp), or 0.
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (((int(__cdecl*)(const char*, const char*))0x5a57cf)(name, (const char*)r + 4) == 0)
            return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x41e9f0, Table_FindByName);

int32_t __fastcall Obj_Count37f(void* o) { return Collection_Count(F<Collection*>(o, 0x37f)); }
RT3_REPLACE(0x41f6a0, Obj_Count37f);

// 0x41fac0. i-th (1-based) record of the table at +0x37f: name to dst, +0x1e to *out.
void __fastcall Obj_Entry37f(void* o, int, int32_t i, char* dst, int32_t* out) {
    Collection* t = F<Collection*>(o, 0x37f);
    const char* r = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i - 1, 0));
    const char* s = r;
    char* d = dst;
    while ((*d++ = *s++) != 0) {}
    *out = F<int32_t>((void*)r, 0x1e);
}
RT3_REPLACE(0x41fac0, Obj_Entry37f);

// 0x4264c0. Number of trains owned by this company (train +0x51 == company id *this).
int32_t __fastcall Company_TrainCount(void* c) {
    int32_t total = Collection_Count(g_trains), n = 0;
    for (int32_t i = 0; i < total; ++i)
        if (F<int32_t>(Collection_Get(g_trains, 0, Collection_IdAt(g_trains, 0, i, 0)), 0x51) == F<int32_t>(c, 0)) ++n;
    return n;
}
RT3_REPLACE(0x4264c0, Company_TrainCount);

} // namespace rt3
