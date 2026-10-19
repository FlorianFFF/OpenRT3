// Block 0x42xxxx, functions (batch 22). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __fastcall Obj_SetNamedAmount(void* o, int, const char* name, float amount);
void __fastcall List1a_Free(uint8_t** head);
void __fastcall Obj_FreeFourLists(void* o);

// 0x420e00. Replaces the named-amount list +0x37f with a copy of other's
// (a fresh inline collection of 0x22-byte records, 2 + 2).
void __fastcall Obj_CopyNamedAmounts(void* o, int, void* other) {
    Collection* c = F<Collection*>(o, 0x37f);
    if (c) {
        Collection_Destruct(c);
        ((void(__cdecl*)(void*))0x53b080)(c);
    }
    F<Collection*>(o, 0x37f) = nullptr;
    Collection* n = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    if (n) n = Collection_Construct(n, 0, 1, 0x22, 2, 2, 0, 0, 0);
    F<Collection*>(o, 0x37f) = n;
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(other, 0x37f)); ++i) {
        Collection* src = F<Collection*>(other, 0x37f);
        uint8_t* r = (uint8_t*)Collection_Get(src, 0, Collection_IdAt(src, 0, i, 0));
        char name[0xc8];
        char* d = name;
        const char* s = (const char*)r;
        while ((*d++ = *s++) != 0) {}
        Obj_SetNamedAmount(o, 0, name, F<float>(r, 0x1e));
    }
}
RT3_REPLACE(0x420e00, Obj_CopyNamedAmounts);

// 0x42e3c0..0x42e4c3. Per-year block (stride 0x38d) sums: float pairs
// returned unrounded, integer sums of the +0x1dc0.. counters.
double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i) {
    uint8_t* b = c + y * 0x38d + i * 4;
    return (double)F<float>(b, 0x2058) + F<float>(b, 0x1f84);
}
RT3_REPLACE(0x42e3c0, Company_YearStatPair2);
double __fastcall Company_YearPair1db0(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1db4) + F<float>(b, 0x1db0);
}
RT3_REPLACE(0x42e3f0, Company_YearPair1db0);
double __fastcall Company_YearPair1db8(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return (double)F<float>(b, 0x1dbc) + F<float>(b, 0x1db8);
}
RT3_REPLACE(0x42e410, Company_YearPair1db8);
int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) + F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e430, Company_YearSum3);
int32_t __fastcall Company_YearNet1(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) - F<int32_t>(b, 0x1dcc) + F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e460, Company_YearNet1);
int32_t __fastcall Company_YearNet3(uint8_t* c, int, int32_t y) {
    uint8_t* b = c + y * 0x38d;
    return F<int32_t>(b, 0x1dc8) - F<int32_t>(b, 0x1dd8) - F<int32_t>(b, 0x1dd4) - F<int32_t>(b, 0x1dd0) +
           F<int32_t>(b, 0x1dc4) + F<int32_t>(b, 0x1dc0);
}
RT3_REPLACE(0x42e490, Company_YearNet3);

// 0x42e4d0..0x42e540. The same sums over the totals (+0x7664..) and +0xbc..
int32_t __fastcall Company_TotalNet1(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x7670) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4d0, Company_TotalNet1);
int32_t __fastcall Company_TotalNet3(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x767c) - F<int32_t>(c, 0x7678) - F<int32_t>(c, 0x7674) +
           F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4f0, Company_TotalNet3);
int32_t __fastcall Obj_NetBc1(void* o) {
    return F<int32_t>(o, 0xc4) - F<int32_t>(o, 0xc8) + F<int32_t>(o, 0xc0) + F<int32_t>(o, 0xbc);
}
RT3_REPLACE(0x42e520, Obj_NetBc1);
int32_t __fastcall Obj_NetBc3(void* o) {
    return F<int32_t>(o, 0xc4) - F<int32_t>(o, 0xd4) - F<int32_t>(o, 0xd0) - F<int32_t>(o, 0xcc) +
           F<int32_t>(o, 0xc0) + F<int32_t>(o, 0xbc);
}
RT3_REPLACE(0x42e540, Obj_NetBc3);

// 0x42e570. Frees the list +0 and the four lists.
void __fastcall Obj_FreeAllLists(void* o) {
    List1a_Free((uint8_t**)o);
    Obj_FreeFourLists(o);
}
RT3_REPLACE(0x42e570, Obj_FreeAllLists);

} // namespace rt3
