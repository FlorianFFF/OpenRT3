// Functions across blocks 0x45..0x49 (batch 43). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static Collection*& g_companies  = hook::Global<Collection*>(0x62be10);

void __fastcall BlockIter_SetFlags(uint8_t* o, uint32_t mask, int32_t mode);
int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data);
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add);
double __fastcall Obj_Float18(void* o);

// 0x45b430. Block flag update (0x45a2e0) on +0x256 when +0x75 is set.
uint32_t __fastcall Obj_SetBlockFlags(uint8_t* o, int, uint32_t mask, int32_t mode) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return (uint32_t)o;
    BlockIter_SetFlags(blocks, mask, mode);
    return 0;
}
RT3_REPLACE(0x45b430, Obj_SetBlockFlags);

// 0x468a10. Reads the string value `name` under subKey into 0x6adbb8; returns
// it, or def (which is then written when the value is missing).
// QUIRK: if creating the key fails, def is used as the handle.
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def) {
    void* hk = (void*)def;
    uint32_t disposition, type, size = 0x400;
    Import<int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*)>(0x5c8008)(
        F<void*>(o, 4), subKey, 0, nullptr, 0, 0xf003f, nullptr, &hk, &disposition);
    const char* result = def;
    int32_t r = Import<int32_t(__stdcall*)(void*, const char*, void*, void*, void*, void*)>(0x5c8000)(
        hk, name, nullptr, &type, (void*)0x6adbb8, &size);
    if (r == 0 && type == 1) result = (const char*)0x6adbb8;
    Import<int32_t(__stdcall*)(void*)>(0x5c800c)(hk);
    if (r == 2 && def) RegKey_SetString(o, 0, subKey, name, def);
    return result;
}
RT3_REPLACE(0x468a10, RegKey_GetString);

// 0x48bd70. Clears a track piece's class and properties (after virtual slot 6 with 0).
void __fastcall Track_ClearProps(uint8_t* t) {
    ((void(__thiscall*)(void*, int32_t))(*(void***)t)[0x18 / 4])(t, 0);
    t[0x219] = 0xff;
    t[0x251] = 0xff;
    F<int32_t>(t, 0x226) = 0;
    t[0x24c] &= 0xdf;
}
RT3_REPLACE(0x48bd70, Track_ClearProps);

void* __fastcall Track_Node20e(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x20e);
    return id != -1 ? Collection_Get(g_trackNodes, 0, id) : nullptr;
}
RT3_REPLACE(0x48e180, Track_Node20e);

// 0x48e1a0 / 0x48e220. Whether / through which link one of this piece's two
// end nodes leads back to it (+0x202); the node's flag +0x201 bit 1 says which side.
static uint8_t* EndNode(uint8_t* t, int end) {
    int32_t id = end ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206);
    return id != -1 ? (uint8_t*)Collection_Get(g_trackNodes, 0, id) : nullptr;
}
int32_t __fastcall Track_EndLinksBack(uint8_t* t) {
    for (int end = 0; end < 2; ++end) {
        uint8_t* n = EndNode(t, end);
        if (!n || F<int32_t>(n, 0x20e) == -1) continue;
        int32_t self = F<int32_t>(t, 0x202);
        if (F<int32_t>(n, 0x20e) == self) return 1;
        int c = (n[0x201] >> 1) & 1;
        if (c && F<int32_t>(n, 0x206) == self) return 1;
        if (!c && F<int32_t>(n, 0x20a) == self) return 1;
    }
    return 0;
}
RT3_REPLACE(0x48e1a0, Track_EndLinksBack);
int32_t __fastcall Track_EndBackLink(uint8_t* t) {
    for (int end = 0; end < 2; ++end) {
        uint8_t* n = EndNode(t, end);
        if (!n || F<int32_t>(n, 0x20e) == -1) continue;
        int32_t self = F<int32_t>(t, 0x202);
        if (F<int32_t>(n, 0x20e) == self) return (n[0x201] & 2) ? F<int32_t>(n, 0x206) : F<int32_t>(n, 0x20a);
        int c = (n[0x201] >> 1) & 1;
        if (c && F<int32_t>(n, 0x206) == self) return F<int32_t>(n, 0x20e);
        if (!c && F<int32_t>(n, 0x20a) == self) return F<int32_t>(n, 0x20e);
    }
    return -1;
}
RT3_REPLACE(0x48e220, Track_EndBackLink);

// 0x48e340. Changes the owning company (+0x22e) and moves the piece between the companies' track counters.
void __fastcall Track_SetOwner(uint8_t* t, int, uint16_t owner) {
    uint16_t cur = F<uint16_t>(t, 0x22e);
    if (cur == owner) return;
    if (Collection_IsValid(g_companies, 0, cur))
        Company_CountTrack((uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>(t, 0x22e)), 0, t, 0);
    if (Collection_IsValid(g_companies, 0, owner))
        Company_CountTrack((uint8_t*)Collection_Get(g_companies, 0, owner), 0, t, 1);
    F<uint16_t>(t, 0x22e) = owner;
}
RT3_REPLACE(0x48e340, Track_SetOwner);

void __fastcall Obj_DecA4(uint8_t* o) {
    if (--F<int32_t>(o, 0xa4) < 0) F<int32_t>(o, 0xa4) = 0;
}
RT3_REPLACE(0x4918a0, Obj_DecA4);
void __fastcall Obj_DecAc(uint8_t* o) {
    if (--F<int32_t>(o, 0xac) < 0) F<int32_t>(o, 0xac) = 0;
}
RT3_REPLACE(0x4918c0, Obj_DecAc);

// 0x491d50. Copies the cursor fields (+0x118..+0x125) into out and whether the
// selected record (+0x125) has byte +0x216 clear.
void __fastcall Table_FillCursor(Collection* t, int, uint8_t* out) {
    F<uint32_t>(out, 6) = F<uint32_t>(t, 0x118);
    F<uint32_t>(out, 0xa) = F<uint32_t>(t, 0x11d);
    F<uint32_t>(out, 0xe) = F<uint32_t>(t, 0x121);
    F<uint32_t>(out, 0x12) = F<uint32_t>(t, 0x125);
    F<int32_t>(out, 0x2e) = 0;
    if (!Collection_IsValid(t, 0, F<uint32_t>(t, 0x125))) return;
    F<int32_t>(out, 0x2e) = F<uint8_t>(Collection_Get(t, 0, F<uint32_t>(t, 0x125)), 0x216) == 0 ? 1 : 0;
}
RT3_REPLACE(0x491d50, Table_FillCursor);

// 0x492090. Frees the per-column chains +0x8c (map width entries; each node
// +6 next, +2 sub-list with +4 next), then the column array.
void __fastcall Obj_FreeColumns(uint8_t* o) {
    int32_t w = F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155);
    auto free53b = [](void* p) { ((void(__cdecl*)(void*))0x53b080)(p); };
    for (int32_t i = 0; i < w; ++i) {
        for (uint8_t* e = F<uint8_t**>(o, 0x8c)[i]; e; ) {
            uint8_t* next = F<uint8_t*>(e, 6);
            for (uint8_t* s = F<uint8_t*>(e, 2); s; ) {
                uint8_t* sn = F<uint8_t*>(s, 4);
                free53b(s);
                s = sn;
            }
            free53b(e);
            e = next;
        }
        F<uint8_t**>(o, 0x8c)[i] = nullptr;
    }
    Free(F<void*>(o, 0x8c));
    F<void*>(o, 0x8c) = nullptr;
}
RT3_REPLACE(0x492090, Obj_FreeColumns);

// 0x493190. Decays every track piece's wear +0x22a by 0.97.
void __fastcall Table_DecayWear(Collection* t) {
    const float k = *(const float*)0x5cfe14;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        F<float>(r, 0x22a) = (float)((double)F<float>(r, 0x22a) * k);
    }
}
RT3_REPLACE(0x493190, Table_DecayWear);

// 0x4932a0 / 0x493300. Starts (counter +0xa0 / +0xa4) a camera action; the
// first one records whether the camera height is below 2000. Always stores
// the camera value +0x18.
static void CameraBegin(uint8_t* o, uint32_t counter) {
    uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
    if (!F<int32_t>(o, 0xa0) && !F<int32_t>(o, 0xa4))
        F<int32_t>(o, 0xb0) = F<float>(cam, 0x66) < *(const float*)0x5c88e0 ? 1 : 0;
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::Global<void*>(0x62be68));
    F<int32_t>(o, counter)++;
}
void __fastcall Camera_BeginA0(uint8_t* o) { CameraBegin(o, 0xa0); }
RT3_REPLACE(0x4932a0, Camera_BeginA0);
void __fastcall Camera_BeginA4(uint8_t* o) { CameraBegin(o, 0xa4); }
RT3_REPLACE(0x493300, Camera_BeginA4);

} // namespace rt3
