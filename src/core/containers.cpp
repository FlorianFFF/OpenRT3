// Containers over a shared node pool (0x53d1d0..0x53ddf0).
#include "containers.h"
#include "crt.h"
#include "hook/hook.h"
#include <string.h>

namespace rt3 {

// --- node pool ------------------------------------------------------------

// Inlined everywhere in the original: storage-backed pools hand out nodes from
// their free list (NULL when exhausted); pools without storage use malloc.
static void* PoolAlloc(NodePool* p) {
    if (p->capacity == 0) return Malloc(p->elemSize);
    SList* f = p->freeList;
    void** n = f->head;
    if (!n) return nullptr;
    f->head = (void**)*n;
    if (n == f->tail) f->tail = (void**)*n;
    f->count--;
    return n;
}

static void PoolRelease(NodePool* p, void* node) {
    if (p->capacity == 0 || (uint8_t*)node < p->begin || (uint8_t*)node >= p->end) {
        Free(node);
        return;
    }
    SList* f = p->freeList;
    *(void***)node = f->head;
    f->count++;
    f->head = (void**)node;
    if (!f->tail) f->tail = (void**)node;
}

static NodePool* NewPool() {
    NodePool* p = (NodePool*)Malloc(sizeof(NodePool));
    if (p) memset(p, 0, sizeof *p);
    return p;
}

void __fastcall Pool_Free(NodePool* p) {
    if (p->freeList) {
        Free(p->freeList);
        p->freeList = nullptr;
    }
    if (p->ownsStorage == 1 && p->begin) {
        Free(p->begin);
        p->begin = nullptr;
    }
}
RT3_REPLACE(0x53dcb0, Pool_Free);

void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag) {
    p->flag = flag;
    if (p->freeList) {
        Free(p->freeList);
        p->freeList = nullptr;
    }
    if (p->ownsStorage == 1 && p->begin) {
        Free(p->begin);
        p->begin = nullptr;
    }
    p->ownsStorage = 0;
    if (!storage) {
        if (count == 0) {
            p->capacity = 0;
            p->elemSize = elemSize;
            return;
        }
        p->ownsStorage = 1;
        storage = Malloc(elemSize * count);
    }
    if (count != 0) {
        p->begin = (uint8_t*)storage;
        p->end = (uint8_t*)storage + elemSize * count;
        SList* f = (SList*)Malloc(sizeof(SList));
        memset(f, 0, sizeof *f);
        p->freeList = f;
        // Free list in address order: push from the last element down.
        for (uint8_t* e = (uint8_t*)storage + (count - 1) * elemSize; e >= (uint8_t*)storage; e -= elemSize) {
            *(void***)e = f->head;
            f->count++;
            f->head = (void**)e;
            if (!f->tail) f->tail = (void**)e;
        }
    }
    p->elemSize = elemSize;
    p->capacity = count;
}
RT3_REPLACE(0x53dcf0, Pool_Init);

// --- max-heap -------------------------------------------------------------

void __fastcall Heap_SiftUp(MaxHeap* h, int, int32_t i) {
    HeapNode* n = h->array[i];
    HeapNode* parent = h->array[i / 2];
    while (parent->key <= n->key) { // QUIRK: ties move up; a key of 0x7fffffff never stops
        h->array[i] = parent;
        parent->slot = &h->array[i];
        i /= 2;
        parent = h->array[i / 2];
    }
    h->array[i] = n;
    n->slot = &h->array[i];
}
RT3_REPLACE(0x53d1d0, Heap_SiftUp);

void __fastcall Heap_SiftDown(MaxHeap* h, int, int32_t i) {
    HeapNode* n = h->array[i];
    while (i <= h->count / 2) {
        int32_t child = i * 2;
        HeapNode* c = h->array[child];
        if (child < h->count && c->key < h->array[child + 1]->key) {
            c = h->array[child + 1];
            ++child;
        }
        if (c->key <= n->key) break;
        h->array[i] = c;
        c->slot = &h->array[i];
        i = child;
    }
    h->array[i] = n;
    n->slot = &h->array[i];
}
RT3_REPLACE(0x53d240, Heap_SiftDown);

int32_t __fastcall Heap_TopKey(MaxHeap* h) { return h->count > 0 ? h->array[1]->key : 0; }
RT3_REPLACE(0x53d2e0, Heap_TopKey);

void __fastcall Heap_DecrementKey(MaxHeap* h, int, HeapNode* n) {
    n->key--;
    Heap_SiftDown(h, 0, (int32_t)(n->slot - h->array));
}
RT3_REPLACE(0x53d300, Heap_DecrementKey);

void __fastcall Heap_Destroy(MaxHeap* h) {
    if (h->array) {
        Free(h->array);
        h->array = nullptr;
    }
    if (NodePool* p = h->pool) {
        Pool_Free(p);
        Free(p);
        h->pool = nullptr;
    }
    if (h->storage) {
        Free(h->storage);
        h->storage = nullptr;
    }
    h->count = 0;
}
RT3_REPLACE(0x53d470, Heap_Destroy);

void __fastcall Heap_Destroy2(MaxHeap* h) { Heap_Destroy(h); } // identical copy in the exe
RT3_REPLACE(0x53d6f0, Heap_Destroy2);

HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value) {
    HeapNode* n = (HeapNode*)PoolAlloc(h->pool);
    if (!n) return nullptr;
    int32_t i = ++h->count;
    h->array[i] = n;
    n->slot = &h->array[i];
    n->key = key;
    n->value = value;
    Heap_SiftUp(h, 0, h->count);
    return n;
}
RT3_REPLACE(0x53d4c0, Heap_Push);

int32_t __fastcall Heap_PopMax(MaxHeap* h) {
    if (h->count <= 0) return 0;
    HeapNode* top = h->array[1];
    int32_t value = top->value;
    PoolRelease(h->pool, *top->slot);
    if (h->count > 1) {
        h->array[1] = h->array[h->count];
        h->count--;
        h->array[1]->slot = &h->array[1];
        Heap_SiftDown(h, 0, 1);
    } else {
        h->count--;
    }
    return value;
}
RT3_REPLACE(0x53d540, Heap_PopMax);

void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity) {
    if (h->pool) Heap_Destroy(h);
    h->storage = Malloc(capacity * sizeof(HeapNode));
    memset(h->storage, 0, capacity * sizeof(HeapNode));
    h->pool = NewPool();
    Pool_Init(h->pool, 0, h->storage, sizeof(HeapNode), capacity, 0);
    uint32_t bytes = capacity * 4 + 4;
    h->array = (HeapNode**)Malloc(bytes);
    memset(h->array, 0, bytes);
    h->sentinel.value = 0;
    h->sentinel.key = 0x7fffffff;
    h->sentinel.slot = nullptr;
    h->array[0] = &h->sentinel;
}
RT3_REPLACE(0x53d740, Heap_Init);

// --- unsorted list with max extraction -----------------------------------

ListNode* __fastcall List_Push(MaxList* l, int, int32_t key, int32_t value) {
    ListNode* n = (ListNode*)PoolAlloc(l->pool);
    if (!n) return nullptr;
    SList* s = l->items;
    n->next = (ListNode*)s->head;
    s->head = (void**)n;
    s->count++;
    if (!s->tail) s->tail = (void**)n;
    n->key = key;
    n->value = value;
    return n;
}
RT3_REPLACE(0x53d320, List_Push);

int32_t __fastcall List_PopMax(MaxList* l) {
    SList* s = l->items;
    ListNode* best = (ListNode*)s->head;
    if (!best) return 0;
    ListNode* bestPrev = nullptr;
    for (ListNode *cur = best, *prev = nullptr; cur; prev = cur, cur = cur->next) {
        if (best->key < cur->key) { // first of equal maxima wins
            best = cur;
            bestPrev = prev;
        }
    }
    if (!bestPrev) {
        ListNode* h = (ListNode*)s->head;
        if (h) {
            s->head = (void**)h->next;
            if ((void**)h == s->tail) s->tail = (void**)h->next;
            s->count--;
        }
    } else {
        bestPrev->next = best->next;
        best->next = nullptr;
        s->count--;
        if ((void**)best == s->tail) s->tail = (void**)bestPrev;
    }
    int32_t value = best->value;
    PoolRelease(l->pool, best);
    return value;
}
RT3_REPLACE(0x53d390, List_PopMax);

int32_t __fastcall List_Contains(MaxList* l, int, int32_t value) {
    for (ListNode* n = (ListNode*)l->items->head; n; n = n->next)
        if (n->value == value) return 1;
    return 0;
}
RT3_REPLACE(0x53d440, List_Contains);

void __fastcall List_Destroy(MaxList* l) {
    if (l->items) {
        Free(l->items);
        l->items = nullptr;
    }
    if (NodePool* p = l->pool) {
        Pool_Free(p);
        Free(p);
        l->pool = nullptr;
    }
    if (l->storage) {
        Free(l->storage);
        l->storage = nullptr;
    }
}
RT3_REPLACE(0x53d5d0, List_Destroy);

void __fastcall List_Init(MaxList* l, int, int32_t capacity) {
    if (l->pool) List_Destroy(l);
    if (capacity > 0) {
        l->storage = Malloc(capacity * sizeof(ListNode));
        memset(l->storage, 0, capacity * sizeof(ListNode));
    }
    l->pool = NewPool();
    Pool_Init(l->pool, 0, l->storage, sizeof(ListNode), capacity, 0);
    SList* s = (SList*)Malloc(sizeof(SList));
    if (s) memset(s, 0, sizeof *s);
    l->items = s;
}
RT3_REPLACE(0x53d630, List_Init);

// --- CRC32 (table 0x624e48, init 0, no final xor) -------------------------

static const uint32_t* const kCrcTable = (const uint32_t*)0x624e48;

uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase) {
    uint32_t crc = 0;
    for (; *s; ++s) {
        uint32_t c = (uint8_t)*s;
        if (ignoreCase == 1) c &= 0xffffffdf; // folds a-z onto A-Z (and other bytes too)
        crc = (crc >> 8) ^ kCrcTable[(c ^ crc) & 0xff];
    }
    return crc;
}
RT3_REPLACE(0x53d810, Crc32String);

uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len) {
    uint32_t crc = 0;
    for (uint32_t i = 0; i < len; ++i) crc = (crc >> 8) ^ kCrcTable[(data[i] ^ crc) & 0xff];
    return crc;
}
RT3_REPLACE(0x53d870, Crc32);

// --- hash map -------------------------------------------------------------

static inline uint32_t Bucket(const HashMap* m, uint32_t key) { return ((key >> 15) ^ key) % m->bucketCount; }

void __fastcall Hash_Destroy(HashMap* m) {
    if (m->pool) {
        if (!m->storage && m->bucketCount) { // nodes came from malloc: release each
            for (uint32_t b = 0; b < m->bucketCount; ++b) {
                for (HashNode* n = m->buckets[b]; n;) {
                    HashNode* next = n->next;
                    PoolRelease(m->pool, n);
                    n = next;
                }
            }
        }
        if (NodePool* p = m->pool) {
            Pool_Free(p);
            Free(p);
        }
        m->pool = nullptr;
    }
    if (m->storage) {
        Free(m->storage);
        m->storage = nullptr;
    }
    if (m->buckets) {
        Free(m->buckets);
        m->buckets = nullptr;
    }
    m->poolSize = 0;
    m->bucketCount = 0;
}
RT3_REPLACE(0x53d8a0, Hash_Destroy);

void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value) {
    HashNode* n = (HashNode*)PoolAlloc(m->pool);
    if (!n) return; // QUIRK: silently dropped when the pool is exhausted
    uint32_t b = Bucket(m, key);
    n->key = key;
    n->value = value;
    n->prev = nullptr;
    n->next = nullptr;
    if (HashNode* head = m->buckets[b]) {
        head->prev = n;
        n->next = m->buckets[b];
    }
    m->buckets[b] = n;
}
RT3_REPLACE(0x53d960, Hash_Insert);

int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    HashNode** bucket = &m->buckets[Bucket(m, key)];
    HashNode* n = *bucket;
    if (!n) return 0;
    while (n->key != key) {
        n = n->next;
        if (!n) return 0;
    }
    int32_t value = n->value;
    if (!n->prev) {
        *bucket = n->next;
        if (n->next) n->next->prev = nullptr;
    } else {
        n->prev->next = n->next;
        if (n->next) n->next->prev = n->prev;
    }
    PoolRelease(m->pool, n);
    return value;
}
RT3_REPLACE(0x53d9e0, Hash_Remove);

int32_t __fastcall Hash_Contains(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    for (HashNode* n = m->buckets[Bucket(m, key)]; n; n = n->next)
        if (n->key == key) return 1;
    return 0;
}
RT3_REPLACE(0x53daa0, Hash_Contains);

int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    for (HashNode* n = m->buckets[Bucket(m, key)]; n; n = n->next)
        if (n->key == key) return n->value;
    return 0;
}
RT3_REPLACE(0x53dae0, Hash_Get);

// Calls fn(node, ctx) for every node (key 0) or every node with `key`, until
// fn returns nonzero; returns that value.
int32_t __fastcall Hash_ForEach(HashMap* m, int, uint32_t key, HashVisit fn, void* ctx) {
    int32_t r = 0;
    if (key == 0) {
        for (uint32_t b = 0; b < m->bucketCount && r == 0; ++b) {
            for (HashNode* n = m->buckets[b]; n;) {
                HashNode* next = n->next; // fetched first: fn may remove n
                r = fn(n, ctx);
                if (r) break;
                n = next;
            }
        }
        return r;
    }
    for (HashNode* n = m->buckets[Bucket(m, key)]; n;) {
        HashNode* next = n->next;
        if (n->key == key && (r = fn(n, ctx)) != 0) break;
        n = next;
    }
    return r;
}
RT3_REPLACE(0x53db20, Hash_ForEach);

int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize) {
    Hash_Destroy(m);
    if (poolSize) {
        m->storage = Malloc(poolSize * sizeof(HashNode));
        memset(m->storage, 0, poolSize * sizeof(HashNode));
    }
    m->pool = NewPool();
    Pool_Init(m->pool, 0, m->storage, sizeof(HashNode), poolSize, 0);
    m->buckets = (HashNode**)Malloc(buckets * 4);
    memset(m->buckets, 0, buckets * 4);
    m->poolSize = poolSize;
    m->bucketCount = buckets;
    return (int32_t)((buckets + poolSize * 4) * 4);
}
RT3_REPLACE(0x53dbf0, Hash_Init);

} // namespace rt3
