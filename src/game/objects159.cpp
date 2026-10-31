// Camera ground follow / camera orbit angles / camera drag axis lock / map grid allocation in blocks 0x43..0x44 (batch 163). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Camera_ClampToGround(uint8_t* c);
void __fastcall Camera_FollowGround(uint8_t* c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy);
double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
void* __fastcall Engine_Renderer(void* e);
Collection* __fastcall Tracks_Construct(Collection* c, int, int32_t columns, int32_t);
void __fastcall Map_ResetCells(uint8_t* m);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
static inline uint8_t* Renderer() { return (uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)); }

// 0x43ce80. Per-frame camera ground handling: free mode clamps above ground; else,
// with the per-mode flag (0x5ee4d0[option +0x10c]), follows the ground and remembers
// the eye height (+0x2ee).
void __fastcall Camera_UpdateGround(uint8_t* c) {
    if (F<int32_t>(c, 0x60)) {
        Camera_ClampToGround(c);
        return;
    }
    if (!((const uint8_t*)0x5ee4d0)[F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c)]) return;
    Camera_FollowGround(c);
    F<uint32_t>(c, 0x2ee) = F<uint32_t>(c, 8);
}
RT3_REPLACE(0x43ce80, Camera_UpdateGround);

// 0x43d480. Works out the camera's focus point (+0xee/+0xf2/+0xf6: the followed
// object, else the screen-centre cell, else the eye itself) with its screen position
// (+0xfe/+0x102), distance (+0xfa), heading (+0x106, mod 2 pi) and the heading
// relative to the camera yaw (+0x10a).
void __fastcall Camera_UpdateFocus(uint8_t* c) {
    F<int32_t>(c, 0xea) = 0;
    F<uint32_t>(c, 0xfe) = F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa94);
    F<uint32_t>(c, 0x102) = F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa98);
    float x = 0.0f, y = 0.0f;
    if (F<void*>(Renderer(), 0x366e)) {
        F<int32_t>(c, 0xea) = 1;
        x = (float)Obj_CellX(F<void*>(Renderer(), 0x366e));
        y = (float)Obj_CellY(F<void*>(Renderer(), 0x366e));
    }
    bool height = F<int32_t>(c, 0xea) != 0;
    if (!height) {
        int32_t ok = 0;
        if (Scene_FocusCell(Renderer(), 0, &x, &y) == 1 && !(x < Fl(0x5c8598)) && !(y < Fl(0x5c8598))) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (x < (float)F<int32_t>(m, 0x2155) && y < (float)F<int32_t>(m, 0x2159)) ok = 1;
        }
        F<int32_t>(c, 0xea) = ok;
        height = ok != 0;
    }
    if (height) F<float>(c, 0xf2) = (float)Terrain_HeightAtTile(hook::Global<uint8_t*>(0x62c120), 0, x, y);
    float k = Fl(0x5c8700);
    F<float>(c, 0xee) = (float)((double)x * k);
    F<float>(c, 0xf6) = (float)((double)y * k);
    if (!F<int32_t>(c, 0xea)) {
        F<uint32_t>(c, 0xee) = F<uint32_t>(c, 4);
        F<uint32_t>(c, 0xf2) = F<uint32_t>(c, 8);
        F<uint32_t>(c, 0xf6) = F<uint32_t>(c, 0xc);
        F<int32_t>(c, 0xea) = 1;
        float tz = (float)((double)F<float>(c, 0xc) / k);
        float tx = (float)((double)F<float>(c, 4) / k);
        if (!Map_TileToScreen(tx, tz, F<float>(c, 8), &F<float>(c, 0xfe), &F<float>(c, 0x102))) {
            F<float>(c, 0xfe) = 0.5f;
            F<float>(c, 0x102) = (float)(((double)F<float>(c, 0x5c) - F<float>(c, 0x54)) * D(0x5c8628));
        }
    }
    F<uint32_t>(c, 0xf2) = F<uint32_t>(c, 0x20);
    F<float>(c, 0xfa) = (float)Distance3D(F<float>(c, 0xee), F<float>(c, 0x20), F<float>(c, 0xf6), F<float>(c, 0x1c),
                                          F<float>(c, 0x20), F<float>(c, 0x24));
    float dx = (float)((double)F<float>(c, 0xee) - F<float>(c, 0x1c));
    if (dx == Fl(0x5c8598)) dx = 0.1f;
    float a = (float)(D(0x5c8740) - ((double(__cdecl*)(double))0x5a1270)(((double)F<float>(c, 0xf6) - F<float>(c, 0x24)) / dx));
    F<float>(c, 0x106) = a;
    if (F<float>(c, 0xee) < F<float>(c, 0x1c)) F<float>(c, 0x106) = (float)((double)a - Fl(0x5c8760));
    float m = (float)((double(__cdecl*)(double, double))0x5a1390)(F<float>(c, 0x106), 6.2831854820251465);
    F<float>(c, 0x106) = m;
    F<float>(c, 0x10a) = (float)((double)F<float>(c, 0x10) - m);
}
RT3_REPLACE(0x43d480, Camera_UpdateFocus);

// 0x43e830. Mouse drag of the camera: on the first move of a drag (+0x306 clear)
// decides which axes follow (+0x33e horizontal, +0x342 vertical) from the dominant
// direction; an axis left out joins once its accumulated motion dominates enough.
// Horizontal turns the yaw (+0x8a), vertical the pitch (+0x92) by the speed option.
void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t) {
    const double k = D(0x5c85c8);
    if (!F<int32_t>(c, 0x306)) {
        F<int32_t>(c, 0x30a) = 0;
        F<int32_t>(c, 0x30e) = 0;
        F<int32_t>(c, 0x33e) = 0;
        F<int32_t>(c, 0x342) = 0;
        if (Fabs(dy) * k < Fabs(dx)) F<int32_t>(c, 0x33e) = 1;
        else if (Fabs(dx) * k < Fabs(dy)) F<int32_t>(c, 0x342) = 1;
        else {
            if (Fabs(dx) > D(0x5c85f0)) F<int32_t>(c, 0x33e) = 1;
            if (Fabs(dy) > D(0x5c85f0)) F<int32_t>(c, 0x342) = 1;
        }
    }
    float sx = (float)((double)dx + F<float>(c, 0x30a));
    F<float>(c, 0x30a) = sx;
    F<float>(c, 0x30e) = (float)((double)dy + F<float>(c, 0x30e));
    if (!F<int32_t>(c, 0x33e)) {
        double t = Fabs(sx) * k;
        if (Fabs(F<float>(c, 0x30e)) < t) F<int32_t>(c, 0x33e) = 1;
        F<float>(c, 0x30a) = dx;
    }
    if (!F<int32_t>(c, 0x342)) {
        double t = Fabs(F<float>(c, 0x30e)) * k;
        if (Fabs(F<float>(c, 0x30a)) < t) F<int32_t>(c, 0x342) = 1;
        F<float>(c, 0x30e) = dy;
    }
    if (F<int32_t>(c, 0x33e)) F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5ca19c) + F<float>(c, 0x8a));
    if (F<int32_t>(c, 0x342)) {
        float speed = ((const float*)0x5f34b0)[F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x118)];
        F<float>(c, 0x92) = (float)(F<float>(c, 0x92) - (double)dy * speed * D(0x5ca278));
    }
    F<int32_t>(c, 0x306) = 1;
}
RT3_REPLACE(0x43e830, Camera_Drag);

static inline uint8_t* ZeroedAlloc(size_t n) {
    uint8_t* p = (uint8_t*)Malloc(n);
    memset(p, 0, n);
    return p;
}

// 0x448470. Allocates the map's grids once (+0x2165) from the vertex size
// (+0x15d5/+0x15d9): tile counts, 4x4 block counts, the track table (0x6cfca8),
// three bit planes, the tile flags (all 8), the vertex grid, the block grid (enabled)
// and the region cells (0x1d7 bytes each), then resets the cells.
RT3_NOINLINE void __fastcall Map_AllocGrids(uint8_t* m) {
    if (F<int32_t>(m, 0x2165)) return;
    int32_t h = F<int32_t>(m, 0x15d9) - 1, w = F<int32_t>(m, 0x15d5) - 1;
    int32_t bh = (h + 3) / 4, bw = (w + 3) / 4;
    F<int32_t>(m, 0x2159) = w;
    F<int32_t>(m, 0x2151) = bw;
    F<int32_t>(m, 0x2149) = bw;
    F<int32_t>(m, 0x2165) = 1;
    F<int32_t>(m, 0x2155) = h;
    F<int32_t>(m, 0x214d) = bh;
    F<int32_t>(m, 0x2145) = bh;
    F<int32_t>(m, 0x215d) = h * 4;
    F<int32_t>(m, 0x2161) = w * 4;
    F<int32_t>(m, 0x21a9) = w * h;
    void* p = ((void*(__cdecl*)(size_t))0x53b070)(0x20a);
    hook::Global<Collection*>(0x6cfca8) =
        p ? Tracks_Construct((Collection*)p, 0, F<int32_t>(m, 0x2155), F<int32_t>(m, 0x2159)) : nullptr;
    int32_t bits = (F<int32_t>(m, 0x215d) * F<int32_t>(m, 0x2161) + 7) / 8;
    int32_t tiles = F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155);
    int32_t blocks = F<int32_t>(m, 0x214d) * F<int32_t>(m, 0x2151) * 4;
    int32_t verts = (F<int32_t>(m, 0x2159) + 1) * (F<int32_t>(m, 0x2155) + 1);
    int32_t cells = F<int32_t>(m, 0x2145) * F<int32_t>(m, 0x2149) * 0x1d7;
    F<uint8_t*>(m, 0x2139) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x213d) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x2141) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x2131) = ZeroedAlloc(tiles);
    for (int32_t i = 0; i < F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155); ++i) {
        uint8_t* t = F<uint8_t*>(m, 0x2131) + i;
        *t = (*t & 7) | 8;
    }
    F<uint8_t*>(m, 0x2135) = ZeroedAlloc(verts);
    F<uint8_t*>(m, 0x212d) = ZeroedAlloc(blocks);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2151); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x214d); ++x) {
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4] = 1;
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4 + 3] = 0;
        }
    F<uint8_t*>(m, 0x2129) = (uint8_t*)Malloc(cells);
    Map_ResetCells(m);
}
RT3_REPLACE(0x448470, Map_AllocGrids);

} // namespace rt3
