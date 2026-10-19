#include "hook.h"
#include <windows.h>
#include <stdio.h>

namespace hook {

static Entry* g_head;

Registrar::Registrar(uint32_t addr, void* impl, const char* name) {
    static Entry pool[8192];
    static int used;
    Entry* e = &pool[used++];
    e->addr = addr; e->impl = impl; e->name = name; e->next = g_head;
    g_head = e;
}

static bool WriteJmp(uint32_t at, void* to) {
    DWORD old;
    uint8_t* p = reinterpret_cast<uint8_t*>(at);
    if (!VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &old)) return false;
    p[0] = 0xE9;
    *reinterpret_cast<int32_t*>(p + 1) = int32_t(uintptr_t(to) - (at + 5));
    VirtualProtect(p, 5, old, &old);
    FlushInstructionCache(GetCurrentProcess(), p, 5);
    return true;
}

// Coverage (RT3RE_COVERAGE set): each replacement is entered through a stub
// "lock inc [counter]; jmp impl", so we know which rewrites actually ran.
struct Counted { Entry* e; volatile LONG calls; };
static Counted g_counted[8192];
static int g_numCounted;

static void* MakeCountingStub(Entry* e) {
    static uint8_t* code;
    static size_t used, cap;
    if (!code || used + 16 > cap) {
        cap = 64 * 1024;
        code = (uint8_t*)VirtualAlloc(nullptr, cap, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        used = 0;
    }
    Counted* c = &g_counted[g_numCounted++];
    c->e = e;
    c->calls = 0;
    uint8_t* s = code + used;
    s[0] = 0xF0; s[1] = 0xFF; s[2] = 0x05;               // lock inc dword ptr [abs32]
    *(uint32_t*)(s + 3) = (uint32_t)(uintptr_t)&c->calls;
    s[7] = 0xE9;                                         // jmp impl
    *(int32_t*)(s + 8) = (int32_t)((uintptr_t)e->impl - (uintptr_t)(s + 12));
    used += 16;
    return s;
}

int InstallAll(const char* iniPath) {
    bool coverage = GetEnvironmentVariableA("RT3RE_COVERAGE", nullptr, 0) != 0;
    int n = 0;
    for (Entry* e = g_head; e; e = e->next) {
        // Default on; "name=0" under [replace] keeps the original.
        if (!GetPrivateProfileIntA("replace", e->name, 1, iniPath)) continue;
        if (WriteJmp(e->addr, coverage ? MakeCountingStub(e) : e->impl)) ++n;
    }
    return n;
}

void DumpCoverage(const char* path) {
    FILE* f = fopen(path, "w");
    if (!f) return;
    for (int i = 0; i < g_numCounted; ++i)
        fprintf(f, "%08x %s %ld\n", g_counted[i].e->addr, g_counted[i].e->name, g_counted[i].calls);
    fclose(f);
}

} // namespace hook
