// Sound3D.cpp - RT3_mp.exe 0x568000-0x5a0000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <initializer_list>
#include <intrin.h>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "core/containers.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
int32_t ClockMs();
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z);
void __fastcall Sound_WaitIdle();
uint8_t* __stdcall Sound_FindProvider(const char* name);
int32_t __fastcall Sound_Open3DProvider(uint8_t* s);
void __fastcall Sound_EnumProviders();
int32_t __fastcall Sound_OpenPreferredProvider(uint8_t* s);
void __fastcall Voices_Release(uint8_t* v, int, uint32_t key);
void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e);
void __fastcall Sound3D_Retire(uint8_t* s, int, uint32_t key, uint32_t* smp);
void __fastcall Sound2D_StopAll(uint8_t* s);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Sound_StopAll(uint8_t* s);
uint32_t __fastcall Voices_Acquire(uint8_t* v);
double __stdcall Sound_ListenerDistSq(float x, float y, float z);
void* __fastcall SoundCache_Load(uint8_t* s, int, const char* name, void* info);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);
uint8_t* __fastcall Sound3D_PrepareSample(uint8_t* s, int, const char* name, int32_t prio, float x, float y, float z);
uint8_t* __fastcall Sound_PrepareSample(uint8_t* s, int, const char* name, int32_t load);
int32_t __fastcall Sound_RegisterEos(uint8_t* s, int, uint32_t sample, int32_t kind);
void __fastcall Sound3D_Close(uint8_t* s);
void __fastcall Voices_Free(uint8_t* v);
void __fastcall Block_HeightRange(uint8_t* o, int, int32_t row, int32_t col, uint8_t* t);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
struct PtrList;
PtrList* __fastcall PtrList_Construct(PtrList* l, int, char usePool, int32_t blockSize);
uint32_t __fastcall Terrain_Get1661(uint8_t* r);
uint32_t __fastcall Terrain_Get1665(uint8_t* r);
uint32_t __fastcall Terrain_Get165d(uint8_t* r);
void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void* __fastcall Obj_Building(void* o);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
void* __fastcall Obj_Ref2a8(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void __fastcall Hash_Destroy(HashMap* m);
void __fastcall Pool_Free(NodePool* p);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
typedef int32_t(__fastcall* StreamFn)(uint8_t* buf, int32_t n);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Water_BuildQuads(uint8_t* g);
uint8_t* __fastcall WaterGrid_Init(uint8_t* g, int, int32_t bx, int32_t by, uint8_t* t);
int32_t __fastcall Sound_Open3DProvider(uint8_t* s);
void __fastcall Sound_EnumProviders();
uint8_t* __stdcall Sound_FindProvider(const char* name);
void __fastcall Sound_SetSpeakerType(uint8_t* s, int, int32_t type, int32_t noApply);
int32_t __fastcall Sound_SpeakerType();
double __stdcall Sound_ListenerDistSq(float x, float y, float z);
void __fastcall Sound_SetListenerPos(uint8_t* s, int, float x, float y, float z);
void __fastcall Sound_SetListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
uint32_t __fastcall Sound_ProviderInfo(uint8_t* s);
uint32_t __fastcall Sound_ProviderFlags(uint8_t* s, int, const char* name);
RT3_NOINLINE uint32_t __fastcall Voices_Acquire(uint8_t* v);
void __fastcall Voices_Release(uint8_t* v, int, uint32_t key);
int32_t __fastcall Sound_OpenPreferredProvider(uint8_t* s);
int32_t __fastcall Sound3D_Open(uint8_t* s, int, const char* name);
void __fastcall Sound3D_CreateVoices(uint8_t* v, int, int32_t);
void __fastcall Sound_StopAll(uint8_t* s);
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol);
void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e);
void __fastcall Sound3D_Retire(uint8_t* s, int, uint32_t key, uint32_t* smp);
void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp);
void __fastcall Sound_PauseAll(uint8_t* s);
void __fastcall Sound_FlagVoice(uint8_t* s, int, uint32_t key);
void __fastcall Voices_Free(uint8_t* v);
void __fastcall Sound3D_Close(uint8_t* s);
void __fastcall Sound3D_Shutdown(uint8_t* v);
RT3_NOINLINE int32_t __fastcall Sound3D_Command(uint8_t* sys, int, uint8_t* h, int32_t* cmd);
uint8_t* __fastcall Sound3D_PrepareSample(uint8_t* s, int, const char* name, int32_t prio, float x, float y, float z);
uint32_t __fastcall Sound3D_Play(uint8_t* s, int, const char* name, float x, float y, float z, float vol, int32_t mode, int32_t paused, int32_t prio, float maxDist);
uint8_t* __fastcall Sound2D_Construct(uint8_t* s);
void __fastcall Sound3D_Destruct(uint8_t* s);
int32_t __fastcall Sound2D_Open(uint8_t* s);
void __fastcall Sound2D_AllocSamples(uint8_t* s, int, int32_t n);
void __fastcall Sound2D_StopAll(uint8_t* s);
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol);
RT3_NOINLINE int32_t __fastcall Sound2D_Command(uint8_t* s, int, uint8_t* node, int32_t* ctx);
uint8_t* __fastcall Sound_PrepareSample(uint8_t* s, int, const char* name, int32_t load);
void __fastcall Sound2D_Stop(uint8_t* s, int, uint32_t key);
void __fastcall Sound2D_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
uint32_t __fastcall Sound2D_Play(uint8_t* s, int, const char* name, float vol, uint32_t pan, int32_t mode, int32_t paused, void* data);
void __fastcall Sound2D_PauseAll(uint8_t* s);
void __fastcall Sound2D_FlagVoice(uint8_t* s, int, uint32_t key);
void __fastcall Sound2D_Close(uint8_t* s);
uint8_t* __fastcall Mixer_Construct(uint8_t* m);
void __fastcall Sound2D_Destruct(uint8_t* s);
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h);
void __fastcall BlockGrid_Free(uint8_t* g);
void __fastcall BlockGrid_Set(uint8_t* g, int, int32_t, int32_t a, int32_t b, uint8_t v);
uint8_t __fastcall BlockGrid_Get(uint8_t* g, int, int32_t a, int32_t b);
int32_t __fastcall BlockGrid_Dec(uint8_t* g, int, int32_t a, int32_t b);
uint32_t __fastcall BlockGrid_Count(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_SetCount(uint8_t* g, int, int32_t a, int32_t b, uint8_t n);
uint32_t __fastcall BlockGrid_Byte(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall Block_HeightRange(uint8_t* o, int, int32_t row, int32_t col, uint8_t* t);
void __fastcall BlockGrid_Load(uint8_t* g, int, Stream s);
void __fastcall BlockGrid_Save(uint8_t* g, int, Stream s);
void __fastcall Grid_AddPoint(uint8_t* g, int, uint8_t* t, int32_t x, int32_t z, uint8_t kind);
void __fastcall BlockGrid_SetByte(uint8_t* g, int, int32_t a, int32_t b, uint8_t v);
void __fastcall Grid_RefreshPoints(uint8_t* g, int, uint8_t* t);
void StreamA_Get();
void StreamA_Put();
void Tree_InsertNode();
void Tree_DeleteNode();
void StreamA_PutPending();
void Coder_Emit();
void Tree_Init();
RT3_NOINLINE int32_t __fastcall Lzss_Unpack(StreamFn read, StreamFn write);
RT3_NOINLINE int32_t __fastcall Lzss_Pack(StreamFn read, StreamFn write);
void StreamB_Put();
void StreamB_Read();
RT3_NOINLINE int32_t __fastcall Rle_Pack(StreamFn read, StreamFn write);
RT3_NOINLINE int32_t __fastcall Rle_Unpack(StreamFn read, StreamFn write);
int32_t __fastcall Module_RegisterCleanup();

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline const char* ExeStrStr(const char* a, const char* b) {
    return ((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)(a, b);
}

static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }

static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }

// 3D providers: 0xd97bc0, 16-byte {handle, name, enabled, -}, count 0xd97dc0 (max 32).
static inline uint8_t* Provider(int32_t i) { return (uint8_t*)0xd97bc0 + i * 16; }

static inline uint32_t Listener(uint8_t* s) {
    if (!F<uint32_t>(s, 0x2c)) F<uint32_t>(s, 0x2c) = Import<uint32_t(__stdcall*)(uint32_t)>(0x5c8460)(*F<uint32_t*>(s, 0x28));
    return F<uint32_t>(s, 0x2c);
}

static inline uint32_t* NewZeroed(size_t n) {
    uint32_t* p = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(n);
    if (p) for (size_t k = 0; k < n / 4; ++k) p[k] = 0;
    return p;
}

static inline void VisitAll(uint8_t* s, int32_t mode, int32_t flag) {
    int32_t ctx[4] = { mode, -1 };
    hook::Global<int32_t>(0xccba04) = flag;
    Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
}

static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x56b000 / 0x56b070. Low nibble of cell (x, y) (block (x/4, y/4), 8-byte cells +0xa).
static inline uint8_t* Cell(uint8_t* g, int32_t a, int32_t b) {
    uint8_t* blk = F<uint8_t*>(g, 0x14 + (b / 4 + (a / 4) * 4) * 4);
    if (!blk) return nullptr;
    return blk + (b % 4 + (a % 4) * 4) * 8 + 0xa;
}

// Block grid cells (+0xa: low nibble flag, bits 4..6 counter; +0xb byte); blocks
// start with a 16-bit cell count (+0) and are freed when it reaches 0.
static inline uint8_t** Slot(uint8_t* g, int32_t a, int32_t b) { return (uint8_t**)(g + 0x14 + (b / 4 + (a / 4) * 4) * 4); }

static inline uint8_t* CellIn(uint8_t* blk, int32_t a, int32_t b) { return blk + (b % 4 + (a % 4) * 4) * 8 + 0xa; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

// Busy-waits ms milliseconds.
static void WaitMs(int32_t ms) {
    int32_t end = ClockMs() + ms;
    while (ClockMs() < end) {}
}

// Takes a free voice (its table entry + 8) under the Miles lock, or 0.
static uint8_t* AcquireVoice(uint8_t* s) {
    uint8_t* res = nullptr;
    Sound_WaitIdle();
    AilLock();
    if (uint32_t v = Voices_Acquire(s)) res = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, v) + 8;
    AilUnlock();
    return res;
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline uint8_t* Snd() { return *hook::Global<uint8_t**>(0x6d402c); }

// Registers the voice res (key res[0]) in hash h under the Miles lock.
static void Register(HashMap* h, uint32_t* res) {
    Sound_WaitIdle();
    AilLock();
    Hash_Insert(h, 0, res[0], (int32_t)res);
    AilUnlock();
}

// Stores point k of cell (cell +2 + k) as its height above the block's range
// (block +2 middle, +6 spread), from terrain point (x, z) + the grid offset.
static void StorePoint(uint8_t* g, uint8_t* t, uint8_t* blk, uint8_t* cell, int32_t k, float px, float pz) {
    double h = Terrain_HeightAt(t, 0, (float)((double)px + F<float>(g, 8)), (float)((double)pz + F<float>(g, 0xc)));
    if (fabs((double)F<float>(blk, 6)) > *(const double*)0x5c88a0) {
        double v = (h - F<float>(blk, 2)) / F<float>(blk, 6);
        int32_t r;
        __asm {
            fld v
            mov eax, 0x5a10d0
            call eax
            mov r, eax
        }
        cell[2 + k] = (uint8_t)r;
    } else {
        cell[2 + k] = 0;
    }
}

// Terrain vtable slot 1: point k of kind `kind` in cell (x, z) -> (px, pz).
static inline void PointPos(uint8_t* t, int32_t x, int32_t z, int32_t k, int32_t kind, float* px, float* pz) {
    ((void(__thiscall*)(void*, int32_t, int32_t, int32_t, int32_t, float*, float*, int32_t, int32_t))(*(void***)t)[1])(
        t, x, z, k, kind, px, pz, 0, 0);
}

// Drops sample entry `e` (handle h): releases its voice, removes it from the table
// and frees its data.
static void Sample_Drop(uint8_t* s, uint8_t* e, uint32_t h) {
    Voices_Release(s, 0, h);
    Hash_Remove((HashMap*)(s + 0xc), 0, F<uint32_t>(e, 0));
    if (void* d = F<void*>(e, 4)) {
        Free(d);
        F<void*>(e, 4) = nullptr;
    }
}

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

namespace {
void Free446(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
// Counts the records of c whose building state is 1, that pass slot 0x70
// and whose city's building class (slot 0x80, +0x8c) is `cls`.
int32_t Count_RecordsOfClass(Collection* c, uint8_t cls) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        if (Obj_Flag3d4(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))) != 1) continue;
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)r)[0x70 / 4])(r)) continue;
        uint8_t* b = (uint8_t*)Obj_Building(Obj_Ref2a8(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0))));
        if (((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x80 / 4])(b)[0x8c] == cls) ++n;
    }
    return n;
}
// Rebuilds a vote state from packet d: two 16-bit values (+4 / +5 bytes),
// optionally a third (+6), then per player slot a "present" mark and a
// "for" mark (inactive or voting in `mask`).
void Vote_BuildState(uint8_t* st, const uint8_t* d, uint32_t mask, uint32_t present, uint32_t voted) {
    F<uint16_t>(st, 4) = d[4];
    F<uint16_t>(st, 6) = d[5];
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ceb9c)); ++i) {
        st[present + i] = 1;
        Collection* c = hook::Global<Collection*>(0x6ceb9c);
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        if (!F<int32_t>(e, 0x1e1) || !F<int32_t>(e, 4) || (mask & (1u << i))) st[voted + i] = 1;
    }
}
}

// Output stream A: 0x8000-byte buffer *0xda7dd8, fill 0xda7dd4, flushed as 0xd97dc8.
static int32_t PutA(uint8_t b, StreamFn flush) {
    int32_t pos = hook::Global<int32_t>(0xda7dd4);
    hook::Global<uint8_t*>(0xda7dd8)[pos] = b;
    ++pos;
    hook::Global<int32_t>(0xda7dd4) = pos;
    int32_t r = 1;
    if (pos == 0x8000 && flush) {
        r = flush((uint8_t*)0xd97dc8, pos);
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    return r;
}

// 0x56b940 body.
static int32_t __cdecl StreamA_PutImpl(uint32_t b, StreamFn flush) { return PutA((uint8_t)b, flush); }

// 0x56c320 body: the same for stream B (buffer *0xdb7dec, fill 0xdafde4, flushed as 0xda7de0).
static int32_t __cdecl StreamB_PutImpl(uint32_t b, StreamFn flush) {
    int32_t pos = hook::Global<int32_t>(0xdafde4);
    hook::Global<uint8_t*>(0xdb7dec)[pos] = (uint8_t)b;
    ++pos;
    hook::Global<int32_t>(0xdafde4) = pos;
    int32_t r = 1;
    if (pos == 0x8000 && flush) {
        r = flush((uint8_t*)0xda7de0, pos);
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return r;
}

// 0x56bbb0 body. Writes the coder's pending bytes (+0x3c, count +0x18) to stream A;
// returns what the original leaves in EAX.
static int32_t __cdecl StreamA_PutPendingImpl(StreamFn flush, uint8_t* c, int32_t eaxIn) {
    if (!flush) return eaxIn;
    int32_t n = F<int32_t>(c, 0x18);
    if (n <= 0) return n;
    int32_t pos = 0;
    for (int32_t i = 0; i < F<int32_t>(c, 0x18); ++i) {
        pos = hook::Global<int32_t>(0xda7dd4);
        hook::Global<uint8_t*>(0xda7dd8)[pos] = c[0x3c + i];
        ++pos;
        hook::Global<int32_t>(0xda7dd4) = pos;
        if (pos == 0x8000) {
            flush((uint8_t*)0xd97dc8, pos);
            pos = 0;
            hook::Global<int32_t>(0xda7dd4) = 0;
        }
    }
    return pos;
}

// 0x56b8f0 body. Reads one byte from input stream A (pointer 0xda7dd0, left
// 0xd9fdc8), refilling 0x8000 bytes at 0xd9fdd0 through the callback; -1 at the end.
static int32_t __cdecl StreamA_GetImpl(StreamFn refill) {
    int32_t left = hook::Global<int32_t>(0xd9fdc8);
    if (left <= 0) {
        if (!refill) return -1;
        hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fdd0;
        left = refill((uint8_t*)0xd9fdd0, 0x8000);
        if (left == -1) {
            hook::Global<int32_t>(0xd9fdc8) = 0;
            return -1;
        }
    }
    uint8_t* p = hook::Global<uint8_t*>(0xda7dd0);
    int32_t b = *p;
    hook::Global<int32_t>(0xd9fdc8) = left - 1;
    hook::Global<uint8_t*>(0xda7dd0) = p + 1;
    return b;
}

// 0x56c370 body. Reads up to n bytes from input stream B (pointer 0xdafde0, left
// 0xdb7de8, refills at 0xdafde8) into dst; the count read.
static int32_t __cdecl StreamB_ReadImpl(int32_t n, StreamFn refill, uint8_t* dst) {
    int32_t i = 0;
    while (i < n) {
        int32_t left = hook::Global<int32_t>(0xdb7de8);
        if (left <= 0) {
            if (!refill) return i;
            hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafde8;
            left = refill((uint8_t*)0xdafde8, 0x8000);
            if (left == -1) {
                hook::Global<int32_t>(0xdb7de8) = 0;
                return i;
            }
        }
        uint8_t* p = hook::Global<uint8_t*>(0xdafde0);
        uint32_t b = *p;
        hook::Global<uint8_t*>(0xdafde0) = p + 1;
        hook::Global<int32_t>(0xdb7de8) = left - 1;
        if (b == 0xffffffff) return i;
        dst[i++] = (uint8_t)b;
    }
    return i;
}

// 0x56bb10 body. Deletes node p from the packer's binary search tree (state +0x24
// parent, +0x28 left, +0x2c right, +8 the nil index), as in LZSS DeleteNode.
static void __cdecl Tree_DeleteNodeImpl(int32_t p, uint8_t* st) {
    int32_t* parent = F<int32_t*>(st, 0x24);
    int32_t* left = F<int32_t*>(st, 0x28);
    int32_t* right = F<int32_t*>(st, 0x2c);
    int32_t nil = F<int32_t>(st, 8);
    if (parent[p] == nil) return;
    int32_t q;
    if (right[p] == nil) q = left[p];
    else if (left[p] == nil) q = right[p];
    else {
        q = left[p];
        if (right[q] != nil) {
            do q = right[q];
            while (right[q] != nil);
            right[parent[q]] = left[q];
            parent[left[q]] = parent[q];
            left[q] = left[p];
            parent[left[p]] = q;
        }
        right[q] = right[p];
        parent[right[p]] = q;
    }
    parent[q] = parent[p];
    if (right[parent[p]] == p) right[parent[p]] = q;
    else left[parent[p]] = q;
    parent[p] = F<int32_t>(st, 8);
}

// 0x56bc00 body. Emits the coder's current token: a literal byte (match length
// +0x20 <= 2; sets the token's flag bit +0x4d in the flag byte +0x3c) or a match
// (position +0x1c, length - 3, split by the bit widths +0xc / +0x10). Every 8
// tokens the group (+0x3c, length +0x18) is written to stream A and restarted.
static int32_t __cdecl Coder_EmitImpl(uint8_t* c, StreamFn flush) {
    if (F<int32_t>(c, 0x20) <= 2) {
        c[0x3c] |= c[0x4d];
        F<int32_t>(c, 0x20) = 1;
        c[0x3c + F<int32_t>(c, 0x18)] = F<uint8_t*>(c, 0x38)[F<int32_t>(c, 0x14)];
    } else {
        uint8_t lo = (uint8_t)F<int32_t>(c, 0x1c);
        c[0x3c + F<int32_t>(c, 0x18)] = lo;
        uint8_t hi = (uint8_t)(F<int32_t>(c, 0x1c) >> ((F<int32_t>(c, 0xc) - 8) & 31));
        uint8_t mask = (uint8_t)(0xff << (c[0x10] & 31));
        int32_t at = ++F<int32_t>(c, 0x18);
        c[0x3c + at] = (uint8_t)((hi & mask) | (uint8_t)(c[0x20] - 3));
    }
    uint8_t bit = (uint8_t)(c[0x4d] << 1);
    int32_t n = ++F<int32_t>(c, 0x18);
    c[0x4d] = bit;
    if (bit) return n;
    F<int32_t>(c, 0) += n;
    StreamA_PutPendingImpl(flush, c, n);
    F<int32_t>(c, 0x18) = 1;
    c[0x4d] = 1;
    c[0x3c] = 0;
    return 1;
}

// 0x56b990 body. LZSS InsertNode: inserts text position r (text +0x38) into the
// coder's binary trees (parent +0x24, left +0x28, right +0x2c, roots 0x201 + first
// byte, nil +8), recording the longest match (+0x1c position, +0x20 length, up to
// 0x42); a full-length match replaces the old node.
static void __cdecl Tree_InsertNodeImpl(uint8_t* c, int32_t r) {
    int32_t* parent = F<int32_t*>(c, 0x24);
    int32_t* left = F<int32_t*>(c, 0x28);
    int32_t* right = F<int32_t*>(c, 0x2c);
    uint8_t* text = F<uint8_t*>(c, 0x38);
    int32_t nil = F<int32_t>(c, 8);
    right[r] = nil;
    left[r] = nil;
    const uint8_t* key = text + r;
    int32_t p = 0x201 + key[0];
    F<int32_t>(c, 0x20) = 0;
    int32_t cmp = 1;
    for (;;) {
        if (cmp >= 0) {
            if (right[p] == F<int32_t>(c, 8)) {
                right[p] = r;
                parent[r] = p;
                return;
            }
            p = right[p];
        } else {
            if (left[p] == F<int32_t>(c, 8)) {
                left[p] = r;
                parent[r] = p;
                return;
            }
            p = left[p];
        }
        int32_t i = 1;
        cmp = 0;
        for (; i < 0x42; ++i)
            if ((cmp = (int32_t)key[i] - (int32_t)text[p + i]) != 0) break;
        if (i <= F<int32_t>(c, 0x20)) continue;
        F<int32_t>(c, 0x1c) = p;
        F<int32_t>(c, 0x20) = i;
        if (i >= 0x42) break;
    }
    parent[r] = parent[p];
    left[r] = left[p];
    right[r] = right[p];
    parent[left[p]] = r;
    parent[right[p]] = r;
    if (right[parent[p]] == p) right[parent[p]] = r;
    else left[parent[p]] = r;
    parent[p] = F<int32_t>(c, 8);
    (void)nil;
}

// 0x56bca0 body. Allocates and clears the coder's trees: 9/7-bit position/length
// split, nil 0x200 in every node and root (roots 0x201..0x300).
static void __cdecl Tree_InitImpl(uint8_t* c) {
    typedef void*(__cdecl * MallocFn)(size_t);
    MallocFn alloc = (MallocFn)0x5a125d;
    F<int32_t>(c, 0x10) = 7;
    F<int32_t>(c, 0xc) = 9;
    F<void*>(c, 0x24) = alloc(0x804);
    F<void*>(c, 0x28) = alloc(0x804);
    F<void*>(c, 0x2c) = alloc(0xc04);
    F<void*>(c, 0x38) = alloc(0x241);
    F<int32_t>(c, 8) = 0x200;
    for (int32_t i = 0; i < 0x200; ++i) {
        F<int32_t*>(c, 0x24)[i] = F<int32_t>(c, 8);
        F<int32_t*>(c, 0x28)[i] = F<int32_t>(c, 8);
        F<int32_t*>(c, 0x2c)[i] = F<int32_t>(c, 8);
    }
    for (int32_t i = 0x201; i <= 0x300; ++i) F<int32_t*>(c, 0x2c)[i] = F<int32_t>(c, 8);
}

// 0x5692e0. Sets up water grid g for the 16 x 16 block (bx, by) of terrain t
// (its two quad lists, cell / height tables and size) and builds its quads.
uint8_t* __fastcall WaterGrid_Init(uint8_t* g, int, int32_t bx, int32_t by, uint8_t* t) {
    static const uint32_t lists[2] = { 0x1c, 0x20 };
    for (uint32_t off : lists) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(g, off) = l ? PtrList_Construct(l, 0, 1, 0x64) : nullptr;
    }
    F<int32_t>(g, 4) = by << 4;
    F<int32_t>(g, 0) = bx << 4;
    F<uint32_t>(g, 8) = Terrain_Get1661(t);
    F<uint32_t>(g, 0xc) = Terrain_Get1665(t);
    F<uint32_t>(g, 0x10) = Terrain_Get165d(t);
    F<uint8_t*>(g, 0x24) = t;
    F<int32_t>(g, 0x14) = Terrain_Rows(t);
    F<int32_t>(g, 0x18) = Terrain_Cols(t);
    Water_BuildQuads(g);
    return g;
}
RT3_REPLACE(0x5692e0, WaterGrid_Init);

// 0x569380. Opens the Miles 3D provider (+0x28): speaker type (once), room
// support (+0xc) and distance factor.
RT3_NOINLINE int32_t __fastcall Sound_Open3DProvider(uint8_t* s) {
    uint32_t* p = F<uint32_t*>(s, 0x28);
    if (!p) return 0;
    if (Import<int32_t(__stdcall*)(uint32_t)>(0x5c83e8)(p[0])) {
        F<void*>(s, 0x28) = nullptr;
        return 0;
    }
    if (hook::Global<int32_t>(0x625c7c) == -2)
        hook::Global<int32_t>(0x625c7c) = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83e0)(F<uint32_t*>(s, 0x28)[0]);
    int32_t room = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83dc)(F<uint32_t*>(s, 0x28)[0]);
    F<uint32_t*>(s, 0x28)[3] = room != -1 ? 1 : 0;
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c83d8)(F<uint32_t*>(s, 0x28)[0], 0x3e806d98);
    return 1;
}
RT3_REPLACE(0x569380, Sound_Open3DProvider);

// 0x569400. Enumerates the Miles 3D providers, disabling the blacklisted ones
// (names containing 0x5de458 / 0x5de450 / 0x5de44c).
RT3_NOINLINE void __fastcall Sound_EnumProviders() {
    for (int k = 0; k < 0x80; ++k) ((uint32_t*)0xd97bc0)[k] = 0;
    auto enumerate = Import<int32_t(__stdcall*)(uint32_t*, void*, void*)>(0x5c8454);
    uint32_t next = 0;
    int32_t& count = hook::Global<int32_t>(0xd97dc0);
    count = 0;
    if (!enumerate(&next, (void*)0xd97bc0, (void*)0xd97bc4)) return;
    while (count < 0x20) {
        F<int32_t>(Provider(count), 8) = 1;
        const char* name = F<const char*>(Provider(count), 4);
        if (ExeStrStr(name, (const char*)0x5de458) || ExeStrStr(F<const char*>(Provider(count), 4), (const char*)0x5de450) ||
            ExeStrStr(F<const char*>(Provider(count), 4), (const char*)0x5de44c))
            F<int32_t>(Provider(count), 8) = 0;
        ++count;
        if (!enumerate(&next, Provider(count), Provider(count) + 4)) break;
    }
}
RT3_REPLACE(0x569400, Sound_EnumProviders);

// 0x5694f0. The provider whose name contains `name`, or 0.
RT3_NOINLINE uint8_t* __stdcall Sound_FindProvider(const char* name) {
    for (int32_t i = 0; i < hook::Global<int32_t>(0xd97dc0); ++i)
        if (ExeStrStr(F<const char*>(Provider(i), 4), name)) return Provider(i);
    return nullptr;
}
RT3_REPLACE(0x5694f0, Sound_FindProvider);

// 0x569540. Changes the 3D speaker type (0x625c78; 0x63 = the detected one,
// 0x625c7c), pausing 400 ms before and 200 ms after.
void __fastcall Sound_SetSpeakerType(uint8_t* s, int, int32_t type, int32_t noApply) {
    int32_t old = hook::Global<int32_t>(0x625c78);
    hook::Global<int32_t>(0x625c78) = type;
    if (noApply || type == old) return;
    int32_t v = type;
    if (type == 0x63) {
        v = hook::Global<int32_t>(0x625c7c);
        if (v < 0 || v > 5 || v == old) return;
    } else if (old == 0x63 && hook::Global<int32_t>(0x625c7c) == type) {
        return;
    }
    ClockMs();
    int32_t until = ClockMs() + 400;
    while (ClockMs() < until) {}
    Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c83ec)(*F<uint32_t*>(s, 0x28), v);
    until = ClockMs() + 200;
    while (ClockMs() < until) {}
}
RT3_REPLACE(0x569540, Sound_SetSpeakerType);

int32_t __fastcall Sound_SpeakerType() { return hook::Global<int32_t>(0x625c78); }
RT3_REPLACE(0x5695d0, Sound_SpeakerType);

// 0x5695e0. Squared distance of a point to the listener (0 when any coordinate is 0).
RT3_NOINLINE double __stdcall Sound_ListenerDistSq(float x, float y, float z) {
    const float zero = *(const float*)0x5c8598;
    if (zero == x || zero == y || zero == z) return zero;
    Sound_TransformPoint(hook::Global<uint8_t*>(0x6d402c), 0, &x, &y, &z);
    return (double)x * x + (double)y * y + (double)z * z;
}
RT3_REPLACE(0x5695e0, Sound_ListenerDistSq);

// 0x569660 / 0x5696b0. Listener position / orientation (+0x30.. / +0x3c..).
void __fastcall Sound_SetListenerPos(uint8_t* s, int, float x, float y, float z) {
    Listener(s);
    F<float>(s, 0x30) = x;
    F<float>(s, 0x38) = z;
    F<float>(s, 0x34) = y;
    Import<void(__stdcall*)(uint32_t, float, float, float)>(0x5c83f0)(F<uint32_t>(s, 0x2c), x, y, z);
}
RT3_REPLACE(0x569660, Sound_SetListenerPos);

void __fastcall Sound_SetListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz) {
    Listener(s);
    F<float>(s, 0x3c) = fx;
    F<float>(s, 0x44) = fz;
    F<float>(s, 0x48) = ux;
    F<float>(s, 0x40) = fy;
    F<float>(s, 0x50) = uz;
    F<float>(s, 0x4c) = uy;
    Import<void(__stdcall*)(uint32_t, float, float, float, float, float, float)>(0x5c83f8)(F<uint32_t>(s, 0x2c), fx, fy,
                                                                                         fz, ux, uy, uz);
}
RT3_REPLACE(0x5696b0, Sound_SetListenerDir);

uint32_t __fastcall Sound_ProviderInfo(uint8_t* s) { return F<uint32_t>(F<uint8_t*>(s, 0x28), 4); }
RT3_REPLACE(0x569720, Sound_ProviderInfo);

// 0x569730. Flags (+8) of the named provider, or of the open one.
uint32_t __fastcall Sound_ProviderFlags(uint8_t* s, int, const char* name) {
    if (!name) return F<uint32_t>(F<uint8_t*>(s, 0x28), 8);
    return F<uint32_t>(Sound_FindProvider(name), 8);
}
RT3_REPLACE(0x569730, Sound_ProviderFlags);

// 0x569750. Takes a voice record from the pool (+8; recycled or new), stamps it
// with a fresh serial (0x625c80, never 0) and returns its handle (+4).
RT3_NOINLINE uint32_t __fastcall Voices_Acquire(uint8_t* v) {
    uint32_t h = 0;
    AilLock();
    uint8_t* pool = F<uint8_t*>(v, 8);
    uint32_t* item;
    if (F<int32_t>(pool, 0xc)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        item = (uint32_t*)fl[0];
        if (item) {
            fl[0] = *item;
            if (item == (uint32_t*)fl[1]) fl[1] = *item;
            fl[2]--;
        }
    } else {
        item = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(F<uint32_t>(pool, 0x10));
    }
    if (item) {
        h = item[1];
        if (++hook::Global<int32_t>(0x625c80) == 0) hook::Global<int32_t>(0x625c80) = 1;
        item[2] = hook::Global<uint32_t>(0x625c80);
    }
    AilUnlock();
    return h;
}
RT3_REPLACE(0x569750, Voices_Acquire);

// 0x5697d0. Returns the voice record for `key` (lookup +4) to the pool.
RT3_NOINLINE void __fastcall Voices_Release(uint8_t* v, int, uint32_t key) {
    AilLock();
    uint32_t* e = (uint32_t*)Hash_Get(F<HashMap*>(v, 4), 0, key);
    uint8_t* pool = F<uint8_t*>(v, 8);
    if (F<int32_t>(pool, 0xc) && (uint32_t)e >= F<uint32_t>(pool, 4) && (uint32_t)e < F<uint32_t>(pool, 8)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        *e = fl[0];
        fl[0] = (uint32_t)e;
        fl[2]++;
        if (!fl[1]) fl[1] = (uint32_t)e;
    } else {
        ExeDelete(e);
    }
    AilUnlock();
}
RT3_REPLACE(0x5697d0, Voices_Release);

// 0x569830. Opens the first available of the preferred 3D providers.
RT3_NOINLINE int32_t __fastcall Sound_OpenPreferredProvider(uint8_t* s) {
    if (F<void*>(s, 0x28)) return 1;
    static const uint32_t names[4] = { 0x5d14d8, 0x5de468, 0x5de494, 0x5de4b4 };
    for (int i = 0; i < 4; ++i) {
        F<uint8_t*>(s, 0x28) = Sound_FindProvider((const char*)names[i]);
        Sound_Open3DProvider(s);
        if (F<void*>(s, 0x28)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x569830, Sound_OpenPreferredProvider);

// 0x5698a0. Opens 3D sound: the named provider (or a preferred one), the system
// (vtbl[0]); then the sample table (+0x14), voice lookup (+0x10) and the pool of
// 64 voice records (+0xc, 0x38 bytes).
int32_t __fastcall Sound3D_Open(uint8_t* s, int, const char* name) {
    Sound_EnumProviders();
    if (hook::Global<int32_t>(0xd97dc0) <= 0) return 0;
    if (name) {
        F<uint8_t*>(s, 0x28) = Sound_FindProvider(name);
        Sound_Open3DProvider(s);
    }
    if (Sound_OpenPreferredProvider(s) != 1) return 0;
    ((void(__thiscall*)(void*, uint32_t))(*(void***)s)[0])(s, 0);
    if (!F<int32_t>(s, 0x54)) {
        Import<void(__stdcall*)(uint32_t)>(0x5c8404)(*F<uint32_t*>(s, 0x28));
        F<void*>(s, 0x28) = nullptr;
        return 0;
    }
    Hash_Init((HashMap*)(s + 0x14), 0, 0x11, 0x40);
    F<uint32_t*>(s, 0x10) = NewZeroed(0x14);
    F<uint32_t*>(s, 0xc) = NewZeroed(0x1c);
    void* buf = Malloc(0xe00);
    Pool_Init(F<NodePool*>(s, 0xc), 0, buf, 0x38, 0x40, 0);
    Hash_Init(F<HashMap*>(s, 0x10), 0, 0x11, 0x40);
    return 1;
}
RT3_REPLACE(0x5698a0, Sound3D_Open);

// 0x569990. Creates up to 32 voices of 3D sound system v (0x5c8408 on the
// device +0x28): the voice table (+4, 7 buckets) and the pool (+8, 0x5c
// bytes each) holding them.
void __fastcall Sound3D_CreateVoices(uint8_t* v, int, int32_t) {
    auto create = hook::Global<uint32_t(__stdcall*)(uint32_t)>(0x5c8408);
    uint32_t voices[0x20];
    int32_t n = 0;
    do {
        voices[n] = create(*F<uint32_t*>(v, 0x28));
        if (!voices[n]) break;
        ++n;
    } while (n < 0x20);
    F<int32_t>(v, 0x54) = n;
    if (!n) return;
    uint32_t* m = ((uint32_t*(__cdecl*)(size_t))0x53b070)(0x14);
    if (m) memset(m, 0, 0x14);
    F<uint32_t*>(v, 4) = m;
    Hash_Init((HashMap*)m, 0, 7, n);
    uint8_t* recs = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)(n * 0x5c));
    uint8_t* r = recs;
    for (int32_t i = 0; i < n; ++i, r += 0x5c) {
        F<uint32_t>(r, 4) = voices[i];
        Hash_Insert(F<HashMap*>(v, 4), 0, voices[i], (int32_t)(uintptr_t)r);
    }
    uint32_t* p = ((uint32_t*(__cdecl*)(size_t))0x53b070)(0x1c);
    if (p) memset(p, 0, 0x1c);
    F<uint32_t*>(v, 8) = p;
    Pool_Init((NodePool*)p, 0, recs, 0x5c, n, 0);
}
RT3_REPLACE(0x569990, Sound3D_CreateVoices);

// 0x569a80. Stops every sample (visitor 0x5318c0 over +0x14) and resets the table.
RT3_NOINLINE void __fastcall Sound_StopAll(uint8_t* s) {
    int32_t ctx[2] = { 1, -1 };
    Sound_WaitIdle();
    AilLock();
    hook::Global<int32_t>(0xccba04) = 1;
    Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, ctx);
    AilUnlock();
    Hash_Init((HashMap*)(s + 0x14), 0, 0x11, 0x40);
    F<int32_t>(s, 0x5c) = 0;
}
RT3_REPLACE(0x569a80, Sound_StopAll);

// 0x569ae0. Sets the effects volume (+0x58, clamped to 0..1) and updates all samples.
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol) {
    int32_t ctx[4] = { 2 };
    const float zero = *(const float*)0x5c8598, one = *(const float*)0x5c85b4;
    float v;
    if (zero > vol) v = zero;
    else if (one < vol) v = one;
    else v = vol;
    F<float>(s, 0x58) = v;
    Sound_WaitIdle();
    AilLock();
    if (F<int32_t>(s, 0x5c) > 0) {
        hook::Global<int32_t>(0xccba04) = 1;
        Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, ctx);
    }
    AilUnlock();
}
RT3_REPLACE(0x569ae0, Sound_SetEffectsVolume);

// 0x569b70. Frees a queued sample entry and recycles it in the pool (+0xc).
RT3_NOINLINE void __fastcall SampleQueue_Free(uint8_t* q, int, uint32_t* e) {
    ((void(__cdecl*)(void*))0x5a1145)((void*)e[4]);
    e[4] = 0;
    Hash_Remove(F<HashMap*>(q, 0x10), 0, e[0]);
    uint8_t* pool = F<uint8_t*>(q, 0xc);
    if (F<int32_t>(pool, 0xc) && (uint32_t)e >= F<uint32_t>(pool, 4) && (uint32_t)e < F<uint32_t>(pool, 8)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        e[0] = fl[0];
        fl[0] = (uint32_t)e;
        fl[2]++;
        if (!fl[1]) fl[1] = (uint32_t)e;
        return;
    }
    ((void(__cdecl*)(void*))0x53b080)(e);
}
RT3_REPLACE(0x569b70, SampleQueue_Free);

// 0x569be0. Retires a finished 3D sample: frees its voice, removes it from the
// table and drops its shared data when unused.
RT3_NOINLINE void __fastcall Sound3D_Retire(uint8_t* s, int, uint32_t key, uint32_t* smp) {
    if (smp[2] == 1) F<int32_t>(s, 0x5c)--;
    Voices_Release(s, 0, key);
    Hash_Remove((HashMap*)(s + 0x14), 0, smp[0]);
    F<uint32_t>(smp, 0x44) &= 0xfffffeff;
    uint32_t* info = (uint32_t*)smp[1];
    info[3]--;
    if ((int32_t)((uint32_t*)smp[1])[3] <= 0) {
        SampleQueue_Free(s, 0, (uint32_t*)smp[1]);
        smp[1] = 0;
    }
}
RT3_REPLACE(0x569be0, Sound3D_Retire);

// 0x569c40. Stops a 3D sample (given, or looked up by key) and retires it.
RT3_NOINLINE void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp) {
    Sound_WaitIdle();
    Import<void(__stdcall*)()>(0x5c8484)();
    if (!smp && key) smp = (uint8_t*)Hash_Get((HashMap*)(s + 0x14), 0, key);
    if (smp) {
        uint32_t h = F<uint32_t>(smp, -4);
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a4)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8414)(h);
        Sound3D_Retire(s, 0, h, (uint32_t*)smp);
    }
    Import<void(__stdcall*)()>(0x5c83e4)();
}
RT3_REPLACE(0x569c40, Sound3D_Stop);

// 0x569ca0. Pauses every sample (visitor mode 3).
void __fastcall Sound_PauseAll(uint8_t* s) {
    int32_t ctx[4] = { 3 };
    Sound_WaitIdle();
    AilLock();
    hook::Global<int32_t>(0xccba04) = 1;
    Hash_ForEach((HashMap*)(s + 0x14), 0, 0, (HashVisit)0x5318c0, ctx);
    F<int32_t>(s, 0x60) = 0;
    AilUnlock();
}
RT3_REPLACE(0x569ca0, Sound_PauseAll);

// 0x569cf0. Flags the sample of voice `key` for restart (0x100).
void __fastcall Sound_FlagVoice(uint8_t* s, int, uint32_t key) {
    uint8_t* v = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    uint8_t* smp = (uint8_t*)Hash_Get((HashMap*)(s + 0x14), 0, F<uint32_t>(v, 8));
    if (smp) F<uint32_t>(smp, 0x44) |= 0x100;
}
RT3_REPLACE(0x569cf0, Sound_FlagVoice);

// 0x569d20. Frees the voice table's hash (+4) and node pool (+8).
RT3_NOINLINE void __fastcall Voices_Free(uint8_t* v) {
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

// 0x569d90. Closes the 3D sound provider (+0x28): stops everything, closes the
// listener (+0x2c), frees the sample tables (+0x14 inline, +0xc pool, +0x10 hash),
// applies a pending speaker type change (0x625c7c -> 0x625c78, 0..5, with pauses).
RT3_NOINLINE void __fastcall Sound3D_Close(uint8_t* s) {
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

// 0x569ea0. Shuts the 3D sound system v down: releases its voices (+0x54,
// through 0x5c841c), the voice table (+4) and the pool (+8, its buffer
// +4 first).
void __fastcall Sound3D_Shutdown(uint8_t* v) {
    if (F<int32_t>(v, 0x54) > 0) {
        auto release = hook::Global<void(__stdcall*)(uint32_t)>(0x5c841c);
        for (int32_t i = 0; i < F<int32_t>(v, 0x54); ++i) {
            const uint32_t h = Voices_Acquire(v);
            if (!h) break;
            release(h);
        }
    }
    F<int32_t>(v, 0x54) = 0;
    F<int32_t>(v, 0x5c) = 0;
    if (F<HashMap*>(v, 4)) {
        Hash_Destroy(F<HashMap*>(v, 4));
        if (HashMap* m = F<HashMap*>(v, 4)) {
            Hash_DestroyThunk(m);
            Free446(m);
        }
        F<HashMap*>(v, 4) = nullptr;
    }
    if (F<uint8_t*>(v, 8) && F<void*>(F<uint8_t*>(v, 8), 4)) {
        ((void(__cdecl*)(void*))0x5a1145)(F<void*>(F<uint8_t*>(v, 8), 4));
        if (NodePool* p = F<NodePool*>(v, 8)) {
            Pool_Free(p);
            Free446(p);
        }
        F<NodePool*>(v, 8) = nullptr;
    }
}
RT3_REPLACE(0x569ea0, Sound3D_Shutdown);

// 0x569f30. Applies command cmd ({op, arg, out, result}) to 3D sample entry h (+4:
// the queue for op 0, else the sample record, its Miles handle just before it):
// 0 frees an empty / any queue, 1 stops (by id or any), 2 sets the volume, 3 pushes
// the record's dirty state (+0x44 | the system's +0x60: volume, position via the
// view transform rounded to whole units, distances, orientation, effects, rate,
// pause / delayed resume, 0x100 retires), 4 / 5 pause (5 only looping), 6 / 7
// resume, 8 finds the nearest audible sample of priority <= arg (retiring finished
// ones, returning 1).
RT3_NOINLINE int32_t __fastcall Sound3D_Command(uint8_t* sys, int, uint8_t* h, int32_t* cmd) {
    uint8_t* queue = nullptr;
    uint32_t* smp = nullptr;
    uint32_t handle = 0;
    if (cmd[0] == 0) {
        queue = F<uint8_t*>(h, 4);
    } else {
        smp = F<uint32_t*>(h, 4);
        handle = smp[-1];
    }
    uint8_t* r = (uint8_t*)smp;
    auto volume = [&]() {
        float v = (float)((double)F<float>(r, 0x28) * F<float>(sys, 0x58) * hook::Global<float>(0x624d5c));
        Import<void(__stdcall*)(uint32_t, float)>(0x5c842c)(handle, v);
    };
    auto stop = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c8410)(handle); };
    auto resume = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c840c)(handle); };
    auto paused = [&]() { return F<int32_t>(r, 0xc) == 1 || (r[0x44] & 8); };
    switch (cmd[0]) {
    case 0:
        if (F<int32_t>(queue, 0xc) <= 0 || cmd[1] == -1) SampleQueue_Free(sys, 0, (uint32_t*)queue);
        return 0;
    case 1:
        if (cmd[1] == -1 || F<int32_t>(r, 4) == cmd[1]) Sound3D_Stop(sys, 0, 0, r);
        return 0;
    case 2:
        volume();
        return 0;
    case 3: {
        uint32_t f = F<uint32_t>(sys, 0x60) | F<uint32_t>(r, 0x44);
        if (!f) return 0;
        F<uint32_t>(r, 0x44) = 0;
        if (f & 0x100) {
            Sound3D_Retire(sys, 0, handle, smp);
            return 0;
        }
        if (f & 1) volume();
        if (f & 2) {
            float x = F<float>(r, 0x10), y = F<float>(r, 0x14), z = F<float>(r, 0x18);
            if (!(Fl(0x5c8598) == x) || !(Fl(0x5c8598) == y) || !(Fl(0x5c8598) == z)) {
                Sound_TransformPoint(hook::Global<uint8_t*>(0x6d402c), 0, &x, &y, &z);
                x = (float)Ftol(x);
                y = (float)Ftol(y);
                z = (float)Ftol(z);
                Import<void(__stdcall*)(uint32_t, float, float, float)>(0x5c83f0)(handle, x, y, z);
            }
        }
        if (f & 0x10) Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8428)(handle, F<uint32_t>(r, 0x34), F<uint32_t>(r, 0x30));
        if (f & 0x20)
            Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c83f8)(
                handle, F<uint32_t>(r, 0x1c), F<uint32_t>(r, 0x20), F<uint32_t>(r, 0x24), 0, 0x3f800000, 0);
        if (f & 0x40) Import<void(__stdcall*)(uint32_t, float)>(0x5c8424)(handle, (float)F<int32_t>(r, 0x38));
        if ((int8_t)f < 0 && F<int32_t>(r, 0x3c)) Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c8420)(handle, F<int32_t>(r, 0x3c));
        if (!(f & 8)) return 0;
        if (F<int32_t>(r, 0xc) == 1) {
            stop();
            return 0;
        }
        if (F<int32_t>(r, 0x50) > 0) {
            int32_t now = (int32_t)Import<DWORD(WINAPI*)()>(0x5c8068)();
            if (now - F<int32_t>(r, 0x4c) < F<int32_t>(r, 0x50)) {
                F<uint32_t>(r, 0x44) |= 8;
                return 0;
            }
            F<int32_t>(r, 0x50) = 0;
        }
        resume();
        return 0;
    }
    case 4:
        if (F<int32_t>(r, 0xc) && !(r[0x44] & 8)) return 0;
        F<int32_t>(r, 0xc) = 1;
        F<uint32_t>(r, 0x44) &= ~8u;
        stop();
        return 0;
    case 5:
        if (F<int32_t>(r, 8) != 1) return 0;
        if (F<int32_t>(r, 0xc) && !(r[0x44] & 8)) return 0;
        F<int32_t>(r, 0xc) = 1;
        F<uint32_t>(r, 0x44) &= ~8u;
        stop();
        return 0;
    case 6:
    case 7:
        if (cmd[0] == 7 && F<int32_t>(r, 8) != 1) return 0;
        if (!paused()) return 0;
        {
            uint32_t fl = F<uint32_t>(r, 0x44) & ~8u;
            F<int32_t>(r, 0xc) = 0;
            F<uint32_t>(r, 0x44) = fl;
        }
        resume();
        return 0;
    case 8: {
        if (F<uint32_t>(r, 0x44) & 0x100) {
            cmd[3] = -1;
            Sound3D_Retire(sys, 0, handle, smp);
            return 1;
        }
        if (F<int32_t>(r, 0x40) > cmd[1]) return 0;
        float* best = (float*)cmd[2];
        double d = Sound_ListenerDistSq(F<float>(r, 0x10), F<float>(r, 0x14), F<float>(r, 0x18));
        if (F<int32_t>(r, 0x40) < cmd[1]) {
            cmd[1] = F<int32_t>(r, 0x40);
            cmd[3] = (int32_t)smp[0];
            *best = (float)d;
        } else if (d > *best) {
            cmd[3] = (int32_t)smp[0];
            *best = (float)d;
        }
        return 0;
    }
    }
    return 0;
}
RT3_REPLACE(0x569f30, Sound3D_Command);

// 0x56a330. A voice for 3D sound `name` at (x, y, z): when none is free, the
// farthest-off playing sample (Hash_ForEach 0x5318c0 over +0x14 with the listener
// distance) is stopped and released and the search retried (0 if nothing can go).
// The voice gets the sound's shared sample data (+0x10 hash of {crc, ..., refs +0xc,
// data +0x10, info +0x14}, loaded through the sound cache).
RT3_NOINLINE uint8_t* __fastcall Sound3D_PrepareSample(uint8_t* s, int, const char* name, int32_t prio, float x, float y, float z) {
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

// 0x56a520. Plays 3D sound `name` at (x, y, z) (transformed to listener space
// unless the origin): volume vol x the 3D master (0x624d5c x +0x58), distances
// maxDist (2048 by default) / x 16, looping when mode == 1 (counted in +0x5c),
// started unless `paused`. Returns the voice key (0 if none).
uint32_t __fastcall Sound3D_Play(uint8_t* s, int, const char* name, float x, float y, float z, float vol, int32_t mode,
                                 int32_t paused, int32_t prio, float maxDist) {
    uint32_t* res = (uint32_t*)Sound3D_PrepareSample(s, 0, name, prio, x, y, z);
    if (!res) return 0;
    uint32_t smp = res[-1];
    Register((HashMap*)(s + 0x14), res);
    F<uint32_t>(res, 0x44) = 0;
    F<uint32_t>(res, 0x50) = 0;
    F<float>(res, 0x14) = y;
    F<int32_t>(res, 0x40) = prio;
    F<float>(res, 0x10) = x;
    F<float>(res, 0x18) = z;
    float px = x, py = y, pz = z;
    const float zero = *(const float*)0x5c8598;
    if (zero != x || zero != y || zero != z) Sound_TransformPoint(Snd(), 0, &px, &py, &pz);
    float d = maxDist > zero ? maxDist : *(const float*)0x5cf0e8;
    F<float>(res, 0x30) = d;
    F<float>(res, 0x34) = (float)((double)d * *(const float*)0x5c8948);
    F<float>(res, 0x28) = vol;
    F<int32_t>(res, 8) = mode;
    F<int32_t>(res, 0xc) = paused;
    Import<void(__stdcall*)(uint32_t, void*)>(0x5c843c)(smp, F<void*>((uint8_t*)res[1], 0x10));
    int32_t loop = 1;
    if (mode == 1) {
        ++F<int32_t>(s, 0x5c);
        loop = 0;
    }
    Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c8438)(smp, loop);
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c83f0)(smp, Bits(px), Bits(py), Bits(pz));
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c8424)(smp, 0);
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c8434)(smp, 0);
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8428)(smp, F<uint32_t>(res, 0x34), F<uint32_t>(res, 0x30));
    float v = (float)((double)*(const float*)0x624d5c * F<float>(s, 0x58) * F<float>(res, 0x28));
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c842c)(smp, Bits(v));
    Sound_RegisterEos(Snd(), 0, smp, 1);
    if (!paused) Import<void(__stdcall*)(uint32_t)>(0x5c8430)(smp);
    F<uint32_t>(res, 0x4c) = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    return res[0];
}
RT3_REPLACE(0x56a520, Sound3D_Play);

// 0x56a6e0. 2D sound system constructor.
uint8_t* __fastcall Sound2D_Construct(uint8_t* s) {
    static const uint8_t zero[] = { 4, 8, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x54, 0x28, 0x5c, 0x60, 0xc, 0x10,
                                    0x2c, 0x30, 0x34, 0x38, 0x3c, 0x40, 0x48, 0x50 };
    for (uint8_t off : zero) F<int32_t>(s, off) = 0;
    F<uint32_t>(s, 0) = 0x5de4cc;
    F<uint32_t>(s, 0x58) = 0x3f800000;
    F<uint32_t>(s, 0x44) = 0x3f800000;
    F<uint32_t>(s, 0x4c) = 0x3f800000;
    return s;
}
RT3_REPLACE(0x56a6e0, Sound2D_Construct);

// 0x56a740. Destructor of the 3D sound system (vtable 0x5de4cc, base 0x5de460).
void __fastcall Sound3D_Destruct(uint8_t* s) {
    F<uint32_t>(s, 0) = 0x5de4cc;
    Sound3D_Close(s);
    Hash_DestroyThunk((HashMap*)(s + 0x14));
    F<uint32_t>(s, 0) = 0x5de460;
    Voices_Free(s);
}
RT3_REPLACE(0x56a740, Sound3D_Destruct);

// 0x56a770. Opens the 22 kHz 16-bit stereo digital driver (+0x2c) and initialises.
int32_t __fastcall Sound2D_Open(uint8_t* s) {
    Import<int32_t(__stdcall*)(uint32_t, int32_t)>(0x5c8444)(0, 100);
    void* drv = Import<void*(__stdcall*)(uint32_t, uint32_t, uint32_t, uint32_t)>(0x5c8440)(0x5622, 0x10, 2, 0);
    F<void*>(s, 0x2c) = drv;
    if (!drv) return 0;
    Hash_Init((HashMap*)(s + 0xc), 0, 0x11, 0x40);
    ((void(__thiscall*)(void*, uint32_t))(*(void***)s)[0])(s, F<uint32_t>(s, 0x28));
    return 1;
}
RT3_REPLACE(0x56a770, Sound2D_Open);

// 0x56a7c0. Allocates n Miles sample handles (records of 0x5c bytes) with a
// handle -> record lookup (+4) and a free pool (+8).
void __fastcall Sound2D_AllocSamples(uint8_t* s, int, int32_t n) {
    F<uint32_t*>(s, 4) = NewZeroed(0x14);
    Hash_Init(F<HashMap*>(s, 4), 0, 7, n);
    uint8_t* recs = (uint8_t*)Malloc(n * 0x5c);
    uint8_t* r = recs;
    for (int32_t i = 0; i < n; ++i, r += 0x5c) {
        uint32_t h = Import<uint32_t(__stdcall*)(uint32_t)>(0x5c8448)(F<uint32_t>(s, 0x2c));
        F<uint32_t>(r, 4) = h;
        Hash_Insert(F<HashMap*>(s, 4), 0, h, (int32_t)r);
    }
    F<uint32_t*>(s, 8) = NewZeroed(0x1c);
    Pool_Init(F<NodePool*>(s, 8), 0, recs, 0x5c, n, 0);
}
RT3_REPLACE(0x56a7c0, Sound2D_AllocSamples);

// 0x56a880. Stops every 2D sample and resets the table.
RT3_NOINLINE void __fastcall Sound2D_StopAll(uint8_t* s) {
    Sound_WaitIdle();
    AilLock();
    VisitAll(s, 1, 0);
    AilUnlock();
    Hash_Init((HashMap*)(s + 0xc), 0, 0x11, 0x40);
    F<int32_t>(s, 0x24) = 0;
}
RT3_REPLACE(0x56a880, Sound2D_StopAll);

// 0x56a8e0. Sets the 2D effects volume (+0x20, clamped to 0..1) and updates the samples.
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol) {
    const float zero = *(const float*)0x5c8598, one = *(const float*)0x5c85b4;
    float v;
    if (zero > vol) v = zero;
    else if (one < vol) v = one;
    else v = vol;
    F<float>(s, 0x20) = v;
    Sound_WaitIdle();
    AilLock();
    if (F<int32_t>(s, 0x24) > 0) {
        int32_t ctx[4] = { 2 };
        hook::Global<int32_t>(0xccba04) = 0;
        Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
    }
    AilUnlock();
}
RT3_REPLACE(0x56a8e0, Sound2D_SetVolume);

// 0x56a970. Applies command ctx[0] to one 2D sample (table node -> entry e, Miles
// handle e[-4]): 1 end it (only entry ctx[1] unless -1), 2 refresh volume of a
// looping one, 3 apply its pending changes (+0x44: 0x100 drop, 0x200 loop count,
// 1 volume, 4 volume + pan, 0x80 rate, 8 stop / delayed resume), 4/5 pause (5 only
// looping ones), 6/7 resume (7 only looping ones). Always 0.
RT3_NOINLINE int32_t __fastcall Sound2D_Command(uint8_t* s, int, uint8_t* node, int32_t* ctx) {
    uint8_t* e = F<uint8_t*>(node, 4);
    uint32_t h = F<uint32_t>(e, -4);
    auto volume = [&]() { return (float)((double)F<float>(e, 0x28) * F<float>(s, 0x20)); };
    auto stop = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c846c)(h); };
    auto resume = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c8468)(h); };
    auto pause = [&]() {
        if (F<int32_t>(e, 0xc) && !(F<uint32_t>(e, 0x44) & 8)) return;
        F<int32_t>(e, 0xc) = 1;
        F<uint32_t>(e, 0x44) &= ~8u;
        stop();
    };
    auto unpause = [&]() {
        if (F<int32_t>(e, 0xc) != 1 && !(F<uint32_t>(e, 0x44) & 8)) return;
        F<uint32_t>(e, 0x44) &= ~8u;
        F<int32_t>(e, 0xc) = 0;
        resume();
    };
    switch (ctx[0]) {
    case 1:
        if (ctx[1] != -1 && F<int32_t>(e, 4) != ctx[1]) break;
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8474)(h);
        Sample_Drop(s, e, h);
        break;
    case 2:
        if (F<int32_t>(e, 8) == 1) {
            float v = volume();
            Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(h, v, v);
        }
        break;
    case 3: {
        uint32_t flags = F<uint32_t>(e, 0x44);
        if (!flags) break;
        F<uint32_t>(e, 0x44) = 0;
        if (flags & 0x100) {
            if (F<int32_t>(e, 8) == 1) --F<int32_t>(s, 0x24);
            Sample_Drop(s, e, h);
            break;
        }
        if (flags & 0x200) Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c845c)(h, F<int32_t>(e, 0x48));
        if (flags & 1) {
            float v = volume();
            Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(h, v, v);
        }
        if (flags & 4) Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8458)(h, volume(), F<float>(e, 0x2c));
        if (flags & 0x80) Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c844c)(h, F<int32_t>(e, 0x3c));
        if (!(flags & 8)) break;
        if (F<int32_t>(e, 0xc) == 1) {
            stop();
            break;
        }
        if (F<int32_t>(e, 0x50) > 0) {
            uint32_t now = Import<uint32_t(__stdcall*)()>(0x5c8068)();
            if ((int32_t)(now - F<uint32_t>(e, 0x4c)) < F<int32_t>(e, 0x50)) {
                F<uint32_t>(e, 0x44) |= 8;
                break;
            }
            F<int32_t>(e, 0x50) = 0;
        }
        resume();
        break;
    }
    case 4:
        pause();
        break;
    case 5:
        if (F<int32_t>(e, 8) == 1) pause();
        break;
    case 6:
        unpause();
        break;
    case 7:
        if (F<int32_t>(e, 8) == 1) unpause();
        break;
    }
    return 0;
}
RT3_REPLACE(0x56a970, Sound2D_Command);

// 0x56abe0. A voice for 2D sound `name`, its data loaded through the sound cache
// when `load` == 1 (else none); 0 when no voice is free.
RT3_NOINLINE uint8_t* __fastcall Sound_PrepareSample(uint8_t* s, int, const char* name, int32_t load) {
    uint8_t* res = AcquireVoice(s);
    if (!res) return nullptr;
    if (load == 1) F<void*>(res, 4) = SoundCache_Load(*hook::Global<uint8_t**>(0x6d402c), 0, name, nullptr);
    else F<void*>(res, 4) = nullptr;
    return res;
}
RT3_REPLACE(0x56abe0, Sound_PrepareSample);

// 0x56ac50. Stops sample `key` (end callback cleared) and frees its data.
void __fastcall Sound2D_Stop(uint8_t* s, int, uint32_t key) {
    Sound_WaitIdle();
    AilLock();
    uint8_t* e = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, key);
    if (e) {
        uint32_t h = F<uint32_t>(e, -4);
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8474)(h);
        if (F<int32_t>(e, 8) == 1) F<int32_t>(s, 0x24)--;
        Voices_Release(s, 0, h);
        Hash_Remove((HashMap*)(s + 0xc), 0, key);
        if (void* d = F<void*>(e, 4)) {
            Free(d);
            F<void*>(e, 4) = nullptr;
        }
    }
    AilUnlock();
}
RT3_REPLACE(0x56ac50, Sound2D_Stop);

// 0x56acd0. Sets sample `key`'s volume (scaled by the effects volume).
void __fastcall Sound2D_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol) {
    Sound_WaitIdle();
    AilLock();
    uint8_t* e = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, key);
    if (e) {
        float v = (float)((double)vol * F<float>(s, 0x20));
        Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(F<uint32_t>(e, -4), v, v);
    }
    AilUnlock();
}
RT3_REPLACE(0x56acd0, Sound2D_SetSampleVolume);

// 0x56ad20. Plays 2D sound `name` (or the given data) at volume vol x the 2D master
// (+0x20) and pan, looping when mode == 1 (counted in +0x24), started unless
// `paused`. Returns the voice key (0 if none).
uint32_t __fastcall Sound2D_Play(uint8_t* s, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data) {
    uint32_t* res = (uint32_t*)Sound_PrepareSample(s, 0, name, data == nullptr);
    if (!res) return 0;
    uint32_t smp = res[-1];
    Register((HashMap*)(s + 0xc), res);
    F<uint32_t>(res, 0x50) = 0;
    F<uint32_t>(res, 0x44) = 0;
    F<float>(res, 0x28) = vol;
    F<uint32_t>(res, 0x2c) = pan;
    F<int32_t>(res, 8) = mode;
    F<int32_t>(res, 0xc) = paused;
    Import<void(__stdcall*)(uint32_t)>(0x5c83b0)(smp);
    if (!data) data = (void*)res[1];
    Import<void(__stdcall*)(uint32_t, void*, int32_t)>(0x5c8480)(smp, data, 0);
    float v = (float)((double)vol * F<float>(s, 0x20));
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8470)(smp, Bits(v), Bits(v));
    float v2 = (float)((double)vol * F<float>(s, 0x20));
    Import<void(__stdcall*)(uint32_t, uint32_t, uint32_t)>(0x5c8458)(smp, Bits(v2), pan);
    int32_t loop = 1;
    if (mode == 1) {
        ++F<int32_t>(s, 0x24);
        loop = 0;
    }
    Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c845c)(smp, loop);
    Sound_RegisterEos(Snd(), 0, smp, 0);
    if (!paused) Import<void(__stdcall*)(uint32_t)>(0x5c8478)(smp);
    F<uint32_t>(res, 0x4c) = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    return res[0];
}
RT3_REPLACE(0x56ad20, Sound2D_Play);

// 0x56ae30. Pauses every 2D sample.
void __fastcall Sound2D_PauseAll(uint8_t* s) {
    Sound_WaitIdle();
    AilLock();
    int32_t ctx[4] = { 3 };
    hook::Global<int32_t>(0xccba04) = 0;
    Hash_ForEach((HashMap*)(s + 0xc), 0, 0, (HashVisit)0x5318c0, ctx);
    AilUnlock();
}
RT3_REPLACE(0x56ae30, Sound2D_PauseAll);

void __fastcall Sound2D_FlagVoice(uint8_t* s, int, uint32_t key) {
    uint8_t* v = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    uint8_t* smp = (uint8_t*)Hash_Get((HashMap*)(s + 0xc), 0, F<uint32_t>(v, 8));
    if (smp) F<uint32_t>(smp, 0x44) |= 0x100;
}
RT3_REPLACE(0x56ae80, Sound2D_FlagVoice);

// 0x56aeb0. Closes 2D sound: stops everything, frees the table, closes the driver (vtbl +4).
void __fastcall Sound2D_Close(uint8_t* s) {
    if (!F<void*>(s, 0x2c)) return;
    Sound2D_StopAll(s);
    Hash_Destroy((HashMap*)(s + 0xc));
    ((void(__thiscall*)(void*))(*(void***)s)[1])(s);
    F<void*>(s, 0x2c) = nullptr;
}
RT3_REPLACE(0x56aeb0, Sound2D_Close);

// 0x56aee0. Sound mixer object constructor.
uint8_t* __fastcall Mixer_Construct(uint8_t* m) {
    for (int k = 1; k < 8; ++k) F<int32_t>(m, k * 4) = 0;
    F<uint32_t>(m, 0) = 0x5de4d4;
    F<uint32_t>(m, 0x20) = 0x3f800000;
    F<int32_t>(m, 0x24) = 0;
    F<int32_t>(m, 0x2c) = 0;
    F<int32_t>(m, 0x28) = 8;
    return m;
}
RT3_REPLACE(0x56aee0, Mixer_Construct);

// 0x56af20. Destructor of the 2D sound system (vtable 0x5de4d4, base 0x5de460).
void __fastcall Sound2D_Destruct(uint8_t* s) {
    F<uint32_t>(s, 0) = 0x5de4d4;
    if (F<void*>(s, 0x2c)) {
        Sound2D_StopAll(s);
        Hash_Destroy((HashMap*)(s + 0xc));
        ((void(__thiscall*)(void*))(*(void***)s)[1])(s);
        F<void*>(s, 0x2c) = nullptr;
    }
    Hash_DestroyThunk((HashMap*)(s + 0xc));
    F<uint32_t>(s, 0) = 0x5de460;
    Voices_Free(s);
}
RT3_REPLACE(0x56af20, Sound2D_Destruct);

// 0x56af70. Block flag grid for a w x h world area (cells +0/+4; 4x4 blocks +0x14).
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h) {
    for (int k = 0; k < 16; ++k) F<int32_t>(g, 0x14 + k * 4) = 0;
    F<float>(g, 8) = w;
    F<float>(g, 0xc) = h;
    F<int32_t>(g, 0) = CellOf(w);
    F<int32_t>(g, 4) = CellOf(h);
    F<int32_t>(g, 0x10) = 0;
    return g;
}
RT3_REPLACE(0x56af70, BlockGrid_Init);

void __fastcall BlockGrid_Free(uint8_t* g) {
    for (int k = 0; k < 16; ++k)
        if (void* p = F<void*>(g, 0x14 + k * 4)) Free(p);
}
RT3_REPLACE(0x56afc0, BlockGrid_Free);

void __fastcall BlockGrid_Set(uint8_t* g, int, int32_t, int32_t a, int32_t b, uint8_t v) {
    if (uint8_t* c = Cell(g, a, b)) *c = (uint8_t)((*c & 0xf0) | (v & 0xf));
}
RT3_REPLACE(0x56b000, BlockGrid_Set);

uint8_t __fastcall BlockGrid_Get(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* c = Cell(g, a, b);
    return c ? (uint8_t)(*c & 0xf) : 0;
}
RT3_REPLACE(0x56b070, BlockGrid_Get);

// 0x56b0e0. Decrements cell (a, b)'s counter (and the totals); frees an emptied block.
int32_t __fastcall BlockGrid_Dec(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t** slot = Slot(g, a, b);
    uint8_t* blk = *slot;
    if (blk) {
        uint8_t* c = CellIn(blk, a, b);
        uint8_t v = *c;
        uint8_t n = v >> 4;
        if (n & 7) {
            *c = (uint8_t)((v & 0x8f) | ((uint8_t)((n - 1) << 4) & 0x70));
            F<uint16_t>(blk, 0)--;
            F<int32_t>(g, 0x10)--;
        }
        if (!F<uint16_t>(blk, 0)) {
            Free(*slot);
            *slot = nullptr;
        }
    }
    return F<int32_t>(g, 0x10);
}
RT3_REPLACE(0x56b0e0, BlockGrid_Dec);

uint32_t __fastcall BlockGrid_Count(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* blk = *Slot(g, a, b);
    return blk ? (*CellIn(blk, a, b) >> 4) & 7 : 0;
}
RT3_REPLACE(0x56b180, BlockGrid_Count);

void __fastcall BlockGrid_SetCount(uint8_t* g, int, int32_t a, int32_t b, uint8_t n) {
    uint8_t* blk = *Slot(g, a, b);
    if (!blk) return;
    uint8_t* c = CellIn(blk, a, b);
    *c = (uint8_t)((*c & 0x8f) | ((uint8_t)(n << 4) & 0x70));
}
RT3_REPLACE(0x56b1f0, BlockGrid_SetCount);

uint32_t __fastcall BlockGrid_Byte(uint8_t* g, int, int32_t a, int32_t b) {
    uint8_t* blk = *Slot(g, a, b);
    return blk ? CellIn(blk, a, b)[1] : 0;
}
RT3_REPLACE(0x56b260, BlockGrid_Byte);

// 0x56b2c0. Height range of the 5x5 vertex block (row, col) of the grid (origin
// +0 / +4 vertices): out (+0x14 table) +2 = the middle, +6 = the spread x 0x5de4dc.
RT3_NOINLINE void __fastcall Block_HeightRange(uint8_t* o, int, int32_t row, int32_t col, uint8_t* t) {
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

// 0x56b490 / 0x56b510. Loads / saves the grid header (0x54 bytes) and its 16
// blocks (0x8a bytes each, preceded by a present flag).
void __fastcall BlockGrid_Load(uint8_t* g, int, Stream s) {
    StreamRead(s, g, 0x54, 0, 1);
    for (int32_t row = 5; row < 0x15; row += 4)
        for (int32_t k = 0; k < 4; ++k) {
            uint8_t flag = 0;
            StreamRead(s, &flag, 1, 0, 1);
            if (!flag) continue;
            void* blk = Malloc(0x8a);
            F<void*>(g, (row + k) * 4) = blk;
            StreamRead(s, blk, 0x8a, 0, 1);
        }
}
RT3_REPLACE(0x56b490, BlockGrid_Load);

void __fastcall BlockGrid_Save(uint8_t* g, int, Stream s) {
    StreamWrite(s, g, 0x54, 0);
    for (int k = 0; k < 16; ++k) {
        void* blk = F<void*>(g, 0x14 + k * 4);
        uint8_t flag = blk != nullptr;
        StreamWrite(s, &flag, 1, 0);
        if (F<void*>(g, 0x14 + k * 4)) StreamWrite(s, F<void*>(g, 0x14 + k * 4), 0x8a, 0);
    }
}
RT3_REPLACE(0x56b510, BlockGrid_Save);

// 0x56b580. Adds a point of `kind` to cell (x, z) of the 4x4-block grid (blocks +0x14,
// created on demand with their height range and the terrain region refreshed;
// 16 cells of {flags (count in bits 4-6, max 6), kind, heights}), counting +0x10.
void __fastcall Grid_AddPoint(uint8_t* g, int, uint8_t* t, int32_t x, int32_t z, uint8_t kind) {
    int32_t bx = x / 4, bz = z / 4;
    uint8_t** slot = &F<uint8_t*>(g, 0x14 + (bz + bx * 4) * 4);
    if (!*slot) {
        uint8_t* blk = (uint8_t*)Malloc(0x8a);
        *slot = blk;
        for (int k = 0; k < 0x8a; ++k) blk[k] = 0;
        Block_HeightRange(g, 0, bx, bz, t);
        int32_t x0 = F<int32_t>(g, 0) + bx * 4, z0 = F<int32_t>(g, 4) + bz * 4;
        ((void(__thiscall*)(void*, int32_t, int32_t, int32_t, int32_t))(*(void***)t)[0])(t, x0, z0, x0 + 3, z0 + 3);
    }
    uint8_t* blk = *slot;
    uint8_t* cell = blk + 0xa + (z % 4 + (x % 4) * 4) * 8;
    uint8_t flags = cell[0];
    cell[1] = kind;
    if ((uint8_t)(flags & 0x70) >= 0x60) return;
    float px, pz;
    PointPos(t, x, z, (flags >> 4) & 7, kind, &px, &pz);
    StorePoint(g, t, blk, cell, (cell[0] >> 4) & 7, px, pz);
    ++F<uint16_t>(blk, 0);
    uint8_t f = cell[0];
    cell[0] = (uint8_t)(((((f & 0xf0) + 0x10) ^ f) & 0x70) ^ f);
    ++F<int32_t>(g, 0x10);
}
RT3_REPLACE(0x56b580, Grid_AddPoint);

// 0x56b700. Sets cell (a, b)'s byte and marks the scene for redraw.
void __fastcall BlockGrid_SetByte(uint8_t* g, int, int32_t a, int32_t b, uint8_t v) {
    uint8_t* blk = *Slot(g, a, b);
    if (!blk) return;
    CellIn(blk, a, b)[1] = v;
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x374a] = 1;
}
RT3_REPLACE(0x56b700, BlockGrid_SetByte);

// 0x56b770. Recomputes the height range of every block and the heights of all
// their points.
void __fastcall Grid_RefreshPoints(uint8_t* g, int, uint8_t* t) {
    for (int32_t row = 0; row < 4; ++row)
        for (int32_t col = 0; col < 4; ++col) {
            uint8_t* blk = F<uint8_t*>(g, 0x14 + (row * 4 + col) * 4);
            if (!blk) continue;
            Block_HeightRange(g, 0, row, col, t);
            uint8_t* cell = blk + 0xa;
            for (int32_t sx = 0; sx < 4; ++sx)
                for (int32_t sz = 0; sz < 4; ++sz, cell += 8) {
                    uint8_t n = (cell[0] >> 4) & 7;
                    for (uint8_t k = 0; k < n; n = (cell[0] >> 4) & 7, ++k) {
                        float px, pz;
                        PointPos(t, sx + row * 4, sz + col * 4, k, cell[1], &px, &pz);
                        StorePoint(g, t, blk, cell, k, px, pz);
                    }
                }
        }
}
RT3_REPLACE(0x56b770, Grid_RefreshPoints);

// 0x56b8f0. Refill callback in ESI.
__declspec(naked) void StreamA_Get() {
    __asm {
        push esi
        call StreamA_GetImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x56b8f0, StreamA_Get);

// 0x56b940. Appends one byte to stream A (flush callback in ESI).
__declspec(naked) void StreamA_Put() {
    __asm {
        mov eax, [esp + 4]
        push esi
        push eax
        call StreamA_PutImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56b940, StreamA_Put);

// 0x56b990. Coder in EBX, position on the stack.
__declspec(naked) void Tree_InsertNode() {
    __asm {
        push [esp + 4]
        push ebx
        call Tree_InsertNodeImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56b990, Tree_InsertNode);

// 0x56bb10. Node in EDX, state on the stack.
__declspec(naked) void Tree_DeleteNode() {
    __asm {
        push [esp + 4]
        push edx
        call Tree_DeleteNodeImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56bb10, Tree_DeleteNode);

// 0x56bbb0. Flush callback in EBX, coder in EDI.
__declspec(naked) void StreamA_PutPending() {
    __asm {
        push eax
        push edi
        push ebx
        call StreamA_PutPendingImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x56bbb0, StreamA_PutPending);

// 0x56bc00. Coder in EAX, flush callback on the stack.
__declspec(naked) void Coder_Emit() {
    __asm {
        push [esp + 4]
        push eax
        call Coder_EmitImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56bc00, Coder_Emit);

// 0x56bca0. Coder in ESI.
__declspec(naked) void Tree_Init() {
    __asm {
        push esi
        call Tree_InitImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x56bca0, Tree_Init);

// 0x56be10. Unpacks an LZSS stream (header word 0x10, 0x91; 512-byte window
// starting at 0x1be; a flag byte per 8 tokens: literal, or 9-bit offset + 7-bit
// length + 3 copy) from read into write. 1 when done, -2 for an empty input, -3
// without callbacks, -4 for a bad header.
RT3_NOINLINE int32_t __fastcall Lzss_Unpack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xd9fdd0, 0x8000);
    hook::Global<int32_t>(0xd9fdc8) = n;
    if (n <= 0) return -2;
    if (hook::Global<uint16_t>(0xd9fddc) != 0x10 || hook::Global<uint16_t>(0xd9fdde) != 0x91) return -4;
    hook::Global<int32_t>(0xd9fdc8) = n - 0x10;
    hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fde0;
    hook::Global<int32_t>(0xda7dd4) = 0;
    hook::Global<uint8_t*>(0xda7dd8) = (uint8_t*)0xd97dc8;
    uint8_t win[0x200];
    memset(win, 0, sizeof win);
    uint32_t r = 0x1be, flags = 0;
    for (;;) {
        flags >>= 1;
        if (!(flags & 0x100)) {
            int32_t c = StreamA_GetImpl(read);
            if (c == -1) break;
            flags = (uint32_t)c | 0xff00;
        }
        if (flags & 1) {
            int32_t c = StreamA_GetImpl(read);
            if (c == -1) break;
            PutA((uint8_t)c, write);
            win[r] = (uint8_t)c;
            r = (r + 1) & 0x1ff;
            continue;
        }
        int32_t lo = StreamA_GetImpl(read);
        if (lo == -1) break;
        int32_t hi = StreamA_GetImpl(read);
        if (hi == -1) break;
        int32_t pos = lo | (hi & (int32_t)0xffffff80) << 1;
        int32_t len = (hi & 0x7f) + 2;
        for (int32_t k = 0; k <= len; ++k) {
            uint8_t c = win[(k + pos) & 0x1ff];
            PutA(c, write);
            win[r] = c;
            r = (r + 1) & 0x1ff;
        }
    }
    if (hook::Global<int32_t>(0xda7dd4) > 0) {
        write((uint8_t*)0xd97dc8, hook::Global<int32_t>(0xda7dd4));
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56be10, Lzss_Unpack);

// 0x56bfe0. Packs read into write as LZSS (header 0xbabe, word 0x10, 0x91; the
// format Lzss_Unpack reads), with the coder state and its trees on the stack and
// in the heap. 1 when done, -2 for an empty input, -3 without callbacks.
RT3_NOINLINE int32_t __fastcall Lzss_Pack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xd9fdd0, 0x8000);
    hook::Global<int32_t>(0xd9fdc8) = n;
    if (n <= 0) return -2;
    hook::Global<int32_t>(0xd97dcc) = 0;
    hook::Global<int32_t>(0xd97dd0) = 0;
    hook::Global<int32_t>(0xd97dd4) = 0x910010;
    uint8_t c[0x50];
    memset(c, 0, 0x4e);
    hook::Global<int32_t>(0xda7dd4) = 0x10;
    hook::Global<int32_t>(0xd97dc8) = 0xbabe;
    hook::Global<uint8_t*>(0xda7dd0) = (uint8_t*)0xd9fdd0;
    hook::Global<uint8_t*>(0xda7dd8) = (uint8_t*)0xd97dc8;
    Tree_InitImpl(c);
    uint8_t* text = F<uint8_t*>(c, 0x38);
    memset(text, 0, 0x200);
    int32_t r = 0x1be, len = 0;
    F<int32_t>(c, 0x14) = r;
    for (; len < 0x42; ++len) {
        int32_t ch = StreamA_GetImpl(read);
        if (ch == -1) break;
        text[0x1be + len] = (uint8_t)ch;
    }
    for (int32_t i = 1; i <= 0x42; ++i) Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14) - i);
    Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
    int32_t s0 = 0;
    F<int32_t>(c, 0x18) = 1;
    c[0x4d] = 1;
    c[0x3c] = 0;
    F<int32_t>(c, 0) = 0;
    do {
        if (F<int32_t>(c, 0x20) >= len) F<int32_t>(c, 0x20) = len;
        Coder_EmitImpl(c, write);
        int32_t last = F<int32_t>(c, 0x20), i = 0;
        for (; i < last; ++i) {
            int32_t ch = StreamA_GetImpl(read);
            if (ch == -1) break;
            Tree_DeleteNodeImpl(s0, c);
            text[s0] = (uint8_t)ch;
            if (s0 < 0x41) text[s0 + 0x200] = (uint8_t)ch;
            s0 = (s0 + 1) & 0x1ff;
            F<int32_t>(c, 0x14) = (F<int32_t>(c, 0x14) + 1) & 0x1ff;
            Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
        }
        for (; i < last; ++i) {
            Tree_DeleteNodeImpl(s0, c);
            s0 = (s0 + 1) & 0x1ff;
            F<int32_t>(c, 0x14) = (F<int32_t>(c, 0x14) + 1) & 0x1ff;
            if (--len) Tree_InsertNodeImpl(c, F<int32_t>(c, 0x14));
        }
    } while (len > 0);
    int32_t pending = F<int32_t>(c, 0x18);
    if (pending > 1) {
        F<int32_t>(c, 0) += pending;
        for (int32_t k = 0; k < pending; ++k) PutA(c[0x3c + k], write);
    }
    if (hook::Global<int32_t>(0xda7dd4) > 0) {
        write((uint8_t*)0xd97dc8, hook::Global<int32_t>(0xda7dd4));
        hook::Global<int32_t>(0xda7dd4) = 0;
    }
    static const uint32_t bufs[] = { 0x24, 0x28, 0x2c, 0x38 };
    for (uint32_t off : bufs)
        if (void* b = F<void*>(c, off)) ((void(__cdecl*)(void*))0x5a1145)(b);
    return 1;
}
RT3_REPLACE(0x56bfe0, Lzss_Pack);

// 0x56c320. Appends one byte to stream B (flush callback in ESI).
__declspec(naked) void StreamB_Put() {
    __asm {
        mov eax, [esp + 4]
        push esi
        push eax
        call StreamB_PutImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x56c320, StreamB_Put);

// 0x56c370. Count in EBX, refill callback in EDI, destination on the stack.
__declspec(naked) void StreamB_Read() {
    __asm {
        push [esp + 4]
        push edi
        push ebx
        call StreamB_ReadImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x56c370, StreamB_Read);

// 0x56c3e0. Packs read into write as RLE (header 0xbabe, word 0x10, 0x92; a byte
// 0x80 | n-1 before a run of n <= 0x80 copies, or n-1 before n literal bytes),
// working through a 256-byte look-ahead. 1 when done, -2 for an empty input, -3
// without callbacks.
RT3_NOINLINE int32_t __fastcall Rle_Pack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xdafde8, 0x8000);
    hook::Global<int32_t>(0xdb7de8) = n;
    if (n <= 0) return -2;
    hook::Global<int32_t>(0xda7de8) = 0;
    hook::Global<int32_t>(0xda7de4) = 0;
    hook::Global<int32_t>(0xdafde4) = 0x10;
    hook::Global<int32_t>(0xda7de0) = 0xbabe;
    hook::Global<int32_t>(0xda7dec) = 0x920010;
    hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafde8;
    hook::Global<uint8_t*>(0xdb7dec) = (uint8_t*)0xda7de0;
    uint8_t buf[0x100];
    int32_t len = StreamB_ReadImpl(0x100, read, buf);
    while (len > 0) {
        int32_t i = 2;
        if (len > 2) {
            while (i < 0x80 && buf[i] == buf[i - 1] && buf[i - 1] == buf[i - 2]) {
                ++i;
                if (i >= len) break;
            }
            if (i > 2) {
                StreamB_PutImpl((uint8_t)(i - 1) | 0x80u, write);
                StreamB_PutImpl(buf[0], write);
                memmove(buf, buf + i, 0x100 - i);
                len += StreamB_ReadImpl(i, read, buf + 0x100 - i) - i;
            }
        }
        if (len <= 0) break;
        int32_t j = 0;
        while (j < 0x80) {
            if (j + 2 < len && buf[j] == buf[j + 1] && buf[j + 1] == buf[j + 2]) break;
            ++j;
            if (j >= len) break;
        }
        if (j) {
            StreamB_PutImpl((uint8_t)(j - 1), write);
            for (int32_t k = 0; k < j; ++k) StreamB_PutImpl(buf[k], write);
            memmove(buf, buf + j, 0x100 - j);
            len += StreamB_ReadImpl(j, read, buf + 0x100 - j) - j;
        }
    }
    if (hook::Global<int32_t>(0xdafde4) > 0) {
        write((uint8_t*)0xda7de0, hook::Global<int32_t>(0xdafde4));
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56c3e0, Rle_Pack);

// 0x56c630. Unpacks Rle_Pack's format from read into write. 1 when done, -2 for an
// empty input, -3 without callbacks, -4 for a bad header.
RT3_NOINLINE int32_t __fastcall Rle_Unpack(StreamFn read, StreamFn write) {
    if (!read || !write) return -3;
    int32_t n = read((uint8_t*)0xdafde8, 0x8000);
    hook::Global<int32_t>(0xdb7de8) = n;
    if (n <= 0) return -2;
    if (hook::Global<uint16_t>(0xdafdf4) != 0x10 || hook::Global<uint16_t>(0xdafdf6) != 0x92) return -4;
    hook::Global<int32_t>(0xdb7de8) = n - 0x10;
    hook::Global<uint8_t*>(0xdafde0) = (uint8_t*)0xdafdf8;
    hook::Global<int32_t>(0xdafde4) = 0;
    hook::Global<uint8_t*>(0xdb7dec) = (uint8_t*)0xda7de0;
    uint8_t buf[0x100];
    int32_t len = StreamB_ReadImpl(0x100, read, buf);
    while (len > 0) {
        uint8_t ctrl = buf[0];
        memmove(buf, buf + 1, 0xff);
        len += StreamB_ReadImpl(1, read, buf + 0xff) - 1;
        int32_t count = (ctrl & 0x7f) + 1;
        if (ctrl & 0x80) {
            uint8_t v = buf[0];
            memmove(buf, buf + 1, 0xff);
            len += StreamB_ReadImpl(1, read, buf + 0xff) - 1;
            for (int32_t k = 0; k < count; ++k) StreamB_PutImpl(v, write);
        } else {
            int32_t m = count < len ? count : len;
            for (int32_t k = 0; k < m; ++k) StreamB_PutImpl(buf[k], write);
            memmove(buf, buf + m, 0x100 - m);
            len += StreamB_ReadImpl(m, read, buf + 0x100 - m) - m;
        }
    }
    if (hook::Global<int32_t>(0xdafde4) > 0) {
        write((uint8_t*)0xda7de0, hook::Global<int32_t>(0xdafde4));
        hook::Global<int32_t>(0xdafde4) = 0;
    }
    return 1;
}
RT3_REPLACE(0x56c630, Rle_Unpack);

// 0x56c840. Registers the module cleanup 0x56c830 with atexit.
int32_t __fastcall Module_RegisterCleanup() {
    ((int(__cdecl*)(void*))0x5a2d10)((void*)0x56c830);
    return 0;
}
RT3_REPLACE(0x56c840, Module_RegisterCleanup);

}  // namespace rt3
