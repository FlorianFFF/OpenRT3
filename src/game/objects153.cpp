// City cargo output / cargo spreading between neighbour cities / city demand flags in block 0x42 (batch 157). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Building_CargoOutput(uint8_t* b, int, int32_t cargo);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
void __fastcall City_TransferCargo(uint8_t* c, int, uint8_t* dst, int32_t i, int32_t amount);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_YearReached(void* o);
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p);
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;

// 0x42bf20. Total output of cargo by the city's buildings (chain from +0xd6, next +0x2a2).
float __fastcall City_CargoOutput(uint8_t* c, int, int32_t cargo) {
    float sum = 0.0f;
    for (uint32_t id = F<uint16_t>(c, 0xd6); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
        sum = (float)(Building_CargoOutput(b, 0, cargo) + sum);
        id = F<uint16_t>(b, 0x2a2);
    }
    return sum;
}
RT3_REPLACE(0x42bf20, City_CargoOutput);

// 0x42c1b0. Spreads the city's cargo to its neighbours (8 around; 16 with the 2-step
// ring in scenario mode, world +0x46c38) whose distance cost (+0xf3 words) is under a
// random cap: per produced cargo, the neighbour with the largest price gap above the
// distance cost (cost x (price x weight x 0.00016 + 2.2) x 256 >> 12) gets cargo.
// Neighbours across water (+0xe6 bit 0) only by the near ring and with a port (bit 2).
// Off with option +0x148.
void __fastcall City_SpreadCargo(uint8_t* c) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x148)) return;
    int32_t ncargo = Collection_Count(Cargos());
    int32_t n = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c38) ? 16 : 8;
    int32_t ring = 1;
    int32_t cap = SimRand(1000, 30000);
    if ((double)cap > D(0x5c9c60)) cap = 999999;
    int32_t ok[16], cost[16];
    uint8_t* target[16];
    uint8_t* cell[16];
    int32_t cx = F<int16_t>(c, 0xe7), cy = F<int16_t>(c, 0xe9);
    for (int32_t k = 0; k < n; ++k) {
        int32_t d = k;
        if (k >= 8) {
            ring = 4;
            d = k & 7;
        }
        int32_t dx = kDirX[d], dy = kDirY[d];
        int32_t x = dx * ring + cx, y = dy * ring + cy;
        ok[k] = 0;
        if (x < 0 || x >= F<int32_t>(Map(), 0x2145) || y < 0 || y >= F<int32_t>(Map(), 0x2149)) continue;
        int32_t dist = F<uint16_t>(c, 0xf3 + k * 2);
        if (dist > cap) continue;
        uint8_t* nb = F<uint8_t*>(Map(), 0x2129) + (F<int32_t>(Map(), 0x2145) * y + x) * 0x1d7;
        cell[k] = nb;
        uint8_t a = c[0xe6], b = nb[0xe6];
        if ((a ^ b) & 1) {
            if (!(a & 4) && !(b & 4)) continue;
            if (ring != 1) continue;
        }
        ok[k] = 1;
        target[k] = nb;
        if (k < 8) {
            cost[k] = dist;
            continue;
        }
        int32_t dd = kDirY[d], dr = kDirX[d];
        uint8_t* r1 = Grid_RegionAt(Map(), 0, cx + dr * 2 + dr, cy + dd * 2 + dd);
        int32_t s = F<uint16_t>(r1, 0xf3 + d * 2);
        uint8_t* r2 = Grid_RegionAt(Map(), 0, cx + dr * 2, cy + dd * 2);
        s += F<uint16_t>(r2, 0xf3 + d * 2);
        uint8_t* r3 = Grid_RegionAt(Map(), 0, cx + dr, cy + dd);
        s += F<uint16_t>(r3, 0xf3 + d * 2);
        cost[k] = s + F<uint16_t>(c, 0xf3 + d * 2);
    }
    int32_t best = 1;
    for (int32_t i = 1; i <= ncargo; ++i) {
        Collection* cg = Cargos();
        if (!F<int32_t>(cg, 0x16e + i * 4) || F<int32_t>(cg, 0x9a + i * 4) != -1) continue;
        int32_t price = F<uint16_t>(c, i * 2);
        int32_t gapMax = 0;
        uint8_t* t = (uint8_t*)Collection_Get(cg, 0, i);
        float weight = F<float>(t, 0x52);
        int32_t f;
        double v = ((double)weight * (double)price * D(0x5c99e8) + D(0x5c8db8)) * D(0x5c9c58);
        __asm {
            fld v
            mov eax, 0x5a10d0
            call eax
            mov f, eax
        }
        for (int32_t k = 0; k < n; ++k) {
            if (!ok[k]) continue;
            int32_t gap = F<uint16_t>(target[k], i * 2) - ((cost[k] * f) >> 12) - price;
            if (gap > gapMax) {
                gapMax = gap;
                best = k;
            }
        }
        if (gapMax > 0) City_TransferCargo(c, 0, cell[best], i, (cost[best] * f) >> 12);
    }
}
RT3_REPLACE(0x42c1b0, City_SpreadCargo);

// 0x42c690. Recomputes the city's cargo flags: +0xdf bit set for each available
// cargo produced by its active buildings (+0x3d4) of industries, +0xd8 for those
// demanded (industries with +0x78c inputs).
void __fastcall City_UpdateCargoFlags(uint8_t* c) {
    uint8_t* demand = c + 0xd8;
    F<uint32_t>(demand, 0) = 0;
    F<uint16_t>(demand, 4) = 0;
    demand[6] = 0;
    int32_t ncargo = Collection_Count(Cargos());
    for (uint32_t id = F<uint16_t>(c, 0xd6); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, id);
        if (Obj_Flag3d4(b) == 1) {
            uint8_t* ind = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
            if (ind[0x32] == 2) {
                for (int32_t i = 1; i <= ncargo; ++i) {
                    if (F<int32_t>(Cargos(), 0x9a + i * 4) != -1) continue;
                    if (!Obj_YearReached(Collection_Get(Cargos(), 0, i))) continue;
                    if (Industry_OutputTotal(ind, 0, i, 0) > D(0x5c85b8)) c[0xdf + (i >> 3)] |= (uint8_t)(1 << (i & 7));
                    if (F<int32_t>(ind, 0x78c) && Industry_InputShare(ind, 0, i, 0) > D(0x5c85b8))
                        demand[i >> 3] |= (uint8_t)(1 << (i & 7));
                }
            }
        }
        id = F<uint16_t>(b, 0x2a2);
    }
}
RT3_REPLACE(0x42c690, City_UpdateCargoFlags);

} // namespace rt3
