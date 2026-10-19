// IndexedCollection (vtable 0x5d1aa0), methods 0x517ce0..0x518bd0.
// Original quirks are kept on purpose (marked QUIRK): later code relies on the
// exact cache state, and saves must stay byte-compatible.
#include "collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "hook/hook.h"
#include <string.h>

namespace rt3 {

extern int g_traceMaskPointer; // core/stream.cpp (save tracing)

static CollectionVtbl* const kVtbl = reinterpret_cast<CollectionVtbl*>(0x5d1aa0);

static inline bool BitFree(const Collection* c, uint32_t id) {
    return (c->freeBits[id >> 3] & (1u << (id & 7))) != 0;
}
static inline bool Live(const Collection* c, uint32_t id) {
    return id != 0 && id < c->capacity && !BitFree(c, id);
}
static inline void InvalidateCaches(Collection* c) {
    for (int k = 0; k < 4; ++k) c->cachePos[k] = kCacheInvalid;
}

Collection* __fastcall Collection_SetVtbl(Collection* c) { c->vtbl = kVtbl; return c; } // returns this
RT3_REPLACE(0x517ce0, Collection_SetVtbl);

int32_t __fastcall Collection_Count(Collection* c) { return c->count; }
RT3_REPLACE(0x517cf0, Collection_Count);

uint32_t __fastcall Collection_IsFree(Collection* c, int, uint32_t id) {
    if (id != 0 && id < c->capacity) return c->freeBits[id >> 3] & (1u << (id & 7));
    return 1;
}
RT3_REPLACE(0x517d00, Collection_IsFree);

int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id) { return Live(c, id) ? 1 : 0; } // full 0/1 in EAX
RT3_REPLACE(0x517d40, Collection_IsValid);

void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize) {
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

void* __fastcall Collection_Get(Collection* c, int, uint32_t id) {
    if (!Live(c, id)) return nullptr;
    uint8_t* slot = c->slots + c->stride * id;
    return c->inlineRecords ? (void*)slot : *(void**)slot;
}
RT3_REPLACE(0x518140, Collection_Get);

uint32_t __fastcall Collection_GetKey1(Collection* c, int, uint32_t id) {
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

uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list) {
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

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list) {
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

void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride,
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

int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize) {
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

uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size,
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

void __fastcall Collection_Release(Collection* c, int, uint32_t id) {
    if (c->inlineRecords == 0) Free(Collection_Get(c, 0, id));
    for (int k = 1; k < 4; ++k)
        if (c->listEnabled[k]) Collection_Unlink(c, 0, id, k);
    c->freeBits[id >> 3] |= (uint8_t)(1u << (id & 7));
    c->count--;
    InvalidateCaches(c);
}
RT3_REPLACE(0x518a30, Collection_Release);

void __fastcall Collection_SetKey1(Collection* c, int, uint32_t id, uint32_t key) {
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

Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                            int32_t initialCapacity, int32_t growBy,
                                            int32_t list1, int32_t list2, int32_t list3) {
    c->vtbl = kVtbl;
    Collection_Init(c, 0, inlineRecords, stride, initialCapacity, growBy, list1, list2, list3);
    return c;
}
RT3_REPLACE(0x518b90, Collection_Construct);

void __fastcall Collection_Destruct(Collection* c) {
    c->vtbl = kVtbl;
    Collection_Clear(c);
}
RT3_REPLACE(0x518bd0, Collection_Destruct);

} // namespace rt3
