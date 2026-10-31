// Visible terrain cell bounds from the view frustum corners in block 0x52 (batch 184). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);

// 0x5271b0. Bounding box (in 92-unit tiles) of the four ground corners of the view
// (+0x36e8.. x, +0x3708.. z), widened to include the camera tile (+0x3682/+0x3686),
// scaled down by `shift` with a margin (-1 / +2) and clipped to the terrain.
void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain,
                                    int32_t shift) {
    int32_t x = (int32_t)F<float>(s, 0x36e8) / 0x5c;
    int32_t z = (int32_t)F<float>(s, 0x3708) / 0x5c;
    *x0 = x;
    *y0 = z;
    *x1 = x;
    *y1 = z;
    for (int32_t k = 1; k < 4; ++k) {
        x = (int32_t)F<float>(s, 0x36e8 + k * 4) / 0x5c;
        z = (int32_t)F<float>(s, 0x3708 + k * 4) / 0x5c;
        if (*x0 >= x) *x0 = x;
        if (*y0 >= z) *y0 = z;
        if (*x1 <= x) *x1 = x;
        if (*y1 <= z) *y1 = z;
    }
    if (*x0 >= F<int32_t>(s, 0x3682)) *x0 = F<int32_t>(s, 0x3682);
    if (*y0 >= F<int32_t>(s, 0x3686)) *y0 = F<int32_t>(s, 0x3686);
    if (*x1 <= F<int32_t>(s, 0x3682)) *x1 = F<int32_t>(s, 0x3682);
    if (*y1 <= F<int32_t>(s, 0x3686)) *y1 = F<int32_t>(s, 0x3686);
    *x0 = (*x0 >> shift) - 1;
    *y0 = (*y0 >> shift) - 1;
    *x1 = (*x1 >> shift) + 2;
    *y1 = (*y1 >> shift) + 2;
    int32_t w = Terrain_Rows(terrain) >> shift;
    int32_t h = Terrain_Cols(terrain) >> shift;
    if (*x0 < 0) *x0 = 0;
    else if (*x0 > w) *x0 = w;
    if (*y0 < 0) *y0 = 0;
    else if (*y0 > h) *y0 = h;
    if (*x1 > w) *x1 = w;
    else if (*x1 < *x0) *x1 = *x0;
    if (*y1 > h) *y1 = h;
    else if (*y1 < *y0) *y1 = *y0;
}
RT3_REPLACE(0x5271b0, Scene_VisibleBounds);

} // namespace rt3
