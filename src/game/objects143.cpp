// Renderer teardown / sound cache init / 3D sound shutdown and sample preparation / block height ranges in blocks 0x55..0x56 (batch 146). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include "core/containers.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Render_RestoreTarget(uint8_t* r);
void __fastcall Textures_ReleaseTargets();
void __fastcall Render_SaveAndShutdown(uint8_t* r);
void __fastcall SoundCache_Clear(uint8_t* s);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Sound_StopAll(uint8_t* s);
void __fastcall Sound_WaitIdle();
uint32_t __fastcall Voices_Acquire(uint8_t* v);
void __fastcall Voices_Release(uint8_t* v, int, uint32_t key);
double __stdcall Sound_ListenerDistSq(float x, float y, float z);
void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e);
void* __fastcall SoundCache_Load(uint8_t* s, int, const char* name, void* info);
int32_t ClockMs();
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }
static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }

// 0x55dd80. Destroys the renderer (+0x2d): restores the render target, releases the
// target textures, saves its settings and shuts it down.
RT3_NOINLINE void __fastcall Display_DestroyRenderer(uint8_t* o) {
    Render_RestoreTarget(F<uint8_t*>(o, 0x2d));
    Textures_ReleaseTargets();
    if (uint8_t* r = F<uint8_t*>(o, 0x2d)) {
        Render_SaveAndShutdown(r);
        ExeDelete(r);
    }
    F<void*>(o, 0x2d) = nullptr;
}
RT3_REPLACE(0x55dd80, Display_DestroyRenderer);

// 0x564570. (Re)creates the sound cache with `capacity` bytes (0 = off): a pool of
// 100 0x38-byte entries (+0), a 17-bucket name hash (+4) and an LRU heap (+8).
int32_t __fastcall SoundCache_Init(uint8_t* s, int, int32_t capacity) {
    if (F<int32_t>(s, 0x10)) SoundCache_Clear(s);
    if (!capacity) {
        F<void*>(s, 0) = nullptr;
        F<void*>(s, 4) = nullptr;
        F<void*>(s, 8) = nullptr;
        F<int32_t>(s, 0x10) = 0;
        F<int32_t>(s, 0x14) = 0;
        F<int32_t>(s, 0xc) = 0;
        s[0x24] = 0;
        return 0;
    }
    uint32_t* h = (uint32_t*)ExeNew(0x14);
    if (h) memset(h, 0, 0x14);
    F<uint32_t*>(s, 4) = h;
    uint32_t* p = (uint32_t*)ExeNew(0x1c);
    if (p) memset(p, 0, 0x1c);
    F<uint32_t*>(s, 0) = p;
    uint32_t* q = (uint32_t*)ExeNew(0x1c);
    if (q) memset(q, 0, 0x10);
    F<uint32_t*>(s, 8) = q;
    void* storage = Malloc(0x15e0);
    Pool_Init(F<NodePool*>(s, 0), 0, storage, 0x38, 100, 0);
    Heap_Init(F<MaxHeap*>(s, 8), 0, 100);
    Hash_Init(F<HashMap*>(s, 4), 0, 0x11, 100);
    F<int32_t>(s, 0x10) = capacity;
    F<int32_t>(s, 0x20) = 0;
    F<int32_t>(s, 0x1c) = 0;
    F<int32_t>(s, 0x18) = 0;
    F<int32_t>(s, 0xc) = 0;
    s[0x24] = 0;
    return 1;
}
RT3_REPLACE(0x564570, SoundCache_Init);

// 0x569d20. Frees the voice table's hash (+4) and node pool (+8).
void __fastcall Voices_Free(uint8_t* v) {
    if (F<HashMap*>(v, 4)) {
        Hash_Destroy(F<HashMap*>(v, 4));
        if (HashMap* h = F<HashMap*>(v, 4)) {
            Hash_DestroyThunk(h);
            ExeDelete(h);
        }
        F<void*>(v, 4) = nullptr;
    }
    if (F<uint8_t*>(v, 8) && F<void*>(F<uint8_t*>(v, 8), 4)) {
        Free(F<void*>(F<uint8_t*>(v, 8), 4));
        if (NodePool* p = F<NodePool*>(v, 8)) {
            Pool_Free(p);
            ExeDelete(p);
        }
        F<void*>(v, 8) = nullptr;
    }
}
RT3_REPLACE(0x569d20, Voices_Free);

// Busy-waits ms milliseconds.
static void WaitMs(int32_t ms) {
    int32_t end = ClockMs() + ms;
    while (ClockMs() < end) {}
}

// 0x569d90. Closes the 3D sound provider (+0x28): stops everything, closes the
// listener (+0x2c), frees the sample tables (+0x14 inline, +0xc pool, +0x10 hash),
// applies a pending speaker type change (0x625c7c -> 0x625c78, 0..5, with pauses).
void __fastcall Sound3D_Close(uint8_t* s) {
    if (!F<void*>(s, 0x28)) return;
    Sound_StopAll(s);
    if (F<void*>(s, 0x2c)) {
        Import<void(__stdcall*)(void*)>(0x5c8418)(F<void*>(s, 0x2c));
        F<void*>(s, 0x2c) = nullptr;
    }
    Hash_Destroy((HashMap*)(s + 0x14));
    ((void(__thiscall*)(void*))(*(void***)s)[1])(s);
    Hash_Destroy(F<HashMap*>(s, 0x10));
    Free(F<void*>(F<uint8_t*>(s, 0xc), 4));
    if (NodePool* p = F<NodePool*>(s, 0xc)) {
        Pool_Free(p);
        ExeDelete(p);
    }
    F<void*>(s, 0xc) = nullptr;
    if (HashMap* h = F<HashMap*>(s, 0x10)) {
        Hash_DestroyThunk(h);
        ExeDelete(h);
    }
    F<void*>(s, 0x10) = nullptr;
    int32_t want = hook::Global<int32_t>(0x625c7c), cur = hook::Global<int32_t>(0x625c78);
    if (want >= 0 && want <= 5 && cur >= 0 && cur <= 5 && want != cur) {
        hook::Global<int32_t>(0x625c78) = want;
        if (want == 99 || cur != 99) {
            ClockMs();
            WaitMs(400);
            Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c83ec)(*F<uint32_t*>(s, 0x28), want);
            WaitMs(200);
        }
    }
    Import<void(__stdcall*)(uint32_t)>(0x5c8404)(*F<uint32_t*>(s, 0x28));
    F<void*>(s, 0x28) = nullptr;
    F<int32_t>(s, 0x60) = 0;
}
RT3_REPLACE(0x569d90, Sound3D_Close);

// Takes a free voice (its table entry + 8) under the Miles lock, or 0.
static uint8_t* AcquireVoice(uint8_t* s) {
    uint8_t* res = nullptr;
    Sound_WaitIdle();
    AilLock();
    if (uint32_t v = Voices_Acquire(s)) res = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, v) + 8;
    AilUnlock();
    return res;
}

// 0x56a330. A voice for 3D sound `name` at (x, y, z): when none is free, the
// farthest-off playing sample (Hash_ForEach 0x5318c0 over +0x14 with the listener
// distance) is stopped and released and the search retried (0 if nothing can go).
// The voice gets the sound's shared sample data (+0x10 hash of {crc, ..., refs +0xc,
// data +0x10, info +0x14}, loaded through the sound cache).
uint8_t* __fastcall Sound3D_PrepareSample(uint8_t* s, int, const char* name, int32_t prio, float x, float y, float z) {
    uint8_t* res;
    for (;;) {
        res = AcquireVoice(s);
        if (res) break;
        float dist = (float)Sound_ListenerDistSq(x, y, z);
        struct { int32_t kind, prio; float* dist; int32_t found; } ctx = { 8, prio, &dist, 0 };
        hook::Global<int32_t>(0xccba04) = 1;
        Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, &ctx);
        if (!ctx.found) return nullptr;
        if (ctx.found == -1) continue;
        uint32_t victim = (uint32_t)ctx.found;
        Sound_WaitIdle();
        AilLock();
        if (victim) {
            if (uint32_t* e = (uint32_t*)Hash_Get((HashMap*)(s + 0x14), 0, victim)) {
                uint32_t smp = e[-1];
                Import<void(__stdcall*)(uint32_t, void*)>(0x5c83a4)(smp, nullptr);
                Import<void(__stdcall*)(uint32_t)>(0x5c8414)(smp);
                if (e[2] == 1) --F<int32_t>(s, 0x5c);
                Voices_Release(s, 0, smp);
                Hash_Remove((HashMap*)(s + 0x14), 0, e[0]);
                F<uint32_t>(e, 0x44) &= ~0x100u;
                uint8_t* q = (uint8_t*)e[1];
                --F<int32_t>(q, 0xc);
                if (F<int32_t>((uint8_t*)e[1], 0xc) <= 0) {
                    SampleQueue_Free(s, 0, (uint32_t*)e[1]);
                    e[1] = 0;
                }
            }
        }
        AilUnlock();
    }
    uint32_t key = Crc32String(name, 1);
    memset(res + 4, 0, 0x50);
    uint8_t* q = (uint8_t*)Hash_Get(F<HashMap*>(s, 0x10), 0, key);
    if (!q) {
        uint8_t* pool = F<uint8_t*>(s, 0xc);
        if (F<int32_t>(pool, 0xc)) {
            uint32_t* l = F<uint32_t*>(pool, 0);
            q = (uint8_t*)l[0];
            if (q) {
                l[0] = F<uint32_t>(q, 0);
                if ((uint32_t)q == l[1]) l[1] = l[0];
                --l[2];
            }
        } else {
            q = (uint8_t*)ExeNew(F<uint32_t>(pool, 0x10));
        }
        F<uint32_t>(q, 0) = key;
        F<int32_t>(q, 0xc) = 0;
        Hash_Insert(F<HashMap*>(s, 0x10), 0, key, (int32_t)q);
        F<void*>(q, 0x10) = SoundCache_Load(*hook::Global<uint8_t**>(0x6d402c), 0, name, q + 0x14);
    }
    ++F<int32_t>(q, 0xc);
    F<uint8_t*>(res, 4) = q;
    return res;
}
RT3_REPLACE(0x56a330, Sound3D_PrepareSample);

// 0x56abe0. A voice for 2D sound `name`, its data loaded through the sound cache
// when `load` == 1 (else none); 0 when no voice is free.
uint8_t* __fastcall Sound_PrepareSample(uint8_t* s, int, const char* name, int32_t load) {
    uint8_t* res = AcquireVoice(s);
    if (!res) return nullptr;
    if (load == 1) F<void*>(res, 4) = SoundCache_Load(*hook::Global<uint8_t**>(0x6d402c), 0, name, nullptr);
    else F<void*>(res, 4) = nullptr;
    return res;
}
RT3_REPLACE(0x56abe0, Sound_PrepareSample);

// 0x56b2c0. Height range of the 5x5 vertex block (row, col) of the grid (origin
// +0 / +4 vertices): out (+0x14 table) +2 = the middle, +6 = the spread x 0x5de4dc.
void __fastcall Block_HeightRange(uint8_t* o, int, int32_t row, int32_t col, uint8_t* t) {
    int32_t stride = Terrain_Rows(t) + 1;
    Terrain_Cols(t);
    const float* grid = *(const float**)Terrain_HeightsSlot(t);
    uint8_t* out = F<uint8_t*>(o, 0x14 + (col + row * 4) * 4);
    int32_t z0 = F<int32_t>(o, 4) + col * 4;
    int32_t x0 = F<int32_t>(o, 0) + row * 4;
    float lo = 0.0f, hi = 0.0f;
    bool first = true;
    for (int32_t x = x0; x < x0 + 5; ++x)
        for (int32_t z = z0; z < z0 + 5; ++z) {
            float v = grid[stride * z + x];
            if (first) {
                first = false;
                lo = v;
                hi = v;
                continue;
            }
            if (lo > v) lo = v;
            if (hi < v) hi = v;
        }
    float mid = (float)(((double)lo + hi) * *(const double*)0x5c8628);
    F<float>(out, 2) = mid;
    F<float>(out, 6) = (float)(((double)hi - lo) * *(const float*)0x5de4dc);
}
RT3_REPLACE(0x56b2c0, Block_HeightRange);

} // namespace rt3
