// Camera viewport / renderer shutdown / particle spawn / text cursor moves / list row refresh / WAV loading in blocks 0x54..0x56 (batch 144). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Camera_BoundsPixels(uint8_t* c, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Render_FreeStages(uint8_t* r);
void __fastcall Render_ReleaseBuffers(uint8_t* r);
void __fastcall Render_ReleaseTextures(uint8_t* r);
const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall IsSingleByte(uint16_t c);
int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
int32_t __fastcall NameTable_Contains(uint8_t* t, int, int32_t id, int32_t fatal);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);

static inline void** Vt(void* p) { return *(void***)p; }
static inline int32_t Release(void* p) { return ((int32_t(__stdcall*)(void*))Vt(p)[2])(p); }

// 0x5482e0. Sets the viewport (or, with projOnly, just the projection) to the
// camera's pixel bounds.
void __fastcall Render_SetCameraViewport(uint8_t* r, int, uint8_t* cam, uint8_t projOnly) {
    int32_t x0, y0, x1, y1;
    Camera_BoundsPixels(cam, 0, &x0, &y0, &x1, &y1);
    if (!projOnly) Render_SetPixelViewport(r, 0, x0, y0, x1, y1);
    else Render_SetProjection(r, 0, x1 - x0, y1 - y0);
}
RT3_REPLACE(0x5482e0, Render_SetCameraViewport);

// 0x54a190. Shuts the renderer down: stages, the shadow texture (+0x33ab),
// buffers, textures, saved viewports, then the device (+0x25df) and Direct3D
// (+0x25db); fatal (strings 0x9b/0x9c) when still referenced. Returns 0 (-1 when
// the device leaks).
int32_t __fastcall Render_Shutdown(uint8_t* r) {
    Render_FreeStages(r);
    if (void* p = F<void*>(r, 0x33ab))
        if (Release(p)) Fatal_Exit(GetString(0x9b));
    Render_ReleaseBuffers(r);
    Render_ReleaseTextures(r);
    if (F<void*>(r, 0x25e3)) Free(F<void*>(r, 0x25e3));
    if (F<void*>(r, 0x25e7)) Free(F<void*>(r, 0x25e7));
    if (void* dev = F<void*>(r, 0x25df)) {
        if (Release(dev)) {
            if (void* d3d = F<void*>(r, 0x25db)) Release(d3d);
            Fatal_Exit(GetString(0x9b));
            return -1;
        }
    }
    if (void* d3d = F<void*>(r, 0x25db))
        if (Release(d3d)) Fatal_Exit(GetString(0x9c));
    F<void*>(r, 0x2643) = nullptr;
    F<void*>(r, 0x25df) = nullptr;
    F<void*>(r, 0x25db) = nullptr;
    F<void*>(hook::Global<uint8_t*>(0x6d4024), 8) = nullptr;
    return 0;
}
RT3_REPLACE(0x54a190, Render_Shutdown);

// 0x554b60. Spawns a particle for emitter e from the shared pool (0x6d4030 +0x14:
// {free list, ..., total +0xc, node size +0x10}; only with over 20% free unless
// flag 0x8000): pushed on the emitter's list (+0, count +0x34) with the emitter's
// colour (+0x48, jittered by the render LCG 0x624324 with flag 2), box +0xe0..+0xec,
// flags +0xac and size +0x98. Returns 1 if spawned.
int32_t __fastcall Emitter_SpawnParticle(uint8_t* e) {
    uint8_t* pool = F<uint8_t*>(hook::Global<uint8_t*>(0x6d4030), 0x14);
    uint32_t** freeList = F<uint32_t**>(pool, 0);
    double ratio = (double)(int32_t)((uint32_t*)freeList)[2] / F<int32_t>(pool, 0xc);
    if (!(ratio > *(const float*)0x5ddcb0) && !(ratio != ratio) && !(F<uint32_t>(e, 0xac) & 0x8000)) return 0;
    uint32_t* node;
    if (F<int32_t>(pool, 0xc)) {
        uint32_t* l = (uint32_t*)F<void*>(pool, 0);
        node = (uint32_t*)l[0];
        if (!node) return 0;
        uint32_t next = node[0];
        l[0] = next;
        if ((uint32_t)node == l[1]) l[1] = next;
        --l[2];
    } else {
        node = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(F<uint32_t>(pool, 0x10));
    }
    if (!node) return 0;
    uint32_t* alive = F<uint32_t*>(e, 0);
    ++F<int32_t>(e, 0x34);
    uint32_t color = F<uint32_t>(e, 0x48);
    node[0] = alive[0];
    alive[0] = (uint32_t)node;
    ++alive[2];
    if (!alive[1]) alive[1] = (uint32_t)node;
    if (F<uint32_t>(e, 0xac) & 2) {
        uint32_t& seed = hook::Global<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        int32_t d = (int8_t)(uint8_t)((seed & 0x3f) - 0x1f);
        int32_t c[3] = { (int32_t)((color >> 16) & 0xff) + d, (int32_t)((color >> 8) & 0xff) + d, (int32_t)(color & 0xff) + d };
        for (int k = 0; k < 3; ++k) c[k] = c[k] > 0xff ? 0xff : (c[k] < 0 ? 0 : c[k]);
        color = ((uint32_t)c[0] << 16) | ((uint32_t)c[1] << 8) | (uint32_t)c[2];
    }
    uint8_t* p = (uint8_t*)node[1];
    F<uint32_t>(p, 0x70) = color & 0xffffff;
    F<uint32_t>(p, 0x2c) = F<uint32_t>(e, 0xe0);
    F<uint32_t>(p, 0x30) = F<uint32_t>(e, 0xe4);
    F<uint32_t>(p, 0x34) = F<uint32_t>(e, 0xe8);
    F<uint32_t>(p, 0x38) = F<uint32_t>(e, 0xec);
    F<uint32_t>(p, 0x5c) = F<uint32_t>(e, 0xac);
    F<uint32_t>(p, 0x60) = F<uint32_t>(e, 0x98);
    return 1;
}
RT3_REPLACE(0x554b60, Emitter_SpawnParticle);

// 0x55b750. Moves the text box's cursor (+0xd8) by delta characters in str,
// stepping over DBCS characters (0x6d4034, code page 0x624328) and, with flag
// 0x4000000 (+0x33), over whole [bracketed] tags.
void __fastcall TextBox_MoveCursor(uint8_t* e, int, const char* str, int32_t delta) {
    int32_t count = delta < 0 ? -delta : delta;
    int32_t step = delta > 0 ? 1 : -1;
    int32_t inTag = 0;
    int32_t len = 0;
    while (str[len]) ++len;
    while (count > 0 || inTag) {
        int32_t pos = F<int32_t>(e, 0xd8);
        int32_t np = pos + step;
        if (np < 0 || np > len) break;
        if ((F<uint32_t>(e, 0x33) & 0x4000000) && step > 0) {
            char ch = str[pos];
            if (ch == '[') inTag = 1;
            if (ch == ']') inTag = 0;
        }
        if (hook::Global<int32_t>(0x6d4034) && (step < 0 ? -step : step) <= 1) {
            const char* cur = str + pos;
            const char* q;
            uint16_t cp = hook::Global<uint16_t>(0x624328);
            if (step == 1) q = Import<const char*(__stdcall*)(uint16_t, const char*, uint32_t)>(0x5c8244)(cp, cur, 0);
            else q = Import<const char*(__stdcall*)(uint16_t, const char*, const char*, uint32_t)>(0x5c8254)(cp, str, cur, 0);
            int32_t d = (int32_t)(q - cur);
            if ((d < 0 ? -d : d) <= 2) F<int32_t>(e, 0xd8) += d;
            else F<int32_t>(e, 0xd8) += step;
        } else {
            F<int32_t>(e, 0xd8) = np;
        }
        if ((step < 0 ? -step : step) > 2 && F<int32_t>(e, 0xd8) > 0) {
            int32_t at = F<int32_t>(e, 0xd8);
            if (IsSingleByte((uint16_t)(int16_t)(int8_t)str[at])) F<int32_t>(e, 0xd8) += step > 0 ? 1 : -1;
        }
        if ((F<uint32_t>(e, 0x33) & 0x4000000) && step < 0) {
            char ch = str[F<int32_t>(e, 0xd8)];
            if (ch == '[') inTag = 0;
            if (ch == ']') inTag = 1;
        }
        --count;
    }
}
RT3_REPLACE(0x55b750, TextBox_MoveCursor);

// 0x560e50. Refreshes the list's visible rows: releases the old ones (callback
// +0x1c6 per row item), takes +0x1de rows from +0x1e2 (first node +0x18a) and lays
// each out (callback +0x1ca with its rect, row height +0xb6 of the screen + +0xb2).
void __fastcall ListView_RefreshVisible(uint8_t* tv) {
    if (F<int32_t>(tv, 0x182)) return;
    if (!F<void*>(tv, 0x1ca) && !F<void*>(tv, 0x1c6)) return;
    for (int32_t i = 0; i < F<int32_t>(tv, 0x186); ++i)
        ((void(__fastcall*)(uint8_t*, int32_t, uint32_t))F<void*>(tv, 0x1c6))(tv, F<int32_t>(tv, 0x18a) + i,
                                                                              F<uint32_t*>(tv, 0x18e)[i]);
    int32_t n = F<int32_t>(tv, 0x1de);
    F<int32_t>(tv, 0x186) = n;
    F<int32_t>(tv, 0x18a) = n ? TreeView_NodeOfRow(tv, 0, F<int32_t>(tv, 0x1e2)) : -1;
    if (F<void*>(tv, 0x18e)) Free(F<void*>(tv, 0x18e));
    if (!F<int32_t>(tv, 0x1de)) {
        F<void*>(tv, 0x18e) = nullptr;
        return;
    }
    uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
    int32_t screenH = F<int32_t>(F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15, 4);
    int32_t rowH;
    float rowF = F<float>(tv, 0xb6);
    __asm {
        fld rowF
        fimul screenH
        mov eax, 0x5a10d0
        call eax
        mov rowH, eax
    }
    float step = (float)(((double)rowH + *(const double*)0x5d0250) / screenH);
    F<uint32_t*>(tv, 0x18e) = (uint32_t*)Malloc(F<int32_t>(tv, 0x1de) * 4);
    for (int32_t i = 0; i < F<int32_t>(tv, 0x1de); ++i) {
        float x0 = (float)((double)F<float>(tv, 0xb) + F<float>(tv, 0xa6));
        double t = ((double)step + F<float>(tv, 0xb2)) * i;
        float y0 = (float)((double)F<float>(tv, 0xaa) + F<float>(tv, 0xf) + t);
        double x1 = (double)F<float>(tv, 0xae) + F<float>(tv, 0xb) + F<float>(tv, 0xa6);
        double y1 = (double)F<float>(tv, 0xaa) + F<float>(tv, 0xf) + t + F<float>(tv, 0xb2);
        int32_t node = TreeView_NodeOfRow(tv, 0, F<int32_t>(tv, 0x1e2) + i);
        float h = (float)(y1 - y0);
        uint32_t item = F<uint32_t>(F<uint8_t*>(tv, 0x176) + node * 32, 8);
        F<uint32_t*>(tv, 0x18e)[i] = item;
        float w = (float)(x1 - x0);
        ((void(__fastcall*)(uint8_t*, int32_t, uint32_t, float, float, float, float))F<void*>(tv, 0x1ca))(
            tv, F<int32_t>(tv, 0x18a) + i, F<uint32_t*>(tv, 0x18e)[i], x0, y0, w, h);
    }
}
RT3_REPLACE(0x560e50, ListView_RefreshVisible);

// 0x5641f0. Loads WAV file `name` (or, when unknown, id `name` itself) from the
// packs into a new buffer, parsing its header into 0xd9798c; *size = its size.
void* __stdcall Sound_LoadWav(const char* name, int32_t, int32_t* size) {
    uint8_t* t = hook::Global<uint8_t*>(0x6d4020);
    int32_t id = NameTable_Contains(t, 0, NameTable_Hash(t, 0, name), 1)
                     ? NameTable_Hash(hook::Global<uint8_t*>(0x6d4020), 0, name)
                     : (int32_t)name;
    int32_t n = NameTable_EntrySize(hook::Global<uint8_t*>(0x6d4020), 0, id);
    NameTable_Seek(hook::Global<uint8_t*>(0x6d4020), 0, id, 0);
    void* p = Malloc(n);
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, p, n);
    Import<int32_t(__stdcall*)(const void*, void*)>(0x5c83d4)(p, (void*)0xd9798c);
    if (size) *size = n;
    return p;
}
RT3_REPLACE(0x5641f0, Sound_LoadWav);

} // namespace rt3
