// Industry cargo stat views / cargo handling / site flatness check in block 0x41 (batch 152). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Industry_CargoStats(uint8_t* ind, int, int32_t cargo, int32_t p, float* base, float* share,
                                    float* in, float* inShare);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t max);
int32_t __fastcall Game_EffectiveVersion();
const char* __fastcall GetString(int32_t id);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x412960. The industry's total output of cargo (base + share) for p.
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return (double)base + share;
}
RT3_REPLACE(0x412960, Industry_OutputTotal);

// 0x4129a0. The industry's input of cargo per output share for p.
float __fastcall Industry_InputShare(uint8_t* ind, int, int32_t cargo, int32_t p) {
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, p, &base, &share, &in, &inShare);
    return inShare;
}
RT3_REPLACE(0x4129a0, Industry_InputShare);

// 0x4129d0. Whether the industry deals in cargo: among its current cargos (+0x7a0),
// producing it (over 0.001), or holding demand / stock of it (+0xad / +0xa1).
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo) {
    for (int32_t i = 0; i < F<int32_t>(ind, 0x7a8); ++i)
        if (F<uint16_t*>(ind, 0x7a0)[i] == cargo) return 1;
    float base, share, in, inShare;
    Industry_CargoStats(ind, 0, cargo, 0, &base, &share, &in, &inShare);
    if ((double)base + share > D(0x5c8678)) return 1;
    int32_t idx = F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cargo * 4);
    if (idx == -1) return 0;
    if (F<float>(ind, 0xad + idx * 4) > Fl(0x5c8598)) return 1;
    return F<float>(ind, 0xa1 + idx * 4) > Fl(0x5c8598) ? 1 : 0;
}
RT3_REPLACE(0x4129d0, Industry_HandlesCargo);

// 0x415020. Checks the placement rect (0x62b7a8 x0, 0x62b7ac x1, 0x62b7b0 y0,
// 0x62b7b4 y1) for flatness: heights must stay within 40 (4 for ports, 0x62b2fc
// +0x9c) of the masked mean (0x62b7c4) unless lenient (allowSteep) or the cell is
// flagged; min/max to 0x62b7b8/0x62b7bc, middle to 0x62b7c0. Failure sets the
// message 0x62b270 (string 0xb8) and returns 0.
int32_t __stdcall Site_CheckFlatness(int32_t allowSteep, int32_t ignoreFlags) {
    float x1f = (float)hook::Global<int32_t>(0x62b7ac);
    if (*(uint32_t*)&x1f == 0xc97423f0) return 1;
    hook::Global<uint32_t>(0x62b7bc) = 0xc97423f0;
    hook::Global<uint32_t>(0x62b7b8) = 0x497423f0;
    hook::Global<uint32_t>(0x62b7c4) = 0x497423f0;
    hook::Global<int32_t>(0x62b7c0) = 0;
    uint8_t* types = hook::Global<uint8_t*>(0x62b2fc);
    float tol = F<int32_t>(types, 0x9c) ? 4.0f : 40.0f;
    float buf[0x80];
    int32_t n = Map_GatherMasked(nullptr, 0, buf, 0x80);
    if (n > 0) {
        float sum = 0.0f;
        for (int32_t i = 0; i < n; ++i) sum = (float)((double)sum + buf[i]);
        hook::Global<float>(0x62b7c4) = (float)((double)sum / (float)n);
    }
    float& lo = hook::Global<float>(0x62b7b8);
    float& hi = hook::Global<float>(0x62b7bc);
    for (int32_t y = hook::Global<int32_t>(0x62b7b0); y <= hook::Global<int32_t>(0x62b7b4); ++y)
        for (int32_t x = hook::Global<int32_t>(0x62b7a8); x <= hook::Global<int32_t>(0x62b7ac); ++x) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            float h = (float)Map_GridFloat(m, 0, x, y);
            m = hook::Global<uint8_t*>(0x62c120);
            bool check = (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 1) ||
                         (!ignoreFlags && !F<int32_t>(hook::Global<uint8_t*>(0x62b2fc), 0x9c) &&
                          Game_EffectiveVersion() >= 0x68);
            if (check) {
                if (hook::Global<uint32_t>(0x62b7c4) == 0x497423f0) {
                    hook::Global<float>(0x62b7c4) = h;
                } else if (fabs((double)h - hook::Global<float>(0x62b7c4)) > tol && !allowSteep) {
                    const char* s = GetString(0xb8);
                    char* d = (char*)0x62b270;
                    while ((*d++ = *s++) != 0) {}
                    return 0;
                }
            }
            if (lo > h) lo = h;
            if (hi < h) hi = h;
        }
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x4fd) && F<int32_t>(hook::Global<uint8_t*>(0x62b2fc), 0x9c)) {
        double diff = (double)hi - lo;
        double lim = hook::Global<int32_t>(0x62ba3a) != -1 ? D(0x5c9680) : (double)Fl(0x5c8c28);
        if (diff > lim && !allowSteep) return 0;
    }
    hook::Global<float>(0x62b7c0) = (float)(((double)lo + hi) * Fl(0x5c8d48));
    return 1;
}
RT3_REPLACE(0x415020, Site_CheckFlatness);

int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial);
int32_t __stdcall Site_ChooseShape(uint8_t* obj);
void __fastcall Terrain_SnapshotFlags();

// The placement message 0x62b270 = string id.
static void Site_SetMessage(int32_t id) {
    const char* s = GetString(id);
    char* d = (char*)0x62b270;
    while ((*d++ = *s++) != 0) {}
}

// 0x416620 body. Checks that building type can go on the placement rect
// (0x62b7a8..0x62b7b4, grown by one cell right / down within the map) at a, b
// (kept in 0x62b30c / 0x62b310): no track around it (0xb9); a type with a shape
// (+0x123) must find one (0xba, rect then 3 cells out each way, 8 wide); ports
// (0x62b2fc +0x9c) must not cover occupied cells of grid 0x62b300 (0xbb). Success
// takes the terrain flag snapshot. An unset rect (x1 = -1000000) always passes.
static int32_t __cdecl Site_CheckPlacementImpl(uint8_t* type, uint32_t a, uint32_t b) {
    float x1f = (float)hook::Global<int32_t>(0x62b7ac);
    hook::Global<uint32_t>(0x62b308) = hook::Global<uint32_t>(0x62b304);
    hook::Global<uint32_t>(0x62b30c) = a;
    hook::Global<uint32_t>(0x62b310) = b;
    if (*(uint32_t*)&x1f == 0xc97423f0) return 1;
    int32_t& x0 = hook::Global<int32_t>(0x62b7a8);
    int32_t& x1 = hook::Global<int32_t>(0x62b7ac);
    int32_t& y0 = hook::Global<int32_t>(0x62b7b0);
    int32_t& y1 = hook::Global<int32_t>(0x62b7b4);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (x1 < F<int32_t>(map, 0x2155) - 1) ++x1;
    if (y1 < F<int32_t>(map, 0x2159) - 1) ++y1;
    ((int32_t*)0x62b318)[hook::Global<int32_t>(0x62b7a4)] = -1;
    int32_t special = F<int32_t>(hook::Global<uint8_t*>(0x62bae0), 0x88) <= 0 ? 1 : 0;
    if (!Track_RectIsFree(hook::Global<void*>(0x6cfca8), 0, x0 - 1, x1 + 1, y0 - 1, y1 + 1, special)) {
        Site_SetMessage(0xb9);
        return 0;
    }
    if (type[0x123]) {
        if (Site_ChooseShape(type) == 1) {
            x0 -= 3;
            x1 = x0 + 8;
            y0 -= 3;
            y1 = y0 + 8;
            Terrain_SnapshotFlags();
            return 1;
        }
        Site_SetMessage(0xba);
        return 0;
    }
    int32_t ok = 1;
    if (F<int32_t>(hook::Global<uint8_t*>(0x62b2fc), 0x9c) == 1) {
        for (int32_t y = y0; y <= y1; ++y) {
            if (ok != 1) continue;
            for (int32_t x = x0; x <= x1; ++x) {
                int32_t* grid = hook::Global<int32_t*>(0x62b300);
                if (grid && grid[F<int32_t>(map, 0x2155) * y + x]) {
                    Site_SetMessage(0xbb);
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (ok == 1) Terrain_SnapshotFlags();
    return ok;
}
// 0x416620. Type in EAX, a and b on the stack.
__declspec(naked) int32_t Site_CheckPlacement() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Site_CheckPlacementImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x416620, Site_CheckPlacement);

} // namespace rt3
