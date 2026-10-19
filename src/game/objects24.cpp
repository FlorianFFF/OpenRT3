// Small functions across blocks 0x46 (batch 27). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static inline void* ExeMalloc53b(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeFree53b(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

void __fastcall Collection5ce044_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce044;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x461560, Collection5ce044_Destruct);
void __fastcall Collection5ce068_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce068;
    Collection_Destruct(c);
}
RT3_REPLACE(0x4619b0, Collection5ce068_Destruct);

// 0x461ca0. 0x51c920 on the name (+0x45) of the 0x62ba88 record +0x72; 0 if none.
uint32_t __fastcall Obj_RefName72(void* o) {
    uint32_t id = F<uint32_t>(o, 0x72);
    if (!id) return 0;
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, id);
    if (!r) return 0;
    return ((uint32_t(__fastcall*)(void*, int))0x51c920)(r + 0x45, 0);
}
RT3_REPLACE(0x461ca0, Obj_RefName72);

uint32_t* __fastcall Pair_Init(uint32_t* p) {
    p[0] = 0;
    p[1] = 0;
    return p;
}
RT3_REPLACE(0x462560, Pair_Init);

// 0x468110 / 0x468140. Collection subclass 0x5ce130 (inline 0x19-byte records, 100 + 100).
Collection* __fastcall Collection5ce130_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce130;
    Collection_Init(c, 0, 1, 0x19, 100, 100, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x468110, Collection5ce130_Construct);
void __fastcall Collection5ce130_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce130;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x468140, Collection5ce130_Destruct);

void __fastcall Obj_Free108(void* o) {
    if (!F<void*>(o, 0x108)) return;
    Free(F<void*>(o, 0x108));
    F<void*>(o, 0x108) = nullptr;
}
RT3_REPLACE(0x4683f0, Obj_Free108);

void ResetGlobals4686d0() {
    hook::Global<int32_t>(0x5f2b24) = 1;
    hook::Global<int32_t>(0x6ada98) = 0;
}
RT3_REPLACE(0x4686d0, ResetGlobals4686d0);

// 0x468740 / 0x468970. Handle wrapper (vtbl 0x5ce15c): closes handle +4
// through the import 0x5c800c; the deleting form frees itself on flag 1.
uint32_t __fastcall HandleObj_Destruct(void* o) {
    *(uint32_t*)o = 0x5ce15c;
    return ((uint32_t(__stdcall*)(void*))hook::Global<void*>(0x5c800c))(F<void*>(o, 4));
}
RT3_REPLACE(0x468740, HandleObj_Destruct);
void* __fastcall HandleObj_Delete(void* o, int, uint32_t flags) {
    *(uint32_t*)o = 0x5ce15c;
    ((uint32_t(__stdcall*)(void*))hook::Global<void*>(0x5c800c))(F<void*>(o, 4));
    if (flags & 1) ExeFree53b(o);
    return o;
}
RT3_REPLACE(0x468970, HandleObj_Delete);

// 0x468b10. Inserts key -> value in the map +4 unless present; 1 if inserted.
int32_t __fastcall Obj_MapInsertNew(void* o, int, uint16_t key, int32_t value) {
    void* m = F<void*>(o, 4);
    if (((int32_t(__thiscall*)(void*, uint32_t))0x53daa0)(m, key)) return 0;
    ((void(__thiscall*)(void*, uint32_t, int32_t))0x53d960)(F<void*>(o, 4), key, value);
    return 1;
}
RT3_REPLACE(0x468b10, Obj_MapInsertNew);

void __fastcall Obj_Set8(void* o, int, int32_t v) { F<int32_t>(o, 8) = v; }
RT3_REPLACE(0x468b50, Obj_Set8);
int32_t __fastcall Obj_Sum987c(void* o) { return F<int32_t>(o, 0x987c) + F<int32_t>(o, 0x9894); }
RT3_REPLACE(0x468b60, Obj_Sum987c);

// 0x468ca0 / 0x468cb0. Forwarders to the map +0x9050.
uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value) {
    return ((uint32_t(__thiscall*)(void*, uint32_t, int32_t))0x53d960)(F<void*>(o, 0x9050), key, value);
}
RT3_REPLACE(0x468ca0, Obj_Map9050Insert);
uint32_t __fastcall Obj_Map9050Query(void* o, int, uint32_t key, int32_t mode) {
    void* m = F<void*>(o, 0x9050);
    if (mode == 1) return ((uint32_t(__thiscall*)(void*, uint32_t))0x53d9e0)(m, key);
    return ((uint32_t(__thiscall*)(void*, uint32_t))0x53dae0)(m, key);
}
RT3_REPLACE(0x468cb0, Obj_Map9050Query);

// 0x468ce0. Whether name +0x905c contains s (exe CRT strstr 0x5a1ff0).
int32_t __fastcall Obj_NameContains(uint8_t* o, int, const char* s) {
    return ((int(__cdecl*)(const char*, const char*))0x5a1ff0)((const char*)o + 0x905c, s) != 0 ? 1 : 0;
}
RT3_REPLACE(0x468ce0, Obj_NameContains);

void __fastcall Obj_SetName8f48(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 0x8f48;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x469700, Obj_SetName8f48);

// 0x4699e0. Value (+4) of the node with key +0x14 in the list *(+0x8f28); 0 if none.
uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key) {
    for (uint8_t* n = *F<uint8_t**>(o, 0x8f28); n; n = *(uint8_t**)n)
        if (F<int32_t>(n, 0x14) == key) return F<uint32_t>(n, 4);
    return 0;
}
RT3_REPLACE(0x4699e0, Obj_FindNode8f28);

// 0x46a390. Sets +0x8f30 and stamps the current date (+0x9048) and its minutes (+0x8f38).
void __fastcall Obj_StampDate(uint8_t* o, int, int32_t v) {
    F<int32_t>(o, 0x8f30) = v;
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    F<uint32_t>(o, 0x9048) = F<uint32_t>(w, 0xd);
    F<uint32_t>(o, 0x904c) = F<uint32_t>(w, 0x11);
    F<int32_t>(o, 0x8f38) = DateToMinutes((const GameDate*)(o + 0x9048));
}
RT3_REPLACE(0x46a390, Obj_StampDate);

// 0x46ab10. New empty 20-byte list header. QUIRK: writes through null on failure.
void* __stdcall NewListHeader(int32_t) {
    uint32_t* p = (uint32_t*)ExeMalloc53b(0x14);
    if (p) {
        p[2] = 0;
        p[3] = 0;
        p[4] = 0;
    }
    p[0] = 0;
    return p;
}
RT3_REPLACE(0x46ab10, NewListHeader);

// 0x46c2c0. Whether the file can be opened (mode 0x5ce2c0).
int32_t __fastcall FileOpens(const char* path) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ce2c0);
    if (!f) return 0;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return 1;
}
RT3_REPLACE(0x46c2c0, FileOpens);

} // namespace rt3
