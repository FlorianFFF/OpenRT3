// In-process oracle for 0x4ac6d0, reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 { int32_t __fastcall Train_LoadConsist(uint8_t*, int, uint8_t*, uint32_t*); }
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
const uint32_t kSlots[] = {0x6cec74, 0x6cec78, 0x62ba84, 0x62ba8c, 0x62be10, 0x62c120};
const uint32_t kFloats[] = {0, 0x80000000, 0x3f800000, 0xbf800000, 0x3f000001, 0x7fc12345, 0x7f801234, 0x7f800000,
                          0xff800000, 1, 0x7f7fffff, 0x00800000, 0x4b800000, 0x3f7fffff, 0x447a0000, 0x80000001};
const uint64_t kDoubles[] = {0, 0x3ff0000000000000ULL, 0x3ff0000010000000ULL, 0x3feffffff0000000ULL,
                            0x7ff0000000000000ULL, 0xfff0000000000000ULL, 0x7ff812345678abcdULL, 0x7ff012345678abcdULL};
const int32_t kClasses[] = {900, 901, 902, 17, 32, 0x12345678, 900, 901};
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } p[32];
    int n = 0;
    void AddOne(uintptr_t addr, void* target) {
        Patch& q = p[n++]; q.at = (uint8_t*)addr; DWORD old;
        VirtualProtect(q.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(q.old, q.at, 5); q.at[0] = 0xe9;
        F<int32_t>(q.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
        VirtualProtect(q.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), q.at, 5);
    }
    // Original code calls the exe address, rewritten code calls the rewrite
    // directly: a redirect covers both.
    void Add(uintptr_t addr, void* target) {
        AddOne(addr, target);
        if (void* r = hook::Find((uint32_t)addr)) AddOne((uintptr_t)r, target);
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
    uint8_t train[0x400], rows[2][0x33], cars[16][0x400], types[16][0x80], cargos[9][0x80];
    uint8_t companies[2][0x80], worlds[2][0x4cb0], game[0x260], station[0x80];
    uint32_t tables[7], length, current, spawned, loads, picks, matches, effects, gets[7], result, destination;
    uint32_t fault, faultKind, faultAddress, globals[6], used, log[65536];
    uint16_t cw;
};
Fixture g_f;
uint32_t g_case, g_bits;
uint16_t g_cw;
void W(uint32_t v) { g_f.log[g_f.used++] = v; }
void Observe(uint32_t addr, void* self = nullptr, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0) {
    W(addr); W((uint32_t)(uintptr_t)self); W(a); W(b); W(c); W(d);
    for (uint32_t slot : kSlots) W(*(uint32_t*)slot);
    W(F<uint32_t>(g_f.train, 0x41)); W(F<uint32_t>(g_f.train, 0x51)); W(F<uint32_t>(g_f.train, 0xf0));
    W(g_f.train[0x1bf]); W(F<uint16_t>(g_f.train, 0x1c0)); W(g_f.train[0x1c2]); W(F<uint16_t>(g_f.train, 0x1c3));
    W(g_f.rows[0][0x28]); W(g_f.rows[0][0x21]); W(g_f.rows[0][0x22]); W(g_f.rows[0][0x23]);
    for (int i = 0; i < 8; ++i) W(F<uint32_t>(g_f.rows[0], 1 + i * 4));
    W(F<uint32_t>(g_f.rows[0], 0x24)); W(g_f.destination); W(g_f.current); W(g_f.spawned); W(g_f.loads);
    W(g_f.picks); W(g_f.matches); W(g_f.effects);
    uint16_t cw; __asm fnstcw cw
    W(cw);
}
uint8_t* __fastcall Current(uint8_t* t) {
    Observe(0x4b2bd0, t);
    return g_case % 71 == 6 ? nullptr : g_f.rows[g_f.current];
}
uint8_t* __fastcall Get(void* table, int, uint32_t id) {
    Observe(0x518140, table, id);
    uint32_t tag = *(uint32_t*)table; ++g_f.gets[tag];
    if (tag < 2) {
        if (g_case % 37 == 11) return nullptr;
        return g_f.cars[id >= 100 && id < 116 ? id - 100 : 0];
    }
    if (tag < 4) {
        if (g_bits & 1) *(void**)0x62ba8c = &g_f.tables[3];
        return g_f.cargos[id % 9];
    }
    if (g_bits & 2) F<uint32_t>(g_f.train, 0x51) ^= 1;
    return g_f.companies[(g_bits & 4) ? id & 1 : 0];
}
uint8_t* __fastcall Ref(uint8_t* car) {
    Observe(0x41adb0, car);
    if (g_case % 59 == 13) return nullptr;
    return g_f.types[(car - g_f.cars[0]) / 0x400];
}
uint8_t* __fastcall Next(uint8_t* car) {
    Observe(0x41ad90, car);
    uint32_t index = (uint32_t)((car - g_f.cars[0]) / 0x400);
    return index + 1 < g_f.length ? g_f.cars[index + 1] : nullptr;
}
void __fastcall Load(uint8_t* car, int, uint8_t* station, uint32_t* destination) {
    Observe(0x41bb40, car, (uint32_t)(uintptr_t)station, *destination);
    ++g_f.loads;
    if (g_bits & 8) *destination ^= 1;
    if (g_bits & 0x10) g_f.rows[0][0x28] ^= 0x40;
    if (g_bits & 0x20) g_f.current ^= 1;
    if (g_bits & 0x40) F<uint32_t>(g_f.train, 0x41) = 100;
}
int32_t __fastcall Matches(uint8_t* cargo, int, int32_t cls) {
    Observe(0x41e260, cargo, (uint32_t)cls); ++g_f.matches;
    if (g_bits & 0x80) F<int32_t>(g_f.rows[0], 1 + (g_f.matches % 8) * 4) = 17;
    return cls == 900 || ((cls ^ (int32_t)(cargo - g_f.cargos[0])) & 3) != 0;
}
int32_t __fastcall Pick(uint8_t* station, int, int32_t cls, uint32_t destination, int32_t* lost, int32_t* fallback) {
    Observe(0x47f910, station, (uint32_t)cls, destination, (uint32_t)*lost, (uint32_t)*fallback);
    ++g_f.picks;
    if (g_bits & 0x100) *lost = (int32_t)(g_f.picks & 1);
    if (g_bits & 0x200) *fallback = (int32_t)(0xabcd0000u + g_f.picks);
    if (g_bits & 0x400) *(void**)0x6cec78 = g_f.worlds[1];
    if (g_bits & 0x800) g_f.destination ^= 1;
    if (g_f.spawned >= (g_case / 3) % 9 || (g_case + (uint32_t)cls + g_f.picks) % 4 == 0) return -1;
    return (int32_t)((g_f.picks + (uint32_t)cls) % 9);
}
double __fastcall X(uint8_t* car) {
    Observe(0x455800, car);
    uint64_t raw = kDoubles[(g_case / 3) % 8];
    return *(double*)&raw;
}
double __fastcall Y(uint8_t* car) {
    Observe(0x455810, car);
    if (g_bits & 0x1000) F<uint32_t>(g_f.train, 0x51) ^= 1;
    uint64_t raw = kDoubles[(g_case / 5) % 8];
    return *(double*)&raw;
}
uint32_t __fastcall Grid(void* map, int, uint32_t x, uint32_t y) {
    Observe(0x44ad60, map, x, y);
    return 0x257e;
}
double __fastcall Effect(uint8_t* world, int, int32_t idx, uint32_t owner, uint32_t player, uint32_t region) {
    Observe(0x436590, world, (uint32_t)idx, owner, player, region); ++g_f.effects;
    if (g_bits & 0x2000) F<uint32_t>(g_f.train, 0xf0) = kFloats[(g_case / 7) % 16];
    if (g_bits & 0x4000) {
        uint16_t cw = 0x007f;
        __asm fldcw cw
    }
    uint64_t raw = kDoubles[(g_case / 11) % 8];
    return *(double*)&raw;
}
int32_t __cdecl Multiplayer() { Observe(0x434050); return (g_case / 13) % 2; }
uint32_t __fastcall ForCargo(uint8_t* cargo) {
    Observe(0x41e500, cargo);
    return 37;
}
uint32_t __fastcall Append(uint8_t* train, int, uint32_t type, uint32_t cargo) {
    Observe(0x4a9b70, train, type, cargo);
    uint32_t id = 100 + g_f.length + g_f.spawned++;
    if (g_bits & 0x8000) F<uint32_t>(train, 0x41) = 100;
    return id;
}
void Setup(uint32_t it) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it; g_bits = it * 1664525u + 1013904223u;
    uint32_t rng = g_bits;
    for (uint8_t* p = g_f.train; p != (uint8_t*)g_f.tables; ++p) {
        rng = rng * 1664525u + 1013904223u; *p = (uint8_t)(rng >> 16);
    }
    for (int i = 0; i < 7; ++i) g_f.tables[i] = i;
    *(void**)0x6cec74 = g_f.game; *(void**)0x6cec78 = g_f.worlds[0];
    *(void**)0x62ba84 = &g_f.tables[0]; *(void**)0x62ba8c = &g_f.tables[2];
    *(void**)0x62be10 = &g_f.tables[4]; *(void**)0x62c120 = &g_f.tables[6];
    g_f.length = it % 9;
    F<uint32_t>(g_f.train, 0x41) = g_f.length ? 100 : 0xffffffff;
    F<uint32_t>(g_f.train, 0x51) = it & 1;
    F<uint32_t>(g_f.train, 0xf0) = kFloats[(it / 3) % 16];
    F<uint32_t>(g_f.game, 0x24f) = (it / 7) % 2;
    g_f.destination = it ^ 0x753281cd;
    for (int i = 0; i < 16; ++i) {
        F<uint32_t>(g_f.cars[i], 0x38c) = 1 + i % 8;
        F<uint32_t>(g_f.cars[i], 0x398) = kFloats[(it / (i + 1)) % 16];
        F<int32_t>(g_f.types[i], 0x41) = (it / (i + 1)) % 3 == 0 ? 0 : 1;
    }
    for (int i = 0; i < 2; ++i) {
        for (int k = 0; k < 8; ++k) F<int32_t>(g_f.rows[i], 1 + k * 4) = kClasses[(it / 7 + k) % 8];
        g_f.rows[i][0x21] = (it / 5) % 10; g_f.rows[i][0x22] = (it / 3) % 10;
        g_f.rows[i][0x23] = (it / 11) % 9;
        F<int32_t>(g_f.rows[i], 0x24) = kClasses[(it / 13) % 8];
        g_f.rows[i][0x28] = (it / 2 + i) % 2 ? 0x40 : 0;
        F<uint32_t>(g_f.worlds[i], 0x4ca2) = kFloats[(it / (i ? 17 : 19)) % 16];
        F<uint32_t>(g_f.companies[i], 0x3b) = 1000 + i;
    }
    const uint16_t cws[] = {0x027f, 0x037f, 0x007f, 0x0a7f, 0x0e7f};
    g_cw = cws[(it / 16) % 5];
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
    __asm fninit
    __asm fldcw g_cw
    __try {
        g_f.result = ((int32_t(__fastcall*)(uint8_t*, int, uint8_t*, uint32_t*))fn)(
            g_f.train, 0, g_f.station, &g_f.destination);
    } __except(Catch(GetExceptionInformation())) {}
    uint16_t cw; __asm fnstcw cw
    g_f.cw = cw;
}
}
DIFFTEST(game_TrainLoadConsist) {
    Redirects p;
    p.Add(0x4b2bd0, (void*)&Current); p.Add(0x518140, (void*)&Get);
    p.Add(0x41adb0, (void*)&Ref); p.Add(0x41ad90, (void*)&Next); p.Add(0x41bb40, (void*)&Load);
    p.Add(0x41e260, (void*)&Matches); p.Add(0x47f910, (void*)&Pick);
    p.Add(0x455800, (void*)&X); p.Add(0x455810, (void*)&Y); p.Add(0x44ad60, (void*)&Grid);
    p.Add(0x436590, (void*)&Effect); p.Add(0x434050, (void*)&Multiplayer);
    p.Add(0x41e500, (void*)&ForCargo); p.Add(0x4a9b70, (void*)&Append);
    uint32_t saved[6]; uint16_t savedCw; __asm fnstcw savedCw
    for (int i = 0; i < 6; ++i) saved[i] = *(uint32_t*)kSlots[i];
    uint32_t sawLoad = 0, sawMatch = 0, sawPick = 0, sawEffect = 0, sawLost = 0, sawFallback = 0;
    for (uint32_t it = 0; it < 8192; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it);
            Invoke(side ? (void*)&rt3::Train_LoadConsist : (void*)0x4ac6d0);
            for (int i = 0; i < 6; ++i) g_f.globals[i] = *(uint32_t*)kSlots[i];
            if (!side) {
                expected = g_f;
                sawLoad += g_f.loads != 0; sawMatch += g_f.matches != 0; sawPick += g_f.picks != 0; sawEffect += g_f.effects != 0;
                sawLost += g_f.train[0x1bf] != 0 && !g_f.fault; sawFallback += g_f.train[0x1c2] != 0 && !g_f.fault;
            } else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t start = (uint8_t*)g_f.log - (uint8_t*)&g_f;
                uint32_t word = off >= start && off < start + sizeof g_f.log ? (uint32_t)((off - start) / 4) : 0;
                EXPECT_EQ(1, 0, "loading case %u byte %u %02x/%02x word %u %08x/%08x trace %u/%u fault %08x/%08x",
                    it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word, expected.log[word], g_f.log[word],
                    expected.used, g_f.used, expected.fault, g_f.fault);
                it = 8192;
                break;
            }
        }
    }
    EXPECT_EQ(true, sawLoad != 0, "existing/new load paths exercised");
    EXPECT_EQ(true, sawMatch != 0, "class assignment exercised");
    EXPECT_EQ(true, sawPick != 0, "cargo selection exercised");
    EXPECT_EQ(true, sawEffect != 0, "cost/effect path exercised");
    EXPECT_EQ(true, sawLost != 0, "lost-cargo output exercised");
    EXPECT_EQ(true, sawFallback != 0, "fallback-cargo output exercised");
    for (int i = 0; i < 6; ++i) *(uint32_t*)kSlots[i] = saved[i];
    __asm fninit
    __asm fldcw savedCw
}
