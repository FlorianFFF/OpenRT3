// Engine/tender replacement fixtures for GPT6.1 SOL MEDIUM's 0x4aefb0 rewrite.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall Train_ReplaceEngine(uint8_t*, int, uint32_t, int32_t);
}
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } entries[48];
    int count = 0;
    void AddOne(uintptr_t addr, void* target) {
        Patch& p = entries[count++]; p.at = (uint8_t*)addr; DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
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
            Patch& p = entries[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
const uint32_t kGlobals[] = {0x62ba84, 0x62ba88, 0x6ada84, 0x62be10, 0x6cec78, 0x6cec7c, 0x62be68};
struct Fixture {
    uint8_t train[0x230], cars[8][0x3d4], refs[8][0x180], defs[4][0x100];
    uint8_t assets[3][0x180], companies[3][0x100], worlds[2][0x80], options[0x90];
    uint8_t tables[7][0x10], owner[0x10];
    uint32_t globals[7], log[16384];
    uint32_t used, stride, lookups[4], refCount, cityCount, prices, added, deleted, nexts;
    uint32_t fault, faultKind, faultAddress, fp;
};
Fixture g_f;
uint32_t g_bits, g_case, g_type;
int32_t g_free;
const uint32_t kFloats[] = {
    0, 0x80000000, 0x3f800000, 0xbf800000, 0x7f800000, 0xff800000,
    0x7fc12345, 0x7f812345, 0xffc54321, 0xff854321, 1, 0x7f7fffff,
    0x3dcccccd, 0x00800000, 0x80800000, 0xc2480000
};
int CarIndex(void* p) {
    for (int i = 0; i < 8; ++i) if (p == g_f.cars[i]) return i;
    return -1;
}
uint32_t Id(void* p) {
    if (!p) return 0;
    if (p == g_f.train) return 1;
    for (int i = 0; i < 8; ++i) {
        if (p == g_f.cars[i]) return 100 + i;
        if (p == g_f.refs[i]) return 200 + i;
    }
    for (int i = 0; i < 4; ++i) if (p == g_f.defs[i]) return 300 + i;
    for (int i = 0; i < 3; ++i) {
        if (p == g_f.assets[i]) return 400 + i;
        if (p == g_f.companies[i]) return 500 + i;
    }
    for (int i = 0; i < 2; ++i) if (p == g_f.worlds[i]) return 600 + i;
    for (int i = 0; i < 7; ++i) if (p == g_f.tables[i]) return 800 + i;
    if (p == g_f.owner) return 900;
    return (uint32_t)(uintptr_t)p;
}
void Word(uint32_t value) { g_f.log[g_f.used++] = value; }
void Observe(uint32_t address, void* self, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0) {
    uint32_t start = g_f.used;
    Word(address); Word(Id(self)); Word(a); Word(b); Word(c); Word(d);
    const uint32_t train[] = {0x25, 0x41, 0x45, 0x51, 0xcc, 0xd4, 0xdc, 0xe0, 0xe4, 0xe8, 0xf5};
    for (uint32_t off : train) Word(F<uint32_t>(g_f.train, off));
    Word(g_f.train[0xf4]); Word(g_f.train[0xf9]);
    const uint32_t car[] = {0x34a, 0x34e, 0x352, 0x356, 0x35a, 0x35e, 0x390, 0x1e2, 0x1ea};
    for (auto& row : g_f.cars) for (uint32_t off : car) Word(F<uint32_t>(row, off));
    for (auto& row : g_f.refs) {
        Word(F<uint32_t>(row, 0x41)); Word(F<uint32_t>(row, 0xfd)); Word(F<uint32_t>(row, 0x126));
    }
    for (auto& row : g_f.defs) {
        Word(F<uint32_t>(row, 4)); Word(F<uint32_t>(row, 0x10));
        Word(F<uint32_t>(row, 0x72)); Word(F<uint32_t>(row, 0x76));
    }
    for (auto& row : g_f.assets) Word(F<uint32_t>(row, 0xfd));
    for (auto& row : g_f.companies) {
        Word(F<uint32_t>(row, 0x3b)); Word(F<uint32_t>(row, 0x70));
        Word(F<uint32_t>(row, 0x74)); Word(F<uint32_t>(row, 0x80));
    }
    for (auto& row : g_f.worlds) {
        Word(F<uint32_t>(row, 0x45)); Word(F<uint32_t>(row, 0x49)); Word(F<uint16_t>(row, 0xd));
    }
    for (uint32_t slot : kGlobals) Word(*(uint32_t*)slot);
    for (uint32_t n : g_f.lookups) Word(n);
    Word(g_f.refCount); Word(g_f.cityCount); Word(g_f.prices); Word(g_f.added); Word(g_f.deleted); Word(g_f.nexts);
    g_f.stride = g_f.used - start;
}
uint8_t* __fastcall Get(void* table, int, uint32_t id) {
    uint32_t tag = F<uint32_t>(table, 4);
    Observe(0x518140, table, id);
    ++g_f.lookups[tag];
    uint8_t* out = nullptr;
    if (tag == 0) out = id < 8 ? g_f.cars[id] : nullptr;
    if (tag == 1) out = id > 0 && id <= 3 ? g_f.assets[id - 1] : nullptr;
    if (tag == 2) out = g_f.defs[id & 3];
    if (tag == 3) out = g_f.companies[id % 3];
    if (tag == 0 && g_f.lookups[0] == 1 && !g_f.added && (g_bits & 0x100)) {
        F<uint32_t>(g_f.worlds[0], 0x49) ^= 1;
        if (g_bits & 0x200) F<uint32_t>(g_f.train, 0x41) = 1;
    }
    if (tag == 2 && (id == 1 || id == 3) && g_f.lookups[2] > 1 && (g_bits & 0x400))
        out = g_f.defs[id == 1 ? 3 : 1];
    if (tag == 1 && g_f.lookups[1] == 2 && (g_bits & 0x800)) {
        for (auto& row : g_f.defs) F<uint32_t>(row, 0x76) = (g_bits & 0x1000) ? 3 : 0;
        F<uint32_t>(g_f.cars[1], 0x352) = 0xffffffff;
    }
    return out;
}
uint8_t* __fastcall Ref(uint8_t* car) {
    Observe(0x41adb0, car); ++g_f.refCount;
    int i = CarIndex(car);
    if (i < 0) return nullptr;
    if ((g_bits & 0x2000) && g_f.refCount == 2) F<uint32_t>(g_f.train, 0xf5) ^= 0x40000000;
    return g_f.refs[i];
}
uint8_t* __fastcall Next(uint8_t* car) {
    Observe(0x41ad90, car); ++g_f.nexts;
    int i = CarIndex(car);
    if (i < 0 || g_f.nexts > 32) return nullptr;
    uint32_t next = F<uint32_t>(car, 0x352);
    if ((g_bits & 0x4000) && i == 1) F<uint32_t>(car, 0x352) = 0xffffffff;
    return next < 8 ? g_f.cars[next] : nullptr;
}
void __fastcall PassSignal(uint8_t* train) {
    Observe(0x4aae90, train);
    if (g_bits & 0x8000) {
        for (auto& row : g_f.cars) F<uint32_t>(row, 0x356) ^= 0x55;
        *(void**)0x6cec78 = g_f.worlds[1];
    }
}
uint8_t* __fastcall CityRef(uint8_t* train) {
    Observe(0x4a8840, train); ++g_f.cityCount;
    int id = (int)F<uint32_t>(train, 0x41);
    uint8_t* result = g_f.defs[F<uint32_t>(g_f.refs[id & 7], 0x126) & 3];
    if (g_bits & 0x10000) F<uint32_t>(train, 0x51) = (F<uint32_t>(train, 0x51) + 1) % 3;
    return result;
}
float __fastcall Price(uint8_t* type, int, uint32_t company, uint32_t player) {
    uint32_t bits = F<uint32_t>(type, 4);
    Observe(0x4619c0, type, company, player); ++g_f.prices;
    if (g_bits & 0x20000) *(void**)0x6ada84 = g_f.tables[6];
    float value; memcpy(&value, &bits, 4); return value;
}
void Finance(void* company, int32_t stat, float amount, uint32_t address, int32_t withCash) {
    uint32_t bits; memcpy(&bits, &amount, 4);
    Observe(address, company, (uint32_t)stat, bits, (uint32_t)withCash);
    F<uint32_t>(company, 0x40 + stat * 4) ^= bits;
    if (g_bits & 0x40000) {
        F<uint32_t>(g_f.train, 0x51) = (F<uint32_t>(g_f.train, 0x51) + 1) % 3;
        *(void**)0x62be10 = g_f.tables[4];
    }
}
void __fastcall AddStat(void* company, int, int32_t stat, float amount) { Finance(company, stat, amount, 0x42a040, 0); }
void __fastcall PostStat(void* company, int, int32_t stat, float amount, int32_t cash) { Finance(company, stat, amount, 0x42a080, cash); }
template <uint32_t Address> double __fastcall Coordinate(uint8_t* car) {
    double value = (double)F<float>(car, Address == 0x455800 ? 0x1e2 : 0x1ea) / 92.0;
    Observe(Address, car);
    if (Address == 0x455800 && (g_bits & 0x80000)) F<uint32_t>(car, 0x1ea) ^= 0x80000000;
    return value;
}
uint32_t __fastcall AddCar(void* table, int, uint8_t* type, uint32_t owner, float x, float y, uint32_t a6) {
    uint32_t xb, yb; memcpy(&xb, &x, 4); memcpy(&yb, &y, 4);
    Observe(0x41d340, table, Id(type), owner, xb, yb);
    Word(a6); // Extra argument has a separate trace word; all other events have fixed stride.
    uint32_t id = 3 + g_f.added++;
    if (g_case % 256 == 77 || (id == 4 && g_case % 256 == 91)) return 0xffffffff;
    F<uint32_t>(g_f.cars[id], 0x34a) = id;
    F<int32_t>(g_f.cars[id], 0x34e) = F<int32_t>(g_f.cars[id], 0x352) = -1;
    return id;
}
void __fastcall Delete(void* table, int, uint32_t id) {
    Observe(0xd311, table, id);
    if (id < 8) {
        g_f.deleted |= 1u << id;
        F<uint32_t>(g_f.cars[id], 0x35e) = 0xdeadbeef;
    }
    if (g_bits & 0x100000) {
        for (auto& row : g_f.defs) {
            F<uint32_t>(row, 0x10) ^= 2;
            F<uint32_t>(row, 0x72) = 3;
        }
    }
    if (g_bits & 0x200000) F<uint32_t>(g_f.train, 0xf5) ^= 0x80000000;
}
void __fastcall Move(uint8_t* car, int, uint32_t node, uint32_t phase) {
    Observe(0x41c700, car, node, phase);
    F<uint32_t>(car, 0x356) = node;
    if (g_bits & 0x400000) F<uint32_t>(car, 0x35a) ^= 1;
}
void __fastcall Pose(uint8_t* car, int, uint32_t dist, uint32_t a, uint32_t b, int32_t c) {
    Observe(0x41ce80, car, dist, a, b, (uint32_t)c);
    F<uint32_t>(car, 0x35e) ^= 0x1000;
    if (g_bits & 0x800000) F<uint32_t>(g_f.train, 0xe8) = 2;
}
int32_t __fastcall Follow(uint8_t* car, int, void* arg) {
    Observe(0x41c790, car, Id(arg));
    F<uint32_t>(car, 0x35e) ^= 0x2000;
    if ((g_bits & 0x1000000) && car == g_f.cars[1]) F<int32_t>(car, 0x352) = -1;
    return 0;
}
void __fastcall WorldFollow(void* world, int, int32_t on, int32_t kind, uint32_t id) {
    Observe(0x437a90, world, (uint32_t)on, (uint32_t)kind, id);
    if (g_bits & 0x2000000) F<uint32_t>(g_f.train, 0xcc) = 0x7f812345;
}
void __fastcall Route(uint8_t* train) { Observe(0x4ad260, train); F<uint32_t>(train, 0x1b0) = 0x4ad260; }
void __fastcall Effects(uint8_t* train, int, uint32_t throttle, uint32_t speed) {
    Observe(0x4a8be0, train, throttle, speed);
}
void __fastcall Owner(void* owner) {
    Observe(0x0a11, owner);
    if (g_bits & 0x4000000) ++g_f.train[0xf4];
}
void __fastcall Ghost(uint8_t* train, int, uint32_t ghost, int32_t force) {
    Observe(0x4a8220, train, ghost, (uint32_t)force);
    train[0xf4] = (uint8_t)ghost; F<uint32_t>(train, 0x1bc) = (uint32_t)force;
}
void* g_carVtable[] = {nullptr, nullptr, (void*)&Delete};
void* g_ownerVtable[] = {(void*)&Owner};

void Setup(uint32_t it) {
    memset(&g_f, 0, sizeof g_f); g_case = it;
    uint32_t r = it * 2654435761u + 0x51a7;
    auto rnd = [&]() { r = r * 1103515245u + 12345u; return r; };
    g_bits = rnd(); g_type = it & 0x1000 ? 3 : 1;
    const int32_t free[] = {0, 0, 1, -1, (int32_t)0x80000000};
    g_free = free[(it / 16) % 5];
    // Fill only object storage; trace/counters remain deterministic zeroes.
    uint8_t* begin = (uint8_t*)&g_f;
    for (uint8_t* p = begin; p != (uint8_t*)&g_f.globals; ++p) *p = (uint8_t)(rnd() >> 16);
    F<uint32_t>(g_f.train, 0x41) = it % 16 == 15 ? 0xffffffff : 0;
    bool following = it % 3 != 0, tail = (it & 4) != 0;
    F<uint32_t>(g_f.train, 0x45) = following ? (tail ? 2 : 1) : 0;
    F<uint32_t>(g_f.train, 0x51) = it % 3;
    const int32_t states[] = {0, 1, 2, 3, 5, 8, -1};
    F<int32_t>(g_f.train, 0xe8) = states[(it / 8) % 7];
    F<uint32_t>(g_f.train, 0xf5) = kFloats[(it / 3) & 15];
    g_f.train[0xf9] = (uint8_t)(it % 4 == 0 ? 255 : it % 4 == 1 ? 0 : it);
    for (int i = 0; i < 8; ++i) {
        F<uint32_t>(g_f.cars[i], 0x34a) = i;
        F<int32_t>(g_f.cars[i], 0x34e) = i ? i - 1 : -1;
        F<int32_t>(g_f.cars[i], 0x352) = -1;
        F<uint32_t>(g_f.cars[i], 0x356) = (it + i) % 5;
        F<uint32_t>(g_f.cars[i], 0x35e) = kFloats[(it + i + 1) & 15];
        F<uint32_t>(g_f.cars[i], 0x1e2) = kFloats[(it + i) & 15];
        F<uint32_t>(g_f.cars[i], 0x1ea) = kFloats[(it + i + 3) & 15];
        F<uint32_t>(g_f.refs[i], 0xfd) = kFloats[(it + i + 7) & 15];
        F<uint32_t>(g_f.refs[i], 0x126) = i == 0 ? 0 : 2;
        F<int32_t>(g_f.refs[i], 0x41) = i == 1 && (it & 8) ? 3 : 1;
    }
    F<int32_t>(g_f.cars[0], 0x352) = following ? 1 : -1;
    F<int32_t>(g_f.cars[1], 0x352) = tail ? 2 : -1;
    for (int i = 0; i < 4; ++i) {
        F<uint32_t>(g_f.defs[i], 4) = kFloats[(it + i + 5) & 15];
        F<uint32_t>(g_f.defs[i], 0x10) = (it / (i & 1 ? 32 : 4)) % 4;
        F<uint32_t>(g_f.defs[i], 0x72) = it % 128 == 93 ? 0 : 1;
        F<uint32_t>(g_f.defs[i], 0x76) = it & 2 ? 2 : 0;
    }
    for (int i = 0; i < 3; ++i) {
        F<uint32_t>(g_f.assets[i], 0xfd) = kFloats[(it + i + 9) & 15];
        F<uint32_t>(g_f.companies[i], 0x3b) = 0xa000 + i;
    }
    for (int i = 0; i < 2; ++i) {
        F<uint16_t>(g_f.worlds[i], 0xd) = (uint16_t)(it + i * 0x1000);
        F<int32_t>(g_f.worlds[i], 0x45) = it % 4 == 0 ? 1 : 2;
        F<uint32_t>(g_f.worlds[i], 0x49) = it & 0x10 ? 7 : 0;
    }
    g_f.options[0x82] = (it >> 2) & 1 ? 255 : 0;
    for (int i = 0; i < 7; ++i) {
        F<void**>(g_f.tables[i], 0) = g_carVtable;
        F<uint32_t>(g_f.tables[i], 4) = i < 4 ? i : i == 4 ? 3 : i == 5 ? 0 : 2;
    }
    F<void**>(g_f.owner, 0) = g_ownerVtable;
    *(void**)0x62ba84 = g_f.tables[0]; *(void**)0x62ba88 = g_f.tables[1];
    *(void**)0x6ada84 = g_f.tables[2]; *(void**)0x62be10 = g_f.tables[3];
    *(void**)0x6cec78 = it % 16 == 15 ? nullptr : g_f.worlds[0];
    *(void**)0x6cec7c = g_f.options; *(void**)0x62be68 = g_f.owner;
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
    __try { ((void(__fastcall*)(uint8_t*, int, uint32_t, int32_t))fn)(g_f.train, 0, g_type, g_free); }
    __except(Catch(GetExceptionInformation())) {}
}
void RunCases() {
    uint32_t saved[7];
    for (int i = 0; i < 7; ++i) saved[i] = *(uint32_t*)kGlobals[i];
    uint16_t oldCW, testCW;
    __asm fnstcw oldCW
    testCW = oldCW | 0x3f;
    bool stop = false;
    for (uint32_t it = 0; it < 8192 && !stop; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            __asm {
                fninit
                fldcw testCW
            }
            Setup(it);
            Invoke(side ? (void*)&rt3::Train_ReplaceEngine : (void*)0x4aefb0);
            if (!g_f.fault) {
                uint16_t cw, sw;
                __asm {
                    fnstcw cw
                    fnstsw sw
                }
                g_f.fp = ((uint32_t)cw << 16) | (sw & 0x3800);
            }
            for (int i = 0; i < 7; ++i) g_f.globals[i] = *(uint32_t*)kGlobals[i];
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t logOff = (uint8_t*)g_f.log - (uint8_t*)&g_f;
                uint32_t word = off >= logOff && off < logOff + sizeof g_f.log ? (uint32_t)((off - logOff) / 4) : 0;
                EXPECT_EQ(1, 0, "engine case %u byte %u: %02x/%02x, log word %u %08x/%08x (used %u/%u, fault %08x/%08x)",
                          it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word,
                          expected.log[word], g_f.log[word], expected.used, g_f.used, expected.fault, g_f.fault);
                stop = true;
            }
        }
    }
    __asm {
        fninit
        fldcw oldCW
    }
    for (int i = 0; i < 7; ++i) *(uint32_t*)kGlobals[i] = saved[i];
}
}

DIFFTEST(game_TrainReplaceEngine) {
    Redirects p;
    p.Add(0x518140, (void*)&Get); p.Add(0x41adb0, (void*)&Ref); p.Add(0x41ad90, (void*)&Next);
    p.Add(0x4aae90, (void*)&PassSignal); p.Add(0x4a8840, (void*)&CityRef);
    p.Add(0x4619c0, (void*)&Price); p.Add(0x42a040, (void*)&AddStat); p.Add(0x42a080, (void*)&PostStat);
    p.Add(0x455800, (void*)&Coordinate<0x455800>); p.Add(0x455810, (void*)&Coordinate<0x455810>);
    p.Add(0x41d340, (void*)&AddCar); p.Add(0x41c700, (void*)&Move); p.Add(0x41ce80, (void*)&Pose);
    p.Add(0x41c790, (void*)&Follow); p.Add(0x437a90, (void*)&WorldFollow); p.Add(0x4ad260, (void*)&Route);
    p.Add(0x4a8be0, (void*)&Effects); p.Add(0x4a8220, (void*)&Ghost);
    RunCases();
}
