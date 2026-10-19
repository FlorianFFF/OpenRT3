// Functions across blocks 0x41..0x4d (batch 61). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t Global6d07b4Set2();
void* __fastcall Node_Company(void* n);
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add);
void __fastcall World_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
void* __fastcall Array33_At(void* a, int, int32_t i);
void* __fastcall Engine_Renderer(void* e);

// 0x41f910 / 0x461500 / 0x474430. Visibility tests against the 0x6d07b4 mode.
int32_t __fastcall Obj_Visible41f910(uint8_t* o) {
    if (!Global6d07b4Set2()) return 1;
    return F<int32_t>(o, 0x23e) ? 1 : o[0x42];
}
RT3_REPLACE(0x41f910, Obj_Visible41f910);
int32_t __fastcall Obj_Visible461500(uint8_t* o) {
    if (!Global6d07b4Set2() && !o[0x23e]) return 1;
    return o[0x42];
}
RT3_REPLACE(0x461500, Obj_Visible461500);
int32_t __fastcall Obj_Visible474430(uint8_t* o) { return Global6d07b4Set2() ? o[0x42] : 1; }
RT3_REPLACE(0x474430, Obj_Visible474430);

// 0x493660. Adds (or removes) a track piece to this object's counters, its
// company's (0x426c20) and its region's (0x487960).
void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    uint8_t kind = t[0x218];
    if (kind == 1) F<int32_t>(o, 0xbc) += d;
    else if (kind == 2) F<int32_t>(o, 0xc4) += d;
    else F<int32_t>(o, 0xc0) += d;
    int32_t cls = F<int32_t>(t, 0x226);
    if (cls == 3) F<int32_t>(o, 0xcc) += d;
    else if (cls == 1) F<int32_t>(o, 0xd0) += d;
    else if (((int32_t(__thiscall*)(void*))(*(void***)t)[0x14 / 4])(t)) F<int32_t>(o, 0xd4) += d;
    if (t[0x44]) F<int32_t>(o, 0xc8) += d;
    if (Node_Company(t)) Company_CountTrack((uint8_t*)Node_Company(t), 0, t, add);
    Collection* regions = hook::Global<Collection*>(0x6cfc9c);
    if (!regions) return;
    int32_t y = (int32_t)Obj_CenterCellY(t);
    int32_t x = (int32_t)Obj_CenterCellX(t);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    uint32_t r = F<uint8_t*>(map, 0x2131)[F<int32_t>(map, 0x2155) * y + x] >> 3;
    if (!Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, r)) return;
    World_CountTrack((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, r), 0, t, add);
}
RT3_REPLACE(0x493660, Obj_CountTrack);

// 0x4a1440. Whether an unjunctioned piece sits on plain track: walking up to
// 4 pieces each way, no junction and no special piece within 3 steps.
static int WalkClear(uint8_t* start, uint8_t* prev, bool via206) {
    uint8_t* cur = start;
    for (int32_t i = 0; cur && i < 4; ++i) {
        if (F<int32_t>(cur, 0x20e) != -1) return 0;
        int32_t cls = F<int32_t>(cur, 0x226);
        bool special = cls == 1 || ((int32_t(__thiscall*)(void*))(*(void***)cur)[0x14 / 4])(cur) || cls == 3;
        if (special && i < 3) return 0;
        uint8_t* next;
        if (via206) next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        else next = (uint8_t*)Obj_Node20a(cur) == prev ? (uint8_t*)Obj_Node206(cur) : (uint8_t*)Obj_Node20a(cur);
        prev = cur;
        cur = next;
    }
    return 1;
}
int32_t __stdcall Track_OnPlainLine(uint8_t* t) {
    if (F<int32_t>(t, 0x20e) != -1) return 0;
    if (!Obj_Node206(t) && !Obj_Node20a(t)) return 0;
    if (!WalkClear((uint8_t*)Obj_Node206(t), t, true)) return 0;
    return WalkClear((uint8_t*)Obj_Node20a(t), t, false);
}
RT3_REPLACE(0x4a1440, Track_OnPlainLine);

// 0x4b9ec0. Copies the edited order's settings (0x6cfe04) into every order of the list 0x6cfe08.
void OrderEdit_ApplyToAll() {
    for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0x6cfe08), 4); ++i) {
        uint8_t* e = (uint8_t*)Array33_At(hook::Global<void*>(0x6cfe08), 0, i);
        auto src = []() { return hook::Global<uint8_t*>(0x6cfe04); };
        F<uint32_t>(e, 0x24) = F<uint32_t>(src(), 0x24);
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x40);
        for (int k = 0; k < 8; ++k) F<uint32_t>(e, 1 + k * 4) = F<uint32_t>(src(), 1 + k * 4);
        e[0x23] = src()[0x23];
        e[0x22] = src()[0x22];
        e[0x21] = src()[0x21];
        e[0] = src()[0];
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x20);
        e[0x28] ^= (uint8_t)((src()[0x28] ^ e[0x28]) & 0x10);
    }
}
RT3_REPLACE(0x4b9ec0, OrderEdit_ApplyToAll);

int32_t Global6d0800Set() { return hook::Global<void*>(0x6d0800) ? 1 : 0; }
RT3_REPLACE(0x4dc7d0, Global6d0800Set);
void SetGlobal6d07fc() { hook::Global<int32_t>(0x6d07fc) = 1; }
RT3_REPLACE(0x4dc7e0, SetGlobal6d07fc);
int32_t Global6d0810Set() { return hook::Global<void*>(0x6d0810) ? 1 : 0; }
RT3_REPLACE(0x4dcd40, Global6d0810Set);

// 0x4dd910. Map cell of the quarter-resolution coordinates (x/4, y/4).
uint8_t* __fastcall Map_CellAtQuarter(uint8_t* m, int, int32_t x, int32_t y) {
    return F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * (y / 4) + x / 4) * 0x1d7;
}
RT3_REPLACE(0x4dd910, Map_CellAtQuarter);

// 0x4de0c0. Whether the screen (+0x8c) may close: map-type screens close
// at once; others first let the renderer's active overlay (+0x366e) finish.
int32_t __fastcall Screen_CanClose(uint8_t* s, int, int32_t) {
    uint32_t i = (uint32_t)(F<int32_t>(s, 0x8c) - 6);
    if (i <= 0x14 && !(i >= 1 && i <= 5) && i != 18 && i != 19) return 1;
    uint8_t* r = (uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024));
    void* p = F<void*>(r, 0x366e);
    if (p && ((int32_t(__thiscall*)(void*))(*(void***)p)[0x1c / 4])(p)) {
        ((void(__thiscall*)(void*))(*(void***)p)[0x5c / 4])(p);
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4de0c0, Screen_CanClose);

} // namespace rt3
