// Available cargos / numbered string parser / effect side test / object grid / route split / station search in blocks 0x47..0x4a (batch 133). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
uint32_t __fastcall Routes_Add(Collection* t);
void __fastcall Node_Set212(void* n, int, int32_t v);
void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta);
int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id);
void* __fastcall Obj_Building(void* o);
int32_t __stdcall Obj_FarFromSiteBuilding(uint8_t* obj, void* site);

static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

// 0x47e330. Number of cargos currently available to o.
RT3_NOINLINE int32_t __fastcall Obj_CountAvailableCargos(uint8_t* o) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(Cargos()); ++i)
        if (Obj_CargoStockAndQuota(o, 0, Collection_IdAt(Cargos(), 0, i, 0), nullptr, nullptr)) ++n;
    return n;
}
RT3_REPLACE(0x47e330, Obj_CountAvailableCargos);

// 0x47e620. The n-th (1-based) cargo available to o, with its stock and quota; 0 if none.
RT3_NOINLINE uint32_t __fastcall Obj_NthAvailableCargo(uint8_t* o, int, int32_t n, float* stock, float* quota, int32_t) {
    for (int32_t i = 0; i < Collection_Count(Cargos()); ++i)
        if (Obj_CargoStockAndQuota(o, 0, Collection_IdAt(Cargos(), 0, i, 0), stock, quota) && !--n)
            return Collection_IdAt(Cargos(), 0, i, 0);
    return 0;
}
RT3_REPLACE(0x47e620, Obj_NthAvailableCargo);

// 0x488540. Reads the next line of the form  <number> ... "<text>"  from *cursor:
// *num = the number, *out = a new copy of the text (\n escapes expanded); lines
// without both are skipped. Returns 0 at the end of the text.
int32_t __fastcall Text_NextNumberedString(char** cursor, int32_t* num, char** out) {
    char* p = *cursor;
    do {
        int32_t n = -1;
        char* start = nullptr;
        char* end = nullptr;
        for (int8_t c = *p; c && c != '\n' && c != '\r'; c = *p) {
            if (c >= '0' && c <= '9' && n == -1 && !start) {
                do {
                    n = n == -1 ? c - '0' : n * 10 + c - '0';
                    c = *++p;
                } while (c >= '0' && c <= '9');
            } else if (c == '"' && !start) {
                c = *++p;
                start = p;
                char* quote = nullptr;
                for (; c && c != '\n'; c = *++p) {
                    if (c == '"') quote = p;
                    else if (c > ' ') quote = nullptr;
                }
                end = quote ? quote - 1 : p - 1;
            } else {
                ++p;
            }
        }
        if (n != -1 && start && end && (uint32_t)(end - 1) >= (uint32_t)start) {
            uint32_t len = (uint32_t)(end - start) + 1;
            char* s = (char*)Malloc(len + 1);
            *out = s;
            if (len) memcpy(s, start, len);
            (*out)[len] = 0;
            Str_Replace(*out, (int32_t)len, (const char*)0x5cf134, (const char*)0x5c9ce8, 1);
            *cursor = p;
            *num = n;
            return 1;
        }
        while (*p == '\n' || *p == '\r') ++p;
    } while (*p);
    *cursor = p;
    return 0;
}
RT3_REPLACE(0x488540, Text_NextNumberedString);

// 0x48a880. Whether (x, y) lies on the positive side of the line through the
// midpoints of the effect's edges (vectors a+b and c+d; reversed unless `forward`).
int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y) {
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(e, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float k = *(const float*)0x5c878c;
    float m1x = (float)(((double)b[0] + a[0]) / k), m1z = (float)(((double)b[2] + a[2]) / k);
    float m2x = (float)(((double)d[0] + c[0]) / k), m2z = (float)(((double)d[2] + c[2]) / k);
    float ax, az, bx, bz;
    if (forward) { ax = m1x; az = m1z; bx = m2x; bz = m2z; }
    else { bx = m1x; bz = m1z; ax = m2x; az = m2z; }
    float dx = (float)((double)bx - ax);
    float dz = (float)((double)bz - az);
    const double zero = *(const double*)0x5c8608;
    if ((double)dx == zero) {
        if ((double)dz > *(const float*)0x5c8598) return x > ax ? 1 : 0;
        return x < ax ? 1 : 0;
    }
    float slope = (float)((double)dz / dx);
    float yl = (float)(((double)x - ax) * slope + az);
    if ((double)dx > zero) return y < yl ? 1 : 0;
    if ((double)dx < zero) return y > yl ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x48a880, Effect_PointOnSide);

// 0x491e60. Adds object t (id +0x202) to the grid's column list for its cell
// (+0x8c: per x a y-sorted chain of {y, ids, next} 10-byte cells, ids a chain of
// {id, next}).
void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t) {
    float fx, fy;
    Obj_CenterCell(t, 0, &fx, &fy);
    int32_t ix = (int32_t)fx;
    uint16_t iy = (uint16_t)(int32_t)fy;
    uint8_t** column = &F<uint8_t**>(g, 0x8c)[(uint16_t)ix];
    uint8_t* cur = *column;
    uint32_t* node = (uint32_t*)ExeNew(8);
    node[0] = F<uint32_t>(t, 0x202);
    node[1] = 0;
    if (!*column) {
        uint8_t* c = (uint8_t*)ExeNew(10);
        *column = c;
        F<uint16_t>(*column, 0) = iy;
        F<uint32_t>(*column, 6) = 0;
        F<uint32_t*>(*column, 2) = node;
        return;
    }
    uint8_t* prev = nullptr;
    for (; cur; prev = cur, cur = F<uint8_t*>(cur, 6)) {
        uint16_t y = F<uint16_t>(cur, 0);
        if (y == iy) {
            uint32_t* l = F<uint32_t*>(cur, 2);
            if (l) {
                while (l[1]) l = (uint32_t*)l[1];
                l[1] = (uint32_t)node;
            } else {
                F<uint32_t*>(cur, 2) = node;
            }
            return;
        }
        if (y > iy) {
            uint8_t* c = (uint8_t*)ExeNew(10);
            F<uint32_t*>(c, 2) = node;
            F<uint16_t>(c, 0) = iy;
            F<uint8_t*>(c, 6) = cur;
            if (prev) F<uint8_t*>(prev, 6) = c;
            else *column = c;
            return;
        }
    }
    uint8_t* c = (uint8_t*)ExeNew(10);
    F<uint32_t*>(c, 2) = node;
    F<uint16_t>(c, 0) = iy;
    F<uint32_t>(c, 6) = 0;
    if (prev) F<uint8_t*>(prev, 6) = c;
}
RT3_REPLACE(0x491e60, Grid_AddObject);

// 0x4924a0. The object of the collection nearest to (x, y) among those listed in
// the grid block of (x, y) and in the same fine cell, skipping ids skip1 / skip2
// and, with `freeOnly`, occupied ones (+0x226); -1 if none or the cell is not flagged.
int32_t __fastcall Grid_NearestObject(Collection* c, int, float x, float y, int32_t skip1, int32_t skip2, int32_t freeOnly) {
    int32_t blockX, blockY;
    __asm {
        fld y
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov blockY, eax
        fld x
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov blockX, eax
    }
    uint8_t* e = (uint8_t*)Obj_CellEntry(c, 0, (uint16_t)blockX, (uint16_t)blockY);
    if (!e) return -1;
    int32_t iy = (int32_t)y, ix = (int32_t)x;
    if (!Map_CellFlag(hook::Global<void*>(0x62c120), 0, ix, iy, 1)) return -1;
    int32_t best = -1;
    float bestD = 9999999.0f;
    for (const uint32_t* n = F<const uint32_t*>(e, 2); n; n = (const uint32_t*)n[1]) {
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, n[0]);
        if (!t) continue;
        if (freeOnly && F<int32_t>(t, 0x226)) continue;
        if ((int32_t)n[0] == skip1 || (int32_t)n[0] == skip2) continue;
        float tx, ty;
        Obj_CenterCell(t, 0, &tx, &ty);
        tx = (float)((double)tx * *(const float*)0x5c8568);
        ty = (float)((double)ty * *(const float*)0x5c8568);
        if ((int32_t)tx != ix || (int32_t)ty != iy) continue;
        float d = (float)DistanceSq2D(tx, ty, x, y);
        if (d < bestD) {
            best = (int32_t)n[0];
            bestD = d;
        }
    }
    return best;
}
RT3_REPLACE(0x4924a0, Grid_NearestObject);

// Walks the track nodes from `from` (coming from `prev`) while they belong to
// route r, moving them to newId; counts them and returns the last node's id in *last.
static void Reassign(uint8_t* prev, uint8_t* cur, int32_t r, int32_t newId, int32_t* count, int32_t* count2,
                     uint32_t* last) {
    while (cur && F<int32_t>(cur, 0x212) == r) {
        Node_Set212(cur, 0, newId);
        ++*count;
        ++*count2;
        *last = F<uint32_t>(cur, 0x202);
        uint8_t* next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        prev = cur;
        cur = next;
    }
}

// 0x4a4ff0. Splits track node id's chain off into a new route when its route
// (+0x212) no longer exists: renumbers the connected nodes of the old route both
// ways (+0x206 / +0x20a), sets the new route's node count (+0x14, refs) and ends.
int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cfca8), 0, id)) return 0;
    uint8_t* n = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id);
    if (!n[0x216]) return 0;
    int32_t r = F<int32_t>(n, 0x212);
    if (Collection_IsValid(routes, 0, r)) return 0;
    int32_t newId = (int32_t)Routes_Add(routes);
    int32_t count = 1, count2 = 1;
    uint32_t endA = id, endB = id;
    Node_Set212(n, 0, newId);
    Reassign(n, (uint8_t*)Obj_Node206(n), r, newId, &count, &count2, &endA);
    Reassign(n, (uint8_t*)Obj_Node20a(n), r, newId, &count, &count2, &endB);
    F<int32_t>(Collection_Get(routes, 0, newId), 0x14) += count2;
    Routes_AddRef(routes, 0, newId, count);
    Route_AddNode((uint8_t*)Collection_Get(routes, 0, newId), 0, endA);
    Route_AddNode((uint8_t*)Collection_Get(routes, 0, newId), 0, endB);
    return 1;
}
RT3_REPLACE(0x4a4ff0, Routes_SplitAt);

// 0x4a7a30. Looks up to 15 track nodes ahead of the train's car (0x62ba84 by +0x41;
// node +0x356, direction +0x35a) for a station (+0x222) of building kind `kind`:
// sets *out to the first one (or the next one after *out; with `check`, only those
// far enough per Obj_FarFromSiteBuilding); -1 at a junction (+0x20e) or after 15.
void __fastcall Train_FindStationAhead(uint8_t* o, int, int32_t* out, int32_t kind, int32_t check) {
    uint8_t* car = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(o, 0x41));
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    uint8_t* s = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(car, 0x356));
    int32_t prevId = F<int32_t>(s, 0x202);
    int32_t dir = F<int32_t>(car, 0x35a);
    int32_t steps = 0;
    for (;;) {
        int32_t sid = F<int32_t>(s, 0x222);
        if (sid != -1) {
            void* bld = Collection_Get(hook::Global<Collection*>(0x6cec20), 0, sid);
            if (bld) {
                uint8_t* b = (uint8_t*)Obj_Building(bld);
                uint8_t* type = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
                if (kind == type[0x8c]) {
                    if (*out == -1) {
                        if (!check || Obj_FarFromSiteBuilding(car, bld) == 1) {
                            *out = F<int32_t>(s, 0x222);
                            break;
                        }
                    } else {
                        if (Collection_Get(hook::Global<Collection*>(0x6cec20), 0, *out) == bld) break;
                        if (!check || Obj_FarFromSiteBuilding(car, bld) == 1) {
                            *out = F<int32_t>(s, 0x222);
                            break;
                        }
                    }
                }
            }
        }
        int32_t self = F<int32_t>(s, 0x202);
        int32_t a = F<int32_t>(s, 0x206), b = F<int32_t>(s, 0x20a);
        if (F<int32_t>(s, 0x20e) != -1) {
            *out = -1;
            return;
        }
        int32_t next;
        if (!steps) {
            next = dir ? b : a;
            if (next == -1) return;
        } else {
            next = b == prevId ? a : b;
            if (next == -1) break;
        }
        s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, next);
        ++steps;
        prevId = self;
        if (steps >= 0xf) break;
    }
    if (steps == 0xf) *out = -1;
}
RT3_REPLACE(0x4a7a30, Train_FindStationAhead);

} // namespace rt3
