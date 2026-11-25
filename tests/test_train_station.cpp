// In-process oracle for 0x4ae250, reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace rt3 { int32_t __fastcall Train_ProcessStop(uint8_t*, int, int32_t); }
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t bytes[5]; } patches[64];
    int count = 0;
    void AddOne(uintptr_t address, void* target) {
        Patch& p = patches[count++]; p.at = (uint8_t*)address; DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.bytes, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)target - address - 5);
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
            Patch& p = patches[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.bytes, 5); VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
const uint32_t kSlots[] = {0x6cec20, 0x62ba84, 0x62be10, 0x6cec78, 0x6cec7c, 0x6acd34};
struct Fixture {
    uint8_t train[0x400], rows[2][6 * 0x33], stations[4][0x500], car[0x400];
    uint8_t companies[2][0x400], worlds[2][0x4b00], options[0x100];
    uint32_t tables[4], buildings[2], vtable[33];
    uint8_t attributes[2][0x90];
    char names[2][32], strings[3][16], message[200];
    uint32_t gets[4], currents, removes, valid, kinds, buildingCalls, active, resets, namesCalled, money, formats;
    uint32_t result, fault, faultKind, faultAddress, globals[6], used;
    uint16_t cw;
    uint32_t log[32768];
};
Fixture g_f;
uint32_t g_case, g_bits, g_payout;
int32_t g_force;
uint16_t g_cw;
const uint32_t kFloats[] = {
    0, 0x80000000, 0x3f800000, 0x3f000000, 0x43fa0000, 0x43fa8000,
    0x447a0000, 0x461c4000, 0x7fc12345, 0x7f801234, 0x7f800000,
    0xff800000, 1, 0xbf800000, 0x3f7fffff, 0x3f800001
};
uint32_t Hash(const char* p) {
    if (!p) return 0;
    uint32_t h = 2166136261u;
    do { h = (h ^ (uint8_t)*p) * 16777619u; } while (*p++);
    return h;
}
uint32_t Id(void* p) {
    if (!p) return 0;
    if (p == g_f.train) return 1;
    if (p == g_f.car) return 2;
    if (p == g_f.options) return 3;
    for (int i = 0; i < 4; ++i) {
        if (p == g_f.stations[i]) return 10 + i;
        if (p == &g_f.tables[i]) return 20 + i;
    }
    for (int i = 0; i < 2; ++i) {
        if (p == g_f.worlds[i]) return 30 + i;
        if (p == g_f.companies[i]) return 40 + i;
        if (p == &g_f.buildings[i]) return 50 + i;
    }
    return (uint32_t)(uintptr_t)p;
}
void Word(uint32_t v) { g_f.log[g_f.used++] = v; }
void Observe(uint32_t addr, void* self = nullptr, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0) {
    Word(addr); Word(Id(self)); Word(a); Word(b); Word(c); Word(d);
    for (uint32_t p : kSlots) Word(*(uint32_t*)p);
    const uint32_t fields[] = {0, 4, 8, 0x25, 0x39, 0x41, 0x51, 0x8c, 0x94, 0xa4, 0xa8, 0xac, 0xcc, 0xdc, 0xe0, 0xe4, 0x199, 0x19d, 0x1ba};
    for (uint32_t p : fields) Word(F<uint32_t>(g_f.train, p));
    Word(g_f.train[0xfb]); Word(g_f.train[0x189]); Word(g_f.train[0x1a1]); Word(g_f.train[0x1b8]);
    for (auto& co : g_f.companies) Word(F<uint32_t>(co, 0x347));
    Word(g_f.currents); Word(g_f.valid); Word(g_f.kinds); Word(g_f.active); Word(g_f.money);
    uint16_t cw; __asm fnstcw cw
    Word(cw);
}
uint8_t* __fastcall Current(uint8_t* t) {
    Observe(0x4b2bd0, t); ++g_f.currents;
    int32_t index = F<int32_t>(t, 8);
    return index < 0 ? nullptr : F<uint8_t*>(t, 0) + (index % 6) * 0x33;
}
int32_t __cdecl Multiplayer() { Observe(0x434050); return g_case % 3 != 0; }
int32_t __stdcall Version(float v) {
    Observe(0x46d2b0, nullptr, *(uint32_t*)&v);
    return (g_case / 3) & 1;
}
uint8_t* __fastcall Get(void* table, int, uint32_t id) {
    uint32_t tag = *(uint32_t*)table;
    Observe(0x518140, table, id); ++g_f.gets[tag];
    if (tag == 1) {
        if (g_f.gets[1] == 1 && (g_bits & 1)) F<int32_t>(g_f.train, 0x41) = -1;
        return g_case % 67 == 4 ? nullptr : g_f.car;
    }
    if (tag == 2) {
        if (g_f.gets[2] == 4 && (g_bits & 2)) F<uint32_t>(g_f.train, 0x199) = g_payout;
        return g_f.companies[(g_bits & 4) && g_f.gets[2] > 1 ? 1 : 0];
    }
    if (g_bits & 8) *(void**)0x6cec20 = &g_f.tables[3];
    return g_f.stations[(id + (tag == 3 ? 1 : 0)) % 4];
}
void __fastcall Move(uint8_t* car, int, uint32_t node, uint32_t position) {
    Observe(0x41c700, car, node, position);
    F<uint32_t>(car, 0x356) ^= 0x12345;
}
void __fastcall Pose(uint8_t* car, int, uint32_t a, uint32_t b, uint32_t c, int32_t d) {
    Observe(0x41ce80, car, a, b, c, (uint32_t)d);
    if (g_bits & 0x10) F<int32_t>(g_f.train, 8) = 1;
}
void __fastcall Align(uint8_t* t) {
    Observe(0x4a9840, t);
    if (g_bits & 0x20) F<void*>(t, 0) = g_f.rows[1];
}
void __fastcall Remove(uint8_t* t, int, int32_t index) {
    Observe(0x4b3000, t, (uint32_t)index); ++g_f.removes;
    --F<int32_t>(t, 4); F<int32_t>(t, 8) = 0;
    if (g_bits & 0x40) F<void*>(t, 0) = g_f.rows[1];
}
int32_t __fastcall Valid(void* table, int, uint32_t id) {
    Observe(0x517d40, table, id); ++g_f.valid;
    return id < 4 && !(id == 2 && (g_bits & 0x80));
}
void __fastcall SetCurrent(uint8_t* t, int, int32_t idx, int32_t mark) {
    Observe(0x4b2f00, t, (uint32_t)idx, (uint32_t)mark);
    F<int32_t>(t, 8) = idx;
}
void __fastcall Highlight(void* site, int, int32_t value) {
    Observe(0x47f290, site, (uint32_t)value);
    if (g_bits & 0x100) F<uint32_t>(g_f.train, 0xac) = (g_case + 1) % 4;
}
void* __fastcall Building(void* site) {
    Observe(0x47de00, site);
    return &g_f.buildings[(g_f.buildingCalls++) & 1];
}
uint8_t* __fastcall Attribute(void* building) {
    Observe(0x80000080, building); ++g_f.kinds;
    if (g_bits & 0x200) F<uint32_t>(g_f.train, 0x94) = g_case & 1 ? 7 : 0xffffffff;
    return g_f.attributes[g_f.kinds > 1 ? 1 : 0];
}
void* __fastcall CityRef(uint8_t* t) {
    Observe(0x4a9020, t);
    if (g_bits & 0x400) F<uint32_t>(t, 0x94) = 55;
    return g_case & 1 ? g_f.stations[0] : nullptr;
}
void __fastcall Leave(uint8_t* t) { Observe(0x4aadd0, t); F<uint32_t>(t, 0xa4) = 33; }
void __fastcall Enter(uint8_t* t) { Observe(0x4aae40, t); F<uint32_t>(t, 0xa8) = 44; }
void __fastcall State3(uint8_t* t, int, void* car, void* site, uint32_t same) {
    Observe(0x4acf90, t, Id(car), Id(site), same); t[0x1b9] = 3;
}
void __fastcall State5(uint8_t* t, int, void* car, void* site, uint32_t same) {
    Observe(0x4ad020, t, Id(car), Id(site), same); t[0x1b9] = 5;
}
void __fastcall State(uint8_t* t, int, int32_t state, int32_t a) {
    Observe(0x4ab980, t, (uint32_t)state, (uint32_t)a); t[0x1b9] = (uint8_t)state;
}
void __fastcall DefaultName(uint8_t* t) { Observe(0x4a90d0, t); F<uint32_t>(t, 0x25) ^= 7; }
void* __fastcall PlayerCompany(void* world) {
    Observe(0x434870, world);
    if (g_bits & 0x800) F<uint32_t>(g_f.train, 0x199) = g_payout;
    return g_f.companies[g_case & 1];
}
void __fastcall News(void* world, int, const char* name, int32_t a, int32_t b, uint32_t id, int32_t value, int32_t x, int32_t y) {
    Observe(0x4337c0, world, Hash(name), (uint32_t)a, (uint32_t)b, id);
    Word((uint32_t)value); Word((uint32_t)x); Word((uint32_t)y);
    if (g_bits & 0x1000) F<uint32_t>(g_f.train, 0x199) = 0x45000000;
}
int32_t __fastcall Active(void* site) {
    Observe(0x47fd50, site); ++g_f.active;
    return site && (g_case / (g_f.active == 1 ? 1 : 7)) % 3 != 0;
}
void __fastcall Reset(uint8_t* t) {
    Observe(0x4a7810, t); ++g_f.resets;
    if (g_bits & 0x2000) F<uint32_t>(t, 0x94) = 23;
}
float __fastcall Arrive(uint8_t* t, int, uint32_t station) {
    Observe(0x4adf30, t, station);
    if (g_bits & 0x4000) g_f.options[0x82] ^= 1;
    return *(float*)&g_payout;
}
uint32_t __fastcall PlayerId(void* world) {
    Observe(0x4337a0, world);
    uint32_t id = F<uint32_t>(g_f.train, 0x51);
    if (g_bits & 0x8000) F<uint32_t>(g_f.train, 0x51) ^= 1;
    return g_case & 1 ? id : id + 1;
}
char* __fastcall Name(uint8_t* t, int, int32_t number) {
    Observe(0x4a9360, t, (uint32_t)number);
    return g_f.names[(g_f.namesCalled++) & 1];
}
int32_t __fastcall IsNumber(const char* name) { Observe(0x51e6e0, nullptr, Hash(name)); return g_case & 1; }
const char* __fastcall Text(int32_t id) {
    Observe(0x5193f0, nullptr, (uint32_t)id);
    return g_f.strings[id == 0xf2a ? 0 : id == 0x117 ? 1 : 2];
}
char* __fastcall Format(char* out, const char* fmt, const char* a, const char* b, const char* c, const char* d,
                        const char* e, const char* f, const char* g, const char* h) {
    Observe(0x518de0, nullptr, Hash(fmt), Hash(a), Hash(b), Hash(c));
    Word(Hash(d)); Word(Hash(e)); Word(Hash(f)); Word(Hash(g)); Word(Hash(h)); ++g_f.formats;
    sprintf(out, "[%s:%s:%s]", fmt, a ? a : "", b ? b : "");
    return g_f.names[0];
}
char* __fastcall Money(char* out, int32_t style, double value, int32_t width) {
    uint32_t bits[2]; memcpy(bits, &value, 8);
    Observe(0x51c000, nullptr, (uint32_t)style, bits[0], bits[1], (uint32_t)width); ++g_f.money;
    sprintf(out, "$%u", g_f.money);
    if (g_bits & 0x10000) {
        uint16_t cw = 0x0e7f;
        __asm fldcw cw
        F<uint32_t>(g_f.train, 0x39) = 0x40000000;
    }
    return g_f.names[1];
}
void __fastcall Log(void* log, int, const char* message, int32_t a, uint32_t train, int32_t b) {
    Observe(0x4554e0, log, Hash(message), (uint32_t)a, train, (uint32_t)b);
    strcpy(g_f.message, message);
    if (g_bits & 0x20000) F<uint32_t>(g_f.train, 0x94) = 19;
}
void Setup(uint32_t it) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it; g_bits = it * 1664525u + 1013904223u;
    uint32_t rng = g_bits;
    for (uint8_t* p = g_f.train; p != (uint8_t*)g_f.tables; ++p) {
        rng = rng * 1664525u + 1013904223u; *p = (uint8_t)(rng >> 16);
    }
    g_force = it % 3 ? 0 : -1;
    const uint16_t cws[] = {0x027f, 0x037f, 0x007f, 0x0a7f, 0x0e7f};
    g_cw = cws[(it / 16) % 5];
    g_payout = kFloats[(it / 11) % 16];
    for (int i = 0; i < 4; ++i) {
        g_f.tables[i] = i;
        F<uint32_t>(g_f.stations[i], 0) = i;
        F<uint32_t>(g_f.stations[i], 8) = (it / 5) % 2 ? i : 99;
        sprintf((char*)g_f.stations[i] + 0x46b, "Station %d", i);
    }
    for (int b = 0; b < 2; ++b) {
        for (int i = 0; i < 6; ++i) {
            uint8_t* row = g_f.rows[b] + 0x33 * i;
            row[0x28] = (it / 9 + i) % 5 == 0 ? 0x80 : 0;
            F<uint16_t>(row, 0x29) = (uint16_t)((it / 13 + i + b) % 4);
            F<uint32_t>(row, 0x2b) = (it / 5) % 2 ? (uint32_t)i : 99;
        }
        F<uint32_t>(g_f.companies[b], 0x347) = kFloats[(it / (b ? 7 : 3)) % 16];
        F<uint32_t>(g_f.worlds[b], 0x4aeb) = (it / 17) % 3 == 0;
        F<uint32_t>(g_f.worlds[b], 0x4aef) = (it / 19) % 5 == 0;
        g_f.buildings[b] = (uint32_t)(uintptr_t)g_f.vtable;
        g_f.attributes[b][0x8c] = (it / 4 + b * ((it / 8) % 2)) % 5;
    }
    g_f.vtable[32] = (uint32_t)(uintptr_t)&Attribute;
    F<void*>(g_f.train, 0) = g_f.rows[0];
    F<int32_t>(g_f.train, 4) = 1 + it % 5;
    F<int32_t>(g_f.train, 8) = (int32_t)(it % (1 + it % 5));
    F<int32_t>(g_f.train, 0x41) = it % 7 == 0 ? -1 : 1;
    F<uint32_t>(g_f.train, 0x51) = it & 1;
    F<uint32_t>(g_f.train, 0x94) = it % 5 ? 7 : 0xffffffff;
    F<uint32_t>(g_f.train, 0xac) = it % 7 ? (it / 13) % 4 : 0xffffffff;
    const uint32_t offsets[] = {0x39, 0xdc, 0xe0, 0xe4, 0x199};
    for (uint32_t i = 0; i < 5; ++i) F<uint32_t>(g_f.train, offsets[i]) = kFloats[(it / (i + 1) + i) % 16];
    g_f.train[0xfb] = (it / 2) & 1; g_f.train[0x189] = (it / 3) & 1; g_f.train[0x1a1] = (it / 5) & 1;
    for (int i = 0; i < 8; ++i) F<uint16_t>(g_f.train, 0x1b2 + 2 * i) = (uint16_t)(it * 37 + i);
    g_f.train[0x1b8] = (uint8_t)(it % 9);
    g_f.options[0x82] = (it / 23) % 5 == 0;
    strcpy(g_f.names[0], "42"); strcpy(g_f.names[1], "Named");
    strcpy(g_f.strings[0], "Train "); strcpy(g_f.strings[1], "Arrive"); strcpy(g_f.strings[2], "Tax");
    *(void**)0x6cec20 = &g_f.tables[0]; *(void**)0x62ba84 = &g_f.tables[1];
    *(void**)0x62be10 = &g_f.tables[2]; *(void**)0x6cec78 = g_f.worlds[0];
    *(void**)0x6cec7c = g_f.options; *(void**)0x6acd34 = &g_f.tables[0];
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
    __asm { fninit }
    __asm { fldcw g_cw }
    __try { g_f.result = ((int32_t(__fastcall*)(uint8_t*, int, int32_t))fn)(g_f.train, 0, g_force); }
    __except(Catch(GetExceptionInformation())) {}
    uint16_t cw; __asm fnstcw cw
    g_f.cw = cw;
}
}
DIFFTEST(game_TrainProcessStop) {
    Redirects p;
    p.Add(0x4b2bd0, (void*)&Current); p.Add(0x434050, (void*)&Multiplayer); p.Add(0x46d2b0, (void*)&Version);
    p.Add(0x518140, (void*)&Get); p.Add(0x41c700, (void*)&Move); p.Add(0x41ce80, (void*)&Pose);
    p.Add(0x4a9840, (void*)&Align); p.Add(0x4b3000, (void*)&Remove); p.Add(0x517d40, (void*)&Valid);
    p.Add(0x4b2f00, (void*)&SetCurrent); p.Add(0x47f290, (void*)&Highlight); p.Add(0x47de00, (void*)&Building);
    p.Add(0x4a9020, (void*)&CityRef); p.Add(0x4aadd0, (void*)&Leave); p.Add(0x4aae40, (void*)&Enter);
    p.Add(0x4acf90, (void*)&State3); p.Add(0x4ad020, (void*)&State5); p.Add(0x4ab980, (void*)&State);
    p.Add(0x4a90d0, (void*)&DefaultName); p.Add(0x434870, (void*)&PlayerCompany); p.Add(0x4337c0, (void*)&News);
    p.Add(0x47fd50, (void*)&Active); p.Add(0x4a7810, (void*)&Reset); p.Add(0x4adf30, (void*)&Arrive);
    p.Add(0x4337a0, (void*)&PlayerId); p.Add(0x4a9360, (void*)&Name); p.Add(0x51e6e0, (void*)&IsNumber);
    p.Add(0x5193f0, (void*)&Text); p.Add(0x518de0, (void*)&Format); p.Add(0x51c000, (void*)&Money); p.Add(0x4554e0, (void*)&Log);
    uint32_t saved[6]; uint16_t savedCw; __asm fnstcw savedCw
    for (int i = 0; i < 6; ++i) saved[i] = *(uint32_t*)kSlots[i];
    for (uint32_t it = 0; it < 8192; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it);
            Invoke(side ? (void*)&rt3::Train_ProcessStop : (void*)0x4ae250);
            for (int i = 0; i < 6; ++i) g_f.globals[i] = *(uint32_t*)kSlots[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t logStart = (uint8_t*)g_f.log - (uint8_t*)&g_f;
                uint32_t word = off >= logStart ? (uint32_t)((off - logStart) / 4) : 0;
                EXPECT_EQ(1, 0, "station case %u byte %u %02x/%02x, word %u %08x/%08x, trace %u/%u, fault %08x/%08x",
                    it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word, expected.log[word], g_f.log[word],
                    expected.used, g_f.used, expected.fault, g_f.fault);
                it = 8192;
                break;
            }
        }
    }
    for (int i = 0; i < 6; ++i) *(uint32_t*)kSlots[i] = saved[i];
    __asm { fninit }
    __asm { fldcw savedCw }
}
