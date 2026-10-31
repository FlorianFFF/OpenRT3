// Placing objects on the ground in blocks 0x40..0x45 (batch 194). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t IsMultiplayer();
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
float __fastcall Obj_Mid1ed(uint8_t* o);

static inline float SnapInt(float v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return (float)r;
}

// 0x40e360. For an object whose type's kind record is a 1/3 kind and that sits on a
// live track node (+0x2a8 -> +8), drops it to the node's height minus a margin,
// re-snapping x/z to its own position (via cell units).
void __fastcall Obj_SettleOnNode(uint8_t* o) {
    uint8_t* type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b2fc), 0, F<uint32_t>(o, 0x3cc));
    uint8_t* kind = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, F<uint32_t>(type, 0x173));
    if (kind[0x32] != 1 || kind[0x8c] != 3) return;
    Collection* places = hook::Global<Collection*>(0x6cec20);
    if (!F<uint32_t>(o, 0x2a8) || !Collection_Get(places, 0, F<uint32_t>(o, 0x2a8))) return;
    uint8_t* place = F<uint32_t>(o, 0x2a8) ? (uint8_t*)Collection_Get(places, 0, F<uint32_t>(o, 0x2a8)) : nullptr;
    uint32_t node = F<uint32_t>(place, 8);
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    if (node == 0xffffffff || !Collection_IsValid(nodes, 0, node)) return;
    float h = Obj_Mid1ed((uint8_t*)Collection_Get(nodes, 0, node));
    h = (float)((double)h - Fl(0x5c8738));
    float z = (float)(Obj_CellY(o) * Fl(0x5c8700));
    float x = (float)(Obj_CellX(o) * Fl(0x5c8700));
    Obj_MoveTo(o, 0, x, h, z);
}
RT3_REPLACE(0x40e360, Obj_SettleOnNode);

// 0x455660. Moves the object to cell (cx, cz) at `lift` above the ground (snapped to
// whole units in multiplayer).
void __fastcall Obj_PlaceAtCell(uint8_t* o, int, float cx, float cz, int32_t lift) {
    float z = (float)((double)cz * Fl(0x5c8700));
    float x = (float)((double)cx * Fl(0x5c8700));
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    float y = (float)(h + (float)lift);
    if (IsMultiplayer()) y = SnapInt(y);
    Obj_MoveTo(o, 0, x, y, z);
}
RT3_REPLACE(0x455660, Obj_PlaceAtCell);

// 0x4556f0. Moves the object to cell (cx, cz) at height y.
void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y) {
    float z = (float)((double)cz * Fl(0x5c8700));
    Obj_MoveTo(o, 0, (float)((double)cx * Fl(0x5c8700)), y, z);
}
RT3_REPLACE(0x4556f0, Obj_MoveToCell);

// 0x455a70. Drops the object to `lift` above the ground under it (snapped to whole
// units in multiplayer).
void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift) {
    float x, y, z;
    SceneObj_GetPos(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    float ny = (float)(h + (float)lift);
    if (IsMultiplayer()) ny = SnapInt(ny);
    Obj_MoveTo(o, 0, x, ny, z);
}
RT3_REPLACE(0x455a70, Obj_DropToGround);

} // namespace rt3
