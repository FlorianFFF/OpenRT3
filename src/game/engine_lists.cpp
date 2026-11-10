// Locked node pools and lists shared by the engine and the network code, and two
// COM setup helpers. A pool is { list*, lo, hi, enabled, size } where list is
// { head, tail, count }: taking pops the head (or allocates size bytes when the pool
// is disabled), giving pushes back nodes inside [lo, hi) and frees the rest.
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include "game/collection.h"
#include "core/containers.h"
#include "core/ptrlist.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
typedef void(WINAPI* CsFn)(LPCRITICAL_SECTION);
static inline void Enter(uint32_t cs, uint32_t iat) { hook::Global<CsFn>(iat)((LPCRITICAL_SECTION)cs); }

// The pool pop shared by the functions below.
static void* Pool_Take(uint8_t* pool) {
    if (!F<int32_t>(pool, 0xc)) return ExeNew(F<uint32_t>(pool, 0x10));
    uint8_t* list = F<uint8_t*>(pool, 0);
    uint8_t* node = F<uint8_t*>(list, 0);
    if (!node) return nullptr;
    uint8_t* next = F<uint8_t*>(node, 0);
    F<uint8_t*>(list, 0) = next;
    if (node == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = next;
    --F<int32_t>(list, 8);
    return node;
}

// The pool push: back onto the list when it is enabled and node is in range.
static void Pool_Give(uint8_t* pool, uint8_t* node) {
    if (F<int32_t>(pool, 0xc) && node >= F<uint8_t*>(pool, 4) && node < F<uint8_t*>(pool, 8)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        F<uint8_t*>(node, 0) = F<uint8_t*>(list, 0);
        F<uint8_t*>(list, 0) = node;
        ++F<int32_t>(list, 8);
        if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = node;
    } else ExeDelete(node);
}

// 0x469800. A network packet buffer: up to 0x300 bytes from the session's pool
// (+0x8f3c, under the lock 0x6cd940, spinning while its first word is set),
// larger ones (or when the pool gives none) from the heap.
RT3_NOINLINE void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size) {
    if (size <= 0x300) {
        while (*(volatile int32_t*)0x6cd940) {
        }
        Enter(0x6cd940, 0x5c8070);
        void* p = Pool_Take(F<uint8_t*>(s, 0x8f3c));
        Enter(0x6cd940, 0x5c8074);
        if (p) return p;
    }
    return ExeNew((size_t)size);
}
RT3_REPLACE(0x469800, NetPacket_Alloc);

// 0x469890. Frees a packet buffer: those inside the session's 0x48000-byte block
// (+0x8f40) go back to its pool, the rest to the heap.
RT3_NOINLINE void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p) {
    uint8_t* base = F<uint8_t*>(s, 0x8f40);
    if (p < base || p >= base + 0x48000) {
        ExeDelete(p);
        return;
    }
    while (*(volatile int32_t*)0x6cd940) {
    }
    Enter(0x6cd940, 0x5c8070);
    Pool_Give(F<uint8_t*>(s, 0x8f3c), p);
    Enter(0x6cd940, 0x5c8074);
}
RT3_REPLACE(0x469890, NetPacket_Free);

// 0x521d60. The texture entry with id key (list 0x6d408c). When found it returns
// with the lock 0x6d4090 still held (and its count +1), as the original does.
RT3_NOINLINE uint8_t* __stdcall Textures_Find(int32_t key) {
    Enter(0x6d4090, 0x5c81fc);
    ++hook::Global<int32_t>(0x6d40a8);
    uint8_t** l = hook::Global<uint8_t**>(0x6d408c);
    if (l)
        for (uint8_t* n = *l; n; n = F<uint8_t*>(n, 0))
            if (F<int32_t>(n, 4) == key) return n;
    --hook::Global<int32_t>(0x6d40a8);
    Enter(0x6d4090, 0x5c81f8);
    return nullptr;
}
RT3_REPLACE(0x521d60, Textures_Find);

// 0x522050. Looks up the entry for t's id (+0x18). keep == 1 returns the entry
// itself with the lock held; otherwise a copy (0x26f4 bytes at 0x6d4220) is returned
// after unlocking. Null when absent.
uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep) {
    uint8_t* result = nullptr;
    Enter(0x6d4090, 0x5c81fc);
    ++hook::Global<int32_t>(0x6d40a8);
    for (uint8_t* n = *hook::Global<uint8_t**>(0x6d408c); n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 4) == F<int32_t>(t, 0x18)) {
            if (keep == 1) return n;
            memcpy((void*)0x6d4220, n, 0x9bd * 4);
            result = (uint8_t*)0x6d4220;
            break;
        }
    --hook::Global<int32_t>(0x6d40a8);
    Enter(0x6d4090, 0x5c81f8);
    return result;
}
RT3_REPLACE(0x522050, Textures_Lookup);

// 0x521f90. Refreshes the stored surface description (+0x2698, 0x5c bytes) of
// texture key from the device (method 0x88); lock == 1 takes the lock around it.
// Whether it succeeded.
RT3_NOINLINE int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock) {
    int32_t ok = 0;
    if (lock == 1) {
        Enter(0x6d4090, 0x5c81fc);
        ++hook::Global<int32_t>(0x6d40a8);
    }
    for (uint8_t* n = *hook::Global<uint8_t**>(0x6d408c); n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 4) == key) {
            uint32_t desc[0x17];
            memset(desc, 0, sizeof desc);
            desc[0] = 0x5c;
            void* dev = F<void*>(F<uint8_t*>(e, 0x18e), 8);
            if (((HRESULT(__stdcall*)(void*, int32_t, uint32_t*, int32_t))(*(void***)dev)[0x88 / 4])(dev, key, desc, 0) >= 0) {
                memcpy(n + 0x2698, desc, sizeof desc);
                ok = 1;
            }
            break;
        }
    if (lock == 1) {
        --hook::Global<int32_t>(0x6d40a8);
        Enter(0x6d4090, 0x5c81f8);
    }
    return ok;
}
RT3_REPLACE(0x521f90, Textures_QueryDesc);

// 0x5221a0. Empties a node queue { head, tail, count }, returning the nodes to the
// command pool 0x6d4064 (lock 0x6d406c).
void __fastcall CmdQueue_Clear(uint8_t* q) {
    Enter(0x6d406c, 0x5c81fc);
    ++hook::Global<int32_t>(0x6d4084);
    for (;;) {
        uint8_t* n = F<uint8_t*>(q, 0);
        if (!n) break;
        F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(q, 8);
        Pool_Give(hook::Global<uint8_t*>(0x6d4064), n);
    }
    --hook::Global<int32_t>(0x6d4084);
    Enter(0x6d406c, 0x5c81f8);
}
RT3_REPLACE(0x5221a0, CmdQueue_Clear);

// 0x5220d0. A command node (0x28 bytes, +0x20 cleared; all zeroed when clear == 1)
// from the pool, locked when lock == 1.
uint8_t* __fastcall CmdNode_New(int32_t clear, int32_t lock) {
    uint8_t* n;
    if (lock == 1) {
        Enter(0x6d406c, 0x5c81fc);
        ++hook::Global<int32_t>(0x6d4084);
        n = (uint8_t*)Pool_Take(hook::Global<uint8_t*>(0x6d4064));
        --hook::Global<int32_t>(0x6d4084);
        Enter(0x6d406c, 0x5c81f8);
    } else n = (uint8_t*)Pool_Take(hook::Global<uint8_t*>(0x6d4064));
    if (n) {
        F<int32_t>(n, 0x20) = 0;
        if (clear == 1) memset(n, 0, 0x28);
    }
    return n;
}
RT3_REPLACE(0x5220d0, CmdNode_New);

// 0x522240. Queues command (a, b, c) on every channel of the list 0x6d40b0 (each a
// queue at +4), stamped with the frame 0x6d40e4; the number queued.
RT3_NOINLINE int32_t __fastcall CmdQueue_Broadcast(int32_t a, int32_t b, int32_t c) {
    int32_t count = 0;
    Enter(0x6d406c, 0x5c81fc);
    ++hook::Global<int32_t>(0x6d4084);
    for (uint8_t* ch = *hook::Global<uint8_t**>(0x6d40b0); ch; ch = F<uint8_t*>(ch, 0)) {
        uint8_t* n = (uint8_t*)Pool_Take(hook::Global<uint8_t*>(0x6d4064));
        if (!n) continue;
        F<int32_t>(n, 0x20) = 0;
        memset(n, 0, 0x28);
        F<int32_t>(n, 0x10) = a;
        F<int32_t>(n, 0x1c) = b;
        F<int32_t>(n, 8) = c;
        F<int32_t>(n, 0x18) = hook::Global<int32_t>(0x6d40e4);
        ++count;
        uint8_t* tail = F<uint8_t*>(ch, 8);
        if (tail) {
            F<uint8_t*>(n, 0) = F<uint8_t*>(tail, 0);
            F<uint8_t*>(tail, 0) = n;
        } else {
            F<uint8_t*>(n, 0) = F<uint8_t*>(ch, 4);
            F<uint8_t*>(ch, 4) = n;
        }
        ++F<int32_t>(ch, 0xc);
        F<uint8_t*>(ch, 8) = n;
    }
    --hook::Global<int32_t>(0x6d4084);
    Enter(0x6d406c, 0x5c81f8);
    return count;
}
RT3_REPLACE(0x522240, CmdQueue_Broadcast);

// 0x557af0 / 0x557b30. Creates the COM object 0x5c84e0 (interface 0x5c84d0) into
// +0xc / +0x10 and initialises it (method 0x34 with 0x5c84a0). Whether both worked.
static int32_t ComCreate(void** slot) {
    typedef HRESULT(WINAPI * CreateFn)(const void*, void*, DWORD, const void*, void**);
    if (hook::Global<CreateFn>(0x5c848c)((const void*)0x5c84e0, nullptr, 1, (const void*)0x5c84d0, slot) < 0) return 0;
    void* o = *slot;
    return ((HRESULT(__stdcall*)(void*, const void*))(*(void***)o)[0x34 / 4])(o, (const void*)0x5c84a0) >= 0 ? 1 : 0;
}
RT3_NOINLINE int32_t __fastcall Music_CreateC(uint8_t* m) { return ComCreate((void**)(m + 0xc)); }
RT3_REPLACE(0x557af0, Music_CreateC);
RT3_NOINLINE int32_t __fastcall Music_Create10(uint8_t* m) { return ComCreate((void**)(m + 0x10)); }
RT3_REPLACE(0x557b30, Music_Create10);

// 0x5579d0. Shuts the music objects down: stops the performance (+8, once, after
// the COM call 0x5c8494(0, 0)), then releases +0x10 and +0xc.
RT3_NOINLINE void __fastcall Music_Shutdown(uint8_t* m) {
    if (F<void*>(m, 8) && !F<int32_t>(m, 4)) {
        ((void(WINAPI*)(void*, void*))hook::Global<void*>(0x5c8494))(nullptr, nullptr);
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

// 0x46c2f0 body. The size of file name (0 when it cannot be opened).
static int32_t __cdecl File_SizeImpl(const char* name) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(name, (const char*)0x5ce2c0);
    if (!f) return 0;
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 2);
    int32_t size = ((int32_t(__cdecl*)(void*))0x5a254f)(f);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return size;
}
// 0x46c2f0. Takes the name in EAX.
__declspec(naked) void File_Size() {
    __asm {
        push eax
        call File_SizeImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46c2f0, File_Size);

int32_t ClockMs();

// 0x521b60. Whether a sound with rec's id (+0x104) and name (+0x84) is in the list
// 0x6d4044; touch also stamps the found one's time (+0x118).
int32_t __stdcall Sounds_Has(uint8_t* rec, int32_t touch) {
    for (uint8_t* n = *hook::Global<uint8_t**>(0x6d4044); n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 0x104) == F<int32_t>(rec, 0x104) && !strcmp((const char*)n + 0x84, (const char*)rec + 0x84)) {
            if (touch) F<int32_t>(n, 0x118) = ClockMs();
            return 1;
        }
    return 0;
}
RT3_REPLACE(0x521b60, Sounds_Has);

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
        void* buf = ExeNew(size);
        o = F<void*>(m, 8);
        F<void*>(m, 0x14) = buf;
        if (((GetFn)(*(void***)o)[0x10 / 4])(o, (const void*)0x5c84a0, 0, buf, &size, &got, 0) >= 0) {
            if (got == 1) return 1;
            o = F<void*>(m, 8);
            if (((GetFn)(*(void***)o)[0x10 / 4])(o, (const void*)0x5c84a0, 0, F<void*>(m, 0x14), &size, &got, 0) >= 0) ok = 1;
        }
    }
    if (got < 1) {
        hook::Global<int32_t>(0x6d411c) = 99999;
        return 0;
    }
    return ok;
}
RT3_REPLACE(0x557a30, Music_ReadParam);

void* Engine_FirstListEntry();
struct HashMap;
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t);

// 0x4705f0. Looks up the session's entry for o's id (*o[8]) in the first texture
// entry's hash (+0x60) and, when present, hands it to 0x521790 (a no-op returning 1).
int32_t __fastcall Net_TouchEntry(uint8_t* o) {
    uint8_t* first = (uint8_t*)Engine_FirstListEntry();
    int32_t h = Hash_Get(F<HashMap*>(first, 0x60), 0, *F<uint32_t*>(o, 8));
    if (h) Return1_521790(hook::Global<void*>(0x6d40d0), 0, h, (int32_t)(uintptr_t)(hook::Global<uint8_t*>(0x6cd8d8) + 0x8f48));
    return 1;
}
RT3_REPLACE(0x4705f0, Net_TouchEntry);

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
        ExeDelete(b);
        F<void*>(m, 0x14) = nullptr;
    }
    if (F<int32_t>(m, 0) == 1) {
        ((void(WINAPI*)())hook::Global<void*>(0x5c8490))();
        F<int32_t>(m, 0) = 0;
    }
}
RT3_REPLACE(0x557b70, Music_Destroy);

// 0x521620. Sets the stream buffer length property (0x5dcf70, 4 bytes, how 2) of
// COM object o to ms (9000 when 0), keeping it in +0x216; whether it succeeded.
RT3_NOINLINE int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms) {
    if (!ms) ms = 0x2328;
    F<int32_t>(e, 0x216) = ms;
    return ((HRESULT(__stdcall*)(void*, const void*, int32_t*, int32_t, int32_t))(*(void***)o)[0x48 / 4])(o, (const void*)0x5dcf70, &ms, 4, 2) == 0 ? 1 : 0;
}
RT3_REPLACE(0x521620, Sound_SetBufferMs);

uint32_t __stdcall HeatColor(float t);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void __stdcall Net_UnlockQueueArg(int32_t);
void Net_UnlockQueue();
int32_t Net_LockQueue();
int32_t Global6d4100();
int32_t Global6d40e4();
static inline int Stricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }

// 0x493000. Tints every object of table t: the heat colour for 1 when its group
// (+0x22e) is group, else for 0.
void __fastcall Objects_HighlightGroup(Collection* t, int, int32_t group) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        float on = (float)(F<uint16_t>(o, 0x22e) == group ? 1 : 0);
        uint32_t c = HeatColor(on);
        Obj_SetRgb(o, 0, (uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c);
    }
}
RT3_REPLACE(0x493000, Objects_HighlightGroup);

// 0x46d9e0. The index of the player profile (table 0x6ceb9c) whose name matches
// that of texture entry key, or 0xff; releases the entry's lock.
RT3_NOINLINE int32_t __stdcall Profiles_IndexOfEntry(int32_t key) {
    int32_t idx = 0xff;
    uint8_t* e = Textures_Find(key);
    if (!e) return idx;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ceb9c)); ++i) {
        Collection* t = hook::Global<Collection*>(0x6ceb9c);
        uint8_t* p = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (!Stricmp((const char*)p + 8, (const char*)e + 8)) {
            idx = i;
            break;
        }
    }
    Net_UnlockQueueArg(key);
    return idx;
}
RT3_REPLACE(0x46d9e0, Profiles_IndexOfEntry);

// 0x469ca0. Refreshes the surface description (+0x54 from +0x269c) of every queued
// texture entry except the current frame's (network play with a world only).
RT3_NOINLINE void __stdcall Textures_RefreshQueued(int32_t) {
    if (hook::Global<void*>(0x6cec78) && !hook::Global<int32_t>(0x6cd910)) return;
    if (!Engine_FirstListEntry()) return;
    if (Global6d4100() <= 1) return;
    uint8_t** l = (uint8_t**)(uintptr_t)Net_LockQueue();
    if (!l) return;
    for (uint8_t* n = *l; n; n = F<uint8_t*>(n, 0)) {
        int32_t id = F<int32_t>(n, 4);
        if (id == Global6d40e4()) continue;
        F<int32_t>(n, 0x54) = 0;
        if (Textures_QueryDesc(hook::Global<uint8_t*>(0x6d40d0), 0, id, 0)) F<int32_t>(n, 0x54) = F<int32_t>(n, 0x269c);
    }
    Net_UnlockQueue();
}
RT3_REPLACE(0x469ca0, Textures_RefreshQueued);

void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);

// 0x4c9cc0. Destroys the game-setup window: releases its cached art (0x6cff88..
// 0x6cff90 and the 41 slots 0x6cfed0..), refreshes which industry types are
// available (the original 0x412c10, which stays unhooked) and becomes a plain widget.
void __fastcall SetupWindow_Destroy(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0ac4;
    // Each slot is read before the previous one is cleared, as in the original.
    for (uint32_t g = 0x6cff88; g <= 0x6cff90; g += 4) {
        void* v = hook::Global<void*>(g);
        hook::Global<void*>(g - 4) = nullptr;
        if (v) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, v);
    }
    hook::Global<void*>(0x6cff90) = nullptr;
    for (uint32_t* p = (uint32_t*)0x6cfed0; (uint32_t)(uintptr_t)p < 0x6cff74; ++p) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, (void*)(uintptr_t)*p);
    ((void(__fastcall*)(void*))0x412c10)(hook::Global<void*>(0x62b268));
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4c9cc0, SetupWindow_Destroy);

// 0x557bc0. Starts the music player: COM and the performance object (0x5c84c0,
// interface 0x5c84b0) on first use, its init (method 0xc with notify 0x5579b0),
// the parameter buffer and both helper objects. 1 on success; on failure the error
// goes to 0x6d411c (99998 when a helper failed).
RT3_NOINLINE int32_t __fastcall Music_Start(uint8_t* m) {
    typedef HRESULT(WINAPI * CreateFn)(const void*, void*, DWORD, const void*, void**);
    Music_Shutdown(m);
    if (!F<int32_t>(m, 0)) {
        ((void(WINAPI*)(void*, void*))hook::Global<void*>(0x5c8494))(nullptr, nullptr);
        HRESULT hr = hook::Global<CreateFn>(0x5c848c)((const void*)0x5c84c0, nullptr, 1, (const void*)0x5c84b0, (void**)(m + 8));
        if (hr < 0) {
            hook::Global<int32_t>(0x6d411c) = hr;
            return 0;
        }
        F<int32_t>(m, 0) = 1;
    }
    void* perf = F<void*>(m, 8);
    HRESULT hr = ((HRESULT(__stdcall*)(void*, void*, uint32_t, void*))(*(void***)perf)[0xc / 4])(perf, nullptr, 0x5579b0, nullptr);
    if (hr < 0) {
        hook::Global<int32_t>(0x6d411c) = hr;
        return 0;
    }
    F<int32_t>(m, 4) = 0;
    if (!F<void*>(m, 0x14) && !Music_ReadParam(m)) return 0;
    int32_t r = Music_CreateC(m);
    if (r == 1) {
        r = Music_Create10(m);
        if (r) return r;
    }
    hook::Global<int32_t>(0x6d411c) = 99998;
    return r;
}
RT3_REPLACE(0x557bc0, Music_Start);

const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

// 0x522d20. A player left the session (message: id +4, reason +0xc): the player
// record with that id (list 0x6d408c) gets the matching leave text (strings
// 0x58..0x5b, with its name +8) at +0x78, the player count 0x6d4100 drops and a
// command 6 for it is queued on every channel (count kept at +0x4c).
int32_t __fastcall Net_PlayerLeft(int32_t, uint8_t* msg) {
    Enter(0x6d4090, 0x5c81fc);
    ++hook::Global<int32_t>(0x6d40a8);
    uint8_t* p = *hook::Global<uint8_t**>(0x6d408c);
    while (p && F<int32_t>(p, 4) != F<int32_t>(msg, 4)) p = F<uint8_t*>(p, 0);
    --hook::Global<int32_t>(0x6d40a8);
    Enter(0x6d4090, 0x5c81f8);
    if (!p) return 0;
    int32_t why = F<int32_t>(msg, 0xc);
    if (why >= 1 && why <= 4)
        FormatText((char*)p + 0x78, GetString(0x57 + why), (const char*)p + 8, 0, 0, 0, 0, 0, 0, 0);
    Enter(0x6d4100, 0x5c8074);
    F<int32_t>(p, 0x4c) = CmdQueue_Broadcast(6, 0, (int32_t)(uintptr_t)p);
    return 0;
}
RT3_REPLACE(0x522d20, Net_PlayerLeft);

void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);

// 0x4edd00. Adds a button for player key to the lobby panel 0x6d1268 (id 0xe61 +
// slot, parent 0xe14) at the right edge of (x, y, w): labelled with string 0xe66
// when the player's flags (+0x5c) have bit 0, else 0xe67. The panel's list 0xe14
// is then refreshed (event 0x86 on pages 0xe15 / 0xe16, else 0x6d).
void __fastcall Lobby_AddPlayerButton(int32_t, int32_t slot, int32_t key, float x, float y, float w, float) {
    if (!hook::Global<void*>(0x6d40d0)) return;
    uint8_t* e = Textures_Find(key);
    if (!e) return;
    uint32_t flags = F<uint32_t>(e, 0x5c);
    Net_UnlockQueueArg(key);
    uint8_t d[0xa4];
    memset(d, 0, 0xa1);
    F<float>(d, 7) = (float)((double)x + w - (double)hook::Global<float>(0x5d0100));
    F<float>(d, 0xb) = (float)((double)y + (double)hook::Global<float>(0x5d0188));
    F<uint32_t>(d, 0xf) = 0x3c4ccccd;
    F<uint32_t>(d, 0x13) = 0x3c888889;
    d[0x29] = 0x20;
    F<int32_t>(d, 0x67) = 0x16;
    F<int32_t>(d, 0x6b) = 0x17;
    F<int32_t>(d, 0x4b) = key;
    F<uint16_t>(d, 0) = (uint16_t)(slot + 0xe61);
    F<uint16_t>(d, 5) = 0xe14;
    String_Assign(&F<char*>(d, 0x43), (const char*)0x5d1258);
    String_Assign(&F<char*>(d, 0x3f), GetString((flags & 1) ? 0xe66 : 0xe67));
    F<int32_t>(d, 0x89) = (flags & 1) ? 1 : 0;
    uint8_t* b = (uint8_t*)ExeNew(0xe8);
    if (b) b = Button_Construct(b, 0, d);
    Window_AddChild(hook::Global<uint8_t*>(0x6d1268), 0, b, 0);
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0x43));
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0x3f));
    uint8_t* panel = hook::Global<uint8_t*>(0x6d1268);
    int32_t page = F<int32_t>(panel, 0x78);
    Window_SendEvent5(panel, 0, page == 0xe15 || page == 0xe16 ? 0x86 : 0x6d, 0xe14, 3, 0, 0);
}
RT3_REPLACE(0x4edd00, Lobby_AddPlayerButton);

char* __fastcall Str_FromWide(char* dst, const wchar_t* src, int32_t n);
void* __fastcall FreeList_Take(uint8_t* q);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
uint32_t __fastcall SysInfo_CpuSignature();

// 0x522ab0. A player joined (message: id +4): reads its DirectPlay8 peer info
// (name, flags) and adds a player record to the list 0x6d408c (name +8, id +4,
// name hash +0x48, message tables +0x60 / +0x2580, queues +0x64 / +0x6c); the
// local player (flag 2) becomes 0x6d40e4, the host (flag 4) gets +0x50. The player
// count 0x6d4100 rises and a command 5 for it is queued. 0 when added, else -1.
int32_t __fastcall Net_PlayerJoined(int32_t, uint8_t* msg) {
    void* peer = hook::Global<void*>(0x6d40e0);
    typedef int32_t(__stdcall * GetInfo)(void*, int32_t, void*, uint32_t*, uint32_t);
    uint32_t size = 0;
    int32_t result = -1;
    if ((*(GetInfo*)(*(uint8_t**)peer + 0x54))(peer, F<int32_t>(msg, 4), nullptr, &size, 0) != (int32_t)0x80158100) return result;
    uint32_t* info = (uint32_t*)ExeNew(size);
    memset(info, 0, 0x18);
    info[0] = 0x18;
    peer = hook::Global<void*>(0x6d40e0);
    if ((*(GetInfo*)(*(uint8_t**)peer + 0x54))(peer, F<int32_t>(msg, 4), info, &size, 0) >= 0) {
        char name[0x40];
        Str_FromWide(name, (const wchar_t*)(uintptr_t)info[2], -1);
        uint32_t hash = Crc32String(name, 1);
        Enter(0x6d4090, 0x5c81fc);
        ++hook::Global<int32_t>(0x6d40a8);
        if (hook::Global<void*>(0x6d408c)) {
            if (uint8_t* p = (uint8_t*)FreeList_Take(hook::Global<uint8_t*>(0x6d4088))) {
                memset(p, 0, 0x9bd * 4);
                strcpy((char*)p + 8, name);
                F<int32_t>(p, 4) = F<int32_t>(msg, 4);
                F<uint32_t>(p, 0x48) = hash;
                uint32_t* m = (uint32_t*)ExeNew(0x14);
                if (m) memset(m, 0, 0x14);
                F<uint32_t*>(p, 0x60) = m;
                Hash_Init((HashMap*)m, 0, 0x65, 0x200);
                m = (uint32_t*)ExeNew(0x14);
                if (m) memset(m, 0, 0x14);
                F<uint32_t*>(p, 0x2580) = m;
                Hash_Init((HashMap*)m, 0, 0x1f, 0x200);
                m = (uint32_t*)ExeNew(0xc);
                if (m) memset(m, 0, 0xc);
                F<uint32_t*>(p, 0x64) = m;
                F<int32_t>(p, 0x68) = 0;
                m = (uint32_t*)ExeNew(0x1c);
                if (m) memset(m, 0, 0x10);
                F<uint32_t*>(p, 0x6c) = m;
                Heap_Init((MaxHeap*)m, 0, 0x400);
                F<int32_t>(p, 0x70) = 0x7ffffffe;
                F<int32_t>(p, 0x74) = 0x7ffffffe;
                F<int32_t>(p, 0x5c) = 1;
                F<int32_t>(p, 0x2690) = 1;
                void** list = hook::Global<void**>(0x6d408c);
                SList_InsertAfter(list, 0, (void**)list[1], (void**)p);
                if (info[5] & 2) {
                    hook::Global<int32_t>(0x6d40e4) = F<int32_t>(p, 4);
                    F<uint32_t>(p, 0x2694) = SysInfo_CpuSignature();
                    F<int32_t>(p, 0x268c) = hook::Global<int32_t>(0x6d4108);
                }
                if (info[5] & 4) F<int32_t>(p, 0x50) = 1;
                Enter(0x6d4100, 0x5c8070);
                CmdQueue_Broadcast(5, 0, (int32_t)(uintptr_t)p);
                result = 0;
            }
        }
        --hook::Global<int32_t>(0x6d40a8);
        Enter(0x6d4090, 0x5c81f8);
    }
    ExeDelete(info);
    return result;
}
RT3_REPLACE(0x522ab0, Net_PlayerJoined);

void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall Terrain_Destructor(uint8_t* r);
void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Windows_Shutdown(uint8_t* m);
void __fastcall Nop564170();
void __fastcall Cursor_Shutdown(uint8_t* c);
void __fastcall Mouse_Destroy(uint8_t* m);
void __fastcall Pack_Shutdown(uint8_t* t);
void __fastcall Nop54e780();
void __fastcall Sound_Destruct(uint8_t* snd);
void __fastcall ModelLibrary_Free(uint32_t* o);
void FreeLanguageStrings();
void StreamReleaseAll();

// 0x521390. Tears the engine e down: its three pointer lists, the terrain (+0x1c),
// the device (Engine_Shutdown), the three pack images, then the window manager
// 0x6d401c, +0x24, the cursor 0xccbb20, the mouse (+0x20), the pack 0x6d4020, the
// globals 0x6d4018 / 0x6d402c (sound) / 0x6d4030 (models), the buffer +0x34, the
// language strings and the streams; 0x6d4024 (the engine) is cleared.
void __fastcall Engine_Destroy(uint8_t* e) {
    F<int32_t>(e, 0x114201) = 0;
    static const uint32_t lists[] = { 0x11369d, 0x1136a1, 0x1136a5 };
    for (uint32_t off : lists)
        if (PtrList* l = F<PtrList*>(e, off)) {
            PtrList_Destroy(l);
            ExeDelete(l);
        }
    if (uint8_t* t = F<uint8_t*>(e, 0x1c)) {
        Terrain_Destructor(t);
        ExeDelete(t);
    }
    F<uint8_t*>(e, 0x1c) = nullptr;
    Engine_Shutdown(e, 0, 1);
    static const uint32_t images[] = { 0x1141f5, 0x1141f9, 0x1141fd };
    for (uint32_t off : images) {
        if (void* p = F<void*>(e, off)) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, p);
        F<void*>(e, off) = nullptr;
    }
    if (uint8_t* m = hook::Global<uint8_t*>(0x6d401c)) {
        Windows_Shutdown(m);
        ExeDelete(m);
    }
    hook::Global<uint8_t*>(0x6d401c) = nullptr;
    if (uint8_t* p = F<uint8_t*>(e, 0x24)) {
        Nop564170();
        ExeDelete(p);
    }
    F<uint8_t*>(e, 0x24) = nullptr;
    if (uint8_t* c = hook::Global<uint8_t*>(0xccbb20)) {
        Cursor_Shutdown(c);
        ExeDelete(c);
    }
    hook::Global<uint8_t*>(0xccbb20) = nullptr;
    if (uint8_t* m = F<uint8_t*>(e, 0x20)) {
        Mouse_Destroy(m);
        ExeDelete(m);
    }
    F<uint8_t*>(e, 0x20) = nullptr;
    if (uint8_t* p = hook::Global<uint8_t*>(0x6d4020)) {
        Pack_Shutdown(p);
        ExeDelete(p);
    }
    hook::Global<uint8_t*>(0x6d4020) = nullptr;
    if (uint8_t* p = hook::Global<uint8_t*>(0x6d4018)) {
        Nop54e780();
        ExeDelete(p);
    }
    hook::Global<uint8_t*>(0x6d4018) = nullptr;
    if (uint8_t* s = hook::Global<uint8_t*>(0x6d402c)) {
        Sound_Destruct(s);
        ExeDelete(s);
    }
    hook::Global<uint8_t*>(0x6d402c) = nullptr;
    if (uint32_t* m = hook::Global<uint32_t*>(0x6d4030)) {
        ModelLibrary_Free(m);
        ExeDelete(m);
    }
    hook::Global<uint32_t*>(0x6d4030) = nullptr;
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(e, 0x34));
    FreeLanguageStrings();
    StreamReleaseAll();
    hook::Global<void*>(0x6d4024) = nullptr;
}
RT3_REPLACE(0x521390, Engine_Destroy);

} // namespace rt3
