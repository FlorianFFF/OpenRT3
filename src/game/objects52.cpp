// Functions across blocks 0x41..0x4b (batch 55). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);

void __fastcall Building_DropSharedDemand(uint8_t* b, int, uint16_t k);
void __fastcall Station_Init(uint8_t* s);
void __fastcall Route_Upgrade(uint8_t* r, int, int32_t);
uint8_t* __fastcall OrderList_Current(uint8_t* o);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
double __fastcall Obj_Float338(void* o, int, int32_t i);
double __fastcall Obj_Float360(void* o, int, int32_t i);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
void* __fastcall Array33_At(void* a, int, int32_t i);
int32_t __fastcall Obj_BuildingStateActive(void* o);

void __fastcall Buildings_DropSharedDemand(Collection* t, int, uint16_t k) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Building_DropSharedDemand((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, k);
}
RT3_REPLACE(0x4138b0, Buildings_DropSharedDemand);

uint8_t* __fastcall Station_Construct(uint8_t* s) {
    Station_Init(s);
    return s;
}
RT3_REPLACE(0x487620, Station_Construct);
void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id) {
    Station_Init(s);
    F<int32_t>(s, 0) = id;
}
RT3_REPLACE(0x487650, Station_InitWithId);

// 0x4a41b0. Routes load: tag 0x396d, records (each upgraded by 0x495020), tag 0x396e.
int32_t __fastcall Routes_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x396d, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    F<int32_t>(t, 0x8c) = 1;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Route_Upgrade((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, (int32_t)s);
    return StreamSkip4(s, 0x396e, 0, 0) + n;
}
RT3_REPLACE(0x4a41b0, Routes_Load);

// 0x4a8470. Target node of the current order: flagged orders' +0x2b, else the site's +8; -1 if none.
int32_t __fastcall Train_CurrentOrderNode(uint8_t* t) {
    if (F<int32_t>(t, 8) == -1) return -1;
    uint8_t* c = OrderList_Current(t);
    if (!c) return -1;
    if ((int8_t)c[0x28] < 0) return F<int32_t>(c, 0x2b);
    uint8_t* s = (uint8_t*)Collection_Get(g_table6cec20, 0, F<uint16_t>(c, 0x29));
    return s ? F<int32_t>(s, 8) : -1;
}
RT3_REPLACE(0x4a8470, Train_CurrentOrderNode);

// 0x4b35d0..0x4b3650. Building stats: this year's slot 0 plus the totals.
double __fastcall Building_Total2e8(uint8_t* b) { return Obj_Float2e8(b, 0, 0) + F<float>(b, 0x2b0); }
RT3_REPLACE(0x4b35d0, Building_Total2e8);
double __fastcall Building_Total310(uint8_t* b) { return Obj_Float310(b, 0, 0) + F<float>(b, 0x2b4); }
RT3_REPLACE(0x4b35f0, Building_Total310);
double __fastcall Building_Total338(uint8_t* b) { return Obj_Float338(b, 0, 0) + F<float>(b, 0x2b8); }
RT3_REPLACE(0x4b3610, Building_Total338);
double __fastcall Building_Total360(uint8_t* b) { return Obj_Float360(b, 0, 0) + F<float>(b, 0x2bc); }
RT3_REPLACE(0x4b3630, Building_Total360);
double __fastcall Building_TotalRemaining(uint8_t* b) {
    double r = Obj_Remaining(b, 0, 0);
    double t = (double)F<float>(b, 0x2b4) - F<float>(b, 0x2b8);
    t = t - F<float>(b, 0x2bc);
    return r + t;
}
RT3_REPLACE(0x4b3650, Building_TotalRemaining);

// 0x4b99c0. ID of the n-th (0-based) cargo type flagged in 0x6cfd38; 0 if none.
uint32_t __fastcall CargoTypes_NthFlagged(int32_t n) {
    Collection* types = hook::Global<Collection*>(0x62ba8c);
    for (int32_t i = 0; i < Collection_Count(types); ++i) {
        if (!((const uint8_t*)0x6cfd38)[Collection_IdAt(types, 0, i, 0)]) continue;
        if (n == 0) return Collection_IdAt(types, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x4b99c0, CargoTypes_NthFlagged);

int32_t Global6cfe00Set() { return hook::Global<void*>(0x6cfe00) ? 1 : 0; }
RT3_REPLACE(0x4b9d70, Global6cfe00Set);
void SetGlobal6cfe10() { hook::Global<int32_t>(0x6cfe10) = 1; }
RT3_REPLACE(0x4b9d80, SetGlobal6cfe10);

// 0x4b9d90. Next active site among the orders of the edited list 0x6cfe08
// after the cursor 0x621df4 (wrapping); 0 if none.
void* OrderEdit_NextActiveSite() {
    uint8_t* list = hook::Global<uint8_t*>(0x6cfe08);
    int32_t cur = hook::Global<int32_t>(0x621df4);
    for (int32_t i = (cur + 1) % F<int32_t>(list, 4); i != hook::Global<int32_t>(0x621df4);
         i = (i + 1) % F<int32_t>(hook::Global<uint8_t*>(0x6cfe08), 4)) {
        uint8_t* e = (uint8_t*)Array33_At(hook::Global<void*>(0x6cfe08), 0, i);
        if ((int8_t)e[0x28] < 0) continue;
        e = (uint8_t*)Array33_At(hook::Global<void*>(0x6cfe08), 0, i);
        uint8_t* s = (uint8_t*)Collection_Get(g_table6cec20, 0, F<uint16_t>(e, 0x29));
        if (s && Obj_BuildingStateActive(s)) return s;
    }
    return nullptr;
}
RT3_REPLACE(0x4b9d90, OrderEdit_NextActiveSite);

} // namespace rt3
