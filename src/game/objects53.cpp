// Functions across blocks 0x40..0x46 (batch 56). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void* __fastcall Obj_Ref2a8(void* o);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
uint8_t* __fastcall Obj_Region(void* o);

// 0x401300. Station platform offset for this object's diagonal parity
// ((4x - (-4)y) & 3 picks the signs of +-1.4375): with `alongX` the offset
// goes to *outX and the direction (0 or 4) to *dir, else to *outY (dir 2 / 6).
void __fastcall Obj_PlatformOffset(void* o, int, int32_t alongX, float* outX, float* outY, int32_t* dir) {
    int32_t cx = (int32_t)(Obj_CellX(o) * *(const float*)0x5c8568);
    int32_t cy = (int32_t)(Obj_CellY(o) * *(const float*)0x5c859c);
    int32_t m = (cx - cy) & 3;
    float a = 1.4375f, b = 1.4375f;
    if (m == 1) b = -1.4375f;
    else if (m == 2) a = b = -1.4375f;
    else if (m == 3) a = -1.4375f;
    if (alongX) {
        *dir = b < *(const float*)0x5c8598 ? 0 : 4;
        *outX = a;
        *outY = 0.0f;
    } else {
        *dir = a < *(const float*)0x5c8598 ? 6 : 2;
        *outX = 0.0f;
        *outY = b;
    }
}
RT3_REPLACE(0x401300, Obj_PlatformOffset);

// 0x449110. Terrain roughness (cached in +0x21c1): mean absolute height step
// along the rows of the height grid +0x1605, divided by 15.
float __fastcall Grid_Roughness(uint8_t* g) {
    if (!(F<float>(g, 0x21c1) == *(const float*)0x5c8598)) return F<float>(g, 0x21c1);
    float sum = 0.0f;
    for (int32_t y = 0; y < F<int32_t>(g, 0x15d5); ++y) {
        float prev = 0.0f;
        for (int32_t x = 0; x < F<int32_t>(g, 0x15d9); ++x) {
            float v = F<float*>(g, 0x1605)[(F<int32_t>(g, 0x15d9) + 1) * y + x];
            double d = ((double(__cdecl*)(double))0x5a152e)((double)prev - v);
            sum = (float)(d + sum);
            prev = v;
        }
    }
    int32_t n = F<int32_t>(g, 0x15d9) * F<int32_t>(g, 0x15d5);
    if (n < 1) n = 1;
    float fn = (float)n;
    float avg = (float)((double)sum / fn / *(const double*)0x5c8798);
    F<float>(g, 0x21c1) = avg;
    return avg;
}
RT3_REPLACE(0x449110, Grid_Roughness);

// 0x4499c0. Whether cell (x, y) or (near the map edge) any of its neighbours
// has water/road bits (low 3 bits of the 3-byte cell grid +0x165d).
int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y) {
    int edge = !(x > 0 && y > 0 && x < F<int32_t>(m, 0x2155) - 1 && y < F<int32_t>(m, 0x2159) - 1);
    int32_t stride = F<int32_t>(m, 0x15dd);
    const uint8_t* cells = F<uint8_t*>(m, 0x165d);
    if (cells[(stride * y + x) * 3] & 7) return 1;
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (int d = 0; d < 8; ++d) {
        int32_t ny = dy[d] + y, nx = dx[d] + x;
        if (edge) {
            uint8_t* map = hook::Global<uint8_t*>(0x62c120);
            if (nx < 0 || nx >= F<int32_t>(map, 0x15d9) - 1) continue;
            if (ny < 0 || ny >= F<int32_t>(map, 0x15d5) - 1) continue;
        }
        if (cells[((dy[d] + y) * stride + dx[d] + x) * 3] & 7) return 1;
    }
    return 0;
}
RT3_REPLACE(0x4499c0, Map_NearFeature);

// 0x4675a0. Ages a cargo load (+0x11): on a train it ripens up to its
// cargo's limit (0x5f2adc) in season 2; waiting at a building it decays
// unless the site still wants it. 1 when it expired (flag 1 of +0x12).
int32_t __fastcall Load_Age(uint8_t* l) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (F<uint16_t>(l, 8)) {
        int32_t limit = ((const int32_t*)0x5f2adc)[l[0x10]];
        if ((int8_t)l[0x11] < limit && w[0x10] == 2) l[0x11]++;
        return 0;
    }
    l[0x11]--;
    if (l[0x10] == 1 && (w[0x10] & 1)) {
        uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint16_t>(l, 0xa));
        if (b) {
            if (((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b)) {
                if (Obj_Array590(Obj_Ref2a8(b), 0, 3) > 0) l[0x11]++;
            } else {
                uint8_t* r = Obj_Region(b);
                for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) {
                    void* s = Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint16_t>(F<uint8_t*>(r, 0xef), i * 6));
                    if (Obj_Array590(s, 0, 3) > 0) {
                        l[0x11]++;
                        break;
                    }
                }
            }
        }
    }
    if ((int8_t)l[0x11] > 0) return 0;
    l[0x12] |= 1;
    return 1;
}
RT3_REPLACE(0x4675a0, Load_Age);

} // namespace rt3
