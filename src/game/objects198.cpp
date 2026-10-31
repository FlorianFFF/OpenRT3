// Larger game functions, part 6 (batch 204). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

void __fastcall City_LayStreets(uint8_t* c);
int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y);

// 0x421ce0. Lays out every city's streets.
void __fastcall Cities_LayStreets(Collection* cities) {
    for (int32_t i = 0; i < Collection_Count(cities); ++i)
        City_LayStreets((uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, i, 0)));
}
RT3_REPLACE(0x421ce0, Cities_LayStreets);

// 0x44a800. City owning cell (x, y) (its 4x4 block).
int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y) { return Map_BlockCity(m, 0, x / 4, y / 4); }
RT3_REPLACE(0x44a800, Map_CellCity);

float __fastcall SceneObj_Angle1(uint8_t* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Vehicle_AnimateLinks(uint8_t* v, int, float step);
static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }
static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static void QuietEmitter(uint8_t* e, int32_t a, uint32_t e5) {
    View_SetParams(e, 0, a, 100, 0x3f333333, 0x3e4ccccd, e5, 0x3f000000);
    for (int32_t k = 0; k < 6; ++k) F<int32_t>(e, 0x70 + k * 4) = 0;
}

// 0x4578e0. Flies the aircraft for `dt`: forward along its heading at +0x275, its
// height kept around the cruise altitude +0x271 over the ground (pitching up when
// low, down when far above, nose limited to +-pi/8) with occasional random course
// changes (+0x23f left, timer +0x29d, heading target +0x2a1); pitch and heading ease
// toward their targets, the exhaust emitters are calmed and the props spun.
void __fastcall Aircraft_Fly(uint8_t* v, int, float dt) {
    float yaw = SceneObj_Angle2(v);
    float pitch = SceneObj_Angle1(v);
    float dx = (float)-(ExeCos(yaw) * F<float>(v, 0x275));
    float dz = (float)-(ExeSin(yaw) * F<float>(v, 0x275));
    float x, y, z;
    SceneObj_GetPos(v, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    float h = (float)Terrain_HeightAt(F<uint8_t*>(v, 4), 0, (float)((double)x + dx), (float)((double)z + dz));
    dx = (float)((double)dx * dt);
    float dy = (float)((double)dt * Fl(0x5c8598));
    dz = (float)((double)dz * dt);
    float target = (float)((double)y - h);
    double half = (double)F<float>(v, 0x271) * D(0x5c8628);
    if (!(target > half) && target == target) {
        y = (float)(h + half);
        target = (float)((double)F<float>(v, 0x271) - target);
    } else if (!(target > F<float>(v, 0x271)) && target == target) {
        float low = (float)((double)F<float>(v, 0x271) - target);
        double q = (double)low / Fl(0x5cb460) * D(0x5c8c20);
        target = (float)(D(0x5c85b8) > q ? 0.01 : q);
        if (!(target < Fl(0x5c8a28))) target = 0.3926991f;
        y = (float)((double)pitch / target * dt * (low / D(0x5c8d68)) + y);
    } else {
        double far = (double)F<float>(v, 0x26d) * D(0x5c8680);
        bool climbDown = false;
        if (!(far >= target)) {
            double far2 = (double)F<float>(v, 0x26d) * D(0x5c8640);
            climbDown = !(far2 >= target) || !(pitch > D(0x5c9bc0));
        }
        if (climbDown) {
            double q = (double)F<float>(v, 0x26d) * D(0x5c8660);
            if (!(target >= q)) target = 0.0f;
            else {
                float t = (float)(q - target);
                if (t < Fl(0x5cb45c)) t = -500.0f;
                target = -0.1f;
                y = (float)((double)pitch / Fl(0x5cb458) * dt * (t / D(0x5caad8)) + y);
            }
        } else {
            F<uint32_t>(&target, 0) = 0x3d6a0ea2; // ~0.0572
            y = (float)((double)pitch / Fl(0x5cb454) * dt * D(0x5c85f0) + y);
        }
    }
    float timer = (float)((double)F<float>(v, 0x29d) - dt);
    F<float>(v, 0x29d) = timer;
    if (v[0x23f] && !(timer > Fl(0x5c8598)) && timer == timer) {
        F<int32_t>(v, 0x29d) = 0;
        if (Rand(0, 0x1388) < 10) {
            --v[0x23f];
            F<float>(v, 0x29d) = 100.0f;
            float r = (float)Rand(-25, 25);
            F<float>(v, 0x2a1) = (float)((double)r / Fl(0x5c8c28) + F<float>(v, 0x2a1));
        }
    }
    if (target > Fl(0x5c8a28)) target = 0.3926991f;
    else if (target < Fl(0x5cb450)) target = -0.3926991f;
    pitch = (float)(((double)target - pitch) * dt * D(0x5c8628) + pitch);
    yaw = (float)(((double)F<float>(v, 0x2a1) - yaw) * dt * D(0x5c8758) + yaw);
    SceneObj_SetAngles(v, 0, Bits(pitch), Bits(yaw), 0);
    Obj_MoveTo(v, 0, x, y, z);
    F<uint32_t>(v, 0x2a5) = F<uint32_t>(v, 0x285);
    if (uint8_t* e = F<uint8_t*>(v, 0x2f6)) QuietEmitter(e, 0x30d4, 0x40800000);
    if (uint8_t* e = F<uint8_t*>(v, 0x2fa)) QuietEmitter(e, 0x5dc, 0x3f333333);
    if (v[0x261]) Vehicle_AnimateLinks(v, 0, dt);
    Obj_MoveBy(v, 0, dx, dy, dz);
}
RT3_REPLACE(0x4578e0, Aircraft_Fly);

void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);

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

// 0x4a2640. Walks from (x0, y0) along `angle` in small steps toward (x1, y1) while
// it gets closer (company `company` must be allowed in both end regions; the walk
// fails off the map): tracks the highest ground (*peak) and whether it crosses
// water (*wet). The first dry, ordinary point below `minH` is the landing; failing
// that, a point rising to the start height that is not special ends the walk.
// Water crossings need 1895+. *dist gets the distance to the chosen point.
int32_t __fastcall Route_ProbeLine(uint8_t* self, int, float x0, float y0, float angle, float x1, float y1,
                                   uint32_t company, int32_t minH, float* peak, int32_t* wet, float* dist) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t* ra = (int32_t*)Map_RegionRecordAt(m, 0, x0, y0);
    int32_t* rb = (int32_t*)Map_RegionRecordAt(hook::Global<uint8_t*>(0x62c120), 0, x1, y1);
    Collection* companies = hook::Global<Collection*>(0x62be10);
    if (ra != rb && Collection_IsValid(companies, 0, company)) {
        if (!Company_EntryFlag(Collection_Get(companies, 0, company), 0, *ra)) return 0;
        if (!Company_EntryFlag(Collection_Get(companies, 0, company), 0, *rb)) return 0;
    }
    auto map = []() { return hook::Global<uint8_t*>(0x62c120); };
    float cx = x0, cy = y0;
    float h0 = (float)Terrain_HeightAt(map(), 0, (float)((double)x0 * Fl(0x5c8700)), (float)((double)y0 * Fl(0x5c8700)));
    float d1 = (float)DistanceSq2D(x0, y0, x1, y1);
    float savedPeak = *peak;
    *(uint32_t*)peak = 0xc7c34f80;
    *wet = 0;
    int32_t special = 0, landed = 0;
    F<int32_t>(self, 0x202) = 0;
    float lx = x0, ly = y0;
    if (!(Fl(0x5c8598) < d1)) return 0;
    float d2;
    for (;;) {
        d1 = (float)DistanceSq2D(cx, cy, x1, y1);
        cx = (float)((double)cx - ExeCos(angle) * D(0x5cfec8));
        double sy = cy;
        double sn = ExeSin(angle) * D(0x5cfec8);
        ++F<int32_t>(self, 0x202);
        cy = (float)(sy - sn);
        d2 = (float)DistanceSq2D(cx, cy, x1, y1);
        if (d2 < D(0x5d0030)) {
            cx = x1;
            cy = y1;
        }
        if (d2 < d1) {
            if (cx < Fl(0x5c8598) || cx > (float)F<int32_t>(map(), 0x2155)) return 0;
            if (cy < Fl(0x5c8598) || cy > (float)F<int32_t>(map(), 0x2159)) return 0;
            float h = (float)Terrain_HeightAt(map(), 0, (float)((double)cx * Fl(0x5c8700)), (float)((double)cy * Fl(0x5c8700)));
            if (h > *peak) *peak = h;
            bool check;
            if (!*wet && Terrain_KindIs1Or3to5(map(), 0, Ftol(cx), Ftol(cy))) {
                *wet = 1;
                special = 1;
                check = true;
            } else if (special) check = true;
            else if (Terrain_IsSpecial(map(), 0, Ftol(cx), Ftol(cy))) {
                special = 1;
                check = true;
            } else check = false;
            if (check && !landed && h < (float)minH) {
                int32_t ix = Ftol(cx), iy = Ftol(cy);
                if (!Terrain_KindIs1Or3to5(map(), 0, ix, iy) && !Terrain_IsSpecial(map(), 0, ix, iy)) {
                    landed = 1;
                    lx = cx;
                    ly = cy;
                    savedPeak = *peak;
                }
            }
            if (h >= h0 && !Terrain_IsSpecial(map(), 0, Ftol(cx), Ftol(cy))) {
                if (*wet && F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) < 0x767) return 0;
                float d = (float)Distance2D(x0, y0, cx, cy);
                *dist = d;
                return d < D(0x5d0028) ? 0 : 1;
            }
        }
        if (!(d2 < d1)) break;
    }
    if (!landed) return 0;
    if (*wet && F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) < 0x767) return 0;
    *peak = savedPeak;
    *dist = (float)Distance2D(x0, y0, lx, ly);
    return 1;
}
RT3_REPLACE(0x4a2640, Route_ProbeLine);

} // namespace rt3
