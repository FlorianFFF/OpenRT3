// Draw trace for the determinism harness (src/harness.cpp, tools/simcheck.py).
//
// The saved game only shows the simulation; drawing code (positions, colours,
// texture choices, UI layout) can differ without changing a byte of it. This
// wraps the Direct3D 8 device and folds everything the game hands it into one
// CRC per main-loop frame, logged as "D <frame> <crc> <draws>": render and
// texture-stage states, transforms, viewport, material, lights, constants, the
// vertices and indices each draw call reads, and which texture or buffer is
// bound (by order of creation, since its address differs between runs).
//
// It only reads what the game passes and always calls the real method with the
// same arguments, so the game behaves the same with it. Its own memory comes
// from a private heap, leaving the game's heap layout alone.
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <intrin.h>
#include "core/log.h"

namespace rt3 { uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len); }

namespace drawtrace {

static HANDLE g_heap;
static uint32_t g_crc, g_draws, g_frame;
// Also one CRC per kind of data, logged after the total, to see which kind a
// difference is in: 0 states, 1 transforms and constants, 2 bindings and draw
// arguments, 3 vertices, 4 indices, 5 vertices passed directly (UP draws).
static uint32_t g_part[6];
static int g_kind;

// RT3RE_DRAWDEBUG=<frame>: also logs each call's arguments in that frame ("B ...", data "V ...").
static long g_debugFrame = -1, g_curFrame = 1;

static void Mix(const void* p, uint32_t n) {
    if (!p || !n) return;
    uint32_t c = rt3::Crc32((const uint8_t*)p, n);
    if (g_curFrame == g_debugFrame && g_kind >= 3) {  // the data itself, as hex
        static char hex[2 * 4096 + 1];
        uint32_t m = n < 4096 ? n : 4096;
        for (uint32_t i = 0; i < m; ++i) wsprintfA(hex + 2 * i, "%02x", ((const uint8_t*)p)[i]);
        hex[2 * m] = 0;
        rt3::Log("V %d %lu %08lx %s", g_kind, n, c, hex);
    }
    uint32_t pair[2] = { g_crc, c };
    g_crc = rt3::Crc32((const uint8_t*)pair, 8);
    pair[0] = g_part[g_kind];
    g_part[g_kind] = rt3::Crc32((const uint8_t*)pair, 8);
}
static void Mix4(uint32_t tag, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0) {
    if (g_curFrame == g_debugFrame) rt3::Log("B %lu %lu %lu %lu", tag, a, b, c);
    uint32_t v[4] = { tag, a, b, c };
    Mix(v, sizeof v);
}

// --- objects by order of first use ---------------------------------------------
struct Slot { void* key; uint32_t id; uint8_t* shadow; uint32_t size; uint32_t lockOff, lockSize; uint8_t* lockPtr; };
static Slot g_slots[16384];
static uint32_t g_nextId = 1;

static Slot* Get(void* key) {
    if (!key) return nullptr;
    uint32_t h = ((uint32_t)(uintptr_t)key >> 4) * 2654435761u;
    for (uint32_t i = 0; i < 16384; ++i) {
        Slot* s = &g_slots[(h + i) & 16383];
        if (s->key == key) return s;
        if (!s->key) {
            s->key = key;
            s->id = 0;  // numbered only when created (Created)
            return s;
        }
    }
    return nullptr;
}
// A new object (created just now): a fresh id, even at a freed one's address.
static void Created(void* key) {
    if (Slot* s = Get(key)) s->id = g_nextId++;
}
static uint32_t Id(void* key) {
    Slot* s = Get(key);
    return s ? s->id : 0;
}

// --- buffers: a shadow copy of what the game wrote, read at draw time ------------
// Index and vertex buffers share the layout we use: Lock 11, Unlock 12,
// GetDesc 13 with the size at +16 of the description.
typedef int32_t(__stdcall* LockFn)(void*, uint32_t, uint32_t, uint8_t**, uint32_t);
typedef int32_t(__stdcall* UnlockFn)(void*);
static LockFn g_vbLock, g_ibLock;
static UnlockFn g_vbUnlock, g_ibUnlock;

static uint32_t BufferSize(void* b) {
    uint32_t desc[8] = {};
    ((int32_t(__stdcall*)(void*, void*))(*(void***)b)[13])(b, desc);
    return desc[4];
}

static void OnLock(void* b, uint32_t off, uint32_t size, uint8_t* p) {
    Slot* s = Get(b);
    if (!s || !p) return;
    uint32_t total = BufferSize(b);
    if (s->size != total) {
        if (s->shadow) HeapFree(g_heap, 0, s->shadow);
        s->shadow = (uint8_t*)HeapAlloc(g_heap, HEAP_ZERO_MEMORY, total ? total : 1);
        s->size = total;
    }
    s->lockOff = off;
    s->lockSize = size ? size : total - off;
    s->lockPtr = p;
}
static void OnUnlock(void* b) {
    Slot* s = Get(b);
    if (!s || !s->lockPtr || !s->shadow) return;
    if (s->lockOff < s->size) {
        uint32_t n = s->lockSize;
        if (n > s->size - s->lockOff) n = s->size - s->lockOff;
        memcpy(s->shadow + s->lockOff, s->lockPtr, n);
    }
    s->lockPtr = nullptr;
}
static int32_t __stdcall VbLock(void* b, uint32_t off, uint32_t size, uint8_t** pp, uint32_t flags) {
    if (g_curFrame == g_debugFrame) {  // who fills which buffer: code addresses on the stack
        HMODULE self = nullptr;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           (LPCSTR)&VbLock, &self);
        uintptr_t lo = (uintptr_t)self, hi = lo + 0x400000;
        char line[512];
        int n = wsprintfA(line, "L %lu %lu %lu base %08lx:", Id(b), off, size, (DWORD)lo);
        const uint32_t* sp = (const uint32_t*)_AddressOfReturnAddress();
        for (int k = 0, found = 0; k < 256 && found < 12; ++k) {
            uint32_t v = sp[k];
            if ((v >= 0x401000 && v < 0x5c8000) || (v >= lo && v < hi)) {
                n += wsprintfA(line + n, " %08lx", v);
                ++found;
            }
        }
        rt3::Log("%s", line);
    }
    int32_t r = g_vbLock(b, off, size, pp, flags);
    if (r >= 0 && pp) OnLock(b, off, size, *pp);
    return r;
}
static int32_t __stdcall VbUnlock(void* b) { OnUnlock(b); return g_vbUnlock(b); }
static int32_t __stdcall IbLock(void* b, uint32_t off, uint32_t size, uint8_t** pp, uint32_t flags) {
    int32_t r = g_ibLock(b, off, size, pp, flags);
    if (r >= 0 && pp) OnLock(b, off, size, *pp);
    return r;
}
static int32_t __stdcall IbUnlock(void* b) { OnUnlock(b); return g_ibUnlock(b); }

static void Patch(void** vt, int slot, void* fn, void** orig) {
    if (vt[slot] == fn) return;
    DWORD old;
    VirtualProtect(&vt[slot], 4, PAGE_EXECUTE_READWRITE, &old);
    if (orig) *orig = vt[slot];
    vt[slot] = fn;
    VirtualProtect(&vt[slot], 4, old, &old);
}

// --- device -----------------------------------------------------------------------
static void* g_orig[128];  // original device methods by vtable slot
template <typename T> static T Orig(int slot) { return (T)g_orig[slot]; }

static void* g_stream0;     // vertex buffer of stream 0
static uint32_t g_stride0;
static void* g_indices;
static uint32_t g_baseVertex;
static uint32_t g_indexSize = 2;

static uint32_t VertexCount(uint32_t type, uint32_t prims) {
    switch (type) {
    case 1: return prims;
    case 2: return prims * 2;
    case 3: return prims + 1;
    case 4: return prims * 3;
    case 5: case 6: return prims + 2;
    }
    return 0;
}

static void MixVertices(uint32_t first, uint32_t count) {
    Slot* s = Get(g_stream0);
    if (!s || !s->shadow) return;
    uint64_t lo = (uint64_t)first * g_stride0, hi = lo + (uint64_t)count * g_stride0;
    if (hi > s->size) hi = s->size;
    if (lo < hi) Mix(s->shadow + lo, (uint32_t)(hi - lo));
}

// A wrapped device method (its vtable slot noted for reference).
#define DEV(slot, ret, name, params) static ret __stdcall name params

DEV(15, int32_t, Present, (void* d, void* a, void* b, void* c, void* e)) {
    ++g_frame;  // presents only counted: modal loops present as often as the wall clock allows
    return Orig<int32_t(__stdcall*)(void*, void*, void*, void*, void*)>(15)(d, a, b, c, e);
}
DEV(20, int32_t, CreateTexture, (void* d, uint32_t w, uint32_t h, uint32_t levels, uint32_t usage, uint32_t fmt, uint32_t pool, void** out)) {
    int32_t r = Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void**)>(20)(
        d, w, h, levels, usage, fmt, pool, out);
    if (r >= 0 && out) Created(*out);
    return r;
}
DEV(22, int32_t, CreateCubeTexture, (void* d, uint32_t edge, uint32_t levels, uint32_t usage, uint32_t fmt, uint32_t pool, void** out)) {
    int32_t r = Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void**)>(22)(
        d, edge, levels, usage, fmt, pool, out);
    if (r >= 0 && out) Created(*out);
    return r;
}
DEV(23, int32_t, CreateVertexBuffer, (void* d, uint32_t len, uint32_t usage, uint32_t fvf, uint32_t pool, void** out)) {
    int32_t r = Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, void**)>(23)(d, len, usage, fvf, pool, out);
    if (r >= 0 && out && *out) {
        Created(*out);
        void** vt = *(void***)*out;
        // (if both buffer kinds share one method table, the other kind's patch is already there)
        if (!g_vbLock) {
            g_vbLock = vt[11] == (void*)&IbLock ? g_ibLock : (LockFn)vt[11];
            g_vbUnlock = vt[12] == (void*)&IbUnlock ? g_ibUnlock : (UnlockFn)vt[12];
        }
        Patch(vt, 11, (void*)&VbLock, nullptr);
        Patch(vt, 12, (void*)&VbUnlock, nullptr);
    }
    return r;
}
DEV(24, int32_t, CreateIndexBuffer, (void* d, uint32_t len, uint32_t usage, uint32_t fmt, uint32_t pool, void** out)) {
    int32_t r = Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, void**)>(24)(d, len, usage, fmt, pool, out);
    if (r >= 0 && out && *out) {
        Created(*out);
        void** vt = *(void***)*out;
        if (!g_ibLock) {
            g_ibLock = vt[11] == (void*)&VbLock ? g_vbLock : (LockFn)vt[11];
            g_ibUnlock = vt[12] == (void*)&VbUnlock ? g_vbUnlock : (UnlockFn)vt[12];
        }
        Patch(vt, 11, (void*)&IbLock, nullptr);
        Patch(vt, 12, (void*)&IbUnlock, nullptr);
    }
    return r;
}
DEV(36, int32_t, Clear, (void* d, uint32_t n, const uint32_t* rects, uint32_t flags, uint32_t color, float z, uint32_t st)) {
    g_kind = 0;
    Mix4(36, n, flags, color);
    Mix(&z, 4);
    Mix4(36, st);
    if (rects) Mix(rects, n * 16);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, const uint32_t*, uint32_t, uint32_t, float, uint32_t)>(36)(d, n, rects, flags, color, z, st);
}
DEV(37, int32_t, SetTransform, (void* d, uint32_t type, const float* m)) {
    g_kind = 1;
    Mix4(37, type);
    Mix(m, 64);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, const float*)>(37)(d, type, m);
}
DEV(40, int32_t, SetViewport, (void* d, const uint32_t* vp)) {
    g_kind = 0;
    Mix4(40);
    Mix(vp, 24);
    return Orig<int32_t(__stdcall*)(void*, const uint32_t*)>(40)(d, vp);
}
DEV(42, int32_t, SetMaterial, (void* d, const uint32_t* m)) {
    g_kind = 0;
    Mix4(42);
    Mix(m, 68);
    return Orig<int32_t(__stdcall*)(void*, const uint32_t*)>(42)(d, m);
}
DEV(44, int32_t, SetLight, (void* d, uint32_t i, const uint32_t* l)) {
    g_kind = 0;
    Mix4(44, i);
    Mix(l, 104);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, const uint32_t*)>(44)(d, i, l);
}
DEV(46, int32_t, LightEnable, (void* d, uint32_t i, uint32_t on)) {
    g_kind = 0;
    Mix4(46, i, on);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t)>(46)(d, i, on);
}
DEV(50, int32_t, SetRenderState, (void* d, uint32_t state, uint32_t v)) {
    g_kind = 0;
    Mix4(50, state, v);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t)>(50)(d, state, v);
}
DEV(61, int32_t, SetTexture, (void* d, uint32_t stage, void* tex)) {
    g_kind = 2;
    Mix4(61, stage, Id(tex));
    return Orig<int32_t(__stdcall*)(void*, uint32_t, void*)>(61)(d, stage, tex);
}
DEV(63, int32_t, SetTextureStageState, (void* d, uint32_t stage, uint32_t type, uint32_t v)) {
    g_kind = 0;
    Mix4(63, stage, type, v);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t)>(63)(d, stage, type, v);
}
DEV(70, int32_t, DrawPrimitive, (void* d, uint32_t type, uint32_t first, uint32_t prims)) {
    ++g_draws;
    g_kind = 2;
    Mix4(70, type, first, prims);
    g_kind = 3;
    MixVertices(first, VertexCount(type, prims));
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t)>(70)(d, type, first, prims);
}
DEV(71, int32_t, DrawIndexedPrimitive, (void* d, uint32_t type, uint32_t minIndex, uint32_t numVerts, uint32_t start, uint32_t prims)) {
    ++g_draws;
    g_kind = 2;
    Mix4(71, type, minIndex, numVerts);
    Mix4(71, start, prims, g_baseVertex);
    g_kind = 4;
    if (Slot* s = Get(g_indices)) {
        uint64_t lo = (uint64_t)start * g_indexSize, hi = lo + (uint64_t)VertexCount(type, prims) * g_indexSize;
        if (s->shadow && hi > s->size) hi = s->size;
        if (s->shadow && lo < hi) Mix(s->shadow + lo, (uint32_t)(hi - lo));
    }
    g_kind = 3;
    MixVertices(g_baseVertex + minIndex, numVerts);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(71)(d, type, minIndex, numVerts, start, prims);
}
DEV(72, int32_t, DrawPrimitiveUP, (void* d, uint32_t type, uint32_t prims, const void* data, uint32_t stride)) {
    ++g_draws;
    g_kind = 2;
    Mix4(72, type, prims, stride);
    g_kind = 5;
    Mix(data, VertexCount(type, prims) * stride);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, const void*, uint32_t)>(72)(d, type, prims, data, stride);
}
DEV(73, int32_t, DrawIndexedPrimitiveUP, (void* d, uint32_t type, uint32_t minIndex, uint32_t numVerts, uint32_t prims,
                                           const void* idx, uint32_t fmt, const void* data, uint32_t stride)) {
    ++g_draws;
    g_kind = 2;
    Mix4(73, type, minIndex, numVerts);
    Mix4(73, prims, fmt, stride);
    g_kind = 5;
    Mix(idx, VertexCount(type, prims) * (fmt == 102 ? 4 : 2));  // D3DFMT_INDEX32 = 102
    if (data) Mix((const uint8_t*)data + minIndex * stride, numVerts * stride);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, const void*, uint32_t, const void*, uint32_t)>(73)(
        d, type, minIndex, numVerts, prims, idx, fmt, data, stride);
}
DEV(76, int32_t, SetVertexShader, (void* d, uint32_t h)) {
    g_kind = 0;
    Mix4(76, h);
    return Orig<int32_t(__stdcall*)(void*, uint32_t)>(76)(d, h);
}
DEV(79, int32_t, SetVertexShaderConstant, (void* d, uint32_t reg, const void* data, uint32_t n)) {
    g_kind = 1;
    Mix4(79, reg, n);
    Mix(data, n * 16);
    return Orig<int32_t(__stdcall*)(void*, uint32_t, const void*, uint32_t)>(79)(d, reg, data, n);
}
DEV(83, int32_t, SetStreamSource, (void* d, uint32_t n, void* vb, uint32_t stride)) {
    g_kind = 2;
    Mix4(83, n, Id(vb), stride);  // (B 83 ...: stream 0's buffer id for the L lines)
    if (n == 0) { g_stream0 = vb; g_stride0 = stride; }
    return Orig<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(83)(d, n, vb, stride);
}
DEV(85, int32_t, SetIndices, (void* d, void* ib, uint32_t base)) {
    g_kind = 2;
    Mix4(85, Id(ib), base);
    g_indices = ib;
    g_baseVertex = base;
    if (ib) {
        uint32_t desc[8] = {};
        ((int32_t(__stdcall*)(void*, void*))(*(void***)ib)[13])(ib, desc);
        g_indexSize = desc[0] == 102 ? 4 : 2;
    }
    return Orig<int32_t(__stdcall*)(void*, void*, uint32_t)>(85)(d, ib, base);
}

static void HookDevice(void* dev) {
    void** vt = *(void***)dev;
    struct { int slot; void* fn; } list[] = {
        { 15, (void*)&Present }, { 20, (void*)&CreateTexture }, { 22, (void*)&CreateCubeTexture }, { 23, (void*)&CreateVertexBuffer }, { 24, (void*)&CreateIndexBuffer },
        { 36, (void*)&Clear }, { 37, (void*)&SetTransform }, { 40, (void*)&SetViewport },
        { 42, (void*)&SetMaterial }, { 44, (void*)&SetLight }, { 46, (void*)&LightEnable },
        { 50, (void*)&SetRenderState }, { 61, (void*)&SetTexture }, { 63, (void*)&SetTextureStageState },
        { 70, (void*)&DrawPrimitive }, { 71, (void*)&DrawIndexedPrimitive }, { 72, (void*)&DrawPrimitiveUP },
        { 73, (void*)&DrawIndexedPrimitiveUP }, { 76, (void*)&SetVertexShader },
        { 79, (void*)&SetVertexShaderConstant }, { 83, (void*)&SetStreamSource }, { 85, (void*)&SetIndices },
    };
    for (auto& e : list) Patch(vt, e.slot, e.fn, &g_orig[e.slot]);
}

// --- IDirect3D8::CreateDevice (slot 15) and Direct3DCreate8 -------------------------
typedef int32_t(__stdcall* CreateDeviceFn)(void*, uint32_t, uint32_t, void*, uint32_t, void*, void**);
static CreateDeviceFn g_createDevice;
static int32_t __stdcall CreateDevice(void* d3d, uint32_t adapter, uint32_t type, void* wnd, uint32_t flags, void* pp, void** out) {
    int32_t r = g_createDevice(d3d, adapter, type, wnd, flags, pp, out);
    if (r >= 0 && out && *out) HookDevice(*out);
    return r;
}

typedef void*(__stdcall* CreateFn)(uint32_t);
static CreateFn g_create;
static void* __stdcall Create(uint32_t sdk) {
    void* d3d = g_create(sdk);
    if (d3d) {
        void** vt = *(void***)d3d;
        if (vt[15] != (void*)&CreateDevice) {
            g_createDevice = (CreateDeviceFn)vt[15];
            Patch(vt, 15, (void*)&CreateDevice, nullptr);
        }
    }
    return d3d;
}

// After main-loop frame `frame` (src/harness.cpp): logs what was drawn since the
// previous one as "D <frame> <crc> <draws> <crc per kind>", and starts over.
void FrameEnd(long frame) {
    rt3::Log("D %ld %08lx %lu %08lx %08lx %08lx %08lx %08lx %08lx", frame, g_crc, g_draws, g_part[0], g_part[1],
             g_part[2], g_part[3], g_part[4], g_part[5]);
    g_crc = 0;
    memset(g_part, 0, sizeof g_part);
    g_draws = 0;
    g_curFrame = (frame < 0 ? -frame : frame) + 1;
}

// Wraps the Direct3DCreate8 import (slot 0x5c8398) before the game starts.
void Install() {
    g_heap = HeapCreate(0, 1 << 20, 0);
    char dbg[16];
    if (GetEnvironmentVariableA("RT3RE_DRAWDEBUG", dbg, sizeof dbg)) g_debugFrame = atol(dbg);
    void** slot = (void**)0x5c8398;
    g_create = (CreateFn)*slot;
    DWORD old;
    VirtualProtect(slot, 4, PAGE_READWRITE, &old);
    *slot = (void*)&Create;
    VirtualProtect(slot, 4, old, &old);
}

} // namespace drawtrace
