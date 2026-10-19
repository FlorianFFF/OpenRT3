// Scene object save / Miles sound helpers in blocks 0x45..0x53 (batch 81). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/containers.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o);
void __fastcall SceneObj_SaveFlags(uint8_t* o, int, Stream s);
void __fastcall SceneObj_SetAttachmentsFlag(uint8_t* o, int, int32_t on);
void __fastcall StreamWriteLenString(Stream s, const char* const* str);

// 0x455610. Resets a named scene object: its three name strings (+0x206..),
// +0x212..+0x22d and +0x22e/+0x232, then the scene object part.
int32_t __fastcall NamedObj_Reset(uint8_t* o) {
    F<int32_t>(o, 0x206) = 0;
    F<int32_t>(o, 0x20a) = 0;
    F<int32_t>(o, 0x20e) = 0;
    F<int32_t>(o, 0x22e) = 0;
    F<int32_t>(o, 0x232) = 0;
    for (int k = 0; k < 7; ++k) F<int32_t>(o, 0x212 + k * 4) = 0;
    return SceneObj_Reset_Impl(o);
}
RT3_REPLACE(0x455610, NamedObj_Reset);

// 0x4559d0. Saves the names (chunk 0x55f1), the object's own data (vtbl +0x4c,
// chunk 0x55f2) and its flags, closing with 0x55f3.
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s) {
    StreamWriteU32(s, 0x55f1);
    StreamWriteLenString(s, (const char* const*)(o + 0x206));
    StreamWriteLenString(s, (const char* const*)(o + 0x20a));
    StreamWriteLenString(s, (const char* const*)(o + 0x20e));
    StreamWriteU32(s, 0x55f2);
    ((void(__thiscall*)(void*, Stream))(*(void***)o)[0x4c / 4])(o, s);
    SceneObj_SaveFlags(o, 0, s);
    StreamWriteU32(s, 0x55f3);
}
RT3_REPLACE(0x4559d0, NamedObj_Save);

// 0x45c840. SceneObj_SetAttachmentsFlag on every model part (+0x316, 0xc0 bytes, count +0x31a).
void __fastcall Model_SetAttachmentsFlag(uint8_t* o, int, int32_t on) {
    for (int32_t i = 0; i < F<int32_t>(o, 0x31a); ++i)
        SceneObj_SetAttachmentsFlag(F<uint8_t*>(F<uint8_t*>(o, 0x316), i * 0xc0 + 0xc), 0, on);
}
RT3_REPLACE(0x45c840, Model_SetAttachmentsFlag);

// 0x530670. Scene object constructor.
uint8_t* __fastcall SceneObj_Construct(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5dd1f0;
    SceneObj_Reset_Impl(o);
    return o;
}
RT3_REPLACE(0x530670, SceneObj_Construct);

// 0x5314b0. The n-th Miles 3D provider (AIL_enumerate_3D_providers), 0 when there is none.
uint32_t __fastcall Sound_Get3DProvider(int32_t n) {
    uint32_t next = 0;
    uint8_t name[4];
    hook::Global<uint32_t>(0xccba08) = 0;
    auto enumerate = Import<int32_t(__stdcall*)(uint32_t*, void*, uint32_t*)>(0x5c8454);
    for (int32_t i = 0; i <= n; ++i)
        if (!enumerate(&next, name, (uint32_t*)0xccba08)) return 0;
    return hook::Global<uint32_t>(0xccba08);
}
RT3_REPLACE(0x5314b0, Sound_Get3DProvider);

// Sound objects: +4 == 1 for 3D sounds; 0xccb9f4 set while 3D sound is unavailable.
static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

uint32_t __fastcall Sound_Is3D(uint8_t* s) {
    if (F<int32_t>(s, 4) != 1) return 0;
    return hook::Global<int32_t>(0xccb9f4) ? 0 : 1;
}
RT3_REPLACE(0x531500, Sound_Is3D);

// 0x5315c0. Transforms a point by the sound's matrix (+8, rows of 4 floats).
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z) {
    double px = *x, py = *y, pz = *z;
    const float* m = (const float*)(s + 8);
    *x = (float)(pz * m[8] + py * m[4] + px * m[0] + m[12]);
    *y = (float)(pz * m[9] + py * m[5] + px * m[1] + m[13]);
    *z = (float)(pz * m[10] + py * m[6] + px * m[2] + m[14]);
}
RT3_REPLACE(0x5315c0, Sound_TransformPoint);

// 0x531880. Stops and releases the sound's Miles timer (+0x4c).
void __fastcall Sound_ReleaseTimer(uint8_t* s) {
    if (F<int32_t>(s, 0x4c) == -1 || hook::Global<int32_t>(0x624d58) != 1) return;
    Import<void(__stdcall*)(int32_t)>(0x5c83b8)(F<int32_t>(s, 0x4c));
    Import<void(__stdcall*)(int32_t)>(0x5c8450)(F<int32_t>(s, 0x4c));
    F<int32_t>(s, 0x4c) = -1;
}
RT3_REPLACE(0x531880, Sound_ReleaseTimer);

// 0x5318b0. Waits until the sound thread clears 0xccb9f0.
void __fastcall Sound_WaitIdle() {
    while (*(volatile int32_t*)0xccb9f0) {}
}
RT3_REPLACE(0x5318b0, Sound_WaitIdle);

// 0x531dc0. Whether the 3D listener is idle (both speeds zero); 1 for 2D sounds.
uint32_t __fastcall Sound_ListenerIdle(uint8_t* s) {
    if (!Is3D(s)) return 1;
    const float zero = *(const float*)0x5c8598;
    if (!(F<float>(hook::Global<uint8_t*>(0xccb9f8), 0x20) == zero)) return 0;
    return F<float>(hook::Global<uint8_t*>(0xccb9fc), 0x58) == zero ? 1 : 0;
}
RT3_REPLACE(0x531dc0, Sound_ListenerIdle);

// 0x531e90. Stores a 4x4 matrix (+8) for a 3D sound and marks the listener dirty.
struct Mat16 { uint32_t v[16]; };
void __fastcall Sound_SetMatrix(uint8_t* s, int, Mat16 m) {
    if (!Is3D(s)) return;
    F<int32_t>(s, 0x48) = 1;
    for (int k = 0; k < 16; ++k) F<uint32_t>(s, 8 + k * 4) = m.v[k];
    F<int32_t>(hook::Global<uint8_t*>(0xccb9fc), 0x60) = 2;
}
RT3_REPLACE(0x531e90, Sound_SetMatrix);

uint32_t __fastcall Sound_Get2c(uint8_t* s) {
    if (!Is3D(s)) return 0;
    return F<uint32_t>(hook::Global<uint8_t*>(0xccb9f8), 0x2c);
}
RT3_REPLACE(0x531ed0, Sound_Get2c);

// 0x531ef0. Looks key up in the 2D (kind 0) or 3D (kind 1) sample table.
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return 0;
    if (kind == 0) return Hash_Get((HashMap*)(hook::Global<uint8_t*>(0xccb9f8) + 0xc), 0, key);
    if (kind == 1) return Hash_Get((HashMap*)(hook::Global<uint8_t*>(0xccb9fc) + 0x14), 0, key);
    return 0;
}
RT3_REPLACE(0x531ef0, Sound_LookupSample);

// 0x531f90. Registers the end-of-sample callback (0x531800 for 2D samples,
// 0x531840 for 3D ones; AIL_register_(3D_)EOS_callback).
int32_t __fastcall Sound_RegisterEos(uint8_t* s, int, uint32_t sample, int32_t kind) {
    if (F<int32_t>(s, 4) != 1) return 0;
    if (int32_t busy = hook::Global<int32_t>(0xccb9f4)) return busy;
    if (kind == 0) return Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(sample, 0x531800);
    if (kind == 1) return Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a4)(sample, 0x531840);
    return kind - 1;
}
RT3_REPLACE(0x531f90, Sound_RegisterEos);

} // namespace rt3
