// Block 0x42xxxx, functions (batch 22). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

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
RT3_NOINLINE double __fastcall Company_YearStatPair2(uint8_t* c, int, int32_t y, int32_t i) {
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
RT3_NOINLINE int32_t __fastcall Company_YearSum3(uint8_t* c, int, int32_t y) {
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
RT3_NOINLINE int32_t __fastcall Company_TotalNet1(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x7670) + F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4d0, Company_TotalNet1);
int32_t __fastcall Company_TotalNet3(void* c) {
    return F<int32_t>(c, 0x766c) - F<int32_t>(c, 0x767c) - F<int32_t>(c, 0x7678) - F<int32_t>(c, 0x7674) +
           F<int32_t>(c, 0x7668) + F<int32_t>(c, 0x7664);
}
RT3_REPLACE(0x42e4f0, Company_TotalNet3);
RT3_NOINLINE int32_t __fastcall Obj_NetBc1(void* o) {
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

// 0x420ed0. Rebuilds the city's building mix (+0x37f, records of 0x22 bytes): a
// rural city (+0x23e) gets houses, farms, camps and mines at 0.2 each; a town
// commercial blocks at 0.3 and houses plus every factory type at 0.2.
void __fastcall City_ResetBuildingMix(uint8_t* c) {
    if (Collection* old = F<Collection*>(c, 0x37f)) {
        Collection_Destruct(old);
        ((void(__cdecl*)(void*))0x53b080)(old);
    }
    F<Collection*>(c, 0x37f) = nullptr;
    Collection* t = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    F<Collection*>(c, 0x37f) = t ? Collection_Construct(t, 0, 1, 0x22, 2, 2, 0, 0, 0) : nullptr;
    static const uint32_t rural[] = { 0x5c9114, 0x5c9264, 0x5c9248, 0x5c923c, 0x5c9224, 0x5c920c, 0x5c91f4, 0x5c91e8, 0x5c91d0,
                                      0x5c91a0, 0x5c9188, 0x5c90d4, 0x5c8fdc, 0x5c935c, 0x5c9300, 0x5c8e74, 0x5c9108 };
    static const uint32_t town[] = { 0x5c9114, 0x5c929c, 0x5c8ff8, 0x5c93bc, 0x5c93a0, 0x5c938c, 0x5c9378, 0x5c9344, 0x5c9334, 0x5c92c8, 0x5c8df0,
                                     0x5c9168, 0x5c9080, 0x5c904c, 0x5c9038, 0x5c8e00, 0x5c8fc4, 0x5c8f08, 0x5c8eec, 0x5c8ecc, 0x5c8eac, 0x5c8e3c,
                                     0x5c9140 };
    uint32_t bits = 0x3e4ccccd, bits3 = 0x3e99999a;
    float fifth = *(float*)&bits, third = *(float*)&bits3;
    if (F<int32_t>(c, 0x23e)) {
        for (uint32_t n : rural) Obj_SetNamedAmount(c, 0, (const char*)n, fifth);
        return;
    }
    Obj_SetNamedAmount(c, 0, (const char*)0x5c92e8, third);
    for (uint32_t n : town) Obj_SetNamedAmount(c, 0, (const char*)n, fifth);
}
RT3_REPLACE(0x420ed0, City_ResetBuildingMix);

int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_BuildingKind(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

// 0x426f20. Names train t (+4, up to 50 chars) after the route between cities a and
// b. The bigger city (+0x25e) leads; +0xd57 becomes the owner's (t+0) active station
// in the bigger city, else in the smaller one. 65% of the time the name is
// "big-lesser" (0x5c9ba8); otherwise, or when that is too long, a compass name
// (0xb5d..0xb60) from where the big city lies, in cells from the map centre plus
// the step towards the lesser city (east/west only beyond 10 cells), and as a last
// resort 0xb61/0xb62 (60% the latter).
void __fastcall Train_AutoName(uint8_t* t, int, uint32_t a, uint32_t b) {
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    uint8_t* big = (uint8_t*)Collection_Get(cities, 0, a);
    uint8_t* lesser = (uint8_t*)Collection_Get(cities, 0, b);
    if (F<float>(lesser, 0x25e) > F<float>(big, 0x25e)) {
        uint8_t* x = lesser;
        lesser = big;
        big = x;
    }
    Collection* sts = hook::Global<Collection*>(0x6cec20);
    for (int32_t i = 0; i < Collection_Count(sts); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(sts, 0, Collection_IdAt(sts, 0, i, 0));
        int32_t owner = F<int32_t>(t, 0);
        if (Obj_BuildingValue(s) != owner || !Obj_BuildingStateActive(s)) continue;
        if (Obj_BuildingKind(s) == big || (F<uint16_t>(t, 0xd57) == 0 && Obj_BuildingKind(s) == lesser))
            F<uint16_t>(t, 0xd57) = F<uint16_t>(s, 0);
    }
    char buf[0xc8];
    const char* name = (const char*)big + 0x356;
    if (SimRand(0, 100) >= 0x23) {
        FormatText(buf, (const char*)0x5c9ba8, name, (const char*)lesser + 0x356, 0, 0, 0, 0, 0, 0);
        if (strlen(buf) <= 0x32) goto done;
    }
    {
        double (__fastcall* cellX)(void*) = &Obj_CellX;
        double (__fastcall* cellY)(void*) = &Obj_CellY;
        uint8_t* map = hook::Global<uint8_t*>(0x62c120);
        int32_t halfW = F<int32_t>(map, 0x2155) / 2, halfH = F<int32_t>(map, 0x2159) / 2;
        int32_t stepX, stepY, ex, ey;
        float tmp;
        __asm {
            mov ecx, lesser
            call cellX
            fstp tmp
            mov ecx, big
            call cellX
            fsubr tmp
            mov eax, 0x5a10d0
            call eax
            mov stepX, eax
            mov ecx, lesser
            call cellY
            fstp tmp
            mov ecx, big
            call cellY
            fsubr tmp
            mov eax, 0x5a10d0
            call eax
            mov stepY, eax
            mov ecx, big
            call cellX
            fild halfW
            fstp tmp
            fsub tmp
            fdiv dword ptr ds:[0x5c85f8]
            fild stepX
            fstp tmp
            fadd tmp
            mov eax, 0x5a10d0
            call eax
            mov ex, eax
            mov ecx, big
            call cellY
            fild halfH
            fstp tmp
            fsub tmp
            fdiv dword ptr ds:[0x5c85f8]
            fild stepY
            fstp tmp
            fadd tmp
            mov eax, 0x5a10d0
            call eax
            mov ey, eax
        }
        int32_t ax = ex < 0 ? -ex : ex, ay = ey < 0 ? -ey : ey;
        buf[0] = 0;
        int32_t id = 0;
        if (ax > ay) {
            if (ax > 10) id = ex > 0 ? 0xb5d : 0xb5e;
        } else {
            id = ey > 0 ? 0xb5f : 0xb60;
        }
        if (id) FormatText(buf, GetString(id), name, 0, 0, 0, 0, 0, 0, 0);
    }
    {
        size_t len = strlen(buf);
        if (len > 0x32 || len < 2) {
            int32_t id = SimRand(0, 100) >= 0x28 ? 0xb62 : 0xb61;
            FormatText(buf, GetString(id), name, 0, 0, 0, 0, 0, 0, 0);
        }
    }
done:
    buf[0x32] = 0;
    strcpy((char*)t + 4, buf);
}
RT3_REPLACE(0x426f20, Train_AutoName);

double __fastcall Obj_Remaining(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
void* __fastcall Obj_Ref3cc(void* o);
int32_t IsMultiplayer();
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                         int32_t serial);

// 0x422be0. Picks the building of kind (state +0x32) to give up among o's
// buildings (list +0x383, next +0x270; idle +0x276, not locked +0x390) and
// returns the chosen one's type value (+0x141 of its type record). Kind 2 is
// scored by its last three years' weighted remaining (0x40cac0) over output
// (0x40ca90), randomised (a random 1..20 when under 3 years old, 1..5 when
// low); a building without state +0x790 instead gets a random 1..3, or about
// 1e9 one time in o+0x28e; other kinds score 1..1000. The best is removed (in
// multiplayer games, unless +0x46c3c, its id goes to the host as message 0x73).
int32_t __fastcall Owner_PickBuildingToSell(uint8_t* o, int, int32_t kind) {
    typedef uint8_t*(__thiscall * StateFn)(uint8_t*);
    uint32_t bestBits = 0xbf800000;
    int32_t bestId = -1;
    Collection* blds = hook::Global<Collection*>(0x62b26c);
    for (uint32_t id = F<uint16_t>(o, 0x383); id;) {
        uint8_t* b = (uint8_t*)Collection_Get(blds, 0, id);
        id = F<uint16_t>(b, 0x270);
        uint8_t* st = ((StateFn)(*(void***)b)[0x80 / 4])(b);
        if ((int32_t)st[0x32] != kind || F<uint32_t>(b, 0x276) != 0 || b[0x390]) continue;
        float score;
        if (kind == 2) {
            int32_t age = (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - F<int32_t>(b, 0x3d5) + 1;
            if (age < 0 || age < 3) {
                score = (float)SimRand(1, 0x14);
            } else {
                float acc = 0.0f, sum = 0.0f;
                if (age > 3) age = 3;
                int32_t w = 4;
                for (int32_t i = 1; i <= age; ++i) {
                    float fw = (float)w;
                    acc = (float)(Obj_Remaining(b, 0, i) * fw + acc);
                    sum = (float)(Obj_Float310(b, 0, i) * fw + sum);
                    --w;
                }
                float denom = (1.0f > sum) ? 1.0f : sum;
                score = (float)((double)acc / denom);
                if (*(const float*)0x5c9b18 > score) score = -3.0f;
                double r = (double)SimRand(0x12c, 0x3e8);
                score = (float)((*(const double*)0x5c9b10 - score) * r);
                if (score < 1.0f) score = (float)SimRand(1, 5);
            }
            uint8_t* st2 = ((StateFn)(*(void***)b)[0x80 / 4])(b);
            if (!F<int32_t>(st2, 0x790)) {
                int32_t n = F<int32_t>(o, 0x28e);
                if (n < 1) n = 1;
                if (SimRand(0, n) == 0) score = (float)(SimRand(0, 3) + 0x3b9ac9f6);
                else score = (float)(SimRand(0, 3) + 1);
            }
        } else {
            score = (float)SimRand(1, 0x3e8);
        }
        if (score > *(float*)&bestBits) {
            bestBits = *(uint32_t*)&score;
            bestId = F<int32_t>(b, 0x2a4);
        }
    }
    if (bestId <= 0) return 0;
    int32_t value = F<int32_t>(Obj_Ref3cc(Collection_Get(blds, 0, (uint32_t)bestId)), 0x141);
    if (IsMultiplayer() && !F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x46c3c)) {
        uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
        Net_Send(s, 0, (const char*)s + 0x8f48, 0x73, 4, (const uint8_t*)&bestId, 5, 0);
        return value;
    }
    uint8_t* c = hook::Global<uint8_t*>(0x62b26c);
    ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)c)[2])(c, bestId);
    return value;
}
RT3_REPLACE(0x422be0, Owner_PickBuildingToSell);

} // namespace rt3
