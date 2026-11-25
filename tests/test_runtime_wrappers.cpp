// Callback-observation fixtures for three GPT6.1 SOL MEDIUM runtime wrappers.
#include "difftest.h"
#include "hook/hook.h"
#include <windows.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {
void __fastcall Map_RuntimeDestructor(uint8_t*);
uint8_t* __fastcall CarTypes_ConstructRuntime(uint8_t*);
void __fastcall ModelObj_SelectTrackTool(uint8_t*);
}
namespace {
template <typename T> T& F(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
struct Redirects {
    struct Patch { uint8_t* at; uint8_t old[5]; } entries[16];
    int count = 0;
    void AddOne(uintptr_t addr, void* target) {
        Patch& p = entries[count++]; p.at = (uint8_t*)addr; DWORD old;
        VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(p.old, p.at, 5); p.at[0] = 0xe9;
        F<int32_t>(p.at, 1) = (int32_t)((uintptr_t)target - addr - 5);
        VirtualProtect(p.at, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), p.at, 5);
    }
    // Original code calls the exe address, rewritten code calls the rewrite
    // directly: a redirect covers both.
    void Add(uintptr_t addr, void* target) {
        AddOne(addr, target);
        if (void* r = hook::Find((uint32_t)addr)) AddOne((uintptr_t)r, target);
    }
    ~Redirects() {
        while (count) {
            Patch& p = entries[--count]; DWORD old;
            VirtualProtect(p.at, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(p.at, p.old, 5);
            VirtualProtect(p.at, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p.at, 5);
        }
    }
};
struct Fixture {
    uint8_t object[0x300], other[0x300], views[2][0x40];
    uint32_t log[128], used, view, result;
};
Fixture g_f;
uint32_t g_bits;
int32_t g_pick;
void* g_vtable[32];
uint32_t Id(void* p) {
    return p == g_f.object ? 1 : p == g_f.other ? 2 :
           p == g_f.views[0] ? 3 : p == g_f.views[1] ? 4 : (uint32_t)(uintptr_t)p;
}
void Observe(uint32_t address, void* self) {
    uint32_t* p = g_f.log + g_f.used;
    p[0] = address; p[1] = Id(self);
    p[2] = F<uint32_t>(g_f.object, 0);
    p[3] = F<uint32_t>(g_f.object, 4);
    p[4] = F<uint32_t>(g_f.object, 0x2a4);
    p[5] = F<uint32_t>(g_f.object, 0x2a8);
    p[6] = Id(*(void**)0x6d0818);
    g_f.used += 7;
}
uint8_t* __fastcall Unload(uint8_t* self) {
    Observe(0x449270, self);
    F<uint32_t>(self, 0) = g_bits;
    F<uint32_t>(self, 4) ^= 0x11223344;
    return g_f.other;
}
void __fastcall BaseDestructor(uint8_t* self) {
    Observe(0x537e30, self);
    F<uint32_t>(self, 8) ^= 0x55667788;
}
uint8_t* __fastcall SetVtbl(uint8_t* self) {
    Observe(0x517ce0, self);
    F<uint32_t>(self, 0) = g_bits;
    F<uint32_t>(self, 4) ^= 0x1234;
    return g_f.other;
}
uint32_t __fastcall Init(uint8_t* self, int, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t h) {
    Observe(0x518570, self);
    const int32_t args[] = {a, b, c, d, e, f, h};
    for (int32_t v : args) g_f.log[g_f.used++] = (uint32_t)v;
    F<uint32_t>(self, 0) ^= 0x80000000;
    F<uint32_t>(self, 4) += g_bits;
    return g_bits;
}
uint8_t* __fastcall LoadAll(uint8_t* self) {
    Observe(0x41d7c0, self);
    F<uint32_t>(self, 0x40) = g_bits;
    return g_f.other;
}
int32_t __fastcall Pick(uint8_t* self) {
    Observe(0x700000, self);
    F<uint32_t>(self, 0x2a4) ^= 0x80000000;
    F<uint32_t>(self, 0x2a8) += g_bits;
    if (g_bits & 1) *(void**)0x6d0818 = g_f.views[1];
    if (g_bits & 2) *(void**)0x6d0818 = nullptr;
    return g_pick;
}
void __fastcall Mode(void* self, int, int32_t mode, int32_t id) {
    Observe(0x4ddbd0, self);
    g_f.log[g_f.used++] = (uint32_t)mode;
    g_f.log[g_f.used++] = (uint32_t)id;
    F<uint32_t>(g_f.object, 4) ^= (uint32_t)mode;
    F<uint32_t>(g_f.object, 0x2a4) = (uint32_t)id;
    *(void**)0x6d0818 = g_f.views[0];
}
void __fastcall Select(uint8_t* self) {
    Observe(0x45b400, self);
    F<uint32_t>(self, 0x40) ^= 0xabcdef01;
}
void Cases(int kind, void* original, void* rewrite) {
    uint32_t saved = *(uint32_t*)0x6d0818;
    for (uint32_t it = 0; it < 2048; ++it) {
        Fixture expected;
        for (int side = 0; side < 2; ++side) {
            memset(&g_f, 0, sizeof g_f);
            uint32_t r = it + 0x26a09;
            for (uint8_t* p = (uint8_t*)&g_f; p != (uint8_t*)g_f.log; ++p) {
                r = r * 1664525u + 1013904223u;
                *p = (uint8_t)(r >> 16);
            }
            g_bits = r;
            const int32_t picks[] = {0, 1, -1, (int32_t)0x80000000, 0x10000};
            g_pick = picks[it % 5];
            if (kind == 2) F<void**>(g_f.object, 0) = g_vtable;
            *(void**)0x6d0818 = g_f.views[0];
            void* fn = side ? rewrite : original;
            if (kind == 1) g_f.result = Id(((uint8_t*(__fastcall*)(uint8_t*))fn)(g_f.object));
            else ((void(__fastcall*)(uint8_t*))fn)(g_f.object);
            g_f.view = Id(*(void**)0x6d0818);
            if (!side) expected = g_f;
            else if (memcmp(&expected, &g_f, sizeof g_f)) {
                size_t off = 0;
                while (((uint8_t*)&expected)[off] == ((uint8_t*)&g_f)[off]) ++off;
                EXPECT_EQ(1, 0, "wrapper %d case %u byte %u (%02x/%02x), events %u/%u",
                          kind, it, (unsigned)off, ((uint8_t*)&expected)[off],
                          ((uint8_t*)&g_f)[off], expected.used, g_f.used);
                *(uint32_t*)0x6d0818 = saved;
                return;
            }
        }
    }
    *(uint32_t*)0x6d0818 = saved;
}
}
DIFFTEST(game_MapRuntimeDestructor) {
    Redirects p;
    p.Add(0x449270, (void*)&Unload); p.Add(0x537e30, (void*)&BaseDestructor);
    Cases(0, (void*)0x449500, (void*)&rt3::Map_RuntimeDestructor);
}
DIFFTEST(game_CarTypesConstructRuntime) {
    Redirects p;
    p.Add(0x517ce0, (void*)&SetVtbl); p.Add(0x518570, (void*)&Init); p.Add(0x41d7c0, (void*)&LoadAll);
    Cases(1, (void*)0x41e170, (void*)&rt3::CarTypes_ConstructRuntime);
}
DIFFTEST(game_ModelObjSelectTrackTool) {
    Redirects p;
    p.Add(0x4ddbd0, (void*)&Mode); p.Add(0x45b400, (void*)&Select);
    g_vtable[0x70 / 4] = (void*)&Pick;
    Cases(2, (void*)0x40d120, (void*)&rt3::ModelObj_SelectTrackTool);
}
