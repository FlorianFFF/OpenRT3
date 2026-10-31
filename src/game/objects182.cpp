// Terrain normal mip averaging in block 0x53 (batch 186). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "core/crt.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

// 0x5331e0. Averages the finer level's vec3 cells (+0x15ed[level], width
// (+0x15d9 >> (level - 1)) + 1) over the 3x3 block around (2x, 2y), clipped, into
// cell (x, y) of level +0x15f1[level]. The x sum is kept on the FPU stack between
// cells (rounded to float only at some steps), which this mirrors exactly.
void __fastcall Terrain_AverageNormal(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t x0 = 2 * x - 1 < 0 ? 0 : 2 * x - 1;
    int32_t y0 = 2 * y - 1 < 0 ? 0 : 2 * y - 1;
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t sw = w >> (level - 1);
    int32_t x1 = sw - 1 >= 2 * x + 1 ? 2 * x + 1 : sw - 1;
    int32_t sh = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t y1 = sh - 1 >= 2 * y + 1 ? 2 * y + 1 : sh - 1;
    double sx = 0.0;
    float sy = 0.0f, sz = 0.0f;
    int32_t n = 0;
    if (y0 > y1) return;
    int32_t cnt = x1 - x0 + 1;
    for (int32_t row = y0; row <= y1; ++row) {
        int32_t col = x0;
        const float* src = F<const float*>(t, 0x15ed + level * 4);
        if (cnt >= 4) {
            int32_t last;
            do {
                const float* e = src + ((sw + 1) * row + col) * 3;
                sx += e[0];
                double dy = (double)sy + e[1];
                sz = (float)((double)sz + e[2]);
                float tmp = (float)(sx + e[3]);
                dy += e[4];
                double dz = (double)sz + e[5];
                tmp = (float)((double)tmp + e[6]);
                sy = (float)(dy + e[7]);
                sz = (float)(dz + e[8]);
                sx = (double)tmp + e[9];
                sy = (float)((double)sy + e[10]);
                sz = (float)((double)sz + e[11]);
                n += 4;
                col += 4;
                last = col + 3;
            } while (last < x1);
        }
        if (col <= x1) {
            const float* e = src + ((sw + 1) * row + col) * 3;
            for (int32_t k = x1 - col + 1; k; --k, e += 3, ++n) {
                sx += e[0];
                sy = (float)((double)sy + e[1]);
                sz = (float)((double)sz + e[2]);
            }
        }
    }
    if (!n) return;
    float inv = (float)(1.0 / n);
    float* dst = F<float*>(t, 0x15f1 + level * 4) + (((w >> level) + 1) * y + x) * 3;
    dst[0] = (float)((double)inv * sx);
    dst[1] = (float)((double)inv * sy);
    dst[2] = (float)((double)inv * sz);
}
RT3_REPLACE(0x5331e0, Terrain_AverageNormal);

} // namespace rt3

namespace rt3 {

// 0x536420. Shading value of terrain vertex (x, y) into +0x1619 (u16): the mean
// cross-product length between its normal (+0x15f1) and its 3x3 neighbours', plus
// one, scaled; type-0 cells (+0x1655) get a bonus, raised to a fixed level next to
// other types (not on the border); then the detail byte (+0x162d at level +0x15e1)
// is added and the result scaled by +0x14e0, capped.
void __fastcall Terrain_ShadeVertex(uint8_t* t, int, int32_t x, int32_t y, int32_t) {
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t x0 = x - 1 < 0 ? 0 : x - 1;
    int32_t ym1 = y - 1;
    int32_t y0 = ym1 < 0 ? 0 : ym1;
    int32_t x1 = w - 1 >= x + 1 ? x + 1 : w - 1;
    int32_t yp1 = y + 1;
    int32_t y1 = F<int32_t>(t, 0x15d5) - 1 >= yp1 ? yp1 : F<int32_t>(t, 0x15d5) - 1;
    double sum = 0.0;
    int32_t n = 0;
    int32_t idx = (w + 1) * y + x;
    const float* nrm = F<const float*>(t, 0x15f1);
    float cx = nrm[idx * 3], cy = nrm[idx * 3 + 1], cz = nrm[idx * 3 + 2];
    if (y0 > y1) return;
    for (int32_t row = y0; row <= y1; ++row)
        for (int32_t col = x0; col <= x1; ++col) {
            if (col == x && row == y) continue;
            const float* e = nrm + ((w + 1) * row + col) * 3;
            float ex = e[0], ey = e[1], ez = e[2];
            ++n;
            double X = (double)ey * cz - (double)ez * cy;
            double Y = (double)ez * cx - (double)cz * ex;
            double Z = (double)cy * ex - (double)ey * cx;
            double len = sqrt(Z * Z + Y * Y + X * X);
            sum += len * *(const double*)0x5dd2e8;
        }
    if (!n) return;
    double v = (sum + *(const float*)0x5c85b4) * *(const float*)0x5c9aa8 / n;
    if (v < *(const float*)0x5c8598) v = *(const float*)0x5c8598;
    int32_t stride = F<int32_t>(t, 0x15dd);
    const uint8_t* map = F<const uint8_t*>(t, 0x1655);
    const uint8_t* p = map + stride * y + x;
    uint8_t b = *p;
    if (!b) {
        v += *(const float*)0x5c8554;
        if (x > 0 && y > 0 && x < w - 2 && y < F<int32_t>(t, 0x15d5) - 2) {
            const uint8_t* q = map + yp1 * stride + x;
            const uint8_t* r = map + ym1 * stride + x;
            if (b != p[1] || b != q[1] || b != q[0] || b != q[-1] || b != p[-1] || b != r[-1] || b != r[0] || b != r[1])
                v = v + ((double)*(const float*)0x5dd2e4 - v);
        }
    }
    const uint8_t* det = F<const uint8_t*>(t, 0x162d) + ((w >> F<int32_t>(t, 0x15e1)) + 1) * y;
    v = (v + det[x]) * F<float>(t, 0x14e0);
    if (v > *(const float*)0x5dd2e0) v = *(const float*)0x5dd2e0;
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    F<uint16_t*>(t, 0x1619)[idx] = (uint16_t)r;
}
RT3_REPLACE(0x536420, Terrain_ShadeVertex);

} // namespace rt3

namespace rt3 {

// 0x541210. Converts the image's 32-bit pixels (+0x26, w +8 x h +0xc) to ARGB4444
// (+0x2a, allocated on first use; format fields +4/+0x10/+0x1c/+0x22 set), with the
// colour rounding error carried to the next pixel when the texture asks for
// dithering (+0x96); frees the source unless it is kept (+0x97). Only when 16-bit
// textures are in use and not disabled (engine +0x114255 / +0x11422f).
void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (e[0x11422f] || !e[0x114255]) return;
    if (!F<void*>(img, 0x26)) return;
    int32_t n = F<int32_t>(img, 0xc) * F<int32_t>(img, 8);
    if (!F<void*>(img, 0x2a)) F<void*>(img, 0x2a) = ((void*(__cdecl*)(size_t))0x5a125d)(n * 2);
    uint16_t* dst = F<uint16_t*>(img, 0x2a);
    F<int32_t>(img, 4) = n * 2;
    const uint32_t* src = F<const uint32_t*>(img, 0x26);
    F<int32_t>(img, 0x10) = 0x10;
    F<int32_t>(img, 0x1c) = 0x1a;
    F<uint16_t*>(img, 0x22) = dst;
    if (tex[0x96]) {
        int32_t r = 0, g = 0, b = 0;
        for (int32_t i = 0; i < n; ++i) {
            uint32_t px = src[i];
            r += (px >> 16) & 0xff;
            g += (px >> 8) & 0xff;
            b += px & 0xff;
            if (r > 0xff) r = 0xff;
            if (g > 0xff) g = 0xff;
            if (b > 0xff) b = 0xff;
            dst[i] = (uint16_t)(((b >> 4) & 0xf) | ((r & 0xf0) << 4) | (g & 0xf0) | ((px >> 16) & 0xf000));
            r &= 0xf;
            g &= 0xf;
            b &= 0xf;
        }
    } else {
        for (int32_t i = 0; i < n; ++i) {
            uint32_t px = src[i];
            dst[i] = (uint16_t)(((px >> 16) & 0xf000) | ((px >> 12) & 0xf00) | ((px >> 8) & 0xf0) | ((px >> 4) & 0xf));
        }
    }
    if (!tex[0x97]) {
        Free(F<void*>(img, 0x26));
        F<void*>(img, 0x26) = nullptr;
    }
}
RT3_REPLACE(0x541210, Image_To4444);

} // namespace rt3

namespace rt3 {

const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);

// 0x53b090. Opens the next animation pack (up to 999; the file stays open, fd in
// +0x204ac) and reads its 13-byte frame records (count +0x417ec, record size
// +0x1f404: 13 before v1003, else 0x4a with a 0x3d-byte name each, ".dsc" names
// registered with 0xccba50), sorted into 32 buckets by the low 5 bits of the
// first byte (+0 pointers, +0x223ec counts).
void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t) {
    int32_t& cur = F<int32_t>(o, 0x427b8);
    if (cur >= 0x3e7) return;
    int32_t fd = Open(path, 0x8000, 0);
    if (fd == -1) Fatal_Exit(GetString(0x60));
    F<int32_t>(o, 0x204ac + cur * 4) = fd;
    int32_t ver = 0, n;
    Read(F<int32_t>(o, 0x204ac + cur * 4), &ver, 4);
    if (ver < 0x3e9) Fatal_Exit(GetString(0x61));
    Read(F<int32_t>(o, 0x204ac + cur * 4), &n, 4);
    uint8_t* buf = (uint8_t*)Malloc(n * 0xd);
    if (ver < 0x3eb) {
        F<int32_t>(o, 0x1f404 + cur * 4) = 0xd;
        Read(F<int32_t>(o, 0x204ac + cur * 4), buf, n * 0xd);
    } else {
        F<int32_t>(o, 0x1f404 + cur * 4) = 0x4a;
        for (int32_t i = 0; i < n; ++i) {
            char name[0x3d];
            Read(F<int32_t>(o, 0x204ac + cur * 4), buf + i * 0xd, 0xd);
            Read(F<int32_t>(o, 0x204ac + cur * 4), name, 0x3d);
            const char* dot = ((const char*(__cdecl*)(const char*, int))0x5a2a00)(name, '.');
            if (dot && !Stricmp(dot, (const char*)0x5dd56c)) {
                void* reg = hook::Global<void*>(0xccba50);
                (*(void(__thiscall**)(void*, const char*, int32_t, int32_t, int32_t))(*(uint8_t**)reg + 4))(reg, name, 0, 0, 0);
            }
        }
    }
    F<int32_t>(o, 0x417ec + cur * 4) = n;
    int32_t fill[32] = {};
    for (int32_t k = 0; k < 32; ++k) F<int32_t>(o, 0x223ec + (cur * 32 + k) * 4) = 0;
    for (int32_t i = 0; i < n; ++i) ++F<int32_t>(o, 0x223ec + (cur * 32 + (buf[i * 0xd] & 0x1f)) * 4);
    for (int32_t k = 0; k < 32; ++k) {
        int32_t c = F<int32_t>(o, 0x223ec + (cur * 32 + k) * 4);
        F<void*>(o, (cur * 32 + k) * 4) = Malloc((c < 1 ? 1 : c) * 0xd);
    }
    for (int32_t i = 0; i < n; ++i) {
        int32_t k = *(uint32_t*)(buf + i * 0xd) & 0x1f;
        memcpy(F<uint8_t*>(o, (cur * 32 + k) * 4) + fill[k] * 0xd, buf + i * 0xd, 0xd);
        ++fill[k];
    }
    Free(buf);
    ++cur;
}
RT3_REPLACE(0x53b090, Anims_LoadPack);

} // namespace rt3

namespace rt3 {

void __fastcall Terrain_AllocMips(uint8_t* t);
void __fastcall Terrain_DownsampleHeight(uint8_t* t, int, int32_t x, int32_t y, int32_t level);
void __fastcall Terrain_DownsampleWord(uint8_t* t, int, int32_t x, int32_t y, int32_t level);

static inline void MipCell(uint8_t* t, int32_t x, int32_t y, int32_t level) {
    Terrain_AverageNormal(t, 0, x, y, level);
    Terrain_DownsampleHeight(t, 0, x, y, level);
    Terrain_DownsampleWord(t, 0, x, y, level);
}

// 0x5337f0. Rebuilds the terrain's four coarser levels from scratch, pumping the
// progress callback (0x6d4014) per column.
void __fastcall Terrain_RebuildMips(uint8_t* t) {
    Terrain_AllocMips(t);
    if (auto cb = hook::Global<void(*)()>(0x6d4014)) cb();
    for (int32_t level = 1; level < 5; ++level)
        for (int32_t x = 0; x <= F<int32_t>(t, 0x15d9) >> level; ++x) {
            if (auto cb = hook::Global<void(*)()>(0x6d4014)) cb();
            for (int32_t y = 0; y <= F<int32_t>(t, 0x15d5) >> level; ++y) MipCell(t, x, y, level);
        }
}
RT3_REPLACE(0x5337f0, Terrain_RebuildMips);

// 0x533890. Rebuilds the coarser levels over the vertex rectangle (x0, y0)-(x1, y1),
// clipped to the terrain.
void __fastcall Terrain_RebuildMipsRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (F<int32_t>(t, 0x15d9) - 1 < x1) x1 = F<int32_t>(t, 0x15d9) - 1;
    if (F<int32_t>(t, 0x15d5) - 1 < y1) y1 = F<int32_t>(t, 0x15d5) - 1;
    for (int32_t level = 1; level < 5; ++level)
        for (int32_t y = y0 >> level; y <= y1 >> level; ++y)
            for (int32_t x = x0 >> level; x <= x1 >> level; ++x) MipCell(t, x, y, level);
}
RT3_REPLACE(0x533890, Terrain_RebuildMipsRect);

// Point-samples an image plane down by whole steps (w / fx, h / fy) in place.
template <typename P>
static void Shrink(uint8_t* img, uint32_t plane, int32_t fx, int32_t fy) {
    int32_t w = F<int32_t>(img, 8), h = F<int32_t>(img, 0xc);
    int32_t sx, sy;
    double qx = (double)w / fx, qy = (double)h / fy;
    __asm {
        fld qx
        mov eax, 0x5a10d0
        call eax
        mov sx, eax
        fld qy
        mov eax, 0x5a10d0
        call eax
        mov sy, eax
    }
    int32_t nw = w / sx, nh = h / sy;
    int32_t bytes = nw * nh * (int32_t)sizeof(P);
    P* buf = (P*)Malloc(bytes);
    P* dst = buf;
    for (int32_t r = 0, row = 0; r < nh; ++r, row += sy) {
        const P* src = F<const P*>(img, plane) + F<int32_t>(img, 8) * row;
        for (int32_t i = 0; i < nw; ++i, src += sx) *dst++ = *src;
    }
    F<int32_t>(img, 8) = nw;
    F<int32_t>(img, 0xc) = nh;
    F<int32_t>(img, 4) = bytes;
    Free(F<void*>(img, plane));
    F<P*>(img, plane) = buf;
    F<P*>(img, 0x22) = buf;
}

// 0x5414e0. Shrinks texture slot idx's image (+0xa0[idx]) by the factors: its
// 32-bit plane (+0x26), then its 16-bit plane (+0x2a) from the already shrunk size.
// Not for kind-2 images.
void __fastcall Texture_Shrink(uint8_t* o, int, int32_t idx, int32_t fx, int32_t fy) {
    uint8_t* img = F<uint8_t*>(o, 0xa0 + idx * 4);
    if (F<int32_t>(img, 0x14) == 2) return;
    if (F<void*>(img, 0x26)) Shrink<uint32_t>(img, 0x26, fx, fy);
    img = F<uint8_t*>(o, 0xa0 + idx * 4);
    if (F<void*>(img, 0x2a)) Shrink<uint16_t>(img, 0x2a, fx, fy);
}
RT3_REPLACE(0x5414e0, Texture_Shrink);

} // namespace rt3

namespace rt3 {

uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);

// 0x542550. Turns the texture's bump slots (+0x98 flags, count +4) into du/dv maps
// once (+0x95; needs engine bump support +0x11422d): from the height in each pixel's
// low byte, the slopes to the right and down neighbours (wrapping) give a 2D
// direction, normalised with a per-slot scale (1.0, growing by k each slot), stored
// as two signed bytes; marks the image converted (+0x21, format 0x3c).
void __fastcall Texture_MakeBumpMaps(uint8_t* o) {
    if (!hook::Global<uint8_t*>(0x6d4024)[0x11422d] || o[0x95]) return;
    o[0x95] = 1;
    float scale = 1.0f;
    const double k = *(const float*)0x5ca210, zero = *(const float*)0x5c8598, k3 = *(const float*)0x5ca0ec;
    for (int32_t s = 0; s < o[4]; ++s) {
        uint8_t* img = F<uint8_t*>(o, 0xa0 + s * 4);
        if (!o[0x98 + s] || F<int32_t>(img, 0x14) == 2 || img[0x21]) continue;
        Texture_Pixels32(o, 0, nullptr, nullptr, s);
        img = F<uint8_t*>(o, 0xa0 + s * 4);
        img[0x21] = 1;
        F<int32_t>(img, 0x1c) = 0x3c;
        if (F<void*>(img, 0x2a)) {
            Free(F<void*>(img, 0x2a));
            F<void*>(img, 0x2a) = nullptr;
        }
        uint16_t* out = (uint16_t*)Malloc(F<int32_t>(img, 0x10) == 0x20 ? F<int32_t>(img, 4) >> 1 : F<int32_t>(img, 4));
        const uint32_t* src = F<const uint32_t*>(img, 0x26);
        int32_t w = F<int32_t>(img, 8);
        uint16_t* dstRow = out;
        int32_t curRow = 0;
        for (int32_t row = 1; row - 1 < F<int32_t>(img, 0xc); ++row) {
            int32_t h = F<int32_t>(img, 0xc);
            int32_t next = (row < h ? row : row - h) * w;
            uint16_t* d = dstRow;
            for (int32_t j = 0; j < F<int32_t>(img, 8); ++j) {
                int32_t right = j + 1 < w ? j + 1 : j + 1 - w;
                double c = (double)(int32_t)(src[curRow + j] & 0xff);
                double dv = (double)(int32_t)(src[next + j] & 0xff) - c;
                double du = (double)(int32_t)(src[curRow + right] & 0xff) - c;
                float a = (float)(k * du - dv * zero);
                double b = k * dv - du * zero;
                double inv = 1.0 / (sqrt((double)a * a + b * b + *(const float*)0x5dd8f0) * scale);
                double qu = (double)a * inv * k3, qv = inv * b * k3;
                int32_t u, v;
                __asm {
                    fld qu
                    mov eax, 0x5a10d0
                    call eax
                    mov u, eax
                    fld qv
                    mov eax, 0x5a10d0
                    call eax
                    mov v, eax
                }
                *d++ = (uint16_t)(((uint32_t)(uint8_t)u << 8) | (uint32_t)v);
            }
            dstRow += w;
            curRow += w;
        }
        Free(F<void*>(img, 0x26));
        F<void*>(img, 0x26) = nullptr;
        F<uint16_t*>(img, 0x22) = out;
        F<uint16_t*>(img, 0x2a) = out;
        if (F<int32_t>(img, 0x10) == 0x20) F<int32_t>(img, 4) >>= 1;
        scale = (float)((double)scale * *(const double*)0x5c85a8 + scale);
    }
}
RT3_REPLACE(0x542550, Texture_MakeBumpMaps);

} // namespace rt3
