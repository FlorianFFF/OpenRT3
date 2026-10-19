// Renderer transforms / clears / tile seams / mouse release in blocks 0x53..0x54 (batch 97). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void __fastcall Texture_DropTarget(uint8_t* t);
void __fastcall Mouse_Unacquire(uint8_t* m);

static inline void Release(void* com) { ((uint32_t(__stdcall*)(void*))(*(void***)com)[2])(com); }

// D3DX8 statics in the exe.
static inline void MatMul(void* out, const void* a, const void* b) {
    ((void*(__stdcall*)(void*, const void*, const void*))0x56d3cd)(out, a, b);
}
static inline void MatTranspose(void* out, const void* m) { ((void*(__stdcall*)(void*, const void*))0x56d616)(out, m); }
static inline void MatInverse(void* out, float* det, const void* m) {
    ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(out, det, m);
}

// Device +0x25df (IDirect3DDevice8): SetTransform (vtbl +0x94), Clear (+0x90).
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline int32_t SetTransform(uint8_t* r, int32_t state, void* m) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, int32_t, void*))(*(void***)d)[0x94 / 4])(d, state, m);
}
static inline int32_t Clear(void* d, uint32_t flags, uint32_t colour, uint32_t z) {
    return ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x90 / 4])(
        d, 0, nullptr, flags, colour, z, 0);
}

// 0x532860. Fixes the seams between the 12 terrain overview tiles (+8.., from
// +0x155c, 4 per row): each tile's last row gets the first row of the tile below
// and (except in the last column) its last column the first column of the tile to the right.
void __fastcall Render_FixTileSeams(uint8_t* r) {
    for (int32_t k = 0; k < 0xc; ++k) {
        int32_t w, h;
        uint32_t* p = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 8), 0, &w, &h, 0);
        const uint32_t* below = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 0x18), 0, &w, &h, 0);
        uint32_t* row = p + (h - 1) * w;
        for (int32_t j = 0; j < w; ++j) row[j] = below[j];
        if (k % 4 != 3) {
            const uint32_t* right = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 0xc), 0, &w, &h, 0);
            for (int32_t i = 0; i < h; ++i) p[(i + 1) * w - 1] = right[i * w];
        }
        Texture_DropTarget(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 8));
    }
}
RT3_REPLACE(0x532860, Render_FixTileSeams);

uint32_t __fastcall Input_Flag800() {
    uint8_t* in = hook::Global<uint8_t*>(0x6d4018);
    return in ? F<uint32_t>(in, 0xa8c) & 0x800 : 0;
}
RT3_REPLACE(0x543e40, Input_Flag800);

// 0x543e60. Releases the mouse device (+4) and DirectInput (+0).
void __fastcall Mouse_Destroy(uint8_t* m) {
    Mouse_Unacquire(m);
    Release(F<void*>(m, 4));
    Release(F<void*>(m, 0));
}
RT3_REPLACE(0x543e60, Mouse_Destroy);

int32_t __fastcall Render_ClearAll(uint8_t* r) { return Clear(Dev(r), 3, 0, 0x3f800000); }
RT3_REPLACE(0x543e80, Render_ClearAll);

// 0x543ea0. Clears target + z (with the background colour +0x25a3 when asked) or only z.
int32_t __fastcall Render_Clear(uint8_t* r, int, uint8_t target, uint8_t coloured, uint32_t z) {
    if (!target) return Clear(Dev(r), 2, 0, 0x3f800000);
    uint32_t c = coloured ? F<uint32_t>(r, 0x25a3) & 0xffffff : 0;
    return Clear(Dev(r), 3, c, z);
}
RT3_REPLACE(0x543ea0, Render_Clear);

void __fastcall Render_SetViewOffset(uint8_t* r, int, uint32_t x, uint32_t y) {
    F<uint32_t>(r, 0x25ef) = x;
    F<uint32_t>(r, 0x25f3) = y;
}
RT3_REPLACE(0x544270, Render_SetViewOffset);

// 0x544290. Off-centre perspective projection (+0x352f; near 8, far 65535) for a
// w x h viewport, offset +0x25ef/+0x25f3 and scale +0x25eb.
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h) {
    float inv = (float)((double)*(const float*)0x5c85b4 / ((double)w / h));
    const double k = *(const float*)0x5c9690, s = F<float>(r, 0x25eb);
    double a = inv * k;
    double b = (double)inv * F<float>(r, 0x25f3);
    float top = (float)((b + a) * s);
    float bottom = (float)((b - a) * s);
    float right = (float)(((double)F<float>(r, 0x25ef) + k) * s);
    float left = (float)(((double)F<float>(r, 0x25ef) - k) * s);
    ((void*(__stdcall*)(void*, float, float, float, float, float, float))0x56e41a)(r + 0x352f, left, right, bottom,
                                                                                   top, 8.0f, 65535.0f);
    F<float>(r, 0x36f9) = (float)((double)inv * F<float>(F<uint8_t*>(r, 0x83c), 0x49));
    SetTransform(r, 3, r + 0x352f);
}
RT3_REPLACE(0x544290, Render_SetProjection);

// 0x544360 / 0x5443b0. World matrix +0x34ef: identity / the object's matrix (+0xce).
void __fastcall Render_WorldIdentity(uint8_t* r) {
    float* m = (float*)(r + 0x34ef);
    for (int k = 0; k < 16; ++k) F<uint32_t>(m, k * 4) = 0;
    F<uint32_t>(r, 0x352b) = 0x3f800000;
    F<uint32_t>(r, 0x3517) = 0x3f800000;
    F<uint32_t>(r, 0x3503) = 0x3f800000;
    F<uint32_t>(m, 0) = 0x3f800000;
    SetTransform(r, 0x100, m);
}
RT3_REPLACE(0x544360, Render_WorldIdentity);
void __fastcall Render_WorldFrom(uint8_t* r, int, uint8_t* obj) {
    for (int k = 0; k < 16; ++k) F<uint32_t>(r, 0x34ef + k * 4) = F<uint32_t>(obj, 0xce + k * 4);
    SetTransform(r, 0x100, r + 0x34ef);
}
RT3_REPLACE(0x5443b0, Render_WorldFrom);

// 0x5443f0. Pushes the object's local matrix (+0x14e) on the matrix stack (+0x33ab):
// +0x10e = the stack top before, +0xce = after.
void __fastcall Render_ApplyLocal(uint8_t* r, int, uint8_t* obj) {
    void* ms = F<void*>(r, 0x33ab);
    const uint32_t* top = ((uint32_t*(__stdcall*)(void*))(*(void***)ms)[0x44 / 4])(ms);
    for (int k = 0; k < 16; ++k) F<uint32_t>(obj, 0x10e + k * 4) = top[k];
    ms = F<void*>(r, 0x33ab);
    ((int32_t(__stdcall*)(void*, void*))(*(void***)ms)[0x18 / 4])(ms, obj + 0x14e);
    ms = F<void*>(r, 0x33ab);
    top = ((uint32_t*(__stdcall*)(void*))(*(void***)ms)[0x44 / 4])(ms);
    for (int k = 0; k < 16; ++k) F<uint32_t>(obj, 0xce + k * 4) = top[k];
}
RT3_REPLACE(0x5443f0, Render_ApplyLocal);

uint8_t __fastcall Render_TakeFlag36f7(uint8_t* r, int, uint8_t clear) {
    uint8_t v = r[0x36f7];
    if (clear) r[0x36f7] = 0;
    return v;
}
RT3_REPLACE(0x544450, Render_TakeFlag36f7);

// 0x544470. Activates projection +0x356f / view +0x362f: combined matrix +0x33af
// (copied to +0x33ef), its transpose +0x35af and inverse +0x342f.
void __fastcall Render_UseView2(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x356f);
    SetTransform(r, 2, r + 0x362f);
    MatMul(m, r + 0x34ef, r + 0x362f);
    MatMul(r + 0x33af, m, r + 0x356f);
    MatTranspose(r + 0x35af, r + 0x33af);
    MatInverse(r + 0x342f, &det, r + 0x33af);
    for (int k = 0; k < 16; ++k) F<uint32_t>(r, 0x33ef + k * 4) = F<uint32_t>(r, 0x33af + k * 4);
}
RT3_REPLACE(0x544470, Render_UseView2);

// 0x544500. Activates projection +0x352f / view +0x35ef: combined +0x33af and its
// inverse +0x342f, plus the inverse of view x projection (+0x366f).
void __fastcall Render_UseView1(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x352f);
    SetTransform(r, 2, r + 0x35ef);
    MatMul(m, r + 0x34ef, r + 0x35ef);
    MatMul(r + 0x33af, m, r + 0x352f);
    MatInverse(r + 0x342f, &det, r + 0x33af);
    MatMul(m, r + 0x35ef, r + 0x352f);
    MatInverse(r + 0x366f, &det, m);
}
RT3_REPLACE(0x544500, Render_UseView1);

// 0x5445a0. Activates projection +0x352f / view +0x346f (combined, transpose, inverse).
void __fastcall Render_UseView3(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x352f);
    SetTransform(r, 2, r + 0x346f);
    MatMul(m, r + 0x34ef, r + 0x346f);
    MatMul(r + 0x33af, m, r + 0x352f);
    MatTranspose(r + 0x35af, r + 0x33af);
    MatInverse(r + 0x342f, &det, r + 0x33af);
}
RT3_REPLACE(0x5445a0, Render_UseView3);

void __fastcall Render_SetVec2533(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c) {
    F<uint32_t>(r, 0x2533) = a;
    F<uint32_t>(r, 0x2537) = b;
    F<uint32_t>(r, 0x253b) = c;
}
RT3_REPLACE(0x544630, Render_SetVec2533);

} // namespace rt3
