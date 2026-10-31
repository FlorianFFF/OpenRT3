// Fatal error exit / camera ground / mesh LOD / terrain init, height curve, smoothing and mip levels in blocks 0x51..0x53 (batch 136). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Cursor_Show(int32_t show, int, int32_t);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                         int32_t f, int32_t g, int32_t h, int32_t i);
const char* __fastcall GetString(int32_t id);
void __fastcall Keyboard_RestoreScrollLock();
float __fastcall Camera_GroundAround(uint8_t* c, int, float x, float z);
float __fastcall Render_GetLayerValue(uint8_t* r, int, int32_t i);
void* __fastcall Engine_Renderer(void* e);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x51e980. Fatal error: once (0x6d3d18), flags the engine (+0x57), shows the
// cursor, logs msg (or string 0x45) and shows it in a message box (title string
// 0x44), restores the keyboard and exits.
void __fastcall Fatal_Exit(const char* msg) {
    if (hook::Global<int32_t>(0x6d3d18)) return;
    hook::Global<int32_t>(0x6d3d18) = 1;
    if (uint8_t* e = hook::Global<uint8_t*>(0x6d4024)) e[0x57] = 1;
    Cursor_Show(1, 0x5dcea4, 0x110);
    const int32_t k = (int32_t)0xfffffb2e;
    if (msg) {
        char buf[0x300];
        char* d = buf;
        while ((*d++ = *msg++) != 0) {}
        DebugLog(buf, k, k, k, k, k, k, k, k, k);
        Import<int32_t(__stdcall*)(void*, const char*, const char*, uint32_t)>(0x5c8220)(nullptr, buf, GetString(0x44), 0x10);
    } else {
        DebugLog(GetString(0x45), k, k, k, k, k, k, k, k, k);
    }
    Keyboard_RestoreScrollLock();
    ((void(__cdecl*)(int))0x5a2e9c)(0);
}
RT3_REPLACE(0x51e980, Fatal_Exit);

// 0x52d0c0. Ground height around the camera target (+0x1c, +0x24 in cells).
float __fastcall Camera_TargetGround(uint8_t* c) {
    const float k = Fl(0x5dcfe8);
    return Camera_GroundAround(c, 0, (float)((double)F<float>(c, 0x1c) * k), (float)((double)F<float>(c, 0x24) * k));
}
RT3_REPLACE(0x52d0c0, Camera_TargetGround);

// 0x52f370. Picks the mesh LOD (+0x21, entry +0x18) of the object for the viewer
// at (x, y, z): its screen size (base +0x10 over 4 d^2 + 1, d from the distance
// table 0x6d9098, times the renderer's layer value and quality halvings) against
// each LOD's threshold (+0xc); forced by engine +0x1146c2. +0x5c bit 2 = tiny.
int32_t __fastcall SceneObj_SelectLod(uint8_t* o, int, float x, float y, float z) {
    if (!F<void*>(o, 4)) return 0;
    const uint16_t* tab = (const uint16_t*)0x6d9098;
    auto cell = [](float v) { int32_t i = (int32_t)v; return (uint16_t)((i < 0 ? -i : i) >> 6); };
    uint32_t a = cell(x);
    uint32_t b = cell(z);
    uint32_t t = tab[a * 0x418 + b];
    uint32_t d = tab[t * 0x418 + cell(y)];
    int32_t dist2 = (int32_t)(d * d * 4 + 1);
    int32_t layer = ((int32_t(__thiscall*)(void*))(*(void***)o)[0x18 / 4])(o);
    float r = Render_GetLayerValue(F<uint8_t*>(o, 4), 0, layer);
    if (F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x22b63) > 0) r = (float)((double)r * D(0x5d08a0));
    double q = r;
    if (F<uint8_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x3692)) q = (double)r * D(0x5d08a0);
    uint8_t* mesh = F<uint8_t*>(o, 0x14);
    float size = (float)((double)F<float>(F<uint8_t*>(mesh, 0xc), 0x10) / dist2 * q);
    if ((o[0x5c] & 1) && size <= D(0x5c8628)) o[0x5c] |= 2;
    else o[0x5c] &= 0xfd;
    F<int32_t>(o, 0x21) = F<int32_t>(mesh, 0x14) - 1;
    for (int32_t i = 0; i < F<int32_t>(F<uint8_t*>(o, 0x14), 0x14); ++i)
        if ((double)F<int32_t>(F<uint8_t*>(mesh, 0xc), 0xc + i * 0x45) < size) {
            F<int32_t>(o, 0x21) = i;
            break;
        }
    int32_t forced = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x1146c2);
    if (forced >= 0) {
        int32_t last = F<int32_t>(mesh, 0x14) - 1;
        F<int32_t>(o, 0x21) = forced < last ? forced : last;
    }
    int32_t lod = F<int32_t>(o, 0x21);
    F<uint8_t*>(o, 0x18) = F<uint8_t*>(mesh, 0xc) + lod * 0x45;
    return lod;
}
RT3_REPLACE(0x52f370, SceneObj_SelectLod);

// Calls the optional hook 0x6d4014 (ECX as the original left it).
static inline void InitHook(void* ecx) {
    if (void* f = hook::Global<void*>(0x6d4014)) ((void(__thiscall*)(void*))f)(ecx);
}

// 0x532590. Resets the terrain renderer's fields to their initial state.
void __fastcall Terrain_Init(uint8_t* t) {
    t[0x1588] = 0;
    F<int32_t>(t, 0x154c) = 0;
    F<int32_t>(t, 0x15d9) = 0;
    F<int32_t>(t, 0x15d5) = 0;
    F<int32_t>(t, 0x155c) = -1;
    InitHook(t);
    t[0x1599] = 0;
    t[0x15b4] = 0;
    t[0x159b] = 0;
    t[0x159a] = 1;
    static const uint16_t zero1[] = { 0x478, 0x1568, 0x1570, 0x1558, 0x1560, 0x1564, 0x159c, 0x15a0, 0x15e1,
                                      0x1605, 0x15f1, 0x1619, 0x162d, 0x1631, 0x1635, 0x1639, 0x163d, 0x1669,
                                      0x1655, 0x1659, 0x165d, 0x1661, 0x1665 };
    for (uint16_t off : zero1) F<int32_t>(t, off) = 0;
    InitHook(t + 0x1631);
    for (int k = 0; k < 0x100; ++k) F<int32_t>(t, 8 + k * 4) = 0;
    for (int k = 0; k < 0x1b; ++k) F<int32_t>(t, 0x40c + k * 4) = 0;
    static const uint16_t zero2[] = { 0x166d, 0x1671, 0x1675, 0x1679, 0x167d, 0x1685, 0x1681, 0x15d1 };
    for (uint16_t off : zero2) F<int32_t>(t, off) = 0;
    for (int k = 0; k < 0x2a3; ++k) F<int32_t>(t, 0x1695 + k * 4) = 0;
    F<int32_t>(t, 0x1691) = 0;
    F<int32_t>(t, 0x15e9) = 0;
    F<int32_t>(t, 0x15ed) = 0;
    F<uint32_t>(t, 0x14e0) = 0x3f800000;
    F<int32_t>(t, 0x1534) = 0x60;
    for (int k = 0; k < 0x14; ++k) F<uint32_t>(t, 0x14e4 + k * 4) = 0x3f800000;
    InitHook(nullptr);
    F<int32_t>(t, 0x15e5) = 0;
    F<int32_t>(t, 0x1544) = 0x18;
    F<int32_t>(t, 0x1540) = 0x60;
    F<int32_t>(t, 0x153c) = 7;
    F<int32_t>(t, 0x1538) = 0x40;
    F<int32_t>(t, 0x1548) = 0x200;
    F<int32_t>(t, 0x1550) = 4;
    for (int k = 0; k < 0x19; ++k) F<int32_t>(t, 0x147c + k * 4) = 0;
}
RT3_REPLACE(0x532590, Terrain_Init);

// 0x532d90. Reshapes the heights (+0x1605, (cols+1) stride) by a 101-step curve of
// steepness k (clamped to 0x5cb4a8..10): heights are scaled by the curve at their
// fraction of the highest point (at least 1).
void __fastcall Terrain_ApplyHeightCurve(uint8_t* t, int, float k) {
    int32_t rows = F<int32_t>(t, 0x15d5);
    double mx = Fl(0x5c85b4);
    for (int32_t y = 0; y < rows; ++y)
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
            float h = F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x];
            if (mx < h) mx = h;
        }
    double kc = k;
    if (kc < D(0x5c86b8)) kc = Fl(0x5cb4a8);
    else if (kc > Fl(0x5c85f8)) kc = Fl(0x5c85f8);
    float tab[101];
    tab[0] = 1.0f;
    double inv = Fl(0x5c85b4) / kc;
    double acc = Fl(0x5c85b4);
    for (int i = 1; i <= 100; ++i) {
        double e = kc > Fl(0x5c85b4) ? kc : inv;
        acc = acc * (D(0x5c8770) - (e - D(0x5c8770)) * D(0x5ca1a8));
        tab[i] = (float)((double)tab[i - 1] + acc);
    }
    for (int n = 1; n <= 100; ++n) {
        float v = (float)(Fl(0x5c85b4) / (double)(n + 1) * tab[n]);
        tab[n] = v;
        if (kc > Fl(0x5c85b4)) tab[n] = (float)(D(0x5c8770) / v);
    }
    double s = kc / tab[100];
    for (int n = 1; n <= 100; ++n) tab[n] = (float)(s * tab[n]);
    for (int32_t y = 0; y < rows; ++y) {
        if (F<int32_t>(t, 0x15d9) <= 0) continue;
        double invMx = Fl(0x5c85b4) / mx;
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
            float* h = &F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x];
            int32_t idx;
            __asm {
                fld invMx
                mov eax, h
                fmul dword ptr [eax]
                fmul dword ptr ds:[0x5c8c28]
                mov eax, 0x5a10d0
                call eax
                mov idx, eax
            }
            if (idx < 0) idx = 0;
            else if (idx > 100) idx = 100;
            *h = (float)((double)tab[idx] * *h);
        }
    }
}
RT3_REPLACE(0x532d90, Terrain_ApplyHeightCurve);

// 0x532f60. Smooths every height above 1 to the mean of the truncated heights in
// the (2r)x(2r) box from (x - r, y - r), in place.
void __fastcall Terrain_SmoothPeaks(uint8_t* t, int, int32_t r) {
    for (int32_t y = 0; y < F<int32_t>(t, 0x15d5); ++y) {
        int32_t cols = F<int32_t>(t, 0x15d9);
        for (int32_t x = 0; x < cols; ++x) {
            float* grid = F<float*>(t, 0x1605);
            float* cell = &grid[(cols + 1) * y + x];
            if ((double)*cell <= Fl(0x5c85b4)) {
                cols = F<int32_t>(t, 0x15d9);
                continue;
            }
            double sum = Fl(0x5c8598), count = Fl(0x5c8598);
            for (int32_t xx = x - r; xx < x + r; ++xx)
                for (int32_t yy = y - r; yy < y + r; ++yy) {
                    if (xx < 0 || xx >= cols || yy < 0 || yy >= F<int32_t>(t, 0x15d5)) continue;
                    sum += (int32_t)grid[(cols + 1) * yy + xx];
                    count += Fl(0x5c85b4);
                }
            if (count > Fl(0x5c8598)) *cell = (float)(sum / count);
            cols = F<int32_t>(t, 0x15d9);
        }
    }
}
RT3_REPLACE(0x532f60, Terrain_SmoothPeaks);

// 0x533430. Mip level `level` height (+0x1605 + 4 level) at (x, y): the mean of the
// level-1 heights (+0x1601 + 4 level) over (2x-1..2x+1, 2y-1..2y+1), clamped.
void __fastcall Terrain_DownsampleHeight(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t x0 = 2 * x - 1 < 0 ? 0 : 2 * x - 1;
    int32_t y0 = 2 * y - 1 < 0 ? 0 : 2 * y - 1;
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t pw = w >> (level - 1);
    int32_t x1 = 2 * x + 1;
    if (pw - 1 < x1) x1 = pw - 1;
    int32_t ph = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t y1 = ph - 1 < 2 * y + 1 ? ph - 1 : 2 * y + 1;
    double sum = D(0x5c8608);
    int32_t count = 0;
    const float* src = F<const float*>(t, 0x1601 + level * 4);
    for (int32_t yy = y0; yy <= y1; ++yy)
        for (int32_t xx = x0; xx <= x1; ++xx) {
            sum += src[(pw + 1) * yy + xx];
            ++count;
        }
    F<float*>(t, 0x1605 + level * 4)[((w >> level) + 1) * y + x] = (float)(sum / count);
}
RT3_REPLACE(0x533430, Terrain_DownsampleHeight);

// 0x5335a0. Mip level `level` word value (+0x1619 + 4 level) at (x, y) from 7
// level-1 samples (+0x1615 + 4 level) around (2x, 2y), edges mirrored: their sum
// / 5 plus the (2x, 2y-1) sample.
void __fastcall Terrain_DownsampleWord(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t pw = w >> (level - 1), ph = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t sx = 2 * x, sy = 2 * y;
    if (sx >= pw) sx = pw - 1;
    if (sy >= ph) sy = ph - 1;
    int32_t xr = sx + 1 >= pw ? sx - 1 : sx + 1;
    int32_t yd = sy + 1 < ph ? sy + 1 : sy - 1;
    int32_t xl = sx - 1 > 0 ? sx - 1 : sx + 1;
    int32_t yu = sy - 1 > 0 ? sy - 1 : sy + 1;
    const uint16_t* src = F<const uint16_t*>(t, 0x1615 + level * 4);
    int32_t s = pw + 1;
    auto at = [&](int32_t i, int32_t j) { return (int32_t)src[s * j + i]; };
    int32_t top = at(sx, yu);
    int32_t sum = at(sx, sy) + at(xr, sy) + at(xr, yd) + at(sx, yd) + at(xl, sy) + at(xl, yu) + top;
    F<uint16_t*>(t, 0x1619 + level * 4)[((w >> level) + 1) * y + x] = (uint16_t)(sum / 5 + top);
}
RT3_REPLACE(0x5335a0, Terrain_DownsampleWord);

} // namespace rt3
