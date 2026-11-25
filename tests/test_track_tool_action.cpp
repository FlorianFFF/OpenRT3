// In-game differential coverage for GPT6.1 SOL MEDIUM's 0x50d740 rewrite.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall TrackTool_Bulldoze(uint8_t* tool, int, uint32_t, uint32_t);
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
}

namespace {
template <typename T> T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t bytes[5]; } patches[48];
    int count = 0;
    void AddOne(uintptr_t address, void* fn) {
        Patch& p = patches[count++];
        p.at = (uint8_t*)address;
        DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.bytes, p.at, 5);
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
            memcpy(p.at, p.bytes, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
const uint32_t kGlobals[] = {
    0x622b1c, 0x622b20, 0x6cfca8, 0x6cec78, 0x62c120, 0xccbb20,
    0x6cd8d8, 0x6cec7c, 0x6d401c, 0x6d1a90, 0x6d40d0
};
const uint32_t kFloats[] = {
    0, 0x80000000, 0x3f800000, 0xbf800000, 0x7f800000, 0xff800000,
    0x7fc12345, 0x7f812345, 0x00800000, 1, 0x4f000000, 0x3e395810
};
struct Fixture {
    uint8_t tool[0xb0], options[0x90];
    uint32_t globals[sizeof kGlobals / sizeof kGlobals[0]];
    uint32_t log[512], packet[4];
    char tooltip[1400];
    int used, mp, count;
};
Fixture g_f;
uint32_t g_bits, g_case;
const char kNumber[] = "NUM";
const char kFormat[] = "cost=%1";
void Event(uint32_t a, uint32_t b = 0, uint32_t c = 0) {
    g_f.log[g_f.used++] = a; g_f.log[g_f.used++] = b; g_f.log[g_f.used++] = c;
}
uint32_t Bits(float f) { return *(uint32_t*)&f; }
int32_t __stdcall Focus(float* x, float* y) {
    Event(0x448ac0, kFloats[(g_case / 3) % 12], kFloats[(g_case / 13) % 12]);
    *(uint32_t*)x = kFloats[(g_case / 3) % 12];
    *(uint32_t*)y = kFloats[(g_case / 13) % 12];
    if (g_bits & 0x4000000) g_f.tool[0x7d] ^= 1;
    return 1;
}
double __stdcall Distance(float x, float y, float oldX, float oldY) {
    Event(0x51db80, Bits(x), Bits(y));
    Event(0x51db81, Bits(oldX), Bits(oldY));
    float cutoff = (float)*(const double*)0x5c97c0;
    uint32_t t = Bits(cutoff);
    uint32_t candidates[] = { 0xbf800000, 0, t - 1, t, t + 1, 0x3f800000, 0x7f800000, 0x7fc12345 };
    uint32_t b = candidates[(g_case / 4) % 8];
    return (double)*(float*)&b;
}
int32_t Mp() {
    Event(0x434050, g_f.mp, g_f.tool[0x7d]);
    return (g_bits >> (10 + g_f.mp++)) & 1;
}
void __fastcall Clear(void* tracks) {
    Event(0x4a3db0, (uint32_t)(uintptr_t)tracks);
    if (g_bits & 0x80000000) ++g_f.count;
}
void __fastcall Motion(void* tracks) {
    Event(0x495480, (uint32_t)(uintptr_t)tracks);
    if (g_bits & 0x8000) g_f.count += 2;
}
int32_t __fastcall Count(void* tracks) {
    Event(0x517cf0, (uint32_t)(uintptr_t)tracks, g_f.count);
    return g_f.count;
}
uint32_t __fastcall Company(void* world) {
    Event(0x4337a0, (uint32_t)(uintptr_t)world, F<uint32_t>(g_f.tool, 0x83));
    return g_bits ^ 0xfedcba98;
}
int32_t __fastcall Bulldoze(void* map, int, float x, float y, float r, uint32_t company, int32_t preview, float* cost, char* message) {
    Event(0x44b160, (uint32_t)(uintptr_t)map, company);
    Event(Bits(x), Bits(y), Bits(r));
    Event((uint32_t)preview);
    *(uint32_t*)cost = kFloats[(g_case / 9) % 12];
    strcpy(message, "rejected");
    if (g_bits & 0x10000) ++g_f.count;
    return (g_bits >> 5) & 1;
}
void __fastcall Cursor(void* cursor, int, int32_t idx, int32_t force, int32_t now) {
    Event(0x53f000, (uint32_t)(uintptr_t)cursor, (uint32_t)idx);
    Event((uint32_t)force, (uint32_t)now, F<uint32_t>(g_f.tool, 0x83));
}
uint32_t Frame() { Event(0x5216c0); return g_bits ^ 0xabababab; }
void __fastcall Send(void* session, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t limit) {
    Event(0x469d30, (uint32_t)(uintptr_t)session, (uint32_t)(uintptr_t)to);
    Event(type, (uint32_t)size, flags);
    Event((uint32_t)limit, g_f.tool[0x7d], F<uint32_t>(g_f.tool, 0x83));
    memcpy(g_f.packet, data, sizeof g_f.packet);
    if (g_bits & 0x20000) ++g_f.count;
}
char* __fastcall Number(char* buffer, int32_t style, double value, int32_t width) {
    uint64_t raw;
    memcpy(&raw, &value, sizeof raw);
    Event(0x51bfe0, (uint32_t)style, (uint32_t)width);
    Event((uint32_t)raw, (uint32_t)(raw >> 32));
    strcpy(buffer, kNumber);
    return buffer;
}
const char* __fastcall String(int32_t id) { Event(0x5193f0, (uint32_t)id); return kFormat; }
char* __fastcall Format(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                        const char* a5, const char* a6, const char* a7, const char* a8) {
    Event(0x518de0, fmt == kFormat ? 1 : (uint32_t)(uintptr_t)fmt, a1 ? (uint32_t)strcmp(a1, kNumber) : 100);
    Event((uint32_t)(uintptr_t)a2, (uint32_t)(uintptr_t)a3, (uint32_t)(uintptr_t)a4);
    Event((uint32_t)(uintptr_t)a5, (uint32_t)(uintptr_t)a6, (uint32_t)(uintptr_t)a7);
    Event((uint32_t)(uintptr_t)a8);
    strcpy(dst, fmt == kFormat ? "cost=NUM" : "");
    return dst;
}
int32_t __fastcall Tooltip(void* ui, int, const char* text, int32_t priority, int32_t sticky, int32_t data) {
    Event(0x538c70, (uint32_t)(uintptr_t)ui, (uint32_t)priority);
    Event((uint32_t)sticky, (uint32_t)data, g_f.tool[0x7d]);
    strcpy(g_f.tooltip, text);
    return 77;
}
void Setup(uint32_t it) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it;
    g_bits = it * 1664525u + 1013904223u;
    F<uint32_t>(g_f.tool, 0x83) = g_bits & 1;
    g_f.tool[0x7d] = (uint8_t)((g_bits >> 1) & 1);
    g_f.options[0x82] = (uint8_t)((g_bits >> 7) & 1);
    g_f.count = (int)((g_bits >> 24) & 7);
    *(uint32_t*)0x622b1c = kFloats[(it / 17) % 12];
    *(uint32_t*)0x622b20 = kFloats[(it / 23) % 12];
    *(uint32_t*)0x6cfca8 = 0xfca8;
    *(uint32_t*)0x6cec78 = 0xcec78;
    *(uint32_t*)0x62c120 = 0xc120;
    *(uint32_t*)0xccbb20 = 0xbb20;
    *(uint32_t*)0x6cd8d8 = 0xd8d8;
    *(void**)0x6cec7c = g_f.options;
    *(uint32_t*)0x6d401c = 0x401c;
    *(uint32_t*)0x6d1a90 = (g_bits >> 28) & 1;
    *(uint32_t*)0x6d40d0 = 0x40d0;
}
} // namespace

DIFFTEST(game_TrackToolBulldoze) {
    Redirects p;
    p.Add(0x448ac0, (void*)&Focus);
    p.Add(0x51db80, (void*)&Distance);
    p.Add(0x434050, (void*)&Mp);
    p.Add(0x4a3db0, (void*)&Clear);
    p.Add((uintptr_t)&rt3::TrackList_ResetBuildState, (void*)&Clear);
    p.Add(0x495480, (void*)&Motion);
    p.Add(0x517cf0, (void*)&Count);
    p.Add(0x4337a0, (void*)&Company);
    p.Add(0x44b160, (void*)&Bulldoze);
    p.Add(0x53f000, (void*)&Cursor);
    p.Add(0x5216c0, (void*)&Frame);
    p.Add(0x469d30, (void*)&Send);
    p.Add(0x51bfe0, (void*)&Number);
    p.Add(0x5193f0, (void*)&String);
    p.Add(0x518de0, (void*)&Format);
    p.Add(0x538c70, (void*)&Tooltip);
    uint32_t saved[sizeof kGlobals / sizeof kGlobals[0]];
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    for (uint32_t it = 0; it < 4096; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it);
            auto fn = side ? rt3::TrackTool_Bulldoze : (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))0x50d740;
            fn(g_f.tool, 0, g_bits, ~g_bits);
            for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t offset = 0;
                while (((uint8_t*)&expected)[offset] == ((uint8_t*)&g_f)[offset]) ++offset;
                EXPECT_EQ(1, 0, "bulldoze case %u byte %u: %02x / %02x (events %d / %d)",
                          it, (unsigned)offset, ((uint8_t*)&expected)[offset], ((uint8_t*)&g_f)[offset],
                          expected.used, g_f.used);
            }
        }
    }
    for (int i = 0; i < sizeof kGlobals / sizeof kGlobals[0]; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}
