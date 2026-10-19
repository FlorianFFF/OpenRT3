// Heap / list / hash map / CRC difftests.
#include "difftest.h"
#include "core/containers.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace rt3;

namespace {

template <typename F> F Fn(uint32_t a) { return (F)a; }

// Free-list contents as storage indices (or -1 for nodes outside storage).
static std::vector<int> FreeOrder(const NodePool* p) {
    std::vector<int> v;
    if (!p || !p->freeList) return v;
    for (void** n = p->freeList->head; n && v.size() < 100000; n = (void**)*n)
        v.push_back((uint8_t*)n >= p->begin && (uint8_t*)n < p->end ? (int)(((uint8_t*)n - p->begin) / p->elemSize) : -1);
    v.push_back(p->freeList->count);
    return v;
}

static std::vector<int64_t> HeapState(const MaxHeap& h) {
    std::vector<int64_t> v{ h.count };
    for (int i = 1; i <= h.count; ++i) {
        v.push_back(h.array[i]->key);
        v.push_back(h.array[i]->value);
        v.push_back(h.array[i]->slot == &h.array[i]);
    }
    for (int x : FreeOrder(h.pool)) v.push_back(x);
    return v;
}

static std::vector<int64_t> ListState(const MaxList& l) {
    std::vector<int64_t> v{ l.items->count };
    for (ListNode* n = (ListNode*)l.items->head; n; n = n->next) { v.push_back(n->key); v.push_back(n->value); }
    v.push_back(l.items->tail ? ((ListNode*)l.items->tail)->value : -1);
    for (int x : FreeOrder(l.pool)) v.push_back(x);
    return v;
}

static std::vector<int64_t> HashState(const HashMap& m) {
    std::vector<int64_t> v{ (int64_t)m.bucketCount, m.poolSize };
    for (uint32_t b = 0; b < m.bucketCount; ++b) {
        v.push_back(-7);
        for (HashNode* n = m.buckets[b]; n; n = n->next) {
            v.push_back(n->key); v.push_back(n->value);
            v.push_back(n->prev ? (int64_t)n->prev->key : -1);
        }
    }
    for (int x : FreeOrder(m.pool)) v.push_back(x);
    return v;
}

static int32_t __fastcall CountVisit(HashNode* n, void* ctx) {
    int* c = (int*)ctx;
    c[0]++;
    c[1] += n->value;
    return (c[2] && n->value % 17 == 0) ? n->value : 0;
}

} // namespace

DIFFTEST(containers_heap) {
    for (int cap : { 64, 300 }) {
        MaxHeap a = {}, b = {};
        Fn<void(__fastcall*)(MaxHeap*, int, int32_t)>(0x53d740)(&a, 0, cap);
        Heap_Init(&b, 0, cap);
        std::vector<HeapNode*> na, nb;
        srand(51 + cap);
        for (int step = 0; step < 20000 && g_fail < 30; ++step) {
            int op = rand() % 10;
            if (op < 5) {
                int32_t k = rand() % 1000 - 300, v = rand();
                HeapNode* x = Fn<HeapNode*(__fastcall*)(MaxHeap*, int, int32_t, int32_t)>(0x53d4c0)(&a, 0, k, v);
                HeapNode* y = Heap_Push(&b, 0, k, v);
                EXPECT_EQ(x == nullptr, y == nullptr, "push null");
                if (x && y) { na.push_back(x); nb.push_back(y); }
            } else if (op < 8) {
                EXPECT_EQ(Fn<int32_t(__fastcall*)(MaxHeap*)>(0x53d540)(&a), Heap_PopMax(&b), "pop");
                na.clear(); nb.clear(); // popped nodes may be reused; stop tracking
            } else if (op < 9) {
                EXPECT_EQ(Fn<int32_t(__fastcall*)(MaxHeap*)>(0x53d2e0)(&a), Heap_TopKey(&b), "top");
            } else if (!na.empty()) {
                size_t i = rand() % na.size();
                Fn<void(__fastcall*)(MaxHeap*, int, HeapNode*)>(0x53d300)(&a, 0, na[i]);
                Heap_DecrementKey(&b, 0, nb[i]);
            }
            EXPECT_EQ(HeapState(a) == HeapState(b), true, "heap state step %d", step);
        }
        Fn<void(__fastcall*)(MaxHeap*)>(0x53d470)(&a);
        Heap_Destroy(&b);
    }
}

DIFFTEST(containers_list) {
    for (int cap : { 0, 50 }) {
        MaxList a = {}, b = {};
        Fn<void(__fastcall*)(MaxList*, int, int32_t)>(0x53d630)(&a, 0, cap);
        List_Init(&b, 0, cap);
        srand(61 + cap);
        for (int step = 0; step < 10000 && g_fail < 30; ++step) {
            int op = rand() % 10;
            if (op < 5) {
                int32_t k = rand() % 50, v = rand() % 1000;
                ListNode* x = Fn<ListNode*(__fastcall*)(MaxList*, int, int32_t, int32_t)>(0x53d320)(&a, 0, k, v);
                ListNode* y = List_Push(&b, 0, k, v);
                EXPECT_EQ(x == nullptr, y == nullptr, "push null");
            } else if (op < 8) {
                EXPECT_EQ(Fn<int32_t(__fastcall*)(MaxList*)>(0x53d390)(&a), List_PopMax(&b), "popmax");
            } else {
                int32_t v = rand() % 1000;
                EXPECT_EQ(Fn<int32_t(__fastcall*)(MaxList*, int, int32_t)>(0x53d440)(&a, 0, v), List_Contains(&b, 0, v), "contains");
            }
            EXPECT_EQ(ListState(a) == ListState(b), true, "list state step %d", step);
        }
        Fn<void(__fastcall*)(MaxList*)>(0x53d5d0)(&a);
        List_Destroy(&b);
    }
}

DIFFTEST(containers_hash) {
    for (int pool : { 0, 40, 400 }) {
        HashMap a = {}, b = {};
        EXPECT_EQ(Fn<int32_t(__fastcall*)(HashMap*, int, uint32_t, int32_t)>(0x53dbf0)(&a, 0, 37, pool),
                  Hash_Init(&b, 0, 37, pool), "init");
        srand(71 + pool);
        for (int step = 0; step < 20000 && g_fail < 30; ++step) {
            int op = rand() % 12;
            uint32_t key = rand() % 8 == 0 ? 0 : (uint32_t)(rand() % 300) * 0x9e37u;
            if (op < 5) {
                int32_t v = rand();
                Fn<void(__fastcall*)(HashMap*, int, uint32_t, int32_t)>(0x53d960)(&a, 0, key, v);
                Hash_Insert(&b, 0, key, v);
            } else if (op < 7) {
                EXPECT_EQ(Fn<int32_t(__fastcall*)(HashMap*, int, uint32_t)>(0x53d9e0)(&a, 0, key), Hash_Remove(&b, 0, key), "remove");
            } else if (op < 9) {
                EXPECT_EQ(Fn<int32_t(__fastcall*)(HashMap*, int, uint32_t)>(0x53daa0)(&a, 0, key), Hash_Contains(&b, 0, key), "contains");
                EXPECT_EQ(Fn<int32_t(__fastcall*)(HashMap*, int, uint32_t)>(0x53dae0)(&a, 0, key), Hash_Get(&b, 0, key), "get");
            } else {
                int ca[3] = { 0, 0, rand() % 2 }, cb[3] = { 0, 0, ca[2] };
                uint32_t k = rand() % 2 ? 0 : key;
                int32_t ra = Fn<int32_t(__fastcall*)(HashMap*, int, uint32_t, HashVisit, void*)>(0x53db20)(&a, 0, k, CountVisit, ca);
                int32_t rb = Hash_ForEach(&b, 0, k, CountVisit, cb);
                EXPECT_EQ(ra, rb, "foreach ret");
                EXPECT_EQ(memcmp(ca, cb, sizeof ca), 0, "foreach visits");
            }
            EXPECT_EQ(HashState(a) == HashState(b), true, "hash state step %d", step);
        }
        Fn<void(__fastcall*)(HashMap*)>(0x53d8a0)(&a);
        Hash_Destroy(&b);
        EXPECT_EQ(memcmp(&a, &b, sizeof a), 0, "destroyed");
    }
}

DIFFTEST(containers_crc) {
    static const char* strs[] = { "", "a", "BuildingDetail.win", "buildingdetail.WIN", "\xe9\xff\x80", "Coal.cty" };
    for (const char* s : strs)
        for (int ci = 0; ci < 3; ++ci)
            EXPECT_EQ(Fn<uint32_t(__fastcall*)(const char*, int32_t)>(0x53d810)(s, ci), Crc32String(s, ci), "crc '%s' %d", s, ci);
    uint8_t buf[4096];
    srand(81);
    for (auto& x : buf) x = (uint8_t)rand();
    for (uint32_t n : { 0u, 1u, 7u, 100u, 4096u })
        EXPECT_EQ(Fn<uint32_t(__fastcall*)(const uint8_t*, uint32_t)>(0x53d870)(buf, n), Crc32(buf, n), "crc data %u", n);
}
