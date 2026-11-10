// Texture objects (mip images, render targets, TGA check) in blocks 0x4e..0x54 (batch 95). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall NameDialog_SetName(uint8_t* d, int, const char* name);
void __fastcall NameTable_Clear(uint8_t* t);
void __fastcall NameTable_ScanDirs(uint8_t* t);
void __fastcall Engine_SampleTextureMem(void* e);
uint32_t __fastcall Texture_Construct(uint8_t* s, int, uint8_t flag);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }
static inline void Release(void* com) { ((uint32_t(__stdcall*)(void*))(*(void***)com)[2])(com); }

// Texture: image slots +0xa0 (8 x 50-byte images: w +8, h +0xc, format +0x14/+0x18,
// in video memory +0x22, 32-bit pixels +0x26, 16-bit pixels +0x2a, extra +0x2e),
// slot used flags +0x98, count +4.

// 0x4edce0. Passes the name to the open name dialog (0x6d1268), if any.
void __fastcall NameDialog_SetNameIfOpen(const char* name) {
    if (uint8_t* d = hook::Global<uint8_t*>(0x6d1268)) NameDialog_SetName(d, 0, name);
}
RT3_REPLACE(0x4edce0, NameDialog_SetNameIfOpen);

// 0x53c900. Rescans the resource directories and forgets the open files (+0x4290c).
RT3_NOINLINE void __fastcall NameTable_Rescan(uint8_t* t) {
    NameTable_Clear(t);
    NameTable_ScanDirs(t);
    for (int k = 0; k < 10; ++k) F<int32_t>(t, 0x4290c + k * 4) = -1;
}
RT3_REPLACE(0x53c900, NameTable_Rescan);

// 0x540f60. Frees an image and its pixel buffers.
void __fastcall Texture_FreeImage(uint8_t*, int, uint8_t* img) {
    if (F<void*>(img, 0x2a)) Free(F<void*>(img, 0x2a));
    if (F<void*>(img, 0x26)) Free(F<void*>(img, 0x26));
    if (F<void*>(img, 0x2e)) Free(F<void*>(img, 0x2e));
    for (int k = 0; k < 0x32; ++k) img[k] = 0;
    Free(img);
}
RT3_REPLACE(0x540f60, Texture_FreeImage);

// 0x540fc0. Recomputes the memory totals: +0xd0 used slots resident in the
// current device generation (+7), +0xcc images in video memory.
void __fastcall Texture_CountMemory(uint8_t* t) {
    F<int32_t>(t, 0xcc) = 0;
    F<int32_t>(t, 0xd0) = 0;
    for (int32_t i = 0; i < 8; ++i) {
        uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
        if (!img) continue;
        if (t[0x98 + i] && F<int32_t>(t, 7) == F<int32_t>(Engine(), 0x114222)) F<int32_t>(t, 0xd0) += F<int32_t>(img, 4);
        if (F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0x22)) F<int32_t>(t, 0xcc) += F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 4);
    }
}
RT3_REPLACE(0x540fc0, Texture_CountMemory);

// 0x5410f0. Drops every image's pixel buffers (device generation reset).
void __fastcall Texture_FreePixels(uint8_t* t) {
    for (int32_t i = 0; i < 8; ++i) {
        uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
        if (!img) continue;
        if (F<void*>(img, 0x2a)) Free(F<void*>(img, 0x2a));
        if (F<void*>(img, 0x26)) Free(F<void*>(img, 0x26));
        if (F<void*>(img, 0x2e)) Free(F<void*>(img, 0x2e));
        F<int32_t>(img, 0x2e) = 0;
        F<int32_t>(img, 0x26) = 0;
        F<int32_t>(img, 0x2a) = 0;
        F<int32_t>(img, 0x22) = 0;
        F<int32_t>(t, 7) = 0;
    }
}
RT3_REPLACE(0x5410f0, Texture_FreePixels);

// 0x541160. Maximum texture size: 2048 (256 on low-end cards) capped by the
// detail level +0xd8 (0: 256, 1: 512, else 1024); 2048 for non-scaled textures (+5 == 0).
int32_t __fastcall Texture_MaxSize(uint8_t* t) {
    int32_t v = 0x800;
    if (!t[5]) return v;
    if (!Engine()[0x11422f]) v = 0x100;
    int32_t q = F<int32_t>(t, 0xd8);
    int32_t cap = q == 0 ? 0x100 : q == 1 ? 0x200 : 0x400;
    return v > cap ? cap : v;
}
RT3_REPLACE(0x541160, Texture_MaxSize);

// 0x5411c0. Whether the file is an uncompressed true-colour TGA (type 2, descriptor 0).
int32_t __fastcall Tga_IsPlain(const char* path) {
    int32_t fd = ((int(__cdecl*)(const char*, int))0x5a4c57)(path, 0x8000);
    if (fd == -1) return 0;
    uint8_t hdr[0x12];
    Read(fd, hdr, 0x12);
    Close(fd);
    return !hdr[0x11] && hdr[2] == 2 ? 1 : 0;
}
RT3_REPLACE(0x5411c0, Tga_IsPlain);

// 0x541780. Releases the render target (+0xc0) and depth surface (+0xc8).
void __fastcall Texture_ReleaseTarget(uint8_t* t) {
    if (void* p = F<void*>(t, 0xc0)) {
        Release(p);
        F<void*>(t, 0xc0) = nullptr;
    }
    if (void* p = F<void*>(t, 0xc8)) {
        Release(p);
        F<void*>(t, 0xc8) = nullptr;
    }
    Engine_SampleTextureMem(Engine());
    int32_t gen = F<int32_t>(t, 7);
    F<int32_t>(t, 0xcc) = 0;
    F<int32_t>(t, 0xd0) = 0;
    F<int32_t>(t, 0xd4) = 0;
    if (gen) hook::Global<int32_t>(0xccbb34)--;
    F<int32_t>(t, 7) = 0;
}
RT3_REPLACE(0x541780, Texture_ReleaseTarget);

// 0x5417e0. Creates a w x h render target texture (+0xc0, surface +0xc4) and its
// depth buffer (+0xc8) on the D3D8 device.
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev) {
    uint8_t* e = Engine();
    int32_t depthFmt = (F<int32_t>(e, 0x114237) ? 3 : 0) + 0x4d;
    int32_t fmt = e[0x11424f] ? 0x17 : F<int32_t>(e, 0x11424b);
    F<int32_t>(t, 0xb) = w;
    F<int32_t>(t, 0xf) = h;
    t[0x93] = 1;
    void** vt = *(void***)dev;
    if (((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void**))vt[0x50 / 4])(
            dev, w, h, 1, 1, fmt, 0, (void**)(t + 0xc0))) {
        F<void*>(t, 0xc0) = nullptr;
        return 0;
    }
    if (((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, void**))(*(void***)dev)[0x68 / 4])(
            dev, w, h, depthFmt, 0, (void**)(t + 0xc8))) {
        Release(F<void*>(t, 0xc0));
        F<void*>(t, 0xc0) = nullptr;
        F<void*>(t, 0xc8) = nullptr;
        return 0;
    }
    void* tex = F<void*>(t, 0xc0);
    ((int32_t(__stdcall*)(void*, int32_t, void**))(*(void***)tex)[0x3c / 4])(tex, 0, (void**)(t + 0xc4));
    Release(F<void*>(t, 0xc4));
    F<int32_t>(t, 7) = F<int32_t>(Engine(), 0x114222);
    t[5] = 0;
    Engine_SampleTextureMem(Engine());
    return 1;
}
RT3_REPLACE(0x5417e0, Texture_CreateTarget);

void __fastcall Texture_ImageSize(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
    if (w) *w = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 8);
    if (h) *h = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0xc);
}
RT3_REPLACE(0x541930, Texture_ImageSize);

// 0x541970. 32-bit pixels of image i, expanded from ARGB4444 on first use;
// 0 for format-2 images.
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
    uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
    if (F<int32_t>(img, 0x14) == 2) return nullptr;
    if (w) *w = F<int32_t>(img, 8);
    if (h) *h = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0xc);
    img = F<uint8_t*>(t, 0xa0 + i * 4);
    if (!F<void*>(img, 0x26)) {
        uint32_t n = (uint32_t)(F<int32_t>(img, 0xc) * F<int32_t>(img, 8));
        F<void*>(F<uint8_t*>(t, 0xa0 + i * 4), 0x26) = Malloc(n * 4);
        img = F<uint8_t*>(t, 0xa0 + i * 4);
        const uint16_t* src = F<uint16_t*>(img, 0x2a);
        uint32_t* dst = F<uint32_t*>(img, 0x26);
        for (uint32_t k = 0; k < n; ++k) {
            uint32_t v = src[k];
            uint32_t a = (v >> 12) & 0xf, r = (v >> 8) & 0xf, g = (v >> 4) & 0xf, b = v & 0xf;
            dst[k] = (((((a << 4) | a) << 8 | (r << 4) | r) << 8 | (g << 4) | g) << 8) | (b << 4) | b;
        }
    }
    return F<uint32_t*>(F<uint8_t*>(t, 0xa0 + i * 4), 0x26);
}
RT3_REPLACE(0x541970, Texture_Pixels32);

int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();

// 0x5428c0. Writes the texture's first image to file f as a baseline RGB JPEG at
// quality (bundled libjpeg; ARGB4444 images are expanded first when 0x6d4024
// +0x11424f is set), then with withAlpha its alpha plane: packed + its size +
// 0xbaba, or raw + its size + 0xbabb when packing fails, and the end mark 0xbabe.
// QUIRK: the raw plane is written from the end of the alpha buffer (the pointer
// the pixel loop advanced), so it holds the n bytes of heap that follow it.
RT3_NOINLINE void __fastcall Texture_SaveJpeg(uint8_t* t, int, void* f, uint32_t withAlpha, int32_t quality) {
    typedef size_t(__cdecl * FwriteFn)(const void*, size_t, size_t, void*);
    FwriteFn fwrite_ = (FwriteFn)0x5a276f;
    uint8_t* img = F<uint8_t*>(t, 0xa0);
    int32_t w = F<int32_t>(img, 8), h = F<int32_t>(img, 0xc);
    int32_t n = h * w;
    uint8_t* alpha = (uint8_t*)Malloc((size_t)n);
    uint8_t* rgb = (uint8_t*)Malloc((size_t)(n * 3));
    if (F<uint8_t>(hook::Global<uint8_t*>(0x6d4024), 0x11424f)) Texture_Pixels32(t, 0, nullptr, nullptr, 0);
    const uint32_t* px = F<uint32_t*>(F<uint8_t*>(t, 0xa0), 0x26);
    uint8_t* a = alpha;
    uint8_t* c = rgb;
    for (int32_t k = n; k != 0; --k) {
        uint32_t v = *px++;
        *a++ = (uint8_t)(v >> 24);
        *c++ = (uint8_t)(v >> 16);
        *c++ = (uint8_t)(v >> 8);
        *c++ = (uint8_t)v;
    }
    struct {
        uint8_t* row;
        uint8_t cinfo[0x15c];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = ((void*(__fastcall*)(void*))0x5b3b30)(l.err);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b3b80)(l.cinfo, 0x3e, 0x15b);
    ((void(__fastcall*)(void*, void*))0x5b48f0)(l.cinfo, f);
    F<int32_t>(l.cinfo, 0x19) = w;
    F<int32_t>(l.cinfo, 0x1d) = h;
    F<int32_t>(l.cinfo, 0x21) = 3;
    F<int32_t>(l.cinfo, 0x25) = 2;
    ((void(__fastcall*)(void*))0x5b4710)(l.cinfo);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b4290)(l.cinfo, quality, 1);
    ((void(__fastcall*)(void*, uint8_t))0x5b3db0)(l.cinfo, 1);
    while (F<uint32_t>(l.cinfo, 0xc6) < F<uint32_t>(l.cinfo, 0x1d)) {
        l.row = rgb + F<uint32_t>(l.cinfo, 0xc6) * (uint32_t)w * 3;
        ((uint32_t(__fastcall*)(void*, uint8_t**, uint32_t))0x5b3e20)(l.cinfo, &l.row, 1);
    }
    ((void(__fastcall*)(void*))0x5b3cc0)(l.cinfo);
    ((void(__fastcall*)(void*))0x5b3c40)(l.cinfo);
    if ((uint8_t)withAlpha) {
        uint8_t* out;
        int32_t outSize;
        int32_t packer[3] = { 0x91, 0, 0 };
        uint32_t mark;
        if (Pack_Compress(packer, 0, alpha, n, &out, &outSize) == 1) {
            fwrite_(out, 1, (size_t)outSize, f);
            fwrite_(&outSize, 1, 4, f);
            mark = 0xbaba;
            fwrite_(&mark, 1, 2, f);
            Free(out);
        } else {
            fwrite_(a, 1, (size_t)n, f);
            int32_t size = n;
            fwrite_(&size, 1, 4, f);
            mark = 0xbabb;
            fwrite_(&mark, 1, 2, f);
        }
        mark = 0xbabe;
        fwrite_(&mark, 1, 2, f);
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    }
    Free(alpha);
    Free(rgb);
}
RT3_REPLACE(0x5428c0, Texture_SaveJpeg);

FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text);
void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);

// 0x541c30. Loads image id (pack 0x6d4020) into img as a JPEG with the alpha
// plane Texture_SaveJpeg appends (end mark 0xbabe, then 0xbaba packed / 0xbabb raw
// with its size; none: opaque) and converts it for texture t (Image_To4444).
// An image already loaded (+0x26, or +0x2a without full colour 0x6d4024
// +0x11422f) is only converted. 1 when it is larger than the device allows
// (0x11423b / 0x11423f, or 256 for small textures, t+5), else 0.
// QUIRK: an unknown alpha mark leaves the alpha pointers unset in the original
// (stack garbage); here they are null.
RT3_NOINLINE int32_t __fastcall Texture_LoadJpeg(uint8_t* t, int, int32_t id, uint8_t* img) {
    typedef size_t(__cdecl * FioFn)(void*, size_t, size_t, void*);
    typedef int(__cdecl * FseekFn)(void*, long, int);
    FioFn fread_ = (FioFn)0x5a1b05;
    FseekFn fseek_ = (FseekFn)0x5a261f;
    uint8_t* dev = hook::Global<uint8_t*>(0x6d4024);
    if (!F<uint8_t>(dev, 0x11422f)) {
        if (F<void*>(img, 0x2a)) return 0;
        if (F<void*>(img, 0x26)) {
            Image_To4444(t, 0, img);
            return 0;
        }
    } else if (F<void*>(img, 0x26)) {
        return 0;
    }
    FILE* f = NameTable_OpenFile(hook::Global<uint8_t*>(0x6d4020), 0, id, 0);
    struct {
        uint8_t* row;
        uint8_t cinfo[0x1a0];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = ((void*(__fastcall*)(void*))0x5b3b30)(l.err);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b3230)(l.cinfo, 0x3e, 0x19f);
    ((void(__fastcall*)(void*, void*))0x5b3980)(l.cinfo, f);
    ((int(__fastcall*)(void*, uint8_t))0x5b3610)(l.cinfo, 1);
    ((int(__fastcall*)(void*))0x5b37e0)(l.cinfo);
    int32_t w = F<int32_t>(l.cinfo, 0x52), h = F<int32_t>(l.cinfo, 0x56);
    int32_t n = h * w;
    dev = hook::Global<uint8_t*>(0x6d4024);
    if (w > F<int32_t>(dev, 0x11423b) || h > F<int32_t>(dev, 0x11423f)) return 1;
    if (!F<uint8_t>(dev, 0x11422f) && t[5] && (w > 0x100 || h > 0x100)) return 1;
    uint8_t* rgb = (uint8_t*)Malloc((size_t)(n * 3));
    while (F<uint32_t>(l.cinfo, 0x6e) < F<uint32_t>(l.cinfo, 0x56)) {
        l.row = rgb + F<uint32_t>(l.cinfo, 0x6e) * F<uint32_t>(l.cinfo, 0x52) * 3;
        ((uint32_t(__fastcall*)(void*, uint8_t**, uint32_t))0x5b3740)(l.cinfo, &l.row, 1);
    }
    ((int(__fastcall*)(void*))0x5b3550)(l.cinfo);
    ((void(__fastcall*)(void*))0x5b3300)(l.cinfo);
    fseek_(f, 0, 2);
    long size = ((long(__cdecl*)(void*))0x5a254f)(f);
    fseek_(f, size - 2, 0);
    uint32_t mark = 0;
    fread_(&mark, 1, 2, f);
    uint8_t* alpha = nullptr;
    uint8_t* alphaFree = nullptr;
    if ((uint16_t)mark != 0xbabe) {
        alpha = (uint8_t*)Malloc((size_t)(F<int32_t>(l.cinfo, 0x56) * F<int32_t>(l.cinfo, 0x52)));
        memset(alpha, 0xff, (size_t)(h * w));
        alphaFree = alpha;
    } else {
        fseek_(f, size - 4, 0);
        fread_(&mark, 1, 2, f);
        int32_t stored;
        fseek_(f, size - 8, 0);
        fread_(&stored, 1, 4, f);
        int32_t packer[3] = { 0x91, 0, 0 };
        fseek_(f, size - stored - 8, 0);
        if ((uint16_t)mark == 0xbaba) {
            uint8_t* buf = (uint8_t*)Malloc((size_t)stored);
            fread_(buf, 1, (size_t)stored, f);
            int32_t outSize;
            Pack_Decompress(packer, 0, (const int32_t*)buf, stored, &alpha, &outSize);
            Free(buf);
            alphaFree = alpha;
        } else if ((uint16_t)mark == 0xbabb) {
            alpha = (uint8_t*)Malloc((size_t)(F<int32_t>(l.cinfo, 0x56) * F<int32_t>(l.cinfo, 0x52)));
            fread_(alpha, 1, (size_t)stored, f);
            alphaFree = alpha;
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    }
    uint32_t bytes = (uint32_t)(h * w) << 2;
    uint32_t* pix = (uint32_t*)Malloc(bytes);
    const uint8_t* a = alpha;
    const uint8_t* c = rgb;
    uint32_t* p = pix;
    for (int32_t k = n; k != 0; --k) {
        *p++ = (uint32_t)*a++ << 24 | (uint32_t)c[0] << 16 | (uint32_t)c[1] << 8 | c[2];
        c += 3;
    }
    Free(alphaFree);
    Free(rgb);
    F<int32_t>(img, 8) = w;
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 0x10) = 0x20;
    F<uint8_t>(img, 0x21) = 0;
    F<int32_t>(img, 0x14) = 3;
    F<int32_t>(img, 0x18) = 4;
    F<int32_t>(img, 0x1c) = 0x15;
    F<int32_t>(img, 0) = id;
    F<uint32_t*>(img, 0x26) = pix;
    F<uint32_t>(img, 4) = bytes;
    F<uint32_t*>(img, 0x22) = pix;
    F<uint8_t>(img, 0x20) = (h < 0x40 && w < 0x40) ? 0 : 1;
    Image_To4444(t, 0, img);
    return 0;
}
RT3_REPLACE(0x541c30, Texture_LoadJpeg);

int32_t __fastcall Texture_Width(uint8_t* t) {
    if (t[4] && t[0x98] && F<uint8_t*>(t, 0xa0)) return F<int32_t>(F<uint8_t*>(t, 0xa0), 8);
    return 1;
}
RT3_REPLACE(0x541aa0, Texture_Width);
int32_t __fastcall Texture_Height(uint8_t* t) {
    if (t[4] && t[0x98] && F<uint8_t*>(t, 0xa0)) return F<int32_t>(F<uint8_t*>(t, 0xa0), 0xc);
    return 1;
}
RT3_REPLACE(0x541ad0, Texture_Height);

uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t) {
    Texture_Construct(t, 0, 0);
    return t;
}
RT3_REPLACE(0x541b00, Texture_ConstructDefault);

// 0x541b10. Adds an empty w x h image of `format` in the first free slot; -1 when full.
RT3_NOINLINE int32_t __fastcall Texture_AddImage(uint8_t* t, int, int32_t w, int32_t h, int32_t format) {
    int32_t i = 0;
    while (F<void*>(t, 0xa0 + i * 4)) {
        if (++i >= 8) return -1;
    }
    t[4]++;
    t[0x98 + i] = 1;
    uint8_t* img = (uint8_t*)Malloc(0x32);
    for (int k = 0; k < 0x32; ++k) img[k] = 0;
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 0x14) = format;
    F<int32_t>(img, 0x18) = format;
    F<int32_t>(img, 8) = w;
    F<uint8_t*>(t, 0xa0 + i * 4) = img;
    return i;
}
RT3_REPLACE(0x541b10, Texture_AddImage);

int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img);
void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i);
void __fastcall Texture_CountMemory(uint8_t* t);

// 0x542f50. Appends texture t's first image as a JPEG (Texture_SaveJpeg) to file
// path; returns where it starts, 0 when the file can't be opened.
int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5dd8f4);
    if (!f) return 0;
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 2);
    int32_t pos = ((long(__cdecl*)(void*))0x5a254f)(f);
    Texture_SaveJpeg(t, 0, f, withAlpha, quality);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return pos;
}
RT3_REPLACE(0x542f50, Texture_AppendJpeg);

// 0x542fb0. Loads image id into img by its file kind (+0x18): 2 DDS, 3 TGA,
// 4 JPEG; 2 for anything else.
int32_t __fastcall Texture_LoadImage(uint8_t* t, int, int32_t id, uint8_t* img) {
    switch (F<int32_t>(img, 0x18)) {
    case 2: return Texture_LoadDds(t, 0, id, img);
    case 3: return Texture_LoadTga(t, 0, id, img);
    case 4: return Texture_LoadJpeg(t, 0, id, img);
    }
    return 2;
}
RT3_REPLACE(0x542fb0, Texture_LoadImage);

// 0x542ff0. Adds an image of format to texture t (+7 cleared) and loads id into
// it by its kind (as Texture_LoadImage); the image is removed again when that
// returns 1 or 2. Recounts the texture memory; 0 when no image could be added.
int32_t __fastcall Texture_AddAndLoad(uint8_t* t, int, int32_t id, int32_t format) {
    F<uint32_t>(t, 7) = 0;
    int32_t i = Texture_AddImage(t, 0, 0, 0, format);
    if (i == -1) return 0;
    uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
    int32_t r;
    switch (F<int32_t>(img, 0x18)) {
    case 2: r = Texture_LoadDds(t, 0, id, img); break;
    case 3: r = Texture_LoadTga(t, 0, id, img); break;
    case 4: r = Texture_LoadJpeg(t, 0, id, img); break;
    default: r = 2; break;
    }
    if (r == 1 || r == 2) Texture_RemoveImage(t, 0, i);
    Texture_CountMemory(t);
    return r;
}
RT3_REPLACE(0x542ff0, Texture_AddAndLoad);

} // namespace rt3
