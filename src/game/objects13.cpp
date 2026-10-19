// Block 0x41xxxx, last small functions (batch 16). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_buildings   = hook::Global<Collection*>(0x62b26c);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);

void* __fastcall Table_GetByName(Collection* t, int, const char* name);

// 0x41f7e0. Pushes building `b` on the front of this's building chain
// (u16 head +0x383, next +0x270, own ID +0x2a4).
void __fastcall Obj_PushBuilding(void* o, int, void* b) {
    F<uint16_t>(b, 0x270) = F<uint16_t>(o, 0x383);
    F<uint16_t>(o, 0x383) = F<uint16_t>(b, 0x2a4);
}
RT3_REPLACE(0x41f7e0, Obj_PushBuilding);

// 0x41f810. First building of the chain, or 0 if the head is not valid.
void* __fastcall Obj_FirstBuilding(void* o) {
    if (!Collection_IsValid(g_buildings, 0, F<uint16_t>(o, 0x383))) return nullptr;
    return Collection_Get(g_buildings, 0, F<uint16_t>(o, 0x383));
}
RT3_REPLACE(0x41f810, Obj_FirstBuilding);

// 0x41f8d0. Clears the links (+0x26e, +0x270) of every building in the chain.
// The head +0x383 itself is left as is.
void __fastcall Obj_UnlinkBuildings(void* o) {
    for (uint32_t id = F<uint16_t>(o, 0x383); id; ) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        id = F<uint16_t>(b, 0x270);
        F<uint16_t>(b, 0x26e) = 0;
        F<uint16_t>(b, 0x270) = 0;
    }
}
RT3_REPLACE(0x41f8d0, Obj_UnlinkBuildings);

// 0x41f940. Records of the +0x37f collection whose 0x62b268 namesake has +0x7ac set.
int32_t __fastcall Obj_CountFlagged7ac(void* o) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(o, 0x37f)); ++i) {
        Collection* c = F<Collection*>(o, 0x37f);
        const char* name = (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        uint8_t* t = (uint8_t*)Table_GetByName(g_table62b268, 0, name);
        if (t && F<int32_t>(t, 0x7ac)) ++n;
    }
    return n;
}
RT3_REPLACE(0x41f940, Obj_CountFlagged7ac);

// 0x41fff0. Option +0x2bb: 0 = off, 2 = on, otherwise on once the value
// at (*0x62be68)+0x20 reaches the threshold 0x5c9a9c.
int32_t Option2bbActive() {
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x2bb);
    if (mode == 0) return 0;
    if (mode == 2) return 1;
    return F<float>(hook::Global<uint8_t*>(0x62be68), 0x20) >= *(const float*)0x5c9a9c ? 1 : 0;
}
RT3_REPLACE(0x41fff0, Option2bbActive);

} // namespace rt3
