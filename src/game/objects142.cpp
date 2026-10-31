// Renderer settings save / emitter reset / sound cache / light point files / quad joins in blocks 0x54..0x56 (batch 145). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include "core/containers.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Render_Shutdown(uint8_t* r);
void __fastcall Engine_SetC(void* e, int, int32_t v);
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode);
int32_t __fastcall Emitter_SpawnParticle(uint8_t* e);
void __fastcall Hash_DestroyThunk(HashMap* m);
void* __stdcall Sound_LoadWav(const char* name, int32_t, int32_t* size);
uint32_t __fastcall TimedQueue_Pop(uint8_t* q);
void* __fastcall Pool_Alloc(int32_t* p);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall ModelSet_Free(uint8_t* s);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x54be60. Saves the renderer settings (+0x840, 0x72c dwords) to 0xccbb68 (flag
// 0xccd820), shuts the renderer down and clears engine +0xc.
RT3_NOINLINE void __fastcall Render_SaveAndShutdown(uint8_t* r) {
    hook::Global<uint8_t>(0xccd820) = 1;
    memcpy((void*)0xccbb68, r + 0x840, 0x72c * 4);
    Render_Shutdown(r);
    Engine_SetC(hook::Global<void*>(0x6d4024), 0, 0);
}
RT3_REPLACE(0x54be60, Render_SaveAndShutdown);

// 0x554ce0. Restarts the emitter with n particles: clears its lists (unless new and
// empty), makes two fresh lists (+0, +4) and spawns n.
RT3_NOINLINE void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t) {
    if (n <= 0 || F<int32_t>(e, 0x34) > 0) {
        ListPair_Clear(e, 0, 1);
        if (n <= 0) return;
    }
    for (int k = 0; k < 2; ++k) {
        uint32_t* l = (uint32_t*)ExeNew(0xc);
        if (l) {
            l[0] = 0;
            l[1] = 0;
            l[2] = 0;
        }
        F<uint32_t*>(e, k * 4) = l;
    }
    for (int32_t i = n; i; --i) Emitter_SpawnParticle(e);
}
RT3_REPLACE(0x554ce0, Emitter_Reset);

// 0x564280. Empties the sound cache (+0 node pool, +4 name hash, +8 LRU heap, size
// +0xc of capacity +0x10, prefix +0x24, enabled +0x14), freeing every cached sound.
void __fastcall SoundCache_Clear(uint8_t* s) {
    if (!F<int32_t>(s, 0x10)) return;
    Hash_ForEach(F<HashMap*>(s, 4), 0, 0, (HashVisit)0x5641d0, nullptr);
    Hash_Destroy(F<HashMap*>(s, 4));
    Free(F<void*>(F<uint8_t*>(s, 0), 4));
    if (NodePool* p = F<NodePool*>(s, 0)) {
        Pool_Free(p);
        ExeDelete(p);
    }
    F<void*>(s, 0) = nullptr;
    if (HashMap* h = F<HashMap*>(s, 4)) {
        Hash_DestroyThunk(h);
        ExeDelete(h);
    }
    F<void*>(s, 4) = nullptr;
    if (MaxHeap* h = F<MaxHeap*>(s, 8)) {
        Heap_Destroy2(h);
        ExeDelete(h);
    }
    F<void*>(s, 8) = nullptr;
    F<int32_t>(s, 0x10) = 0;
    F<int32_t>(s, 0xc) = 0;
    s[0x24] = 0;
    F<int32_t>(s, 0x14) = 0;
}
RT3_REPLACE(0x564280, SoundCache_Clear);

// 0x564390. Loads sound <prefix +0x24><name> into a new buffer, through the cache
// when enabled (+0x14): cached entries {crc, size, heap node, data, WAV info x 9}
// are reused (hits +0x1c of lookups +0x18); new ones evict the oldest until they
// fit (sounds larger than the cache are returned uncached). *info gets the WAV info.
void* __fastcall SoundCache_Load(uint8_t* s, int, const char* name, void* info) {
    char path[0x10c];
    char* d = path;
    if (s[0x24])
        for (const char* p = (const char*)s + 0x24; (*d = *p) != 0; ++d, ++p) {}
    for (const char* p = name; (*d++ = *p++) != 0;) {}
    int32_t size;
    if (!F<int32_t>(s, 0x14)) return Sound_LoadWav(path, (int32_t)name, &size);
    uint32_t key = Crc32String(path, 1);
    ++F<int32_t>(s, 0x18);
    uint8_t* e = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    if (!e) {
        void* data = Sound_LoadWav(path, (int32_t)name, &size);
        if (size > F<int32_t>(s, 0x10)) return data;
        while (F<int32_t>(s, 0xc) + size > F<int32_t>(s, 0x10)) F<int32_t>(s, 0xc) -= TimedQueue_Pop(s);
        F<int32_t>(s, 0xc) += size;
        e = (uint8_t*)Pool_Alloc(F<int32_t*>(s, 0));
        memcpy(e + 0x14, (const void*)0xd9798c, 36);
        F<uint32_t>(e, 0) = key;
        F<void*>(e, 0x10) = data;
        F<int32_t>(e, 4) = size;
        F<HeapNode*>(e, 8) = Heap_Push(F<MaxHeap*>(s, 8), 0, 0x7ffffffe, (int32_t)e);
        Hash_Insert(F<HashMap*>(s, 4), 0, key, (int32_t)e);
    } else {
        size = F<int32_t>(e, 4);
        ++F<int32_t>(s, 0x1c);
    }
    if (info) memcpy(info, e + 0x14, 36);
    Heap_DecrementKey(F<MaxHeap*>(s, 8), 0, F<HeapNode*>(e, 8));
    void* p = Malloc(size);
    memcpy(p, F<void*>(e, 0x10), size);
    return p;
}
RT3_REPLACE(0x564390, SoundCache_Load);

// 0x564560. Thunk to SoundCache_Clear.
void __fastcall SoundCache_ClearThunk(uint8_t* s) { SoundCache_Clear(s); }
RT3_REPLACE(0x564560, SoundCache_ClearThunk);

// 0x566a80. Reads a light point set from the current pack: a 9-dword header
// ({?, count, ?, +0xc, +0x10, r, g, b, a}; colour packed to +8 from 0..1 floats)
// then count points of 12 bytes (+0) with y and z swapped.
int32_t __stdcall Light_LoadPoints(uint8_t* o) {
    int32_t h[9];
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, h, 0x24);
    const float* hf = (const float*)h;
    F<int32_t>(o, 0x14) = h[5];
    F<int32_t>(o, 0x18) = h[6];
    F<int32_t>(o, 0x1c) = h[7];
    F<int32_t>(o, 4) = h[1];
    F<int32_t>(o, 0x20) = h[8];
    F<int32_t>(o, 0x10) = h[4];
    F<int32_t>(o, 0xc) = h[3];
    const float k = Fl(0x5ca0ec);
    uint32_t c = (uint32_t)(int32_t)((double)hf[5] * k) << 8;
    c = (c | (uint32_t)(int32_t)((double)hf[6] * k)) << 8;
    c = (c | (uint32_t)(int32_t)((double)hf[7] * k)) << 8;
    c |= (uint32_t)(int32_t)((double)hf[8] * k);
    F<uint32_t>(o, 8) = c;
    int32_t n = h[1];
    float* p = (float*)Malloc(n * 12);
    F<float*>(o, 0) = p;
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, p, n * 12);
    for (int32_t i = 0; i < h[1]; ++i) {
        float* q = F<float*>(o, 0) + i * 3;
        float y = q[1];
        F<uint32_t>(q, 4) = F<uint32_t>(q, 8);
        q[2] = y;
    }
    return 0;
}
RT3_REPLACE(0x566a80, Light_LoadPoints);

// 0x567520. Thunk to ModelSet_Free.
void __fastcall ModelSet_FreeThunk(uint8_t* s) { ModelSet_Free(s); }
RT3_REPLACE(0x567520, ModelSet_FreeThunk);

// 0x5675c0. Finds the closest pair of corners (4 x 12 bytes, x/z) between quads a
// and b, the distance weighted 0.85 on the opposite-corner pairs, under *best and
// at least 0.01: stores them and the distance and averages the quads' +0x39 into
// the corners' +0x35 bytes; corners closer than 0.01 are marked (+0x31 = 1),
// corners marked 2 are skipped.
void __fastcall Quad_FindJoin(uint8_t* a, uint8_t* b, uint8_t** outA, uint8_t** outB, float* best) {
    static const uint32_t w[16] = { 0x40000000, 0x40000000, 0x40000000, 0x3f59999a, 0x40000000, 0x40000000,
                                    0x3f59999a, 0x40000000, 0x40000000, 0x3f59999a, 0x40000000, 0x40000000,
                                    0x3f59999a, 0x40000000, 0x40000000, 0x40000000 };
    for (int32_t i = 0; i < 4; ++i) {
        uint8_t* p = a + i * 12;
        double px = F<float>(p, 0), pz = F<float>(p, 8);
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = b + j * 12;
            double dz = pz - F<float>(q, 8), dx = px - F<float>(q, 0);
            double d = sqrt(dx * dx + dz * dz) * *(const float*)&w[i * 4 + j];
            if (a[0x31 + i] == 2 || b[0x31 + j] == 2) continue;
            if (d < *best && !(d < D(0x5c85b8) || d != d)) {
                *outA = p;
                *outB = q;
                uint8_t v = (uint8_t)(((int32_t)b[0x39] + a[0x39]) >> 1);
                b[0x35 + j] = v;
                a[0x35 + i] = v;
                *best = (float)d;
            } else if (d < D(0x5c85b8)) {
                a[0x31 + i] = 1;
                b[0x31 + j] = 1;
            }
        }
    }
}
RT3_REPLACE(0x5675c0, Quad_FindJoin);

} // namespace rt3
