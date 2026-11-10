// Thin game-side wrappers around the GameSpy peer library (0x58d000..), the free
// list used for its packets, and a few small engine list helpers. The library
// stays original code; these call it with its register conventions (ECX object,
// EDX first argument, the rest on the stack, callee pops).
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include "core/containers.h"
#include "game/collection.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void* Peer() { return hook::Global<void*>(0x6cd970); }

// 0x4691d0 body. The first of the 0x80 player slots (0x11c bytes, from base) that is
// in use (+0x104) with id key (+0x10c), or null.
static uint8_t* __cdecl Slots_FindImpl(uint8_t* base, int32_t key) {
    for (int32_t i = 0; i < 0x80; ++i, base += 0x11c)
        if (F<int32_t>(base, 0x104) && key == F<int32_t>(base, 0x10c)) return base;
    return nullptr;
}
// 0x4691d0. Takes the slot array in EAX and the key in EDX (kept).
__declspec(naked) void Slots_Find() {
    __asm {
        push edx
        push edx
        push eax
        call Slots_FindImpl
        add esp, 8
        pop edx
        ret
    }
}
RT3_REPLACE(0x4691d0, Slots_Find);

// 0x4696d0. Takes a block from the free list (+0 list, +0xc enabled) or, when the
// list is disabled, allocates one of size +0x10. Null when the list is empty.
RT3_NOINLINE void* __fastcall FreeList_Take(uint8_t* q) {
    if (!F<int32_t>(q, 0xc)) return ExeNew(F<uint32_t>(q, 0x10));
    uint8_t* list = F<uint8_t*>(q, 0);
    uint8_t* node = F<uint8_t*>(list, 0);
    if (!node) return nullptr;
    uint8_t* next = F<uint8_t*>(node, 0);
    F<uint8_t*>(list, 0) = next;
    if (node == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = next;
    --F<int32_t>(list, 8);
    return node;
}
RT3_REPLACE(0x4696d0, FreeList_Take);

// 0x469680. Polls the peer object (the argument is unused).
int32_t __stdcall Peer_Think(int32_t) { return ((int32_t(__fastcall*)(void*))0x58db00)(Peer()); }
RT3_REPLACE(0x469680, Peer_Think);

// 0x469660. Sends a message to the peer's room (no-op for null).
RT3_NOINLINE int32_t __stdcall Peer_SendRoom(const char* msg) {
    if (!msg) return 0;
    return ((int32_t(__fastcall*)(void*, int32_t, const char*, int32_t))0x58d9e0)(Peer(), 0, msg, 0);
}
RT3_REPLACE(0x469660, Peer_SendRoom);

// 0x469620. Builds the room key from name and asks the peer to join it.
int32_t __stdcall Peer_JoinNamed(const char* name) {
    char key[0x100];
    ((void(__fastcall*)(char*, const char*))0x58e650)(key, name);
    return ((int32_t(__fastcall*)(void*, const char*, uint32_t, int32_t, int32_t))0x58df20)(Peer(), key, 0x4691a0, 0, 1);
}
RT3_REPLACE(0x469620, Peer_JoinNamed);

// 0x4694c0. Leaves the staging room when connected.
void Peer_LeaveStaging() {
    if (!Peer() || !hook::Global<int32_t>(0x6cd978) || !hook::Global<int32_t>(0x6cd920)) return;
    ((void(__fastcall*)(void*, int32_t))0x58d960)(Peer(), 1);
    ((void(__fastcall*)(void*, const char*, int32_t))0x58e100)(Peer(), (const char*)0x5c87a8, 1);
}
RT3_REPLACE(0x4694c0, Peer_LeaveStaging);

// 0x469580. Starts listing the game servers: clears the 32000-dword list at 0x6ae4d8
// and its count, then asks the peer (callback 0x4693b0). Returns the peer's result,
// or the busy flag 0x6cd920 when already busy.
RT3_NOINLINE int32_t __fastcall Lobby_ListGames(uint8_t* lobby) {
    if (hook::Global<int32_t>(0x6cd920)) return hook::Global<int32_t>(0x6cd920);
    F<int32_t>(lobby, 0x9874) = 1;
    memset((void*)0x6ae4d8, 0, 0x7d00 * 4);
    hook::Global<int32_t>(0x6cd97c) = 0;
    return ((int32_t(__fastcall*)(void*, int32_t, uint32_t, int32_t))0x58da60)(Peer(), 0, 0x4693b0, 0);
}
RT3_REPLACE(0x469580, Lobby_ListGames);

// 0x469520. Starts the server list update with the 8 standard keys (callback 0x469200).
RT3_NOINLINE int32_t __fastcall Lobby_UpdateList(uint8_t* lobby) {
    if (hook::Global<int32_t>(0x6cd920)) return hook::Global<int32_t>(0x6cd920);
    F<int32_t>(lobby, 0x8f18) = 1;
    uint8_t keys[8] = { 3, 1, 4, 8, 10, 11, 0x13, 5 };
    return ((int32_t(__fastcall*)(void*, uint8_t*, int32_t, int32_t, uint32_t, uint8_t*))0x58d970)(Peer(), keys, 8, 0, 0x469200, lobby + 0x10);
}
RT3_REPLACE(0x469520, Lobby_UpdateList);

// 0x469450. Shuts the peer down: leaves the room and stops listing when connected,
// then disconnects and frees it.
void Peer_Shutdown() {
    if (!Peer()) return;
    if (hook::Global<int32_t>(0x6cd978)) {
        if (hook::Global<int32_t>(0x6cd920)) ((void(__fastcall*)(void*, int32_t))0x58d960)(Peer(), 0);
        if (hook::Global<int32_t>(0x6cd978) && hook::Global<int32_t>(0x6cd920)) ((void(__fastcall*)(void*))0x58db20)(Peer());
    }
    ((void(__fastcall*)(void*))0x58d830)(Peer());
    ((void(__fastcall*)(void*))0x58de90)(Peer());
    hook::Global<void*>(0x6cd970) = nullptr;
}
RT3_REPLACE(0x469450, Peer_Shutdown);

// 0x521d40. The first entry of the list at 0x6d408c, or null.
RT3_NOINLINE void* Engine_FirstListEntry() {
    void** l = hook::Global<void**>(0x6d408c);
    if (!l || !*l) return nullptr;
    return *l;
}
RT3_REPLACE(0x521d40, Engine_FirstListEntry);

// 0x521b40. The node of the list at 0x6d40b0 whose byte +0x1c is tag, or null.
uint8_t* __fastcall Engine_FindTagged(char tag) {
    for (uint8_t* n = *hook::Global<uint8_t**>(0x6d40b0); n; n = F<uint8_t*>(n, 0))
        if ((char)n[0x1c] == tag) return n;
    return nullptr;
}
RT3_REPLACE(0x521b40, Engine_FindTagged);

// 0x521d10. When 0x6d40dc is set, calls method 0x68 of the device at +0x18e -> +8
// with (v, 0, 0, 0).
int32_t __fastcall Engine_DeviceCall68(uint8_t* e, int, int32_t v) {
    if (!hook::Global<int32_t>(0x6d40dc)) return 0;
    void* dev = F<void*>(F<uint8_t*>(e, 0x18e), 8);
    return ((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t))(*(void***)dev)[0x68 / 4])(dev, v, 0, 0, 0);
}
RT3_REPLACE(0x521d10, Engine_DeviceCall68);

// 0x455a40. Tail call to virtual method +0x44 (any arguments).
__declspec(naked) void Obj_VCall44() {
    __asm {
        mov eax, [ecx]
        jmp dword ptr [eax + 0x44]
    }
}
RT3_REPLACE(0x455a40, Obj_VCall44);

void __fastcall Net_ResetOutgoing();
const char* __fastcall Obj_NextBufferedLine(void* o);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void* __fastcall NetPacket_Alloc(uint8_t* s, int, int32_t size);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);

// 0x46a1a0. Starts a fresh server list (cleared 0x2380 dwords at +0x10) unless one
// is in progress, updating it from the peer when connected (+0x985c) and otherwise
// resetting the outgoing traffic; returns the list.
RT3_NOINLINE uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby) {
    if (!F<int32_t>(lobby, 0x8f18)) {
        F<int32_t>(lobby, 0x8f10) = 0;
        F<int32_t>(lobby, 0x8f18) = 1;
        memset(lobby + 0x10, 0, 0x2380 * 4);
        if (!F<int32_t>(lobby, 0x985c)) {
            Net_ResetOutgoing();
            return lobby + 0x10;
        }
        Lobby_UpdateList(lobby);
    }
    return lobby + 0x10;
}
RT3_REPLACE(0x46a1a0, Lobby_FreshList);

// 0x4ed670. Refreshes the game list window w: requests the list and posts each
// buffered line as event 0x88 / 0xe57; the number posted (0 while busy).
RT3_NOINLINE int32_t __fastcall GameList_Refresh(uint8_t* w) {
    int32_t n = 0;
    if (hook::Global<int32_t>(0x6cd920)) return 0;
    Lobby_ListGames(hook::Global<uint8_t*>(0x6cd8d8));
    for (const char* line; (line = Obj_NextBufferedLine(hook::Global<void*>(0x6cd8d8))) != nullptr;) {
        ++n;
        Window_SendEvent5(w, 0, 0x88, 0xe57, (uint32_t)(uintptr_t)line, 0, 0);
    }
    return n;
}
RT3_REPLACE(0x4ed670, GameList_Refresh);

// 0x46a590. Frees every packet queued on the session (+0x9878 queue).
void __fastcall NetQueue_Drain(uint8_t* s) {
    for (;;) {
        uint8_t* q = F<uint8_t*>(s, 0x9878);
        if (!q) return;
        uint8_t* n = F<uint8_t*>(q, 0);
        if (!n) return;
        F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(q, 8);
        NetPacket_Free(s, 0, n);
    }
}
RT3_REPLACE(0x46a590, NetQueue_Drain);

// 0x46a5f0. Queues a copy of the 16-byte message m with tag (front of +0x9878);
// messages with id 999999 are dropped.
void __fastcall NetQueue_Push(uint8_t* s, int, const uint32_t* m, int32_t tag) {
    uint8_t* q = F<uint8_t*>(s, 0x9878);
    if (!q || m[0] == 0xf423f) return;
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, 0x18);
    F<int32_t>(p, 0x14) = tag;
    memcpy(p + 4, m, 16);
    q = F<uint8_t*>(s, 0x9878);
    F<uint8_t*>(p, 0) = F<uint8_t*>(q, 0);
    F<uint8_t*>(q, 0) = p;
    ++F<int32_t>(q, 8);
    if (!F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = p;
}
RT3_REPLACE(0x46a5f0, NetQueue_Push);

struct GameDate;
int32_t __fastcall DateToMinutes(const GameDate* d);
void __stdcall Net_UnlockQueueArg(int32_t);
int32_t Global6d40e4();
uint8_t* __stdcall Textures_Find(int32_t key);
struct HashMap;
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t ClockMs();

// 0x46a3d0. Whether the session's current message (stamped at the date +0x9048) is
// complete: true unless the frame's entry (0x6d40d0 list) has it with fewer parts
// received (+0x10) than expected (+0). Releases the entry's lock afterwards.
int32_t __fastcall Net_MessageComplete(uint8_t* s) {
    int32_t key = DateToMinutes((const GameDate*)(s + 0x9048));
    int32_t ok = 1;
    uint8_t* e = Textures_Find(Global6d40e4());
    if (e) {
        uint8_t* h = (uint8_t*)(uintptr_t)Hash_Get(F<HashMap*>(e, 0x2580), 0, (uint32_t)key);
        if (h) ok = F<int32_t>(h, 0x10) >= F<int32_t>(h, 0);
        Net_UnlockQueueArg(Global6d40e4());
    }
    return ok;
}
RT3_REPLACE(0x46a3d0, Net_MessageComplete);

// 0x46a310. A packet copy of message m (0x28-byte header, its 12-byte address block
// at +0x28 and the payload after its first word at +0x34, linked from +8 when
// present; size +0xc less 4), unlinked.
RT3_NOINLINE uint8_t* __fastcall NetPacket_Copy(uint8_t* s, int, const uint8_t* m) {
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, F<int32_t>((void*)m, 0xc) + 0x30);
    memcpy(p, m, 0x28);
    F<int32_t>(p, 0) = 0;
    F<int32_t>(p, 0x14) = 0;
    F<int32_t>(p, 0x1c) = 0;
    memcpy(p + 0x28, F<void*>((void*)m, 4), 12);
    F<uint8_t*>(p, 4) = p + 0x28;
    int32_t n = F<int32_t>((void*)m, 0xc);
    uint8_t* payload = nullptr;
    if (n > 4) {
        payload = p + 0x34;
        memcpy(payload, F<uint8_t*>((void*)m, 8) + 4, (size_t)(n - 4));
    }
    F<uint8_t*>(p, 8) = payload;
    F<int32_t>(p, 0xc) = F<int32_t>(p, 0xc) - 4;
    return p;
}
RT3_REPLACE(0x46a310, NetPacket_Copy);

// 0x469a50. Queues a text message (fields a..d, time stamp, text) at the end of
// the session's chat queue (+0x8f2c).
uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d) {
    uint8_t* p = (uint8_t*)NetPacket_Alloc(s, 0, (int32_t)strlen(text) + 0x21);
    F<int32_t>(p, 0x14) = a;
    F<int32_t>(p, 4) = c;
    F<int32_t>(p, 0x18) = d;
    F<int32_t>(p, 0x10) = b;
    F<int32_t>(p, 0x1c) = 0;
    F<int32_t>(p, 8) = ClockMs();
    strcpy((char*)p + 0x20, text);
    uint8_t* q = F<uint8_t*>(s, 0x8f2c);
    uint8_t* tail = F<uint8_t*>(q, 4);
    if (tail) {
        F<uint8_t*>(p, 0) = F<uint8_t*>(tail, 0);
        F<uint8_t*>(tail, 0) = p;
    } else {
        F<uint8_t*>(p, 0) = F<uint8_t*>(q, 0);
        F<uint8_t*>(q, 0) = p;
    }
    ++F<int32_t>(q, 8);
    if (tail || !F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = p;
    return p;
}
RT3_REPLACE(0x469a50, Net_QueueText);

void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
void* __stdcall NewListHeader(int32_t);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x46b6d0. The session's message date (+0x9048); with count set it also counts
// one more part for that date in the current frame entry's hash (creating its list
// header), under the entry's lock. Null without a running world.
RT3_NOINLINE uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count) {
    uint8_t* w = hook::Global<uint8_t*>(0x6cec78);
    if (!w || !F<int32_t>(w, 0x19)) return nullptr;
    if (count) {
        int32_t key = DateToMinutes((const GameDate*)(s + 0x9048));
        uint8_t* e = Textures_Find(Global6d40e4());
        if (e) {
            int32_t* h = (int32_t*)(uintptr_t)Hash_Get(F<HashMap*>(e, 0x2580), 0, (uint32_t)key);
            if (!h) {
                h = (int32_t*)NewListHeader(key);
                Hash_Insert(F<HashMap*>(e, 0x2580), 0, (uint32_t)key, (int32_t)(uintptr_t)h);
            }
            ++*h;
            Net_UnlockQueueArg(Global6d40e4());
        }
    }
    return s + 0x9048;
}
RT3_REPLACE(0x46b6d0, Net_CountMessagePart);

// 0x46b910. Drops entry e's stored message lists older than the given minute
// stamp (the current date when 0): for each key the visitor 0x468ac0 selects
// (into 0x6cd968) the list is unhooked and its packets freed.
void __fastcall Net_DropOldMessages(uint8_t* s, int, uint8_t* e, int32_t minutes) {
    typedef int32_t(__fastcall * ForEachFn)(HashMap*, int, uint32_t, void*, void*);
    int32_t stamp = 0;
    if (!minutes && hook::Global<uint8_t*>(0x6cec78)) stamp = DateToMinutes((const GameDate*)(hook::Global<uint8_t*>(0x6cec78) + 0xd));
    ForEachFn forEach = (ForEachFn)0x53db20;
    while (forEach(F<HashMap*>(e, 0x2580), 0, 0, (void*)0x468ac0, &stamp)) {
        uint8_t* n = (uint8_t*)(uintptr_t)Hash_Remove(F<HashMap*>(e, 0x2580), 0, hook::Global<uint32_t>(0x6cd968));
        if (!n) continue;
        for (;;) {
            uint8_t* p = F<uint8_t*>(n, 8);
            if (!p) break;
            F<uint8_t*>(n, 8) = F<uint8_t*>(p, 0);
            if (p == F<uint8_t*>(n, 0xc)) F<uint8_t*>(n, 0xc) = F<uint8_t*>(p, 0);
            --F<int32_t>(n, 0x10);
            NetPacket_Free(s, 0, p);
        }
        ExeDelete(n);
    }
}
RT3_REPLACE(0x46b910, Net_DropOldMessages);

void __stdcall Net_PurgeSentMessages(int32_t);
int32_t Net_LockList2();
void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name);
void Net_UnlockList2();
void __stdcall Textures_RefreshQueued(int32_t);

// 0x46af70. The lobby's periodic work at time now (ms): expires the 1 s hold
// +0x9054, refreshes the queued textures every 5 s (in the lobby or when +0x8f30),
// and every second either advertises the hosted game (host, +0x8e10) or, while a
// server list is being shown (+0x8f18), copies the list entries seen in the last
// 10 s (0x11c bytes each, up to 0x80) into it and purges old messages.
void __fastcall Lobby_Tick(uint8_t* l, int, uint32_t now) {
    if (F<uint32_t>(l, 0x9054) && now - F<uint32_t>(l, 0x9054) > 1000) F<uint32_t>(l, 0x9054) = 0;
    if (!hook::Global<void*>(0x6cec78) || F<int32_t>(l, 0x8f30) == 1) {
        if (now - F<uint32_t>(l, 0x8f14) > 5000) {
            F<uint32_t>(l, 0x8f14) = now;
            Textures_RefreshQueued((int32_t)now);
        }
    }
    void* w = hook::Global<void*>(0x6cec78);
    if (hook::Global<int32_t>(0x6d40dc) == 1) {
        if (!w && now - F<uint32_t>(l, 0x8f10) > 1000) {
            F<uint32_t>(l, 0x8f10) = now;
            Net_BroadcastGame(hook::Global<uint8_t*>(0x6d40d0), 0, (const char*)l + 0x8e10);
        }
        return;
    }
    if (F<int32_t>(l, 0x8f18) != 1 || w || F<int32_t>(l, 0x985c)) return;
    if (now - F<uint32_t>(l, 0x8f10) <= 1000) return;
    F<uint32_t>(l, 0x8f10) = now;
    memset(l + 0x10, 0, 0x2380 * 4);
    uint8_t** lst = (uint8_t**)(uintptr_t)Net_LockList2();
    if (lst) {
        uint8_t* dst = l + 0x10;
        int32_t k = 0;
        for (uint8_t* n = *lst; n && k < 0x80; n = F<uint8_t*>(n, 0), ++k, dst += 0x11c) {
            if (now - F<uint32_t>(n, 0x118) < 10000) memcpy(dst, n, 0x47 * 4);
            else F<int32_t>(n, 0x118) = -1;
        }
    }
    Net_UnlockList2();
    Net_PurgeSentMessages(0);
}
RT3_REPLACE(0x46af70, Lobby_Tick);

void CloseFile6ce994();
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Nop5217b0(void*);
int32_t Net_LockQueue();
void Net_UnlockQueue();
int32_t __fastcall Net_ShutdownShared(uint8_t* net);
void __fastcall Music_Shutdown(uint8_t* m);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Music_Close(uint8_t* m);

// 0x46bc40. Closes the network session s (when open, +0): drops its pack object
// (+0x9884), stops hosting (+0x8f18), cleans up the first player's messages,
// shuts the peer, the shared state and the music stream down, returns every
// queued packet (+0x8f2c) to the pool and frees the session's tables, pools and
// buffers, then the global stream 0x6cd8e0.
RT3_NOINLINE void __fastcall Session_Close(uint8_t* s) {
    hook::Global<int32_t>(0x6cd91c) = 0;
    if (!F<int32_t>(s, 0)) return;
    CloseFile6ce994();
    if (void* o = F<void*>(s, 0x9884)) {
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, o);
        F<void*>(s, 0x9884) = nullptr;
    }
    if (F<int32_t>(s, 0x8f18) == 1) Nop5217b0(hook::Global<void*>(0x6d40d0));
    if (hook::Global<void*>(0x6d40d0) && Net_LockQueue()) {
        if (Engine_FirstListEntry()) Net_DropOldMessages(s, 0, (uint8_t*)Engine_FirstListEntry(), 1);
        Net_UnlockQueue();
    }
    Peer_Shutdown();
    Net_ShutdownShared(hook::Global<uint8_t*>(0x6d40d0));
    Music_Shutdown(F<uint8_t*>(s, 0));
    if (uint8_t* q = F<uint8_t*>(s, 0x8f2c)) {
        for (;;) {
            uint8_t* n = F<uint8_t*>(q, 0);
            if (!n) break;
            F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
            if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
            --F<int32_t>(q, 8);
            NetPacket_Free(s, 0, n);
            q = F<uint8_t*>(s, 0x8f2c);
        }
    }
    if (void* p = F<void*>(s, 0x9878)) {
        ExeDelete(p);
        F<void*>(s, 0x9878) = nullptr;
    }
    NodePool* pool = F<NodePool*>(s, 0x8f20);
    F<int32_t>(s, 0x9054) = 0;
    F<int32_t>(s, 0) = 0;
    F<int32_t>(s, 0xc) = 0;
    F<int32_t>(s, 0x8f18) = 0;
    F<int32_t>(s, 0x8f30) = 0;
    if (pool) {
        Pool_Free(pool);
        ExeDelete(pool);
        F<void*>(s, 0x8f20) = nullptr;
    }
    if (void* p = F<void*>(s, 0x8f24)) {
        ((void(__cdecl*)(void*))0x5a1145)(p);
        F<void*>(s, 0x8f24) = nullptr;
    }
    if (HashMap* m = F<HashMap*>(s, 0x9050)) Hash_ForEach(m, 0, 0, (HashVisit)0x468ae0, nullptr);
    if (HashMap* m = F<HashMap*>(s, 0x9050)) {
        Hash_DestroyThunk(m);
        ExeDelete(m);
        F<void*>(s, 0x9050) = nullptr;
    }
    if (NodePool* p = F<NodePool*>(s, 0x8f3c)) {
        Pool_Free(p);
        ExeDelete(p);
        F<void*>(s, 0x8f3c) = nullptr;
    }
    static const uint32_t bufs[] = { 0x8f40, 0x8f28, 0x8f2c };
    for (uint32_t off : bufs)
        if (void* p = F<void*>(s, off)) {
            ExeDelete(p);
            F<void*>(s, off) = nullptr;
        }
    if (HashMap* m = F<HashMap*>(s, 4)) {
        Hash_DestroyThunk(m);
        ExeDelete(m);
        F<void*>(s, 4) = nullptr;
    }
    if (uint8_t* m = hook::Global<uint8_t*>(0x6cd8e0)) {
        Music_Close(m);
        ExeDelete(m);
        hook::Global<uint8_t*>(0x6cd8e0) = nullptr;
    }
}
RT3_REPLACE(0x46bc40, Session_Close);

void Net_DeleteLocks();
void __fastcall Music_Destroy(uint8_t* m);

// 0x46c230. Closes session s, then frees the shared network object 0x6d40d0 and
// the music player 0xd973b4.
RT3_NOINLINE void __fastcall Session_Shutdown(uint8_t* s) {
    Session_Close(s);
    if (uint8_t* n = hook::Global<uint8_t*>(0x6d40d0)) {
        Net_DeleteLocks();
        ExeDelete(n);
        hook::Global<uint8_t*>(0x6d40d0) = nullptr;
    }
    if (uint8_t* m = hook::Global<uint8_t*>(0xd973b4)) {
        Music_Destroy(m);
        ExeDelete(m);
        hook::Global<uint8_t*>(0xd973b4) = nullptr;
    }
}
RT3_REPLACE(0x46c230, Session_Shutdown);

uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i);
int32_t __fastcall Str_IsBlank(const char* s);

// 0x4ee6a0. Refills window w's game list (control 0xe56 on page 0xe13, else
// 0xe22) from a fresh server list, up to the first empty slot, keeping the
// selection on the game of the same name. Skipped while connected (0x6cd920),
// while it runs (0x6d1290) or when the status text (0xe47) isn't blank.
void __fastcall GameList_Fill(uint8_t* w) {
    if (hook::Global<int32_t>(0x6cd920) || hook::Global<char>(0x6d1290)) return;
    hook::Global<char>(0x6d1290) = 1;
    uint32_t list = F<int32_t>(w, 0x7c) == 0xe13 ? 0xe56 : 0xe22;
    char name[0x100];
    memset(name, 0, sizeof name);
    int32_t sel = -1;
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6) > -1) {
        int32_t i = F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6);
        strcpy(name, (const char*)(uintptr_t)TreeView_NodeValue(Widget_FindChild(w, 0, list), 0, i));
    }
    int32_t count = 0;
    Window_SendEvent5(w, 0, 0x8b, list, 0, 0, 0);
    uint8_t* status = Widget_FindChild(w, 0, 0xe47);
    uint8_t* games = Lobby_FreshList(hook::Global<uint8_t*>(0x6cd8d8));
    if (Str_IsBlank(F<const char*>(status, 0xba))) {
        uint8_t* g = games + 4;
        for (int32_t k = 0; k < 0x80; ++k, g += 0x11c) {
            if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)g, name)) sel = count;
            if (!F<int32_t>(g, 0x100)) break;
            Window_SendEvent5(w, 0, 0x88, list, (uint32_t)(uintptr_t)g, 0, 0);
            ++count;
            if (sel != -1) {
                Window_SendEvent5(w, 0, 0x66, list, (uint32_t)sel, 0, 0);
                sel = -1;
            }
        }
    }
    hook::Global<char>(0x6d1290) = 0;
}
RT3_REPLACE(0x4ee6a0, GameList_Fill);

// 0x46a900. Logs lobby onto GameSpy: creates the peer (0x6cd970) with the lobby
// callbacks, sets the RT3 title (rooms pinged: title + group, cross-pinged:
// staging), connects as nick (or "RT3Player<n>", n from the shared LCG 0x624324)
// and joins the title room, then asks for the game list. When already connected
// (0x6cd920) it only marks the connect done (0x6cd974). 1 on success.
RT3_NOINLINE int32_t __fastcall Lobby_Connect(uint8_t* lobby, int, const char* nick) {
    uint32_t cb[0x1c];
    memset(cb, 0, sizeof cb);
    hook::Global<int32_t>(0x6cd978) = 0;
    F<int32_t>(lobby, 0x9874) = 1;
    cb[0] = 0x468de0;
    cb[1] = 0x468e00;
    cb[9] = 0x468ec0;
    cb[10] = 0x468f50;
    cb[13] = 0x469010;
    cb[16] = 0x469030;
    cb[17] = 0x469060;
    cb[20] = 0x46a6c0;
    cb[21] = 0x469070;
    cb[22] = 0x4690d0;
    cb[23] = 0x4690f0;
    cb[24] = 0x469160;
    cb[25] = 0x469180;
    cb[27] = 0;
    hook::Global<int32_t>(0x6cd974) = 0;
    void* peer = ((void*(__fastcall*)(uint32_t*))0x58dc50)(cb);
    hook::Global<void*>(0x6cd970) = peer;
    if (!peer) return 0;
    int32_t cross[3] = { 0, 0, 1 }, ping[3] = { 1, 1, 0 };
    const char* title = hook::Global<const char*>(0x5f2b44);
    const char* secret = hook::Global<const char*>(0x5f2b48);
    int32_t ok = ((int32_t(__fastcall*)(void*, const char*, const char*, const char*, const char*, int32_t, int32_t, int32_t, int32_t*, int32_t*))0x58dce0)(
        peer, title, secret, title, secret, 0, 0x1e, 0, ping, cross);
    if (hook::Global<int32_t>(0x6cd920)) {
        hook::Global<int32_t>(0x6cd974) = 1;
        return 1;
    }
    if (!ok) return 0;
    char name[0x100];
    if (nick && *nick) strcpy(name, nick);
    else {
        uint32_t r = hook::Global<uint32_t>(0x624324) * 0x19660d + 0x3c6ef35f;
        hook::Global<uint32_t>(0x624324) = r;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, (const char*)0x5ce1e4, (int32_t)(r & 0x7fff) % 1000);
    }
    char key[0x100];
    ((void(__fastcall*)(char*, const char*))0x58e650)(key, name);
    ((void(__fastcall*)(void*, const char*, int32_t, uint32_t, uint32_t, int32_t, int32_t))0x58e200)(hook::Global<void*>(0x6cd970), key, 0, 0x46a830,
                                                                                                    0x469190, 0, 1);
    if (!hook::Global<int32_t>(0x6cd974)) return 0;
    hook::Global<int32_t>(0x6cd978) = 0;
    ((void(__fastcall*)(void*, int32_t, uint32_t, int32_t, int32_t))0x58dfb0)(hook::Global<void*>(0x6cd970), 0, 0x469410, 0, 1);
    if (!hook::Global<int32_t>(0x6cd978)) return 0;
    Lobby_UpdateList(lobby);
    return 1;
}
RT3_REPLACE(0x46a900, Lobby_Connect);

int32_t __fastcall Net_Host(uint8_t* o, int, const char* game, const char* name, const char* password, int32_t ms, int32_t maxPlayers);
int32_t __fastcall Net_Connect(uint8_t* o, int, const char* host, const char* name, const char* password, int32_t ms);

// 0x46a030. Hosts game as name (password optional: 0x6cd928 says whether one is
// set) unless a connection is up (0x6d40dc): a running server-list request is
// stopped first; the names are kept at +0x8e10 / +0x8f48. The GameSpy peer then
// leaves its room and stops listing.
void __fastcall Lobby_HostGame(uint8_t* lobby, int, const char* game, const char* name, const char* password, int32_t ms) {
    if (hook::Global<int32_t>(0x6d40dc)) return;
    if (F<int32_t>(lobby, 0x8f18) == 1) {
        if (!F<int32_t>(lobby, 0x985c)) Nop5217b0(hook::Global<void*>(0x6d40d0));
        F<int32_t>(lobby, 0x8f18) = 0;
    }
    F<int32_t>(lobby, 0x8f10) = 0;
    strcpy((char*)lobby + 0x8e10, game);
    strcpy((char*)lobby + 0x8f48, name);
    const char* pw = nullptr;
    if (!password) hook::Global<int32_t>(0x6cd928) = 0;
    else {
        hook::Global<int32_t>(0x6cd928) = *password != 0;
        pw = *password ? password : nullptr;
    }
    Net_Host(hook::Global<uint8_t*>(0x6d40d0), 0, game, name, pw, ms, 0);
    if (void* peer = Peer()) {
        ((void(__fastcall*)(void*))0x58d9d0)(peer);
        ((void(__fastcall*)(void*))0x58e1f0)(Peer());
    }
}
RT3_REPLACE(0x46a030, Lobby_HostGame);

// 0x46a110. Joins the game at address host as name (password optional): the
// game's title (from the server list, matched on its address +0x80) goes to
// +0x8e10; +0xc becomes 1 while connecting, 2 when the connect fails at once.
void __fastcall Lobby_JoinGame(uint8_t* lobby, int, const char* host, const char* name, const char* password) {
    uint8_t* g = lobby + 0x14;
    for (int32_t k = 0; k < 0x80; ++k, g += 0x11c)
        if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)g + 0x80, host)) strcpy((char*)lobby + 0x8e10, (const char*)g);
    F<int32_t>(lobby, 0xc) = 1;
    const char* pw = password && *password ? password : nullptr;
    if (Net_Connect(hook::Global<uint8_t*>(0x6d40d0), 0, host, name, pw, 0) < 0) F<int32_t>(lobby, 0xc) = 2;
}
RT3_REPLACE(0x46a110, Lobby_JoinGame);

int32_t __fastcall Music_Start(uint8_t* m);
uint8_t* __fastcall Net_Construct(uint8_t* o);
int32_t __fastcall Net_InitShared(uint8_t* net);
float __fastcall Game_VersionNumber();
void __stdcall SetGlobal6d4108(int32_t v);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void MpSession_Reset();
void RegisterSyncChecks();

static uint32_t* NewZeroed(size_t n, size_t zero) {
    uint32_t* p = (uint32_t*)ExeNew(n);
    if (p) memset(p, 0, zero);
    return p;
}

// 0x46be80. Opens network session s (closing any open one): resets the session
// globals, creates the music stream 0xd973b4 (kept at +0) and the shared network
// object 0x6d40d0 (game GUID 0x6257e8 from 0x5f2b28), stores the game version as
// 0x6d4108, and allocates the session's tables, pools and packet buffers. With
// gamespy == 1 it also logs onto the GameSpy lobby as nick (result at +0x985c).
// 1 when open, 0 when a step fails.
int32_t __fastcall Session_Open(uint8_t* s, int, int32_t gamespy, const char* nick) {
    Session_Close(s);
    if (F<int32_t>(s, 0)) return 0;
    hook::Global<int32_t>(0x6ae4cc) = 0;
    hook::Global<int32_t>(0x6ae4d0) = 0;
    hook::Global<int32_t>(0x6ae4d4) = 0;
    hook::Global<int32_t>(0x6cd96c) = 0;
    hook::Global<int32_t>(0x6d10a0) = 0;
    hook::Global<int32_t>(0x622090) = -1;
    hook::Global<int32_t>(0x622094) = -1;
    static const uint32_t zeros[] = { 0x6cd92c, 0x6cd928, 0x6cd924, 0x6cd91c, 0x6cd8e4, 0x6cd8e8 };
    for (uint32_t a : zeros) hook::Global<int32_t>(a) = 0;
    hook::Global<int32_t>(0x5f2b38) = 0x3c;
    static const uint32_t zeros2[] = { 0x6cd8f4, 0x6cd8f8, 0x6cd8fc, 0x6cd908, 0x6cd90c, 0x6cd918, 0x6cd8ec, 0x6cd8f0, 0x6cd900, 0x5f2b3c, 0x6cd904 };
    for (uint32_t a : zeros2) hook::Global<int32_t>(a) = 0;
    hook::Global<int32_t>(0x5f2b40) = 1;
    F<int32_t>(s, 0x988c) = 0;
    F<int32_t>(s, 0x9890) = 0;
    uint32_t* m = hook::Global<uint32_t*>(0xd973b4);
    if (!m) {
        m = (uint32_t*)ExeNew(0x18);
        if (m) {
            m[0] = 0;
            m[1] = 1;
            m[2] = m[3] = m[4] = m[5] = 0;
        }
        hook::Global<uint32_t*>(0xd973b4) = m;
    }
    F<uint32_t*>(s, 0) = m;
    for (int k = 0; k < 4; ++k) ((uint32_t*)0x6257e8)[k] = ((const uint32_t*)0x5f2b28)[k];
    if (Music_Start(F<uint8_t*>(s, 0)) != 1) return 0;
    uint8_t* net = hook::Global<uint8_t*>(0x6d40d0);
    if (!net) {
        net = (uint8_t*)ExeNew(0x25a);
        net = net ? Net_Construct(net) : nullptr;
        hook::Global<uint8_t*>(0x6d40d0) = net;
    }
    if (Net_InitShared(net) != 1) return 0;
    float ver = Game_VersionNumber();
    SetGlobal6d4108(*(int32_t*)&ver);
    uint32_t* h = NewZeroed(0x14, 0x14);
    F<uint32_t*>(s, 4) = h;
    Hash_Init((HashMap*)h, 0, 0x65, 0x100);
    memset(s + 0x10, 0, 0x2380 * 4);
    F<uint32_t*>(s, 0x8f2c) = NewZeroed(0xc, 0xc);
    F<void*>(s, 0x8f24) = ((void*(__cdecl*)(size_t))0x5a125d)(0x2900);
    uint32_t* pool = NewZeroed(0x1c, 0x1c);
    F<uint32_t*>(s, 0x8f20) = pool;
    Pool_Init((NodePool*)pool, 0, F<void*>(s, 0x8f24), 0xa4, 0x40, 0);
    F<uint32_t*>(s, 0x8f28) = NewZeroed(0xc, 0xc);
    F<void*>(s, 0x8f40) = ExeNew(0x48000);
    pool = NewZeroed(0x1c, 0x1c);
    F<uint32_t*>(s, 0x8f3c) = pool;
    Pool_Init((NodePool*)pool, 0, F<void*>(s, 0x8f40), 0x300, 0x180, 0);
    h = NewZeroed(0x14, 0x14);
    F<uint32_t*>(s, 0x9050) = h;
    Hash_Init((HashMap*)h, 0, 0x11, 0x64);
    memset(s + 0x8f48, 0, 0x100);
    for (int k = 0; k < 4; ++k) F<int32_t>(s, 0x9864 + k * 4) = -1;
    F<int32_t>(s, 0x9860) = -1;
    F<int32_t>(s, 0x8f34) = 0;
    F<int32_t>(s, 0x8f14) = 0;
    F<int32_t>(s, 0x8f30) = 0;
    F<int32_t>(s, 0x8f44) = 0;
    F<int32_t>(s, 0x9054) = 0;
    memset(s + 0x905c, 0, 0x800);
    F<uint32_t*>(s, 0x9878) = NewZeroed(0xc, 0xc);
    F<uint8_t*>(s, 0x9884) = Pack_AcquireFont(hook::Global<uint8_t*>(0x6d4020), 0, 1);
    MpSession_Reset();
    RegisterSyncChecks();
    if (gamespy != 1) return 1;
    F<int32_t>(s, 0x985c) = Lobby_Connect(s, 0, nick);
    return F<int32_t>(s, 0x985c) ? 1 : 0;
}
RT3_REPLACE(0x46be80, Session_Open);

int32_t Global6d4100();
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();
int32_t __fastcall Net_SendTo(uint8_t* net, int, const uint8_t* data, uint16_t size, uint8_t type, uint8_t channel, const char* to,
                              uint32_t flags, uint32_t timeout, uint32_t serial, int32_t);

// Net_SendTo with every argument as the full dword the original pushes.
typedef int32_t(__fastcall* SendToFn)(void*, int, const void*, uint32_t, uint32_t, uint32_t, const char*, uint32_t, uint32_t,
                                      int32_t, int32_t);

// 0x469d30. Sends message type (size bytes at data) to player to (null: everyone)
// under the send lock 0x6cd960. Chat (type 1) goes to the GameSpy room instead
// while connected there (+0x985c) and 0x6d4100 is clear. Flags: 0x10 prefixes the
// session id (+0x8f38, send flag 0x40000000); 4 also queues a copy (0x28-byte
// header) on the first engine list's queue and sends an empty type 0xe instead,
// reliable unless 0x6d40dc is set; 1 is reliable (8, no timeout instead of 2000);
// 2 adds 0x20; 0x20 adds 0x20000000; 8, and anything of 0x30 bytes or more, is
// LZSS-packed when that succeeds (0x80000000). A serial also stamps the time.
// Totals of raw / sent bytes go to 0x6cd930 / 0x6cd934.
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags,
                         int32_t serial) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    uint32_t sendFlags = 0, idFlag = 0, timeout = 2000;
    if ((uint16_t)type == 1) {
        if (!data || !size) return;
        if (F<int32_t>(s, 0x985c) == 1 && !Global6d4100()) {
            Peer_SendRoom((const char*)data);
            return;
        }
    }
    while (*(volatile LONG*)0x6cd960) {
    }
    hook::Global<LockFn>(0x5c8070)((LONG volatile*)0x6cd960);
    uint8_t local[0x1000];
    if (flags) {
        if (flags & 0x10) {
            *(uint32_t*)local = F<uint32_t>(s, 0x8f38);
            idFlag = 0x40000000;
            if (size > 0) memcpy(local + 4, data, (uint32_t)size);
            data = local;
            size += 4;
        }
        if (flags & 4) {
            flags = (flags & ~4u) | 1;
            if (hook::Global<int32_t>(0x6d40dc)) flags &= ~2u;
            else flags |= 2;
            uint8_t* pkt = (uint8_t*)NetPacket_Alloc(s, 0, size + 0x28);
            memset(pkt, 0, (uint32_t)(size + 0x28));
            F<uint32_t>(pkt, 0x10) = flags;
            if (flags & 8) flags &= ~8u;
            F<uint32_t>(pkt, 0x14) = (uint16_t)type;
            F<int32_t>(pkt, 0xc) = size;
            if (size > 0) {
                F<uint8_t*>(pkt, 8) = pkt + 0x28;
                memcpy(pkt + 0x28, data, (uint32_t)size);
            }
            void** q = F<void**>(Engine_FirstListEntry(), 0x64);
            SList_InsertAfter(q, 0, (void**)q[1], (void**)pkt);
            data = nullptr;
            size = 0;
            type = 0xe;
        }
        if (flags & 1) {
            timeout = 0;
            sendFlags = 8;
        }
        if (flags & 2) sendFlags |= 0x20;
        if (flags & 0x20) sendFlags |= 0x20000000;
    }
    int32_t now = 0;
    if (serial) now = ClockMs();
    SendToFn send = (SendToFn)&Net_SendTo;
    uint8_t* net = hook::Global<uint8_t*>(0x6d40d0);
    hook::Global<int32_t>(0x6cd930) += size;
    if (!(flags & 8) && size < 0x30) {
        hook::Global<int32_t>(0x6cd934) += size;
        send(net, 0, data, (uint32_t)size, type, 0xff, to, sendFlags | idFlag, timeout, serial, now);
        hook::Global<LockFn>(0x5c8074)((LONG volatile*)0x6cd960);
        return;
    }
    uint8_t* packed = nullptr;
    int32_t packedSize = 0;
    int32_t packer[4] = { 0x91, 0, 0, (int32_t)(uintptr_t)s };
    if (Pack_Compress(packer, 0, data, size, &packed, &packedSize) == 1) {
        hook::Global<int32_t>(0x6cd934) += packedSize;
        send(net, 0, packed, (uint32_t)packedSize, type, 0xff, to, sendFlags | idFlag | 0x80000000, timeout, serial, now);
        if (packed) ((void(__cdecl*)(void*))0x5a1145)(packed);
    } else {
        hook::Global<int32_t>(0x6cd934) += size;
        send(net, 0, data, (uint32_t)size, type, 0xff, to, sendFlags | idFlag, timeout, serial, now);
    }
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    hook::Global<LockFn>(0x5c8074)((LONG volatile*)0x6cd960);
}
RT3_REPLACE(0x469d30, Net_Send);

uint8_t __stdcall Net_MsgKind(const uint8_t* msg);
int32_t __fastcall Msg_Queue(void*, int, int32_t key, uint32_t* msg, uint8_t* ctx);
void __fastcall NetPacket_Free(uint8_t* s, int, uint8_t* p);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);

// 0x46ac00. Hands message msg to the handler registered for its kind (hash +4;
// none: dropped) with arg. A packed message (header +0xb bit 1) is unpacked
// first and its payload / size restored afterwards (dropped when unpacking
// fails). A multi-part one (bit 2) goes as an unpacked copy through Msg_Queue,
// keyed by its first payload word, and is handled (key kept in +0x9888) and freed
// only when that does not queue it.
RT3_NOINLINE void __fastcall Net_Dispatch(uint8_t* s, int, uint8_t* msg, uint32_t arg) {
    typedef void(__fastcall * Handler)(uint8_t*, uint32_t);
    Handler h = (Handler)(uintptr_t)Hash_Get(F<HashMap*>(s, 4), 0, Net_MsgKind(msg));
    if (!h) return;
    uint8_t flags = F<uint8_t*>(msg, 4)[0xb];
    if (flags & 1) {
        int32_t savedSize = F<int32_t>(msg, 0xc);
        uint8_t* out = nullptr;
        int32_t outSize = 0;
        int32_t packer[3] = { 0, 0, 0 };
        if (Pack_Decompress(packer, 0, F<const int32_t*>(msg, 8), 0, &out, &outSize) == 1) {
            F<uint8_t*>(msg, 8) = out;
            F<int32_t>(msg, 0xc) = outSize;
            if (F<uint8_t*>(msg, 4)[0xb] & 2) {
                int32_t key = *(int32_t*)out;
                uint8_t* copy = NetPacket_Copy(s, 0, msg);
                F<uint8_t*>(copy, 4)[0xb] &= ~1;
                F<uint8_t*>(copy, 4)[0xb] &= ~2;
                if (!Msg_Queue(s, 0, key, (uint32_t*)copy, (uint8_t*)Engine_FirstListEntry())) {
                    F<int32_t>(s, 0x9888) = key;
                    h(copy, arg);
                    NetPacket_Free(s, 0, copy);
                }
            } else {
                h(msg, arg);
            }
            uint8_t* hdr = F<uint8_t*>(msg, 4);
            F<uint8_t*>(msg, 8) = hdr + hdr[0xa];
            F<int32_t>(msg, 0xc) = savedSize;
            ((void(__cdecl*)(void*))0x5a1145)(out);
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
        return;
    }
    if (flags & 2) {
        int32_t key = *F<int32_t*>(msg, 8);
        uint8_t* copy = NetPacket_Copy(s, 0, msg);
        F<uint8_t*>(copy, 4)[0xb] &= ~2;
        if (!Msg_Queue(s, 0, key, (uint32_t*)copy, (uint8_t*)Engine_FirstListEntry())) {
            F<int32_t>(s, 0x9888) = key;
            h(copy, arg);
            NetPacket_Free(s, 0, copy);
        }
        return;
    }
    h(msg, arg);
}
RT3_REPLACE(0x46ac00, Net_Dispatch);

int32_t Net_LockQueue();
void Net_UnlockQueue();
const char* Net_WaitingPlayersText();
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);

// 0x46b0c0. The host starts the game once (+0x8f30) a world exists and every
// connected player is ready (queue nodes flagged 8 in +0x5c have their +0x58
// cleared and don't count): each player record (0x6ceb9c, with a slot +4 and
// +0x1e1 set; the first takes the engine's own name) goes out as a 0x88-byte
// type 0x56 message {slot index +4, name +8}, packed when that works, then the
// empty start message 0xa. Until then the tooltip lists who is still missing.
void __fastcall Net_HostStart(uint8_t* s) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    if (F<int32_t>(s, 0x8f30) || !hook::Global<void*>(0x6cec78)) return;
    bool ready = false;
    if (hook::Global<int32_t>(0x6d40dc)) {
        int32_t waiting = Global6d4100();
        uint8_t* q = (uint8_t*)(uintptr_t)Net_LockQueue();
        for (uint8_t* n = *(uint8_t**)q; n; n = *(uint8_t**)n)
            if (n[0x5c] & 8) {
                F<int32_t>(n, 0x58) = 0;
                --waiting;
            }
        Net_UnlockQueue();
        ready = waiting == 0;
    }
    if (!ready) {
        if (uint8_t* ui = hook::Global<uint8_t*>(0x6d401c)) Ui_ShowTooltip(ui, 0, Net_WaitingPlayersText(), 3, 0, -1);
        return;
    }
    SendToFn send = (SendToFn)&Net_SendTo;
    Collection* players = hook::Global<Collection*>(0x6ceb9c);
    for (int32_t i = 0; i < Collection_Count(players); ++i) {
        uint8_t* p = (uint8_t*)Collection_Get(players, 0, Collection_IdAt(players, 0, (uint32_t)i, 0));
        if (!F<int32_t>(p, 4) || !F<int32_t>(p, 0x1e1)) continue;
        if (i == 0) strcpy((char*)p + 8, (const char*)Engine_FirstListEntry() + 8);
        uint8_t msg[0x88];
        memset(msg, 0, sizeof msg);
        F<int32_t>(msg, 4) = i;
        strcpy((char*)msg + 8, (const char*)p + 8);
        while (*(volatile LONG*)0x6cd960) {
        }
        hook::Global<LockFn>(0x5c8070)((LONG volatile*)0x6cd960);
        hook::Global<int32_t>(0x6cd930) += 0x88;
        uint8_t* out = nullptr;
        int32_t outSize = 0;
        int32_t packer[3] = { 0x91, 0, 0 };
        if (Pack_Compress(packer, 0, msg, 0x88, &out, &outSize) == 1) {
            hook::Global<int32_t>(0x6cd934) += outSize;
            send(hook::Global<uint8_t*>(0x6d40d0), 0, out, (uint32_t)outSize, 0x56, 0xff, nullptr, 0x80000028, 0, 0, 0);
            if (out) ((void(__cdecl*)(void*))0x5a1145)(out);
        } else {
            hook::Global<int32_t>(0x6cd934) += 0x88;
            send(hook::Global<uint8_t*>(0x6d40d0), 0, msg, 0x88, 0x56, 0xff, nullptr, 0x28, 0, 0, 0);
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
        hook::Global<LockFn>(0x5c8074)((LONG volatile*)0x6cd960);
    }
    F<int32_t>(s, 0x8f30) = 1;
    while (*(volatile LONG*)0x6cd960) {
    }
    hook::Global<LockFn>(0x5c8070)((LONG volatile*)0x6cd960);
    send(hook::Global<uint8_t*>(0x6d40d0), 0, nullptr, 0, 0xa, 0xff, nullptr, 8, 0, 0, 0);
    hook::Global<LockFn>(0x5c8074)((LONG volatile*)0x6cd960);
}
RT3_REPLACE(0x46b0c0, Net_HostStart);

// 0x407870. Sends message type (size bytes at data) to the player named at
// +0x8f48, reliable (flags 5).
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data) {
    Net_Send(s, 0, (const char*)s + 0x8f48, type, size, data, 5, 0);
}
RT3_REPLACE(0x407870, Net_SendToHost);

uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
uint32_t __fastcall Obj_Map9050Insert(void* o, int, uint32_t key, int32_t value);

// 0x4ee4c0. Sends the empty message 0x69 to everyone (reliable from the host).
void Net_SendReady() {
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, 0x69, 0, nullptr, hook::Global<int32_t>(0x6d40dc) ? 3 : 1, 0);
}
RT3_REPLACE(0x4ee4c0, Net_SendReady);

// 0x471070..0x4711ea body. Stamps the session (+0x8f38) with the date of its
// first pending message part (0 without one) and relays msg's payload to
// everyone as type, with the session id (flags 0x19).
static int32_t Net_RelayStamped(const uint8_t* msg, uint32_t type) {
    const uint8_t* data = F<const uint8_t*>((void*)msg, 8);
    uint8_t* part = Net_CountMessagePart(hook::Global<uint8_t*>(0x6cd8d8), 0, 1);
    uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, type, F<int32_t>((void*)msg, 0xc), data, 0x19, 0);
    return 1;
}
int32_t __fastcall Net_Relay1e(const uint8_t* msg) { return Net_RelayStamped(msg, 0x1e); }
RT3_REPLACE(0x471070, Net_Relay1e);
int32_t __fastcall Net_Relay20(const uint8_t* msg) { return Net_RelayStamped(msg, 0x20); }
RT3_REPLACE(0x4710d0, Net_Relay20);
int32_t __fastcall Net_Relay22(const uint8_t* msg) { return Net_RelayStamped(msg, 0x22); }
RT3_REPLACE(0x471130, Net_Relay22);
int32_t __fastcall Net_Relay24(const uint8_t* msg) { return Net_RelayStamped(msg, 0x24); }
RT3_REPLACE(0x471190, Net_Relay24);

// 0x46cbf0 / 0x46cc60 body. Clears the last four bytes (the checksum) of msg's
// n-byte payload, files a heap copy of it in the session's map +0x9050 under its
// CRC and sends it reliably to everyone as type.
static int32_t Net_SendFiled(const uint8_t* msg, uint32_t n, uint32_t clear, uint32_t type) {
    uint8_t* data = F<uint8_t*>((void*)msg, 8);
    F<uint32_t>(data, clear) = 0;
    uint8_t* copy = (uint8_t*)ExeNew(n);
    memcpy(copy, data, n);
    uint32_t crc = Crc32(copy, n);
    Obj_Map9050Insert(hook::Global<uint8_t*>(0x6cd8d8), 0, crc, (int32_t)(uintptr_t)copy);
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, type, F<int32_t>((void*)msg, 0xc), data, 1, 0);
    return 1;
}
int32_t __fastcall Net_SendFiled48(const uint8_t* msg) { return Net_SendFiled(msg, 0xc, 8, 0x48); }
RT3_REPLACE(0x46cbf0, Net_SendFiled48);
int32_t __fastcall Net_SendFiled4c(const uint8_t* msg) { return Net_SendFiled(msg, 0xa, 6, 0x4c); }
RT3_REPLACE(0x46cc60, Net_SendFiled4c);

uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep);
void __fastcall CmdQueue_Clear(uint8_t* q);

// 0x473b40. Sends the n (0x6ce9a0) pending 18-byte records at 0x6cd990 to the
// player named at +0x8f48 as message 0x71 {word 0, word n, records}, reliably,
// and clears them.
void Net_SendPendingRecords() {
    int32_t n = hook::Global<int32_t>(0x6ce9a0);
    if (n <= 0) return;
    int32_t size = n * 0x12 + 4;
    uint8_t* buf = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)((size_t)size);
    F<uint16_t>(buf, 0) = 0;
    F<uint16_t>(buf, 2) = (uint16_t)hook::Global<int32_t>(0x6ce9a0);
    memcpy(buf + 4, (const void*)0x6cd990, (size_t)hook::Global<int32_t>(0x6ce9a0) * 0x12);
    uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
    Net_Send(s, 0, (const char*)s + 0x8f48, 0x71, size, buf, 5, 0);
    hook::Global<int32_t>(0x6ce9a0) = 0;
    ((void(__cdecl*)(void*))0x5a1145)(buf);
}
RT3_REPLACE(0x473b40, Net_SendPendingRecords);

// 0x46aeb0. Dispatches the first engine list entry's queued messages (heap +0x6c,
// count +0x70) while the heap's top key equals the count: each is popped, handed
// to Net_Dispatch with its sender's record, its packet freed and the message kept
// on a local list that is cleared at the end.
void __fastcall Net_DrainQueue(uint8_t* s, int, int32_t) {
    void* list[3] = { nullptr, nullptr, nullptr };
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    if (e) {
        for (int32_t top = Heap_TopKey(F<MaxHeap*>(e, 0x6c)); top; top = Heap_TopKey(F<MaxHeap*>(e, 0x6c))) {
            if (top != F<int32_t>(e, 0x70)) break;
            uint8_t* m = (uint8_t*)(uintptr_t)Heap_PopMax(F<MaxHeap*>(e, 0x6c));
            --F<int32_t>(e, 0x70);
            uint8_t* from = Textures_Lookup(m, 0);
            Net_Dispatch(s, 0, m, (uint32_t)(uintptr_t)from);
            NetPacket_Free(s, 0, F<uint8_t*>(m, 4));
            *(void**)m = list[0];
            list[0] = m;
            list[2] = (void*)((uintptr_t)list[2] + 1);
            if (!list[1]) list[1] = m;
        }
    }
    if (list[0]) CmdQueue_Clear((uint8_t*)list);
}
RT3_REPLACE(0x46aeb0, Net_DrainQueue);

void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Obj_Get21(void* o);
int32_t Global6d40e4();

// 0x50d9d0. Marks window w sent (+0x83), shows the busy cursor and sends the
// game settings record 0x6d1a40 (0x4b bytes: engine value 0x6d40e4, world flag
// +0x21, 0x622b0c, three values of 0x62be68 +0x1c.., four options of 0x6cec74,
// the world byte +0x4c74) to the player named at +0x8f48, reliably, as type 0x14.
void __fastcall Net_SendSettings(uint8_t* w) {
    F<int32_t>(w, 0x83) = 1;
    Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 6, 0, 0);
    hook::Global<uint8_t>(0x6d1a44) = (uint8_t)Obj_Get21(hook::Global<void*>(0x6cec78));
    hook::Global<uint8_t>(0x6d1a45) = hook::Global<uint8_t>(0x622b0c);
    hook::Global<int32_t>(0x6d1a40) = Global6d40e4();
    uint8_t* v = hook::Global<uint8_t*>(0x62be68);
    hook::Global<uint32_t>(0x6d1a62) = F<uint32_t>(v, 0x1c);
    hook::Global<uint32_t>(0x6d1a66) = F<uint32_t>(v, 0x20);
    hook::Global<uint32_t>(0x6d1a6a) = F<uint32_t>(v, 0x24);
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    hook::Global<uint8_t>(0x6d1a86) = F<uint8_t>(opt, 0x140);
    hook::Global<uint8_t>(0x6d1a87) = F<uint8_t>(opt, 0x13c);
    hook::Global<uint8_t>(0x6d1a88) = F<uint8_t>(opt, 0x138);
    hook::Global<uint8_t>(0x6d1a89) = F<uint8_t>(opt, 0x144);
    hook::Global<uint8_t>(0x6d1a8a) = F<uint8_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c74);
    uint8_t* s = hook::Global<uint8_t*>(0x6cd8d8);
    hook::Global<uint32_t>(0x6d1a82) = 0;
    Net_Send(s, 0, (const char*)s + 0x8f48, 0x14, 0x4b, (const uint8_t*)0x6d1a40, 5, 0);
}
RT3_REPLACE(0x50d9d0, Net_SendSettings);

// 0x434200. Tells the network player of slot (player record slot + 1 in
// 0x6ceb9c) its slot number (type 0x55, reliable): by name match (case-blind)
// in the engine's player list, or, with 0x6ce9bc clear, the pick-th entry,
// whose name then also becomes the record's. 1 when sent.
int32_t __stdcall Net_SendSlot(int32_t slot, int32_t pick) {
    char name[0x100];
    name[0] = 0;
    uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)(slot + 1));
    uint8_t* n = *(uint8_t**)(uintptr_t)Net_LockQueue();
    for (int32_t i = 0; n; n = *(uint8_t**)n, ++i) {
        if (!hook::Global<int32_t>(0x6ce9bc)) {
            if (i != pick) continue;
            strcpy(name, (const char*)n + 8);
            strcpy((char*)p + 8, name);
            break;
        }
        if (((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)p + 8, (const char*)n + 8) == 0) {
            strcpy(name, (const char*)n + 8);
            break;
        }
    }
    Net_UnlockQueue();
    if (!name[0]) return 0;
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, name, 0x55, 4, (const uint8_t*)&slot, 1, 0);
    return 1;
}
RT3_REPLACE(0x434200, Net_SendSlot);

const char* __fastcall Options_SaveFolder(void* opt);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);
int32_t FirstFreeSlot6ce2e8();
void File_Size();
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
void __fastcall Session_Restart(uint8_t* w, int, int32_t host);

// 0x46d640. The host offers the resync save 0x6ce630 (in the save folder) to the
// other players: a free transfer slot (0x6ce290, 0x5c bytes, 10 = none) gets
// {engine value, 0, name +8, CRC +0x48, open +0x4c, size +0x50, file +0x58} and
// goes out reliably as message 0x7f. Nothing without other players or the file.
void Net_OfferResync() {
    if (!hook::Global<int32_t>(0x6d40dc) || Global6d4100() <= 1) return;
    int32_t slot = FirstFreeSlot6ce2e8();
    if (slot == 10) return;
    uint8_t* rec = (uint8_t*)0x6ce290 + slot * 0x5c;
    memset(rec, 0, 0x5c);
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x6c) = 0;
    char path[0x12c];
    FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::Global<void*>(0x6cec74)), (const char*)0x6ce630, 0, 0, 0, 0,
               0, 0);
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ce2c0);
    if (!f) return;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    F<int32_t>(rec, 0x4c) = 1;
    int32_t bytes;
    void* fn = (void*)&File_Size;
    char* p = path;
    __asm {
        mov eax, p
        call fn
        mov bytes, eax
    }
    F<int32_t>(rec, 0x50) = bytes;
    F<uint32_t>(rec, 0x48) = Crc32String((const char*)0x6ce630, 1);
    F<int32_t>(rec, 0) = Global6d40e4();
    F<int32_t>(rec, 4) = 0;
    F<void*>(rec, 0x58) = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c9678);
    strcpy((char*)rec + 8, (const char*)0x6ce630);
    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, 0x7f, 0x5c, rec, 3, 0);
}
RT3_REPLACE(0x46d640, Net_OfferResync);

// 0x4ee540. Refills the lobby's player list (0xe57 in the game browser w+0x7c ==
// 0xe13, else 0xe35; guarded by 0x6d1290): the browser without a session asks
// GameSpy (GameList_Refresh), otherwise the connected players are listed. Empty
// without a session it restarts it; outside the browser it then sends this
// player's ready bit (engine entry +0x5c bit 0) as message 9.
void __fastcall Lobby_RefreshPlayers(uint8_t* w) {
    if (hook::Global<uint8_t>(0x6d1290)) return;
    hook::Global<uint8_t>(0x6d1290) = 1;
    int32_t list = F<int32_t>(w, 0x7c) == 0xe13 ? 0xe57 : 0xe35;
    int32_t n = 0;
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6) > -1) TreeView_SelectedData(Widget_FindChild(w, 0, list));
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d2)) Window_SendEvent5(w, 0, 0x8b, (uint32_t)list, 0, 0, 0);
    if (list == 0xe57 && !hook::Global<int32_t>(0x6cd920)) {
        n = GameList_Refresh(w);
    } else {
        uint32_t names[0x10];
        memset(names, 0, sizeof names);
        for (uint8_t* p = *(uint8_t**)(uintptr_t)Net_LockQueue(); p; p = *(uint8_t**)p) names[n++] = F<uint32_t>(p, 4);
        Net_UnlockQueue();
        for (int32_t i = 0; i < n; ++i) Window_SendEvent5(w, 0, 0x88, (uint32_t)list, 0, names[i], 0);
    }
    if (!n && !hook::Global<int32_t>(0x6cd920)) {
        Session_Restart(w, 0, F<int32_t>(hook::Global<uint8_t*>(0x6cd8d8), 0x985c));
        hook::Global<uint8_t>(0x6d1290) = 0;
        return;
    }
    if (F<int32_t>(w, 0x7c) != 0xe13) {
        uint8_t ready = F<uint8_t>(Engine_FirstListEntry(), 0x5c) & 1;
        Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, 9, 1, &ready, 3, 0);
    }
    hook::Global<uint8_t>(0x6d1290) = 0;
}
RT3_REPLACE(0x4ee540, Lobby_RefreshPlayers);

int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes);
const char* __fastcall GetString(int32_t id);
char* __stdcall Net_PlayerNameById(int32_t id);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

// 0x46d300. Feeds this engine's resync transfers (the slots of 0x6ce290 opened
// by it): while the send queue holds under 25 messages, up to 25 chunks of 3840
// bytes go out as message 0x80 {crc, length, data} (unless the transfer is
// addressed, +4) and are counted in +0x54; at the end of the file the end mark
// 0x81 {crc} follows and the slot is freed. Open transfers show their progress
// (string 0x10f5: file, receiver or 0x5ca2bc, percent). Keeps s in 0x6ce9cc.
void __fastcall Net_FeedResync(uint8_t* s) {
    if (Global6d40e4()) {
        for (uint32_t off = 0; off < 0x398; off += 0x5c) {
            uint8_t* rec = (uint8_t*)0x6ce290 + off;
            if (F<int32_t>(rec, 0) != Global6d40e4()) continue;
            int32_t queued = 0, bytes = 0;
            if (Net_SendQueueTotals(hook::Global<uint8_t*>(0x6d40d0), 0, &queued, &bytes) && (uint32_t)queued < 0x19) {
                for (int32_t chunk = 0;;) {
                    struct {
                        uint32_t crc;
                        uint16_t len;
                        uint8_t data[0xf00];
                    } pkt;
                    pkt.crc = F<uint32_t>(rec, 0x48);
                    int32_t n = (int32_t)((size_t(__cdecl*)(void*, size_t, size_t, void*))0x5a1b05)(pkt.data, 1, 0xf00,
                                                                                                  F<void*>(rec, 0x58));
                    if (n && !F<int32_t>(rec, 4)) {
                        pkt.len = (uint16_t)n;
                        Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, 0x80, n + 6, (const uint8_t*)&pkt, 3, 0);
                        F<int32_t>(rec, 0x54) += n;
                    }
                    if (n == 0xf00) {
                        if (++chunk < 0x19) continue;
                        break;
                    }
                    uint32_t crc = F<uint32_t>(rec, 0x48);
                    Net_Send(hook::Global<uint8_t*>(0x6cd8d8), 0, nullptr, 0x81, 4, (const uint8_t*)&crc, 3, 0);
                    ((int(__cdecl*)(void*))0x5a1740)(F<void*>(rec, 0x58));
                    memset(rec, 0, 0x5c);
                    break;
                }
            }
            if (!hook::Global<uint8_t*>(0x6d401c) || !F<void*>(rec, 0x58)) continue;
            const char* who = F<int32_t>(rec, 4) ? Net_PlayerNameById(F<int32_t>(rec, 4)) : (const char*)0x5ca2bc;
            float sent = (float)F<int32_t>(rec, 0x54);
            float total = (float)F<int32_t>(rec, 0x50);
            double scale = *(const float*)0x5c8c28;
            int32_t pct;
            __asm {
                fld sent
                fdiv total
                fmul scale
                mov eax, 0x5a10d0
                call eax
                mov pct, eax
            }
            char num[0x130];
            const char* pctText = Num_Format(num, 1, (double)pct, 0xa, 1, 0, 0);
            char line[0xf00];
            FormatText(line, GetString(0x10f5), (const char*)rec + 8, who, pctText, 0, 0, 0, 0, 0);
            Status_SetMessage(hook::Global<uint8_t*>(0x6d401c), 0, line);
        }
    }
    hook::Global<uint8_t*>(0x6ce9cc) = s;
}
RT3_REPLACE(0x46d300, Net_FeedResync);

uint8_t* __fastcall Obj_BuildStatusPacket(uint8_t* o, int, int32_t* outSize);
void __stdcall Table9_Append(uint8_t* o, const uint8_t* src);
uint8_t* __fastcall Net_FindFlaggedPlayer();
uint8_t* __stdcall Table9_Find(uint8_t* o, const uint8_t* e);
uint32_t __fastcall Obj_FindNode8f28(void* o, int, int32_t key);
void __stdcall Table9_Prune(uint8_t* o, uint32_t limit);
uint8_t* __stdcall Textures_Find(int32_t key);
void __stdcall Net_UnlockQueueArg(int32_t);

// 0x46b360. The weekly (world day byte +0x11 % 7 == 1) state check of a started
// game (+0x8f30, 0x6cd8ec): the host packs its status (Obj_BuildStatusPacket,
// timed into 0x6ae4cc when 0x6cd900) and sends it as message 0xd. A client
// collects its own 9-byte entries from the status sources (+0x8f28, cleared +0xc)
// and compares them with the host's (the flagged player's table, +0x178 entries
// from +0x180 in block +0x17c): each slot with both checksums set is marked in
// 0x6cd908; a mismatch flags the slot's source (+0x98) and, unless reporting is
// muted (0x6cd91c without 0x6cd918), goes straight back as message 0x4f. Both
// tables are then pruned to the date.
// QUIRK: a slot without a matching source clears +0x98 of a null pointer.
void __fastcall Net_WeeklyCheck(uint8_t* s) {
    typedef LONG(WINAPI * LockFn)(LONG volatile*);
    hook::Global<uint32_t>(0x6cd908) = 0;
    uint8_t* world = hook::Global<uint8_t*>(0x6cec78);
    if (!world || F<int32_t>(s, 0x8f30) != 1 || hook::Global<int32_t>(0x6cd8ec) != 1) return;
    if ((int32_t)world[0x11] % 7 != 1) return;
    uint32_t date = (uint32_t)DateToMinutes((const GameDate*)(world + 0xd));
    bool timed = hook::Global<int32_t>(0x6cd900) == 1;
    int32_t t0 = 0;
    if (hook::Global<int32_t>(0x6d40dc)) {
        if (Global6d4100() <= 1) return;
        if (timed) t0 = ClockMs();
        int32_t size = 0;
        uint8_t* pkt = Obj_BuildStatusPacket(s, 0, &size);
        if (hook::Global<int32_t>(0x6cd900) == 1) hook::Global<int32_t>(0x6ae4cc) = ClockMs() - t0;
        Net_Send(s, 0, nullptr, 0xd, size, pkt, 2, 0);
        if (pkt) ExeDelete(pkt);
        hook::Global<int32_t>(0x6cd918) = 0;
        return;
    }
    uint8_t* e = (uint8_t*)Engine_FirstListEntry();
    if (!e) return;
    if (timed) t0 = ClockMs();
    for (uint8_t* n = *F<uint8_t**>(s, 0x8f28); n; n = *(uint8_t**)n) {
        F<int32_t>(n, 0xc) = 0;
        if (const uint8_t* r = ((const uint8_t*(*)())F<void*>(n, 4))()) Table9_Append(e, r);
    }
    if (hook::Global<int32_t>(0x6cd900) == 1) hook::Global<int32_t>(0x6ae4cc) = ClockMs() - t0;
    uint8_t* q = Net_FindFlaggedPlayer();
    const uint8_t* rec = q + F<int32_t>(q, 0x17c) * 0x1200 + 0x180;
    uint8_t* own = (uint8_t*)Engine_FirstListEntry();
    for (int32_t i = 0; i < F<int32_t>(q, 0x178); ++i, rec += 9) {
        uint8_t* p = Table9_Find(own, rec);
        if (!p) continue;
        F<uint32_t>(s, 0x8f44) = F<uint32_t>((void*)rec, 5);
        volatile uint8_t* src = nullptr;
        if (void** list = F<void**>(s, 0x8f28)) {
            for (uint8_t* n = (uint8_t*)*list; n; n = *(uint8_t**)n)
                if (F<uint32_t>(n, 4) == Obj_FindNode8f28(s, 0, p[0])) {
                    src = n;
                    break;
                }
        }
        *(volatile int32_t*)(src + 0x98) = 0;
        if (!F<int32_t>(p, 1) || !F<int32_t>((void*)rec, 1)) continue;
        hook::Global<uint32_t>(0x6cd908) |= 1u << (p[0] & 0x1f);
        if (F<int32_t>(p, 1) == F<int32_t>((void*)rec, 1)) continue;
        *(volatile int32_t*)(src + 0x98) = 1;
        if (hook::Global<int32_t>(0x6cd91c) && hook::Global<int32_t>(0x6cd918) != 1) continue;
        while (*(volatile LONG*)0x6cd960) {
        }
        hook::Global<LockFn>(0x5c8070)((LONG volatile*)0x6cd960);
        hook::Global<int32_t>(0x6cd930) += 9;
        hook::Global<int32_t>(0x6cd934) += 9;
        ((SendToFn)&Net_SendTo)(hook::Global<uint8_t*>(0x6d40d0), 0, p, 9, 0x4f, 0xff, (const char*)q + 8, 8, 0, 0, 0);
        hook::Global<LockFn>(0x5c8074)((LONG volatile*)0x6cd960);
        if (hook::Global<int32_t>(0x5f2b3c)) hook::Global<int32_t>(0x5f2b3c) = 0;
        if (hook::Global<int32_t>(0x6cd918) == 1) hook::Global<int32_t>(0x6cd91c) = 1;
    }
    if (hook::Global<uint32_t>(0x6cd908)) hook::Global<int32_t>(0x6cd918) = 0;
    Table9_Prune(own, date);
    Table9_Prune(Textures_Find(F<int32_t>(q, 4)), date);
    Net_UnlockQueueArg(F<int32_t>(q, 4));
}
RT3_REPLACE(0x46b360, Net_WeeklyCheck);

} // namespace rt3
