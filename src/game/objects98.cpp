// Vertex/index buffer locks and draw calls in blocks 0x52..0x54 (batch 101). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

float __fastcall Render_GetLightValue(uint8_t* r, int, int32_t i, int32_t j);

static inline int32_t& LockCount() { return hook::Global<int32_t>(0xccd824); }
static inline int32_t Lock(void* b, uint32_t off, uint32_t size, void* pp, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))(*(void***)b)[0x2c / 4])(b, off, size, pp, flags);
}
static inline int32_t Unlock(void* b) { return ((int32_t(__stdcall*)(void*))(*(void***)b)[0x30 / 4])(b); }
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }

float __fastcall Scene_GetLightValue0(uint8_t* s, int, int32_t i) { return Render_GetLightValue(F<uint8_t*>(s, 0x2d), 0, i, 0); }
RT3_REPLACE(0x527540, Scene_GetLightValue0);

// 0x5459f0. Unlocks the dynamic vertex buffer (+0x2643) if locked.
void __fastcall Render_UnlockVbRange(uint8_t* r) {
    if (!r[0x254e]) return;
    r[0x254e] = 0;
    LockCount()--;
    Unlock(F<void*>(r, 0x2643));
}
RT3_REPLACE(0x5459f0, Render_UnlockVbRange);

// Fixed buffers: +0x264f (0x7800 bytes), +0x2653 / +0x264b (0x3e80), +0x263f (0x2ee00).
int32_t __fastcall Render_UnlockVb264f(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x264f));
}
RT3_REPLACE(0x545a20, Render_UnlockVb264f);
void* __fastcall Render_LockVb264f(uint8_t* r) {
    LockCount()++;
    Lock(F<void*>(r, 0x264f), 0, 0x7800, r + 0x2657, 0x2800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x545a40, Render_LockVb264f);
void* __fastcall Render_LockVb2653(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x2653), 0, 0x3e80, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x545a70, Render_LockVb2653);
int32_t __fastcall Render_UnlockVb2653(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x2653));
}
RT3_REPLACE(0x545aa0, Render_UnlockVb2653);
void* __fastcall Render_LockVb264b(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x264b), 0, 0x3e80, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x545ac0, Render_LockVb264b);
int32_t __fastcall Render_UnlockVb264b(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x264b));
}
RT3_REPLACE(0x545af0, Render_UnlockVb264b);

// 0x545b10 / 0x545b50. Locks records first..first+count (24 bytes) of buffer +0x2647
// (discard / no-overwrite).
void* __fastcall Render_LockVb2647(uint8_t* r, int, int32_t first, int32_t count) {
    void* p;
    Lock(F<void*>(r, 0x2647), first * 24, count * 24, &p, 0x2800);
    LockCount()++;
    return p;
}
RT3_REPLACE(0x545b10, Render_LockVb2647);
void* __fastcall Render_AppendVb2647(uint8_t* r, int, int32_t first, int32_t count) {
    Lock(F<void*>(r, 0x2647), first * 24, count * 24, r + 0x2657, 0x1800);
    LockCount()++;
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x545b50, Render_AppendVb2647);
int32_t __fastcall Render_UnlockVb2647(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x2647));
}
RT3_REPLACE(0x545b90, Render_UnlockVb2647);
void* __fastcall Render_LockVb263f(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x263f), 0, 0x2ee00, &p, 0x800);
    return p;
}
RT3_REPLACE(0x545bb0, Render_LockVb263f);
int32_t __fastcall Render_UnlockVb263f(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x263f));
}
RT3_REPLACE(0x545be0, Render_UnlockVb263f);

// 0x545c00. Indexed triangle list of `indices` indices (DrawIndexedPrimitive),
// counting triangles in +0x3711.
void __fastcall Render_DrawIndexed(uint8_t* r, int, int32_t indices, uint32_t minIndex, uint32_t vertices) {
    int32_t tri = indices / 3;
    if (tri < 1) return;
    F<int32_t>(r, 0x3711) += tri;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x11c / 4])(
        d, 4, minIndex, vertices, 0, tri);
}
RT3_REPLACE(0x545c00, Render_DrawIndexed);

static inline int32_t DrawPrimitive(void* d, uint32_t type, uint32_t first, uint32_t count) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x118 / 4])(d, type, first, count);
}
// 0x545c50. Triangle list of vertices first..end.
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end) {
    int32_t tri = (end - first) / 3;
    if (tri < 1) return;
    F<int32_t>(r, 0x3711) += tri;
    DrawPrimitive(Dev(r), 4, first, tri);
}
RT3_REPLACE(0x545c50, Render_DrawTriangles);

// 0x545ca0. Untextured point list of vertices first..end (point size 1).
int32_t __fastcall Render_DrawPoints(uint8_t* r, int, int32_t first, int32_t end) {
    if (!r[0x254d] && F<int32_t>(r, 0x258f) > 0) {
        if (*F<int32_t*>(r, 0x2593)) {
            F<int32_t>(r, 0x370d)++;
            void* d = Dev(r);
            ((int32_t(__stdcall*)(void*, uint32_t, void*))(*(void***)d)[0xf4 / 4])(d, 0, nullptr);
        }
        *F<int32_t*>(r, 0x2593) = 0;
    }
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, 0x9e, 0x3f800000);
    int32_t n = end - first;
    F<int32_t>(r, 0x3711) += n / 3;
    return DrawPrimitive(Dev(r), 1, first, n);
}
RT3_REPLACE(0x545ca0, Render_DrawPoints);

int32_t __fastcall Render_TriangleCount(uint8_t* r) { return F<int32_t>(r, 0x3711); }
RT3_REPLACE(0x545d50, Render_TriangleCount);

} // namespace rt3
