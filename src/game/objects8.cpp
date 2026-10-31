// More simulation accessors (batch 11). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table62bae0 = hook::Global<Collection*>(0x62bae0);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);
static Collection*& g_demand      = hook::Global<Collection*>(0x6ada90); // cargo-flow / demand matrix
static uint8_t*& g_cargoTypes = hook::Global<uint8_t*>(0x62ba8c);

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
static inline double ExeSqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }
void __fastcall Obj_RefreshHighlight(void* o);
void* __fastcall Table_GetByName(Collection* t, int, const char* name);
int32_t __fastcall Vehicle_Available(void*, int, uint8_t* rec, int32_t flag);

// 0x413620. Obj_RefreshHighlight on every record.
void __fastcall Table_RefreshAll(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_RefreshHighlight(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x413620, Table_RefreshAll);

// 0x456780. Normalises a float vec3 in place (if its length is > 0); returns it.
float* __fastcall Vec3_Normalize(float* v) {
    float sq = (float)(((double)v[0] * v[0] + (double)v[1] * v[1]) + (double)v[2] * v[2]);
    float len = (float)ExeSqrt(sq);
    if (*(const float*)0x5c8598 < len) {
        float inv = (float)(*(const float*)0x5c85b4 / (double)len);
        v[0] = (float)((double)inv * v[0]);
        v[1] = (float)((double)inv * v[1]);
        v[2] = (float)((double)inv * v[2]);
    }
    return v;
}
RT3_REPLACE(0x456780, Vec3_Normalize);

// 0x449df0. Clamps a rect to the map grid (w +0x2155, h +0x2159); true if it is non-empty.
int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    if (*x0 < 0) *x0 = 0;
    if (*y0 < 0) *y0 = 0;
    int32_t maxX = F<int32_t>(m, 0x2155) - 1;
    *x1 = maxX < *x1 ? maxX : *x1;
    int32_t maxY = F<int32_t>(m, 0x2159) - 1;
    *y1 = maxY < *y1 ? maxY : *y1;
    if (*x0 < 0 || *y0 < 0) return 0;
    if (*x1 > F<int32_t>(m, 0x2155) - 1 || *y1 > F<int32_t>(m, 0x2159) - 1) return 0;
    if (*x1 < *x0 || *y1 < *y0) return 0;
    return 1;
}
RT3_REPLACE(0x449df0, Map_ClampRect);

// 0x44bd10. Terrain kind (+0x272 of the 0x62bae0 record, 5 if none) of the
// coarse tile (4 cells) at world cell (x, y), clamped to the tile grid.
int32_t __fastcall Map_TerrainKindAt(void* m, int, float x, float y) {
    int32_t cx = (int32_t)((double)x / 4.0f);
    int32_t cy = (int32_t)((double)y / 4.0f);
    int32_t w = F<int32_t>(m, 0x214d), h = F<int32_t>(m, 0x2151);
    cx = cx <= 0 ? 0 : cx;
    if (cx >= w - 1) cx = w - 1;
    cy = cy <= 0 ? 0 : cy;
    if (cy >= h - 1) cy = h - 1;
    uint8_t* t = F<uint8_t*>(m, 0x212d) + (w * cy + cx) * 4;
    uint32_t id = *(uint16_t*)(t + 1) ? *(uint16_t*)(t + 1) : t[0];
    uint8_t* r = (uint8_t*)Collection_Get(g_table62bae0, 0, id);
    return r ? F<int32_t>(r, 0x272) : 5;
}
RT3_REPLACE(0x44bd10, Map_TerrainKindAt);

// 0x461c00. ID of the record whose name at +0x34 or +0x53 equals `name`, or 0.
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (ExeStricmp((const char*)r + 0x34, name) == 0) return Collection_IdAt(t, 0, i, 0);
        if (ExeStricmp((const char*)r + 0x53, name) == 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x461c00, Table_FindByAltName);

// 0x41f9b0. Entries of the +0x37f name table whose 0x62b268 type is enabled
// (+0x7ac), of `kind` (+0x32), and available by `year` (u16 +0x26).
int32_t __fastcall Obj_CountTypesAvailable(void* o, int, int32_t year, int32_t kind) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        uint8_t* r = (uint8_t*)Table_GetByName(g_table62b268, 0, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
        if (r && F<int32_t>(r, 0x7ac) && F<uint8_t>(r, 0x32) == kind && F<uint16_t>(r, 0x26) <= year) ++n;
    }
    return n;
}
RT3_REPLACE(0x41f9b0, Obj_CountTypesAvailable);

// 0x421490. Bracket 0..4 of a value against 50 / 62.5 / 87.5 / 117.5 / 155 (0x5f3a3c..).
int32_t __stdcall Bracket5(int32_t value) {
    const float* t = (const float*)0x5f3a3c;
    float v = (float)value;
    int32_t c = 0;
    if (!(v < t[1])) c = 1;
    if (!(v < t[2])) c = 2;
    if (!(v < t[3])) c = 3;
    return !(v < t[4]) ? 4 : c;
}
RT3_REPLACE(0x421490, Bracket5);

// 0x4125f0. Vehicle entries (0xbc bytes, +0x37, count +0x33) that are unused
// (+0 == 0), defined (+0x2c) and available.
int32_t __fastcall Obj_CountAvailableVehicles(void* o, int, int32_t flag) {
    int32_t n = 0;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc)
        if (F<int32_t>(e, 0) == 0 && F<int32_t>(e, 0x2c) != 0 && Vehicle_Available(o, 0, e, flag)) ++n;
    return n;
}
RT3_REPLACE(0x4125f0, Obj_CountAvailableVehicles);

// 0x413eb0. Static record (28 x 0xb0 bytes at 0x5ef3e0) whose name (+0x10) equals obj +0x123.
void* __fastcall FindStaticRecord(void*, int, uint8_t* obj) {
    for (int k = 0; k < 28; ++k) {
        const char* name = *(const char**)(0x5ef3f0 + k * 0xb0);
        if (name && ExeStricmp(name, (const char*)obj + 0x123) == 0) return (void*)(0x5ef3e0 + k * 0xb0);
    }
    return nullptr;
}
RT3_REPLACE(0x413eb0, FindStaticRecord);

// 0x416590. Resets the global block 0x62b308 (and allocates the per-cell
// buffer 0x62b300 if missing).
void ResetGlobals62b308() {
    uint8_t* b = (uint8_t*)0x62b308;
    for (int i = 0; i < 0x1d3 * 4 + 2; ++i) b[i] = 0;
    hook::Global<int32_t>(0x62b318) = -1;
    hook::Global<int32_t>(0x62ba3a) = -1;
    hook::Global<int32_t>(0x62b7ac) = (int32_t)0xfff0bdc1;
    hook::Global<int32_t>(0x62b7b4) = (int32_t)0xfff0bdc1;
    hook::Global<int32_t>(0x62b7a8) = 0xf423f;
    hook::Global<int32_t>(0x62b7b0) = 0xf423f;
    hook::Global<uint8_t>(0x62b270) = 0;
    if (hook::Global<void*>(0x62b300)) return;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    uint32_t n = F<int32_t>(map, 0x2159) * F<int32_t>(map, 0x2155) * 4;
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)(n);
    hook::Global<void*>(0x62b300) = p;
    for (uint32_t i = 0; i < n; ++i) p[i] = 0;
}
RT3_REPLACE(0x416590, ResetGlobals62b308);

// 0x41e650. Cargo demand weight: +0x5a, or for cargoes flagged +0x46 the number
// of open demand entries for its type (0x6ada90: type byte +0x10, flag +0x12
// bit 1, u16 +8 == 0) divided by 5.
RT3_NOINLINE double __fastcall Cargo_DemandWeight(void* c) {
    if (!F<uint8_t>(c, 0x46)) return F<float>(c, 0x5a);
    int32_t type = F<int32_t>(g_cargoTypes, 0x9a + F<int32_t>(c, 0x3e) * 4);
    int32_t total = Collection_Count(g_demand), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* d = (uint8_t*)Collection_Get(g_demand, 0, Collection_IdAt(g_demand, 0, i, 0));
        if (F<uint8_t>(d, 0x10) == type && (F<uint8_t>(d, 0x12) & 2) && F<uint16_t>(d, 8) == 0) ++n;
    }
    float fn = (float)n;
    return (double)fn / *(const float*)0x5c8554;
}
RT3_REPLACE(0x41e650, Cargo_DemandWeight);

// 0x41fa30. Float +0x1e of the +0x37f entry named `name`, or 0.
double __fastcall Obj_NamedValue(void* o, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        if (ExeStricmp(name, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) {
            t = F<Collection*>(o, 0x37f);
            return F<float>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x1e);
        }
    }
    return *(const float*)0x5c8598;
}
RT3_REPLACE(0x41fa30, Obj_NamedValue);

// 0x4204c0. Renames port/warehouse entries of the +0x37f table to
// "Port%02d"/"Warehouse%02d" with number n (once per change of n, +0x2f2).
void __fastcall Obj_NumberPorts(void* o, int, int32_t n) {
    if (n == F<int32_t>(o, 0x2f2)) return;
    auto sprintf_ = (int(__cdecl*)(char*, const char*, ...))0x5a19c4;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        char* name = (char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        uint8_t* r = (uint8_t*)Table_GetByName(g_table62b268, 0, name);
        if (!r) continue;
        if (F<uint8_t>(r, 0xba)) sprintf_(name, (const char*)0x5c93e8, n);      // "Port%02d"
        else if (F<uint8_t>(r, 0xbb)) sprintf_(name, (const char*)0x5c93d8, n); // "Warehouse%02d"
    }
    F<int32_t>(o, 0x2f2) = n;
}
RT3_REPLACE(0x4204c0, Obj_NumberPorts);

} // namespace rt3
