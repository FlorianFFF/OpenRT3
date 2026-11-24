// In-process dialog fixtures for functions reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace rt3 {
void __fastcall HighScores_Show(int32_t, int32_t, int32_t);
void __fastcall MpScenario_SelectSource(uint8_t*, int, int32_t);
}
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } entries[24];
    int count = 0;
    void Add(uintptr_t addr, void* target) {
        Patch& p = entries[count++]; p.at = (uint8_t*)addr; DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    ~Redirects() {
        while (count) {
            Patch& p = entries[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
const uint32_t kGlobals[] = {
    0x6cec78, 0x6cec7c, 0x6cec74, 0x6d401c, 0x6d0818, 0x6d1078,
    0x6d1088, 0x6d1090, 0x6d4000, 0x6d126c, 0x6d40dc, 0x6ce9bc, 0x6d1291
};
struct Fixture {
    uint8_t dialog[0xa0], options[2][0x100], apps[2][0x80], views[2][0x10];
    uint8_t managers[2][0x10], worlds[2][0x10], companies[2][0x140];
    uint8_t blocks[3][0x100], lists[2][0x10], items[2][0x180];
    char fallback[300], name[300], scenario[300];
    uint8_t scores[0x850];
    uint32_t globals[13], log[16384], used;
    uint32_t companyCalls, events, copies, allocations, frees, adds;
    uint32_t fault, faultKind, faultAddress;
};
Fixture g_f;
uint32_t g_bits, g_case;
int g_kind;
int32_t g_value, g_extra, g_keep, g_choice;
uint32_t Hash(const void* p, size_t n) {
    uint32_t h = 2166136261u;
    const uint8_t* s = (const uint8_t*)p;
    while (n--) h = (h ^ *s++) * 16777619u;
    return h;
}
uint32_t Text(const char* p) { return p ? Hash(p, strlen(p) + 1) : 0; }
uint32_t Id(void* p) {
    if (!p) return 0;
    if (p == g_f.dialog) return 1;
    for (int i = 0; i < 2; ++i) {
        if (p == g_f.options[i]) return 10 + i;
        if (p == g_f.apps[i]) return 20 + i;
        if (p == g_f.views[i]) return 30 + i;
        if (p == g_f.managers[i]) return 40 + i;
        if (p == g_f.worlds[i]) return 50 + i;
        if (p == g_f.companies[i]) return 60 + i;
        if (p == g_f.lists[i]) return 70 + i;
    }
    for (int i = 0; i < 3; ++i) if (p == g_f.blocks[i]) return 80 + i;
    return (uint32_t)(uintptr_t)p;
}
void Word(uint32_t v) { g_f.log[g_f.used++] = v; }
void Observe(uint32_t address, void* self = nullptr, uint32_t a = 0, uint32_t b = 0,
             uint32_t c = 0, uint32_t d = 0, uint32_t e = 0, uint32_t f = 0) {
    Word(address); Word(Id(self)); Word(a); Word(b); Word(c); Word(d); Word(e); Word(f);
    for (uint32_t slot : kGlobals) Word(*(uint32_t*)slot);
    Word(F<uint32_t>(g_f.dialog, 0x78)); Word(F<uint32_t>(g_f.dialog, 0x7c));
    for (auto& o : g_f.options) {
        Word(o[0x10]); Word(F<uint16_t>(o, 0x77)); Word(o[0xc5]);
        Word(Text((char*)o + 0x11)); Word(Text((char*)o + 0x44));
    }
    for (auto& a0 : g_f.apps) Word(F<uint32_t>(a0, 0x6c));
    for (auto& l : g_f.lists) { Word(F<uint32_t>(l, 0)); Word(F<uint32_t>(l, 8)); }
    Word(Hash((void*)0x6d0828, 0x850));
    Word(g_f.companyCalls); Word(g_f.events); Word(g_f.copies);
    Word(g_f.allocations); Word(g_f.frees); Word(g_f.adds);
}
void __cdecl ScoresLoad() {
    Observe(0x4e1840);
    *(uint32_t*)0x6d0828 = g_case;
    *(uint32_t*)0x6d1074 = g_bits;
    *(uint32_t*)0x6d1088 = 0xdeadbeef;
    if (g_bits & 0x1000000) *(void**)0x6d0818 = g_f.views[1];
}
uint8_t* __fastcall Company(uint8_t* world) {
    Observe(0x4348c0, world); ++g_f.companyCalls;
    if (g_f.companyCalls == 1) {
        if (g_bits & 2) *(void**)0x6cec78 = g_f.worlds[1];
        if (g_bits & 1) return nullptr;
    }
    if (g_f.companyCalls == 2 && (g_bits & 0x10))
        *(void**)0x6cec7c = g_case & 1 ? nullptr : g_f.options[1];
    return g_f.companies[(g_bits & 4) && g_f.companyCalls == 2 ? 1 : 0];
}
const char* __fastcall GetString(int32_t id) {
    Observe(0x5193f0, nullptr, (uint32_t)id);
    return g_f.fallback;
}
int32_t __fastcall AddScore(int32_t list, const char* name, const char* scenario, int32_t value,
                           int32_t n2, int32_t n3, int32_t n4, int32_t extra, int32_t quiet) {
    Observe(0x4e1650, nullptr, (uint32_t)list, Text(name), Text(scenario), (uint32_t)value, (uint32_t)n2, (uint32_t)n3);
    Word((uint32_t)n4); Word((uint32_t)extra); Word((uint32_t)quiet);
    ++g_f.adds;
    strcpy(g_f.name, name); strcpy(g_f.scenario, scenario);
    *(uint32_t*)0x6d1090 = 3;
    if (g_bits & 0x20) *(uint32_t*)0x6d4000 ^= 1;
    if (g_bits & 0x40) *(void**)0x6d0818 = g_case & 1 ? nullptr : g_f.views[1];
    return 4;
}
void __fastcall Mode(void* view, int, int32_t mode, int32_t id) {
    Observe(0x4ddbd0, view, (uint32_t)mode, (uint32_t)id);
}
void* __cdecl Allocate(uint32_t size) {
    Observe(0x53b070, nullptr, size); ++g_f.allocations;
    if (g_kind == 1 && (g_bits & 0x80000)) *(void**)0x6d126c = g_f.lists[1];
    if (g_case % 64 == (g_kind == 0 ? 7 : 9)) return nullptr;
    return g_f.blocks[0];
}
uint8_t* __fastcall ScoreCtor(uint8_t* mem) {
    Observe(0x4e19a0, mem);
    F<uint32_t>(mem, 4) = g_bits;
    if (g_case % 64 == 13) return nullptr;
    return g_bits & 0x80 ? g_f.blocks[1] : mem;
}
void __fastcall PausePush(void* app) {
    Observe(0x483450, app);
    if (g_bits & 0x80) *(void**)0x6d401c = g_f.managers[1];
}
void __fastcall Modal(void* manager, int, void* dialog, int32_t a, int32_t b, int32_t c, int32_t d) {
    Observe(0x538f10, manager, Id(dialog), (uint32_t)a, (uint32_t)b, (uint32_t)c, (uint32_t)d);
    if (g_bits & 0x100) *(void**)0x6d1078 = g_f.blocks[2];
    *(void**)0x6cec74 = g_f.apps[1];
}
void __fastcall PausePop(void* app) {
    Observe(0x4834a0, app);
    if (g_bits & 0x200) *(void**)0x6d1078 = g_case & 1 ? nullptr : g_f.blocks[1];
}
void __fastcall Remove(void* manager, int, void* dialog) {
    Observe(0x5389c0, manager, Id(dialog));
    if (g_bits & 0x400) *(void**)0x6d1078 = g_f.blocks[2];
}
void __fastcall ScoreDtor(void* dialog) {
    Observe(0x4e1a70, dialog);
    if (g_bits & 0x800) *(void**)0x6d1078 = g_f.blocks[0];
}
void __cdecl Free(void* p) { Observe(0x53b080, p); ++g_f.frees; }
void __fastcall Event(uint8_t* dialog, int, uint32_t message, uint32_t id, uint32_t value, uint32_t d, uint32_t e) {
    uint32_t data = message == 0x6f && id == 0xe2f ? Text((const char*)(uintptr_t)value) : value;
    Observe(0x540120, dialog, message, id, data, d, e); ++g_f.events;
    if (id == 0xe18 && (g_bits & 0x1000)) F<uint32_t>(dialog, 0x7c) ^= 1;
    if (id == 0xe17 && (g_bits & 0x2000)) *(uint32_t*)0x6ce9bc = 1;
    if (id == 0xe24 && (g_bits & 0x4000)) F<uint32_t>(dialog, 0x78) = 0xe16;
    if (id == 0xe14 && (g_bits & 0x8000)) *(uint8_t*)0x6d1291 ^= 1;
    if (id == 0xe2f && (g_bits & 0x10000)) *(void**)0x6cec74 = g_f.apps[1];
}
char* __fastcall Number(char* out, int32_t style, double value, int32_t width, int32_t unused, int32_t inK, int32_t inM) {
    uint32_t raw[2]; memcpy(raw, &value, 8);
    Observe(0x51b700, nullptr, (uint32_t)style, raw[0], raw[1], (uint32_t)width, (uint32_t)unused, (uint32_t)inK);
    Word((uint32_t)inM);
    sprintf(out, "N%u", (unsigned)value);
    if (g_bits & 0x20000) *(void**)0x6cec7c = g_f.options[1];
    return g_f.fallback;
}
void __fastcall AppMode(void* app, int, int32_t mode) {
    Observe(0x482e50, app, (uint32_t)mode);
    if (g_bits & 0x40000) *(void**)0x6d126c = g_case % 3 ? nullptr : g_f.lists[0];
}
uint8_t* __fastcall ListCtor(uint8_t* mem, int, int32_t kind) {
    Observe(0x4336a0, mem, (uint32_t)kind);
    uint8_t* ret = g_bits & 0x100000 ? g_f.lists[0] : mem;
    F<int32_t>(ret, 0) = (int32_t)(g_case % 4) - 1;
    F<void*>(ret, 8) = g_f.items[0];
    if (g_bits & 0x200000) *(void**)0x6d126c = g_f.lists[1];
    return g_case % 64 == 11 ? nullptr : ret;
}
void __fastcall Copy(char* dest, const char* source, uint32_t n) {
    Observe(0x51d8a0, dest, (uint32_t)(uintptr_t)source, Text(source), n); ++g_f.copies;
    strncpy(dest, source, n); dest[n] = '\0';
    if (g_f.copies == 1 && (g_bits & 0x400000)) {
        *(void**)0x6d126c = g_f.lists[1];
        *(void**)0x6cec7c = g_f.options[1];
    }
    if (g_f.copies == 2 && (g_bits & 0x800000)) *(uint8_t*)0x6d1291 ^= 1;
}
void __fastcall Panel(uint8_t* dialog, int, const char* path, const char* name, int32_t force) {
    Observe(0x4ee950, dialog, Text(path), Text(name), (uint32_t)force);
}
void __fastcall Fill(uint8_t* dialog) { Observe(0x4eed30, dialog); }

void Setup(uint32_t it, int kind) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it; g_kind = kind;
    uint32_t r = it * 2654435761u + 0x9d1a;
    for (uint8_t* p = (uint8_t*)&g_f; p != (uint8_t*)g_f.fallback; ++p) {
        r = r * 1664525u + 1013904223u; *p = (uint8_t)(r >> 16);
    }
    g_bits = r;
    const int32_t values[] = {(int32_t)0x80000000, -2, -1, 0, 1, 7, 0x7fffffff};
    const int32_t choices[] = {-1, 0xe15, 0xe16, 0, 12345};
    g_value = values[it % 7]; g_extra = (int32_t)r; g_keep = (it / 7) % 3 ? -1 : 0;
    g_choice = choices[it % 5];
    strcpy(g_f.fallback, "Fallback player");
    for (int i = 0; i < 2; ++i) {
        char* name = (char*)g_f.companies[i] + 8;
        uint32_t len = (it + i) % 4 == 0 ? 299 : (it + i) % 4 == 1 ? 0 : 13 + i;
        for (uint32_t k = 0; k < len; ++k) name[k] = (char)('A' + (it + i + k) % 26);
        name[len] = '\0';
        sprintf((char*)g_f.options[i] + 0x11, "Path %u-%d", it, i);
        sprintf((char*)g_f.options[i] + 0x44, "Scenario %u-%d", it, i);
        g_f.options[i][0x10] = (it + i) % 2 ? 255 : 0;
        g_f.options[i][0xc5] = (it + i) % 3 ? 255 : 0;
        for (int k = 0; k < 16; ++k) g_f.options[i][0xc6 + k] = (uint8_t)((it + i + k) % 6);
        F<uint16_t>(g_f.options[i], 0x77) = (uint16_t)(it * 37 + i);
        F<int32_t>(g_f.lists[i], 0) = i ? 2 : (int32_t)(it % 4) - 1;
        F<void*>(g_f.lists[i], 8) = g_f.items[i];
        sprintf((char*)g_f.items[i], "Save path %u-%d", it, i);
        sprintf((char*)g_f.items[i] + 0x12d, "Save name %u-%d", it, i);
    }
    F<uint32_t>(g_f.dialog, 0x7c) = it & 1 ? 0xe12 : 0xe13;
    memset((void*)0x6d0828, 0xa5, 0x850);
    *(void**)0x6cec78 = it % 4 ? g_f.worlds[0] : nullptr;
    *(void**)0x6cec7c = kind == 0 && it % 4 == 1 ? nullptr : g_f.options[0];
    *(void**)0x6cec74 = g_f.apps[0]; *(void**)0x6d401c = g_f.managers[0];
    *(void**)0x6d0818 = it & 1 ? nullptr : g_f.views[0];
    *(void**)0x6d1078 = g_f.blocks[2];
    *(uint32_t*)0x6d1088 = 0x99887766;
    *(uint32_t*)0x6d1090 = 0x12345678;
    *(uint32_t*)0x6d4000 = (it >> 2) & 1;
    *(void**)0x6d126c = it % 3 ? nullptr : g_f.lists[0];
    *(uint32_t*)0x6d40dc = (it >> 3) & 1;
    *(uint32_t*)0x6ce9bc = (it >> 4) % 3;
    *(uint32_t*)0x6d1291 = 0xaabbcc00 | ((it >> 5) & 1);
}
int Catch(EXCEPTION_POINTERS* e) {
    g_f.fault = e->ExceptionRecord->ExceptionCode;
    if (g_f.fault == EXCEPTION_ACCESS_VIOLATION) {
        g_f.faultKind = (uint32_t)e->ExceptionRecord->ExceptionInformation[0];
        g_f.faultAddress = (uint32_t)e->ExceptionRecord->ExceptionInformation[1];
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
void Invoke(void* fn, int kind) {
    __try {
        if (!kind) ((void(__fastcall*)(int32_t, int32_t, int32_t))fn)(g_value, g_extra, g_keep);
        else ((void(__fastcall*)(uint8_t*, int, int32_t))fn)(g_f.dialog, 0, g_choice);
    } __except(Catch(GetExceptionInformation())) {}
}
void Cases(int kind) {
    uint32_t saved[13]; uint8_t scores[0x850];
    for (int i = 0; i < 13; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    memcpy(scores, (void*)0x6d0828, sizeof scores);
    for (uint32_t it = 0; it < 4096; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, kind);
            void* original = kind ? (void*)0x4eee00 : (void*)0x4e1af0;
            void* replacement = kind ? (void*)&rt3::MpScenario_SelectSource : (void*)&rt3::HighScores_Show;
            Invoke(side ? replacement : original, kind);
            for (int i = 0; i < 13; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            memcpy(g_f.scores, (void*)0x6d0828, sizeof g_f.scores);
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t logOff = (uint8_t*)g_f.log - (uint8_t*)&g_f;
                uint32_t word = off >= logOff && off < logOff + sizeof g_f.log ? (uint32_t)((off - logOff) / 4) : 0;
                EXPECT_EQ(1, 0, "dialog %d case %u byte %u (%02x/%02x), word %u %08x/%08x, log %u/%u fault %08x/%08x",
                          kind, it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word,
                          expected.log[word], g_f.log[word], expected.used, g_f.used, expected.fault, g_f.fault);
                it = 4096;
                break;
            }
        }
    }
    memcpy((void*)0x6d0828, scores, sizeof scores);
    for (int i = 0; i < 13; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}
void Install(Redirects& p) {
    p.Add(0x4e1840, (void*)&ScoresLoad); p.Add(0x4348c0, (void*)&Company); p.Add(0x5193f0, (void*)&GetString);
    p.Add(0x4e1650, (void*)&AddScore); p.Add(0x4ddbd0, (void*)&Mode); p.Add(0x53b070, (void*)&Allocate);
    p.Add(0x4e19a0, (void*)&ScoreCtor); p.Add(0x483450, (void*)&PausePush); p.Add(0x538f10, (void*)&Modal);
    p.Add(0x4834a0, (void*)&PausePop); p.Add(0x5389c0, (void*)&Remove); p.Add(0x4e1a70, (void*)&ScoreDtor);
    p.Add(0x53b080, (void*)&Free); p.Add(0x540120, (void*)&Event); p.Add(0x51b700, (void*)&Number);
    p.Add(0x482e50, (void*)&AppMode); p.Add(0x4336a0, (void*)&ListCtor); p.Add(0x51d8a0, (void*)&Copy);
    p.Add(0x4ee950, (void*)&Panel); p.Add(0x4eed30, (void*)&Fill);
}
}
DIFFTEST(game_HighScoresShow) { Redirects p; Install(p); Cases(0); }
DIFFTEST(game_MpScenarioSelectSource) { Redirects p; Install(p); Cases(1); }
