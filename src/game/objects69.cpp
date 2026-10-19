// Camera helpers in blocks 0x43..0x52 (batch 72). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);

static inline double Fsqrt(double v) {
    double r;
    __asm {
        fld v
        fsqrt
        fstp r
    }
    return r;
}
static inline double Fpatan(double y, double x) {  // x87 FPATAN: atan2(y, x)
    double r;
    __asm {
        fld y
        fld x
        fpatan
        fstp r
    }
    return r;
}

// 0x43a0a0. Switches the camera between full and 70% height bounds;
// engine +0x114216 = !full; refreshes the main camera (virtual slot 0) and flags it.
void __fastcall Camera_SetFullView(uint8_t* c, int, int32_t full) {
    F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114216) = full == 0;
    Camera_SetBounds(c, 0, 0, 0, 0x3f800000, full ? 0x3f800000 : 0x3f333333);
    void* cam = hook::Global<void*>(0x62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
    hook::Global<uint8_t*>(0x62be68)[0x65] = 1;
}
RT3_REPLACE(0x43a0a0, Camera_SetFullView);

// 0x52d180. Sets the eye position (+4..+0xc), flagging a change when it moved
// more than 1472 units from the snapshot (+0x38..+0x40) on any axis.
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z) {
    const double lim = *(const double*)0x5dd1c0;
    auto moved = [&](uint32_t snap, uint32_t cur) {
        double d = (double)F<float>(c, snap) - F<float>(c, cur);
        return (d < 0 ? -d : d) > lim;
    };
    if (moved(0x38, 4) || moved(0x3c, 8) || moved(0x40, 0xc)) c[0x64] = 1;
    F<uint32_t>(c, 4) = x;
    F<uint32_t>(c, 8) = y;
    F<uint32_t>(c, 0xc) = z;
}
RT3_REPLACE(0x52d180, Camera_SetEye);

// 0x52d230. Yaw, pitch and distance from the eye (+4) to the target (+0x1c); outputs may be null.
void __fastcall Camera_Angles(uint8_t* c, int, float* yaw, float* pitch, float* dist) {
    double dx = (double)F<float>(c, 0x1c) - F<float>(c, 4);
    double dy = (double)F<float>(c, 0x20) - F<float>(c, 8);
    double dz = (double)F<float>(c, 0x24) - F<float>(c, 0xc);
    const double q = *(const double*)0x5c8748;
    if (dist) {
        double s = dz * dz + dy * dy;
        *dist = (float)Fsqrt(s + dx * dx);
    }
    if (yaw) *yaw = (float)(q - Fpatan(dz, dx));
    if (pitch) {
        double h = dz * dz + dx * dx;
        *pitch = (float)(q - Fpatan(Fsqrt(h), dy));
    }
}
RT3_REPLACE(0x52d230, Camera_Angles);

// 0x52d2b0. Camera save: eye, the +0x18/+0x10/+0x14 triple, target, 20 zero bytes.
void __fastcall Camera_Save(uint8_t* c, int, Stream s) {
    static const uint32_t kFields[] = { 4, 8, 0xc, 0x18, 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (uint32_t off : kFields) StreamWrite(s, c + off, 4, 0);
    uint8_t pad[0x14] = {};
    StreamWrite(s, pad, 0x14, 0);
}
RT3_REPLACE(0x52d2b0, Camera_Save);

// 0x52d3e0 / 0x52d570. Detach from the engine's active camera slot (+0x28); the second is the destructor.
void __fastcall Camera_Detach(void* c) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (e && F<void*>(e, 0x28) == c) F<void*>(e, 0x28) = nullptr;
}
RT3_REPLACE(0x52d3e0, Camera_Detach);
void __fastcall Camera_Destruct(uint32_t* c) {
    c[0] = 0x5dd1c8;
    Camera_Detach(c);
}
RT3_REPLACE(0x52d570, Camera_Destruct);

// 0x52d950. Appends to the listener list +0x79 (created on first use).
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item) {
    if (!F<PtrList*>(c, 0x79)) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(c, 0x79) = l ? PtrList_Init(l) : nullptr;
    }
    return PtrList_Append(F<PtrList*>(c, 0x79), 0, item);
}
RT3_REPLACE(0x52d950, Camera_AddListener);

} // namespace rt3
