// Functions across blocks 0x44..0x4a (batch 50). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_cities = hook::Global<Collection*>(0x62ba84);

void __fastcall Map_FreeCellKeyLists(uint8_t* m);
void __fastcall Tracks_Destruct(Collection* c);
double __fastcall Train_Float159(uint8_t* t, int, int32_t i);
void* __fastcall Cursor33_Init(uint8_t* c);
void* __fastcall Obj_Ref388(void* o);
void* __fastcall Obj_City352(void* o);
double __fastcall Obj_Ref388Float10d(void* o);

// 0x4483d0. Frees the map's buffers and the track collection (once, flag +0x2165).
void __fastcall Map_Free(uint8_t* m) {
    if (!F<int32_t>(m, 0x2165)) return;
    F<int32_t>(m, 0x2165) = 0;
    Map_FreeCellKeyLists(m);
    if (Collection* t = hook::Global<Collection*>(0x6cfca8)) {
        Tracks_Destruct(t);
        ((void(__cdecl*)(void*))0x53b080)(t);
        hook::Global<Collection*>(0x6cfca8) = nullptr;
    }
    static const uint32_t kBuffers[] = { 0x2141, 0x2139, 0x213d, 0x212d, 0x2129, 0x2135, 0x2131 };
    for (uint32_t off : kBuffers) Free(F<void*>(m, off));
}
RT3_REPLACE(0x4483d0, Map_Free);

// 0x46d5b0. This year's +0x159 stat plus the total +0x105 (unrounded).
double __fastcall Train_Total159(uint8_t* t) { return Train_Float159(t, 0, 0) + F<float>(t, 0x105); }
RT3_REPLACE(0x46d5b0, Train_Total159);

void __fastcall Nop4a7320(void*) {}
RT3_REPLACE(0x4a7320, Nop4a7320);
void __fastcall Nop4a7350(void*) {}
RT3_REPLACE(0x4a7350, Nop4a7350);

int32_t __fastcall Train_State1Or2(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 1 || s == 2 ? 1 : 0;
}
RT3_REPLACE(0x4a7330, Train_State1Or2);

// 0x4a7360. State 1 with speed +0xf0 at most 0.01 (stopped).
int32_t __fastcall Train_IsStopped(uint8_t* t) {
    if (F<int32_t>(t, 0xe8) != 1) return 0;
    return (double)F<float>(t, 0xf0) <= *(const double*)0x5c85b8 ? 1 : 0;
}
RT3_REPLACE(0x4a7360, Train_IsStopped);

// 0x4a7390. Replaces the 63-byte buffer +0x1c6 with a copy of src.
void __fastcall Train_SetBuffer1c6(uint8_t* t, int, const uint8_t* src) {
    if (F<void*>(t, 0x1c6)) {
        Free(F<void*>(t, 0x1c6));
        F<void*>(t, 0x1c6) = nullptr;
    }
    uint8_t* p = (uint8_t*)Malloc(0x3f);
    F<uint8_t*>(t, 0x1c6) = p;
    for (int k = 0; k < 0x3f; ++k) p[k] = src[k];
}
RT3_REPLACE(0x4a7390, Train_SetBuffer1c6);

void __fastcall Obj_Dec58c(uint8_t* o) {
    if (F<int32_t>(o, 0x58c) > 0) F<int32_t>(o, 0x58c)--;
}
RT3_REPLACE(0x4a73e0, Obj_Dec58c);

uint8_t* __fastcall Cursor33_Construct(uint8_t* c) {
    Cursor33_Init(c);
    return c;
}
RT3_REPLACE(0x4a7400, Cursor33_Construct);

// 0x4a76a0 / 0x4a7700 / 0x4a7760. Over the consist chain from city record
// +0x41 (next via 0x41ad90): n-th car whose type (+0x388)->+0x41 is 1 /
// is not 2 or 3, and the count of the latter.
void* __fastcall Train_NthCarKind1(uint8_t* t, int, int32_t n) {
    if (n > t[0xfa]) return nullptr;
    for (void* c = Collection_Get(g_cities, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 1) --n;
        if (n <= 0) return c;
    }
    return nullptr;
}
RT3_REPLACE(0x4a76a0, Train_NthCarKind1);
void* __fastcall Train_NthCarOther(uint8_t* t, int, int32_t n) {
    for (void* c = Collection_Get(g_cities, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) != 2 && F<int32_t>(Obj_Ref388(c), 0x41) != 3) --n;
        if (n <= 0) return c;
    }
    return nullptr;
}
RT3_REPLACE(0x4a7700, Train_NthCarOther);
int32_t __fastcall Train_CountCarsOther(uint8_t* t) {
    int32_t n = 0;
    for (void* c = Collection_Get(g_cities, 0, F<uint32_t>(t, 0x41)); c; c = Obj_City352(c))
        if (F<int32_t>(Obj_Ref388(c), 0x41) != 2 && F<int32_t>(Obj_Ref388(c), 0x41) != 3) ++n;
    return n;
}
RT3_REPLACE(0x4a7760, Train_CountCarsOther);

// 0x4a7db0. Sum of the cars' type value +0x10d over the consist chain (next +0x352).
float __fastcall Train_SumCarValues(uint8_t* t) {
    float s = 0.0f;
    for (uint8_t* c = (uint8_t*)Collection_Get(g_cities, 0, F<uint32_t>(t, 0x41)); c; ) {
        s = (float)(Obj_Ref388Float10d(c) + s);
        int32_t next = F<int32_t>(c, 0x352);
        if (next == -1) break;
        c = (uint8_t*)Collection_Get(g_cities, 0, next);
    }
    return s;
}
RT3_REPLACE(0x4a7db0, Train_SumCarValues);

} // namespace rt3
