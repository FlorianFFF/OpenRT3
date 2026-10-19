// Block 0x42xxxx, functions (batch 24). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_BuildingStats(void* o, int, int32_t* n0, int32_t* n1, int32_t* n2, int32_t* n3,
                                     int32_t* c0, int32_t* c1, int32_t* c2, int32_t* c3, int32_t all);
void __fastcall Obj_UpdateAgeFactor(void* o);

// 0x420c30. Year roll-over: adds this year's six amounts (+0x2b2..) to the
// totals (+0x29a..), keeps them as last year's (+0x2ca..), clears them,
// recounts the buildings and updates the age factor.
void __fastcall Obj_YearRollover(uint8_t* o) {
    uint32_t cur[6];
    for (int k = 0; k < 6; ++k) cur[k] = F<uint32_t>(o, 0x2b2 + k * 4);
    for (int k = 0; k < 6; ++k)
        F<float>(o, 0x29a + k * 4) = (float)((double)F<float>(o, 0x29a + k * 4) + F<float>(o, 0x2b2 + k * 4));
    for (int k = 0; k < 6; ++k) F<uint32_t>(o, 0x2ca + k * 4) = cur[k];
    for (int k = 0; k < 6; ++k) F<uint32_t>(o, 0x2b2 + k * 4) = 0;
    F<int32_t>(o, 0x296) = Obj_BuildingStats(o, 0, &F<int32_t>(o, 0x28a), nullptr, &F<int32_t>(o, 0x28e),
                                             &F<int32_t>(o, 0x292), nullptr, nullptr, nullptr, nullptr, 1);
    Obj_UpdateAgeFactor(o);
}
RT3_REPLACE(0x420c30, Obj_YearRollover);

// 0x423d30. Building counts of kinds 0, 2 and 3 into +0x27a/+0x27e/+0x282; total into +0x286.
void __fastcall Obj_CountBuildings(uint8_t* o) {
    F<int32_t>(o, 0x286) = Obj_BuildingStats(o, 0, &F<int32_t>(o, 0x27a), nullptr, &F<int32_t>(o, 0x27e),
                                             &F<int32_t>(o, 0x282), nullptr, nullptr, nullptr, nullptr, 1);
}
RT3_REPLACE(0x423d30, Obj_CountBuildings);

// 0x42bc70. Flag 8 of +0xe6: own bit 0 clear, or any in-map neighbour cell
// (8 directions) with bit 0 of its +0xe6 clear.
void __fastcall Station_UpdateEdgeFlag(uint8_t* o) {
    uint8_t flags = o[0xe6];
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    int32_t x = F<int16_t>(o, 0xe7), y = F<int16_t>(o, 0xe9);
    int32_t found = ~flags & 1;
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (int d = 0; d < 8; ++d) {
        int32_t nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= F<int32_t>(map, 0x2145) || ny < 0 || ny >= F<int32_t>(map, 0x2149)) continue;
        uint8_t* cell = F<uint8_t*>(map, 0x2129) + (F<int32_t>(map, 0x2145) * ny + nx) * 0x1d7;
        if (!(cell[0xe6] & 1)) found = 1;
    }
    o[0xe6] = found ? (uint8_t)(flags | 8) : (uint8_t)(flags & 0xf7);
}
RT3_REPLACE(0x42bc70, Station_UpdateEdgeFlag);

} // namespace rt3
