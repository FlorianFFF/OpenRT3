// Map block / tile marking inside a circle and city cargo flag refresh in block 0x44 (batch 164). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall City_UpdateCargoFlags(uint8_t* c);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// Share of the radius left at the centre of cell (x, y).
static inline float Falloff(int32_t x, int32_t y, float cx, float cy, float r) {
    float py = (float)((double)y + D(0x5c8628));
    float px = (float)((double)x + D(0x5c8628));
    return (float)(((double)r - Distance2D(px, py, cx, cy)) / r);
}

// Sets byte `byteOff` of the 4x4 blocks (+0x212d) inside the circle (world units);
// radius 0 marks the whole bounding range.
static void MarkBlocks(uint8_t* m, float x, float y, float r, uint8_t v, int32_t byteOff) {
    float k = Fl(0x5c8568);
    float cx = (float)((double)x / k), cy = (float)((double)y / k), rr = (float)((double)r / k);
    int32_t x0, x1, y0, y1;
    float t = (float)((double)cx - rr - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)cy - rr - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x214d) - 1);
    float u = (float)((double)rr + cx + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2151) - 1);
    u = (float)((double)rr + cy + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx) {
            if (!(rr == Fl(0x5c8598)) && Falloff(bx, by, cx, cy, rr) <= Fl(0x5c8598)) continue;
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * by + bx) * 4 + byteOff] = v;
        }
}

// 0x44a0e0. Sets block byte 3 inside the circle.
void __fastcall Map_MarkBlocks3(uint8_t* m, int, float x, float y, float r, uint8_t v) { MarkBlocks(m, x, y, r, v, 3); }
RT3_REPLACE(0x44a0e0, Map_MarkBlocks3);

// 0x44a320. Sets block byte 0 inside the circle.
void __fastcall Map_MarkBlocks0(uint8_t* m, int, float x, float y, float r, uint8_t v) { MarkBlocks(m, x, y, r, v, 0); }
RT3_REPLACE(0x44a320, Map_MarkBlocks0);

// 0x44a560. Sets the upper bits (v << 3) of the tile flags (+0x2131) inside the
// circle (tile units); radius 0 marks just the centre tile.
void __fastcall Map_MarkTiles(uint8_t* m, int, float x, float y, float r, uint8_t v) {
    int32_t x0, x1, y0, y1;
    float t = (float)((double)x - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)y - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x2155) - 1);
    float u = (float)((double)x + r + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2159) - 1);
    u = (float)((double)y + r + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    for (int32_t ty = y0; ty <= y1; ++ty)
        for (int32_t tx = x0; tx <= x1; ++tx) {
            if (r == Fl(0x5c8598)) {
                if (tx != (int32_t)x || ty != (int32_t)y) continue;
            } else if (Falloff(tx, ty, x, y, r) <= Fl(0x5c8598)) continue;
            uint8_t* p = F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * ty + tx;
            *p = (uint8_t)((v << 3) | (*p & 7));
        }
}
RT3_REPLACE(0x44a560, Map_MarkTiles);

// 0x44c450. Recomputes the cargo flags of every region cell.
void __fastcall Map_UpdateCargoFlags(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            City_UpdateCargoFlags(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44c450, Map_UpdateCargoFlags);

} // namespace rt3
