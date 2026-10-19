// Engine / scene object / stream helpers in blocks 0x48..0x53 (batch 80). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Station_RecordNearby(uint8_t* s, int, uint8_t* obj);
void __fastcall Tile_UpdateBlocked(uint8_t* o);
void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx);
int32_t __fastcall Engine_ResetState_Impl(uint8_t* e);
void* __fastcall Engine_Renderer(void* e);

// COM vtable call helpers (D3D8 interfaces, stdcall with `this` first).
static inline void** Vt(void* p) { return *(void***)p; }
template <typename Fn> static inline Fn Slot(void* p, uint32_t off) { return (Fn)Vt(p)[off / 4]; }

// 0x481480. Station_RecordNearby(obj) for every element of this collection.
void __fastcall Collection_RecordNearby(Collection* c, int, uint8_t* obj) {
    for (int32_t j = 0; j < Collection_Count(c); ++j)
        Station_RecordNearby((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0)), 0, obj);
}
RT3_REPLACE(0x481480, Collection_RecordNearby);

// 0x48f290. Whether the tile is blocked (computed on first use).
uint32_t __fastcall Tile_IsBlocked(uint8_t* o) {
    if (!(o[0x24c] & 1)) Tile_UpdateBlocked(o);
    return (o[0x24c] >> 1) & 1;
}
RT3_REPLACE(0x48f290, Tile_IsBlocked);

// 0x4f5dd0. Sets the map style (+0x8c) and reapplies the water preset.
void __fastcall Map_SetStyle(uint8_t* o, int, int32_t style) {
    F<int32_t>(o, 0x8c) = style;
    Map_SetWaterPreset(o, 0, F<int32_t>(o, 0x98));
}
RT3_REPLACE(0x4f5dd0, Map_SetStyle);

// 0x51faf0. Copies the front buffer (via a scratch A8R8G8B8 texture of the current
// display mode's size) into dst at (x, y) with a row stride in pixels. The source
// rows are advanced by width*4, not by the locked pitch.
uint8_t __fastcall Engine_CaptureFrontBuffer(uint8_t* e, int, uint8_t* dst, int32_t x, int32_t y, int32_t stride) {
    if (!e[0x114226]) return 0;
    uint8_t* mode = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    int32_t w = F<int32_t>(mode, 0), h = F<int32_t>(mode, 4);
    void* dev = F<void*>(e, 8);
    void* tex;
    void* surf;
    Slot<int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void**)>(dev, 0x50)(
        dev, w, h, 1, 0, 0x15, 3, &tex);
    Slot<int32_t(__stdcall*)(void*, int32_t, void**)>(tex, 0x3c)(tex, 0, &surf);
    void* d = F<void*>(e, 8);
    if (Slot<int32_t(__stdcall*)(void*, void*)>(d, 0x78)(d, surf)) {
        Slot<uint32_t(__stdcall*)(void*)>(surf, 8)(surf);
        Slot<uint32_t(__stdcall*)(void*)>(tex, 8)(tex);
        return 0;
    }
    struct { int32_t pitch; uint8_t* bits; } lr;
    Slot<int32_t(__stdcall*)(void*, void*, void*, uint32_t)>(surf, 0x24)(surf, &lr, nullptr, 0);
    const uint8_t* src = lr.bits;
    uint8_t* out = dst + (y * stride + x) * 4;
    if (h > 0) {
        uint32_t row = w * 4;
        for (int32_t r = h; r; --r) {
            for (uint32_t k = 0; k < row; ++k) out[k] = src[k];
            out += stride * 4;
            src += row;
        }
    }
    Slot<int32_t(__stdcall*)(void*)>(surf, 0x28)(surf);
    Slot<uint32_t(__stdcall*)(void*)>(surf, 8)(surf);
    Slot<uint32_t(__stdcall*)(void*)>(tex, 8)(tex);
    return 1;
}
RT3_REPLACE(0x51faf0, Engine_CaptureFrontBuffer);

// 0x51ff90. Engine constructor: registers itself as the global engine.
uint8_t* __fastcall Engine_Construct(uint8_t* e) {
    F<int32_t>(e, 0x114201) = 0;
    hook::Global<uint8_t*>(0x6d4024) = e;
    Engine_ResetState_Impl(e);
    return e;
}
RT3_REPLACE(0x51ff90, Engine_Construct);

// 0x52e380. Emits animation 0's frame (clamped) as a triangle list of 24-byte
// vertices {xyz + offset, colour, uv} from the tinted vertex copy (+0x35) and
// adds the vertex count to *count.
void __fastcall Model_EmitTriangles(uint8_t* m, int, uint8_t* out, int32_t* count, uint32_t colour,
                                    int32_t frame, float dx, float dy, float dz) {
    uint8_t* anim = F<uint8_t*>(m, 0x14);
    int32_t last = F<int32_t>(anim, 0x14) - 1;
    if (frame > last) frame = last;
    uint8_t* fr = F<uint8_t*>(anim, 0xc) + frame * 0x45;
    uint8_t* tri = F<uint8_t*>(fr, 4);
    int32_t delta = (int32_t)(F<uint8_t*>(fr, 0x35) - F<uint8_t*>(fr, 0));
    int32_t n = F<int32_t>(fr, 0xc);
    *count += n * 3;
    do {
        const float* v0 = (const float*)(F<uint8_t*>(tri, 0) + delta);
        const float* v1 = (const float*)(F<uint8_t*>(tri, 4) + delta);
        const float* v2 = (const float*)(F<uint8_t*>(tri, 8) + delta);
        --n;
        const float* p[3] = { v0, v1, v2 };
        for (int k = 0; k < 3; ++k) {
            uint8_t* o = out + k * 0x18;
            F<float>(o, 0) = (float)((double)dx + p[k][0]);
            F<float>(o, 4) = (float)((double)dy + p[k][1]);
            F<float>(o, 8) = (float)((double)dz + p[k][2]);
            F<uint32_t>(o, 0xc) = colour;
            F<uint32_t>(o, 0x10) = F<uint32_t>(tri, 0x30 + k * 8);
            F<uint32_t>(o, 0x14) = F<uint32_t>(tri, 0x34 + k * 8);
        }
        tri += 0x4c;
        out += 0x48;
    } while (n);
}
RT3_REPLACE(0x52e380, Model_EmitTriangles);

// 0x52ecd0. Resets a scene object: identity transform/quaternion, scale 1, no parts.
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o) {
    o[0x20] &= 0x77;
    o[0x8d] = 0;
    o[0x5c] = 0;
    F<int32_t>(o, 0x5d) = 0;
    F<int32_t>(o, 0x61) = 0;
    F<int32_t>(o, 0x1ee) = -1;
    F<int32_t>(o, 0x1fa) = -1;
    F<int32_t>(o, 0x3e) = -1;
    uint8_t fl = o[0x20];
    F<uint32_t>(o, 0x47) = 0xff000000;
    static const uint16_t zero[] = { 0x8, 0xc, 0x75, 0x79, 0x7d, 0x81, 0x4, 0x10, 0x1c, 0x14 };
    for (uint16_t off : zero) F<int32_t>(o, off) = 0;
    o[0x5b] = 0;
    F<int32_t>(o, 0x85) = 0;
    F<int32_t>(o, 0x89) = 0;
    o[0x25] = 0;
    o[0x42] = 0;
    o[0x20] = (uint8_t)((fl & 0xee) | 0x62);
    F<int32_t>(o, 0x4b) = 0;
    F<int32_t>(o, 0x4f) = 0;
    F<int32_t>(o, 0x53) = 0;
    for (int k = 0; k < 16; ++k) F<uint32_t>(o, 0x18e + k * 4) = (k % 5 == 0) ? 0x3f800000 : 0;
    F<int32_t>(o, 0x1d6) = 0;
    F<int32_t>(o, 0x1d2) = 0;
    F<int32_t>(o, 0x1ce) = 0;
    F<uint32_t>(o, 0x1da) = 0x3f800000;
    F<uint32_t>(o, 0x36) = 0x3f800000;
    F<int32_t>(o, 0x3a) = 0;
    F<int32_t>(o, 0x21) = 0;
    F<int32_t>(o, 0x65) = 0;
    return 0;
}
// 0x53067b keeps using ECX (the object) after the call.
__declspec(naked) int32_t SceneObj_Reset() { RT3_KEEP_ECX_EDX(SceneObj_Reset_Impl, 0) }
RT3_REPLACE(0x52ecd0, SceneObj_Reset);

// 0x52f120. Rebuilds the world matrix +0x14e when dirty (flag 2): scale +0x36 and
// translation, then rotation by yaw/pitch/roll (+0x4f/+0x4b/+0x53) or, for
// quaternion-driven objects (flag 0x80), by the local matrix +0x18e (D3DX).
void __fastcall SceneObj_UpdateMatrix(uint8_t* o) {
    if (!(o[0x20] & 2)) return;
    o[0x20] = (uint8_t)((o[0x20] & 0xfd) | 4);
    float* m = (float*)(o + 0x14e);
    for (int k = 0; k < 16; ++k) m[k] = 0.0f;
    float s = F<float>(o, 0x36);
    m[10] = s;
    m[5] = s;
    m[0] = s;
    F<uint32_t>(o, 0x17e) = F<uint32_t>(o, 0x1e2);
    F<uint32_t>(o, 0x186) = F<uint32_t>(o, 0x1ea);
    F<uint32_t>(o, 0x18a) = 0x3f800000;
    F<uint32_t>(o, 0x182) = F<uint32_t>(o, 0x1e6);
    float rot[16], res[16];
    auto mul = (void*(__stdcall*)(float*, const void*, const float*))0x56d3cd;
    if (!(o[0x20] & 0x80)) {
        ((void*(__stdcall*)(float*, float, float, float))0x56e1a8)(rot, F<float>(o, 0x4f), F<float>(o, 0x4b),
                                                                   F<float>(o, 0x53));
        mul(res, rot, m);
    } else {
        mul(res, o + 0x18e, m);
    }
    for (int k = 0; k < 16; ++k) m[k] = res[k];
}
RT3_REPLACE(0x52f120, SceneObj_UpdateMatrix);

// 0x5305c0. Whether the object was drawn within the last two renderer frames (+0x65).
int32_t __fastcall SceneObj_RecentlyDrawn(uint8_t* o) {
    int32_t d = F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 5) - F<int32_t>(o, 0x65);
    return d > 2 || d < 0 ? 0 : 1;
}
RT3_REPLACE(0x5305c0, SceneObj_RecentlyDrawn);

// 0x5305f0. Sets or clears flag 1 (+0xb0) on every attachment (+0x79).
void __fastcall SceneObj_SetAttachmentsFlag(uint8_t* o, int, int32_t on) {
    if (!F<PtrList*>(o, 0x79)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x79));
    for (uint8_t* a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)); a;
         a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79))) {
        if (on) F<uint32_t>(a, 0xb0) |= 1;
        else F<uint32_t>(a, 0xb0) &= ~1u;
    }
}
RT3_REPLACE(0x5305f0, SceneObj_SetAttachmentsFlag);

// 0x531410. Writes *str with its length (incl. NUL) as a 1-byte prefix, or 2 bytes
// (high byte | 0x80 first) past 0x7f; a null string is a single 0 byte.
void __fastcall StreamWriteLenString(Stream s, const char* const* str) {
    const char* p = *str;
    uint8_t b;
    if (!p) {
        b = 0;
        StreamWrite(s, &b, 1, 0);
        return;
    }
    int32_t len = 0;
    while (p[len]) ++len;
    ++len;
    if (len > 0x7f) {
        b = (uint8_t)((len >> 8) | 0x80);
        StreamWrite(s, &b, 1, 0);
    }
    b = (uint8_t)len;
    StreamWrite(s, &b, 1, 0);
    StreamWrite(s, *str, len, 0);
}
RT3_REPLACE(0x531410, StreamWriteLenString);

// 0x531490. Writes str without its NUL.
void __fastcall StreamWriteText(Stream s, const char* str) {
    uint32_t len = 0;
    while (str[len]) ++len;
    StreamWrite(s, str, len, 0);
}
RT3_REPLACE(0x531490, StreamWriteText);

} // namespace rt3
