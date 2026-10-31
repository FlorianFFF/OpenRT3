// Fog end of an object / water cell edits / block masks / group flags / scene pivots in blocks 0x41..0x46 (batch 130). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __stdcall Map_FogEnd(int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
void __fastcall Map_MarkNearKind245(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);
void __fastcall Group_ResetLists(uint8_t* g);
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }
static inline float FromBits(uint32_t u) { return *(float*)&u; }

// Fog end at the object's cell (Obj_CellX / Obj_CellY truncated).
static int32_t ObjFogEnd(void* o) {
    int32_t cellX, cellY;
    __asm {
        mov ecx, o
        mov eax, 0x455810
        call eax
        mov eax, 0x5a10d0
        call eax
        mov cellY, eax
        mov ecx, o
        mov eax, 0x455800
        call eax
        mov eax, 0x5a10d0
        call eax
        mov cellX, eax
    }
    return Map_FogEnd(cellX, cellY);
}

// 0x41f6b0. Fog end distance at the object's cell.
int32_t __fastcall Obj_FogEnd(void* o) { return ObjFogEnd(o); }
RT3_REPLACE(0x41f6b0, Obj_FogEnd);

// 0x4614d0. Same as 0x41f6b0 (another class).
int32_t __fastcall Obj_FogEnd2(void* o) { return ObjFogEnd(o); }
RT3_REPLACE(0x4614d0, Obj_FogEnd2);

// Cell word (3-byte stride) / byte grids of the map at (x, y).
static inline uint16_t* CellWord(uint8_t* m, int32_t x, int32_t y) {
    return (uint16_t*)(F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3);
}
static inline uint8_t& CellByte(uint8_t* m, uint32_t grid, int32_t x, int32_t y) {
    return F<uint8_t*>(m, grid)[F<int32_t>(m, 0x15dd) * y + x];
}
static inline uint8_t& ShadeByte(uint8_t* m, int32_t x, int32_t y) {
    return F<uint8_t*>(m, 0x162d)[((F<int32_t>(m, 0x15d9) >> (uint8_t)F<int32_t>(m, 0x15e1)) + 1) * y + x];
}

// 0x44de30. Floods cell (x, y) unless already water: kind 1 -> 4, 3 -> 5, else 2;
// flags both water grids, updates the counts and camera, shades it, re-marks shores.
void __fastcall Map_FloodCell(uint8_t* m, int, int32_t x, int32_t y, int32_t) {
    if (Terrain_KindIs2Or4Or5(m, 0, x, y)) return;
    uint16_t* w = CellWord(m, x, y);
    uint32_t v = *w;
    uint32_t k = v & 7;
    if (k == 1) v = (v & 0xfffc) | 4;
    else if (k == 3) v = (v & 0xfffd) | 5;
    else v = (v & 0xfffa) | 2;
    *w = (uint16_t)v;
    CellByte(m, 0x1661, x, y) = 1;
    CellByte(m, 0x1665, x, y) = 1;
    Terrain_DecCellCount(m, 0, x, y, 6);
    uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
    ShadeByte(m, x, y) = 0xc4;
    Map_MarkNearKind245(m, 0, x - 1, y - 1, x + 1, y + 1);
}
RT3_REPLACE(0x44de30, Map_FloodCell);

// 0x44df10. Drains water cell (x, y): kind 4 -> 1, 5 -> 3, else 0; clears the
// shade and water grids and re-marks shores.
void __fastcall Map_DrainCell(uint8_t* m, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs2Or4Or5(m, 0, x, y)) return;
    ShadeByte(m, x, y) = 0;
    CellByte(m, 0x1661, x, y) = 0;
    CellByte(m, 0x1665, x, y) = 0;
    uint16_t* w = CellWord(m, x, y);
    uint32_t v = *w;
    uint32_t k = v & 7;
    if (k == 4) v = (v & 0xfff9) | 1;
    else if (k == 5) v = (v & 0xfffb) | 3;
    else v &= 0xfff8;
    *w = (uint16_t)v;
    Map_MarkNearKind245(m, 0, x - 1, y - 1, x + 1, y + 1);
}
RT3_REPLACE(0x44df10, Map_DrainCell);

// 0x454d70. Recomputes the 4-bit mask of block (x, y): bit a + 2b set when the
// 4x4 quarter (a, b) of fine cells (x*4-1+2a.., y*4-1+2b..) has a cell in either
// bit grid (+0x2139 / +0x213d); stores it with Terrain_SetFlag and flags the renderer.
// Quirk: the far edge is clamped only when the near edge is already past it.
void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t w = F<int32_t>(m, 0x215d);
    int32_t hMax = F<int32_t>(m, 0x2161) - 1;
    uint8_t mask = 0;
    for (int32_t a = 0; a < 2; ++a) {
        int32_t sx = x * 4 - 1 + 2 * a;
        for (int32_t b = 0; b < 2; ++b) {
            int32_t sy = y * 4 - 1 + 2 * b;
            int32_t x0 = sx < 0 ? 0 : sx, x1 = sx + 3;
            int32_t y0 = sy < 0 ? 0 : sy, y1 = sy + 3;
            if (x0 > w - 1) x1 = w - 1;
            if (y0 > hMax) y1 = hMax;
            for (int32_t i = x0; i <= x1; ++i) {
                const uint8_t* g0 = F<uint8_t*>(m, 0x2139);
                for (int32_t j = y0; j <= y1; ++j) {
                    int32_t idx = w * j + i;
                    uint8_t bit = (uint8_t)(1 << (idx & 7));
                    if ((g0[idx >> 3] & bit) || (F<uint8_t*>(m, 0x213d)[idx >> 3] & bit)) {
                        mask |= (uint8_t)(1 << (a + 2 * b));
                        goto next;
                    }
                }
            }
        next:;
        }
    }
    Terrain_SetFlag(m, 0, x, y, mask);
    F<uint8_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x374a) = 1;
}
RT3_REPLACE(0x454d70, Map_UpdateBlockMask);

// Applies o's 4 element flags (+0xf4) to the group of each id in ids (up to 4,
// 0-terminated) looked up in the entry list 0x6acd50 ({id, flags, group}, +0xc
// entries); entries with flag 0x20000 only have it cleared.
static void SyncGroups(uint8_t* o, const uint32_t* ids, int32_t full, int32_t modeIfNotFull) {
    for (int32_t k = 0; k < 4; ++k) {
        uint32_t id = ids[k];
        if (!id) return;
        uint8_t* list = hook::Global<uint8_t*>(0x6acd50);
        int32_t n = F<int32_t>(list, 0xc);
        uint8_t** group = nullptr;
        for (int32_t i = 0; i < n; ++i)
            if (F<uint32_t>(list, 0x10 + i * 0xc) == id) {
                group = (uint8_t**)(list + 0x18 + i * 0xc);
                break;
            }
        uint32_t* flags = nullptr;
        for (int32_t i = 0; i < n; ++i)
            if (F<uint32_t>(list, 0x10 + i * 0xc) == id) {
                flags = (uint32_t*)(list + 0x14 + i * 0xc);
                break;
            }
        if (*flags & 0x20000) {
            *flags &= ~0x20000u;
            continue;
        }
        for (int32_t s = 0; s < 4; ++s) {
            int32_t r = Obj_EntryClear1d8(*group, s);
            if (!o[0xf4 + s]) continue;
            int32_t mode;
            if (!full) {
                Group_ResetLists(*group);
                mode = modeIfNotFull;
            } else {
                if (!r && F<int32_t>(*group, 8) > s) Group_ResetLists(*group);
                mode = !r ? 1 : 0;
            }
            Obj_SetElementFlag1d8(*group, mode, s);
        }
    }
}

// 0x459e10. Syncs o's element flags to its groups (+0xc8, then +0xd8).
void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full) {
    SyncGroups(o, (const uint32_t*)(o + 0xc8), full, 1);
    SyncGroups(o, (const uint32_t*)(o + 0xd8), full, 0);
}
RT3_REPLACE(0x459e10, Obj_SyncGroupFlags);

// 0x45c500. Turns pivot (a, b) of o (+0x316 +0xa0, 48 per row) to `angle`: rotates
// its offset (+0x1c-+0x20, +0x28-+0x2c) to place the parts +0x38 / +0x34, then
// swings arm +0x30 around +0x34 and aims +0x34 at it (atan2 less +4).
// Quirk: the arm uses sin/cos of +0 alone, not +0 + angle, for its axes.
void __fastcall Obj_TurnPivot(uint8_t* o, int, float angle, int32_t a, int32_t b) {
    uint8_t* p = F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xa0 + (a * 48 + b) * 4);
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float u = (float)((double)F<float>(p, 0x1c) - F<float>(p, 0x20));
    float v = (float)((double)F<float>(p, 0x28) - F<float>(p, 0x2c));
    double t = cos_(angle) * v;
    float rx = (float)(sin_(angle) * u + t);
    t = cos_(angle) * u;
    float rz = (float)(t - sin_(angle) * v);
    SceneObj_SetAngles(F<uint8_t*>(p, 0x3c), 0, Bits(angle), 0, 0);
    if (F<uint8_t*>(p, 0x38))
        SceneObj_SetPos(F<uint8_t*>(p, 0x38), 0, F<uint32_t>(p, 0x14), Bits((float)((double)rx + F<float>(p, 0x20))),
                        Bits((float)((double)rz + F<float>(p, 0x2c))));
    if (!F<uint8_t*>(p, 0x34)) return;
    SceneObj_SetPos(F<uint8_t*>(p, 0x34), 0, F<uint32_t>(p, 0x10), Bits((float)((double)rx + F<float>(p, 0x20))),
                    Bits((float)((double)rz + F<float>(p, 0x2c))));
    float base = F<float>(p, 0);
    float arm = (float)(cos_((double)base + angle) * u);
    float az = (float)(sin_(base) * arm + F<float>(p, 0x24));
    float ax = (float)(cos_(base) * arm + F<float>(p, 0x18));
    SceneObj_SetPos(F<uint8_t*>(p, 0x30), 0, F<uint32_t>(p, 0xc), Bits(ax), Bits(az));
    uint32_t x1, y1, z1, x2, y2, z2;
    SceneObj_GetPos2(F<uint8_t*>(p, 0x30), 0, &x1, &y1, &z1);
    SceneObj_GetPos2(F<uint8_t*>(p, 0x34), 0, &x2, &y2, &z2);
    double r = ((double(__cdecl*)(double, double))0x5a29d0)((double)FromBits(z1) - FromBits(z2),
                                                           (double)FromBits(y1) - FromBits(y2));
    SceneObj_SetAngles(F<uint8_t*>(p, 0x34), 0, Bits((float)(r - F<float>(p, 4))), 0, 0);
}
RT3_REPLACE(0x45c500, Obj_TurnPivot);

} // namespace rt3
