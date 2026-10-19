// Block 0x41xxxx, small functions (batch 13). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_table62b2fc = hook::Global<Collection*>(0x62b2fc);
static Collection*& g_table62b268 = hook::Global<Collection*>(0x62b268);

uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_CheckActive(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Obj_StateB9(void* o);
void* __fastcall Obj_Ref3cc(void* o);
int32_t __fastcall Obj_Available(void* o);

// 0x411cb0. Frees the three buffers at +0x79c, +0x7a0 and +0x37.
void __fastcall Obj_FreeBuffers(void* o) {
    FreeAndNull(&F<void*>(o, 0x79c));
    FreeAndNull(&F<void*>(o, 0x7a0));
    FreeAndNull(&F<void*>(o, 0x37));
}
RT3_REPLACE(0x411cb0, Obj_FreeBuffers);

// 0x411d50. All vehicle entries unused (+0 == 0) and at least one undefined (+0x2c == 0).
int32_t __fastcall Obj_AllUnusedSomeUndefined(void* o) {
    int32_t found = 0;
    uint8_t* e = F<uint8_t*>(o, 0x37);
    for (int32_t i = 0; i < F<int32_t>(o, 0x33); ++i, e += 0xbc) {
        if (F<int32_t>(e, 0) != 0) return 0;
        if (F<int32_t>(e, 0x2c) == 0) found = 1;
    }
    return found ? 1 : 0;
}
RT3_REPLACE(0x411d50, Obj_AllUnusedSomeUndefined);

void __fastcall Obj_Nop411d90(void*) {}
RT3_REPLACE(0x411d90, Obj_Nop411d90);

// 0x411da0. Entry +0x798 of the static table 0x5ed33c (9 dwords each):
// copies its name, outputs its two values; 0 if none.
int32_t __fastcall Obj_StaticEntry(void* o, int, char* name, int32_t* a, int32_t* b) {
    int32_t i = F<int32_t>(o, 0x798);
    if (i == -1) return 0;
    const int32_t* rec = (const int32_t*)0x5ed33c + i * 9;
    if (rec[1] == 0) return 0;
    const char* s = (const char*)rec[0];
    while ((*name++ = *s++) != 0) {}
    *a = rec[2];
    *b = rec[3];
    return 1;
}
RT3_REPLACE(0x411da0, Obj_StaticEntry);

// 0x412880. ID of the 0x62b2fc record whose +0x173 equals this +0xe4, or 0.
uint32_t __fastcall Obj_FindRecord62b2fc(void* o) {
    for (int32_t i = 0; i < Collection_Count(g_table62b2fc); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(g_table62b2fc, 0, Collection_IdAt(g_table62b2fc, 0, i, 0));
        if (F<int32_t>(r, 0x173) == F<int32_t>(o, 0xe4)) return Collection_IdAt(g_table62b2fc, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x412880, Obj_FindRecord62b2fc);

// 0x4128e0. Sort priority +0xbc: 3 if +0x78c, 4 for kind 2, else 99999;
// overridden by the static name-prefix table 0x5edca8 ({prefix, value} x 15).
void __fastcall Obj_SetPriority(void* o) {
    F<int32_t>(o, 0xbc) = 0x1869f;
    if (F<int32_t>(o, 0x78c)) F<int32_t>(o, 0xbc) = 3;
    else if (F<uint8_t>(o, 0x32) == 2) F<int32_t>(o, 0xbc) = 4;
    for (const int32_t* e = (const int32_t*)0x5edca8; e < (const int32_t*)0x5edd20; e += 2) {
        const char* prefix = (const char*)e[0];
        size_t n = 0;
        while (prefix[n]) ++n;
        if (Strnicmp((const char*)o + 4, prefix, n) == 0) F<int32_t>(o, 0xbc) = e[1];
    }
}
RT3_REPLACE(0x4128e0, Obj_SetPriority);

// 0x412a90 / 0x413260. Destructors of two Collection subclasses (vtables
// 0x5c93cc / 0x5c9414): clear, then the base destructor.
void __fastcall Collection5c93cc_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c93cc;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x412a90, Collection5c93cc_Destruct);
void __fastcall Collection5c9414_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9414;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x413260, Collection5c9414_Destruct);

// 0x413230. Constructor of the 0x5c9414 collection (heap records, 10 + 20).
Collection* __fastcall Collection5c9414_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9414;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x413230, Collection5c9414_Construct);

// 0x413940. Sum over records with +0x3d0 == v of max(1, their 0x62b2fc record's +0x141).
int32_t __fastcall Table_SumCapacity(Collection* t, int, int32_t v) {
    int32_t sum = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x3d0) != v) continue;
        int32_t c = F<int32_t>(Obj_Ref3cc(r), 0x141);
        sum += c < 1 ? 1 : c;
    }
    return sum;
}
RT3_REPLACE(0x413940, Table_SumCapacity);

// 0x4139a0. Records with flag +0x3d4 == 1 that pass Obj_CheckActive.
int32_t __fastcall Table_CountActive(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Obj_Flag3d4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) != 1) continue;
        if (Obj_CheckActive(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) ++n;
    }
    return n;
}
RT3_REPLACE(0x4139a0, Table_CountActive);

// 0x413b40. Records with flag +0x3d4 == 1, state kind 4 and state byte +0xb9 == 0.
int32_t __fastcall Table_CountKind4Idle(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Obj_Flag3d4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) != 1) continue;
        if (!Obj_StateKindIs4(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) continue;
        if (Obj_StateB9(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0))) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x413b40, Table_CountKind4Idle);

// 0x413dd0. Its 0x62b268 type (+0x173) is available.
int32_t __fastcall Obj_TypeAvailable(void* o) {
    return Obj_Available(Collection_Get(g_table62b268, 0, F<uint32_t>(o, 0x173)));
}
RT3_REPLACE(0x413dd0, Obj_TypeAvailable);

void FreeBuffer62b2f8() {
    void*& p = hook::Global<void*>(0x62b2f8);
    if (p) {
        Free(p);
        p = nullptr;
    }
}
RT3_REPLACE(0x413f30, FreeBuffer62b2f8);

// 0x413f50. Pops the next id from the global queue (0x62ba70, count 0x62ba64); -1 if empty.
int32_t PopQueue62ba70() {
    if (!hook::Global<int32_t>(0x62ba6c) || hook::Global<int32_t>(0x62ba64) <= 0) return -1;
    int32_t*& p = hook::Global<int32_t*>(0x62ba70);
    int32_t v = *p++;
    hook::Global<int32_t>(0x62ba64)--;
    return v;
}
RT3_REPLACE(0x413f50, PopQueue62ba70);

// 0x414230 / 0x414250. Singly linked list {head, tail, count}: push / pop at the front.
void __fastcall SList_PushFront(void** l, int, void** node) {
    *node = l[0];
    ((int32_t*)l)[2]++;
    void* tail = l[1];
    l[0] = node;
    if (!tail) l[1] = node;
}
RT3_REPLACE(0x414230, SList_PushFront);
void* __fastcall SList_PopFront(void** l) {
    void** n = (void**)l[0];
    if (!n) return nullptr;
    l[0] = *n;
    if (n == (void**)l[1]) l[1] = *n;
    ((int32_t*)l)[2]--;
    return n;
}
RT3_REPLACE(0x414250, SList_PopFront);

// 0x4143e0 / 0x414410. Node pool alloc / release (see core/containers.cpp).
void* __fastcall Pool_Alloc(int32_t* p) {
    if (p[3] == 0) return ((void*(__cdecl*)(size_t))0x53b070)(p[4]);
    void** f = (void**)p[0];
    void** n = (void**)f[0];
    if (!n) return nullptr;
    f[0] = *n;
    if (n == (void**)f[1]) f[1] = *n;
    ((int32_t*)f)[2]--;
    return n;
}
RT3_REPLACE(0x4143e0, Pool_Alloc);
void __fastcall Pool_Release(int32_t* p, int, void** node) {
    if (p[3] == 0 || (uint8_t*)node < (uint8_t*)p[1] || (uint8_t*)node >= (uint8_t*)p[2]) {
        ((void(__cdecl*)(void*))0x53b080)(node);
        return;
    }
    void** f = (void**)p[0];
    *node = f[0];
    ((int32_t*)f)[2]++;
    void* tail = f[1];
    f[0] = node;
    if (!tail) f[1] = node;
}
RT3_REPLACE(0x414410, Pool_Release);

void __stdcall SetGlobal5ee030(int32_t v) { hook::Global<int32_t>(0x5ee030) = v; }
RT3_REPLACE(0x414470, SetGlobal5ee030);
int32_t GetQueueCount62ba64() { return hook::Global<int32_t>(0x62ba64); }
RT3_REPLACE(0x414480, GetQueueCount62ba64);

} // namespace rt3
