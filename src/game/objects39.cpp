// Functions across blocks 0x45..0x48 (batch 42). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all);
void* __fastcall ReturnThis473cc0(void* o);
void __fastcall Nop473cd0(void*);
void __fastcall News_Init(uint8_t* e, int, int32_t kind, const char* title, const char* text, int32_t a, int32_t b);

// 0x457e10. (+0x2dd, +0x2e1, +0x2e5) scaled by 0x5cb468, through the matrix
// +0x18e, then scaled by +0x27d.
void __fastcall Obj_OffsetVector(uint8_t* o, int, float* out) {
    const float k = *(const float*)0x5cb468;
    float v[3] = { (float)((double)F<float>(o, 0x2dd) * k), (float)((double)F<float>(o, 0x2e1) * k),
                   (float)((double)F<float>(o, 0x2e5) * k) };
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform(v, 0, r, (const float*)(o + 0x18e));
    float z = (float)((double)s * t[2]);
    float y = (float)((double)s * t[1]);
    out[1] = y;
    out[2] = z;
    out[0] = (float)((double)s * t[0]);
}
RT3_REPLACE(0x457e10, Obj_OffsetVector);

// 0x45a2e0. Starts a block iteration of o and clears (mode 1) or sets `mask`
// in the flags +0x8c of every block.
void __fastcall BlockIter_SetFlags(uint8_t* o, uint32_t mask, int32_t mode) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<uint8_t*>(0x6acd50) = o;
    hook::Global<int32_t>(0x6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    hook::Global<uint8_t*>(0x6acd4c) = p;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i, p += 0x168) {
        if (mode == 1) F<uint32_t>(p, 0x8c) &= ~mask;
        else F<uint32_t>(p, 0x8c) |= mask;
        o = hook::Global<uint8_t*>(0x6acd50);
    }
}
RT3_REPLACE(0x45a2e0, BlockIter_SetFlags);

// 0x468b70. Keeps the entries of the current row (see 0x468c10) whose dword
// +5 exceeds `limit`, compacting them into the other row, which becomes current.
void __stdcall Table9_Prune(uint8_t* o, uint32_t limit) {
    int32_t n = F<int32_t>(o, 0x178);
    if (n <= 0) return;
    int32_t row = F<int32_t>(o, 0x17c);
    uint8_t* src = o + row * 9 * 512 + 0x180;
    uint8_t* dst = o + (row ^ 1) * 9 * 512 + 0x180;
    int32_t kept = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 0x178); ++i, src += 9) {
        if (limit >= F<uint32_t>(src, 5)) continue;
        F<uint32_t>(dst, 0) = F<uint32_t>(src, 0);
        F<uint32_t>(dst, 4) = F<uint32_t>(src, 4);
        dst[8] = src[8];
        dst += 9;
        ++kept;
    }
    F<int32_t>(o, 0x17c) ^= 1;
    F<int32_t>(o, 0x178) = kept;
}
RT3_REPLACE(0x468b70, Table9_Prune);

// 0x473f80. Adds a news entry, first dropping the oldest while more than 25.
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b) {
    while (Collection_Count(t) > 0x19) {
        uint32_t id = F<uint32_t>(Table_Oldest14(t, 0, 1), 0);
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
    uint8_t local[0x188] = {};  // original: uninitialised stack (overwritten by News_Init)
    ReturnThis473cc0(local);
    uint32_t id = Collection_Allocate(t, 0, local, 0, 0, 0);
    News_Init((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id, title, text, a, b);
    Nop473cd0(local);
    return id;
}
RT3_REPLACE(0x473f80, News_Add);

// 0x487780. Picks a region (+0x35) other than this one with +0x31 set
// (keeps the current one if still valid); 1 if none. Only while own +0x31 is 0.
void __fastcall Region_PickOther(uint8_t* o) {
    if (F<int32_t>(o, 0x31)) return;
    Collection* regions = hook::Global<Collection*>(0x6cfc9c);
    int32_t cur = F<int32_t>(o, 0x35);
    if (cur != F<int32_t>(o, 0) && Collection_IsValid(regions, 0, cur) &&
        F<int32_t>(Collection_Get(regions, 0, F<uint32_t>(o, 0x35)), 0x31))
        return;
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0) != F<int32_t>(o, 0) && F<int32_t>(r, 0x31)) {
            F<int32_t>(o, 0x35) = F<int32_t>(r, 0);
            return;
        }
    }
    F<int32_t>(o, 0x35) = 1;
}
RT3_REPLACE(0x487780, Region_PickOther);

// 0x48a3c0. As 0x48a380 with class 13.
uint32_t __fastcall Track_MarkSpecial13(uint8_t* t, int, int32_t v) {
    if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) return 0xffffffff;
    if (F<int32_t>(t, 0x24d)) return F<uint32_t>(t, 0x24d);
    F<int32_t>(t, 0x226) = 0xd;
    F<int32_t>(t, 0x24d) = v;
    return (uint32_t)v;
}
RT3_REPLACE(0x48a3c0, Track_MarkSpecial13);

int32_t __fastcall Track_Link2(uint8_t* t, int, int32_t second) { return second ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206); }
RT3_REPLACE(0x48a420, Track_Link2);

// 0x48a590. Wear +0x22a grows by 0x5c8d48 for kind 4, else by 0x5c85b4.
void __fastcall Track_AddWear(uint8_t* t) {
    float inc = t[0x218] == 4 ? *(const float*)0x5c8d48 : *(const float*)0x5c85b4;
    F<float>(t, 0x22a) = (float)((double)F<float>(t, 0x22a) + inc);
}
RT3_REPLACE(0x48a590, Track_AddWear);

int32_t __fastcall Track_Field238(uint8_t* t, int, int32_t flag) {
    return flag || t[0x218] == 1 ? F<int32_t>(t, 0x238) : F<int32_t>(t, 0x23c);
}
RT3_REPLACE(0x48a690, Track_Field238);

void __fastcall Nop48a780(void*) {}
RT3_REPLACE(0x48a780, Nop48a780);

float __fastcall Obj_Mid1ed(uint8_t* o) {
    return (float)(((double)F<float>(o, 0x1f1) + F<float>(o, 0x1ed)) / *(const double*)0x5c85d0);
}
RT3_REPLACE(0x48aab0, Obj_Mid1ed);

// 0x48b630. Option +0x2bf: 0 off, 2 on, else on in screen mode 12 (0x6d0818 +0x8c).
int32_t Option2bfActive() {
    int32_t m = F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x2bf);
    if (m == 0) return 0;
    if (m == 2) return 1;
    uint8_t* s = hook::Global<uint8_t*>(0x6d0818);
    return s && F<int32_t>(s, 0x8c) == 0xc ? 1 : 0;
}
RT3_REPLACE(0x48b630, Option2bfActive);

// 0x48b8d0. Virtual slot 6 (1) for an unclassed track piece with +8, unless the node table's +0xed is set.
void __fastcall Track_Refresh(uint8_t* t) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cfca8), 0xed)) return;
    if (F<int32_t>(t, 0x226) || !F<int32_t>(t, 8)) return;
    ((void(__thiscall*)(void*, int32_t))(*(void***)t)[0x18 / 4])(t, 1);
}
RT3_REPLACE(0x48b8d0, Track_Refresh);

} // namespace rt3
