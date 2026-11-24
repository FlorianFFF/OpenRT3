#include "difftest.h"
#include "game/collection.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void Net_ApplyTrainPosition();
void* __fastcall Obj_City41(void* o);
}

namespace {

// Use the same five-byte entry redirection as test_ui.cpp, but keep this
// fixture independent so concurrent decomp batches do not share test edits.
struct EntryPatch {
    uint8_t* at;
    uint8_t saved[5];
    EntryPatch(uintptr_t address, void* replacement) : at((uint8_t*)address) {
        DWORD old;
        VirtualProtect(at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(saved, at, 5);
        at[0] = 0xe9;
        *(int32_t*)(at + 1) = (int32_t)((uintptr_t)replacement - ((uintptr_t)at + 5));
        VirtualProtect(at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), at, 5);
    }
    ~EntryPatch() {
        DWORD old;
        VirtualProtect(at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(at, saved, 5);
        VirtualProtect(at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), at, 5);
    }
};

struct Trace {
    uint32_t events[48];
    int count;
    uint32_t result;
    uint32_t packet[5];
    uint8_t train[0x100], car[2][0x380];
};

Trace g_trace;
uint32_t g_case;
int g_refs;

void Event(uint32_t a, uint32_t b, uint32_t c) {
    g_trace.events[g_trace.count++] = a;
    g_trace.events[g_trace.count++] = b;
    g_trace.events[g_trace.count++] = c;
}
int CarIndex(const void* car) { return car == g_trace.car[1] ? 1 : 0; }

int32_t __fastcall FakeValid(rt3::Collection* table, int, uint32_t id) {
    Event(1, (uint32_t)(uintptr_t)table, id);
    return (g_case & 1) ? 7 : 0;
}
void* __fastcall FakeGet(rt3::Collection* table, int, uint32_t id) {
    Event(2, (uint32_t)(uintptr_t)table, id);
    return g_trace.train;
}
void* __fastcall FakeFirstCar(void* train) {
    Event(3, train == g_trace.train, g_refs);
    int n = g_refs++;
    if (!(g_case & 2)) return nullptr;
    return g_trace.car[n && (g_case & 4) ? 1 : 0];
}
void __fastcall FakeMove(uint8_t* car, int, uint32_t node, uint32_t position) {
    Event(4, (uint32_t)CarIndex(car), *(uint32_t*)(car + 0x35a));
    Event(5, node, position);
    *(uint32_t*)(car + 0x356) = node;
    *(uint32_t*)(car + 0x35e) = position;
}
int32_t __fastcall FakeAdvance(uint8_t* car, int, uint32_t movement, void* out, void* count, uint32_t last) {
    Event(6, (uint32_t)CarIndex(car), movement);
    Event(7, (uint32_t)(uintptr_t)out, (uint32_t)(uintptr_t)count);
    Event(8, last, *(uint32_t*)(g_trace.train + 0xcc));
    if (g_case & 8) g_trace.packet[1] ^= 0x55aa55aa;
    return (g_case & 16) ? -1 : 0x12345678;
}

uint32_t CallEax(void* entry, const uint32_t* packet) {
    uint32_t result;
    __asm {
        mov eax, packet
        call entry
        mov result, eax
    }
    return result;
}

} // namespace

DIFFTEST(game_NetApplyTrainPosition) {
    EntryPatch validOrig(0x517d40, (void*)&FakeValid);
    EntryPatch validNew((uintptr_t)&rt3::Collection_IsValid, (void*)&FakeValid);
    EntryPatch getOrig(0x518140, (void*)&FakeGet);
    EntryPatch getNew((uintptr_t)&rt3::Collection_Get, (void*)&FakeGet);
    EntryPatch firstOrig(0x4a77b0, (void*)&FakeFirstCar);
    EntryPatch firstNew((uintptr_t)&rt3::Obj_City41, (void*)&FakeFirstCar);
    EntryPatch move(0x41c700, (void*)&FakeMove);
    EntryPatch advance(0x41ce80, (void*)&FakeAdvance);
    uint32_t saved = hook::Global<uint32_t>(0x6cfcbc);
    hook::Global<uint32_t>(0x6cfcbc) = 0x6cfcbc;
    for (uint32_t it = 0; it < 2048; ++it) {
        Trace original;
        for (int side = 0; side < 2; ++side) {
            memset(&g_trace, 0, sizeof g_trace);
            g_case = it;
            g_refs = 0;
            uint32_t r = it * 2654435761u + 461;
            for (int k = 0; k < 5; ++k) {
                r = r * 1103515245u + 12345u;
                g_trace.packet[k] = r;
            }
            memset(g_trace.train, 0x5a, sizeof g_trace.train);
            memset(g_trace.car, 0xa5, sizeof g_trace.car);
            void* entry = side ? (void*)&rt3::Net_ApplyTrainPosition : (void*)0x46d780;
            g_trace.result = CallEax(entry, g_trace.packet);
            if (!side) original = g_trace;
            else EXPECT_EQ(memcmp(&original, &g_trace, sizeof original), 0, "network train position case %u", it);
        }
    }
    hook::Global<uint32_t>(0x6cfcbc) = saved;
}
