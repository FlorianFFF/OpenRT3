// Texture rect extract / height-based and neighbour light modes / light ctor / pack name table lookup, open and read in blocks 0x41..0x53 (batch 139). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/containers.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __stdcall Map_PixelsPerTile(float* sx, float* sy);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW,
                                            int32_t* outH);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode);
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x417f20. Scales the map cell rect in (x0, x1, y0, y1) to texture pixels (out)
// and extracts that part of the terrain texture.
uint32_t* __stdcall Map_ExtractTextureRect(const int32_t* in, int32_t* out) {
    float sx, sy;
    Map_PixelsPerTile(&sx, &sy);
    out[0] = (int32_t)((double)(float)in[0] * sx);
    out[1] = (int32_t)((double)(float)in[1] * sx);
    out[2] = (int32_t)((double)(float)in[2] * sy);
    out[3] = (int32_t)((double)(float)in[3] * sy);
    int32_t w, h;
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, &w, &h);
    float hf = (float)h, wf = (float)w;
    float y1 = (float)((double)(float)(out[3] + 1) / hf);
    float x1 = (float)((double)(float)(out[1] + 1) / wf);
    float y0 = (float)((double)(float)out[2] / hf);
    float x0 = (float)((double)(float)out[0] / wf);
    return Terrain_ExtractTexture((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, x0, y0, x1, y1, nullptr,
                                  nullptr);
}
RT3_REPLACE(0x417f20, Map_ExtractTextureRect);

// 0x489f90. Sets the piece's centre (+0x1ed/+0x1f1 in cells) and its light mode: 1
// (raised) when the mean of its corner heights is over 10 above the ground, else 0.
void __fastcall Obj_UpdateHeightLight(uint8_t* o) {
    float ground = (float)Terrain_HeightAt(F<uint8_t*>(o, 4), 0, F<float>(o, 0xc), F<float>(o, 0x10));
    float s = (float)((double)F<float>(o, 0x3c) + F<float>(o, 0x30));
    float avg = (float)(((double)s + F<float>(o, 0x24) + F<float>(o, 0x18)) / D(0x5c87b8));
    F<float>(o, 0x1ed) = (float)((double)s / Fl(0x5c878c));
    F<float>(o, 0x1f1) = (float)(((double)F<float>(o, 0x24) + F<float>(o, 0x18)) / Fl(0x5c878c));
    Light_SetMode(o, 0, (double)avg - ground > D(0x5c85f0) ? 1 : 0);
}
RT3_REPLACE(0x489f90, Obj_UpdateHeightLight);

// 0x48b4b0. Matches the node's light mode (+0x5a) to its lit neighbours (+0x206
// back, +0x20a ahead; flag +0x59 & 0x10, mode not 0xff): mode 2/3 (slopes) when
// they differ, their mode when they agree; neighbours already on a slope win.
void __fastcall Node_UpdateLightFromNeighbors(uint8_t* n) {
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    auto lit = [&](uint32_t off) {
        int32_t id = F<int32_t>(n, off);
        return id != -1 && (((uint8_t*)Collection_Get(nodes, 0, id))[0x59] & 0x10) ? 1 : 0;
    };
    auto mode = [&](uint32_t off) { return ((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(n, off)))[0x5a]; };
    int32_t a = lit(0x206), b = lit(0x20a);
    if (a && mode(0x206) == 0xff) a = 0;
    if (b && mode(0x20a) == 0xff) b = 0;
    if (a) {
        uint8_t ma = mode(0x206);
        if (b) {
            uint8_t mb = mode(0x20a);
            if (ma == 2 || ma == 3 || mb == 2 || mb == 3) return;
            if (ma == mb) {
                if (n[0x5a] != ma) Light_SetMode(n, 0, ma);
                return;
            }
            Light_SetMode(n, 0, ma ? 2 : 3);
            return;
        }
        if (ma == 2 || ma == 3 || ma == n[0x5a]) return;
        Light_SetMode(n, 0, ma ? 2 : 3);
        return;
    }
    if (!b) return;
    uint8_t mb = mode(0x20a);
    if (mb == 2 || mb == 3 || mb == n[0x5a]) return;
    Light_SetMode(n, 0, mb ? 3 : 2);
}
RT3_REPLACE(0x48b4b0, Node_UpdateLightFromNeighbors);

// 0x53aca0. Constructs a light (vtable 0x5dd3d8) with its default colours.
uint8_t* __fastcall Light_Construct(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5dd3d8;
    F<uint32_t>(o, 4) = 0;
    F<uint32_t>(o, 0x5b) = 0;
    o[0x5a] = 0xff;
    F<uint32_t>(o, 0x1e3) = 0;
    F<uint32_t>(o, 0x5f) = 0xff7f7f7f;
    F<uint32_t>(o, 0x1e7) = 0;
    F<uint16_t>(o, 0x1eb) = 0;
    for (int k = 0; k < 16; ++k) F<uint32_t>(o, 0x6f + k * 0x18) = 0xff7f7f7f;
    F<uint32_t>(o, 0x4d) = 0x41c80000;
    F<uint32_t>(o, 0x45) = 0x41c80000;
    F<uint32_t>(o, 0x49) = 0x41000000;
    F<uint32_t>(o, 0x51) = 0x41000000;
    F<uint32_t>(o, 0xeb) = 0x3f266666;
    F<uint32_t>(o, 0x8b) = 0x3f266666;
    o[0x44] = 0;
    static const uint16_t halves[] = { 0xd3, 0x73, 0xbf, 0xa7, 0x8f, 0x77 };
    for (uint16_t off : halves) F<uint32_t>(o, off) = 0x3f000000;
    F<uint32_t>(o, 0x103) = 0x3f59999a;
    F<uint32_t>(o, 0xa3) = 0x3f59999a;
    static const uint16_t ones[] = { 0x11b, 0xbb, 0x11f, 0x107, 0xef, 0xd7 };
    for (uint16_t off : ones) F<uint32_t>(o, off) = 0x3f800000;
    F<uint32_t>(o, 0x1fd) = hook::Global<uint32_t>(0x624dd4);
    F<uint32_t>(o, 0x17b) = hook::Global<uint32_t>(0x624d88);
    F<uint32_t>(o, 0x17f) = hook::Global<uint32_t>(0x624d8c);
    F<uint32_t>(o, 0x163) = hook::Global<uint32_t>(0x624d90);
    F<uint32_t>(o, 0x167) = hook::Global<uint32_t>(0x624d8c);
    F<uint32_t>(o, 0x133) = hook::Global<uint32_t>(0x624d90);
    F<uint32_t>(o, 0x137) = hook::Global<uint32_t>(0x624d94);
    F<uint32_t>(o, 0x14b) = hook::Global<uint32_t>(0x624d88);
    F<uint32_t>(o, 0x14f) = hook::Global<uint32_t>(0x624d94);
    uint8_t f = (uint8_t)((o[0x59] & 0xf) | 0x10);
    F<uint32_t>(o, 0xc) = 0;
    F<uint32_t>(o, 0x10) = 0;
    F<uint32_t>(o, 0x1f1) = 0;
    F<uint32_t>(o, 0x1ed) = 0;
    o[0x59] = f;
    F<uint32_t>(o, 8) = 0;
    F<uint32_t>(o, 0x55) = 0;
    return o;
}
RT3_REPLACE(0x53aca0, Light_Construct);

// 0x53b390. Locates name id in the pack name table: in a hash bucket (+0x203a4,
// 32 by id & 31) -> 2 with *pack = -1 - position; in the 10-entry lookup cache
// (+0x4290c) or the packs' sorted lists (+0x427b8 packs, lists +0 / counts +0x223ec
// per bucket, 13-byte entries) -> 1 with (pack, bucket, index), cached. Else 0,
// fatal (string 0x62, table +0x427c0) when asked.
int32_t __fastcall NameTable_Locate(uint8_t* t, int, int32_t id, int32_t* pack, int32_t* bucket, int32_t* index,
                                    int32_t fatal) {
    int32_t b = id & 0x1f;
    int32_t pos = F<int32_t>(t, 0x2042c + b * 4);
    for (uint8_t* e = F<uint8_t*>(t, 0x203a4 + b * 4); e; e = F<uint8_t*>(e, 0x14), ++pos)
        if (id == F<int32_t>(e, 0)) {
            *index = 0;
            *bucket = 0;
            *pack = -1 - pos;
            return 2;
        }
    for (int32_t i = 0; i < 10; ++i) {
        int32_t c = F<int32_t>(t, 0x4290c + i * 4);
        if (c == id && c != -1) {
            *pack = F<int32_t>(t, 0x42934 + i * 4);
            *bucket = F<int32_t>(t, 0x4295c + i * 4);
            *index = F<int32_t>(t, 0x42984 + i * 4);
            return 1;
        }
    }
    for (int32_t p = 0; p < F<int32_t>(t, 0x427b8); ++p) {
        uint8_t* slot = t + b * 4 + p * 0x80;
        for (int32_t k = F<int32_t>(slot, 0x223ec) - 1; k >= 0; --k) {
            if (F<int32_t>(F<uint8_t*>(slot, 0) + k * 0xd, 0) != id) continue;
            *pack = p;
            *bucket = b;
            *index = k;
            int32_t s = F<int32_t>(t, 0x429ac);
            F<int32_t>(t, 0x4290c + s * 4) = id;
            F<int32_t>(t, 0x42934 + s * 4) = p;
            F<int32_t>(t, 0x4295c + s * 4) = b;
            F<int32_t>(t, 0x42984 + s * 4) = k;
            F<int32_t>(t, 0x429ac) = s == 9 ? 0 : s + 1;
            return 1;
        }
    }
    if (fatal) {
        char msg[0x100];
        FormatText(msg, GetString(0x62), (const char*)t + 0x427c0, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr, nullptr);
        Fatal_Exit(msg);
    }
    return 0;
}
RT3_REPLACE(0x53b390, NameTable_Locate);

// 0x53b6f0. Opens the current pack's file (entry -(+0x427bc), name +0x10, fd +4,
// position +8 = 0); fatal (string 0x63, with the working directory) on failure.
int32_t __fastcall NameTable_OpenPack(uint8_t* t) {
    char path[0x12c];
    path[0] = 0;
    uint8_t* e = NameTable_Nth(t, 0, -F<int32_t>(t, 0x427bc));
    const char* name = F<const char*>(e, 0x10);
    char* d = path;
    while ((*d++ = *name++) != 0) {}
    int32_t fd = ((int(__cdecl*)(const char*, int))0x5a4c57)(path, 0x8000);
    F<int32_t>(e, 4) = fd;
    F<int32_t>(e, 8) = 0;
    if (fd == -1) {
        char cwd[0x100], msg[0x100];
        Import<uint32_t(__stdcall*)(uint32_t, char*)>(0x5c807c)(0x100, cwd);
        FormatText(msg, GetString(0x63), path, cwd, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Fatal_Exit(msg);
    }
    return F<int32_t>(e, 4);
}
RT3_REPLACE(0x53b6f0, NameTable_OpenPack);

// 0x53b7c0. Reads n bytes from the current pack (+0x427bc: < 0 = opened entry,
// else fd table +0x204ac), counting bytes (+0x428ec) and reads (+0x428f0); fatal
// (string 0x64) on a short read.
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n) {
    int32_t k = F<int32_t>(t, 0x427bc);
    int32_t fd = k < 0 ? F<int32_t>(NameTable_Nth(t, 0, -k), 4) : F<int32_t>(t, 0x204ac + k * 4);
    int32_t r = ((int(__cdecl*)(int, void*, unsigned))0x5a48c5)(fd, dst, n);
    F<int32_t>(t, 0x428ec) += n;
    F<int32_t>(t, 0x428f0) += 1;
    if (r != n) Fatal_Exit(GetString(0x64));
    return r;
}
RT3_REPLACE(0x53b7c0, NameTable_Read);

// 0x53d800. Thunk to List_Destroy.
void __fastcall List_DestroyThunk(MaxList* l) { List_Destroy(l); }
RT3_REPLACE(0x53d800, List_DestroyThunk);

} // namespace rt3
