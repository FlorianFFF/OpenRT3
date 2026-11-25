// Native oracles for 0x4acaf0 / 0x4b0140, reverse engineered by GPT6.1 SOL MEDIUM.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall Train_AutoAddCars(uint8_t*);
void __fastcall Train_PlaceInitialEngine(uint8_t*, int, uint32_t, uint8_t*);
}
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
const uint32_t kSlots[] = {0x6cec78, 0x6cec7c, 0x62ba88, 0x6ada84, 0x62ba84, 0x62be10};
const uint32_t kFloats[] = {0, 0x80000000, 0x3f800000, 0xbf800000, 0x3f000001, 0x7fc12345, 0x7f801234, 0x7f800000,
                          0xff800000, 1, 0x7f7fffff, 0x00800000, 0x4b800000, 0x3f7fffff, 0x447a0000, 0x80000001};
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
    uint8_t train[0x400], rows[2][0x33], worlds[2][0x4b00], options[2][0x90];
    uint8_t models[2][0x90], assets[2][0x120], car[0x400], companies[2][0x50], node[0x220];
    uint32_t tables[7], gets[7], currents, finds[6], appends, counts, pose, assetGets, modelGets, companyGets, stats;
    int32_t count;
    uint32_t fault, faultKind, faultAddress, globals[6], price, used, log[16384];
    uint16_t cw;
};
Fixture g_f;
uint32_t g_case, g_bits, g_price, g_type;
bool g_place;
uint16_t g_cw;
void W(uint32_t v) { g_f.log[g_f.used++] = v; }
void Observe(uint32_t addr, void* self = nullptr, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0, uint32_t e = 0) {
    W(addr); W((uint32_t)(uintptr_t)self); W(a); W(b); W(c); W(d); W(e);
    for (uint32_t slot : kSlots) W(*(uint32_t*)slot);
    const uint32_t fields[] = {0x25, 0x41, 0x45, 0x51, 0xf5};
    for (uint32_t off : fields) W(F<uint32_t>(g_f.train, off));
    W(g_f.train[0xf9]); W(g_f.train[0x197]); W(g_f.train[0x198]);
    W(g_f.rows[0][0x28]); W(g_f.rows[1][0x28]); W(F<uint32_t>(g_f.car, 0x390));
    for (int i = 0; i < 2; ++i) {
        W(F<uint32_t>(g_f.models[i], 0x72)); W(F<uint32_t>(g_f.models[i], 0x76)); W(g_f.models[i][0x7a]);
        W(F<uint32_t>(g_f.assets[i], 0xfd)); W(F<uint32_t>(g_f.worlds[i], 0x4af3)); W(g_f.options[i][0x82]);
    }
    W(g_f.appends); W(g_f.counts); W((uint32_t)g_f.count); W(g_f.assetGets); W(g_f.modelGets); W(g_f.companyGets); W(g_f.stats);
    uint16_t cw; __asm fnstcw cw
    W(cw);
}
uint8_t* __fastcall Current(uint8_t* t) {
    Observe(0x4b2bd0, t); ++g_f.currents;
    return g_case % 61 == 3 ? nullptr : g_f.rows[0];
}
uint32_t __fastcall Find(void* table, int, int32_t category) {
    Observe(0x41e100, table, (uint32_t)category); ++g_f.finds[category];
    if (g_bits & 1) g_f.rows[0][0x28] ^= 0x20;
    if (g_bits & 2) *(void**)0x62ba88 = &g_f.tables[1];
    return (g_bits >> (category + 3)) & 1 ? 0 : 10 + category;
}
void __fastcall Append(uint8_t* t, int, uint32_t type, int32_t flags) {
    Observe(0x4a9b70, t, type, (uint32_t)flags); ++g_f.appends; ++g_f.count;
    if (!g_place) {
        if (g_bits & 4) g_f.rows[0][0x28] ^= 0x20;
        if (g_bits & 8) t[0x197] ^= 1;
        if (g_bits & 0x10) *(void**)0x6cec78 = g_f.worlds[1];
        if (g_bits & 0x20) *(void**)0x62ba88 = &g_f.tables[1];
    } else {
        if (g_bits & 4) F<uint32_t>(t, 0x51) ^= 1;
        if (g_bits & 8) *(void**)0x6cec7c = g_f.options[1];
    }
}
int32_t __fastcall Count(uint8_t* t) {
    Observe(0x4a7760, t); ++g_f.counts;
    if (g_bits & 0x40) F<uint32_t>(g_f.worlds[1], 0x4af3) = 0;
    return g_f.count;
}
void __fastcall Center(uint8_t* node, int, uint32_t* x, uint32_t* z) {
    Observe(0x48a170, node, *x, *z);
    if (g_case % 7 != 0) *x = kFloats[g_case % 16];
    if (g_case % 11 != 0) *z = kFloats[(g_case / 3) % 16];
    if (g_bits & 1) F<uint32_t>(node, 0x206) ^= 0xffffffff;
}
uint8_t* __fastcall Get(void* table, int, uint32_t id) {
    Observe(0x518140, table, id);
    uint32_t tag = *(uint32_t*)table; ++g_f.gets[tag];
    if (tag < 2) {
        ++g_f.assetGets;
        if (g_case % 37 == 5 && g_f.assetGets == 2) return nullptr;
        return g_f.assets[id & 1];
    }
    if (tag < 4) {
        ++g_f.modelGets;
        if (g_case % 43 == 2) return nullptr;
        if (g_bits & 2) F<uint32_t>(g_f.node, 0x206) ^= 0xffffffff;
        return g_f.models[(g_bits & 4) && g_f.modelGets > 1 ? 1 : 0];
    }
    if (tag == 4) {
        if (g_case % 41 == 9) return nullptr;
        if (g_bits & 8) F<uint32_t>(g_f.train, 0x25) ^= 0xabcdef12;
        return g_f.car;
    }
    ++g_f.companyGets;
    if (g_bits & 0x10) F<uint32_t>(g_f.train, 0x51) ^= 1;
    return g_f.companies[(g_bits & 0x20) && (g_f.companyGets & 1) ? 1 : 0];
}
uint32_t __fastcall Add(void* table, int, uint8_t* asset, uint32_t owner, uint32_t x, uint32_t z, uint32_t end) {
    Observe(0x41d340, table, (uint32_t)(uintptr_t)asset, owner, x, z, end);
    if (g_bits & 0x40) F<uint32_t>(g_f.train, 0x25) ^= 0x12345678;
    return g_bits ^ 0xa583c930;
}
void __fastcall Move(uint8_t* car, int, uint32_t node, uint32_t position) {
    Observe(0x41c700, car, node, position);
    if (g_bits & 0x80) F<uint32_t>(g_f.models[0], 0x72) = 2;
    if (g_bits & 0x100) *(void**)0x6cec78 = nullptr;
    if (g_bits & 0x200) *(void**)0x62ba88 = &g_f.tables[1];
}
int32_t __cdecl Multiplayer() {
    Observe(0x434050);
    return (g_case / 3) % 2;
}
int32_t __stdcall Version(uint32_t bits) {
    Observe(0x46d2b0, nullptr, bits);
    return (g_case / 5) % 2;
}
void MutatePose() {
    ++g_f.pose;
    if (g_bits & 0x400) F<uint32_t>(g_f.models[0], 0x72) = g_case % 3;
    if (g_bits & 0x800) F<uint32_t>(g_f.train, 0x41) = 17;
    if (g_bits & 0x1000) g_f.train[0xf9] = 255;
}
void __fastcall Legacy(uint8_t* car, int, uint32_t a, uint32_t b, uint32_t c, int32_t d) {
    Observe(0x41ce80, car, a, b, c, (uint32_t)d); MutatePose();
}
void __fastcall Modern(uint8_t* car, int, uint32_t a) {
    Observe(0x41b080, car, a); MutatePose();
}
float __fastcall Price(uint8_t* model, int, uint32_t owner, uint32_t player) {
    Observe(0x4619c0, model, owner, player);
    if (g_bits & 0x2000) F<uint32_t>(g_f.train, 0x51) ^= 1;
    g_f.price = g_price;
    return *(float*)&g_price;
}
void __fastcall Stat(uint8_t* company, int, int32_t stat, uint32_t bits) {
    Observe(0x42a040, company, (uint32_t)stat, bits); ++g_f.stats;
    if (g_bits & 0x4000) F<uint32_t>(g_f.train, 0x51) ^= 1;
    if (g_bits & 0x8000) *(void**)0x62be10 = &g_f.tables[6];
    g_price ^= 0x01ab7352;
    if (g_bits & 0x10000) {
        uint16_t cw = 0x0e7f;
        __asm fldcw cw
    }
}
void __fastcall Breakdown(uint8_t* t) { Observe(0x4afeb0, t); t[0x196] ^= 1; }
void Setup(uint32_t it, bool place) {
    memset(&g_f, 0, sizeof g_f);
    g_case = it; g_place = place; g_bits = it * 1664525u + 1013904223u;
    uint32_t rng = g_bits;
    for (uint8_t* p = g_f.train; p != (uint8_t*)g_f.tables; ++p) {
        rng = rng * 1664525u + 1013904223u; *p = (uint8_t)(rng >> 16);
    }
    for (int i = 0; i < 7; ++i) g_f.tables[i] = i;
    *(void**)0x6cec78 = place && it % 7 == 0 ? nullptr : g_f.worlds[0];
    *(void**)0x6cec7c = g_f.options[0]; *(void**)0x62ba88 = &g_f.tables[0];
    *(void**)0x6ada84 = &g_f.tables[2]; *(void**)0x62ba84 = &g_f.tables[4]; *(void**)0x62be10 = &g_f.tables[5];
    const uint16_t cws[] = {0x027f, 0x037f, 0x007f, 0x0a7f, 0x0e7f};
    g_cw = cws[(it / 16) % 5]; g_type = it ^ 0x753fb921;
    g_price = kFloats[(it / 13) % 16];
    F<uint32_t>(g_f.train, 0x41) = it % 5 ? 0xffffffff : 13;
    F<uint32_t>(g_f.train, 0x51) = it & 1;
    F<uint32_t>(g_f.train, 0xf5) = kFloats[(it / 3) % 16];
    g_f.train[0xf9] = (uint8_t)it;
    g_f.train[0x197] = (it / 64) % 3; g_f.train[0x198] = (it / 128) % 3;
    g_f.rows[0][0x28] = (uint8_t)(it % 64); g_f.rows[1][0x28] = (uint8_t)~it;
    for (int i = 0; i < 2; ++i) {
        F<uint32_t>(g_f.models[i], 0x72) = (it / (i + 1)) % 3;
        F<uint32_t>(g_f.models[i], 0x76) = 41 + i;
        g_f.models[i][0x7a] = (it / 11 + i) % 2;
        F<uint32_t>(g_f.assets[i], 0xfd) = kFloats[(it / (i ? 7 : 5)) % 16];
        F<uint32_t>(g_f.worlds[i], 0x4af3) = (it / (i ? 5 : 4)) % 2;
        g_f.options[i][0x82] = (it / (i ? 17 : 19)) % 2;
        F<uint32_t>(g_f.companies[i], 0x3b) = 100 + i;
    }
    F<uint32_t>(g_f.node, 0x206) = it % 3 == 0 ? 0xffffffff : 4;
    F<uint32_t>(g_f.node, 0x202) = it ^ 0x7f801234;
    g_f.count = (int32_t)(it % 12) - 2;
}
int Catch(EXCEPTION_POINTERS* e) {
    g_f.fault = e->ExceptionRecord->ExceptionCode;
    if (g_f.fault == EXCEPTION_ACCESS_VIOLATION) {
        g_f.faultKind = (uint32_t)e->ExceptionRecord->ExceptionInformation[0];
        g_f.faultAddress = (uint32_t)e->ExceptionRecord->ExceptionInformation[1];
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
void Invoke(void* fn, bool place) {
    __asm fninit
    __asm fldcw g_cw
    __try {
        if (place) ((void(__fastcall*)(uint8_t*, int, uint32_t, uint8_t*))fn)(g_f.train, 0, g_type, g_f.node);
        else ((void(__fastcall*)(uint8_t*))fn)(g_f.train);
    } __except(Catch(GetExceptionInformation())) {}
    uint16_t cw; __asm fnstcw cw
    g_f.cw = cw;
}
void Run(bool place) {
    Redirects p;
    p.Add(0x4b2bd0, (void*)&Current); p.Add(0x41e100, (void*)&Find);
    p.Add(0x4a9b70, (void*)&Append); p.Add(0x4a7760, (void*)&Count);
    p.Add(0x48a170, (void*)&Center); p.Add(0x518140, (void*)&Get); p.Add(0x41d340, (void*)&Add);
    p.Add(0x41c700, (void*)&Move); p.Add(0x434050, (void*)&Multiplayer); p.Add(0x46d2b0, (void*)&Version);
    p.Add(0x41ce80, (void*)&Legacy); p.Add(0x41b080, (void*)&Modern);
    p.Add(0x4619c0, (void*)&Price); p.Add(0x42a040, (void*)&Stat); p.Add(0x4afeb0, (void*)&Breakdown);
    uint32_t saved[6]; uint16_t savedCw; __asm fnstcw savedCw
    for (int i = 0; i < 6; ++i) saved[i] = *(uint32_t*)kSlots[i];
    uint32_t n = place ? 8192 : 4096;
    uint32_t reachedStats = 0, reachedPose = 0, reachedFill = 0;
    for (uint32_t it = 0; it < n; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            Setup(it, place);
            Invoke(side ? (place ? (void*)&rt3::Train_PlaceInitialEngine : (void*)&rt3::Train_AutoAddCars)
                        : (place ? (void*)0x4b0140 : (void*)0x4acaf0), place);
            for (int i = 0; i < 6; ++i) g_f.globals[i] = *(uint32_t*)kSlots[i];
            if (!side) {
                expected = g_f;
                reachedStats += g_f.stats != 0; reachedPose += g_f.pose != 0; reachedFill += g_f.counts != 0;
            } else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                size_t start = (uint8_t*)g_f.log - (uint8_t*)&g_f;
                uint32_t word = off >= start && off < start + sizeof g_f.log ? (uint32_t)((off - start) / 4) : 0;
                EXPECT_EQ(1, 0, "initialization case %u byte %u %02x/%02x word %u %08x/%08x trace %u/%u fault %08x/%08x",
                    it, (unsigned)off, ((uint8_t*)&expected)[off], ((uint8_t*)&g_f)[off], word, expected.log[word], g_f.log[word],
                    expected.used, g_f.used, expected.fault, g_f.fault);
                it = n;
                break;
            }
        }
    }
    if (place) {
        EXPECT_EQ(true, reachedStats != 0, "pricing branch exercised");
        EXPECT_EQ(true, reachedPose != 0, "pose branch exercised");
    } else EXPECT_EQ(true, reachedFill != 0, "fill loop exercised");
    for (int i = 0; i < 6; ++i) *(uint32_t*)kSlots[i] = saved[i];
    __asm fninit
    __asm fldcw savedCw
}
}
DIFFTEST(game_TrainAutoAddCars) { Run(false); }
DIFFTEST(game_TrainPlaceInitialEngine) { Run(true); }
