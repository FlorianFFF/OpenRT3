// Site table dtor / vertex buffer slots / sub-viewports / texture stages / gradient quads in blocks 0x41..0x54 (batch 143). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Sites_DestroyAll(Collection* c);
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h);
float __fastcall Fade_Step_Impl();
void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);
void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Engine_QueueType6(void* e);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline void** Vt(void* p) { return *(void***)p; }
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }
static inline int32_t VbUnlock(void* vb) { return ((int32_t(__stdcall*)(void*))Vt(vb)[0x30 / 4])(vb); }
static inline uint8_t* Slot(uint8_t* r, int32_t i) { return r + 0x266b + i * 0x2d; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// 0x419680. Destructor of the site table (vtable 0x5c9718): removes all sites, frees
// the grid 0x62b2f8, then the collection.
void __fastcall Sites_Destruct(Collection* c) {
    F<uint32_t>(c, 0) = 0x5c9718;
    Sites_DestroyAll(c);
    if (hook::Global<void*>(0x62b2f8)) {
        Free(hook::Global<void*>(0x62b2f8));
        hook::Global<void*>(0x62b2f8) = nullptr;
    }
    Collection_Destruct(c);
}
RT3_REPLACE(0x419680, Sites_Destruct);

// 0x546a30. Unlocks all 64 vertex buffers (+0x31ab, locked flags in the slots;
// count 0xccd824) and relinks the slots (+0x266b, 0x2d bytes, count +0x2667): +0x14
// chains slots with the same key 1, +0x18 those with the same key 2, +0x1c = next.
void __fastcall Render_ResetVertexSlots(uint8_t* r) {
    for (int32_t k = 0; k < 0x40; ++k) {
        uint8_t* s = Slot(r, k);
        if (s[0x20]) {
            --hook::Global<int32_t>(0xccd824);
            VbUnlock(F<void*>(r, 0x31ab + k * 4));
        }
        s[0x20] = 0;
    }
    int32_t i = 0;
    for (; i < F<int32_t>(r, 0x2667); ++i) {
        uint8_t* s = Slot(r, i);
        for (int pass = 0; pass < 2; ++pass) {
            uint32_t link = pass ? 0x18 : 0x14, key = pass ? 4 : 0;
            if (F<int32_t>(s, link)) continue;
            int32_t want = F<int32_t>(s, key);
            F<int32_t>(s, link) = -1;
            uint8_t* prev = s;
            for (int32_t j = i + 1; j < F<int32_t>(r, 0x2667); ++j) {
                uint8_t* t = Slot(r, j);
                if (want != F<int32_t>(t, key)) continue;
                F<int32_t>(prev, link) = j;
                F<int32_t>(t, link) = -1;
                prev = t;
            }
        }
        F<int32_t>(s, 0x1c) = i + 1;
    }
    if (F<int32_t>(r, 0x2667) > 0) F<int32_t>(Slot(r, i - 1), 0x1c) = -1;
}
RT3_REPLACE(0x546a30, Render_ResetVertexSlots);

// 0x546b80. The vertex slot for (key1, key2): an existing one with room (+0x21 > 3)
// is returned; otherwise (or new) slot n = +0x2667 (< 64) is locked (data +0x32ab,
// also +0x29), set up with 0x5208 free and chained from the old one. 0 when full.
uint8_t* __fastcall Render_AcquireVertexSlot(uint8_t* r, int, int32_t key1, int32_t key2) {
    int32_t n = F<int32_t>(r, 0x2667);
    if (n >= 0x40) return nullptr;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* s = Slot(r, i);
        if (key1 != F<int32_t>(s, 0) || key2 != F<int32_t>(s, 4)) continue;
        int32_t* tail = &F<int32_t>(s, 0xc);
        int32_t t = *tail;
        uint8_t* ts = Slot(r, t);
        if (F<int32_t>(ts, 0x21) > 3) return ts;
        if (ts[0x20]) {
            --hook::Global<int32_t>(0xccd824);
            VbUnlock(F<void*>(r, 0x31ab + t * 4));
            ts[0x20] = 0;
        }
        F<int32_t>(ts, 0x10) = F<int32_t>(r, 0x2667);
        *tail = F<int32_t>(r, 0x2667);
        n = F<int32_t>(r, 0x2667);
        break;
    }
    uint8_t* s = Slot(r, n);
    if (!s[0x20]) {
        ++hook::Global<int32_t>(0xccd824);
        int32_t m = F<int32_t>(r, 0x2667);
        void* vb = F<void*>(r, 0x31ab + m * 4);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))Vt(vb)[0x2c / 4])(vb, 0, 0, r + 0x32ab + m * 4,
                                                                                            0x2800);
        Slot(r, F<int32_t>(r, 0x2667))[0x20] = 1;
    }
    int32_t m = F<int32_t>(r, 0x2667);
    s = Slot(r, m);
    F<int32_t>(s, 0) = key1;
    F<int32_t>(s, 4) = key2;
    F<int32_t>(s, 0xc) = m;
    F<int32_t>(s, 0x10) = -1;
    F<int32_t>(s, 0x21) = 0x5208;
    F<uint32_t>(s, 0x29) = F<uint32_t>(r, 0x32ab + m * 4);
    F<uint16_t>(s, 8) = 0xffff;
    F<uint16_t>(s, 0xa) = 0;
    F<int32_t>(r, 0x2667) = m + 1;
    return s;
}
RT3_REPLACE(0x546b80, Render_AcquireVertexSlot);

// Saves the device viewport into *slot (6 dwords) the first time.
static void SaveViewport(uint8_t* r, uint32_t slot) {
    if (F<void*>(r, slot)) return;
    void* vp = Malloc(0x18);
    F<void*>(r, slot) = vp;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa4 / 4])(d, vp);
}

// 0x5474a0. Sets the viewport to the screen fraction (x0, y0)-(x1, y1) (screen
// +0x25c7 x +0x25cb) and the world transform mapping it to clip space.
void __fastcall Render_SetSubViewport(uint8_t* r, int, float x0, float y0, float x1, float y1) {
    SaveViewport(r, 0x25e7);
    int32_t w = F<int32_t>(r, 0x25c7), h = F<int32_t>(r, 0x25cb);
    float hf = (float)h;
    float dx = (float)((double)x1 - x0), dy = (float)((double)y1 - y0);
    struct { int32_t x, y, w, h; float minZ, maxZ; } vp;
    vp.x = (int32_t)((double)x0 * w);
    vp.y = (int32_t)((double)hf * y0);
    vp.w = (int32_t)((double)dx * w);
    vp.h = (int32_t)((double)dy * hf);
    vp.minZ = 0.0f;
    vp.maxZ = 1.0f;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, &vp);
    float m[16] = {};
    double sx = D(0x5c85d0) / dx, sy = D(0x5c9be8) / dy;
    m[0] = (float)sx;
    m[5] = (float)sy;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[12] = (float)((double)Fl(0x5c9b08) - sx * x0);
    m[13] = (float)(-(sy * y0) + Fl(0x5c85b4));
    void* dev = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, const float*))Vt(dev)[0x94 / 4])(dev, 0x100, m);
}
RT3_REPLACE(0x5474a0, Render_SetSubViewport);

// 0x548180. Sets the viewport to the pixel rect (x0, y0)-(x1, y1) (the saved full
// one, +0x25e3, when under 2 pixels) with matching projection, and records the
// scale (+0x2523/+0x252b) and centre offset (+0x2527/+0x252f) against the screen.
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    SaveViewport(r, 0x25e3);
    int32_t sw = F<int32_t>(r, 0x25c7), sh = F<int32_t>(r, 0x25cb);
    int32_t w = x1 - x0, h = y1 - y0;
    struct { int32_t x, y, w, h; float minZ, maxZ; } vp = { x0, y0, w, h, 0.0f, 1.0f };
    F<float>(r, 0x2523) = (float)((double)sw / (double)(uint32_t)w);
    F<float>(r, 0x252b) = (float)((double)sh / (double)(uint32_t)h);
    int32_t hw = sw / 2, cx = (x1 + x0) / 2, hh = sh / 2, cy = (y1 + y0) / 2;
    F<float>(r, 0x2527) = (float)(((double)hw - cx) / hw);
    F<float>(r, 0x252f) = (float)(((double)hh - cy) / hh);
    if (w < 2 || h < 2) vp = *(decltype(vp)*)F<void*>(r, 0x25e3);
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, &vp);
    Render_SetProjection(r, 0, vp.w, vp.h);
}
RT3_REPLACE(0x548180, Render_SetPixelViewport);

// 0x548ae0. Sets up n texture stages once (+0x258f; bound textures +0x2593, 6-dword
// states +0x2597 = {1, 2, 1, 1, 2, 1}) and the two draw lists (+0x259f, +0x259b).
void __fastcall Render_InitStages(uint8_t* r, int, int32_t n) {
    if (F<int32_t>(r, 0x258f)) return;
    F<int32_t>(r, 0x258f) = n;
    F<void*>(r, 0x2593) = Malloc(n * 4);
    F<void*>(r, 0x2597) = Malloc(n * 24);
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) {
        int32_t* st = (int32_t*)(F<uint8_t*>(r, 0x2597) + i * 24);
        st[0] = 1;
        st[1] = 2;
        st[2] = 1;
        if (i < F<int32_t>(r, 0x258f)) {
            st[3] = 1;
            st[4] = 2;
            st[5] = 1;
        }
        F<int32_t*>(r, 0x2593)[i] = 0;
        if (i < F<int32_t>(r, 0x258f)) {
            if (F<int32_t*>(r, 0x2593)[i]) {
                ++F<int32_t>(r, 0x370d);
                void* d = Dev(r);
                ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0xf4 / 4])(d, i, nullptr);
            }
            F<int32_t*>(r, 0x2593)[i] = 0;
        }
    }
    auto newList = []() -> PtrList* {
        void* p = ((void*(__cdecl*)(size_t))0x53b070)(0x18);
        return p ? PtrList_Init((PtrList*)p) : nullptr;
    };
    if (!F<PtrList*>(r, 0x259f)) F<PtrList*>(r, 0x259f) = newList();
    if (!F<PtrList*>(r, 0x259b)) F<PtrList*>(r, 0x259b) = newList();
}
RT3_REPLACE(0x548ae0, Render_InitStages);

// 0x548c50. Frees the draw lists (and their items) and the texture stages, unbinding
// any bound textures (counting +0x370d).
void __fastcall Render_FreeStages(uint8_t* r) {
    if (!F<int32_t>(r, 0x258f)) return;
    static const uint32_t lists[2] = { 0x259f, 0x259b };
    for (uint32_t off : lists) {
        PtrList_Rewind(F<PtrList*>(r, off));
        for (void* p = PtrList_Next(F<PtrList*>(r, off)); p; p = PtrList_Next(F<PtrList*>(r, off))) Free(p);
        if (PtrList* l = F<PtrList*>(r, off)) {
            PtrList_Destroy(l);
            ((void(__cdecl*)(void*))0x53b080)(l);
        }
        F<PtrList*>(r, off) = nullptr;
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) {
        if (F<int32_t*>(r, 0x2593)[i]) {
            ++F<int32_t>(r, 0x370d);
            void* d = Dev(r);
            ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0xf4 / 4])(d, i, nullptr);
        }
        F<int32_t*>(r, 0x2593)[i] = 0;
        F<int32_t*>(r, 0x2593)[i] = 0;
    }
    F<int32_t>(r, 0x258f) = 0;
    Free(F<void*>(r, 0x2593));
    Free(F<void*>(r, 0x2597));
}
RT3_REPLACE(0x548c50, Render_FreeStages);

// 0x54fed0. Queues a gradient quad (x, y) x 4 with corner colours (c0, c1, c2, c3)
// as two faded triangles; `clip` brackets them with the clip rect (cx0..cy1).
RT3_NOINLINE void __fastcall Engine_QueueGradientQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1,
                                         uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
                                         uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0,
                                         uint32_t cx1, uint32_t cy1) {
    const uint32_t one = 0x3f800000;
    if (clip) Engine_QueueType5(hook::Global<void*>(0x6d4024), 0, cx0, cy0, cx1, cy1);
    float f = Fade_Step_Impl();
    Engine_QueueQuad(hook::Global<void*>(0x6d4024), 0, x0, y0, x1, y1, x2, y2, Bits(f), c0, c1, c2, 0, 0, 0, one,
                     one, one, 0, one, one, one);
    f = Fade_Step_Impl();
    Engine_QueueQuad(hook::Global<void*>(0x6d4024), 0, x2, y2, x3, y3, x0, y0, Bits(f), c2, c3, c0, 0, 0, 0, one,
                     one, one, 0, one, one, one);
    if (clip) Engine_QueueType6(hook::Global<void*>(0x6d4024));
}
RT3_REPLACE(0x54fed0, Engine_QueueGradientQuad);

} // namespace rt3
