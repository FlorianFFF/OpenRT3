// Signal blocks / flood fill / editor cost / music resume / cursor / camera ground / mesh emit / ray-triangle in blocks 0x4a..0x52 (batch 135). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Obj_CityRef10(void* o);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Signal_IsBehind(uint8_t* o, int, uint8_t* car, uint8_t* other);
void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_Get21(void* o);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode);
void __fastcall Playlist_Tick(uint8_t* p);
void* __fastcall Engine_Input(void* e);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
uint8_t __fastcall Engine_IsFullscreen(void* e);
void __fastcall Mouse_Acquire(uint8_t* m);
void __fastcall Mouse_Unacquire(uint8_t* m);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;

// 0x4aad20. Once (+0x1a2) when the vehicle has entered a signal's block (+0xa4) and
// is not in a city: clears the signal's block slot for the side it came from.
void __fastcall Vehicle_PassSignal(uint8_t* o) {
    int32_t id = F<int32_t>(o, 0xa4);
    Collection* sites = hook::Global<Collection*>(0x6cec20);
    if (id == -1 || !Collection_IsValid(sites, 0, id)) return;
    if (!Obj_CityRef10(o) && !o[0x1a2]) {
        uint8_t* b = (uint8_t*)Obj_Building(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(o, 0xa4)));
        uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(o, 0xa4));
        Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(s, 8));
        int32_t car = F<int32_t>(o, 0x41);
        uint8_t* c = car != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, car) : nullptr;
        Obj_MarkBlockSlot(b, 0, 0, Signal_IsBehind(o, 0, c, b), 0);
    }
    o[0x1a2] = 1;
}
RT3_REPLACE(0x4aad20, Vehicle_PassSignal);

// 0x4f89b0. Flood fills the map-sized byte grid from (x, y) through cells marked 1
// (4-neighbours; filled cells end as 3), then clears the unreached 1s to 0.
void __fastcall Map_FloodFill(int32_t x, int32_t y, uint8_t* grid) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    int32_t minX = x, maxX = x, minY = y, maxY = y;
    grid[w * y + x] = 2;
    int32_t changed;
    do {
        int32_t x0 = minX, x1 = maxX, y0 = minY, y1 = maxY;
        changed = 0;
        int32_t edge = !(x0 > 0 && y0 > 0 && x1 < w - 1 && y1 < h - 1);
        for (int32_t j = y0; j <= y1; ++j) {
            uint8_t* row = grid + w * j;
            for (int32_t i = x0; i <= x1; ++i) {
                if (row[i] != 2) continue;
                row[i] = 3;
                int32_t near = edge && !(i && j && i < w - 1 && j < h - 1);
                for (int k = 0; k < 4; ++k) {
                    int32_t nx = kDirX[k * 2] + i, ny = kDirY[k * 2] + j;
                    if (near && (nx < 0 || nx > w - 1 || ny < 0 || ny > h - 1)) continue;
                    uint8_t* p = grid + ny * w + nx;
                    if (*p != 1) continue;
                    *p = 2;
                    changed = 1;
                    if (nx < minX) minX = nx;
                    if (nx > maxX) maxX = nx;
                    if (ny < minY) minY = ny;
                    if (ny > maxY) maxY = ny;
                }
            }
        }
    } while (changed);
    for (int32_t j = 0; j < h; ++j)
        for (int32_t i = 0; i < w; ++i)
            if (grid[j * w + i] == 1) grid[j * w + i] = 0;
}
RT3_REPLACE(0x4f89b0, Map_FloodFill);

// 0x50f790. Cost of placing the editor's selected building (dialog 0x6d3ac8,
// tree 0x2afe) on every marked cell (0x6d1aa8, 0x2000 bytes): *count cells x the
// type's scaled price for the world's company / player.
void __fastcall Editor_SelectionCost(int32_t* total, int32_t* count) {
    *total = 0;
    *count = 0;
    uint8_t* dlg = hook::Global<uint8_t*>(0x6d3ac8);
    if (!dlg) return;
    uint8_t* tree = Widget_FindChild(dlg, 0, 0x2afe);
    if (F<int32_t>(tree, 0x1d6) < 0) return;
    uint32_t sel = TreeView_SelectedData(tree);
    if (!sel) return;
    int32_t n = 0;
    for (int k = 0; k < 0x2000; ++k)
        if (((const uint8_t*)0x6d1aa8)[k]) ++n;
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, sel);
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    int32_t player = Obj_Get25(w);
    float price = Obj_EffectScale(t, 0, Obj_Get21(hook::Global<uint8_t*>(0x6cec78)), player);
    *count = n;
    *total = (int32_t)price * n;
}
RT3_REPLACE(0x50f790, Editor_SelectionCost);

// 0x517420. Resumes the music: re-arms the playlist (+0x110 when it has tracks),
// restarts the fade timer and plays.
void __cdecl Music_Resume() {
    uint8_t* p = F<uint8_t*>(hook::Global<uint8_t*>(0x6cec74), 0x14);
    if (F<int32_t>(p, 0x100) > 0) F<int32_t>(p, 0x110) = 1;
    Sound_StartFadeTimer(hook::Global<uint8_t*>(0x6d402c), 0, 0);
    Playlist_Tick(F<uint8_t*>(hook::Global<uint8_t*>(0x6cec74), 0x14));
}
RT3_REPLACE(0x517420, Music_Resume);

// 0x51e810. Shows or hides the Windows cursor (nesting count 0x624b20, kept at 0/1)
// by calling ShowCursor until its count agrees, (un)acquires the mouse and, in
// fullscreen, puts the cursor at the game's mouse position.
void __fastcall Cursor_Show(int32_t show, int, int32_t) {
    int32_t& cnt = hook::Global<int32_t>(0x624b20);
    cnt = show ? cnt + 1 : cnt - 1;
    if (cnt && cnt != 1) cnt = 1;
    int32_t px, py;
    void* eng = hook::Global<void*>(0x6d4024);
    if (eng && Engine_Input(eng)) {
        float fx, fy;
        Mouse_GetPos((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)), 0, (uint32_t*)&fx, (uint32_t*)&fy);
        uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
        const int32_t* res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
        px = (int32_t)((double)res[0] * fx);
        py = (int32_t)((double)res[1] * fy);
    } else {
        px = py = 100;
    }
    int32_t on = hook::Global<int32_t>(0x624b20) > 0 ? 1 : 0;
    auto showCursor = Import<int32_t(__stdcall*)(int32_t)>(0x5c82d4);
    int32_t r = showCursor(on);
    int32_t k = 0;
    if (on) {
        if (r <= 0)
            while (k++ < 0x3e7 && showCursor(on) <= 0) {}
    } else if (r >= 1) {
        while (k++ < 0x3e7 && showCursor(0) >= 1) {}
    }
    showCursor(on);
    showCursor(on);
    showCursor(on);
    if (!hook::Global<void*>(0x6d4024) || !Engine_Input(hook::Global<void*>(0x6d4024))) return;
    if (Engine_IsFullscreen(hook::Global<void*>(0x6d4024)) && !on)
        Mouse_Acquire((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)));
    else
        Mouse_Unacquire((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)));
    if (Engine_IsFullscreen(hook::Global<void*>(0x6d4024))) Import<int32_t(__stdcall*)(int32_t, int32_t)>(0x5c8264)(px, py);
}
RT3_REPLACE(0x51e810, Cursor_Show);

// 0x52cf00. Ground height for the camera around cell (x, z) (clamped to 2..size-3):
// the height there, raised toward any higher point within 2.01 cells (averaging).
float __fastcall Camera_GroundAround(uint8_t* c, int, float x, float z) {
    if (Fl(0x5c878c) > x) x = 2.0f;
    if (Fl(0x5c878c) > z) z = 2.0f;
    uint8_t* r = F<uint8_t*>(c, 0x6e);
    if ((float)(Terrain_Rows(r) - 3) < x) x = (float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) - 3);
    if ((float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) - 3) < z) z = (float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) - 3);
    const float k = Fl(0x5c8700);
    const double span = D(0x5dd1b8);
    float best = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)x * k), (float)((double)z * k));
    float zz = (float)((double)z - span);
    double zEnd = (double)z + span;
    if (!(zz < zEnd)) return best;
    float xs = (float)((double)x - span);
    double xEnd = (double)x + span;
    double t;
    do {
        float xx = xs;
        if (xx < xEnd) {
            float zf = (float)((double)zz * k);
            do {
                double hgt = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)xx * k), zf);
                if (hgt > best) best = (float)((hgt + best) * D(0x5c8628));
                t = (double)xx + D(0x5c8770);
                xx = (float)t;
            } while (t < xEnd);
        }
        t = (double)zz + D(0x5c8770);
        zz = (float)t;
    } while (t < zEnd);
    return best;
}
RT3_REPLACE(0x52cf00, Camera_GroundAround);

// 0x52e520. Appends the triangles of the object's mesh (+0x14 meshes of 24 bytes,
// index +0x3a; LOD lod, clamped) to verts (x, y, z, color, u, v) offset by (x, y, z);
// *count grows by 3 per triangle. Leaves the walk state in 0xcc3b58..0xcc3b6c.
void __fastcall Mesh_EmitTriangles(uint8_t* o, int, uint8_t* verts, int32_t* count, uint32_t color, int32_t lod,
                                   float x, float y, float z) {
    uint8_t* mesh = F<uint8_t*>(o, 0x14) + F<int32_t>(o, 0x3a) * 24;
    hook::Global<uint8_t*>(0xcc3b6c) = mesh;
    if (lod >= F<int32_t>(mesh, 0x14)) lod = F<int32_t>(mesh, 0x14) - 1;
    uint8_t* l = F<uint8_t*>(mesh, 0xc) + lod * 0x45;
    hook::Global<uint8_t*>(0xcc3b58) = l;
    hook::Global<int32_t>(0xcc3b5c) = F<int32_t>(l, 0xc);
    hook::Global<uint8_t*>(0xcc3b64) = F<uint8_t*>(l, 4);
    *count += F<int32_t>(l, 0xc) * 3;
    uint32_t* v = (uint32_t*)verts;
    do {
        uint8_t* tri = hook::Global<uint8_t*>(0xcc3b64);
        const float* p0 = F<const float*>(tri, 0);
        const float* p1 = F<const float*>(tri, 4);
        const float* p2 = F<const float*>(tri, 8);
        --hook::Global<int32_t>(0xcc3b5c);
        const float* ps[3] = { p0, p1, p2 };
        for (int k = 0; k < 3; ++k) {
            float* f = (float*)(v + k * 6);
            f[0] = (float)((double)x + ps[k][0]);
            f[1] = (float)((double)y + ps[k][1]);
            f[2] = (float)((double)z + ps[k][2]);
            v[k * 6 + 3] = color;
            v[k * 6 + 4] = F<uint32_t>(hook::Global<uint8_t*>(0xcc3b64), 0x30 + k * 8);
            v[k * 6 + 5] = F<uint32_t>(hook::Global<uint8_t*>(0xcc3b64), 0x34 + k * 8);
        }
        v += 18;
        hook::Global<uint8_t*>(0xcc3b64) += 0x4c;
    } while (hook::Global<int32_t>(0xcc3b5c));
}
RT3_REPLACE(0x52e520, Mesh_EmitTriangles);

// 0x52f1e0. Whether the ray from o along dir hits triangle (a, b, c) from the front
// (determinant at least 0x5dd230), Moller-Trumbore style. Callers keep ECX/EDX.
int32_t __stdcall Ray_HitsTriangle_Impl(float a0, float a1, float a2, float b0, float b1, float b2, float c0, float c1,
                                   float c2, float o0, float o1, float o2, float d0, float d1, float d2) {
    float e2x = (float)((double)c0 - a0), e2y = (float)((double)c1 - a1), e2z = (float)((double)c2 - a2);
    double e1x = (double)b0 - a0, e1y = (double)b1 - a1, e1z = (double)b2 - a2;
    float px = (float)((double)d1 * e1z - (double)d2 * e1y);
    float py = (float)((double)d2 * e1x - e1z * d0);
    double pz = e1y * d0 - (double)d1 * e1x;
    double detx = (double)e2z * pz + (double)e2y * py + (double)px * e2x;
    float det = (float)detx;
    if (detx < Fl(0x5dd230)) return 0;
    float t0 = (float)((double)o0 - a0), t1 = (float)((double)o1 - a1);
    double t2x = (double)o2 - a2;
    float t2 = (float)t2x;
    float u = (float)(t2x * pz + (double)t1 * py + (double)t0 * px);
    if (u < Fl(0x5c8598)) return 0;
    if (u > det) return 0;
    double q0 = (double)t1 * e2z - (double)t2 * e2y;
    double q1 = (double)t2 * e2x - (double)e2z * t0;
    double q2 = (double)e2y * t0 - (double)t1 * e2x;
    double v = (double)d2 * q2 + (double)d1 * q1 + q0 * d0;
    if (v < Fl(0x5c8598)) return 0;
    return u + v > det ? 0 : 1;
}
__declspec(naked) int32_t __stdcall Ray_HitsTriangle(float, float, float, float, float, float, float, float, float,
                                                     float, float, float, float, float, float) {
    RT3_KEEP_ECX_EDX(Ray_HitsTriangle_Impl, 15)
}
RT3_REPLACE(0x52f1e0, Ray_HitsTriangle);

} // namespace rt3
