// Cargo transfer / version-gated values / model sets / Miles 3D sound layer in blocks 0x42..0x56 (batch 117). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Game_EffectiveVersion();
int32_t __stdcall Model_FreeFrames(uint8_t* m);
void __fastcall Lists_FreeItems(uint8_t* o);
int32_t ClockMs();
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z);
void __fastcall Sound_WaitIdle();

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline const char* ExeStrStr(const char* a, const char* b) {
    return ((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)(a, b);
}
static inline void AilLock() { Import<void(__stdcall*)()>(0x5c8484)(); }
static inline void AilUnlock() { Import<void(__stdcall*)()>(0x5c83e4)(); }

// 0x42bf80. Moves part of cargo i's stock (+0x103) from this city to `dst`: a fixed
// share (0x5c9c50), reduced (v1.03+) when nearly empty and for over-capacity stock
// (+0x6a); the waiting count (+0) shrinks by `amount` (taken from dst's count).
void __fastcall City_TransferCargo(uint8_t* c, int, uint8_t* dst, int32_t i, int32_t amount) {
    float stock = F<float>(c, 0x103 + i * 4);
    float x = (float)((double)stock * *(const double*)0x5c9c50);
    if (c[0xdf + (i >> 3)] & (1 << (i & 7))) {
        if ((double)stock - x < *(const double*)0x5c9c48 && Game_EffectiveVersion() >= 0x67)
            x = (float)((double)x * *(const double*)0x5c8758);
    }
    float cap = (float)((double)F<uint16_t>(c, 0x6a + i * 2) / *(const double*)0x5c9c40);
    if (cap > F<float>(c, 0x103 + i * 4)) x = (float)((double)x * *(const double*)0x5c88e8);
    F<float>(c, 0x103 + i * 4) = (float)((double)F<float>(c, 0x103 + i * 4) - x);
    F<float>(dst, 0x103 + i * 4) = (float)((double)x + F<float>(dst, 0x103 + i * 4));
    uint16_t n = F<uint16_t>(dst, i * 2);
    F<uint16_t>(c, i * 2) = (int32_t)n > amount ? (uint16_t)(n - amount) : 0;
}
RT3_REPLACE(0x42bf80, City_TransferCargo);

// 0x5084a0. Rounded value for building `id` (0x62b268, +0x78c set; v1.03+ only):
// above 0x5c8d10 in steps of 100000, else of 50000.
float __fastcall Building_RoundedValue(uint32_t id, int, float a) {
    if (Game_EffectiveVersion() < 0x67) return *(const float*)0x5c8598;
    uint8_t* b = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b268), 0, id);
    if (!F<int32_t>(b, 0x78c)) return *(const float*)0x5c8598;
    float v = (float)((double)a * *(const double*)0x5d08a0);
    int32_t n;
    if (a > *(const float*)0x5c8d10) {
        __asm {
            fld v
            fadd dword ptr ds:[0x5c8db0]
            fdiv dword ptr ds:[0x5c8d00]
            mov eax, 0x5a10d0
            call eax
            mov n, eax
        }
        return (float)(n * 100000);
    }
    __asm {
        fld v
        fadd dword ptr ds:[0x5d1808]
        fdiv dword ptr ds:[0x5c8db0]
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    return (float)(n * 50000);
}
RT3_REPLACE(0x5084a0, Building_RoundedValue);

// 0x5674c0. Frees every model of the set (+0, 0x18 bytes, count +4) and the array.
void __fastcall ModelSet_Free(uint8_t* s) {
    for (int32_t i = 0; i < F<uint16_t>(s, 4); ++i) Model_FreeFrames(F<uint8_t*>(s, 0) + i * 0x18);
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(s, 0));
}
RT3_REPLACE(0x5674c0, ModelSet_Free);

// 0x567d50. Frees the item lists +0x20 / +0x1c and the lists themselves.
void __fastcall Lists_Destroy(uint8_t* o) {
    Lists_FreeItems(o);
    if (PtrList* l = F<PtrList*>(o, 0x20)) {
        PtrList_Destroy(l);
        ExeDelete(l);
    }
    PtrList* l2 = F<PtrList*>(o, 0x1c);
    F<PtrList*>(o, 0x20) = nullptr;
    if (l2) {
        PtrList_Destroy(l2);
        ExeDelete(l2);
    }
    F<PtrList*>(o, 0x1c) = nullptr;
}
RT3_REPLACE(0x567d50, Lists_Destroy);

// 3D providers: 0xd97bc0, 16-byte {handle, name, enabled, -}, count 0xd97dc0 (max 32).
static inline uint8_t* Provider(int32_t i) { return (uint8_t*)0xd97bc0 + i * 16; }

// 0x569400. Enumerates the Miles 3D providers, disabling the blacklisted ones
// (names containing 0x5de458 / 0x5de450 / 0x5de44c).
void __fastcall Sound_EnumProviders() {
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
uint8_t* __stdcall Sound_FindProvider(const char* name) {
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
double __stdcall Sound_ListenerDistSq(float x, float y, float z) {
    const float zero = *(const float*)0x5c8598;
    if (zero == x || zero == y || zero == z) return zero;
    Sound_TransformPoint(hook::Global<uint8_t*>(0x6d402c), 0, &x, &y, &z);
    return (double)x * x + (double)y * y + (double)z * z;
}
RT3_REPLACE(0x5695e0, Sound_ListenerDistSq);

static inline uint32_t Listener(uint8_t* s) {
    if (!F<uint32_t>(s, 0x2c)) F<uint32_t>(s, 0x2c) = Import<uint32_t(__stdcall*)(uint32_t)>(0x5c8460)(*F<uint32_t*>(s, 0x28));
    return F<uint32_t>(s, 0x2c);
}
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

// 0x569750. Takes a voice record from the pool (+8; recycled or new), stamps it
// with a fresh serial (0x625c80, never 0) and returns its handle (+4).
uint32_t __fastcall Voices_Acquire(uint8_t* v) {
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
void __fastcall Voices_Release(uint8_t* v, int, uint32_t key) {
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

// 0x569a80. Stops every sample (visitor 0x5318c0 over +0x14) and resets the table.
void __fastcall Sound_StopAll(uint8_t* s) {
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

} // namespace rt3
