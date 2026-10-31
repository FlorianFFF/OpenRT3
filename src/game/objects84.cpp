// Terrain queries / scene object registration in blocks 0x52..0x53 (batch 87). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include <stdint.h>
#include <initializer_list>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

PtrListNode* __fastcall Terrain_AddGlobal(uint8_t* r, int, void* v);
double __stdcall Plane_HeightAt(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2,
                                float z2, float qx, float qz);

// 0x52da70. Registers the object with its terrain (+4) once (pending flag 0x40).
void __fastcall SceneObj_Register(uint8_t* o) {
    if (!F<uint8_t*>(o, 4) || !(o[0x20] & 0x40)) return;
    o[0x20] &= 0xbf;
    F<PtrListNode*>(o, 0xc) = Terrain_AddGlobal(F<uint8_t*>(o, 4), 0, o);
}
RT3_REPLACE(0x52da70, SceneObj_Register);

// 0x52ebd0. Loads flag bits 5 and 6 (see SceneObj_SaveFlags); an object saved
// without bit 6 is registered with its terrain right away.
void __fastcall SceneObj_LoadFlags(uint8_t* o, int, Stream s) {
    uint8_t b = 0;
    StreamRead(s, &b, 1, 0, 1);
    o[0x20] ^= (uint8_t)(((b << 5) ^ o[0x20]) & 0x20);
    StreamRead(s, &b, 1, 0, 1);
    uint8_t fl = o[0x20];
    fl ^= (uint8_t)(((b << 6) ^ fl) & 0x40);
    o[0x20] = fl;
    if (fl & 0x40) return;
    fl |= 0x40;
    o[0x20] = fl;
    if (!F<uint8_t*>(o, 4) || !(fl & 0x40)) return;
    o[0x20] = fl & 0xbf;
    F<PtrListNode*>(o, 0xc) = Terrain_AddGlobal(F<uint8_t*>(o, 4), 0, o);
}
RT3_REPLACE(0x52ebd0, SceneObj_LoadFlags);

static inline PtrList* Cell(uint8_t* r, uint32_t grid, int32_t x, int32_t y) {
    return F<PtrList**>(r, grid)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
}
PtrList* __fastcall Terrain_Cell1675(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1675, x, y); }
RT3_REPLACE(0x533db0, Terrain_Cell1675);
PtrListNode* __fastcall Terrain_Add1675(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1675, x, y), 0, v);
}
RT3_REPLACE(0x533dd0, Terrain_Add1675);
void __fastcall Terrain_Remove1675(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1675, x, y), 0, n);
}
RT3_REPLACE(0x533e00, Terrain_Remove1675);
PtrList* __fastcall Terrain_Cell1681(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1681, x, y); }
RT3_REPLACE(0x533e30, Terrain_Cell1681);
PtrList* __fastcall Terrain_Cell1685(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1685, x, y); }
RT3_REPLACE(0x533e50, Terrain_Cell1685);

// 0x5343f0. Appends an entry (9 bytes, first dword = v) to the table +0x1695
// (count +0x1691, max 300); returns the new count or -1 when full.
int32_t __fastcall Terrain_AddMarker(uint8_t* r, int, uint32_t v, int32_t, int32_t) {
    int32_t n = F<int32_t>(r, 0x1691);
    if (n >= 0x12c) return -1;
    F<uint32_t>(r, 0x1695 + n * 9) = v;
    return ++F<int32_t>(r, 0x1691);
}
RT3_REPLACE(0x5343f0, Terrain_AddMarker);

// (int)(v * 0x5dcfe8) with the x87 product and the exe's _ftol.
static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x534490. Terrain height at world (x, z): the cell (92 units) is clamped to
// the grid and split along its diagonal into two triangles (heights +0x1605).
RT3_NOINLINE double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z) {
    int32_t ix = CellOf(x);
    int32_t iz = CellOf(z);
    float xs = x;
    if (ix < 0) {
        ix = 0;
        xs = 0.0f;
    } else if (ix + 1 >= F<int32_t>(r, 0x15d9)) {
        ix = F<int32_t>(r, 0x15d9) - 2;
        xs = (float)(ix * 0x5c);
    }
    double zv;
    if (iz < 0) {
        iz = 0;
        zv = *(const float*)0x5c8598;
    } else if (iz + 1 >= F<int32_t>(r, 0x15d5)) {
        iz = F<int32_t>(r, 0x15d5) - 2;
        zv = (double)(iz * 0x5c);
    } else {
        zv = z;
    }
    double dx = (double)xs - (double)(ix * 0x5c);
    float fx = (float)dx;
    float fz = (float)(zv - (double)(iz * 0x5c));
    int32_t stride = F<int32_t>(r, 0x15d9) + 1;
    const float* h = F<float*>(r, 0x1605) + (stride * iz + ix);
    if (dx > fz)
        return Plane_HeightAt(0.0f, h[0], 0.0f, 92.0f, h[1], 0.0f, 92.0f, h[stride + 1], 92.0f, fx, fz);
    return Plane_HeightAt(0.0f, h[0], 0.0f, 0.0f, h[stride], 92.0f, 92.0f, h[stride + 1], 92.0f, fx, fz);
}
RT3_REPLACE(0x534490, Terrain_HeightAt);

uint32_t __fastcall Render_Get1588(uint8_t* r) { return r[0x1588]; }
RT3_REPLACE(0x534910, Render_Get1588);
uint32_t __fastcall Render_Get1584(uint8_t* r) { return F<uint32_t>(r, 0x1584); }
RT3_REPLACE(0x534920, Render_Get1584);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h) {
    *w = F<int32_t>(r, 0x157c) << 2;
    *h = F<int32_t>(r, 0x1580) << 2;
}
RT3_REPLACE(0x534c50, Render_GetSize4);

// Terrain vertex records (+0x165d, 3 bytes, stride +0x15dd): low 3 bits = kind.
static inline uint8_t* Rec(uint8_t* r, int32_t x, int32_t y) {
    return F<uint8_t*>(r, 0x165d) + (F<int32_t>(r, 0x15dd) * y + x) * 3;
}
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y) {
    int32_t k = Rec(r, x, y)[0] & 7;
    return k == 1 || (k > 2 && k <= 5) ? 1 : 0;
}
RT3_REPLACE(0x534e10, Terrain_KindIs1Or3to5);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 1 || k == 4 ? 1 : 0;
}
RT3_REPLACE(0x534e50, Terrain_KindIs1Or4);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y) {
    return (*(const uint16_t*)Rec(r, x, y) >> 9) & 1;
}
RT3_REPLACE(0x534e90, Terrain_RecBit9);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 2 || k == 5 || k == 4 ? 1 : 0;
}
RT3_REPLACE(0x534ec0, Terrain_KindIs2Or4Or5);

void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Destroy(PtrList* l);

// 0x534430. Deletes the marker objects of the table +0x1695 (9-byte entries,
// count +0x1691) and empties it.
RT3_NOINLINE void __fastcall Terrain_DeleteMarkers(uint8_t* r) {
    for (int32_t i = 0; i < F<int32_t>(r, 0x1691); ++i) {
        uint8_t* o = F<uint8_t*>(r, 0x1695 + i * 9);
        if (o) {
            SceneObj_Destructor(o);
            ((void(__cdecl*)(void*))0x53b080)(o);
        }
        F<uint8_t*>(r, 0x1695 + i * 9) = nullptr;
    }
    F<int32_t>(r, 0x1691) = 0;
}
RT3_REPLACE(0x534430, Terrain_DeleteMarkers);

// 0x535b40. Deletes the per-16x16-block object lists +0x1671 with all their
// scene objects, then the grid itself.
RT3_NOINLINE void __fastcall Terrain_FreeBlockObjects(uint8_t* r) {
    if (!F<PtrList**>(r, 0x1671)) return;
    for (int32_t x = 0; x < F<int32_t>(r, 0x15d9) >> 4; ++x) {
        for (int32_t y = 0; y < F<int32_t>(r, 0x15d5) >> 4; ++y) {
            PtrList* l = F<PtrList**>(r, 0x1671)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
            int32_t n = l->count;
            if (n > 0) {
                PtrList_Rewind(l);
                uint8_t* o = (uint8_t*)PtrList_Next(l);
                do {
                    if (o) {
                        SceneObj_Destructor(o);
                        ((void(__cdecl*)(void*))0x53b080)(o);
                    }
                    o = (uint8_t*)PtrList_Next(l);
                } while (--n);
            }
            l = F<PtrList**>(r, 0x1671)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
            if (l) {
                PtrList_Destroy(l);
                ((void(__cdecl*)(void*))0x53b080)(l);
            }
        }
    }
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(r, 0x1671));
    F<void*>(r, 0x1671) = nullptr;
}
RT3_REPLACE(0x535b40, Terrain_FreeBlockObjects);

void __fastcall Terrain_DestroyCell1681(uint8_t* r);
void __fastcall Terrain_DestroyCell1685(uint8_t* r);
void __fastcall Terrain_DestroyCell1679(uint8_t* r);
void __fastcall Terrain_DestroyCell1675(uint8_t* r);
void __fastcall Terrain_DestroyCell167d(uint8_t* r);

// 0x536030. Frees the terrain (when its height grids +0x1605 exist): the five
// level grids (+0x15f1 / +0x1605 / +0x1619), the block object lists and cell
// grids, and the remaining buffers.
RT3_NOINLINE void __fastcall Terrain_Free(uint8_t* r) {
    auto crtFree = [](void* p) { ((void(__cdecl*)(void*))0x5a1145)(p); };
    if (!F<void*>(r, 0x1605)) return;
    for (int k = 0; k < 5; ++k) {
        crtFree(F<void*>(r, 0x1605 + k * 4));
        crtFree(F<void*>(r, 0x15f1 + k * 4));
        crtFree(F<void*>(r, 0x1619 + k * 4));
        F<void*>(r, 0x1605 + k * 4) = nullptr;
        F<void*>(r, 0x15f1 + k * 4) = nullptr;
        F<void*>(r, 0x1619 + k * 4) = nullptr;
    }
    Terrain_FreeBlockObjects(r);
    Terrain_DestroyCell1681(r);
    Terrain_DestroyCell1685(r);
    Terrain_DestroyCell1679(r);
    Terrain_DestroyCell1675(r);
    Terrain_DestroyCell167d(r);
    if (F<void*>(r, 0x15e5)) {
        crtFree(F<void*>(r, 0x15e5));
        F<void*>(r, 0x15e5) = nullptr;
    }
    if (F<void*>(r, 0x162d)) crtFree(F<void*>(r, 0x162d));
    for (int k = 0; k < 4; ++k) {
        if (F<void*>(r, 0x1631 + k * 4)) crtFree(F<void*>(r, 0x1631 + k * 4));
        F<void*>(r, 0x1631 + k * 4) = nullptr;
    }
    for (uint32_t off : { 0x1669u, 0x1655u, 0x1659u })
        if (F<void*>(r, off)) crtFree(F<void*>(r, off));
    F<void*>(r, 0x1669) = nullptr;
    F<void*>(r, 0x1659) = nullptr;
    F<void*>(r, 0x1655) = nullptr;
    F<void*>(r, 0x162d) = nullptr;
    if (F<void*>(r, 0x165d)) crtFree(F<void*>(r, 0x165d));
    F<void*>(r, 0x165d) = nullptr;
    if (F<void*>(r, 0x1661)) crtFree(F<void*>(r, 0x1661));
    if (F<void*>(r, 0x1665)) crtFree(F<void*>(r, 0x1665));
    F<void*>(r, 0x1661) = nullptr;
    F<void*>(r, 0x1665) = nullptr;
    crtFree(F<void*>(r, 0x15ed));
    crtFree(F<void*>(r, 0x15e9));
    F<void*>(r, 0x15e9) = nullptr;
    F<void*>(r, 0x15ed) = nullptr;
    F<int32_t>(r, 0x15d1) = 0;
}
RT3_REPLACE(0x536030, Terrain_Free);

void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols);
void __fastcall Terrain_AllocMips(uint8_t* t);
void __fastcall Terrain_FreeFixedChannels(uint8_t* r);
void __fastcall Render_ReleaseTextureSlots(uint8_t* r);
void __fastcall Texture_Destroy(uint8_t* t);

// 0x5361d0. Resizes the terrain to w x h cells (vertex stride w + 1) when the
// size changes: frees it and allocates the grids and mip levels again.
RT3_NOINLINE void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h) {
    if (w == F<int32_t>(r, 0x15d9) && h == F<int32_t>(r, 0x15d5)) return;
    Terrain_Free(r);
    F<int32_t>(r, 0x15d9) = w;
    F<int32_t>(r, 0x15d5) = h;
    F<int32_t>(r, 0x15dd) = w + 1;
    Render_AllocGrid(r, 0, 0, 0);
    Terrain_AllocMips(r);
}
RT3_REPLACE(0x5361d0, Terrain_Resize);

// 0x537330. Destroys the terrain (when built, +0x154c): its channels, grids
// and markers, texture slots, the texture +0x478 and the list +0x166d.
RT3_NOINLINE void __fastcall Terrain_Destroy(uint8_t* r) {
    if (!F<int32_t>(r, 0x154c)) return;
    Terrain_FreeFixedChannels(r);
    Terrain_Free(r);
    Terrain_DeleteMarkers(r);
    Render_ReleaseTextureSlots(r);
    if (uint8_t* t = F<uint8_t*>(r, 0x478)) {
        Texture_Destroy(t);
        ((void(__cdecl*)(void*))0x53b080)(t);
        F<uint8_t*>(r, 0x478) = nullptr;
        r[0x159b] = 0;
    }
    if (PtrList* l = F<PtrList*>(r, 0x166d)) {
        PtrList_Destroy(l);
        ((void(__cdecl*)(void*))0x53b080)(l);
    }
    F<PtrList*>(r, 0x166d) = nullptr;
    F<int32_t>(r, 0x154c) = 0;
}
RT3_REPLACE(0x537330, Terrain_Destroy);

void* __fastcall Engine_Get1C(void* e);
void __fastcall Engine_Set1C(void* e, int, void* v);

// 0x537e30. Terrain renderer destructor: destroys it and unhooks it from the
// engine (+0x1c) when it is the current one.
void __fastcall Terrain_Destructor(uint8_t* r) {
    F<uint32_t>(r, 0) = 0x5dd25c;
    Terrain_Destroy(r);
    if (Engine_Get1C(hook::Global<uint8_t*>(0x6d4024)) == r) Engine_Set1C(hook::Global<uint8_t*>(0x6d4024), 0, nullptr);
}
RT3_REPLACE(0x537e30, Terrain_Destructor);

void __fastcall Map_Free(uint8_t* m);
void __fastcall Map_AllocGrids(uint8_t* m);

// 0x4494c0. Resizes the map to w x h cells when the size changes: frees the
// map grids, resizes the terrain and allocates the grids again.
void __fastcall Map_Resize(uint8_t* m, int, int32_t w, int32_t h) {
    if (w == F<int32_t>(m, 0x15d9) && h == F<int32_t>(m, 0x15d5)) return;
    Map_Free(m);
    Terrain_Resize(m, 0, w, h);
    Map_AllocGrids(m);
}
RT3_REPLACE(0x4494c0, Map_Resize);

} // namespace rt3
