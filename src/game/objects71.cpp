// Camera / model helpers in blocks 0x45..0x52 (batch 74). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);

static inline double Fsin(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }
static inline double Fcos(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// 0x458230. Drops the object's private camera (+0x352), handing the engine back the main camera.
void __fastcall Obj_DropCamera(uint8_t* o) {
    if (!F<void*>(o, 0x352)) return;
    F<void*>(hook::Global<uint8_t*>(0x6d4024), 0x28) = hook::Global<void*>(0x62be68);
    if (uint32_t* c = F<uint32_t*>(o, 0x352)) {
        Camera_Destruct(c);
        ((void(__cdecl*)(void*))0x53b080)(c);
    }
    F<void*>(o, 0x352) = nullptr;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0xa0) = 0;
    F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114216) = 0;
}
RT3_REPLACE(0x458230, Obj_DropCamera);

// 0x52d400. Places the eye at distance +0x18 from the target (+0x1c) along yaw +0x10 / pitch +0x14.
void __fastcall Camera_UpdateEye(uint8_t* c) {
    double cp = Fcos(F<float>(c, 0x14));
    float r = F<float>(c, 0x18);
    float z = (float)((double)F<float>(c, 0x24) - Fcos(F<float>(c, 0x10)) * r * cp);
    float y = (float)((double)F<float>(c, 0x20) - Fsin(F<float>(c, 0x14)) * r);
    float x = (float)((double)F<float>(c, 0x1c) - Fsin(F<float>(c, 0x10)) * r * cp);
    Camera_SetEye(c, 0, Bits(x), Bits(y), Bits(z));
}
RT3_REPLACE(0x52d400, Camera_UpdateEye);

// 0x52d9a0. Adds an attachment at this object's transform origin (+0x14) to the list +0x7d.
PtrListNode* __fastcall SceneObj_AddAttachment(uint8_t* o, int, uint8_t* item) {
    if (!F<PtrList*>(o, 0x7d)) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(o, 0x7d) = l ? PtrList_Init(l) : nullptr;
    }
    for (int k = 0; k < 3; ++k) F<uint32_t>(item, 0x29 + k * 4) = F<uint32_t*>(o, 0x14)[k];
    return PtrList_Append(F<PtrList*>(o, 0x7d), 0, item);
}
RT3_REPLACE(0x52d9a0, SceneObj_AddAttachment);

// 0x52daa0. Selects animation frame f (+0x21, frame pointer +0x18); clamps to the
// last frame and returns 0 past the end.
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f) {
    if (!F<int32_t>(m, 0x10)) return 0;
    uint8_t* anim = F<uint8_t*>(m, 0x14);
    int32_t n = F<int32_t>(anim, 0x14);
    if (f >= n) {
        F<int32_t>(m, 0x21) = n - 1;
        F<uint8_t*>(m, 0x18) = F<uint8_t*>(anim, 0xc) + (n - 1) * 0x45;
        return 0;
    }
    F<int32_t>(m, 0x21) = f;
    F<uint8_t*>(m, 0x18) = F<uint8_t*>(anim, 0xc) + f * 0x45;
    return 1;
}
RT3_REPLACE(0x52daa0, Model_SetFrame);

// 0x52db10. Prepares the current animation (+0x3a) for colour tinting (+0x69):
// each frame gets a private vertex copy (+0x35: positions plus one byte per vertex).
void __fastcall Model_PrepareTint(uint8_t* m, int, int32_t tint) {
    F<int32_t>(m, 0x69) = tint;
    m[0x8d] = 1;
    m[0x20] |= 2;
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    hook::Global<uint8_t*>(0xcc3b6c) = anim;
    for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0xcc3b6c), 0x14); ++i) {
        uint8_t* fr = F<uint8_t*>(hook::Global<uint8_t*>(0xcc3b6c), 0xc) + i * 0x45;
        hook::Global<uint8_t*>(0xcc3b58) = fr;
        if (F<void*>(fr, 0x35)) continue;
        int32_t n = F<int32_t>(fr, 8);
        hook::Global<int32_t>(0xcc3b60) = n;
        uint8_t* buf = (uint8_t*)Malloc(n + n * 12);
        F<uint8_t*>(hook::Global<uint8_t*>(0xcc3b58), 0x35) = buf;
        const uint8_t* src = F<uint8_t*>(hook::Global<uint8_t*>(0xcc3b58), 0);
        for (int32_t k = 0; k < n * 12; ++k) buf[k] = src[k];
        for (int32_t k = 0; k < n; ++k) buf[n * 12 + k] = 0;
    }
}
RT3_REPLACE(0x52db10, Model_PrepareTint);

void __fastcall Nop52e670(void*, int, int32_t) {}
RT3_REPLACE(0x52e670, Nop52e670);
uint32_t __fastcall Obj_Get1c(uint8_t* o) { return F<uint32_t>(o, 0x1c); }
RT3_REPLACE(0x52e710, Obj_Get1c);
RT3_NOINLINE void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1e2);
    *y = F<uint32_t>(o, 0x1e6);
    *z = F<uint32_t>(o, 0x1ea);
}
RT3_REPLACE(0x52e720, SceneObj_GetPos);

// 0x52e480. Emits the current frame's vertices as 32-byte {xyz, 0, 1, 0, 0, 0}
// records into out and adds their count to *count.
void __fastcall Model_EmitVertices(uint8_t* m, int, uint8_t* out, int32_t* count) {
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    hook::Global<uint8_t*>(0xcc3b6c) = anim;
    uint8_t* fr = F<uint8_t*>(anim, 0xc) + F<int32_t>(m, 0x21) * 0x45;
    hook::Global<uint8_t*>(0xcc3b58) = fr;
    hook::Global<uint8_t*>(0xcc3b68) = F<uint8_t*>(fr, 0);
    hook::Global<int32_t>(0xcc3b60) = F<int32_t>(fr, 8);
    *count += hook::Global<int32_t>(0xcc3b60);
    while (hook::Global<int32_t>(0xcc3b60)) {
        const uint32_t* src = hook::Global<uint32_t*>(0xcc3b68);
        F<uint32_t>(out, 0) = src[0];
        F<uint32_t>(out, 4) = src[1];
        F<uint32_t>(out, 8) = src[2];
        F<uint32_t>(out, 0x1c) = 0;
        F<uint32_t>(out, 0x18) = 0;
        F<uint32_t>(out, 0xc) = 0;
        F<uint32_t>(out, 0x10) = 0x3f800000;
        F<uint32_t>(out, 0x14) = 0;
        hook::Global<uint8_t*>(0xcc3b68) += 0xc;
        out += 0x20;
        hook::Global<int32_t>(0xcc3b60)--;
    }
}
RT3_REPLACE(0x52e480, Model_EmitVertices);

} // namespace rt3
