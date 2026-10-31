// PtrList (0x556d80..0x5571d0). QUIRKs of the original are kept.
#include "ptrlist.h"
#include "crt.h"
#include "hook/hook.h"
#include <string.h>

namespace rt3 {

const char* __fastcall GetString(int32_t id);
static inline void FatalError(const char* msg) { ((void(__fastcall*)(const char*))0x51e980)(msg); }

PtrList* __fastcall PtrList_Construct(PtrList* l, int, char usePool, int32_t blockSize) {
    l->blockSize = blockSize;
    l->cursor = nullptr;
    l->head = nullptr;
    l->tail = nullptr;
    l->count = 0;
    l->pool = nullptr;
    if (usePool) PtrList_CreatePool(l);
    return l;
}
RT3_REPLACE(0x556d80, PtrList_Construct);

PtrList* __fastcall PtrList_Init(PtrList* l) { memset(l, 0, sizeof *l); return l; } // returns this (callers use it)
RT3_REPLACE(0x556db0, PtrList_Init);

void __fastcall PtrList_CreatePool(PtrList* l) {
    PtrList* p = (PtrList*)Malloc(sizeof(PtrList));
    if (!p) {
        l->pool = nullptr;
        return;
    }
    memset(p, 0, sizeof *p);
    p->blockSize = 100;
    l->pool = p;
}
RT3_REPLACE(0x556dd0, PtrList_CreatePool);

PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value) {
    PtrListNode* n = PtrList_AllocNode(l);
    n->value = value;
    n->prev = l->tail;
    n->next = nullptr;
    if (l->tail) l->tail->next = n;
    l->tail = n;
    if (!l->head) l->head = n;
    l->count++;
    return n;
}
RT3_REPLACE(0x556e10, PtrList_Append);

// QUIRK: frees the node even when it came from a pool, and leaves the new
// head's prev pointing at the freed node.
void* __fastcall PtrList_PopFront(PtrList* l) {
    PtrListNode* n = l->head;
    if (!n) return nullptr;
    l->count--;
    void* v = n->value;
    PtrListNode* next = n->next;
    Free(n);
    l->head = next;
    return v;
}
RT3_REPLACE(0x556e50, PtrList_PopFront);

void __fastcall PtrList_RemoveNode(PtrList* l, int, PtrListNode* n) {
    if (l->cursor == n) l->cursor = n->next;
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n == l->tail) l->tail = l->tail->prev;
    if (n == l->head) {
        l->head = n->next;
        if (l->head) l->head->prev = nullptr;
    }
    if (!l->pool) Free(n);
    l->count--;
}
RT3_REPLACE(0x556e80, PtrList_RemoveNode);

void __fastcall PtrList_Rewind(PtrList* l) { l->cursor = l->head; }
RT3_REPLACE(0x556ef0, PtrList_Rewind);

void* __fastcall PtrList_Next(PtrList* l) {
    PtrListNode* n = l->cursor;
    if (!n) return nullptr;
    l->cursor = n->next;
    return n->value;
}
RT3_REPLACE(0x556f00, PtrList_Next);

void* __fastcall PtrList_RemoveCurrent(PtrList* l) {
    PtrListNode* n = l->cursor;
    if (!n) return nullptr;
    void* v = n->value;
    l->cursor = n->next;
    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n == l->tail) l->tail = l->tail->prev;
    if (n == l->head) {
        l->head = n->next;
        if (l->head) l->head->prev = nullptr;
    }
    if (!l->pool) Free(n);
    l->count--;
    return v;
}
RT3_REPLACE(0x556f20, PtrList_RemoveCurrent);

void* __fastcall PtrList_TailValue(PtrList* l) { return l->tail ? l->tail->value : nullptr; }
RT3_REPLACE(0x556f90, PtrList_TailValue);

void* __fastcall PtrList_CurrentValue(PtrList* l) { return l->cursor ? l->cursor->value : nullptr; }
RT3_REPLACE(0x556fa0, PtrList_CurrentValue);

PtrListBlock* __fastcall PtrList_AddBlock(PtrList* l, int, int32_t nodes) {
    PtrListBlock* b = (PtrListBlock*)Malloc(sizeof(PtrListBlock));
    b->nodes = (PtrListNode*)Malloc(nodes * sizeof(PtrListNode));
    b->remaining = nodes;
    PtrList_Append(l->pool, 0, b);
    return b;
}
RT3_REPLACE(0x556fb0, PtrList_AddBlock);

void __fastcall PtrList_FreePool(PtrList* l) {
    PtrList_Rewind(l->pool);
    while (PtrListBlock* b = (PtrListBlock*)PtrList_RemoveCurrent(l->pool)) {
        Free(b->nodes);
        b->nodes = nullptr;
        Free(b);
    }
    l->count = 0;
}
RT3_REPLACE(0x557010, PtrList_FreePool);

void __fastcall PtrList_Clear(PtrList* l) {
    if (!l->pool) {
        for (PtrListNode* n = l->head; n;) {
            PtrListNode* next = n->next;
            Free(n);
            l->count--;
            n = next;
        }
    } else {
        PtrList_FreePool(l); // pooled nodes go with their blocks
    }
    l->tail = nullptr;
    l->cursor = nullptr;
    l->head = nullptr;
}
RT3_REPLACE(0x5570b0, PtrList_Clear);

PtrListNode* __fastcall PtrList_AllocNode(PtrList* l) {
    if (!l->pool) return (PtrListNode*)Malloc(sizeof(PtrListNode));
    PtrListBlock* b = l->pool->tail ? (PtrListBlock*)l->pool->tail->value : nullptr;
    if (!b || b->remaining == 0) b = PtrList_AddBlock(l, 0, l->blockSize);
    b->remaining--;
    return b->nodes + b->remaining;
}
RT3_REPLACE(0x557100, PtrList_AllocNode);

void __fastcall PtrList_Prepend(PtrList* l, int, void* value) {
    PtrListNode* n = PtrList_AllocNode(l);
    n->value = value;
    n->prev = nullptr;
    n->next = l->head;
    if (l->head) l->head->prev = n;
    l->head = n;
    l->count++; // QUIRK: tail is not set when the list was empty
}
RT3_REPLACE(0x557150, PtrList_Prepend);

RT3_NOINLINE void __fastcall PtrList_Destroy(PtrList* l) {
    PtrList_Clear(l);
    if (PtrList* p = l->pool) {
        PtrList_Destroy(p);
        Free(p);
        l->pool = nullptr;
    }
    if (l->count > 0) FatalError(GetString(0x41));      // list destroyed while not empty
    else if (l->count < 0) FatalError(GetString(0x42)); // count went negative
}
RT3_REPLACE(0x5571d0, PtrList_Destroy);

} // namespace rt3
