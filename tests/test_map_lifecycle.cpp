// Map lifecycle fixtures for functions reversed by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall Map_InitRuntimeState(uint8_t*);
void __fastcall Map_UnloadWorld(uint8_t*);
}
namespace {
template <typename T> T& F(void* p, uint32_t o) { return *(T*)((uint8_t*)p + o); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } patches[24];
    int count = 0;
    void Add(uintptr_t address, void* fn) {
        Patch& p = patches[count++]; p.at = (uint8_t*)address; DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)fn - address - 5);
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    ~Redirects() {
        while (count) {
            Patch& p = patches[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
struct Slot { uint32_t address, destructor; };
const Slot kSlots[] = {
    {0x6ada80, 0x461560}, {0x6cea50, 0x474480}, {0x62ba84, 0x41d320},
    {0x6ada90, 0x468140}, {0x62b26c, 0x413260}, {0x6cec20, 0x4811f0},
    {0x62bae0, 0x421470}, {0x6cfca8, 0x493ba0}, {0x6cfcb4, 0x4a41a0},
    {0x6cfc9c, 0x487c00}, {0x62be18, 0x432e80}, {0x62be68, 0x43a560}
};
const uint32_t kGlobals[] = {
    0x6ada80, 0x6cea50, 0x62ba84, 0x6ada90, 0x62b26c, 0x6cec20, 0x62bae0,
    0x6cfca8, 0x6cfcb4, 0x6cfc9c, 0x62be18, 0x62be68, 0x62c120, 0x6cec74, 0x6cec78
};
struct Fixture {
    uint8_t map[0x2220];
    uint32_t globals[sizeof kGlobals / sizeof kGlobals[0]], log[8192];
    int used, pumps;
};
Fixture g_f;
uint32_t g_bits, g_case;
void Word(uint32_t value) { g_f.log[g_f.used++] = value; }
void Snapshot(uint32_t address, void* self = nullptr, uint32_t a = 0, uint32_t b = 0) {
    Word(address); Word((uint32_t)(uintptr_t)self); Word(a); Word(b);
    Word(F<uint32_t>(g_f.map, 0));
    for (uint32_t slot : kGlobals) Word(*(uint32_t*)slot);
    // Exact byte snapshots at every callback, not only a final-state checksum.
    for (uint32_t off = 0x2100; off < sizeof g_f.map; off += 4) Word(F<uint32_t>(g_f.map, off));
}
void __fastcall Pump(void* context, int, int32_t step, int32_t force) {
    Snapshot(0x4834e0, context, (uint32_t)step, (uint32_t)force);
    ++g_f.pumps;
    if (g_bits & 0x10000) F<uint32_t>(g_f.map, 0x2135) ^= 0x12345678;
    if (g_bits & 0x20000) *(uint32_t*)0x6cec74 ^= 0x40;
    if (g_bits & 0x40000) *(uint32_t*)kSlots[(g_case + g_f.pumps) % 12].address = 0xf00000 + g_f.pumps;
    if (g_bits & 0x80000) *(uint32_t*)0x62c120 = 0xfaceb00c;
}
template <uint32_t Address> void __fastcall Method(void* self) {
    Snapshot(Address, self);
    for (int i = 0; i < 12; ++i) if (kSlots[i].destructor == Address) {
        if (g_bits & 0x100000) *(uint32_t*)kSlots[i].address = 0xd00000 + i;
        if (g_bits & 0x200000) *(uint32_t*)kSlots[(i + 1) % 12].address = 0xe00000 + i;
        if (g_bits & 0x400000) F<uint32_t>(g_f.map, 0x2125) = 0;
    }
    if (self == g_f.map) {
        F<uint32_t>(g_f.map, 0) ^= Address;
        if (g_bits & 0x800000) g_f.map[0x21c5] = 0xc5;
    }
}
template <uint32_t Address> void Call() { Snapshot(Address); }
void __cdecl Free(void* memory) { Snapshot(0x53b080, memory); }
void Setup(uint32_t it, int mode) {
    memset(&g_f, 0, sizeof g_f); g_case = it;
    uint32_t r = it * 2654435761u + 0xc0ffee;
    auto rnd = [&]() { r = r * 1103515245u + 12345u; return r; };
    g_bits = rnd();
    for (uint8_t& b : g_f.map) b = (uint8_t)(rnd() >> 16);
    if (mode == 1) F<uint32_t>(g_f.map, 0x2125) = (g_bits >> 21) & 1;
    for (int i = 0; i < 12; ++i) *(uint32_t*)kSlots[i].address = it & (1u << i) ? 0x100000 + i * 0x100 : 0;
    *(void**)0x62c120 = mode == 1 ? g_f.map : (uint8_t*)0xbad0000;
    *(uint32_t*)0x6cec74 = 0xcec74;
    *(uint32_t*)0x6cec78 = 0xcec78;
}
void RunCases(int mode, uintptr_t original, void* cpp, uint32_t count) {
    uint32_t saved[sizeof kGlobals / sizeof kGlobals[0]];
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    for (uint32_t it = 0; it < count; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, mode);
            ((void(__fastcall*)(uint8_t*))(side ? cpp : (void*)original))(g_f.map);
            for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                EXPECT_EQ(1, 0, "map lifecycle %d case %u byte %u: %02x / %02x (words %d / %d)",
                          mode, it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off],
                          expected.used, g_f.used);
            }
        }
    }
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}
}

DIFFTEST(game_MapInitRuntimeState) {
    Redirects p;
    p.Add(0x4834e0, (void*)&Pump);
    p.Add(0x532590, (void*)&Method<0x532590>);
    RunCases(0, 0x448260, (void*)&rt3::Map_InitRuntimeState, 1024);
}
DIFFTEST(game_MapUnloadWorld) {
    Redirects p;
    p.Add(0x4834e0, (void*)&Pump);
    p.Add(0x47bb90, (void*)&Call<0x47bb90>);
    p.Add(0x53b080, (void*)&Free);
#define DTOR(a) p.Add(a, (void*)&Method<a>)
    DTOR(0x461560); DTOR(0x474480); DTOR(0x41d320); DTOR(0x468140);
    DTOR(0x413260); DTOR(0x4811f0); DTOR(0x421470); DTOR(0x493ba0);
    DTOR(0x4a41a0); DTOR(0x487c00); DTOR(0x432e80); DTOR(0x43a560);
    DTOR(0x4539c0); DTOR(0x453510); DTOR(0x537330); DTOR(0x4483d0);
#undef DTOR
    p.Add(0x40b7d0, (void*)&Call<0x40b7d0>);
    RunCases(1, 0x449270, (void*)&rt3::Map_UnloadWorld, 4096);
}
