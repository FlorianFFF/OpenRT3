// Track block splitting / block component numbering / signal look-ahead / route station links in block 0x4a (batch 172). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
uint32_t __fastcall Obj_CenterRegionType(void* o);
uint8_t* __fastcall Route_Init(uint8_t* r);
int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v);
void __fastcall Nop494e80(void*);
int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id);
void __fastcall Node_Set212(void* n, int, int32_t v);
int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id);
int32_t __fastcall Node_GetLink(void* n, int, int32_t first);
float __fastcall Train_SumCarValues(uint8_t* t);
int32_t __fastcall Train_IsRunning(void*, int, uint32_t id);
int32_t __fastcall Track_Link2(uint8_t* t, int, int32_t second);
int32_t __fastcall Cargo_MatchesClass(void* c, int, int32_t id);
void __fastcall Site_RecordMin(uint8_t* o, int, int32_t i, int32_t list, float v);
int32_t __fastcall Obj_BuildingStateActive(void* o);
float __fastcall Obj_NodeCenterY(void* o);
float __fastcall Obj_NodeCenterX(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline Collection* Nodes() { return hook::Global<Collection*>(0x6cfca8); }
static inline uint8_t* Node(uint32_t id) { return (uint8_t*)Collection_Get(Nodes(), 0, id); }
static inline int32_t Ftol(float v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x4a4380. Walks block `id` from its start node (+4) towards its end node (+8) and
// splits it wherever the track changes kind (+0x44), owner (+0x22e) or region type:
// the old block ends before the change and a new block (owner set, start node
// there) takes the rest; nodes after a split are re-tagged (+0x212) with the new id.
void __fastcall Routes_SplitBlock(Collection* c, int, uint32_t id) {
    if (!Collection_IsValid(c, 0, id)) return;
    uint8_t* b = (uint8_t*)Collection_Get(c, 0, id);
    int32_t kind = b[0x1c], region = F<int32_t>(b, 0x18), owner = F<int32_t>(b, 0x1e);
    uint8_t* endP = Node(F<uint32_t>(b, 8));
    int32_t len = F<int32_t>(b, 0x10), endId = F<int32_t>(b, 8);
    int32_t start = F<int32_t>(b, 4);
    int32_t n = 0, total = 0, split = 0;
    uint8_t* p = Node(start);
    uint8_t* prev;
    if (Obj_Node206(p) && F<uint32_t>(Obj_Node206(p), 0x212) == id) {
        if (Obj_Node20a(p) && F<uint32_t>(Obj_Node20a(p), 0x212) == id) return;
        prev = (uint8_t*)Obj_Node20a(p);
    } else prev = (uint8_t*)Obj_Node206(p);
    if (prev == endP) return;
    do {
        if (total >= len || !p) break;
        bool same = !prev || ((p[0x44] != 0) == kind && F<uint16_t>(p, 0x22e) == owner && (int32_t)Obj_CenterRegionType(p) == region);
        if (!same) {
            F<int32_t>(b, 8) = F<int32_t>(prev, 0x202);
            F<int32_t>(b, 4) = start;
            F<int32_t>(b, 0x10) = n;
            F<int32_t>(b, 0x14) = n;
            uint8_t tmp[0x4c];
            Route_Init(tmp);
            F<int32_t>(c, 0x8c) = 1;
            uint32_t nid = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x4a44fc)(c, 0, tmp, 0, 0, 0);
            Route_SetOwner((uint8_t*)Collection_Get(c, 0, nid), 0, nid);
            Nop494e80(tmp);
            id = nid;
            b = (uint8_t*)Collection_Get(c, 0, nid);
            start = F<int32_t>(p, 0x202);
            Route_AddNode(b, 0, start);
            kind = b[0x1c];
            region = F<int32_t>(b, 0x18);
            owner = F<int32_t>(b, 0x1e);
            F<int32_t>(b, 4) = start;
            F<int32_t>(b, 8) = endId;
            n = 0;
            split = 1;
        }
        if (split) Node_Set212(p, 0, id);
        bool back = Obj_Node206(p) == prev;
        prev = p;
        p = (uint8_t*)(back ? Obj_Node20a(p) : Obj_Node206(p));
        ++total;
        ++n;
    } while (prev != endP);
    if (split) {
        F<int32_t>(b, 0x10) = n;
        F<int32_t>(b, 0x14) = n;
    }
}
RT3_REPLACE(0x4a4380, Routes_SplitBlock);

// Gives block g component `comp` and queues it, if it exists and is unnumbered.
static inline void Visit(Collection* c, uint32_t g, int32_t comp) {
    if (!Collection_IsValid(c, 0, g)) return;
    uint8_t* e = (uint8_t*)Collection_Get(c, 0, g);
    if (F<int32_t>(e, 0xc) != -1) return;
    F<int32_t>(e, 0xc) = comp;
    F<uint32_t*>(c, 0x94)[F<int32_t>(c, 0x98)] = g;
    ++F<int32_t>(c, 0x98);
}

// 0x4a5fc0. Numbers the connected components of the track blocks (+0xc) by flood
// fill through the nodes at both ends; empty blocks are handed to the manager at
// 0x6cfcb4, and nodes not yet in a block get one first.
void __fastcall Routes_NumberComponents(Collection* c) {
    F<int32_t>(c, 0x8c) = 0;
    if (!Collection_Count(c)) return;
    F<uint32_t*>(c, 0x94) = (uint32_t*)Malloc(Collection_Count(c) * 4 + 0x14);
    F<int32_t>(c, 0x98) = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xc) = -1;
    int32_t comp = 1;
    Visit(c, Collection_IdAt(c, 0, 0, 0), comp);
    for (;;) {
        int32_t cur = -1;
        if (F<int32_t>(c, 0x98)) cur = F<int32_t*>(c, 0x94)[--F<int32_t>(c, 0x98)];
        if (cur == -1) {
            bool found = false;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                if (F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xc) != -1) continue;
                ++comp;
                Visit(c, Collection_IdAt(c, 0, i, 0), comp);
                found = true;
                break;
            }
            if (found) continue;
            Free(F<uint32_t*>(c, 0x94));
            return;
        }
        uint8_t* b = (uint8_t*)Collection_Get(c, 0, cur);
        if (F<int32_t>(b, 0x10) <= 0) {
            void* mgr = hook::Global<void*>(0x6cfcb4);
            int32_t save = F<int32_t>(c, 0x8c);
            (*(void(__thiscall**)(void*, int32_t))(*(uint8_t**)mgr + 8))(mgr, cur);
            F<int32_t>(c, 0x8c) = save;
            continue;
        }
        for (int32_t j = 0; j < 2; ++j) {
            int32_t pid = j ? F<int32_t>(b, 8) : F<int32_t>(b, 4);
            if (pid == -1) continue;
            uint8_t* p = Node(pid);
            for (int32_t k = 0; k < 3; ++k) {
                int32_t nb = k == 0 ? F<int32_t>(p, 0x20e) : k == 1 ? F<int32_t>(p, 0x206) : F<int32_t>(p, 0x20a);
                if (nb == -1) continue;
                uint8_t* q = Node(nb);
                int32_t g = F<int32_t>(q, 0x212);
                if (g == cur) continue;
                if (Collection_IsValid(c, 0, g)) {
                    Visit(c, g, comp);
                    continue;
                }
                Routes_SplitAt(c, 0, nb);
                Visit(c, F<int32_t>(q, 0x212), comp);
                F<int32_t>(c, 0x8c) = 0;
            }
        }
    }
}
RT3_REPLACE(0x4a5fc0, Routes_NumberComponents);

// Whether signal s ahead blocks train t (none, its own, or one passed when the
// train may run through it counts as clear).
static inline int32_t Blocking(uint8_t* t, int32_t s) {
    if (s != -1 && s != F<int32_t>(t, 0x25) && !Train_IsRunning(t, 0, s)) s = -1;
    return s;
}

// 0x4a7e90. Signal look-ahead for train t (node +0x49, direction from its engine
// +0x35a): checks the signals at its node, the connected node (+0x253) both ways,
// then nodes ahead up to its stopping distance (car values) until +0x4d. A red
// signal for someone else stops it unless option +0x1b3 lets it through.
int32_t __fastcall Train_SignalsClear(uint8_t* t) {
    int32_t cur = F<int32_t>(t, 0x49);
    if (cur == -1) return 1;
    int32_t fwd = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62ba84), 0, F<uint32_t>(t, 0x41)), 0x35a) == 0;
    int32_t s = Node_GetLink(Node(F<uint32_t>(t, 0x49)), 0, !fwd);
    float lim = Train_SumCarValues(t);
    float dist = 0.0f;
    if (s != -1 && s != F<int32_t>(t, 0x25)) {
        if (!Train_IsRunning(t, 0, s)) s = -1;
        else goto check;
    }
    {
        uint8_t* p = Node(F<uint32_t>(t, 0x49));
        if (Collection_IsValid(Nodes(), 0, F<uint32_t>(p, 0x253))) {
            s = Node_GetLink(Node(F<uint32_t>(p, 0x253)), 0, 1);
            if (s != -1 && s != F<int32_t>(t, 0x25) && Train_IsRunning(t, 0, s) && s != F<int32_t>(t, 0x25)) goto check;
            s = Node_GetLink(Node(F<uint32_t>(p, 0x253)), 0, 0);
            if (s == -1) goto end;
            if (s != F<int32_t>(t, 0x25)) s = Blocking(t, s);
        }
    }
    for (;;) {
check:
        if (s != -1 && s != F<int32_t>(t, 0x25)) return F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1b3) != 0;
end:
        if (cur == F<int32_t>(t, 0x4d)) return 1;
        int32_t prev = cur;
        cur = Track_Link2(Node(cur), 0, fwd);
        if (cur == -1 || dist > lim) return 1;
        s = Node_GetLink(Node(cur), 0, !fwd);
        if (s != -1 && !Train_IsRunning(t, 0, s)) s = -1;
        uint8_t* q = Node(cur);
        dist = (float)(dist + D(0x5c97e0));
        fwd = F<int32_t>(q, 0x20a) != prev;
    }
}
RT3_REPLACE(0x4a7e90, Train_SignalsClear);

// 0x4a89a0. Records, for each pair of consecutive active stations on the route
// (+0 stops of 0x33 bytes, +4 count; stop +0x28 bit 7 skips, bit 6 = cargo list),
// the scaled distance from the previous station for each of the three special
// cargos it loads there; goes round the list twice.
void __fastcall Route_LinkStations(uint8_t* o) {
    int32_t loop = 1, wrapped = 0, i = 0, prev = -1;
    int32_t flags[3] = { 0, 0, 0 };
    int32_t px = 0, py = 0;
    do {
        int32_t next = i + 1;
        if (i >= F<int32_t>(o, 4)) {
            next = 0;
            if (wrapped) loop = 0;
            wrapped = 1;
        } else {
            uint8_t* e = F<uint8_t*>(o, 0) + i * 0x33;
            if ((int8_t)e[0x28] >= 0) {
                int32_t st = F<uint16_t>(e, 0x29);
                uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, st);
                if (Obj_BuildingStateActive(s) && st != prev) {
                    for (int32_t k = 0; k < 3; ++k) {
                        if (prev == -1 || !flags[k]) continue;
                        int32_t d = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(s)), Ftol(Obj_NodeCenterY(s)), px, py);
                        if (d < 1) d = 1;
                        float t1 = (float)((double)d / D(0x5caad8));
                        float f = (float)(((double)t1 + D(0x5c88e8)) * D(0x5d0060));
                        Site_RecordMin((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cec20), 0, prev), 0, st, k, f);
                    }
                    prev = st;
                    for (int32_t k = 0; k < 3; ++k) {
                        uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
                        void* ct = Collection_Get((Collection*)cargos, 0, F<uint32_t>(cargos, 0x8e + k * 4));
                        px = Ftol(Obj_NodeCenterX(s));
                        py = Ftol(Obj_NodeCenterY(s));
                        flags[k] = 0;
                        if (e[0x28] & 0x40) {
                            for (int32_t m = 0; m < e[0]; ++m)
                                if (Cargo_MatchesClass(ct, 0, F<int32_t>(e, 1 + m * 4))) flags[k] = 1;
                        } else flags[k] = Cargo_MatchesClass(ct, 0, F<int32_t>(e, 0x24));
                    }
                    if (wrapped) loop = 0;
                }
            }
        }
        i = next;
    } while (loop);
}
RT3_REPLACE(0x4a89a0, Route_LinkStations);

} // namespace rt3
