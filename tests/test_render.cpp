// Difftests for render-side code the simulation check never runs: colour ramps in
// every pixel format, visible terrain bounds, image area average colour.
#include "core/ptrlist.h"
#include "core/containers.h"
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

namespace rt3 {
uint32_t __stdcall HeatColor(float t);
uint32_t __stdcall OverlayColor(float t);
uint32_t __stdcall HeightColor(float t);
void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain,
                                    int32_t shift);
void __fastcall Image_AverageColor(uint8_t* o, int, int32_t x, int32_t y, uint8_t* outR, uint8_t* outG, uint8_t* outB);
}
using namespace rt3;

namespace {

uint32_t g_seed = 12345;
// The LCG's low bits have short periods (bit 0 just alternates), so `Rnd() % 2`
// would repeat with the call count; fold the high bits in.
uint32_t Rnd() {
    g_seed = g_seed * 1103515245 + 12345;
    return g_seed ^ (g_seed >> 16);
}
// Zeroes the stack the next call will use, for originals that read stale locals.
__declspec(noinline) void ZeroStackBelow() {
    volatile uint8_t pad[0x4000];
    for (size_t k = 0; k < sizeof pad; ++k) pad[k] = 0;
}
// Offset of the first differing byte, or -1.
int FirstDiff(const void* a, const void* b, size_t n) {
    for (size_t k = 0; k < n; ++k)
        if (((const uint8_t*)a)[k] != ((const uint8_t*)b)[k]) return (int)k;
    return -1;
}
float RndF(float lo, float hi) { return lo + (hi - lo) * (float)((Rnd() >> 8) & 0xffff) / 65535.0f; }

// Points the engine global at a fake engine with the given pixel format for the
// duration of a test.
struct FakeEngine {
    uint8_t* e;
    void* saved;
    FakeEngine() {
        e = (uint8_t*)VirtualAlloc(nullptr, 0x114800, MEM_COMMIT, PAGE_READWRITE);
        saved = hook::Global<void*>(0x6d4024);
        hook::Global<void*>(0x6d4024) = e;
    }
    ~FakeEngine() {
        hook::Global<void*>(0x6d4024) = saved;
        VirtualFree(e, 0, MEM_RELEASE);
    }
    void Mode(int32_t m) { *(int32_t*)(e + 0x11474e) = m; }
};

} // namespace

DIFFTEST(render_ColorRamps) {
    FakeEngine fe;
    auto oHeat = (uint32_t(__stdcall*)(float))0x51dfc0;
    auto oOver = (uint32_t(__stdcall*)(float))0x4bd360;
    auto oHeight = (uint32_t(__stdcall*)(float))0x51e1d0;
    static const int32_t modes[] = { 0, 1, 2, 3, 4, 5 };
    for (int32_t m : modes) {
        fe.Mode(m);
        for (int i = -40; i <= 1040; ++i) {
            float t = i / 1000.0f;
            EXPECT_EQ(oHeat(t), HeatColor(t), "heat mode %d t %f", m, t);
            EXPECT_EQ(oOver(t), OverlayColor(t), "overlay mode %d t %f", m, t);
            EXPECT_EQ(oHeight(t), HeightColor(t), "height mode %d t %f", m, t);
        }
        for (int i = 0; i < 2000; ++i) {
            float t = RndF(-0.5f, 1.5f);
            EXPECT_EQ(oHeat(t), HeatColor(t), "heat mode %d t %f", m, t);
            EXPECT_EQ(oOver(t), OverlayColor(t), "overlay mode %d t %f", m, t);
            EXPECT_EQ(oHeight(t), HeightColor(t), "height mode %d t %f", m, t);
        }
    }
}

DIFFTEST(render_VisibleBounds) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t*, int32_t*, int32_t*, int32_t*, uint8_t*, int32_t))0x5271b0;
    static uint8_t scene[0x3800], terrain[0x1600];
    for (int it = 0; it < 3000; ++it) {
        for (int k = 0; k < 8; ++k) *(float*)(scene + 0x36e8 + k * 4) = RndF(-2000.0f, 60000.0f);
        *(int32_t*)(scene + 0x3682) = (int32_t)(Rnd() % 700) - 50;
        *(int32_t*)(scene + 0x3686) = (int32_t)(Rnd() % 700) - 50;
        *(int32_t*)(terrain + 0x15d9) = (int32_t)(Rnd() % 1024);
        *(int32_t*)(terrain + 0x15d5) = (int32_t)(Rnd() % 1024);
        int32_t shift = Rnd() % 4;
        int32_t a[4] = { -7, -7, -7, -7 }, b[4] = { -7, -7, -7, -7 };
        orig(scene, 0, &a[0], &a[1], &a[2], &a[3], terrain, shift);
        Scene_VisibleBounds(scene, 0, &b[0], &b[1], &b[2], &b[3], terrain, shift);
        for (int k = 0; k < 4; ++k) EXPECT_EQ(a[k], b[k], "bound %d iteration %d", k, it);
    }
}

DIFFTEST(render_AverageColor) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, uint8_t*, uint8_t*, uint8_t*))0x4f8770;
    static uint8_t img[0x200];
    static uint32_t px[64 * 48];
    for (uint32_t& p : px) p = Rnd();
    *(uint32_t**)(img + 0x118) = px;
    *(int32_t*)(img + 0xfc) = 64;
    *(int32_t*)(img + 0x100) = 48;
    for (int it = 0; it < 2000; ++it) {
        *(int32_t*)(img + 0xc4) = Rnd() % 9;
        int32_t x = (int32_t)(Rnd() % 80) - 8, y = (int32_t)(Rnd() % 64) - 8;
        uint8_t a[3] = {}, b[3] = {};
        orig(img, 0, x, y, &a[0], &a[1], &a[2]);
        Image_AverageColor(img, 0, x, y, &b[0], &b[1], &b[2]);
        for (int k = 0; k < 3; ++k) EXPECT_EQ(a[k], b[k], "channel %d at %d,%d r %d", k, x, y, *(int32_t*)(img + 0xc4));
    }
}

namespace rt3 { int32_t __fastcall Mesh_RayHit(uint8_t* o, int, const float* dir, const float* origin); }

DIFFTEST(render_MeshRayHit) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, const float*, const float*))0x530030;
    static float verts[30][3];
    static uint8_t tris[10 * 0x4c], sub[2 * 0x45], lod[2 * 0x18];
    static uint8_t oa[0x200], ob[0x200];
    for (int it = 0; it < 4000; ++it) {
        for (auto& v : verts) for (float& c : v) c = RndF(-10.0f, 10.0f);
        int32_t n = Rnd() % 11;
        for (int t = 0; t < 10; ++t)
            for (int k = 0; k < 3; ++k) *(float**)(tris + t * 0x4c + k * 4) = verts[Rnd() % 30];
        *(uint8_t**)(sub + 0x45 + 4) = tris;
        *(int32_t*)(sub + 0x45 + 0xc) = n;
        *(uint8_t**)(lod + 0x18 + 0xc) = sub;
        memset(oa, 0, sizeof oa);
        *(uint8_t**)(oa + 0x14) = lod;
        *(int32_t*)(oa + 0x3a) = 1;
        *(int32_t*)(oa + 0x21) = 1;
        oa[0x20] = (uint8_t)(Rnd() & 0xff);
        for (int k = 0; k < 16; ++k) {
            *(float*)(oa + 0x8e + k * 4) = RndF(-2.0f, 2.0f);
            *(float*)(oa + 0xce + k * 4) = RndF(-2.0f, 2.0f);
        }
        for (int k = 0; k < 3; ++k) *(float*)(oa + 0x1e2 + k * 4) = RndF(-5.0f, 5.0f);
        memcpy(ob, oa, sizeof oa);
        float dir[3] = { RndF(-1, 1), RndF(-1, 1), RndF(-1, 1) }, org[3] = { RndF(-20, 20), RndF(-20, 20), RndF(-20, 20) };
        int32_t ra = orig(oa, 0, dir, org);
        uint32_t ta = hook::Global<uint32_t>(0xcc3b64), la = hook::Global<uint32_t>(0xcc3b5c);
        int32_t rb = Mesh_RayHit(ob, 0, dir, org);
        uint32_t tb = hook::Global<uint32_t>(0xcc3b64), lb = hook::Global<uint32_t>(0xcc3b5c);
        EXPECT_EQ(ra, rb, "hit iteration %d", it);
        EXPECT_EQ(ta, tb, "triangle pointer iteration %d", it);
        EXPECT_EQ(la, lb, "left iteration %d", it);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "object iteration %d", it);
    }
}

namespace rt3 { void __fastcall Mesh_FaceCamera(uint8_t* o, int, uint8_t* cam, int32_t key, int32_t idx); }

DIFFTEST(render_MeshFaceCamera) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*, int32_t, int32_t))0x52f970;
    static float verts[20][3], outA[20][3], outB[20][3];
    static uint8_t subA[3 * 0x45], subB[3 * 0x45], lodA[0x20], lodB[0x20], o_a[0x40], o_b[0x40];
    static uint8_t cam[0x40];
    for (int it = 0; it < 2000; ++it) {
        for (auto& v : verts) for (float& c : v) c = RndF(-50.0f, 50.0f);
        for (int k = 0; k < 16; ++k) *(float*)(cam + k * 4) = RndF(-100.0f, 100.0f);
        memset(subA, 0, sizeof subA);
        int32_t n = Rnd() % 21, idx = Rnd() % 5;
        for (int s = 0; s < 3; ++s) {
            *(float**)(subA + s * 0x45) = &verts[0][0];
            *(int32_t*)(subA + s * 0x45 + 8) = n;
            *(float**)(subA + s * 0x45 + 0x35) = &outA[0][0];
        }
        memcpy(subB, subA, sizeof subA);
        for (int s = 0; s < 3; ++s) *(float**)(subB + s * 0x45 + 0x35) = &outB[0][0];
        memset(outA, 0, sizeof outA);
        memset(outB, 0, sizeof outB);
        *(uint8_t**)(lodA + 0xc) = subA;
        *(int32_t*)(lodA + 0x14) = 3;
        *(uint8_t**)(lodB + 0xc) = subB;
        *(int32_t*)(lodB + 0x14) = 3;
        *(uint8_t**)(o_a + 0x14) = lodA;
        *(uint8_t**)(o_b + 0x14) = lodB;
        int32_t key = (int32_t)(Rnd() % 3);
        orig(o_a, 0, cam, key, idx);
        Mesh_FaceCamera(o_b, 0, cam, key, idx);
        EXPECT_EQ(memcmp(outA, outB, sizeof outA), 0, "vertices iteration %d", it);
        for (int s = 0; s < 3; ++s)
            EXPECT_EQ(*(void**)(subA + s * 0x45 + 0x31), *(void**)(subB + s * 0x45 + 0x31), "key %d", it);
    }
}

namespace rt3 { void __fastcall Terrain_AverageNormal(uint8_t* t, int, int32_t x, int32_t y, int32_t level); }

DIFFTEST(render_TerrainAverageNormal) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x5331e0;
    static uint8_t ta[0x1700], tb[0x1700];
    static float src[65 * 65 * 3], da[40 * 40 * 3], db[40 * 40 * 3];
    for (float& f : src) f = RndF(-1.0f, 1.0f);
    for (int it = 0; it < 3000; ++it) {
        memset(ta, 0, sizeof ta);
        int32_t level = 1 + Rnd() % 2;
        int32_t w = 16 + Rnd() % 48, h = 16 + Rnd() % 48;
        *(int32_t*)(ta + 0x15d9) = w;
        *(int32_t*)(ta + 0x15d5) = h;
        *(float**)(ta + 0x15ed + level * 4) = src;
        memcpy(tb, ta, sizeof ta);
        *(float**)(ta + 0x15f1 + level * 4) = da;
        *(float**)(tb + 0x15f1 + level * 4) = db;
        memset(da, 0, sizeof da);
        memset(db, 0, sizeof db);
        int32_t x = Rnd() % ((w >> level) + 1), y = Rnd() % ((h >> level) + 1);
        orig(ta, 0, x, y, level);
        Terrain_AverageNormal(tb, 0, x, y, level);
        EXPECT_EQ(memcmp(da, db, sizeof da), 0, "cell %d,%d level %d w %d h %d", x, y, level, w, h);
    }
}

namespace rt3 { void __fastcall Terrain_ShadeVertex(uint8_t* t, int, int32_t x, int32_t y, int32_t); }

DIFFTEST(render_TerrainShadeVertex) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x536420;
    static uint8_t ta[0x1700], tb[0x1700];
    static float nrm[41 * 41 * 3];
    static uint8_t map[40 * 40], det[41 * 41];
    static uint16_t oa[41 * 41], ob[41 * 41];
    for (int it = 0; it < 3000; ++it) {
        for (float& f : nrm) f = RndF(-1.0f, 1.0f);
        for (uint8_t& m : map) m = (Rnd() % 4 == 0) ? (uint8_t)(Rnd() % 3) : 0;
        for (uint8_t& d : det) d = (uint8_t)Rnd();
        memset(ta, 0, sizeof ta);
        int32_t w = 8 + Rnd() % 32, h = 8 + Rnd() % 32;
        *(int32_t*)(ta + 0x15d9) = w;
        *(int32_t*)(ta + 0x15d5) = h;
        *(int32_t*)(ta + 0x15dd) = 40;
        *(int32_t*)(ta + 0x15e1) = 0;
        *(float**)(ta + 0x15f1) = nrm;
        *(uint8_t**)(ta + 0x1655) = map;
        *(uint8_t**)(ta + 0x162d) = det;
        *(float*)(ta + 0x14e0) = RndF(0.0f, 300.0f);
        memcpy(tb, ta, sizeof ta);
        *(uint16_t**)(ta + 0x1619) = oa;
        *(uint16_t**)(tb + 0x1619) = ob;
        memset(oa, 0, sizeof oa);
        memset(ob, 0, sizeof ob);
        int32_t x = Rnd() % w, y = Rnd() % h;
        orig(ta, 0, x, y, 0);
        Terrain_ShadeVertex(tb, 0, x, y, 0);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "vertex %d,%d w %d h %d", x, y, w, h);
    }
}

namespace rt3 { void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img); }

DIFFTEST(render_ImageTo4444) {
    FakeEngine fe;
    fe.e[0x114255] = 1;
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*))0x541210;
    static uint32_t src[33 * 17];
    static uint16_t da[33 * 17], db[33 * 17];
    static uint8_t tex[0x100], ia[0x40], ib[0x40];
    for (int it = 0; it < 500; ++it) {
        for (uint32_t& p : src) p = Rnd();
        tex[0x96] = (uint8_t)(it & 1);
        tex[0x97] = 1;
        memset(ia, 0, sizeof ia);
        *(int32_t*)(ia + 8) = 1 + Rnd() % 33;
        *(int32_t*)(ia + 0xc) = 1 + Rnd() % 17;
        *(uint32_t**)(ia + 0x26) = src;
        memcpy(ib, ia, sizeof ia);
        *(uint16_t**)(ia + 0x2a) = da;
        *(uint16_t**)(ib + 0x2a) = db;
        memset(da, 0, sizeof da);
        memset(db, 0, sizeof db);
        orig(tex, 0, ia);
        Image_To4444(tex, 0, ib);
        EXPECT_EQ(memcmp(da, db, sizeof da), 0, "pixels iteration %d", it);
        *(uint16_t**)(ia + 0x2a) = nullptr;
        *(uint16_t**)(ib + 0x2a) = nullptr;
        *(uint16_t**)(ia + 0x22) = nullptr;
        *(uint16_t**)(ib + 0x22) = nullptr;
        EXPECT_EQ(memcmp(ia, ib, sizeof ia), 0, "header iteration %d", it);
    }
}

namespace rt3 { void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t); }

DIFFTEST(render_AnimsLoadPack) {
    auto orig = (void(__fastcall*)(uint8_t*, int, const char*, int32_t))0x53b090;
    const uint32_t size = 0x42800;
    uint8_t* a = (uint8_t*)VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* b = (uint8_t*)VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_anims.bin");
    for (int it = 0; it < 40; ++it) {
        int32_t ver = it & 1 ? 0x3eb : 0x3ea, n = Rnd() % 60;
        FILE* f = fopen(path, "wb");
        fwrite(&ver, 4, 1, f);
        fwrite(&n, 4, 1, f);
        for (int32_t i = 0; i < n; ++i) {
            uint8_t rec[0xd];
            for (uint8_t& c : rec) c = (uint8_t)Rnd();
            fwrite(rec, 0xd, 1, f);
            if (ver >= 0x3eb) {
                char name[0x3d] = {};
                sprintf(name, "anim%d.%s", i, Rnd() % 2 ? "abc" : "xyz");
                fwrite(name, 0x3d, 1, f);
            }
        }
        fclose(f);
        int32_t slot = Rnd() % 5;
        *(int32_t*)(a + 0x427b8) = slot;
        *(int32_t*)(b + 0x427b8) = slot;
        orig(a, 0, path, 0);
        Anims_LoadPack(b, 0, path, 0);
        ((int(__cdecl*)(int))0x5a440d)(*(int32_t*)(a + 0x204ac + slot * 4));
        ((int(__cdecl*)(int))0x5a440d)(*(int32_t*)(b + 0x204ac + slot * 4));
        EXPECT_EQ(*(int32_t*)(a + 0x427b8), *(int32_t*)(b + 0x427b8), "slot");
        EXPECT_EQ(*(int32_t*)(a + 0x417ec + slot * 4), *(int32_t*)(b + 0x417ec + slot * 4), "count");
        EXPECT_EQ(*(int32_t*)(a + 0x1f404 + slot * 4), *(int32_t*)(b + 0x1f404 + slot * 4), "record size");
        for (int k = 0; k < 32; ++k) {
            int32_t ca = *(int32_t*)(a + 0x223ec + (slot * 32 + k) * 4), cb = *(int32_t*)(b + 0x223ec + (slot * 32 + k) * 4);
            EXPECT_EQ(ca, cb, "bucket %d count", k);
            if (ca == cb && ca > 0)
                EXPECT_EQ(memcmp(*(void**)(a + (slot * 32 + k) * 4), *(void**)(b + (slot * 32 + k) * 4), ca * 0xd), 0, "bucket %d", k);
        }
    }
    DeleteFileA(path);
    VirtualFree(a, 0, MEM_RELEASE);
    VirtualFree(b, 0, MEM_RELEASE);
}

namespace rt3 { void __fastcall Texture_Shrink(uint8_t* o, int, int32_t idx, int32_t fx, int32_t fy); }

DIFFTEST(render_TextureShrink) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x5414e0;
    auto exeMalloc = (void*(__cdecl*)(size_t))0x5a125d;
    static uint8_t oa[0x100], ob[0x100], ia[0x40], ib[0x40];
    for (int it = 0; it < 500; ++it) {
        int32_t w = 1 + Rnd() % 64, h = 1 + Rnd() % 64, fx = 1 + Rnd() % 8, fy = 1 + Rnd() % 8;
        if (w / fx == 0 || h / fy == 0) continue;
        memset(ia, 0, sizeof ia);
        *(int32_t*)(ia + 8) = w;
        *(int32_t*)(ia + 0xc) = h;
        memcpy(ib, ia, sizeof ia);
        uint32_t* p32a = (uint32_t*)exeMalloc(w * h * 4);
        uint32_t* p32b = (uint32_t*)exeMalloc(w * h * 4);
        uint16_t* p16a = (uint16_t*)exeMalloc(w * h * 2);
        uint16_t* p16b = (uint16_t*)exeMalloc(w * h * 2);
        for (int i = 0; i < w * h; ++i) {
            p32a[i] = p32b[i] = Rnd();
            p16a[i] = p16b[i] = (uint16_t)Rnd();
        }
        if (it & 1) { *(uint32_t**)(ia + 0x26) = p32a; *(uint32_t**)(ib + 0x26) = p32b; }
        if (it & 2) { *(uint16_t**)(ia + 0x2a) = p16a; *(uint16_t**)(ib + 0x2a) = p16b; }
        *(uint8_t**)(oa + 0xa0 + 4) = ia;
        *(uint8_t**)(ob + 0xa0 + 4) = ib;
        orig(oa, 0, 1, fx, fy);
        Texture_Shrink(ob, 0, 1, fx, fy);
        EXPECT_EQ(*(int32_t*)(ia + 8), *(int32_t*)(ib + 8), "w");
        EXPECT_EQ(*(int32_t*)(ia + 0xc), *(int32_t*)(ib + 0xc), "h");
        EXPECT_EQ(*(int32_t*)(ia + 4), *(int32_t*)(ib + 4), "bytes");
        int32_t nw = *(int32_t*)(ia + 8), nh = *(int32_t*)(ia + 0xc);
        if (it & 1) EXPECT_EQ(memcmp(*(void**)(ia + 0x26), *(void**)(ib + 0x26), nw * nh * 4), 0, "32-bit %d", it);
        if (it & 2) EXPECT_EQ(memcmp(*(void**)(ia + 0x2a), *(void**)(ib + 0x2a), nw * nh * 2), 0, "16-bit %d", it);
    }
}

namespace rt3 { void __fastcall Texture_MakeBumpMaps(uint8_t* o); }

DIFFTEST(render_BumpMaps) {
    FakeEngine fe;
    fe.e[0x11422d] = 1;
    auto orig = (void(__fastcall*)(uint8_t*))0x542550;
    auto exeMalloc = (void*(__cdecl*)(size_t))0x5a125d;
    static uint8_t oa[0x100], ob[0x100], ia[3][0x40], ib[3][0x40];
    for (int it = 0; it < 200; ++it) {
        memset(oa, 0, sizeof oa);
        oa[4] = 3;
        int32_t w[3], h[3];
        for (int s = 0; s < 3; ++s) {
            oa[0x98 + s] = (uint8_t)(Rnd() % 4 != 0);
            memset(ia[s], 0, 0x40);
            w[s] = 1 + Rnd() % 24;
            h[s] = 1 + Rnd() % 24;
            *(int32_t*)(ia[s] + 8) = w[s];
            *(int32_t*)(ia[s] + 0xc) = h[s];
            *(int32_t*)(ia[s] + 0x10) = Rnd() % 2 ? 0x20 : 0x10;
            *(int32_t*)(ia[s] + 4) = w[s] * h[s] * (*(int32_t*)(ia[s] + 0x10) / 8);
            memcpy(ib[s], ia[s], 0x40);
            uint32_t* pa = (uint32_t*)exeMalloc(w[s] * h[s] * 4);
            uint32_t* pb = (uint32_t*)exeMalloc(w[s] * h[s] * 4);
            for (int i = 0; i < w[s] * h[s]; ++i) pa[i] = pb[i] = Rnd();
            *(uint32_t**)(ia[s] + 0x26) = pa;
            *(uint32_t**)(ib[s] + 0x26) = pb;
            *(uint8_t**)(oa + 0xa0 + s * 4) = ia[s];
        }
        memcpy(ob, oa, sizeof oa);
        for (int s = 0; s < 3; ++s) *(uint8_t**)(ob + 0xa0 + s * 4) = ib[s];
        orig(oa);
        Texture_MakeBumpMaps(ob);
        for (int s = 0; s < 3; ++s) {
            EXPECT_EQ(*(int32_t*)(ia[s] + 4), *(int32_t*)(ib[s] + 4), "size slot %d", s);
            EXPECT_EQ(ia[s][0x21], ib[s][0x21], "done slot %d", s);
            if (ia[s][0x21])
                EXPECT_EQ(memcmp(*(void**)(ia[s] + 0x2a), *(void**)(ib[s] + 0x2a), w[s] * h[s] * 2), 0, "map slot %d it %d", s, it);
        }
    }
}

namespace rt3 { void __fastcall Sky_BuildSunTable(uint8_t* o); }

DIFFTEST(render_SunTable) {
    auto orig = (void(__fastcall*)(uint8_t*))0x545180;
    static uint8_t a[0x86b + 0x33 * 0x90 + 0x10], b[sizeof a];
    memset(a, 0x5a, sizeof a);
    memset(b, 0x5a, sizeof b);
    orig(a);
    Sky_BuildSunTable(b);
    for (size_t i = 0; i < sizeof a; ++i)
        if (a[i] != b[i]) {
            EXPECT_EQ(a[i], b[i], "byte +0x%x (entry %d, field %d)", (unsigned)i, (int)((i - 0x86b + 0x2b) / 0x33),
                      (int)((i - 0x86b + 0x2b) % 0x33) - 0x2b);
        }
}

namespace rt3 {
void __fastcall Font_FitText(uint8_t* font, int, const char* text, char* out, float maxW, int32_t ellipsis, float* width,
                             const char** next, int32_t clipFront);
}

DIFFTEST(render_FontFitText) {
    auto orig = (void(__fastcall*)(uint8_t*, int, const char*, char*, float, int32_t, float*, const char**, int32_t))0x5501a0;
    static uint8_t font[0x40], glyphFont[0x40], table[0x30], sliders[256][0x28];
    static uint32_t entries[256];
    *(int32_t*)(font + 0xc) = 0;
    *(float*)(font + 0x14) = 1.0f;
    *(uint8_t**)(font + 0x18) = table;
    *(float*)(font + 0x2c) = 3.0f;
    *(float*)(glyphFont + 0x2e) = 1.0f;
    *(uint32_t**)(table + 0x24) = entries;
    *(uint16_t*)(table + 0x28) = 256;
    for (int c = 0; c < 256; ++c) {
        *(uint8_t**)(sliders[c]) = glyphFont;
        *(float*)(sliders[c] + 4) = RndF(2.0f, 9.0f);
        *(float*)(sliders[c] + 0xc) = 0.0f;
        *(float*)(sliders[c] + 0x1c) = RndF(0.7f, 1.3f);
        entries[c] = (uint32_t)(uintptr_t)sliders[c];
    }
    static const char alphabet[] = "abcdefghij klm-no/pq\x5crs tuv0123 456789";
    for (int it = 0; it < 4000; ++it) {
        char text[80];
        int n = Rnd() % 70;
        for (int k = 0; k < n; ++k) text[k] = alphabet[Rnd() % (sizeof alphabet - 1)];
        text[n] = 0;
        if (n > 5 && Rnd() % 4 == 0) text[Rnd() % n] = '\n';
        float maxW = RndF(0.0f, 300.0f);
        int32_t ell = Rnd() % 2, clip = Rnd() % 3 == 0;
        char oa[128], ob[128];
        memset(oa, 0x7f, sizeof oa);
        memset(ob, 0x7f, sizeof ob);
        float wa = -1, wb = -1;
        const char *na = nullptr, *nb = nullptr;
        orig(font, 0, text, oa, maxW, ell, &wa, &na, clip);
        Font_FitText(font, 0, text, ob, maxW, ell, &wb, &nb, clip);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "out '%s' max %f ell %d clip %d", text, maxW, ell, clip);
        EXPECT_EQ(*(uint32_t*)&wa, *(uint32_t*)&wb, "width '%s'", text);
        EXPECT_EQ(na, nb, "next '%s'", text);
    }
}

namespace rt3 { uint32_t __fastcall Particle_Color(uint8_t* e, int, uint8_t* p, int32_t mode); }

DIFFTEST(render_ParticleColor) {
    auto orig = (uint32_t(__fastcall*)(uint8_t*, int, uint8_t*, int32_t))0x5554f0;
    static uint8_t e[0xb0], pa[0x80], pb[0x80];
    uint32_t& seed = hook::Global<uint32_t>(0x624324);
    uint32_t saved = seed;
    for (int it = 0; it < 3000; ++it) {
        *(uint32_t*)(e + 0x48) = Rnd();
        *(uint32_t*)(e + 0x4c) = Rnd();
        for (int k = 0; k < 3; ++k) *(float*)(e + 0x3c + k * 4) = RndF(0.0f, 2.0f);
        *(uint32_t*)(e + 0xac) = Rnd() % 2 ? 0x8000000 : 0;
        memset(pa, 0, sizeof pa);
        *(uint32_t*)(pa + 0x74) = Rnd();
        *(int32_t*)(pa + 0x48) = Rnd() % 100;
        *(int32_t*)(pa + 0x4c) = 1 + Rnd() % 100;
        memcpy(pb, pa, sizeof pa);
        int32_t mode = Rnd() % 3;
        uint32_t s0 = Rnd();
        seed = s0;
        uint32_t ra = orig(e, 0, pa, mode);
        uint32_t sa = seed;
        seed = s0;
        uint32_t rb = Particle_Color(e, 0, pb, mode);
        uint32_t sb = seed;
        EXPECT_EQ(ra, rb, "colour mode %d", mode);
        EXPECT_EQ(sa, sb, "seed mode %d", mode);
        EXPECT_EQ(memcmp(pa, pb, sizeof pa), 0, "particle mode %d", mode);
    }
    seed = saved;
}

namespace rt3 { void __stdcall Sprite_ToTriangles(const uint8_t* s, float* out, int32_t* count); }

DIFFTEST(render_SpriteToTriangles) {
    auto orig = (void(__stdcall*)(const uint8_t*, float*, int32_t*))0x564660;
    static uint8_t s[0x60];
    for (int it = 0; it < 5000; ++it) {
        for (int k = 0; k < 0x60; k += 4) *(float*)(s + k) = RndF(-100.0f, 100.0f);
        for (int k = 1; k < 0x1d; k += 4) *(float*)(s + k) = RndF(-100.0f, 100.0f);
        *(uint32_t*)(s + 0x1d) = Rnd();
        *(float*)(s + 0x29) = RndF(-1.0f, 3.0f);
        for (int k = 0; k < 4; ++k) *(float*)(s + 0x45 + k * 4) = (float)(Rnd() % 3);
        float oa[36], ob[36];
        int32_t na = 0, nb = 0;
        orig(s, oa, &na);
        Sprite_ToTriangles(s, ob, &nb);
        EXPECT_EQ(na, nb, "count");
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "vertices iteration %d", it);
    }
}

namespace rt3 { void __stdcall Tri_ToVertices(const uint8_t* s, float* out, int32_t* count); }

DIFFTEST(render_TriToVertices) {
    auto orig = (void(__stdcall*)(const uint8_t*, float*, int32_t*))0x5648c0;
    static uint8_t s[0x60];
    for (int it = 0; it < 5000; ++it) {
        for (int k = 1; k < 0x1d; k += 4) *(float*)(s + k) = RndF(-100.0f, 100.0f);
        for (int k = 0; k < 3; ++k) {
            *(uint32_t*)(s + 0x1d + k * 4) = Rnd();
            *(float*)(s + 0x29 + k * 4) = RndF(-1.0f, 3.0f);
        }
        for (int k = 0; k < 6; ++k) *(float*)(s + 0x45 + k * 4) = RndF(0.0f, 1.0f);
        float oa[18], ob[18];
        int32_t na = 0, nb = 0;
        orig(s, oa, &na);
        Tri_ToVertices(s, ob, &nb);
        EXPECT_EQ(na, nb, "count");
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "vertices iteration %d", it);
    }
}

namespace rt3 { int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt); }

DIFFTEST(render_ParticleAdvance) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, float))0x565f40;
    static uint8_t pa[0x80], pb[0x80];
    int32_t& paused1 = hook::Global<int32_t>(0xd93998);
    int32_t& paused2 = hook::Global<int32_t>(0xd97af4);
    float* wind = (float*)0x625ba8;
    float& limit = hook::Global<float>(0xd97af0);
    int32_t s1 = paused1, s2 = paused2;
    float sw[7], sl = limit;
    memcpy(sw, wind, sizeof sw);
    static const uint32_t kFlags[] = { 0, 0x100, 0x10000000, 0x20080000, 0x40000000, 0x800, 0x40000900, 0x60080800 };
    for (int it = 0; it < 20000; ++it) {
        for (int k = 0; k < 0x80; k += 4) *(float*)(pa + k) = RndF(-5.0f, 5.0f);
        *(int32_t*)(pa + 0x48) = Rnd() % 200 - 20;
        *(int32_t*)(pa + 0x4c) = 1 + Rnd() % 200;
        *(float*)(pa + 0x28) = RndF(0.0f, 1.0f);
        *(uint32_t*)(pa + 0x5c) = kFlags[Rnd() % 8];
        memcpy(pb, pa, sizeof pa);
        paused1 = Rnd() % 4 == 0;
        paused2 = Rnd() % 4 == 0;
        for (int k = 0; k < 7; ++k) wind[k] = RndF(-2.0f, 2.0f);
        limit = RndF(-2.0f, 2.0f);
        float dt = RndF(-1.0f, 30.0f);
        int32_t ra = orig(pa, 0, dt);
        int32_t rb = Particle_Advance(pb, 0, dt);
        EXPECT_EQ(ra, rb, "result flags %x", *(uint32_t*)(pa + 0x5c));
        EXPECT_EQ(memcmp(pa, pb, sizeof pa), 0, "particle flags %x iteration %d", *(uint32_t*)(pa + 0x5c), it);
    }
    paused1 = s1;
    paused2 = s2;
    memcpy(wind, sw, sizeof sw);
    limit = sl;
}

namespace rt3 { void __fastcall Particles_FrameSetup(uint8_t* cam); }

DIFFTEST(render_ParticlesFrameSetup) {
    auto orig = (void(__fastcall*)(uint8_t*))0x566670;
    struct Range { uint32_t a, n; } ranges[] = { { 0xd97abc, 0xe8 }, { 0x625ba8, 0x38 }, { 0x624324, 4 } };
    uint8_t saved[3][0x100], pre[3][0x100], post[3][0x100];
    for (int r = 0; r < 3; ++r) memcpy(saved[r], (void*)ranges[r].a, ranges[r].n);
    static uint8_t cam[0x40];
    for (int it = 0; it < 4000; ++it) {
        *(float*)(cam + 0x10) = RndF(-7.0f, 7.0f);
        *(float*)(cam + 0x14) = RndF(-2.0f, 2.0f);
        hook::Global<int32_t>(0xd97aec) = (int32_t)(Rnd() % 40) - 3;
        hook::Global<int32_t>(0x625bdc) = 16 + Rnd() % 100;
        hook::Global<uint32_t>(0x624324) = Rnd();
        for (int k = 0; k < 4; ++k) hook::Global<float>(0x625bac + k * 4) = RndF(-1.0f, 1.0f);
        hook::Global<float>(0xd97af0) = RndF(-1.0f, 1.0f);
        for (int r = 0; r < 3; ++r) memcpy(pre[r], (void*)ranges[r].a, ranges[r].n);
        orig(cam);
        for (int r = 0; r < 3; ++r) {
            memcpy(post[r], (void*)ranges[r].a, ranges[r].n);
            memcpy((void*)ranges[r].a, pre[r], ranges[r].n);
        }
        Particles_FrameSetup(cam);
        for (int r = 0; r < 3; ++r)
            EXPECT_EQ(memcmp(post[r], (void*)ranges[r].a, ranges[r].n), 0, "globals %x iteration %d", ranges[r].a, it);
    }
    for (int r = 0; r < 3; ++r) memcpy((void*)ranges[r].a, saved[r], ranges[r].n);
}

namespace rt3 { void __fastcall QuadGrid_Weld(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1); }

DIFFTEST(render_QuadGridWeld) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, int32_t))0x567730;
    const int32_t W = 8, H = 6, N = (W + 1) * (H + 1);
    static uint8_t terrain[0x1700], ta[0x1700], tb[0x1700];
    static float heights[N];
    static uint8_t qa[N][0x40], qb[N][0x40];
    static uint8_t* ga[N];
    static uint8_t* gb[N];
    *(int32_t*)(terrain + 0x15d9) = W;
    *(int32_t*)(terrain + 0x15d5) = H;
    *(float**)(terrain + 0x1605) = heights;
    *(uint8_t**)(fe.e + 0x1c) = terrain;
    for (int it = 0; it < 300; ++it) {
        for (float& h : heights) h = RndF(0.0f, 50.0f);
        for (int c = 0; c < N; ++c) {
            int32_t cx = c % (W + 1), cy = c / (W + 1);
            for (int k = 0; k < 0x40; ++k) qa[c][k] = (uint8_t)Rnd();
            static const int32_t ox[4] = { 0, 1, 1, 0 }, oz[4] = { 0, 0, 1, 1 };
            for (int k = 0; k < 4; ++k) {
                *(float*)(qa[c] + k * 12) = (cx + ox[k]) * 92.0f + RndF(-30.0f, 30.0f);
                *(float*)(qa[c] + k * 12 + 4) = RndF(0.0f, 50.0f);
                *(float*)(qa[c] + k * 12 + 8) = (cy + oz[k]) * 92.0f + RndF(-30.0f, 30.0f);
                qa[c][0x31 + k] = (uint8_t)(Rnd() % 4 == 0 ? 2 : Rnd() % 2);
            }
            memcpy(qb[c], qa[c], 0x40);
            bool on = Rnd() % 10 < 7;
            ga[c] = on ? qa[c] : nullptr;
            gb[c] = on ? qb[c] : nullptr;
        }
        memcpy(ta, terrain, sizeof ta);
        memcpy(tb, terrain, sizeof tb);
        *(uint8_t***)(ta + 0x1669) = ga;
        *(uint8_t***)(tb + 0x1669) = gb;
        int32_t x0 = (int32_t)(Rnd() % 5) - 2, y0 = (int32_t)(Rnd() % 4) - 2;
        int32_t x1 = x0 + Rnd() % 12, y1 = y0 + Rnd() % 10;
        orig(ta, 0, x0, y0, x1, y1);
        QuadGrid_Weld(tb, 0, x0, y0, x1, y1);
        EXPECT_EQ(memcmp(qa, qb, sizeof qa), 0, "quads iteration %d rect %d,%d..%d,%d", it, x0, y0, x1, y1);
    }
}

namespace rt3 { void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift); }

namespace {
uint32_t g_fakeTicks;
uint32_t __stdcall FakeTicks() { return g_fakeTicks; }
}

DIFFTEST(render_QuadToVertices) {
    auto orig = (void(__fastcall*)(uint8_t*, uint8_t*, float))0x5679b0;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    uint32_t savedTint = hook::Global<uint32_t>(0xd97bbc);
    static uint8_t qa[0x40], qb[0x40];
    for (int it = 0; it < 5000; ++it) {
        for (uint8_t& c : qa) c = (uint8_t)Rnd();
        for (int k = 0; k < 12; ++k) *(float*)(qa + k * 4) = RndF(-500.0f, 500.0f);
        qa[0x30] = (uint8_t)(Rnd() % 4);
        for (int k = 0; k < 4; ++k) qa[0x31 + k] = (uint8_t)(Rnd() % 3 + 1);
        memcpy(qb, qa, sizeof qa);
        g_fakeTicks = Rnd();
        hook::Global<uint32_t>(0xd97bbc) = Rnd();
        float lift = RndF(-5.0f, 5.0f);
        float oa[36], ob[36];
        orig((uint8_t*)oa, qa, lift);
        Quad_ToVertices((uint8_t*)ob, qb, lift);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "vertices iteration %d", it);
        EXPECT_EQ(memcmp(qa, qb, sizeof qa), 0, "quad iteration %d", it);
    }
    hook::Global<uint32_t>(0xd97bbc) = savedTint;
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
}

namespace rt3 {
void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w,
                                    const float* p);
}

DIFFTEST(render_TerrainPaintLayers) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, float, int32_t, const float*, const float*))0x44a9a0;
    const int32_t W = 12, H = 10, N = (W + 1) * (H + 1);
    static uint8_t ta[0x1700], tb[0x1700], la[4][N], lb[4][N];
    *(int32_t*)(ta + 0x15d9) = W;
    *(int32_t*)(ta + 0x15d5) = H;
    for (int j = 0; j < 4; ++j) *(uint8_t**)(ta + 0x1631 + j * 4) = la[j];
    memcpy(tb, ta, sizeof ta);
    for (int j = 0; j < 4; ++j) *(uint8_t**)(tb + 0x1631 + j * 4) = lb[j];
    for (int it = 0; it < 20000; ++it) {
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < N; ++k) la[j][k] = (uint8_t)Rnd();
        memcpy(lb, la, sizeof la);
        float w[4], p[4];
        for (int k = 0; k < 4; ++k) {
            w[k] = RndF(-0.2f, 1.2f);
            p[k] = RndF(-0.1f, 0.6f);
        }
        int32_t x = (int32_t)(Rnd() % (W + 2)) - 1, y = (int32_t)(Rnd() % (H + 2)) - 1, mode = Rnd() % 5;
        float s = RndF(-0.5f, 1.5f);
        orig(ta, 0, x, y, s, mode, w, p);
        Terrain_PaintLayers(tb, 0, x, y, s, mode, w, p);
        EXPECT_EQ(memcmp(la, lb, sizeof la), 0, "layers mode %d at %d,%d iteration %d", mode, x, y, it);
    }
}

namespace rt3 { void __fastcall Box_EmitVertices(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t color, int32_t noTop); }

DIFFTEST(render_BoxEmitVertices) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*, int32_t*, uint32_t, int32_t))0x539fb0;
    static uint8_t oa[0x200], ob[0x200], roof[0xc0], va[0x18 * 48], vb[0x18 * 48];
    for (int it = 0; it < 3000; ++it) {
        for (uint8_t& c : oa) c = (uint8_t)Rnd();
        for (uint8_t& c : roof) c = (uint8_t)Rnd();
        *(uint8_t**)(oa + 0x1e3) = roof;
        oa[0x44] = (uint8_t)(Rnd() % 5);
        memcpy(ob, oa, sizeof oa);
        memset(va, 0xcd, sizeof va);
        memset(vb, 0xcd, sizeof vb);
        int32_t ca = (int32_t)(Rnd() % 10), cb = ca;
        uint32_t color = Rnd();
        int32_t noTop = Rnd() % 2;
        orig(oa, 0, va, &ca, color, noTop);
        Box_EmitVertices(ob, 0, vb, &cb, color, noTop);
        EXPECT_EQ(ca, cb, "count iteration %d", it);
        EXPECT_EQ(memcmp(va, vb, sizeof va), 0, "vertices iteration %d", it);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "object iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Terrain_PaintBrush(uint8_t* t, int, float cx, float cy, float r, float strength, int32_t hard,
                                   int32_t mode, const float* w, const float* p, uint8_t* mask);
}

DIFFTEST(render_TerrainPaintBrush) {
    auto orig = (void(__fastcall*)(uint8_t*, int, float, float, float, float, int32_t, int32_t, const float*, const float*,
                                   uint8_t*))0x44dfe0;
    const int32_t W = 16, H = 12, N = (W + 1) * (H + 1);
    static uint8_t ta[0x2200], tb[0x2200], la[4][N], lb[4][N], ma[N], mb[N];
    *(int32_t*)(ta + 0x15d9) = W;
    *(int32_t*)(ta + 0x15d5) = H;
    *(int32_t*)(ta + 0x2155) = W;
    *(int32_t*)(ta + 0x2159) = H;
    for (int j = 0; j < 4; ++j) *(uint8_t**)(ta + 0x1631 + j * 4) = la[j];
    memcpy(tb, ta, sizeof ta);
    for (int j = 0; j < 4; ++j) *(uint8_t**)(tb + 0x1631 + j * 4) = lb[j];
    for (int it = 0; it < 3000; ++it) {
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < N; ++k) la[j][k] = (uint8_t)Rnd();
        for (int k = 0; k < N; ++k) ma[k] = Rnd() % 3 == 0 ? 0xff : (uint8_t)(Rnd() % 200);
        memcpy(lb, la, sizeof la);
        memcpy(mb, ma, sizeof ma);
        float w[4], p[4];
        for (int k = 0; k < 4; ++k) {
            w[k] = RndF(-0.2f, 1.2f);
            p[k] = RndF(-0.1f, 0.6f);
        }
        float cx = RndF(-3.0f, W + 3.0f), cy = RndF(-3.0f, H + 3.0f), r = RndF(0.0f, 6.0f), s = RndF(-1.5f, 1.5f);
        int32_t hard = Rnd() % 2, mode = Rnd() % 4;
        orig(ta, 0, cx, cy, r, s, hard, mode, w, p, ma);
        Terrain_PaintBrush(tb, 0, cx, cy, r, s, hard, mode, w, p, mb);
        EXPECT_EQ(memcmp(la, lb, sizeof la), 0, "layers iteration %d", it);
        EXPECT_EQ(memcmp(ma, mb, sizeof ma), 0, "mask iteration %d", it);
    }
}

namespace rt3 { void __fastcall Image_SoftenPixel(uint8_t* img, int, int32_t x, int32_t y); }

DIFFTEST(render_ImageSoftenPixel) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4f8370;
    const int32_t W = 9, H = 7;
    static uint32_t pa[W * H], pb[W * H];
    static uint8_t ia[0x120], ib[0x120];
    *(int32_t*)(ia + 0xfc) = W;
    *(int32_t*)(ia + 0x100) = H;
    memcpy(ib, ia, sizeof ia);
    *(uint32_t**)(ia + 0x114) = pa;
    *(uint32_t**)(ib + 0x114) = pb;
    for (int it = 0; it < 20000; ++it) {
        for (uint32_t& v : pa) v = Rnd();
        memcpy(pb, pa, sizeof pa);
        int32_t x = (int32_t)(Rnd() % (W + 2)) - 1, y = (int32_t)(Rnd() % (H + 2)) - 1;
        orig(ia, 0, x, y);
        Image_SoftenPixel(ib, 0, x, y);
        EXPECT_EQ(memcmp(pa, pb, sizeof pa), 0, "pixels at %d,%d", x, y);
    }
}

namespace rt3 { void Language_Select(); }

namespace {
uint16_t g_fakeLang;
uint16_t __stdcall FakeLangId() { return g_fakeLang; }
}

DIFFTEST(game_LanguageSelect) {
    static const char* names[] = { "spanish", "english", "chinese", "german", "portug", "italian",
                                   "french", "russian", "japan", "polish", "hungar" };
    // Runs in a scratch folder (cwd switched) holding data\language\<name>\RT3.lng
    // for every language, so the game folder is never touched.
    char cwd[MAX_PATH], root[MAX_PATH], path[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cwd);
    GetTempPathA(MAX_PATH, root);
    strcat(root, "rt3re_lang");
    CreateDirectoryA(root, nullptr);
    if (!SetCurrentDirectoryA(root)) return;
    CreateDirectoryA("data", nullptr);
    CreateDirectoryA("data\\language", nullptr);
    for (const char* n : names) {
        sprintf(path, "data\\language\\%s", n);
        CreateDirectoryA(path, nullptr);
        strcat(path, "\\RT3.lng");
        if (FILE* f = fopen(path, "wb")) fclose(f);
    }
    void** iat = (void**)0x5c8218;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeLangId;
    char** lang = (char**)0x6d3c90;
    char* savedLang = *lang;
    auto orig = (void(__cdecl*)())0x518fa0;
    for (uint32_t id = 0; id < 0x6000; ++id) {
        g_fakeLang = (uint16_t)id;
        *lang = nullptr;
        orig();
        char a[64] = {};
        if (*lang) strcpy(a, *lang);
        *lang = nullptr;
        Language_Select();
        const char* b = *lang ? *lang : "";
        EXPECT_EQ(strcmp(a, b), 0, "lang id %x: %s vs %s", id, a, b);
        if (id == 0x40c) EXPECT_EQ(strcmp(a, "french"), 0, "fixture: id 40c gave '%s'", a);
    }
    *lang = savedLang;
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    for (const char* n : names) {
        sprintf(path, "data\\language\\%s\\RT3.lng", n);
        DeleteFileA(path);
        sprintf(path, "data\\language\\%s", n);
        RemoveDirectoryA(path);
    }
    RemoveDirectoryA("data\\language");
    RemoveDirectoryA("data");
    SetCurrentDirectoryA(cwd);
    RemoveDirectoryA(root);
}

#include "game/collection.h"
namespace rt3 { int32_t __fastcall Cities_AssignBlocks(Collection* cities); }

DIFFTEST(game_CitiesAssignBlocks) {
    auto orig = (void(__fastcall*)(Collection*))0x421730;
    const int32_t W = 14, H = 11, VS = W * 4 + 4, VN = VS * (H * 4 + 4);
    static uint8_t ma[0x2210], mb[0x2210], verts[VN * 3], ba[W * H * 4], bb[W * H * 4];
    uint8_t*& map = hook::Global<uint8_t*>(0x62c120);
    uint8_t* savedMap = map;
    for (int it = 0; it < 200; ++it) {
        Collection ca, cb;
        memset(&ca, 0, sizeof ca);
        memset(&cb, 0, sizeof cb);
        Collection_Construct(&ca, 0, 1, 0x300, 8, 8, 0, 0, 0);
        Collection_Construct(&cb, 0, 1, 0x300, 8, 8, 0, 0, 0);
        int32_t n = 1 + Rnd() % 12;
        for (int32_t i = 0; i < n; ++i) {
            uint8_t rec[0x300] = {};
            *(float*)(rec + 0x1e2) = RndF(-50.0f, W * 4 * 92.0f + 50.0f);
            *(float*)(rec + 0x1ea) = RndF(-50.0f, H * 4 * 92.0f + 50.0f);
            *(int32_t*)(rec + 0x23e) = Rnd() % 5 == 0;
            *(int32_t*)(rec + 0x256) = (int32_t)(Rnd() % 20) - 2;
            uint32_t id = Collection_Allocate(&ca, 0, rec, 0x300, 0, 0);
            Collection_Allocate(&cb, 0, rec, 0x300, 0, 0);
            *(uint16_t*)((uint8_t*)Collection_Get(&ca, 0, id) + 0x23a) = (uint16_t)id;
            *(uint16_t*)((uint8_t*)Collection_Get(&cb, 0, id) + 0x23a) = (uint16_t)id;
        }
        for (uint8_t& v : verts) v = (uint8_t)Rnd();
        for (int32_t k = 0; k < W * H; ++k) {
            ba[k * 4] = (uint8_t)(1 + Rnd() % n);
            ba[k * 4 + 1] = ba[k * 4 + 2] = ba[k * 4 + 3] = (uint8_t)Rnd();
        }
        memcpy(bb, ba, sizeof ba);
        memset(ma, 0, sizeof ma);
        *(int32_t*)(ma + 0x214d) = W;
        *(int32_t*)(ma + 0x2151) = H;
        *(int32_t*)(ma + 0x15dd) = VS;
        *(uint8_t**)(ma + 0x165d) = verts;
        memcpy(mb, ma, sizeof ma);
        *(uint8_t**)(ma + 0x212d) = ba;
        *(uint8_t**)(mb + 0x212d) = bb;
        map = ma;
        orig(&ca);
        map = mb;
        Cities_AssignBlocks(&cb);
        map = savedMap;
        EXPECT_EQ(memcmp(ba, bb, sizeof ba), 0, "blocks iteration %d", it);
        for (int32_t i = 1; i <= n; ++i)
            EXPECT_EQ(memcmp(Collection_Get(&ca, 0, i), Collection_Get(&cb, 0, i), 0x300), 0, "city %d iteration %d", i, it);
        Collection_Destruct(&ca);
        Collection_Destruct(&cb);
    }
}

namespace rt3 {
int32_t __fastcall Route_ProbeLine(uint8_t* self, int, float x0, float y0, float angle, float x1, float y1,
                                   uint32_t company, int32_t minH, float* peak, int32_t* wet, float* dist);
}

DIFFTEST(game_RouteProbeLine) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, float, float, float, float, float, uint32_t, int32_t, float*, int32_t*,
                                      float*))0x4a2640;
    const int32_t N = 20, V = N + 1;
    static uint8_t map[0x2200], world[0x20], selfA[0x210], selfB[0x210], regions[N * N], recs[V * V * 3];
    static float heights[V * V];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15d5) = N;
    *(int32_t*)(map + 0x15dd) = V;
    *(float**)(map + 0x1605) = heights;
    *(uint8_t**)(map + 0x165d) = recs;
    *(uint8_t**)(map + 0x2131) = regions;
    memset(regions, 8, sizeof regions);
    Collection types;
    memset(&types, 0, sizeof types);
    Collection_Construct(&types, 0, 1, 0x40, 4, 4, 0, 0, 0);
    uint8_t rec[0x40] = {};
    *(int32_t*)(rec + 0x31) = 1;
    uint32_t typeId = Collection_Allocate(&types, 0, rec, 0x40, 0, 0);
    memset(regions, (int)(typeId << 3), sizeof regions);
    EXPECT_EQ(Collection_Get(&types, 0, typeId) != nullptr, true, "type id %u", typeId);
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t*& gworld = hook::Global<uint8_t*>(0x6cec78);
    Collection*& gtypes = hook::Global<Collection*>(0x6cfc9c);
    uint8_t* sm = gmap;
    uint8_t* sw = gworld;
    Collection* st = gtypes;
    gmap = map;
    gworld = world;
    gtypes = &types;
    int hits[2] = {};
    for (int it = 0; it < 4000; ++it) {
        float x0 = RndF(1.0f, N - 1.0f), y0 = RndF(1.0f, N - 1.0f), x1 = RndF(0.0f, N - 0.01f), y1 = RndF(0.0f, N - 0.01f);
        float slope = RndF(-1.0f, 6.0f), noise = RndF(0.0f, 4.0f);
        for (int k = 0; k < V * V; ++k)
            heights[k] = 80.0f - slope * (float)sqrt((double)((k % V - x0) * (k % V - x0) + (k / V - y0) * (k / V - y0))) +
                         RndF(0.0f, noise);
        int32_t wx = (int32_t)(Rnd() % N), wy = (int32_t)(Rnd() % N), wr = (int32_t)(Rnd() % 5);
        for (int k = 0; k < V * V; ++k) {
            bool water = abs(k % V - wx) <= wr && abs(k / V - wy) <= wr;
            recs[k * 3] = (uint8_t)(water ? 1 + Rnd() % 5 : Rnd() % 12 == 0 ? Rnd() % 6 : 0);
            recs[k * 3 + 1] = (uint8_t)(Rnd() % 16 == 0 ? Rnd() : 0);
            recs[k * 3 + 2] = (uint8_t)Rnd();
        }
        *(uint16_t*)(world + 0xd) = (uint16_t)(1850 + Rnd() % 80);
        float ang = Rnd() % 2 ? (float)atan2((double)y0 - y1, (double)x0 - x1) : RndF(-4.0f, 4.0f);
        int32_t minH = (int32_t)(Rnd() % 90);
        float pa = RndF(0.0f, 9.0f), pb = pa, da = -1.0f, db = -1.0f;
        int32_t wa = 7, wb = 7;
        memset(selfA, 0, sizeof selfA);
        memset(selfB, 0, sizeof selfB);
        int32_t ra = orig(selfA, 0, x0, y0, ang, x1, y1, 0, minH, &pa, &wa, &da);
        int32_t rb = Route_ProbeLine(selfB, 0, x0, y0, ang, x1, y1, 0, minH, &pb, &wb, &db);
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        hits[ra & 1]++;
        EXPECT_EQ(*(uint32_t*)&pa, *(uint32_t*)&pb, "peak iteration %d", it);
        EXPECT_EQ(wa, wb, "wet iteration %d", it);
        EXPECT_EQ(*(uint32_t*)&da, *(uint32_t*)&db, "dist iteration %d", it);
        EXPECT_EQ(memcmp(selfA, selfB, sizeof selfA), 0, "steps iteration %d", it);
    }
    gmap = sm;
    gworld = sw;
    gtypes = st;
    Collection_Destruct(&types);
    EXPECT_EQ(hits[0] > 100 && hits[1] > 100, true, "coverage: %d fails, %d successes", hits[0], hits[1]);
}

namespace rt3 {
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin);
}

DIFFTEST(render_MeshEmitTriangles) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, uint8_t*, int32_t*, const float*, const float*))0x52f500;
    static float verts[30][3];
    static uint8_t tris[12 * 0x4c], mesh[0x20], oa[0x200], ob[0x200], va[12 * 0x60], vb[12 * 0x60];
    uint8_t*& gt = hook::Global<uint8_t*>(0xcc3b64);
    int32_t& gc = hook::Global<int32_t>(0xcc3b5c);
    uint8_t* st = gt;
    int32_t sc = gc;
    int hits = 0;
    for (int it = 0; it < 6000; ++it) {
        for (auto& v : verts) for (float& c : v) c = RndF(-10.0f, 10.0f);
        for (uint8_t& b : tris) b = (uint8_t)Rnd();
        for (int t = 0; t < 12; ++t)
            for (int k = 0; k < 3; ++k) *(float**)(tris + t * 0x4c + k * 4) = verts[Rnd() % 30];
        *(uint8_t**)(mesh + 4) = tris;
        *(int32_t*)(mesh + 0xc) = (int32_t)(Rnd() % 13) - 1;
        for (uint8_t& b : oa) b = (uint8_t)Rnd();
        for (int k = 0; k < 32; ++k) *(float*)(oa + 0x8e + k * 4) = RndF(-1.5f, 1.5f);
        for (int k = 0; k < 3; ++k) *(float*)(oa + 0x1e2 + k * 4) = RndF(-3.0f, 3.0f);
        if (it & 1) {
            for (int k = 0; k < 16; ++k) *(float*)(oa + 0x8e + k * 4) = k % 5 == 0 ? 1.0f : 0.0f;
            for (int k = 0; k < 3; ++k) *(float*)(oa + 0x1e2 + k * 4) = 0.0f;
            oa[0x20] = (uint8_t)((oa[0x20] | 1) & ~4);
        }
        *(uint8_t**)(oa + 0x18) = mesh;
        memcpy(ob, oa, sizeof oa);
        float origin[3] = { RndF(-20.0f, 20.0f), RndF(-20.0f, 20.0f), RndF(-20.0f, 20.0f) };
        float dir[3];
        const float* aim = verts[Rnd() % 30];
        for (int k = 0; k < 3; ++k) dir[k] = Rnd() % 2 ? aim[k] - origin[k] : RndF(-1.0f, 1.0f);
        bool ray = Rnd() % 4 != 0;
        memset(va, 0xcd, sizeof va);
        memset(vb, 0xcd, sizeof vb);
        int32_t ca = 5, cb = 5;
        int32_t ra = orig(oa, 0, va, &ca, ray ? dir : nullptr, origin);
        uint8_t* ta = gt;
        int32_t na = gc;
        int32_t rb = Mesh_EmitPickTriangles(ob, 0, vb, &cb, ray ? dir : nullptr, origin);
        hits += ra;
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        EXPECT_EQ(ca, cb, "count iteration %d", it);
        EXPECT_EQ(ta == gt && na == gc, true, "globals iteration %d", it);
        EXPECT_EQ(memcmp(va, vb, sizeof va), 0, "vertices iteration %d", it);
        EXPECT_EQ(memcmp(oa, ob, sizeof oa), 0, "object iteration %d", it);
    }
    EXPECT_EQ(hits > 50, true, "only %d hits", hits);
    gt = st;
    gc = sc;
}

namespace rt3 { void __fastcall Renderer_SetShaderConstants(uint8_t* r); }

namespace {
struct ConstLog {
    uint32_t calls[40][7];
    int n;
};
ConstLog g_constLog;
int32_t __stdcall RecordVs(void*, uint32_t reg, const uint32_t* d, uint32_t n) {
    for (uint32_t k = 0; k < n && g_constLog.n < 40; ++k) {
        uint32_t* c = g_constLog.calls[g_constLog.n++];
        c[0] = 0x13c, c[1] = reg + k, c[2] = n;
        memcpy(c + 3, d + k * 4, 16);
    }
    return 0;
}
int32_t __stdcall RecordPs(void*, uint32_t reg, const uint32_t* d, uint32_t n) {
    uint32_t* c = g_constLog.calls[g_constLog.n++];
    c[0] = 0x16c, c[1] = reg, c[2] = n;
    memcpy(c + 3, d, 16);
    return 0;
}
}

DIFFTEST(render_SetShaderConstants) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*))0x546d40;
    static void* vtbl[0x60];
    vtbl[0x13c / 4] = (void*)&RecordVs;
    vtbl[0x16c / 4] = (void*)&RecordPs;
    static void* dev[1] = { vtbl };
    static uint8_t r[0x3800], caps[0x40], terrain[0x1700];
    *(uint8_t**)(fe.e + 0x1c) = terrain;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 0; k < 0x3800; k += 4) *(float*)(r + k) = RndF(-3.0f, 3.0f);
        for (int k = 0; k < 0x1700; k += 4) *(int32_t*)(terrain + k) = 8 + Rnd() % 300;
        *(void**)(r + 0x25df) = dev;
        caps[0x1f] = (uint8_t)(Rnd() % 2);
        *(uint8_t**)(r + 0x83c) = caps;
        g_constLog.n = 0;
        orig(r);
        ConstLog a = g_constLog;
        g_constLog.n = 0;
        Renderer_SetShaderConstants(r);
        EXPECT_EQ(a.n, g_constLog.n, "call count");
        EXPECT_EQ(memcmp(a.calls, g_constLog.calls, sizeof a.calls[0] * a.n), 0, "constants iteration %d", it);
    }
}

namespace rt3 { void __fastcall Renderer_SetupWaterStages(uint8_t* r); }

namespace {
uint32_t g_devLog[64][20];
int g_devN;
uint32_t* DevRec(uint32_t op) {
    uint32_t* c = g_devLog[g_devN < 63 ? g_devN++ : 63];
    memset(c, 0, sizeof g_devLog[0]);
    c[0] = op;
    return c;
}
int32_t __stdcall DevTss(void*, uint32_t a, uint32_t b, uint32_t v) { uint32_t* c = DevRec(0xfc); c[1] = a, c[2] = b, c[3] = v; return 0; }
int32_t __stdcall DevVs(void*, uint32_t v) { DevRec(0x130)[1] = v; return 0; }
int32_t __stdcall DevPs(void*, uint32_t v) { DevRec(0x160)[1] = v; return 0; }
int32_t __stdcall DevXf(void*, uint32_t s, const uint32_t* m) { uint32_t* c = DevRec(0x94); c[1] = s; memcpy(c + 2, m, 64); return 0; }
}

DIFFTEST(render_SetupWaterStages) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*))0x5478a0;
    static void* vtbl[0x60];
    vtbl[0xfc / 4] = (void*)&DevTss;
    vtbl[0x130 / 4] = (void*)&DevVs;
    vtbl[0x160 / 4] = (void*)&DevPs;
    vtbl[0x94 / 4] = (void*)&DevXf;
    static void* dev[1] = { vtbl };
    static uint8_t r[0x2700], tex[0x100];
    static uint8_t* texp = tex;
    for (int it = 0; it < 400; ++it) {
        for (uint8_t& b : r) b = (uint8_t)Rnd();
        static uint8_t img[0x20];
        memset(tex, 0, sizeof tex);
        tex[4] = (uint8_t)(Rnd() % 2);
        tex[0x98] = 1;
        *(uint8_t**)(tex + 0xa0) = img;
        *(int32_t*)(img + 0xc) = 1 + Rnd() % 512;
        *(void**)(r + 0x25df) = dev;
        *(uint8_t***)(r + 0x2593) = &texp;
        *(float*)(r + 0x2537) = RndF(1.0f, 500.0f);
        *(int32_t*)(r + 0x25cb) = 1 + Rnd() % 50;
        fe.e[0x11422d] = (uint8_t)(it & 1);
        g_devN = 0;
        orig(r);
        static uint32_t a[64][20];
        int na = g_devN;
        memcpy(a, g_devLog, sizeof a);
        g_devN = 0;
        Renderer_SetupWaterStages(r);
        EXPECT_EQ(na, g_devN, "calls");
        EXPECT_EQ(memcmp(a, g_devLog, sizeof a[0] * na), 0, "device calls iteration %d", it);
    }
}

namespace rt3 { void __fastcall Renderer_SetupViews(uint8_t* r, int, uint8_t* cam, uint8_t reflect); }

DIFFTEST(render_SetupViews) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*, uint8_t))0x548350;
    const int32_t W = 30;
    static uint8_t ra[0x3720], rb[0x3720], cam[0x60], terrain[0x1700];
    static float heights[(W + 1) * (W + 1)];
    *(int32_t*)(terrain + 0x15d9) = W;
    *(int32_t*)(terrain + 0x15d5) = W;
    *(float**)(terrain + 0x1605) = heights;
    *(uint8_t**)(fe.e + 0x1c) = terrain;
    uint8_t cache[0x1c], saved[0x1c];
    memcpy(saved, (void*)0x625438, 0x1c);
    for (int it = 0; it < 3000; ++it) {
        for (float& h : heights) h = RndF(0.0f, 100.0f);
        *(int32_t*)(fe.e + 0x11420e) = 1 + Rnd() % 40;
        for (int k = 0; k < 0x60; k += 4) *(float*)(cam + k) = RndF(0.0f, W * 92.0f);
        for (int k = 0x2c; k < 0x38; k += 4) *(float*)(cam + k) = RndF(-1.0f, 1.0f);
        *(float*)(cam + 0x20) = RndF(50.0f, 3000.0f);
        if (it % 7 == 0) memcpy(cam + 4, cam + 0x1c, 12);
        for (uint8_t& b : ra) b = (uint8_t)Rnd();
        *(float*)(ra + 0x2537) = RndF(0.0f, 60.0f);
        *(int32_t*)(ra + 0x36f3) = Rnd() % 4;
        for (int k = 0; k < 4; ++k) {
            *(float*)(ra + k * 0x33 + 0x867) = RndF(-500.0f, 500.0f);
            *(float*)(ra + k * 0x33 + 0x86b) = RndF(-100.0f, 4000.0f);
            *(float*)(ra + k * 0x33 + 0x86f) = RndF(-500.0f, 500.0f);
        }
        memcpy(rb, ra, sizeof ra);
        if (Rnd() % 3) for (int k = 0; k < 0x1c; ++k) ((uint8_t*)0x625438)[k] = (uint8_t)Rnd();
        memcpy(cache, (void*)0x625438, 0x1c);
        uint8_t refl = (uint8_t)(Rnd() % 2);
        orig(ra, 0, cam, refl);
        uint8_t ca[0x1c];
        memcpy(ca, (void*)0x625438, 0x1c);
        memcpy((void*)0x625438, cache, 0x1c);
        Renderer_SetupViews(rb, 0, cam, refl);
        EXPECT_EQ(memcmp(ra, rb, sizeof ra), 0, "renderer iteration %d", it);
        EXPECT_EQ(memcmp(ca, (void*)0x625438, 0x1c), 0, "cache iteration %d", it);
    }
    memcpy((void*)0x625438, saved, 0x1c);
}

namespace rt3 { void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night); }

namespace {
int32_t __stdcall DevLight(void*, uint32_t i, const uint32_t* l) { uint32_t* c = DevRec(0xb0); c[1] = i; memcpy(c + 2, l, 0x48); return 0; }
int32_t __stdcall DevRs(void*, uint32_t s, uint32_t v) { uint32_t* c = DevRec(0xc8); c[1] = s, c[2] = v; return 0; }
}

DIFFTEST(render_SetTimeOfDay) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x54bea0;
    static void* vtbl[0x60];
    vtbl[0xb0 / 4] = (void*)&DevLight;
    vtbl[0xc8 / 4] = (void*)&DevRs;
    static void* dev[1] = { vtbl };
    static uint8_t ra[0x3720], rb[0x3720], caps[0x40];
    static uint32_t a[64][20];
    for (int it = 0; it < 3000; ++it) {
        for (int k = 0; k < 0x3720; k += 4) *(float*)(ra + k) = RndF(-2.0f, 2.0f);
        for (int k = 0x840; k < 0x840 + 0x90 * 0x33 + 0x33; ++k) ra[k] = (uint8_t)Rnd();
        for (int f = 0; f < 0x91; ++f)
            for (int k = 0; k < 0x33; k += 4)
                if (k != 0x20) *(float*)(ra + 0x840 + f * 0x33 + k) = RndF(-2.0f, 2.0f);
        *(void**)(ra + 0x25df) = dev;
        for (uint8_t& c : caps) c = (uint8_t)(Rnd() % 2);
        caps[0x22] = (uint8_t)(Rnd() % 6 != 0);
        *(uint8_t**)(ra + 0x83c) = caps;
        fe.e[0x11422e] = (uint8_t)(Rnd() % 2);
        memcpy(rb, ra, sizeof ra);
        int32_t secs = (int32_t)(Rnd() % 90000) - 1000, night = (int32_t)(Rnd() % 300) - 50;
        g_devN = 0;
        orig(ra, 0, secs, night);
        int na = g_devN;
        memcpy(a, g_devLog, sizeof a);
        g_devN = 0;
        Renderer_SetTimeOfDay(rb, 0, secs, night);
        EXPECT_EQ(memcmp(ra, rb, sizeof ra), 0, "renderer iteration %d secs %d", it, secs);
        EXPECT_EQ(na, g_devN, "calls");
        EXPECT_EQ(memcmp(a, g_devLog, sizeof a[0] * na), 0, "device iteration %d", it);
    }
}

namespace rt3 { int32_t __fastcall Emitter_Update(uint8_t* e, int, int32_t dt); }

namespace {
struct EmitterFix {
    uint8_t e[0x200], obj[0x200], parts[12][0x80];
    void* nodes[12][2];
    void* live[3];
    void* freel[3];
    void Build(const uint8_t* proto, const uint8_t (*pp)[0x80], int n, const uint8_t* o) {
        memcpy(e, proto, sizeof e);
        memcpy(obj, o, sizeof obj);
        memcpy(parts, pp, sizeof parts);
        live[0] = live[1] = nullptr;
        live[2] = (void*)(intptr_t)n;
        for (int k = n - 1; k >= 0; --k) {
            nodes[k][1] = parts[k];
            nodes[k][0] = live[0];
            live[0] = nodes[k];
            if (!live[1]) live[1] = nodes[k];
        }
        freel[0] = freel[1] = nullptr;
        freel[2] = 0;
        *(void**)(e + 4) = live;
        *(void**)(e + 0) = freel;
        *(void**)(e + 0xc) = Rnd() % 4 ? obj : nullptr;
    }
};
EmitterFix g_ea, g_eb;
}

DIFFTEST(render_EmitterUpdate) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, int32_t))0x555120;
    static uint8_t proto[0x200], obj[0x200], pp[12][0x80], frameObj[0x40], track[0x100];
    uint8_t*& gframe = hook::Global<uint8_t*>(0x6d4030);
    uint8_t* sf = gframe;
    int32_t& frozen = hook::Global<int32_t>(0xd939a0);
    uint8_t*& gtrack = hook::Global<uint8_t*>(0xd939a4);
    int32_t sfz = frozen;
    uint8_t* st = gtrack;
    gframe = frameObj;
    for (int it = 0; it < 3000; ++it) {
        for (int k = 0; k < 0x200; k += 4) *(float*)(proto + k) = RndF(-5.0f, 5.0f);
        for (int k = 0; k < 0x200; k += 4) *(float*)(obj + k) = RndF(-50.0f, 50.0f);
        static const uint32_t fl[] = { 0, 1, 0x10, 0x11, 0x8, 0x800009, 0x2000001, 0x19 };
        *(uint32_t*)(proto + 0xac) = fl[Rnd() % 8];
        *(int32_t*)(proto + 0x1f9) = Rnd() % 3;
        *(int32_t*)(frameObj + 0x2c) = Rnd() % 3;
        *(int32_t*)(proto + 0x118) = Rnd() % 6;
        *(int32_t*)(proto + 0x124) = 1 + Rnd() % 4;
        *(int32_t*)(proto + 0x128) = 1 + Rnd() % 60;
        for (int k = 0; k < 4; ++k) *(int32_t*)(proto + 0x110 + k * 4) = Rnd() % 64;
        for (auto& p : pp) {
            for (int k = 0; k < 0x80; k += 4) *(float*)(p + k) = RndF(-5.0f, 5.0f);
            *(int32_t*)(p + 0x48) = (int32_t)(Rnd() % 200) - 20;
            *(int32_t*)(p + 0x4c) = 1 + Rnd() % 200;
            *(uint32_t*)(p + 0x5c) = Rnd() % 2 ? 0 : 0x100;
            *(int32_t*)(p + 0x54) = Rnd() % 50;
            *(int32_t*)(p + 0x58) = Rnd() % 2 ? -100000000 : 2000000000;
        }
        frozen = Rnd() % 4 == 0;
        gtrack = Rnd() % 2 ? track : nullptr;
        int n = Rnd() % 13;
        g_seed ^= 0;
        uint32_t s0 = g_seed;
        g_ea.Build(proto, pp, n, obj);
        g_seed = s0;
        g_eb.Build(proto, pp, n, obj);
        int32_t dt = (int32_t)(Rnd() % 40) - 3;
        int32_t ra = orig(g_ea.e, 0, dt);
        int32_t rb = Emitter_Update(g_eb.e, 0, dt);
        EXPECT_EQ(ra, rb, "result");
        for (int k = 0; k < 12; ++k) {
            *(int32_t*)(g_eb.parts[k] + 0x58) = *(int32_t*)(g_ea.parts[k] + 0x58);
        }
        EXPECT_EQ(memcmp(g_ea.parts, g_eb.parts, sizeof g_ea.parts), 0, "particles iteration %d", it);
        EXPECT_EQ(memcmp(g_ea.e + 0x10, g_eb.e + 0x10, 0x1f0), 0, "emitter iteration %d", it);
        // List shapes: compare by node index.
        auto idx = [](EmitterFix& f, void* p) { return p ? (int)(((void**)p - &f.nodes[0][0]) / 2) : -1; };
        EXPECT_EQ(idx(g_ea, g_ea.live[0]), idx(g_eb, g_eb.live[0]), "live head");
        EXPECT_EQ(idx(g_ea, g_ea.live[1]), idx(g_eb, g_eb.live[1]), "live tail");
        EXPECT_EQ(idx(g_ea, g_ea.freel[0]), idx(g_eb, g_eb.freel[0]), "free head");
        EXPECT_EQ(g_ea.live[2] == g_eb.live[2] && g_ea.freel[2] == g_eb.freel[2], true, "counts");
        for (int k = 0; k < 12; ++k) EXPECT_EQ(idx(g_ea, g_ea.nodes[k][0]), idx(g_eb, g_eb.nodes[k][0]), "next %d", k);
    }
    gframe = sf;
    frozen = sfz;
    gtrack = st;
}

namespace rt3 { void __fastcall Particle_EmitQuads(uint8_t* pd, int, float* out, int32_t* count); }

DIFFTEST(render_ParticleEmitQuads) {
    FakeEngine fe;
    auto orig = (void(__fastcall*)(uint8_t*, int, float*, int32_t*))0x553ba0;
    static uint8_t pd[0x40], part[0x20], obj[0x120], cam[0x40], saved[0x30];
    static float pos[3], oa[72], ob[72];
    memcpy(saved, (void*)0xd97abc, 0x30);
    *(uint8_t**)(fe.e + 0x28) = cam;
    for (int it = 0; it < 5000; ++it) {
        for (int k = 0; k < 0x30; k += 4) *(float*)(0xd97abc + k) = RndF(-1.0f, 1.0f);
        for (int k = 0; k < 0x40; k += 4) *(float*)(pd + k) = RndF(-3.0f, 3.0f);
        for (int k = 0x25; k <= 0x31; k += 4) *(float*)(pd + k) = RndF(-3.0f, 3.0f); // unaligned fields: no stitched NaNs
        for (int k = 0; k < 0x120; k += 4) *(float*)(obj + k) = RndF(-2.0f, 2.0f);
        for (int k = 0xce; k < 0x10e; k += 4) *(float*)(obj + k) = RndF(-2.0f, 2.0f);
        for (int k = 0; k < 0x40; k += 4) *(float*)(cam + k) = RndF(-50.0f, 50.0f);
        for (float& c : pos) c = RndF(-20.0f, 20.0f);
        *(uint8_t**)(pd + 0) = obj;
        *(uint8_t**)(pd + 8) = part;
        *(float**)(part + 0) = pos;
        *(int32_t*)(part + 4) = Rnd() % 3;
        *(uint32_t*)(part + 8) = Rnd();
        *(float*)(part + 0x14) = RndF(-0.5f, 2.0f);
        memset(oa, 0xcd, sizeof oa);
        memset(ob, 0xcd, sizeof ob);
        int32_t ca = 4, cb = 4;
        orig(pd, 0, oa, &ca);
        Particle_EmitQuads(pd, 0, ob, &cb);
        EXPECT_EQ(ca, cb, "count");
        int d = FirstDiff(oa, ob, sizeof oa);
        EXPECT_EQ(d, -1, "vertices iteration %d: +0x%x %08x vs %08x (%.9g vs %.9g)", it, d, d >= 0 ? ((uint32_t*)oa)[d / 4] : 0,
                  d >= 0 ? ((uint32_t*)ob)[d / 4] : 0, d >= 0 ? oa[d / 4] : 0.0f, d >= 0 ? ob[d / 4] : 0.0f);
    }
    memcpy((void*)0xd97abc, saved, 0x30);
}

namespace rt3 { int32_t __fastcall BuildingType_LoadFootprint(void*, int, uint8_t* t, const char* path); }

DIFFTEST(game_LoadFootprint) {
    auto orig = (int32_t(__fastcall*)(void*, int, uint8_t*, const char*))0x414490;
    static uint8_t ta[0x1b0], tb[0x1b0];
    char dir[MAX_PATH], pattern[MAX_PATH];
    GetModuleFileNameA(nullptr, dir, MAX_PATH);
    *strrchr(dir, 0x5c) = 0;
    strcat(dir, "\\Data\\BuildingTypes\\");
    sprintf(pattern, "%s*.bty", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    int files = 0;
    if (h != INVALID_HANDLE_VALUE) {
        do {
            char path[MAX_PATH];
            sprintf(path, "%s%s", dir, fd.cFileName);
            memset(ta, 0, sizeof ta);
            memset(tb, 0, sizeof tb);
            int32_t ra = orig(nullptr, 0, ta, path);
            int32_t rb = BuildingType_LoadFootprint(nullptr, 0, tb, path);
            EXPECT_EQ(ra, rb, "result %s", path);
            int32_t cells = ta[0xb8] * ta[0xb9] * 32;
            for (int k = 0; k < 4; ++k) {
                uint8_t* ma = *(uint8_t**)(ta + 0xcf + k * 4);
                uint8_t* mb = *(uint8_t**)(tb + 0xcf + k * 4);
                EXPECT_EQ(!ma, !mb, "mask %d presence %s", k, path);
                if (ma && mb) EXPECT_EQ(memcmp(ma, mb, cells), 0, "mask %d %s", k, path);
                *(uint8_t**)(ta + 0xcf + k * 4) = nullptr;
                *(uint8_t**)(tb + 0xcf + k * 4) = nullptr;
            }
            EXPECT_EQ(memcmp(ta, tb, sizeof ta), 0, "type %s", path);
            ++files;
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    EXPECT_EQ(files > 50, true, "only %d footprint files", files);
}

namespace rt3 { float __fastcall Building_Price(uint8_t* b, int, uint32_t company); }

namespace {
uint8_t g_bpType[0x60];
uint8_t* __fastcall FakeBuildingType(void*) { return g_bpType; }
}

DIFFTEST(game_BuildingPrice) {
    auto orig = (float(__fastcall*)(uint8_t*, int, uint32_t))0x40d540;
    static void* vtbl[0x30];
    vtbl[0x80 / 4] = (void*)&FakeBuildingType;
    static uint8_t b[0x400], world[0x200], map[0x2200], grid[64 * 64];
    uint8_t*& gw = hook::Global<uint8_t*>(0x6cec78);
    uint8_t*& gm = hook::Global<uint8_t*>(0x62c120);
    Collection*& greg = hook::Global<Collection*>(0x6cfc9c);
    uint8_t* sw = gw;
    uint8_t* sm = gm;
    Collection* sr = greg;
    Collection empty;
    memset(&empty, 0, sizeof empty);
    Collection_Construct(&empty, 0, 1, 0x10, 4, 4, 0, 0, 0);
    gw = world;
    gm = map;
    greg = &empty;
    *(int32_t*)(map + 0x2155) = 64;
    *(uint8_t**)(map + 0x2131) = grid;
    for (int it = 0; it < 20000; ++it) {
        for (int k = 0; k < 0x400; k += 4) *(float*)(b + k) = RndF(-200000.0f, 900000.0f);
        *(void**)b = vtbl;
        *(int32_t*)(b + 0x246) = Rnd() % 2;
        *(int32_t*)(b + 0x3d5) = 1830 + Rnd() % 40;
        *(float*)(b + 0x1e2) = RndF(0.0f, 63.0f * 92.0f);
        *(float*)(b + 0x1ea) = RndF(0.0f, 63.0f * 92.0f);
        *(uint32_t*)(g_bpType + 0x22) = Rnd() % 3000000;
        *(int32_t*)(g_bpType + 0x43) = (int32_t)(Rnd() % 500000);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1825 + Rnd() % 50);
        world[0xf] = (uint8_t)(Rnd() % 13);
        *(int32_t*)(world + 0x8a + 0x3c * 4) = (int32_t)(Rnd() % 300) - 120;
        float pa = orig(b, 0, 0);
        float pb = Building_Price(b, 0, 0);
        EXPECT_EQ(*(uint32_t*)&pa, *(uint32_t*)&pb, "price iteration %d: %f vs %f", it, pa, pb);
    }
    gw = sw;
    gm = sm;
    greg = sr;
    Collection_Destruct(&empty);
}

namespace rt3 {
struct PathNode;
int32_t __fastcall Track_PlanRoute(uint8_t* self, int, float x0, float y0, float x1, float y1, PathNode** out,
                                   PathNode** pending);
float __fastcall Track_RampGrade(uint8_t* self, int, float x0, float y0, float ang, float tx, float ty);
}

namespace {
struct Wp {
    float x, y;
    Wp* next;
};
Wp* NewWp(float x, float y, Wp* next) {
    Wp* w = (Wp*)((void*(__cdecl*)(size_t))0x53b070)(0xc);
    w->x = x;
    w->y = y;
    w->next = next;
    return w;
}
void FreeWps(Wp* w) {
    while (w) {
        Wp* n = w->next;
        ((void(__cdecl*)(void*))0x53b080)(w);
        w = n;
    }
}
} // namespace

DIFFTEST(game_TrackPlanRoute) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, float, float, float, float, Wp**, Wp**))0x49cc40;
    const int32_t N = 80; // quarter tiles
    static uint8_t map[0x2200], bits[N * N / 8], selfA[0x200], selfB[0x200];
    *(int32_t*)(map + 0x215d) = N;
    *(int32_t*)(map + 0x2161) = N;
    *(uint8_t**)(map + 0x2139) = bits;
    *(uint8_t**)(map + 0x213d) = bits;
    *(uint8_t**)(map + 0x2141) = bits;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sm = gmap;
    gmap = map;
    int res[2] = {}, multi = 0;
    for (int it = 0; it < 1500; ++it) {
        memset(bits, 0, sizeof bits);
        int blobs = (int)(Rnd() % 6);
        for (int b = 0; b < blobs; ++b) {
            int cx = (int)(Rnd() % N), cy = (int)(Rnd() % N), r = 2 + (int)(Rnd() % 14);
            bool box = Rnd() % 2;
            for (int y = 0; y < N; ++y)
                for (int x = 0; x < N; ++x) {
                    int dx = x - cx, dy = y - cy;
                    if (box ? (abs(dx) <= r && abs(dy) <= r / 3) : dx * dx + dy * dy <= r * r) {
                        int i = y * N + x;
                        bits[i >> 3] |= (uint8_t)(1 << (i & 7));
                    }
                }
        }
        float x0 = RndF(0.5f, N / 4 - 0.5f), y0 = RndF(0.5f, N / 4 - 0.5f);
        float x1 = Rnd() % 20 == 0 ? x0 : RndF(0.5f, N / 4 - 0.5f), y1 = Rnd() % 20 == 0 ? y0 : RndF(0.5f, N / 4 - 0.5f);
        memset(selfA, 0, sizeof selfA);
        int mode = (int)(Rnd() % 4);
        if (mode == 3) selfA[0x1cd] = 1;
        memcpy(selfB, selfA, sizeof selfA);
        Wp *cacheA = nullptr, *cacheB = nullptr;
        if (mode >= 2) {
            int n = 2 + (int)(Rnd() % 4);
            for (int k = 0; k < n; ++k) {
                float cx = RndF(0.5f, N / 4 - 0.5f), cy = RndF(0.5f, N / 4 - 0.5f);
                cacheA = NewWp(cx, cy, cacheA);
                cacheB = NewWp(cx, cy, cacheB);
            }
        }
        *(Wp**)(selfA + 0x1c9) = cacheA;
        *(Wp**)(selfB + 0x1c9) = cacheB;
        Wp *pendA = nullptr, *pendB = nullptr;
        if (mode == 1) {
            int n = 1 + (int)(Rnd() % 3);
            for (int k = 0; k < n; ++k) {
                float px = RndF(0.5f, N / 4 - 0.5f), py = RndF(0.5f, N / 4 - 0.5f);
                pendA = NewWp(px, py, pendA);
                pendB = NewWp(px, py, pendB);
            }
        }
        Wp *outA = nullptr, *outB = nullptr;
        int32_t ra = orig(selfA, 0, x0, y0, x1, y1, &outA, &pendA);
        int32_t rb = rt3::Track_PlanRoute(selfB, 0, x0, y0, x1, y1, (rt3::PathNode**)&outB, (rt3::PathNode**)&pendB);
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        res[ra & 1]++;
        EXPECT_EQ(pendA == nullptr, pendB == nullptr, "pending iteration %d", it);
        Wp* a = outA;
        Wp* b = outB;
        int len = 0;
        for (; a && b; a = a->next, b = b->next, ++len) {
            EXPECT_EQ(*(uint32_t*)&a->x, *(uint32_t*)&b->x, "x node %d iteration %d", len, it);
            EXPECT_EQ(*(uint32_t*)&a->y, *(uint32_t*)&b->y, "y node %d iteration %d", len, it);
        }
        EXPECT_EQ(a == nullptr && b == nullptr, true, "length iteration %d", it);
        if (len > 2) multi++;
        bool cachedA = *(Wp**)(selfA + 0x1c9) == outA, cachedB = *(Wp**)(selfB + 0x1c9) == outB;
        EXPECT_EQ(cachedA, cachedB, "cache reuse iteration %d", it);
        EXPECT_EQ(*(int32_t*)(selfA + 0x135), *(int32_t*)(selfB + 0x135), "replan flag iteration %d", it);
        if (!cachedA) FreeWps(*(Wp**)(selfA + 0x1c9));
        if (!cachedB) FreeWps(*(Wp**)(selfB + 0x1c9));
        FreeWps(outA);
        FreeWps(outB);
        FreeWps(pendA);
        FreeWps(pendB);
    }
    gmap = sm;
    EXPECT_EQ(res[0] > 50 && res[1] > 50 && multi > 50, true, "coverage: %d fail, %d ok, %d multi", res[0], res[1], multi);
}

DIFFTEST(game_TrackRampGrade) {
    auto orig = (float(__fastcall*)(uint8_t*, int, float, float, float, float, float))0x4a0d20;
    const int32_t N = 40, V = N + 1;
    static uint8_t map[0x2200], world[0x200], selfA[0x200], selfB[0x200], recs[V * V * 3];
    static float heights[V * V];
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15d5) = N;
    *(int32_t*)(map + 0x15dd) = V;
    *(float**)(map + 0x1605) = heights;
    *(uint8_t**)(map + 0x165d) = recs;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t*& gworld = hook::Global<uint8_t*>(0x6cec74);
    uint8_t* sm = gmap;
    uint8_t* sw = gworld;
    gmap = map;
    gworld = world;
    int kinds[3] = {};
    for (int it = 0; it < 6000; ++it) {
        float x0 = RndF(15.0f, N - 15.0f), y0 = RndF(15.0f, N - 15.0f);
        float ang = RndF(-4.0f, 4.0f);
        float dist = RndF(2.0f, 12.0f);
        float tx = x0 - (float)cos(ang) * dist + RndF(-1.0f, 1.0f), ty = y0 - (float)sin(ang) * dist + RndF(-1.0f, 1.0f);
        float h0 = RndF(0.0f, 300.0f), amp = RndF(0.0f, 900.0f), valley = RndF(1.0f, 10.0f);
        for (int k = 0; k < V * V; ++k) {
            double ex = k % V - x0, ey = k / V - y0;
            double along = -(ex * cos(ang) + ey * sin(ang));
            heights[k] = (float)(h0 + amp * sin(along / valley * 3.14159) + RndF(0.0f, 20.0f));
        }
        for (int k = 0; k < V * V * 3; ++k) recs[k] = (uint8_t)(Rnd() % 200 == 0 ? Rnd() : 0);
        *(int32_t*)(world + 0x140) = (int32_t)(Rnd() % 4);
        memset(selfA, 0, sizeof selfA);
        memset(selfB, 0, sizeof selfB);
        float ra = orig(selfA, 0, x0, y0, ang, tx, ty);
        float rb = rt3::Track_RampGrade(selfB, 0, x0, y0, ang, tx, ty);
        EXPECT_EQ(*(uint32_t*)&ra, *(uint32_t*)&rb, "result iteration %d (%g vs %g)", it, ra, rb);
        EXPECT_EQ(memcmp(selfA, selfB, sizeof selfA), 0, "foot iteration %d", it);
        kinds[ra == 1.5f ? 0 : ra == 0.0f ? 1 : 2]++;
    }
    gmap = sm;
    gworld = sw;
    EXPECT_EQ(kinds[0] > 100 && kinds[2] > 100, true, "coverage: %d fail, %d flat, %d grade", kinds[0], kinds[1], kinds[2]);
}

namespace rt3 {
void __fastcall Track_MarkCells(uint8_t* t);
void __fastcall Track_UnmarkCells(uint8_t* t, int, int32_t remark);
}

namespace {
uint32_t g_townSeq;
int32_t __fastcall FakeIsTown(void*) {
    g_townSeq = g_townSeq * 1103515245u + 12345u;
    return (g_townSeq >> 16) & 1;
}
} // namespace

DIFFTEST(game_TrackMarkUnmarkCells) {
    auto origMark = (void(__fastcall*)(uint8_t*))0x48bdb0;
    auto origUnmark = (void(__fastcall*)(uint8_t*, int, int32_t))0x48c400;
    const int32_t N = 32, FN = N * 4;
    static uint8_t map[0x2200], nodes[0x100], renderer[0x4000], piece[0x240];
    static uint8_t gridA[N * N], gridB[N * N], fine[2][3][FN * FN / 8];
    static void* flagGrids[16];
    static void* vtbl[8];
    vtbl[5] = (void*)&FakeIsTown;
    FakeEngine fe;
    *(void**)(fe.e + 0x18) = renderer;
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x215d) = FN;
    *(int32_t*)(map + 0x2161) = FN;
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15d5) = N;
    *(void***)(map + 0x1685) = flagGrids;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t*& gnodes = hook::Global<uint8_t*>(0x6cfca8);
    uint8_t* sm = gmap;
    uint8_t* sn = gnodes;
    gmap = map;
    gnodes = nodes;
    auto use = [&](int side) {
        *(uint8_t**)(map + 0x2131) = side ? gridB : gridA;
        *(uint8_t**)(map + 0x2139) = fine[side][0];
        *(uint8_t**)(map + 0x213d) = fine[side][1];
        *(uint8_t**)(map + 0x2141) = fine[side][2];
    };
    for (int it = 0; it < 3000; ++it) {
        for (int k = 0; k < N * N; ++k) gridA[k] = gridB[k] = (uint8_t)(Rnd() % 4 == 0 ? Rnd() : 0);
        for (int l = 0; l < 3; ++l)
            for (int k = 0; k < FN * FN / 8; ++k) fine[0][l][k] = fine[1][l][k] = (uint8_t)(Rnd() % 3 == 0 ? Rnd() : 0);
        memset(piece, 0, sizeof piece);
        *(void***)piece = vtbl;
        *(int32_t*)(piece + 8) = (int32_t)(Rnd() % 2);
        *(int32_t*)(piece + 0x226) = (int32_t)(Rnd() % 5);
        for (int v = 0; v < 4; ++v) {
            *(float*)(piece + 0x14 + v * 0xc) = RndF(1.0f, (N - 1) * 92.0f);
            *(float*)(piece + 0x14 + v * 0xc + 4) = RndF(0.0f, 300.0f);
            *(float*)(piece + 0x14 + v * 0xc + 8) = RndF(1.0f, (N - 1) * 92.0f);
        }
        *(int32_t*)(nodes + 0xed) = Rnd() % 10 == 0;
        bool unmark = Rnd() % 2;
        uint32_t seed = Rnd();
        renderer[0x374a] = 0;
        use(0);
        g_townSeq = seed;
        if (unmark) origUnmark(piece, 0, 0); else origMark(piece);
        uint8_t flagA = renderer[0x374a];
        renderer[0x374a] = 0;
        use(1);
        g_townSeq = seed;
        if (unmark) rt3::Track_UnmarkCells(piece, 0, 0); else rt3::Track_MarkCells(piece);
        EXPECT_EQ(flagA, renderer[0x374a], "renderer flag iteration %d", it);
        EXPECT_EQ(memcmp(gridA, gridB, sizeof gridA), 0, "cells iteration %d (unmark %d)", it, (int)unmark);
        for (int l = 0; l < 3; ++l)
            EXPECT_EQ(memcmp(fine[0][l], fine[1][l], sizeof fine[0][l]), 0, "fine layer %d iteration %d (unmark %d)", l, it, (int)unmark);
    }
    gmap = sm;
    gnodes = sn;
}

namespace rt3 {
void Cpu_Name();
}

namespace {
void CallCpuName(void* fn, char* dst, const char* vendor, int32_t model, int32_t family) {
    __asm {
        push ebx
        push edi
        push family
        mov eax, dst
        mov edi, vendor
        mov ebx, model
        call fn
        pop edi
        pop ebx
    }
}
} // namespace

DIFFTEST(game_CpuName) {
    const char* vendors[4] = { "AuthenticAMD", "GenuineIntel", "CyrixInstead", "GenuineIntex" };
    for (int v = 0; v < 4; ++v)
        for (int32_t family = 0; family < 18; ++family)
            for (int32_t model = -2; model < 18; ++model) {
                char a[64], b[64];
                memset(a, 0x5a, sizeof a);
                memset(b, 0x5a, sizeof b);
                CallCpuName((void*)0x55ce60, a, vendors[v], model, family);
                CallCpuName((void*)&rt3::Cpu_Name, b, vendors[v], model, family);
                EXPECT_EQ(memcmp(a, b, sizeof a), 0, "%s family %d model %d: %s vs %s", vendors[v], family, model, a, b);
            }
}

namespace rt3 {
void __fastcall Emitter_LaunchParticle(uint8_t* e, int, uint8_t* p, const float* at, int32_t t);
}

namespace {
// Empties the x87 register tags (Particle_Color leaves one value behind in mode 1)
// without touching the control word.
void X87Clear() {
    __asm {
        ffree st(0)
        ffree st(1)
        ffree st(2)
        ffree st(3)
        ffree st(4)
        ffree st(5)
        ffree st(6)
        ffree st(7)
    }
}
} // namespace

DIFFTEST(render_EmitterLaunchParticle) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*, const float*, int32_t))0x555780;
    static uint8_t ea[0x200], eb[0x200], pa[0x80], pb[0x80], obj[0x100], clock[0x40];
    uint32_t& seed = hook::Global<uint32_t>(0x624324);
    int32_t& slow = hook::Global<int32_t>(0xd939a8);
    uint8_t*& gclock = hook::Global<uint8_t*>(0x6d4030);
    int32_t& now = hook::Global<int32_t>(0x6257d8);
    uint32_t sSeed = seed;
    int32_t sSlow = slow, sNow = now;
    uint8_t* sClock = gclock;
    gclock = clock;
    for (int it = 0; it < 6000; ++it) {
        for (int k = 0; k < 0x200; k += 4) *(float*)(ea + k) = RndF(-3.0f, 3.0f);
        *(int32_t*)(ea + 0x54) = (int32_t)(Rnd() % 3000) - 200;
        *(int32_t*)(ea + 0x58) = (int32_t)(Rnd() % 2000);
        *(int32_t*)(ea + 0x50) = (int32_t)(Rnd() % 50);
        *(int32_t*)(ea + 0x6c) = (int32_t)(Rnd() % 5000);
        *(uint32_t*)(ea + 0x48) = Rnd();
        *(uint32_t*)(ea + 0x4c) = Rnd();
        for (int k = 0; k < 3; ++k) *(float*)(ea + 0x3c + k * 4) = RndF(0.0f, 1.0f);
        uint32_t fl = Rnd() & 0xf7fffbff;
        *(uint32_t*)(ea + 0xac) = fl;
        *(uint8_t**)(ea + 0xc) = Rnd() % 2 ? obj : nullptr;
        for (int k = 0; k < 0x30; k += 4) *(float*)(obj + 0xce + k) = RndF(-1.0f, 1.0f);
        *(int32_t*)(ea + 0x110) = 1 + (int32_t)(Rnd() % 64);
        *(int32_t*)(ea + 0x114) = 1 + (int32_t)(Rnd() % 64);
        *(int32_t*)(ea + 0x118) = 1 + (int32_t)(Rnd() % 16);
        *(int32_t*)(ea + 0x11c) = (int32_t)(Rnd() % 256);
        *(int32_t*)(ea + 0x120) = (int32_t)(Rnd() % 256);
        *(int32_t*)(ea + 0x124) = 1 + (int32_t)(Rnd() % 4);
        *(float*)(ea + 0xcc) = RndF(-0.2f, 2.0f);
        memcpy(eb, ea, sizeof ea);
        for (int k = 0; k < 0x80; k += 4) *(float*)(pa + k) = RndF(-5.0f, 5.0f);
        *(int32_t*)(pa + 0x54) = (int32_t)(Rnd() % 100);
        *(int32_t*)(pa + 0x48) = (int32_t)(Rnd() % 100);
        *(int32_t*)(pa + 0x4c) = 1 + (int32_t)(Rnd() % 100);
        *(uint32_t*)(pa + 0x5c) = (fl & 0x40000000) | (Rnd() & 0x30000100);
        memcpy(pb, pa, sizeof pa);
        float at[3] = { RndF(-50.0f, 50.0f), RndF(-50.0f, 50.0f), RndF(-50.0f, 50.0f) };
        int32_t t = (int32_t)(Rnd() % 6000);
        slow = Rnd() % 2;
        *(float*)(clock + 0x30) = RndF(0.0f, 3.0f);
        now = (int32_t)(Rnd() % 100);
        uint32_t s0 = Rnd();
        seed = s0;
        orig(ea, 0, pa, at, t);
        X87Clear();
        uint32_t sa = seed;
        seed = s0;
        Emitter_LaunchParticle(eb, 0, pb, at, t);
        X87Clear();
        EXPECT_EQ(sa, seed, "seed iteration %d", it);
        EXPECT_EQ(memcmp(ea, eb, sizeof ea), 0, "emitter iteration %d", it);
        // Without a turn or an end velocity the original leaves an uninitialized stack
        // copy in +0x18..+0x20, visible when the particle gets no lifetime.
        if (!(fl & 0x40000000) && !((fl & 1) && *(uint8_t**)(ea + 0xc))) memcpy(pb + 0x18, pa + 0x18, 0xc);
        int pd = -1;
        for (int k = 0; k < (int)sizeof pa && pd < 0; ++k)
            if (pa[k] != pb[k]) pd = k & ~3;
        EXPECT_EQ(pd, -1, "particle iteration %d (flags %08x, p flags %08x, obj %d): +0x%x %08x (%.9g) vs %08x (%.9g)", it, fl,
                  *(uint32_t*)(pa + 0x5c), *(uint8_t**)(ea + 0xc) != nullptr, pd, *(uint32_t*)(pa + (pd & 0x7c)),
                  *(float*)(pa + (pd & 0x7c)), *(uint32_t*)(pb + (pd & 0x7c)), *(float*)(pb + (pd & 0x7c)));
    }
    seed = sSeed;
    slow = sSlow;
    now = sNow;
    gclock = sClock;
}

namespace rt3 {
void __fastcall Track_BuildGeometry(uint8_t* e);
}

DIFFTEST(game_TrackBuildGeometry) {
    auto orig = (void(__fastcall*)(uint8_t*))0x539640;
    const int32_t N = 16, V = N + 1;
    static uint8_t terrain[0x1700], ea[0x240], eb[0x240];
    static float heights[V * V];
    *(int32_t*)(terrain + 0x15d9) = N;
    *(int32_t*)(terrain + 0x15d5) = N;
    *(float**)(terrain + 0x1605) = heights;
    float& cached = hook::Global<float>(0x624e00);
    int32_t& corner = hook::Global<int32_t>(0xccba24);
    float sCached = cached;
    int32_t sCorner = corner;
    auto mallocExe = (void*(__cdecl*)(size_t))0x5a125d;
    auto freeExe = (void(__cdecl*)(void*))0x5a1145;
    for (int it = 0; it < 6000; ++it) {
        for (int k = 0; k < V * V; ++k) heights[k] = RndF(-50.0f, 400.0f);
        for (int k = 0; k < 0x240; k += 4) *(float*)(ea + k) = RndF(-100.0f, 100.0f);
        for (int k = 0x14; k <= 0x40; k += 4) *(float*)(ea + k) = RndF(0.0f, N * 92.0f);
        for (int k = 0x45; k <= 0x51; k += 4) *(float*)(ea + k) = RndF(0.0f, 30.0f);
        for (int k = 0x1f5; k <= 0x1fd; k += 4) *(float*)(ea + k) = RndF(0.5f, 20.0f);
        *(uint8_t**)(ea + 4) = terrain;
        ea[0x44] = (uint8_t)(Rnd() % 6);
        memcpy(eb, ea, sizeof ea);
        bool have = Rnd() % 2;
        *(void**)(ea + 0x1e3) = have ? mallocExe(0xc0) : nullptr;
        *(void**)(eb + 0x1e3) = have ? mallocExe(0xc0) : nullptr;
        if (have) {
            memset(*(void**)(ea + 0x1e3), 0x33, 0xc0);
            memset(*(void**)(eb + 0x1e3), 0x33, 0xc0);
        }
        corner = (int32_t)(Rnd() % 8);
        cached = 0.0f;
        orig(ea);
        float ca = cached;
        cached = 0.0f;
        Track_BuildGeometry(eb);
        EXPECT_EQ(*(uint32_t*)&ca, *(uint32_t*)&cached, "cached height iteration %d", it);
        void* qa = *(void**)(ea + 0x1e3);
        void* qb = *(void**)(eb + 0x1e3);
        EXPECT_EQ(qa == nullptr, qb == nullptr, "quads iteration %d", it);
        if (qa && qb) {
            if (have || ea[0x44] == 2 || ea[0x44] == 3) // other kinds leave (some of) a fresh buffer uninitialized
                EXPECT_EQ(memcmp(qa, qb, 0xc0), 0, "quads iteration %d kind %d", it, ea[0x44]);
            else
                for (int v = 0; v < 8; ++v)
                    EXPECT_EQ(memcmp((uint8_t*)qa + v * 0x18 + 0xc, (uint8_t*)qb + v * 0x18 + 0xc, 0xc), 0, "quad colours iteration %d", it);
        }
        *(void**)(ea + 0x1e3) = nullptr;
        *(void**)(eb + 0x1e3) = nullptr;
        int diff = -1;
        for (int k = 0; k < (int)sizeof ea && diff < 0; ++k)
            if (ea[k] != eb[k]) diff = k;
        if (diff >= 0) {
            int o = diff >= 0x63 ? 0x63 + (diff - 0x63) / 4 * 4 : diff & ~3;
            EXPECT_EQ(diff, -1, "piece iteration %d kind %d: first difference at +0x%x (%.9g vs %.9g; in %.9g %.9g %.9g %.9g)", it,
                      ea[0x44], o, *(float*)(ea + o), *(float*)(eb + o), *(float*)(eb + 0x14), *(float*)(eb + 0x20),
                      *(float*)(eb + 0x1fd), *(float*)(eb + 0x1f5));
        }
        if (qa) freeExe(qa);
        if (qb) freeExe(qb);
    }
    cached = sCached;
    corner = sCorner;
}

namespace rt3 {
int32_t __fastcall Track_AfterLoad(uint8_t* e, int, int32_t);
void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                            void* terrain, uint8_t style);
void __fastcall Track_SetEnds(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d);
void __fastcall Track_DropOnTerrain(uint8_t* e, int, float lift);
void __fastcall Track_SetFlatHeight(uint8_t* e, int, uint32_t h);
}

DIFFTEST(game_TrackSetEndsAndHeights) {
    auto origEnds = (void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*))0x53a740;
    auto origDrop = (void(__fastcall*)(uint8_t*, int, float))0x53a890;
    auto origFlat = (void(__fastcall*)(uint8_t*, int, uint32_t))0x53a920;
    const int32_t N = 16, V = N + 1;
    static uint8_t ta[0x1700], tb[0x1700], ea[0x240], eb[0x240];
    static float heights[V * V];
    static PtrList la, lb;
    static PtrList* cellsA[1] = { &la };
    static PtrList* cellsB[1] = { &lb };
    uint8_t* both[2] = { ta, tb };
    for (uint8_t* t : both) {
        *(int32_t*)(t + 0x15d9) = N;
        *(int32_t*)(t + 0x15d5) = N;
        *(float**)(t + 0x1605) = heights;
    }
    *(PtrList***)(ta + 0x1675) = cellsA;
    *(PtrList***)(tb + 0x1675) = cellsB;
    PtrList_Init(&la);
    PtrList_Init(&lb);
    FakeEngine fe;
    float& cached = hook::Global<float>(0x624e00);
    float sCached = cached;
    for (int it = 0; it < 4000; ++it) {
        for (int k = 0; k < V * V; ++k) heights[k] = RndF(-50.0f, 400.0f);
        for (int k = 0; k < 0x240; k += 4) *(float*)(ea + k) = RndF(-100.0f, 100.0f);
        for (int k = 0xc; k <= 0x40; k += 4) *(float*)(ea + k) = RndF(0.0f, (N - 0.5f) * 92.0f);
        for (int k = 0x45; k <= 0x51; k += 4) *(float*)(ea + k) = RndF(0.0f, 30.0f);
        for (int k = 0x1f5; k <= 0x1fd; k += 4) *(float*)(ea + k) = RndF(0.5f, 20.0f);
        ea[0x44] = (uint8_t)(Rnd() % 4 == 0 ? 2 : 0);
        *(void**)(ea + 0x1e3) = nullptr;
        bool quads = Rnd() % 3 == 0;
        int op = (int)(Rnd() % 5);
        bool terrain = op != 0 || Rnd() % 4 != 0; // only Track_SetEnds checks for missing terrain
        *(int32_t*)(fe.e + 0x114256) = Rnd() % 4 == 0;
        memcpy(eb, ea, sizeof ea);
        *(uint8_t**)(ea + 4) = terrain ? ta : nullptr;
        *(uint8_t**)(eb + 4) = terrain ? tb : nullptr;
        if (quads) {
            *(void**)(ea + 0x1e3) = ((void*(__cdecl*)(size_t))0x5a125d)(0xc0);
            *(void**)(eb + 0x1e3) = ((void*(__cdecl*)(size_t))0x5a125d)(0xc0);
            memset(*(void**)(ea + 0x1e3), 0x33, 0xc0);
            memset(*(void**)(eb + 0x1e3), 0x33, 0xc0);
        }
        bool listed = Rnd() % 2;
        *(PtrListNode**)(ea + 0x55) = listed ? PtrList_Append(&la, 0, ea) : nullptr;
        *(PtrListNode**)(eb + 0x55) = listed ? PtrList_Append(&lb, 0, eb) : nullptr;
        uint32_t vec[4][3];
        for (auto& v : vec)
            for (uint32_t& c : v) {
                float f = RndF(0.0f, (N - 0.5f) * 92.0f);
                c = *(uint32_t*)&f;
            }
        const uint32_t* vp[4];
        for (int k = 0; k < 4; ++k) vp[k] = Rnd() % 3 ? vec[k] : nullptr;
        float lift = RndF(-5.0f, 20.0f);
        uint32_t flat = Rnd();
        if (op == 0) {
            origEnds(ea, 0, vp[0], vp[1], vp[2], vp[3]);
            X87Clear();
            Track_SetEnds(eb, 0, vp[0], vp[1], vp[2], vp[3]);
        } else if (op == 3) {
            void* stale[2] = { *(void**)(ea + 0x1e3), *(void**)(eb + 0x1e3) };
            *(void**)(fe.e + 0x1c) = ta;
            int32_t ra = ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x53ac40)(ea, 0, 0);
            *(void**)(fe.e + 0x1c) = tb;
            int32_t rb = Track_AfterLoad(eb, 0, 0);
            EXPECT_EQ(ra, rb, "after-load result iteration %d", it);
            for (void* q : stale)
                if (q) ((void(__cdecl*)(void*))0x5a1145)(q);
        } else if (op == 4) {
            // Style 3 reads past the 3-entry style tables, up to the cached corner
            // height 0x624e00 (which the geometry rebuild rewrites).
            uint8_t style = (uint8_t)(Rnd() % 5);
            float c0 = RndF(0.0f, 1.0f);
            cached = c0;
            ((void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, void*, uint32_t))0x53ae10)(
                ea, 0, vp[0], vp[1], vp[2], vp[3], ta, style);
            cached = c0;
            Track_Setup(eb, 0, vp[0], vp[1], vp[2], vp[3], tb, style);
        } else if (op == 1) {
            origDrop(ea, 0, lift);
            Track_DropOnTerrain(eb, 0, lift);
        } else {
            float f = RndF(-10.0f, 300.0f);
            flat = *(uint32_t*)&f;
            origFlat(ea, 0, flat);
            Track_SetFlatHeight(eb, 0, flat);
        }
        EXPECT_EQ(la.count, lb.count, "cell list op %d iteration %d", op, it);
        PtrListNode* na = *(PtrListNode**)(ea + 0x55);
        PtrListNode* nb = *(PtrListNode**)(eb + 0x55);
        EXPECT_EQ(na == nullptr, nb == nullptr, "cell entry op %d iteration %d", op, it);
        if (na && nb) EXPECT_EQ(na->value == ea && nb->value == eb, true, "cell entry value op %d iteration %d", op, it);
        *(PtrListNode**)(ea + 0x55) = nullptr;
        *(PtrListNode**)(eb + 0x55) = nullptr;
        *(uint8_t**)(ea + 4) = nullptr;
        *(uint8_t**)(eb + 4) = nullptr;
        void* qa = *(void**)(ea + 0x1e3);
        void* qb = *(void**)(eb + 0x1e3);
        if (qa && qb) EXPECT_EQ(memcmp(qa, qb, 0xc0), 0, "quads op %d iteration %d", op, it);
        *(void**)(ea + 0x1e3) = nullptr;
        *(void**)(eb + 0x1e3) = nullptr;
        if (qa) ((void(__cdecl*)(void*))0x5a1145)(qa);
        if (qb) ((void(__cdecl*)(void*))0x5a1145)(qb);
        int pdiff = FirstDiff(ea, eb, sizeof ea);
        EXPECT_EQ(pdiff, -1, "piece op %d iteration %d: +0x%x %02x vs %02x (kind %d)", op, it, pdiff, pdiff >= 0 ? ea[pdiff] : 0,
                  pdiff >= 0 ? eb[pdiff] : 0, ea[0x44]);
        PtrList_Clear(&la);
        PtrList_Clear(&lb);
    }
    cached = sCached;
}

namespace rt3 {
char* Os_VersionName();
}

namespace {
OSVERSIONINFOEXA g_fakeOs;
BOOL g_fakeOsOk;
BOOL __stdcall FakeGetVersionEx(OSVERSIONINFOA* vi) {
    if (vi->dwOSVersionInfoSize != 0x9c) return FALSE;
    memcpy(vi, &g_fakeOs, sizeof g_fakeOs);
    return g_fakeOsOk;
}
} // namespace

DIFFTEST(game_OsVersionName) {
    auto orig = (char*(__cdecl*)())0x55d4c0;
    void** iat = (void**)0x5c81d4;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeGetVersionEx;
    static const char* packs[] = { "", "Service Pack 1", "Service Pack 3", "x" };
    static const uint16_t suites[] = { 0, 2, 0x80, 0x82, 0x200, 0x400, 0x402, 0x10 };
    char a[0x100];
    for (int it = 0; it < 4000; ++it) {
        memset(&g_fakeOs, 0, sizeof g_fakeOs);
        g_fakeOs.dwOSVersionInfoSize = 0x9c;
        g_fakeOs.dwPlatformId = Rnd() % 4;
        g_fakeOs.dwMajorVersion = 3 + Rnd() % 5;
        g_fakeOs.dwMinorVersion = Rnd() % 3 ? Rnd() % 4 : Rnd() % 120;
        strcpy(g_fakeOs.szCSDVersion, packs[Rnd() % 4]);
        g_fakeOs.wSuiteMask = Rnd() % 2 ? suites[Rnd() % 8] : (uint16_t)Rnd();
        g_fakeOs.wProductType = (uint8_t)(Rnd() % 4);
        g_fakeOsOk = Rnd() % 10 != 0;
        memset((void*)0xd97868, 0x5a, 0x100);
        char* ra = orig();
        memcpy(a, (void*)0xd97868, sizeof a);
        memset((void*)0xd97868, 0x5a, 0x100);
        char* rb = Os_VersionName();
        EXPECT_EQ(ra == rb, true, "result iteration %d", it);
        EXPECT_EQ(memcmp(a, (void*)0xd97868, sizeof a), 0, "text iteration %d: '%s' vs '%s'", it, a, (char*)0xd97868);
    }
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
}

namespace rt3 {
void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);
void __fastcall SList_PushFront(void** l, int, void** node);
}

namespace {
struct TickSide {
    uint8_t e[0x240], obj[0x200], ring[64 * 0xe8 + 0x100];
    uint32_t freeList[3], liveList[3];
    void* nodes[24][2];
    uint8_t parts[24][0x80];
    float points[12];
};
TickSide g_tick[2];
} // namespace

DIFFTEST(render_EmitterTick) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t))0x555e50;
    uint8_t clock[0x40] = {};
    FakeEngine fe;
    uint8_t*& gclock = hook::Global<uint8_t*>(0x6d4030);
    uint8_t* sClock = gclock;
    gclock = clock;
    struct G {
        uint32_t seed, ring, now, reset, a0, slow;
    };
    auto saveG = [] {
        return G{ hook::Global<uint32_t>(0x624324), hook::Global<uint32_t>(0xd939a4), hook::Global<uint32_t>(0x6257d8),
                  hook::Global<uint32_t>(0xd97af4), hook::Global<uint32_t>(0xd939a0), hook::Global<uint32_t>(0xd939a8) };
    };
    auto loadG = [](const G& g) {
        hook::Global<uint32_t>(0x624324) = g.seed;
        hook::Global<uint32_t>(0xd939a4) = g.ring;
        hook::Global<uint32_t>(0x6257d8) = g.now;
        hook::Global<uint32_t>(0xd97af4) = g.reset;
        hook::Global<uint32_t>(0xd939a0) = g.a0;
        hook::Global<uint32_t>(0xd939a8) = g.slow;
    };
    G saved = saveG();
    int launched = 0;
    for (int it = 0; it < 4000; ++it) {
        TickSide& a = g_tick[0];
        TickSide& b = g_tick[1];
        uint8_t* e = a.e;
        for (int k = 0; k < 0x240; k += 4) *(float*)(e + k) = RndF(-3.0f, 3.0f);
        *(int32_t*)(e + 0x54) = (int32_t)(Rnd() % 3000) - 200;
        *(int32_t*)(e + 0x58) = (int32_t)(Rnd() % 2000);
        *(int32_t*)(e + 0x50) = (int32_t)(Rnd() % 50);
        *(uint32_t*)(e + 0x48) = Rnd();
        *(uint32_t*)(e + 0x4c) = Rnd();
        for (int k = 0; k < 3; ++k) *(float*)(e + 0x3c + k * 4) = RndF(0.0f, 1.0f);
        uint32_t fl = Rnd() & ~(uint32_t)(0x4 | (Rnd() % 4 ? 0x1000000 : 0) | (Rnd() % 2 ? 0x400 : 0));
        *(uint32_t*)(e + 0xac) = fl;
        *(int32_t*)(e + 0x110) = 1 + (int32_t)(Rnd() % 64);
        *(int32_t*)(e + 0x114) = 1 + (int32_t)(Rnd() % 64);
        *(int32_t*)(e + 0x118) = 1;
        *(int32_t*)(e + 0x11c) = (int32_t)(Rnd() % 256);
        *(int32_t*)(e + 0x120) = (int32_t)(Rnd() % 256);
        *(int32_t*)(e + 0x124) = 1;
        *(float*)(e + 0xcc) = RndF(-0.2f, 2.0f);
        int32_t now = (int32_t)(Rnd() % 100000);
        *(int32_t*)(e + 0x68) = (int32_t)(Rnd() % 1000); // ring slot counter (only counts up in the game)
        *(int32_t*)(e + 0x6c) = (int32_t)(Rnd() % 20000) - 10000;
        *(int32_t*)(e + 0xc0) = *(int32_t*)(e + 0x6c) + now - (int32_t)(Rnd() % 400) + 50;
        *(int32_t*)(e + 0xc4) = *(int32_t*)(e + 0x6c) + now - (int32_t)(Rnd() % 600) + 50;
        *(int32_t*)(e + 0xb8) = 5 + (int32_t)(Rnd() % 60);
        *(float*)(e + 0xb4) = RndF(0.5f, 4.0f);
        *(int32_t*)(e + 0x30) = (int32_t)(Rnd() % 4);
        *(int32_t*)(e + 0x38) = 1 + (int32_t)(Rnd() % 10);
        *(int32_t*)(e + 0xbc) = (int32_t)(Rnd() % 20);
        *(float*)(e + 0xd8) = RndF(0.0f, 2.0f);
        *(int32_t*)(e + 0x1ed) = Rnd() % 2 ? 0 : (int32_t)(Rnd() % 8);
        *(int32_t*)(e + 0x1f1) = (int32_t)(Rnd() % 12);
        *(int32_t*)(e + 0x1f5) = (int32_t)(Rnd() % 300);
        *(int32_t*)(e + 0x1f9) = Rnd() % 10 == 0 ? 7 : 8;
        e[0x1d8] = Rnd() % 8 == 0;
        e[0xb0] = (uint8_t)(Rnd() % 8 == 0);
        memset(a.obj, 0, sizeof a.obj);
        for (int k = 0xce; k < 0x1f0; k += 4) *(float*)(a.obj + k) = RndF(-1.0f, 1.0f);
        for (float& p : a.points) p = RndF(-20.0f, 20.0f);
        bool hasObj = Rnd() % 2, hasPoints = Rnd() % 2;
        int nFree = (int)(Rnd() % 12), nLive = (int)(Rnd() % 12);
        *(int32_t*)(e + 0x34) = nFree + nLive;
        for (int k = 0; k < 24; ++k) {
            for (int j = 0; j < 0x80; j += 4) *(float*)(a.parts[k] + j) = RndF(-5.0f, 5.0f);
            *(int32_t*)(a.parts[k] + 0x48) = (int32_t)(Rnd() % 100);
            *(int32_t*)(a.parts[k] + 0x4c) = 1 + (int32_t)(Rnd() % 100);
            *(int32_t*)(a.parts[k] + 0x54) = (int32_t)(Rnd() % 100);
            *(uint32_t*)(a.parts[k] + 0x5c) = (fl & 0x40000000) | (Rnd() & 0x30000100);
        }
        float step = RndF(0.0f, 3.0f);
        *(float*)(clock + 0x28) = Rnd() % 8 == 0 ? 0.0f : step;
        *(int32_t*)(clock + 0x2c) = 7;
        *(float*)(clock + 0x30) = RndF(0.0f, 2.0f);
        *(int32_t*)(fe.e + 0x2c) = now + (int32_t)(Rnd() % 50);
        int32_t force = (int32_t)(Rnd() % 2);
        G g0{ Rnd(), 0, (uint32_t)now, 0, 0, Rnd() % 2 };
        // Side b: the same state, rebased onto its own buffers.
        memcpy(b.e, a.e, sizeof a.e);
        memcpy(b.obj, a.obj, sizeof a.obj);
        memcpy(b.points, a.points, sizeof a.points);
        memcpy(b.parts, a.parts, sizeof a.parts);
        TickSide* sides[2] = { &a, &b };
        for (TickSide* s : sides) {
            *(uint8_t**)(s->e + 0xc) = hasObj ? s->obj : nullptr;
            *(float**)(s->e + 0x10) = hasPoints ? s->points : nullptr;
            *(uint8_t**)(s->e + 0x64) = s->ring;
            *(uint32_t**)(s->e + 0) = s->freeList;
            *(uint32_t**)(s->e + 4) = s->liveList;
            memset(s->freeList, 0, sizeof s->freeList);
            memset(s->liveList, 0, sizeof s->liveList);
            for (int k = 0; k < 24; ++k) s->nodes[k][1] = s->parts[k];
            for (int k = nFree - 1; k >= 0; --k) SList_PushFront((void**)s->freeList, 0, s->nodes[k]);
            for (int k = nFree + nLive - 1; k >= nFree; --k) SList_PushFront((void**)s->liveList, 0, s->nodes[k]);
        }
        loadG(g0);
        orig(a.e, 0, force);
        X87Clear();
        G ga = saveG();
        loadG(g0);
        Emitter_Tick(b.e, 0, force);
        X87Clear();
        G gb = saveG();
        EXPECT_EQ(ga.seed, gb.seed, "seed iteration %d", it);
        EXPECT_EQ(ga.ring ? ga.ring - (uint32_t)a.ring : 0, gb.ring ? gb.ring - (uint32_t)b.ring : 0, "ring slot iteration %d", it);
        EXPECT_EQ(ga.now == gb.now && ga.reset == gb.reset, true, "globals iteration %d", it);
        int ed = -1;
        for (int k = 0; k < 0x240 && ed < 0; ++k) {
            bool ptr = k < 8 || (k >= 0xc && k < 0x14) || (k >= 0x64 && k < 0x68);
            if (!ptr && a.e[k] != b.e[k]) ed = k;
        }
        EXPECT_EQ(ed, -1, "emitter iteration %d (flags %08x): +0x%x", it, fl, ed);
        for (int l = 0; l < 2; ++l) {
            uint32_t* la = l ? a.liveList : a.freeList;
            uint32_t* lb = l ? b.liveList : b.freeList;
            EXPECT_EQ(la[2], lb[2], "list %d count iteration %d", l, it);
            void** na = (void**)la[0];
            void** nb = (void**)lb[0];
            for (int guard = 0; na && nb && guard < 30; ++guard, na = (void**)na[0], nb = (void**)nb[0]) {
                int ia = (int)(na - &a.nodes[0][0]) / 2, ib = (int)(nb - &b.nodes[0][0]) / 2;
                EXPECT_EQ(ia, ib, "list %d order iteration %d", l, it);
            }
            EXPECT_EQ(na == nullptr && nb == nullptr, true, "list %d length iteration %d", l, it);
        }
        for (int k = 0; k < 24; ++k) {
            if (!(fl & 0x40000000)) memcpy(b.parts[k] + 0x18, a.parts[k] + 0x18, 0xc); // see EmitterLaunchParticle
            int pd = FirstDiff(a.parts[k], b.parts[k], 0x80);
            EXPECT_EQ(pd, -1, "particle %d iteration %d (flags %08x, free %d live %d, slot a %d b %d): +0x%x %08x vs %08x (p flags %08x life %d/%d)", k, it,
                      fl, nFree, nLive, (int)(ga.ring - (uint32_t)a.ring), (int)(gb.ring - (uint32_t)b.ring), pd, pd >= 0 ? *(uint32_t*)(a.parts[k] + (pd & ~3)) : 0,
                      pd >= 0 ? *(uint32_t*)(b.parts[k] + (pd & ~3)) : 0, *(uint32_t*)(a.parts[k] + 0x5c),
                      *(int32_t*)(a.parts[k] + 0x48), *(int32_t*)(b.parts[k] + 0x48));
        }
        launched += (int)(hook::Global<uint32_t>(0x624324) != g0.seed);
    }
    loadG(saved);
    gclock = sClock;
    EXPECT_EQ(launched > 200, true, "coverage: %d ticks used the LCG", launched);
}

namespace rt3 {
void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
}

namespace {
struct WheelSide {
    uint8_t v[0x400], parts[0x200];
    uint8_t wheels[2][3][0x200];
    uint8_t* wheelPtr[2][3];
    float circ[2][3];
    uint8_t pivots[2][2][0x60];
    uint8_t pivotObj[2][2][4][0x200];
    uint8_t lights[2][0x20];
    uint8_t lamps[2][2][0x200];
    uint8_t* emitters[1];
};
WheelSide g_wheel[2];
} // namespace

DIFFTEST(game_VehicleRollWheels) {
    auto orig = (void(__fastcall*)(uint8_t*, int, float))0x45c990;
    for (int it = 0; it < 3000; ++it) {
        WheelSide& a = g_wheel[0];
        uint8_t* raw = (uint8_t*)&a;
        for (size_t k = 0; k + 4 <= sizeof a; k += 4) *(float*)(raw + k) = RndF(-4.0f, 4.0f);
        static const uint32_t unaligned[] = { 0xee, 0xf2, 0xf6, 0x1e2, 0x1e6, 0x1ea, 0x242, 0x246 }; // no stitched NaNs
        for (uint32_t o : unaligned) *(float*)(a.v + o) = RndF(-4.0f, 4.0f);
        int groups = (int)(Rnd() % 3);
        int nw[2], np[2];
        bool noArm[2][2][2];
        for (int g = 0; g < 2; ++g) {
            nw[g] = (int)(Rnd() % 4);
            np[g] = (int)(Rnd() % 3);
            for (int k = 0; k < 3; ++k) a.circ[g][k] = Rnd() % 10 ? RndF(0.2f, 3.0f) : 0.0f;
            for (int k = 0; k < 2; ++k) {
                *(float*)(a.pivots[g][k] + 0x40) = Rnd() % 6 ? RndF(0.2f, 3.0f) : -1.0f;
                noArm[g][k][0] = Rnd() % 4 == 0;
                noArm[g][k][1] = Rnd() % 4 == 0;
            }
        }
        uint8_t wheelFlags[2][3];
        for (auto& row : wheelFlags)
            for (uint8_t& f : row) f = (uint8_t)(Rnd() % 2 ? 0 : 0x80);
        int32_t lightsOn = (int32_t)(Rnd() % 2);
        int nLamps[2] = { (int)(Rnd() % 3), (int)(Rnd() % 3) };
        float dist = RndF(-30.0f, 30.0f);
        memcpy(&g_wheel[1], &g_wheel[0], sizeof a);
        for (int si = 0; si < 2; ++si) {
            WheelSide& s = g_wheel[si];
            *(int32_t*)(s.v + 0x31a) = groups;
            *(uint8_t**)(s.v + 0x316) = s.parts;
            *(const char**)(s.v + 0x32e) = "test";
            for (int g = 0; g < 2; ++g) {
                uint8_t* grp = s.parts + g * 0xc0;
                *(int32_t*)(grp + 0) = nw[g];
                *(int32_t*)(grp + 4) = np[g];
                for (int k = 0; k < 3; ++k) {
                    s.wheelPtr[g][k] = s.wheels[g][k];
                    s.wheels[g][k][0x20] = wheelFlags[g][k];
                }
                *(uint8_t***)(grp + 0x98) = s.wheelPtr[g];
                *(float**)(grp + 0x9c) = s.circ[g];
                for (int k = 0; k < 2; ++k) {
                    uint8_t* pv = s.pivots[g][k];
                    *(uint8_t**)(grp + 0xa0 + k * 4) = pv;
                    *(uint8_t**)(pv + 0x3c) = s.pivotObj[g][k][0];
                    *(uint8_t**)(pv + 0x30) = s.pivotObj[g][k][1];
                    *(uint8_t**)(pv + 0x34) = noArm[g][k][0] ? nullptr : s.pivotObj[g][k][2];
                    *(uint8_t**)(pv + 0x38) = noArm[g][k][1] ? nullptr : s.pivotObj[g][k][3];
                }
            }
            // The header shares group 0's block: emitters +0x10 (none), lights +0x34..+0x3c.
            *(uint8_t**)(s.parts + 0x10) = nullptr;
            *(int32_t*)(s.parts + 0x34) = lightsOn;
            for (int l = 0; l < 2; ++l) {
                *(uint8_t**)(s.parts + 0x38 + l * 4) = s.lights[l];
                *(int32_t*)(s.lights[l] + 8) = nLamps[l];
                for (int k = 0; k < 2; ++k) *(uint8_t**)(s.lights[l] + 0xc + k * 4) = s.lamps[l][k];
            }
        }
        orig(g_wheel[0].v, 0, dist);
        Vehicle_RollWheels(g_wheel[1].v, 0, dist);
        WheelSide& b = g_wheel[1];
        EXPECT_EQ(FirstDiff(a.wheels, b.wheels, sizeof a.wheels), -1, "wheels iteration %d", it);
        EXPECT_EQ(FirstDiff(a.pivotObj, b.pivotObj, sizeof a.pivotObj), -1, "pivots iteration %d", it);
        EXPECT_EQ(FirstDiff(a.lamps, b.lamps, sizeof a.lamps), -1, "lamps iteration %d", it);
        EXPECT_EQ(memcmp(a.circ, b.circ, sizeof a.circ), 0, "circumferences iteration %d", it);
    }
}

namespace rt3 {
void __fastcall Particle_ToBillboard(uint8_t* p, int, uint32_t* out, int32_t* count, const float* at);
}

DIFFTEST(render_ParticleToBillboard) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint32_t*, int32_t*, const float*))0x5662a0;
    static uint8_t p[0x80];
    float* cam = (float*)0xd97abc;
    float savedCam[12], savedQuad[12];
    memcpy(savedCam, cam, sizeof savedCam);
    memcpy(savedQuad, (void*)0xd97b38, sizeof savedQuad);
    int32_t& quads = hook::Global<int32_t>(0xd97ab8);
    int32_t savedQuads = quads;
    for (int it = 0; it < 5000; ++it) {
        for (int k = 0; k < 0x80; k += 4) *(float*)(p + k) = RndF(-10.0f, 10.0f);
        *(int32_t*)(p + 0x48) = (int32_t)(Rnd() % 200);
        *(int32_t*)(p + 0x4c) = Rnd() % 20 == 0 ? 0 : 1 + (int32_t)(Rnd() % 200);
        static const uint32_t modes[] = { 0, 0x20, 0x40, 0x80, 0x60, 0x20080000, 0x80000, 0x20000000 };
        *(uint32_t*)(p + 0x5c) = modes[Rnd() % 8] | (Rnd() % 2 ? 0x20080000 : 0);
        *(float*)(p + 0x40) = Rnd() % 4 ? RndF(-6.0f, 6.0f) : 0.0f;
        *(float*)(p + 0x60) = RndF(0.0f, 1.0f);
        *(uint32_t*)(p + 0x70) = Rnd() & 0xffffff;
        for (int k = 0; k < 12; ++k) cam[k] = RndF(-1.0f, 1.0f);
        float at[3] = { RndF(-100.0f, 100.0f), RndF(-100.0f, 100.0f), RndF(-100.0f, 100.0f) };
        uint32_t oa[36], ob[36];
        float qa[12];
        int32_t ca = 3, cb = 3;
        quads = 0;
        orig(p, 0, oa, &ca, at);
        memcpy(qa, (void*)0xd97b38, sizeof qa);
        int32_t na = quads;
        quads = 0;
        Particle_ToBillboard(p, 0, ob, &cb, at);
        EXPECT_EQ(ca, cb, "count iteration %d", it);
        EXPECT_EQ(na, quads, "quads iteration %d", it);
        EXPECT_EQ(memcmp(qa, (void*)0xd97b38, sizeof qa), 0, "scratch iteration %d", it);
        int d = FirstDiff(oa, ob, sizeof oa);
        EXPECT_EQ(d, -1, "vertices iteration %d (flags %08x): +0x%x %.9g vs %.9g; q1 %.9g %.9g %.9g vs %.9g %.9g %.9g", it,
                  *(uint32_t*)(p + 0x5c), d, d >= 0 ? *(float*)((uint8_t*)oa + (d & ~3)) : 0.0f,
                  d >= 0 ? *(float*)((uint8_t*)ob + (d & ~3)) : 0.0f, qa[3], qa[4], qa[5], ((float*)0xd97b38)[3],
                  ((float*)0xd97b38)[4], ((float*)0xd97b38)[5]);
    }
    memcpy(cam, savedCam, sizeof savedCam);
    memcpy((void*)0xd97b38, savedQuad, sizeof savedQuad);
    quads = savedQuads;
}

namespace rt3 {
void __fastcall TerrainPatch_Triangulate(uint8_t* g, int, int32_t x0, int32_t y0);
}

DIFFTEST(render_TerrainPatchTriangulate) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x565510;
    const size_t size = 0x3e900;
    uint8_t* ga = (uint8_t*)VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* gb = (uint8_t*)VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    static uint8_t grid[128 * 128];
    static uint32_t visA[300], visB[300];
    int32_t saveLists[64];
    memcpy(saveLists, (void*)0xd979b0, sizeof saveLists);
    int tris = 0;
    for (int it = 0; it < 2000; ++it) {
        int zeroOdds = 2 + (int)(Rnd() % 6);
        for (uint8_t& c : grid) c = (uint8_t)(Rnd() % zeroOdds == 0 ? 0 : 1 + Rnd() % 5);
        int32_t x0 = (int32_t)(Rnd() % 64), y0 = (int32_t)(Rnd() % 64);
        grid[y0 * 128 + x0] = (uint8_t)(1 + Rnd() % 5);
        memset(ga, 0, size);
        *(uint32_t*)(ga + 0x1f408) = Rnd() % 50 == 0 ? 0xfffffff0u : Rnd() % 1000;
        *(uint32_t**)(ga + 0x1f40c) = visA;
        *(uint8_t**)(ga + 0x1f414) = grid;
        *(int32_t*)(ga + 0x1f418) = 7;
        *(int32_t*)(ga + 0x3e83b) = 128;
        *(int32_t*)(ga + 4) = (int32_t)(Rnd() % 10);
        for (uint32_t& v : visA) v = Rnd() % 4 == 0 ? *(uint32_t*)(ga + 0x1f408) + 1 : Rnd() % 1000;
        memcpy(gb, ga, size);
        memcpy(visB, visA, sizeof visA);
        *(uint32_t**)(gb + 0x1f40c) = visB;
        orig(ga, 0, x0, y0);
        int32_t la[64];
        memcpy(la, (void*)0xd979b0, sizeof la);
        TerrainPatch_Triangulate(gb, 0, x0, y0);
        tris += *(int32_t*)(ga + 4) > 30;
        EXPECT_EQ(*(int32_t*)(ga + 4), *(int32_t*)(gb + 4), "vertex count iteration %d", it);
        int d = FirstDiff(ga + 8, gb + 8, 0x1f400);
        EXPECT_EQ(d, -1, "vertices iteration %d: +0x%x", it, d);
        EXPECT_EQ(*(int32_t*)(ga + 0x1f41c), *(int32_t*)(gb + 0x1f41c), "queue length iteration %d", it);
        EXPECT_EQ(memcmp(ga + 0x1f420, gb + 0x1f420, 0x2000), 0, "queue iteration %d", it);
        EXPECT_EQ(*(uint32_t*)(ga + 0x1f408), *(uint32_t*)(gb + 0x1f408), "stamp iteration %d", it);
        EXPECT_EQ(memcmp(visA, visB, sizeof visA), 0, "visited iteration %d", it);
        EXPECT_EQ(memcmp(la, (void*)0xd979b0, sizeof la), 0, "edge lists iteration %d", it);
        EXPECT_EQ((uint8_t*)*(int16_t**)(ga + 0x3e824) - ga, (uint8_t*)*(int16_t**)(gb + 0x3e824) - gb, "write end iteration %d", it);
        EXPECT_EQ(*(uint32_t*)(ga + 0x3e828) - (uint32_t)grid, *(uint32_t*)(gb + 0x3e828) - (uint32_t)grid, "row base iteration %d", it);
    }
    memcpy((void*)0xd979b0, saveLists, sizeof saveLists);
    VirtualFree(ga, 0, MEM_RELEASE);
    VirtualFree(gb, 0, MEM_RELEASE);
    EXPECT_EQ(tris > 300, true, "coverage: %d patches with over 30 vertices", tris);
}

namespace rt3 {
void __fastcall Emitter_Draw(uint8_t* e, int, uint8_t* out, int32_t* count);
}

namespace {
struct DrawSide {
    uint8_t e[0x240], obj[0x200], group[0x10];
    void* nodes[10][2];
    uint8_t parts[10][0x80];
    uint32_t list[3];
    float points[4][3];
    float* pointPtr[4];
    uint8_t h[8];
};
DrawSide g_draw[2];
} // namespace

DIFFTEST(render_EmitterDraw) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint8_t*, int32_t*))0x554e70;
    FakeEngine fe;
    static uint8_t camObj[0x40], clock[0x40];
    *(uint8_t**)(fe.e + 0x28) = camObj;
    uint8_t*& gclock = hook::Global<uint8_t*>(0x6d4030);
    uint8_t* sClock = gclock;
    gclock = clock;
    static HashMap map;
    memset(&map, 0, sizeof map);
    Hash_Init(&map, 0, 16, 8);
    Hash_Insert(&map, 0, (uint32_t)g_draw[0].e, (int32_t)g_draw[0].h);
    Hash_Insert(&map, 0, (uint32_t)g_draw[1].e, (int32_t)g_draw[1].h);
    *(HashMap**)(clock + 4) = &map;
    float savedCam[12];
    memcpy(savedCam, (void*)0xd97abc, sizeof savedCam);
    static uint8_t oa[200 * 0x18], ob[200 * 0x18];
    int drawn = 0;
    for (int it = 0; it < 3000; ++it) {
        DrawSide& a = g_draw[0];
        uint8_t* raw = (uint8_t*)&a;
        for (size_t k = 0; k + 4 <= sizeof a; k += 4) *(float*)(raw + k) = RndF(-4.0f, 4.0f);
        for (int k = 0; k < 10; ++k) {
            *(int32_t*)(a.parts[k] + 0x48) = (int32_t)(Rnd() % 100);
            *(int32_t*)(a.parts[k] + 0x4c) = 1 + (int32_t)(Rnd() % 100);
            *(uint32_t*)(a.parts[k] + 0x5c) = Rnd() & 0x200800e0;
        }
        for (int k = 0; k < 12; ++k) ((float*)0xd97abc)[k] = RndF(-1.0f, 1.0f);
        for (int k = 0x1c; k <= 0x24; k += 4) *(float*)(camObj + k) = RndF(-3000.0f, 3000.0f);
        *(float*)(clock + 0x28) = Rnd() % 10 == 0 ? 0.0f : RndF(0.0f, 3.0f);
        *(uint32_t*)(a.e + 0xac) = Rnd() % 2 ? 0x400 : 0;
        a.e[0xb0] = (uint8_t)(Rnd() % 4);
        *(int32_t*)(a.e + 0x30) = (int32_t)(Rnd() % 5);
        *(int32_t*)(a.e + 0x1e1) = (int32_t)(Rnd() % 2);
        int nLive = (int)(Rnd() % 11);
        memcpy(&g_draw[1], &g_draw[0], sizeof a);
        for (int si = 0; si < 2; ++si) {
            DrawSide& s = g_draw[si];
            for (int k = 0; k < 10; ++k) {
                s.nodes[k][0] = k + 1 < nLive ? s.nodes[k + 1] : nullptr;
                s.nodes[k][1] = s.parts[k];
            }
            s.list[0] = nLive ? (uint32_t)s.nodes[0] : 0;
            *(uint32_t**)(s.e + 4) = s.list;
            *(uint32_t**)(s.group + 4) = s.list;
            *(uint8_t**)(s.h + 4) = s.group;
            *(uint8_t**)(s.e + 0xc) = s.obj;
            for (int k = 0; k < 4; ++k) s.pointPtr[k] = s.points[k];
            *(float***)(s.e + 0x1dd) = s.pointPtr;
        }
        memset(oa, 0xcd, sizeof oa);
        memset(ob, 0xcd, sizeof ob);
        int32_t ca = 0, cb = 0;
        orig(g_draw[0].e, 0, oa, &ca);
        Emitter_Draw(g_draw[1].e, 0, ob, &cb);
        drawn += ca > 0;
        EXPECT_EQ(ca, cb, "count iteration %d", it);
        EXPECT_EQ(FirstDiff(oa, ob, sizeof oa), -1, "vertices iteration %d", it);
        EXPECT_EQ(*(uint32_t*)(g_draw[0].e + 0xdc), *(uint32_t*)(g_draw[1].e + 0xdc), "density iteration %d", it);
        EXPECT_EQ(*(int32_t*)(g_draw[0].e + 0x1e1), *(int32_t*)(g_draw[1].e + 0x1e1), "transformed flag iteration %d", it);
    }
    memcpy((void*)0xd97abc, savedCam, sizeof savedCam);
    gclock = sClock;
    Hash_Destroy(&map);
    EXPECT_EQ(drawn > 500, true, "coverage: %d draws", drawn);
}

namespace rt3 {
int32_t __fastcall Sound3D_Command(uint8_t* sys, int, uint8_t* h, int32_t* cmd);
}

namespace {
uint32_t g_ailLog[64][8];
int g_ailCount;
DWORD g_fakeTick;
void AilLog(uint32_t id, const uint32_t* args, int n) {
    if (g_ailCount >= 64) return;
    g_ailLog[g_ailCount][0] = id;
    for (int i = 0; i < n; ++i) g_ailLog[g_ailCount][1 + i] = args[i];
    ++g_ailCount;
}
void __stdcall FakeAil1(uint32_t a) { AilLog(1, &a, 1); }
void __stdcall FakeAil1b(uint32_t a) { AilLog(2, &a, 1); }
void __stdcall FakeAil2(uint32_t a, uint32_t b) { uint32_t v[2] = { a, b }; AilLog(3, v, 2); }
void __stdcall FakeAil2b(uint32_t a, uint32_t b) { uint32_t v[2] = { a, b }; AilLog(4, v, 2); }
void __stdcall FakeAil2c(uint32_t a, uint32_t b) { uint32_t v[2] = { a, b }; AilLog(5, v, 2); }
void __stdcall FakeAil3(uint32_t a, uint32_t b, uint32_t c) { uint32_t v[3] = { a, b, c }; AilLog(6, v, 3); }
void __stdcall FakeAil4(uint32_t a, uint32_t b, uint32_t c, uint32_t d) { uint32_t v[4] = { a, b, c, d }; AilLog(7, v, 4); }
void __stdcall FakeAil7(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g) {
    uint32_t v[7] = { a, b, c, d, e, f, g };
    AilLog(8, v, 7);
}
DWORD WINAPI FakeTick2() { return g_fakeTick; }
} // namespace

DIFFTEST(game_Sound3DCommand) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, uint8_t*, int32_t*))0x569f30;
    struct Patch {
        uint32_t iat;
        void* fn;
        void* saved;
    } patches[] = { { 0x5c8410, (void*)&FakeAil1 }, { 0x5c840c, (void*)&FakeAil1b }, { 0x5c842c, (void*)&FakeAil2 },
                    { 0x5c8424, (void*)&FakeAil2b }, { 0x5c8420, (void*)&FakeAil2c }, { 0x5c8428, (void*)&FakeAil3 },
                    { 0x5c83f0, (void*)&FakeAil4 }, { 0x5c83f8, (void*)&FakeAil7 }, { 0x5c8068, (void*)&FakeTick2 } };
    for (Patch& p : patches) {
        DWORD prot;
        VirtualProtect((void*)p.iat, 4, PAGE_READWRITE, &prot);
        p.saved = *(void**)p.iat;
        *(void**)p.iat = p.fn;
    }
    static uint8_t sys[0x80], view[0x60], ha[8], hb[8];
    static uint32_t ra[0x20], rb[0x20];
    uint8_t*& gview = hook::Global<uint8_t*>(0x6d402c);
    uint8_t* sView = gview;
    gview = view;
    float& vol = hook::Global<float>(0x624d5c);
    float sVol = vol;
    uint32_t logA[64][8];
    int calls = 0;
    for (int it = 0; it < 6000; ++it) {
        for (int k = 0; k < 0x80; k += 4) *(float*)(sys + k) = RndF(-2.0f, 2.0f);
        *(uint32_t*)(sys + 0x60) = Rnd() % 3 ? 0 : Rnd() & 0xff;
        for (int k = 8; k < 0x48; k += 4) *(float*)(view + k) = RndF(-2.0f, 2.0f);
        vol = RndF(0.0f, 1.0f);
        for (int k = 0; k < 0x20; ++k) ra[k] = Rnd();
        for (int k = 5; k < 0x10; ++k) ((float*)ra)[k] = Rnd() % 6 ? RndF(-500.0f, 500.0f) : 0.0f;
        ra[1 + 0x44 / 4] = Rnd() & 0xff;
        ra[1 + 0xc / 4] = Rnd() % 3;
        ra[1 + 8 / 4] = Rnd() % 2;
        ra[1 + 0x38 / 4] = Rnd() % 200;
        ra[1 + 0x3c / 4] = Rnd() % 2 ? 0 : Rnd() % 44100;
        ra[1 + 0x40 / 4] = Rnd() % 10;
        ra[1 + 0x4c / 4] = Rnd() % 1000;
        ra[1 + 0x50 / 4] = Rnd() % 3 ? 0 : Rnd() % 500;
        memcpy(rb, ra, sizeof ra);
        *(uint32_t**)(ha + 4) = ra + 1;
        *(uint32_t**)(hb + 4) = rb + 1;
        int32_t op = 2 + (int32_t)(Rnd() % 7);
        float bestA = RndF(0.0f, 100000.0f), bestB = bestA;
        int32_t ca[4] = { op, (int32_t)(Rnd() % 12), (int32_t)&bestA, 7 };
        int32_t cb[4] = { op, ca[1], (int32_t)&bestB, 7 };
        g_fakeTick = Rnd() % 2000;
        g_ailCount = 0;
        int32_t resA = orig(sys, 0, ha, ca);
        int na = g_ailCount;
        memcpy(logA, g_ailLog, sizeof logA);
        g_ailCount = 0;
        int32_t resB = Sound3D_Command(sys, 0, hb, cb);
        calls += na;
        EXPECT_EQ(resA, resB, "result op %d iteration %d", op, it);
        EXPECT_EQ(na, g_ailCount, "Miles calls op %d iteration %d", op, it);
        for (int c = 0; c < na && c < g_ailCount; ++c) {
            logA[c][1] = logA[c][1] == ra[0] ? 0 : logA[c][1] ^ 1; // handles differ only by side
            uint32_t hbv = g_ailLog[c][1] == rb[0] ? 0 : g_ailLog[c][1] ^ 1;
            g_ailLog[c][1] = hbv;
            EXPECT_EQ(memcmp(logA[c], g_ailLog[c], sizeof logA[c]), 0, "Miles call %d op %d iteration %d", c, op, it);
        }
        EXPECT_EQ(memcmp(ra, rb, sizeof ra), 0, "record op %d iteration %d", op, it);
        EXPECT_EQ(ca[1] == cb[1] && ca[3] == cb[3] && *(uint32_t*)&bestA == *(uint32_t*)&bestB, true, "query op %d iteration %d", op, it);
    }
    for (Patch& p : patches) *(void**)p.iat = p.saved;
    gview = sView;
    vol = sVol;
    EXPECT_EQ(calls > 2000, true, "coverage: %d Miles calls", calls);
}

namespace rt3 {
int32_t __stdcall CollisionMesh_Load(uint8_t* m);
}

DIFFTEST(game_CollisionMeshLoad) {
    auto orig = (int32_t(__stdcall*)(uint8_t*))0x566b80;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_mesh.bin");
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t*& gpack = hook::Global<uint8_t*>(0x6d4020);
    uint8_t* sPack = gpack;
    PtrList*& meshes = hook::Global<PtrList*>(0xd97bb8);
    PtrList* sMeshes = meshes;
    float sOff[4];
    memcpy(sOff, (void*)0xd97ba8, sizeof sOff);
    static uint8_t ma[0x60], mb[0x60];
    int faced = 0;
    for (int it = 0; it < 400; ++it) {
        int32_t nv = 1 + (int32_t)(Rnd() % 40), nf = Rnd() % 6 == 0 ? 0 : (int32_t)(Rnd() % 30);
        FILE* w = fopen(path, "wb");
        if (!w) return;
        int32_t hdr[3] = { (int32_t)Rnd(), nv, nf };
        fwrite(hdr, 4, 3, w);
        for (int k = 0; k < nv * 3; ++k) {
            float f = RndF(-50.0f, 50.0f);
            fwrite(&f, 4, 1, w);
        }
        for (int k = 0; k < nf; ++k) {
            for (int j = 0; j < 3; ++j) {
                int32_t idx = (int32_t)(Rnd() % nv);
                fwrite(&idx, 4, 1, w);
            }
            for (int j = 0; j < 9; ++j) {
                float f = Rnd() % 20 == 0 ? 0.0f : RndF(-2.0f, 2.0f);
                fwrite(&f, 4, 1, w);
            }
            for (int j = 0; j < 7; ++j) {
                uint32_t u = Rnd();
                fwrite(&u, 4, 1, w);
            }
        }
        fclose(w);
        for (int k = 0; k < 4; ++k) ((float*)0xd97ba8)[k] = RndF(-5.0f, 5.0f);
        memset(ma, 0x11, sizeof ma);
        memset(mb, 0x11, sizeof mb);
        gpack = pack;
        *(int32_t*)(pack + 0x427bc) = 0;
        FILE* fa = xfopen(path, "rb");
        *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)fa + 0x10);
        int32_t resA = orig(ma);
        xfclose(fa);
        FILE* fb = xfopen(path, "rb");
        *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)fb + 0x10);
        int32_t resB = CollisionMesh_Load(mb);
        xfclose(fb);
        faced += nf > 0;
        EXPECT_EQ(resA, resB, "result iteration %d", it);
        EXPECT_EQ(memcmp(*(void**)ma, *(void**)mb, nv * 12), 0, "vertices iteration %d", it);
        uint8_t* fa4 = *(uint8_t**)(ma + 4);
        uint8_t* fb4 = *(uint8_t**)(mb + 4);
        EXPECT_EQ(fa4 == nullptr, fb4 == nullptr, "faces iteration %d", it);
        for (int k = 0; fa4 && fb4 && k < nf; ++k) {
            for (int j = 0; j < 3; ++j)
                EXPECT_EQ(((uint8_t**)(fa4 + k * 0x4c))[j] - *(uint8_t**)ma, ((uint8_t**)(fb4 + k * 0x4c))[j] - *(uint8_t**)mb,
                          "face %d vertex %d iteration %d", k, j, it);
            EXPECT_EQ(FirstDiff(fa4 + k * 0x4c + 0xc, fb4 + k * 0x4c + 0xc, 0x40), -1, "face %d iteration %d", k, it);
        }
        uint8_t ca[0x60], cb[0x60];
        memcpy(ca, ma, sizeof ca);
        memcpy(cb, mb, sizeof cb);
        memset(ca, 0, 8);
        memset(cb, 0, 8);
        memset(ca + 0x41, 0, 4);
        memset(cb + 0x41, 0, 4);
        EXPECT_EQ(FirstDiff(ca, cb, sizeof ca), -1, "mesh iteration %d", it);
        xfree(*(void**)ma);
        xfree(*(void**)mb);
        if (fa4) xfree(fa4);
        if (fb4) xfree(fb4);
    }
    gpack = sPack;
    meshes = sMeshes;
    memcpy((void*)0xd97ba8, sOff, sizeof sOff);
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
    EXPECT_EQ(faced > 200, true, "coverage: %d meshes with faces", faced);
}

namespace rt3 {
int32_t __fastcall CollisionBlock_Load(uint8_t*, int, uint32_t* out);
}

DIFFTEST(game_CollisionBlockLoad) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x567190;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_block.bin");
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t*& gpack = hook::Global<uint8_t*>(0x6d4020);
    uint8_t* sPack = gpack;
    PtrList*& meshes = hook::Global<PtrList*>(0xd97bb8);
    PtrList* sMeshes = meshes;
    int32_t& version = hook::Global<int32_t>(0xd97bb0);
    int32_t sVersion = version;
    float sOff[4];
    memcpy(sOff, (void*)0xd97ba8, sizeof sOff);
    for (int it = 0; it < 200; ++it) {
        int32_t n = (int32_t)(Rnd() % 4);
        bool meshesTag = Rnd() % 4 != 0;
        version = (int32_t)(Rnd() % 5);
        FILE* w = fopen(path, "wb");
        if (!w) return;
        int32_t hdr[2] = { (int32_t)Rnd(), n };
        fwrite(hdr, 4, 2, w);
        if (version > 2)
            for (int k = 0; k < 3; ++k) {
                float f = RndF(-9.0f, 9.0f);
                fwrite(&f, 4, 1, w);
            }
        for (int m = 0; m < (meshesTag && n > 0 ? n : 1); ++m) { // the tag peek needs one header
            int32_t nv = 1 + (int32_t)(Rnd() % 12), nf = (int32_t)(Rnd() % 8);
            int32_t mh[3] = { m == 0 ? (meshesTag ? 'I' : 'X') : (int32_t)Rnd(), nv, nf };
            fwrite(mh, 4, 3, w);
            for (int k = 0; k < nv * 3; ++k) {
                float f = RndF(-50.0f, 50.0f);
                fwrite(&f, 4, 1, w);
            }
            for (int k = 0; k < nf; ++k) {
                for (int j = 0; j < 3; ++j) {
                    int32_t idx = (int32_t)(Rnd() % nv);
                    fwrite(&idx, 4, 1, w);
                }
                for (int j = 0; j < 16; ++j) {
                    float f = RndF(-2.0f, 2.0f);
                    fwrite(&f, 4, 1, w);
                }
            }
        }
        fclose(w);
        uint32_t oa[6], ob[6];
        memset(oa, 0x22, sizeof oa);
        memset(ob, 0x22, sizeof ob);
        uint32_t offA[3], offB[3];
        gpack = pack;
        *(int32_t*)(pack + 0x427bc) = 0;
        *(void**)(pack + 0x20428) = nullptr;
        FILE* fa = xfopen(path, "rb");
        *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)fa + 0x10);
        int32_t ra = orig(nullptr, 0, oa);
        memcpy(offA, (void*)0xd97ba8, 8);
        offA[2] = *(uint32_t*)0xd97bb4;
        xfclose(fa);
        FILE* fb = xfopen(path, "rb");
        *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)fb + 0x10);
        int32_t rb = CollisionBlock_Load(nullptr, 0, ob);
        memcpy(offB, (void*)0xd97ba8, 8);
        offB[2] = *(uint32_t*)0xd97bb4;
        xfclose(fb);
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        EXPECT_EQ(oa[0] == ob[0] && oa[1] == ob[1] && oa[2] == ob[2] && oa[5] == ob[5], true, "header iteration %d", it);
        EXPECT_EQ(memcmp(offA, offB, sizeof offA), 0, "offset iteration %d", it);
        EXPECT_EQ((oa[3] == 0) == (ob[3] == 0) && (oa[4] == 0x22222222) == (ob[4] == 0x22222222), true, "arrays iteration %d", it);
        bool loadedA = meshesTag && oa[3] && oa[3] != 0x22222222u, loadedB = meshesTag && ob[3] && ob[3] != 0x22222222u;
        for (int m = 0; loadedA && loadedB && m < n; ++m) {
            uint8_t* ma = (uint8_t*)oa[3] + m * 0x45;
            uint8_t* mb = (uint8_t*)ob[3] + m * 0x45;
            if (*(int32_t*)(ma + 0xc) > 0) // without faces the size stays uninitialized
                EXPECT_EQ(*(uint32_t*)(ma + 0x10), *(uint32_t*)(mb + 0x10), "mesh %d size iteration %d", m, it);
            EXPECT_EQ(memcmp(*(void**)ma, *(void**)mb, *(int32_t*)(ma + 8) * 12), 0, "mesh %d vertices iteration %d", m, it);
        }
        if (loadedA) xfree((void*)oa[3]);
        if (loadedB) xfree((void*)ob[3]);
    }
    gpack = sPack;
    meshes = sMeshes;
    version = sVersion;
    memcpy((void*)0xd97ba8, sOff, sizeof sOff);
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
void __fastcall Scenario_ApplyFixes(uint8_t* scn, int, const char* name);
}

DIFFTEST(game_ScenarioApplyFixes) {
    auto orig = (void(__fastcall*)(uint8_t*, int, const char*))0x442c30;
    static const char* scenarios[] = { "Germany", "Go West!", "Orient Express", "Central Pacific", "State of Germany",
                                       "Dutchlantis", "New Beginnings", "Germantown", "Britain", "South East Australia",
                                       "Tex-Mex", "Somewhere Else" };
    static const char* ruleNames[] = { "Open Aus", "STATUS", "Setup P3 Welcome", "Setup P2 Welcome", "ba- more meat.",
                                       "ba- couting meat", "ba- meat pays", "ha- The investors", "ha- counting profit",
                                       "ha- Industry pays", "SP - GOLD", "Labor", "The American", "Test connections",
                                       "New Zealand", "Reward 1", "a- Steam theme 2", "MP- STATUS", "Win - Gold",
                                       "Win - Silver", "Win - Bronze", "Other" };
    static const int32_t types[] = { 0x1b, 0x1a, 0x1d, 7, 0x3c, -1, 0x3b, 0x38, 0x92, 0xce, 1, 0x2b, 0x3d, 0x20 };
    static const int32_t values[] = { 1, 2, 3, 0x989680, 0x4c4b40, -5, -0xf, 0x19, 0x2bf20, 0x38, 0x20 };
    static Collection rules, cities, btypes;
    memset(&rules, 0, sizeof rules);
    memset(&cities, 0, sizeof cities);
    memset(&btypes, 0, sizeof btypes);
    Collection_Construct(&rules, 0, 1, 0x800, 0x20, 0x20, 0, 0, 0);
    Collection_Construct(&cities, 0, 1, 0x270, 0x70, 0x10, 0, 0, 0);
    Collection_Construct(&btypes, 0, 1, 0x40, 8, 8, 0, 0, 0);
    static uint8_t blank[0x800];
    for (int k = 0; k < 0x20; ++k) Collection_Allocate(&rules, 0, blank, 0x800, 0, 0);
    for (int k = 0; k < 0x68; ++k) Collection_Allocate(&cities, 0, blank, 0x270, 0, 0);
    for (int k = 0; k < 6; ++k) Collection_Allocate(&btypes, 0, blank, 0x40, 0, 0);
    Collection*& grules = hook::Global<Collection*>(0x62be18);
    Collection*& gcities = hook::Global<Collection*>(0x62bae0);
    Collection*& gtypes = hook::Global<Collection*>(0x62b268);
    Collection *sRules = grules, *sCities = gcities, *sTypes = gtypes;
    grules = &rules;
    gcities = &cities;
    gtypes = &btypes;
    static uint8_t pool[0x20][4][0x40];
    static uint8_t scn[0x6800];
    static uint8_t snapRules[0x20][0x800], snapPool[sizeof pool], snapScn[sizeof scn], snapCities[0x68][0x270], snapTypes[6][0x40];
    auto snap = [&]() {
        for (int k = 0; k < 0x20; ++k) memcpy(snapRules[k], Collection_Get(&rules, 0, k + 1), 0x800);
        for (int k = 0; k < 0x68; ++k) memcpy(snapCities[k], Collection_Get(&cities, 0, k + 1), 0x270);
        for (int k = 0; k < 6; ++k) memcpy(snapTypes[k], Collection_Get(&btypes, 0, k + 1), 0x40);
        memcpy(snapPool, pool, sizeof pool);
        memcpy(snapScn, scn, sizeof scn);
    };
    auto restore = [&]() {
        for (int k = 0; k < 0x20; ++k) memcpy(Collection_Get(&rules, 0, k + 1), snapRules[k], 0x800);
        for (int k = 0; k < 0x68; ++k) memcpy(Collection_Get(&cities, 0, k + 1), snapCities[k], 0x270);
        for (int k = 0; k < 6; ++k) memcpy(Collection_Get(&btypes, 0, k + 1), snapTypes[k], 0x40);
        memcpy(pool, snapPool, sizeof pool);
        memcpy(scn, snapScn, sizeof scn);
    };
    static uint8_t outRules[0x20][0x800], outPool[sizeof pool], outScn[sizeof scn], outCities[0x68][0x270], outTypes[6][0x40];
    int changed = 0;
    for (int it = 0; it < 3000; ++it) {
        for (int k = 0; k < 0x20; ++k) {
            uint8_t* r = (uint8_t*)Collection_Get(&rules, 0, k + 1);
            memset(r, 0, 0x800);
            strcpy((char*)(r + 0x18), ruleNames[Rnd() % 22]);
            strcpy((char*)(r + 0xe0), Rnd() % 2 ? "Amount of track already laid [Company Track Miles]. Rest 25 x 25"
                                                : "Binari posati [Company Track Miles]. 25");
            for (int j = 0; j < 4; ++j) {
                uint8_t* c = pool[k][j];
                memset(c, 0, 0x40);
                *(int32_t*)c = types[Rnd() % 14];
                c[4] = (uint8_t)(Rnd() % 3 == 0 ? 0 : Rnd() % 2 ? 4 : 5);
                *(int32_t*)(c + 9) = values[Rnd() % 11];
                c[0xd] = (uint8_t)(Rnd() % 3 ? 2 : 0);
                *(int32_t*)(c + 0xe) = Rnd() % 2 ? 0x20 : 0;
                *(int32_t*)(c + 0x12) = Rnd() % 2 ? 0x38 : 0;
                *(int32_t*)(c + 4) = Rnd() % 3 == 0 ? values[Rnd() % 11] : *(int32_t*)(c + 4);
            }
            *(uint8_t**)(r + 0) = Rnd() % 6 ? pool[k][0] : nullptr;
            *(uint8_t**)(r + 4) = Rnd() % 4 ? pool[k][1] : nullptr;
            *(uint8_t**)(r + 8) = Rnd() % 4 ? pool[k][2] : nullptr;
            *(uint8_t**)(pool[k][0] + 0x1a) = Rnd() % 3 ? pool[k][3] : nullptr;
            *(uint8_t**)(pool[k][1] + 0x24) = Rnd() % 3 ? pool[k][3] : nullptr;
            r[0x7ef] = (uint8_t)(Rnd() % 10);
            r[0x7f4] = (uint8_t)(Rnd() % 2);
            r[0x7f9] = (uint8_t)(Rnd() % 3);
            r[0x7fa] = (uint8_t)(Rnd() % 2);
        }
        for (int k = 0; k < 0x68; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k + 1);
            *(float*)(c + 0x25a) = RndF(10.0f, 35.0f);
            *(float*)(c + 0x25e) = RndF(10.0f, 35.0f);
        }
        static const char* tnames[] = { "PostOffice", "Mill", "postoffice", "Bank" };
        for (int k = 0; k < 6; ++k) {
            uint8_t* t = (uint8_t*)Collection_Get(&btypes, 0, k + 1);
            strcpy((char*)(t + 4), tnames[Rnd() % 4]);
            *(int32_t*)(t + 0x22) = Rnd() % 2 ? 25000 : 30000;
        }
        memset(scn, 0, sizeof scn);
        scn[0x66de] = (uint8_t)(Rnd() % 2);
        strcpy((char*)(scn + 0x4f30), "Paris is connected to Berlin, and a connection between Paris and Rome.");
        const char* name = scenarios[Rnd() % 12];
        snap();
        orig(scn, 0, name);
        for (int k = 0; k < 0x20; ++k) memcpy(outRules[k], Collection_Get(&rules, 0, k + 1), 0x800);
        for (int k = 0; k < 0x68; ++k) memcpy(outCities[k], Collection_Get(&cities, 0, k + 1), 0x270);
        for (int k = 0; k < 6; ++k) memcpy(outTypes[k], Collection_Get(&btypes, 0, k + 1), 0x40);
        memcpy(outPool, pool, sizeof pool);
        memcpy(outScn, scn, sizeof scn);
        changed += memcmp(outRules, snapRules, sizeof outRules) || memcmp(outPool, snapPool, sizeof pool) ||
                   memcmp(outCities, snapCities, sizeof outCities) || memcmp(outScn, snapScn, sizeof scn);
        restore();
        Scenario_ApplyFixes(scn, 0, name);
        for (int k = 0; k < 0x20; ++k)
            EXPECT_EQ(FirstDiff(outRules[k], Collection_Get(&rules, 0, k + 1), 0x800), -1, "rule %d %s iteration %d", k + 1, name, it);
        for (int k = 0; k < 0x68; ++k)
            EXPECT_EQ(memcmp(outCities[k], Collection_Get(&cities, 0, k + 1), 0x270), 0, "city %d %s iteration %d", k + 1, name, it);
        for (int k = 0; k < 6; ++k)
            EXPECT_EQ(memcmp(outTypes[k], Collection_Get(&btypes, 0, k + 1), 0x40), 0, "type %d iteration %d", k + 1, it);
        EXPECT_EQ(FirstDiff(outPool, pool, sizeof pool), -1, "conditions %s iteration %d", name, it);
        EXPECT_EQ(FirstDiff(outScn, scn, sizeof scn), -1, "scenario %s iteration %d", name, it);
    }
    grules = sRules;
    gcities = sCities;
    gtypes = sTypes;
    Collection_Destruct(&rules);
    Collection_Destruct(&cities);
    Collection_Destruct(&btypes);
    EXPECT_EQ(changed > 300, true, "coverage: %d runs changed something", changed);
}

namespace rt3 {
int32_t __fastcall Widget_IsVisible(uint8_t* w);
int32_t __fastcall Widget_IsEnabled(uint8_t* w);
void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
void __fastcall SceneObj_ApplyStack(uint8_t* o, int, void* ms);
}

namespace {
// A fake ID3DXMatrixStack: GetTop (+0x44), Push (+0x10), MultMatrixLocal (+0x20),
// Pop (+0xc) on a small array of matrices; every call is logged.
struct FakeStack {
    void** vtbl;
    float m[16][16];
    int depth;
    uint32_t log[256];
    int n;
};
void StackLog(FakeStack* s, uint32_t v) {
    if (s->n < 256) s->log[s->n++] = v;
}
const float* __stdcall FakeTop(FakeStack* s) {
    StackLog(s, 1);
    return s->m[s->depth];
}
long __stdcall FakePush(FakeStack* s) {
    StackLog(s, 2);
    memcpy(s->m[s->depth + 1], s->m[s->depth], 0x40);
    ++s->depth;
    return 0;
}
long __stdcall FakeMult(FakeStack* s, const float* x) {
    StackLog(s, 3);
    float r[16];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            double v = 0;
            for (int k = 0; k < 4; ++k) v += (double)x[i * 4 + k] * s->m[s->depth][k * 4 + j];
            r[i * 4 + j] = (float)v;
        }
    memcpy(s->m[s->depth], r, sizeof r);
    for (int k = 0; k < 16; ++k) StackLog(s, *(const uint32_t*)&x[k]);
    return 0;
}
long __stdcall FakePop(FakeStack* s) {
    StackLog(s, 4);
    --s->depth;
    return 0;
}
void* g_stackVtbl[0x48 / 4];
struct SceneSide {
    uint8_t objs[8][0x200];
    PtrList lists[8];
};
SceneSide g_scene[2];
} // namespace

DIFFTEST(render_SceneTreeHelpers) {
    auto origVis = (int32_t(__fastcall*)(uint8_t*))0x558670;
    auto origEn = (int32_t(__fastcall*)(uint8_t*))0x5586c0;
    auto origTint = (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, int32_t))0x52e680;
    auto origStack = (void(__fastcall*)(uint8_t*, int, void*))0x530690;
    FakeEngine fe;
    g_stackVtbl[0x44 / 4] = (void*)&FakeTop;
    g_stackVtbl[0x10 / 4] = (void*)&FakePush;
    g_stackVtbl[0x20 / 4] = (void*)&FakeMult;
    g_stackVtbl[0xc / 4] = (void*)&FakePop;
    static uint8_t widgets[6][0x100], handlers[6][0x80];
    static FakeStack sa, sb;
    sa.vtbl = sb.vtbl = g_stackVtbl;
    for (int it = 0; it < 3000; ++it) {
        // Widget chains.
        for (int k = 0; k < 6; ++k) {
            widgets[k][0x6a] = (uint8_t)(Rnd() % 4 ? 3 : Rnd() & 3);
            widgets[k][0x68] = (uint8_t)(Rnd() % 4 ? 3 : Rnd() & 3);
            *(uint8_t**)(widgets[k] + 0x86) = Rnd() % 8 ? handlers[k] : nullptr;
            *(void**)(handlers[k] + 0x5c) = Rnd() % 6 ? (void*)1 : nullptr;
            *(void**)(handlers[k] + 0x60) = Rnd() % 6 ? (void*)1 : nullptr;
            *(uint8_t**)(widgets[k] + 0x4b) = k + 1 < 6 && Rnd() % 3 ? widgets[k + 1] : nullptr;
        }
        fe.e[0x57] = (uint8_t)(Rnd() % 5 == 0);
        int w = (int)(Rnd() % 6);
        EXPECT_EQ(origVis(widgets[w]), Widget_IsVisible(widgets[w]), "visible iteration %d", it);
        EXPECT_EQ(origEn(widgets[w]), Widget_IsEnabled(widgets[w]), "enabled iteration %d", it);
        // Scene trees: object 0 is the root, others hang below by random parents.
        SceneSide& a = g_scene[0];
        for (int k = 0; k < 8; ++k)
            for (int j = 0; j < 0x200; j += 4) *(float*)(a.objs[k] + j) = RndF(-2.0f, 2.0f);
        int parent[8];
        bool hasKids[8] = {};
        for (int k = 1; k < 8; ++k) {
            parent[k] = (int)(Rnd() % k);
            hasKids[parent[k]] = true;
        }
        for (int k = 0; k < 8; ++k) a.objs[k][0x20] = (uint8_t)(Rnd() % 4 == 0 ? 2 : 0);
        memcpy(&g_scene[1], &g_scene[0], sizeof a);
        for (int si = 0; si < 2; ++si) {
            SceneSide& s = g_scene[si];
            for (int k = 0; k < 8; ++k) {
                PtrList_Init(&s.lists[k]);
                *(PtrList**)(s.objs[k] + 0x75) = hasKids[k] || Rnd() % 4 == 0 ? &s.lists[k] : nullptr;
            }
            for (int k = 1; k < 8; ++k)
                if (*(PtrList**)(s.objs[parent[k]] + 0x75)) PtrList_Append(&s.lists[parent[k]], 0, s.objs[k]);
        }
        uint32_t r = Rnd() % 3 ? Rnd() : 0, g = Rnd() % 3 ? Rnd() : 0, b = Rnd() % 3 ? Rnd() : 0;
        int32_t deep = (int32_t)(Rnd() % 2);
        int op = (int)(Rnd() % 2);
        if (op == 0) {
            origTint(g_scene[0].objs[0], 0, r, g, b, deep);
            SceneObj_SetTint(g_scene[1].objs[0], 0, r, g, b, deep);
        } else {
            for (int k = 0; k < 16; ++k) sa.m[0][k] = sb.m[0][k] = RndF(-1.0f, 1.0f);
            sa.depth = sb.depth = 0;
            sa.n = sb.n = 0;
            origStack(g_scene[0].objs[0], 0, &sa);
            SceneObj_ApplyStack(g_scene[1].objs[0], 0, &sb);
            EXPECT_EQ(sa.n == sb.n && !memcmp(sa.log, sb.log, sa.n * 4), true, "stack calls iteration %d", it);
        }
        for (int k = 0; k < 8; ++k) {
            *(void**)(g_scene[0].objs[k] + 0x75) = nullptr;
            *(void**)(g_scene[1].objs[k] + 0x75) = nullptr;
            EXPECT_EQ(FirstDiff(g_scene[0].objs[k], g_scene[1].objs[k], 0x200), -1, "object %d op %d iteration %d", k, op, it);
            PtrList_Clear(&g_scene[0].lists[k]);
            PtrList_Clear(&g_scene[1].lists[k]);
        }
    }
}

namespace rt3 {
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from);
struct InputEvent;
InputEvent* __fastcall Input_PopEvent(uint8_t* q, int, InputEvent* out);
}

DIFFTEST(game_TrackLinksAndInputPop) {
    auto origLinks = (int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x48e050;
    auto origPop = (void*(__fastcall*)(uint8_t*, int, void*))0x54ea20;
    static Collection nodes;
    memset(&nodes, 0, sizeof nodes);
    Collection_Construct(&nodes, 0, 1, 0x220, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x220];
    for (int k = 0; k < 0x18; ++k) Collection_Allocate(&nodes, 0, blank, 0x220, 0, 0);
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &nodes;
    static uint8_t qa[0xb00], qb[0xb00];
    int hits = 0;
    for (int it = 0; it < 4000; ++it) {
        for (int k = 1; k <= 0x18; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            *(int32_t*)(n + 0x202) = k;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % 0x18);
        }
        int32_t start = 1 + (int32_t)(Rnd() % 0x18), target = 1 + (int32_t)(Rnd() % 0x18), depth = 1 + (int32_t)(Rnd() % 6);
        int32_t from = Rnd() % 2 ? -1 : 1 + (int32_t)(Rnd() % 0x18);
        uint8_t* sn = (uint8_t*)Collection_Get(&nodes, 0, start);
        int32_t ra = origLinks(sn, 0, target, depth, from);
        int32_t rb = Track_LinksWithin(sn, 0, target, depth, from);
        hits += ra;
        EXPECT_EQ(ra, rb, "links iteration %d", it);
        // Input queue.
        for (int k = 0; k < 0xb00; k += 4) *(uint32_t*)(qa + k) = Rnd();
        int32_t n = (int32_t)(Rnd() % 8);
        *(int32_t*)(qa + 0xa88) = n;
        *(int32_t*)(qa + 0xa90) = Rnd() % 6 == 0;
        *(int32_t*)(qa + 0x100) = Rnd() % 4 == 0;
        for (int k = 0; k < 8; ++k) *(uint32_t*)(qa + 0x128 + k * 0x30) = Rnd() % 2 ? 2 : Rnd() % 4;
        memcpy(qb, qa, sizeof qa);
        uint32_t ea[12], eb[12];
        memset(ea, 0x55, sizeof ea);
        memset(eb, 0x55, sizeof eb);
        void* pa = origPop(qa, 0, ea);
        void* pb = Input_PopEvent(qb, 0, (InputEvent*)eb);
        EXPECT_EQ(pa == (void*)ea && pb == (void*)eb, true, "return iteration %d", it);
        EXPECT_EQ(memcmp(ea, eb, sizeof ea), 0, "event iteration %d", it);
        EXPECT_EQ(FirstDiff(qa, qb, sizeof qa), -1, "queue iteration %d", it);
    }
    gnodes = sNodes;
    Collection_Destruct(&nodes);
    EXPECT_EQ(hits > 500, true, "coverage: %d reachable", hits);
}

namespace rt3 {
void __fastcall Track_PairWith(uint8_t* t, int, int32_t id, int32_t mutual);
}

DIFFTEST(game_TrackPairWith) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x48e820;
    static Collection nodes;
    memset(&nodes, 0, sizeof nodes);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 12;
    for (int k = 0; k < N; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &nodes;
    static uint8_t snapA[N][0x260];
    int paired = 0;
    for (int it = 0; it < 4000; ++it) {
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            memset(n, 0, 0x260);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, 2000.0f);
            *(int32_t*)(n + 0x202) = k;
            *(int32_t*)(n + 0x206) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % N);
            *(int32_t*)(n + 0x20a) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % N);
            *(int32_t*)(n + 0x253) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % (N + 2));
        }
        static uint8_t init[N][0x260];
        for (int k = 0; k < N; ++k) memcpy(init[k], Collection_Get(&nodes, 0, k + 1), 0x260);
        int32_t self = 1 + (int32_t)(Rnd() % N);
        int32_t id = Rnd() % 5 == 0 ? -1 : 1 + (int32_t)(Rnd() % (N + 2));
        int32_t mutual = (int32_t)(Rnd() % 2);
        orig((uint8_t*)Collection_Get(&nodes, 0, self), 0, id, mutual);
        for (int k = 0; k < N; ++k) memcpy(snapA[k], Collection_Get(&nodes, 0, k + 1), 0x260);
        for (int k = 0; k < N; ++k) memcpy(Collection_Get(&nodes, 0, k + 1), init[k], 0x260);
        Track_PairWith((uint8_t*)Collection_Get(&nodes, 0, self), 0, id, mutual);
        for (int k = 0; k < N; ++k) {
            EXPECT_EQ(memcmp(snapA[k], Collection_Get(&nodes, 0, k + 1), 0x260), 0, "node %d iteration %d", k + 1, it);
            paired += *(int32_t*)(snapA[k] + 0x253) != *(int32_t*)(init[k] + 0x253);
        }
    }
    gnodes = sNodes;
    Collection_Destruct(&nodes);
    EXPECT_EQ(paired > 300, true, "coverage: %d pairings changed", paired);
}

namespace rt3 {
void __fastcall Track_MatchNeighbourHeights(uint8_t* t);
}

DIFFTEST(game_TrackMatchNeighbourHeights) {
    auto orig = (void(__fastcall*)(uint8_t*))0x48b120;
    static Collection nodes;
    memset(&nodes, 0, sizeof nodes);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 10;
    for (int k = 0; k < N; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &nodes;
    FakeEngine fe;
    static uint8_t init[N][0x260], outA[N][0x260];
    int changed = 0;
    for (int it = 0; it < 3000; ++it) {
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            memset(n, 0, 0x260);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = (float)(Rnd() % 4) * 10.0f;
            *(int32_t*)(n + 0x202) = k;
            *(int32_t*)(n + 0x206) = *(int32_t*)(n + 0x20a) = *(int32_t*)(n + 0x20e) = -1;
            n[0x201] = (uint8_t)Rnd();
            n[0x216] = (uint8_t)(Rnd() % 2);
        }
        // A chain 1 - 2 - ... - N; each joint uses +0x206 or +0x20a on either side, and a
        // few pieces get a one-way switch link (+0x20e) further down the chain.
        for (int k = 1; k < N; ++k) {
            uint8_t* a = (uint8_t*)Collection_Get(&nodes, 0, k);
            uint8_t* b = (uint8_t*)Collection_Get(&nodes, 0, k + 1);
            uint32_t sa = *(int32_t*)(a + 0x206) == -1 && Rnd() % 2 ? 0x206 : 0x20a;
            uint32_t sb = *(int32_t*)(b + 0x206) == -1 ? 0x206 : 0x20a;
            if (*(int32_t*)(a + sa) != -1) continue;
            *(int32_t*)(a + sa) = k + 1;
            *(int32_t*)(b + sb) = k;
        }
        for (int k = 1; k + 2 <= N; ++k)
            if (Rnd() % 4 == 0) *(int32_t*)((uint8_t*)Collection_Get(&nodes, 0, k) + 0x20e) = k + 2;
        for (int k = 0; k < N; ++k) memcpy(init[k], Collection_Get(&nodes, 0, k + 1), 0x260);
        int32_t start = 1 + (int32_t)(Rnd() % N);
        orig((uint8_t*)Collection_Get(&nodes, 0, start));
        for (int k = 0; k < N; ++k) memcpy(outA[k], Collection_Get(&nodes, 0, k + 1), 0x260);
        for (int k = 0; k < N; ++k) memcpy(Collection_Get(&nodes, 0, k + 1), init[k], 0x260);
        Track_MatchNeighbourHeights((uint8_t*)Collection_Get(&nodes, 0, start));
        for (int k = 0; k < N; ++k) {
            EXPECT_EQ(FirstDiff(outA[k], Collection_Get(&nodes, 0, k + 1), 0x260), -1, "node %d iteration %d", k + 1, it);
            changed += memcmp(outA[k], init[k], 0x260) != 0;
        }
    }
    gnodes = sNodes;
    Collection_Destruct(&nodes);
    EXPECT_EQ(changed > 500, true, "coverage: %d nodes changed", changed);
}

namespace rt3 {
void __fastcall Water_FloodFill(uint8_t* m, int, int32_t x, int32_t y, int32_t dir, int32_t flag);
}

DIFFTEST(game_WaterFloodFill) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, int32_t))0x44be20;
    const int32_t N = 24;
    static uint8_t map[0x2200], recsA[N * N * 3], recsB[N * N * 3], visA[N * N], visB[N * N];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15dd) = N;
    uint8_t*& gvis = hook::Global<uint8_t*>(0x62c128);
    uint8_t* sVis = gvis;
    int big = 0;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 0; k < N * N; ++k) {
            recsA[k * 3] = (uint8_t)(Rnd() % 8);
            recsA[k * 3 + 1] = (uint8_t)Rnd();
            recsA[k * 3 + 2] = (uint8_t)(Rnd() % 3 == 0 ? 0x20 | (Rnd() & 0xdf) : Rnd() & 0xdf);
        }
        int cx = (int)(Rnd() % N), cy = (int)(Rnd() % N), r = 2 + (int)(Rnd() % 8);
        for (int y = 0; y < N; ++y)
            for (int x = 0; x < N; ++x)
                if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r) recsA[(y * N + x) * 3] = (uint8_t)((recsA[(y * N + x) * 3] & 0xf8) | 2);
        memcpy(recsB, recsA, sizeof recsA);
        memset(visA, 0, sizeof visA);
        memset(visB, 0, sizeof visB);
        for (int k = 0x21ad; k <= 0x21b9; k += 4) *(int32_t*)(map + k) = (int32_t)(Rnd() % N);
        *(int32_t*)(map + 0x21bd) = (int32_t)(Rnd() % 3 == 0);
        uint8_t box[0x14];
        memcpy(box, map + 0x21ad, sizeof box);
        int32_t x = cx, y = cy, dir = (int32_t)(Rnd() % 8), flag = (int32_t)(Rnd() % 2);
        *(uint8_t**)(map + 0x165d) = recsA;
        gvis = visA;
        orig(map, 0, x, y, dir, flag);
        uint8_t boxA[0x14];
        memcpy(boxA, map + 0x21ad, sizeof boxA);
        memcpy(map + 0x21ad, box, sizeof box);
        *(uint8_t**)(map + 0x165d) = recsB;
        gvis = visB;
        Water_FloodFill(map, 0, x, y, dir, flag);
        big += memcmp(recsA, recsB, 0) == 0 && r > 4;
        EXPECT_EQ(FirstDiff(recsA, recsB, sizeof recsA), -1, "records iteration %d", it);
        EXPECT_EQ(memcmp(visA, visB, sizeof visA), 0, "visited iteration %d", it);
        EXPECT_EQ(memcmp(boxA, map + 0x21ad, sizeof boxA), 0, "box iteration %d", it);
    }
    gvis = sVis;
    EXPECT_EQ(big > 100, true, "coverage: %d larger lakes", big);
}

namespace rt3 {
int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner);
}

DIFFTEST(game_TrackBuildCost) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x48abf0;
    const int32_t N = 32, V = N + 1;
    static uint8_t map[0x2200], world[0x400], newGame[0x100], grid[N * N], piece[0x260];
    static float heights[V * V];
    static void* flagGrids[4];
    static int32_t cells[N * N];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15d5) = N;
    *(float**)(map + 0x1605) = heights;
    *(uint8_t**)(map + 0x2131) = grid;
    *(void***)(map + 0x1685) = flagGrids;
    static Collection companies, players, regions;
    memset(&companies, 0, sizeof companies);
    memset(&players, 0, sizeof players);
    memset(&regions, 0, sizeof regions);
    Collection_Construct(&companies, 0, 1, 0xe00, 4, 4, 0, 0, 0);
    Collection_Construct(&players, 0, 1, 0x400, 4, 4, 0, 0, 0);
    Collection_Construct(&regions, 0, 1, 0x500, 4, 4, 0, 0, 0);
    static uint8_t blank[0xe00];
    Collection_Allocate(&companies, 0, blank, 0xe00, 0, 0);
    Collection_Allocate(&players, 0, blank, 0x400, 0, 0);
    Collection_Allocate(&regions, 0, blank, 0x500, 0, 0);
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x62c120 }, { 0x6cec78 }, { 0x6cec7c }, { 0x62be10 }, { 0x6ceb9c }, { 0x6cfc9c }, { 0x62b300 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<Collection*>(0x62be10) = &companies;
    hook::Global<Collection*>(0x6ceb9c) = &players;
    hook::Global<Collection*>(0x6cfc9c) = &regions;
    int nonzero = 0;
    for (int it = 0; it < 6000; ++it) {
        for (float& h : heights) h = RndF(0.0f, 60.0f);
        for (uint8_t& g : grid) g = (uint8_t)(Rnd() % 3 == 0 ? 8 : 0);
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(world + 0x8a + k * 4) = (int32_t)(Rnd() % 200) - 100;
        *(int32_t*)(world + 0x2d) = (int32_t)(Rnd() % 4);
        newGame[0x7d] = (uint8_t)(Rnd() % 4);
        uint8_t* c = (uint8_t*)Collection_Get(&companies, 0, 1);
        *(uint32_t*)(c + 0x3b) = Rnd() % 2;
        for (int k = 0; k < 0x48; ++k) *(int32_t*)(c + 0x35b + k * 4) = (int32_t)(Rnd() % 100) - 50;
        *(float*)(c + 0xd31) = RndF(0.0f, 1.0f);
        uint8_t* p = (uint8_t*)Collection_Get(&players, 0, 1);
        for (int k = 0; k < 0x48; ++k) *(int32_t*)(p + 0x2ab + k * 4) = (int32_t)(Rnd() % 100) - 50;
        uint8_t* r = (uint8_t*)Collection_Get(&regions, 0, 1);
        for (int k = 0; k < 0x48; ++k) *(int32_t*)(r + 0x3b5 + k * 4) = (int32_t)(Rnd() % 100) - 50;
        for (int32_t& v : cells) v = Rnd() % 4 == 0;
        hook::Global<int32_t*>(0x62b300) = Rnd() % 2 ? cells : nullptr;
        memset(piece, 0, sizeof piece);
        for (int k = 0x14; k < 0x44; k += 4) *(float*)(piece + k) = (k % 0xc == 8) ? RndF(0.0f, 80.0f) : RndF(100.0f, (N - 2) * 92.0f);
        *(float*)(piece + 0xc) = RndF(100.0f, (N - 2) * 92.0f);
        *(float*)(piece + 0x10) = RndF(100.0f, (N - 2) * 92.0f);
        *(float*)(piece + 0x1ed) = RndF(0.0f, 120.0f);
        *(float*)(piece + 0x1f1) = RndF(0.0f, 120.0f);
        piece[0x217] = (uint8_t)(Rnd() % 6);
        piece[0x218] = (uint8_t)(Rnd() % 6);
        piece[0x24c] = (uint8_t)Rnd();
        piece[0x44] = (uint8_t)(Rnd() % 2);
        piece[0x219] = (uint8_t)(Rnd() % 3 == 0 ? 0xff : Rnd() % 8);
        piece[0x251] = (uint8_t)(Rnd() % 3 == 0 ? 0xff : Rnd() % 8);
        *(uint32_t*)(piece + 0x226) = Rnd() % 15;
        uint32_t company = Rnd() % 3;
        int32_t inner = (int32_t)(Rnd() % 4 == 0);
        uint8_t before[0x260];
        memcpy(before, piece, sizeof piece);
        int32_t ra = orig(piece, 0, company, inner);
        EXPECT_EQ(memcmp(before, piece, sizeof piece), 0, "piece restored iteration %d", it);
        int32_t rb = Track_BuildCost(piece, 0, company, inner);
        nonzero += ra != 0;
        EXPECT_EQ(ra, rb, "cost iteration %d (type %u kind %d/%d)", it, *(uint32_t*)(piece + 0x226), piece[0x217], piece[0x218]);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    Collection_Destruct(&companies);
    Collection_Destruct(&players);
    Collection_Destruct(&regions);
    EXPECT_EQ(nonzero > 2000, true, "coverage: %d nonzero costs", nonzero);
}

namespace rt3 {
void __fastcall TerrainLod_Mark(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEndArg,
                                int32_t vEndArg);
}

DIFFTEST(render_TerrainLodMark) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, uint8_t*, int32_t, int32_t, int32_t, int32_t))0x526670;
    static uint8_t self[0x3700], t[0x1700], gridA[0x4000], gridB[0x4000];
    static uint16_t heights[4][0x1000];
    const int DIST = 0x22000, THR = 0x400;
    static uint16_t savedDist[DIST];
    static int16_t savedThr[THR];
    memcpy(savedDist, (void*)0x6d9098, sizeof savedDist);
    memcpy(savedThr, (void*)0xc33d38, sizeof savedThr);
    float& scale = hook::Global<float>(0x6d9090);
    int32_t& offset = hook::Global<int32_t>(0xaf3528);
    float sScale = scale;
    int32_t sOffset = offset;
    int refined = 0;
    for (int it = 0; it < 1500; ++it) {
        uint16_t* dist = (uint16_t*)0x6d9098;
        for (int k = 0; k < DIST; ++k) dist[k] = (uint16_t)(Rnd() % 3000);
        int16_t* thr = (int16_t*)0xc33d38;
        for (int k = 0; k < THR; ++k) thr[k] = (int16_t)(Rnd() % 1500);
        scale = 0.1f;
        offset = 400;
        for (auto& lv : heights)
            for (uint16_t& h : lv) h = (uint16_t)(Rnd() % 4000);
        *(int32_t*)(self + 0x69) = 2;
        *(int32_t*)(self + 0x3682) = (int32_t)(Rnd() % 64);
        *(int32_t*)(self + 0x3686) = (int32_t)(Rnd() % 64);
        *(int32_t*)(t + 0x15d9) = 64;
        for (int l = 0; l < 4; ++l) *(uint16_t**)(t + 0x1619 + l * 4) = heights[l];
        int32_t level = 1 + (int32_t)(Rnd() % 3);
        int32_t u0 = (int32_t)(Rnd() % 4), v0 = (int32_t)(Rnd() % 4);
        int32_t uEnd = u0 + (int32_t)(Rnd() % 5), vEnd = v0 + (int32_t)(Rnd() % 5);
        memset(gridA, 0, sizeof gridA);
        memset(gridB, 0, sizeof gridB);
        *(uint8_t**)(self + 0x61) = gridA;
        orig(self, 0, level, t, u0, v0, uEnd, vEnd);
        uint32_t a15e1 = *(uint32_t*)(t + 0x15e1);
        *(uint32_t*)(t + 0x15e1) = 0;
        *(uint8_t**)(self + 0x61) = gridB;
        TerrainLod_Mark(self, 0, level, t, u0, v0, uEnd, vEnd);
        EXPECT_EQ(a15e1, *(uint32_t*)(t + 0x15e1), "level field iteration %d", it);
        int d = FirstDiff(gridA, gridB, sizeof gridA);
        EXPECT_EQ(d, -1, "grid iteration %d (level %d, %d..%d x %d..%d): +0x%x", it, level, u0, uEnd, v0, vEnd, d);
        int ones = 0;
        for (uint8_t g : gridA) ones += g == 1;
        refined += ones > 0;
    }
    memcpy((void*)0x6d9098, savedDist, sizeof savedDist);
    memcpy((void*)0xc33d38, savedThr, sizeof savedThr);
    scale = sScale;
    offset = sOffset;
    EXPECT_EQ(refined > 200, true, "coverage: %d runs refined to the finest grid", refined);
}

namespace rt3 {
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
}

namespace {
int g_widgetCalls[2];
void __fastcall FakeWidgetBlur(uint8_t*) { g_widgetCalls[0]++; }
void __fastcall FakeWidgetRedraw(uint8_t*) { g_widgetCalls[1]++; }
uint32_t g_filterSeed;
int32_t __fastcall FakeInputFilter(uint32_t* e) {
    g_filterSeed = g_filterSeed * 69069 + 1;
    return (int32_t)((e[1] ^ g_filterSeed) & 1);
}
} // namespace

DIFFTEST(game_WidgetFlagsAndInputFilter) {
    auto origClear = (void(__fastcall*)(uint8_t*, int, uint32_t))0x5588b0;
    auto origNext = (void*(__fastcall*)(uint8_t*, int, void*))0x54f4d0;
    FakeEngine fe;
    static void* vtbl[8];
    vtbl[0x10 / 4] = (void*)&FakeWidgetBlur;
    vtbl[0x1c / 4] = (void*)&FakeWidgetRedraw;
    static uint8_t wa[0x100], wb[0x100], handler[0x80], hover[0xd00];
    uint8_t*& ghover = hook::Global<uint8_t*>(0x6d401c);
    uint8_t* sHover = ghover;
    ghover = hover;
    void*& gfilter = hook::Global<void*>(0x6d4008);
    void* sFilter = gfilter;
    static uint8_t qa[0xb00], qb[0xb00];
    for (int it = 0; it < 4000; ++it) {
        memset(wa, 0, sizeof wa);
        *(void***)wa = vtbl;
        *(uint16_t*)(wa + 0x6a) = (uint16_t)(Rnd() & 0x1f);
        wa[0x68] = (uint8_t)(Rnd() % 4 ? 3 : Rnd());
        *(uint8_t**)(wa + 0x86) = handler;
        *(void**)(handler + 0x60) = Rnd() % 4 ? (void*)1 : nullptr;
        fe.e[0x57] = (uint8_t)(Rnd() % 6 == 0);
        memcpy(wb, wa, sizeof wa);
        uint32_t mask = Rnd() & 0x1f;
        *(int32_t*)(hover + 0xc71) = 7;
        g_widgetCalls[0] = g_widgetCalls[1] = 0;
        origClear(wa, 0, mask);
        int ca0 = g_widgetCalls[0], ca1 = g_widgetCalls[1];
        int32_t ha = *(int32_t*)(hover + 0xc71);
        *(int32_t*)(hover + 0xc71) = 7;
        g_widgetCalls[0] = g_widgetCalls[1] = 0;
        Widget_ClearFlags(wb, 0, mask);
        EXPECT_EQ(memcmp(wa, wb, sizeof wa), 0, "widget iteration %d", it);
        EXPECT_EQ(ca0 == g_widgetCalls[0] && ca1 == g_widgetCalls[1] && ha == *(int32_t*)(hover + 0xc71), true, "widget calls iteration %d", it);
        // Filtered input.
        for (int k = 0; k < 0xb00; k += 4) *(uint32_t*)(qa + k) = Rnd();
        *(int32_t*)(qa + 0xa88) = (int32_t)(Rnd() % 8);
        *(int32_t*)(qa + 0xa90) = (int32_t)(Rnd() % 3 == 0);
        *(int32_t*)(qa + 0x100) = (int32_t)(Rnd() % 2);
        for (int k = 0; k < 8; ++k) *(uint32_t*)(qa + 0x128 + k * 0x30) = Rnd() % 3 ? 2 : Rnd() % 4;
        memcpy(qb, qa, sizeof qa);
        gfilter = Rnd() % 3 ? (void*)&FakeInputFilter : nullptr;
        uint32_t seed = Rnd();
        uint32_t ea[12], eb[12];
        g_filterSeed = seed;
        origNext(qa, 0, ea);
        g_filterSeed = seed;
        Input_NextEvent(qb, 0, (InputEvent*)eb);
        EXPECT_EQ(memcmp(ea, eb, sizeof ea), 0, "event iteration %d", it);
        EXPECT_EQ(FirstDiff(qa, qb, sizeof qa), -1, "queue iteration %d", it);
    }
    ghover = sHover;
    gfilter = sFilter;
}

namespace rt3 {
void __fastcall Tracks_ApplyUpgrades(Collection* c, int, uint32_t company, int32_t amount);
}

DIFFTEST(game_TracksApplyUpgrades) {
    auto orig = (void(__fastcall*)(Collection*, int, uint32_t, int32_t))0x49ae00;
    const int32_t N = 32, V = N + 1;
    static uint8_t map[0x2200], world[0x5000], newGame[0x100], grid[N * N];
    static float heights[V * V];
    static void* flagGrids[4];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15d5) = N;
    *(float**)(map + 0x1605) = heights;
    *(uint8_t**)(map + 0x2131) = grid;
    *(void***)(map + 0x1685) = flagGrids;
    static Collection companies, players, regions;
    static uint32_t piecesMem[0x100]; // the track collection has fields past the Collection header (+0x9c)
    Collection& pieces = *(Collection*)piecesMem;
    Collection* cs[4] = { &companies, &players, &regions, &pieces };
    const uint32_t sizes[4] = { 0x2000, 0x400, 0x500, 0x260 };
    static uint8_t blank[0x2000];
    for (int k = 0; k < 4; ++k) {
        memset(cs[k], 0, k == 3 ? sizeof piecesMem : sizeof(Collection));
        Collection_Construct(cs[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
        for (int j = 0; j < (k == 3 ? 6 : 1); ++j) Collection_Allocate(cs[k], 0, blank, sizes[k], 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x62c120 }, { 0x6cec78 }, { 0x6cec7c }, { 0x62be10 }, { 0x6ceb9c }, { 0x6cfc9c }, { 0x62b300 }, { 0x6cd8d8 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<Collection*>(0x62be10) = &companies;
    hook::Global<Collection*>(0x6ceb9c) = &players;
    hook::Global<Collection*>(0x6cfc9c) = &regions;
    hook::Global<void*>(0x62b300) = nullptr;
    hook::Global<void*>(0x6cd8d8) = nullptr;
    static uint8_t snapP[6][0x260], outP[6][0x260], snapC[0x2000], outC[0x2000];
    for (int it = 0; it < 3000; ++it) {
        for (float& h : heights) h = RndF(0.0f, 60.0f);
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(world + 0x8a + k * 4) = (int32_t)(Rnd() % 200) - 100;
        *(int32_t*)(world + 0x2d) = (int32_t)(Rnd() % 4);
        *(int32_t*)(world + 0x4aef) = Rnd() % 8 == 0;
        newGame[0x7d] = (uint8_t)(Rnd() % 4);
        newGame[0x82] = (uint8_t)(Rnd() % 8 == 0);
        uint8_t* c = (uint8_t*)Collection_Get(&companies, 0, 1);
        memset(c, 0, 0x2000);
        *(float*)(c + 0xd31) = RndF(0.0f, 1.0f);
        for (int k = 1; k <= 6; ++k) {
            uint8_t* t = (uint8_t*)Collection_Get(&pieces, 0, k);
            memset(t, 0, 0x260);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(t + j) = (j % 0xc == 8) ? RndF(0.0f, 80.0f) : RndF(100.0f, (N - 2) * 92.0f);
            *(float*)(t + 0xc) = RndF(100.0f, (N - 2) * 92.0f);
            *(float*)(t + 0x10) = RndF(100.0f, (N - 2) * 92.0f);
            t[0x217] = (uint8_t)(Rnd() % 4);
            t[0x218] = Rnd() % 2 ? t[0x217] : (uint8_t)(Rnd() % 4);
            t[0x24c] = (uint8_t)Rnd();
            t[0x44] = (uint8_t)(Rnd() % 2);
            *(uint32_t*)(t + 0x226) = Rnd() % 4;
            *(int32_t*)(t + 0x230) = (int32_t)(Rnd() % 1000);
        }
        *(float*)((uint8_t*)&pieces + 0x9c) = RndF(0.0f, 1000.0f);
        uint32_t company = Rnd() % 3 ? 1 : 0;
        int32_t amount = (int32_t)(Rnd() % 20000) - 5000;
        for (int k = 0; k < 6; ++k) memcpy(snapP[k], Collection_Get(&pieces, 0, k + 1), 0x260);
        memcpy(snapC, c, 0x2000);
        float sumBefore = *(float*)((uint8_t*)&pieces + 0x9c);
        orig(&pieces, 0, company, amount);
        float sumA = *(float*)((uint8_t*)&pieces + 0x9c);
        for (int k = 0; k < 6; ++k) memcpy(outP[k], Collection_Get(&pieces, 0, k + 1), 0x260);
        memcpy(outC, c, 0x2000);
        for (int k = 0; k < 6; ++k) memcpy(Collection_Get(&pieces, 0, k + 1), snapP[k], 0x260);
        memcpy(c, snapC, 0x2000);
        *(float*)((uint8_t*)&pieces + 0x9c) = sumBefore;
        Tracks_ApplyUpgrades(&pieces, 0, company, amount);
        EXPECT_EQ(*(uint32_t*)&sumA, *(uint32_t*)((uint8_t*)&pieces + 0x9c), "total iteration %d", it);
        for (int k = 0; k < 6; ++k)
            EXPECT_EQ(memcmp(outP[k], Collection_Get(&pieces, 0, k + 1), 0x260), 0, "piece %d iteration %d", k + 1, it);
        EXPECT_EQ(FirstDiff(outC, c, 0x2000), -1, "company iteration %d", it);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection* col : cs) Collection_Destruct(col);
}

namespace rt3 {
int32_t __fastcall Grid_NearestNode(Collection* g, int, float px, float py, int32_t cx, int32_t cy, float* second,
                                    int32_t exclude, int32_t byDegree, float* best, int32_t skipLinked);
}

DIFFTEST(game_GridNearestNode) {
    auto orig = (int32_t(__fastcall*)(Collection*, int, float, float, int32_t, int32_t, float*, int32_t, int32_t, float*, int32_t))0x4921a0;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    static uint32_t gmem[0x100];
    Collection& g = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&g, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 14;
    for (int k = 0; k < N; ++k) Collection_Allocate(&g, 0, blank, 0x260, 0, 0);
    Collection_Release(&g, 0, 13);
    Collection_Release(&g, 0, 14);
    static uint8_t map[0x2200];
    *(int32_t*)(map + 0x15d9) = 16;
    *(int32_t*)(map + 0x15d5) = 16;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sMap = gmap;
    gmap = map;
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &g;
    static uint8_t* columns[16];
    static uint8_t entry[2][0x10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    int found = 0;
    for (int it = 0; it < 3000; ++it) {
        for (int k = 1; k <= 12; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&g, 0, k);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, 1500.0f);
            *(int32_t*)(n + 0x202) = k;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % 12);
        }
        int count = (int)(Rnd() % 8);
        uint32_t ids[8];
        // 13 / 14 are stale; only at the head, as a stale entry further down leaves a
        // freed node linked in the original (undefined behaviour).
        for (int k = 0; k < count; ++k) ids[k] = k == 0 ? 1 + Rnd() % 14 : 1 + Rnd() % 12;
        uint32_t* lists[2] = {};
        for (int si = 0; si < 2; ++si) {
            uint32_t* head = nullptr;
            for (int k = count - 1; k >= 0; --k) {
                uint32_t* n = (uint32_t*)xmalloc(8);
                n[0] = ids[k];
                n[1] = (uint32_t)head;
                head = n;
            }
            lists[si] = head;
            memset(entry[si], 0, 0x10);
            *(uint16_t*)entry[si] = 5;
            *(uint32_t**)(entry[si] + 2) = head;
        }
        float px = RndF(0.0f, 16.0f), py = RndF(0.0f, 16.0f);
        int32_t exclude = Rnd() % 3 ? -1 : 1 + (int32_t)(Rnd() % 12);
        int32_t byDegree = (int32_t)(Rnd() % 2), skip = (int32_t)(Rnd() % 2);
        int32_t cx = Rnd() % 6 == 0 ? 15 : 3, cy = Rnd() % 6 == 0 ? 4 : 5;
        float sa = RndF(0.0f, 20.0f), ba = RndF(0.0f, 20.0f), sb = sa, bb = ba;
        columns[3] = entry[0];
        columns[15] = entry[0];
        int32_t ra = orig(&g, 0, px, py, cx, cy, &sa, exclude, byDegree, &ba, skip);
        columns[3] = entry[1];
        columns[15] = entry[1];
        int32_t rb = Grid_NearestNode(&g, 0, px, py, cx, cy, &sb, exclude, byDegree, &bb, skip);
        found += ra != -1;
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        EXPECT_EQ(*(uint32_t*)&sa == *(uint32_t*)&sb && *(uint32_t*)&ba == *(uint32_t*)&bb, true, "distances iteration %d", it);
        uint32_t* na = *(uint32_t**)(entry[0] + 2);
        uint32_t* nb = *(uint32_t**)(entry[1] + 2);
        for (; na && nb; na = (uint32_t*)na[1], nb = (uint32_t*)nb[1]) EXPECT_EQ(na[0], nb[0], "list iteration %d", it);
        EXPECT_EQ(na == nullptr && nb == nullptr, true, "list length iteration %d", it);
    }
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    gmap = sMap;
    gnodes = sNodes;
    Collection_Destruct(&g);
    EXPECT_EQ(found > 500, true, "coverage: %d found", found);
}

namespace rt3 {
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);
void __fastcall Water_FloodFrom(uint8_t* m, int, int32_t x, int32_t y);
}

DIFFTEST(render_SceneObjSetAlpha) {
    auto orig = (void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))0x52fb70;
    FakeEngine fe;
    for (int it = 0; it < 3000; ++it) {
        SceneSide& a = g_scene[0];
        for (int k = 0; k < 8; ++k)
            for (int j = 0; j < 0x200; j += 4) *(uint32_t*)(a.objs[k] + j) = Rnd();
        int parent[8];
        bool hasKids[8] = {};
        for (int k = 1; k < 8; ++k) {
            parent[k] = (int)(Rnd() % k);
            hasKids[parent[k]] = true;
        }
        memcpy(&g_scene[1], &g_scene[0], sizeof a);
        for (int si = 0; si < 2; ++si) {
            SceneSide& s = g_scene[si];
            for (int k = 0; k < 8; ++k) {
                PtrList_Init(&s.lists[k]);
                *(PtrList**)(s.objs[k] + 0x75) = hasKids[k] || Rnd() % 4 == 0 ? &s.lists[k] : nullptr;
            }
            for (int k = 1; k < 8; ++k)
                if (*(PtrList**)(s.objs[parent[k]] + 0x75)) PtrList_Append(&s.lists[parent[k]], 0, s.objs[k]);
        }
        *(int32_t*)(fe.e + 0x114742) = (int32_t)(Rnd() % 2);
        uint32_t alpha = Rnd() % 3 == 0 ? 0xff : Rnd();
        int32_t deep = (int32_t)(Rnd() % 2);
        orig(g_scene[0].objs[0], 0, alpha, deep);
        SceneObj_SetAlpha(g_scene[1].objs[0], 0, alpha, deep);
        for (int k = 0; k < 8; ++k) {
            *(void**)(g_scene[0].objs[k] + 0x75) = nullptr;
            *(void**)(g_scene[1].objs[k] + 0x75) = nullptr;
            EXPECT_EQ(FirstDiff(g_scene[0].objs[k], g_scene[1].objs[k], 0x200), -1, "object %d iteration %d", k, it);
            PtrList_Clear(&g_scene[0].lists[k]);
            PtrList_Clear(&g_scene[1].lists[k]);
        }
    }
}

DIFFTEST(game_WaterFloodFrom) {
    auto orig = (void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x44c200;
    const int32_t N = 24;
    static uint8_t map[0x2200], recsA[N * N * 3], recsB[N * N * 3], options[0xb00];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15dd) = N;
    uint8_t*& gopt = hook::Global<uint8_t*>(0x6d4018);
    uint8_t* sOpt = gopt;
    gopt = options;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 0; k < N * N; ++k) {
            recsA[k * 3] = (uint8_t)(Rnd() % 8);
            recsA[k * 3 + 1] = (uint8_t)Rnd();
            recsA[k * 3 + 2] = (uint8_t)(Rnd() % 3 == 0 ? 0x20 | (Rnd() & 0xdf) : Rnd() & 0xdf);
        }
        int cx = (int)(Rnd() % N), cy = (int)(Rnd() % N), r = 1 + (int)(Rnd() % 8);
        for (int y = 0; y < N; ++y)
            for (int x = 0; x < N; ++x)
                if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r)
                    recsA[(y * N + x) * 3] = (uint8_t)((recsA[(y * N + x) * 3] & 0xf8) | (Rnd() % 4 ? 2 : 4));
        memcpy(recsB, recsA, sizeof recsA);
        *(int32_t*)(options + 0xa8c) = (int32_t)Rnd();
        int32_t x = Rnd() % 4 ? cx : (int32_t)(Rnd() % N), y = Rnd() % 4 ? cy : (int32_t)(Rnd() % N);
        *(uint8_t**)(map + 0x165d) = recsA;
        orig(map, 0, x, y);
        uint8_t boxA[0x14];
        memcpy(boxA, map + 0x21ad, sizeof boxA);
        *(uint8_t**)(map + 0x165d) = recsB;
        Water_FloodFrom(map, 0, x, y);
        EXPECT_EQ(FirstDiff(recsA, recsB, sizeof recsA), -1, "records iteration %d", it);
        EXPECT_EQ(memcmp(boxA, map + 0x21ad, sizeof boxA), 0, "box iteration %d", it);
        EXPECT_EQ(hook::Global<void*>(0x62c128) == nullptr, true, "visited freed iteration %d", it);
    }
    gopt = sOpt;
}

namespace rt3 {
int32_t __fastcall Route_Find(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                              int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                   int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
}

namespace {
using RouteFindFn = int32_t(__fastcall*)(Collection*, int, int32_t, int32_t, uint32_t*, int32_t, int32_t, uint32_t*, uint32_t*, int32_t);
// Shared by both route searches: the plain one can return the address of the
// cost variable, so both calls use the same one.
void RouteFindCase(RouteFindFn orig, RouteFindFn mine) {
    const int R = 12, N = R * 2;
    static Collection nodes, routes;
    memset(&nodes, 0, sizeof nodes);
    memset(&routes, 0, sizeof routes);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    Collection_Construct(&routes, 0, 1, 0x40, 0x10, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    for (int k = 0; k < N; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    for (int k = 0; k < R; ++k) Collection_Allocate(&routes, 0, blank, 0x40, 0, 0);
    static uint8_t companyMem[0x1d9f + 4 * 0x38d];
    uint32_t* company = (uint32_t*)companyMem;
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6cfca8 }, { 0x6cfcac }, { 0x6cfcb0 }, { 0x6cfcb4 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x6cfcb4) = &routes;
    int found = 0;
    for (int it = 0; it < 6000; ++it) {
        // Routes r join nodes 2r-1 and 2r; junctions pair nodes up (a matching, so
        // link-only moves can't cycle).
        for (int r = 1; r <= R; ++r) {
            uint8_t* rt = (uint8_t*)Collection_Get(&routes, 0, r);
            *(int32_t*)(rt + 4) = 2 * r - 1;
            *(int32_t*)(rt + 8) = 2 * r;
            *(uint32_t*)(rt + 0x14) = 1 + Rnd() % 20;
            *(int32_t*)(rt + 0x18) = (int32_t)(Rnd() % 4);
            rt[0x1c] = (uint8_t)(Rnd() % 5 != 0);
            *(uint32_t*)(rt + 0x1e) = Rnd() % 3;
        }
        int perm[N];
        for (int k = 0; k < N; ++k) perm[k] = k + 1;
        for (int k = N - 1; k > 0; --k) {
            int j = (int)(Rnd() % (k + 1)), t = perm[k];
            perm[k] = perm[j];
            perm[j] = t;
        }
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            *(int32_t*)(n + 0x202) = k;
            *(int32_t*)(n + 0x212) = (k + 1) / 2;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = -1;
            n[0x216] = (uint8_t)(Rnd() % 8 ? 2 : 1);
            *(uint16_t*)(n + 0x22e) = (uint16_t)(Rnd() % 3);
            for (int o = 0x14; o <= 0x34; o += (o == 0x1c ? 0x10 : 8)) *(float*)(n + o) = (float)(Rnd() % 40) * 50.0f;
        }
        for (int k = 0; k + 1 < N; k += 2) {
            if (Rnd() % 5 == 0) continue;
            uint8_t* a = (uint8_t*)Collection_Get(&nodes, 0, perm[k]);
            uint8_t* b = (uint8_t*)Collection_Get(&nodes, 0, perm[k + 1]);
            *(int32_t*)(a + 0x206 + (Rnd() % 3) * 4) = perm[k + 1];
            *(int32_t*)(b + 0x206 + (Rnd() % 3) * 4) = perm[k];
        }
        memset(companyMem, 0, sizeof companyMem);
        *company = Rnd() % 3;
        for (int k = 0; k < 4; ++k) companyMem[0x1d9f + k * 0x38d] = (uint8_t)(Rnd() % 4 != 0);
        int32_t cur = 1 + (int32_t)(Rnd() % N), target = 1 + (int32_t)(Rnd() % N);
        int32_t needFlag = Rnd() % 3 == 0, anyTrack = Rnd() % 3 == 0;
        uint32_t* co = Rnd() % 2 ? company : nullptr;
        bool useForeign = Rnd() % 4 != 0;
        uint32_t cost = Rnd(), forA = Rnd(), forB = forA;
        int32_t ra = orig(&routes, 0, cur, target, &cost, -1, needFlag, co, useForeign ? &forA : nullptr, anyTrack);
        uint32_t boundA = hook::Global<uint32_t>(0x6cfcb0), costA = cost;
        int32_t rb = mine(&routes, 0, cur, target, &cost, -1, needFlag, co, useForeign ? &forB : nullptr, anyTrack);
        uint32_t costB = cost;
        found += costA != 0xffffffff && cur != target;
        EXPECT_EQ(ra, rb, "next node iteration %d", it);
        EXPECT_EQ(costA, costB, "cost iteration %d", it);
        EXPECT_EQ(forA, forB, "foreign iteration %d", it);
        EXPECT_EQ(boundA, hook::Global<uint32_t>(0x6cfcb0), "bound iteration %d", it);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    Collection_Destruct(&nodes);
    Collection_Destruct(&routes);
    EXPECT_EQ(found > 1000, true, "coverage: %d routes found", found);
}
} // namespace

DIFFTEST(game_RouteFind) { RouteFindCase((RouteFindFn)0x4a5280, &Route_Find); }
DIFFTEST(game_RouteFindPlain) { RouteFindCase((RouteFindFn)0x4a5900, &Route_FindPlain); }
namespace rt3 {
int32_t __fastcall Route_FindForVersion(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                        int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
}
DIFFTEST(game_RouteFindForVersion) { RouteFindCase((RouteFindFn)0x4a65b0, &Route_FindForVersion); }

namespace rt3 {
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
}

DIFFTEST(game_NumFormat) {
    auto orig = (char*(__fastcall*)(char*, int32_t, double, int32_t, int32_t, int32_t, int32_t))0x51b700;
    FakeEngine fe;
    char& comma = hook::Global<char>(0x62432c);
    char& point = hook::Global<char>(0x62432d);
    char sComma = comma, sPoint = point;
    static const char seps[] = { ',', '.', ' ', '\'' };
    static const double scales[] = { 1.0, 10.0, 1000.0, 1e5, 1e6, 1e8, 1e9, 1e11, 1e12, 1e14 };
    int32_t& depth = hook::Global<int32_t>(0x6d3c98);
    for (int it = 0; it < 40000; ++it) {
        *(int32_t*)(fe.e + 0x114732) = Rnd() % 4 == 0;
        comma = seps[Rnd() % 4];
        point = seps[Rnd() % 4];
        int32_t style = (int32_t)(Rnd() % 15);
        double v;
        switch (Rnd() % 8) {
        case 0: v = (double)((int32_t)(Rnd() % 2001) - 1000); break;
        case 1: v = (double)RndF(-2.0f, 2.0f); break;
        case 2: { uint64_t b = ((uint64_t)Rnd() << 32) | Rnd(); memcpy(&v, &b, 8); if (v != v || fabs(v) > 1e18) v = 0.5; break; }
        default: v = (double)RndF(-1.0f, 1.0f) * scales[Rnd() % 10];
        }
        if (Rnd() % 500 == 0) { uint64_t nan = 0x7ff8000000000000ull; memcpy(&v, &nan, 8); }
        int32_t width = (int32_t)(Rnd() % 16), unused = (int32_t)(Rnd() % 2), inK = Rnd() % 4 == 0, inM = Rnd() % 4 == 0;
        int32_t d0 = depth;
        char a[0x100], b[0x100];
        memset(a, 0x5a, sizeof a);
        memset(b, 0x5a, sizeof b);
        // Unknown styles leave the buffer as it is and the separator pass then runs to
        // its NUL: without one it rewrites ',' bytes past the end (once the stack cookie).
        a[sizeof a - 1] = b[sizeof b - 1] = 0;
        char* ra = orig(a, style, v, width, unused, inK, inM);
        char* rb = Num_Format(b, style, v, width, unused, inK, inM);
        EXPECT_EQ(ra == a && rb == b && depth == d0, true, "return iteration %d", it);
        int diff = FirstDiff((uint8_t*)a, (uint8_t*)b, sizeof a);
        EXPECT_EQ(diff, -1, "style %d v %.17g width %d k%d m%d iteration %d: '%.40s' vs '%.40s'", style, v, width, inK, inM, it, a, b);
    }
    comma = sComma;
    point = sPoint;
}

namespace rt3 {
void __fastcall Track_SetHighlight(uint8_t* t, int, int32_t on);
void __fastcall Track_Reverse(uint8_t* t);
void __fastcall Track_MoveEnds(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                               int32_t unused, int32_t force);
void __fastcall Track_ReshapeCells(uint8_t* t, int, float x0, float z0, float x1, float z1, float x2, float z2, float x3,
                                   float z3, int32_t drop, int32_t neighbours);
void __fastcall Tracks_RestoreEdit(Collection* c, int, uint8_t* t);
void __fastcall Tracks_ClearHighlights(Collection* c);
void __fastcall Tracks_ReplayEdit(Collection* c);
void __fastcall Tracks_HighlightRun(Collection* c, int, uint32_t id, int32_t on, int32_t follow);
void Tracks_UndoPendingEdit();
void __fastcall Track_InitRotated(uint8_t* t, int, int32_t id, int32_t group, float cx, float cy, float angle, int32_t kind,
                                  int32_t owner, int32_t style);
void __fastcall Track_PlaceRotated(uint8_t* t, int, float cx, float cy, float angle);
void __fastcall Track_Init(uint8_t* t, int, int32_t id, int32_t group, float x0, float z0, float x1, float z1, float x2, float z2,
                           float x3, float z3, int32_t kind, int32_t owner, int32_t style);
int32_t __fastcall Tracks_SettleGroup(Collection* c, int, uint32_t id);
void __fastcall Track_LevelEnds(Collection* c, int, uint8_t* t);
void __fastcall Tracks_MoveEnds(Collection* c, int, uint32_t id, const uint32_t* a, const uint32_t* b, const uint32_t* d,
                                const uint32_t* e);
}

namespace {
int32_t g_slot14Result;
int32_t __fastcall FakeSlot14(uint8_t*) { return g_slot14Result; }
} // namespace
namespace {
// One self-contained track world: a terrain, its cell list, the piece collection
// (with the undo record fields past the header) and scenery records.
struct TrackWorld {
    uint8_t terrain[0x1700];
    PtrList cell;
    PtrList* cells[1];
    uint32_t piecesMem[0x200];
    uint8_t scen[6][0x50];
    Collection& pieces() { return *(Collection*)piecesMem; }
};
TrackWorld g_tw[2];
} // namespace

DIFFTEST(game_TrackEditWrappers) {
    const int32_t N = 16, V = N + 1, P = 6;
    static float heights[V * V];
    FakeEngine fe;
    static uint8_t blank[0x260];
    for (TrackWorld& w : g_tw) {
        memset(&w, 0, sizeof w);
        *(int32_t*)(w.terrain + 0x15d9) = N;
        *(int32_t*)(w.terrain + 0x15d5) = N;
        *(float**)(w.terrain + 0x1605) = heights;
        w.cells[0] = &w.cell;
        *(PtrList***)(w.terrain + 0x1675) = w.cells;
        PtrList_Init(&w.cell);
        Collection_Construct(&w.pieces(), 0, 1, 0x260, 8, 8, 0, 0, 0);
        for (int k = 0; k < P; ++k) Collection_Allocate(&w.pieces(), 0, blank, 0x260, 0, 0);
    }
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    static uint8_t opts[0x400], world[0x5000];
    uint8_t*& gopts = hook::Global<uint8_t*>(0x6cec74);
    uint8_t*& gworld = hook::Global<uint8_t*>(0x6cec78);
    uint8_t *sOpts = gopts, *sWorld = gworld;
    gopts = opts;
    gworld = world;
    static uint8_t tmap[0x2200], coarse[0x400];
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sMap = gmap;
    gmap = tmap;
    *(int32_t*)(tmap + 0x15d9) = N;
    *(int32_t*)(tmap + 0x15e1) = 2;
    *(uint8_t**)(tmap + 0x162d) = coarse;
    static Collection noTrains;
    memset(&noTrains, 0, sizeof noTrains);
    Collection_Construct(&noTrains, 0, 1, 0x100, 8, 8, 0, 0, 0);
    Collection*& gtrains = hook::Global<Collection*>(0x6cfcbc);
    Collection* sTrains = gtrains;
    gtrains = &noTrains;
    static void* pieceVtbl[8];
    pieceVtbl[0x14 / 4] = (void*)&FakeSlot14;
    static uint8_t init[P][0x260], scenInit[6][0x50], extInit[0x800];
    static uint8_t outA[P][0x260], scenA[6][0x50], extA[0x800];
    int changed = 0;
    for (int it = 0; it < 4000; ++it) {
        for (float& h : heights) h = RndF(-50.0f, 400.0f);
        *(int32_t*)(fe.e + 0x114256) = Rnd() % 4 == 0;
        *(int32_t*)(opts + 0x2bf) = (int32_t)(Rnd() % 3 == 0 ? 2 : 0);
        *(int32_t*)(opts + 0x98) = (int32_t)(Rnd() % 2);
        *(int32_t*)(world + 0x4ae3) = (int32_t)(Rnd() % 2);
        // Pieces in a chain, filled like the Track_SetEnds test.
        for (int k = 0; k < P; ++k) {
            uint8_t* t = init[k];
            for (int j = 0; j < 0x260; j += 4) *(float*)(t + j) = RndF(-100.0f, 100.0f);
            for (int j = 0xc; j <= 0x40; j += 4) *(float*)(t + j) = RndF(0.0f, (N - 0.5f) * 92.0f);
            for (int j = 0x45; j <= 0x51; j += 4) *(float*)(t + j) = RndF(0.0f, 30.0f);
            for (int j = 0x1f5; j <= 0x1fd; j += 4) *(float*)(t + j) = RndF(0.5f, 20.0f);
            t[0x44] = (uint8_t)(Rnd() % 4 == 0 ? 2 : 0);
            *(void**)(t + 0x1e3) = nullptr;
            *(void**)(t + 0x55) = nullptr;
            *(void**)(t + 8) = nullptr;
            *(void**)(t + 0x248) = nullptr;
            *(int32_t*)(t + 0x202) = k + 1;
            *(int32_t*)(t + 0x206) = *(int32_t*)(t + 0x20a) = *(int32_t*)(t + 0x20e) = -1;
            *(int32_t*)(t + 0x212) = -1;
            t[0x216] = 0;
            static const uint32_t kinds[5] = { 0, 1, 2, 12, 13 };
            *(uint32_t*)(t + 0x226) = kinds[Rnd() % 5];
            *(int32_t*)(t + 0x24d) = Rnd() % 8 == 0 ? -1 : (int32_t)(Rnd() % 3);
            t[0x201] = (uint8_t)Rnd();
            *(void***)t = pieceVtbl;
        }
        for (int k = 0; k + 1 < P; ++k) {
            if (Rnd() % 5 == 0) continue;
            uint32_t sa = *(int32_t*)(init[k] + 0x206) == -1 && Rnd() % 2 ? 0x206 : 0x20a;
            uint32_t sb = *(int32_t*)(init[k + 1] + 0x206) == -1 ? 0x206 : 0x20a;
            if (*(int32_t*)(init[k] + sa) != -1) continue;
            *(int32_t*)(init[k] + sa) = k + 2;
            *(int32_t*)(init[k + 1] + sb) = k + 1;
        }
        for (int k = 0; k + 2 < P; ++k)
            if (Rnd() % 4 == 0) *(int32_t*)(init[k] + 0x20e) = k + 3;
        for (auto& s : scenInit)
            for (int j = 0; j < 0x50; j += 4) *(float*)(s + j) = RndF(-100.0f, 100.0f);
        int scenCount = (int)(Rnd() % 4);
        // The collection's undo record.
        for (int j = 0x100; j < 0x800; j += 4) *(float*)(extInit + j) = RndF(0.0f, (N - 0.5f) * 92.0f);
        const uint32_t cornerBases[3] = { 0x139, 0x169, 0x199 }; // unaligned: set each float explicitly
        for (uint32_t base : cornerBases)
            for (int j = 0; j < 12; ++j) *(float*)(extInit + base + j * 4) = RndF(0.0f, (N - 0.5f) * 92.0f);
        *(int32_t*)(extInit + 0xe9) = 0;
        *(int32_t*)(extInit + 0x118) = Rnd() % 4 != 0;
        *(uint32_t*)(extInit + 0x125) = Rnd() % 4 ? 1 + Rnd() % P : 0x40;
        *(uint32_t*)(extInit + 0x129) = Rnd() % 3 ? 1 + Rnd() % P : 0x40;
        *(int32_t*)(extInit + 0x12d) = Rnd() % 6 == 0 ? -1 : Rnd() % 3 ? 1 + (int32_t)(Rnd() % P) : 0x40;
        *(uint32_t*)(extInit + 0x131) = Rnd() % 2 ? 1 + Rnd() % P : 0x40;
        if (Rnd() % 6 == 0) *(float*)(extInit + 0x169) = -1.0f;
        // Operation arguments.
        int op = (int)(Rnd() % 15);
        *(int32_t*)(extInit + 0xed) = Rnd() % 3 == 0;
        g_slot14Result = Rnd() % 2;
        float angle = RndF(-4.0f, 4.0f);
        int32_t newKind = (int32_t)(Rnd() % 3), ownerId = (int32_t)(Rnd() % 4), style = (int32_t)(Rnd() % 4);
        int32_t target = 1 + (int32_t)(Rnd() % P);
        int32_t flagA = (int32_t)(Rnd() % 2), flagB = (int32_t)(Rnd() % 2);
        if (op == 3) flagA = 0; // with `drop` the original reads stale stack heights
        uint32_t vec[4][3];
        for (auto& v : vec)
            for (uint32_t& c : v) {
                float f = RndF(0.0f, (N - 0.5f) * 92.0f);
                c = *(uint32_t*)&f;
            }
        float cellsArg[8];
        for (float& c : cellsArg) c = RndF(0.2f, N - 1.5f);
        for (int side = 0; side < 2; ++side) {
            TrackWorld& w = g_tw[side];
            memcpy((uint8_t*)w.piecesMem + sizeof(Collection), extInit + sizeof(Collection), sizeof w.piecesMem - sizeof(Collection));
            for (int k = 0; k < P; ++k) {
                uint8_t* t = (uint8_t*)Collection_Get(&w.pieces(), 0, k + 1);
                memcpy(t, init[k], 0x260);
                *(uint8_t**)(t + 4) = w.terrain;
                *(uint8_t**)(t + 0x244) = nullptr;
            }
            memcpy(w.scen, scenInit, sizeof scenInit);
            uint8_t* owner = (uint8_t*)Collection_Get(&w.pieces(), 0, target);
            uint8_t* prev = nullptr;
            for (int s = 0; s < scenCount; ++s) {
                *(uint8_t**)(w.scen[s] + 0xe) = nullptr;
                if (prev) *(uint8_t**)(prev + 0xe) = w.scen[s];
                else *(uint8_t**)(owner + 0x244) = w.scen[s];
                prev = w.scen[s];
            }
            gnodes = &w.pieces();
            *(uint8_t**)(fe.e + 0x1c) = w.terrain;
            memset(coarse, 0, sizeof coarse);
            uint8_t* t = owner;
            int sop = op;
            if (side == 0) {
                switch (sop) {
                case 0: ((void(__fastcall*)(uint8_t*, int, int32_t))0x48e2c0)(t, 0, flagA); break;
                case 1: ((void(__fastcall*)(uint8_t*))0x48eb80)(t); break;
                case 2:
                    ((void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, int32_t, int32_t))0x48f740)(
                        t, 0, vec[0], vec[1], vec[2], vec[3], 0, flagA);
                    break;
                case 3:
                    ((void(__fastcall*)(uint8_t*, int, float, float, float, float, float, float, float, float, int32_t, int32_t))0x48f7f0)(
                        t, 0, cellsArg[0], cellsArg[1], cellsArg[2], cellsArg[3], cellsArg[4], cellsArg[5], cellsArg[6], cellsArg[7], flagA, flagB);
                    break;
                case 4: ((void(__fastcall*)(Collection*, int, uint8_t*))0x49afe0)(&w.pieces(), 0, t); break;
                case 5:
                    ((void(__fastcall*)(Collection*, int, uint32_t, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*))0x491e30)(
                        &w.pieces(), 0, flagB || target != 1 ? (uint32_t)target : 0x30u, vec[0], vec[1], vec[2], vec[3]);
                    break;
                case 6: ((void(__fastcall*)(Collection*))0x4937b0)(&w.pieces()); break;
                case 7: ((void(__fastcall*)(Collection*))0x49b070)(&w.pieces()); break;
                case 8: ((void(__fastcall*)(Collection*, int, uint32_t, int32_t, int32_t))0x4937f0)(&w.pieces(), 0, (uint32_t)target, flagA | flagB, flagB); break;
                case 9: ((void(*)())0x50dab0)(); break;
                case 10: ((int32_t(__fastcall*)(Collection*, int, uint32_t))0x497f80)(&w.pieces(), 0, (uint32_t)target); break;
                case 11: ((void(__fastcall*)(Collection*, int, uint8_t*))0x4a2aa0)(&w.pieces(), 0, t); break;
                case 12: ((void(__fastcall*)(uint8_t*, int, float, float, float))0x490f80)(t, 0, cellsArg[0], cellsArg[1], angle); break;
                case 14:
                    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, float, float, float, int32_t, int32_t, int32_t))0x490ac0)(
                        t, 0, target, flagA, cellsArg[0], cellsArg[1], angle, newKind, ownerId, style);
                    break;
                case 13:
                    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, float, float, float, float, float, float, float, float, int32_t, int32_t,
                                        int32_t))0x48f2b0)(t, 0, target, flagA, cellsArg[0], cellsArg[1], cellsArg[2], cellsArg[3], cellsArg[4],
                                                           cellsArg[5], cellsArg[6], cellsArg[7], newKind, ownerId, style);
                    break;
                }
            } else {
                switch (sop) {
                case 0: Track_SetHighlight(t, 0, flagA); break;
                case 1: Track_Reverse(t); break;
                case 2: Track_MoveEnds(t, 0, vec[0], vec[1], vec[2], vec[3], 0, flagA); break;
                case 3:
                    Track_ReshapeCells(t, 0, cellsArg[0], cellsArg[1], cellsArg[2], cellsArg[3], cellsArg[4], cellsArg[5], cellsArg[6],
                                       cellsArg[7], flagA, flagB);
                    break;
                case 4: Tracks_RestoreEdit(&w.pieces(), 0, t); break;
                case 5: Tracks_MoveEnds(&w.pieces(), 0, flagB || target != 1 ? (uint32_t)target : 0x30u, vec[0], vec[1], vec[2], vec[3]); break;
                case 6: Tracks_ClearHighlights(&w.pieces()); break;
                case 7: Tracks_ReplayEdit(&w.pieces()); break;
                case 8: Tracks_HighlightRun(&w.pieces(), 0, (uint32_t)target, flagA | flagB, flagB); break;
                case 9: Tracks_UndoPendingEdit(); break;
                case 10: Tracks_SettleGroup(&w.pieces(), 0, (uint32_t)target); break;
                case 11: Track_LevelEnds(&w.pieces(), 0, t); break;
                case 12: Track_PlaceRotated(t, 0, cellsArg[0], cellsArg[1], angle); break;
                case 14: Track_InitRotated(t, 0, target, flagA, cellsArg[0], cellsArg[1], angle, newKind, ownerId, style); break;
                case 13:
                    Track_Init(t, 0, target, flagA, cellsArg[0], cellsArg[1], cellsArg[2], cellsArg[3], cellsArg[4], cellsArg[5], cellsArg[6],
                               cellsArg[7], newKind, ownerId, style);
                    break;
                }
            }
            X87Clear();
            // Normalise this world's own pointers before comparing.
            for (int k = 0; k < P; ++k) {
                uint8_t* p = (uint8_t*)Collection_Get(&w.pieces(), 0, k + 1);
                uint8_t norm[0x260];
                memcpy(norm, p, 0x260);
                *(void**)(norm + 4) = nullptr;
                *(uintptr_t*)(norm + 0x244) = *(uint8_t**)(norm + 0x244) ? 1 : 0;
                *(uintptr_t*)(norm + 0x55) = *(uint8_t**)(norm + 0x55) ? 1 : 0;
                if (op == 13 || op == 14) memset(norm + 0x12f, 0, 0x1e3 - 0x12f); // mesh fields from stale stack heights
                // The quad buffer (+0x1e3) is allocated by Track_SetEnds: compare its contents.
                static uint8_t quadsA[P][0xc0];
                uint8_t quads[0xc0] = {};
                if (void* q = *(void**)(p + 0x1e3)) {
                    memcpy(quads, q, 0xc0);
                    ((void(__cdecl*)(void*))0x5a1145)(q);
                    *(void**)(p + 0x1e3) = nullptr;
                    *(uintptr_t*)(norm + 0x1e3) = 1;
                }
                if (side == 0) memcpy(quadsA[k], quads, 0xc0);
                else EXPECT_EQ(FirstDiff(quadsA[k], quads, 0xc0), -1, "op %d quads %d iteration %d", op, k + 1, it);
                if (side == 0) memcpy(outA[k], norm, 0x260);
                else {
                    EXPECT_EQ(FirstDiff(outA[k], norm, 0x260), -1, "op %d piece %d (target %d) iteration %d at 0x%x: %08x vs %08x", op, k + 1, target, it, FirstDiff(outA[k], norm, 0x260), *(uint32_t*)(outA[k] + (FirstDiff(outA[k], norm, 0x260) & ~3)), *(uint32_t*)(norm + (FirstDiff(outA[k], norm, 0x260) & ~3)));
                    changed += memcmp(outA[k], init[k], 0x200) != 0;
                }
            }
            for (int s = 0; s < 6; ++s) {
                uint8_t norm[0x50];
                memcpy(norm, w.scen[s], 0x50);
                *(void**)(norm + 0xe) = nullptr;
                if (side == 0) memcpy(scenA[s], norm, 0x50);
                else EXPECT_EQ(memcmp(scenA[s], norm, 0x50), 0, "op %d scenery %d iteration %d", op, s, it);
            }
            if (side == 0) memcpy(extA, w.piecesMem, sizeof extA);
            else
                EXPECT_EQ(memcmp(extA + sizeof(Collection), (uint8_t*)w.piecesMem + sizeof(Collection), sizeof extA - sizeof(Collection)), 0,
                          "op %d collection iteration %d", op, it);
            static uint8_t coarseA[0x400];
            if (side == 0) memcpy(coarseA, coarse, sizeof coarse);
            else EXPECT_EQ(memcmp(coarseA, coarse, sizeof coarse), 0, "op %d coarse map iteration %d", op, it);
            PtrList_Clear(&w.cell);
        }
    }
    gnodes = sNodes;
    gopts = sOpts;
    gworld = sWorld;
    gmap = sMap;
    gtrains = sTrains;
    Collection_Destruct(&noTrains);
    for (TrackWorld& w : g_tw) {
        Collection_Destruct(&w.pieces());
        PtrList_Clear(&w.cell);
    }
    EXPECT_EQ(changed > 3000, true, "coverage: %d pieces changed", changed);
}

namespace rt3 {
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
const char* __stdcall Save_ProgressText(int32_t total, float done);
void __fastcall Train_ShiftGear(uint8_t* o, int, int32_t delta);
}

DIFFTEST(game_NumTextWrappers) {
    static Collection logs[2];
    for (Collection& l : logs) {
        memset(&l, 0, sizeof l);
        Collection_Construct(&l, 0, 1, 0x21c, 0x40, 0x10, 0, 0, 0);
    }
    Collection*& glog = hook::Global<Collection*>(0x6acd34);
    Collection* sLog = glog;
    static uint8_t trainA[0x500], trainB[0x500];
    for (int it = 0; it < 3000; ++it) {
        char a[0x100], b[0x100];
        double v = (double)RndF(-1.0f, 1.0f) * (Rnd() % 2 ? 1e3 : 1e7);
        int32_t style = (int32_t)(Rnd() % 14), width = (int32_t)(Rnd() % 14);
        const uint32_t addrs[2] = { 0x51bfe0u, 0x51c000u };
        for (uint32_t addr : addrs) {
            memset(a, 0x5a, sizeof a);
            memset(b, 0x5a, sizeof b);
            ((char*(__fastcall*)(char*, int32_t, double, int32_t))addr)(a, style, v, width);
            Num_FormatSimple(b, style, v, width);
            EXPECT_EQ(memcmp(a, b, sizeof a), 0, "simple %x iteration %d", addr, it);
        }
        int32_t total = (int32_t)(Rnd() % 5000) - 10;
        float done = RndF(-50.0f, 6000.0f);
        strcpy(a, ((const char*(__stdcall*)(int32_t, float))0x441d90)(total, done));
        const char* pb = Save_ProgressText(total, done);
        EXPECT_EQ(pb == (const char*)0x62c0d8 && strcmp(a, pb) == 0, true, "progress %d %g iteration %d: '%s' vs '%s'", total, done, it, a, pb);
        // Gear shifts into two separate logs.
        int32_t gear = (int32_t)(Rnd() % 8) - 2, delta = (int32_t)(Rnd() % 5) - 2;
        *(int32_t*)(trainA + 0x430) = *(int32_t*)(trainB + 0x430) = gear;
        int32_t serial = hook::Global<int32_t>(0x6acd30);
        glog = &logs[0];
        ((void(__fastcall*)(uint8_t*, int, int32_t))0x4856b0)(trainA, 0, delta);
        hook::Global<int32_t>(0x6acd30) = serial;
        glog = &logs[1];
        Train_ShiftGear(trainB, 0, delta);
        EXPECT_EQ(*(int32_t*)(trainA + 0x430), *(int32_t*)(trainB + 0x430), "gear iteration %d", it);
        EXPECT_EQ(Collection_Count(&logs[0]), Collection_Count(&logs[1]), "log count iteration %d", it);
        uint32_t la = Collection_IdAt(&logs[0], 0, Collection_Count(&logs[0]) - 1, 0);
        uint32_t lb = Collection_IdAt(&logs[1], 0, Collection_Count(&logs[1]) - 1, 0);
        EXPECT_EQ(la, lb, "log id iteration %d", it);
        {
            // Only the fields Entry21a_Init writes are defined (records are allocated without data).
            uint8_t* ea = (uint8_t*)Collection_Get(&logs[0], 0, la);
            uint8_t* eb = (uint8_t*)Collection_Get(&logs[1], 0, lb);
            bool same = !memcmp(ea, eb, 0xd) && !strcmp((char*)ea + 0xd, (char*)eb + 0xd) && !memcmp(ea + 0x20e, eb + 0x20e, 0xc);
            EXPECT_EQ(same, true, "log entry iteration %d: '%s' vs '%s'", it, (char*)ea + 0xd, (char*)eb + 0xd);
        }
    }
    glog = sLog;
    for (Collection& l : logs) Collection_Destruct(&l);
}

namespace rt3 {
void __fastcall Widget_SetValue(uint8_t* w, int, int32_t v);
int32_t __fastcall Button_OnRelease(uint8_t* w, int, uint32_t* out);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
int32_t __fastcall Button_OnPress(uint8_t* w, int, uint32_t* out);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
void __fastcall List_SyncScrollbar(uint8_t* l);
void __fastcall ListView_Refresh(uint8_t* l, int, int32_t toEnd);
void __fastcall Tracks_ClearHighlights(Collection* c);
void __fastcall Tracks_MoveEnds(Collection* c, int, uint32_t id, const uint32_t* a, const uint32_t* b, const uint32_t* d,
                                const uint32_t* e);
}

namespace {
// Widgets whose virtual calls are logged as (widget index, slot, args).
uint32_t g_wlog[64];
int g_wn;
uint8_t g_widgets[2][3][0x200];
void WLog(uint8_t* w, uint32_t slot) {
    for (int s = 0; s < 2; ++s)
        for (int k = 0; k < 3; ++k)
            if (w == g_widgets[s][k] && g_wn < 64) g_wlog[g_wn++] = k * 0x100 + slot;
}
void __fastcall FakeWNotify(uint8_t* w, int, uint32_t a, uint32_t b, int32_t c, int32_t d) {
    WLog(w, 0xc);
    if (g_wn + 4 <= 64) {
        g_wlog[g_wn++] = a;
        g_wlog[g_wn++] = b;
        g_wlog[g_wn++] = (uint32_t)c;
        g_wlog[g_wn++] = (uint32_t)d;
    }
}
void __fastcall FakeWBlur(uint8_t* w) { WLog(w, 0x10); }
uint32_t __fastcall FakeWRedraw(uint8_t* w) {
    WLog(w, 0x1c);
    return 0x1234u + w[0x6a];
}
} // namespace

DIFFTEST(game_WidgetStateHelpers) {
    FakeEngine fe;
    static void* vtbl[8];
    vtbl[0xc / 4] = (void*)&FakeWNotify;
    vtbl[0x10 / 4] = (void*)&FakeWBlur;
    vtbl[0x1c / 4] = (void*)&FakeWRedraw;
    static uint8_t ui[2][0xd00], handler[0x80];
    uint8_t*& gui = hook::Global<uint8_t*>(0x6d401c);
    uint8_t* sUi = gui;
    int32_t& mute = hook::Global<int32_t>(0xccba14);
    int32_t sMute = mute;
    mute = 1;
    for (int it = 0; it < 6000; ++it) {
        uint8_t init[3][0x200];
        for (int k = 0; k < 3; ++k) {
            for (int j = 0; j < 0x200; ++j) init[k][j] = (uint8_t)Rnd();
            *(void***)init[k] = vtbl;
            *(uint16_t*)(init[k] + 0x6a) = (uint16_t)(Rnd() & 0x3f);
            init[k][0x68] = (uint8_t)(Rnd() % 4 ? 3 : Rnd());
            *(uint8_t**)(init[k] + 0x86) = handler;
            *(void**)(init[k] + 0x4b) = nullptr; // parent chain
            *(int32_t*)(init[k] + 0x5c) = (int32_t)(Rnd() % 3);
            init[k][0xc2] = (uint8_t)(Rnd() % 2);
            init[k][0xe8] = (uint8_t)(Rnd() % 2);
        }
        *(void**)(handler + 0x60) = Rnd() % 4 ? (void*)1 : nullptr;
        *(void**)(handler + 0x5c) = Rnd() % 4 ? (void*)1 : nullptr;
        fe.e[0x57] = (uint8_t)(Rnd() % 6 == 0);
        int focusSel = (int)(Rnd() % 4), hoverSel = (int)(Rnd() % 4), owner = (int)(Rnd() % 3);
        bool ownerLink = Rnd() % 2 != 0;
        int op = (int)(Rnd() % 7);
        int32_t value = (int32_t)(Rnd() % 3);
        // List links for WidgetList_Insert: widgets 1 and 2 form a chain (or not).
        int chain = (int)(Rnd() % 4);
        int32_t depths[3] = { (int32_t)(Rnd() % 5), (int32_t)(Rnd() % 5), (int32_t)(Rnd() % 5) };
        if (depths[1] > depths[2]) { int32_t d = depths[1]; depths[1] = depths[2]; depths[2] = d; }
        int32_t rows = (int32_t)(Rnd() % 20), fit = (int32_t)(Rnd() % 20), barMode = (int32_t)(Rnd() % 3);
        uint32_t mask = Rnd() & 0x3f, a = Rnd(), b = Rnd();
        uint32_t logA[64], outA[12], outB[12], rA = 0, rB = 0;
        int nA = 0;
        for (int side = 0; side < 2; ++side) {
            memcpy(g_widgets[side], init, sizeof init);
            memset(ui[side], 0, sizeof ui[side]);
            gui = ui[side];
            *(uint8_t**)(ui[side] + 0xc6d) = focusSel < 3 ? g_widgets[side][focusSel] : nullptr;
            *(uint8_t**)(ui[side] + 0xc71) = hoverSel < 3 ? g_widgets[side][hoverSel] : nullptr;
            if (focusSel < 3) *(uint8_t**)(g_widgets[side][focusSel] + 0x86) = ownerLink ? g_widgets[side][owner] : handler;
            uint8_t* w = g_widgets[side][0];
            uint8_t** W = (uint8_t**)g_widgets[side];
            (void)W;
            static uint8_t* lists[2][2];
            uint8_t* w1 = g_widgets[side][1];
            uint8_t* w2 = g_widgets[side][2];
            for (int k = 0; k < 3; ++k) *(int32_t*)(g_widgets[side][k] + 0x21) = depths[k];
            *(uint8_t**)(w1 + 0x58) = nullptr;
            *(uint8_t**)(w2 + 0x54) = nullptr;
            if (chain == 0) {
                lists[side][0] = lists[side][1] = nullptr;
            } else if (chain == 1) {
                lists[side][0] = lists[side][1] = w1;
                *(uint8_t**)(w1 + 0x54) = nullptr;
            } else {
                lists[side][1] = w1;
                lists[side][0] = w2;
                *(uint8_t**)(w1 + 0x54) = w2;
                *(uint8_t**)(w2 + 0x58) = w1;
            }
            *(int32_t*)(w + 0x1da) = rows;
            *(int32_t*)(w + 0xa2) = fit;
            *(uint8_t**)(w + 0x19a) = barMode ? w1 : nullptr;
            *(int32_t*)(w + 0x172) = (int32_t)(barMode == 2);
            uint32_t* out = side ? outB : outA;
            memset(out, 0x77, 12 * 4);
            g_wn = 0;
            uint32_t r = 0;
            switch (op) {
            case 0:
                if (side == 0) ((void(__fastcall*)(uint8_t*, int, int32_t))0x53f7f0)(w, 0, value);
                else Widget_SetValue(w, 0, value);
                break;
            case 1:
                if (side == 0) r = (uint32_t)((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x5404d0)(w, 0, out);
                else r = (uint32_t)Button_OnRelease(w, 0, out);
                break;
            case 2:
                if (side == 0) r = ((uint32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t))0x558980)(w, 0, mask, a, b);
                else r = Widget_SetFlags(w, 0, mask, a, b);
                break;
            case 3:
                if (side == 0) r = (uint32_t)((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x540890)(w, 0, out);
                else r = (uint32_t)Button_OnPress(w, 0, out);
                break;
            case 4:
                if (side == 0) ((void(__fastcall*)(uint8_t**, int, uint8_t*, int32_t))0x538e50)(lists[side], 0, w, value & 1);
                else WidgetList_Insert(lists[side], 0, w, value & 1);
                for (int k = 0; k < 2; ++k)
                    for (int s = 0; s < 3; ++s)
                        if (lists[side][k] == g_widgets[side][s]) r |= (uint32_t)(s + 1) << (k * 4);
                break;
            case 5:
                if (side == 0) ((void(__fastcall*)(uint8_t*))0x55faf0)(w);
                else List_SyncScrollbar(w);
                break;
            case 6:
                *(int32_t*)(w + 0x182) = 1; // no row refresh callbacks
                if (side == 0) ((void(__fastcall*)(uint8_t*, int, int32_t))0x561150)(w, 0, value & 1);
                else ListView_Refresh(w, 0, value & 1);
                break;
            }
            X87Clear();
            if (side == 0) {
                rA = r;
                nA = g_wn;
                memcpy(logA, g_wlog, sizeof logA);
            } else {
                rB = r;
            }
        }
        // Normalise pointers into each side's widget array before comparing.
        for (int k = 0; k < 3; ++k) {
            uint8_t na[0x200], nb[0x200];
            memcpy(na, g_widgets[0][k], 0x200);
            memcpy(nb, g_widgets[1][k], 0x200);
            const uint32_t ptrFields[4] = { 0x86, 0x54, 0x58, 0x19a };
            for (uint32_t pf : ptrFields)
                for (int s = 0; s < 3; ++s) {
                    if (*(uint8_t**)(na + pf) == g_widgets[0][s]) *(uintptr_t*)(na + pf) = s + 1;
                    if (*(uint8_t**)(nb + pf) == g_widgets[1][s]) *(uintptr_t*)(nb + pf) = s + 1;
                }
            EXPECT_EQ(FirstDiff(na, nb, 0x200), -1, "op %d widget %d iteration %d at 0x%x (%02x vs %02x)", op, k, it, FirstDiff(na, nb, 0x200), na[FirstDiff(na, nb, 0x200) & 0x1ff], nb[FirstDiff(na, nb, 0x200) & 0x1ff]);
        }
        uintptr_t fa = *(uintptr_t*)(ui[0] + 0xc6d), fb = *(uintptr_t*)(ui[1] + 0xc6d);
        uintptr_t ha = *(uintptr_t*)(ui[0] + 0xc71), hb = *(uintptr_t*)(ui[1] + 0xc71);
        for (int s = 0; s < 3; ++s) {
            if (fa == (uintptr_t)g_widgets[0][s]) fa = s + 1;
            if (fb == (uintptr_t)g_widgets[1][s]) fb = s + 1;
            if (ha == (uintptr_t)g_widgets[0][s]) ha = s + 1;
            if (hb == (uintptr_t)g_widgets[1][s]) hb = s + 1;
        }
        EXPECT_EQ(fa == fb && ha == hb, true, "op %d focus/hover iteration %d", op, it);
        EXPECT_EQ(nA == g_wn && !memcmp(logA, g_wlog, nA * 4), true, "op %d calls iteration %d", op, it);
        if (op == 1 || op == 3) {
            if (outA[11] == (uint32_t)(uintptr_t)g_widgets[0][0]) outA[11] = 1;
            if (outB[11] == (uint32_t)(uintptr_t)g_widgets[1][0]) outB[11] = 1;
            EXPECT_EQ(memcmp(outA, outB, sizeof outA), 0, "event iteration %d", it);
        }
        if (op != 0) EXPECT_EQ(rA, rB, "op %d result iteration %d", op, it);
    }
    gui = sUi;
    mute = sMute;
}

namespace rt3 {
void __fastcall Train_SetGhost(uint8_t* train, int, int32_t ghost, int32_t force);
void __fastcall Train_StopAndGhost(uint8_t* train);
void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
void __fastcall Stop_Name(uint8_t* stop, char* out);
int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
}

DIFFTEST(game_TrainAndTextHelpers) {
    FakeEngine fe;
    const int C = 6, NN = 6;
    static Collection cars, nodes, trains, cities;
    Collection* cs[4] = { &cars, &nodes, &trains, &cities };
    const uint32_t sizes[4] = { 0x400, 0x260, 0x200, 0x500 };
    const int counts[4] = { C, NN, 0, 3 };
    static uint8_t blank[0x500];
    for (int k = 0; k < 4; ++k) {
        memset(cs[k], 0, sizeof(Collection));
        Collection_Construct(cs[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
        for (int j = 0; j < counts[k]; ++j) Collection_Allocate(cs[k], 0, blank, sizes[k], 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x62ba84 }, { 0x6cfca8 }, { 0x6cfcbc }, { 0x6cec20 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<Collection*>(0x62ba84) = &cars;
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x6cfcbc) = &trains;
    hook::Global<Collection*>(0x6cec20) = &cities;
    static uint8_t opts[0x400];
    uint8_t*& gopts = hook::Global<uint8_t*>(0x6cec74);
    uint8_t* sOpts = gopts;
    gopts = opts;
    static uint8_t carInit[C][0x400], nodeInit[NN][0x260], carA[C][0x400], nodeA[NN][0x260];
    static uint8_t train[0x200], trainInit[0x200], trainA[0x200];
    for (int it = 0; it < 4000; ++it) {
        *(int32_t*)(fe.e + 0x114742) = Rnd() % 3 == 0;
        *(uint8_t*)(fe.e + 0x1146bd) = (uint8_t)(Rnd() % 4 != 0);
        for (int k = 0; k < C; ++k) {
            uint8_t* c = carInit[k];
            for (int j = 0; j < 0x400; ++j) c[j] = (uint8_t)Rnd();
            *(void**)(c + 0x75) = nullptr;
            *(int32_t*)(c + 0x31a) = 0;
            *(void**)(c + 0x7d) = nullptr;
            *(int32_t*)(c + 0x352) = k + 1 < C && Rnd() % 4 ? k + 2 : (Rnd() % 2 ? -1 : 0x30);
            *(int32_t*)(c + 0x35a) = (int32_t)(Rnd() % 4) - 1;
        }
        for (int k = 0; k < NN; ++k) {
            uint8_t* n = nodeInit[k];
            for (int j = 0; j < 0x260; j += 4) *(float*)(n + j) = RndF(0.0f, 3000.0f);
            for (int j = 0x21a; j < 0x244; j += 4) *(int32_t*)(n + j) = (int32_t)(Rnd() % 4) - 3;
        }
        for (int j = 0; j < 0x200; ++j) trainInit[j] = (uint8_t)Rnd();
        *(uint32_t*)(trainInit + 0x41) = 1 + Rnd() % C;
        *(uint32_t*)(trainInit + 0x45) = 1 + Rnd() % C;
        *(int32_t*)(trainInit + 0x49) = Rnd() % 3 ? 1 + (int32_t)(Rnd() % NN) : -1;
        *(int32_t*)(trainInit + 0x4d) = Rnd() % 3 ? 1 + (int32_t)(Rnd() % NN) : -1;
        *(int32_t*)(trainInit + 0x25) = (int32_t)(Rnd() % 4) - 1;
        trainInit[0xf4] = (uint8_t)(Rnd() % 3);
        trainInit[0x1a1] = (uint8_t)(Rnd() % 3 == 0);
        trainInit[0x1ce] = (uint8_t)Rnd();
        int op = (int)(Rnd() % 3);
        int32_t ghost = (int32_t)(Rnd() % 3), force = (int32_t)(Rnd() % 2);
        *(int32_t*)(opts + 0x287) = (int32_t)(Rnd() % 2);
        *(int32_t*)(trainInit + 0xd8) = (int32_t)(Rnd() % 2);
        *(int32_t*)(trainInit + 0xe8) = Rnd() % 3 ? 0 : 5;
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < C; ++k) memcpy(Collection_Get(&cars, 0, k + 1), carInit[k], 0x400);
            for (int k = 0; k < NN; ++k) memcpy(Collection_Get(&nodes, 0, k + 1), nodeInit[k], 0x260);
            memcpy(train, trainInit, sizeof train);
            if (op == 0) {
                if (side == 0) ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4a8220)(train, 0, ghost, force);
                else Train_SetGhost(train, 0, ghost, force);
            } else if (op == 1) {
                if (side == 0) ((void(__fastcall*)(uint8_t*))0x4ad170)(train);
                else Train_StopAndGhost(train);
            } else {
                if (side == 0) ((void(__fastcall*)(uint8_t*, int, int32_t))0x4abd70)(train, 0, ghost & 1);
                else Train_SetRunning(train, 0, ghost & 1);
            }
            X87Clear();
            if (side == 0) {
                for (int k = 0; k < C; ++k) memcpy(carA[k], Collection_Get(&cars, 0, k + 1), 0x400);
                for (int k = 0; k < NN; ++k) memcpy(nodeA[k], Collection_Get(&nodes, 0, k + 1), 0x260);
                memcpy(trainA, train, sizeof train);
            }
        }
        for (int k = 0; k < C; ++k) EXPECT_EQ(FirstDiff(carA[k], (uint8_t*)Collection_Get(&cars, 0, k + 1), 0x400), -1, "op %d car %d iteration %d", op, k, it);
        for (int k = 0; k < NN; ++k)
            EXPECT_EQ(FirstDiff(nodeA[k], (uint8_t*)Collection_Get(&nodes, 0, k + 1), 0x260), -1, "op %d node %d iteration %d", op, k, it);
        EXPECT_EQ(FirstDiff(trainA, train, sizeof train), -1, "op %d train iteration %d", op, it);
        // Display name.
        int32_t numberOnly = (int32_t)(Rnd() % 2);
        memcpy(train, trainInit, sizeof train);
        char* ra = ((char*(__fastcall*)(uint8_t*, int, int32_t))0x4a9360)(train, 0, numberOnly);
        memcpy(trainA, train, sizeof train);
        memcpy(train, trainInit, sizeof train);
        char* rb = Train_DisplayName(train, 0, numberOnly);
        EXPECT_EQ(ra == rb && memcmp(trainA, train, sizeof train) == 0, true, "display name iteration %d", it);
        // Stop names.
        uint8_t stop[0x40];
        for (uint8_t& s : stop) s = (uint8_t)Rnd();
        *(uint32_t*)(stop + 0x2b) = 1 + Rnd() % NN;
        *(uint16_t*)(stop + 0x29) = (uint16_t)(1 + Rnd() % 3);
        for (int k = 1; k <= 3; ++k) sprintf((char*)Collection_Get(&cities, 0, k) + 0x46b, "City %d-%u", k, Rnd() % 100);
        char sa[0x200], sb[0x200];
        memset(sa, 0x5a, sizeof sa);
        memset(sb, 0x5a, sizeof sb);
        ((void(__fastcall*)(uint8_t*, char*))0x4be6f0)(stop, sa);
        Stop_Name(stop, sb);
        EXPECT_EQ(memcmp(sa, sb, sizeof sa), 0, "stop name iteration %d: '%.40s' vs '%.40s'", it, sa, sb);
        // Debug tooltip.
        uint8_t* t = nodeInit[Rnd() % NN];
        int32_t n = 1 + (int32_t)(Rnd() % 40);
        memset(sa, 0x5a, sizeof sa);
        memset(sb, 0x5a, sizeof sb);
        int32_t ta = ((int32_t(__fastcall*)(uint8_t*, int, char*, int32_t, int32_t, int32_t, int32_t, int32_t))0x53a450)(t, 0, sa, n, 0, 0, 0, 0);
        int32_t tb = Track_DebugTooltip(t, 0, sb, n, 0, 0, 0, 0);
        EXPECT_EQ(ta == tb && memcmp(sa, sb, sizeof sa) == 0, true, "tooltip iteration %d: '%.40s' vs '%.40s'", it, sa, sb);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    gopts = sOpts;
    for (Collection* c : cs) Collection_Destruct(c);
}

namespace rt3 {
int32_t __fastcall Train_LoadExtra(uint8_t* train, int, int32_t st);
void __fastcall Trains_ReapplyGhosts(Collection* c);
}

DIFFTEST(game_TrainLoadAndGhosts) {
    FakeEngine fe;
    const int C = 4, T = 3;
    static Collection cars, trains;
    memset(&cars, 0, sizeof cars);
    memset(&trains, 0, sizeof trains);
    Collection_Construct(&cars, 0, 1, 0x400, 8, 8, 0, 0, 0);
    Collection_Construct(&trains, 0, 1, 0x200, 8, 8, 0, 0, 0);
    static uint8_t blank[0x400];
    for (int k = 0; k < C; ++k) Collection_Allocate(&cars, 0, blank, 0x400, 0, 0);
    for (int k = 0; k < T; ++k) Collection_Allocate(&trains, 0, blank, 0x200, 0, 0);
    Collection*& gcars = hook::Global<Collection*>(0x62ba84);
    Collection*& gtrains = hook::Global<Collection*>(0x6cfcbc);
    Collection *sCars = gcars, *sTrains = gtrains;
    gcars = &cars;
    gtrains = &trains;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_train_extra.bin");
    static uint8_t carInit[C][0x400], trainInit[T][0x200], carA[C][0x400], trainA[T][0x200];
    for (int it = 0; it < 600; ++it) {
        *(int32_t*)(fe.e + 0x114742) = Rnd() % 3 == 0;
        for (int k = 0; k < C; ++k) {
            uint8_t* c = carInit[k];
            for (int j = 0; j < 0x400; ++j) c[j] = (uint8_t)Rnd();
            *(void**)(c + 0x75) = nullptr;
            *(int32_t*)(c + 0x31a) = 0;
            *(void**)(c + 0x7d) = nullptr;
            *(int32_t*)(c + 0x352) = k + 1 < C && Rnd() % 3 ? k + 2 : -1;
        }
        for (int k = 0; k < T; ++k) {
            uint8_t* t = trainInit[k];
            for (int j = 0; j < 0x200; ++j) t[j] = (uint8_t)Rnd();
            *(uint32_t*)(t + 0x41) = 1 + Rnd() % C;
            *(int32_t*)(t + 0x49) = -1; // no track nodes to unlink
            t[0xf4] = (uint8_t)(Rnd() % 3 == 0);
            *(int32_t*)(t + 4) = (int32_t)(Rnd() % 5);
        }
        // A stream holding: tag, entries, tag.
        FILE* w = fopen(path, "wb");
        if (!w) return;
        int32_t count = *(int32_t*)(trainInit[0] + 4);
        for (int k = 0; k < 8 + count * 0x33; ++k) fputc((int)(Rnd() & 0xff), w);
        fclose(w);
        int32_t ra = 0, rb = 0;
        uint8_t entriesA[5 * 0x33] = {}, entriesB[5 * 0x33] = {};
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < C; ++k) memcpy(Collection_Get(&cars, 0, k + 1), carInit[k], 0x400);
            for (int k = 0; k < T; ++k) memcpy(Collection_Get(&trains, 0, k + 1), trainInit[k], 0x200);
            uint8_t* t0 = (uint8_t*)Collection_Get(&trains, 0, 1);
            Stream st = StreamOpen(path, 5, 0, 0);
            int32_t r;
            if (side == 0) {
                r = ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x4a84b0)(t0, 0, st);
                ((void(__fastcall*)(Collection*))0x4b2ae0)(&trains);
            } else {
                r = Train_LoadExtra(t0, 0, st);
                Trains_ReapplyGhosts(&trains);
            }
            StreamClose(st);
            (side ? rb : ra) = r;
            if (count) {
                void* arr = *(void**)t0;
                memcpy(side ? entriesB : entriesA, arr, count * 0x33);
                ((void(__cdecl*)(void*))0x5a1145)(arr);
                *(void**)t0 = nullptr;
            }
            X87Clear();
            if (side == 0) {
                for (int k = 0; k < C; ++k) memcpy(carA[k], Collection_Get(&cars, 0, k + 1), 0x400);
                for (int k = 0; k < T; ++k) memcpy(trainA[k], Collection_Get(&trains, 0, k + 1), 0x200);
            }
        }
        EXPECT_EQ(ra, rb, "bytes read iteration %d", it);
        EXPECT_EQ(memcmp(entriesA, entriesB, sizeof entriesA), 0, "entries iteration %d", it);
        for (int k = 0; k < C; ++k) EXPECT_EQ(FirstDiff(carA[k], (uint8_t*)Collection_Get(&cars, 0, k + 1), 0x400), -1, "car %d iteration %d", k, it);
        for (int k = 0; k < T; ++k)
            EXPECT_EQ(FirstDiff(trainA[k], (uint8_t*)Collection_Get(&trains, 0, k + 1), 0x200), -1, "train %d iteration %d", k, it);
    }
    DeleteFileA(path);
    gcars = sCars;
    gtrains = sTrains;
    Collection_Destruct(&cars);
    Collection_Destruct(&trains);
}

namespace rt3 {
void World_RefreshColors();
}

namespace {
uint32_t g_rlog[256];
int g_rn;
void __fastcall FakeRefresh54(uint8_t* o, int, int32_t a) {
    if (g_rn + 2 <= 256) {
        g_rlog[g_rn++] = *(uint32_t*)(o + 4);
        g_rlog[g_rn++] = (uint32_t)a;
    }
}
} // namespace

DIFFTEST(game_WorldRefreshColors) {
    FakeEngine fe;
    static void* vtbl[0x20];
    vtbl[0x54 / 4] = (void*)&FakeRefresh54;
    static Collection tabs[4], pieces;
    const uint32_t addrs[4] = { 0x62b26c, 0x6ada80, 0x62bae0, 0x6cea50 };
    uint32_t saved[7];
    for (int k = 0; k < 4; ++k) saved[k] = hook::Global<uint32_t>(addrs[k]);
    saved[4] = hook::Global<uint32_t>(0x6cfca8);
    saved[5] = hook::Global<uint32_t>(0x6cec74);
    saved[6] = hook::Global<uint32_t>(0x6cec78);
    static uint8_t opts[0x400], world[0x5000], blank[0x260];
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(int32_t*)(world + 0x4ae7) = 0;
    static uint32_t piecesMem[0x80];
    Collection& pc = *(Collection*)piecesMem;
    memset(piecesMem, 0, sizeof piecesMem);
    Collection_Construct(&pc, 0, 1, 0x260, 8, 8, 0, 0, 0);
    const int P = 4;
    for (int k = 0; k < P; ++k) Collection_Allocate(&pc, 0, blank, 0x260, 0, 0);
    hook::Global<Collection*>(0x6cfca8) = &pc;
    for (int k = 0; k < 4; ++k) {
        memset(&tabs[k], 0, sizeof(Collection));
        Collection_Construct(&tabs[k], 0, 1, 0x40, 8, 8, 0, 0, 0);
        for (int j = 0; j < 1 + k; ++j) {
            uint8_t* r = (uint8_t*)Collection_Get(&tabs[k], 0, Collection_Allocate(&tabs[k], 0, blank, 0x40, 0, 0));
            *(void***)r = vtbl;
            *(uint32_t*)(r + 4) = (uint32_t)(k * 16 + j);
        }
        hook::Global<Collection*>(addrs[k]) = &tabs[k];
    }
    static uint8_t init[P][0x260], outA[P][0x260];
    for (int it = 0; it < 500; ++it) {
        *(int32_t*)(opts + 0x2bf) = (int32_t)(Rnd() % 3);
        *(int32_t*)(opts + 0x98) = (int32_t)(Rnd() % 2);
        *(int32_t*)(world + 0x4ae3) = (int32_t)(Rnd() % 2);
        for (int k = 0; k < P; ++k) {
            for (int j = 0; j < 0x260; ++j) init[k][j] = (uint8_t)Rnd();
            *(void**)(init[k] + 8) = nullptr;
            init[k][0x216] = (uint8_t)(1 + Rnd() % 2);
        }
        uint32_t logA[256];
        int nA = 0;
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < P; ++k) memcpy(Collection_Get(&pc, 0, k + 1), init[k], 0x260);
            g_rn = 0;
            if (side == 0) ((void(*)())0x4361d0)();
            else World_RefreshColors();
            if (side == 0) {
                nA = g_rn;
                memcpy(logA, g_rlog, sizeof logA);
                for (int k = 0; k < P; ++k) memcpy(outA[k], Collection_Get(&pc, 0, k + 1), 0x260);
            }
        }
        EXPECT_EQ(nA == g_rn && !memcmp(logA, g_rlog, nA * 4), true, "calls iteration %d", it);
        for (int k = 0; k < P; ++k)
            EXPECT_EQ(FirstDiff(outA[k], (uint8_t*)Collection_Get(&pc, 0, k + 1), 0x260), -1, "piece %d iteration %d", k, it);
    }
    for (int k = 0; k < 4; ++k) {
        hook::Global<uint32_t>(addrs[k]) = saved[k];
        Collection_Destruct(&tabs[k]);
    }
    hook::Global<uint32_t>(0x6cfca8) = saved[4];
    hook::Global<uint32_t>(0x6cec74) = saved[5];
    hook::Global<uint32_t>(0x6cec78) = saved[6];
    Collection_Destruct(&pc);
}

namespace rt3 {
void Mem_ReportLeaks();
void __fastcall Train_DestinationText(void* train, int, char* dst, int32_t n);
int32_t __fastcall Trains_Load(Collection* c, int, int32_t st, int32_t recordSize);
}

namespace {
std::string g_dbgOut;
void __stdcall FakeOutputDebugString(const char* s) { g_dbgOut += s; g_dbgOut += '|'; }
} // namespace

DIFFTEST(game_MemReportLeaks) {
    void** iat = (void**)0x5c8078;
    DWORD oldProt;
    VirtualProtect(iat, 4, PAGE_READWRITE, &oldProt);
    void* sOut = *iat;
    *iat = (void*)&FakeOutputDebugString;
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0xccba34 }, { 0xccba30 }, { 0xccba2c }, { 0xccba4c }, { 0xccba3c }, { 0x6d3ca0 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<int32_t>(0x6d3ca0) = 0;
    static void* ptrs[8];
    static uint8_t recs[8][0x30];
    for (int it = 0; it < 500; ++it) {
        int32_t n = (int32_t)(Rnd() % 9);
        for (int k = 0; k < 8; ++k) {
            ptrs[k] = Rnd() % 3 ? (void*)(uintptr_t)(Rnd() | 1) : nullptr;
            for (uint8_t& b : recs[k]) b = (uint8_t)Rnd();
            sprintf((char*)recs[k] + 4, "f%u.cpp", Rnd() % 1000);
            sprintf((char*)recs[k] + 0x1b, "res%u", Rnd() % 1000);
        }
        int32_t counter = (int32_t)(Rnd() % 50);
        int32_t total = (int32_t)Rnd();
        std::string outA;
        int32_t counterA = 0;
        for (int side = 0; side < 2; ++side) {
            hook::Global<int32_t>(0xccba34) = n;
            hook::Global<void**>(0xccba30) = ptrs;
            hook::Global<uint8_t*>(0xccba2c) = &recs[0][0];
            hook::Global<int32_t>(0xccba4c) = counter;
            hook::Global<int32_t>(0xccba3c) = total;
            g_dbgOut.clear();
            if (side == 0) ((void(*)())0x53ae70)();
            else Mem_ReportLeaks();
            if (side == 0) {
                outA = g_dbgOut;
                counterA = hook::Global<int32_t>(0xccba4c);
            }
        }
        EXPECT_EQ(outA == g_dbgOut && counterA == hook::Global<int32_t>(0xccba4c), true, "report iteration %d: '%.80s' vs '%.80s'", it,
                  outA.c_str(), g_dbgOut.c_str());
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    *iat = sOut;
    VirtualProtect(iat, 4, oldProt, &oldProt);
}

DIFFTEST(game_TrainDestinationAndLoad) {
    FakeEngine fe;
    const int NN = 4, C = 3, T = 3;
    static Collection nodes, cities, cars, src, dstA, dstB;
    Collection* fixed[3] = { &nodes, &cities, &cars };
    const uint32_t sizes[3] = { 0x260, 0x500, 0x400 };
    const int counts[3] = { NN, 3, C };
    static uint8_t blank[0x500];
    for (int k = 0; k < 3; ++k) {
        memset(fixed[k], 0, sizeof(Collection));
        Collection_Construct(fixed[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
        for (int j = 0; j < counts[k]; ++j) Collection_Allocate(fixed[k], 0, blank, sizes[k], 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6cfca8 }, { 0x6cec20 }, { 0x62ba84 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x6cec20) = &cities;
    hook::Global<Collection*>(0x62ba84) = &cars;
    for (int k = 1; k <= C; ++k) {
        uint8_t* c = (uint8_t*)Collection_Get(&cars, 0, k);
        *(int32_t*)(c + 0x352) = -1;
    }
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_trains.bin");
    for (int it = 0; it < 1500; ++it) {
        // Destination text.
        for (int k = 1; k <= NN; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            for (int j = 0; j < 0x40; j += 4) *(float*)(n + j) = RndF(0.0f, 3000.0f);
        }
        for (int k = 1; k <= 3; ++k) sprintf((char*)Collection_Get(&cities, 0, k) + 0x46b, "Town%d %u", k, Rnd() % 1000);
        uint8_t stops[3 * 0x33];
        for (uint8_t& b : stops) b = (uint8_t)Rnd();
        for (int k = 0; k < 3; ++k) {
            *(uint32_t*)(stops + k * 0x33 + 0x2b) = 1 + Rnd() % NN;
            *(uint16_t*)(stops + k * 0x33 + 0x29) = (uint16_t)(1 + Rnd() % 3);
        }
        uint8_t train[0x10];
        *(uint8_t**)train = stops;
        *(int32_t*)(train + 8) = (int32_t)(Rnd() % 4) - 1;
        int32_t n = 1 + (int32_t)(Rnd() % 60);
        char a[0x200], b[0x200];
        memset(a, 0x5a, sizeof a);
        memset(b, 0x5a, sizeof b);
        ((void(__fastcall*)(void*, int, char*, int32_t))0x4a8540)(train, 0, a, n);
        Train_DestinationText(train, 0, b, n);
        EXPECT_EQ(memcmp(a, b, sizeof a), 0, "destination iteration %d: '%.40s' vs '%.40s'", it, a, b);
        if (it % 5) continue;
        // Train table round trip: save a source table plus extras, load it twice.
        memset(&src, 0, sizeof src);
        Collection_Construct(&src, 0, 1, 0x200, 8, 8, 0, 0, 0);
        int32_t extraCounts[T];
        for (int k = 0; k < T; ++k) {
            uint8_t rec[0x200];
            for (uint8_t& x : rec) x = (uint8_t)Rnd();
            *(uint32_t*)(rec + 0x41) = 1 + Rnd() % C;
            *(int32_t*)(rec + 0x49) = -1;
            rec[0xf4] = (uint8_t)(Rnd() % 3 == 0);
            extraCounts[k] = (int32_t)(Rnd() % 4);
            *(int32_t*)(rec + 4) = extraCounts[k];
            Collection_Allocate(&src, 0, rec, 0x200, 0, 0);
        }
        Stream ws = StreamOpen(path, 0, 0, 0);
        StreamWriteU32(ws, 0x5209);
        Collection_Save(&src, 0, ws, 0x200);
        StreamWriteU32(ws, 0x520a);
        for (int k = 0; k < T; ++k) {
            StreamWriteU32(ws, 0x5dc1);
            for (int j = 0; j < extraCounts[k] * 0x33; ++j) {
                uint8_t x = (uint8_t)Rnd();
                StreamWrite(ws, &x, 1, 0);
            }
            StreamWriteU32(ws, 0x5dc2);
        }
        StreamWriteU32(ws, 0x520b);
        StreamClose(ws);
        Collection_Destruct(&src);
        int32_t ra = 0, rb = 0;
        for (int side = 0; side < 2; ++side) {
            Collection& d = side ? dstB : dstA;
            memset(&d, 0, sizeof d);
            Collection_Construct(&d, 0, 1, 0x200, 8, 8, 0, 0, 0);
            Stream rs = StreamOpen(path, 5, 0, 0);
            if (side == 0) ra = ((int32_t(__fastcall*)(Collection*, int, int32_t, int32_t))0x4b2700)(&d, 0, rs, 0x200);
            else rb = Trains_Load(&d, 0, rs, 0x200);
            StreamClose(rs);
            X87Clear();
        }
        EXPECT_EQ(ra, rb, "load bytes iteration %d", it);
        EXPECT_EQ(Collection_Count(&dstA), Collection_Count(&dstB), "load count iteration %d", it);
        for (int k = 0; k < Collection_Count(&dstA) && k < Collection_Count(&dstB); ++k) {
            uint8_t* pa = (uint8_t*)Collection_Get(&dstA, 0, Collection_IdAt(&dstA, 0, k, 0));
            uint8_t* pb = (uint8_t*)Collection_Get(&dstB, 0, Collection_IdAt(&dstB, 0, k, 0));
            int32_t cnt = *(int32_t*)(pa + 4);
            void* ea = *(void**)pa;
            void* eb = *(void**)pb;
            EXPECT_EQ(cnt == 0 || (ea && eb && memcmp(ea, eb, cnt * 0x33) == 0), true, "extras %d iteration %d", k, it);
            EXPECT_EQ(memcmp(pa + 4, pb + 4, 0x200 - 4), 0, "train %d iteration %d", k, it);
            if (cnt) {
                ((void(__cdecl*)(void*))0x5a1145)(ea);
                ((void(__cdecl*)(void*))0x5a1145)(eb);
            }
        }
        Collection_Destruct(&dstA);
        Collection_Destruct(&dstB);
    }
    DeleteFileA(path);
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection* c : fixed) Collection_Destruct(c);
}

namespace rt3 {
int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused,
                                          int32_t byDegree, int32_t skipLinked);
void Mem_Shutdown();
}

DIFFTEST(game_GridNearestNodeWithin) {
    auto orig = (int32_t(__fastcall*)(Collection*, int, float, float, int32_t, float, int32_t, int32_t, int32_t))0x494cb0;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    static uint32_t gmem[0x100];
    Collection& g = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&g, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 12, W = 16;
    for (int k = 0; k < N; ++k) Collection_Allocate(&g, 0, blank, 0x260, 0, 0);
    static uint8_t map[0x2200];
    *(int32_t*)(map + 0x15d9) = W;
    *(int32_t*)(map + 0x15d5) = W;
    *(int32_t*)(map + 0x2155) = W;
    *(int32_t*)(map + 0x2159) = W;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sMap = gmap;
    gmap = map;
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &g;
    static uint8_t* columns[W];
    static uint8_t entries[W][W][10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    int found = 0;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&g, 0, k);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, W * 92.0f);
            *(int32_t*)(n + 0x202) = k;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % N);
        }
        // Each column: a sorted chain of cell entries, each listing a few live nodes.
        for (int x = 0; x < W; ++x) {
            columns[x] = nullptr;
            uint8_t* prev = nullptr;
            for (int y = 0; y < W; ++y) {
                if (Rnd() % 3) continue;
                uint8_t* e = entries[x][y];
                *(uint16_t*)e = (uint16_t)y;
                uint32_t* head = nullptr;
                for (int c = (int)(Rnd() % 3); c > 0; --c) {
                    uint32_t* l = (uint32_t*)xmalloc(8);
                    l[0] = 1 + Rnd() % N;
                    l[1] = (uint32_t)head;
                    head = l;
                }
                *(uint32_t**)(e + 2) = head;
                *(uint8_t**)(e + 6) = nullptr;
                if (prev) *(uint8_t**)(prev + 6) = e;
                else columns[x] = e;
                prev = e;
            }
        }
        float px = RndF(0.0f, (float)W), py = RndF(0.0f, (float)W);
        float radius = Rnd() % 8 == 0 ? -1.0f : RndF(0.0f, 6.0f);
        int32_t exclude = Rnd() % 3 ? -1 : 1 + (int32_t)(Rnd() % N);
        int32_t byDegree = (int32_t)(Rnd() % 2), skip = (int32_t)(Rnd() % 2);
        int32_t ra = orig(&g, 0, px, py, exclude, radius, 0, byDegree, skip);
        int32_t rb = Grid_NearestNodeWithin(&g, 0, px, py, exclude, radius, 0, byDegree, skip);
        found += ra != -1;
        EXPECT_EQ(ra, rb, "result iteration %d", it);
        for (int x = 0; x < W; ++x)
            for (uint8_t* e = columns[x]; e; e = *(uint8_t**)(e + 6))
                for (uint32_t* l = *(uint32_t**)(e + 2); l;) {
                    uint32_t* next = (uint32_t*)l[1];
                    xfree(l);
                    l = next;
                }
    }
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    gmap = sMap;
    gnodes = sNodes;
    Collection_Destruct(&g);
    EXPECT_EQ(found > 500, true, "coverage: %d found", found);
}

DIFFTEST(game_MemShutdown) {
    void** iat = (void**)0x5c8078;
    DWORD oldProt;
    VirtualProtect(iat, 4, PAGE_READWRITE, &oldProt);
    void* sOut = *iat;
    *iat = (void*)&FakeOutputDebugString;
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0xccba34 }, { 0xccba30 }, { 0xccba2c }, { 0xccba4c }, { 0xccba3c }, { 0x6d3ca0 }, { 0xccba28 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<int32_t>(0x6d3ca0) = 0;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    for (int it = 0; it < 200; ++it) {
        int32_t n = (int32_t)(Rnd() % 5);
        std::string outA;
        uint32_t stateA[4] = {};
        bool hasPtrs = Rnd() % 4 != 0, hasRecs = Rnd() % 4 != 0;
        for (int side = 0; side < 2; ++side) {
            void** ptrs = hasPtrs ? (void**)xmalloc(8 * sizeof(void*)) : nullptr;
            uint8_t* recs = hasRecs ? (uint8_t*)xmalloc(8 * 0x30) : nullptr;
            if (!ptrs || !recs) n = 0;
            for (int k = 0; ptrs && k < 8; ++k) ptrs[k] = k % 2 ? (void*)(uintptr_t)(0x1000 + k) : nullptr;
            for (int k = 0; recs && k < 8; ++k) {
                memset(recs + k * 0x30, 0, 0x30);
                sprintf((char*)recs + k * 0x30 + 4, "m%d.cpp", k);
            }
            hook::Global<int32_t>(0xccba34) = n;
            hook::Global<void**>(0xccba30) = ptrs;
            hook::Global<uint8_t*>(0xccba2c) = recs;
            hook::Global<int32_t>(0xccba4c) = 0;
            hook::Global<int32_t>(0xccba3c) = 1234;
            hook::Global<int32_t>(0xccba28) = 77;
            g_dbgOut.clear();
            if (side == 0) ((void(*)())0x53b020)();
            else Mem_Shutdown();
            uint32_t state[4] = { hook::Global<uint32_t>(0xccba34), hook::Global<uint32_t>(0xccba30), hook::Global<uint32_t>(0xccba2c),
                                  hook::Global<uint32_t>(0xccba28) };
            if (side == 0) {
                outA = g_dbgOut;
                memcpy(stateA, state, sizeof state);
            } else {
                EXPECT_EQ(outA == g_dbgOut && !memcmp(stateA, state, sizeof state), true, "shutdown iteration %d", it);
            }
        }
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    *iat = sOut;
    VirtualProtect(iat, 4, oldProt, &oldProt);
}

namespace rt3 {
void UpgradeDialog_UpdateCostText();
void __fastcall TreeView_AddRow(uint8_t* tv, int, const uint32_t* row);
}

DIFFTEST(game_UpgradeDialogCostText) {
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    static uint8_t dlg[0x200], tree[0x200], line[0x200], world[0x5000];
    static uint8_t rows[4][0x20];
    static Collection engines, types;
    memset(&engines, 0, sizeof engines);
    memset(&types, 0, sizeof types);
    Collection_Construct(&engines, 0, 1, 0x100, 8, 8, 0, 0, 0);
    Collection_Construct(&types, 0, 1, 0x100, 8, 8, 0, 0, 0);
    static uint8_t blank[0x100];
    for (int k = 0; k < 3; ++k) {
        Collection_Allocate(&engines, 0, blank, 0x100, 0, 0);
        Collection_Allocate(&types, 0, blank, 0x100, 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6d3ac8 }, { 0x6ada84 }, { 0x62ba88 }, { 0x6cec78 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    static uint8_t selSaved[0x2000];
    memcpy(selSaved, (void*)0x6d1aa8, sizeof selSaved);
    hook::Global<Collection*>(0x6ada84) = &engines;
    hook::Global<Collection*>(0x62ba88) = &types;
    hook::Global<uint8_t*>(0x6cec78) = world;
    *(uint8_t**)(dlg + 0x70) = tree;
    *(uint16_t*)(tree + 4) = 0x2afe;
    *(uint8_t**)(tree + 0x8e) = line;
    *(uint16_t*)(line + 4) = 0x275c;
    *(uint8_t**)(line + 0x8e) = nullptr;
    *(uint8_t**)(tree + 0x176) = &rows[0][0];
    for (int k = 1; k <= 3; ++k) sprintf((char*)Collection_Get(&types, 0, k) + 0x45, "Engine%d", k);
    for (int it = 0; it < 1000; ++it) {
        hook::Global<uint8_t*>(0x6d3ac8) = Rnd() % 10 ? dlg : nullptr;
        *(int32_t*)(tree + 0x1d6) = Rnd() % 8 == 0 ? -1 : (int32_t)(Rnd() % 4);
        for (auto& r : rows) *(uint32_t*)(r + 8) = Rnd() % 5 == 0 ? 0 : 1 + Rnd() % 3;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* e = (uint8_t*)Collection_Get(&engines, 0, k);
            *(float*)(e + 4) = RndF(0.0f, 900000.0f);
            *(int32_t*)(e + 0x10) = (int32_t)(Rnd() % 4);
            *(uint32_t*)(e + 0x72) = 1 + Rnd() % 3;
            *(int32_t*)e = (int32_t)(Rnd() % 8);
        }
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(world + 0x8a + k * 4) = (int32_t)(Rnd() % 200) - 100;
        memset((void*)0x6d1aa8, 0, 0x2000);
        for (int k = (int)(Rnd() % 4); k > 0; --k) ((uint8_t*)0x6d1aa8)[Rnd() % 0x2000] = 1;
        std::string textA;
        for (int side = 0; side < 2; ++side) {
            *(char**)(line + 0xba) = nullptr;
            if (side == 0) ((void(*)())0x50f890)();
            else UpgradeDialog_UpdateCostText();
            X87Clear();
            char* s = *(char**)(line + 0xba);
            std::string text = s ? s : "(none)";
            if (s) xfree(s);
            *(char**)(line + 0xba) = nullptr;
            if (side == 0) textA = text;
            else EXPECT_EQ(textA == text, true, "text iteration %d: '%s' vs '%s'", it, textA.c_str(), text.c_str());
        }
    }
    memcpy((void*)0x6d1aa8, selSaved, sizeof selSaved);
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    Collection_Destruct(&engines);
    Collection_Destruct(&types);
}

DIFFTEST(game_TreeViewAddRow) {
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    static uint8_t tv[2][0x200], bar[2][0x200];
    for (int it = 0; it < 3000; ++it) {
        uint8_t init[0x200], barInit[0x200];
        for (uint8_t& b : init) b = (uint8_t)Rnd();
        for (uint8_t& b : barInit) b = (uint8_t)Rnd();
        static void* barVtbl[8] = {};
        static uint8_t barHandler[0x80];
        barVtbl[0xc / 4] = (void*)&FakeWNotify;
        barVtbl[0x10 / 4] = (void*)&FakeWBlur;
        barVtbl[0x1c / 4] = (void*)&FakeWRedraw;
        *(void***)barInit = barVtbl;
        *(void**)(barInit + 0x4b) = nullptr;
        *(uint8_t**)(barInit + 0x86) = barHandler;
        int32_t n = (int32_t)(Rnd() % 6);
        *(int32_t*)(init + 0x1d2) = n;
        *(int32_t*)(init + 0x182) = 1; // no row refresh callbacks
        *(int32_t*)(init + 0x1da) = (int32_t)(Rnd() % 8);
        *(int32_t*)(init + 0xa2) = (int32_t)(Rnd() % 8);
        *(int32_t*)(init + 0x1e6) = (int32_t)(Rnd() % 3);
        *(int32_t*)(init + 0x1e2) = Rnd() % 2 ? *(int32_t*)(init + 0x1e6) : 9;
        *(int32_t*)(init + 0xea) = (int32_t)(Rnd() % 2);
        bool hasBar = Rnd() % 3 != 0;
        uint8_t rowsInit[8][0x20];
        for (int k = 0; k < 8; ++k) {
            for (uint8_t& b : rowsInit[k]) b = (uint8_t)Rnd();
            *(int32_t*)(rowsInit[k] + 0x10) = k > 0 && Rnd() % 2;
            *(int32_t*)(rowsInit[k] + 0x14) = k > 0 ? (int32_t)(Rnd() % k) : 0;
            *(int32_t*)(rowsInit[k] + 0x18) = (int32_t)(Rnd() % 2);
        }
        uint32_t row[8];
        for (uint32_t& v : row) v = Rnd();
        static const char* strs[3] = { "alpha", "beta gamma", nullptr };
        row[0] = (uint32_t)strs[Rnd() % 3];
        row[1] = (uint32_t)strs[Rnd() % 3];
        row[4] = n > 0 && Rnd() % 2;
        row[5] = n > 0 ? Rnd() % n : 0;
        uint8_t* outRows[2] = {};
        for (int side = 0; side < 2; ++side) {
            memcpy(tv[side], init, sizeof init);
            memcpy(bar[side], barInit, sizeof barInit);
            *(uint8_t**)(tv[side] + 0x19a) = hasBar ? bar[side] : nullptr;
            uint8_t* rows = n ? (uint8_t*)xmalloc(n * 0x20) : nullptr;
            if (n) memcpy(rows, rowsInit, n * 0x20);
            *(uint8_t**)(tv[side] + 0x176) = rows;
            if (side == 0) ((void(__fastcall*)(uint8_t*, int, const uint32_t*))0x561aa0)(tv[side], 0, row);
            else TreeView_AddRow(tv[side], 0, row);
            outRows[side] = *(uint8_t**)(tv[side] + 0x176);
        }
        // Rows: same bytes except the new row's two string copies (compared by text).
        for (int k = 0; k <= n; ++k) {
            uint8_t* a = outRows[0] + k * 0x20;
            uint8_t* b = outRows[1] + k * 0x20;
            if (k == n) {
                for (int s = 0; s < 2; ++s) {
                    EXPECT_EQ(strcmp(*(char**)(a + s * 4), *(char**)(b + s * 4)), 0, "row string iteration %d", it);
                    xfree(*(char**)(a + s * 4));
                    xfree(*(char**)(b + s * 4));
                }
                EXPECT_EQ(memcmp(a + 8, b + 8, 0x18), 0, "new row iteration %d", it);
            } else {
                EXPECT_EQ(memcmp(a, b, 0x20), 0, "row %d iteration %d", k, it);
            }
        }
        xfree(outRows[0]);
        xfree(outRows[1]);
        *(uint8_t**)(tv[0] + 0x176) = *(uint8_t**)(tv[1] + 0x176) = nullptr;
        *(uint8_t**)(tv[0] + 0x19a) = *(uint8_t**)(tv[1] + 0x19a) = nullptr;
        EXPECT_EQ(FirstDiff(tv[0], tv[1], 0x200), -1, "view iteration %d", it);
        EXPECT_EQ(FirstDiff(bar[0], bar[1], 0x200), -1, "bar iteration %d", it);
    }
}

namespace rt3 {
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
}


DIFFTEST(game_SceneObjDebugTooltip) {
    FakeEngine fe;
    static uint8_t renderer[0x40], obj[0x100], model[0x40], parts[0x400];
    static void* vtbl[8];
    vtbl[0x14 / 4] = (void*)&FakeSlot14;
    *(void**)(fe.e + 0x18) = renderer;
    *(void***)obj = vtbl;
    *(uint8_t**)(model + 0xc) = parts;
    for (int it = 0; it < 3000; ++it) {
        *(int32_t*)(fe.e + 0x1146be) = (int32_t)(Rnd() % 3);
        *(int32_t*)(renderer + 5) = (int32_t)(Rnd() % 100);
        *(int32_t*)(obj + 0x65) = *(int32_t*)(renderer + 5) - (int32_t)(Rnd() % 5) + 1;
        obj[0x20] = (uint8_t)(Rnd() % 4 == 0 ? 0x10 : 0);
        obj[0x5c] = (uint8_t)Rnd();
        g_slot14Result = Rnd() % 5 == 0;
        *(uint8_t**)(obj + 0x14) = Rnd() % 4 ? model : nullptr;
        *(int32_t*)(obj + 0x21) = (int32_t)(Rnd() % 12);
        for (int j = 0; j < 0x400; j += 4) *(int32_t*)(parts + j) = (int32_t)(Rnd() % 100000);
        *(float*)(parts + 0x10) = RndF(0.0f, 5e6f);
        int32_t n = 1 + (int32_t)(Rnd() % 50);
        char a[0x200], b[0x200];
        memset(a, 0x5a, sizeof a);
        memset(b, 0x5a, sizeof b);
        int32_t ra = ((int32_t(__fastcall*)(uint8_t*, int, char*, int32_t, int32_t, int32_t, int32_t))0x530820)(obj, 0, a, n, 0, 0, 0);
        int32_t rb = SceneObj_DebugTooltip(obj, 0, b, n, 0, 0, 0);
        EXPECT_EQ(ra == rb && memcmp(a, b, sizeof a) == 0, true, "tooltip iteration %d: %d '%.50s' vs %d '%.50s'", it, ra, a, rb, b);
    }
}

namespace rt3 {
void __fastcall Text_ExportLine(int32_t st, int32_t section, int32_t item, int32_t sub, const char* text);
}

DIFFTEST(game_TextExportLine) {
    char pa[MAX_PATH], pb[MAX_PATH];
    GetTempPathA(MAX_PATH, pa);
    strcpy(pb, pa);
    strcat(pa, "rt3re_export_a.txt");
    strcat(pb, "rt3re_export_b.txt");
    int32_t& words = hook::Global<int32_t>(0x6cfca0);
    int32_t sWords = words;
    static const char* pieces[] = { "Hello", " ", "world", "\r\n", "\n", "\r", "x", "  ", "Rail road", "\t", "a\"b" };
    int32_t wa = 0, wb = 0;
    for (int pass = 0; pass < 2; ++pass) {
        uint32_t seed = 12345;
        Stream st = StreamOpen(pass ? pb : pa, 0, 0, 0);
        words = 0;
        for (int it = 0; it < 400; ++it) {
            seed = seed * 1103515245 + 12345;
            std::string text;
            for (int k = (int)((seed >> 16) % 12); k > 0; --k) {
                seed = seed * 1103515245 + 12345;
                text += pieces[(seed >> 16) % 11];
            }
            int32_t section = (int32_t)((seed >> 8) % 40), item = (int32_t)((seed >> 4) % 3000), sub = (int32_t)(seed % 10);
            if (pass == 0) ((void(__fastcall*)(int32_t, int32_t, int32_t, int32_t, const char*))0x4883f0)(st, section, item, sub, text.c_str());
            else Text_ExportLine(st, section, item, sub, text.c_str());
        }
        StreamClose(st);
        (pass ? wb : wa) = words;
    }
    words = sWords;
    EXPECT_EQ(wa, wb, "word count %d vs %d", wa, wb);
    FILE* fa = fopen(pa, "rb");
    FILE* fb = fopen(pb, "rb");
    std::string ca, cb;
    int c;
    while (fa && (c = fgetc(fa)) != EOF) ca += (char)c;
    while (fb && (c = fgetc(fb)) != EOF) cb += (char)c;
    if (fa) fclose(fa);
    if (fb) fclose(fb);
    EXPECT_EQ(ca.size() > 1000 && ca == cb, true, "files differ (%u vs %u bytes)", (unsigned)ca.size(), (unsigned)cb.size());
    DeleteFileA(pa);
    DeleteFileA(pb);
}

namespace rt3 {
void Cities_PayConnectionBonuses();
void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai,
                            int32_t clearHistory);
}

namespace {
uint8_t g_bstate[4][0x100];
void* __fastcall FakeBuildingState(uint8_t* b) { return g_bstate[b[0x10] & 3]; }
} // namespace

DIFFTEST(game_CitiesPayConnectionBonuses) {
    const int NC = 3, NS = 4;
    static void* bvtbl[0x30];
    bvtbl[0x80 / 4] = (void*)&FakeBuildingState;
    static Collection cities, stations, buildings, nodes, companies, logs[2];
    static uint32_t routesMem[0x80];
    Collection& routes = *(Collection*)routesMem;
    struct Tab {
        Collection* c;
        uint32_t addr, size;
        int count;
    } tabs[] = { { &cities, 0x62bae0, 0x400, NC }, { &stations, 0x6cec20, 0x40, NS }, { &buildings, 0x62b26c, 0x400, NS },
                 { &nodes, 0x6cfca8, 0x260, NS }, { &routes, 0x6cfcb4, 0x40, NS }, { &companies, 0x62be10, 0x2000, 2 } };
    uint32_t saved[8];
    static uint8_t blank[0x2000];
    for (int k = 0; k < 6; ++k) {
        saved[k] = hook::Global<uint32_t>(tabs[k].addr);
        memset(tabs[k].c, 0, k == 4 ? sizeof routesMem : sizeof(Collection));
        Collection_Construct(tabs[k].c, 0, 1, tabs[k].size, 8, 8, 0, 0, 0);
        for (int j = 0; j < tabs[k].count; ++j) Collection_Allocate(tabs[k].c, 0, blank, tabs[k].size, 0, 0);
        hook::Global<Collection*>(tabs[k].addr) = tabs[k].c;
    }
    for (Collection& l : logs) {
        memset(&l, 0, sizeof l);
        Collection_Construct(&l, 0, 1, 0x21c, 0x40, 0x10, 0, 0, 0);
    }
    saved[6] = hook::Global<uint32_t>(0x62c120);
    saved[7] = hook::Global<uint32_t>(0x6acd34);
    static uint8_t map[0x2200], regions[NC + 1][0x1d7], keys[NC][NS][6];
    hook::Global<uint8_t*>(0x62c120) = map;
    *(uint8_t**)(map + 0x2129) = regions[NC];
    *(int32_t*)(map + 0x2145) = 1;
    static uint8_t cityInit[NC][0x400], cityA[NC][0x400], compA[2][0x2000];
    int paid = 0;
    for (int it = 0; it < 3000; ++it) {
        // Region r (row r) sits r records below the base; each lists some station keys.
        for (int r = 0; r < NC; ++r) {
            uint8_t* reg = regions[NC] - r * 0x1d7;
            int n = 0;
            for (int s = 1; s <= NS; ++s)
                if (Rnd() % 3 == 0) *(uint16_t*)keys[r][n++] = (uint16_t)s;
            *(int32_t*)(reg + 0xeb) = n;
            *(uint8_t**)(reg + 0xef) = keys[r][0];
        }
        for (int s = 1; s <= NS; ++s) {
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, s);
            *(int32_t*)st = s;
            *(uint32_t*)(st + 4) = (uint32_t)s;
            *(uint32_t*)(st + 8) = (uint32_t)s;
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, s);
            *(void***)b = bvtbl;
            b[0x10] = (uint8_t)(Rnd() % 4);
            b[0x3d4] = (uint8_t)(Rnd() % 3 ? 1 : 0);
            *(int32_t*)(b + 0x276) = 1 + (int32_t)(Rnd() % 2);
            *(int32_t*)((uint8_t*)Collection_Get(&nodes, 0, s) + 0x212) = s;
            *(int32_t*)((uint8_t*)Collection_Get(&routes, 0, s) + 0xc) = (int32_t)(Rnd() % 2);
        }
        for (auto& bs : g_bstate) bs[0x8c] = (uint8_t)(Rnd() % 4);
        for (int k = 0; k < NC; ++k) {
            uint8_t* c = cityInit[k];
            for (int j = 0; j < 0x400; ++j) c[j] = (uint8_t)Rnd();
            *(float*)(c + 0x1ea) = -368.0f * k - 1.0f;
            *(float*)(c + 0x1e2) = -1.0f;
            *(int32_t*)(c + 0x276) = Rnd() % 4 ? (int32_t)(Rnd() % 2000000) : 0;
            *(int32_t*)(c + 0x23e) = Rnd() % 3 == 0;
            c[0x316] = (uint8_t)(Rnd() % 3 == 0);
            sprintf((char*)c + 0x356, "City%d", k);
        }
        for (int k = 1; k <= 2; ++k) {
            uint8_t* co = (uint8_t*)Collection_Get(&companies, 0, k);
            memset(co, 0, 0x2000);
            sprintf((char*)co + 4, "Company%d", k);
        }
        int32_t serial = hook::Global<int32_t>(0x6acd30);
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < NC; ++k) memcpy(Collection_Get(&cities, 0, k + 1), cityInit[k], 0x400);
            for (int k = 1; k <= 2; ++k) {
                uint8_t* co = (uint8_t*)Collection_Get(&companies, 0, k);
                memset(co + 0x100, 0, 0x1f00);
            }
            hook::Global<int32_t>(0x6acd30) = serial;
            hook::Global<Collection*>(0x6acd34) = &logs[side];
            if (side == 0) ((void(*)())0x4358d0)();
            else Cities_PayConnectionBonuses();
            X87Clear();
            if (side == 0) {
                for (int k = 0; k < NC; ++k) memcpy(cityA[k], Collection_Get(&cities, 0, k + 1), 0x400);
                for (int k = 0; k < 2; ++k) memcpy(compA[k], Collection_Get(&companies, 0, k + 1), 0x2000);
            }
        }
        for (int k = 0; k < NC; ++k) {
            EXPECT_EQ(FirstDiff(cityA[k], (uint8_t*)Collection_Get(&cities, 0, k + 1), 0x400), -1, "city %d iteration %d", k, it);
            paid += *(int32_t*)(cityInit[k] + 0x276) && !*(int32_t*)(cityA[k] + 0x276);
        }
        for (int k = 0; k < 2; ++k) EXPECT_EQ(FirstDiff(compA[k], (uint8_t*)Collection_Get(&companies, 0, k + 1), 0x2000), -1, "company %d iteration %d", k, it);
        EXPECT_EQ(Collection_Count(&logs[0]), Collection_Count(&logs[1]), "log count iteration %d", it);
        for (int i = 0; i < Collection_Count(&logs[0]) && i < Collection_Count(&logs[1]); ++i) {
            uint8_t* ea = (uint8_t*)Collection_Get(&logs[0], 0, Collection_IdAt(&logs[0], 0, i, 0));
            uint8_t* eb = (uint8_t*)Collection_Get(&logs[1], 0, Collection_IdAt(&logs[1], 0, i, 0));
            bool same = !memcmp(ea, eb, 0xd) && !strcmp((char*)ea + 0xd, (char*)eb + 0xd) && !memcmp(ea + 0x20e, eb + 0x20e, 0xc);
            EXPECT_EQ(same, true, "log entry %d iteration %d: '%.60s' vs '%.60s'", i, it, (char*)ea + 0xd, (char*)eb + 0xd);
        }
    }
    for (int k = 0; k < 6; ++k) {
        hook::Global<uint32_t>(tabs[k].addr) = saved[k];
        Collection_Destruct(tabs[k].c);
    }
    hook::Global<uint32_t>(0x62c120) = saved[6];
    hook::Global<uint32_t>(0x6acd34) = saved[7];
    for (Collection& l : logs) Collection_Destruct(&l);
    EXPECT_EQ(paid > 300, true, "coverage: %d bonuses paid", paid);
}

DIFFTEST(game_PlayerInit) {
    static Collection players;
    memset(&players, 0, sizeof players);
    Collection_Construct(&players, 0, 1, 0x100, 8, 8, 0, 0, 0);
    static uint8_t blank[0x100];
    for (int k = 0; k < 3; ++k) Collection_Allocate(&players, 0, blank, 0x100, 0, 0);
    Collection*& gplayers = hook::Global<Collection*>(0x6ceb9c);
    Collection* sPlayers = gplayers;
    gplayers = &players;
    static uint8_t opts[0x400];
    uint8_t*& gopts = hook::Global<uint8_t*>(0x6cec74);
    uint8_t* sOpts = gopts;
    gopts = opts;
    static uint8_t slotsSaved[41 * 0x27], slotsInit[41 * 0x27];
    memcpy(slotsSaved, (void*)0x5f2d28, sizeof slotsSaved);
    int32_t sSeed = hook::Global<int32_t>(0x62431c);
    static uint8_t pa[0xd00], pb[0xd00], init[0xd00], slotsA[41 * 0x27];
    for (int it = 0; it < 2000; ++it) {
        for (int k = 1; k <= 3; ++k) *(int32_t*)((uint8_t*)Collection_Get(&players, 0, k) + 4) = Rnd() % 2;
        *(int32_t*)(opts + 0x2ab) = (int32_t)(Rnd() % 2);
        for (uint8_t& b : init) b = (uint8_t)Rnd();
        for (int k = 0; k < 41; ++k) slotsInit[k * 0x27] = (uint8_t)(Rnd() % 3 != 0);
        int32_t id = (int32_t)(Rnd() % 4), kind = (int32_t)(Rnd() % 4), ai = (int32_t)(Rnd() % 2), clear = (int32_t)(Rnd() % 2);
        static const char* names[] = { "", "Cornelius Vanderbilt", "A very long player name that runs past thirty chars" };
        static const char* descs[] = { "", "Some description." };
        const char* name = names[Rnd() % 3];
        const char* desc = descs[Rnd() % 2];
        int32_t seed = (int32_t)Rnd();
        int32_t seedA = 0;
        for (int side = 0; side < 2; ++side) {
            uint8_t* p = side ? pb : pa;
            memcpy(p, init, sizeof init);
            memcpy((void*)0x5f2d28, slotsInit, sizeof slotsInit);
            hook::Global<int32_t>(0x62431c) = seed;
            if (side == 0) ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, const char*, const char*, int32_t, int32_t))0x476140)(p, 0, id, kind, name, desc, ai, clear);
            else Player_Init(p, 0, id, kind, name, desc, ai, clear);
            if (side == 0) {
                seedA = hook::Global<int32_t>(0x62431c);
                memcpy(slotsA, (void*)0x5f2d28, sizeof slotsA);
            }
        }
        EXPECT_EQ(FirstDiff(pa, pb, sizeof pa), -1, "player iteration %d", it);
        EXPECT_EQ(seedA == hook::Global<int32_t>(0x62431c) && !memcmp(slotsA, (void*)0x5f2d28, sizeof slotsA), true, "rng/slots iteration %d", it);
    }
    memcpy((void*)0x5f2d28, slotsSaved, sizeof slotsSaved);
    hook::Global<int32_t>(0x62431c) = sSeed;
    gplayers = sPlayers;
    gopts = sOpts;
    Collection_Destruct(&players);
}

namespace rt3 {
int32_t __fastcall Map_TidyWater(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Train_Init(uint8_t* t, int, int32_t id, int32_t owner);
}

DIFFTEST(game_MapTidyWater) {
    auto orig = (int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, int32_t))0x44e500;
    const int32_t N = 24;
    static uint8_t map[0x2200], options[0xb00], queue[0x100];
    static uint8_t recs[2][N * N * 3], g1661[2][N * N], g1665[2][N * N], shade[2][(N + 1) * (N + 1)];
    *(int32_t*)(map + 0x2155) = N;
    *(int32_t*)(map + 0x2159) = N;
    *(int32_t*)(map + 0x15dd) = N;
    *(int32_t*)(map + 0x15d9) = N;
    *(int32_t*)(map + 0x15e1) = 0;
    uint8_t*& gopt = hook::Global<uint8_t*>(0x6d4018);
    uint8_t*& gq = hook::Global<uint8_t*>(0x6d1304);
    uint8_t *sOpt = gopt, *sQ = gq;
    gopt = options;
    int changed = 0;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 0; k < N * N; ++k) {
            recs[0][k * 3] = (uint8_t)(Rnd() % 8);
            recs[0][k * 3 + 1] = (uint8_t)Rnd();
            recs[0][k * 3 + 2] = (uint8_t)Rnd();
            g1661[0][k] = (uint8_t)Rnd();
            g1665[0][k] = (uint8_t)Rnd();
        }
        for (uint8_t& s : shade[0]) s = (uint8_t)Rnd();
        for (int b = (int)(Rnd() % 4); b >= 0; --b) {
            int cx = (int)(Rnd() % N), cy = (int)(Rnd() % N), r = (int)(Rnd() % 4);
            for (int y = 0; y < N; ++y)
                for (int x = 0; x < N; ++x)
                    if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r)
                        recs[0][(y * N + x) * 3] = (uint8_t)((recs[0][(y * N + x) * 3] & 0xf8) | (Rnd() % 4 ? 2 : 4));
        }
        memcpy(recs[1], recs[0], sizeof recs[0]);
        memcpy(g1661[1], g1661[0], sizeof g1661[0]);
        memcpy(g1665[1], g1665[0], sizeof g1665[0]);
        memcpy(shade[1], shade[0], sizeof shade[0]);
        *(int32_t*)(options + 0xa8c) = (int32_t)Rnd();
        int32_t x0 = (int32_t)(Rnd() % (N + 4)) - 2, y0 = (int32_t)(Rnd() % (N + 4)) - 2;
        int32_t x1 = x0 + (int32_t)(Rnd() % 16) - 2, y1 = y0 + (int32_t)(Rnd() % 16) - 2;
        bool hasQ = Rnd() % 2 != 0;
        int32_t qx = Rnd() % 3 ? (int32_t)(Rnd() % N) : -1, qy = (int32_t)(Rnd() % N);
        int32_t r[2];
        uint8_t qOut[2][8];
        for (int side = 0; side < 2; ++side) {
            *(uint8_t**)(map + 0x165d) = recs[side];
            *(uint8_t**)(map + 0x1661) = g1661[side];
            *(uint8_t**)(map + 0x1665) = g1665[side];
            *(uint8_t**)(map + 0x162d) = shade[side];
            *(int32_t*)(queue + 0x78) = qx;
            *(int32_t*)(queue + 0x7c) = qy;
            gq = hasQ ? queue : nullptr;
            r[side] = side ? Map_TidyWater(map, 0, x0, y0, x1, y1) : orig(map, 0, x0, y0, x1, y1);
            memcpy(qOut[side], queue + 0x78, 8);
        }
        changed += memcmp(recs[0], recs[1], 0) || r[0];
        EXPECT_EQ(r[0], r[1], "result iteration %d", it);
        EXPECT_EQ(memcmp(qOut[0], qOut[1], 8), 0, "queue iteration %d", it);
        EXPECT_EQ(FirstDiff(recs[0], recs[1], sizeof recs[0]), -1, "records iteration %d", it);
        EXPECT_EQ(memcmp(g1661[0], g1661[1], sizeof g1661[0]) == 0 && memcmp(g1665[0], g1665[1], sizeof g1665[0]) == 0 &&
                      memcmp(shade[0], shade[1], sizeof shade[0]) == 0,
                  true, "grids iteration %d", it);
    }
    gopt = sOpt;
    gq = sQ;
}

DIFFTEST(game_TrainInit) {
    static uint8_t world[0x100], ta[0x200], tb[0x200];
    uint8_t*& gworld = hook::Global<uint8_t*>(0x6cec78);
    uint8_t* sWorld = gworld;
    gworld = world;
    for (int it = 0; it < 2000; ++it) {
        *(uint16_t*)(world + 0xd) = (uint16_t)Rnd();
        for (uint8_t& b : ta) b = (uint8_t)Rnd();
        memcpy(tb, ta, sizeof ta);
        int32_t id = Rnd() % 4 == 0 ? (int32_t)Rnd() : (int32_t)(Rnd() % 500), owner = (int32_t)(Rnd() % 8);
        ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4a7410)(ta, 0, id, owner);
        Train_Init(tb, 0, id, owner);
        EXPECT_EQ(FirstDiff(ta, tb, sizeof ta), -1, "train iteration %d", it);
    }
    gworld = sWorld;
}

namespace rt3 {
void __fastcall Engine_EnumDisplayModes(uint8_t* e);
}

namespace {
// A fake IDirect3D8 with a scripted adapter mode table.
struct FakeD3D {
    void** vtbl;
};
uint32_t g_modes[24][4], g_desktop[4], g_checkSeed;
int32_t g_modeCount;
uint32_t __stdcall FakeD3DRelease(FakeD3D*) { return 0; }
uint32_t __stdcall FakeD3DModeCount(FakeD3D*, uint32_t) { return (uint32_t)g_modeCount; }
long __stdcall FakeD3DEnumModes(FakeD3D*, uint32_t, uint32_t i, uint32_t* m) {
    memcpy(m, g_modes[i], 16);
    return 0;
}
long __stdcall FakeD3DDisplayMode(FakeD3D*, uint32_t, uint32_t* m) {
    memcpy(m, g_desktop, 16);
    return 0;
}
long __stdcall FakeD3DCheckType(FakeD3D*, uint32_t, uint32_t, uint32_t fmt, uint32_t, int32_t) {
    return ((fmt * 2654435761u) ^ g_checkSeed) & 1 ? 0 : (long)0x8876086a;
}
void* g_d3dVtbl[0x40];
FakeD3D g_d3d = { g_d3dVtbl };
void* __stdcall FakeDirect3DCreate8(uint32_t) { return &g_d3d; }
} // namespace

DIFFTEST(engine_EnumDisplayModes) {
    g_d3dVtbl[0x8 / 4] = (void*)&FakeD3DRelease;
    g_d3dVtbl[0x18 / 4] = (void*)&FakeD3DModeCount;
    g_d3dVtbl[0x1c / 4] = (void*)&FakeD3DEnumModes;
    g_d3dVtbl[0x20 / 4] = (void*)&FakeD3DDisplayMode;
    g_d3dVtbl[0x2c / 4] = (void*)&FakeD3DCheckType;
    void** iat = (void**)0x5c8398;
    DWORD oldProt;
    VirtualProtect(iat, 4, PAGE_READWRITE, &oldProt);
    void* sCreate = *iat;
    *iat = (void*)&FakeDirect3DCreate8;
    static const uint32_t widths[] = { 320, 640, 800, 1024, 1280, 1600, 480, 600 };
    static const uint32_t heights[] = { 240, 480, 600, 768, 1024, 1200, 640, 400 };
    static const uint32_t okDesktop[] = { 0x14, 0x16, 0x17, 0x18 };
    static uint8_t ea[0x100], eb[0x100];
    // The 100 Hz cap (JA at 0x51f71b) may be NOPped by DesyncWatch; test both forms.
    uint8_t* cap = (uint8_t*)0x51f71b;
    uint8_t capSaved[6];
    static const uint8_t capOrig[6] = { 0x0f, 0x87, 0xa2, 0x00, 0x00, 0x00 }, capNops[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    DWORD capProt;
    VirtualProtect(cap, 6, PAGE_EXECUTE_READWRITE, &capProt);
    memcpy(capSaved, cap, 6);
    for (int it = 0; it < 2000; ++it) {
        memcpy(cap, it % 2 ? capOrig : capNops, 6);
        FlushInstructionCache(GetCurrentProcess(), cap, 6);
        g_modeCount = 1 + (int32_t)(Rnd() % 24);
        for (int k = 0; k < g_modeCount; ++k) {
            int s = (int)(Rnd() % 8);
            g_modes[k][0] = widths[s];
            g_modes[k][1] = Rnd() % 6 ? heights[s] : heights[Rnd() % 8];
            g_modes[k][2] = Rnd() % 3 ? 60 + Rnd() % 30 : Rnd() % 200;
            g_modes[k][3] = 0x14 + Rnd() % 6;
        }
        // Ensure at least one usable landscape mode (otherwise the game exits).
        g_modes[0][0] = 1024;
        g_modes[0][1] = 768;
        g_modes[0][3] = 0x16 + Rnd() % 3;
        for (int k = 0; k < 3; ++k) g_desktop[k] = Rnd() % 2000;
        g_desktop[3] = okDesktop[Rnd() % 4];
        g_checkSeed = Rnd();
        for (uint8_t& b : ea) b = (uint8_t)Rnd();
        memcpy(eb, ea, sizeof ea);
        ((void(__fastcall*)(uint8_t*))0x51f540)(ea);
        Engine_EnumDisplayModes(eb);
        uint8_t* la = *(uint8_t**)(ea + 0x34);
        uint8_t* lb = *(uint8_t**)(eb + 0x34);
        int32_t n = *(int32_t*)(ea + 0x30);
        EXPECT_EQ(n, *(int32_t*)(eb + 0x30), "count iteration %d: %d vs %d (modes %d)", it, n, *(int32_t*)(eb + 0x30), g_modeCount);
        EXPECT_EQ(memcmp(la, lb, (g_modeCount + 5) * 0x15), 0, "list iteration %d", it);
        *(uint8_t**)(ea + 0x34) = *(uint8_t**)(eb + 0x34) = nullptr;
        EXPECT_EQ(FirstDiff(ea, eb, sizeof ea), -1, "engine iteration %d", it);
        ((void(__cdecl*)(void*))0x5a1145)(la);
        ((void(__cdecl*)(void*))0x5a1145)(lb);
    }
    memcpy(cap, capSaved, 6);
    FlushInstructionCache(GetCurrentProcess(), cap, 6);
    VirtualProtect(cap, 6, capProt, &capProt);
    *iat = sCreate;
    VirtualProtect(iat, 4, oldProt, &oldProt);
}

namespace rt3 {
const char* SysInfo_Report();
}

DIFFTEST(game_SysInfoReport) {
    static char saved[0x400], a[0x400];
    memcpy(saved, (void*)0x6cec88, sizeof saved);
    uint32_t sFeat = hook::Global<uint32_t>(0xd97433), sSig = hook::Global<uint32_t>(0xd9743b), sMem = hook::Global<uint32_t>(0xd97550);
    for (int it = 0; it < 64; ++it) {
        hook::Global<uint32_t>(0xd97433) = (uint32_t)it;
        hook::Global<uint32_t>(0xd9743b) = Rnd() % 4000;
        hook::Global<uint32_t>(0xd97550) = Rnd();
        memset((void*)0x6cec88, 0x5a, sizeof a);
        const char* ra = ((const char*(*)())0x4819f0)();
        memcpy(a, (void*)0x6cec88, sizeof a);
        memset((void*)0x6cec88, 0x5a, sizeof a);
        const char* rb = SysInfo_Report();
        EXPECT_EQ(ra == rb && memcmp(a, (void*)0x6cec88, sizeof a) == 0, true, "report iteration %d: '%.80s'", it, a);
    }
    hook::Global<uint32_t>(0xd97433) = sFeat;
    hook::Global<uint32_t>(0xd9743b) = sSig;
    hook::Global<uint32_t>(0xd97550) = sMem;
    memcpy((void*)0x6cec88, saved, sizeof saved);
}

namespace rt3 {
int32_t __fastcall Nodes_FindAlongLine(Collection* nodes, int, float x0, float y0, float x1, float y1, int32_t, int32_t exclude,
                                       int32_t* found);
}

DIFFTEST(game_NodesFindAlongLine) {
    auto orig = (int32_t(__fastcall*)(Collection*, int, float, float, float, float, int32_t, int32_t, int32_t*))0x49c900;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    static uint32_t gmem[0x100];
    Collection& g = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&g, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 12, W = 16;
    for (int k = 0; k < N; ++k) Collection_Allocate(&g, 0, blank, 0x260, 0, 0);
    static uint8_t map[0x2200];
    *(int32_t*)(map + 0x15d9) = W;
    *(int32_t*)(map + 0x15d5) = W;
    *(int32_t*)(map + 0x2155) = W;
    *(int32_t*)(map + 0x2159) = W;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sMap = gmap;
    gmap = map;
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &g;
    static uint8_t* columns[W];
    static uint8_t entries[W][W][10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    int found = 0;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&g, 0, k);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, W * 92.0f);
            *(int32_t*)(n + 0x202) = k;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = Rnd() % 3 == 0 ? -1 : 1 + (int32_t)(Rnd() % N);
        }
        // Each column: a sorted chain of cell entries, each listing a few live nodes.
        for (int x = 0; x < W; ++x) {
            columns[x] = nullptr;
            uint8_t* prev = nullptr;
            for (int y = 0; y < W; ++y) {
                if (Rnd() % 3) continue;
                uint8_t* e = entries[x][y];
                *(uint16_t*)e = (uint16_t)y;
                uint32_t* head = nullptr;
                for (int c = (int)(Rnd() % 3); c > 0; --c) {
                    uint32_t* l = (uint32_t*)xmalloc(8);
                    l[0] = 1 + Rnd() % N;
                    l[1] = (uint32_t)head;
                    head = l;
                }
                *(uint32_t**)(e + 2) = head;
                *(uint8_t**)(e + 6) = nullptr;
                if (prev) *(uint8_t**)(prev + 6) = e;
                else columns[x] = e;
                prev = e;
            }
        }
        for (int k = 1; k <= N; ++k) ((uint8_t*)Collection_Get(&g, 0, k))[0x216] = (uint8_t)(Rnd() % 3 != 0);
        float x0 = RndF(0.0f, (float)W), y0 = RndF(0.0f, (float)W), x1 = RndF(0.0f, (float)W), y1 = RndF(0.0f, (float)W);
        if (Rnd() % 8 == 0) x1 = x0;
        if (Rnd() % 8 == 0) y1 = y0;
        int32_t exclude = Rnd() % 3 ? -1 : 1 + (int32_t)(Rnd() % N);
        int32_t fa = 7, fb = 7;
        int32_t ra = orig(&g, 0, x0, y0, x1, y1, 0, exclude, &fa);
        int32_t rb = Nodes_FindAlongLine(&g, 0, x0, y0, x1, y1, 0, exclude, &fb);
        found += ra != 0;
        EXPECT_EQ(ra == rb && fa == fb, true, "result iteration %d: %d/%d vs %d/%d", it, ra, fa, rb, fb);
        for (int x = 0; x < W; ++x)
            for (uint8_t* e = columns[x]; e; e = *(uint8_t**)(e + 6))
                for (uint32_t* l = *(uint32_t**)(e + 2); l;) {
                    uint32_t* next = (uint32_t*)l[1];
                    xfree(l);
                    l = next;
                }
    }
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    gmap = sMap;
    gnodes = sNodes;
    Collection_Destruct(&g);
    EXPECT_EQ(found > 100, true, "coverage: %d found", found);
}


namespace rt3 {
void __fastcall Pack_Shutdown(uint8_t* t);
}

DIFFTEST(game_PackShutdown) {
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    void** iat = (void**)0x5c8078;
    DWORD oldProt;
    VirtualProtect(iat, 4, PAGE_READWRITE, &oldProt);
    void* sOut = *iat;
    *iat = (void*)&FakeOutputDebugString;
    int32_t sDebug = hook::Global<int32_t>(0x6d3ca0);
    hook::Global<int32_t>(0x6d3ca0) = 0;
    void* sGlobal = hook::Global<void*>(0xccba50);
    uint8_t* packs[2];
    for (uint8_t*& p : packs) p = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    for (int it = 0; it < 300; ++it) {
        // Item table: some leaked, ids found through the per-pack bucket arrays.
        int32_t items = (int32_t)(Rnd() % 8), nPacks = (int32_t)(Rnd() % 3);
        uint32_t ids[8], uses[8], ptrs[8];
        uint8_t types[8];
        for (int k = 0; k < 8; ++k) {
            ids[k] = Rnd() % 64;
            uses[k] = Rnd() % 3 ? 0 : Rnd() % 5 + (Rnd() % 4 == 0 ? 0x80000000u : 0);
            ptrs[k] = Rnd();
            types[k] = (uint8_t)Rnd();
        }
        uses[0] = 1; // the first leaked item is always found (see Pack_Shutdown)
        ids[0] = 5;
        int32_t cnt[2][0x20];
        for (int pk = 0; pk < 2; ++pk)
            for (int b = 0; b < 0x20; ++b) cnt[pk][b] = (b == 5 && pk == 0) ? 2 : (Rnd() % 4 == 0 ? 1 : 0);
        std::string outA;
        for (int side = 0; side < 2; ++side) {
            uint8_t* p = packs[side];
            memset(p, 0, 0x43000);
            for (int k = 0; k < 10; ++k) *(int32_t*)(p + 0x4290c + k * 4) = -1;
            uint8_t* table = (uint8_t*)xmalloc(8 * 0xd + 1);
            for (int k = 0; k < items; ++k) {
                *(uint32_t*)(table + k * 0xd) = uses[k];
                *(uint32_t*)(table + k * 0xd + 4) = ids[k];
                table[k * 0xd + 8] = types[k];
                *(uint32_t*)(table + k * 0xd + 9) = ptrs[k];
            }
            *(uint8_t**)(p + 0x1f400) = table;
            *(int32_t*)(p + 0x427b4) = items ? items : 1;
            if (!items) memset(table, 0, 0xd);
            *(int32_t*)(p + 0x427b8) = nPacks > 0 ? nPacks : 1;
            for (int pk = 0; pk < 2; ++pk) {
                *(int32_t*)(p + 0x204ac + pk * 4) = -1;
                for (int b = 0; b < 0x20; ++b) {
                    uint8_t* slot = p + b * 4 + pk * 0x80;
                    int32_t n = cnt[pk][b];
                    *(int32_t*)(slot + 0x223ec) = n;
                    if (!n) continue;
                    uint8_t* arr = (uint8_t*)xmalloc(n * 0xd);
                    for (int j = 0; j < n; ++j) *(int32_t*)(arr + j * 0xd) = b + 32 * j;
                    *(uint8_t**)slot = arr;
                }
            }
            hook::Global<void*>(0xccba50) = nullptr;
            g_dbgOut.clear();
            if (side == 0) ((void(__fastcall*)(uint8_t*))0x53b9e0)(p);
            else Pack_Shutdown(p);
            // Free whatever the shutdown left (packs beyond the count).
            for (int pk = 0; pk < 2; ++pk)
                for (int b = 0; b < 0x20; ++b)
                    if (void* a = *(void**)(p + b * 4 + pk * 0x80)) {
                        ((void(__cdecl*)(void*))0x5a1145)(a);
                        *(void**)(p + b * 4 + pk * 0x80) = nullptr;
                    }
            if (side == 0) outA = g_dbgOut;
        }
        EXPECT_EQ(outA == g_dbgOut, true, "debug output iteration %d: '%.120s' vs '%.120s'", it, outA.c_str(), g_dbgOut.c_str());
        *(uint8_t**)(packs[0] + 0x1f400) = *(uint8_t**)(packs[1] + 0x1f400) = nullptr;
        EXPECT_EQ(memcmp(packs[0], packs[1], 0x43000), 0, "pack state iteration %d", it);
    }
    for (uint8_t* p : packs) VirtualFree(p, 0, MEM_RELEASE);
    hook::Global<void*>(0xccba50) = sGlobal;
    hook::Global<int32_t>(0x6d3ca0) = sDebug;
    *iat = sOut;
    VirtualProtect(iat, 4, oldProt, &oldProt);
}

namespace rt3 {
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
}

namespace {
int32_t g_childResult[2], g_baseCalls;
int32_t __fastcall FakeChildEvent(uint8_t* c, int, uint32_t*) { return g_childResult[c[0x10] & 1]; }
int32_t __fastcall FakeWindowBase(uint8_t* w, int, uint32_t* ev) {
    ++g_baseCalls;
    return 0x50 + (int32_t)(ev[0] & 0xf);
}
} // namespace

DIFFTEST(game_WindowHandleEvent) {
    static void* childVtbl[4] = { nullptr, (void*)&FakeChildEvent };
    static void* winVtbl[8] = { nullptr, (void*)&FakeWindowBase };
    static uint8_t wins[2][0x200], kids[2][2][0x100], queues[2][0xb00], hit[2][0x100];
    uint8_t*& gq = hook::Global<uint8_t*>(0x6d4018);
    void*& gfilter = hook::Global<void*>(0x6d4008);
    void*& gidle = hook::Global<void*>(0x6d4004);
    uint8_t* sQ = gq;
    void *sFilter = gfilter, *sIdle = gidle;
    gfilter = nullptr;
    gidle = nullptr;
    int dragged = 0;
    for (int it = 0; it < 3000; ++it) {
        uint8_t winInit[0x200], hitInit[0x100];
        for (uint8_t& b : winInit) b = (uint8_t)Rnd();
        for (uint8_t& b : hitInit) b = (uint8_t)Rnd();
        *(void***)winInit = winVtbl;
        *(float*)(winInit + 5) = RndF(0.0f, 0.5f);
        *(float*)(winInit + 9) = RndF(0.0f, 0.5f);
        *(float*)(winInit + 0xd) = RndF(0.1f, 0.5f);
        *(float*)(winInit + 0x11) = RndF(0.1f, 0.5f);
        *(int32_t*)(winInit + 0x60) = Rnd() % 4 != 0;
        *(int32_t*)(winInit + 0x15) = Rnd() % 4 != 0;
        int nKids = (int)(Rnd() % 3);
        g_childResult[0] = 1;
        g_childResult[1] = Rnd() % 6 != 0;
        *(uint16_t*)(hitInit + 6) = (uint16_t)(Rnd() % 2 ? 4 : 3);
        *(uint32_t*)(hitInit + 0x33) = Rnd() & 0x200800;
        bool hitOwned = Rnd() % 3 != 0, hasHit = Rnd() % 6 != 0;
        uint32_t evInit[12];
        for (uint32_t& v : evInit) v = Rnd();
        evInit[0] = Rnd() % 3 ? 6 : 2;
        bool inside = Rnd() % 4 != 0;
        *(float*)&evInit[9] = inside ? *(float*)(winInit + 5) + RndF(0.01f, 0.99f) * *(float*)(winInit + 0xd) : RndF(0.0f, 1.0f);
        *(float*)&evInit[10] = inside ? *(float*)(winInit + 9) + RndF(0.01f, 0.99f) * *(float*)(winInit + 0x11) : RndF(0.0f, 1.0f);
        // Queued events during a drag: some moves (and other events), then a release.
        uint8_t qInit[0xb00] = {};
        int nEv = 1 + (int)(Rnd() % 6);
        for (int k = 0; k < nEv; ++k) {
            uint32_t* e = (uint32_t*)(qInit + 0x128 + k * 0x30);
            for (int j = 0; j < 12; ++j) e[j] = Rnd();
            e[0] = k == nEv - 1 ? 7 : (Rnd() % 4 ? 2 : 3);
            *(float*)&e[9] = RndF(-0.3f, 1.3f);
            *(float*)&e[10] = RndF(-0.3f, 1.3f);
        }
        *(int32_t*)(qInit + 0xa88) = nEv;
        *(int32_t*)(qInit + 0x100) = (int32_t)(Rnd() % 2);
        int32_t r[2], base[2];
        uint32_t evOut[2][12];
        for (int side = 0; side < 2; ++side) {
            uint8_t* w = wins[side];
            memcpy(w, winInit, sizeof winInit);
            memcpy(hit[side], hitInit, sizeof hitInit);
            *(uint8_t**)(hit[side] + 0x86) = hitOwned ? w : nullptr;
            *(uint8_t**)(w + 0x74) = nKids ? kids[side][0] : nullptr;
            for (int k = 0; k < 2; ++k) {
                memset(kids[side][k], 0, 0x100);
                *(void***)kids[side][k] = childVtbl;
                kids[side][k][0x10] = (uint8_t)k;
                *(uint8_t**)(kids[side][k] + 0x8a) = k + 1 < nKids ? kids[side][k + 1] : nullptr;
            }
            memcpy(queues[side], qInit, sizeof qInit);
            gq = queues[side];
            uint32_t* ev = evOut[side];
            memcpy(ev, evInit, sizeof evInit);
            ev[0xb] = hasHit ? (uint32_t)(uintptr_t)hit[side] : 0;
            g_baseCalls = 0;
            r[side] = side ? Window_HandleEvent(w, 0, ev) : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x53fe90)(w, 0, ev);
            base[side] = g_baseCalls;
            X87Clear();
        }
        dragged += *(int32_t*)(queues[0] + 0xa88) != nEv;
        EXPECT_EQ(r[0] == r[1] && base[0] == base[1], true, "result iteration %d: %d vs %d", it, r[0], r[1]);
        evOut[0][0xb] = evOut[1][0xb] = 0;
        EXPECT_EQ(memcmp(evOut[0], evOut[1], sizeof evOut[0]), 0, "event iteration %d", it);
        *(void**)(wins[0] + 0x74) = *(void**)(wins[1] + 0x74) = nullptr;
        EXPECT_EQ(FirstDiff(wins[0], wins[1], 0x200), -1, "window iteration %d", it);
        EXPECT_EQ(memcmp(queues[0], queues[1], 0xb00), 0, "queue iteration %d", it);
    }
    gq = sQ;
    gfilter = sFilter;
    gidle = sIdle;
    EXPECT_EQ(dragged > 200, true, "coverage: %d drags", dragged);
}

namespace rt3 {
int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle,
                                       float* outX, float* outY, float* outAngle, int32_t update, int32_t* outNode);
}

DIFFTEST(game_StationSnapToTrack) {
    auto orig = (int32_t(__fastcall*)(Collection*, int, int32_t, uint32_t, float, float, float, float*, float*, float*, int32_t, int32_t*))0x417b40;
    FakeEngine fe;
    g_seed = 0x5a7e; // fixed: the snap coverage floor below depends on the draws
    for (int d = 0; d < 360; ++d) {
        *(float*)(fe.e + 0x1136b1 + d * 4) = (float)sin(d * 3.14159265358979 / 180.0);
        *(float*)(fe.e + 0x113c51 + d * 4) = (float)cos(d * 3.14159265358979 / 180.0);
    }
    static Collection types, bigTypes;
    memset(&types, 0, sizeof types);
    memset(&bigTypes, 0, sizeof bigTypes);
    Collection_Construct(&types, 0, 1, 0x200, 8, 8, 0, 0, 0);
    Collection_Construct(&bigTypes, 0, 1, 0x40, 8, 8, 0, 0, 0);
    static uint8_t tblank[0x200];
    for (int k = 0; k < 3; ++k) {
        Collection_Allocate(&types, 0, tblank, 0x200, 0, 0);
        Collection_Allocate(&bigTypes, 0, tblank, 0x40, 0, 0);
    }
    Collection*& gbig = hook::Global<Collection*>(0x62b268);
    Collection* sBig = gbig;
    gbig = &bigTypes;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    static uint32_t gmem[0x100];
    Collection& g = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&g, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    const int N = 12, W = 16;
    for (int k = 0; k < N; ++k) Collection_Allocate(&g, 0, blank, 0x260, 0, 0);
    static uint8_t map[0x2200];
    *(int32_t*)(map + 0x15d9) = W;
    *(int32_t*)(map + 0x15d5) = W;
    *(int32_t*)(map + 0x2155) = W;
    *(int32_t*)(map + 0x2159) = W;
    uint8_t*& gmap = hook::Global<uint8_t*>(0x62c120);
    uint8_t* sMap = gmap;
    gmap = map;
    Collection*& gnodes = hook::Global<Collection*>(0x6cfca8);
    Collection* sNodes = gnodes;
    gnodes = &g;
    static uint8_t* columns[W];
    static uint8_t entries[W][W][10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    int found = 0;
    for (int it = 0; it < 2000; ++it) {
        for (int k = 1; k <= N; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&g, 0, k);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, W * 92.0f);
            *(int32_t*)(n + 0x202) = k;
            // A chain 1 - 2 - ... - N with occasional gaps, junctions and claims.
            *(int32_t*)(n + 0x206) = k > 1 && Rnd() % 8 ? k - 1 : -1;
            *(int32_t*)(n + 0x20a) = k < N && Rnd() % 8 ? k + 1 : -1;
            *(int32_t*)(n + 0x20e) = Rnd() % 6 == 0 ? 1 + (int32_t)(Rnd() % N) : -1;
            *(int32_t*)(n + 0x222) = Rnd() % 4 == 0 ? (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(n + 0x226) = Rnd() % 6 == 0;
            n[0x201] = (uint8_t)Rnd();
        }
        for (int k = 1; k <= 3; ++k) {
            uint8_t* ty = (uint8_t*)Collection_Get(&types, 0, k);
            *(uint32_t*)(ty + 0x173) = (uint32_t)k;
            *(uint16_t*)(ty + 0xcb) = (uint16_t)(Rnd() % 4);
            *(uint16_t*)(ty + 0xcd) = (uint16_t)(Rnd() % 4);
            ty[0xb8] = (uint8_t)(1 + Rnd() % 4);
            ty[0xb9] = (uint8_t)(1 + Rnd() % 4);
            ((uint8_t*)Collection_Get(&bigTypes, 0, k))[0x32] = 1; // cell-less types read stale stack in Type_RotatedCell
        }
        // Each column: a sorted chain of cell entries, each listing a few live nodes.
        for (int x = 0; x < W; ++x) {
            columns[x] = nullptr;
            uint8_t* prev = nullptr;
            for (int y = 0; y < W; ++y) {
                if (Rnd() % 3) continue;
                uint8_t* e = entries[x][y];
                *(uint16_t*)e = (uint16_t)y;
                uint32_t* head = nullptr;
                for (int c = (int)(Rnd() % 3); c > 0; --c) {
                    uint32_t* l = (uint32_t*)xmalloc(8);
                    l[0] = 1 + Rnd() % N;
                    l[1] = (uint32_t)head;
                    head = l;
                }
                *(uint32_t**)(e + 2) = head;
                *(uint8_t**)(e + 6) = nullptr;
                if (prev) *(uint8_t**)(prev + 6) = e;
                else columns[x] = e;
                prev = e;
            }
        }
        float px = RndF(0.0f, (float)W), py = RndF(0.0f, (float)W), angle = RndF(0.0f, 6.3f);
        int32_t company = (int32_t)(Rnd() % 3), update = (int32_t)(Rnd() % 2);
        uint32_t type = 1 + Rnd() % 3;
        float oa[3] = { -1, -1, -1 }, ob[3] = { -1, -1, -1 };
        int32_t na = 99, nb = 99;
        bool wantNode = Rnd() % 4 != 0;
        int32_t ra = orig(&types, 0, company, type, px, py, angle, &oa[0], &oa[1], &oa[2], update, wantNode ? &na : nullptr);
        int32_t rb = Station_SnapToTrack(&types, 0, company, type, px, py, angle, &ob[0], &ob[1], &ob[2], update, wantNode ? &nb : nullptr);
        EXPECT_EQ(na, nb, "node iteration %d", it);
        found += ra != 0;
        EXPECT_EQ(ra == rb && !memcmp(oa, ob, sizeof oa), true, "result iteration %d: %d vs %d (%.9g %.9g %.9g / %.9g %.9g %.9g) angle %.9g", it, ra, rb, oa[0], oa[1], oa[2], ob[0], ob[1], ob[2], angle);
        for (int x = 0; x < W; ++x)
            for (uint8_t* e = columns[x]; e; e = *(uint8_t**)(e + 6))
                for (uint32_t* l = *(uint32_t**)(e + 2); l;) {
                    uint32_t* next = (uint32_t*)l[1];
                    xfree(l);
                    l = next;
                }
    }
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    gbig = sBig;
    Collection_Destruct(&types);
    Collection_Destruct(&bigTypes);
    gmap = sMap;
    gnodes = sNodes;
    Collection_Destruct(&g);
    EXPECT_EQ(found > 30, true, "coverage: %d snapped", found);
}


namespace rt3 {
void __fastcall World_YearEvents(uint8_t* w);
}

DIFFTEST(game_WorldYearEvents) {
    static uint8_t worlds[2][0x47000];
    static Collection news[2], types, buildings;
    for (Collection& n : news) {
        memset(&n, 0, sizeof n);
        Collection_Construct(&n, 0, 1, 0x188, 0x20, 0x10, 0, 0, 0);
    }
    memset(&types, 0, sizeof types);
    memset(&buildings, 0, sizeof buildings);
    Collection_Construct(&types, 0, 1, 0x100, 8, 8, 0, 0, 0);
    Collection_Construct(&buildings, 0, 1, 0x400, 8, 8, 0, 0, 0);
    static uint8_t blank[0x400];
    static const char* names[4] = { "Tool And Die", "Weapons Factory", "Munitions Factory", "Toy Factory" };
    for (int k = 0; k < 4; ++k) {
        uint8_t* r = (uint8_t*)Collection_Get(&types, 0, Collection_Allocate(&types, 0, blank, 0x100, 0, 0));
        strcpy((char*)r + 4, names[k]);
    }
    for (int k = 0; k < 4; ++k) Collection_Allocate(&buildings, 0, blank, 0x400, 0, 0);
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6cec78 }, { 0x6cea4c }, { 0x62b268 }, { 0x62b26c }, { 0x6cea48 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<Collection*>(0x62b268) = &types;
    hook::Global<Collection*>(0x62b26c) = &buildings;
    static const int32_t years[] = { 1850, 1865, 1877, 1895, 1900, 1910, 1920, 1929, 1940, 1950, 1956, 1958, 1830, 1999 };
    for (int it = 0; it < 600; ++it) {
        int32_t year = years[Rnd() % 14];
        int32_t last = Rnd() % 6 == 0 ? year : year - 1;
        int32_t blocked = Rnd() % 8 == 0;
        for (int k = 1; k <= 4; ++k) *(int32_t*)((uint8_t*)Collection_Get(&buildings, 0, k) + 0x3d0) = (int32_t)(Rnd() % 6);
        int32_t serial = hook::Global<int32_t>(0x6cea48);
        for (int side = 0; side < 2; ++side) {
            uint8_t* w = worlds[side];
            *(uint16_t*)(w + 0xd) = (uint16_t)year;
            w[0xf] = 3;
            w[0x11] = 7;
            *(uint16_t*)(w + 5) = (uint16_t)last;
            *(int32_t*)(w + 0x46c38) = blocked;
            *(void**)(w + 0x66a6) = nullptr;
            hook::Global<uint8_t*>(0x6cec78) = w;
            hook::Global<Collection*>(0x6cea4c) = &news[side];
            hook::Global<int32_t>(0x6cea48) = serial;
            if (side == 0) ((void(__fastcall*)(uint8_t*))0x435b50)(w);
            else World_YearEvents(w);
        }
        // Named entries (list at +0x66a6: {next, name, a..f}) and news must match.
        int32_t* ea = *(int32_t**)(worlds[0] + 0x66a6);
        int32_t* eb = *(int32_t**)(worlds[1] + 0x66a6);
        for (; ea && eb; ea = *(int32_t**)ea, eb = *(int32_t**)eb)
            EXPECT_EQ(strcmp((char*)ea[1], (char*)eb[1]) == 0 && !memcmp(ea + 2, eb + 2, 24), true, "entry year %d iteration %d: '%.60s' vs '%.60s'",
                      year, it, (char*)ea[1], (char*)eb[1]);
        EXPECT_EQ(ea == nullptr && eb == nullptr, true, "entry count year %d iteration %d", year, it);
        EXPECT_EQ(Collection_Count(&news[0]), Collection_Count(&news[1]), "news count iteration %d", it);
        int32_t n = Collection_Count(&news[0]);
        if (n && n == Collection_Count(&news[1])) {
            uint8_t* na = (uint8_t*)Collection_Get(&news[0], 0, Collection_IdAt(&news[0], 0, n - 1, 0));
            uint8_t* nb = (uint8_t*)Collection_Get(&news[1], 0, Collection_IdAt(&news[1], 0, n - 1, 0));
            EXPECT_EQ(strcmp((char*)na + 0x19, (char*)nb + 0x19) == 0 && strcmp((char*)na + 0x92, (char*)nb + 0x92) == 0, true,
                      "news text iteration %d", it);
        }
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection& n : news) Collection_Destruct(&n);
    Collection_Destruct(&types);
    Collection_Destruct(&buildings);
}

namespace rt3 {
void __fastcall Station_Init(uint8_t* st, int, int32_t id, uint32_t building, float x, float y);
uint32_t __fastcall Stations_Add(Collection* c, int, uint32_t building, float x, float y);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);
}

namespace {
// Stand-in for the exe's Tracks_AddPieceRotated (0x493cf0) during the Station_Init
// test: logs its arguments and returns a scripted id.
uint32_t g_addLog[16];
int g_addCalls;
int32_t g_addResult;
int32_t __fastcall FakeAddPieceRotated(Collection*, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style,
                                       int32_t force) {
    g_addLog[0] = *(uint32_t*)&cx;
    g_addLog[1] = *(uint32_t*)&cy;
    g_addLog[2] = *(uint32_t*)&angle;
    g_addLog[3] = (uint32_t)kind & 0xff; // the original only sets the low byte of this argument (the callee reads that byte)
    g_addLog[4] = (uint32_t)owner;
    g_addLog[5] = (uint32_t)style;
    g_addLog[6] = (uint32_t)force;
    ++g_addCalls;
    return g_addResult;
}
uint8_t g_stState[3][0x100];
void* __fastcall FakeBuildingStateObj(uint8_t* b) { return g_stState[b[0x10] % 3]; }
} // namespace

DIFFTEST(game_StationInit) {
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    FakeEngine fe;
    for (int d = 0; d < 360; ++d) {
        *(float*)(fe.e + 0x1136b1 + d * 4) = (float)sin(d * 3.14159265358979 / 180.0);
        *(float*)(fe.e + 0x113c51 + d * 4) = (float)cos(d * 3.14159265358979 / 180.0);
    }
    // Patch 0x493cf0 with a jump to the stub (exe code is unhooked in tests).
    uint8_t* site = (uint8_t*)0x493cf0;
    uint8_t siteSaved[5];
    DWORD prot;
    VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &prot);
    memcpy(siteSaved, site, 5);
    site[0] = 0xe9;
    *(int32_t*)(site + 1) = (int32_t)((uint8_t*)&FakeAddPieceRotated - (site + 5));
    FlushInstructionCache(GetCurrentProcess(), site, 5);

    const int W = 8, CELLS = 32, N = 12;
    static uint8_t map[0x2200], regions[W * W][0x1d7], regionInit[W * W][0x1d7];
    *(int32_t*)(map + 0x2145) = W;
    *(int32_t*)(map + 0x2149) = W;
    *(uint8_t**)(map + 0x2129) = regions[0];
    *(int32_t*)(map + 0x15d9) = CELLS + 1;
    *(int32_t*)(map + 0x15d5) = CELLS + 1;
    *(int32_t*)(map + 0x2155) = CELLS;
    *(int32_t*)(map + 0x2159) = CELLS;
    static uint32_t gmem[0x100];
    Collection& nodes = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x800];
    for (int k = 0; k < N; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    static uint8_t* columns[CELLS];
    static uint8_t entries[CELLS][CELLS][10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    static Collection types, smallTypes, buildings, stations;
    Collection* tabs[4] = { &types, &smallTypes, &buildings, &stations };
    const uint32_t sizes[4] = { 0x200, 0x40, 0x400, 0x700 };
    for (int k = 0; k < 4; ++k) {
        memset(tabs[k], 0, sizeof(Collection));
        Collection_Construct(tabs[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
        for (int j = 0; j < 3; ++j) Collection_Allocate(tabs[k], 0, blank, sizes[k], 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x62c120 }, { 0x6cfca8 }, { 0x62b2fc }, { 0x62b268 }, { 0x62b26c }, { 0x6cec20 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x62b2fc) = &types;
    hook::Global<Collection*>(0x62b268) = &smallTypes;
    hook::Global<Collection*>(0x62b26c) = &buildings;
    hook::Global<Collection*>(0x6cec20) = &stations;
    static void* bvtbl[0x30];
    bvtbl[0x80 / 4] = (void*)&FakeBuildingStateObj;
    static uint8_t nodeInit[N][0x260], stInit[0x700], outSt[2][0x700];
    std::string extra[2], lists[2];
    int snapped = 0, laid = 0;
    for (int it = 0; it < 400; ++it) {
        // Nodes in a chain with grid cells (as the snap test).
        for (int k = 0; k < N; ++k) {
            uint8_t* n = nodeInit[k];
            memset(n, 0, 0x260);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, CELLS * 92.0f);
            *(int32_t*)(n + 0x202) = k + 1;
            *(int32_t*)(n + 0x206) = k > 0 && Rnd() % 8 ? k : -1;
            *(int32_t*)(n + 0x20a) = k + 1 < N && Rnd() % 8 ? k + 2 : -1;
            *(int32_t*)(n + 0x20e) = Rnd() % 6 == 0 ? 1 + (int32_t)(Rnd() % N) : -1;
            *(int32_t*)(n + 0x222) = Rnd() % 4 == 0 ? (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(n + 0x226) = Rnd() % 6 == 0;
            n[0x201] = (uint8_t)Rnd();
        }
        uint32_t cellIds[CELLS][CELLS][2];
        int cellCount[CELLS][CELLS];
        for (int x = 0; x < CELLS; ++x)
            for (int y = 0; y < CELLS; ++y) {
                cellCount[x][y] = Rnd() % 3 == 0 ? 1 + (int)(Rnd() % 2) : 0;
                for (int c = 0; c < 2; ++c) cellIds[x][y][c] = 1 + Rnd() % N;
            }
        for (int r = 0; r < W * W; ++r) {
            uint8_t* reg = regionInit[r];
            memset(reg, 0, 0x1d7);
            *(int16_t*)(reg + 0xe7) = (int16_t)(r % W);
            *(int16_t*)(reg + 0xe9) = (int16_t)(r / W);
            for (int d = 0; d < 8; ++d) *(uint16_t*)(reg + 0xf3 + d * 2) = (uint16_t)(Rnd() % 50);
            *(uint16_t*)(reg + 0xd6) = (uint16_t)(Rnd() % 6 == 0 ? 1 + Rnd() % 3 : 0);
        }
        for (int k = 1; k <= 3; ++k) {
            uint8_t* ty = (uint8_t*)Collection_Get(&types, 0, k);
            *(uint32_t*)(ty + 0x173) = (uint32_t)k;
            *(uint16_t*)(ty + 0xcb) = (uint16_t)(Rnd() % 4);
            *(uint16_t*)(ty + 0xcd) = (uint16_t)(Rnd() % 4);
            ty[0xb8] = (uint8_t)(1 + Rnd() % 4);
            ty[0xb9] = (uint8_t)(1 + Rnd() % 4);
            ((uint8_t*)Collection_Get(&smallTypes, 0, k))[0x32] = 1;
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            memset(b, 0, 0x400);
            *(void***)b = bvtbl;
            b[0x10] = (uint8_t)(Rnd() % 3);
            *(uint32_t*)(b + 0x3cc) = 1 + Rnd() % 3;
            *(int32_t*)(b + 0x276) = (int32_t)(Rnd() % 3);
            b[0x3d4] = (uint8_t)(Rnd() % 2);
            *(float*)(b + 0x4f) = RndF(0.0f, 6.28f);
            *(uint16_t*)(b + 0x2a2) = 0;
        }
        for (auto& s : g_stState) s[0x8c] = (uint8_t)(Rnd() % 3);
        for (uint8_t& v : stInit) v = (uint8_t)Rnd();
        *(void**)(stInit + 0x34) = nullptr;
        *(void**)(stInit + 0x466) = nullptr;
        uint32_t bId = 1 + Rnd() % 3;
        int32_t stId = 1 + (int32_t)(Rnd() % 3);
        float px = RndF(1.0f, CELLS - 1.0f), py = RndF(1.0f, CELLS - 1.0f);
        if (it % 2) {
            // Snap-friendly: a plain stretch around node k, the building facing along it and
            // the station exactly where the snap would put it.
            int k = 2 + (int)(Rnd() % (N - 4));
            for (int j = k - 2; j <= k + 2; ++j) {
                uint8_t* n = nodeInit[j];
                *(int32_t*)(n + 0x206) = j > 0 ? j : -1;
                *(int32_t*)(n + 0x20a) = j + 1 < N ? j + 2 : -1;
                *(int32_t*)(n + 0x20e) = -1;
                *(int32_t*)(n + 0x222) = -1;
                *(int32_t*)(n + 0x226) = 0;
            }
            float cxk = (float)(((double)*(float*)(nodeInit[k] + 0x2c) + *(float*)(nodeInit[k] + 0x14)) / 184.0);
            float cyk = (float)(((double)*(float*)(nodeInit[k] + 0x34) + *(float*)(nodeInit[k] + 0x1c)) / 184.0);
            int gx = (int)cxk, gy = (int)cyk;
            if (gx >= 0 && gx < CELLS && gy >= 0 && gy < CELLS) {
                cellCount[gx][gy] = 1;
                cellIds[gx][gy][0] = (uint32_t)(k + 1);
            }
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, bId);
            uint32_t type = *(uint32_t*)(b + 0x3cc);
            memcpy(Collection_Get(&nodes, 0, k + 1), nodeInit[k], 0x260);
            float heading = (float)((double)Vehicle_ScreenHeading((uint8_t*)Collection_Get(&nodes, 0, k + 1), 0, 1) - 1.5707963705062866);
            heading = (float)fmod((double)heading + 50.26548245744, 3.14159265359);
            *(float*)(b + 0x4f) = heading;
            float ox, oy;
            Type_RotatedCell(&types, 0, type, &ox, &oy, heading);
            px = (float)((double)cxk + ox);
            py = (float)((double)cyk - oy);
        }
        g_addResult = Rnd() % 3 ? 1 + (int32_t)(Rnd() % N) : -1;
        uint32_t logs[2][8];
        int calls[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < N; ++k) memcpy(Collection_Get(&nodes, 0, k + 1), nodeInit[k], 0x260);
            memcpy(regions, regionInit, sizeof regions);
            for (int x = 0; x < CELLS; ++x) {
                columns[x] = nullptr;
                uint8_t* prev = nullptr;
                for (int y = 0; y < CELLS; ++y) {
                    if (!cellCount[x][y]) continue;
                    uint8_t* e = entries[x][y];
                    *(uint16_t*)e = (uint16_t)y;
                    uint32_t* head = nullptr;
                    for (int c = 0; c < cellCount[x][y]; ++c) {
                        uint32_t* l = (uint32_t*)((void*(__cdecl*)(size_t))0x5a125d)(8);
                        l[0] = cellIds[x][y][c];
                        l[1] = (uint32_t)head;
                        head = l;
                    }
                    *(uint32_t**)(e + 2) = head;
                    *(uint8_t**)(e + 6) = nullptr;
                    if (prev) *(uint8_t**)(prev + 6) = e;
                    else columns[x] = e;
                    prev = e;
                }
            }
            for (int k = 1; k <= 3; ++k) *(uint16_t*)((uint8_t*)Collection_Get(&stations, 0, k) + 0x3c) = 0;
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, (uint32_t)stId);
            memcpy(st, stInit, sizeof stInit);
            g_addCalls = 0;
            memset(g_addLog, 0, sizeof g_addLog);
            if (it % 4 == 3) {
                // Through Stations_Add: the next free slot (4) holds the same template.
                memcpy(stations.slots + stations.stride * 4, stInit, sizeof stInit);
                uint32_t nid = side == 0 ? ((uint32_t(__fastcall*)(Collection*, int, uint32_t, float, float))0x481390)(&stations, 0, bId, px, py)
                                         : Stations_Add(&stations, 0, bId, px, py);
                EXPECT_EQ(nid, 4u, "added id iteration %d", it);
                st = (uint8_t*)Collection_Get(&stations, 0, nid);
            } else if (side == 0) {
                ((void(__fastcall*)(uint8_t*, int, int32_t, uint32_t, float, float))0x480210)(st, 0, stId, bId, px, py);
            } else {
                Station_Init(st, 0, stId, bId, px, py);
            }
            X87Clear();
            calls[side] = g_addCalls;
            memcpy(logs[side], g_addLog, sizeof logs[side]);
            // Collect pointed-to contents, then free and normalise.
            std::string& ex = extra[side];
            ex.clear();
            void** ents = *(void***)(st + 0x5bd);
            for (int k = 0; k < 32 && ents; ++k) {
                ex.append((const char*)ents[k], 0x1a);
                xfree(ents[k]);
            }
            if (ents) xfree(ents);
            if (void* c = *(void**)(st + 0x34)) {
                ex.append((const char*)c, *(int32_t*)(st + 0x38) * 0xc);
                xfree(c);
            }
            if (void* k2 = *(void**)(st + 0x466)) {
                ex.append((const char*)k2, *(int32_t*)(st + 0x462) * 6);
                xfree(k2);
            }
            for (int r = 0; r < W * W; ++r) {
                ex.append((const char*)regions[r] + 0xd4, 4);
                if (void* k3 = *(void**)(regions[r] + 0xef)) {
                    ex.append((const char*)k3, *(int32_t*)(regions[r] + 0xeb) * 6);
                    xfree(k3);
                }
            }
            for (int k = 0; k < N; ++k) ex.append((const char*)Collection_Get(&nodes, 0, k + 1) + 0x222, 4);
            for (int k = 1; k <= 3; ++k) ex.append((const char*)Collection_Get(&stations, 0, k) + 0x3c, 2);
            *(void**)(st + 0x5bd) = *(void**)(st + 0x34) = *(void**)(st + 0x466) = nullptr;
            memcpy(outSt[side], st, sizeof stInit);
            // The five 8-byte item lists (counts +0x590, heap pointers +0x5a4): by contents.
            lists[side].clear();
            for (int j = 0; j < 5; ++j) {
                int32_t n = *(int32_t*)(st + 0x590 + j * 4);
                uint8_t* l = *(uint8_t**)(st + 0x5a4 + j * 4);
                if (n > 0 && l) lists[side].append((const char*)l, n * 8);
                *(uint32_t*)(outSt[side] + 0x5a4 + j * 4) = l ? 1 : 0;
            }
            if (it % 4 == 3) Collection_Release(&stations, 0, 4);
            for (int x = 0; x < CELLS; ++x)
                for (uint8_t* e = columns[x]; e; e = *(uint8_t**)(e + 6))
                    for (uint32_t* l = *(uint32_t**)(e + 2); l;) {
                        uint32_t* next = (uint32_t*)l[1];
                        xfree(l);
                        l = next;
                    }
        }
        snapped += outSt[0][0x46a] != 0;
        laid += calls[0];
        EXPECT_EQ(calls[0] == calls[1] && !memcmp(logs[0], logs[1], sizeof logs[0]), true, "piece call iteration %d: %d %08x %08x %08x %d %d %d %d / %d %08x %08x %08x %d %d %d %d", it, calls[0], logs[0][0], logs[0][1], logs[0][2], logs[0][3], logs[0][4], logs[0][5], logs[0][6], calls[1], logs[1][0], logs[1][1], logs[1][2], logs[1][3], logs[1][4], logs[1][5], logs[1][6]);
        EXPECT_EQ(FirstDiff(outSt[0], outSt[1], sizeof stInit), -1, "station iteration %d", it);
        EXPECT_EQ(extra[0] == extra[1], true, "tables iteration %d", it);
        EXPECT_EQ(lists[0] == lists[1], true, "item lists iteration %d", it);
    }
    EXPECT_EQ(snapped > 5 && laid > 50, true, "coverage: %d snapped, %d laid", snapped, laid);
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection* c : tabs) Collection_Destruct(c);
    Collection_Destruct(&nodes);
    memcpy(site, siteSaved, 5);
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, prot, &prot);
}

namespace rt3 {
void __fastcall Station_Move(uint8_t* st, int, float x, float y, int32_t keepTrack, int32_t kind);
}

DIFFTEST(game_StationMove) {
    g_seed = 3751679088u; // this sequence moves a station off the map at iteration 81
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    FakeEngine fe;
    for (int d = 0; d < 360; ++d) {
        *(float*)(fe.e + 0x1136b1 + d * 4) = (float)sin(d * 3.14159265358979 / 180.0);
        *(float*)(fe.e + 0x113c51 + d * 4) = (float)cos(d * 3.14159265358979 / 180.0);
    }
    // Patch 0x493cf0 with a jump to the stub (exe code is unhooked in tests).
    uint8_t* site = (uint8_t*)0x493cf0;
    uint8_t siteSaved[5];
    DWORD prot;
    VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &prot);
    memcpy(siteSaved, site, 5);
    site[0] = 0xe9;
    *(int32_t*)(site + 1) = (int32_t)((uint8_t*)&FakeAddPieceRotated - (site + 5));
    FlushInstructionCache(GetCurrentProcess(), site, 5);

    const int W = 8, CELLS = 32, N = 12;
    // Rows past the grid: off-map stations index beyond it (both sides), keep that inside the per-side copy.
    static uint8_t map[0x2200], regions[W * (W + 4)][0x1d7], regionInit[W * (W + 4)][0x1d7];
    *(int32_t*)(map + 0x2145) = W;
    *(int32_t*)(map + 0x2149) = W;
    *(uint8_t**)(map + 0x2129) = regions[0];
    *(int32_t*)(map + 0x15d9) = CELLS + 1;
    *(int32_t*)(map + 0x15d5) = CELLS + 1;
    *(int32_t*)(map + 0x2155) = CELLS;
    *(int32_t*)(map + 0x2159) = CELLS;
    static uint32_t gmem[0x100];
    Collection& nodes = *(Collection*)gmem;
    memset(gmem, 0, sizeof gmem);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    static uint8_t blank[0x800];
    for (int k = 0; k < N; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    static uint8_t* columns[CELLS];
    static uint8_t entries[CELLS][CELLS][10];
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = columns;
    static Collection types, smallTypes, buildings, stations;
    Collection* tabs[4] = { &types, &smallTypes, &buildings, &stations };
    const uint32_t sizes[4] = { 0x200, 0x40, 0x400, 0x700 };
    for (int k = 0; k < 4; ++k) {
        memset(tabs[k], 0, sizeof(Collection));
        Collection_Construct(tabs[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
        for (int j = 0; j < 3; ++j) Collection_Allocate(tabs[k], 0, blank, sizes[k], 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x62c120 }, { 0x6cfca8 }, { 0x62b2fc }, { 0x62b268 }, { 0x62b26c }, { 0x6cec20 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x62b2fc) = &types;
    hook::Global<Collection*>(0x62b268) = &smallTypes;
    hook::Global<Collection*>(0x62b26c) = &buildings;
    hook::Global<Collection*>(0x6cec20) = &stations;
    static uint8_t terrain[0x1700];
    static float theights[(CELLS + 1) * (CELLS + 1)];
    static PtrList tcells[4];
    static PtrList* tcellPtrs[4] = { &tcells[0], &tcells[1], &tcells[2], &tcells[3] };
    *(int32_t*)(terrain + 0x15d9) = CELLS;
    *(int32_t*)(terrain + 0x15d5) = CELLS;
    *(float**)(terrain + 0x1605) = theights;
    *(PtrList***)(terrain + 0x1675) = tcellPtrs;
    for (PtrList& l : tcells) PtrList_Init(&l);
    static void* bvtbl[0x30];
    bvtbl[0x80 / 4] = (void*)&FakeBuildingStateObj;
    static uint8_t nodeInit[N][0x260], stInit[0x700], outSt[2][0x700];
    std::string extra[2], lists[2];
    int snapped = 0, laid = 0;
    for (int it = 0; it < 400; ++it) {
        // Nodes in a chain with grid cells (as the snap test).
        for (int k = 0; k < N; ++k) {
            uint8_t* n = nodeInit[k];
            memset(n, 0, 0x260);
            for (int j = 0x14; j < 0x44; j += 4) *(float*)(n + j) = RndF(0.0f, CELLS * 92.0f);
            *(int32_t*)(n + 0x202) = k + 1;
            *(int32_t*)(n + 0x206) = k > 0 && Rnd() % 8 ? k : -1;
            *(int32_t*)(n + 0x20a) = k + 1 < N && Rnd() % 8 ? k + 2 : -1;
            *(int32_t*)(n + 0x20e) = Rnd() % 6 == 0 ? 1 + (int32_t)(Rnd() % N) : -1;
            *(int32_t*)(n + 0x222) = Rnd() % 4 == 0 ? (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(n + 0x226) = Rnd() % 6 == 0;
            n[0x201] = (uint8_t)Rnd();
            *(uint8_t**)(n + 4) = terrain;
            for (int j = 0x45; j <= 0x51; j += 4) *(float*)(n + j) = RndF(0.0f, 30.0f);
            for (int j = 0x1f5; j <= 0x1fd; j += 4) *(float*)(n + j) = RndF(0.5f, 20.0f);
        }
        for (float& h : theights) h = RndF(0.0f, 100.0f);
        uint32_t cellIds[CELLS][CELLS][2];
        int cellCount[CELLS][CELLS];
        for (int x = 0; x < CELLS; ++x)
            for (int y = 0; y < CELLS; ++y) {
                cellCount[x][y] = Rnd() % 3 == 0 ? 1 + (int)(Rnd() % 2) : 0;
                for (int c = 0; c < 2; ++c) cellIds[x][y][c] = 1 + Rnd() % N;
            }
        for (int r = 0; r < W * W; ++r) {
            uint8_t* reg = regionInit[r];
            memset(reg, 0, 0x1d7);
            *(int16_t*)(reg + 0xe7) = (int16_t)(r % W);
            *(int16_t*)(reg + 0xe9) = (int16_t)(r / W);
            for (int d = 0; d < 8; ++d) *(uint16_t*)(reg + 0xf3 + d * 2) = (uint16_t)(Rnd() % 50);
            *(uint16_t*)(reg + 0xd6) = (uint16_t)(Rnd() % 6 == 0 ? 1 + Rnd() % 3 : 0);
        }
        for (int k = 1; k <= 3; ++k) {
            uint8_t* ty = (uint8_t*)Collection_Get(&types, 0, k);
            *(uint32_t*)(ty + 0x173) = (uint32_t)k;
            *(uint16_t*)(ty + 0xcb) = (uint16_t)(Rnd() % 4);
            *(uint16_t*)(ty + 0xcd) = (uint16_t)(Rnd() % 4);
            ty[0xb8] = (uint8_t)(1 + Rnd() % 4);
            ty[0xb9] = (uint8_t)(1 + Rnd() % 4);
            ((uint8_t*)Collection_Get(&smallTypes, 0, k))[0x32] = 1;
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            memset(b, 0, 0x400);
            *(void***)b = bvtbl;
            b[0x10] = (uint8_t)(Rnd() % 3);
            *(uint32_t*)(b + 0x3cc) = 1 + Rnd() % 3;
            *(int32_t*)(b + 0x276) = (int32_t)(Rnd() % 3);
            b[0x3d4] = (uint8_t)(Rnd() % 2);
            *(float*)(b + 0x4f) = RndF(0.0f, 6.28f);
            *(uint16_t*)(b + 0x2a2) = 0;
        }
        for (auto& s : g_stState) s[0x8c] = (uint8_t)(Rnd() % 3);
        for (auto& s : g_stState) { // building type fields; kind 4 fills the station's item lists
            *(int32_t*)(s + 0x3f) = Rnd() % 4 != 0;
            *(uint32_t*)(s + 0x22) = Rnd() % 2000000;
            *(int32_t*)(s + 0x43) = (int32_t)(Rnd() % 100000);
            s[0x32] = (uint8_t)(Rnd() % 6);
            strcpy((char*)s + 0x4f, "Coal");
        }
        for (uint8_t& v : stInit) v = (uint8_t)Rnd();
        *(void**)(stInit + 0x34) = nullptr;
        *(void**)(stInit + 0x466) = nullptr;
        uint32_t bId = 1 + Rnd() % 3;
        int32_t stId = 1 + (int32_t)(Rnd() % 3);
        float px = RndF(1.0f, CELLS - 1.0f), py = RndF(1.0f, CELLS - 1.0f);
        if (it % 2) {
            // Snap-friendly: a plain stretch around node k, the building facing along it and
            // the station exactly where the snap would put it.
            int k = 2 + (int)(Rnd() % (N - 4));
            for (int j = k - 2; j <= k + 2; ++j) {
                uint8_t* n = nodeInit[j];
                *(int32_t*)(n + 0x206) = j > 0 ? j : -1;
                *(int32_t*)(n + 0x20a) = j + 1 < N ? j + 2 : -1;
                *(int32_t*)(n + 0x20e) = -1;
                *(int32_t*)(n + 0x222) = -1;
                *(int32_t*)(n + 0x226) = 0;
            }
            float cxk = (float)(((double)*(float*)(nodeInit[k] + 0x2c) + *(float*)(nodeInit[k] + 0x14)) / 184.0);
            float cyk = (float)(((double)*(float*)(nodeInit[k] + 0x34) + *(float*)(nodeInit[k] + 0x1c)) / 184.0);
            int gx = (int)cxk, gy = (int)cyk;
            if (gx >= 0 && gx < CELLS && gy >= 0 && gy < CELLS) {
                cellCount[gx][gy] = 1;
                cellIds[gx][gy][0] = (uint32_t)(k + 1);
            }
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, bId);
            uint32_t type = *(uint32_t*)(b + 0x3cc);
            memcpy(Collection_Get(&nodes, 0, k + 1), nodeInit[k], 0x260);
            float heading = (float)((double)Vehicle_ScreenHeading((uint8_t*)Collection_Get(&nodes, 0, k + 1), 0, 1) - 1.5707963705062866);
            heading = (float)fmod((double)heading + 50.26548245744, 3.14159265359);
            *(float*)(b + 0x4f) = heading;
            float ox, oy;
            Type_RotatedCell(&types, 0, type, &ox, &oy, heading);
            px = (float)((double)cxk + ox);
            py = (float)((double)cyk - oy);
        }
        g_addResult = Rnd() % 3 ? 1 + (int32_t)(Rnd() % N) : -1;
        int32_t addResult1 = g_addResult, addResult2 = Rnd() % 3 ? 1 + (int32_t)(Rnd() % N) : -1;
        // The move target: near the original spot (often snappable again) or elsewhere.
        float mx = Rnd() % 2 ? px : RndF(1.0f, CELLS - 1.0f), my = Rnd() % 2 ? py : RndF(1.0f, CELLS - 1.0f);
        // The catchment kind must match the one it was built with (-1): Obj_RemoveKey
        // assumes every key it removes is present and overflows its buffer otherwise.
        int32_t keep = Rnd() % 6 == 0, mkind = -1;
        uint32_t logs[2][8];
        int calls[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < N; ++k) memcpy(Collection_Get(&nodes, 0, k + 1), nodeInit[k], 0x260);
            memcpy(regions, regionInit, sizeof regions);
            for (int x = 0; x < CELLS; ++x) {
                columns[x] = nullptr;
                uint8_t* prev = nullptr;
                for (int y = 0; y < CELLS; ++y) {
                    if (!cellCount[x][y]) continue;
                    uint8_t* e = entries[x][y];
                    *(uint16_t*)e = (uint16_t)y;
                    uint32_t* head = nullptr;
                    for (int c = 0; c < cellCount[x][y]; ++c) {
                        uint32_t* l = (uint32_t*)((void*(__cdecl*)(size_t))0x5a125d)(8);
                        l[0] = cellIds[x][y][c];
                        l[1] = (uint32_t)head;
                        head = l;
                    }
                    *(uint32_t**)(e + 2) = head;
                    *(uint8_t**)(e + 6) = nullptr;
                    if (prev) *(uint8_t**)(prev + 6) = e;
                    else columns[x] = e;
                    prev = e;
                }
            }
            for (int k = 1; k <= 3; ++k) *(uint16_t*)((uint8_t*)Collection_Get(&stations, 0, k) + 0x3c) = 0;
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, (uint32_t)stId);
            memcpy(st, stInit, sizeof stInit);
            g_addCalls = 0;
            memset(g_addLog, 0, sizeof g_addLog);
            ((void(__fastcall*)(uint8_t*, int, int32_t, uint32_t, float, float))0x480210)(st, 0, stId, bId, px, py);
            g_addResult = addResult2;
            if (side == 0) ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x480710)(st, 0, mx, my, keep, mkind);
            else Station_Move(st, 0, mx, my, keep, mkind);
            g_addResult = addResult1;
            X87Clear();
            calls[side] = g_addCalls;
            memcpy(logs[side], g_addLog, sizeof logs[side]);
            // Collect pointed-to contents, then free and normalise.
            std::string& ex = extra[side];
            ex.clear();
            void** ents = *(void***)(st + 0x5bd);
            for (int k = 0; k < 32 && ents; ++k) {
                ex.append((const char*)ents[k], 0x1a);
                xfree(ents[k]);
            }
            if (ents) xfree(ents);
            if (void* c = *(void**)(st + 0x34)) {
                ex.append((const char*)c, *(int32_t*)(st + 0x38) * 0xc);
                xfree(c);
            }
            if (void* k2 = *(void**)(st + 0x466)) {
                ex.append((const char*)k2, *(int32_t*)(st + 0x462) * 6);
                xfree(k2);
            }
            for (int r = 0; r < W * W; ++r) {
                ex.append((const char*)regions[r] + 0xd4, 4);
                if (void* k3 = *(void**)(regions[r] + 0xef)) {
                    ex.append((const char*)k3, *(int32_t*)(regions[r] + 0xeb) * 6);
                    xfree(k3);
                }
            }
            for (int k = 0; k < N; ++k) {
                char valid = (char)Collection_IsValid(&nodes, 0, (uint32_t)(k + 1));
                ex.append(&valid, 1);
                if (valid) {
                    uint8_t norm[0x260];
                    memcpy(norm, Collection_Get(&nodes, 0, k + 1), 0x260);
                    if (void* q = *(void**)(norm + 0x1e3)) {
                        ex.append((const char*)q, 0xc0);
                        xfree(q);
                        *(void**)((uint8_t*)Collection_Get(&nodes, 0, k + 1) + 0x1e3) = nullptr;
                    }
                    *(uintptr_t*)(norm + 0x1e3) = 0;
                    *(uintptr_t*)(norm + 0x55) = *(uintptr_t*)(norm + 0x55) ? 1 : 0;
                    ex.append((const char*)norm, 0x260);
                }
                else Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0); // put the released piece back for the next run
            }
            for (int k = 1; k <= 3; ++k) ex.append((const char*)Collection_Get(&stations, 0, k) + 0x3c, 2);
            *(void**)(st + 0x5bd) = *(void**)(st + 0x34) = *(void**)(st + 0x466) = nullptr;
            memcpy(outSt[side], st, sizeof stInit);
            // The five 8-byte item lists (counts +0x590, heap pointers +0x5a4): by contents.
            lists[side].clear();
            for (int j = 0; j < 5; ++j) {
                int32_t n = *(int32_t*)(st + 0x590 + j * 4);
                uint8_t* l = *(uint8_t**)(st + 0x5a4 + j * 4);
                if (n > 0 && l) lists[side].append((const char*)l, n * 8);
                *(uint32_t*)(outSt[side] + 0x5a4 + j * 4) = l ? 1 : 0;
            }
            for (PtrList& l : tcells) PtrList_Clear(&l);
            for (int x = 0; x < CELLS; ++x)
                for (uint8_t* e = columns[x]; e; e = *(uint8_t**)(e + 6))
                    for (uint32_t* l = *(uint32_t**)(e + 2); l;) {
                        uint32_t* next = (uint32_t*)l[1];
                        xfree(l);
                        l = next;
                    }
        }
        snapped += outSt[0][0x46a] != 0;
        laid += calls[0] > 0;
        EXPECT_EQ(calls[0] == calls[1] && !memcmp(logs[0], logs[1], sizeof logs[0]), true, "piece call iteration %d: %d %08x %08x %08x %d %d %d %d / %d %08x %08x %08x %d %d %d %d", it, calls[0], logs[0][0], logs[0][1], logs[0][2], logs[0][3], logs[0][4], logs[0][5], logs[0][6], calls[1], logs[1][0], logs[1][1], logs[1][2], logs[1][3], logs[1][4], logs[1][5], logs[1][6]);
        EXPECT_EQ(FirstDiff(outSt[0], outSt[1], sizeof stInit), -1, "station iteration %d", it);
        EXPECT_EQ(extra[0] == extra[1], true, "tables iteration %d", it);
        EXPECT_EQ(lists[0] == lists[1], true, "item lists iteration %d", it);
    }
    EXPECT_EQ(snapped > 5 && laid > 20, true, "coverage: %d snapped, %d laid", snapped, laid);
    *(uint8_t***)((uint8_t*)gmem + 0x8c) = nullptr;
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection* c : tabs) Collection_Destruct(c);
    Collection_Destruct(&nodes);
    memcpy(site, siteSaved, 5);
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    VirtualProtect(site, 5, prot, &prot);
}

namespace rt3 {
int32_t __fastcall Dialog_HandleEvent(uint32_t* ev);
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall Ui_Dispatch(uint8_t** mgr, int, uint32_t* ev);
void __fastcall RulesDialog_Store(uint8_t* dlg);
}

namespace {
// Scripted handlers: each widget's answer and an optional event edit depend on its
// script byte (+0x10) and the event; every call is logged.
uint32_t g_evLog[256];
int g_evN, g_evCalls;
int32_t ScriptedAnswer(uint8_t* w, uint32_t* ev) {
    if (g_evN + 3 <= 256) {
        g_evLog[g_evN++] = w[0x11];
        g_evLog[g_evN++] = ev[0];
        g_evLog[g_evN++] = ev[1];
    }
    if (++g_evCalls > 200) return 0; // keep restarts finite
    uint8_t s = w[0x10];
    if (ev[0] == 0x94 && (s & 0x10)) ev[2] = 0x5014 + (s & 7) + (s & 0x20 ? 4 : 0);
    if ((s & 0x40) && ev[3] < 3) ++ev[3];
    switch (s & 3) {
    case 0: return 1;
    case 1: return 0;
    case 2: return 2;
    default: return (int32_t)(ev[0] & 3);
    }
}
int32_t __fastcall FakeScriptedHandler(uint8_t* w, int, uint32_t* ev) { return ScriptedAnswer(w, ev); }
} // namespace

DIFFTEST(game_UiEventHelpers) {
    static void* vtbl[4] = { nullptr, (void*)&FakeScriptedHandler };
    static uint8_t wins[2][4][0x200], kids[2][4][0x100], cap[2][0x100], ui[2][0xd00], rules[2][0x900];
    static Collection rulesTab[2];
    for (Collection& c : rulesTab) {
        memset(&c, 0, sizeof c);
        Collection_Construct(&c, 0, 1, 0x900, 4, 4, 0, 0, 0);
        static uint8_t blank[0x900];
        Collection_Allocate(&c, 0, blank, 0x900, 0, 0);
    }
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6d3b44 }, { 0x6d401c }, { 0x62be18 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    for (int it = 0; it < 4000; ++it) {
        uint8_t winScript[4], kidScript[4], capScript = (uint8_t)Rnd();
        int32_t kidValue[4];
        for (int32_t& v : kidValue) v = (int32_t)(Rnd() % 100);
        bool hasCap = Rnd() % 3 == 0, hasDlg = Rnd() % 6 != 0;
        int nWins = 1 + (int)(Rnd() % 4), nKids = (int)(Rnd() % 3);
        for (int k = 0; k < 4; ++k) {
            winScript[k] = (uint8_t)Rnd();
            kidScript[k] = (uint8_t)Rnd();
        }
        uint32_t evInit[12];
        for (uint32_t& v : evInit) v = Rnd() % 4;
        evInit[0] = Rnd() % 3 ? Rnd() % 8 : 0x94;
        evInit[1] = Rnd() % 4 ? 0x5006 : Rnd();
        int op = (int)(Rnd() % 5);
        uint32_t args[5];
        for (uint32_t& a : args) a = Rnd() % 8;
        int32_t dlgSlot = (int32_t)(Rnd() % 4);
        int32_t ruleId = Rnd() % 6 ? 1 : -1;
        int32_t r[2];
        uint32_t evOut[2][12], logs[2][256];
        int logN[2];
        for (int side = 0; side < 2; ++side) {
            for (int k = 0; k < 4; ++k) {
                uint8_t* w = wins[side][k];
                memset(w, 0, 0x200);
                *(void***)w = vtbl;
                w[0x10] = winScript[k];
                w[0x11] = (uint8_t)(0x10 + k);
                *(int32_t*)(w + 0x60) = 0; // not draggable: Window_HandleEvent passes presses to the base handler
                *(uint8_t**)(w + 0x58) = k + 1 < nWins ? wins[side][k + 1] : nullptr;
                *(uint8_t**)(w + 0x74) = nullptr;
                uint8_t* c = kids[side][k];
                memset(c, 0, 0x100);
                *(void***)c = vtbl;
                c[0x10] = kidScript[k];
                c[0x11] = (uint8_t)(0x20 + k);
                *(uint16_t*)(c + 4) = (uint16_t)(k == 0 ? 0x500a : 0x5000 + k);
                *(int32_t*)(c + 0xcc) = kidValue[k];
                *(uint8_t**)(c + 0x8a) = k + 1 < nKids ? kids[side][k + 1] : nullptr;
                *(uint8_t**)(c + 0x8e) = k + 1 < 4 ? kids[side][k + 1] : nullptr;
            }
            *(uint8_t**)(wins[side][0] + 0x74) = nKids ? kids[side][0] : nullptr;
            *(uint8_t**)(wins[side][0] + 0x70) = kids[side][0];
            *(int32_t*)(wins[side][0] + 0x84) = ruleId;
            *(int32_t*)(wins[side][0] + 0x9c) = dlgSlot;
            *(int32_t*)(wins[side][0] + 0x8c) = 0x777;
            memset(cap[side], 0, 0x100);
            *(void***)cap[side] = vtbl;
            cap[side][0x10] = capScript;
            cap[side][0x11] = 0x30;
            memset(ui[side], 0, sizeof ui[side]);
            *(uint8_t**)(ui[side] + 0xc75) = hasCap ? cap[side] : nullptr;
            hook::Global<uint8_t*>(0x6d401c) = ui[side];
            hook::Global<uint8_t*>(0x6d3b44) = hasDlg ? wins[side][0] : nullptr;
            hook::Global<Collection*>(0x62be18) = &rulesTab[side];
            memset(Collection_Get(&rulesTab[side], 0, 1), 0, 0x900);
            uint32_t* ev = evOut[side];
            memcpy(ev, evInit, sizeof evInit);
            g_evN = 0;
            g_evCalls = 0;
            uint8_t* mgr[1] = { wins[side][0] };
            int32_t res = 0;
            switch (op) {
            case 0: res = side ? Dialog_HandleEvent(ev) : ((int32_t(__fastcall*)(uint32_t*))0x5164d0)(ev); break;
            case 1:
                res = side ? Window_SendEvent4(wins[side][0], 0, args[0], args[1], args[2], args[3])
                           : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t))0x5400d0)(wins[side][0], 0, args[0], args[1],
                                                                                                                  args[2], args[3]);
                break;
            case 2:
                res = side ? Window_SendEvent5(wins[side][0], 0, args[0], args[1], args[2], args[3], args[4])
                           : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))0x540120)(
                                 wins[side][0], 0, args[0], args[1], args[2], args[3], args[4]);
                break;
            case 3: res = side ? Ui_Dispatch(mgr, 0, ev) : ((int32_t(__fastcall*)(uint8_t**, int, uint32_t*))0x5388d0)(mgr, 0, ev); break;
            case 4:
                if (side) RulesDialog_Store(wins[side][0]);
                else ((void(__fastcall*)(uint8_t*))0x4d8d50)(wins[side][0]);
                break;
            }
            r[side] = res;
            logN[side] = g_evN;
            memcpy(logs[side], g_evLog, sizeof g_evLog);
            memcpy(rules[side], Collection_Get(&rulesTab[side], 0, 1), 0x900);
        }
        EXPECT_EQ(r[0], r[1], "op %d result iteration %d", op, it);
        EXPECT_EQ(memcmp(evOut[0], evOut[1], sizeof evOut[0]), 0, "op %d event iteration %d: %u %u %u / %u %u %u (r %d %d, calls %d %d)", op, it, evOut[0][0],
                  evOut[0][1], evOut[0][2], evOut[1][0], evOut[1][1], evOut[1][2], r[0], r[1], logN[0] / 3, logN[1] / 3);
        EXPECT_EQ(logN[0] == logN[1] && !memcmp(logs[0], logs[1], logN[0] * 4), true, "op %d calls iteration %d", op, it);
        EXPECT_EQ(memcmp(rules[0], rules[1], 0x900), 0, "op %d rule iteration %d at 0x%x (%02x vs %02x) slot %d", op, it, FirstDiff(rules[0], rules[1], 0x900),
                  rules[0][FirstDiff(rules[0], rules[1], 0x900) & 0xfff], rules[1][FirstDiff(rules[0], rules[1], 0x900) & 0xfff], dlgSlot);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    for (Collection& c : rulesTab) Collection_Destruct(&c);
}

namespace rt3 {
void __fastcall Window_HighlightTab7(uint8_t* w);
void __fastcall Window_ShowWorldPicks(uint8_t* w);
void CityDialog_UpdateButtons();
int32_t __fastcall Dialog_SetCallback4a3e(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3b(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a43(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a42(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a3f(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a41(uint8_t* w);
int32_t __fastcall Dialog_SetCallback4a40(uint8_t* w);
int32_t __fastcall Dialog_SetupControl5141(uint8_t* w);
void __fastcall ListDialog_PickRow(int32_t row);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
uint8_t* __fastcall Window_EnableControl7f9(uint8_t* w, int, uint8_t on);
void __fastcall NewGameDialog_ShowSetting77(uint8_t* w);
void __fastcall NewGameDialog_ShowSetting7b(uint8_t* w);
void __fastcall NewGameDialog_ShowDifficulty(uint8_t* w);
void __fastcall Window_HighlightTabs7And5(uint8_t* w);
void __fastcall Window_FillList759e(uint8_t* w);
void __fastcall Window_SelectOption755f(uint8_t* w);
void __fastcall Window_SelectOption7569(uint8_t* w);
}

namespace {
std::vector<uint32_t> g_sent;
std::vector<std::string> g_texts[2];
int g_textSide;
// Radio-group queries (0x94) the fake window answers: id -> value in ev[2] (0: none).
uint32_t g_ansId[4], g_ansVal[4];
int32_t __fastcall FakeLoggingBase(uint8_t* w, int, uint32_t* ev) {
    std::string text = (ev[0] == 0x6f || ev[0] == 0x88) && ev[2] ? (const char*)(uintptr_t)ev[2] : "";
    if (ev[0] == 0x88 && ev[4] > 0x10000) text += std::string("|") + (const char*)(uintptr_t)ev[4];
    if (ev[0] == 0x6e && ev[2] > 0x10000) text = (const char*)(uintptr_t)ev[2];
    if ((ev[0] == 0x70 || ev[0] == 0x75) && ev[2] > 0x10000) text = (const char*)(uintptr_t)ev[2];
    if (ev[0] == 0x89 && ev[2]) { // rich row: {name, text, 0, color, ...}
        const uint32_t* item = (const uint32_t*)(uintptr_t)ev[2];
        char c[16];
        sprintf(c, "|%08x", item[3]);
        text = std::string(item[0] ? (const char*)(uintptr_t)item[0] : "") + "|" + (item[1] ? (const char*)(uintptr_t)item[1] : "") + c;
    }
    g_texts[g_textSide].push_back(text);
    g_sent.push_back(w[0x11]);
    // A query event (0x94) is sent with only its first two fields set (the original
    // leaves the rest uninitialized).
    for (int k = 0; k < 12; ++k) g_sent.push_back(ev[0] == 0x94 && k >= 2 ? 0 : ev[k]);
    for (int k = 0; k < 4; ++k)
        if (ev[0] == 0x94 && g_ansId[k] && g_ansId[k] == ev[1]) ev[2] = g_ansVal[k];
    return 0x40 + (int32_t)(ev[0] & 7);
}
} // namespace

DIFFTEST(game_DialogSetupHelpers) {
    static void* vtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    static uint8_t win[0x200], list[0x200], world[0x5000], buf[2][0x100];
    static Collection cities;
    memset(&cities, 0, sizeof cities);
    Collection_Construct(&cities, 0, 1, 0x400, 4, 4, 0, 0, 0);
    static uint8_t blank[0x400];
    for (int k = 0; k < 3; ++k) Collection_Allocate(&cities, 0, blank, 0x400, 0, 0);
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6cec78 }, { 0x62bae0 }, { 0x6cff74 }, { 0x6cff84 }, { 0x6d0810 }, { 0x6d0808 }, { 0x6d0804 },
                  { 0x6cffc4 }, { 0x6d07ac }, { 0x6cffb0 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<Collection*>(0x62bae0) = &cities;
    static const char* rowsText[4] = { "Alpha", "Bravo row", "", "Delta" };
    static uint8_t rows[4][0x20];
    for (int k = 0; k < 4; ++k) *(const char**)rows[k] = rowsText[k];
    memset(win, 0, sizeof win);
    *(void***)win = vtbl;
    win[0x11] = 1;
    *(uint8_t**)(win + 0x70) = list;
    memset(list, 0, sizeof list);
    *(uint16_t*)(list + 4) = 0x1f45;
    *(uint8_t**)(list + 0x176) = rows[0];
    static uint8_t newGame[0x100], opts[0x400], panel[0x100], ui[0xd00];
    uint32_t sNewGame = hook::Global<uint32_t>(0x6cec7c), sOpts = hook::Global<uint32_t>(0x6cec74), sUi = hook::Global<uint32_t>(0x6d401c);
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6d401c) = ui;
    FakeEngine fe;
    *(uint16_t*)(list + 6) = 0;
    *(uint16_t*)(list + 4) = 0x1f45;
    static uint8_t ctl808[0x100];
    *(uint8_t**)(list + 0x8e) = ctl808;
    *(uint16_t*)(ctl808 + 4) = 0x808;
    for (int it = 0; it < 800; ++it) {
        int op = it % 23;
        *(uint16_t*)(newGame + 0x77) = (uint16_t)Rnd();
        *(uint16_t*)(newGame + 0x7b) = (uint16_t)Rnd();
        newGame[0x7d] = (uint8_t)(Rnd() % 6);
        *(int32_t*)(win + 0x7c) = (int32_t)(Rnd() % 6);
        *(int32_t*)(opts + 0xac + 4) = Rnd() % 4 ? (int32_t)(Rnd() % 4) : -1;
        *(int32_t*)(opts + 0xac + 8) = Rnd() % 4 ? (int32_t)(Rnd() % 4) : -1;
        *(uint32_t*)(fe.e + 0x11474e) = Rnd() % 5;
        uint32_t uargs[5] = { Rnd() % 9, Rnd() % 9, Rnd() % 9, Rnd() % 9, Rnd() % 9 };
        int32_t pval = (int32_t)Rnd();
        uint8_t on = (uint8_t)(Rnd() % 2);
        bool hasPanelWin = Rnd() % 3 != 0;
        *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 9);
        *(int32_t*)(world + 0x4c78) = (int32_t)(Rnd() % 5);
        *(int32_t*)(world + 0x4c7c) = (int32_t)(Rnd() % 5);
        hook::Global<uint32_t>(0x6cff74) = Rnd() % 5;
        for (int k = 1; k <= 3; ++k) *(int32_t*)((uint8_t*)Collection_Get(&cities, 0, k) + 0x2f6) = (int32_t)(Rnd() % 2);
        int32_t row = Rnd() % 5 == 0 ? -1 : (int32_t)(Rnd() % 4);
        std::vector<uint32_t> sent[2];
        uint32_t globals[2][4];
        int32_t res[2];
        for (int side = 0; side < 2; ++side) {
            hook::Global<uint8_t*>(0x6cff84) = win;
            hook::Global<uint8_t*>(0x6d0810) = win;
            memset(buf[side], 0x5a, 0x100);
            hook::Global<uint8_t*>(0x6d0808) = buf[side];
            hook::Global<int32_t>(0x6d0804) = hook::Global<int32_t>(0x6cffc4) = hook::Global<int32_t>(0x6d07ac) =
                hook::Global<int32_t>(0x6cffb0) = 9;
            g_sent.clear();
            g_textSide = side;
            g_texts[side].clear();
            int32_t r = 0;
            auto pick = [&](uint32_t addr, int32_t(__fastcall * mine)(uint8_t*)) {
                return side ? mine(win) : ((int32_t(__fastcall*)(uint8_t*))addr)(win);
            };
            switch (op) {
            case 0: side ? Window_HighlightTab7(win) : ((void(__fastcall*)(uint8_t*))0x4b84b0)(win); break;
            case 1: side ? Window_ShowWorldPicks(win) : ((void(__fastcall*)(uint8_t*))0x4ca910)(win); break;
            case 2: side ? CityDialog_UpdateButtons() : ((void(*)())0x4cbdd0)(); break;
            case 3: r = pick(0x4d2fe0, &Dialog_SetCallback4a3e); break;
            case 4: r = pick(0x4d3000, &Dialog_SetCallback4a3b); break;
            case 5: r = pick(0x4d4110, &Dialog_SetCallback4a43); break;
            case 6: r = pick(0x4d4130, &Dialog_SetCallback4a42); break;
            case 7: r = pick(0x4d3060, &Dialog_SetCallback4a3f); break;
            case 8: r = pick(0x4d3080, &Dialog_SetCallback4a41); break;
            case 9: r = pick(0x4d30a0, &Dialog_SetCallback4a40); break;
            case 10: r = pick(0x4d3020, &Dialog_SetupControl5141); break;
            case 11: side ? ListDialog_PickRow(row) : ((void(__fastcall*)(int32_t))0x4dcfb0)(row); break;
            case 12: {
                uint8_t* mgr[1] = { win };
                r = side ? Ui_SendEvent5(mgr, 0, uargs[0], uargs[1], uargs[2], uargs[3], uargs[4])
                         : ((int32_t(__fastcall*)(uint8_t**, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))0x538e00)(mgr, 0, uargs[0], uargs[1],
                                                                                                                   uargs[2], uargs[3], uargs[4]);
                break;
            }
            case 13:
                memset(panel, 0, sizeof panel);
                *(uint8_t**)(panel + 0x88) = hasPanelWin ? win : nullptr;
                side ? Panel_SetValue90(panel, 0, pval) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4dd8a0)(panel, 0, pval);
                r = *(int32_t*)(panel + 0x90);
                break;
            case 14:
                r = (int32_t)(uintptr_t)(side ? Window_EnableControl7f9(win, 0, on) : ((uint8_t*(__fastcall*)(uint8_t*, int, uint8_t))0x4dd8d0)(win, 0, on));
                break;
            case 15: side ? NewGameDialog_ShowSetting77(win) : ((void(__fastcall*)(uint8_t*))0x4ed6d0)(win); break;
            case 16: side ? NewGameDialog_ShowSetting7b(win) : ((void(__fastcall*)(uint8_t*))0x4ed730)(win); break;
            case 17: side ? NewGameDialog_ShowDifficulty(win) : ((void(__fastcall*)(uint8_t*))0x4ed790)(win); break;
            case 18: side ? Window_HighlightTabs7And5(win) : ((void(__fastcall*)(uint8_t*))0x4f5600)(win); break;
            case 19: side ? Window_FillList759e(win) : ((void(__fastcall*)(uint8_t*))0x4fd8d0)(win); break;
            case 20: side ? Window_SelectOption755f(win) : ((void(__fastcall*)(uint8_t*))0x4fe6f0)(win); break;
            default: side ? Window_SelectOption7569(win) : ((void(__fastcall*)(uint8_t*))0x4fe730)(win); break;
            }
            res[side] = r;
            sent[side] = g_sent;
            globals[side][0] = hook::Global<uint32_t>(0x6d0804);
            globals[side][1] = hook::Global<uint32_t>(0x6cffc4);
            globals[side][2] = hook::Global<uint32_t>(0x6d07ac);
            globals[side][3] = hook::Global<uint32_t>(0x6cffb0);
        }
        // Text pointers sent by PickRow point into each side's buffer: compare contents.
        if (op >= 11)
            for (size_t k = 0; k + 12 < sent[0].size() && k + 12 < sent[1].size(); k += 13)
                if (sent[0][k + 3] == (uint32_t)(uintptr_t)buf[0]) sent[0][k + 3] = (uint32_t)(uintptr_t)buf[1];
        if (op == 15 || op == 16 || op == 19)
            for (size_t k = 0; k + 12 < sent[0].size() && k + 12 < sent[1].size(); k += 13) {
                EXPECT_EQ(strcmp(g_texts[0][k / 13].c_str(), g_texts[1][k / 13].c_str()), 0, "op %d text iteration %d", op, it);
                if (!g_texts[0][k / 13].empty()) sent[0][k + 3] = sent[1][k + 3] = 0;
            }
        EXPECT_EQ(sent[0] == sent[1], true, "op %d events iteration %d", op, it);
        EXPECT_EQ(res[0] == res[1] && !memcmp(globals[0], globals[1], sizeof globals[0]), true, "op %d result iteration %d", op, it);
        EXPECT_EQ(memcmp(buf[0], buf[1], 0x100), 0, "op %d buffer iteration %d", op, it);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    hook::Global<uint32_t>(0x6cec7c) = sNewGame;
    hook::Global<uint32_t>(0x6cec74) = sOpts;
    hook::Global<uint32_t>(0x6d401c) = sUi;
    Collection_Destruct(&cities);
}

namespace rt3 {
void Window_ShowMoney7149();
void __fastcall Window_HighlightViewTabs(uint8_t* w);
void __fastcall RulesDialog_UpdateButton(uint8_t* dlg);
void __fastcall Window_SetMode1(uint8_t* w, int, int32_t mode);
void __fastcall Picker_Hover90ec(int32_t idx, int32_t on);
void __fastcall Picker_Hover9376(int32_t idx, int32_t on);
void __fastcall Window_BalancePercentages(uint8_t* w);
void __fastcall OptionsDialog_ShowValues(uint8_t* w);
void __fastcall SoundDialog_PickProvider(int32_t idx);
void __fastcall Window_SetLargeLayout(uint8_t* w, int, int32_t large);
void __fastcall Window_UpdateListButtons(uint8_t* w);
void __fastcall Window_HighlightGridTabs(uint8_t* w);
void __fastcall RulesDialog_FillList(uint8_t* w);
void __fastcall NewGame_RefreshAiSlots(uint8_t* o);
void __fastcall OptionsDialog_ShowSliders(uint8_t* w);
void __fastcall OptionsDialog_ShowSwitches(uint8_t* w);
void StationDialog_UpdateButtons();
void __fastcall RulesDialog_DeleteRule(uint8_t* dlg);
void NewGame_CapAiSlots();
void __fastcall OptionsDialog_FillList7569(uint8_t* w);
void __fastcall EditorDialog_FillCargoList(uint8_t* w);
void __fastcall EditorDialog_ShowBriefings(uint8_t* w);
void CityDialog_ShowMapParameters();
void __fastcall RulesDialog_UpdateButtons4fed(uint8_t* dlg);
void __fastcall OptionsDialog_ShowSliders2(uint8_t* w);
void __fastcall NewGameDialog_StepPortrait(uint8_t* w, int, int32_t dir);
void __fastcall RulesDialog_UpdateButtons4ff7(uint8_t* w);
void __fastcall RulesDialog_FillTerritories(uint8_t* w);
void __fastcall OptionsDialog_FillList755f(uint8_t* w);
}

DIFFTEST(game_DialogSetupHelpers2) {
    static void* vtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    static uint8_t win[0x200], kids[4][0x200], view[0x100], opts[0x400], input[0xb00], snd[0x200], rules[0x900], table622770[0x18];
    FakeEngine fe;
    static Collection rulesTab;
    memset(&rulesTab, 0, sizeof rulesTab);
    Collection_Construct(&rulesTab, 0, 1, 0x900, 4, 4, 0, 1, 0);
    static uint8_t blank[0x900];
    for (int k = 0; k < 3; ++k) Collection_Allocate(&rulesTab, 0, blank, 0x900, 0, 0);
    static Collection stations, buildings;
    memset(&stations, 0, sizeof stations);
    memset(&buildings, 0, sizeof buildings);
    Collection_Construct(&stations, 0, 1, 0x40, 4, 4, 0, 0, 0);
    Collection_Construct(&buildings, 0, 1, 0x100, 4, 4, 0, 0, 0);
    for (int k = 0; k < 3; ++k) {
        Collection_Allocate(&stations, 0, blank, 0x40, 0, 0);
        Collection_Allocate(&buildings, 0, blank, 0x100, 0, 0);
    }
    static void* bvtbl[0x30];
    bvtbl[0x80 / 4] = (void*)&FakeBuildingStateObj;
    static uint8_t newGame[0x100], players[0x400], world[0x7000], kid4e8f[0x200];
    static Collection regionsTab;
    memset(&regionsTab, 0, sizeof regionsTab);
    Collection_Construct(&regionsTab, 0, 1, 0x80, 4, 4, 0, 0, 0);
    for (int k = 0; k < 4; ++k) Collection_Allocate(&regionsTab, 0, blank, 0x80, 0, 0);
    memset(kid4e8f, 0, sizeof kid4e8f);
    *(uint16_t*)(kid4e8f + 4) = 0x4e8f;
    static uint8_t kid5a7e[0x200], kid4ff6[0x200];
    memset(kid5a7e, 0, sizeof kid5a7e);
    memset(kid4ff6, 0, sizeof kid4ff6);
    *(uint16_t*)(kid5a7e + 4) = 0x5a7e;
    *(uint16_t*)(kid4ff6 + 4) = 0x4ff6;
    static Collection playersTab;
    memset(&playersTab, 0, sizeof playersTab);
    Collection_Construct(&playersTab, 0, 1, 0x300, 4, 4, 0, 0, 0);
    for (int k = 0; k < 2; ++k) Collection_Allocate(&playersTab, 0, blank, 0x300, 0, 0);
    static uint8_t portraits[0x29 * 0x27];
    memcpy(portraits, (void*)0x5f2d28, sizeof portraits);
    const uint32_t addrs[] = { 0x6ceb9c, 0x6d4100, 0x621f50, 0x6cff94, 0x6cfc9c, 0x6cff84, 0x6cec7c, 0x6d1270, 0x6d1268, 0x6cd914, 0x6cec20, 0x62b26c, 0x6d16d8, 0x6cec18, 0x6cec14, 0x6cec10, 0x6d0818, 0x62be18, 0x6d40dc, 0x6d12f0, 0x6d12d4, 0x6d12d0, 0x6d4018, 0xccba14,
                               0x6d1330, 0x6cec74, 0x6d135d, 0x6d402c, 0x6d1350, 0xccb9fc, 0x6cec78, 0x6d3b28, 0x6d3b24 };
    uint32_t saved[sizeof addrs / 4];
    for (size_t k = 0; k < sizeof addrs / 4; ++k) saved[k] = hook::Global<uint32_t>(addrs[k]);
    memcpy(table622770, (void*)0x622770, sizeof table622770);
    // A temp directory holding Data\Configuration for Engine_SaveConfig.
    char cwd[MAX_PATH], tmp[MAX_PATH], dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, cwd);
    GetTempPathA(MAX_PATH, tmp);
    strcat(tmp, "rt3re_cfgtest");
    CreateDirectoryA(tmp, nullptr);
    sprintf(dir, "%s\\Data", tmp);
    CreateDirectoryA(dir, nullptr);
    sprintf(dir, "%s\\Data\\Configuration", tmp);
    CreateDirectoryA(dir, nullptr);
    memset(win, 0, sizeof win);
    *(void***)win = vtbl;
    win[0x11] = 1;
    *(uint8_t**)(win + 0x70) = kids[0];
    const uint16_t kidIds[4] = { 0x5019, 0x5002, 0x65a4, 0x65a3 };
    for (int k = 0; k < 4; ++k) {
        memset(kids[k], 0, 0x200);
        *(uint16_t*)(kids[k] + 4) = kidIds[k];
        *(uint8_t**)(kids[k] + 0x8e) = k + 1 < 4 ? kids[k + 1] : nullptr;
    }
    hook::Global<uint8_t*>(0x6cec18) = win;
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<Collection*>(0x62be18) = &rulesTab;
    hook::Global<uint8_t*>(0x6d4018) = input;
    hook::Global<int32_t>(0xccba14) = 1; // muted UI sounds
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<uint8_t*>(0x6d402c) = snd;
    hook::Global<uint8_t*>(0x6d1350) = win;
    hook::Global<void*>(0xccb9fc) = nullptr;
    hook::Global<void*>(0x6cec78) = nullptr;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6d1268) = win;
    hook::Global<uint8_t*>(0x6d16d8) = win;
    hook::Global<Collection*>(0x6cec20) = &stations;
    hook::Global<Collection*>(0x62b26c) = &buildings;
    for (int it = 0; it < 2900; ++it) {
        int op = it % 29;
        *(uint8_t**)(kids[3] + 0x8e) = kid4e8f;
        *(uint8_t**)(kid4e8f + 0x8e) = kid5a7e;
        *(uint8_t**)(kid5a7e + 0x8e) = kid4ff6;
        *(int32_t*)(kid5a7e + 0x4f) = (int32_t)(Rnd() % 0x29);
        *(int32_t*)(kid4ff6 + 0xcc) = (int32_t)(Rnd() % 2);
        hook::Global<Collection*>(0x6ceb9c) = &playersTab;
        uint8_t portraitInit[0x29];
        bool allTaken = Rnd() % 8 == 0;
        for (uint8_t& b : portraitInit) b = allTaken || Rnd() % 2 ? 1 : 0;
        int32_t portraitDir = (int32_t)(Rnd() % 5) - 2;
        *(int32_t*)(kid4e8f + 0xcc) = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d4100) = (int32_t)(Rnd() % 8);
        hook::Global<int32_t>(0x621f50) = (int32_t)(Rnd() % 2);
        hook::Global<Collection*>(0x6cfc9c) = &regionsTab;
        hook::Global<uint8_t*>(0x6cff84) = Rnd() % 4 ? win : nullptr;
        for (int k = 1; k <= 4; ++k) ((uint8_t*)Collection_Get(&regionsTab, 0, k))[0x3d] = (uint8_t)(Rnd() % 3);
        for (int k = 0; k < 0x40; ++k) world[0x6987 + k] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(world + 0x6a6c) = (int32_t)(Rnd() % 9) - 2;
        sprintf((char*)world + 0x4f30, "Briefing %u", Rnd() % 100);
        sprintf((char*)world + 0x5ae9, "MP briefing %u", Rnd() % 100);
        int32_t guard = Rnd() % 6 == 0;
        if (op == 17) *(int32_t*)(win + 0x84) = Rnd() % 4 ? 1 + (int32_t)(Rnd() % 3) : -1; // a real rule id (or none)

        for (int k = 0x78; k <= 0x90; k += 4)
            if (k != 0x84) *(int32_t*)(win + k) = (int32_t)(Rnd() % 30);
        *(int32_t*)(win + 0x84) = Rnd() % 4 ? 1 + (int32_t)(Rnd() % 3) : -1;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* r = (uint8_t*)Collection_Get(&rulesTab, 0, k);
            *(int32_t*)(r + 0x14) = (int32_t)(Rnd() % 50);
            sprintf((char*)r + 0x18, "Rule %d-%u", k, Rnd() % 100);
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, k);
            *(uint32_t*)(st + 4) = 1 + Rnd() % 3;
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            *(void***)b = bvtbl;
            b[0x10] = (uint8_t)(Rnd() % 3);
        }
        for (auto& s : g_stState) s[0x8c] = (uint8_t)(Rnd() % 3);
        *(uint32_t*)(view + 0x90) = Rnd() % 5;
        uint8_t ngInit[0x100];
        for (uint8_t& b : ngInit) b = (uint8_t)Rnd();
        *(uint16_t*)(ngInit + 0x79) = (uint16_t)(Rnd() % 6);
        *(uint16_t*)(ngInit + 0x7b) = (uint16_t)(Rnd() % 6);
        for (int k = 0; k < 16; ++k) ngInit[0x87 + k] = Rnd() % 3 ? 0 : (uint8_t)(Rnd() % 120 + 90);
        for (uint8_t& b : players) b = (uint8_t)(Rnd() % 2);
        *(int32_t*)(players + 0x3ae) = (int32_t)(Rnd() % 12);
        *(int32_t*)(players + 0x3be) = (int32_t)(Rnd() % 12);
        hook::Global<int32_t>(0x6cd914) = Rnd() % 6 == 0;
        bool useDefault = Rnd() % 2 != 0;
        hook::Global<int32_t>(0x6cec14) = (int32_t)Rnd() % 9000000;
        hook::Global<int32_t>(0x6cec10) = (int32_t)Rnd() % 9000000;
        *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 8);
        *(int32_t*)(win + 0x7c) = (int32_t)(Rnd() % 8);
        *(int32_t*)(view + 0x8c) = 0xc + (int32_t)(Rnd() % 10);
        *(int32_t*)(win + 0x84) = Rnd() % 4 ? 1 : -1;
        *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
        for (int k = 0; k < 3; ++k) *(int32_t*)(kids[k] + 0xcc) = (int32_t)(Rnd() % 2);
        *(int32_t*)(kids[2] + 0x1d6) = (int32_t)(Rnd() % 3) - 1;
        *(int32_t*)(kids[3] + 0x1d6) = (int32_t)(Rnd() % 3) - 1;
        hook::Global<int32_t>(0x6d40dc) = (int32_t)(Rnd() % 2);
        hook::Global<uint8_t*>(0x6d12f0) = win; // only the hold-to-arm path checks it for null
        *(uint32_t*)(input + 0xa8c) = Rnd() % 3 ? Rnd() & 0x407 : 0;
        for (int k = 0; k < 0x30; k += 4) *(uint32_t*)(opts + 0x114 + k) = Rnd() % 4;
        uint8_t* rec = (uint8_t*)Collection_Get(&rulesTab, 0, 1);
        for (int k = 0; k < 0x900; ++k) rec[k] = (uint8_t)Rnd();
        // Condition lists (+4.., next +0x24) for Obj_ListFlags.
        static uint8_t condNodes[3][2][0x30];
        for (int l = 0; l < 3; ++l) {
            for (int n = 0; n < 2; ++n) {
                memset(condNodes[l][n], 0, 0x30);
                *(int32_t*)condNodes[l][n] = (int32_t)(Rnd() % 60);
                *(uint8_t**)(condNodes[l][n] + 0x24) = n == 0 && Rnd() % 2 ? condNodes[l][1] : nullptr;
            }
            *(uint8_t**)(rec + 4 + l * 4) = Rnd() % 4 ? condNodes[l][0] : nullptr;
        }
        sprintf((char*)rec + 0x18, "Rule 1-%u", Rnd() % 100); // the random fill above overwrote the name
        int32_t mode = (int32_t)(Rnd() % 3), idx = (int32_t)(Rnd() % 6) - 1, on = (int32_t)(Rnd() % 2);
        int32_t pct[3] = { (int32_t)(Rnd() % 120), (int32_t)(Rnd() % 120), (int32_t)(Rnd() % 120) };
        int32_t hover0 = Rnd() % 3 ? (int32_t)(Rnd() % 4) : -1;
        uint8_t busy = (uint8_t)(Rnd() % 4 == 0);
        hook::Global<int32_t>(0x6d3b24) = (int32_t)(Rnd() % 2);
        std::vector<uint32_t> sent[2];
        uint32_t state[2][12];
        std::string cfg[2];
        static uint8_t ruleSnap[3][0x900];
        for (int k = 0; k < 3; ++k) memcpy(ruleSnap[k], Collection_Get(&rulesTab, 0, k + 1), 0x900);
        static uint8_t winSnap[0x200];
        memcpy(winSnap, win, sizeof winSnap);
        for (int side = 0; side < 2; ++side) {
            g_sent.clear();
            g_textSide = side;
            g_texts[side].clear();
            hook::Global<int32_t>(0x6d12d4) = hover0;
            hook::Global<int32_t>(0x6d12d0) = 7;
            hook::Global<uint8_t>(0x6d135d) = busy;
            for (int k = 0; k < 3; ++k) {
                *(int32_t*)(win + 0xd8 + k * 8) = 0x30 + k;
                *(int32_t*)(win + 0xdc + k * 8) = pct[k];
            }
            memset((void*)0x622770, 0, 0x18);
            memcpy(newGame, ngInit, sizeof ngInit);
            memcpy(win, winSnap, sizeof winSnap);
            if (op == 25) {
                *(int32_t*)(win + 0x80) = portraitDir & 3; // row: players 1..2 exist, 3 does not
                for (int k = 0; k < 0x29; ++k) ((uint8_t*)0x5f2d28)[k * 0x27] = portraitInit[k];
                for (int k = 1; k <= 2; ++k) memset(Collection_Get(&playersTab, 0, k), 0x5a, 0x300);
                memset(world + 0x69d8, 0, 0x30);
            }
            hook::Global<uint8_t*>(0x6cec78) = op == 8 ? nullptr : world;
            hook::Global<int32_t>(0x6cff94) = guard;
            hook::Global<uint8_t*>(0x6d1270) = players;
            *(int32_t*)(fe.e + 0x114706) = -5;
            switch (op) {
            case 0: side ? Window_ShowMoney7149() : ((void(*)())0x47c190)(); break;
            case 1: side ? Window_HighlightViewTabs(win) : ((void(__fastcall*)(uint8_t*))0x4bc290)(win); break;
            case 2: side ? RulesDialog_UpdateButton(win) : ((void(__fastcall*)(uint8_t*))0x4d9d10)(win); break;
            case 3: side ? Window_SetMode1(win, 0, mode) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4ee430)(win, 0, mode); break;
            case 4: side ? Picker_Hover90ec(idx, on) : ((void(__fastcall*)(int32_t, int32_t))0x4f3b90)(idx, on); break;
            case 5: side ? Picker_Hover9376(idx, on) : ((void(__fastcall*)(int32_t, int32_t))0x4f3c90)(idx, on); break;
            case 6: side ? Window_BalancePercentages(win) : ((void(__fastcall*)(uint8_t*))0x4f5df0)(win); break;
            case 7: side ? OptionsDialog_ShowValues(win) : ((void(__fastcall*)(uint8_t*))0x4fdf20)(win); break;
            case 8: {
                SetCurrentDirectoryA(tmp);
                char cfgPath[MAX_PATH];
                sprintf(cfgPath, "%s\\Data\\Configuration\\engine.cfg", tmp);
                DeleteFileA(cfgPath);
                side ? SoundDialog_PickProvider(idx) : ((void(__fastcall*)(int32_t))0x4fe1b0)(idx);
                SetCurrentDirectoryA(cwd);
                if (FILE* f = fopen(cfgPath, "rb")) {
                    int c;
                    while ((c = fgetc(f)) != EOF) cfg[side] += (char)c;
                    fclose(f);
                }
                break;
            }
            case 9: side ? Window_SetLargeLayout(win, 0, on) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x512f80)(win, 0, on); break;
            case 10: side ? Window_UpdateListButtons(win) : ((void(__fastcall*)(uint8_t*))0x513720)(win); break;
            case 11: side ? Window_HighlightGridTabs(win) : ((void(__fastcall*)(uint8_t*))0x4bccd0)(win); break;
            case 12: side ? RulesDialog_FillList(win) : ((void(__fastcall*)(uint8_t*))0x4d7ef0)(win); break;
            case 13: {
                uint8_t* o = useDefault ? nullptr : players;
                side ? NewGame_RefreshAiSlots(o) : ((void(__fastcall*)(uint8_t*))0x4ed940)(o);
                break;
            }
            case 14: side ? OptionsDialog_ShowSliders(win) : ((void(__fastcall*)(uint8_t*))0x4fd650)(win); break;
            case 15: side ? OptionsDialog_ShowSwitches(win) : ((void(__fastcall*)(uint8_t*))0x4fd940)(win); break;
            case 16: side ? StationDialog_UpdateButtons() : ((void(*)())0x5044b0)(); break;
            case 17: {
                // Deleting reorders the table: give each side its own copy.
                static Collection sideRules;
                memset(&sideRules, 0, sizeof sideRules);
                Collection_Construct(&sideRules, 0, 1, 0x900, 4, 4, 0, 1, 0);
                for (int k = 0; k < 3; ++k) Collection_Allocate(&sideRules, 0, ruleSnap[k], 0x900, 0, 0);
                hook::Global<Collection*>(0x62be18) = &sideRules;
                side ? RulesDialog_DeleteRule(win) : ((void(__fastcall*)(uint8_t*))0x4d9360)(win);
                hook::Global<Collection*>(0x62be18) = &rulesTab;
                Collection_Destruct(&sideRules);
                break;
            }
            case 18: side ? NewGame_CapAiSlots() : ((void(*)())0x4edbb0)(); break;
            case 19: side ? OptionsDialog_FillList7569(win) : ((void(__fastcall*)(uint8_t*))0x4febd0)(win); break;
            case 20: side ? EditorDialog_FillCargoList(win) : ((void(__fastcall*)(uint8_t*))0x4ca540)(win); break;
            case 21: side ? EditorDialog_ShowBriefings(win) : ((void(__fastcall*)(uint8_t*))0x4ca670)(win); break;
            case 22: side ? CityDialog_ShowMapParameters() : ((void(*)())0x4d76a0)(); break;
            case 23: side ? RulesDialog_UpdateButtons4fed(win) : ((void(__fastcall*)(uint8_t*))0x4d77b0)(win); break;
            case 24: side ? OptionsDialog_ShowSliders2(win) : ((void(__fastcall*)(uint8_t*))0x4fd550)(win); break;
            case 25: side ? NewGameDialog_StepPortrait(win, 0, portraitDir) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4ceb90)(win, 0, portraitDir); break;
            case 26: side ? RulesDialog_UpdateButtons4ff7(win) : ((void(__fastcall*)(uint8_t*))0x4d5f50)(win); break;
            case 27: side ? RulesDialog_FillTerritories(win) : ((void(__fastcall*)(uint8_t*))0x4d7fe0)(win); break;
            default: side ? OptionsDialog_FillList755f(win) : ((void(__fastcall*)(uint8_t*))0x4feb20)(win); break;
            }
            if (op == 25) {
                static uint8_t portraitOut[2][0x29], playerOut[2][2][0x300], worldOut[2][0x30];
                for (int k = 0; k < 0x29; ++k) portraitOut[side][k] = ((uint8_t*)0x5f2d28)[k * 0x27];
                for (int k = 0; k < 2; ++k) memcpy(playerOut[side][k], Collection_Get(&playersTab, 0, k + 1), 0x300);
                memcpy(worldOut[side], world + 0x69d8, 0x30);
                if (side) {
                    EXPECT_EQ(memcmp(portraitOut[0], portraitOut[1], 0x29), 0, "portrait table iteration %d", it);
                    EXPECT_EQ(memcmp(playerOut[0], playerOut[1], sizeof playerOut[0]), 0, "portrait players iteration %d", it);
                    EXPECT_EQ(memcmp(worldOut[0], worldOut[1], 0x30), 0, "portrait world iteration %d", it);
                }
            }
            sent[side] = g_sent;
            state[side][0] = hook::Global<uint32_t>(0x6d12d4);
            state[side][1] = hook::Global<uint32_t>(0x6d12d0);
            state[side][2] = *(uint32_t*)(win + 0xe4);
            state[side][3] = *(uint32_t*)(win + 0xec);
            memcpy(&state[side][4], (void*)0x622770, 0x18);
            state[side][10] = hook::Global<uint32_t>(0x6d3b28);
            state[side][11] = *(uint32_t*)(fe.e + 0x114706) ^ *(uint32_t*)(win + 0x84) ^ (*(uint32_t*)(win + 0x90) << 8);
            static uint8_t ngOut[2][0x100];
            memcpy(ngOut[side], newGame, 0x100);
            if (side) EXPECT_EQ(memcmp(ngOut[0], ngOut[1], 0x100), 0, "op %d new game iteration %d", op, it);
        }
        if (op == 0 || op == 10 || op == 12 || op == 13 || op == 17 || op == 18 || op == 19 || op == 20 || op == 21 || op == 22 || op >= 25)
            for (size_t k = 0; k + 12 < sent[0].size() && k + 12 < sent[1].size(); k += 13) {
                EXPECT_EQ(strcmp(g_texts[0][k / 13].c_str(), g_texts[1][k / 13].c_str()), 0, "op %d text iteration %d: '%s' vs '%s'", op, it,
                          g_texts[0][k / 13].c_str(), g_texts[1][k / 13].c_str());
                uint32_t type = sent[0][k + 1];
                if (!g_texts[0][k / 13].empty() || type == 0x88 || type == 0x6f || type == 0x6e)
                    sent[0][k + 3] = sent[1][k + 3] = sent[0][k + 5] = sent[1][k + 5] = 0;
            }
        EXPECT_EQ(sent[0] == sent[1], true, "op %d events iteration %d", op, it);
        EXPECT_EQ(memcmp(state[0], state[1], sizeof state[0]), 0, "op %d state iteration %d", op, it);
        EXPECT_EQ(cfg[0] == cfg[1], true, "op %d config iteration %d", op, it);
    }
    for (size_t k = 0; k < sizeof addrs / 4; ++k) hook::Global<uint32_t>(addrs[k]) = saved[k];
    memcpy((void*)0x622770, table622770, sizeof table622770);
    Collection_Destruct(&rulesTab);
    Collection_Destruct(&stations);
    Collection_Destruct(&buildings);
    Collection_Destruct(&regionsTab);
    Collection_Destruct(&playersTab);
    memcpy((void*)0x5f2d28, portraits, sizeof portraits);
}

namespace rt3 {
void __fastcall EditorDialog_ShowSettings(uint8_t* w);
void CityDialog_ShowSummary();
void __fastcall Window_HighlightViewModes(uint8_t* w);
void __fastcall RulesDialog_ShowLimits(uint8_t* w);
void MapSizeDialog_Show();
int32_t __fastcall NewGame_ClaimSlot(int, int32_t mode);
void __fastcall TerritoryDialog_Show(uint8_t* w);
void __fastcall OptionsDialog_ShowGeneral(uint8_t* w);
void __fastcall OptionsDialog_ShowLists7587(uint8_t* w);
void __fastcall BuildingDialog_ShowTabs(uint8_t* w);
void __fastcall BuildingDialog_ShowPrice(uint8_t* w);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Map_ShowPickTooltip(uint8_t* o);
void __fastcall NewGameDialog_ShowRow(uint8_t* w, int, int32_t row);
void Window_HighlightViewModesIfOpen();
void __fastcall CityEditor_Show(uint8_t* w);
void __fastcall OptionsDialog_ShowGraphics(uint8_t* w);
void __fastcall OptionsDialog_ShowEngine(uint8_t* w);
void __fastcall OptionsDialog_ShowDisplay(uint8_t* w);
void __fastcall BuildingDialog_Select(uint8_t* w, int, int32_t id);
void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
void __fastcall Widget_OnLeave(uint8_t* w);
void __fastcall MoneySlider_Setup(int32_t reset);
void __fastcall IndustryView_UpdateButtons(uint8_t* w);
void __fastcall MpNewGameDialog_Show(uint8_t* w);
void __fastcall Widget_OnHoverTimed(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
void __fastcall EditorDialog_ShowAchievements(uint8_t* w, int, int32_t keep);
int32_t __fastcall EditorDialog_OnSettingsEvent(uint8_t* w, int, uint32_t* ev);
int32_t __fastcall EditorDialog_OnCargoEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Window_HighlightMapModes(uint8_t* w);
void __fastcall OptionsDialog_ShowSound(uint8_t* w);
void __fastcall OptionsDialog_FillResolutions(uint8_t* w);
void __fastcall TrainList_UpdateButtons(uint8_t* w);
void __stdcall Territory_GiveToOwner(int32_t* owner, int32_t* filter, int32_t kind);
void __fastcall NewGameDialog_CountRows(uint8_t* w);
void __fastcall RulesDialog_FillConditionTypes(uint8_t* w, int, int32_t sel);
void __fastcall RulesDialog_ShowEffect(uint8_t* w);
void __fastcall StationPicker_Fill(uint8_t* w);
void __fastcall MpSetupDialog_Show(uint8_t* w);
void __fastcall StationView_CargoTooltip(int32_t id, int, int32_t, int32_t);
void __fastcall CompanyList_Fill(uint8_t* w);
void __fastcall TrainOrders_CargoTooltip(int32_t id, int, int32_t, int32_t);
void __fastcall TrainOrders_UpdateButtons(uint8_t* w);
void __fastcall EditorDialog_ShowEconomy(uint8_t* w);
void __fastcall MoneySliders_Read(int32_t rerange);
int32_t __fastcall EditorDialog_OnAchievementsEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall NewGameDialog_ListHumanRows(uint8_t* w);
void __fastcall TrainList_Refill(uint8_t* w);
void __fastcall OptionsDialog_ShowVideo(uint8_t* w);
void __fastcall TrainPicker_Fill(uint8_t* w, int, uint32_t exclude);
void __fastcall RulesDialog_UpdateScope(uint8_t* w);
void __fastcall RulesDialog_FillConditions(uint8_t* w);
int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
int32_t __fastcall ConditionDialog_Store(uint8_t* w);
void __fastcall RulesDialog_UpdateSlot(uint8_t* w);
void __fastcall TerritoryEditor_Show(uint8_t* w, int, int32_t id);
void __fastcall EditorDialog_FillIndustries(uint8_t* w, int, int32_t keep);
void __fastcall EditorDialog_FillHouses(uint8_t* w, int, int32_t keep);
void __fastcall NewGameDialog_SetupRows(uint8_t* w);
void __fastcall RulesDialog_DeleteCondition(uint8_t* w);
void __fastcall RulesDialog_ShowSlot(uint8_t* w);
void __fastcall RulesDialog_SwitchPage(uint8_t* w);
void __fastcall RulesDialog_ShowMessageKind(uint8_t* w);
void __fastcall CityEditor_Fill(uint8_t* w, int, int32_t id, int32_t keep);
void __fastcall EditorDialog_ShowRegionTypes(uint8_t* w);
void __fastcall ConditionDialog_ShowValue(uint8_t* w);
void __fastcall RulesDialog_ShowEffectType(uint8_t* w);
void __fastcall RulesDialog_FillTriggers(uint8_t* w);
int32_t __fastcall FacePicker_OnClick(uint8_t* w, uint32_t* ev);
void __fastcall RulesDialog_DeleteTrigger(uint8_t* w);
void __fastcall RulesDialog_ShowMessage(uint8_t* w);
void __fastcall RulesDialog_Reset(uint8_t* w);
}

namespace {
uint32_t g_cbLog[16];
void __fastcall FakeMoveCb(uint32_t id, void*, float x, float y) {
    g_cbLog[0] = 1;
    g_cbLog[1] = id;
    memcpy(&g_cbLog[2], &x, 4);
    memcpy(&g_cbLog[3], &y, 4);
}
void __fastcall FakeDragCb(uint32_t id, uint32_t hovered, int32_t a, uint32_t x, uint32_t y, int32_t b, int32_t c) {
    g_cbLog[4] = 1;
    g_cbLog[5] = id;
    g_cbLog[6] = hovered;
    g_cbLog[7] = (uint32_t)a;
    g_cbLog[8] = x;
    g_cbLog[9] = y;
    g_cbLog[10] = (uint32_t)b;
    g_cbLog[11] = (uint32_t)c;
}
int g_nProviders;
int32_t __fastcall FakeIsPassenger(uint8_t* b) { return b[0x11] & 1; }
int32_t __stdcall FakeEnum3DProviders(uint32_t* next, uint32_t* provider, const char** name) {
    static const char* names[] = { "Miles Fast 2D", "DirectSound3D", "EAX" };
    if ((int)*next >= g_nProviders) return 0;
    *provider = 0x100 + *next;
    *name = names[*next % 3];
    ++*next;
    return 1;
}
} // namespace

DIFFTEST(game_DialogSetupHelpers3) {
    static void* vtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    static uint8_t win[0x200], trees[2][0x200], treeNodes[4][32], world[0x46d00], view[0x100], rule[0x40], setup[0x400], newGame[0x100];
    static uint8_t opts[0x400], map[0x2200], tiles[8 * 8], cells[9 * 9 * 3], ui[0xd00], capHolder[0xd00];
    FakeEngine fe;
    static uint8_t blank[0xe00];
    static Collection stations, buildings, nodes, trainsTab, trackStations, cargoTypes, rules3, indList, indTypes, namedTab, cityNamesEmpty, packFaces, packLogos;
    static Collection cities, cityNames, regions, types, smallTypes, companies, players;
    struct TabDef {
        Collection* c;
        uint32_t size;
        int n;
    } tabs[] = { { &cities, 0x400, 24 }, { &cityNames, 0x40, 2 }, { &regions, 0x500, 24 }, { &types, 0x200, 3 },
                 { &smallTypes, 0x800, 3 }, { &companies, 0xe00, 3 }, { &cargoTypes, 0x60, 4 }, { &rules3, 0x900, 3 }, { &indList, 0x80, 5 }, { &indTypes, 0x120, 4 }, { &namedTab, 0x48, 3 }, { &cityNamesEmpty, 0x40, 0 }, { &players, 0xd00, 2 }, { &packFaces, 0x80, 3 }, { &packLogos, 0x80, 2 },
                 { &stations, 0x600, 3 }, { &buildings, 0x400, 3 }, { &nodes, 0x260, 3 }, { &trainsTab, 0x60, 3 }, { &trackStations, 0x40, 3 } };
    for (TabDef& t : tabs) {
        memset(t.c, 0, sizeof(Collection));
        Collection_Construct(t.c, 0, 1, t.size, 4, 4, 0, 0, 0);
        for (int k = 0; k < t.n; ++k) Collection_Allocate(t.c, 0, blank, t.size, 0, 0);
    }
    const uint32_t addrs[] = { 0x6cec78, 0x6cff94, 0x621f50, 0x62bae0, 0x6cff74, 0x6cff84, 0x6d0818, 0x6d10bc, 0x6d10b4, 0x6d10b8,
                               0x6d1270, 0x6cec7c, 0x6d1268, 0x6cd970, 0x6cd914, 0x6cfc9c, 0x62c120, 0x6cec74, 0x622af4, 0x6d1734,
                               0x6d1728, 0x62b2fc, 0x62b268, 0x6d1738, 0x6d173c, 0x62be10, 0x6ceb9c, 0x6d401c, 0x6d3d14, 0x6cec20, 0x62b26c, 0x6cfca8, 0x6d07b4, 0x6d4020, 0xccba14, 0x6cec18, 0x6cec14, 0x6cec10, 0x6cebcc, 0x6cebd0, 0x5f2c50, 0x6d14a8, 0x6d402c, 0x6d3ae8, 0x6d3aec, 0x6d3ae0, 0x6cfcbc, 0x6cfcb4, 0x6d07c8, 0x62ba84, 0x622ae4, 0x6d14bc, 0x62ba8c, 0x6cfe04, 0x6cfe08, 0x621df4, 0x6cff9c, 0x6cebd4, 0x6cec0c, 0x5f2c4c, 0x622b28, 0xccbb20, 0x622b2c, 0x6d1350, 0x62be18, 0x6d07d0, 0x6d07d4, 0x621f48, 0x621f4c, 0x6ada84, 0x62ba88, 0x6cff78, 0x6cff98, 0x6cffa0, 0x6cffa4, 0x6d07cc, 0x6d07c0, 0x6d10a8, 0x622074 };
    uint32_t saved[sizeof addrs / 4];
    for (size_t k = 0; k < sizeof addrs / 4; ++k) saved[k] = hook::Global<uint32_t>(addrs[k]);
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    memset(win, 0, sizeof win);
    *(void***)win = vtbl;
    win[0x11] = 1;
    *(uint8_t**)(win + 0x70) = trees[0];
    for (int k = 0; k < 2; ++k) {
        memset(trees[k], 0, 0x200);
        *(uint16_t*)(trees[k] + 4) = (uint16_t)(0x69df + k * 4);
        *(uint8_t**)(trees[k] + 0x8e) = k ? nullptr : trees[1];
        *(uint8_t**)(trees[k] + 0x176) = treeNodes[0];
    }
    *(uint8_t**)(win + 0x78) = rule;
    *(Collection**)((uint8_t*)Collection_Get(&cities, 0, 1) + 0x37f) = &cityNames;
    *(Collection**)((uint8_t*)Collection_Get(&cities, 0, 2) + 0x37f) = &cityNames;
    *(int32_t*)(map + 0x2155) = 8;
    *(int32_t*)(map + 0x2159) = 8;
    *(int32_t*)(map + 0x15d9) = 9;
    *(int32_t*)(map + 0x15d5) = 9;
    *(int32_t*)(map + 0x15dd) = 9;
    *(uint8_t**)(map + 0x2131) = tiles;
    *(uint8_t**)(map + 0x165d) = cells;
    *(void**)(map + 0x167d) = nullptr;
    memset(capHolder, 0, sizeof capHolder);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<Collection*>(0x62bae0) = &cities;
    hook::Global<uint8_t*>(0x6d0818) = view;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    hook::Global<uint8_t*>(0x6d1268) = win;
    hook::Global<void*>(0x6cd970) = nullptr;
    hook::Global<Collection*>(0x6cfc9c) = &regions;
    hook::Global<uint8_t*>(0x62c120) = map;
    hook::Global<uint8_t*>(0x6cec74) = opts;
    hook::Global<Collection*>(0x62b2fc) = &types;
    hook::Global<Collection*>(0x62b268) = &smallTypes;
    hook::Global<Collection*>(0x62be10) = &companies;
    hook::Global<Collection*>(0x6ceb9c) = &players;
    hook::Global<uint8_t*>(0x6d401c) = capHolder;
    hook::Global<uint32_t>(0x6d3d14) = 0;
    hook::Global<Collection*>(0x6cec20) = &stations;
    hook::Global<Collection*>(0x62b26c) = &buildings;
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<int32_t>(0xccba14) = 1; // muted UI sounds
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    *(Collection**)(pack + 0x429b4) = &packFaces;
    *(Collection**)(pack + 0x429b0) = &packLogos;
    static uint8_t faceDlg[0x100];
    hook::Global<uint8_t*>(0x6d10a8) = faceDlg;
    static uint8_t stStateSaved[sizeof g_stState];
    memcpy(stStateSaved, g_stState, sizeof g_stState); // shared with other tests
    static void* bvtbl[0x30];
    bvtbl[0x80 / 4] = (void*)&FakeBuildingStateObj;
    static uint8_t blocks[2 * 2 * 4], picked[0x200], widget[0x100], parentW[0x40];
    *(int32_t*)(map + 0x214d) = 2;
    *(int32_t*)(map + 0x2151) = 2;
    *(uint8_t**)(map + 0x212d) = blocks;
    void** enumIat = (void**)0x5c8454;
    DWORD enumProt;
    VirtualProtect(enumIat, 4, PAGE_READWRITE, &enumProt);
    void* savedEnum = *enumIat;
    *enumIat = (void*)&FakeEnum3DProviders;
    static uint8_t sndSys[0x200], modes[16 * 0x15], kid2aff[0x200], trainNodes[4][32], trains[3][0x33];
    memset(sndSys, 0, sizeof sndSys);
    hook::Global<uint8_t*>(0x6d402c) = sndSys; // not 3D: no active provider, flags or speakers
    memset(kid2aff, 0, sizeof kid2aff);
    *(uint16_t*)(kid2aff + 4) = 0x2aff;
    *(uint8_t**)(kid2aff + 0x176) = trainNodes[0];
    *(uint8_t**)(trees[1] + 0x8e) = kid2aff;
    hook::Global<uint8_t*>(0x6d3ae8) = trains[0];
    hook::Global<Collection*>(0x6cfcbc) = &trainsTab;
    hook::Global<Collection*>(0x6cfcb4) = &trackStations;
    hook::Global<Collection*>(0x62ba84) = &cities; // train stops (positions +0x1e2 / +0x1ea)
    // Trains get a second order (list 1), as the picker walks it.
    Collection_Destruct(&trainsTab);
    memset(&trainsTab, 0, sizeof trainsTab);
    Collection_Construct(&trainsTab, 0, 1, 0x60, 4, 4, 1, 0, 0);
    for (int k = 0; k < 3; ++k) Collection_Allocate(&trainsTab, 0, blank, 0x60, 0, 0);
    static uint8_t modeObj[0x40];
    hook::Global<uint8_t*>(0xccbb20) = modeObj;
    hook::Global<uint8_t*>(0x6d1350) = win;
    hook::Global<Collection*>(0x62be18) = &rules3;
    hook::Global<Collection*>(0x6ada84) = &indList;
    hook::Global<Collection*>(0x62ba88) = &indTypes;
    static uint8_t kid5001[0x200], kid4ec3[0x200], kid4eb8[0x200], ruleNodes[3][3][0x20];
    const uint16_t scopeIds[3] = { 0x5001, 0x4ec3, 0x4eb8 };
    uint8_t* scopeKids[3] = { kid5001, kid4ec3, kid4eb8 };
    for (int k = 0; k < 3; ++k) {
        memset(scopeKids[k], 0, 0x200);
        *(uint16_t*)(scopeKids[k] + 4) = scopeIds[k];
        *(uint8_t**)(scopeKids[k] + 0x8e) = k < 2 ? scopeKids[k + 1] : nullptr;
    }
    static uint8_t kid5033[0x200];
    memset(kid5033, 0, sizeof kid5033);
    *(uint16_t*)(kid5033 + 4) = 0x5033;
    *(uint8_t**)(kid2aff + 0x8e) = kid5033;
    static uint8_t kidce8[0x200];
    memset(kidce8, 0, sizeof kidce8);
    *(uint16_t*)(kidce8 + 4) = 0xce8;
    *(uint8_t**)(kid5033 + 0x8e) = kidce8;
    static uint8_t kid714f[0x200], kid7150[0x200];
    memset(kid714f, 0, sizeof kid714f);
    memset(kid7150, 0, sizeof kid7150);
    *(uint16_t*)(kid714f + 4) = 0x714f;
    *(uint16_t*)(kid7150 + 4) = 0x7150;
    *(uint8_t**)(kidce8 + 0x8e) = kid714f;
    *(uint8_t**)(kid714f + 0x8e) = kid7150;
    *(uint8_t**)(kid7150 + 0x8e) = kid5001;
    // Rule-settings widgets (tick +0xcc, text +0xba).
    static const uint16_t ruleKidIds[] = { 0x4e8e, 0x4e8f, 0x4e90, 0x4e91, 0x4e92, 0x4eac, 0x4ed9, 0x4fdf, 0x4fe0, 0x4fe1, 0x4fe2,
                                           0x4edb, 0x4edd, 0x4e98, 0x4e99, 0x4e9a, 0x4e9b, 0x4e9c, 0x4e9d, 0x4e9e, 0x4e9f, 0x4ea0,
                                           0x4ea1, 0x4ea2, 0x4ead, 0x4eca, 0x4ecb, 0x4ecc, 0x500a, 0x503e, 0x5041, 0x5044, 0x5047, 0x4fc7, 0x4fce, 0x4ff6, 0x4ff9, 0x4ffc, 0x5002, 0x4ed5, 0x5014, 0x5015, 0x5016, 0x5017, 0x5018,
                                           0x5019, 0x501a, 0x501b, 0x501c, 0x5079, 0x4fc1, 0x4f61, 0x5029 };
    const int nRuleKids = (int)(sizeof ruleKidIds / sizeof ruleKidIds[0]);
    static uint8_t ruleKids[64][0x200], kidParent[0x80];
    static char ruleKidText[64][0x30];
    for (int k = 0; k < nRuleKids; ++k) {
        memset(ruleKids[k], 0, 0x200);
        *(uint16_t*)(ruleKids[k] + 4) = ruleKidIds[k];
        *(char**)(ruleKids[k] + 0xba) = ruleKidText[k];
        *(uint8_t**)(ruleKids[k] + 0x8e) = k + 1 < nRuleKids ? ruleKids[k + 1] : nullptr;
    }
    *(uint8_t**)(kid4eb8 + 0x8e) = ruleKids[0];
    hook::Global<Collection*>(0x62ba8c) = &cargoTypes;
    static uint8_t cargoFlagsSaved[0x40], regionGrid[64][0x1d7], orderList[8];
    memcpy(cargoFlagsSaved, (void*)0x6cfd38, sizeof cargoFlagsSaved);
    static uint8_t dswSaved[6];
    memcpy(dswSaved, (void*)0x4c6e4a, 6);
    *(uint8_t**)(map + 0x2129) = regionGrid[0];
    *(int32_t*)(map + 0x2145) = 8;
    *(uint8_t**)orderList = trains[0];
    *(int32_t*)(orderList + 4) = 3;
    hook::Global<uint8_t*>(0x6cfe08) = orderList;
    hook::Global<uint8_t*>(0x6cfe04) = trains[1];
    const char* tipTexts[] = { "Hello", "", "  ", "Other tip" };
    for (int it = 0; it < 14400; ++it) {
        int op = it % 72;
        // Shared random state.
        for (int k = 0x66ca; k < 0x66f4; ++k) world[k] = (uint8_t)Rnd();
        sprintf((char*)world + 0x672e, "Scenario %u", Rnd() % 100);
        *(int32_t*)(world + 0x4aef) = Rnd() % 3 == 0;
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(world + 0x8a + k * 4) = (int32_t)(Rnd() % 200) - 100;
        *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 3);
        *(int32_t*)(world + 0x25) = (int32_t)(Rnd() % 3);
        int32_t guard = Rnd() % 6 == 0;
        hook::Global<int32_t>(0x621f50) = (int32_t)(Rnd() % 2);
        for (int k = 1; k <= 2; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
            *(int32_t*)(c + 0x23e) = (int32_t)(Rnd() % 2);
            *(int32_t*)(c + 0x242) = (int32_t)(Rnd() % 5000);
        }
        hook::Global<uint32_t>(0x6cff74) = 1 + Rnd() % 2;
        hook::Global<uint8_t*>(0x6cff84) = win;
        *(int32_t*)(view + 0x8c) = (int32_t)(Rnd() % 30);
        for (uint8_t& b : rule) b = (uint8_t)Rnd();
        if (Rnd() % 3 == 0) rule[0x11] = 0;
        if (Rnd() % 3 == 0) *(uint16_t*)(rule + 0x16) = (uint16_t)(Rnd() % 6);
        hook::Global<uint8_t*>(0x6d10bc) = Rnd() % 5 ? win : nullptr;
        hook::Global<int32_t>(0x6d10b4) = (int32_t)(Rnd() % 1000);
        hook::Global<int32_t>(0x6d10b8) = (int32_t)(Rnd() % 1000);
        // New-game setup.
        uint8_t ngInit[0x100], setupInit[0x400];
        for (uint8_t& b : ngInit) b = (uint8_t)Rnd();
        *(uint16_t*)(ngInit + 0x79) = (uint16_t)(Rnd() % 6);
        *(uint16_t*)(ngInit + 0x7b) = (uint16_t)(Rnd() % 6);
        for (int k = 0; k < 16; ++k) ngInit[0x87 + k] = Rnd() % 3 ? 0 : (uint8_t)(Rnd() % 120 + 90);
        for (uint8_t& b : setupInit) b = (uint8_t)(Rnd() % 3 == 0);
        *(int32_t*)(setupInit + 0x3ae) = (int32_t)(Rnd() % 12);
        *(int32_t*)(setupInit + 0x3b6) = (int32_t)(Rnd() % 8);
        hook::Global<int32_t>(0x6cd914) = Rnd() % 6 == 0;
        bool noSetup = Rnd() % 6 == 0;
        int32_t mode = (int32_t)(Rnd() % 3);
        // Territories (ids 1..24; one group leader so no borders get built).
        for (uint8_t& t : tiles) t = (uint8_t)Rnd();
        for (uint8_t& c : cells) c = (uint8_t)(Rnd() % 3 == 0 ? Rnd() : 0);
        for (int k = 1; k <= 24; ++k) {
            uint8_t* r = (uint8_t*)Collection_Get(&regions, 0, k);
            *(int32_t*)r = k;
            sprintf((char*)r + 4, "Territory %d", k);
            *(int32_t*)(r + 0x31) = k == 1;
            *(int32_t*)(r + 0x35) = 1 + (int32_t)(Rnd() % 24);
            for (int j = 0; j < 0x40; ++j) *(int32_t*)(r + 0x3b5 + j * 4) = (int32_t)(Rnd() % 200) - 100;
        }
        *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 8);
        *(int32_t*)(win + 0x80) = (int32_t)(Rnd() % 26);
        if (op == 3) *(uint8_t**)(win + 0x78) = rule;
        // Options.
        for (int k = 0; k < 0x400; k += 4) *(uint32_t*)(opts + k) = Rnd() % 3;
        *(uint32_t*)(fe.e + 0x114742) = Rnd() % 2;
        // Building dialog.
        hook::Global<int32_t>(0x622af4) = Rnd() % 3 ? 5 : (int32_t)(Rnd() % 6);
        hook::Global<int32_t>(0x6d1734) = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d1728) = (int32_t)(Rnd() % 2);
        for (int k = 0; k < 2; ++k) *(int32_t*)(trees[k] + 0x1d6) = (int32_t)(Rnd() % 4) - 1;
        for (int k = 0; k < 4; ++k) *(uint32_t*)(treeNodes[k] + 8) = Rnd() % 5;
        *(uint32_t*)(win + 0x8c) = Rnd() % 5;
        for (int k = 1; k <= 3; ++k) {
            *(uint32_t*)((uint8_t*)Collection_Get(&types, 0, k) + 0x173) = 1 + Rnd() % 3;
            uint8_t* t = (uint8_t*)Collection_Get(&smallTypes, 0, k);
            *(uint32_t*)(t + 0x22) = Rnd() % 4 == 0 ? 0x80000000u + Rnd() : Rnd() % 900000;
            t[0x32] = (uint8_t)(Rnd() % 3);
            *(int32_t*)(t + 0x78c) = (int32_t)(Rnd() % 2);
        }
        uint8_t* co = (uint8_t*)Collection_Get(&companies, 0, 1);
        *(uint32_t*)(co + 0x3b) = Rnd() % 2;
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(co + 0x35b + k * 4) = (int32_t)(Rnd() % 200) - 100;
        uint8_t* pl = (uint8_t*)Collection_Get(&players, 0, 1);
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(pl + 0x2ab + k * 4) = (int32_t)(Rnd() % 200) - 100;
        hook::Global<float>(0x6d1738) = RndF(-2.0f, 10.0f);
        hook::Global<float>(0x6d173c) = RndF(-2.0f, 10.0f);
        // Picked object, stations, nodes.
        memset(picked, 0, sizeof picked);
        int pk = (int)(Rnd() % 3);
        if (pk == 0) *(uint32_t*)(picked + 0x10f) = 1 + Rnd() % 3;
        if (pk == 1) *(uint32_t*)(picked + 0x113) = 1 + Rnd() % 3;
        bool nullPick = Rnd() % 10 == 0;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, k);
            *(uint32_t*)(st + 4) = 1 + Rnd() % 3;
            sprintf((char*)st + 0x46b, "Station %u", Rnd() % 100);
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            *(void***)b = bvtbl;
            b[0x10] = (uint8_t)(Rnd() % 3);
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            for (int j = 0x14; j <= 0x34; j += 4) *(float*)(n + j) = RndF(0.0f, 9000.0f);
        }
        for (auto& st : g_stState) st[0x8c] = (uint8_t)(Rnd() % 4);
        // New-game rows.
        int32_t row = (int32_t)(Rnd() % 4);
        for (int k = 0; k < 4 * 9; ++k) world[0x69d8 + k] = (uint8_t)Rnd();
        for (int k = 0; k < 4; ++k) world[0x69d8 + k * 9] = (uint8_t)(Rnd() % 0x29);
        hook::Global<uint8_t*>(0x6d07b4) = Rnd() % 3 ? win : nullptr;
        // Cities (24) on a 2x2 block map.
        for (int k = 1; k <= 24; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
            *(uint16_t*)(c + 0x23a) = (uint16_t)k;
            *(int32_t*)(c + 0x23e) = Rnd() % 4 == 0;
            *(int32_t*)(c + 0x256) = (int32_t)(Rnd() % 6);
            *(float*)(c + 0x1e2) = RndF(0.0f, 700.0f);
            *(float*)(c + 0x1ea) = RndF(0.0f, 700.0f);
            *(float*)(c + 0x25a) = RndF(0.0f, 5000.0f);
            *(int32_t*)(c + 0x272) = (int32_t)(Rnd() % 3);
            sprintf((char*)c + 0x356, "City %d", k);
        }
        for (int k = 0; k < 4; ++k) blocks[k * 4] = (uint8_t)(1 + Rnd() % 24);
        // Building dialog selection.
        int32_t pickId = (int32_t)(Rnd() % 15) - 11;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* b = (uint8_t*)Collection_Get(&types, 0, k);
            sprintf((char*)b + 0x40, Rnd() % 2 ? "Commercial%u00" : "Thing%u", Rnd() % 3);
            sprintf((char*)Collection_Get(&smallTypes, 0, k) + 0x4f, Rnd() % 2 ? "Coal" : "Odd %u", Rnd() % 9);
        }
        // Hover widget.
        memset(widget, 0, sizeof widget);
        memset(parentW, 0, sizeof parentW);
        *(uint16_t*)(widget + 4) = (uint16_t)Rnd();
        widget[0x2f] = (uint8_t)(Rnd() % 3);
        widget[0x30] = (uint8_t)(Rnd() % 3);
        *(uint8_t**)(widget + 0x86) = parentW;
        *(float*)(parentW + 5) = RndF(0.0f, 600.0f);
        *(float*)(parentW + 9) = RndF(0.0f, 400.0f);
        *(float*)(widget + 0xb) = RndF(0.0f, 300.0f);
        *(float*)(widget + 0xf) = RndF(0.0f, 300.0f);
        *(float*)(widget + 0x13) = RndF(0.0f, 100.0f);
        *(void**)(widget + 0x9e) = Rnd() % 4 ? (void*)&FakeMoveCb : nullptr;
        *(void**)(widget + 0x96) = Rnd() % 4 ? (void*)&FakeDragCb : nullptr;
        *(const char**)(widget + 0x43) = tipTexts[Rnd() % 4];
        float hx = RndF(0.0f, 800.0f), hy = RndF(0.0f, 600.0f);
        int32_t moved = (int32_t)(Rnd() % 2), track = (int32_t)(Rnd() % 2);
        int hoverState = (int)(Rnd() % 3);
        // Money slider.
        hook::Global<uint8_t*>(0x6cec18) = win;
        hook::Global<int32_t>(0x6cec14) = Rnd() % 8 == 0 ? -(int32_t)(Rnd() % 300000) : (int32_t)(Rnd() % (Rnd() % 2 ? 900000 : 90000000));
        hook::Global<int32_t>(0x6cec10) = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % 6000000) - 100000;
        int32_t reset = Rnd() % 4 == 0;
        *(int32_t*)(pl + 0x1e1) = (int32_t)(Rnd() % 2);
        *(uint32_t*)(pl + 0x1dd) = Rnd() % 2;
        *(int32_t*)(pl + 0x28d) = (int32_t)(Rnd() % 300) - 50;
        *(int32_t*)(co + 0x43) = (int32_t)(1800 + Rnd() % 200);
        *(uint16_t*)(world + 0xd) = (uint16_t)(1800 + Rnd() % 200);
        world[0xf] = (uint8_t)(Rnd() % 13);
        if (op == 22) *(int32_t*)(world + 0x25) = 1;
        if (op == 23) *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 2);
        // Industry view.
        *(uint32_t*)(view + 0x90) = Rnd() % 5;
        *(int32_t*)(world + 0x4af7) = Rnd() % 3 == 0;
        *(int32_t*)(world + 0x4ac3) = Rnd() % 4 == 0;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            *(int32_t*)(b + 0x276) = Rnd() % 4 == 0;
            *(int32_t*)(b + 0x246) = (int32_t)(Rnd() % 2);
            *(int32_t*)(b + 0x3d5) = (int32_t)(1800 + Rnd() % 210);
            for (int j = 0; j < 30; ++j) *(float*)(b + 0x310 + j * 4) = RndF(0.0f, 900000.0f);
            *(float*)(b + 0x1e2) = RndF(0.0f, 600.0f);
            *(float*)(b + 0x1ea) = RndF(0.0f, 600.0f);
        }
        for (int k = 0; k < 3; ++k) {
            uint8_t* st = g_stState[k];
            *(int32_t*)(st + 0x3f) = Rnd() % 4 != 0;
            *(uint32_t*)(st + 0x22) = Rnd() % 2000000;
            *(int32_t*)(st + 0x43) = (int32_t)(Rnd() % 100000);
            st[0x32] = (uint8_t)(Rnd() % 6);
            strcpy((char*)st + 0x4f, Rnd() % 2 ? "Coal" : "Mill");
        }
        sprintf((char*)ngInit + 0x44, "Game %u", Rnd() % 100);
        // Editor pages.
        for (int k = 0; k < 49; ++k) *(int32_t*)(world + 0x4a7f + k * 4) = (int32_t)(Rnd() % 2);
        int32_t keep = (int32_t)(Rnd() % 2);
        static uint8_t evWidget[0x100];
        static char evText[0x40];
        const char* edits[] = { "1900", "1700", "2500", "abc", "1850 trains", "" };
        strcpy(evText, Rnd() % 2 ? edits[Rnd() % 6] : "Some scenario text");
        *(char**)(evWidget + 0xba) = evText;
        *(int32_t*)(evWidget + 0x82) = 1 + (int32_t)(Rnd() % 40);
        uint32_t ev[12] = {};
        ev[0] = Rnd() % 6 ? 0xca + Rnd() % 2 : Rnd() % 300;
        ev[1] = op == 27 ? 0x5b68 + Rnd() % 14 : Rnd() % 3 ? 0x5aa0 + Rnd() % 5 : 0x5aa8 + Rnd() % 0x60;
        ev[2] = Rnd() % 3;
        ev[0xb] = (uint32_t)(uintptr_t)evWidget;
        *(int32_t*)(world + 0x66ca) = (int32_t)(1800 + Rnd() % 300);
        *(int32_t*)(world + 0x66ce) = (int32_t)(1800 + Rnd() % 300);
        *(int32_t*)(world + 0x66d2) = (int32_t)(1800 + Rnd() % 300);
        // Sound page, resolutions, train list.
        g_nProviders = (int)(Rnd() % 4);
        for (int k = 0; k < 3; ++k) *(float*)(fe.e + 0x1146fa + k * 4) = RndF(0.0f, 1.0f);
        static const int32_t sizes[][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 }, { 512, 384 }, { 1920, 1200 } };
        int32_t nModes = (int32_t)(Rnd() % 16);
        for (int k = 0; k < nModes; ++k) {
            const int32_t* sz = sizes[Rnd() % 7];
            *(int32_t*)(modes + k * 0x15) = sz[0];
            *(int32_t*)(modes + k * 0x15 + 4) = sz[1];
            *(int32_t*)(modes + k * 0x15 + 8) = Rnd() % 2 ? 32 : 16;
        }
        *(int32_t*)(fe.e + 0x30) = nModes;
        *(uint8_t**)(fe.e + 0x34) = modes;
        *(int32_t*)(fe.e + 0x11421e) = nModes ? (int32_t)(Rnd() % nModes) : 0;
        fe.e[0x114226] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(fe.e + 0x38) = Rnd() % 2 ? 1280 : 1920;
        *(int32_t*)(fe.e + 0x3c) = Rnd() % 2 ? 1024 : 1200;
        *(int32_t*)(kid2aff + 0x1d6) = (int32_t)(Rnd() % 4) - 1;
        for (int k = 0; k < 4; ++k) *(uint32_t*)(trainNodes[k] + 8) = Rnd() % 3;
        for (int k = 0; k < 3; ++k) {
            trains[k][0x28] = Rnd() % 4 ? 0 : 0xff;
            *(uint16_t*)(trains[k] + 0x29) = (uint16_t)(1 + Rnd() % 3);
        }
        hook::Global<int32_t>(0x6d3aec) = (int32_t)(Rnd() % 3);
        hook::Global<uint8_t>(0x6d3ae1) = Rnd() % 6 == 0;
        if (op == 29) {
            *(int32_t*)(win + 0x8c) = (int32_t)(Rnd() % 25);
            *(int32_t*)(win + 0x90) = (int32_t)(Rnd() % 3);
        }
        // Territory transfer, row counts, condition types.
        bvtbl[0x70 / 4] = (void*)&FakeIsPassenger;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* b = (uint8_t*)Collection_Get(&buildings, 0, k);
            b[0x3d4] = (uint8_t)(Rnd() % 3);
            b[0x11] = (uint8_t)Rnd();
            uint8_t* tr = (uint8_t*)Collection_Get(&trainsTab, 0, k);
            *(int32_t*)(tr + 0x41) = Rnd() % 4 ? 1 + (int32_t)(Rnd() % 24) : -1;
            *(int32_t*)(tr + 0x45) = Rnd() % 4 ? 1 + (int32_t)(Rnd() % 24) : -1;
            *(int32_t*)(tr + 0x51) = (int32_t)(Rnd() % 4);
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            n[0x216] = (uint8_t)(Rnd() % 3);
            *(uint32_t*)(n + 0x212) = Rnd() % 5;
            *(uint16_t*)(n + 0x22e) = Rnd() % 2 ? 0 : 7; // not a company: no track counting
            *(int32_t*)((uint8_t*)Collection_Get(&trackStations, 0, k) + 0x1e) = (int32_t)(Rnd() % 6);
        }
        int32_t newOwner = Rnd() % 2 ? 0 : 5, terr = (int32_t)(Rnd() % 32);
        bool noOwner = Rnd() % 10 == 0, noFilter = Rnd() % 3 == 0;
        const int32_t kinds[] = { 9, 10, 11, 12, 23, 4 };
        int32_t gkind = kinds[Rnd() % 6];
        static uint8_t ownSnap[3][4][0x10];
        for (int k = 0; k < 3; ++k) {
            memcpy(ownSnap[k][0], (uint8_t*)Collection_Get(&buildings, 0, k + 1) + 0x276, 4);
            memcpy(ownSnap[k][1], (uint8_t*)Collection_Get(&trainsTab, 0, k + 1) + 0x51, 4);
            memcpy(ownSnap[k][2], (uint8_t*)Collection_Get(&nodes, 0, k + 1) + 0x22e, 2);
            memcpy(ownSnap[k][3], (uint8_t*)Collection_Get(&trackStations, 0, k + 1) + 0x1e, 4);
        }
        if (op == 34) *(int32_t*)(world + 0x6a6c) = (int32_t)(Rnd() % 5);
        *(int32_t*)(kid5033 + 0xcc) = (int32_t)(Rnd() % 2);
        int32_t condSel = (int32_t)(Rnd() % 0x266);
        // Effect panel, station picker.
        static uint8_t effect[0x20];
        for (uint8_t& b : effect) b = (uint8_t)(Rnd() % 3 == 0 ? Rnd() : Rnd() % 3);
        effect[4] = (uint8_t)(Rnd() % 10);
        if (op == 36) {
            *(uint8_t**)(win + 0x84) = effect;
            *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 20);
            *(int32_t*)(win + 0x7c) = (int32_t)(Rnd() % 9);
            *(int32_t*)(win + 0x80) = (int32_t)(Rnd() % 2);
        }
        for (int k = 1; k <= 3; ++k) *(int32_t*)Collection_Get(&stations, 0, k) = k;
        *(uint32_t*)(world + 0x4cba) = Rnd() % 5;
        hook::Global<int32_t>(0x622ae4) = Rnd() % 3 ? (int32_t)(Rnd() % 5) : -1;
        // MP setup page, station cargo tooltip.
        if (op == 38) *(int32_t*)(win + 0x7b) = (int32_t)(Rnd() % 4 != 0);
        *(int32_t*)(kidce8 + 0x1d6) = (int32_t)(Rnd() % 8) - 1;
        hook::Global<uint8_t*>(0x6d14bc) = setup;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* st = (uint8_t*)Collection_Get(&stations, 0, k);
            for (int j = 0x4b0; j <= 0x53c; j += 4) *(float*)(st + j) = Rnd() % 3 ? (float)(Rnd() % 3) : RndF(0.0f, 5000.0f);
        }
        int32_t tipId = Rnd() % 2 ? 0xb3f6 : 0xb3f7;
        // Company list, train-order cargo tooltip.
        for (int k = 1; k <= 3; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&companies, 0, k);
            *(int32_t*)c = k;
            *(int32_t*)(c + 0x3f) = Rnd() % 4 != 0;
            sprintf((char*)c + 4, "Railroad %d", k);
            *(uint32_t*)(c + 0x3b) = Rnd() % 2;
            *(uint16_t*)(c + 0xd57) = (uint16_t)(Rnd() % 5);
            *(uint32_t*)(c + 0x3b) = Rnd() % 2;
        }
        sprintf((char*)pl + 8, "Chairman %u", Rnd() % 9);
        *(int32_t*)(pl + 0x2a7) = (int32_t)(Rnd() % 3);
        for (int k = 0x4acb; k <= 0x4ad7; k += 4) *(int32_t*)(world + k) = Rnd() % 3 == 0;
        if (op == 40) {
            // DesyncWatch's always_new_company jump at 0x4c6e4a, on or off.
            static const uint8_t dswOff[6] = { 0x8b, 0x0d, 0x78, 0xec, 0x6c, 0x00 };
            uint8_t dswOn[6] = { 0xe9, 0, 0, 0, 0, 0x90 };
            *(int32_t*)(dswOn + 1) = 0x4c6e8b - (0x4c6e4a + 5);
            DWORD dp;
            VirtualProtect((void*)0x4c6e4a, 6, PAGE_EXECUTE_READWRITE, &dp);
            memcpy((void*)0x4c6e4a, Rnd() % 2 ? dswOn : dswOff, 6);
            VirtualProtect((void*)0x4c6e4a, 6, dp, &dp);
            FlushInstructionCache(GetCurrentProcess(), (void*)0x4c6e4a, 6);
            *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 4);
            *(int32_t*)(world + 0x25) = 1;
            *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 6) - 1;
        }
        for (int k = 1; k <= 4; ++k) {
            uint8_t* ct = (uint8_t*)Collection_Get(&cargoTypes, 0, k);
            *(int32_t*)(ct + 0x56) = Rnd() % 4 != 0;
            *(int32_t*)(ct + 0x21) = (int32_t)(1800 + Rnd() % 200);
            ct[0x46] = (uint8_t)(Rnd() % 3 == 0);
            strcpy((char*)ct + 4, Rnd() % 2 ? "Coal" : "Grain");
        }
        for (int k = 0; k < 0x40; ++k) ((uint8_t*)0x6cfd38)[k] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(world + 0x46c38) = 0;
        for (int k = 0; k < 64; ++k)
            for (int j = 0; j < 8; ++j) *(uint16_t*)(regionGrid[k] + j * 2) = (uint16_t)(Rnd() % 500);
        for (int k = 1; k <= 3; ++k) *(int32_t*)((uint8_t*)Collection_Get(&stations, 0, k) + 0x38) = 0;
        hook::Global<int32_t>(0x621df4) = (int32_t)(Rnd() % 3);
        int32_t orderTip = 0x7f55 + (int32_t)(Rnd() % 10);
        if (op == 42) {
            trains[1][0x28] = (uint8_t)Rnd();
            *(uint32_t*)(trains[1] + 0x24) = Rnd() % 3 ? Rnd() % 6 : 900 + Rnd() % 220;
        }
        for (int k = 0; k < 6; ++k) *(float*)(world + 0xbe2 + k * 4) = RndF(-1.0f, 3.0f);
        // Loan sliders, achievements events, human rows, train list refill.
        *(int32_t*)(kid714f + 0x185) = (int32_t)(Rnd() % 12);
        *(int32_t*)(kid7150 + 0x185) = (int32_t)(Rnd() % 12);
        hook::Global<int32_t>(0x6cebd4) = (int32_t)(Rnd() % 200000);
        hook::Global<int32_t>(0x6cec0c) = hook::Global<int32_t>(0x6cebd4) + (int32_t)(Rnd() % 3000000);
        hook::Global<int32_t>(0x5f2c4c) = (int32_t)(Rnd() % 12);
        hook::Global<int32_t>(0x5f2c50) = (int32_t)(Rnd() % 12);
        hook::Global<int32_t>(0x6cebcc) = (int32_t)(Rnd() % 200000);
        hook::Global<int32_t>(0x6cebd0) = hook::Global<int32_t>(0x6cebcc) + (int32_t)(Rnd() % 3000000);
        int32_t rerange = (int32_t)(Rnd() % 2);
        if (op == 44) *(int32_t*)(world + 0x25) = 1;
        if (op == 45) {
            const uint32_t achIds[] = { 0xa7fb, 0xa7fc, 0xa7ff, 0xa7fa, 0xabe0, 0xac10, 0xafc7, 0xafc8 };
            ev[0] = Rnd() % 6 ? 0xca + Rnd() % 2 : 0x10;
            ev[1] = achIds[Rnd() % 8];
            int32_t g = (int32_t)(Rnd() % 47);
            *(int32_t*)(evWidget + 0x82) = g >= 25 ? g + 2 : g; // not 25 / 26 (track / building refresh)
        }
        if (op == 46)
            for (int k = 0; k < 0x90; ++k) world[0x69d8 + k] = (uint8_t)(Rnd() % 3);
        *(uint32_t*)(kid2aff + 0x1e2) = Rnd() % 9;
        hook::Global<int32_t>(0x622b28) = (int32_t)(Rnd() % 4);
        // Video page, train picker.
        *(int32_t*)(modeObj + 0x14) = Rnd() % 2 ? 0 : (int32_t)Rnd();
        *(int32_t*)(modeObj + 0x18) = Rnd() % 2 ? 0 : (int32_t)Rnd();
        fe.e[0x1146ae] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(fe.e + 0x11427a) = (int32_t)(Rnd() % 2);
        *(int32_t*)(fe.e + 0x11470e) = (int32_t)(Rnd() % 3);
        *(int32_t*)(fe.e + 0x114206) = (int32_t)(Rnd() % 2);
        for (int k = 1; k <= 3; ++k) *(uint32_t*)((uint8_t*)Collection_Get(&trainsTab, 0, k) + 0x25) = 100 + Rnd() % 50;
        uint32_t excludeTrain = Rnd() % 5;
        *(uint32_t*)(world + 0x4cb6) = Rnd() % 5;
        hook::Global<int32_t>(0x622b2c) = Rnd() % 3 ? (int32_t)(Rnd() % 5) : -1;
        if (op == 49) *(int32_t*)(world + 0x21) = (int32_t)(Rnd() % 4);
        // Rule scope controls.
        for (int k = 0; k < 3; ++k) *(int32_t*)(scopeKids[k] + 0xcc) = Rnd() % 4 != 0;
        for (int k = 0; k < 3; ++k) {
            uint8_t* r = (uint8_t*)Collection_Get(&rules3, 0, k + 1);
            for (int n = 0; n < 3; ++n) {
                *(int32_t*)ruleNodes[k][n] = Rnd() % 10 ? (int32_t)(Rnd() % 40) : -1 - (int32_t)(Rnd() % 2);
                *(uint8_t**)(ruleNodes[k][n] + 0x1a) = n < 2 && Rnd() % 2 ? ruleNodes[k][n + 1] : nullptr;
            }
            *(uint8_t**)r = Rnd() % 6 ? ruleNodes[k][0] : nullptr;
            r[0x82b] = Rnd() % 8 == 0;
            r[0x7f4] = (uint8_t)(Rnd() % 2);
        }
        if (op == 50) *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
        // Condition rows (slot chains at rule +4..).
        static uint8_t condRows[3][3][4][0x28];
        const uint8_t firstKinds[4] = { 1, 2, 4, 8 };
        for (int k = 0; k < 3; ++k) {
            uint8_t* r = (uint8_t*)Collection_Get(&rules3, 0, k + 1);
            for (int sl = 0; sl < 3; ++sl) {
                for (int n = 0; n < 4; ++n) {
                    uint8_t* c = condRows[k][sl][n];
                    for (int b = 0; b < 0x24; ++b) c[b] = (uint8_t)Rnd();
                    *(int32_t*)c = (int32_t)(Rnd() % 40);
                    *(int32_t*)(c + 4) = (int32_t)(Rnd() % 4000) - 2000;
                    c[8] = n == 0 ? firstKinds[Rnd() % 4] : (uint8_t)(1 + Rnd() % 9);
                    *(int32_t*)(c + 9) = Rnd() % 3 == 0 ? -1 : Rnd() % 2 ? 0 : (int32_t)(Rnd() % 500);
                    *(int32_t*)(c + 0xd) = Rnd() % 2 ? 0 : (int32_t)(Rnd() % 500);
                    *(uint8_t**)(c + 0x24) = n < 3 && Rnd() % 2 ? condRows[k][sl][n + 1] : nullptr;
                }
                *(uint8_t**)(r + 4 + sl * 4) = Rnd() % 5 ? condRows[k][sl][0] : nullptr;
            }
        }
        for (int k = 0; k < nRuleKids; ++k) {
            *(int32_t*)(ruleKids[k] + 0xcc) = Rnd() % 3 == 0;
            sprintf(ruleKidText[k], "Text %d-%u", k, Rnd() % 1000);
        }
        const char* numTexts[] = { "0", "5", "150", "-3", "7", "70000", "abc", "1000", "1001" };
        for (int k = 29; k < nRuleKids; ++k) strcpy(ruleKidText[k], numTexts[Rnd() % 9]);
        // Visibility for Widget_IsVisible (flags +0x6a / +0x68, parent +0x86 with +0x5c).
        *(void**)(kidParent + 0x5c) = kidParent;
        for (int k = 0; k < nRuleKids; ++k) {
            ruleKids[k][0x6a] = (uint8_t)(Rnd() % 4 != 0);
            ruleKids[k][0x68] = (uint8_t)(Rnd() % 4 != 0);
            *(uint8_t**)(ruleKids[k] + 0x86) = kidParent;
            *(int32_t*)(ruleKids[k] + 0x1d2) = (int32_t)(Rnd() % 3);
        }
        int32_t slotGuard = Rnd() % 8 == 0;
        int32_t terrId = (int32_t)(Rnd() % 26) - 1, terrLast = (int32_t)(Rnd() % 25);
        // Editor industry list.
        static const char* indNames[] = { "Coal Mine", "Farm", "Lumber Mill", "Steel Mill" };
        for (int k = 1; k <= 4; ++k) {
            uint8_t* ty = (uint8_t*)Collection_Get(&indTypes, 0, k);
            *(int32_t*)(ty + 0x105) = (int32_t)(Rnd() % 5) * 1000;
            *(int32_t*)(ty + 0x109) = (int32_t)(Rnd() % 9000);
            strcpy((char*)ty + 0x45, indNames[k - 1]);
        }
        for (int k = 1; k <= 5; ++k) {
            uint8_t* e = (uint8_t*)Collection_Get(&indList, 0, k);
            *(uint32_t*)(e + 0x72) = 1 + Rnd() % 4;
            for (int j = 0x30; j <= 0x32; ++j) e[j] = (uint8_t)(Rnd() % 2);
        }
        for (int k = 1; k <= 3; ++k) {
            uint8_t* nr = (uint8_t*)Collection_Get(&namedTab, 0, k);
            strcpy((char*)nr, indNames[Rnd() % 4]);
            *(int32_t*)(nr + 0x3d) = (int32_t)(Rnd() % 3);
        }
        *(Collection**)(world + 0x66b6) = &namedTab;
        // Editor house list: type names (+4) listed by cities (+0x37f name tables).
        static const char* houseNames[] = { "House A", "House B", "Shop", "Lodge" };
        for (int k = 1; k <= 3; ++k) {
            uint8_t* ty = (uint8_t*)Collection_Get(&smallTypes, 0, k);
            strcpy((char*)ty + 4, houseNames[Rnd() % 4]);
            ty[0xba] = Rnd() % 5 == 0;
            ty[0xbb] = Rnd() % 5 == 0;
        }
        for (int k = 1; k <= 2; ++k) {
            uint8_t* cn = (uint8_t*)Collection_Get(&cityNames, 0, k);
            strcpy((char*)cn, houseNames[Rnd() % 4]);
            *(int32_t*)(cn + 0x1e) = (int32_t)(Rnd() % 9);
        }
        for (int k = 1; k <= 24; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
            *(Collection**)(c + 0x37f) = Rnd() % 3 ? &cityNames : &cityNamesEmpty;
        }
        *(Collection**)(world + 0x66b2) = &namedTab;
        int32_t houseKeep = Rnd() % 3 == 0;
        int32_t cityPick = (int32_t)(Rnd() % 28) - 2, cityKeep = Rnd() % 4 == 0;
        uint32_t cityCur = Rnd() % 3 ? 1 + Rnd() % 24 : Rnd() % 30;
        if (op == 63)
            for (int k = 1; k <= 24; ++k) {
                uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
                const char* words[] = { "Bath", "york", "Dover", "ely", "Leeds" };
                sprintf((char*)c + 0x356, "%s %u", words[Rnd() % 5], Rnd() % 3);
                *(float*)(c + 0x25a) = RndF(0.0f, 600.0f);
                *(uint32_t*)(c + 0x2f2) = Rnd() % 12;
                for (int j = 1; j <= 3; ++j) {
                    uint8_t* ty = (uint8_t*)Collection_Get(&smallTypes, 0, j);
                    *(uint32_t*)(ty + 0x794) = Rnd() % 12;
                }
            }
        if (op == 64) {
            // Region-type templates (world +0xfe7, 0x4e1 bytes; trade slots at +0x3f1).
            const char* cargoNames[] = { "Coal", "Grain", "", "Wool" };
            const char* distinct[4] = { "Coal", "Grain", "Wool", "Iron" }; // the conversion fix-up walks distinct names
            for (int k = 1; k <= 4; ++k) strcpy((char*)Collection_Get(&cargoTypes, 0, k) + 4, distinct[k - 1]);
            for (int i = 0; i < 12; ++i) {
                uint8_t* tp = world + 0xfe7 + i * 0x4e1;
                sprintf((char*)tp, "Region %d", i);
                *(float*)(tp + 0x3ed) = RndF(0.0f, 9.0f);
                for (int j = 0; j < 5; ++j) {
                    uint8_t* e = tp + 0x3f1 + j * 0x30;
                    *(int32_t*)e = (int32_t)(Rnd() % 4);
                    *(float*)(e + 4) = RndF(0.0f, 5.0f);
                    strcpy((char*)e + 8, cargoNames[Rnd() % 4]);
                    strcpy((char*)e + 0x1c, cargoNames[Rnd() % 4]);
                }
            }
            for (int k = 1; k <= 24; ++k) {
                uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
                *(int32_t*)(c + 0x2f6) = Rnd() % 3 != 0;
                *(int32_t*)(c + 0x242) = (int32_t)(Rnd() % 3);
                *(uint32_t*)(c + 0x2f2) = Rnd() % 12;
            }
            hook::Global<int32_t>(0x6cffa4) = (int32_t)(Rnd() % 12);
        }
        if (op == 62) {
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
        }
        if (op == 59) {
            // Deleting row 0 makes row 1 the first row, whose kind must be listable
            // (kinds 3 / 5..7 resend the previous text: stale stack in the original).
            for (int k = 0; k < 3; ++k)
                for (int sl = 0; sl < 3; ++sl) condRows[k][sl][1][8] = firstKinds[Rnd() % 4];
        }
        if (op >= 59 && op <= 60) {
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
            *(int32_t*)(win + 0x98) = (int32_t)(Rnd() % 6) - 1;
            for (int k = 1; k <= 3; ++k) {
                uint8_t* r = (uint8_t*)Collection_Get(&rules3, 0, k);
                for (int sl = 0; sl < 3; ++sl) {
                    *(int32_t*)(r + 0x7ff + sl * 4) = (int32_t)(Rnd() % 3);
                    *(int32_t*)(r + 0x80f + sl * 4) = (int32_t)(Rnd() % 5) - 1;
                    r[0x7fb + sl] = (uint8_t)(Rnd() % 11);
                }
            }
        }
        int32_t indKeep = Rnd() % 3 == 0;
        if (op == 55)
            for (int k = 1; k <= 24; ++k) {
                uint8_t* r = (uint8_t*)Collection_Get(&regions, 0, k);
                const char* words[] = { "north", "South", "east", "West", "Lake", "lake", "Bay" };
                sprintf((char*)r + 4, "%s %u", words[Rnd() % 7], Rnd() % 4);
                *(int32_t*)(r + 0x31) = Rnd() % 3 == 0;
                *(int32_t*)(r + 0x2d) = (int32_t)(Rnd() % 2);
                *(int32_t*)(r + 0x39) = (int32_t)(Rnd() % 9000);
            }
        if (op == 54) {
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
            *(int32_t*)(win + 0x8c) = (int32_t)(Rnd() % 5) - 1;
        }
        static uint8_t condNode[0x30], condSnap[0x30];
        for (uint8_t& b : condNode) b = (uint8_t)Rnd();
        memcpy(condSnap, condNode, sizeof condNode);
        int32_t condGuard = Rnd() % 8 == 0;
        {
            const uint32_t qa1[] = { 0, 0x4fc0, 0x4fc1, 0x4fbf, 0x4fba }, qa2[] = { 0, 0x4fba, 0x4fbb };
            g_ansId[0] = 0x4fc2; g_ansVal[0] = qa1[Rnd() % 5];
            g_ansId[1] = 0x4fbc; g_ansVal[1] = qa2[Rnd() % 3];
            g_ansId[2] = 0x4fd7; g_ansVal[2] = Rnd() % 4 ? 0x4ec0 + Rnd() % 6 : 0;
            g_ansId[3] = 0x4fd8; g_ansVal[3] = Rnd() % 4 ? 0x4eb6 + Rnd() % 6 : 0;
            if (op == 66) {
                const uint32_t a1[] = { 0, 0x4f4c, 0x4f4d, 0x4f4e, 0x4f4e, 0x4f4e }, a2[] = { 0, 0x4f5d, 0x4f5e, 0x4f57 };
                g_ansId[1] = 0x4f4f;
                g_ansVal[1] = a1[Rnd() % 6];
                g_ansId[2] = 0x4fd6;
                g_ansVal[2] = a2[Rnd() % 4];
                *(int32_t*)(win + 0x78) = (int32_t)(Rnd() % 901);
                *(int32_t*)(win + 0x80) = (int32_t)(Rnd() % 2);
            }
            if (op == 61) {
                g_ansId[1] = 0x5000;
                g_ansVal[1] = Rnd() % 4 ? 0x5001 + Rnd() % 3 : 0;
                g_ansId[2] = 0x5006;
                g_ansVal[2] = Rnd() % 4 ? 0x5014 + Rnd() % 9 : 0;
                *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
                *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
            }
            if (op == 54) {
                g_ansId[2] = 0x5006;
                g_ansVal[2] = Rnd() % 4 ? 0x5014 + Rnd() % 9 : 0;
            }
        }
        int32_t valueGuard = Rnd() % 8 == 0;
        if (op == 65) {
            *(uint8_t**)(win + 0x78) = condNode;
            *(int32_t*)(win + 0x7c) = (int32_t)(Rnd() % 60);
            condNode[8] = (uint8_t)(Rnd() % 6);
            *(int32_t*)(condNode + 4) = Rnd() % 4 ? (int32_t)(Rnd() % 2000) - 1000 : 0;
            memcpy(condSnap, condNode, sizeof condNode);
        }
        if (op == 53) {
            *(uint8_t**)(win + 0x78) = condNode;
            *(int32_t*)(win + 0x7c) = (int32_t)(Rnd() % 60);
        }
        if (op >= 69 && op <= 71) {
            *(int32_t*)(win + 0x84) = op == 70 || Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x94) = (int32_t)(Rnd() % 6) - 1;
            hook::Global<uint32_t>(0x622074) = 0x5001 + Rnd() % 2;
            for (int k = 1; k <= 3; ++k) ((uint8_t*)Collection_Get(&rules3, 0, k))[0x7ee] = (uint8_t)(Rnd() % 6);
        }
        if (op == 67 || op == 69) {
            // Trigger chains (rule +0, next +0x1a): type -2..900, comparison +4, value +9.
            static uint8_t trig[3][5][0x20];
            const int32_t specialTypes[] = { 0x4b, 0x4c, 0x4d, 0x4e, 0x35f, 0x360, 6, 7, 0x3c };
            for (int k = 0; k < 3; ++k) {
                for (int n = 0; n < 5; ++n) {
                    uint8_t* e = trig[k][n];
                    memset(e, 0, 0x20);
                    int32_t ty = Rnd() % 8 == 0 ? -1 - (int32_t)(Rnd() % 2) : Rnd() % 3 == 0 ? specialTypes[Rnd() % 9] : (int32_t)(Rnd() % 901);
                    *(int32_t*)e = ty;
                    e[4] = n == 0 ? (uint8_t)(Rnd() % 8) : (uint8_t)(Rnd() % 9);
                    *(int32_t*)(e + 5) = Rnd() % 4 == 0;
                    *(int32_t*)(e + 9) = Rnd() % 3 ? (int32_t)(Rnd() % 40) : 0;
                    e[0xd] = (uint8_t)(Rnd() % 4);
                    *(uint32_t*)(e + 0xe) = Rnd() % 30;
                    *(uint32_t*)(e + 0x12) = Rnd() % 30;
                    *(uint8_t**)(e + 0x1a) = n < 4 && Rnd() % 4 ? trig[k][n + 1] : nullptr;
                }
                *(uint8_t**)Collection_Get(&rules3, 0, k + 1) = Rnd() % 6 ? trig[k][0] : nullptr;
            }
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
        }
        // Face / logo picker.
        for (int k = 1; k <= 3; ++k) {
            uint8_t* f = (uint8_t*)Collection_Get(&packFaces, 0, k);
            sprintf((char*)f, "Face%d", k);
            sprintf((char*)f + 0x40, "face_pic%d", k);
        }
        for (int k = 1; k <= 2; ++k) sprintf((char*)Collection_Get(&packLogos, 0, k), "Logo%d", k);
        for (int k = 1; k <= 2; ++k) {
            uint8_t* p = (uint8_t*)Collection_Get(&players, 0, k);
            p[0x15c] = (uint8_t)(Rnd() % 42);
            sprintf((char*)p + 0xc47, "Face%u", Rnd() % 5);
        }
        for (int k = 1; k <= 3; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&companies, 0, k);
            *(int32_t*)(c + 0x37) = Rnd() % 4 == 0 ? -1 : (int32_t)(Rnd() % 36);
            sprintf((char*)c + 0xd65, "Logo%u", 1 + Rnd() % 2); // a missing pack logo runs the picker off its array (game bug)
        }
        *(int32_t*)(faceDlg + 0x78) = Rnd() % 2 ? 8 : (int32_t)(Rnd() % 8);
        *(uint32_t*)(faceDlg + 0x8c) = 1 + Rnd() % 2;
        *(uint32_t*)(faceDlg + 0x88) = 1 + Rnd() % 3;
        static uint8_t faceSnap[2][0xd00], logoSnap[3][0xe00];
        for (int k = 0; k < 2; ++k) memcpy(faceSnap[k], Collection_Get(&players, 0, k + 1), 0xd00);
        for (int k = 0; k < 3; ++k) memcpy(logoSnap[k], Collection_Get(&companies, 0, k + 1), 0xe00);
        uint32_t faceEv[12] = {};
        faceEv[0] = Rnd() % 8 ? 0xcb : 0xca;
        faceEv[1] = Rnd() % 8 ? 0x3f20 + Rnd() % 2 : 0x3f22;
        static uint8_t ruleSnap3[3][0x900];
        for (int k = 0; k < 3; ++k) memcpy(ruleSnap3[k], Collection_Get(&rules3, 0, k + 1), 0x900);
        if (op == 52) {
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
        }
        if (op == 51) {
            *(int32_t*)(win + 0x84) = Rnd() % 6 ? 1 + (int32_t)(Rnd() % 3) : -1;
            *(int32_t*)(win + 0x9c) = (int32_t)(Rnd() % 3);
        }
        const uint32_t moneyAddrs[7] = { 0x6cec14, 0x6cec10, 0x6cebcc, 0x6cebd0, 0x5f2c50, 0x5f2c4c, 0x6cebd4 };
        uint32_t moneySnap[7];
        for (int k = 0; k < 7; ++k) moneySnap[k] = hook::Global<uint32_t>(moneyAddrs[k]);
        static uint8_t worldSnap[0x7000];
        memcpy(worldSnap, world, sizeof worldSnap);
        int32_t briefTab = hook::Global<int32_t>(0x621f50);
        // Tooltip.
        uint32_t now = 100000 + Rnd() % 100000;
        uint8_t uiInit[0xd00];
        memset(uiInit, 0, sizeof uiInit);
        *(uint8_t**)uiInit = win;
        *(int32_t*)(uiInit + 0xc58) = Rnd() % 4 != 0;
        *(int32_t*)(uiInit + 0xbdc) = (int32_t)(Rnd() % 4);
        *(int32_t*)(uiInit + 0xbe0) = (int32_t)Rnd();
        *(int32_t*)(uiInit + 0xbe4) = (int32_t)(now + Rnd() % 2000) - 1000;
        const char* oldTip = tipTexts[Rnd() % 4];
        const char* newTip = Rnd() % 5 ? tipTexts[Rnd() % 4] : nullptr;
        int32_t prio = (int32_t)(Rnd() % 4), sticky = (int32_t)(Rnd() % 2), data = (int32_t)Rnd();
        fe.e[0x57] = (uint8_t)(Rnd() % 8 == 0);

        std::vector<uint32_t> sent[2];
        uint32_t state[2][16];
        static uint8_t ngOut[2][0x100], setupOut[2][0x400], regOut[2][24][0x48];
        std::string tip[2];
        static uint8_t winSnap[0x200];
        memcpy(winSnap, win, sizeof winSnap);
        for (int side = 0; side < 2; ++side) {
            g_sent.clear();
            g_textSide = side;
            g_texts[side].clear();
            memcpy(win, winSnap, sizeof winSnap);
            memcpy(newGame, ngInit, sizeof ngInit);
            memcpy(setup, setupInit, sizeof setupInit);
            hook::Global<uint8_t*>(0x6d1270) = noSetup && mode == 1 ? nullptr : setup;
            hook::Global<int32_t>(0x6cff94) = guard;
            g_fakeTicks = now;
            memcpy(ui, uiInit, sizeof ui);
            *(char**)(ui + 0xbd8) = (char*)((void*(__cdecl*)(size_t))0x5a125d)(strlen(oldTip) + 1);
            strcpy(*(char**)(ui + 0xbd8), oldTip);
            int32_t r = 0;
            memset(capHolder, 0, sizeof capHolder);
            *(uint8_t**)(capHolder + 0xc6d) = hoverState == 0 ? widget : hoverState == 1 ? win : nullptr;
            memset(g_cbLog, 0, sizeof g_cbLog);
            memcpy(world, worldSnap, sizeof worldSnap);
            hook::Global<int32_t>(0x621f50) = briefTab;
            uint32_t evCopy[12];
            memcpy(evCopy, ev, sizeof ev);
            memcpy(condNode, condSnap, sizeof condNode);
            hook::Global<int32_t>(0x6d07d0) = condGuard;
            hook::Global<int32_t>(0x6d07d4) = slotGuard;
            hook::Global<int32_t>(0x6d07cc) = valueGuard;
            hook::Global<int32_t>(0x6d07c0) = valueGuard;
            hook::Global<int32_t>(0x621f4c) = terrLast;
            if (op == 63) hook::Global<uint32_t>(0x6cff74) = cityCur;
            for (int k = 0; k < 3; ++k) memcpy(Collection_Get(&rules3, 0, k + 1), ruleSnap3[k], 0x900);
            for (int k = 0; k < 7; ++k) hook::Global<uint32_t>(moneyAddrs[k]) = moneySnap[k];
            // IdAt's walk cache (which has a mirrored-position quirk) starts the same on both sides.
            for (int k = 0; k < 4; ++k) trainsTab.cachePos[k] = kCacheInvalid;
            for (int k = 0; k < 3; ++k) {
                memcpy((uint8_t*)Collection_Get(&buildings, 0, k + 1) + 0x276, ownSnap[k][0], 4);
                memcpy((uint8_t*)Collection_Get(&trainsTab, 0, k + 1) + 0x51, ownSnap[k][1], 4);
                memcpy((uint8_t*)Collection_Get(&nodes, 0, k + 1) + 0x22e, ownSnap[k][2], 2);
                memcpy((uint8_t*)Collection_Get(&trackStations, 0, k + 1) + 0x1e, ownSnap[k][3], 4);
            }
            switch (op) {
            case 0: side ? EditorDialog_ShowSettings(win) : ((void(__fastcall*)(uint8_t*))0x4ca790)(win); break;
            case 1: side ? CityDialog_ShowSummary() : ((void(*)())0x4cbc60)(); break;
            case 2: side ? Window_HighlightViewModes(win) : ((void(__fastcall*)(uint8_t*))0x4d4160)(win); break;
            case 3: side ? RulesDialog_ShowLimits(win) : ((void(__fastcall*)(uint8_t*))0x4d5ba0)(win); break;
            case 4: side ? MapSizeDialog_Show() : ((void(*)())0x4ea780)(); break;
            case 5: r = side ? NewGame_ClaimSlot(0, mode) : ((int32_t(__fastcall*)(int, int32_t))0x4eda30)(0, mode); break;
            case 6: side ? TerritoryDialog_Show(win) : ((void(__fastcall*)(uint8_t*))0x4fcb80)(win); break;
            case 7: side ? OptionsDialog_ShowGeneral(win) : ((void(__fastcall*)(uint8_t*))0x4fda20)(win); break;
            case 8: side ? OptionsDialog_ShowLists7587(win) : ((void(__fastcall*)(uint8_t*))0x4fdfd0)(win); break;
            case 9: side ? BuildingDialog_ShowTabs(win) : ((void(__fastcall*)(uint8_t*))0x507b90)(win); break;
            case 10: side ? BuildingDialog_ShowPrice(win) : ((void(__fastcall*)(uint8_t*))0x508550)(win); break;
            case 12: {
                uint8_t* o = nullPick ? nullptr : picked;
                side ? Map_ShowPickTooltip(o) : ((void(__fastcall*)(uint8_t*))0x4bffc0)(o);
                break;
            }
            case 13: side ? NewGameDialog_ShowRow(win, 0, row) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4ca010)(win, 0, row); break;
            case 14: side ? Window_HighlightViewModesIfOpen() : ((void(*)())0x4d44f0)(); break;
            case 15: side ? CityEditor_Show(win) : ((void(__fastcall*)(uint8_t*))0x4f5110)(win); break;
            case 16: side ? OptionsDialog_ShowGraphics(win) : ((void(__fastcall*)(uint8_t*))0x4fd730)(win); break;
            case 17: side ? OptionsDialog_ShowEngine(win) : ((void(__fastcall*)(uint8_t*))0x4fdd70)(win); break;
            case 18: side ? OptionsDialog_ShowDisplay(win) : ((void(__fastcall*)(uint8_t*))0x4fe960)(win); break;
            case 19: side ? BuildingDialog_Select(win, 0, pickId) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x508730)(win, 0, pickId); break;
            case 20:
                side ? Widget_OnHover(widget, 0, hx, hy, moved, track)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x558760)(widget, 0, hx, hy, moved, track);
                break;
            case 21: side ? Widget_OnLeave(widget) : ((void(__fastcall*)(uint8_t*))0x558840)(widget); break;
            case 22: side ? MoneySlider_Setup(reset) : ((void(__fastcall*)(int32_t))0x47bea0)(reset); break;
            case 23: side ? IndustryView_UpdateButtons(win) : ((void(__fastcall*)(uint8_t*))0x4b6fe0)(win); break;
            case 24: side ? MpNewGameDialog_Show(win) : ((void(__fastcall*)(uint8_t*))0x502550)(win); break;
            case 25:
                side ? Widget_OnHoverTimed(widget, 0, hx, hy, moved, track)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x562e40)(widget, 0, hx, hy, moved, track);
                break;
            case 26:
                side ? EditorDialog_ShowAchievements(win, 0, keep) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4cb2b0)(win, 0, keep);
                break;
            case 27:
                r = side ? EditorDialog_OnSettingsEvent(win, 0, evCopy) : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x4cb4a0)(win, 0, evCopy);
                break;
            case 28:
                r = side ? EditorDialog_OnCargoEvent(win, 0, evCopy) : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x4cb6f0)(win, 0, evCopy);
                break;
            case 29: side ? Window_HighlightMapModes(win) : ((void(__fastcall*)(uint8_t*))0x4dd410)(win); break;
            case 30: side ? OptionsDialog_ShowSound(win) : ((void(__fastcall*)(uint8_t*))0x4fdb90)(win); break;
            case 31: side ? OptionsDialog_FillResolutions(win) : ((void(__fastcall*)(uint8_t*))0x4fe4c0)(win); break;
            case 32: side ? TrainList_UpdateButtons(win) : ((void(__fastcall*)(uint8_t*))0x511170)(win); break;
            case 33: {
                int32_t* o = noOwner ? nullptr : &newOwner;
                int32_t* f = noFilter ? nullptr : &terr;
                side ? Territory_GiveToOwner(o, f, gkind) : ((void(__stdcall*)(int32_t*, int32_t*, int32_t))0x430040)(o, f, gkind);
                break;
            }
            case 34: side ? NewGameDialog_CountRows(win) : ((void(__fastcall*)(uint8_t*))0x4ca1c0)(win); break;
            case 35:
                side ? RulesDialog_FillConditionTypes(win, 0, condSel) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4d5d00)(win, 0, condSel);
                break;
            case 36: side ? RulesDialog_ShowEffect(win) : ((void(__fastcall*)(uint8_t*))0x4d4560)(win); break;
            case 37: side ? StationPicker_Fill(win) : ((void(__fastcall*)(uint8_t*))0x506f30)(win); break;
            case 38: side ? MpSetupDialog_Show(win) : ((void(__fastcall*)(uint8_t*))0x502910)(win); break;
            case 39:
                side ? StationView_CargoTooltip(tipId, 0, 0, 0) : ((void(__fastcall*)(int32_t, int, int32_t, int32_t))0x504770)(tipId, 0, 0, 0);
                break;
            case 40:
                side ? CompanyList_Fill(win) : ((void(__fastcall*)(uint8_t*))0x4c6c30)(win);
                break;
            case 41:
                side ? TrainOrders_CargoTooltip(orderTip, 0, 0, 0)
                     : ((void(__fastcall*)(int32_t, int, int32_t, int32_t))0x4b9fd0)(orderTip, 0, 0, 0);
                break;
            case 42: side ? TrainOrders_UpdateButtons(win) : ((void(__fastcall*)(uint8_t*))0x4b9a20)(win); break;
            case 43: side ? EditorDialog_ShowEconomy(win) : ((void(__fastcall*)(uint8_t*))0x4cadf0)(win); break;
            case 44: side ? MoneySliders_Read(rerange) : ((void(__fastcall*)(int32_t))0x47c220)(rerange); break;
            case 45:
                r = side ? EditorDialog_OnAchievementsEvent(win, 0, evCopy)
                         : ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))0x4cb8e0)(win, 0, evCopy);
                break;
            case 46: side ? NewGameDialog_ListHumanRows(win) : ((void(__fastcall*)(uint8_t*))0x4cc250)(win); break;
            case 47: side ? TrainList_Refill(win) : ((void(__fastcall*)(uint8_t*))0x512080)(win); break;
            case 48: side ? OptionsDialog_ShowVideo(win) : ((void(__fastcall*)(uint8_t*))0x4fe770)(win); break;
            case 49:
                side ? TrainPicker_Fill(win, 0, excludeTrain) : ((void(__fastcall*)(uint8_t*, int, uint32_t))0x5158f0)(win, 0, excludeTrain);
                break;
            case 50: side ? RulesDialog_UpdateScope(win) : ((void(__fastcall*)(uint8_t*))0x4d9970)(win); break;
            case 51: side ? RulesDialog_FillConditions(win) : ((void(__fastcall*)(uint8_t*))0x4d88f0)(win); break;
            case 52: r = side ? RulesDialog_StoreRule(win) : ((int32_t(__fastcall*)(uint8_t*))0x4d8ea0)(win); break;
            case 53: r = side ? ConditionDialog_Store(win) : ((int32_t(__fastcall*)(uint8_t*))0x4d67f0)(win); break;
            case 54: side ? RulesDialog_UpdateSlot(win) : ((void(__fastcall*)(uint8_t*))0x4da0f0)(win); break;
            case 55: side ? TerritoryEditor_Show(win, 0, terrId) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4cd070)(win, 0, terrId); break;
            case 56:
                side ? EditorDialog_FillIndustries(win, 0, indKeep) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4cd680)(win, 0, indKeep);
                break;
            case 57:
                side ? EditorDialog_FillHouses(win, 0, houseKeep) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4cdbd0)(win, 0, houseKeep);
                break;
            case 58: side ? NewGameDialog_SetupRows(win) : ((void(__fastcall*)(uint8_t*))0x4cc2d0)(win); break;
            case 59: {
                // The deleted row is freed with the exe's allocator: give each side a heap chain.
                auto xnew = (void*(__cdecl*)(size_t))0x53b070;
                auto xdel = (void(__cdecl*)(void*))0x53b080;
                uint8_t* rr = *(int32_t*)(win + 0x84) == -1 ? nullptr : (uint8_t*)Collection_Get(&rules3, 0, *(uint32_t*)(win + 0x84));
                uint8_t** hd = rr ? (uint8_t**)(rr + 4 + *(int32_t*)(win + 0x9c) * 4) : nullptr;
                if (hd) {
                    uint8_t** link = hd;
                    for (uint8_t* src = *hd; src; src = *(uint8_t**)(src + 0x24)) {
                        uint8_t* n = (uint8_t*)xnew(0x28);
                        memcpy(n, src, 0x28);
                        *link = n;
                        link = (uint8_t**)(n + 0x24);
                    }
                    *link = nullptr;
                }
                side ? RulesDialog_DeleteCondition(win) : ((void(__fastcall*)(uint8_t*))0x4da920)(win);
                if (hd) {
                    static uint8_t chainOut[2][8][0x24];
                    static int chainLen[2];
                    memset(chainOut[side], 0, sizeof chainOut[side]);
                    chainLen[side] = 0;
                    for (uint8_t* n = *hd; n;) {
                        uint8_t* next = *(uint8_t**)(n + 0x24);
                        if (chainLen[side] < 8) memcpy(chainOut[side][chainLen[side]], n, 0x24);
                        ++chainLen[side];
                        xdel(n);
                        n = next;
                    }
                    *hd = nullptr;
                    if (side) {
                        EXPECT_EQ(chainLen[0], chainLen[1], "op 59 chain length iteration %d", it);
                        EXPECT_EQ(memcmp(chainOut[0], chainOut[1], sizeof chainOut[0]), 0, "op 59 chain iteration %d", it);
                    }
                }
                break;
            }
            case 60: side ? RulesDialog_ShowSlot(win) : ((void(__fastcall*)(uint8_t*))0x4da9a0)(win); break;
            case 61: side ? RulesDialog_SwitchPage(win) : ((void(__fastcall*)(uint8_t*))0x4dab60)(win); break;
            case 62: side ? RulesDialog_ShowMessageKind(win) : ((void(__fastcall*)(uint8_t*))0x4d9390)(win); break;
            case 63: side ? CityEditor_Fill(win, 0, cityPick, cityKeep) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4cc980)(win, 0, cityPick, cityKeep); break;
            case 64: {
                Collection* inds = hook::Global<Collection*>(0x62b268);
                hook::Global<Collection*>(0x62b268) = nullptr; // no industry port rebuild
                side ? EditorDialog_ShowRegionTypes(win) : ((void(__fastcall*)(uint8_t*))0x4cf910)(win);
                hook::Global<Collection*>(0x62b268) = inds;
                break;
            }
            case 65: side ? ConditionDialog_ShowValue(win) : ((void(__fastcall*)(uint8_t*))0x4d6090)(win); break;
            case 66: side ? RulesDialog_ShowEffectType(win) : ((void(__fastcall*)(uint8_t*))0x4d4d10)(win); break;
            case 67: // the original's first row can show stale stack: start from a zeroed one
                ZeroStackBelow();
                side ? RulesDialog_FillTriggers(win) : ((void(__fastcall*)(uint8_t*))0x4d8120)(win);
                break;
            case 68: {
                for (int k = 0; k < 2; ++k) memcpy(Collection_Get(&players, 0, k + 1), faceSnap[k], 0xd00);
                for (int k = 0; k < 3; ++k) memcpy(Collection_Get(&companies, 0, k + 1), logoSnap[k], 0xe00);
                r = side ? FacePicker_OnClick(win, faceEv) : ((int32_t(__fastcall*)(uint8_t*, uint32_t*))0x4e2320)(win, faceEv);
                static uint8_t faceOut[2][2][0xd00], logoOut[2][3][0xe00];
                for (int k = 0; k < 2; ++k) memcpy(faceOut[side][k], Collection_Get(&players, 0, k + 1), 0xd00);
                for (int k = 0; k < 3; ++k) memcpy(logoOut[side][k], Collection_Get(&companies, 0, k + 1), 0xe00);
                if (side) {
                    EXPECT_EQ(FirstDiff(faceOut[0], faceOut[1], sizeof faceOut[0]), -1, "op 68 players iteration %d", it);
                    EXPECT_EQ(FirstDiff(logoOut[0], logoOut[1], sizeof logoOut[0]), -1, "op 68 companies iteration %d", it);
                }
                break;
            }
            case 69: {
                // The deleted trigger is freed with the exe's allocator: give each side a heap chain.
                auto xnew = (void*(__cdecl*)(size_t))0x53b070;
                auto xdel = (void(__cdecl*)(void*))0x53b080;
                uint8_t* rr = *(int32_t*)(win + 0x84) == -1 ? nullptr : (uint8_t*)Collection_Get(&rules3, 0, *(uint32_t*)(win + 0x84));
                uint8_t** hd = rr ? (uint8_t**)rr : nullptr;
                if (hd) {
                    uint8_t** link = hd;
                    for (uint8_t* src = *hd; src; src = *(uint8_t**)(src + 0x1a)) {
                        uint8_t* n = (uint8_t*)xnew(0x20);
                        memcpy(n, src, 0x20);
                        *link = n;
                        link = (uint8_t**)(n + 0x1a);
                    }
                    *link = nullptr;
                }
                side ? RulesDialog_DeleteTrigger(win) : ((void(__fastcall*)(uint8_t*))0x4d9ed0)(win);
                if (hd) {
                    static uint8_t tOut[2][8][0x1a];
                    static int tLen[2];
                    memset(tOut[side], 0, sizeof tOut[side]);
                    tLen[side] = 0;
                    for (uint8_t* n = *hd; n;) {
                        uint8_t* next = *(uint8_t**)(n + 0x1a);
                        if (tLen[side] < 8) memcpy(tOut[side][tLen[side]], n, 0x1a);
                        ++tLen[side];
                        xdel(n);
                        n = next;
                    }
                    *hd = nullptr;
                    if (side) {
                        EXPECT_EQ(tLen[0], tLen[1], "op 69 chain length iteration %d", it);
                        EXPECT_EQ(memcmp(tOut[0], tOut[1], sizeof tOut[0]), 0, "op 69 chain iteration %d", it);
                    }
                }
                break;
            }
            case 70: side ? RulesDialog_ShowMessage(win) : ((void(__fastcall*)(uint8_t*))0x4d9f50)(win); break;
            case 71: side ? RulesDialog_Reset(win) : ((void(__fastcall*)(uint8_t*))0x4daf40)(win); break;
            default:
                r = side ? Ui_ShowTooltip(ui, 0, newTip, prio, sticky, data)
                         : ((int32_t(__fastcall*)(uint8_t*, int, const char*, int32_t, int32_t, int32_t))0x538c70)(ui, 0, newTip, prio, sticky, data);
                break;
            }
            sent[side] = g_sent;
            memset(state[side], 0, sizeof state[side]);
            state[side][0] = (uint32_t)r;
            state[side][1] = *(uint32_t*)(win + 0x8c);
            state[side][2] = hook::Global<uint32_t>(0x6cff94);
            state[side][3] = *(uint32_t*)(ui + 0xc58);
            state[side][4] = *(uint32_t*)(ui + 0xbdc);
            state[side][5] = *(uint32_t*)(ui + 0xbe0);
            state[side][6] = *(uint32_t*)(ui + 0xbe4);
            tip[side] = *(char**)(ui + 0xbd8);
            ((void(__cdecl*)(void*))0x5a1145)(*(char**)(ui + 0xbd8));
            // The global tooltip (0x6d401c).
            state[side][7] = *(uint32_t*)(capHolder + 0xc58);
            state[side][8] = *(uint32_t*)(capHolder + 0xbdc);
            state[side][9] = *(uint32_t*)(capHolder + 0xbe0);
            state[side][10] = *(uint32_t*)(capHolder + 0xbe4);
            state[side][11] = *(uint32_t*)(capHolder + 0xc6d);
            state[side][12] = (op == 23 ? *(uint32_t*)(win + 0x78) : 0) ^ hook::Global<uint32_t>(0x6d14a8);
            state[side][13] = *(uint32_t*)(widget + 0xce) ^ (*(uint32_t*)(widget + 0xd2) << 1);
            state[side][14] = hook::Global<uint32_t>(0x6cebcc) ^ (hook::Global<uint32_t>(0x6cebd0) << 3);
            state[side][15] = hook::Global<uint32_t>(0x5f2c50) ^ (hook::Global<uint32_t>(0x6cec14) << 8) ^ (hook::Global<uint32_t>(0x6cec10) << 16);
            if (*(uint32_t*)(capHolder + 0xc58)) {
                tip[side] += std::string("|") + *(char**)(capHolder + 0xbd8);
                ((void(__cdecl*)(void*))0x5a1145)(*(char**)(capHolder + 0xbd8));
            }
            static uint8_t worldOut[2][0x7000];
            memcpy(worldOut[side], world, sizeof worldOut[0]);
            worldOut[side][0] ^= (uint8_t)hook::Global<int32_t>(0x621f50);
            if (side) EXPECT_EQ(FirstDiff(worldOut[0], worldOut[1], sizeof worldOut[0]), -1, "op %d world iteration %d", op, it);
            static uint8_t ownOut[2][3][4][0x10];
            for (int k = 0; k < 3; ++k) {
                memcpy(ownOut[side][k][0], (uint8_t*)Collection_Get(&buildings, 0, k + 1) + 0x276, 4);
                memcpy(ownOut[side][k][1], (uint8_t*)Collection_Get(&trainsTab, 0, k + 1) + 0x51, 4);
                memcpy(ownOut[side][k][2], (uint8_t*)Collection_Get(&nodes, 0, k + 1) + 0x22e, 2);
                memcpy(ownOut[side][k][3], (uint8_t*)Collection_Get(&trackStations, 0, k + 1) + 0x1e, 4);
            }
            if (side) EXPECT_EQ(memcmp(ownOut[0], ownOut[1], sizeof ownOut[0]), 0, "op %d owners iteration %d", op, it);
            state[side][12] ^= hook::Global<uint32_t>(0x621f48) << 12;
            state[side][12] ^= op == 60 ? *(uint32_t*)(win + 0x8c) << 20 : 0;
            state[side][12] ^= op == 62 ? *(uint32_t*)(win + 0x9c) << 24 : 0;
            state[side][12] ^= op == 66 ? *(uint32_t*)(win + 0x78) * 3 : 0;
            state[side][12] ^= op == 63 ? hook::Global<uint32_t>(0x6cff74) * 7 + hook::Global<uint32_t>(0x6cff78) * 13 : 0;
            state[side][12] ^= (op == 51 ? *(uint32_t*)(win + 0x98) : 0);
            state[side][12] ^= (op == 35 ? *(uint32_t*)(win + 0x7c) : 0) ^ (hook::Global<uint32_t>(0x6d07c8) << 4);
            static uint8_t ruleOut[2][3][0x900];
            for (int k = 0; k < 3; ++k) memcpy(ruleOut[side][k], Collection_Get(&rules3, 0, k + 1), 0x900);
            if (side) EXPECT_EQ(FirstDiff(ruleOut[0], ruleOut[1], sizeof ruleOut[0]), -1, "op %d rules iteration %d", op, it);
            static uint8_t condOut[2][0x30];
            memcpy(condOut[side], condNode, 0x30);
            if (side) EXPECT_EQ(FirstDiff(condOut[0], condOut[1], 0x30), -1, "op %d condition iteration %d", op, it);
            static uint32_t cbOut[2][16];
            memcpy(cbOut[side], g_cbLog, sizeof g_cbLog);
            if (side) EXPECT_EQ(memcmp(cbOut[0], cbOut[1], sizeof g_cbLog), 0, "op %d callbacks iteration %d", op, it);
            static uint8_t cityOut[2][24][0x20];
            for (int k = 0; k < 24; ++k) memcpy(cityOut[side][k], (uint8_t*)Collection_Get(&cities, 0, k + 1) + 0x23a, 0x20);
            if (side) EXPECT_EQ(memcmp(cityOut[0], cityOut[1], sizeof cityOut[0]), 0, "op %d cities iteration %d", op, it);
            memcpy(ngOut[side], newGame, 0x100);
            memcpy(setupOut[side], setup, 0x400);
            for (int k = 0; k < 24; ++k) memcpy(regOut[side][k], Collection_Get(&regions, 0, k + 1), 0x48);
        }
        for (size_t k = 0; k + 12 < sent[0].size() && k + 12 < sent[1].size(); k += 13) {
            EXPECT_EQ(strcmp(g_texts[0][k / 13].c_str(), g_texts[1][k / 13].c_str()), 0, "op %d text iteration %d: '%s' vs '%s'", op, it,
                      g_texts[0][k / 13].c_str(), g_texts[1][k / 13].c_str());
            uint32_t type = sent[0][k + 1];
            if (type == 0x88 || type == 0x6f || type == 0x6e || type == 0x89 || type == 0x70 || type == 0x75)
                sent[0][k + 3] = sent[1][k + 3] = sent[0][k + 5] = sent[1][k + 5] = 0;
            // The tooltip event leaves all but its type and text uninitialized (stack).
            if (type == 0xae)
                for (int j = 2; j <= 12; ++j) sent[0][k + j] = sent[1][k + j] = 0;
        }
        EXPECT_EQ(sent[0] == sent[1], true, "op %d events iteration %d (%d vs %d)", op, it, (int)sent[0].size(), (int)sent[1].size());
        EXPECT_EQ(memcmp(state[0], state[1], sizeof state[0]), 0, "op %d state iteration %d", op, it);
        EXPECT_EQ(tip[0] == tip[1], true, "op %d tip iteration %d", op, it);
        EXPECT_EQ(memcmp(ngOut[0], ngOut[1], 0x100), 0, "op %d new game iteration %d", op, it);
        EXPECT_EQ(memcmp(setupOut[0], setupOut[1], 0x400), 0, "op %d setup iteration %d", op, it);
        EXPECT_EQ(memcmp(regOut[0], regOut[1], sizeof regOut[0]), 0, "op %d territories iteration %d", op, it);
    }
    *enumIat = savedEnum;
    VirtualProtect(enumIat, 4, enumProt, &enumProt);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    for (size_t k = 0; k < sizeof addrs / 4; ++k) hook::Global<uint32_t>(addrs[k]) = saved[k];
    for (TabDef& t : tabs) Collection_Destruct(t.c);
    VirtualFree(pack, 0, MEM_RELEASE);
    memcpy(g_stState, stStateSaved, sizeof g_stState);
    memset(g_ansId, 0, sizeof g_ansId);
    memcpy((void*)0x6cfd38, cargoFlagsSaved, sizeof cargoFlagsSaved);
    {
        DWORD dp;
        VirtualProtect((void*)0x4c6e4a, 6, PAGE_EXECUTE_READWRITE, &dp);
        memcpy((void*)0x4c6e4a, dswSaved, 6);
        VirtualProtect((void*)0x4c6e4a, 6, dp, &dp);
        FlushInstructionCache(GetCurrentProcess(), (void*)0x4c6e4a, 6);
    }
}

namespace rt3 {
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Pack_FreeSlot(uint8_t* pack, int, int32_t idx);
void __fastcall Texture_ReleaseHandles(uint8_t* t);
}

namespace {
int g_comReleases;
uint32_t __stdcall FakeComRelease(void*) { return (uint32_t)++g_comReleases; }
}

DIFFTEST(game_PackRelease) {
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    auto xdel = (void(__cdecl*)(void*))0x53b080;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* savedPack = hook::Global<uint8_t*>(0x6d4020);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static void* comVtbl[4] = { nullptr, nullptr, (void*)&FakeComRelease, nullptr };
    static uint8_t comObj[8];
    *(void***)comObj = comVtbl;
    const int N = 8;
    static uint8_t table[2][N * 0xd];
    for (int it = 0; it < 3000; ++it) {
        // Slot plan: kind 0 / 5 / 7 / 9, refcount 0..3; kind-7 objects are textures,
        // either pack-owned (pointing at another slot's object) or holding the fake COM object.
        uint8_t kinds[N], refs[N], owned[N], target[N], keep[16];
        for (int k = 0; k < N; ++k) {
            const uint8_t ks[4] = { 0, 5, 7, 9 };
            kinds[k] = ks[Rnd() % 4];
            refs[k] = (uint8_t)(Rnd() % 4);
            owned[k] = (uint8_t)(Rnd() % 2);
            target[k] = (uint8_t)(Rnd() % N);
        }
        for (uint8_t& b : keep) b = (uint8_t)(Rnd() % 3 == 0);
        int pick = (int)(Rnd() % (N + 1)); // N: an unknown object
        int mode = (int)(Rnd() % 3);        // 0 release, 1 free slot, 2 texture handles
        uint32_t logs[2][N * 4 + 2];
        for (int side = 0; side < 2; ++side) {
            g_comReleases = 0;
            uint8_t* objs[N];
            for (int k = 0; k < N; ++k) objs[k] = (uint8_t*)xnew(0x20);
            for (int k = 0; k < N; ++k) {
                memset(objs[k], 0, 0x20);
                *(uint32_t*)(objs[k] + 0x1c) = 0x1000 + k; // identity
            }
            for (int k = 0; k < N; ++k) {
                uint8_t* e = table[side] + k * 0xd;
                memset(e, 0, 0xd);
                *(uint32_t*)e = refs[k];
                e[8] = kinds[k];
                *(uint8_t**)(e + 9) = objs[k];
                if (kinds[k] == 7) {
                    // Owned textures only reference non-texture slots (a texture cycle would free twice).
                    *(int32_t*)(objs[k] + 0x10) = owned[k] && target[k] != k && kinds[target[k]] != 7;
                    *(uint8_t**)(objs[k] + 0x18) = objs[target[k]];
                    *(uint8_t**)objs[k] = owned[k] ? nullptr : comObj;
                }
            }
            *(uint8_t**)(pack + 0x1f400) = table[side];
            *(int32_t*)(pack + 0x427b4) = N;
            for (int k = 0; k < 16; ++k) *(int32_t*)(pack + 0x4278c + k * 4) = keep[k];
            void* obj = pick < N ? (void*)objs[pick] : (void*)comVtbl;
            int slot = pick < N ? pick : 0;
            uint8_t* tex = objs[slot];
            if (mode == 2) { // a free-standing texture (not in the table)
                tex = (uint8_t*)xnew(0x20);
                memset(tex, 0, 0x20);
                *(int32_t*)(tex + 0x10) = owned[slot] && kinds[target[slot]] != 7;
                *(uint8_t**)(tex + 0x18) = objs[target[slot]];
                *(uint8_t**)tex = comObj;
            }
            if (side == 0) {
                if (mode == 0) ((void(__fastcall*)(uint8_t*, int, void*))0x53c000)(pack, 0, obj);
                else if (mode == 1) ((void(__fastcall*)(uint8_t*, int, int32_t))0x53bf60)(pack, 0, slot);
                else ((void(__fastcall*)(uint8_t*))0x5512a0)(tex);
            } else {
                if (mode == 0) Pack_Release(pack, 0, obj);
                else if (mode == 1) Pack_FreeSlot(pack, 0, slot);
                else Texture_ReleaseHandles(tex);
            }
            // Log the table by object identity (freed objects show as 0 after clearing).
            for (int k = 0; k < N; ++k) {
                uint8_t* e = table[side] + k * 0xd;
                logs[side][k * 4] = *(uint32_t*)e;
                logs[side][k * 4 + 1] = e[8]; // (+0xc is the object pointer's top byte)
                logs[side][k * 4 + 2] = *(uint32_t*)(e + 4);
                logs[side][k * 4 + 3] = *(uint8_t**)(e + 9) ? *(uint32_t*)(*(uint8_t**)(e + 9) + 0x1c) : 0;
            }
            logs[side][N * 4] = (uint32_t)g_comReleases;
            logs[side][N * 4 + 1] = mode == 2 ? (*(uint32_t*)(tex + 0x18) != 0) | ((*(uint32_t*)tex != 0) << 1) : 0;
            // Free what is left: objects still in the table (deleted ones were cleared).
            for (int k = 0; k < N; ++k) {
                uint8_t* o = *(uint8_t**)(table[side] + k * 0xd + 9);
                if (o) xdel(o);
            }
            if (mode == 2) xdel(tex);
        }
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "pack mode %d pick %d iteration %d", mode, pick, it);
    }
    hook::Global<uint8_t*>(0x6d4020) = savedPack;
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
uint8_t* __fastcall Pack_Acquire(uint8_t* pack, int, int32_t kind, int32_t key);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall Pack_RecreateFonts(uint8_t* pack);
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);
}

namespace {
template <class T> T& AF(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
// Stands in for the D3DX font creation at 0x5711b5 (stdcall device, HFONT, out).
int g_d3dxFonts;
int32_t __stdcall FakeD3DXCreateFont(void*, void* hf, void** out) {
    *out = hf ? (void*)0x51 : nullptr;
    ++g_d3dxFonts;
    return 0;
}
}

DIFFTEST(game_PackAcquire) {
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    auto xdel = (void(__cdecl*)(void*))0x53b080;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_acq.bin");
    { FILE* w = fopen(path, "wb"); fwrite(path, 1, 64, w); fclose(w); }
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* g = (uint8_t*)VirtualAlloc(nullptr, 0x115000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    uint8_t* sG = hook::Global<uint8_t*>(0x6d4024);
    int32_t sGdi = hook::Global<int32_t>(0x6d4034);
    char sName[0x40];
    memcpy(sName, (void*)0xd93858, sizeof sName);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint8_t*>(0x6d4024) = g;
    hook::Global<int32_t>(0x6d4034) = 0;
    static uint8_t modes[4 * 0x15];
    AF<uint8_t*>(g, 0x34) = modes;
    // Bitmap font: glyph sliders (+0x24, count +0x28) sharing one frame.
    static uint8_t frame[0x40];
    static uint8_t glyphs[0x60][0x28];
    static uint32_t glyphPtrs[0x60];
    static uint8_t bfont[0x40];
    for (int k = 0; k < 0x60; ++k) glyphPtrs[k] = (uint32_t)(uintptr_t)glyphs[k];
    AF<uint32_t*>(bfont, 0x24) = glyphPtrs;
    AF<uint16_t>(bfont, 0x28) = 0x60;
    int fonts = 0;
    uint8_t* stub = (uint8_t*)0x5711b5;
    uint8_t stubSaved[5];
    DWORD prot;
    VirtualProtect(stub, 5, PAGE_EXECUTE_READWRITE, &prot);
    memcpy(stubSaved, stub, 5);
    stub[0] = 0xE9;
    *(int32_t*)(stub + 1) = (int32_t)((uintptr_t)&FakeD3DXCreateFont - (0x5711b5 + 5));
    int32_t sLang = hook::Global<int32_t>(0x6d3c84);
    for (int it = 0; it < 3000; ++it) {
        int n = 1 + (int)(Rnd() % 8);
        int32_t keys[8], kinds[8], refs[8], tags[8];
        bool full = Rnd() % 3 == 0;
        for (int k = 0; k < n; ++k) {
            bool freeSlot = !full && Rnd() % 3 == 0;
            keys[k] = freeSlot ? 0 : (int32_t)(Rnd() % 6) + 1;
            kinds[k] = (int32_t)(Rnd() % 8);
            refs[k] = freeSlot ? 0 : (int32_t)(Rnd() % 3);
            tags[k] = freeSlot ? 0 : (Rnd() % 4 ? 0x1000 + k : 0); // object or none
            if (!freeSlot && !tags[k] && !refs[k]) refs[k] = 1;
        }
        int mode = (int)(Rnd() % 8);         // 0 by key, 1 font (kind 7), 2 named, 3 other kind new key, 4 GDI font,
                                             // 5 recreate the fonts, 6 collision file by name
        int32_t key = mode == 0 ? (int32_t)(Rnd() % 8) : 0x100 + (int32_t)(Rnd() % 50);
        int32_t kind = (int32_t)(Rnd() % 10);
        if (kind == 1 || kind == 6 || kind == 7) kind = 0;
        int fontIdx = (int)(Rnd() % 22);
        if (mode == 1 || mode == 4) { kind = 7; key = 0x754d4c0 + fontIdx; }
        hook::Global<int32_t>(0x6d4034) = mode == 4;
        hook::Global<int32_t>(0x6d3c84) = Rnd() % 2 ? 0xc : 0;
        if (mode == 4 && Rnd() % 2) fontIdx = 8;
        // Screen mode for the font's width suffix.
        const int32_t ws[6][2] = { { 1600, 1200 }, { 1280, 1024 }, { 1024, 768 }, { 800, 600 }, { 640, 480 }, { 1152, 864 } };
        int32_t mi = (int32_t)(Rnd() % 4);
        for (int k = 0; k < 4; ++k) {
            int w = (int)(Rnd() % 6);
            AF<int32_t>(modes + k * 0x15, 0) = ws[w][0];
            AF<int32_t>(modes + k * 0x15, 4) = Rnd() % 8 ? ws[w][1] : 600;
        }
        AF<int32_t>(g, 0x11421e) = mi;
        AF<float>(frame, 0x2e) = RndF(0.5f, 2.0f);
        AF<float>(frame, 0x32) = RndF(0.5f, 2.0f);
        for (int k = 0; k < 0x60; ++k) {
            uint8_t* s = glyphs[k];
            AF<uint8_t*>(s, 0) = frame;
            for (int o = 4; o <= 0x20; o += 4) AF<float>(s, o) = RndF(-1.0f, 1.0f);
            s[0x24] = (uint8_t)(Rnd() % 2);
        }
        // The bitmap font a kind-7 font asks for, under its name hash.
        char bname[0x20];
        {
            int32_t width = 1600;
            uint8_t* m = modes + mi * 0x15;
            for (auto& sz : ws)
                if (&sz - ws < 5 && sz[0] == AF<int32_t>(m, 0) && sz[1] == AF<int32_t>(m, 4)) width = sz[0];
            char num[0x10];
            rt3::FormatText(bname, (const char*)0x5ddc68, *(const char**)(0x625470 + fontIdx * 40),
                            rt3::Num_Format(num, 8, (double)width, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        }
        int32_t bkey = rt3::NameTable_Hash(pack, 0, bname);
        int bslot = mode == 1 || mode == 2 || mode == 5 || mode == 6 || mode == 7 ? (int)(Rnd() % n) : -1;
        int fslot = (mode == 5 || mode == 7) && n > 1 ? (bslot + 1) % n : -1;
        static uint8_t fontRec[2][0x84];
        int32_t ckey = rt3::NameTable_Hash(pack, 0, "Coll_Rock.cof");
        uint32_t logs[2][120];
        uint8_t fontOut[2][0x84];
        char nameOut[2][0x40];
        for (int side = 0; side < 2; ++side) {
            uint8_t* objs[8];
            uint8_t* table = (uint8_t*)xmalloc(n * 0xd);
            for (int k = 0; k < n; ++k) {
                uint8_t* e = table + k * 0xd;
                memset(e, 0, 0xd);
                objs[k] = nullptr;
                AF<int32_t>(e, 0) = refs[k];
                AF<int32_t>(e, 4) = keys[k];
                e[8] = (uint8_t)((mode == 5 || mode == 7) && kinds[k] == 7 ? 5 : kinds[k]); // loaded fonts always have an object
                if (tags[k]) {
                    objs[k] = (uint8_t*)xnew(0x40);
                    memset(objs[k], 0, 0x40);
                    AF<int32_t>(objs[k], 0x3c) = tags[k];
                }
                if (k == fslot) {
                    memset(fontRec[side], 0, 0x84);
                    AF<int32_t>(fontRec[side], 0xc) = fontIdx;
                    AF<int32_t>(fontRec[side], 0x10) = 1;
                    AF<float>(fontRec[side], 0x14) = 1.0f;
                    if (mode == 7) AF<uint8_t*>(fontRec[side], 0x18) = bfont;
                    AF<int32_t>(e, 0) = 1;
                    e[8] = 7;
                    objs[k] = fontRec[side];
                }
                if (k == bslot) {
                    AF<int32_t>(e, 4) = mode == 6 ? ckey : bkey;
                    e[8] = mode == 6 ? 1 : 6;
                    AF<int32_t>(e, 0) = 1 + refs[k];
                    objs[k] = bfont;
                }
                AF<uint8_t*>(e, 9) = objs[k];
            }
            AF<uint8_t*>(pack, 0x1f400) = table;
            AF<int32_t>(pack, 0x427b4) = n;
            AF<int32_t>(pack, 0x427bc) = 0;
            AF<uint8_t*>(pack, 0x20428) = nullptr;
            FILE* f = xfopen(path, "rb");
            AF<int32_t>(pack, 0x204ac) = AF<int32_t>(f, 0x10);
            uint8_t* r;
            if (mode == 7) {
                r = nullptr;
                side ? rt3::Pack_ReleaseFontHandles(pack) : ((void(__fastcall*)(uint8_t*))0x53b920)(pack);
            } else if (mode == 5) {
                r = nullptr;
                side ? rt3::Pack_RecreateFonts(pack) : ((void(__fastcall*)(uint8_t*))0x53b8d0)(pack);
            } else if (mode == 6) {
                r = side ? rt3::Pack_AcquireCollision(pack, 0, "Coll_Rock.cof")
                         : ((uint8_t*(__fastcall*)(uint8_t*, int, const char*))0x53d0f0)(pack, 0, "Coll_Rock.cof");
            } else if (side == 0) {
                r = mode == 2 ? ((uint8_t*(__fastcall*)(uint8_t*, int, const char*))0x53d110)(pack, 0, bname)
                              : ((uint8_t*(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x53c070)(pack, 0, kind, key);
            } else {
                r = mode == 2 ? rt3::Pack_AcquireNamed(pack, 0, bname) : rt3::Pack_Acquire(pack, 0, kind, key);
            }
            xfclose(f);
            uint8_t* fontObj = nullptr;
            auto ident = [&](uint8_t* o) -> uint32_t {
                if (!o) return 0;
                if (o == bfont) return 0xbf;
                if (o == fontRec[side]) return 0xfe;
                for (int k = 0; k < n; ++k)
                    if (o == objs[k]) return (uint32_t)AF<int32_t>(o, 0x3c);
                fontObj = o;
                return 0xf0;
            };
            int cnt = AF<int32_t>(pack, 0x427b4);
            uint8_t* t2 = AF<uint8_t*>(pack, 0x1f400);
            memset(logs[side], 0, sizeof logs[side]);
            logs[side][0] = (uint32_t)cnt;
            logs[side][1] = ident(r);
            for (int k = 0; k < cnt && k < 28; ++k) {
                uint8_t* e = t2 + k * 0xd;
                logs[side][2 + k * 4] = AF<uint32_t>(e, 0);
                logs[side][3 + k * 4] = AF<uint32_t>(e, 4);
                logs[side][4 + k * 4] = e[8];
                logs[side][5 + k * 4] = ident(AF<uint8_t*>(e, 9));
            }
            memset(fontOut[side], 0, 0x84);
            if (fontObj) {
                // Only the fields the constructor sets (the rest is allocator garbage).
                memcpy(fontOut[side], fontObj, mode == 4 ? 0x18 : 0x34);
                memcpy(fontOut[side] + 0x40, fontObj + 0x40, 0x2c);
                if (mode == 4) {
                    AF<uint32_t>(fontOut[side], 4) = AF<void*>(fontObj, 4) != nullptr;
                    if (AF<void*>(fontObj, 4)) (*(int(__stdcall**)(void*))0x5c8058)(AF<void*>(fontObj, 4));
                } else {
                    memset(fontOut[side] + 4, 0, 8);
                    AF<uint32_t>(fontOut[side], 0x18) = AF<uint8_t*>(fontObj, 0x18) == bfont;
                }
                ++fonts;
            }
            memcpy(nameOut[side], (void*)0xd93858, 0x40);
            if (fontObj) xdel(fontObj);
            for (int k = 0; k < n; ++k)
                if (objs[k] && objs[k] != bfont && objs[k] != fontRec[side]) xdel(objs[k]);
            xfree(AF<void*>(pack, 0x1f400));
            EXPECT_EQ(AF<uint8_t*>(pack, 0x20428) == nullptr, true, "position stack iteration %d", it);
        }
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "table mode %d kind %d iteration %d", mode, kind, it);
        EXPECT_EQ(memcmp(fontOut[0], fontOut[1], 0x84), 0, "font %d iteration %d", fontIdx, it);
        if (fslot >= 0) {
            for (int sd = 0; sd < 2; ++sd) AF<uint32_t>(fontRec[sd], 0x18) = AF<uint8_t*>(fontRec[sd], 0x18) == bfont;
            EXPECT_EQ(memcmp(fontRec[0], fontRec[1], 0x84), 0, "recreated font %d iteration %d at %#x", fontIdx, it, FirstDiff(fontRec[0], fontRec[1], 0x84));
        }
        EXPECT_EQ(memcmp(nameOut[0], nameOut[1], 0x40), 0, "name iteration %d", it);
    }
    EXPECT_EQ(fonts > 400, true, "fonts built %d", fonts);
    EXPECT_EQ(g_d3dxFonts > 200, true, "GDI fonts built %d", g_d3dxFonts);
    memcpy(stub, stubSaved, 5);
    VirtualProtect(stub, 5, prot, &prot);
    hook::Global<int32_t>(0x6d3c84) = sLang;
    memcpy((void*)0xd93858, sName, sizeof sName);
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    hook::Global<uint8_t*>(0x6d4024) = sG;
    hook::Global<int32_t>(0x6d4034) = sGdi;
    VirtualFree(pack, 0, MEM_RELEASE);
    VirtualFree(g, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
uint8_t* __fastcall MapView_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Scrollbar_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall ListBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall EditBox_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Picture_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TreeView_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Button_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
}

DIFFTEST(game_WidgetConstructors) {
    typedef uint8_t*(__fastcall * LoadFn)(uint8_t*, int, int32_t);
    typedef uint8_t*(__fastcall * DefFn)(uint8_t*, int, const uint8_t*);
    // Loaders: exe address, rewrite, record size, text block (0 none, 1 edit box +0xba,
    // 2 button +0xb2, 3 control +0xce).
    struct L { uint32_t addr; LoadFn fn; int32_t size; int text; };
    const L loads[9] = {
        { 0x47b070, rt3::MapView_Load, 0x60, 0 },     { 0x559f90, rt3::Scrollbar_Load, 0x89, 0 },
        { 0x55a990, rt3::ListBox_Load, 0xe3, 0 },     { 0x55b900, rt3::EditBox_Load, 0x91, 1 },
        { 0x55ecb0, rt3::Picture_Load, 0x87, 0 },     { 0x55fcc0, rt3::TreeView_Load, 0x12f, 0 },
        { 0x563120, rt3::TimerWidget_Load, 0x8b, 0 }, { 0x563c00, rt3::Button_Load, 0xa1, 2 },
        { 0x5402a0, rt3::Control_Load, 0xa5, 3 },
    };
    // Definition constructors: exe address, rewrite, heap text field and its def source.
    struct D { uint32_t addr; DefFn fn; uint32_t text, src; };
    const D defs[8] = {
        { 0x47b0d0, rt3::MapView_Construct, 0, 0 },        { 0x55a040, rt3::Scrollbar_Construct, 0, 0 },
        { 0x55ab50, rt3::ListBox_Construct, 0, 0 },        { 0x55ba70, rt3::EditBox_Construct, 0xba, 0x77 },
        { 0x55ff70, rt3::TreeView_Construct, 0, 0 },       { 0x563210, rt3::TimerWidget_Construct, 0, 0 },
        { 0x563d70, rt3::Button_Construct, 0xb2, 0x6f },   { 0x540430, rt3::Control_Construct, 0xce, 0x8b },
    };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_widget.bin");
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    uint8_t* sMap = hook::Global<uint8_t*>(0x62c120);
    int32_t sPictures = hook::Global<int32_t>(0xd9797c);
    static uint8_t map[0x2200];
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint8_t*>(0x62c120) = map;
    // Pack slots: fonts 0..7 and three bitmap fonts by name (fake objects, never touched).
    static const char* names[3] = { "Frame_1600.imb", "slider", "BTN" };
    const int N = 14;
    uint8_t tmpl[N * 0xd];
    memset(tmpl, 0, sizeof tmpl);
    for (int k = 0; k < 8; ++k) {
        uint8_t* e = tmpl + k * 0xd;
        *(int32_t*)(e + 4) = 0x754d4c0 + k;
        e[8] = 7;
        *(uint32_t*)(e + 9) = 0x7100 + k;
        *(uint32_t*)e = 1;
    }
    for (int k = 0; k < 3; ++k) {
        uint8_t* e = tmpl + (8 + k) * 0xd;
        *(int32_t*)(e + 4) = rt3::NameTable_Hash(pack, 0, names[k]);
        e[8] = 6;
        *(uint32_t*)(e + 9) = 0x6100 + k;
        *(uint32_t*)e = 2;
    }
    static char strs[4][16] = { "OK", "", "Cancel game", "x" };
    static uint8_t def[0x200];
    static uint8_t wobj[2][0x200];
    for (int it = 0; it < 4000; ++it) {
        bool isLoad = Rnd() % 2 == 0;
        const L& ld = loads[Rnd() % 9];
        const D& df = defs[Rnd() % 8];
        *(int32_t*)(map + 0x2155) = (int32_t)(Rnd() % 300);
        *(int32_t*)(map + 0x2159) = (int32_t)(Rnd() % 300);
        // The stream: the record (font byte +0x33 0..7), then a text block.
        uint8_t stream[0x200];
        for (uint8_t& b : stream) b = (uint8_t)Rnd();
        if (Rnd() % 4 == 0) stream[0x33] = 0;
        else stream[0x33] %= 8;
        int textLen = Rnd() % 3 ? (int)(Rnd() % 12) + 1 : 0;
        uint8_t* blk = stream + ld.size;
        *(int32_t*)(blk + 4) = textLen;
        for (int k = 0; k < textLen; ++k) blk[8 + k] = (uint8_t)('a' + Rnd() % 26);
        if (textLen) blk[8 + textLen - 1] = 0;
        { FILE* w = fopen(path, "wb"); fwrite(stream, 1, sizeof stream, w); fclose(w); }
        // The definition: font 0..7, names null / strings / bitmap font names.
        for (uint8_t& b : def) b = (uint8_t)Rnd();
        if (Rnd() % 4 == 0) def[0x33] = 0;
        else def[0x33] %= 8;
        *(const char**)(def + 0x3f) = Rnd() % 3 ? strs[Rnd() % 4] : nullptr;
        *(const char**)(def + 0x43) = Rnd() % 2 ? names[Rnd() % 3] : nullptr;
        if (df.src) *(const char**)(def + df.src) = Rnd() % 4 || df.text == 0xba ? strs[Rnd() % 4] : nullptr;
        uint8_t fill = (uint8_t)Rnd();
        int32_t picCount[2];
        uint8_t tables[2][N * 0xd];
        char texts[2][3][32];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(sizeof tmpl);
            memcpy(table, tmpl, sizeof tmpl);
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = N;
            *(int32_t*)(pack + 0x427bc) = 0;
            *(uint8_t**)(pack + 0x20428) = nullptr;
            hook::Global<int32_t>(0xd9797c) = 5;
            FILE* f = xfopen(path, "rb");
            *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)f + 0x10);
            uint8_t* w = wobj[side];
            memset(w, fill, sizeof wobj[side]);
            uint8_t* r;
            if (isLoad) r = side ? ld.fn(w, 0, 0) : ((LoadFn)ld.addr)(w, 0, 0);
            else r = side ? df.fn(w, 0, def) : ((DefFn)df.addr)(w, 0, def);
            xfclose(f);
            EXPECT_EQ(r == w, true, "result iteration %d", it);
            picCount[side] = hook::Global<int32_t>(0xd9797c);
            memcpy(tables[side], table, sizeof tmpl);
            xfree(table);
            // Heap strings: compare by contents, then free.
            uint32_t heap[3] = { 0, 0, 0 };
            if (!isLoad) heap[0] = 0x43, heap[1] = 0x47, heap[2] = df.text;
            else heap[2] = ld.text == 1 ? 0xba : ld.text == 2 ? 0xb2 : ld.text == 3 ? 0xce : 0;
            for (int k = 0; k < 3; ++k) {
                memset(texts[side][k], 0, 32);
                if (!heap[k]) continue;
                char* s = *(char**)(w + heap[k]);
                if (s) {
                    strncpy(texts[side][k], s, 31);
                    xfree(s);
                } else {
                    strcpy(texts[side][k], "<null>");
                }
                *(char**)(w + heap[k]) = nullptr;
            }
        }
        EXPECT_EQ(memcmp(wobj[0], wobj[1], sizeof wobj[0]), 0, "widget %s %x iteration %d", isLoad ? "load" : "def",
                  isLoad ? ld.addr : df.addr, it);
        EXPECT_EQ(memcmp(texts[0], texts[1], sizeof texts[0]), 0, "texts %x iteration %d", isLoad ? ld.addr : df.addr, it);
        EXPECT_EQ(memcmp(tables[0], tables[1], sizeof tables[0]), 0, "pack table iteration %d", it);
        EXPECT_EQ(picCount[0], picCount[1], "picture count iteration %d", it);
    }
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    hook::Global<uint8_t*>(0x62c120) = sMap;
    hook::Global<int32_t>(0xd9797c) = sPictures;
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
}

DIFFTEST(game_CursorSet) {
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, prot, &prot);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    void* sEng = hook::Global<void*>(0x6d4024);
    int32_t sShown = hook::Global<int32_t>(0x624b20);
    void* sSlots[0x2e];
    memcpy(sSlots, (void*)0xccba60, sizeof sSlots);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    *(int32_t*)(pack + 0x4278c + 6 * 4) = 1; // keep the (fake) bitmap font loaded
    hook::Global<void*>(0x6d4024) = nullptr;
    void* arrow = (*(void*(__stdcall**)(void*, uint32_t))0x5c82a0)(nullptr, 0x7f00);
    const int N = 4;
    uint8_t tmpl[N * 0xd];
    memset(tmpl, 0, sizeof tmpl);
    // Slot 0: the cursor bitmap font; 1, 2: other resources (kind 5: freed without delete).
    *(int32_t*)(tmpl + 4) = rt3::NameTable_Hash(pack, 0, "MouseCursor.imb");
    tmpl[8] = 6;
    *(uint32_t*)(tmpl + 9) = 0x6100;
    for (int k = 1; k < 3; ++k) {
        *(int32_t*)(tmpl + k * 0xd + 4) = 0x50 + k;
        tmpl[k * 0xd + 8] = 5;
        *(uint32_t*)(tmpl + k * 0xd + 9) = 0x6100 + k;
    }
    for (int it = 0; it < 6000; ++it) {
        uint32_t c0[10];
        for (uint32_t& v : c0) v = Rnd();
        int32_t idx = (int32_t)(Rnd() % 26) - 3; // 23 cursors
        int32_t force = Rnd() % 3 == 0, now = Rnd() % 3 == 0;
        c0[1] = Rnd() % 3 ? (uint32_t)(Rnd() % 23) : (uint32_t)idx; // current cursor
        c0[3] = Rnd() % 2 ? 0 : c0[3];                             // busy
        c0[5] = Rnd() % 2 ? 0 : c0[5];                             // colour
        c0[6] = Rnd() % 2 ? 0 : c0[6];                             // game-drawn
        uint32_t held = Rnd() % 4;
        c0[0] = held == 3 ? 0 : 0x6100 + held;
        uint32_t refs[N];
        for (int k = 0; k < N; ++k) refs[k] = Rnd() % 3 + (k < 3 && c0[0] == 0x6100u + k);
        bool missing = Rnd() % 10 == 0;
        int32_t shown = (int32_t)(Rnd() % 3) - 1;
        g_fakeTicks = Rnd();
        uint32_t out[2][10];
        uint8_t tabs[2][N * 0xd];
        int32_t shownOut[2];
        bool slotSet[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(sizeof tmpl);
            memcpy(table, tmpl, sizeof tmpl);
            for (int k = 0; k < N; ++k) *(uint32_t*)(table + k * 0xd) = refs[k];
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = N;
            for (int k = 0; k < 0x2e; ++k) ((void**)0xccba60)[k] = arrow;
            int32_t ci = idx < 0 ? 0 : idx;
            void** slot = (void**)0xccba60 + (c0[5] == 0) * 0x17 + ci;
            if (missing) *slot = nullptr;
            hook::Global<int32_t>(0x624b20) = shown;
            memcpy(out[side], c0, sizeof c0);
            if (side) rt3::Cursor_Set((uint8_t*)out[side], 0, idx, force, now);
            else ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x53f000)((uint8_t*)out[side], 0, idx, force, now);
            slotSet[side] = *slot != nullptr;
            shownOut[side] = hook::Global<int32_t>(0x624b20);
            memcpy(tabs[side], table, sizeof tmpl);
            xfree(table);
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "cursor %d force %d now %d iteration %d", idx, force, now, it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "pack iteration %d", it);
        EXPECT_EQ(shownOut[0], shownOut[1], "shown iteration %d", it);
        EXPECT_EQ(slotSet[0], slotSet[1], "cursor loaded iteration %d", it);
    }
    memcpy((void*)0xccba60, sSlots, sizeof sSlots);
    hook::Global<int32_t>(0x624b20) = sShown;
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    hook::Global<void*>(0x6d4024) = sEng;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Company_LiveStat(uint8_t* c, int, int32_t id);
double __fastcall Company_DerivedStat(uint8_t* c, int, int32_t id, int32_t year);
double __fastcall Company_YearEstimate(uint8_t* c, int, int32_t id);
float __fastcall Company_BookValue(uint8_t* c);
float __fastcall Company_GrowthScore(uint8_t* c);
float __fastcall Company_CreditRating(uint8_t* c, int, int32_t extraDebt);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
float __fastcall Company_BondRate(uint8_t* c);
float __fastcall Company_BondRateWithDebt(uint8_t* c, int, int32_t extraDebt, int32_t);
int32_t __fastcall Company_CanIssueBond(uint8_t* c);
float __fastcall Company_CashRate(uint8_t* c);
void __fastcall Company_GoBankrupt(uint8_t* c);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
void __fastcall World_SetDifficulty(uint8_t* w, int, int32_t level);
float __fastcall Company_MaxDividend(uint8_t* c);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
double __fastcall Player_HoldingsCollateral(uint8_t* p, int, int32_t stored);
void __fastcall Companies_AdjustCash(int32_t amount);
float __fastcall Company_CashInterestRate(uint8_t* c);
double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
void __fastcall Player_RecordWealth(uint8_t* p);
void __fastcall Dialog_SetupWorthSlider(int32_t select);
}

DIFFTEST(game_CompanyFinance) {
    static uint8_t world[0x5000];
    static Collection companies, players, regions, trains, nodes;
    Collection* cs[5] = { &companies, &players, &regions, &trains, &nodes };
    const uint32_t sizes[5] = { 0xe00, 0x400, 0x500, 0x60, 0x240 };
    for (int k = 0; k < 5; ++k) {
        memset(cs[k], 0, sizeof(Collection));
        Collection_Construct(cs[k], 0, 1, sizes[k], 8, 8, 0, 0, 0);
    }
    static uint8_t blank[0xe00];
    Collection_Allocate(&companies, 0, blank, 0xe00, 0, 0);
    Collection_Allocate(&players, 0, blank, 0x400, 0, 0);
    Collection_Allocate(&regions, 0, blank, 0x500, 0, 0);
    for (int k = 0; k < 6; ++k) Collection_Allocate(&trains, 0, blank, 0x60, 0, 0);
    for (int k = 0; k < 6; ++k) Collection_Allocate(&nodes, 0, blank, 0x240, 0, 0);
    const uint32_t gl[6] = { 0x6cec78, 0x62be10, 0x6ceb9c, 0x6cfc9c, 0x6cfcbc, 0x6cfca8 };
    uint32_t saved[6];
    for (int k = 0; k < 6; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<Collection*>(0x62be10) = &companies;
    hook::Global<Collection*>(0x6ceb9c) = &players;
    hook::Global<Collection*>(0x6cfc9c) = &regions;
    hook::Global<Collection*>(0x6cfcbc) = &trains;
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    static uint8_t comp[0x2000], cc[2][0x2000];
    auto rndVal = [](int scale) -> double {
        switch (Rnd() % 8) {
        case 0: return 0.0;
        case 1: return (double)((int32_t)(Rnd() % 2001) - 1000);
        default: return (double)RndF(-1.0f, 1.0f) * (scale ? 1e6 : 100.0);
        }
    };
    for (int it = 0; it < 20000; ++it) {
        // World: date, difficulty, interest rate, effects.
        *(uint16_t*)(world + 0xd) = (uint16_t)(1850 + Rnd() % 150);
        world[0xf] = (uint8_t)(1 + Rnd() % 12);
        world[0x11] = (uint8_t)(1 + Rnd() % 28);
        if (Rnd() % 10 == 0) world[0xf] = 12, world[0x11] = 28;
        *(int32_t*)(world + 0x15) = (int32_t)(Rnd() % 4000000);
        *(int32_t*)(world + 0x2d) = (int32_t)(Rnd() % 5);
        *(float*)(world + 0x29) = RndF(0.0f, 0.2f);
        *(int32_t*)(world + 0x4a8b) = Rnd() % 4 == 0;
        for (int k = 0; k < 0x40; ++k) *(int32_t*)(world + 0x8a + k * 4) = Rnd() % 3 ? 0 : (int32_t)(Rnd() % 200) - 100;
        uint8_t* bc = (uint8_t*)Collection_Get(&companies, 0, 1);
        for (int k = 0; k < 0x48; ++k) *(int32_t*)(bc + 0x35b + k * 4) = Rnd() % 3 ? 0 : (int32_t)(Rnd() % 100) - 50;
        *(uint32_t*)(bc + 0x3b) = 1;
        uint8_t* bp = (uint8_t*)Collection_Get(&players, 0, 1);
        for (int k = 0; k < 0x48; ++k) *(int32_t*)(bp + 0x2ab + k * 4) = Rnd() % 3 ? 0 : (int32_t)(Rnd() % 100) - 50;
        for (int k = 1; k <= 6; ++k) *(int32_t*)((uint8_t*)Collection_Get(&trains, 0, k) + 0x51) = (int32_t)(Rnd() % 3);
        for (int k = 1; k <= 6; ++k) {
            uint8_t* nd = (uint8_t*)Collection_Get(&nodes, 0, k);
            *(uint16_t*)(nd + 0x22e) = (uint16_t)(Rnd() % 3);
            nd[0x216] = (uint8_t)(1 + Rnd() % 2);
            nd[0x218] = (uint8_t)(3 + Rnd() % 2);
        }
        // The company.
        int32_t year = *(uint16_t*)(world + 0xd);
        for (uint8_t& b : comp) b = 0;
        *(uint32_t*)comp = 1;
        *(int32_t*)(comp + 0x3b) = (int32_t)(Rnd() % 2);
        *(int32_t*)(comp + 0x3f) = Rnd() % 20 != 0;
        *(int32_t*)(comp + 0x47) = Rnd() % 30 ? (int32_t)(1000 + Rnd() % 200000) : 0;
        for (uint32_t o : { 0x4bu, 0x4fu, 0x53u, 0x57u }) *(float*)(comp + o) = (float)fabs(rndVal(o == 0x4f));
        *(uint16_t*)(comp + 0x157) = (uint16_t)(year - (int32_t)(Rnd() % 14) + 1);
        *(int32_t*)(comp + 0x163) = year - (int32_t)(Rnd() % 25);
        int32_t nb = (int32_t)(Rnd() % 4);
        *(int32_t*)(comp + 0x5b) = nb;
        for (int k = 0; k < nb; ++k) {
            *(int32_t*)(comp + 0x5f + k * 0xc) = (int32_t)(Rnd() % 100000);
            *(float*)(comp + 0x5f + k * 0xc + 8) = RndF(0.02f, 0.1f);
        }
        for (uint32_t o = 0x323; o <= 0x343; o += 4) *(float*)(comp + o) = (float)fabs(rndVal(0));
        *(int32_t*)(comp + 0x34b) = (int32_t)(Rnd() % 100);
        *(int32_t*)(comp + 0x14f) = (int32_t)(Rnd() % 100);
        for (uint32_t o : { 0xd0bu, 0xd0fu, 0xd13u }) *(int32_t*)(comp + o) = (int32_t)(Rnd() % 1000);
        *(int32_t*)(comp + 0xd07) = *(int32_t*)(world + 0x15) - (int32_t)(Rnd() % 3000000);
        *(int32_t*)(comp + 0xcfb) = Rnd() % 6 ? 0 : *(int32_t*)(world + 0x15);
        *(double*)(comp + 0xcff) = rndVal(0);
        for (int k = 0; k < 0x2b * 11; ++k) *(double*)(comp + 0xd7f + k * 8) = rndVal(1);
        for (int k = 0; k < 0x2b; ++k) *(double*)(comp + 0x1c47 + k * 8) = rndVal(1);
        int op = (int)(Rnd() % 27);
        float bondRate = Rnd() % 2 ? 0.0f : RndF(0.01f, 0.1f);
        static uint8_t player[0x300];
        memset(player, 0, sizeof player);
        *(int32_t*)(player + 0x15d + 4) = (int32_t)(Rnd() % 20000) - 10000;
        *(double*)(player + 0x154) = (double)RndF(-1.0f, 1.0f) * 1e6;
        int32_t id = (int32_t)(Rnd() % 0x3a);
        int32_t yr;
        switch (Rnd() % 4) {
        case 0: yr = 0x2329; break;
        case 1: yr = 0x232a; break;
        default: yr = year - (int32_t)(Rnd() % 14) + 1;
        }
        int32_t arg = Rnd() % 3 ? (int32_t)(Rnd() % 200000) - 100000 : 0;
        int32_t store = (int32_t)(Rnd() % 2);
        double r[2];
        static uint8_t worldSave[sizeof world];
        memcpy(worldSave, world, sizeof world);
        uint8_t* rec = (uint8_t*)Collection_Get(&companies, 0, 1);
        int32_t level = (int32_t)(Rnd() % 8) - 2;
        for (int side = 0; side < 2; ++side) {
            memcpy(world, worldSave, sizeof world);
            memcpy(cc[side], comp, sizeof comp);
            uint8_t* c = cc[side];
            if (op == 15) {
                // Reprices the companies of the collection: give record 1 this company's data.
                memcpy(rec, comp, 0xe00);
                *(int32_t*)(rec + 0x3f) = 1;
                side ? rt3::World_SetDifficulty(world, 0, level) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4339b0)(world, 0, level);
                memcpy(cc[side], rec, 0xe00);
                memcpy(cc[side] + 0xe00, world, 0x40);
                r[side] = 0;
                continue;
            }
            if (op == 19) {
                memcpy(rec, comp, 0xe00);
                *(uint32_t*)(rec + 0x3b) = store;
                *(int32_t*)((uint8_t*)Collection_Get(&players, 0, 1) + 0x1e1) = id % 3 == 0;
                int32_t amt = arg & 1 ? 0 : arg;
                side ? rt3::Companies_AdjustCash(amt) : ((void(__fastcall*)(int32_t))0x464310)(amt);
                memcpy(cc[side], rec, 0xe00);
                r[side] = 0;
                continue;
            }
            if (op == 25) {
                memcpy(rec, comp, 0xe00);
                *(int32_t*)(rec + 0x3f) = 1;
                static uint8_t pl[2][0x300];
                memcpy(pl[side], player, sizeof player);
                for (int k = 0; k < 18; ++k) *(double*)(pl[side] + 0x1e9 + k * 8) = k * 1.5;
                side ? rt3::Player_RecordWealth(pl[side]) : ((void(__fastcall*)(uint8_t*))0x476810)(pl[side]);
                memcpy(cc[side], pl[side], sizeof pl[side]);
                memcpy(cc[side] + 0x300, rec, 0xe00);
                r[side] = 0;
                continue;
            }
            if (op == 26) {
                // Amount slider sized from player 1's collateral worth.
                memcpy(rec, comp, 0xe00);
                *(int32_t*)(rec + 0x3f) = 1;
                uint8_t* p1 = (uint8_t*)Collection_Get(&players, 0, 1);
                memcpy(p1, player, sizeof player);
                *(double*)(p1 + 0x154) = *(double*)(player + 0x154) * (double)(1 << (2 * (id % 8))) * 1.7;
                *(int32_t*)(world + 0x25) = 1;
                static void* sliderVtbl[4] = { nullptr, (void*)&FakeLoggingBase };
                static uint8_t sliderWin[0x80];
                *(void***)sliderWin = sliderVtbl;
                uint32_t sv[4] = { hook::Global<uint32_t>(0x6cec18), hook::Global<uint32_t>(0x6cebd4), hook::Global<uint32_t>(0x6cec0c), hook::Global<uint32_t>(0x5f2c4c) };
                hook::Global<uint8_t*>(0x6cec18) = sliderWin;
                g_sent.clear();
                side ? rt3::Dialog_SetupWorthSlider(store) : ((void(__fastcall*)(int32_t))0x47c070)(store);
                memset(cc[side], 0, 0x40);
                *(uint32_t*)cc[side] = hook::Global<uint32_t>(0x6cebd4);
                *(uint32_t*)(cc[side] + 4) = hook::Global<uint32_t>(0x6cec0c);
                *(uint32_t*)(cc[side] + 8) = hook::Global<uint32_t>(0x5f2c4c);
                *(uint32_t*)(cc[side] + 12) = (uint32_t)g_sent.size();
                for (size_t k = 0; k < g_sent.size(); ++k) *(uint32_t*)(cc[side] + 16) = *(uint32_t*)(cc[side] + 16) * 31 + g_sent[k];
                hook::Global<uint32_t>(0x6cec18) = sv[0];
                hook::Global<uint32_t>(0x6cebd4) = sv[1];
                hook::Global<uint32_t>(0x6cec0c) = sv[2];
                hook::Global<uint32_t>(0x5f2c4c) = sv[3];
                r[side] = 0;
                continue;
            }
            if (op >= 21 && op <= 23) {
                memcpy(rec, comp, 0xe00);
                *(int32_t*)(rec + 0x3f) = 1;
                if (op == 21) r[side] = side ? rt3::Player_NetWorth(player, 0, store) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x476780)(player, 0, store);
                else if (op == 22) r[side] = side ? rt3::Player_CollateralWorth(player, 0, store) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x476c20)(player, 0, store);
                else r[side] = side ? rt3::Player_Wealth(player, 0, id % 5) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x4767a0)(player, 0, id % 5);
                memcpy(cc[side], rec, 0xe00);
                continue;
            }
            if (op == 17 || op == 18) {
                // Holdings of company 1 (the collection's record), priced from this company's data.
                memcpy(rec, comp, 0xe00);
                *(int32_t*)(rec + 0x3f) = 1;
                if (op == 17) r[side] = side ? rt3::Player_HoldingsValue(player, 0, store) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x476320)(player, 0, store);
                else r[side] = side ? rt3::Player_HoldingsCollateral(player, 0, store) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x4763b0)(player, 0, store);
                memcpy(cc[side], rec, 0xe00);
                continue;
            }
            switch (op) {
            case 0: r[side] = side ? rt3::Company_Stat(c, 0, id, yr) : ((double(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x42a5d0)(c, 0, id, yr); break;
            case 1: r[side] = side ? rt3::Company_LiveStat(c, 0, id % 0x2b) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x42a2e0)(c, 0, id % 0x2b); break;
            case 2: r[side] = side ? rt3::Company_DerivedStat(c, 0, id, yr) : ((double(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x429c70)(c, 0, id, yr); break;
            case 3: r[side] = side ? rt3::Company_YearEstimate(c, 0, id) : ((double(__fastcall*)(uint8_t*, int, int32_t))0x42a130)(c, 0, id); break;
            case 4: r[side] = side ? rt3::Company_BookValue(c) : ((float(__fastcall*)(uint8_t*))0x4248d0)(c); break;
            case 5: r[side] = side ? rt3::Company_GrowthScore(c) : ((float(__fastcall*)(uint8_t*))0x4246b0)(c); break;
            case 6: r[side] = side ? rt3::Company_CreditRating(c, 0, arg) : ((float(__fastcall*)(uint8_t*, int, int32_t))0x425320)(c, 0, arg); break;
            case 7: r[side] = side ? rt3::Company_SharePrice(c, 0, arg, store) : ((float(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x424fd0)(c, 0, arg, store); break;
            case 8: r[side] = side ? rt3::Company_BondRate(c) : ((float(__fastcall*)(uint8_t*))0x425880)(c); break;
            case 9: r[side] = side ? rt3::Company_BondRateWithDebt(c, 0, arg, 0) : ((float(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x427540)(c, 0, arg, 0); break;
            case 10: r[side] = side ? rt3::Company_CanIssueBond(c) : ((int32_t(__fastcall*)(uint8_t*))0x427590)(c); break;
            case 11: r[side] = side ? rt3::Company_CashRate(c) : ((float(__fastcall*)(uint8_t*))0x4258c0)(c); break;
            case 12: r[side] = 0; side ? rt3::Company_GoBankrupt(c) : ((void(__fastcall*)(uint8_t*))0x425a90)(c); break;
            case 15: break;
            case 24: r[side] = side ? rt3::Company_IssueBond(c, 0, arg & 0xfffff, id % 30, store, bondRate) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t, float))0x4275c0)(c, 0, arg & 0xfffff, id % 30, store, bondRate); break;
            case 20: r[side] = side ? rt3::Company_CashInterestRate(c) : ((float(__fastcall*)(uint8_t*))0x425a50)(c); break;
            case 16: r[side] = side ? rt3::Company_MaxDividend(c) : ((float(__fastcall*)(uint8_t*))0x426260)(c); break;
            case 13: r[side] = 0; side ? rt3::Company_BuyBackShares(c, 0, arg & 0xffff) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4273c0)(c, 0, arg & 0xffff); break;
            default: r[side] = 0; side ? rt3::Company_IssueShares(c, 0, arg & 0xffff, store, id * 37) : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x427450)(c, 0, arg & 0xffff, store, id * 37); break;
            }
        }
        EXPECT_EQ(memcmp(&r[0], &r[1], 8), 0, "op %d id %x year %d arg %d iteration %d: %.17g vs %.17g", op, id, yr, arg, it, r[0], r[1]);
        int diff = FirstDiff(cc[0], cc[1], sizeof comp);
        EXPECT_EQ(diff, -1, "company op %d id %x iteration %d", op, id, it);
    }
    for (int k = 0; k < 6; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
}

namespace rt3 {
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Window_DeleteChild(uint8_t* win, int, uint8_t* w);
}

namespace {
uint32_t g_delLog[64];
int g_delN;
uint8_t g_delWidgets[2][10][0xa0];
void __fastcall DelWidgetDtor(uint8_t* w, int, int32_t flag) {
    if (g_delN < 64) g_delLog[g_delN++] = 0x100 * (uint32_t)flag + (uint32_t)AF<uint16_t>(w, 4);
}
void __fastcall DelWidgetFocus(uint8_t* w) {
    if (g_delN < 64) g_delLog[g_delN++] = 0x10000 + AF<uint16_t>(w, 4);
}
void __fastcall DelWidgetRedraw(uint8_t* w) {
    if (g_delN < 64) g_delLog[g_delN++] = 0x20000 + AF<uint16_t>(w, 4);
}
}

DIFFTEST(game_WindowDeleteChildren) {
    memset(g_delWidgets, 0, sizeof g_delWidgets); // other tests leave side-specific pointers in the pool
    static void* vtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    const int S = 4;
    for (int it = 0; it < 5000; ++it) {
        int n = (int)(Rnd() % 10);
        uint16_t ids[10];
        uint32_t parents[10];
        uint8_t flags[10], fonts[10], bfonts[10];
        for (int k = 0; k < n; ++k) {
            ids[k] = (uint16_t)(100 + Rnd() % 8);
            parents[k] = Rnd() % 3 ? 0x5000 : 0x6000;
            flags[k] = (uint8_t)(Rnd() & 0xef); // no hover flag (global 0x6d401c)
            fonts[k] = (uint8_t)(Rnd() % (S + 1));
            bfonts[k] = (uint8_t)(Rnd() % (S + 1));
        }
        uint32_t refs[S];
        for (uint32_t& r : refs) r = 1 + Rnd() % 4;
        int32_t lo = 100 + (int32_t)(Rnd() % 8), hi = Rnd() % 3 ? 0xfff0 : lo + (int32_t)(Rnd() % 4);
        uint32_t parent = Rnd() % 4 ? 0x5000 : 0x6000;
        bool single = Rnd() % 4 == 0;
        int pick = n ? (int)(Rnd() % n) : 0;
        uint8_t wins[2][0x80];
        uint32_t logs[2][65];
        uint8_t tabs[2][S * 0xd];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(S * 0xd);
            memset(table, 0, S * 0xd);
            for (int k = 0; k < S; ++k) {
                *(uint32_t*)(table + k * 0xd) = refs[k];
                *(int32_t*)(table + k * 0xd + 4) = 0x70 + k;
                table[k * 0xd + 8] = 5;
                *(uint32_t*)(table + k * 0xd + 9) = 0x7100 + k;
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = S;
            uint8_t* win = wins[side];
            memset(win, 0, sizeof wins[side]);
            for (int k = 0; k < n; ++k) {
                uint8_t* w = g_delWidgets[side][k];
                memset(w, 0, 0xa0);
                *(void***)w = vtbl;
                *(uint16_t*)(w + 4) = ids[k];
                *(uint32_t*)(w + 0x4b) = parents[k];
                w[0x6a] = flags[k];
                *(uint32_t*)(w + 0x63) = fonts[k] < S ? 0x7100 + fonts[k] : 0;
                *(uint32_t*)(w + 0x92) = bfonts[k] < S ? 0x7100 + bfonts[k] : 0;
                *(uint8_t**)(w + 0x8a) = k + 1 < n ? g_delWidgets[side][k + 1] : nullptr;
                *(uint8_t**)(w + 0x8e) = k ? g_delWidgets[side][k - 1] : nullptr;
            }
            *(uint8_t**)(win + 0x74) = n ? g_delWidgets[side][0] : nullptr;
            *(uint8_t**)(win + 0x70) = n ? g_delWidgets[side][n - 1] : nullptr;
            g_delN = 0;
            memset(g_delLog, 0, sizeof g_delLog);
            if (single) {
                uint8_t* w = n ? g_delWidgets[side][pick] : nullptr;
                side ? rt3::Window_DeleteChild(win, 0, w) : ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x53f860)(win, 0, w);
            } else {
                side ? rt3::Window_DeleteChildren(win, 0, lo, hi, parent)
                     : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, uint32_t))0x53fe00)(win, 0, lo, hi, parent);
            }
            memcpy(logs[side], g_delLog, sizeof g_delLog);
            logs[side][64] = (uint32_t)g_delN;
            memcpy(tabs[side], table, sizeof tabs[side]);
            xfree(table);
            // Links as widget indexes.
            for (int k = 0; k < 0x80; k += 4) {
                uint8_t* p = *(uint8_t**)(win + k);
                for (int j = 0; j < n; ++j)
                    if (p == g_delWidgets[side][j]) *(uint32_t*)(win + k) = 0xa000 + j;
            }
            for (int k = 0; k < n; ++k)
                for (uint32_t o : { 0x8au, 0x8eu }) {
                    uint8_t* p = *(uint8_t**)(g_delWidgets[side][k] + o);
                    for (int j = 0; j < n; ++j)
                        if (p == g_delWidgets[side][j]) *(uint32_t*)(g_delWidgets[side][k] + o) = 0xa000 + j;
                }
        }
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "calls iteration %d", it);
        EXPECT_EQ(memcmp(wins[0], wins[1], sizeof wins[0]), 0, "window iteration %d", it);
        EXPECT_EQ(memcmp(g_delWidgets[0], g_delWidgets[1], sizeof g_delWidgets[0]), 0, "widgets iteration %d", it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "pack iteration %d", it);
    }
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
}

DIFFTEST(game_WindowsRemoveAndStatus) {
    static void* vtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, prot, &prot);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    int32_t sBusy = hook::Global<int32_t>(0xccba18);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static uint8_t wins[2][4][0x80];
    static const char* texts[4] = { "", "x", "Saved", "Track built: 12 miles" };
    for (int it = 0; it < 6000; ++it) {
        bool status = Rnd() % 2 == 0;
        // Window list (4 windows; each with up to 3 children, parents 0 or not).
        int nw = 1 + (int)(Rnd() % 4), pick = (int)(Rnd() % (nw + 1));
        int nc = (int)(Rnd() % 4);
        uint32_t cpar[3];
        for (uint32_t& p : cpar) p = Rnd() % 3 ? 0 : 0x5000;
        // Status line.
        int ti = (int)(Rnd() % 4), oldText = (int)(Rnd() % 3);
        uint32_t font = Rnd() % 2 ? 0x7107 : 0;
        int32_t busy = Rnd() % 6 == 0, ms = Rnd() % 3 ? (int32_t)(Rnd() % 5000) : 0;
        g_fakeTicks = Rnd();
        uint32_t ref = 1 + Rnd() % 3;
        uint8_t mgrs[2][8], sts[2][0x20];
        char stText[2][32];
        uint32_t logs[2][65], tabs[2][2], busyOut[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(0xd);
            memset(table, 0, 0xd);
            *(uint32_t*)table = ref;
            *(int32_t*)(table + 4) = 0x754d4c7;
            table[8] = 5;
            *(uint32_t*)(table + 9) = 0x7107;
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 1;
            g_delN = 0;
            memset(g_delLog, 0, sizeof g_delLog);
            uint8_t* mgr = mgrs[side];
            uint8_t* st = sts[side];
            if (!status) {
                for (int k = 0; k < nw; ++k) {
                    uint8_t* w = wins[side][k];
                    memset(w, 0, 0x80);
                    *(uint8_t**)(w + 0x58) = k + 1 < nw ? wins[side][k + 1] : nullptr;
                    *(uint8_t**)(w + 0x54) = k ? wins[side][k - 1] : nullptr;
                }
                // Children only on the picked window (static widget pool).
                if (pick < nw) {
                    uint8_t* w = wins[side][pick];
                    for (int j = 0; j < nc; ++j) {
                        uint8_t* c = g_delWidgets[side][j];
                        memset(c, 0, 0xa0);
                        *(void***)c = vtbl;
                        *(uint16_t*)(c + 4) = (uint16_t)(10 + j);
                        *(uint32_t*)(c + 0x4b) = cpar[j];
                        *(uint8_t**)(c + 0x8a) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                        *(uint8_t**)(c + 0x8e) = j ? g_delWidgets[side][j - 1] : nullptr;
                    }
                    *(uint8_t**)(w + 0x74) = nc ? g_delWidgets[side][0] : nullptr;
                    *(uint8_t**)(w + 0x70) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                }
                *(uint8_t**)mgr = wins[side][0];
                *(uint8_t**)(mgr + 4) = wins[side][nw - 1];
                uint8_t* w = pick < nw ? wins[side][pick] : nullptr;
                side ? rt3::Windows_Remove(mgr, 0, w) : ((void(__fastcall*)(uint8_t*, int, uint8_t*))0x5389c0)(mgr, 0, w);
                // Pointers as indexes.
                for (int k = 0; k < 2; ++k)
                    for (int j = 0; j < nw; ++j)
                        if (((uint8_t**)mgr)[k] == wins[side][j]) ((uint32_t*)mgr)[k] = 0xa000 + j;
                for (int k = 0; k < nw; ++k) {
                    uint8_t* w2 = wins[side][k];
                    for (uint32_t o : { 0x54u, 0x58u })
                        for (int j = 0; j < nw; ++j)
                            if (*(uint8_t**)(w2 + o) == wins[side][j]) *(uint32_t*)(w2 + o) = 0xa000 + j;
                    for (uint32_t o : { 0x70u, 0x74u })
                        for (int j = 0; j < nc; ++j)
                            if (*(uint8_t**)(w2 + o) == g_delWidgets[side][j]) *(uint32_t*)(w2 + o) = 0xb000 + j;
                }
            } else {
                memset(st, 0, sizeof sts[side]);
                if (oldText) {
                    char* s = (char*)xmalloc(8);
                    strcpy(s, "old");
                    *(char**)(st + 0x14) = s;
                }
                *(uint32_t*)(st + 0x18) = font;
                hook::Global<int32_t>(0xccba18) = busy;
                side ? rt3::StatusLine_Set(st, 0, texts[ti], ms) : ((void(__fastcall*)(uint8_t*, int, const char*, int32_t))0x5386e0)(st, 0, texts[ti], ms);
                memset(stText[side], 0, 32);
                if (char* s = *(char**)(st + 0x14)) {
                    strncpy(stText[side], s, 31);
                    xfree(s);
                    *(uint32_t*)(st + 0x14) = 1;
                }
                busyOut[side] = (uint32_t)hook::Global<int32_t>(0xccba18);
            }
            memcpy(logs[side], g_delLog, sizeof g_delLog);
            logs[side][64] = (uint32_t)g_delN;
            tabs[side][0] = *(uint32_t*)table;
            tabs[side][1] = *(uint32_t*)(table + 9);
            xfree(table);
        }
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "calls iteration %d", it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "pack iteration %d", it);
        if (status) {
            EXPECT_EQ(memcmp(sts[0], sts[1], sizeof sts[0]), 0, "status iteration %d", it);
            EXPECT_EQ(memcmp(stText[0], stText[1], 32), 0, "status text iteration %d", it);
            EXPECT_EQ(busyOut[0], busyOut[1], "busy iteration %d", it);
        } else {
            EXPECT_EQ(memcmp(mgrs[0], mgrs[1], 8), 0, "manager iteration %d", it);
            EXPECT_EQ(memcmp(wins[0], wins[1], sizeof wins[0]), 0, "windows iteration %d", it);
        }
    }
    hook::Global<int32_t>(0xccba18) = sBusy;
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Window_AddTrainDetailStrip(uint8_t* win, int, float x, float y, int32_t, int32_t);
void __fastcall MapView_LoadTrainFont(uint8_t* v);
}

namespace {
int g_ctlCalls;
void __fastcall CountControlCall(uint8_t*) { ++g_ctlCalls; }
}

DIFFTEST(game_TrainDetailStrip) {
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xdel = (void(__cdecl*)(void*))0x53b080;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_tds.bin");
    { FILE* w = fopen(path, "wb"); fwrite(path, 1, 32, w); fclose(w); }
    // The control's attach hooks (vtable 0x5dd870 slots 5 and 15) are stubbed for both sides.
    void** vt = (void**)0x5dd870;
    DWORD prot;
    VirtualProtect(vt, 0x40, PAGE_READWRITE, &prot);
    void* sv5 = vt[5];
    void* sv15 = vt[15];
    vt[5] = (void*)&CountControlCall;
    vt[15] = (void*)&CountControlCall;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    int32_t key = rt3::NameTable_Hash(pack, 0, "TrainDetail.imb");
    static uint8_t win[2][0x80], view[2][0x100];
    for (int it = 0; it < 2000; ++it) {
        bool strip = Rnd() % 2 == 0;
        float x = RndF(-1.0f, 2.0f), y = RndF(-1.0f, 2.0f);
        uint32_t flags = Rnd() & 0x3030003;
        uint32_t refs[2] = { 1 + Rnd() % 3, 1 + Rnd() % 3 };
        uint8_t child[2][0xe9];
        char names[2][2][32];
        uint32_t tabs[2][2];
        int calls[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(2 * 0xd);
            memset(table, 0, 2 * 0xd);
            *(uint32_t*)table = refs[0];
            *(int32_t*)(table + 4) = key;
            table[8] = 6;
            *(uint32_t*)(table + 9) = 0x6100;
            *(uint32_t*)(table + 0xd) = refs[1];
            *(int32_t*)(table + 0xd + 4) = 0x754d4c3;
            table[0xd + 8] = 7;
            *(uint32_t*)(table + 0xd + 9) = 0x7103;
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 2;
            *(int32_t*)(pack + 0x427bc) = 0;
            *(uint8_t**)(pack + 0x20428) = nullptr;
            FILE* f = xfopen(path, "rb");
            *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)f + 0x10);
            g_ctlCalls = 0;
            memset(child[side], 0, sizeof child[side]);
            memset(names[side], 0, sizeof names[side]);
            if (strip) {
                uint8_t* w = win[side];
                memset(w, 0, sizeof win[side]);
                side ? rt3::Window_AddTrainDetailStrip(w, 0, x, y, 0, 0)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x433f20)(w, 0, x, y, 0, 0);
                uint8_t* c = *(uint8_t**)(w + 0x70);
                EXPECT_EQ(c != nullptr && c == *(uint8_t**)(w + 0x74), true, "child iteration %d", it);
                if (c) {
                    memcpy(child[side], c, sizeof child[side]);
                    for (int k = 0; k < 2; ++k) {
                        char* s = *(char**)(c + (k ? 0x47 : 0x43));
                        if (s) {
                            strncpy(names[side][k], s, 31);
                            xfree(s);
                        }
                        *(uint32_t*)(child[side] + (k ? 0x47 : 0x43)) = 0;
                    }
                    if (char* s = *(char**)(c + 0xce)) xfree(s);
                    *(uint32_t*)(child[side] + 0x86) = *(uint8_t**)(c + 0x86) == w;
                    *(uint32_t*)(child[side] + 0x82) = 0; // never initialized (allocator garbage)
                    xdel(c);
                }
                *(uint32_t*)(w + 0x70) = *(uint32_t*)(w + 0x74) = 0;
            } else {
                uint8_t* v = view[side];
                memset(v, 0, sizeof view[side]);
                *(uint32_t*)(v + 0x33) = flags;
                side ? rt3::MapView_LoadTrainFont(v) : ((void(__fastcall*)(uint8_t*))0x477980)(v);
                memcpy(child[side], v, sizeof child[side]);
            }
            xfclose(f);
            calls[side] = g_ctlCalls;
            tabs[side][0] = *(uint32_t*)table;
            tabs[side][1] = *(uint32_t*)(table + 0xd);
            xfree(table);
            EXPECT_EQ(*(uint8_t**)(pack + 0x20428) == nullptr, true, "position stack iteration %d", it);
        }
        EXPECT_EQ(memcmp(child[0], child[1], sizeof child[0]), 0, "%s iteration %d at %#x", strip ? "strip" : "view", it, FirstDiff(child[0], child[1], sizeof child[0]));
        EXPECT_EQ(memcmp(names[0], names[1], sizeof names[0]), 0, "names iteration %d", it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "pack iteration %d", it);
        EXPECT_EQ(calls[0], calls[1], "attach calls iteration %d", it);
    }
    vt[5] = sv5;
    vt[15] = sv15;
    VirtualProtect(vt, 0x40, prot, &prot);
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
void __fastcall Widget_DeleteGeneratedRows(uint8_t* w, int, int32_t);
void __fastcall MapTool_SetMode(uint8_t* t, int, int32_t mode);
void ViewModesPanel_Close();
void Panel6d081c_Close();
void Panel6d0820_Close();
uint8_t* __fastcall PortraitDialog_Construct(uint8_t* d);
void __fastcall EditTool_UpdateCursor(uint8_t* t);
void PendingRows_Clear();
}

DIFFTEST(game_PanelsAndTools) {
    static void* vtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, prot, &prot);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x6d401c, 0xccbb20, 0x6d07b4, 0x6d081c, 0x6d0820, 0x6cff84, 0x6cff88, 0x6cff8c, 0x6cff90, 0xccba1c, 0x6d16f0, 0x6d16d8 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    uint32_t savedPortraits[0x29];
    memcpy(savedPortraits, (void*)0x6cfed0, sizeof savedPortraits);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    // Portrait slots by name hash, fonts 1..3.
    const int N = 0x29 + 3;
    static uint8_t tmpl[N * 0xd];
    memset(tmpl, 0, sizeof tmpl);
    for (int k = 0; k < 0x29; ++k) {
        char name[0x40];
        sprintf(name, "portraitL%02d01.imb", k);
        uint8_t* e = tmpl + k * 0xd;
        *(int32_t*)(e + 4) = rt3::NameTable_Hash(pack, 0, name);
        e[8] = 6;
        *(uint32_t*)(e + 9) = 0x6100 + k;
    }
    for (int k = 0; k < 3; ++k) {
        uint8_t* e = tmpl + (0x29 + k) * 0xd;
        *(int32_t*)(e + 4) = 0x754d4c0 + 1 + k;
        e[8] = 7;
        *(uint32_t*)(e + 9) = 0x7101 + k;
    }
    static uint8_t mgr[2][0x40], cursor[2][0x30], tool[2][0x90], win[2][0x80], dlg[2][0xa0], w[2][0xa0];
    for (int it = 0; it < 3000; ++it) {
        int op = (int)(Rnd() % 8);
        int32_t toolMode = (int32_t)(Rnd() % 30);
        uint32_t pending = Rnd() % 3 != 0, haveWin = Rnd() % 4 != 0;
        uint32_t refs[N];
        for (uint32_t& r : refs) r = Rnd() % 3;
        int32_t mode = (int32_t)(Rnd() % 6) - 1, cur = (int32_t)(Rnd() % 3) ? (int32_t)(Rnd() % 0x17) : 7;
        int nc = (int)(Rnd() % 5);
        uint16_t ids[5];
        for (uint16_t& id : ids) {
            const uint16_t pool[6] = { 0x2b0b, 0x2b0c, 0x2b34, 0x2b35, 0x65f4, 0x66bb };
            id = pool[Rnd() % 6];
        }
        uint16_t wid = Rnd() % 2 ? 0x2aff : 0x1234;
        int neighbours = (int)(Rnd() % 3);
        uint32_t busy = Rnd() % 4 == 0;
        uint32_t tick = Rnd();
        bool havePanel = Rnd() % 5 != 0;
        uint32_t logs[2][65], tabs[2][N], outG[2][G], portraits[2][0x29];
        char status[2][8];
        for (int side = 0; side < 2; ++side) {
            g_fakeTicks = tick;
            uint8_t* table = (uint8_t*)xmalloc(sizeof tmpl);
            memcpy(table, tmpl, sizeof tmpl);
            for (int k = 0; k < N; ++k) *(uint32_t*)(table + k * 0xd) = refs[k];
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = N;
            g_delN = 0;
            memset(g_delLog, 0, sizeof g_delLog);
            memset(mgr[side], 0, sizeof mgr[side]);
            hook::Global<uint8_t*>(0x6d401c) = mgr[side];
            hook::Global<uint32_t>(0xccba1c) = busy;
            memset(status[side], 0, 8);
            switch (op) {
            case 0: { // generated rows
                uint8_t* wn = win[side];
                memset(wn, 0, sizeof win[side]);
                for (int j = 0; j < nc; ++j) {
                    uint8_t* c = g_delWidgets[side][j];
                    memset(c, 0, 0xa0);
                    *(void***)c = vtbl;
                    *(uint16_t*)(c + 4) = ids[j];
                    *(uint8_t**)(c + 0x8e) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                    *(uint8_t**)(c + 0x8a) = j ? g_delWidgets[side][j - 1] : nullptr;
                }
                *(uint8_t**)(wn + 0x70) = nc ? g_delWidgets[side][0] : nullptr;
                *(uint8_t**)(wn + 0x74) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                memset(w[side], 0, sizeof w[side]);
                *(uint16_t*)(w[side] + 4) = wid;
                *(uint8_t**)(w[side] + 0x86) = wn;
                side ? rt3::Widget_DeleteGeneratedRows(w[side], 0, 0) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4bd460)(w[side], 0, 0);
                *(uint32_t*)(w[side] + 0x86) = 0;
                for (uint32_t o : { 0x70u, 0x74u })
                    for (int j = 0; j < nc; ++j)
                        if (*(uint8_t**)(wn + o) == g_delWidgets[side][j]) *(uint32_t*)(wn + o) = 0xb000 + j;
                break;
            }
            case 1: { // tool mode (busy cursor: the change is only queued)
                memset(cursor[side], 0, sizeof cursor[side]);
                *(int32_t*)(cursor[side] + 4) = cur;
                *(int32_t*)(cursor[side] + 0xc) = 1;
                hook::Global<uint8_t*>(0xccbb20) = cursor[side];
                memset(tool[side], 0, sizeof tool[side]);
                side ? rt3::MapTool_SetMode(tool[side], 0, mode) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x4bcc60)(tool[side], 0, mode);
                break;
            }
            case 2:
            case 3:
            case 4: { // panel closers
                const uint32_t slot = op == 2 ? 0x6d07b4 : op == 3 ? 0x6d081c : 0x6d0820;
                uint8_t* p = havePanel ? (uint8_t*)xnew(0x80) : nullptr;
                if (p) memset(p, 0, 0x80);
                // Window list: [before] p [after], links next +0x58 / previous +0x54.
                static uint8_t nb[2][2][0x80];
                memset(nb[side], 0, sizeof nb[side]);
                uint8_t* first = p;
                if (p && neighbours & 1) {
                    *(uint8_t**)(nb[side][0] + 0x58) = p;
                    *(uint8_t**)(p + 0x54) = nb[side][0];
                    first = nb[side][0];
                }
                uint8_t* last = p;
                if (p && neighbours & 2) {
                    *(uint8_t**)(nb[side][1] + 0x54) = p;
                    *(uint8_t**)(p + 0x58) = nb[side][1];
                    last = nb[side][1];
                }
                *(uint8_t**)mgr[side] = first;
                *(uint8_t**)(mgr[side] + 4) = last;
                hook::Global<uint8_t*>(slot) = p;
                if (side) op == 2 ? rt3::ViewModesPanel_Close() : op == 3 ? rt3::Panel6d081c_Close() : rt3::Panel6d0820_Close();
                else ((void(__cdecl*)())(op == 2 ? 0x4d44a0 : op == 3 ? 0x4e06d0 : 0x4e0df0))();
                for (int k = 0; k < 2; ++k) {
                    uint8_t* q = ((uint8_t**)mgr[side])[k];
                    ((uint32_t*)mgr[side])[k] = q == nb[side][0] ? 1 : q == nb[side][1] ? 2 : q ? 3 : 0;
                    uint8_t* l = *(uint8_t**)(nb[side][k] + (k ? 0x54 : 0x58));
                    *(uint32_t*)(mgr[side] + 8 + k * 4) = l == nb[side][0] ? 1 : l == nb[side][1] ? 2 : l ? 3 : 0;
                }
                break;
            }
            case 6: { // edit tool cursor (busy cursor: queued)
                memset(cursor[side], 0, sizeof cursor[side]);
                *(int32_t*)(cursor[side] + 4) = cur;
                *(int32_t*)(cursor[side] + 0xc) = 1;
                hook::Global<uint8_t*>(0xccbb20) = cursor[side];
                memset(tool[side], 0, sizeof tool[side]);
                *(int32_t*)(tool[side] + 0x8c) = toolMode;
                side ? rt3::EditTool_UpdateCursor(tool[side]) : ((void(__fastcall*)(uint8_t*))0x4dd630)(tool[side]);
                break;
            }
            case 7: { // pending rows of window 0x6d16d8
                const uint16_t pool[8] = { 0xb3f5, 0xb3f6, 0xb3f7, 0xb3f8, 0xb400, 0xb409, 0xb40a, 0xb5a3 };
                uint8_t* wn = win[side];
                memset(wn, 0, sizeof win[side]);
                for (int j = 0; j < nc; ++j) {
                    uint8_t* c = g_delWidgets[side][j];
                    memset(c, 0, 0xa0);
                    *(void***)c = vtbl;
                    *(uint16_t*)(c + 4) = pool[ids[j] % 8];
                    *(uint8_t**)(c + 0x8e) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                    *(uint8_t**)(c + 0x8a) = j ? g_delWidgets[side][j - 1] : nullptr;
                }
                *(uint8_t**)(wn + 0x70) = nc ? g_delWidgets[side][0] : nullptr;
                *(uint8_t**)(wn + 0x74) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                hook::Global<uint32_t>(0x6d16f0) = pending;
                hook::Global<uint8_t*>(0x6d16d8) = haveWin ? wn : nullptr;
                side ? rt3::PendingRows_Clear() : ((void(__cdecl*)())0x5042c0)();
                hook::Global<uint32_t>(0x6d16d8) = 0;
                for (uint32_t o : { 0x70u, 0x74u })
                    for (int j = 0; j < nc; ++j)
                        if (*(uint8_t**)(wn + o) == g_delWidgets[side][j]) *(uint32_t*)(wn + o) = 0xb000 + j;
                break;
            }
            default: { // portrait dialog
                memset(dlg[side], 0, sizeof dlg[side]);
                uint8_t* r = side ? rt3::PortraitDialog_Construct(dlg[side]) : ((uint8_t*(__fastcall*)(uint8_t*))0x4c9c10)(dlg[side]);
                EXPECT_EQ(r == dlg[side] && hook::Global<uint8_t*>(0x6cff84) == dlg[side], true, "dialog iteration %d", it);
                if (char* s = *(char**)(mgr[side] + 0x10)) {
                    strncpy(status[side], s, 7);
                    xfree(s);
                    *(char**)(mgr[side] + 0x10) = (char*)1;
                }
                memcpy(portraits[side], (void*)0x6cfed0, sizeof portraits[side]);
                hook::Global<uint32_t>(0x6cff84) = 0;
                break;
            }
            }
            for (int k = 0; k < G; ++k) outG[side][k] = gl[k] == 0x6d4020 || gl[k] == 0x6d401c || gl[k] == 0xccbb20 ? 0 : hook::Global<uint32_t>(gl[k]);
            memcpy(logs[side], g_delLog, sizeof g_delLog);
            logs[side][64] = (uint32_t)g_delN;
            for (int k = 0; k < N; ++k) tabs[side][k] = *(uint32_t*)(table + k * 0xd);
            xfree(table);
        }
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "op %d calls iteration %d", op, it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "op %d pack iteration %d", op, it);
        EXPECT_EQ(memcmp(outG[0], outG[1], sizeof outG[0]), 0, "op %d globals iteration %d", op, it);
        EXPECT_EQ(memcmp(mgr[0], mgr[1], sizeof mgr[0]), 0, "op %d manager iteration %d", op, it);
        if (op == 7) EXPECT_EQ(memcmp(win[0], win[1], sizeof win[0]), 0, "pending rows window iteration %d", it);
        if (op == 6) EXPECT_EQ(memcmp(cursor[0], cursor[1], sizeof cursor[0]), 0, "edit cursor iteration %d", it);
        if (op == 0) {
            EXPECT_EQ(memcmp(win[0], win[1], sizeof win[0]), 0, "rows window iteration %d", it);
            EXPECT_EQ(memcmp(w[0], w[1], sizeof w[0]), 0, "rows widget iteration %d", it);
        }
        if (op == 1) {
            EXPECT_EQ(memcmp(tool[0], tool[1], sizeof tool[0]), 0, "tool iteration %d", it);
            EXPECT_EQ(memcmp(cursor[0], cursor[1], sizeof cursor[0]), 0, "cursor iteration %d", it);
        }
        if (op >= 5 && op != 6 && op != 7) {
            EXPECT_EQ(memcmp(dlg[0], dlg[1], sizeof dlg[0]), 0, "dialog iteration %d", it);
            EXPECT_EQ(memcmp(portraits[0], portraits[1], sizeof portraits[0]), 0, "portraits iteration %d", it);
            EXPECT_EQ(memcmp(status[0], status[1], 8), 0, "status iteration %d", it);
        }
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    memcpy((void*)0x6cfed0, savedPortraits, sizeof savedPortraits);
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Dialog_AddFilterToggle(uint8_t* d, int, int32_t idx, float x, float y, int32_t, float h);
void __fastcall MapWidget_DrawCityMarker(uint8_t* w);
}
namespace rt3 {
void __fastcall SetupToggle_AddWorldByte(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall SetupToggle_AddWorldFlag(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall SetupToggle_AddNamed2(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall SetupToggle_AddNamed1(uint8_t* d, int, int32_t idx, float x, float y, float w, float h);
void __fastcall EditButton_SetCursor(int32_t id, int, int32_t active, int32_t, int32_t, int32_t, int32_t);
void __fastcall Tutorial_ShowPicture(int32_t page, int, float x, float y);
}

DIFFTEST(game_SetupToggles) {
    typedef void(__fastcall * AddFn)(uint8_t*, int, int32_t, float, float, float, float);
    const AddFn mine[4] = { rt3::SetupToggle_AddWorldByte, rt3::SetupToggle_AddWorldFlag, rt3::SetupToggle_AddNamed2, rt3::SetupToggle_AddNamed1 };
    const uint32_t orig[4] = { 0x4ca450, 0x4cb1c0, 0x4cd570, 0x4cc130 };
    static void* winVtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xdel = (void(__cdecl*)(void*))0x53b080;
    void** vt = (void**)0x5de3b0; // button attach hooks, stubbed for both sides
    DWORD prot;
    VirtualProtect(vt, 0x40, PAGE_READWRITE, &prot);
    void* sv5 = vt[5];
    void* sv15 = vt[15];
    vt[5] = (void*)&CountControlCall;
    vt[15] = (void*)&CountControlCall;
    void** vs = (void**)0x5ddd18; // scrollbar attach hooks
    DWORD prot2;
    VirtualProtect(vs, 0x40, PAGE_READWRITE, &prot2);
    void* ss5 = vs[5];
    void* ss15 = vs[15];
    vs[5] = (void*)&CountControlCall;
    vs[15] = (void*)&CountControlCall;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    static uint8_t world[0x7000], input[0xb00], engine[0x40];
    const uint32_t gl[] = { 0x6d4020, 0x6cec78, 0x62b268, 0x6ada84, 0x62ba88, 0xccbb20, 0x622078, 0x6d4024, 0x6d4018, 0x6d3b44, 0x622b30 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<uint8_t*>(0x6d4024) = engine;
    hook::Global<uint8_t*>(0x6d4018) = input;
    static const char* names[4] = { "Alpha", "Beta", "gamma", "Gamma" };
    static Collection named1, named2, t268, t84, t88;
    Collection* cs[5] = { &named1, &named2, &t268, &t84, &t88 };
    for (Collection* c : cs) {
        memset(c, 0, sizeof(Collection));
        Collection_Construct(c, 0, 1, 0x80, 8, 8, 0, 0, 0);
        static uint8_t blank[0x80];
        for (int k = 0; k < 4; ++k) Collection_Allocate(c, 0, blank, 0x80, 0, 0);
    }
    *(Collection**)(world + 0x66b2) = &named1;
    *(Collection**)(world + 0x66b6) = &named2;
    hook::Global<Collection*>(0x62b268) = &t268;
    hook::Global<Collection*>(0x6ada84) = &t84;
    hook::Global<Collection*>(0x62ba88) = &t88;
    const int NT = 6;
    uint32_t tmpl[NT][3] = { { 0, (uint32_t)rt3::NameTable_Hash(pack, 0, "Setup_Options_Buttons.imb"), 6 }, { 0, 0x754d4c3, 7 } };
    for (int k = 0; k < 4; ++k) {
        char tn[32];
        sprintf(tn, "tutorial%d.imb", k);
        tmpl[2 + k][1] = (uint32_t)rt3::NameTable_Hash(pack, 0, tn);
        tmpl[2 + k][2] = 6;
    }
    static uint8_t dlg[2][0x100], win[2][0x200], cursor[2][0x30];
    uint8_t savedFilter[8];
    memcpy(savedFilter, (void*)0x6d1aa8, 8);
    for (int it = 0; it < 3000; ++it) {
        int op = (int)(Rnd() % 7);
        for (int k = 0; k < 8; ++k) ((uint8_t*)0x6d1aa8)[k] = (uint8_t)(Rnd() % 3);
        int32_t page = (int32_t)(Rnd() % 4);
        bool oldPic = Rnd() % 2 == 0;
        int32_t idx = 1 + (int32_t)(Rnd() % 4);
        float x = RndF(-1.0f, 1.0f), y = RndF(-1.0f, 1.0f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.5f);
        for (int k = 0; k < 8; ++k) world[0x6987 + k] = (uint8_t)(Rnd() % 3);
        for (int k = 0; k < 8; ++k) *(uint32_t*)(world + 0x4a7f + k * 4) = Rnd() % 3;
        for (int k = 1; k <= 4; ++k) {
            uint8_t* r1 = (uint8_t*)Collection_Get(&named1, 0, k);
            strcpy((char*)r1, names[Rnd() % 4]);
            *(int32_t*)(r1 + 0x1e) = (int32_t)(Rnd() % 5);
            uint8_t* r2 = (uint8_t*)Collection_Get(&named2, 0, k);
            strcpy((char*)r2, names[Rnd() % 4]);
            *(int32_t*)(r2 + 0x3d) = (int32_t)(Rnd() % 5);
            strcpy((char*)Collection_Get(&t268, 0, k) + 4, names[Rnd() % 4]);
            *(uint32_t*)((uint8_t*)Collection_Get(&t84, 0, k) + 0x72) = 1 + Rnd() % 4;
            strcpy((char*)Collection_Get(&t88, 0, k) + 0x45, names[Rnd() % 4]);
        }
        // Edit cursor.
        int32_t bid = Rnd() % 4 == 0 ? 0x7920 + (int32_t)(Rnd() % 2) : 0x790c + (int32_t)(Rnd() % 0x10);
        int32_t active = Rnd() % 4 != 0, cur = (int32_t)(Rnd() % 0x17), remembered = (int32_t)(Rnd() % 0x17);
        uint32_t flags = Rnd() % 3 == 0 ? 0x800 : Rnd() % 3 == 0 ? 0x2000 : 0;
        uint8_t child[2][0xe8];
        char texts[2][3][32];
        uint32_t tabs[2][2], misc[2][2];
        int calls[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(NT * 0xd);
            memset(table, 0, NT * 0xd);
            for (int k = 0; k < NT; ++k) {
                *(uint32_t*)(table + k * 0xd) = 1;
                *(uint32_t*)(table + k * 0xd + 4) = tmpl[k][1];
                table[k * 0xd + 8] = (uint8_t)tmpl[k][2];
                *(uint32_t*)(table + k * 0xd + 9) = 0x6100 + k;
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = NT;
            g_ctlCalls = 0;
            g_sent.clear();
            g_textSide = side;
            g_texts[side].clear();
            memset(child[side], 0, sizeof child[side]);
            memset(texts[side], 0, sizeof texts[side]);
            memset(misc[side], 0, sizeof misc[side]);
            if (op == 5) {
                uint8_t* wn = win[side];
                memset(wn, 0, sizeof win[side]);
                *(void***)wn = winVtbl;
                static void* picVtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
                uint8_t* old = g_delWidgets[side][0];
                memset(old, 0, 0xa0);
                *(void***)old = picVtbl;
                *(uint16_t*)(old + 4) = 0xc79f;
                *(uint8_t**)(wn + 0x70) = *(uint8_t**)(wn + 0x74) = oldPic ? old : nullptr;
                hook::Global<uint8_t*>(0x6d3b44) = wn;
                g_delN = 0;
                side ? rt3::Tutorial_ShowPicture(page, 0, x, y) : ((void(__fastcall*)(int32_t, int, float, float))0x5163f0)(page, 0, x, y);
                uint8_t* c = *(uint8_t**)(wn + 0x70);
                EXPECT_EQ(c != nullptr && c != old, true, "picture iteration %d", it);
                if (c && c != old) {
                    memcpy(child[side], c, sizeof child[side]);
                    const uint32_t heap[2] = { 0x43, 0x47 };
                    for (int k = 0; k < 2; ++k) {
                        if (char* s2 = *(char**)(c + heap[k])) {
                            strncpy(texts[side][k], s2, 31);
                            xfree(s2);
                        }
                        *(uint32_t*)(child[side] + heap[k]) = 0;
                    }
                    *(uint32_t*)(child[side] + 0x86) = *(uint8_t**)(c + 0x86) == wn;
                    *(uint32_t*)(child[side] + 0x82) = 0;
                    memset(child[side] + 0xd8, 0, sizeof child[side] - 0xd8); // past the scrollbar's own fields: allocator garbage
                    xdel(c);
                }
                misc[side][0] = hook::Global<uint32_t>(0x622b30);
                misc[side][1] = (uint32_t)g_delN;
            } else if (op < 4 || op == 6) {
                uint8_t* wn = win[side];
                memset(wn, 0, sizeof win[side]);
                *(void***)wn = winVtbl;
                uint8_t* d = dlg[side];
                memset(d, 0, sizeof dlg[side]);
                *(uint8_t**)(d + 0x86) = wn;
                if (op == 6)
                    side ? rt3::Dialog_AddFilterToggle(d, 0, idx, x, y, 7, h)
                         : ((void(__fastcall*)(uint8_t*, int, int32_t, float, float, int32_t, float))0x511050)(d, 0, idx, x, y, 7, h);
                else
                    side ? mine[op](d, 0, idx, x, y, w, h) : ((AddFn)orig[op])(d, 0, idx, x, y, w, h);
                uint8_t* c = *(uint8_t**)(wn + 0x70);
                EXPECT_EQ(c != nullptr, true, "op %d child iteration %d", op, it);
                if (c) {
                    memcpy(child[side], c, sizeof child[side]);
                    const uint32_t heap[3] = { 0x43, 0x47, 0xb2 };
                    for (int k = 0; k < 3; ++k) {
                        if (char* s = *(char**)(c + heap[k])) {
                            strncpy(texts[side][k], s, 31);
                            xfree(s);
                        }
                        *(uint32_t*)(child[side] + heap[k]) = 0;
                    }
                    *(uint32_t*)(child[side] + 0x4b) = *(uint8_t**)(c + 0x4b) == d;
                    *(uint32_t*)(child[side] + 0xe4) = 0; // never initialized (allocator garbage)
                    *(uint32_t*)(child[side] + 0x86) = *(uint8_t**)(c + 0x86) == wn;
                    xdel(c);
                }
            } else {
                memset(cursor[side], 0, sizeof cursor[side]);
                *(int32_t*)(cursor[side] + 4) = cur;
                *(int32_t*)(cursor[side] + 0xc) = 1; // busy: the change is only queued
                hook::Global<uint8_t*>(0xccbb20) = cursor[side];
                hook::Global<int32_t>(0x622078) = remembered;
                *(uint32_t*)(input + 0xa8c) = flags;
                side ? rt3::EditButton_SetCursor(bid, 0, active, 0, 0, 0, 0)
                     : ((void(__fastcall*)(int32_t, int, int32_t, int32_t, int32_t, int32_t, int32_t))0x4e0a70)(bid, 0, active, 0, 0, 0, 0);
                memcpy(child[side], cursor[side], sizeof cursor[side]);
                misc[side][0] = hook::Global<uint32_t>(0x622078);
            }
            calls[side] = g_ctlCalls;
            static std::vector<uint32_t> sent[2];
            sent[side] = g_sent;
            if (side && sent[0] != sent[1]) {
                size_t k = 0;
                while (k < sent[0].size() && k < sent[1].size() && sent[0][k] == sent[1][k]) ++k;
                EXPECT_EQ(0, 1, "op %d events iteration %d: sizes %d/%d, field %d: %x vs %x", op, it, (int)sent[0].size(), (int)sent[1].size(), (int)k,
                          k < sent[0].size() ? sent[0][k] : 0xdead, k < sent[1].size() ? sent[1][k] : 0xdead);
            }
            for (int k = 0; k < 2; ++k) tabs[side][k] = *(uint32_t*)(table + k * 0xd) + *(uint32_t*)(table + (k + 2) * 0xd) * 16 + *(uint32_t*)(table + (k + 4) * 0xd) * 256;
            xfree(table);
        }
        EXPECT_EQ(memcmp(child[0], child[1], sizeof child[0]), 0, "op %d child iteration %d at %#x", op, it, FirstDiff(child[0], child[1], sizeof child[0]));
        EXPECT_EQ(memcmp(texts[0], texts[1], sizeof texts[0]), 0, "op %d texts iteration %d", op, it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "op %d pack iteration %d", op, it);
        EXPECT_EQ(memcmp(misc[0], misc[1], sizeof misc[0]), 0, "op %d remembered cursor iteration %d", op, it);
        EXPECT_EQ(calls[0], calls[1], "op %d attach calls iteration %d", op, it);
    }
    vt[5] = sv5;
    vt[15] = sv15;
    VirtualProtect(vt, 0x40, prot, &prot);
    vs[5] = ss5;
    vs[15] = ss15;
    VirtualProtect(vs, 0x40, prot2, &prot2);
    memcpy((void*)0x6d1aa8, savedFilter, 8);
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (Collection* c : cs) Collection_Destruct(c);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Cursor_UseGameDrawn(uint8_t* c);
void __fastcall Cursor_UseSystem(uint8_t* c);
void __fastcall Cursor_UseWindows(uint8_t* c);
void __fastcall Cursor_PushBusy(uint8_t* c);
void __fastcall Cursor_PopBusy(uint8_t* c);
void __fastcall Cursor_Shutdown(uint8_t* c);
void __fastcall Cursor_Update(uint8_t* c);
uint8_t* __fastcall Cursor_Construct(uint8_t* c);
void __fastcall Cursor_Reapply(uint8_t* c);
void __fastcall Cursor_PopWindowsMode(uint8_t* c);
}

namespace {
int g_mouseResets;
void __fastcall FakeMouseReset(void*) { ++g_mouseResets; }
}

DIFFTEST(game_CursorModes) {
    typedef void(__fastcall * Fn)(uint8_t*);
    const Fn mine[10] = { rt3::Cursor_UseGameDrawn, rt3::Cursor_UseSystem, rt3::Cursor_UseWindows, rt3::Cursor_PushBusy,
                          rt3::Cursor_PopBusy, rt3::Cursor_Shutdown, rt3::Cursor_Update, (Fn)rt3::Cursor_Construct,
                          rt3::Cursor_Reapply, rt3::Cursor_PopWindowsMode };
    const uint32_t orig[10] = { 0x53f0f0, 0x53f170, 0x53f210, 0x53f2f0, 0x53f310, 0x53f3d0, 0x53f450, 0x53f330, 0x53f2c0, 0x53f400 };
    const uint32_t cgl[5] = { 0xccbb24, 0xccbb1c, 0xccba58, 0xccba5c, 0xccbb18 };
    uint32_t cglSaved[5];
    for (int k = 0; k < 5; ++k) cglSaved[k] = hook::Global<uint32_t>(cgl[k]);
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, prot, &prot);
    FakeEngine fe;
    static void* mouseVtbl[8] = { 0, 0, 0, 0, 0, 0, 0, (void*)&FakeMouseReset };
    static void* mouse[1] = { mouseVtbl };
    static uint8_t modes[0x15];
    *(int32_t*)modes = 1 << 20;
    *(int32_t*)(modes + 4) = 1 << 20;
    *(uint8_t**)(fe.e + 0x34) = modes;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    uint8_t* sPack = hook::Global<uint8_t*>(0x6d4020);
    uint8_t* sMgr = hook::Global<uint8_t*>(0xccbb20);
    int32_t sShown = hook::Global<int32_t>(0x624b20);
    void* sSlots[0x2e];
    memcpy(sSlots, (void*)0xccba60, sizeof sSlots);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    *(int32_t*)(pack + 0x4278c + 6 * 4) = 1;
    void* arrow = (*(void*(__stdcall**)(void*, uint32_t))0x5c82a0)(nullptr, 0x7f00);
    for (int k = 0; k < 0x2e; ++k) ((void**)0xccba60)[k] = arrow;
    int32_t key = rt3::NameTable_Hash(pack, 0, "MouseCursor.imb");
    static uint8_t cur[2][0x30], mgr2[2][0x30];
    for (int it = 0; it < 4000; ++it) {
        int op = (int)(Rnd() % 10);
        uint32_t cglv[5] = { Rnd() % 3, Rnd() % 2, Rnd() % 2, Rnd() % 2, Rnd() % 2 };
        uint32_t gameDrawn = Rnd() % 2, sysFlag = Rnd() % 2;
        uint32_t c0[12];
        for (uint32_t& v : c0) v = 0;
        c0[0] = Rnd() % 2 ? 0x6100 : 0;
        c0[1] = Rnd() % 4 ? (uint32_t)(Rnd() % 23) : 0;
        c0[2] = Rnd() % 2;
        c0[3] = Rnd() % 3;
        c0[4] = Rnd() % 3 ? (uint32_t)(Rnd() % 23) : 0;
        c0[5] = Rnd() % 2;
        c0[6] = Rnd() % 2;
        bool buf = Rnd() % 2 == 0;
        bool useGlobal = Rnd() % 2 == 0; // Cursor_Update: the global manager is this one
        uint32_t m0[12];
        for (uint32_t& v : m0) v = 0;
        m0[1] = Rnd() % 23;
        m0[3] = 1;
        m0[6] = Rnd() % 2;
        uint8_t full = (uint8_t)(Rnd() % 2);
        void* dev = Rnd() % 2 ? (void*)mouse : nullptr;
        int32_t shown = (int32_t)(Rnd() % 3) - 1;
        g_fakeTicks = Rnd();
        uint32_t eng[2][2], tab[2], out[2][2];
        int resets[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(0xd);
            memset(table, 0, 0xd);
            *(uint32_t*)table = 2;
            *(int32_t*)(table + 4) = key;
            table[8] = 6;
            *(uint32_t*)(table + 9) = 0x6100;
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 1;
            uint8_t* c = cur[side];
            memcpy(c, c0, sizeof c0);
            if (buf) {
                *(void**)(c + 0x1c) = xmalloc(16);
            }
            memcpy(mgr2[side], m0, sizeof m0);
            hook::Global<uint8_t*>(0xccbb20) = useGlobal ? c : mgr2[side];
            fe.e[0x114226] = full;
            *(void**)(fe.e + 0x28) = dev;
            *(int32_t*)(fe.e + 0x11473a) = op >= 7 ? (int32_t)sysFlag : 7;
            *(int32_t*)(fe.e + 0x11473e) = op >= 7 ? (int32_t)gameDrawn : 7;
            for (int k = 0; k < 5; ++k) hook::Global<uint32_t>(cgl[k]) = cglv[k];
            for (int k = 0; k < 0x2e; ++k) ((void**)0xccba60)[k] = arrow;
            hook::Global<int32_t>(0x624b20) = shown;
            g_mouseResets = 0;
            side ? mine[op](c) : ((Fn)orig[op])(c);
            if (op != 5 && *(void**)(c + 0x1c)) xfree(*(void**)(c + 0x1c));
            *(uint32_t*)(c + 0x1c) = *(void**)(c + 0x1c) != nullptr;
            eng[side][0] = *(uint32_t*)(fe.e + 0x11473a);
            eng[side][1] = *(uint32_t*)(fe.e + 0x11473e);
            tab[side] = *(uint32_t*)table;
            out[side][0] = (uint32_t)hook::Global<int32_t>(0x624b20);
            out[side][1] = hook::Global<uint32_t>(0xccbb24) + (((void**)0xccba60)[3] == nullptr) * 0x100;
            resets[side] = g_mouseResets;
            xfree(table);
        }
        EXPECT_EQ(memcmp(cur[0], cur[1], sizeof cur[0]), 0, "op %d cursor iteration %d at %#x", op, it, FirstDiff(cur[0], cur[1], sizeof cur[0]));
        EXPECT_EQ(memcmp(mgr2[0], mgr2[1], sizeof mgr2[0]), 0, "op %d manager iteration %d", op, it);
        EXPECT_EQ(memcmp(eng[0], eng[1], sizeof eng[0]), 0, "op %d engine iteration %d", op, it);
        EXPECT_EQ(tab[0], tab[1], "op %d pack iteration %d", op, it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d shown iteration %d", op, it);
        EXPECT_EQ(resets[0], resets[1], "op %d resets iteration %d", op, it);
    }
    (*(BOOL(__stdcall**)())0x5c82a8)();
    for (int k = 0; k < 5; ++k) hook::Global<uint32_t>(cgl[k]) = cglSaved[k];
    memcpy((void*)0xccba60, sSlots, sizeof sSlots);
    hook::Global<int32_t>(0x624b20) = sShown;
    hook::Global<uint8_t*>(0x6d4020) = sPack;
    hook::Global<uint8_t*>(0xccbb20) = sMgr;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall BitmapFont_Destroy(uint8_t* f);
void ReleaseGlobalResource();
void __fastcall Windows_Shutdown(uint8_t* m);
}

DIFFTEST(game_FontAndWindowShutdown) {
    static void* vtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    void** iat = (void**)0x5c8068;
    DWORD prot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, prot, &prot);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x6d401c, 0xccba20, 0xccba18 };
    uint32_t saved[4];
    for (int k = 0; k < 4; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static uint8_t mgr[2][0xd00], font[2][0x40], wins[2][3][0x80];
    for (int it = 0; it < 3000; ++it) {
        int op = (int)(Rnd() % 3);
        uint32_t refs[4];
        for (uint32_t& r : refs) r = 1 + Rnd() % 3;
        int ng = (int)(Rnd() % 4), nt = (int)(Rnd() % 4), nw = (int)(Rnd() % 4), nc = (int)(Rnd() % 3);
        uint32_t gmask = Rnd(), fontId = 1 + Rnd() % 3, haveList = Rnd() % 4 != 0, haveMgr = Rnd() % 5 != 0;
        uint32_t res = Rnd() % 3 ? 0x7100 + Rnd() % 4 : 0;
        g_fakeTicks = Rnd();
        uint32_t tabs[2][4], counts[2], out[2], logs[2][65];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(4 * 0xd);
            memset(table, 0, 4 * 0xd);
            for (int k = 0; k < 4; ++k) {
                *(uint32_t*)(table + k * 0xd) = refs[k];
                *(uint32_t*)(table + k * 0xd + 4) = 0x50 + k;
                table[k * 0xd + 8] = 5;
                *(uint32_t*)(table + k * 0xd + 9) = 0x7100 + k;
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 4;
            uint8_t* m = mgr[side];
            memset(m, 0, sizeof mgr[side]);
            Collection* list = nullptr;
            if (haveList) {
                list = (Collection*)xnew(sizeof(Collection));
                memset(list, 0, sizeof(Collection));
                Collection_Construct(list, 0, 1, 0x10, 4, 4, 0, 0, 0);
                static uint8_t blank[0x10];
                for (int k = 0; k < 3; ++k) Collection_Allocate(list, 0, blank, 0x10, 0, 0);
            }
            *(Collection**)(m + 0xc69) = list;
            hook::Global<uint8_t*>(0x6d401c) = haveMgr ? m : nullptr;
            g_delN = 0;
            memset(g_delLog, 0, sizeof g_delLog);
            out[side] = 0;
            if (op == 0) {
                uint8_t* f = font[side];
                memset(f, 0, sizeof font[side]);
                uint8_t** glyphs = (uint8_t**)xmalloc(4 * sizeof(void*));
                for (int k = 0; k < ng; ++k) glyphs[k] = gmask >> k & 1 ? (uint8_t*)xnew(0x28) : nullptr;
                *(uint8_t***)(f + 0x24) = glyphs;
                *(uint16_t*)(f + 0x28) = (uint16_t)ng;
                *(uint32_t*)(f + 0x2a) = res;
                *(void**)(f + 0x20) = gmask & 0x100 ? xmalloc(8) : nullptr;
                *(uint32_t*)(f + 0x36) = fontId;
                side ? rt3::BitmapFont_Destroy(f) : ((void(__fastcall*)(uint8_t*))0x53e2e0)(f);
                for (uint32_t o : { 0x20u, 0x24u }) *(uint32_t*)(f + o) = *(uint32_t*)(f + o) != 0;
            } else if (op == 1) {
                hook::Global<uint32_t>(0xccba20) = res;
                side ? rt3::ReleaseGlobalResource() : ((void(__cdecl*)())0x53a560)();
                out[side] = hook::Global<uint32_t>(0xccba20);
            } else {
                hook::Global<uint8_t*>(0x6d401c) = m;
                hook::Global<uint32_t>(0xccba18) = 0;
                for (int k = 0; k < nw; ++k) {
                    uint8_t* w = wins[side][k];
                    memset(w, 0, 0x80);
                    *(uint8_t**)(w + 0x58) = k + 1 < nw ? wins[side][k + 1] : nullptr;
                    *(uint8_t**)(w + 0x54) = k ? wins[side][k - 1] : nullptr;
                }
                if (nw) {
                    // Children on the first window.
                    uint8_t* w = wins[side][0];
                    for (int j = 0; j < nc; ++j) {
                        uint8_t* c = g_delWidgets[side][j];
                        memset(c, 0, 0xa0);
                        *(void***)c = vtbl;
                        *(uint16_t*)(c + 4) = (uint16_t)(j + 1);
                        *(uint8_t**)(c + 0x8e) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                        *(uint8_t**)(c + 0x8a) = j ? g_delWidgets[side][j - 1] : nullptr;
                    }
                    *(uint8_t**)(w + 0x70) = nc ? g_delWidgets[side][0] : nullptr;
                    *(uint8_t**)(w + 0x74) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                }
                *(uint8_t**)m = nw ? wins[side][0] : nullptr;
                *(uint8_t**)(m + 4) = nw ? wins[side][nw - 1] : nullptr;
                for (int k = 0; k < nt; ++k) *(void**)(m + 0xbd8 + k * 0x10) = xmalloc(4);
                *(int32_t*)(m + 0xc58) = nt;
                *(void**)(m + 0x10) = res & 1 ? xmalloc(4) : nullptr;
                *(uint32_t*)(m + 0xc) = res;
                *(void**)(m + 0x14) = res & 2 ? xmalloc(4) : nullptr;
                *(uint32_t*)(m + 0x18) = res ? 0x7100 + (res + 1) % 4 : 0;
                side ? rt3::Windows_Shutdown(m) : ((void(__fastcall*)(uint8_t*))0x538a70)(m);
                for (int k = 0; k < nt; ++k) *(uint32_t*)(m + 0xbd8 + k * 0x10) = 1;
                for (int k = 0; k < nw; ++k)
                    for (uint32_t o : { 0x54u, 0x58u, 0x70u, 0x74u }) *(uint32_t*)(wins[side][k] + o) = *(uint32_t*)(wins[side][k] + o) != 0;
                list = nullptr;
            }
            Collection* l2 = *(Collection**)(m + 0xc69);
            counts[side] = l2 ? (uint32_t)Collection_Count(l2) : 0xffff;
            if (l2) {
                Collection_Destruct(l2);
                ((void(__cdecl*)(void*))0x53b080)(l2);
            }
            *(uint32_t*)(m + 0xc69) = 0;
            for (int k = 0; k < 4; ++k) tabs[side][k] = *(uint32_t*)(table + k * 0xd);
            memcpy(logs[side], g_delLog, sizeof g_delLog);
            logs[side][64] = (uint32_t)g_delN;
            xfree(table);
        }
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "op %d pack iteration %d", op, it);
        EXPECT_EQ(counts[0], counts[1], "op %d font list iteration %d", op, it);
        EXPECT_EQ(out[0], out[1], "op %d global iteration %d", op, it);
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "op %d calls iteration %d", op, it);
        EXPECT_EQ(memcmp(mgr[0], mgr[1], sizeof mgr[0]), 0, "op %d manager iteration %d at %#x", op, it, FirstDiff(mgr[0], mgr[1], sizeof mgr[0]));
        EXPECT_EQ(memcmp(font[0], font[1], sizeof font[0]), 0, "op %d font iteration %d", op, it);
        EXPECT_EQ(memcmp(wins[0], wins[1], sizeof wins[0]), 0, "op %d windows iteration %d", op, it);
    }
    for (int k = 0; k < 4; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    VirtualProtect(iat, 4, PAGE_READWRITE, &prot);
    *iat = savedIat;
    VirtualProtect(iat, 4, prot, &prot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Font_ReleaseHandles(uint8_t* t);
void __fastcall Sprite_ReleaseImage(uint8_t* s);
void ReleaseResourceD93990();
void ReleaseResourceD9399c();
void ReleaseResourceD97ab0();
void __fastcall TreeView_ClosePopup(uint8_t* w);
void __fastcall Widget_Destruct(uint8_t* w);
void __fastcall Widget_SetFont(uint8_t* w, int, int32_t font, int32_t temporary);
void __fastcall Panel_SetDragging(uint8_t* w, int, int32_t on);
void CompanyList_ClearRows();
void __fastcall CompanyList_Select(uint8_t* w, int, int32_t idx);
void __fastcall Panel_ReleaseImages(uint8_t* w);
uint8_t* __fastcall Control_Delete(uint8_t* w, int, int32_t flags);
uint8_t* __fastcall Scrollbar_Delete(uint8_t* w, int, int32_t flags);
uint8_t* __fastcall EditBox_Delete(uint8_t* w, int, int32_t flags);
uint8_t* __fastcall TimerWidget_Delete(uint8_t* w, int, int32_t flags);
uint8_t* __fastcall Button_Delete(uint8_t* w, int, int32_t flags);
void __fastcall SpriteSet_Free(uint8_t* o);
uint8_t* __fastcall Object_InitFromCollision(uint8_t* o, int, const char* name, int32_t);
void __fastcall ListBox_Destruct(uint8_t* w);
void CancelMouseAction();
void __fastcall CompanyList_Refill(uint8_t* w);
}

DIFFTEST(game_WidgetReleases) {
    static void* vtbl[8] = { (void*)&DelWidgetDtor, 0, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
    static void* comVtbl[4] = { nullptr, nullptr, (void*)&FakeComRelease, nullptr };
    static void* comObj[1] = { comVtbl };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xfopen = (FILE*(__cdecl*)(const char*, const char*))0x5a1887;
    auto xfclose = (int(__cdecl*)(FILE*))0x5a1740;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_rel.bin");
    { FILE* w = fopen(path, "wb"); fwrite(path, 1, 32, w); fclose(w); }
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x6d401c, 0xd93990, 0xd9399c, 0xd97ab0, 0xccba14, 0xccbb20, 0x6d19f0, 0x625a94, 0x622b00, 0x62be10, 0x6d3abc, 0x6d3ac0, 0x6d3ac4, 0xccbb2c, 0x6d1a8c, 0x6d3ac8, 0x6d1720, 0x6d1aa4 };
    hook::Global<uint32_t>(0xccbb2c) = 0; // no texture list
    FakeEngine fe; // scrollbar textures sample the device's memory
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint32_t>(0xccba14) = 1; // UI sound muted
    const int N = 12;
    // A collision file: object +0 -> record whose +0xc is the mesh (vertices +0, count +8).
    static float verts[3] = { 1.5f, -2.0f, 3.25f };
    static uint8_t mesh[0x10], collRec[0x10];
    static uint8_t* collObj[1] = { collRec };
    *(float**)mesh = verts;
    *(uint8_t**)(collRec + 0xc) = mesh;
    static void* winVtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    static Collection cos;
    memset(&cos, 0, sizeof cos);
    Collection_Construct(&cos, 0, 1, 0x80, 8, 8, 0, 0, 0);
    { static uint8_t blank[0x80]; for (int k = 0; k < 4; ++k) Collection_Allocate(&cos, 0, blank, 0x80, 0, 0); }
    hook::Global<Collection*>(0x62be10) = &cos;
    static uint8_t cursorObj[2][0x30];
    static uint8_t obj[2][0x200], mgr[2][0xd00], wn[2][0x80];
    for (int it = 0; it < 4000; ++it) {
        int op = (int)(Rnd() % 22);
        int which = (int)(Rnd() % 4);
        for (int k = 1; k <= 4; ++k) *(int32_t*)((uint8_t*)Collection_Get(&cos, 0, k) + 0x3f) = Rnd() % 2;
        int32_t sel = (int32_t)(Rnd() % 6) - 1;
        uint32_t curIdx = Rnd() % 23;
        uint32_t refs[N];
        for (uint32_t& r : refs) r = 1 + Rnd() % 3;
        uint32_t fields[8];
        for (uint32_t& f : fields) f = Rnd() % 3 ? 0x7100 + Rnd() % 4 : 0;
        uint32_t flag = Rnd() % 2, font = Rnd() % 8, temp = Rnd() % 2;
        int nc = (int)(Rnd() % 4);
        uint32_t parents[4];
        for (uint32_t& p : parents) p = Rnd() % 2;
        uint32_t tabs[2][N], out[2][8], logs[2][65];
        int coms[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(N * 0xd);
            memset(table, 0, N * 0xd);
            for (int k = 0; k < N; ++k) {
                uint8_t* e = table + k * 0xd;
                *(uint32_t*)e = refs[k];
                if (k < 4) {
                    *(int32_t*)(e + 4) = k == 3 ? rt3::NameTable_Hash(pack, 0, "Coll_Tree.cof") : 0x50 + k;
                    if (k == 3) e[8] = 1;
                    if (k != 3) e[8] = 5;
                    *(uint32_t*)(e + 9) = k == 3 ? (uint32_t)(uintptr_t)collObj : 0x7100 + k;
                } else {
                    *(int32_t*)(e + 4) = 0x754d4c0 + k - 4;
                    e[8] = 5; // a loaded font (no real object: never freed with refs >= 1 after acquire)
                    *(uint32_t*)(e + 9) = 0x7200 + k;
                }
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = N;
            *(int32_t*)(pack + 0x427bc) = 0;
            *(uint8_t**)(pack + 0x20428) = nullptr;
            FILE* f = xfopen(path, "rb");
            *(int32_t*)(pack + 0x204ac) = *(int32_t*)((uint8_t*)f + 0x10);
            uint8_t* o = obj[side];
            memset(o, 0, sizeof obj[side]);
            uint8_t* m = mgr[side];
            memset(m, 0, sizeof mgr[side]);
            hook::Global<uint8_t*>(0x6d401c) = m;
            g_comReleases = 0;
            g_delN = 0;
            memset(g_delLog, 0, sizeof g_delLog);
            memset(out[side], 0, sizeof out[side]);
            switch (op) {
            case 0:
                *(uint32_t*)(o + 0x10) = flag;
                *(uint32_t*)(o + 0x18) = fields[0];
                *(void**)o = fields[1] ? (void*)comObj : nullptr;
                *(uint32_t*)(o + 4) = fields[2] ? 0x12345 : 0;
                side ? rt3::Font_ReleaseHandles(o) : ((void(__fastcall*)(uint8_t*))0x550420)(o);
                break;
            case 1:
                *(uint32_t*)(o + 4) = fields[0];
                *(uint32_t*)(o + 0x20) = 5;
                side ? rt3::Sprite_ReleaseImage(o) : ((void(__fastcall*)(uint8_t*))0x553a00)(o);
                break;
            case 2:
            case 3:
            case 4: {
                const uint32_t slot = op == 2 ? 0xd93990 : op == 3 ? 0xd9399c : 0xd97ab0;
                hook::Global<uint32_t>(slot) = fields[3];
                if (side) op == 2 ? rt3::ReleaseResourceD93990() : op == 3 ? rt3::ReleaseResourceD9399c() : rt3::ReleaseResourceD97ab0();
                else ((void(__cdecl*)())(op == 2 ? 0x553aa0 : op == 3 ? 0x5549b0 : 0x565da0))();
                out[side][0] = hook::Global<uint32_t>(slot);
                break;
            }
            case 5: { // tree view popup
                uint8_t* w = wn[side];
                memset(w, 0, sizeof wn[side]);
                for (int j = 0; j < nc; ++j) {
                    uint8_t* c = g_delWidgets[side][j];
                    memset(c, 0, 0xa0);
                    *(void***)c = vtbl;
                    *(uint16_t*)(c + 4) = (uint16_t)(j + 1);
                    *(uint8_t**)(c + 0x4b) = parents[j] ? o : nullptr;
                    *(uint8_t**)(c + 0x8e) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                    *(uint8_t**)(c + 0x8a) = j ? g_delWidgets[side][j - 1] : nullptr;
                }
                *(uint8_t**)(w + 0x70) = nc ? g_delWidgets[side][0] : nullptr;
                *(uint8_t**)(w + 0x74) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                *(uint8_t**)(o + 0x86) = w;
                *(uint16_t*)(o + 4) = (uint16_t)(fields[4] & 0xff);
                *(uint32_t*)(o + 0x1a8) = flag;
                for (int k = 0; k < 8; ++k) {
                    *(uint16_t*)(m + 0x408 + k * 2) = (uint16_t)k;
                    m[0x20 + k] = 1;
                }
                side ? rt3::TreeView_ClosePopup(o) : ((void(__fastcall*)(uint8_t*))0x55fc50)(o);
                *(uint32_t*)(o + 0x86) = 0;
                for (uint32_t of : { 0x70u, 0x74u })
                    for (int j = 0; j < nc; ++j)
                        if (*(uint8_t**)(w + of) == g_delWidgets[side][j]) *(uint32_t*)(w + of) = 0xb000 + j;
                break;
            }
            case 6: { // widget destructor
                *(uint8_t**)(m + 0xc6d) = flag ? o : nullptr;
                *(uint8_t**)(m + 0xc71) = temp ? o : (uint8_t*)m;
                *(char**)(o + 0x43) = fields[5] ? (char*)xmalloc(4) : nullptr;
                *(char**)(o + 0x47) = fields[6] ? (char*)xmalloc(4) : nullptr;
                *(uint32_t*)(o + 0x63) = fields[0];
                *(uint32_t*)(o + 0x92) = fields[1];
                side ? rt3::Widget_Destruct(o) : ((void(__fastcall*)(uint8_t*))0x558000)(o);
                for (uint32_t of : { 0xc6du, 0xc71u }) {
                    uint8_t* v = *(uint8_t**)(m + of);
                    *(uint32_t*)(m + of) = v == o ? 1 : v == m ? 2 : 0;
                }
                break;
            }
            case 8:
                memset(cursorObj[side], 0, sizeof cursorObj[side]);
                *(uint32_t*)(cursorObj[side] + 4) = curIdx;
                *(uint32_t*)(cursorObj[side] + 0xc) = 1;
                hook::Global<uint8_t*>(0xccbb20) = cursorObj[side];
                side ? rt3::Panel_SetDragging(o, 0, (int32_t)flag) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x507a60)(o, 0, (int32_t)flag);
                memcpy(o + 0x100, cursorObj[side], 0x30);
                hook::Global<uint32_t>(0xccbb20) = 0;
                break;
            case 9:
            case 10: {
                uint8_t* w = wn[side];
                memset(w, 0, sizeof wn[side]);
                *(void***)w = winVtbl;
                const uint16_t pool[4] = { 0x3b5f, 0x3b60, 0x3e7f, 0x3e80 };
                for (int j = 0; j < nc; ++j) {
                    uint8_t* c = g_delWidgets[side][j];
                    memset(c, 0, 0xa0);
                    static void* rowVtbl[8] = { (void*)&DelWidgetDtor, (void*)&FakeLoggingBase, 0, 0, (void*)&DelWidgetFocus, 0, 0, (void*)&DelWidgetRedraw };
                    *(void***)c = rowVtbl;
                    *(uint16_t*)(c + 4) = pool[parents[j] * 2 + (j & 1)];
                    *(uint8_t**)(c + 0x8e) = j + 1 < nc ? g_delWidgets[side][j + 1] : nullptr;
                    *(uint8_t**)(c + 0x8a) = j ? g_delWidgets[side][j - 1] : nullptr;
                }
                *(uint8_t**)(w + 0x70) = nc ? g_delWidgets[side][0] : nullptr;
                *(uint8_t**)(w + 0x74) = nc ? g_delWidgets[side][nc - 1] : nullptr;
                g_sent.clear();
                hook::Global<int32_t>(0x625a94) = 5;
                if (op == 9) {
                    hook::Global<uint8_t*>(0x6d19f0) = flag ? w : nullptr;
                    side ? rt3::CompanyList_ClearRows() : ((void(__cdecl*)())0x50a670)();
                } else {
                    side ? rt3::CompanyList_Select(w, 0, sel) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x50a6d0)(w, 0, sel);
                }
                out[side][1] = hook::Global<uint32_t>(0x625a94);
                out[side][2] = hook::Global<uint32_t>(0x622b00);
                out[side][3] = (uint32_t)g_sent.size();
                for (size_t k = 0; k < g_sent.size() && k < 4; ++k) out[side][4 + k] = g_sent[k + 1];
                for (uint32_t of : { 0x70u, 0x74u })
                    for (int j = 0; j < nc; ++j)
                        if (*(uint8_t**)(w + of) == g_delWidgets[side][j]) *(uint32_t*)(w + of) = 0xb000 + j;
                *(uint32_t*)w = 0;
                break;
            }
            case 11:
                hook::Global<uint32_t>(0x6d3abc) = fields[0];
                hook::Global<uint32_t>(0x6d3ac0) = fields[1];
                hook::Global<uint32_t>(0x6d3ac4) = fields[2];
                *(uint32_t*)(o + 0x78) = 7;
                side ? rt3::Panel_ReleaseImages(o) : ((void(__fastcall*)(uint8_t*))0x50f6d0)(o);
                out[side][0] = hook::Global<uint32_t>(0x6d3abc) | hook::Global<uint32_t>(0x6d3ac0) | hook::Global<uint32_t>(0x6d3ac4);
                break;
            case 12:
            case 13:
            case 14:
            case 15:
            case 16: {
                typedef uint8_t*(__fastcall * DelFn)(uint8_t*, int, int32_t);
                const DelFn mineD[5] = { rt3::Control_Delete, rt3::Scrollbar_Delete, rt3::EditBox_Delete, rt3::TimerWidget_Delete, rt3::Button_Delete };
                const uint32_t origD[5] = { 0x5403e0, 0x55a090, 0x55ba20, 0x5631e0, 0x563d20 };
                const uint32_t textOff[5] = { 0xce, 0xaa, 0xba, 0, 0xb2 };
                int kd = op - 12;
                uint8_t* w = flag ? (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x200) : o;
                memset(w, 0, 0x200);
                *(uint8_t**)(m + 0xc6d) = temp ? w : nullptr;
                *(char**)(w + 0x43) = fields[5] ? (char*)xmalloc(4) : nullptr;
                *(uint32_t*)(w + 0x63) = fields[0];
                *(uint32_t*)(w + 0x92) = fields[1];
                if (textOff[kd]) {
                    if (kd == 1) {
                        uint8_t* tx = fields[6] ? (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x100) : nullptr;
                        if (tx) memset(tx, 0, 0x100);
                        *(uint8_t**)(w + 0xaa) = tx;
                    } else {
                        *(char**)(w + textOff[kd]) = fields[6] ? (char*)xmalloc(4) : nullptr;
                    }
                }
                uint8_t* r = side ? mineD[kd](w, 0, (int32_t)flag) : ((DelFn)origD[kd])(w, 0, (int32_t)flag);
                out[side][0] = r == w;
                out[side][1] = *(uint32_t*)(m + 0xc6d) != 0;
                if (flag) memset(o, 0, sizeof obj[side]);
                break;
            }
            case 17: { // sprite set
                PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(sizeof(PtrList));
                PtrList_Construct(l, 0, 0, 4);
                for (int j = 0; j < nc; ++j) {
                    uint8_t* sp = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x30);
                    memset(sp, 0, 0x30);
                    *(uint32_t*)(sp + 4) = 0x7100 + (fields[j] & 3);
                    PtrList_Append(l, 0, sp);
                }
                *(PtrList**)(o + 0x7d) = flag ? l : nullptr;
                if (!flag) { PtrList_Destroy(l); ((void(__cdecl*)(void*))0x53b080)(l); }
                side ? rt3::SpriteSet_Free(o) : ((void(__fastcall*)(uint8_t*))0x52ef10)(o);
                break;
            }
            case 18: // collision vertex
                *(int32_t*)(mesh + 8) = (int32_t)flag;
                memset(o, 0x11, 0x100);
                out[side][0] = (uint32_t)(uintptr_t)(side ? rt3::Object_InitFromCollision(o, 0, "Coll_Tree.cof", 0)
                                                          : ((uint8_t*(__fastcall*)(uint8_t*, int, const char*, int32_t))0x565dc0)(o, 0, "Coll_Tree.cof", 0)) == (uint32_t)(uintptr_t)o;
                break;
            case 19: { // list box destructor
                uint8_t* w = wn[side];
                memset(w, 0, sizeof wn[side]);
                for (int j = 0; j < 2; ++j) {
                    uint8_t* c = g_delWidgets[side][j];
                    memset(c, 0, 0xa0);
                    *(void***)c = vtbl;
                    *(uint16_t*)(c + 4) = (uint16_t)(j + 1);
                    *(uint16_t*)(c + 0x7c) = (uint16_t)(0xea40 + 8 + j);
                    *(uint8_t**)(c + 0x8e) = j == 0 ? g_delWidgets[side][1] : nullptr;
                    *(uint8_t**)(c + 0x8a) = j == 1 ? g_delWidgets[side][0] : nullptr;
                }
                *(uint8_t**)(w + 0x70) = g_delWidgets[side][0];
                *(uint8_t**)(w + 0x74) = g_delWidgets[side][1];
                *(uint8_t**)(o + 0x86) = w;
                *(uint8_t**)(o + 0x1ad) = parents[0] ? g_delWidgets[side][0] : nullptr;
                *(uint8_t**)(o + 0x1b1) = parents[1] ? g_delWidgets[side][1] : nullptr;
                *(char**)(o + 0x47) = fields[5] ? (char*)xmalloc(4) : nullptr;
                memset(m + 8, 1, 4);
                side ? rt3::ListBox_Destruct(o) : ((void(__fastcall*)(uint8_t*))0x55a0e0)(o);
                *(uint32_t*)(o + 0x86) = 0;
                for (uint32_t of : { 0x70u, 0x74u })
                    for (int j = 0; j < 2; ++j)
                        if (*(uint8_t**)(w + of) == g_delWidgets[side][j]) *(uint32_t*)(w + of) = 0xb000 + j;
                break;
            }
            case 20: { // cancel mouse action
                memset(cursorObj[side], 0, sizeof cursorObj[side]);
                *(uint32_t*)(cursorObj[side] + 4) = curIdx;
                *(uint32_t*)(cursorObj[side] + 0xc) = 1;
                hook::Global<uint8_t*>(0xccbb20) = cursorObj[side];
                *(uint32_t*)(o + 0x83) = flag;
                *(uint32_t*)(o + 0x78) = temp;
                *(uint32_t*)(o + 0x94) = parents[0];
                hook::Global<uint8_t*>(0x6d1a8c) = which == 0 ? o : nullptr;
                hook::Global<uint8_t*>(0x6d3ac8) = which == 1 ? o : nullptr;
                hook::Global<uint8_t*>(0x6d1720) = which == 2 ? o : nullptr;
                hook::Global<int32_t>(0x6d1aa4) = 5;
                side ? rt3::CancelMouseAction() : ((void(__cdecl*)())0x46f410)();
                memcpy(o + 0x100, cursorObj[side], 0x30);
                out[side][0] = hook::Global<uint32_t>(0x6d1aa4);
                hook::Global<uint32_t>(0xccbb20) = 0;
                hook::Global<uint32_t>(0x6d1a8c) = hook::Global<uint32_t>(0x6d3ac8) = hook::Global<uint32_t>(0x6d1720) = 0;
                break;
            }
            case 21: { // company list refill
                uint8_t* w = wn[side];
                memset(w, 0, sizeof wn[side]);
                *(void***)w = winVtbl;
                g_sent.clear();
                hook::Global<int32_t>(0x622b00) = sel;
                side ? rt3::CompanyList_Refill(w) : ((void(__fastcall*)(uint8_t*))0x50ba00)(w);
                out[side][2] = hook::Global<uint32_t>(0x622b00);
                out[side][3] = (uint32_t)g_sent.size();
                uint32_t hsh = 0;
                for (uint32_t v : g_sent) hsh = hsh * 31 + v;
                out[side][4] = hsh;
                *(uint32_t*)w = 0;
                break;
            }
            default: // set font
                *(uint32_t*)(o + 0x63) = fields[0];
                o[0x67] = 9;
                side ? rt3::Widget_SetFont(o, 0, (int32_t)font, (int32_t)temp)
                     : ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x558180)(o, 0, (int32_t)font, (int32_t)temp);
                break;
            }
            xfclose(f);
            coms[side] = g_comReleases;
            for (int k = 0; k < N; ++k) tabs[side][k] = *(uint32_t*)(table + k * 0xd);
            memcpy(logs[side], g_delLog, sizeof g_delLog);
            logs[side][64] = (uint32_t)g_delN;
            xfree(table);
        }
        EXPECT_EQ(memcmp(obj[0], obj[1], sizeof obj[0]), 0, "op %d object iteration %d at %#x", op, it, FirstDiff(obj[0], obj[1], sizeof obj[0]));
        EXPECT_EQ(memcmp(mgr[0], mgr[1], sizeof mgr[0]), 0, "op %d manager iteration %d", op, it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "op %d pack iteration %d", op, it);
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d globals iteration %d", op, it);
        EXPECT_EQ(memcmp(logs[0], logs[1], sizeof logs[0]), 0, "op %d calls iteration %d", op, it);
        EXPECT_EQ(coms[0], coms[1], "op %d releases iteration %d", op, it);
        if (op == 5 || op == 9 || op == 10 || op == 19 || op == 21) EXPECT_EQ(memcmp(wn[0], wn[1], sizeof wn[0]), 0, "op %d window iteration %d", op, it);
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    Collection_Destruct(&cos);
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void EditorPanel_Open();
void GameMessagePanel_Open();
uint8_t* __fastcall Newspaper_Construct(uint8_t* w);
uint8_t* __fastcall TutorialWindow_Construct(uint8_t* w);
uint8_t* __fastcall CompanyListDialog_Construct(uint8_t* w);
uint8_t* __fastcall TrainListDialog_Construct(uint8_t* w);
uint8_t* __fastcall StationListDialog_Construct(uint8_t* w);
uint8_t* __fastcall HighScoresDialog_Construct(uint8_t* w);
uint8_t* __fastcall LoadScreen_Construct(uint8_t* w);
uint8_t* __fastcall FileRequest_Construct(uint8_t* w, int, int32_t kind);
uint8_t* __fastcall FileOptions_Construct(uint8_t* w);
uint8_t* __fastcall GameWindow_Construct(uint8_t* w);
uint8_t* __fastcall StationDetail_Construct(uint8_t* w);
uint8_t* __fastcall WinLose_Construct(uint8_t* w);
}

namespace {
int g_optSaves;
void __fastcall FakeOptionsSave(uint8_t*, int, int32_t) { ++g_optSaves; }
}

DIFFTEST(game_WindowLoad) {
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    auto xdel = (void(__cdecl*)(void*))0x53b080;
    auto xclose = (int(__cdecl*)(int))0x5a440d;
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    strcat(path, "rt3re_dlg.pk4");
    // Every widget kind's attach hooks (vtable slots 5 and 15) are stubbed for both sides.
    const uint32_t vts[9] = { 0x5dd870, 0x5ddd98, 0x5ddd18, 0x5de318, 0x5de360, 0x5de3b0, 0x5ddd58, 0x5de2a8, 0x5ce840 };
    // The panels' own event handlers (slot 1) log instead.
    const uint32_t pvts[13] = { 0x5d0cb8, 0x5d0ed0, 0x5d1368, 0x5d0914, 0x5d19d0, 0x5d177c, 0x5d103c, 0x5d1194, 0x5d0e34, 0x5d0e1c, 0x5d0eb8, 0x5d1748, 0x5d1a58 };
    void* savedPvt[13];
    DWORD pprot[13];
    for (int k = 0; k < 13; ++k) {
        void** vt = (void**)pvts[k];
        VirtualProtect(vt, 8, PAGE_READWRITE, &pprot[k]);
        savedPvt[k] = vt[1];
        vt[1] = (void*)&FakeLoggingBase;
    }
    void* savedVt[9][2];
    DWORD prot[9];
    for (int k = 0; k < 9; ++k) {
        void** vt = (void**)vts[k];
        VirtualProtect(vt, 0x40, PAGE_READWRITE, &prot[k]);
        savedVt[k][0] = vt[5];
        savedVt[k][1] = vt[15];
        vt[5] = vt[15] = (void*)&CountControlCall;
    }
    const uint32_t gl[] = { 0x6d4020, 0x62c120, 0xd9797c, 0x6d3b4c, 0x6d401c, 0x6d07b4, 0x6d081c, 0x6d0818, 0x6d3b3c,
                            0x6cec78, 0x62be10, 0x6ceb9c, 0x6cfcbc, 0x622b2c, 0x6cfe70, 0x6cfe74, 0x6cfe78, 0x6cfe7c, 0x6d3b34, 0x6d3b30,
                            0x6cec20, 0x6d1708, 0x6d1704, 0x6d170c, 0x6d1710, 0x6d402c, 0x6d1084, 0x6d107c, 0x6d1080, 0x6d1094, 0x6d109c,
                            0x6d10b0, 0x6cec74, 0x624324, 0x6d0814, 0x6d3c84, 0x6cec7c, 0x6d07f8, 0x6d07ec, 0x6d07f0,
                            0x6d16d8, 0x6d16dc, 0x6d16e0, 0x6d16e4, 0x6d16e8, 0x6d16ec, 0x6d16f0, 0x6d16f4, 0x6d16f8, 0x6d3b80, 0x624308 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    static uint8_t world[0x5000];
    static Collection emptyCos, emptyPlayers, emptyTrains, emptyBuildings;
    Collection* ecs[4] = { &emptyCos, &emptyPlayers, &emptyTrains, &emptyBuildings };
    for (Collection* c : ecs) {
        memset(c, 0, sizeof(Collection));
        Collection_Construct(c, 0, 1, 0x80, 4, 4, 0, 0, 0);
    }
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<Collection*>(0x62be10) = &emptyCos;
    hook::Global<Collection*>(0x6ceb9c) = &emptyPlayers;
    hook::Global<Collection*>(0x6cfcbc) = &emptyTrains;
    hook::Global<int32_t>(0x622b2c) = -1;
    hook::Global<Collection*>(0x6cec20) = &emptyBuildings;
    hook::Global<uint32_t>(0x6d402c) = 0;
    static uint8_t options[0x600], newGame[0x100];
    hook::Global<uint8_t*>(0x6cec74) = options;
    hook::Global<uint8_t*>(0x6cec7c) = newGame;
    // The options file write is stubbed for both sides.
    uint8_t* osave = (uint8_t*)0x484910;
    uint8_t osaved[5];
    DWORD oprot;
    VirtualProtect(osave, 5, PAGE_EXECUTE_READWRITE, &oprot);
    memcpy(osaved, osave, 5);
    osave[0] = 0xE9;
    *(int32_t*)(osave + 1) = (int32_t)((uintptr_t)&FakeOptionsSave - (0x484910 + 5));
    void** iat = (void**)0x5c8068;
    DWORD iprot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &iprot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, iprot, &iprot);
    static uint8_t view[0x100];
    hook::Global<uint8_t*>(0x6d0818) = view;
    static uint8_t mgr[2][0xd00];
    static uint8_t map[0x2200];
    hook::Global<uint8_t*>(0x62c120) = map;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static const char* dlgNames[15] = { "TestDialog.win", "EditorPanel.win", "GameMessage.win", "Newspaper.win", "Tutorial.win", "CompanyList.win", "TrainList.win",
                                        "StationList.win", "HighScores.win", "LoadScreen.win", "filerqst.win", "fileopt.win", "Game.win", "StationDetail.win", "WinLose.win" };
    struct Kind { int32_t kind, rec; uint32_t text; };
    const Kind kinds[9] = { { 1, 0xa5, 0xbba }, { 2, 0x91, 0xbbb }, { 4, 0x89, 0 }, { 8, 0x12f, 0 }, { 0x10, 0x8b, 0 },
                            { 0x20, 0xa1, 0xbc1 }, { 0x40, 0xe3, 0 }, { 0x80, 0x87, 0 }, { 0x1000, 0x60, 0 } };
    static const char* names[3] = { "Close", "OK_btn.imb", "list.imb" };
    for (int it = 0; it < 400; ++it) {
        int mode = (int)(Rnd() % 15);
        *(int32_t*)(options + 0x68) = (int32_t)(Rnd() % 2);
        *(int32_t*)(world + 0x4af7) = (int32_t)(Rnd() % 2);
        *(uint32_t*)(view + 0x90) = Rnd() % 3;
        hook::Global<int32_t>(0x6d3b80) = (int32_t)(Rnd() % 4);
        {
            static const char* ends[4] = { "", " ", "  You won the scenario!  ", "A very long ending text that goes on and on about the railroad, its trains, its stations, its rivals and the people who built it." };
            strcpy((char*)world + 0x4b47, ends[Rnd() % 4]);
        }
        newGame[0xc5] = (uint8_t)(Rnd() % 2);
        newGame[0x82] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(world + 0x19) = (int32_t)(Rnd() % 5);
        *(int32_t*)(world + 0x4a77) = (int32_t)(Rnd() % 2);
        *(int32_t*)(world + 0x4aeb) = (int32_t)(Rnd() % 2);
        options[0x2c3] = (uint8_t)(Rnd() % 2);
        *(int32_t*)(options + 0x283) = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d1710) = (int32_t)(Rnd() % 2);
        uint8_t lastPic = (uint8_t)(Rnd() % 6);
        uint32_t lcg = Rnd();
        hook::Global<int32_t>(0x6d3c84) = (int32_t)(Rnd() % 2);
        g_fakeTicks = Rnd();
        hook::Global<int32_t>(0x6cfe7c) = hook::Global<int32_t>(0x6d3b30) = (int32_t)(Rnd() % 2);
        *(int32_t*)(view + 0x8c) = 0xc + (int32_t)(Rnd() % 16);
        const char* dlgName = dlgNames[mode];
        int32_t id = rt3::NameTable_Hash(pack, 0, dlgName);
        // The pack file: one entry (the dialog) with a random widget stream.
        std::vector<uint8_t> data;
        auto put = [&](const void* p, size_t n) { data.insert(data.end(), (const uint8_t*)p, (const uint8_t*)p + n); };
        auto putInt = [&](int32_t v) { put(&v, 4); };
        auto putBlock = [&](const char* s) {
            putInt(0);
            int32_t n = s ? (int32_t)strlen(s) + 1 : 0;
            putInt(n);
            if (n) put(s, n);
        };
        putInt(0x7d0);
        uint8_t hdr[0x31];
        for (uint8_t& b : hdr) b = (uint8_t)Rnd();
        put(hdr, sizeof hdr);
        putInt(0x7d1);
        int nw = (int)(Rnd() % 6);
        for (int k = 0; k < nw; ++k) {
            const Kind& kd = kinds[Rnd() % 9];
            putInt(0x7d2);
            putInt(kd.kind);
            putBlock(Rnd() % 3 ? names[Rnd() % 3] : nullptr);
            putBlock(Rnd() % 3 == 0 ? names[1 + Rnd() % 2] : nullptr);
            std::vector<uint8_t> rec(kd.rec);
            for (uint8_t& b : rec) b = (uint8_t)Rnd();
            rec[0x33] = (uint8_t)(Rnd() % 4); // font
            put(rec.data(), rec.size());
            if (kd.text) putBlock(Rnd() % 2 ? "text" : nullptr);
            putInt(0x7d3);
        }
        putInt(0x7d4);
        {
            FILE* f = fopen(path, "wb");
            int32_t ver = 0x3ea, n = 1;
            fwrite(&ver, 4, 1, f);
            fwrite(&n, 4, 1, f);
            uint8_t e[0xd] = {};
            *(int32_t*)e = id;
            fwrite(e, 0xd, 1, f);
            fwrite(data.data(), 1, data.size(), f);
            fclose(f);
        }
        float x = RndF(0.0f, 1.0f), y = RndF(0.0f, 1.0f);
        int32_t demo = Rnd() % 4 == 0;
        uint32_t refs[11];
        for (uint32_t& r : refs) r = 1 + Rnd() % 3;
        static uint8_t win[2][0x80];
        static uint8_t heads[2][6][0xa2];
        char texts[2][6][2][16];
        uint32_t tabs[2][11], misc[2][3];
        int counts[2], calls[2];
        for (int side = 0; side < 2; ++side) {
            memset(pack, 0, 0x43000);
            rt3::Anims_LoadPack(pack, 0, path, 0);
            // Slot table: fonts 0..3 and the two bitmap names.
            uint8_t* table = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)(11 * 0xd);
            memset(table, 0, 11 * 0xd);
            for (int k = 0; k < 11; ++k) {
                uint8_t* e = table + k * 0xd;
                *(uint32_t*)e = refs[k];
                *(int32_t*)(e + 4) = k == 10 ? rt3::NameTable_Hash(pack, 0, "StationDetail.imb") : k == 9 ? 0x754d4c4 : k < 4 ? 0x754d4c0 + k : rt3::NameTable_Hash(pack, 0, k == 8 ? "StationIcons.imb" : k == 7 ? "CompanyLogo.imb" : k == 6 ? "TutorialWindow.imb" : names[k - 3]);
                e[8] = 5;
                *(uint32_t*)(e + 9) = 0x7100 + k;
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 11;
            hook::Global<int32_t>(0x6d3b4c) = demo;
            hook::Global<int32_t>(0xd9797c) = 0;
            g_ctlCalls = 0;
            uint8_t* w = win[side];
            memset(w, 0, sizeof win[side]);
            memset(mgr[side], 0, sizeof mgr[side]);
            misc[side][2] = 0;
            hook::Global<uint8_t*>(0x6d401c) = mgr[side];
            hook::Global<uint32_t>(0x6d07b4) = hook::Global<uint32_t>(0x6d081c) = 0;
            hook::Global<uint32_t>(0x6d3b3c) = 0;
            hook::Global<uint8_t*>(0x6d0818) = view;
            hook::Global<uint32_t>(0x624308) = 0;
            g_sent.clear();
            g_textSide = side;
            g_texts[side].clear();
            uint8_t* alloc = nullptr;
            if (mode == 0) {
                side ? rt3::Window_Load(w, 0, x, y, dlgName) : ((void(__fastcall*)(uint8_t*, int, float, float, const char*))0x53fa50)(w, 0, x, y, dlgName);
            } else if (mode >= 13) {
                static uint8_t sd[2][0x100];
                memset(sd[side], 0, sizeof sd[side]);
                uint8_t* r = side ? (mode == 13 ? rt3::StationDetail_Construct : rt3::WinLose_Construct)(sd[side])
                                  : ((uint8_t*(__fastcall*)(uint8_t*))(mode == 13 ? 0x5068c0 : 0x517660))(sd[side]);
                EXPECT_EQ(r == sd[side], true, "dialog %d iteration %d", mode, it);
                memcpy(w, sd[side], sizeof win[side]);
                *(uint32_t*)(sd[side] + 0x70) = 0;
                misc[side][2] = (hook::Global<uint8_t*>(0x6d16d8) == sd[side]) + hook::Global<uint32_t>(0x624308) * 2 + (hook::Global<uint32_t>(0x6d16e0) << 4) +
                                hook::Global<uint32_t>(0x6d16e4) + hook::Global<uint32_t>(0x6d16e8) * 3 + hook::Global<uint32_t>(0x6d16ec) * 5;
                for (uint32_t g2 : { 0x6d16d8u, 0x6d16dcu, 0x6d16e0u, 0x6d16e4u, 0x6d16e8u, 0x6d16ecu, 0x624308u }) hook::Global<uint32_t>(g2) = 0;
            } else if (mode >= 11) {
                static uint8_t gw[2][0x100];
                memset(gw[side], 0, sizeof gw[side]);
                uint8_t* r = side ? (mode == 11 ? rt3::FileOptions_Construct : rt3::GameWindow_Construct)(gw[side])
                                  : ((uint8_t*(__fastcall*)(uint8_t*))(mode == 11 ? 0x4dc670 : 0x4dfbe0))(gw[side]);
                EXPECT_EQ(r == gw[side], true, "window %d iteration %d", mode, it);
                memcpy(w, gw[side], sizeof win[side]);
                *(uint32_t*)(gw[side] + 0x70) = 0;
                misc[side][2] = (hook::Global<uint8_t*>(0x6d0818) == gw[side]) + gw[side][0x94] * 2 + gw[side][0x95] * 4 + *(uint32_t*)(gw[side] + 0x88) * 8 + *(uint32_t*)(gw[side] + 0x8c) * 16;
            } else if (mode >= 7) {
                static uint8_t dl[2][0x100];
                memset(dl[side], 0, sizeof dl[side]);
                hook::Global<uint32_t>(0x624324) = lcg;
                options[0x2c7] = lastPic;
                g_optSaves = 0;
                typedef uint8_t*(__fastcall * CtorFn)(uint8_t*);
                typedef uint8_t*(__fastcall * Ctor1Fn)(uint8_t*, int, int32_t);
                uint8_t* r;
                if (mode == 10) r = side ? rt3::FileRequest_Construct(dl[side], 0, 1) : ((Ctor1Fn)0x4dcd50)(dl[side], 0, 1);
                else {
                    CtorFn mine2 = mode == 7 ? rt3::StationListDialog_Construct : mode == 8 ? rt3::HighScoresDialog_Construct : rt3::LoadScreen_Construct;
                    r = side ? mine2(dl[side]) : ((CtorFn)(mode == 7 ? 0x5074c0 : mode == 8 ? 0x4e19a0 : 0x4ea620))(dl[side]);
                }
                EXPECT_EQ(r == dl[side], true, "dialog %d iteration %d", mode, it);
                memcpy(w, dl[side], sizeof win[side]);
                *(uint32_t*)(dl[side] + 0x70) = 0;
                misc[side][2] = (hook::Global<uint8_t*>(0x6d1708) == dl[side]) + (hook::Global<uint8_t*>(0x6d10b0) == dl[side]) * 2 + g_optSaves * 4 + options[0x2c7] * 16 +
                                (hook::Global<uint32_t>(0x6d170c) << 8) + (hook::Global<uint32_t>(0x6d109c) == g_fakeTicks - 0 ? 0 : 0);
                misc[side][2] += hook::Global<uint32_t>(0x6d1084) + hook::Global<uint32_t>(0x6d107c) * 3 + hook::Global<uint32_t>(0x6d1080) * 5 + hook::Global<uint32_t>(0x6d1704) * 7;
                if (uint8_t* l = hook::Global<uint8_t*>(0x6d0814)) {
                    misc[side][2] += *(uint32_t*)l * 1000;
                    ((void(__cdecl*)(void*))0x5a1145)(*(void**)(l + 8));
                    xdel(l);
                }
                for (uint32_t g2 : { 0x6d1708u, 0x6d10b0u, 0x6d170cu, 0x6d1084u, 0x6d107cu, 0x6d1080u, 0x6d1704u, 0x6d0814u }) hook::Global<uint32_t>(g2) = 0;
            } else if (mode >= 5) {
                static uint8_t dl[2][0x100];
                memset(dl[side], 0, sizeof dl[side]);
                typedef uint8_t*(__fastcall * CtorFn)(uint8_t*);
                uint8_t* r = side ? (mode == 5 ? rt3::CompanyListDialog_Construct : rt3::TrainListDialog_Construct)(dl[side])
                                  : ((CtorFn)(mode == 5 ? 0x4c7200 : 0x515f50))(dl[side]);
                EXPECT_EQ(r == dl[side], true, "list dialog iteration %d", it);
                memcpy(w, dl[side], sizeof win[side]);
                *(uint32_t*)(dl[side] + 0x70) = 0;
                misc[side][2] = (hook::Global<uint8_t*>(0x6cfe70) == dl[side]) + (hook::Global<uint8_t*>(0x6d3b34) == dl[side]) * 2 + (hook::Global<uint32_t>(0x6cfe74) << 4);
                hook::Global<uint32_t>(0x6cfe70) = hook::Global<uint32_t>(0x6d3b34) = hook::Global<uint32_t>(0x6cfe74) = 0;
            } else if (mode == 4) {
                static uint8_t tw[2][0x100];
                memset(tw[side], 0, sizeof tw[side]);
                uint8_t* r = side ? rt3::TutorialWindow_Construct(tw[side]) : ((uint8_t*(__fastcall*)(uint8_t*))0x516b00)(tw[side]);
                EXPECT_EQ(r == tw[side], true, "tutorial iteration %d", it);
                memcpy(w, tw[side], sizeof win[side]);
                *(uint32_t*)(tw[side] + 0x70) = 0;
            } else if (mode == 3) {
                static uint8_t paper[2][0x100];
                memset(paper[side], 0, sizeof paper[side]);
                uint8_t* r = side ? rt3::Newspaper_Construct(paper[side]) : ((uint8_t*(__fastcall*)(uint8_t*))0x4f38a0)(paper[side]);
                EXPECT_EQ(r == paper[side], true, "newspaper iteration %d", it);
                memcpy(w, paper[side], sizeof win[side]);
                *(uint32_t*)(paper[side] + 0x70) = 0;
            } else {
                if (side) mode == 1 ? rt3::EditorPanel_Open() : rt3::GameMessagePanel_Open();
                else ((void(__cdecl*)())(mode == 1 ? 0x4d4500 : 0x4dfdf0))();
                alloc = hook::Global<uint8_t*>(mode == 1 ? 0x6d07b4 : 0x6d081c);
                if (alloc) {
                    memcpy(w, alloc, sizeof win[side]);
                    memset(w + 0x78, 0, 8); // past what the loader sets: allocator garbage
                }
            }
            *(uint32_t*)(w + 0x54) = *(uint32_t*)(w + 0x54) != 0;
            *(uint32_t*)(w + 0x58) = *(uint32_t*)(w + 0x58) != 0;
            static std::vector<uint32_t> evs[2];
            evs[side] = g_sent;
            for (size_t k = 0; k + 12 < evs[side].size(); k += 13) {
                uint32_t type = evs[side][k + 1];
                if (type == 0x88 || type == 0x6f || type == 0x6e || type == 0x89 || type == 0x70 || type == 0x75) evs[side][k + 3] = evs[side][k + 5] = 0;
                if (type == 0xaa) evs[side][k + 3] = 0; // &window +0x78
                if (type == 0x94) for (int q = 2; q < 12; ++q) evs[side][k + 1 + q] = 0;
            }
            static std::vector<std::string> tx[2];
            tx[side] = g_texts[side];
            g_texts[side].clear();
            if (side) EXPECT_EQ(tx[0] == tx[1], true, "mode %d texts iteration %d", mode, it);
            for (int k = 0; k < 2; ++k) *(uint32_t*)(mgr[side] + k * 4) = *(uint32_t*)(mgr[side] + k * 4) != 0;
            memset(heads[side], 0, sizeof heads[side]);
            memset(texts[side], 0, sizeof texts[side]);
            int n = 0;
            for (uint8_t* c = *(uint8_t**)(w + 0x70); c; ++n) {
                uint8_t* next = *(uint8_t**)(c + 0x8e);
                if (n < 6) {
                    memcpy(heads[side][n], c, 0xa2);
                    for (int k = 0; k < 2; ++k) {
                        char* s = *(char**)(c + 0x43 + k * 4);
                        if (s) strncpy(texts[side][n][k], s, 15);
                        *(uint32_t*)(heads[side][n] + 0x43 + k * 4) = s != nullptr;
                    }
                    *(uint32_t*)(heads[side][n] + 0x82) = 0; // never initialized
                    *(uint32_t*)(heads[side][n] + 0x86) = *(uint8_t**)(c + 0x86) == w;
                    *(uint32_t*)(heads[side][n] + 0x8a) = *(uint32_t*)(c + 0x8a) != 0;
                    *(uint32_t*)(heads[side][n] + 0x8e) = next != nullptr;
                }
                c = next;
            }
            counts[side] = n;
            calls[side] = g_ctlCalls;
            misc[side][0] = hook::Global<uint32_t>(0xd9797c) + (hook::Global<uint32_t>(0x6d3b3c) << 8);
            misc[side][1] = *(uint32_t*)(w + 0x70) != 0;
            misc[side][2] += (uint32_t)(*(uint32_t*)(w + 0x74) != 0) << 28;
            // Free the widgets and their names.
            for (uint8_t* c = *(uint8_t**)(w + 0x70); c;) {
                uint8_t* next = *(uint8_t**)(c + 0x8e);
                for (int k = 0; k < 2; ++k)
                    if (char* s = *(char**)(c + 0x43 + k * 4)) xfree(s);
                xdel(c);
                c = next;
            }
            *(uint32_t*)(w + 0x70) = *(uint32_t*)(w + 0x74) = 0;
            if (alloc) xdel(alloc);
            if (side) EXPECT_EQ(evs[0] == evs[1], true, "mode %d events iteration %d", mode, it);
            for (int k = 0; k < 11; ++k) tabs[side][k] = *(uint32_t*)(table + k * 0xd);
            xfree(table);
            for (int k = 0; k < 32; ++k) xfree(*(void**)(pack + k * 4));
            xclose(*(int32_t*)(pack + 0x204ac));
            EXPECT_EQ(*(uint8_t**)(pack + 0x20428) == nullptr, true, "position stack iteration %d", it);
        }
        EXPECT_EQ(counts[0], counts[1], "widget count iteration %d", it);
        EXPECT_EQ(counts[0], nw, "widgets loaded iteration %d", it);
        EXPECT_EQ(memcmp(win[0], win[1], sizeof win[0]), 0, "window iteration %d at %#x", it, FirstDiff(win[0], win[1], sizeof win[0]));
        EXPECT_EQ(memcmp(heads[0], heads[1], sizeof heads[0]), 0, "widgets iteration %d at %#x", it, FirstDiff(heads[0], heads[1], sizeof heads[0]));
        EXPECT_EQ(memcmp(texts[0], texts[1], sizeof texts[0]), 0, "names iteration %d", it);
        EXPECT_EQ(memcmp(tabs[0], tabs[1], sizeof tabs[0]), 0, "pack iteration %d", it);
        EXPECT_EQ(memcmp(misc[0], misc[1], sizeof misc[0]), 0, "misc mode %d iteration %d: %x %x %x / %x %x %x", mode, it, misc[0][0], misc[0][1], misc[0][2], misc[1][0], misc[1][1], misc[1][2]);
        EXPECT_EQ(memcmp(mgr[0], mgr[1], sizeof mgr[0]), 0, "manager iteration %d", it);
        EXPECT_EQ(calls[0], calls[1], "attach calls iteration %d", it);
    }
    for (int k = 0; k < 9; ++k) {
        void** vt = (void**)vts[k];
        vt[5] = savedVt[k][0];
        vt[15] = savedVt[k][1];
    }
    for (int k = 8; k >= 0; --k) VirtualProtect((void*)vts[k], 0x40, prot[k], &prot[k]); // shared pages: reverse order
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (Collection* c : ecs) Collection_Destruct(c);
    memcpy(osave, osaved, 5);
    VirtualProtect(osave, 5, oprot, &oprot);
    VirtualProtect(iat, 4, PAGE_READWRITE, &iprot);
    *iat = savedIat;
    VirtualProtect(iat, 4, iprot, &iprot);
    for (int k = 0; k < 13; ++k) ((void**)pvts[k])[1] = savedPvt[k];
    for (int k = 12; k >= 0; --k) VirtualProtect((void*)pvts[k], 8, pprot[k], &pprot[k]);
    VirtualFree(pack, 0, MEM_RELEASE);
    DeleteFileA(path);
}

namespace rt3 {
int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce);
int32_t __fastcall World_StepSpeed(uint8_t* w, int, int32_t delta);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
}

namespace {
uint32_t g_mpLog[8];
int g_mpN;
void __fastcall FakeMpSend(uint8_t*, int, int32_t type, int32_t n, uint8_t* data) {
    if (g_mpN < 8) g_mpLog[g_mpN++] = 0x10000 * type + n * 0x100 + data[0] + data[1] * 0x10;
}
void* __fastcall FakeMpChat(uint8_t*, int, const char* text, int32_t, int32_t, int32_t, int32_t) {
    if (g_mpN < 8) g_mpLog[g_mpN++] = 0xc0000000u + (uint32_t)strlen(text);
    return nullptr;
}
}

DIFFTEST(game_GameSpeed) {
    static void* winVtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    auto xfree = (void(__cdecl*)(void*))0x5a1145;
    // The multiplayer senders are stubbed for both sides.
    struct Stub { uint32_t at; void* fn; uint8_t saved[5]; DWORD prot; } stubs[2] = { { 0x407870, (void*)&FakeMpSend }, { 0x469a50, (void*)&FakeMpChat } };
    for (Stub& st : stubs) {
        uint8_t* p = (uint8_t*)st.at;
        VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &st.prot);
        memcpy(st.saved, p, 5);
        p[0] = 0xE9;
        *(int32_t*)(p + 1) = (int32_t)((uintptr_t)st.fn - (st.at + 5));
    }
    void** iat = (void**)0x5c8068;
    DWORD iprot;
    VirtualProtect(iat, 4, PAGE_READWRITE, &iprot);
    void* savedIat = *iat;
    *iat = (void*)&FakeTicks;
    VirtualProtect(iat, 4, iprot, &iprot);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x6cec74, 0x6cec78, 0x6cd8d8, 0x62be24, 0x6d40dc, 0x6cd924, 0xccbb20, 0x6d0818, 0x6d401c, 0xccba18 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static uint8_t options[0x400], world[2][0x7000], lobby[0x10], cursor[2][0x30], view[2][0x80], mgr[2][0xd00];
    hook::Global<uint8_t*>(0x6cec74) = options;
    for (int it = 0; it < 4000; ++it) {
        int op = (int)(Rnd() % 5);
        options[0x2c8] = (uint8_t)(Rnd() % 2);
        int32_t speed = (int32_t)(Rnd() % 13) - 2, request = (int32_t)(Rnd() % 3), announce = (int32_t)(Rnd() % 2);
        int32_t cur = (int32_t)(Rnd() % 10), resume = (int32_t)(Rnd() % 6), mpState = Rnd() % 3 ? 1 : 0;
        bool mp = Rnd() % 3 == 0, host = Rnd() % 2, haveView = Rnd() % 4 != 0, haveWorld = Rnd() % 6 != 0;
        int32_t pending = Rnd() % 3 == 0 ? (int32_t)(Rnd() % 20) : 0, waiting = Rnd() % 4 == 0;
        int32_t lock0 = (int32_t)(Rnd() % 3), lock1 = (int32_t)(Rnd() % 2);
        uint32_t ref = 1 + Rnd() % 3;
        g_fakeTicks = Rnd();
        uint32_t out[2][16];
        char status[2][32];
        static std::vector<uint32_t> evs[2];
        for (int side = 0; side < 2; ++side) {
            uint8_t* table = (uint8_t*)xmalloc(0xd);
            memset(table, 0, 0xd);
            *(uint32_t*)table = ref;
            *(int32_t*)(table + 4) = 0x754d4c7;
            table[8] = 5;
            *(uint32_t*)(table + 9) = 0x7107;
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 1;
            uint8_t* w = world[side];
            memset(w, 0, 0x7000);
            *(int32_t*)(w + 0x19) = cur;
            *(int32_t*)(w + 0x1d) = resume;
            *(int32_t*)(w + 0x66ae) = mpState;
            hook::Global<uint8_t*>(0x6cec78) = haveWorld ? w : nullptr;
            hook::Global<uint8_t*>(0x6cd8d8) = mp ? lobby : nullptr;
            hook::Global<int32_t>(0x6d40dc) = host;
            hook::Global<int32_t>(0x62be24) = pending;
            hook::Global<int32_t>(0x6cd924) = waiting;
            memset(cursor[side], 0, sizeof cursor[side]);
            *(int32_t*)(cursor[side] + 4) = 3;
            *(int32_t*)(cursor[side] + 0xc) = 1; // busy: cursor changes are only queued
            hook::Global<uint8_t*>(0xccbb20) = cursor[side];
            memset(view[side], 0, sizeof view[side]);
            *(void***)view[side] = winVtbl;
            hook::Global<uint8_t*>(0x6d0818) = haveView ? view[side] : nullptr;
            memset(mgr[side], 0, sizeof mgr[side]);
            hook::Global<uint8_t*>(0x6d401c) = mgr[side];
            hook::Global<uint32_t>(0xccba18) = 0;
            g_sent.clear();
            g_mpN = 0;
            memset(g_mpLog, 0, sizeof g_mpLog);
            memset(out[side], 0, sizeof out[side]);
            int32_t lockState[2] = { lock0, lock1 };
            if (op == 0) {
                if (!haveWorld) hook::Global<uint8_t*>(0x6cec78) = w;
                out[side][0] = (uint32_t)(side ? rt3::World_SetSpeed(w, 0, speed, request, announce)
                                               : ((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))0x434680)(w, 0, speed, request, announce));
            } else if (op == 1) {
                out[side][0] = (uint32_t)(side ? rt3::World_TogglePause(w, 0, announce) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x437a60)(w, 0, announce));
            } else if (op == 4) {
                if (!haveWorld) hook::Global<uint8_t*>(0x6cec78) = w;
                out[side][0] = (uint32_t)(side ? rt3::World_StepSpeed(w, 0, speed - 5) : ((int32_t(__fastcall*)(uint8_t*, int, int32_t))0x434850)(w, 0, speed - 5));
            } else if (op == 2) {
                side ? rt3::PauseLock_Push(lockState) : ((void(__fastcall*)(int32_t*))0x483450)(lockState);
            } else {
                lockState[0] = 1 + lock0;
                side ? rt3::PauseLock_Pop(lockState) : ((void(__fastcall*)(int32_t*))0x4834a0)(lockState);
            }
            out[side][1] = (uint32_t)lockState[0];
            out[side][2] = (uint32_t)lockState[1];
            out[side][3] = *(uint32_t*)(w + 0x19);
            out[side][4] = *(uint32_t*)(w + 0x1d);
            out[side][5] = hook::Global<uint32_t>(0x62be24);
            out[side][6] = hook::Global<uint32_t>(0x6cd924);
            out[side][7] = *(uint32_t*)(cursor[side] + 0x10);
            for (int k = 0; k < 8; ++k) out[side][8] = out[side][8] * 31 + g_mpLog[k];
            out[side][9] = (uint32_t)g_mpN;
            out[side][10] = *(uint32_t*)(mgr[side] + 0x18) + *(uint32_t*)(mgr[side] + 0x1c) * 3;
            out[side][11] = *(uint32_t*)table;
            memset(status[side], 0, 32);
            if (char* s = *(char**)(mgr[side] + 0x14)) {
                strncpy(status[side], s, 31);
                xfree(s);
            }
            evs[side] = g_sent;
            xfree(table);
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
        EXPECT_EQ(memcmp(status[0], status[1], 32), 0, "status iteration %d", it);
        EXPECT_EQ(evs[0] == evs[1], true, "events iteration %d", it);
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (Stub& st : stubs) {
        memcpy((void*)st.at, st.saved, 5);
        VirtualProtect((void*)st.at, 5, st.prot, &st.prot);
    }
    VirtualProtect(iat, 4, PAGE_READWRITE, &iprot);
    *iat = savedIat;
    VirtualProtect(iat, 4, iprot, &iprot);
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall Glyph_SnapToPixels(uint8_t* g, int, int32_t mode, float* x, float* y, float* du, float* dv);
}

DIFFTEST(game_GlyphSnap) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyph[0x28];
    *(uint8_t**)(fe.e + 0x34) = modes;
    *(int32_t*)(fe.e + 0x11421e) = 0;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    *(uint8_t**)glyph = frame;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    int snapped = 0;
    for (int it = 0; it < 20000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        int32_t n1 = 4 + (int32_t)(Rnd() % 60), n2 = 4 + (int32_t)(Rnd() % 60);
        *(int32_t*)(frame + 8) = n1 * 2;
        *(int32_t*)(frame + 0xc) = n2 * 2;
        *(float*)(frame + 0x2e) = 2.0f;
        *(float*)(frame + 0x32) = 2.0f;
        const float scales[5] = { 1.0f, 2.0f, 4.0f, 8.0f, 3.0f };
        float tx = scales[Rnd() % 5] * (Rnd() % 4 ? 1.0f : RndF(0.995f, 1.005f)), ty = scales[Rnd() % 5];
        *(float*)(glyph + 0x1c) = tx * sizes[s][0] / n1;
        *(float*)(glyph + 0x20) = ty * sizes[s][1] / n2;
        for (int o = 4; o <= 0x10; o += 4) *(float*)(glyph + o) = RndF(-0.2f, 1.2f);
        tex[4] = (uint8_t)(Rnd() % 4 != 0);
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 16 << (Rnd() % 6);
        *(int32_t*)(img + 0xc) = 16 << (Rnd() % 6);
        int32_t mode = (int32_t)(Rnd() % 3);
        float x0 = RndF(-0.2f, 1.2f), y0 = RndF(-0.2f, 1.2f);
        float out[2][4];
        for (int side = 0; side < 2; ++side) {
            float x = x0, y = y0, du = 9.0f, dv = 9.0f;
            side ? rt3::Glyph_SnapToPixels(glyph, 0, mode, &x, &y, &du, &dv)
                 : ((void(__fastcall*)(uint8_t*, int, int32_t, float*, float*, float*, float*))0x5521e0)(glyph, 0, mode, &x, &y, &du, &dv);
            out[side][0] = x;
            out[side][1] = y;
            out[side][2] = du;
            out[side][3] = dv;
        }
        snapped += out[0][0] != x0;
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "mode %d iteration %d: %g %g %g %g vs %g %g %g %g", mode, it, out[0][0], out[0][1], out[0][2], out[0][3],
                  out[1][0], out[1][1], out[1][2], out[1][3]);
    }
    EXPECT_EQ(snapped > 3000, true, "snapped %d", snapped);
}

namespace rt3 {
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t,
                            int32_t snap, int32_t clip, float left, float top, float right, float bottom);
}

DIFFTEST(game_GlyphQueue) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyph[0x28];
    *(uint8_t**)(fe.e + 0x34) = modes;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    *(uint8_t**)glyph = frame;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    for (int it = 0; it < 20000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int o = 4; o <= 0x20; o += 4) *(float*)(glyph + o) = RndF(-0.5f, 1.5f);
        glyph[0x24] = (uint8_t)(Rnd() % 2);
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), sx = RndF(0.5f, 2.0f), sy = RndF(0.5f, 2.0f);
        float l = RndF(-0.2f, 1.2f), t = RndF(-0.2f, 1.2f), r = RndF(-0.2f, 1.2f), b = RndF(-0.2f, 1.2f);
        int32_t snap = (int32_t)(Rnd() % 3), clip = (int32_t)(Rnd() % 2);
        uint32_t color = Rnd(), tint = Rnd();
        float d0 = RndF(0.0f, 0.05f);
        uint8_t items[2][0x5e];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = d0;
            side ? rt3::Glyph_Queue(glyph, 0, x, y, tint, sx, sy, color, 0, snap, clip, l, t, r, b)
                 : ((void(__fastcall*)(uint8_t*, int, float, float, uint32_t, float, float, uint32_t, int32_t, int32_t, int32_t, float, float, float, float))0x552560)(
                       glyph, 0, x, y, tint, sx, sy, color, 0, snap, clip, l, t, r, b);
            memcpy(items[side], fe.e + 0x5c, 0x5e);
            EXPECT_EQ(*(int32_t*)(fe.e + 0x58), 1, "queued iteration %d", it);
        }
        EXPECT_EQ(memcmp(items[0], items[1], 0x5e), 0, "clip %d snap %d iteration %d at %d", clip, snap, it, FirstDiff(items[0], items[1], 0x5e));
    }
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
void __fastcall Glyph_QueueRotated(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, float angle,
                                   float px, float py);
}

DIFFTEST(game_GlyphQueueRotated) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyph[0x28];
    *(uint8_t**)(fe.e + 0x34) = modes;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    *(uint8_t**)glyph = frame;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    for (int it = 0; it < 20000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int o = 4; o <= 0x20; o += 4) *(float*)(glyph + o) = RndF(-0.5f, 1.5f);
        glyph[0x24] = (uint8_t)(Rnd() % 2);
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), sx = RndF(0.5f, 2.0f), sy = RndF(0.5f, 2.0f);
        float angle = RndF(-7.0f, 7.0f), px = RndF(-0.5f, 1.5f), py = RndF(-0.5f, 1.5f);
        uint32_t color = Rnd(), tint = Rnd();
        float d0 = RndF(0.0f, 0.05f);
        uint8_t items[2][2 * 0x5e];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = d0;
            side ? rt3::Glyph_QueueRotated(glyph, 0, x, y, tint, sx, sy, color, angle, px, py)
                 : ((void(__fastcall*)(uint8_t*, int, float, float, uint32_t, float, float, uint32_t, float, float, float))0x552900)(
                       glyph, 0, x, y, tint, sx, sy, color, angle, px, py);
            memcpy(items[side], fe.e + 0x5c, 2 * 0x5e);
            EXPECT_EQ(*(int32_t*)(fe.e + 0x58), 2, "queued iteration %d", it);
        }
        EXPECT_EQ(memcmp(items[0], items[1], 2 * 0x5e), 0, "iteration %d at %d", it, FirstDiff(items[0], items[1], 2 * 0x5e));
    }
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
void __fastcall Font_DrawLine(uint8_t* f, int, const uint8_t* text, float x, float y, float width, int32_t, uint32_t color, int32_t align,
                              int32_t snap, uint8_t flags, int32_t* link);
}

DIFFTEST(game_FontDrawLine) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyphs[4][0x28], table[0x30], font[0x90];
    static uint32_t entries[256];
    *(uint8_t**)(fe.e + 0x34) = modes;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    for (int k = 0; k < 4; ++k) *(uint8_t**)glyphs[k] = frame;
    for (int k = 0; k < 256; ++k) entries[k] = (uint32_t)(uintptr_t)glyphs[k % 4];
    *(uint32_t**)(table + 0x24) = entries;
    *(uint16_t*)(table + 0x28) = 256;
    *(uint8_t**)(font + 0x18) = table;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    const char alphabet[] = "ab[]|0 7\n.W";
    int turned = 0, hits = 0, carets = 0;
    for (int it = 0; it < 6000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int k = 0; k < 4; ++k) {
            for (int o = 4; o <= 0x20; o += 4) *(float*)(glyphs[k] + o) = RndF(-0.5f, 1.5f);
            glyphs[k][0x24] = (uint8_t)(Rnd() % 2);
        }
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        memset(font + 0x1c, 0, sizeof font - 0x1c);
        *(int32_t*)(font + 0xc) = Rnd() % 3 ? (int32_t)(Rnd() % 22) : (Rnd() % 2 ? 12 : 20); // styles 12 and 20 are turned
        *(float*)(font + 0x14) = RndF(0.5f, 2.0f);
        *(float*)(font + 0x1c) = RndF(0.01f, 0.1f);
        *(float*)(font + 0x28) = RndF(0.0f, 0.02f);
        *(float*)(font + 0x30) = RndF(0.0f, 0.02f);
        int mode = (int)(Rnd() % 4);
        *(int32_t*)(font + 0x40) = mode == 1;
        *(int32_t*)(font + 0x68) = mode == 2;
        *(float*)(font + 0x44) = RndF(-0.2f, 1.2f);
        *(float*)(font + 0x48) = RndF(-0.2f, 1.2f);
        *(double*)(font + 0x4c) = Rnd() % 4 ? 1e9 : RndF(0.0f, 0.5f);
        *(int32_t*)(font + 0x58) = (int32_t)(Rnd() % 50);
        *(float*)(font + 0x64) = RndF(-0.2f, 1.2f);
        *(int32_t*)(font + 0x6c) = Rnd() % 4 == 0;
        *(int32_t*)(font + 0x70) = (int32_t)(Rnd() % 60);
        *(float*)(font + 0x7c) = RndF(-0.2f, 1.2f);
        *(float*)(font + 0x80) = RndF(-0.2f, 1.2f);
        turned += *(const int32_t*)(0x62547c + *(int32_t*)(font + 0xc) * 0x28) != 0;
        uint8_t text[16];
        int n = (int)(Rnd() % 12);
        for (int k = 0; k < n; ++k) text[k] = (uint8_t)alphabet[Rnd() % (sizeof alphabet - 1)];
        text[n] = 0;
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), width = RndF(0.0f, 0.5f);
        uint32_t color = Rnd();
        int32_t align = (int32_t)(Rnd() % 4), snap = (int32_t)(Rnd() % 2), link0 = (int32_t)(Rnd() % 2);
        uint8_t flags = (uint8_t)Rnd();
        float d0 = RndF(0.0f, 0.05f);
        static uint8_t items[2][64 * 0x5e];
        int32_t counts[2], links[2];
        uint8_t fonts[2][0x90];
        uint8_t start[0x90];
        memcpy(start, font, sizeof font);
        for (int side = 0; side < 2; ++side) {
            memcpy(font, start, sizeof font);
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = d0;
            int32_t link = link0;
            side ? rt3::Font_DrawLine(font, 0, text, x, y, width, 0, color, align, snap, flags, &link)
                 : ((void(__fastcall*)(uint8_t*, int, const uint8_t*, float, float, float, int32_t, uint32_t, int32_t, int32_t, uint32_t, int32_t*))0x550dd0)(
                       font, 0, text, x, y, width, 0, color, align, snap, flags, &link);
            counts[side] = *(int32_t*)(fe.e + 0x58);
            memcpy(items[side], fe.e + 0x5c, (size_t)(counts[side] < 64 ? counts[side] : 64) * 0x5e);
            links[side] = link;
            memcpy(fonts[side], font, sizeof font);
        }
        hits += mode == 1 && *(int32_t*)(fonts[0] + 0x54) != 0;
        carets += mode == 2 && *(int32_t*)(fonts[0] + 0x6c) != *(int32_t*)(start + 0x6c);
        EXPECT_EQ(counts[0], counts[1], "count iteration %d", it);
        EXPECT_EQ(links[0], links[1], "link iteration %d", it);
        EXPECT_EQ(memcmp(fonts[0], fonts[1], sizeof font), 0, "font iteration %d at %d", it, FirstDiff(fonts[0], fonts[1], sizeof font));
        if (counts[0] == counts[1] && counts[0] <= 64)
            EXPECT_EQ(memcmp(items[0], items[1], (size_t)counts[0] * 0x5e), 0, "items iteration %d at %d", it, FirstDiff(items[0], items[1], (size_t)counts[0] * 0x5e));
    }
    EXPECT_EQ(turned > 100 && hits > 100 && carets > 100, true, "coverage %d %d %d", turned, hits, carets);
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
void __fastcall Font_DrawText(uint8_t* f, int, const char* text, float x, float y, float width, float height, uint32_t color, int32_t align,
                              int32_t valign, uint32_t flags, int32_t measure, int32_t* lines, float* maxWidth, char* copy, int32_t copyLine);
}

DIFFTEST(game_FontDrawText) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyphs[4][0x28], table[0x30], font[0x90];
    static uint32_t entries[256];
    *(uint8_t**)(fe.e + 0x34) = modes;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    for (int k = 0; k < 4; ++k) *(uint8_t**)glyphs[k] = frame;
    for (int k = 0; k < 256; ++k) entries[k] = (uint32_t)(uintptr_t)glyphs[k % 4];
    *(uint32_t**)(table + 0x24) = entries;
    *(uint16_t*)(table + 0x28) = 256;
    *(uint8_t**)(font + 0x18) = table;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    const char alphabet[] = "ab[]|0 7\n.W";
    int turned = 0, hits = 0, carets = 0;
    for (int it = 0; it < 4000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int k = 0; k < 4; ++k) {
            for (int o = 4; o <= 0x20; o += 4) *(float*)(glyphs[k] + o) = RndF(-0.5f, 1.5f);
            glyphs[k][0x24] = (uint8_t)(Rnd() % 2);
        }
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        memset(font + 0x1c, 0, sizeof font - 0x1c);
        *(int32_t*)(font + 0xc) = Rnd() % 3 ? (int32_t)(Rnd() % 22) : (Rnd() % 2 ? 12 : 20); // styles 12 and 20 are turned
        *(float*)(font + 0x14) = RndF(0.5f, 2.0f);
        *(float*)(font + 0x1c) = RndF(0.01f, 0.1f);
        *(float*)(font + 0x28) = RndF(0.0f, 0.02f);
        *(float*)(font + 0x30) = RndF(0.0f, 0.02f);
        *(float*)(font + 0x20) = Rnd() % 2 ? 0.0f : RndF(0.0f, 1.0f);
        *(float*)(font + 0x24) = RndF(0.0f, 0.01f);
        *(int32_t*)(fe.e + 0x114697) = (int32_t)(Rnd() % 2);
        int mode = (int)(Rnd() % 4);
        *(int32_t*)(font + 0x40) = mode == 1;
        *(int32_t*)(font + 0x68) = mode == 2;
        *(float*)(font + 0x44) = RndF(-0.2f, 1.2f);
        *(float*)(font + 0x48) = RndF(-0.2f, 1.2f);
        *(double*)(font + 0x4c) = Rnd() % 4 ? 1e9 : RndF(0.0f, 0.5f);
        *(int32_t*)(font + 0x58) = (int32_t)(Rnd() % 50);
        *(float*)(font + 0x64) = RndF(-0.2f, 1.2f);
        *(int32_t*)(font + 0x6c) = Rnd() % 4 == 0;
        *(int32_t*)(font + 0x70) = (int32_t)(Rnd() % 60);
        *(float*)(font + 0x7c) = RndF(-0.2f, 1.2f);
        *(float*)(font + 0x80) = RndF(-0.2f, 1.2f);
        turned += *(const int32_t*)(0x62547c + *(int32_t*)(font + 0xc) * 0x28) != 0;
        char text[64];
        int n = (int)(Rnd() % 60);
        for (int k = 0; k < n; ++k) text[k] = alphabet[Rnd() % (sizeof alphabet - 1)];
        text[n] = 0;
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), width = RndF(0.0f, 0.5f), height = RndF(0.0f, 0.5f);
        uint32_t color = Rnd();
        int32_t align = (int32_t)(Rnd() % 7), valign = (int32_t)(Rnd() % 5), measure = Rnd() % 5 == 0, copyLine = (int32_t)(Rnd() % 4);
        uint32_t flags = Rnd() & 0x1ff;
        bool wantOut = Rnd() % 4 != 0;
        float d0 = RndF(0.0f, 0.05f);
        static uint8_t items[2][512 * 0x5e];
        int32_t counts[2], links[2];
        float widths[2];
        char copies[2][1000];
        uint8_t fonts[2][0x90];
        uint8_t start[0x90];
        memcpy(start, font, sizeof font);
        for (int side = 0; side < 2; ++side) {
            memcpy(font, start, sizeof font);
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = d0;
            int32_t link = -5;
            widths[side] = -5.0f;
            memset(copies[side], 0, sizeof copies[side]);
            side ? rt3::Font_DrawText(font, 0, text, x, y, width, height, color, align, valign, flags, measure, wantOut ? &link : nullptr,
                                      wantOut ? &widths[side] : nullptr, wantOut ? copies[side] : nullptr, copyLine)
                 : ((void(__fastcall*)(uint8_t*, int, const char*, float, float, float, float, uint32_t, int32_t, int32_t, uint32_t, int32_t, int32_t*, float*, char*,
                                       int32_t))0x5514f0)(font, 0, text, x, y, width, height, color, align, valign, flags, measure, wantOut ? &link : nullptr,
                                                          wantOut ? &widths[side] : nullptr, wantOut ? copies[side] : nullptr, copyLine);
            counts[side] = *(int32_t*)(fe.e + 0x58);
            memcpy(items[side], fe.e + 0x5c, (size_t)(counts[side] < 512 ? counts[side] : 512) * 0x5e);
            links[side] = link;
            memcpy(fonts[side], font, sizeof font);
        }
        hits += links[0] > 1;
        carets += counts[0] > 40;
        EXPECT_EQ(counts[0], counts[1], "count iteration %d", it);
        EXPECT_EQ(links[0], links[1], "lines iteration %d", it);
        EXPECT_EQ(memcmp(&widths[0], &widths[1], 4), 0, "width iteration %d", it);
        EXPECT_EQ(memcmp(copies[0], copies[1], sizeof copies[0]), 0, "copy iteration %d", it);
        EXPECT_EQ(memcmp(fonts[0], fonts[1], sizeof font), 0, "font iteration %d at %d", it, FirstDiff(fonts[0], fonts[1], sizeof font));
        if (counts[0] == counts[1] && counts[0] <= 512)
            EXPECT_EQ(memcmp(items[0], items[1], (size_t)counts[0] * 0x5e), 0, "items iteration %d at %d", it, FirstDiff(items[0], items[1], (size_t)counts[0] * 0x5e));
    }
    EXPECT_EQ(turned > 100 && hits > 100 && carets > 100, true, "coverage %d %d %d", turned, hits, carets);
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                 int32_t valign, uint32_t flags, float scale);
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out);
int32_t __fastcall Widget_TextLines(uint8_t* w);
void __fastcall Sprite_DrawFirstGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
int32_t __fastcall Widget_LinesToCaret(uint8_t* w);
void __fastcall Sprite_DrawSecondGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
void __stdcall Overlay_DrawText(const char* text, int32_t shadow);
}

namespace {
uint32_t g_d3dxCalls;
// Fake ID3DXFont::DrawTextA: 7 pixels a character, 13 a line, wrapping at the box width.
int32_t __stdcall FakeD3dxDrawText(void*, const char* text, int32_t count, RECT* r, uint32_t format, uint32_t color) {
    int32_t n = (int32_t)strlen(text), perLine = r->right > 7 ? r->right / 7 : 1;
    int32_t lines = n / perLine + 1;
    g_d3dxCalls = g_d3dxCalls * 31 + (uint32_t)count + format + color + (uint32_t)r->right * 7 + (uint32_t)r->bottom;
    r->right = (n < perLine ? n : perLine) * 7;
    r->bottom = lines * 13;
    return r->bottom;
}
}

DIFFTEST(game_FontTextBox) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyphs[4][0x28], table[0x30], font[0x90];
    static uint32_t entries[256];
    static void* d3dxVtbl[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, (void*)&FakeD3dxDrawText };
    static void* d3dxFont[1] = { d3dxVtbl };
    static uint8_t input[0x10] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 }; // the original reads (and drops) the mouse position
    *(uint8_t**)(fe.e + 0x20) = input;
    *(uint8_t**)(fe.e + 0x34) = modes;
    const int32_t sizes[5][2] = { { 640, 480 }, { 800, 600 }, { 1024, 768 }, { 1280, 1024 }, { 1600, 1200 } };
    for (int k = 0; k < 4; ++k) *(uint8_t**)glyphs[k] = frame;
    for (int k = 0; k < 256; ++k) entries[k] = (uint32_t)(uintptr_t)glyphs[k % 4];
    *(uint32_t**)(table + 0x24) = entries;
    *(uint16_t*)(table + 0x28) = 256;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    int32_t savedDebug = hook::Global<int32_t>(0x6d3ff8);
    const char alphabet[] = "ab[]|0  7\n.W-/";
    for (int it = 0; it < 6000; ++it) {
        int s = (int)(Rnd() % 5);
        *(int32_t*)modes = sizes[s][0];
        *(int32_t*)(modes + 4) = sizes[s][1];
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int k = 0; k < 4; ++k) {
            for (int o = 4; o <= 0x20; o += 4) *(float*)(glyphs[k] + o) = RndF(-0.5f, 1.5f);
            glyphs[k][0x24] = (uint8_t)(Rnd() % 2);
        }
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        memset(font, 0, sizeof font);
        bool bitmap = Rnd() % 2;
        *(void**)font = bitmap ? nullptr : (void*)d3dxFont;
        *(float*)(font + 8) = RndF(0.01f, 0.05f);
        *(int32_t*)(font + 0xc) = Rnd() % 3 ? (int32_t)(Rnd() % 22) : (Rnd() % 2 ? 12 : 20);
        *(int32_t*)(font + 0x10) = bitmap;
        *(float*)(font + 0x14) = RndF(0.5f, 2.0f);
        *(uint8_t**)(font + 0x18) = table;
        *(float*)(font + 0x1c) = RndF(0.01f, 0.1f);
        *(float*)(font + 0x20) = Rnd() % 2 ? 0.0f : RndF(0.0f, 1.0f);
        *(float*)(font + 0x24) = RndF(0.0f, 0.01f);
        *(float*)(font + 0x28) = RndF(0.0f, 0.02f);
        *(float*)(font + 0x30) = RndF(0.0f, 0.02f);
        *(int32_t*)(font + 0x40) = Rnd() % 8 == 0;
        *(int32_t*)(font + 0x68) = Rnd() % 8 == 0;
        *(double*)(font + 0x4c) = 1e9;
        *(int32_t*)(fe.e + 0x114697) = (int32_t)(Rnd() % 2);
        hook::Global<int32_t>(0x6d3ff8) = Rnd() % 4 == 0;
        char text[64];
        int n = (int)(Rnd() % 60);
        for (int k = 0; k < n; ++k) text[k] = alphabet[Rnd() % (sizeof alphabet - 1)];
        text[n] = 0;
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), w = RndF(0.0f, 0.5f), h = RndF(0.0f, 0.5f), scale = RndF(0.5f, 2.0f);
        uint32_t color = Rnd(), flags = Rnd() & 0x1ff;
        int32_t align = (int32_t)(Rnd() % 5), valign = (int32_t)(Rnd() % 5), op = (int)(Rnd() % 9), line = (int32_t)(Rnd() % 4);
        bool wantW = Rnd() % 3 != 0;
        float d0 = RndF(0.0f, 0.05f);
        static uint8_t items[2][512 * 0x5e];
        int32_t counts[2];
        uint32_t results[2][3];
        static char lineOut[2][1000];
        static uint8_t widget[0x100], sprite[0xb40];
        uint8_t fonts[2][0x90];
        uint8_t start[0x90];
        memcpy(start, font, sizeof font);
        for (int side = 0; side < 2; ++side) {
            memcpy(font, start, sizeof font);
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = d0;
            g_d3dxCalls = 0;
            float tw = -5.0f;
            results[side][0] = 0;
            if (op == 0) {
                results[side][0] = (uint32_t)(side ? rt3::Font_CountLines(font, 0, text, w, wantW ? &tw : nullptr)
                                                   : ((int32_t(__fastcall*)(uint8_t*, int, const char*, float, float*))0x550470)(font, 0, text, w, wantW ? &tw : nullptr));
            } else if (op == 1) {
                float r = (float)(side ? rt3::Font_TextHeight(font, 0, text, w, wantW ? &tw : nullptr)
                                       : ((double(__fastcall*)(uint8_t*, int, const char*, float, float*))0x550b50)(font, 0, text, w, wantW ? &tw : nullptr));
                memcpy(&results[side][0], &r, 4);
            } else if (op == 3) {
                memset(lineOut[side], 0x55, sizeof lineOut[side]);
                side ? rt3::Font_GetLine(font, 0, text, w, line, lineOut[side])
                     : ((void(__fastcall*)(uint8_t*, int, const char*, float, int32_t, char*))0x551e50)(font, 0, text, w, line, lineOut[side]);
            } else if (op == 4) {
                *(uint8_t**)(widget + 0x63) = font;
                *(const char**)(widget + 0xba) = text;
                *(float*)(widget + 0x13) = w;
                results[side][0] = (uint32_t)(side ? rt3::Widget_TextLines(widget) : ((int32_t(__fastcall*)(uint8_t*))0x55b8e0)(widget));
            } else if (op == 6) {
                *(uint8_t**)(widget + 0x63) = font;
                *(char**)(widget + 0xba) = text;
                *(float*)(widget + 0x13) = w;
                *(int32_t*)(widget + 0xd8) = it % 70 - 5;
                results[side][0] = (uint32_t)(side ? rt3::Widget_LinesToCaret(widget) : ((int32_t(__fastcall*)(uint8_t*))0x55b650)(widget));
                results[side][0] += (uint32_t)strlen(text) << 16;
            } else if (op == 7) {
                *(uint8_t**)(sprite + 0xb2d) = table;
                side ? rt3::Sprite_DrawSecondGlyph(sprite, 0, x, y, 7, 9)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x4e50d0)(sprite, 0, x, y, 7, 9);
            } else if (op == 8) {
                *(uint8_t**)(fe.e + 0x1141f5) = it % 6 ? font : nullptr;
                int32_t shadow = wantW;
                side ? rt3::Overlay_DrawText(text, shadow) : ((void(__stdcall*)(const char*, int32_t))0x566980)(text, shadow);
                *(uint8_t**)(fe.e + 0x1141f5) = nullptr;
            } else if (op == 5) {
                *(uint8_t**)(sprite + 0xb2d) = table;
                side ? rt3::Sprite_DrawFirstGlyph(sprite, 0, x, y, 7, 9)
                     : ((void(__fastcall*)(uint8_t*, int, float, float, int32_t, int32_t))0x4e5080)(sprite, 0, x, y, 7, 9);
            } else {
                side ? rt3::Font_DrawTextBox(font, 0, text, x, y, w, h, color, align, valign, flags, scale)
                     : ((void(__fastcall*)(uint8_t*, int, const char*, float, float, float, float, uint32_t, int32_t, int32_t, uint32_t, float))0x5519f0)(
                           font, 0, text, x, y, w, h, color, align, valign, flags, scale);
            }
            memcpy(&results[side][1], &tw, 4);
            results[side][2] = g_d3dxCalls;
            counts[side] = *(int32_t*)(fe.e + 0x58);
            int32_t m = counts[side] < 512 ? counts[side] : 512;
            memcpy(items[side], fe.e + 0x5c, (size_t)m * 0x5e);
            if ((op == 2 || op == 8) && !bitmap && m > 0) {
                // The text item (last) holds a heap copy of the string: compare the text.
                uint8_t* item = items[side] + (m - 1) * 0x5e;
                const char* str = *(const char**)(item + 0x3d);
                uint32_t hsh = 0;
                if (str)
                    for (const char* c = str; *c; ++c) hsh = hsh * 31 + (uint8_t)*c;
                *(uint32_t*)(item + 0x3d) = hsh;
            }
            memcpy(fonts[side], font, sizeof font);
        }
        EXPECT_EQ(counts[0], counts[1], "count op %d iteration %d", op, it);
        if (op == 3) EXPECT_EQ(memcmp(lineOut[0], lineOut[1], sizeof lineOut[0]), 0, "line iteration %d", it);
        EXPECT_EQ(memcmp(results[0], results[1], sizeof results[0]), 0, "results op %d iteration %d at %d", op, it, FirstDiff(results[0], results[1], sizeof results[0]) / 4);
        EXPECT_EQ(memcmp(fonts[0], fonts[1], sizeof font), 0, "font op %d iteration %d at %d", op, it, FirstDiff(fonts[0], fonts[1], sizeof font));
        if (counts[0] == counts[1] && counts[0] <= 512)
            EXPECT_EQ(memcmp(items[0], items[1], (size_t)counts[0] * 0x5e), 0, "items op %d iteration %d at %d", op, it, FirstDiff(items[0], items[1], (size_t)counts[0] * 0x5e));
    }
    hook::Global<int32_t>(0x6d3ff8) = savedDebug;
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner);
void __fastcall Model_Release(uint8_t* m);
void __fastcall Widget_DrawDebugFrames(uint8_t* w);
void Player_OnLeft();
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
}

namespace {
void CallWithEsi(void* fn, uint8_t* p) {
    __asm {
        push esi
        mov esi, p
        call fn
        pop esi
    }
}
}

DIFFTEST(game_CameraModelFrames) {
    FakeEngine fe;
    static uint8_t modes[0x15];
    *(uint8_t**)(fe.e + 0x34) = modes;
    *(int32_t*)modes = 1024;
    *(int32_t*)(modes + 4) = 768;
    static PtrList lists[3];
    for (int k = 0; k < 3; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x6d401c, 0xccbb20, 0x6d1720, 0x6d40e4, 0xccba14, 0xd939a4 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<int32_t>(0xccba14) = 1; // UI sounds muted
    static uint8_t table[0xd], res[8], holder[0x20], shape[0x20], camera[2][0x40], mgr[0xd00], cursor[2][0x30], panel[2][0x100], player[0x20];
    static float pts[6];
    static uint8_t widget[0x100], parent[0x100];
    int32_t ckey = rt3::NameTable_Hash(pack, 0, "Cam_Test.cof");
    *(uint8_t**)res = holder;
    *(uint8_t**)(holder + 0x10) = shape;
    *(float**)shape = pts;
    for (int it = 0; it < 6000; ++it) {
        int op = (int)(Rnd() % 4);
        for (float& p : pts) p = RndF(-50.0f, 50.0f);
        *(int32_t*)(shape + 4) = Rnd() % 4 ? 2 : (int32_t)(Rnd() % 5);
        *(float*)(shape + 0xc) = RndF(-80.0f, 80.0f);
        *(float*)(shape + 0x10) = RndF(-80.0f, 80.0f);
        uint32_t owner = Rnd();
        // model
        bool m1dd = Rnd() % 2, m64 = Rnd() % 2;
        // widget
        uint8_t vis = (uint8_t)(Rnd() % 4 ? 1 : 0), kind = (uint8_t)(Rnd() % 4);
        uint32_t mask = Rnd() % 4;
        float wf[8];
        for (float& v : wf) v = RndF(-1.0f, 1.0f);
        if (Rnd() % 3 == 0) wf[6] = 0.0f;
        float px = RndF(-1.0f, 1.0f), py = RndF(-1.0f, 1.0f);
        // player
        int32_t local = (int32_t)(Rnd() % 3), who = (int32_t)(Rnd() % 3), drag = (int32_t)(Rnd() % 3);
        bool havePlayer = Rnd() % 4 != 0, havePanel = Rnd() % 4 != 0;
        uint32_t out[2][24];
        for (int side = 0; side < 2; ++side) {
            memset(out[side], 0, sizeof out[side]);
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            depth = 0.01f;
            if (op == 0) {
                memset(table, 0, sizeof table);
                *(int32_t*)table = 1;
                *(int32_t*)(table + 4) = ckey;
                table[8] = 1;
                *(uint8_t**)(table + 9) = res;
                *(uint8_t**)(pack + 0x1f400) = table;
                *(int32_t*)(pack + 0x427b4) = 1;
                memset(camera[side], 0xcc, sizeof camera[side]);
                uint8_t* r = side ? rt3::Camera_Construct(camera[side], 0, "Cam_Test.cof", owner)
                                  : ((uint8_t*(__fastcall*)(uint8_t*, int, const char*, uint32_t))0x553ac0)(camera[side], 0, "Cam_Test.cof", owner);
                out[side][0] = r == camera[side];
                out[side][1] = *(uint32_t*)table;
                memcpy(&out[side][2], camera[side], 0x35);
            } else if (op == 1) {
                uint8_t* m = (uint8_t*)xmalloc(0x200);
                memset(m, 0, 0x200);
                if (m1dd) *(void**)(m + 0x1dd) = xmalloc(16);
                if (m64) *(void**)(m + 0x64) = xmalloc(16);
                hook::Global<int32_t>(0xd939a4) = 7;
                side ? rt3::Model_Release(m) : ((void(__fastcall*)(uint8_t*))0x556460)(m);
                out[side][0] = *(uint32_t*)(m + 0x1dd);
                out[side][1] = *(uint32_t*)(m + 0x64);
                out[side][2] = hook::Global<uint32_t>(0xd939a4);
                out[side][3] = *(uint32_t*)m + *(uint32_t*)(m + 4);
                ((void(__cdecl*)(void*))0x5a1145)(m);
            } else if (op == 2) {
                memset(widget, 0, sizeof widget);
                memset(parent, 0, sizeof parent);
                widget[0x6a] = vis;
                widget[0x68] = 1;
                *(uint8_t**)(widget + 0x86) = parent;
                *(void**)(parent + 0x5c) = parent;
                *(uint16_t*)(widget + 6) = (uint16_t)(1u << kind);
                *(float*)(parent + 5) = px;
                *(float*)(parent + 9) = py;
                for (int k = 0; k < 8; ++k) *(float*)(widget + 0xb + 4 * k) = wf[k];
                *(uint32_t*)(widget + 0x1b) = *(uint32_t*)&wf[4];
                memset(mgr, 0, sizeof mgr);
                *(uint32_t*)(mgr + 0xc79) = mask << kind;
                hook::Global<uint8_t*>(0x6d401c) = mgr;
                side ? rt3::Widget_DrawDebugFrames(widget) : ((void(__fastcall*)(uint8_t*))0x562da0)(widget);
                int32_t n = *(int32_t*)(fe.e + 0x58);
                out[side][0] = (uint32_t)n;
                for (int k = 0; k < n && k < 20; ++k)
                    for (int b = 0; b < 0x5e; b += 4) out[side][1 + k % 20] = out[side][1 + k % 20] * 31 + *(uint32_t*)(fe.e + 0x5c + k * 0x5e + b);
            } else {
                memset(cursor[side], 0, sizeof cursor[side]);
                *(int32_t*)(cursor[side] + 4) = 3;
                *(int32_t*)(cursor[side] + 0xc) = 1;
                hook::Global<uint8_t*>(0xccbb20) = cursor[side];
                memset(panel[side], 0, sizeof panel[side]);
                *(int32_t*)(panel[side] + 0x94) = drag;
                hook::Global<uint8_t*>(0x6d1720) = havePanel ? panel[side] : nullptr;
                hook::Global<int32_t>(0x6d40e4) = local;
                *(int32_t*)(player + 0x18) = who;
                CallWithEsi(side ? (void*)&rt3::Player_OnLeft : (void*)0x46ecd0, havePlayer ? player : nullptr);
                out[side][0] = *(uint32_t*)(panel[side] + 0x94);
                out[side][1] = *(uint32_t*)(cursor[side] + 0x10);
            }
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
void __fastcall StopList_DrawLabel(void* stops, int32_t idx, float x, float y, int32_t, float h);
void __fastcall Lobby_DrawQueue(uint8_t* lobby);
void __fastcall WindowManager_Draw(uint8_t* m);
int32_t ClockMs();
}

DIFFTEST(game_StopLabelAndPings) {
    FakeEngine fe;
    static uint8_t modes[0x15];
    *(uint8_t**)(fe.e + 0x34) = modes;
    *(int32_t*)modes = 1024;
    *(int32_t*)(modes + 4) = 768;
    static PtrList lists[3];
    for (int k = 0; k < 3; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    static uint8_t input[0x10] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 }; // the text box reads (and drops) the mouse position
    *(uint8_t**)(fe.e + 0x20) = input;
    // A GDI font: middle-aligned text without underline is queued without measuring.
    static uint8_t font[0x90], world[0x46aa0], lobby[0x9890];
    *(uint8_t**)(world + 0x46a98) = font;
    *(uint8_t**)(lobby + 0x9884) = font;
    static Collection cities;
    memset(&cities, 0, sizeof cities);
    Collection_Construct(&cities, 0, 1, 0x500, 0x10, 0x10, 0, 0, 0);
    static uint8_t blank[0x500];
    for (int k = 0; k < 4; ++k) Collection_Allocate(&cities, 0, blank, 0x500, 0, 0);
    static uint8_t stopRecs[8 * 0x33];
    static uint8_t* stops[1] = { stopRecs };
    const uint32_t gl[] = { 0x6cec78, 0x6cec20, 0x6cd910, 0x6d408c, 0x6d40a8, 0x6d4020, 0xccba18, 0xccbb20 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6cec78) = world;
    hook::Global<Collection*>(0x6cec20) = &cities;
    // The network queue lock (0x6d4090) is only set up by the network layer.
    CRITICAL_SECTION& cs = hook::Global<CRITICAL_SECTION>(0x6d4090);
    CRITICAL_SECTION savedCs = cs;
    InitializeCriticalSection(&cs);
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    static uint8_t ptable[0xd], mgr[2][0xd00];
    auto xmalloc = (void*(__cdecl*)(size_t))0x5a125d;
    static uint8_t nodes[4][0x60];
    static uint8_t* queue[1];
    for (int it = 0; it < 3000; ++it) {
        memset(font, 0, sizeof font);
        static void* d3dxVtbl2[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, (void*)&FakeD3dxDrawText };
        static void* d3dxFont2[1] = { d3dxVtbl2 };
        *(void**)font = d3dxFont2;
        *(float*)(font + 8) = 0.02f;
        *(int32_t*)(font + 0xc) = (int32_t)(Rnd() % 12);
        for (int k = 1; k <= 4; ++k) sprintf((char*)Collection_Get(&cities, 0, k) + 0x46b, "Town %d-%u", k, Rnd() % 1000);
        for (int k = 0; k < 8; ++k) {
            memset(stopRecs + k * 0x33, 0, 0x33);
            *(uint16_t*)(stopRecs + k * 0x33 + 0x29) = (uint16_t)(1 + Rnd() % 4);
        }
        int n = (int)(Rnd() % 5);
        for (int k = 0; k < 4; ++k) {
            memset(nodes[k], 0, sizeof nodes[k]);
            *(uint8_t**)nodes[k] = k + 1 < n ? nodes[k + 1] : nullptr;
            sprintf((char*)nodes[k] + 8, "Player%u", Rnd() % 100);
            *(int32_t*)(nodes[k] + 0x54) = (int32_t)(Rnd() % 900);
        }
        queue[0] = n ? nodes[0] : nullptr;
        hook::Global<uint8_t**>(0x6d408c) = Rnd() % 5 ? queue : nullptr;
        hook::Global<int32_t>(0x6cd910) = Rnd() % 4 != 0;
        int op = (int)(Rnd() % 3);
        bool frozen = Rnd() % 8 == 0, haveStatus = Rnd() % 4 != 0, statusLive = Rnd() % 3 != 0, haveFont3 = Rnd() % 2;
        int tipKind = (int)(Rnd() % 4); // 0 none, 1 one character, 2 text
        int32_t lineDeadline = (int32_t)(Rnd() % 3); // 0 none, 1 expired, 2 later
        int32_t now = rt3::ClockMs();
        int32_t idx = (int32_t)(Rnd() % 8);
        float x = RndF(-0.2f, 1.2f), y = RndF(-0.2f, 1.2f), h = RndF(0.0f, 0.3f);
        uint32_t out[2][4];
        uint8_t items[2][2][0x5e];
        char texts[2][2][0x400];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            depth = 0.01f;
            hook::Global<int32_t>(0x6d40a8) = 0;
            hook::Global<int32_t>(0xccba18) = 0;
            static uint8_t cursor[2][0x30];
            memset(cursor[side], 0, sizeof cursor[side]);
            *(int32_t*)(cursor[side] + 4) = 3;
            *(int32_t*)(cursor[side] + 0xc) = 1; // busy: cursor changes are only queued
            hook::Global<uint8_t*>(0xccbb20) = cursor[side];
            memset(ptable, 0, sizeof ptable);
            *(int32_t*)ptable = 5; // never drops to 0 (that would free the font)
            *(int32_t*)(ptable + 4) = 0x754d4c3;
            ptable[8] = 7;
            *(uint8_t**)(ptable + 9) = font;
            *(uint8_t**)(pack + 0x1f400) = ptable;
            *(int32_t*)(pack + 0x427b4) = 1;
            if (op == 2) {
                uint8_t* m = mgr[side];
                memset(m, 0, 0xd00);
                m[0xc68] = frozen;
                *(const char**)(m + 0x10) = haveStatus ? "Status line" : nullptr;
                *(int32_t*)(m + 8) = statusLive ? now + 100000 : now - 100000;
                *(uint8_t**)(m + 0xc) = haveFont3 ? font : nullptr;
                if (tipKind) {
                    char* tip = (char*)xmalloc(32);
                    strcpy(tip, tipKind == 1 ? "x" : "Tooltip text");
                    *(char**)(m + 0x14) = tip;
                }
                *(uint8_t**)(m + 0x18) = font;
                *(int32_t*)(m + 0x1c) = lineDeadline == 0 ? 0 : lineDeadline == 1 ? now - 100000 : now + 100000;
                side ? rt3::WindowManager_Draw(m) : ((void(__fastcall*)(uint8_t*))0x538b60)(m);
            } else if (op == 0)
                side ? rt3::StopList_DrawLabel(stops, idx, x, y, 5, h)
                     : ((void(__fastcall*)(void*, int32_t, float, float, int32_t, float))0x4be7f0)(stops, idx, x, y, 5, h);
            else
                side ? rt3::Lobby_DrawQueue(lobby) : ((void(__fastcall*)(uint8_t*))0x469720)(lobby);
            memset(out[side], 0, sizeof out[side]);
            memset(items[side], 0, sizeof items[side]);
            memset(texts[side], 0, sizeof texts[side]);
            out[side][0] = *(uint32_t*)(fe.e + 0x58);
            out[side][1] = hook::Global<uint32_t>(0x6d40a8);
            memcpy(&out[side][2], font + 0x14, 4);
            if (op == 2) {
                uint8_t* m = mgr[side];
                out[side][3] = *(uint32_t*)ptable * 7 + (*(uint32_t*)(m + 0xc) == (uint32_t)(uintptr_t)font) * 2 + (*(uint32_t*)(m + 0x18) != 0) * 4 +
                               (*(uint32_t*)(m + 0x14) != 0) * 8 + (*(uint32_t*)(m + 0x1c) != 0) * 16 + *(uint32_t*)(cursor[side] + 0x10) * 32;
                if (char* tip = *(char**)(m + 0x14)) ((void(__cdecl*)(void*))0x5a1145)(tip);
            }
            for (uint32_t k = 0; k < out[side][0] && k < 2; ++k) {
                memcpy(items[side][k], fe.e + 0x5c + k * 0x5e, 0x5e);
                if (const char* s = *(const char**)(items[side][k] + 0x3d)) strncpy(texts[side][k], s, sizeof texts[side][k] - 1);
                *(uint32_t*)(items[side][k] + 0x3d) = 0;
            }
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
        EXPECT_EQ(memcmp(items[0], items[1], sizeof items[0]), 0, "op %d item iteration %d at %d", op, it, FirstDiff(items[0], items[1], sizeof items[0]));
        EXPECT_EQ(memcmp(texts[0], texts[1], sizeof texts[0]), 0, "op %d text iteration %d: '%.60s' vs '%.60s'", op, it, texts[0][0], texts[1][0]);
    }
    DeleteCriticalSection(&cs);
    cs = savedCs;
    VirtualFree(pack, 0, MEM_RELEASE);
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

DIFFTEST(game_MapCityMarker) {
    FakeEngine fe;
    static uint8_t modes[0x15], frame[0x40], tex[0x100], img[0x20], glyphs[4][0x28], table[0x30], widget[0x130], parent[0x40];
    static uint32_t entries[4];
    *(uint8_t**)(fe.e + 0x34) = modes;
    for (int k = 0; k < 4; ++k) {
        *(uint8_t**)glyphs[k] = frame;
        entries[k] = (uint32_t)(uintptr_t)glyphs[k];
    }
    *(uint32_t**)(table + 0x24) = entries;
    *(uint16_t*)(table + 0x28) = 4;
    *(uint8_t**)(frame + 0x2a) = tex;
    *(uint8_t**)(tex + 0xa0) = img;
    static PtrList lists[4];
    for (int k = 0; k < 4; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(tex + 0xe4) = &lists[3];
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    static Collection cities;
    memset(&cities, 0, sizeof cities);
    Collection_Construct(&cities, 0, 1, 0x200, 0x10, 0x10, 0, 0, 0);
    static uint8_t blank[0x200];
    for (int k = 0; k < 3; ++k) Collection_Allocate(&cities, 0, blank, 0x200, 0, 0);
    Collection* savedCities = hook::Global<Collection*>(0x62bae0);
    hook::Global<Collection*>(0x62bae0) = &cities;
    for (int it = 0; it < 5000; ++it) {
        *(int32_t*)modes = 1024;
        *(int32_t*)(modes + 4) = 768;
        *(int32_t*)(frame + 8) = 64;
        *(int32_t*)(frame + 0xc) = 64;
        *(float*)(frame + 0x2e) = RndF(0.1f, 2.0f);
        *(float*)(frame + 0x32) = RndF(0.1f, 2.0f);
        for (int k = 0; k < 4; ++k) {
            for (int o = 4; o <= 0x20; o += 4) *(float*)(glyphs[k] + o) = RndF(-0.5f, 1.5f);
            glyphs[k][0x24] = (uint8_t)(Rnd() % 2);
        }
        tex[4] = 1;
        tex[0x98] = 1;
        *(int32_t*)(img + 8) = 128;
        *(int32_t*)(img + 0xc) = 128;
        for (int k = 1; k <= 3; ++k) {
            uint8_t* c = (uint8_t*)Collection_Get(&cities, 0, k);
            *(float*)(c + 0x1e2) = RndF(0.0f, 20000.0f);
            *(float*)(c + 0x1ea) = RndF(0.0f, 20000.0f);
        }
        memset(widget, 0, sizeof widget);
        *(uint8_t**)(widget + 0x86) = parent;
        *(float*)(parent + 5) = RndF(0.0f, 0.5f);
        *(float*)(parent + 9) = RndF(0.0f, 0.5f);
        *(uint8_t**)(widget + 0xaf) = table;
        for (int o = 0xb; o <= 0x17; o += 4) *(float*)(widget + o) = RndF(0.0f, 0.8f);
        *(int32_t*)(widget + 0xc3) = (int32_t)(Rnd() % 50);
        *(int32_t*)(widget + 0xc7) = (int32_t)(Rnd() % 50);
        *(int32_t*)(widget + 0xcb) = 60 + (int32_t)(Rnd() % 400);
        *(int32_t*)(widget + 0xcf) = 60 + (int32_t)(Rnd() % 400);
        *(uint32_t*)(widget + 0x11b) = Rnd() % 6 ? 1 + Rnd() % 3 : 7;
        uint8_t items[2][0x5e];
        int32_t counts[2];
        for (int side = 0; side < 2; ++side) {
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            *(void**)(tex + 0xe8) = nullptr;
            depth = 0.01f;
            side ? rt3::MapWidget_DrawCityMarker(widget) : ((void(__fastcall*)(uint8_t*))0x478200)(widget);
            counts[side] = *(int32_t*)(fe.e + 0x58);
            memset(items[side], 0, 0x5e);
            if (counts[side]) memcpy(items[side], fe.e + 0x5c, 0x5e);
        }
        EXPECT_EQ(counts[0], counts[1], "count iteration %d", it);
        EXPECT_EQ(memcmp(items[0], items[1], 0x5e), 0, "item iteration %d at %d", it, FirstDiff(items[0], items[1], 0x5e));
    }
    hook::Global<Collection*>(0x62bae0) = savedCities;
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
void __fastcall Scene_PushBusy(uint8_t* s);
void __fastcall Scene_PopBusy(uint8_t* s);
void __fastcall Scene_Set372c(uint8_t* s, int, uint32_t v);
void __fastcall Scene_Set3720(uint8_t* s, int, uint32_t a, uint32_t b);
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v);
double __stdcall Render_Scaled1540(uint8_t* r);
}

DIFFTEST(game_SceneSmallHelpers) {
    static uint8_t obj[2][0x3800];
    for (int it = 0; it < 20000; ++it) {
        int op = (int)(Rnd() % 6);
        int32_t depth = (int32_t)(Rnd() % 5) - 1;
        uint32_t a = Rnd(), b = Rnd();
        int32_t v = (int32_t)(Rnd() % 600) - 200;
        if (Rnd() % 4 == 0) v = (int32_t)Rnd();
        int32_t n1540 = (int32_t)(Rnd() % 200) - 50;
        double r[2] = { 0, 0 };
        for (int side = 0; side < 2; ++side) {
            uint8_t* o = obj[side];
            memset(o, 0, sizeof obj[side]);
            *(int32_t*)(o + 0x3741) = depth;
            *(int32_t*)(o + 0x1540) = n1540;
            switch (op) {
            case 0: side ? rt3::Scene_PushBusy(o) : ((void(__fastcall*)(uint8_t*))0x523e10)(o); break;
            case 1: side ? rt3::Scene_PopBusy(o) : ((void(__fastcall*)(uint8_t*))0x523e20)(o); break;
            case 2: side ? rt3::Scene_Set372c(o, 0, a) : ((void(__fastcall*)(uint8_t*, int, uint32_t))0x523de0)(o, 0, a); break;
            case 3: side ? rt3::Scene_Set3720(o, 0, a, b) : ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))0x523df0)(o, 0, a, b); break;
            case 4: side ? rt3::Fx_SetIntensity(o, 0, v) : ((void(__fastcall*)(uint8_t*, int, int32_t))0x523d60)(o, 0, v); break;
            default: r[side] = side ? rt3::Render_Scaled1540(o) : ((double(__stdcall*)(uint8_t*))0x524750)(o); break;
            }
        }
        EXPECT_EQ(memcmp(obj[0], obj[1], sizeof obj[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(obj[0], obj[1], sizeof obj[0]));
        EXPECT_EQ(memcmp(&r[0], &r[1], 8), 0, "op %d result iteration %d", op, it);
    }
}

namespace rt3 {
uint8_t* __fastcall Net_FindFlaggedPlayer();
int32_t __stdcall Net_PlayerIdByName(const char* name);
char* __stdcall Net_PlayerNameById(int32_t id);
void __fastcall Net_ResetOutgoing();
int32_t __fastcall Net_FreePendingList(uint8_t* o);
void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);
int32_t __stdcall Net_OpenChannel(uint8_t id);
uint32_t* __stdcall Net_TakeChannelMessages(uint8_t id);
int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes);
int32_t __fastcall Net_SetPlayerName(uint8_t* o, int, const char* name);
double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
}

namespace {
uint32_t g_peerLog;
int32_t __stdcall FakeGetSendQueueInfo(void*, uint32_t id, uint32_t* msgs, uint32_t* bytes, uint32_t flags) {
    g_peerLog = g_peerLog * 31 + id + flags;
    if (id % 3 == 0) return (int32_t)0x80004005;
    *msgs = id * 3;
    *bytes = id * 100 + 7;
    return 0;
}
int32_t __stdcall FakeSetPeerInfo(void*, const uint32_t* info, void* ctx, void* handle, uint32_t flags) {
    g_peerLog = g_peerLog * 31 + info[0] + info[1] * 7 + info[3] + info[4] + info[5] + (uint32_t)(uintptr_t)ctx + (uint32_t)(uintptr_t)handle + flags;
    for (const wchar_t* c = (const wchar_t*)(uintptr_t)info[2]; *c; ++c) g_peerLog = g_peerLog * 31 + *c;
    return (g_peerLog & 3) ? 0 : (int32_t)0x80004005;
}
}

DIFFTEST(net_PlayerListHelpers) {
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    CRITICAL_SECTION& cs1 = hook::Global<CRITICAL_SECTION>(0x6d4090);
    CRITICAL_SECTION& cs2 = hook::Global<CRITICAL_SECTION>(0x6d4048);
    CRITICAL_SECTION saved1 = cs1, saved2 = cs2;
    InitializeCriticalSection(&cs1);
    InitializeCriticalSection(&cs2);
    const uint32_t gl[] = { 0x6d408c, 0x6d40a8, 0x6d4044, 0x6d4060, 0x6d40ec, 0x6d40b0, 0x6d40ac, 0x6d4084, 0x6d40e0 };
    CRITICAL_SECTION& cs3 = hook::Global<CRITICAL_SECTION>(0x6d406c);
    CRITICAL_SECTION saved3 = cs3;
    InitializeCriticalSection(&cs3);
    static void* peerVtbl[24];
    peerVtbl[8] = (void*)&FakeGetSendQueueInfo;
    peerVtbl[20] = (void*)&FakeSetPeerInfo;
    static void* peer[1] = { peerVtbl };
    static uint8_t dpHolder[0x10], netObj[0x220];
    *(void**)(dpHolder + 8) = peer;
    *(uint8_t**)(netObj + 0x18e) = dpHolder;
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    static uint8_t nodes[5][0x26f4];
    static uint8_t* queue[1];
    static uint32_t outList[3];
    static uint8_t scene[0x3720];
    uint8_t savedFlagged[0x26f4], savedName[0x40];
    memcpy(savedFlagged, (void*)0x6d6918, sizeof savedFlagged);
    memcpy(savedName, (void*)0x6d9010, sizeof savedName);
    const char* names[4] = { "Alice", "bob", "BOB", "Carol" };
    for (int it = 0; it < 4000; ++it) {
        int n = (int)(Rnd() % 6);
        for (int k = 0; k < 5; ++k) {
            memset(nodes[k], 0, 0x60);
            *(uint8_t**)nodes[k] = k + 1 < n ? nodes[k + 1] : nullptr;
            *(int32_t*)(nodes[k] + 4) = 1 + (int32_t)(Rnd() % 6);
            strcpy((char*)nodes[k] + 8, names[Rnd() % 4]);
            *(int32_t*)(nodes[k] + 0x50) = Rnd() % 4 == 0;
            for (int b = 0x60; b < 0x26f4; b += 0x400) nodes[k][b] = (uint8_t)Rnd();
        }
        queue[0] = n ? nodes[0] : nullptr;
        hook::Global<uint8_t**>(0x6d408c) = queue;
        int op = (int)(Rnd() % 11);
        static uint8_t chans[2][6][0x20];
        static uint32_t chanList[2][3], freeList[2][3];
        static uint8_t pool[2][0x14];
        int nch = (int)(Rnd() % 6), nfree = (int)(Rnd() % 3);
        uint8_t chanIds[6];
        uint32_t chanMsgs[6];
        for (int k = 0; k < 6; ++k) {
            chanIds[k] = (uint8_t)(Rnd() % 8);
            chanMsgs[k] = Rnd() % 3 ? Rnd() : 0;
        }
        uint8_t cid = (uint8_t)(Rnd() % 8);
        bool reuse = Rnd() % 2, netUp = Rnd() % 5 != 0;
        float depthV = RndF(0.0f, 3000.0f);
        int32_t dist = (int32_t)(Rnd() % 200) - 20;
        char pname[24];
        sprintf(pname, "Player %u", Rnd() % 1000);
        const char* query = names[Rnd() % 4];
        int32_t qid = 1 + (int32_t)(Rnd() % 7);
        int pending = (int)(Rnd() % 5);
        for (int k = 0; k < 0x30; ++k) scene[0x36e8 + k] = (uint8_t)Rnd();
        uint32_t out[2][16];
        for (int side = 0; side < 2; ++side) {
            memset(out[side], 0, sizeof out[side]);
            memset((void*)0x6d6918, 0, 0x26f4);
            memset((void*)0x6d9010, 0x77, 0x40);
            hook::Global<int32_t>(0x6d40a8) = 0;
            hook::Global<int32_t>(0x6d4060) = 0;
            hook::Global<int32_t>(0x6d40ec) = 5;
            if (op == 0) {
                uint8_t* r = side ? rt3::Net_FindFlaggedPlayer() : ((uint8_t*(__fastcall*)())0x522510)();
                out[side][0] = (uint32_t)(uintptr_t)r;
                for (int b = 0; b < 0x26f4; b += 4) out[side][1] = out[side][1] * 31 + *(uint32_t*)(0x6d6918 + b);
            } else if (op == 1) {
                out[side][0] = (uint32_t)(side ? rt3::Net_PlayerIdByName(query) : ((int32_t(__stdcall*)(const char*))0x522600)(query));
            } else if (op == 2) {
                char* r = side ? rt3::Net_PlayerNameById(qid) : ((char*(__stdcall*)(int32_t))0x522580)(qid);
                out[side][0] = (uint32_t)(uintptr_t)r;
                memcpy(&out[side][1], (void*)0x6d9010, 16);
            } else if (op == 3) {
                outList[0] = 0x111;
                outList[1] = 0x222;
                outList[2] = 3;
                hook::Global<uint32_t*>(0x6d4044) = outList;
                side ? rt3::Net_ResetOutgoing() : ((void(__fastcall*)())0x5223d0)();
                memcpy(&out[side][0], outList, 12);
            } else if (op == 4) {
                static uint8_t holder[8];
                memset(holder, 0, sizeof holder);
                if (pending) {
                    uint32_t* list = (uint32_t*)xnew(12);
                    list[0] = list[1] = 0;
                    list[2] = 0;
                    for (int k = 0; k < pending - 1; ++k) {
                        uint32_t* node = (uint32_t*)xnew(8);
                        node[0] = list[0];
                        node[1] = k;
                        if (!list[0]) list[1] = (uint32_t)(uintptr_t)node;
                        list[0] = (uint32_t)(uintptr_t)node;
                        ++list[2];
                    }
                    *(uint32_t**)(holder + 4) = list;
                }
                out[side][0] = (uint32_t)(side ? rt3::Net_FreePendingList(holder) : ((int32_t(__fastcall*)(uint8_t*))0x522a40)(holder));
                out[side][1] = *(uint32_t*)(holder + 4);
            } else if (op == 6 || op == 7) {
                // Channel list (in fixed arrays; the pool's free list holds the spares).
                memset(chans[side], 0, sizeof chans[side]);
                memset(chanList[side], 0, 12);
                memset(freeList[side], 0, 12);
                for (int k = nch - 1; k >= 0; --k) {
                    uint8_t* c = chans[side][k];
                    *(uint32_t*)c = chanList[side][0];
                    if (!chanList[side][0]) chanList[side][1] = (uint32_t)(uintptr_t)c;
                    chanList[side][0] = (uint32_t)(uintptr_t)c;
                    ++chanList[side][2];
                    c[0x1c] = chanIds[k];
                    *(uint32_t*)(c + 4) = chanMsgs[k];
                    *(uint32_t*)(c + 8) = chanMsgs[k] * 3;
                    *(uint32_t*)(c + 0xc) = chanMsgs[k] ? 2 : 0;
                }
                for (int k = 0; k < 1 + nfree; ++k) {
                    if (5 - k < nch) break;
                    uint8_t* c = chans[side][5 - k];
                    memset(c, 0xee, 0x20);
                    *(uint32_t*)c = freeList[side][0];
                    if (!freeList[side][0]) freeList[side][1] = (uint32_t)(uintptr_t)c;
                    freeList[side][0] = (uint32_t)(uintptr_t)c;
                    ++freeList[side][2];
                }
                memset(pool[side], 0, sizeof pool[side]);
                *(uint32_t**)pool[side] = freeList[side];
                *(int32_t*)(pool[side] + 0xc) = reuse && freeList[side][0];
                *(int32_t*)(pool[side] + 0x10) = 0x20;
                hook::Global<uint32_t*>(0x6d40b0) = chanList[side];
                hook::Global<uint8_t*>(0x6d40ac) = pool[side];
                hook::Global<int32_t>(0x6d4084) = 0;
                if (!netUp) hook::Global<uint32_t>(0x6d408c) = 0;
                uint8_t* fresh = nullptr;
                if (op == 6) {
                    out[side][0] = (uint32_t)(side ? rt3::Net_OpenChannel(cid) : ((int32_t(__stdcall*)(uint8_t))0x522340)(cid));
                    uint8_t* head0 = (uint8_t*)(uintptr_t)chanList[side][0];
                    if (head0 && (head0 < chans[side][0] || head0 >= chans[side][0] + sizeof chans[side])) fresh = head0;
                } else {
                    uint32_t* r = side ? rt3::Net_TakeChannelMessages(cid) : ((uint32_t*(__stdcall*)(uint8_t))0x522f70)(cid);
                    out[side][0] = r ? (uint32_t)((uint8_t*)r - chans[side][0]) : 0xffffffff;
                }
                uint8_t* base = chans[side][0];
                auto rel = [base, fresh](uint32_t p) -> uint32_t {
                    uint8_t* q = (uint8_t*)(uintptr_t)p;
                    if (!q) return 0;
                    if (q == fresh) return 0xf00d;
                    return (uint32_t)(q - base) + 1;
                };
                out[side][1] = rel(chanList[side][0]);
                out[side][2] = rel(chanList[side][1]);
                out[side][3] = chanList[side][2];
                out[side][4] = rel(freeList[side][0]) + rel(freeList[side][1]) * 0x1000 + freeList[side][2] * 0x1000000;
                for (int k = 0; k < 6; ++k)
                    for (int b = 0; b < 0x20; b += 4) {
                        uint32_t v = *(uint32_t*)(chans[side][k] + b);
                        if (b == 0) v = rel(v);
                        out[side][5] = out[side][5] * 31 + v;
                    }
                if (fresh) {
                    out[side][6] = *(uint32_t*)(fresh + 4) + *(uint32_t*)(fresh + 8) * 3 + *(uint32_t*)(fresh + 0xc) * 5 + fresh[0x1c] * 7;
                    out[side][7] = rel(*(uint32_t*)fresh);
                    ((void(__cdecl*)(void*))0x53b080)(fresh);
                }
                out[side][8] = hook::Global<uint32_t>(0x6d4084);
            } else if (op == 8) {
                g_peerLog = 0;
                int32_t msgs = 5, bytes = 9;
                out[side][0] = (uint32_t)(side ? rt3::Net_SendQueueTotals(netObj, 0, &msgs, &bytes)
                                               : ((int32_t(__fastcall*)(uint8_t*, int, int32_t*, int32_t*))0x522990)(netObj, 0, &msgs, &bytes));
                out[side][1] = (uint32_t)msgs;
                out[side][2] = (uint32_t)bytes;
                out[side][3] = g_peerLog;
            } else if (op == 9) {
                g_peerLog = 0;
                hook::Global<uint32_t>(0x6d40e0) = 0;
                out[side][0] = (uint32_t)(side ? rt3::Net_SetPlayerName(netObj, 0, pname) : ((int32_t(__fastcall*)(uint8_t*, int, const char*))0x521c80)(netObj, 0, pname));
                out[side][1] = g_peerLog;
                out[side][2] = hook::Global<uint32_t>(0x6d40e0) == (uint32_t)(uintptr_t)peer;
            } else if (op == 10) {
                static uint8_t rnd[0x1548];
                *(int32_t*)(rnd + 0x1540) = dist;
                *(float*)(scene + 0x368e) = depthV;
                double r = side ? rt3::Scene_FogFactor(scene, 0, rnd) : ((double(__fastcall*)(uint8_t*, int, uint8_t*))0x5246b0)(scene, 0, rnd);
                memcpy(&out[side][0], &r, 8);
            } else {
                uint32_t v[12];
                memset(v, 0, sizeof v);
                side ? rt3::Scene_GetVectors(scene, 0, v, v + 4, v + 8)
                     : ((void(__fastcall*)(uint8_t*, int, uint32_t*, uint32_t*, uint32_t*))0x524170)(scene, 0, v, v + 4, v + 8);
                memcpy(&out[side][0], v, sizeof v);
            }
            out[side][14] = hook::Global<uint32_t>(0x6d40a8) + hook::Global<uint32_t>(0x6d4060) * 16;
            out[side][15] = hook::Global<uint32_t>(0x6d40ec);
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
    }
    memcpy((void*)0x6d6918, savedFlagged, sizeof savedFlagged);
    memcpy((void*)0x6d9010, savedName, sizeof savedName);
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    DeleteCriticalSection(&cs1);
    DeleteCriticalSection(&cs2);
    DeleteCriticalSection(&cs3);
    cs3 = saved3;
    cs1 = saved1;
    cs2 = saved2;
}

namespace rt3 {
void __stdcall Net_PurgeSentMessages(int32_t);
uint8_t __fastcall Scene_RayToHeight(uint8_t* s, int, uint8_t* o, float sx, float sy, float height, float* out);
void __fastcall Scene_ComputeFootprint(uint8_t* s);
void __fastcall Scene_UpdateLensFlare(uint8_t* s);
}

DIFFTEST(game_SceneRaysAndPurge) {
    FakeEngine fe;
    static PtrList lists[3];
    for (int k = 0; k < 3; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    *(PtrList**)(fe.e + 0x11369d) = &lists[0];
    *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
    *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
    float& depth = hook::Global<float>(0xd93850);
    float savedDepth = depth;
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    CRITICAL_SECTION& cs = hook::Global<CRITICAL_SECTION>(0x6d4048);
    CRITICAL_SECTION savedCs = cs;
    InitializeCriticalSection(&cs);
    const uint32_t gl[] = { 0x6d4044, 0x6d4040, 0x6d4060 };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    static uint8_t scene[2][0x3720], rnd[0x3480], obj[0x30], settings[0x60], flare[0x100];
    static uint8_t block[2][4][0x120];
    for (int it = 0; it < 8000; ++it) {
        int op = (int)(Rnd() % 4);
        for (int o = 0x342f; o < 0x342f + 0x40; o += 4) *(float*)(rnd + o) = RndF(-2.0f, 2.0f);
        *(float*)(rnd + 0x2523) = RndF(0.5f, 1.5f);
        *(float*)(rnd + 0x2527) = RndF(-0.2f, 0.2f);
        *(float*)(rnd + 0x252b) = RndF(0.5f, 1.5f);
        *(float*)(rnd + 0x252f) = RndF(-0.2f, 0.2f);
        for (int o = 0x1c; o <= 0x24; o += 4) *(float*)(obj + o) = RndF(-50.0f, 50.0f);
        float sx = RndF(0.0f, 1.0f), sy = RndF(0.0f, 1.0f), h = RndF(-5.0f, 5.0f);
        float eye[3] = { RndF(-50.0f, 50.0f), RndF(-5.0f, 50.0f), RndF(-50.0f, 50.0f) };
        settings[0x58] = Rnd() % 4 != 0;
        uint32_t sun = Rnd() % 4 ? 1 : 0;
        *(float*)(flare + 0x10) = RndF(-1.0f, 1.0f);
        *(float*)(flare + 0x14) = RndF(-1.0f, 1.0f);
        flare[0xd8] = (uint8_t)(Rnd() % 2 ? Rnd() % 0x100 : 0x10);
        for (int i = 0; i < 12; ++i) {
            *(uint32_t*)(flare + i * 16 + 0x24) = Rnd();
            *(float*)(flare + i * 16 + 0x18) = RndF(-1.0f, 1.0f);
            *(float*)(flare + i * 16 + 0x1c) = RndF(0.0f, 0.1f);
            *(int32_t*)(flare + (i + 2) * 16) = (int32_t)(Rnd() % 8);
        }
        int n = (int)(Rnd() % 5);
        int marks[4];
        bool inBlock[4];
        for (int k = 0; k < 4; ++k) {
            marks[k] = Rnd() % 2 ? -1 : (int)(Rnd() % 3);
            inBlock[k] = Rnd() % 2;
        }
        bool poolOn = Rnd() % 2;
        uint32_t out[2][24];
        for (int side = 0; side < 2; ++side) {
            memset(out[side], 0, sizeof out[side]);
            uint8_t* s = scene[side];
            memset(s, 0, sizeof scene[side]);
            *(uint8_t**)(s + 0x2d) = rnd;
            *(uint8_t**)(s + 0x29) = settings;
            *(uint32_t*)(s + 0x99) = sun;
            *(uint8_t**)(s + 0x3665) = flare;
            *(float*)(s + 0x369c) = eye[0];
            *(float*)(s + 0x36a0) = eye[1];
            *(float*)(s + 0x36a4) = eye[2];
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            depth = 0.01f;
            if (op == 0) {
                float res[3] = { -7.0f, -7.0f, -7.0f };
                out[side][0] = side ? rt3::Scene_RayToHeight(s, 0, obj, sx, sy, h, res)
                                    : ((uint8_t(__fastcall*)(uint8_t*, int, uint8_t*, float, float, float, float*))0x524070)(s, 0, obj, sx, sy, h, res);
                memcpy(&out[side][1], res, 12);
            } else if (op == 1) {
                side ? rt3::Scene_ComputeFootprint(s) : ((void(__fastcall*)(uint8_t*))0x5241e0)(s);
                memcpy(&out[side][0], s + 0x36e8, 0x30);
            } else if (op == 2) {
                side ? rt3::Scene_UpdateLensFlare(s) : ((void(__fastcall*)(uint8_t*))0x524530)(s);
                int32_t cnt = *(int32_t*)(fe.e + 0x58);
                out[side][0] = (uint32_t)cnt;
                for (int k = 0; k < cnt && k < 12; ++k)
                    for (int b = 0; b < 0x5c; b += 4) out[side][1 + k] = out[side][1 + k] * 31 + *(uint32_t*)(fe.e + 0x5c + k * 0x5e + b);
            } else {
                // Outgoing list: n nodes, some in the pool's block, some on the heap.
                static uint32_t list[2][3], freeL[2][3];
                static uint8_t pool[2][0x10];
                memset(list[side], 0, 12);
                memset(freeL[side], 0, 12);
                memset(pool[side], 0, 0x10);
                *(uint32_t**)pool[side] = freeL[side];
                *(uint32_t*)(pool[side] + 4) = (uint32_t)(uintptr_t)block[side][0];
                *(uint32_t*)(pool[side] + 8) = (uint32_t)(uintptr_t)block[side][0] + sizeof block[side];
                *(int32_t*)(pool[side] + 0xc) = poolOn;
                uint8_t* nodes[4];
                for (int k = 0; k < n; ++k) {
                    nodes[k] = inBlock[k] && poolOn ? block[side][k] : (uint8_t*)xnew(0x120); // block nodes are only ever handed back to an active pool
                    memset(nodes[k], 0, 0x120);
                    *(int32_t*)(nodes[k] + 0x118) = marks[k];
                    *(uint32_t*)(nodes[k] + 4) = k;
                }
                for (int k = 0; k < n; ++k) *(uint32_t*)nodes[k] = k + 1 < n ? (uint32_t)(uintptr_t)nodes[k + 1] : 0;
                list[side][0] = n ? (uint32_t)(uintptr_t)nodes[0] : 0;
                list[side][1] = n ? (uint32_t)(uintptr_t)nodes[n - 1] : 0;
                list[side][2] = (uint32_t)n;
                hook::Global<uint32_t*>(0x6d4044) = list[side];
                hook::Global<uint8_t*>(0x6d4040) = pool[side];
                hook::Global<int32_t>(0x6d4060) = 0;
                side ? rt3::Net_PurgeSentMessages(0) : ((void(__stdcall*)(int32_t))0x522430)(0);
                auto id = [&](uint32_t p) -> uint32_t { return p ? *(uint32_t*)((uint8_t*)(uintptr_t)p + 4) + 1 : 0; };
                out[side][0] = id(list[side][0]);
                out[side][1] = id(list[side][1]);
                out[side][2] = list[side][2];
                out[side][3] = id(freeL[side][0]);
                out[side][4] = id(freeL[side][1]);
                out[side][5] = freeL[side][2];
                int k = 6;
                for (uint32_t p = list[side][0]; p && k < 12; p = *(uint32_t*)(uintptr_t)p) out[side][k++] = id(p);
                for (uint32_t p = freeL[side][0]; p && k < 20; p = *(uint32_t*)(uintptr_t)p) out[side][k++] = id(p) + 0x100;
                out[side][21] = hook::Global<uint32_t>(0x6d4060);
                for (uint32_t p = list[side][0]; p;) {
                    uint32_t next = *(uint32_t*)(uintptr_t)p;
                    uint8_t* q = (uint8_t*)(uintptr_t)p;
                    if (q < block[side][0] || q >= block[side][0] + sizeof block[side]) ((void(__cdecl*)(void*))0x53b080)(q);
                    p = next;
                }
            }
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    DeleteCriticalSection(&cs);
    cs = savedCs;
    for (PtrList& l : lists) PtrList_Clear(&l);
    depth = savedDepth;
}

namespace rt3 {
void __fastcall ChangeTrees_Destruct(uint8_t* w);
void __fastcall ChangeHeight_Destruct(uint8_t* w);
void __fastcall StationList_Destruct(uint8_t* w);
void __fastcall CompanyList_Destruct(uint8_t* w);
void __fastcall MapWidget_Destruct(uint8_t* w);
void __fastcall ModelSet_Clear(uint8_t* o);
void __fastcall TrainDetail_Destruct(uint8_t* w);
void __fastcall SettingsWindow_Destruct(uint8_t* w);
void __fastcall HighScores_Destruct(uint8_t* w);
void __fastcall Setup_Destruct(uint8_t* w);
void __fastcall Credits_Destruct(uint8_t* w);
void __fastcall StationDetail_Destruct(uint8_t* w);
void __fastcall StockBuy_Destruct(uint8_t* w);
void __fastcall Newspaper_DrawRules(uint8_t* w);
}

DIFFTEST(game_ToolWindowDestructors) {
    FakeEngine fe;
    auto xnew = (void*(__cdecl*)(size_t))0x53b070;
    uint8_t* pack = (uint8_t*)VirtualAlloc(nullptr, 0x43000, MEM_COMMIT, PAGE_READWRITE);
    const uint32_t gl[] = { 0x6d4020, 0x62c120, 0x6cfe18, 0x6d1704, 0x6d170c, 0x6d1708, 0x6cfe70, 0x6cfe74, 0x6cfe78, 0xccbb2c, 0x6d401c,
                            0x6d3b14, 0x6d3b18, 0x6d3b1c, 0x6d3b20, 0x6d1354, 0x6d1358, 0x6d1084, 0x6d107c, 0x6d1080, 0x6d1094,
                            0x6d14b0, 0x6d14b4, 0x6d14b8, 0x6d14bc, 0x6d14c0, 0x6cfe8c, 0x6cfe90, 0x6cfe94, 0x6cfe98, 0x6cfe9c, 0x6cfea0,
                            0x5ee4fc, 0x5ee500, 0x62be84, 0x6d16d8, 0x6d16dc, 0x6d16e0, 0x6d16e4, 0x6d16e8, 0x6d16ec, 0x6d19d8, 0x6d19dc,
                            0x6d19e0, 0x6d19e4, 0x6d19e8, 0x6d19ec, 0x6d19f0, 0x6cea4c };
    static PtrList lists[3];
    for (int k = 0; k < 3; ++k) PtrList_Construct(&lists[k], 0, 0, 0);
    float& depthG = hook::Global<float>(0xd93850);
    float savedDepthG = depthG;
    static uint8_t modes[0x15];
    *(int32_t*)modes = 1024;
    *(int32_t*)(modes + 4) = 768;
    static Collection portraits;
    memset(&portraits, 0, sizeof portraits);
    Collection_Construct(&portraits, 0, 1, 0x40, 4, 4, 0, 0, 0);
    { static uint8_t blank[0x40]; for (int k = 0; k < 3; ++k) Collection_Allocate(&portraits, 0, blank, 0x40, 0, 0); }
    static uint8_t coWin[2][0x100], edition[0x10];
    static void* coVtbl[4] = { nullptr, (void*)&FakeLoggingBase };
    const int G = sizeof gl / sizeof gl[0];
    uint32_t saved[G];
    for (int k = 0; k < G; ++k) saved[k] = hook::Global<uint32_t>(gl[k]);
    hook::Global<uint8_t*>(0x6d4020) = pack;
    hook::Global<uint32_t>(0xccbb2c) = 0;
    static uint8_t scene[0x3800], map[0x2200], table[4 * 0xd], objs[4][0x10], w[2][0x180];
    *(uint8_t**)(fe.e + 0x18) = scene;
    hook::Global<uint8_t*>(0x62c120) = map;
    static uint8_t mgr[0xd00];
    hook::Global<uint8_t*>(0x6d401c) = mgr;
    for (int it = 0; it < 4000; ++it) {
        int op = (int)(Rnd() % 14);
        int32_t openCount = (int32_t)(Rnd() % 2), fog = Rnd() % 2 ? 1 : 0, fogKind = Rnd() % 3 == 0 ? 0x15 : (int32_t)(Rnd() % 3);
        bool news = Rnd() % 4 != 0;
        int32_t seed = (int32_t)Rnd();
        float rx = RndF(0.0f, 1.0f), ry = RndF(0.0f, 1.0f), rh = RndF(0.0f, 600.0f);
        bool haveBuf = Rnd() % 2;
        int imgs[31];
        for (int& v : imgs) v = (int)(Rnd() % 5) - 1;
        int32_t modal = (int32_t)(Rnd() % 3);
        bool haveColl = Rnd() % 2;
        int32_t busy = (int32_t)(Rnd() % 3);
        int32_t fogMode = Rnd() % 2 ? 1 : 0, fogOn = (int32_t)(Rnd() % 2), fogActive = (int32_t)(Rnd() % 2);
        uint32_t refs[4];
        for (uint32_t& r : refs) r = 1 + Rnd() % 3;
        int pa = (int)(Rnd() % 5) - 1, pb = (int)(Rnd() % 5) - 1;
        bool haveImg = Rnd() % 2, haveTex = Rnd() % 2, haveList = Rnd() % 4 != 0;
        uint32_t out[2][16];
        for (int side = 0; side < 2; ++side) {
            memset(out[side], 0, sizeof out[side]);
            *(int32_t*)(scene + 0x3741) = busy;
            *(int32_t*)(map + 0x2171) = fogMode;
            *(int32_t*)(map + 0x219d) = fogOn;
            *(int32_t*)(map + 0x21a1) = fogActive;
            *(int32_t*)(fe.e + 0x114272) = 5;
            *(int32_t*)(fe.e + 0x114266) = 7;
            memset(table, 0, sizeof table);
            for (int k = 0; k < 4; ++k) {
                *(uint32_t*)(table + k * 0xd) = refs[k] + 40; // up to 35 releases never free the (static) object
                *(uint32_t*)(table + k * 0xd + 4) = 0x60 + k;
                table[k * 0xd + 8] = 6;
                *(uint8_t**)(table + k * 0xd + 9) = objs[k];
            }
            *(uint8_t**)(pack + 0x1f400) = table;
            *(int32_t*)(pack + 0x427b4) = 4;
            void* a = pa >= 0 ? (void*)objs[pa] : nullptr;
            void* b = pb >= 0 ? (void*)objs[pb] : nullptr;
            hook::Global<void*>(0x6d1704) = a;
            hook::Global<void*>(0x6d170c) = b;
            hook::Global<void*>(0x6cfe74) = a;
            hook::Global<void*>(0x6cfe78) = b;
            hook::Global<void*>(0x6d3b14) = a;
            hook::Global<void*>(0x6d3b18) = b;
            hook::Global<void*>(0x6d3b1c) = nullptr;
            hook::Global<int32_t>(0x6d3b20) = 9;
            hook::Global<void*>(0x6d1358) = a;
            hook::Global<void*>(0x6d1084) = a;
            hook::Global<void*>(0x6d107c) = b;
            hook::Global<void*>(0x6d1080) = nullptr;
            hook::Global<void*>(0x6d1094) = nullptr;
            hook::Global<uint8_t>(0x6d1098) = 9;
            hook::Global<void*>(0x6d14b0) = a;
            hook::Global<void*>(0x6d14b4) = b;
            hook::Global<void*>(0x6d14b8) = nullptr;
            hook::Global<void*>(0x6d14bc) = haveBuf ? ((void*(__cdecl*)(size_t))0x5a125d)(16) : nullptr;
            hook::Global<int32_t>(0x6d14c0) = 9;
            hook::Global<void*>(0x6cfe90) = a;
            hook::Global<void*>(0x6cfe94) = nullptr;
            hook::Global<void*>(0x6cfe98) = b;
            hook::Global<void*>(0x6cfe9c) = nullptr;
            hook::Global<int32_t>(0x6cfe8c) = 9;
            hook::Global<void*>(0x6cfea0) = nullptr;
            hook::Global<uint8_t>(0x6cfea4) = 9;
            hook::Global<int32_t>(0x5ee4fc) = 3;
            hook::Global<int32_t>(0x5ee500) = 4;
            hook::Global<int32_t>(0x62be84) = 5;
            hook::Global<int32_t>(0x6d16d8) = 9;
            for (uint32_t g : { 0x6d16dcu, 0x6d16e4u, 0x6d16ecu, 0x6d19d8u, 0x6d19e0u, 0x6d19e8u }) hook::Global<void*>(g) = a;
            for (uint32_t g : { 0x6d16e0u, 0x6d16e8u, 0x6d19dcu, 0x6d19e4u, 0x6d19ecu }) hook::Global<void*>(g) = b;
            *(int32_t*)(map + 0x2175) = fogKind;
            memset(coWin[side], 0, sizeof coWin[side]);
            *(void***)coWin[side] = coVtbl;
            hook::Global<uint8_t*>(0x6d19f0) = coWin[side];
            *(Collection**)(pack + 0x429b4) = &portraits;
            for (int k = 1; k <= 3; ++k) *(uint32_t*)((uint8_t*)Collection_Get(&portraits, 0, k) + 0x20) = 0;
            hook::Global<uint32_t>(0x6cea4c) = news;
            *(uint8_t**)(fe.e + 0x34) = modes;
            *(PtrList**)(fe.e + 0x11369d) = &lists[0];
            *(PtrList**)(fe.e + 0x1136a1) = &lists[1];
            *(PtrList**)(fe.e + 0x1136a5) = &lists[2];
            *(int32_t*)(fe.e + 0x58) = 0;
            for (PtrList& l : lists) { PtrList_Clear(&l); l.count = 0; }
            depthG = 0.01f;
            *(int32_t*)(mgr + 0xc60) = modal;
            hook::Global<Collection*>(0x6d1354) = nullptr;
            if (haveColl) {
                Collection* c = (Collection*)xnew(sizeof(Collection));
                memset(c, 0, sizeof(Collection));
                Collection_Construct(c, 0, 1, 0x20, 4, 4, 0, 0, 0);
                hook::Global<Collection*>(0x6d1354) = c;
            }
            hook::Global<int32_t>(0x6d1708) = 9;
            hook::Global<int32_t>(0x6cfe70) = 9;
            hook::Global<int32_t>(0x6cfe18) = 9;
            uint8_t* wd = w[side];
            memset(wd, 0, sizeof w[side]);
            typedef void(__fastcall * Fn)(uint8_t*);
            if (op == 0) side ? rt3::ChangeTrees_Destruct(wd) : ((Fn)0x4bcbf0)(wd);
            else if (op == 1) side ? rt3::ChangeHeight_Destruct(wd) : ((Fn)0x4bc1d0)(wd);
            else if (op == 2) side ? rt3::StationList_Destruct(wd) : ((Fn)0x506a80)(wd);
            else if (op == 3) side ? rt3::CompanyList_Destruct(wd) : ((Fn)0x4c6ae0)(wd);
            else if (op == 6) side ? rt3::TrainDetail_Destruct(wd) : ((Fn)0x512f10)(wd);
            else if (op == 7) side ? rt3::SettingsWindow_Destruct(wd) : ((Fn)0x4fe450)(wd);
            else if (op == 8) side ? rt3::HighScores_Destruct(wd) : ((Fn)0x4e1a70)(wd);
            else if (op == 9) side ? rt3::Setup_Destruct(wd) : ((Fn)0x502720)(wd);
            else if (op == 11) {
                *(int32_t*)(wd + 0x7c) = openCount;
                *(int32_t*)(map + 0x2171) = fog;
                side ? rt3::StationDetail_Destruct(wd) : ((Fn)0x505bf0)(wd);
            } else if (op == 12) {
                side ? rt3::StockBuy_Destruct(wd) : ((Fn)0x50bac0)(wd);
            } else if (op == 13) {
                *(int32_t*)(edition + 8) = seed;
                *(uint8_t**)(wd + 0x78) = edition;
                *(float*)(wd + 5) = rx;
                *(float*)(wd + 9) = ry;
                *(float*)(wd + 0x7c) = rh;
                side ? rt3::Newspaper_DrawRules(wd) : ((Fn)0x4f3750)(wd);
            }
            else if (op == 10) {
                for (int k = 0; k < 31; ++k) *(void**)(wd + 0x78 + k * 8) = imgs[k] >= 0 ? (void*)objs[imgs[k]] : nullptr;
                side ? rt3::Credits_Destruct(wd) : ((Fn)0x4c7bc0)(wd);
            }
            else if (op == 4) {
                *(void**)(wd + 0xaf) = haveImg ? (void*)objs[2] : nullptr;
                if (haveTex) {
                    uint8_t* t = (uint8_t*)xnew(0x100);
                    memset(t, 0, 0x100);
                    *(uint8_t**)(wd + 0xab) = t;
                }
                side ? rt3::MapWidget_Destruct(wd) : ((Fn)0x47b110)(wd);
            } else {
                if (haveList) {
                    PtrList* l = (PtrList*)xnew(sizeof(PtrList));
                    PtrList_Construct(l, 0, 0, 0);
                    *(PtrList**)(wd + 0x79) = l;
                }
                side ? rt3::ModelSet_Clear(wd) : ((Fn)0x52eeb0)(wd);
            }
            memcpy(&out[side][0], wd, 0x20);
            out[side][8] = *(uint32_t*)(wd + 0x79) + *(uint32_t*)(wd + 0xab) * 3 + *(uint32_t*)(wd + 0xaf) * 5;
            out[side][9] = *(uint32_t*)(scene + 0x3741);
            out[side][10] = *(uint32_t*)(map + 0x219d) + *(uint32_t*)(fe.e + 0x114272) * 16 + *(uint32_t*)(fe.e + 0x114266) * 256;
            for (int k = 0; k < 4; ++k) out[side][11] = out[side][11] * 7 + *(uint32_t*)(table + k * 0xd);
            out[side][12] = hook::Global<uint32_t>(0x6d1708) + hook::Global<uint32_t>(0x6cfe70) * 16 + hook::Global<uint32_t>(0x6cfe18) * 256;
            out[side][13] = (hook::Global<uint32_t>(0x6cfe74) != 0) + (hook::Global<uint32_t>(0x6cfe78) != 0) * 2;
            out[side][15] = (hook::Global<uint32_t>(0x6d14b0) != 0) + (hook::Global<uint32_t>(0x6d14b4) != 0) * 2 + (hook::Global<uint32_t>(0x6d14bc) != 0) * 4 +
                            hook::Global<uint32_t>(0x6d14c0) * 8 + hook::Global<uint32_t>(0x6cfe8c) * 256 + hook::Global<uint8_t>(0x6cfea4) * 4096;
            if (void* buf = hook::Global<void*>(0x6d14bc)) {
                ((void(__cdecl*)(void*))0x5a1145)(buf);
                hook::Global<void*>(0x6d14bc) = nullptr;
            }
            {
                int32_t cnt = *(int32_t*)(fe.e + 0x58);
                uint32_t hsh = (uint32_t)cnt;
                for (int k = 0; k < cnt && k < 8; ++k)
                    for (int bb = 0; bb < 0x5c; bb += 4) hsh = hsh * 31 + *(uint32_t*)(fe.e + 0x5c + k * 0x5e + bb);
                out[side][7] = hsh + hook::Global<uint32_t>(0x5ee4fc) + hook::Global<uint32_t>(0x5ee500) * 3 + hook::Global<uint32_t>(0x62be84) * 7 +
                               hook::Global<uint32_t>(0x6d16d8) * 11 + (hook::Global<uint32_t>(0x6d19f0) != 0) * 13 + *(uint32_t*)(map + 0x219d) * 17;
            }
            out[side][14] = hook::Global<uint32_t>(0x6d3b20) + hook::Global<uint8_t>(0x6d1098) * 16 + (hook::Global<uint32_t>(0x6d1354) != 0) * 256 +
                            *(uint32_t*)(mgr + 0xc60) * 4096;
            if (Collection* c = hook::Global<Collection*>(0x6d1354)) {
                Collection_Destruct(c);
                ((void(__cdecl*)(void*))0x53b080)(c);
                hook::Global<Collection*>(0x6d1354) = nullptr;
            }
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "op %d iteration %d at %d", op, it, FirstDiff(out[0], out[1], sizeof out[0]) / 4);
    }
    for (int k = 0; k < G; ++k) hook::Global<uint32_t>(gl[k]) = saved[k];
    for (PtrList& l : lists) PtrList_Clear(&l);
    depthG = savedDepthG;
    VirtualFree(pack, 0, MEM_RELEASE);
}

namespace rt3 {
float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom);
}

namespace {
int g_camUpdates;
void __fastcall FakeCameraUpdate(uint8_t*) { ++g_camUpdates; }
}

DIFFTEST(game_CameraPitchForZoom) {
    FakeEngine fe;
    static void* vtbl[8] = { nullptr, nullptr, nullptr, (void*)&FakeCameraUpdate };
    static uint8_t cam[0x40], options[0x400];
    *(void***)cam = vtbl;
    uint8_t* savedOpt = hook::Global<uint8_t*>(0x6cec74);
    hook::Global<uint8_t*>(0x6cec74) = options;
    for (int it = 0; it < 20000; ++it) {
        int32_t mode = (int32_t)(Rnd() % 4);
        *(int32_t*)(options + 0x10c) = mode;
        fe.e[0x11422e] = (uint8_t)(Rnd() % 2);
        float zoom = Rnd() % 4 ? RndF(0.0f, 20000.0f) : RndF(0.0f, 60.0f);
        float pitch = RndF(-1.0f, 2.0f);
        uint32_t out[2][3];
        for (int side = 0; side < 2; ++side) {
            *(float*)(cam + 0x14) = pitch;
            g_camUpdates = 0;
            float r = side ? rt3::Camera_PitchForZoom(cam, 0, zoom) : ((float(__fastcall*)(uint8_t*, int, float))0x43a750)(cam, 0, zoom);
            memcpy(&out[side][0], &r, 4);
            out[side][1] = (uint32_t)g_camUpdates;
            memcpy(&out[side][2], cam + 0x14, 4);
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "mode %d flag %d iteration %d zoom %g pitch %g: %08x vs %08x upd %u/%u", mode, fe.e[0x11422e], it, zoom, pitch, out[0][0], out[1][0], out[0][1], out[1][1]);
    }
    hook::Global<uint8_t*>(0x6cec74) = savedOpt;
}

namespace rt3 {
void __fastcall Placement_RotateSteps(void*, int, int32_t steps);
void __fastcall Placement_RotateToMouse(uint8_t* w);
}

DIFFTEST(game_PlacementRotate) {
    static uint8_t panel[0x100], input[0xb00], cam[0x40];
    uint32_t saved[5] = { hook::Global<uint32_t>(0x62b26c), hook::Global<uint32_t>(0x6d4018), hook::Global<uint32_t>(0x62be68),
                          hook::Global<uint32_t>(0x6d1720), hook::Global<uint32_t>(0x6d172c) };
    uint32_t savedKeep = hook::Global<uint32_t>(0x6d1730);
    hook::Global<void*>(0x62b26c) = nullptr;
    hook::Global<void*>(0x6d4018) = input;
    hook::Global<void*>(0x62be68) = cam;
    hook::Global<void*>(0x6d1720) = panel;
    for (int it = 0; it < 20000; ++it) {
        float rot = RndF(-10.0f, 10.0f);
        int32_t steps = (int32_t)(Rnd() % 64) - 32;
        *(float*)(input + 0xa94) = Rnd() % 8 ? RndF(0.0f, 1.0f) : 0.926875f;
        *(float*)(input + 0xa98) = Rnd() % 8 ? RndF(0.0f, 1.0f) : 0.7825f;
        *(float*)(cam + 0x10) = RndF(-7.0f, 7.0f);
        uint32_t out[2][4];
        for (int side = 0; side < 2; ++side) {
            hook::Global<float>(0x6d172c) = rot;
            hook::Global<uint32_t>(0x6d1730) = 0;
            if (side) rt3::Placement_RotateSteps(nullptr, 0, steps);
            else ((void(__fastcall*)(void*, int, int32_t))0x5086c0)(nullptr, 0, steps);
            out[side][0] = hook::Global<uint32_t>(0x6d172c);
            out[side][1] = hook::Global<uint32_t>(0x6d1730);
            hook::Global<float>(0x6d172c) = rot;
            if (side) rt3::Placement_RotateToMouse(panel);
            else ((void(__fastcall*)(uint8_t*))0x507d80)(panel);
            out[side][2] = hook::Global<uint32_t>(0x6d172c);
            out[side][3] = hook::Global<uint32_t>(0x6d1730);
        }
        EXPECT_EQ(memcmp(out[0], out[1], sizeof out[0]), 0, "iteration %d: %08x %08x %08x vs %08x %08x %08x", it, out[0][0], out[0][2], out[0][3],
                  out[1][0], out[1][2], out[1][3]);
    }
    hook::Global<uint32_t>(0x62b26c) = saved[0];
    hook::Global<uint32_t>(0x6d4018) = saved[1];
    hook::Global<uint32_t>(0x62be68) = saved[2];
    hook::Global<uint32_t>(0x6d1720) = saved[3];
    hook::Global<uint32_t>(0x6d172c) = saved[4];
    hook::Global<uint32_t>(0x6d1730) = savedKeep;
}

namespace rt3 {
int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company,
                                  uint32_t* foreign, int32_t anyTrack);
}

// Track networks with multi-node routes: chains of 1..4 nodes per route, route
// ends joined to other route ends through free link slots.
DIFFTEST(game_RouteNextStep) {
    const int R = 8, MAXN = R * 4;  // routes of 2..4 nodes, every route end paired (no cycles, no dead ends)
    static Collection nodes, routes;
    memset(&nodes, 0, sizeof nodes);
    memset(&routes, 0, sizeof routes);
    Collection_Construct(&nodes, 0, 1, 0x260, 0x20, 0x10, 0, 0, 0);
    Collection_Construct(&routes, 0, 1, 0x40, 0x10, 0x10, 0, 0, 0);
    static uint8_t blank[0x260];
    for (int k = 0; k < MAXN; ++k) Collection_Allocate(&nodes, 0, blank, 0x260, 0, 0);
    for (int k = 0; k < R; ++k) Collection_Allocate(&routes, 0, blank, 0x40, 0, 0);
    static uint8_t companyMem[0x1d9f + 4 * 0x38d];
    uint32_t* company = (uint32_t*)companyMem;
    struct Save {
        uint32_t addr, value;
    } saves[] = { { 0x6cfca8 }, { 0x6cfcac }, { 0x6cfcb0 }, { 0x6cfcb4 } };
    for (Save& sv : saves) sv.value = hook::Global<uint32_t>(sv.addr);
    hook::Global<Collection*>(0x6cfca8) = &nodes;
    hook::Global<Collection*>(0x6cfcb4) = &routes;
    int found = 0, multi = 0;
    for (int it = 0; it < 8000; ++it) {
        for (int k = 1; k <= MAXN; ++k) {
            uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, k);
            *(int32_t*)(n + 0x202) = k;
            *(int32_t*)(n + 0x212) = 0x7fff;
            for (int j = 0; j < 3; ++j) *(int32_t*)(n + 0x206 + j * 4) = -1;
            n[0x216] = (uint8_t)(Rnd() % 8 ? 2 : 1);
            n[0x44] = (uint8_t)(Rnd() % 6 != 0);
            *(uint16_t*)(n + 0x22e) = (uint16_t)(Rnd() % 3);
            for (int o = 0x14; o <= 0x34; o += (o == 0x1c ? 0x10 : 8)) *(float*)(n + o) = (float)(Rnd() % 40) * 50.0f;
        }
        auto link = [&](int a, int b) {
            uint8_t* na = (uint8_t*)Collection_Get(&nodes, 0, a);
            uint8_t* nb = (uint8_t*)Collection_Get(&nodes, 0, b);
            int sa = *(int32_t*)(na + 0x206) == -1 ? 0 : 1, sb = *(int32_t*)(nb + 0x206) == -1 ? 0 : 1;
            *(int32_t*)(na + 0x206 + sa * 4) = b;
            *(int32_t*)(nb + 0x206 + sb * 4) = a;
        };
        int next = 1, ends[R * 2], nEnds = 0;
        for (int r = 1; r <= R; ++r) {
            int len = 2 + (int)(Rnd() % 3);
            uint8_t* rt = (uint8_t*)Collection_Get(&routes, 0, r);
            *(int32_t*)(rt + 4) = next;
            *(int32_t*)(rt + 8) = next + len - 1;
            *(int32_t*)(rt + 0x10) = len;
            *(uint32_t*)(rt + 0x14) = (uint32_t)len + Rnd() % 20;
            *(int32_t*)(rt + 0x18) = (int32_t)(Rnd() % 4);
            rt[0x1c] = (uint8_t)(Rnd() % 5 != 0);
            *(uint32_t*)(rt + 0x1e) = Rnd() % 3;
            for (int k = 0; k < len; ++k) {
                uint8_t* n = (uint8_t*)Collection_Get(&nodes, 0, next + k);
                *(int32_t*)(n + 0x212) = r;
                if (k) {
                    // inner chain links in the first two slots
                    uint8_t* p = (uint8_t*)Collection_Get(&nodes, 0, next + k - 1);
                    int sp = *(int32_t*)(p + 0x206) == -1 && Rnd() % 2 ? 0 : (*(int32_t*)(p + 0x20a) == -1 ? 1 : 0);
                    int sn = *(int32_t*)(n + 0x206) == -1 && Rnd() % 2 ? 0 : (*(int32_t*)(n + 0x20a) == -1 ? 1 : 0);
                    *(int32_t*)(p + 0x206 + sp * 4) = next + k;
                    *(int32_t*)(n + 0x206 + sn * 4) = next + k - 1;
                }
            }
            ends[nEnds++] = next;
            if (len > 1) ends[nEnds++] = next + len - 1;
            next += len;
        }
        for (int k = nEnds - 1; k > 0; --k) {
            int j = (int)(Rnd() % (k + 1)), t = ends[k];
            ends[k] = ends[j];
            ends[j] = t;
        }
        for (int k = 0; k + 1 < nEnds; k += 2)
            link(ends[k], ends[k + 1]);
        memset(companyMem, 0, sizeof companyMem);
        *company = Rnd() % 3;
        for (int k = 0; k < 4; ++k) companyMem[0x1d9f + k * 0x38d] = (uint8_t)(Rnd() % 4 != 0);
        int nNodes = next - 1;
        int32_t from = Rnd() % 20 == 0 ? -1 : 1 + (int32_t)(Rnd() % nNodes), to = 1 + (int32_t)(Rnd() % nNodes);
        int32_t needFlag = Rnd() % 4 == 0, anyTrack = Rnd() % 3 == 0;
        uint32_t* co = Rnd() % 2 ? company : nullptr;
        bool useForeign = Rnd() % 4 != 0;
        uint32_t costA = Rnd(), costB = costA, forA = Rnd(), forB = forA;
        int32_t ra = ((int32_t(__fastcall*)(Collection*, int, int32_t, int32_t, uint32_t*, int32_t, uint32_t*, uint32_t*, int32_t))0x4a6630)(
            &routes, 0, from, to, &costA, needFlag, co, useForeign ? &forA : nullptr, anyTrack);
        uint32_t boundA = hook::Global<uint32_t>(0x6cfcb0);
        int32_t rb = rt3::Route_NextStep(&routes, 0, from, to, &costB, needFlag, co, useForeign ? &forB : nullptr, anyTrack);
        found += ra != -1 && from != to;
        multi += ra != -1 && costA > 1;
        EXPECT_EQ(ra, rb, "next step iteration %d", it);
        EXPECT_EQ(costA, costB, "next step cost iteration %d", it);
        EXPECT_EQ(forA, forB, "next step foreign iteration %d", it);
        EXPECT_EQ(boundA, hook::Global<uint32_t>(0x6cfcb0), "next step bound iteration %d", it);
    }
    for (Save& sv : saves) hook::Global<uint32_t>(sv.addr) = sv.value;
    Collection_Destruct(&nodes);
    Collection_Destruct(&routes);
    EXPECT_EQ(found > 1500 && multi > 500, true, "next step coverage: %d found, %d multi-step", found, multi);
}
