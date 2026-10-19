// More simulation accessors (batch 9). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_companies = hook::Global<Collection*>(0x62be10);
static Collection*& g_buildings = hook::Global<Collection*>(0x62b26c);
static Collection*& g_trains    = hook::Global<Collection*>(0x6cfcbc);

static inline int ExeStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);

// 0x4138f0. Number of records with +0x3d0 == v.
int32_t __fastcall Table_CountField3d0(Collection* t, int, int32_t v) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3d0) == v) ++n;
    return n;
}
RT3_REPLACE(0x4138f0, Table_CountField3d0);

// 0x414450. Bounds of the point list at +0x10 of `shape`; always 1.
int32_t __fastcall Shape_Bounds(void* self, int, uint8_t* shape, int32_t* out) {
    Points_Bounds(self, 0, (const uint32_t*)(shape + 0x10), out);
    return 1;
}
RT3_REPLACE(0x414450, Shape_Bounds);

// 0x41bf90. Rate +0x39c * +0x3a0 (as float); x1.2 when this is the special
// kind (u16 +0x88 of object 0x62ba8c) and its train (+0x390) has flag +0x198.
double __fastcall Obj_Rate(void* o) {
    float f = (float)((double)F<float>(o, 0x3a0) * F<float>(o, 0x39c));
    if (F<int32_t>(o, 0x38c) != F<uint16_t>(hook::Global<uint8_t*>(0x62ba8c), 0x88)) return f;
    int32_t id = F<int32_t>(o, 0x390);
    volatile uint8_t* t = id != -1 ? (uint8_t*)Collection_Get(g_trains, 0, id) : nullptr;
    if (t[0x198]) f = (float)(f * *(const double*)0x5c8698); // QUIRK: no train reads address 0x198
    return f;
}
RT3_REPLACE(0x41bf90, Obj_Rate);

// 0x41f6e0. The region under this object has key k.
int32_t __fastcall Obj_RegionHasKey(void* o, int, uint16_t k) { return Obj_HasKey(Obj_Region(o), 0, k); }
RT3_REPLACE(0x41f6e0, Obj_RegionHasKey);

// 0x4228b0. Buildings in the chain from u16 +0x383 (next u16 +0x270) whose
// state kind (+0x32) equals `kind`.
int32_t __fastcall Obj_CountBuildingsOfKind(void* o, int, int32_t kind) {
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

// 0x423e40. Weighted average over {int count, ?, float value} entries
// (+0x5f, count +0x5b), accumulated in float; 0 if inactive (+0x3f) or empty.
double __fastcall Obj_WeightedAverage(void* o) {
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

void __fastcall Obj_Set4b(void* o, int, int32_t v) { F<int32_t>(o, 0x4b) = v; }
RT3_REPLACE(0x423fb0, Obj_Set4b);
double __fastcall Company_QualityRaw(void* c) { return F<float>(c, 0xd31); }
RT3_REPLACE(0x4240f0, Company_QualityRaw);

// 0x424230. Index of this name (+0xd65) in the table of object 0x6d4020
// (+0x429b0); the table count if absent.
int32_t __fastcall Company_NameIndex(void* c) {
    auto table = [] { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0); };
    int32_t i = 0;
    for (; i < Collection_Count(table()); ++i) {
        Collection* t = table();
        if (ExeStricmp((const char*)c + 0xd65, (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) break;
    }
    return i;
}
RT3_REPLACE(0x424230, Company_NameIndex);

// 0x4266c0. Buildings this company owns (+0x276) with flag +0x3d4 == 1 and
// virtual slot 28 returning 0.
int32_t __fastcall Company_CountIdleBuildings(void* c) {
    int32_t total = Collection_Count(g_buildings), n = 0;
    for (int32_t i = 0; i < total; ++i) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, Collection_IdAt(g_buildings, 0, i, 0));
        if (F<int32_t>(b, 0x276) != F<int32_t>(c, 0)) continue;
        if (Obj_Flag3d4(b) != 1) continue;
        if (((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x4266c0, Company_CountIdleBuildings);

// 0x426890. Position of this company among the live companies, or -1.
int32_t __fastcall Company_LiveIndex(void* c) {
    int32_t n = Table_CountField3f(g_companies);
    for (int32_t i = 0; i < n; ++i)
        if (F<int32_t>(Table_NthField3f(g_companies, 0, i), 0) == F<int32_t>(c, 0)) return i;
    return -1;
}
RT3_REPLACE(0x426890, Company_LiveIndex);

} // namespace rt3
