// Group placement, particle billboards and quad-grid seams in blocks 0x53..0x56
// (batch 197). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename T> static inline T& G(uint32_t a) { return *(T*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

void __fastcall Group_MoveAllTo(uint8_t* g, int, float x, float y, float z);

// 0x53a5b0. Moves the group's objects to its point (+0xc, +0x10) at height y.
void __fastcall Group_MoveAllToHeight(uint8_t* g, int, float y) {
    Group_MoveAllTo(g, 0, F<float>(g, 0xc), y, F<float>(g, 0x10));
}
RT3_REPLACE(0x53a5b0, Group_MoveAllToHeight);

void __fastcall Obj_GetXY10(uint8_t* o, int, uint32_t* a, uint32_t* b);

// 0x566670. Per-frame particle setup from the camera's angles: stores its forward
// axis (0xd97b98), the billboard corner offsets (0xd97abc..0xd97ae8, half-size
// 0x5c9b08 along its right / up axes) and steps the wind gusts: every 32..1024
// frames (counter 0xd97aec, period 0x625bdc) the gust 0x625bb0 may reverse, and the
// wind strength 0x625bac is pushed by it up to 0x625bb4 / down to 0xd97af0.
void __fastcall Particles_FrameSetup(uint8_t* cam) {
    float yaw, pitch;
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    float r[16];
    Obj_GetXY10(cam, 0, (uint32_t*)&yaw, (uint32_t*)&pitch);
    auto mul = (void*(__stdcall*)(float*, const float*, const float*))0x56d3cd;
    ((void*(__stdcall*)(float*, float))0x56dda8)(r, -pitch);
    mul(m, m, r);
    ((void*(__stdcall*)(float*, float))0x56de44)(r, yaw);
    mul(m, m, r);
    const double z = Fl(0x5c8598), k = Fl(0x5c9b08);
    double a = m[8] * z;
    G<uint32_t>(0xd97b98) = ((uint32_t*)m)[8];
    G<uint32_t>(0xd97b9c) = ((uint32_t*)m)[9];
    G<uint32_t>(0xd97ba0) = ((uint32_t*)m)[10];
    double b = m[4] * k + a;
    double t = m[0] * k;
    float rx = (float)t;
    G<float>(0xd97ae0) = (float)(t + b);
    float l4 = (float)(m[9] * z);
    double c = m[5] * k + l4;
    t = m[1] * k;
    float ry = (float)t;
    G<float>(0xd97ac4) = (float)(t + c);
    float l0 = (float)(m[10] * z);
    float l8 = (float)(m[6] * k + l0);
    int32_t counter = G<int32_t>(0xd97aec);
    t = m[2] * k;
    float rz = (float)t;
    G<float>(0xd97ae8) = (float)(t + l8);
    G<float>(0xd97ad4) = (float)(m[0] + b);
    G<float>(0xd97acc) = (float)(m[1] + c);
    G<float>(0xd97ad8) = (float)(l8 + (double)m[2]);
    t = m[4] + a;
    l8 = (float)t;
    G<float>(0xd97ad0) = (float)(t + m[0]);
    t = (double)m[5] + l4;
    l4 = (float)t;
    G<float>(0xd97adc) = (float)(t + m[1]);
    t = (double)m[6] + l0;
    l0 = (float)t;
    G<float>(0xd97ac8) = (float)(t + m[2]);
    G<float>(0xd97ac0) = (float)((double)l8 + rx);
    G<float>(0xd97ae4) = (float)((double)l4 + ry);
    G<float>(0xd97abc) = (float)((double)l0 + rz);
    if (counter < 0) {
        uint32_t& seed = G<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        int32_t n = (int32_t)(seed & 0x7fff) % G<int32_t>(0x625bdc);
        G<int32_t>(0xd97aec) = n;
        if (n < 0x20) {
            G<int32_t>(0x625bdc) = 0x400;
            G<float>(0x625bb0) = -G<float>(0x625bb0);
        }
    } else {
        if (G<int32_t>(0x625bdc) > 0x20) --G<int32_t>(0x625bdc);
        --G<int32_t>(0xd97aec);
    }
    float gust = G<float>(0x625bb0);
    if ((gust > z && G<float>(0x625bac) < G<float>(0x625bb4)) || (gust < z && G<float>(0x625bac) > G<float>(0xd97af0)))
        G<float>(0x625bac) = (float)((double)G<float>(0x625bac) + G<float>(0x625bb0));
}
RT3_REPLACE(0x566670, Particles_FrameSetup);

void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Quad_FindJoin(uint8_t* a, uint8_t* b, uint8_t** outA, uint8_t** outB, float* best);

// 0x567730. Welds the seams of the quad grid (+0x1669, (+0x15d9 + 1) x +0x15d5
// cells) within (x0, y0)..(x1, y1): for each quad, the closest corner pair with a
// neighbour (Quad_FindJoin, under 71.3) is moved to its midpoint on the terrain
// (+ a small lift), and the scan backs up one cell to re-check; at most 32000 welds.
void __fastcall QuadGrid_Weld(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 > 0 ? x0 : 0;
    if (x1 >= F<int32_t>(t, 0x15d9)) x1 = F<int32_t>(t, 0x15d9);
    y0 = y0 > 0 ? y0 : 0;
    if (y1 >= F<int32_t>(t, 0x15d5)) y1 = F<int32_t>(t, 0x15d5);
    int32_t budget = 0x7d00;
    auto cellAt = [&](int32_t x, int32_t y) { return F<uint8_t**>(t, 0x1669)[(F<int32_t>(t, 0x15d9) + 1) * y + x]; };
    for (int32_t y = y0; y < y1; ++y) {
        for (int32_t x = x0; x < x1; ++x) {
            uint8_t* cell = cellAt(x, y);
            while (cell) {
                int32_t xm = x - 1 > 0 ? x - 1 : 0;
                int32_t xp = x + 1 < F<int32_t>(t, 0x15d9) ? x + 1 : F<int32_t>(t, 0x15d9);
                int32_t ym = y - 1 > 0 ? y - 1 : 0;
                int32_t yp = y + 1 < F<int32_t>(t, 0x15d5) ? y + 1 : F<int32_t>(t, 0x15d5);
                uint8_t* a = nullptr;
                uint8_t* b = nullptr;
                float best = 71.3f;
                if (ym > yp) break;
                for (int32_t j = ym; j <= yp; ++j)
                    for (int32_t i = xm; i <= xp; ++i) {
                        if (i == x && j == y) continue;
                        if (uint8_t* nb = cellAt(i, j)) Quad_FindJoin(cell, nb, &a, &b, &best);
                    }
                if (!a) break;
                uint8_t* grid = (uint8_t*)Engine_Get1C(*(void**)0x6d4024);
                double mx = ((double)F<float>(b, 0) + F<float>(a, 0)) * *(const double*)0x5c8628;
                float fx = (float)mx;
                double mz = ((double)F<float>(a, 8) + F<float>(b, 8)) * *(const double*)0x5c8628;
                float fz = (float)mz;
                F<float>(b, 0) = (float)mx;
                F<float>(a, 0) = (float)mx;
                F<float>(b, 8) = (float)mz;
                F<float>(a, 8) = (float)mz;
                double h = Terrain_HeightAt(grid, 0, fx, fz) + *(const double*)0x5c8d68;
                F<float>(b, 4) = (float)h;
                F<float>(a, 4) = (float)h;
                if (!budget) break;
                y = y0 > y - 1 ? y0 : y - 1;
                x = x0 > x - 1 ? x0 : x - 1;
                --budget;
                cell = cellAt(x, y);
            }
        }
    }
}
RT3_REPLACE(0x567730, QuadGrid_Weld);

// 0x5679b0. Expands a terrain quad (corners 0/+0xc/+0x18/+0x24, raised by `lift`)
// into two triangles (6 vertices of x, y, z, colour, u, v). The texture cell steps
// through an 8x8 atlas every 88 ms (GetTickCount + phase +0x3a); the colour is the
// tint 0xd97bbc with red/green darkened by each corner's shade (+0x35..), corners
// pinned white by the edge kind +0x30, alpha from the shade unless the corner is
// fixed (+0x31.. == 2).
void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift) {
    uint32_t now = (*(uint32_t(__stdcall**)())0x5c8068)();
    uint32_t step = ((now + F<int32_t>(q, 0x3a)) / 88) & 0xff;
    int32_t cell = 0xff - (int32_t)step;
    const double k = *(const double*)0x5d08a0;
    float fu = (float)((double)(cell & 7) * k);
    double fv = (double)((cell >> 3) & 7) * k;
    uint32_t tint = G<uint32_t>(0xd97bbc);
    int32_t red = (tint >> 16) & 0xff, green = (tint >> 8) & 0xff, blue = tint & 0xff;
    double u1 = fu + *(const double*)0x5de408;
    double u2 = fu + *(const double*)0x5de400;
    double v2 = fv + k;
    uint32_t col[4];
    for (int32_t i = 0; i < 4; ++i) {
        int32_t s = (0xff - q[0x35 + i]) >> 1;
        int32_t r = red - s, g = green - s;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        col[i] = (((uint32_t)r << 8 | (uint32_t)g) << 8) | (uint32_t)blue;
    }
    uint8_t edge = q[0x30];
    if (edge == 2) {
        q[0x38] = 0;
        q[0x36] = 0;
        col[1] = 0xffffff;
        col[3] = 0xffffff;
    }
    uint32_t c0 = col[0];
    if (edge == 1) {
        q[0x37] = 0;
        q[0x35] = 0;
        col[2] = 0xffffff;
        c0 = 0xffffff;
    }
    uint32_t c1 = (uint32_t)q[0x36] << 24 | col[1];
    uint32_t c2 = (uint32_t)q[0x37] << 24 | col[2];
    uint32_t c3 = (uint32_t)q[0x38] << 24 | col[3];
    uint32_t a0 = (uint32_t)q[0x35] << 24 | c0;
    if (q[0x31] == 2) a0 = c0;
    if (q[0x32] == 2) c1 = col[1];
    if (q[0x33] == 2) c2 = col[2];
    if (q[0x34] == 2) c3 = col[3];
    auto put = [&](int32_t i, int32_t corner, uint32_t c, double u, double v) {
        F<uint32_t>(out, i * 0x18) = F<uint32_t>(q, corner * 0xc);
        F<float>(out, i * 0x18 + 4) = (float)((double)lift + F<float>(q, corner * 0xc + 4));
        F<uint32_t>(out, i * 0x18 + 8) = F<uint32_t>(q, corner * 0xc + 8);
        F<uint32_t>(out, i * 0x18 + 0xc) = c;
        F<float>(out, i * 0x18 + 0x10) = (float)u;
        F<float>(out, i * 0x18 + 0x14) = (float)v;
    };
    put(0, 0, a0, u2, v2);
    put(1, 1, c1, u1, v2);
    put(2, 3, c3, u2, fv);
    put(3, 1, c1, u1, v2);
    put(4, 2, c2, u1, fv);
    put(5, 3, c3, u2, fv);
}
RT3_REPLACE(0x5679b0, Quad_ToVertices);

} // namespace rt3
