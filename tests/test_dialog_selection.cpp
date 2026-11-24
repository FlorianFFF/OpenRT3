// Native in-process oracle for 0x4ef090, reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace rt3 { void __fastcall MpDialog_SelectMode(uint8_t*, int, int32_t); }
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } p[8];
    int n = 0;
    void Add(uintptr_t addr, void* target) {
        Patch& q = p[n++]; q.at = (uint8_t*)addr; DWORD old;
        VirtualProtect(q.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(q.old, q.at, 5); q.at[0] = 0xe9;
        F<int32_t>(q.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
        VirtualProtect(q.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), q.at, 5);
    }
    ~Redirects() {
        while (n) {
            Patch& q = p[--n]; DWORD old;
            VirtualProtect(q.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(q.at, q.old, 5); VirtualProtect(q.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), q.at, 5);
        }
    }
};
struct Fixture {
    uint8_t dialog[0xa0], sessions[2][0x9860], providers[2][16];
    char address[128];
    uint32_t words[4096], used, events, choices, fault, faultKind, faultAddress, globals[2];
};
Fixture g_f;
uint32_t g_case, g_bits, g_language, g_clock;
int32_t g_choice;
uint32_t Hash(const char* s) {
    if (!s) return 0;
    uint32_t h = 2166136261u;
    do { h = (h ^ (uint8_t)*s) * 16777619u; } while (*s++);
    return h;
}
void W(uint32_t v) { g_f.words[g_f.used++] = v; }
void Observe(uint32_t addr, void* self = nullptr, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0, uint32_t e = 0) {
    W(addr); W((uint32_t)(uintptr_t)self); W(a); W(b); W(c); W(d); W(e);
    W(F<uint32_t>(g_f.dialog, 0x7c)); W(F<uint32_t>(g_f.dialog, 0x80));
    W(*(uint32_t*)0x6cd8d8); W(*(uint32_t*)0x6d40d0);
    W(F<uint32_t>(g_f.sessions[0], 0x985c)); W(F<uint32_t>(g_f.sessions[1], 0x985c));
    W(g_f.events); W(g_f.choices);
}
void __fastcall Event(uint8_t* self, int, uint32_t message, uint32_t id, uint32_t value, uint32_t d, uint32_t e) {
    Observe(0x540120, self, message, id, message == 0x6f && id == 0xe46 ? Hash((char*)(uintptr_t)value) : value, d, e);
    ++g_f.events;
    if (id == 0xe10 && (g_bits & 1)) F<uint32_t>(self, 0x7c) = 0xe12;
    if (id == 0xe11 && (g_bits & 2)) *(void**)0x6d40d0 = g_f.providers[1];
    if (id == 0xe35 && (g_bits & 4)) *(void**)0x6cd8d8 = g_f.sessions[1];
    if (id == 0xe55 && (g_bits & 8)) F<uint32_t>(self, 0x80) = 0x11223344;
}
char* __fastcall Address(void* self, int, char* out) {
    Observe(0x5216f0, self);
    memset(out, 'L', 127); out[127] = 0;
    if (g_bits & 0x10) return g_f.address;
    if (g_case % 17 == 0) return nullptr;
    return out;
}
void __fastcall Setting(uint8_t* self) {
    Observe(0x4ed730, self);
    if (g_bits & 0x20) F<uint32_t>(self, 0x7c) = 0xe10;
}
void __fastcall Difficulty(uint8_t* self) {
    Observe(0x4ed790, self);
    if (g_bits & 0x40) F<uint32_t>(g_f.sessions[0], 0x985c) ^= 1;
}
void __fastcall Source(uint8_t* self, int, int32_t choice) {
    Observe(0x4eee00, self, (uint32_t)choice); ++g_f.choices;
    if (choice == 0xe15 && (g_bits & 0x80))
        *(void**)0x6cd8d8 = (g_case / 7) % 4 == 0 ? nullptr : g_f.sessions[1];
    F<uint32_t>(self, 0x78) = (uint32_t)choice;
}
uint32_t __cdecl English() { Observe(0x51b6f0); return g_language; }
uint32_t __cdecl Clock() {
    Observe(0x51d890);
    F<uint32_t>(g_f.dialog, 0x80) = ~g_clock;
    return g_clock;
}
void Setup(uint32_t it) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it; g_bits = it * 1664525u + 1013904223u;
    const int32_t choices[] = {-1, 0xe10, 0xe11, 0xe12, 0xe13, 0, 0x7fffffff};
    const uint32_t languages[] = {0, 1, 0x100, 0xffff0000, 0xffff00ff, 0x80000001};
    g_choice = choices[it % 7]; g_language = languages[(it / 7) % 6]; g_clock = g_bits ^ 0x934d218c;
    for (int i = 0; i < 0xa0; ++i) g_f.dialog[i] = (uint8_t)(g_bits >> (i % 24));
    F<uint32_t>(g_f.sessions[0], 0x985c) = (it / 11) % 2 ? 0x80000000 : 0;
    F<uint32_t>(g_f.sessions[1], 0x985c) = (it / 13) % 3;
    sprintf(g_f.address, "Returned %u", it);
    *(void**)0x6cd8d8 = g_f.sessions[0]; *(void**)0x6d40d0 = g_f.providers[0];
}
int Catch(EXCEPTION_POINTERS* e) {
    g_f.fault = e->ExceptionRecord->ExceptionCode;
    if (g_f.fault == EXCEPTION_ACCESS_VIOLATION) {
        g_f.faultKind = (uint32_t)e->ExceptionRecord->ExceptionInformation[0];
        g_f.faultAddress = (uint32_t)e->ExceptionRecord->ExceptionInformation[1];
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
void Invoke(void* fn) {
    __try { ((void(__fastcall*)(uint8_t*, int, int32_t))fn)(g_f.dialog, 0, g_choice); }
    __except(Catch(GetExceptionInformation())) {}
}
}
DIFFTEST(game_MpDialogSelectMode) {
    Redirects p;
    p.Add(0x540120, (void*)&Event); p.Add(0x5216f0, (void*)&Address); p.Add(0x4ed730, (void*)&Setting);
    p.Add(0x4ed790, (void*)&Difficulty); p.Add(0x4eee00, (void*)&Source); p.Add(0x51b6f0, (void*)&English);
    p.Add(0x51d890, (void*)&Clock);
    uint32_t saved[] = {*(uint32_t*)0x6cd8d8, *(uint32_t*)0x6d40d0};
    for (uint32_t it = 0; it < 4096; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it);
            Invoke(side ? (void*)&rt3::MpDialog_SelectMode : (void*)0x4ef090);
            g_f.globals[0] = *(uint32_t*)0x6cd8d8; g_f.globals[1] = *(uint32_t*)0x6d40d0;
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t start = (uint8_t*)g_f.words - (uint8_t*)&g_f;
                uint32_t word = off >= start && off < start + sizeof g_f.words ? (uint32_t)((off - start) / 4) : 0;
                EXPECT_EQ(1, 0, "mode case %u byte %u %02x/%02x word %u %08x/%08x, trace %u/%u fault %08x/%08x",
                    it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word, expected.words[word], g_f.words[word],
                    expected.used, g_f.used, expected.fault, g_f.fault);
                it = 4096;
                break;
            }
        }
    }
    *(uint32_t*)0x6cd8d8 = saved[0]; *(uint32_t*)0x6d40d0 = saved[1];
}
