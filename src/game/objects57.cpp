// Functions across blocks 0x40..0x47 (batch 60). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);

int32_t __fastcall Obj_BuildingValue(void* o);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Table_ActiveSiteNear(Collection* t, int, uint32_t bid, uint32_t siteId);
void __fastcall Obj_RefreshHighlight(void* o);
int32_t Global6d07b4Set();
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall CountMp3Files(const char* dir);
void __fastcall Music_BuildList(uint8_t* o);

static inline double ExeSqrt(double v) { return ((double(__cdecl*)(double))0x5a1634)(v); }

// 0x408280. The company's best site by its per-company entry value (+0x16;
// +1000 for unshared own sites); -1 if none.
int32_t __fastcall Company_BestSite(int32_t* c) {
    int32_t bestId = -1;
    float bestVal = 0.0f;
    int32_t n = Collection_Count(g_table6cec20);
    if (n <= 0) return -1;
    for (int32_t i = 0; i < Collection_Count(g_table6cec20); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        uint8_t* e = F<uint8_t**>(s, 0x5bd)[c[0]];
        if (!e[0] || !e[1] || !F<int32_t>(e, 0xa) || Obj_BuildingValue(s) != c[0]) continue;
        float v = F<float>(F<uint8_t**>(s, 0x5bd)[c[0]], 0x16);
        if (Obj_BuildingValue(s) == c[0] && !F<int32_t>(s, 0x5c1) && !F<int32_t>(s, 0x5c5))
            v = (float)((double)v + *(const float*)0x5c86a0);
        if (bestId == -1 || v > bestVal) {
            bestId = F<int32_t>(s, 0);
            bestVal = v;
        }
    }
    return bestId;
}
RT3_REPLACE(0x408280, Company_BestSite);

// 0x413750. Refreshes every building's highlight for `key` (0: any): lit
// (virtual slot 21, world colour) when an active site is near (kind-4
// buildings) or its region lists the key; otherwise slot 22 / 0x40d2d0.
void __fastcall Buildings_RefreshHighlight(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        uint8_t* r = Obj_Region(b);
        void** vt = *(void***)b;
        bool lit;
        if (Obj_StateKindIs4(b)) {
            lit = Table_ActiveSiteNear(g_table6cec20, 0, F<uint32_t>(b, 0x2a4), key) != 0;
            if (!lit) {
                ((void(__thiscall*)(void*, int32_t, int32_t, int32_t))vt[0x58 / 4])(b, 0, 0, 0);
                continue;
            }
        } else {
            lit = false;
            for (int32_t k = 0; k < F<int32_t>(r, 0xeb); ++k)
                if (!key || F<uint16_t>(F<uint8_t*>(r, 0xef), k * 6) == key) lit = true;
            if (!lit) {
                Obj_RefreshHighlight(b);
                continue;
            }
        }
        ((void(__thiscall*)(void*, int32_t))vt[0x54 / 4])(b, F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ba));
    }
}
RT3_REPLACE(0x413750, Buildings_RefreshHighlight);

RT3_NOINLINE int32_t Global6d07b4Set2() { return Global6d07b4Set() ? 1 : 0; }
RT3_REPLACE(0x4338c0, Global6d07b4Set2);

// 0x44a830. Value +0x23a of the untyped region nearest to cell (x, y) (u16, AX only).
uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y) {
    Collection* regions = hook::Global<Collection*>(0x62bae0);
    float best = 4294967296.0f;
    uint16_t sel = 0;
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        int32_t dx = (int32_t)Obj_CellX(r) - x;
        if (dx < 0) dx = -dx;
        int32_t dy = (int32_t)Obj_CellY(r) - y;
        if (dy < 0) dy = -dy;
        float d = (float)ExeSqrt((double)(dy * dy + dx * dx));
        if (d < best) {
            sel = F<uint16_t>(r, 0x23a);
            best = d;
        }
    }
    return sel;
}
RT3_REPLACE(0x44a830, Regions_NearestValue);

// 0x44ce60. Bounding box (+0x21c6..+0x21d2) and count (+0x21d6) of the
// cells with feature bits 0x3e in the byte grid +0x2135 (if world +0x4af3).
void __fastcall Map_FeatureBounds(uint8_t* m) {
    for (int k = 0; k < 5; ++k) F<int32_t>(m, 0x21c6 + k * 4) = 0;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4af3)) return;
    int32_t w = F<int32_t>(m, 0x2155);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2159); ++y) {
        for (int32_t x = 0; x < w && x < F<int32_t>(m, 0x2155); ++x) {
            if (!(F<uint8_t*>(m, 0x2135)[(w + 1) * y + x] & 0x3e)) continue;
            if (F<int32_t>(m, 0x21d6) == 0) {
                F<int32_t>(m, 0x21c6) = x;
                F<int32_t>(m, 0x21ca) = y;
                F<int32_t>(m, 0x21ce) = x;
                F<int32_t>(m, 0x21d2) = y;
            } else {
                if (x < F<int32_t>(m, 0x21c6)) F<int32_t>(m, 0x21c6) = x;
                if (y < F<int32_t>(m, 0x21ca)) F<int32_t>(m, 0x21ca) = y;
                if (x > F<int32_t>(m, 0x21ce)) F<int32_t>(m, 0x21ce) = x;
                if (y > F<int32_t>(m, 0x21d2)) F<int32_t>(m, 0x21d2) = y;
            }
            F<int32_t>(m, 0x21d6)++;
        }
    }
}
RT3_REPLACE(0x44ce60, Map_FeatureBounds);

// 0x468760. Music player constructor: directory "data\music\" and its file list.
uint8_t* __fastcall Music_Construct(uint8_t* o) {
    F<int32_t>(o, 0x104) = 1;
    F<int32_t>(o, 0x108) = 0;
    F<int32_t>(o, 0x110) = 0;
    F<int32_t>(o, 0x10c) = -1;
    F<int32_t>(o, 0x114) = 0;
    char dir[12];
    for (int k = 0; k < 12; ++k) dir[k] = ((const char*)0x5ce160)[k];
    char* d = (char*)o;
    const char* s = dir;
    while ((*d++ = *s++) != 0) {}
    int32_t n = CountMp3Files((const char*)o);
    F<int32_t>(o, 0x100) = n;
    if (n > 0) Music_BuildList(o);
    return o;
}
RT3_REPLACE(0x468760, Music_Construct);

// 0x476950. AI player aggressiveness: (+0x28d + company age penalty + 100)
// / 10, at least 0.5, capped at 15; humans get 10.
float __fastcall Player_Aggressiveness(uint8_t* p) {
    if (!F<int32_t>(p, 0x1e1)) return *(const float*)0x5c85f8;
    int32_t pen = 0;
    if (uint32_t id = F<uint32_t>(p, 0x1dd)) {
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, id);
        int32_t d = F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(c, 0x43);
        if (d <= 0) d = 0;
        if (d >= 10) d = 10;
        pen = (d - 10) * 7;
    }
    float v = (float)((double)(F<int32_t>(p, 0x28d) + pen + 100) / *(const double*)0x5c85d8 * *(const double*)0x5c85f0);
    double dv = (double)v > *(const double*)0x5c8628 ? (double)v : 0.5;
    float f = (float)dv;
    if ((double)f < *(const double*)0x5c8798) return f;
    return 15.0f;
}
RT3_REPLACE(0x476950, Player_Aggressiveness);

} // namespace rt3
