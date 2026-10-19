// Block 0x42xxxx, functions (batch 21). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);

int __fastcall ToUpper(char c);
void* __fastcall Obj_Ref3cc(void* o);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
int32_t __fastcall Obj_Count37f(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
void __fastcall Obj_SumBuildingWeights(void* o);

static inline uint32_t Obj_Refresh420410(void* o) { return ((uint32_t(__thiscall*)(void*))0x420410)(o); }
static inline void Obj_Entry41fac0(void* o, int32_t i, char* name, void* out) {
    ((void(__thiscall*)(void*, int32_t, char*, void*))0x41fac0)(o, i, name, out);
}
static inline void* Virtual80(void* o) { return ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o); }

// 0x4206b0. Sets the amount (+0x1e) of the named entry in +0x37f: removes
// it when amount <= 0, adds a new {name, amount} entry when missing and
// amount > 0; then refreshes (0x420410).
uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount) {
    Collection* c = F<Collection*>(o, 0x37f);
    int32_t i = 0;
    for (; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        c = F<Collection*>(o, 0x37f);
        if (Stricmp(name, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0))) == 0) break;
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

// 0x420d40. Sums the buildings of the chain +0x383 into +0x30a / +0x306
// (slot 0 values) and +0x30e (capacity of kind-0 types), then the weights.
void __fastcall Obj_SumBuildings(void* o) {
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

// Whether record r (used, or type +0x23e with +0x242 > 0) has an entry
// named `name` (first letter `first`) among its +0x37f entries.
static int HasEntryNamed(uint8_t* r, const char* name, char first) {
    if (F<int32_t>(r, 0x23e) && F<int32_t>(r, 0x242) <= 0) return 0;
    int32_t n = Obj_Count37f(r);
    for (int32_t j = 1; j <= n; ++j) {
        char buf[0x200];
        float out;
        Obj_Entry41fac0(r, j, buf, &out);
        if ((char)ToUpper(buf[0]) == first && Stricmp(buf, name) == 0) return 1;
    }
    return 0;
}

// 0x421d20 / 0x421e30. Records that list the 0x62b268 type `typeId` by
// name: count / the n-th (1-based) record.
int32_t __fastcall Table_CountListing(Collection* t, int, uint32_t typeId) {
    const char* name = (const char*)Collection_Get(g_table62b268, 0, typeId) + 4;
    char first = (char)ToUpper(name[0]);
    int32_t count = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (HasEntryNamed((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), name, first)) ++count;
    return count;
}
RT3_REPLACE(0x421d20, Table_CountListing);
void* __fastcall Table_NthListing(Collection* t, int, uint32_t typeId, int32_t nth) {
    const char* name = (const char*)Collection_Get(g_table62b268, 0, typeId) + 4;
    char first = (char)ToUpper(name[0]);
    int32_t count = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (HasEntryNamed(r, name, first) && ++count == nth) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x421e30, Table_NthListing);

// 0x427260. The company's headquarters site: +0xd57 if valid, otherwise its
// active site with the lowest 10 x level - type bonus (+0x8c: 0/1/2 -> 1/2/3).
void* __fastcall Company_HeadquartersSite(void* c) {
    uint32_t sel = 0;
    uint16_t hq = F<uint16_t>(c, 0xd57);
    if (hq && Collection_IsValid(g_table6cec20, 0, hq)) {
        sel = F<uint16_t>(c, 0xd57);
        if (sel) return Collection_Get(g_table6cec20, 0, sel);
    }
    int32_t best = 999999999;
    for (int32_t i = 0; i < Collection_Count(g_table6cec20); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_BuildingValue(r) != F<int32_t>(c, 0)) continue;
        if (!Obj_BuildingStateActive(r)) continue;
        int32_t s = F<int32_t>(Obj_Building(r), 0x3d5) * 10;
        switch (F<uint8_t>(Virtual80(Obj_Building(r)), 0x8c)) {
        case 0: s -= 1; break;
        case 1: s -= 2; break;
        case 2: s -= 3; break;
        }
        if (s < best) {
            best = s;
            sel = F<uint32_t>(r, 0);
        }
    }
    return sel ? Collection_Get(g_table6cec20, 0, sel) : nullptr;
}
RT3_REPLACE(0x427260, Company_HeadquartersSite);

// 0x42c580. Sum of the per-direction cell values (+0xf3[dir], u16) along
// the 8-way line from the station (+0xe7, +0xe9) to (x, y).
float __fastcall Station_PathCost(void* o, int, int32_t x, int32_t y) {
    int32_t cx = F<int16_t>(o, 0xe7), cy = F<int16_t>(o, 0xe9);
    int32_t ax = x - cx < 0 ? cx - x : x - cx;
    int32_t ay = y - cy < 0 ? cy - y : y - cy;
    int32_t steps = ax > ay ? ax : ay;
    float sum = 0.0f;
    if (steps <= 0) return sum;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
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

// 0x42e2b0. Any entry of kind 4..7 in the four lists +4.. (next +0x24).
int32_t __fastcall Obj_AnyKind4to7(void* o) {
    for (int k = 0; k < 4; ++k)
        for (uint8_t* e = F<uint8_t*>(o, 4 + k * 4); e; e = F<uint8_t*>(e, 0x24))
            if (F<int32_t>(e, 0) >= 4 && F<int32_t>(e, 0) <= 7) return 1;
    return 0;
}
RT3_REPLACE(0x42e2b0, Obj_AnyKind4to7);

int32_t __fastcall Company_Sum42e300(void* c) {
    return F<int32_t>(c, 0xd59) - F<int32_t>(c, 0xd5d) + F<int32_t>(c, 0xd55) + F<int32_t>(c, 0xd51);
}
RT3_REPLACE(0x42e300, Company_Sum42e300);
int32_t __fastcall Company_Sum42e320(void* c) {
    return F<int32_t>(c, 0xd59) - F<int32_t>(c, 0xd69) - F<int32_t>(c, 0xd65) - F<int32_t>(c, 0xd61) +
           F<int32_t>(c, 0xd55) + F<int32_t>(c, 0xd51);
}
RT3_REPLACE(0x42e320, Company_Sum42e320);

// 0x42e350 / 0x42e370 / 0x42e390. Sums of float pairs in the per-year
// blocks (stride 0x38d) starting at +0x1da0; returned unrounded (ST0).
double __fastcall Company_YearPair1da0(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1da4) + F<float>(b, 0x1da0);
}
RT3_REPLACE(0x42e350, Company_YearPair1da0);
double __fastcall Company_YearPair1da8(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1dac) + F<float>(b, 0x1da8);
}
RT3_REPLACE(0x42e370, Company_YearPair1da8);
double __fastcall Company_YearStatPair(uint8_t* c, int, int32_t y, int32_t i) {
    uint8_t* b = c + y * 0x38d + i * 4;
    return (double)F<float>(b, 0x1eb0) + F<float>(b, 0x1ddc);
}
RT3_REPLACE(0x42e390, Company_YearStatPair);

} // namespace rt3
