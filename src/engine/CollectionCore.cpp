// CollectionCore.cpp - RT3.exe 0x517c00-0x51a000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"

namespace rt3 {

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

int32_t __fastcall FileExists(const char* path);
void __fastcall String_Assign(char** s, const char* src);
extern int g_traceMaskPointer;

 // core/stream.cpp (save tracing)

static CollectionVtbl* const kVtbl = reinterpret_cast<CollectionVtbl*>(0x5d1aa0);

struct BuiltinString { int32_t id; const char* text; };
struct BuiltinTable { const BuiltinString* entries; int32_t count; };
#include "game/builtin_strings.inc"
struct StringList { int32_t count; char** items; };

char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

// Functions of this file, in exe order.
RT3_NOINLINE Collection* __fastcall Collection_SetVtbl(Collection* c);
RT3_NOINLINE int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IsFree(Collection* c, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
RT3_NOINLINE void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);
void __fastcall Collection_Grow(Collection* c, int, uint32_t by);
RT3_NOINLINE void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
RT3_NOINLINE uint32_t __fastcall Collection_GetKey1(Collection* c, int, uint32_t id);
void __fastcall Collection_Unlink(Collection* c, int, uint32_t id, int list);
void __fastcall Collection_Link(Collection* c, int, uint32_t id, int list, uint32_t key);
RT3_NOINLINE uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
RT3_NOINLINE uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
RT3_NOINLINE void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1, int32_t list2, int32_t list3);
void __fastcall Collection_Clear(Collection* c);
RT3_NOINLINE int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
RT3_NOINLINE void __fastcall Collection_Release(Collection* c, int, uint32_t id);
RT3_NOINLINE void __fastcall Collection_SetKey1(Collection* c, int, uint32_t id, uint32_t key);
void __fastcall Collection_SwapOrder(Collection* c, int, uint32_t a, uint32_t b, int list);
RT3_NOINLINE Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1, int32_t list2, int32_t list3);
RT3_NOINLINE void __fastcall Collection_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t GetSeed();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
void PushSeed();
void PopSeed();
RT3_NOINLINE int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void ResetRand();
RT3_NOINLINE int32_t __fastcall Rand(int32_t lo, int32_t hi);
RT3_NOINLINE char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
RT3_NOINLINE void __fastcall StringList_Free(StringList* l);
RT3_NOINLINE void Language_Select();
RT3_NOINLINE void FreeLanguageStrings();
RT3_NOINLINE const char* __fastcall GetString(int32_t id);
RT3_NOINLINE int __fastcall ToUpper(char c);

static inline bool BitFree(const Collection* c, uint32_t id) {
    return (c->freeBits[id >> 3] & (1u << (id & 7))) != 0;
}

static inline bool Live(const Collection* c, uint32_t id) {
    return id != 0 && id < c->capacity && !BitFree(c, id);
}

static inline void InvalidateCaches(Collection* c) {
    for (int k = 0; k < 4; ++k) c->cachePos[k] = kCacheInvalid;
}

static inline uint32_t LcgNext(uint32_t s) { return s * 0x19660d + 0x3c6ef35f; }

static const char* Builtin(int32_t lang, int32_t id) {
    const BuiltinTable& t = kBuiltinTables[lang];
    int32_t lo = 0, hi = t.count - 1;
    while (lo <= hi) {
        int32_t mid = (lo + hi) / 2;
        if (t.entries[mid].id == id) return t.entries[mid].text;
        if (t.entries[mid].id < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}

// QUIRK: like the original, a negative id indexes before the loaded array.
static const char* Loaded(int32_t id) {
    StringList* l = g_6d3c94;
    if (!l || !l->items || l->count <= id) return nullptr;
    return l->items[id];
}

RT3_NOINLINE Collection* __fastcall Collection_SetVtbl(Collection* c) { c->vtbl = kVtbl; return c; }

 // returns this
RT3_REPLACE(0x517ce0, Collection_SetVtbl);

RT3_NOINLINE int32_t __fastcall Collection_Count(Collection* c) { return c->count; }
RT3_REPLACE(0x517cf0, Collection_Count);

uint32_t __fastcall Collection_IsFree(Collection* c, int, uint32_t id) {
    if (id != 0 && id < c->capacity) return c->freeBits[id >> 3] & (1u << (id & 7));
    return 1;
}
RT3_REPLACE(0x517d00, Collection_IsFree);

RT3_NOINLINE int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id) { return Live(c, id) ? 1 : 0; }

 // full 0/1 in EAX
RT3_REPLACE(0x517d40, Collection_IsValid);

RT3_NOINLINE void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize) {
    StreamWrite(s, &c->inlineRecords, 4, 0);
    StreamWrite(s, &c->stride, 4, 0);
    StreamWrite(s, &c->initialCapacity, 4, 0);
    StreamWrite(s, &c->growBy, 4, 0);
    StreamWrite(s, &c->capacity, 4, 0);
    StreamWrite(s, &c->count, 4, 0);
    StreamWrite(s, &c->sequence, 4, 0);
    for (int k = 0; k < 4; ++k) {
        StreamWrite(s, &c->listEnabled[k], 4, 0);
        StreamWrite(s, &c->head[k], 4, 0);
        StreamWrite(s, &c->tail[k], 4, 0);
        if (k > 0 && c->listEnabled[k]) StreamWrite(s, c->nodes[k], c->capacity * 12, 0);
    }
    StreamWrite(s, c->freeBits, (c->capacity + 8) >> 3, 0);
    for (int32_t i = 0; i < (int32_t)c->capacity; ++i) {
        if (!Live(c, (uint32_t)i)) continue;
        int32_t n = (c->inlineRecords && recordSize > 0 && recordSize < c->stride) ? recordSize : c->stride;
        uint8_t* slot = c->slots + c->stride * i;
        if (!c->inlineRecords) g_traceMaskPointer = 1;
        StreamWrite(s, slot, n, 0); // QUIRK: non-inline slots write their heap pointer too
        g_traceMaskPointer = 0;
        if (c->inlineRecords) {
            if (recordSize > c->stride) { // pad newer, larger records with zeros
                uint32_t pad = recordSize - c->stride;
                void* z = Malloc(pad);
                memset(z, 0, pad);
                StreamWrite(s, z, pad, 0);
                Free(z);
            }
        } else {
            StreamWrite(s, slot + 4, 2, 0);
            StreamWrite(s, *(void**)slot, *(uint16_t*)(slot + 4), 0);
        }
    }
}
RT3_REPLACE(0x517d90, Collection_Save);

void __fastcall Collection_Grow(Collection* c, int, uint32_t by) {
    uint32_t oldCap = c->capacity;
    uint32_t newCap = oldCap + by;

    uint8_t* slots = (uint8_t*)Malloc(c->stride * newCap);
    if (c->slots) {
        memcpy(slots, c->slots, c->stride * oldCap);
        Free(c->slots);
    }
    c->slots = slots; // new slots are left uninitialised, as in the original

    uint32_t newBits = (newCap + 8) >> 3;
    uint32_t oldBits = oldCap ? (oldCap + 8) >> 3 : 0;
    uint8_t* bits = (uint8_t*)Malloc(newBits);
    if (c->freeBits) {
        memcpy(bits, c->freeBits, oldBits);
        Free(c->freeBits);
    }
    c->freeBits = bits;
    if ((int32_t)(newBits - oldBits) > 0) memset(bits + oldBits, 0xff, newBits - oldBits);

    for (int k = 1; k < 4; ++k) {
        if (!c->listEnabled[k]) continue;
        CollectionNode* n = (CollectionNode*)Malloc(newCap * 12);
        if (oldCap * 12 != 0) {
            memcpy(n, c->nodes[k], oldCap * 12);
            Free(c->nodes[k]);
        }
        memset(n + oldCap, 0, (newCap - oldCap) * 12);
        c->nodes[k] = n;
    }
    c->capacity = newCap;
    InvalidateCaches(c);
}
RT3_REPLACE(0x517f90, Collection_Grow);

RT3_NOINLINE void* __fastcall Collection_Get(Collection* c, int, uint32_t id) {
    if (!Live(c, id)) return nullptr;
    uint8_t* slot = c->slots + c->stride * id;
    return c->inlineRecords ? (void*)slot : *(void**)slot;
}
RT3_REPLACE(0x518140, Collection_Get);

RT3_NOINLINE uint32_t __fastcall Collection_GetKey1(Collection* c, int, uint32_t id) {
    return Live(c, id) ? c->nodes[1][id].key : 0;
}
RT3_REPLACE(0x5181a0, Collection_GetKey1);

void __fastcall Collection_Unlink(Collection* c, int, uint32_t id, int list) {
    if (list == 0) return;
    CollectionNode* nodes = c->nodes[list];
    c->cachePos[list] = kCacheInvalid;
    uint32_t prev = nodes[id].prev;
    uint32_t next = nodes[id].next;
    if (prev) nodes[prev].next = next;
    if (next) c->nodes[list][next].prev = prev;
    if (c->head[list] == id) c->head[list] = next;
    if (c->tail[list] == id) c->tail[list] = prev;
    InvalidateCaches(c);
}
RT3_REPLACE(0x5181f0, Collection_Unlink);

void __fastcall Collection_Link(Collection* c, int, uint32_t id, int list, uint32_t key) {
    if (list == 0) return;
    CollectionNode* nodes = c->nodes[list];
    uint32_t after = c->tail[list];
    c->cachePos[list] = kCacheInvalid;
    nodes[id].key = key;
    if (after == 0) {
        c->tail[list] = id;
        c->head[list] = id;
        nodes[id].prev = 0;
        nodes[id].next = 0;
    } else {
        // Walk back from the tail to the last node with key <= new key.
        uint32_t cur = after;
        for (;;) {
            uint32_t at = cur;
            if (nodes[at].key <= key) {
                if (at != 0) {
                    if (at == c->tail[list]) {
                        nodes[id].next = 0;
                        nodes[id].prev = at;
                        nodes[after].next = id;
                        c->tail[list] = id;
                    } else {
                        nodes[id].next = after;
                        nodes[id].prev = at;
                        nodes[after].prev = id;
                        nodes[at].next = id;
                    }
                    InvalidateCaches(c);
                    return;
                }
                break;
            }
            cur = nodes[at].prev;
            after = at;
            if (cur == 0) break;
        }
        // Smaller than everything: new head.
        nodes[id].next = c->head[list];
        nodes[id].prev = 0;
        nodes[after].prev = id;
        c->head[list] = id;
    }
    InvalidateCaches(c);
}
RT3_REPLACE(0x518260, Collection_Link);

RT3_NOINLINE uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list) {
    if (list != 0) {
        CollectionNode* nodes = c->nodes[list];
        if (c->cachePos[list] == pos - 1) { // sequential walk: one step from the cache
            uint32_t id = nodes[c->cacheId[list]].next;
            c->cachePos[list] = pos;
            c->cacheId[list] = id;
            return id;
        }
        uint32_t id;
        if ((uint32_t)c->count >> 1 < pos) {
            pos = (c->count - pos) - 1; // QUIRK: this, not the real position, goes into the cache
            id = c->tail[list];
            for (uint32_t n = pos; n; --n) id = nodes[id].prev;
        } else {
            id = c->head[list];
            for (uint32_t n = pos; n; --n) id = nodes[id].next;
        }
        c->cachePos[list] = pos;
        c->cacheId[list] = id;
        return id;
    }

    // Slot order: the pos-th live slot.
    uint32_t i, seen;
    uint32_t cached = c->cachePos[0];
    if (cached == pos - 1) { // next after the cached slot
        i = c->cacheId[0] + 1;
        seen = pos;
    } else {
        bool backward = true;
        if (cached == pos + 1) {
            i = c->cacheId[0]; // previous before the cached slot
        } else if (cached == pos) {
            return c->cacheId[0];
        } else if (pos != (uint32_t)c->count - 1) {
            backward = false; // count from the start
            i = 0;
            seen = 0;
        } else {
            i = c->capacity; // last live slot
        }
        if (backward) {
            do {
                --i;
                if (Collection_IsFree(c, 0, i) == 0) {
                    c->cachePos[0] = pos;
                    c->cacheId[0] = i;
                    return i;
                }
            } while (i != 0);
            return 0;
        }
    }
    for (;; ++i) {
        if (c->capacity <= i) {
            c->cachePos[0] = pos;
            c->cacheId[0] = 0;
            return 0;
        }
        if (Live(c, i)) {
            if (pos <= seen) {
                c->cachePos[0] = pos;
                c->cacheId[0] = i;
                return i;
            }
            ++seen;
        }
    }
}
RT3_REPLACE(0x518380, Collection_IdAt);

RT3_NOINLINE uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list) {
    uint32_t n = 0;
    if (list != 0) {
        // QUIRK: no end check, loops forever if id is not in the list.
        for (uint32_t cur = c->head[list]; cur != id; cur = c->nodes[list][cur].next) ++n;
        return n;
    }
    for (uint32_t i = 0; i < c->capacity; ++i) {
        if (!Live(c, i)) continue;
        if (i == id) return n;
        ++n;
    }
    return 0;
}
RT3_REPLACE(0x5184f0, Collection_PositionOf);

RT3_NOINLINE void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                int32_t initialCapacity, int32_t growBy,
                                int32_t list1, int32_t list2, int32_t list3) {
    c->stride = stride;
    c->inlineRecords = inlineRecords;
    if (inlineRecords == 0) c->stride = 6; // {void* data, uint16 size}
    c->initialCapacity = initialCapacity;
    c->growBy = growBy;
    c->capacity = 0;
    c->count = 0;
    c->slots = nullptr;
    c->freeBits = nullptr;
    c->sequence = 0;
    for (int k = 0; k < 4; ++k) {
        c->nodes[k] = nullptr;
        c->head[k] = 0;
        c->tail[k] = 0;
        c->listEnabled[k] = 0;
    }
    c->listEnabled[1] = list1;
    c->listEnabled[2] = list2;
    c->listEnabled[3] = list3;
    c->listEnabled[0] = 1;
    InvalidateCaches(c);
    Collection_Grow(c, 0, initialCapacity);
}
RT3_REPLACE(0x518570, Collection_Init);

void __fastcall Collection_Clear(Collection* c) {
    while (c->count != 0) c->vtbl->release(c, Collection_IdAt(c, 0, 0, 0));
    FreeAndNull((void**)&c->slots);
    FreeAndNull((void**)&c->freeBits);
    for (int k = 0; k < 4; ++k) {
        FreeAndNull((void**)&c->nodes[k]);
        c->head[k] = 0;
        c->tail[k] = 0;
    }
    c->sequence = 0;
    c->count = 0;
    c->capacity = 0;
    InvalidateCaches(c);
}
RT3_REPLACE(0x518600, Collection_Clear);

RT3_NOINLINE int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize) {
    int32_t keepStride = c->stride;
    c->vtbl->clear(c); // QUIRK: keeps listEnabled, so Grow below sizes lists from the old flags
    uint32_t savedCap;
    int32_t total = StreamRead(s, &c->inlineRecords, 4, 0, 1);
    total += StreamRead(s, &c->stride, 4, 0, 1);
    total += StreamRead(s, &c->initialCapacity, 4, 0, 1);
    total += StreamRead(s, &c->growBy, 4, 0, 1);
    total += StreamRead(s, &savedCap, 4, 0, 1);
    total += StreamRead(s, &c->count, 4, 0, 1);
    if (recordSize != 0) c->stride = keepStride; // records are converted to the current size
    if (savedCap > c->capacity) Collection_Grow(c, 0, savedCap - c->capacity);
    total += StreamRead(s, &c->sequence, 4, 0, 1);
    for (int k = 0; k < 4; ++k) {
        total += StreamRead(s, &c->listEnabled[k], 4, 0, 1);
        total += StreamRead(s, &c->head[k], 4, 0, 1);
        total += StreamRead(s, &c->tail[k], 4, 0, 1);
        if (k > 0 && c->listEnabled[k]) total += StreamRead(s, c->nodes[k], c->capacity * 12, 0, 1);
    }
    total += StreamRead(s, c->freeBits, (c->capacity + 8) >> 3, 0, 1);
    for (int32_t i = 0; i < (int32_t)c->capacity; ++i) {
        uint8_t* slot = c->slots + c->stride * i;
        if (!Live(c, (uint32_t)i)) {
            memset(slot, 0, c->stride);
        } else if (recordSize != 0) {
            memset(slot, 0, c->stride);
            total += StreamRead(s, slot, recordSize, 0, 1);
        } else {
            total += StreamRead(s, slot, c->stride, 0, 1);
            if (!c->inlineRecords) {
                total += StreamRead(s, slot + 4, 2, 0, 1);
                uint16_t size = *(uint16_t*)(slot + 4);
                void* data = Malloc(size);
                *(void**)slot = data;
                total += StreamRead(s, data, size, 0, 1);
            }
        }
    }
    return total;
}
RT3_REPLACE(0x518680, Collection_Load);

RT3_NOINLINE uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size,
                                        uint32_t key2, uint32_t key3) {
    uint32_t id = 1; // lowest free slot
    bool found = false;
    if (1 < c->capacity) {
        do {
            if (BitFree(c, id)) { found = true; break; }
            ++id;
        } while (id < c->capacity);
    }
    if (!found) Collection_Grow(c, 0, c->growBy);

    if (c->inlineRecords == 0) {
        uint8_t* slot = c->slots + c->stride * id;
        void* p = Malloc(size);
        *(void**)slot = p;
        *(uint16_t*)(slot + 4) = (uint16_t)size;
        if (data) memcpy(p, data, size);
    } else if (data) {
        memcpy(c->slots + c->stride * id, data, c->stride);
    }
    for (int k = 1; k < 4; ++k) {
        if (!c->listEnabled[k]) continue;
        uint32_t key = k == 1 ? (uint32_t)(c->sequence++ + 0xb) * 1000 : k == 2 ? key2 : key3;
        Collection_Link(c, 0, id, k, key);
    }
    c->freeBits[id >> 3] &= ~(uint8_t)(1u << (id & 7));
    c->count++;
    InvalidateCaches(c);
    return id;
}
RT3_REPLACE(0x518900, Collection_Allocate);

RT3_NOINLINE void __fastcall Collection_Release(Collection* c, int, uint32_t id) {
    if (c->inlineRecords == 0) Free(Collection_Get(c, 0, id));
    for (int k = 1; k < 4; ++k)
        if (c->listEnabled[k]) Collection_Unlink(c, 0, id, k);
    c->freeBits[id >> 3] |= (uint8_t)(1u << (id & 7));
    c->count--;
    InvalidateCaches(c);
}
RT3_REPLACE(0x518a30, Collection_Release);

RT3_NOINLINE void __fastcall Collection_SetKey1(Collection* c, int, uint32_t id, uint32_t key) {
    Collection_Unlink(c, 0, id, 1);
    Collection_Link(c, 0, id, 1, key);
}
RT3_REPLACE(0x518ab0, Collection_SetKey1);

void __fastcall Collection_SwapOrder(Collection* c, int, uint32_t a, uint32_t b, int list) {
    if (list == 0) return;
    uint32_t posA = Collection_PositionOf(c, 0, a, list);
    uint32_t posB = Collection_PositionOf(c, 0, b, list);
    Collection_Unlink(c, 0, a, list);
    Collection_Unlink(c, 0, b, list);
    Collection_Link(c, 0, a, list, posB); // QUIRK: positions become the new keys
    Collection_Link(c, 0, b, list, posA);
}
RT3_REPLACE(0x518b20, Collection_SwapOrder);

RT3_NOINLINE Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                            int32_t initialCapacity, int32_t growBy,
                                            int32_t list1, int32_t list2, int32_t list3) {
    c->vtbl = kVtbl;
    Collection_Init(c, 0, inlineRecords, stride, initialCapacity, growBy, list1, list2, list3);
    return c;
}
RT3_REPLACE(0x518b90, Collection_Construct);

RT3_NOINLINE void __fastcall Collection_Destruct(Collection* c) {
    c->vtbl = kVtbl;
    Collection_Clear(c);
}
RT3_REPLACE(0x518bd0, Collection_Destruct);

// 0x518be0. Value in [lo, hi] from the simulation generator. Ranges wider than
// 15 bits combine two draws (first << 15 | second).
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi) {
    int32_t range = hi - lo;
    if (range > 0x7fff) {
        uint32_t first = (uint32_t)SimRand(0, 0x7fff);
        uint32_t second = (uint32_t)SimRand(0, 0x7fff);
        return (int32_t)((first << 15 | second) % (uint32_t)(range + 1)) + lo;
    }
    if (hi <= lo) return hi;
    uint32_t s = (uint32_t)hook::As<int32_t>(g_seed);
    s = s * 0x41c64e6d + (uint32_t)(int32_t)(int16_t)(s >> 16) + 0x3039;
    hook::As<int32_t>(g_seed) = (int32_t)s;
    return (int32_t)(s & 0x7fffffff) % (range + 1) + lo;
}
RT3_REPLACE(0x518be0, SimRand);

RT3_NOINLINE int32_t GetSeed() { return hook::As<int32_t>(g_seed); }
RT3_REPLACE(0x518c60, GetSeed);

RT3_NOINLINE void __fastcall SetSeed(int32_t seed) { hook::As<int32_t>(g_seed) = seed; }
RT3_REPLACE(0x518c70, SetSeed);

// Nested save/restore of the global seed.
void PushSeed() {
    if (g_seedDepth == 0) g_seedSaved = hook::As<int32_t>(g_seed);
    g_seedDepth++;
}
RT3_REPLACE(0x518c80, PushSeed);

void PopSeed() {
    g_seedDepth--;
    if (g_seedDepth == 0) {
        hook::As<int32_t>(g_seed) = g_seedSaved;
        g_seedSaved = -1;
    }
}
RT3_REPLACE(0x518ca0, PopSeed);

// 0x518cc0. Value in [lo, hi] derived from `seed` alone. Ranges wider than
// 15 bits combine two draws (seed+7 for the low half, seed for the high half).
RT3_NOINLINE int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi) {
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

// 0x518d70. Value in [lo, hi] from the global LCG.
RT3_NOINLINE int32_t __fastcall Rand(int32_t lo, int32_t hi) {
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

// 0x518de0. The game's text formatter (strings from RT3.lng use it):
//   %1..%8 (and %10.. two digits) insert argument N; %S inserts the next
//   argument in sequence; %% is a literal percent. Any other %x is copied as is.
// QUIRKS kept from the original:
//   - "%05" reads as argument 5 (05) but only advances 2 chars, so "5" is
//     then copied literally.
//   - Arguments 8 and above, and %0, all take the 8th argument.
//   - A trailing "%%" copies the terminator and keeps reading past it.
RT3_NOINLINE char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2,
                            const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8) {
    char* out = dst;
    const char* p = fmt;
    if (*p != '\0') {
        int seq = 1;
        do {
            if (*p == '%') {
                if (p[1] == '%') {
                    *out++ = '%';
                    p += 2;
                }
                if (*p != '%') goto literal;
                bool isS = ToUpper(p[1]) == 'S';
                if (!isS && (p[1] < '0' || p[1] > '9')) goto literal;
                int n;
                if (ToUpper(p[1]) == 'S') {
                    n = seq;
                } else {
                    n = p[1] - '0';
                    if (p[2] >= '0' && p[2] <= '9') n = n * 10 + p[2] - '0';
                }
                const char* arg;
                switch (n) {
                case 1: arg = a1; break;
                case 2: arg = a2; break;
                case 3: arg = a3; break;
                case 4: arg = a4; break;
                case 5: arg = a5; break;
                case 6: arg = a6; break;
                case 7: arg = a7; break;
                default: arg = a8; break;
                }
                if (arg) {
                    size_t len = strlen(arg);
                    memcpy(out, arg, len + 1);
                    out += len;
                }
                p += (n >= 10 && ToUpper(p[1]) != 'S') ? 3 : 2;
                seq++;
                continue;
            }
        literal:
            *out++ = *p++;
        } while (*p != '\0');
    }
    *out = '\0';
    return dst;
}
RT3_REPLACE(0x518de0, FormatText);

// 0x518f60
RT3_NOINLINE void __fastcall StringList_Free(StringList* l) {
    if (!l->items) return;
    for (int32_t i = 0; i < l->count; ++i)
        if (l->items[i]) Free(l->items[i]);
    FreeAndNull((void**)&l->items);
}
RT3_REPLACE(0x518f60, StringList_Free);

// 0x518fa0. Picks the game language (global string 0x6d3c90) from the Windows UI
// language (variants of Spanish, French, German, Chinese, Italian, Portuguese, and
// Japanese, Polish, Russian, Hungarian; else English), overridden by the first line
// of data2\overridelanguage (control characters cut), if its RT3.lng exists.
RT3_NOINLINE void Language_Select() {
    struct {
        char lang[0x198]; // the override is copied here unbounded (fits before buf)
        char buf[0x12d];
    } f;
    struct Entry { uint16_t id; uint32_t name; };
    static const Entry table[] = {
        { 0x404, 0x5d21cc }, { 0x804, 0x5d21cc }, { 0xc04, 0x5d21cc }, { 0x1004, 0x5d21cc }, { 0x1404, 0x5d21cc },
        { 0x407, 0x5d21d4 }, { 0x807, 0x5d21d4 }, { 0xc07, 0x5d21d4 }, { 0x1007, 0x5d21d4 }, { 0x1407, 0x5d21d4 },
        { 0x40c, 0x5d21ec }, { 0x80c, 0x5d21ec }, { 0xc0c, 0x5d21ec }, { 0x100c, 0x5d21ec }, { 0x140c, 0x5d21ec },
        { 0x180c, 0x5d21ec }, { 0x410, 0x5d21e4 }, { 0x810, 0x5d21e4 }, { 0x416, 0x5d21dc }, { 0x816, 0x5d21dc },
        { 0x411, 0x5d21fc }, { 0x415, 0x5d2204 }, { 0x419, 0x5d21f4 }, { 0x40e, 0x5d220c },
    };
    uint16_t id = ((uint16_t(__stdcall*)())g_5c8218)();
    const char* name = (const char*)0x5d21c4;
    if ((id & 0x3ff) == 0xa && id >= 0x40a && id <= 0x500a) name = (const char*)0x5d21bc; // all Spanish variants
    for (const Entry& e : table)
        if (e.id == id) name = (const char*)e.name;
    strcpy(f.lang, name);
    char path[0x17];
    memcpy(path, (const void*)0x5d21a4, sizeof path);
    if (FileExists(path)) {
        int fd = Open(path, 0x8000, 0);
        int n = Read(fd, f.buf, 0x12c);
        Close(fd);
        f.buf[n] = 0;
        for (int32_t i = 0; i < n; ++i)
            if (f.buf[i] < 0x1f) f.buf[i] = 0;
        if (strlen(f.buf) >= 2) strcpy(f.lang, f.buf);
    }
    char lng[0x190];
    FormatText(lng, (const char*)0x5d2188, f.lang, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (FileExists(lng)) String_Assign((char**)0x6d3c90, f.lang);
}
RT3_REPLACE(0x518fa0, Language_Select);

// 0x5193d0. Frees the strings loaded from the language file (0x6d3c94).
RT3_NOINLINE void FreeLanguageStrings() {
    StringList*& args = g_6d3c94;
    if (!args) return;
    StringList_Free(args);
    FreeAndNull((void**)&args);
}
RT3_REPLACE(0x5193d0, FreeLanguageStrings);

RT3_NOINLINE const char* __fastcall GetString(int32_t id) {
    if (id == 1) return (const char*)0x5c9ce8; // "\n"
    int32_t lang = g_language;
    const char* s;
    switch (lang) {
    case 6: case 11: case 12:
        if ((s = Loaded(id)) != nullptr) return s;
        if ((s = Builtin(0, id)) != nullptr) return s;
        break;
    case 0: case 1: case 2: case 3: case 4: case 8: case 10:
        if ((s = Builtin(lang, id)) != nullptr) return s;
        break;
    }
    if ((s = Loaded(id)) != nullptr) return s;
    return id == 7 ? g_6243c4 : g_6243c0;
}
RT3_REPLACE(0x5193f0, GetString);

}  // namespace rt3
