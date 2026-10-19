// Functions across blocks 0x44..0x49 (batch 46). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);

uint8_t* __fastcall Obj_Region(void* o);
uint8_t* __fastcall Obj_RegionAt(void* o);
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d);
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
void* __stdcall NewListHeader(int32_t);
void __fastcall Nop48a780(void*);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
uint32_t __fastcall Obj_CenterRegionType(void* o);

// 0x44c4b0. Recomputes flag 0x10 of every map cell: set only on the cells
// of regions without a type (+0x23e).
void __fastcall Map_MarkUntypedRegions(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            (F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7)[0xe6] &= 0xef;
    Collection* regions = hook::Global<Collection*>(0x62bae0);
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        Obj_Region(r)[0xe6] |= 0x10;
    }
}
RT3_REPLACE(0x44c4b0, Map_MarkUntypedRegions);

// 0x467df0. Unlinks a cargo load (ID +0, next +4) from its holder: a train
// (+8, list head +0x29) or a building (+0xa, head +0x272; for flag 2 of
// +0x12 the cargo +0x10 is also returned to the region stock, -0.2).
void __fastcall Load_Unlink(uint32_t* o) {
    uint32_t* head;
    if (uint16_t train = F<uint16_t>(o, 8)) {
        head = (uint32_t*)((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, train) + 0x29);
    } else if (uint16_t b = F<uint16_t>(o, 0xa)) {
        uint8_t* bld = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0, b);
        if (F<uint8_t>(o, 0x12) & 2) {
            uint8_t* ct = hook::Global<uint8_t*>(0x62ba8c);
            Obj_AddFloat103(Obj_RegionAt(bld), 0, F<int32_t>(ct, 0x8e + F<uint8_t>(o, 0x10) * 4), -0.2f);
        }
        head = (uint32_t*)(bld + 0x272);
    } else {
        return;
    }
    if (*head == o[0]) {
        *head = o[1];
    } else {
        uint32_t cur = *head;
        uint8_t* r;
        do {
            r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada90), 0, cur);
            cur = F<uint32_t>(r, 4);
        } while (cur != o[0]);
        F<uint32_t>(r, 4) = o[1];
    }
    F<uint16_t>(o, 0xa) = 0;
    F<uint16_t>(o, 8) = 0;
}
RT3_REPLACE(0x467df0, Load_Unlink);

// 0x469be0. Builds the message status packet {0xff, count, date minutes,
// count x value} from the message list +0x8f28 (each node's callback +4);
// stamps the nodes and +0x8f44 with the date.
uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize) {
    int32_t count = F<int32_t>(F<uint8_t*>(o, 0x8f28), 8);
    int32_t size = count * 4 + 6;
    *outSize = size;
    uint8_t* buf = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(size);
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    int32_t minutes = DateToMinutes((const GameDate*)(w + 0xd));
    buf[0] = 0xff;
    buf[1] = F<uint8_t>(F<uint8_t*>(o, 0x8f28), 8);
    F<int32_t>(buf, 2) = minutes;
    uint8_t* out = buf + 6;
    for (uint8_t* n = *F<uint8_t**>(o, 0x8f28); n; n = *(uint8_t**)n, out += 4) {
        F<int32_t>(n, 0xc) = 0;
        uint8_t* r = ((uint8_t*(__cdecl*)())F<void*>(n, 4))();
        if (F<int32_t>(r, 1)) hook::Global<uint32_t>(0x6cd908) |= 1u << (r[0] & 31);
        w = hook::Global<uint8_t*>(0x6cec78);
        F<uint32_t>(n, 0x9c) = F<uint32_t>(w, 0xd);
        F<uint32_t>(n, 0xa0) = F<uint32_t>(w, 0x11);
        F<uint32_t>(out, 0) = F<uint32_t>(r, 1);
    }
    F<int32_t>(o, 0x8f44) = minutes;
    return buf;
}
RT3_REPLACE(0x469be0, Obj_BuildStatusPacket);

// 0x46ab40. Queues message `msg` for `key` in ctx's map +0x2580 (new list
// headers via 0x46ab10); tracks the highest 'g' sequence in +0x2584.
// 1 if queued (or no world), 0 for 'n' messages and key 0.
int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx) {
    if (!hook::Global<void*>(0x6cec78)) return 1;
    if (F<uint8_t>((void*)msg[1], 9) == 0x6e) return 0;
    if (!key) return 0;
    void** h = ((void**(__thiscall*)(void*, int32_t))0x53dae0)(F<void*>(ctx, 0x2580), key);
    if (F<uint8_t>((void*)msg[1], 9) == 0x67) {
        uint32_t seq = F<uint32_t>((void*)msg[2], 4);
        if (seq > F<uint32_t>(ctx, 0x2584)) F<uint32_t>(ctx, 0x2584) = seq;
    }
    if (h) {
        SList_InsertAfter(h + 2, 0, (void**)h[3], (void**)msg);
        return 1;
    }
    uint32_t* l = (uint32_t*)NewListHeader(key);
    msg[0] = l[2];
    l[2] = (uint32_t)msg;
    l[4]++;
    if (!l[3]) l[3] = (uint32_t)msg;
    ((void(__thiscall*)(void*, int32_t, void*))0x53d960)(F<void*>(ctx, 0x2580), key, l);
    return 1;
}
RT3_REPLACE(0x46ab40, Msg_Queue);

// 0x47f010. Adds {key, value} to the 6-byte list +0x466 (count +0x462) unless the key is present.
void __fastcall Obj_AddKeyUnique(uint8_t* o, int, uint16_t key, int32_t value) {
    int32_t n = F<int32_t>(o, 0x462);
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i)
        if (F<uint16_t>(F<uint8_t*>(o, 0x466), i * 6) == key) return;
    uint8_t* p = (uint8_t*)Malloc((n + 1) * 6);
    n = F<int32_t>(o, 0x462);
    if (n > 0) {
        const uint8_t* old = F<uint8_t*>(o, 0x466);
        for (int32_t k = 0; k < n * 6; ++k) p[k] = old[k];
    }
    F<uint16_t>(p, n * 6) = key;
    F<int32_t>(p, n * 6 + 2) = value;
    if (F<void*>(o, 0x466)) Free(F<void*>(o, 0x466));
    F<uint8_t*>(o, 0x466) = p;
    F<int32_t>(o, 0x462) = n + 1;
}
RT3_REPLACE(0x47f010, Obj_AddKeyUnique);

// 0x4931e0. Debug validation (option +0x1cb): per-record hook and a lookup of
// every track piece referenced from the cell lists. No effects.
void __fastcall Track_Validate(Collection* t) {
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x1cb)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop48a780(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    for (int32_t x = 0; x < F<int32_t>(map, 0x2155) - 1; ++x) {
        for (int32_t y = 0; y < F<int32_t>(map, 0x2159) - 1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(t, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* s = F<uint32_t*>(e, 2); s; s = (uint32_t*)s[1]) Collection_IsValid(g_trackNodes, 0, s[0]);
            map = hook::Global<uint8_t*>(0x62c120);
        }
    }
}
RT3_REPLACE(0x4931e0, Track_Validate);

// 0x494f00. Extends a route with track node `id` if it matches the route's
// region type, owner and flag (the first node defines them). 1 if accepted.
int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id) {
    uint32_t type = Obj_CenterRegionType(Collection_Get(g_trackNodes, 0, id));
    int32_t owner = F<uint16_t>(Collection_Get(g_trackNodes, 0, id), 0x22e);
    uint8_t flag = F<uint8_t>(Collection_Get(g_trackNodes, 0, id), 0x44) ? 1 : 0;
    if (F<int32_t>(r, 4) == -1 && F<int32_t>(r, 8) == -1) {
        F<uint32_t>(r, 4) = id;
        F<uint32_t>(r, 8) = id;
        F<uint32_t>(r, 0x18) = type;
        F<int32_t>(r, 0x1e) = owner;
        r[0x1c] = flag;
        return 1;
    }
    if ((flag != 0) != (r[0x1c] != 0)) return 0;
    if (type != F<uint32_t>(r, 0x18) || owner != F<int32_t>(r, 0x1e)) return 0;
    if (F<int32_t>(r, 4) == -1) F<uint32_t>(r, 4) = id;
    else F<uint32_t>(r, 8) = id;
    return 1;
}
RT3_REPLACE(0x494f00, Route_AddNode);

} // namespace rt3
