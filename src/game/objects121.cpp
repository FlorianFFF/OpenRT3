// Daylight curve / site flatness / attachment offsets / car skins in blocks 0x40..0x41 (batch 124). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y);
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

// 0x409d00. Daylight factor for an hour of the day (mod 24): a piecewise-linear
// curve steeper by 2.2 (1 in arctic maps, world +0x4aeb), mirrored after midday.
float __stdcall Daylight_Factor(float hour) {
    float h = (float)((double(__cdecl*)(double, double))0x5a1390)(hour, 24.0);
    float slope = F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aeb) ? 1.0f : 2.2f;
    if (h < Fl(0x5c8788)) {
        float v = (float)((double)slope * h);
        if (v < D(0x5c8980)) return v;
        float knee = (float)(D(0x5c8980) / slope);
        float s2 = (float)(D(0x5c8978) - knee);
        s2 = (float)(D(0x5c8970) / s2);
        return (float)(((double)h - knee) * s2 + D(0x5c8980));
    }
    h = (float)((double)Fl(0x5c8968) - h);
    float v = (float)((double)h * slope);
    if (v < D(0x5c8960)) {
        h = v;
    } else {
        float knee = (float)(D(0x5c8960) / slope);
        float s2 = (float)(D(0x5c8978) - knee);
        s2 = (float)(D(0x5c8958) / s2);
        h = (float)(((double)h - knee) * s2 + D(0x5c8960));
    }
    return (float)((double)Fl(0x5c8968) - h);
}
RT3_REPLACE(0x409d00, Daylight_Factor);

// 0x4153f0. Whether the 8x8 footprint (mask 0x5edd30 for `shape`) around pos is
// buildable and flat enough: every masked cell free of features and objects
// (0x62b300), and the height range within 1 (5 without regions) x 0x5c9680.
int32_t __stdcall Site_FootprintFlat(const int32_t* pos, int32_t shape) {
    float lo = 1000000.0f, hi = -1000000.0f;
    for (int32_t j = 0; j < 8; ++j) {
        uint32_t y = (uint32_t)(pos[2] + j - 3);
        if (y >= (uint32_t)(F<int32_t>(Map(), 0x2159) - 1)) return 0;
        for (int32_t i = 0; i < 8; ++i) {
            uint32_t x = (uint32_t)(pos[0] + i - 3);
            int32_t w = F<int32_t>(Map(), 0x2155);
            if (x >= (uint32_t)(w - 1)) return 0;
            if (((const uint8_t*)0x5edd30)[(j + shape * 8) * 8 + i]) {
                uint32_t* occ = hook::Global<uint32_t*>(0x62b300);
                uint32_t o = occ ? occ[w * (int32_t)y + (int32_t)x] : 0;
                if (Map_CellFlag(Map(), 0, (int32_t)x * 4, (int32_t)y * 4, 0)) return 0;
                if (Map_NearFeature(Map(), 0, (int32_t)x, (int32_t)y)) return 0;
                if (o) return 0;
            }
            float h = (float)Map_GridFloat(Map(), 0, (int32_t)x, (int32_t)y);
            if (lo > h) lo = h;
            if (hi < h) hi = h;
        }
    }
    float k = F<int32_t>(hook::Global<uint8_t*>(0x62bae0), 0x88) > 0 ? 1.0f : 5.0f;
    double limit = (double)k * D(0x5c9680);
    return !((double)hi - lo > limit) ? 1 : 0;
}
RT3_REPLACE(0x4153f0, Site_FootprintFlat);

// 0x417910. Moves (*x, *z) to the attachment point of object `id`: its cell
// centre rotated by `angle`, then back (mode 0) or forward along the heading.
void __fastcall Obj_AttachmentPoint(Collection* c, int, int32_t mode, uint32_t id, float* x, float* z, float angle) {
    uint8_t* o = (uint8_t*)Collection_Get(c, 0, id);
    uint16_t cx, cy;
    Obj_GetCell(o, 0, &cx, &cy);
    const double k = D(0x5c96c8), two = D(0x5c85d0);
    float py = (float)((double)(int16_t)cy / k - (double)o[0xb9] / two);
    float px = (float)((double)(int16_t)cx / k - (double)o[0xb8] / two);
    RotateNegated(px, py, angle, x, z);
    auto sinf_ = (double(__cdecl*)(double))0x5a1494;
    auto cosf_ = (double(__cdecl*)(double))0x5a13e4;
    if (!mode) {
        double t = *x;
        *x = (float)(t - sinf_(angle) * D(0x5c96d8));
        t = *z;
        *z = (float)(t - cosf_(angle) * D(0x5c96d8));
        return;
    }
    *x = (float)(sinf_(angle) * D(0x5c96d0) + *x);
    *z = (float)(cosf_(angle) * D(0x5c96d0) + *z);
}
RT3_REPLACE(0x417910, Obj_AttachmentPoint);

// 0x41c060. The car's skin texture: a named livery (+0x3ac, while +0x3cc) from
// its type's list (+0x196 / +0x19a), else the type's base skin (+0x3d) shifted by
// 4 per cargo load level (cargo +0x20, 1..8).
RT3_NOINLINE uint32_t __fastcall Car_SkinTexture(uint8_t* car) {
    Collection* types = hook::Global<Collection*>(0x62ba88);
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    if (car[0x3cc]) {
        for (int32_t i = 0; i < Collection_Count(F<Collection*>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x196)); ++i) {
            Collection* names = F<Collection*>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x196);
            const char* name = (const char*)Collection_Get(names, 0, Collection_IdAt(names, 0, i, 0));
            if (!Stricmp((const char*)car + 0x3ac, name)) {
                uint8_t* t = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(car, 0x388));
                return Table_Entry(F<uint8_t*>(t, 0x19a + i * 4), 0, 0);
            }
        }
        car[0x3cc] = 0;
    }
    int32_t idx = F<int32_t>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x3d);
    if (F<uint32_t>(car, 0x38c) && Collection_Get(cargos, 0, F<uint32_t>(car, 0x38c))) {
        int8_t lvl = (int8_t)((uint8_t*)Collection_Get(cargos, 0, F<uint32_t>(car, 0x38c)))[0x20];
        if (lvl > 0 && lvl <= 8) idx += lvl * 4;
    }
    uint8_t* t = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(car, 0x388));
    return Table_Entry(F<uint8_t*>(t, 0x12a), 0, idx);
}
RT3_REPLACE(0x41c060, Car_SkinTexture);

} // namespace rt3
