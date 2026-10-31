// Airplane table ctor / terrain texture extract, ctor, vertex normals, quads / fog params / light modes in blocks 0x40..0x53 (batch 138). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include <stdint.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Airplanes_LoadTypes(uint8_t* o);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void __fastcall Terrain_Init(uint8_t* t);
float* __fastcall Terrain_NormalAt(uint8_t* t, int, float* out, float x, float z);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x40aeb0. Constructs the airplane type table (a collection, vtable 0x5c8a0c) and
// loads it.
uint8_t* __fastcall Airplanes_Construct(uint8_t* o) {
    Collection_SetVtbl((Collection*)o);
    F<uint32_t>(o, 0) = 0x5c8a0c;
    Collection_Init((Collection*)o, 0, 0, 0, 5, 10, 0, 0, 0);
    for (int k = 0; k < 0x2e0; ++k) F<uint32_t>(o, 0x90 + k * 4) = 0;
    F<int32_t>(o, 0x8c) = 9999;
    F<int32_t>(o, 0x88) = 0;
    Airplanes_LoadTypes(o);
    return o;
}
RT3_REPLACE(0x40aeb0, Airplanes_Construct);

// 0x534930. Once (+0x1588): copies the terrain texture area (x0, y0)-(x1, y1) (map
// fractions; tiles +8 of +0x157c x +0x1580 pixels, 4 per row) into a new pixel
// buffer (+0x1584, origin +0x1589/+0x158d, end +0x1591/+0x1595); 0 if done before.
// Quirk: x0 scales by the tile width, the others by the tile height.
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW,
                                            int32_t* outH) {
    if (t[0x1588]) return nullptr;
    uint32_t* cache[0xff] = {};
    int32_t tw = F<int32_t>(t, 0x157c) * 4, th = F<int32_t>(t, 0x1580) * 4;
    t[0x1588] = 1;
    int32_t a = (int32_t)((double)tw * x0);
    F<int32_t>(t, 0x1589) = a;
    int32_t b = (int32_t)((double)y0 * th);
    F<int32_t>(t, 0x158d) = b;
    int32_t c = (int32_t)((double)x1 * th);
    F<int32_t>(t, 0x1591) = c;
    int32_t d = (int32_t)((double)th * y1);
    F<int32_t>(t, 0x1595) = d;
    int32_t w = c - a, h = d - b;
    uint32_t* buf = (uint32_t*)Malloc(h * w * 4);
    F<uint32_t*>(t, 0x1584) = buf;
    uint32_t* dst = buf;
    for (int32_t row = F<int32_t>(t, 0x158d); row < F<int32_t>(t, 0x158d) + h; ++row) {
        int32_t q = row / F<int32_t>(t, 0x1580);
        int32_t base = F<int32_t>(t, 0x157c) * (row % F<int32_t>(t, 0x1580));
        for (int32_t col = F<int32_t>(t, 0x1589); col < F<int32_t>(t, 0x1589) + w; ++col) {
            int32_t idx = col / F<int32_t>(t, 0x157c) + q * 4 + 1;
            if (!cache[idx]) cache[idx] = Texture_Pixels32(F<uint8_t*>(t, 8 + idx * 4), 0, nullptr, nullptr, 0);
            *dst++ = cache[idx][col % F<int32_t>(t, 0x157c) + base];
        }
    }
    if (outW) *outW = w;
    if (outH) *outH = h;
    return F<uint32_t*>(t, 0x1584);
}
RT3_REPLACE(0x534930, Terrain_ExtractTexture);

// 0x534f60. Constructs the terrain renderer (vtable 0x5dd25c).
uint8_t* __fastcall Terrain_Construct(uint8_t* t) {
    F<uint32_t>(t, 0) = 0x5dd25c;
    F<int32_t>(t, 0x154c) = 0;
    Terrain_Init(t);
    return t;
}
RT3_REPLACE(0x534f60, Terrain_Construct);

// 0x536230. Vertex normal (+0x15f1, 12 bytes per vertex) at (x, y): the normalised
// mean of the surface normals at the vertices around it. Returns 0.
float __fastcall Terrain_ComputeVertexNormal(uint8_t* t, int, int32_t x, int32_t y) {
    int32_t x0 = x - 1 < 0 ? 0 : x - 1;
    int32_t y0 = y - 1 < 0 ? 0 : y - 1;
    int32_t x1 = F<int32_t>(t, 0x15d9) - 1 < x + 1 ? F<int32_t>(t, 0x15d9) - 1 : x + 1;
    int32_t y1 = F<int32_t>(t, 0x15d5) - 1 < y + 1 ? F<int32_t>(t, 0x15d5) - 1 : y + 1;
    int32_t count = 0;
    float sx = 0.0f, sy = 0.0f, sz = 0.0f;
    if (y0 > y1) return 0.0f;
    for (int32_t yy = y0; yy <= y1; ++yy) {
        float fz = (float)(yy * 0x5c);
        if (x0 > x1) continue;
        count += x1 - x0 + 1;
        for (int32_t xx = x0; xx <= x1; ++xx) {
            float n[3];
            float* r = Terrain_NormalAt(t, 0, n, (float)(xx * 0x5c), fz);
            sx = (float)((double)sx + r[0]);
            sy = (float)((double)sy + r[1]);
            sz = (float)((double)sz + r[2]);
        }
    }
    if (!count) return 0.0f;
    double inv = D(0x5c8770) / count;
    float invf = (float)inv;
    double a = inv * sx, b = (double)invf * sy;
    double c = (double)invf * sz;
    sz = (float)c;
    double len = sqrt(c * sz + b * b + a * a);
    if (Fl(0x5c8598) < len) {
        double k = Fl(0x5c85b4) / len;
        a *= k;
        b *= k;
        sz = (float)(k * sz);
    }
    float* out = (float*)(F<uint8_t*>(t, 0x15f1) + ((F<int32_t>(t, 0x15d9) + 1) * y + x) * 12);
    out[2] = sz;
    out[0] = (float)a;
    out[1] = (float)b;
    return 0.0f;
}
RT3_REPLACE(0x536230, Terrain_ComputeVertexNormal);

// One terrain overlay quad (0x31 bytes): 4 corners (x, y, z) and a raise over the ground.
struct Quad {
    float c[12];
    uint8_t raise;
};

// Puts the raised quad's corners on the terrain.
static void DropQuad(uint8_t* t, Quad* q) {
    float r = (float)q->raise;
    for (int k = 0; k < 4; ++k)
        q->c[k * 3 + 1] = (float)(Terrain_HeightAt(t, 0, q->c[k * 3], q->c[k * 3 + 2]) + r);
}

// 0x536ea0. Adds an overlay quad with corners (x, y, z) x 4 (laid on the ground +
// raise when raise != 0) to the per-16-cell block list (+0x167d) of its centre.
PtrListNode* __fastcall Terrain_AddQuad(uint8_t* t, int, float x0, float y0, float z0, float x1, float y1, float z1,
                                        float x2, float y2, float z2, float x3, float y3, float z3, uint8_t raise) {
    Quad* q = (Quad*)Malloc(0x31);
    const float in[12] = { x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3 };
    for (int k = 0; k < 12; ++k) q->c[k] = in[k];
    q->raise = raise;
    if (raise) DropQuad(t, q);
    int32_t sx, sz;
    __asm {
        fld x3
        fadd x2
        fadd x1
        fadd x0
        mov eax, 0x5a10d0
        call eax
        mov sx, eax
        fld z3
        fadd z2
        fadd z1
        fadd z0
        mov eax, 0x5a10d0
        call eax
        mov sz, eax
    }
    int32_t bx = ((sx >> 2) / 92) >> 4;
    int32_t bz = ((sz >> 2) / 92) >> 4;
    PtrList* l = F<PtrList**>(t, 0x167d)[(F<int32_t>(t, 0x15d9) >> 4) * bz + bx];
    return PtrList_Append(l, 0, q);
}
RT3_REPLACE(0x536ea0, Terrain_AddQuad);

// 0x537010. Re-lays the raised overlay quads of the blocks covering cells
// (x0, y0)-(x1, y1) on the terrain.
void __fastcall Terrain_RefreshQuads(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    if (F<int32_t>(t, 0x15d9) - 2 < x1) x1 = F<int32_t>(t, 0x15d9) - 2;
    if (F<int32_t>(t, 0x15d5) - 2 < y1) y1 = F<int32_t>(t, 0x15d5) - 2;
    y1 >>= 4;
    y0 >>= 4;
    x0 >>= 4;
    x1 >>= 4;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx) {
            PtrList* l = F<PtrList**>(t, 0x167d)[(F<int32_t>(t, 0x15d9) >> 4) * by + bx];
            PtrList_Rewind(l);
            for (Quad* q = (Quad*)PtrList_Next(l); q; q = (Quad*)PtrList_Next(l))
                if (q->raise) DropQuad(t, q);
        }
}
RT3_REPLACE(0x537010, Terrain_RefreshQuads);

// 0x5371f0. Fog/haze for the point (x, z): a strength from the camera distance
// (sqrt(300 / distance), or `bias` itself / scaled by -bias), clamped to
// 0.25..1.25; *color = engine +0x1141f5. Weak values (< 0.3667) or option
// 0x6d4034 use the fixed haze (engine +0x1141f9, 0.3667, scale 1).
RT3_NOINLINE void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color,
                             float* strength) {
    float cy, cx, cz;
    Obj_GetXYZ1c(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, (uint32_t*)&cx, (uint32_t*)&cy,
                 (uint32_t*)&cz);
    double dx = (double)x - cx, dz = (double)z - cz;
    double len = sqrt(dz * dz + dx * dx + (double)cy * cy);
    if (Fl(0x5c85b4) > len) len = Fl(0x5c85b4);
    double v = sqrt(Fl(0x5c87c0) / len);
    if (bias < Fl(0x5c8598)) v = -(double)bias * v;
    else if (bias > Fl(0x5c8598)) v = bias;
    if (v < D(0x5c8758)) v = D(0x5c8758);
    if (v > D(0x5c87e8)) v = D(0x5c87e8);
    float s = (float)v;
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    *color = F<uint32_t>(e, 0x1141f5);
    *strength = s;
    if ((double)s < D(0x5dd2f8) || hook::Global<int32_t>(0x6d4034)) {
        *(uint32_t*)strength = 0x3ebbbbbc;
        *color = F<uint32_t>(hook::Global<uint8_t*>(0x6d4024), 0x1141f9);
        *scale = 1.0f;
        return;
    }
    *scale = s;
}
RT3_REPLACE(0x5371f0, Fog_ParamsAt);

// 0x539230. Sets the light's colours (+0x73..+0x11f) for mode 0..3 (+0x5a).
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode) {
    const uint32_t a65 = 0x3f266666, a85 = 0x3f59999a, half = 0x3f000000, one = 0x3f800000;
    F<uint32_t>(o, 0xeb) = a65;
    F<uint32_t>(o, 0x8b) = a65;
    F<uint32_t>(o, 0x103) = a85;
    F<uint32_t>(o, 0xa3) = a85;
    o[0x5a] = mode;
    F<uint32_t>(o, 0xd3) = half;
    F<uint32_t>(o, 0x73) = half;
    F<uint32_t>(o, 0x11b) = one;
    F<uint32_t>(o, 0xbb) = one;
    F<uint32_t>(o, 0xbf) = half;
    F<uint32_t>(o, 0xa7) = half;
    F<uint32_t>(o, 0x8f) = half;
    F<uint32_t>(o, 0x77) = half;
    F<uint32_t>(o, 0x11f) = one;
    F<uint32_t>(o, 0x107) = one;
    F<uint32_t>(o, 0xef) = one;
    F<uint32_t>(o, 0xd7) = one;
    static const uint16_t lo[4] = { 0x77, 0x8f, 0xa7, 0xbf }, hi[4] = { 0xd7, 0xef, 0x107, 0x11f };
    if (mode == 2) {
        for (int k = 0; k < 4; ++k) {
            F<uint32_t>(o, lo[k]) = 0;
            F<uint32_t>(o, hi[k]) = half;
        }
        F<uint32_t>(o, 0x8b) = 0x3f333333;
        F<uint32_t>(o, 0xa3) = 0x3f4ccccd;
    } else if (mode == 3) {
        for (int k = 0; k < 4; ++k) {
            F<uint32_t>(o, lo[k]) = half;
            F<uint32_t>(o, hi[k]) = 0;
        }
        F<uint32_t>(o, 0xeb) = 0x3f333333;
        F<uint32_t>(o, 0x103) = 0x3f4ccccd;
    } else if (mode == 1) {
        F<uint32_t>(o, 0xbb) = half;
        F<uint32_t>(o, 0x11b) = half;
        F<uint32_t>(o, 0x8b) = 0x3e4ccccb;
        F<uint32_t>(o, 0xeb) = 0x3e4ccccb;
        F<uint32_t>(o, 0x73) = 0;
        F<uint32_t>(o, 0xd3) = 0;
        F<uint32_t>(o, 0xa3) = 0x3e99999a;
        F<uint32_t>(o, 0x103) = 0x3e99999a;
    }
}
RT3_REPLACE(0x539230, Light_SetMode);

} // namespace rt3
