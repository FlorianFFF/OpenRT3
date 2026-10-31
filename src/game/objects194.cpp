// Larger game functions, part 2 (batch 200). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Record_EncodeTags(uint8_t* r);
void __fastcall EventRules_Save(uint8_t* o, int, Stream st);

// 0x433060. Saves the scenario rule sets: tag 0x4e99, version 1001, the collection
// (texts tag-encoded first), tag 0x4e9a, every set's rules, tag 0x4e9b.
void __fastcall RuleSets_Save(Collection* c, int, Stream st, int32_t recordSize) {
    StreamWriteU32(st, 0x4e99);
    int32_t version = 0x3e9;
    StreamWrite(st, &version, 4, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Record_EncodeTags((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
    Collection_Save(c, 0, st, recordSize);
    StreamWriteU32(st, 0x4e9a);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        EventRules_Save((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, st);
    StreamWriteU32(st, 0x4e9b);
}
RT3_REPLACE(0x433060, RuleSets_Save);

float __stdcall DarknessOf(uint8_t b);
int32_t __stdcall BrightnessOf(float f);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);

// 0x44a9a0. Paints the four ground-texture layers (+0x1631, one byte per vertex) at
// vertex (x, y) (not on the map edge) with strength s: mode 3 fades all (to 0 under
// a floor), mode 2 boosts all (to 1 at the cap), mode 0 blends them toward `w`;
// mode 1 picks one layer by a hashed random draw over the shares `p` (the 4th by its
// own draw) and blends it toward its `w` (all layers with `p` summing >= 1).
void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w,
                                    const float* p) {
    int32_t W = F<int32_t>(t, 0x15d9);
    int32_t idx = (W + 1) * y + x;
    if (x <= 0 || y <= 0 || x >= W - 1 || y >= F<int32_t>(t, 0x15d5) - 1) return;
    auto layer = [&](int32_t j) { return F<uint8_t*>(t, 0x1631 + j * 4) + idx; };
    if (mode == 3) {
        double k = D(0x5c8770) - s;
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            v = (float)(v * k);
            *q = (uint8_t)BrightnessOf(v > D(0x5c9b80) || v != v ? v : 0.0f);
        }
        return;
    }
    if (mode == 2) {
        double k = (double)s + D(0x5c8770);
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            v = (float)(v * k);
            *q = (uint8_t)BrightnessOf(v < D(0x5c8650) || v != v ? v : 1.0f);
        }
        return;
    }
    if (mode == 0) {
        double k = D(0x5c8770) - s;
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            *q = (uint8_t)BrightnessOf((float)((double)s * w[j] + v * k));
        }
        return;
    }
    if (mode != 1) return;
    float sum = (float)((double)p[0] + p[1]);
    sum = (float)((double)sum + p[2]);
    int32_t all = sum < D(0x5caa68) || sum != sum ? 0 : 1;
    int32_t range = (x * 0x8d + y * 7) % 1000 + 1000;
    if (Fl(0x5c85b4) > sum) sum = 1.0f;
    float rnd = (float)HashRand(y * 0x8dad + x * 0xaf1, 0, range);
    float draw = (float)((double)rnd / (float)range * sum);
    int32_t done = 0;
    for (int32_t j = 0; j < 4; ++j) {
        float cur = DarknessOf(*layer(j));
        float target = 0.0f;
        if (j == 3) {
            if (!(p[3] > D(0x5c85b8)) && p[3] == p[3] && !all) continue;
            double r = (double)HashRand(x * 0x1c13 + y * 0x3b, 0, 0x7d0) / D(0x5caa60);
            if (p[3] > r) target = w[3];
        } else if (!done) {
            draw = (float)((double)draw - p[j]);
            if (!(draw > Fl(0x5c8598)) && draw == draw) {
                target = w[j];
                done = 1;
            }
        }
        if (!(target > D(0x5c85b8)) && target == target && !all) continue;
        *layer(j) = (uint8_t)BrightnessOf((float)((D(0x5c8770) - s) * cur + (double)target * s));
    }
}
RT3_REPLACE(0x44a9a0, Terrain_PaintLayers);

int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);

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

// 0x44fb70. Builds the route-planner costs: for every 4x4 block record (0x1d7
// bytes, +0x2129, W x H blocks +0x2145 / +0x2149) and each of the 8 directions
// (0x624b28 / 0x624b48) inside the map (+0x2155 / +0x2159), walks the 4 cells from
// the block centre: each step costs (1 + slope^2 / k) x a base (0.09, 0.072 into
// blocks flagged +0xe6 & 8, x0.8 out of them; 0.4 on water), diagonals scaled, plus
// a shore penalty; the total is scaled by the blocks' +0xe6 & 2/1 flags and stored
// as u16 (max 65000) in the record's +0xf3 table.
void __fastcall Routes_BuildCosts(uint8_t* t) {
    const int32_t* dxs = (const int32_t*)0x624b28;
    const int32_t* dys = (const int32_t*)0x624b48;
    for (int32_t by = 0, cy = 2; by < F<int32_t>(t, 0x2149); ++by, cy += 4) {
        for (int32_t bx = 0, cx = 2; bx < F<int32_t>(t, 0x2145); ++bx, cx += 4) {
            uint8_t* rec = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * by + bx) * 0x1d7;
            uint16_t* slot = (uint16_t*)(rec + 0xf3);
            for (int32_t dir = 0; dir < 8; ++dir, ++slot) {
                int32_t ddx = dxs[dir], ddy = dys[dir];
                if (cx + ddx * 4 < 0 || cx + ddx * 4 >= F<int32_t>(t, 0x2155)) continue;
                if (cy + ddy * 4 < 0 || cy + ddy * 4 >= F<int32_t>(t, 0x2159)) continue;
                uint8_t* nrec = F<uint8_t*>(t, 0x2129) + ((ddy + by) * F<int32_t>(t, 0x2145) + ddx + bx) * 0x1d7;
                float sum = 0.0f;
                int32_t x = cx, y = cy;
                int32_t water0 = Terrain_KindIs1Or4(t, 0, cx, cy) == 0;
                float base = nrec[0xe6] & 8 ? 0.072f : 0.09f;
                if (rec[0xe6] & 8) base = (float)(base * D(0x5c8640));
                int32_t diag = dir & 1;
                for (int32_t step = 0; step < 4; ++step) {
                    int32_t nx = dxs[dir] + x, ny = dys[dir] + y;
                    int32_t water = Terrain_KindIs1Or4(t, 0, nx, ny) == 0;
                    Terrain_KindIs1Or4(t, 0, x, y);
                    float hn = (float)Map_GridFloat(t, 0, nx, ny);
                    float d = (float)((double)hn - Map_GridFloat(t, 0, x, y));
                    float k = (float)(water | water0 ? 0.4 : (double)base);
                    if (diag) {
                        d = (float)(d / D(0x5caab0));
                        k = (float)(k * D(0x5caab0));
                    }
                    d = (float)(d / D(0x5c8798));
                    if (d < Fl(0x5c8598)) d = (float)(((double(__cdecl*)(double))0x5a152e)(d) / D(0x5c85c8));
                    d = (float)((double)d * d);
                    sum = (float)(((double)d + D(0x5c8698)) / D(0x5c8698) * k + sum);
                    if (water0 != water) sum = (float)(sum + D(0x5c8640));
                    x = nx;
                    y = ny;
                }
                uint8_t fa = rec[0xe6], fb = nrec[0xe6];
                int32_t n = (fa & 2 ? 1 : 0) + (fb & 2 ? 1 : 0);
                if (n == 0) sum = (float)(sum * D(0x5c87f0));
                else if (n == 1) {
                    if (!(fa & 1) && !(fb & 1)) sum = (float)(sum * D(0x5c8578));
                } else sum = (float)(sum * D(0x5c88e8));
                float v = (float)(sum * D(0x5c9a00));
                if (v > Fl(0x5caaa8)) v = 65000.0f;
                *slot = (uint16_t)Ftol(v);
            }
        }
    }
}
RT3_REPLACE(0x44fb70, Routes_BuildCosts);

// 0x539fb0. Emits a box model as triangle-list vertices (0x18 bytes each) into `out`,
// counting them in *count: the 16 corner vertices (+0x63) are first tinted (+0x5f
// when +0x59 & 0xf, else `color` keeping their alpha); then the sides (+0x59 & 0x10,
// 18 vertices), the top unless `noTop` (12) and the roof from +0x1e3 (kind +0x44:
// 1 one face, 2/3 two).
void __fastcall Box_EmitVertices(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t color, int32_t noTop) {
    for (int32_t i = 0; i < 16; ++i) {
        uint32_t& c = F<uint32_t>(o, 0x6f + i * 0x18);
        c = o[0x59] & 0xf ? F<uint32_t>(o, 0x5f) : (c & 0xff000000) | color;
    }
    auto emit = [&](const uint8_t* src) {
        memcpy(out, src, 0x18);
        out += 0x18;
    };
    if (o[0x59] & 0x10) {
        static const uint16_t sides[18] = { 0x7b, 0xf3, 0xdb, 0x7b, 0x93, 0xf3, 0x63, 0xc3, 0xdb,
                                            0x63, 0xdb, 0x7b, 0x93, 0xf3, 0x10b, 0x93, 0x10b, 0xab };
        for (uint16_t v : sides) emit(o + v);
        *count += 0x12;
    }
    if (!noTop) {
        static const uint16_t top[12] = { 0x123, 0x153, 0x16b, 0x123, 0x16b, 0x13b, 0x183, 0x1b3, 0x1cb, 0x183, 0x1cb, 0x19b };
        for (uint16_t v : top) emit(o + v);
        *count += 0xc;
    }
    uint8_t kind = o[0x44];
    if (kind == 0 || kind > 3) return;
    if (kind != 1) {
        static const uint8_t back[6] = { 0x60, 0x78, 0x90, 0xa8, 0x90, 0x78 };
        for (uint8_t v : back) emit(F<uint8_t*>(o, 0x1e3) + v);
        *count += 6;
    }
    static const uint8_t front[6] = { 0, 0x18, 0x30, 0x48, 0x30, 0x18 };
    for (uint8_t v : front) emit(F<uint8_t*>(o, 0x1e3) + v);
    *count += 6;
}
RT3_REPLACE(0x539fb0, Box_EmitVertices);

} // namespace rt3
