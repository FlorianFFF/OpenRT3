// Small functions that only pass a call on (thunks, forwarders, destructors)
// or keep a little state, original against rewrite. A forwarder's targets are
// redirected to recorders (at the exe address and at the rewrite's own entry,
// so both sides land there) and the recorded calls are compared.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace {

template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

// JMP patches, undone at the end of the test.
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
    // The exe address and, when rewritten, the rewrite's entry.
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

// The call log: {target, ecx, edx, stack args...} per call.
uint32_t g_log[64][6];
int g_n;
uint32_t* Rec(uint32_t target, uint32_t ecx, uint32_t edx) {
    uint32_t* r = g_log[g_n < 63 ? g_n++ : 63];
    memset(r, 0, sizeof g_log[0]);
    r[0] = target;
    r[1] = ecx;
    r[2] = edx;
    return r;
}
struct Snapshot {
    uint32_t log[64][6];
    int n;
    void Take() { memcpy(log, g_log, sizeof log); n = g_n; g_n = 0; }
    bool Same() const { return n == g_n && memcmp(log, g_log, sizeof log[0] * n) == 0; }
};

// Recorders (their conventions match the targets').
void __fastcall RecNamedObjSave(uint32_t ecx, uint32_t edx, uint32_t s) { Rec(0x4559d0, ecx, edx)[3] = s; }
void __cdecl RecUpgradeCost() { Rec(0x50f890, 0, 0); }
int32_t __fastcall RecStoreRule(uint32_t ecx) { Rec(0x4d8ea0, ecx, 0); return 7; }
void __fastcall RecPopBusy(uint32_t ecx) { Rec(0x523e20, ecx, 0); }
// (0x4361d0 overwrites ECX first - MOV ECX,[0x6cfca8] - so what a caller
// leaves there is not an argument; the rewrite takes none.)
void __fastcall RecRefreshColors(uint32_t) { Rec(0x4361d0, 0, 0); }
void __fastcall RecSetVtbl(uint32_t ecx) { Rec(0x53f7d0, ecx, 0); }
uint32_t __fastcall RecMapInsert(uint32_t ecx, uint32_t edx, uint32_t key, uint32_t value) {
    uint32_t* r = Rec(0x53d960, ecx, edx);
    r[3] = key;
    r[4] = value;
    return key ^ 0x5a5a;
}
uint32_t __fastcall RecHashGet(uint32_t ecx, uint32_t, uint32_t key) { Rec(0x53dae0, ecx, 0)[3] = key; return key + 1; }
uint32_t __fastcall RecHashRemove(uint32_t ecx, uint32_t, uint32_t key) { Rec(0x53d9e0, ecx, 0)[3] = key; return key + 2; }
void __fastcall RecCampaignRefresh(uint32_t ecx) { Rec(0x4b89c0, ecx, 0); }
void __fastcall RecTextureDestroy(uint32_t ecx) { Rec(0x542c90, ecx, 0); }
void __cdecl RecDelete(uint32_t p) { Rec(0x53b080, 0, 0)[3] = p; }
void __fastcall RecStreamWrite(uint32_t s, uint32_t data, uint32_t size, uint32_t flag) {
    uint32_t* r = Rec(0x531030, s, 0);
    r[3] = size;
    r[4] = flag;
    r[5] = size ? *(const uint8_t*)data | ((uint32_t)((const uint8_t*)data)[size - 1] << 8) : 0;
    (void)data;
}

DWORD g_fakeTick = 5000;
DWORD WINAPI FakeTick() { return g_fakeTick; }

}  // namespace

DIFFTEST(fwd_Thunks) {
    Redirects r;
    r.Add(0x4559d0, (void*)&RecNamedObjSave);
    r.Add(0x50f890, (void*)&RecUpgradeCost);
    r.Add(0x4d8ea0, (void*)&RecStoreRule);
    for (int i = 0; i < 20; ++i) {
        uint32_t a = 0x1000 + i * 77, b = i * 3, s = 0x40 + i;
        g_n = 0;
        ((void(__fastcall*)(uint32_t, uint32_t, uint32_t))0x4742d0)(a, b, s);
        ((void(__cdecl*)())0x510e20)();
        int32_t ra = ((int32_t(__fastcall*)(uint32_t))0x4d9ec0)(a);
        Snapshot o;
        o.Take();
        ((void(__fastcall*)(uint32_t, uint32_t, uint32_t))hook::Find(0x4742d0))(a, b, s);
        ((void(__cdecl*)())hook::Find(0x510e20))();
        int32_t rb = ((int32_t(__fastcall*)(uint32_t))hook::Find(0x4d9ec0))(a);
        EXPECT_EQ(ra, rb, "store rule result");
        EXPECT_EQ(o.Same() ? 1 : 0, 1, "thunk calls, case %d", i);
    }
}

DIFFTEST(fwd_BulldozeDestruct) {
    static uint8_t engine[0x40], renderer[0x40], w[0x40];
    uint8_t*& eng = hook::Global<uint8_t*>(0x6d4024);
    uint8_t*& world = hook::Global<uint8_t*>(0x6cec78);
    uint8_t* keepEng = eng;
    uint8_t* keepWorld = world;
    Redirects r;
    r.Add(0x523e20, (void*)&RecPopBusy);
    r.Add(0x4361d0, (void*)&RecRefreshColors);
    r.Add(0x53f7d0, (void*)&RecSetVtbl);
    eng = engine;
    world = (uint8_t*)0x1234;
    F<uint8_t*>(engine, 0x18) = renderer;
    for (int side = 0; side < 2; ++side) {
        memset(w, 0x11, sizeof w);
        g_n = 0;
        ((void(__fastcall*)(uint8_t*))(side ? hook::Find(0x4b8440) : (void*)0x4b8440))(w);
        static Snapshot o;
        static uint8_t ow[0x40];
        if (!side) {
            o.Take();
            memcpy(ow, w, sizeof w);
        } else {
            EXPECT_EQ(memcmp(ow, w, sizeof w), 0, "window");
            EXPECT_EQ(o.Same() ? 1 : 0, 1, "calls");
        }
    }
    eng = keepEng;
    world = keepWorld;
}

DIFFTEST(fwd_SessionMap) {
    Redirects r;
    r.Add(0x53d960, (void*)&RecMapInsert);
    r.Add(0x53dae0, (void*)&RecHashGet);
    r.Add(0x53d9e0, (void*)&RecHashRemove);
    static uint8_t session[0x9060];
    F<uint32_t>(session, 0x9050) = 0x4440;
    for (int i = 0; i < 10; ++i) {
        uint32_t key = 0x100 + i * 13, results[2][3];
        for (int side = 0; side < 2; ++side) {
            auto ins = (uint32_t(__fastcall*)(void*, int, uint32_t, int32_t))(side ? hook::Find(0x468ca0) : (void*)0x468ca0);
            auto qry = (uint32_t(__fastcall*)(void*, int, uint32_t, int32_t))(side ? hook::Find(0x468cb0) : (void*)0x468cb0);
            g_n = 0;
            results[side][0] = ins(session, 0, key, i * 5);
            results[side][1] = qry(session, 0, key, 0);
            results[side][2] = qry(session, 0, key, 1);
            static Snapshot o;
            if (!side) o.Take();
            else EXPECT_EQ(o.Same() ? 1 : 0, 1, "calls %d", i);
        }
        EXPECT_EQ(memcmp(results[0], results[1], sizeof results[0]), 0, "results %d", i);
    }
}

DIFFTEST(fwd_CampaignRefresh) {
    Redirects r;
    r.Add(0x4b89c0, (void*)&RecCampaignRefresh);
    uint8_t*& win = hook::Global<uint8_t*>(0x6cfd1c);
    uint8_t* keep = win;
    static const uint32_t kWindows[] = { 0u, 0x777u };
    for (uint32_t v : kWindows) {
        win = (uint8_t*)(uintptr_t)v;
        g_n = 0;
        ((void(*)())0x4b8db0)();
        Snapshot o;
        o.Take();
        ((void(*)())hook::Find(0x4b8db0))();
        EXPECT_EQ(o.Same() ? 1 : 0, 1, "window %x", v);
    }
    win = keep;
}

DIFFTEST(state_DialogSetters) {
    // The scenario setup window's fields (0x6cff84 +0x8c / +0x94 / +0x98) and the
    // counter 0x6cff7c of the editor's +/- buttons.
    static uint8_t dlg[2][0x100];
    uint8_t*& d = hook::Global<uint8_t*>(0x6cff84);
    int32_t& counter = hook::Global<int32_t>(0x6cff7c);
    uint8_t* keepD = d;
    const int32_t keepC = counter;
    for (int i = 0; i < 20; ++i) {
        const int32_t v = (int32_t)(i * 0x1234567 - 50);
        int32_t r[2][6];
        for (int side = 0; side < 2; ++side) {
            auto pick = [&](uint32_t a) { return side ? hook::Find(a) : (void*)(uintptr_t)a; };
            memset(dlg[side], 0x33, sizeof dlg[side]);
            d = dlg[side];
            ((void(__fastcall*)(void*, int32_t))pick(0x4d7950))(nullptr, v);
            ((void(__fastcall*)(int32_t))pick(0x4d7960))(v + 1);
            ((void(__fastcall*)(int32_t))pick(0x4d7970))(v + 2);
            counter = i;
            const int32_t msgs[6][2] = { { 0xcb, 0x5143 }, { 0xcb, 0x5142 }, { 0xcb, 0x5143 }, { 0xca, 0x5143 }, { 0xcb, 0x5144 }, { 0, 0 } };
            for (int k = 0; k < 6; ++k) {
                int32_t msg[12] = { msgs[k][0], msgs[k][1] };
                r[side][k] = ((int32_t(__fastcall*)(void*, int, const int32_t*))pick(0x4cb6b0))(nullptr, 0, msg) * 1000 + counter;
            }
        }
        EXPECT_EQ(memcmp(dlg[0], dlg[1], sizeof dlg[0]), 0, "dialog fields %d", i);
        EXPECT_EQ(memcmp(r[0], r[1], sizeof r[0]), 0, "counter messages %d", i);
    }
    d = keepD;
    counter = keepC;
}

DIFFTEST(state_Pool) {
    // A node pool: p[0] the free list {head, tail, count}, p[1]..p[2] the
    // nodes' range, p[3] pooled (else the heap), p[4] the node size.
    for (int c = 0; c < 200; ++c) {
        static uint32_t nodes[2][16][4];
        uint32_t list[2][3], pool[2][5];
        void* got[2][8];
        for (int side = 0; side < 2; ++side) {
            auto alloc = (void*(__fastcall*)(int32_t*))(side ? hook::Find(0x4143e0) : (void*)0x4143e0);
            auto rel = (void(__fastcall*)(int32_t*, int, void**))(side ? hook::Find(0x414410) : (void*)0x414410);
            srand(c);
            memset(nodes[side], 0, sizeof nodes[side]);
            const int n = rand() % 6;
            for (int k = 0; k < n; ++k) nodes[side][k][0] = k + 1 < n ? (uint32_t)(uintptr_t)nodes[side][k + 1] : 0;
            list[side][0] = n ? (uint32_t)(uintptr_t)nodes[side][0] : 0;
            list[side][1] = n ? (uint32_t)(uintptr_t)nodes[side][n - 1] : 0;
            list[side][2] = n;
            pool[side][0] = (uint32_t)(uintptr_t)list[side];
            pool[side][1] = (uint32_t)(uintptr_t)nodes[side];
            pool[side][2] = (uint32_t)(uintptr_t)nodes[side][16];
            pool[side][3] = 1;
            pool[side][4] = 16;
            for (int k = 0; k < 8; ++k) {
                if (rand() % 2) {
                    got[side][k] = alloc((int32_t*)pool[side]);
                } else {
                    const int which = rand() % 16;
                    rel((int32_t*)pool[side], 0, (void**)nodes[side][which]);
                    got[side][k] = nodes[side][which];
                }
            }
        }
        // Compare as positions in the node arrays (the arrays differ per side).
        for (int k = 0; k < 8; ++k) {
            const intptr_t a = got[0][k] ? (uint8_t*)got[0][k] - (uint8_t*)nodes[0] : -1;
            const intptr_t b = got[1][k] ? (uint8_t*)got[1][k] - (uint8_t*)nodes[1] : -1;
            EXPECT_EQ(a, b, "case %d op %d", c, k);
        }
        auto rel0 = [&](int side, uint32_t v) { return v ? (intptr_t)(v - (uint32_t)(uintptr_t)nodes[side]) : -1; };
        EXPECT_EQ(rel0(0, list[0][0]), rel0(1, list[1][0]), "head %d", c);
        EXPECT_EQ(rel0(0, list[0][1]), rel0(1, list[1][1]), "tail %d", c);
        EXPECT_EQ(list[0][2], list[1][2], "count %d", c);
        for (int k = 0; k < 16; ++k)
            EXPECT_EQ(rel0(0, nodes[0][k][0]), rel0(1, nodes[1][k][0]), "link %d of case %d", k, c);
    }
    // Unpooled: straight to the heap (operator new / delete).
    uint32_t list[3] = {}, pool[5] = { (uint32_t)(uintptr_t)list, 0, 0, 0, 24 };
    for (int side = 0; side < 2; ++side) {
        auto alloc = (void*(__fastcall*)(int32_t*))(side ? hook::Find(0x4143e0) : (void*)0x4143e0);
        auto rel = (void(__fastcall*)(int32_t*, int, void**))(side ? hook::Find(0x414410) : (void*)0x414410);
        void* p = alloc((int32_t*)pool);
        EXPECT_EQ(p != nullptr ? 1 : 0, 1, "heap node, side %d", side);
        rel((int32_t*)pool, 0, (void**)p);
    }
}

DIFFTEST(state_TimerRestart) {
    void** iat = (void**)0x5c8068;  // GetTickCount
    DWORD old;
    VirtualProtect(iat, 4, PAGE_READWRITE, &old);
    void* real = *iat;
    *iat = (void*)&FakeTick;
    DWORD& base = hook::Global<DWORD>(0x6d3d14);
    const DWORD keepBase = base;
    for (int i = 0; i < 10; ++i) {
        uint8_t t[2][0x30];
        for (int side = 0; side < 2; ++side) {
            memset(t[side], 0x44, sizeof t[side]);
            base = 1000 + i;
            g_fakeTick = 5000 + i * 333;
            ((void(__fastcall*)(uint8_t*))(side ? hook::Find(0x53ef00) : (void*)0x53ef00))(t[side]);
        }
        EXPECT_EQ(memcmp(t[0], t[1], sizeof t[0]), 0, "timer %d", i);
    }
    base = keepBase;
    *iat = real;
    VirtualProtect(iat, 4, old, &old);
}

DIFFTEST(state_Streams) {
    // StreamSize reads the stream table (0xccb880); StreamWriteText hands the
    // text without its NUL to StreamWrite (0x531030, recorded).
    Redirects r;
    r.Add(0x531030, (void*)&RecStreamWrite);
    const char* texts[] = { "", "a", "Hello world", "trailing\n" };
    for (int s = 0; s < 4; ++s) {
        EXPECT_EQ(((int32_t(__fastcall*)(int32_t))0x530c10)(s), ((int32_t(__fastcall*)(int32_t))hook::Find(0x530c10))(s), "size of %d", s);
        g_n = 0;
        ((void(__fastcall*)(int32_t, const char*))0x531490)(s, texts[s]);
        Snapshot o;
        o.Take();
        ((void(__fastcall*)(int32_t, const char*))hook::Find(0x531490))(s, texts[s]);
        EXPECT_EQ(o.Same() ? 1 : 0, 1, "write text %d", s);
    }
}

DIFFTEST(state_PictureFreeTexture) {
    // Releases the picture's texture (+0xe6: 0x542c90, then delete) and
    // re-enables the engine flag (+0x114227) with 0xd97984 cleared.
    Redirects r;
    r.Add(0x542c90, (void*)&RecTextureDestroy);
    r.Add(0x53b080, (void*)&RecDelete);
    static uint8_t engine[2][0x114300];
    uint8_t*& eng = hook::Global<uint8_t*>(0x6d4024);
    uint8_t* keepEng = eng;
    int32_t& flag = hook::Global<int32_t>(0xd97984);
    const int32_t keepFlag = flag;
    for (int c = 0; c < 8; ++c) {
        uint8_t w[2][0x100];
        int32_t f[2];
        static Snapshot o;
        for (int side = 0; side < 2; ++side) {
            memset(w[side], 0x55, sizeof w[side]);
            F<uint32_t>(w[side], 0xe6) = (c & 1) ? 0x123400 + c : 0;
            eng = engine[side];
            engine[side][0x114227] = (uint8_t)(c * 37);
            flag = 9;
            g_n = 0;
            ((void(__fastcall*)(uint8_t*))(side ? hook::Find(0x55ed40) : (void*)0x55ed40))(w[side]);
            f[side] = flag;
            if (!side) o.Take();
        }
        EXPECT_EQ(memcmp(w[0], w[1], sizeof w[0]), 0, "picture %d", c);
        EXPECT_EQ(engine[0][0x114227], engine[1][0x114227], "engine flag %d", c);
        EXPECT_EQ(f[0], f[1], "0xd97984 %d", c);
        EXPECT_EQ(o.Same() ? 1 : 0, 1, "calls %d", c);
    }
    eng = keepEng;
    flag = keepFlag;
}
