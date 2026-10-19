#include "difftest.h"
#include "core/ptrlist.h"
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace rt3;

namespace {

struct O {
    static PtrList* Construct(PtrList* l, char p, int32_t b) { return ((PtrList*(__fastcall*)(PtrList*, int, char, int32_t))0x556d80)(l, 0, p, b); }
    static PtrListNode* Append(PtrList* l, void* v) { return ((PtrListNode*(__fastcall*)(PtrList*, int, void*))0x556e10)(l, 0, v); }
    static void Prepend(PtrList* l, void* v) { ((void(__fastcall*)(PtrList*, int, void*))0x557150)(l, 0, v); }
    static void* PopFront(PtrList* l) { return ((void*(__fastcall*)(PtrList*))0x556e50)(l); }
    static void RemoveNode(PtrList* l, PtrListNode* n) { ((void(__fastcall*)(PtrList*, int, PtrListNode*))0x556e80)(l, 0, n); }
    static void Rewind(PtrList* l) { ((void(__fastcall*)(PtrList*))0x556ef0)(l); }
    static void* Next(PtrList* l) { return ((void*(__fastcall*)(PtrList*))0x556f00)(l); }
    static void* RemoveCurrent(PtrList* l) { return ((void*(__fastcall*)(PtrList*))0x556f20)(l); }
    static void* Tail(PtrList* l) { return ((void*(__fastcall*)(PtrList*))0x556f90)(l); }
    static void* Current(PtrList* l) { return ((void*(__fastcall*)(PtrList*))0x556fa0)(l); }
    static void Clear(PtrList* l) { ((void(__fastcall*)(PtrList*))0x5570b0)(l); }
    static void Destroy(PtrList* l) { ((void(__fastcall*)(PtrList*))0x5571d0)(l); }
};

static std::vector<void*> Forward(const PtrList& l) {
    std::vector<void*> v;
    for (PtrListNode* n = l.head; n && v.size() < 100000; n = n->next) v.push_back(n->value);
    return v;
}
static std::vector<void*> Backward(const PtrList& l) {
    std::vector<void*> v;
    for (PtrListNode* n = l.tail; n && v.size() < 100000; n = n == l.head ? nullptr : n->prev) v.push_back(n->value);
    return v;
}
static int Index(const PtrList& l, PtrListNode* x) {
    int i = 0;
    for (PtrListNode* n = l.head; n; n = n->next, ++i) if (n == x) return i;
    return x ? -2 : -1;
}
static PtrListNode* At(const PtrList& l, int i) {
    PtrListNode* n = l.head;
    while (n && i--) n = n->next;
    return n;
}

static void Run(int seed, char pool, int block) {
    PtrList a, b;
    O::Construct(&a, pool, block);
    PtrList_Construct(&b, 0, pool, block);
    srand(seed);
    bool prevUnsafe = false; // after PopFront the head's prev dangles (QUIRK)
    for (int step = 0; step < 30000 && g_fail < 30; ++step) {
        int op = rand() % 100;
        void* v = (void*)(uintptr_t)(1 + rand() % 100000);
        if (op < 35) {
            EXPECT_EQ(Index(a, O::Append(&a, v)), Index(b, PtrList_Append(&b, 0, v)), "append");
        } else if (op < 45) {
            if (a.head) { O::Prepend(&a, v); PtrList_Prepend(&b, 0, v); }
        } else if (op < 50 && !pool) { // pooled: would free a block interior (QUIRK)
            EXPECT_EQ(O::PopFront(&a), PtrList_PopFront(&b), "pop");
            if (a.head) a.head->prev = nullptr; // QUIRK repair: PopFront leaves it dangling
            if (b.head) b.head->prev = nullptr;
            if (!a.head) a.tail = nullptr; // ...and the tail when it emptied the list
            if (!b.head) b.tail = nullptr;
            O::Rewind(&a); PtrList_Rewind(&b); // QUIRK: PopFront leaves the cursor dangling
        } else if (op < 60 && a.count > 0 && !prevUnsafe) {
            int i = rand() % a.count;
            O::RemoveNode(&a, At(a, i));
            PtrList_RemoveNode(&b, 0, At(b, i));
        } else if (op < 70) {
            O::Rewind(&a); PtrList_Rewind(&b);
        } else if (op < 85) {
            EXPECT_EQ(O::Next(&a), PtrList_Next(&b), "next");
        } else if (op < 92 && !prevUnsafe) {
            EXPECT_EQ(O::RemoveCurrent(&a), PtrList_RemoveCurrent(&b), "removeCurrent");
        } else if (op < 99) {
            EXPECT_EQ(O::Tail(&a), PtrList_TailValue(&b), "tail");
            EXPECT_EQ(O::Current(&a), PtrList_CurrentValue(&b), "current");
        } else {
            O::Clear(&a); PtrList_Clear(&b);
            prevUnsafe = false;
        }
        if (a.head && a.head->prev == nullptr) prevUnsafe = false;
        EXPECT_EQ(a.count, b.count, "step %d count", step);
        EXPECT_EQ(Forward(a) == Forward(b), true, "step %d forward", step);
        if (!prevUnsafe) EXPECT_EQ(Backward(a) == Backward(b), true, "step %d backward", step);
        EXPECT_EQ(Index(a, a.cursor), Index(b, b.cursor), "step %d cursor", step);
        EXPECT_EQ(Index(a, a.tail), Index(b, b.tail), "step %d tail", step);
        EXPECT_EQ(a.pool == nullptr, b.pool == nullptr, "pool");
        if (a.pool && b.pool) EXPECT_EQ(a.pool->count, b.pool->count, "step %d pool blocks", step);
    }
    O::Clear(&a); PtrList_Clear(&b);
    a.count = b.count = 0; // PopFront on pooled lists can unbalance it; Destroy would raise a fatal error
    O::Destroy(&a); PtrList_Destroy(&b);
}

} // namespace

DIFFTEST(ptrlist_plain)  { Run(21, 0, 0); }
DIFFTEST(ptrlist_pooled) { Run(22, 1, 7); }
DIFFTEST(ptrlist_pooled_big) { Run(23, 1, 100); }
