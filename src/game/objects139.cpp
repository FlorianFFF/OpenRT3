// Site collection teardown / DXT3 texture conversion / DirectInput mouse / projection / filter states / D3D releases in blocks 0x41..0x54 (batch 142). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/containers.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Hash_DestroyThunk(HashMap* m);
const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline void** Vt(void* p) { return *(void***)p; }
static inline int32_t Release(void* p) { return ((int32_t(__stdcall*)(void*))Vt(p)[2])(p); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x416950. Removes every site of the collection (vtable slot 2), clears it and
// frees the placement grid (0x62b300), the name hash (+0x88), the two node pools
// (+0x90, +0x8c, with their blocks) and the site globals 0x62ba58..0x62ba74.
void __fastcall Sites_DestroyAll(Collection* c) {
    while (Collection_Count(c)) {
        void** vt = *(void***)c;
        ((void(__thiscall*)(Collection*, uint32_t))vt[2])(c, Collection_IdAt(c, 0, 0, 0));
    }
    Collection_Clear(c);
    if (hook::Global<void*>(0x62b300)) {
        Free(hook::Global<void*>(0x62b300));
        hook::Global<void*>(0x62b300) = nullptr;
    }
    if (HashMap* h = F<HashMap*>(c, 0x88)) {
        Hash_DestroyThunk(h);
        ExeDelete(h);
        F<void*>(c, 0x88) = nullptr;
    }
    static const uint32_t pools[2] = { 0x90, 0x8c };
    for (uint32_t off : pools) {
        NodePool* p = F<NodePool*>(c, off);
        if (!p) continue;
        void* block = F<void*>(p, 4);
        Pool_Free(p);
        ExeDelete(p);
        F<void*>(c, off) = nullptr;
        if (block) Free(block);
    }
    if (hook::Global<void*>(0x62ba6c)) Free(hook::Global<void*>(0x62ba6c));
    static const uint32_t globals[] = { 0x62ba74, 0x62ba58, 0x62ba5c, 0x62ba60, 0x62ba64, 0x62ba6c, 0x62ba70 };
    for (uint32_t g : globals) hook::Global<int32_t>(g) = 0;
    hook::Global<int32_t>(0x5ee030) = -1;
}
RT3_REPLACE(0x416950, Sites_DestroyAll);

// 0x5423d0. Converts a DXT3 image record (data +0x2e, w +8, h +0xc) to A4R4G4B4
// once (+0x2a): decodes it through two D3DX scratch textures and keeps the 16-bit
// pixels (+0x2a/+0x22, size +4, bpp +0x10 = 16, format +0x1c = 0x1a, +0x14 = 3).
void __stdcall Texture_DecompressDxt3(uint8_t* rec) {
    if (F<void*>(rec, 0x2a) || !F<void*>(rec, 0x2e)) return;
    int32_t w = F<int32_t>(rec, 8), h = F<int32_t>(rec, 0xc);
    void* dev = F<void*>(hook::Global<uint8_t*>(0x6d4024), 8);
    auto create = (int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void**))0x570691;
    void* texA = nullptr;
    void* texB = nullptr;
    create(dev, w, h, 1, 0, 0x33545844, 3, &texA);
    create(F<void*>(hook::Global<uint8_t*>(0x6d4024), 8), w, h, 1, 0, 0x1a, 3, &texB);
    void* surfA = nullptr;
    void* surfB = nullptr;
    auto level = [](void* tex, void** out) { ((int32_t(__stdcall*)(void*, uint32_t, void**))Vt(tex)[0x3c / 4])(tex, 0, out); };
    level(texA, &surfA);
    level(texB, &surfB);
    int32_t rect[4] = { 0, 0, w, h };
    ((int32_t(__stdcall*)(void*, void*, void*, const void*, uint32_t, uint32_t, void*, const void*, uint32_t, uint32_t))0x57025d)(
        surfA, nullptr, nullptr, F<void*>(rec, 0x2e), 0x33545844, (uint32_t)((w >> 2) << 4), nullptr, rect, 0xffffffff, 0);
    ((int32_t(__stdcall*)(void*, void*, void*, void*, void*, void*, uint32_t, uint32_t))0x5706f7)(
        surfB, nullptr, nullptr, surfA, nullptr, nullptr, 0xffffffff, 0);
    Release(surfA);
    Release(texA);
    Free(F<void*>(rec, 0x2e));
    int32_t size = h * w * 2;
    F<void*>(rec, 0x2e) = nullptr;
    void* p = Malloc(size);
    F<void*>(rec, 0x2a) = p;
    F<void*>(rec, 0x22) = p;
    F<int32_t>(rec, 4) = size;
    F<int32_t>(rec, 0x10) = 0x10;
    F<int32_t>(rec, 0x1c) = 0x1a;
    F<int32_t>(rec, 0x14) = 3;
    struct { int32_t pitch; void* bits; } locked;
    ((int32_t(__stdcall*)(void*, void*, const void*, uint32_t))Vt(surfB)[0x24 / 4])(surfB, &locked, nullptr, 0);
    memcpy(F<void*>(rec, 0x2a), locked.bits, F<int32_t>(rec, 4));
    ((int32_t(__stdcall*)(void*))Vt(surfB)[0x28 / 4])(surfB);
    Release(surfB);
    Release(texB);
}
RT3_REPLACE(0x5423d0, Texture_DecompressDxt3);

// 0x543a10. Creates the DirectInput 8 mouse (+0 interface, +4 device; exclusive
// foreground) with centred position (+9/+0xd = 0.5) and cleared state; fatal
// (strings 0x47..0x4a) on failure.
uint8_t* __fastcall Mouse_Construct(uint8_t* m, int, void* hwnd, void* inst) {
    F<uint32_t>(m, 0xd) = 0x3f000000;
    F<uint32_t>(m, 9) = 0x3f000000;
    F<int32_t>(m, 0x19) = 0;
    F<int32_t>(m, 0x15) = 0;
    F<int32_t>(m, 0x11) = 0;
    F<int32_t>(m, 0x1d) = 0;
    if (((int32_t(__stdcall*)(void*, uint32_t, const void*, void**, void*))0x58bc30)(inst, 0x800, (const void*)0x5dd974,
                                                                                      (void**)m, nullptr) < 0)
        Fatal_Exit(GetString(0x47));
    void* di = F<void*>(m, 0);
    if (((int32_t(__stdcall*)(void*, const void*, void**, void*))Vt(di)[0xc / 4])(di, (const void*)0x5dd934,
                                                                                    (void**)(m + 4), nullptr) < 0)
        Fatal_Exit(GetString(0x48));
    void* dev = F<void*>(m, 4);
    if (((int32_t(__stdcall*)(void*, const void*))Vt(dev)[0x2c / 4])(dev, (const void*)0x5e1b34) < 0)
        Fatal_Exit(GetString(0x49));
    dev = F<void*>(m, 4);
    if (((int32_t(__stdcall*)(void*, void*, uint32_t))Vt(dev)[0x34 / 4])(dev, hwnd, 5) < 0) Fatal_Exit(GetString(0x4a));
    m[8] = 0;
    return m;
}
RT3_REPLACE(0x543a10, Mouse_Construct);

// 0x544930. Projects (x, y, z) with view x projection x viewport (+0x34ef, +0x35ef,
// +0x352f): screen position to *sx / *sy and |x| + |y| x 106 as a distance code;
// 0xff when behind the eye (w < 1) or past the far plane.
int32_t __fastcall Render_ProjectPoint(uint8_t* r, int, float x, float y, float z, float* sx, float* sy) {
    float m1[16], m[16];
    auto mul = (void*(__stdcall*)(float*, const void*, const void*))0x56d3cd;
    mul(m1, r + 0x34ef, r + 0x35ef);
    mul(m, m1, r + 0x352f);
    double w = (double)m[3] * x + (double)m[7] * y + (double)m[11] * z + m[15];
    if (w < Fl(0x5c85b4)) return 0xff;
    float inv = (float)(D(0x5c8770) / w);
    double zz = ((double)m[6] * y + (double)m[10] * z + (double)m[2] * x + m[14]) * inv;
    if (zz > D(0x5c8770)) return 0xff;
    double px = ((double)m[0] * x + (double)m[4] * y + (double)m[8] * z + m[12]) * inv;
    double py = ((double)m[1] * x + (double)m[5] * y + (double)m[9] * z + m[13]) * inv;
    *sx = (float)px;
    *sy = (float)py;
    double code = (fabs(py) + fabs(px)) * D(0x5dd988);
    int32_t r32;
    __asm {
        fld code
        mov eax, 0x5a10d0
        call eax
        mov r32, eax
    }
    return r32;
}
RT3_REPLACE(0x544930, Render_ProjectPoint);

// 0x544df0. Applies the texture filter settings (+0x83c): render states 0x8e/0x1d
// (by engine +0x11422e), blend 5/6 when +0x21, and per stage (+0x258f) the LOD bias
// (2 x +0x25) and the min filter (+0x29: 1 point, 2 linear) .
void __fastcall Render_ApplyFilterStates(uint8_t* r) {
    auto rs = [&](uint32_t s, uint32_t v) {
        void* d = F<void*>(r, 0x25df);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, s, v);
    };
    auto tss = [&](uint32_t stage, uint32_t type, uint32_t v) {
        void* d = F<void*>(r, 0x25df);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))Vt(d)[0xfc / 4])(d, stage, type, v);
    };
    if (hook::Global<uint8_t*>(0x6d4024)[0x11422e]) {
        rs(0x8e, 1);
        rs(0x1d, 0);
    } else {
        rs(0x1d, 0);
    }
    if (F<uint8_t*>(r, 0x83c)[0x21]) {
        rs(0x13, 5);
        rs(0x14, 6);
    }
    uint8_t* s = F<uint8_t*>(r, 0x83c);
    float bias = (float)((double)F<float>(s, 0x25) + F<float>(s, 0x25));
    uint32_t biasBits = *(uint32_t*)&bias;
    uint32_t filter = 0;
    int32_t mode = F<int32_t>(s, 0x29);
    if (mode == 1 || mode == 2) {
        filter = (uint32_t)mode;
        for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) tss(i, 0x13, biasBits);
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) tss(i, 0x12, filter);
}
RT3_REPLACE(0x544df0, Render_ApplyFilterStates);

// Releases COM object *slot (clearing it when asked); fatal (string 0x9b) when it
// still has references.
static void ReleaseChecked(uint8_t* r, uint32_t off, bool clear) {
    void* p = F<void*>(r, off);
    if (!p) {
        if (clear) F<void*>(r, off) = nullptr;
        return;
    }
    int32_t left = Release(p);
    if (clear) F<void*>(r, off) = nullptr;
    if (left) Fatal_Exit(GetString(0x9b));
}

// 0x5463e0. Releases the renderer's 5 vertex/index buffers.
void __fastcall Render_ReleaseBuffers(uint8_t* r) {
    static const uint16_t offs[] = { 0x2627, 0x260f, 0x262f, 0x261f, 0x2617 };
    for (uint16_t off : offs)
        if (F<void*>(r, off)) ReleaseChecked(r, off, true);
}
RT3_REPLACE(0x5463e0, Render_ReleaseBuffers);

// 0x5465c0. Releases the 64 texture slots (+0x31ab, cleared) and the 9 fixed
// textures (left set).
void __fastcall Render_ReleaseTextures(uint8_t* r) {
    for (int k = 0; k < 0x40; ++k) ReleaseChecked(r, 0x31ab + k * 4, true);
    static const uint16_t offs[] = { 0x265b, 0x2663, 0x265f, 0x263f, 0x2643, 0x264f, 0x264b, 0x2653, 0x2647 };
    for (uint16_t off : offs) ReleaseChecked(r, off, false);
}
RT3_REPLACE(0x5465c0, Render_ReleaseTextures);

// 0x5468c0. Projects (x, y, z) with the combined matrix (+0x33af, 4x4); 1 with
// out = (x, y, z) when in front (w >= 0.0001), z <= maxZ and |x|, |y| <= limit.
int32_t __fastcall Render_ProjectClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out) {
    const float* m = (const float*)(r + 0x33af);
    double w = (double)x * m[3] + (double)y * m[7] + (double)z * m[11] + m[15];
    if (w < D(0x5c88a0)) return 0;
    float inv = (float)(Fl(0x5c85b4) / w);
    double vz = ((double)x * m[2] + (double)y * m[6] + (double)z * m[10] + m[14]) * inv;
    float zf = (float)vz;
    if (vz > maxZ) return 0;
    double vx = ((double)x * m[0] + (double)y * m[4] + (double)z * m[8] + m[12]) * inv;
    float xf = (float)vx;
    if (limit < fabs(vx)) return 0;
    double vy = ((double)x * m[1] + (double)y * m[5] + (double)z * m[9] + m[13]) * inv;
    if (limit < fabs(vy)) return 0;
    if (out) {
        out[1] = (float)vy;
        out[0] = xf;
        out[2] = zf;
    }
    return 1;
}
RT3_REPLACE(0x5468c0, Render_ProjectClip);

} // namespace rt3
