// Functions in blocks 0x49..0x4a (batch 58). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
double __stdcall AngleOf(float x0, float y0, float x1, float y1);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
int32_t __fastcall Train_Rank(void* self);

// 0x492ec0. Whether the junction at node id turns left: the angle from its
// diverging neighbour to the node is below the angle to the straight
// neighbour (+0x20e). 1 for nodes without +0x20e.
int32_t __fastcall Node_TurnsLeft(Collection* nodes, int, uint32_t id) {
    uint8_t* n = (uint8_t*)Collection_Get(nodes, 0, id);
    if (F<int32_t>(n, 0x20e) == -1) return 1;
    uint8_t* a = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(n, 0x20e));
    uint8_t* b = (uint8_t*)Collection_Get(nodes, 0, (n[0x201] & 2) ? F<uint32_t>(n, 0x206) : F<uint32_t>(n, 0x20a));
    float nx, ny, ax, ay, bx, by;
    Obj_CenterCell(n, 0, &nx, &ny);
    Obj_CenterCell(a, 0, &ax, &ay);
    Obj_CenterCell(b, 0, &bx, &by);
    float toNode = (float)AngleOf(bx, by, nx, ny);
    float toA = (float)AngleOf(bx, by, ax, ay);
    float d = (float)AngleDelta(nullptr, 0, toNode, toA);
    return (double)d < *(const double*)0x5c8608 ? 1 : 0;
}
RT3_REPLACE(0x492ec0, Node_TurnsLeft);

// 0x4ab730. Moves the train to position `rank` among its company's trains
// by giving it a sort key (list 1) between its new neighbours.
void __fastcall Train_SetRank(uint8_t* t, int, int32_t rank) {
    if (rank == Train_Rank(t)) return;
    Collection* trains = hook::Global<Collection*>(0x6cfcbc);
    uint32_t prevKey = 1;
    for (int32_t i = 0; i < Collection_Count(trains); ++i) {
        uint8_t* tr = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, i, 1));
        if (tr == t || F<int32_t>(tr, 0x51) != F<int32_t>(t, 0x51)) continue;
        int32_t s = F<int32_t>(tr, 0xe8);
        if ((s == 0x11 || s == 0xc || s == 0xd) && (tr[0x1ce] & 2)) continue;
        int32_t rk = Train_Rank(tr);
        uint32_t id = F<uint32_t>(tr, 0x25);
        if (rank < rk) {
            int32_t k = (int32_t)Collection_GetKey1(trains, 0, id);
            int32_t v = k - (int32_t)prevKey > 2 ? (k + (int32_t)prevKey) / 2 : k - 1;
            if (v < 1) v = 1;
            Collection_SetKey1(trains, 0, F<uint32_t>(t, 0x25), (uint32_t)v);
            return;
        }
        prevKey = Collection_GetKey1(trains, 0, id);
    }
    Collection_SetKey1(trains, 0, F<uint32_t>(t, 0x25), prevKey + 1000);
}
RT3_REPLACE(0x4ab730, Train_SetRank);

} // namespace rt3
