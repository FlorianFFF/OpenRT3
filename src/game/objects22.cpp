// Small functions across blocks 0x40..0x43 (batch 25). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);
static Collection*& g_companies   = hook::Global<Collection*>(0x62be10);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

void __fastcall List1a_Free(uint8_t** head);
void __fastcall Obj_FreeFourLists(void* o);
void* __fastcall Engine_Renderer(void* e);

static inline uint8_t* TypeOf173(void* o) { return (uint8_t*)Collection_Get(g_table62b268, 0, F<uint32_t>(o, 0x173)); }

// 0x401070. Rating by +0x296: 9.4 / 8.4 / 7.4 / 6.4 from 85 / 70 / 50.
float __fastcall Obj_Rating296(void* o) {
    int32_t v = F<int32_t>(o, 0x296);
    if (v >= 0x55) return *(const float*)0x5c8564;
    if (v >= 0x46) return *(const float*)0x5c8560;
    if (v >= 0x32) return *(const float*)0x5c855c;
    return *(const float*)0x5c8558;
}
RT3_REPLACE(0x401070, Obj_Rating296);

// 0x4010d0 / 0x406bf0 / 0x432e60. Record at position `pos` (of list 0 / `list`).
void* __fastcall Collection_RecordAt(Collection* t, int, uint32_t pos) { return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, 0)); }
RT3_REPLACE(0x4010d0, Collection_RecordAt);
void* __fastcall Collection_RecordAt2(Collection* t, int, uint32_t pos) { return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, 0)); }
RT3_REPLACE(0x406bf0, Collection_RecordAt2);
void* __fastcall Collection_RecordAtList(Collection* t, int, uint32_t pos, int list) {
    return Collection_Get(t, 0, Collection_IdAt(t, 0, pos, list));
}
RT3_REPLACE(0x432e60, Collection_RecordAtList);

void __fastcall Nop40ca60(void*) {}
RT3_REPLACE(0x40ca60, Nop40ca60);
void __fastcall Nop433730(void*) {}
RT3_REPLACE(0x433730, Nop433730);
void __fastcall Nop433e90(void*) {}
RT3_REPLACE(0x433e90, Nop433e90);
void __fastcall Nop436040(void*, int, int32_t) {}
RT3_REPLACE(0x436040, Nop436040);

// 0x40cb10. Stores v in world +0x66ba, then forwards it to virtual slot 21.
uint32_t __fastcall Obj_SetWorld66ba(void* o, int, int32_t v) {
    F<int32_t>(g_world, 0x66ba) = v;
    return ((uint32_t(__thiscall*)(void*, int32_t))(*(void***)o)[0x54 / 4])(o, F<int32_t>(g_world, 0x66ba));
}
RT3_REPLACE(0x40cb10, Obj_SetWorld66ba);

// 0x40cba0. Address of frame +0x21 (0x45 bytes each) in animation +0x3a of the table +0x14; 0 if none.
uint32_t __fastcall Obj_AnimFrame(void* o) {
    uint8_t* t = F<uint8_t*>(o, 0x14);
    if (!t) return 0;
    return F<uint32_t>(t, F<int32_t>(o, 0x3a) * 24 + 0xc) + F<int32_t>(o, 0x21) * 0x45;
}
RT3_REPLACE(0x40cba0, Obj_AnimFrame);

// 0x40cc40 / 0x40cc60 / 0x40cc80. Queries of the 0x62b268 type (+0x173).
int32_t __fastcall Obj_TypeKindIs1(void* o) { return F<uint8_t>(TypeOf173(o), 0x32) == 1 ? 1 : 0; }
RT3_REPLACE(0x40cc40, Obj_TypeKindIs1);
int32_t __fastcall Obj_TypeByteBa(void* o) { return F<uint8_t>(TypeOf173(o), 0xba); }
RT3_REPLACE(0x40cc60, Obj_TypeByteBa);
int32_t __fastcall Obj_TypeKind1Class3(void* o) {
    uint8_t* t = TypeOf173(o);
    return F<uint8_t>(t, 0x32) == 1 && F<uint8_t>(t, 0x8c) == 3 ? 1 : 0;
}
RT3_REPLACE(0x40cc80, Obj_TypeKind1Class3);

// 0x40d210. Owning company (+0x276), or 0.
void* __fastcall Obj_OwnerCompany(void* o) {
    uint32_t id = F<uint32_t>(o, 0x276);
    return id ? Collection_Get(g_companies, 0, id) : nullptr;
}
RT3_REPLACE(0x40d210, Obj_OwnerCompany);

// 0x40fb70. Forwards to the type's 0x412560 with flag (+0x246 != 0).
uint32_t __fastcall Obj_TypeQuery(void* o, int, int32_t a) {
    int32_t flag = F<int32_t>(o, 0x246) != 0;
    void* t = ((void*(__thiscall*)(void*))(*(void***)o)[0x80 / 4])(o);
    return ((uint32_t(__thiscall*)(void*, int32_t, int32_t))0x412560)(t, a, flag);
}
RT3_REPLACE(0x40fb70, Obj_TypeQuery);

void __fastcall Obj_FreeAllLists2(void* o) {
    List1a_Free((uint8_t**)o);
    Obj_FreeFourLists(o);
}
RT3_REPLACE(0x4315c0, Obj_FreeAllLists2);

// 0x432de0 / 0x432e20. Collection subclass 0x5c9cec (heap records, 10 + 20,
// list 2) and its clear (releases every record through virtual slot 2).
Collection* __fastcall Collection5c9cec_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9cec;
    F<int32_t>(c, 0x88) = 0;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 1, 0);
    return c;
}
RT3_REPLACE(0x432de0, Collection5c9cec_Construct);
void __fastcall Collection_ReleaseAll(Collection* c) {
    while (Collection_Count(c)) {
        void** vt = *(void***)c;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(c, Collection_IdAt(c, 0, 0, 0));
    }
    Collection_Clear(c);
}
RT3_REPLACE(0x432e20, Collection_ReleaseAll);

// 0x434060. Not in a lobby, or +0x66ae == 1.
int32_t __fastcall World_IsHostOrLocal(void* w) {
    if (!hook::Global<void*>(0x6cd8d8)) return 1;
    return F<int32_t>(w, 0x66ae) == 1 ? 1 : 0;
}
RT3_REPLACE(0x434060, World_IsHostOrLocal);

uint32_t World46aa0() { return g_world ? F<uint32_t>(g_world, 0x46aa0) : 0; }
RT3_REPLACE(0x4340d0, World46aa0);

void __fastcall Obj_SetName8(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 8;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x4340f0, Obj_SetName8);

int32_t __fastcall Obj_Has78(void* o) { return !F<uint8_t>(o, 0x82) && F<int32_t>(o, 0x78) != -1 ? 1 : 0; }
RT3_REPLACE(0x434110, Obj_Has78);

// 0x435460 / 0x435480. Renderer depth/mode (+0x15) above 128 / zero.
int32_t RendererMode15Above128() { return F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15) > 0x80 ? 1 : 0; }
RT3_REPLACE(0x435460, RendererMode15Above128);
int32_t RendererMode15Zero() { return F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15) == 0 ? 1 : 0; }
RT3_REPLACE(0x435480, RendererMode15Zero);

int32_t Global6d0818Is25() { return F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c) == 0x19 ? 1 : 0; }
RT3_REPLACE(0x435610, Global6d0818Is25);

void __fastcall Obj_Set65(void* o) { F<uint8_t>(o, 0x65) = 1; }
RT3_REPLACE(0x439a80, Obj_Set65);

// 0x439a90. Position (+4, +0xc) in cells (92 units).
RT3_NOINLINE void __fastcall Obj_CellPos(void* o, int, float* x, float* y) {
    const float cell = *(const float*)0x5c8700;
    *x = (float)((double)F<float>(o, 4) / cell);
    *y = (float)((double)F<float>(o, 0xc) / cell);
}
RT3_REPLACE(0x439a90, Obj_CellPos);

void __fastcall Obj_Nudge92(void* o) {
    const double eps = *(const double*)0x5c88a0;
    F<float>(o, 0x96) = (float)((double)F<float>(o, 0x96) + eps);
    F<float>(o, 0x92) = (float)((double)F<float>(o, 0x92) + eps);
}
RT3_REPLACE(0x439b70, Obj_Nudge92);

} // namespace rt3
