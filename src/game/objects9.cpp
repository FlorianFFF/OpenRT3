// More simulation functions (batch 12). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_companies   = hook::Global<Collection*>(0x62be10);
static Collection*& g_players     = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_cities      = hook::Global<Collection*>(0x62ba84);
static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_trackNodes  = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);

static inline uint8_t ConfigByte82() { return hook::Global<uint8_t*>(0x6cec7c)[0x82]; }
void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first);
void* __fastcall Table_GetByName(Collection* t, int, const char* name);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// 0x436710. Cargo-i bonus: own value (+0x8a) plus the company's (+0x35b), the
// player's (given, or the company's owner; +0x2ab) and the region type's (+0x3b5).
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region) {
    int32_t v = F<int32_t>(o, 0x8a + i * 4);
    uint32_t p = player;
    if (company && Collection_IsValid(g_companies, 0, company)) {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, company);
        v += F<int32_t>(c, 0x35b + i * 4);
        if (!p) p = F<uint32_t>(c, 0x3b);
    }
    if (p && Collection_IsValid(g_players, 0, p))
        v += F<int32_t>(Collection_Get(g_players, 0, p), 0x2ab + i * 4);
    if (region && Collection_IsValid(g_table6cfc9c, 0, region))
        v += F<int32_t>(Collection_Get(g_table6cfc9c, 0, region), 0x3b5 + i * 4);
    return v;
}
RT3_REPLACE(0x436710, Obj_CargoBonus);

// 0x4a8100. Re-links this station into its two track nodes (+0x49/+0x4d)
// with the cities' +0x35a values.
void __fastcall Station_RelinkNodes(void* s) {
    if (F<int32_t>(s, 0x49) == -1) return;
    int32_t a = F<int32_t>(Collection_Get(g_cities, 0, F<uint32_t>(s, 0x41)), 0x35a);
    int32_t b = F<int32_t>(Collection_Get(g_cities, 0, F<uint32_t>(s, 0x45)), 0x35a);
    int32_t n1 = F<int32_t>(s, 0x49);
    if (n1 != -1 && Collection_IsValid(g_trackNodes, 0, n1))
        Node_SetLinkPair(Collection_Get(g_trackNodes, 0, F<uint32_t>(s, 0x49)), 0, F<int32_t>(s, 0x25), -2, a);
    int32_t n2 = F<int32_t>(s, 0x4d);
    if (n2 != -1 && Collection_IsValid(g_trackNodes, 0, n2))
        Node_SetLinkPair(Collection_Get(g_trackNodes, 0, F<uint32_t>(s, 0x4d)), 0, F<int32_t>(s, 0x25), -3, b);
}
RT3_REPLACE(0x4a8100, Station_RelinkNodes);

// 0x417790. Rotates (x, y) by `angle` (radians, via the engine's per-degree
// sin/cos tables), negated: out = -(c*x - s*y, s*x + c*y).
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY) {
    const float zero = *(const float*)0x5c8598;
    while (angle < zero) angle = (float)(angle + *(const double*)0x5c96b8);
    int32_t deg = (int32_t)((double)angle * *(const float*)0x5c96b0 / *(const double*)0x5c96a8) % 360;
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    float s = F<float>(e, 0x1136b1 + deg * 4);
    float c = F<float>(e, 0x113c51 + deg * 4);
    *outX = (float)-((double)c * x - (double)s * y);
    *outY = (float)-((double)s * x + (double)c * y);
}
RT3_REPLACE(0x417790, RotateNegated);

// 0x4a6f20. Speed factor of a gradient/curve value: positive: (1 + v/(pi/2)*24.5)^2;
// otherwise 1 + 12v, at least 0.2.
double __stdcall SpeedFactor(float v) {
    if (v > *(const float*)0x5c8598) {
        float r = (float)((double)v / *(const float*)0x5c8764 * *(const double*)0x5d0050 + *(const double*)0x5c8770);
        r = (float)((double)r * r);
        return r;
    }
    float r = (float)((double)v * *(const float*)0x5c8788 + *(const double*)0x5c8770);
    if (r < *(const double*)0x5c8c20) return (float)0.2;
    return (float)(double)r;
}
RT3_REPLACE(0x4a6f20, SpeedFactor);

// 0x420410. Port flags from the +0x37f type list: +0x2f6 any port/warehouse,
// +0x2fa a port (+0xba), +0x2fe a warehouse (+0xbb only).
void __fastcall Obj_UpdatePortFlags(void* o) {
    F<int32_t>(o, 0x2f6) = 0;
    F<int32_t>(o, 0x2fa) = 0;
    F<int32_t>(o, 0x2fe) = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* t = F<Collection*>(o, 0x37f);
        uint8_t* r = (uint8_t*)Table_GetByName(g_table62b268, 0, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
        if (!r || (!F<uint8_t>(r, 0xba) && !F<uint8_t>(r, 0xbb))) continue;
        F<int32_t>(o, 0x2f6) = 1;
        if (F<uint8_t>(r, 0xba)) F<int32_t>(o, 0x2fa) = 1;
        else F<int32_t>(o, 0x2fe) = 1;
    }
}
RT3_REPLACE(0x420410, Obj_UpdatePortFlags);

// 0x422010. +0x23a of the nearest record (with +0x23e == 0) to (x, y) in cells.
int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y) {
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

// 0x426a10. Releases a building from this company, booking the sale.
void __fastcall Company_ReleaseBuilding(void* c, int, uint32_t building, float price) {
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
void __fastcall Company_InvestInBuilding(void* c, int, uint32_t building, float amount) {
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, building);
    if (!b) return;
    ((void(__thiscall*)(void*))(*(void***)b)[0x74 / 4])(b);
    if (ConfigByte82()) return;
    F<float>(b, 0x27a) = (float)((double)amount + F<float>(b, 0x27a));
    Company_AddStat(c, 0, 0x11, amount);
    Company_AddStat(c, 0, 0xd, -amount);
}
RT3_REPLACE(0x426aa0, Company_InvestInBuilding);

// 0x42a250. Company track length: its nodes (u16 +0x22e) of kind 2 (+0x216),
// double-track (+0x218 == 4) counting twice, times 0.5.
double __fastcall Company_TrackLength(void* c) {
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

// 0x42a8e0. dst = file-name part of src minus its last character, cut at the
// first '_'. QUIRK: a src without '\\' makes the original read address 1.
void __fastcall BaseNameNoSuffix(char* dst, const char* src) {
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

// 0x4336d0. World constructor: clears the fields set up later; returns this.
void* __fastcall World_Construct(uint8_t* w) {
    F<int32_t>(w, 0x4cae) = 0;
    F<int32_t>(w, 0x4cb2) = 0;
    for (int k = 0; k < 9; ++k) F<int32_t>(w, 0x46a80 + k * 4) = 0;
    F<int32_t>(w, 0x66b2) = 0;
    F<int32_t>(w, 0x66b6) = 0;
    F<int32_t>(w, 0x46c34) = 0;
    F<int32_t>(w, 0x66ae) = 0;
    return w;
}
RT3_REPLACE(0x4336d0, World_Construct);

// 0x434890. Sets the player (+0x25) and copies its company (+0x1dd) to +0x21.
void __fastcall Obj_SetPlayer(void* o, int, uint32_t player) {
    F<uint32_t>(o, 0x25) = player;
    F<int32_t>(o, 0x21) = F<int32_t>(Collection_Get(g_players, 0, player), 0x1dd);
}
RT3_REPLACE(0x434890, Obj_SetPlayer);

// 0x434d40. Marks the state object of every kind-2 building (+0x7b0 = 1).
void MarkKind2Buildings() {
    for (int32_t i = 0; i < Collection_Count(g_buildings); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0));
        void** vt = *(void***)b;
        uint8_t* st = (uint8_t*)((void*(__thiscall*)(void*))vt[0x80 / 4])(b);
        if (F<uint8_t>(st, 0x32) == 2) {
            st = (uint8_t*)((void*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            F<int32_t>(st, 0x7b0) = 1;
        }
    }
}
RT3_REPLACE(0x434d40, MarkKind2Buildings);

} // namespace rt3
