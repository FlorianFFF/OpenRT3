// Functions the runs cannot reach, original against rewrite on synthetic data:
// the map editor's flood fill, an edit box's paste (fake clipboard), the open
// file dialog (fake comdlg / directory imports), D3DX font text (a fake font),
// a model's tint rectangle, the sound cache's timed queue and the credits
// ticker. Calls they make are recorded or faked at the exe address and the
// rewrite's own entry, so both sides see the same.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace {

template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } entries[32];
    int count = 0;
    void AddOne(uintptr_t addr, void* target) {
        Patch& p = entries[count++];
        p.at = (uint8_t*)addr;
        DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5);
        p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    void Add(uintptr_t addr, void* target) {
        AddOne(addr, target);
        if (void* r = hook::Find((uint32_t)addr)) AddOne((uintptr_t)r, target);
    }
    ~Redirects() {
        while (count) {
            Patch& p = entries[--count];
            DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};

struct Slots {
    struct S { void** at; void* old; } s[16];
    int n = 0;
    void Set(uint32_t slot, void* fake) {
        DWORD o;
        VirtualProtect((void*)slot, 4, PAGE_READWRITE, &o);
        s[n++] = { (void**)slot, *(void**)slot };
        *(void**)slot = fake;
    }
    ~Slots() {
        while (n) {
            --n;
            *s[n].at = s[n].old;
        }
    }
};

char g_log[4096];
int g_logLen;
void Log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (g_logLen < (int)sizeof g_log - 300) g_logLen += vsprintf(g_log + g_logLen, fmt, ap);
    va_end(ap);
}
void ClearLog() { g_logLen = 0; g_log[0] = 0; }

uint32_t Rnd() {
    static uint32_t s = 0x2468ace1;
    s = s * 1103515245u + 12345u;
    return s >> 8;
}

// --- clipboard (EditBox_Paste reads it through Clipboard_GetText's imports) ---
char g_clip[0x1400];
int32_t g_clipOpen;
int32_t __stdcall FakeOpenClipboard(void*) { Log("Open;"); return g_clipOpen; }
int32_t __stdcall FakeCloseClipboard() { Log("Close;"); return 1; }
void* __stdcall FakeGetClipboardData(uint32_t fmt) { Log("Get %u;", fmt); return (void*)0x5678; }
void* __stdcall FakeGlobalLock(void* h) { return h == (void*)0x5678 ? (void*)g_clip : nullptr; }
int32_t __stdcall FakeGlobalUnlock(void*) { return 1; }
void __fastcall RecInsertCursor(uint8_t* w, int, char* dst, int32_t pos) { Log("Insert %d [%s] %d;", (int)(w != nullptr), dst, pos); }

// --- the open file dialog ---
uint32_t __stdcall FakeGetCurrentDirectory(uint32_t n, char* buf) { Log("GetDir %u;", n); strcpy(buf, "C:\\Game"); return 7; }
int32_t __stdcall FakeSetCurrentDirectory(const char* d) { Log("SetDir [%s];", d); return 1; }
int32_t g_pick;
int32_t __stdcall FakeGetOpenFileName(uint32_t* ofn) {
    Log("Open size %x owner %x filter [%s] custom %x %x %x file %d max %x title %x dir [%s] caption %x flags %x;", ofn[0], ofn[1],
        (const char*)ofn[3], ofn[4], ofn[5], ofn[6], ofn[7] != 0, ofn[8], ofn[10], ofn[11] ? (const char*)ofn[11] : "(null)", ofn[12],
        ofn[13]);
    if (g_pick) strcpy((char*)ofn[7], "C:\\Game\\maps\\Picked.gmp");
    return g_pick;
}

// --- a D3DX font (vtable slots 0x14 Begin, 0x18 DrawTextA, 0x20 End) ---
int32_t __stdcall FontBegin(void*) { Log("Begin;"); return 0; }
int32_t __stdcall FontDraw(void*, const char* text, int32_t n, int32_t* rc, uint32_t fmt, uint32_t colour) {
    Log("Draw [%s] %d %d %d %d %d %x %x;", text, n, rc[0], rc[1], rc[2], rc[3], fmt, colour);
    return 0;
}
int32_t __stdcall FontEnd(void*) { Log("End;"); return 0; }

// --- the timed queue's pieces ---
uint8_t* g_popped;
void* __fastcall FakeHeapPopMax(void*) { Log("PopMax;"); return g_popped; }
void __cdecl RecFree(void* p) { Log("Free %d;", (int)(p != nullptr)); }
uint32_t __fastcall RecHashRemove(void* h, int, uint32_t key) { Log("Remove %x %x;", (uint32_t)(uintptr_t)h, key); return 0; }
void __cdecl RecDelete(void* p) { Log("Delete %d;", (int)(p == g_popped)); }

// --- the ticker's widths (Table_Entry, Slider_StepB) ---
uint32_t __fastcall FakeTableEntry(uint8_t* t, int, int32_t idx) { return (uint32_t)(uintptr_t)t + idx; }
double __fastcall FakeStepB(uint8_t* s) { return ((uint32_t)(uintptr_t)s & 0xff) / 512.0; }

}  // namespace

DIFFTEST(misc_MapFloodFill) {
    static uint8_t map[0x2200];
    uint8_t*& m = hook::Global<uint8_t*>(0x62c120);
    uint8_t* keep = m;
    m = map;
    for (int c = 0; c < 300; ++c) {
        const int w = 2 + Rnd() % 30, h = 2 + Rnd() % 30;
        F<int32_t>(map, 0x2155) = w;
        F<int32_t>(map, 0x2159) = h;
        static uint8_t a[32 * 32], b[32 * 32];
        const int dens = 30 + Rnd() % 60;
        for (int i = 0; i < w * h; ++i) a[i] = (uint8_t)((int)(Rnd() % 100) < dens ? 1 : Rnd() % 4 == 0 ? 3 : 0);
        memcpy(b, a, sizeof a);
        const int x = Rnd() % w, y = Rnd() % h;
        ((void(__fastcall*)(int32_t, int32_t, uint8_t*))0x4f89b0)(x, y, a);
        ((void(__fastcall*)(int32_t, int32_t, uint8_t*))hook::Find(0x4f89b0))(x, y, b);
        EXPECT_EQ(memcmp(a, b, w * h), 0, "case %d (%dx%d from %d,%d)", c, w, h, x, y);
    }
    m = keep;
}

DIFFTEST(misc_EditBoxPaste) {
    Slots slots;
    slots.Set(0x5c825c, (void*)&FakeOpenClipboard);
    slots.Set(0x5c82d8, (void*)&FakeCloseClipboard);
    slots.Set(0x5c8260, (void*)&FakeGetClipboardData);
    slots.Set(0x5c820c, (void*)&FakeGlobalLock);
    slots.Set(0x5c8214, (void*)&FakeGlobalUnlock);
    Redirects r;
    r.Add(0x55bd80, (void*)&RecInsertCursor);
    const char* clips[] = { "", "x", "Hello", "A much longer text than most boxes take, to be cut at the limit" };
    for (int c = 0; c < 64; ++c) {
        g_clipOpen = c & 1;
        strcpy(g_clip, clips[(c >> 1) & 3]);
        const int32_t limits[] = { 0, 1, 2, 3, 8, 20, 200, 0x2000 };
        const int32_t limit = limits[(c >> 3) & 7];
        char logA[4096], dstA[256], dstB[256], boxA[256], boxB[256];
        uint8_t wa[0x100], wb[0x100];
        for (int side = 0; side < 2; ++side) {
            uint8_t* w = side ? wb : wa;
            char* dst = side ? dstB : dstA;
            memset(w, 0, sizeof wa);
            F<int32_t>(w, 0xaa) = limit;
            F<int32_t>(w, 0xd8) = 3;
            strcpy(dst, "old text");
            ClearLog();
            ((void(__fastcall*)(uint8_t*, int, char*))(side ? hook::Find(0x55be40) : (void*)0x55be40))(w, 0, dst);
            char* box = F<char*>(w, 0xba);
            strcpy(side ? boxB : boxA, box ? box : "(none)");
            F<char*>(w, 0xba) = nullptr;  // (a malloc of the exe's runtime; its address differs per side)
            if (!side) strcpy(logA, g_log);
        }
        EXPECT_EQ(memcmp(wa, wb, sizeof wa), 0, "box %d", c);
        EXPECT_EQ(strcmp(dstA, dstB), 0, "text %d: [%s] / [%s]", c, dstA, dstB);
        EXPECT_EQ(strcmp(boxA, boxB), 0, "copy %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls %d: [%s] / [%s]", c, logA, g_log);
    }
}

DIFFTEST(misc_OpenFileDialog) {
    Slots slots;
    slots.Set(0x5c807c, (void*)&FakeGetCurrentDirectory);
    slots.Set(0x5c80d8, (void*)&FakeSetCurrentDirectory);
    slots.Set(0x5c8390, (void*)&FakeGetOpenFileName);
    for (int c = 0; c < 4; ++c) {
        g_pick = c & 1;
        const char* dir = (c & 2) ? "maps" : nullptr;
        char fa[MAX_PATH], fb[MAX_PATH], logA[4096];
        memset(fa, 0x41, sizeof fa);
        memset(fb, 0x41, sizeof fb);
        ClearLog();
        int32_t ra = ((int32_t(__fastcall*)(char*, const char*, const char*))0x42a970)(fa, "Maps\0*.gmp\0", dir);
        strcpy(logA, g_log);
        ClearLog();
        int32_t rb = ((int32_t(__fastcall*)(char*, const char*, const char*))hook::Find(0x42a970))(fb, "Maps\0*.gmp\0", dir);
        EXPECT_EQ(ra, rb, "result %d", c);
        EXPECT_EQ(strcmp(fa, fb), 0, "file %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls %d: [%s] / [%s]", c, logA, g_log);
    }
}

DIFFTEST(misc_DrawFontText) {
    static void* vtbl[16];
    vtbl[0x14 / 4] = (void*)&FontBegin;
    vtbl[0x18 / 4] = (void*)&FontDraw;
    vtbl[0x20 / 4] = (void*)&FontEnd;
    static void* font[1] = { vtbl };
    static void* fontPtr = font;
    static uint8_t r[0x2600];
    for (int c = 0; c < 200; ++c) {
        F<int32_t>(r, 0x25c7) = 320 + Rnd() % 1600;
        F<int32_t>(r, 0x25cb) = 200 + Rnd() % 1200;
        float v[4];
        for (float& x : v) x = (float)((int)(Rnd() % 2000) - 500) / 1000.0f;
        const uint32_t colour = Rnd(), flags = Rnd() & 0x600;
        void** f = (c % 10) ? &fontPtr : nullptr;
        char logA[4096];
        ClearLog();
        ((void(__fastcall*)(uint8_t*, int, const char*, float, float, float, float, uint32_t, void**, uint32_t))0x5669d0)(
            r, 0, "Some text", v[0], v[1], v[2], v[3], colour, f, flags);
        strcpy(logA, g_log);
        ClearLog();
        ((void(__fastcall*)(uint8_t*, int, const char*, float, float, float, float, uint32_t, void**, uint32_t))hook::Find(0x5669d0))(
            r, 0, "Some text", v[0], v[1], v[2], v[3], colour, f, flags);
        EXPECT_EQ(strcmp(logA, g_log), 0, "case %d: [%s] / [%s]", c, logA, g_log);
    }
}

DIFFTEST(misc_ModelAddTintRect) {
    // A model (+0x8d loaded, +0x5b tints, +0x69 tint values, +0x14 animations
    // of 24 bytes, current +0x3a); an animation's frames (+0xc, count +0x14)
    // are 0x45 bytes: vertex count +8, buffer +0x35 (12-byte vertices, then
    // one mark byte per vertex).
    static float verts[2][4][64 * 3 + 64];
    static uint8_t frames[2][4][0x45], anims[2][2][24], model[2][0x100];
    for (int c = 0; c < 300; ++c) {
        uint32_t seed = Rnd();
        for (int side = 0; side < 2; ++side) {
            uint32_t s = seed;
            auto rnd = [&]() { s = s * 1103515245u + 12345u; return s >> 8; };
            memset(model[side], 0, sizeof model[side]);
            model[side][0x8d] = (uint8_t)(rnd() % 5 != 0);
            model[side][0x5b] = (uint8_t)(rnd() % 3);
            F<uint8_t*>(model[side], 0x14) = anims[side][0];
            F<int32_t>(model[side], 0x3a) = (int32_t)(rnd() % 2);
            for (int a = 0; a < 2; ++a) {
                memset(anims[side][a], 0, 24);
                F<uint8_t*>(anims[side][a], 0xc) = frames[side][0];
                F<int32_t>(anims[side][a], 0x14) = (int32_t)(rnd() % 5);
            }
            for (int fr = 0; fr < 4; ++fr) {
                memset(frames[side][fr], 0, 0x45);
                const int n = (int)(rnd() % 64);
                F<int32_t>(frames[side][fr], 8) = n;
                F<float*>(frames[side][fr], 0x35) = verts[side][fr];
                for (int k = 0; k < 64 * 3 + 64; ++k) verts[side][fr][k] = (float)((int)(rnd() % 4000) - 2000) / 100.0f;
            }
            const float x0 = (float)((int)(rnd() % 40) - 20), y0 = (float)((int)(rnd() % 40) - 20);
            const float x1 = x0 + (float)(rnd() % 30), y1 = y0 + (float)(rnd() % 30), tint = (float)(rnd() % 100) / 7.0f;
            ((void(__fastcall*)(uint8_t*, int, float, float, float, float, float))(side ? hook::Find(0x52dc00) : (void*)0x52dc00))(
                model[side], 0, x0, y0, x1, y1, tint);
        }
        // (+0x14 points into each side's own arrays)
        EXPECT_EQ(memcmp(model[0] + 0x18, model[1] + 0x18, 0x100 - 0x18) == 0 && memcmp(model[0], model[1], 0x14) == 0 ? 1 : 0, 1,
                  "model %d", c);
        EXPECT_EQ(memcmp(verts[0], verts[1], sizeof verts[0]), 0, "vertices and marks %d", c);
    }
}

DIFFTEST(misc_TimedQueuePop) {
    // The queue: +0 a node pool, +4 the lookup, +8 the heap, +0x20 a count.
    Redirects r;
    r.Add(0x53d540, (void*)&FakeHeapPopMax);
    r.Add(0x5a1145, (void*)&RecFree);
    r.Add(0x53d9e0, (void*)&RecHashRemove);
    r.Add(0x53b080, (void*)&RecDelete);
    static uint32_t nodes[2][8][6];
    for (int c = 0; c < 40; ++c) {
        uint32_t results[2], qs[2][9], fls[2][3];
        char logA[4096];
        for (int side = 0; side < 2; ++side) {
            uint32_t* q = qs[side];
            uint32_t* fl = fls[side];
            static uint32_t pool[2][4];
            memset(q, 0, sizeof qs[0]);
            fl[0] = (c & 2) ? 0x900 : 0;
            fl[1] = (c & 4) ? 0x904 : 0;
            fl[2] = c;
            pool[side][0] = (uint32_t)(uintptr_t)fl;
            pool[side][1] = (uint32_t)(uintptr_t)nodes[side][0];
            pool[side][2] = (uint32_t)(uintptr_t)nodes[side][8];
            pool[side][3] = c & 1;
            q[0] = (uint32_t)(uintptr_t)pool[side];
            q[1] = 0x4444;
            q[8] = 10;
            memset(nodes[side], 0, sizeof nodes[side]);
            g_popped = (uint8_t*)nodes[side][c % 8];
            F<uint32_t>(g_popped, 0) = 0xabc + c;
            F<uint32_t>(g_popped, 4) = 0x77 + c;
            F<uint32_t>(g_popped, 0x10) = (c & 8) ? 0x1234 : 0;
            ClearLog();
            results[side] = ((uint32_t(__fastcall*)(uint32_t*))(side ? hook::Find(0x564310) : (void*)0x564310))(q);
            if (!side) strcpy(logA, g_log);
        }
        EXPECT_EQ(results[0], results[1], "value %d", c);
        EXPECT_EQ(qs[0][8], qs[1][8], "count %d", c);
        EXPECT_EQ(fls[0][2], fls[1][2], "free count %d", c);
        EXPECT_EQ((fls[0][0] == (uint32_t)(uintptr_t)nodes[0][c % 8]) ? 1 : 0, (fls[1][0] == (uint32_t)(uintptr_t)nodes[1][c % 8]) ? 1 : 0,
                  "free head %d", c);
        EXPECT_EQ(nodes[0][c % 8][0] == fls[0][0] ? 1 : 0, nodes[1][c % 8][0] == fls[1][0] ? 1 : 0, "link %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls %d: [%s] / [%s]", c, logA, g_log);
    }
}

DIFFTEST(misc_TickerPickNext) {
    Redirects r;
    r.Add(0x53de00, (void*)&FakeTableEntry);
    r.Add(0x5521a0, (void*)&FakeStepB);
    uint32_t& lcg = hook::Global<uint32_t>(0x624324);
    const uint32_t keep = lcg;
    static uint8_t t[2][0x190];
    for (int c = 0; c < 300; ++c) {
        const uint32_t seed = Rnd();
        uint32_t lcgAfter[2];
        for (int side = 0; side < 2; ++side) {
            uint32_t s = seed;
            for (int k = 0; k < 0x190; ++k) {
                s = s * 1103515245u + 12345u;
                t[side][k] = (uint8_t)(s >> 16);
            }
            // Cool-downs 0..5, entries any value, phase 0..2.
            // (some always free: the pick loops until it finds a free one)
            for (int k = 0; k < 0x1f; ++k) F<int32_t>(t[side], 0x7c + k * 8) = k % 4 == 0 ? 0 : (int32_t)((seed >> (k % 16)) % 6);
            F<int32_t>(t[side], 0x178) = (int32_t)(seed % 3);
            lcg = seed ^ 0x55;
            ((void(__fastcall*)(uint8_t*))(side ? hook::Find(0x4c7ad0) : (void*)0x4c7ad0))(t[side]);
            lcgAfter[side] = lcg;
        }
        EXPECT_EQ(memcmp(t[0], t[1], sizeof t[0]), 0, "ticker %d", c);
        EXPECT_EQ(lcgAfter[0], lcgAfter[1], "generator %d", c);
    }
    lcg = keep;
}

DIFFTEST(misc_OrderEditApplyToAll) {
    // The edited order (0x6cfe04, a 0x33-byte stop) copied into every order of
    // the list 0x6cfe08 ({data, count}, 0x33-byte entries).
    uint8_t*& src = hook::Global<uint8_t*>(0x6cfe04);
    uint8_t*& list = hook::Global<uint8_t*>(0x6cfe08);
    uint8_t* keepSrc = src;
    uint8_t* keepList = list;
    static uint8_t order[0x40], entries[2][8 * 0x33];
    static uint32_t arr[2][2];
    for (int c = 0; c < 200; ++c) {
        for (uint8_t& b : order) b = (uint8_t)Rnd();
        const int n = (int)(Rnd() % 9);
        for (int k = 0; k < 8 * 0x33; ++k) entries[0][k] = entries[1][k] = (uint8_t)Rnd();
        for (int side = 0; side < 2; ++side) {
            arr[side][0] = (uint32_t)(uintptr_t)entries[side];
            arr[side][1] = (uint32_t)n;
            src = order;
            list = (uint8_t*)arr[side];
            ((void(*)())(side ? hook::Find(0x4b9ec0) : (void*)0x4b9ec0))();
        }
        EXPECT_EQ(memcmp(entries[0], entries[1], sizeof entries[0]), 0, "orders %d (%d)", c, n);
    }
    src = keepSrc;
    list = keepList;
}

namespace {
// A fake D3D8 device, texture and surface for the front buffer capture.
uint8_t g_front[64 * 48 * 4];
int32_t g_frontFails;
void* g_texVtbl[16];
void* g_surfVtbl[16];
void* g_tex[1] = { g_texVtbl };
void* g_surf[1] = { g_surfVtbl };
int32_t __stdcall DevCreateTexture(void*, int32_t w, int32_t h, int32_t levels, int32_t usage, int32_t fmt, int32_t pool, void** out) {
    Log("CreateTexture %d %d %d %d %d %d;", w, h, levels, usage, fmt, pool);
    *out = g_tex;
    return 0;
}
int32_t __stdcall DevGetFrontBuffer(void*, void* s) { Log("GetFrontBuffer %d;", (int)(s == g_surf)); return g_frontFails; }
int32_t __stdcall TexGetSurface(void*, int32_t level, void** out) { Log("GetSurface %d;", level); *out = g_surf; return 0; }
uint32_t __stdcall ObjRelease(void* o) { Log("Release %d;", (int)(o == g_tex ? 1 : o == g_surf ? 2 : 0)); return 0; }
int32_t __stdcall SurfLock(void*, int32_t* lr, void* rect, uint32_t flags) {
    Log("Lock %d %x;", (int)(rect != nullptr), flags);
    lr[0] = 999;  // a pitch the copy ignores (it steps by width x 4)
    lr[1] = (int32_t)(uintptr_t)g_front;
    return 0;
}
int32_t __stdcall SurfUnlock(void*) { Log("Unlock;"); return 0; }
}  // namespace

DIFFTEST(misc_CaptureFrontBuffer) {
    static void* devVtbl[64];
    devVtbl[0x50 / 4] = (void*)&DevCreateTexture;
    devVtbl[0x78 / 4] = (void*)&DevGetFrontBuffer;
    static void* dev[1] = { devVtbl };
    g_texVtbl[0x3c / 4] = (void*)&TexGetSurface;
    g_texVtbl[8 / 4] = (void*)&ObjRelease;
    g_surfVtbl[8 / 4] = (void*)&ObjRelease;
    g_surfVtbl[0x24 / 4] = (void*)&SurfLock;
    g_surfVtbl[0x28 / 4] = (void*)&SurfUnlock;
    static uint8_t e[0x114300];
    static int32_t modes[4][6];  // 0x15-byte mode records: w, h, ...
    for (uint8_t& b : g_front) b = (uint8_t)Rnd();
    for (int c = 0; c < 24; ++c) {
        memset(e, 0, sizeof e);
        e[0x114226] = (uint8_t)(c % 6 != 0);
        F<void*>(e, 8) = dev;
        const int w = 1 + Rnd() % 16, h = 1 + Rnd() % 12;
        uint8_t* mode = (uint8_t*)modes + (c & 1) * 0x15;
        F<int32_t>(mode, 0) = w;
        F<int32_t>(mode, 4) = h;
        F<uint8_t*>(e, 0x34) = (uint8_t*)modes;
        F<int32_t>(e, 0x11421e) = c & 1;
        g_frontFails = (c % 5 == 4) ? -1 : 0;
        const int x = Rnd() % 8, y = Rnd() % 8, stride = 24 + Rnd() % 8;
        static uint8_t da[40 * 40 * 4], db[40 * 40 * 4];
        memset(da, 0x5a, sizeof da);
        memset(db, 0x5a, sizeof db);
        char logA[4096];
        ClearLog();
        const uint8_t ra = ((uint8_t(__fastcall*)(uint8_t*, int, uint8_t*, int32_t, int32_t, int32_t))0x51faf0)(e, 0, da, x, y, stride);
        strcpy(logA, g_log);
        ClearLog();
        const uint8_t rb = ((uint8_t(__fastcall*)(uint8_t*, int, uint8_t*, int32_t, int32_t, int32_t))hook::Find(0x51faf0))(e, 0, db, x, y, stride);
        EXPECT_EQ(ra, rb, "result %d", c);
        EXPECT_EQ(memcmp(da, db, sizeof da), 0, "pixels %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "device calls %d: [%s] / [%s]", c, logA, g_log);
    }
}

namespace {
// Fatal_Exit's calls, recorded; exit raises an exception the test catches
// (the original does not come back from exit).
void __fastcall RecCursorShow(int32_t show, int32_t edx, int32_t s) { Log("Cursor %d %x %x;", show, edx, s); }
void __fastcall RecDebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i) {
    Log("Debug [%s] %x %x %x %x %x %x %x %x %x;", text, a, b, c, d, e, f, g, h, i);
}
void __fastcall RecRestoreScroll() { Log("RestoreScroll;"); }
void __cdecl RecExit(int code) {
    Log("Exit %d;", code);
    RaiseException(0xe0000123, 0, 0, nullptr);
}
int32_t __stdcall FakeMessageBox(void* w, const char* text, const char* title, uint32_t type) {
    Log("MessageBox %d [%s] [%s] %x;", (int)(w != nullptr), text, title, type);
    return 1;
}
int CallFatal(void* fn, const char* msg) {
    __try {
        ((void(__fastcall*)(const char*))fn)(msg);
    } __except (GetExceptionCode() == 0xe0000123 ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        return 1;
    }
    return 0;
}
}  // namespace

DIFFTEST(misc_FatalExit) {
    Redirects r;
    r.Add(0x51e810, (void*)&RecCursorShow);
    r.Add(0x51d680, (void*)&RecDebugLog);
    r.Add(0x54e700, (void*)&RecRestoreScroll);
    r.Add(0x5a2e9c, (void*)&RecExit);
    Slots slots;
    slots.Set(0x5c8220, (void*)&FakeMessageBox);
    int32_t& once = hook::Global<int32_t>(0x6d3d18);
    uint8_t*& eng = hook::Global<uint8_t*>(0x6d4024);
    const int32_t keepOnce = once;
    uint8_t* keepEng = eng;
    static uint8_t engine[2][0x100];
    const char* msgs[] = { "Could not open data\pack.pk4", nullptr, "" };
    for (int c = 0; c < 6; ++c) {
        int exited[2];
        int32_t onceAfter[2];
        char logA[4096];
        for (int side = 0; side < 2; ++side) {
            memset(engine[side], 0, sizeof engine[side]);
            eng = (c & 4) ? nullptr : engine[side];
            once = c == 5 ? 1 : 0;
            ClearLog();
            exited[side] = CallFatal(side ? hook::Find(0x51e980) : (void*)0x51e980, msgs[c % 3]);
            onceAfter[side] = once;
            if (!side) strcpy(logA, g_log);
        }
        EXPECT_EQ(exited[0], exited[1], "exit %d", c);
        EXPECT_EQ(onceAfter[0], onceAfter[1], "once flag %d", c);
        EXPECT_EQ(memcmp(engine[0], engine[1], sizeof engine[0]), 0, "engine %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls %d: [%s] / [%s]", c, logA, g_log);
    }
    once = keepOnce;
    eng = keepEng;
}

namespace {
// Texture_DecompressDxt3's D3DX calls (linked into the exe), the device's
// textures / surfaces and the runtime's malloc / free, all faked.
void* g_dxVt[16];
void* g_dxTex[2][1] = { { g_dxVt }, { g_dxVt } };
void* g_dxSurf[2][1] = { { g_dxVt }, { g_dxVt } };
uint8_t g_dxPixels[64 * 64 * 2];
uint8_t g_dxHeap[64 * 64 * 2 + 64];
int g_dxCreated;
const char* DxName(void* o) {
    return o == g_dxTex[0] ? "texA" : o == g_dxTex[1] ? "texB" : o == g_dxSurf[0] ? "surfA" : o == g_dxSurf[1] ? "surfB" : o ? "other" : "null";
}
int32_t __stdcall FakeDxCreate(void* dev, uint32_t w, uint32_t h, uint32_t levels, uint32_t usage, uint32_t fmt, uint32_t pool, void** out) {
    Log("Create %d %u %u %u %u %x %u;", (int)(dev != nullptr), w, h, levels, usage, fmt, pool);
    *out = g_dxTex[g_dxCreated++ & 1];
    return 0;
}
int32_t __stdcall FakeDxLoadMem(void* dst, void* pal, void* rect, const void* src, uint32_t fmt, uint32_t pitch, void* spal, const int32_t* srect,
                                uint32_t filter, uint32_t key) {
    Log("LoadMem %s %d %d %d %x %u %d %d %d %d %d %x %x;", DxName(dst), (int)(pal != nullptr), (int)(rect != nullptr), (int)(src != nullptr), fmt,
        pitch, (int)(spal != nullptr), srect[0], srect[1], srect[2], srect[3], filter, key);
    return 0;
}
int32_t __stdcall FakeDxLoadSurf(void* dst, void* pal, void* rect, void* src, void* spal, void* srect, uint32_t filter, uint32_t key) {
    Log("LoadSurf %s %d %d %s %d %d %x %x;", DxName(dst), (int)(pal != nullptr), (int)(rect != nullptr), DxName(src), (int)(spal != nullptr),
        (int)(srect != nullptr), filter, key);
    return 0;
}
int32_t __stdcall DxGetSurface(void* tex, uint32_t level, void** out) {
    Log("Surface %s %u;", DxName(tex), level);
    *out = tex == g_dxTex[0] ? g_dxSurf[0] : g_dxSurf[1];
    return 0;
}
uint32_t __stdcall DxRelease(void* o) { Log("Release %s;", DxName(o)); return 0; }
int32_t __stdcall DxLock(void* s, int32_t* lr, const void* rect, uint32_t flags) {
    Log("Lock %s %d %x;", DxName(s), (int)(rect != nullptr), flags);
    lr[0] = 77;
    lr[1] = (int32_t)(uintptr_t)g_dxPixels;
    return 0;
}
int32_t __stdcall DxUnlock(void* s) { Log("Unlock %s;", DxName(s)); return 0; }
void* __cdecl FakeMalloc(size_t n) { Log("Malloc %u;", (unsigned)n); return g_dxHeap; }
void __cdecl FakeDxFree(void* p) { Log("Free %d;", (int)(p != nullptr)); }
}  // namespace

DIFFTEST(misc_DecompressDxt3) {
    Redirects r;
    r.Add(0x570691, (void*)&FakeDxCreate);
    r.Add(0x57025d, (void*)&FakeDxLoadMem);
    r.Add(0x5706f7, (void*)&FakeDxLoadSurf);
    r.Add(0x5a125d, (void*)&FakeMalloc);
    r.Add(0x5a1145, (void*)&FakeDxFree);
    g_dxVt[8 / 4] = (void*)&DxRelease;
    g_dxVt[0x24 / 4] = (void*)&DxLock;
    g_dxVt[0x28 / 4] = (void*)&DxUnlock;
    g_dxVt[0x3c / 4] = (void*)&DxGetSurface;
    static uint8_t engine[0x40];
    uint8_t*& eng = hook::Global<uint8_t*>(0x6d4024);
    uint8_t* keepEng = eng;
    eng = engine;
    F<uint32_t>(engine, 8) = 0x4321;
    for (uint8_t& b : g_dxPixels) b = (uint8_t)Rnd();
    for (int c = 0; c < 30; ++c) {
        uint8_t rec[2][0x40], heap[2][sizeof g_dxHeap];
        char logA[4096];
        const int w = 4 << (Rnd() % 4), h = 4 << (Rnd() % 4);
        for (int side = 0; side < 2; ++side) {
            memset(rec[side], 0x11, sizeof rec[side]);
            F<int32_t>(rec[side], 8) = w;
            F<int32_t>(rec[side], 0xc) = h;
            F<uint32_t>(rec[side], 0x2a) = c % 7 == 0 ? 0x999 : 0;     // already converted
            F<uint32_t>(rec[side], 0x2e) = c % 5 == 0 ? 0 : 0x777;     // no DXT3 data
            memset(g_dxHeap, 0, sizeof g_dxHeap);
            g_dxCreated = 0;
            ClearLog();
            ((void(__stdcall*)(uint8_t*))(side ? hook::Find(0x5423d0) : (void*)0x5423d0))(rec[side]);
            memcpy(heap[side], g_dxHeap, sizeof g_dxHeap);
            if (!side) strcpy(logA, g_log);
        }
        EXPECT_EQ(memcmp(rec[0], rec[1], sizeof rec[0]), 0, "record %d", c);
        EXPECT_EQ(memcmp(heap[0], heap[1], sizeof heap[0]), 0, "pixels %d", c);
        EXPECT_EQ(strcmp(logA, g_log), 0, "calls %d: [%s] / [%s]", c, logA, g_log);
    }
    eng = keepEng;
}
