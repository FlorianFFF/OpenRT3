// DDS texture loading from the packs in block 0x54 (batch 188). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
int32_t __fastcall Texture_MaxSize(uint8_t* t);
void __stdcall Texture_DecompressDxt3(uint8_t* rec);

// 0x542d50. Loads pack entry `id` as a DXT3 .dds into the image record (unless it
// already has pixels): 0 = done (or nothing to do), 1 = too big for the device
// (+0x11423b/+0x11423f) or the texture (Texture_MaxSize), 2 = not a DDS / no memory.
// Unpacks it right away when compressed textures are off (+0x11422f clear).
int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if ((!e[0x11422f] && F<void*>(img, 0x2a)) || F<void*>(img, 0x2e)) return 0;
    uint8_t* fs = hook::Global<uint8_t*>(0x6d4020);
    NameTable_Seek(fs, 0, id, 0);
    uint32_t size = (uint32_t)NameTable_EntrySize(hook::Global<uint8_t*>(0x6d4020), 0, id);
    uint8_t* buf = (uint8_t*)Malloc(size);
    int32_t hdr = size < 0x80 ? (int32_t)size : 0x80;
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, buf, hdr);
    if (F<uint32_t>(buf, 0) != 0x20534444) {
        Free(buf);
        return 2;
    }
    e = hook::Global<uint8_t*>(0x6d4024);
    int32_t w = F<int32_t>(buf, 0x10), h = F<int32_t>(buf, 0xc);
    if ((uint32_t)w > F<uint32_t>(e, 0x11423b) || (uint32_t)h > F<uint32_t>(e, 0x11423f)) {
        Free(buf);
        return 1;
    }
    int32_t m = Texture_MaxSize(o);
    if (w > m || h > m) {
        Free(buf);
        return 1;
    }
    if (hdr < (int32_t)size) NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, buf + hdr, size - hdr);
    uint32_t n = (uint32_t)(h / 4 * (w / 4)) << 4; // whole 4x4 blocks only (signed division, as the original)
    uint8_t* data = (uint8_t*)Malloc(n);
    if (!data) {
        Free(buf);
        return 2;
    }
    memcpy(data, buf + 0x80, n);
    Free(buf);
    F<int32_t>(img, 0x14) = 2;
    F<int32_t>(img, 0x18) = 2;
    F<int32_t>(img, 0) = id; // the record keeps its pack id (a reload after a device reset needs it)
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 8) = w;
    F<int32_t>(img, 0x10) = 0;
    img[0x21] = 0;
    F<uint32_t>(img, 0x1c) = 0x33545844;
    F<uint8_t*>(img, 0x2e) = data;
    F<uint32_t>(img, 4) = n;
    F<uint8_t*>(img, 0x22) = data;
    img[0x20] = (h >= 0x80 || w >= 0x80) ? 1 : 0;
    if (!hook::Global<uint8_t*>(0x6d4024)[0x11422f]) Texture_DecompressDxt3(img);
    return 0;
}
RT3_REPLACE(0x542d50, Texture_LoadDds);

} // namespace rt3

namespace rt3 {

const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);

typedef int32_t(__stdcall* CreateBufFn)(void* dev, uint32_t length, uint32_t usage, uint32_t format, uint32_t pool, void** out);

static inline void* Device(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline void VertexBuffer(uint8_t* r, uint32_t len, uint32_t usage, uint32_t fvf, uint32_t at, int32_t err) {
    F<void*>(r, at) = nullptr;
    void* dev = Device(r);
    (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, len, usage, fvf, 0, &F<void*>(r, at));
    if (!F<void*>(r, at)) Fatal_Exit(GetString(err));
    F<uint32_t>(r, 0x2543) += len;
}
static inline void IndexBuffer(uint8_t* r, uint32_t len, uint32_t at, int32_t err) {
    void* dev = Device(r);
    (*(CreateBufFn*)(*(uint8_t**)dev + 0x60))(dev, len, 0x208, 0x65, 0, &F<void*>(r, at));
    if (!F<void*>(r, at)) Fatal_Exit(GetString(err));
    F<uint32_t>(r, 0x2543) += len;
}

// 0x543f10. Creates the renderer's dynamic vertex buffers (terrain, models, two
// batch buffers, sprites, water / shadow by device caps, lines, overlay) and index
// buffers (one shared, 64 per-batch, one more), counting their bytes in +0x2543;
// any failure is fatal (strings 0x68..0x71). Then selects FVF 0x152.
int32_t __fastcall Renderer_CreateBuffers(uint8_t* r) {
    F<uint32_t>(r, 0x2543) = 0;
    VertexBuffer(r, 0x17cdc0, 0x208, 0x142, 0x2647, 0x68);
    IndexBuffer(r, 0x3e80, 0x264b, 0x69);
    VertexBuffer(r, 0x1fffe0, 0x208, 0x112, 0x2643, 0x6a);
    VertexBuffer(r, 0x1fffe0, 0x208, 0x112, 0x265b, 0x6b);
    VertexBuffer(r, 0x17ffe8, 0x208, 0x142, 0x2663, 0x6c);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (!e[0x11422e]) {
        F<void*>(r, 0x265f) = nullptr;
        void* dev = Device(r);
        if (e[0x114250]) {
            (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, 0x27ffd8, 0x10, 0x2c4, 0, &F<void*>(r, 0x265f));
            F<uint32_t>(r, 0x2543) += 0x27ffd8;
        } else {
            (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, 0x1fffe0, 0x10, 0x1c4, 0, &F<void*>(r, 0x265f));
            F<uint32_t>(r, 0x2543) += 0x1fffe0;
        }
        if (!F<void*>(r, 0x265f)) Fatal_Exit(GetString(0x6d));
    }
    VertexBuffer(r, 0x2ee00, 8, 0x112, 0x263f, 0x6e);
    for (int32_t k = 0; k < 0x40; ++k) IndexBuffer(r, 0xa410, 0x31ab + k * 4, 0x6f);
    IndexBuffer(r, 0x3e80, 0x2653, 0x70);
    VertexBuffer(r, 0x7800, 0x208, 0x142, 0x264f, 0x71);
    void* dev = Device(r);
    (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x130))(dev, 0x152);
    return 0;
}
RT3_REPLACE(0x543f10, Renderer_CreateBuffers);

} // namespace rt3

namespace rt3 {

static inline double Fsin(double v) {
    __asm {
        fld v
        fsin
        fstp v
    }
    return v;
}
static inline double Fcos(double v) {
    __asm {
        fld v
        fcos
        fstp v
    }
    return v;
}
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
static inline double Dq(uint32_t a) { return *(const double*)a; }
static inline double Fq(uint32_t a) { return *(const float*)a; }

// 0x545180. Builds the 144 sun positions over the day (+0x86b.., 0x33 bytes each,
// every 600 units from -43200): the light direction (-4/0/+4), a tilt (-0x2b..-0x1f),
// the sun colour bytes (-0xb..-9, warm at dawn and dusk, dim at night), the light
// colour floats (-0x17..-0xf), a strength (-0x1b) and the fog falloff (-8).
void __fastcall Sky_BuildSunTable(uint8_t* o) {
    uint8_t* p = o + 0x86b;
    for (int32_t i = 0, deg = -0xa8c0; i < 0x90; ++i, deg += 0x258, p += 0x33) {
        double a = (double)deg * Fq(0x5dd9e0);
        float c = (float)Fcos(a);
        double s2 = Fsin(a) * 2.0;
        float s2f = (float)s2;
        double inv = 1.0 / sqrt(s2 * s2f + (double)c * c + Fq(0x5c85b4));
        F<float>(p, -4) = (float)((double)s2f * inv);
        double ci = (double)c * inv;
        F<float>(p, 0) = (float)ci;
        F<float>(p, 4) = (float)inv;
        double t = -ci;
        float tilt = (float)(Fcos(t) * Dq(0x5cecd0));
        F<float>(p, -0x1f) = tilt;
        F<float>(p, -0x23) = tilt;
        float lift = (float)(Fsin(t) * Dq(0x5c8c18) + Dq(0x5c8da8));
        F<float>(p, -0x27) = lift;
        F<float>(p, -0x2b) = lift;
        int32_t r, g, b;
        if (c > Dq(0x5c8680)) {
            r = 0xcd;
            g = 0xd0;
            b = 0xde;
        } else if (c > Fq(0x5c8598)) {
            double q = Dq(0x5c8680) - c;
            double k = Dq(0x5c88a8) * q;
            r = Ftol(Dq(0x5dd9d8) + k);
            g = Ftol(Dq(0x5dd9d0) - k);
            b = Ftol(Dq(0x5dd9c0) - q * Dq(0x5dd9c8));
        } else {
            r = Ftol((double)c * Fq(0x5dd9b8) + Fq(0x5dd9b4));
            g = Ftol((double)c * Fq(0x5c8904) + Fq(0x5dd9b0));
            b = Ftol((double)c * Fq(0x5dd9ac) + Fq(0x5dd9a8));
            if (r < 2) r = 2;
            if (g < 2) g = 2;
            if (b < 0xf) b = 0xf;
        }
        p[-0xb] = (uint8_t)r;
        p[-0xa] = (uint8_t)g;
        p[-9] = (uint8_t)b;
        if (c > Dq(0x5c8c20)) F<uint32_t>(p, -0x1b) = 0x3f170a3d;
        else {
            double v = (double)c + Dq(0x5dd9a0);
            F<float>(p, -0x1b) = (float)v;
            if (v < Fq(0x5c8598)) F<float>(p, -0x1b) = 0.0f;
        }
        F<float>(p, -0x17) = (float)((double)(uint8_t)r * Fq(0x5dd17c) * Dq(0x5c8578) + Dq(0x5c8c20));
        F<float>(p, -0x13) = (float)((double)(uint8_t)g * Fq(0x5dd17c) * Dq(0x5c87e8) + Dq(0x5c88e8));
        F<float>(p, -0xf) = (float)((double)(uint8_t)b * Fq(0x5dd17c) * Dq(0x5c87e8) + Dq(0x5c88c8));
        double v = F<float>(p, -0x1b);
        if (v < Dq(0x5c8628)) v = Fq(0x5c8d48);
        F<float>(p, -8) = (float)((Dq(0x5c8770) - v * v * v) / ((v + Dq(0x5c86b8)) * Dq(0x5dd998)));
    }
}
RT3_REPLACE(0x545180, Sky_BuildSunTable);

} // namespace rt3

namespace rt3 {

static inline void StageState(uint8_t* r, uint32_t stage, uint32_t type, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t, uint32_t))(*(uint8_t**)dev + 0xfc))(dev, stage, type, value);
}

// 0x547650. Texture stage setup: with bump mapping (+0x11422d) every stage uses its
// own coordinates untransformed, stage 1 addresses mirrored, and the bump shaders
// (+0x2623 vertex, +0x2633 pixel) are selected; else with detail textures (+0x114250)
// stage 1 reuses coordinates 0 scaled by +0x36fd and wraps. Stage 0 clamps.
void __fastcall Renderer_SetupStages(uint8_t* r) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (e[0x11422d]) {
        for (uint32_t s = 0; s < 4; ++s) StageState(r, s, 0xb, s);
        for (uint32_t s = 0; s < 4; ++s) StageState(r, s, 0x18, 0);
        StageState(r, 1, 0xd, 2);
        StageState(r, 1, 0xe, 2);
        void* dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x130))(dev, F<uint32_t>(r, 0x2623));
        dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x160))(dev, F<uint32_t>(r, 0x2633));
        return;
    }
    if (e[0x114250]) {
        float m[16] = {};
        F<uint32_t>(m, 0) = F<uint32_t>(r, 0x36fd);
        F<uint32_t>(m, 0x14) = F<uint32_t>(r, 0x36fd);
        m[10] = 1.0f;
        m[15] = 1.0f;
        StageState(r, 1, 0xb, 0);
        StageState(r, 1, 0x18, 2);
        void* dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t, const float*))(*(uint8_t**)dev + 0x94))(dev, 0x11, m);
        StageState(r, 1, 0xd, 1);
        StageState(r, 1, 0xe, 1);
    }
    StageState(r, 0, 0xd, 3);
    StageState(r, 0, 0xe, 3);
}
RT3_REPLACE(0x547650, Renderer_SetupStages);

void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img);
void __fastcall Fatal_Exit(const char* msg);

// 0x542030. Loads pack entry `id` as an uncompressed true-colour .tga (24 / 32 bit,
// either row order) into the image record as 32-bit pixels (+0x26 / +0x22, 24-bit
// gets full alpha), streaming through a 64K-pixel buffer, then converts it for the
// texture. 0 = done (or nothing to do), 1 = too big for the device or for a small
// texture (+5, 256 max), 2 = not that kind of TGA / no memory.
int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (!e[0x11422f]) {
        if (F<void*>(img, 0x2a)) return 0;
        if (F<void*>(img, 0x26)) {
            Image_To4444(tex, 0, img);
            return 0;
        }
    } else if (F<void*>(img, 0x26)) return 0;
    auto fs = []() { return hook::Global<uint8_t*>(0x6d4020); };
    NameTable_Seek(fs(), 0, id, 0);
    NameTable_EntrySize(fs(), 0, id);
    uint8_t hdr[0x12];
    NameTable_Read(fs(), 0, hdr, 0x12);
    uint16_t w = F<uint16_t>(hdr, 0xc), h = F<uint16_t>(hdr, 0xe);
    e = hook::Global<uint8_t*>(0x6d4024);
    if (!hook::Global<int32_t>(0xccbb58) &&
        ((int32_t)w > F<int32_t>(e, 0x11423b) || (int32_t)h > F<int32_t>(e, 0x11423f)))
        return 1;
    if (!e[0x11422f] && tex[5] && (w > 0x100 || h > 0x100)) return 1;
    if (hdr[1] || hdr[2] != 2) return 2;
    if (hdr[0]) {
        uint8_t skip[0x100];
        NameTable_Read(fs(), 0, skip, hdr[0]);
    }
    int32_t W = w, H = h, bpp = hdr[0x10];
    int32_t n = H * W;
    uint32_t* pix = (uint32_t*)Malloc(n * 4);
    uint8_t* buf = (uint8_t*)Malloc(0x40000);
    if (!pix) return 2;
    int32_t inBuf = 0, left = n;
    int32_t bytes = bpp / 8;
    const uint8_t* src = buf;
    for (int32_t row = 0; row < H; ++row) {
        int32_t off = hdr[0x11] & 0x20 ? row * W * 4 : (H * 4 - 4) * W - row * W * 4;
        uint32_t* p = (uint32_t*)((uint8_t*)pix + off);
        uint32_t* end = (uint32_t*)((uint8_t*)p + W * 4);
        if (bpp != 0x20 && bpp != 0x18) {
            Fatal_Exit((const char*)0x5dd8b0);
            continue;
        }
        int32_t rest = bytes * left;
        while (p < end) {
            if (!inBuf) {
                NameTable_Read(fs(), 0, buf, left > 0x10000 ? bytes << 16 : rest);
                src = buf;
                inBuf = 0x10000;
            }
            if (bpp == 0x20) {
                *p = *(const uint32_t*)src;
                src += 4;
            } else {
                *p = (((0xffffff00u | src[2]) << 8 | src[1]) << 8) | src[0];
                src += 3;
            }
            ++p;
            --inBuf;
            --left;
            rest -= bytes;
        }
    }
    Free(buf);
    F<int32_t>(img, 0x10) = bpp;
    F<int32_t>(img, 0x14) = 3;
    F<int32_t>(img, 0x18) = 3;
    F<int32_t>(img, 0) = id;
    F<int32_t>(img, 0xc) = H;
    F<int32_t>(img, 8) = W;
    img[0x21] = 0;
    F<uint32_t>(img, 0x1c) = 0x15;
    F<uint32_t**>(img, 0x26) = (uint32_t**)pix;
    F<int32_t>(img, 4) = n * 4;
    F<uint32_t**>(img, 0x22) = (uint32_t**)pix;
    img[0x20] = (H >= 0x40 || W >= 0x40) ? 1 : 0;
    Image_To4444(tex, 0, img);
    return 0;
}
RT3_REPLACE(0x542030, Texture_LoadTga);

void* __fastcall Engine_Get1C(void* e);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// 0x546d40. Uploads the per-frame shader constants: vertex registers 0-3 (the
// matrix at +0x35af), 4/5 (0.5 scale-bias), 6 (0.95, 100, 0.1), 7 (the negated
// light direction +0x2517..+0x251f with the ambient term), 8 (sun colour x
// intensity), 9, 10 (detail scale +0x36fd), 11 (ambient), 12 (texture scale over the
// map size), 13 (lit sun colour), 14 (fog: -density x start, when table fog), and the
// pixel registers 0-3.
void __fastcall Renderer_SetShaderConstants(uint8_t* r) {
    auto dev = [&]() { return F<uint8_t*>(r, 0x25df); };
    auto vs = [&](uint32_t reg, const void* data, uint32_t n) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, const void*, uint32_t))(*(uint8_t**)d + 0x13c))(d, reg, data, n);
    };
    auto ps = [&](uint32_t reg, const void* data) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, const void*, uint32_t))(*(uint8_t**)d + 0x16c))(d, reg, data, 1);
    };
    double a = (double)F<float>(r, 0x25ff) * F<float>(r, 0x24f8) * *(const float*)0x5c8d60;
    double b = (double)F<float>(r, 0x25f7) * F<float>(r, 0x24f0) * *(const double*)0x5c8c18;
    uint32_t c4[4] = { 0x3f000000, 0x3f000000, 0, 0 };
    uint32_t c6[4] = { 0x3f733333, 0x42c80000, 0x3dcccccd, 0 };
    uint32_t c7[4] = { Bits(-F<float>(r, 0x2517)), F<uint32_t>(r, 0x251b), Bits(-F<float>(r, 0x251f)), Bits((float)b) };
    uint32_t c8[4] = { Bits((float)a), Bits((float)a), Bits((float)a), 0x3f800000 };
    uint32_t c9[4] = { 0x3f800000, 0x3f800000, 0, 0x3f800000 };
    uint32_t c10[4] = { F<uint32_t>(r, 0x36fd), F<uint32_t>(r, 0x36fd), 0, 0x3f800000 };
    uint32_t c11[4] = { Bits((float)b), Bits((float)b), Bits((float)b), 0 };
    uint32_t c13[4] = { Bits((float)-((double)F<float>(r, 0x2504) * F<float>(r, 0x2517) * a)),
                        Bits((float)((double)F<float>(r, 0x2508) * F<float>(r, 0x251b) * a)),
                        Bits((float)-((double)F<float>(r, 0x250c) * F<float>(r, 0x251f) * a)), 0 };
    vs(0, r + 0x35af, 4);
    vs(4, c4, 1);
    vs(5, c4, 1);
    vs(6, c6, 1);
    vs(7, c7, 1);
    vs(8, c8, 1);
    vs(9, c9, 1);
    vs(10, c10, 1);
    vs(11, c11, 1);
    vs(13, c13, 1);
    float fog[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    if (F<uint8_t*>(r, 0x83c)[0x1f]) {
        fog[1] = 1.0f;
        fog[0] = (float)(-((double)F<float>(r, 0x25b3) * F<float>(r, 0x2513)) * *(const double*)0x5dda20);
    }
    vs(14, fog, 1);
    int32_t w = Terrain_Rows((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024))) * 0x5c;
    int32_t h = Terrain_Cols((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024))) * 0x5c;
    double ih = *(const float*)0x5c85b4 / (double)h;
    float c12[4] = { (float)(*(const float*)0x5c85b4 / (double)w * F<float>(r, 0x36fd)), (float)(ih * F<float>(r, 0x36fd)), 0.0f,
                     0.0f };
    vs(12, c12, 1);
    uint32_t p0[4] = { 0x3f4ccccd, 0x3f59999a, 0x3f4ccccd, 0x3f800000 };
    uint32_t p1[4] = { 0, 0, 0, 0x3f000000 };
    uint32_t p2[4] = { 0, 0, 0, 0 };
    uint32_t p3[4] = { 0, 0, 0, 0x3f800000 };
    ps(0, p0);
    ps(1, p1);
    ps(2, p2);
    ps(3, p3);
}
RT3_REPLACE(0x546d40, Renderer_SetShaderConstants);

int32_t __fastcall Texture_Height(uint8_t* t);

static inline double FsinX(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }
static inline double FcosX(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }

// 0x5478a0. Sets up the water texture stages: with bump mapping (+0x11422d) stages
// 0-3 use their own coordinates untransformed, stages 2 / 3 get a rotated bump
// matrix (the second one scaled), stages 0 and 2 clamp, and the water shaders
// (+0x261b / +0x262b) are bound; otherwise stage 0 projects camera-space positions
// through a fixed matrix whose y offset follows the texture height.
void __fastcall Renderer_SetupWaterStages(uint8_t* r) {
    ((uint32_t(__stdcall*)())hook::Global<void*>(0x5c8068))();
    auto dev = [&]() { return F<uint8_t*>(r, 0x25df); };
    auto tss = [&](uint32_t stage, uint32_t type, uint32_t v) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t, uint32_t))(*(uint8_t**)d + 0xfc))(d, stage, type, v);
    };
    if (hook::Global<uint8_t*>(0x6d4024)[0x11422d]) {
        for (uint32_t k = 0; k < 4; ++k) tss(k, 0xb, k);
        for (uint32_t k = 0; k < 4; ++k) tss(k, 0x18, 0);
        float sn = (float)(FsinX(*(const double*)0x5c8608) * *(const double*)0x5dda28);
        float cs = (float)(FcosX(*(const double*)0x5c8608) * *(const double*)0x5dda28);
        tss(2, 7, Bits(sn));
        tss(2, 8, Bits(cs));
        tss(2, 9, Bits(cs));
        tss(2, 0xa, Bits(-sn));
        float sn2 = (float)((double)sn * *(const double*)0x5c8c20);
        float cs2 = (float)((double)cs * *(const double*)0x5c9b60);
        tss(3, 7, Bits(sn2));
        tss(3, 8, Bits(cs2));
        tss(3, 9, Bits(cs2));
        tss(3, 0xa, Bits(-sn2));
        tss(0, 0xd, 3);
        tss(0, 0xe, 3);
        tss(2, 0xd, 3);
        tss(2, 0xe, 3);
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)d + 0x130))(d, F<uint32_t>(r, 0x261b));
        d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)d + 0x160))(d, F<uint32_t>(r, 0x262b));
        return;
    }
    uint32_t m[16] = { 0x3edeb852, 0, 0, 0, 0, 0x3f59999a, 0, 0, 0x3f000000, 0, 0x3f800000, 0, 0, 0, 0, 0x3f800000 };
    int32_t th = Texture_Height(*F<uint8_t**>(r, 0x2593));
    m[9] = Bits((float)(*(const double*)0x5c8628 - (double)F<float>(r, 0x2537) / F<int32_t>(r, 0x25cb) / th));
    tss(0, 0xb, 0x20000);
    tss(0, 0x18, 0x103);
    uint8_t* d = dev();
    (*(int32_t(__stdcall**)(void*, uint32_t, const void*))(*(uint8_t**)d + 0x94))(d, 0x10, m);
}
RT3_REPLACE(0x5478a0, Renderer_SetupWaterStages);

} // namespace rt3
