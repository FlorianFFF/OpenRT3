#include "difftest.h"
#include "game/collection.h"
#include <string.h>
#include <stdlib.h>

using namespace rt3;

namespace {

// Original methods (thiscall), reached through __fastcall with a dummy EDX.
struct Orig {
    static Collection* Construct(Collection* c, int a, int b, int d, int e, int f, int g, int h) {
        return ((Collection*(__fastcall*)(Collection*, int, int, int, int, int, int, int, int))0x518b90)(c, 0, a, b, d, e, f, g, h);
    }
    static uint32_t Allocate(Collection* c, const void* d, uint32_t sz, uint32_t k2, uint32_t k3) {
        return ((uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t))0x518900)(c, 0, d, sz, k2, k3);
    }
    static void Release(Collection* c, uint32_t id) { ((void(__fastcall*)(Collection*, int, uint32_t))0x518a30)(c, 0, id); }
    static uint32_t IdAt(Collection* c, uint32_t p, int l) { return ((uint32_t(__fastcall*)(Collection*, int, uint32_t, int))0x518380)(c, 0, p, l); }
    static uint32_t PositionOf(Collection* c, uint32_t id, int l) { return ((uint32_t(__fastcall*)(Collection*, int, uint32_t, int))0x5184f0)(c, 0, id, l); }
    static void* Get(Collection* c, uint32_t id) { return ((void*(__fastcall*)(Collection*, int, uint32_t))0x518140)(c, 0, id); }
    static uint32_t GetKey1(Collection* c, uint32_t id) { return ((uint32_t(__fastcall*)(Collection*, int, uint32_t))0x5181a0)(c, 0, id); }
    static uint32_t IsFree(Collection* c, uint32_t id) { return ((uint32_t(__fastcall*)(Collection*, int, uint32_t))0x517d00)(c, 0, id); }
    static int32_t IsValid(Collection* c, uint32_t id) { return ((int32_t(__fastcall*)(Collection*, int, uint32_t))0x517d40)(c, 0, id); }
    static void SetKey1(Collection* c, uint32_t id, uint32_t k) { ((void(__fastcall*)(Collection*, int, uint32_t, uint32_t))0x518ab0)(c, 0, id, k); }
    static void Swap(Collection* c, uint32_t a, uint32_t b, int l) { ((void(__fastcall*)(Collection*, int, uint32_t, uint32_t, int))0x518b20)(c, 0, a, b, l); }
    static void Destruct(Collection* c) { ((void(__fastcall*)(Collection*))0x518bd0)(c); }
};

// Compares everything observable; pointers are compared by content.
static const char* g_op = "";
static char g_opbuf[128];
static uint32_t g_preA[8], g_preB[8];
static void CompareState(const Collection& a, const Collection& b, int step) {
    int failBefore = g_fail;
    EXPECT_EQ(a.inlineRecords, b.inlineRecords, "step %d inline", step);
    EXPECT_EQ(a.stride, b.stride, "step %d stride", step);
    EXPECT_EQ(a.capacity, b.capacity, "step %d capacity", step);
    EXPECT_EQ(a.count, b.count, "step %d count", step);
    EXPECT_EQ(a.sequence, b.sequence, "step %d sequence", step);
    for (int k = 0; k < 4; ++k) {
        EXPECT_EQ(a.listEnabled[k], b.listEnabled[k], "step %d listEnabled[%d]", step, k);
        EXPECT_EQ(a.head[k], b.head[k], "step %d head[%d]", step, k);
        EXPECT_EQ(a.tail[k], b.tail[k], "step %d tail[%d]", step, k);
        EXPECT_EQ(a.cachePos[k], b.cachePos[k], "step %d cachePos[%d]", step, k);
        if (a.cachePos[k] != kCacheInvalid)
            EXPECT_EQ(a.cacheId[k], b.cacheId[k], "step %d cacheId[%d]", step, k);
        if (k && a.listEnabled[k] && a.capacity == b.capacity)
            EXPECT_EQ(memcmp(a.nodes[k], b.nodes[k], a.capacity * 12), 0, "step %d nodes[%d]", step, k);
    }
    if (g_fail != failBefore && g_fail < 50) {
        fprintf(g_out, "    op %s count=%d\n", g_op, a.count);
        for (int k = 0; k < 4; ++k)
            fprintf(g_out, "    cache[%d] before a=%d/%d b=%d/%d  after a=%d/%d b=%d/%d\n", k,
                    g_preA[k], g_preA[4 + k], g_preB[k], g_preB[4 + k],
                    a.cachePos[k], a.cacheId[k], b.cachePos[k], b.cacheId[k]);
    }
    if (a.capacity != b.capacity || a.capacity == 0) return;
    EXPECT_EQ(memcmp(a.freeBits, b.freeBits, (a.capacity + 8) >> 3), 0, "step %d freeBits", step);
    for (uint32_t i = 1; i < a.capacity; ++i) {
        if (a.freeBits[i >> 3] & (1 << (i & 7))) continue;
        const uint8_t* sa = a.slots + a.stride * i;
        const uint8_t* sb = b.slots + b.stride * i;
        if (a.inlineRecords) {
            EXPECT_EQ(memcmp(sa, sb, a.stride), 0, "step %d slot %u", step, i);
        } else {
            uint16_t na = *(const uint16_t*)(sa + 4), nb = *(const uint16_t*)(sb + 4);
            EXPECT_EQ(na, nb, "step %d slot %u size", step, i);
            if (na == nb) EXPECT_EQ(memcmp(*(void* const*)sa, *(void* const*)sb, na), 0, "step %d slot %u data", step, i);
        }
    }
}

// Picks a random live ID via the original IdAt and copies the cache it leaves
// to b, so both sides stay in step for the operation under test.
static uint32_t PickId(Collection& a, Collection& b, int live) {
    uint32_t id = Orig::IdAt(&a, rand() % live, 0);
    memcpy(b.cachePos, a.cachePos, sizeof a.cachePos);
    memcpy(b.cacheId, a.cacheId, sizeof a.cacheId);
    return id;
}

static void RunScenario(int seed, int inl, int stride, int cap, int grow, int l1, int l2, int l3) {
    Collection a, b;
    memset(&a, 0, sizeof a);
    memset(&b, 0, sizeof b);
    Orig::Construct(&a, inl, stride, cap, grow, l1, l2, l3);
    Collection_Construct(&b, 0, inl, stride, cap, grow, l1, l2, l3);
    CompareState(a, b, -1);
    srand(seed);
    uint8_t buf[64];
    for (int step = 0; step < 20000 && g_fail < 50; ++step) {
        int op = rand() % 100;
        sprintf(g_opbuf, "op%d", op); g_op = g_opbuf;
        memcpy(g_preA, a.cachePos, 16); memcpy(g_preA + 4, a.cacheId, 16);
        memcpy(g_preB, b.cachePos, 16); memcpy(g_preB + 4, b.cacheId, 16);
        int live = a.count;
        if (op < 35 || live == 0) {
            for (auto& x : buf) x = (uint8_t)rand();
            uint32_t size = 1 + rand() % 40;
            uint32_t k2 = rand() % 50, k3 = rand() % 50;
            const void* data = (rand() % 8) ? buf : nullptr;
            if (!inl && !data) data = buf; // original leaves heap data uninitialised
            EXPECT_EQ(Orig::Allocate(&a, data, size, k2, k3), Collection_Allocate(&b, 0, data, size, k2, k3), "step %d alloc", step);
            if (inl && !data) { // uninitialised inline slot: make both equal
                for (uint32_t i = 1; i < a.capacity; ++i)
                    if (!(a.freeBits[i >> 3] & (1 << (i & 7)))) memcpy(b.slots + b.stride * i, a.slots + a.stride * i, a.stride);
            }
        } else if (op < 55) {
            uint32_t id = PickId(a, b, live);
            Orig::Release(&a, id);
            Collection_Release(&b, 0, id);
        } else if (op < 75) {
            int list = rand() % 4;
            if (list && !a.listEnabled[list]) list = 0;
            uint32_t pos = rand() % live;
            if (rand() % 3 == 0 && a.cachePos[list] != kCacheInvalid) pos = a.cachePos[list] + 1 - (rand() % 3);
            if (pos >= (uint32_t)live) pos = live - 1;
            sprintf(g_opbuf, "IdAt(%u,%d) cacheBefore=%u/%u", pos, list, a.cachePos[list], a.cacheId[list]); g_op = g_opbuf;
            EXPECT_EQ(Orig::IdAt(&a, pos, list), Collection_IdAt(&b, 0, pos, list), "step %d IdAt(%u,%d)", step, pos, list);
        } else if (op < 82) {
            uint32_t id = rand() % (a.capacity + 2);
            EXPECT_EQ(Orig::IsFree(&a, id), Collection_IsFree(&b, 0, id), "step %d IsFree", step);
            EXPECT_EQ(Orig::IsValid(&a, id), Collection_IsValid(&b, 0, id), "step %d IsValid", step);
            void* ga = Orig::Get(&a, id); void* gb = Collection_Get(&b, 0, id);
            EXPECT_EQ(ga == nullptr, gb == nullptr, "step %d Get", step);
            if (a.listEnabled[1]) EXPECT_EQ(Orig::GetKey1(&a, id), Collection_GetKey1(&b, 0, id), "step %d GetKey1", step);
        } else if (op < 90) {
            int list = rand() % 4;
            if (list && !a.listEnabled[list]) list = 0;
            uint32_t id = PickId(a, b, live);
            EXPECT_EQ(Orig::PositionOf(&a, id, list), Collection_PositionOf(&b, 0, id, list), "step %d PositionOf", step);
        } else if (op < 95 && a.listEnabled[1]) {
            uint32_t id = PickId(a, b, live);
            uint32_t key = rand() % 100000;
            Orig::SetKey1(&a, id, key);
            Collection_SetKey1(&b, 0, id, key);
        } else if (live >= 2) {
            int list = 1 + rand() % 3;
            if (!a.listEnabled[list]) { CompareState(a, b, step); continue; }
            uint32_t x = PickId(a, b, live);
            uint32_t y = PickId(a, b, live);
            if (x == y) { CompareState(a, b, step); continue; }
            Orig::Swap(&a, x, y, list);
            Collection_SwapOrder(&b, 0, x, y, list);
        }
        CompareState(a, b, step);
    }
    Orig::Destruct(&a);
    Collection_Destruct(&b);
    CompareState(a, b, 99999);
}

} // namespace

DIFFTEST(collection_inline_nolists)  { RunScenario(1, 1, 0x20, 10, 5, 0, 0, 0); }
DIFFTEST(collection_inline_alllists) { RunScenario(2, 1, 0x14, 4, 3, 1, 1, 1); }
DIFFTEST(collection_inline_list2)    { RunScenario(3, 1, 7, 1, 1, 0, 1, 0); }
DIFFTEST(collection_heap_lists)      { RunScenario(4, 0, 0, 2, 8, 1, 0, 1); }
DIFFTEST(collection_heap_nolists)    { RunScenario(5, 0, 0, 50, 50, 0, 0, 0); }
