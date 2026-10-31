// Larger game functions, part 11 (batch 212). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);

// 0x40a150. Re-applies the world clock (refreshes the derived times).
void __fastcall World_RefreshTime(uint8_t* w) { World_SetTime(w, 0, F<int32_t>(w, 0x15)); }
RT3_REPLACE(0x40a150, World_RefreshTime);

// 0x4351c0. Schedules the next weather spell: a random kind (40% heavy 0xff, 30%
// 200, 30% 230 in +0x61), a length of 40..60 days and a start some hours ahead
// (aligned to the clock's time of day, maybe a day later); with `scaled` the start
// is spread by how far the scene's weather level (+0x15) is from the target. Sets
// the start (+0x51) and the build-up / peak / end times (+0x55 / +0x59 / +0x5d).
void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled) {
    int32_t roll = SimRand(0, 100);
    int32_t level = F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x15);
    int32_t len, d;
    if (roll < 0x28) {
        F<int32_t>(w, 0x61) = 0xff;
        len = SimRand(0x28, 0x37) * 0x5a0;
        d = SimRand(0x898, 0xa28);
    } else if (roll < 0x46) {
        F<int32_t>(w, 0x61) = 200;
        len = SimRand(0x2d, 0x3c) * 0x5a0;
        d = SimRand(0x320, 0x41a);
    } else {
        F<int32_t>(w, 0x61) = 0xe6;
        len = SimRand(0x28, 0x37) * 0x5a0;
        d = SimRand(0x4b0, 0x640);
    }
    float ahead = (float)((double)d / D(0x5c85d8));
    World_SetTime(w, 0, F<int32_t>(w, 0x15));
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, &h, &m, &s);
    float now = (float)((double)m / D(0x5c88a8) + (double)s / D(0x5c9f30) + (double)h);
    ahead = (float)((double)ahead - now);
    if (SimRand(0, 100) < 0x32) ahead = (float)((double)ahead + D(0x5c9ab8));
    if (!(ahead > D(0x5c8978)) && ahead == ahead) ahead = (float)((double)ahead + D(0x5c9ab8));
    if (scaled) {
        int32_t kind = F<int32_t>(w, 0x61);
        if (kind <= level) kind = level;
        F<int32_t>(w, 0x61) = kind;
        if (level >= kind) ahead = (float)((double)len * D(0x5c9f28) / D(0x5c9f20));
        else {
            int32_t c = kind < 1 ? 1 : kind;
            ahead = (float)((double)(float)level / (float)c * ((double)len * D(0x5c9f28)) / D(0x5c9f20));
        }
    }
    int32_t start = Ftol((double)ahead * Fl(0x5c9f18) + (float)F<int32_t>(w, 0x15));
    F<int32_t>(w, 0x51) = start;
    double l = len, st = start;
    F<int32_t>(w, 0x55) = Ftol(l * D(0x5c9f28) + st);
    F<int32_t>(w, 0x59) = Ftol(l * D(0x5c9f10) + st);
    F<int32_t>(w, 0x5d) = Ftol(st + l);
}
RT3_REPLACE(0x4351c0, Weather_ScheduleNext);

void __fastcall LerpPercent(int32_t pct, float* ox, float x0, float y0, float x1, float y1, float* oy);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);

// 0x48bdb0. Marks the map under a track piece (unless the node table is bulk
// loading, +0xed): six points sampled along its two curve segments (control points
// from Effect_GetVectors, in cells) flag their cell (in town pieces bit 4, with the
// fine-grid street bit when +8; otherwise fine-grid layer 0/1 by kind +0x226 and
// cell bit 2).
void __fastcall Track_MarkCells(uint8_t* t) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cfca8), 0xed)) return;
    int32_t town = (*(int32_t(__fastcall**)(void*))(*(uint8_t**)t + 0x14))(t);
    int32_t street = F<int32_t>(t, 8) != 0;
    int32_t layer = (F<int32_t>(t, 0x226) == 1 || F<int32_t>(t, 0x226) == 3) ? 0 : 1;
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float cell = *(const float*)0x5c8700;
    float* pts[4] = { a, b, c, d };
    for (float* v : pts) {
        v[0] = (float)((double)v[0] / cell);
        v[2] = (float)((double)v[2] / cell);
    }
    float px[6], py[6];
    LerpPercent(0x14, &px[0], a[0], a[2], b[0], b[2], &py[0]);
    LerpPercent(0x50, &px[1], a[0], a[2], b[0], b[2], &py[1]);
    LerpPercent(0x14, &px[2], c[0], c[2], d[0], d[2], &py[2]);
    LerpPercent(0x50, &px[3], c[0], c[2], d[0], d[2], &py[3]);
    LerpPercent(0x32, &px[4], px[0], py[0], px[3], py[3], &py[4]);
    LerpPercent(0x32, &px[5], px[1], py[1], px[2], py[2], &py[5]);
    for (int32_t k = 0; k < 6; ++k) {
        int32_t xi = Ftol((double)px[k] * Fl(0x5c8568)), yi = Ftol((double)py[k] * Fl(0x5c8568));
        auto grid = [&]() -> uint8_t& {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            int32_t row = Ftol(py[k]);
            return F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * row + Ftol(px[k])];
        };
        if (town) {
            grid() |= 4;
            if (street) Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, xi, yi, 1, 2, 1);
        } else {
            Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, xi, yi, 1, layer, 1);
            grid() |= 2;
        }
    }
}
RT3_REPLACE(0x48bdb0, Track_MarkCells);

int32_t __fastcall Grid_NearestObject(Collection* c, int, float x, float y, int32_t skip1, int32_t skip2, int32_t freeOnly);

// 0x48cc20. Whether the track piece overlaps another one: 17 probe points (its four
// control points, the centre, the edge midpoints and points 0x5c88f0 of the way
// along the edges) are looked up in the node grid, skipping the piece's own
// neighbours (+0x202 with +0x206 or +0x20a); a hit on a non-bridge piece (+0x226 == 0)
// answers 1 and stores its id in *hit.
int32_t __fastcall Track_FindOverlap(uint8_t* t, int, int32_t* hit) {
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const double cell = *(const float*)0x5c8700, four = *(const float*)0x5c8568;
    auto probe = [&](float x, float y, uint32_t other) {
        Collection* nodes = hook::Global<Collection*>(0x6cfca8);
        int32_t id = Grid_NearestObject(nodes, 0, x, y, F<int32_t>(t, 0x202), F<int32_t>(t, other), 1);
        if (id == -1) return false;
        if (F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id), 0x226)) return false;
        if (hit) *hit = id;
        return true;
    };
    auto fine = [&](double v) { return (float)(v / cell * four); };
    auto at = [&](const float* p, uint32_t other) { return probe(fine(p[0]), fine(p[2]), other); };
    auto mid = [&](const float* p, const float* q, uint32_t other) {
        auto m = [&](double u, double v) { return (float)((u + v) / D(0x5c85d0) / D(0x5c8c48) * D(0x5c87b8)); };
        return probe(m(p[0], q[0]), m(p[2], q[2]), other);
    };
    auto along = [&](const float* from, const float* to, uint32_t other) {
        float px = (float)(((double)to[0] - from[0]) * D(0x5c88f0) + from[0]);
        float pz = (float)(((double)to[2] - from[2]) * D(0x5c88f0) + from[2]);
        return probe(fine(px), fine(pz), other);
    };
    if (at(a, 0x206) || at(b, 0x206) || at(c, 0x20a) || at(d, 0x20a)) return 1;
    float cx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl(0x5cfcfc));
    float cz = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl(0x5cfcfc));
    if (probe((float)((double)cx * four), (float)((double)cz * four), 0x20a)) return 1;
    if (mid(b, a, 0x206) || mid(c, b, 0x206) || mid(d, c, 0x20a) || mid(d, a, 0x20a)) return 1;
    if (along(d, a, 0x206) || along(c, b, 0x206) || along(b, c, 0x20a) || along(a, d, 0x20a)) return 1;
    if (along(b, a, 0x206) || along(a, b, 0x206) || along(d, c, 0x20a) || along(c, d, 0x20a)) return 1;
    return 0;
}
RT3_REPLACE(0x48cc20, Track_FindOverlap);

// 0x482880. Fixes the game's time of day (+0x124 on/off, +0x128 hour) and
// refreshes the world clock.
void __stdcall Game_SetFixedTime(int32_t on, int32_t hour) {
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x124) = on;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x128) = hour;
    if (uint8_t* w = hook::Global<uint8_t*>(0x6cec78)) World_RefreshTime(w);
}
RT3_REPLACE(0x482880, Game_SetFixedTime);

void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);

// 0x492360. Re-marks the map under every track piece listed in the grid blocks
// around fine cell (fx, fy); stale entries are freed (only the list head is
// unlinked, as in the original).
void __fastcall Nodes_RemarkAround(Collection* nodes, int, int32_t fx, int32_t fy) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t x0 = (fx - 1 < 0 ? 0 : fx - 1) / 4;
    int32_t xm = F<int32_t>(m, 0x2155) * 4 - 4;
    int32_t x1 = (xm < fx + 1 ? xm : fx + 1) / 4;
    int32_t y0 = (fy - 1 < 0 ? 0 : fy - 1) / 4;
    int32_t ym = F<int32_t>(m, 0x2159) * 4 - 4;
    int32_t y1 = (ym < fy + 1 ? ym : fy + 1) / 4;
    for (int32_t x = x0; x <= x1; ++x)
        for (int32_t y = y0; y <= y1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(nodes, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* n = F<uint32_t*>(e, 2); n;) {
                if (Collection_IsValid(nodes, 0, n[0])) {
                    Track_MarkCells((uint8_t*)Collection_Get(nodes, 0, n[0]));
                    n = (uint32_t*)n[1];
                } else {
                    if (n == F<uint32_t*>(e, 2)) F<uint32_t*>(e, 2) = (uint32_t*)n[1];
                    uint32_t* dead = n;
                    n = (uint32_t*)n[1];
                    ((void(__cdecl*)(void*))0x53b080)(dead);
                }
            }
        }
}
RT3_REPLACE(0x492360, Nodes_RemarkAround);

// 0x48c400. Undoes Track_MarkCells (unless the node table is bulk loading): clears
// the fine-grid layer bit (layer 0 for kinds 1..3) and the cell bit (4 for town
// pieces, else 2) at the same six samples; asking the piece whether it is a town
// piece again for each. With `remark`, re-marks the nodes over the samples'
// fine-grid box (the last row is left out).
void __fastcall Track_UnmarkCells(uint8_t* t, int, int32_t remark) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cfca8), 0xed)) return;
    int32_t kind = F<int32_t>(t, 0x226);
    int32_t layer = (kind == 1 || kind == 3 || kind == 2) ? 0 : 1;
    int32_t minX = 99999, maxX = 0, minY = 99999, maxY = 0;
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float cell = *(const float*)0x5c8700;
    float* pts[4] = { a, b, c, d };
    for (float* v : pts) {
        v[0] = (float)((double)v[0] / cell);
        v[2] = (float)((double)v[2] / cell);
    }
    float px[6], py[6];
    LerpPercent(0x14, &px[0], a[0], a[2], b[0], b[2], &py[0]);
    LerpPercent(0x50, &px[1], a[0], a[2], b[0], b[2], &py[1]);
    LerpPercent(0x14, &px[2], c[0], c[2], d[0], d[2], &py[2]);
    LerpPercent(0x50, &px[3], c[0], c[2], d[0], d[2], &py[3]);
    LerpPercent(0x32, &px[4], px[0], py[0], px[3], py[3], &py[4]);
    LerpPercent(0x32, &px[5], px[1], py[1], px[2], py[2], &py[5]);
    for (int32_t k = 0; k < 6; ++k) {
        int32_t xi = Ftol((double)px[k] * Fl(0x5c8568)), yi = Ftol((double)py[k] * Fl(0x5c8568));
        Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, xi, yi, 0, layer, 1);
        if (xi <= minX) minX = xi;
        if (xi >= maxX) maxX = xi;
        if (yi <= minY) minY = yi;
        if (yi >= maxY) maxY = yi;
        int32_t town = (*(int32_t(__fastcall**)(void*))(*(uint8_t**)t + 0x14))(t);
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        int32_t row = Ftol(py[k]);
        uint8_t& g = F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * row + Ftol(px[k])];
        g &= town ? 0xfb : 0xfd;
    }
    if (!remark) return;
    for (int32_t x = minX; x <= maxX; ++x)
        for (int32_t y = minY; y < maxY; ++y) Nodes_RemarkAround(hook::Global<Collection*>(0x6cfca8), 0, x, y);
}
RT3_REPLACE(0x48c400, Track_UnmarkCells);

} // namespace rt3
