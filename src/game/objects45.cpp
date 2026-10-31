// Functions across blocks 0x49..0x4a (batch 48). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
void __fastcall Camera_BeginA4(uint8_t* o);
void __fastcall Camera_BeginAc(uint8_t* o);
void __fastcall Nop494ef0(void*, int, int32_t);
uint8_t* __fastcall Route_Init(uint8_t* r);
void __fastcall Nop494e80(void*);
int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v);
int32_t __fastcall Route_Add10(uint8_t* r, int, int32_t v);

// 0x494240. Whether the cell rectangle holds only plain track (class 0), or
// also classes 12/13 when `allowSpecial`. QUIRK: the clamp is passed &x0 for
// both x0 and y0, so y0 is never clamped.
int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial) {
    Map_ClampRect(hook::Global<void*>(0x62c120), 0, &x0, &x0, &x1, &y1);
    for (int32_t x = x0; (int32_t)(uint16_t)x <= x1; ++x) {
        for (int32_t y = y0; (int32_t)(uint16_t)y <= y1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(t, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* s = F<uint32_t*>(e, 2); s; s = (uint32_t*)s[1]) {
                int32_t cls = F<int32_t>(Collection_Get((Collection*)t, 0, s[0]), 0x226);
                if (cls == 0) continue;
                if ((cls == 0xc || cls == 0xd) && allowSpecial) continue;
                return 0;
            }
        }
    }
    return 1;
}
RT3_REPLACE(0x494240, Track_RectIsFree);

// 0x495480. Resets a train's movement state.
void __fastcall Train_ResetMotion(uint8_t* t) {
    F<int32_t>(t, 0x118) = 0;
    t[0x11c] = 0;
    F<int32_t>(t, 0x125) = -1;
    F<int32_t>(t, 0x129) = -1;
    F<int32_t>(t, 0x12d) = -1;
    F<int32_t>(t, 0x131) = -1;
    F<int32_t>(t, 0x135) = 0;
    F<uint32_t>(t, 0x169) = 0xbf800000;  // -1.0f
    F<int32_t>(t, 0x1c9) = 0;
    F<int32_t>(t, 0x1cd) = 0;
    F<uint32_t>(t, 0x1d9) = 0xbf800000;
    F<int32_t>(t, 0x1dd) = 0;
    F<uint32_t>(t, 0x1e1) = 0xbf800000;
    F<int32_t>(t, 0x1e5) = 0;
    F<int32_t>(t, 0x1ed) = 0;
    F<int32_t>(t, 0x1f5) = -1;
    F<int32_t>(t, 0x1f9) = -1;
    F<int32_t>(t, 0x1fe) = 0;
    F<int32_t>(t, 0x202) = 0;
    F<int32_t>(t, 0x1d1) = 0;
    F<int32_t>(t, 0xdd) = 0;
    F<int32_t>(t, 0xe1) = 0;
    for (int k = 0; k < 5; ++k) t[0xd8 + k] = 0;
}
RT3_REPLACE(0x495480, Train_ResetMotion);

// 0x495620. Camera actions per the options +0x25b / +0x257.
void __fastcall Obj_CameraByOptions(uint8_t* o) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x25b)) Camera_BeginA4(o);
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x257)) Camera_BeginAc(o);
}
RT3_REPLACE(0x495620, Obj_CameraByOptions);

RT3_NOINLINE int32_t __fastcall Obj_Has98(uint8_t* o) { return F<int32_t>(o, 0x98) ? 1 : 0; }
RT3_REPLACE(0x4a3cf0, Obj_Has98);

// 0x4a3d70. Frees the list +0x98 (next +0xc); +0x88 = -1.
void __fastcall Obj_FreeList98(uint8_t* o) {
    while (uint8_t* n = F<uint8_t*>(o, 0x98)) {
        F<uint8_t*>(o, 0x98) = F<uint8_t*>(n, 0xc);
        Free(n);
    }
    F<int32_t>(o, 0x88) = -1;
}
RT3_REPLACE(0x4a3d70, Obj_FreeList98);

// 0x4a4150 / 0x4a41a0. Routes collection (vtbl 0x5d0040, inline 0x4a-byte records, 100 + 50).
Collection* __fastcall Routes_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5d0040;
    Collection_Init(c, 0, 1, 0x4a, 100, 50, 0, 0, 0);
    F<int32_t>(c, 0x88) = 0;
    F<int32_t>(c, 0x94) = 0;
    F<int32_t>(c, 0x98) = 0;
    F<int32_t>(c, 0x90) = 0;
    F<int32_t>(c, 0x8c) = 1;
    return c;
}
RT3_REPLACE(0x4a4150, Routes_Construct);
void __fastcall Routes_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5d0040;
    Collection_Destruct(c);
}
RT3_REPLACE(0x4a41a0, Routes_Destruct);

// 0x4a4240. Routes save: tag 0x396d, records (with the empty hook 0x494ef0), tag 0x396e.
void __fastcall Routes_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x396d);
    Collection_Save(t, 0, s, recordSize);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((void(__thiscall*)(void*, Stream))0x494ef0)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x396e);
}
RT3_REPLACE(0x4a4240, Routes_Save);

// 0x4a42b0. Adds a fresh route; returns its ID.
uint32_t __fastcall Routes_Add(Collection* t) {
    uint8_t local[0x4c] = {};
    Route_Init(local);
    F<int32_t>(t, 0x8c) = 1;
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x4a42d7)(t, 0, local, 0, 0, 0);
    Route_SetOwner((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id);
    Nop494e80(local);
    return id;
}
RT3_REPLACE(0x4a42b0, Routes_Add);

// 0x4a4340. Adjusts route id's reference count (+0x10); releases it at 0.
void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta) {
    if (!Collection_IsValid(t, 0, id)) return;
    if (Route_Add10((uint8_t*)Collection_Get(t, 0, id), 0, delta) == 0) {
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
}
RT3_REPLACE(0x4a4340, Routes_AddRef);

void __fastcall Nop4a6e50(void*) {}
RT3_REPLACE(0x4a6e50, Nop4a6e50);
int32_t __fastcall Obj_ByteF9(uint8_t* o) { return o[0xf9]; }
RT3_REPLACE(0x4a6e60, Obj_ByteF9);

} // namespace rt3
