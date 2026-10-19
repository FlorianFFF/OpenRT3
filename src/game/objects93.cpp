// Texture list / render targets / TGA export / mouse device in blocks 0x52..0x54 (batch 96). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Texture_CountMemory(uint8_t* t);
uint32_t __fastcall Obj_Next0(uint8_t* o);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
void __fastcall Texture_FreeImage(uint8_t*, int, uint8_t* img);
void __fastcall Texture_ReleaseTarget(uint8_t* t);
void __fastcall Engine_SampleTextureMem(void* e);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Window_FrameSize(void** o, int, int32_t* w, int32_t* h);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }
static inline void Release(void* com) { ((uint32_t(__stdcall*)(void*))(*(void***)com)[2])(com); }
static inline uint8_t* Input() { return hook::Global<uint8_t*>(0x6d4018); }

// 0x5207a0. Texture memory totals over the global texture list (video memory,
// resident, count).
void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count) {
    if (video) *video = 0;
    if (resident) *resident = 0;
    if (count) *count = 0;
    int32_t v = 0, r = 0, n = 0;
    for (uint8_t* t = hook::Global<uint8_t*>(0xccbb2c); t; t = (uint8_t*)Obj_Next0(t)) {
        Texture_CountMemory(t);
        v += F<int32_t>(t, 0xcc);
        r += F<int32_t>(t, 0xd0);
        ++n;
    }
    if (video) *video = v;
    if (resident) *resident = r;
    if (count) *count = n;
}
RT3_REPLACE(0x5207a0, Textures_MemoryTotals);

// 0x532490. The terrain colour map (+0x478) pixels and its size (+0x159c/+0x15a0).
void __fastcall Render_ColourMap(uint8_t* r, int, uint32_t** pix, uint32_t* w, uint32_t* h) {
    if (pix) *pix = F<uint8_t*>(r, 0x478) ? Texture_Pixels32(F<uint8_t*>(r, 0x478), 0, nullptr, nullptr, 0) : nullptr;
    if (w) *w = F<uint32_t>(r, 0x159c);
    if (h) *h = F<uint32_t>(r, 0x15a0);
}
RT3_REPLACE(0x532490, Render_ColourMap);

// 0x5418f0. Makes this texture the render target (created on demand).
void __fastcall Texture_BindTarget(uint8_t* t, int, void* dev) {
    if (!F<int32_t>(t, 7)) Texture_CreateTarget(t, 0, F<int32_t>(t, 0xb), F<int32_t>(t, 0xf), dev);
    void* depth = F<void*>(t, 0xc8);
    if (!depth) return;
    void* surf = F<void*>(t, 0xc4);
    if (!surf) return;
    ((int32_t(__stdcall*)(void*, void*, void*))(*(void***)dev)[0x7c / 4])(dev, surf, depth);
}
RT3_REPLACE(0x5418f0, Texture_BindTarget);

static inline void RemoveImage(uint8_t* t, int32_t i) {
    t[4]--;
    Texture_FreeImage(t, 0, F<uint8_t*>(t, 0xa0 + i * 4));
    t[0x98 + i] = 0;
    F<void*>(t, 0xa0 + i * 4) = nullptr;
}
void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i) {
    if (i >= 8 || i < 0 || !F<void*>(t, 0xa0 + i * 4)) return;
    RemoveImage(t, i);
}
RT3_REPLACE(0x541ba0, Texture_RemoveImage);

void* __fastcall Texture_TargetForFrame(uint8_t* t) {
    F<uint32_t>(t, 0xd4) = F<uint32_t>(Engine_Renderer(Engine()), 5);
    return F<void*>(t, 0xc0);
}
RT3_REPLACE(0x541bf0, Texture_TargetForFrame);

void __fastcall Texture_DropTarget(uint8_t* t) {
    if (!F<int32_t>(t, 7)) return;
    Texture_ReleaseTarget(t);
    F<int32_t>(t, 7) = 0;
}
RT3_REPLACE(0x541c10, Texture_DropTarget);

void __fastcall Texture_RemoveAllImages(uint8_t* t) {
    for (int32_t i = 0; i < 8; ++i)
        if (F<void*>(t, 0xa0 + i * 4)) RemoveImage(t, i);
}
RT3_REPLACE(0x542870, Texture_RemoveAllImages);

// 0x542b30. Writes image 0 as an uncompressed 32-bit TGA (top-down when asked).
void __fastcall Texture_SaveTga(uint8_t* t, int, const char* path, int32_t topDown) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ca334);
    if (!f) return;
    auto fwrite = (size_t(__cdecl*)(const void*, size_t, size_t, void*))0x5a276f;
    uint8_t hdr[0x14] = {};
    hdr[2] = 2;
    uint8_t* img = F<uint8_t*>(t, 0xa0);
    *(uint16_t*)(hdr + 0xc) = (uint16_t)F<int32_t>(img, 8);
    *(uint16_t*)(hdr + 0xe) = (uint16_t)F<int32_t>(img, 0xc);
    hdr[0x10] = 0x20;
    hdr[0x11] = topDown ? 0x20 : 0;
    fwrite(hdr, 0x12, 1, f);
    img = F<uint8_t*>(t, 0xa0);
    uint32_t n = (uint32_t)(F<int32_t>(img, 0xc) * F<int32_t>(img, 8)) << 2;
    if (Engine()[0x11424f]) Texture_Pixels32(t, 0, nullptr, nullptr, 0);
    fwrite(F<void*>(F<uint8_t*>(t, 0xa0), 0x26), 1, n, f);
    ((int(__cdecl*)(void*))0x5a1740)(f);
}
RT3_REPLACE(0x542b30, Texture_SaveTga);

// 0x542c10. Releases every texture's render target (device lost).
void __fastcall Textures_ReleaseTargets() {
    for (uint8_t* t = hook::Global<uint8_t*>(0xccbb2c); t; t = F<uint8_t*>(t, 0)) {
        if (!F<int32_t>(t, 7)) continue;
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
    Engine_SampleTextureMem(Engine());
}
RT3_REPLACE(0x542c10, Textures_ReleaseTargets);

// 0x543ae0. Acquires the mouse device (+4, vtbl +0x1c), remembering the cursor (+9/+0xd).
void __fastcall Mouse_Acquire(uint8_t* m) {
    if (m[8]) return;
    void* dev = F<void*>(m, 4);
    m[8] = 1;
    ((int32_t(__stdcall*)(void*))(*(void***)dev)[0x1c / 4])(dev);
    if (!Input()) return;
    F<float>(m, 9) = F<float>(Input(), 0xa94);
    if (!Input()) return;
    F<float>(m, 0xd) = F<float>(Input(), 0xa98);
}
RT3_REPLACE(0x543ae0, Mouse_Acquire);

// 0x543b20. Unacquires the mouse and puts the Windows cursor back at the
// remembered position (fractions of the display mode, plus the frame offset).
void __fastcall Mouse_Unacquire(uint8_t* m) {
    if (!m[8]) return;
    void* dev = F<void*>(m, 4);
    m[8] = 0;
    ((int32_t(__stdcall*)(void*))(*(void***)dev)[0x20 / 4])(dev);
    uint8_t* e = Engine();
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t x = (int32_t)((double)mode[0] * F<float>(m, 9));
    int32_t y = (int32_t)((double)mode[1] * F<float>(m, 0xd));
    int32_t fw, fh;
    Window_FrameSize((void**)e, 0, &fw, &fh);
    Import<int32_t(__stdcall*)(int32_t, int32_t)>(0x5c8264)(fw + x, fh + y);
}
RT3_REPLACE(0x543b20, Mouse_Unacquire);

// 0x543ba0 / 0x543c10. Cursor position (live input values while not acquired).
void __fastcall Mouse_GetPos3(uint8_t* m, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(m, 0x11);
    *y = F<uint32_t>(m, 0x15);
    if (z) *z = F<uint32_t>(m, 0x19);
    if (m[8] || !Input()) return;
    *x = F<uint32_t>(Input(), 0xa94);
    *y = F<uint32_t>(Input(), 0xa98);
    if (z) *z = F<uint32_t>(Input(), 0xa9c);
}
RT3_REPLACE(0x543ba0, Mouse_GetPos3);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y) {
    *x = F<uint32_t>(m, 9);
    *y = F<uint32_t>(m, 0xd);
    if (m[8] || !Input()) return;
    *x = F<uint32_t>(Input(), 0xa94);
    *y = F<uint32_t>(Input(), 0xa98);
}
RT3_REPLACE(0x543c10, Mouse_GetPos);

uint32_t __fastcall Input_Flag400() { return Input() ? F<uint32_t>(Input(), 0xa8c) & 0x400 : 0; }
RT3_REPLACE(0x543e00, Input_Flag400);
uint32_t __fastcall Input_Flag2000() { return Input() ? F<uint32_t>(Input(), 0xa8c) & 0x2000 : 0; }
RT3_REPLACE(0x543e20, Input_Flag2000);

} // namespace rt3
