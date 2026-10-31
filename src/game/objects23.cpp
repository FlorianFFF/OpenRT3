// Small functions across blocks 0x43..0x46 (batch 26). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Collection_ReleaseAll(Collection* c);
int32_t ClockMs();

void __fastcall Collection5c9cec_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9cec;
    Collection_ReleaseAll(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x432e80, Collection5c9cec_Destruct);

// 0x439bb0. Byte of table 0x5ee4d0 for option +0x10c.
int32_t OptionTable10c() { return ((const uint8_t*)0x5ee4d0)[F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c)]; }
RT3_REPLACE(0x439bb0, OptionTable10c);

// 0x43a070. Copies a 48-byte entry into slot i of the array +0x10e.
void __fastcall Obj_SetEntry10e(uint8_t* o, int, const uint32_t* src, int32_t i) {
    uint32_t* d = (uint32_t*)(o + 0x10e + i * 0x30);
    for (int k = 0; k < 12; ++k) d[k] = src[k];
}
RT3_REPLACE(0x43a070, Obj_SetEntry10e);

void __fastcall Obj_StampClock346(void* o) { F<int32_t>(o, 0x346) = ClockMs(); }
RT3_REPLACE(0x43a180, Obj_StampClock346);
void __fastcall Obj_ClearClock346(void* o) { F<int32_t>(o, 0x346) = 0; }
RT3_REPLACE(0x43a190, Obj_ClearClock346);

void __stdcall SetGlobals5ee4fc(int32_t a, int32_t b) {
    hook::Global<int32_t>(0x5ee4fc) = a;
    hook::Global<int32_t>(0x5ee500) = b;
}
RT3_REPLACE(0x43f620, SetGlobals5ee4fc);

// 0x448c00..0x448f30. Grid accessors: float grid +0x1605 (stride
// +0x15d9 + 1), 3-byte cells +0x165d and byte grids +0x1661 / +0x1665
// (stride +0x15dd).
double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y) {
    return F<float*>(g, 0x1605)[(F<int32_t>(g, 0x15d9) + 1) * y + x];
}
RT3_REPLACE(0x448c00, Grid_Float1605);
static inline uint8_t* Cell165d(uint8_t* g, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x165d) + (F<int32_t>(g, 0x15dd) * y + x) * 3 + 2;
}
void __fastcall Grid_MarkCell(uint8_t* g, int, int32_t x, int32_t y) {
    *Cell165d(g, x, y) |= 0x10;
    void* o = hook::Global<void*>(0x62be68);
    ((void(__thiscall*)(void*))(*(void***)o)[0])(o);
}
RT3_REPLACE(0x448d90, Grid_MarkCell);
uint8_t __fastcall Grid_CellLow(uint8_t* g, int, int32_t x, int32_t y) { return *Cell165d(g, x, y) & 0xf; }
RT3_REPLACE(0x448df0, Grid_CellLow);
void __fastcall Grid_SetCellLow(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    uint8_t* c = Cell165d(g, x, y);
    *c ^= (uint8_t)((*c ^ v) & 0xf);
}
RT3_REPLACE(0x448e20, Grid_SetCellLow);
void __fastcall Grid_SetByte1661(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    F<uint8_t*>(g, 0x1661)[F<int32_t>(g, 0x15dd) * y + x] = v;
}
RT3_REPLACE(0x448e60, Grid_SetByte1661);
void __fastcall Grid_SetByte1665(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    F<uint8_t*>(g, 0x1665)[F<int32_t>(g, 0x15dd) * y + x] = v;
}
RT3_REPLACE(0x448e90, Grid_SetByte1665);
uint8_t __fastcall Grid_Byte1661(uint8_t* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x1661)[F<int32_t>(g, 0x15dd) * y + x];
}
RT3_REPLACE(0x448ec0, Grid_Byte1661);
uint8_t __fastcall Grid_Byte1665(uint8_t* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x1665)[F<int32_t>(g, 0x15dd) * y + x];
}
RT3_REPLACE(0x448ee0, Grid_Byte1665);
int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y) { return (*Cell165d(g, x, y) & 0x10) ? 1 : 0; }
RT3_REPLACE(0x448f30, Grid_CellMarked);

int32_t __fastcall Obj_Byte21c5(void* o) { return F<uint8_t>(o, 0x21c5); }
RT3_REPLACE(0x450150, Obj_Byte21c5);

// 0x453890. Frees the four buffers 0x6acd20..0x6acd2c.
RT3_NOINLINE void FreeBuffers6acd20() {
    for (void** p = (void**)0x6acd20; p < (void**)0x6acd30; ++p) {
        if (*p) Free(*p);
        *p = nullptr;
    }
}
RT3_REPLACE(0x453890, FreeBuffers6acd20);

void* __fastcall ReturnThis455290(void* o) { return o; }
RT3_REPLACE(0x455290, ReturnThis455290);
void __fastcall Nop4552a0(void*) {}
RT3_REPLACE(0x4552a0, Nop4552a0);
void __fastcall Obj_Reset455310(void* o) {
    F<uint8_t>(o, 0xc) = 0;
    F<int32_t>(o, 8) = 0;
}
RT3_REPLACE(0x455310, Obj_Reset455310);

// 0x455320 / 0x455350. Collection subclass 0x5cae10 (inline 0x21a-byte records, 10 + 5).
Collection* __fastcall Collection5cae10_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cae10;
    Collection_Init(c, 0, 1, 0x21a, 10, 5, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x455320, Collection5cae10_Construct);
void __fastcall Collection5cae10_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5cae10;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x455350, Collection5cae10_Destruct);

int32_t __fastcall IsCurrent6acd3c(void* o) { return hook::Global<void*>(0x6acd3c) == o ? 1 : 0; }
RT3_REPLACE(0x4557f0, IsCurrent6acd3c);

// 0x455a50. Virtual slot 16, then clears the current-object globals 0x6acd38..0x6acd40.
int32_t __fastcall Obj_CloseCurrent(void* o, int, int32_t a) {
    ((void(__thiscall*)(void*, int32_t))(*(void***)o)[0x40 / 4])(o, a);
    hook::Global<int32_t>(0x6acd38) = 0;
    hook::Global<int32_t>(0x6acd3c) = 0;
    hook::Global<int32_t>(0x6acd40) = 0;
    return 0;
}
RT3_REPLACE(0x455a50, Obj_CloseCurrent);

void Clear6acd60() { hook::Global<int32_t>(0x6acd60) = 0; }
RT3_REPLACE(0x45a3f0, Clear6acd60);

double __fastcall Obj_Float242(void* o, int, int32_t i) { return F<float>(o, 0x242 + i * 4); }
RT3_REPLACE(0x45c6e0, Obj_Float242);

// 0x45ed70. Repacks flags: bit 0 = any of bits 0-1, bit 1 = bit 5, bit 2 = bit 2.
uint32_t __fastcall RepackFlags(uint32_t f) { return ((f & 3) ? 1u : 0u) | ((f >> 4) & 2) | (f & 4); }
RT3_REPLACE(0x45ed70, RepackFlags);

Collection* __fastcall Collection5ce044_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce044;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x461530, Collection5ce044_Construct);

void __fastcall Terrain_DeleteMarkers(uint8_t* r);

// 0x4539c0. Drops the map's markers once (flag +0x2169): frees the buffers
// 0x6acd20 and deletes the marker objects.
void __fastcall Map_DropMarkers(uint8_t* m) {
    if (!F<int32_t>(m, 0x2169)) return;
    F<int32_t>(m, 0x2169) = 0;
    FreeBuffers6acd20();
    Terrain_DeleteMarkers(m);
}
RT3_REPLACE(0x4539c0, Map_DropMarkers);

} // namespace rt3
