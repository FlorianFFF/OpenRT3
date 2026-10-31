// Airplane types file / name lookups / camera height smoothing / window creation / terrain mips and normals in blocks 0x40..0x53 (batch 137). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);
float __fastcall Camera_TargetGround(uint8_t* c);
void __fastcall Camera_FollowGround(uint8_t* c);
ATOM __fastcall Engine_RegisterWindowClass(HINSTANCE inst);
void __fastcall Plane_From3Points(float* nx, float* ny, float x0, float y0, float z0, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float* nz, float* d);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }

// Formats string id with one argument and exits with it.
static void FatalFormat(int32_t id, const char* arg) {
    char msg[0x12c];
    FormatText(msg, GetString(id), arg, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Fatal_Exit(msg);
}

// One AirplaneTypes.txt record (0x5c bytes).
struct PlaneRec {
    char name[0x20];
    int32_t i[3];
    float f[12];
};

// 0x40acb0. Loads up to 32 airplane types (+0x90, 0x5c each; count +0x88) from
// data\Other\AirplaneTypes.txt (';' lines are comments): name, 3 ints, 11 floats;
// field +0x3c = 1 / +0x38 (at least 0.1). +0x8c = the earliest year (+0x24) of
// the types without flag +0x20.
void __fastcall Airplanes_LoadTypes(uint8_t* o) {
    char path[0x100];
    FormatText(path, (const char*)0x5c89ec, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    auto fopen_ = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto fscanf_ = (int(__cdecl*)(FILE*, const char*, ...))0x5a17e1;
    auto fgetc_ = (int(__cdecl*)(FILE*))0x5a1791;
    FILE* f = fopen_(path, (const char*)0x5c89e8);
    if (!f) FatalFormat(0xac, path);
    F<int32_t>(o, 0x8c) = 9999;
    F<int32_t>(o, 0x88) = 0;
    auto eof = [&]() { return (F<uint8_t>(f, 0xc) & 0x10) != 0; };
    while (!eof()) {
        char line[0x400];
        fscanf_(f, (const char*)0x5c89e4, line);
        if (line[0] == ';') {
            if (fgetc_(f) != '\n')
                while (!eof() && fgetc_(f) != '\n') {}
            continue;
        }
        PlaneRec r;
        char* d = r.name;
        for (const char* s = line; (*d++ = *s++) != 0;) {}
        fscanf_(f, (const char*)0x5c89b8, &r.i[0], &r.i[1], &r.i[2], &r.f[0], &r.f[1], &r.f[2], &r.f[7], &r.f[5],
                &r.f[6], &r.f[3], &r.f[8], &r.f[9], &r.f[10], &r.f[11]);
        if ((double)r.f[3] < D(0x5c86b8)) *(uint32_t*)&r.f[3] = 0x3dcccccd;
        r.f[4] = (float)(D(0x5c8770) / r.f[3]);
        int32_t n = F<int32_t>(o, 0x88);
        memcpy(o + 0x90 + n * 0x5c, &r, 0x5c);
        uint8_t* e = o + n * 0x5c;
        if (F<int32_t>(o, 0x8c) > F<int32_t>(e, 0xb4) && !F<int32_t>(e, 0xb0)) F<int32_t>(o, 0x8c) = F<int32_t>(e, 0xb4);
        F<int32_t>(o, 0x88) = ++n;
        if (n >= 0x20) break;
    }
    ((int(__cdecl*)(FILE*))0x5a1740)(f);
}
RT3_REPLACE(0x40acb0, Airplanes_LoadTypes);

// 0x40cd70. Resolves the object's type name (+0x3ae) in 0x62b2fc (names +4): id to
// +0x3cc and the type's +0x173 to +0x3d0; fatal (string 0xad) if unknown.
void __fastcall Obj_ResolveType3ae(uint8_t* o) {
    F<uint32_t>(o, 0x3cc) = 0;
    F<uint32_t>(o, 0x3d0) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62b2fc)); ++i) {
        Collection* c = hook::Global<Collection*>(0x62b2fc);
        if (Stricmp((const char*)o + 0x3ae, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4)) continue;
        F<uint32_t>(o, 0x3cc) = Collection_IdAt(hook::Global<Collection*>(0x62b2fc), 0, i, 0);
        c = hook::Global<Collection*>(0x62b2fc);
        F<uint32_t>(o, 0x3d0) = F<uint32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x173);
        break;
    }
    if (!F<uint32_t>(o, 0x3cc)) FatalFormat(0xad, (const char*)o + 0x3ae);
}
RT3_REPLACE(0x40cd70, Obj_ResolveType3ae);

// 0x416e20. Id of the element named `name` (+4); fatal (string 0xbd) and -1 if none.
int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (!Stricmp(name, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4))
            return Collection_IdAt(c, 0, i, 0);
    FatalFormat(0xbd, name);
    return -1;
}
RT3_REPLACE(0x416e20, Collection_FindByName4);

// 0x41ac60. Resolves the car's type name (+0x36a) in 0x62ba88 (names +0x82) into
// +0x388; fatal (string 0xc1) if unknown.
void __fastcall Car_ResolveType(uint8_t* o) {
    F<uint32_t>(o, 0x388) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62ba88)); ++i) {
        Collection* c = hook::Global<Collection*>(0x62ba88);
        if (Stricmp((const char*)o + 0x36a, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 0x82)) continue;
        F<uint32_t>(o, 0x388) = Collection_IdAt(hook::Global<Collection*>(0x62ba88), 0, i, 0);
        break;
    }
    if (!F<uint32_t>(o, 0x388)) FatalFormat(0xc1, (const char*)o + 0x36a);
}
RT3_REPLACE(0x41ac60, Car_ResolveType);

// 0x43cec0. For attached cameras of modes with ground following (0x5ee4d0): follows
// the ground, then eases the remembered height (+0x2ee) toward the eye height (+8)
// by at most 0.2 (up) / 0.4 (down) of cos(pitch) x zoom per call, and below zoom
// 1250 blends the eye height between them (fully the eye's below zoom 1000).
void __fastcall Camera_SmoothHeight(uint8_t* c) {
    if (F<int32_t>(c, 0x60)) return;
    if (!((const uint8_t*)0x5ee4d0)[F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x10c)]) return;
    Camera_TargetGround(c);
    float reach = (float)(((double(__cdecl*)(double))0x5a13e4)(F<float>(c, 0x14)) * F<float>(c, 0x18));
    Camera_FollowGround(c);
    float y = F<float>(c, 8);
    if (F<float>(c, 0x18) < Fl(0x5c86a0)) {
        F<float>(c, 0x2ee) = y;
        return;
    }
    if (F<float>(c, 0x2ee) < y) {
        float diff = (float)((double)y - F<float>(c, 0x2ee));
        double lim = (double)reach * D(0x5c8c20);
        if ((double)diff > lim) F<float>(c, 0x2ee) = (float)((double)diff - lim + F<float>(c, 0x2ee));
    } else {
        float diff = (float)((double)F<float>(c, 0x2ee) - y);
        double lim = (double)reach * D(0x5c88e8);
        if ((double)diff > lim) F<float>(c, 0x2ee) = (float)((double)F<float>(c, 0x2ee) - ((double)diff - lim));
    }
    if (F<float>(c, 0x18) < D(0x5ca228)) {
        float t = (float)((D(0x5ca228) - F<float>(c, 0x18)) / D(0x5ca220));
        if (t > Fl(0x5c85b4)) t = 1.0f;
        else if (t < Fl(0x5c8598)) t = 0.0f;
        F<float>(c, 8) = (float)((D(0x5c8770) - t) * F<float>(c, 0x2ee) + (double)t * y);
    } else {
        F<uint32_t>(c, 8) = F<uint32_t>(c, 0x2ee);
    }
}
RT3_REPLACE(0x43cec0, Camera_SmoothHeight);

// 0x520040. Creates the game window at (x, y) (+0x4d/+0x51, reset when off the
// first quarter of the display mode +0x34[+0x11421e]): windowed with the frame
// style 0x4cb0000, or a bare popup for fullscreen; sets the icon 0x6d4038.
int32_t __fastcall Engine_CreateWindow(uint8_t* e, int, int32_t x, int32_t y, int32_t mode, uint8_t fullscreen) {
    F<int32_t>(e, 0x4d) = x;
    F<int32_t>(e, 0x51) = y;
    if (x < 0) F<int32_t>(e, 0x4d) = 0;
    if (y < 0) F<int32_t>(e, 0x51) = 0;
    const int32_t* res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    if (F<int32_t>(e, 0x4d) > res[0] >> 2) F<int32_t>(e, 0x4d) = 0;
    if (F<int32_t>(e, 0x51) > res[1] >> 2) F<int32_t>(e, 0x51) = 0;
    if (mode > F<int32_t>(e, 0x30)) Fatal_Exit(GetString(0x15));
    res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t w = res[0], h = res[1];
    if (!Engine_RegisterWindowClass(F<HINSTANCE>(e, 4))) Fatal_Exit(GetString(0x16));
    auto create = Import<HWND(__stdcall*)(DWORD, const char*, const char*, DWORD, int, int, int, int, HWND, HMENU,
                                          HINSTANCE, void*)>(0x5c82b8);
    if (!fullscreen) {
        RECT r = { 0, 0, w - 1, h - 1 };
        Import<BOOL(__stdcall*)(RECT*, DWORD, BOOL)>(0x5c82b4)(&r, 0x4cb0000, FALSE);
        F<HWND>(e, 0) = create(0, (const char*)0x5dcee8, (const char*)0x624b68, 0x4cb0000, F<int32_t>(e, 0x4d),
                               F<int32_t>(e, 0x51), r.right - r.left + 1, r.bottom - r.top + 1, nullptr, nullptr,
                               F<HINSTANCE>(e, 4), nullptr);
    } else {
        F<HWND>(e, 0) = nullptr;
        HWND wnd = create(0, (const char*)0x5dcee8, (const char*)0x624b68, 0x4000000, 0, 0, w, h, nullptr, nullptr,
                          F<HINSTANCE>(e, 4), nullptr);
        F<HWND>(e, 0) = wnd;
        Import<LONG(__stdcall*)(HWND, int, LONG)>(0x5c82bc)(wnd, -16, 0x4000000);
    }
    e[0x55] = 0;
    if (uint16_t icon = hook::Global<uint16_t>(0x6d4038)) {
        auto loadIcon = Import<HICON(__stdcall*)(HINSTANCE, const char*)>(0x5c82cc);
        auto send = Import<LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM)>(0x5c82c0);
        send(F<HWND>(e, 0), 0x80, 1, (LPARAM)loadIcon(F<HINSTANCE>(e, 4), (const char*)(uintptr_t)icon));
        send(F<HWND>(e, 0), 0x80, 0,
             (LPARAM)loadIcon(F<HINSTANCE>(e, 4), (const char*)(uintptr_t)hook::Global<uint16_t>(0x6d4038)));
    }
    return 0;
}
RT3_REPLACE(0x520040, Engine_CreateWindow);

// 0x5336e0. Allocates the terrain's 4 mip levels once (+0x15d1): per level floats
// (+0x1609), 12-byte normals (+0x15f5) and words (+0x161d), zeroed; fatal 0xa4.
RT3_NOINLINE void __fastcall Terrain_AllocMips(uint8_t* t) {
    if (F<int32_t>(t, 0x15d1)) return;
    for (int32_t level = 1; level < 5; ++level) {
        if (F<int32_t>(t, 0x15d1)) continue;
        int32_t n = ((F<int32_t>(t, 0x15d9) >> level) + 1) * ((F<int32_t>(t, 0x15d5) >> level) + 1);
        uint8_t* slot = t + 0x15f5 + (level - 1) * 4;
        F<void*>(slot, 0x14) = Malloc(n * 4);
        F<void*>(slot, 0) = Malloc(n * 12);
        F<void*>(slot, 0x28) = Malloc(n * 2);
        if (!F<void*>(slot, 0x14) || !F<void*>(slot, 0) || !F<void*>(slot, 0x28)) Fatal_Exit(GetString(0xa4));
        memset(F<void*>(slot, 0x14), 0, n * 4);
        memset(F<void*>(slot, 0), 0, n * 12);
        memset(F<void*>(slot, 0x28), 0, n * 2);
    }
    F<int32_t>(t, 0x15d1) = 1;
}
RT3_REPLACE(0x5336e0, Terrain_AllocMips);

// 0x5345c0. Surface normal of the terrain triangle under world point (x, z) (cells
// of 92, clamped to the map), from the cell's lower or upper triangle; returns out.
float* __fastcall Terrain_NormalAt(uint8_t* t, int, float* out, float x, float z) {
    int32_t ix, iz;
    __asm {
        fld x
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov ix, eax
        fld z
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov iz, eax
    }
    if (ix < 0) {
        ix = 0;
        x = 0.0f;
    } else if (ix + 1 >= F<int32_t>(t, 0x15d9)) {
        ix = F<int32_t>(t, 0x15d9) - 2;
        x = (float)(ix * 0x5c);
    }
    double zz = z;
    if (iz < 0) {
        iz = 0;
        zz = Fl(0x5c8598);
    } else if (iz + 1 >= F<int32_t>(t, 0x15d5)) {
        iz = F<int32_t>(t, 0x15d5) - 2;
        zz = (double)(iz * 0x5c);
    }
    int32_t w1 = F<int32_t>(t, 0x15d9) + 1;
    const float* h = F<const float*>(t, 0x1605) + w1 * iz + ix;
    double fx = (double)x - ix * 0x5c;
    double fz = zz - iz * 0x5c;
    float nx, ny, nz, d;
    if (fz < fx)
        Plane_From3Points(&nx, &ny, 0.0f, h[0], 0.0f, 92.0f, h[1], 0.0f, 92.0f, h[w1 + 1], 92.0f, &nz, &d);
    else
        Plane_From3Points(&nx, &ny, 0.0f, h[0], 0.0f, 0.0f, h[w1], 92.0f, 92.0f, h[w1 + 1], 92.0f, &nz, &d);
    out[0] = nx;
    out[1] = ny;
    out[2] = nz;
    return out;
}
RT3_REPLACE(0x5345c0, Terrain_NormalAt);

} // namespace rt3
