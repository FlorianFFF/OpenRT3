// IndexedCollection: the game's universal object table (vtable 0x5d1aa0).
//
// Trains, stations, companies, cargo types, track nodes... all live in one of
// these. Objects are addressed by a 1-based slot ID (slot 0 is never used); a
// bitmap marks free slots, and the lowest free slot is always reused first
// (so ID assignment depends on the exact order of creates/deletes: the "H"
// desync surface in full_reverse/README.md).
//
// Up to three ordered views can be kept in addition to slot order. Each is a
// doubly linked list over slot IDs, sorted ascending by a 32-bit key (ties go
// after existing equal keys). List 1 is keyed automatically by creation order,
// lists 2 and 3 by keys the caller passes to Allocate.
//
// The layout is shared with not-yet-rewritten code and must not change.
#pragma once
#include <stdint.h>
#include "core/stream.h"

namespace rt3 {


struct CollectionNode {     // 12 bytes, nodes[list][id]
    uint32_t key;
    uint32_t prev;
    uint32_t next;
};

struct CollectionVtbl {
    void     (__thiscall* clear)(struct Collection*);                                       // 0x518600
    uint32_t (__thiscall* allocate)(struct Collection*, const void*, uint32_t, uint32_t, uint32_t); // 0x518900
    void     (__thiscall* release)(struct Collection*, uint32_t);                            // 0x518a30
};

struct Collection {
    CollectionVtbl* vtbl;
    int32_t  inlineRecords;   // 0x04: 1 = records stored in the slot array; 0 = slot holds {ptr, u16 size}
    int32_t  stride;          // 0x08: slot size in bytes (6 when not inline)
    int32_t  initialCapacity; // 0x0c
    int32_t  growBy;          // 0x10
    uint32_t capacity;        // 0x14: number of slots, including unused slot 0
    int32_t  count;           // 0x18: live objects
    int32_t  sequence;        // 0x1c: next creation number, gives list-1 keys
    int32_t  listEnabled[4];  // 0x20: [0] (slot order) is always 1
    uint8_t* slots;           // 0x30
    uint8_t* freeBits;        // 0x34: bit set = slot free
    CollectionNode* nodes[4]; // 0x38: [0] unused
    uint32_t head[4];         // 0x48
    uint32_t tail[4];         // 0x58
    uint32_t cachePos[4];     // 0x68: IdAt() walk cache: last position...
    uint32_t cacheId[4];      // 0x78: ...and the ID found there
};
static_assert(sizeof(Collection) == 0x88, "Collection layout");

static const uint32_t kCacheInvalid = 0xfffffff0;

// Methods: `__fastcall(self, edx, ...)` = the original `__thiscall`.
Collection* __fastcall Collection_SetVtbl(Collection* c);                                          // 0x517ce0
int32_t  __fastcall Collection_Count(Collection* c);                                            // 0x517cf0
uint32_t __fastcall Collection_IsFree(Collection* c, int, uint32_t id);                         // 0x517d00
int32_t  __fastcall Collection_IsValid(Collection* c, int, uint32_t id);                        // 0x517d40
void     __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);         // 0x517d90
void     __fastcall Collection_Grow(Collection* c, int, uint32_t by);                           // 0x517f90
void*    __fastcall Collection_Get(Collection* c, int, uint32_t id);                            // 0x518140
uint32_t __fastcall Collection_GetKey1(Collection* c, int, uint32_t id);                        // 0x5181a0
void     __fastcall Collection_Unlink(Collection* c, int, uint32_t id, int list);               // 0x5181f0
void     __fastcall Collection_Link(Collection* c, int, uint32_t id, int list, uint32_t key);   // 0x518260
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);                // 0x518380
uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);           // 0x5184f0
void     __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                    int32_t initialCapacity, int32_t growBy,
                                    int32_t list1, int32_t list2, int32_t list3);              // 0x518570
void     __fastcall Collection_Clear(Collection* c);                                            // 0x518600
int32_t  __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);         // 0x518680
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size,
                                        uint32_t key2, uint32_t key3);                          // 0x518900
void     __fastcall Collection_Release(Collection* c, int, uint32_t id);                        // 0x518a30
void     __fastcall Collection_SetKey1(Collection* c, int, uint32_t id, uint32_t key);          // 0x518ab0
void     __fastcall Collection_SwapOrder(Collection* c, int, uint32_t a, uint32_t b, int list); // 0x518b20
Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride,
                                    int32_t initialCapacity, int32_t growBy,
                                    int32_t list1, int32_t list2, int32_t list3);              // 0x518b90
void     __fastcall Collection_Destruct(Collection* c);                                         // 0x518bd0

} // namespace rt3
