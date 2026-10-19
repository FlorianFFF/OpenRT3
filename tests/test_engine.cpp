// Engine/display object difftests on two fake engine objects.
#include "difftest.h"
#include "hook/hook.h"
#include "core/ptrlist.h"
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace rt3;

namespace rt3 {
void __fastcall Engine_CurrentMode(void*, int, int32_t*, int32_t*, int32_t*, int32_t*, uint32_t*);
void __fastcall Engine_ClampWindowSize(void*, int, int32_t*, int32_t*);
int32_t __fastcall Engine_FreeTextureMem(void*);
int32_t __fastcall Engine_TotalTextureMem(void*);
int32_t __fastcall Engine_TextureMemLimit(void*);
void __fastcall Engine_BuildTrigTables(void*);
void __fastcall Engine_UpdateFrameTime(void*);
int32_t __fastcall Engine_ModeHasFlags(void*);
void __fastcall Engine_QueueType5(void*, int, uint32_t, uint32_t, uint32_t, uint32_t);
void __fastcall Engine_QueueType6(void*);
void __fastcall Engine_QueueQuad(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                 uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                 uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
void __fastcall Engine_QueueSprite(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                   uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
void __fastcall Engine_QueueText(void*, int, uint32_t, float, float, uint32_t, float, float, uint32_t, uint32_t, uint32_t);
void __fastcall Engine_ResetTextureQueue(void*);
}

namespace {

const uint32_t kEngineSize = 0x114800;
template <typename T> T& F(uint8_t* e, uint32_t off) { return *(T*)(e + off); }

static DWORD g_fakeTick;
static DWORD WINAPI FakeTick() { return g_fakeTick; }

struct Side {
    uint8_t* e;
    std::vector<uint8_t*> textures;
    PtrList lists[3];
};

static void Setup(Side& s, uint8_t* modes) {
    s.e = (uint8_t*)VirtualAlloc(nullptr, kEngineSize, MEM_COMMIT, PAGE_READWRITE);
    F<uint8_t*>(s.e, 0x34) = modes;
    for (int i = 0; i < 3; ++i) {
        PtrList_Construct(&s.lists[i], 0, 0, 0);
        F<PtrList*>(s.e, i == 0 ? 0x11369d : i == 1 ? 0x1136a1 : 0x1136a5) = &s.lists[i];
    }
    for (int t = 0; t < 4; ++t) {
        uint8_t* tex = (uint8_t*)calloc(1, 0x100);
        PtrList* l = (PtrList*)calloc(1, sizeof(PtrList));
        *(PtrList**)(tex + 0xe4) = l;
        s.textures.push_back(tex);
    }
}

// Converts pointers into "which item / which texture" so both sides compare.
static int64_t Tag(const Side& s, void* p) {
    uintptr_t q = (uintptr_t)p;
    if (q >= (uintptr_t)s.e + 0x5c && q < (uintptr_t)s.e + 0x5c + 12000 * 0x5e) return 100000 + (q - (uintptr_t)s.e - 0x5c);
    for (size_t i = 0; i < s.textures.size(); ++i) if (p == s.textures[i]) return 200000 + i;
    return (int64_t)q;
}
static std::vector<int64_t> Dump(const Side& s) {
    std::vector<int64_t> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(-1); v.push_back(s.lists[i].count);
        for (PtrListNode* n = s.lists[i].head; n; n = n->next) v.push_back(Tag(s, n->value));
    }
    for (uint8_t* t : s.textures) {
        PtrList* l = *(PtrList**)(t + 0xe4);
        v.push_back(-2); v.push_back(l->count);
        for (PtrListNode* n = l->head; n; n = n->next) v.push_back(Tag(s, n->value));
        v.push_back(*(void**)(t + 0xe8) != nullptr);
    }
    int32_t n = F<int32_t>(s.e, 0x58);
    v.push_back(n);
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* it = s.e + 0x5c + i * 0x5e;
        uint8_t copy[0x5e];
        memcpy(copy, it, 0x5e);
        uint32_t tex = *(uint32_t*)(copy + 0x41);
        if (it[0] == 1 || it[0] == 4) *(uint32_t*)(copy + 0x41) = (uint32_t)Tag(s, (void*)(uintptr_t)tex);
        char* text = *(char**)(copy + 0x3d);
        if (it[0] == 3) { *(uint32_t*)(copy + 0x3d) = 0; for (const char* c = text; c && *c; ++c) v.push_back(*c); }
        for (int b = 0; b < 0x5e; ++b) v.push_back(copy[b]);
    }
    return v;
}

} // namespace

DIFFTEST(engine_queries) {
    uint8_t modes[0x15 * 4] = {};
    for (int m = 0; m < 4; ++m) {
        *(int32_t*)(modes + m * 0x15) = 640 + m * 160;
        *(int32_t*)(modes + m * 0x15 + 4) = 480 + m * 120;
        *(int32_t*)(modes + m * 0x15 + 8) = 16 + 16 * (m & 1);
        *(int32_t*)(modes + m * 0x15 + 0xc) = 60 + m;
        modes[m * 0x15 + 0x14] = (uint8_t)(m & 2);
    }
    Side a, b;
    Setup(a, modes);
    Setup(b, modes);
    auto oMode = (void(__fastcall*)(void*, int, int32_t*, int32_t*, int32_t*, int32_t*, uint32_t*))0x51f100;
    auto oClamp = (void(__fastcall*)(void*, int, int32_t*, int32_t*))0x51f870;
    auto oFree = (int32_t(__fastcall*)(void*))0x51fa00;
    auto oTotal = (int32_t(__fastcall*)(void*))0x51fa40;
    auto oLimit = (int32_t(__fastcall*)(void*))0x51fa60;
    auto oFlags = (int32_t(__fastcall*)(void*))0x51fd00;
    srand(31);
    for (int i = 0; i < 20000; ++i) {
        uint8_t* e = a.e;
        F<int32_t>(e, 0x11421e) = rand() % 4;
        F<int32_t>(e, 0x11470e) = rand() % 2;
        F<uint8_t>(e, 0x114226) = (uint8_t)(rand() % 2);
        F<uint8_t>(e, 0x114696) = (uint8_t)(rand() % 4 == 0);
        F<int32_t>(e, 0x38) = 400 + rand() % 1400;
        F<int32_t>(e, 0x3c) = 300 + rand() % 1000;
        F<int32_t>(e, 0x1136a9) = rand() * 1000 - 5000000;
        F<int32_t>(e, 0x1136ad) = rand() * 1000;
        F<int32_t>(e, 0x1146ca) = rand() % 3 ? 0 : rand() % 8;
        F<int32_t>(e, 0x1146c6) = rand() % 3 ? 0 : rand() % 8;
        F<int32_t>(e, 0x1146ce) = rand() % 3 ? 0 : rand() % 8;
        memcpy(b.e + 0x30, a.e + 0x30, 0x10);
        memcpy(b.e + 0x1136a9, a.e + 0x1136a9, 8);
        memcpy(b.e + 0x11421e, a.e + 0x11421e, 4);
        memcpy(b.e + 0x114226, a.e + 0x114226, 1);
        memcpy(b.e + 0x114696, a.e + 0x114696, 1);
        memcpy(b.e + 0x1146c6, a.e + 0x1146c6, 12);
        memcpy(b.e + 0x11470e, a.e + 0x11470e, 4);
        int32_t r1[5], r2[5];
        uint32_t f1, f2;
        oMode(a.e, 0, &r1[0], &r1[1], &r1[2], &r1[3], &f1);
        Engine_CurrentMode(b.e, 0, &r2[0], &r2[1], &r2[2], &r2[3], &f2);
        EXPECT_EQ(memcmp(r1, r2, 16), 0, "mode"); EXPECT_EQ(f1, f2, "flags");
        int32_t w1 = rand() % 2000, h1 = rand() % 1500, w2 = w1, h2 = h1;
        oClamp(a.e, 0, &w1, &h1);
        Engine_ClampWindowSize(b.e, 0, &w2, &h2);
        EXPECT_EQ(w1, w2, "clamp w"); EXPECT_EQ(h1, h2, "clamp h");
        EXPECT_EQ(oFree(a.e), Engine_FreeTextureMem(b.e), "free");
        EXPECT_EQ(oTotal(a.e), Engine_TotalTextureMem(b.e), "total");
        EXPECT_EQ(oLimit(a.e), Engine_TextureMemLimit(b.e), "limit");
        EXPECT_EQ(oFlags(a.e), Engine_ModeHasFlags(b.e), "hasflags");
    }
    ((void(__fastcall*)(void*))0x51faa0)(a.e);
    Engine_BuildTrigTables(b.e);
    EXPECT_EQ(memcmp(a.e + 0x1136b1, b.e + 0x1136b1, 720 * 4), 0, "trig tables");

    // Frame timing with a fake clock through the exe import slot.
    void** iat = (void**)0x5c8068;
    void* realTick = *iat;
    DWORD old;
    VirtualProtect(iat, 4, PAGE_READWRITE, &old);
    *iat = (void*)&FakeTick;
    void*& engine = hook::Global<void*>(0x6d4024);
    void* savedEngine = engine;
    int32_t& counter = hook::Global<int32_t>(0x6d403c);
    for (int i = 0; i < 3000; ++i) {
        F<int32_t>(a.e, 0x114206) = F<int32_t>(b.e, 0x114206) = rand() % 8 != 0;
        int32_t c0 = (int32_t)(rand() % 600);
        DWORD last = 1000 + rand() % 100000;
        g_fakeTick = last + rand() % 200;
        F<DWORD>(a.e, 0x114286) = F<DWORD>(b.e, 0x114286) = (rand() % 50 == 0) ? 0 : last;
        for (int k = 0; k < 256; ++k) ((int32_t*)(a.e + 0x11428a))[k] = ((int32_t*)(b.e + 0x11428a))[k] = rand() % (rand() % 4 == 0 ? 400 : 40);
        engine = a.e; counter = c0;
        ((void(__fastcall*)(void*))0x51fd70)(a.e);
        int32_t ca = counter;
        engine = b.e; counter = c0;
        Engine_UpdateFrameTime(b.e);
        EXPECT_EQ(ca, counter, "frame counter");
        EXPECT_EQ(F<uint32_t>(a.e, 0x114282), F<uint32_t>(b.e, 0x114282), "avg ms bits (c0=%d)", c0);
        EXPECT_EQ(memcmp(a.e + 0x114286, b.e + 0x114286, 4 + 1024), 0, "ring");
    }
    *iat = realTick;
    VirtualProtect(iat, 4, old, &old);
    engine = savedEngine;
    counter = 0;
}

DIFFTEST(engine_render_queue) {
    uint8_t modes[0x15] = {};
    Side a, b;
    Setup(a, modes);
    Setup(b, modes);
    int32_t& asian = hook::Global<int32_t>(0x6d4034);
    int32_t savedAsian = asian;
    static const char* texts[] = { "Hello", "", "Train 7 arrived" };
    srand(32);
    for (int i = 0; i < 4000 && g_fail < 30; ++i) {
        asian = rand() % 4 == 0;
        uint8_t sorted = (uint8_t)(rand() % 2), layer = (uint8_t)rand();
        F<uint8_t>(a.e, 0x11369c) = F<uint8_t>(b.e, 0x11369c) = sorted;
        F<uint8_t>(a.e, 0x114227) = F<uint8_t>(b.e, 0x114227) = layer;
        uint32_t p[20];
        for (auto& x : p) x = (uint32_t)rand() << 16 ^ rand();
        int t = rand() % 5;
        uint32_t ta = t < 4 ? (uint32_t)(uintptr_t)a.textures[t] : 0, tb = t < 4 ? (uint32_t)(uintptr_t)b.textures[t] : 0;
        switch (rand() % 6) {
        case 0:
            ((void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t, uint32_t))0x51f2b0)(a.e, 0, p[0], p[1], p[2], p[3]);
            Engine_QueueType5(b.e, 0, p[0], p[1], p[2], p[3]);
            break;
        case 1:
            ((void(__fastcall*)(void*))0x51f320)(a.e);
            Engine_QueueType6(b.e);
            break;
        case 2:
            ((void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                uint32_t, uint32_t, uint32_t))0x51f370)(a.e, 0, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
                                p[8], p[9], ta, p[11], p[12], p[13], p[14], p[15], p[16], p[17], p[18], p[19]);
            Engine_QueueQuad(b.e, 0, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], tb, p[11], p[12], p[13],
                             p[14], p[15], p[16], p[17], p[18], p[19]);
            break;
        case 3:
            ((void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                uint32_t, uint32_t, uint32_t, uint32_t))0x51f460)(a.e, 0, p[0], p[1], p[2], p[3], p[4], p[5],
                                ta, p[7], p[8], p[9], p[10], p[11]);
            Engine_QueueSprite(b.e, 0, p[0], p[1], p[2], p[3], p[4], p[5], tb, p[7], p[8], p[9], p[10], p[11]);
            break;
        case 4: {
            const char* s = texts[rand() % 3];
            float x = (float)(rand() % 1000) / 7, y = (float)(rand() % 1000) / 3, w = (float)(rand() % 300) / 11, h = (float)(rand() % 50) / 13;
            ((void(__fastcall*)(void*, int, uint32_t, float, float, uint32_t, float, float, uint32_t, uint32_t, uint32_t))0x51fc20)(
                a.e, 0, (uint32_t)(uintptr_t)s, x, y, p[0], w, h, p[1], p[2], p[3]);
            Engine_QueueText(b.e, 0, (uint32_t)(uintptr_t)s, x, y, p[0], w, h, p[1], p[2], p[3]);
            break;
        }
        case 5:
            if (rand() % 20 == 0) {
                ((void(__fastcall*)(void*))0x51f1d0)(a.e);
                Engine_ResetTextureQueue(b.e);
            }
            break;
        }
        if (F<int32_t>(a.e, 0x58) > 11990) { // also exercise the full-queue path, then reset
            EXPECT_EQ(Dump(a) == Dump(b), true, "full queue state");
            F<int32_t>(a.e, 0x58) = F<int32_t>(b.e, 0x58) = 0;
            for (Side* s : { &a, &b }) {
                for (auto& l : s->lists) PtrList_Clear(&l), l.count = 0;
                for (uint8_t* t : s->textures) { PtrList_Clear(*(PtrList**)(t + 0xe4)); (*(PtrList**)(t + 0xe4))->count = 0; *(void**)(t + 0xe8) = nullptr; }
            }
        }
    }
    EXPECT_EQ(Dump(a) == Dump(b), true, "queue state");
    asian = savedAsian;
}
