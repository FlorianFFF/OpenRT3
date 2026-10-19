// Texture objects (mip images, render targets, TGA check) in blocks 0x4e..0x54 (batch 95). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

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
void __fastcall NameTable_Rescan(uint8_t* t) {
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
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
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
int32_t __fastcall Texture_AddImage(uint8_t* t, int, int32_t w, int32_t h, int32_t format) {
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

} // namespace rt3
