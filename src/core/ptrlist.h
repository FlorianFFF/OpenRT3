// PtrList (0x556d80..0x5571d0): doubly linked list of pointers with a cursor.
// Nodes come either from malloc (pool == NULL) or from a pool of node blocks.
// The pool is itself a PtrList whose values are PtrListBlock*.
#pragma once
#include <stdint.h>

namespace rt3 {

struct PtrListNode {   // 12 bytes
    void*        value;
    PtrListNode* prev;
    PtrListNode* next;
};

struct PtrListBlock {  // 8 bytes; nodes are handed out from the end
    PtrListNode* nodes;
    int32_t      remaining;
};

struct PtrList {       // 24 bytes
    PtrListNode* head;
    PtrListNode* cursor;
    PtrListNode* tail;
    int32_t      count;
    int32_t      blockSize;
    PtrList*     pool;
};
static_assert(sizeof(PtrList) == 24, "PtrList layout");

PtrList*     __fastcall PtrList_Construct(PtrList* l, int, char usePool, int32_t blockSize); // 0x556d80
PtrList*     __fastcall PtrList_Init(PtrList* l);                                            // 0x556db0
void         __fastcall PtrList_CreatePool(PtrList* l);                                      // 0x556dd0
PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value);                        // 0x556e10
void*        __fastcall PtrList_PopFront(PtrList* l);                                        // 0x556e50
void         __fastcall PtrList_RemoveNode(PtrList* l, int, PtrListNode* n);                 // 0x556e80
void         __fastcall PtrList_Rewind(PtrList* l);                                          // 0x556ef0
void*        __fastcall PtrList_Next(PtrList* l);                                            // 0x556f00
void*        __fastcall PtrList_RemoveCurrent(PtrList* l);                                   // 0x556f20
void*        __fastcall PtrList_TailValue(PtrList* l);                                       // 0x556f90
void*        __fastcall PtrList_CurrentValue(PtrList* l);                                    // 0x556fa0
PtrListBlock* __fastcall PtrList_AddBlock(PtrList* l, int, int32_t nodes);                   // 0x556fb0
void         __fastcall PtrList_FreePool(PtrList* l);                                        // 0x557010
void         __fastcall PtrList_Clear(PtrList* l);                                           // 0x5570b0
PtrListNode* __fastcall PtrList_AllocNode(PtrList* l);                                       // 0x557100
void         __fastcall PtrList_Prepend(PtrList* l, int, void* value);                       // 0x557150
void         __fastcall PtrList_Destroy(PtrList* l);                                         // 0x5571d0

} // namespace rt3
