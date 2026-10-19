// Terrain renderer accessors / plane math in block 0x53 (batch 85). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

float __fastcall Render_Get14e0(uint8_t* r) { return F<float>(r, 0x14e0); }
RT3_REPLACE(0x5331c0, Render_Get14e0);
uint32_t __fastcall Render_Get1548(uint8_t* r) { return F<uint32_t>(r, 0x1548); }
RT3_REPLACE(0x5331d0, Render_Get1548);
int32_t __fastcall Terrain_Rows(uint8_t* r) { return F<int32_t>(r, 0x15d9); }
RT3_REPLACE(0x533970, Terrain_Rows);
int32_t __fastcall Terrain_Cols(uint8_t* r) { return F<int32_t>(r, 0x15d5); }
RT3_REPLACE(0x533980, Terrain_Cols);
void __fastcall Terrain_Set15e1(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x15e1) = v; }
RT3_REPLACE(0x533990, Terrain_Set15e1);

// 0x5339a0. Plane through three points: normal (p1 - p0) x (p2 - p0) into
// *nx / *ny / *nz and d = -(n . p0) into *d. The second edge's x and y are
// rounded to float (stored back into the argument slots); nz is used unrounded for d.
void __fastcall Plane_From3Points(float* nx, float* ny, float x0, float y0, float z0, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float* nz, float* d) {
    double a = (double)x1 - x0, b = (double)y1 - y0, c = (double)z1 - z0;
    float e1 = (float)((double)x2 - x0);
    float e2 = (float)((double)y2 - y0);
    double f = (double)z2 - z0;
    *nx = (float)(b * f - c * e2);
    *ny = (float)(c * e1 - a * f);
    double n3 = a * e2 - b * e1;
    *nz = (float)n3;
    *d = (float)-((double)x0 * *nx + (double)y0 * *ny + n3 * z0);
}
RT3_REPLACE(0x5339a0, Plane_From3Points);

// 0x533a30. Height (y) at (qx, qz) of the plane through the three points:
// -(d + nx qx + nz qz) * (1 / ny), left unrounded in ST0.
double __stdcall Plane_HeightAt(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2,
                                float z2, float qx, float qz) {
    double a = (double)x1 - x0, b = (double)y1 - y0, c = (double)z1 - z0;
    float e1 = (float)((double)x2 - x0);
    float e2 = (float)((double)y2 - y0);
    double f = (double)z2 - z0;
    float nx = (float)(b * f - c * e2);
    float ny = (float)(c * e1 - a * f);
    double nz = a * e2 - b * e1;
    double dd = -((double)nx * x0 + (double)ny * y0 + nz * z0);
    double u = dd + (double)nx * qx + nz * qz;
    return -(u * ((double)*(const float*)0x5c85b4 / ny));
}
RT3_REPLACE(0x533a30, Plane_HeightAt);

uint32_t __fastcall Terrain_Get165d(uint8_t* r) { return F<uint32_t>(r, 0x165d); }
RT3_REPLACE(0x533ae0, Terrain_Get165d);
uint32_t __fastcall Terrain_Get1661(uint8_t* r) { return F<uint32_t>(r, 0x1661); }
RT3_REPLACE(0x533af0, Terrain_Get1661);
uint32_t __fastcall Terrain_Get1665(uint8_t* r) { return F<uint32_t>(r, 0x1665); }
RT3_REPLACE(0x533b00, Terrain_Get1665);
void* __fastcall Terrain_HeightsSlot(uint8_t* r) { return r + 0x1605; }
RT3_REPLACE(0x533b10, Terrain_HeightsSlot);
uint32_t __fastcall Terrain_Get15e9(uint8_t* r) { return F<uint32_t>(r, 0x15e9); }
RT3_REPLACE(0x533b20, Terrain_Get15e9);
uint32_t __fastcall Terrain_Get15ed(uint8_t* r) { return F<uint32_t>(r, 0x15ed); }
RT3_REPLACE(0x533b30, Terrain_Get15ed);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i) { return F<uint32_t>(r, 0x1631 + i * 4); }
RT3_REPLACE(0x533b40, Terrain_Layer);
uint32_t __fastcall Terrain_Get1655(uint8_t* r) { return F<uint32_t>(r, 0x1655); }
RT3_REPLACE(0x533b50, Terrain_Get1655);
uint32_t __fastcall Terrain_Get1659(uint8_t* r) { return F<uint32_t>(r, 0x1659); }
RT3_REPLACE(0x533b60, Terrain_Get1659);

} // namespace rt3
