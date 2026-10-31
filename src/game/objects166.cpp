// Station cargo pickup dispatch / territory border walls / object renaming / route grade factor in blocks 0x47..0x49 (batch 170). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);
uint8_t* __fastcall Obj_Region(void* o);
void __fastcall Stock_Consume(uint16_t* o, int, const uint16_t* other, uint32_t id, float f);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
void __fastcall Terrain_FreeCell167d(uint8_t* m);
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);
struct PtrListNode;
PtrListNode* __fastcall Terrain_AddQuad(uint8_t* t, int, float x0, float y0, float z0, float x1, float y1, float z1,
                                        float x2, float y2, float z2, float x3, float y3, float z3, uint8_t raise);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Obj_SetName46b(uint8_t* o, int, const char* s);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
static inline uint8_t* RegionOf(uint8_t* o) {
    return Obj_Region(F<uint32_t>(o, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr);
}

// 0x47e9a0. Picks up `cargo` at the station for a train with room `cap`: city cargos
// via the waiting lots; others straight from the stock of the station's catchment
// cells (+0x34 list of {x, y, share}), reporting the amount (outCount, all of it with
// option +0x130) and its value at the region's quota (outValue), stamping the pickup
// time (+0x1e6) and, with `from`, its source (+0x2ba / +0x38e).
void __fastcall Station_PickUp(uint8_t* o, int, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount,
                               float* outValue, float* outAmount) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4) != -1) {
        Train_GatherLoads(o, 0, 0, city, to, cargo, cap, outCount, outAmount, nullptr);
        return;
    }
    uint8_t* region = RegionOf(o);
    *outCount = 0.0f;
    for (int32_t i = 0; i < F<int32_t>(o, 0x38); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0x34) + i * 0xc;
        uint8_t* m = Map();
        uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(e, 4) * F<int32_t>(m, 0x2145) + F<int32_t>(e, 0)) * 0x1d7;
        float* stock = &F<float>(cell, 0x103 + cargo * 4);
        if (!(*stock > Fl(0x5c8598))) continue;
        float room = (float)((double)cap - *outCount);
        float take = *stock < room ? *stock : room;
        Stock_Consume((uint16_t*)cell, 0, (const uint16_t*)region, cargo, F<float>(e, 8));
        *outCount = (float)((double)take + *outCount);
        *stock = (float)((double)*stock - take);
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130)) *outCount = cap;
    F<int32_t>(o, 0x1e6 + cargo * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    if (!(*outCount > Fl(0x5c8598))) {
        *outValue = 0.0f;
        return;
    }
    int32_t q = City_CargoQuota(RegionOf(o), 0, cargo);
    *outValue = (float)((double)(float)q * *outCount);
    if (to) {
        F<int32_t>(o, 0x2ba + cargo * 4) = F<int32_t>(to, 0);
        F<int32_t>(o, 0x38e + cargo * 4) = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x15);
    }
}
RT3_REPLACE(0x47e9a0, Station_PickUp);

// 0x487de0. With more than one territory in use, walls off the borders between
// tiles of different territory groups (tile flags >> 3; grouped by +0x31 / +0x35)
// with thin quads along the west and north tile edges.
void __fastcall Territories_BuildBorders(Collection* c) {
    Terrain_FreeCell167d(Map());
    int32_t used = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x31) && F<int32_t>(e, 0x41) + F<int32_t>(e, 0x3d) > 0) ++used;
    }
    if (used <= 1) return;
    int32_t w = F<int32_t>(Map(), 0x2155), h = F<int32_t>(Map(), 0x2159);
    int32_t group[100];
    memset(group, 0, sizeof group);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        group[F<int32_t>(e, 0)] = F<int32_t>(e, 0x31) ? F<int32_t>(e, 0) : F<int32_t>(e, 0x35);
    }
    const double d = D(0x5cf118);
    for (int32_t y = 0; y < h; ++y)
        for (int32_t x = 0; x < w; ++x) {
            if (Terrain_IsSpecial(Map(), 0, x, y)) continue;
            for (int32_t k = 0; k < 2; ++k) {
                int32_t nx = x + (k ? 0 : -1), ny = y + (k ? -1 : 0);
                if (nx < 0 || ny < 0) continue;
                if (Terrain_IsSpecial(Map(), 0, nx, ny)) continue;
                uint8_t* m = Map();
                uint8_t* tiles = F<uint8_t*>(m, 0x2131);
                int32_t t1 = tiles[F<int32_t>(m, 0x2155) * y + x] >> 3;
                int32_t t2 = tiles[F<int32_t>(m, 0x2155) * ny + nx] >> 3;
                if (group[t1] == group[t2]) continue;
                int32_t px = x * 0x5c, pz = y * 0x5c;
                if (!k) {
                    float xa = (float)((double)px + d), xb = (float)((double)px - d);
                    float za = (float)pz, zb = (float)(pz + 0x5c);
                    Terrain_AddQuad(Map(), 0, xb, 0.0f, zb, xb, 0.0f, za, xa, 0.0f, za, xa, 0.0f, zb, 2);
                } else {
                    float za = (float)((double)pz + d), zb = (float)((double)pz - d);
                    float xa = (float)px, xb = (float)(px + 0x5c);
                    Terrain_AddQuad(Map(), 0, xb, 0.0f, zb, xa, 0.0f, zb, xa, 0.0f, za, xb, 0.0f, za, 2);
                }
            }
        }
}
RT3_REPLACE(0x487de0, Territories_BuildBorders);

// 0x4895c0. Renames object `id` of the given kind: 2..4 scenario texts in the world,
// 5 ..., 6 cities, 7 event texts (field 1..6), 8 players, 9 stations, 10 ...
void __fastcall Obj_Rename(int32_t kind, uint32_t id, int32_t field, const char* text) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    auto in = [&](uint32_t g) -> uint8_t* {
        Collection* c = hook::Global<Collection*>(g);
        return Collection_IsValid(c, 0, id) ? (uint8_t*)Collection_Get(hook::Global<Collection*>(g), 0, id) : nullptr;
    };
    uint8_t* o;
    switch (kind) {
    case 2: StrCopyN((char*)w + 0x672e, text, 0x258); break;
    case 3: StrCopyN((char*)w + 0x4f30, text, 0xbb8); break;
    case 4: StrCopyN((char*)w + 0x5ae9, text, 0xbb8); break;
    case 5: if ((o = in(0x6cfc9c)) != nullptr) StrCopyN((char*)o + 4, text, 0x28); break;
    case 6: if ((o = in(0x62bae0)) != nullptr) StrCopyN((char*)o + 0x356, text, 0x28); break;
    case 7:
        if ((o = in(0x62be18)) == nullptr) break;
        switch (field) {
        case 1: StrCopyN((char*)o + 0xe0, text, 0x320); break;
        case 2: StrCopyN((char*)o + 0x401, text, 0xc8); break;
        case 3: StrCopyN((char*)o + 0x4ca, text, 0xc8); break;
        case 4: StrCopyN((char*)o + 0x593, text, 0xc8); break;
        case 5: StrCopyN((char*)o + 0x65c, text, 0xc8); break;
        case 6: StrCopyN((char*)o + 0x725, text, 0xc8); break;
        }
        break;
    case 8: if ((o = in(0x62be10)) != nullptr) StrCopyN((char*)o + 4, text, 0x32); break;
    case 9: if ((o = in(0x6cec20)) != nullptr) Obj_SetName46b(o, 0, text); break;
    case 10: if ((o = in(0x6ada80)) != nullptr) StrCopyN((char*)o + 0x2a2, text, 0x28); break;
    }
}
RT3_REPLACE(0x4895c0, Obj_Rename);

// 0x4918e0. Speed/cost factor for a route step from tile (x1, z1) to (x2, z2) (world
// units, height h1): the slope to the terrain height at the end over the distance,
// from 0.7 (falling) through 1.0 to 4.2 (steep climb).
float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1) {
    float dist = (float)Distance2D(x1, z1, (float)((double)x2 * Fl(0x5c8700)), (float)((double)z2 * Fl(0x5c8700)));
    float h = (float)Terrain_HeightAtTile(Map(), 0, x2, z2);
    float s = (float)Distance2D(0.0f, h1, dist, h);
    static const uint32_t lo[] = { 0x5ca06c, 0x5ca068, 0x5c86a0 };
    static const uint32_t loV[] = { 0x3f333333, 0x3f4ccccd, 0x3f666666 };
    for (int32_t i = 0; i < 3; ++i)
        if (s <= Fl(lo[i])) return *(const float*)&loV[i];
    if (s <= Fl(0x5cfe10)) return 1.0f;
    static const uint32_t hi[] = { 0x5ca198, 0x5cfe0c, 0x5cfe08, 0x5cfe04, 0x5ca1c8, 0x5c85b0, 0x5cfe00, 0x5ca1c4 };
    static const uint32_t hiV[] = { 0x3f8ccccd, 0x3fa00000, 0x3fb9999a, 0x3fd9999a, 0x3ff9999a, 0x40133333, 0x40266666, 0x40400000 };
    for (int32_t i = 0; i < 8; ++i)
        if (s <= Fl(hi[i])) return *(const float*)&hiV[i];
    return s <= Fl(0x5ca1c0) ? 3.5f : 4.2f;
}
RT3_REPLACE(0x4918e0, Route_GradeFactor);

} // namespace rt3
