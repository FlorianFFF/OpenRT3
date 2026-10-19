// Terrain layer save/load and cell list accessors in blocks 0x44..0x53 (batch 86). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);

static inline uint32_t VertexCount(uint8_t* r) {
    return (uint32_t)((F<int32_t>(r, 0x15d9) + 1) * (F<int32_t>(r, 0x15d5) + 1));
}

// 0x448f60. Loads the four terrain layers (chunks 0x9471..0x9472).
void __fastcall Terrain_LoadLayers(uint8_t* r, int, Stream s) {
    StreamSkip4(s, 0x9471, 0, 0);
    uint32_t n = VertexCount(r);
    void* layer[4];
    for (int k = 0; k < 4; ++k) layer[k] = (void*)Terrain_Layer(r, 0, (uint8_t)k);
    for (int k = 0; k < 4; ++k) StreamRead(s, layer[k], n, 0, 1);
    StreamSkip4(s, 0x9472, 0, 0);
}
RT3_REPLACE(0x448f60, Terrain_LoadLayers);

// 0x448fe0. Saves the four terrain layers.
void __fastcall Terrain_SaveLayers(uint8_t* r, int, Stream s) {
    StreamWriteU32(s, 0x9471);
    uint32_t n = VertexCount(r);
    void* layer[4];
    for (int k = 0; k < 4; ++k) layer[k] = (void*)Terrain_Layer(r, 0, (uint8_t)k);
    for (int k = 0; k < 4; ++k) StreamWrite(s, layer[k], n, 0);
    StreamWriteU32(s, 0x9472);
}
RT3_REPLACE(0x448fe0, Terrain_SaveLayers);

uint32_t __fastcall Terrain_Get15e5(uint8_t* r) { return F<uint32_t>(r, 0x15e5); }
RT3_REPLACE(0x533b70, Terrain_Get15e5);
uint32_t __fastcall Terrain_Get1619(uint8_t* r, int, int8_t i) { return F<uint32_t>(r, 0x1619 + i * 4); }
RT3_REPLACE(0x533b80, Terrain_Get1619);
void* __fastcall Terrain_Slot15f1(uint8_t* r) { return r + 0x15f1; }
RT3_REPLACE(0x533b90, Terrain_Slot15f1);

// Per-16x16-block object lists: grids +0x1671, +0x1679, +0x167d (rows / 16 per line)
// and a global list +0x166d.
static inline PtrList* Cell(uint8_t* r, uint32_t grid, int32_t x, int32_t y) {
    return F<PtrList**>(r, grid)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
}

PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1671, x, y); }
RT3_REPLACE(0x533ba0, Terrain_Cell1671);
PtrListNode* __fastcall Terrain_AddGlobal(uint8_t* r, int, void* v) { return PtrList_Append(F<PtrList*>(r, 0x166d), 0, v); }
RT3_REPLACE(0x533bc0, Terrain_AddGlobal);
void __fastcall Terrain_RemoveGlobal(uint8_t* r, int, PtrListNode* n) { PtrList_RemoveNode(F<PtrList*>(r, 0x166d), 0, n); }
RT3_REPLACE(0x533bd0, Terrain_RemoveGlobal);
PtrList* __fastcall Terrain_GlobalList(uint8_t* r) { return F<PtrList*>(r, 0x166d); }
RT3_REPLACE(0x533be0, Terrain_GlobalList);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1671, x, y), 0, v);
}
RT3_REPLACE(0x533bf0, Terrain_Add1671);
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1671, x, y), 0, n);
}
RT3_REPLACE(0x533c20, Terrain_Remove1671);
PtrList* __fastcall Terrain_Cell1679(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1679, x, y); }
RT3_REPLACE(0x533c50, Terrain_Cell1679);
PtrListNode* __fastcall Terrain_Add1679(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1679, x, y), 0, v);
}
RT3_REPLACE(0x533c70, Terrain_Add1679);
void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1679, x, y), 0, n);
}
RT3_REPLACE(0x533ca0, Terrain_Remove1679);
PtrList* __fastcall Terrain_Cell167d(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x167d, x, y); }
RT3_REPLACE(0x533cd0, Terrain_Cell167d);

// 0x533cf0. Frees every item of the +0x167d block lists and clears them.
void __fastcall Terrain_FreeCell167d(uint8_t* r) {
    if (!F<void*>(r, 0x167d)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) <= 0) return;
    for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
        if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
        for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
            PtrList* l = Cell(r, 0x167d, i, j);
            int32_t n = F<int32_t>(l, 0xc);
            if (n <= 0) continue;
            PtrList_Rewind(l);
            void* p = PtrList_Next(l);
            do {
                Free(p);
                p = PtrList_Next(l);
            } while (--n);
            PtrList_Clear(l);
        }
    }
}
RT3_REPLACE(0x533cf0, Terrain_FreeCell167d);

} // namespace rt3
