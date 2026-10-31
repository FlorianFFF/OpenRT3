// Sound front-end / terrain block grids (flags, counters, save/load) in blocks 0x53..0x56 (batch 120). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/containers.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol);
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol);
void __fastcall Sound2D_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
void __fastcall Sound_ReleaseTimer(uint8_t* s);
void __fastcall Sound2D_PauseAll(uint8_t* s);
void __fastcall Sound_PauseAll(uint8_t* s);
void __fastcall Sound3D_Retire(uint8_t* s, int, uint32_t key, uint32_t* smp);
void __fastcall Sound2D_StopAll(uint8_t* s);
void __fastcall Sound_WaitIdle();
void __fastcall BlockGrid_Set(uint8_t* g, int, int32_t, int32_t a, int32_t b, uint8_t v);
uint8_t __fastcall BlockGrid_Get(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_Free(uint8_t* g);
void* __fastcall Engine_Renderer(void* e);

static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

// 0x531560. Sets a volume: kind 0 music, kind 2 effects (2D and 3D).
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind) {
    if (!Is3D(s)) return;
    if (kind == 0) {
        Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, vol);
    } else if (kind == 2) {
        Sound2D_SetVolume(hook::Global<uint8_t*>(0xccb9f8), 0, vol);
        Sound_SetEffectsVolume(hook::Global<uint8_t*>(0xccb9fc), 0, vol);
    }
}
RT3_REPLACE(0x531560, Sound_SetVolume);

void __fastcall Sound_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol) {
    if (Is3D(s)) Sound2D_SetSampleVolume(hook::Global<uint8_t*>(0xccb9f8), 0, key, vol);
}
RT3_REPLACE(0x531790, Sound_SetSampleVolume);

// 0x531f40. Pauses all sound (stopping the fade timer first).
void __fastcall Sound_Pause(uint8_t* s) {
    if (!Is3D(s)) return;
    if (F<int32_t>(s, 0x4c) != -1 && hook::Global<int32_t>(0x624d58) == 1) Sound_ReleaseTimer(s);
    Sound2D_PauseAll(hook::Global<uint8_t*>(0xccb9f8));
    Sound_PauseAll(hook::Global<uint8_t*>(0xccb9fc));
    F<int32_t>(s, 0x48) = 0;
}
RT3_REPLACE(0x531f40, Sound_Pause);

// Terrain sound-flag grids: one block grid per 16x16 vertices (+0x1685).
static inline uint8_t* FlagGrid(uint8_t* t, int32_t x, int32_t y) {
    return F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * (y >> 4) + (x >> 4)];
}
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) {
    if (uint8_t* g = FlagGrid(t, x, y)) BlockGrid_Set(g, 0, (int32_t)t, x % 16, y % 16, v);
}
RT3_REPLACE(0x533fe0, Terrain_SetFlag);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = FlagGrid(t, x, y);
    return g ? BlockGrid_Get(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x534040, Terrain_GetFlag);

// 0x535d30. Destroys the flag grids (+0x1685).
RT3_NOINLINE void __fastcall Terrain_DestroyCell1685(uint8_t* r) {
    if (!F<void*>(r, 0x1685)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                uint8_t* g = F<uint8_t**>(r, 0x1685)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (!g) continue;
                BlockGrid_Free(g);
                ((void(__cdecl*)(void*))0x53b080)(g);
                F<uint8_t**>(r, 0x1685)[(F<int32_t>(r, 0x15d9) >> 4) * j + i] = nullptr;
            }
        }
    }
    Free(F<void*>(r, 0x1685));
    F<void*>(r, 0x1685) = nullptr;
}
RT3_REPLACE(0x535d30, Terrain_DestroyCell1685);

// 0x569c40. Stops a 3D sample (given, or looked up by key) and retires it.
void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp) {
    Sound_WaitIdle();
    Import<void(__stdcall*)()>(0x5c8484)();
    if (!smp && key) smp = (uint8_t*)Hash_Get((HashMap*)(s + 0x14), 0, key);
    if (smp) {
        uint32_t h = F<uint32_t>(smp, -4);
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a4)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8414)(h);
        Sound3D_Retire(s, 0, h, (uint32_t*)smp);
    }
    Import<void(__stdcall*)()>(0x5c83e4)();
}
RT3_REPLACE(0x569c40, Sound3D_Stop);

// 0x56aeb0. Closes 2D sound: stops everything, frees the table, closes the driver (vtbl +4).
void __fastcall Sound2D_Close(uint8_t* s) {
    if (!F<void*>(s, 0x2c)) return;
    Sound2D_StopAll(s);
    Hash_Destroy((HashMap*)(s + 0xc));
    ((void(__thiscall*)(void*))(*(void***)s)[1])(s);
    F<void*>(s, 0x2c) = nullptr;
}
RT3_REPLACE(0x56aeb0, Sound2D_Close);

// Block grid cells (+0xa: low nibble flag, bits 4..6 counter; +0xb byte); blocks
// start with a 16-bit cell count (+0) and are freed when it reaches 0.
static inline uint8_t** Slot(uint8_t* g, int32_t a, int32_t b) { return (uint8_t**)(g + 0x14 + (b / 4 + (a / 4) * 4) * 4); }
static inline uint8_t* CellIn(uint8_t* blk, int32_t a, int32_t b) { return blk + (b % 4 + (a % 4) * 4) * 8 + 0xa; }

// 0x56b0e0. Decrements cell (a, b)'s counter (and the totals); frees an emptied block.
int32_t __fastcall BlockGrid_Dec(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t** slot = Slot(g, a, b);
    uint8_t* blk = *slot;
    if (blk) {
        uint8_t* c = CellIn(blk, a, b);
        uint8_t v = *c;
        uint8_t n = v >> 4;
        if (n & 7) {
            *c = (uint8_t)((v & 0x8f) | ((uint8_t)((n - 1) << 4) & 0x70));
            F<uint16_t>(blk, 0)--;
            F<int32_t>(g, 0x10)--;
        }
        if (!F<uint16_t>(blk, 0)) {
            Free(*slot);
            *slot = nullptr;
        }
    }
    return F<int32_t>(g, 0x10);
}
RT3_REPLACE(0x56b0e0, BlockGrid_Dec);

uint32_t __fastcall BlockGrid_Count(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* blk = *Slot(g, a, b);
    return blk ? (*CellIn(blk, a, b) >> 4) & 7 : 0;
}
RT3_REPLACE(0x56b180, BlockGrid_Count);
void __fastcall BlockGrid_SetCount(uint8_t* g, int, int32_t a, int32_t b, uint8_t n) {
    uint8_t* blk = *Slot(g, a, b);
    if (!blk) return;
    uint8_t* c = CellIn(blk, a, b);
    *c = (uint8_t)((*c & 0x8f) | ((uint8_t)(n << 4) & 0x70));
}
RT3_REPLACE(0x56b1f0, BlockGrid_SetCount);
uint32_t __fastcall BlockGrid_Byte(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* blk = *Slot(g, a, b);
    return blk ? CellIn(blk, a, b)[1] : 0;
}
RT3_REPLACE(0x56b260, BlockGrid_Byte);
// 0x56b700. Sets cell (a, b)'s byte and marks the scene for redraw.
void __fastcall BlockGrid_SetByte(uint8_t* g, int, int32_t a, int32_t b, uint8_t v) {
    uint8_t* blk = *Slot(g, a, b);
    if (!blk) return;
    CellIn(blk, a, b)[1] = v;
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x374a] = 1;
}
RT3_REPLACE(0x56b700, BlockGrid_SetByte);

// 0x56b490 / 0x56b510. Loads / saves the grid header (0x54 bytes) and its 16
// blocks (0x8a bytes each, preceded by a present flag).
void __fastcall BlockGrid_Load(uint8_t* g, int, Stream s) {
    StreamRead(s, g, 0x54, 0, 1);
    for (int32_t row = 5; row < 0x15; row += 4)
        for (int32_t k = 0; k < 4; ++k) {
            uint8_t flag = 0;
            StreamRead(s, &flag, 1, 0, 1);
            if (!flag) continue;
            void* blk = Malloc(0x8a);
            F<void*>(g, (row + k) * 4) = blk;
            StreamRead(s, blk, 0x8a, 0, 1);
        }
}
RT3_REPLACE(0x56b490, BlockGrid_Load);
void __fastcall BlockGrid_Save(uint8_t* g, int, Stream s) {
    StreamWrite(s, g, 0x54, 0);
    for (int k = 0; k < 16; ++k) {
        void* blk = F<void*>(g, 0x14 + k * 4);
        uint8_t flag = blk != nullptr;
        StreamWrite(s, &flag, 1, 0);
        if (F<void*>(g, 0x14 + k * 4)) StreamWrite(s, F<void*>(g, 0x14 + k * 4), 0x8a, 0);
    }
}
RT3_REPLACE(0x56b510, BlockGrid_Save);

} // namespace rt3
