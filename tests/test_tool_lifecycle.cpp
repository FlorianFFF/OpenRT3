// Differential tests for tool functions reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
void __fastcall TrackTool_Destroy(uint8_t* tool);
void __fastcall View_DestroyToolForMode(uint8_t* view, int, uint32_t mode);
void __fastcall View_Destroy(uint8_t* view);
}

namespace {

template <typename T> T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t saved[5]; } patches[96];
    int count = 0;
    void AddOne(uintptr_t address, void* fn) {
        Patch& p = patches[count++];
        p.at = (uint8_t*)address;
        DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.saved, p.at, 5);
        p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)fn - (address + 5));
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    // Original code calls the exe address, rewritten code calls the rewrite
    // directly: a redirect covers both.
    void Add(uintptr_t addr, void* target) {
        AddOne(addr, target);
        if (void* r = hook::Find((uint32_t)addr)) AddOne((uintptr_t)r, target);
    }
    ~Redirects() {
        while (count) {
            Patch& p = patches[--count];
            DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.saved, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};

const uint32_t kGlobals[] = {
    0x6cec74, 0x6cec78, 0x6cfca8, 0x6d401c, 0x6ada90, 0x62be18,
    0x62c120, 0xccbb20, 0x6d1a8c, 0x6d1a90, 0x6d1a94, 0x6d0818
};
struct Fixture {
    uint8_t view[0xa0], tools[2][0xb0], tracks[0x110], prefs[0x320];
    uint8_t map[0x2200], nodes[4][0x250], pending[3][0x10];
    uint32_t globals[sizeof kGlobals / sizeof kGlobals[0]];
    uint32_t log[512];
    int used;
};
Fixture g_f;
uint32_t g_bits;
int g_mode, g_nodeCount;

uint32_t Id(void* p) {
    if (p == g_f.view) return 1;
    if (p == g_f.tracks) return 2;
    if (p == g_f.prefs) return 3;
    if (p == g_f.map) return 4;
    for (int i = 0; i < 2; ++i) if (p == g_f.tools[i]) return 10 + i;
    for (int i = 0; i < 4; ++i) if (p == g_f.nodes[i]) return 20 + i;
    for (int i = 0; i < 3; ++i) if (p == g_f.pending[i]) return 30 + i;
    return (uint32_t)(uintptr_t)p;
}
void Event(uint32_t a, uint32_t b = 0, uint32_t c = 0) {
    g_f.log[g_f.used++] = a;
    g_f.log[g_f.used++] = b;
    g_f.log[g_f.used++] = c;
}
template <uint32_t Address> void __fastcall Method(void* self) {
    bool tool = self == g_f.tools[0] || self == g_f.tools[1];
    Event(Address, Id(self), tool ? F<uint32_t>(self, 0) : 0);
    if (tool) {
        Event(Address | 0x80000000, F<uint32_t>(self, 0x83), *(uint32_t*)0x6d1a8c);
        F<uint32_t>(self, 0) = Address;
        if (g_mode == 0 && (g_bits & 0x1000)) F<void*>(g_f.view, 0x88) = g_f.tools[1];
    }
    if (Address == 0x489f40 && (g_bits & 0x20000000)) F<uint32_t>(g_f.tracks, 0xed) = 0;
    if (Address == 0x4358d0 && (g_bits & 0x40000000)) *(uint32_t*)0x62be18 = 0xbe19;
}
void __fastcall Unregister(void* manager, int, void* tool) {
    Event(0x5389c0, Id(manager), Id(tool));
    uint32_t change = (g_bits >> 8) & 3;
    if (change == 1) F<void*>(g_f.view, 0x88) = nullptr;
    if (change == 2) F<void*>(g_f.view, 0x88) = g_f.tools[1];
}
template <uint32_t Address> void __cdecl Free(void* memory) {
    Event(Address, Id(memory), Id(F<void*>(g_f.view, 0x88)));
    if (Address == 0x5a1145) {
        Event(0x5a1146, Id(F<void*>(g_f.tracks, 0x98)));
        if (g_bits & 0x8000) F<void*>(g_f.tracks, 0x98) = nullptr;
    }
}
int32_t __fastcall Count(void* tracks) {
    Event(0x517cf0, Id(tracks), F<uint32_t>(tracks, 0x94));
    return g_nodeCount;
}
uint32_t __fastcall IdAt(void* tracks, int, int32_t i, uint32_t order) {
    Event(0x518380, (uint32_t)i, order);
    return ((uint32_t)i + 1) % 4;
}
void* __fastcall Get(void* tracks, int, uint32_t id) {
    Event(0x518140, Id(tracks), id);
    return g_f.nodes[id % 4];
}
void __fastcall Finalize(void* node, int, uint32_t mode) {
    Event(0x48b830, Id(node), mode);
    ((uint8_t*)node)[0x216] = 2;
}
void __fastcall Notify(void* manager, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Event(0x538e00, Id(manager), type);
    Event(id, a, b);
    Event(c);
}
void __fastcall Rebuild(void* table, int, uint32_t mode, uint32_t a, uint32_t b) {
    Event(0x432f40, Id(table), mode);
    Event(a, b);
}
void __fastcall ResetTracks(void* tracks) {
    Event(0x4a3db0, Id(tracks), F<uint32_t>(g_f.tools[0], 0));
    F<uint32_t>(tracks, 0x88) = 0xffffffff;
}
void __fastcall CursorText(void* cursor, int, const char* text, int32_t value) {
    Event(0x53ef90, Id(cursor), (uint32_t)(uintptr_t)text);
    Event((uint32_t)value, *(uint32_t*)0x6d1a8c);
    if (g_bits & 0x8000000) F<uint32_t>(g_f.map, 0x1689) ^= 0xabcdef01;
}
int32_t __fastcall MapCheck(void* map) {
    Event(0x450150, Id(map));
    return (g_bits >> 24) & 1;
}
void __fastcall DestroyMode(uint8_t* view, int, uint32_t mode) {
    Event(0x4dd950, mode, F<uint32_t>(view, 0));
    Event(0x6d0818, *(uint32_t*)0x6d0818, Id(F<void*>(view, 0x88)));
    F<void*>(view, 0x88) = nullptr;
}

void Setup(uint32_t it, int mode) {
    memset(&g_f, 0, sizeof g_f);
    uint32_t r = it * 2654435761u + 541;
    auto rnd = [&]() { r = r * 1103515245u + 12345u; return r; };
    g_bits = rnd();
    g_mode = mode;
    g_nodeCount = (int)((it / 7) % 5);
    for (uint8_t& b : g_f.view) b = (uint8_t)(rnd() >> 16);
    for (auto& tool : g_f.tools) for (uint8_t& b : tool) b = (uint8_t)(rnd() >> 16);
    for (uint8_t& b : g_f.tracks) b = (uint8_t)(rnd() >> 16);
    F<void*>(g_f.view, 0x88) = (it / 32) % 3 ? g_f.tools[0] : nullptr;
    F<uint32_t>(g_f.prefs, 0x94) = (g_bits >> 1) & 1;
    F<uint32_t>(g_f.prefs, 0x263) = (g_bits >> 2) & 1;
    F<uint32_t>(g_f.prefs, 0x2cc) = (g_bits >> 3) & 1;
    F<uint32_t>(g_f.prefs, 0x309) = (g_bits >> 4) & 1;
    F<uint32_t>(g_f.map, 0x1689) = rnd();
    F<uint32_t>(g_f.map, 0x168d) = rnd();
    F<uint32_t>(g_f.map, 0x2175) = (g_bits & 0x20) ? 0x15 : 0x16;
    F<uint32_t>(g_f.tracks, 0xed) = (g_bits >> 6) & 1;
    F<uint32_t>(g_f.tracks, 0x102) = rnd();
    for (int i = 0; i < 4; ++i) {
        F<uint32_t>(g_f.nodes[i], 0x240) = rnd();
        g_f.nodes[i][0x216] = (uint8_t)((g_bits >> (10 + i)) & 3);
    }
    for (int i = 0; i < 3; ++i) F<void*>(g_f.pending[i], 0xc) = i < 2 ? g_f.pending[i + 1] : nullptr;
    F<void*>(g_f.tracks, 0x98) = it % 4 ? g_f.pending[0] : nullptr;
    *(void**)0x6cec74 = g_f.prefs;
    *(uint32_t*)0x6cec78 = 0xcec78;
    *(void**)0x6cfca8 = g_f.tracks;
    *(uint32_t*)0x6d401c = (g_bits & 0x10000) ? 0x401c : 0;
    *(uint32_t*)0x6ada90 = 0xada90;
    *(uint32_t*)0x62be18 = 0xbe18;
    *(void**)0x62c120 = g_f.map;
    *(uint32_t*)0xccbb20 = 0xbb20;
    *(uint32_t*)0x6d1a8c = rnd();
    *(uint32_t*)0x6d1a90 = (g_bits >> 18) & 1;
    *(uint32_t*)0x6d1a94 = (g_bits >> 19) & 1;
    *(uint32_t*)0x6d0818 = rnd();
}
void RunCases(int mode, uintptr_t original, void* cpp) {
    uint32_t saved[sizeof kGlobals / sizeof kGlobals[0]];
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    for (uint32_t it = 0; it < 2048; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, mode);
            void* fn = side ? cpp : (void*)original;
            if (mode == 0) {
                uint32_t kind = it % 32;
                if (kind == 31) kind = 0xffffffff;
                ((void(__fastcall*)(uint8_t*, int, uint32_t))fn)(g_f.view, 0, kind);
            } else ((void(__fastcall*)(uint8_t*))fn)(mode == 1 ? g_f.tracks : mode == 3 ? g_f.view : g_f.tools[0]);
            for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t offset = 0;
                while (((uint8_t*)&expected)[offset] == ((uint8_t*)&g_f)[offset]) ++offset;
                EXPECT_EQ(1, 0, "tool lifecycle mode %d case %u byte %u: %02x / %02x (events %d / %d)",
                          mode, it, (unsigned)offset, ((uint8_t*)&expected)[offset], ((uint8_t*)&g_f)[offset],
                          expected.used, g_f.used);
            }
        }
    }
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}

} // namespace

DIFFTEST(game_ViewDestroyToolForMode) {
    Redirects p;
    p.Add(0x5389c0, (void*)&Unregister);
    p.Add(0x53b080, (void*)&Free<0x53b080>);
#define DTOR(address) p.Add(address, (void*)&Method<address>)
    DTOR(0x53f7d0); DTOR(0x5157c0); DTOR(0x512f10); DTOR(0x506a80);
    DTOR(0x505bf0); DTOR(0x507b50); DTOR(0x4c6ae0); DTOR(0x4c2320);
    DTOR(0x4f3b30); DTOR(0x4b3520); DTOR(0x50bac0); DTOR(0x50db10);
    DTOR(0x4b8440); DTOR(0x4bc1d0); DTOR(0x4bcbf0); DTOR(0x4f5ee0);
    DTOR(0x4fcb40); DTOR(0x4f50d0); DTOR(0x4f5750);
#undef DTOR
    RunCases(0, 0x4dd950, (void*)&rt3::View_DestroyToolForMode);
}
DIFFTEST(game_TrackListResetBuildState) {
    Redirects p;
    p.Add(0x517cf0, (void*)&Count);
    p.Add(0x518380, (void*)&IdAt);
    p.Add(0x518140, (void*)&Get);
    p.Add(0x48b830, (void*)&Finalize);
    p.Add(0x489f40, (void*)&Method<0x489f40>);
    p.Add(0x5a1145, (void*)&Free<0x5a1145>);
    p.Add(0x538e00, (void*)&Notify);
    p.Add(0x4358d0, (void*)&Method<0x4358d0>);
    p.Add(0x432f40, (void*)&Rebuild);
    p.Add(0x4682c0, (void*)&Method<0x4682c0>);
    RunCases(1, 0x4a3db0, (void*)&rt3::TrackList_ResetBuildState);
}
DIFFTEST(game_TrackToolDestroy) {
    Redirects p;
    p.Add(0x50d1e0, (void*)&Method<0x50d1e0>);
    p.Add(0x4a3db0, (void*)&ResetTracks);
    p.Add((uintptr_t)&rt3::TrackList_ResetBuildState, (void*)&ResetTracks);
    p.Add(0x53ef90, (void*)&CursorText);
    p.Add(0x450150, (void*)&MapCheck);
    p.Add(0x453510, (void*)&Method<0x453510>);
    p.Add(0x4361d0, (void*)&Method<0x4361d0>);
    p.Add(0x491880, (void*)&Method<0x491880>);
    p.Add(0x538e00, (void*)&Notify);
    p.Add(0x53f7d0, (void*)&Method<0x53f7d0>);
    RunCases(2, 0x50db10, (void*)&rt3::TrackTool_Destroy);
}
DIFFTEST(game_ViewDestroy) {
    Redirects p;
    p.Add(0x4dd950, (void*)&DestroyMode);
    p.Add((uintptr_t)&rt3::View_DestroyToolForMode, (void*)&DestroyMode);
    p.Add(0x53f7d0, (void*)&Method<0x53f7d0>);
    RunCases(3, 0x4dfd70, (void*)&rt3::View_Destroy);
}
