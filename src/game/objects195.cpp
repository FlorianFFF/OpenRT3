// Larger game functions, part 3 (batch 201). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline double Fl(uint32_t a) { return *(const float*)a; }

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

double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w,
                                    const float* p);

// 0x44dfe0. Ground-texture brush at (cx, cy) of radius r (min 0.1): each vertex
// within it is painted (Terrain_PaintLayers, also the far map edge) by the part of
// the brush falloff (or full with `hard`) that exceeds the stroke's coverage so far
// in `mask` (byte per vertex, 255 = done), which is raised to it.
void __fastcall Terrain_PaintBrush(uint8_t* t, int, float cx, float cy, float r, float strength, int32_t hard,
                                   int32_t mode, const float* w, const float* p, uint8_t* mask) {
    strength = (float)((double(__cdecl*)(double))0x5a152e)(strength);
    int32_t x0 = Ftol((double)cx - r - Fl(0x5c85b4));
    int32_t x1 = Ftol((double)cx + r + Fl(0x5c85b4));
    int32_t y0 = Ftol((double)cy - r - Fl(0x5c85b4));
    int32_t y1 = Ftol((double)cy + r + Fl(0x5c85b4));
    if (x0 < 0) x0 = 0;
    if (F<int32_t>(t, 0x2155) < x1) x1 = F<int32_t>(t, 0x2155);
    if (y0 < 0) y0 = 0;
    if (F<int32_t>(t, 0x2159) < y1) y1 = F<int32_t>(t, 0x2159);
    r = (float)(r < D(0x5c86b8) ? 0.1 : (double)r);
    for (int32_t y = y0; y < y1; ++y) {
        for (int32_t x = x0; x < x1; ++x) {
            int32_t i = F<int32_t>(t, 0x2155) * y + x;
            if (mask[i] >= 0xff) continue;
            float d = (float)Distance2D((float)x, (float)y, cx, cy);
            if (!(d < r) && d == d) continue;
            float k = (float)(hard ? 1.0 : D(0x5c8770) - (double)d / r);
            float cur = (float)((double)mask[i] / D(0x5ca110));
            if (!(k > cur)) continue;
            float diff = (float)((double)k - cur);
            mask[i] = (uint8_t)Ftol((double)k * Fl(0x5ca0ec));
            float s = (float)((double)diff * strength);
            Terrain_PaintLayers(t, 0, x, y, s, mode, w, p);
            if (x == F<int32_t>(t, 0x2155) - 1) Terrain_PaintLayers(t, 0, x + 1, y, s, mode, w, p);
            if (y == F<int32_t>(t, 0x2159) - 1) Terrain_PaintLayers(t, 0, x, y + 1, s, mode, w, p);
        }
    }
}
RT3_REPLACE(0x44dfe0, Terrain_PaintBrush);

void* __fastcall Obj_Ref388(void* o);
float __fastcall Car_TiltValue(uint8_t* c);
double __stdcall SpeedFactor(float v);
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
int32_t __fastcall Game_EffectiveVersion();

// Bend at a track node between headings h and the neighbour's: |delta|, beyond the
// threshold folded back (v1.03+) or reduced.
static float Bend(float h, uint8_t* next) {
    float h2 = Vehicle_Heading(next, 0, 1);
    float b = (float)((double(__cdecl*)(double))0x5a152e)(AngleDelta(nullptr, 0, h, h2));
    if (b > Fl(0x5c8764)) {
        if (Game_EffectiveVersion() >= 0x67) b = (float)(Fl(0x5c8760) - b);
        else b = (float)((double)b - Fl(0x5c8764));
    }
    return b;
}

// 0x4aaee0. Sums the train's car weights (+0x394; engines skipped unless
// `withEngines`): with `slope` each is scaled by its tilt's speed factor (doubled
// uphill when the train is stopped), with `curves` by 1 + the bend at its track node
// (capped 1.5). Outputs the mean / max bend and the weight-averaged tilt.
float __fastcall Train_ConsistWeight(uint8_t* t, int, int32_t quiet, int32_t slope, int32_t curves, int32_t withEngines,
                                     float* meanBend, float* maxBend, float* meanTilt) {
    if (F<int32_t>(t, 0x41) == -1) return 0.0f;
    Collection* cars = hook::Global<Collection*>(0x62ba84);
    float weightSum = 0.0f, tiltSum = 0.0f;
    if (!quiet) {
        if (uint8_t* c = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(t, 0x41)))
            Collection_Get(hook::Global<Collection*>(0x6ada84), 0, F<uint32_t>((uint8_t*)Obj_Ref388(c), 0x126));
    }
    uint8_t* car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(t, 0x41));
    float total = 0.0f, bendSum = 0.0f, bendCount = 0.001f, bendMax = 0.0f;
    if (!withEngines) {
        while (car && (F<int32_t>((uint8_t*)Obj_Ref388(car), 0x41) == 2 || F<int32_t>((uint8_t*)Obj_Ref388(car), 0x41) == 3)) {
            if (F<int32_t>(car, 0x352) == -1) {
                car = nullptr;
                break;
            }
            car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(car, 0x352));
        }
    }
    while (car) {
        float w = F<float>(car, 0x394);
        if (slope || meanTilt) {
            float tilt = Car_TiltValue(car);
            if (meanTilt) {
                weightSum = (float)((double)w + weightSum);
                tiltSum = (float)((double)tilt * w + tiltSum);
            }
            if (slope) {
                tilt = Car_TiltValue(car);
                if (F<float>(t, 0xe4) == Fl(0x5c8598) && tilt > Fl(0x5c8598)) tilt = (float)((double)tilt + tilt);
                w = (float)(SpeedFactor(tilt) * w);
            }
        }
        if (curves) {
            float k = 1.0f;
            if (F<int32_t>(car, 0x356) != -1) {
                Collection* nodes = hook::Global<Collection*>(0x6cfca8);
                uint8_t* node = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(car, 0x356));
                float h = Vehicle_Heading(node, 0, 1);
                float bend = 0.0f;
                if (F<int32_t>(node, 0x206) != -1)
                    bend = Bend(h, (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(node, 0x206)));
                if (F<int32_t>(node, 0x20a) != -1) {
                    float b = Bend(h, (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(node, 0x20a)));
                    bend = (float)((double)b + bend);
                }
                k = (float)((double)bend + Fl(0x5c85b4));
                k = (float)(k < D(0x5c8578) ? (double)k : 1.5);
                bendSum = (float)((double)bend + bendSum);
                bendCount = (float)((double)bendCount + Fl(0x5c85b4));
                if (!(bendMax > bend)) bendMax = bend;
            }
            w = (float)((double)k * w);
        }
        total = (float)((double)w + total);
        if (F<int32_t>(car, 0x352) == -1) break;
        car = (uint8_t*)Collection_Get(cars, 0, F<uint32_t>(car, 0x352));
    }
    if (meanBend) *meanBend = (float)((double)bendSum / bendCount);
    if (maxBend) *maxBend = bendMax;
    if (meanTilt) *meanTilt = (float)((double)tiltSum / (Fl(0x5c85b4) > weightSum ? 1.0f : weightSum));
    return total;
}
RT3_REPLACE(0x4aaee0, Train_ConsistWeight);

// Average of two channel bytes (x 0.5 in float, truncated).
static inline uint8_t Avg(uint8_t a, uint8_t b) { return (uint8_t)Ftol(((double)(float)a + (float)b) * Fl(0x5c8d48)); }

// 0x4f8370. Softens pixel (x, y) of the 32-bit image (+0x114, +0xfc x +0x100, not
// on the border): each channel is averaged with the right, then left, lower and
// upper neighbours in turn.
void __fastcall Image_SoftenPixel(uint8_t* img, int, int32_t x, int32_t y) {
    int32_t w = F<int32_t>(img, 0xfc);
    if (x <= 0 || x >= w - 1 || y <= 0 || y >= F<int32_t>(img, 0x100) - 1) return;
    uint8_t* pix = F<uint8_t*>(img, 0x114);
    uint8_t* p = pix + (w * y + x) * 4;
    uint8_t c[4];
    static const int8_t order[4] = { 2, 1, 0, 3 };
    for (int8_t k : order) c[k] = Avg(p[4 + k], p[k]);
    for (int8_t k : order) c[k] = Avg(p[k - 4], c[k]);
    uint8_t* q = pix + ((y + 1) * w + x) * 4;
    for (int8_t k : order) c[k] = Avg(q[k], c[k]);
    uint8_t* r = pix + ((y - 1) * w + x) * 4;
    uint8_t a3 = Avg(r[3], c[3]), a2 = Avg(r[2], c[2]), a1 = Avg(r[1], c[1]), a0 = Avg(r[0], c[0]);
    *(uint32_t*)p = (uint32_t)a3 << 24 | (uint32_t)a2 << 16 | (uint32_t)a1 << 8 | a0;
}
RT3_REPLACE(0x4f8370, Image_SoftenPixel);

int32_t __fastcall FileExists(const char* path);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall String_Assign(char** s, const char* src);

// 0x518fa0. Picks the game language (global string 0x6d3c90) from the Windows UI
// language (variants of Spanish, French, German, Chinese, Italian, Portuguese, and
// Japanese, Polish, Russian, Hungarian; else English), overridden by the first line
// of data2\overridelanguage (control characters cut), if its RT3.lng exists.
void Language_Select() {
    struct {
        char lang[0x198]; // the override is copied here unbounded (fits before buf)
        char buf[0x12d];
    } f;
    struct Entry { uint16_t id; uint32_t name; };
    static const Entry table[] = {
        { 0x404, 0x5d21cc }, { 0x804, 0x5d21cc }, { 0xc04, 0x5d21cc }, { 0x1004, 0x5d21cc }, { 0x1404, 0x5d21cc },
        { 0x407, 0x5d21d4 }, { 0x807, 0x5d21d4 }, { 0xc07, 0x5d21d4 }, { 0x1007, 0x5d21d4 }, { 0x1407, 0x5d21d4 },
        { 0x40c, 0x5d21ec }, { 0x80c, 0x5d21ec }, { 0xc0c, 0x5d21ec }, { 0x100c, 0x5d21ec }, { 0x140c, 0x5d21ec },
        { 0x180c, 0x5d21ec }, { 0x410, 0x5d21e4 }, { 0x810, 0x5d21e4 }, { 0x416, 0x5d21dc }, { 0x816, 0x5d21dc },
        { 0x411, 0x5d21fc }, { 0x415, 0x5d2204 }, { 0x419, 0x5d21f4 }, { 0x40e, 0x5d220c },
    };
    uint16_t id = ((uint16_t(__stdcall*)())hook::Global<void*>(0x5c8218))();
    const char* name = (const char*)0x5d21c4;
    if ((id & 0x3ff) == 0xa && id >= 0x40a && id <= 0x500a) name = (const char*)0x5d21bc; // all Spanish variants
    for (const Entry& e : table)
        if (e.id == id) name = (const char*)e.name;
    strcpy(f.lang, name);
    char path[0x17];
    memcpy(path, (const void*)0x5d21a4, sizeof path);
    if (FileExists(path)) {
        int fd = Open(path, 0x8000, 0);
        int n = Read(fd, f.buf, 0x12c);
        Close(fd);
        f.buf[n] = 0;
        for (int32_t i = 0; i < n; ++i)
            if (f.buf[i] < 0x1f) f.buf[i] = 0;
        if (strlen(f.buf) >= 2) strcpy(f.lang, f.buf);
    }
    char lng[0x190];
    FormatText(lng, (const char*)0x5d2188, f.lang, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (FileExists(lng)) String_Assign((char**)0x6d3c90, f.lang);
}
RT3_REPLACE(0x518fa0, Language_Select);

} // namespace rt3
