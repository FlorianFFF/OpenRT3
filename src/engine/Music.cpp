// Music.cpp - RT3.exe 0x556000-0x558000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Fatal_Exit(const char* msg);

void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
char* __fastcall StrCopy(char* dst, const char* src);
void __fastcall Holder_Free(uint8_t* h);
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode);
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
void __fastcall Hash_Destroy(HashMap* m);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Pool_Free(NodePool* p);
void __fastcall Nop565e20();
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Emitter_Init(uint8_t* e);
void __fastcall Emitter_Reset(uint8_t* e, int, int32_t n, int32_t);

uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);

uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);
void __fastcall Emitter_RetintAll(uint8_t* e);
const char* __fastcall GetString(int32_t id);
void __fastcall Str_TrimRight(char* s);
void __fastcall Str_TrimLeft(char* s);
void __fastcall Holder_Alloc(uint8_t* h);
void __fastcall Pool_Release(int32_t* p, int, void** node);
void __fastcall ModelLibrary_Free(uint32_t* o);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Texture_SetD9399c(const char* name);
int32_t __fastcall EmitterRef_Update(uint8_t* o, const uint32_t* p);
void __fastcall Model_Release(uint8_t* m);
void __fastcall Ambience_Tick(uint8_t* a, int, uint32_t flag);
void __fastcall Ambience_Visit(uint8_t* a, int, uint32_t x, uint32_t y, uint32_t z);
RT3_NOINLINE void __fastcall ModelLibrary_Free(uint32_t* o);
RT3_NOINLINE void __fastcall ModelLibrary_Init(uint32_t* o, int, int32_t count);
RT3_NOINLINE uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj, uint32_t flags);
RT3_NOINLINE int32_t __fastcall EmitterLibrary_Register(uint8_t* lib, int, uint32_t key, int32_t perPoint, const float* rect, uint32_t flags);
uint8_t* __fastcall EmitterLibrary_Get(uint8_t* lib, int, uint32_t key, uint32_t color, const char* name, uint8_t* obj, int32_t spawn);
PtrList* __fastcall PtrList_Construct(PtrList* l, int, char usePool, int32_t blockSize);
PtrList* __fastcall PtrList_Init(PtrList* l);
void __fastcall PtrList_CreatePool(PtrList* l);
PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value);
void* __fastcall PtrList_PopFront(PtrList* l);
void __fastcall PtrList_RemoveNode(PtrList* l, int, PtrListNode* n);
RT3_NOINLINE void __fastcall PtrList_Rewind(PtrList* l);
RT3_NOINLINE void* __fastcall PtrList_Next(PtrList* l);
void* __fastcall PtrList_RemoveCurrent(PtrList* l);
void* __fastcall PtrList_TailValue(PtrList* l);
void* __fastcall PtrList_CurrentValue(PtrList* l);
PtrListBlock* __fastcall PtrList_AddBlock(PtrList* l, int, int32_t nodes);
void __fastcall PtrList_FreePool(PtrList* l);
RT3_NOINLINE void __fastcall PtrList_Clear(PtrList* l);
PtrListNode* __fastcall PtrList_AllocNode(PtrList* l);
void __fastcall PtrList_Prepend(PtrList* l, int, void* value);
RT3_NOINLINE void __fastcall PtrList_Destroy(PtrList* l);
char* __fastcall StrCopy(char* dst, const char* src);
RT3_NOINLINE int32_t __fastcall Str_IsBlank(const char* s);
RT3_NOINLINE void __fastcall Str_TrimLeft(char* s);
RT3_NOINLINE void __fastcall Str_TrimRight(char* s);
RT3_NOINLINE void __fastcall Set_6257e0(uint32_t a, uint32_t b);
void Str_InnerParens();
void Str_SplitKeyValue();
RT3_NOINLINE uint8_t* __fastcall Holder_Init(uint8_t* h, int, uint32_t a, uint32_t b);
void __fastcall Holder_Free(uint8_t* h);
RT3_NOINLINE void __fastcall Holder_Destruct(uint8_t* h);
void __fastcall Holder_Alloc(uint8_t* h);
RT3_NOINLINE int32_t __fastcall Config_Load(uint8_t* c, int, const char* path);
void __fastcall Set_d973b0(uint32_t v);
RT3_NOINLINE wchar_t* __fastcall Str_ToWide(wchar_t* dst, const char* src, int32_t n);
RT3_NOINLINE char* __fastcall Str_FromWide(char* dst, const wchar_t* src, int32_t n);
int32_t __stdcall Hook_Calld973b0(int32_t a, int32_t b, int32_t c);
RT3_NOINLINE void __fastcall Music_Shutdown(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_ReadParam(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_CreateC(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_Create10(uint8_t* m);
void __fastcall Music_Destroy(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_Start(uint8_t* m);
RT3_NOINLINE void __fastcall Music_Close(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_IsPlaying(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Music_IsPaused(uint8_t* m);
RT3_NOINLINE void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
void __stdcall Handles_Forget(uint32_t h);
RT3_NOINLINE void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
RT3_NOINLINE int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
void __fastcall Mem_Fill32(uint32_t* dst, uint32_t v, uint32_t count);
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec);

// The texture name from the main pack's cache (all mips, flag 0x7f).
static uint8_t* Texture_Acquire(const char* name) { return TextureCache_Get(g_pack, 0, name, 0, 0, 0x7f); }

// Replaces the texture held in global slot by name's (the old one released).
static void Texture_Replace(uint32_t slot, const char* name) {
    if (void* old = hook::Global<void*>(slot)) {
        Pack_Release(g_pack, 0, old);
        hook::Global<void*>(slot) = nullptr;
    }
    hook::Global<uint8_t*>(slot) = Texture_Acquire(name);
}


static inline bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

// 0x5574b0. Frees the two pools (+4, +0; each with a buffer +4) and the block +8.
static inline void FreePool(uint8_t* o, uint32_t off) {
    if (!F<uint8_t*>(o, off)) return;
    if (F<void*>(F<uint8_t*>(o, off), 4)) crt::free(F<void*>(F<uint8_t*>(o, off), 4));
    if (NodePool* p = F<NodePool*>(o, off)) {
        Pool_Free(p);
        crt::operator_delete(p);
        F<void*>(o, off) = nullptr;
    }
}


// Music stream (+0, Miles HSTREAM; volume +0x104).
static inline int32_t StreamStatus(void* s) { return imp::AIL_stream_status(s); }

static inline void StreamPause(void* s, int32_t on) { imp::AIL_pause_stream(s, on); }

RT3_NOINLINE static inline void StreamClose(void* s) { imp::AIL_close_stream(s); }

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

// Copies a float through the x87 (FLD / FSTP), which quiets signalling NaNs as the
// original's copies do.
static inline void X87Copy(void* dst, const void* src) {
    __asm {
        mov eax, src
        mov ecx, dst
        fld dword ptr [eax]
        fstp dword ptr [ecx]
    }
}


static inline void FatalError(const char* msg) { Fatal_Exit(msg); }

// 0x557af0 / 0x557b30. Creates the COM object 0x5c84e0 (interface 0x5c84d0) into
// +0xc / +0x10 and initialises it (method 0x34 with 0x5c84a0). Whether both worked.
static int32_t ComCreate(void** slot) {
    typedef HRESULT(WINAPI * CreateFn)(const void*, void*, DWORD, const void*, void**);
    if (((CreateFn)imp::CoCreateInstance)((const void*)0x5c84e0, nullptr, 1, (const void*)0x5c84d0, slot) < 0) return 0;
    void* o = *slot;
    return ((HRESULT(__stdcall*)(void*, const void*))(*(void***)o)[0x34 / 4])(o, (const void*)0x5c84a0) >= 0 ? 1 : 0;
}


// 0x557330 body. Reduces s to the trimmed text between the open and close
// characters (0x6257e0 / 0x6257e4, '(' and ')'); 0 when either is missing.
static int32_t __cdecl Str_InnerParensImpl(char* s) {
    char* open = crt::strchr(s, g_6257e0);
    char* close = crt::strchr(s, g_6257e4);
    if (!open || !close) return 0;
    ++open;
    *close = 0;
    Str_TrimLeft(open);
    Str_TrimRight(open);
    if (open != s) StrCopy(s, open);
    return 1;
}

// 0x557390 body. Splits "key = value" into out +4 (key) and +0x104 (value), both
// trimmed; 0 without '=' or with an empty key.
static int32_t __cdecl Str_SplitKeyValueImpl(const char* line, uint8_t* out) {
    const char* eq = crt::strchr(line, '=');
    if (!eq) return 0;
    char key[0x100], value[0x100];
    size_t n = (size_t)(eq - line);
    strncpy(key, line, n);
    key[n] = 0;
    Str_TrimLeft(key);
    Str_TrimRight(key);
    if (!strlen(key)) return 0;
    strcpy(value, eq + 1);
    Str_TrimLeft(value);
    Str_TrimRight(value);
    strcpy((char*)out + 4, key);
    strcpy((char*)out + 0x104, value);
    return 1;
}

// The node pop of a pool { list*, lo, hi, enabled, size } (heap when disabled).
static uint8_t* Config_Take(uint8_t* pool) {
    if (!F<int32_t>(pool, 0xc)) return (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    uint8_t* list = F<uint8_t*>(pool, 0);
    uint8_t* n = F<uint8_t*>(list, 0);
    if (n) {
        F<uint8_t*>(list, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(list, 8);
    }
    return n;
}

// 0x556370 / 0x554060 / 0x53a520 / 0x565d60. The textures of 0xd9399c,
// 0xd93990 and (for a name) 0xccba20, 0xd97ab0.
RT3_NOINLINE void __fastcall Texture_SetD9399c(const char* name) { Texture_Replace(0xd9399c, name); }
RT3_REPLACE(0x556370, Texture_SetD9399c);

// 0x5563b0 (vtable). Emitter wrapper o (+4: {state, emitter}) moves its
// emitter to p (unless frozen, flag 0x400; flag 0x2000 only retints) or,
// without p, ticks it (0xd93998 copies +0x54 to 0x6257d8); then freezes it.
int32_t __fastcall EmitterRef_Update(uint8_t* o, const uint32_t* p) {
    uint8_t* s = F<uint8_t*>(o, 4);
    uint8_t* e = F<uint8_t*>(s, 4);
    if (!e || !F<int32_t>(s, 0)) return 0;
    if (p) {
        F<uint32_t>(e, 0xac) &= ~0x400u;
        uint8_t* e2 = F<uint8_t*>(s, 4);
        const uint32_t flags = F<uint32_t>(e2, 0xac);
        if (!(flags & 0x400)) {
            if (!(flags & 0x2000)) {
                F<uint32_t>(e2, 0x40) = p[1];
                F<uint32_t>(e2, 0x3c) = p[0];
                F<uint32_t>(e2, 0x44) = p[2];
            }
            Emitter_RetintAll(e2);
        }
    } else {
        if (g_d93998) hook::As<uint32_t>(g_6257d8) = F<uint32_t>(e, 0x54);
        F<uint32_t>(e, 0xac) &= ~0x400u;
        Emitter_Tick(F<uint8_t*>(s, 4), 0, 0);
    }
    F<uint32_t>(F<uint8_t*>(s, 4), 0xac) |= 0x400;
    return 0;
}
RT3_REPLACE(0x5563b0, EmitterRef_Update);

// 0x556460. Releases a model: drops its shared record from the hash 0x6d4030 +4
// (flag 0x400 in +0xac; the record's list is cleared with its last user), frees
// +0x1dd, clears its own lists, releases the pack entry +8 and frees +0x64 (also
// clearing 0xd939a4).
void __fastcall Model_Release(uint8_t* m) {
    if (F<uint32_t>(m, 0xac) & 0x400) {
        uint8_t* g = g_models;
        if (int32_t* rec = (int32_t*)(uintptr_t)Hash_Get(F<HashMap*>(g, 4), 0, (uint32_t)(uintptr_t)m)) {
            if (--rec[0] == 0) ListPair_Clear((uint8_t*)(uintptr_t)rec[1], 0, 1);
            Hash_Remove(F<HashMap*>(g, 4), 0, (uint32_t)(uintptr_t)m);
        }
    }
    if (void* p = F<void*>(m, 0x1dd)) {
        crt::free(p);
        F<void*>(m, 0x1dd) = nullptr;
    }
    ListPair_Clear(m, 0, 1);
    if (F<uint8_t*>(m, 8)) Pack_Release(g_pack, 0, F<uint8_t*>(m, 8));
    if (void* p = F<void*>(m, 0x64)) {
        crt::free(p);
        F<void*>(m, 0x64) = nullptr;
        hook::As<int32_t>(g_d939a4) = 0;
    }
}
RT3_REPLACE(0x556460, Model_Release);

// 0x556500. Ambient sound tick: fades the day/night blend +0x30 toward the ends
// (direction +0x34, speed 0x6257dc) and updates all sounds (Hash_ForEach 0x5563b0),
// twice when the 10 s refresh (0x6257d8) is due.
void __fastcall Ambience_Tick(uint8_t* a, int, uint32_t flag) {
    g_d97ab8 = 0;
    F<int32_t>(a, 0x20) = 0;
    F<int32_t>(a, 0x24) = 0;
    hook::As<uint32_t>(g_d93998) = flag;
    int32_t due = F<int32_t>(g_engine, 0x2c) + 10000;
    if (!(F<float>(a, 0x30) > *(const float*)0x5c970c)) {
        F<uint32_t>(a, 0x34) = 0x3f800000;
        g_6257dc = 0x3951b717;
    } else if (!(F<float>(a, 0x30) < *(const float*)0x5c85b4) && F<float>(a, 0x30) == F<float>(a, 0x30)) {
        F<uint32_t>(a, 0x34) = 0xbf800000;
        g_6257dc = 0x39d1b717;
    }
    auto visit = (HashVisit)0x5563b0;
    g_d939a8 = 1;
    F<float>(a, 0x30) = (float)((double)hook::As<float>(g_6257dc) * F<float>(a, 0x34) + F<float>(a, 0x30));
    if (due < g_6257d8) {
        g_6257d8 = due - 10000;
        Hash_ForEach(F<HashMap*>(a, 8), 0, 0, visit, nullptr);
        g_d93998 = 1;
    }
    g_6257d8 = due;
    Hash_ForEach(F<HashMap*>(a, 8), 0, 0, visit, nullptr);
    F<int32_t>(a, 0x2c)++;
    g_d939a8 = 0;
}
RT3_REPLACE(0x556500, Ambience_Tick);

// 0x5565e0. Runs the sound visitor over all entries with context {a, b, c}.
void __fastcall Ambience_Visit(uint8_t* a, int, uint32_t x, uint32_t y, uint32_t z) {
    uint32_t ctx[3] = { x, y, z };
    Hash_ForEach(F<HashMap*>(a, 8), 0, 0, (HashVisit)0x5563b0, ctx);
}
RT3_REPLACE(0x5565e0, Ambience_Visit);

// 0x556620. Frees a model library: its buffer +0, the name hashes +8 and +4,
// the 200 shared models +0x10 (8 bytes apart), the pools +0x14 and +0xc and the
// emitters +0x18 (count +0x1c).
RT3_NOINLINE void __fastcall ModelLibrary_Free(uint32_t* o) {
    if (o[0]) {
        crt::operator_delete((void*)(uintptr_t)o[0]);
        o[0] = 0;
    }
    if (o[2]) {
        Hash_Destroy((HashMap*)(uintptr_t)o[2]);
        if (HashMap* h = (HashMap*)(uintptr_t)o[2]) {
            Hash_DestroyThunk(h);
            crt::operator_delete(h);
        }
        o[2] = 0;
    }
    if (uint8_t* arr = (uint8_t*)(uintptr_t)o[4]) {
        for (int k = 0; k < 200; ++k) {
            uint8_t*& m = F<uint8_t*>(arr, 4 + k * 8);
            if (m) {
                Model_Release(m);
                crt::operator_delete(m);
                m = nullptr;
            }
        }
        crt::free((void*)(uintptr_t)o[4]);
        o[4] = 0;
    }
    if (o[1]) {
        Hash_Destroy((HashMap*)(uintptr_t)o[1]);
        if (HashMap* h = (HashMap*)(uintptr_t)o[1]) {
            Hash_DestroyThunk(h);
            crt::operator_delete(h);
        }
        o[1] = 0;
    }
    if (NodePool* p = (NodePool*)(uintptr_t)o[5]) {
        Pool_Free(p);
        crt::operator_delete(p);
        o[5] = 0;
    }
    if (uint8_t* arr = (uint8_t*)(uintptr_t)o[6]) {
        for (int32_t k = 0; k < (int32_t)o[7]; ++k) {
            if (void* em = F<void*>(arr, 4 + k * 8)) {
                Nop565e20();
                crt::operator_delete(em);
            }
        }
        crt::free((void*)(uintptr_t)o[6]);
        o[6] = 0;
    }
    if (NodePool* p = (NodePool*)(uintptr_t)o[3]) {
        Pool_Free(p);
        crt::operator_delete(p);
        o[3] = 0;
    }
    o[7] = 0;
}
RT3_REPLACE(0x556620, ModelLibrary_Free);

  // -> 0x4d8ea0
RT3_REPLACE(0x556760, ModelLibrary_Free);

// 0x556770. Sets up a model library for `count` emitters (default 50,000): the
// emitter pool +0x14 over +0x18 (8-byte slots, each with a new emitter), the
// 200-slot model pool +0xc over +0x10, the name hashes +8 (17 buckets) and +4
// (101), the list +0; the library scale (0x6d4030 +0x28) goes back to 1.
RT3_NOINLINE void __fastcall ModelLibrary_Init(uint32_t* o, int, int32_t count) {
    o[0xb] = 100;
    if (count <= 0) count = 50000;
    if (o[5]) ModelLibrary_Free(o);
    o[6] = (uint32_t)(uintptr_t)crt::malloc((size_t)count * 8);
    auto newZeroed = [](size_t n) {
        void* p = crt::operator_new(n);
        if (p) memset(p, 0, n);
        return p;
    };
    NodePool* emitters = (NodePool*)newZeroed(0x1c);
    o[5] = (uint32_t)(uintptr_t)emitters;
    Pool_Init(emitters, 0, (void*)(uintptr_t)o[6], 8, count, 0);
    for (int32_t k = 0; k < count; ++k) {
        uint8_t* e = (uint8_t*)crt::operator_new(0x79);
        if (e) Emitter_Init(e);
        F<uint8_t*>((void*)(uintptr_t)o[6], 4 + k * 8) = e;
    }
    o[7] = (uint32_t)count;
    o[4] = (uint32_t)(uintptr_t)crt::malloc(0x640);
    memset((void*)(uintptr_t)o[4], 0, 0x640);
    NodePool* models = (NodePool*)newZeroed(0x1c);
    o[3] = (uint32_t)(uintptr_t)models;
    Pool_Init(models, 0, (void*)(uintptr_t)o[4], 8, 200, 0);
    HashMap* h1 = (HashMap*)newZeroed(0x14);
    o[2] = (uint32_t)(uintptr_t)h1;
    Hash_Init(h1, 0, 0x11, 0);
    HashMap* h2 = (HashMap*)newZeroed(0x14);
    o[1] = (uint32_t)(uintptr_t)h2;
    Hash_Init(h2, 0, 0x65, 0);
    o[0] = (uint32_t)(uintptr_t)newZeroed(0xc);
    F<uint32_t>(g_models, 0x28) = 0x3f800000;
}
RT3_REPLACE(0x556770, ModelLibrary_Init);

// 0x556920. Particle emitter constructor body: shape from collision model
// `name` (its points copied into +0x1dd as pointers + vertices, name CRC
// +0x1d4) or a single point, attached to scene object `obj` (+0xc, position
// copied), `perPoint` particles per point (+0x38), RGB `color` (+0x48 / +0x4c),
// optional screen rectangle `rect` (+0xe0.., in 1024ths +0x110..), `flags`
// (+0xac; 0x800000 adds a 0x3a00-byte trail buffer +0x64, 0x400 skips the
// reset). Starts the model library (0x6d4030, 50000) on first use.
RT3_NOINLINE uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags) {
    if (!F<int32_t>(g_models, 0x14)) ModelLibrary_Init(hook::As<uint32_t*>(g_models), 0, 0xc350);
    if (name) {
        uint8_t* m = Pack_AcquireCollision(g_pack, 0, name);
        F<uint8_t*>(e, 8) = m;
        uint8_t* shape = F<uint8_t*>(*(uint8_t**)m, 0xc);
        F<uint8_t*>(e, 0x10) = F<uint8_t*>(shape, 0);
        F<int32_t>(e, 0x30) = F<int32_t>(shape, 8);
        uint32_t** ptrs = (uint32_t**)crt::malloc((size_t)(F<int32_t>(shape, 8) << 4));
        F<uint32_t**>(e, 0x1dd) = ptrs;
        uint32_t* src = F<uint32_t*>(e, 0x10);
        uint32_t* dst = (uint32_t*)(ptrs + F<int32_t>(e, 0x30));
        for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i, dst += 3, src += 3) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            ptrs[i] = dst;
        }
        F<uint32_t>(e, 0x1d4) = Crc32String(name, 1);
    } else {
        F<uint8_t*>(e, 8) = nullptr;
        F<uint8_t*>(e, 0x10) = nullptr;
        F<int32_t>(e, 0x30) = 1;
        F<void*>(e, 0x1dd) = crt::malloc(0x10);
        F<uint32_t>(e, 0x1d4) = 0;
    }
    F<uint8_t*>(e, 0xc) = obj;
    if (obj) {
        X87Copy(e + 0x14, obj + 0x1e2);
        X87Copy(e + 0x18, obj + 0x1e6);
        X87Copy(e + 0x1c, obj + 0x1ea);
    } else {
        F<uint32_t>(e, 0x14) = 0;
        F<uint32_t>(e, 0x18) = 0;
        F<uint32_t>(e, 0x1c) = 0;
    }
    F<uint32_t>(e, 0x3c) = F<uint32_t>(e, 0x40) = F<uint32_t>(e, 0x44) = 0x3f400000;
    F<int32_t>(e, 0x38) = F<int32_t>(e, 0x30) * perPoint;
    for (uint32_t off : { 0xd4u, 0x94u, 0xc8u, 0x5cu, 0x34u, 0x50u, 0x54u, 0x58u }) F<uint32_t>(e, off) = 0;
    F<uint32_t>(e, 0xcc) = F<uint32_t>(e, 0xd0) = 0x3f800000;
    F<int32_t>(e, 0xbc) = -1;
    F<uint32_t>(e, 0x48) = F<uint32_t>(e, 0x4c) = color & 0xffffff;
    for (uint32_t off : { 0x70u, 0x74u, 0x78u, 0x7cu, 0x80u, 0x84u, 0x88u, 0x8cu, 0x90u, 0x9cu, 0xa0u, 0xa4u, 0xa8u, 0xb8u }) F<uint32_t>(e, off) = 0;
    F<int32_t>(e, 0xc0) = F<int32_t>(e, 0xc4) = -1;
    F<uint32_t>(e, 0xb4) = 0x3f800000;
    F<int32_t>(e, 0x2c) = 1;
    F<uint32_t>(e, 0x98) = 0x3f800000;
    F<uint32_t>(e, 0xac) = flags;
    for (uint32_t off : { 0xb0u, 0xd8u, 0x1e1u, 0x1edu, 0x1f1u, 0x1f5u, 0x68u, 0x64u, 0x6cu, 0x1f9u }) F<uint32_t>(e, off) = 0;
    X87Copy(e + 0xdc, g_models + 0x28);
    F<uint32_t>(e, 0x1e9) = 0x365d7591;
    if (flags & 0x800000) F<void*>(e, 0x64) = crt::malloc(0x3a00);
    memset(e + 0x10c, 0, 0xa0);
    if (rect) {
        F<float>(e, 0xe0) = rect[0];
        F<float>(e, 0xe4) = rect[1];
        F<float>(e, 0xe8) = rect[2];
        F<float>(e, 0xec) = rect[3];
        F<int32_t>(e, 0x118) = 1;
        F<int32_t>(e, 0x124) = 1;
        F<int32_t>(e, 0x11c) = Ftol((double)F<float>(e, 0xe0) * Fl(0x5ca248));
        F<int32_t>(e, 0x120) = Ftol((double)F<float>(e, 0xe4) * Fl(0x5ca248));
        F<int32_t>(e, 0x110) = Ftol(((double)F<float>(e, 0xe8) - F<float>(e, 0xe0)) * Fl(0x5ca248));
        F<int32_t>(e, 0x114) = Ftol(((double)F<float>(e, 0xec) - F<float>(e, 0xe4)) * Fl(0x5ca248));
    } else {
        F<uint32_t>(e, 0xec) = F<uint32_t>(e, 0xe8) = F<uint32_t>(e, 0xe4) = F<uint32_t>(e, 0xe0) = 0;
    }
    F<uint32_t>(e, 0) = 0;
    F<uint32_t>(e, 4) = 0;
    e[0x1d8] = 0;
    F<uint32_t>(e, 0x1e5) = 0;
    if (!(F<uint32_t>(e, 0xac) & 0x400)) Emitter_Reset(e, 0, F<int32_t>(e, 0x38), (int32_t)flags);
    return e;
}
RT3_REPLACE(0x556920, Emitter_Construct);

// 0x556c50. Registers an emitter template under `key` in the library: a node
// from the model pool (+0xc) holding a use count (0) and a shape-less emitter
// (`perPoint`, `rect`, `flags` | 0x400 so it is not reset yet), filed in the
// template hash +8.
RT3_NOINLINE int32_t __fastcall EmitterLibrary_Register(uint8_t* lib, int, uint32_t key, int32_t perPoint, const float* rect, uint32_t flags) {
    NodePool* pool = F<NodePool*>(lib, 0xc);
    uint32_t* node;
    if (pool->capacity) {
        SList* fl = pool->freeList;
        node = (uint32_t*)fl->head;
        if (node) {
            void** next = (void**)node[0];
            bool last = (void**)node == fl->tail;
            fl->head = next;
            if (last) fl->tail = next;
            --fl->count;
        }
    } else {
        node = (uint32_t*)crt::operator_new((size_t)pool->elemSize);
    }
    node[0] = 0;
    uint8_t* e = (uint8_t*)crt::operator_new(0x1fd);
    e = e ? Emitter_Construct(e, 0, nullptr, 0, perPoint, rect, nullptr, flags | 0x400) : nullptr;
    node[1] = (uint32_t)(uintptr_t)e;
    Hash_Insert(F<HashMap*>(lib, 8), 0, key, (int32_t)(uintptr_t)node);
    return 1;
}
RT3_REPLACE(0x556c50, EmitterLibrary_Register);

// 0x556ce0. The emitter template `key`, or with `spawn` 1 a new emitter from
// it: shape `name`, `color`, the template's particle count, attached to `obj`
// (the template is reset on its first use); new emitters are filed in the
// instance hash +4 against their template node. 0 when the key is unknown.
uint8_t* __fastcall EmitterLibrary_Get(uint8_t* lib, int, uint32_t key, uint32_t color, const char* name, uint8_t* obj, int32_t spawn) {
    uint32_t* node = (uint32_t*)(uintptr_t)Hash_Get(F<HashMap*>(lib, 8), 0, key);
    if (!node) return nullptr;
    if (spawn != 1) return (uint8_t*)(uintptr_t)node[1];
    if (++node[0] == 1) Emitter_Reset((uint8_t*)(uintptr_t)node[1], 0, F<int32_t>((void*)(uintptr_t)node[1], 0x38), 0);
    uint8_t* e = (uint8_t*)crt::operator_new(0x1fd);
    if (!e) {
        Hash_Insert(F<HashMap*>(lib, 4), 0, 0, (int32_t)(uintptr_t)node);
        return nullptr;
    }
    e = Emitter_Construct(e, 0, name, color, F<int32_t>((void*)(uintptr_t)node[1], 0x38), nullptr, obj, 0x400);
    Hash_Insert(F<HashMap*>(lib, 4), 0, (uint32_t)(uintptr_t)e, (int32_t)(uintptr_t)node);
    return e;
}
RT3_REPLACE(0x556ce0, EmitterLibrary_Get);

PtrList* __fastcall PtrList_Construct(PtrList* l, int, char usePool, int32_t blockSize) {
    l->blockSize = blockSize;
    l->cursor = nullptr;
    l->head = nullptr;
    l->tail = nullptr;
    l->count = 0;
    l->pool = nullptr;
    if (usePool) PtrList_CreatePool(l);
    return l;
}
RT3_REPLACE(0x556d80, PtrList_Construct);

PtrList* __fastcall PtrList_Init(PtrList* l) { memset(l, 0, sizeof *l); return l; }

 // returns this (callers use it)
RT3_REPLACE(0x556db0, PtrList_Init);

void __fastcall PtrList_CreatePool(PtrList* l) {
    PtrList* p = (PtrList*)crt::malloc(sizeof(PtrList));
    if (!p) {
        l->pool = nullptr;
        return;
    }
    memset(p, 0, sizeof *p);
    p->blockSize = 100;
    l->pool = p;
}
RT3_REPLACE(0x556dd0, PtrList_CreatePool);

PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value) {
    PtrListNode* n = PtrList_AllocNode(l);
    n->value = value;
    n->prev = l->tail;
    n->next = nullptr;
    if (l->tail) l->tail->next = n;
    l->tail = n;
    if (!l->head) l->head = n;
    l->count++;
    return n;
}
RT3_REPLACE(0x556e10, PtrList_Append);

// QUIRK: frees the node even when it came from a pool, and leaves the new
// head's prev pointing at the freed node.
void* __fastcall PtrList_PopFront(PtrList* l) {
    PtrListNode* n = l->head;
    if (!n) return nullptr;
    l->count--;
    void* v = n->value;
    PtrListNode* next = n->next;
    crt::free(n);
    l->head = next;
    return v;
}
RT3_REPLACE(0x556e50, PtrList_PopFront);

void __fastcall PtrList_RemoveNode(PtrList* l, int, PtrListNode* n) {
    if (l->cursor == n) l->cursor = n->next;
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n == l->tail) l->tail = l->tail->prev;
    if (n == l->head) {
        l->head = n->next;
        if (l->head) l->head->prev = nullptr;
    }
    if (!l->pool) crt::free(n);
    l->count--;
}
RT3_REPLACE(0x556e80, PtrList_RemoveNode);

RT3_NOINLINE void __fastcall PtrList_Rewind(PtrList* l) { l->cursor = l->head; }
RT3_REPLACE(0x556ef0, PtrList_Rewind);

RT3_NOINLINE void* __fastcall PtrList_Next(PtrList* l) {
    PtrListNode* n = l->cursor;
    if (!n) return nullptr;
    l->cursor = n->next;
    return n->value;
}
RT3_REPLACE(0x556f00, PtrList_Next);

void* __fastcall PtrList_RemoveCurrent(PtrList* l) {
    PtrListNode* n = l->cursor;
    if (!n) return nullptr;
    void* v = n->value;
    l->cursor = n->next;
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n == l->tail) l->tail = l->tail->prev;
    if (n == l->head) {
        l->head = n->next;
        if (l->head) l->head->prev = nullptr;
    }
    if (!l->pool) crt::free(n);
    l->count--;
    return v;
}
RT3_REPLACE(0x556f20, PtrList_RemoveCurrent);

void* __fastcall PtrList_TailValue(PtrList* l) { return l->tail ? l->tail->value : nullptr; }
RT3_REPLACE(0x556f90, PtrList_TailValue);

void* __fastcall PtrList_CurrentValue(PtrList* l) { return l->cursor ? l->cursor->value : nullptr; }
RT3_REPLACE(0x556fa0, PtrList_CurrentValue);

PtrListBlock* __fastcall PtrList_AddBlock(PtrList* l, int, int32_t nodes) {
    PtrListBlock* b = (PtrListBlock*)crt::malloc(sizeof(PtrListBlock));
    b->nodes = (PtrListNode*)crt::malloc(nodes * sizeof(PtrListNode));
    b->remaining = nodes;
    PtrList_Append(l->pool, 0, b);
    return b;
}
RT3_REPLACE(0x556fb0, PtrList_AddBlock);

void __fastcall PtrList_FreePool(PtrList* l) {
    PtrList_Rewind(l->pool);
    while (PtrListBlock* b = (PtrListBlock*)PtrList_RemoveCurrent(l->pool)) {
        crt::free(b->nodes);
        b->nodes = nullptr;
        crt::free(b);
    }
    l->count = 0;
}
RT3_REPLACE(0x557010, PtrList_FreePool);

RT3_NOINLINE void __fastcall PtrList_Clear(PtrList* l) {
    if (!l->pool) {
        for (PtrListNode* n = l->head; n;) {
            PtrListNode* next = n->next;
            crt::free(n);
            l->count--;
            n = next;
        }
    } else {
        PtrList_FreePool(l); // pooled nodes go with their blocks
    }
    l->tail = nullptr;
    l->cursor = nullptr;
    l->head = nullptr;
}
RT3_REPLACE(0x5570b0, PtrList_Clear);

PtrListNode* __fastcall PtrList_AllocNode(PtrList* l) {
    if (!l->pool) return (PtrListNode*)crt::malloc(sizeof(PtrListNode));
    PtrListBlock* b = l->pool->tail ? (PtrListBlock*)l->pool->tail->value : nullptr;
    if (!b || b->remaining == 0) b = PtrList_AddBlock(l, 0, l->blockSize);
    b->remaining--;
    return b->nodes + b->remaining;
}
RT3_REPLACE(0x557100, PtrList_AllocNode);

void __fastcall PtrList_Prepend(PtrList* l, int, void* value) {
    PtrListNode* n = PtrList_AllocNode(l);
    n->value = value;
    n->prev = nullptr;
    n->next = l->head;
    if (l->head) l->head->prev = n;
    l->head = n;
    l->count++; // QUIRK: tail is not set when the list was empty
}
RT3_REPLACE(0x557150, PtrList_Prepend);

RT3_NOINLINE void __fastcall PtrList_Destroy(PtrList* l) {
    PtrList_Clear(l);
    if (PtrList* p = l->pool) {
        PtrList_Destroy(p);
        crt::free(p);
        l->pool = nullptr;
    }
    if (l->count > 0) FatalError(GetString(0x41));      // list destroyed while not empty
    else if (l->count < 0) FatalError(GetString(0x42)); // count went negative
}
RT3_REPLACE(0x5571d0, PtrList_Destroy);

RT3_NOINLINE char* __fastcall StrCopy(char* dst, const char* src) {
    char* p = dst;
    *p = *src;
    while (*src) *++p = *++src;
    return dst;
}
RT3_REPLACE(0x557230, StrCopy);

// 0x557250. Whether the string is empty or whitespace only.
RT3_NOINLINE int32_t __fastcall Str_IsBlank(const char* s) {
    if (*s)
        while (IsSpace(*s)) {
            if (!*++s) break;
        }
    return *s ? 0 : 1;
}
RT3_REPLACE(0x557250, Str_IsBlank);

// 0x557280. Trims leading whitespace in place.
RT3_NOINLINE void __fastcall Str_TrimLeft(char* s) {
    int32_t n = 0;
    while (s[n]) ++n;
    if (n <= 0) return;
    char* p = s;
    if (!*p) return;
    while (IsSpace(*p)) {
        if (!*++p) break;
    }
    if (p == s) return;
    if (p < s + n) StrCopy(s, p);
    else *s = 0;
}
RT3_REPLACE(0x557280, Str_TrimLeft);

// 0x5572e0. Trims trailing whitespace in place.
RT3_NOINLINE void __fastcall Str_TrimRight(char* s) {
    int32_t n = 0;
    while (s[n]) ++n;
    if (n <= 0) return;
    char* p = s + n - 1;
    if (*p)
        while (IsSpace(*p)) {
            char prev = p[-1];
            --p;
            if (!prev) break;
        }
    p[1] = 0;
}
RT3_REPLACE(0x5572e0, Str_TrimRight);

RT3_NOINLINE void __fastcall Set_6257e0(uint32_t a, uint32_t b) {
    hook::As<uint32_t>(g_6257e0) = a;
    hook::As<uint32_t>(g_6257e4) = b;
}
RT3_REPLACE(0x557320, Set_6257e0);

// 0x557330. String in EDI.
__declspec(naked) void Str_InnerParens() {
    __asm {
        push edi
        call Str_InnerParensImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x557330, Str_InnerParens);

// 0x557390. Line in EDI, output on the stack.
__declspec(naked) void Str_SplitKeyValue() {
    __asm {
        push [esp + 4]
        push edi
        call Str_SplitKeyValueImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x557390, Str_SplitKeyValue);

RT3_NOINLINE uint8_t* __fastcall Holder_Init(uint8_t* h, int, uint32_t a, uint32_t b) {
    F<uint32_t>(h, 0x10) = b;
    F<uint32_t>(h, 0xc) = a;
    F<int32_t>(h, 8) = 0;
    return h;
}
RT3_REPLACE(0x557490, Holder_Init);

RT3_NOINLINE void __fastcall Holder_Free(uint8_t* h) {
    FreePool(h, 4);
    FreePool(h, 0);
    if (void* p = F<void*>(h, 8)) {
        crt::operator_delete(p);
        F<void*>(h, 8) = nullptr;
    }
}
RT3_REPLACE(0x5574b0, Holder_Free);

RT3_NOINLINE void __fastcall Holder_Destruct(uint8_t* h) { Holder_Free(h); }
RT3_REPLACE(0x557540, Holder_Destruct);

// 0x557550. (Re)allocates the holder's pools: +4 of +0xc records (0x114 bytes),
// +0 of +0x10 records (0x204 bytes), and the 12-byte list +8.
RT3_NOINLINE void __fastcall Holder_Alloc(uint8_t* h) {
    if (F<void*>(h, 8)) Holder_Free(h);
    h[0x14] = 0;
    F<int32_t>(h, 0x114) = 0;
    void* a = crt::malloc(F<int32_t>(h, 0xc) * 0x114);
    void* b = crt::malloc(F<int32_t>(h, 0x10) * 0x204);
    uint32_t* p = (uint32_t*)crt::operator_new(0x1c);
    if (p) for (int k = 0; k < 7; ++k) p[k] = 0;
    F<uint32_t*>(h, 4) = p;
    Pool_Init((NodePool*)p, 0, a, 0x114, F<int32_t>(h, 0xc), 0);
    p = (uint32_t*)crt::operator_new(0x1c);
    if (p) for (int k = 0; k < 7; ++k) p[k] = 0;
    F<uint32_t*>(h, 0) = p;
    Pool_Init((NodePool*)p, 0, b, 0x204, F<int32_t>(h, 0x10), 0);
    p = (uint32_t*)crt::operator_new(0xc);
    if (p) p[0] = p[1] = p[2] = 0;
    F<uint32_t*>(h, 8) = p;
}
RT3_REPLACE(0x557550, Holder_Alloc);

// 0x557630. Loads the config file path into c: "[section]" lines (records of
// 0x114 bytes from the pool +4, kept in file order in the list +8; entries list
// +0x108 / +0x10c / count +0x110, total +0x104) and "key = value" lines (0x204-
// byte records from the pool +0, appended to the current section); ';' starts a
// comment and blank lines are skipped. +0x114 counts the lines read. On an error
// (no file 0x27, entry outside a section 0x25, bad entry 0x26) its text goes to
// +0x14 and 0 comes back, else 1.
RT3_NOINLINE int32_t __fastcall Config_Load(uint8_t* c, int, const char* path) {
    Holder_Alloc(c);
    void* f = crt::fopen(path, (const char*)0x5ce2c0);
    if (!f) {
        strcpy((char*)c + 0x14, GetString(0x27));
        return 0;
    }
    uint8_t* section = nullptr;
    uint8_t* entry = nullptr;
    char line[0x100];
    while (!(F<uint8_t>(f, 0xc) & 0x10)) {
        if (!crt::fgets(line, 0x100, f)) continue;
        if (char* semi = crt::strchr(line, ';')) *semi = 0;
        const char* p = line;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
        if (*p) {
            if (Str_InnerParensImpl(line)) {
                uint8_t* prev = section;
                section = Config_Take(F<uint8_t*>(c, 4));
                memset(section, 0, 0x114);
                F<int32_t>(section, 0x108) = 0;
                F<int32_t>(section, 0x10c) = 0;
                F<int32_t>(section, 0x110) = 0;
                F<int32_t>(section, 0x104) = 0;
                strcpy((char*)section + 4, line);
                uint8_t* list = F<uint8_t*>(c, 8);
                if (prev) {
                    F<uint8_t*>(section, 0) = F<uint8_t*>(prev, 0);
                    F<uint8_t*>(prev, 0) = section;
                    ++F<int32_t>(list, 8);
                    if (prev == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = section;
                } else {
                    F<uint8_t*>(section, 0) = F<uint8_t*>(list, 0);
                    ++F<int32_t>(list, 8);
                    F<uint8_t*>(list, 0) = section;
                    if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = section;
                }
                entry = nullptr;
            } else {
                if (!section) {
                    strcpy((char*)c + 0x14, GetString(0x25));
                    break;
                }
                uint8_t* prev = entry;
                entry = Config_Take(F<uint8_t*>(c, 0));
                memset(entry, 0, 0x204);
                if (!Str_SplitKeyValueImpl(line, entry)) {
                    Pool_Release(F<int32_t*>(c, 0), 0, (void**)entry);
                    strcpy((char*)c + 0x14, GetString(0x26));
                    break;
                }
                if (prev) {
                    F<uint8_t*>(entry, 0) = F<uint8_t*>(prev, 0);
                    F<uint8_t*>(prev, 0) = entry;
                    ++F<int32_t>(section, 0x110);
                    if (prev == F<uint8_t*>(section, 0x10c)) F<uint8_t*>(section, 0x10c) = entry;
                } else {
                    F<uint8_t*>(entry, 0) = F<uint8_t*>(section, 0x108);
                    ++F<int32_t>(section, 0x110);
                    F<uint8_t*>(section, 0x108) = entry;
                    if (!F<uint8_t*>(section, 0x10c)) F<uint8_t*>(section, 0x10c) = entry;
                }
                ++F<int32_t>(section, 0x104);
            }
        }
        ++F<int32_t>(c, 0x114);
    }
    crt::fclose(f);
    return c[0x14] ? 0 : 1;
}
RT3_REPLACE(0x557630, Config_Load);

void __fastcall Set_d973b0(uint32_t v) { g_d973b0 = v; }
RT3_REPLACE(0x557920, Set_d973b0);

// 0x557930 / 0x557970. ANSI <-> UTF-16 conversion into a buffer of n characters
// (n = -1: the source length + 1), always terminated.
RT3_NOINLINE wchar_t* __fastcall Str_ToWide(wchar_t* dst, const char* src, int32_t n) {
    if (n == -1) {
        int32_t k = 0;
        while (src[k]) ++k;
        n = k + 1;
    }
    imp::MultiByteToWideChar(0, 0, src, -1, dst,
                                                                                                    n - 1);
    dst[n - 1] = 0;
    return dst;
}
RT3_REPLACE(0x557930, Str_ToWide);

RT3_NOINLINE char* __fastcall Str_FromWide(char* dst, const wchar_t* src, int32_t n) {
    if (n == -1) n = (int32_t)crt::wcslen(src) + 1;
    imp::WideCharToMultiByte(0, 0, src, -1, dst, n - 1, nullptr, nullptr);
    dst[n - 1] = 0;
    return dst;
}
RT3_REPLACE(0x557970, Str_FromWide);

// 0x5579b0. Calls the hook 0xd973b0 (fastcall, one stack argument) if set.
int32_t __stdcall Hook_Calld973b0(int32_t a, int32_t b, int32_t c) {
    auto fn = hook::Global<int32_t(__fastcall*)(int32_t, int32_t, int32_t)>(0xd973b0);
    return fn ? fn(a, b, c) : 0;
}
RT3_REPLACE(0x5579b0, Hook_Calld973b0);

// 0x5579d0. Shuts the music objects down: stops the performance (+8, once, after
// the COM call 0x5c8494(0, 0)), then releases +0x10 and +0xc.
RT3_NOINLINE void __fastcall Music_Shutdown(uint8_t* m) {
    if (F<void*>(m, 8) && !F<int32_t>(m, 4)) {
        (((void(WINAPI*)(void*, void*))imp::CoInitializeEx))(nullptr, nullptr);
        void* o = F<void*>(m, 8);
        F<int32_t>(m, 4) = 1;
        ((void(__stdcall*)(void*, int32_t))(*(void***)o)[0x60 / 4])(o, 0);
    }
    if (void* o = F<void*>(m, 0x10)) {
        ((void(__stdcall*)(void*))(*(void***)o)[8 / 4])(o);
        F<void*>(m, 0x10) = nullptr;
    }
    if (void* o = F<void*>(m, 0xc)) {
        ((void(__stdcall*)(void*))(*(void***)o)[8 / 4])(o);
        F<void*>(m, 0xc) = nullptr;
    }
}
RT3_REPLACE(0x5579d0, Music_Shutdown);

// 0x557a30. Reads the performance's parameter 0x5c84a0 (method 0x10) into a buffer
// (+0x14) sized by a first query; 1 when it was read, else 0 with the error code
// 0x6d411c = 99999 when nothing came back.
RT3_NOINLINE int32_t __fastcall Music_ReadParam(uint8_t* m) {
    typedef HRESULT(__stdcall * GetFn)(void*, const void*, int32_t, void*, uint32_t*, uint32_t*, int32_t);
    uint32_t size = 0, got = 0;
    int32_t ok = 0;
    void* o = F<void*>(m, 8);
    HRESULT hr = ((GetFn)(*(void***)o)[0x10 / 4])(o, (const void*)0x5c84a0, 0, nullptr, &size, &got, 0);
    if (hr == (HRESULT)0x80158100) {
        void* buf = crt::operator_new(size);
        o = F<void*>(m, 8);
        F<void*>(m, 0x14) = buf;
        if (((GetFn)(*(void***)o)[0x10 / 4])(o, (const void*)0x5c84a0, 0, buf, &size, &got, 0) >= 0) {
            if (got == 1) return 1;
            o = F<void*>(m, 8);
            if (((GetFn)(*(void***)o)[0x10 / 4])(o, (const void*)0x5c84a0, 0, F<void*>(m, 0x14), &size, &got, 0) >= 0) ok = 1;
        }
    }
    if (got < 1) {
        g_6d411c = 99999;
        return 0;
    }
    return ok;
}
RT3_REPLACE(0x557a30, Music_ReadParam);

RT3_NOINLINE int32_t __fastcall Music_CreateC(uint8_t* m) { return ComCreate((void**)(m + 0xc)); }
RT3_REPLACE(0x557af0, Music_CreateC);

RT3_NOINLINE int32_t __fastcall Music_Create10(uint8_t* m) { return ComCreate((void**)(m + 0x10)); }
RT3_REPLACE(0x557b30, Music_Create10);

// 0x557b70. Destroys the music player: shuts it down, releases the performance
// (+8), frees the parameter buffer (+0x14) and uninitialises COM when it did the
// initialisation (+0 == 1).
void __fastcall Music_Destroy(uint8_t* m) {
    Music_Shutdown(m);
    if (void* o = F<void*>(m, 8)) {
        ((void(__stdcall*)(void*))(*(void***)o)[8 / 4])(o);
        F<void*>(m, 8) = nullptr;
    }
    if (void* b = F<void*>(m, 0x14)) {
        crt::operator_delete(b);
        F<void*>(m, 0x14) = nullptr;
    }
    if (F<int32_t>(m, 0) == 1) {
        (((void(WINAPI*)())imp::CoUninitialize))();
        F<int32_t>(m, 0) = 0;
    }
}
RT3_REPLACE(0x557b70, Music_Destroy);

// 0x557bc0. Starts the music player: COM and the performance object (0x5c84c0,
// interface 0x5c84b0) on first use, its init (method 0xc with notify 0x5579b0),
// the parameter buffer and both helper objects. 1 on success; on failure the error
// goes to 0x6d411c (99998 when a helper failed).
RT3_NOINLINE int32_t __fastcall Music_Start(uint8_t* m) {
    typedef HRESULT(WINAPI * CreateFn)(const void*, void*, DWORD, const void*, void**);
    Music_Shutdown(m);
    if (!F<int32_t>(m, 0)) {
        (((void(WINAPI*)(void*, void*))imp::CoInitializeEx))(nullptr, nullptr);
        HRESULT hr = ((CreateFn)imp::CoCreateInstance)((const void*)0x5c84c0, nullptr, 1, (const void*)0x5c84b0, (void**)(m + 8));
        if (hr < 0) {
            g_6d411c = hr;
            return 0;
        }
        F<int32_t>(m, 0) = 1;
    }
    void* perf = F<void*>(m, 8);
    HRESULT hr = ((HRESULT(__stdcall*)(void*, void*, uint32_t, void*))(*(void***)perf)[0xc / 4])(perf, nullptr, 0x5579b0, nullptr);
    if (hr < 0) {
        g_6d411c = hr;
        return 0;
    }
    F<int32_t>(m, 4) = 0;
    if (!F<void*>(m, 0x14) && !Music_ReadParam(m)) return 0;
    int32_t r = Music_CreateC(m);
    if (r == 1) {
        r = Music_Create10(m);
        if (r) return r;
    }
    g_6d411c = 99998;
    return r;
}
RT3_REPLACE(0x557bc0, Music_Start);

RT3_NOINLINE void __fastcall Music_Close(uint8_t* m) {
    if (!F<void*>(m, 0)) return;
    StreamPause(F<void*>(m, 0), 1);
    StreamClose(F<void*>(m, 0));
}
RT3_REPLACE(0x557c70, Music_Close);

RT3_NOINLINE int32_t __fastcall Music_IsPlaying(uint8_t* m) { return F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4 ? 1 : 0; }
RT3_REPLACE(0x557c90, Music_IsPlaying);

RT3_NOINLINE int32_t __fastcall Music_IsPaused(uint8_t* m) { return F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8 ? 1 : 0; }
RT3_REPLACE(0x557cb0, Music_IsPaused);

// 0x557cd0. Stops and closes the stream, forgets it in the active table
// (0xd973b8, 10) and reports the end (0xd973e0) when asked.
RT3_NOINLINE void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify) {
    void* s = F<void*>(m, 0);
    if (!s) return;
    imp::AIL_register_stream_callback(s, nullptr);
    StreamPause(F<void*>(m, 0), 1);
    StreamClose(F<void*>(m, 0));
    for (int i = 0; i < 10; ++i)
        if (((void**)0xd973b8)[i] == F<void*>(m, 0)) {
            ((void**)0xd973b8)[i] = nullptr;
            break;
        }
    F<void*>(m, 0) = nullptr;
    auto cb = hook::Global<void(__fastcall*)(uint32_t)>(0xd973e0);
    if (cb && notify) cb(notify);
}
RT3_REPLACE(0x557cd0, Music_Stop);

// 0x557d40. Forgets handle h in the 10-slot table 0xd973b8 and calls the hook
// 0xd973e0 (h in ECX) inside the nesting count 0xccb9f0.
void __stdcall Handles_Forget(uint32_t h) {
    ++g_ccb9f0;
    for (int32_t i = 0; i < 10; ++i)
        if (((uint32_t*)0xd973b8)[i] == h) {
            ((uint32_t*)0xd973b8)[i] = 0;
            break;
        }
    if (auto fn = hook::Global<void(__fastcall*)(uint32_t)>(0xd973e0)) fn(h);
    --g_ccb9f0;
}
RT3_REPLACE(0x557d40, Handles_Forget);

// 0x557d90. Sets the music volume: 0 pauses a playing stream, non-zero resumes
// a paused one; levels scaled by 0x5ddcc8.
RT3_NOINLINE void __fastcall Music_SetVolume(uint8_t* m, int, float vol) {
    void* s = F<void*>(m, 0);
    if (!s) {
        F<float>(m, 0x104) = vol;
        return;
    }
    if (vol == *(const float*)0x5c8598) {
        if (StreamStatus(s) == 4) {
            if ((F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4) ||
                (F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8))
                StreamPause(F<void*>(m, 0), 1);
        }
    } else if (StreamStatus(s) == 8) {
        if ((F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4) ||
            (F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8))
            StreamPause(F<void*>(m, 0), 0);
    }
    float v = (float)((double)vol * *(const float*)0x5ddcc8);
    imp::AIL_set_stream_volume_levels(F<void*>(m, 0), v, v);
    F<float>(m, 0x104) = vol;
}
RT3_REPLACE(0x557d90, Music_SetVolume);

// 0x557e50. Opens and starts a music stream (file +4, copied from `file` when
// given) at the stored volume (+0x104 > 0 only); `onEnd` becomes the end callback.
RT3_NOINLINE int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd) {
    if (!Sound_Is3D(g_sound)) return 0;
    if (!(*(const float*)0x5c8598 < F<float>(m, 0x104))) return 0;
    char* name = (char*)m + 4;
    if (!file) {
        if (!*name) return 0;
    } else {
        for (size_t k = 0; (name[k] = file[k]) != 0; ++k) {}
    }
    Music_Stop(m, 0, 0);
    void* s = imp::AIL_open_stream(digital, name, 0x80000);
    F<void*>(m, 0) = s;
    if (!s) return 0;
    Music_SetVolume(m, 0, F<float>(m, 0x104));
    for (int i = 0; i < 10; ++i)
        if (!((void**)0xd973b8)[i]) {
            ((void**)0xd973b8)[i] = F<void*>(m, 0);
            break;
        }
    imp::AIL_start_stream(F<void*>(m, 0));
    auto reg = ((void(__stdcall*)(void*, uint32_t))imp::AIL_register_stream_callback);
    if (onEnd) {
        g_d973e0 = onEnd;
        reg(F<void*>(m, 0), 0x557d40);
    } else {
        reg(F<void*>(m, 0), 0);
    }
    return 1;
}
RT3_REPLACE(0x557e50, Music_Play);

void __fastcall Mem_Fill32(uint32_t* dst, uint32_t v, uint32_t count) {
    for (uint32_t k = 0; k < count; ++k) dst[k] = v;
}
RT3_REPLACE(0x557f50, Mem_Fill32);

// 0x557f80. Applies the common head of a widget record read from the pack (sic:
// the name pointers come along raw); the font (default 3) is acquired only when the
// widget has none yet.
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec) {
    memcpy(w + 4, rec, 0x5f);
    if (!w[0x37]) w[0x37] = 3;
    w[0x67] = w[0x37];
    if (!F<void*>(w, 0x63)) F<uint8_t*>(w, 0x63) = Pack_AcquireFont(g_pack, 0, w[0x37]);
    memcpy(w + 0x6c, w + 0xb, 0x10);
    F<uint16_t>(w, 0x7c) = F<uint16_t>(w, 4);
    F<uint16_t>(w, 0x6a) = 3;
    F<uint16_t>(w, 0x68) = 3;
}
RT3_REPLACE(0x557f80, Widget_LoadBase);

}  // namespace rt3
