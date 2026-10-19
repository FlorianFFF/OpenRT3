// Small containers over a shared node pool (0x53d1d0..0x53ddf0): used by the
// route finder (heap = open list), tick queues, audio, command tables.
#pragma once
#include <stdint.h>

namespace rt3 {

struct SList {              // 12 bytes: singly linked free/item list
    void**  head;           // node[0] = next
    void**  tail;
    int32_t count;
};

struct NodePool {           // 28 bytes
    SList*   freeList;      // 0x00
    uint8_t* begin;         // 0x04 storage
    uint8_t* end;           // 0x08
    int32_t  capacity;      // 0x0c (0 = no storage: plain malloc/free)
    int32_t  elemSize;      // 0x10
    int32_t  flag;          // 0x14
    int32_t  ownsStorage;   // 0x18
};

struct HeapNode { HeapNode** slot; int32_t key; int32_t value; };

struct MaxHeap {            // 28 bytes; array is 1-based, array[0] -> sentinel
    void*      storage;     // 0x00
    NodePool*  pool;        // 0x04
    HeapNode** array;       // 0x08
    int32_t    count;       // 0x0c
    HeapNode   sentinel;    // 0x10 {slot, key 0x7fffffff, value 0}
};

struct ListNode { ListNode* next; int32_t key; int32_t value; };

struct MaxList {            // 12 bytes; unsorted, PopMax scans
    void*     storage;
    NodePool* pool;
    SList*    items;
};

struct HashNode { uint32_t key; int32_t value; HashNode* next; HashNode* prev; };

struct HashMap {            // 20 bytes
    HashNode** buckets;     // 0x00
    void*      storage;     // 0x04
    NodePool*  pool;        // 0x08
    int32_t    poolSize;    // 0x0c
    uint32_t   bucketCount; // 0x10
};

typedef int32_t (__fastcall* HashVisit)(HashNode* node, void* ctx);

void     __fastcall Heap_SiftUp(MaxHeap* h, int, int32_t i);                        // 0x53d1d0
void     __fastcall Heap_SiftDown(MaxHeap* h, int, int32_t i);                      // 0x53d240
int32_t  __fastcall Heap_TopKey(MaxHeap* h);                                        // 0x53d2e0
void     __fastcall Heap_DecrementKey(MaxHeap* h, int, HeapNode* n);                // 0x53d300
ListNode* __fastcall List_Push(MaxList* l, int, int32_t key, int32_t value);        // 0x53d320
int32_t  __fastcall List_PopMax(MaxList* l);                                        // 0x53d390
int32_t  __fastcall List_Contains(MaxList* l, int, int32_t value);                  // 0x53d440
void     __fastcall Heap_Destroy(MaxHeap* h);                                       // 0x53d470
HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value);        // 0x53d4c0
int32_t  __fastcall Heap_PopMax(MaxHeap* h);                                        // 0x53d540
void     __fastcall List_Destroy(MaxList* l);                                       // 0x53d5d0
void     __fastcall List_Init(MaxList* l, int, int32_t capacity);                   // 0x53d630
void     __fastcall Heap_Destroy2(MaxHeap* h);                                      // 0x53d6f0
void     __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);                   // 0x53d740
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);                 // 0x53d810
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);                       // 0x53d870
void     __fastcall Hash_Destroy(HashMap* m);                                       // 0x53d8a0
void     __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);      // 0x53d960
int32_t  __fastcall Hash_Remove(HashMap* m, int, uint32_t key);                     // 0x53d9e0
int32_t  __fastcall Hash_Contains(HashMap* m, int, uint32_t key);                   // 0x53daa0
int32_t  __fastcall Hash_Get(HashMap* m, int, uint32_t key);                        // 0x53dae0
int32_t  __fastcall Hash_ForEach(HashMap* m, int, uint32_t key, HashVisit fn, void* ctx); // 0x53db20
int32_t  __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize); // 0x53dbf0
void     __fastcall Pool_Free(NodePool* p);                                         // 0x53dcb0
void     __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag); // 0x53dcf0

} // namespace rt3
