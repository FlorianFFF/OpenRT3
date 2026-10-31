// Cargo lot creation / linked territory count / train colour by yearly ratio / image area average colour in blocks 0x46..0x4f (batch 178). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id);
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t);
int32_t __fastcall Region_StationsLinked(uint32_t* o, int, int32_t company);
float __fastcall Obj_YearlyRatio(uint8_t* o);
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
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

// 0x467f50. Creates a cargo lot of the given kind at building `src` bound for
// `dst`: its rate (+0x13) grows with the distance (world factor +0x7d per kind, plus
// k, times the kind's table 0x5f2ad0); counts it in the cargo type's totals.
void __fastcall Lot_Create(uint32_t src, uint32_t dst, int32_t kind) {
    uint8_t lot[0x19];
    memset(lot, 0, sizeof lot);
    F<uint16_t>(lot, 0xa) = 0;
    F<uint32_t>(lot, 4) = 0;
    lot[0x11] = ((const uint8_t*)0x5f2adc)[kind * 4];
    uint8_t flags = lot[0x12] & 0xfc;
    F<uint16_t>(lot, 0xc) = (uint16_t)src;
    F<uint16_t>(lot, 0xe) = (uint16_t)dst;
    lot[0x10] = (uint8_t)kind;
    lot[0x17] = 0;
    lot[0x18] = 0;
    lot[0x12] = flags;
    void* b1 = Collection_Get(hook::Global<Collection*>(0x62b26c), 0, src);
    void* b2 = Collection_Get(hook::Global<Collection*>(0x62b26c), 0, dst);
    int32_t y2 = Ftol(Obj_CellY(b2)), x2 = Ftol(Obj_CellX(b2));
    int32_t y1 = Ftol(Obj_CellY(b1)), x1 = Ftol(Obj_CellX(b1));
    int32_t d = (uint16_t)DistanceInt(x1, y1, x2, y2);
    float k = F<float>(hook::Global<uint8_t*>(0x6cec78), 0x7d + kind * 4);
    float rate = (float)((double)(float)d * k + Fl(0x5c86a0));
    F<float>(lot, 0x13) = (float)((double)rate * ((const float*)0x5f2ad0)[kind]);
    Collection* lots = hook::Global<Collection*>(0x6ada90);
    typedef uint32_t(__thiscall * AddFn)(Collection*, void*, int32_t, int32_t, int32_t);
    uint32_t id = (*(AddFn*)(*(uint8_t**)lots + 4))(lots, lot, 0, 0, 0);
    uint32_t* o = (uint32_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, id);
    o[0] = id;
    Lot_AttachTo(o, 0, src);
    uint8_t* ct = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0,
                                           F<uint32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x8e + kind * 4));
    F<float>(ct, 0x5a) = (float)(F<float>(ct, 0x5a) + D(0x5c8c20));
    F<float>(ct, 0x5e) = (float)(F<float>(ct, 0x5e) + D(0x5c8c20));
    Lot_UpdateLinked((uint8_t*)o, 0, 1);
}
RT3_REPLACE(0x467f50, Lot_Create);

// 0x488110. Number of territory groups (leader +0 / group +0x35) with a station of
// the company linked to another area.
int32_t __fastcall Territories_CountLinked(Collection* c, int, int32_t company) {
    uint8_t seen[0x19];
    memset(seen, 0, sizeof seen);
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint32_t* e = (uint32_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (!Region_StationsLinked(e, 0, company)) continue;
        uint32_t g = F<int32_t>(e, 0x31) ? e[0] : F<uint32_t>(e, 0x35);
        if (seen[g]) continue;
        seen[g] = 1;
        ++n;
    }
    return n;
}
RT3_REPLACE(0x488110, Territories_CountLinked);

// 0x4b28e0. Tints each train by its yearly ratio r (1 - ratio): red-ish channel full
// below 0.5 fading above, the other rising to full at 0.5 (alpha 0x33).
void __fastcall Trains_UpdateRatioColors(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        float r = (float)(D(0x5c8770) - Obj_YearlyRatio(e));
        float a = (float)(r < D(0x5c8628) ? 1.0 : D(0x5c8770) - ((double)r - D(0x5c8628) + ((double)r - D(0x5c8628))));
        float b = (float)(r > D(0x5c8628) ? 1.0 : (double)r + r);
        int32_t ia = Ftol((double)a * Fl(0x5ca0ec));
        int32_t ib = Ftol((double)b * Fl(0x5ca0ec));
        Obj_ForCityChain(e, 0, ib, ia, 0x33);
    }
}
RT3_REPLACE(0x4b28e0, Trains_UpdateRatioColors);

// 0x4f8770. Average colour of the image's current frame (+0x118, or frame +0x1e4,
// or +0x114) around (x, y) within the diamond of radius +0xc4, weighted by
// closeness; into the three byte outputs.
void __fastcall Image_AverageColor(uint8_t* o, int, int32_t x, int32_t y, uint8_t* outR, uint8_t* outG, uint8_t* outB) {
    const uint32_t* px;
    if (F<uint32_t>(o, 0x118)) px = F<const uint32_t*>(o, 0x118);
    else if (F<int32_t>(o, 0x1e4) > 0) px = F<const uint32_t*>(o, 0x118 + F<int32_t>(o, 0x1e4) * 4);
    else px = F<const uint32_t*>(o, 0x114);
    int32_t r = F<int32_t>(o, 0xc4);
    int32_t x0 = x - r < 0 ? 0 : x - r, y0 = y - r < 0 ? 0 : y - r;
    int32_t w = F<int32_t>(o, 0xfc);
    int32_t x1 = w - 1 >= r + x ? r + x : w - 1;
    int32_t y1 = F<int32_t>(o, 0x100) - 1 < r + y ? F<int32_t>(o, 0x100) - 1 : r + y;
    float sr = 0.0f, sg = 0.0f, sb = 0.0f, ws = 0.0f;
    if (y0 <= y1) {
        for (int32_t yy = y0, row = w * y0; yy <= y1; ++yy, row += w) {
            int32_t ady = yy - y < 0 ? y - yy : yy - y;
            for (int32_t xx = x0; xx <= x1; ++xx) {
                int32_t adx = xx - x < 0 ? x - xx : xx - x;
                int32_t dist = adx + ady;
                if (dist > r) continue;
                float wt = (float)(r - dist + 1);
                uint32_t p = px[xx + row];
                ws = (float)((double)wt + ws);
                float cr = (float)(int32_t)((p >> 16) & 0xff);
                sr = (float)((double)cr * wt + sr);
                float cg = (float)(int32_t)((p >> 8) & 0xff);
                sg = (float)((double)cg * wt + sg);
                float cb = (float)(int32_t)(p & 0xff);
                sb = (float)((double)cb * wt + sb);
            }
        }
        if (ws <= Fl(0x5c8598)) ws = 0.001f;
    } else ws = 0.001f;
    *outR = (uint8_t)Ftol((double)sr / ws);
    *outG = (uint8_t)Ftol((double)sg / ws);
    *outB = (uint8_t)Ftol((double)sb / ws);
}
RT3_REPLACE(0x4f8770, Image_AverageColor);

} // namespace rt3
