#include "hook.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace hook {

static Entry* g_head;

Registrar::Registrar(uint32_t addr, void* impl, const char* name) {
    static Entry pool[8192];
    static int used;
    Entry* e = &pool[used++];
    e->addr = addr; e->impl = impl; e->name = name; e->next = g_head;
    g_head = e;
}

void* Find(uint32_t addr) {
    for (Entry* e = g_head; e; e = e->next)
        if (e->addr == addr) return e->impl;
    return nullptr;
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

// Coverage of the original code (a replacement switched off): a one-shot INT3
// on the function's first byte. The first time it runs, the handler puts the
// byte back and resumes at the same address, so the original runs unchanged.
// Unlike the counting stubs, this also sees calls that rewritten code makes
// directly (they never pass the exe address), so the original run is where
// coverage is measured.
struct Probe { Entry* e; uint8_t byte; volatile LONG hit; };
static Probe g_probes[8192];
static int g_numProbes;

static void Unprobe(Probe* p) {
    uint8_t* at = reinterpret_cast<uint8_t*>(p->e->addr);
    DWORD old;
    VirtualProtect(at, 1, PAGE_EXECUTE_READWRITE, &old);
    if (*at == 0xCC) *at = p->byte;  // still ours (not overwritten since)
    VirtualProtect(at, 1, old, &old);
    FlushInstructionCache(GetCurrentProcess(), at, 1);
}

static LONG CALLBACK ProbeHit(EXCEPTION_POINTERS* ep) {
    if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT) return EXCEPTION_CONTINUE_SEARCH;
    uint32_t at = (uint32_t)(uintptr_t)ep->ExceptionRecord->ExceptionAddress;
    for (int i = 0; i < g_numProbes; ++i) {
        Probe* p = &g_probes[i];
        if (p->e->addr != at) continue;
        InterlockedIncrement(&p->hit);
        Unprobe(p);  // another thread may have done it already: then a no-op
        ep->ContextRecord->Eip = at;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void AddProbe(Entry* e) {
    for (int i = 0; i < g_numProbes; ++i)
        if (g_probes[i].e->addr == e->addr) return;  // aliases share one probe
    uint8_t* at = reinterpret_cast<uint8_t*>(e->addr);
    Probe* p = &g_probes[g_numProbes++];
    p->e = e;
    p->byte = *at;
    p->hit = 0;
    DWORD old;
    VirtualProtect(at, 1, PAGE_EXECUTE_READWRITE, &old);
    *at = 0xCC;
    VirtualProtect(at, 1, old, &old);
    FlushInstructionCache(GetCurrentProcess(), at, 1);
}

// Rewrites of switched-off entries that now start with a JMP to the original,
// with the bytes it replaced (Unredirect).
struct Redirect { void* impl; uint8_t bytes[5]; };
static Redirect g_redirects[8192];
static int g_numRedirects;

void Unredirect(void* impl) {
    for (int i = 0; i < g_numRedirects; ++i) {
        if (g_redirects[i].impl != impl) continue;
        DWORD old;
        VirtualProtect(impl, 5, PAGE_EXECUTE_READWRITE, &old);
        memcpy(impl, g_redirects[i].bytes, 5);
        VirtualProtect(impl, 5, old, &old);
        FlushInstructionCache(GetCurrentProcess(), impl, 5);
        g_redirects[i].impl = nullptr;
    }
}

// Can the rewrite of a switched-off entry start with a JMP to the original?
// Not when another, switched-on entry shares the rewrite, nor when the rewrite
// might be shorter than the 5-byte JMP: a return or other jump (or padding)
// in its first five bytes leaves it alone (direct calls then still reach it).
static bool SafeToRedirect(Entry* off, const char* iniPath) {
    for (Entry* e = g_head; e; e = e->next)
        if (e != off && e->impl == off->impl && GetPrivateProfileIntA("replace", e->name, 1, iniPath)) return false;
    const uint8_t* p = reinterpret_cast<const uint8_t*>(off->impl);
    // A JMP rel32 first: an incremental-link thunk (what &fn gives, and what
    // every call goes through) or a rewrite that is one tail jump. Either way
    // exactly these 5 bytes are replaced.
    if (p[0] == 0xE9) return true;
    for (int i = 0; i < 5; ++i)
        if (p[i] == 0xC3 || p[i] == 0xC2 || p[i] == 0xEB || p[i] == 0xE9 || p[i] == 0xFF || p[i] == 0xCC) return false;
    return true;
}

int InstallAll(const char* iniPath) {
    bool coverage = GetEnvironmentVariableA("RT3RE_COVERAGE", nullptr, 0) != 0;
    int n = 0;
    for (Entry* e = g_head; e; e = e->next) {
        // Default on; "name=0" under [replace] keeps the original.
        if (!GetPrivateProfileIntA("replace", e->name, 1, iniPath)) {
            if (coverage) AddProbe(e);
            // Rewritten code calls the rewrite directly: send those calls to the
            // original too, so "off" means off (bisecting with simcheck). The
            // rewrite has the original's calling convention, as the JMP needs.
            if (SafeToRedirect(e, iniPath) && g_numRedirects < 8192) {
                Redirect& r = g_redirects[g_numRedirects++];
                r.impl = e->impl;
                memcpy(r.bytes, e->impl, 5);
                WriteJmp((uint32_t)(uintptr_t)e->impl, reinterpret_cast<void*>(e->addr));
            }
            continue;
        }
        if (WriteJmp(e->addr, coverage ? MakeCountingStub(e) : e->impl)) ++n;
    }
    if (g_numProbes) AddVectoredExceptionHandler(1, ProbeHit);
    return n;
}

// Tracing (RT3RE_TRACE=<hex addr>,...): logs "T <addr> <ret> <ecx>" on every call
// of those exe functions. A breakpoint on the first byte; when hit, the byte goes
// back, one instruction runs single-stepped and the breakpoint is set again.
struct Trace { uint32_t addr; uint8_t byte; };
static Trace g_traces[64];
static int g_numTraces;
static Trace* g_stepping;
void (*g_traceLog)(const char*, ...);

static void PutByte(uint32_t at, uint8_t b) {
    DWORD old;
    VirtualProtect(reinterpret_cast<void*>(at), 1, PAGE_EXECUTE_READWRITE, &old);
    *reinterpret_cast<uint8_t*>(at) = b;
    VirtualProtect(reinterpret_cast<void*>(at), 1, old, &old);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(at), 1);
}

static LONG CALLBACK TraceHit(EXCEPTION_POINTERS* ep) {
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    if (code == EXCEPTION_SINGLE_STEP && g_stepping) {
        PutByte(g_stepping->addr, 0xCC);
        g_stepping = nullptr;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (code != EXCEPTION_BREAKPOINT) return EXCEPTION_CONTINUE_SEARCH;
    uint32_t at = (uint32_t)(uintptr_t)ep->ExceptionRecord->ExceptionAddress;
    for (int i = 0; i < g_numTraces; ++i) {
        if (g_traces[i].addr != at) continue;
        if (g_traceLog) {
            // The first stack arguments, then exe code addresses further up the stack (callers).
            const DWORD* sp = reinterpret_cast<const DWORD*>(ep->ContextRecord->Esp);
            char line[400];
            const CONTEXT* c = ep->ContextRecord;
            int n = wsprintfA(line, "T %06x ret %08lx ecx %08lx edx %08lx args %08lx %08lx %08lx %08lx regs %08lx %08lx %08lx %08lx up", at,
                              sp[0], c->Ecx, c->Edx, sp[1], sp[2], sp[3], sp[4], c->Eax, c->Ebx, c->Esi, c->Edi);
            for (int k = 1, found = 0; k < 400 && found < 10; ++k)
                if (sp[k] >= 0x401000 && sp[k] < 0x5c8000) {
                    n += wsprintfA(line + n, " %06lx", sp[k]);
                    ++found;
                }
            g_traceLog("%s", line);
        }
        PutByte(at, g_traces[i].byte);
        g_stepping = &g_traces[i];
        ep->ContextRecord->Eip = at;
        ep->ContextRecord->EFlags |= 0x100;  // trap flag: one instruction
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void TraceFromEnv(void (*log)(const char*, ...)) {
    char spec[512];
    if (!GetEnvironmentVariableA("RT3RE_TRACE", spec, sizeof spec)) return;
    g_traceLog = log;
    for (char* p = spec; *p && g_numTraces < 64;) {
        uint32_t a = strtoul(p, &p, 16);
        if (a) {
            g_traces[g_numTraces].addr = a;
            g_traces[g_numTraces].byte = *reinterpret_cast<uint8_t*>(a);
            PutByte(a, 0xCC);
            ++g_numTraces;
        }
        if (*p == ',') ++p; else break;
    }
    if (g_numTraces) AddVectoredExceptionHandler(1, TraceHit);
}

void ProbeAll() {
    for (Entry* e = g_head; e; e = e->next) AddProbe(e);
    if (g_numProbes) AddVectoredExceptionHandler(1, ProbeHit);
}

void DumpCoverage(const char* path) {
    FILE* f = fopen(path, "w");
    if (!f) return;
    for (int i = 0; i < g_numCounted; ++i)
        fprintf(f, "%08x %s %ld\n", g_counted[i].e->addr, g_counted[i].e->name, g_counted[i].calls);
    // Original code: 1 if it ran (the probe fires once), for every alias.
    for (Entry* e = g_head; e; e = e->next)
        for (int i = 0; i < g_numProbes; ++i)
            if (g_probes[i].e->addr == e->addr)
                fprintf(f, "%08x %s %ld\n", e->addr, e->name, g_probes[i].hit ? 1L : 0L);
    fclose(f);
}

} // namespace hook
