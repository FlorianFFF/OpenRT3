// Difftests for the utility layer: random, calendar, text, math.
#include "difftest.h"
#include "hook/hook.h"
#include "game/calendar.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace rt3 {
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void PushSeed();
void PopSeed();
char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);
void __fastcall StrCatN(char* dst, const char* src, int32_t n);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t cs);
int32_t __fastcall StrIsBlank(const unsigned char* s);
double __stdcall Distance3D(float, float, float, float, float, float);
double __stdcall Distance2D(float, float, float, float);
double __stdcall DistanceSq2D(float, float, float, float);
int32_t __fastcall DistanceSqInt(int32_t, int32_t, int32_t, int32_t);
int32_t __fastcall DistanceInt(int32_t, int32_t, int32_t, int32_t);
}
using namespace rt3;

static uint32_t R32() { return (uint32_t)rand() << 17 ^ (uint32_t)rand() << 5 ^ (uint32_t)rand(); }
static int32_t RangeEdge() {
    static const int32_t e[] = { 0, 1, -1, 2, 0x7ffe, 0x7fff, 0x8000, 0x8001, 100, 1000, 0x7fffffff, -0x7fffffff };
    return rand() % 2 ? e[rand() % 12] : (int32_t)R32() >> (rand() % 31);
}
static uint64_t Bits(double d) { uint64_t b; memcpy(&b, &d, 8); return b; }

DIFFTEST(random_HashRand) {
    auto orig = (int32_t(__fastcall*)(int32_t, int32_t, int32_t))0x518cc0;
    srand(11);
    for (int i = 0; i < 200000; ++i) {
        int32_t seed = (int32_t)R32(), lo = RangeEdge(), hi = RangeEdge();
        if ((int64_t)hi - lo > 0x7fffffff) continue; // the original overflows the range too; skip UB
        EXPECT_EQ(orig(seed, lo, hi), HashRand(seed, lo, hi), "seed=%d lo=%d hi=%d", seed, lo, hi);
    }
}

DIFFTEST(random_Rand) {
    auto orig = (int32_t(__fastcall*)(int32_t, int32_t))0x518d70;
    uint32_t& state = hook::Global<uint32_t>(0x624324);
    srand(12);
    for (int i = 0; i < 200000; ++i) {
        int32_t lo = RangeEdge(), hi = RangeEdge();
        if ((int64_t)hi - lo > 0x7fffffff) continue;
        uint32_t s0 = R32();
        state = s0;
        int32_t a = orig(lo, hi);
        uint32_t sa = state;
        state = s0;
        int32_t b = Rand(lo, hi);
        EXPECT_EQ(a, b, "lo=%d hi=%d s=%u", lo, hi, s0);
        EXPECT_EQ(sa, state, "state lo=%d hi=%d s=%u", lo, hi, s0);
    }
}

DIFFTEST(random_SimRand) {
    auto orig = (int32_t(__fastcall*)(int32_t, int32_t))0x518be0;
    uint32_t& state = hook::Global<uint32_t>(0x62431c);
    uint32_t keep = state;
    srand(16);
    for (int i = 0; i < 200000; ++i) {
        int32_t lo = RangeEdge(), hi = RangeEdge();
        if ((int64_t)hi - lo > 0x7fffffff) continue;
        uint32_t s0 = R32();
        state = s0;
        int32_t a = orig(lo, hi);
        uint32_t sa = state;
        state = s0;
        int32_t b = SimRand(lo, hi);
        EXPECT_EQ(a, b, "lo=%d hi=%d s=%u", lo, hi, s0);
        EXPECT_EQ(sa, state, "state lo=%d hi=%d s=%u", lo, hi, s0);
    }
    state = keep;
}

DIFFTEST(random_SeedStack) {
    int32_t& seed = hook::Global<int32_t>(0x62431c);
    int32_t& saved = hook::Global<int32_t>(0x624320);
    int32_t& depth = hook::Global<int32_t>(0x6d3b94);
    auto origPush = (void(*)())0x518c80, origPop = (void(*)())0x518ca0;
    srand(13);
    for (int i = 0; i < 2000; ++i) {
        int32_t s0 = (int32_t)R32(), d0 = rand() % 3, v0 = (int32_t)R32();
        bool push = rand() % 2;
        seed = s0; depth = d0; saved = v0;
        push ? origPush() : origPop();
        int32_t s1 = seed, d1 = depth, v1 = saved;
        seed = s0; depth = d0; saved = v0;
        push ? PushSeed() : PopSeed();
        EXPECT_EQ(s1, seed, "seed"); EXPECT_EQ(d1, depth, "depth"); EXPECT_EQ(v1, saved, "saved");
    }
    seed = 0; depth = 0; saved = 0;
}

DIFFTEST(text_FormatText) {
    auto orig = (char*(__fastcall*)(char*, const char*, const char*, const char*, const char*, const char*,
                                    const char*, const char*, const char*, const char*))0x518de0;
    static const char* fmts[] = {
        "", "plain", "%1", "%2 and %1", "%S %S %S", "%s-%s", "100%% sure", "%%1", "%3%2%1", "%05",
        "%12x", "%10", "%9", "%8", "%0", "%C%F%x%", "a%", "%%%1%% x","%1%S%S%2", "%s%1%s", "x%Sy%10z",
        "%7%6%5%4%3%2%1", "Train %1 arrived at %2 with %3 cars", "%11%12%13",
    };
    const char* args[8] = { "one", "TWO", nullptr, "4", "", "six six", "7", "eight" };
    for (const char* f : fmts) {
        char a[512], b[512];
        memset(a, 0x55, sizeof a);
        memset(b, 0x55, sizeof b);
        orig(a, f, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
        FormatText(b, f, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
        EXPECT_EQ(memcmp(a, b, sizeof a), 0, "fmt '%s' -> '%s' vs '%s'", f, a, b);
    }
}

DIFFTEST(text_StrHelpers) {
    auto origCat = (void(__fastcall*)(char*, const char*, int32_t))0x51d8c0;
    auto origFind = (const char*(__fastcall*)(const char*, const char*, int32_t))0x51d900;
    auto origBlank = (int32_t(__fastcall*)(const unsigned char*))0x51d9a0;
    static const char* strs[] = { "", "a", "abc", "Hello World", "hello", "WORLD", " \t\n", " x ", "aaaa", "aa" };
    for (const char* x : strs) {
        EXPECT_EQ(origBlank((const unsigned char*)x), StrIsBlank((const unsigned char*)x), "blank '%s'", x);
        for (const char* y : strs) {
            for (int cs = 0; cs < 2; ++cs) {
                const char* r1 = origFind(x, y, cs);
                const char* r2 = StrFind(x, y, cs);
                EXPECT_EQ(r1, r2, "find '%s' in '%s' cs=%d", y, x, cs);
            }
            for (int n = -1; n < 16; ++n) {
                char a[64], b[64];
                memset(a, 0x33, sizeof a); memset(b, 0x33, sizeof b);
                strcpy(a, x); strcpy(b, x);
                origCat(a, y, n);
                StrCatN(b, y, n);
                EXPECT_EQ(memcmp(a, b, sizeof a), 0, "cat '%s'+'%s' n=%d", x, y, n);
            }
        }
    }
    EXPECT_EQ(origBlank(nullptr), StrIsBlank(nullptr), "blank NULL");
}

DIFFTEST(calendar_all) {
    auto oFromMin = (uint64_t(__fastcall*)(int32_t))0x51d460;
    auto oToMin = (int32_t(__fastcall*)(const GameDate*))0x51d3c0;
    auto oParts = (int32_t(__fastcall*)(int32_t, int32_t, int32_t, int32_t, int32_t))0x51d390;
    auto oMake = (uint64_t(__fastcall*)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t))0x51d3f0;
    auto oAddMin = (uint64_t(__fastcall*)(int32_t, GameDate))0x51d550;
    auto oAddDays = (uint64_t(__fastcall*)(int32_t, GameDate))0x51d5a0;
    auto oDiff = (int32_t(__stdcall*)(GameDate, GameDate))0x51d5f0;
    srand(14);
    for (int i = 0; i < 200000; ++i) {
        int32_t m = (int32_t)R32();
        if (rand() % 2) m = 1800 * kMinutesPerYear + (int32_t)(R32() % (300u * kMinutesPerYear));
        EXPECT_EQ(oFromMin(m), DateFromMinutes(m), "FromMinutes %d", m);
        uint64_t raw = ((uint64_t)R32() << 32) | R32();
        GameDate d, e;
        memcpy(&d, &raw, 8);
        raw = ((uint64_t)R32() << 32) | R32();
        memcpy(&e, &raw, 8);
        EXPECT_EQ(oToMin(&d), DateToMinutes(&d), "ToMinutes");
        int32_t add = (int32_t)R32() >> (rand() % 32);
        EXPECT_EQ(oAddMin(add, d), DateAddMinutes(add, d), "AddMinutes");
        EXPECT_EQ(oAddDays(add, d), DateAddDays(add, d), "AddDays");
        EXPECT_EQ(oDiff(d, e), DateDiffMinutes(d, e), "Diff");
        EXPECT_EQ(oParts(d.year, d.month, d.day, d.hour, d.minute), MinutesFromParts(d.year, d.month, d.day, d.hour, d.minute), "Parts");
        EXPECT_EQ(oMake(d.year, d.month, d.day, d.hour, d.minute), MakeDate(d.year, d.month, d.day, d.hour, d.minute), "Make");
    }
}

DIFFTEST(math_Distances) {
    auto o3 = (double(__stdcall*)(float, float, float, float, float, float))0x51db40;
    auto o2 = (double(__stdcall*)(float, float, float, float))0x51db80;
    auto oSq = (double(__stdcall*)(float, float, float, float))0x51dbb0;
    auto oSqI = (int32_t(__fastcall*)(int32_t, int32_t, int32_t, int32_t))0x51dbe0;
    auto oI = (int32_t(__fastcall*)(int32_t, int32_t, int32_t, int32_t))0x51dc00;
    srand(15);
    auto F = []() { return (float)((int32_t)R32() % 2000000) / (float)(1 + rand() % 1000); };
    for (int i = 0; i < 100000; ++i) {
        float a = F(), b = F(), c = F(), d = F(), e = F(), f = F();
        EXPECT_EQ(Bits(o3(a, b, c, d, e, f)), Bits(Distance3D(a, b, c, d, e, f)), "3D");
        EXPECT_EQ(Bits(o2(a, b, c, d)), Bits(Distance2D(a, b, c, d)), "2D");
        EXPECT_EQ(Bits(oSq(a, b, c, d)), Bits(DistanceSq2D(a, b, c, d)), "Sq2D");
        int32_t x1 = (int32_t)(R32() % 4000), y1 = (int32_t)(R32() % 4000), x2 = (int32_t)(R32() % 4000), y2 = (int32_t)(R32() % 4000);
        EXPECT_EQ(oSqI(x1, y1, x2, y2), DistanceSqInt(x1, y1, x2, y2), "SqInt");
        EXPECT_EQ(oI(x1, y1, x2, y2), DistanceInt(x1, y1, x2, y2), "Int %d %d %d %d", x1, y1, x2, y2);
    }
}
