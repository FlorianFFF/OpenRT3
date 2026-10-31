// Sound stop dispatch / terrain flag-grid wrappers / atexit hook in blocks 0x53..0x56 (batch 121). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Sound2D_Stop(uint8_t* s, int, uint32_t key);
void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp);
void __fastcall BlockGrid_SetByte(uint8_t* g, int, int32_t a, int32_t b, uint8_t v);
uint32_t __fastcall BlockGrid_Byte(uint8_t* g, int, int32_t a, int32_t b);
uint32_t __fastcall BlockGrid_Count(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_SetCount(uint8_t* g, int, int32_t a, int32_t b, uint8_t n);
int32_t __fastcall BlockGrid_Dec(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_Free(uint8_t* g);

static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

// 0x5317b0. Stops sample `key`: kind 0 = 2D, kind 1 = 3D.
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind) {
    if (!Is3D(s)) return;
    if (kind == 0) Sound2D_Stop(hook::Global<uint8_t*>(0xccb9f8), 0, key);
    else if (kind == 1) Sound3D_Stop(hook::Global<uint8_t*>(0xccb9fc), 0, key, nullptr);
}
RT3_REPLACE(0x5317b0, Sound_Stop);

// Terrain flag grids (+0x1685, one per 16x16 vertices).
static inline uint8_t** GridSlot(uint8_t* t, int32_t x, int32_t y) {
    return F<uint8_t**>(t, 0x1685) + ((F<int32_t>(t, 0x15d9) >> 4) * (y >> 4) + (x >> 4));
}

void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) {
    if (uint8_t* g = *GridSlot(t, x, y)) BlockGrid_SetByte(g, 0, x % 16, y % 16, v);
}
RT3_REPLACE(0x533f20, Terrain_SetCellByte);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = *GridSlot(t, x, y);
    return g ? BlockGrid_Byte(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x533f80, Terrain_CellByte);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = *GridSlot(t, x, y);
    return g ? BlockGrid_Count(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x5340a0, Terrain_CellCount);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n) {
    if (uint8_t* g = *GridSlot(t, x, y)) BlockGrid_SetCount(g, 0, x % 16, y % 16, n);
}
RT3_REPLACE(0x534100, Terrain_SetCellCount);

// 0x534270. Decrements cell (x, y)'s counter up to n (max 6) times; once its grid
// is empty the grid is freed.
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n) {
    if (!*GridSlot(t, x, y)) return;
    int32_t cx = x % 16, cy = y % 16;
    if (n > 6) n = 6;
    for (int32_t i = 0; i < n; ++i) {
        if (BlockGrid_Dec(*GridSlot(t, x, y), 0, cx, cy)) continue;
        if (uint8_t* g = *GridSlot(t, x, y)) {
            BlockGrid_Free(g);
            ((void(__cdecl*)(void*))0x53b080)(g);
        }
        *GridSlot(t, x, y) = nullptr;
        return;
    }
}
RT3_REPLACE(0x534270, Terrain_DecCellCount);

// 0x56c840. Registers the module cleanup 0x56c830 with atexit.
int32_t __fastcall Module_RegisterCleanup() {
    ((int(__cdecl*)(void*))0x5a2d10)((void*)0x56c830);
    return 0;
}
RT3_REPLACE(0x56c840, Module_RegisterCleanup);

} // namespace rt3
