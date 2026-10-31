// City building chain add / remove / refresh / record copy in block 0x42 (batch 158). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_Set2a2(void* o, int, uint16_t v);
void __fastcall City_UpdateCargoFlags(uint8_t* c);
void __fastcall Obj_UpdateFlagE6(void* o);

static inline Collection* Buildings() { return hook::Global<Collection*>(0x62b26c); }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void Refresh(uint8_t* c) {
    City_UpdateCargoFlags(c);
    Obj_UpdateFlagE6(c);
}

// 0x42c9a0. Puts building id at the head of the city's building chain (+0xd6,
// links +0x2a2) and refreshes the city's flags.
void __fastcall City_AddBuilding(uint8_t* c, int, uint16_t id) {
    Obj_Set2a2(Collection_Get(Buildings(), 0, id), 0, F<uint16_t>(c, 0xd6));
    F<uint16_t>(c, 0xd6) = id;
    Refresh(c);
}
RT3_REPLACE(0x42c9a0, City_AddBuilding);

// 0x42c9f0. Unlinks building id from the city's chain (giving up after 1000 links
// or a broken one) and refreshes the city's flags.
void __fastcall City_RemoveBuilding(uint8_t* c, int, uint16_t id) {
    uint32_t cur = F<uint16_t>(c, 0xd6);
    if (!cur) return;
    uint32_t prev = 0;
    int32_t steps = 0;
    bool found = true;
    while (cur != id) {
        if (steps++ > 1000) {
            found = false;
            break;
        }
        prev = cur;
        cur = F<uint16_t>(Collection_Get(Buildings(), 0, cur), 0x2a2);
        if (!Collection_IsValid(Buildings(), 0, cur)) {
            found = false;
            break;
        }
    }
    if (found) {
        uint16_t next = F<uint16_t>(Collection_Get(Buildings(), 0, id), 0x2a2);
        if (prev) Obj_Set2a2(Collection_Get(Buildings(), 0, prev), 0, next);
        else F<uint16_t>(c, 0xd6) = next;
    }
    Refresh(c);
}
RT3_REPLACE(0x42c9f0, City_RemoveBuilding);

// 0x42cdf0. Refreshes the city's cargo and +0xe6 flags.
void __fastcall City_Refresh(uint8_t* c) { Refresh(c); }
RT3_REPLACE(0x42cdf0, City_Refresh);

// Deep-copies a singly linked list of n-byte nodes (next at +nextOff) into *head.
static void CopyList(const uint8_t* src, uint8_t** head, uint32_t size, uint32_t nextOff) {
    uint8_t* prev = nullptr;
    for (int32_t i = 0; src; src = F<const uint8_t*>((void*)src, nextOff), ++i) {
        uint8_t* n = (uint8_t*)ExeNew(size);
        memcpy(n, src, size);
        F<void*>(n, nextOff) = nullptr;
        if (!i) *head = n;
        else F<uint8_t*>(prev, nextOff) = n;
        prev = n;
    }
}

// 0x42e050. Copies record src into dst: its six name strings, flags and the
// 4-slot tables, plus deep copies of its event list (+0, 0x1e-byte nodes) and four
// condition lists (+4.., 0x28-byte nodes).
void __fastcall Record_CopyFrom(uint8_t* dst, int, uint8_t* src) {
    static const uint16_t strs[] = { 0xe0, 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
    for (uint16_t off : strs) strcpy((char*)dst + off, (const char*)src + off);
    dst[0x7ee] = src[0x7ee];
    dst[0x7ef] = src[0x7ef];
    F<uint32_t>(dst, 0x7f0) = F<uint32_t>(src, 0x7f0);
    dst[0x7f4] = src[0x7f4];
    F<uint32_t>(dst, 0x7f5) = F<uint32_t>(src, 0x7f5);
    dst[0x7f9] = src[0x7f9];
    dst[0x7fa] = src[0x7fa];
    for (int k = 0; k < 4; ++k) {
        F<uint32_t>(dst, 4 + k * 4) = 0;
        dst[0x7fb + k] = src[0x7fb + k];
        F<uint32_t>(dst, 0x7ff + k * 4) = F<uint32_t>(src, 0x7ff + k * 4);
        F<uint32_t>(dst, 0x80f + k * 4) = F<uint32_t>(src, 0x80f + k * 4);
    }
    CopyList(F<uint8_t*>(src, 0), &F<uint8_t*>(dst, 0), 0x1e, 0x1a);
    for (int k = 0; k < 4; ++k) CopyList(F<uint8_t*>(src, 4 + k * 4), &F<uint8_t*>(dst, 4 + k * 4), 0x28, 0x24);
}
RT3_REPLACE(0x42e050, Record_CopyFrom);

} // namespace rt3
