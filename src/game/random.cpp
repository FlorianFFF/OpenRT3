// Random numbers (0x518be0..0x518d70).
//
// Three generators:
// - SimRand(lo, hi): the simulation's generator, state in 0x62431c. Seeded
//   from the clock by the Setup screen in single player, from the host's value
//   (new-game config +0x83) in multiplayer; scoped fixed seeds via
//   PushSeed/SetSeed/PopSeed. The main determinism anchor.
// - HashRand(seed, lo, hi): stateless; the same seed always gives the same
//   value. The world/game code passes a seed it derives from the global seed
//   (0x62431c) plus object data, which keeps MP peers in step.
// - Rand(lo, hi): classic LCG with global state (0x624324). Only safe in MP if
//   every peer makes the same calls in the same order.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

static int32_t&  g_seed      = hook::Global<int32_t>(0x62431c);
static int32_t&  g_seedSaved = hook::Global<int32_t>(0x624320);
static int32_t&  g_seedDepth = hook::Global<int32_t>(0x6d3b94);
static uint32_t& g_lcg       = hook::Global<uint32_t>(0x624324);

// 0x518be0. Value in [lo, hi] from the simulation generator. Ranges wider than
// 15 bits combine two draws (first << 15 | second).
int32_t __fastcall SimRand(int32_t lo, int32_t hi) {
    int32_t range = hi - lo;
    if (range > 0x7fff) {
        uint32_t first = (uint32_t)SimRand(0, 0x7fff);
        uint32_t second = (uint32_t)SimRand(0, 0x7fff);
        return (int32_t)((first << 15 | second) % (uint32_t)(range + 1)) + lo;
    }
    if (hi <= lo) return hi;
    uint32_t s = (uint32_t)g_seed;
    s = s * 0x41c64e6d + (uint32_t)(int32_t)(int16_t)(s >> 16) + 0x3039;
    g_seed = (int32_t)s;
    return (int32_t)(s & 0x7fffffff) % (range + 1) + lo;
}
RT3_REPLACE(0x518be0, SimRand);

int32_t GetSeed() { return g_seed; }
RT3_REPLACE(0x518c60, GetSeed);

void __fastcall SetSeed(int32_t seed) { g_seed = seed; }
RT3_REPLACE(0x518c70, SetSeed);

// Nested save/restore of the global seed.
void PushSeed() {
    if (g_seedDepth == 0) g_seedSaved = g_seed;
    g_seedDepth++;
}
RT3_REPLACE(0x518c80, PushSeed);

void PopSeed() {
    g_seedDepth--;
    if (g_seedDepth == 0) {
        g_seed = g_seedSaved;
        g_seedSaved = -1;
    }
}
RT3_REPLACE(0x518ca0, PopSeed);

// 0x518cc0. Value in [lo, hi] derived from `seed` alone. Ranges wider than
// 15 bits combine two draws (seed+7 for the low half, seed for the high half).
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi) {
    if (hi < lo) return lo;
    int32_t range = hi - lo;
    if (range > 0x7fff) {
        uint32_t low = (uint32_t)HashRand(seed + 7, 0, 0x7fff);
        uint32_t high = (uint32_t)HashRand(seed, 0, 0x7fff);
        return (int32_t)((low | high << 15) % (uint32_t)(range + 1)) + lo;
    }
    if (hi == lo) return hi;
    int32_t v = (((seed >> 16) - seed * 0x3e39b193 + 0x3039) >> 16) & 0x7fff;
    return v % (range + 1) + lo;
}
RT3_REPLACE(0x518cc0, HashRand);

// 0x518d50. Reseeds both the CRT rand() and the game LCG with 7.
void ResetRand() {
    ((void(__cdecl*)(unsigned))0x5a299e)(7); // srand
    g_lcg = 7;
}
RT3_REPLACE(0x518d50, ResetRand);

static inline uint32_t LcgNext(uint32_t s) { return s * 0x19660d + 0x3c6ef35f; }

// 0x518d70. Value in [lo, hi] from the global LCG.
int32_t __fastcall Rand(int32_t lo, int32_t hi) {
    if (hi == lo) return hi;
    if (hi < lo) return lo;
    int32_t range = hi - lo;
    uint32_t a = LcgNext(g_lcg);
    if (range > 0x7fff) {
        uint32_t b = LcgNext(a);
        g_lcg = b;
        return (int32_t)(((b & 0x7fff) | (a & 0x7fff) << 15) % (uint32_t)(range + 1)) + lo;
    }
    g_lcg = a;
    return (int32_t)(a & 0x7fff) % (range + 1) + lo;
}
RT3_REPLACE(0x518d70, Rand);

} // namespace rt3
