// More simulation accessors (batch 6). See objects.cpp for conventions.
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_companies  = hook::Global<Collection*>(0x62be10);
static Collection*& g_buildings  = hook::Global<Collection*>(0x62b26c);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_table6cfc9c = hook::Global<Collection*>(0x6cfc9c); // region types
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);

void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Obj_BuildingStateActive(void* o);
static inline double ExeSqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }

// 0x4a81b0. Unhooks this station from its two track nodes (+0x49/+0x4d).
void __fastcall Station_UnlinkNodes(void* s) {
    int32_t a = F<int32_t>(s, 0x49);
    if (a == -1) return;
    if (Collection_IsValid(g_trackNodes, 0, a))
        Node_ClearLinks(Collection_Get(g_trackNodes, 0, F<uint32_t>(s, 0x49)), 0, F<int32_t>(s, 0x25), -2);
    int32_t b = F<int32_t>(s, 0x4d);
    if (b == -1) return;
    if (Collection_IsValid(g_trackNodes, 0, b))
        Node_ClearLinks(Collection_Get(g_trackNodes, 0, F<uint32_t>(s, 0x4d)), 0, F<int32_t>(s, 0x25), -3);
}
RT3_REPLACE(0x4a81b0, Station_UnlinkNodes);

// 0x456710. out = vec * matrix (4x4 floats, translation in row 3), x87 order of the original.
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m) { // returns out (callers use EAX)
    float z = (float)((((double)m[10] * v[2]) + (double)m[2] * v[0]) + (double)m[6] * v[1] + m[14]);
    float y = (float)((((double)m[9] * v[2]) + (double)m[1] * v[0]) + (double)m[5] * v[1] + m[13]);
    float x = (float)((((double)m[8] * v[2]) + (double)m[4] * v[1]) + (double)m[0] * v[0] + m[12]);
    out[1] = y;
    out[2] = z;
    out[0] = x;
    return out;
}
RT3_REPLACE(0x456710, Vec3_Transform);

// 0x44e270. Region type at (x, y): the type record's +0x35 unless it has +0x31, else the raw type.
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    int32_t col = (int32_t)x;
    uint32_t type = (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
    uint8_t* rec = (uint8_t*)Collection_Get(g_table6cfc9c, 0, type);
    if (F<int32_t>(rec, 0x31) == 0) return F<uint32_t>(rec, 0x35);
    return (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
}
RT3_REPLACE(0x44e270, Map_RegionTypeAt);

// 0x492130. Entry for cell (x, y) in per-column lists sorted by y (+0x8c; next at +6).
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y) {
    uint8_t** columns = F<uint8_t**>(o, 0x8c);
    if (!columns) return nullptr;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if ((int32_t)x >= F<int32_t>(map, 0x15d9) - 1 || (int32_t)y >= F<int32_t>(map, 0x15d5) - 1) return nullptr;
    for (uint8_t* n = columns[x]; n; n = *(uint8_t**)(n + 6)) {
        uint16_t k = *(uint16_t*)n;
        if (k > y) return nullptr;
        if (k == y) return n;
    }
    return nullptr;
}
RT3_REPLACE(0x492130, Obj_CellEntry);

// 0x475030. Copies the 3 stored dwords (0x6cea68..) to +0x1b0 of element idx (-1 = all).
void __fastcall Obj_ApplyStored3(void* o, int32_t idx) {
    auto apply = [](uint8_t* e) {
        F<int32_t>(e, 0x1b0) = hook::Global<int32_t>(0x6cea68);
        F<int32_t>(e, 0x1b4) = hook::Global<int32_t>(0x6cea6c);
        F<int32_t>(e, 0x1b8) = hook::Global<int32_t>(0x6cea70);
    };
    if (idx == -1) {
        for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) apply(F<uint8_t*>(o, 0xc + i * 4));
    } else {
        apply(F<uint8_t*>(o, 0xc + idx * 4));
    }
}
RT3_REPLACE(0x475030, Obj_ApplyStored3);

// 0x429990. ID of the n-th (0-based) record with a nonzero +0x3f, or 0.
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3f) == 0) continue;
        if (n == 0) return Collection_IdAt(t, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x429990, Table_NthIdField3f);

// 0x42d690. Frees the list whose head is *head (next at +0x1a).
void __fastcall List1a_Free(uint8_t** head) {
    while (*head) {
        uint8_t* next = *(uint8_t**)(*head + 0x1a);
        ((void(__cdecl*)(void*))0x53b080)(*head);
        *head = next;
    }
    *head = nullptr;
}
RT3_REPLACE(0x42d690, List1a_Free);

// 0x4568a0. Length of a float vec3 (squared sum rounded to float, result too).
double __fastcall Vec3_Length(const float* v) {
    float sq = (float)(((double)v[0] * v[0] + (double)v[1] * v[1]) + (double)v[2] * v[2]);
    float len = (float)ExeSqrt(sq);
    return len;
}
RT3_REPLACE(0x4568a0, Vec3_Length);

// 0x469500. GameSpy: pumps the master-server object (0x6cd970) if present.
void GameSpy_Pump() {
    void* o = hook::Global<void*>(0x6cd970);
    if (!o) return;
    ((void(__fastcall*)(void*))0x58db60)(o);
    ((void(__fastcall*)(void*))0x58def0)(hook::Global<void*>(0x6cd970));
}
RT3_REPLACE(0x469500, GameSpy_Pump);

int32_t __fastcall Obj_Array590(void* o, int, int32_t i) { return F<int32_t>(o, 0x590 + i * 4); }
RT3_REPLACE(0x47dc90, Obj_Array590);
int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i) { return F<int32_t>(o, 0xac + i * 4); }
RT3_REPLACE(0x484af0, Obj_ArrayAc2);

// 0x48dcb0. Owning company (u16 +0x22e) of a track node, if valid.
void* __fastcall Node_Company(void* n) {
    if (!g_companies) return nullptr;
    if (!Collection_IsValid(g_companies, 0, F<uint16_t>(n, 0x22e))) return nullptr;
    return Collection_Get(g_companies, 0, F<uint16_t>(n, 0x22e));
}
RT3_REPLACE(0x48dcb0, Node_Company);

// 0x4955b0. 1 if mode == 1, or +0x1e1 != -1 and kind +0x1fd == 3; else 4. (AL only.)
static uint8_t __fastcall Obj_Mode1Or4_Impl(void* o, int, int32_t mode) {
    if (mode == 1) return 1;
    if ((double)F<float>(o, 0x1e1) != *(const double*)0x5c9bf0 && F<uint8_t>(o, 0x1fd) == 3) return 1;
    return 4;
}
// 0x4a06c7 keeps using ECX after the call.
__declspec(naked) uint8_t __stdcall Obj_Mode1Or4(int32_t) { RT3_KEEP_ECX_EDX(Obj_Mode1Or4_Impl, 1) }
RT3_REPLACE(0x4955b0, Obj_Mode1Or4);

// 0x4b2ba0. Initialises a 0x25-byte cursor record; returns it.
void* __fastcall Cursor33_Init(uint8_t* c) {
    F<int32_t>(c, 0) = 0;
    F<int32_t>(c, 4) = 0;
    F<int32_t>(c, 8) = -1;
    F<int32_t>(c, 0xd) = 0;
    F<uint8_t>(c, 0xc) = 0;
    for (int i = 0; i < 5; ++i) F<int32_t>(c, 0x11 + i * 4) = 0;
    return c;
}
RT3_REPLACE(0x4b2ba0, Cursor33_Init);

// 0x401000. Size class of +0x30e: <= 5 -> 0, <= 13 -> 1, else 2 (written to both outputs).
void __fastcall Obj_SizeClass(void* o, int, int32_t* a, int32_t* b) {
    float v = F<float>(o, 0x30e);
    int32_t c = v <= *(const float*)0x5c8554 ? 0 : v <= *(const float*)0x5c8550 ? 1 : 2;
    *b = c;
    *a = c;
}
RT3_REPLACE(0x401000, Obj_SizeClass);

double __fastcall Obj_Float360(void* o, int, int32_t i) { return F<float>(o, 0x360 + i * 4); }
RT3_REPLACE(0x40cab0, Obj_Float360);

// 0x40d230. Virtual check (slot 28) and the 0x6cec20 record at +0x2a8 is active.
int32_t __fastcall Obj_CheckActive(void* o) {
    if (!((int32_t(__thiscall*)(void*))(*(void***)o)[0x70 / 4])(o)) return 0;
    uint32_t id = F<uint32_t>(o, 0x2a8);
    void* r = id ? Collection_Get(g_table6cec20, 0, id) : nullptr;
    return Obj_BuildingStateActive(r) ? 1 : 0;
}
RT3_REPLACE(0x40d230, Obj_CheckActive);

// 0x40d350. Signed byte +0xb9 of the state object (vtable slot 32).
int32_t __fastcall Obj_StateB9(void* o) {
    void* st = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return (int32_t)F<int8_t>(st, 0xb9);
}
RT3_REPLACE(0x40d350, Obj_StateB9);

// 0x413d80. If the 0x62b268 record (+0x173) has kind 1: outputs u16 +0xcb/+0xcd, returns 1.
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y) {
    uint8_t* r = (uint8_t*)Collection_Get(g_table62b268, 0, F<uint32_t>(o, 0x173));
    if (F<uint8_t>(r, 0x32) != 1) return 0;
    *x = F<uint16_t>(o, 0xcb);
    *y = F<uint16_t>(o, 0xcd);
    return 1;
}
RT3_REPLACE(0x413d80, Obj_GetCell);

// 0x4142e0. Bounding box {minX, maxX, minY, maxY} of a -1-terminated list of
// packed points (x = low 15 bits, y = bits 16..30).
void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out) {
    int32_t minX = 99999, minY = 99999, maxX = -99999, maxY = -99999;
    for (; *p != 0xffffffffu; ++p) {
        int32_t x = *p & 0x7fff, y = (*p >> 16) & 0x7fff;
        if (minX > x) minX = x;
        if (minY > y) minY = y;
        if (maxX < x) maxX = x;
        if (maxY < y) maxY = y;
    }
    out[3] = maxY;
    out[2] = minY;
    out[1] = maxX;
    out[0] = minX;
}
RT3_REPLACE(0x4142e0, Points_Bounds);

// 0x4185a0. Sets bit 0 of the map's mask grid (+0x2135) to `bit` over a rect
// {x0, x1, y0, y1}, skipping cells whose value is 999999.0f (0x497423f0).
void __fastcall Map_MaskRect(void*, int, const int32_t* r, const uint32_t* values, uint8_t bit) {
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t i = 0;
    for (int32_t y = r[2]; y <= r[3]; ++y) {
        for (int32_t x = r[0]; x <= r[1]; ++x, ++i) {
            if (values[i] == 0x497423f0) continue;
            uint8_t* cell = F<uint8_t*>(map, 0x2135) + (F<int32_t>(map, 0x2155) + 1) * y + x;
            *cell ^= (*cell ^ bit) & 1;
        }
    }
}
RT3_REPLACE(0x4185a0, Map_MaskRect);

double __fastcall Company_FloatD7b(void* c) { return F<float>(c, 0xd7b); }
RT3_REPLACE(0x423eb0, Company_FloatD7b);

// 0x424100. Difficulty-scaled quality factor: lo*(1-q) + hi*q with q = +0xd31
// and lo/hi from the tables 0x5f3cd8/0x5f3ce8 by difficulty (config +0x7d).
float __fastcall Company_QualityFactor(void* c) {
    uint32_t d = hook::Global<uint8_t*>(0x6cec7c)[0x7d];
    double lo = ((const float*)0x5f3cd8)[d], hi = ((const float*)0x5f3ce8)[d];
    double q = F<float>(c, 0xd31);
    float r = (float)(lo * (*(const double*)0x5c8770 - q) + hi * q);
    return r;
}
RT3_REPLACE(0x424100, Company_QualityFactor);

// 0x4269b0. Gives a building to this company (+0x276 = company id); outside
// scenario-editor mode also records the price (+0x27a) and books it (stats 17, 13).
void __fastcall Company_TakeBuilding(void* c, int, uint32_t building, float price) {
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, building);
    if (!b) return;
    F<int32_t>(b, 0x276) = F<int32_t>(c, 0);
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    F<float>(b, 0x27a) = price;
    Company_AddStat(c, 0, 0x11, price);
    Company_AddStat(c, 0, 0xd, -price);
}
RT3_REPLACE(0x4269b0, Company_TakeBuilding);

} // namespace rt3
